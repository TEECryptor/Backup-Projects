/////////////////////////////////////////////////////////////////////////////////////////
// 名  称: CDownLoadN3DFile
// 描  述: 从Web服务器上下载文件的类
// 作  者: lixin
// 日  期: 2006-06-19
/////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "CDownLoadN3DFile.h"
#include "..\\Download\\VGSFileDownloadManager.h"
#include <time.h>

DWORD			DownLoad_lastTimer = 0;			//Timer对象 
DWORD			DowndLoad_SPF = 100;			//多长时间间隔执行一次下载过程

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownLoadN3DFile::CDownLoadN3DFile(CDownloadN3DFileManager*  pDownloadN3DFileManager, CRITICAL_SECTION* pCtrl_cs,  CRITICAL_SECTION* pRender_cs)
{
	m_pDownloadN3DFileManager = pDownloadN3DFileManager;

	//VGS Player的版本号
	VGSPlayerVer = N3D_CURRENT_V3DVERSION /*/ 10000.0; 5.1007f*/;

	m_pCtrl_cs = pCtrl_cs;
	m_pRender_cs = pRender_cs;
	//m_hInternetSession = hInternetSession;

	m_fileSize = 0;
	index = 0;
	//每次下载1024字节，初始化下载文件的缓冲
	m_bufSize = 1024;
	ZeroMemory(m_pBuf, m_bufSize);
	// ZeroMemory(m_err, sizeof(m_err));

	//初始化缓冲
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

	//初始化变量
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

	//设置N3DFile对象的场景	
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
//	名称：DownThreadFunc
//	简介：线程函数
//	参数：impl:i:传入参数
//	返回：
//----------------------------------------------------------------
DWORD WINAPI DownThreadFunc(LPVOID impl)
{
	return ((CDownLoadN3DFile*)impl)->Execute();
}

//-----------------------------------------------------------------------------
// 方法名: CreateDownThread
// 参  数: 
// 用  途: 创建一个下载线程
//-----------------------------------------------------------------------------
bool CDownLoadN3DFile::CreateDownThread()
{
	m_threadHandle = CreateThread(NULL, 0, DownThreadFunc, (LPVOID)this, 0, &m_threadID);
	if (m_threadHandle == NULL) return false;
	return true;
}	

//-----------------------------------------------------------------------------
// 方法名: EndDownThread
// 参  数: 
// 用  途: 终止一个下载线程
//-----------------------------------------------------------------------------
void CDownLoadN3DFile::EndDownThread()
{
	//断开网络连接
    InternetCloseHandle(m_hURL);
	
	//删除线程
	DWORD ExitCode;
	if (GetExitCodeThread(m_threadHandle, &ExitCode))
		ExitThread(ExitCode);
}
//----------------------------------------------------------------
//	名称：Execute
//	简介：线程执行函数
//	参数：无
//	返回：
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

	//断开网络连接
    InternetCloseHandle(m_hURL);
	

	//N3D文件授权过期
	if (m_bOutDate == TRUE)
	{
		MessageBox(GetActiveWindow(),
			TEXT("    文件没有取得合法授权或授权已过期，WebMax软件系统为中国国家发明型专利技术，\n受到法律保护，必须获得WebMax软件服务商合法授权才能发布WebMax三维网页。"), 
			TEXT("授权不合法或已经过期..."),
			MB_OK | MB_ICONEXCLAMATION);
		// CVGS2DLL::getSingleton().RestoreScene();
		// m_pScene->Reset();
	}

	//E3D文件版本过高，本版本引擎无法打开
	if (m_bFileVerErr == TRUE)
	{	
		MessageBox(GetActiveWindow(),
			TEXT("    您的系统安装的WebMax Player版本需要升级，请访问http://www.suntoward.com并\n安装最新版本的WebMax Player，然后再运行本网页。"), 
			TEXT("WebMax Player 版本需要升级"),
			MB_OK | MB_ICONEXCLAMATION);
		// m_pScene->Reset();
		// CVGS2DLL::getSingleton().RestoreScene();
	}
	//释放资源
	Release();
	
	m_bExecuteOver = TRUE;

	return 0;
}

//判断N3D文件的版本号
DWORD CDownLoadN3DFile::GetN3DVersion()
{
	m_downloadStatus = VGS_DOWNLOAD_STATUS_VERSION;

     BOOL bResult = FALSE;		//下载方法的返回值
     DWORD dwBytesRead = 0;		//下载的真实数据的长度		
 	
 	//读取4个字节的数据，判断N3D文件的版本号
 	m_bufSize = 4;
 	bResult = InternetReadFile(m_hURL, (LPSTR)m_pBuf, m_bufSize, &dwBytesRead);
 	if (bResult && (dwBytesRead == m_bufSize) )
 	{
 		//拷贝数据到
 		memcpy(&m_Header_5_1.Version, m_pBuf, m_bufSize);
 		//老版本N3D, SUNT== 2896867755
 		
 		if (m_Header_5_1.Version >= 51000 && m_Header_5_1.Version <= N3D_CURRENT_V3DVERSION /*52000*/)
 		{
 			m_N3DVersion = m_Header_5_1.Version;
 			
 			//清空数据缓存，保存变量
 			curPos += dwBytesRead;
 			dwBytesRead = 0;
 			ZeroMemory(m_pBuf, 1024);
 			m_bufSize = 1024;
 			//版本信息已经下载
 			m_bVersionInfoDown = TRUE;	
 			// return (m_Header_5_1.Version/10000.0f);
 		}		
 		else //不合法版本
 		{
			m_N3DVersion = 0xffffffff;
 			// m_pScene->m_bAuthorized = FALSE;
 			//return 3.0f;
 		} 		
 	}
	
	return m_N3DVersion;
}

//读入N3D的文件头
BOOL CDownLoadN3DFile::ReadN3DHeader()
{
	m_downloadStatus = VGS_DOWNLOAD_STATUS_HEADER;

     BOOL bResult = FALSE;		//下载方法的返回值
     DWORD dwBytesRead = 0;		//下载的真实数据的长度	
 
 	if (!m_bHeaderDown)
 	{
 		//5.1版本的N3D文件的文件头
 		if (m_N3DVersion >= 51000 && m_N3DVersion <= N3D_CURRENT_V3DVERSION/*52000*/)
 		{
 			m_bufSize = 92;
 			BYTE* tempBuf = new BYTE[m_bufSize];
 			bResult = InternetReadFile(m_hURL, (LPSTR)tempBuf, m_bufSize, &dwBytesRead);
 			if (bResult && (dwBytesRead == m_bufSize) )
 			{
 				//授权联盟的名称
 				memcpy(&m_Header_5_1.RegInfo, tempBuf, sizeof(m_Header_5_1.RegInfo));
 				tempBuf += sizeof(m_Header_5_1.RegInfo);
 					
 				//场景数据的起始指针
 				memcpy(&m_Header_5_1.SceneDataPtr, tempBuf, sizeof(m_Header_5_1.SceneDataPtr));
 				tempBuf += sizeof(m_Header_5_1.SceneDataPtr);
         
 				//压缩后的场景数据长度
 				memcpy(&m_Header_5_1.SceneZipDataLen, tempBuf, sizeof(m_Header_5_1.SceneZipDataLen));
 				tempBuf += sizeof(m_Header_5_1.SceneZipDataLen);
 				//原场景数据的长度
 				memcpy(&m_Header_5_1.OriginSceneDataLen, tempBuf, sizeof(m_Header_5_1.OriginSceneDataLen));
 				tempBuf += sizeof(m_Header_5_1.OriginSceneDataLen);
 				//贴图数据的起始指针
 				memcpy(&m_Header_5_1.ImageDataPtr, tempBuf, sizeof(m_Header_5_1.ImageDataPtr));
 				tempBuf += sizeof(m_Header_5_1.ImageDataPtr);
 				//贴图数据的长度
 				memcpy(&m_Header_5_1.ImageDataLen, tempBuf, sizeof(m_Header_5_1.ImageDataLen));
 				tempBuf += sizeof(m_Header_5_1.ImageDataLen);
 				//reserve
 				memcpy(&m_Header_5_1.Reserved, tempBuf, sizeof(m_Header_5_1.Reserved));
 				tempBuf += sizeof(m_Header_5_1.Reserved);
 
 				//释放缓冲
 				tempBuf -= 92;
 				SAFE_DELETE_ARRAY(tempBuf);
 
 				//清空数据缓存
 				curPos += dwBytesRead;
 				dwBytesRead = 0;
 				ZeroMemory(m_pBuf, 1024);
 				m_bufSize = 1024;
 
 				//文件头已经下载
 				m_bHeaderDown = TRUE;

				N3DFile.SetHeaderInfo(m_Header_5_1);
 
 				//写入本地文件
 				//if (m_pLocalFile != NULL)
 				//	fwrite(&m_Header_3_0, sizeof(m_Header_3_0), 1, m_pLocalFile);
 
 				//发送消息到主窗体
				PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, HEADER_ISDOWN, index);
 
 				return TRUE;
 			}
 			//下载失败
 			else
 			{
 				//释放缓冲
 				SAFE_DELETE_ARRAY(tempBuf);
 				return FALSE;
 			}
 		}
 	}
	return FALSE;
}

//读入3.1以下版本的N3D场景数据
BOOL CDownLoadN3DFile::ReadN3DScene()
{
	m_downloadStatus = VGS_DOWNLOAD_STATUS_SCENE;

     BOOL bResult = FALSE;		//下载方法的返回值
     DWORD dwBytesRead = 0;		//下载的真实数据的长度	
 
 	if (pSceneDataBuf == NULL)
 	{
 		pSceneDataBuf = new BYTE[m_Header_5_1.SceneZipDataLen];
 		ZeroMemory(pSceneDataBuf, m_Header_5_1.SceneZipDataLen);
 	}
 
 	//如果场景数据不够1K
 	if (m_Header_5_1.SceneZipDataLen <= 1024)
 	{
 		m_bufSize = m_Header_5_1.SceneZipDataLen;
 		bResult = InternetReadFile(m_hURL,				// handle to URL
 								  (LPSTR)m_pBuf,		// pointer to  buffer
 								  (DWORD)m_bufSize,	    // size of buffer 
 								  &dwBytesRead);		// pointer to var to hold return value
 		if (bResult && (dwBytesRead == m_bufSize) )
 		{
 			//拷贝数据到pBuf
 			memcpy(pSceneDataBuf, m_pBuf, m_bufSize);
 
 			//清空数据缓存
 			curPos += m_bufSize;
 			ZeroMemory(m_pBuf, 1024);
 			m_bufSize = 1024;  
 			dwBytesRead = 0;
 
 			//场景已经下载完
 			m_bSceneDown = true;
 			m_SceneDataDownedByte = curPos;
 
 			// 在这里解压场景数据...
 			if (N3DFile.UnzipSceneData(pSceneDataBuf, 
 									   m_Header_5_1.SceneZipDataLen,
 									   m_Header_5_1.OriginSceneDataLen ))
 			{
 				if (!m_bTerminated)
 				{
 					//将模型插入到场景模型渲染队列
 					//EnterCriticalSection(m_pRender_cs);
 					// INT result = N3DFile.ConstructScene(name, m_pSceneResource);
					N3DFile.ConstructScene("");
 					//LeaveCriticalSection(m_pRender_cs);
 				}
 				else //线程终止
				{
					return FALSE;
				}
 			}
 			//解压失败
 			else
 			{
 				::MessageBox(GetActiveWindow(), 
 							("场景数据读取错误, VGS引擎将自动关闭。"), 
 							("场景数据读取失败..."), 
 							MB_OK|MB_ICONINFORMATION);
 
 				m_bTerminated = TRUE;
 
 				return FALSE;
 			}
 			
 			//发送消息到主窗口
			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, SCENE_ISDOWN, index);
 
 			//写入本地文件
 			//if (m_pLocalFile != NULL)
 			//	fwrite(pSceneDataBuf, m_Header_3_0.SceneZipDataLen, 1, m_pLocalFile);
 		}
 		else//下载数据失败
 			return FALSE;
 	}
 	else//场景数据大于1K
 	{
 		//读取N3D->Scene信息,正好读到文场景信息结束
 		while(!m_bSceneDown)
 		{
 			bResult = InternetReadFile(m_hURL,				// handle to URL
 									   (LPSTR)m_pBuf,		// pointer to  buffer
 									   (DWORD)m_bufSize,	// size of buffer
 									   &dwBytesRead);		// pointer to var to hold return value	
 			
 			if (bResult && (dwBytesRead == m_bufSize) )
 			{
 				//判断场景数据是否已经下载完--已经下载完
 				if (m_SceneDataDownedByte == m_Header_5_1.SceneZipDataLen)
 				{
 					//清空数据缓存
 					curPos += m_bufSize;
 					ZeroMemory(m_pBuf, 1024);
 					m_bufSize = 1024;  
 					dwBytesRead = 0;
 					
 					//场景数据已经下载完成
 					m_bSceneDown = TRUE;
 					m_SceneDataDownedByte = curPos;
 
 					//在这里解压场景数据...
 					if (N3DFile.UnzipSceneData(pSceneDataBuf, 
 											   m_Header_5_1.SceneZipDataLen,
 											   m_Header_5_1.OriginSceneDataLen ))
 					{
 						if (!m_bTerminated)  //如果当前线程没有被关掉
 						{
 							//将模型插入到场景模型渲染队列
 							//EnterCriticalSection(m_pRender_cs);
 							//创建场景到主场景
 							INT result = N3DFile.ConstructScene(""/*name, m_pSceneResource*/);
 							//LeaveCriticalSection(m_pRender_cs);
 						}
 						else //线程终止
 							return FALSE;
 					}
 					else//解压失败
 					{
 						::MessageBox(GetActiveWindow(), 
 									("场景数据读取错误, VGS引擎将自动关闭。"), 
 									("场景数据读取失败..."), 
 									MB_OK|MB_ICONINFORMATION);
 
 						m_bTerminated = TRUE;
 						return FALSE;
 					}
 
 					//发送消息到主窗口
 					PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, SCENE_ISDOWN, index);
 
 					//写入本地文件
 					//if (m_pLocalFile != NULL)
 					//	fwrite(pSceneDataBuf, m_Header_3_0.SceneZipDataLen, 1, m_pLocalFile);
 				}
 				//没有下载完
 				else
 				{	
 					if (!m_bTerminated)  //如果当前线程没有被关掉
 					{	
 						//拷贝数据到pBuf
 						for (DWORD i = 0; i< m_bufSize; i++)
 							pSceneDataBuf[m_SceneDataDownedByte + i] = m_pBuf[i];
 						
 						m_SceneDataDownedByte += dwBytesRead;
 						
 						//清空数据缓存
 						curPos += m_bufSize;
 						ZeroMemory(m_pBuf, 1024);
 						m_bufSize = 1024;  
 						dwBytesRead = 0;
 						
 						//判断场景信息是否将要下载完成，最后一个循环
 						if ( m_Header_5_1.SceneZipDataLen - m_SceneDataDownedByte < m_bufSize)
 							m_bufSize = m_Header_5_1.SceneZipDataLen - m_SceneDataDownedByte;
 						
 						//发送消息到主窗口，正在下载
 						PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, IS_DOWN, index);
 					}
 					else //线程终止
 						return FALSE;
 				}
 			}
 			//下载数据错误
 			else
 				return FALSE;    
 		}
 	}	
	return TRUE;
}

//读入3.1以下版本的Image数据
BOOL CDownLoadN3DFile::ReadN3DImage()
{
	m_downloadStatus = VGS_DOWNLOAD_STATUS_IMAGE;

	BOOL bResult = FALSE;		//下载方法的返回值
	DWORD dwBytesRead = 0;		//下载的真实数据的长度	

	pImageDataBuf = NULL;
	m_ImageCnt    = 0;

	while (!m_bAllImageDown)
	{
		if (!m_bTerminated)
		{
			//得到图像数量
			if (!m_bGetImageCnt)
			{
				m_bufSize = 4;
				bResult = InternetReadFile(m_hURL,				// handle to URL
										   (LPSTR)m_pBuf,		// pointer to  buffer
										   (DWORD)m_bufSize,	// size of buffer
										   &dwBytesRead);		// pointer to var to hold return value	
				if (bResult && (dwBytesRead == m_bufSize))
				{
					//图像的数量
					memcpy(&m_ImageCnt, m_pBuf, m_bufSize);
					m_bGetImageCnt = TRUE;
				
					//清空数据缓存
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;

					//初始化保存所有Image的缓冲
					if (m_ImageCnt > 0)
					{
						pImageDataBuf = new N3D_5_1_ZipImageData[m_ImageCnt];
						for (INT i = 0; i < m_ImageCnt; i++)
						{
							pImageDataBuf[i].Data = NULL;
						}
					}					
					else //如果N3D文件中没有Image
					{
						//发送消息到主窗口，带当前的刚刚下载完的贴图编号
						//PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ALLIMAGE_ISDOWN, index);	
						
						//状态变量
						m_bAllImageDown = TRUE;
						return TRUE;
					}

					//写入本地文件
					//if (m_pLocalFile != NULL)
					//	fwrite(&m_ImageCnt, sizeof(m_ImageCnt), 1, m_pLocalFile);
				}
				//下载失败，跳出，等待下一次循环时尝试继续下载
				else
					return FALSE;
			}
		}
		else
			return FALSE;


		//图片名称
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
					
					//贴图的参数
					memcpy(&m_CurImageNameSize, m_pBuf, m_bufSize);

					//最大不能超过32个字节
					if (m_CurImageNameSize > 32)
					{
						MessageBox(GetActiveWindow(), TEXT("错误...该版本的VGS三维引擎不支持图片名称字符数超过32个字节。"), TEXT("VGS引擎不支持的特性..."), MB_OK|MB_ICONERROR);
						return FALSE;
					}
							
					//已经获得图像参数
					m_bGetImageNameSize = TRUE;
					m_bGetImageName = FALSE;
	            
					// 清空数据缓存
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;
				}
				else//失败，等待下一次循环时尝试继续下载
					return FALSE;		
			}
		}
		else
			return FALSE;

		//获取图像名称
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
						//贴图的参数
						memcpy(pImageDataBuf[m_DownedImageCnt].imageInfo.Name, m_pBuf, m_bufSize);
						
						//已经获得图像参数
						m_bGetImageName = TRUE;
						m_bGetCurImageInfo =FALSE;
						
						//清空数据缓存
						curPos += m_bufSize;
						ZeroMemory(m_pBuf, 1024);
						m_bufSize = 1024;  
						dwBytesRead = 0;
					}
					//失败，等待下一次循环时尝试继续下载
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


		//得到当前Image的参数
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
					//贴图的参数
					memcpy(&pImageDataBuf[m_DownedImageCnt].imageInfo.Size, m_pBuf, m_bufSize);

					//最大不能超过4M
					if (pImageDataBuf[m_DownedImageCnt].imageInfo.Size > 1024*1024*4)
					{
						MessageBox(GetActiveWindow(), ("错误...该版本的VGS三维引擎不支持文件大小超过1024*1024的图像。"), ("VGS引擎不支持的特性..."), MB_OK|MB_ICONERROR);
						return FALSE;
					}
					
					pImageDataBuf[m_DownedImageCnt].Data = new BYTE[pImageDataBuf[m_DownedImageCnt].imageInfo.Size];

					//已经获得图像参数
					m_bGetCurImageInfo = TRUE;

					//清空数据缓存
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;
				}
				else //失败，等待下一次循环时尝试继续下载
					return FALSE;
			}
		}
		else
			return FALSE;

		
		if (!m_bTerminated)
		{
			//判断当前Image的数据大小是否<1K
			if (pImageDataBuf[m_DownedImageCnt].imageInfo.Size < 1024)
			{
				m_bufSize = pImageDataBuf[m_DownedImageCnt].imageInfo.Size;
				//下载当前Image的实际压缩数据
				bResult = InternetReadFile(m_hURL,				// handle to URL
										   (LPSTR)m_pBuf,		// pointer to  buffer
										   (DWORD)m_bufSize,	// size of buffer
										   &dwBytesRead);		// pointer to var to hold return value	
				if (bResult && dwBytesRead > 0)
				{
					//拷贝数据到pBuf
					memcpy(pImageDataBuf[m_DownedImageCnt].Data, m_pBuf, m_bufSize);
					
					//清空数据缓存
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;
					
					//下载的字节数
					m_CurImageDataDownedByte += m_bufSize;
					
					//已经下载的图像数据数量
					m_DownedImageCnt++;
					m_bCurImageDown = TRUE;

					//状态值复位
					m_bGetImageNameSize = FALSE;
					m_CurImageDataDownedByte = 0;

					//插入图像数据到场景
					INT imageIndex = m_DownedImageCnt - 1;

					// 创建贴图
					EnterCriticalSection(m_pRender_cs);
					N3DFile.ConstructTextureFromImage(&(pImageDataBuf[imageIndex]), imageIndex);
					LeaveCriticalSection(m_pRender_cs);

					//PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ONETEX_ISDOWN, index);

					//写入本地文件
					//if (m_pLocalFile != NULL)
					//	fwrite(pTexDataBuf[m_DownedImageCnt].Data, m_CurImageDataSize, 1, m_pLocalFile);
				}
				else  //失败，下次循环再次下载
					return FALSE;
			}
			else //图像数据大于1K
			{
				//下载当前Image的实际压缩数据
				bResult = InternetReadFile(m_hURL,				// handle to URL
										   (LPSTR)m_pBuf,		// pointer to  buffer
										   (DWORD)m_bufSize,	// size of buffer
										   &dwBytesRead);		// pointer to var to hold return value	
				
				if (bResult && (dwBytesRead == m_bufSize) )
				{
					//判断当前Image数据是否已经下载完
					if (m_CurImageDataDownedByte == pImageDataBuf[m_DownedImageCnt].imageInfo.Size)
					{
						//清空数据缓存
						curPos += m_bufSize;
						ZeroMemory(m_pBuf, 1024);
						m_bufSize = 1024;  
						dwBytesRead = 0;

						//已经下载的图像数据数量
						m_DownedImageCnt++;
						m_bCurImageDown = TRUE;

						//状态值复位
						m_bGetImageNameSize = FALSE;
						m_CurImageDataDownedByte = 0;

						//插入图像数据到场景
						INT imageIndex = m_DownedImageCnt - 1;
						
						// 创建贴图
						EnterCriticalSection(m_pRender_cs);
						N3DFile.ConstructTextureFromImage(&(pImageDataBuf[imageIndex]), imageIndex);
						LeaveCriticalSection(m_pRender_cs);

						//PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ONETEX_ISDOWN, index);

						//写入本地文件
						//if (m_pLocalFile != NULL)
						//	fwrite(pTexDataBuf[m_DownedImageCnt].Data, m_CurImageDataSize, 1, m_pLocalFile);
					}
					else //还没有下载完
					{
						//拷贝数据到pBuf
						for (DWORD i = 0; i< m_bufSize; i++)
							pImageDataBuf[m_DownedImageCnt].Data[m_CurImageDataDownedByte + i] = m_pBuf[i];
						
						m_CurImageDataDownedByte += m_bufSize;

						//清空数据缓存
						curPos += m_bufSize;
						ZeroMemory(m_pBuf, 1024);
						m_bufSize   = 1024;
						dwBytesRead = 0;

						//判断当前Image数据是否已经将要下载完成，最后一个循环
						if (pImageDataBuf[m_DownedImageCnt].imageInfo.Size - m_CurImageDataDownedByte < m_bufSize)
							m_bufSize = pImageDataBuf[m_DownedImageCnt].imageInfo.Size - m_CurImageDataDownedByte;
						
						//发送消息到主窗口，正在下载
						PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, IS_DOWN, index);
					}
				}
				else  //失败，下次循环继续
					return FALSE;
			}
		}
		else //线程终止
			return FALSE; 
			

		//-----------------------------------------------------------------------------------------------
		//Image是否全部下载完
		//-----------------------------------------------------------------------------------------------
		if (m_DownedImageCnt == m_ImageCnt)
		{
			m_bAllImageDown = TRUE;
			//发送消息到主窗口，带当前的刚刚下载完的贴图编号
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
// 方法名: ReadN3DFile
// 参  数: 
// 用  途: 读取http上的N3D文件数据到内存，用于流式下载，实时动态显示场景
//		   一边下载数据一边显示场景，读取完数据后保存在Internet Cache中
//-----------------------------------------------------------------------
BOOL CDownLoadN3DFile::ReadN3DFile()
{
    BOOL bResult = true;		//下载方法的返回值
    DWORD dwBytesRead = 0;		//下载的真实数据的长度

	//判断N3D文件的版本号
 	if (!m_bVersionInfoDown && !m_bTerminated)
 	{
 		GetN3DVersion();

		if (m_N3DVersion == 0xffffffff)
		{
			::MessageBox(GetActiveWindow(), 
				TEXT("N3D文件版本号不合法"),
				TEXT("错误"),
				MB_OK | MB_ICONINFORMATION);

			//退出程序
			m_bTerminated = TRUE;
			m_bOutDate  = TRUE;
			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
			return FALSE;
		} 		
 		else if (VGSPlayerVer < m_N3DVersion)  // 如果引擎的版本低于文件的版本
 		{
 			::MessageBox(GetActiveWindow(), 
 						TEXT("升级提示：\n\t您所使用的VGS三维引擎版本太低了，必须要安装更高级的版本才能正常浏览，\n请根据提示自动升级VGS。"), 
 						TEXT("VGS三维引擎需要升级..."),
 						MB_OK | MB_ICONINFORMATION);
 			
 			//退出程序
 			m_bTerminated = TRUE;
 			m_bOutDate  = TRUE;
 			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
 			return FALSE;
 		}
 	}	
 
 	//读入N3D的文件头, ReadN3DHeader()函数自动处理不同版本的N3D文件头
 	if (!ReadN3DHeader())
 	{
 		m_bTerminated = TRUE;
 		PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
 		return FALSE;
 	}
 
 	if (m_N3DVersion >= 51000 && m_N3DVersion <= N3D_CURRENT_V3DVERSION)
 	{
 		//读入N3D文件的场景数据
 		if (!ReadN3DScene())
 		{
 			m_bTerminated = TRUE;
			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
 			return FALSE;
 		}
 
 		//读入N3D文件的图像数据
 		if (!ReadN3DImage())
 		{
 			m_bTerminated = TRUE;
 			PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, TERMINATED, index);
 			return FALSE;
 		}
 		
 		//正常下载完成
 		m_bAllDown = TRUE;
 		//将数据保存到Cache
 		m_bufSize = 10;
 		bResult = InternetReadFile(m_hURL,				// handle to URL
 								   (LPSTR)m_pBuf,		// pointer to  buffer
 								   (DWORD)m_bufSize,	// size of buffer
 								   &dwBytesRead);		// pointer to var to hold return value	
 		
 		PostMessage(m_hWnd, WM_DOWNLOADN3D_PROC, ALL_ISDOWN, index);
 
 		//状态量
 		m_bTerminated = TRUE;
 
 		return TRUE;
 	}

	return TRUE;
}

//----------------------------------------------------------------
//	名称：JudgeFileLink
//	简介：判断网络文件是否存在
//	参数：url:i:连接
//	返回：如果网络文件可以下载，则返回文件尺寸，否则返回-1
//----------------------------------------------------------------
int CDownLoadN3DFile::JudgeFileLink(HINTERNET url)
{	
	DWORD index = 0;
	DWORD readBytes = 32;
	TCHAR  buffer[32] = {0};
	int result = HttpQueryInfo(url, HTTP_QUERY_STATUS_CODE, buffer, &readBytes, &index);

	if(atoi(buffer) != HTTP_STATUS_OK)
		return 0;

	//获取N3D文件大小
	char  size[255] = {0};
	DWORD read = 255;
	result = HttpQueryInfo(url, HTTP_QUERY_CONTENT_LENGTH, size, &read, &index);
	int filesize = atoi(size);
	
	return filesize;
}

//-----------------------------------------------------------------------------
// 方法名: DownLoadHttpFile()
// 参  数:  char* strHttpFile, 被下载的文件地址    
//			char* strDestFile, 保存到本地的文件路径
//		    HWND  hWnd,		   主窗体句柄
//			CSceneResource* pSceneResource  场景资源对象
// 用  途: 下载http文件
//-----------------------------------------------------------------------------
bool CDownLoadN3DFile::DownLoadHttpFile(const string &strHttpFile, const string &strDestFile, HWND hWnd, DWORD pSceneResource)
{
	if (strHttpFile == "" || !m_pDownloadN3DFileManager) return false;
	m_pSceneResource = pSceneResource;

	//要保存文件到本地吗?
	m_strLocalFileName = strDestFile;
	/*if (!strDestFile.empty())
	{
		 strcpy(m_strLocalFileName, strDestFile);
	}*/

	//--------------------------------------------------------------
	//传递变量
	//--------------------------------------------------------------
	// strcpy(m_strNetFileName, strHttpFile);
	m_strNetFileName = strHttpFile;
	m_hWnd = hWnd;
	//strcpy(N3DFile.m_strFileName, strHttpFile);

	//--------------------------------------------------------------
	//获得url对象
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
		string sErr = string("VGS无法连接到 ") + m_strNetFileName + string(" 文件. 可能文件已经不存在，与网站服务商联系可以解决该问题.");
		/*strcpy(m_err, "VGS无法连接到 ");
		strcat(m_err, m_strNetFileName);
		strcat(m_err, " 文件. 可能文件已经不存在，与网站服务商联系可以解决该问题.");*/
		MessageBox(NULL,  (sErr.c_str()), ("正在连接远程文件...错误"), MB_OK | MB_ICONERROR);
		return false;
	}

	/*
	//新建本地文件对象
	m_pLocalFile = fopen(m_strLocalFileName, "wb");
	m_localFileClosed = false;
	if (m_pLocalFile == NULL)
	{
		strcpy(m_err, "打开本地文件 ");
		strcat(m_err, m_strLocalFileName);
		strcat(m_err, "失败。");
		MessageBox(NULL, m_err, "保存下载的文件...错误", MB_ICONASTERISK | MB_OK);
		return false;
	}
*/

	//--------------------------------------------------------------
	//判断网络文件是否可以下载，返回N3D文件大小
	//--------------------------------------------------------------
	m_fileSize = JudgeFileLink(m_hURL);
	if (m_fileSize <= 0)
	{
		string sErr = "正在确认N3D文件的大小," + m_strNetFileName + "...错误!";
		MessageBox(NULL, (sErr.c_str()), ("分析文件大小...错误, 远程服务器文件不存在."), MB_ICONASTERISK | MB_OK);
		return false;
	}

	//--------------------------------------------------------------
	//创建一个下载线程
	//--------------------------------------------------------------
	if (CreateDownThread() == false)
	{
		string sErr = "CreateDownThread() -> error";
		MessageBox(NULL, (sErr.c_str()), ("正在创建下载线程...错误"), MB_ICONASTERISK | MB_OK);
		return false;
	}
	
	//--------------------------------------------------------------
	//发送下载类型的消息
	//--------------------------------------------------------------
	//PostMessage(m_hWnd, WM_DOWNLOAD_PROC, DOWN_TYPE, type);

	return true;
}

// 开始下载
void  CDownLoadN3DFile::StartDownLoad()
{
	if (m_pDownloadN3DFileManager)
	{
		DownLoadHttpFile(m_sFileFullName, "", m_pDownloadN3DFileManager->getMsgWndHandle(), 0);
	}
}

//-----------------------------------------------------------------------------
// 方法名: Release()
// 参  数: 
// 用  途: 释放内存及相关资源
//-----------------------------------------------------------------------------
void  CDownLoadN3DFile::Release()
{
	// m_pScene = NULL;

	//释放N3D文件的过渡缓冲
	N3DFile.Release();

	//释放场景缓冲
	SAFE_DELETE_ARRAY(pSceneDataBuf);

	//释放图像缓冲
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