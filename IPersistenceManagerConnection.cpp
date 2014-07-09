#include "IPersistenceManagerConnection.h"
#include "Database.h"

namespace PersistenceFramework
{
	IPersistenceManagerConnection::IPersistenceManagerConnection()
	{
		database = new Database();
	}

	IPersistenceManagerConnection::~IPersistenceManagerConnection()
	{
		delete database;
		database = NULL;
	}

	Database* IPersistenceManagerConnection::connect()
	{
		char ErrStr[500];
		::CoInitialize(NULL);
		std::string connectionString = this->getConnectionString();

		static bool databaseIsConnected = false;

		if (databaseIsConnected == false)
		{
			if (database->Open("", "", connectionString.c_str()) == false)
			{
				database->GetErrorErrStr(ErrStr);
				return NULL;
			}

			databaseIsConnected = true;
		}
		return database;
	}


	bool IPersistenceManagerConnection::runQuery(std::string _query, Table& _table)
	{
		char ErrStr[500];
		::CoInitialize(NULL);
		Database* db = connect();

		if (db != NULL && db->OpenTbl(ADODB::adCmdText, _query.c_str(), _table) == false)
		{
			db->GetErrorErrStr(ErrStr);
			return false;
		}

		return true;
	}
}