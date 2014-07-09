#ifndef ATTRIBUTEVALUELIST_H
#define ATTRIBUTEVALUELIST_H

#include <list>
#include "AttributeValue.h"
#include "AttributeList.h"

namespace PersistenceFramework
{
	class AttributeValueList : public std::list<AttributeValue*>
	{
	public:
		void getAttributes(AttributeList* _attributes);
		void push_back(AttributeValueList* _values);
		void push_back(AttributeValue* _value);

		AttributeValue* get(std::string _nome);
	};
}

#endif