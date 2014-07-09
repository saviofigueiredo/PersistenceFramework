#include <stdio.h>
#include <iostream>
#include <comdef.h>
#include <conio.h>
#include <math.h>
#include "Database.h"
#include "Table.h"

ADODB::_RecordsetPtr rec1 = NULL;

_variant_t  vtMissing1(DISP_E_PARAMNOTFOUND, VT_ERROR);

namespace PersistenceFramework{

	Database::Database()
	{
		m_Cnn = NULL;
		sprintf(m_ErrStr, "NULL POINTER");
	}

	void Database::GetErrorErrStr(char* ErrStr)
	{
		sprintf(ErrStr, "%s", m_ErrStr);
	}

	bool Database::Open(char* UserName, char* Pwd, const char* CnnStr)
	{
		//cnn->Open(strCnn,"sa","sa",NULL);
		try
		{
			HRESULT hr;
			hr = m_Cnn.CreateInstance(__uuidof(ADODB::Connection));
			m_Cnn->Open(CnnStr, UserName, Pwd, NULL);
		}

		CATCHERROR(m_Cnn, 0)

			sprintf(m_ErrStr, "Success");
		return 1;
	}

	bool Database::OpenTbl(int Mode, const char* CmdStr, Table &Tbl)
	{
		if (m_Cnn == NULL)
		{
			Tbl.m_Rec = NULL;
			sprintf(m_ErrStr, "Invalid Connection");
			return 0;
		}
		RecPtr t_Rec = NULL;
		try
		{
			//t_Rec->putref_ActiveConnection(m_Cnn);
			//vtMissing<<-->>_variant_t((IDispatch *) m_Cnn, true)
			t_Rec.CreateInstance(__uuidof(ADODB::Recordset));
			t_Rec->Open(CmdStr, _variant_t((IDispatch *)m_Cnn, true), ADODB::adOpenStatic, ADODB::adLockOptimistic, Mode);
		}

		CATCHERROR(Tbl.m_Rec, 0)

			Tbl.m_Rec = t_Rec;
		sprintf(m_ErrStr, "Success");

		return 1;
	}

	bool Database::Execute(const char* CmdStr)
	{
		try
		{
			m_Cnn->Execute(CmdStr, NULL, 1);
		}
		catch (_com_error &e)
		{
			ErrorHandler(e, m_ErrStr);
			return 0;
		}
		sprintf(m_ErrStr, "Success");
		return 1;
	}

	bool Database::Execute(const char* CmdStr, Table& Tbl)
	{
		RecPtr t_Rec = NULL;
		try
		{
			t_Rec = m_Cnn->Execute(CmdStr, NULL, 1);
		}

		CATCHERROR(Tbl.m_Rec, 0)

			sprintf(m_ErrStr, "Success");
		Tbl.m_Rec = t_Rec;
		sprintf(m_ErrStr, "Success");
		return 1;
	}
}
