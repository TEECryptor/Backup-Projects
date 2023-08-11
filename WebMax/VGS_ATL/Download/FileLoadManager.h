#pragma once

#include "..\\..\\VGSCoreV3\\VGSHeader.h"
#include <list>

using namespace std;

// ��������
enum VGS_FILELOADREQUEST_TYPE
{
	VGS_FILELOADREQUEST_TYPE_LOADN3D = 0,
	VGS_FILELOADREQUEST_TYPE_LOADA3D,
	VGS_FILELOADREQUEST_TYPE_LOADFLASH2D,
	VGS_FILELOADREQUEST_TYPE_LOADFLASHTEXTURE,
	VGS_FILELOADREQUEST_TYPE_LOADIMAGETEXTURE,
	VGS_FILELOADREQUEST_TYPE_NONE
};

// �������ݽṹ
struct FileLoadRequest
{
	UINT ReqID;   
	VGS_FILELOADREQUEST_TYPE type;
	//string sFileName;
	//HWND hwnd;        // ������Ϣ�Ĵ���
	void* pProcessor;   // ������ָ��
	void* pDataInfo;    // Ҫ���������ָ��
};

class CFileLoadManager
{
private:	

	std::list <FileLoadRequest> m_vRequestQueue; // �������

	bool m_bTreadTerminated;
	UINT m_RequestID;
	HANDLE m_ThreadHandle; // �̶߳���

	bool CreateLoadThread();  // ��������/ж���߳�
	void ProcessNextRequest();

	CRITICAL_SECTION			m_ResQue;	 // �߳�ͬ��
public:

	CFileLoadManager();
	~CFileLoadManager();
	void Release();	
	void Init();   // ��ʼ��
	UINT AddRequest(FileLoadRequest &req); // ���һ������
	void StopThread(); 
	void ThreadFunc(); // ����/ж���̹߳���// ɾ����������
	void ClearAllRequests();
};