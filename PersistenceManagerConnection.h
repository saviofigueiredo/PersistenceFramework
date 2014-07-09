#ifndef PERSISTENCEMANAGERCONNECTION_H
#define PERSISTENCEMANAGERCONNECTION_H

#include "IPersistenceManagerConnection.h"

namespace PersistenceFramework
{
	class PersistenceManagerConnection : IPersistenceManagerConnection
	{
	public:
		virtual std::string getConnectionString();
	};
}

#endif