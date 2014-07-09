#ifndef IPERSISTENCEMANAGERUPDATE_H
#define IPERSISTENCEMANAGERUPDATE_H

#include <string>

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;
	class AttributeValue;

	class IPersistenceManagerUpdate
	{
	public:

		virtual bool update(IPersistence* _object) = 0;
		virtual bool updateAttributesPrimitive(IPersistence* _object) = 0;
		virtual bool updateAttributeObject(IPersistence* _objectToUpdate, std::string _attributeName) = 0;
		virtual bool updateAttributesObject(IPersistence* _objectToUpdate) = 0;
		virtual bool updateAttributesList_1_N(IPersistence* _object, bool _cascade = true) = 0;
		virtual bool updateParent(IPersistence* _parentObject, AttributeValue* _value, IPersistenceList* _childObjects) = 0;
	};
}

#endif