//--------------------------------------------------
//	FileName:ADODatabase.h
//	Introduce:The CADODatabase class definiton file.
//			  This class use Connection object to connect
//			  the database.
//	Creator:Youfang Yao
//	Date:04/05/06
//	Version:1.0V
//--------------------------------------------------
#ifndef _ADODATABASE_H_
#define _ADODATABASE_H_
//--------------------------------------------------
#include <afxdb.h>
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")
//--------------------------------------------------
//	Name:CADODatabase
//	Introduce:The class CADODatabase definition
//--------------------------------------------------
class CADODatabase
{
public:
	CADODatabase();
	virtual ~CADODatabase();
	//----------------------------------------------
public:
	//To open the database
	HRESULT			Open(LPCTSTR lpszConnectString);
	//Return current the connection pointer
	_ConnectionPtr	GetConnectPointer();
	//Get tables count
	LONG			GetTableCnt();
	//Get table name by special index
	HRESULT			GetTableName(LONG lID, LPTSTR lpszTableName);
	//To close the datebase connection
	HRESULT			Close();
	//---------------------------------------------
private:
	_ConnectionPtr	m_pConnection;		//The Connection object
	//---------------------------------------------
};
//--------------------------------------------------
#endif  //_ADODATABASE_H_
//--------------------------------------------------
