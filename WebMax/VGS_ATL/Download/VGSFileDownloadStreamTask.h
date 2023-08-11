#pragma once

#include "..\VGSATLHeader.h"
#include "VGSFileDownloadTaskIF.h"
#include "VGSFileDownloadManager.h"
#include <set>

using namespace std;


// ��ʽ�����߳���Ϣ
typedef class CStreamThreadInfo
{
public:
	long  lThreadIdx;   // �߳�����
	DWORD dwStartPos;   // ������ʼλ��
	DWORD dwDataLen;    // Ҫ���ص����ݳ���
	DWORD dwFinished;   // �Ѿ���ɵ��ֽ���Ŀ
	DWORD dwCurSpeed;   // ��ǰ�������ٶ�
	DWORD dwMoreInfo;   // ��������������Ϣ

	CStreamThreadInfo()
	{
		lThreadIdx  = 0;
		dwStartPos  = 0;
		dwDataLen   = 0;
		dwFinished  = 0;
		dwCurSpeed  = 0;
		dwMoreInfo  = 0;
	}

	friend bool operator < (const CStreamThreadInfo& lv, const CStreamThreadInfo &rv)
	{
		return lv.lThreadIdx < rv.lThreadIdx;
	}


}CStreamThreadInfo;

class CVGSFileDownloadStreamTask : public IVGSFileDownloadTask
{
public:
	DWORD Init(const string& sName, const string &sURL, const string &sSaveFile, const string &sUsrName, const string &passward);
	DWORD GetFileSize();
	void  Download(bool bForceDownload = false);
	void  EnableReportStatus(bool bEnable);
	DWORD Stop();
	DWORD Release();
	string GetName(){return m_sName;}

	// ��Ҫ����������ļ����ͷ����߳�����
	virtual void  CreateThread() = 0;          
	virtual void  OnPrecessDownloadProgress(LONG lThreadID, DWORD dwFinishedBytes, DWORD dwCurrentSpeed) = 0;
	virtual void  OnPrecessDownloadStatusChange(LONG lThreadID, VGS_DOWNLOADTASK_STATUS status) = 0;

	// bool    m_bScanOK;            // �ļ��Ƿ��Ѿ�ɨ�����

public:
	CVGSFileDownloadStreamTask(CVGSFileDownloadManager* pMgr, IHttpDownloadIF * pHttpDownloader);
	~CVGSFileDownloadStreamTask();

protected:

	int  m_port; // �˿ں�

	CVGSFileDownloadManager* m_pVGSDownloadManager;
	virtual void ScanFile() = 0;  // �����ļ���ʽɨ���ļ�

	bool ParseURL();  //  ����URL

	set<CStreamThreadInfo> m_vStreamThreadInfo;  // �����߳���Ϣ 
private:
};