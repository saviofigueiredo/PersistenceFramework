#ifndef IPERSISTENCE_H
#define IPERSISTENCE_H

#include <string>
#include <unordered_map>
#include "AttributeValue.h"
#include "PersistenceMacros.h"

namespace PersistenceFramework
{
	class AttributeValueList;
	class AttributeList;

	class IPersistence
	{
	private:
		std::unordered_map<std::string, AttributeValue*>* attributeValues;
		unsigned long long* primaryKeyValue;
		std::string primaryKeyAttributeName;

	public:
		IPersistence();
		virtual ~IPersistence();

		virtual unsigned long long getId()
		{
			return 0;
		}

		inline void atribuirValorChavePrimaria(AttributeValue* _value)
		{
			primaryKeyValue = _value->getAddress().pLong;
			primaryKeyAttributeName = _value->getName();
		}

		inline std::string getPrimaryKeyAttributeName()
		{
			return primaryKeyAttributeName;
		}

		inline unsigned long long getPrimaryKeyValue()
		{
			return *primaryKeyValue;
		}

		inline AttributeValue* getValue(std::string _name)
		{
			std::unordered_map<std::string, AttributeValue*>::iterator it = this->attributeValues->find(_name);

			if (it != this->attributeValues->end())
				return it->second;

			return NULL;
		}

		virtual std::list<std::string>& getStrings()
		{
			static 	std::list<std::string> fields;
			return fields;
		}

		virtual std::string toString()
		{
			auto fields = getStrings();
			if (fields.size() > 0)
			{
				auto returnValue = fields.front();
				fields.clear();
				return returnValue;
			}
			return "";
		}

		virtual std::unordered_map<std::string, std::pair<std::string, std::string>>& addAttributeValue()
		{
			static 	std::unordered_map<std::string, std::pair<std::string, std::string>> fields;
			return fields;
		}

		virtual bool getChildItens(IPersistenceList*)
		{
			return false;
		}

		virtual bool addedChildItem()
		{
			return false;
		}

		virtual std::unordered_map<int, std::string>& change()
		{
			static std::unordered_map<int, std::string> mapping;
			return mapping;
		}

		virtual bool canDelete(std::string _attributeName)
		{
			return false;
		}

		virtual std::string getColumnListReferencesAttribute(std::string _attributeName){ return ""; }
		virtual std::string getAttributeNameReferencedByList(std::string _attributeName) { return ""; }
		virtual std::string getClassNameVirtual() { return ""; };
		virtual IPersistence* newObject() { return new IPersistence(); }
		virtual std::string getColumnName(std::string _attributeName) { return ""; };
		virtual std::string getAttributeName(std::string _columnName) { return ""; }

		bool hasBeenRetrieved();

		virtual void initializeAttributes() {};
		virtual std::string getTableName();
		static std::string getClassName();

		std::unordered_map<std::string, AttributeValue*>* getValues();
		void getValues(std::unordered_map<std::string, AttributeValue*>* _list);
		void getValues(AttributeValueList* _values);
		void setValues(std::unordered_map<std::string, AttributeValue*>* _attributeValues, bool _partial, bool _recursive);
		void setObject(IPersistence* _object, IPersistence* _equivalentObject, bool _partial, bool _recursive);

		bool isEqual(IPersistence* _object, bool _recursive);

		virtual void getColumnNames(std::list<std::string> &) {}
		virtual void getColumnNamesNotProxy(std::list<std::string> &){}
		virtual void getAttributeNames(std::list<std::string>&);
		IPersistence* setAttributeValue(std::string _attributeName, std::string _attributeValue, IPersistenceList* _objects);

		void addAttributeValue(AttributeValue* _attributeValue);
		void addAttributeValue(AttributeValue* _attributeValue, IPersistenceList* _list);
		void getAttributes(AttributeList* _dados);

		void setAttributeValueString(std::string& _value, std::string _newValor);
		void setAttributeValueInt(int& _value, int _newValor);
		void setAttributeValueFloat(float& _value, float _newValor);
		void setAttributeValueDate(std::string& _value, std::string&_newValor);
		void setAttributeValueChar(char& _value, char _newValor);
		void setAttributeValueEnum(int& _value, int _newValor);
		void setAttributeValueObject(IPersistence*& _value, IPersistence* _newValor);

		void addAttributeList(IPersistenceList* _value, IPersistenceList* _newValue);
		void addObjectPropertyList(IPersistenceList* _value, IPersistence* _object);
		bool addObjectPropertyList(std::string _nameAttribute, IPersistence* _newObject);
		bool removeObjectPropertyList(std::string _nameAttribute, IPersistence* _object);

		DECLARE_RUNTIME_CLASS
	};
}

#endif