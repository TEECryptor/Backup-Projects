//----------------------------------------------------
//	File Name:CommonDefine.cpp
//	Introduce:Implement file for commonDefine.h
//----------------------------------------------------
#include "stdafx.h"
#include "CommonDefine.h"
//----------------------------------------------------
#include <shlwapi.h>
#include <shlobj.h>
#include "..\\BMHoistSaleManager\\BMParamCalculator.h"
//----------------------------------------------------
CString	g_strUserID = _T("\0");
CString	g_strUserName = _T("\0");
CADODataBase* g_pSettingDatabase = NULL;
CADODataBase* g_pOfferDatabase = NULL;
//----------------------------------------------------



//----------------------------------------------------
void InitDataListCtrl(CListCtrl* pListCtrl, 
					  TableData &td)
{
	if (NULL == pListCtrl)
		return;

	int iColumnCnt = pListCtrl->GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < iColumnCnt; i++)
		pListCtrl->DeleteColumn(0);
	
	int iIndex = 0;
	for(TableColumnsList::iterator it = td.lstColumns.begin();
		it != td.lstColumns.end();
		it++, iIndex++)
	{
		TableColumn cl = *it;
		if (cl.bIsVisible)
		{
			pListCtrl->InsertColumn(iIndex, cl.nTextID > 0 ? myGetText(cl.nTextID) : cl.strDispalyText, 0, cl.lColumnWidth);
		}
	}
}
void ShowTableRecord(CADOTable* pTable, 
					 int iRecordIndex, 
					 CListCtrl* pListCtrl, 
					 TableData &td)
{
	if (iRecordIndex < 0 || NULL == pTable || NULL == pListCtrl)
		return;

	DWORD dwColumn = 0;
	for(TableColumnsList::iterator it = td.lstColumns.begin();
		it != td.lstColumns.end();
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
					dwCellValue = iRecordIndex + 1;
				else
					pTable->GetRecordCell(iRecordIndex, cl.strName, dwCellValue);
				strItemText.Format(_T("%d"), dwCellValue);
			}
			break;
		case eTableDataType_FLOAT:
			{
				float fCellValue = 0;
				pTable->GetRecordCell(iRecordIndex, cl.strName, fCellValue);
				strItemText.Format(_T("%.2f"), fCellValue);
			}
			break;
		case eTableDataType_Date:
			{
				CTime tm;
				pTable->GetRecordCell(iRecordIndex, cl.strName, tm);
				strItemText.Format(_T("%02d/%02d/%d"), tm.GetMonth(), tm.GetDay(), tm.GetYear());
			}
			break;
		case eTableDataType_String:
		default:
			{
				WCHAR wcCellValue[256] = {0};
				pTable->GetRecordCell(iRecordIndex, cl.strName, wcCellValue, 256);
				strItemText = wcCellValue;
			}
			break;
		}
		if (0 == dwColumn)
			pListCtrl->InsertItem(iRecordIndex, strItemText);
		else
			pListCtrl->SetItemText(iRecordIndex, dwColumn, strItemText);
	}
}
//----------------------------------------------------
void ShowTableRecords(CADOTable* pTable, 
					  CListCtrl* pListCtrl, 
					  TableData &td)
{
	if (NULL == pTable || NULL == pListCtrl)
		return;

	pListCtrl->DeleteAllItems();
	
	DWORD dwRecordCnt = pTable->GetRecordCnt();
	for (DWORD dwIndex = 0; dwIndex < dwRecordCnt; dwIndex++)
	{
		ShowTableRecord(pTable, dwIndex, pListCtrl, td);
	}
}
//----------------------------------------------------
void SelectComboBoxItem(CComboBox* pComboBox, int iItemData)
{
	if (NULL == pComboBox || pComboBox->GetCount() == 0)
		return;

	DWORD dwItemCnt = pComboBox->GetCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		int iCurItemData = pComboBox->GetItemData(dwIndex);
		if (iItemData == iCurItemData)
		{
			pComboBox->SetCurSel(dwIndex);
			return;
		}
	}
	pComboBox->SetCurSel(0);
}
//----------------------------------------------------
void SelectComboBoxItem(CComboBox* pComboBox, CString strItemText)
{
	if (NULL == pComboBox || pComboBox->GetCount() == 0)
		return;

	DWORD dwItemCnt = pComboBox->GetCount();
	for (DWORD dwIndex = 0; dwIndex < dwItemCnt; dwIndex++)
	{
		CString strText;
		pComboBox->GetLBText(dwIndex, strText);
		if (strText == strItemText)
		{
			pComboBox->SetCurSel(dwIndex);
			return;
		}
	}
	pComboBox->SetCurSel(0);
}
//----------------------------------------------------
CString GetOfferStatusText(eOfferStatus eStatus)
{
	static CString strStatusText;
	strStatusText = _T("\0");
	switch (eStatus)
	{
	case eOfferStatus_New:
		strStatusText = _T("New");
		break;
	case eOfferStatus_Process:
		strStatusText = _T("In Process");
		break;
	case eOfferStatus_Closed:
		strStatusText = _T("Closed");
		break;	
	default:
		strStatusText = _T("\0");
		break;
	}

	return strStatusText;
}
//----------------------------------------------------
CString GetStringValueFromTable(CString strSQL, CString strName, CADODataBase* pDatabase)
{	
	CString strResult = _T("");

	CADOTable tb;
	BOOL bResult = FALSE;	
	if (pDatabase == NULL) 
		bResult = tb.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL);
	else
		bResult = tb.OpenTable(pDatabase->GetConnectionPtr(), strSQL);
	
	if (bResult && tb.GetRecordCnt() > 0)
	{
		TCHAR tcValue[256] = {0};
		tb.GetRecordCell(0, strName, tcValue, 256);
		strResult = tcValue;
		tb.CloseTable();
	}
	return strResult;
}
//----------------------------------------------------
LONG GetLONGValueFromTable(CString strSQL, CString strName, CADODataBase* pDatabase)
{
	LONG lResult = 0;

	CADOTable tb;
	BOOL bResult = FALSE;	
	if (pDatabase == NULL) 
		bResult = tb.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL);
	else
		bResult = tb.OpenTable(pDatabase->GetConnectionPtr(), strSQL);
	
	if (bResult && tb.GetRecordCnt() > 0)
	{
		DWORD dwValue = {0};
		tb.GetRecordCell(0, strName, dwValue);
		lResult = dwValue;
		tb.CloseTable();
	}
	return lResult;
}
//----------------------------------------------------
float GetFLOATValueFromTable(CString strSQL, CString strName, CADODataBase* pDatabase)
{
	float fResult = 0.0f;

	CADOTable tb;
	BOOL bResult = FALSE;	
	if (pDatabase == NULL) 
		bResult = tb.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL);
	else
		bResult = tb.OpenTable(pDatabase->GetConnectionPtr(), strSQL);
	
	if (bResult && tb.GetRecordCnt() > 0)
	{
		tb.GetRecordCell(0, strName, fResult);
		tb.CloseTable();
	}
	return fResult;
}
//----------------------------------------------------



//----------------------------------------------------
BOOL ExportEndCarriageDataToWordTemplate(CMSWordApplication* pWordApp, 
										  LPVOID pPointer)
{
	if (NULL == pWordApp || NULL == pPointer)
		return FALSE;
	CBMParamCalculator* pCalculator = (CBMParamCalculator*)pPointer;

	CString strTemp;
	StringPartnerList lstTextPartner;		
	lstTextPartner.push_back(StringPartner(_T("(ececcode)"), pCalculator->GetEndCarDataText()));

	lstTextPartner.push_back(StringPartner(_T("(ecec1)"), pCalculator->m_strEndCraCode));
	lstTextPartner.push_back(StringPartner(_T("(ecec2)"), pCalculator->m_strDriverCode));
	lstTextPartner.push_back(StringPartner(_T("(ecec3)"), pCalculator->m_strRailType));
	lstTextPartner.push_back(StringPartner(_T("(ecec4)"), pCalculator->GetJoiPlaTypeString()));
	//
	float fBufExtLen = pCalculator->GetBufferExtend();
	if (fBufExtLen > 0)	strTemp.Format(_T("%.2f"), fBufExtLen);
	else strTemp = _T("不需要");
	lstTextPartner.push_back(StringPartner(_T("(ecec5)"), strTemp));
	//
	lstTextPartner.push_back(StringPartner(_T("(ecec6)"), pCalculator->m_strECStructure));
	//
	strTemp.Format(_T("%d"), pCalculator->GetWheelBase());		
	lstTextPartner.push_back(StringPartner(_T("(ecec7)"), strTemp));
	//			
	CString strSQL;
	CString strWheDim;	
	strWheDim.Format(_T("%c%c"), pCalculator->m_strEndCraCode.GetAt(1), pCalculator->m_strEndCraCode.GetAt(2));
	int iWheelDim = _ttol(strWheDim);
	//
	CString strStructHeight;
	strStructHeight = pCalculator->m_strECStructure.Left(3);
	int iStructH = _ttol(strStructHeight);
	//
	int iECEC9 = 0;
	int iECEC10 = 0;
	int iECEC11 = 0;
	int iECEC12 = 0;
	int iECEC13 = 0;
	int iECEC14 = 0;
	int iECEC15 = 0;
	int iECEC16 = 0;
	int iECEC17 = 0;
	int iECEC18 = 0;
	int iECEC19 = 0;
	int iECEC20 = 0;
	int iECEC21 = 0;
	int iECEC22 = 0;
	int iECEC23 = 0;
	int iECEC24 = 0;
	int iECEC25 = 0;
	int iECEC26 = 0;
	int iECEC27 = 0;
	int iECEC28 = 0;
	int iECEC29 = 0;
	int iECEC30 = 0;
	int iECEC31 = 0;
	int iECEC32 = 0;
	int iECEC33 = 0;
	int iECEC34 = 0;
	int iECEC35 = 0;
	CADOTable tbJoiPlaDat;
	CADOTable tbTroDriDat;
	strSQL.Format(_T("SELECT * FROM %s WHERE WheelDim=%d"), g_tcTableName[eTableID_JoiPlaDat], iWheelDim);
	if (tbJoiPlaDat.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbJoiPlaDat.GetRecordCnt() > 0)
	{
		tbJoiPlaDat.GetRecordCell(0, L"JoiPlath", (DWORD&)iECEC9);
		tbJoiPlaDat.GetRecordCell(0, L"ecec17", (DWORD&)iECEC17);
		tbJoiPlaDat.GetRecordCell(0, L"ecec21", (DWORD&)iECEC21);
		tbJoiPlaDat.GetRecordCell(0, L"ecec26", (DWORD&)iECEC26);
		tbJoiPlaDat.GetRecordCell(0, L"ecec27", (DWORD&)iECEC27);
		tbJoiPlaDat.GetRecordCell(0, L"ecec30", (DWORD&)iECEC30);
		tbJoiPlaDat.GetRecordCell(0, L"ecec32", (DWORD&)iECEC32);
		tbJoiPlaDat.GetRecordCell(0, L"ecec35", (DWORD&)iECEC35);
	}
	tbJoiPlaDat.CloseTable();
	//
	int iECEC181 = 0;
	strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], pCalculator->m_strDriverCode);	
	if (tbTroDriDat.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbTroDriDat.GetRecordCnt() > 0)
	{
		tbTroDriDat.GetRecordCell(0, L"ecec181", (DWORD&)iECEC181);
		tbTroDriDat.GetRecordCell(0, L"ecec23", (DWORD&)iECEC23);
		tbTroDriDat.GetRecordCell(0, L"ecec29", (DWORD&)iECEC29);
	}
	tbTroDriDat.CloseTable();	
	//	
	CADOTable tbEndCarMatPar;
	strSQL.Format(_T("SELECT * FROM %s WHERE EndCarSec='%s'"), g_tcTableName[eTableID_EndCarMatPar], pCalculator->m_strECStructure);
	if (tbEndCarMatPar.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbEndCarMatPar.GetRecordCnt() > 0)
	{
		tbEndCarMatPar.GetRecordCell(0, L"Bufhei", (DWORD&)iECEC22);
		tbEndCarMatPar.GetRecordCell(0, L"ecec24", (DWORD&)iECEC24);
	}
	tbEndCarMatPar.CloseTable();
	//
	CString strCodBufSel = pCalculator->m_strEndCraCode.Left(3);
	strSQL.Format(_T("SELECT * FROM %s WHERE CodBufSel='%s'"), g_tcTableName[eTableID_Buffer], strCodBufSel);	
	int iBufLen = GetFLOATValueFromTable(strSQL, L"BufLen");
	//
	CString strRailLen;
	strRailLen.Format(_T("%c%c"), pCalculator->m_strEndCraCode.GetAt(3), pCalculator->m_strEndCraCode.GetAt(4));
	int iRailLen = _ttol(strRailLen);
	//
	iECEC10 = _ttol(pCalculator->m_strRaiGauge);
	iECEC11 = pCalculator->m_iNumOfMotor/2;
	iECEC12 = pCalculator->GetWheelBase() + iWheelDim + 100 + 2 * iBufLen;
	iECEC13 = pCalculator->m_iGirder_w1;
	iECEC15 = _ttol(strRailLen) * 100;
	iECEC16 = iBufLen;
	iECEC18 = iECEC181 + iECEC24;
	iECEC19 = iECEC26 - 15;
	iECEC20 = iECEC21 - 15;
	iECEC25 = pCalculator->m_iGirder_w1 + iECEC35;
	iECEC28 = iStructH + iECEC27;
	iECEC31 = pCalculator->m_iGirder_w1;
	iECEC34 = (_T("S") == pCalculator->m_strECType || _T("D") == pCalculator->m_strECType) ? (2-pCalculator->m_iNumOfMotor/2) : (4-pCalculator->m_iNumOfMotor/2);

	strTemp.Format(_T("%d"), pCalculator->m_iRaiWidTop + 20);		
	lstTextPartner.push_back(StringPartner(_T("(ecec8)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC9);		
	lstTextPartner.push_back(StringPartner(_T("(ecec9)"), strTemp));
	//		
	strTemp.Format(_T("%d"), iECEC10);		
	lstTextPartner.push_back(StringPartner(_T("(ecec10)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC11);		
	lstTextPartner.push_back(StringPartner(_T("(ecec11)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC12);		
	lstTextPartner.push_back(StringPartner(_T("(ecec12)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC13);		
	lstTextPartner.push_back(StringPartner(_T("(ecec13)"), strTemp));
	//
	lstTextPartner.push_back(StringPartner(_T("(ecec14)"), pCalculator->GetBufferType()));
	//
	strTemp.Format(_T("%d"), iECEC15);		
	lstTextPartner.push_back(StringPartner(_T("(ecec15)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC16);		
	lstTextPartner.push_back(StringPartner(_T("(ecec16)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC17);		
	lstTextPartner.push_back(StringPartner(_T("(ecec17)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC18);		
	lstTextPartner.push_back(StringPartner(_T("(ecec18)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC19);		
	lstTextPartner.push_back(StringPartner(_T("(ecec19)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC20);		
	lstTextPartner.push_back(StringPartner(_T("(ecec20)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC21);		
	lstTextPartner.push_back(StringPartner(_T("(ecec21)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC22);		
	lstTextPartner.push_back(StringPartner(_T("(ecec22)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC23);		
	lstTextPartner.push_back(StringPartner(_T("(ecec23)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC24);		
	lstTextPartner.push_back(StringPartner(_T("(ecec24)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC24);		
	lstTextPartner.push_back(StringPartner(_T("(ecec24)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC25);		
	lstTextPartner.push_back(StringPartner(_T("(ecec25)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC26);		
	lstTextPartner.push_back(StringPartner(_T("(ecec26)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC27);		
	lstTextPartner.push_back(StringPartner(_T("(ecec27)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC28);		
	lstTextPartner.push_back(StringPartner(_T("(ecec28)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC29);		
	lstTextPartner.push_back(StringPartner(_T("(ecec29)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC30);		
	lstTextPartner.push_back(StringPartner(_T("(ecec30)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC31);		
	lstTextPartner.push_back(StringPartner(_T("(ecec31)"), strTemp));
	//
	strTemp.Format(_T("%d"), iECEC32);		
	lstTextPartner.push_back(StringPartner(_T("(ecec32)"), strTemp));
	//
	if (fBufExtLen > 0)	strTemp.Format(_T("%.2f"), fBufExtLen);
	else strTemp = _T("不需要");
	lstTextPartner.push_back(StringPartner(_T("(ecec33)"), strTemp));
	//	
	strTemp.Format(_T("%d"), iECEC34);		
	lstTextPartner.push_back(StringPartner(_T("(ecec34)"), strTemp));

	pWordApp->ReplaceTexts(lstTextPartner);

	return TRUE;
}
//----------------------------------------------------