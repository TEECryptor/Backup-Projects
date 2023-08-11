// VGS2Window.h : CVGS2Window 的声明
//-------------------------------------------------------------------
#pragma once
//-------------------------------------------------------------------
#include "resource.h"       // 主符号
#include <atlhost.h>
#include <Mmsystem.h>
#include <exdisp.h> 
#include <comdef.h>
#include <vector>
#include <map>
// 
#include "VGSGetCore.h"

// //N3D文件下载类头文件
#include ".\\N3D\\CDownLoadN3DFile.h"
// //任意文件下载类头文件
#include ".\\Download\\VGSFileDownloadManager.h"
#include ".\\Download\\FileLoadManager.h"

#include "InvokeJSFunc.h"
// 
// //N3D文件数据解析类头文件
// #include "GetN3DFileInfo_5.h"
// //Flash控件调用头文件
// #include "ATLFlashCtrlUnit.h"
// //包含VGS2引擎头文件
 #include "VGS2DLL.h"
//-------------------------------------------------------------------
class CVGS2Window : 
	public CAxDialogImpl<CVGS2Window>, public CVGSGetCore
{
public:
	//构造析构函数
	CVGS2Window(IOleClientSite* pClientSite, CRITICAL_SECTION* pRenderCS, CRITICAL_SECTION* pCtrlCS);
	~CVGS2Window();
	//-------------------------------------------------------
	enum { IDD = IDD_VGS2WINDOW };
	VOID SetVGS2Interface(CVGS2 *pVGS2);

	//-------------------------------------------------------
	//VGS窗体消息映射
	//-------------------------------------------------------
BEGIN_MSG_MAP(CVGS2Window)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDBClick)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
	MESSAGE_HANDLER(MM_JOY1MOVE, OnMmJoyMove)
//	MESSAGE_HANDLER(WM_DOWNLOADN3D_PROC, OnDownLoadN3DProc)
//	MESSAGE_HANDLER(WM_DOWNLOADFILE_PROC, OnDownLoadFileProc)
//	MESSAGE_HANDLER(WM_JSFUNC_PROC, OnJSFuncProc);
//	MESSAGE_HANDLER(WM_ZTRIGFUNC_PROC,OnZTrigFuncProc);
//  MESSAGE_HANDLER(WM_CANCELMODE, OnCancelMode)
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)

/*
	MESSAGE_HANDLER(VGS_DOWNLOAD_MSG_STATUSCHANGE, OnHttpDownLoad_ChangeStatus)
	MESSAGE_HANDLER(VGS_DOWNLOAD_MSG_DOWNLOADPROGRESS, OnHttpDownLoad_Progress)
	MESSAGE_HANDLER(VGS_DOWNLOAD_EVENT_FILE_DOWNLOADED, OnHttpDownLoad_FileDownloaded)
	MESSAGE_HANDLER(VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED, OnHttpDownLoad_N3DOneImageDownloaded)
	MESSAGE_HANDLER(VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED, OnHttpDownLoad_N3DSceneDownloaded)
*/
	MESSAGE_HANDLER(WM_DOWNLOAD_MSG_FILE_DOWNLOADED, OnDownLoadFileCompletedMT);
	MESSAGE_HANDLER(WM_DOWNLOAD_MSG_DOWNLOADPROGRESS, OnDownLoadFilePercentMT);
	MESSAGE_HANDLER(WM_JSFUNC_MSG_SCENEISDOWN, OnSceneIsDownProc);
	MESSAGE_HANDLER(WM_JSFUNC_MSG_ALLISDOWN, OnAllIsDownProc);
END_MSG_MAP()
	//-------------------------------------------------------
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMmJoyMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDownLoadN3DProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDownLoadFileProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnJSFuncProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT OnZTrigFuncProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// http下载响应消息
	/*
	LRESULT OnHttpDownLoad_ChangeStatus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHttpDownLoad_Progress(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHttpDownLoad_FileDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHttpDownLoad_N3DOneImageDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHttpDownLoad_N3DSceneDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	*/	
	// 多线程下载消息处理
	LRESULT OnDownLoadFileCompletedMT(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDownLoadFilePercentMT(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSceneIsDownProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAllIsDownProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//-------------------------------------------------------	


	//-------------------------------------------------------
	//VGS窗体变量
	//-------------------------------------------------------
public:
    IOleClientSite*				m_pClientSite;					//客户区对象
// 	int							m_iWndWidth;					//窗口宽
// 	int							m_iWndHeight;					//窗口高
// 	RECT						m_ClientRect;					//窗口客户区的Rect
// 
// 	HCURSOR						m_hHandCursor;					//鼠标句柄：手
// 	HCURSOR						m_hDefaultCursor;				//鼠标句柄：缺省
// 	POINT						m_CursorPos;					//光标当前的位置
// 	bool						m_bTerminated;					//当前主线程是否被终止
// 	BOOL						m_bUnitedLogoStartDown;			//联盟的Logo是否已经开始下载

	//-------------------------------------------------------
	//系统变量
	//-------------------------------------------------------
public:
 	HWND						m_IEhWnd;						//IE窗体的句柄
 	HWND						m_VGSWnd;						//VGS窗体的句柄
// 	DWORD						m_timer1;						//当前时钟
// 	DWORD						m_timer2;						//当前时钟
// 	DWORD						m_timeStep;						//当前设置的渲染时间间隔
// 	DWORD						m_UserEventTimeStep;			//用户自定义事件时间间隔
// 
 	BOOL						m_bActive;						//当前窗体是否已经激活
// 	BOOL						m_bStepFinish;					//一个渲染循环结束
// 
	//-------------------------------------------------------
	//下载N3D文件类-CDownLoadN3DFile类变量
	//-------------------------------------------------------
public:	
	//Load的N3D下载对象列表，Load的N3D文件直接加载到场景
 	vector<CDownLoadN3DFile*>   m_LoadN3DObj;	
	BOOL						m_bN3DFileDownloaded;
// 	
// 	//任意文件的下载对象列表, 譬如外部图片，联盟(暂时保留)
//   	CDownLoadFile*				UnitedLogo_pDownLoadFile;
//     //下载
// 	vector<CDownLoadFile*>      m_LoadFiles;
// 	//标志：前台或后台下载N3D对象
// 	INT							m_bDownType;
// 	CDownLoadN3DFile*			m_pCurN3D;						//当前下载的N3D对象
// 
// 	DWORD						m_curDownSize;					//已经下载的N3D数据大小
 	DWORD						m_downText;						//down text
// 	INT							m_percent;						//已经下载的百分比
// 	CHAR						m_strPercent[32];				//m_Percent转化为字符串	
// 	CHAR						m_strDownLoading[256];			//下载过程中显示的字符串			
// 	BOOL						m_bGetSceneDataOK;				//场景数据是否已经下载完
// 
// 	BOOL						m_bCanConstructScene;			//现在是否可以构造场景	
// 	BOOL						m_AllIsOK;						//场景已经下载完成并创建完毕	
// 	BOOL						m_DownedTexIndex;				//当前下载的贴图编号	
// 	BOOL						m_DownedImageIndex;				//当前下载的贴图编号

	//-------------------------------------------------------
	//VGS场景相关变量
	//-------------------------------------------------------
private:
 	// CVGS2DLL					m_VGS2;							//当前的VGS2对象
public:
	BOOL						m_ShowDefaultLogo;				//显示默认的VGS LOGO
// 	BOOL						m_IsCreasing;					//下载开始时的VGS LOGO效果标志位
// 
// 	CONTROLSTATUS				m_controlStatus;				//当前的控制类型
// 
// 	CButton*					m_pButton;						//鼠标最近一次移出的按钮
// 	COverlay*					m_pOverlay;						//鼠标最近一次按下的Overlay
// 	CModel*						m_pCurCursorModel;				//当前光标下方的模型
// 	CModel*						m_pLastCursorModel;				//鼠标最近一次移出的model
// 	CModel*						m_pLastSelectedModel;			//最近一次选择的模型
// 	CModel*						m_pCurSelectedModel;			//当前选中的模型

	//-------------------------------------------------------
	//Flash Ctrl相关变量
	//-------------------------------------------------------
public:
	//vector<CATLFlashCtrl*>		m_Flash;						//Flash列表

	//-------------------------------------------------------
	//JavaScript调用相关变量
	//-------------------------------------------------------
public:
 	CInvokeJSFunc*				invokeJS;						//调用Javascript的对象	
// 	
// 	//VGS引擎调用JS的方法
// 	BSTR						BSTR_EngineStart;				//引擎初始化方法
// 	BSTR						BSTR_SceneIsDown;				//SceneIsDown方法，在场景下载完成时自动调用	
// 	BSTR						BSTR_AllIsDown;					//AllIsDown方法，在场景和贴图全部下载完成时调用
// 	
// 	DISPPARAMS					dispparams;						//执行IE中JavaScript方法的参数
// 
 	BSTR						BSTR_VGSMouseDown;				//VGSMouseUp字符串	
 	BSTR						BSTR_VGSMouseUp;				//VGSMouseUp字符串
 	BSTR						BSTR_VGSMouseMove;
 	BSTR						BSTR_VGSMouseDBClick;
// 
// 
 	BOOL						bEnable_MouseDown_Callback;
 	BOOL						bEnable_MouseUp_Callback;
 	BOOL						bEnable_MouseMove_Callback;
 	BOOL						bEnable_MouseDBClick_Callback;
// 
// 	//用于表示一个事件中的代码是否已经执行完，
// 	//防止在没有执行完时进行第二次调用
// 	BOOL						m_bMouseDownEvent_Finished;
// 	BOOL						m_bMouseUpEvent_Finished;
// 	BOOL						m_bMouseMoveEvent_Finished;
// 	BOOL						m_bMouseDBClickEvent_Finished;
// 	
// 
// 	//当前光标是否选中了Nav的Pointer
// 	BOOL						m_bNavPointerSelected;
// 	CNavigater*					m_pSelectedNavigater;
// 	NavPointer*					m_pSelectedNavPointer;
// 
// 	//在鼠标移移动事件中是否激活ModelUnderLoc检测
// 	BOOL						m_bModelEventOnMouseMove;
// 	//右键菜单显示标记
// 	BOOL                        m_bRightMenu;
// 
// 	HMENU                       hMainMenu;
// 	HMENU                       hPopMenu;
// 
// 
// 	//-------------------------------------------------------
// 	//场景管理相关变量
// 	//-------------------------------------------------------
 	//渲染线程同步对象指针，从CVGS2类传递	
 	CRITICAL_SECTION*			m_pRender_cs;		
 	//控制线程同步对象指针，从CVGS2类传递			
 	CRITICAL_SECTION*			m_pCtrl_cs;						
	
	//-------------------------------------------------------
	//调试用相关变量
	//-------------------------------------------------------
public:
// 	DWORD						modelCntStr;
// 	DWORD						texCntStr;
// 	DWORD						shaderCntStr;
// 	DWORD						triCntStr;
// 	DWORD						curCamFromStr;
// 	DWORD						curCamAtStr;
// 	
// 	char						str_shaderCnt[256];
// 	char						str_modelCnt[256];
// 	char						str_texCnt[256];
// 	char						str_triCnt[256];
// 	char						str_curCamFrom[256];
// 	char						str_curCamAt[256];


public:
	HRESULT						SetCurrentPath(const std::string &sCurrentPath);
	HRESULT                     AddFileToProgressbar(const std::string &sFileURL);
	HRESULT                     DownloadAndLoadN3DFile(const std::string &sURL, const std::string sN3DName);
	HRESULT                     DownloadAndLoadA3DFile(const std::string &sURL, const std::string sA3DName);
	HRESULT						ReadLocalN3DFile(CFileDwonloadInfo *pInfo);
	HRESULT						ReadLocalA3DFile(CFileDwonloadInfo *pInfo);
 	//初始化各种变量
 	HRESULT						InitVars();
 	//初始化三维场景
 	HRESULT						InitScene();
// 	//初始化软键盘控制按钮
// 	VOID						InitControlButton();
 	//初始化其他内容
	VOID						InitOther();
// 	//创建联盟Logo 
// 	VOID						CreateUnitedLogo(CDownLoadFile* pLogoDownLoadFile, RECT* pRect);
// 
// 	//构造当前场景
// 	HRESULT						ConstructCurScene();
// 
// 	//判断鼠标在哪个按钮上方
// 	CButton*					GetMouseOverButton(INT x, INT y);
// 
// 	//设置下载百分比的文字
// 	VOID						SetDownPercentText();
// 
// 	//停止并释放N3D下载队列
// 	VOID						DestroyN3D();
// 	//停止并释放filedownload下载队列
// 	VOID                        DestroyDownloadFile();
// 	//释放Flash资源
// 	VOID						DestroyFlash();
// 	//得到临时目录
// 	VOID						GetTmpFolder(char*  );
// 	//自定义的鼠标移动消息
// 	VOID						UserMouseMoveEvent();
// 	//Flash播放到最后一帧的事件
// 	VOID						FlashPlayOverEvent();
// 	//Flash下载完成的事件
// 	VOID						FlashLoadOverEvent();
// 	//更新FlashRect
// 	VOID						UpdateFlashRect();
// 	//Add by Baojiahui 2009/1/9-------------------------
// 	//更新flash中的信号量信息
// 	VOID                        UpdateFlashInfo();
// 	//显示场景详细信息
// 	VOID						ShowSceneDetailInfo();
// 	//为场景排序                
// 	VOID                        SortTheModelList();
// 	
// 	//删除已经下载完的下载对象
// 	void						EraseN3DDownloader(CDownLoadN3DFile* pN3D);
//     //删除已经下载完的下载文件对象
// 	void                        EraseFileDownloader(CDownLoadFile* pDFile);
// 	
// 	//------------------------------------------------------------------------------
// 	//Invoke JS 的函数
// 	//------------------------------------------------------------------------------
 	HRESULT				SceneIsDownInvoke(const std::string &sN3DName);
 	HRESULT				AllIsDownInvoke(const std::string &sN3DName);
// 	HRESULT				MouseDownInvoke();
// 	HRESULT				MouseUpInvoke();
 	HRESULT				MouseDBClickInvoke(int x, int y);
// 	HRESULT				MouseMoveInvoke();
// 	HRESULT				ButtonClickInvoke();
// 	HRESULT				DownloadInvoke(CHAR* strPercent);
// 
// 	LRESULT OnCancelMode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// 	{
// 		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		//return 0;
// 	}
	//---------------------------------------------------------------
public:
	// CVGS2DLL* getVGSObject(){return &m_VGS2;}
	// 回调js脚本入口函数，开始执行脚本
	VOID EnableStartEngineCall();
	// 调用引擎开始的脚本
	LRESULT InvokeStartEngine();

	CVGSFileDownloadManager& GetDownloadN3DFileManager(){return m_DownloadFileManagerMT;}

	virtual VOID   SetVGSCore(CVGS2DLL *pVGSCore);
	//---------------------------------------------------------------
private:
	// 判断窗口是否是flash窗口
	bool IsFlashWnd(HWND wnd);
	// 创建一张图片纹理
	void CreateN3DImageTexture(CVGSN3DFileDownloadStreamTask *pN3DTask, int imgIdx);
	// 当前已经完成的进度	
	DWORD	GetProgressbarFinishedBytes();				
	// 删除进度条
	void	DeleteProgressbar();
	// 处理进度显示
	void	ProcessProgress(float percent);
	//---------------------------------------------------------------
private:
	DISPPARAMS					dispparams;					//执行IE中JavaScript方法的参数
	string						m_sCurrentURLPath;			// 当前URL目录
	CVGSFileDownloadManager		m_DownloadFileManagerMT;	// 文件下载任务管理器
	CFileLoadManager			m_FileLoadManagerMT;		// 加载资源的管理器
	vector<CFileDwonloadInfo*>	m_vFileDownloadInfoMT;		// 多线程下载的文件下载信息
	set<string>					m_vFilesUseProgressbar;		// 使用进度条的文件列表
	DWORD						m_dwTotalProgressbar;       // 进度条的总进度
	//---------------------------------------------------------------
/*
 * For Stream downloading, don't support still
*/
/*
	// n3d 任务信息
	typedef struct CTaskCreateInfo
	{
		DWORD imageCount;
		set<int> setImageTobeCreated;
		set<int> setImageCreated;
		bool  bSceneCreated;	
		CTaskCreateInfo()
		{
			imageCount = 0;
			bSceneCreated = false;
		}
	};

	map<DWORD, CTaskCreateInfo> m_mapN3DTaskCreateInfo; // N3D任务信息
*/
};
//-------------------------------------------------------------------



