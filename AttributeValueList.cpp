#include "AttributeValueList.h"
#include "Attribute.h"

namespace PersistenceFramework
{
	AttributeValue* AttributeValueList::get(std::string _name)
	{
		for (std::list<AttributeValue*>::iterator it = this->begin(); it != this->end(); it++)
		{
			if ((*it)->getAttribute()->getName() == _name)
				return (*it);
		}
		return NULL;
	}

	void AttributeValueList::push_back(AttributeValueList* _list)
	{
		for (std::list<AttributeValue*>::iterator it = _list->begin(); it != _list->end(); it++)
		{
			this->push_back(*it);
		}
	}

	void AttributeValueList::push_back(AttributeValue* _value)
	{
		std::list<AttributeValue*>::push_back(_value);
	}

	void AttributeValueList::getAttributes(AttributeList* _attributes)
	{
		AttributeValue* value;
		for (std::list<AttributeValue*>::iterator it = this->begin(); it != this->end(); it++)
		{
			value = *it;

			_attributes->push_back(value->getAttribute());
		}
	}
}