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
	m_bTerminated = FALSE;			//是否下载过程终止(1.下载过程发生异常2.下载文件完成)
	m_bLogoFile = FALSE;
	m_bExecuteOver = FALSE;			//线程执行的Execute函数结束

	curPos = 0;						//当前下载N3D文件的偏移量=已经下载的数据字节长度
	m_fileSize = 0;					//下载的文件的大小
	m_fileBuf = NULL;				//文件的缓冲
	m_bufSize = 1024;

	m_DownForObj = NULL;

	m_bAllDown = FALSE;				//所有的数据都已经下载
	ZeroMemory(file, sizeof(file)); //初始化下载地址

	//创建一个新的网络会话对象
	//CreateInternetSession();
}

CDownLoadFile::~CDownLoadFile()
{

}

//-----------------------------------------------------------------------------
// 方法名: CreateInternetSession
// 参  数: 
// 用  途: 创建网络会话对象
//-----------------------------------------------------------------------------
//BOOL CDownLoadFile::CreateInternetSession()
//{
//	创建网络会话对象
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
//	名称：DownThreadFunc
//	简介：线程函数
//	参数：impl:i:传入参数
//	返回：
//----------------------------------------------------------------
DWORD WINAPI DownLoadFileThreadFunc(LPVOID impl)
{
	return ((CDownLoadFile*)impl)->Execute();
}

//----------------------------------------------------------------
//	名称：Execute
//	简介：线程执行函数
//	参数：无
//	返回：
//----------------------------------------------------------------
DWORD CDownLoadFile::Execute()
{
	while(!m_bTerminated)
		ReadFile();

	EndDownThread();

	m_bExecuteOver = TRUE;

	return 0;
}

//读取网络文件
BOOL CDownLoadFile::ReadFile()
{
	BOOL bResult = FALSE;
	DWORD dwBytesRead = 0;

	while(!m_bAllDown)
	{
		//如果数据不够1K
		if (m_fileSize <= 1024)
		{
			m_bufSize = m_fileSize;
			bResult = InternetReadFile(m_hURL,				// handle to URL
									  (LPSTR)m_pBuf,		// pointer to  buffer
									  (DWORD)m_bufSize,	    // size of buffer 
									  &dwBytesRead);		// pointer to var to hold return value
			if (bResult && (dwBytesRead == m_bufSize) )
			{
				//拷贝数据到 m_fileBuf
				memcpy(m_fileBuf, m_pBuf, m_bufSize);

				//清空数据缓存
				curPos += m_bufSize;
				ZeroMemory(m_pBuf, 1024);
				m_bufSize = 1024;  
				dwBytesRead = 0;

				//场景已经下载完
				m_bAllDown = TRUE;
				m_bTerminated = TRUE;

				//发送消息到主窗口
				//if (m_bLogoFile)
				//	PostMessage(m_hWnd, WM_DOWNLOADFILE_PROC, TYPE_LOGOFILE, 0);
				//else
					PostMessage(m_hWnd, WM_DOWNLOADFILE_PROC, m_DownType, index);

				Sleep(0);

				//写入本地文件
				//if (m_pLocalFile != NULL)
				//	fwrite(m_fileBuf, m_bufSize, 1, m_pLocalFile);	
				
			}
			//下载失败
			else
				return FALSE;
		}
		//如果数据大于1024字节
		else
		{
			bResult = InternetReadFile(m_hURL,				// handle to URL
									   (LPSTR)m_pBuf,		// pointer to  buffer
									   (DWORD)m_bufSize,	// size of buffer
									   &dwBytesRead);		// pointer to var to hold return value	
			
			if (bResult && (m_bufSize == dwBytesRead))
			{
				//判断当前N3D_3_1数据是否已经下载完--已经下载完成
				if (curPos == m_fileSize )
				{
					//清空数据缓存
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;

					//场景已经下载完
					m_bAllDown = TRUE;
					m_bTerminated = TRUE;
				
					//发送消息到主窗口
					//if (m_bLogoFile)
					PostMessage(m_hWnd, WM_DOWNLOADFILE_PROC, m_DownType, index);
					//else
						//PostMessage(m_hWnd, WM_DOWNLOADFILE_PROC, TYPE_OTHERFILE, 0);
					Sleep(0);

					//写入本地文件
					//if (m_pLocalFile != NULL)
					//	fwrite(pTexDataBuf[m_DownedTexCnt].Data, m_CurTexDataSize, 1, m_pLocalFile);	
				}
				//还没有下载完成
				else
				{
					//拷贝数据到pBuf
					for (DWORD i = 0; i< m_bufSize; i++)
						m_fileBuf[curPos + i] = m_pBuf[i];
					
					//清空数据缓存
					curPos += m_bufSize;
					ZeroMemory(m_pBuf, 1024);
					m_bufSize = 1024;  
					dwBytesRead = 0;

					//判断当前贴图数据是否已经将要下载完成，最后一个循环
					if (m_fileSize - curPos < m_bufSize)
						m_bufSize = m_fileSize - curPos;
					
					Sleep(0);
				}
			}
			//下载数据失败，跳出，等待下一次循环时尝试继续下载
			else
				return FALSE;	
		}
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// 方法名: CreateDownThread
// 参  数: 
// 用  途: 创建一个下载线程
//-----------------------------------------------------------------------------
bool CDownLoadFile::CreateDownThread()
{
	m_threadHandle = CreateThread(NULL, 0, DownLoadFileThreadFunc, (LPVOID)this, 0, &m_threadID);
	if (m_threadHandle == NULL) return false;
	return true;
}	

//-----------------------------------------------------------------------------
// 方法名: DownLoadHttpFile()
// 参  数:  char* strHttpFile, 被下载的文件地址    
//			char* strDestFile, 保存到本地的文件路径
//		    HWND  hWnd,		   主窗体句柄
// 用  途: 下载http文件
//-----------------------------------------------------------------------------
bool CDownLoadFile::DownLoadHttpFile(char* strHttpFile, char* strDestFile, HWND hWnd, INT type)
{
	if(strHttpFile == NULL) return false;

	//要保存文件到本地吗?
	if (strDestFile != NULL)
		strcpy(m_strLocalFileName, strDestFile);

	//--------------------------------------------------------------
	//传递变量
	//--------------------------------------------------------------
	strcpy(m_strNetFileName, strHttpFile);
	m_hWnd = hWnd;

	//--------------------------------------------------------------
	//获得url对象
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
		//strcpy(m_err, "Now confirm size of file ");
		//strcat(m_err, m_strNetFileName);
		//strcat(m_err, " error");
		//MessageBox(NULL, m_err, "Analasys file size...error, the file is not exist.", MB_ICONASTERISK | MB_OK);
		return false;
	}

	//--------------------------------------------------------------
	//创建一个下载线程
	//--------------------------------------------------------------
	if (CreateDownThread() == false)
	{
		//strcpy(m_err, "CreateDownThread() -> error");
		//MessageBox(NULL, m_err, "Now create download thread...error", MB_ICONASTERISK | MB_OK);
		return false;
	}
	
	//初始化缓存
	m_fileBuf = new BYTE[m_fileSize];
	//--------------------------------------------------------------
	//发送下载类型的消息
	//--------------------------------------------------------------
	//PostMessage(m_hWnd, WM_DOWNLOAD_PROC, DOWN_TYPE, type);

	return true;
}

//-----------------------------------------------------------------------------
// 方法名: EndDownThread
// 参  数: 
// 用  途: 终止一个下载线程
//-----------------------------------------------------------------------------
void CDownLoadFile::EndDownThread()
{
	//断开网络连接
    InternetCloseHandle(m_hURL);
	
	//释放会话
	ReleaseSession();

	//删除线程
	DWORD ExitCode;
	if (GetExitCodeThread(m_threadHandle, &ExitCode))
		ExitThread(ExitCode);
	
}

//----------------------------------------------------------------
//	名称：JudgeFileLink
//	简介：判断网络文件是否存在
//	参数：url:i:连接
//	返回：如果网络文件可以下载，则返回文件尺寸，否则返回-1
//----------------------------------------------------------------
int CDownLoadFile::JudgeFileLink(HINTERNET url)
{	
	DWORD index = 0;
	DWORD readBytes = 32;
	char  buffer[32] = {0};
	int result = HttpQueryInfo(url, HTTP_QUERY_STATUS_CODE, buffer, &readBytes, &index);

	if(atoi(buffer) != HTTP_STATUS_OK)
		return 0;

	//获取文件大小
	char  size[255] = {0};
	DWORD read = 255;
	result = HttpQueryInfo(url, HTTP_QUERY_CONTENT_LENGTH, size, &read, &index);
	int filesize = atoi(size);
	
	return filesize;
}

//----------------------------------------------------------------
// 方法名: ReleaseSession
// 参  数: 
// 用  途: 关闭及释放网络会话对象
//----------------------------------------------------------------
VOID CDownLoadFile::ReleaseSession()
{
	if(m_hInternetSession)
		InternetCloseHandle(m_hInternetSession);
}

//-----------------------------------------------------------------------------
// 方法名: Release()
// 参  数: 
// 用  途: 释放内存及相关资源
//-----------------------------------------------------------------------------
void  CDownLoadFile::Release()
{
	SAFE_DELETE_ARRAY(m_fileBuf);
	//fclose(m_pLocalFile);
	//m_pLocalFile = NULL;
}