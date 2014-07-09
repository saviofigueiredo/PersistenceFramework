#include "AttributeList.h"
#include "Attribute.h"

namespace PersistenceFramework
{
	Attribute* AttributeList::get(std::string _name)
	{
		for (std::list<Attribute*>::iterator it = this->begin(); it != this->end(); ++it)
		{
			if ((*it)->getName().compare(_name) == 0)
				return (*it);
		}
		return NULL;
	}
}