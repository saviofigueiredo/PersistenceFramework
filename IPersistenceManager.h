#ifndef IPERSISTENCEMANAGER_H
#define IPERSISTENCEMANAGER_H

#include <string>

namespace PersistenceFramework
{
	class IPersistenceManagerConnection;
	class IPersistenceManagerDelete;
	class IPersistenceManagerGet;
	class IPersistenceManagerUpdate;
	class IPersistenceManagerQuery;
	class IPersistenceList;
	class IPersistence;
	class IPersistenceManagerInsert;
	class AttributeValue;

	class IPersistenceManager
	{
	protected:
		IPersistenceManagerConnection* managerConnection;
		IPersistenceManagerDelete* managerDelete;
		IPersistenceManagerGet* managerGet;
		IPersistenceManagerUpdate* managerUpdate;
		IPersistenceManagerQuery* managerQuery;
		IPersistenceManagerInsert* managerInsert;

	public:

		virtual bool getIPersistenceList(IPersistenceList* _list, IPersistence* _prototype) = 0;
		virtual bool getIPersistenceList(IPersistenceList* _lista, IPersistence* _prototype, IPersistence* _objectAttribute, std::string _attributeName) = 0;
		virtual bool insert(IPersistenceList* _objects) = 0;
		virtual bool insert(IPersistence* _object) = 0;
		virtual bool update(IPersistence* _object) = 0;
		virtual bool updatePrimitiveAttributes(IPersistence* _object) = 0;
		virtual bool updateObjectAttribute(IPersistence* _objectUpdate, std::string _attributeName) = 0;
		virtual bool updateAttributesList_1_N(IPersistence* _objeto, bool _cascade = true) = 0;
		virtual bool getAttributeObject(IPersistence* _parentObject, std::string _attributeName, AttributeValue* _value) = 0;
		virtual bool getAttributesPrimitive(IPersistence* _parentObject, std::string _attributeName, AttributeValue* _value) = 0;
		virtual bool getIPersistence(IPersistence* _object) = 0;
		virtual bool deleteObjects(IPersistenceList* _objects) = 0;
		virtual bool deleteObject(IPersistence* _object) = 0;
	};
}
#endif