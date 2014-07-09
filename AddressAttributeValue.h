#ifndef ADDRESSATTRIBUTEVALUE_H
#define ADDRESSATTRIBUTEVALUE_H

#include <ctime>
#include <string>

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;

	union AddressAttributeValue
	{
		int *pInt;
		char *pChar;

		float *pFloat;
		double *pDouble;

		unsigned long long* pLong;

		time_t *pDate;

		int *pEnum;

		std::string *pString;

		IPersistence **ppObject;

		IPersistenceList **ppList;

		AddressAttributeValue()
		{
			this->pInt = nullptr;
		}
	};
}

#endif