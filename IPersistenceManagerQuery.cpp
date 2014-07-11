#include "IPersistenceManagerQuery.h"
#include "Database.h"
#include <sstream>

namespace PersistenceFramework
{
	std::string IPersistenceManagerQuery::getQueryDelete(const std::string& _tableName, const std::string& _columnName, const std::string& _columnValue)
	{
		if (_tableName == "" || _columnName == "" || _columnValue == "")
			return "";

		std::string query("DELETE FROM ");
		query.append(_tableName);
		query.append(" WHERE ");
		query.append(_columnName);
		query.append(" = ");
		query.append(_columnValue);
		query.append(";");

		return query;
	}

	std::string IPersistenceManagerQuery::getQuerySelect(const std::string& _tableName, const std::list<std::string>& _columnsNames)
	{
		if (_tableName == "" || _columnsNames.size() == 0)
		{
			return "";
		}

		std::string sql("SELECT ");

		for (std::list<std::string>::const_iterator it = _columnsNames.begin(); it != _columnsNames.end(); it++)
		{
			if (it != _columnsNames.begin())
				sql.append(", ");
			sql.append("\"");
			sql.append((*it));
			sql.append("\"");
		}

		sql.append(" FROM ");
		sql.append("\"" + _tableName + "\"");
		sql.append(";");

		return sql;
	}

	std::string IPersistenceManagerQuery::getQueryListReferencesObject(const std::string& _tableName, const std::list<std::string>& _columnsNames, const std::string& _objectReferenceColumnName, const std::string& _referencedColumnValue)
	{
		if (_tableName == "" || _columnsNames.size() == 0 || _objectReferenceColumnName == "" || _referencedColumnValue == "")
			return "";

		std::string sql("SELECT ");

		for (std::list<std::string>::const_iterator it = _columnsNames.begin(); it != _columnsNames.end(); it++)
		{
			if (it != _columnsNames.begin())
				sql.append(", ");

			sql.append((*it));
		}

		sql.append(" FROM ");
		sql.append(_tableName);
		sql.append(" WHERE ");
		sql.append(_tableName);
		sql.append(".");
		sql.append(_objectReferenceColumnName);
		sql.append(" = ");
		sql.append(_referencedColumnValue);
		sql.append(";");

		return sql;
	}

	std::string IPersistenceManagerQuery::getQueryObject(const std::string& _objectTableName, const std::list<std::string>& _objectColumnsNames, const std::string& _referencedObjectTableName, const std::string& _referencedObjectColumnName, const std::string& _referencedObjectColumnValue)
	{

		if (_objectTableName == "" || _objectColumnsNames.size() == 0 || _referencedObjectTableName == "" || _referencedObjectColumnName == "" || _referencedObjectColumnValue == "")
		{
			return "";
		}

		std::string query = this->getQuerySelect(_objectTableName, _objectColumnsNames);
		query.replace(query.length() - 1, 1, " ");
		query.append(" WHERE ");
		query.append(_referencedObjectTableName);
		query.append(".");
		query.append(_referencedObjectColumnName);
		query.append(" = ");
		query.append(_referencedObjectColumnValue);
		query.append(";");

		return query;
	}

	std::string IPersistenceManagerQuery::getQueryUpdateAttributeObject(const std::string& _tableNameToUpdate, const std::string& _columnToUpdate, const std::string& _columnValueToUpdate, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyColumnValue)
	{
		if (_tableNameToUpdate == "" || _columnToUpdate == "" || _columnValueToUpdate == "" || _primaryKeyColumnName == "" || _primaryKeyColumnValue == "")
		{
			return "";
		}

		std::string sql("UPDATE ");
		sql.append(_tableNameToUpdate);
		sql.append(" SET ");
		sql.append(_columnToUpdate);
		sql.append(" =");
		sql.append(_columnValueToUpdate);
		sql.append(" WHERE ");
		sql.append(_primaryKeyColumnName);
		sql.append(" =");
		sql.append(_primaryKeyColumnValue);
		sql.append(";");
		return sql;
	}

	std::string IPersistenceManagerQuery::getQueryList_1_N(const std::string& _tableName, const std::list<std::string>& _columnsNames, const std::string& _referencedColumn, const std::string& _valueReferencedColumn)
	{
		if (_tableName == "" || _columnsNames.size() == 0 || _referencedColumn == "" || _valueReferencedColumn == "")
		{
			return "";
		}

		std::string query = this->getQuerySelect(_tableName, _columnsNames);
		query.replace(query.length() - 1, 1, " ");
		query.append(" WHERE ");
		query.append(_tableName);
		query.append(".");
		query.append(_referencedColumn);
		query.append(" = ");
		query.append(_valueReferencedColumn);
		query.append(";");
		return query;
	}

	std::string IPersistenceManagerQuery::getQueryObjectColumnValue(const std::string& _columnName, const std::string& _tableName, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyColumnValue)
	{
		std::string query("SELECT ");
		query.append(_columnName);
		query.append(" FROM ");
		query.append(_tableName);
		query.append(" WHERE ");
		query.append(_primaryKeyColumnName);
		query.append(" = ");
		query.append(_primaryKeyColumnValue);
		query.append(";");
		return query;
	}

	std::string IPersistenceManagerQuery::getQueryPrimaryKeyValue(const std::string& _tableName, const std::list<std::string>& _columnsNames, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyValue)
	{
		std::string query = this->getQuerySelect(_tableName, _columnsNames);

		if (query.size() == 0)
			return "";

		query.replace(query.length() - 1, 1, " ");
		query.append(" WHERE ");
		query.append(_primaryKeyColumnName);
		query.append(" = ");
		query.append(_primaryKeyValue);
		query.append(";");
		return query;
	}

	std::string IPersistenceManagerQuery::getQueryPrimitiveAttribute(const std::string &_objectTableName, const std::string &_primaryKeyColumnName, const std::string &_primaryKeyValue, const std::string &_columnNameToRetrieve)
	{
		if (_objectTableName == "" || _primaryKeyColumnName == "" || _primaryKeyValue == "" || _columnNameToRetrieve == "")
		{
			return "";
		}

		std::string query;
		query.append("SELECT ");
		query.append(_columnNameToRetrieve);
		query.append(" FROM ");
		query.append(_objectTableName);
		query.append(" WHERE ");
		query.append(_primaryKeyColumnName);
		query.append(" = ");
		query.append(_primaryKeyValue);
		query.append(";");
		return query;
	}
	
	std::string IPersistenceManagerQuery::getQueryUpdateParentList(const std::string& _listTableName, const std::string& _columnList, const std::string& _objectColumnValue, const std::string& _listPrimaryKeyColumn, const std::list<std::string>& _primaryKeyChildObjects)
	{
		if (_listTableName == "" || _columnList == "" || _objectColumnValue == "" || _primaryKeyChildObjects.size() == 0)
		{
			return "";
		}

		std::string query;
		query.append("UPDATE ");
		query.append(_listTableName);
		query.append(" SET ");
		query.append(_columnList);
		query.append(" = ");
		query.append(_objectColumnValue);
		query.append(" WHERE ");
		query.append(_listPrimaryKeyColumn);
		query.append(" IN (");

		for (std::list<std::string>::const_iterator it = _primaryKeyChildObjects.begin(); it != _primaryKeyChildObjects.end(); it++)
		{
			if (it != _primaryKeyChildObjects.begin())
				query.append(", ");

			query.append((*it));
		}
		query.append(");");
		return query;
	}
}