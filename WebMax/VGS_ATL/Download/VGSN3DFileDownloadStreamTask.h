#pragma  once

#include "..\\VGSATLHeader.h"
#include "VGSFileDownloadStreamTask.h"
#include "..\\n3d\\N3D_Def.h"

#include <vector>
#include <wininet.h>

using namespace std;
using namespace N3D;

// �ļ�λ����Ϣ
struct CImagePosInfo
{
	DWORD startPos;  // ��ʼλ��
	DWORD datalen;   // ���ݳ���
	bool  bDownloaded;
	CImagePosInfo()
	{
		startPos = 0;
		datalen  = 0;
		bDownloaded = false;
	}
};

// �̺߳�ͼ�����ݵĶ�Ӧ��ϵ
class CThreadImageInfo
{
public:
	long lThreadIdx;
	vector<int>  vImageIndex;
	vector<bool> vbDownloaded;
	CThreadImageInfo()
	{
		lThreadIdx = 0;
	}

	friend bool operator < (const CThreadImageInfo& lv, const CThreadImageInfo &rv)
	{
		return lv.lThreadIdx < rv.lThreadIdx;
	}
};

class CVGSN3DFileDownloadStreamTask : public CVGSFileDownloadStreamTask
{
public:
	void  CreateThread(); 
	void  OnPrecessDownloadProgress(LONG lThreadID, DWORD finishedBytes, DWORD currentSpeed);
	void  OnPrecessDownloadStatusChange(LONG lThreadID, VGS_DOWNLOADTASK_STATUS status);

	
public:
	CVGSN3DFileDownloadStreamTask(CVGSFileDownloadManager* pMgr, IHttpDownloadIF * pHttpDownloader);
	~CVGSN3DFileDownloadStreamTask();

	// ���N3D��������ѹ����ĳ���
	DWORD GetN3DSceneZippedLength(){return m_N3DHeader.SceneZipDataLen;}

	// ���N3D�������ݽ�ѹ��ĳ���
	DWORD GetN3DSceneUnZippedLength(){return m_N3DHeader.OriginSceneDataLen;}

	// ��ȡN3D��������
	// pData : in �������ݵ�bufferָ��
	// len   : in �������ݵ�buffer���ȣ�������ڵ���GetN3DSceneZippedLength
	// return: �Ƿ���յ����ݡ�����������յ����ݣ�����true��
	bool ReadN3DSceneData(BYTE* pData, DWORD len);

	// ��ȡһ��ͼƬ������
	// pData : in �������ݵ�bufferָ��
	// len   : in �������ݵ�buffer���ȣ�������ڵ���GetImageDataLength
	// return: �Ƿ���յ����ݡ�����������յ����ݣ�����true��
	bool ReadnN3DOneImage(BYTE *pData, DWORD len, int imgIdx);

	// ��ȡһ��ͼƬ�����ݳ���
	DWORD GetImageDataLength(int imgIdx);

	// get n3d header
	bool GetN3DHeader(N3D_Header & header);

	// n3d��ͼƬ�ļ�����Ŀ
	int GetImageCount(){return m_vImagePosInfo.size();}

protected:
	void ScanFile();
	void ScanFileEx();
	bool ParseURL(LPCTSTR lpURL, 
		LPTSTR lpHost,  
		LPTSTR lpPort, 
		LPTSTR lpNetFile);

	// �����ص����ļ��ж�ȡһ������
	// pOutBuffer�Ĵ�СӦ�ò�С��len
	bool ReadDataFromLocalFile(BYTE* pOutBuffer, DWORD startPos, DWORD len);

	bool m_bSceneDownloaded;  // whether scene data is downloaed

private:
	N3D_Header    m_N3DHeader;  // n3d header
	long          m_ThreadIdx;
	
	bool          m_bSceneCreated;         // is scene data created to the vgs Scene
	bool          m_bAllImageCreated;      // is all image files created to the vgs texture 

	vector<CImagePosInfo> m_vImagePosInfo;       // n3d��ͼƬ�ļ����б�
	set<CThreadImageInfo> m_setThreadImageInfo;  // �̺߳�ͼ��Ķ�Ӧ��ϵ

	// ��һ����ͼ�ļ���Ϣ
	bool ReadOneImageInfo(CImagePosInfo& imagePos, HINTERNET hURL);


	// �����Ƿ��������
	bool IsSceneDwonloaded();

	// �����Щ�ļ��������
	void CheckDownloaded(const CThreadImageInfo& info, DWORD downloadedBytes, vector<int> &vDownloadedImgIdx);
};