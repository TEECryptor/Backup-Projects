//----------------------------------------------------
//	File Name: ADODataBase.h
//	Introduce: Class CADODataBase definition
//----------------------------------------------------
#pragma once
//----------------------------------------------------
#import "C:\\Program Files\\Common Files\\System\\ado\\msado15.dll" no_namespace rename("EOF","rsEOF")
//----------------------------------------------------
class CADODataBase
{
public:
	CADODataBase(void);
	~CADODataBase(void);
	//------------------------------------------------
public:
	IUnknown*	GetConnectionPtr();
	BOOL		OpenDatabase(LPCWSTR lpwzDatabaseFile, LPCWSTR lpwzDatabasePassword = L"", BOOL bIsExcel = FALSE);
	BOOL		ExcuteSQL(LPCWSTR lpwzSQL);
	void		CloseDatabase();
	//------------------------------------------------
private:
	_ConnectionPtr	m_pConnection;
	//------------------------------------------------
};
//----------------------------------------------------
