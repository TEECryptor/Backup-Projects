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
 *	����:USBKey_CreateCSP
 *	˵��:����CSP�ӿڶ���
 *	����:��
 *	����:����CSP�ӿڶ���ָ��
 ***********************************************************************/
USBKEYAPI_API IUSBKeyApi* USBKey_CreateCSP()
{
	CUSBKeyCSPImpl* pCSP = new CUSBKeyCSPImpl();
	return pCSP;
}
/***********************************************************************
 *	����:USBKey_CreateP11
 *	˵��:����PKCS11�ӿڶ���
 *	����:��
 *	����:����PKCS11�ӿڶ���ָ��
 ***********************************************************************/
USBKEYAPI_API IUSBKeyApi* USBKey_CreateP11()
{
	CUSBKeyP11Impl* pP11 = new CUSBKeyP11Impl();
	return pP11;
}
/***********************************************************************
 *	����:USBKey_CreateSKF
 *	˵��:��������SKF�ӿڶ���
 *	����:��
 *	����:���ع���SKF�ӿڶ���ָ��
 ***********************************************************************/
USBKEYAPI_API IUSBKeyApi* USBKey_CreateSKF()
{
	CUSBKeySKFImpl* pSKF = new CUSBKeySKFImpl();
	return pSKF;
}
/***********************************************************************
 *	����:USBKey_Destroy
 *	˵��:����USB Key�ӿڶ���
 *	����:�����ٵ�USB Key�ӿڶ���
 *	����:��
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
 *	����:USBKey_GetErrorLog
 *	˵��:��ȡ������Ϣ�ַ�
 *	����:lpLogText:[OUT]:�����ַ�����Buffer
 *		 pulLen:[IN/OUT]:Buffer����
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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
 *	����:USBKey_LogEnabled
 *	˵��:DLL Log�������
 *	����:bEnable:[IN]:TRUEΪ����Log���ܣ�FALSEΪ�ر�Log���ܡ�
 *		 lpLogFile:[IN]:ָ��Log�ļ���
 *	����:�ɹ�����USBKEY_ERR_OK�����򷵻ش�����롣
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