#ifndef ATTRIBUTEVALUE_H
#define ATTRIBUTEVALUE_H

#include "AddressAttributeValue.h"
#include "AttributeType.h"

#include <string>

namespace PersistenceFramework
{
	class Attribute;

	class AttributeValue
	{
	private:
		AddressAttributeValue address;
		Attribute* attribute;
		bool retrieved;

	public:
		AttributeValue();
		virtual ~AttributeValue();

		AttributeValue(Attribute* _attribute, IPersistenceList **_address);
		AttributeValue(Attribute* _attribute, int *_address);
		AttributeValue(Attribute* _attribute, unsigned long long *_address);
		AttributeValue(Attribute* _attribute, float *_address);
		AttributeValue(Attribute* _attribute, double *_address);
		AttributeValue(Attribute* _attribute, std::string *_address);
		AttributeValue(Attribute* _attribute, time_t *_address);
		AttributeValue(Attribute* _attribute, char *_address);
		AttributeValue(Attribute* _attribute, IPersistence **_address);

		AttributeType getType();

		void setHasBeenRetrieved(bool);
		bool hasBeenRetrieved();
		bool isProxy();

		std::string getName();

		std::string getClassName();
		std::string getNameAttributeReferencedObject();

		AddressAttributeValue getAddress();

		std::string getValue();
		void setValue(std::string _value, std::string _dateFormat = "");
		void setValue(IPersistence* _address);
		void setValue(IPersistenceList* _address);

		bool isPrimitive();
		bool isPrimaryKey();

		Attribute* getAttribute();
	};
}

#endif