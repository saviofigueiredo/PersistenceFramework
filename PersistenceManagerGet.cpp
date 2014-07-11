#include "PersistenceManagerGet.h"
#include "IPersistenceManagerConnection.h"
#include "IPersistenceManagerQuery.h"
#include "IPersistence.h"
#include "IPersistenceList.h"
#include "IPersistenceManagerConnection.h"
#include "IPersistenceManagerQuery.h"
#include "ClassesManager.h"
#include <sstream>
#include "DateTimeHelper.h"

namespace PersistenceFramework
{
	PersistenceManagerGet::PersistenceManagerGet(IPersistenceManagerConnection* _managerConnection, IPersistenceManagerQuery* _managerQuery)
	{
		managerConnection = _managerConnection;
		managerQuery = _managerQuery;
	}

	bool PersistenceManagerGet::read(IPersistence* _objeto)
	{
		return false;
	}

	bool PersistenceManagerGet::read(IPersistenceList* _list, IPersistence* _prototype)
	{
		if (_list == NULL || _prototype == NULL)
		{
			return false;
		}

		std::string objectTableName = _prototype->getTableName();
		std::list<std::string> objectColumnNames;
		_prototype->getColumnNamesNotProxy(objectColumnNames);
		std::string sql = managerQuery->getQuerySelect(objectTableName, objectColumnNames);

		Table tbl;
		managerConnection->runQuery(sql, tbl);

		if (tbl.ISEOF() == false)
		{
			tbl.MoveFirst();
		}

		while (tbl.ISEOF() == false)
		{
			auto newObject = _prototype->newObject();

			if (read(newObject, tbl) == true)
				_list->push_back(newObject);

			MSG message;
			if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}

			tbl.MoveNext();
		}

		::CoUninitialize();

		return true;
	}

	bool PersistenceManagerGet::read(IPersistenceList* _list, IPersistence* _prototype, IPersistence* _objectAttribute, std::string _attributeName)
	{
		if (_list == nullptr || _prototype == nullptr || _objectAttribute == nullptr || _attributeName == "")
			return false;

		std::list<std::string> columnNames;
		_prototype->getColumnNamesNotProxy(columnNames);

		std::string columnNameReferencesObject = _prototype->getColumnName(_attributeName);

		auto attributeValueReferencesObject = _prototype->getValue(_attributeName);
		auto attributeValueReferencedObject = _objectAttribute->getValue(attributeValueReferencesObject->getNameAttributeReferencedObject());

		if (attributeValueReferencedObject == nullptr)
		{
			return "";
		}

		std::string columnValueObjectReferenced = attributeValueReferencedObject->getValue();

		std::string sql = managerQuery->getQueryListReferencesObject(_prototype->getTableName(), columnNames, columnNameReferencesObject, columnValueObjectReferenced);

		Table tbl;
		managerConnection->runQuery(sql, tbl);

		if (tbl.ISEOF() == false)
			tbl.MoveFirst();

		while (tbl.ISEOF() == false)
		{
			auto newObject = _prototype->newObject();

			if (read(newObject, tbl, _attributeName, _objectAttribute) == true)
				_list->push_back(newObject);

			tbl.MoveNext();
		}

		return true;
	}

	bool PersistenceManagerGet::readAttributeObject(IPersistence* _newObject, std::string _attributeName, AttributeValue* _value)
	{
		if (_newObject == nullptr || _value == nullptr || _attributeName == "")
		{
			return false;
		}

		std::string columnValue = getObjectColumnValue(_newObject, _newObject->getColumnName(_attributeName));

		if (columnValue != "")
		{
			IPersistence* referencedObject = nullptr;

			//If the object is in memory, I don t need to get from database.
			//referencedObject = PersistenceManagerController::get(_value->getClassName(), columnValue);

			if (referencedObject != nullptr)
			{
				*_value->getAddress().ppObject = referencedObject;
				_value->setHasBeenRetrieved(true);
				return true;
			}

			if ((*_value->getAddress().ppObject) == nullptr)
			{
				referencedObject = ClassesManager::get()->newObject(_value->getClassName());
				*(_value->getAddress().ppObject) = referencedObject;
			}
			else
				referencedObject = *(_value->getAddress().ppObject);

			std::string objectTableName = referencedObject->getTableName();
			std::list<std::string> columnNamesObject;
			referencedObject->getColumnNamesNotProxy(columnNamesObject);
			auto attributeValueReferencedObject = referencedObject->getValue(_value->getNameAttributeReferencedObject());

			if (attributeValueReferencedObject == nullptr)
			{
				return false;
			}
			std::string referencedObjectColumnName = referencedObject->getColumnName(_value->getNameAttributeReferencedObject());
			std::string referencedObjectTableName = referencedObject->getTableName();

			std::string sql = managerQuery->getQueryObject(objectTableName, columnNamesObject, referencedObjectTableName, referencedObjectColumnName, columnValue);

			Table newTable;
			managerConnection->runQuery(sql, newTable);

			if (newTable.ISEOF() == false)
				newTable.MoveFirst();
			else
			{
				return false;
			}

			auto newObject = (*_value->getAddress().ppObject);

			bool retorno = read(newObject, newTable);
			_value->setHasBeenRetrieved(retorno);
			return retorno;
		}
		else
		{
			_value->setHasBeenRetrieved(true);
			return true;
		}
	}

	bool PersistenceManagerGet::readAttributeList_1_N(IPersistence* _newObject, std::string _attributeName, AttributeValue* _value)
	{
		if (!_newObject || _attributeName == "" || !_value)
		{
			return false;
		}

		auto prototype = (*_value->getAddress().ppList)->newObject();
		std::string objectTableName = prototype->getTableName();
		std::list<std::string> objectColumnNames;
		prototype->getColumnNamesNotProxy(objectColumnNames);
		delete prototype;
		std::string referencedAttributeName = _newObject->getAttributeNameReferencedByList(_attributeName);

		auto attributeValueReferenced = _newObject->getValue(referencedAttributeName);
		std::string columnValueReferenced;

		if (attributeValueReferenced != NULL && attributeValueReferenced->getType() == _long)
		{
			std::stringstream ss;
			ss << (*attributeValueReferenced->getAddress().pLong);
			columnValueReferenced.assign(ss.str());
		}

		std::string sql = managerQuery->getQueryList_1_N(objectTableName, objectColumnNames, _newObject->getColumnListReferencesAttribute(_attributeName), columnValueReferenced);
		Table tbl;
		managerConnection->runQuery(sql, tbl);

		if (tbl.ISEOF() == false)
			tbl.MoveFirst();

		while (tbl.ISEOF() == false)
		{
			auto newObject = (*_value->getAddress().ppList)->newObject();
			bool retrieved = false;

			char primaryKeyValue[50];
			if (tbl.GetFieldValue(newObject->getColumnName(newObject->getPrimaryKeyAttributeName()).c_str(), primaryKeyValue))
			{
				IPersistence* referencedObject = nullptr; // = PersistenceManagerController::get(newObject->getClassNameVirtual(), primaryKeyValue);

				if (referencedObject != nullptr)
				{
					delete newObject;
					newObject = referencedObject;
					retrieved = true;
				}
			}

			if (retrieved == true || read(newObject, tbl) == true)
				(*_value->getAddress().ppList)->push_back(newObject);

			tbl.MoveNext();
		}
		_value->setHasBeenRetrieved(true);
		return true;
	}

	std::string PersistenceManagerGet::getObjectColumnValue(IPersistence* _object, std::string _column)
	{
		std::string primaryKeyAttributeName = _object->getPrimaryKeyAttributeName();
		std::string primaryKeyColumnName = _object->getColumnName(primaryKeyAttributeName);
		auto primaryKeyAttributeValue = _object->getValue(primaryKeyAttributeName);

		if (primaryKeyAttributeValue == NULL)
			return "";

		std::string queryGetValorColunaObjeto = managerQuery->getQueryObjectColumnValue(_column, _object->getTableName(), primaryKeyColumnName, primaryKeyAttributeValue->getValue());

		Table table;
		managerConnection->runQuery(queryGetValorColunaObjeto, table);
		std::string columnValue;
		if (table.ISEOF() == false)
		{
			char columnValueChar[255];
			table.MoveFirst();
			table.GetFieldValue(_column.c_str(), columnValueChar);
			columnValue = columnValueChar;
		}
		return columnValue;
	}

	bool PersistenceManagerGet::alreadyExists(IPersistence* _objeto)
	{
		std::string objectTableName = _objeto->getTableName();
		std::list<std::string> objectColumnNames;
		_objeto->getColumnNamesNotProxy(objectColumnNames);
		std::string primaryKeyAttributeName = _objeto->getPrimaryKeyAttributeName();
		auto primaryKeyAttributeValue = _objeto->getValue(primaryKeyAttributeName);
		std::string primaryKeyColumnName = _objeto->getColumnName(primaryKeyAttributeName);

		if (primaryKeyAttributeValue == nullptr)
			return false;

		std::string query = managerQuery->getQueryPrimaryKeyValue(objectTableName, objectColumnNames, primaryKeyColumnName, primaryKeyAttributeValue->getValue());

		if (query == "")
			return false;

		Table tbl;
		managerConnection->runQuery(query, tbl);

		if (tbl.ISEOF() == false)
			return true;

		return false;
	}

	bool PersistenceManagerGet::readAttributePrimitive(IPersistence* _object, std::string _attributeName, AttributeValue* _value)
	{
		if (_object == nullptr || _attributeName == "" || _value == nullptr)
		{
			return false;
		}

		std::string objectTableName = _object->getTableName();
		auto primaryKeyValue = _object->getPrimaryKeyValue();
		std::string primaryKeyValueStr;
		std::string primaryKeyAttributeName = _object->getPrimaryKeyAttributeName();
		std::string primaryKeyColumnName = _object->getColumnName(primaryKeyAttributeName);

		if (primaryKeyValue != 0)
		{
			std::stringstream ss;
			ss << primaryKeyValue;
			primaryKeyValueStr.assign(ss.str());
		}

		std::string columnNameToRead = _object->getColumnName(_attributeName);

		std::string sql = managerQuery->getQueryPrimitiveAttribute(objectTableName, primaryKeyColumnName, primaryKeyValueStr, columnNameToRead);
		Table tbl;
		managerConnection->runQuery(sql, tbl);

		if (tbl.ISEOF() == false)
			tbl.MoveFirst();

		if (tbl.ISEOF() == false)
		{
			this->readAttributePrimitive(_object, _attributeName, _value, tbl);
			return true;
		}
		else
			return false;
	}

	bool PersistenceManagerGet::read(IPersistence* _newObject, Table _tbl, std::string _attributeName, IPersistence* _objectAttribute)
	{
		if (_newObject == NULL)
		{
			return false;
		}

		std::list<std::string> attributeNames;

		_newObject->getAttributeNames(attributeNames);
		std::string primaryKeyAttributeName = _newObject->getPrimaryKeyAttributeName();
		attributeNames.remove(primaryKeyAttributeName);
		attributeNames.push_front(primaryKeyAttributeName);

		for (auto it = attributeNames.begin(); it != attributeNames.end(); it++)
		{
			std::string attributeName = (*it);
			auto value = _newObject->getValue(attributeName);

			if (value->isProxy() == true)
				continue;

			if (value->isPrimitive())
				readAttributePrimitive(_newObject, attributeName, value, _tbl);
			else if (value->getType() == _object)
			{
				if (attributeName != _attributeName)
					readAttributeObject(_newObject, attributeName, value);
				else // if the object is in memory
				{
					value->setHasBeenRetrieved(true);
					(*value->getAddress().ppObject) = _objectAttribute;
				}
			}
			else if (value->getType() == _list_1_N)
				readAttributeList_1_N(_newObject, attributeName, value);
		}
		return true;
	}

	bool PersistenceManagerGet::readAttributePrimitive(IPersistence* _newObject, std::string _attributeName, AttributeValue* _valor, Table& _tbl)
	{
		if (_newObject == nullptr)
		{
			return false;
		}

		char ErrStr[257];

		std::string columnName = _newObject->getColumnName(_attributeName);

		if ((_valor->getType() == _int || _valor->getType() == _bool) || (_valor->getType() == _variantType))
		{
			int valorInt;

			if (_tbl.Get(columnName.c_str(), valorInt))
				*_valor->getAddress().pInt = valorInt;
			else
				_tbl.GetErrorErrStr(ErrStr);

			_valor->setHasBeenRetrieved(true);
		}
		if (_valor->getType() == _string)
		{
			char valueString[5000];

			if (_tbl.Get(columnName.c_str(), valueString))
				(*_valor->getAddress().pString) = valueString;
			else
				_tbl.GetErrorErrStr(ErrStr);
			_valor->setHasBeenRetrieved(true);
		}
		else if (_valor->getType() == _float)
		{
			float valorFloat;

			if (_tbl.Get(columnName.c_str(), valorFloat))
				*_valor->getAddress().pFloat = valorFloat;
			else
				_tbl.GetErrorErrStr(ErrStr);

			_valor->setHasBeenRetrieved(true);
		}
		else if (_valor->getType() == _double)
		{
			double valorDouble;

			if (_tbl.Get(columnName.c_str(), valorDouble))
				*_valor->getAddress().pDouble = valorDouble;
			else
				_tbl.GetErrorErrStr(ErrStr);

			_valor->setHasBeenRetrieved(true);
		}
		else if (_valor->getType() == _enum)
		{
			int valorEnum;

			if (_tbl.Get(columnName.c_str(), valorEnum))
				*_valor->getAddress().pEnum = valorEnum;
			else
				_tbl.GetErrorErrStr(ErrStr);

			_valor->setHasBeenRetrieved(true);
		}
		else if (_valor->getType() == _long)
		{
			unsigned long long valueLong;

			if (_tbl.Get(columnName.c_str(), valueLong))
				*_valor->getAddress().pLong = valueLong;
			else
				_tbl.GetErrorErrStr(ErrStr);

			_valor->setHasBeenRetrieved(true);
		}
		else if (_valor->getType() == _date)
		{
			char valueString[1000];

			if (_tbl.GetFieldValue(columnName.c_str(), valueString))
				*_valor->getAddress().pDate = DateTimeHelper::ConvertDateTime(valueString);
			else
				_tbl.GetErrorErrStr(ErrStr);

			_valor->setHasBeenRetrieved(true);
		}
		return true;

	}
}