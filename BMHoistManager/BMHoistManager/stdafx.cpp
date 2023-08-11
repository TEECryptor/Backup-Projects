// stdafx.cpp : source file that includes just the standard includes
// BMHoistManager.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "StdAfx.h"

BOOL InitListHeader(CColorListCtrl* pListCtrl, TableData* pStruct)
{
	if (!pListCtrl || !pStruct)
	{
		return FALSE;
	}

	// 清除当前表格内容
	int iColumnCount = pListCtrl->GetHeaderCtrl()->GetItemCount();
	pListCtrl->DeleteAllItems();
	for (int i = 0;i < iColumnCount; i++)
	{
	   pListCtrl->DeleteColumn(i);
	}

	// 根据当前要打开的表,构造新的表格控件
	int iIndex = 0;
	for(TableColumnsList::iterator it = pStruct->lstColumns.begin();
		it != pStruct->lstColumns.end();
		it++, iIndex++)
	{
		TableColumn cl = *it;
		if (cl.bIsVisible)
		{
			pListCtrl->InsertColumn(iIndex, cl.strDisplayText, 0, cl.lColumnWidth);
		}
	}

	return TRUE;
}

BOOL FillListFromTable(CColorListCtrl* pListCtrl, CADOTable* pTable, TableData* pStruct, DWORD dwStart, DWORD dwEnd)
{
	if (!pListCtrl || !pTable || !pStruct)
	{
		return FALSE;
	}

	for (DWORD dwIndex = dwStart; dwIndex < dwEnd; dwIndex++)
	{
		DWORD dwColumn = 0;
		for(TableColumnsList::iterator it = pStruct->lstColumns.begin();
			it != pStruct->lstColumns.end();
			it++, dwColumn++)
		{
			TableColumn cl = *it;
			if (!cl.bIsVisible)
			{
				dwColumn--;
				continue;
			}

			CString strItemText;
			switch(cl.eDataType)
			{
			case eTableDataType_DWORD:
				{
					DWORD dwCellValue = 0;
					if (TRUE == cl.bIsIndex)
						dwCellValue = dwIndex + 1;
					else
						pTable->GetRecordCell(dwIndex, cl.strName, dwCellValue);
					strItemText.Format(_T("%d"), dwCellValue);
				}
				break;
			case eTableDataType_FLOAT:
				{
					float fCellValue = 0;
					pTable->GetRecordCell(dwIndex, cl.strName, fCellValue);
					strItemText.Format(_T("%.2f"), fCellValue);
				}
				break;
			case eTableDataType_Date:
				{
					CTime tm, nullTm;
					pTable->GetRecordCell(dwIndex, cl.strName, tm);
					if (tm != nullTm)	
					strItemText.Format(_T("%02d/%02d/%d"), tm.GetMonth(), tm.GetDay(), tm.GetYear());
				}
				break;
			case eTableDataType_String:
			default:
				{
					WCHAR wcCellValue[256] = {0};
					pTable->GetRecordCell(dwIndex, cl.strName, wcCellValue, 256);
					strItemText = wcCellValue;
				}
				break;
			}
			
			if (0 == dwColumn)
				pListCtrl->InsertItem(dwIndex, strItemText);
			else
				pListCtrl->SetItemText(dwIndex, dwColumn, strItemText);
			pListCtrl->SetFullItemBKColor(dwIndex, dwIndex%2==0? RGB(180,180,90):RGB(90,180,180));
		}
	}

	return TRUE;
}


DWORD GetNewRecordIndex(CString strTableName, CString strIndexColumn, CString strExt)
{
	BOOL bResult = FALSE;
	DWORD dwCurIndex = 0;
	DWORD dwItemCnt = 0;
	CString strSQL;
	CADOTable table;

	strSQL.Format(_T("SELECT %s FROM %s WHERE %s LIKE '%s%s' ORDER BY %s"), 
		strIndexColumn, strTableName, strIndexColumn, strExt, _T("%"), strIndexColumn);

	bResult = table.OpenTable(g_pDatabase->GetConnectionPtr(), strSQL);
	if (FALSE == bResult)
	{
		return 0;
	}
	
	dwItemCnt = table.GetRecordCnt();
	if (dwItemCnt > 0)
	{
		CString strID;
		TCHAR tcIDValue[32] = {0};
		table.GetRecordCell(dwItemCnt-1, strIndexColumn, tcIDValue, 32);
		strID = tcIDValue;
		strID = strID.Right(strID.GetLength() - strExt.GetLength());
		dwCurIndex = _ttol(strID);
	}
	dwCurIndex += 1;
	table.CloseTable();

	return dwCurIndex;
}


BOOL FillComboBoxFromTable(CString strSQL, CString strColumnName, 
						   CString strDefault, CComboBox* pComboBox)
{	
	CADOTable table;
	DWORD dwRecordCnt = 0;
	DWORD dwDefaultIndex = 0;

	if (strSQL.IsEmpty() || strColumnName.IsEmpty() || !pComboBox)
	{
		return FALSE;
	}

	pComboBox->ResetContent();

	if (!table.OpenTable(g_pDatabase->GetConnectionPtr(), strSQL))
	{
		return FALSE;
	}

	dwRecordCnt = table.GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
	{
		DWORD dwIsDefault = 0;
		CString strColumnValue;
		table.GetRecordCell(dwIndex, strColumnName, strColumnValue);
		pComboBox->AddString(strColumnValue);
		if (strColumnValue == strDefault)
		{
			dwDefaultIndex = dwIndex;
		}
	}
	table.CloseTable();

	pComboBox->SetCurSel(dwDefaultIndex);

	return TRUE;
}


BOOL FillEquipmentsComboBox(CComboBox* pComboBox)
{
	if (!pComboBox)
	{
		return FALSE;
	}
	
	pComboBox->ResetContent();
	pComboBox->AddString(_T("吉替葫芦"));
	pComboBox->AddString(_T("摩睿葫芦"));
	pComboBox->AddString(_T("速卫葫芦"));
	pComboBox->AddString(_T("吉替备件"));
	pComboBox->AddString(_T("速卫备件"));
	pComboBox->AddString(_T("摩睿备件"));
	pComboBox->AddString(_T("吉替全套部件"));
	pComboBox->AddString(_T("速卫全套部件"));
	pComboBox->AddString(_T("摩睿全套部件"));
	pComboBox->AddString(_T("端梁"));
	pComboBox->AddString(_T("驱动"));
	pComboBox->AddString(_T("混合"));
	pComboBox->AddString(_T("其他"));
	pComboBox->SetCurSel(0);

	return TRUE;
}
