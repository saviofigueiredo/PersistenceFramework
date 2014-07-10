#ifndef DELETERULE_H
#define DELETERULE_H

#include <string>
#include <list>

namespace PersistenceFramework
{
	class DeleteRule
	{
	public:
		enum Rule
		{
			undefined = 0,
			canDelete = 1,
			canNotDelete = 2
		};

	private:
		std::string nameOfClassContainsAttribute;
		std::string attributeName;
		Rule rule;

	public:
		DeleteRule(std::string _className, std::string _attributeName, Rule _rule)
			: nameOfClassContainsAttribute(_className), attributeName(attributeName), rule(_rule)
		{
		}

		virtual ~DeleteRule() {}

		void atribuirNomeClasseContemAtributo(std::string _nameOfClassContainsAttribute)
		{
			nameOfClassContainsAttribute = _nameOfClassContainsAttribute;
		}

		std::string getNameOfClassContainsAttribute()
		{
			return nameOfClassContainsAttribute;
		}

		void setAttributeName(std::string _attributeName)
		{
			attributeName = _attributeName;
		}

		std::string getAttributeName()
		{
			return attributeName;
		}

		Rule getRule()
		{
			return rule;
		}

		void setRule(Rule _rule)
		{
			rule = _rule;
		}
	};

	class DeleteRuleList : public std::list<DeleteRule*>
	{
	public:
		bool canDelete(std::string _attributeName)
		{
			for (auto it = this->begin(); it != this->end(); ++it)
			{
				auto currentDeleteRule = (*it);

				if (currentDeleteRule->getAttributeName().compare(_attributeName) == 0)
				{
					if (currentDeleteRule->getRule() == DeleteRule::Rule::canDelete)
						return true;
					else
						return false;
				}
			}
			return false;
		}
	};
}

#endif