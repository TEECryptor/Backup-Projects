//----------------------------------------------------------------------------------
//VGS2DLL.cpp VGS2 DLL 实现文件
//描述：VGS2版本引擎动态链接库
//作者: lixin
//日期: 2006 -4 -1
//-----------------------------------------------------------------------------------
#include "VGS_CAPI.h"
#include "VGSCore.h"

#pragma   warning(disable:4786)  // 暂时屏蔽掉warning


//在C API接口中使用的全局变量.
CVGSCore	globalVgsCore;


//-----------------------------------------------------------------------------
// 方法名: InitVGS()
// 参  数: 
// 用  途: 初始化VGS引擎，必须先调用该方法，才能调用CreateScene
// 创建成功返回TUR, 否则返回FALSE
//-----------------------------------------------------------------------------
BOOL InitVGS(HWND hWnd, INT browserType/* = 0*/, BOOL bWritelog/* = FALSE*/)
{
	return globalVgsCore.InitVGS(hWnd,browserType, browserType);
}
VOID ExitVGS()
{
	globalVgsCore.ExitVGS();
}

/** 设置浏览器类型
@param type INT : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
*/
VOID SetWebBrowserType(INT type)
{
	globalVgsCore.SetWebBrowserType(type);
}

/** 获取浏览器类型
@return : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
*/
INT  GetWebBrowserType()
{
	return globalVgsCore.GetWebBrowserType();
}

/** 设置IE浏览器的子类型
@param type INT : 0 IE, 1 maxthon, 2 tencent
*/
VOID SetWebBowserSubtype(INT subType)
{
	globalVgsCore.SetWebBowserSubtype(subType);
}

/** 获取IE浏览器的子类型
@return : 0 IE, 1 maxthon, 2 tencent
*/
INT  GetWebBowserSubtype()
{
	return globalVgsCore.GetWebBowserSubtype ();
}

//---------------------------------------------------------
//设置抗拒齿的级别，
//当Level等于0，则没有抗拒齿，
//当Level等于-1，则中度抗拒齿（默认情形）
//当Level大于0，则根据不同的系统选择抗拒齿的参数
//---------------------------------------------------------
VOID SetAntiAliasLevel(INT level)
{
	globalVgsCore.SetAntiAliasLevel(level);
}

//---------------------------------------------------------
//获得抗拒齿的级别，
//当Level等于0，则没有抗拒齿，
//当Level等于-1，则中度抗拒齿（默认情形）
//当Level大于0，则根据不同的系统选择抗拒齿的参数
//---------------------------------------------------------
INT	GetAntiAliasLevel()
{
	return globalVgsCore.GetAntiAliasLevel();
}

//-----------------------------------------------------------------------------
// 方法名: CreateScene()
// 参  数: CHAR* name, HWND hWnd, RECT* rect
// 用  途: 创建VGS场景到指定窗体
//-----------------------------------------------------------------------------
DWORD CreateScene(CHAR* name)
{
	return globalVgsCore.CreateScene(name);
}

//-----------------------------------------------------------------------------
// 方法名: DeleteScene()
// 参  数: DWORD sceneHandle
// 用  途: 清除VGS场景
//-----------------------------------------------------------------------------
VOID DeleteScene(DWORD sceneHandle)
{
	globalVgsCore.DeleteScene(sceneHandle);
}

//-----------------------------------------------------------------------------
// 方法名: ResetScene()
// 参  数: DWORD sceneHandle
// 用  途: Reset VGS场景
//-----------------------------------------------------------------------------
VOID ResetScene(DWORD sceneHandle)
{
	globalVgsCore.ResetScene(sceneHandle);
}

//删除所有的场景
VOID DeleteAllScene()
{
	globalVgsCore.DeleteAllScene();
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneCount()
// 参  数: 
// 用  途: 获得场景的数量
//-----------------------------------------------------------------------------
INT	GetSceneCount()
{
	return globalVgsCore.GetSceneCount();
}

//-----------------------------------------------------------------------------
// 方法名: ResetSceneIndex()
// 参  数: 
// 用  途: 给场景重新编号, 只有在删除场景后才需要调用
//-----------------------------------------------------------------------------
VOID ResetSceneIndex()
{
	globalVgsCore.ResetSceneIndex();
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneHandleByIndex()
// 参  数: INT index
// 用  途: 通过编号获得场景
//-----------------------------------------------------------------------------
DWORD GetSceneHandleByIndex(INT index)
{
	return globalVgsCore.GetSceneHandleByIndex(index);
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneHandleByName()
// 参  数: CHAR* name
// 用  途: 通过名称获得场景
//-----------------------------------------------------------------------------
DWORD GetSceneHandleByName(CHAR* name)
{
	return globalVgsCore.GetSceneHandleByName(name);
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneNameByHandle()
// 参  数: DWORD sceneHandle
// 用  途: 通过句柄获得场景的名称
//-----------------------------------------------------------------------------
VOID GetSceneNameByHandle(DWORD sceneHandle, CHAR* name)
{
	globalVgsCore.GetSceneNameByHandle(sceneHandle,name);
}
/** 通过句柄索引号获取场景名称
@param in index 场景索引号
@param in/out name 场景名称，最长MAX_NAMESTR_LENGTH个字节 
@return 得到场景名称返回TRUE，否则FALSE
*/
BOOL GetSceneNameByIndex(INT index, CHAR* name)
{
	return globalVgsCore.GetSceneNameByIndex(index,name);
}

//-----------------------------------------------------------------------------
// 方法名: GetSceneIndexByHandle()
// 参  数: DWORD sceneHandle
// 用  途: 通过句柄获得场景的名称
//-----------------------------------------------------------------------------
INT GetSceneIndexByHandle(DWORD sceneHandle)
{
	return globalVgsCore.GetSceneIndexByHandle(sceneHandle);
}

////-----------------------------------------------------------------------------
//// 方法名: GetWndBySceneHandle()
//// 参  数: DWORD sceneHandle
//// 用  途: 通过场景的句柄获得窗体句柄
////-----------------------------------------------------------------------------
//HWND GetWndBySceneHandle(DWORD sceneHandle)
//{
//	CScene* pScene = (CScene*)sceneHandle;
//	if (pScene)
//	{
//		return pScene->m_hWnd;
//	}
//	return 0;
//}

//-----------------------------------------------------------------------------
// 方法名: GetSceneHandleByWnd()
// 参  数: HWND hwnd
// 用  途: 通过窗体句柄获得场景句柄
//-----------------------------------------------------------------------------
//DWORD GetSceneHandleByWnd(HWND hWnd)
//{
//	INT sceneCnt = g_vScene.size();
//	CScene** ppScene = g_vScene.begin();
//	for (INT i = 0; i< sceneCnt; i++)
//	{
//		CScene* pScene = *(ppScene + i);
//		if (pScene->m_hWnd == hWnd)
//			return (DWORD)pScene;
//	}
//	return 0;
//}

//----------------------------------------------------------------------------
// 方法名: GetSceneEditAxisStatus(DWORD sceneHandle)
// 参  数: DWORD sceneHandle
// 用  途: 获得场景中坐标轴选择的状态
//-----------------------------------------------------------------------------
UINT GetSceneEditAxisStatus(DWORD sceneHandle)
{
	return globalVgsCore.GetSceneEditAxisStatus(sceneHandle);
}

/////////////////////// 渲染窗口 //////////////////////////////////

/** 创建渲染窗体
@param sceneHandle in VGS场景
@param hExternalWnd in 要VGS渲染窗体所嵌入的窗体句柄
@param Name in 创建的渲染窗体名字
@return 如果创建成功，返回渲染窗体指针，否则返回0
@remarks 一个场景可以包含多个渲染窗体
*/
DWORD CreateRenderWindow(DWORD sceneHandle, HWND hExternalWnd, CHAR* Name)
{
	return globalVgsCore.CreateRenderWindow(sceneHandle,hExternalWnd,Name);
}

/**删除指定的VGS场景
@param sceneHandle in VGS场景管理器指针
@param windowHandle in 要删除的VGS窗口句柄
*/
VOID DeleteRenderWindow(DWORD sceneHandle, DWORD windowHandle)
{
	globalVgsCore.DeleteRenderWindow(sceneHandle,windowHandle);
}

/** 创建渲染窗口内容的缩略图
@param sceneHandle  in : 场景指针
@param pRenderWnd   in : 渲染窗体指针
@param sFileName    in : 要保存的缩略图文件名称，包含路径和扩展名
*/
VOID CreateRenderwindowThumb(DWORD sceneHandle, DWORD pRenderWnd, const CHAR *sFileName)
{
	globalVgsCore.CreateRenderwindowThumb(sceneHandle,pRenderWnd,sFileName);
}

//设置渲染窗体的投影模式
//DLL_EXPORT VOID SetRenderWindowProjMode(DWORD sceneHandle, DWORD swapChainHandle, INT projMode)
//{
//}
//获得渲染窗体的投影模式
//DLL_EXPORT INT GetRenderWindowProjMode(DWORD sceneHandle, DWORD swapChainHandle);

/** 设置场景的渲染窗口是否可以响应鼠标、键盘、手柄的控制
@param in sceneHandle : 场景指针
@param in RenderWndHandle : 渲染窗口指针
@param in WndHandle : 响应消息的窗体，通常是主窗体
*/
VOID CreateWndControl(DWORD WndHandle, DWORD sceneHandle, DWORD RenderWndHandle)
{
	return; // 函数不需要了
}

/** 设置场景的控制状态
@param bEnable in TRUE:场景可通过鼠标键盘等控制，FALSE：不能控制
*/
// DLL_EXPORT VOID SetWndControlStatus(BOOL bKeyboardEnable, BOOL bMouseEnable);
// DLL_EXPORT VOID SetWndControlStatus(BOOL bEnable);

/** 设置场景是否可以键盘控制
*/
VOID SetWndControlKeyboardStatus(BOOL bEnable)
{
	globalVgsCore.SetWndControlKeyboardStatus(bEnable);
}

/** 设置场景是否可以鼠标控制
*/
VOID SetWndControlMouseStatus(BOOL bEnable)
{
	globalVgsCore.SetWndControlMouseStatus(bEnable);
}

/** 设置场景的控制状态
@param out: 场景是否可通过鼠标键盘等控制，FALSE：不能控制
*/
// DLL_EXPORT VOID GetWndControlStatus(BOOL &bKeyboardEnable, BOOL &bMouseEnable);
// DLL_EXPORT BOOL GetWndControlStatus();

/** 获取场景是否可以键盘控制
*/
BOOL GetWndControlKeyboardStatus()
{
	return globalVgsCore.GetWndControlKeyboardStatus();
}

/** 获取场景是否可以鼠标控制
*/
BOOL GetWndControlMouseStatus()
{
	return globalVgsCore.GetWndControlMouseStatus();
}

////////////////// 资源路径 //////////////////////
/** 添加资源文件搜索路径
*/ 
VOID AddResourcePath(const CHAR * sFilePath)
{
	globalVgsCore.AddResourcePath(sFilePath);
}

/** 删除资源文件搜索路径
*/ 
VOID RemoveResourcePath(const CHAR * sFilePath)
{
	globalVgsCore.RemoveResourcePath(sFilePath);
}

/** 删除所有资源文件搜索路径
*/ 
VOID ClearResourcePaths()
{
	globalVgsCore.ClearResourcePaths();
}

//-------------------------------------------------------------------------------------------
//***场景渲染相关的方法
//-------------------------------------------------------------------------------------------
VOID RenderAll()
{
	globalVgsCore.RenderAll();
}

/**渲染指定的场景，可能包含多个渲染窗体
@param in sceneHandle VGS场景管理器指针
*/
//-----------------------------------------------------------------------------
// 方法名: RenderScene()
// 参  数: 
// 用  途: 渲染整个场景
//-----------------------------------------------------------------------------
VOID RenderScene(DWORD sceneHandle)
{
	globalVgsCore.RenderScene(sceneHandle);
}

//更新后缓冲
BOOL UpdateBackBuffer(DWORD hRenderWnd)
{
	return globalVgsCore.UpdateBackBuffer(hRenderWnd);
}

/*//渲染材质球场景
VOID RenderMtrlSphereScene(DWORD sceneHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;

	vgsScene->RenderMtrlSphere();
}*/


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
DWORD AddViewport(DWORD windowHandle, DWORD camHandle, INT level, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
	return globalVgsCore.AddViewport(windowHandle,camHandle,level,left,top,width,height);
}
/** 删除渲染窗口的视口
@param windowHandle in 渲染窗体指针
@param viewportHandle in 视口指针
*/
VOID DeleteViewport(DWORD windowHandle, DWORD vpHandle)
{
	globalVgsCore.DeleteViewport(windowHandle,vpHandle);
}

/** 设置视口使用的相机
@param viewportHandle in 视口指针
@param camHandle in 相机指针
*/
VOID SetViewportCamera(DWORD viewportHandle, DWORD camHandle)
{
	globalVgsCore.SetViewportCamera(viewportHandle,camHandle);
}

/** 获取视口当前使用的相机
@param viewportHandle 视口指针
@return 相机指针
*/
DLL_EXPORT DWORD GetViewportCamera(DWORD viewportHandle)
{	
	return globalVgsCore.GetViewportCamera(viewportHandle);
}

/**设置视口位置和大小 
@param ViewPort 视口指针
@param left,top,width,height取值均为[0,1],表示在RenderTarget中的位置
*/
VOID SetViewportRect(DWORD ViewPort, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
	globalVgsCore.SetViewportRect(ViewPort,left,top,width,height);
}

/** 设置视口使用相机的宽高比
*/
VOID UpdateViewportAspectRatio(DWORD viewportHandle)
{
	globalVgsCore.UpdateViewportAspectRatio(viewportHandle);
}

/**获取视口大小 
@param ViewPort 视口指针
@param left,top,width,height取值均为[0,1],表示在RenderTarget中的位置
*/
VOID GetViewportRect(DWORD viewportHandle, float &left, float &top, float &width, float &height)
{
	globalVgsCore.GetViewportRect(viewportHandle,left,top,width,height);
}

/** 得到视口的真实宽度
@return : 如果视口不存在，返回-1
*/
INT GetViewportActualWidth(DWORD viewportHandle)
{
	return globalVgsCore.GetViewportActualWidth(viewportHandle);
}

/** 得到视口的真实高度
@return : 如果视口不存在，返回-1
*/
INT GetViewportActualHeight(DWORD viewportHandle)
{
	return globalVgsCore.GetViewportActualHeight(viewportHandle);
}

/**设置当前场景是否自动渲染
@param sceneHandle 场景指针
@param renderScene 是否应该渲染，如果设为<b>不</b>自动渲染，则只有调用RenderTarget的Update方法时窗体才能被渲染。
*/
VOID SetSceneRenderStatus(DWORD sceneHandle, BOOL renderScene)
{	
	globalVgsCore.SetSceneRenderStatus(sceneHandle,renderScene);
}
/**获取当前场景是否自动渲染
@param sceneHandle 场景指针
*/
BOOL GetSceneRenderStatus(DWORD sceneHandle)
{
	return globalVgsCore.GetSceneRenderStatus(sceneHandle);
}

/**设置渲染窗体是否渲染
@param sceneHandle 场景管理器指针
@param RenderTargetHandle 渲染窗体指针
@param bAutoRender 是否自动渲染，如果设为<b>不</b>自动渲染，则只有调用RenderTarget的Update方法时窗体才能被渲染。
*/
VOID SetRenderTargetStatus(DWORD sceneHandle, DWORD RenderTargetHandle, BOOL bAutoRender)
{
	globalVgsCore.SetRenderTargetStatus(sceneHandle,RenderTargetHandle,bAutoRender);
}

/**获取渲染窗体是否渲染
@param sceneHandle 场景管理器指针
@param RenderTargetHandle 渲染窗体指针
*/
BOOL GetRenderTargetStatus(DWORD sceneHandle, DWORD RenderTargetHandle)
{
	return globalVgsCore.GetRenderTargetStatus(sceneHandle,RenderTargetHandle);
}

/**设置渲染窗体的填充模式, 2为线框，3为实体
@param sceneHandle 场景管理器指针
@param CameraHandle 视口指针
@param fillMode 渲染模式，1 : Only points are rendered， 2 : Wireframe models are rendered. 3: Solid polygons are rendered.
*/
// VOID SetSwapRenderWndFillMode(DWORD sceneHandle, DWORD CameraHandle, INT fillMode)
VOID SetViewportFillMode(DWORD sceneHandle, DWORD viewport, INT fillMode)
{
	globalVgsCore.SetViewportFillMode(sceneHandle,viewport,fillMode);
}

/**获得渲染窗体的填充模式, 1 : Only points are rendered， 2 : Wireframe models are rendered. 3: Solid polygons are rendered.
@param sceneHandle 场景管理器指针
@param viewport 视口指针
*/
// INT GetSwapRenderWndFillMode(DWORD sceneHandle, DWORD CameraHandle)
INT GetViewportFillMode(DWORD sceneHandle, DWORD viewport)
{
	return globalVgsCore.GetViewportFillMode(sceneHandle,viewport);
}


/** 强制渲染当前帧
@param sceneHandle 场景指针
*/
VOID RenderCurrentFrame(DWORD sceneHandle)
{
	globalVgsCore.RenderCurrentFrame(sceneHandle);
}

/**设置视口是否渲染二维界面
@param sceneHandle in 场景管理器指针
@param vpHandle in : 视口指针
*/
VOID SetViewportRenderGUI(DWORD sceneHandle, DWORD vpHandle, BOOL renderGUI)
{
	globalVgsCore.SetViewportRenderGUI(sceneHandle,vpHandle,renderGUI);
}

/**获取视口是否渲染二维界面
@param sceneHandle in ：场景管理器指针
@param vpHandle in : 视口指针
@renderGUI 是否渲染二维元素
*/
BOOL IsViewportRenderGUI(DWORD sceneHandle, DWORD vpHandle)
{
	return globalVgsCore.IsViewportRenderGUI(sceneHandle,vpHandle);
}

/**设置场景的背景
@param sceneHandle 场景管理器指针
@param ViewportHandle 视口的指针，当前版本必须设置NULL，将设置场景中的所有视口
@param color 颜色
*/
VOID SetSceneBackColor(DWORD sceneHandle, DWORD ViewportHandle, DWORD color)
{
	globalVgsCore.SetSceneBackColor(sceneHandle,ViewportHandle,color);
}

/**获得场景的背景
@param sceneHandle 场景管理器指针
@param ViewportHandle 视口的指针,当前版本必须为NULL
*/
DLL_EXPORT DWORD GetSceneBackColor(DWORD sceneHandle, DWORD ViewportHandle)
{
	return globalVgsCore.GetSceneBackColor(sceneHandle,ViewportHandle);
}

/*
//设置场景的VGS窗体初始尺寸
VOID SetSceneRect(DWORD sceneHandle, RECT* pRect)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pScene->m_oriWidth  = pRect->right - pRect->left;
		pScene->m_oriHeight = pRect->bottom - pRect->top;
	}
}

//获得场景的VGS窗体初始尺寸
VOID GetSceneRect(DWORD sceneHandle, RECT* pRect)
{
	CScene* pScene = (CScene*)sceneHandle;
	if (pScene)
	{
		pRect->left   = 0;
		pRect->top    = 0;
		pRect->right  = pScene->m_oriWidth;
		pRect->bottom = pScene->m_oriHeight;
	}
}
*/

//计算三维坐标的屏幕坐标
VOID CalcScreenCoords(DWORD sceneHandle, DWORD viewportHandle, VECTOR3* pVec3, INT* X, INT* Y)
{
	globalVgsCore.CalcScreenCoords(sceneHandle,viewportHandle,pVec3,X,Y);
}

/*
//缩放轴侧窗口
VOID ZoomOrthoView(DWORD sceneHandle, DWORD swapChainHandle, FLOAT zoomVal)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return;
	
	CSwapChain* pSwapChain  = (CSwapChain*)swapChainHandle;
	if (pSwapChain == NULL) return;
	
	EnterCriticalSection(&m_render_cs);
	
	pSwapChain->m_rect.left = 0;
	pSwapChain->m_rect.top = 0;
	pSwapChain->m_rect.right = pSwapChain->m_rect.right / zoomVal;
	pSwapChain->m_rect.bottom = pSwapChain->m_rect.bottom / zoomVal;

	if (pSwapChain->m_rect.right < 10)
		pSwapChain->m_rect.right = 10;
	if (pSwapChain->m_rect.right > 1000000)
		pSwapChain->m_rect.right = 1000000;

	pSwapChain->m_rect.bottom = pSwapChain->m_rect.right / pSwapChain->m_ratio; 

	LeaveCriticalSection(&m_render_cs);
}
*/
/*
//当前窗体是否为轴侧窗口
BOOL IsOrthoView(DWORD sceneHandle, DWORD swapChainHandle)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene == NULL) return FALSE;
	
	CSwapChain* pSwapChain  = (CSwapChain*)swapChainHandle;
	if (pSwapChain == NULL) return FALSE;

	if (pSwapChain->GetViewport(0)->GetCamera()->m_Type == VTC_ORTHO)
		return TRUE;
	else
		return FALSE;
}
*/

//将当前窗口中的三维场景设置到合适的大小
// VOID MaximizeSceneToView(DWORD sceneHandle, DWORD vpHandel, FLOAT fillRatio)
// {
// 	CScene* vgsScene = (CScene*)sceneHandle;
// 	if (vgsScene == NULL) return;
// 	
// 	CSwapChain* pSwapChain  = (CSwapChain*)swapChainHandle;
// 	if (pSwapChain == NULL) return;
// 	
// 	EnterCriticalSection(&m_render_cs);
// 
// 	FLOAT MinX = 1000000000;
// 	FLOAT MinY = 1000000000;
// 
// 	FLOAT MaxX = -1000000000;
// 	FLOAT MaxY = -1000000000;
// 
// 	if (pSwapChain->m_projMode)  //轴侧模式
// 	{
// 		INT modelCnt = vgsScene->model.size();
// 		CModel** ppModel = vgsScene->model.begin();
// 		for (INT i = 0; i< modelCnt; i++)
// 		{
// 			CModel* pModel = *(ppModel + i);
// 
// 			//最小值
// 			FLOAT x1 = pModel->boundingSphere.Center.x - 
// 				pModel->boundingSphere.Radius;
// 			FLOAT y1 = pModel->boundingSphere.Center.y - 
// 				pModel->boundingSphere.Radius;
// 			FLOAT z1 = pModel->boundingSphere.Center.z - 
// 				pModel->boundingSphere.Radius;
// 
// 			D3DXVECTOR3 vMin = D3DXVECTOR3(x1, y1, z1);
// 			INT sx1, sy1;
// 			vgsScene->CalcScreenCoords(pSwapChain, &vMin, &sx1, &sy1);
// 
// 			if (sx1 < MinX) MinX = sx1;
// 			if (sy1 < MinY) MinY = sy1;
// 			if (sx1 > MaxX) MaxX = sx1;
// 			if (sy1 > MaxY) MaxY = sy1;
// 				
// 			//最大值
// 			FLOAT x2 = pModel->boundingSphere.Center.x + 
// 				pModel->boundingSphere.Radius;
// 			FLOAT y2 = pModel->boundingSphere.Center.y + 
// 				pModel->boundingSphere.Radius;
// 			FLOAT z2 = pModel->boundingSphere.Center.z + 
// 				pModel->boundingSphere.Radius;
// 
// 			D3DXVECTOR3 vMax = D3DXVECTOR3(x2, y2, z2);
// 			INT sx2, sy2;
// 			vgsScene->CalcScreenCoords(pSwapChain, &vMax, &sx2, &sy2);
// 
// 			if (sx2 < MinX) MinX = sx2;
// 			if (sy2 < MinY) MinY = sy2;
// 			if (sx2 > MaxX) MaxX = sx2;
// 			if (sy2 > MaxY) MaxY = sy2;
// 		}
// 		
// 		INT width = MaxX - MinX;
// 		INT height = MaxY - MinY;
// 		
// 		float widthRatio = width * 1.0f / (pSwapChain->m_rect.right - pSwapChain->m_rect.left);
// 		float heightRatio = height * 1.0f / (pSwapChain->m_rect.bottom - pSwapChain->m_rect.top);
// 		
// 		float scRatio = 1.0f/fillRatio;
// 
// 		RECT rect = {0, 0, 0, 0};
// 		if (widthRatio > heightRatio)
// 		{
// 			rect.bottom = scRatio * (pSwapChain->m_rect.bottom - pSwapChain->m_rect.top) * widthRatio;
// 			rect.right = rect.bottom * pSwapChain->m_ratio;
// 			pSwapChain->m_rect = rect;
// 		}
// 		else
// 		{
// 			rect.right = scRatio * (pSwapChain->m_rect.right - pSwapChain->m_rect.left) * heightRatio;
// 			rect.bottom = rect.right / pSwapChain->m_ratio;
// 			pSwapChain->m_rect = rect;
// 		}
// 	}
// 	
// 	LeaveCriticalSection(&m_render_cs);
// }


//平移轴侧窗口
VOID PanOrthoView(DWORD sceneHandle, DWORD swapChainHandle, INT X, INT Y, INT offsetX, INT offsetY)
{
	globalVgsCore.PanOrthoView(sceneHandle, swapChainHandle, X,Y, offsetX, offsetY);
}

//-------------------------------------------------------------------------------------------
///文件相关的方法
//-------------------------------------------------------------------------------------------
/** 导入S3D文件
@param sceneHandle		: 场景管理器句柄
@param strS3DFileName	: s3D文件的完整路径
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 成功返回S3D加载器的句柄，否则返回0
@remarks 通过返回的S3D加载器句柄，还可以获取跟本S3D相关的一些信息。切记，不用时要及时调用DeleteS3DLoader()方法删除加载器
*/
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
DWORD ImportS3DFile(DWORD sceneHandle, const CHAR* strS3DFileName, HWND hWnd)
{
	return globalVgsCore.ImportS3DFile(sceneHandle,strS3DFileName,hWnd);
}

/** 删除S3D加载对象
*/
VOID DeleteS3DLoader(DWORD sceneHandle, DWORD S3DLoaderHandle)
{
	globalVgsCore.DeleteS3DLoader(sceneHandle,S3DLoaderHandle);
}

/** 指定的贴图文件是否被S3D使用
@param S3DLoaderHandle in  : S3D加载器指针，由ImportS3DFile()返回
@param ImageFileName   in  : 图片文件名称，不包括路径
*/
BOOL IsTextureImageUsedbyS3D(DWORD S3DLoaderHandle, const CHAR* ImageFileName)
{
	return globalVgsCore.IsTextureImageUsedbyS3D(S3DLoaderHandle,ImageFileName);
}

/** 得到S3D中使用的贴图的数量
@param loaderHandle in : S3D加载器指针，由ImportS3DFile()返回
*/
INT GetS3DTextureImageCnt(DWORD S3DLoaderHandle)
{
	return globalVgsCore.GetS3DTextureImageCnt(S3DLoaderHandle);
}

/** 得到S3D中使用的第index个贴图文件名称，包含路径
@param loaderHandle in  : S3D加载器指针，由ImportS3DFile()返回
@param texIndex     in  : 索引
@param strTexName   out : 贴图文件名称
*/
BOOL GetS3DTextureImageFile(DWORD S3DLoaderHandle, INT texIndex, CHAR* strTexName)
{
	return globalVgsCore.GetS3DTextureImageFile(S3DLoaderHandle,texIndex,strTexName);
}

/** 保存场景到指定的V3D文件
@param in sceneHandle    ：VGS场景管理器指针
@param in strV3DFileName ：要保存的V3D文件名
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
DWORD SaveSceneAsV3D(DWORD sceneHandle, CHAR* strV3DFileName, HWND hWnd)
{
	return globalVgsCore.SaveSceneAsV3D(sceneHandle,strV3DFileName,hWnd);
}

/**打开V3D文件, 自动识别不同版本的V3D
@param in sceneHandle VGS场景管理器指针
@param in strV3DFileName 要打开的V3D文件名
@param in OT 打开V3D的方法，例如替换方式还是合并方式，目前仅支持替换方式
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 返回结果，0表示成功，其他值表示失败。可通过GetErrorStr可以得到原因描述
*/
DWORD OpenV3DFile(DWORD sceneHandle, const CHAR* strV3DFileName, V3D_OPENTYPE OT, HWND hWnd)
{
	return globalVgsCore.OpenV3DFile(sceneHandle,strV3DFileName,OT,hWnd);
}

/**保存当前场景为N3D格式文件
@param in sceneHandle ：要保存的场景
@param in strN3DFileName ： N3D文件名，包含目录
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 返回结果，0表示成功，其他值表示失败。可通过GetErrorStr可以得到原因描述
*/
DWORD SaveSceneAsN3D(DWORD sceneHandle, const CHAR* strN3DFileName, HWND hWnd)
{
	return globalVgsCore.SaveSceneAsN3D(sceneHandle,strN3DFileName,hWnd);
}

/** 打开N3D场景文件, 自动识别不同版本的N3D
@param in sceneHandle ：要保存的场景
@param in strN3DFileName ： N3D文件名，包含目录
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
// DLL_EXPORT DWORD ImportN3DFile(DWORD sceneHandle, const CHAR* strN3DFileName);

/**保存当前场景为E3D格式文件
@param in sceneHandle    ：要保存的场景
@param in strE3DFileName ：E3D文件名，包含目录
@param in bLinkRes       : 所需的资源是以外部链接方式还是打包到文件内部。bLinkRes为true，资源将以外部链接的方式，不打包到E3D内部
@param in bPackData      : 是否压缩文件，TRUE：压缩，FALSE：不压缩
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return 返回结果，0表示成功，其他值表示失败。可通过GetErrorStr可以得到原因描述
*/
DWORD SaveSceneAsE3D(DWORD sceneHandle, const CHAR* strE3DFileName, BOOL bLinkRes, BOOL bPackData, HWND hWnd)
{
	return globalVgsCore.SaveSceneAsE3D(sceneHandle,strE3DFileName,bLinkRes,bPackData,hWnd);
}

/** 读入N3D文件
@param strE3DFileName in : 要读入的文件名称
@param hWnd           in : 传入的窗体句柄，用于接收进度信息
@return错误码。0表示成功，其他值表示失败。通过GetErrorStr可以得到详细描述
*/
DWORD LoadE3DFile(DWORD sceneHandle, const CHAR* strE3DFileName, HWND hWnd)
{
	return globalVgsCore.LoadE3DFile(sceneHandle,strE3DFileName,hWnd);
}
#endif

/** 从buffer中读取N3D场景信息
@param in sceneHandle    ：要加载数据的场景
@param in version        : 版本号
@param in pBuffer        ：N3D数据buffer
@param hWnd           in ：传入的窗体句柄，用于接收进度信息
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
#if(defined(_VGS_LIB)||defined(_VGS_ATL))
DWORD LoadSceneFromBuffer(DWORD sceneHandle, DWORD version, const BYTE* pBuffer, HWND hWnd)
{
	return globalVgsCore.LoadSceneFromBuffer(sceneHandle,version,pBuffer,hWnd);
}

/** 从buffer中读取图像信息
@param in sceneHandle    ：要加载图像的场景
@param in version        : 版本号
@param in pBuffer        ：N3D数据buffer
@param hWnd           in ：传入的窗体句柄，用于接收进度信息
@return 返回结果，可通过GetErrorStr可以得到原因描述
*/
DWORD LoadImageDataFromBuffer(DWORD sceneHandle, DWORD version, const BYTE* pBuffer, HWND hWnd)
{
	return globalVgsCore.LoadImageDataFromBuffer(sceneHandle,version,pBuffer,hWnd);
}
#endif
//-------------------------------------------------------------------------------------------
///模型相关的方法
//-------------------------------------------------------------------------------------------

/**得到场景中模型的数量
@param in sceneHandle 场景管理器指针
@return 场景中模型的数量
*/
INT GetModelCount(DWORD sceneHandle)
{
	return globalVgsCore.GetModelCount(sceneHandle);
}

/*删除场景中的模型
@param sceneHandle 场景管理器指针
@param modelHandle 模型(entity)指针
@return 成功删除返回TRUE，否则FALSE
*/
BOOL DeleteModelbyHandle(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore.DeleteModelbyHandle(sceneHandle,modelHandle);
}


/** 删除场景中的模型
@param sceneHandle 场景管理器指针
@param strModelName 模型(entity)名称
@return 成功删除返回TRUE，否则FALSE
*/
BOOL DeleteModel(DWORD sceneHandle, const CHAR* strModelName)
{	
	return globalVgsCore.DeleteModel(sceneHandle,strModelName);	
}

//根据模型的名称得到模型的句柄
DWORD GetModelHandleByName(DWORD sceneHandle, const CHAR* strModelName)
{
	return globalVgsCore.GetModelHandleByName(sceneHandle,strModelName);
}
//根据模型的编号得到模型的句柄
DWORD GetModelHandleByIndex(DWORD sceneHandle, INT index)
{
	return globalVgsCore.GetModelHandleByIndex(sceneHandle,index);
}

/**根据模型的句柄得到模型的名称
@param in sceneHandle 场景指针
@param in modelHandle 模型的句柄
@param in & out strModelName 模型名称
@return 得到名字返回TRUE，否则FALSE
*/
BOOL GetModelNameByHandle(DWORD sceneHandle, DWORD modelHandle, CHAR* strModelName)
{
	return globalVgsCore.GetModelNameByHandle(sceneHandle,modelHandle,strModelName);
}
/**根据模型的索引号得到模型的名称
@param in sceneHandle 场景指针
@param in index 模型的索引号
@param in & out strModelName 模型名称
@return 得到名字返回TRUE，否则FALSE
*/
BOOL GetModelNameByIndex(DWORD sceneHandle, INT index, CHAR* strModelName)
{
	return globalVgsCore.GetModelNameByIndex(sceneHandle,index,strModelName);
}


//设置模型的名称
VOID SetModelName(DWORD sceneHandle, DWORD modelHandle, CHAR* strModelName)
{
	globalVgsCore.SetModelName(sceneHandle,modelHandle,strModelName);
}

/** 移动模型的位置
@param in sceneHandle	: 场景句柄
@param in camHandle     : 相机句柄
@param in pVec          ：移动的相对向量
@param in coordEnum     ：移动的参考坐标系
			               0 - 在世界坐标系平移
						   1 - 在自身坐标系平移
*/
VOID MoveModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec, UINT flag)
{
	globalVgsCore.MoveModel(sceneHandle,modelHandle,pVec,flag);
}

/** 得到模型的位置,即包围盒中心的世界坐标
@param out pVec : 模型包围盒中心的世界坐标位置
*/
VOID GetModelPosition(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec)
{
	globalVgsCore.GetModelPosition(sceneHandle,modelHandle,pVec);
}
//设置模型的位置
VOID SetModelPosition(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec)
{
	globalVgsCore.SetModelPosition(sceneHandle,modelHandle,pVec);
}

/** 获取模型的包围盒
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param out vMinCorner : 包围盒坐标最小的角的世界坐标
@param out vMinCorner : 包围盒坐标最大的角的世界坐标
@return 包围盒有效，返回TRUE, 否则FALSE
*/ 
BOOL GetModelBoundsBox(DWORD sceneHandle, DWORD modelHandle, VECTOR3 &vMinCorner, VECTOR3 &vMaxCorner)
{	
	return globalVgsCore.GetModelBoundsBox(sceneHandle,modelHandle,vMinCorner,vMaxCorner);
}

/** 获取模型的包围球
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param out vCenter    : 包围球中心点的世界坐标
@param out Radius     : 包围球半径
@return 包围盒有效，返回TRUE, 否则FALSE
*/ 
BOOL GetModelBoundsSphere(DWORD sceneHandle, DWORD modelHandle, VECTOR3 &vCenter, FLOAT &Radius)
{	
	return globalVgsCore.GetModelBoundsSphere(sceneHandle,modelHandle,vCenter,Radius);
}

/** 设置模型是否显示包围盒
@param in sceneHandle : 场景管理器指针
@param in modelHandle : 模型指针
@param in bvisible    : 是否显示包围盒
*/
VOID SetModelBoundsBoxVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL bVisible)
{	
	globalVgsCore.SetModelBoundsBoxVisibility(sceneHandle,modelHandle,bVisible);
}

/** 旋转模型
@param in pVec : 在各个方向上旋转的角度增量,弧度
@param in coordEnum :
	//0 - 绕穿过自身中心，与世界坐标系平行的坐标系旋转
	//1 - 绕自身坐标系旋转模型
	//2 - 绕父坐标系旋转
*/
VOID RotateModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pVec, INT coordEnum)
{
	globalVgsCore.RotateModel(sceneHandle,modelHandle,pVec,coordEnum);
}

//模型绕指定的轴旋转
//DLL_EXPORT VOID RotateModelByAxis(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pVec, FLOAT fAngle);

/** 得到模型的转角
@param out pRot ：模型当前相对于原始状态在三个方向上的转角，单位是弧度
*/
BOOL GetModelRotation(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pRot)
{
	return globalVgsCore.GetModelRotation(sceneHandle,modelHandle,pRot);
}

/** 设置模型的转角
@param out pRot ：模型当前相对于原始状态在三个方向上的转角，单位是弧度
*/
VOID SetModelRotation(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pRot)
{
	globalVgsCore.SetModelRotation(sceneHandle,modelHandle,pRot);
}

/** 以自身中心为中心，在当前基础上缩放模型
@param in pScale : 各个方向上的缩放值
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型将会在现在的基础上，在X方向不缩放，y方向放大一倍，z方向缩小一倍
*/
VOID ScaleModel(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale, INT flag /*= 0*//*世界坐标系*/)
{
	globalVgsCore.ScaleModel(sceneHandle,modelHandle,pScale,flag /*= 0*//*世界坐标系*/);
}

//-----------------------------------------------------------------------------
// 方法名: ScaleModelByPoint()
// 参  数: DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale
// 用  途: 以固定点为中心缩放模型
//-----------------------------------------------------------------------------
VOID ScaleModelByPoint(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale)
{
	globalVgsCore.ScaleModelByPoint(sceneHandle,modelHandle,pPos,pScale);
}

/** 以自身中心为中心，在模型原始尺寸基础上缩放模型
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型相对于原始尺寸，将在X方向不缩放，y方向放大一倍，z方向缩小一倍
*/
VOID SetModelScale(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale)
{
	globalVgsCore.SetModelScale(sceneHandle,modelHandle,pScale);
}

/** 得到模型的缩放
@param out pScale : 模型绝对缩放值，相对于原始大小，不是相对于上一次的。
@reamarks: 例如：pScale等于(1, 2, 0.5)：就表示模型现在相对于原始尺寸，在X方向没有缩放，y方向放大了一倍，z方向缩小了一倍
*/
BOOL GetModelScale(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pScale)
{
	return globalVgsCore.GetModelScale(sceneHandle,modelHandle,pScale);
}
//以固定点为中心缩放模型
// DLL_EXPORT VOID ScaleModelByPoint(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale);

//Add By Baojiahui 修正移动相机以及移动物体时位置不准确的方法----------------------------------------------------------------------------------------------------------
/** 得到垂直于屏幕坐标下对应的3d场景中某个平面上的交点坐标
@param in sceneHandle      :场景管理器指针
@param in vpHandle         :视口指针
@param in ScreenX, ScreenY :相对于RenderWindow区域的屏幕坐标
@param in planeNormal*      :特定平面的法线向量指针
@param in planePoint*       :过特定平面的点坐标指针
@param out vec3WorldPoint*  :返回的平面交点坐标指针
*/
bool GetWorldPointUnderLoc(DWORD sceneHandle,DWORD vpHandle,UINT ScreenX, UINT ScreenY,VECTOR3* planeNormal,VECTOR3* planePoint,VECTOR3* vec3WorldPoint)
{
	globalVgsCore.GetWorldPointUnderLoc(sceneHandle,vpHandle,ScreenX,ScreenY,planeNormal, planePoint,vec3WorldPoint);
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

/** 得到屏幕坐标下对应的3d场景中的模型，如有多个模型，得到最近的一个。隐藏或被冻结的model不会被选中
@param in sceneHandle      : 场景管理器指针
@param in vpHandle         : 视口指针
@param in ScreenX, ScreenY : 相对于RenderWindow区域的屏幕坐标
@param in modeltype        : 0 : 仅选中模型， 1：选中所有，包括灯光和相机
@return : 模型指针
*/
DWORD GetModelUnderLoc(DWORD sceneHandle, DWORD vpHandle, UINT ScreenX, UINT ScreenY, int modeltype/* = 0*/)
{
	return globalVgsCore.GetModelUnderLoc(sceneHandle,vpHandle,ScreenX,ScreenY,modeltype/* = 0*/);
}

/** 获取模型属性信息
*/
BOOL GetModelInfo(DWORD sceneHandle, DWORD modelHandle, VGS_ModelInfo &Info)
{
	return globalVgsCore.GetModelInfo(sceneHandle,modelHandle, Info);
}

/** 设置模型属性信息
*/
VOID SetModelInfo(DWORD sceneHandle, DWORD modelHandle, const VGS_ModelInfo &Info)
{
	globalVgsCore.SetModelInfo( sceneHandle,  modelHandle,   Info);
}

/** 设置模型的鼠标事件函数名称
@param scenHandle   in : 场景指针
@param modelHandle  in : 模型指针
@param type         in  : 事件类型
@param sEventName   in : 函数名称
*/
VOID SetModelMouseEvent(DWORD sceneHandle, DWORD modelHandle, VGS_MOUSEEVENT_TYPE type, const CHAR* sEventName)
{
	globalVgsCore.SetModelMouseEvent(sceneHandle,modelHandle,type,sEventName);
}

/** 获取模型的鼠标事件函数名称
@param scenHandle   in  : 场景指针
@param modelHandle  in  : 模型指针
@param type         in  : 事件类型
@param sEventName   out : 函数名称
@return 如果模型已经制定了对应的事件函数，返回TRUE，否则FALSE
*/
BOOL GetModelMouseEvent(DWORD sceneHandle, DWORD modelHandle, VGS_MOUSEEVENT_TYPE type, CHAR* sEventName)
{
	return globalVgsCore.GetModelMouseEvent(sceneHandle,modelHandle,type,sEventName);
}

/** 把相机设置为billboard相机
*/
VOID SetBillboardCamera(DWORD sceneHandle, DWORD camHanlde)
{
	globalVgsCore.SetBillboardCamera(sceneHandle, camHanlde);
}

/** 获取billboard相机
*/
DWORD GetBillboardCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetBillboardCamera(sceneHandle);
}

/** 设置模型正对Billboard状态
@param axis in : 设置模型的哪个坐标轴（自身）指向相机
*/
VOID SetModelBillboardStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status, VGS_AXIS_TYPE UpAxis/* = VGS_LAXIS_NEGATIVEZ*/)
{
	globalVgsCore.SetModelBillboardStatus(sceneHandle, modelHandle,status,UpAxis/* = VGS_LAXIS_NEGATIVEZ*/);
}

/** 判断模型是否为Billboard模型
@param axis in : 模型的哪个坐标轴（自身）指向相机
*/
BOOL GetModelBillboardStatus(DWORD sceneHandle, DWORD modelHandle, VGS_AXIS_TYPE &axis)
{
	return globalVgsCore.GetModelBillboardStatus(sceneHandle,modelHandle,axis);
}

//-----------------------------------------------------------------------------
// 方法名: SetBillboardUp()
// 参  数: DWORD modelHandle, VECTOR3* pModelRay, VECTOR3* pAt, VECTOR3* pUp
//modelHandle ->模型的指针
//pUp ->模型的上方是什么方向，譬如Tree，要求Z方向正对目标点，但up方向始终为(0,1,0), 否则就倒了
// 用  途: 设置模型正对Billboard
//-----------------------------------------------------------------------------
VOID SetBillboardUp(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pUp)
{
	globalVgsCore.SetBillboardUp(sceneHandle,modelHandle,pUp);
}

//设置模型正对Billboard的Up向量
//DLL_EXPORT VOID SetBillboardUp(DWORD sceneHandle, DWORD modelHandle, VECTOR3* pUp);

//设置场景碰撞的有效状态
/*VOID SetSceneCollisionStatus(DWORD sceneHandle, BOOL status)
{
	CScene* vgsScene = (CScene*)sceneHandle;
	if (vgsScene)
	{
		vgsScene->m_bIsDetectCollision = status;
	}
}
*/
////设置碰撞发生的有效距离, 默认为1.0
//DLL_EXPORT VOID SetSceneCollisionSta(DWORD sceneHandle, FLOAT distance);

/** 设置模型为水平碰撞模型
*/
VOID SetModelCollisionStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{	
	globalVgsCore.SetModelCollisionStatus(sceneHandle,modelHandle,status);
}

/** 判断模型是否为水平碰撞模型
*/
BOOL GetModelCollisionStatus(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore.GetModelCollisionStatus(sceneHandle,modelHandle);
}

/** 设置模型为地面模型
*/
VOID SetModelAsGround(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{
	globalVgsCore.SetModelAsGround(sceneHandle,modelHandle,status);
}

/** 判断模型是否为地面模型
*/
BOOL GetModelAsGround(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore.GetModelAsGround(sceneHandle,modelHandle);
}

////移动模型Diffuse贴图的UV
//DLL_EXPORT VOID MoveModelUV(DWORD sceneHandle, DWORD modelHandle, FLOAT x, FLOAT y);
//
////设置模型动态UV的状态
//DLL_EXPORT VOID SetModelDynamicUVStatus(DWORD sceneHandle, DWORD modelHandle, BOOL status);
//
////获得模型动态UV的状态
//DLL_EXPORT BOOL GetModelDynamicUVStatus(DWORD sceneHandle, DWORD modelHandle);
//
////设置模型的动态UV
//DLL_EXPORT VOID SetModelDynamicUV(DWORD sceneHandle, DWORD modelHandle, FLOAT u, FLOAT v);
////获得模型的动态UV
//DLL_EXPORT VOID GetModelDynamicUV(DWORD sceneHandle, DWORD modelHandle, FLOAT* u, FLOAT* v);


/** 设置模型是否可见
*/
VOID SetModelVisibility(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{
	globalVgsCore.SetModelVisibility(sceneHandle,modelHandle,status);
}

/** 获取模型是否可见
*/
BOOL GetModelVisibility(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore. GetModelVisibility(sceneHandle,modelHandle);
}

/** 设置模型在播放状态下是否可以点击
*/
VOID SetModelClickable(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{
	globalVgsCore. SetModelClickable(sceneHandle,modelHandle, status);
}

/** 获取模型在播放状态下是否可以点击
*/
BOOL GetModelClickabel(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore. GetModelClickabel(sceneHandle,modelHandle);
}

//设置模型为SkyBox模型
VOID SetSkyBox(DWORD sceneHandle, DWORD modelHandle, BOOL status)
{
	globalVgsCore. SetSkyBox(sceneHandle,modelHandle,status);
}

//判断模型是否为SkyBox
BOOL IsSkyBox(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore.IsSkyBox(sceneHandle,modelHandle);
}

/** 设置模型的颜色
*/
VOID SetModelColor(DWORD sceneHandle, DWORD modelHandle, BYTE red, BYTE green, BYTE blue, BYTE alpha)
{
	globalVgsCore. SetModelColor(sceneHandle,modelHandle,red,green,blue,alpha);
}

/** 设置模型透明图
*/
VOID SetModelOpacity(DWORD sceneHandle, DWORD modelHandle, INT opacity)
{
	globalVgsCore. SetModelOpacity(sceneHandle, modelHandle,opacity);
}

/** 模型是否包含关键帧动画
*/
BOOL IsModelHasKeyframeAnimation(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore.IsModelHasKeyframeAnimation(sceneHandle,modelHandle);
}

/** 模型是否包含骨骼动画
*/
BOOL IsModelHasSkeletonAnimation(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore.IsModelHasSkeletonAnimation(sceneHandle,modelHandle);
}

////设置模型的颜色
//DLL_EXPORT VOID SetModelColor(DWORD sceneHandle, DWORD modelHandle, INT red, INT green, INT blue, INT alpha);
//
////得到模型的透明度
//DLL_EXPORT INT GetModelOpacity(DWORD sceneHandle, DWORD modelHandle);
//
////设置模型的透明度
//DLL_EXPORT VOID SetModelOpacity(DWORD sceneHandle, DWORD modelHandle, INT opacity);

// 子模型方法
/** 获取模型子模型使用的材质名称，每个子模型只能有一个材质
@param in sceneHandle ： 场景管理器
@param in modelHandle ：模型名指针
@return 子模型数目
**/
INT GetSubModelCount(DWORD sceneHandle, DWORD modelHandle)
{
	return globalVgsCore.GetSubModelCount(sceneHandle,modelHandle);
}

// 获取模型子模型使用的材质名称，每个子模型只能有一个材质
BOOL GetSubModelMaterialName(DWORD modelHandle, int index, CHAR *sName)
{
	return globalVgsCore.GetSubModelMaterialName(modelHandle,index,sName);
}

// 获取模型子模型使用的材质指针，每个子模型只能有一个材质
DWORD GetSubModelMaterialHandle(DWORD modelHandle, int index)
{
	return globalVgsCore.GetSubModelMaterialHandle(modelHandle,index);
}

/** 为模型中的第index个子模型指定材质
@param in modelHandle : 场景管理器指针
@param in index : 子模型索引
@param in pMtrName : 材质名称
*/
VOID SetSubModelMaterial(DWORD sceneHandle, DWORD modelHandle, int index, const CHAR* pMtrName)
{
	globalVgsCore.SetSubModelMaterial(sceneHandle,modelHandle,index,pMtrName);
}

/**为模型制定材质，如果模型包含多个子模型，那么所有子模型都会使用这个材质
@param in modelHandle : 场景管理器指针
@param in pMtrName : 材质名称
*/
VOID SetModelMaterial(DWORD sceneHandle, DWORD modelHandle, const CHAR* pMtrName)
{
	globalVgsCore.SetModelMaterial(sceneHandle,modelHandle,pMtrName);
}


//-------------------------------------------------------------------------------------------
///材质相关的方法
//-------------------------------------------------------------------------------------------
/** 创建材质
@param strMtrlName in : 要创建的材质名称
*/
DWORD CreateMaterial(DWORD sceneHandle, const CHAR* strMtrlName)
{
	return globalVgsCore.CreateMaterial(sceneHandle,strMtrlName);
}

/** 删除材质
@param mtrlHandle in : 要删除的材质指针
*/
VOID DeleteMaterialByHandle(DWORD sceneHandle, DWORD mtrlHandle)
{
	globalVgsCore.DeleteMaterialByHandle(sceneHandle,mtrlHandle);
}

/** 删除材质
@param pName in : 要删除的材质名称
*/
VOID DeleteMaterialByName(DWORD sceneHandle, const CHAR* pName)
{
	globalVgsCore.DeleteMaterialByName(sceneHandle,pName);
}

/** 得到场景中材质的数量
*/
INT GetMaterialCount(DWORD sceneHandle)
{
	return globalVgsCore.GetMaterialCount(sceneHandle);
}

/** 根据材质的名称得到材质的句柄
*/
DWORD GetMaterialHandleByName(DWORD sceneHandle, const CHAR* strMtrlName)
{
	return globalVgsCore.GetMaterialHandleByName(sceneHandle,strMtrlName);
}

/** 根据材质的编号得到材质的句柄
*/
DWORD GetMaterialHandleByIndex(DWORD sceneHandle, INT index)
{
	return globalVgsCore.GetMaterialHandleByIndex(sceneHandle,index);
}

/** 根据材质的句柄得到材质的名称
*/
VOID GetMaterialNameByHandle(DWORD sceneHandle, DWORD mtrlHandle, CHAR* strMtrlName)
{
	globalVgsCore.GetMaterialNameByHandle(sceneHandle,mtrlHandle,strMtrlName);
}
//获得材质信息
VOID GetMaterialInfo(DWORD sceneHandle, DWORD mtrlHandle, MtrlInfo* pMtrlInfo)
{
	return globalVgsCore.GetMaterialInfo(sceneHandle,mtrlHandle, pMtrlInfo);
}
//设置材质信息
VOID SetMaterialInfo(DWORD sceneHandle, DWORD mtrlHandle, MtrlInfo* pMtrlInfo)
{
	return globalVgsCore.SetMaterialInfo(sceneHandle,mtrlHandle,pMtrlInfo);
}

//判断当前材质是否被模型关联
DWORD IsMtrlUsingByModel(DWORD sceneHandle, DWORD mtrlHandle)
{
	return globalVgsCore.IsMtrlUsingByModel(sceneHandle,  mtrlHandle);
}

/** 获取指定的材质是否使用了贴图
*/
BOOL IsMaterialHasTexture(DWORD sceneHandle, DWORD mtrlHandle)
{
	return globalVgsCore.IsMaterialHasTexture(sceneHandle,mtrlHandle);
}


///////////// texture layer ///////////////////////////////////
/** 获取材质的texture数目
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@return texture的数目 
*/
DWORD GetMaterialTextureCount(DWORD sceneHandle, const CHAR* pMaterailName)
{
	return globalVgsCore.GetMaterialTextureCount(sceneHandle, pMaterailName);
}

/** 获取材质的第textureIndex个texture句柄
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureIndex  in : texture索引
@return texture的句柄
*/
DWORD GetMaterialTextureHandle(DWORD sceneHandle, const CHAR* pMaterailName, int textureIndex)
{
	return globalVgsCore.GetMaterialTextureHandle(sceneHandle,pMaterailName,textureIndex);
}

/** 清空材质的一个texture内容, 清空后texture的指针依然有效
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle in : texture句柄
*/
VOID ClearMaterialTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle)
{
	globalVgsCore.ClearMaterialTexture(sceneHandle, pMaterailName,textureHandle);
}

/** 删除材质的一层texture，删除后，该层texture之后的所有texture索引将会改变, <b>使用时一定要注意</b>。
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle in : texture句柄
*/
VOID RemoveMaterialTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle)
{
	//注:当前不实现-linxq
	return globalVgsCore.RemoveMaterialTexture(sceneHandle,pMaterailName,textureHandle);
}

/** 获取textureIndex的类型
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param textureHandle  in : texture句柄
@return texture的类型
*/
VGS_TextureType GetTextureType(DWORD sceneHandle, const CHAR* pMaterailName, DWORD textureHandle)
{
	return globalVgsCore.GetTextureType(sceneHandle,pMaterailName,textureHandle);
}


/** 设置材质的动态UV速度
*/
// VOID SetMaterialUVSpeed(DWORD sceneHandle, DWORD mtrlHandle, float USpeed, float VSpeed)
VOID SetTextureUVSpeed(DWORD sceneHandle, DWORD textureHandle, float USpeed, float VSpeed)
{
	globalVgsCore.SetTextureUVSpeed(sceneHandle,textureHandle,USpeed,VSpeed);
}

/** 获取材质的动态UV速度
*/
// VOID GetMaterialUVSpeed(DWORD sceneHandle, DWORD mtrlHandle, float &USpeed, float &VSpeed)
VOID GetTextureUVSpeed(DWORD sceneHandle, DWORD textureHandle, float &USpeed, float &VSpeed)
{
	globalVgsCore.GetTextureUVSpeed(sceneHandle,textureHandle,USpeed,VSpeed);
}


/** 获取textureIndex使用的UV坐标编号
@param sceneHandle   in : VGS场景指针
@param textureHandle  in : texture句柄
@return texture的UV坐标编号
*/
INT  GetTextureUVIndex(DWORD sceneHandle, DWORD textureHandle)
{
	return globalVgsCore.GetTextureUVIndex(sceneHandle,textureHandle);
}

/** 设置textureIndex使用的UV坐标编号
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param UVIndex		 in : texture使用的UV坐标编号
*/
VOID SetTextureUVIndex(DWORD sceneHandle, DWORD textureHandle, INT UVIndex)
{
	globalVgsCore.SetTextureUVIndex(sceneHandle,textureHandle,UVIndex);
}

/** 获取贴图的Mipmaps数目
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture指针
@return 贴图的Mipmaps数目
*/
INT GetTextureMipmaps(DWORD sceneHandle, DWORD textureHandle)
{
	return globalVgsCore.GetTextureMipmaps(sceneHandle,textureHandle);
}

/** 设置贴图的Mipmaps数目
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture指针
@param Mipmaps       in : 贴图的Mipmaps数目
*/
VOID SetTextureMipmaps(DWORD sceneHandle, DWORD textureHandle, INT Mipmaps)
{
	globalVgsCore.SetTextureMipmaps(sceneHandle,textureHandle,Mipmaps);
}


/** 设置texture的混色信息
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param BlenderInfo	 in : texture的混色参数
*/
VOID SetTextureBlenderInfo(DWORD sceneHandle, DWORD textureHandle, VGS_BlenderType type, const VGS_BlenderInfo &BlenderInfo)
{
	globalVgsCore.SetTextureBlenderInfo(sceneHandle,textureHandle,type, BlenderInfo);
}

/** 获取texture的混色信息
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param BlenderInfo	 out : texture的混色参数
*/
VOID GetTextureBlenderInfo(DWORD sceneHandle, DWORD textureHandle, VGS_BlenderType type, VGS_BlenderInfo &BlenderInfo)
{
	globalVgsCore.GetTextureBlenderInfo(sceneHandle,textureHandle,type,BlenderInfo);
}


///////////// texture ///////////////////////////////////


// 从Image buffer更新使用该Image的所有texture
bool FillTexturesFromBuffer(UINT len, CHAR* pBuffer, INT imgIndex)
{
	return (globalVgsCore.FillTexturesFromBuffer(len,pBuffer,imgIndex)== TRUE ?true:false);
}

//------------基本、透明、反射贴图-----------start

/** 为材质增加一层基本贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片名称，包括完成路径
@param pTextureName  in : texture层的名称，传入NULL表示自动命名
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
DWORD AddMaterialBasalTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureFile, const CHAR* pTextureName/* = NULL*/, int UVIndex/* = 0*/)
{
	return globalVgsCore.AddMaterialBasalTexture(sceneHandle,pMaterailName,pTextureFile,pTextureName/* = NULL*/, UVIndex/* = 0*/);
}
/** 设置基本贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件，包含完整路径
*/
VOID SetBasalTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile)
{
	globalVgsCore.SetBasalTextureImage(sceneHandle,textureHandle, pTextureFile);
}
/** 获取基本贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到贴图，返回TRUE,否则FALSE
*/
BOOL GetBasalTextureImage(DWORD sceneHandle, DWORD textureHandle, CHAR* pTextureFile)
{
	return globalVgsCore.GetBasalTextureImage(sceneHandle,textureHandle,pTextureFile);
}

/** 为材质增加一层透明贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片名称，包括完整路径
@param pTextureName  in : texture层的名称，传入NULL表示自动命名
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
DWORD AddOpacityTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureFile, const CHAR* pTextureName/* = NULL*/, int UVIndex/* = 0*/)
{
	return globalVgsCore.AddOpacityTexture(sceneHandle,pMaterailName, pTextureFile, pTextureName/* = NULL*/,  UVIndex/* = 0*/);
}
VOID SetTransparentTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile)
{
	globalVgsCore.SetTransparentTextureImage(sceneHandle,textureHandle,pTextureFile);
}

/** 为材质增加一层反射贴图
@param hScene   in : VGS场景指针
@param szMtrl in : 材质名称
@param szTextureFile  in : texture使用的图片名称，包括完整路径
@param szTexture  in : texture层的名称，传入NULL表示自动命名
@param nUVindex	 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@param vrt	 in : 反射类型
@return 新创建的texture句柄
*/
DWORD AddReflectTexture(DWORD hScene, const char* szMtrl, const char* szTextureFile, const char* szTexture, int nUVindex, VGS_REFLECT_TYPE vrt)
{
	return globalVgsCore.AddReflectTexture(hScene, szMtrl, szTextureFile,  szTexture,  nUVindex,  vrt);
}
bool SetReflectTextureType(DWORD hScene, DWORD hTexture, VGS_REFLECT_TYPE vrt)
{
	return globalVgsCore.SetReflectTextureType(hScene, hTexture, vrt)== TRUE ?true:false;
}
VGS_REFLECT_TYPE GetReflectTextureType(DWORD hScene, DWORD hTexture)
{
	return globalVgsCore.GetReflectTextureType(hScene,hTexture);
}

//------------基本、透明、反射贴图-----------end

//------------动态贴图-----------start

/** 为材质增加一层动态贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param FrameCount  in : texture使用的图片序列的图像数目
@param pTextureFile  in : texture使用的图片序列，包括完成路径
@param Duration      in : 播放所有图片的时间长度，单位秒。例如，如果一共有5张图片，Duration = 1，那么平均200毫秒播放一张图片
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
DWORD AddMaterialDynamicTexture(DWORD sceneHandle, const CHAR* pMaterailName, int FrameCount, DWORD* pTextureFile, int Duration, int UVIndex/* = 0*/)
{
	return globalVgsCore.AddMaterialDynamicTexture(sceneHandle,  pMaterailName,  FrameCount, pTextureFile,  Duration,  UVIndex/* = 0*/);
}
/** 获取动态贴图所用图片的数量
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
*/
INT GetDynamicTextureImageCount(DWORD sceneHandle, DWORD textureHandle)
{
	return globalVgsCore.GetDynamicTextureImageCount(sceneHandle, textureHandle);
}
/** 获取动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到图片，返回TRUE，否则FALSE
*/
BOOL GetDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT index, CHAR* pTextureFile)
{
	return globalVgsCore.GetDynamicTextureImageByIndex(sceneHandle, textureHandle, index,  pTextureFile);
}

/** 设置动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@param pTextureFile	 in : texture使用的图像文件， 包含完整路径
*/
VOID SetDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT index, const CHAR* pTextureFile)
{
	globalVgsCore.SetDynamicTextureImageByIndex(sceneHandle, textureHandle,index, pTextureFile);
}
/** 为动态贴图重新设置所有使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param FrameCount    in : texture使用的图片数目
@param pTextureFile  in : texture使用的图片列表
*/
VOID SetDynamicTextureImages(DWORD sceneHandle, DWORD textureHandle, int FrameCount, DWORD* pTextureFile)
{
	globalVgsCore.SetDynamicTextureImages(sceneHandle,textureHandle,FrameCount, pTextureFile);
}

/** 添加一张动态贴图图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : 要添加的texture图像文件， 包含完整路径
*/
VOID AddDynamicTextureImage(DWORD sceneHandle, DWORD textureHandle, const CHAR* pTextureFile)
{
	globalVgsCore.AddDynamicTextureImage(sceneHandle,textureHandle,pTextureFile);
}

/** 删除动态贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param index		 in : 动态贴图序列中的图片索引
@return 删除成功返回TRUE，否则FALSE
*/
BOOL DeleteDynamicTextureImageByIndex(DWORD sceneHandle, DWORD textureHandle, INT FrameIdx)
{
	return globalVgsCore.DeleteDynamicTextureImageByIndex(sceneHandle, textureHandle, FrameIdx);
}

/** 设置动态贴图的播放周期长度，单位秒。 注意：如果图片序列为空，调用该方法是无效的。
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param Duration      in : 动态贴图序列播放一个周期所需的时间长度,单位秒
*/
VOID SetDynamicTextureDuration(DWORD sceneHandle, DWORD textureHandle, INT Duration)
{
	globalVgsCore.SetDynamicTextureDuration(sceneHandle, textureHandle, Duration);
}
/** 获取动态贴图序列播放时间长度
@param sceneHandle	   in  : 场景管理器
@param textureHandle   in  : texture指针  
@return 动态贴图序列播放时间长度
*/
INT GetDynamicDuration(DWORD sceneHandle, DWORD textureHandle)
{
	return globalVgsCore.GetDynamicDuration(sceneHandle, textureHandle);
}
//------------动态贴图-----------end


//------------立方贴图-----------start
VGS_CubemapType GetCubeTextureType(DWORD hScene, DWORD hTexture)
{
	return globalVgsCore.GetCubeTextureType(hScene, hTexture);
}

/** 为材质增加一层立方贴图
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片序列，包括完成路径。cubic必须包含6张图片，图片需要按照前后左右上下的顺序提供
@param pMaterailName in : 材质名称，如果为空，将自动命名
@return 新创建的texture句柄
*/
DWORD AddMaterialCubicStaticTexture(DWORD sceneHandle, const CHAR* pMaterailName, DWORD* pTextureFile, const CHAR* pTextureName)
{
	return globalVgsCore.AddMaterialCubicStaticTexture(sceneHandle,  pMaterailName,  pTextureFile,  pTextureName);
}
/** 设置cubic贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件序列，包含完整路径。必须是6张图片，而且按照前后左右上下的顺序提供
*/
VOID SetCubicTextureImages(DWORD sceneHandle, DWORD textureHandle, DWORD* pTextureFile)
{
	globalVgsCore.SetCubicTextureImages(sceneHandle, textureHandle, pTextureFile);
}
/** 获取立方贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param faceType      in : 指定一个面
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
@return 得到贴图，返回TRUE,否则FALSE
*/
BOOL GetCubicTextureImages(DWORD sceneHandle, DWORD textureHandle, VGS_CUBIC_FACE faceType, CHAR* pTextureFile)
{
	return globalVgsCore.GetCubicTextureImages( sceneHandle, textureHandle, faceType, pTextureFile);
}

/** 为材质增加一层动态立方贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称，如果为空，将自动命名
@param pTextureName  in : 创建的texture名称
@return 新创建的texture句柄
*/
DWORD AddMaterialCubicDynamicTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* pTextureName)
{
	return globalVgsCore.AddMaterialCubicDynamicTexture(sceneHandle,  pMaterailName,  pTextureName);
}

// 为动态cubmap添加反射模型
VOID AddDynamicReflectedModel(DWORD sceneHandle, DWORD DynamicCubeHandle, DWORD modelHandle)
{
	globalVgsCore.AddDynamicReflectedModel(sceneHandle, DynamicCubeHandle,  modelHandle);
}
// 移除动态cubemap的反射模型
VOID RemoveDynamicReflectedModel(DWORD sceneHandle, DWORD DynamicCubeHandle, DWORD modelHandle)
{
	globalVgsCore.RemoveDynamicReflectedModel(sceneHandle, DynamicCubeHandle,modelHandle);
}
void RemoveDynamicReflectedAllModel(DWORD hScene, DWORD hDynamicCubeMap)
{
	globalVgsCore.RemoveDynamicReflectedAllModel(hScene,hDynamicCubeMap);
}

int GetDynamicReflectedModelCount(DWORD hScene, DWORD hDynamicCubeMap)
{
	return globalVgsCore.GetDynamicReflectedModelCount( hScene, hDynamicCubeMap);
}
DWORD GetDynamicReflectedModelHandle(DWORD hScene, DWORD hDynamicCubeMap, int nIdx)
{
	return globalVgsCore.GetDynamicReflectedModelHandle(hScene, hDynamicCubeMap, nIdx);
}

//------------立方贴图-----------end

//------------bump贴图-----------start
/** 为材质增加一层bump贴图，新增加的贴图将会在最上层
@param sceneHandle   in : VGS场景指针
@param pMaterailName in : 材质名称
@param pTextureFile  in : texture使用的图片，包括完整路径。如果不指定图片，传入NULL或一个空字符串。
@param UVIndex		 in : texture使用哪一套UV坐标，目前VGS支持4组UV坐标
@return 新创建的texture句柄
*/
DWORD AddMaterialBumpTexture(DWORD sceneHandle, const char* pMaterailName, const CHAR* pTextureFile, VGS_BumpType type, int UVIndex/* = 0*/)
{
	return globalVgsCore.AddMaterialBumpTexture(sceneHandle, pMaterailName, pTextureFile,  type, UVIndex/* = 0*/);
}

DWORD AddMaterialBumpEnviromentTexture(DWORD sceneHandle, const char* pMaterailName, const char* pTextureFile, float noise1, float noise2)
{
	return globalVgsCore.AddMaterialBumpEnviromentTexture(sceneHandle, pMaterailName, pTextureFile, noise1, noise2);
}
DWORD AddMaterialBumpNormalTexture(DWORD sceneHandle, const char* pMaterailName, const char* pTextureFile, DWORD lightHandle)
{
	return globalVgsCore.AddMaterialBumpNormalTexture( sceneHandle, pMaterailName,  pTextureFile, lightHandle);
}

DWORD AddMaterialBumpwaveTexture(DWORD sceneHandle, const char* pMaterailName, INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY)
{
	return globalVgsCore.AddMaterialBumpwaveTexture(sceneHandle,pMaterailName, dwWidth,  dwHeight,  xSize,  ySize,  radius,  speed,  centerX,  centerY);
}

VGS_BumpType GetBumpTextureType(DWORD hScene, DWORD hBumpTexture)
{
	return globalVgsCore.GetBumpTextureType(hScene,hBumpTexture);
}
/** 获取bump贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 out : texture使用的图像文件， 不 包含路径
*/
BOOL GetBumpTextureImage(DWORD sceneHandle, DWORD textureHandle, char* pTextureFile)
{
	return globalVgsCore.GetBumpTextureImage(sceneHandle, textureHandle,  pTextureFile);
}

/** 设置Bump贴图使用的图片
@param sceneHandle   in : VGS场景指针
@param textureHandle in : texture句柄
@param pTextureFile	 in : texture使用的图像文件，包含完整路径. 如果不需要图片，设置为NULL或空字符串。
*/
VOID SetBumpTextureImage(DWORD sceneHandle, DWORD textureHandle, const char* pTextureFile)
{
	globalVgsCore.SetBumpTextureImage(sceneHandle, textureHandle,  pTextureFile);
}

/** 获取bump贴图的参数
@param type  out : bump的类型
@param param out : 参数
*/
BOOL GetBumpTextureParam(DWORD sceneHandle, DWORD textureHandle, VGS_BumpParam &param)
{
	return globalVgsCore.GetBumpTextureParam(sceneHandle,textureHandle,  param);
}
/** 设置bump贴图的参数
*/
VOID SetBumpTextureParam(DWORD sceneHandle, DWORD textureHandle, const VGS_BumpParam &param)
{
	globalVgsCore.SetBumpTextureParam(sceneHandle,textureHandle, param);
}

/** 设置bump water的参数
@param sceneHandle      in : VGS场景指针
@param pTexture         in : texture句柄
@param dwWidth/dwHeight in : 贴图的尺寸
@param xSize/ySize      in :
@param radius:
@param speed:
@param centerX/centerY
*/
VOID SetBumpwaveTextureParam(DWORD sceneHandle, DWORD pTexture, 
							  INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY)
{
	globalVgsCore.SetBumpwaveTextureParam(sceneHandle, pTexture, 
							  dwWidth, dwHeight,  xSize,  ySize,  radius,  speed,  centerX,  centerY);
}

/** 判断一层texture是否被用作bump
@param type out : 如果是bump，type返回bump的类型
@return 如果是bump，返回TRUE，否则FALSE
*/
BOOL GetTextureAsBump(DWORD sceneHandle, DWORD textureHandle, VGS_BumpType &type)
{

	return globalVgsCore.GetTextureAsBump(sceneHandle, textureHandle, type);
}
/** 设置材质为bump
*/
VOID SetTextureAsBump(DWORD sceneHandle, DWORD textureHandle, VGS_BumpType type)
{
	globalVgsCore.SetTextureAsBump(sceneHandle, textureHandle, type);
}

//------------bump贴图-----------end

//------------实时反射/折射-----------start

// 增加一层实时反射层
DWORD AddMaterialRTReflectionTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* sTextureLayerName, INT width/* = 256*/, INT height/* = 256*/)
{
	return globalVgsCore.AddMaterialRTReflectionTexture(sceneHandle,  pMaterailName,sTextureLayerName,  width/* = 256*/,  height/* = 256*/);
}
// 设置反射模型以获取反射平面
VOID setRTReflectionModel(DWORD sceneHandle, DWORD textureHandle, DWORD hReflectionModel)
{
	globalVgsCore.setRTReflectionModel(sceneHandle,textureHandle, hReflectionModel);
}

// 获取反射模型
DWORD getRTReflectionModel(DWORD sceneHandle, DWORD textureHandle)
{
	return globalVgsCore.getRTReflectionModel( sceneHandle,  textureHandle);
}
// 设置反射的模型列表
VOID setRTReflectedModel(DWORD sceneHandle, DWORD textureHandle, DWORD count, DWORD* pReflectedModels)
{
	globalVgsCore.setRTReflectedModel(sceneHandle, textureHandle,  count,  pReflectedModels);
}

// 获取被反射的模型数目
DWORD getRTReflectedModelCount(DWORD sceneHandle, DWORD textureHandle)
{
	return globalVgsCore.getRTReflectedModelCount(sceneHandle,  textureHandle);
}

// 获取被反射的模型
DWORD getRTReflectedModelByIndex(DWORD sceneHandle, DWORD textureHandle, INT index)
{
	return globalVgsCore.getRTReflectedModelByIndex(sceneHandle, textureHandle,  index);
}
// 获取反射贴图的尺寸
BOOL getRTReflectionDimension(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height)
{
	return globalVgsCore.getRTReflectionDimension( sceneHandle,  textureHandle, width,height);
}

/** 设置反射贴图的尺寸
*/
VOID setRTReflectionDimension(DWORD sceneHandle, DWORD textureHandle, INT width, INT height)
{
	globalVgsCore.setRTReflectionDimension(sceneHandle, textureHandle,  width,  height);
}

// 增加一层实时折射层
DWORD AddMaterialRTRefractionTexture(DWORD sceneHandle, const CHAR* pMaterailName, const CHAR* sTextureLayerName, INT width/* = 256*/, INT height/* = 256*/)
{
	return globalVgsCore.AddMaterialRTRefractionTexture(sceneHandle,  pMaterailName,  sTextureLayerName,  width/* = 256*/,  height/* = 256*/);
}


// 设置图层为实时折射
VOID setTextureAsRTRefraction(DWORD sceneHandle, DWORD textureHandle)
{
	globalVgsCore.setTextureAsRTRefraction(sceneHandle, textureHandle);
}
// 设置折射模型以获取折射平面
VOID setRTRefractionModel(DWORD sceneHandle, DWORD textureHandle, DWORD hRefractionModel)
{
	globalVgsCore.setRTRefractionModel(sceneHandle, textureHandle, hRefractionModel);
}

// 获取折射模型
DWORD getRTRefractionModel(DWORD sceneHandle, DWORD textureHandle)
{
	return globalVgsCore.getRTRefractionModel(sceneHandle, textureHandle);
}
// 设置折射的模型列表
VOID setRTRefractedModel(DWORD sceneHandle, DWORD textureHandle, DWORD count, DWORD* phRefractedModels)
{
	globalVgsCore.setRTRefractedModel(sceneHandle, textureHandle, count, phRefractedModels);
}
// 获取被折射的模型数目
DWORD getRTRefractedModelCount(DWORD sceneHandle, DWORD textureHandle)
{
	return globalVgsCore.getRTRefractedModelCount(sceneHandle,textureHandle);
}

// 获取被反射的模型
DWORD getRTRefractedModelByIndex(DWORD sceneHandle, DWORD textureHandle, INT index)
{
	return globalVgsCore.getRTRefractedModelByIndex(sceneHandle,textureHandle,  index);
}

// 获取折射贴图的尺寸
BOOL getRTRefractionDimension(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height)
{
	return globalVgsCore.getRTRefractionDimension( sceneHandle, textureHandle, width, height);
}
/** 设置折射贴图的尺寸
*/
VOID setRTRefractionDimension(DWORD sceneHandle, DWORD textureHandle, INT width, INT height)
{
	globalVgsCore.setRTRefractionDimension(sceneHandle, textureHandle, width, height);
}
// 设置贴图层为实时反射
VOID setTextureAsRTReflection(DWORD sceneHandle, DWORD textureHandle)
{
	globalVgsCore.setTextureAsRTReflection(sceneHandle, textureHandle);
}

//------------实时反射/折射-----------end
//---------Flash Texture Start----linxq 2009-12-15
DWORD AddMaterialFlashTexture(DWORD hScene, const char* pMtrlName, const char* pTxtrName, const char* pFlashPath,
							  int flashWidth, int flashHeight, int nWidth, int nHeight)
{
	return globalVgsCore.AddMaterialFlashTexture( hScene,   pMtrlName,  pTxtrName,  pFlashPath,
							   flashWidth,  flashHeight,  nWidth,  nHeight);
}
void SetFlashTexturePlayStatus(DWORD hScene, DWORD textureHandle, int nStatus)
{
	globalVgsCore.SetFlashTexturePlayStatus( hScene,  textureHandle,  nStatus);
}
void SetFlashTextureGoToFrame(DWORD hScene, DWORD textureHandle, int nFrameIndex)
{
	globalVgsCore.SetFlashTextureGoToFrame( hScene,  textureHandle,  nFrameIndex);
}

//---------Flash Texture end----



/** 设置纹理的大小
@param sceneHandle  in  : 场景管理器
@param mtrlHandle   in  : 材质指针  
@param mapChanel    in  : 贴图通道 
@param width/height out : 图片的大小
*/
//DLL_EXPORT VIOD SetTextureSize(DWORD sceneHandle, DWORD textureHandle, INT width, INT height);

/** 获取纹理的大小
@param sceneHandle  in  : 场景管理器
@param mtrlHandle   in  : 材质指针  
@param mapChanel    in  : 贴图通道 
@param width/height out : 图片的大小
@return TRUE：返回的width/heigh有效，FALSE:返回的width/heigh无效
*/
BOOL GetTextureSize(DWORD sceneHandle, DWORD textureHandle, INT &width, INT &height, INT iFrmIndex/* = 0*/)
{
	return globalVgsCore.GetTextureSize( sceneHandle,  textureHandle,  width,  height,  iFrmIndex/* = 0*/);
}


///** 获取贴图图像的大小
//@param sceneHandle  in  : 场景管理器
//@param mtrlHandle   in  : 材质指针  
//@param mapChanel    in  : 贴图通道 
//@param width/height out : 图片的大小
//@return TRUE：返回的width/heigh有效，FALSE:返回的width/heigh无效
//*/
//DLL_EXPORT BOOL GetTextureSize(DWORD sceneHandle, DWORD mtrlHandle, INT mapChanel, INT &width, INT &height);

//设置材质的颜色
VOID SetMaterialColor(DWORD sceneHandle, DWORD mtrlHandle, INT red, INT green, INT blue, INT alpha)
{
	return globalVgsCore.SetMaterialColor( sceneHandle,  mtrlHandle,  red,  green,  blue,  alpha);
}

//设置是否显示材质的贴图
VOID SetShowMtrlTexture(DWORD sceneHandle, DWORD mtrlHandle, BOOL showTexture)
{
	globalVgsCore.SetShowMtrlTexture(sceneHandle, mtrlHandle, showTexture);
}
//获得是否显示贴图的状态
BOOL GetShowMtrlTexture(DWORD sceneHandle, DWORD mtrlHandle)
{
	return (BOOL)(globalVgsCore.GetShowMtrlTexture(sceneHandle, mtrlHandle));
}

//-------------------------------------------------------------------------------------------
///材质与纹理----------------end--
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
///其他与图像相关的接口----------------start--
//-------------------------------------------------------------------------------------------

/** 得到场景中贴图的数量, 包括2D、3D使用的贴图
*/ 
// DLL_EXPORT UINT GetTextureCount(DWORD sceneHandle);
UINT GetTextureImageCount(DWORD sceneHandle)
{
	return globalVgsCore.GetTextureImageCount( sceneHandle);
}

/** 根据索引得到纹理的文件名称（不包含路径）
@param in sceneHandle : 场景指针
@param in index : 纹理在队列中的索引
@param out TextureName : 纹理名称
*/
// DLL_EXPORT BOOL GetTextureNamebyIndex(DWORD sceneHandle, INT index, CHAR* TextureName);
BOOL GetTextureImageNamebyIndex(DWORD sceneHandle, INT index, CHAR* TextureName)
{
	return globalVgsCore.GetTextureImageNamebyIndex( sceneHandle,  index,  TextureName);
}

/** 图像是否正被使用
@param ImagePureName in : 图像文件名称，不包含路径
*/
BOOL IsImageInused(DWORD sceneHandle, const CHAR* ImagePureName)
{
	return globalVgsCore.IsImageInused( sceneHandle,   ImagePureName);
}
//-------------------------------------------------------------------------------------------
///其他与图像相关的接口----------------end--
//-------------------------------------------------------------------------------------------


/** 创建天空盒
*/
VOID CreateSkyBox(DWORD sceneHandle, const CHAR* sTextureName)
{
	globalVgsCore.CreateSkyBox(sceneHandle, sTextureName);
}

/** 设置天空盒使用的贴图
*/
VOID ChangeSetSkyBoxTexture(DWORD sceneHandle, const CHAR* sTextureName)
{
	globalVgsCore.ChangeSetSkyBoxTexture(sceneHandle, sTextureName);
}

/** 设置天空盒的距离
*/
VOID SetSkyBoxDistance(DWORD sceneHandle, FLOAT dist)
{
	globalVgsCore.SetSkyBoxDistance(sceneHandle, dist);
}

/** 设置天空盒是否有效
*/
VOID SetSkyBoxEnabled(DWORD sceneHandle, BOOL bEnabled)
{
	globalVgsCore.SetSkyBoxEnabled(sceneHandle, bEnabled);
}

/** 使用指定图片创建背景图,每个场景只能有一张背景图。重复设置背景图将清除之前已经创建的背景图。
*/
DWORD SetBackground(DWORD sceneHandle, const CHAR* sImageFile)
{	
	return globalVgsCore.SetBackground( sceneHandle,   sImageFile);
}

// 获取背景图句柄，每个场景只能有一张背景图
DWORD GetBackgroundHandle(DWORD sceneHandle)
{
	return globalVgsCore.GetBackgroundHandle( sceneHandle);
}

/** 删除场景背景图
*/
VOID RemoveBackground(DWORD sceneHandle)
{
	globalVgsCore.RemoveBackground( sceneHandle);
}

/** 获取背景图使用的图像名称，不包括路径
*/
BOOL GetBackgroundImage(DWORD sceneHandle, CHAR *pImageFile)
{
	return globalVgsCore.GetBackgroundImage(sceneHandle, pImageFile);
}  

/** 设置场景的背景图是否可见
*/
VOID SetBackgroundVisibility(DWORD sceneHandle, BOOL bVisible)
{
	globalVgsCore.SetBackgroundVisibility( sceneHandle,  bVisible);
}

/** 获取场景的背景图是否可见
*/
BOOL IsBackgroundVisible(DWORD sceneHandle)
{
	return globalVgsCore.IsBackgroundVisible( sceneHandle);
}

/** 设置背景图的相对与视口的相对大小和相对位置，left/top/width/height取值范围[0, 1]，坐标原点在视口的左上角
建议使用FRect结构传递参数.
*/
VOID SetBackgroundRect(DWORD sceneHandle, FLOAT left, FLOAT top, FLOAT width, FLOAT height)
{
	globalVgsCore.SetBackgroundRect( sceneHandle,  left,  top,  width,  height);
} 

/** 获取背景图的相对与视口的相对大小和相对位置，left/top/width/height取值范围[0, 1]，坐标原点在视口的左上角
建议使用FRect结构传递参数.
*/
VOID GetBackgroundRect(DWORD sceneHandle, FLOAT &left, FLOAT &top, FLOAT &width, FLOAT &height)
{
	globalVgsCore.GetBackgroundRect( sceneHandle,  left,  top,  width,  height);
}

/** 背景图全屏显示
*/
VOID FullScreenBackground(DWORD sceneHandle)
{
	globalVgsCore.FullScreenBackground( sceneHandle);
}

//-------------------------------------------------------------------------------------------
///摄像机相关的方法
//-------------------------------------------------------------------------------------------
/** 使用缺省参数创建一个相机
*/
DWORD CreateCamera(DWORD sceneHandle, CHAR* strCameraName)
{
	return globalVgsCore.CreateCamera( sceneHandle,  strCameraName);
}

/** 创建各种预定义相机
*/
DWORD CreateFrontCamera(DWORD sceneHandle)
{
	return globalVgsCore.CreateFrontCamera( sceneHandle);
}
DWORD CreateBackCamera(DWORD sceneHandle)
{
	return globalVgsCore.CreateBackCamera(sceneHandle);
}
DWORD CreateTopCamera(DWORD sceneHandle)
{
	return globalVgsCore.CreateTopCamera( sceneHandle);
}
DWORD CreateBottomCamera(DWORD sceneHandle)
{
	return globalVgsCore.CreateBottomCamera( sceneHandle);

} 
DWORD CreateLeftCamera(DWORD sceneHandle)
{
	return globalVgsCore.CreateLeftCamera(sceneHandle);
}
DWORD CreateRightCamera(DWORD sceneHandle)
{
	return globalVgsCore.CreateRightCamera( sceneHandle);
}
DWORD CreatePerspectiveCamera(DWORD sceneHandle)
{
	return globalVgsCore.CreatePerspectiveCamera( sceneHandle);
}

// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD GetFrontCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetFrontCamera( sceneHandle);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD GetBackCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetBackCamera( sceneHandle);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD GetTopCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetTopCamera( sceneHandle);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD GetBottomCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetBottomCamera( sceneHandle);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD GetLeftCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetLeftCamera( sceneHandle);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD GetRightCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetRightCamera( sceneHandle);
}
// 获取预定义相机的指针，如果相机已经存在，返回相机的指针，否则返回NULL
DWORD GetPerspectiveCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetPerspectiveCamera( sceneHandle);
}

//得到场景中摄像机的数量
INT GetCameraCount(DWORD sceneHandle)
{
	return globalVgsCore.GetCameraCount( sceneHandle);
}

//根据像机的名称得到像机的句柄
DWORD GetCameraHandleByName(DWORD sceneHandle, CHAR* strCameraName)
{
	return globalVgsCore.GetCameraHandleByName( sceneHandle,  strCameraName);
}
//根据摄像机的编号得到像机的句柄
DWORD GetCameraHandleByIndex(DWORD sceneHandle, INT index)
{
	return globalVgsCore.GetCameraHandleByIndex( sceneHandle,  index);
}

//根据像机的句柄得到像机的名称
VOID GetCameraNameByHandle(DWORD sceneHandle, DWORD camHandle, CHAR* strCameraName)
{
	globalVgsCore.GetCameraNameByHandle( sceneHandle,  camHandle,  strCameraName);
}
//设置相机的名称
VOID SetCameraNamebyHandle(DWORD sceneHandle, DWORD camHandle, CHAR* strCameraName)
{
	globalVgsCore.SetCameraNamebyHandle( sceneHandle,  camHandle,  strCameraName);
}
VOID SetCameraNamebyName(DWORD sceneHandle, CHAR* strOldCameraName, CHAR* strNewCameraName)
{
	globalVgsCore.SetCameraNamebyName( sceneHandle,  strOldCameraName,  strNewCameraName);
}


/** 根据相机的索引得到相机的名字
*/
BOOL GetCameraNameByIndex(DWORD sceneHandle, INT index, CHAR* pName)
{
	return globalVgsCore.GetCameraNameByIndex( sceneHandle,  index,  pName);
}


/**删除相机
*/
VOID DeleteCamerabyHandle(DWORD sceneHandle, DWORD camHandle)
{
	globalVgsCore.DeleteCamerabyHandle( sceneHandle,  camHandle);
}
/**删除相机
*/
VOID DeleteCamerabyName(DWORD sceneHandle, CHAR* camName)
{
	globalVgsCore.DeleteCamerabyName( sceneHandle,  camName);
}

//设置当前摄像机
VOID SetCurrentCamera(DWORD sceneHandle, DWORD swapChainHandle, DWORD camHandle)
{
	globalVgsCore.SetCurrentCamera( sceneHandle,  swapChainHandle,  camHandle);
}
//得到当前摄像机句柄
DWORD GetCurrentCamera(DWORD sceneHandle, DWORD swapChainHandle)
{
	return globalVgsCore.GetCurrentCamera( sceneHandle,  swapChainHandle);
}

//设置当前相机为系统预设相机
// DLL_EXPORT VOID SetCurrentCameraToPreConfig(DWORD sceneHandle, DWORD swapChainHandle, CAMERATYPE cameraType);

//获得当前相机的是否为预设相机，如果不是返回NONE
// DLL_EXPORT CAMERATYPE GetCurrentPreConfigCamera(DWORD sceneHandle, DWORD swapChainHandle);


/** 平移摄像机
@param in sceneHandle 场景管理器
@param in camHandle 相机句柄
@param in pVec 相机移动的相对向量
@param in coordEnum 参考坐标系
	//0 - 在世界坐标系平移摄像机
	//1 - 在自身坐标系平移
	//2 - walk，不能飞到天上去
*/
VOID MoveCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3* pVec, UINT coordEnum)
{
	globalVgsCore.MoveCamera( sceneHandle,  camHandle,  pVec,  coordEnum);
}

/** 旋转摄像机
@param in pVec : 各个方向上的旋转角度
@param in coordEnum : 参考坐标系
//0 - 绕世界坐标系旋转摄像机
//1 - 绕自身坐标系旋转像机
//2 - 绕穿过from中心，与世界坐标系平行的坐标系旋转
*/
VOID RotateCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3* pVec, UINT coordEnum)
{
	globalVgsCore.RotateCamera( sceneHandle,  camHandle, pVec,  coordEnum);
}

/** 以pTargetPos点为中心,旋转相机fAngle弧度
@param in sceneHandle   : 场景管理器
@param in camHandle     : 相机句柄
@param in pTargetPos    : 围绕旋转的中心点(世界坐标系)
@param in axistype      : 围绕旋转的轴(世界坐标系)， 0：world X， 1：world y，2：world z, 3: local X, 4: local Y, 5: local Z
@param in fAngle        : 旋转的角度，以 弧度 为单位
*/ 
VOID RoundCamera(DWORD sceneHandle, DWORD camHandle, VECTOR3 *pTargetPos, INT axistype, FLOAT fAngle)
{
	globalVgsCore.RoundCamera( sceneHandle,  camHandle,  pTargetPos,  axistype,  fAngle);
}
/** 以pTargetPos点为中心,旋转相机fAngle弧度
@param in sceneHandle   : 场景管理器
@param in camHandle     : 相机句柄
@param in pTargetPos    : 围绕旋转的中心点
@param in axistype      : 围绕旋转的轴， 0：x， 1：y，2：z
@param in fAngle        : 旋转的角度，以 弧度 为单位
*/ 
// VOID RoundCamera1(DWORD sceneHandle, DWORD camHandle, VECTOR3* pAxisCenter, VECTOR3* pAxisVec, FLOAT fAngle)
// {
// 	CScene* vgsScene = (CScene*)sceneHandle;
// 	if (vgsScene == NULL) return;
// 
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->Round1(&D3DXVECTOR3(pAxisCenter->x, pAxisCenter->y, pAxisCenter->z), 
// 					&D3DXVECTOR3(pAxisVec->x, pAxisVec->y, pAxisVec->z), 
// 					fAngle*ATOS);
// }

/** 设置相机的宽高比

VOID SetCameraAspect(DWORD camHandle, const FLOAT fAspectRatio)
{

}*/

/**以定点为轴心, 在相机坐标系中旋转相机
@param pAxisCenter in : 相机围绕旋转的目标点
@param pXYZAangle  in : 相机旋转的角度，弧度 
*/
VOID RoundInCameraSpace(DWORD sceneHandle, DWORD camHandle, VECTOR3* pAxisCenter, VECTOR3* pXYZAngle)
{
	globalVgsCore.RoundInCameraSpace( sceneHandle,  camHandle,  pAxisCenter,  pXYZAngle);
}

/** 相机目标点不动，沿相机方向前后推拉相机。
@param in sceneHandle : 场景指针
@param in camHandle : 相机指针
@param dollyValue  in   : 相机推拉的距离，> 0 向前， < 1向后
*/
VOID DollyCamera(DWORD sceneHandle, DWORD camHandle, FLOAT dollyValue)
{
	globalVgsCore.DollyCamera( sceneHandle,  camHandle,  dollyValue);
}

/*设置摄像机的世界坐标系位置
@param in sceneHandle : 场景指针
@param in camHandle   : 相机指针
@param in pPos        : 世界坐标系中的新位置
*/
VOID SetCameraPosition(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos)
{
	globalVgsCore.SetCameraPosition( sceneHandle,  camHandle,  pPos);
}

/*获取摄像机的世界坐标系位置
@param in sceneHandle : 场景指针
@param in camHandle   : 相机指针
@param in pPos        : 世界坐标系中的新位置
*/
VOID GetCameraPosition(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos)
{
	globalVgsCore.GetCameraPosition( sceneHandle,  camHandle,  pPos);
}

//设置摄像机的目标点
VOID SetCameraAt(DWORD sceneHandle, DWORD camHandle, const VECTOR3* pPos)
{
	globalVgsCore.SetCameraAt( sceneHandle,  camHandle,  pPos);
}
VOID GetCameraAt(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos)
{
	globalVgsCore.GetCameraAt( sceneHandle,  camHandle, pPos);

}
/** 摄像机正对模型
@param in sceneHandle : 场景指针
@param in camHandle : 相机指针
@param in modelHandle : 模型指针
*/
VOID SetCameraLookAtModel(DWORD sceneHandle, DWORD camHandle, DWORD modelHandle)
{
	globalVgsCore.SetCameraLookAtModel( sceneHandle,  camHandle,  modelHandle);
}
// DLL_EXPORT VOID GetCameraAt(DWORD sceneHandle, DWORD camHandle, VECTOR3* pPos);

/** 设置摄像机的FOV
@param in fov : 相机的fov，即y方向的视角
*/
VOID SetCameraFov(DWORD sceneHandle, DWORD camHandle, FLOAT fov)
{
	globalVgsCore.SetCameraFov( sceneHandle,  camHandle,  fov);
}

/** 设置摄像机的nearClip，与相机距离小于nearClip的物体将不会被看到
*/
VOID SetCameraNearClip(DWORD sceneHandle, DWORD camHandle, FLOAT nearClip)
{
	globalVgsCore.SetCameraNearClip( sceneHandle,  camHandle,  nearClip);
}

/** 设置摄像机的farClip，与相机距离大于farClip的物体将不会被看到
*/
VOID SetCameraFarClip(DWORD sceneHandle, DWORD camHandle, FLOAT farClip)
{
	globalVgsCore.SetCameraFarClip( sceneHandle,  camHandle,  farClip);
}

//设置摄像机的Roll
// DLL_EXPORT VOID SetCameraRoll(DWORD sceneHandle, DWORD camHandle, FLOAT rollAngle);

/** 一次性获得相机的属性
*/
BOOL GetCameraInfo(DWORD sceneHandle, DWORD camHandle, CameraInfoEx* pCamInfo)
{
	return globalVgsCore.GetCameraInfo( sceneHandle,  camHandle,  pCamInfo);
}

/** 一次性设置相机的属性
*/
VOID SetCameraInfo(DWORD sceneHandle, DWORD camHandle, CameraInfoEx* pCamInfo)
{
	globalVgsCore.SetCameraInfo( sceneHandle,  camHandle,  pCamInfo);
}

/** 获取相机的方向
@param vDirection out : 相机在世界坐标系中的方向，即lookat的方向。如果获取方向失败，vDirection = (0, 0, 0)
*/
VOID GetCameraDirection(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vDirection)
{
	globalVgsCore.GetCameraDirection( sceneHandle,  camHandle,  vDirection);
}

/** 设置相机的方向
@param vDirection in : 相机在世界坐标系中的方向，即lookat方向
*/
VOID SetCameraDirection(DWORD sceneHandle, DWORD camHandle, const VECTOR3 &vDirection)
{
	globalVgsCore.SetCameraDirection( sceneHandle,  camHandle,  vDirection);
}

// 获取相机的up方向
VOID GetCameraUp(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vUp)
{
	globalVgsCore.GetCameraUp( sceneHandle,  camHandle,  vUp);
}

// 获取相机的right方向
VOID GetCameraRight(DWORD sceneHandle, DWORD camHandle, VECTOR3 &vRight)
{
	globalVgsCore.GetCameraRight( sceneHandle,  camHandle,  vRight);
}

// 获取相机的near/far clip
VOID GetCameraClip(DWORD sceneHandle, DWORD camHandle, float &nearClip, float &farClip)
{
	globalVgsCore.GetCameraClip( sceneHandle,  camHandle,  nearClip,  farClip);
}

//-------------------------------------------------------------------------------------------
///灯光相关的方法
//-------------------------------------------------------------------------------------------
/** 得到场景中灯光的数量
@return 场景中的所有灯光数目
*/
INT GetLightCount(DWORD sceneHandle)
{
	return globalVgsCore.GetLightCount( sceneHandle);
}

/** 删除灯光
*/
VOID DeleteLight(DWORD sceneHandle, DWORD lightHandle)
{
	globalVgsCore.DeleteLight( sceneHandle,  lightHandle);
}

/** 根据灯光的名称得到灯光的句柄
*/
DWORD GetLightHandleByName(DWORD sceneHandle, CHAR* strLightName)
{
	return globalVgsCore.GetLightHandleByName( sceneHandle,  strLightName);
}

/** 根据灯光的编号得到灯光的句柄
*/
DWORD GetLightHandleByIndex(DWORD sceneHandle, INT index)
{
	return globalVgsCore.GetLightHandleByIndex( sceneHandle,  index);
}

/** 根据灯光的句柄得到灯光的名称
*/
VOID GetLightNameByHandle(DWORD sceneHandle, DWORD lightHandle, CHAR* strLightName)
{
	globalVgsCore.GetLightNameByHandle( sceneHandle,  lightHandle,  strLightName);
}

/*设置灯光的名称
@return 设置成功返回TRUE，否则FALSE，失败可能是由于命名冲突导致的。
*/
BOOL SetLightName(DWORD sceneHandle, DWORD lightHandle, CHAR* strLightName)
{
	return globalVgsCore.SetLightName( sceneHandle,  lightHandle,  strLightName);
}

/** 根据灯光的编号得到灯光的名称
*/
BOOL GetLightNameByIndex(DWORD sceneHandle, INT index, CHAR* strLightName)
{
	return globalVgsCore.GetLightNameByIndex( sceneHandle,  index,  strLightName);
}

/** 在世界坐标系中移动灯光
@param in pVec : 移动的目标点世界坐标，绝对值
*/
VOID MoveLight(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pVec)
{
	globalVgsCore.MoveLight( sceneHandle,  lightHandle, pVec);
}

//旋转灯光
// DLL_EXPORT VOID RotateLight(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pRos);

/** 设置灯光的位置
@param in vPos : 灯光要移动到的新位置，参考坐标系是世界坐标系
*/
VOID SetLightPosition(DWORD sceneHandle, DWORD lightHandle, VECTOR3* vPos)
{
	globalVgsCore.SetLightPosition( sceneHandle,  lightHandle,  vPos);
}

//设置灯光的方向
// DLL_EXPORT VOID SetLightDirection(DWORD sceneHandle, DWORD lightHandle, VECTOR3* pDir);

//-----------------------------------------------------------------------------
// 方法名: EnableLight()
// 参  数: DWORD lightHandle, BOOL isEnable
// 用  途: 设置灯光变是否有效
//-----------------------------------------------------------------------------
VOID EnableLight(DWORD sceneHandle, DWORD lightHandle, BOOL isEnable)
{
	globalVgsCore.EnableLight( sceneHandle,  lightHandle,  isEnable);
}

/** 得到灯光的属性
*/
VOID GetLightInfo(DWORD sceneHandle, DWORD lightHandle, LightInfo* pLightInfo)
{
	globalVgsCore.GetLightInfo( sceneHandle,  lightHandle,  pLightInfo);
}

/** 设置灯光的属性
*/
VOID SetLightInfo(DWORD sceneHandle, DWORD lihgtHandle, LightInfo* pLightInfo)
{
	globalVgsCore.SetLightInfo( sceneHandle,  lihgtHandle,  pLightInfo);
}


/** 设置灯光是否有效
*/
VOID SetLightEnableStatus(DWORD sceneHandle, DWORD lightHandle, BOOL bEnable)
{
	globalVgsCore.SetLightEnableStatus(sceneHandle, lightHandle, bEnable);
}


/////////////////////// 二维元素创建方法 /////////////////////////////////////////

/** 初始化GUI状态，新创建的场景如果要使用GUI，必须首先调用该方法
// 如果已经初始化，将不做任何操作，直接返回
// 如果清空或销毁场景后，可重现初始化GUI状态
*/
VOID InitGUIStatus(DWORD sceneHandle, DWORD renderWnd, DWORD viewport)
{
     globalVgsCore.InitGUIStatus( sceneHandle,  renderWnd,  viewport);
}


/** 得到光标下方的GUI对象句柄及类型,如果有多个对象，返回最上层（ZOrder最大）的一个GUI对象
@param sceneHandle in : 场景指针
@param x/y  : 视口中的坐标
@param type out ： GUI对象的类型，参考VGS_GUIOBJ_TYPE定义
@return GUI对象的指针
*/
DWORD GetGUIObjectUnderLoc(DWORD sceneHandle, INT x, INT y, VGS_GUIOBJ_TYPE &type)
{
	return globalVgsCore.GetGUIObjectUnderLoc( sceneHandle,  x,  y,  type);
}

/** 获取GUI对象的类型
@param GUIObjectHandle in : GUI对象的句柄
@return GUI对象的类型，参考VGS_GUIOBJ_TYPE定义
*/
VGS_GUIOBJ_TYPE GetGUIObjectType(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	return globalVgsCore.GetGUIObjectType( sceneHandle,  GUIObjectHandle);
}


/** 返回GUI对象的ZOrder，越大表示GUI对象处于越上层
*/
INT GetGUIObjectZOrder(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	return globalVgsCore.GetGUIObjectZOrder( sceneHandle,  GUIObjectHandle);
}


/** 设置GUI对象的ZOrder，越大表示GUI对象处于越上层
*/
//DLL_EXPORT void SetObjectZOrder(DWORD GUIObjectHandle, int offset);

// GUI对象上移一层
VOID MoveGUIObjectUp(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	globalVgsCore.MoveGUIObjectUp( sceneHandle,  GUIObjectHandle);
}

/** GUI对象下移一层
*/
VOID MoveGUIObjectDown(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	globalVgsCore.MoveGUIObjectDown( sceneHandle,  GUIObjectHandle);
}

/** GUI对象移动到最顶层
*/
VOID GotoGUIObjectsTop(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	globalVgsCore.GotoGUIObjectsTop( sceneHandle,  GUIObjectHandle);
}

/** GUI对象移动到最底层
*/
VOID GotoGUIObjectsBottom(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	globalVgsCore.GotoGUIObjectsBottom( sceneHandle,  GUIObjectHandle);
}

/** 获取所有二维元素的句柄列表
// vHandle必须要预先分配足够的空间
*/ 
VOID GetAllGUIObjectsList(DWORD* vHandle)
{
	//考虑去除该接口...
	globalVgsCore.GetAllGUIObjectsList(vHandle);
}

/** 获取所有GUI(不包括背景图)的数目
*/
UINT GetAllGUIObjectsCount(DWORD sceneHandle)
{
	return globalVgsCore.GetAllGUIObjectsCount( sceneHandle);
}


/**得到某种二维元素(不包括背景图)的数量
*/
INT GetGUIObjectCount(DWORD sceneHandle, VGS_GUIOBJ_TYPE guitype)
{
	return globalVgsCore.GetGUIObjectCount( sceneHandle,  guitype);
}


/** 获取指定类型GUI对象的句柄列表
*/
VOID GetGUIObjectList(VGS_GUIOBJ_TYPE guitype, DWORD* vHandle)
{
	//考虑去除该接口...
	globalVgsCore.GetGUIObjectList(guitype, vHandle);
}

/** 通过索引得到某种gui对象的句柄
*/
DWORD GetGUIObjetHandleByIndex(DWORD sceneHandle, VGS_GUIOBJ_TYPE guitype, INT index)
{
	return globalVgsCore.GetGUIObjetHandleByIndex( sceneHandle,  guitype,  index);
}


/** 设置GUI对象定位的参考原点
*/
VOID SetGUIObjOffsetOrigin(DWORD sceneHandle, DWORD overlayHandle, VGSOFFSETORIGIN offsetOrigin)
{
	globalVgsCore.SetGUIObjOffsetOrigin( sceneHandle,  overlayHandle, offsetOrigin);
}

/** 获得GUI对象定位的参考原点
*/
VGSOFFSETORIGIN GetGUIObjOffsetOrigin(DWORD sceneHandle, DWORD overlayHandle)
{
	return globalVgsCore.GetGUIObjOffsetOrigin( sceneHandle,  overlayHandle);
}


/**设置GUI对象是否使用相对坐标(%)保存位置
*/
VOID SetGUIObjUseRelativePos(DWORD sceneHandle, DWORD guiHandle, bool bRelativePos)
{
	globalVgsCore.SetGUIObjUseRelativePos( sceneHandle,  guiHandle,  bRelativePos);
}

/**获取GUI对象是否使用相对坐标(%)保存位置
*/
bool GetGUIObjUseRelativePos(DWORD sceneHandle, DWORD guiHandle)
{
	return (globalVgsCore.GetGUIObjUseRelativePos( sceneHandle,  guiHandle))== TRUE ?true:false;
}


/** 设置GUI对象是否使用相对尺寸(%)保存尺寸
*/
VOID SetGUIObjUseRelativeSize(DWORD sceneHandle, DWORD guiHandle, bool bRelativeSize)
{
	BOOL b = FALSE;
	globalVgsCore. SetGUIObjUseRelativeSize( sceneHandle,  guiHandle,  b);
	bRelativeSize = b == TRUE ? true: false;
}

/** 获取GUI对象是否使用相对尺寸(%)保存尺寸
*/
bool GetGUIObjUseRelativeSize(DWORD sceneHandle, DWORD guiHandle)
{
	return (globalVgsCore.GetGUIObjUseRelativeSize( sceneHandle,  guiHandle)== TRUE ?true:false);
}


/** 删除 GUI
@param sceneHandle   in : 场景管理器
@param GUIHandle in : GUI句柄
*/
VOID	DeleteGUIObject(DWORD sceneHandle, DWORD GUIHandle)
{
	globalVgsCore.DeleteGUIObject( sceneHandle,  GUIHandle);
}


/** 得到 GUI对象 屏幕坐标的绝对Rect
@param rect out : rect的表示的位置是GUI对象相对于视口offsetOrigin的位置,单位为像素
*/
VOID GetGUIObjectRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, bool &bRelativeSize, bool &bRelativePos)
{
	BOOL  b = FALSE, bb = FALSE;
	globalVgsCore.GetGUIObjectRect( sceneHandle,  GUIHandle,  rect,  b,  bb);
	bRelativeSize = b == TRUE ?true:false;
	bRelativePos = bb == TRUE ?true:false;
}

/** 得到 GUI对象 屏幕坐标的绝对Rect
@param rect out : rect的表示的位置是GUI对象相对于视口左上角的位置,也就是GUI对象在屏幕的实际位置。单位为像素
*/
VOID GetGUIObjectAbsoluteRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, bool &bRelativeSize, bool &bRelativePos)
{
	BOOL  b = FALSE, bb = FALSE;
	globalVgsCore.GetGUIObjectAbsoluteRect( sceneHandle,  GUIHandle,  rect,  b,  bb);
	bRelativeSize = b == TRUE ?true:false;
	bRelativePos = bb == TRUE ?true:false;
}

/** 设置 GUI对象 的相对屏幕坐标的绝对Rect
@param rect in : rect的表示的位置是GUI对象相对于视口offsetOrigin的位置,单位为像素
*/
VOID SetGUIObjectRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, bool bRelativeSize/* = TRUE*/, bool bRelativePos/* = TRUE*/)
{
	globalVgsCore.SetGUIObjectRect( sceneHandle,  GUIHandle,  rect,  bRelativeSize/* = TRUE*/,  bRelativePos/* = TRUE*/);
}

/** 设置 GUI对象 的相对屏幕坐标的绝对Rect
@param rect in : rect的表示的位置是GUI对象相对于视口左上角的位置,也就是GUI对象的实际位置。单位为像素
*/
VOID SetGUIObjectAbsoluteRect(DWORD sceneHandle, DWORD GUIHandle, RECT* rect, bool bRelativeSize/* = TRUE*/, bool bRelativePos/* = TRUE*/)
{
	globalVgsCore.SetGUIObjectAbsoluteRect( sceneHandle,  GUIHandle,  rect,  bRelativeSize/* = TRUE*/,  bRelativePos/* = TRUE*/);
}

/** 设置GUI对象的位置
@param sceneHandle   in : 场景管理器
@param guiHandle in : gui句柄
@param x, y : 相对于视口offsetOrigin的坐标,单位为像素
@param bRelative : true采用相对位置，false采用绝对位置。如果采用相对位置，实际上记录了overlay左上角位置相对于目前视口大小的百分比，
视口缩放时，overlay会保持这个比例不变
*/
VOID SetGUIObjectPosition(DWORD sceneHandle, DWORD guiHandle, INT x, INT y, bool bRelative/* = true*/)
{
	globalVgsCore.SetGUIObjectPosition( sceneHandle,  guiHandle,  x,  y,  bRelative/* = true*/);
}

/** 设置gui对象的尺寸
@param sceneHandle   in : 场景管理器
@param guiHandle in : overlay句柄
@param width, height    : Overlay的宽高
@param bRelative : true采用相对大小，false采用绝对大小。如果采用相对大小，实际上记录了overlay尺寸相对于目前视口尺寸的百分比，
视口缩放时，gui对象会保持这个比例不变
*/
VOID SetGUIObjectSize(DWORD sceneHandle, DWORD guiHandle, INT width, INT height, bool bRelative/* = true*/)
{
	globalVgsCore.SetGUIObjectSize( sceneHandle,  guiHandle,  width,  height,  bRelative/* = true*/);
}
/**得到 gui对象 的透明度
@return gui对象的不透明度 [0, 100]
*/
INT GetGUIObjectOpacity(DWORD sceneHandle, DWORD guiHandle)
{
	return globalVgsCore.GetGUIObjectOpacity( sceneHandle,  guiHandle);
}
 

/**设置 gui对象 的透明度
@opacity in : gui对象的不透明度 [0, 100]
*/
VOID SetGUIObjectOpacity(DWORD sceneHandle, DWORD guiHandle, INT opacity)
{
	globalVgsCore.SetGUIObjectOpacity( sceneHandle,  guiHandle,  opacity);
} 

/** 设置gui对象是否可见
*/
VOID SetGUIObjectVisibility(DWORD sceneHandle, DWORD guiHandle, bool visible)
{
	globalVgsCore.SetGUIObjectVisibility( sceneHandle,  guiHandle,  visible);
}

/** 获取gui对象是否可见
*/
bool GetGUIObjectVisibility(DWORD sceneHandle, DWORD guiHandle)
{
	return (globalVgsCore.GetGUIObjectVisibility( sceneHandle,  guiHandle)== TRUE ?true:false);
}


/** 通过名称得到gui对象
*/
DWORD GetGUIObjectHandleByName(DWORD sceneHandle, CHAR* name)
{
	return globalVgsCore.GetGUIObjectHandleByName( sceneHandle,  name);
}


/** 得到gui对象的名称
*/
VOID GetGUIObjectNameByHandle(DWORD sceneHandle, DWORD guiHandle, CHAR* name)
{
	globalVgsCore.GetGUIObjectNameByHandle( sceneHandle,  guiHandle,  name);
}

/** 设置gui对象的名称
*/
bool SetGUIObjectName(DWORD sceneHandle, DWORD guiHandle, CHAR* NewName)
{
	return globalVgsCore.SetGUIObjectName( sceneHandle,  guiHandle,  NewName)== TRUE ?true:false;
}


/** 设置gui对象选择框是否可见
*/
VOID ShowGUIObjectSelcetionBox(DWORD sceneHandle, DWORD guiHandle, bool bVisible)
{
	globalVgsCore.ShowGUIObjectSelcetionBox( sceneHandle,  guiHandle,  bVisible);
}

/** 获取二维对象选择框是否显示
*/
bool GetGUISelectionBoxVisible(DWORD sceneHandle, DWORD GUIObjectHandle)
{
	return globalVgsCore.GetGUISelectionBoxVisible( sceneHandle,  GUIObjectHandle)== TRUE ?true:false;
}

/** 设置GUI对象的鼠标事件函数名称
*/
VOID SetGuiMouseEvent(DWORD sceneHandle, DWORD guiHandle, VGS_MOUSEEVENT_TYPE type, const CHAR* sEventName)
{
	globalVgsCore.SetGuiMouseEvent( sceneHandle,  guiHandle,  type, sEventName);
}

/** 获取GUI对象的鼠标事件函数名称
*/
bool GetGuiMouseEvent(DWORD sceneHandle, DWORD guiHandle, VGS_MOUSEEVENT_TYPE type, CHAR* sEventName)
{
	return globalVgsCore.GetGuiMouseEvent( sceneHandle,  guiHandle,  type,  sEventName)== TRUE ?true:false;
}

//------------------Flash GUI Start-------------------------------------------------------------------------
DWORD CreateFlashGUIFromFile(DWORD sceneHandle, HWND hParent, const char* sName, const char* sFile, const RECT &rect)
{
	return globalVgsCore.CreateFlashGUIFromFile( sceneHandle,  hParent,   sName,   sFile,  rect);
}
void MoveFlashGUI(DWORD sceneHandle, DWORD guiHandle,INT x, INT y)
{
	globalVgsCore.MoveFlashGUI( sceneHandle,  guiHandle, x,  y);

}
void SetFlashGUIPlayStatus(DWORD sceneHandle, DWORD guiHandle,INT status)
{
	globalVgsCore.SetFlashGUIPlayStatus( sceneHandle,  guiHandle, status);
}
void AddFlashGUISemaphore(DWORD sceneHandle, DWORD guiHandle,const char*  SemaphoreName, const char*  SemaphoreFunc)
{
	globalVgsCore.AddFlashGUISemaphore( sceneHandle,  guiHandle,   SemaphoreName,    SemaphoreFunc);
}
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
DWORD CreateOverlayFromFile(DWORD sceneHandle, const CHAR* name, const CHAR* sFile, const CHAR* sOpcatityImageFile, RECT &rect)
{
	return globalVgsCore.CreateOverlayFromFile( sceneHandle,   name,   sFile,   sOpcatityImageFile,  rect);
}

/** 获取overlay的属性
@param sceneHandle   in  : 场景管理器
@param overlayHandle in  : overlay句柄
@param info          out : Overlay的属性
*/
// DLL_EXPORT BOOL GetOverlayInfo(DWORD sceneHandle, DWORD overlayHandle, OverlayInfo &info);

/**得到Overlay的数量
*/
INT GetOverlayCount(DWORD sceneHandle)
{
	return globalVgsCore.GetOverlayCount( sceneHandle);
}

// 获取所有Overlay的句柄列表
void GetOverlayList(DWORD sceneHandle,DWORD* vHandle)
{
	//考虑去除该接口...
// 	CScene *pScene = (CScene*)sceneHandle;
// 	if (pScene)
// 	{
// 		return pScene->GetGuiManagerRef().GetObjectsList(VGS_GUIOBJ_TYPE_OVERLAY, );
// 	}
	globalVgsCore.GetOverlayList(sceneHandle,vHandle);
}

/**获得Overlay使用的图像文件
@param sBaseFile in : 基本贴图文件
@param sOpacFile in : 透明贴图文件
*/
VOID GetOverlayImageFile(DWORD sceneHandle, DWORD overlayHandle, CHAR* sBaseFile, CHAR* sOpacFile)
{
	globalVgsCore.GetOverlayImageFile( sceneHandle,  overlayHandle,  sBaseFile,  sOpacFile);
}

/**设置Overlay使用的图像文件
@param sBaseFile in : 基本贴图文件
@param sOpacFile in : 透明贴图文件
*/
VOID SetOverlayImageFile(DWORD sceneHandle, DWORD overlayHandle, const CHAR* sBaseFile, const CHAR* sOpacFile/* = NULL*/)
{
	globalVgsCore.SetOverlayImageFile( sceneHandle,  overlayHandle,   sBaseFile,   sOpacFile/* = NULL*/);
}

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
DWORD CreateButtonFromFile(DWORD sceneHandle, const CHAR* sName, const RECT &rect, 
						              const CHAR* mouseUpImageFile,
							          const CHAR* mouseOverImageFile/* = NULL*/,
							          const CHAR* mouseDownImageFile/* = NULL*/)
{
	
	return globalVgsCore.CreateButtonFromFile( sceneHandle,   sName,   rect, 
						               mouseUpImageFile,
							           mouseOverImageFile/* = NULL*/,
							           mouseDownImageFile/* = NULL*/);
}

/** 获得按钮的回调函数名称, 长度不超过MAX_NAMESTR_LENGTH
*/
VOID GetButtonCallBack(DWORD sceneHandle, DWORD buttonHandle, CHAR* callBackFunc)
{
	globalVgsCore.GetButtonCallBack( sceneHandle,  buttonHandle,  callBackFunc);
}

/** 设置按钮的回调函数名称[按钮按下，回调函数], 长度不超过MAX_NAMESTR_LENGTH
*/
VOID SetButtonCallBack(DWORD sceneHandle, DWORD buttonHandle, const CHAR* callBackFunc)
{
	globalVgsCore.SetButtonCallBack( sceneHandle,  buttonHandle,   callBackFunc);
}
/** 获得按钮使用的MouseUp图像文件,也就是正常状态下显示的图像文件
*/
VOID GetButtonMouseUpImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseUpImageFile)
{
	globalVgsCore.GetButtonMouseUpImageFile( sceneHandle,  buttonHandle,  strMouseUpImageFile);
}

/** 设置按钮使用的MouseUp图像文件
*/
VOID SetButtonMouseUpImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseUpImageFile)
{
	globalVgsCore.SetButtonMouseUpImageFile( sceneHandle,  buttonHandle,   strMouseUpImageFile);
}

/** 获得按钮使用的Mouse over(鼠标悬停在按钮上面时)图像文件
*/
VOID GetButtonMouseOverImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseOverImageFile)
{
	globalVgsCore.GetButtonMouseOverImageFile( sceneHandle,  buttonHandle,  strMouseOverImageFile);
}

/** 设置按钮使用的Mouse over(鼠标悬停在按钮上面时)图像文件
*/
VOID SetButtonMouseOverImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseOverImageFile)
{
	globalVgsCore.SetButtonMouseOverImageFile( sceneHandle,  buttonHandle,   strMouseOverImageFile);
}

/** 获得按钮使用的MouseDown(按钮被按下)图像文件
*/
VOID GetButtonMouseDownImageFile(DWORD sceneHandle, DWORD buttonHandle, CHAR* strMouseDownImageFile)
{
	globalVgsCore.GetButtonMouseDownImageFile( sceneHandle,  buttonHandle,  strMouseDownImageFile);
}

/** 设置按钮使用的MouseDown(按钮被按下)图像文件
*/
VOID SetButtonMouseDownImageFile(DWORD sceneHandle, DWORD buttonHandle, const CHAR* strMouseDownImageFile)
{
	globalVgsCore.SetButtonMouseDownImageFile( sceneHandle,  buttonHandle,   strMouseDownImageFile);
}

/** 设置Button是否激活
*/
VOID SetButtonEnable(DWORD sceneHandle, DWORD buttonHandle, bool enable)
{
	globalVgsCore.SetButtonEnable( sceneHandle,  buttonHandle,  enable);
}

/** 判断Button是否激活
*/
bool IsButtonEnable(DWORD sceneHandle, DWORD buttonHandle)
{
	return globalVgsCore.IsButtonEnable( sceneHandle,  buttonHandle)== TRUE ?true:false;
}

//-------------------------------------------------------------------------------------------
/** 导航图相关的方法 */
//-------------------------------------------------------------------------------------------
/**创建导航图, 
@param name        in : 导航图使用的名称
@param strMapFile  in : 导航图使用的地图文件名称，包含完整路径
@param pRect       in : 导航范围，场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
@return 返回导航图对象的句柄或0
*/
DWORD CreateNavigater(DWORD sceneHandle, const CHAR* name, const CHAR* strMapFile, const FRect &pRect)
{
	return globalVgsCore.CreateNavigater( sceneHandle,  name,  strMapFile,  pRect);
}

/** 获得导航图底图
@param strMapFile out : 导航图背景图使用的图片文件，不包含路径
*/
VOID GetNavigaterMap(DWORD sceneHandle, DWORD navHandle, CHAR* strMapFile)
{
	globalVgsCore.GetNavigaterMap( sceneHandle,  navHandle,  strMapFile);
}

/** 设置导航图底图
@param strMapFile in : 导航图背景图使用的图片文件，需要包含完整路径
*/
VOID SetNavigaterMap(DWORD sceneHandle, DWORD navHandle, const CHAR* strMapFile)
{
	globalVgsCore.SetNavigaterMap( sceneHandle,  navHandle,   strMapFile);
}

/** 获得导航图的导航范围坐标
@param pRect out : 场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
*/
VOID GetNavigaterSceneRect(DWORD sceneHandle, DWORD navHandle, FRect &pRect)
{
	globalVgsCore.GetNavigaterSceneRect( sceneHandle,  navHandle,  pRect);
}

/** 设置导航图的导航范围坐标
@param pRect in : 场景顶视图中看到的场景区域，left < right(x方向), top < bottom(z方向)
*/
VOID SetNavigaterSceneRect(DWORD sceneHandle, DWORD navHandle, const FRect &pRect)
{
	globalVgsCore.SetNavigaterSceneRect( sceneHandle,  navHandle,   pRect);
}

/**增加导航点对象
@param  navigaterHandle - 导航图对象的句柄
@param  navPointerName - 导航图点的名称
@param  pointerType - 导航图点的类型，0为相机导航，1为模型导航
@param  pointerHandle - 导航点所导航的对象的句柄，如果导航图点类型为相机，则为相机句柄，否则为模型句柄
@return 成功创建导航点返回TRUE，否则FALSE
*/
bool AddNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle,
									const CHAR* navPointerName,
									const CHAR* strPointerFile, 
									INT	  pointerType/*   = 0*/,	// 0 - camera or 1 - model 
									DWORD pointerHandle/* = 0*/     //camera or model handle
									)
{
	return globalVgsCore.AddNavigaterPointer( sceneHandle,  navigaterHandle,
									 navPointerName,
									 strPointerFile, 
									 pointerType/*   = 0*/,	// 0 - camera or 1 - model 
									 pointerHandle/* = 0*/     //camera or model handle
									)== TRUE ?true:false;
}

/** 使用已有的导航图点创建一个新导航点
@param  navPointerName   in : 要创建的导航点名称
@param  sSrcPointerName   in : 已存在的导航点名称，新创建的点会使用其图片，并且具有相同的导航类型
@return 成功创建导航点返回TRUE，否则FALSE
*/
bool CloneNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* navPointerName, 
							          const CHAR* sSrcPointerName)
{
	return globalVgsCore.CloneNavigaterPointer( sceneHandle,  navigaterHandle,   navPointerName, 
							            sSrcPointerName)== TRUE ?true:false;
}

/** 删除导航点
@param navigaterHandle in : 导航图
@param sName           in : 导航点名称
*/
void DeleteNavigaterPointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName)
{
	globalVgsCore.DeleteNavigaterPointer( sceneHandle,  navigaterHandle,   sName);
}

/** 得到Navigater的导航点数量
@param navigaterHandle in : 导航图对象指针
@return 导航图中的导航点数目
*/
INT GetNavPointerCount(DWORD sceneHandle, DWORD navigaterHandle)
{
	return globalVgsCore.GetNavPointerCount( sceneHandle,  navigaterHandle);
}

/** 根据索引，获取导航图的导航点名称
*/
VOID GetNavPointerNameByIndex(DWORD sceneHandle, DWORD navigaterHandle, INT index, CHAR* sName)
{
	globalVgsCore.GetNavPointerNameByIndex( sceneHandle,  navigaterHandle,  index,  sName);
}

/** 获得导航图点所使用的图片名称，不带路径
@param navigaterHandle in : 导航图对象指针
@param sName           in : 导航点名称
@param strPointerFile  out : 导航点使用的图片名称，不包含路径
*/
VOID GetNavPointerImage(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, CHAR* strPointerFile)
{
	globalVgsCore.GetNavPointerImage( sceneHandle,  navigaterHandle,  sName,  strPointerFile);
}
/** 设置导航图点所使用的图片名称
@param navigaterHandle in : 导航图对象指针
@param sName           in : 导航点名称
@param strPointerFile  in : 导航点使用的图片名称，需要包含完整路径
*/
VOID SetNavPointerImage(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, const CHAR* strPointerFile)
{
	globalVgsCore.SetNavPointerImage( sceneHandle,  navigaterHandle,   sName,   strPointerFile);
}

/** 获得导航点的导航类型和导航对象
*/
bool GetNavPointerObj(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &pointerType, DWORD &pointerHandle)
{
	return globalVgsCore.GetNavPointerObj( sceneHandle,  navigaterHandle,   sName,  pointerType,  pointerHandle)== TRUE ?true:false;
}

/** 设置导航点的导航类型和导航对象
*/
VOID SetNavPointerObj(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT pointerType, DWORD pointerHandle)
{
	globalVgsCore.SetNavPointerObj( sceneHandle,  navigaterHandle,   sName,  pointerType,  pointerHandle);
}

/** 设置导航点是否可见
*/
VOID SetNavPointerVisiblilty(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, bool bVisible)
{
	globalVgsCore.SetNavPointerVisiblilty( sceneHandle,  navigaterHandle,   sName,  bVisible);
}

/** 获取导航点是否可见
*/
bool GetNavPointerVisiblilty(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName)
{
	return globalVgsCore.GetNavPointerVisiblilty( sceneHandle,  navigaterHandle,   sName)== TRUE ?true:false;
}

/** 设置导航点的不透明度 [0, 100]
*/
VOID SetNavPointerOpacity(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT opacity)
{
	globalVgsCore.SetNavPointerOpacity( sceneHandle,  navigaterHandle,   sName,  opacity);
}

/** 获取导航点的不透明度 [0, 100]
*/
INT  GetNavPointerOpacity(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName)
{
	return globalVgsCore.GetNavPointerOpacity( sceneHandle,  navigaterHandle,   sName);
}

/** 设置导航点的宽度和高度，像素数目
*/
VOID SetNavPointerSize(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT width, INT height)
{
	globalVgsCore.SetNavPointerSize( sceneHandle,  navigaterHandle,   sName,  width,  height);
}

/** 获取导航点的宽度和高度，像素数目
*/
bool GetNavPointerSize(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &width, INT &height)
{
	return globalVgsCore.GetNavPointerSize( sceneHandle,  navigaterHandle,  sName,  width,  height)== TRUE ?true:false;
}

/** 设置导航点的位置，像素数目
*/
VOID SetPointerPosition(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT left, INT top)
{
	globalVgsCore.SetPointerPosition( sceneHandle,  navigaterHandle,   sName,  left,  top);
}

/** 获取导航点的位置，像素数目
*/
bool GetPointerPosition(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sName, INT &left, INT &top)
{
	return globalVgsCore.GetPointerPosition( sceneHandle,  navigaterHandle,   sName,  left,  top)== TRUE ?true:false;
}

/** 导航点更名
*/
VOID RenamePointer(DWORD sceneHandle, DWORD navigaterHandle, const CHAR* sOldName, const CHAR* sNewName)
{
	globalVgsCore.RenamePointer( sceneHandle,  navigaterHandle,  sOldName,  sNewName);
}

////////////////// 控制对象 ////////////////////////////


///// 控制对象方法 ////////////////////////////////
// 设置当前控制器
VOID SetCurrentController(DWORD sceneHandle, DWORD ControllerHandle)
{
	globalVgsCore.SetCurrentController( sceneHandle,  ControllerHandle);
}

// 获取当前控制器
DWORD GetCurrentController(DWORD sceneHandle)
{
	return globalVgsCore.GetCurrentController( sceneHandle);
}

// 获取控制对象的类型
CONTROLTYPE GetControllerType(DWORD ControllerHandle)
{
	return globalVgsCore.GetControllerType( ControllerHandle);
}

// 获取当前控制器使用的相机
DWORD GetCurrentControllerCamera(DWORD sceneHandle)
{
	return globalVgsCore.GetCurrentControllerCamera( sceneHandle);
}

// 设置控制器的相机
VOID SetControllerCamera(DWORD sceneHandle, DWORD ControllerHandle, DWORD CameraHandle)
{
	globalVgsCore.SetControllerCamera( sceneHandle,  ControllerHandle,  CameraHandle);
}

// 获取当前控制器的相机
DWORD GetControllerCamera(DWORD sceneHandle, DWORD ControllerHandle)
{
	return globalVgsCore.GetControllerCamera( sceneHandle,  ControllerHandle);
}

//-------------------------------------------------------------------------------------------
//***Walker漫游对象相关的方法
//-------------------------------------------------------------------------------------------

/** 创建Walker漫游对象
param in scneneHandle : 场景指针
param in wInfo : walker的参数
return 如果成功返回walker的指针，否则返回NULL
*/
DWORD CreateWalker(DWORD sceneHandle, WalkerInfo* wInfo)
{
	return globalVgsCore.CreateWalker( sceneHandle,  wInfo);
}

/** 删除Walker漫游对象,如果删除了当前控制前，引擎会自动设置一个当前控制器
*/
VOID DeleteWalkerByHandle(DWORD sceneHandle, DWORD walkerHandle)
{
	globalVgsCore.DeleteWalkerByHandle( sceneHandle,  walkerHandle);
}
VOID DeleteWalkerByName(DWORD sceneHandle, const CHAR* pName)
{
	globalVgsCore.DeleteWalkerByName( sceneHandle,   pName);
}


//得到Walker的数量
INT GetWalkerCount(DWORD sceneHandle)
{
	return globalVgsCore.GetWalkerCount( sceneHandle);
}

//通过编号获得Walker
DWORD GetWalkerHandleByIndex(DWORD sceneHandle, INT index)
{
	return globalVgsCore.GetWalkerHandleByIndex( sceneHandle,  index);
}

//通过name获得Walker
DWORD GetWalkerHandleByName(DWORD sceneHandle, const CHAR* sName)
{
	return globalVgsCore.GetWalkerHandleByName( sceneHandle, sName);
}

//通过句柄获得名称
VOID GetWalkerNameByHandle(DWORD sceneHandle, DWORD walkerHandle, CHAR* name)
{
	globalVgsCore.	GetWalkerNameByHandle( sceneHandle,  walkerHandle,  name);
}

//设置预览状态时行走的walker的属性
VOID SetWalkerProp(DWORD sceneHandle, DWORD walkerHandle, WalkerInfo* wInfo)
{
	globalVgsCore.SetWalkerProp( sceneHandle,  walkerHandle,  wInfo);
}
//从摄像机得到Walker的属性
VOID GetWalkerProp(DWORD sceneHandle, DWORD walkerHandle, WalkerInfo* wInfo)
{
	globalVgsCore.GetWalkerProp( sceneHandle,  walkerHandle,  wInfo);
}


//-------------------------------------------------------------------------------------------
//***Editor控制对象相关的方法
//-------------------------------------------------------------------------------------------
//创建Editor对象
DWORD CreateEditor(DWORD sceneHandle, EditorInfo* esInfo)
{
	return globalVgsCore.CreateEditor( sceneHandle,  esInfo);
}
//删除Editor对象
VOID DeleteEditorbyHandle(DWORD sceneHandle, DWORD editorHandle)
{
	globalVgsCore.DeleteEditorbyHandle( sceneHandle,  editorHandle);
}

VOID DeleteEditorbyName(DWORD sceneHandle, const CHAR* pName)
{
	globalVgsCore.DeleteEditorbyName( sceneHandle,   pName);
}

//得到Editor的数量
INT GetEditorCount(DWORD sceneHandle)
{
	return globalVgsCore.GetEditorCount( sceneHandle);
}
//通过编号获得Walker
DWORD GetEditorHandleByIndex(DWORD sceneHandle, INT index)
{	
	return globalVgsCore.GetEditorHandleByIndex( sceneHandle,  index);
}

//通过名称获得Editor
DWORD GetEditorHandleByName(DWORD sceneHandle, CHAR* name)
{	
	return globalVgsCore.GetEditorHandleByName( sceneHandle,  name);
}

//通过句柄获得名称
VOID GetEditorNameByHandle(DWORD sceneHandle, DWORD editorHandle, CHAR* name)
{
	globalVgsCore.GetEditorNameByHandle( sceneHandle,  editorHandle,  name);
}

//设置预览状态时行走的Editor的属性
VOID SetEditorProp(DWORD sceneHandle, DWORD editorHandle, EditorInfo* eInfo)
{
	globalVgsCore.SetEditorProp( sceneHandle,  editorHandle,  eInfo);
}
//从摄像机得到Editor的属性
VOID GetEditorProp(DWORD sceneHandle, DWORD editorHandle, EditorInfo* eInfo)
{
	globalVgsCore.GetEditorProp( sceneHandle,  editorHandle,  eInfo);
}
/** 设置editor所对的目标点，实际上要围绕该点旋转
@param lookatType in : looat的类型，0：场景中心 1：场景中的一点
*/
VOID SetEditorLookAt(DWORD sceneHandle, DWORD editorHandle,	const VECTOR3 &v3, INT lookatType/* = 0*/)
{
	globalVgsCore.SetEditorLookAt( sceneHandle,  editorHandle,v3,  lookatType/* = 0*/);
}

/** 设置editor所对的模型，实际上要围绕模型包围盒的中心旋转
*/
VOID SetEditorLookAtModel(DWORD sceneHandle, DWORD editorHandle, DWORD modelHandle)
{
	globalVgsCore.SetEditorLookAtModel( sceneHandle,  editorHandle,  modelHandle);
}

// 获取editor控制对象正对的模型名称，如果没有正对模型，返回false
BOOL GetEditorLookAtModelName(DWORD sceneHandle, DWORD editorHandle, CHAR* sName)
{
	return globalVgsCore.GetEditorLookAtModelName( sceneHandle,  editorHandle,  sName);
}

//-------------------------------------------------------------------------------------------
//***Flyer控制对象相关的方法
//-------------------------------------------------------------------------------------------
//创建Flyer对象
DWORD CreateFlyer(DWORD sceneHandle, FlyerInfo* fInfo)
{
	return globalVgsCore.CreateFlyer( sceneHandle,  fInfo);
}
//删除Flyer对象
VOID DeleteFlyerbyHandle(DWORD sceneHandle, DWORD flyerHandle)
{
	globalVgsCore.DeleteFlyerbyHandle( sceneHandle,  flyerHandle);
}
VOID DeleteFlyerbyName(DWORD sceneHandle, const CHAR* pName)
{
	globalVgsCore.DeleteFlyerbyName( sceneHandle,  pName);
}

//得到Flyer的数量
INT GetFlyerCount(DWORD sceneHandle)
{
	return globalVgsCore.GetFlyerCount( sceneHandle);
}

//通过编号获得Flyer
DWORD GetFlyerHandleByIndex(DWORD sceneHandle, INT index)
{
	return globalVgsCore.GetFlyerHandleByIndex( sceneHandle,  index);
}
//通过名称获得Flyer
DWORD GetFlyerHandleByName(DWORD sceneHandle, CHAR* name)
{
	return globalVgsCore.GetFlyerHandleByName( sceneHandle,  name);
}

//通过句柄获得名称
VOID GetFlyerNameByHandle(DWORD sceneHandle, DWORD flyerHandle, CHAR* name)
{
	globalVgsCore.GetFlyerNameByHandle( sceneHandle, flyerHandle,  name);
}

//设置预览状态时行走的Flyer的属性
VOID SetFlyerProp(DWORD sceneHandle, DWORD flyerHandle, FlyerInfo* fInfo)
{
	globalVgsCore.SetFlyerProp( sceneHandle,  flyerHandle,  fInfo);
}
//从摄像机得到Editor的属性
VOID GetFlyerProp(DWORD sceneHandle, DWORD flyerHandle, FlyerInfo* fInfo)
{
	globalVgsCore.GetFlyerProp( sceneHandle,  flyerHandle,  fInfo);
}
///////////////////////////////// 特效 /////////////////////////////////////////

//-------------------------------------------------------------------------------------------
///后期特效处理的相关方法
//-------------------------------------------------------------------------------------------
VOID CreateBloomEffect(DWORD viewportHandle)
{
	globalVgsCore.CreateBloomEffect(viewportHandle);
}
VOID EnableBloomEffect(DWORD viewportHandle, BOOL enabled)
{
	globalVgsCore.EnableBloomEffect(viewportHandle, enabled);
}
VOID DestroyBloomEffect(DWORD viewportHandle)
{
	globalVgsCore.DestroyBloomEffect(viewportHandle);
}

VOID CreateHDREffect(DWORD viewportHandle)
{
	globalVgsCore.CreateHDREffect(viewportHandle);
}
VOID EnableHDREffect(DWORD viewportHandle, BOOL enabled)
{
	globalVgsCore.EnableHDREffect(viewportHandle, enabled);
}
VOID DestroyHDREffect(DWORD viewportHandle)
{
	globalVgsCore.DestroyHDREffect(viewportHandle);
}

//创建动态模糊特效对象
VOID CreateFSMotionBlur(DWORD sceneHandle)
{
	globalVgsCore.CreateFSMotionBlur(sceneHandle);
}
//打开动态模糊特效
VOID	OpenFSMotionBlur(DWORD sceneHandle)
{
	globalVgsCore.OpenFSMotionBlur(sceneHandle);
}
//关闭动态模糊特效
VOID    CloseFSMotionBlur(DWORD sceneHandle)
{
	globalVgsCore.CloseFSMotionBlur(sceneHandle);
}
//设置动态模糊的强度, 0-100
VOID	SetFSMotionBlurPower(DWORD sceneHandle, INT value)
{
	globalVgsCore.SetFSMotionBlurPower(sceneHandle, value);
}

//---------------------------------------------------------------------------------
//创建全屏光晕对象
VOID CreateFSGlow(DWORD sceneHandle)
{
	globalVgsCore.CreateFSGlow(sceneHandle);
}
//打开全屏光晕特效
VOID OpenFSGlow(DWORD sceneHandle)
{
	globalVgsCore.OpenFSGlow(sceneHandle);
}
//关闭全屏光晕特效
VOID CloseFSGlow(DWORD sceneHandle)
{
	globalVgsCore.CloseFSGlow(sceneHandle);
}
//设置全屏光晕强度 0-100
VOID SetFSGlowPower(DWORD sceneHandle, INT value)
{
	globalVgsCore.SetFSGlowPower(sceneHandle, value);
}

//----------------------------------------------------------------------------------
//创建LensFlare镜头光斑对象
VOID	CreateLensFlare(DWORD sceneHandle, VECTOR3* sunPos)
{
	globalVgsCore.CreateLensFlare(sceneHandle, sunPos);
}
//删除镜头光斑对象
VOID		DeleteLensFlare(DWORD sceneHandle)
{
	globalVgsCore.DeleteLensFlare(sceneHandle);
}

/** 创建雾效
@param   fogMode         in : 雾的类型
@param   red/green/blue  in : 雾的颜色
@param   density         in : 雾的密度
@param   start/end       in : 雾的开始结束距离（到相机的距离）
*/
 VOID CreateFog(DWORD sceneHandle, INT fogMode, INT red, INT green, INT blue, FLOAT density, FLOAT start, FLOAT end)
 {
	globalVgsCore.CreateFog(sceneHandle, fogMode, red, green, blue, density, start, end);
 }

////////////////////////////////// 特效结束 ////////////////////////////////////////

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
DWORD GetAnimationCount(DWORD sceneHandle, VGS_ANIMATIONTYPE type)
{
	return globalVgsCore.GetAnimationCount( sceneHandle,  type);
}

/** 根据索引得到动画的名称
@param in type  : VAT_KEYFRM：关键帧动画，VAT_CAMERA：相机动画，VAT_LIGHT：灯光动画, VAT_SKELETON:骨骼动画
@param in index : 动画索引
@param out sName : 名称
@return 得到名称，返回true，否则false
*/
bool GetAnimationNameByIndex(DWORD sceneHandle, VGS_ANIMATIONTYPE type, int index, CHAR* sName)
{
	return globalVgsCore.GetAnimationNameByIndex( sceneHandle,  type,  index,  sName)== TRUE ?true:false;
}

/** 播放指定名称的动画
*/
VOID PlayAnimation(DWORD sceneHandle, const CHAR* sAnimName)
{
	globalVgsCore.PlayAnimation( sceneHandle, sAnimName);
}

/** 播放所有动画
*/
VOID PlayAllAnimation(DWORD sceneHandle)
{
	globalVgsCore.PlayAllAnimation( sceneHandle);
}

/** 暂停指定名称的动画
*/
VOID PauseAnimation(DWORD sceneHandle, const CHAR* sAnimName)
{		
	globalVgsCore.PauseAnimation( sceneHandle,   sAnimName);
}
/** 暂停所有动画
*/
VOID PauseAllAnimation(DWORD sceneHandle)
{
	globalVgsCore.PauseAllAnimation( sceneHandle);
}

/** 停止指定名称的动画
*/
VOID StopAnimation(DWORD sceneHandle, const CHAR* sAnimName)
{	
	globalVgsCore.StopAnimation( sceneHandle,  sAnimName);
}
/** 停止所有动画
*/
VOID StopAllAnimation(DWORD sceneHandle)
{
	globalVgsCore.StopAllAnimation( sceneHandle);
}

/** 得到关键帧动画的帧数
*/
INT GetKeyframeCount(DWORD sceneHandle, const CHAR* sAnimName)
{
	return globalVgsCore.GetKeyframeCount( sceneHandle,  sAnimName);
}

/** 跳到某一帧
*/
VOID GotoFrame(DWORD sceneHandle, const CHAR* sAnimName, INT FrameIndex)
{
	globalVgsCore.GotoFrame( sceneHandle,  sAnimName,  FrameIndex);
}

/** 得到当前正在播放的帧
*/
INT GetCurrentFrame(DWORD sceneHandle, const CHAR* sAnimName)
{
	return globalVgsCore.GetCurrentFrame( sceneHandle,  sAnimName);
}

/** 设置所有关键帧动画播放的帧率
*/
VOID SetAllKfrPlaySpeed(DWORD sceneHandle, INT FPS)
{
	globalVgsCore.SetAllKfrPlaySpeed( sceneHandle,  FPS);
}

/** 设置指定关键帧动画播放的帧率
*/
VOID SetKfrPlaySpeed(DWORD sceneHandle, const CHAR* sAnimName, INT FPS)
{
	globalVgsCore.SetKfrPlaySpeed( sceneHandle,  sAnimName,  FPS);
}

/** 获得关键帧动画播放的帧率
*/ 
INT GetKfrPlaySpeed(DWORD sceneHandle, const CHAR* sAnimName)
{
	return globalVgsCore.GetKfrPlaySpeed( sceneHandle,  sAnimName);
}

/** 获取动画FPS的缺省值
*/
INT GetDefaultFPS()
{
	return globalVgsCore.GetDefaultFPS();
}

/** 获取动画播是否循环播放
*/
BOOL GetAnimationLoop(DWORD sceneHandle, const CHAR* sAnimName)
{
	return globalVgsCore.GetAnimationLoop( sceneHandle,  sAnimName);
} 

/** 设置动画播是否循环播放
*/
VOID SetAnimationLoop(DWORD sceneHandle, const CHAR* sAnimName, BOOL bLoop)
{
	globalVgsCore.SetAnimationLoop( sceneHandle,  sAnimName,  bLoop);
}

///////////////////////// math functions /////////////////////////////////
/** 根据法线和点创建一个平面
@param vNormal in ：屏幕的法线
@param point   in : 平面上任意一点
@return : 平面的指针
*/
DWORD CreatePlane(const VECTOR3 &vNormal, const VECTOR3 &point)
{
	return globalVgsCore.CreatePlane(vNormal,  point);
}

/** 销毁一个平面
*/ 
VOID DestroyPlane(const DWORD hPlane)
{
	globalVgsCore.DestroyPlane(hPlane);
}

/** 计算一点到一个平面的距离
@param hPlane  in ：平面对象指针
@param point   in : 空间中的任意一点
@param dist    out：点point到平面hPlane的距离，如果point位于hPlane的法线正方向，返回值>0,如果在负方向上，返回值<0,如果点在平面上，返回值=0
@return : 正确得到结果，返回true，否则false
*/
BOOL GetDistance(const DWORD hPlane, const VECTOR3 &point, FLOAT &dist)
{
	return globalVgsCore.GetDistance( hPlane,  point, dist);
}

/** 计算一个轴方向，经过一个四元数作用后的新的方向
@param quat     in  : 旋转四元数
@param vOrigDir in  : 初始的方向
@param vNewDir  out : vOrigDir经过quat作用后的新的方向
*/
VOID RotationAxis(const VGS_QUATERNION &quat, const VECTOR3 &vOrigDir, VECTOR3 &vNewDir)
{
	//MessageBox(NULL, "暂时没有实现", "提示", 0);
}
////////////////// 消息响应 /////////////////////////

/** 消息响应
@param sceneHandle in : 场景句柄
@param DWORD in : 渲染窗口句柄。设为NULL，表示对所有渲染窗口。
@param uMsg in : 消息ID，可参考windows标准消息说明
@param wParam/lParam  in : 消息参数，可参考windows标准消息说明
*/
VOID VGSMsgProc(DWORD sceneHandle, DWORD hRenderWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	globalVgsCore.VGSMsgProc( sceneHandle,  hRenderWnd,  uMsg,  wParam,  lParam);
}

//////////////////// 其他 ////////////////////////////////
/// 获取最近一次错误码
DWORD GetVGSLastErrCode()
{
	return globalVgsCore.GetVGSLastErrCode();
}

/// 获取最近一次错误描述
VOID GetVGSLastErrStr(char *sErrStr)
{
	globalVgsCore.GetVGSLastErrStr(sErrStr);
}

/// 得到错误码对应的详细描述
VOID GetVGSErrStr(DWORD Code, char *sErrStr)
{
	globalVgsCore.GetVGSErrStr( Code,  sErrStr);
}


///////////////////坐标轴相关——Start/////////////////////////////////////////////////////////
void SelectModelAxis(DWORD sceneHandle, DWORD modelHandle, UINT selectStatus)
{
	globalVgsCore.SelectModelAxis( sceneHandle,  modelHandle,  selectStatus);
}
void SetShowModelAxis(DWORD sceneHandle, DWORD modelHandle, bool bShow)
{
	globalVgsCore.SetShowModelAxis( sceneHandle,  modelHandle,  bShow);
}
void SetModelAxisStatus(DWORD sceneHandle, DWORD modelHandle, int nStatus)
{
	globalVgsCore.SetModelAxisStatus( sceneHandle,  modelHandle,  nStatus);
}
int GetModelAxisStatusUnderCursor(DWORD sceneHandle, DWORD modelHandle, DWORD hViewport, UINT nScreenX, UINT nScreenY)
{
	return globalVgsCore.GetModelAxisStatusUnderCursor( sceneHandle,  modelHandle,  hViewport,  nScreenX,  nScreenY);
}

///////////////////坐标轴相关——End/////////////////////////////////////////////////////////
