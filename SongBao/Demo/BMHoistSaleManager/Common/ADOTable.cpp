//----------------------------------------------------
//	File Name: ADOTable.cpp
//	Introduce: Class CADOTable implement
//----------------------------------------------------
#include "StdAfx.h"
#include "ADOTable.h"
//----------------------------------------------------
CADOTable::CADOTable(void)
: m_pRecordset(NULL)
{
}
//----------------------------------------------------
CADOTable::~CADOTable(void)
{
	CloseTable();
	m_pRecordset = NULL;
}
//----------------------------------------------------


//----------------------------------------------------
BOOL CADOTable::OpenTable(IUnknown* pDatabaseConnect,
						  LPCWSTR lpwzOpenFilter)
{
	if (!pDatabaseConnect ||
		NULL == lpwzOpenFilter ||
		wcslen(lpwzOpenFilter) == 0)
		return FALSE;

	HRESULT hr = S_OK;
	m_pRecordset.CreateInstance(__uuidof(Recordset));	

	try
	{
		hr = m_pRecordset->Open(lpwzOpenFilter, pDatabaseConnect, adOpenStatic, adLockOptimistic, adCmdText);
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
void CADOTable::CloseTable()
{
	if (NULL != m_pRecordset && 
		adStateOpen == m_pRecordset->State)
	{
		try
		{
			m_pRecordset->Close();
		}
		catch(_com_error e)
		{
		}
	}
}
//----------------------------------------------------
DWORD CADOTable::GetRecordCnt()
{
	HRESULT hr = S_OK;
	LONG lRecordCount = 0;
	try
	{
		hr = m_pRecordset->get_RecordCount(&lRecordCount);
	}
	catch(_com_error e)
	{
		hr = e.WCode();
		lRecordCount = 0;
	}

	return (S_OK == hr) ? lRecordCount : 0;
}
//----------------------------------------------------
BOOL CADOTable::AddNewItem()
{
	if (NULL == m_pRecordset)
		return FALSE;

	HRESULT hr = S_OK;
	try
	{
		hr = m_pRecordset->AddNew();
	}
	catch(_com_error e)
	{
		hr = e.WCode();
	}

	return (S_OK == hr) ? TRUE : FALSE;
}
//----------------------------------------------------
BOOL CADOTable::DeleteItem(LONG lIndex)
{
	if (NULL == m_pRecordset)
		return FALSE;

	HRESULT hr = S_OK;
	try
	{
		hr = m_pRecordset->MoveFirst();
		hr = m_pRecordset->Move(lIndex);
		hr = m_pRecordset->Delete(adAffectCurrent);
	}
	catch(_com_error e)
	{
		hr = e.WCode();
	}

	return (S_OK == hr) ? TRUE : FALSE;
}
//----------------------------------------------------
BOOL CADOTable::UpdateTable()
{
	if (NULL == m_pRecordset)
		return FALSE;

	HRESULT hr = S_OK;
	try
	{
		hr = m_pRecordset->Update();
	}
	catch(_com_error e)
	{
		hr = e.WCode();
	}

	return (S_OK == hr) ? TRUE : FALSE;
}
//----------------------------------------------------
BOOL CADOTable::GetRecordCell(LONG lIndex, 
							  LPCWSTR lpwzCellName, 
							  DWORD &dwResult)
{	
	_variant_t var;
	if (TRUE == _GetRecordCell(lIndex, lpwzCellName, var))
	{
		if (VT_I4 == var.vt || VT_I2 == var.vt)
		{
			dwResult = var.lVal;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}
//----------------------------------------------------
BOOL CADOTable::GetRecordCell(LONG lIndex, 
							  LPCWSTR lpwzCellName, 
							  float &fResult)
{
	_variant_t var;
	if (TRUE == _GetRecordCell(lIndex, lpwzCellName, var))
	{
		switch(var.vt)
		{
		case VT_R4:
			fResult = var.fltVal;
			break;
		case VT_R8:
			fResult = (float)var.dblVal;
			break;
		default:
			return FALSE;
			break;
		}
		return TRUE;
	}

	return FALSE;
}
//----------------------------------------------------
BOOL CADOTable::GetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, double &fResult)
{
	float fTemp;
	BOOL bResult = GetRecordCell(lIndex, lpwzCellName, fTemp);
	if (bResult) fResult = fTemp;

	return bResult;
}
//----------------------------------------------------
BOOL CADOTable::GetRecordCell(LONG lIndex, 
							  LPCWSTR lpwzCellName, 
							  CTime &dateResult)
{
	_variant_t var;
	if (TRUE == _GetRecordCell(lIndex, lpwzCellName, var))
	{
		if (VT_DATE == var.vt)
		{
			COleDateTime tm; 
			tm.m_dt = var.date;					
			dateResult = CTime(tm.GetYear(), tm.GetMonth(), tm.GetDay(), 0, 0, 0);
		}
		else
		{
			dateResult = CTime();
		}
		return TRUE;
	}

	return FALSE;
}
//----------------------------------------------------
BOOL CADOTable::GetRecordCell(LONG lIndex, 
							  LPCWSTR lpwzCellName, 
							  LPWSTR lpwzResult, 
							  DWORD dwMaxLen)
{
	_variant_t var;
	if (TRUE == _GetRecordCell(lIndex, lpwzCellName, var))
	{
		if (VT_BSTR == var.vt)
		{
			DWORD dwLen = (wcslen(var.bstrVal) < dwMaxLen) ? wcslen(var.bstrVal) : dwMaxLen;
			wcsncpy_s(lpwzResult, dwMaxLen, var.bstrVal, dwLen);
		}
		else
		{
			return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}
//----------------------------------------------------
BOOL CADOTable::SetRecordCell(LONG lIndex, 
							  LPCWSTR lpwzCellName, 
							  DWORD dwVal)
{
	_variant_t var;
	var.vt = VT_I4;
	var.lVal = dwVal;
	return _SetRecordCell(lIndex, lpwzCellName, var);
}
//----------------------------------------------------
BOOL CADOTable::SetRecordCell(LONG lIndex, 
							  LPCWSTR lpwzCellName, 
							  float fResult)
{
	_variant_t var;
	var.vt = VT_R4;
	var.fltVal = fResult;
	return _SetRecordCell(lIndex, lpwzCellName, var);
}
//----------------------------------------------------
BOOL CADOTable::SetRecordCell(LONG lIndex, 
							  LPCWSTR lpwzCellName, 
							  CTime dateResult)
{
	COleDateTime tm; 
	tm.SetDate(dateResult.GetYear(), dateResult.GetMonth(), dateResult.GetDay());

	_variant_t var;
	var.vt = VT_DATE;
	var.date = tm;
	return _SetRecordCell(lIndex, lpwzCellName, var);
}
//----------------------------------------------------
BOOL CADOTable::SetRecordCell(LONG lIndex, 
							  LPCWSTR lpwzCellName, 
							  LPWSTR lpwzResult)
{
	_variant_t var;
	var.vt = VT_BSTR;
	var.bstrVal = ::SysAllocString(lpwzResult);
	BOOL bRes = _SetRecordCell(lIndex, lpwzCellName, var);
	::SysFreeString(var.bstrVal);
	return bRes;
}
//----------------------------------------------------


	
//----------------------------------------------------
BOOL CADOTable::_GetRecordCell(LONG lIndex, 
							   LPCWSTR lpwzCellName, 
							   _variant_t &var)
{
	if(NULL == m_pRecordset || m_pRecordset->BOF)
		return FALSE;

	LONG lRecordCount = GetRecordCnt();
	if (lIndex >= lRecordCount)
		return FALSE;

	try
	{
		if (lIndex >= 0)
		{
			m_pRecordset->MoveFirst();
			m_pRecordset->Move(lIndex);
		}
		var = m_pRecordset->GetCollect(lpwzCellName);
	}
	catch(_com_error e)
	{
		return FALSE;
	}

	return TRUE;
}
//----------------------------------------------------
BOOL CADOTable::_SetRecordCell(LONG lIndex, 
							   LPCWSTR lpwzCellName, 
							   _variant_t &var)
{
	if(NULL == m_pRecordset || m_pRecordset->BOF)
		return FALSE;

	LONG lRecordCount = GetRecordCnt();
	if (lIndex >= lRecordCount)
		return FALSE;

	try
	{
		if (lIndex >= 0)
		{
			m_pRecordset->MoveFirst();
			m_pRecordset->Move(lIndex);
		}
		m_pRecordset->PutCollect(lpwzCellName, var);
	}
	catch(_com_error e)
	{
		return FALSE;
	}

	return TRUE;
}
//----------------------------------------------------