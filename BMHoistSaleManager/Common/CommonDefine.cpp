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
LoginUserInfo	g_LoginUserInfo;
CADODataBase*	g_pSettingDatabase = NULL;
CADODataBase*	g_pOfferDatabase = NULL;
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
void GetValueListInTable(CString strTableName, 
						 CString strColumnName, 
						 eTableDataType eType,
						 list<CString> &lstValueString)
{	
	CADOTable table;
	CString strOperations;
	strOperations.Format(_T("SELECT DISTINCT %s FROM %s"), strColumnName, strTableName);
	if (TRUE == table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOperations))
	{
		CString strValue;
		DWORD dwUserCnt = table.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwUserCnt; dwIndex++)
		{
			switch(eType)
			{
			case eTableDataType_DWORD:
				{
					DWORD dwCellValue = 0;
					table.GetRecordCell(dwIndex, strColumnName, dwCellValue);
					strValue.Format(_T("%d"), dwCellValue);
				}
				break;
			case eTableDataType_FLOAT:
				{
					float fCellValue = 0;
					table.GetRecordCell(dwIndex, strColumnName, fCellValue);
					strValue.Format(_T("%.2f"), fCellValue);
				}
				break;
			case eTableDataType_String:
				{
					WCHAR wcCellValue[256] = {0};
					table.GetRecordCell(dwIndex, strColumnName, wcCellValue, 256);
					strValue = wcCellValue;
				}
				break;
			default:
				break;
			}
			lstValueString.push_back(strValue);
		}
		table.CloseTable();
	}
}
//----------------------------------------------------
long GetNearBigValue(CString strTableName, 
					 CString strColumnName, 
					 long lBaseValue)
{
	long lRes = lBaseValue;

	CADOTable table;
	CString strOperations;
	strOperations.Format(_T("SELECT  %s FROM %s WHERE %s>=%d ORDER BY %s"), strColumnName, strTableName, strColumnName, lBaseValue, strColumnName);
	if (TRUE == table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOperations))
	{
		CString strValue;
		DWORD dwItemCnt = table.GetRecordCnt();
		if (dwItemCnt > 0)
		{
			DWORD dwValue = 0;
			table.GetRecordCell(0, strColumnName, dwValue);
			lRes = dwValue;
		}
	}

	return lRes;
}
//----------------------------------------------------
float GetNearBigValue(CString strTableName, 
					  CString strColumnName, 
					  float fBaseValue)
{
	float fRes = fBaseValue;

	CADOTable table;
	CString strOperations;
	strOperations.Format(_T("SELECT  %s FROM %s WHERE %s >= %f ORDER BY %s"), strColumnName, strTableName, strColumnName, fBaseValue, strColumnName);
	if (TRUE == table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOperations))
	{
		CString strValue;
		DWORD dwItemCnt = table.GetRecordCnt();
		if (dwItemCnt > 0)
		{
			float fValue = 0;
			table.GetRecordCell(0, strColumnName, fValue);
			fRes = fValue;
		}
	}

	return fRes;
}
//----------------------------------------------------
CString GetHoistSelTableName(CString strType, CString strBrand)
{
	CString strHoistTable;
	if (strType == _T("D"))
	{
		if (strBrand == _T("BM") || strBrand == _T("GT"))
			strHoistTable = g_tcTableName[eTableID_HoiSel_D];
		else if (strBrand == _T("NS"))
			strHoistTable = g_tcTableName[eTableID_SHoiSel_D];
		else
			strHoistTable = g_tcTableName[eTableID_SHoiSel_F];
	}
	else if (strType == _T("S"))
	{
		if (strBrand == _T("BM") || strBrand == _T("GT"))
			strHoistTable = g_tcTableName[eTableID_HoiSel_S];
		else
			strHoistTable = g_tcTableName[eTableID_SHoiSel_S];
	}

	return strHoistTable;
}
//----------------------------------------------------



//----------------------------------------------------
CString	GetOfferOrderTemplateFile(CString strSubFolder, 
								  CString strCompany, 
								  CString strLang, 
								  int iComCnt)
{
	const TCHAR cTemplateFile[] = {_T("Templates\\%s\\%s-hoist-%s-template%d.doc")};

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strSubFolder, strCompany, strLang, iComCnt);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	strTemplateFullName.Format(_T("%s%s"), strExtPath, strTemplateFile);
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}

	return strTemplateFullName;
}
//----------------------------------------------------
CString	GetMainGirderTemplateFile(CString strSubFolder, 
								  CString strCompany,
								  CString strLang,
								  CString strMainAux, 
								  CString strECType, 
								  int iMoQty, 
								  CString strCtrlType)
{
	const TCHAR cTemplateFile[] = {_T("Templates\\DrawingTemplates\\%s\\%s%s%s%d%s%s.doc")};

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strSubFolder, strCompany, strMainAux, strECType, iMoQty, strCtrlType, strLang);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	strTemplateFullName.Format(_T("%s%s"), strExtPath, strTemplateFile);
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}

	return strTemplateFullName;
}
//----------------------------------------------------
CString	GetEndCarTemplateFile(CString strSubFolder, 
							  CString strCompany, 
							  CString strLang, 
							  CString strECType, 
							  CString strDriverType, 
							  int iMoQty)
{	
	const TCHAR cTemplateFile[] = {_T("Templates\\EndCarTemplates\\%s\\%s%s%s%d%s.doc")};

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strSubFolder, strCompany, strECType, strDriverType, iMoQty, strLang);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	strTemplateFullName.Format(_T("%s%s"), strExtPath, strTemplateFile);
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}
	return strTemplateFullName;
}
//----------------------------------------------------
CString	GetSteelStructureTemplateFile(CString strSubFolder, 
									  CString strCompany, 
									  CString strLang, 
									  CString strMainGirderType, 
									  CString strJointType, 
									  int iMB81)
{
	const TCHAR cTemplateFile[] = {_T("Templates\\StelStrTemplates\\%s\\%s%s%s%d%s.doc")};

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strSubFolder, strCompany, strMainGirderType, strJointType, iMB81, strLang);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	strTemplateFullName.Format(_T("%s%s"), strExtPath, strTemplateFile);
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}
	return strTemplateFullName;
}
//----------------------------------------------------
BOOL ExportMainGirderDataToWordTemplate(CMSWordApplication* pWordApp, 
										LPVOID pPointer, 
										int iLangIndex)
{
	if (NULL == pWordApp || NULL == pPointer)
		return FALSE;
	CBMParamCalculator* pCalculator = (CBMParamCalculator*)pPointer;
	if (NULL == pCalculator)
		return FALSE;

	pCalculator->ConstructMainGirderDrawingData();

	StringPartnerList lstTextPartner;

	CString strTemp;
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim0);
	lstTextPartner.push_back(StringPartner(_T("(dim0)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim1);
	lstTextPartner.push_back(StringPartner(_T("(dim1)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim2);
	lstTextPartner.push_back(StringPartner(_T("(dim2)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim3);
	lstTextPartner.push_back(StringPartner(_T("(dim3)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim4);
	lstTextPartner.push_back(StringPartner(_T("(dim4)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim5);
	lstTextPartner.push_back(StringPartner(_T("(dim5)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim6);
	lstTextPartner.push_back(StringPartner(_T("(dim6)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim7);
	lstTextPartner.push_back(StringPartner(_T("(dim7)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim8);
	lstTextPartner.push_back(StringPartner(_T("(dim8)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.GB3 - pCalculator->m_CalcDatalst.dim6);
	lstTextPartner.push_back(StringPartner(_T("(dim9)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim10);
	lstTextPartner.push_back(StringPartner(_T("(dim10)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim11);
	lstTextPartner.push_back(StringPartner(_T("(dim11)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim12);
	lstTextPartner.push_back(StringPartner(_T("(dim12)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim13);
	lstTextPartner.push_back(StringPartner(_T("(dim13)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim14);
	lstTextPartner.push_back(StringPartner(_T("(dim14)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim15);
	lstTextPartner.push_back(StringPartner(_T("(dim15)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim16);
	lstTextPartner.push_back(StringPartner(_T("(dim16)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim17);
	lstTextPartner.push_back(StringPartner(_T("(dim17)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim18);
	lstTextPartner.push_back(StringPartner(_T("(dim18)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim19);
	lstTextPartner.push_back(StringPartner(_T("(dim19)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim20);
	lstTextPartner.push_back(StringPartner(_T("(dim20)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim21);
	lstTextPartner.push_back(StringPartner(_T("(dim21)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim22);
	lstTextPartner.push_back(StringPartner(_T("(dim22)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim23);
	lstTextPartner.push_back(StringPartner(_T("(dim23)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim30);
	lstTextPartner.push_back(StringPartner(_T("(dim30)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim31);
	lstTextPartner.push_back(StringPartner(_T("(dim31)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim32);
	lstTextPartner.push_back(StringPartner(_T("(dim32)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim33);
	lstTextPartner.push_back(StringPartner(_T("(dim33)"), strTemp));
	//
	strTemp.Format(_T("%d"), pCalculator->m_CalcDatalst.dim34);
	lstTextPartner.push_back(StringPartner(_T("(dim34)"), strTemp));

	lstTextPartner.push_back(StringPartner(_T("(cralod)"), pCalculator->m_CalcDatalst.cralod));
	lstTextPartner.push_back(StringPartner(_T("(hlod1)"), pCalculator->m_CalcDatalst.hlod1));
	lstTextPartner.push_back(StringPartner(_T("(hlod2)"), pCalculator->m_CalcDatalst.hlod2));
	lstTextPartner.push_back(StringPartner(_T("(span1)"), pCalculator->m_CalcDatalst.span1));
	lstTextPartner.push_back(StringPartner(_T("(span2)"), pCalculator->m_CalcDatalst.span2));
	lstTextPartner.push_back(StringPartner(_T("(hoistcode)"), pCalculator->m_CalcDatalst.hoistcode));
	lstTextPartner.push_back(StringPartner(_T("(maxwheelload)"), pCalculator->m_CalcDatalst.maxwheelload));
	lstTextPartner.push_back(StringPartner(_T("(eccode)"), pCalculator->m_CalcDatalst.eccode));
	lstTextPartner.push_back(StringPartner(_T("(hol)"), pCalculator->m_CalcDatalst.hol));
	lstTextPartner.push_back(StringPartner(_T("(hol1)"), pCalculator->m_CalcDatalst.hol1));
	lstTextPartner.push_back(StringPartner(_T("(hol2)"), pCalculator->m_CalcDatalst.hol2));
	lstTextPartner.push_back(StringPartner(_T("(hol3)"), pCalculator->m_CalcDatalst.hol3));
	lstTextPartner.push_back(StringPartner(_T("(span)"), pCalculator->m_CalcDatalst.span));
	lstTextPartner.push_back(StringPartner(_T("(hoispeed)"), pCalculator->m_CalcDatalst.hoispeed));
	lstTextPartner.push_back(StringPartner(_T("(hoispeed2)"), pCalculator->m_CalcDatalst.hoispeed2));
	lstTextPartner.push_back(StringPartner(_T("(trospeed)"), pCalculator->m_CalcDatalst.trospeed));
	lstTextPartner.push_back(StringPartner(_T("(brispeed)"), pCalculator->m_CalcDatalst.brispeed));
	//	
	if (0 == iLangIndex) strTemp.Format(_T("%s"), (_T("2-SPEED") == pCalculator->m_CalcDatalst.GC4) ? _T("双速") : _T("变频"));
	else strTemp.Format(_T("%s"), (_T("2-SPEED") == pCalculator->m_CalcDatalst.GC4) ? _T("2 Speeds") : _T("Inverter"));
	lstTextPartner.push_back(StringPartner(_T("(hoitype)"), strTemp));
	//
	if (0 == iLangIndex) strTemp.Format(_T("%s"), (_T("2-SPEED") == pCalculator->m_CalcDatalst.GC5) ? _T("双速") : _T("变频"));
	else strTemp.Format(_T("%s"), (_T("2-SPEED") == pCalculator->m_CalcDatalst.GC5) ? _T("2 Speeds") : _T("Inverter"));
	lstTextPartner.push_back(StringPartner(_T("(trotype)"), strTemp));
	//
	if (0 == iLangIndex) strTemp.Format(_T("%s"), (_T("2-SPEED") == pCalculator->m_CalcDatalst.GC6) ? _T("双速") : _T("变频"));
	else strTemp.Format(_T("%s"), (_T("2-SPEED") == pCalculator->m_CalcDatalst.GC6) ? _T("2 Speeds") : _T("Inverter"));
	lstTextPartner.push_back(StringPartner(_T("(britype)"), strTemp));
	//
	lstTextPartner.push_back(StringPartner(_T("(weitro)"), pCalculator->m_CalcDatalst.weitro));
	lstTextPartner.push_back(StringPartner(_T("(weibri)"), pCalculator->m_CalcDatalst.weibri));
	lstTextPartner.push_back(StringPartner(_T("(V1)"), pCalculator->m_CalcDatalst.V1));
	lstTextPartner.push_back(StringPartner(_T("(V2)"), pCalculator->m_CalcDatalst.V2));
	lstTextPartner.push_back(StringPartner(_T("(cragro)"), pCalculator->m_CalcDatalst.cargro));
	lstTextPartner.push_back(StringPartner(_T("(hoigro)"), pCalculator->m_CalcDatalst.hoigro));
	lstTextPartner.push_back(StringPartner(_T("(hoigro2)"), pCalculator->m_CalcDatalst.hoigro2));
	lstTextPartner.push_back(StringPartner(_T("(trogro)"), pCalculator->m_CalcDatalst.trogro));
	lstTextPartner.push_back(StringPartner(_T("(brigro)"), pCalculator->m_CalcDatalst.brigro));
	lstTextPartner.push_back(StringPartner(_T("(suppow)"), pCalculator->m_CalcDatalst.suppow));
	lstTextPartner.push_back(StringPartner(_T("(railen)"), pCalculator->m_CalcDatalst.railen));
	lstTextPartner.push_back(StringPartner(_T("(railtype)"), pCalculator->m_CalcDatalst.railtype));
	lstTextPartner.push_back(StringPartner(_T("(sales)"), g_LoginUserInfo.C30));
	lstTextPartner.push_back(StringPartner(_T("(contype)"), pCalculator->m_CalcDatalst.contype));

	pWordApp->ReplaceTextsInTextBoxes(lstTextPartner);

	return TRUE;
}
//----------------------------------------------------
BOOL ExportEndCarriageDataToWordTemplate(CMSWordApplication* pWordApp, 
										  LPVOID pPointer)
{
	if (NULL == pWordApp || NULL == pPointer)
		return FALSE;
	CBMParamCalculator* pCalculator = (CBMParamCalculator*)pPointer;

	pCalculator->ConstructEndcarDrawingData();

	CString strTemp;
	StringPartnerList lstTextPartner;		
	lstTextPartner.push_back(StringPartner(_T("(ececcode)"), pCalculator->m_CalcDatalst.ececcode));
	lstTextPartner.push_back(StringPartner(_T("(ecec1)"), pCalculator->m_CalcDatalst.ecec1));
	lstTextPartner.push_back(StringPartner(_T("(ecec2)"), pCalculator->m_CalcDatalst.ecec2));
	lstTextPartner.push_back(StringPartner(_T("(ecec3)"), pCalculator->m_CalcDatalst.ecec3));
	lstTextPartner.push_back(StringPartner(_T("(ecec4)"), pCalculator->m_CalcDatalst.ecec4));
	lstTextPartner.push_back(StringPartner(_T("(ecec5)"), pCalculator->m_CalcDatalst.ecec5));
	lstTextPartner.push_back(StringPartner(_T("(ecec6)"), pCalculator->m_CalcDatalst.ecec6));
	lstTextPartner.push_back(StringPartner(_T("(ecec7)"), pCalculator->m_CalcDatalst.ecec7));
	lstTextPartner.push_back(StringPartner(_T("(ecec8)"), pCalculator->m_CalcDatalst.ecec8));
	lstTextPartner.push_back(StringPartner(_T("(ecec9)"), pCalculator->m_CalcDatalst.ecec9));
	lstTextPartner.push_back(StringPartner(_T("(ecec10)"), pCalculator->m_CalcDatalst.ecec10));
	lstTextPartner.push_back(StringPartner(_T("(ecec11)"), pCalculator->m_CalcDatalst.ecec11));
	lstTextPartner.push_back(StringPartner(_T("(ecec12)"), pCalculator->m_CalcDatalst.ecec12));
	lstTextPartner.push_back(StringPartner(_T("(ecec13)"), pCalculator->m_CalcDatalst.ecec13));
	lstTextPartner.push_back(StringPartner(_T("(ecec14)"), pCalculator->m_CalcDatalst.ecec14));
	lstTextPartner.push_back(StringPartner(_T("(ecec15)"), pCalculator->m_CalcDatalst.ecec15));
	lstTextPartner.push_back(StringPartner(_T("(ecec16)"), pCalculator->m_CalcDatalst.ecec16));
	lstTextPartner.push_back(StringPartner(_T("(ecec17)"), pCalculator->m_CalcDatalst.ecec17));
	lstTextPartner.push_back(StringPartner(_T("(ecec18)"), pCalculator->m_CalcDatalst.ecec18));
	lstTextPartner.push_back(StringPartner(_T("(ecec19)"), pCalculator->m_CalcDatalst.ecec19));
	lstTextPartner.push_back(StringPartner(_T("(ecec20)"), pCalculator->m_CalcDatalst.ecec20));
	lstTextPartner.push_back(StringPartner(_T("(ecec21)"), pCalculator->m_CalcDatalst.ecec21));
	lstTextPartner.push_back(StringPartner(_T("(ecec22)"), pCalculator->m_CalcDatalst.ecec22));
	lstTextPartner.push_back(StringPartner(_T("(ecec23)"), pCalculator->m_CalcDatalst.ecec23));
	lstTextPartner.push_back(StringPartner(_T("(ecec24)"), pCalculator->m_CalcDatalst.ecec24));
	lstTextPartner.push_back(StringPartner(_T("(ecec25)"), pCalculator->m_CalcDatalst.ecec25));
	lstTextPartner.push_back(StringPartner(_T("(ecec26)"), pCalculator->m_CalcDatalst.ecec26));
	lstTextPartner.push_back(StringPartner(_T("(ecec27)"), pCalculator->m_CalcDatalst.ecec27));
	lstTextPartner.push_back(StringPartner(_T("(ecec28)"), pCalculator->m_CalcDatalst.ecec28));
	lstTextPartner.push_back(StringPartner(_T("(ecec29)"), pCalculator->m_CalcDatalst.ecec29));
	lstTextPartner.push_back(StringPartner(_T("(ecec30)"), pCalculator->m_CalcDatalst.ecec30));
	lstTextPartner.push_back(StringPartner(_T("(ecec31)"), pCalculator->m_CalcDatalst.ecec31));
	lstTextPartner.push_back(StringPartner(_T("(ecec32)"), pCalculator->m_CalcDatalst.ecec32));
	lstTextPartner.push_back(StringPartner(_T("(ecec33)"), pCalculator->m_CalcDatalst.ecec33));
	lstTextPartner.push_back(StringPartner(_T("(ecec34)"), pCalculator->m_CalcDatalst.ecec34));
	//
	lstTextPartner.push_back(StringPartner(_T("(ecec40)"), pCalculator->m_CalcDatalst.ecec40));
	lstTextPartner.push_back(StringPartner(_T("(ecec41)"), pCalculator->m_CalcDatalst.ecec41));
	lstTextPartner.push_back(StringPartner(_T("(ecec42)"), pCalculator->m_CalcDatalst.ecec42));
	lstTextPartner.push_back(StringPartner(_T("(ecec43)"), pCalculator->m_CalcDatalst.ecec43));


	pWordApp->ReplaceTexts(lstTextPartner);

	return TRUE;
}
//----------------------------------------------------
BOOL ExportSteelStructureDataToWordTemplate(CMSWordApplication* pWordApp, LPVOID pPointer)
{
	if (NULL == pWordApp || NULL == pPointer)
		return FALSE;
	CBMParamCalculator* pCalculator = (CBMParamCalculator*)pPointer;

	pCalculator->ConstructSteelStructDrawingData();

	CString strTemp;
	StringPartnerList lstTextPartner;		
	lstTextPartner.push_back(StringPartner(_T("(ctype)"), pCalculator->m_CalcDatalst.ctype));
	lstTextPartner.push_back(StringPartner(_T("(st01)"), pCalculator->m_CalcDatalst.st01));
	lstTextPartner.push_back(StringPartner(_T("(st02)"), pCalculator->m_CalcDatalst.st02));
	lstTextPartner.push_back(StringPartner(_T("(st03)"), pCalculator->m_CalcDatalst.st03));
	lstTextPartner.push_back(StringPartner(_T("(st04)"), pCalculator->m_CalcDatalst.st04));
	lstTextPartner.push_back(StringPartner(_T("(st05)"), pCalculator->m_CalcDatalst.st05));
	lstTextPartner.push_back(StringPartner(_T("(st06)"), pCalculator->m_CalcDatalst.st06));
	lstTextPartner.push_back(StringPartner(_T("(st07)"), pCalculator->m_CalcDatalst.st07));
	lstTextPartner.push_back(StringPartner(_T("(st08)"), pCalculator->m_CalcDatalst.st08));
	lstTextPartner.push_back(StringPartner(_T("(st09)"), pCalculator->m_CalcDatalst.st09));
	lstTextPartner.push_back(StringPartner(_T("(st10)"), pCalculator->m_CalcDatalst.st10));
	lstTextPartner.push_back(StringPartner(_T("(st11)"), pCalculator->m_CalcDatalst.st11));
	lstTextPartner.push_back(StringPartner(_T("(st12)"), pCalculator->m_CalcDatalst.st12));
	lstTextPartner.push_back(StringPartner(_T("(st13)"), pCalculator->m_CalcDatalst.st13));
	lstTextPartner.push_back(StringPartner(_T("(st14)"), pCalculator->m_CalcDatalst.st14));
	lstTextPartner.push_back(StringPartner(_T("(st15)"), pCalculator->m_CalcDatalst.st15));
	lstTextPartner.push_back(StringPartner(_T("(st16)"), pCalculator->m_CalcDatalst.st16));
	lstTextPartner.push_back(StringPartner(_T("(st17)"), pCalculator->m_CalcDatalst.st17));
	lstTextPartner.push_back(StringPartner(_T("(st18)"), pCalculator->m_CalcDatalst.st18));
	lstTextPartner.push_back(StringPartner(_T("(st19)"), pCalculator->m_CalcDatalst.st19));
	lstTextPartner.push_back(StringPartner(_T("(st20)"), pCalculator->m_CalcDatalst.st20));
	lstTextPartner.push_back(StringPartner(_T("(st21)"), pCalculator->m_CalcDatalst.st21));
	lstTextPartner.push_back(StringPartner(_T("(st22)"), pCalculator->m_CalcDatalst.st22));
	lstTextPartner.push_back(StringPartner(_T("(st23)"), pCalculator->m_CalcDatalst.st23));
	lstTextPartner.push_back(StringPartner(_T("(st24)"), pCalculator->m_CalcDatalst.st24));
	lstTextPartner.push_back(StringPartner(_T("(st25)"), pCalculator->m_CalcDatalst.st25));
	lstTextPartner.push_back(StringPartner(_T("(st26)"), pCalculator->m_CalcDatalst.st26));
	lstTextPartner.push_back(StringPartner(_T("(st27)"), pCalculator->m_CalcDatalst.st27));
	lstTextPartner.push_back(StringPartner(_T("(st28)"), pCalculator->m_CalcDatalst.st28));
	lstTextPartner.push_back(StringPartner(_T("(st29)"), pCalculator->m_CalcDatalst.st29));
	lstTextPartner.push_back(StringPartner(_T("(st30)"), pCalculator->m_CalcDatalst.st30));
	lstTextPartner.push_back(StringPartner(_T("(st31)"), pCalculator->m_CalcDatalst.st31));
	lstTextPartner.push_back(StringPartner(_T("(st32)"), pCalculator->m_CalcDatalst.st32));
	lstTextPartner.push_back(StringPartner(_T("(st33)"), pCalculator->m_CalcDatalst.st33));
	lstTextPartner.push_back(StringPartner(_T("(st34)"), pCalculator->m_CalcDatalst.st34));
	lstTextPartner.push_back(StringPartner(_T("(st35)"), pCalculator->m_CalcDatalst.st35));
	lstTextPartner.push_back(StringPartner(_T("(st36)"), pCalculator->m_CalcDatalst.st36));
	lstTextPartner.push_back(StringPartner(_T("(st37)"), pCalculator->m_CalcDatalst.st37));
	lstTextPartner.push_back(StringPartner(_T("(st38)"), pCalculator->m_CalcDatalst.st38));

	pWordApp->ReplaceTexts(lstTextPartner);

	return TRUE;
}
//----------------------------------------------------



//----------------------------------------------------
CString	ValueToString(CString value)
{
	CString strResult = value;

	return strResult;
}
//----------------------------------------------------
CString	ValueToString(long value)
{
	CString strResult;
	strResult.Format(_T("%d"), value);

	return strResult;
}
//----------------------------------------------------
CString	ValueToString(float value)
{
	CString strResult;
	strResult.Format(_T("%.2f"), value);

	return strResult;
}
//----------------------------------------------------
CString	ValueToString(LONGLONG value)
{
	CString strResult;
	strResult.Format(_T("%d"), value);

	return strResult;
}
//----------------------------------------------------
CString	GetMainGirderCalcDataTemplateFile(CString strLang, CString strMainGirderType)
{	
	const TCHAR cTemplateFile[] = {_T("Templates\\CalcDataTemplates\\%s-%s-template.doc")};

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strMainGirderType, strLang);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	strTemplateFullName.Format(_T("%s%s"), strExtPath, strTemplateFile);
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}
	return strTemplateFullName;
}
//----------------------------------------------------
CString	GetEndCarriageCalcDataTemplateFile(CString strLang, CString strECType)
{
	const TCHAR cTemplateFile[] = {_T("Templates\\CalcDataTemplates\\EC%s-%s-template.doc")};

	CString strTemplateFile;
	strTemplateFile.Format(cTemplateFile, strECType, strLang);

	CString strExtPath;
	TCHAR tcExePath[MAX_PATH] = {_T("\0")};
	GetModuleFileName(NULL, tcExePath, MAX_PATH);
	strExtPath = tcExePath;
	strExtPath = strExtPath.Left(strExtPath.ReverseFind(_T('\\')) + 1);
	//
	CString strTemplateFullName;
	strTemplateFullName.Format(_T("%s%s"), strExtPath, strTemplateFile);
	if (!PathFileExists(strTemplateFullName))
	{
		strTemplateFullName = _T("\0");
	}
	return strTemplateFullName;
}
//----------------------------------------------------
BOOL ExportMainGirderCalcDataForTest(CMSWordApplication* pWordApp, LPVOID pPointer)
{
	if (NULL == pWordApp || NULL == pPointer)
		return FALSE;
	CBMParamCalculator* pCalculator = (CBMParamCalculator*)pPointer;

	pCalculator->ConstructMainGirderDrawingData();
	pCalculator->ConstructSteelStructDrawingData();

	CString strTemp;
	StringPartnerList lstTextPartner;	
	lstTextPartner.push_back(StringPartner(_T("(equicode)"), ValueToString(pCalculator->m_CalcDatalst.equicode)));
	lstTextPartner.push_back(StringPartner(_T("(GA3)"), ValueToString(pCalculator->m_CalcDatalst.GA3)));
	lstTextPartner.push_back(StringPartner(_T("(GA4)"), ValueToString(pCalculator->m_CalcDatalst.GA4)));
	lstTextPartner.push_back(StringPartner(_T("(st01)"), ValueToString(pCalculator->m_CalcDatalst.st01)));
	lstTextPartner.push_back(StringPartner(_T("(st02)"), ValueToString(pCalculator->m_CalcDatalst.st02)));
	lstTextPartner.push_back(StringPartner(_T("(st03)"), ValueToString(pCalculator->m_CalcDatalst.st03)));
	lstTextPartner.push_back(StringPartner(_T("(GA6)"), ValueToString(pCalculator->m_CalcDatalst.GA6)));
	lstTextPartner.push_back(StringPartner(_T("(T07)"), ValueToString(pCalculator->m_CalcDatalst.T07)));
	lstTextPartner.push_back(StringPartner(_T("(T12)"), ValueToString(pCalculator->m_CalcDatalst.T12)));
	lstTextPartner.push_back(StringPartner(_T("(T47)"), ValueToString(pCalculator->m_CalcDatalst.T47)));
	lstTextPartner.push_back(StringPartner(_T("(st11)"), ValueToString(pCalculator->m_CalcDatalst.st11)));
	lstTextPartner.push_back(StringPartner(_T("(st15)"), ValueToString(pCalculator->m_CalcDatalst.st15)));
	lstTextPartner.push_back(StringPartner(_T("(st26)"), ValueToString(pCalculator->m_CalcDatalst.st26)));
	lstTextPartner.push_back(StringPartner(_T("(st35)"), ValueToString(pCalculator->m_CalcDatalst.st35)));
	lstTextPartner.push_back(StringPartner(_T("(st17)"), ValueToString(pCalculator->m_CalcDatalst.st17)));
	lstTextPartner.push_back(StringPartner(_T("(st27)"), ValueToString(pCalculator->m_CalcDatalst.st27)));
	lstTextPartner.push_back(StringPartner(_T("(st33)"), ValueToString(pCalculator->m_CalcDatalst.st33)));
	//
	lstTextPartner.push_back(StringPartner(_T("(GA5)"), ValueToString(pCalculator->m_CalcDatalst.GA5)));
	lstTextPartner.push_back(StringPartner(_T("(P05)"), ValueToString(pCalculator->m_CalcDatalst.P05)));
	lstTextPartner.push_back(StringPartner(_T("(P06)"), ValueToString(pCalculator->m_CalcDatalst.P06)));
	lstTextPartner.push_back(StringPartner(_T("(P07)"), ValueToString(pCalculator->m_CalcDatalst.P07)));
	lstTextPartner.push_back(StringPartner(_T("(P08)"), ValueToString(pCalculator->m_CalcDatalst.P08)));
	lstTextPartner.push_back(StringPartner(_T("(P11)"), ValueToString(pCalculator->m_CalcDatalst.P11)));
	lstTextPartner.push_back(StringPartner(_T("(P12)"), ValueToString(pCalculator->m_CalcDatalst.P12)));
	lstTextPartner.push_back(StringPartner(_T("(P13)"), ValueToString(pCalculator->m_CalcDatalst.P13)));
	lstTextPartner.push_back(StringPartner(_T("(P14)"), ValueToString(pCalculator->m_CalcDatalst.P14)));
	lstTextPartner.push_back(StringPartner(_T("(P15)"), ValueToString(pCalculator->m_CalcDatalst.P15)));
	lstTextPartner.push_back(StringPartner(_T("(P16)"), ValueToString(pCalculator->m_CalcDatalst.P16)));
	lstTextPartner.push_back(StringPartner(_T("(P17)"), ValueToString(pCalculator->m_CalcDatalst.P17)));
	lstTextPartner.push_back(StringPartner(_T("(P18)"), ValueToString(pCalculator->m_CalcDatalst.P18)));
	lstTextPartner.push_back(StringPartner(_T("(P19)"), ValueToString(pCalculator->m_CalcDatalst.P19)));
	lstTextPartner.push_back(StringPartner(_T("(P20)"), ValueToString(pCalculator->m_CalcDatalst.P20)));
	lstTextPartner.push_back(StringPartner(_T("(P21)"), ValueToString(pCalculator->m_CalcDatalst.P21)));
	lstTextPartner.push_back(StringPartner(_T("(P22)"), ValueToString(pCalculator->m_CalcDatalst.P22)));
	lstTextPartner.push_back(StringPartner(_T("(HD01)"), ValueToString(pCalculator->m_CalcDatalst.HD01)));
	lstTextPartner.push_back(StringPartner(_T("(HD08)"), ValueToString(pCalculator->m_CalcDatalst.HD08)));
	lstTextPartner.push_back(StringPartner(_T("(HD09)"), ValueToString(pCalculator->m_CalcDatalst.HD09)));
	lstTextPartner.push_back(StringPartner(_T("(HB220)"), ValueToString(pCalculator->m_CalcDatalst.HB220)));
	lstTextPartner.push_back(StringPartner(_T("(GA2)"), ValueToString((long)pCalculator->m_CalcDatalst.GA2)));
	lstTextPartner.push_back(StringPartner(_T("(GA3)"), ValueToString(pCalculator->m_CalcDatalst.GA3)));
	lstTextPartner.push_back(StringPartner(_T("(GC3)"), ValueToString(pCalculator->m_CalcDatalst.GC3)));
	lstTextPartner.push_back(StringPartner(_T("(GC31)"), ValueToString((long)pCalculator->m_CalcDatalst.GC31)));
	lstTextPartner.push_back(StringPartner(_T("(T41)"), ValueToString(pCalculator->m_CalcDatalst.T41)));
	lstTextPartner.push_back(StringPartner(_T("(HD10)"), ValueToString(pCalculator->m_CalcDatalst.HD10)));
	//
	lstTextPartner.push_back(StringPartner(_T("(MA11)"), ValueToString(pCalculator->m_CalcDatalst.MA11)));
	lstTextPartner.push_back(StringPartner(_T("(MA2)"), ValueToString(pCalculator->m_CalcDatalst.MA2)));
	lstTextPartner.push_back(StringPartner(_T("(MA3)"), ValueToString(pCalculator->m_CalcDatalst.MA3)));
	lstTextPartner.push_back(StringPartner(_T("(MA4)"), ValueToString(pCalculator->m_CalcDatalst.MA4)));
	lstTextPartner.push_back(StringPartner(_T("(MA5)"), ValueToString(pCalculator->m_CalcDatalst.MA5)));
	lstTextPartner.push_back(StringPartner(_T("(MA6)"), ValueToString(pCalculator->m_CalcDatalst.MA6)));
	lstTextPartner.push_back(StringPartner(_T("(MA7)"), ValueToString(pCalculator->m_CalcDatalst.MA7)));
	lstTextPartner.push_back(StringPartner(_T("(MB1)"), ValueToString(pCalculator->m_CalcDatalst.MB1)));
	lstTextPartner.push_back(StringPartner(_T("(MB2)"), ValueToString(pCalculator->m_CalcDatalst.MB2)));
	lstTextPartner.push_back(StringPartner(_T("(MB21)"), ValueToString(pCalculator->m_CalcDatalst.MB21)));
	lstTextPartner.push_back(StringPartner(_T("(MB3)"), ValueToString(pCalculator->m_CalcDatalst.MB3)));
	lstTextPartner.push_back(StringPartner(_T("(MB31)"), ValueToString(pCalculator->m_CalcDatalst.MB31)));
	lstTextPartner.push_back(StringPartner(_T("(MB32)"), ValueToString(pCalculator->m_CalcDatalst.MB32)));
	lstTextPartner.push_back(StringPartner(_T("(MB33)"), ValueToString(pCalculator->m_CalcDatalst.MB33)));
	lstTextPartner.push_back(StringPartner(_T("(MB34)"), ValueToString(pCalculator->m_CalcDatalst.MB34)));
	lstTextPartner.push_back(StringPartner(_T("(MB35)"), ValueToString(pCalculator->m_CalcDatalst.MB35)));
	lstTextPartner.push_back(StringPartner(_T("(MB36)"), ValueToString(pCalculator->m_CalcDatalst.MB36)));
	lstTextPartner.push_back(StringPartner(_T("(MB4)"), ValueToString(pCalculator->m_CalcDatalst.MB4)));
	lstTextPartner.push_back(StringPartner(_T("(MB5)"), ValueToString(pCalculator->m_CalcDatalst.MB5)));
	lstTextPartner.push_back(StringPartner(_T("(MB51)"), ValueToString(pCalculator->m_CalcDatalst.MB51)));
	lstTextPartner.push_back(StringPartner(_T("(MB6)"), ValueToString(pCalculator->m_CalcDatalst.MB6)));
	lstTextPartner.push_back(StringPartner(_T("(MB7)"), ValueToString(pCalculator->m_CalcDatalst.MB7)));
	lstTextPartner.push_back(StringPartner(_T("(MB71)"), ValueToString(pCalculator->m_CalcDatalst.MB71)));
	lstTextPartner.push_back(StringPartner(_T("(MB72)"), ValueToString(pCalculator->m_CalcDatalst.MB72)));
	lstTextPartner.push_back(StringPartner(_T("(MB73)"), ValueToString(pCalculator->m_CalcDatalst.MB73)));
	lstTextPartner.push_back(StringPartner(_T("(MB74)"), ValueToString(pCalculator->m_CalcDatalst.MB74)));
	lstTextPartner.push_back(StringPartner(_T("(MB81)"), ValueToString(pCalculator->m_CalcDatalst.MB81)));
	lstTextPartner.push_back(StringPartner(_T("(M01)"), ValueToString(pCalculator->m_CalcDatalst.M01)));
	lstTextPartner.push_back(StringPartner(_T("(M011)"), ValueToString(pCalculator->m_CalcDatalst.M011)));
	lstTextPartner.push_back(StringPartner(_T("(M012)"), ValueToString(pCalculator->m_CalcDatalst.M012)));
	lstTextPartner.push_back(StringPartner(_T("(M013)"), ValueToString(pCalculator->m_CalcDatalst.M013)));
	lstTextPartner.push_back(StringPartner(_T("(M014)"), ValueToString(pCalculator->m_CalcDatalst.M014)));
	lstTextPartner.push_back(StringPartner(_T("(M02)"), ValueToString(pCalculator->m_CalcDatalst.M02)));
	lstTextPartner.push_back(StringPartner(_T("(M021)"), ValueToString(pCalculator->m_CalcDatalst.M021)));
	lstTextPartner.push_back(StringPartner(_T("(M022)"), ValueToString(pCalculator->m_CalcDatalst.M022)));
	lstTextPartner.push_back(StringPartner(_T("(M023)"), ValueToString(pCalculator->m_CalcDatalst.M023)));
	lstTextPartner.push_back(StringPartner(_T("(M024)"), ValueToString(pCalculator->m_CalcDatalst.M024)));
	lstTextPartner.push_back(StringPartner(_T("(M03)"), ValueToString(pCalculator->m_CalcDatalst.M03)));
	lstTextPartner.push_back(StringPartner(_T("(M031)"), ValueToString(pCalculator->m_CalcDatalst.M031)));
	lstTextPartner.push_back(StringPartner(_T("(M032)"), ValueToString(pCalculator->m_CalcDatalst.M032)));
	lstTextPartner.push_back(StringPartner(_T("(M033)"), ValueToString(pCalculator->m_CalcDatalst.M033)));
	lstTextPartner.push_back(StringPartner(_T("(M034)"), ValueToString(pCalculator->m_CalcDatalst.M034)));
	lstTextPartner.push_back(StringPartner(_T("(M04)"), ValueToString(pCalculator->m_CalcDatalst.M04)));
	lstTextPartner.push_back(StringPartner(_T("(M041)"), ValueToString(pCalculator->m_CalcDatalst.M041)));
	lstTextPartner.push_back(StringPartner(_T("(M042)"), ValueToString(pCalculator->m_CalcDatalst.M042)));
	lstTextPartner.push_back(StringPartner(_T("(M043)"), ValueToString(pCalculator->m_CalcDatalst.M043)));
	lstTextPartner.push_back(StringPartner(_T("(M044)"), ValueToString(pCalculator->m_CalcDatalst.M044)));
	lstTextPartner.push_back(StringPartner(_T("(M05)"), ValueToString(pCalculator->m_CalcDatalst.M05)));
	lstTextPartner.push_back(StringPartner(_T("(M051)"), ValueToString(pCalculator->m_CalcDatalst.M051)));
	lstTextPartner.push_back(StringPartner(_T("(M052)"), ValueToString(pCalculator->m_CalcDatalst.M052)));
	lstTextPartner.push_back(StringPartner(_T("(M053)"), ValueToString(pCalculator->m_CalcDatalst.M053)));
	lstTextPartner.push_back(StringPartner(_T("(M054)"), ValueToString(pCalculator->m_CalcDatalst.M054)));
	lstTextPartner.push_back(StringPartner(_T("(M06)"), ValueToString(pCalculator->m_CalcDatalst.M06)));
	lstTextPartner.push_back(StringPartner(_T("(M07)"), ValueToString(pCalculator->m_CalcDatalst.M07)));
	lstTextPartner.push_back(StringPartner(_T("(M08)"), ValueToString(pCalculator->m_CalcDatalst.M08)));
	lstTextPartner.push_back(StringPartner(_T("(M09)"), ValueToString(pCalculator->m_CalcDatalst.M09)));
	lstTextPartner.push_back(StringPartner(_T("(M10)"), ValueToString(pCalculator->m_CalcDatalst.M10)));
	lstTextPartner.push_back(StringPartner(_T("(M101)"), ValueToString(pCalculator->m_CalcDatalst.M101)));
	lstTextPartner.push_back(StringPartner(_T("(M1010)"), ValueToString(pCalculator->m_CalcDatalst.M1010)));
	lstTextPartner.push_back(StringPartner(_T("(M102)"), ValueToString(pCalculator->m_CalcDatalst.M102)));
	lstTextPartner.push_back(StringPartner(_T("(M103)"), ValueToString(pCalculator->m_CalcDatalst.M103)));
	lstTextPartner.push_back(StringPartner(_T("(M104)"), ValueToString(pCalculator->m_CalcDatalst.M104)));
	lstTextPartner.push_back(StringPartner(_T("(M105)"), ValueToString(pCalculator->m_CalcDatalst.M105)));
	lstTextPartner.push_back(StringPartner(_T("(M106)"), ValueToString(pCalculator->m_CalcDatalst.M106)));
	lstTextPartner.push_back(StringPartner(_T("(M107)"), ValueToString(pCalculator->m_CalcDatalst.M107)));
	lstTextPartner.push_back(StringPartner(_T("(M108)"), ValueToString(pCalculator->m_CalcDatalst.M108)));
	lstTextPartner.push_back(StringPartner(_T("(M109)"), ValueToString(pCalculator->m_CalcDatalst.M109)));
	lstTextPartner.push_back(StringPartner(_T("(M101)"), ValueToString(pCalculator->m_CalcDatalst.M101)));
	lstTextPartner.push_back(StringPartner(_T("(M11)"), ValueToString(pCalculator->m_CalcDatalst.M11)));
	lstTextPartner.push_back(StringPartner(_T("(M111)"), ValueToString(pCalculator->m_CalcDatalst.M111)));
	lstTextPartner.push_back(StringPartner(_T("(M112)"), ValueToString(pCalculator->m_CalcDatalst.M112)));
	lstTextPartner.push_back(StringPartner(_T("(M113)"), ValueToString(pCalculator->m_CalcDatalst.M113)));
	lstTextPartner.push_back(StringPartner(_T("(M12)"), ValueToString(pCalculator->m_CalcDatalst.M12)));
	lstTextPartner.push_back(StringPartner(_T("(M13)"), ValueToString(pCalculator->m_CalcDatalst.M13)));
	lstTextPartner.push_back(StringPartner(_T("(M14)"), ValueToString(pCalculator->m_CalcDatalst.M14)));
	lstTextPartner.push_back(StringPartner(_T("(M141)"), ValueToString(pCalculator->m_CalcDatalst.M141)));
	lstTextPartner.push_back(StringPartner(_T("(M142)"), ValueToString(pCalculator->m_CalcDatalst.M142)));
	lstTextPartner.push_back(StringPartner(_T("(M143)"), ValueToString(pCalculator->m_CalcDatalst.M143)));
	lstTextPartner.push_back(StringPartner(_T("(M15)"), ValueToString(pCalculator->m_CalcDatalst.M15)));
	lstTextPartner.push_back(StringPartner(_T("(M151)"), ValueToString(pCalculator->m_CalcDatalst.M151)));
	lstTextPartner.push_back(StringPartner(_T("(M152)"), ValueToString(pCalculator->m_CalcDatalst.M152)));
	lstTextPartner.push_back(StringPartner(_T("(M153)"), ValueToString(pCalculator->m_CalcDatalst.M153)));
	lstTextPartner.push_back(StringPartner(_T("(M16)"), ValueToString(pCalculator->m_CalcDatalst.M16)));
	lstTextPartner.push_back(StringPartner(_T("(M161)"), ValueToString(pCalculator->m_CalcDatalst.M161)));
	lstTextPartner.push_back(StringPartner(_T("(M162)"), ValueToString(pCalculator->m_CalcDatalst.M162)));
	lstTextPartner.push_back(StringPartner(_T("(M163)"), ValueToString(pCalculator->m_CalcDatalst.M163)));
	lstTextPartner.push_back(StringPartner(_T("(M17)"), ValueToString(pCalculator->m_CalcDatalst.M17)));
	lstTextPartner.push_back(StringPartner(_T("(M18)"), ValueToString(pCalculator->m_CalcDatalst.M18)));
	lstTextPartner.push_back(StringPartner(_T("(M19)"), ValueToString(pCalculator->m_CalcDatalst.M19)));
	lstTextPartner.push_back(StringPartner(_T("(M20)"), ValueToString(pCalculator->m_CalcDatalst.M20)));
	lstTextPartner.push_back(StringPartner(_T("(M21)"), ValueToString(pCalculator->m_CalcDatalst.M21)));
	lstTextPartner.push_back(StringPartner(_T("(M22)"), ValueToString(pCalculator->m_CalcDatalst.M22)));
	lstTextPartner.push_back(StringPartner(_T("(M23)"), ValueToString(pCalculator->m_CalcDatalst.M23)));
	lstTextPartner.push_back(StringPartner(_T("(M24)"), ValueToString(pCalculator->m_CalcDatalst.M24)));
	lstTextPartner.push_back(StringPartner(_T("(M241)"), ValueToString(pCalculator->m_CalcDatalst.M241)));
	lstTextPartner.push_back(StringPartner(_T("(M25)"), ValueToString(pCalculator->m_CalcDatalst.M25)));
	lstTextPartner.push_back(StringPartner(_T("(M26)"), ValueToString(pCalculator->m_CalcDatalst.M26)));
	lstTextPartner.push_back(StringPartner(_T("(M27)"), ValueToString(pCalculator->m_CalcDatalst.M27)));
	lstTextPartner.push_back(StringPartner(_T("(M271)"), ValueToString(pCalculator->m_CalcDatalst.M271)));
	lstTextPartner.push_back(StringPartner(_T("(M272)"), ValueToString(pCalculator->m_CalcDatalst.M272)));
	lstTextPartner.push_back(StringPartner(_T("(M273)"), ValueToString(pCalculator->m_CalcDatalst.M273)));
	lstTextPartner.push_back(StringPartner(_T("(M281)"), ValueToString(pCalculator->m_CalcDatalst.M281)));
	lstTextPartner.push_back(StringPartner(_T("(M282)"), ValueToString(pCalculator->m_CalcDatalst.M282)));
	lstTextPartner.push_back(StringPartner(_T("(M283)"), ValueToString(pCalculator->m_CalcDatalst.M283)));
	lstTextPartner.push_back(StringPartner(_T("(M284)"), ValueToString(pCalculator->m_CalcDatalst.M284)));
	lstTextPartner.push_back(StringPartner(_T("(M285)"), ValueToString(pCalculator->m_CalcDatalst.M285)));
	lstTextPartner.push_back(StringPartner(_T("(M286)"), ValueToString(pCalculator->m_CalcDatalst.M286)));
	lstTextPartner.push_back(StringPartner(_T("(M291)"), ValueToString(pCalculator->m_CalcDatalst.M291)));
	lstTextPartner.push_back(StringPartner(_T("(M292)"), ValueToString(pCalculator->m_CalcDatalst.M292)));
	lstTextPartner.push_back(StringPartner(_T("(M293)"), ValueToString(pCalculator->m_CalcDatalst.M293)));
	lstTextPartner.push_back(StringPartner(_T("(M294)"), ValueToString(pCalculator->m_CalcDatalst.M294)));
	lstTextPartner.push_back(StringPartner(_T("(M295)"), ValueToString(pCalculator->m_CalcDatalst.M295)));
	lstTextPartner.push_back(StringPartner(_T("(M30)"), ValueToString(pCalculator->m_CalcDatalst.M30)));
	lstTextPartner.push_back(StringPartner(_T("(M31)"), ValueToString(pCalculator->m_CalcDatalst.M31)));
	lstTextPartner.push_back(StringPartner(_T("(M32)"), ValueToString(pCalculator->m_CalcDatalst.M32)));
	lstTextPartner.push_back(StringPartner(_T("(M331)"), ValueToString(pCalculator->m_CalcDatalst.M331)));
	lstTextPartner.push_back(StringPartner(_T("(M332)"), ValueToString(pCalculator->m_CalcDatalst.M332)));
	lstTextPartner.push_back(StringPartner(_T("(M333)"), ValueToString(pCalculator->m_CalcDatalst.M333)));
	lstTextPartner.push_back(StringPartner(_T("(M334)"), ValueToString(pCalculator->m_CalcDatalst.M334)));
	lstTextPartner.push_back(StringPartner(_T("(MC1)"), ValueToString(pCalculator->m_CalcDatalst.MC1)));
	lstTextPartner.push_back(StringPartner(_T("(MC2)"), ValueToString(pCalculator->m_CalcDatalst.MC2)));
	lstTextPartner.push_back(StringPartner(_T("(MC3)"), ValueToString(pCalculator->m_CalcDatalst.MC3)));
	lstTextPartner.push_back(StringPartner(_T("(MC4)"), ValueToString(pCalculator->m_CalcDatalst.MC4)));
	lstTextPartner.push_back(StringPartner(_T("(MC5)"), ValueToString(pCalculator->m_CalcDatalst.MC5)));
	lstTextPartner.push_back(StringPartner(_T("(MC6)"), ValueToString(pCalculator->m_CalcDatalst.MC6)));
	lstTextPartner.push_back(StringPartner(_T("(MD1)"), ValueToString(pCalculator->m_CalcDatalst.MD1)));
	lstTextPartner.push_back(StringPartner(_T("(MD2)"), ValueToString(pCalculator->m_CalcDatalst.MD2)));
	lstTextPartner.push_back(StringPartner(_T("(MD3)"), ValueToString(pCalculator->m_CalcDatalst.MD3)));
	lstTextPartner.push_back(StringPartner(_T("(MD4)"), ValueToString(pCalculator->m_CalcDatalst.MD4)));
	lstTextPartner.push_back(StringPartner(_T("(MD5)"), ValueToString(pCalculator->m_CalcDatalst.MD5)));
	lstTextPartner.push_back(StringPartner(_T("(MD6)"), ValueToString(pCalculator->m_CalcDatalst.MD6)));
	lstTextPartner.push_back(StringPartner(_T("(ME1)"), ValueToString(pCalculator->m_CalcDatalst.ME1)));
	lstTextPartner.push_back(StringPartner(_T("(ME2)"), ValueToString(pCalculator->m_CalcDatalst.ME2)));
	lstTextPartner.push_back(StringPartner(_T("(ME3)"), ValueToString(pCalculator->m_CalcDatalst.ME3)));
	lstTextPartner.push_back(StringPartner(_T("(ME4)"), ValueToString(pCalculator->m_CalcDatalst.ME4)));
	lstTextPartner.push_back(StringPartner(_T("(ME5)"), ValueToString(pCalculator->m_CalcDatalst.ME5)));
	lstTextPartner.push_back(StringPartner(_T("(ME6)"), ValueToString(pCalculator->m_CalcDatalst.ME6)));
	lstTextPartner.push_back(StringPartner(_T("(ME7)"), ValueToString(pCalculator->m_CalcDatalst.ME7)));
	lstTextPartner.push_back(StringPartner(_T("(ME8)"), ValueToString(pCalculator->m_CalcDatalst.ME8)));
	lstTextPartner.push_back(StringPartner(_T("(ME9)"), ValueToString(pCalculator->m_CalcDatalst.ME9)));
	lstTextPartner.push_back(StringPartner(_T("(MF1)"), ValueToString(pCalculator->m_CalcDatalst.MF1)));
	pWordApp->ReplaceTexts(lstTextPartner);

	return TRUE;
}
//----------------------------------------------------
BOOL ExportEndCarriageCalcDataForTest(CMSWordApplication* pWordApp, LPVOID pPointer)
{
	if (NULL == pWordApp || NULL == pPointer)
		return FALSE;
	CBMParamCalculator* pCalculator = (CBMParamCalculator*)pPointer;

	pCalculator->ConstructEndcarDrawingData();
	pCalculator->ConstructSteelStructDrawingData();
	//
	pCalculator->m_CalcDatalst.AA01 = pCalculator->m_CalcDatalst.DB61 / pCalculator->m_CalcDatalst.P14;
	pCalculator->m_CalcDatalst.AA02 = pCalculator->m_CalcDatalst.DB81 * pCalculator->m_CalcDatalst.P091;
	pCalculator->m_CalcDatalst.AA03 = (pCalculator->m_CalcDatalst.DC12 + pCalculator->m_CalcDatalst.HD01 + pCalculator->m_CalcDatalst.GA4)/1000.0f;
	pCalculator->m_CalcDatalst.AA04 = pCalculator->m_CalcDatalst.DA41 + pCalculator->m_CalcDatalst.P23;
	pCalculator->m_CalcDatalst.AA05 = pCalculator->m_CalcDatalst.GA5 * pCalculator->m_CalcDatalst.P21 * 1000;
	//
	CString strTemp;
	StringPartnerList lstTextPartner;	
	lstTextPartner.push_back(StringPartner(_T("(equicode)"), ValueToString(pCalculator->m_CalcDatalst.equicode)));
	lstTextPartner.push_back(StringPartner(_T("(GA3)"), ValueToString(pCalculator->m_CalcDatalst.GA3)));
	lstTextPartner.push_back(StringPartner(_T("(GA4)"), ValueToString(pCalculator->m_CalcDatalst.GA4)));
	lstTextPartner.push_back(StringPartner(_T("(st01)"), ValueToString(pCalculator->m_CalcDatalst.st01)));
	lstTextPartner.push_back(StringPartner(_T("(st02)"), ValueToString(pCalculator->m_CalcDatalst.st02)));
	lstTextPartner.push_back(StringPartner(_T("(st03)"), ValueToString(pCalculator->m_CalcDatalst.st03)));
	lstTextPartner.push_back(StringPartner(_T("(GA6)"), ValueToString(pCalculator->m_CalcDatalst.GA6)));
	lstTextPartner.push_back(StringPartner(_T("(T07)"), ValueToString(pCalculator->m_CalcDatalst.T07)));
	lstTextPartner.push_back(StringPartner(_T("(T12)"), ValueToString(pCalculator->m_CalcDatalst.T12)));
	lstTextPartner.push_back(StringPartner(_T("(T47)"), ValueToString(pCalculator->m_CalcDatalst.T47)));
	lstTextPartner.push_back(StringPartner(_T("(ecec10)"), ValueToString(pCalculator->m_CalcDatalst.ecec10)));
	lstTextPartner.push_back(StringPartner(_T("(ecec1)"), ValueToString(pCalculator->m_CalcDatalst.ecec1)));
	lstTextPartner.push_back(StringPartner(_T("(ecec6)"), ValueToString(pCalculator->m_CalcDatalst.ecec6)));
	lstTextPartner.push_back(StringPartner(_T("(ecec7)"), ValueToString(pCalculator->m_CalcDatalst.ecec7)));
	lstTextPartner.push_back(StringPartner(_T("(ecec2)"), ValueToString(pCalculator->m_CalcDatalst.ecec2)));
	lstTextPartner.push_back(StringPartner(_T("(ecec11)"), ValueToString(pCalculator->m_CalcDatalst.ecec11)));
	lstTextPartner.push_back(StringPartner(_T("(ecec12)"), ValueToString(pCalculator->m_CalcDatalst.ecec12)));
	lstTextPartner.push_back(StringPartner(_T("(ecec3)"), ValueToString(pCalculator->m_CalcDatalst.ecec3)));
	lstTextPartner.push_back(StringPartner(_T("(ecec8)"), ValueToString(pCalculator->m_CalcDatalst.ecec8)));
	lstTextPartner.push_back(StringPartner(_T("(ecec13)"), ValueToString(pCalculator->m_CalcDatalst.ecec13)));
	lstTextPartner.push_back(StringPartner(_T("(ecec4)"), ValueToString(pCalculator->m_CalcDatalst.ecec4)));
	lstTextPartner.push_back(StringPartner(_T("(ecec14)"), ValueToString(pCalculator->m_CalcDatalst.ecec14)));
	lstTextPartner.push_back(StringPartner(_T("(ecec33)"), ValueToString(pCalculator->m_CalcDatalst.ecec33)));
	lstTextPartner.push_back(StringPartner(_T("(ecec15)"), ValueToString(pCalculator->m_CalcDatalst.ecec15)));
	//
	lstTextPartner.push_back(StringPartner(_T("(MA11)"), ValueToString(pCalculator->m_CalcDatalst.MA11)));
	lstTextPartner.push_back(StringPartner(_T("(ME5)"), ValueToString(pCalculator->m_CalcDatalst.ME5)));
	lstTextPartner.push_back(StringPartner(_T("(HD01)"), ValueToString(pCalculator->m_CalcDatalst.HD01)));
	lstTextPartner.push_back(StringPartner(_T("(GA2)"), ValueToString((long)pCalculator->m_CalcDatalst.GA2)));
	lstTextPartner.push_back(StringPartner(_T("(GA3)"), ValueToString(pCalculator->m_CalcDatalst.GA3)));
	lstTextPartner.push_back(StringPartner(_T("(GA5)"), ValueToString(pCalculator->m_CalcDatalst.GA5)));
	lstTextPartner.push_back(StringPartner(_T("(MB5)"), ValueToString(pCalculator->m_CalcDatalst.MB5)));
	lstTextPartner.push_back(StringPartner(_T("(P14)"), ValueToString(pCalculator->m_CalcDatalst.P14)));
	lstTextPartner.push_back(StringPartner(_T("(GA4)"), ValueToString(pCalculator->m_CalcDatalst.GA4)));
	lstTextPartner.push_back(StringPartner(_T("(P23)"), ValueToString(pCalculator->m_CalcDatalst.P23)));
	lstTextPartner.push_back(StringPartner(_T("(P21)"), ValueToString(pCalculator->m_CalcDatalst.P21)));
	lstTextPartner.push_back(StringPartner(_T("(MA2)"), ValueToString(pCalculator->m_CalcDatalst.MA2)));
	lstTextPartner.push_back(StringPartner(_T("(P09)"), ValueToString(pCalculator->m_CalcDatalst.P09)));
	lstTextPartner.push_back(StringPartner(_T("(GC31)"), ValueToString((long)pCalculator->m_CalcDatalst.GC31)));
	lstTextPartner.push_back(StringPartner(_T("(ececcode)"), ValueToString(pCalculator->m_CalcDatalst.ececcode)));
	lstTextPartner.push_back(StringPartner(_T("(ecec11)"), ValueToString(pCalculator->m_CalcDatalst.ecec11)));
	//
	lstTextPartner.push_back(StringPartner(_T("(DA11)"), ValueToString(pCalculator->m_CalcDatalst.DA11)));
	lstTextPartner.push_back(StringPartner(_T("(DA12)"), ValueToString(pCalculator->m_CalcDatalst.DA12)));
	lstTextPartner.push_back(StringPartner(_T("(DA13)"), ValueToString(pCalculator->m_CalcDatalst.DA13)));
	lstTextPartner.push_back(StringPartner(_T("(DA14)"), ValueToString(pCalculator->m_CalcDatalst.DA14)));
	lstTextPartner.push_back(StringPartner(_T("(DA15)"), ValueToString(pCalculator->m_CalcDatalst.DA15)));
	lstTextPartner.push_back(StringPartner(_T("(DA16)"), ValueToString(pCalculator->m_CalcDatalst.DA16)));
	lstTextPartner.push_back(StringPartner(_T("(DA2)"), ValueToString(pCalculator->m_CalcDatalst.DA2)));
	lstTextPartner.push_back(StringPartner(_T("(DA3)"), ValueToString(pCalculator->m_CalcDatalst.DA3)));
	lstTextPartner.push_back(StringPartner(_T("(DA4)"), ValueToString(pCalculator->m_CalcDatalst.DA4)));
	lstTextPartner.push_back(StringPartner(_T("(DA41)"), ValueToString(pCalculator->m_CalcDatalst.DA41)));
	lstTextPartner.push_back(StringPartner(_T("(DA42)"), ValueToString(pCalculator->m_CalcDatalst.DA42)));
	lstTextPartner.push_back(StringPartner(_T("(DA43)"), ValueToString(pCalculator->m_CalcDatalst.DA43)));
	lstTextPartner.push_back(StringPartner(_T("(DA44)"), ValueToString(pCalculator->m_CalcDatalst.DA44)));
	lstTextPartner.push_back(StringPartner(_T("(DA45)"), ValueToString(pCalculator->m_CalcDatalst.DA45)));
	lstTextPartner.push_back(StringPartner(_T("(DA46)"), ValueToString(pCalculator->m_CalcDatalst.DA46)));
	lstTextPartner.push_back(StringPartner(_T("(DA5)"), ValueToString(pCalculator->m_CalcDatalst.DA5)));
	lstTextPartner.push_back(StringPartner(_T("(DA51)"), ValueToString(pCalculator->m_CalcDatalst.DA51)));
	lstTextPartner.push_back(StringPartner(_T("(DB1)"), ValueToString(pCalculator->m_CalcDatalst.DB1)));
	lstTextPartner.push_back(StringPartner(_T("(DB11)"), ValueToString(pCalculator->m_CalcDatalst.DB11)));
	lstTextPartner.push_back(StringPartner(_T("(DB12)"), ValueToString(pCalculator->m_CalcDatalst.DB12)));
	lstTextPartner.push_back(StringPartner(_T("(DB2)"), ValueToString(pCalculator->m_CalcDatalst.DB2)));
	lstTextPartner.push_back(StringPartner(_T("(DB21)"), ValueToString(pCalculator->m_CalcDatalst.DB21)));
	lstTextPartner.push_back(StringPartner(_T("(DB22)"), ValueToString(pCalculator->m_CalcDatalst.DB22)));
	lstTextPartner.push_back(StringPartner(_T("(DB23)"), ValueToString(pCalculator->m_CalcDatalst.DB23)));
	lstTextPartner.push_back(StringPartner(_T("(DB24)"), ValueToString(pCalculator->m_CalcDatalst.DB24)));
	lstTextPartner.push_back(StringPartner(_T("(DB3)"), ValueToString(pCalculator->m_CalcDatalst.DB3)));
	lstTextPartner.push_back(StringPartner(_T("(DB31)"), ValueToString(pCalculator->m_CalcDatalst.DB31)));
	lstTextPartner.push_back(StringPartner(_T("(DB32)"), ValueToString(pCalculator->m_CalcDatalst.DB32)));
	lstTextPartner.push_back(StringPartner(_T("(DB33)"), ValueToString(pCalculator->m_CalcDatalst.DB33)));
	lstTextPartner.push_back(StringPartner(_T("(DB34)"), ValueToString(pCalculator->m_CalcDatalst.DB34)));
	lstTextPartner.push_back(StringPartner(_T("(DB35)"), ValueToString(pCalculator->m_CalcDatalst.DB35)));
	lstTextPartner.push_back(StringPartner(_T("(DB36)"), ValueToString(pCalculator->m_CalcDatalst.DB36)));
	lstTextPartner.push_back(StringPartner(_T("(DB37)"), ValueToString(pCalculator->m_CalcDatalst.DB37)));
	lstTextPartner.push_back(StringPartner(_T("(DB38)"), ValueToString(pCalculator->m_CalcDatalst.DB38)));
	lstTextPartner.push_back(StringPartner(_T("(DB39)"), ValueToString(pCalculator->m_CalcDatalst.DB39)));
	lstTextPartner.push_back(StringPartner(_T("(DB310)"), ValueToString(pCalculator->m_CalcDatalst.DB310)));
	lstTextPartner.push_back(StringPartner(_T("(DB311)"), ValueToString(pCalculator->m_CalcDatalst.DB311)));
	lstTextPartner.push_back(StringPartner(_T("(DB312)"), ValueToString(pCalculator->m_CalcDatalst.DB312)));
	lstTextPartner.push_back(StringPartner(_T("(DB313)"), ValueToString(pCalculator->m_CalcDatalst.DB313)));
	lstTextPartner.push_back(StringPartner(_T("(DB314)"), ValueToString(pCalculator->m_CalcDatalst.DB314)));
	lstTextPartner.push_back(StringPartner(_T("(DB315)"), ValueToString(pCalculator->m_CalcDatalst.DB315)));
	lstTextPartner.push_back(StringPartner(_T("(DB4)"), ValueToString(pCalculator->m_CalcDatalst.DB4)));
	lstTextPartner.push_back(StringPartner(_T("(DB41)"), ValueToString(pCalculator->m_CalcDatalst.DB41)));
	lstTextPartner.push_back(StringPartner(_T("(DB42)"), ValueToString(pCalculator->m_CalcDatalst.DB42)));
	lstTextPartner.push_back(StringPartner(_T("(DB5)"), ValueToString(pCalculator->m_CalcDatalst.DB5)));
	lstTextPartner.push_back(StringPartner(_T("(DB51)"), ValueToString(pCalculator->m_CalcDatalst.DB51)));
	lstTextPartner.push_back(StringPartner(_T("(DB52)"), ValueToString(pCalculator->m_CalcDatalst.DB52)));
	lstTextPartner.push_back(StringPartner(_T("(DB53)"), ValueToString(pCalculator->m_CalcDatalst.DB53)));
	lstTextPartner.push_back(StringPartner(_T("(DB54)"), ValueToString(pCalculator->m_CalcDatalst.DB54)));
	lstTextPartner.push_back(StringPartner(_T("(DB55)"), ValueToString(pCalculator->m_CalcDatalst.DB55)));
	lstTextPartner.push_back(StringPartner(_T("(DB56)"), ValueToString(pCalculator->m_CalcDatalst.DB56)));
	lstTextPartner.push_back(StringPartner(_T("(DB57)"), ValueToString(pCalculator->m_CalcDatalst.DB57)));
	lstTextPartner.push_back(StringPartner(_T("(DB58)"), ValueToString(pCalculator->m_CalcDatalst.DB58)));
	lstTextPartner.push_back(StringPartner(_T("(DB59)"), ValueToString(pCalculator->m_CalcDatalst.DB59)));
	lstTextPartner.push_back(StringPartner(_T("(DB510)"), ValueToString(pCalculator->m_CalcDatalst.DB510)));
	lstTextPartner.push_back(StringPartner(_T("(DB6)"), ValueToString(pCalculator->m_CalcDatalst.DB6)));
	lstTextPartner.push_back(StringPartner(_T("(DB61)"), ValueToString(pCalculator->m_CalcDatalst.DB61)));
	lstTextPartner.push_back(StringPartner(_T("(DB7)"), ValueToString(pCalculator->m_CalcDatalst.DB7)));
	lstTextPartner.push_back(StringPartner(_T("(DB71)"), ValueToString(pCalculator->m_CalcDatalst.DB71)));
	lstTextPartner.push_back(StringPartner(_T("(DB72)"), ValueToString(pCalculator->m_CalcDatalst.DB72)));
	lstTextPartner.push_back(StringPartner(_T("(DB73)"), ValueToString(pCalculator->m_CalcDatalst.DB73)));
	lstTextPartner.push_back(StringPartner(_T("(DB74)"), ValueToString(pCalculator->m_CalcDatalst.DB74)));
	lstTextPartner.push_back(StringPartner(_T("(DB75)"), ValueToString(pCalculator->m_CalcDatalst.DB75)));
	lstTextPartner.push_back(StringPartner(_T("(DB76)"), ValueToString(pCalculator->m_CalcDatalst.DB76)));
	lstTextPartner.push_back(StringPartner(_T("(DB77)"), ValueToString(pCalculator->m_CalcDatalst.DB77)));
	lstTextPartner.push_back(StringPartner(_T("(DB8)"), ValueToString(pCalculator->m_CalcDatalst.DB8)));
	lstTextPartner.push_back(StringPartner(_T("(DB81)"), ValueToString(pCalculator->m_CalcDatalst.DB81)));
	lstTextPartner.push_back(StringPartner(_T("(DB82)"), ValueToString(pCalculator->m_CalcDatalst.DB82)));
	lstTextPartner.push_back(StringPartner(_T("(DB83)"), ValueToString(pCalculator->m_CalcDatalst.DB83)));
	lstTextPartner.push_back(StringPartner(_T("(DB84)"), ValueToString(pCalculator->m_CalcDatalst.DB84)));
	lstTextPartner.push_back(StringPartner(_T("(DC1)"), ValueToString(pCalculator->m_CalcDatalst.DC1)));
	lstTextPartner.push_back(StringPartner(_T("(DC11)"), ValueToString(pCalculator->m_CalcDatalst.DC11)));
	lstTextPartner.push_back(StringPartner(_T("(DC12)"), ValueToString(pCalculator->m_CalcDatalst.DC12)));
	lstTextPartner.push_back(StringPartner(_T("(DC13)"), ValueToString(pCalculator->m_CalcDatalst.DC13)));
	lstTextPartner.push_back(StringPartner(_T("(DC14)"), ValueToString(pCalculator->m_CalcDatalst.DC14)));
	lstTextPartner.push_back(StringPartner(_T("(DC2)"), ValueToString(pCalculator->m_CalcDatalst.DC2)));
	lstTextPartner.push_back(StringPartner(_T("(DC21)"), ValueToString(pCalculator->m_CalcDatalst.DC21)));
	lstTextPartner.push_back(StringPartner(_T("(DC22)"), ValueToString(pCalculator->m_CalcDatalst.DC22)));
	lstTextPartner.push_back(StringPartner(_T("(DC23)"), ValueToString(pCalculator->m_CalcDatalst.DC23)));
	lstTextPartner.push_back(StringPartner(_T("(DC3)"), ValueToString(pCalculator->m_CalcDatalst.DC3)));
	lstTextPartner.push_back(StringPartner(_T("(DC4)"), ValueToString(pCalculator->m_CalcDatalst.DC4)));
	lstTextPartner.push_back(StringPartner(_T("(DC5)"), ValueToString(pCalculator->m_CalcDatalst.DC5)));
	lstTextPartner.push_back(StringPartner(_T("(DC51)"), ValueToString(pCalculator->m_CalcDatalst.DC51)));
	lstTextPartner.push_back(StringPartner(_T("(DC52)"), ValueToString(pCalculator->m_CalcDatalst.DC52)));
	lstTextPartner.push_back(StringPartner(_T("(DC53)"), ValueToString(pCalculator->m_CalcDatalst.DC53)));
	lstTextPartner.push_back(StringPartner(_T("(DC54)"), ValueToString(pCalculator->m_CalcDatalst.DC54)));
	lstTextPartner.push_back(StringPartner(_T("(DC55)"), ValueToString(pCalculator->m_CalcDatalst.DC55)));
	lstTextPartner.push_back(StringPartner(_T("(DC56)"), ValueToString(pCalculator->m_CalcDatalst.DC56)));
	lstTextPartner.push_back(StringPartner(_T("(DC57)"), ValueToString(pCalculator->m_CalcDatalst.DC57)));
	lstTextPartner.push_back(StringPartner(_T("(DC58)"), ValueToString(pCalculator->m_CalcDatalst.DC58)));
	lstTextPartner.push_back(StringPartner(_T("(DC59)"), ValueToString(pCalculator->m_CalcDatalst.DC59)));
	lstTextPartner.push_back(StringPartner(_T("(DC6)"), ValueToString(pCalculator->m_CalcDatalst.DC6)));
	lstTextPartner.push_back(StringPartner(_T("(DC61)"), ValueToString(pCalculator->m_CalcDatalst.DC61)));
	lstTextPartner.push_back(StringPartner(_T("(DC7)"), ValueToString(pCalculator->m_CalcDatalst.DC7)));
	lstTextPartner.push_back(StringPartner(_T("(DC71)"), ValueToString(pCalculator->m_CalcDatalst.DC71)));
	lstTextPartner.push_back(StringPartner(_T("(DC72)"), ValueToString(pCalculator->m_CalcDatalst.DC72)));
	lstTextPartner.push_back(StringPartner(_T("(DC73)"), ValueToString(pCalculator->m_CalcDatalst.DC73)));
	lstTextPartner.push_back(StringPartner(_T("(DC74)"), ValueToString(pCalculator->m_CalcDatalst.DC74)));
	lstTextPartner.push_back(StringPartner(_T("(DC75)"), ValueToString(pCalculator->m_CalcDatalst.DC75)));
	lstTextPartner.push_back(StringPartner(_T("(DC76)"), ValueToString(pCalculator->m_CalcDatalst.DC76)));
	lstTextPartner.push_back(StringPartner(_T("(DC77)"), ValueToString(pCalculator->m_CalcDatalst.DC77)));
	lstTextPartner.push_back(StringPartner(_T("(DC78)"), ValueToString(pCalculator->m_CalcDatalst.DC78)));
	lstTextPartner.push_back(StringPartner(_T("(DC8)"), ValueToString(pCalculator->m_CalcDatalst.DC8)));
	lstTextPartner.push_back(StringPartner(_T("(DC81)"), ValueToString(pCalculator->m_CalcDatalst.DC81)));
	lstTextPartner.push_back(StringPartner(_T("(DC9)"), ValueToString(pCalculator->m_CalcDatalst.DC9)));
	lstTextPartner.push_back(StringPartner(_T("(DD1)"), ValueToString(pCalculator->m_CalcDatalst.DD1)));
	lstTextPartner.push_back(StringPartner(_T("(DD2)"), ValueToString(pCalculator->m_CalcDatalst.DD2)));
	lstTextPartner.push_back(StringPartner(_T("(DD3)"), ValueToString(pCalculator->m_CalcDatalst.DD3)));
	lstTextPartner.push_back(StringPartner(_T("(DD4)"), ValueToString(pCalculator->m_CalcDatalst.DD4)));
	lstTextPartner.push_back(StringPartner(_T("(DD5)"), ValueToString(pCalculator->m_CalcDatalst.DD5)));
	lstTextPartner.push_back(StringPartner(_T("(DD6)"), ValueToString(pCalculator->m_CalcDatalst.DD6)));
	lstTextPartner.push_back(StringPartner(_T("(DD61)"), ValueToString(pCalculator->m_CalcDatalst.DD61)));
	lstTextPartner.push_back(StringPartner(_T("(DD62)"), ValueToString(pCalculator->m_CalcDatalst.DD62)));
	lstTextPartner.push_back(StringPartner(_T("(DD63)"), ValueToString(pCalculator->m_CalcDatalst.DD63)));
	lstTextPartner.push_back(StringPartner(_T("(DD64)"), ValueToString(pCalculator->m_CalcDatalst.DD64)));
	lstTextPartner.push_back(StringPartner(_T("(DD7)"), ValueToString(pCalculator->m_CalcDatalst.DD7)));
	lstTextPartner.push_back(StringPartner(_T("(DD8)"), ValueToString(pCalculator->m_CalcDatalst.DD8)));
	lstTextPartner.push_back(StringPartner(_T("(DE3)"), ValueToString(pCalculator->m_CalcDatalst.DE3)));
	lstTextPartner.push_back(StringPartner(_T("(DE4)"), ValueToString(pCalculator->m_CalcDatalst.DE4)));
	lstTextPartner.push_back(StringPartner(_T("(DE41)"), ValueToString(pCalculator->m_CalcDatalst.DE41)));
	lstTextPartner.push_back(StringPartner(_T("(DE5)"), ValueToString(pCalculator->m_CalcDatalst.DE5)));
	lstTextPartner.push_back(StringPartner(_T("(DE6)"), ValueToString(pCalculator->m_CalcDatalst.DE6)));
	lstTextPartner.push_back(StringPartner(_T("(DE7)"), ValueToString(pCalculator->m_CalcDatalst.DE7)));
	lstTextPartner.push_back(StringPartner(_T("(DF1)"), ValueToString(pCalculator->m_CalcDatalst.DF1)));
	lstTextPartner.push_back(StringPartner(_T("(DF11)"), ValueToString(pCalculator->m_CalcDatalst.DF11)));
	lstTextPartner.push_back(StringPartner(_T("(DF2)"), ValueToString(pCalculator->m_CalcDatalst.DF2)));
	lstTextPartner.push_back(StringPartner(_T("(DF3)"), ValueToString(pCalculator->m_CalcDatalst.DF3)));
	lstTextPartner.push_back(StringPartner(_T("(DF4)"), ValueToString(pCalculator->m_CalcDatalst.DF4)));
	lstTextPartner.push_back(StringPartner(_T("(MO01)"), ValueToString(pCalculator->m_CalcDatalst.MO01)));
	lstTextPartner.push_back(StringPartner(_T("(MO02)"), ValueToString(pCalculator->m_CalcDatalst.MO02)));
	lstTextPartner.push_back(StringPartner(_T("(MO03)"), ValueToString(pCalculator->m_CalcDatalst.MO03)));
	lstTextPartner.push_back(StringPartner(_T("(MO04)"), ValueToString(pCalculator->m_CalcDatalst.MO04)));
	lstTextPartner.push_back(StringPartner(_T("(MO05)"), ValueToString(pCalculator->m_CalcDatalst.MO05)));
	lstTextPartner.push_back(StringPartner(_T("(MO06)"), ValueToString(pCalculator->m_CalcDatalst.MO06)));
	lstTextPartner.push_back(StringPartner(_T("(MO07)"), ValueToString(pCalculator->m_CalcDatalst.MO07)));
	lstTextPartner.push_back(StringPartner(_T("(MO08)"), ValueToString(pCalculator->m_CalcDatalst.MO08)));
	lstTextPartner.push_back(StringPartner(_T("(MO09)"), ValueToString(pCalculator->m_CalcDatalst.MO09)));
	lstTextPartner.push_back(StringPartner(_T("(MO10)"), ValueToString(pCalculator->m_CalcDatalst.MO10)));
	lstTextPartner.push_back(StringPartner(_T("(MO11)"), ValueToString(pCalculator->m_CalcDatalst.MO11)));
	lstTextPartner.push_back(StringPartner(_T("(MO12)"), ValueToString(pCalculator->m_CalcDatalst.MO12)));
	lstTextPartner.push_back(StringPartner(_T("(MO13)"), ValueToString(pCalculator->m_CalcDatalst.MO13)));
	lstTextPartner.push_back(StringPartner(_T("(MO14)"), ValueToString(pCalculator->m_CalcDatalst.MO14)));
	lstTextPartner.push_back(StringPartner(_T("(MO15)"), ValueToString(pCalculator->m_CalcDatalst.MO15)));
	lstTextPartner.push_back(StringPartner(_T("(MO16)"), ValueToString(pCalculator->m_CalcDatalst.MO16)));
	lstTextPartner.push_back(StringPartner(_T("(MO17)"), ValueToString(pCalculator->m_CalcDatalst.MO17)));
	lstTextPartner.push_back(StringPartner(_T("(MO18)"), ValueToString(pCalculator->m_CalcDatalst.MO18)));
	lstTextPartner.push_back(StringPartner(_T("(MO23)"), ValueToString(pCalculator->m_CalcDatalst.MO23)));
	lstTextPartner.push_back(StringPartner(_T("(MO24)"), ValueToString(pCalculator->m_CalcDatalst.MO24)));
	lstTextPartner.push_back(StringPartner(_T("(MO25)"), ValueToString(pCalculator->m_CalcDatalst.MO25)));
	lstTextPartner.push_back(StringPartner(_T("(MO26)"), ValueToString(pCalculator->m_CalcDatalst.MO26)));
	lstTextPartner.push_back(StringPartner(_T("(RA01)"), ValueToString(pCalculator->m_CalcDatalst.RA01)));
	lstTextPartner.push_back(StringPartner(_T("(RA02)"), ValueToString(pCalculator->m_CalcDatalst.RA02)));
	lstTextPartner.push_back(StringPartner(_T("(RA03)"), ValueToString(pCalculator->m_CalcDatalst.RA03)));
	lstTextPartner.push_back(StringPartner(_T("(RA031)"), ValueToString(pCalculator->m_CalcDatalst.RA031)));
	lstTextPartner.push_back(StringPartner(_T("(RA04)"), ValueToString(pCalculator->m_CalcDatalst.RA04)));
	lstTextPartner.push_back(StringPartner(_T("(RA041)"), ValueToString(pCalculator->m_CalcDatalst.RA041)));
	lstTextPartner.push_back(StringPartner(_T("(RA05)"), ValueToString(pCalculator->m_CalcDatalst.RA05)));
	lstTextPartner.push_back(StringPartner(_T("(RA06)"), ValueToString(pCalculator->m_CalcDatalst.RA06)));
	lstTextPartner.push_back(StringPartner(_T("(RA07)"), ValueToString(pCalculator->m_CalcDatalst.RA07)));
	lstTextPartner.push_back(StringPartner(_T("(RA08)"), ValueToString(pCalculator->m_CalcDatalst.RA08)));
	lstTextPartner.push_back(StringPartner(_T("(RA09)"), ValueToString(pCalculator->m_CalcDatalst.RA09)));
	lstTextPartner.push_back(StringPartner(_T("(RA10)"), ValueToString(pCalculator->m_CalcDatalst.RA10)));
	lstTextPartner.push_back(StringPartner(_T("(RA11)"), ValueToString(pCalculator->m_CalcDatalst.RA11)));
	lstTextPartner.push_back(StringPartner(_T("(RA12)"), ValueToString(pCalculator->m_CalcDatalst.RA12)));
	lstTextPartner.push_back(StringPartner(_T("(RA13)"), ValueToString(pCalculator->m_CalcDatalst.RA13)));
	//	
	lstTextPartner.push_back(StringPartner(_T("(AA01)"), ValueToString(pCalculator->m_CalcDatalst.AA01)));
	lstTextPartner.push_back(StringPartner(_T("(AA02)"), ValueToString(pCalculator->m_CalcDatalst.AA02)));
	lstTextPartner.push_back(StringPartner(_T("(AA03)"), ValueToString(pCalculator->m_CalcDatalst.AA03)));
	lstTextPartner.push_back(StringPartner(_T("(AA04)"), ValueToString(pCalculator->m_CalcDatalst.AA04)));
	lstTextPartner.push_back(StringPartner(_T("(AA05)"), ValueToString(pCalculator->m_CalcDatalst.AA05)));
	//
	//
	pWordApp->ReplaceTexts(lstTextPartner);

	return TRUE;
}
//----------------------------------------------------