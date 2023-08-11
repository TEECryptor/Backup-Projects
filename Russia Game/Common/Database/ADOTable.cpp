//--------------------------------------------------
//	FileName:ADOTable.cpp
//	Introduce:The .cpp file of CADOTable class.
//	Creator:Youfang Yao
//	Date:04/05/06
//	Version:1.0V
//--------------------------------------------------
#include "stdafx.h"
#include <comutil.h>
#include "ADOTable.h"
//--------------------------------------------------
//	Name:CADOTable
//	Introduce:Construction
//--------------------------------------------------
CADOTable::CADOTable()
 : m_pConnect(NULL),
   m_pRecordset(NULL)
{
	lstrcpy(m_szCurTableName, _T("\0"));
}
//--------------------------------------------------
//	Name:~CADOTable
//	Introduce:Distruction
//--------------------------------------------------
CADOTable::~CADOTable()
{
	Close();
}
//--------------------------------------------------
//	Name:Open
//	Introduce:Open the recordset based on connect pointer and table name
//	Param:pConnect:i:The databse connect interface pointerg
//        lpszTable:i:Table name string
//	Return:Return S_OK if successful, otherwise return an error code
//--------------------------------------------------
HRESULT	CADOTable::Open(CADODatabase* pConnect, LPCTSTR lpszTable)
{
	HRESULT hr = S_OK;

	if(NULL == pConnect)
		return E_POINTER;	

	m_pConnect = pConnect;
	lstrcpy(m_szCurTableName, lpszTable);

	TCHAR szSQLString[MAX_SQL_LEN] = {_T("\0")};
	lstrcpy(szSQLString, _T("Select * from "));
	lstrcat(szSQLString, m_szCurTableName);
	
	try
	{
		if(m_pRecordset)
		{
			Close();
		}

		hr = m_pRecordset.CreateInstance("ADODB.Recordset");
		if(FAILED(hr))
			return hr;
		
		m_pRecordset->CursorLocation = adUseClient;
		hr = m_pRecordset->Open(szSQLString, _variant_t((IDispatch*)m_pConnect->GetConnectPointer(), TRUE), adOpenStatic, adLockOptimistic, adCmdText);
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
//	Name:GetRecordsetCnt
//	Introduce:Get the recordset count
//	Param:N/A
//	Return:Return the count for this recordset
//--------------------------------------------------
LONG CADOTable::GetRecordsetCnt()
{
	if(NULL == m_pRecordset)
		return 0;

	return m_pRecordset->RecordCount;
}
//--------------------------------------------------
//	Name:GetValue
//	Introduce:Get recordset value string based on title string
//	Param:lpszTitle:i:The title string
//        lpszValue:i:The recordset value string
//	Return:Return S_OK if successful, otherwise return an error code
//--------------------------------------------------
HRESULT	CADOTable::GetValue(LPCTSTR lpszTitle, LPTSTR lpszValue)
{
	HRESULT hr = S_OK;
	
	if((NULL == lpszTitle) || (NULL == lpszValue))
		return E_POINTER;

	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	try
	{
		_variant_t vrTmp = m_pRecordset->GetCollect(lpszTitle);
		vrTmp.ChangeType(VT_BSTR);

		LPTSTR lpszTmp = _com_util::ConvertBSTRToString(vrTmp.bstrVal);
		lstrcpy(lpszValue, lpszTmp);
		delete []lpszTmp;
	}
	catch(_com_error e)
	{
		return e.Error();
	}

	return S_OK;
}
//--------------------------------------------------
//	Name:MoveFirst
//	Introduce:Move the recorset to the first
//	Param:N/A
//	Return:Return S_OK if successful, otherwise return an error code
//--------------------------------------------------
HRESULT CADOTable::MoveFirst()
{
	HRESULT hr = S_OK;
	
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	try
	{
		hr = m_pRecordset->MoveFirst();
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
//	Name:MovePrevious
//	Introduce:Move the recordset to the previous one 
//	Param:N/A
//	Return:Return S_OK if successful, otherwise return an error code
//--------------------------------------------------
HRESULT	CADOTable::MovePrevious()
{
	HRESULT hr = S_OK;
	
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	try
	{
		hr = m_pRecordset->MovePrevious();
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
//	Name:MoveNext
//	Introduce:Move the recorset to the next one
//	Param:N/A
//	Return:Return S_OK if successful, otherwise return an error code
//--------------------------------------------------
HRESULT CADOTable::MoveNext()
{
	HRESULT hr = S_OK;
	
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	try
	{
		hr = m_pRecordset->MoveNext();
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
//	Name:MoveEnd
//	Introduce:Move the recorset to the end
//	Param:N/A
//	Return:Return S_OK if successful, otherwise return an error code
//--------------------------------------------------
HRESULT CADOTable::MoveEnd()
{
	HRESULT hr = S_OK;
	
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	try
	{
		hr = m_pRecordset->MoveLast();
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
//	Name:IsBOF
//	Introduce:The recordset is at begin or not
//	Param:N/A
//	Return:Return TRUE when at the begin of recordset , otherwise return FALSE
//--------------------------------------------------
BOOL CADOTable::IsBOF()
{
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return TRUE;

	return m_pRecordset->BOF;
}
//--------------------------------------------------
//	Name:IsBOF
//	Introduce:The recordset is at end or not
//	Param:N/A
//	Return:Return TRUE when at the end of recordset , otherwise return FALSE
//--------------------------------------------------
BOOL CADOTable::IsEOF()
{
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return TRUE;
	
	return m_pRecordset->adoEOF;
}
//--------------------------------------------------
//	Name:Close
//	Introduce:To close the table
//	Param:N/A
//	Return:Return S_OK if successful , otherwise return FALSE
//--------------------------------------------------
HRESULT CADOTable::Close()
{
	if(m_pRecordset)
	{
		if(m_pRecordset->State)
			m_pRecordset->Close();
		m_pRecordset.Release();
		m_pRecordset = NULL;
	}

	return S_OK;
}
//--------------------------------------------------
//	Name:AddRecord
//	Introduce:To add a new record to current table
//	Param:lppTitles:i:The new record title strings
//        lppValues:i:The new record value strings
//        iCnt:i:The count
//	Return:Return S_OK if successful , otherwise return FALSE
//--------------------------------------------------
HRESULT CADOTable::AddRecord(LPCTSTR lppTitles[], LPCTSTR lppValues[], int iCnt)
{
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	if((lstrcmp(m_szCurTableName, _T("\0")) == 0) || (iCnt <= 0))
		return S_FALSE;

	HRESULT hr = S_OK;
	try
	{
		//Add a new record to current table
		hr = m_pRecordset->AddNew();
		if(FAILED(hr))
			return hr;

		//Fill the new record
		for(int i = 0; i < iCnt; i++)
		{
			m_pRecordset->PutCollect(_variant_t(lppTitles[i]), _variant_t(lppValues[i]));
		}

		//Update the new record to database
		hr = m_pRecordset->Update();
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
//	Name:EditRecord
//	Introduce:To edit current the record int the table
//	Param:lppTitles:i:The table titles for editing
//        lppValues:i:The values to titles
//        iCnt:i:The titles and values count
//	Return:Return TRUE when at the end of recordset , otherwise return FALSE
//--------------------------------------------------
HRESULT CADOTable::EditRecord(LPCTSTR lppTitles[], LPCTSTR lppValues[], int iCnt)
{
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	if((lstrcmp(m_szCurTableName, _T("\0")) == 0) || (iCnt <= 0))
		return S_FALSE;

	HRESULT hr = S_OK;
	try
	{
		//Fill the new record
		for(int i = 0; i < iCnt; i++)
		{
			m_pRecordset->PutCollect(_variant_t(lppTitles[i]), _variant_t(lppValues[i]));
		}

		//Update the new record to database
		hr = m_pRecordset->Update();
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
//	Name:FindRecord
//	Introduce:To find records with find condition
//	Param:lpFindString:i:The find condition string
//	Return:Return TRUE when at the end of recordset , otherwise return FALSE
//--------------------------------------------------
HRESULT CADOTable::FindRecord(LPCTSTR lpFindString)
{
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	if((lstrcmp(m_szCurTableName, _T("\0")) == 0) || (NULL == lpFindString))
		return E_POINTER;

	try
	{
		m_pRecordset->PutFilter(lpFindString);
	}
	catch(_com_error e)
	{
		return e.Error();
	}

	return S_OK;
}
//--------------------------------------------------
//	Name:SortRecord
//	Introduce:To sort the records based specific title
//	Param:lpSortTile:i:The sort title string
//	Return:Return TRUE when at the end of recordset , otherwise return FALSE
//--------------------------------------------------
HRESULT CADOTable::SortRecord(LPCTSTR lpSortTile)
{
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	try
	{
		m_pRecordset->PutSort(lpSortTile);
	}
	catch(_com_error e)
	{
		return e.Error();
	}

	return S_OK;
}
//--------------------------------------------------
//	Name:DeleteRecord
//	Introduce:To delete a record in the table based on iIndex
//	Param:iIndex:i:The iIndex for record will be delete
//	Return:Return TRUE when at the end of recordset , otherwise return FALSE
//--------------------------------------------------
HRESULT CADOTable::DeleteRecord(int iIndex)
{
	if((NULL == m_pConnect) || (NULL == m_pRecordset))
		return E_POINTER;

	if(iIndex < 0)
		return S_FALSE;

	HRESULT hr = S_OK;
	try
	{
		hr = m_pRecordset->MoveFirst();
		if(FAILED(hr))
			return hr;

		hr = m_pRecordset->Move(iIndex);
		if(FAILED(hr))
			return hr;

		hr = m_pRecordset->Delete(adAffectCurrent);
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