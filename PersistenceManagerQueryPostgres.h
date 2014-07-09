#ifndef PERSISTENCEMANAGERQUERYPOSTGRES_H
#define PERSISTENCEMANAGERQUERYPOSTGRES_H

#include "IPersistenceManagerQuery.h"
#include <string>
#include <list>

namespace PersistenceFramework
{

	class IPersistenceManager;
	class IPersistenceManagerGet;
	class IPersistence;

	class PersistenceManagerQueryPostgres : IPersistenceManagerQuery
	{
	private:
		IPersistenceManager* persistenceManager;
		IPersistenceManagerGet* managerGet;
	public:
		PersistenceManagerQueryPostgres();

		void setPersistenceManager(IPersistenceManager* _persistenceManager);
		void atribuirGerenteGet(IPersistenceManagerGet* _managerGet);

		virtual std::string getQueryInsert(IPersistence* _object) = 0;

		virtual std::string getQueryBiggestValue(const std::string& _tableName, const std::string& _columnName, const std::string& _qualifier) = 0;

		virtual std::string getQueryUpdatePrimitiveAttribute(const std::string &_tableName, const std::list<std::pair<std::string, std::pair<std::string, int>>>& _colunasAtributosValoresAtributos, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyValue) = 0;
	};
}

#endif