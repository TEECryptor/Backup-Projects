/////////////////////////////////////////////////////////////////////////////////////////
// ��  ��: CDownLoadWebFile
// ��  ��: ͨ��CDownLoadWebFile���Web�������������ļ����࣬
//		   ͨ��CGetN3DFileInfo��������ص�N3D�ļ���������Ԥ��Ⱦ���С�
// ��  ��: lixin
// ��  ��: 2006-10-17
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CDOWNLOADN3DFILE_H
#define _CDOWNLOADN3DFILE_H

#include "..\VGSATLHeader.h"
#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#include "..\\VGSATLHeader.h"
#include "N3D_Def.h"

#include "..\\CSysFunc.h"
#include "GetN3DFileInfo_5.h"
#include "..\\vgs2dll.h"

//#include "..\\VGSSingelton.h"

using namespace std;
/*
//�Զ�����Ϣ
#define WM_DOWNLOADN3D_PROC		(WM_USER + 118)		//�ļ����ع����з��͸������ڵ���Ϣ
*/
//���ڷ��͸���������Ϣʱ�����Ĳ���
enum 
{	
	DOWN_TYPE,					//�������ݵ����ͣ�ǰ̨0�ͺ�̨1����
	HEADER_ISDOWN,				//Header��Ϣ�����Ƿ����
	SCENE_ISDOWN,				//������Ϣ�����Ƿ����
	ONETEX_ISDOWN,				//һ����ͼ��Ϣ�����Ƿ����
	ALLTEX_ISDOWN,				//������ͼ��Ϣ�����Ƿ����
	ONEIMAGE_ISDOWN,			//һ��Image��Ϣ�����Ƿ����
	ALLIMAGE_ISDOWN,			//����Image��Ϣ�����Ƿ����
	ALL_ISDOWN,					//����N3D��Ϣ�����Ƿ����
	IS_DOWN,					//�����������ݣ���ʾ���ؽ���
	TERMINATED					//�����̹߳ر�
};

// ����״̬
enum VGS_DOWNLOAD_STATUS
{	
	VGS_DOWNLOAD_STATUS_NONE = 0,
	VGS_DOWNLOAD_STATUS_VERSION,            // ���ڻ�ȡ�ļ��汾��
	VGS_DOWNLOAD_STATUS_HEADER,				// ���������ļ�ͷ
	VGS_DOWNLOAD_STATUS_SCENE,				// �������س���
	VGS_DOWNLOAD_STATUS_IMAGE,			    // ��������ͼ��
	VGS_DOWNLOAD_STATUS_ALLISDOWN,			// ����N3D��Ϣ�������
	VGS_DOWNLOAD_STATUS_TERMINATED			// �����̹߳ر�
};

class CDownLoadN3DFile  : public CVGSGetCore
{
	//----------------------------------------------------------------------------
	//Load N3D�����ı���
	//----------------------------------------------------------------------------
public:
	//char					name[32];				//���ض��������,JSSet
	//char					file[512];				//�ļ���ַ,JSSet
	string					m_sName;
	string					m_sFileFullName;         // �����ļ����������·��
	INT						index;					//��ǰ���ض���ı��

	//LoadN3DFileΪ���ز����ص������� type = 0;
	//PreloadN3DFileΪ���ص������ص����� type = 1;
	INT						type;		

	VGS_DOWNLOAD_STATUS     m_downloadStatus;              // ����״̬
	
	DWORD					lastTimer;				//ʱ��

	//----------------------------------------------------------------------------
	//����������ص�ϵͳ����
	//----------------------------------------------------------------------------
private:
	HINTERNET				m_hInternetSession;		//����Ự����
    HINTERNET				m_hURL;					//URL���	
	HWND					m_hWnd;					//��������
	DWORD					m_threadID;				//��ǰ�����߳�ID

	char					m_strHost[256];			//������http��ַ
	char					m_strFloder[256];		//�ļ���Ŀ¼
	char					m_strTmpFloder[256];	//IE��ʱĿ¼

	// char					m_strNetFileName[256];	//Ҫ���ص��ļ�
	string                  m_strNetFileName;
	DWORD					m_bufSize;				//ÿ�����ص����ݳߴ磬Ĭ��Ϊ1024Byte
	char					m_pBuf[1024];			//ÿ�����ص����ݻ���

	bool					m_bConnected;			//�Ƿ��Ѿ���http����������

	//FILE*					m_pLocalFile;			//���ص����ص��ļ�ָ��
	// char					m_strLocalFileName[256];//���غ�������ļ���
	string                  m_strLocalFileName;
	bool					m_localFileClosed;		//����д�뱾�ص��ļ��Ƿ��Ѿ�close

	// char					m_err[256];				//����error��Ϣ�Ļ���
	// string                  m_sErr;

	int						m_reconnectMaxCnt;		//������صĹ����ж��ߣ��������ӵ�������
	int						m_curReconnectIndex;	//��ǰ���ӵĴ���

	CDownloadN3DFileManager*  m_pDownloadN3DFileManager; // ���ع�����ָ��

public:
	HANDLE					m_threadHandle;			//��ǰ�̵߳�Handle
	bool					m_bTerminated;			//�Ƿ����ع�����ֹ(1.���ع��̷����쳣2.�����ļ����)

	BOOL					m_bExecuteOver;			//�߳�ִ�е�Execute��������
	//----------------------------------------------------------------------------
	//N3D�ļ�������صı���
	//----------------------------------------------------------------------------
public:
	FLOAT					VGSPlayerVer;			//VGS Player ���沥�����İ汾��
	BOOL					m_bOutDate;				//�����Ѿ�����

	//File something
	DWORD					curPos;					//��ǰ����N3D�ļ���ƫ����=�Ѿ����ص������ֽڳ���
	DWORD					m_fileSize;				//���ص��ļ��Ĵ�С

	//Version Something
	DWORD					m_N3DVersion;				//�ļ��İ汾��
	BOOL					m_bVersionInfoDown;		//�汾��������Ƿ��Ѿ�������
	BOOL					m_bFileVerErr;			//�ļ��汾����

	//Header Something...
    N3D_Header              m_Header_5_1;           //5.1�汾�ļ�ͷ
	BOOL					m_bHeaderDown;			//�ļ�ͷ�����Ƿ��Ѿ�������

	//Scene Something...
	//DWORD					m_ZipSceneDataSize;		//ѹ����ĳ������ݴ�С
	DWORD					m_SceneDataDownedByte;  //�Ѿ����صĳ������ݴ�С
	BYTE*					pSceneDataBuf;			//����N3D�ļ��������ݵ����ػ��壬��ȡhttp�ϵ�N3D�ļ��ĳ������ݶε� pSceneDataBuf
	BOOL					m_bSceneDown;			//��ǰN3D�ļ��ĳ�����Ϣ�Ƿ��Ѿ�������


	//Texture Something...
	DWORD					m_TexCnt;				//N3D�ļ��е���ͼ����
	DWORD					m_DownedTexCnt;			//��ǰ�Ѿ����ص���ͼ����
	BOOL					m_bAllTexDown;			//��ǰN3D�ļ��Ƿ�������ȫ����ͼ
	BOOL					m_bGetTexCnt;			//�Ƿ��õ�ǰN3D�ļ��е���ͼ����

	BOOL					m_bGetCurTexInfo;		//�Ƿ�õ���ǰ����ͼ�Ĳ�����Ϣ
	DWORD					m_CurTexDataDownedByte; //��ǰ��ͼ�ļ��Ѿ����ص����ݴ�С
	BOOL					m_bCurTexDown;			//��ǰN3D�ļ��Ƿ�������һ����������ͼ

	//Image Something...
	DWORD					m_ImageCnt;				//ͼ�����
	BOOL					m_bGetImageCnt;			//�Ƿ����˵�ǰImage����ĸ���
	DWORD					m_DownedImageCnt;		//��ǰ�Ѿ����ص�ͼ�����
	BOOL					m_bAllImageDown;		//��ǰN3D�ļ��Ƿ�������ȫ��ͼ����Դ
    N3D_5_1_ZipImageData*   pImageDataBuf;
	N3D_5_1_ZipImageData*	pImageDataBuf_5_1;		//��ȡhttp�ϵ�N3D�ļ�������Image���ݶ�
	
	BOOL	                m_bGetImageNameSize;
	BOOL                    m_bGetImageName;
	UINT                    m_CurImageNameSize;
	BOOL					m_bGetCurImageInfo;		//�Ƿ�õ��˵�ǰImage���ݲ���
	DWORD					m_CurImageDataDownedByte; //��ǰͼ�������Ѿ����ص����ݴ�С
	BOOL					m_bCurImageDown;		//��ǰN3D�ļ��Ƿ�������һ��Image


	DWORD					m_CurN3D_3_1_DownedByte; //��ǰN3D_3_1�ļ��Ѿ����ص��ֽ���
	bool					m_bAllDown;				//��ǰN3D�ļ��Ƿ��Ѿ�ȫ��������
	//----------------------------------------------------------------------------
	//����N3D�ļ��Ķ���
	//----------------------------------------------------------------------------
public:
	DWORD					m_pScene;				//��������ָ��
	DWORD			m_pSceneResource;		//������Դ����
	CGetN3DFileInfo_5	    N3DFile;				//N3D���ݽ�������

	//�����߳�ͬ������ָ�룬��CVGS2�ഫ��			
	CRITICAL_SECTION*		m_pCtrl_cs;	
	CRITICAL_SECTION*		m_pRender_cs;

	//----------------------------------------------------------------------------
	//���ؼ��������ݵķ���
	//----------------------------------------------------------------------------
public:
	// ��ʼ����
	void                    StartDownLoad();
	//����http�ļ�
	bool					DownLoadHttpFile(const string &strHttpFile, const string &strDestFile, HWND hWnd, DWORD pSceneResource);
	//����һ�������߳�
	bool					CreateDownThread();
	//�ж��ļ��Ƿ����
	int						JudgeFileLink(HINTERNET url);

	//��ȡ����N3D�ļ����ݵ��ڴ�ͱ����ļ�
	BOOL					ReadN3DFile();

	//3.1���ϰ汾ֱ�Ӷ�ȡ����N3D�ļ���ȫ����������ټ��볡��
	//BOOL					ReadN3D_3_1_File();
	
	//�ж�N3D�ļ��İ汾��
	DWORD					GetN3DVersion();

	//����N3D���ļ�ͷ
	BOOL					ReadN3DHeader();

	//����N3D�ĳ�������
	BOOL					ReadN3DScene();
	//����3.1���°汾��N3D��������
//	BOOL					ReadN3D_3_0_Scene();
	//����3.1�汾��N3D��������
	//BOOL					ReadN3D_3_1_Scene();

	//���볡���ľ�̬��ͼ����
	//BOOL					ReadN3DTexture();
	//����3.1���°汾��N3D��ͼ����
	//BOOL					ReadN3D_3_0_Texture();
	//����3.1�汾��N3D��ͼ����
	//BOOL					ReadN3D_3_1_Texture();

	//���볡���е�Image����
	BOOL					ReadN3DImage();
	//����3.1���°汾��Image����
	//BOOL					ReadN3D_3_0_Image();
	//����3.1�汾��Image����
	//BOOL					ReadN3D_3_1_Image();

	//���뵱ǰ�����еĶ�̬��ͼ����
	//BOOL					ReadN3DDyTex();
	//����3.1�汾�Ķ�̬��ͼ����
	//BOOL					ReadN3D_3_1_DyTex();


	//�߳�ִ�к���
	DWORD					Execute();
	//��ֹ�����߳�
	void					EndDownThread();
	//�ͷ��ڴ漰�����Դ
	void					Release();


	//��N3D�ļ����ض�ӦJS Callback�����Ƿ��Ѿ�����
	BOOL					bSceneIsDownInvoked;  
	BOOL					bAllIsDownInvoked;

public:
	CDownLoadN3DFile(CDownloadN3DFileManager*  pDownloadN3DFileManager, CRITICAL_SECTION* pCtrl_cs,  CRITICAL_SECTION* pRender_cs);
	~CDownLoadN3DFile();

	virtual VOID   SetVGSCore(CVGS2DLL *pVGSCore);

};

#endif