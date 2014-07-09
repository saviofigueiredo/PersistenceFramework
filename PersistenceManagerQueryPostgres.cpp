#include "PersistenceManagerQueryPostgres.h"
#include "IPersistenceManager.h"
#include "IPersistenceManagerGet.h"

namespace PersistenceFramework
{
	PersistenceManagerQueryPostgres::PersistenceManagerQueryPostgres()
	{
		persistenceManager = nullptr;
		managerGet = nullptr;
	}

	void PersistenceManagerQueryPostgres::setPersistenceManager(IPersistenceManager* _persistenceManager)
	{
		persistenceManager = _persistenceManager;
	}
	
	void PersistenceManagerQueryPostgres::atribuirGerenteGet(IPersistenceManagerGet* _managerGet)
	{
		managerGet = managerGet;
	}

	// TODO: Finish this method.
	std::string PersistenceManagerQueryPostgres::getQueryInsert(IPersistence* _object)
	{
		return "";
	}

	// TODO: Finish this method.
	std::string PersistenceManagerQueryPostgres::getQueryBiggestValue(const std::string& _tableName, const std::string& _columnName, const std::string& _qualifier)
	{
		return "";
	}

	// TODO: Finish this method.
	std::string PersistenceManagerQueryPostgres::getQueryUpdatePrimitiveAttribute(const std::string &_tableName, const std::list<std::pair<std::string, std::pair<std::string, int>>>& _colunasAtributosValoresAtributos, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyValue)
	{
		return "";
	}

}