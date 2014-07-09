#ifndef IPERSISTENCEMANAGERINSERT_H
#define IPERSISTENCEMANAGERINSERT_H

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;

	class IPersistenceManagerInsert
	{
	public:
		virtual bool insert(IPersistence* _object) = 0;
		virtual bool insert(IPersistenceList* _objects) = 0;
	};
}

#endif