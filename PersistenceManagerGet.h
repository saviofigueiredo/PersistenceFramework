#ifndef PERSISTENCEMANAGERGET_H
#define PERSISTENCEMANAGERGET_H

#include "IPersistenceManagerGet.h"
#include <string>
#include "Table.h"

namespace PersistenceFramework
{
	class IPersistence;
	class IPersistenceList;
	class AttributeValue;
	class IPersistenceManagerConnection;
	class IPersistenceManagerQuery;

	class PersistenceManagerGet : public IPersistenceManagerGet
	{
	private:
		IPersistenceManagerConnection* managerConnection;
		IPersistenceManagerQuery* managerQuery;

	public:
		PersistenceManagerGet(IPersistenceManagerConnection* _managerConnection, IPersistenceManagerQuery* _managerQuery);

		virtual bool read(IPersistence* _objeto);
		virtual bool read(IPersistenceList* _lista, IPersistence* _prototipo);
		virtual bool read(IPersistenceList* _lista, IPersistence* _prototipo, IPersistence* _atributoObjeto, std::string _nomeAtributo);
		virtual bool readAttributeObject(IPersistence* _novoObjeto, std::string _nomeAtributo, AttributeValue* _valor);
		virtual bool readAttributeList_1_N(IPersistence* _novoObjeto, std::string _nomeAtributo, AttributeValue* _valor);
		virtual std::string getObjectColumnValue(IPersistence* _objeto, std::string _coluna);
		virtual bool alreadyExists(IPersistence* _objeto);
		virtual bool readAttributePrimitive(IPersistence* _novoObjeto, std::string _nomeAtributo, AttributeValue* _valor);
		virtual bool read(IPersistence* _novoObjeto, Table _tbl, std::string _nomeAtributo = "", IPersistence* _atributoObjeto = nullptr);
		virtual bool readAttributePrimitive(IPersistence* _novoObjeto, std::string _nomeAtributo, AttributeValue* _valor, Table& _tbl);
	};
}

#endif