#ifndef CLASSESMANAGER_H
#define CLASSESMANAGER_H

#include <string>
#include <unordered_map>

namespace PersistenceFramework
{
	class IPersistence;
	class Attribute;
	class AttributeList;
	class DeleteRuleList;
	class DeleteRule;
	class GerenteClasses;

	class ClassesManager
	{
	private:
		std::unordered_map<std::string, IPersistence*> mappingClassNameInstance;
		ClassesManager();
		AttributeList* attributes;
		DeleteRuleList* deleteRules;
		static ClassesManager* singleInstance;

	public:
		IPersistence* newObject(std::string _className);

		void addClass(std::string _nomeClasse, IPersistence* _objeto);
		void deleteClasses();
		void deleteAttributes();
		void deleteDeleteRules();
		void addClass(IPersistence* _object);
		void addAttribute(Attribute*);
		void addDeleteRule(DeleteRule*);

		static ClassesManager* ClassesManager::get()
		{
			if (singleInstance == NULL)
				singleInstance = new ClassesManager();

			return singleInstance;
		}

		static void ClassesManager::freeMemory()
		{
			if (singleInstance != NULL)
			{
				delete singleInstance;
				singleInstance = NULL;
			}
		}
	};
}

#endif