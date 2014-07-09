#include "PersistenceManagerDelete.h"
#include "IPersistence.h"
#include "IPersistenceList.h"
#include "AttributeValue.h"
#include "AttributeValueList.h"
#include "Database.h"
#include "Table.h"
#include "IPersistenceManagerConnection.h"
#include "IPersistenceManagerQuery.h"

namespace PersistenceFramework
{
	PersistenceManagerDelete::PersistenceManagerDelete(IPersistenceManagerQuery* _managerQuery, IPersistenceManagerConnection* _managerConnection)
	{
		managerQuery = _managerQuery;
		managerConnection = _managerConnection;
	}

	bool PersistenceManagerDelete::deleteList(IPersistenceList* _objects)
	{
		if (_objects == nullptr)
			return false;

		for (auto it = _objects->begin(); it != _objects->end(); ++it)
		{
			IPersistence* objeto = (*it);

			//Agora sim vou deletar o objeto propriamente dito.
			if (deleteObject(objeto) == false)
				return false;
		}
		return true;
	}

	bool PersistenceManagerDelete::deleteObject(IPersistence* _objectToDelete)
	{
		if (_objectToDelete == nullptr || managerQuery == nullptr)
		{
			return false;
		}
		AttributeValueList values;
		_objectToDelete->getValues(&values);

		//Primeiro vou deletar todas os objetos que estão em listas minhas ou então os objetos 
		//que eu estou referenciando.
		for (auto it2 = values.begin(); it2 != values.end(); ++it2)
		{
			auto value = (*it2);

			if (value->getType() == _list_1_N && _objectToDelete->canDelete(value->getName()) == true)
				deleteList((*value->getAddress().ppList));
			else if (value->getType() == _object && *(value->getAddress().ppObject) != nullptr && _objectToDelete->canDelete(value->getName()) == true)
				deleteObject(*(value->getAddress().ppObject));
		}

		auto primaryKeyAttributeName = _objectToDelete->getPrimaryKeyAttributeName();
		auto primaryKeyValue = _objectToDelete->getValue(primaryKeyAttributeName);

		if (primaryKeyValue == nullptr)
			return false;

		std::string queryRemove = managerQuery->getQueryDelete(_objectToDelete->getTableName(), _objectToDelete->getColumnName(primaryKeyAttributeName), primaryKeyValue->getValue());

		Table tbl;
		return managerConnection->runQuery(queryRemove, tbl);
	}
}