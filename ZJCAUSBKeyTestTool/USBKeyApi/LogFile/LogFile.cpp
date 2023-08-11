#include "StdAfx.h"
#include "LogFile.h"
#include <Shlobj.h>
#include <atlconv.h>
#include <imagehlp.h>
#include "TimeAgent.h"

#pragma comment(lib, "imagehlp.lib")  

const BYTE UniFileHeader[2] = {0xFF, 0xFE};
TCHAR	g_tcLogFile[MAX_PATH] = {0};
BOOL	g_bLogEnabled = FALSE;

CLogFile::CLogFile()
	: _hFile(INVALID_HANDLE_VALUE)
	, m_nLogClass(0)
{
}

CLogFile::~CLogFile()
{
	::DeleteCriticalSection(&_csLock);
	Close();
}

bool CLogFile::OpenFile()
{
	if (!g_bLogEnabled)
		return false;

	if(IsOpen())
		Close();

	if(!g_tcLogFile)
		return false;

	InitializeCriticalSection(&_csLock);
	_hFile =  CreateFile(g_tcLogFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
	if (sizeof(TCHAR) > 1)
	{
		DWORD dwBytesWritten;
		WriteFile(_hFile, UniFileHeader, 2, &dwBytesWritten, NULL);
	}

	if(IsOpen())
	{
		WriteLog(_T("--------------------Start Log--------------------"));
		TCHAR szModuleType[MAX_PATH] = {0};
#ifdef ZJCA_ADMIN
		_tcscpy_s(szModuleType, _T("The module running under Admin mode."));
#else
		_tcscpy_s(szModuleType, _T("The module running under User mode."));
#endif
		WriteLog(szModuleType);
		TCHAR szModulePath[MAX_PATH] = {0};
		TCHAR szModulePathLog[2*MAX_PATH] = {0};
		GetModuleFileName(NULL, szModulePath, MAX_PATH - 1);
		_stprintf_s(szModulePathLog, 2*MAX_PATH-1, _T("The host application is: %s."), szModulePath);
		WriteLog(szModulePathLog);
	}

	return IsOpen();
}

void CLogFile::Close()
{
	if(IsOpen())
	{
		WriteLog(_T("---------------------End Log---------------------"));
		CloseHandle(_hFile);
		_hFile = INVALID_HANDLE_VALUE;
	}
}

void CLogFile::WriteLog(LPCVOID lpBuffer, DWORD dwBytesNum)
{
	TCHAR szTime[11] = {0};
	DWORD dwWriteLength = 0;

	if(IsOpen())
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		_stprintf_s(szTime, _T("[%02d:%02d:%02d]"), st.wHour, st.wMinute, st.wSecond);

		WriteFile(_hFile, szTime, 10 * sizeof(TCHAR), &dwWriteLength, NULL);
		WriteFile(_hFile, lpBuffer, dwBytesNum, &dwWriteLength, NULL);
		WriteFile(_hFile, _T("\xd\xa"), 2 * sizeof(TCHAR), &dwWriteLength, NULL);

		FlushFileBuffers(_hFile);
	}
}

void CLogFile::WriteLog(LPCTSTR szLog)
{
	if (szLog == NULL)
		return;

	WriteLog(reinterpret_cast<LPCVOID>(szLog), _tcslen(szLog) * sizeof(TCHAR));
}

void CLogFile::iLog(LPCVOID lpBuffer, DWORD dwBytesNum) //Add log
{
	//assert(lpBuffer);
	__try
	{
		Lock();

		if(!IsOpen())
			return;

		WriteLog(lpBuffer, dwBytesNum);
	}
	__finally
	{
		Unlock();
	} 
}

bool CLogFile::IsOpen()
{
	return _hFile != INVALID_HANDLE_VALUE;
}


void CLogFile::AddLog(LPCSTR lpcsFunc, int nLine, LPTSTR lpctFormat, ...)
{
	if (!g_bLogEnabled || lpctFormat == NULL)
		return;
	if (_tcslen(g_tcLogFile) == 0)
		return;
	if (!IsOpen())
	{
		if (!OpenFile())
		{
			return;
		}
	}

	va_list vp;
	va_start(vp, lpctFormat);
	int rc = _vsctprintf(lpctFormat, vp) + 1;

	LPTSTR szBuffer = new TCHAR[rc];
	ZeroMemory(szBuffer, rc * sizeof(TCHAR));
	rc = _vstprintf_s(szBuffer, rc, lpctFormat, vp);

	rc += strlen(lpcsFunc) + 16;
	LPTSTR szLogText = new TCHAR[rc];
	ZeroMemory(szLogText, rc * sizeof(TCHAR));
#ifdef UNICODE
	USES_CONVERSION;
	_stprintf_s(szLogText, rc, _T("[%s()][%d] %s"), A2W(lpcsFunc), nLine, szBuffer);
#else
	_stprintf_s(szLogText, rc, "[%s()][%d] %s", lpcsFunc, nLine, szBuffer);
#endif	//UNICODE

	iLog(szLogText, _tcslen(szLogText)*sizeof(TCHAR));
	delete[] szLogText;
	delete[] szBuffer;
	va_end(vp);
}

void CLogFile::AddLog(LPCSTR lpcsFunc, int nLine, int nLogClass, LPTSTR lpcsFormat, ...)
{
	if (!g_bLogEnabled || nLogClass > m_nLogClass)
		return;
	if (_tcslen(g_tcLogFile) == 0)
		return;
	if (!IsOpen())
	{
		if (!OpenFile())
		{
			return;
		}
	}

	CLogFile::AddLog(lpcsFunc, nLine, lpcsFormat);
}

void CLogFile::LogError(LPCSTR lpcsFunc, int nLine, LPTSTR lpctFormat, ...)
{
	if (!g_bLogEnabled || lpctFormat == NULL)
		return;
	if (_tcslen(g_tcLogFile) == 0)
		return;
	if (!IsOpen())
	{
		if (!OpenFile())
		{
			return;
		}
	}

	va_list vp;
	va_start(vp, lpctFormat);
	int clen = _tcslen(_T("[ERROR]"));
	int rc = _vsctprintf(lpctFormat, vp) + 1;

	LPTSTR szBuffer = new TCHAR[rc + clen];
	ZeroMemory(szBuffer, (rc + clen) * sizeof(TCHAR));
	_tcscpy_s(szBuffer, clen + 1, _T("[ERROR]"));
	clen += _vstprintf_s(szBuffer + clen, rc, lpctFormat, vp);
	
	rc = clen + strlen(lpcsFunc) + 16;
	LPTSTR szLogText = new TCHAR[rc];
	ZeroMemory(szLogText, rc * sizeof(TCHAR));
#ifdef UNICODE
	USES_CONVERSION;
	_stprintf_s(szLogText, rc, _T("[%s()][%d] %s"), A2W(lpcsFunc), nLine, szBuffer);
#else
	_stprintf_s(szLogText, rc, "[%s()][%d] %s", lpcsFunc, nLine, szBuffer);
#endif	//UNICODE

	iLog(szLogText, _tcslen(szLogText)*sizeof(TCHAR));
	delete[] szLogText;
	delete[] szBuffer;
	va_end(vp);
}
