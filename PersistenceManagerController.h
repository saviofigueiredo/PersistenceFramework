#ifndef PERSISTENCEMANAGERCONTROLLER
#define PERSISTENCEMANAGERCONTROLLER

#include <unordered_map>

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;
	class IPersistenceManager;
	class IPersistenceManagerConnection;
	class IPersistenceManagerGet;
	class IPersistenceManagerQuery;
	class IPersistenceManagerUpdate;
	class IPersistenceManagerDelete;
	class IPersistenceManagerInsert;

	class PersistenceManagerController
	{
	private:
		/**
		*	Stores the a mapping between the class name and the list of objects already retrieved for that class.
		**/
		static std::unordered_map<std::string, IPersistenceList*> mappingClassNameObjects;

	public:
		static IPersistenceManager* managerPersistence;
		static IPersistenceManager* obterGerentePersistencia();
		static IPersistenceManagerConnection*  managerConnection;
		static IPersistenceManagerQuery*    managerQuery;
		static IPersistenceManagerDelete*  managerDelete;
		static IPersistenceManagerGet*      managerGet;
		static IPersistenceManagerInsert* managerInsert;
		static IPersistenceManagerUpdate*   managerUpdate;

		static void freeMemory();

		static void setList(IPersistenceList* _objects);
		static void removeList();
		static void removeObject(IPersistence* _object);

		static IPersistence* getInstance(std::string _className, std::string _valorChavePrimaria);
	};
}

#endif