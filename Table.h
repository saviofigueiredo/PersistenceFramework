#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#import "msado15.dll" \
	    rename("EOF", "EndOfFile")

#pragma warning( disable: 4251 )
#pragma warning( disable: 4190 )

#define CATCHERROR(ptr,a)	catch(_com_error &e)\
																												{\
								ErrorHandler(e,m_ErrStr);\
								ptr=NULL;\
								return a;\
																												}

#define CATCHERRGET			catch(_com_error &e)\
																												{\
								ErrorHandler(e,m_ErrStr);\
								sprintf(m_ErrStr,"%s\n**For Field Name:%s",m_ErrStr,FieldName);\
								return 0;\
																												}
typedef ADODB::_RecordsetPtr	RecPtr;
typedef ADODB::_ConnectionPtr	CnnPtr;

namespace PersistenceFramework
{
	class Table
	{
	public:
		RecPtr m_Rec;
		char m_ErrStr[500];
		Table();
		void GetErrorErrStr(char* ErrStr);
		int ISEOF();
		HRESULT MoveNext();
		HRESULT MovePrevious();
		HRESULT MoveFirst();
		HRESULT MoveLast();
		int AddNew();
		int Update();
		int Add(char* FieldName, char* FieldValue);
		int Add(char* FieldName, int FieldValue);
		int Add(char* FieldName, float FieldValue);
		int Add(char* FieldName, double FieldValue);
		int Add(char* FieldName, long FieldValue);
		bool Get(const char* FieldName, char* FieldValue);
		bool Get(const char* FieldName, int& FieldValue);
		bool Get(const char* FieldName, float& FieldValue);
		bool Get(const char* FieldName, double& FieldValue);
		bool Get(const char* FieldName, double& FieldValue, int Scale);
		bool Get(const char* FieldName, long& FieldValue);
		bool Get(const char* FieldName, unsigned long long& fieldValue);
		bool GetFieldValue(const char* FieldName, char* fieldValue);

		void TCharToChar(const wchar_t* Src, char* Dest, int Size);
		void TCharToChar(const char* Src, char* Dest, int);

		void ErrorHandler(_com_error &e, char* ErrStr)
		{
			sprintf(ErrStr, "Error:\n Code = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s", e.Error(), (char*)e.ErrorMessage(), (char*)e.Source(), (char*)e.Description());
		}
	};
}

#endif