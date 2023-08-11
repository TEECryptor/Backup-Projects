
#include "stdafx.h"
#include "CommonDef.h"



/* 表数组定义 */
TableData	g_Tables[eTableID_Cnt];


/* 公司类型字符串定义 */
CompanyType g_strCompanyType[5] = {	CompanyType(_T("客户"), _T("KH")), 
									CompanyType(_T("供应商"), _T("GY")),
									CompanyType(_T("物流"), _T("WL")), 
									CompanyType(_T("行政"), _T("XZ")), 
									CompanyType(_T("其他"), _T("QT"))};