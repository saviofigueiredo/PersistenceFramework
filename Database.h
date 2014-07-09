#ifndef DATABASE_H
#define DATABASE_H

#include <time.h>

#pragma warning( disable: 4251 )
#pragma warning( disable: 4190 )

#import "msado15.dll" \
	    rename("EOF", "EndOfFile")

typedef ADODB::_RecordsetPtr	RecPtr;
typedef ADODB::_ConnectionPtr	CnnPtr;

namespace PersistenceFramework{

	class Database;
	class Table;

	class Database
	{
	public:
		CnnPtr m_Cnn;
		char m_ErrStr[500];
		Database();
		bool Open(char* UserName, char* Pwd, const char* CnnStr);
		bool OpenTbl(int Mode, const char* CmdStr, Table& Tbl);
		bool Execute(const char* CmdStr);
		bool Execute(const char* CmdStr, Table& Tbl);
		void GetErrorErrStr(char* ErrStr);

		void ErrorHandler(_com_error &e, char* ErrStr)
		{
			sprintf(ErrStr, "Error:\n Code = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s", e.Error(), (char*)e.ErrorMessage(), (char*)e.Source(), (char*)e.Description());
		}
	};
}

#endif