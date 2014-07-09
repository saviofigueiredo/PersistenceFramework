#include "AttributeValue.h"
#include "Attribute.h"
#include "DateTimeHelper.h"

namespace PersistenceFramework
{
	AttributeValue::AttributeValue()
	{
		this->address.pInt = NULL;
		this->attribute = NULL;
	}

	AttributeValue::~AttributeValue()
	{
	}

	AddressAttributeValue AttributeValue::getAddress()
	{
		return address;
	}

	AttributeValue::AttributeValue(Attribute* _attribute, int* _address)
	{
		this->address.pInt = _address;
		if (this->address.pInt != NULL)
			*(this->address.pInt) = 0;

		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeValue::AttributeValue(Attribute* _attribute, unsigned long long *_address)
	{
		this->address.pLong = _address;
		if (this->address.pLong != NULL)
			*(this->address.pLong) = 0;

		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeValue::AttributeValue(Attribute* _attribute, float* _address)
	{
		this->address.pFloat = _address;
		if (this->address.pFloat != NULL)
			*(this->address.pFloat) = 0.0;

		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeValue::AttributeValue(Attribute* _attribute, double* _address)
	{
		this->address.pDouble = _address;
		if (this->address.pDouble != NULL)
			*(this->address.pDouble) = 0.0;

		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeValue::AttributeValue(Attribute* _attribute, time_t * _address)
	{
		this->address.pDate = _address;
		if (this->address.pDate != NULL)
			*(this->address.pDate) = 0;

		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeValue::AttributeValue(Attribute* _attribute, std::string* _address)
	{
		this->address.pString = _address;
		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeValue::AttributeValue(Attribute* _attribute, char * _address)
	{
		this->address.pChar = _address;
		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeValue::AttributeValue(Attribute* _attribute, IPersistence ** _address)
	{
		this->address.ppObject = _address;
		if (this->address.ppObject != NULL)
			*(this->address.ppObject) = NULL;

		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeValue::AttributeValue(Attribute* _attribute, IPersistenceList** _address)
	{
		this->address.ppList = _address;
		this->attribute = _attribute;

		this->setHasBeenRetrieved(false);
	}

	AttributeType AttributeValue::getType()
	{
		return attribute->getType();
	}

	void AttributeValue::setHasBeenRetrieved(bool _retrieved)
	{
		retrieved = _retrieved;
	}

	bool AttributeValue::hasBeenRetrieved()
	{
		return retrieved;
	}
	bool AttributeValue::isProxy()
	{
		return attribute->getIsProxy();
	}

	std::string AttributeValue::getClassName()
	{
		return attribute->getClassName();
	}

	std::string AttributeValue::getName()
	{
		return attribute->getName();
	}

	std::string AttributeValue::getNameAttributeReferencedObject()
	{
		return attribute->getNameAttributeReferencedObject();
	}

	std::string AttributeValue::getValue()
	{
		std::string returnValue;
		char temp[500];

		if (getType() == AttributeType::_long)
		{
			sprintf(temp, "%ld", *address.pLong);
			returnValue.assign(temp);
		}
		else if (getType() == AttributeType::_int || getType() == AttributeType::_bool)
		{
			sprintf(temp, "%d", *address.pInt);
			returnValue.assign(temp);
		}
		else if (getType() == AttributeType::_float)
		{
			sprintf(temp, "%.0f", *address.pFloat);
			returnValue.assign(temp);
		}
		else if (getType() == AttributeType::_double)
		{
			sprintf(temp, "%.0f", *address.pDouble);
			returnValue.assign(temp);
		}
		else if (getType() == AttributeType::_char)
		{
			returnValue = *address.pChar;
		}
		else if (getType() == AttributeType::_string)
		{
			returnValue = *address.pString;
		}
		else if (getType() == AttributeType::_enum)
		{
			sprintf(temp, "%d", *address.pInt);
			returnValue.assign(temp);
		}
		else if (getType() == AttributeType::_date)
		{
			returnValue = DateTimeHelper::ConverteDateTimeToString(*address.pDate);
		}

		return returnValue;
	}

	void AttributeValue::setValue(std::string _value, std::string _dateFormat)
	{
		if (getType() == AttributeType::_int || getType() == AttributeType::_bool)
		{
			int valorTemp;

			valorTemp = atoi(_value.c_str());
			*address.pInt = valorTemp;
			return;
		}
		else if (getType() == AttributeType::_float)
		{
			float valorTemp;

			valorTemp = (float)atof(_value.c_str());
			*address.pFloat = valorTemp;
			return;
		}
		else if (getType() == AttributeType::_double)
		{
			double valorTemp;

			valorTemp = atof(_value.c_str());
			*address.pDouble = valorTemp;
			return;
		}
		else if (getType() == AttributeType::_char)
		{
			if (_value.empty())
			{
				*address.pChar = '\0';
				return;
			}

			*address.pChar = _value[0];
			return;
		}
		else if (getType() == AttributeType::_string)
		{
			*address.pString = _value;
			return;
		}
		else if (getType() == AttributeType::_enum)
		{
			int valorTemp;

			valorTemp = atoi(_value.c_str());
			*address.pInt = valorTemp;
			return;
		}
		else if (getType() == AttributeType::_date)
		{
			*address.pDate = DateTimeHelper::ConvertDateTime(_value.c_str());
			return;
		}
	}
	void AttributeValue::setValue(IPersistence* _address)
	{
		*(this->address.ppObject) = _address;
	}
	void AttributeValue::setValue(IPersistenceList* _address)
	{
		*(this->address.ppList) = _address;
	}

	bool AttributeValue::isPrimitive()
	{
		if ((attribute->getType() == AttributeType::_object) || (attribute->getType() == AttributeType::_list_1_N) || (attribute->getType() == AttributeType::_list_N_N))
		{
			return false;
		}

		return true;
	}
	bool AttributeValue::isPrimaryKey()
	{
		return attribute->getIsPrimaryKey();
	}

	Attribute* AttributeValue::getAttribute()
	{
		return attribute;
	}
}