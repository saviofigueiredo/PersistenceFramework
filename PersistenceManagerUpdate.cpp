#include "PersistenceManagerUpdate.h"
#include "IPersistenceManagerConnection.h"
#include "IPersistenceManagerDelete.h"
#include "IPersistenceManagerGet.h"
#include "IPersistenceManagerQuery.h"
#include "IPersistenceManagerInsert.h"
#include "IPersistence.h"
#include "IPersistenceList.h"
#include "PersistenceManagerController.h"
#include "AttributeValue.h"
#include "AddressAttributeValue.h"
#include <string>
#include <list>
#include <sstream>
#include "Database.h"
#include "Table.h"

namespace PersistenceFramework
{
	PersistenceManagerUpdate::PersistenceManagerUpdate(IPersistenceManagerConnection* _managerConnection,
		IPersistenceManagerDelete* _managerDelete,
		IPersistenceManagerGet* _managerGet,
		IPersistenceManagerQuery* _managerQuery,
		IPersistenceManagerInsert* _managerInsert)
	{
		managerConnection = _managerConnection;
		managerDelete = _managerDelete;
		managerGet = _managerGet;
		managerQuery = _managerQuery;
		managerInsert = _managerInsert;
	}

	bool PersistenceManagerUpdate::update(IPersistence* _object)
	{
		if (_object == nullptr)
		{
			return nullptr;
		}

		if (updateAttributesPrimitive(_object) && updateAttributesList_1_N(_object) && updateAttributesObject(_object))
			return true;
		else
			return false;
	}

	bool PersistenceManagerUpdate::updateAttributeObject(IPersistence* _objectToUpdate, std::string _attributeName)
	{
		auto value = _objectToUpdate->getValue(_attributeName);

		if (value == nullptr)
			return false;

		// If I can do the drop cascade of this attribute, then, I will retrieve it to know if it has been changed.
		if (_objectToUpdate->canDelete(_attributeName) == true)
		{
			IPersistence* oldAttribute = nullptr;
			auto clonnedValue = new AttributeValue(value->getAttribute(), &oldAttribute);
			managerGet->readAttributeObject(_objectToUpdate, _attributeName, clonnedValue);
			delete clonnedValue;

			// The object old attribute has been retrieved by the method retrieve attribute object
			if (oldAttribute != nullptr)
			{
				if ((*value->getAddress().ppObject) == nullptr)
				//if ((*value->getAddress().ppObject) == nullptr || (*value->getAddress.ppObject)->getPrimaryKeyValue() != oldAttribute->getPrimaryKeyValue())
					managerDelete->deleteObject(oldAttribute);

				if (oldAttribute != nullptr)
				{
					delete oldAttribute;
					oldAttribute = nullptr;
				}
			}
		}
		//Now I will update the object
		auto referencedObject = (*value->getAddress().ppObject);
		std::string columnValueToUpdate;
		if (referencedObject == nullptr)
			columnValueToUpdate.assign("null");
		else
		{
			std::string referencedObjectColumnName = referencedObject->getColumnName(value->getNameAttributeReferencedObject());
			columnValueToUpdate.assign(managerGet->getObjectColumnValue(referencedObject, referencedObjectColumnName));
		}
		std::string primaryKeyAttributeName = _objectToUpdate->getPrimaryKeyAttributeName();

		if (primaryKeyAttributeName == "")
			return false;

		std::string nomeColunaChavePrimaria;
		nomeColunaChavePrimaria.assign(_objectToUpdate->getColumnName(primaryKeyAttributeName));
		char primaryKeyValue[100];
		sprintf(primaryKeyValue, "%d", _objectToUpdate->getPrimaryKeyValue());

		std::string queryUpdateAtributoObjeto = managerQuery->getQueryUpdateAttributeObject(_objectToUpdate->getTableName(),
			_objectToUpdate->getColumnName(value->getName()),
			columnValueToUpdate,
			nomeColunaChavePrimaria,
			primaryKeyValue);
		Table tbl;
		if (managerConnection->runQuery(queryUpdateAtributoObjeto, tbl) == false)
			return false;

		return true;
	}

	bool PersistenceManagerUpdate::updateAttributesObject(IPersistence* objectToUpdate)
	{
		if (objectToUpdate == nullptr)
		{
			return false;
		}

		std::list<std::string> attributesNames;

		objectToUpdate->getAttributeNames(attributesNames);

		for (auto it = attributesNames.begin(); it != attributesNames.end(); it++)
		{
			std::string attributeName = (*it);
			auto value = objectToUpdate->getValue(attributeName);

			if (value->getType() == _object)
			{
				if (updateAttributeObject(objectToUpdate, attributeName) == false)
					return false;
			}
		}
		return true;
	}

	bool PersistenceManagerUpdate::updateParent(IPersistence* _parentObject, AttributeValue* _value, IPersistenceList* _childObjects)
	{
		if (_parentObject == nullptr)
		{
			return false;
		}

		if (_value == nullptr)
		{
			return false;
		}
		if (_childObjects == nullptr)
		{
			return false;
		}

		if (_childObjects->size() == 0)
			return true;

		std::string primaryKey = _parentObject->getPrimaryKeyAttributeName();
		std::string primaryKeyColumn = _parentObject->getColumnName(primaryKey);
		std::string objectColumnValue = managerGet->getObjectColumnValue(_parentObject, primaryKeyColumn);

		if (objectColumnValue == "")
		{
			return false;
		}

		std::string listColumn = _parentObject->getColumnListReferencesAttribute(_value->getName());

		if (listColumn == "")
		{
			return false;
		}

		//Getting any object from the list just to know the table name and the name of some attributes
		auto firstElement = _childObjects->front();

		std::string listTableName = firstElement->getTableName();
		std::string primaryKeyAttributeList = firstElement->getPrimaryKeyAttributeName();
		std::string listPrimaryKeyColumn = firstElement->getColumnName(primaryKeyAttributeList);
		std::list<std::string> primaryKeyAttributeValues;

		for (auto it = _childObjects->begin(); it != _childObjects->end(); it++)
		{
			auto currentObject = (*it);

			std::string primaryKeyAttributeNameList = currentObject->getAttributeName(listPrimaryKeyColumn);

			auto listObjectPrimaryKeyValue = currentObject->getValue(primaryKeyAttributeNameList);

			if (listObjectPrimaryKeyValue == nullptr)
			{
				return false;
			}

			primaryKeyAttributeValues.push_back(listObjectPrimaryKeyValue->getValue());
		}
		
		std::string query = managerQuery->getQueryUpdateParentList(listTableName, listColumn, objectColumnValue, listPrimaryKeyColumn, primaryKeyAttributeValues);

		if (query == "")
		{
			return false;
		}

		Table tbl;
		return managerConnection->runQuery(query, tbl);
	}

	bool PersistenceManagerUpdate::updateAttributesPrimitive(IPersistence* _object)
	{
		if (_object == nullptr)
		{
			return false;
		}

		std::string tableName = _object->getTableName();
		std::string primaryKeyAttributeName = _object->getPrimaryKeyAttributeName();
		std::string primaryKeyColumnName = _object->getColumnName(primaryKeyAttributeName);

		auto primaryKeyLong = _object->getPrimaryKeyValue();
		std::stringstream ss;
		ss << primaryKeyLong;
		std::string primaryKeyValue(ss.str());

		// This variable stores  a list of paris where the first element is the column name and the second element another pair.
		// For this pair, the first element is the column value and the second the column type.
		std::list<std::pair<std::string, std::pair<std::string, int>>> columnsValuesAttributes;

		std::list<std::string> attributesNames;

		_object->getAttributeNames(attributesNames);

		for (auto it = attributesNames.begin(); it != attributesNames.end(); it++)
		{
			std::string attributeName = (*it);
			auto value = _object->getValue(attributeName);

			if (value->isPrimaryKey() == false && value->isPrimitive() == true && value->hasBeenRetrieved() == true)
			{
				std::string columnName = _object->getColumnName(attributeName);
				std::string columnValue = value->getValue();
				auto columnType = value->getType();

				std::pair<std::string, int> columnValueColumnType;
				columnValueColumnType.first = columnValue;
				columnValueColumnType.second = columnType;
				std::pair<std::string, std::pair<std::string, int>> parNomeColunaCaracteristicasColuna;
				parNomeColunaCaracteristicasColuna.first = columnName;
				parNomeColunaCaracteristicasColuna.second = columnValueColumnType;
				columnsValuesAttributes.push_back(parNomeColunaCaracteristicasColuna);
			}
		}

		//In case I don t have any primitive attributes to be updated.
		if (columnsValuesAttributes.size() == 0)
			return true;

		std::string sqlUpdate = managerQuery->getQueryUpdatePrimitiveAttribute(tableName, columnsValuesAttributes, primaryKeyColumnName, primaryKeyValue);

		Table tbl;
		return managerConnection->runQuery(sqlUpdate, tbl);
	}

	bool PersistenceManagerUpdate::updateAttributesList_1_N(IPersistence* _object, bool _cascade)
	{
		if (_object == nullptr)
		{
			return false;
		}

		std::list<std::string> attributesNames;

		_object->getAttributeNames(attributesNames);

		for (auto it = attributesNames.begin(); it != attributesNames.end(); it++)
		{
			auto attributeName = (*it);
			auto value = _object->getValue(attributeName);

			if (value->getType() == _list_1_N && value->hasBeenRetrieved() == true)
			{
				//Get the current list.
				auto objects = (*value->getAddress().ppList);
				auto objectsOld = (*value->getAddress().ppList)->newList();

				auto valueClone = new AttributeValue(value->getAttribute(), &objectsOld);
				managerGet->readAttributeList_1_N(_object, attributeName, valueClone);
				delete valueClone;

				// Get the elements to be deleted. In other words, these elements are in the second list
				// but are not in the first list.
				if (_object->canDelete(attributeName) == true)
				{
					IPersistenceList objectsToDelete;
					objects->getObjectsIDontHave(objectsOld, &objectsToDelete);
					managerDelete->deletaList(&objectsToDelete);
				}

				// Get the elements to be inserted. These elements are in the first list but are not in the second list.
				IPersistenceList objectsToInsert;
				objectsOld->getObjectsIDontHave(objects, &objectsToInsert);

				for (auto it = objectsToInsert.begin(); it != objectsToInsert.end(); ++it)
				{
					if (managerGet->alreadyExists(*it) == false)
						managerInsert->insert(*it);
				}

				// Now I need to update the column that indicates who is the parent of the list of objects to insert.
				updateParent(_object, value, &objectsToInsert);

				// Now I will update each one of the objects in the list because they may have been changed.
				if (_cascade == true)
				{
					for (auto it = objects->begin(); it != objects->end(); ++it)
					{
						this->update(*it);
					}
				}

				// Now I will delete from the old objects just the objects that are not in memory
				auto iter = objectsOld->begin();
				auto endIter = objectsOld->end();
				for (; iter != endIter; ++iter)
				{
					unsigned long long oldObjectPrimaryKey = (*iter)->getPrimaryKeyValue();
					char strPrimaryKeyObjectOld[50];
					sprintf(strPrimaryKeyObjectOld, "%ld", oldObjectPrimaryKey);

					if (PersistenceManagerController::getInstance((*iter)->getClassNameVirtual(), strPrimaryKeyObjectOld) == nullptr)
						delete (*iter);
				}

				delete objectsOld;
			}
		}

		return true;
	}
}