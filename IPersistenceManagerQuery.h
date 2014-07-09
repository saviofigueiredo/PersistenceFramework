#ifndef IPERSISTENCEMANAGERQUERY_H
#define IPERSISTENCEMANAGERQUERY_H

#include <string>
#include <list>

namespace PersistenceFramework
{
	class IPersistenceList;
	class IPersistence;

	class IPersistenceManagerQuery
	{
	public:
		std::string getQueryDelete(const std::string& _tableName, const std::string& _columnName,	const std::string& _columnValue);

		std::string getQuerySelect(const std::string& _tableName, const std::list<std::string>& _columnsNames);

		std::string getQueryList_1_N(const std::string& _tableName, const std::list<std::string>& _columnsNames, const std::string& _referencedColumn, const std::string& _valueReferencedColumn);

		std::string getQueryObject(const std::string& _objectTableName, const std::list<std::string>& _objectColumnsNames,	const std::string& _referencedObjectTableName, const std::string& _referencedObjectColumnName,	const std::string& _referencedObjectColumnValue);

		std::string getQueryPrimitiveAttribute(const std::string &_nomeTabelaObjeto, const std::string &_primaryKeyColumnName, const std::string &_primaryKeyValue, const std::string &_columnNameToRetrieve);

		std::string getQueryObjectColumnValue(const std::string& _columnName, const std::string& _tableName, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyColumnValue);

		std::string getQueryPrimaryKeyValue(const std::string& _tableName, const std::list<std::string>& _columnsNames, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyValue);

		virtual std::string getQueryInsert(IPersistence* _object) = 0;

		virtual std::string getQueryBiggestValue(const std::string& _tableName, const std::string& _columnName, const std::string& _qualifier) = 0;

		virtual std::string getQueryUpdatePrimitiveAttribute(const std::string &_tableName, const std::list<std::pair<std::string, std::pair<std::string, int>>>& _colunasAtributosValoresAtributos, const std::string& _primaryKeyColumnName, const std::string& _primaryKeyValue) = 0;

		std::string getQueryUpdateParentList(const std::string& _listTableName,	const std::string& _columnList, const std::string& _objectColumnValue, const std::string& _listPrimaryKeyColumn, const std::list<std::string>& _primaryKeyChildObjects);
		
		std::string getQueryUpdateAttributeObject(const std::string& _tableNameToUpdate, const std::string& _columnToUpdate, const std::string& _columnValueToUpdate, const std::string& _nomeColunaChavePrimaria, const std::string& _primaryKeyColumnValue);

		std::string getQueryListReferencesObject(const std::string& _tableName, const std::list<std::string>& _columnsNames, const std::string& _objectReferenceColumnName, const std::string& _referencedColumnValue);

		bool fitToDBNumero(const std::string &value) const
		{
			int numDot = 0;
			int i;
			int len = value.length();
			for (i = 0; i < len; i++)
			{
				if (value[i] >= '0' && value[i] <= '9')
				{
					continue;
				}
				else if ((value[i] == '.'))
				{
					++numDot;
					if (numDot>1)
						return false;
				}
				else
					return false;
			}
			return true;
		}

		bool fitToDBDate(const std::string &_string) const
		{

			int yy, mm, dd;
			const char* buff = _string.c_str();
			sscanf(buff, "%d/%d/%d", &dd, &mm, &yy);

			if ((mm == 4 || mm == 6 || mm == 9 || mm == 11) && dd == 31)
			{
				return false;
			}
			else if (mm == 02 && dd >= 29)
			{
				if (dd > 29)
				{
					return false;
				}
				else
				{
					// Se o mês=2 e dia=29 então verifica se o ano não é bissexto.
					// A verificação feita abaixo segue as regras do calendário gregoriano:
					//  1. Todo ano divisível por 4 é bissexto. (ex: 1957, 2005, ...)
					//  2. Todo ano divisível por 100 não é bissexto. (ex: 1800, 1900, 2100, ...)
					//  3. Todo ano divisível por 400 é bissexto. (ex: 1600, 2000, 2400, ...)
					//  4. Prevalece as últimas regras sobre as primeiras.
					//   (2000 é divisível por 400, logo é bissexto.)
					//          (1900 é divisível por 100, logo não é bissexto.)
					if (!(yy % 4 == 0) || (yy % 100 == 0 && !(yy % 400 == 0))) // Se o ano não for bisexto...
					{
						return false;
					}
				}
			}
			else if (!(yy>1900 && yy <2050) || !(mm>0 && mm <= 12) || !(dd>0 && dd <= 31))
			{
				return false;
			}
			return true;
		}

		bool fitToDBPalavra(std::string &_string)
		{
			int len = _string.length();
			std::string _str;
			for (int i = 0; i < len; i++)
			{
				if (_string[i] == '\'')
				{
					_str = _str + "''";
					continue;
				}
				_str = _str + _string[i];

			}
			_string = _str;

			return true;
		}
	};
}

#endif