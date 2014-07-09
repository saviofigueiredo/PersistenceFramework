#include "PersistenceManagerController.h"

#include "IPersistenceList.h"

#include "IPersistenceManager.h"
#include "IPersistenceManagerConnection.h"
#include "IPersistenceManagerQuery.h"
#include "IPersistenceManagerDelete.h"
#include "IPersistenceManagerGet.h"
#include "IPersistenceManagerInsert.h"
#include "IPersistenceManagerUpdate.h"

namespace PersistenceFramework
{
	std::unordered_map<std::string, IPersistenceList*> PersistenceManagerController::mappingClassNameObjects;
	IPersistenceManager* PersistenceManagerController::managerPersistence;
	IPersistenceManagerConnection* PersistenceManagerController::managerConnection;
	IPersistenceManagerQuery* PersistenceManagerController::managerQuery;
	IPersistenceManagerDelete* PersistenceManagerController::managerDelete;
	IPersistenceManagerGet* PersistenceManagerController::managerGet;
	IPersistenceManagerInsert* PersistenceManagerController::managerInsert;
	IPersistenceManagerUpdate* PersistenceManagerController::managerUpdate;

	void PersistenceManagerController::freeMemory()
	{

		if (managerPersistence != nullptr)
		{
			delete managerPersistence;
			managerPersistence = nullptr;
		}

		if (managerConnection != nullptr)
		{
			delete managerConnection;
			managerConnection = nullptr;
		}

		if (managerQuery != nullptr)
		{
			delete managerQuery;
			managerQuery = nullptr;
		}

		if (managerDelete != nullptr)
		{
			delete managerDelete;
			managerDelete = nullptr;
		}

		if (managerGet != nullptr)
		{
			delete managerGet;
			managerGet = nullptr;
		}

		if (managerInsert != nullptr)
		{
			delete managerInsert;
			managerInsert = nullptr;
		}

		if (managerUpdate != nullptr)
		{
			delete managerUpdate;
			managerUpdate = nullptr;
		}
	}

	IPersistence* PersistenceManagerController::getInstance(std::string _className, std::string _valorChavePrimaria)
	{
		IPersistenceList* objects = nullptr;

		if (mappingClassNameObjects.find(_className) != mappingClassNameObjects.end())
			objects = mappingClassNameObjects[_className];

		if (objects != nullptr)
		{
			for (auto it = objects->begin(); it != objects->end(); ++it)
			{
				auto object = (*it);

				unsigned long long longPrimaryKey = object->getPrimaryKeyValue();

				char strPrimaryKey[50];
				sprintf(strPrimaryKey, "%ld", longPrimaryKey);

				if (strPrimaryKey == _valorChavePrimaria)
					return object;
			}
		}
		return nullptr;
	}

	void PersistenceManagerController::removeList()
	{
		for (auto it = mappingClassNameObjects.begin(); it != mappingClassNameObjects.end(); ++it)
		{
			auto insertedObjects = (*it).second;
			insertedObjects->clear();
			delete insertedObjects;
		}

		mappingClassNameObjects.clear();
	}

	void PersistenceManagerController::removeObject(IPersistence* _object)
	{
		IPersistenceList* objects = nullptr;
		if (mappingClassNameObjects.find(_object->getClassNameVirtual()) != mappingClassNameObjects.end())
			objects = mappingClassNameObjects[_object->getClassNameVirtual()];

		if (objects != nullptr)
		{
			objects->remove(_object);
			if (objects->size() == 0)
			{
				mappingClassNameObjects.erase(_object->getClassNameVirtual());
				delete objects;
				objects = nullptr;
			}
		}
	}

	void PersistenceManagerController::setList(IPersistenceList* _objects)
	{
		if (_objects != nullptr)
		{
			for (auto it = _objects->begin(); it != _objects->end(); ++it)
			{
				auto currentObject = (*it);

				std::string nomeClasse = currentObject->getClassNameVirtual();

				auto insertedObjects = mappingClassNameObjects[nomeClasse];

				if (insertedObjects == nullptr)
				{
					insertedObjects = new IPersistenceList();
					mappingClassNameObjects[nomeClasse] = insertedObjects;
				}
				insertedObjects->push_back(currentObject);
			}
		}
	}
}