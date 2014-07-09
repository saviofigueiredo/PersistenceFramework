#include "Table.h"
#include <stdio.h>
#include <iostream>
#include <comdef.h>
#include <conio.h>
#include <math.h>

namespace PersistenceFramework
{
	void Table::GetErrorErrStr(char* ErrStr)
	{
		sprintf(ErrStr, "%s", m_ErrStr);
	}
	Table::Table()
	{
		m_Rec = nullptr;
	}

	int Table::ISEOF()
	{
		int rs;
		if (m_Rec == nullptr)
		{
			sprintf(m_ErrStr, "Invalid Record");
			return -1;
		}
		try{
			rs = m_Rec->EndOfFile;
		}
		CATCHERROR(m_Rec, -2)

			sprintf(m_ErrStr, "Success");
		return rs;
	}

	bool Table::GetFieldValue(const char* FieldName, char* FieldValue)
	{
		try
		{
			_variant_t  vtFld;
			vtFld = m_Rec->Fields->GetItem(FieldName)->GetValue();

			switch (vtFld.vt)
			{
			case VT_BOOL:
				if (vtFld.boolVal == 0)
					sprintf(FieldValue, "%s", "false");
				else
					sprintf(FieldValue, "%s", "true");
				break;
			case VT_R4:
				sprintf(FieldValue, "%f", vtFld.fltVal);
				break;
			case VT_R8:
				sprintf(FieldValue, "%lf", vtFld.dblVal);
				break;
			case VT_BSTR:
				sprintf(FieldValue, "%s", (LPCSTR)((_bstr_t)vtFld.bstrVal));
				break;
			case VT_I2:
			case VT_UI1:
				sprintf(FieldValue, "%d", vtFld.iVal);
				break;
			case VT_INT:
				sprintf(FieldValue, "%d", vtFld.iVal);
				break;
			case VT_I4:
				sprintf(FieldValue, "%ld", vtFld.lVal);
				break;
			case VT_UI4:
				sprintf(FieldValue, "%ld", vtFld.ulVal);
				break;
			case VT_DECIMAL:
			{
				double val = vtFld.decVal.Lo32;
				val *= (vtFld.decVal.sign == 128) ? -1 : 1;
				val /= pow(static_cast<float>(10), vtFld.decVal.scale);
				sprintf(FieldValue, "%f", val);
			}
				break;
			case VT_DATE:
			{
				double time = vtFld.date;
				BSTR timestamp;
				TCHAR szFinal[255];
				const LCID LOCATION_IDENTIFICATION_BRAZIL_PORTUGUESE = 1046;
				VarBstrFromDate(time, LOCATION_IDENTIFICATION_BRAZIL_PORTUGUESE, 0, &timestamp);
#ifdef UNICODE
				wsprintf(szFinal, L"%s", timestamp);
#else
				_bstr_t bstrStart(timestamp);
				wsprintf(szFinal, "%s", (LPCTSTR)bstrStart);
#endif
				TCharToChar(szFinal, FieldValue, sizeof(szFinal));
			}
				break;
			case VT_EMPTY:
			case VT_NULL:
				strcpy(FieldValue, "\0");
				break;
			default:
				strcpy(FieldValue, "\0");
				return false;
			}

			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	bool Table::Get(const char* FieldName, char* FieldValue)
	{
		try
		{
			_variant_t  vtValue;
			vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();

			if (vtValue.vt == VT_NULL)
			{
				strcpy(FieldValue, "\0");
			}
			else
				sprintf(FieldValue, "%s", (LPCSTR)((_bstr_t)vtValue.bstrVal));
		}
		CATCHERRGET

			sprintf(m_ErrStr, "Success");
		return 1;
	}


	bool Table::Get(const char* FieldName, int& FieldValue)
	{
		try
		{
			_variant_t  vtValue;
			vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();

			if (vtValue.vt != VT_NULL)
			{
				if (vtValue.vt == VT_BOOL)
					FieldValue = vtValue.iVal;
				else if (vtValue.vt == VT_INT)
					FieldValue = vtValue.intVal;
				else
					FieldValue = vtValue.lVal;
			}
		}

		CATCHERRGET

			sprintf(m_ErrStr, "Success");
		return 1;
	}

	bool Table::Get(const char* FieldName, float& FieldValue)
	{
		try
		{
			_variant_t  vtValue;
			vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();

			if (vtValue.vt != VT_NULL && vtValue.vt != VT_EMPTY)
			{
				if (vtValue.vt == VT_R8)
					FieldValue = static_cast<float>(vtValue.dblVal);
				else if (vtValue.vt == VT_I4)
				{
					std::string nomeColuna = FieldName;

					FieldValue = static_cast<float>(vtValue.lVal);
				}
				else
					FieldValue = vtValue.fltVal;
			}
			else
				FieldValue = 0;
		}

		CATCHERRGET

			sprintf(m_ErrStr, "Success");
		return 1;
	}

	bool Table::Get(const char* FieldName, double& FieldValue)
	{
		try
		{
			_variant_t  vtValue;
			vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();
			if (vtValue.vt == VT_R8)
				FieldValue = vtValue.dblVal;
		}

		CATCHERRGET

			sprintf(m_ErrStr, "Success");
		return 1;
	}
	bool Table::Get(const char* FieldName, unsigned long long& FieldValue)
	{
		try
		{
			_variant_t  vtValue;
			vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();

			if (vtValue.vt != VT_NULL)
			{
				if (vtValue.vt == VT_UI4)
					FieldValue = vtValue.ulVal;
				else if (vtValue.vt == VT_I4)
					FieldValue = vtValue.lVal;
				else
				{
					if (vtValue.decVal.Lo64 < (std::numeric_limits<unsigned long>::max)())
						FieldValue = static_cast<unsigned long>(vtValue.decVal.Lo64);
				}
			}
		}

		CATCHERRGET

			sprintf(m_ErrStr, "Success");
		return 1;
	}


	HRESULT Table::MoveNext()
	{
		HRESULT hr;
		try
		{
			hr = m_Rec->MoveNext();
		}
		catch (_com_error &e)
		{
			ErrorHandler(e, m_ErrStr);
			//m_Rec=NULL;
			return -2;
		}
		sprintf(m_ErrStr, "Success");
		return hr;
	}

	HRESULT Table::MovePrevious()
	{
		HRESULT hr;
		try
		{
			hr = m_Rec->MovePrevious();
		}
		catch (_com_error &e)
		{
			ErrorHandler(e, m_ErrStr);
			//m_Rec=NULL;
			return -2;
		}
		sprintf(m_ErrStr, "Success");
		return hr;
	}

	HRESULT Table::MoveFirst()
	{
		HRESULT hr;
		try
		{
			hr = m_Rec->MoveFirst();
		}
		catch (_com_error &e)
		{
			ErrorHandler(e, m_ErrStr);
			//m_Rec=NULL;
			return -2;
		}
		sprintf(m_ErrStr, "Success");
		return hr;
	}

	HRESULT Table::MoveLast()
	{
		HRESULT hr;
		try
		{
			hr = m_Rec->MoveLast();
		}
		catch (_com_error &e)
		{
			ErrorHandler(e, m_ErrStr);
			//m_Rec=NULL;
			return -2;
		}
		sprintf(m_ErrStr, "Success");
		return hr;
	}

	void Table::TCharToChar(const wchar_t* Src, char* Dest, int Size)
	{
		WideCharToMultiByte(CP_ACP, 0, Src, wcslen(Src) + 1, Dest, Size, NULL, NULL);
	}

	void Table::TCharToChar(const char* Src, char* Dest, int)
	{
		strcpy(Dest, Src);
	}
}