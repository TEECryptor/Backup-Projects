//--------------------------------------------------
//	FileName:ADODatabase.cpp
//	Introduce:The .cpp file of CADODatabase class.
//	Creator:Youfang Yao
//	Date:04/05/06
//	Version:1.0V
//--------------------------------------------------
#include "stdafx.h"
#include <comutil.h>
#include "ADODatabase.h"
//--------------------------------------------------
//	Name:CADODatabase
//	Introduce:Construction
//--------------------------------------------------
CADODatabase::CADODatabase()
 :m_pConnection(NULL)
{
}
//--------------------------------------------------
//	Name:~CADODatabase
//	Introduce:Distruction
//--------------------------------------------------
CADODatabase::~CADODatabase()
{
	if(m_pConnection)
	{
		if(m_pConnection->State)
			m_pConnection->Close();
		m_pConnection.Release();
		m_pConnection = NULL;
	}
}
//--------------------------------------------------
//	Name:Open
//	Introduce:Connection the database and open
//	Param:pszConnectString:i:The connection string
//	Return:Return S_OK if successful, otherwise return an error code
//--------------------------------------------------
HRESULT CADODatabase::Open(LPCTSTR lpszConnectString)
{
	::CoInitialize(NULL);

	HRESULT hr = S_OK;
	
	try
	{
		if(m_pConnection)
		{
			if(m_pConnection->State)
				m_pConnection->Close();
			m_pConnection.Release();
		}

		hr = m_pConnection.CreateInstance("ADODB.Connection");
		if(FAILED(hr))
			return hr;

		hr = m_pConnection->Open(lpszConnectString, "","",adModeUnknown);
		if(FAILED(hr))
			return hr;
	}
	catch(_com_error e)
	{
		return e.Error();
	}

	return S_OK;
}
//--------------------------------------------------
//	Name:GetConnectPointer
//	Introduce:Return current the connection pointer
//	Param:N/A
//	Return:The connection interface pointer
//--------------------------------------------------
_ConnectionPtr CADODatabase::GetConnectPointer()
{
	return m_pConnection;
}
//--------------------------------------------------
//	Name:GetTableCnt
//	Introduce:Get tables count
//	Param:N/A
//	Return:The all tables count
//--------------------------------------------------
LONG CADODatabase::GetTableCnt()
{
	long lTableCnt = 0;
	char szTableName[128] = {0};
	if(m_pConnection)
	{
		_RecordsetPtr pTableNameRS = NULL;
		pTableNameRS.CreateInstance(__uuidof(Recordset));
		pTableNameRS = m_pConnection->OpenSchema(adSchemaTables);

		while(VARIANT_FALSE == pTableNameRS->GetadoEOF())
		{
			_bstr_t vrVal = pTableNameRS->GetCollect("TABLE_TYPE"); 
            char* pTableType = _com_util::ConvertBSTRToString(vrVal);
			if(strcmp(pTableType, "TABLE") == 0)
			{
				_bstr_t vrTableName = pTableNameRS->GetCollect("TABLE_NAME"); 
				char* pszTmp = _com_util::ConvertBSTRToString(vrTableName);
				strcpy(szTableName, pszTmp);

				lTableCnt++;
			}
			pTableNameRS->MoveNext();
		}
		pTableNameRS->Close();
	}

	return lTableCnt;
}
//--------------------------------------------------
//	Name:GetTableName
//	Introduce:Get table name by special index
//	Param:lID:i:The id special to get table name
//		  lpszTableName:o:The table name buffer pointer
//	Return:Return true if success, otherwise return false
//--------------------------------------------------
HRESULT CADODatabase::GetTableName(LONG lID, LPTSTR lpszTableName)
{
	HRESULT hr = S_OK;

	if((NULL == m_pConnection) || (NULL == lpszTableName)) 
		return E_POINTER;

	try
	{
		_RecordsetPtr pTableNameRS = NULL;
		hr = pTableNameRS.CreateInstance(__uuidof(Recordset));
		if(FAILED(hr))
			return hr;

		pTableNameRS = m_pConnection->OpenSchema(adSchemaTables);

		long lCurTableCnt = 0;
		while(VARIANT_FALSE == pTableNameRS->GetadoEOF())
		{
			_bstr_t vrVal = pTableNameRS->GetCollect("TABLE_TYPE"); 
			LPTSTR lpTableType = _com_util::ConvertBSTRToString(vrVal);
			if(lstrcmp(lpTableType, _T("TABLE")) == 0)
			{
				if(lCurTableCnt == lID)
				{
					_bstr_t vrTableName = pTableNameRS->GetCollect("TABLE_NAME"); 
					LPTSTR lpszTmp = _com_util::ConvertBSTRToString(vrTableName);
					strcpy(lpszTableName, lpszTmp);
					delete []lpszTmp;

					delete []lpTableType;
					pTableNameRS->Close();
					return S_OK;
				}

				lCurTableCnt++;
			}
			delete []lpTableType;
			pTableNameRS->MoveNext();
		}
		pTableNameRS->Close();
	}
	catch(_com_error e)
	{
		return e.Error();
	}

	return S_OK;
}
//--------------------------------------------------
//	Name:Close
//	Introduce:To close the datebase connection
//	Param:N/A
//	Return:Return S_OK if successful, otherwise return an error code
//--------------------------------------------------
HRESULT CADODatabase::Close()
{
	HRESULT hr = S_OK;

	if(m_pConnection)
	{
		if(m_pConnection->State)
		{
			hr = m_pConnection->Close();
			if(FAILED(hr))
				return hr;
		}
	}

	return S_OK;
}
//--------------------------------------------------
