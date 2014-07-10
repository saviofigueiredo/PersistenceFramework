#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include "IPersistenceManager.h"
#include <string>

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;

	class PersistenceManager : public IPersistenceManager
	{
	public:
		PersistenceManager(IPersistenceManagerConnection* _managerConnection,
			IPersistenceManagerDelete* _managerDelete,
			IPersistenceManagerGet* _managerGet,
			IPersistenceManagerUpdate* _managerUpdate,
			IPersistenceManagerQuery* _managerQuery,
			IPersistenceManagerInsert* _managerInsert);
		
		virtual bool getIPersistenceList(IPersistenceList* _list, IPersistence* _prototype);
		virtual bool getIPersistenceList(IPersistenceList* _lista, IPersistence* _prototype, IPersistence* _objectAttribute, std::string _attributeName);
		virtual bool insert(IPersistenceList* _objects);
		virtual bool insert(IPersistence* _object);
		virtual bool update(IPersistence* _object);
		virtual bool updatePrimitiveAttributes(IPersistence* _object);
		virtual bool updateObjectAttribute(IPersistence* _objectUpdate, std::string _attributeName);
		virtual bool updateAttributesList_1_N(IPersistence* _objeto, bool _cascade = true);
		virtual bool getAttributeObject(IPersistence* _parentObject, std::string _attributeName, AttributeValue* _value);
		virtual bool getAttributesPrimitive(IPersistence* _parentObject, std::string _attributeName, AttributeValue* _value);
		virtual bool getIPersistence(IPersistence* _object);
		virtual bool deleteObjects(IPersistenceList* _objects);
		virtual bool deleteObject(IPersistence* _object);
	};
}

#endif