#include "Customer.h"
#include "ClassesManager.h"

namespace PersistenceFrameworkTest
{
	using namespace PersistenceFramework;

	IMPLEMENT_SERIAL(Customer, IPersistence, CUSTOMER)

	Customer::Customer()
	{
		IMPLEMENT_SERIAL_CONSTRUTOR
	}

	Customer::~Customer()
	{
	}

	std::string Customer::getName()
	{
		return name;
	}

	void Customer::setName(std::string customerName)
	{
		name = customerName;
	}

	std::string Customer::getEmail()
	{
		return email;
	}

	void Customer::setEmail(std::string customerEmail)
	{
		email = customerEmail;
	}

	void Customer::setId(unsigned long long customerId)
	{
		id = customerId;
	}

	unsigned long long Customer::getId()
	{
		return id;
	}

	void Customer::setAttributes()
	{
		BEGIN_DEFINING_MAPPING_BETWEEN_ATTRIBUTES_AND_COLUMNS
		ADD_ATTRIBUTE_LONG(id, ID, true, false)
		ADD_ATTRIBUTE_STRING(name, NAME, false)
		ADD_ATTRIBUTE_STRING(email, EMAIL, false)
		END_DEFINING_MAPPING_BETWEEN_ATTRIBUTES_AND_COLUMNS
	}

	void Customer::defineDeleteRules()
	{
		BEGIN_DEFINING_DELETE_RULES
		END_DEFINING_DELETE_RULES
	}

	IPersistence* Customer::newObject()
	{
		return new Customer();
	}
}