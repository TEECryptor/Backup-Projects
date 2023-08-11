// CDownLoadFile.h: interface for the CDownLoadFile class.
// ������ͨ�ļ�����
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDOWNLOADFILE_H__35033E06_69C7_448A_AE2B_4C3437C02FF6__INCLUDED_)
#define AFX_CDOWNLOADFILE_H__35033E06_69C7_448A_AE2B_4C3437C02FF6__INCLUDED_

#include <stdio.h>
#include <windows.h>
#include <wininet.h>
#include "..\\VGSATLHeader.h"
#include "..\\CSysFunc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�Զ�����Ϣ
#define WM_DOWNLOADFILE_PROC		(WM_USER + 120)		//�ļ����ع����з��͸������ڵ���Ϣ
enum downloadType
{
	TYPE_LOGOFILE,
	TYPE_IMAGEFILE,
	TYPE_OTHERFILE
};


class CDownLoadFile  
{
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

	char					m_strNetFileName[256];	//Ҫ���ص��ļ�
	DWORD					m_bufSize;				//ÿ�����ص����ݳߴ磬Ĭ��Ϊ1024Byte
	char					m_pBuf[1024];			//ÿ�����ص����ݻ���

	bool					m_bConnected;			//�Ƿ��Ѿ���http����������

	//FILE*					m_pLocalFile;			//���ص����ص��ļ�ָ��
	char					m_strLocalFileName[256];//���غ�������ļ���
	bool					m_localFileClosed;		//����д�뱾�ص��ļ��Ƿ��Ѿ�close

	char					m_err[256];				//����error��Ϣ�Ļ���

public:
	HANDLE					m_threadHandle;			//��ǰ�̵߳�Handle
	bool					m_bTerminated;			//�Ƿ����ع�����ֹ(1.���ع��̷����쳣2.�����ļ����)

	BOOL					m_bExecuteOver;			//�߳�ִ�е�Execute��������
  
	//File something
	char                    file[512];
	UINT                    index;
	DWORD					curPos;					//��ǰ����N3D�ļ���ƫ����=�Ѿ����ص������ֽڳ���
	DWORD					m_fileSize;				//���ص��ļ��Ĵ�С
	BYTE*					m_fileBuf;				//�ļ��Ļ���

	BOOL					m_bLogoFile;			//�Ƿ���Logo�ļ�
	BOOL					m_bAllDown;				//���е����ݶ��Ѿ�����
	downloadType            m_DownType;             //�����ļ�����������

	DWORD                   m_DownForObj;           //�������������ڵĶ���ָ��
	
	//----------------------------------------------------------------------------
	//���ؼ��������ݵķ���
	//----------------------------------------------------------------------------
public:
	//����һ���µ�����Ự����
	// BOOL					CreateInternetSession();
	void                    SetInternetSession(HINTERNET hInternetSession){m_hInternetSession = hInternetSession;}
	//ɾ������Ự����
	VOID					ReleaseSession();
	//����http�ļ�
	bool					DownLoadHttpFile(char* strHttpFile, char* strDestFile, HWND hWnd, INT type);
	//����һ�������߳�
	bool					CreateDownThread();
	//�ж��ļ��Ƿ����
	int						JudgeFileLink(HINTERNET url);

	//��ȡ�����ļ����ݵ��ڴ�ͱ����ļ�
	BOOL					ReadFile();
	//�߳�ִ�к���
	DWORD					Execute();
	//��ֹ�����߳�
	void					EndDownThread();
	//�ͷ��ڴ漰�����Դ
	void					Release();

public:
	CDownLoadFile();
	~CDownLoadFile();

};

#endif // !defined(AFX_CDOWNLOADFILE_H__35033E06_69C7_448A_AE2B_4C3437C02FF6__INCLUDED_)
