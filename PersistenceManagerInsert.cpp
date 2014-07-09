#include "PersistenceManagerInsert.h"
#include "IPersistenceManagerQuery.h"
#include "IPersistenceManagerUpdate.h"
#include "IPersistenceManagerConnection.h"
#include "IPersistenceList.h"
#include "AttributeValueList.h"
#include "Table.h"

namespace PersistenceFramework
{
	PersistenceManagerInsert::PersistenceManagerInsert(IPersistenceManagerQuery* _managerQuery, IPersistenceManagerConnection* _managerConnection)
	{
		managerQuery = _managerQuery;
		managerConnection = _managerConnection;
	}
	
	void PersistenceManagerInsert::setManagerUpdate(IPersistenceManagerUpdate* _managerUpdate)
	{
		managerUpdate = _managerUpdate;
	}

	bool PersistenceManagerInsert::insert(IPersistence* _object)
	{
		if (!_object)
		{
			return false;
		}

		if (!managerQuery)
		{
			return false;
		}

		if (!managerConnection)
		{
			return false;
		}


		std::string sql = managerQuery->getQueryInsert(_object);
		Table tbl;
		if (managerConnection->runQuery(sql, tbl) == false)
			return false;

		//Get the id of the element that has been just inserted
		auto chavePrimaria = _object->getPrimaryKeyAttributeName();
		auto value = _object->getValue(chavePrimaria);

		if (value == NULL)
		{
			return false;
		}

		std::string nomeColuna = _object->getColumnName(chavePrimaria);
		std::string queryMaiorValor = managerQuery->getQueryBiggestValue(_object->getTableName(), nomeColuna, "maximo");

		managerConnection->runQuery(queryMaiorValor, tbl);

		if (tbl.ISEOF() == false)
			tbl.MoveFirst();
		else
		{
			return false;
		}

		unsigned long long valorMaximo;

		if (tbl.Get("maximo", valorMaximo))
			*value->getAddress().pLong = valorMaximo;
		else
		{
			return false;
		}

		AttributeValueList values;
		_object->getValues(&values);
		for (auto it = values.begin(); it != values.end(); ++it)
		{
			AttributeValue* value = (*it);

			if (value->getType() == _list_1_N)
			{
				insert(*value->getAddress().ppList);
				managerUpdate->updateParent(_object, value, *value->getAddress().ppList);
			}

			value->setHasBeenRetrieved(true);
		}

		return true;
	}
	
	bool PersistenceManagerInsert::insert(IPersistenceList* _objects)
	{
		for (auto it = _objects->begin(); it != _objects->end(); ++it)
		{
			auto object = (*it);

			if (insert(object) == false)
				return false;
		}
		return true;
	}
}