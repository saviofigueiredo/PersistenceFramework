#ifndef PERSISTENCEMANAGERUPDATE_H
#define PERSISTENCEMANAGERUPDATE_H

#include <string>
#include "IPersistenceManagerUpdate.h"

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;
	class AttributeValue;

	class IPersistenceManagerConnection;
	class IPersistenceManagerDelete;
	class IPersistenceManagerGet;
	class IPersistenceManagerQuery;
	class IPersistenceManagerInsert;

	class PersistenceManagerUpdate : IPersistenceManagerUpdate
	{
	private:
		IPersistenceManagerConnection* managerConnection;
		IPersistenceManagerDelete* managerDelete;
		IPersistenceManagerGet* managerGet;
		IPersistenceManagerQuery* managerQuery;
		IPersistenceManagerInsert* managerInsert;

	public:
		PersistenceManagerUpdate(IPersistenceManagerConnection* _managerConnection,
			IPersistenceManagerDelete* _managerDelete,
			IPersistenceManagerGet* _managerGet,
			IPersistenceManagerQuery* _managerQuery,
			IPersistenceManagerInsert* _managerInsert);

		virtual bool update(IPersistence* _object);
		virtual bool updateAttributesPrimitive(IPersistence* _object);
		virtual bool updateAttributeObject(IPersistence* _objectToUpdate, std::string _attributeName);
		virtual bool updateAttributesObject(IPersistence* _objectToUpdate);
		virtual bool updateAttributesList_1_N(IPersistence* _object, bool _cascade = true);
		virtual bool updateParent(IPersistence* _parentObject, AttributeValue* _value, IPersistenceList* _childObjects);
	};
}

#endif