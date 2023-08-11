/*
	File Name: LogFile.h
	Date: 2015/04/27
	Introduce:

*/

#ifndef _LOG_FILE_H_
#define	_LOG_FILE_H_

#include <tchar.h>
#include "Singleton.h"

/*	Log 相关宏定义 */
#define LogInstance CLogFile::Instance()
#define ZJCA_LogI(...)	LogInstance.AddLog(__FUNCTION__, __LINE__, __VA_ARGS__);
#define ZJCA_LogC(...)	LogInstance.AddLog(__FUNCTION__, __LINE__, __CLASS__, __VA_ARGS__);
#define ZJCA_LogE(...)	LogInstance.LogError(__FUNCTION__, __LINE__, __VA_ARGS__);
#define FUNC_BEGIN	ZJCA_LogI(_T("Begin!"));
#define FUNC_END	ZJCA_LogI(_T("End!"));

/*	Log 开关和文件路径 */
extern BOOL	g_bLogEnabled;
extern TCHAR g_tcLogFile[MAX_PATH];

class CLogFile : public Singleton<CLogFile>
{
	DECLARE_AS_SINGLETON(CLogFile);

private:
	CLogFile();
	~CLogFile(void);
	
protected:
	bool OpenFile();
	void Close();
	void WriteLog(LPCVOID lpBuffer, DWORD dwBytesNum);
	void WriteLog(LPCTSTR szLog);
	void iLog(LPCVOID lpBuffer, DWORD dwBytesNum);
	void SetFileName(LPCTSTR szName);
	bool IsOpen();

	void Lock()  { ::EnterCriticalSection(&_csLock); }
	void Unlock() { ::LeaveCriticalSection(&_csLock); }

protected:
	CRITICAL_SECTION _csLock;
	int m_nLogClass;
	HANDLE _hFile;

public:
	void AddLog(LPCSTR lpcsFunc, int nLine, LPTSTR lpctFormat, ...);
	void AddLog(LPCSTR lpcsFunc, int nLine, int nLogClass, LPTSTR lpcsFormat, ...);
	void LogError(LPCSTR lpcsFunc, int nLine, LPTSTR lpctFormat, ...);
	LPCTSTR GetFilePath() { return g_tcLogFile; }
};
#endif // !_LOG_FILE_H_
