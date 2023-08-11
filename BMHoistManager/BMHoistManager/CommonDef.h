//----------------------------------------------------
//	File Name:CommonDef.h
//	Introduce:Define common data structs
//
//----------------------------------------------------
#ifndef _COMMONDEF_H_
#define	_COMMONDEF_H_

#include "Database/DataBaseDefine.h"
#include "Database/ADODatabase.h"

/* 公司类型字符串定义 */
typedef struct tagCompanyType_Str
{
	tagCompanyType_Str(CString strType, CString strShort)
	{
		strTypeName = strType;
		strShortName = strShort;
	};
	CString	strTypeName;	// 显示字符串
	CString	strShortName;	// 缩写字符
}CompanyType;


extern CADODataBase*	g_pDatabase;
extern CString			g_strUserName;
extern TableData		g_Tables[eTableID_Cnt];
extern CompanyType		g_strCompanyType[5];


#endif	//_COMMONDEF_H_