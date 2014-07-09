#ifndef ATTRIBUTELIST_H
#define ATTRIBUTELIST_H

#include <list>

namespace PersistenceFramework
{
	class Attribute;

	class AttributeList : public std::list<Attribute*>
	{
	public:
		Attribute* get(std::string _name);
	};
}

#endif