#ifndef PERSISTENCEMANAGERINSERT_H
#define PERSISTENCEMANAGERINSERT_H

#include "IPersistenceManagerInsert.h"

namespace PersistenceFramework
{
	class IPersistenceManagerQuery;
	class IPersistenceManagerConnection;
	class IPersistenceManagerUpdate;

	class PersistenceManagerInsert : public IPersistenceManagerInsert
	{
	private:
		IPersistenceManagerQuery* managerQuery;
		IPersistenceManagerConnection* managerConnection;
		IPersistenceManagerUpdate* managerUpdate;

	public:
		PersistenceManagerInsert(IPersistenceManagerQuery*, IPersistenceManagerConnection*);
		void setManagerUpdate(IPersistenceManagerUpdate* _managerUpdate);

		virtual bool insert(IPersistence* _object);
		virtual bool insert(IPersistenceList* _objects);
	};
}

#endif