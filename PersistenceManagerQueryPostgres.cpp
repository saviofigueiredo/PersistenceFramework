#include "PersistenceManagerQueryPostgres.h"
#include "IPersistenceManager.h"
#include "IPersistenceManagerGet.h"
#include "IPersistence.h"
#include "AttributeValue.h"

namespace PersistenceFramework
{
	PersistenceManagerQueryPostgres::PersistenceManagerQueryPostgres()
	{
		persistenceManager = nullptr;
		managerGet = nullptr;
	}

	void PersistenceManagerQueryPostgres::setPersistenceManager(IPersistenceManager* _persistenceManager)
	{
		persistenceManager = _persistenceManager;
	}
	
	void PersistenceManagerQueryPostgres::setManagerGet(IPersistenceManagerGet* _managerGet)
	{
		managerGet = managerGet;
	}

	std::string PersistenceManagerQueryPostgres::getQueryInsert(IPersistence* _newObject)
	{
		if (!_newObject || persistenceManager == nullptr || managerGet == nullptr)
		{
			return "";
		}

		std::list<std::string> columnsNames;
		_newObject->getColumnNames(columnsNames);
		std::string sql("INSERT INTO ");
		sql.append(_newObject->getTableName());
		sql.append(" (");

		std::string primaryKey = _newObject->getPrimaryKeyAttributeName();

		bool firstElementHasBeenInserted = false;
		for (std::list<std::string>::iterator it = columnsNames.begin(); it != columnsNames.end(); it++)
		{
			if (_newObject->getColumnName(primaryKey) == (*it))
				continue;

			if (firstElementHasBeenInserted == true)
				sql.append(", ");

			sql.append((*it));
			firstElementHasBeenInserted = true;
		}

		sql.append(") VALUES (");

		std::string columnName;
		std::string attributeName;
		AttributeValue* attributeValue;
		firstElementHasBeenInserted = false;
		for (std::list<std::string>::iterator it = columnsNames.begin(); it != columnsNames.end(); it++)
		{
			if (_newObject->getColumnName(primaryKey) == (*it))
				continue;

			if (firstElementHasBeenInserted == true)
				sql.append(", ");

			firstElementHasBeenInserted = true;

			columnName = (*it);
			attributeName = _newObject->getAttributeName(columnName);
			attributeValue = _newObject->getValue(attributeName);

			if (attributeValue == nullptr)
			{
				return "";
			}

			if (attributeValue->getType() == _string || attributeValue->getType() == _varchar || attributeValue->getType() == _char)
			{
				std::string value = attributeValue->getValue();
				if (!fitToDBPalavra(value))
				{
					return "";
				}

				sql.append("'");
				sql.append(value);
				sql.append("'");
			}
			else if (attributeValue->getType() == _int || attributeValue->getType() == _float || attributeValue->getType() == _double || attributeValue->getType() == _long || attributeValue->getType() == _enum)
			{
				if (!fitToDBNumero(attributeValue->getValue()))
				{
					return "";
				}

				sql.append(attributeValue->getValue());
			}
			else if (attributeValue->getType() == _bool)
			{
				if (attributeValue->getValue() == "0")
					sql.append("FALSE");
				else
					sql.append("TRUE");
			}
			else if (attributeValue->getType() == _date)
			{
				if (attributeValue->getValue().size() == 0)
					sql.append("null");
				else
				{
					if (!fitToDBDate(attributeValue->getValue()))
					{
						return "";
					}
					sql.append("'");
					sql.append(attributeValue->getValue());
					sql.append("'");
				}
			}
			else if (attributeValue->getType() == _object)
			{
				auto referencedObject = (*attributeValue->getAddress().ppObject);

				if (referencedObject == NULL)
					sql.append("null");
				else
				{
					persistenceManager->insert(referencedObject);
					std::string referencedObjectColumnName = referencedObject->getColumnName(attributeValue->getNameAttributeReferencedObject());
					sql.append(managerGet->getObjectColumnValue(referencedObject, referencedObjectColumnName));
				}
			}
		}

		sql.append(");");
		return sql;

	}

	std::string PersistenceManagerQueryPostgres::getQueryBiggestValue(const std::string& _tableName, const std::string& _columnName, const std::string& _qualifier)
	{
		if (_tableName == "" || _columnName == "")
		{
			return "";
		}

		std::string sql("SELECT MAX(");

		sql.append(_columnName);
		sql.append(") AS ");
		sql.append(_qualifier);
		sql.append(" FROM ");
		sql.append(_tableName);
		sql.append(";");
		return sql;
	}

	std::string PersistenceManagerQueryPostgres::getQueryUpdatePrimitiveAttribute(const std::string &_tableName, const std::list<std::pair<std::string, std::pair<std::string, int>>>& _attributesNamesValues, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyValue)
	{
		std::string sql("UPDATE ");
		sql.append(_tableName);
		sql.append(" SET ");

		for (std::list<std::pair<std::string, std::pair<std::string, int>>>::const_iterator it = _attributesNamesValues.begin(); it != _attributesNamesValues.end(); ++it)
		{
			std::pair<std::string, std::pair<std::string, int>> mappingAttributesNamesAttributesCharacteristics = (*it);

			std::string columnName = mappingAttributesNamesAttributesCharacteristics.first;
			std::pair<std::string, int> mappingColumnValueColumnType = mappingAttributesNamesAttributesCharacteristics.second;
			std::string columnValue = mappingColumnValueColumnType.first;
			int columnType = mappingColumnValueColumnType.second;

			if (it != _attributesNamesValues.begin())
				sql.append(", ");

			sql.append(columnName);
			sql.append("=");


			if (columnType == _string || columnType == _varchar || columnType == _char)
			{
				if (fitToDBPalavra(columnValue) == false)
				{
					return "";
				}

				sql.append("'");
				sql.append(columnValue);
				sql.append("'");
			}
			else if (columnType == _int || columnType == _float || columnType == _double
				|| columnType == _long || columnType == _enum)
			{
				if (fitToDBNumero(columnValue) == false)
				{
					return "";
				}

				sql.append(columnValue);
			}
			else if (columnType == _bool)
			{
				if (columnValue == "0")
					sql.append("FALSE");
				else
					sql.append("TRUE");
			}
			else if (columnType == _date)
			{
				if (columnValue.size() == 0)
					sql.append("null");
				else
				{
					if (fitToDBDate(columnValue) == false)
					{
						return "";
					}
					sql.append("'");
					sql.append(columnValue);
					sql.append("'");
				}
			}
		}

		sql.append(" WHERE ");
		sql.append(_primaryKeyColumnName);
		sql.append("=");
		sql.append(_primaryKeyValue);
		sql.append(";");
		return sql;
	}
}