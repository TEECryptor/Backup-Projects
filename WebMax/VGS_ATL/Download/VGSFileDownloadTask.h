#pragma once

#include "..\VGSATLHeader.h"
#include "VGSFileDownloadTaskIF.h"
#include "HTTPDownloadIF.h"

class CVGSFileDownloadTask : public IVGSFileDownloadTask
{
public:
	DWORD Init(const string& sName, const string &sURL, const string &sSaveFile, const string &sUsrName, const string &passward);
	DWORD GetFileSize();
	void  Download(bool bForceDownload = false);
	void  EnableReportStatus(bool bEnable);
	DWORD Stop();
	DWORD Release();

	// void  SetThreadCount(DWORD dwCount);
	void CreateThread();

public:
	CVGSFileDownloadTask(IHttpDownloadIF * pHttpDownloader);
	~CVGSFileDownloadTask();
protected:
private:
};