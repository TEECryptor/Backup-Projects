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
	BOOL		OpenDatabase(LPCWSTR lpwzDatabaseFile, LPCWSTR lpwzUser = L"", LPCWSTR lpwzPassword = L"");
	void		CloseDatabase();
	//------------------------------------------------
private:
	_ConnectionPtr	m_pConnection;
	//------------------------------------------------
};
//----------------------------------------------------
