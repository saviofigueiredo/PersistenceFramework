#ifndef IPERSISTENCELIST_H
#define IPERSISTENCELIST_H

#include <list>
#include "IPersistence.h"
#include <unordered_map>

namespace PersistenceFramework
{
	class IPersistenceList : public std::list<IPersistence*>
	{
	public:
		virtual IPersistence* newObject()
		{
			return new IPersistence();
		}
		virtual IPersistenceList* newList()
		{
			return new IPersistenceList();
		}

		virtual void deleteAllElements()
		{
			std::list<IPersistence*>::const_iterator iter = this->begin();
			std::list<IPersistence*>::const_iterator endIter = this->end();
			for (; iter != endIter; ++iter)
			{
				delete *iter;
			}
			this->clear();
		}

		IPersistenceList::iterator find(IPersistence* _object);

		bool isEqual(IPersistenceList* _list, bool _recursive);

		void push_back(IPersistenceList* _list);

		void push_back(IPersistence* _object);

		void getObjects(std::unordered_map<std::string, std::string> &_mapping, IPersistenceList * _objects);
		void getObjects(std::unordered_map<std::string, std::string> &_mapping, std::pair<std::string, std::string> &_startDate, std::pair<std::string, std::string> &_endDate, IPersistenceList * _object);

		void getObjectsIDontHave(IPersistenceList* _list, IPersistenceList* _returnList);
	};
}

#endif