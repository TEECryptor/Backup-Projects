//--------------------------------------------
//	File Name:HTTPLogFile.h
//	Introduce:Class CHTTPLogFile, define log file class
//	Date:2010-10-09
//--------------------------------------------
#ifndef	_HTTPLOGFILE_H_
#define	_HTTPLOGFILE_H_
//--------------------------------------------
#include <stdio.h>
#include <string.h>
#include <Windows.h>
//--------------------------------------------
#define	HTTPLOG(a, b) {if (a) {a->Log(b);}}
#define	HTTPLOG1(a, b, c) {if (a) {a->LogFormat(b, c);}}
#define	HTTPLOG2(a, b, c, d) {if (a) {a->LogFormat(b, c, d);}}
//--------------------------------------------
class CHttpLogFile
{
public:
	CHttpLogFile(void);
	~CHttpLogFile(void);
	//----------------------------------------
public:
	void	Open(const char* lpszLogFile);
	void	Log(const char* lpszText);
	void	LogFormat(const char* lpszFormat, long lParam);
	void	LogFormat(const char* lpszFormat, const char* lParam);
	void	LogFormat(const char* lpszFormat, long lParam1, long lParam2);
	void	Close();
	//----------------------------------------
private:
	FILE*				m_pLogFile;
	CRITICAL_SECTION	m_csWriting;
	//----------------------------------------
};
//--------------------------------------------
#endif	//_HTTPLOGFILE_H_
//--------------------------------------------
