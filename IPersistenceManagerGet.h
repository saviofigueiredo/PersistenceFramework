#ifndef IPERSISTENCEMANAGERGET_H
#define IPERSISTENCEMANAGERGET_H

#include <string>
#include "Table.h"

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;
	class AttributeValue;

	class IPersistenceManagerGet
	{
	public:
		virtual bool read(IPersistence* _objeto) = 0;
		virtual bool read(IPersistenceList* _lista, IPersistence* _prototipo) = 0;
		virtual bool read(IPersistenceList* _lista, IPersistence* _prototipo, IPersistence* _atributoObjeto, std::string _nomeAtributo) = 0;
		virtual bool readAttributeObject(IPersistence* _novoObjeto, std::string _nomeAtributo, AttributeValue* _valor) = 0;
		virtual bool readAttributeList_1_N(IPersistence* _novoObjeto, std::string _nomeAtributo, AttributeValue* _valor) = 0;
		virtual std::string getObjectColumnValue(IPersistence* _objeto, std::string _coluna) = 0;
		virtual bool alreadyExists(IPersistence* _objeto) = 0;
		virtual bool readAttributePrimitive(IPersistence* _novoObjeto, std::string _nomeAtributo, AttributeValue* _valor) = 0;
		virtual bool read(IPersistence* _novoObjeto, Table _tbl, std::string _nomeAtributo = "", IPersistence* _atributoObjeto = nullptr) = 0;
		virtual bool readAttributePrimitive(IPersistence* _novoObjeto, std::string _nomeAtributo, AttributeValue* _valor, Table& _tbl) = 0;
	};
}

#endif