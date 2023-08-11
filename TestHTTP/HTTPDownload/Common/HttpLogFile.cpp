//--------------------------------------------
//	File Name:HTTPLogFile.cpp
//	Introduce:The implement file of class CHTTPLogFile
//	Date:2010-10-09
//--------------------------------------------
#include ".\httplogfile.h"
//--------------------------------------------
//	Name:CHttpDownloadThread
//	Introduce:Construction
//--------------------------------------------
CHttpLogFile::CHttpLogFile(void)
 : m_pLogFile(NULL)
{
}
//--------------------------------------------
//	Name:~CHttpLogFile
//	Introduce:Distruction
//--------------------------------------------
CHttpLogFile::~CHttpLogFile(void)
{
}
//--------------------------------------------


//--------------------------------------------
//	Name:Open
//	Introduce:Open this log file
//--------------------------------------------
void CHttpLogFile::Open(const char* lpszLogFile)
{
	// Close current at first
	Close();

	if (NULL != lpszLogFile && strlen(lpszLogFile) > 0)
	{
		m_pLogFile = fopen(lpszLogFile, "w");
		InitializeCriticalSection(&m_csWriting);
	}	
}
//--------------------------------------------
//	Name:Log
//	Introduce:Write a text log information to this log file
//--------------------------------------------
void CHttpLogFile::Log(const char* lpszText)
{
	EnterCriticalSection(&m_csWriting);
	if (NULL != m_pLogFile)
	{
		fprintf(m_pLogFile, lpszText);
		fprintf(m_pLogFile, "\r\n");
	}
	LeaveCriticalSection(&m_csWriting);
}
//--------------------------------------------
//	Name:LogFormat
//	Introduce:Write a text with a long parater log information to this log file
//--------------------------------------------
void CHttpLogFile::LogFormat(const char* lpszFormat, 
							 long lParam)
{
	EnterCriticalSection(&m_csWriting);
	fprintf(m_pLogFile, lpszFormat, lParam);
	fprintf(m_pLogFile, "\r\n");
	LeaveCriticalSection(&m_csWriting);
}
//--------------------------------------------
//	Name:LogFormat
//	Introduce:Write a text with a string parater log information to this log file
//--------------------------------------------
void CHttpLogFile::LogFormat(const char* lpszFormat, const char* lParam)
{
	EnterCriticalSection(&m_csWriting);
	fprintf(m_pLogFile, lpszFormat, lParam);
	fprintf(m_pLogFile, "\r\n");
	LeaveCriticalSection(&m_csWriting);
}
//--------------------------------------------
//	Name:LogFormat
//	Introduce:Write a text with two long paraters log information to this log file
//--------------------------------------------
void CHttpLogFile::LogFormat(const char* lpszFormat, 
							 long lParam1, 
							 long lParam2)
{
	EnterCriticalSection(&m_csWriting);
	fprintf(m_pLogFile, lpszFormat, lParam1, lParam2);
	fprintf(m_pLogFile, "\r\n");
	LeaveCriticalSection(&m_csWriting);
}
//--------------------------------------------
//	Name:Close
//	Introduce:Close this log file
//--------------------------------------------
void CHttpLogFile::Close()
{
	if (NULL != m_pLogFile)
	{
		fclose(m_pLogFile);
		m_pLogFile = NULL;
		DeleteCriticalSection(&m_csWriting);
	}
}
//--------------------------------------------
