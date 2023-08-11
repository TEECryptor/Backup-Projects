// CDownLoadFile.h: interface for the CDownLoadFile class.
// 下载普通文件的类
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

//自定义消息
#define WM_DOWNLOADFILE_PROC		(WM_USER + 120)		//文件下载过程中发送给主窗口的消息
enum downloadType
{
	TYPE_LOGOFILE,
	TYPE_IMAGEFILE,
	TYPE_OTHERFILE
};


class CDownLoadFile  
{
	//----------------------------------------------------------------------------
	//网络下载相关的系统变量
	//----------------------------------------------------------------------------
private:
	HINTERNET				m_hInternetSession;		//网络会话对象
    HINTERNET				m_hURL;					//URL句柄	
	HWND					m_hWnd;					//主窗体句柄
	DWORD					m_threadID;				//当前下载线程ID

	char					m_strHost[256];			//服务器http地址
	char					m_strFloder[256];		//文件根目录
	char					m_strTmpFloder[256];	//IE临时目录

	char					m_strNetFileName[256];	//要下载的文件
	DWORD					m_bufSize;				//每次下载的数据尺寸，默认为1024Byte
	char					m_pBuf[1024];			//每次下载的数据缓冲

	bool					m_bConnected;			//是否已经和http服务器连接

	//FILE*					m_pLocalFile;			//下载到本地的文件指针
	char					m_strLocalFileName[256];//下载后的完整文件名
	bool					m_localFileClosed;		//下载写入本地的文件是否已经close

	char					m_err[256];				//承载error信息的缓冲

public:
	HANDLE					m_threadHandle;			//当前线程的Handle
	bool					m_bTerminated;			//是否下载过程终止(1.下载过程发生异常2.下载文件完成)

	BOOL					m_bExecuteOver;			//线程执行的Execute函数结束
  
	//File something
	char                    file[512];
	UINT                    index;
	DWORD					curPos;					//当前下载N3D文件的偏移量=已经下载的数据字节长度
	DWORD					m_fileSize;				//下载的文件的大小
	BYTE*					m_fileBuf;				//文件的缓冲

	BOOL					m_bLogoFile;			//是否是Logo文件
	BOOL					m_bAllDown;				//所有的数据都已经下载
	downloadType            m_DownType;             //下载文件的类型属性

	DWORD                   m_DownForObj;           //下载内容所属于的对象指针
	
	//----------------------------------------------------------------------------
	//下载及解析数据的方法
	//----------------------------------------------------------------------------
public:
	//创建一个新的网络会话对象
	// BOOL					CreateInternetSession();
	void                    SetInternetSession(HINTERNET hInternetSession){m_hInternetSession = hInternetSession;}
	//删除网络会话对象
	VOID					ReleaseSession();
	//下载http文件
	bool					DownLoadHttpFile(char* strHttpFile, char* strDestFile, HWND hWnd, INT type);
	//创建一个下载线程
	bool					CreateDownThread();
	//判断文件是否存在
	int						JudgeFileLink(HINTERNET url);

	//读取网络文件数据到内存和本地文件
	BOOL					ReadFile();
	//线程执行函数
	DWORD					Execute();
	//终止下载线程
	void					EndDownThread();
	//释放内存及相关资源
	void					Release();

public:
	CDownLoadFile();
	~CDownLoadFile();

};

#endif // !defined(AFX_CDOWNLOADFILE_H__35033E06_69C7_448A_AE2B_4C3437C02FF6__INCLUDED_)
