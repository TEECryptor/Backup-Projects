// CDownLoadFile.cpp: implementation of the CDownLoadFile class.
//
////////////////////////////////////////////////////////////////////// 

#include "stdafx.h"
#include "CDownLoadFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownLoadFile::CDownLoadFile()
{
	m_bTerminated = FALSE;			//�Ƿ����ع�����ֹ(1.���ع��̷����쳣2.�����ļ����)
	m_bLogoFile = FALSE;
	m_bExecuteOver = FALSE;			//�߳�ִ�е�Execute��������

	curPos = 0;						//��ǰ����N3D�ļ���ƫ����=�Ѿ����ص������ֽڳ���
	m_fileSize = 0;					//���ص��ļ��Ĵ�С
	m_fileBuf = NULL;				//�ļ��Ļ���
	m_bufSize = 1024;

	m_DownForObj = NULL;

	m_bAllDown = FALSE;				//���е����ݶ��Ѿ�����
	ZeroMemory(file, sizeof(file)); //��ʼ�����ص�ַ

	//����һ���µ�����Ự����
	//CreateInternetSession();
}

CDownLoadFile::~CDownLoadFile()
{

}

//-----------------------------------------------------------------------------
// ������: CreateInternetSession
// ��  ��: 
// ��  ;: ��������Ự����
//-----------------------------------------------------------------------------
//BOOL CDownLoadFile::CreateInternetSession()
//{
//	��������Ự����
//	CHAR AppName[256];
//	CHAR strModuleName[256];
//	GetModuleFileName(NULL, LPWCH(strModuleName), 256);
//	GetAppName(AppName, strModuleName);
//	m_hInternetSession = InternetOpen(LPWCH(AppName) /*"Microsoft Internet Explorer"*/,
//                                    INTERNET_OPEN_TYPE_PRECONFIG,
//                                    NULL, NULL, 0);
//	if(m_hInternetSession == NULL) 
//	{
//		//::MessageBox(GetActiveWindow(), "Create Internet session error.", "Now creating Internet session...", MB_OK|MB_ICONERROR);
//		return FALSE;
//	}
//	return TRUE;
//}

//----------------------------------------------------------------
//	���ƣ�DownThreadFunc
//	��飺�̺߳���
//	������impl:i:�������
//	���أ�
//----------------------------------------------------------------
DWORD WINAPI DownLoadFileThreadFunc(LPVOID impl)
{
	return ((CDownLoadFile*)impl)->Execute();
}

//----------------------------------------------------------------
//	���ƣ�Execute
//	��飺�߳�ִ�к���
//	��������
//	���أ�
//----------------------------------------------------------------
DWORD CDownLoadFile::Execute()
{
	while(!m_bTerminated)
		ReadFile();

	EndDownThread();

	m_bExecuteOver = TRUE;

	return 0;
}

//��ȡ�����ļ�
BOOL CDownLoadFile::ReadFile()
{
	BOOL bResult = FALSE;
	DWORD dwBytesRead = 0;

	while(!m_bAllDown)
	{
		//������ݲ���1K
		if (m_fileSize <= 1024)
		{
			m_bufSize = m_fileSize;
			bResult = InternetReadFile(m_hURL,				// handle to URL
									  (LPSTR)m_pBuf,		// pointer to  buffer
									  (DWORD)m_bufSize,	    // size of buffer 
									  &dwBytesRead);		// pointer to var to hold return value
			if (bResult && (dwBytesRead == m_bufSize) )
			{
				//�������ݵ� m_fileBuf
				memcpy(m_fileBuf, m_pBuf, m_bufSize);

				//������ݻ���
				curPos += m_bufSize;
				ZeroMemory(m_pBuf, 1024);
				m_bufSize = 1024;  
				dwBytesRead = 0;

				//�����Ѿ�������
				m_bAllDown = TRUE;
				m_bTerminated = TRUE;

				//������Ϣ��������
				//if (m_bLogoFile)
				//	PostMessage(m_hWnd, WM_DOWNLOADFILE_PROC, TYPE_LOGOFILE, 0);
				//else
					PostMessage(m_hWnd, WM_DOWNLOADFILE_PROC, m_DownType, index);

				Sleep(0);

				//д�뱾���ļ�
				//if (m_pLocalFile != NULL)
				//	fwrite(m_fileBuf, m_bufSize, 1, m_pLocalFile);	
				
			}
			//����ʧ��
			else
				return FALSE;
		}
		//������ݴ���1024�ֽ�
		else
		{
			bResult = InternetReadFile(m_hURL,				// handle to URL
									   (LPSTR)m_pBuf,		// pointer to  buffer
									   (DWORD)m_bufSize,	// size of buffer
									   &dwBytesRead);		// pointer to var to hold return value	
			
			if (bResult && (m_bufSize == dwBytesRead))
			{
				//�жϵ�ǰN3D_3_1�����Ƿ��Ѿ�������--�Ѿ��������
				if (curPos == m_fileSize )
				{
					//������ݻ���
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;

					//�����Ѿ�������
					m_bAllDown = TRUE;
					m_bTerminated = TRUE;
				
					//������Ϣ��������
					//if (m_bLogoFile)
					PostMessage(m_hWnd, WM_DOWNLOADFILE_PROC, m_DownType, index);
					//else
						//PostMessage(m_hWnd, WM_DOWNLOADFILE_PROC, TYPE_OTHERFILE, 0);
					Sleep(0);

					//д�뱾���ļ�
					//if (m_pLocalFile != NULL)
					//	fwrite(pTexDataBuf[m_DownedTexCnt].Data, m_CurTexDataSize, 1, m_pLocalFile);	
				}
				//��û���������
				else
				{
					//�������ݵ�pBuf
					for (DWORD i = 0; i< m_bufSize; i++)
						m_fileBuf[curPos + i] = m_pBuf[i];
					
					//������ݻ���
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;

					//�жϵ�ǰ��ͼ�����Ƿ��Ѿ���Ҫ������ɣ����һ��ѭ��
					if (m_fileSize - curPos < m_bufSize)
						m_bufSize = m_fileSize - curPos;
					
					Sleep(0);
				}
			}
			//��������ʧ�ܣ��������ȴ���һ��ѭ��ʱ���Լ�������
			else
				return FALSE;	
		}
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// ������: CreateDownThread
// ��  ��: 
// ��  ;: ����һ�������߳�
//-----------------------------------------------------------------------------
bool CDownLoadFile::CreateDownThread()
{
	m_threadHandle = CreateThread(NULL, 0, DownLoadFileThreadFunc, (LPVOID)this, 0, &m_threadID);
	if (m_threadHandle == NULL) return false;
	return true;
}	

//-----------------------------------------------------------------------------
// ������: DownLoadHttpFile()
// ��  ��:  char* strHttpFile, �����ص��ļ���ַ    
//			char* strDestFile, ���浽���ص��ļ�·��
//		    HWND  hWnd,		   ��������
// ��  ;: ����http�ļ�
//-----------------------------------------------------------------------------
bool CDownLoadFile::DownLoadHttpFile(char* strHttpFile, char* strDestFile, HWND hWnd, INT type)
{
	if(strHttpFile == NULL) return false;

	//Ҫ�����ļ���������?
	if (strDestFile != NULL)
		strcpy(m_strLocalFileName, strDestFile);

	//--------------------------------------------------------------
	//���ݱ���
	//--------------------------------------------------------------
	strcpy(m_strNetFileName, strHttpFile);
	m_hWnd = hWnd;

	//--------------------------------------------------------------
	//���url����
	//--------------------------------------------------------------
	m_hURL = InternetOpenUrl(m_hInternetSession, 
							(m_strNetFileName), 
							NULL, 
							0, 
							INTERNET_FLAG_RESYNCHRONIZE|INTERNET_FLAG_NEED_FILE, 
							0);
	if(m_hURL == NULL)
	{
		//strcpy(m_err, "VGS can not connect to");
		//strcat(m_err, m_strNetFileName);
		//strcat(m_err, ". Please check your internet connection.");
		//MessageBox(NULL, m_err, "Now connect to remote file...error", MB_OK | MB_ICONERROR);
		return false;
	}

	/*
	//�½������ļ�����
	m_pLocalFile = fopen(m_strLocalFileName, "wb");
	m_localFileClosed = false;
	if (m_pLocalFile == NULL)
	{
		strcpy(m_err, "�򿪱����ļ� ");
		strcat(m_err, m_strLocalFileName);
		strcat(m_err, "ʧ�ܡ�");
		MessageBox(NULL, m_err, "�������ص��ļ�...����", MB_ICONASTERISK | MB_OK);
		return false;
	}
*/

	//--------------------------------------------------------------
	//�ж������ļ��Ƿ�������أ�����N3D�ļ���С
	//--------------------------------------------------------------
	m_fileSize = JudgeFileLink(m_hURL);
	if (m_fileSize <= 0)
	{
		//strcpy(m_err, "Now confirm size of file ");
		//strcat(m_err, m_strNetFileName);
		//strcat(m_err, " error");
		//MessageBox(NULL, m_err, "Analasys file size...error, the file is not exist.", MB_ICONASTERISK | MB_OK);
		return false;
	}

	//--------------------------------------------------------------
	//����һ�������߳�
	//--------------------------------------------------------------
	if (CreateDownThread() == false)
	{
		//strcpy(m_err, "CreateDownThread() -> error");
		//MessageBox(NULL, m_err, "Now create download thread...error", MB_ICONASTERISK | MB_OK);
		return false;
	}
	
	//��ʼ������
	m_fileBuf = new BYTE[m_fileSize];
	//--------------------------------------------------------------
	//�����������͵���Ϣ
	//--------------------------------------------------------------
	//PostMessage(m_hWnd, WM_DOWNLOAD_PROC, DOWN_TYPE, type);

	return true;
}

//-----------------------------------------------------------------------------
// ������: EndDownThread
// ��  ��: 
// ��  ;: ��ֹһ�������߳�
//-----------------------------------------------------------------------------
void CDownLoadFile::EndDownThread()
{
	//�Ͽ���������
    InternetCloseHandle(m_hURL);
	
	//�ͷŻỰ
	ReleaseSession();

	//ɾ���߳�
	DWORD ExitCode;
	if (GetExitCodeThread(m_threadHandle, &ExitCode))
		ExitThread(ExitCode);
	
}

//----------------------------------------------------------------
//	���ƣ�JudgeFileLink
//	��飺�ж������ļ��Ƿ����
//	������url:i:����
//	���أ���������ļ��������أ��򷵻��ļ��ߴ磬���򷵻�-1
//----------------------------------------------------------------
int CDownLoadFile::JudgeFileLink(HINTERNET url)
{	
	DWORD index = 0;
	DWORD readBytes = 32;
	char  buffer[32] = {0};
	int result = HttpQueryInfo(url, HTTP_QUERY_STATUS_CODE, buffer, &readBytes, &index);

	if(atoi(buffer) != HTTP_STATUS_OK)
		return 0;

	//��ȡ�ļ���С
	char  size[255] = {0};
	DWORD read = 255;
	result = HttpQueryInfo(url, HTTP_QUERY_CONTENT_LENGTH, size, &read, &index);
	int filesize = atoi(size);
	
	return filesize;
}

//----------------------------------------------------------------
// ������: ReleaseSession
// ��  ��: 
// ��  ;: �رռ��ͷ�����Ự����
//----------------------------------------------------------------
VOID CDownLoadFile::ReleaseSession()
{
	if(m_hInternetSession)
		InternetCloseHandle(m_hInternetSession);
}

//-----------------------------------------------------------------------------
// ������: Release()
// ��  ��: 
// ��  ;: �ͷ��ڴ漰�����Դ
//-----------------------------------------------------------------------------
void  CDownLoadFile::Release()
{
	SAFE_DELETE_ARRAY(m_fileBuf);
	//fclose(m_pLocalFile);
	//m_pLocalFile = NULL;
}