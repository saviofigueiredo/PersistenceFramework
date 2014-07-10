#include "ClassesManager.h"
#include "IPersistence.h"
#include "Attribute.h"
#include "DeleteRule.h"

namespace PersistenceFramework
{
	ClassesManager* ClassesManager::singleInstance = nullptr;

	ClassesManager::ClassesManager()
	{
		attributes = new AttributeList();
		deleteRules = new DeleteRuleList();
	}

	void ClassesManager::addAttribute(Attribute* _attribute)
	{
		attributes->push_back(_attribute);
	}

	void ClassesManager::addDeleteRule(DeleteRule* _rule)
	{
		deleteRules->push_back(_rule);
	}

	IPersistence* ClassesManager::newObject(std::string _className)
	{
		if (mappingClassNameInstance.find(_className) == mappingClassNameInstance.end())
			return nullptr;

		auto objectTemp = mappingClassNameInstance[_className];

		if (objectTemp != nullptr)
			return objectTemp->newObject();
		else
			return nullptr;
	}

	void ClassesManager::addClass(std::string _className, IPersistence* _object)
	{
		mappingClassNameInstance[_className] = _object;
	}

	void ClassesManager::addClass(IPersistence* _object)
	{
		//This code is executed to make sure that the primary key has been created
		_object->getPrimaryKeyValue();

		this->addClass(_object->getClassNameVirtual(), _object);
	}

	void ClassesManager::deleteClasses()
	{
		for (auto it = mappingClassNameInstance.begin(); it != mappingClassNameInstance.end(); ++it)
		{
			if ((*it).second != NULL)
			{
				delete (*it).second;
				(*it).second = NULL;
			}
		}
		mappingClassNameInstance.clear();

		deleteAttributes();
		deleteDeleteRules();
	}

	void ClassesManager::deleteAttributes()
	{
		auto iter = attributes->begin();
		auto endIter = attributes->end();
		for (; iter != endIter; ++iter)
		{
			delete *iter;
		}

		attributes->clear();
		delete attributes;
		attributes = nullptr;
	}
	void ClassesManager::deleteDeleteRules()
	{
		auto iter = deleteRules->begin();
		auto endIter = deleteRules->end();
		for (; iter != endIter; ++iter)
		{
			delete *iter;
		}

		deleteRules->clear();
		delete deleteRules;
		deleteRules = nullptr;
	}
}