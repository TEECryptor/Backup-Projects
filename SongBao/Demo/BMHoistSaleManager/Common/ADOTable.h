//----------------------------------------------------
//	File Name: ADOTable.h
//	Introduce: Class CADOTable definition
//----------------------------------------------------
#pragma once
//----------------------------------------------------
#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" no_namespace rename("EOF","rsEOF")
//----------------------------------------------------
class CADOTable
{
public:
	CADOTable(void);
	~CADOTable(void);
	//------------------------------------------------
public:
	BOOL	OpenTable(IUnknown* pDatabaseConnect, LPCWSTR lpwzOpenFilter);
	void	CloseTable();
	DWORD	GetRecordCnt();
	BOOL	AddNewItem();
	BOOL	DeleteItem(LONG lIndex);
	BOOL	UpdateTable();
	BOOL	GetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, DWORD &dwResult);
	BOOL	GetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, float &fResult);
	BOOL	GetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, double &fResult);
	BOOL	GetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, CTime &dateResult);
	BOOL	GetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, LPWSTR lpwzResult, DWORD dwMaxLen);
	BOOL	SetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, DWORD dwVal);
	BOOL	SetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, float fVal);
	BOOL	SetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, CTime dateVal);
	BOOL	SetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, LPWSTR lpwzVal);
	//------------------------------------------------
private:
	BOOL	_GetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, _variant_t &var);
	BOOL	_SetRecordCell(LONG lIndex, LPCWSTR lpwzCellName, _variant_t &var);
	//------------------------------------------------
private:
	_RecordsetPtr	m_pRecordset;
	//------------------------------------------------
};
//----------------------------------------------------
