//-----------------------------------------------------------------------
//	File Name:ExcelExporter.h
//	Introduce:header file
//-----------------------------------------------------------------------
#ifndef	_EXCEL_EXPORTER_H_
#define _EXCEL_EXPORTER_H_
//-----------------------------------------------------------------------
#include <afxdb.h>
#include "CommonDefine.h"
//-----------------------------------------------------------------------
class CExcelExporter
{
public:
	CExcelExporter(void);
	virtual ~CExcelExporter(void);
	//-------------------------------------------------------------------
public:
	static BOOL SaveDataFromListCtrl(LPCTSTR lpszExcelFile, LPCTSTR lpszSheetName, TableData &tbStruct, CListCtrl* pListCtrl);
	static BOOL SaveDataFromTable(LPCTSTR lpszExcelFile, LPCTSTR lpszSheetName, TableData &tbStruct, CADOTable table);
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
#endif	//_EXCEL_EXPORTER_H_
//-----------------------------------------------------------------------
