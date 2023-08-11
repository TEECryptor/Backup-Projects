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
}
//----------------------------------------------------
CADODataBase::~CADODataBase(void)
{
	CloseDatabase();
	m_pConnection = NULL;
}
//----------------------------------------------------


//----------------------------------------------------
IUnknown* CADODataBase::GetConnectionPtr()
{
	return m_pConnection.GetInterfacePtr();
}
//----------------------------------------------------
BOOL CADODataBase::OpenDatabase(LPCWSTR lpwzDatabaseFile, 
								LPCWSTR lpwzUser, 
								LPCWSTR lpwzPassword)
{
	AfxOleInit(); 
	m_pConnection.CreateInstance(__uuidof(Connection)); 
	
	try
	{
		HRESULT hr = S_OK;
		WCHAR wcConnectText[1024] = {0};
		wsprintf(wcConnectText, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", lpwzDatabaseFile);
		hr = m_pConnection->Open(wcConnectText, lpwzUser, lpwzPassword, adModeUnknown); 
		if (S_OK != hr)
			return FALSE;
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
