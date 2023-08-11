//------------------------------------------------------------------------
// VGSCore.h头文件
// 描述：VGS引擎库接口
// 作者: fuxb
// 日期: 2008 -3 -1
//------------------------------------------------------------------------

#pragma once

#include "VGSDef.h"
 
// #include "VGSHeader.h"
#include <stdio.h>
#include <windows.h>

// #include "root.h"
#include <vector>
#include <set>

using namespace std;

class CRoot;
class CScene;
class CMainFrameListener;
//class LuaEngine;
//class LuaState;

// 用于webmax的专有接口
// #if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))

#include "lua/LuaEngine.h"
// #include "GfxManager.h"
#include "VGSRender.h"

class VGS_EXPORT CVGSCore
{
public:
	CVGSCore();
	~CVGSCore();

	CVGSRender& getVGSRender() { return g_VGSRender;}
public:
	//---------渲染线程同步对象---------------
	//用途: 该线程同步对象用于防止渲染线程在渲染时，渲染队列被其它线程修改
	CRITICAL_SECTION			m_render_cs;	
	//---------------------------------------

	//---------控制线程同步对象---------------
	//用途: 该线程同步对象用于防止多个线程同时取得顶点缓冲、贴图表面的控制权
	CRITICAL_SECTION			m_ctrl_cs;	

	//-------------------------------------------------------------------------------------------
	//***系统变量
	//-------------------------------------------------------------------------------------------
	CVGSRender g_VGSRender;  // 渲染器对象

	CRoot*                       m_pRoot;
	//VGS窗体列表
	// vector<CVGSTestWindow*>	g_vWindow; 
	//VGS场景列表						
	vector<CScene*>		g_vScene;					

	BOOL                g_bSceneDirty;     //  场景数据需要更新

	// CRoot*              g_pRoot = NULL;   // 引擎的root

	std::set<std::string>    g_ResourcePath;   // 资源的搜索路径

	//-------------------------------------------------------------------------------------------
	//时钟相关变量
	//-------------------------------------------------------------------------------------------
	UINT				lastFrameTimer;						//上一帧渲染结束的时间


	INT					frameCnt;

	//用于记录当前场景在使用的图像文件名.
	set<string>	g_setImage;

	//引擎是否初始化.
	bool mInitialized;

	//for test 2010-6-1
// 	GfxMovieViewPtr m_ptrSwfView;
	//引擎的事件处理主函数
	void MessageWndProc(UINT message, WPARAM wParam, LPARAM lParam);
public:

//-------------------------------------------------------------------------------------------
///引擎初始化及建立VGS场景相关的方法
//-------------------------------------------------------------------------------------------
//创建VGS标准窗体，如果parentWnd为NULL，则创建Overlap窗体;否则创建子窗体
/*DLL_EXPORT*/ HWND CreateVGSWindow(RECT* rect, HWND parentWnd);

/**初始化VGS引擎，必须先调用该方法，才能调用CreateScene
@param bWritelog in : 是否保存log文件。如果设为true，将会在dll同一目录下生成vgs.log文件
//创建成功返回TUR, 否则返回FALSE
*/
/*DLL_EXPORT*/ BOOL InitVGS(HWND hWnd, INT browserType = 0, BOOL bWritelog = FALSE);
//退出VGS引擎
/*DLL_EXPORT*/ VOID ExitVGS();

/** 设置浏览器类型
@param type INT : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
*/
/*DLL_EXPORT*/ VOID SetWebBrowserType(INT type);

/** 获取浏览器类型
@return : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
*/
/*DLL_EXPORT*/ INT  GetWebBrowserType();

/** 设置IE浏览器的子类型
@param type INT : 0 IE, 1 maxthon, 2 tencent
*/
/*DLL_EXPORT*/ VOID SetWebBowserSubtype(INT subType);

/** 获取IE浏览器的子类型
@return : 0 IE, 1 maxthon, 2 tencent
*/
/*DLL_EXPORT*/ INT  GetWebBowserSubtype();


/** 创建VGS场景
@param in name 要创建的VGS场景名称
@return 如果创建成功，返回VGS场景的指针；否则返回0
@remarks 可以创建多个VGS场景
*/
/*DLL_EXPORT*/ DWORD CreateScene(CHAR* name);

//给场景重新编号, 只有在删除场景后才需要调用
/*DLL_EXPORT*/ VOID ResetSceneIndex();

/**清除VGS场景
@param in sceneHandle VGS场景指针
*/
/*DLL_EXPORT*/ VOID	 DeleteScene(DWORD sceneHandle);
/** 清空VGS场景内容
@param in sceneHandle VGS场景指针
*/
/*DLL_EXPORT*/ VOID	 ResetScene(DWORD sceneHandle);

/** 删除所有VGS场景
*/
/*DLL_EXPORT*/ VOID	 DeleteAllScene();

/**获得场景的数量*/
/*DLL_EXPORT*/ INT	GetSceneCount();

/**通过编号获得场景
@param in index VGS场景管理器索引
@return VGS场景管理器指针
*/
/*DLL_EXPORT*/ DWORD GetSceneHandleByIndex(INT index);
/**通过名称获得场景
@param in name VGS场景管理器名字
@return VGS场景管理器指针
*/
/*DLL_EXPORT*/ DWORD GetSceneHandleByName(CHAR* name);

/**通过名称句柄获得场景名字
@param in sceneHandle VGS场景管理器指针
@param in&out name VGS场景管理器名字
*/
/*DLL_EXPORT*/ VOID GetSceneNameByHandle(DWORD sceneHandle, CHAR* name);

/** 通过句柄索引号获取场景名称
@param in index 场景索引号
@param in/out name 场景名称，最长MAX_NAMESTR_LENGTH个字节 
@return 得到场景名称返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetSceneNameByIndex(INT index, CHAR* name);

/** 通过句柄获得场景的索引
@param in sceneHandle VGS场景管理器指针
@return VGS场景管理器索引,句柄无效返回-1
*/
/*DLL_EXPORT*/ INT GetSceneIndexByHandle(DWORD sceneHandle);


//获得场景中编辑状态下坐标轴的选择状态
/*DLL_EXPORT*/ UINT GetSceneEditAxisStatus(DWORD sceneHandle);

/////////////////////// 渲染窗口 //////////////////////////////////
/** 创建渲染窗体，供测试用
@remarks 一个场景可以包含多个渲染窗体
*/
/*DLL_EXPORT*/ DWORD CreateTestRenderWindow(DWORD sceneHandle, HWND hRenderWnd);

/** 创建渲染窗体
@param sceneHandle in VGS场景
@param hExternalWnd in 要VGS渲染窗体所嵌入的窗体句柄
@param Name in 创建的渲染窗体名字
@return 如果创建成功，返回渲染窗体指针，否则返回0
@remarks 一个场景可以包含多个渲染窗体
*/
/*DLL_EXPORT*/ DWORD CreateRenderWindow(DWORD sceneHandle, HWND hExternalWnd, CHAR* Name);

/**删除指定的VGS场景
@param sceneHandle in VGS场景管理器指针
@param windowHandle in 要删除的VGS窗口句柄
*/
/*DLL_EXPORT*/ VOID DeleteRenderWindow(DWORD sceneHandle, DWORD windowHandle);

/** 创建渲染窗口内容的缩略图
@param sceneHandle  in : 场景指针
@param pRenderWnd   in : 渲染窗体指针
@param sFileName    in : 要保存的缩略图文件名称，包含路径和扩展名
*/
/*DLL_EXPORT*/ VOID CreateRenderwindowThumb(DWORD sceneHandle, DWORD pRenderWnd, const CHAR *sFileName);

//设置渲染窗体的投影模式
///*DLL_EXPORT*/ VOID SetRenderWindowProjMode(DWORD sceneHandle, DWORD swapChainHandle, INT projMode);
//获得渲染窗体的投影模式
///*DLL_EXPORT*/ INT GetRenderWindowProjMode(DWORD sceneHandle, DWORD swapChainHandle);

/** 该函数已经废弃. 设置场景的渲染窗口是否可以响应鼠标、键盘、手柄的控制
@param in sceneHandle : 场景指针
@param in RenderWndHandle : 渲染窗口指针
@param in WndHandle : 响应消息的窗体，通常是主窗体
*/
/*DLL_EXPORT*/ VOID CreateWndControl(DWORD WndHandle, DWORD sceneHandle, DWORD RenderWndHandle);

/** 设置场景的控制状态
@param bEnable in TRUE:场景可通过鼠标键盘等控制，FALSE：不能控制
*/
// /*DLL_EXPORT*/ VOID SetWndControlStatus(BOOL bKeyboardEnable, BOOL bMouseEnable);
// /*DLL_EXPORT*/ VOID SetWndControlStatus(BOOL bEnable);

/** 设置场景是否可以键盘控制
*/
/*DLL_EXPORT*/ VOID SetWndControlKeyboardStatus(BOOL bEnable);

/** 设置场景是否可以鼠标控制
*/
/*DLL_EXPORT*/ VOID SetWndControlMouseStatus(BOOL bEnable);

/** 设置场景的控制状态
@param out: 场景是否可通过鼠标键盘等控制，FALSE：不能控制
*/
// /*DLL_EXPORT*/ VOID GetWndControlStatus(BOOL &bKeyboardEnable, BOOL &bMouseEnable);
// /*DLL_EXPORT*/ BOOL GetWndControlStatus();

/** 获取场景是否可以键盘控制
*/
/*DLL_EXPORT*/ BOOL GetWndControlKeyboardStatus();

/** 获取场景是否可以鼠标控制
*/
/*DLL_EXPORT*/ BOOL GetWndControlMouseStatus();

////////////////// 资源路径 //////////////////////
/** 添加资源文件搜索路径
*/ 
/*DLL_EXPORT*/ VOID AddResourcePath(const CHAR * sFilePath);

/** 删除资源文件搜索路径
*/ 
/*DLL_EXPORT*/ VOID RemoveResourcePath(const CHAR * sFilePath);

/** 删除所有资源文件搜索路径
*/ 
/*DLL_EXPORT*/ VOID ClearResourcePaths();

//-------------------------------------------------------------------------------------------
//***场景渲染相关的方法
//-------------------------------------------------------------------------------------------

// 渲染所有的场景
/*DLL_EXPORT*/ VOID RenderAll();

/**渲染指定的场景，可能包含多个渲染窗体
@param in sceneHandle VGS场景管理器指针
*/
/*DLL_EXPORT*/ VOID	RenderScene(DWORD sceneHandle);

/** 更新制定渲染窗口的后缓冲区
*/
/*DLL_EXPORT*/ BOOL UpdateBackBuffer(DWORD hRenderWnd);

/**添加Viewport
@param in windowHandle 渲染窗体句柄
@camHandle in 视口所属的相机
@param in level   视口的层次，层次越高的视口会覆盖在层次低的视口上方
@param in left	  视口的最左角位置，相对值[0-1] left = vp_left/window_width;
@param in top	  视口的最上角位置，相对值[0-1] top = vp_top/window_height;
@param in width	  视口的宽度，相对值[0-1] width = vp_width/window_width;
@param in  height 视口的高度，相对值[0-1] height = vp_height/window_height;
@return 所创建的视口指针，如果创建失败，返回0
*/
/*DLL_EXPORT*/ DWORD AddViewport(DWORD windowHandle, DWORD camHandle, INT level, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

/** 删除渲染窗口的视口
@param windowHandle in 渲染窗体指针
@param viewportHandle in 视口指针
*/
/*DLL_EXPORT*/ VOID DeleteViewport(DWORD windowHandle, DWORD vpHandle);

/** 设置视口使用的相机
@param viewportHandle in 视口指针
@param camHandle in 相机指针
*/
/*DLL_EXPORT*/ VOID SetViewportCamera(DWORD viewportHandle, DWORD camHandle);
/** 获取视口当前使用的相机
@param viewportHandle 视口指针
@return 相机指针
*/
/*DLL_EXPORT*/ DWORD GetViewportCamera(DWORD viewportHandle);

/**设置视口位置和大小 
@param ViewPort 视口指针
@param left,top,width,height取值均为[0,1],表示在RenderTarget中的位置
*/
/*DLL_EXPORT*/ VOID	SetViewportRect(DWORD ViewPort, float left, float top, float width, float height);

/** 设置视口使用相机的宽高比
*/
/*DLL_EXPORT*/ VOID UpdateViewportAspectRatio(DWORD viewportHandle);

/**获取视口大小 
@param ViewPort 视口指针
@param left,top,width,height取值均为[0,1],表示在RenderTarget中的位置
*/
/*DLL_EXPORT*/ VOID GetViewportRect(DWORD ViewPort, float &left, float &top, float &width, float &height);

/** 得到视口的真实宽度
@return : 如果视口不存在，返回-1
*/
/*DLL_EXPORT*/ INT GetViewportActualWidth(DWORD ViewPort);

/** 得到视口的真实高度
@return : 如果视口不存在，返回-1
*/
/*DLL_EXPORT*/ INT GetViewportActualHeight(DWORD ViewPort);

/**设置当前场景是否自动渲染
@param sceneHandle 场景指针
@param renderScene 是否应该被渲染，如果设为<b>不</b>自动渲染，则只有调用RenderTarget的Update方法时窗体才能被渲染。
*/
/*DLL_EXPORT*/ VOID SetSceneRenderStatus(DWORD sceneHandle, BOOL renderScene);
/**获取当前场景是否自动渲染
@param sceneHandle 场景指针
*/
/*DLL_EXPORT*/ BOOL	GetSceneRenderStatus(DWORD sceneHandle);

/**设置渲染窗体是否渲染
@param sceneHandle 场景管理器指针
@param RenderTargetHandle 渲染窗体指针
@param bAutoRender 是否自动渲染，如果设为<b>不</b>自动渲染，则只有调用RenderTarget的Update方法时窗体才能被渲染。
*/
/*DLL_EXPORT*/ VOID SetRenderTargetStatus(DWORD sceneHandle, DWORD RenderTargetHandle, BOOL bAutoRender);

/**获取渲染窗体是否渲染
@param sceneHandle 场景管理器指针
@param RenderTargetHandle 渲染窗体指针
*/
/*DLL_EXPORT*/ BOOL GetRenderTargetStatus(DWORD sceneHandle, DWORD RenderTargetHandle);

/**设置渲染窗体的填充模式, 2为线框，3为实体
@param sceneHandle 场景管理器指针
@param CameraHandle 视口指针
@param fillMode 渲染模式，1 : Only points are rendered， 2 : Wireframe models are rendered. 3: Solid polygons are rendered.
*/
// /*DLL_EXPORT*/ VOID SetSwapRenderWndFillMode(DWORD sceneHandle, DWORD CameraHandle, INT fillMode);
/*DLL_EXPORT*/ VOID SetViewportFillMode(DWORD sceneHandle, DWORD viewport, INT fillMode);

/**获得渲染窗体的填充模式, 1 : Only points are rendered， 2 : Wireframe models are rendered. 3: Solid polygons are rendered.
@param sceneHandle 场景管理器指针
@param CameraHandle 相机指针
*/
// /*DLL_EXPORT*/ INT GetSwapRenderWndFillMode(DWORD sceneHandle, DWORD CameraHandle);
/*DLL_EXPORT*/ INT GetViewportFillMode(DWORD sceneHandle, DWORD viewport);

//for debug
/*DLL_EXPORT*/ INT	FrameRenderComplete(DWORD sceneHandle);

/** 强制渲染当前帧
@param sceneHandle 场景指针
*/
/*DLL_EXPORT*/ VOID RenderCurrentFrame(DWORD sceneHandle);

/**设置视口是否渲染二维界面
@param sceneHandle in 场景管理器指针
@param vpHandle in : 视口指针
*/
/*DLL_EXPORT*/ VOID SetViewportRenderGUI(DWORD sceneHandle, DWORD vpHandle, BOOL renderGUI);

/**获取视口是否渲染二维界面
@param sceneHandle in ：场景管理器指针
@param vpHandle in : 视口指针
@renderGUI 是否渲染二维元素
*/
/*DLL_EXPORT*/ BOOL IsViewportRenderGUI(DWORD sceneHandle, DWORD vpHandle);

//设置抗拒齿的级别
/*DLL_EXPORT*/ VOID SetAntiAliasLevel(INT level);
//获得抗拒齿级别
/*DLL_EXPORT*/ INT	GetAntiAliasLevel();

/**设置场景的背景
@param sceneHandle 场景管理器指针
@param ViewportHandle 视口的指针，当前版本必须设置NULL，将设置场景中的所有视口
@param color 颜色
*/
/*DLL_EXPORT*/ VOID SetSceneBackColor(DWORD sceneHandle, DWORD ViewportHandle, DWORD color);
/**获得场景的背景
@param sceneHandle 场景管理器指针
@param ViewportHandle 视口的指针
*/
/*DLL_EXPORT*/ DWORD GetSceneBackColor(DWORD sceneHandle, DWORD ViewportHandle);

//设置场景的VGS窗体初始尺寸
/*DLL_EXPORT*/ VOID SetSceneRect(DWORD sceneHandle, RECT* pRect);
//获得场景的VGS窗体初始尺寸
/*DLL_EXPORT*/ VOID GetSceneRect(DWORD sceneHandle, RECT* pRect);

//计算三维坐标的屏幕坐标
/*DLL_EXPORT*/ VOID CalcScreenCoords(DWORD sceneHandle, DWORD viewportHandle, 
								 VECTOR3* pVec3, INT* X, INT* Y);

//缩放轴侧窗口
///*DLL_EXPORT*/ VOID ZoomOrthoView(DWORD sceneHandle, DWORD swapChainHandle, FLOAT zoomVal);

//当前窗体是否为轴侧窗口
// /*DLL_EXPORT*/ BOOL IsOrthoView(DWORD sceneHandle, DWORD swapChainHandle);

//将当前窗口中的三维场景设置到合适的大小
// /*DLL_EXPORT*/ VOID MaximizeSceneToView(DWORD sceneHandle, DWORD swapChainHandle, FLOAT fillRatio);

//平移轴侧窗口
/*DLL_EXPORT*/ VOID PanOrthoView(DWORD sceneHandle, DWORD swapChainHandle, INT X, INT Y, INT offsetX, INT offsetY);

//-------------------------------------------------------------------------------------------
///文件相关的方法
//-------------------------------------------------------------------------------------------
/** 导入S3D文件
@param sceneHandle		 : 场景管理器句柄
@param strS3DFileName	 : s3D文件的完整路径
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 成功返回S3D加载器的句柄，否则返回0
@remarks 通过返回的S3D加载器句柄，还可以获取跟本S3D相关的一些信息。切记，不用时要及时调用DeleteS3DLoader()方法删除加载器
*/
#if (!defined(_VGS_LIB) && !defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD ImportS3DFile(DWORD sceneHandle, const CHAR* strS3DFileName, HWND hWnd);
#endif


/** 删除S3D加载对象
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ VOID DeleteS3DLoader(DWORD sceneHandle, DWORD S3DLoaderHandle);
#endif


/** 指定的贴图文件是否被S3D使用
@param S3DLoaderHandle in  : S3D加载器指针，由ImportS3DFile()返回
@param ImageFileName   in  : 图片文件名称，不包括路径
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ BOOL IsTextureImageUsedbyS3D(DWORD S3DLoaderHandle, const CHAR* ImageFileName);
#endif
/** 得到S3D中使用的贴图的数量
@param loaderHandle in : S3D加载器指针，由ImportS3DFile()返回
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ INT GetS3DTextureImageCnt(DWORD S3DLoaderHandle);
#endif

/** 得到S3D中使用的第index个贴图文件名称，包含路径
@param loaderHandle in  : S3D加载器指针，由ImportS3DFile()返回
@param texIndex     in  : 索引
@param strTexName   out : 贴图文件名称
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ BOOL GetS3DTextureImageFile(DWORD S3DLoaderHandle, INT texIndex, CHAR* strTexName);
#endif

/** 保存场景到指定的V3D文件
@param in sceneHandle    ：VGS场景管理器指针
@param in strV3DFileName ：要保存的V3D文件名
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD SaveSceneAsV3D(DWORD sceneHandle, CHAR* strV3DFileName, HWND hWnd);
#endif

/**打开V3D文件, 自动识别不同版本的V3D
@param in sceneHandle VGS场景管理器指针
@param in strV3DFileName 要打开的V3D文件名
@param in OT 打开V3D的方法，例如替换方式还是合并方式，目前仅支持替换方式
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@param in&out pErrCode 如果失败，返回错误码，通过GetErrorStr可以得到原因描述
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD OpenV3DFile(DWORD sceneHandle, const CHAR* strV3DFileName, V3D_OPENTYPE OT, HWND hWnd);
#endif

/**保存当前场景为N3D格式文件
@param in sceneHandle ：要保存的场景
@param in strN3DFileName ： N3D文件名，包含目录
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD SaveSceneAsN3D(DWORD sceneHandle, const CHAR* strN3DFileName, HWND hWnd);
#endif

/** 从buffer中读取N3D场景信息
@param in sceneHandle    ：要加载数据的场景
@param in version        : 版本号
@param in pBuffer        ：N3D数据buffer
@param hWnd           in ：传入的窗体句柄，用于接收进度信息
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
#if(defined(_VGS_LIB)||defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD LoadSceneFromBuffer(DWORD sceneHandle, DWORD version, const BYTE* pBuffer, HWND hWnd);
#endif

/** 从buffer中读取图像信息
@param in sceneHandle    ：要加载图像的场景
@param in version        : 版本号
@param in pBuffer        ：N3D数据buffer
@param hWnd           in ：传入的窗体句柄，用于接收进度信息
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
#if(defined(_VGS_LIB)||defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD LoadImageDataFromBuffer(DWORD sceneHandle, DWORD version, const BYTE* pBuffer, HWND hWnd);
#endif

// 由一张图片的buffer创建纹理
/*DLL_EXPORT*/ DWORD CreateImageTextureFromBuffer(DWORD sceneHandle, const BYTE* pBuffer, DWORD len, int idx, HWND hWnd);


/** 打开N3D场景文件, 自动识别不同版本的N3D
@param in sceneHandle ：要保存的场景
@param in strN3DFileName ： N3D文件名，包含目录
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
// /*DLL_EXPORT*/ DWORD ImportN3DFile(DWORD sceneHandle, const CHAR* strN3DFileName);

/**保存当前场景为E3D格式文件
@param in sceneHandle    ：要保存的场景
@param in strE3DFileName ：E3D文件名，包含目录
@param in bLinkRes       : 所需的资源是以外部链接方式还是打包到文件内部。bLinkRes为true，资源将以外部链接的方式，不打包到E3D内部
@param in bPackData      : 是否压缩文件，TRUE：压缩，FALSE：不压缩
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD SaveSceneAsE3D(DWORD sceneHandle, const CHAR* strE3DFileName, BOOL bLinkRes/* = FALSE*/, BOOL bPackData/* = TRUE*/, HWND hWnd);
#endif

/** 读入N3D文件
@param strE3DFileName in : 要读入的文件名称
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
/*DLL_EXPORT*/ DWORD LoadE3DFile(DWORD sceneHandle, const CHAR* strE3DFileName, HWND hWnd);
#endif

//-------------------------------------------------------------------------------------------
///模型相关的方法
//-------------------------------------------------------------------------------------------

/**得到场景中模型的数量
@param in sceneHandle 场景管理器指针
@return 场景中模型的数量
*/
/*DLL_EXPORT*/ INT GetModelCount(DWORD sceneHandle);

/*删除场景中的模型
@param sceneHandle 场景管理器指针
@param modelHandle 模型(entity)指针
@return 成功删除返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL DeleteModelbyHandle(DWORD sceneHandle, DWORD modelHandle);


/** 删除场景中的模型
@param sceneHandle 场景管理器指针
@param strModelName 模型(entity)名称
@return 成功删除返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL DeleteModel(DWORD sceneHandle, const CHAR* strModelName);

//根据模型的名称得到模型的句柄
/*DLL_EXPORT*/ DWORD GetModelHandleByName(DWORD sceneHandle, const CHAR* strModelName);
//根据模型的编号得到模型的句柄
/*DLL_EXPORT*/ DWORD GetModelHandleByIndex(DWORD sceneHandle, INT index);

/**根据模型的句柄得到模型的名称
@param in sceneHandle 场景指针
@param in modelHandle 模型的句柄
@param in & out strModelName 模型名称
@return 得到名字返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetModelNameByHandle(DWORD sceneHandle, DWORD modelHandle, CHAR* strModelName);

/**根据模型的索引号得到模型的名称
@param in sceneHandle 场景指针
@param in index 模型的索引号
@param in & out strModelName 模型名称
@return 得到名字返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetModelNameByIndex(DWORD sceneHandle, INT index, CHAR* strModelName);


//设置模型的名称
/*DLL_EXPORT*/ VOID SetModelName(DWORD sceneHandle, DWORD modelHandle, CHAR* strModelName);

/** 移动模型的位置
@param in sceneHandle	: 场景句柄
@param in camHandle     : 相机句柄
@param in pVec          ：移动的相对向量
@param in coordEnum     ：移动的参考坐标系
			               0 - 在世界坐标系平移
						   1 - 在自身坐标系平移
*/
/*DLL_EXPORT*/ VOID MoveModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec, UINT flag);

/** 得到模型的位置,即包围盒中心的世界坐标
@param out pVec : 模型包围盒中心的世界坐标位置
*/
/*DLL_EXPORT*/ VOID GetModelPosition(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec);
//设置模型的位置
/*DLL_EXPORT*/ VOID SetModelPosition(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec);

/** 获取模型的包围盒
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param out vMinCorner : 包围盒坐标最小的角的世界坐标
@param out vMinCorner : 包围盒坐标最大的角的世界坐标
@return 包围盒有效，返回TRUE, 否则FALSE
*/ 
/*DLL_EXPORT*/ BOOL GetModelBoundsBox(DWORD sceneHandle, DWORD modelHandle, VECTOR3 &vMinCorner, VECTOR3 &vMaxCorner);

/** 获取模型的包围球
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param out vCenter    : 包围球中心点的世界坐标
@param out Radius     : 包围球半径
@return 包围盒有效，返回TRUE, 否则FALSE
*/ 
/*DLL_EXPORT*/ BOOL GetModelBoundsSphere(DWORD sceneHandle, DWORD modelHandle, VECTOR3 &vCenter, FLOAT &Radius);

/** 设置模型是否显示包围盒
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param in bvisible    : 是否显示包围盒
*/
/*DLL_EXPORT*/ VOID SetModelBoundsBoxVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL bVisible);

///** 设置模型是否显示包围盒
//@param in sceneHandle : 场景管理器指针
//@param in modelHandle : 模型指针
//@param in bvisible    : 是否显示包围盒
//@param in type        : 
//						VAT_REPLACE = 0,  // 将modelHandle的包围盒设为bvisible，同时所有其他模型的包围盒设置!bvisible
//						VAT_REVERSE,      // 将modelHandle的包围盒设为!bvisible，同时所有其他模型的包围盒设置bvisible
//						VAT_ADD           // 将modelHandle的包围盒设为bvisible，其他所有模型不改变包围盒状态
//*/
///*DLL_EXPORT*/ VOID SetModelBoundsBoxVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL bVisible, VGS_ADDTYPE type);

/** 旋转模型
@param in pVec : 在各个方向上旋转的角度增量,弧度
@param in coordEnum :
	//0 - 绕穿过自身中心，与世界坐标系平行的坐标系旋转
	//1 - 绕自身坐标系旋转模型
	//2 - 绕父坐标系旋转
*/
/*DLL_EXPORT*/ VOID RotateModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec, INT coordEnum);

//模型绕指定的轴旋转
///*DLL_EXPORT*/ VOID RotateModelByAxis(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pVec, FLOAT fAngle);

/** 得到模型的转角
@param out pRot ：模型当前相对于原始状态在三个方向上的转角，单位是弧度
*/
/*DLL_EXPORT*/ BOOL GetModelRotation(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pRot);

/** 设置模型的转角
@param out pRot ：模型当前相对于原始状态在三个方向上的转角，单位是弧度
*/
/*DLL_EXPORT*/ VOID SetModelRotation(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pRot);

/**-----------------------------------------------------------------------------
// 方法名: ScaleModelByPoint()
// 参  数: DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale
// 用  途: 以固定点为中心缩放模型
//-----------------------------------------------------------------------------*/
/*DLL_EXPORT*/ VOID ScaleModelByPoint(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale);

/** 以自身中心为中心，在当前基础上缩放模型
@param in pScale : 各个方向上的缩放值
@param in flag   : 参考坐标系，0：世界坐标系，1：自身坐标系
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型将会在现在的基础上，在X方向不缩放，y方向放大一倍，z方向缩小一倍
*/
/*DLL_EXPORT*/ VOID ScaleModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale, int flag = 0/*世界坐标系*/);

/** 以自身中心为中心，在模型原始尺寸基础上缩放模型
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型相对于原始尺寸，将在X方向不缩放，y方向放大一倍，z方向缩小一倍
*/
/*DLL_EXPORT*/ VOID SetModelScale(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale);

/** 得到模型的缩放
@param out pScale : 模型绝对缩放值，相对于原始大小，不是相对于上一次的。
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型现在相对于原始尺寸，在X方向没有缩放，y方向放大了一倍，z方向缩小了一倍
*/
/*DLL_EXPORT*/ BOOL GetModelScale(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale);

//以固定点为中心缩放模型
// /*DLL_EXPORT*/ VOID ScaleModelByPoint(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale);

//Add By Baojiahui 修正移动相机以及移动物体时位置不准确的方法----------------------------------------------------------------------------------------------------------
/** 得到垂直于屏幕坐标下对应的3d场景中某个平面上的交点坐标
@param in sceneHandle      :场景管理器指针
@param in vpHandle         :视口指针
@param in ScreenX, ScreenY :相对于RenderWindow区域的屏幕坐标
@param in planeNormal*      :特定平面的法线向量指针
@param in planePoint*       :过特定平面的点坐标指针
@param out vec3WorldPoint*  :返回的平面交点坐标指针
*/
/*DLL_EXPORT*/ VOID GetWorldPointUnderLoc(DWORD sceneHandle,DWORD vpHandle,UINT ScreenX, UINT ScreenY,VECTOR3* planeNormal,VECTOR3* planePoint,VECTOR3* vec3WorldPoint);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

/** 得到屏幕坐标下对应的3d场景中的模型，如有多个模型，得到最近的一个。隐藏或被冻结的model不会被选中
@param in sceneHandle      : 场景管理器指针
@param in vpHandle         : 视口指针
@param in ScreenX, ScreenY : 相对于RenderWindow区域的屏幕坐标
@param in modeltype        : 0 : 仅选中模型， 1：选中所有，包括灯光和相机
@return : 模型指针
*/
/*DLL_EXPORT*/ DWORD GetModelUnderLoc(DWORD sceneHandle, DWORD vpHandle, UINT ScreenX, UINT ScreenY, int modeltype = 0);

  //-----------------------------------------------------------------------------
// 方法名: GetModelUnderLoc()
// 参  数: VECTOR* rayVec, FLOAT* dis
// 用  途: 得到矢量方向相交的最近的模型，需要检测模型的collision属性要设置为TRUE
//-----------------------------------------------------------------------------
/*DLL_EXPORT*/ DWORD GetModelUnderRay(DWORD sceneHandle, DWORD swapChainHandle, 
					   VECTOR3* rayPoint, VECTOR3* rayVec,
					   FLOAT* dis, VECTOR3* pVNormal );
/** 获取模型属性信息
*/
/*DLL_EXPORT*/ BOOL GetModelInfo(DWORD sceneHandle, DWORD modelHandle, VGS_ModelInfo &Info);

/** 设置模型属性信息
*/
/*DLL_EXPORT*/ VOID SetModelInfo(DWORD sceneHandle, DWORD modelHandle, const VGS_ModelInfo &Info);

/** 设置模型的鼠标事件函数名称
@param scenHandle   in : 场景指针
@param modelHandle  in : 模型指针
@param type         in  : 事件类型
@param sEventName   in : 函数名称
*/
/*DLL_EXPORT*/ VOID SetModelMouseEvent(DWORD sceneHandle, DWORD modelHandle, VGS_MOUSEEVENT_TYPE type, const CHAR* sEventName);

/** 获取模型的鼠标事件函数名称
@param scenHandle   in  : 场景指针
@param modelHandle  in  : 模型指针
@param type         in  : 事件类型
@param sEventName   out : 函数名称
@return 如果模型已经制定了对应的事件函数，返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetModelMouseEvent(DWORD sceneHandle, DWORD modelHandle, VGS_MOUSEEVENT_TYPE type, CHAR* sEventName);

/** 把相机设置为billboard相机
*/
/*DLL_EXPORT*/ VOID SetBillboardCamera(DWORD sceneHandle, DWORD camHanlde);

/** 获取billboard相机
*/
/*DLL_EXPORT*/ DWORD GetBillboardCamera(DWORD sceneHandle); 

/** 设置模型正对Billboard状态
@param axis in : 设置模型的哪个坐标轴（自身）指向相机
*/
/*DLL_EXPORT*/ VOID SetModelBillboardStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status, VGS_AXIS_TYPE axis = VGS_LAXIS_NEGATIVEZ);

/** 判断模型是否为Billboard模型
@param axis in : 模型的哪个坐标轴（自身）指向相机
*/
/*DLL_EXPORT*/ BOOL GetModelBillboardStatus(DWORD sceneHandle, DWORD modelHandle, VGS_AXIS_TYPE &axis);

//-----------------------------------------------------------------------------
// 方法名: SetBillboardUp()
// 参  数: DWORD modelHandle, VECTOR3* pModelRay, VECTOR3* pAt, VECTOR3* pUp
//modelHandle ->模型的指针
//pUp ->模型的上方是什么方向，譬如Tree，要求Z方向正对目标点，但up方向始终为(0,1,0), 否则就倒了
// 用  途: 设置模型正对Billboard
//-----------------------------------------------------------------------------
/*DLL_EXPORT*/ VOID SetBillboardUp(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pUp);

//设置模型正对Billboard的Up向量
///*DLL_EXPORT*/ VOID SetBillboardUp(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pUp);

////设置场景碰撞的有效状态
///*DLL_EXPORT*/ VOID EnableCollision(DWORD sceneHandle, BOOL status);

////设置碰撞发生的有效距离, 默认为1.0
///*DLL_EXPORT*/ VOID SetColDistance(DWORD sceneHandle, FLOAT distance);

/** 设置模型为水平碰撞模型
*/
/*DLL_EXPORT*/ VOID SetModelCollisionStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status);

/** 判断模型是否为水平碰撞模型
*/
/*DLL_EXPORT*/ BOOL	GetModelCollisionStatus(DWORD sceneHandle, DWORD modelHandle);

/** 设置模型为地面模型
*/
/*DLL_EXPORT*/ VOID SetModelAsGround(DWORD sceneHandle, DWORD modelHandle, BOOL status);

/** 判断模型是否为地面模型
*/
/*DLL_EXPORT*/ BOOL GetModelAsGround(DWORD sceneHandle, DWORD modelHandle);

////移动模型Diffuse贴图的UV
///*DLL_EXPORT*/ VOID MoveModelUV(DWORD sceneHandle, DWORD modelHandle, FLOAT x, FLOAT y);
//
////设置模型动态UV的状态
///*DLL_EXPORT*/ VOID SetModelDynamicUVStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status);
//
////获得模型动态UV的状态
///*DLL_EXPORT*/ BOOL GetModelDynamicUVStatus(DWORD sceneHandle, DWORD modelHandle);
//
////设置模型的动态UV
///*DLL_EXPORT*/ VOID SetModelDynamicUV(DWORD sceneHandle, DWORD modelHandle, FLOAT u, FLOAT v);
////获得模型的动态UV
///*DLL_EXPORT*/ VOID GetModelDynamicUV(DWORD sceneHandle, DWORD modelHandle, FLOAT* u, FLOAT* v);


/** 设置模型是否可见
*/
/*DLL_EXPORT*/ VOID SetModelVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL status);

/** 获取模型是否可见
*/
/*DLL_EXPORT*/ BOOL GetModelVisibility(DWORD sceneHandle, DWORD modelHandle);

/** 设置模型在播放状态下是否可以点击
*/
/*DLL_EXPORT*/ VOID SetModelClickable(DWORD sceneHandle, DWORD modelHandle, BOOL status);

/** 获取模型在播放状态下是否可以点击
*/
/*DLL_EXPORT*/ BOOL GetModelClickabel(DWORD sceneHandle, DWORD modelHandle);

//设置模型为SkyBox模型
/*DLL_EXPORT*/ VOID SetSkyBox(DWORD sceneHandle, DWORD modelHandle, BOOL status);

//判断模型是否为SkyBox
/*DLL_EXPORT*/ BOOL IsSkyBox(DWORD sceneHandle, DWORD modelHandle);

/** 设置模型的颜色
*/
/*DLL_EXPORT*/ VOID SetModelColor(DWORD sceneHandle, DWORD modelHandle, BYTE red, BYTE green, BYTE blue, BYTE alpha);
///*DLL_EXPORT*/ VOID GetModelColor(DWORD sceneHandle, DWORD modelHandle, BYTE &red, BYTE &green, BYTE &blue, BYTE *alpha);

/** 设置模型透明图
*/
/*DLL_EXPORT*/ VOID SetModelOpacity(DWORD sceneHandle, DWORD modelHandle, INT opacity);
///*DLL_EXPORT*/ INT GetModelOpacity(DWORD sceneHandle, DWORD modelHandle);

/** 模型是否包含关键帧动画
*/
/*DLL_EXPORT*/ BOOL IsModelHasKeyframeAnimation(DWORD sceneHandle, DWORD modelHandle);

/** 模型是否包含骨骼动画
*/
/*DLL_EXPORT*/ BOOL IsModelHasSkeletonAnimation(DWORD sceneHandle, DWORD modelHandle);

////设置模型的颜色
///*DLL_EXPORT*/ VOID SetModelColor(DWORD sceneHandle, DWORD modelHandle, INT red, INT green, INT blue, INT alpha);
//
////得到模型的透明度
///*DLL_EXPORT*/ INT GetModelOpacity(DWORD sceneHandle, DWORD modelHandle);
//
////设置模型的透明度
///*DLL_EXPORT*/ VOID SetModelOpacity(DWORD sceneHandle, DWORD modelHandle, INT opacity);

// 子模型方法
/** 获取模型子模型使用的材质名称，每个子模型只能有一个材质
@param in sceneHandle ： 场景管理器
@param in modelHandle ：模型名指针
@return 子模型数目
**/
/*DLL_EXPORT*/ INT GetSubModelCount(DWORD sceneHandle, DWORD modelHandle);

// 获取模型子模型使用的材质名称，每个子模型只能有一个材质
/*DLL_EXPORT*/ BOOL GetSubModelMaterialName(DWORD modelHandle, int index, CHAR *sName);

// 获取模型子模型使用的材质指针，每个子模型只能有一个材质
/*DLL_EXPORT*/ DWORD GetSubModelMaterialHandle(DWORD modelHandle, int index);

/** 为模型中的第index个子模型指定材质
@param in modelHandle : 场景管理器指针
@param in index : 子模型索引
@param in pMtrName : 材质名称
*/
/*DLL_EXPORT*/ VOID SetSubModelMaterial(DWORD sceneHandle, DWORD modelHandle, int index, const CHAR* pMtrName);

/**为模型制定材质，如果模型包含多个子模型，那么所有子模型都会使用这个材质
@param in modelHandle : 场景管理器指针
@param in pMtrName : 材质名称
*/
/*DLL_EXPORT*/ VOID SetModelMaterial(DWORD sceneHandle, DWORD modelHandle, const CHAR* pMtrName);

//-------------------------------------------------------------------------------------------
///材质相关的方法
//-------------------------------------------------------------------------------------------
/** 创建材质
@param strMtrlName in : 要创建的材质名称
*/
/*DLL_EXPORT*/ DWORD CreateMaterial(DWORD sceneHandle, const CHAR* strMtrlName);

/** 删除材质
@param mtrlHandle in : 要删除的材质指针
*/
/*DLL_EXPORT*/ VOID	DeleteMaterialByHandle(DWORD sceneHandle, DWORD mtrlHandle);

/** 删除材质
@param pName in : 要删除的材质名称
*/
/*DLL_EXPORT*/ VOID DeleteMaterialByName(DWORD sceneHandle, const CHAR* pName);

/** 得到场景中材质的数量
*/
/*DLL_EXPORT*/ INT GetMaterialCount(DWORD sceneHandle);

/** 根据材质的名称得到材质的句柄
*/
/*DLL_EXPORT*/ DWORD GetMaterialHandleByName(DWORD sceneHandle, const CHAR* strMtrlName);

/** 根据材质的编号得到材质的句柄
*/
/*DLL_EXPORT*/ DWORD GetMaterialHandleByIndex(DWORD sceneHandle, INT index);

/** 根据材质的句柄得到材质的名称
*/
/*DLL_EXPORT*/ VOID GetMaterialNameByHandle(DWORD sceneHandle, DWORD mtrlHandle, CHAR* strMtrlName);

/** 设置材质的动态UV速度
*/
// /*DLL_EXPORT*/ VOID SetMaterialUVSpeed(DWORD sceneHandle, DWORD mtrlHandle, float USpeed, float VSpeed);
/*DLL_EXPORT*/ VOID SetTextureUVSpeed(DWORD sceneHandle, DWORD textureHandle, float USpeed, float VSpeed);

/** 获取材质的动态UV速度
*/
// /*DLL_EXPORT*/ VOID GetMaterialUVSpeed(DWORD sceneHandle, DWORD mtrlHandle, float &USpeed, float &VSpeed);
/*DLL_EXPORT*/ VOID GetTextureUVSpeed(DWORD sceneHandle, DWORD textureHandle, float &USpeed, float &VSpeed);


///////////// texture ///////////////////////////////////

// 从Image buffer更新使用该Image的所有texture
/*DLL_EXPORT*/ BOOL FillTexturesFromBuffer(UINT len, CHAR* pBuffer, INT imgIndex);

/** 为材质增加一层基本贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片名称，包括完成路径
@param pTextureName  in : texture层的名称，传入NULL表示自动命名
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
/*DLL_EXPORT*/ DWORD AddMaterialBasalTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureFile, 
										 const CHAR* pTextureName = NULL, int UVIndex = 0);

/** 为材质增加一层透明贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片名称，包括完整路径
@param pTextureName  in : texture层的名称，传入NULL表示自动命名
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
/*DLL_EXPORT*/ DWORD AddOpacityTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureFile, const CHAR* pTextureName = NULL, int UVIndex = 0);

/** 为材质增加一层反射贴图
@param hScene   in : VGS场景指针
@param szMtrl in : 材质名称
@param szTextureFile  in : texture使用的图片名称，包括完整路径
@param szTexture  in : texture层的名称，传入NULL表示自动命名
@param nUVindex	 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@param vrt	 in : 反射类型
@return 新创建的texture句柄
*/
/*DLL_EXPORT*/ DWORD AddReflectTexture(DWORD hScene, const char* szMtrl, const char* szTextureFile, const char* szTexture = NULL, int nUVindex = 0, VGS_REFLECT_TYPE vrt = VRT_CURVED);
/** 设置反射贴图的类型--added by Linxq
@param hScene   in : VGS场景指针
@param hTexture in : 贴图句柄
@param vrt	 in : 反射类型
@return:设置是否成功
*/
/*DLL_EXPORT*/ BOOL SetReflectTextureType(DWORD hScene, DWORD hTexture, VGS_REFLECT_TYPE vrt);
/** 获取反射贴图的类型--added by Linxq
@param hScene   in : VGS场景指针
@param hTexture in : 贴图句柄
@return:反射类型
*/
/*DLL_EXPORT*/ VGS_REFLECT_TYPE GetReflectTextureType(DWORD hScene, DWORD hTexture);

/** 获取立方贴图的子类型--added by linxq --2009-6-3
@param hScene       in : VGS场景指针
@param hTexture in : 贴图层句柄
@return:立方贴图类型.
*/
/*DLL_EXPORT*/ VGS_CubemapType GetCubeTextureType(DWORD hScene, DWORD hTexture);

/** 为材质增加一层静态立方贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片序列，包括完成路径。cubic必须包含6张图片，图片需要按照前后左右上下的顺序提供
@param pMaterailName in : 材质名称，如果为空，将自动命名
@return 新创建的texture句柄
*/
/*DLL_EXPORT*/ DWORD AddMaterialCubicStaticTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD* pTextureFile, 
    									       const CHAR* pTextureName = NULL);

/** 为材质增加一层动态立方贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称，如果为空，将自动命名
@param pTextureName  in : 创建的texture名称
@return 新创建的texture句柄
*/
/*DLL_EXPORT*/ DWORD AddMaterialCubicDynamicTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureName = NULL);

/** 为动态立方贴图添加一个反射模型
@param sceneHandle       in : VGS场景指针
@param DynamicCubeHandle in : 动态cubemap指针
@param modelHandle       in : 被反射模型句柄
*/
// 移除动态cubemap的反射模型
/*DLL_EXPORT*/ VOID AddDynamicReflectedModel(DWORD sceneHandle, DWORD DynamicCubeHandle, DWORD modelHandle);

/** 为动态立方贴图移除一个反射模型
@param sceneHandle       in : VGS场景指针
@param DynamicCubeHandle in : 动态cubemap指针
@param modelHandle       in : 被移除的模型句柄
*/
// 移除动态cubemap的反射模型
/*DLL_EXPORT*/ VOID RemoveDynamicReflectedModel(DWORD sceneHandle, DWORD DynamicCubeHandle, DWORD modelHandle);
/** 移除动态cubemap的全部反射模型.--added by linxq --2009-6-3
@param hScene       in : VGS场景指针
@param hDynamicCubeMap in : 动态cubemap指针
@return : 映射的模型数量.
*/
/*DLL_EXPORT*/ VOID RemoveDynamicReflectedAllModel(DWORD hScene, DWORD hDynamicCubeMap);
/** 获取动态立方贴图的模型数量.--added by linxq --2009-6-3
@param hScene       in : VGS场景指针
@param hDynamicCubeMap in : 动态cubemap指针
@return : 映射的模型数量.
*/
/*DLL_EXPORT*/ int GetDynamicReflectedModelCount(DWORD hScene, DWORD hDynamicCubeMap);
/** 按索引获取动态立方贴图的模型句柄.--added by linxq --2009-6-3
@param hScene       in : VGS场景指针
@param hDynamicCubeMap in : 动态cubemap指针
@param nIdx in: 有效的索引号.
@return : 模型句柄.
*/
/*DLL_EXPORT*/ DWORD GetDynamicReflectedModelHandle(DWORD hScene, DWORD hDynamicCubeMap, int nIdx);


/** 为材质增加一层bump贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片，包括完整路径。如果不指定图片，传入NULL或一个空字符串。
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
@remark:缺省创建VBT_ENVIRONMENT类型的bump贴图
*/
/*DLL_EXPORT*/ DWORD AddMaterialBumpTexture(DWORD sceneHandle, const char* pMaterailName, const CHAR* pTextureFile, VGS_BumpType type, int UVIndex = 0);


/*DLL_EXPORT*/ DWORD AddMaterialBumpwaveTexture(DWORD sceneHandle, const char* pMaterailName, INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);

/*DLL_EXPORT*/ DWORD AddMaterialBumpEnviromentTexture(DWORD sceneHandle, const char* pMaterailName, const char* pTextureFile, float noise1, float noise2);

/*DLL_EXPORT*/ DWORD AddMaterialBumpNormalTexture(DWORD sceneHandle, const char* pMaterailName, const char* pTextureFile, DWORD lightHandle);
/** 获取bump贴图的类型.--added by linxq --2009-6-8
@param hScene in : VGS场景指针
@param hBump in: bump贴图句柄.
@return : bump贴图的类型
*/
/*DLL_EXPORT*/ VGS_BumpType GetBumpTextureType(DWORD hScene, DWORD hBumpTexture);

/** 设置bump water的参数
@param sceneHandle      in : VGS场景指针
@param pTexture         in : texture句柄
@param dwWidth/dwHeight in : 贴图的尺寸
@param xSize/ySize      in :
@param radius:
@param speed:
@param centerX/centerY
*/
/*DLL_EXPORT*/ VOID SetBumpwaveTextureParam(DWORD sceneHandle, DWORD pTexture, 
							  INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);

/** 为材质增加一层动态贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片序列，包括完成路径
@param Duration      in : 播放所有图片的时间长度，单位秒。例如，如果一共有5张图片，Duration = 1，那么平均200毫秒播放一张图片
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
/*DLL_EXPORT*/ DWORD AddMaterialDynamicTexture(DWORD sceneHandle, const CHAR* pMaterailName, int FrameCount, DWORD* pTextureFile, int Duration, int UVIndex = 0);

/**@param pMaterailName     in : 材质名称
@param sTextureLayerName in : 图层名称
@param width/height      in : 生成贴图的尺寸
*/
/*DLL_EXPORT*/ DWORD AddMaterialRTReflectionTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* sTextureLayerName, INT width = 256, INT height = 256);

/** 设置贴图层为实时反射
*/
/*DLL_EXPORT*/ VOID setTextureAsRTReflection(DWORD sceneHandle, DWORD textureHandle);

/** 设置实时反射模型以获取反射平面
@param textureHandle    in : 图层句柄
@param hReflectionModel in : 指定的模型指针，通过该模型获取反射平面
*/
/*DLL_EXPORT*/ VOID setRTReflectionModel(DWORD sceneHandle, DWORD textureHandle, DWORD hReflectionModel);

/** 获取实时反射的模型
*/
/*DLL_EXPORT*/ DWORD getRTReflectionModel(DWORD sceneHandle, DWORD textureHandle);

/** 获取被实时反射的模型数目
*/
/*DLL_EXPORT*/ DWORD getRTReflectedModelCount(DWORD sceneHandle, DWORD textureHandle);

/** 根据索引获取被反射的模型
*/
/*DLL_EXPORT*/ DWORD getRTReflectedModelByIndex(DWORD sceneHandle, DWORD textureHandle, INT index);

/** 设置被实时反射的模型列表 
@param textureHandle     in : 图层句柄
@param count             in : 被反射模型的个数
@param phReflectedModels in : 被反射的模型指针列表
*/
/*DLL_EXPORT*/ VOID setRTReflectedModel(DWORD sceneHandle, DWORD textureHandle, DWORD count, DWORD* phReflectedModels);

/** 获取反射贴图的尺寸
*/
/*DLL_EXPORT*/ BOOL getRTReflectionDimension(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height);

/** 设置反射贴图的尺寸
*/
/*DLL_EXPORT*/ VOID setRTReflectionDimension(DWORD sceneHandle, DWORD textureHandle, INT width, INT height);

/** 为制定的材质增加一层实时折射层
@param sceneHandle       in : 场景指针
@param pMaterailName     in : 材质名称
@param sTextureLayerName in : 图层名称
@param width/height      in : 生成贴图的尺寸
*/
/*DLL_EXPORT*/ DWORD AddMaterialRTRefractionTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* sTextureLayerName, INT width = 256, INT height = 256);

/** 设置图层为实时折射
*/
/*DLL_EXPORT*/ VOID setTextureAsRTRefraction(DWORD sceneHandle, DWORD textureHandle);

/** 设置实时折射模型以获取折射平面
@param textureHandle    in : 图层句柄
@param hReflectionModel in : 指定的模型指针，通过该模型获取折射平面
*/
/*DLL_EXPORT*/ VOID setRTRefractionModel(DWORD sceneHandle, DWORD textureHandle, DWORD hRefractionModel);

/** 获取折射模型
*/
/*DLL_EXPORT*/ DWORD getRTRefractionModel(DWORD sceneHandle, DWORD textureHandle);

/** 获取被折射的模型数目
*/
/*DLL_EXPORT*/ DWORD getRTRefractedModelCount(DWORD sceneHandle, DWORD textureHandle);

/** 获取被折射的模型
*/
/*DLL_EXPORT*/ DWORD getRTRefractedModelByIndex(DWORD sceneHandle, DWORD textureHandle, INT index);

/** 设置被实时折射的模型列表 
@param textureHandle     in : 图层句柄
@param count             in : 被折射模型的个数，为0且phReflectedModels为空时将对已设置的模型列表进行清空.
@param phReflectedModels in : 被折射的模型指针列表
*/
/*DLL_EXPORT*/ VOID setRTRefractedModel(DWORD sceneHandle, DWORD textureHandle, DWORD count, DWORD* phRefractedModels);

/** 获取折射贴图的尺寸
*/
/*DLL_EXPORT*/ BOOL getRTRefractionDimension(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height);

/** 设置折射贴图的尺寸
*/
/*DLL_EXPORT*/ VOID setRTRefractionDimension(DWORD sceneHandle, DWORD textureHandle, INT width, INT height);

// /*DLL_EXPORT*/ VOID SetTextureAsProjective(DWORD sceneHandle, DWORD textureHandle, BOOL enabled, DWORD cameraHandle);

/** 设置贴图为bump
@param type : 三种bump之一
*/
/*DLL_EXPORT*/ VOID SetTextureAsBump(DWORD sceneHandle, DWORD textureHandle, VGS_BumpType type);

/** 设置bump贴图的参数
@param type  in : bump的类型
@param param in : 参数
*/
/*DLL_EXPORT*/ VOID SetBumpTextureParam(DWORD sceneHandle, DWORD textureHandle, const VGS_BumpParam &param);

/** 获取bump贴图的参数
@param type  out : bump的类型
@param param out : 参数
*/
/*DLL_EXPORT*/ BOOL GetBumpTextureParam(DWORD sceneHandle, DWORD textureHandle, VGS_BumpParam &param);

/** 判断一层texture是否被用作bump
@param type out : 如果是bump，type返回bump的类型
@return 如果是bump，返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetTextureAsBump(DWORD sceneHandle, DWORD textureHandle, VGS_BumpType &type);

/** 获取材质的texture数目
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@return texture的数目 
*/
/*DLL_EXPORT*/ DWORD GetMaterialTextureCount(DWORD sceneHandle, const CHAR* pMaterailName);

/** 获取材质的第textureIndex个texture句柄
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureIndex  in : texture索引
@return texture的句柄
*/
/*DLL_EXPORT*/ DWORD GetMaterialTextureHandle(DWORD sceneHandle, const CHAR* pMaterailName, int textureIndex);

/** 清空材质的一个texture内容, 清空后texture的指针依然有效
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle in : texture句柄
*/
/*DLL_EXPORT*/ VOID ClearMaterialTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle);

/** 删除材质的一层texture，删除后，该层texture之后的所有texture索引将会改变, <b>使用时一定要注意</b>。
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle in : texture句柄
*/
/*DLL_EXPORT*/ VOID RemoveMaterialTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle);

/** 设置textureIndex的类型。<b>注意：如果texture的类型改变，texture的内容将会被清空</b>。
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle in : texture句柄
@param type          in : texture 的类型
*/
///*DLL_EXPORT*/ VOID SetTextureType(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle, VGS_TextureType type);

/** 获取textureIndex的类型
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle  in : texture句柄
@return texture的类型
*/
/*DLL_EXPORT*/ VGS_TextureType GetTextureType(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle);

/** 获取textureIndex使用的UV坐标编号
@param sceneHandle   in : VGS场景指针
@param textureHandle  in : texture句柄
@return texture的UV坐标编号
*/
/*DLL_EXPORT*/ INT  GetTextureUVIndex(DWORD sceneHandle, DWORD textureHandle);

/** 设置textureIndex使用的UV坐标编号
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param UVIndex		 in : texture使用的UV坐标编号
*/
/*DLL_EXPORT*/ VOID SetTextureUVIndex(DWORD sceneHandle, DWORD textureHandle, INT UVIndex);

/** 设置基本贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件，包含完整路径
*/
// /*DLL_EXPORT*/ VOID SetTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile);
/*DLL_EXPORT*/ VOID SetBasalTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile);
/*DLL_EXPORT*/ VOID SetTransparentTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile);

/** 设置cubic贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件序列，包含完整路径。必须是6张图片，而且按照前后左右上下的顺序提供
*/
/*DLL_EXPORT*/ VOID SetCubicTextureImages(DWORD sceneHandle, DWORD textureHandle, DWORD* pTextureFile);

/** 为动态贴图重新设置所有使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param FrameCount    in : texture使用的图片数目
@param pTextureFile  in : texture使用的图片列表
*/
/*DLL_EXPORT*/ VOID SetDynamicTextureImages(DWORD sceneHandle, DWORD textureHandle, int FrameCount, DWORD* pTextureFile);

/** 设置Bump贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件，包含完整路径. 如果不需要图片，设置为NULL或空字符串。
*/
/*DLL_EXPORT*/ VOID SetBumpTextureImage(DWORD sceneHandle, DWORD textureHandle, const char* pTextureFile);

/** 设置动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@param pTextureFile	 in : texture使用的图像文件， 包含完整路径
*/
/*DLL_EXPORT*/ VOID SetDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT index, const CHAR* pTextureFile);

/** 添加一张动态贴图图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : 要添加的texture图像文件， 包含完整路径
*/
/*DLL_EXPORT*/ VOID AddDynamicTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile);

/** 删除动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@return 删除成功返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL DeleteDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT FrameIdx);

/** 设置动态贴图的播放周期长度，单位秒。 注意：如果图片序列为空，调用该方法是无效的。
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param Duration      in : 动态贴图序列播放一个周期所需的时间长度,单位秒
*/
/*DLL_EXPORT*/ VOID SetDynamicTextureDuration(DWORD sceneHandle, DWORD textureHandle, INT Duration);


/** 获取基本贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到贴图，返回TRUE,否则FALSE
*/
// /*DLL_EXPORT*/ BOOL GetTextureImage(DWORD sceneHandle, DWORD textureHandle, CHAR* pTextureFile);

/** 获取基本贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到贴图，返回TRUE,否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetBasalTextureImage(DWORD sceneHandle, DWORD textureHandle, char* pTextureFile);

/** 获取bump贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
*/
/*DLL_EXPORT*/ BOOL GetBumpTextureImage(DWORD sceneHandle, DWORD textureHandle, char* pTextureFile);

/** 获取动态贴图序列播放时间长度
@param sceneHandle	   in  : 场景管理器
@param textureHandle   in  : texture指针  
@return 动态贴图序列播放时间长度
*/
/*DLL_EXPORT*/ INT GetDynamicDuration(DWORD sceneHandle, DWORD textureHandle);

/** 获取立方贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param faceType      in : 指定一个面
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到贴图，返回TRUE,否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetCubicTextureImages(DWORD sceneHandle, DWORD textureHandle, VGS_CUBIC_FACE faceType, CHAR* pTextureFile);

/** 获取动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 out : texture使用的图像文件序列， 不 包含路径
*/
/*DLL_EXPORT*/ INT GetDynamicTextureImageCount(DWORD sceneHandle, DWORD textureHandle);

/** 获取动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到图片，返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT index, CHAR* pTextureFile);

/** 设置texture的混色信息
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param BlenderInfo	 in : texture的混色参数
*/
/*DLL_EXPORT*/ VOID SetTextureBlenderInfo(DWORD sceneHandle, DWORD textureHandle, VGS_BlenderType type, const VGS_BlenderInfo &BlenderInfo);

/** 获取texture的混色信息
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param BlenderInfo	 out : texture的混色参数
*/
/*DLL_EXPORT*/ VOID GetTextureBlenderInfo(DWORD sceneHandle, DWORD textureHandle, VGS_BlenderType type, VGS_BlenderInfo &BlenderInfo);

/** 获取贴图的Mipmaps数目
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture指针
@return 贴图的Mipmaps数目
*/
/*DLL_EXPORT*/ INT GetTextureMipmaps(DWORD sceneHandle, DWORD textureHandle);

/** 设置贴图的Mipmaps数目
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture指针
@param Mipmaps       in : 贴图的Mipmaps数目
*/
/*DLL_EXPORT*/ VOID SetTextureMipmaps(DWORD sceneHandle, DWORD textureHandle, INT Mipmaps);

/** 设置纹理的大小
@param sceneHandle  in  : 场景管理器
@param mtrlHandle   in  : 材质指针  
@param mapChanel    in  : 贴图通道 
@param width/height out : 图片的大小
*/
///*DLL_EXPORT*/ VIOD SetTextureSize(DWORD sceneHandle, DWORD textureHandle, INT width, INT height);

/** 获取纹理的大小
@param sceneHandle  in  : 场景管理器
@param mtrlHandle   in  : 材质指针  
@param mapChanel    in  : 贴图通道 
@param width/height out : 图片的大小
@return TRUE：返回的width/heigh有效，FALSE:返回的width/heigh无效
*/
/*DLL_EXPORT*/ BOOL GetTextureSize(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height, INT iFrmIndex = 0);
//---------Flash Texture Start----linxq 2009-12-15
/** 给指定的材质添加一个FlashTexture.
@param hScene in:场景.
@param pMtrlName in:材质名称，不可为空.
@param pTxtrName in:要创建的贴图名称，不可空.
@param pFlashFile in: Flash文件的路径名称.
@param flashWidth in: Flash的宽度.
@param flashHeight in: Flash高度.
@param nWidth in:要创建的贴图的宽度.
@param nHeight in:要创建的贴图的高度.
@return:返回贴图.
*/
/*DLL_EXPORT*/ DWORD AddMaterialFlashTexture(DWORD hScene, const char* pMtrlName, const char* pTxtrName, const char* pFlashPath,
										 int flashWidth, int flashHeight, int nWidth, int nHeight); 
/** 设置给定名称的Flash贴图的播放状态.
@param hScene in:场景.
@param textureHandle in:贴图.
@param nStatur in:播放状态——0:暂停,1:播放,2:停止.
*/
/*DLL_EXPORT*/ VOID SetFlashTexturePlayStatus(DWORD hScene, DWORD textureHandle, int nStatus); 
/** 设置Flash贴图移动指定帧.
@param hScene in:场景.
@param textureHandle in:贴图.
@param nFrameIndex in: 要移动的Flash索引.
*/
/*DLL_EXPORT*/ VOID SetFlashTextureGoToFrame(DWORD hScene, DWORD textureHandle, int nFrameIndex); 

//---------Flash Texture end----

///** 获取贴图图像的大小
//@param sceneHandle  in  : 场景管理器
//@param mtrlHandle   in  : 材质指针  
//@param mapChanel    in  : 贴图通道 
//@param width/height out : 图片的大小
//@return TRUE：返回的width/heigh有效，FALSE:返回的width/heigh无效
//*/
///*DLL_EXPORT*/ BOOL GetTextureSize(DWORD sceneHandle, DWORD mtrlHandle, INT mapChanel, INT &width, INT &height);

//设置材质的颜色
/*DLL_EXPORT*/ VOID SetMaterialColor(DWORD sceneHandle, DWORD mtrlHandle, INT red, INT green, INT blue, INT alpha);

//获得材质信息
/*DLL_EXPORT*/ VOID GetMaterialInfo(DWORD sceneHandle, DWORD mtrlHandle, MtrlInfo* pMtrlInfo);
//设置材质信息
/*DLL_EXPORT*/ VOID SetMaterialInfo(DWORD sceneHandle, DWORD mtrlHandle, MtrlInfo* pMtrlInfo);

//判断当前材质是否被模型关联
/*DLL_EXPORT*/ DWORD IsMtrlUsingByModel(DWORD sceneHandle, DWORD mtrlHandle);

//设置是否显示材质的贴图
/*DLL_EXPORT*/ VOID SetShowMtrlTexture(DWORD sceneHandle, DWORD mtrlHandle, BOOL showTexture);
//获得是否显示贴图的状态
/*DLL_EXPORT*/ BOOL GetShowMtrlTexture(DWORD sceneHandle, DWORD mtrlHandle);

/** 获取指定的材质是否使用了贴图
*/
/*DLL_EXPORT*/ BOOL IsMaterialHasTexture(DWORD sceneHandle, DWORD mtrlHandle);

//-------------------------------------------------------------------------------------------
///贴图相关的方法
//-------------------------------------------------------------------------------------------
//从本地文件创建贴图
///*DLL_EXPORT*/ DWORD CreateTextureFromFile(DWORD sceneHandle, CHAR* texName, 
//									   CHAR* strImageFile, INT mipLevel);
////删除贴图
///*DLL_EXPORT*/ VOID	DeleteTexture(DWORD sceneHandle, DWORD texHandle);

/** 得到场景中贴图的数量, 包括2D、3D使用的贴图
*/ 
// /*DLL_EXPORT*/ UINT GetTextureCount(DWORD sceneHandle);
/*DLL_EXPORT*/ UINT GetTextureImageCount(DWORD sceneHandle);

/** 根据索引得到纹理的文件名称（不包含路径）
@param in sceneHandle : 场景指针
@param in index : 纹理在队列中的索引
@param out TextureName : 纹理名称
*/
// /*DLL_EXPORT*/ BOOL GetTextureNamebyIndex(DWORD sceneHandle, INT index, CHAR* TextureName);
/*DLL_EXPORT*/ BOOL GetTextureImageNamebyIndex(DWORD sceneHandle, INT index, CHAR* TextureName);

/** 图像是否正被使用
@param ImagePureName in : 图像文件名称，不包含路径
*/
/*DLL_EXPORT*/ BOOL IsImageInused(DWORD sceneHandle, const CHAR* ImagePureName);

// 得到纹理被多少个材质使用
// /*DLL_EXPORT*/ DWORD GetTextureReferencedCount(DWORD sceneHandle, const CHAR* pTextureName);

/** 得到第index个材质使用指定纹理的材质信息
@param in sceneHandle : 场景指针
@param in pTextureName:纹理文件名称，不包含路径
@param in iMaterialIndex:使用指定纹理的材质序号
@param out pMaterailName : 材质名称
@param out mapChannel : 通道
@return 成功得到材质信息返回true，否则false
*/
// /*DLL_EXPORT*/ BOOL GetMaterialInfoofTexture(DWORD sceneHandle, const CHAR* pTextureName, INT iMaterialIndex, CHAR* pMaterailName/*, INT &mapChannel*/);

////根据贴图的名称得到贴图的句柄
///*DLL_EXPORT*/ DWORD GetTextureHandleByName(DWORD sceneHandle, CHAR* strTexName);
////根据贴图的编号得到贴图的句柄
///*DLL_EXPORT*/ DWORD GetTextureHandleByIndex(DWORD sceneHandle, INT index);
//
////根据贴图的句柄得到贴图的名称
///*DLL_EXPORT*/ VOID GetTextureNameByHandle(DWORD sceneHandle, DWORD texHandle, CHAR* strTexName);
////根据贴图的句柄得到其编号
///*DLL_EXPORT*/ INT GetTextureIndexByHandle(DWORD sceneHandle, DWORD mtrlHandle);

////设置贴图所用的图像文件
///*DLL_EXPORT*/ VOID	SetTextureSource(DWORD sceneHandle, DWORD texHandle, CHAR* strImageFile);
////获得贴图所用的图像源文件
///*DLL_EXPORT*/ VOID GetTextureSource(DWORD sceneHandle, DWORD texHandle, CHAR* strImageFile);

//获得贴图的参数
///*DLL_EXPORT*/ VOID GetTextureInfo(DWORD sceneHandle, DWORD texHandle, TexInfo* pTexInfo);
////设置贴图参数
///*DLL_EXPORT*/ VOID SetTextureInfo(DWORD sceneHandle, DWORD texHandle, TexInfo* pTexInfo);

//调节贴图的亮度和对比度
//brightness (-100, 100);
//contrast (-100, 100);
///*DLL_EXPORT*/ VOID SetTextureBriAndCon(DWORD sceneHandle, DWORD texHandle, INT brightness, INT contrast);
//
////判断贴图是否被材质所关联, 返回关联材质句柄
///*DLL_EXPORT*/ DWORD IsTextureUsingByMaterial(DWORD sceneHandle, DWORD texHandle);


/** 创建天空盒
*/
/*DLL_EXPORT*/ VOID CreateSkyBox(DWORD sceneHandle, const CHAR* sTextureName);

/** 设置天空盒使用的贴图
*/
/*DLL_EXPORT*/ VOID ChangeSetSkyBoxTexture(DWORD sceneHandle, const CHAR* sTextureName);

/** 设置天空盒的距离
*/
/*DLL_EXPORT*/ VOID SetSkyBoxDistance(DWORD sceneHandle, FLOAT dist);

/** 设置天空盒是否有效
*/
/*DLL_EXPORT*/ VOID SetSkyBoxEnabled(DWORD sceneHandle, BOOL bEnabled);

/** 使用指定图片创建背景图,每个场景只能有一张背景图。重复设置背景图将清除之前已经创建的背景图。
*/
/*DLL_EXPORT*/ DWORD SetBackground(DWORD sceneHandle, const CHAR* sImageFile);

// 获取背景图句柄，每个场景只能有一张背景图
/*DLL_EXPORT*/ DWORD GetBackgroundHandle(DWORD sceneHandle);

/** 删除场景背景图
*/
/*DLL_EXPORT*/ VOID RemoveBackground(DWORD sceneHandle);

/** 获取背景图使用的图像名称，不包括路径
@return 成功获取背景图返回TRUE,否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetBackgroundImage(DWORD sceneHandle, CHAR *pImageFile);  

/** 设置场景的背景图是否可见
*/
/*DLL_EXPORT*/ VOID SetBackgroundVisibility(DWORD sceneHandle, BOOL bVisible);

/** 获取场景的背景图是否可见
*/
/*DLL_EXPORT*/ BOOL IsBackgroundVisible(DWORD sceneHandle);

/** 设置背景图的相对与视口的相对大小和相对位置，left/top/width/height取值范围[0, 1]，坐标原点在视口的左上角
*/
/*DLL_EXPORT*/ VOID SetBackgroundRect(DWORD sceneHandle, FLOAT left, FLOAT top, FLOAT width, FLOAT height);  

/** 获取背景图的相对与视口的相对大小和相对位置，left/top/width/height取值范围[0, 1]，坐标原点在视口的左上角
*/
/*DLL_EXPORT*/ VOID GetBackgroundRect(DWORD sceneHandle, FLOAT &left, FLOAT &top, FLOAT &width, FLOAT &height);

/** 背景图全屏显示
*/
/*DLL_EXPORT*/ VOID FullScreenBackground(DWORD sceneHandle);  


//-------------------------------------------------------------------------------------------
///摄像机相关的方法
//-------------------------------------------------------------------------------------------
/** 使用缺省参数创建一个相机
*/
/*DLL_EXPORT*/ DWORD CreateCamera(DWORD sceneHandle, const CHAR* strCameraName);

/** 创建各种预定义相机
*/
/*DLL_EXPORT*/ DWORD CreateFrontCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreateBackCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreateTopCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreateBottomCamera(DWORD sceneHandle); 
/*DLL_EXPORT*/ DWORD CreateLeftCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreateRightCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD CreatePerspectiveCamera(DWORD sceneHandle);

/** 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
*/
/*DLL_EXPORT*/ DWORD GetFrontCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetBackCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetTopCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetBottomCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetLeftCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetRightCamera(DWORD sceneHandle);
/*DLL_EXPORT*/ DWORD GetPerspectiveCamera(DWORD sceneHandle);

//得到场景中摄像机的数量
/*DLL_EXPORT*/ INT GetCameraCount(DWORD sceneHandle);

//根据像机的名称得到像机的句柄
/*DLL_EXPORT*/ DWORD GetCameraHandleByName(DWORD sceneHandle, CHAR* strCameraName);
//根据摄像机的编号得到像机的句柄
/*DLL_EXPORT*/ DWORD GetCameraHandleByIndex(DWORD sceneHandle, INT index);

//根据像机的句柄得到像机的名称
/*DLL_EXPORT*/ VOID GetCameraNameByHandle(DWORD sceneHandle, DWORD camHandle, CHAR* strCameraName);
//设置相机的名称
/*DLL_EXPORT*/ VOID SetCameraNamebyHandle(DWORD sceneHandle, DWORD camHandle, CHAR* strCameraName);
/*DLL_EXPORT*/ VOID SetCameraNamebyName(DWORD sceneHandle, CHAR* strOldCameraName, CHAR* strNewCameraName);

/** 根据相机的索引得到相机的名字
*/
/*DLL_EXPORT*/ BOOL GetCameraNameByIndex(DWORD sceneHandle, INT index, CHAR* pName);

/**删除相机
*/
/*DLL_EXPORT*/ VOID DeleteCamerabyHandle(DWORD sceneHandle, DWORD camHandle);
/**删除相机
*/
/*DLL_EXPORT*/ VOID DeleteCamerabyName(DWORD sceneHandle, CHAR* camName);

//设置当前摄像机
/*DLL_EXPORT*/ VOID SetCurrentCamera(DWORD sceneHandle, DWORD swapChainHandle, DWORD camHandle);
//得到当前摄像机句柄
/*DLL_EXPORT*/ DWORD GetCurrentCamera(DWORD sceneHandle, DWORD swapChainHandle);

//设置当前相机为系统预设相机
// /*DLL_EXPORT*/ VOID SetCurrentCameraToPreConfig(DWORD sceneHandle, DWORD swapChainHandle, CAMERATYPE cameraType);

//获得当前相机的是否为预设相机，如果不是返回NONE
// /*DLL_EXPORT*/ CAMERATYPE GetCurrentPreConfigCamera(DWORD sceneHandle, DWORD swapChainHandle);


/** 平移摄像机
@param in sceneHandle 场景管理器
@param in camHandle 相机句柄
@param in pVec 相机移动的相对向量
@param in coordEnum 参考坐标系
	//0 - 在世界坐标系平移摄像机
	//1 - 在自身坐标系平移
	//2 - walk，不能飞到天上去
*/
/*DLL_EXPORT*/ VOID MoveCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3* pVec, UINT coordEnum);

/** 旋转摄像机
@param in pVec : 各个方向上的旋转角度
@param in coordEnum : 参考坐标系
//0 - 绕世界坐标系旋转摄像机
//1 - 绕自身坐标系旋转像机
//2 - 绕穿过from中心，与世界坐标系平行的坐标系旋转
*/
/*DLL_EXPORT*/ VOID RotateCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3* pVec, UINT coordEnum);

/** 以pTargetPos点为中心,旋转相机fAngle弧度
@param in sceneHandle   : 场景管理器
@param in camHandle     : 相机句柄
@param in pTargetPos    : 围绕旋转的中心点(世界坐标系)
@param in axistype      : 围绕旋转的轴(世界坐标系)， 0：world X， 1：world y，2：world z, 3: local X, 4: local Y, 5: local Z
@param in fAngle        : 旋转的角度，以 弧度 为单位
*/  
/*DLL_EXPORT*/ VOID RoundCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3 *pTargetPos, INT axistype, FLOAT fAngle);

/** 设置相机的宽高比
*/
// /*DLL_EXPORT*/ VOID SetCameraAspect(DWORD camHandle, const FLOAT fAspectRatio);

//以定点为轴心，指定坐标轴，旋转fAngle弧度
///*DLL_EXPORT*/ VOID RoundCamera1(DWORD sceneHandle, DWORD camHandle, VECTOR3* pAxisCenter, VECTOR3* pAxisVec, FLOAT fAngle);

/**以定点为轴心, 在相机坐标系中旋转相机
@param pAxisCenter in : 相机围绕旋转的目标点
@param pXYZAangle  in : 相机旋转的角度，弧度 
*/
/*DLL_EXPORT*/ VOID RoundInCameraSpace(DWORD sceneHandle, DWORD camHandle, VECTOR3* pAxisCenter, VECTOR3* pXYZAngle);

/** 相机目标点不动，沿相机方向前后推拉相机。
@param in sceneHandle : 场景指针
@param in camHandle : 相机指针
@param dollyValue  in   : 相机推拉的距离，> 0 向前， < 1向后
*/
/*DLL_EXPORT*/ VOID DollyCamera(DWORD sceneHandle, DWORD camHandle, FLOAT dollyValue);

/** 摄像机正对模型
@param in sceneHandle : 场景指针
@param in camHandle : 相机指针
@param in modelHandle : 模型指针
*/
/*DLL_EXPORT*/ VOID SetCameraLookAtModel(DWORD sceneHandle, DWORD camHandle, DWORD modelHandle);

/*设置摄像机的世界坐标系位置
@param in sceneHandle : 场景指针
@param in camHandle   : 相机指针
@param in pPos        : 世界坐标系中的新位置
*/
/*DLL_EXPORT*/ VOID SetCameraPosition(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos);

/*获取摄像机的世界坐标系位置
@param in sceneHandle : 场景指针
@param in camHandle   : 相机指针
@param in pPos        : 世界坐标系中的新位置
*/
/*DLL_EXPORT*/ VOID GetCameraPosition(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos);

//设置摄像机的目标点
/*DLL_EXPORT*/ VOID SetCameraAt(DWORD sceneHandle, DWORD camHandle, const VECTOR3* pPos);
/*DLL_EXPORT*/ VOID GetCameraAt(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos);

/** 设置摄像机的FOV
@param in fov : 相机的fov，即y方向的视角
*/
/*DLL_EXPORT*/ VOID SetCameraFov(DWORD sceneHandle, DWORD camHandle, FLOAT fov);

/** 设置摄像机的nearClip，与相机距离小于nearClip的物体将不会被看到
*/
/*DLL_EXPORT*/ VOID SetCameraNearClip(DWORD sceneHandle, DWORD camHandle, FLOAT nearClip);

/** 设置摄像机的farClip，与相机距离大于farClip的物体将不会被看到
*/
/*DLL_EXPORT*/ VOID SetCameraFarClip(DWORD sceneHandle, DWORD camHandle, FLOAT farClip);

//设置摄像机的Roll
// /*DLL_EXPORT*/ VOID SetCameraRoll(DWORD sceneHandle, DWORD camHandle, FLOAT rollAngle);

/** 一次性获得相机的属性
*/
/*DLL_EXPORT*/ BOOL GetCameraInfo(DWORD sceneHandle, DWORD camHandle, CameraInfoEx* pCamInfo);

/** 一次性设置相机的属性
*/
/*DLL_EXPORT*/ VOID SetCameraInfo(DWORD sceneHandle, DWORD camHandle, CameraInfoEx* pCamInfo);

/** 获取相机的方向
@param vDirection out : 相机在世界坐标系中的方向，即lookat的方向。如果获取方向失败，vDirection = (0, 0, 0)
*/
/*DLL_EXPORT*/ VOID GetCameraDirection(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vDirection);

/** 设置相机的方向
@param vDirection in : 相机在世界坐标系中的方向，即lookat方向
*/
/*DLL_EXPORT*/ VOID SetCameraDirection(DWORD sceneHandle, DWORD camHandle, const VECTOR3 &vDirection);

// 获取相机的up方向
/*DLL_EXPORT*/ VOID GetCameraUp(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vUp);

// 获取相机的right方向
/*DLL_EXPORT*/ VOID GetCameraRight(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vRight);

// 获取相机的near/far clip
/*DLL_EXPORT*/ VOID GetCameraClip(DWORD sceneHandle, DWORD camHandle, float &nearClip, float &farClip);
//-------------------------------------------------------------------------------------------
///灯光相关的方法
//-------------------------------------------------------------------------------------------
/** 得到场景中灯光的数量
@return 场景中的所有灯光数目
*/
/*DLL_EXPORT*/ INT GetLightCount(DWORD sceneHandle);

/** 删除灯光
*/
/*DLL_EXPORT*/ VOID	DeleteLight(DWORD sceneHandle, DWORD lightHandle);

/** 根据灯光的名称得到灯光的句柄
*/
/*DLL_EXPORT*/ DWORD GetLightHandleByName(DWORD sceneHandle, CHAR* strLightName);

/** 根据灯光的编号得到灯光的句柄
*/
/*DLL_EXPORT*/ DWORD GetLightHandleByIndex(DWORD sceneHandle, INT index);

/** 根据灯光的句柄得到灯光的名称
*/
/*DLL_EXPORT*/ VOID GetLightNameByHandle(DWORD sceneHandle, DWORD lightHandle, CHAR* strLightName);

/*设置灯光的名称
@return 设置成功返回TRUE，否则FALSE，失败可能是由于命名冲突导致的。
*/
/*DLL_EXPORT*/ BOOL SetLightName(DWORD sceneHandle, DWORD lightHandle, CHAR* strLightName);

/** 根据灯光的编号得到灯光的名称
*/
/*DLL_EXPORT*/ BOOL GetLightNameByIndex(DWORD sceneHandle, INT index, CHAR* strLightName);

/** 在世界坐标系中移动灯光
@param in pVec : 移动的目标点世界坐标，绝对值
*/
/*DLL_EXPORT*/ VOID MoveLight(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pVec);

//旋转灯光
// /*DLL_EXPORT*/ VOID RotateLight(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pRos);

/** 设置灯光的位置
@param in vPos : 灯光要移动到的新位置，参考坐标系是世界坐标系
*/
/*DLL_EXPORT*/ VOID SetLightPosition(DWORD sceneHandle, DWORD lightHandle, VECTOR3* vPos);

/**-----------------------------------------------------------------------------
// 方法名: EnableLight()
// 参  数: DWORD lightHandle, BOOL isEnable
// 用  途: 设置灯光变是否有效
//-----------------------------------------------------------------------------*/
/*DLL_EXPORT*/ VOID EnableLight(DWORD sceneHandle, DWORD lightHandle, BOOL isEnable);

//设置灯光的方向
// /*DLL_EXPORT*/ VOID SetLightDirection(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pDir);

/** 得到灯光的属性
*/
/*DLL_EXPORT*/ VOID GetLightInfo(DWORD sceneHandle, DWORD lightHandle, LightInfo* pLightInfo);

/** 设置灯光的属性
*/
/*DLL_EXPORT*/ VOID SetLightInfo(DWORD sceneHandle, DWORD lihgtHandle, LightInfo* pLightInfo);

/** 设置灯光是否有效
*/
/*DLL_EXPORT*/ VOID SetLightEnableStatus(DWORD sceneHandle, DWORD lightHandle, BOOL bEnable);

/////////////////////// 二维元素创建方法 /////////////////////////////////////////

/** 初始化GUI状态，新创建的场景如果要使用GUI，必须首先调用该方法
// 如果已经初始化，将不做任何操作，直接返回
// 如果清空或销毁场景后，可重现初始化GUI状态
*/
/*DLL_EXPORT*/ VOID InitGUIStatus(DWORD sceneHandle, DWORD renderWnd, DWORD viewport);

/** 得到光标下方的GUI对象句柄及类型,如果有多个对象，返回最上层（ZOrder最大）的一个GUI对象
@param sceneHandle in : 场景指针
@param x/y  : 视口中的坐标
@param type out ： GUI对象的类型，参考VGS_GUIOBJ_TYPE定义
@return GUI对象的指针
*/
/*DLL_EXPORT*/ DWORD GetGUIObjectUnderLoc(DWORD sceneHandle, INT x, INT y, VGS_GUIOBJ_TYPE &type);


/** 获取GUI对象的类型
@param GUIObjectHandle in : GUI对象的句柄
@return GUI对象的类型，参考VGS_GUIOBJ_TYPE定义
*/
/*DLL_EXPORT*/ VGS_GUIOBJ_TYPE GetGUIObjectType(DWORD sceneHandle, DWORD GUIObjectHandle);

/** 返回GUI对象的ZOrder，越大表示GUI对象处于越上层
*/
/*DLL_EXPORT*/ INT GetGUIObjectZOrder(DWORD sceneHandle, DWORD GUIObjectHandle);

/** 设置GUI对象的ZOrder，越大表示GUI对象处于越上层
*/
///*DLL_EXPORT*/ VOID SetObjectZOrder(DWORD GUIObjectHandle, int offset);

// GUI对象上移一层
/*DLL_EXPORT*/ VOID MoveGUIObjectUp(DWORD sceneHandle, DWORD GUIObjectHandle);

/** GUI对象下移一层
*/
/*DLL_EXPORT*/ VOID MoveGUIObjectDown(DWORD sceneHandle, DWORD GUIObjectHandle);

/** GUI对象移动到最顶层
*/
/*DLL_EXPORT*/ VOID GotoGUIObjectsTop(DWORD sceneHandle, DWORD GUIObjectHandle);

/** GUI对象移动到最底层
*/
/*DLL_EXPORT*/ VOID GotoGUIObjectsBottom(DWORD sceneHandle, DWORD GUIObjectHandle);

/** 获取所有二维元素的句柄列表
// vHandle必须要预先分配足够的空间
*/ 
/*DLL_EXPORT*/ VOID GetAllGUIObjectsList(DWORD* vHandle);

/** 获取所有二维元素的数目
*/
/*DLL_EXPORT*/ UINT GetAllGUIObjectsCount(DWORD sceneHandle);

/**得到Overlay的数量
*/
/*DLL_EXPORT*/ INT GetGUIObjectCount(DWORD sceneHandle, VGS_GUIOBJ_TYPE guitype);

/** 获取指定类型GUI对象的句柄列表
*/
/*DLL_EXPORT*/ VOID GetGUIObjectList(VGS_GUIOBJ_TYPE guitype, DWORD* vHandle);

/** 通过索引得到某种gui对象的句柄
*/
/*DLL_EXPORT*/ DWORD GetGUIObjetHandleByIndex(DWORD sceneHandle, VGS_GUIOBJ_TYPE guitype, INT index);

/** 设置GUI对象定位的参考原点
*/
/*DLL_EXPORT*/ VOID SetGUIObjOffsetOrigin(DWORD sceneHandle, DWORD overlayHandle, VGSOFFSETORIGIN offsetOrigin);

/** 获得GUI对象定位的参考原点
*/
/*DLL_EXPORT*/ VGSOFFSETORIGIN GetGUIObjOffsetOrigin(DWORD sceneHandle, DWORD overlayHandle);

/**设置GUI对象是否使用相对坐标(%)保存位置
*/
/*DLL_EXPORT*/ VOID SetGUIObjUseRelativePos(DWORD sceneHandle, DWORD guiHandle, BOOL bRelativePos);

/**获取GUI对象是否使用相对坐标(%)保存位置
*/
/*DLL_EXPORT*/ BOOL GetGUIObjUseRelativePos(DWORD sceneHandle, DWORD guiHandle);

/** 设置GUI对象是否使用相对尺寸(%)保存尺寸
*/
/*DLL_EXPORT*/ VOID SetGUIObjUseRelativeSize(DWORD sceneHandle, DWORD guiHandle, BOOL bRelativeSize);

/** 获取GUI对象是否使用相对尺寸(%)保存尺寸
*/
/*DLL_EXPORT*/ BOOL GetGUIObjUseRelativeSize(DWORD sceneHandle, DWORD guiHandle);

/** 删除 GUI
@param sceneHandle   in : 场景管理器
@param GUIHandle in : GUI句柄
*/
/*DLL_EXPORT*/ VOID	DeleteGUIObject(DWORD sceneHandle, DWORD GUIHandle);


/** 得到 GUI对象 屏幕坐标的绝对Rect
@param rect out : rect的表示的位置是GUI对象相对于视口offsetOrigin的位置,单位为像素
*/
/*DLL_EXPORT*/ VOID GetGUIObjectRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL &bRelativeSize, BOOL &bRelativePos);

/** 得到 GUI对象 屏幕坐标的绝对Rect
@param rect out : rect的表示的位置是GUI对象相对于视口左上角的位置,也就是GUI对象在屏幕的实际位置。单位为像素
*/
/*DLL_EXPORT*/ VOID GetGUIObjectAbsoluteRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL &bRelativeSize, BOOL &bRelativePos);

/** 设置 GUI对象 的相对屏幕坐标的绝对Rect
@param rect in : rect的表示的位置是GUI对象相对于视口offsetOrigin的位置,单位为像素
*/
/*DLL_EXPORT*/ VOID SetGUIObjectRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL bRelativeSize = TRUE, BOOL bRelativePos = TRUE);

/** 设置 GUI对象 的相对屏幕坐标的绝对Rect
@param rect in : rect的表示的位置是GUI对象相对于视口左上角的位置,也就是GUI对象的实际位置。单位为像素
*/
/*DLL_EXPORT*/ VOID SetGUIObjectAbsoluteRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, BOOL bRelativeSize = TRUE, BOOL bRelativePos = TRUE);

/** 设置GUI对象的位置
@param sceneHandle   in : 场景管理器
@param guiHandle in : gui句柄
@param x, y : 相对于视口offsetOrigin的坐标,单位为像素
@param bRelative : true采用相对位置，false采用绝对位置。如果采用相对位置，实际上记录了overlay左上角位置相对于目前视口大小的百分比，
视口缩放时，overlay会保持这个比例不变
*/
/*DLL_EXPORT*/ VOID SetGUIObjectPosition(DWORD sceneHandle, DWORD guiHandle, INT x, INT y, BOOL bRelative = true);

/** 设置gui对象的尺寸
@param sceneHandle   in : 场景管理器
@param guiHandle in : overlay句柄
@param width, height    : Overlay的宽高
@param bRelative : true采用相对大小，false采用绝对大小。如果采用相对大小，实际上记录了overlay尺寸相对于目前视口尺寸的百分比，
视口缩放时，gui对象会保持这个比例不变
*/
/*DLL_EXPORT*/ VOID SetGUIObjectSize(DWORD sceneHandle, DWORD guiHandle, INT width, INT height, BOOL bRelative = true);
/**得到 gui对象 的透明度
@return gui对象的不透明度 [0, 100]
*/
/*DLL_EXPORT*/ INT GetGUIObjectOpacity(DWORD sceneHandle, DWORD guiHandle); 

/**设置 gui对象 的透明度
@opacity in : gui对象的不透明度 [0, 100]
*/
/*DLL_EXPORT*/ VOID SetGUIObjectOpacity(DWORD sceneHandle, DWORD guiHandle, INT opacity); 

/** 设置gui对象是否可见
*/
/*DLL_EXPORT*/ VOID SetGUIObjectVisibility(DWORD sceneHandle, DWORD guiHandle, BOOL visible);

/** 获取gui对象是否可见
*/
/*DLL_EXPORT*/ BOOL GetGUIObjectVisibility(DWORD sceneHandle, DWORD guiHandle);

/** 通过名称得到gui对象
*/
/*DLL_EXPORT*/ DWORD GetGUIObjectHandleByName(DWORD sceneHandle, CHAR* name);

/** 得到gui对象的名称
*/
/*DLL_EXPORT*/ VOID	GetGUIObjectNameByHandle(DWORD sceneHandle, DWORD guiHandle, CHAR* name);

/** 设置gui对象的名称
*/
/*DLL_EXPORT*/ BOOL SetGUIObjectName(DWORD sceneHandle, DWORD guiHandle, CHAR* NewName);

/** 设置gui对象选择框是否可见
*/
/*DLL_EXPORT*/ VOID ShowGUIObjectSelcetionBox(DWORD sceneHandle, DWORD guiHandle, BOOL bVisible);

/** 获取二维对象选择框是否显示
*/
/*DLL_EXPORT*/ BOOL GetGUISelectionBoxVisible(DWORD sceneHandle, DWORD GUIObjectHandle);

/** 设置GUI对象的鼠标事件函数名称
@param scenHandle   in : 场景指针
@param guiHandle  in : GUI对象指针
@param type         in  : 事件类型
@param sEventName   in : 函数名称
*/
/*DLL_EXPORT*/ VOID SetGuiMouseEvent(DWORD sceneHandle, DWORD guiHandle, VGS_MOUSEEVENT_TYPE type, const CHAR* sEventName);

/** 获取GUI对象的鼠标事件函数名称
@param scenHandle   in  : 场景指针
@param guiHandle  in  : GUI对象指针
@param type         in  : 事件类型
@param sEventName   out : 函数名称
@return 如果GUI对象已经制定了对应的事件函数，返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL GetGuiMouseEvent(DWORD sceneHandle, DWORD guiHandle, VGS_MOUSEEVENT_TYPE type, CHAR* sEventName);

//------------------Flash GUI Start-------------------------------------------------------------------------
/** 从Flash文件创建一个GUI.
@param scenHandle   in  : 场景指针
@param hParent		in	:	父窗体句柄.
@param sName        in : GUI名称
@param sFile       in : Flash文件名称，包含路径
@param rect        in : GUI四个角的坐标
@return : 返回Flash GUI的指针
*/
/*DLL_EXPORT*/ DWORD CreateFlashGUIFromFile(DWORD sceneHandle, HWND hParent, const char* sName, const char* sFile, const RECT &rect);
/** 相对移动Flash GUI.
@param scenHandle   in  : 场景指针
@param guiHandle  in  : GUI对象指针
@param x in: 
@param y in: 
*/
/*DLL_EXPORT*/ VOID MoveFlashGUI(DWORD sceneHandle, DWORD guiHandle,INT x, INT y);
/** 设置Flash播放状态.
@param scenHandle   in  : 场景指针
@param guiHandle  in  : GUI对象指针
@param status in: 0为暂停播放，1为从暂停点开始播放，2为停止并跳转到第一帧
*/
/*DLL_EXPORT*/ VOID SetFlashGUIPlayStatus(DWORD sceneHandle, DWORD guiHandle,INT status);
/** 给Flash GUI对象添加一个信号量.
@param scenHandle   in  : 场景指针
@param guiHandle  in  : GUI对象指针
@param SemaphoreName in : FLash中信号量的名称.
@param SemaphoreFunc in : 信号要触发的JS回调用函数.
*/
/*DLL_EXPORT*/ VOID AddFlashGUISemaphore(DWORD sceneHandle, DWORD guiHandle,const char*  SemaphoreName, const char*  SemaphoreFunc);

//DeleteFlashCtrl——由DeleteGUIObject实现、SetFlashRect——由SetGUIObjectRect和SetGUIObjectAbsoluteRect实现、SetFlashCtrlVisibility——由SetGUIObjectVisibility实现
//------------------Flash GUI End-------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
///前景图相关的方法
//-------------------------------------------------------------------------------------------
/**
由图片文件创建Overlay
@param sceneHandle in : 场景管理器
@param name        in : Overlay名称
@param sFile       in : 图片文件名称，包含路径
@param sOpcatityImageFile in : 透明图片文件名称，包含路径, 如果没有透明贴图，sOpcatityImageFile设置空即可
@param rect        in : Overlay四个角的坐标
@return 返回overlay的指针
*/
/*DLL_EXPORT*/ DWORD CreateOverlayFromFile(DWORD sceneHandle, const CHAR* name, const CHAR* sFile, const CHAR* sOpcatityImageFile, RECT &rect);

/** 获取overlay的属性
@param sceneHandle   in  : 场景管理器
@param overlayHandle in  : overlay句柄
@param info          out : Overlay的属性
*/
// /*DLL_EXPORT*/ BOOL GetOverlayInfo(DWORD sceneHandle, DWORD overlayHandle, OverlayInfo &info);

/**得到Overlay的数量
*/
/*DLL_EXPORT*/ INT GetOverlayCount(DWORD sceneHandle);

// 获取所有Overlay的句柄列表
/*DLL_EXPORT*/ VOID GetOverlayList(DWORD sceneHandle, DWORD* vHandle);

/**获得Overlay使用的图像文件
@param sBaseFile in : 基本贴图文件
@param sOpacFile in : 透明贴图文件
*/
/*DLL_EXPORT*/ VOID GetOverlayImageFile(DWORD sceneHandle, DWORD overlayHandle, CHAR* sBaseFile, CHAR* sOpacFile);

/**设置Overlay使用的图像文件
@param sBaseFile in : 基本贴图文件
@param sOpacFile in : 透明贴图文件
*/
/*DLL_EXPORT*/ VOID SetOverlayImageFile(DWORD sceneHandle, DWORD overlayHandle, const CHAR* sBaseFile, const CHAR* sOpacFile = NULL);

//-------------------------------------------------------------------------------------------
///Button相关的方法
//-------------------------------------------------------------------------------------------
/** 创建Button
@param  sName             in ：按钮的名称
@param rect               in ：按钮所在区域的四个坐标，控制按钮的位置和大小
@param mouseUpImageFile   in ：鼠标松开的源图像文件
@param mouseOverImageFile in ：鼠标移上的源图像文件
@param mouseDownImageFile in ：鼠标按下的源图像文件
@return 如果成功创建，返回按钮指针，否则0
*/
/*DLL_EXPORT*/ DWORD CreateButtonFromFile(DWORD sceneHandle, const CHAR* sName, const RECT &rect, 
									  const CHAR* mouseUpImageFile,
									  const CHAR* mouseOverImageFile = NULL,
									  const CHAR* mouseDownImageFile = NULL);

/** 获得按钮的回调函数名称, 长度不超过MAX_NAMESTR_LENGTH
*/
/*DLL_EXPORT*/ VOID GetButtonCallBack(DWORD sceneHandle, DWORD buttonHandle, CHAR* callBackFunc);

/** 设置按钮的回调函数名称[按钮按下，回调函数], 长度不超过MAX_NAMESTR_LENGTH
*/
/*DLL_EXPORT*/ VOID SetButtonCallBack(DWORD sceneHandle, DWORD buttonHandle, const CHAR* callBackFunc);

/** 获得按钮使用的MouseUp图像文件,也就是正常状态下显示的图像文件
*/
/*DLL_EXPORT*/ VOID GetButtonMouseUpImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseUpImageFile);

/** 设置按钮使用的MouseUp图像文件
*/
/*DLL_EXPORT*/ VOID SetButtonMouseUpImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseUpImageFile);

/** 获得按钮使用的Mouse over(鼠标悬停在按钮上面时)图像文件
*/
/*DLL_EXPORT*/ VOID GetButtonMouseOverImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseOverImageFile);

/** 设置按钮使用的Mouse over(鼠标悬停在按钮上面时)图像文件
*/
/*DLL_EXPORT*/ VOID SetButtonMouseOverImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseOverImageFile);

/** 获得按钮使用的MouseDown(按钮被按下)图像文件
*/
/*DLL_EXPORT*/ VOID GetButtonMouseDownImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseDownImageFile);

/** 设置按钮使用的MouseDown(按钮被按下)图像文件
*/
/*DLL_EXPORT*/ VOID SetButtonMouseDownImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseDownImageFile);

/** 设置Button是否激活
*/
/*DLL_EXPORT*/ VOID SetButtonEnable(DWORD sceneHandle, DWORD buttonHandle, BOOL enable);

/** 判断Button是否激活
*/
/*DLL_EXPORT*/ BOOL IsButtonEnable(DWORD sceneHandle, DWORD buttonHandle);

//-------------------------------------------------------------------------------------------
/** 导航图相关的方法 */
//-------------------------------------------------------------------------------------------
/**创建导航图, 
@param name        in : 导航图使用的名称
@param strMapFile  in : 导航图使用的地图文件名称，包含完整路径
@param pRect       in : 导航范围，场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
@return 返回导航图对象的句柄或0
*/
/*DLL_EXPORT*/ DWORD CreateNavigater(DWORD sceneHandle, const CHAR* name, const CHAR* strMapFile, const FRect &pRect);

/** 获得导航图底图
@param strMapFile out : 导航图背景图使用的图片文件，不包含路径
*/
/*DLL_EXPORT*/ VOID GetNavigaterMap(DWORD sceneHandle, DWORD navHandle, CHAR* strMapFile);

/** 设置导航图底图
@param strMapFile in : 导航图背景图使用的图片文件，需要包含完整路径
*/
/*DLL_EXPORT*/ VOID SetNavigaterMap(DWORD sceneHandle, DWORD navHandle, const CHAR* strMapFile);

/** 获得导航图的导航范围坐标
@param pRect out : 场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
*/
/*DLL_EXPORT*/ VOID GetNavigaterSceneRect(DWORD sceneHandle, DWORD navHandle, FRect &pRect);

/** 设置导航图的导航范围坐标
@param pRect in : 场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
*/
/*DLL_EXPORT*/ VOID SetNavigaterSceneRect(DWORD sceneHandle, DWORD navHandle, const FRect &pRect);


/**增加导航点对象
@param  navigaterHandle - 导航图对象的句柄
@param  navPointerName - 导航图点的名称
@param  pointerType - 导航图点的类型，0为相机导航，1为模型导航
@param  pointerHandle - 导航点所导航的对象的句柄，如果导航图点类型为相机，则为相机句柄，否则为模型句柄
@return 成功创建导航点返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL AddNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle,
									const CHAR* navPointerName,
									const CHAR* strPointerFile, 
									INT	  pointerType   = 0,	// 0 - camera or 1 - model 
									DWORD pointerHandle = 0     //camera or model handle
									);

/** 使用已有的导航图点创建一个新导航点
@param  navPointerName   in : 要创建的导航点名称
@param  sSrcPointerName   in : 已存在的导航点名称，新创建的点会使用其图片，并且具有相同的导航类型
@return 成功创建导航点返回TRUE，否则FALSE
*/
/*DLL_EXPORT*/ BOOL CloneNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* navPointerName, 
									  const CHAR* sSrcPointerName);

/** 删除导航点
@param navigaterHandle in : 导航图
@param sName           in : 导航点名称
*/
/*DLL_EXPORT*/ VOID DeleteNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName);

/** 得到Navigater的导航点数量
@param navigaterHandle in : 导航图对象指针
@return 导航图中的导航点数目
*/
/*DLL_EXPORT*/ INT GetNavPointerCount(DWORD sceneHandle, DWORD navigaterHandle);

/** 根据索引，获取导航图的导航点名称
*/
/*DLL_EXPORT*/ VOID GetNavPointerNameByIndex(DWORD sceneHandle, DWORD navigaterHandle, INT index, CHAR* sName);

/** 获得导航图点所使用的图片名称，不带路径
@param navigaterHandle in : 导航图对象指针
@param sName           in : 导航点名称
@param strPointerFile  out : 导航点使用的图片名称，不包含路径
*/
/*DLL_EXPORT*/ VOID GetNavPointerImage(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, CHAR* strPointerFile);

/** 设置导航图点所使用的图片名称
@param navigaterHandle in : 导航图对象指针
@param sName           in : 导航点名称
@param strPointerFile  in : 导航点使用的图片名称，需要包含完整路径
*/
/*DLL_EXPORT*/ VOID SetNavPointerImage(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, const CHAR* strPointerFile);

/** 获得导航点的导航类型和导航对象
*/
/*DLL_EXPORT*/ BOOL GetNavPointerObj(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &pointerType, DWORD &pointerHandle);

/** 设置导航点的导航类型和导航对象
*/
/*DLL_EXPORT*/ VOID SetNavPointerObj(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT pointerType, DWORD pointerHandle);

/** 设置导航点是否可见
*/
/*DLL_EXPORT*/ VOID SetNavPointerVisiblilty(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, BOOL bVisible);

/** 获取导航点是否可见
*/
/*DLL_EXPORT*/ BOOL GetNavPointerVisiblilty(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName);

/** 设置导航点的不透明度 [0, 100]
*/
/*DLL_EXPORT*/ VOID SetNavPointerOpacity(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT opacity);

/** 获取导航点的不透明度 [0, 100]
*/
/*DLL_EXPORT*/ INT  GetNavPointerOpacity(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName);

/** 设置导航点的宽度和高度，像素数目
*/
/*DLL_EXPORT*/ VOID SetNavPointerSize(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT width, INT height);

/** 获取导航点的宽度和高度，像素数目
*/
/*DLL_EXPORT*/ BOOL GetNavPointerSize(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &width, INT &height);

/** 设置导航点的位置，像素数目
*/
/*DLL_EXPORT*/ VOID SetPointerPosition(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT left, INT top);

/** 获取导航点的位置，像素数目
*/
/*DLL_EXPORT*/ BOOL GetPointerPosition(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &left, INT &top);

/** 导航点更名
*/
/*DLL_EXPORT*/ VOID RenamePointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sOldName, const CHAR* sNewName);

//-------------------------------------------------------------------------------------------
///字体相关的方法
//-------------------------------------------------------------------------------------------
//新建字体对象
/*DLL_EXPORT*/ DWORD NewVGSFont(DWORD sceneHandle, CHAR* name, CHAR* fontName);
//删除字体对象
/*DLL_EXPORT*/ VOID DeleteVGSFont(DWORD sceneHandle, DWORD fontHandle);

//得到字体对象的数量
/*DLL_EXPORT*/ INT GetVGSFontCout(DWORD sceneHandle);
 
//通过字体对象的编号得到字体对象的句柄
/*DLL_EXPORT*/ DWORD GetVGSFontHandleByIndex(DWORD sceneHandle, INT index);
//通过字体的名称得到字体对象
/*DLL_EXPORT*/ DWORD GetVGSFontHandleByName(DWORD sceneHandle, CHAR* name);

//通过字体对象的句柄获得名称
/*DLL_EXPORT*/ VOID GetVGSFontNameByHandle(DWORD sceneHandle, DWORD fontHandle, CHAR* name);
//通过字体对象的句柄获得编号
/*DLL_EXPORT*/ INT	GetVGSFontIndexByHandle(DWORD sceneHandle, DWORD fontHandle);

//设置字体的颜色
/*DLL_EXPORT*/ VOID SetVGSFontColor(DWORD sceneHandle, DWORD fontHandle, DWORD color);
//获得字体的颜色
/*DLL_EXPORT*/ VOID GetVGSFontColor(DWORD sceneHandle, DWORD fontHandle, VGSRGB* pColor);

//设置字体的大小
/*DLL_EXPORT*/ VOID SetVGSFontSize(DWORD sceneHandle, DWORD fontHandle, INT size);
//获得字体的大小
/*DLL_EXPORT*/ INT GetVGSFontSize(DWORD sceneHandle, DWORD fontHandle);

//设置字体的粗体状态
/*DLL_EXPORT*/ VOID SetVGSFontBold(DWORD sceneHandle, DWORD fontHandle, BOOL bBold);
//获得字体的粗体状态
/*DLL_EXPORT*/ BOOL GetVGSFontBold(DWORD sceneHandle, DWORD fontHandle);

//设置字体斜体状态
/*DLL_EXPORT*/ VOID SetVGSFontItalic(DWORD sceneHandle, DWORD fontHandle, BOOL bItalic);
//获得字体的斜体状态
/*DLL_EXPORT*/ BOOL GetVGSFontItalic(DWORD sceneHandle, DWORD fontHandle);

//设置字体下划线状态
/*DLL_EXPORT*/ VOID SetVGSFontUnderline(DWORD sceneHandle, DWORD fontHandle, BOOL bUnderline);
//获得字体的下划线状态
/*DLL_EXPORT*/ BOOL GetVGSFontUnderline(DWORD sceneHandle, DWORD fontHandle);


//-------------------------------------------------------------------------------------------
///文字相关的方法
//-------------------------------------------------------------------------------------------
//新建文字对象
// /*DLL_EXPORT*/ DWORD NewText(DWORD sceneHandle, CHAR* textName, DWORD fontHandle, CHAR* strText);
/*DLL_EXPORT*/ DWORD NewText(DWORD sceneHandle, const CHAR* textName, const CHAR* fontName, const CHAR* strText);

//删除文字对象
/*DLL_EXPORT*/ VOID DeleteText(DWORD sceneHandle, DWORD textHandle);

//得到场景中文字对象的数量
/*DLL_EXPORT*/ INT	 GetTextCount(DWORD sceneHandle);

//通过文字对象的编号得到文字对象的句柄
/*DLL_EXPORT*/ DWORD GetTextHandleByIndex(DWORD sceneHandle, INT index);
//通过文字的名称得到文字对象
/*DLL_EXPORT*/ DWORD GetTextHandleByName(DWORD sceneHandle, CHAR* textName);

//通过文字对象的句柄获得名称
/*DLL_EXPORT*/ VOID GetTextNameByHandle(DWORD sceneHandle, DWORD textHandle, CHAR* name);
//通过文字对象的句柄获得编号
/*DLL_EXPORT*/ INT	GetTextIndexByHandle(DWORD sceneHandle, DWORD textHandle);

//设置文字对象的位置
/*DLL_EXPORT*/ VOID SetTextPosition(DWORD sceneHandle, DWORD textHandle, UINT x, UINT y);
//获得文字对象的位置
/*DLL_EXPORT*/ VOID GetTextPosition(DWORD sceneHandle, DWORD textHandle, VECTOR2* pVec2);

//设置文字对象的颜色
/*DLL_EXPORT*/ VOID SetTextRGB(DWORD sceneHandle, DWORD textHandle, DWORD color);
//获得文字对象的颜色
/*DLL_EXPORT*/ DWORD GetTextRGB(DWORD sceneHandle, DWORD textHandle);

//设置文字的内容
/*DLL_EXPORT*/ VOID SetTextString(DWORD sceneHandle, DWORD textHandle, CHAR* strText);
//获得文字对象的字符串
/*DLL_EXPORT*/ VOID GetTextString(DWORD sceneHandle, DWORD textHandle, CHAR* strText);

//-------------------------------------------------------------------------------------------
///Image相关的方法, 新建的Image对象将作为资源保存在E3D文件中，可以动态调用
//-------------------------------------------------------------------------------------------
//新建Image资源
/*DLL_EXPORT*/ DWORD NewImage(DWORD sceneHandle, CHAR* name, CHAR* fileName);
//删除Image对象
/*DLL_EXPORT*/ VOID DeleteImage(DWORD sceneHandle, DWORD imageHandle);

//给Image对象重新编号
/*DLL_EXPORT*/ VOID ResetImageIndex(DWORD sceneHandle);
//得到Image的数量
/*DLL_EXPORT*/ INT GetImageCount(DWORD sceneHandle);

//通过编号得到Image对象
/*DLL_EXPORT*/ DWORD GetImageHandleByIndex(DWORD sceneHandle, INT index);
//通过图像名称得到Image对象
/*DLL_EXPORT*/ DWORD GetImageHandleByName(DWORD sceneHandle, CHAR* name);

//通过Image的Handle得到名字
/*DLL_EXPORT*/ VOID GetImageNameByHandle(DWORD sceneHandle, DWORD imageHandle, CHAR* name);
//设置Image对象的名称
/*DLL_EXPORT*/ VOID SetImageName(DWORD sceneHandle, DWORD imageHandle, CHAR* strImageName);
//通过Image对象的Handle得到Index
/*DLL_EXPORT*/ INT GetImageIndexByHandle(DWORD sceneHandle, DWORD imageHandle);

//设置Image所用的图像文件
/*DLL_EXPORT*/ VOID	SetImageSource(DWORD sceneHandle, DWORD imageHandle, CHAR* strImageFile);
//获得Image所用的图像源文件
/*DLL_EXPORT*/ VOID GetImageSource(DWORD sceneHandle, DWORD imageHandle, CHAR* strImageFile);

//得到图像文件的信息
/*DLL_EXPORT*/ VOID GetImageInfoByFile(DWORD sceneHandle, CHAR* imageFileName, ImageInfo* pImageInfo);
//得到图像对象的信息
/*DLL_EXPORT*/ VOID GetImageInfoByHandle(DWORD sceneHandle, DWORD imageHandle, ImageInfo* pImageInfo);


//-------------------------------------------------------------------------------------------
///模型成组相关的方法
//-------------------------------------------------------------------------------------------
/*DLL_EXPORT*/ DWORD CreateModelGroup(DWORD sceneHandle, CHAR* groupName);

/*DLL_EXPORT*/ VOID	DeleteModelGroup(DWORD sceneHandle, DWORD ModelGroupHandle);
/*DLL_EXPORT*/ VOID	DeleteModelGroupByName(DWORD sceneHandle, CHAR* groupName);
/*DLL_EXPORT*/ VOID	DeleteModelGroupByIndex(DWORD sceneHandle, INT index);
/*DLL_EXPORT*/ VOID	DeleteAllModelGroup(DWORD sceneHandle);

/*DLL_EXPORT*/ INT	GetModelGroupCount(DWORD sceneHandle);
/*DLL_EXPORT*/ VOID	ResetModelGroupIndex(DWORD sceneHandle);

/*DLL_EXPORT*/ DWORD GetModelGroupHandleByName(DWORD sceneHandle, CHAR* groupName);
/*DLL_EXPORT*/ DWORD GetModelGroupHandleByIndex(DWORD sceneHandle, INT index);

/*DLL_EXPORT*/ VOID	GetModelGroupNameByHandle(DWORD sceneHandle, DWORD ModelGroupHandle, CHAR* groupName);
/*DLL_EXPORT*/ INT	GetModelGroupIndexByHandle(DWORD sceneHandle, DWORD ModelGroupHandle);
	
/*DLL_EXPORT*/ VOID	AddModelToModelGroup(DWORD sceneHandle, DWORD modelGroupHandle, DWORD modelHandle);
/*DLL_EXPORT*/ VOID	DeleteModelFromModelGroup(DWORD sceneHandle, DWORD modelGroupHandle, DWORD modelHandle);
/*DLL_EXPORT*/ VOID	ClearModelGroup(DWORD sceneHandle, DWORD modelGroupHandle);

//获得ModelGroup中的模型数量
/*DLL_EXPORT*/ INT GetModelCountInModelGroup(DWORD sceneHandle, DWORD modelGroupHandle);
//获得ModelGroup中的模型
/*DLL_EXPORT*/ DWORD GetModelHandleInModelGroupByIndex(DWORD sceneHandle, DWORD modelGroupHandle, INT index);

//获得ModelGroup中的模型组数量
/*DLL_EXPORT*/ INT GetModelGroupCountInModelGroup(DWORD sceneHandle, DWORD modelGroupHandle);
//获得ModelGroup中的模型组
/*DLL_EXPORT*/ DWORD GetModelGroupHandleInModelGroupByIndex(DWORD sceneHandle, DWORD modelGroupHandle, INT index);

//增加ModelGroup到ModelGroup
/*DLL_EXPORT*/ VOID AddModelGroupToModelGroup(DWORD sceneHandle, DWORD parentModelGroupHandle, DWORD modelGroupHandle);
//删除ModelGroup从ModelGroup
/*DLL_EXPORT*/ VOID DeleteModelGroupFromModelGroup(DWORD sceneHandle, DWORD parentModelGroupHandle, DWORD modelGroupHandle);

//得到ModelGroup的Parent的Handle, 返回ModelGroup的父级ModelGroup, 最高级ModelGroup返回0
/*DLL_EXPORT*/ DWORD GetModelGroupParent(DWORD sceneHandle, DWORD modelGroupHandle);

//改变ModelGroup的名字
/*DLL_EXPORT*/ VOID ChangeModelGroupName(DWORD sceneHandle, DWORD modelGroupHandle, CHAR* groupName);

//-------------------------------------------------------------------------------------------
///后期特效处理的相关方法
//-------------------------------------------------------------------------------------------
/*DLL_EXPORT*/ VOID CreateBloomEffect(DWORD viewportHandle);
/*DLL_EXPORT*/ VOID EnableBloomEffect(DWORD viewportHandle, BOOL enabled);
/*DLL_EXPORT*/ VOID DestroyBloomEffect(DWORD viewportHandle);

/*DLL_EXPORT*/ VOID CreateHDREffect(DWORD viewportHandle);
/*DLL_EXPORT*/ VOID EnableHDREffect(DWORD viewportHandle, BOOL enabled);
/*DLL_EXPORT*/ VOID DestroyHDREffect(DWORD viewportHandle);

//创建动态模糊特效对象
/*DLL_EXPORT*/ VOID CreateFSMotionBlur(DWORD sceneHandle);
//打开动态模糊特效
/*DLL_EXPORT*/ VOID	OpenFSMotionBlur(DWORD sceneHandle);
//关闭动态模糊特效
/*DLL_EXPORT*/ VOID    CloseFSMotionBlur(DWORD sceneHandle);
//设置动态模糊的强度, 0-100
/*DLL_EXPORT*/ VOID	SetFSMotionBlurPower(DWORD sceneHandle, INT value);

//---------------------------------------------------------------------------------
//创建全屏光晕对象
/*DLL_EXPORT*/ VOID CreateFSGlow(DWORD sceneHandle);
//打开全屏光晕特效
/*DLL_EXPORT*/ VOID OpenFSGlow(DWORD sceneHandle);
//关闭全屏光晕特效
/*DLL_EXPORT*/ VOID CloseFSGlow(DWORD sceneHandle);
//设置全屏光晕强度 0-100
/*DLL_EXPORT*/ VOID SetFSGlowPower(DWORD sceneHandle, INT value);

//----------------------------------------------------------------------------------
//创建LensFlare镜头光斑对象
/*DLL_EXPORT*/ VOID	CreateLensFlare(DWORD sceneHandle, VECTOR3* sunPos);
//删除镜头光斑对象
/*DLL_EXPORT*/ VOID		DeleteLensFlare(DWORD sceneHandle);

/** 创建雾效
@param   fogMode         in : 雾的类型
@param   red/green/blue  in : 雾的颜色
@param   density         in : 雾的密度
@param   start/end       in : 雾的开始结束距离（到相机的距离）
*/
 /*DLL_EXPORT*/ VOID CreateFog(DWORD sceneHandle, INT fogMode, INT red, INT green, INT blue, FLOAT density, FLOAT start, FLOAT end);

//-------------------------------------------------------------------------------------------
///播放声音相关的方法
//-------------------------------------------------------------------------------------------
/*DLL_EXPORT*/ DWORD CreateAudio(DWORD sceneHandle, CHAR* name, CHAR* AudioFilePath, INT repeatCnt);

/*DLL_EXPORT*/ VOID DeleteAudio(DWORD sceneHandle, DWORD audioHandle);
/*DLL_EXPORT*/ VOID DeleteAudioByName(DWORD sceneHandle, CHAR* name);
/*DLL_EXPORT*/ VOID DeleteAudioByIndex(DWORD sceneHandle, INT index);
/*DLL_EXPORT*/ VOID DeleteAllAudio(DWORD sceneHandle);

/*DLL_EXPORT*/ INT GetAudioCount(DWORD sceneHandle);
/*DLL_EXPORT*/ VOID SetAudioName(DWORD sceneHandle, DWORD audioHandle, CHAR* name);

/*DLL_EXPORT*/ DWORD GetAudioHandleByName(DWORD sceneHandle, CHAR* name);
/*DLL_EXPORT*/ DWORD GetAudioHandleByIndex(DWORD sceneHandle, INT index);

/*DLL_EXPORT*/ VOID GetAudioNameByHandle(DWORD sceneHandle, DWORD audioHandle, CHAR* name);
/*DLL_EXPORT*/ INT GetAudioIndexByHandle(DWORD sceneHandle, DWORD audioHandle);

/*DLL_EXPORT*/ VOID SetAudioFilePath(DWORD sceneHandle, DWORD audioHandle, CHAR* AudioFilePath);
/*DLL_EXPORT*/ VOID GetAudioFilePath(DWORD sceneHandle, DWORD audioHandle, CHAR* AudioFilePath);

/*DLL_EXPORT*/ VOID SetAudioRepeatCount(DWORD sceneHandle, DWORD audioHandle, INT repeatCount);
/*DLL_EXPORT*/ INT	GetAudioRepeatCount(DWORD sceneHandle, DWORD audioHandle);

/*DLL_EXPORT*/ VOID PlayAudio(DWORD sceneHandle, DWORD audioHandle);
/*DLL_EXPORT*/ VOID PauseAudio(DWORD sceneHandle, DWORD audioHandle);
/*DLL_EXPORT*/ VOID StopAudio(DWORD sceneHandle, DWORD audioHandle);
/*DLL_EXPORT*/ VOID RewindAudio(DWORD sceneHandle, DWORD audioHandle);

//-------------------------------------------------------------------------------------------
///用户输入相关的方法
//-------------------------------------------------------------------------------------------

///// 控制对象方法 ////////////////////////////////
// 设置当前控制器
/*DLL_EXPORT*/ VOID SetCurrentController(DWORD sceneHandle, DWORD ControllerHandle);

// 获取当前控制器
/*DLL_EXPORT*/ DWORD GetCurrentController(DWORD sceneHandle);

// 获取控制对象的类型
/*DLL_EXPORT*/ CONTROLTYPE GetControllerType(DWORD ControllerHandle);

// 获取当前控制器使用的相机
/*DLL_EXPORT*/ DWORD GetCurrentControllerCamera(DWORD sceneHandle);

// 设置控制器的相机
/*DLL_EXPORT*/ VOID SetControllerCamera(DWORD sceneHandle, DWORD ControllerHandle, DWORD CameraHandle);

// 获取当前控制器的相机
/*DLL_EXPORT*/ DWORD GetControllerCamera(DWORD sceneHandle, DWORD ControllerHandle);

//-------------------------------------------------------------------------------------------
///Walker漫游对象相关的方法
//-------------------------------------------------------------------------------------------
/** 创建Walker漫游对象
param in scneneHandle : 场景指针
param in wInfo : walker的参数
return 如果成功返回walker的指针，否则返回NULL
*/
/*DLL_EXPORT*/ DWORD CreateWalker(DWORD sceneHandle, WalkerInfo* wInfo);
//删除Walker漫游对象
/*DLL_EXPORT*/ VOID DeleteWalkerByHandle(DWORD sceneHandle, DWORD walkerHandle);
/*DLL_EXPORT*/ VOID DeleteWalkerByName(DWORD sceneHandle, const CHAR* pName);
//得到Walker的数量
/*DLL_EXPORT*/ INT GetWalkerCount(DWORD sceneHandle);

//通过编号获得Walker
/*DLL_EXPORT*/ DWORD GetWalkerHandleByIndex(DWORD sceneHandle, INT index);

//通过name获得Walker
/*DLL_EXPORT*/ DWORD GetWalkerHandleByName(DWORD sceneHandle, const CHAR* sName);

//通过句柄获得名称
/*DLL_EXPORT*/ VOID GetWalkerNameByHandle(DWORD sceneHandle, DWORD walkerHandle, CHAR* name);

//设置预览状态时行走的walker的属性
/*DLL_EXPORT*/ VOID SetWalkerProp(DWORD sceneHandle, DWORD walkerHandle, WalkerInfo* wInfo);
//从摄像机得到Walker的属性
/*DLL_EXPORT*/ VOID GetWalkerProp(DWORD sceneHandle, DWORD walkerHandle, WalkerInfo* wInfo);


//-------------------------------------------------------------------------------------------
///Editor控制对象相关的方法
//-------------------------------------------------------------------------------------------
//创建Editor对象
/*DLL_EXPORT*/ DWORD CreateEditor(DWORD sceneHandle, EditorInfo* esInfo);
//删除Editor对象
/*DLL_EXPORT*/ VOID DeleteEditorbyHandle(DWORD sceneHandle, DWORD editorHandle);
//删除Editor对象
/*DLL_EXPORT*/ VOID DeleteEditorbyName(DWORD sceneHandle, const CHAR* pName);

//得到Editor的数量
/*DLL_EXPORT*/ INT GetEditorCount(DWORD sceneHandle);

//通过编号获得Editor
/*DLL_EXPORT*/ DWORD GetEditorHandleByIndex(DWORD sceneHandle, INT index);
//通过名称获得Editor
/*DLL_EXPORT*/ DWORD GetEditorHandleByName(DWORD sceneHandle, CHAR* name);

//通过句柄获得名称
/*DLL_EXPORT*/ VOID GetEditorNameByHandle(DWORD sceneHandle, DWORD editorHandle, CHAR* name);

//设置预览状态时行走的Editor的属性
/*DLL_EXPORT*/ VOID SetEditorProp(DWORD sceneHandle, DWORD editorHandle, EditorInfo* eInfo);
//从摄像机得到Editor的属性
/*DLL_EXPORT*/ VOID GetEditorProp(DWORD sceneHandle, DWORD editorHandle, EditorInfo* eInfo);

/** 设置editor所对的目标点，实际上要围绕该点旋转
@param lookatType in : looat的类型，0：场景中心 1：场景中的一点
*/
/*DLL_EXPORT*/ VOID SetEditorLookAt(DWORD sceneHandle, DWORD editorHandle,	const VECTOR3 &v3, INT lookatType = 0);

/** 设置editor所对的模型，实际上要围绕模型包围盒的中心旋转
*/
/*DLL_EXPORT*/ VOID SetEditorLookAtModel(DWORD sceneHandle, DWORD editorHandle,	DWORD modelHandle);

// 获取editor控制对象正对的模型名称，如果没有正对模型，返回false
/*DLL_EXPORT*/ BOOL GetEditorLookAtModelName(DWORD sceneHandle, DWORD editorHandle, CHAR* sName);

//-------------------------------------------------------------------------------------------
///Flyer控制对象相关的方法
//-------------------------------------------------------------------------------------------
//创建Flyer对象
/*DLL_EXPORT*/ DWORD CreateFlyer(DWORD sceneHandle, FlyerInfo* fInfo);
//删除Flyer对象
/*DLL_EXPORT*/ VOID DeleteFlyerbyHandle(DWORD sceneHandle, DWORD flyerHandle);
//删除Flyer对象
/*DLL_EXPORT*/ VOID DeleteFlyerbyName(DWORD sceneHandle, const CHAR* pName);

//得到Flyer的数量
/*DLL_EXPORT*/ INT GetFlyerCount(DWORD sceneHandle);

//通过编号获得Flyer
/*DLL_EXPORT*/ DWORD GetFlyerHandleByIndex(DWORD sceneHandle, INT index);
//通过名称获得Flyer
/*DLL_EXPORT*/ DWORD GetFlyerHandleByName(DWORD sceneHandle, CHAR* name);

//通过句柄获得名称
/*DLL_EXPORT*/ VOID GetFlyerNameByHandle(DWORD sceneHandle, DWORD flyerHandle, CHAR* name);

//设置预览状态时行走的Flyer的属性
/*DLL_EXPORT*/ VOID SetFlyerProp(DWORD sceneHandle, DWORD flyerHandle, FlyerInfo* fInfo);
//从摄像机得到Editor的属性
/*DLL_EXPORT*/ VOID GetFlyerProp(DWORD sceneHandle, DWORD flyerHandle, FlyerInfo* fInfo);


//-------------------------------------------------------------------------------------------
///关键帧动画相关的方法
//-------------------------------------------------------------------------------------------
/** 获取指定类型动画的数目
@param in type : 目前有四种
	VAT_KEYFRM = 0,       // 关键帧动画
	VAT_CAMERA,           // 相机动画
	VAT_LIGHT,            // 灯光动画
	VAT_SKELETON          // 骨骼动画
*/
/*DLL_EXPORT*/ DWORD GetAnimationCount(DWORD sceneHandle, VGS_ANIMATIONTYPE type);

/** 根据索引得到动画的名称
@param in type  : VAT_KEYFRM：关键帧动画，VAT_CAMERA：相机动画，VAT_LIGHT：灯光动画, VAT_SKELETON:骨骼动画
@param in index : 动画索引
@param out sName : 名称
@return 得到名称，返回true，否则false
*/
/*DLL_EXPORT*/ BOOL GetAnimationNameByIndex(DWORD sceneHandle, VGS_ANIMATIONTYPE type, int index, CHAR* sName);

/** 播放指定名称的动画
*/
/*DLL_EXPORT*/ VOID PlayAnimation(DWORD sceneHandle, const CHAR* sAnimName);

/** 播放所有动画
*/
/*DLL_EXPORT*/ VOID PlayAllAnimation(DWORD sceneHandle);

/** 暂停指定名称的动画
*/
/*DLL_EXPORT*/ VOID PauseAnimation(DWORD sceneHandle, const CHAR* sAnimName);
/** 暂停所有动画
*/
/*DLL_EXPORT*/ VOID PauseAllAnimation(DWORD sceneHandle);

/** 停止指定名称的动画
*/
/*DLL_EXPORT*/ VOID StopAnimation(DWORD sceneHandle, const CHAR* sAnimName);
/** 停止所有动画
*/
/*DLL_EXPORT*/ VOID StopAllAnimation(DWORD sceneHandle);

/** 得到关键帧动画的帧数
*/
/*DLL_EXPORT*/ INT GetKeyframeCount(DWORD sceneHandle, const CHAR* sAnimName);

/** 跳到某一帧
*/
/*DLL_EXPORT*/ VOID GotoFrame(DWORD sceneHandle, const CHAR* sAnimName, INT FrameIndex);

/** 得到当前正在播放的帧
*/
/*DLL_EXPORT*/ INT GetCurrentFrame(DWORD sceneHandle, const CHAR* sAnimName);

/** 设置所有关键帧动画播放的帧率
*/
/*DLL_EXPORT*/ VOID SetAllKfrPlaySpeed(DWORD sceneHandle, INT FPS);

/** 设置指定关键帧动画播放的帧率
*/
/*DLL_EXPORT*/ VOID SetKfrPlaySpeed(DWORD sceneHandle, const CHAR* sAnimName, INT FPS);

/** 获得关键帧动画播放的帧率
*/ 
/*DLL_EXPORT*/ INT GetKfrPlaySpeed(DWORD sceneHandle, const CHAR* sAnimName); 

/** 获取动画FPS的缺省值
*/
/*DLL_EXPORT*/ INT GetDefaultFPS();

/** 获取动画播是否循环播放
*/
/*DLL_EXPORT*/ BOOL GetAnimationLoop(DWORD sceneHandle, const CHAR* sAnimName); 

/** 设置动画播是否循环播放
*/
/*DLL_EXPORT*/ VOID SetAnimationLoop(DWORD sceneHandle, const CHAR* sAnimName, BOOL bLoop); 


///////////////////////// math functions /////////////////////////////////
/** 根据法线和点创建一个平面
@param vNormal in ：屏幕的法线
@param point   in : 平面上任意一点
@return : 平面的指针
*/
/*DLL_EXPORT*/ DWORD CreatePlane(const VECTOR3 &vNormal, const VECTOR3 &point);

/** 销毁一个平面
*/ 
/*DLL_EXPORT*/ VOID DestroyPlane(const DWORD hPlane);

/** 计算一点到一个平面的距离
@param hPlane  in ：平面对象指针
@param point   in : 空间中的任意一点
@param dist    out：点point到平面hPlane的距离，如果point位于hPlane的法线正方向，返回值>0,如果在负方向上，返回值<0,如果点在平面上，返回值=0
@return : 正确得到结果，返回true，否则false
*/
/*DLL_EXPORT*/ BOOL GetDistance(const DWORD hPlane, const VECTOR3 &point, FLOAT &dist);

/** 计算一个轴方向，经过一个四元数作用后的新的方向
@param quat     in  : 旋转四元数
@param vOrigDir in  : 初始的方向
@param vNewDir  out : vOrigDir经过quat作用后的新的方向
*/
/*DLL_EXPORT*/ VOID RotationAxis(const VGS_QUATERNION &quat, const VECTOR3 &vOrigDir, VECTOR3 &vNewDir); 

////////////////////////////////////////////////////////////////////////////

//////////////////// 消息响应 ///////////////////////////////

/** 消息响应
@param sceneHandle in : 场景句柄
@param hWnd in : 渲染窗口句柄。设为NULL，表示对所有渲染窗口。
@param uMsg in : 消息ID，可参考windows标准消息说明
@param wParam/lParam  in : 消息参数，可参考windows标准消息说明
*/
/*DLL_EXPORT*/ VOID VGSMsgProc(DWORD sceneHandle, DWORD hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//////////////////// 其他 ////////////////////////////////
/// 获取最近一次错误码
/*DLL_EXPORT*/ DWORD GetVGSLastErrCode();

/// 获取最近一次错误描述
/*DLL_EXPORT*/ VOID GetVGSLastErrStr(CHAR *sErrStr);

/// 得到错误码对应的详细描述
/*DLL_EXPORT*/ VOID GetVGSErrStr(DWORD Code, CHAR *sErrStr);

///////////////////坐标轴相关——Start/////////////////////////////////////////////////////////
/**设置模型坐标系被选择的轴
@param sceneHandle in : 场景句柄
@param modelHandle in : 模型句柄
@param selectStatus in :0-不选择任何轴,1-选择x轴，2-选择y轴，3-选择z轴，4-XY平面, 5-YZ平面, 6-XZ平面.
*/
/*DLL_EXPORT*/ VOID SelectModelAxis(DWORD sceneHandle, DWORD modelHandle, UINT selectStatus);
/**设置是否显示模型坐标系
@param sceneHandle in : 场景句柄
@param modelHandle in : 模型句柄
@param modelHandle in :true-显示.
*/
/*DLL_EXPORT*/ VOID SetShowModelAxis(DWORD sceneHandle, DWORD modelHandle, BOOL bShow);
/**设置模型坐标系被选择的状态.
@param sceneHandle in : 场景句柄
@param modelHandle in : 模型句柄
@param nStatus in :0-显示基本坐标系，1－显示平移坐标系，2－显示旋转坐标系，3－显示缩放坐标系，其他值不显示.
*/
/*DLL_EXPORT*/ VOID SetModelAxisStatus(DWORD sceneHandle, DWORD modelHandle, int nStatus);
//点选轴包围盒
int GetModelAxisStatusUnderCursor(DWORD sceneHandle, DWORD modelHandle, DWORD hViewport, UINT nScreenX, UINT nScreenY);

///////////////////坐标轴相关——End/////////////////////////////////////////////////////////

protected:
	LuaEngine*	m_pLuaEngine;
	//记录N3d文件所在的目录，用于测试
	std::string	m_strN3dDir;
public:
	void setN3dDir(const char* n3dDir);
	const std::string& getN3dDir()
	{
		return m_strN3dDir;
	}
public://Lua专用区
	LuaEngine* getLuaEnginePtr() { return m_pLuaEngine;}
	int Lua_GetViewPortRect(LuaState * state);
	int Lua_GetModelNameByIndex(LuaState * state);
	int Lua_GetModelNameByHandle(LuaState * state);
	int Lua_GetModelPosition(LuaState * state);
	int Lua_SetModelPosition(LuaState * state);
	int Lua_MoveModel(LuaState * state);
	int Lua_GetModelRotation(LuaState * state);
	int Lua_SetModelRotation(LuaState * state);
	int Lua_RotateModel(LuaState * state);
	int Lua_GetModelScale(LuaState * state);
	int Lua_SetModelScale(LuaState * state);
	int Lua_ScaleModel(LuaState * state);

public://用于演示与测试临时使用
	int Lua_createFlashOverlay(LuaState* state);
	int Lua_createRenderTexture(LuaState* state);
	int Lua_createFlashRTT(LuaState* state);
	int Lua_setFlashRenderState(LuaState* state);

private:
	CMainFrameListener* m_pMainFrameListener;

};
