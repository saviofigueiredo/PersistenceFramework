#include "PersistenceManagerConnection.h"

namespace PersistenceFramework
{
	std::string PersistenceManagerConnection::getConnectionString()
	{
		return "DSN=PostGreSQL35W;Server=localhost;Port=5432;Database=TestCompany;Uid = mbsuser; Pwd = mbsuser; ";
	}
}