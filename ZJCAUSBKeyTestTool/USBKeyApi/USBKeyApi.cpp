// USBKeyApi.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "../Include/USBKeyApi.h"
#include <atlconv.h>
#include "USBKeyCSPImpl.h"
#include "USBKeyP11Impl.h"
#include "USBKeySKFImpl.h"
#include "./LogFile/LogFile.h"


/***********************************************************************
 *	名称:USBKey_CreateCSP
 *	说明:创建CSP接口对象
 *	参数:无
 *	返回:返回CSP接口对象指针
 ***********************************************************************/
USBKEYAPI_API IUSBKeyApi* USBKey_CreateCSP()
{
	CUSBKeyCSPImpl* pCSP = new CUSBKeyCSPImpl();
	return pCSP;
}
/***********************************************************************
 *	名称:USBKey_CreateP11
 *	说明:创建PKCS11接口对象
 *	参数:无
 *	返回:返回PKCS11接口对象指针
 ***********************************************************************/
USBKEYAPI_API IUSBKeyApi* USBKey_CreateP11()
{
	CUSBKeyP11Impl* pP11 = new CUSBKeyP11Impl();
	return pP11;
}
/***********************************************************************
 *	名称:USBKey_CreateSKF
 *	说明:创建国密SKF接口对象
 *	参数:无
 *	返回:返回国密SKF接口对象指针
 ***********************************************************************/
USBKEYAPI_API IUSBKeyApi* USBKey_CreateSKF()
{
	CUSBKeySKFImpl* pSKF = new CUSBKeySKFImpl();
	return pSKF;
}
/***********************************************************************
 *	名称:USBKey_Destroy
 *	说明:销毁USB Key接口对象
 *	参数:待销毁的USB Key接口对象
 *	返回:无
 ***********************************************************************/
USBKEYAPI_API void USBKey_Destroy(IUSBKeyApi* pApi)
{
	if (pApi)
	{
		pApi->DisconnectDev();
		pApi->Release();
		delete pApi;
		pApi = NULL;
	}
}
/***********************************************************************
 *	名称:USBKey_GetErrorLog
 *	说明:获取错误信息字符
 *	参数:lpLogText:[OUT]:接受字符串的Buffer
 *		 pulLen:[IN/OUT]:Buffer长度
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
USBKEYAPI_API ULONG USBKey_GetErrorLog(LPSTR lpLogText, ULONG *pulLen)
{
	if (!lpLogText)
	{
		*pulLen = strlen(g_csLogText) + 1;
		return USBKEY_ERR_OK;
	}
	if (*pulLen < strlen(g_csLogText) + 1)
	{
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	strcpy_s(lpLogText, *pulLen, g_csLogText);
	*pulLen = strlen(g_csLogText) + 1;

	return USBKEY_ERR_OK;
}
/***********************************************************************
 *	名称:USBKey_LogEnabled
 *	说明:DLL Log输出设置
 *	参数:bEnable:[IN]:TRUE为开启Log功能；FALSE为关闭Log功能。
 *		 lpLogFile:[IN]:指定Log文件名
 *	返回:成功返回USBKEY_ERR_OK，否则返回错误代码。
 ***********************************************************************/
USBKEYAPI_API ULONG USBKey_LogEnabled(BOOL bEnable, LPSTR lpLogFile)
{
	USES_CONVERSION;

	g_bLogEnabled = bEnable;
	if (g_bLogEnabled)
	{
		if (!lpLogFile || strlen(lpLogFile) == 0)
		{
			return USBKEY_ERR_INVALIDPARAM;
		}

#ifdef UNICODE
		wcscpy_s(g_tcLogFile, MAX_PATH, A2W(lpLogFile));
#else
		strcpy_s(g_tcLogFile, MAX_PATH, lpLogFile);
#endif	//UNICODE
	}

	return USBKEY_ERR_OK;
}