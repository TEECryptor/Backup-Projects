// stdafx.cpp : source file that includes just the standard includes
// SynoTFKeySDKTest.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CHAR g_csLogText[512] = {0};
FuncMsgCallback g_ErrMsgCallback = NULL;
LPVOID			g_pCallbackParam = NULL;


void USBKey_LogErr(LPSTR lpctFormat, ...)
{	
	va_list vp;
	va_start(vp, lpctFormat);
	int rc = _vscprintf(lpctFormat, vp) + 1;

	USES_CONVERSION;

	LPSTR szBuffer = new CHAR[rc];
	ZeroMemory(szBuffer, rc * sizeof(CHAR));
	rc = vsprintf_s(szBuffer, rc, lpctFormat, vp);

	memset(g_csLogText, 0, 512);
	sprintf_s(g_csLogText, 512, "%s", szBuffer);

	if (g_ErrMsgCallback)
	{
#ifdef UNICODE
		g_ErrMsgCallback(A2W(g_csLogText), g_pCallbackParam);
#else
		g_ErrMsgCallback(g_csLogText, g_pCallbackParam);
#endif	//UNICODE
	}

	delete[] szBuffer;
	va_end(vp);
}

void USBKey_ConvertBufToHexStr(LPBYTE lpData, ULONG ulLen, LPTSTR *ppHexText)
{
	ULONG ulOutLen = 0;
	TCHAR tcTemp[4] = {0};

	ulOutLen = 3*ulLen + 1;
	*ppHexText = new TCHAR[ulOutLen];
	memset(*ppHexText, 0, ulOutLen);

	for (ULONG ulIndex = 0; ulIndex < ulLen; ulIndex++)
	{
		_stprintf_s(tcTemp, 4, _T("%02X "), lpData[ulIndex]);
		_tcscat_s(*ppHexText, ulOutLen, tcTemp);
	}
}