//----------------------------------------------------
//	File Name:DatabaseDefine
//	Introduce:Define database data structs
//
//----------------------------------------------------
#ifndef	_DATABASE_DEFINE_H_
#define	_DATABASE_DEFINE_H_
//----------------------------------------------------
#include <list>
using namespace std;
//----------------------------------------------------
#define	STR_FEMALE			L"女士"
#define	STR_MALE			L"男士"
#define	STR_GROUP_ADMIN		L"系统管理员：可对系统中任何数据进行查找、添加、修改和删除操作，并能对系统进行设置，比如：数据库文件路径设置、备份/恢复设置等。"
#define	STR_GROUP_NORMAL	L"普通用户：只能对系统中\"VIP会员数据表\"和\"销售记录表\"进行查找、添加、修改和删除操作，对\"商品管理表\"有查找和浏览功能。"
#define	STR_GROUP_VIEWER	L"浏览用户：只能对系统中\"VIP会员数据表\"、\"销售记录表\"和\"商品管理表\"有查找和浏览功能。"
//----------------------------------------------------
//	Name:eTableID
//	Introduce:Table ID defintion
//----------------------------------------------------
enum eTableID
{
	eTableID_None			= 0,
	eTableID_Member			= 1,
	eTableID_Record			= 2,
	eTableID_User			= 3,
	eTableID_Goods			= 4,
	eTableID_Cnt
};
//----------------------------------------------------
//	Name:eEmployeeGroup
//	Introduce:Employee groups defintion
//----------------------------------------------------
enum eEmployeeGroup
{
	eEmployeeGroup_Admin	= 0,
	eEmployeeGroup_Normal	= 1,
	eEmployeeGroup_Viewer	= 2,
	eEmployeeGroup_Cnt
};
//----------------------------------------------------
//	Name:eTableDataType
//	Introduce:Table data type defintion
//----------------------------------------------------
enum eTableDataType
{
	eTableDataType_UnKnown	= 0,
	eTableDataType_DWORD	= 1,
	eTableDataType_FLOAT	= 2,
	eTableDataType_Date		= 3,
	eTableDataType_String	= 4,
	eTableDataType_Cnt
};
//----------------------------------------------------


//----------------------------------------------------
//	Name:TableColumn
//	Introduce:Table column struct defintion
//----------------------------------------------------
typedef struct tagTableColumn
{
	tagTableColumn()
	{
		bIsIndex = FALSE;
		bIsVisible = TRUE;
		lColumnWidth = 100;
		strName = L"";
		strDisplayText = L"";
		eDataType = eTableDataType_String;
	};
	//------------------------------------------------
	tagTableColumn(BOOL bIsAudtoIndex, LONG lWidth, CStringW strColumnName, CStringW strColumnText, eTableDataType eType, BOOL bVisible = TRUE)
	{
		bIsIndex = bIsAudtoIndex;
		bIsVisible = bVisible;
		lColumnWidth = lWidth;
		strName = strColumnName;
		strDisplayText = strColumnText;
		eDataType = eType;
	};
	//------------------------------------------------
	tagTableColumn & operator=(tagTableColumn src)
	{
		bIsIndex = src.bIsIndex;
		bIsVisible = src.bIsVisible;
		lColumnWidth = src.lColumnWidth;
		strName = src.strName;
		strDisplayText = src.strDisplayText;
		eDataType = src.eDataType;
	}
	//------------------------------------------------
	BOOL				bIsIndex;
	BOOL				bIsVisible;
	CStringW			strName;
	CStringW			strDisplayText;
	LONG				lColumnWidth;
	eTableDataType		eDataType;
}TableColumn, *LPTABLECOLUMN;
//----------------------------------------------------
typedef list<TableColumn> TableColumnsList;
//----------------------------------------------------
//	Name:TableData
//	Introduce:A database table struct defintion
//----------------------------------------------------
typedef struct tagTableData
{
	tagTableData()
	{
		eID = eTableID_None;
		strName = L"";
	}
	//------------------------------------------------
	eTableID			eID;
	CStringW			strName;
	TableColumnsList	lstColumns;	
}TableData, *LPTABLEDATA;
//----------------------------------------------------
#endif	//_DATABASE_DEFINE_H_
//----------------------------------------------------
