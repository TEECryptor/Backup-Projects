/////////////////////////////////////////////////////////////////////////////////////////
// 名  称: CDownLoadWebFile
// 描  述: 通过CDownLoadWebFile类从Web服务器上下载文件的类，
//		   通过CGetN3DFileInfo类解析下载的N3D文件，并插入预渲染队列。
// 作  者: lixin
// 日  期: 2006-10-17
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
//自定义消息
#define WM_DOWNLOADN3D_PROC		(WM_USER + 118)		//文件下载过程中发送给主窗口的消息
*/
//用于发送给主窗口消息时附带的参数
enum 
{	
	DOWN_TYPE,					//下载数据的类型，前台0和后台1下载
	HEADER_ISDOWN,				//Header信息下载是否完成
	SCENE_ISDOWN,				//场景信息下载是否完成
	ONETEX_ISDOWN,				//一个贴图信息下载是否完成
	ALLTEX_ISDOWN,				//所有贴图信息下载是否完成
	ONEIMAGE_ISDOWN,			//一个Image信息下载是否完成
	ALLIMAGE_ISDOWN,			//所有Image信息下载是否完成
	ALL_ISDOWN,					//所有N3D信息下载是否完成
	IS_DOWN,					//正在下载数据，显示下载进度
	TERMINATED					//下载线程关闭
};

// 下载状态
enum VGS_DOWNLOAD_STATUS
{	
	VGS_DOWNLOAD_STATUS_NONE = 0,
	VGS_DOWNLOAD_STATUS_VERSION,            // 正在获取文件版本号
	VGS_DOWNLOAD_STATUS_HEADER,				// 正在下载文件头
	VGS_DOWNLOAD_STATUS_SCENE,				// 正在下载场景
	VGS_DOWNLOAD_STATUS_IMAGE,			    // 正在下载图像
	VGS_DOWNLOAD_STATUS_ALLISDOWN,			// 所有N3D信息下载完成
	VGS_DOWNLOAD_STATUS_TERMINATED			// 下载线程关闭
};

class CDownLoadN3DFile  : public CVGSGetCore
{
	//----------------------------------------------------------------------------
	//Load N3D方法的变量
	//----------------------------------------------------------------------------
public:
	//char					name[32];				//下载对象的名称,JSSet
	//char					file[512];				//文件地址,JSSet
	string					m_sName;
	string					m_sFileFullName;         // 下载文件对象的完整路径
	INT						index;					//当前下载对象的编号

	//LoadN3DFile为下载并加载到场景， type = 0;
	//PreloadN3DFile为下载但不加载到场景 type = 1;
	INT						type;		

	VGS_DOWNLOAD_STATUS     m_downloadStatus;              // 下载状态
	
	DWORD					lastTimer;				//时钟

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

	// char					m_strNetFileName[256];	//要下载的文件
	string                  m_strNetFileName;
	DWORD					m_bufSize;				//每次下载的数据尺寸，默认为1024Byte
	char					m_pBuf[1024];			//每次下载的数据缓冲

	bool					m_bConnected;			//是否已经和http服务器连接

	//FILE*					m_pLocalFile;			//下载到本地的文件指针
	// char					m_strLocalFileName[256];//下载后的完整文件名
	string                  m_strLocalFileName;
	bool					m_localFileClosed;		//下载写入本地的文件是否已经close

	// char					m_err[256];				//承载error信息的缓冲
	// string                  m_sErr;

	int						m_reconnectMaxCnt;		//如果下载的过程中断线，重新连接的最大次数
	int						m_curReconnectIndex;	//当前连接的次数

	CDownloadN3DFileManager*  m_pDownloadN3DFileManager; // 下载管理器指针

public:
	HANDLE					m_threadHandle;			//当前线程的Handle
	bool					m_bTerminated;			//是否下载过程终止(1.下载过程发生异常2.下载文件完成)

	BOOL					m_bExecuteOver;			//线程执行的Execute函数结束
	//----------------------------------------------------------------------------
	//N3D文件下载相关的变量
	//----------------------------------------------------------------------------
public:
	FLOAT					VGSPlayerVer;			//VGS Player 引擎播放器的版本号
	BOOL					m_bOutDate;				//引擎已经过期

	//File something
	DWORD					curPos;					//当前下载N3D文件的偏移量=已经下载的数据字节长度
	DWORD					m_fileSize;				//下载的文件的大小

	//Version Something
	DWORD					m_N3DVersion;				//文件的版本号
	BOOL					m_bVersionInfoDown;		//版本相关数据是否已经下载完
	BOOL					m_bFileVerErr;			//文件版本不符

	//Header Something...
    N3D_Header              m_Header_5_1;           //5.1版本文件头
	BOOL					m_bHeaderDown;			//文件头数据是否已经下载完

	//Scene Something...
	//DWORD					m_ZipSceneDataSize;		//压缩后的场景数据大小
	DWORD					m_SceneDataDownedByte;  //已经下载的场景数据大小
	BYTE*					pSceneDataBuf;			//保存N3D文件场景数据的下载缓冲，读取http上的N3D文件的场景数据段到 pSceneDataBuf
	BOOL					m_bSceneDown;			//当前N3D文件的场景信息是否已经下载完


	//Texture Something...
	DWORD					m_TexCnt;				//N3D文件中的贴图数量
	DWORD					m_DownedTexCnt;			//当前已经下载的贴图数量
	BOOL					m_bAllTexDown;			//当前N3D文件是否下载完全部贴图
	BOOL					m_bGetTexCnt;			//是否获得当前N3D文件中的贴图数量

	BOOL					m_bGetCurTexInfo;		//是否得到当前的贴图的参数信息
	DWORD					m_CurTexDataDownedByte; //当前贴图文件已经下载的数据大小
	BOOL					m_bCurTexDown;			//当前N3D文件是否下载完一个完整的贴图

	//Image Something...
	DWORD					m_ImageCnt;				//图像个数
	BOOL					m_bGetImageCnt;			//是否获得了当前Image对象的个数
	DWORD					m_DownedImageCnt;		//当前已经下载的图像个数
	BOOL					m_bAllImageDown;		//当前N3D文件是否下载完全部图像资源
    N3D_5_1_ZipImageData*   pImageDataBuf;
	N3D_5_1_ZipImageData*	pImageDataBuf_5_1;		//读取http上的N3D文件的所有Image数据段
	
	BOOL	                m_bGetImageNameSize;
	BOOL                    m_bGetImageName;
	UINT                    m_CurImageNameSize;
	BOOL					m_bGetCurImageInfo;		//是否得到了当前Image数据参数
	DWORD					m_CurImageDataDownedByte; //当前图像数据已经下载的数据大小
	BOOL					m_bCurImageDown;		//当前N3D文件是否下载完一个Image


	DWORD					m_CurN3D_3_1_DownedByte; //当前N3D_3_1文件已经下载的字节数
	bool					m_bAllDown;				//当前N3D文件是否已经全部下载完
	//----------------------------------------------------------------------------
	//解析N3D文件的对象
	//----------------------------------------------------------------------------
public:
	DWORD					m_pScene;				//场景对象指针
	DWORD			m_pSceneResource;		//场景资源对象
	CGetN3DFileInfo_5	    N3DFile;				//N3D数据解析对象

	//控制线程同步对象指针，从CVGS2类传递			
	CRITICAL_SECTION*		m_pCtrl_cs;	
	CRITICAL_SECTION*		m_pRender_cs;

	//----------------------------------------------------------------------------
	//下载及解析数据的方法
	//----------------------------------------------------------------------------
public:
	// 开始下载
	void                    StartDownLoad();
	//下载http文件
	bool					DownLoadHttpFile(const string &strHttpFile, const string &strDestFile, HWND hWnd, DWORD pSceneResource);
	//创建一个下载线程
	bool					CreateDownThread();
	//判断文件是否存在
	int						JudgeFileLink(HINTERNET url);

	//读取网络N3D文件数据到内存和本地文件
	BOOL					ReadN3DFile();

	//3.1以上版本直接读取网络N3D文件，全部下载完后再加入场景
	//BOOL					ReadN3D_3_1_File();
	
	//判断N3D文件的版本号
	DWORD					GetN3DVersion();

	//读入N3D的文件头
	BOOL					ReadN3DHeader();

	//读入N3D的场景数据
	BOOL					ReadN3DScene();
	//读入3.1以下版本的N3D场景数据
//	BOOL					ReadN3D_3_0_Scene();
	//读入3.1版本的N3D场景数据
	//BOOL					ReadN3D_3_1_Scene();

	//读入场景的静态贴图数据
	//BOOL					ReadN3DTexture();
	//读入3.1以下版本的N3D贴图数据
	//BOOL					ReadN3D_3_0_Texture();
	//读入3.1版本的N3D贴图数据
	//BOOL					ReadN3D_3_1_Texture();

	//读入场景中的Image数据
	BOOL					ReadN3DImage();
	//读入3.1以下版本的Image数据
	//BOOL					ReadN3D_3_0_Image();
	//读入3.1版本的Image数据
	//BOOL					ReadN3D_3_1_Image();

	//读入当前场景中的动态贴图数据
	//BOOL					ReadN3DDyTex();
	//读入3.1版本的动态贴图数据
	//BOOL					ReadN3D_3_1_DyTex();


	//线程执行函数
	DWORD					Execute();
	//终止下载线程
	void					EndDownThread();
	//释放内存及相关资源
	void					Release();


	//本N3D文件下载对应JS Callback函数是否已经调用
	BOOL					bSceneIsDownInvoked;  
	BOOL					bAllIsDownInvoked;

public:
	CDownLoadN3DFile(CDownloadN3DFileManager*  pDownloadN3DFileManager, CRITICAL_SECTION* pCtrl_cs,  CRITICAL_SECTION* pRender_cs);
	~CDownLoadN3DFile();

	virtual VOID   SetVGSCore(CVGS2DLL *pVGSCore);

};

#endif