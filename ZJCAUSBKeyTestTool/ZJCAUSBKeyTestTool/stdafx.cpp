
// stdafx.cpp : source file that includes just the standard includes
// ZJCAUSBKeyTestTool.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


TCHAR g_tcErrlogText[512] = {0};


void LogErrorMsg(LPCTSTR lpctFormat, ...)
{
	va_list vp;
	va_start(vp, lpctFormat);
	int clen = _tcslen(_T("[ERROR]"));
	int rc = _vsctprintf(lpctFormat, vp) + 1;

	LPTSTR szBuffer = new TCHAR[rc + clen];
	ZeroMemory(szBuffer, (rc + clen) * sizeof(TCHAR));
	_tcscpy_s(szBuffer, clen + 1, _T("[ERROR]"));
	clen += _vstprintf_s(szBuffer + clen, rc, lpctFormat, vp);
	
	memset(g_tcErrlogText, 0, 512);
	_stprintf_s(g_tcErrlogText, 512, _T("%s"), szBuffer);

	delete[] szBuffer;
	va_end(vp);
}