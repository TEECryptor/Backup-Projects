//-----------------------------------------------------------------------
//	File Name:ExcelExporter.cpp
//	Introduce:Class CExcelExporter implementation file
//-----------------------------------------------------------------------
#include "StdAfx.h"
#include "ExcelExporter.h"
//-----------------------------------------------------------------------
CExcelExporter::CExcelExporter(void)
{
}
//-----------------------------------------------------------------------

CExcelExporter::~CExcelExporter(void)
{
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
BOOL CExcelExporter::SaveDataFromListCtrl(LPCTSTR lpszExcelFile, 
										  LPCTSTR lpszSheetName,
										  TableData &tbStruct,
										  CListCtrl* pListCtrl)
{
	if (NULL == lpszExcelFile || _tcslen(lpszExcelFile) == 0)
		return FALSE;
	if (NULL == lpszSheetName || _tcslen(lpszSheetName) == 0)
		return FALSE;
	if (NULL == pListCtrl)
		return FALSE;

	TCHAR tcSQL[512] = {_T("\0")};
	TCHAR tcDriver[] = {_T("MICROSOFT EXCEL DRIVER (*.XLS)")};
	_stprintf(	tcSQL, 
				_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),
				tcDriver,
				lpszExcelFile,
				lpszExcelFile);
	TRY
	{
		// Create/Open the excel file
		CDatabase db;
		if(FALSE == db.OpenEx(tcSQL, CDatabase::noOdbcDialog))
			return FALSE;

		// To check the same sheet is exist or not
		BOOL bHasSameSheet = FALSE;
		_stprintf(tcSQL, _T("SELECT * FROM %s"), lpszSheetName);
		TRY
		{
			db.ExecuteSQL(tcSQL);
			bHasSameSheet = TRUE;
		}
		CATCH_ALL(e)
		{
			bHasSameSheet = FALSE;
		}
		END_CATCH_ALL;

		// Create sheet
		int iIndex = 0;
		TCHAR tcColumnText[512] = {_T("\0")};
		_stprintf(tcSQL, _T("CREATE TABLE %s ("), lpszSheetName);
		CHeaderCtrl* pListHeader = pListCtrl->GetHeaderCtrl();
		for (TableColumnsList::iterator it = tbStruct.lstColumns.begin();
			it != tbStruct.lstColumns.end() && iIndex < pListHeader->GetItemCount();
			it++, iIndex++)
		{
			HDITEM hdi;
			TCHAR  lpBuffer[256];
			bool   fFound = false;
			//
			hdi.mask = HDI_TEXT;
			hdi.pszText = lpBuffer;
			hdi.cchTextMax = 256;
			pListHeader->GetItem(iIndex, &hdi);
			//
			if (0 != iIndex)
			{
				_tcscat(tcSQL, _T(","));
				_tcscat(tcColumnText, _T(","));
			}
			_tcscat(tcSQL, lpBuffer);
			if (eTableDataType_DWORD == it->eDataType)
				_tcscat(tcSQL, _T(" NUMBER"));
			else
				_tcscat(tcSQL, _T(" TEXT"));
			//			
			_tcscat(tcColumnText, lpBuffer);
		}
		_tcscat(tcSQL, _T(")"));
		if (FALSE == bHasSameSheet)
		{
			db.ExecuteSQL(tcSQL);
		}

		// Copy data to sheet
		int iItemCnt = pListCtrl->GetItemCount();
		int iColumnCnt = pListHeader->GetItemCount();
		for (int iItem = 0; iItem < iItemCnt; iItem++)
		{
			_stprintf(tcSQL, _T("INSERT INTO %s ("), lpszSheetName);
			_tcscat(tcSQL, tcColumnText);
			_tcscat(tcSQL, _T(") VALUES ("));

			for (int iColumn = 0; iColumn < iColumnCnt; iColumn++)
			{
				TCHAR tcItemText[128] = {_T("\0")};
				pListCtrl->GetItemText(iItem, iColumn, tcItemText, 128);
				if (_tcslen(tcItemText) == 0) _tcscpy(tcItemText, _T(" "));
				if (iColumn > 0) _tcscat(tcSQL, _T(","));
				_tcscat(tcSQL, _T("'"));
				_tcscat(tcSQL, tcItemText);
				_tcscat(tcSQL, _T("'"));
			}
			_tcscat(tcSQL, _T(")"));
			db.ExecuteSQL(tcSQL);
		}
		db.Close();
	}
	CATCH_ALL(e)
	{
		return FALSE;
	}
	END_CATCH_ALL;

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CExcelExporter::SaveDataFromTable(LPCTSTR lpszExcelFile, 
									   LPCTSTR lpszSheetName, 
									   TableData &tbStruct, 
									   CADOTable table)
{
	if (NULL == lpszExcelFile || _tcslen(lpszExcelFile) == 0)
		return FALSE;
	if (NULL == lpszSheetName || _tcslen(lpszSheetName) == 0)
		return FALSE;

	CDatabase db;
	TCHAR tcSQL[2048] = {_T("\0")};
	TCHAR tcDriver[] = {_T("MICROSOFT EXCEL DRIVER (*.XLS)")};
	_stprintf(	tcSQL, 
				_T("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),
				tcDriver,
				lpszExcelFile,
				lpszExcelFile);
	TRY
	{
		// Create/Open the excel file
		if(FALSE == db.OpenEx(tcSQL, CDatabase::noOdbcDialog))
			return FALSE;

		// To check the same sheet is exist or not
		BOOL bHasSameSheet = FALSE;
		_stprintf(tcSQL, _T("SELECT * FROM %s"), lpszSheetName);
		TRY
		{
			db.ExecuteSQL(tcSQL);
			bHasSameSheet = TRUE;
		}
		CATCH_ALL(e)
		{
			bHasSameSheet = FALSE;
		}
		END_CATCH_ALL;

		// Create sheet
		int iCurColumn = 0;
		_stprintf(tcSQL, _T("CREATE TABLE %s ("), lpszSheetName);
		for (TableColumnsList::iterator it = tbStruct.lstColumns.begin();
			it != tbStruct.lstColumns.end();
			it++)
		{			
			if (it->bIsIndex)
				continue;

			if (0 != iCurColumn)
			{
				_tcscat(tcSQL, _T(","));
			}
			_tcscat(tcSQL, _T("["));
			_tcscat(tcSQL, it->strName);
			_tcscat(tcSQL, _T("]"));
			if (eTableDataType_DWORD == it->eDataType || eTableDataType_FLOAT == it->eDataType)
				_tcscat(tcSQL, _T(" NUMBER"));
			else
				_tcscat(tcSQL, _T(" TEXT"));
			iCurColumn++;
		}
		_tcscat(tcSQL, _T(")"));
		if (FALSE == bHasSameSheet)
		{
			db.ExecuteSQL(tcSQL);
		}

		// Copy data to sheet
		int iItemCnt = table.GetRecordCnt();
		int iColumnCnt = tbStruct.lstColumns.size();
		for (int iItem = 0; iItem < iItemCnt; iItem++)
		{
			_stprintf(tcSQL, _T("INSERT INTO %s "), lpszSheetName);
			_tcscat(tcSQL, _T(" VALUES ("));

			int iColumn = 0;
			for (TableColumnsList::iterator it = tbStruct.lstColumns.begin();
				it != tbStruct.lstColumns.end();
				it++)
			{
				if (it->bIsIndex)
					continue;

				TCHAR tcItemText[128] = {_T("\0")};
				switch(it->eDataType)
				{
				case eTableDataType_DWORD:
					{
						DWORD dwValue;
						table.GetRecordCell(iItem, it->strName, dwValue);
						_stprintf(tcItemText, _T("%d"), dwValue);
					}
					break;
				case eTableDataType_FLOAT:
					{
						float fValue;
						table.GetRecordCell(iItem, it->strName, fValue);
						_stprintf(tcItemText, _T("%f"), fValue);
					}
					break;
				case eTableDataType_Date:
					{
						CTime tm;
						table.GetRecordCell(iItem, it->strName, tm);
						_stprintf(tcItemText, _T("%02d/%02d/%d"), tm.GetMonth(), tm.GetDay(), tm.GetYear());
					}
					break;	
				case eTableDataType_String:
				default:
					{
						table.GetRecordCell(iItem, it->strName, tcItemText, 128);
					}
					break;
				}
				if (_tcslen(tcItemText) == 0) 
				{
					if (eTableDataType_DWORD != it->eDataType && eTableDataType_FLOAT != it->eDataType)
						_tcscpy(tcItemText, _T(" "));
					else
						_tcscpy(tcItemText, _T("0"));
				}
				if (iColumn > 0) _tcscat(tcSQL, _T(","));
				if (eTableDataType_DWORD != it->eDataType && eTableDataType_FLOAT != it->eDataType) _tcscat(tcSQL, _T("'"));
				_tcscat(tcSQL, tcItemText);
				if (eTableDataType_DWORD != it->eDataType && eTableDataType_FLOAT != it->eDataType) _tcscat(tcSQL, _T("'"));
				iColumn++;
			}
			_tcscat(tcSQL, _T(")"));
			db.ExecuteSQL(tcSQL);
		}
		db.Close();
	}
	CATCH_ALL(e)
	{
		db.Close();
		return FALSE;
	}
	END_CATCH_ALL;

	return TRUE;
}
//-----------------------------------------------------------------------
