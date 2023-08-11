//----------------------------------------------------
//	File Name:CommonDef.h
//	Introduce:Define common data structs
//
//----------------------------------------------------
#ifndef _COMMONDEF_H_
#define	_COMMONDEF_H_

#include "Database/DataBaseDefine.h"
#include "Database/ADODatabase.h"

/* ��˾�����ַ������� */
typedef struct tagCompanyType_Str
{
	tagCompanyType_Str(CString strType, CString strShort)
	{
		strTypeName = strType;
		strShortName = strShort;
	};
	CString	strTypeName;	// ��ʾ�ַ���
	CString	strShortName;	// ��д�ַ�
}CompanyType;


extern CADODataBase*	g_pDatabase;
extern CString			g_strUserName;
extern TableData		g_Tables[eTableID_Cnt];
extern CompanyType		g_strCompanyType[5];


#endif	//_COMMONDEF_H_