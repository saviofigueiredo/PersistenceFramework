#ifndef PERSISTENCEMANAGERDELETE_H
#define PERSISTENCEMANAGERDELETE_H

#include <string>
#include "IPersistenceManagerDelete.h"

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;
	class IPersistenceManagerQuery;
	class IPersistenceManagerConnection;

	class PersistenceManagerDelete : public IPersistenceManagerDelete
	{
	private:
		IPersistenceManagerQuery* managerQuery;
		IPersistenceManagerConnection* managerConnection;

	public:
		PersistenceManagerDelete(IPersistenceManagerQuery* _managerQuery, IPersistenceManagerConnection* _managerConnection);

		virtual bool deleteList(IPersistenceList* _objects);
		virtual bool deleteObject(IPersistence* _object);
	};
}

#endif