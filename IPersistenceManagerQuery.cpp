#include "IPersistenceManagerQuery.h"
#include "Database.h"
#include <sstream>

namespace PersistenceFramework
{
	std::string IPersistenceManagerQuery::getQueryDelete(const std::string& _nomeTabela,
		const std::string& _nomeColuna,
		const std::string& _valorColuna)
	{
		if (_nomeTabela == "" || _nomeColuna == "" || _valorColuna == "")
			return "";

		std::string query("DELETE FROM ");
		query.append(_nomeTabela);
		query.append(" WHERE ");
		query.append(_nomeColuna);
		query.append(" = ");
		query.append(_valorColuna);
		query.append(";");

		return query;
	}

	std::string IPersistenceManagerQuery::getQuerySelect(const std::string& _nomeTabela,
		const std::list<std::string>& _nomesColunas)
	{
		if (_nomeTabela == "")
		{
			return "";
		}
		if (_nomesColunas.size() == 0)
		{
			return "";
		}

		std::string sql("SELECT ");

		for (std::list<std::string>::const_iterator it = _nomesColunas.begin(); it != _nomesColunas.end(); it++)
		{
			if (it != _nomesColunas.begin())
				sql.append(", ");

			sql.append((*it));
		}

		sql.append(" FROM ");
		sql.append(_nomeTabela);
		sql.append(";");

		return sql;
	}

	std::string IPersistenceManagerQuery::getQueryListReferencesObject(const std::string& _nomeTabela,
		const std::list<std::string>& _nomesColunas,
		const std::string& _nomeColunaReferenciaObjeto,
		const std::string& _valorColunaReferenciada)
	{
		if (_nomeTabela == "" || _nomesColunas.size() == 0 || _nomeColunaReferenciaObjeto == "" || _valorColunaReferenciada == "")
			return "";

		std::string sql("SELECT ");

		for (std::list<std::string>::const_iterator it = _nomesColunas.begin(); it != _nomesColunas.end(); it++)
		{
			if (it != _nomesColunas.begin())
				sql.append(", ");

			sql.append((*it));
		}

		sql.append(" FROM ");
		sql.append(_nomeTabela);
		sql.append(" WHERE ");
		sql.append(_nomeTabela);
		sql.append(".");
		sql.append(_nomeColunaReferenciaObjeto);
		sql.append(" = ");
		sql.append(_valorColunaReferenciada);
		sql.append(";");

		return sql;
	}

	std::string IPersistenceManagerQuery::getQueryObject(const std::string& _nomeTabelaObjeto,
		const std::list<std::string>& _nomesColunasObjeto,
		const std::string& _nomeTabelaObjetoReferenciado,
		const std::string& _nomeColunaObjetoReferenciado,
		const std::string& _valorColunaObjetoReferenciado)
	{

		if (_nomeTabelaObjeto == "")
		{
			return "";
		}
		if (_nomesColunasObjeto.size() == 0)
		{
			return "";
		}
		if (_nomeTabelaObjetoReferenciado == "")
		{
			return "";
		}
		if (_nomeColunaObjetoReferenciado == "")
		{
			return "";
		}
		if (_valorColunaObjetoReferenciado == "")
		{
			return "";
		}

		std::string query = this->getQuerySelect(_nomeTabelaObjeto, _nomesColunasObjeto);
		query.replace(query.length() - 1, 1, " ");
		query.append(" WHERE ");
		query.append(_nomeTabelaObjetoReferenciado);
		query.append(".");
		query.append(_nomeColunaObjetoReferenciado);
		query.append(" = ");
		query.append(_valorColunaObjetoReferenciado);
		query.append(";");

		return query;
	}


	std::string IPersistenceManagerQuery::getQueryUpdateAttributeObject(const std::string& _nomeTabelaAtualizar,
		const std::string& _nomeColunaAtualizar,
		const std::string& _valorColunaAtualizar,
		const std::string& _nomeColunaChavePrimaria,
		const std::string& _valorColunaChavePrimaria)
	{
		if (_nomeTabelaAtualizar == "")
		{
			return "";
		}
		if (_nomeColunaAtualizar == "")
		{
			return "";
		}
		if (_valorColunaAtualizar == "")
		{
			return "";
		}
		if (_nomeColunaChavePrimaria == "")
		{
			return "";
		}
		if (_valorColunaChavePrimaria == "")
		{
			return "";
		}

		std::string sql("UPDATE ");
		sql.append(_nomeTabelaAtualizar);
		sql.append(" SET ");
		sql.append(_nomeColunaAtualizar);
		sql.append(" =");
		sql.append(_valorColunaAtualizar);
		sql.append(" WHERE ");
		sql.append(_nomeColunaChavePrimaria);
		sql.append(" =");
		sql.append(_valorColunaChavePrimaria);
		sql.append(";");
		return sql;
	}

	std::string IPersistenceManagerQuery::getQueryList_1_N(const std::string& _nomeTabela,
		const std::list<std::string>& _nomesColunas,
		const std::string& _colunaReferenciada,
		const std::string& _valorColunaReferenciada)
	{
		if (_nomeTabela == "")
		{
			return "";
		}
		if (_nomesColunas.size() == 0)
		{
			return "";
		}
		if (_colunaReferenciada == "")
		{
			return "";
		}
		if (_valorColunaReferenciada == "")
		{
			return "";
		}

		std::string query = this->getQuerySelect(_nomeTabela, _nomesColunas);
		query.replace(query.length() - 1, 1, " ");
		query.append(" WHERE ");
		query.append(_nomeTabela);
		query.append(".");
		query.append(_colunaReferenciada);
		query.append(" = ");
		query.append(_valorColunaReferenciada);
		query.append(";");
		return query;
	}

	std::string IPersistenceManagerQuery::getQueryObjectColumnValue(const std::string& _nomeColuna,
		const std::string& _nomeTabela,
		const std::string& _nomeColunaChavePrimaria,
		const std::string& _valorColunaChavePrimaria)
	{
		std::string query("SELECT ");
		query.append(_nomeColuna);
		query.append(" FROM ");
		query.append(_nomeTabela);
		query.append(" WHERE ");
		query.append(_nomeColunaChavePrimaria);
		query.append(" = ");
		query.append(_valorColunaChavePrimaria);
		query.append(";");
		return query;
	}

	std::string IPersistenceManagerQuery::getQueryPrimaryKeyValue(const std::string& _nomeTabela,
		const std::list<std::string>& _nomesColunas,
		const std::string& _colunaChavePrimaria,
		const std::string& _valorChavePrimaria)
	{
		std::string query = this->getQuerySelect(_nomeTabela, _nomesColunas);

		if (query.size() == 0)
			return "";

		query.replace(query.length() - 1, 1, " ");
		query.append(" WHERE ");
		query.append(_colunaChavePrimaria);
		query.append(" = ");
		query.append(_valorChavePrimaria);
		query.append(";");
		return query;
	}

	std::string IPersistenceManagerQuery::getQueryPrimitiveAttribute(const std::string &_nomeTabelaObjeto,
		const std::string &_nomeColunaChavePrimaria,
		const std::string &_valorChavePrimaria,
		const std::string &_nomeColunaRecuperar)
	{
		if (_nomeTabelaObjeto == "")
		{
			return "";
		}

		if (_nomeColunaChavePrimaria == "")
		{
			return "";
		}

		if (_valorChavePrimaria == "")
		{
			return "";
		}
		if (_nomeColunaRecuperar == "")
		{
			return "";
		}

		std::string query;
		query.append("SELECT ");
		query.append(_nomeColunaRecuperar);
		query.append(" FROM ");
		query.append(_nomeTabelaObjeto);
		query.append(" WHERE ");
		query.append(_nomeColunaChavePrimaria);
		query.append(" = ");
		query.append(_valorChavePrimaria);
		query.append(";");
		return query;
	}


	std::string IPersistenceManagerQuery::getQueryUpdateParentList(const std::string& _nomeTabelaLista,
		const std::string& _colunaLista,
		const std::string& _valorColunaObjeto,
		const std::string& _colunaChavePrimariaLista,
		const std::list<std::string>& _chavesPrimariasObjetosFilhos)
	{
		if (_nomeTabelaLista == "")
		{
			return "";
		}

		if (_colunaLista == "")
		{
			return "";
		}

		if (_valorColunaObjeto == "")
		{
			return "";
		}
		if (_chavesPrimariasObjetosFilhos.size() == 0)
		{
			return "";
		}

		std::string query;
		query.append("UPDATE ");
		query.append(_nomeTabelaLista);
		query.append(" SET ");
		query.append(_colunaLista);
		query.append(" = ");
		query.append(_valorColunaObjeto);
		query.append(" WHERE ");
		query.append(_colunaChavePrimariaLista);
		query.append(" IN (");

		for (std::list<std::string>::const_iterator it = _chavesPrimariasObjetosFilhos.begin(); it != _chavesPrimariasObjetosFilhos.end(); it++)
		{
			if (it != _chavesPrimariasObjetosFilhos.begin())
				query.append(", ");

			query.append((*it));
		}
		query.append(");");
		return query;
	}
}