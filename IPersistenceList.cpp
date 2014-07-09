#include "IPersistenceList.h"
#include <list>
#include "DateTimeHelper.h"

namespace PersistenceFramework
{
	IPersistenceList::iterator IPersistenceList::find(IPersistence* _object)
	{
		for (auto it = this->begin(); it != this->end(); ++it)
		{
			if ((*it) == _object)
				return it;
		}
		return this->end();
	}

	bool IPersistenceList::isEqual(IPersistenceList* _list, bool _recursive)
	{
		if (this == nullptr || _list == nullptr)
			return false;
		if (this->size() != _list->size())
			return false;

		auto itCompared= _list->begin();

		for (auto it = this->begin(); it != this->end() && itCompared != _list->end(); it++, itCompared++)
		{
			if ((*it)->isEqual(*itCompared, _recursive) == false)
				return false;
		}
		return true;
	}

	void IPersistenceList::push_back(IPersistenceList* _list)
	{
		for (auto it = _list->begin(); it != _list->end(); ++it)
		{
			this->push_back((IPersistence*)(*it));
		}
	}

	void IPersistenceList::push_back(IPersistence* _object)
	{
		std::list<IPersistence*>::push_back(_object);
	}

	void IPersistenceList::getObjects(std::unordered_map<std::string, std::string> &_mapping, IPersistenceList * _objects)
	{
		if (_mapping.size() == 0)
			_objects->push_back(this);
		else
		{
			for (auto it = this->begin(); it != this->end(); ++it)
			{
				auto currentObject = (*it);

				bool allValuesAreEqual = true;

				for (auto itMapping = _mapping.begin(); itMapping != _mapping.end(); ++itMapping)
				{
					std::string attributeName = (*itMapping).first;
					std::string attributeValue = (*itMapping).second;

					auto value = currentObject->getValue(attributeName);

					if (value != NULL && value->isPrimitive())
					{
						if (value == NULL || value->getValue().compare(attributeValue) != 0)
						{
							allValuesAreEqual = false;
							break;
						}
					}
					else if (value != NULL && value->getType() == _object)
					{
						std::string attributeObjectValue;

						if ((*value->getAddress().ppObject) != NULL)
						{
							auto objectAttribute = *value->getAddress().ppObject;
							std::list<std::string> campos = objectAttribute->getStrings();
							if (campos.size() > 0)
								attributeObjectValue = *campos.begin();
							campos.clear();
						}

						if (attributeObjectValue.compare(attributeValue) != 0)
						{
							allValuesAreEqual = false;
							break;
						}
					}
				}

				if (allValuesAreEqual == true)
					_objects->push_back((IPersistence*)currentObject);
			}
		}
	}

	void IPersistenceList::getObjects(std::unordered_map<std::string, std::string> &_mapping, std::pair<std::string, std::string> &_startDate, std::pair<std::string, std::string> &_endDate, IPersistenceList * _objects)
	{
		if (_objects == NULL)
			return;

		this->getObjects(_mapping, _objects);

		if (_startDate.first != "" && _startDate.second != "" && _endDate.first != "" && _endDate.second != "")
		{
			IPersistenceList returnedObjects;

			for (auto it = _objects->begin(); it != _objects->end(); ++it)
			{
				auto currentObject = (*it);

				auto startDateValue = currentObject->getValue(_startDate.first);
				auto endDateValue = currentObject->getValue(_endDate.first);

				if (startDateValue != __nullptr && endDateValue != __nullptr)
				{
					time_t myStartDate = DateTimeHelper::ConvertDateTime(startDateValue->getValue().c_str());
					time_t myEndDate = DateTimeHelper::ConvertDateTime(endDateValue->getValue().c_str());

					time_t startDateParameter = DateTimeHelper::ConvertDateTime(_startDate.second.c_str());
					time_t endDateParameter = DateTimeHelper::ConvertDateTime(_endDate.second.c_str());

					if (myStartDate > startDateParameter && myEndDate < endDateParameter)
						returnedObjects.push_back(currentObject);
				}
			}

			_objects->clear();
			std::copy(returnedObjects.begin(), returnedObjects.end(), std::back_inserter(*_objects));
		}
	}

	void IPersistenceList::getObjectsIDontHave(IPersistenceList* _list, IPersistenceList* _returnList)
	{
		if (_list == NULL || _returnList == NULL)
			return;

		IPersistenceList objectsToDelete;
		for (auto it = _list->begin(); it != _list->end(); ++it)
		{
			bool exists = false;

			auto obj = (*it);

			for (auto it2 = begin(); it2 != end(); ++it2)
			{
				auto newObject = (*it2);

				if (newObject->getClassName() == obj->getClassName())
				{
					std::string primaryKeyAttributeName = obj->getPrimaryKeyAttributeName();

					auto value1 = obj->getValue(primaryKeyAttributeName);
					auto value2 = newObject->getValue(primaryKeyAttributeName);

					if (value1->getValue() == value2->getValue())
					{
						exists = true;
						break;
					}
				}
			}
			if (exists == false)
				_returnList->push_back(obj);
		}
	}
}