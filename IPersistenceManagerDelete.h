#ifndef IPERSISTENCEMANAGERDELETE_H
#define IPERSISTENCEMANAGERDELETE_H

#include <string>

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;

	class IPersistenceManagerDelete
	{
	public:
		virtual bool deletaList(IPersistenceList* _objects) = 0;
		virtual bool deleteObject(IPersistence* _object) = 0;
	};
}

#endif