#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "IPersistence.h"
#include "PersistenceMacros.h"
#include <string>

namespace PersistenceFrameworkTest
{
	using namespace PersistenceFramework;

	class Customer : public IPersistence
	{
	private:
		unsigned long long id;
		std::string name;
		std::string email;

	public:
		Customer();
		virtual ~Customer();

		std::string getName();
		void setName(std::string customerName);

		std::string getEmail();
		void setEmail(std::string customerEmail);

		void setId(unsigned long long customerId);
		unsigned long long getId();
		
		DECLARE_SERIAL(Customer)
	};
}

#endif