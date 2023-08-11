//--------------------------------------------------
//	FileName:ADOTable.h
//	Introduce:The CADOTable class definition file.
//			  This class use Recordset object to opterate
//			  a table or view in database.
//	Creator:Youfang Yao
//	Date:04/05/06
//	Version:1.0V
//--------------------------------------------------
#ifndef _ADOTABLE_H_
#define _ADOTABLE_H_
//--------------------------------------------------
#include <afxdb.h>
#include "ADODatabase.h"
//--------------------------------------------------
#define	MAX_TName_LEN	32
#define	MAX_SQL_LEN		256
//--------------------------------------------------
//	Name:CADOTable
//	Introduce:The class CADOTable definition
//--------------------------------------------------
class CADOTable
{
public:
	CADOTable();
	virtual ~CADOTable();
	//----------------------------------------------
public:
	//Open the recordset based on connect pointer and table name
	HRESULT		Open(CADODatabase* pConnect, LPCTSTR lpszTable);
	//Get the recordset count
	LONG		GetRecordsetCnt();
	//Get recordset value string based on title string
	HRESULT		GetValue(LPCTSTR lpszTitle, LPTSTR lpszValue);
	//Move the recordset to the first
	HRESULT		MoveFirst();
	//Move the recordset to the previous one 
	HRESULT		MovePrevious();
	//Move the recordset to the next one
	HRESULT		MoveNext();
	//Move the recordset to the end
	HRESULT		MoveEnd();
	//The recordset is at begin or not
	BOOL		IsBOF();
	//The recordset is at end or not
	BOOL		IsEOF();
	//To close the table
	HRESULT		Close();
	//----------------------------------------------
	//To add a new record to current table
	HRESULT		AddRecord(LPCTSTR lppTitles[], LPCTSTR lppValues[], int iCnt);
	//To edit current the record int the table
	HRESULT		EditRecord(LPCTSTR lppTitles[], LPCTSTR lppValues[], int iCnt);
	//To find records with find condition
	HRESULT		FindRecord(LPCTSTR lpFindString);
	//To sort the records based specific title
	HRESULT		SortRecord(LPCTSTR lpSortTile);
	//To delete the current record in the table
	HRESULT		DeleteRecord(int iIndex);
	//----------------------------------------------
private:
	CADODatabase*	m_pConnect;			//The database connect interface pointer
	_RecordsetPtr	m_pRecordset;		//The Recordset object
	//
	TCHAR			m_szCurTableName[MAX_TName_LEN];
	//----------------------------------------------
};
//--------------------------------------------------
#endif	//_ADOTABLE_H_
//--------------------------------------------------