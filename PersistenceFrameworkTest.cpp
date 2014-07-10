#include <iostream>
#include "Customer.h"
#include "ClassesManager.h"
#include "IPersistence.h"
#include "PersistenceManager.h"
#include "IPersistenceManager.h"
#include "IPersistenceManagerConnection.h"
#include "IPersistenceManagerQuery.h"
#include "PersistenceManagerQueryPostgres.h"
#include "IPersistenceManagerDelete.h"
#include "PersistenceManagerDelete.h"
#include "IPersistenceManagerGet.h"
#include "PersistenceManagerInsert.h"
#include "IPersistenceManagerInsert.h"
#include "IPersistenceManagerUpdate.h"
#include "PersistenceManagerUpdate.h"
#include "PersistenceManagerConnection.h"

using namespace PersistenceFramework;
using namespace PersistenceFrameworkTest;

void addClasses()
{
	static bool classesAdded = false;

	if (classesAdded == false)
	{
		ClassesManager::get()->addClass(new Customer());
	}

	classesAdded = true;
}

IPersistence* getInstance(std::string _className)
{
	addClasses();
	return ClassesManager::get()->newObject(_className);
}

void removeClasses()
{
	addClasses();
	ClassesManager::get()->deleteClasses();
	ClassesManager::get()->freeMemory();
}

IPersistenceManager* getPersistenceManager()
{
	IPersistenceManagerConnection* managerConnection = new PersistenceManagerConnection();
	IPersistenceManagerQuery* managerQuery = new PersistenceManagerQueryPostgres();
	IPersistenceManagerDelete* managerDelete = new PersistenceManagerDelete(managerQuery, managerConnection);
	//IPersistenceManagerGet* managerGet = new PersistenceManagerGet(managerConnection, managerQuery);
	IPersistenceManagerInsert* managerInsert = new PersistenceManagerInsert(managerQuery, managerConnection);
	IPersistenceManagerUpdate* managerUpdate = new PersistenceManagerUpdate(managerConnection, managerDelete, nullptr, managerQuery, managerInsert);

	IPersistenceManager* persistenceManager = new PersistenceManager(managerConnection, managerDelete, nullptr, managerUpdate, managerQuery, managerInsert);

	reinterpret_cast<PersistenceManagerQueryPostgres*>(managerQuery)->setPersistenceManager(persistenceManager);
	reinterpret_cast<PersistenceManagerQueryPostgres*>(managerQuery)->setManagerGet(nullptr);

	return persistenceManager;
}

bool getCustomers(IPersistenceList* _customers)
{
	auto customerPrototype = new Customer();

	auto persistenceManager = getPersistenceManager();

	auto ok = persistenceManager->getIPersistenceList(_customers, customerPrototype);

	delete customerPrototype;

	return ok;
}

int main(int argc, char* argv[])
{

	return EXIT_SUCCESS;
}
