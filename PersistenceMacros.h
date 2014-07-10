#ifndef PERSISTENCEMACROS_H
#define PERSISTENCEMACROS_H

#include <string>
#include <map>
#include <list>
#include <unordered_map>

#define DECLARE_RUNTIME_CLASS \
private: \
	static const std::string m_class_name; \
public:  \
	virtual bool IsOfType(const std::string&);


#define IMPLEMENT_RUNTIME_CLASS(class_name) \
const std::string class_name##::m_class_name = #class_name; \
bool class_name##::IsOfType(const std::string& name) \
{ \
	return (name == m_class_name); \
}

#define DECLARE_SERIAL(class_name) \
	protected: \
		static AttributeList attributes; \
		static std::unordered_map<std::string,std::string> mappingAttributeNameColumnName; \
		static std::unordered_map<std::string, std::pair<std::string, std::string>> mappingNomeAtributoNomeTabelaObjeto; \
		static DeleteRuleList deleteRules; \
	public: \
		virtual void defineDeleteRules(); \
		virtual void addDeleteRule(DeleteRule* _rule); \
		virtual bool canDelete(std::string _attributeName); \
		virtual Attribute* getAttribute(std::string _name); \
		virtual void addAttribute(Attribute* _attribute); \
		virtual void set(class_name* _object, bool partial = false, bool recursive = false); \
		virtual void setAttributes(); \
		static std::string getClassName(); \
		virtual std::string getTableName(); \
		static std::string getTableNameStatic(); \
		virtual std::string getClassNameVirtual(); \
		virtual std::string getColumnName(std::string _attributeName); \
		virtual std::string getAttributeName(std::string _columnName); \
		virtual IPersistence* createObject(); \
		virtual void getColumnNames(std::list<std::string> &_columnNames); \
		virtual void getColumnNamesNotProxy(std::list<std::string> &_columnNames); \
		virtual std::string getColumnListReferencesAttribute(std::string _attributeName); \
		virtual std::string getAttributeNameReferencedByList(std::string _attributeName);

#define IMPLEMENT_SERIAL(class_name, base_class_name, class_table_name) \
	AttributeList class_name::attributes; \
	std::unordered_map<std::string, std::string> class_name::mappingAttributeNameColumnName; \
	std::unordered_map<std::string, std::pair<std::string, std::string>> class_name::mappingNomeAtributoNomeTabelaObjeto; \
	DeleteRuleList class_name::deleteRules; \
	void class_name::addDeleteRule(DeleteRule* _rule) \
		{ \
		deleteRules.push_back(_rule); \
		} \
	bool class_name::canDelete(std::string _attributeName) \
		{ \
		return deleteRules.canDelete(_attributeName); \
		} \
	void class_name::set(class_name* _object, bool partial, bool recursive) \
		{ \
		setValues(_object->getValues(), partial, recursive); \
		} \
	std::string class_name::getClassNameVirtual() \
		{ \
		return #class_name; \
		} \
	Attribute* class_name::getAttribute(std::string _name) \
		{ \
		Attribute* attributeTemp; \
		attributeTemp = attributes.get(_name); \
		return attributeTemp; \
		} \
	void class_name::addAttribute(Attribute* _attribute) \
		{ \
		attributes.push_back(_attribute); \
		} \
	std::string class_name::getClassName() \
		{ \
		return #class_name; \
		} \
	std::string class_name::getTableName() \
		{ \
		return #class_table_name; \
		} \
	std::string class_name::getTableNameStatic() \
		{ \
		return #class_table_name; \
		} \
	std::string class_name::getColumnName(std::string _attributeName) \
		{ \
		return mappingAttributeNameColumnName[_attributeName]; \
		} \
	std::string class_name::getAttributeName(std::string _columnName) \
		{ \
		for(auto it = mappingAttributeNameColumnName.begin(); it != mappingAttributeNameColumnName.end(); ++it) \
			if( it->second  == _columnName) \
				return it->first; \
		return ""; \
		} \
	void class_name::getColumnNamesNotProxy(std::list<std::string> &_columnNames) \
		{ \
		for(auto it = mappingAttributeNameColumnName.begin(); it != mappingAttributeNameColumnName.end(); it++) \
				{ \
			auto attributeColumn = getAttribute((*it).first); \
			if(attributeColumn != NULL && attributeColumn->getIsProxy() == false) \
				_columnNames.push_back((*it).second); \
				} \
		} \
	void class_name::getColumnNames(std::list<std::string> &_columnNames) \
		{ \
		for(auto it = mappingAttributeNameColumnName.begin(); it != mappingAttributeNameColumnName.end(); it++) \
				{ \
			_columnNames.push_back((*it).second); \
				} \
		} \
	std::string class_name::getColumnListReferencesAttribute(std::string _nomeAtributo) \
		{ \
		std::pair<std::string, std::string>	p; \
		p = mappingNomeAtributoNomeTabelaObjeto[_nomeAtributo]; \
		return p.second; \
		} \
	std::string class_name::getAttributeNameReferencedByList(std::string _nomeAtributo) \
		{ \
		std::pair<std::string, std::string>	p; \
		p = mappingNomeAtributoNomeTabelaObjeto[_nomeAtributo]; \
		return p.first; \
		} 

#define IMPLEMENT_SERIAL_CONSTRUTOR \
		setAttributes(); \
		defineDeleteRules();


#define BEGIN_DEFINING_DELETE_RULES \
	static bool rulesDefined = false; \
	if(rulesDefined == false) \
		{ \
		DeleteRule* newRule = nullptr;

#define END_DEFINING_DELETE_RULES \
	rulesDefined = true; \
		}

#define BEGIN_DEFINING_MAPPING_BETWEEN_ATTRIBUTES_AND_COLUMNS \
	static bool mappingDefined = false;

#define END_DEFINING_MAPPING_BETWEEN_ATTRIBUTES_AND_COLUMNS \
	mappingDefined = true;

#define ADD_ATTRIBUTE_LONG(attributeName, _columnName, isPrimaryKey, isProxy) \
	if (mappingDefined == false) \
		{ \
		auto* attribute = new Attribute(this->getClassName(), _long, #attributeName, ##isPrimaryKey, ##isProxy); \
		ClassesManager::get()->addAttribute(attribute); \
		addAttribute(attribute); \
		mappingAttributeNameColumnName[#attributeName] = #_columnName; \
		} \
	if(##isPrimaryKey == true) \
		{ \
		auto newValue = new AttributeValue(getAttribute(#attributeName), &##attributeName); \
		addAttributeValue(newValue); \
		this->setPrimaryKeyValue(newValue); \
		} \
		else \
		{ \
		addAttributeValue(new AttributeValue(getAttribute(#attributeName), &##attributeName)); \
		}

#define ADD_ATTRIBUTE_STRING(attributeName, _columnName, isProxy) \
	if (mappingDefined == false) \
		{ \
		auto attribute = new Attribute(this->getClassName(), _string, #attributeName, false, ##isProxy); \
		ClassesManager::get()->addAttribute(attribute); \
		addAttribute(attribute); \
		mappingAttributeNameColumnName[#attributeName] = #_columnName; \
		} \
		addAttributeValue(new AttributeValue(getAttribute(#attributeName), &##attributeName));
#endif