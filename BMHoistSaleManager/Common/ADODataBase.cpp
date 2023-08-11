//----------------------------------------------------
//	File Name: ADODataBase.cpp
//	Introduce: Class CADODataBase implement
//----------------------------------------------------
#include "StdAfx.h"
#include "ADODataBase.h"
//----------------------------------------------------
CADODataBase::CADODataBase(void)
: m_pConnection(NULL)
{
	_AFX_THREAD_STATE* pState = AfxGetThreadState();
	if (!pState->m_bNeedTerm)
	{
		AfxOleInit(); 
	}
}
//----------------------------------------------------
CADODataBase::~CADODataBase(void)
{
	CloseDatabase();
	if (NULL != m_pConnection)
	{		
		m_pConnection.Release();
		m_pConnection = NULL;
	}
}
//----------------------------------------------------


//----------------------------------------------------
IUnknown* CADODataBase::GetConnectionPtr()
{
	return m_pConnection.GetInterfacePtr();
}
//----------------------------------------------------
BOOL CADODataBase::OpenDatabase(LPCWSTR lpwzDatabaseFile, 
								LPCWSTR lpwzDatabasePassword,
								BOOL bIsExcel)
{
	
	try
	{
		HRESULT hr = S_OK;
		hr = m_pConnection.CreateInstance(__uuidof(Connection)); 
		if (S_OK != hr)
			return FALSE;

		WCHAR wcConnectText[1024] = {0};
		if (TRUE == bIsExcel)
		{
			LPCWSTR pExt = lpwzDatabaseFile;
			while (*pExt != L'.' && *pExt != L'\0') pExt++;
			if (wcsicmp(pExt, L".xls") == 0)// Excel 97-2003
				wsprintf(wcConnectText, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Extended Properties = \"Excel 8.0;HDR=Yes;IMEX=0\"", lpwzDatabaseFile);
			else							// Excel 2007
				wsprintf(wcConnectText, L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s;Extended Properties = \"Excel 8.0;HDR=Yes;IMEX=0\"", lpwzDatabaseFile);
		}
		else	// Access
		{
			wsprintf(wcConnectText, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Jet OLEDB:Database Password=%s", lpwzDatabaseFile, lpwzDatabasePassword);
		}
		hr = m_pConnection->Open(wcConnectText, /*lpwzUser, lpwzPassword*/L"", L"", adModeUnknown); 
		if (S_OK != hr)
		{
			m_pConnection.Release();
			m_pConnection = NULL;
			return FALSE;
		}
	}
	catch(_com_error e)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
		return FALSE;
	}
	return TRUE;
}
//----------------------------------------------------
BOOL CADODataBase::ExcuteSQL(LPCWSTR lpwzSQL)
{
	try
	{
		HRESULT hr = S_OK;
		_variant_t vtRecordsAffected;
		hr = m_pConnection->Execute(lpwzSQL, &vtRecordsAffected, adCmdText);
		if (FAILED(hr))
		{
			return FALSE;
		}
	}
	catch(_com_error e)
	{
		return FALSE;
	}
	return TRUE;
}
//----------------------------------------------------
void CADODataBase::CloseDatabase()
{
	if (NULL != m_pConnection &&
		adStateOpen == m_pConnection->State)
	{
		m_pConnection->Close();
	}
}
//----------------------------------------------------
