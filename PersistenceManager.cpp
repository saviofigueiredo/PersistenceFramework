#include "PersistenceManager.h"
#include "IPersistenceManagerGet.h"
#include "IPersistenceManagerDelete.h"
#include "IPersistenceManagerUpdate.h"
#include "IPersistenceManagerConnection.h"
#include "IPersistenceManagerQuery.h"
#include "IPersistenceManagerInsert.h"
#include "IPersistenceList.h"

namespace PersistenceFramework
{
	PersistenceManager::PersistenceManager(IPersistenceManagerConnection* _managerConnection,
		IPersistenceManagerDelete* _managerDelete,
		IPersistenceManagerGet* _managerGet,
		IPersistenceManagerUpdate* _managerUpdate,
		IPersistenceManagerQuery* _managerQuery,
		IPersistenceManagerInsert* _managerInsert)
	{
		managerConnection = _managerConnection;
		managerDelete = _managerDelete;
		managerGet = _managerGet;
		managerUpdate = _managerUpdate;
		managerQuery = _managerQuery;
		managerInsert = _managerInsert;
	}

	bool PersistenceManager::getIPersistenceList(IPersistenceList* _list, IPersistence* _prototype)
	{
		if (managerGet != NULL && _list != NULL && _prototype != NULL)
		{
			return managerGet->read(_list, _prototype);
		}

		return false;
	}

	bool PersistenceManager::getIPersistenceList(IPersistenceList* _list, IPersistence* _prototype, IPersistence* _objectAttribute, std::string _attributeName)
	{
		if (managerGet != NULL && _list != NULL && _prototype != NULL && _objectAttribute != NULL && _attributeName != "")
		{
			return managerGet->read(_list, _prototype, _objectAttribute, _attributeName);
		}

		return false;
	}

	bool PersistenceManager::insert(IPersistenceList* _objects)
	{
		if (_objects == NULL)
		{
			return false;
	 	}

		for (std::list<IPersistence*>::iterator it = _objects->begin(); it != _objects->end(); ++it)
		{
			auto currentObject = (*it);

			insert(currentObject);
		}

		return true;
	}

	bool PersistenceManager::insert(IPersistence* _object)
	{
		if (_object == nullptr)
		{
			return false;
		}

		if (managerGet->alreadyExists(_object) == true)
		{
			managerUpdate->update(_object);
		}
		else
		{
			managerInsert->insert(_object);
		}

		return true;
	}

	bool PersistenceManager::update(IPersistence* _object)
	{
		if (managerUpdate == nullptr || _object == nullptr)
		{
			return false;
		}

		return managerUpdate->update(_object);
	}

	bool PersistenceManager::updatePrimitiveAttributes(IPersistence* _object)
	{
		if (managerUpdate == nullptr || _object == nullptr)
		{
			return false;
		}

		return managerUpdate->updateAttributesPrimitive(_object);
	}

	bool PersistenceManager::updateObjectAttribute(IPersistence* _objectUpdate, std::string _attributeName)
	{
		if (managerUpdate == nullptr || _objectUpdate == nullptr)
		{
			return false;
		}

		return managerUpdate->updateAttributeObject(_objectUpdate, _attributeName);
	}

	bool PersistenceManager::updateAttributesList_1_N(IPersistence* _object, bool _cascade)
	{
		if (managerUpdate == nullptr || _object == nullptr)
		{
			return false;
		}

		return managerUpdate->updateAttributesList_1_N(_object, _cascade);

	}

	bool PersistenceManager::getAttributeObject(IPersistence* _parentObject, std::string _attributeName, AttributeValue* _value)
	{
		if (managerGet == nullptr || _parentObject == nullptr || _attributeName.size() == 0 || _value == nullptr)
		{
			return false;
		}

		return managerGet->readAttributeObject(_parentObject, _attributeName, _value);
	}

	bool PersistenceManager::getAttributesPrimitive(IPersistence* _parentObject, std::string _attributeName, AttributeValue* _value)
	{
		if (managerGet == nullptr || _parentObject == nullptr || _attributeName.size() == 0 || _value == nullptr)
		{
			return false;
		}

		return managerGet->readAttributePrimitive(_parentObject, _attributeName, _value);
	}

	bool PersistenceManager::getIPersistence(IPersistence* _object)
	{
		if (managerGet == nullptr || _object == nullptr)
		{
			return false;
		}

		return managerGet->read(_object);
	}

	bool PersistenceManager::deleteObjects(IPersistenceList* _objects)
	{
		if (managerDelete == nullptr || _objects == nullptr)
		{
			return false;
		}

		return managerDelete->deletaList(_objects);
	}

	bool PersistenceManager::deleteObject(IPersistence* _object)
	{
		if (managerDelete == nullptr || _object == nullptr)
		{
			return false;
		}

		return managerDelete->deleteObject(_object);
	}
}