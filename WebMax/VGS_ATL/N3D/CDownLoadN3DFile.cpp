/////////////////////////////////////////////////////////////////////////////////////////
// ��  ��: CDownLoadN3DFile
// ��  ��: ��Web�������������ļ�����
// ��  ��: lixin
// ��  ��: 2006-06-19
/////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CDownLoadN3DFile.h"
#include "..\\Download\\VGSFileDownloadManager.h"
#include <time.h>

DWORD			DownLoad_lastTimer = 0;			//Timer���� 
DWORD			DowndLoad_SPF = 100;			//�೤ʱ����ִ��һ�����ع���

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownLoadN3DFile::CDownLoadN3DFile(CDownloadN3DFileManager*  pDownloadN3DFileManager, CRITICAL_SECTION* pCtrl_cs,  CRITICAL_SECTION* pRender_cs)
{
	m_pDownloadN3DFileManager = pDownloadN3DFileManager;

	//VGS Player�İ汾��
	VGSPlayerVer = N3D_CURRENT_V3DVERSION /*/ 10000.0; 5.1007f*/;

	m_pCtrl_cs = pCtrl_cs;
	m_pRender_cs = pRender_cs;
	//m_hInternetSession = hInternetSession;

	m_fileSize = 0;
	index = 0;
	//ÿ������1024�ֽڣ���ʼ�������ļ��Ļ���
	m_bufSize = 1024;
	ZeroMemory(m_pBuf, m_bufSize);
	// ZeroMemory(m_err, sizeof(m_err));

	//��ʼ������
	/*ZeroMemory(m_strNetFileName, sizeof(m_strNetFileName));
	ZeroMemory(m_strLocalFileName, sizeof(m_strLocalFileName));
	ZeroMemory(name, sizeof(name));
	ZeroMemory(file, sizeof(file));*/

	m_strNetFileName = "";
	m_strLocalFileName = "";
	m_sName = "";
	m_sFileFullName = "";

	//m_pLocalFile = NULL;

	pSceneDataBuf = NULL;
	
//	pTexDataBuf = NULL;

	pImageDataBuf = NULL;
	pImageDataBuf_5_1 = NULL;
	
	//pCubeMapBuf = NULL;

	//��ʼ������
	curPos = 0;
	m_bConnected = false;
	m_bTerminated = false;
	m_bExecuteOver = FALSE;
	
	m_bVersionInfoDown = FALSE;
	m_bHeaderDown = FALSE;
	m_bSceneDown = FALSE;

	m_SceneDataDownedByte = 0;
	m_CurN3D_3_1_DownedByte = 0;

	m_TexCnt = 0;
	m_DownedTexCnt = 0;	
	m_bGetTexCnt = false;
	m_bAllTexDown = false;

	m_bGetCurTexInfo = false;
	m_CurTexDataDownedByte = 0;
	m_bCurTexDown = false;

	m_ImageCnt = 0;
	m_CurImageNameSize = -1;
	m_DownedImageCnt = 0;
	m_CurImageDataDownedByte = 0;

	m_bGetCurImageInfo = false;
	m_bGetImageCnt = false;
	m_bGetImageNameSize = false;
	m_bGetImageName = false;
	m_bCurImageDown = false;
	m_bAllImageDown = false;

	m_bAllDown = false;
	m_localFileClosed = false;

	m_reconnectMaxCnt = 1000;	
	m_curReconnectIndex = 0;
	
	bSceneIsDownInvoked = FALSE;
	bAllIsDownInvoked = FALSE;
	m_bOutDate = FALSE;
	m_bFileVerErr = FALSE;
	m_bTerminated = FALSE;

	//m_CubeMapCnt = 0;	
	//m_bGetCubeMapCnt = 0;
	//m_DownedCubeMapCnt = 0;
	//m_bAllCubeMapDown = 0;

	//����N3DFile����ĳ���	
	//m_pScene = pScene;
	// N3DFile.m_pScene = pScene;
	// N3DFile.m_pCtrlCS = pCtrl_cs;
	// N3DFile.m_pRenderCS = pRender_cs;
/*
	N3DFile_3_1.m_pScene = pScene;
	N3DFile_3_1.m_pCtrlCS = pCtrl_cs;
	N3DFile_3_1.m_pRenderCS = pRender_cs;
	//Add by Baojiahui
	N3DFile_5.m_pScene = pScene;
	N3DFile_5.m_pCtrlCS = pCtrl_cs;
	N3DFile_5.m_pRenderCS = pRender_cs;*/

	m_downloadStatus = VGS_DOWNLOAD_STATUS_NONE;
}

CDownLoadN3DFile::~CDownLoadN3DFile()
{
	m_pCtrl_cs = 0;
	m_pRender_cs = 0;
}

//----------------------------------------------------------------
//	���ƣ�DownThreadFunc
//	��飺�̺߳���
//	������impl:i:�������
//	���أ�
//----------------------------------------------------------------
DWORD WINAPI DownThreadFunc(LPVOID impl)
{
	return ((CDownLoadN3DFile*)impl)->Execute();
}

//-----------------------------------------------------------------------------
// ������: CreateDownThread
// ��  ��: 
// ��  ;: ����һ�������߳�
//-----------------------------------------------------------------------------
bool CDownLoadN3DFile::CreateDownThread()
{
	m_threadHandle = CreateThread(NULL, 0, DownThreadFunc, (LPVOID)this, 0, &m_threadID);
	if (m_threadHandle == NULL) return false;
	return true;
}	

//-----------------------------------------------------------------------------
// ������: EndDownThread
// ��  ��: 
// ��  ;: ��ֹһ�������߳�
//-----------------------------------------------------------------------------
void CDownLoadN3DFile::EndDownThread()
{
	//�Ͽ���������
    InternetCloseHandle(m_hURL);
	
	//ɾ���߳�
	DWORD ExitCode;
	if (GetExitCodeThread(m_threadHandle, &ExitCode))
		ExitThread(ExitCode);
}
//----------------------------------------------------------------
//	���ƣ�Execute
//	��飺�߳�ִ�к���
//	��������
//	���أ�
//----------------------------------------------------------------
DWORD CDownLoadN3DFile::Execute()
{
	while (!m_bTerminated)
	{	
		if (GetTickCount() - DownLoad_lastTimer > DowndLoad_SPF)
		{
			DownLoad_lastTimer = GetTickCount();
			ReadN3DFile();
		}
		else
		{
			Sleep(0);
		}
	}

	//�Ͽ���������
    InternetCloseHandle(m_hURL);
	

	//N3D�ļ���Ȩ����
	if (m_bOutDate == TRUE)
	{
		MessageBox(GetActiveWindow(),
			TEXT("    �ļ�û��ȡ�úϷ���Ȩ����Ȩ�ѹ��ڣ�WebMax���ϵͳΪ�й����ҷ�����ר��������\n�ܵ����ɱ�����������WebMax��������̺Ϸ���Ȩ���ܷ���WebMax��ά��ҳ��"), 
			TEXT("��Ȩ���Ϸ����Ѿ�����..."),
			MB_OK | MB_ICONEXCLAMATION);
		// CVGS2DLL::getSingleton().RestoreScene();
		// m_pScene->Reset();
	}

	//E3D�ļ��汾���ߣ����汾�����޷���
	if (m_bFileVerErr == TRUE)
	{	
		MessageBox(GetActiveWindow(),
			TEXT("    ����ϵͳ��װ��WebMax Player�汾��Ҫ�����������http://www.suntoward.com��\n��װ���°汾��WebMax Player��Ȼ�������б���ҳ��"), 
			TEXT("WebMax Player �汾��Ҫ����"),
			MB_OK | MB_ICONEXCLAMATION);
		// m_pScene->Reset();
		// CVGS2DLL::getSingleton().RestoreScene();
	}
	//�ͷ���Դ
	Release();
	
	m_bExecuteOver = TRUE;

	return 0;
}

//�ж�N3D�ļ��İ汾��
DWORD CDownLoadN3DFile::GetN3DVersion()
{
	m_downloadStatus = VGS_DOWNLOAD_STATUS_VERSION;

     BOOL bResult = FALSE;		//���ط����ķ���ֵ
     DWORD dwBytesRead = 0;		//���ص���ʵ���ݵĳ���		
 	
 	//��ȡ4���ֽڵ����ݣ��ж�N3D�ļ��İ汾��
 	m_bufSize = 4;
 	bResult = InternetReadFile(m_hURL, (LPSTR)m_pBuf, m_bufSize, &dwBytesRead);
 	if (bResult && (dwBytesRead == m_bufSize) )
 	{
 		//�������ݵ�
 		memcpy(&m_Header_5_1.Version, m_pBuf, m_bufSize);
 		//�ϰ汾N3D, SUNT== 2896867755
 		
 		if (m_Header_5_1.Version >= 51000 && m_Header_5_1.Version <= N3D_CURRENT_V3DVERSION /*52000*/)
 		{
 			m_N3DVersion = m_Header_5_1.Version;
 			
 			//������ݻ��棬�������
 			curPos += dwBytesRead;
 			dwBytesRead = 0;
 			ZeroMemory(m_pBuf, 1024);
 			m_bufSize = 1024;
 			//�汾��Ϣ�Ѿ�����
 			m_bVersionInfoDown = TRUE;	
 			// return (m_Header_5_1.Version/10000.0f);
 		}		
 		else //���Ϸ��汾
 		{
			m_N3DVersion = 0xffffffff;
 			// m_pScene->m_bAuthorized = FALSE;
 			//return 3.0f;
 		} 		
 	}
	
	return m_N3DVersion;
}

//����N3D���ļ�ͷ
BOOL CDownLoadN3DFile::ReadN3DHeader()
{
	m_downloadStatus = VGS_DOWNLOAD_STATUS_HEADER;

     BOOL bResult = FALSE;		//���ط����ķ���ֵ
     DWORD dwBytesRead = 0;		//���ص���ʵ���ݵĳ���	
 
 	if (!m_bHeaderDown)
 	{
 		//5.1�汾��N3D�ļ����ļ�ͷ
 		if (m_N3DVersion >= 51000 && m_N3DVersion <= N3D_CURRENT_V3DVERSION/*52000*/)
 		{
 			m_bufSize = 92;
 			BYTE* tempBuf = new BYTE[m_bufSize];
 			bResult = InternetReadFile(m_hURL, (LPSTR)tempBuf, m_bufSize, &dwBytesRead);
 			if (bResult && (dwBytesRead == m_bufSize) )
 			{
 				//��Ȩ���˵�����
 				memcpy(&m_Header_5_1.RegInfo, tempBuf, sizeof(m_Header_5_1.RegInfo));
 				tempBuf += sizeof(m_Header_5_1.RegInfo);
 					
 				//�������ݵ���ʼָ��
 				memcpy(&m_Header_5_1.SceneDataPtr, tempBuf, sizeof(m_Header_5_1.SceneDataPtr));
 				tempBuf += sizeof(m_Header_5_1.SceneDataPtr);
         
 				//ѹ����ĳ������ݳ���
 				memcpy(&m_Header_5_1.SceneZipDataLen, tempBuf, sizeof(m_Header_5_1.SceneZipDataLen));
 				tempBuf += sizeof(m_Header_5_1.SceneZipDataLen);
 				//ԭ�������ݵĳ���
 				memcpy(&m_Header_5_1.OriginSceneDataLen, tempBuf, sizeof(m_Header_5_1.OriginSceneDataLen));
 				tempBuf += sizeof(m_Header_5_1.OriginSceneDataLen);
 				//��ͼ���ݵ���ʼָ��
 				memcpy(&m_Header_5_1.ImageDataPtr, tempBuf, sizeof(m_Header_5_1.ImageDataPtr));
 				tempBuf += sizeof(m_Header_5_1.ImageDataPtr);
 				//��ͼ���ݵĳ���
 				memcpy(&m_Header_5_1.ImageDataLen, tempBuf, sizeof(m_Header_5_1.ImageDataLen));
 				tempBuf += sizeof(m_Header_5_1.ImageDataLen);
 				//reserve
 				memcpy(&m_Header_5_1.Reserved, tempBuf, sizeof(m_Header_5_1.Reserved));
 				tempBuf += sizeof(m_Header_5_1.Reserved);
 
 				//�ͷŻ���
 				tempBuf -= 92;
 				SAFE_DELETE_ARRAY(tempBuf);
 
 				//������ݻ���
 				curPos += dwBytesRead;
 				dwBytesRead = 0;
 				ZeroMemory(m_pBuf, 1024);
 				m_bufSize = 1024;
 
 				//�ļ�ͷ�Ѿ�����
 				m_bHeaderDown = TRUE;

				N3DFile.SetHeaderInfo(m_Header_5_1);
 
 				//д�뱾���ļ�
 				//if (m_pLocalFile != NULL)
 				//	fwrite(&m_Header_3_0, sizeof(m_Header_3_0), 1, m_pLocalFile);
 
 				//������Ϣ��������
				PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, HEADER_ISDOWN, index);
 
 				return TRUE;
 			}
 			//����ʧ��
 			else
 			{
 				//�ͷŻ���
 				SAFE_DELETE_ARRAY(tempBuf);
 				return FALSE;
 			}
 		}
 	}
	return FALSE;
}

//����3.1���°汾��N3D��������
BOOL CDownLoadN3DFile::ReadN3DScene()
{
	m_downloadStatus = VGS_DOWNLOAD_STATUS_SCENE;

     BOOL bResult = FALSE;		//���ط����ķ���ֵ
     DWORD dwBytesRead = 0;		//���ص���ʵ���ݵĳ���	
 
 	if (pSceneDataBuf == NULL)
 	{
 		pSceneDataBuf = new BYTE[m_Header_5_1.SceneZipDataLen];
 		ZeroMemory(pSceneDataBuf, m_Header_5_1.SceneZipDataLen);
 	}
 
 	//����������ݲ���1K
 	if (m_Header_5_1.SceneZipDataLen <= 1024)
 	{
 		m_bufSize = m_Header_5_1.SceneZipDataLen;
 		bResult = InternetReadFile(m_hURL,				// handle to URL
 								  (LPSTR)m_pBuf,		// pointer to  buffer
 								  (DWORD)m_bufSize,	    // size of buffer 
 								  &dwBytesRead);		// pointer to var to hold return value
 		if (bResult && (dwBytesRead == m_bufSize) )
 		{
 			//�������ݵ�pBuf
 			memcpy(pSceneDataBuf, m_pBuf, m_bufSize);
 
 			//������ݻ���
 			curPos += m_bufSize;
 			ZeroMemory(m_pBuf, 1024);
 			m_bufSize = 1024;  
 			dwBytesRead = 0;
 
 			//�����Ѿ�������
 			m_bSceneDown = true;
 			m_SceneDataDownedByte = curPos;
 
 			// �������ѹ��������...
 			if (N3DFile.UnzipSceneData(pSceneDataBuf, 
 									   m_Header_5_1.SceneZipDataLen,
 									   m_Header_5_1.OriginSceneDataLen ))
 			{
 				if (!m_bTerminated)
 				{
 					//��ģ�Ͳ��뵽����ģ����Ⱦ����
 					//EnterCriticalSection(m_pRender_cs);
 					// INT result = N3DFile.ConstructScene(name, m_pSceneResource);
					N3DFile.ConstructScene("");
 					//LeaveCriticalSection(m_pRender_cs);
 				}
 				else //�߳���ֹ
				{
					return FALSE;
				}
 			}
 			//��ѹʧ��
 			else
 			{
 				::MessageBox(GetActiveWindow(), 
 							("�������ݶ�ȡ����, VGS���潫�Զ��رա�"), 
 							("�������ݶ�ȡʧ��..."), 
 							MB_OK|MB_ICONINFORMATION);
 
 				m_bTerminated = TRUE;
 
 				return FALSE;
 			}
 			
 			//������Ϣ��������
			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, SCENE_ISDOWN, index);
 
 			//д�뱾���ļ�
 			//if (m_pLocalFile != NULL)
 			//	fwrite(pSceneDataBuf, m_Header_3_0.SceneZipDataLen, 1, m_pLocalFile);
 		}
 		else//��������ʧ��
 			return FALSE;
 	}
 	else//�������ݴ���1K
 	{
 		//��ȡN3D->Scene��Ϣ,���ö����ĳ�����Ϣ����
 		while(!m_bSceneDown)
 		{
 			bResult = InternetReadFile(m_hURL,				// handle to URL
 									   (LPSTR)m_pBuf,		// pointer to  buffer
 									   (DWORD)m_bufSize,	// size of buffer
 									   &dwBytesRead);		// pointer to var to hold return value	
 			
 			if (bResult && (dwBytesRead == m_bufSize) )
 			{
 				//�жϳ��������Ƿ��Ѿ�������--�Ѿ�������
 				if (m_SceneDataDownedByte == m_Header_5_1.SceneZipDataLen)
 				{
 					//������ݻ���
 					curPos += m_bufSize;
 					ZeroMemory(m_pBuf, 1024);
 					m_bufSize = 1024;  
 					dwBytesRead = 0;
 					
 					//���������Ѿ��������
 					m_bSceneDown = TRUE;
 					m_SceneDataDownedByte = curPos;
 
 					//�������ѹ��������...
 					if (N3DFile.UnzipSceneData(pSceneDataBuf, 
 											   m_Header_5_1.SceneZipDataLen,
 											   m_Header_5_1.OriginSceneDataLen ))
 					{
 						if (!m_bTerminated)  //�����ǰ�߳�û�б��ص�
 						{
 							//��ģ�Ͳ��뵽����ģ����Ⱦ����
 							//EnterCriticalSection(m_pRender_cs);
 							//����������������
 							INT result = N3DFile.ConstructScene(""/*name, m_pSceneResource*/);
 							//LeaveCriticalSection(m_pRender_cs);
 						}
 						else //�߳���ֹ
 							return FALSE;
 					}
 					else//��ѹʧ��
 					{
 						::MessageBox(GetActiveWindow(), 
 									("�������ݶ�ȡ����, VGS���潫�Զ��رա�"), 
 									("�������ݶ�ȡʧ��..."), 
 									MB_OK|MB_ICONINFORMATION);
 
 						m_bTerminated = TRUE;
 						return FALSE;
 					}
 
 					//������Ϣ��������
 					PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, SCENE_ISDOWN, index);
 
 					//д�뱾���ļ�
 					//if (m_pLocalFile != NULL)
 					//	fwrite(pSceneDataBuf, m_Header_3_0.SceneZipDataLen, 1, m_pLocalFile);
 				}
 				//û��������
 				else
 				{	
 					if (!m_bTerminated)  //�����ǰ�߳�û�б��ص�
 					{	
 						//�������ݵ�pBuf
 						for (DWORD i = 0; i< m_bufSize; i++)
 							pSceneDataBuf[m_SceneDataDownedByte + i] = m_pBuf[i];
 						
 						m_SceneDataDownedByte += dwBytesRead;
 						
 						//������ݻ���
 						curPos += m_bufSize;
 						ZeroMemory(m_pBuf, 1024);
 						m_bufSize = 1024;  
 						dwBytesRead = 0;
 						
 						//�жϳ�����Ϣ�Ƿ�Ҫ������ɣ����һ��ѭ��
 						if ( m_Header_5_1.SceneZipDataLen - m_SceneDataDownedByte < m_bufSize)
 							m_bufSize = m_Header_5_1.SceneZipDataLen - m_SceneDataDownedByte;
 						
 						//������Ϣ�������ڣ���������
 						PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, IS_DOWN, index);
 					}
 					else //�߳���ֹ
 						return FALSE;
 				}
 			}
 			//�������ݴ���
 			else
 				return FALSE;    
 		}
 	}	
	return TRUE;
}

//����3.1���°汾��Image����
BOOL CDownLoadN3DFile::ReadN3DImage()
{
	m_downloadStatus = VGS_DOWNLOAD_STATUS_IMAGE;

	BOOL bResult = FALSE;		//���ط����ķ���ֵ
	DWORD dwBytesRead = 0;		//���ص���ʵ���ݵĳ���	

	pImageDataBuf = NULL;
	m_ImageCnt    = 0;

	while (!m_bAllImageDown)
	{
		if (!m_bTerminated)
		{
			//�õ�ͼ������
			if (!m_bGetImageCnt)
			{
				m_bufSize = 4;
				bResult = InternetReadFile(m_hURL,				// handle to URL
										   (LPSTR)m_pBuf,		// pointer to  buffer
										   (DWORD)m_bufSize,	// size of buffer
										   &dwBytesRead);		// pointer to var to hold return value	
				if (bResult && (dwBytesRead == m_bufSize))
				{
					//ͼ�������
					memcpy(&m_ImageCnt, m_pBuf, m_bufSize);
					m_bGetImageCnt = TRUE;
				
					//������ݻ���
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;

					//��ʼ����������Image�Ļ���
					if (m_ImageCnt > 0)
					{
						pImageDataBuf = new N3D_5_1_ZipImageData[m_ImageCnt];
						for (INT i = 0; i < m_ImageCnt; i++)
						{
							pImageDataBuf[i].Data = NULL;
						}
					}					
					else //���N3D�ļ���û��Image
					{
						//������Ϣ�������ڣ�����ǰ�ĸո����������ͼ���
						//PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ALLIMAGE_ISDOWN, index);	
						
						//״̬����
						m_bAllImageDown = TRUE;
						return TRUE;
					}

					//д�뱾���ļ�
					//if (m_pLocalFile != NULL)
					//	fwrite(&m_ImageCnt, sizeof(m_ImageCnt), 1, m_pLocalFile);
				}
				//����ʧ�ܣ��������ȴ���һ��ѭ��ʱ���Լ�������
				else
					return FALSE;
			}
		}
		else
			return FALSE;


		//ͼƬ����
		if (!m_bTerminated)
		{
			if (!m_bGetImageNameSize)
			{
				m_bufSize = 4;
				bResult = InternetReadFile(m_hURL,				// handle to URL
										   (LPSTR)m_pBuf,		// pointer to  buffer
										   (DWORD)m_bufSize,	// size of buffer
										   &dwBytesRead);		// pointer to var to hold return value	
				if (bResult && dwBytesRead > 0)
				{
					
					//��ͼ�Ĳ���
					memcpy(&m_CurImageNameSize, m_pBuf, m_bufSize);

					//����ܳ���32���ֽ�
					if (m_CurImageNameSize > 32)
					{
						MessageBox(GetActiveWindow(), TEXT("����...�ð汾��VGS��ά���治֧��ͼƬ�����ַ�������32���ֽڡ�"), TEXT("VGS���治֧�ֵ�����..."), MB_OK|MB_ICONERROR);
						return FALSE;
					}
							
					//�Ѿ����ͼ�����
					m_bGetImageNameSize = TRUE;
					m_bGetImageName = FALSE;
	            
					// ������ݻ���
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;
				}
				else//ʧ�ܣ��ȴ���һ��ѭ��ʱ���Լ�������
					return FALSE;		
			}
		}
		else
			return FALSE;

		//��ȡͼ������
		if (!m_bTerminated)
		{
			if (!m_bGetImageName)
			{
				if (m_CurImageNameSize > 0 )
				{
					m_bufSize = min(m_CurImageNameSize, sizeof(pImageDataBuf[m_DownedImageCnt].imageInfo.Name));
					bResult = InternetReadFile(m_hURL,				// handle to URL
						(LPSTR)m_pBuf,		// pointer to  buffer
						(DWORD)m_bufSize,	// size of buffer
						&dwBytesRead);		// pointer to var to hold return value	
					if(bResult && dwBytesRead > 0)
					{
						ZeroMemory(pImageDataBuf[m_DownedImageCnt].imageInfo.Name, sizeof(pImageDataBuf[m_DownedImageCnt].imageInfo.Name));
						//��ͼ�Ĳ���
						memcpy(pImageDataBuf[m_DownedImageCnt].imageInfo.Name, m_pBuf, m_bufSize);
						
						//�Ѿ����ͼ�����
						m_bGetImageName = TRUE;
						m_bGetCurImageInfo =FALSE;
						
						//������ݻ���
						curPos += m_bufSize;
						ZeroMemory(m_pBuf, 1024);
						m_bufSize = 1024;  
						dwBytesRead = 0;
					}
					//ʧ�ܣ��ȴ���һ��ѭ��ʱ���Լ�������
					else
						return FALSE;	
				}
				else
				{
					ZeroMemory(pImageDataBuf[m_DownedImageCnt].imageInfo.Name,sizeof(pImageDataBuf[m_DownedImageCnt].imageInfo.Name));
					m_bGetImageName = TRUE;
					m_bGetCurImageInfo =FALSE;
				}
			}
		}
		else
			return FALSE;


		//�õ���ǰImage�Ĳ���
		if (!m_bTerminated)
		{
			if (!m_bGetCurImageInfo)
			{
				m_bufSize = 4;
				bResult = InternetReadFile(m_hURL,				// handle to URL
										   (LPSTR)m_pBuf,		// pointer to  buffer
										   (DWORD)m_bufSize,	// size of buffer
										   &dwBytesRead);		// pointer to var to hold return value	
				if(bResult && dwBytesRead > 0)
				{
					//��ͼ�Ĳ���
					memcpy(&pImageDataBuf[m_DownedImageCnt].imageInfo.Size, m_pBuf, m_bufSize);

					//����ܳ���4M
					if (pImageDataBuf[m_DownedImageCnt].imageInfo.Size > 1024*1024*4)
					{
						MessageBox(GetActiveWindow(), ("����...�ð汾��VGS��ά���治֧���ļ���С����1024*1024��ͼ��"), ("VGS���治֧�ֵ�����..."), MB_OK|MB_ICONERROR);
						return FALSE;
					}
					
					pImageDataBuf[m_DownedImageCnt].Data = new BYTE[pImageDataBuf[m_DownedImageCnt].imageInfo.Size];

					//�Ѿ����ͼ�����
					m_bGetCurImageInfo = TRUE;

					//������ݻ���
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;
				}
				else //ʧ�ܣ��ȴ���һ��ѭ��ʱ���Լ�������
					return FALSE;
			}
		}
		else
			return FALSE;

		
		if (!m_bTerminated)
		{
			//�жϵ�ǰImage�����ݴ�С�Ƿ�<1K
			if (pImageDataBuf[m_DownedImageCnt].imageInfo.Size < 1024)
			{
				m_bufSize = pImageDataBuf[m_DownedImageCnt].imageInfo.Size;
				//���ص�ǰImage��ʵ��ѹ������
				bResult = InternetReadFile(m_hURL,				// handle to URL
										   (LPSTR)m_pBuf,		// pointer to  buffer
										   (DWORD)m_bufSize,	// size of buffer
										   &dwBytesRead);		// pointer to var to hold return value	
				if (bResult && dwBytesRead > 0)
				{
					//�������ݵ�pBuf
					memcpy(pImageDataBuf[m_DownedImageCnt].Data, m_pBuf, m_bufSize);
					
					//������ݻ���
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;
					
					//���ص��ֽ���
					m_CurImageDataDownedByte += m_bufSize;
					
					//�Ѿ����ص�ͼ����������
					m_DownedImageCnt++;
					m_bCurImageDown = TRUE;

					//״ֵ̬��λ
					m_bGetImageNameSize = FALSE;
					m_CurImageDataDownedByte = 0;

					//����ͼ�����ݵ�����
					INT imageIndex = m_DownedImageCnt - 1;

					// ������ͼ
					EnterCriticalSection(m_pRender_cs);
					N3DFile.ConstructTextureFromImage(&(pImageDataBuf[imageIndex]), imageIndex);
					LeaveCriticalSection(m_pRender_cs);

					//PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ONETEX_ISDOWN, index);

					//д�뱾���ļ�
					//if (m_pLocalFile != NULL)
					//	fwrite(pTexDataBuf[m_DownedImageCnt].Data, m_CurImageDataSize, 1, m_pLocalFile);
				}
				else  //ʧ�ܣ��´�ѭ���ٴ�����
					return FALSE;
			}
			else //ͼ�����ݴ���1K
			{
				//���ص�ǰImage��ʵ��ѹ������
				bResult = InternetReadFile(m_hURL,				// handle to URL
										   (LPSTR)m_pBuf,		// pointer to  buffer
										   (DWORD)m_bufSize,	// size of buffer
										   &dwBytesRead);		// pointer to var to hold return value	
				
				if (bResult && (dwBytesRead == m_bufSize) )
				{
					//�жϵ�ǰImage�����Ƿ��Ѿ�������
					if (m_CurImageDataDownedByte == pImageDataBuf[m_DownedImageCnt].imageInfo.Size)
					{
						//������ݻ���
						curPos += m_bufSize;
						ZeroMemory(m_pBuf, 1024);
						m_bufSize = 1024;  
						dwBytesRead = 0;

						//�Ѿ����ص�ͼ����������
						m_DownedImageCnt++;
						m_bCurImageDown = TRUE;

						//״ֵ̬��λ
						m_bGetImageNameSize = FALSE;
						m_CurImageDataDownedByte = 0;

						//����ͼ�����ݵ�����
						INT imageIndex = m_DownedImageCnt - 1;
						
						// ������ͼ
						EnterCriticalSection(m_pRender_cs);
						N3DFile.ConstructTextureFromImage(&(pImageDataBuf[imageIndex]), imageIndex);
						LeaveCriticalSection(m_pRender_cs);

						//PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ONETEX_ISDOWN, index);

						//д�뱾���ļ�
						//if (m_pLocalFile != NULL)
						//	fwrite(pTexDataBuf[m_DownedImageCnt].Data, m_CurImageDataSize, 1, m_pLocalFile);
					}
					else //��û��������
					{
						//�������ݵ�pBuf
						for (DWORD i = 0; i< m_bufSize; i++)
							pImageDataBuf[m_DownedImageCnt].Data[m_CurImageDataDownedByte + i] = m_pBuf[i];
						
						m_CurImageDataDownedByte += m_bufSize;

						//������ݻ���
						curPos += m_bufSize;
						ZeroMemory(m_pBuf, 1024);
						m_bufSize   = 1024;
						dwBytesRead = 0;

						//�жϵ�ǰImage�����Ƿ��Ѿ���Ҫ������ɣ����һ��ѭ��
						if (pImageDataBuf[m_DownedImageCnt].imageInfo.Size - m_CurImageDataDownedByte < m_bufSize)
							m_bufSize = pImageDataBuf[m_DownedImageCnt].imageInfo.Size - m_CurImageDataDownedByte;
						
						//������Ϣ�������ڣ���������
						PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, IS_DOWN, index);
					}
				}
				else  //ʧ�ܣ��´�ѭ������
					return FALSE;
			}
		}
		else //�߳���ֹ
			return FALSE; 
			

		//-----------------------------------------------------------------------------------------------
		//Image�Ƿ�ȫ��������
		//-----------------------------------------------------------------------------------------------
		if (m_DownedImageCnt == m_ImageCnt)
		{
			m_bAllImageDown = TRUE;
			//������Ϣ�������ڣ�����ǰ�ĸո����������ͼ���
			//PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ALLIMAGE_ISDOWN, index);
		}
	}

	// fuxb ==>
	if (pImageDataBuf)
	{
		for (DWORD iData = 0; iData < m_ImageCnt; ++iData)
		{
			N3D_5_1_ZipImageData &imageData = pImageDataBuf[iData];
			SAFE_DELETE_ARRAY(imageData.Data);
		}
		SAFE_DELETE_ARRAY(pImageDataBuf);
	}
	// <==

	return TRUE;
}

//-----------------------------------------------------------------------
// ������: ReadN3DFile
// ��  ��: 
// ��  ;: ��ȡhttp�ϵ�N3D�ļ����ݵ��ڴ棬������ʽ���أ�ʵʱ��̬��ʾ����
//		   һ����������һ����ʾ��������ȡ�����ݺ󱣴���Internet Cache��
//-----------------------------------------------------------------------
BOOL CDownLoadN3DFile::ReadN3DFile()
{
    BOOL bResult = true;		//���ط����ķ���ֵ
    DWORD dwBytesRead = 0;		//���ص���ʵ���ݵĳ���

	//�ж�N3D�ļ��İ汾��
 	if (!m_bVersionInfoDown && !m_bTerminated)
 	{
 		GetN3DVersion();

		if (m_N3DVersion == 0xffffffff)
		{
			::MessageBox(GetActiveWindow(), 
				TEXT("N3D�ļ��汾�Ų��Ϸ�"),
				TEXT("����"),
				MB_OK | MB_ICONINFORMATION);

			//�˳�����
			m_bTerminated = TRUE;
			m_bOutDate  = TRUE;
			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
			return FALSE;
		} 		
 		else if (VGSPlayerVer < m_N3DVersion)  // �������İ汾�����ļ��İ汾
 		{
 			::MessageBox(GetActiveWindow(), 
 						TEXT("������ʾ��\n\t����ʹ�õ�VGS��ά����汾̫���ˣ�����Ҫ��װ���߼��İ汾�������������\n�������ʾ�Զ�����VGS��"), 
 						TEXT("VGS��ά������Ҫ����..."),
 						MB_OK | MB_ICONINFORMATION);
 			
 			//�˳�����
 			m_bTerminated = TRUE;
 			m_bOutDate  = TRUE;
 			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
 			return FALSE;
 		}
 	}	
 
 	//����N3D���ļ�ͷ, ReadN3DHeader()�����Զ�����ͬ�汾��N3D�ļ�ͷ
 	if (!ReadN3DHeader())
 	{
 		m_bTerminated = TRUE;
 		PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
 		return FALSE;
 	}
 
 	if (m_N3DVersion >= 51000 && m_N3DVersion <= N3D_CURRENT_V3DVERSION)
 	{
 		//����N3D�ļ��ĳ�������
 		if (!ReadN3DScene())
 		{
 			m_bTerminated = TRUE;
			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
 			return FALSE;
 		}
 
 		//����N3D�ļ���ͼ������
 		if (!ReadN3DImage())
 		{
 			m_bTerminated = TRUE;
 			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
 			return FALSE;
 		}
 		
 		//�����������
 		m_bAllDown = TRUE;
 		//�����ݱ��浽Cache
 		m_bufSize = 10;
 		bResult = InternetReadFile(m_hURL,				// handle to URL
 								   (LPSTR)m_pBuf,		// pointer to  buffer
 								   (DWORD)m_bufSize,	// size of buffer
 								   &dwBytesRead);		// pointer to var to hold return value	
 		
 		PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ALL_ISDOWN, index);
 
 		//״̬��
 		m_bTerminated = TRUE;
 
 		return TRUE;
 	}

	return TRUE;
}

//----------------------------------------------------------------
//	���ƣ�JudgeFileLink
//	��飺�ж������ļ��Ƿ����
//	������url:i:����
//	���أ���������ļ��������أ��򷵻��ļ��ߴ磬���򷵻�-1
//----------------------------------------------------------------
int CDownLoadN3DFile::JudgeFileLink(HINTERNET url)
{	
	DWORD index = 0;
	DWORD readBytes = 32;
	TCHAR  buffer[32] = {0};
	int result = HttpQueryInfo(url, HTTP_QUERY_STATUS_CODE, buffer, &readBytes, &index);

	if(atoi(buffer) != HTTP_STATUS_OK)
		return 0;

	//��ȡN3D�ļ���С
	char  size[255] = {0};
	DWORD read = 255;
	result = HttpQueryInfo(url, HTTP_QUERY_CONTENT_LENGTH, size, &read, &index);
	int filesize = atoi(size);
	
	return filesize;
}

//-----------------------------------------------------------------------------
// ������: DownLoadHttpFile()
// ��  ��:  char* strHttpFile, �����ص��ļ���ַ    
//			char* strDestFile, ���浽���ص��ļ�·��
//		    HWND  hWnd,		   ��������
//			CSceneResource* pSceneResource  ������Դ����
// ��  ;: ����http�ļ�
//-----------------------------------------------------------------------------
bool CDownLoadN3DFile::DownLoadHttpFile(const string &strHttpFile, const string &strDestFile, HWND hWnd, DWORD pSceneResource)
{
	if (strHttpFile == "" || !m_pDownloadN3DFileManager) return false;
	m_pSceneResource = pSceneResource;

	//Ҫ�����ļ���������?
	m_strLocalFileName = strDestFile;
	/*if (!strDestFile.empty())
	{
		 strcpy(m_strLocalFileName, strDestFile);
	}*/

	//--------------------------------------------------------------
	//���ݱ���
	//--------------------------------------------------------------
	// strcpy(m_strNetFileName, strHttpFile);
	m_strNetFileName = strHttpFile;
	m_hWnd = hWnd;
	//strcpy(N3DFile.m_strFileName, strHttpFile);

	//--------------------------------------------------------------
	//���url����
	//--------------------------------------------------------------
	m_hInternetSession = m_pDownloadN3DFileManager->getInternetSession();
	m_hURL = InternetOpenUrlA(m_hInternetSession, 
							(m_strNetFileName.c_str()), 
							NULL, 
							0, 
							INTERNET_FLAG_RESYNCHRONIZE|INTERNET_FLAG_NEED_FILE, 
							0);
	if(m_hURL == NULL)
	{
		string sErr = string("VGS�޷����ӵ� ") + m_strNetFileName + string(" �ļ�. �����ļ��Ѿ������ڣ�����վ��������ϵ���Խ��������.");
		/*strcpy(m_err, "VGS�޷����ӵ� ");
		strcat(m_err, m_strNetFileName);
		strcat(m_err, " �ļ�. �����ļ��Ѿ������ڣ�����վ��������ϵ���Խ��������.");*/
		MessageBox(NULL,  (sErr.c_str()), ("��������Զ���ļ�...����"), MB_OK | MB_ICONERROR);
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
		string sErr = "����ȷ��N3D�ļ��Ĵ�С," + m_strNetFileName + "...����!";
		MessageBox(NULL, (sErr.c_str()), ("�����ļ���С...����, Զ�̷������ļ�������."), MB_ICONASTERISK | MB_OK);
		return false;
	}

	//--------------------------------------------------------------
	//����һ�������߳�
	//--------------------------------------------------------------
	if (CreateDownThread() == false)
	{
		string sErr = "CreateDownThread() -> error";
		MessageBox(NULL, (sErr.c_str()), ("���ڴ��������߳�...����"), MB_ICONASTERISK | MB_OK);
		return false;
	}
	
	//--------------------------------------------------------------
	//�����������͵���Ϣ
	//--------------------------------------------------------------
	//PostMessage(m_hWnd, WM_DOWNLOAD_PROC, DOWN_TYPE, type);

	return true;
}

// ��ʼ����
void  CDownLoadN3DFile::StartDownLoad()
{
	if (m_pDownloadN3DFileManager)
	{
		DownLoadHttpFile(m_sFileFullName, "", m_pDownloadN3DFileManager->getMsgWndHandle(), 0);
	}
}

//-----------------------------------------------------------------------------
// ������: Release()
// ��  ��: 
// ��  ;: �ͷ��ڴ漰�����Դ
//-----------------------------------------------------------------------------
void  CDownLoadN3DFile::Release()
{
	// m_pScene = NULL;

	//�ͷ�N3D�ļ��Ĺ��ɻ���
	N3DFile.Release();

	//�ͷų�������
	SAFE_DELETE_ARRAY(pSceneDataBuf);

	//�ͷ�ͼ�񻺳�
	if (m_ImageCnt != 0 && pImageDataBuf != NULL)
	{
		if (pImageDataBuf)
		{
			for (UINT i = 0; i< m_ImageCnt; i++)
				SAFE_DELETE_ARRAY(pImageDataBuf[i].Data);
			SAFE_DELETE_ARRAY(pImageDataBuf);
		}

		
		if (pImageDataBuf_5_1)
		{
			for (UINT i = 0; i< m_ImageCnt; i++)
				SAFE_DELETE_ARRAY(pImageDataBuf_5_1[i].Data);
			SAFE_DELETE_ARRAY(pImageDataBuf_5_1);
		}
	}

	//fclose(m_pLocalFile);
	//m_pLocalFile = NULL;
}

VOID CDownLoadN3DFile::SetVGSCore(CVGS2DLL *pVGSCore)
{
	m_pVGSCore = pVGSCore;
	N3DFile.SetVGSCore(pVGSCore);
}