#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <map>
#include <list>
#include "AttributeType.h"

namespace PersistenceFramework
{
	class Attribute
	{
	private:

		std::string attributeDeclaringClassName;

		AttributeType type;

		std::string name;

		std::string className;

		std::string nameAttributeReferencedObject;

		std::map<std::string, std::string>* enumValues;

		bool isPrimaryKey;

		bool isProxy;

	public:
		Attribute();
		virtual ~Attribute();

		Attribute(std::string _attributeDeclaringClassName, AttributeType _type, char* _name, bool _isPrimaryKey, bool _isProxy);
		Attribute(std::string _attributeDeclaringClassName, std::string _className, AttributeType _type, char* _name, bool _isProxy);
		Attribute(std::string _attributeDeclaringClassName, std::string _className, AttributeType _type, char* _name, std::string _nameAttributeReferencedObject, bool _isProxy);
		Attribute(std::string _attributeDeclaringClassName, AttributeType _type, char* _name, std::map<std::string, std::string>* _enumValues, bool _isProxy);
		Attribute(Attribute* _attribute);

		std::string getName();

		std::string getClassName();
		std::string getNameAttributeReferencedObject();

		bool isPrimitive();
		bool getIsPrimaryKey();
		bool getIsProxy();
		void setIsProxy(bool);
		AttributeType getType();

		std::map<std::string, std::string>* getEnumValues();

		std::string getAttributeDeclaringClassName();
	};
}

#endif