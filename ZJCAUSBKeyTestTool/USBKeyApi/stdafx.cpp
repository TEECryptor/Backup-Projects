// stdafx.cpp : source file that includes just the standard includes
// USBKeyApi.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <stdio.h>
#include <atlconv.h>
#include "./LogFile/LogFile.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

CHAR g_csLogText[512] = {0};

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

	ZJCA_LogE(A2W(g_csLogText));

	delete[] szBuffer;
	va_end(vp);
}