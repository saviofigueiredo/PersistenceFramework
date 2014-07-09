#ifndef IPERSISTENCEMANAGERCONNECTION_H
#define IPERSISTENCEMANAGERCONNECTION_H

#include <string>

namespace PersistenceFramework
{
	class Table;
	class Database;

	class IPersistenceManagerConnection
	{
	protected:
		Database* database;

	public:
		IPersistenceManagerConnection();
		virtual ~IPersistenceManagerConnection();

		virtual std::string getConnectionString() = 0;

		Database* connect();
		bool runQuery(std::string _query, Table& _table);
	};
}

#endif