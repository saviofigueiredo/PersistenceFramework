#include "Attribute.h"

namespace PersistenceFramework
{
	Attribute::Attribute(Attribute* _attribute)
	{
		this->type = _attribute->type;
		this->name = _attribute->name;
		this->className = _attribute->className;
		this->isPrimaryKey = _attribute->isPrimaryKey;
		this->nameAttributeReferencedObject = _attribute->nameAttributeReferencedObject;
		this->attributeDeclaringClassName = _attribute->attributeDeclaringClassName;
		this->setIsProxy(_attribute->getIsProxy());
	}

	Attribute::Attribute()
	{
		this->type = AttributeType::_invalid;
		this->name = "";
		this->className = "";
		this->enumValues = nullptr;
		this->isPrimaryKey = false;
		this->nameAttributeReferencedObject = "";
		this->setIsProxy(false);
	}

	Attribute::Attribute(std::string _attributeDeclaringClassName, AttributeType _type, char* _name, bool _isPrimaryKey, bool _isProxy)
	{
		this->type = _type;
		this->name = _name;
		this->enumValues = nullptr;
		this->attributeDeclaringClassName = _attributeDeclaringClassName;
		this->isPrimaryKey = _isPrimaryKey;
		this->nameAttributeReferencedObject = "";
		this->setIsProxy(_isProxy);
	}

	Attribute::Attribute(std::string _attributeDeclaringClassName, std::string _className, AttributeType _type, char* _name, bool _isProxy)
	{
		this->type = _type;
		this->name = _name;
		this->className = _className;
		this->enumValues = nullptr;
		this->attributeDeclaringClassName = _attributeDeclaringClassName;
		this->isPrimaryKey = false;
		this->nameAttributeReferencedObject = "";
		this->setIsProxy(_isProxy);
	}

	Attribute::Attribute(std::string _attributeDeclaringClassName, std::string _className, AttributeType _type, char* _name, std::string _nameAttributeReferencedObject, bool _isProxy)
	{
		this->type = _type;
		this->name = _name;
		this->className = _className;
		this->enumValues = NULL;
		this->attributeDeclaringClassName = _attributeDeclaringClassName;
		this->isPrimaryKey = false;
		this->nameAttributeReferencedObject = _nameAttributeReferencedObject;
		this->setIsProxy(_isProxy);
	}

	Attribute::Attribute(std::string attributeDeclaringClassName, AttributeType _type, char* _name, std::map<std::string, std::string>* _enumValues, bool _isProxy)
	{
		this->type = _type;
		this->name = _name;
		this->enumValues = _enumValues;
		this->isPrimaryKey = false;
		this->setIsProxy(_isProxy);
	}

	Attribute::~Attribute()
	{

	}

	bool Attribute::getIsProxy()
	{
		return isProxy;
	}

	void Attribute::setIsProxy(bool _isProxy)
	{
		this->isProxy = _isProxy;
	}

	std::string Attribute::getNameAttributeReferencedObject()
	{
		return nameAttributeReferencedObject;
	}

	bool Attribute::getIsPrimaryKey()
	{
		return isPrimaryKey;
	}

	std::string Attribute::getName()
	{
		return name;
	}

	std::string Attribute::getClassName()
	{
		return className;
	}

	bool Attribute::isPrimitive()
	{
		if ((type == _object) ||
			(type == _list_1_N) ||
			(type == _list_N_N))
		{
			return false;
		}

		return true;
	}

	std::map<std::string, std::string>* Attribute::getEnumValues()
	{
		return enumValues;
	}

	std::string Attribute::getAttributeDeclaringClassName()
	{
		return attributeDeclaringClassName;
	}

	AttributeType Attribute::getType()
	{
		return type;
	}
}