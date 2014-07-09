#include "IPersistence.h"
#include "IPersistenceList.h"
#include "AttributeValueList.h"
#include "AttributeValue.h"

namespace PersistenceFramework
{
	IMPLEMENT_RUNTIME_CLASS(IPersistence)

	IPersistence::IPersistence()
	{
		attributeValues = new std::unordered_map<std::string, AttributeValue*>;
		primaryKeyValue = NULL;
		primaryKeyAttributeName = "";
	}

	IPersistence::~IPersistence()
	{
		auto iter = attributeValues->begin();
		auto endIter = attributeValues->end();
		for (; iter != endIter; ++iter)
		{
			delete iter->second;
		}

		attributeValues->clear();
		delete attributeValues;
	}

	bool IPersistence::hasBeenRetrieved()
	{
		for (auto it = attributeValues->begin(); it != attributeValues->end(); ++it)
		{
			auto currentValue = it->second;

			if (currentValue->hasBeenRetrieved() == false)
				return false;
		}
		return true;
	}

	std::string IPersistence::getClassName()
	{
		return "IPersistence";
	}

	std::string IPersistence::getTableName()
	{
		return "";
	}

	std::unordered_map<std::string, AttributeValue*>* IPersistence::getValues()
	{
		return attributeValues;
	}

	void IPersistence::getValues(std::unordered_map<std::string, AttributeValue*>* _list)
	{
		for (auto it = attributeValues->begin(); it != attributeValues->end(); it++)
		{
			_list->insert(std::pair<std::string, AttributeValue*>(it->second->getName(), it->second));
		}
	}

	void IPersistence::getValues(AttributeValueList* _values)
	{
		for (auto it = attributeValues->begin(); it != attributeValues->end(); it++)
		{
			_values->push_back(it->second);
		}
	}

	void IPersistence::setValues(std::unordered_map<std::string, AttributeValue*>* _attributeValues, bool _partial, bool _recursive)
	{
		AttributeValue* attributeValue;
		AttributeValue* attributeValueMatching;

		for (auto it = attributeValues->begin(); it != attributeValues->end(); it++)
		{
			attributeValue = it->second;

			auto itMatching = _attributeValues->find(attributeValue->getName());

			if (itMatching != _attributeValues->end())
				attributeValueMatching = itMatching->second;
			else
				attributeValueMatching = NULL;

			if ((attributeValueMatching != NULL) && (attributeValue->getType() == attributeValueMatching->getType()))
			{
				if (attributeValue->getType() == _int || attributeValue->getType() == _bool)
				{
					*attributeValue->getAddress().pInt = *attributeValueMatching->getAddress().pInt;
				}
				else if (attributeValue->getType() == _float)
				{
					*attributeValue->getAddress().pFloat = *attributeValueMatching->getAddress().pFloat;
				}
				else if (attributeValue->getType() == _double)
				{
					*attributeValue->getAddress().pDouble = *attributeValueMatching->getAddress().pDouble;
				}
				else if (attributeValue->getType() == _date)
				{
					*attributeValue->getAddress().pDate = *attributeValueMatching->getAddress().pDate;
				}
				else if (attributeValue->getType() == _string)
				{
					*attributeValue->getAddress().pString = *attributeValueMatching->getAddress().pString;
				}
				else if (attributeValue->getType() == _char)
				{
					*attributeValue->getAddress().pChar = *attributeValueMatching->getAddress().pChar;
				}
				else if (attributeValue->getType() == _enum)
				{
					*attributeValue->getAddress().pEnum = *attributeValueMatching->getAddress().pEnum;
				}
				else if ((attributeValue->getType() == _object) && !_partial)
				{
					auto object = *attributeValue->getAddress().ppObject;
					auto objectMatching = *attributeValueMatching->getAddress().ppObject;

					if (_recursive == false)
						*attributeValue->getAddress().ppObject = *attributeValueMatching->getAddress().ppObject;
					else
					{
						if (objectMatching != nullptr)
						{
							object->setValues(objectMatching->getValues(), _partial, _recursive);
						}
						else
							object = nullptr;
					}
				}
				else if ((attributeValue->getType() == _list_1_N || attributeValue->getType() == _list_N_N) && !_partial)
				{
					if (_recursive == false)
					{
						(*attributeValue->getAddress().ppList)->clear();
						(*attributeValue->getAddress().ppList)->push_back(*attributeValueMatching->getAddress().ppList);
					}
					else
					{
						auto* list = *attributeValue->getAddress().ppList;
						auto listMatching = *attributeValueMatching->getAddress().ppList;

						list->clear();
						for (auto it2 = listMatching->begin(); it2 != listMatching->end(); it2++)
						{
							IPersistence* objectMatching = *it2;
							IPersistence* object = nullptr;

							if (_recursive == false)
								*attributeValue->getAddress().ppObject = *attributeValueMatching->getAddress().ppObject;
							else
							{
								if (objectMatching != nullptr)
								{
									object->setValues(objectMatching->getValues(), _partial, _recursive);
								}
								else
									object = NULL;
							}

							list->push_back(object);
						}
					}
				}
			}
		}
	}

	bool IPersistence::isEqual(IPersistence* _object, bool _recursive)
	{
		std::unordered_map<std::string, AttributeValue*> attributeValuesComparedObject;
		std::unordered_map<std::string, AttributeValue*> attributeValuesObject;

		AttributeValue* attributeValueComparedObject;
		AttributeValue* attributeValue;
		bool returnValue = true;

		if (getClassName() != _object->getClassName())
			returnValue = false;
		else if (this == NULL && _object == NULL)
			returnValue = true;
		else if (this == NULL || _object == NULL)
			returnValue = false;
		else
		{
			_object->getValues(&attributeValuesComparedObject);
			getValues(&attributeValuesObject);

			auto itComparado = attributeValuesComparedObject.begin();

			for (auto it = attributeValuesObject.begin(); returnValue && (it != attributeValuesObject.end(), itComparado != attributeValuesObject.end());
				it++, itComparado++)
			{
				attributeValue = it->second;
				attributeValueComparedObject = itComparado->second;

				if (attributeValue->getType() == AttributeType::_int || attributeValue->getType() == AttributeType::_bool)
				{
					if (*attributeValue->getAddress().pInt != *attributeValueComparedObject->getAddress().pInt)
						returnValue = false;
				}
				else if (attributeValue->getType() == AttributeType::_float)
				{
					if (*attributeValue->getAddress().pFloat != *attributeValueComparedObject->getAddress().pFloat)
						returnValue = false;
				}
				else if (attributeValue->getType() == AttributeType::_double)
				{
					if (*attributeValue->getAddress().pDouble != *attributeValueComparedObject->getAddress().pDouble)
						returnValue = false;
				}
				else if (attributeValue->getType() == AttributeType::_date)
				{
					if (*attributeValue->getAddress().pDate != *attributeValueComparedObject->getAddress().pDate)
						returnValue = false;
				}
				else if (attributeValue->getType() == AttributeType::_string)
				{
					if (*attributeValue->getAddress().pString != *attributeValueComparedObject->getAddress().pString)
						returnValue = false;
				}
				else if (attributeValue->getType() == AttributeType::_char)
				{
					if (*attributeValue->getAddress().pChar != *attributeValueComparedObject->getAddress().pChar)
						returnValue = false;
				}
				else if (attributeValue->getType() == AttributeType::_enum)
				{
					if (attributeValue->getAddress().pEnum != attributeValueComparedObject->getAddress().pEnum)
						returnValue = false;
				}
				else if ((attributeValue->getType() == AttributeType::_object) && (_recursive == true))
				{
					auto objecdtAttribute = *attributeValue->getAddress().ppObject;
					auto objectAttributeCompared = *attributeValueComparedObject->getAddress().ppObject;

					if (!(objecdtAttribute->isEqual(objectAttributeCompared, false)))
						returnValue = false;
				}
				else if ((attributeValue->getType() == _list_1_N || attributeValue->getType() == _list_N_N) && (_recursive == true))
				{
					if (!((*attributeValue->getAddress().ppList)->isEqual(*attributeValueComparedObject->getAddress().ppList, false)))
						returnValue = false;
				}
			}
		}

		return returnValue;
	}

	void IPersistence::addAttributeValue(AttributeValue* _attributeValue)
	{
		attributeValues->insert(std::pair<std::string, AttributeValue*>(_attributeValue->getName(), _attributeValue));
	}

	void IPersistence::addAttributeValue(AttributeValue* _attributeValue, IPersistenceList* _list)
	{
		addAttributeValue(_attributeValue);

		*(_attributeValue->getAddress().ppList) = _list;
	}

	void IPersistence::getAttributes(AttributeList* _attributes)
	{
		for (auto it = attributeValues->begin(); it != attributeValues->end(); ++it)
		{
			_attributes->push_back(it->second->getAttribute());
		}
	}

	void IPersistence::getAttributeNames(std::list<std::string>& _attributeNames)
	{
		for (auto it = attributeValues->begin(); it != attributeValues->end(); ++it)
			_attributeNames.push_back(it->first);
	}

	IPersistence* IPersistence::setAttributeValue(std::string _attributeName, std::string _attributeValue, IPersistenceList* _objects)
	{
		AttributeValue* attributeValue = getValue(_attributeName);

		if (attributeValue != nullptr)
		{
			if (attributeValue->getType() == _object)
			{
				for (auto it = _objects->begin(); it != _objects->end(); ++it)
				{
					IPersistence* currentObject = (*it);

					if (_attributeValue == currentObject->toString())
					{
						(*attributeValue->getAddress().ppObject) = currentObject;
						return currentObject;
					}
				}
			}
		}
		return nullptr;
	}

	void IPersistence::setAttributeValueString(std::string& _value, std::string _newValor)
	{
		if (strcmp(_value.c_str(), _newValor.c_str()) == 0)
			return;

		_value = _newValor;
	}

	void IPersistence::setAttributeValueInt(int& _value, int _newValor)
	{
		if (_value == _newValor)
			return;

		_value = _newValor;
	}

	void IPersistence::setAttributeValueFloat(float& _value, float _newValor)
	{
		if (_value == _newValor)
			return;

		_value = _newValor;
	}

	void IPersistence::setAttributeValueDate(std::string& _value, std::string&_newValor)
	{
		if (_value == _newValor)
			return;

		_value = _newValor;
	}

	void IPersistence::setAttributeValueChar(char& _value, char _newValor)
	{
		if (_value == _newValor)
			return;

		_value = _newValor;
	}

	void IPersistence::setAttributeValueEnum(int& _value, int _newValor)
	{
		if (_value == _newValor)
			return;

		_value = _newValor;
	}

	void IPersistence::setAttributeValueObject(IPersistence*& _value, IPersistence* _newValor)
	{
		if (_value == _newValor)
			return;

		_value = _newValor;
	}

	void IPersistence::addAttributeList(IPersistenceList* _value, IPersistenceList* _newValue)
	{
		_value->push_back(_newValue);
	}

	void IPersistence::addObjectPropertyList(IPersistenceList* _value, IPersistence* _object)
	{
		_value->push_back(_object);
	}

	bool IPersistence::addObjectPropertyList(std::string _nameAttribute, IPersistence* _newObject)
	{
		auto attributeValue = getValue(_nameAttribute);
		if (attributeValue != nullptr && attributeValue->getType() == _list_1_N)
		{
			(*attributeValue->getAddress().ppList)->push_back(_newObject);
			return true;
		}
		return false;
	}

	bool IPersistence::removeObjectPropertyList(std::string _nameAttribute, IPersistence* _object)
	{
		auto attributeValue = getValue(_nameAttribute);
		if (attributeValue != nullptr && attributeValue->getType() == _list_1_N)
		{
			(*attributeValue->getAddress().ppList)->remove(_object);
			return true;
		}
		return false;
	}
}