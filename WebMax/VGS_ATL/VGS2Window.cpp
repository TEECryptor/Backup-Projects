
#include "stdafx.h"

#include "VGS2Window.h"
#include "VGS2.h"

#include "VGSTypedef.h"
#include "..\\VGSCoreV3\\VGSDef.h"
#include "..\\VGSCoreV3\\vgsConstDef.h"

#include "vgszip/vgszip.h"

// download file
#include "Download/VGSFileDownloadTaskIF.h"
#include "Download/VGSN3DFileDownloadStreamTask.h"

#include <algorithm>
using namespace std;


DWORD lastFrameTimer; 
CVGS2* g_pVGS2 = NULL;   

// js 回调函数的名称
const std::string sSceneIsDown     = "SceneIsDown";
const std::string sAllIsDown       = "AllIsDown";
const std::string sVGSMouseDown    = "VGSMouseDown";
const std::string sVGSMouseDBClick = "VGSMouseDBClick";
const std::string sVGSMouseUp      = "VGSMouseUp";
const std::string sVGSMouseMove    = "VGSMouseMove";
const std::string sOnDownload      = "OnDownload";
const std::string sFlashPlayOver   = "FlashPlayOver";
const std::string sFlashLoadOver   = "FlashLoadOver";
const std::string sStartEngine     = "OnVGSStart"; //"SetEngineStart";  // 启动引擎的回调函数，新模板中改名以兼容旧脚本

// 非ie浏览器
#define _NOT_MSIE(wbtype) (wbtype != WBT_IE)

// 在字符串前后添加引号
std::string _addQuotes(const std::string &str)
{
	std::string strDes = "\"";
	strDes.append(str).append("\"");
	return strDes;
}

//----------------------------------------------------------------------
//构造函数
//----------------------------------------------------------------------
CVGS2Window::CVGS2Window(IOleClientSite* pClientSite, 
						 CRITICAL_SECTION* pRenderCS, 
						 CRITICAL_SECTION* pCtrlCS)
{
	
 	m_pClientSite = pClientSite;
 	
 	m_pRender_cs = pRenderCS;
 	m_pCtrl_cs = pCtrlCS;
	m_bN3DFileDownloaded = FALSE;
// 	
// 	m_pButton = NULL;
// 	m_pOverlay = NULL;
// 	invokeJS = NULL;
// 	
// 	m_pCurCursorModel = NULL;
// 	m_pLastCursorModel = NULL;
// 	
// 	m_pLastSelectedModel = NULL;
// 	m_pCurSelectedModel = NULL;
// 	
// 	bEnable_MouseDown_Callback = FALSE;
// 	bEnable_MouseUp_Callback = FALSE;
// 	bEnable_MouseMove_Callback = FALSE;
// 	bEnable_MouseDBClick_Callback = FALSE;
// 	
// 	m_bMouseDownEvent_Finished = TRUE;
// 	m_bMouseUpEvent_Finished = TRUE;
// 	m_bMouseMoveEvent_Finished = TRUE;
// 	m_bMouseDBClickEvent_Finished = TRUE;
// 	
 	m_bActive = TRUE; 
// 
// 	m_bRightMenu = FALSE;
// 
// 	hPopMenu  = NULL;
//     hMainMenu = NULL;
// 	
 	m_downText = 0;
// 	m_bTerminated = false;
// 	m_bStepFinish = TRUE;
}

CVGS2Window::~CVGS2Window()
{
	m_DownloadFileManagerMT.ClearAllTask();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//方法: OnInitDialog()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: VGS2窗口初始化
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//得到IE窗体
	m_IEhWnd = GetTopLevelWindow(); 
	m_VGSWnd = GetForegroundWindow();
	
	//初始化变量
	InitVars();
	
	//初始化VGS场景
	if (FAILED(InitScene())) 
		return E_FAIL;
	
	//初始化其它对象
	InitOther();
	
	//有关JavaScript的调用对象
	invokeJS = new CInvokeJSFunc(m_pClientSite);
	if (invokeJS == NULL)
		return E_FAIL;
	
	//设置时钟
	SetTimer(1, 10);

	//每35ms播放一帧动画，时间可能会修改 = 1.0 / m_kfrPlayspeed;
	UINT time1Span = UINT(1000.0 / (float)gkfrPlaySpeed + 0.5);
	SetTimer(2, (UINT)time1Span); 

	//本窗体接收下载消息
	m_DownloadFileManagerMT.SetMsgWndHandle(m_hWnd);

	return S_OK; 
	
}
//-------------------------------------------------------------------
VOID CVGS2Window::SetVGS2Interface(CVGS2 *pVGS2)
{
	g_pVGS2 = pVGS2; 
}
//-------------------------------------------------------------------
VOID CVGS2Window::SetVGSCore(CVGS2DLL *pVGSCore)
{
	m_pVGSCore = pVGSCore;
	m_DownloadFileManagerMT.SetVGSCore(pVGSCore);
}
//-------------------------------------------------------------------
//方法: OnDestroy()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 释放场景资源
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
	m_FileLoadManagerMT.Release();	

	//防止在创建MovieTex时删除
// 	EnterCriticalSection(m_pCtrl_cs);
// 	EnterCriticalSection(m_pRender_cs);
// 	
//	m_vDownloadingN3DFile.ClearAll();
// 	//释放场景中的N3D对象和正在下载的N3D对象
// 	DestroyN3D();
// 	//释放场景中的N3D对象和正在下载的N3D对象
// 	DestroyDownloadFile();	
// 	//删除startLogo
// 	m_pVGSCore->DeleteLogo();
// 	
// 	//释放VGS场景对象
// 	if (m_pVGSCore->vgsScene != NULL) 
// 		m_pVGSCore->SunDeleteScene();
// 	
// 	//清空场景资源列表
// 	m_pVGSCore->sceneResource.clear();
// 
// 	//释放Flash对象
// 	DestroyFlash();
// 	//释放InvokeJS对象
// 	SAFE_DELETE(invokeJS);
// 	
// 	//删除音频对象序列
// 	m_pVGSCore->DeleteAllAudio();
// 	
// 	//删除Timer
// 	KillTimer(1);
// 	
// 	LeaveCriticalSection(m_pRender_cs);
// 	LeaveCriticalSection(m_pCtrl_cs);
// 	
	return S_OK;
}
//-------------------------------------------------------------------
//方法: OnSize()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 窗口尺寸变化时的方法
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
 	// if (m_pVGSCore->vgsScene == NULL) return S_OK;
 	if (m_bActive == FALSE) return S_OK;
 	
 	/*RECT clientRect;
 	GetClientRect(&clientRect);
     
 	m_ClientRect=clientRect;*/
 	
 	//模型还没有完全创建到场景时...，需要与创建场景线程同步
 	EnterCriticalSection(m_pCtrl_cs);//---
 	EnterCriticalSection(m_pRender_cs);//---
 	
 	//更新场景模型的顶点缓冲
 	if (!m_pVGSCore->SunUpdateBackBuffer())
 	{
 		LeaveCriticalSection(m_pRender_cs);//---
 		LeaveCriticalSection(m_pCtrl_cs);//---
 		return S_OK;
 	}
 	
 	//初始化控制按钮
 	//InitControlButton();
 	
 	LeaveCriticalSection(m_pRender_cs);//---
 	LeaveCriticalSection(m_pCtrl_cs);//---
 	
 	//更新Flash的Rect
 	//UpdateFlashRect();
	
	return S_OK;
}
//-------------------------------------------------------------------
// 回调js脚本入口函数，开始执行脚本
//-------------------------------------------------------------------
VOID CVGS2Window::EnableStartEngineCall()
{
	SetTimer(3, 100);  // 100ms后回调js脚本
}
//-------------------------------------------------------------------
//方法: OnTimer()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: Timer方法
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pVGSCore->m_vgsScene == NULL) return E_FAIL;

	if (wParam == 1)  // 渲染线程定时器
	{
		if ((g_BrowserType == WBT_IE && g_BrowserSubType == WBST_MAXTHON)) // 如果是maxthon，始终渲染
		{
			m_pVGSCore->SunSetRenderStatus(1);
			m_bActive = TRUE;
		}
		else
		{
			//判断每个N3D是否已经创建场景		
			HWND hWnd = GetForegroundWindow();			
			if (hWnd == m_VGSWnd || hWnd == m_IEhWnd || IsFlashWnd(hWnd))  // 如果是当前窗体就渲染
			{
				if (m_bActive == FALSE)
				{
					//初始化控制按钮
					//InitControlButton();
					m_pVGSCore->SunSetRenderStatus(1);
					m_bActive = TRUE;

					// fuxb 2009-05-15 ==>
					//m_VGS2.vgsScene->RestoreDeviceObject();
					// <== 
				}
			}
			else
			{
				if (m_bActive == TRUE)
				{
					m_bActive = FALSE;
					//VGS窗体失去焦点，其它窗体被激活，从激活状态变为非激活状态，DeActive
					m_pVGSCore->SunSetRenderStatus(0);
				}
			}
		}

		//------------------------------------------------------------------
		//进入渲染保留区************************
		EnterCriticalSection(m_pRender_cs);

		//渲染N3D场景
		if (m_pVGSCore->m_bRender)
			m_pVGSCore->SunRenderScene();

		//速率控制
		//BOOL EventCtrl = TRUE;

//		if (GetTickCount() - lastFrameTimer >= m_UserEventTimeStep)
//		{
//			//显示下载的百分比
//			//SetDownPercentText();
//
//			lastFrameTimer = GetTickCount();
//			//EventCtrl = TRUE;
//
//			//自定义鼠标移动消息, Player静止状态有效
//			if (m_VGS2.rosDis < 0.00001f && m_VGS2.moveStep < 0.00001f)
//				UserMouseMoveEvent();
//
//			UpdateFlashInfo();
//
//			//和模型碰撞触发的消息
//			//	ModelCollisionInvoke();
//
//			//Player和地面模型发生踩踏后的Invoke方法
//			//	PlayerOnGroundInvoke();
//
//			//显示调试信息-----------------------------------------------------------
//#ifdef _DEBUG
//			ShowSceneDetailInfo();	
//#endif
//
//		}

		//离开保留区
		LeaveCriticalSection(m_pRender_cs);

		//Flash播放完毕消息
		//FlashPlayOverEvent();
		//Flash下载完毕消息
		//FlashLoadOverEvent();

		//鼠标滚轮继续阻尼运动
		/*if(m_VGS2.editor.dollyValue != 0)
		{
			if (fabs(m_VGS2.editor.dollyValue) < 0.001f)
				m_VGS2.editor.dollyValue = 0;

			D3DXVECTOR3 from = m_VGS2.vgsScene->currentCam->GetFrom();
			D3DXVECTOR3 at = m_VGS2.vgsScene->currentCam->GetAt();
			D3DXVECTOR3 dis = at - from;
			FLOAT len = D3DXVec3Length(&dis);
			FLOAT dollyNearDis = m_VGS2.editor.dollyNearDis;
			FLOAT dollyFarDis = m_VGS2.editor.dollyFarDis;;
			FLOAT dollyValue = -m_VGS2.editor.dollyValue;

			if (len > dollyNearDis + dollyValue)
			{
				if (len < dollyFarDis + dollyValue)
				{
					m_VGS2.vgsScene->currentCam->Dolly(dollyValue);
					m_VGS2.editor.dollyValue *= 0.82f;
				}
			}
		}*/

		// fuxb 2009-05-15 ==>
		//		if (!m_VGS2.vgsScene->renderScene)
		// 		{
		// 			m_VGS2.vgsScene->InvalidateDeviceObject();
		// 		}
		// <==

	} // 定时器1
	else if (wParam == 2)  // 定时器2，负责动画播放速度
	{
		//if (m_VGS2.vgsScene->m_bKfrPlaying)
		//{
			//m_VGS2.vgsScene->stepFrame(1);
		//}
		m_pVGSCore->PlayKeyframe();
	}
	else if (wParam == 3)  // 回调脚本入口函数
	{
		KillTimer(3);
		Sleep(0);

		InvokeStartEngine();  // 回调
	}

	return S_OK;
}
//-------------------------------------------------------------------
//方法: OnLButtonDown()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 鼠标左键按下方法
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
// 	if (m_pVGSCore->vgsScene == NULL) return E_FAIL;
// 	if (!m_bActive) return E_FAIL;
// 	
// 	//记录鼠标按下的位置
// 	INT x = m_pVGSCore->vgsScene->lastClickPos.x = LOWORD(lParam);
// 	INT y = m_pVGSCore->vgsScene->lastClickPos.y = HIWORD(lParam);
// 	
// 	if (!m_bMouseDownEvent_Finished) return E_FAIL;
// 	
// 	m_bMouseDownEvent_Finished = FALSE;
// 	
// 	EnterCriticalSection(m_pCtrl_cs);
// 	
// 	//------------------------------------------------------------------
// 	//通过Overlay的方向键控制场景, 第0个Overlay就是控制方向键Overlay, 
// 	//只需要在鼠标点击时计算一次
// 	//------------------------------------------------------------------
// 	DWORD overlayHandle = 0;
// 	if ( m_pVGSCore->vgsScene->InputDevice.mouseInfo.LButton == FALSE)
// 		overlayHandle = m_pVGSCore->SunGetOverlayUnderLoc(m_pVGSCore->vgsScene->lastClickPos.x,
// 		m_pVGSCore->vgsScene->lastClickPos.y);
// 	
// 	m_pOverlay = (COverlay*)overlayHandle;
// 	if (m_pOverlay != NULL)
// 	{
// 		if (m_pOverlay == m_pVGSCore->vgsScene->pCtrlOl)
// 		{
// 			//转化为相对坐标
// 			INT x, y;
// 			RECT overlayRect;
// 			m_pVGSCore->SunGetOverlayRect(overlayHandle, &overlayRect);
// 			x = m_pVGSCore->vgsScene->lastClickPos.x - overlayRect.left;
// 			y = m_pVGSCore->vgsScene->lastClickPos.y - overlayRect.top; 
// 			
// 			//看看是否按在UP键上
// 			if (x > 50 && x < 78 && y > 23 && y < 45)
// 				m_pVGSCore->vgsScene->softKey[0] = TRUE;
// 			//看看是否在Down键上
// 			if (x > 50 && x < 78 && y > 79 && y < overlayRect.bottom)
// 				m_pVGSCore->vgsScene->softKey[1] = TRUE;
// 			//看看是否在Left键上
// 			if (x > 23 && x < 46 && y > 50 && y < 76)
// 				m_pVGSCore->vgsScene->softKey[2] = TRUE;
// 			//看看是否在Right键上
// 			if (x > 80 && x < overlayRect.right && y > 50 && y < 76)
// 				m_pVGSCore->vgsScene->softKey[3] = TRUE;
// 		}
// 		
// 		//如果鼠标点中导航指针
// 		CNavigater** ppNav = m_pVGSCore->vgsScene->navigater.begin();
// 		INT navCnt = m_pVGSCore->vgsScene->navigater.size();
// 		for (INT i = 0; i< navCnt; i++)
// 		{
// 			CNavigater* pNav = *(ppNav + i);
// 			
// 			NavPointer** ppNavPointer = pNav->m_NavPointer.begin();
// 			INT navPointerCnt = pNav->m_NavPointer.size();
// 			for (INT j = 0; j< navPointerCnt; j++)
// 			{
// 				NavPointer* pNavPointer = *(ppNavPointer + j);
// 				if (m_pOverlay == pNavPointer->pPointerOverlay)
// 				{
// 					//如果是相机Pointer
// 					if (pNavPointer->NavPointerType == 0)
// 					{
// 						m_pSelectedNavPointer = pNavPointer;
// 						m_pSelectedNavigater = pNav;
// 					}
// 					else
// 					{
// 						m_pSelectedNavPointer = NULL;
// 						m_pSelectedNavigater = NULL;
// 					}
// 				}
// 			}
// 			
// 			/*
// 			//如果在map上点击鼠标
// 			if (m_pOverlay == pNav->pNavigateMap)
// 			{
// 			m_pSelectedNavigater = pNav;
// 			m_bNavPointerSelected = true;
// 			break;
// 			}
// 			*/
// 		}
// 		
// 		/*
// 		//点中nav Pointer，并且可以通过Pointer导航
// 		if (m_pSelectedNavigater && m_pSelectedNavigater->m_bCanNavByPointer)
// 		{
// 		RECT rect;
// 		m_pSelectedNavigater->pNavigatePointer->GetRect(&rect);
// 		INT width = rect.right - rect.left;
// 		INT height = rect.bottom - rect.top;
// 		RECT rect_1;
// 		rect_1.left = x - width/2;
// 		rect_1.right = x + width/2;
// 		rect_1.top = y - height/2;
// 		rect_1.bottom = y + height/2;
// 		m_pSelectedNavigater->pNavigatePointer->SetRect(&rect_1);
// 		m_pSelectedNavigater->UpdateCameraPosition();
// 		}
// 		*/
// 	}	
// 	
// 	
// 	
// 	//-------------------------------------------------------------
// 	//鼠标按下按钮，按钮变化
// 	//-------------------------------------------------------------
// 	//改变button的材质为mouseDown材质
// 	if (m_pVGSCore->vgsScene->pMouseOverButton)
// 	{
// 		if (m_pVGSCore->vgsScene->pMouseOverButton->pMtrl_MouseDown)
// 			m_pVGSCore->vgsScene->pMouseOverButton->btnOverlay->m_pMtrl = 
// 			m_pVGSCore->vgsScene->pMouseOverButton->pMtrl_MouseDown;
// 		else
// 			m_pVGSCore->vgsScene->pMouseOverButton->btnOverlay->m_pMtrl = 
// 			m_pVGSCore->vgsScene->pMouseOverButton->pMtrl_MouseUp;
// 	}
// 	m_pVGSCore->vgsScene->InputDevice.mouseInfo.LButton = TRUE;
// 	
// 	//------------------------------------------------------------------------
// 	//执行Button里的设置的callBack函数
// 	//------------------------------------------------------------------------
// 	ButtonClickInvoke();
// 	
// 	//鼠标点击后停止自动旋转
// 	m_pVGSCore->editor.bIsAutoRoundRoll = false;
// 	m_pVGSCore->editor.autoRoundSpeed = 0;
// 	m_pVGSCore->editor.autoRollSpeed = 0;
// 	
// 	
// 	//-------------------------------------------------------------
// 	//鼠标选中模型，模型材质变化
// 	//-------------------------------------------------------------
// 	DWORD modelHandle = m_pVGSCore->SunGetModelUnderLoc(m_pVGSCore->vgsScene->InputDevice.mouseInfo.X, m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y);
// 	m_pCurSelectedModel = (CModel*)modelHandle;
// 	
// 	//Add by Baojiahui  5/12/2007-------------------------------------------------------------------------------------
//     //当场景处于编辑状态时，鼠标按下选择物体，按住shift可以多选物体
// 	//-----------------------------------------------------------------------------------------------------
//     BOOL modelSelected= FALSE;
// 	
// 	if(m_pVGSCore->vgsScene->sceneEditMode!=0)
// 	{
// 		if(m_pVGSCore->vgsScene->m_ModelEdit==false)
// 		{
// 			if(m_pCurSelectedModel)
// 			{
// 				if (m_pVGSCore->GetShiftStatus())
// 				{
// 					
// 					INT modelCnt = m_pVGSCore->vgsScene->m_SelectedModel.size();
// 					CModel** ppSelectModel = m_pVGSCore->vgsScene->m_SelectedModel.begin();
// 					for (INT i = 0; i< modelCnt; i++)
// 					{
// 						CModel* pSelectModel = *(ppSelectModel + i);
// 						if (pSelectModel==m_pCurSelectedModel)
// 						{
// 							//隐藏物体的包围盒
// 							pSelectModel->showRBBox=false;
// 							m_pVGSCore->vgsScene->m_SelectedModel.erase(ppSelectModel + i);
// 							modelSelected=TRUE;
// 							break;
// 						}						
// 					}				
// 					//已经选中的模型没有当前选择的模型
// 					if(!modelSelected)
// 					{						
// 						m_pVGSCore->vgsScene->m_SelectedModel.push_back(m_pCurSelectedModel);
// 						m_pCurSelectedModel->showRBBox=true;		
// 					}
// 					
// 					D3DXVECTOR3 pVec=D3DXVECTOR3(0.0f,0.0f,0.0f);
// 					
// 					modelCnt = m_pVGSCore->vgsScene->m_SelectedModel.size();
// 					//设置editAxis的位置
// 					if(modelCnt>0)
// 					{
// 						ppSelectModel = m_pVGSCore->vgsScene->m_SelectedModel.begin();
// 						for (i = 0; i< modelCnt; i++)
// 						{
// 							CModel* pSelectModel = *(ppSelectModel + i);
// 							//隐藏物体的包围盒
// 							pVec.x=pVec.x+pSelectModel->pNode->worldPosition.x;
// 							pVec.y=pVec.y+pSelectModel->pNode->worldPosition.y;
// 							pVec.z=pVec.z+pSelectModel->pNode->worldPosition.z;
// 						}
// 
// 						pVec=pVec/(float)modelCnt;
// 						
// 						m_pVGSCore->vgsScene->editAxis->SetPosition(&pVec);
// 					}
// 					
// 				}else
// 				{
// 					//删除所有被选中的模型
// 					INT modelCnt = m_pVGSCore->vgsScene->m_SelectedModel.size();
// 					if (modelCnt >0)
// 					{
// 						CModel** ppSelectModel = m_pVGSCore->vgsScene->m_SelectedModel.begin();
// 						for (INT i = 0; i< modelCnt; i++)
// 						{
// 							CModel* pSelectModel = *(ppSelectModel + i);
// 							//隐藏物体的包围盒
// 							pSelectModel->showRBBox=false;
// 						}
// 						m_pVGSCore->vgsScene->m_SelectedModel.clear();
// 					}	
// 						//将当前选择的模型添加到选择物体队列中
// 						m_pVGSCore->vgsScene->m_SelectedModel.push_back(m_pCurSelectedModel);
// 						m_pCurSelectedModel->showRBBox=true;
// 						m_pVGSCore->vgsScene->editAxis->SetPosition(&m_pCurSelectedModel->pNode->worldPosition);
// 					
// 				}	
// 			}else
// 			{
// 				//删除所有被选中的模型
// 				INT modelCnt = m_pVGSCore->vgsScene->m_SelectedModel.size();
// 				if (modelCnt >0)
// 				{
// 					CModel** ppSelectModel = m_pVGSCore->vgsScene->m_SelectedModel.begin();
// 					for (INT i = 0; i< modelCnt; i++)
// 					{
// 						CModel* pSelectModel = *(ppSelectModel + i);
// 						//隐藏物体的包围盒
// 						pSelectModel->showRBBox=false;
// 					}
// 					
// 					m_pVGSCore->vgsScene->m_SelectedModel.clear();			
// 					
// 				}
// 				
// 			}
// 		}
// 	}else{
// 		//当场景处于非编辑状态的时候处理
// 		//-----------------------------------------------------------------------------------------------------
// 		//复位前一个选中模型的材质--前一次
// 		if (m_pLastSelectedModel &&
// 			m_pLastSelectedModel != m_pCurSelectedModel && 
// 			m_pLastSelectedModel->mouseInOutable )
// 		{
// 			//改变模型的mouseUp材质
// 			if (m_pLastSelectedModel->bMouseShader)
// 			{
// 				//如果以[模型]为单元响应鼠标事件
// 				if (!m_pLastSelectedModel->bmouseHintByGroup)
// 				{
// 			//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 					//		m_pVGSCore->SunSetModelMtrl((DWORD)m_pLastSelectedModel,
// 			//			(DWORD)(m_pLastSelectedModel->mouseUpShader));
// 					//-------------------------------------------------------------------------------
// 				}
// 				else //以[模型组]为单元响应鼠标事件
// 				{
// 					if (m_pLastSelectedModel->modelGroup)
// 					{
// 						CModelGroup* pModelGroup = (CModelGroup*)(m_pLastSelectedModel->modelGroup);
// 						
// 						INT modelCnt = pModelGroup->model.size();
// 						CModel** ppModel = pModelGroup->model.begin();
// 						for (INT i = 0; i<modelCnt; i++)
// 						{
// 							CModel* pModel = *(ppModel + i);
// 							//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 							//m_pVGSCore->SunSetModelMtrl((DWORD)pModel,
// 							//	(DWORD)(pModel->mouseUpShader));
// 							//---------------------------------------------------------------------------------------
// 						}
// 					}
// 				}
// 			}
// 			
// 			//改变模型的mouseUp顶点颜色
// 			if (m_pLastSelectedModel->bMouseColor)
// 			{
// 				//如果以[模型]为单元响应鼠标事件
// 				if (!m_pLastSelectedModel->bmouseHintByGroup)
// 				{
// 					INT alpha = (m_pLastSelectedModel->mouseUpColor >> 24) & 0xff;
// 					INT red = (m_pLastSelectedModel->mouseUpColor >> 16) & 0xff;
// 					INT green = (m_pLastSelectedModel->mouseUpColor >> 8) & 0xff;
// 					INT blue = (m_pLastSelectedModel->mouseUpColor) & 0xff;
// 					m_pVGSCore->SetModelColor((DWORD)m_pLastSelectedModel, red, green, blue, alpha);
// 				}
// 				else //以[模型组]为单元响应鼠标事件
// 				{
// 					if (m_pLastSelectedModel->modelGroup)
// 					{
// 						CModelGroup* pModelGroup = (CModelGroup*)(m_pLastSelectedModel->modelGroup);
// 						
// 						INT modelCnt = pModelGroup->model.size();
// 						CModel** ppModel = pModelGroup->model.begin();
// 						for (INT i = 0; i<modelCnt; i++)
// 						{
// 							CModel* pModel = *(ppModel + i);
// 							INT alpha = (pModel->mouseUpColor >> 24) & 0xff;
// 							INT red = (pModel->mouseUpColor >> 16) & 0xff;
// 							INT green = (pModel->mouseUpColor >> 8) & 0xff;
// 							INT blue = (pModel->mouseUpColor) & 0xff;
// 							m_pVGSCore->SetModelColor((DWORD)pModel, red, green, blue, alpha);
// 						}
// 					}
// 				}
// 			}
// 			
// 			m_pLastSelectedModel->bSelected = FALSE;
// 		}
// 		
// 		//改变当前选中模型的材质
// 		if (m_pCurSelectedModel && m_pCurSelectedModel->mouseInOutable)
// 		{
// 			if (m_pCurSelectedModel->bMouseShader)
// 			{
// 				//如果以[模型]为单元响应鼠标事件
// 				if (!m_pLastSelectedModel->bmouseHintByGroup)
// 				{
// 					//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 					//m_pVGSCore->SunSetModelMtrl((DWORD)m_pCurSelectedModel, 
// 					//	(DWORD)(m_pCurSelectedModel->mouseClickedShader));
// 					//---------------------------------------------------------------------------------------
// 				}
// 				else //以[模型组]为单元响应鼠标事件
// 				{
// 					if (m_pCurSelectedModel->modelGroup)
// 					{
// 						CModelGroup* pModelGroup = (CModelGroup*)(m_pCurSelectedModel->modelGroup);
// 						
// 						INT modelCnt = pModelGroup->model.size();
// 						CModel** ppModel = pModelGroup->model.begin();
// 						for (INT i = 0; i<modelCnt; i++)
// 						{
// 							CModel* pModel = *(ppModel + i);
// 							//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 							//m_pVGSCore->SunSetModelMtrl((DWORD)pModel, 
// 							//	(DWORD)(pModel->mouseClickedShader));
// 							//---------------------------------------------------------------------------------------
// 						}
// 					}
// 				}	
// 			}
// 			
// 			//改变模型的mouseUp顶点颜色
// 			if (m_pCurSelectedModel->bMouseColor)
// 			{
// 				//如果以[模型]为单元响应鼠标事件
// 				if (!m_pCurSelectedModel->bmouseHintByGroup)
// 				{
// 					INT alpha = (m_pCurSelectedModel->mouseClickedColor >> 24) & 0xff;
// 					INT red = (m_pCurSelectedModel->mouseClickedColor >> 16) & 0xff;
// 					INT green = (m_pCurSelectedModel->mouseClickedColor >> 8) & 0xff;
// 					INT blue = (m_pCurSelectedModel->mouseClickedColor) & 0xff;
// 					m_pVGSCore->SetModelColor((DWORD)m_pCurSelectedModel, red, green, blue, alpha);
// 				}
// 				else //以[模型组]为单元响应鼠标事件
// 				{
// 					if (m_pCurSelectedModel->modelGroup)
// 					{
// 						CModelGroup* pModelGroup = (CModelGroup*)(m_pCurSelectedModel->modelGroup);
// 						
// 						INT modelCnt = pModelGroup->model.size();
// 						CModel** ppModel = pModelGroup->model.begin();
// 						for (INT i = 0; i<modelCnt; i++)
// 						{
// 							CModel* pModel = *(ppModel + i);
// 							INT alpha = (pModel->mouseClickedColor >> 24) & 0xff;
// 							INT red = (pModel->mouseClickedColor >> 16) & 0xff;
// 							INT green = (pModel->mouseClickedColor >> 8) & 0xff;
// 							INT blue = (pModel->mouseClickedColor) & 0xff;
// 							m_pVGSCore->SetModelColor((DWORD)pModel, red, green, blue, alpha);
// 						}
// 					}
// 				}
// 			}
// 			
// 			m_pCurSelectedModel->bSelected = TRUE;
// 		}
// 		
// 		//复位前一个选中模型的材质--后一次
// 		if (m_pLastSelectedModel &&
// 			m_pLastSelectedModel != m_pCurSelectedModel && 
// 			m_pLastSelectedModel->mouseInOutable)
// 		{
// 			
// 			//改变模型的mouseUp材质
// 			if (m_pLastSelectedModel->bMouseShader)
// 			{
// 				//如果以[模型]为单元响应鼠标事件
// 				if (!m_pLastSelectedModel->bmouseHintByGroup)
// 				{
// 					//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 					//m_pVGSCore->SunSetModelMtrl((DWORD)m_pLastSelectedModel,
// 					//	(DWORD)(m_pLastSelectedModel->mouseUpShader));
//                     //---------------------------------------------------------------------------------------
// 				}
// 				else //以[模型组]为单元响应鼠标事件
// 				{
// 					if (m_pLastSelectedModel->modelGroup)
// 					{
// 						CModelGroup* pModelGroup = (CModelGroup*)(m_pLastSelectedModel->modelGroup);
// 						
// 						INT modelCnt = pModelGroup->model.size();
// 						CModel** ppModel = pModelGroup->model.begin();
// 						for (INT i = 0; i<modelCnt; i++)
// 						{
// 							//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 							//CModel* pModel = *(ppModel + i);
// 							//m_pVGSCore->SunSetModelMtrl((DWORD)pModel,
// 							//	(DWORD)(pModel->mouseUpShader));
// 							//---------------------------------------------------------------------------------------
// 						}
// 					}
// 				}
// 			}
// 			
// 			
// 			//改变模型的mouseUp顶点颜色
// 			if (m_pLastSelectedModel->bMouseColor)
// 			{
// 				//如果以[模型]为单元响应鼠标事件
// 				if (!m_pLastSelectedModel->bmouseHintByGroup)
// 				{
// 					INT alpha = (m_pLastSelectedModel->mouseUpColor >> 24) & 0xff;
// 					INT red = (m_pLastSelectedModel->mouseUpColor >> 16) & 0xff;
// 					INT green = (m_pLastSelectedModel->mouseUpColor >> 8) & 0xff;
// 					INT blue = (m_pLastSelectedModel->mouseUpColor) & 0xff;
// 					m_pVGSCore->SetModelColor((DWORD)m_pLastSelectedModel, red, green, blue, alpha);
// 				}
// 				else //以[模型组]为单元响应鼠标事件
// 				{
// 					if (m_pLastSelectedModel->modelGroup)
// 					{
// 						CModelGroup* pModelGroup = (CModelGroup*)(m_pLastSelectedModel->modelGroup);
// 						
// 						INT modelCnt = pModelGroup->model.size();
// 						CModel** ppModel = pModelGroup->model.begin();
// 						for (INT i = 0; i<modelCnt; i++)
// 						{
// 							CModel* pModel = *(ppModel + i);
// 							INT alpha = (m_pLastSelectedModel->mouseUpColor >> 24) & 0xff;
// 							INT red = (m_pLastSelectedModel->mouseUpColor >> 16) & 0xff;
// 							INT green = (m_pLastSelectedModel->mouseUpColor >> 8) & 0xff;
// 							INT blue = (m_pLastSelectedModel->mouseUpColor) & 0xff;
// 							m_pVGSCore->SetModelColor((DWORD)pModel, red, green, blue, alpha);
// 						}
// 					}
// 				}
// 			}
// 			m_pLastSelectedModel->bSelected = FALSE;
// 		}		
// 	}	
// 	
// 	
// 	m_pLastSelectedModel = m_pCurSelectedModel;
// 	
// 	//调用MouseDownInvoke
// 	if (bEnable_MouseDown_Callback)
// 		MouseDownInvoke();
// 	
// 	LeaveCriticalSection(m_pCtrl_cs);
// 	
// 	//必须加入这个方法，否则无法响应mouseWheel
// 	SetFocus();
// 	
// 	m_bMouseDownEvent_Finished = TRUE;
	
	return S_OK;
}
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
/*	if (m_pVGSCore->vgsScene == NULL) return E_FAIL;
	if (!m_bActive) return E_FAIL; 
	if(m_bRightMenu)
	{

	}
	  POINT pt;   
	  ::GetCursorPos(&pt); 
	    // display a menu created using CreateMenu()
    HMENU hMenu = ::CreatePopupMenu();
    if (NULL != hMenu)
    {
        // add a few test items
        ::AppendMenu(hMenu, MF_STRING, 1, "制作自己的三维网页");
        ::AppendMenu(hMenu, MF_STRING, 2, "发布自己的三维网页");
		::AppendMenu(hMenu, MF_SEPARATOR, 3,"");		
        ::AppendMenu(hMenu, MF_STRING, 4, "        关于赛虎网");
		HBITMAP hBitMap = ::LoadBitmap(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP1));
		::SetMenuItemBitmaps(hMenu, 3, MF_BYPOSITION , hBitMap, hBitMap);
        int sel = ::TrackPopupMenuEx(hMenu, 
                TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
                pt.x,
                pt.y,
                m_hWnd,
                NULL);
        ::DestroyMenu(hMenu);
    }*/

//	hMainMenu = LoadMenu(_Module.GetResourceInstance(),  MAKEINTRESOURCE(IDR_MENU1));   
//	hPopMenu = GetSubMenu(hMainMenu, 0);   
	
//	int selection = TrackPopupMenuEx(hPopMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, LOWORD(lParam), HIWORD(lParam),   GetActiveWindow(),   NULL);   
	//m_bRightMenu = TRUE;

	return S_OK;

}
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    return S_OK;
}
//-------------------------------------------------------------------
//方法: OnLButtonUP()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 鼠标左键松开方法
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{	
// 	if (m_pVGSCore->vgsScene == NULL) return E_FAIL;
// 	if (!m_bActive) return E_FAIL;
// 
// 	if(m_bRightMenu)
// 	{
// 		DestroyMenu(hPopMenu);   
// 		DestroyMenu(hMainMenu);   
// 		hPopMenu   =   hMainMenu   =   NULL;
// 	}
// 	
// 	m_pSelectedNavPointer = NULL;
// 	m_pSelectedNavigater = NULL;
// 	m_pVGSCore->bCanRos = TRUE;
// 	
// 	m_pVGSCore->vgsScene->InputDevice.mouseInfo.LButton = FALSE;
// 	
// 	if (!m_bMouseUpEvent_Finished) 
// 		return E_FAIL;
// 	
// 	m_bMouseUpEvent_Finished = FALSE;
// 	
// 	EnterCriticalSection(m_pCtrl_cs);
// 	
// 	//软键盘-方向键
// 	m_pVGSCore->vgsScene->softKey[0] = FALSE;
// 	m_pVGSCore->vgsScene->softKey[1] = FALSE;
// 	m_pVGSCore->vgsScene->softKey[2] = FALSE;
// 	m_pVGSCore->vgsScene->softKey[3] = FALSE;
// 	
// 	//-------------------------------------------------------------
// 	//鼠标松开按钮，按钮变化
// 	//-------------------------------------------------------------
// 	//改变button的材质为mouseOver材质
// 	if (m_pVGSCore->vgsScene->pMouseOverButton != NULL)
// 		m_pVGSCore->vgsScene->pMouseOverButton->btnOverlay->m_pMtrl = 
// 		m_pVGSCore->vgsScene->pMouseOverButton->pMtrl_MouseOver;
// 	
// 	//------------------------------------------------------------------------
// 	//执行JavaScript里的"VGSMouseClick"函数
// 	//------------------------------------------------------------------------
// 	if (bEnable_MouseUp_Callback)
// 		MouseUpInvoke();
// 	
// 	LeaveCriticalSection(m_pCtrl_cs);
// 	
// 	//必须加入这个方法，否则无法响应mouseWheel
// 	SetFocus();
// 	
// 	m_bMouseUpEvent_Finished = TRUE;
// 
// 	m_pVGSCore->vgsScene->pLButton= FALSE;
	
	return S_OK;
}
//-------------------------------------------------------------------
//方法: OnLButtonDBClick()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 鼠标左键双击方法
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
 	//if (m_pVGSCore->vgsScene == NULL) return E_FAIL;
 	//if (!m_bActive) return E_FAIL;
 	//if (!m_bMouseDBClickEvent_Finished) return E_FAIL;
 	
 	//m_bMouseDBClickEvent_Finished = FALSE;
 	
 	EnterCriticalSection(m_pCtrl_cs);
 	
 	//调用MouseDownInvoke
 	if (bEnable_MouseDBClick_Callback)
	{
		MouseDBClickInvoke(LOWORD(lParam), HIWORD(lParam));
	}

 	LeaveCriticalSection(m_pCtrl_cs);
 	
 	//m_bMouseDBClickEvent_Finished = TRUE;
	
	return S_OK;
}
//-------------------------------------------------------------------
//方法: OnMouseMove()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 鼠标移动方法
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
// 	if (m_pVGSCore->vgsScene == NULL) return E_FAIL;
// 	if (!m_bActive) return E_FAIL;
// 	if (!m_bMouseMoveEvent_Finished) return E_FAIL;
// 	
// 	m_bMouseMoveEvent_Finished = FALSE;
// 	
// 	m_pVGSCore->vgsScene->InputDevice.mouseInfo.X = LOWORD(lParam);
// 	m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y = HIWORD(lParam);
// 	
// 	EnterCriticalSection(m_pCtrl_cs);
// 	EnterCriticalSection(m_pRender_cs);
// 	
// 	//通过鼠标拖动导航指针控制相机的位置
// 	if(m_pSelectedNavPointer && m_pSelectedNavigater->m_bCanNavByPointer)
// 	{
// 		m_pVGSCore->bCanRos = FALSE;
// 		
// 		RECT rect;
// 		m_pSelectedNavPointer->pPointerOverlay->GetRect(&rect);
// 		INT width = rect.right - rect.left;
// 		INT height = rect.bottom - rect.top;
// 		
// 		RECT rect0;
// 		m_pSelectedNavigater->pNavigateMap->GetRect(&rect0);
// 		
// 		RECT rect_1;
// 		rect_1.left = m_pVGSCore->vgsScene->InputDevice.mouseInfo.X - width/2;
// 		rect_1.right = m_pVGSCore->vgsScene->InputDevice.mouseInfo.X + width/2;
// 		rect_1.top = m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y - height/2;
// 		rect_1.bottom = m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y + height/2;
// 		
// 		//判断是否鼠标拖出导航区域
// 		INT navWidth = m_pSelectedNavigater->navRect.right - 
// 			m_pSelectedNavigater->navRect.left;
// 		INT navHeight = m_pSelectedNavigater->navRect.bottom - 
// 			m_pSelectedNavigater->navRect.top;
// 		if ((rect_1.left > rect0.left + m_pSelectedNavigater->navRect.left) && 
// 			(rect_1.right < rect0.left + m_pSelectedNavigater->navRect.left + navWidth) &&
// 			(rect_1.top > rect0.top + m_pSelectedNavigater->navRect.top) &&
// 			(rect_1.bottom < rect0.top + m_pSelectedNavigater->navRect.top + rect0.top + navHeight))
// 		{
// 			m_pSelectedNavPointer->pPointerOverlay->SetRect(&rect_1);
// 			m_pSelectedNavigater->UpdateCameraPosition(m_pSelectedNavPointer->pPointerOverlay);
// 		}
// 	}
// 	
// 	//鼠标移动调用的JS方法
// 	if (bEnable_MouseMove_Callback)
// 		MouseMoveInvoke();
// 	
// 	LeaveCriticalSection(m_pRender_cs);
// 	LeaveCriticalSection(m_pCtrl_cs);
// 	
// 	m_bMouseMoveEvent_Finished = TRUE;
	
	return S_OK;	
}
//-------------------------------------------------------------------
//自定义的鼠标移动消息, 用于鼠标移动到模型上的鼠标事件响应
//-------------------------------------------------------------------
// VOID CVGS2Window::UserMouseMoveEvent()
// {
// 	if (m_pVGSCore->vgsScene == NULL) return;
// 	if (!m_bActive) return;
// 	
// 	//-------------------------------------------------------------
// 	//光标移动到按钮上，按钮的响应
// 	//-------------------------------------------------------------
// 	INT mouseX = m_pVGSCore->vgsScene->InputDevice.mouseInfo.X;
// 	INT mouseY = m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y;
// 	
// 	m_pVGSCore->vgsScene->pMouseOverButton = GetMouseOverButton(mouseX, mouseY);
// 	//改变button的材质为mouseOver材质
// 	if (m_pVGSCore->vgsScene->pMouseOverButton != NULL)
// 	{	
// 		if (m_pButton != NULL)
// 		{
// 			m_pButton->btnOverlay->m_pMtrl = m_pButton->pMtrl_MouseUp;
// 			m_pButton = NULL;
// 		}
// 		
// 		m_pButton = m_pVGSCore->vgsScene->pMouseOverButton;
// 
// 		// fuxb 2009-03-13 ==>
// 		// m_pVGSCore->vgsScene->pMouseOverButton->btnOverlay->m_pMtrl = m_pVGSCore->vgsScene->pMouseOverButton->pMtrl_MouseOver;
// 		if (m_pVGSCore->vgsScene->InputDevice.mouseInfo.LButton == 0 && m_pVGSCore->vgsScene->InputDevice.mouseInfo.RButton == 0)
// 		{
// 			m_pVGSCore->vgsScene->pMouseOverButton->btnOverlay->m_pMtrl = m_pVGSCore->vgsScene->pMouseOverButton->pMtrl_MouseOver;
// 		}
// 		else
// 		{
// 			m_pVGSCore->vgsScene->pMouseOverButton->btnOverlay->m_pMtrl = m_pVGSCore->vgsScene->pMouseOverButton->pMtrl_MouseDown;
// 		}
// 		// <==
// 	}
// 	else
// 	{
// 		if (m_pButton != NULL)
// 		{
// 			m_pButton->btnOverlay->m_pMtrl = m_pButton->pMtrl_MouseUp;
// 			m_pButton = NULL;
// 		}
// 	}
// 	
// 	//当鼠标在Overlay上方，则不检测模型
// 	DWORD overlayHandle = m_pVGSCore->SunGetOverlayUnderLoc(mouseX, mouseY);
// 	if (overlayHandle) return;
// 	
// 	//-------------------------------------------------------------------------
// 	//关于模型的鼠标移动事件响应
// 	//-------------------------------------------------------------------------
// 	//复位上一个模型的材质--前一次
// 	if (m_pLastCursorModel && m_pLastCursorModel != m_pCurCursorModel && !m_pLastCursorModel->bSelected)
// 	{
// 		//如果以[模型]为单元响应鼠标事件
// 		if (!m_pLastCursorModel->bmouseHintByGroup)
// 		{
// 			//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 			//DWORD mtrlHandle = (DWORD)(m_pLastCursorModel->mouseUpShader);
// 			DWORD modelHandle = (DWORD)m_pLastCursorModel;
// 			//---------------------------------------------------------------------------------------
// 			//恢复模型的材质
// 			//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 			//m_pVGSCore->SunSetModelMtrl(modelHandle, mtrlHandle);
// 			//---------------------------------------------------------------------------------------
// 			//恢复模型的颜色
// 			CModel* pModel = (CModel*)modelHandle;
// 			INT alpha = (pModel->mouseUpColor >> 24) & 0xff;
// 			INT red = (pModel->mouseUpColor >> 16) & 0xff;
// 			INT green = (pModel->mouseUpColor >> 8) & 0xff;
// 			INT blue = (pModel->mouseUpColor) & 0xff;
// 			m_pVGSCore->SetModelColor(modelHandle, red, green, blue, alpha);
// 		}
// 		else //以[模型组]为单元响应鼠标事件
// 		{
// 			if (m_pLastCursorModel->modelGroup)
// 			{
// 				CModelGroup* pModelGroup = (CModelGroup*)(m_pLastCursorModel->modelGroup);
// 				
// 				INT modelCnt = pModelGroup->model.size();
// 				CModel** ppModel = pModelGroup->model.begin();
// 				for (INT i = 0; i<modelCnt; i++)
// 				{
// 					CModel* pModel = *(ppModel + i);
// 					//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 					//DWORD mtrlHandle = (DWORD)(pModel->mouseUpShader);
// 					//---------------------------------------------------------------------------------------
// 					DWORD modelHandle = (DWORD)pModel;
// 					//恢复模型的材质
//                     //Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 					//m_pVGSCore->SunSetModelMtrl(modelHandle, mtrlHandle);
// 					//---------------------------------------------------------------------------------------
// 					//恢复模型的颜色
// 					INT alpha = (pModel->mouseUpColor >> 24) & 0xff;
// 					INT red = (pModel->mouseUpColor >> 16) & 0xff;
// 					INT green = (pModel->mouseUpColor >> 8) & 0xff;
// 					INT blue = (pModel->mouseUpColor) & 0xff;
// 					m_pVGSCore->SetModelColor((DWORD)pModel, red, green, blue, alpha);	
// 				}
// 			}
// 		}
// 		
// 		m_pLastCursorModel = NULL;
// 	}
// 	
// 	//得到光标下方的模型
// 	DWORD modelHandle = m_pVGSCore->SunGetMouseHintModelUnderLoc(m_pVGSCore->vgsScene->InputDevice.mouseInfo.X, m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y);
// 	m_pCurCursorModel = (CModel*)modelHandle;
// 	
// 	//设置鼠标移上某个物体时物体改变材质的效果------------------------------------
// 	if (m_pCurCursorModel && m_pCurCursorModel->bMouseShader)
// 	{
// 		//改变鼠标下方的模型的材质
// 		if (!m_pCurCursorModel->bSelected)//如果以[模型]为单元响应鼠标事件
// 		{ 	
// 			if (!m_pCurCursorModel->bmouseHintByGroup)
// 			{
// 				//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 				//DWORD mtrlHandle = (DWORD)(m_pCurCursorModel->mouseOverShader);
// 				//m_pVGSCore->SunSetModelMtrl(modelHandle, mtrlHandle);
// 				//---------------------------------------------------------------------------------------
// 			}
// 			else//以[模型组]为单元响应鼠标事件
// 			{
// 				if (m_pCurCursorModel->modelGroup)
// 				{
// 					CModelGroup* pModelGroup = (CModelGroup*)(m_pCurCursorModel->modelGroup);
// 					
// 					INT modelCnt = pModelGroup->model.size();
// 					CModel** ppModel = pModelGroup->model.begin();
// 					for (INT i = 0; i<modelCnt; i++)
// 					{
// 						CModel* pModel = *(ppModel + i);
// 						//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 						//DWORD mtrlHandle = (DWORD)(pModel->mouseOverShader);
// 						//m_pVGSCore->SunSetModelMtrl((DWORD)pModel, mtrlHandle);
// 						//---------------------------------------------------------------------------------------
// 					}
// 				}
// 			}
// 		}
// 		
// 		//复位上一个模型的材质--后一次
// 		if (m_pLastCursorModel && m_pLastCursorModel != m_pCurCursorModel && !m_pLastCursorModel->bSelected)
// 		{
// 			//如果以[模型]为单元响应鼠标事件
// 			if (!m_pLastCursorModel->bmouseHintByGroup)
// 			{
// 				//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 				//DWORD mtrlHandle = (DWORD)(m_pLastCursorModel->mouseUpShader);
// 				//DWORD modelHandle = (DWORD)m_pLastCursorModel;
// 				//m_pVGSCore->SunSetModelMtrl(modelHandle, mtrlHandle);
// 				//---------------------------------------------------------------------------------------
// 			}
// 			else //以[模型组]为单元响应鼠标事件
// 			{
// 				if (m_pLastCursorModel->modelGroup)
// 				{
// 					CModelGroup* pModelGroup = (CModelGroup*)(m_pLastCursorModel->modelGroup);
// 					
// 					INT modelCnt = pModelGroup->model.size();
// 					CModel** ppModel = pModelGroup->model.begin();
// 					for (INT i = 0; i<modelCnt; i++)
// 					{
// 						CModel* pModel = *(ppModel + i);
// 						//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
// 						//DWORD mtrlHandle = (DWORD)(pModel->mouseUpShader);
// 						//DWORD modelHandle = (DWORD)pModel;
// 						//恢复模型的材质
// 						//m_pVGSCore->SunSetModelMtrl(modelHandle, mtrlHandle);	
// 						//-------------------------------------------------------------------------------------
// 					}
// 				}
// 			}
// 			
// 			m_pLastCursorModel = NULL;
// 		}
// 		m_pLastCursorModel = m_pCurCursorModel;
// 	}
// 	
// 	
// 	//--------------------------------------------------------------------------------
// 	//设置鼠标移上某个物体时物体改变顶点颜色的效果
// 	//--------------------------------------------------------------------------------
// 	if (m_pCurCursorModel && m_pCurCursorModel->bMouseColor)
// 	{
// 		if (!m_pCurCursorModel->bSelected)
// 		{ 	
// 			if (!m_pCurCursorModel->bmouseHintByGroup)//如果以[模型]为单元响应鼠标事件
// 			{
// 				INT alpha = (m_pCurCursorModel->mouseOverColor >> 24) & 0xff;
// 				INT red = (m_pCurCursorModel->mouseOverColor >> 16) & 0xff;
// 				INT green = (m_pCurCursorModel->mouseOverColor >> 8) & 0xff;
// 				INT blue = (m_pCurCursorModel->mouseOverColor) & 0xff;
// 				m_pVGSCore->SetModelColor(modelHandle, red, green, blue, alpha);
// 			}
// 			//以[模型组]为单元响应鼠标事件
// 			else
// 			{
// 				if (m_pCurCursorModel->modelGroup)
// 				{
// 					CModelGroup* pModelGroup = (CModelGroup*)(m_pCurCursorModel->modelGroup);
// 					
// 					INT modelCnt = pModelGroup->model.size();
// 					CModel** ppModel = pModelGroup->model.begin();
// 					for (INT i = 0; i<modelCnt; i++)
// 					{
// 						CModel* pModel = *(ppModel + i);
// 						INT alpha = (pModel->mouseOverColor >> 24) & 0xff;
// 						INT red = (pModel->mouseOverColor >> 16) & 0xff;
// 						INT green = (pModel->mouseOverColor >> 8) & 0xff;
// 						INT blue = (pModel->mouseOverColor) & 0xff;
// 						m_pVGSCore->SetModelColor((DWORD)pModel, red, green, blue, alpha);
// 					}
// 				}
// 			}
// 		}
// 		
// 		//复位上一个模型的颜色--后一次
// 		if (m_pLastCursorModel && m_pLastCursorModel != m_pCurCursorModel && !m_pLastCursorModel->bSelected)
// 		{
// 			//如果以[模型]为单元响应鼠标事件
// 			if (!m_pLastCursorModel->bmouseHintByGroup)
// 			{
// 				DWORD modelHandle = (DWORD)m_pLastCursorModel;
// 				//恢复模型的颜色
// 				INT alpha = (m_pLastCursorModel->mouseUpColor >> 24) & 0xff;
// 				INT red = (m_pLastCursorModel->mouseUpColor >> 16) & 0xff;
// 				INT green = (m_pLastCursorModel->mouseUpColor >> 8) & 0xff;
// 				INT blue = (m_pLastCursorModel->mouseUpColor) & 0xff;
// 				m_pVGSCore->SetModelColor(modelHandle, red, green, blue, alpha);
// 			}
// 			else //以[模型组]为单元响应鼠标事件
// 			{
// 				if (m_pLastCursorModel->modelGroup)
// 				{
// 					CModelGroup* pModelGroup = (CModelGroup*)(m_pLastCursorModel->modelGroup);
// 					
// 					INT modelCnt = pModelGroup->model.size();
// 					CModel** ppModel = pModelGroup->model.begin();
// 					for (INT i = 0; i<modelCnt; i++)
// 					{
// 						CModel* pModel = *(ppModel + i);
// 						
// 						//恢复模型的颜色
// 						INT alpha = (m_pLastCursorModel->mouseUpColor >> 24) & 0xff;
// 						INT red = (m_pLastCursorModel->mouseUpColor >> 16) & 0xff;
// 						INT green = (m_pLastCursorModel->mouseUpColor >> 8) & 0xff;
// 						INT blue = (m_pLastCursorModel->mouseUpColor) & 0xff;
// 						m_pVGSCore->SetModelColor((DWORD)pModel, red, green, blue, alpha);
// 					}
// 				}
// 			}
// 			m_pLastCursorModel = NULL;
// 		}
// 		
// 		m_pLastCursorModel = m_pCurCursorModel;
// 	}
	
// 	return;
// }

//Flash播放到最后一帧的事件
// VOID CVGS2Window::FlashPlayOverEvent()
// {
// 	CATLFlashCtrl** ppFlash = m_Flash.begin();
// 	INT flashCnt = m_Flash.size();
// 	for (INT i = 0; i< flashCnt; i++)
// 	{
// 		CATLFlashCtrl* pFlash = *(ppFlash + i);
// 		INT frameCnt = pFlash->GetFrameCnt();
// 		if (frameCnt > 1)
// 		{
// 			if (pFlash->GetCurrentFrame()+1 >= frameCnt && !pFlash->bPlayOver)
// 			{
// 				pFlash->bPlayOver = TRUE;
// 				
// 				_bstr_t bst("FlashPlayOver");
// 				BSTR BSTR_FlashOver = bst.copy();
// 				
// 				CComBSTR bstr = NULL;
// 				bstr = pFlash->name;
// 				
// 				memset(&dispparams, 0, sizeof dispparams);
// 				dispparams.cArgs = 1;
// 				dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 				bstr.CopyTo(&dispparams.rgvarg[0].bstrVal);
// 				dispparams.rgvarg[0].vt = VT_BSTR;
// 				
// 				invokeJS->InvokeJSFunc(BSTR_FlashOver, &dispparams); 
// 				
// 				SAFE_DELETE(dispparams.rgvarg);
// 			}
// 		}
// 	}
//}

//Flash下载完成的事件
// VOID CVGS2Window::FlashLoadOverEvent()
// {
// 	CATLFlashCtrl** ppFlash = m_Flash.begin();
// 	INT flashCnt = m_Flash.size();
// 	for (INT i = 0; i< flashCnt; i++)
// 	{
// 		CATLFlashCtrl* pFlash = *(ppFlash + i);
// 		INT percent = pFlash->GetLoadPercent();
// 		
// 		if (percent == 100 && pFlash->bCreated)
// 		{
// 			if (!pFlash->bLoadOver)
// 			{
// 				pFlash->bLoadOver = TRUE;
// 				
// 				//调用JS事件，Flash动画文件下载完毕
// 				_bstr_t bst("FlashLoadOver");
// 				BSTR BSTR_FlashOver = bst.copy();
// 				
// 				CComBSTR bstr = NULL;
// 				bstr = pFlash->name;
// 				
// 				memset(&dispparams, 0, sizeof dispparams);
// 				dispparams.cArgs = 1;
// 				dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 				bstr.CopyTo(&dispparams.rgvarg[0].bstrVal);
// 				dispparams.rgvarg[0].vt = VT_BSTR;
// 				
// 				invokeJS->InvokeJSFunc(BSTR_FlashOver, &dispparams); 
// 				
// 				SAFE_DELETE(dispparams.rgvarg);
// 				
// 				//---------------------------------------------------
// 			}
// 			
// 			if (i == flashCnt -1)
// 				break;
// 			
// 			CATLFlashCtrl* pNextFlash = *(ppFlash + i + 1);
// 			if (!pNextFlash->bCreated)
// 			{
// 				RECT clientRect;
// 				GetClientRect(&clientRect);
// 				RECT& rect = clientRect;
// 				if(pNextFlash->m_bVisible == FALSE)
// 					pNextFlash->SetVisible(0);
// 				
// 				pNextFlash->Create(pNextFlash->name, 
// 					rect, 
// 					pNextFlash->oriLeft, 
// 					pNextFlash->oriTop, 
// 					pNextFlash->width,
// 					pNextFlash->height, 
// 					pNextFlash->m_strMovieName, 
// 					30, 
// 					TRUE);
// 				if(pNextFlash->playStatus == 1)
// 					pNextFlash->Play(true);
// 				
// 				//更新Flash Rect
// 				UpdateFlashRect();
// 			}
// 		}
// 		else
// 			break;
// 	}
//}
//更新flash的Info
// VOID CVGS2Window::UpdateFlashInfo()
// {
//更新Flash的位置
// 	UINT flashCnt = m_Flash.size();
// 	for (INT i = 0; i< flashCnt; i++)
// 	{
// 		CATLFlashCtrl* pFlash = m_Flash[i];
// 		UINT flashCnt = pFlash->SemaphoreList.size();
// 		for(UINT n =0; n< flashCnt; n++)
// 		{
// 		  	std::string SemaphoreName = pFlash->SemaphoreList[n].SemaphoreName;
// 			std::string SemaphoreValue = pFlash->GetFlashVariable(SemaphoreName);
// 			if(strcmp(SemaphoreValue.c_str(),"true")==0)
// 			{
// 			    pFlash->SetFlashVariable(SemaphoreName,std::string("false"));
// 			    //调用javascript的flash信号量设置后需要执行的函数
// 				_bstr_t bst(pFlash->SemaphoreList[n].SemaphoreFunc.c_str());
// 				BSTR command = bst.copy();			
// 			
// 				memset(&dispparams, 0, sizeof(dispparams));
// 				dispparams.cArgs = 1;
// 				dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 				dispparams.rgvarg[0].lVal = -1;
// 				dispparams.rgvarg[0].vt = VT_INT;
// 				
// 				dispparams.cNamedArgs = 0;
// 				invokeJS->InvokeJSFunc(command, &dispparams); 
// 				BSTR BSTR_CamKfrJSFunc = bst.copy();
// 				SAFE_DELETE(dispparams.rgvarg);
// 			}
// 		}
// 
// 	}
//}
//更新FlashRect
// VOID CVGS2Window::UpdateFlashRect()
// {
// 	RECT clientRect;
// 	GetClientRect(&clientRect);
// 	//更新Flash的位置
// 	UINT flashCnt = m_Flash.size();
// 	for (INT i = 0; i< flashCnt; i++)
// 	{
// 		CATLFlashCtrl* pFlash = m_Flash[i];
// 		
// 		INT realLeft, realTop;
// 		//对齐模式
// 		switch ((ALIGNMODE)(pFlash->alignMode))
// 		{	
// 		case ALIGN_NONE:
// 			realLeft = clientRect.left;
// 			realTop = clientRect.top;
// 			break;
// 		case ALIGN_LEFT:
// 			realLeft = clientRect.left;
// 			break;
// 		case ALIGN_RIGHT:
// 			realLeft = clientRect.right - pFlash->width;
// 			break;
// 		case ALIGN_BOTTOM:
// 			realTop = clientRect.bottom - pFlash->height;
// 			break;
// 		case ALIGN_TOP:
// 			realTop = clientRect.top;
// 			break;
// 		case ALIGN_LEFT_TOP:
// 			realLeft = clientRect.left;
// 			realTop = clientRect.top;
// 			break;
// 		case ALIGN_MIDDLE_TOP:
// 			realLeft = (clientRect.left + clientRect.right)/2 - pFlash->width/2;
// 			realTop =  clientRect.top;
// 			break;
// 		case ALIGN_RIGHT_TOP:
// 			realLeft = clientRect.right - pFlash->width;
// 			realTop = clientRect.top;
// 			break;
// 		case ALIGN_LEFT_BOTTOM:
// 			realLeft = clientRect.left;
// 			realTop = clientRect.bottom - pFlash->height;
// 			break;
// 		case ALIGN_MIDDLE_BOTTOM:
// 			realLeft = (clientRect.left + clientRect.right)/2 - pFlash->width/2;
// 			realTop = clientRect.bottom - pFlash->height;
// 			break;
// 		case ALIGN_RIGHT_BOTTOM:
// 			realLeft = clientRect.right - pFlash->width;
// 			realTop = clientRect.bottom - pFlash->height;
// 			break;
// 		case ALIGN_CENTER:
// 			realLeft =  (clientRect.left + clientRect.right)/2 - pFlash->width/2;
// 			realTop = (clientRect.top + clientRect.bottom)/2 - pFlash->height/2;
// 			break;
// 		}
// 		
// 		//增加相对位置
// 		realLeft += pFlash->oriLeft;
// 		realTop += pFlash->oriTop;
// 		
// 		//宽度占满吗?
// 		if (pFlash->isFullWidth)
// 			pFlash->width = clientRect.right - clientRect.left;
// 		
// 		//高度度占满吗?
// 		if (pFlash->isFullHeight)
// 			pFlash->height = clientRect.bottom - clientRect.top;
// 		
// 		m_Flash[i]->SetRect(realLeft, realTop, pFlash->width, pFlash->height);
// 	}
//}

//-------------------------------------------------------------------
//方法: OnMouseWheel()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 鼠标滚轮时调用的方法
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
// 	if (m_pVGSCore->vgsScene == NULL) return E_FAIL;
// 	if (!m_bActive) return E_FAIL;
// 	
// 	INT nRollPosition = 0;  //鼠标滚轮
// 	nRollPosition = (SHORT)HIWORD(wParam);
// 	if (!nRollPosition) return S_OK;
// 	
// 	EnterCriticalSection(m_pCtrl_cs);
// 	
// 	D3DXVECTOR3 from, at, dis;
// 	FLOAT len;
// 	
// 	switch (m_pVGSCore->controlStatus)
// 	{
// 	case WALK:
// 		m_pVGSCore->walker.walkSpeed += nRollPosition/100.0f;
// 		if (m_pVGSCore->walker.walkSpeed < 0.01f)
// 			m_pVGSCore->walker.walkSpeed = 0.01f;
// 		
// 		if (m_pVGSCore->walker.walkSpeed > m_pVGSCore->colDistance)
// 			m_pVGSCore->walker.walkSpeed = m_pVGSCore->colDistance;
// 		break;
// 		
// 	case EDIT:
// 		from = m_pVGSCore->vgsScene->currentCam->GetFrom();
// 		at = m_pVGSCore->vgsScene->currentCam->GetAt();
// 		dis = at - from;
// 		len = D3DXVec3Length(&dis);
// 		m_pVGSCore->editor.dollyValue = m_pVGSCore->editor.dollySpeed * len * nRollPosition/2400;
// 		break;
// 		
// 	case FLY:
// 		//判断是否超过了DollyNearDis和DollyFarDis
// 		from = m_pVGSCore->vgsScene->currentCam->GetFrom();
// 		at = m_pVGSCore->vgsScene->currentCam->GetAt();
// 		dis = at - from;
// 		len = D3DXVec3Length(&dis);
// 		m_pVGSCore->flyer.dollySpeed = 0.03f * len;
// 		
// 		m_pVGSCore->flyer.dollyValue = m_pVGSCore->flyer.dollySpeed * nRollPosition/120;
// 		if (len > m_pVGSCore->flyer.dollyNearDis + m_pVGSCore->flyer.dollyValue)
// 		{
// 			if (len < m_pVGSCore->flyer.dollyFarDis + m_pVGSCore->flyer.dollyValue)
// 				m_pVGSCore->vgsScene->currentCam->Dolly(-m_pVGSCore->flyer.dollyValue);
// 		}
// 		break;
// 	}
// 	
// 	LeaveCriticalSection(m_pCtrl_cs);
	
	return S_OK;
}
//-------------------------------------------------------------------
//方法: OnMmJoyMove()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 使用游戏操纵杆时调用的方法
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnMmJoyMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
// 	if (m_pVGSCore->vgsScene == NULL) return E_FAIL;
// 	if (!m_bActive) return E_FAIL;
	return S_OK;
}
//-------------------------------------------------------------------
//方法: OnJSFuncProc()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 其它类调用JS Func, 通过消息传递到主窗体，主窗体在这里接受消息，并调用JS Func
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnJSFuncProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
// 	CCamera* pCamera = (CCamera*)wParam;
// 	INT frameIndex = lParam;
// 	
// 	EnterCriticalSection(m_pCtrl_cs);
// 	
// 	_bstr_t bst(pCamera->InvokeFuncName[frameIndex].name);
// 	BSTR BSTR_CamKfrJSFunc = bst.copy();
// 	
// 	memset(&dispparams, 0, sizeof(dispparams));
// 	dispparams.cArgs = 1;
// 	dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 	dispparams.rgvarg[0].lVal = frameIndex;
// 	dispparams.rgvarg[0].vt = VT_INT;
// 	
// 	dispparams.cNamedArgs = 0;
// 	
// 	invokeJS->InvokeJSFunc(BSTR_CamKfrJSFunc, &dispparams); 
// 	
// 	SAFE_DELETE(dispparams.rgvarg);
// 	
// 	LeaveCriticalSection(m_pCtrl_cs);
	
	return S_OK;
}
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnZTrigFuncProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	
// 	TriggerCallBack* TriggerModel=(TriggerCallBack*)wParam;
// 	EnterCriticalSection(m_pCtrl_cs);
// 	
//     
// 	_bstr_t bst(TriggerModel->callBackFunc);
// 	BSTR BSTR_ZTrigJSFunc = bst.copy();
// 	
// 	memset(&dispparams, 0, sizeof(dispparams));
// 	dispparams.cArgs = 1;
// 	dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 	dispparams.rgvarg[0].lVal = 0;
// 	dispparams.rgvarg[0].vt = 0;
// 	
// 	dispparams.cNamedArgs = 0;
// 	
// 	invokeJS->InvokeJSFunc(BSTR_ZTrigJSFunc, &dispparams); 
// 	
// 	SAFE_DELETE(dispparams.rgvarg);
// 	
// 	LeaveCriticalSection(m_pCtrl_cs);
	
	return S_OK;
	
}
//-------------------------------------------------------------------
//方法: OnDownLoadN3DProc()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 下载N3D文件线程消息调用的方法
//-------------------------------------------------------------------
 LRESULT CVGS2Window::OnDownLoadN3DProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
 {
	// 到底是哪个 N3D呢？就是这个
 	//CDownLoadN3DFile** ppN3D = m_LoadN3DObj.begin();
 	//CDownLoadN3DFile* pN3D = *(ppN3D + lParam);
 	////if(pN3D == NULL) return E_FAIL;
 	//m_pCurN3D = pN3D;
 	//CDownLoadN3DFile* pNextN3D = NULL;
 	
 	switch (wParam)
 	{
 		//得知N3D文件头经下载完
 	case HEADER_ISDOWN:
 		{
 			RECT clientRect;
 			GetClientRect(&clientRect);
 			UINT x,y;
 			x = clientRect.left + (clientRect.right -clientRect.left)/2 -64;
 			y = clientRect.top + (clientRect.bottom -clientRect.top)/2  +64 + 10;
 			m_pVGSCore->SunSetTextPosition(m_downText, x, y);
 		}
 		break;
 		
 		//正在下载
 	case IS_DOWN:
 		//显示下载的百分比
 		EnterCriticalSection(m_pRender_cs);//---
 		// SetDownPercentText();
 		LeaveCriticalSection(m_pRender_cs);//---
 		break;
 		
 		//得知场景已经下载完
 	case SCENE_ISDOWN: 		
	{	
 		//把Logo对象移到左上角
 		EnterCriticalSection(m_pCtrl_cs);//---
 		EnterCriticalSection(m_pRender_cs);//---
 		
 		//执行JavaScript里的"SceneIsDown"函数
 		// SceneIsDownInvoke(pN3D);
 		// pN3D->bSceneIsDownInvoked = TRUE;
 
 		//清除logo
 		m_pVGSCore->DeleteLogo();
 		m_pVGSCore->SunSetTextPosition(m_downText, 10, 10);

		// 缺省的视口和相机
		DWORD pVP = m_pVGSCore->GetDefaultViewport();
		if (m_pVGSCore->SunGetViewportCamera(pVP) == 0)
		{
			DWORD cam = 0; 
			DWORD curController = m_pVGSCore->SunGetCurrentController();
			if (curController != 0)
			{
				cam = m_pVGSCore->SunGetControllerCamera(curController);
			}
			if (cam == 0)
			{
				cam = m_pVGSCore->SunCreatePredefinedCamera(PERSPECTIVE_CAMERA);
			}

			m_pVGSCore->SunSetViewportCamera(pVP, cam);
		}
 
 		LeaveCriticalSection(m_pRender_cs);//---
 		LeaveCriticalSection(m_pCtrl_cs);//---
 		break;
 		
	}
 		//所有的数据都已经下载
 	case ALL_ISDOWN:
 		//删除已经下载完的下载对象
 		// EraseN3DDownloader(pN3D);
 
 		EnterCriticalSection(m_pCtrl_cs);//---
 		EnterCriticalSection(m_pRender_cs);//---
 		
 		//删除下载进度文字对象
 		m_pVGSCore->SunDelText(m_downText);
 		m_downText = NULL;
 		
 		//下载完成
 		// DownloadInvoke("100");
 		
 		// 开始Load对列中的下一个N3D
		// m_DownloadN3DFileManager.kickoutFirstFile();
 
 		//  AllIsDownInvoke(pN3D);
 		
 		LeaveCriticalSection(m_pRender_cs);//---
 		LeaveCriticalSection(m_pCtrl_cs);//---
 		break;
 
 		//异常终止，譬如强制关闭下载线程
 	case TERMINATED:
 		//删除已经下载完的下载对象
		// EraseN3DDownloader(pN3D);
		// m_DownloadN3DFileManager.kickoutFirstFile();
 		break;
 		
 		//所有贴图数据已经下载完
 	case ALLTEX_ISDOWN:	
 		break;
 		
 		//所有图像数据已经下载
 	case ALLIMAGE_ISDOWN:
 		break;
 	}
 	return S_OK;
 }
#if 0
//-------------------------------------------------------------------
// 下载任务的状态发生变化
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnHttpDownLoad_ChangeStatus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (VGS_DOWNLOADTASK_STATUS_COMPLETED == lParam)
	{
		m_bN3DFileDownloaded = TRUE;
	}

	return S_OK;
}
//-------------------------------------------------------------------
// 下载进度更新
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnHttpDownLoad_Progress(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return S_OK;
}
//-------------------------------------------------------------------
// 一个文件下载完成
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnHttpDownLoad_FileDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return S_OK;
}
//-------------------------------------------------------------------
// 一个N3D文件中的图片下载完成
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnHttpDownLoad_N3DOneImageDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CVGSN3DFileDownloadStreamTask *pN3DTask = (CVGSN3DFileDownloadStreamTask*)wParam;
	if (pN3DTask)
	{
		CTaskCreateInfo &pTaskInfo = m_mapN3DTaskCreateInfo[(DWORD)pN3DTask];
		if (!pTaskInfo.bSceneCreated)  // 如果场景还没有创建完成
		{
			pTaskInfo.setImageTobeCreated.insert((int)lParam);
			return S_OK;
		}

		CreateN3DImageTexture(pN3DTask, (int)lParam);
		/*int imgIdx = (int)lParam;
		DWORD dataLen = pN3DTask->GetImageDataLength(imgIdx);
		BYTE *pData = new BYTE[dataLen];
		if (pData && pN3DTask->ReadnN3DOneImage(pData, dataLen, imgIdx))
		{
			m_pVGSCore->SunCreateImageTextureFromBuffer(pData, dataLen, imgIdx);
			pTaskInfo.setImageTobeCreated.erase(imgIdx);
			pTaskInfo.setImageCreated.insert(imgIdx);
		}*/

	}
	return S_OK;
}
//-------------------------------------------------------------------
// 创建一张图片纹理
//-------------------------------------------------------------------
void CVGS2Window::CreateN3DImageTexture(CVGSN3DFileDownloadStreamTask *pN3DTask, int imgIdx)
{
	if (pN3DTask)
	{
		CTaskCreateInfo &pTaskInfo = m_mapN3DTaskCreateInfo[(DWORD)pN3DTask];
		DWORD dataLen = pN3DTask->GetImageDataLength(imgIdx);
		BYTE *pData = new BYTE[dataLen];
		if (pData && pN3DTask->ReadnN3DOneImage(pData, dataLen, imgIdx))
		{
			DWORD nameLen = *((DWORD*)pData);
			BYTE *pTmp = (pData + sizeof(nameLen) + nameLen);
			DWORD contentLen = *((DWORD*)pTmp);
			BYTE *pImageContent = pTmp + sizeof(contentLen);
#if _DEBUG
			char pName[1024];
			ZeroMemory(pName, 1024);
			memcpy(pName, pData + 4, nameLen);
			string sName = string("d:\\") + string(pName);
			FILE *pFile = fopen(sName.c_str(), "wb");
			fwrite(pImageContent, contentLen, 1, pFile); 
			fclose(pFile);

#endif
			m_pVGSCore->SunCreateImageTextureFromBuffer(pImageContent, contentLen, imgIdx);
			pTaskInfo.setImageTobeCreated.erase(imgIdx);
			pTaskInfo.setImageCreated.insert(imgIdx);

			// 是否所有的贴图都已经创建
			if (pTaskInfo.setImageCreated.size() == pTaskInfo.imageCount)
			{
				AllIsDownInvoke();
			}
		}
	}

}
//-------------------------------------------------------------------
// 一个N3D文件中的场景下载完成
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnHttpDownLoad_N3DSceneDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CVGSN3DFileDownloadStreamTask *pN3DTask = (CVGSN3DFileDownloadStreamTask*)wParam;
	if (pN3DTask && !m_mapN3DTaskCreateInfo[(DWORD)pN3DTask].bSceneCreated)
	{
		N3D_Header N3DHeader;
		pN3DTask->GetN3DHeader(N3DHeader);
		DWORD dwSceneLen = pN3DTask->GetN3DSceneZippedLength();
		BYTE *pSceneDataZipBuffer = new BYTE[dwSceneLen];
		if (true == pN3DTask->ReadN3DSceneData(pSceneDataZipBuffer, dwSceneLen))
		{
			m_mapN3DTaskCreateInfo[(DWORD)pN3DTask].bSceneCreated = true;

			// 创建场景
			BYTE *pSceneDataUnzipBuffer = new BYTE[N3DHeader.OriginSceneDataLen];  // 解压后的数据
			vgszipDeCodeFrmBuffToBuff(pSceneDataZipBuffer, dwSceneLen, pSceneDataUnzipBuffer);
			SAFE_DELETE_ARRAY(pSceneDataZipBuffer);

			m_pVGSCore->SunLoadSceneFromBuffer(N3DHeader.Version, pSceneDataUnzipBuffer);
			SAFE_DELETE_ARRAY(pSceneDataUnzipBuffer);

			// 设置当前视口使用的相机
			DWORD pViewport = m_pVGSCore->SunGetDefaultViewport();
			if (m_pVGSCore->SunGetViewportCamera(pViewport) == 0)
			{
				m_pVGSCore->SunSetViewportCamera(pViewport, m_pVGSCore->SunGetCurrentControllerCamera());
			}

			m_pVGSCore->SunSetRenderStatus(TRUE);

			if (m_mapN3DTaskCreateInfo[(DWORD)pN3DTask].setImageTobeCreated.size() > 0)
			{
				set<int>::iterator it = m_mapN3DTaskCreateInfo[(DWORD)pN3DTask].setImageTobeCreated.begin();
				set<int>::iterator itEnd = m_mapN3DTaskCreateInfo[(DWORD)pN3DTask].setImageTobeCreated.end();
				for (; it != itEnd; ++it)
				{
					CreateN3DImageTexture(pN3DTask, (*it));
				}
			}

			// script callback
			SceneIsDownInvoke();
		} // if ReadN3DSceneData
	} // if pN3D
	return S_OK;
}
#endif	//0
//-------------------------------------------------------------------
//	Name:OnDownLoadFileCompletedMT
//	Introduce:处理多线程文件下载完成消息
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnDownLoadFileCompletedMT(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CFileDwonloadInfo *pDownloadInfo = (CFileDwonloadInfo*)lParam;
	if (NULL == pDownloadInfo)
		return E_FAIL;

	FileLoadRequest req;
	req.pProcessor = (void*)this;
	req.pDataInfo = (void*)pDownloadInfo;

	VGS_FILE_USAGE usage = (VGS_FILE_USAGE)wParam;
	switch (usage)
	{
	case VGS_FILE_USAGE_N3D:
		req.type = VGS_FILELOADREQUEST_TYPE_LOADN3D;
		break;
	case VGS_FILE_USAGE_A3D:
		req.type = VGS_FILELOADREQUEST_TYPE_LOADA3D;
		break;
	case VGS_FILE_USAGE_FLASH2D:
		{
			return E_FAIL;
		}
		break;
	case VGS_FILE_USAGE_FLASHTEXTURE:
		{
			return E_FAIL;
		}
		break;
	case VGS_FILE_USAGE_IMAGETEXTURE:
		{
			return E_FAIL;
		}
		break;
	default:
		return E_FAIL;
	}
	m_FileLoadManagerMT.AddRequest(req);

	return S_OK;
}
//-------------------------------------------------------------------
//	Name:OnDownLoadFilePercentMT
//	Introduce:处理多线程文件下载进度消息
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnDownLoadFilePercentMT(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CFileDwonloadInfo *pDownloadInfo = (CFileDwonloadInfo*)lParam;
	if (NULL == pDownloadInfo)
		return E_FAIL;

	if (pDownloadInfo->bDisplayProgress && m_dwTotalProgressbar > 0)
	{
		// 当前已经完成的进度
		DWORD dwFinished = GetProgressbarFinishedBytes();
		int percent = (int)(100 * dwFinished / (float)m_dwTotalProgressbar + 0.5);
		percent = min(100, max(percent, 0));

		// 处理进度条
		ProcessProgress(percent);

		// 下载完成后删除进度条
		if (percent == 100)
		{
			DeleteProgressbar();
		}
	}

	return S_OK;
}
//-------------------------------------------------------------------
//	Name:OnSceneIsDownProc
//	Introduce:处理场景下载完成消息 
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnSceneIsDownProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT hr = S_OK;
	CFileDwonloadInfo *pInfo = (CFileDwonloadInfo*)lParam;
	if (NULL != pInfo)
	{
		hr = SceneIsDownInvoke(pInfo->sName);	
	}

	return hr;
}
//-------------------------------------------------------------------
//	Name:OnAllIsDownProc
//	Introduce:处理所有文件下载完成消息 
//-------------------------------------------------------------------
LRESULT CVGS2Window::OnAllIsDownProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT hr = S_OK;
	CFileDwonloadInfo *pInfo = (CFileDwonloadInfo*)lParam;
	if (NULL != pInfo)
	{
		hr = AllIsDownInvoke(pInfo->sName);	
	}

	return hr;	
}
//-------------------------------------------------------------------
//删除已经下载完的下载对象
//-------------------------------------------------------------------
// void CVGS2Window::EraseN3DDownloader(CDownLoadN3DFile* pN3D)
// {
// 	int downloaderCnt = m_LoadN3DObj.size();
// 	CDownLoadN3DFile** ppDownloader = m_LoadN3DObj.begin();
// 	for (int i = 0; i< downloaderCnt; i++)
// 	{
// 		CDownLoadN3DFile* pDownloader = *(ppDownloader + i);
// 		if (pDownloader == pN3D)
// 		{
// 			m_LoadN3DObj.erase(ppDownloader + i);
// 			return;
// 		}
// 	}
//}

//删除已经下载完的下载文件对象
// void CVGS2Window::EraseFileDownloader(CDownLoadFile* pDFile)
// {
// 	int downloaderCnt = m_LoadFiles.size();
// 	CDownLoadFile** ppDownloader = m_LoadFiles.begin();
// 	for (int i = 0; i< downloaderCnt; i++)
// 	{
// 		CDownLoadFile* pDownloader = *(ppDownloader + i);
// 		if (pDownloader == pDFile)
// 		{
// 			m_LoadFiles.erase(ppDownloader + i);
// 			return;
// 		}
// 	}
//}

//-------------------------------------------------------------------
//方法: OnDownLoadFileProc()
//参数: UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled
//描述: 下载普通文件线程消息调用的方法
//-------------------------------------------------------------------
// LRESULT CVGS2Window::OnDownLoadFileProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// {
	
// 	switch (wParam)
// 	{
// 		//Logo文件
// 	case TYPE_LOGOFILE:
// 		//创建联盟Logo 
// 		{
// 			RECT rect = {0, 80, 100, 160};
// 			CreateUnitedLogo(UnitedLogo_pDownLoadFile, &rect);
// 		}
// 		break;
// 	case TYPE_IMAGEFILE:
//         //下载完一幅贴图创建材质 
// 		{
// 			//将下载的图片文件
// 			CDownLoadFile** ppFile = m_LoadFiles.begin();
// 			CDownLoadFile* pFile = *(ppFile + lParam);
// 			m_pVGSCore->vgsScene->SetTexFromBuffer(pFile->m_fileBuf, pFile->m_fileSize, (CTexture*)pFile->m_DownForObj);
// 			EraseFileDownloader(pFile);
// 
// 			if (m_LoadFiles.size() > 0)
// 			{
// 				pFile = *(m_LoadFiles.begin());
//                 //设置下载下一个下载队列文件
// 				pFile->DownLoadHttpFile(pFile->file, NULL, m_hWnd, 1);
// 			}
// 
// 		}
// 		break;
// 	case TYPE_OTHERFILE:
// 		break;
// 		
// 	}
	
// 	return S_OK;
// }

//-------------------------------------------------------------------
//方法: GetMouseOverButton()
//参数: INT x, INT y
//描述: 判断鼠标在哪个按钮上方
//-------------------------------------------------------------------	
// CButton* CVGS2Window::GetMouseOverButton(INT x, INT y)
// {
// 	UINT btnCnt = m_pVGSCore->vgsScene->button.size();
// 	CButton** ppButton = m_pVGSCore->vgsScene->button.begin();
// 	
// 	for (UINT i= 0; i< btnCnt; i++)
// 	{
// 		CButton* pButton = *(ppButton + i);
// 		RECT rect;
// 		if (pButton->enable && pButton->visible) // baojh 2007-10-10 排除不可用和不可见的按钮
// 		{
// 			pButton->btnOverlay->GetRect(&rect);
// 			//判断光标位置是否在pButton上
// 			if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom)
// 				return pButton;
// 		}
// 	}
// 	return NULL;
// }

//-------------------------------------------------------------------
//方法: InitVars()
//参数: 
//描述: 初始化各种变量
//-------------------------------------------------------------------
HRESULT CVGS2Window::InitVars()
{	
	m_DownloadFileManagerMT.ClearAllTask();
	m_vFilesUseProgressbar.clear();
	m_FileLoadManagerMT.Init();
	
	m_dwTotalProgressbar = 0;
	m_sCurrentURLPath = "";

// 	m_ShowDefaultLogo = TRUE;
// 	m_bUnitedLogoStartDown = FALSE;
// 	UnitedLogo_pDownLoadFile = NULL;
// 	
// 	m_bModelEventOnMouseMove = TRUE;
// 	
// 	m_timeStep = 40;
// 	m_timer1 = 0;
// 	m_timer2 = 0;
// 	m_percent = 0;
// 	m_UserEventTimeStep = 200;
// 	
// 	m_pButton = NULL;
// 	m_pOverlay = NULL;
// 	
// 	m_pCurCursorModel = NULL;	
// 	m_pLastCursorModel = NULL;	
// 	
// 	m_pCurSelectedModel = NULL;			
// 	m_pLastSelectedModel = NULL;
// 	m_pCurN3D = NULL;
// 	
// 	modelCntStr = NULL;
// 	texCntStr = NULL;
// 	shaderCntStr = NULL;
// 	m_pSelectedNavigater = NULL;
// 	m_bNavPointerSelected = FALSE;
// 	m_pSelectedNavPointer = NULL;
// 	m_IsCreasing = FALSE;
// 	
// 	//鼠标句柄
// 	m_hHandCursor = LoadCursor(NULL, MAKEINTRESOURCE(32649));	
// 	m_hDefaultCursor = LoadCursor(NULL, IDC_ARROW);

 	return S_OK;
}

//-------------------------------------------------------------------
//方法: InitScene()
//参数: 
//描述: 初始化三维场景
//-------------------------------------------------------------------
HRESULT CVGS2Window::InitScene()
{
	RECT clientRect;
	GetClientRect(&clientRect);
	//for test
	clientRect.right = 1024;
	clientRect.bottom = 768;

	if (FALSE == m_pVGSCore->InitDefaultRenderForm(m_hWnd))  // 初始化VGS引擎，创建一个缺省的渲染窗体
	{
		return E_FAIL;
	}

	// 初始化GUI对象
	m_pVGSCore->SunInitGUIStatus();
		
	// CVGS2DLL::getSingleton() = CVGS2DLL::getSingleton();
	
// 	if (m_pVGSCore->vgsScene == NULL)
// 		return E_FAIL;
	
 	return S_OK;
}

//-------------------------------------------------------------------
//方法: InitOther()
//参数: 
//描述: 初始化其他内容
//-------------------------------------------------------------------
VOID CVGS2Window::InitOther()
{
	//设置场景的背景色
	//m_pVGSCore->SunSetBackColor(0,0,0);
	
	//初始化下载场景时的LOGO
//	if (m_ShowDefaultLogo)
//	{
		//m_pVGSCore->CreateLogo();
//	}
	
	//设置后缓冲尺寸
// 	RECT clientRect;
// 	GetClientRect(&clientRect);
// 	m_pVGSCore->SunUpdateBackBuffer(&clientRect);
// 
// 	//set view status---------------------------------------
// 	m_controlStatus = WALK;
// 	m_pVGSCore->SetControlStatus(m_controlStatus);
// 	
// 	//create text for download percent----------------------
// 	strcpy(m_strPercent, "正在下载, 请稍候...");
// 	m_downText = m_pVGSCore->SunNewText(m_strPercent);
// 	//放在Logo的下面
// 
// 	m_pVGSCore->SunSetTextPosition(m_downText, 10, 10);
// 	m_pVGSCore->SunSetTextColor(m_downText, D3DCOLOR_RGBA(255, 255, 255, 208));
	
	/*
	//调试场景管理器用
	modelCntStr = m_pVGSCore->SunNewText("Scene Model Count:");
	m_pVGSCore->SunSetTextPosition(modelCntStr, 0, 0);
	CText* pText = (CText*)modelCntStr;
	pText->visible = FALSE;
	
	  shaderCntStr = m_pVGSCore->SunNewText("Scene shader Count:");
	  m_pVGSCore->SunSetTextPosition(shaderCntStr, 0, 20);
	  pText = (CText*)shaderCntStr;
	  pText->visible = FALSE;
	  
		texCntStr = m_pVGSCore->SunNewText("Scene Texture Count:");
		m_pVGSCore->SunSetTextPosition(texCntStr, 0, 40);
		pText = (CText*)texCntStr;
		pText->visible = FALSE;
		
		  triCntStr = m_pVGSCore->SunNewText("Scene Triangle Count:");
		  m_pVGSCore->SunSetTextPosition(triCntStr, 0, 60);
		  pText = (CText*)triCntStr;
		  pText->visible = FALSE;
		  
			curCamFromStr = m_pVGSCore->SunNewText("Current Camera From:");
			m_pVGSCore->SunSetTextPosition(curCamFromStr, 0, 80);
			pText = (CText*)curCamFromStr;
			pText->visible = FALSE;
			
			  curCamAtStr = m_pVGSCore->SunNewText("Current Camera At:");
			  m_pVGSCore->SunSetTextPosition(curCamAtStr, 0, 100);
			  pText = (CText*)curCamAtStr;
			  pText->visible = FALSE;
	*/
	
	//设置场景可以渲染
	//m_pVGSCore->SunSetRenderStatus(TRUE);
}

//创建联盟Logo 
// VOID CVGS2Window::CreateUnitedLogo(CDownLoadFile* pLogoDownLoadFile, RECT* pRect)
// {
// 	if (pLogoDownLoadFile == NULL) return;
// 	
// 	m_pVGSCore->vgsScene->m_pUnitedLogo = 
// 		(COverlay*)m_pVGSCore->SunNewOverlayFromBuf("UnitedLogo", 
// 		pLogoDownLoadFile->m_fileBuf, 
// 		pLogoDownLoadFile->m_fileSize,
// 		NULL, 0, 
// 		ALIGN_NONE, 
// 		pRect);
// 	m_pVGSCore->vgsScene->m_pUnitedLogo->offsetOrigin = 0;
// 	m_pVGSCore->vgsScene->m_pUnitedLogo->useRelativeSize = FALSE;
// 	
//}

//-------------------------------------------------------------------
//方法: InitControlButton()
//参数: 
//描述: 初始化控制按钮
//-------------------------------------------------------------------
// VOID CVGS2Window::InitControlButton()
// {	
// 	RECT clientRect;
// 	GetClientRect(&clientRect);
// 	
// 	//设置控制按钮的默认位置为右下角
// 	DWORD CTRL_ROS = m_pVGSCore->SunGetOverlayHandleByName("CTRL_ROS");
// 	m_pVGSCore->vgsScene->pRosOl = (COverlay*)CTRL_ROS;
// 	if (m_pVGSCore->vgsScene->pRosOl != NULL)
// 	{
// 		RECT tRect;
// 		RECT overlayRect;
// 		INT tBorder = 18;
// 		m_pVGSCore->vgsScene->pRosOl->GetRect(&overlayRect);
// 		tRect.right = clientRect.right - tBorder;
// 		tRect.bottom = clientRect.bottom - tBorder;
// 		tRect.left = tRect.right - (overlayRect.right - overlayRect.left);
// 		tRect.top = tRect.bottom - (overlayRect.bottom - overlayRect.top);
// 		m_pVGSCore->SunSetOverlayRect(CTRL_ROS, &tRect);
// 	}
// 	
// 	DWORD CTRL_ARROW = m_pVGSCore->SunGetOverlayHandleByName("CTRL_ARROW");
// 	m_pVGSCore->vgsScene->pCtrlOl = (COverlay*)CTRL_ARROW;
// 	if (m_pVGSCore->vgsScene->pCtrlOl != NULL)
// 	{
// 		RECT tRect;
// 		RECT overlayRect;
// 		INT tBorder = 18;
// 		m_pVGSCore->vgsScene->pCtrlOl->GetRect(&overlayRect);
// 		tRect.right = clientRect.right - tBorder;
// 		tRect.bottom = clientRect.bottom - tBorder;
// 		tRect.left = tRect.right - (overlayRect.right - overlayRect.left);
// 		tRect.top = tRect.bottom - (overlayRect.bottom - overlayRect.top);
// 		m_pVGSCore->SunSetOverlayRect(CTRL_ARROW, &tRect);
// 	}
//}

//---------------------------------------------------------------------------------------------------
//方法: DestroyOther()
//参数: 
//描述: 释放其它资源
//----------------------------------------------------------------------------------------------------
// VOID CVGS2Window::DestroyFlash()
// {
	//清空m_Flash中的Flash对象
// 	for (UINT i = 0; i< m_Flash.size(); i++)
// 	{
// 		CATLFlashCtrl* pFlash = *(m_Flash.begin() + i);
// 		//pFlash->Stop();
// 		//SAFE_DELETE(pFlash);
// 		
// 		pFlash->Delete();
// 		SAFE_RELEASE(pFlash);
// 		SAFE_DELETE(pFlash);  
// 	}
// 	m_Flash.clear();
//}

//停止并释放N3D下载队列
// VOID CVGS2Window::DestroyN3D()
// {
	//释放N3D文件下载队列
// 	INT N3DCnt = m_LoadN3DObj.size();
// 	CDownLoadN3DFile** ppN3D = m_LoadN3DObj.begin();
// 	for (INT i = 0; i< N3DCnt; i++)
// 	{
// 		CDownLoadN3DFile* pN3D = *(ppN3D + i);
// 		if (pN3D->m_bExecuteOver == FALSE)
// 		{
// 			pN3D->m_bTerminated = TRUE;
// 			WaitForSingleObject(pN3D->m_threadHandle, INFINITE);
// 			SAFE_RELEASE(pN3D);
// 		}
// 		SAFE_DELETE(pN3D);
// 	}
// 	m_LoadN3DObj.clear();
//}
//停止并释放FileDownload下载队列
// VOID CVGS2Window::DestroyDownloadFile()
// {
	//释放N3D文件下载队列
// 	INT downLoadCnt = m_LoadFiles.size();
// 	CDownLoadFile** ppDownLoadFile = m_LoadFiles.begin();
// 	for (INT i = 0; i< downLoadCnt; i++)
// 	{
// 		CDownLoadFile* pDownLoadFile = *(ppDownLoadFile + i);
// 		if (pDownLoadFile->m_bExecuteOver == FALSE)
// 		{
// 			pDownLoadFile->m_bTerminated = TRUE;
// 			WaitForSingleObject(pDownLoadFile->m_threadHandle, INFINITE);
// 			SAFE_RELEASE(pDownLoadFile);
// 		}
// 		
// 		SAFE_DELETE(pDownLoadFile);
// 	}
// 	m_LoadFiles.clear();
//}


//---------------------------------------------------------------------------------------------------
//方法: SetDownPercentText()
//参数: 
//描述: 设置下载百分比的文字
//----------------------------------------------------------------------------------------------------	
// VOID CVGS2Window::SetDownPercentText()
// {
// 	
// 	if (m_pCurN3D && !m_pCurN3D->m_bAllDown)
// 	{
// 		m_curDownSize = m_pCurN3D->curPos;
// 		if (m_curDownSize == m_pCurN3D->m_fileSize || m_curDownSize == 0)
// 			return;
// 
// 		INT curPrecent;
// 		if(!m_pCurN3D->bSceneIsDownInvoked)
// 		{
// 		  //当前模型下载进度值
// 		  curPrecent = (INT)((1.0f * m_curDownSize)/ (m_pCurN3D->m_fileSize - m_pCurN3D->m_Header_5_1.ImageDataLen) * 100);  
// 		  //更新进度条
// 		  RECT barRect;
// 		  if(m_pVGSCore->vgsScene->m_pLoadingBar)
// 		  {
// 			  m_pVGSCore->vgsScene->m_pLoadingBar->GetRect(&barRect);	  
// 			  barRect.right = barRect.left + (INT)(curPrecent * 128/ 100);
// 			  m_pVGSCore->vgsScene->m_pLoadingBar->SetRect(&barRect);
// 		  }
// 		}else
// 		{
// 		  curPrecent = (INT)((1.0f * (m_curDownSize - (m_pCurN3D->m_fileSize - m_pCurN3D->m_Header_5_1.ImageDataLen)))/  m_pCurN3D->m_Header_5_1.ImageDataLen * 100);  
// 		}
// 		
// 		m_percent = (INT)((1.0f * m_curDownSize)/(m_pCurN3D->m_fileSize) * 100);
// 		
// 
// 		//设置Download文字
// 		if (m_percent <= 100)
// 		{
// 			_gcvt(curPrecent, 3, m_strPercent);
// 			itoa(curPrecent, m_strPercent, 10);
//             if(!m_pCurN3D->bSceneIsDownInvoked)
// 			
// 				strcpy(m_strDownLoading, "场景下载进度-");		
// 			else{
// 			    
// 				strcpy(m_strDownLoading, "材质下载进度-");
// 			}
// 			strcat(m_strDownLoading, m_strPercent);
// 			strcat(m_strDownLoading, "%");
// 			m_pVGSCore->SunSetTextString(m_downText, m_strDownLoading);
// 
// 			
// 			//调用JavaScript函数，把当前下载的百分比传到网页
// 			DownloadInvoke(m_strPercent);
// 		}
// 	}
// 	
//}

//----------------------------------------------------------------------------  
//	名称：GetTmpFolder
//	简介：获取IE的临时目录
//	参数：strTmpFolder:o:保存临时马路的变量
//	返回：无
//----------------------------------------------------------------------------  
// VOID CVGS2Window::GetTmpFolder(char* strTmpFolder)
// {   
	//获取系统的临时目录
//     char buf[512];
//     GetTempPath(sizeof(buf), buf);
// 	
// 	UINT Len = 0;
// 	while (buf[Len] != '\0')
// 		Len++;
//     //转换成IE的临时目录
// 	buf[Len-5] = '\0';
// 	strcat(buf, "Temporary Internet Files\\");
// 	strcpy(strTmpFolder, buf);
//}
//-------------------------------------------------------------------
//	Name:SetCurrentPath
//	Introduce:Set current URL path
//-------------------------------------------------------------------
HRESULT CVGS2Window::SetCurrentPath(const std::string &sCurrentPath)
{
	m_sCurrentURLPath = sCurrentPath;
	m_DownloadFileManagerMT.SetSourcePath(sCurrentPath);
	return S_OK;
}
//-------------------------------------------------------------------
//	Name:AddFileToProgressbar
//	Introduce:Add file name which should show downloading progress to list
//-------------------------------------------------------------------
HRESULT CVGS2Window::AddFileToProgressbar(const std::string &sFileURL)
{
	m_vFilesUseProgressbar.insert(sFileURL);

	return S_OK;
}
//-------------------------------------------------------------------
//	Name:DownloadAndLoadN3DFile
//	Introduce:Create a download task to down a N3D file
//-------------------------------------------------------------------
HRESULT CVGS2Window::DownloadAndLoadN3DFile(const std::string &sURL, const std::string sN3DName)
{
	CFileDwonloadInfo* pDownloadInfo = new CFileDwonloadInfo();
	pDownloadInfo->sName = sN3DName;
	pDownloadInfo->sLocalFile = "";
	pDownloadInfo->loadType = VGS_N3DLOAD_TYPE_LOAD;
	pDownloadInfo->dwTotalBytes = 0;
	pDownloadInfo->dwFinishedBytes = 0;

	// 创建下载任务
	VGSFileDownloadTaskInfo *pTaskInfo = m_DownloadFileManagerMT.CreateVGSFileDownloadTask(	false, 
																							sN3DName, 
																							sURL, 
																							VGS_DOWNLOADFILE_TYPE_N3D,
																							VGS_FILE_USAGE_N3D,
																							pDownloadInfo);
	if (NULL != pTaskInfo)
	{	
		// 该文件的大小
		DWORD dwFileSize = pDownloadInfo->dwTotalBytes;

		// 检查是否需要在进度条中显示进度
		set<string>::iterator it = m_vFilesUseProgressbar.find(sURL);
		if (it != m_vFilesUseProgressbar.end())
		{
			pDownloadInfo->bDisplayProgress = true;
			m_dwTotalProgressbar += dwFileSize;
		}
		m_vFileDownloadInfoMT.push_back(pDownloadInfo);
	}
	else
	{
		delete pDownloadInfo;
		pDownloadInfo = NULL;
	}

	return S_OK;
}
//-------------------------------------------------------------------
//	Name:DownloadAndLoadA3DFile
//	Introduce:Create a download task to down a A3D file
//-------------------------------------------------------------------
HRESULT CVGS2Window::DownloadAndLoadA3DFile(const std::string &sURL, const std::string sA3DName)
{
	CFileDwonloadInfo* pDownloadInfo = new CFileDwonloadInfo();
	pDownloadInfo->sName = sA3DName;
	pDownloadInfo->sLocalFile = "";
	pDownloadInfo->loadType = VGS_N3DLOAD_TYPE_LOAD;
	pDownloadInfo->dwTotalBytes = 0;
	pDownloadInfo->dwFinishedBytes = 0;

	// 创建下载任务
	VGSFileDownloadTaskInfo *pTaskInfo = m_DownloadFileManagerMT.CreateVGSFileDownloadTask(	false, 
																							sA3DName, 
																							sURL, 
																							VGS_DOWNLOADFILE_TYPE_A3D,
																							VGS_FILE_USAGE_A3D,
																							pDownloadInfo);
	if (NULL != pTaskInfo)
	{	
		// 该文件的大小
		DWORD dwFileSize = pDownloadInfo->dwTotalBytes;

		// 检查是否需要在进度条中显示进度
		set<string>::iterator it = m_vFilesUseProgressbar.find(sURL);
		if (it != m_vFilesUseProgressbar.end())
		{
			pDownloadInfo->bDisplayProgress = true;
			m_dwTotalProgressbar += dwFileSize;
		}
		m_vFileDownloadInfoMT.push_back(pDownloadInfo);
	}
	else
	{
		delete pDownloadInfo;
		pDownloadInfo = NULL;
	}

	return S_OK;
}
//-------------------------------------------------------------------
//	Name:ReadLocalN3DFile
//	Introduce:Load a local N3D file and costruct scene by DownloadInfo
//-------------------------------------------------------------------
HRESULT CVGS2Window::ReadLocalN3DFile(CFileDwonloadInfo *pInfo)
{
	if (NULL == pInfo)
		return E_FAIL;

	if (pInfo->sLocalFile.length() == 0)
		return E_FAIL;

	FILE* pFile = fopen(pInfo->sLocalFile.c_str(), "rb");
	if (pFile == NULL) 
	{
		std::string errStr = "Open N3D file ";
		errStr.append(pInfo->sLocalFile);
		errStr.append(" failed!");
		MessageBox(LPCTSTR(errStr.c_str()), LPCTSTR("Open N3D error"), MB_OK);
		return E_FAIL;
	}
	
	//判断N3D文件版本号
	N3D_Header N3DHeader;
	fread(&N3DHeader, sizeof(N3DHeader), 1, pFile);	
	if (N3DHeader.Version >= 51000.0f && (N3DHeader.Version <= N3D_CURRENT_V3DVERSION/*52000.0f*/ ))
	{		
		CGetN3DFileInfo_5 *pGetN3DFile = new CGetN3DFileInfo_5();
		if (!pGetN3DFile)
		{
			return E_FAIL;
		}
		pGetN3DFile->SetVGSCore(m_pVGSCore);
		pGetN3DFile->SetHeaderInfo(N3DHeader);

		// 解压并构建场景
        fseek(pFile, N3DHeader.SceneDataPtr, SEEK_SET);
        BYTE *pSceneDataZipBuffer = new BYTE[N3DHeader.SceneZipDataLen];
		fread(pSceneDataZipBuffer, N3DHeader.SceneZipDataLen, 1, pFile);  // 把压缩后的数据读进来		
		pGetN3DFile->UnzipSceneData(pSceneDataZipBuffer, N3DHeader.SceneZipDataLen, N3DHeader.OriginSceneDataLen);  // 解压数据
		SAFE_DELETE_ARRAY(pSceneDataZipBuffer);
		//
		pGetN3DFile->ConstructScene(pInfo->sName); 
	
		// 缺省的视口和相机
		DWORD pVP = m_pVGSCore->GetDefaultViewport();
		if (m_pVGSCore->SunGetViewportCamera(pVP) == 0)
		{
			DWORD cam = 0; 
			DWORD curController = m_pVGSCore->SunGetCurrentController();
			if (curController != 0)
			{
				cam = m_pVGSCore->SunGetControllerCamera(curController);
			}
			if (cam == 0)
			{
				cam = m_pVGSCore->SunCreatePredefinedCamera(PERSPECTIVE_CAMERA);
			}

			m_pVGSCore->SunSetViewportCamera(pVP, cam); 
		}

		// 读取贴图
		fseek(pFile, N3DHeader.ImageDataPtr, SEEK_SET);
		BYTE *pImageData = new BYTE[N3DHeader.ImageDataLen];
		fread(pImageData, N3DHeader.ImageDataLen, 1, pFile);  // 读入image信息
		pGetN3DFile->ReadImageData(pImageData);
		SAFE_DELETE_ARRAY(pImageData);
		
		fclose(pFile);
		
		//删除Logo对象
		m_pVGSCore->DeleteLogo();
		//删除下载进度文字对象
		m_pVGSCore->SunDelText(m_downText);
		
		// 发送消息回调SceneIsDown 和 AllIsDown
		PostMessage(WM_JSFUNC_MSG_SCENEISDOWN, 0, DWORD(pInfo));
		PostMessage(WM_JSFUNC_MSG_ALLISDOWN, 0, DWORD(pInfo));
     }

	return S_OK;
}
//-------------------------------------------------------------------
//	Name:ReadLocalA3DFile
//	Introduce:加载本地的A3D文件
//-------------------------------------------------------------------
HRESULT	CVGS2Window::ReadLocalA3DFile(CFileDwonloadInfo *pInfo)
{
	if (NULL == pInfo)
		return E_FAIL;

	CScene* pVGSScene = (CScene*)m_pVGSCore->m_vgsScene;
	if (NULL == pVGSScene) 
		return E_FAIL;

	//进入渲染保留区************************
	EnterCriticalSection(m_pCtrl_cs);
	EnterCriticalSection(m_pRender_cs);
	pVGSScene->m_A3DManager.loadA3DFile(pInfo->sLocalFile, pInfo->sName);
	//离开渲染保留区************************
	LeaveCriticalSection(m_pCtrl_cs);
	LeaveCriticalSection(m_pRender_cs);

	//发送消息回调SceneIsDown 和 AllIsDown
	PostMessage(WM_JSFUNC_MSG_SCENEISDOWN, 0, DWORD(pInfo));
	PostMessage(WM_JSFUNC_MSG_ALLISDOWN, 0, DWORD(pInfo));

	return S_OK;
}
//-------------------------------------------------------------------
//	Name:SceneIsDownInvoke
//	Introduce:处理场景文件下载完毕消息
//-------------------------------------------------------------------
HRESULT	CVGS2Window::SceneIsDownInvoke(const std::string &sN3DName)
{	
	//------------------------------------------------------------------------
	//初始化控制按钮的位置
// 	InitControlButton();
// 	
// 	//为了提高查找模型的算法的速度，在这里先对模型列表进行排序, 从大到小排序
// 	sort(m_pVGSCore->vgsScene->model.begin(), m_pVGSCore->vgsScene->model.end(), SortModleByName);
// 	//------------------------------------------------------------------------
// 	
// 	if (invokeJS)
// 	{
// 		_bstr_t bst("SceneIsDown");
// 		BSTR_SceneIsDown = bst.copy();
// 		
// 		memset(&dispparams, 0, sizeof(dispparams));
// 		dispparams.cArgs = 1;
// 		
// 		dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 		CComBSTR bstr = pN3D->name;
// 		bstr.CopyTo(&dispparams.rgvarg[0].bstrVal);
// 		dispparams.rgvarg[0].vt = VT_BSTR;
// 		
// 		dispparams.cNamedArgs = 0;
// 		
// 		invokeJS->InvokeJSFunc(BSTR_SceneIsDown, &dispparams); 
// 		
// 		SAFE_DELETE_ARRAY(dispparams.rgvarg);
// 	}
	
	return S_OK;
}
//-------------------------------------------------------------------
//N3D文件已经下载完成，调用JS函数
//-------------------------------------------------------------------
HRESULT	CVGS2Window::AllIsDownInvoke(const std::string &sN3DName)
{
// 	_bstr_t bst("AllIsDown");
// 	BSTR_AllIsDown = bst.copy();
// 	
// 	memset(&dispparams, 0, sizeof dispparams);
// 	dispparams.cArgs = 1;
// 	
// 	dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 	CComBSTR bstr = pN3D->name;
// 	bstr.CopyTo(&dispparams.rgvarg[0].bstrVal);
// 	dispparams.rgvarg[0].vt = VT_BSTR;
// 	
// 	dispparams.cNamedArgs = 0;
// 	
// 	invokeJS->InvokeJSFunc(BSTR_AllIsDown, &dispparams); 
// 	
// 	SAFE_DELETE_ARRAY(dispparams.rgvarg);
	
	return S_OK;
}

//鼠标按下模型或Overlay，调用JS函数
// HRESULT	CVGS2Window::MouseDownInvoke()
// {	
// 	_bstr_t bst("VGSMouseDown");
// 	BSTR_VGSMouseDown = bst.copy();
// 	
// 	memset(&dispparams, 0, sizeof(dispparams));
// 	dispparams.cArgs = 6;
// 	dispparams.rgvarg = new VARIANT[dispparams.cArgs]; //注意每个变量都一定要赋值，否则会出错
// 	
// 	CComBSTR bstr = NULL;
// 	
// 	//第0个参数为，当前光标下方的可点击物体的名称, 注意次序正好和JS中参数次序相反, JS中最后一个参数为第0个参数
// 	//如果点击在Overlay上，则返回Overlay的名称
// 	//------------先判断是否点击在Overlay上
// 	//JS第3个参数为鼠标点中的模型类型
// 	DWORD overlayHandle = m_pVGSCore->SunGetOverlayUnderLoc(m_pVGSCore->vgsScene->InputDevice.mouseInfo.X, m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y);
// 	if (overlayHandle != 0)
// 	{
// 		char overlayName[32];
// 		m_pVGSCore->SunGetOverlayName(overlayHandle, overlayName);
// 		bstr = overlayName;
// 		
// 		dispparams.rgvarg[3].lVal = 0;   //0代表点中了Overlay
// 		dispparams.rgvarg[3].vt = VT_INT;
// 		
// 		dispparams.rgvarg[1].lVal = -1;
// 		dispparams.rgvarg[1].vt = VT_INT;
// 		
// 		dispparams.rgvarg[0].lVal = -1;
// 		dispparams.rgvarg[0].vt = VT_INT;	
// 	}
// 	else
// 	{
// 		DWORD modelHandle = m_pVGSCore->SunGetModelUnderLoc(m_pVGSCore->vgsScene->InputDevice.mouseInfo.X, m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y);
// 		
// 		CModel* pModel = (CModel*)modelHandle;
// 		if (pModel) 
// 		{
// 			//第三个参数为模型或者Overlay的标示//0代表点中了Overlay//1代表点中了model
// 			CHAR modelName[32];
// 			m_pVGSCore->SunGetModelNameByHandle(modelHandle, modelName);
// 			bstr = modelName; 
// 			dispparams.rgvarg[3].lVal = 1;   
// 			dispparams.rgvarg[3].vt = VT_INT;
// 			
// 			dispparams.rgvarg[1].lVal = pModel->ID1;
// 			dispparams.rgvarg[1].vt = VT_INT;
// 			
// 			dispparams.rgvarg[0].lVal = pModel->ID2;
// 			dispparams.rgvarg[0].vt = VT_INT;
// 		}
// 		else
// 		{
// 			dispparams.rgvarg[3].lVal = -1;   
// 			dispparams.rgvarg[3].vt = VT_INT;
// 			
// 			dispparams.rgvarg[1].lVal = -1;
// 			dispparams.rgvarg[1].vt = VT_INT;
// 			
// 			dispparams.rgvarg[0].lVal = -1;
// 			dispparams.rgvarg[0].vt = VT_INT;
// 			
// 		}	
// 	}
// 	
// 	//JS第4个参数为模型
// 	bstr.CopyTo(&dispparams.rgvarg[2].bstrVal);
// 	dispparams.rgvarg[2].vt = VT_BSTR;
// 	
// 	//JS第2个参数为，当前光标在Client区域的Y位置
// 	dispparams.rgvarg[4].lVal = m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y;
// 	dispparams.rgvarg[4].vt = VT_INT;
// 	
// 	//JS第1个参数为, 当前光标在Client区域的X位置
// 	dispparams.rgvarg[5].lVal = m_pVGSCore->vgsScene->InputDevice.mouseInfo.X;
// 	dispparams.rgvarg[5].vt = VT_INT;
// 	
// 	dispparams.cNamedArgs = 0;
// 	
// 	invokeJS->InvokeJSFunc(BSTR_VGSMouseDown, &dispparams);
// 	
// 	SAFE_DELETE(dispparams.rgvarg);
	
// 	return S_OK;	
// }

//-------------------------------------------------------------------
//鼠标双击模型或Overlay，调用JS函数
//-------------------------------------------------------------------
 HRESULT CVGS2Window::MouseDBClickInvoke(int x, int y)
 {	
	 if (g_BrowserType == WBT_IE && invokeJS)  // ie browser
	 {
		 _bstr_t bst(sVGSMouseDBClick.c_str());// ("VGSMouseDBClick");
		 BSTR_VGSMouseDBClick = bst.copy();

		 memset(&dispparams, 0, sizeof(dispparams));
		 dispparams.cArgs = 6;
		 dispparams.rgvarg = new VARIANT[dispparams.cArgs]; //注意每个变量都一定要赋值，否则会出错

		 CComBSTR bstr = NULL;

		 //第0个参数为，当前光标下方的可点击物体的名称, 注意次序正好和JS中参数次序相反, JS中最后一个参数为第0个参数
		 //如果点击在Overlay上，则返回Overlay的名称
		 //------------先判断是否点击在Overlay上
		 //JS第3个参数为鼠标点中的模型类型
		 DWORD overlayHandle = m_pVGSCore->SunGetOverlayUnderLoc(x, y);
		 if (overlayHandle != 0)
		 {
			 CHAR overlayName[MAX_FILENAME_LENGTH];
			 ZeroMemory(overlayName, sizeof(overlayName));
			 m_pVGSCore->SunGetOverlayName(overlayHandle, overlayName);
			 bstr = overlayName;

			 dispparams.rgvarg[3].lVal = 0;   //0代表点中了Overlay
			 dispparams.rgvarg[3].vt = VT_INT;

			 dispparams.rgvarg[1].lVal = -1;
			 dispparams.rgvarg[1].vt = VT_INT;

			 dispparams.rgvarg[0].lVal = -1;
			 dispparams.rgvarg[0].vt = VT_INT;	
		 }
		 else
		 {
			 DWORD modelHandle = m_pVGSCore->SunGetModelUnderLoc(x, y);
			 if (modelHandle) 
			 {
				 //第三个参数为模型或者Overlay的标示//0代表点中了Overlay//1代表点中了model
				 CHAR modelName[MAX_FILENAME_LENGTH];
				 ZeroMemory(modelName, sizeof(modelName));
				 m_pVGSCore->SunGetModelNameByHandle(modelHandle, modelName);				 
				 bstr = modelName; 
				 dispparams.rgvarg[3].lVal = 1;   
				 dispparams.rgvarg[3].vt = VT_INT;

				 dispparams.rgvarg[1].lVal = 0;//pModel->ID1;
				 dispparams.rgvarg[1].vt = VT_INT;

				 dispparams.rgvarg[0].lVal = 0;//pModel->ID2;
				 dispparams.rgvarg[0].vt = VT_INT;
			 }
			 else
			 {
				 dispparams.rgvarg[3].lVal = -1;   
				 dispparams.rgvarg[3].vt = VT_INT;

				 dispparams.rgvarg[1].lVal = -1;
				 dispparams.rgvarg[1].vt = VT_INT;

				 dispparams.rgvarg[0].lVal = -1;
				 dispparams.rgvarg[0].vt = VT_INT;

			 }	
		 }

		 //JS第4个参数为模型
		 bstr.CopyTo(&dispparams.rgvarg[2].bstrVal);
		 dispparams.rgvarg[2].vt = VT_BSTR;

		 //JS第2个参数为，当前光标在Client区域的Y位置
		 dispparams.rgvarg[4].lVal = y;
		 dispparams.rgvarg[4].vt = VT_INT;

		 //JS第1个参数为, 当前光标在Client区域的X位置
		 dispparams.rgvarg[5].lVal = x;
		 dispparams.rgvarg[5].vt = VT_INT;

		 dispparams.cNamedArgs = 0;

		 invokeJS->InvokeJSFunc(BSTR_VGSMouseDBClick, &dispparams);

		 SAFE_DELETE(dispparams.rgvarg);
	 }

	 if (_NOT_MSIE(g_BrowserType))  // 非ie
	 {
		 if (g_pVGS2)
		 {
			 CComVariant pVar[7];
			 pVar[0] = CComVariant(sVGSMouseDBClick.c_str());  // func name
			 pVar[1] = CComVariant(x);  // 屏幕坐标x
			 pVar[2] = CComVariant(y);  // 屏幕坐标y
			 pVar[3] = CComVariant(-1); // 类型
			 pVar[4] = CComVariant(""); // 模型或overlay名称
			 pVar[5] = CComVariant(0);  // 如果是模型，ID1
			 pVar[6] = CComVariant(0);  // 如果是模型，ID2

			 DWORD dwHandle = 0;
			 //DWORD dwHandle = m_pVGSCore->SunGetOverlayUnderLoc(x, y);
			 //if (dwHandle != 0)
			 //{
				// pVar[3] = CComVariant(0);        // overlay
				// std::string sName = _addQuotes(((COverlay*)dwHandle)->getName());
				// pVar[4] = CComVariant(sName.c_str());
			 //}
			 //else
			 {
				 dwHandle = m_pVGSCore->SunGetModelUnderLoc(x, y); 
				 if (dwHandle != 0)
				 {
					 CHAR modelName[MAX_FILENAME_LENGTH];
					 ZeroMemory(modelName, sizeof(modelName));
					 m_pVGSCore->SunGetModelNameByHandle(dwHandle, modelName);
					 pVar[3] = CComVariant(1);
					 std::string str = std::string(modelName);
					 std::string sName = _addQuotes(str);
					 pVar[4] = CComVariant(sName.c_str());
					 pVar[5] = CComVariant((long)0/*(CModel*)dwHandle)->ID1*/);
					 pVar[6] = CComVariant((long)1/*(CModel*)dwHandle)->ID2*/);
				 }
			 }
			 g_pVGS2->Fire_DoScriptFunc(pVar, sizeof(pVar) / sizeof(pVar[0]));
		 }
	 }

	 return S_OK;
 }
//-------------------------------------------------------------------
//鼠标点击模型或Overlay，调用JS函数
//-------------------------------------------------------------------
// HRESULT	CVGS2Window::MouseUpInvoke()
// {	
// 	_bstr_t bst("VGSMouseUp");
// 	BSTR_VGSMouseUp = bst.copy();
// 	
// 	memset(&dispparams, 0, sizeof(dispparams));
// 	dispparams.cArgs = 6;
// 	dispparams.rgvarg = new VARIANT[dispparams.cArgs]; //注意每个变量都一定要赋值，否则会出错
// 	
// 	CComBSTR bstr = NULL;
// 	
// 	//第0个参数为，当前光标下方的可点击物体的名称, 注意次序正好和JS中参数次序相反, JS中最后一个参数为第0个参数
// 	//如果点击在Overlay上，则返回Overlay的名称
// 	//------------先判断是否点击在Overlay上
// 	//JS第3个参数为鼠标点中的模型类型
// 	DWORD overlayHandle = m_pVGSCore->SunGetOverlayUnderLoc(m_pVGSCore->vgsScene->InputDevice.mouseInfo.X, m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y);
// 	if (overlayHandle != 0)
// 	{
// 		char overlayName[32];
// 		m_pVGSCore->SunGetOverlayName(overlayHandle, overlayName);
// 		bstr = overlayName;
// 		
// 		dispparams.rgvarg[3].lVal = 0;   //0代表点中了Overlay
// 		dispparams.rgvarg[3].vt = VT_INT;
// 		
// 		dispparams.rgvarg[1].lVal = -1;
// 		dispparams.rgvarg[1].vt = VT_INT;
// 		
// 		dispparams.rgvarg[0].lVal = -1;
// 		dispparams.rgvarg[0].vt = VT_INT;
// 	}
// 	else
// 	{
// 		DWORD modelHandle = m_pVGSCore->SunGetModelUnderLoc(m_pVGSCore->vgsScene->InputDevice.mouseInfo.X, m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y);
// 		
// 		CModel* pModel = (CModel*)modelHandle;
// 		if (pModel) 
// 		{
// 			//第三个参数为模型或者Overlay的标示//0代表点中了Overlay//1代表点中了model
// 			CHAR modelName[32];
// 			m_pVGSCore->SunGetModelNameByHandle(modelHandle, modelName);
// 			bstr = modelName; 
// 			dispparams.rgvarg[3].lVal = 1;   
// 			dispparams.rgvarg[3].vt = VT_INT;
// 			
// 			dispparams.rgvarg[1].lVal = pModel->ID1;
// 			dispparams.rgvarg[1].vt = VT_INT;
// 			
// 			dispparams.rgvarg[0].lVal = pModel->ID2;
// 			dispparams.rgvarg[0].vt = VT_INT;
// 		}
// 		else
// 		{
// 			dispparams.rgvarg[3].lVal = -1;   
// 			dispparams.rgvarg[3].vt = VT_INT;
// 			
// 			dispparams.rgvarg[1].lVal = -1;
// 			dispparams.rgvarg[1].vt = VT_INT;
// 			
// 			dispparams.rgvarg[0].lVal = -1;
// 			dispparams.rgvarg[0].vt = VT_INT;
// 			
// 		}	
// 	}
// 	
// 	//JS第4个参数为模型
// 	bstr.CopyTo(&dispparams.rgvarg[2].bstrVal);
// 	dispparams.rgvarg[2].vt = VT_BSTR;
// 	
// 	//JS第2个参数为，当前光标在Client区域的Y位置
// 	dispparams.rgvarg[4].lVal = m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y;
// 	dispparams.rgvarg[4].vt = VT_INT;
// 	
// 	//JS第1个参数为, 当前光标在Client区域的X位置
// 	dispparams.rgvarg[5].lVal = m_pVGSCore->vgsScene->InputDevice.mouseInfo.X;
// 	dispparams.rgvarg[5].vt = VT_INT;
// 	
// 	dispparams.cNamedArgs = 0;
// 	
// 	invokeJS->InvokeJSFunc(BSTR_VGSMouseUp, &dispparams);
// 	
// 	SAFE_DELETE(dispparams.rgvarg);
	
// 	return S_OK;
// }

//鼠标移动的Invoke函数
// HRESULT	CVGS2Window::MouseMoveInvoke()
// {
// 	_bstr_t bst("VGSMouseMove");
// 	BSTR_VGSMouseMove = bst.copy();
// 	
// 	memset(&dispparams, 0, sizeof(dispparams));
// 	dispparams.cArgs = 6;
// 	dispparams.rgvarg = new VARIANT[dispparams.cArgs]; //注意每个变量都一定要赋值，否则会出错
// 	
// 	CComBSTR bstr = NULL;
// 	
// 	//第0个参数为，当前光标下方的可点击物体的名称, 注意次序正好和JS中参数次序相反, JS中最后一个参数为第0个参数
// 	//如果点击在Overlay上，则返回Overlay的名称
// 	//------------先判断是否点击在Overlay上
// 	//JS第3个参数为鼠标点中的模型类型
// 	DWORD overlayHandle = m_pVGSCore->SunGetOverlayUnderLoc(m_pVGSCore->vgsScene->InputDevice.mouseInfo.X, m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y);
// 	if (overlayHandle != 0)
// 	{
// 		char overlayName[32];
// 		m_pVGSCore->SunGetOverlayName(overlayHandle, overlayName);
// 		bstr = overlayName;
// 		
// 		dispparams.rgvarg[3].lVal = 0;   //0代表点中了Overlay
// 		dispparams.rgvarg[3].vt = VT_INT;
// 		
// 		dispparams.rgvarg[1].lVal = -1;
// 		dispparams.rgvarg[1].vt = VT_INT;
// 		
// 		dispparams.rgvarg[0].lVal = -1;
// 		dispparams.rgvarg[0].vt = VT_INT;	
// 	}
// 	else
// 	{
// 		if (m_bModelEventOnMouseMove)
// 		{
// 			DWORD modelHandle = m_pVGSCore->SunGetModelUnderLoc(m_pVGSCore->vgsScene->InputDevice.mouseInfo.X, m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y);
// 			
// 			CModel* pModel = (CModel*)modelHandle;
// 			if (pModel) 
// 			{
// 				//第三个参数为模型或者Overlay的标示//0代表点中了Overlay//1代表点中了model
// 				CHAR modelName[32];
// 				m_pVGSCore->SunGetModelNameByHandle(modelHandle, modelName);
// 				bstr = modelName; 
// 				dispparams.rgvarg[3].lVal = 1;   
// 				dispparams.rgvarg[3].vt = VT_INT;
// 				
// 				dispparams.rgvarg[1].lVal = pModel->ID1;
// 				dispparams.rgvarg[1].vt = VT_INT;
// 				
// 				dispparams.rgvarg[0].lVal = pModel->ID2;
// 				dispparams.rgvarg[0].vt = VT_INT;
// 			}
// 			else
// 			{
// 				dispparams.rgvarg[3].lVal = -1;   
// 				dispparams.rgvarg[3].vt = VT_INT;
// 				
// 				dispparams.rgvarg[1].lVal = -1;
// 				dispparams.rgvarg[1].vt = VT_INT;
// 				
// 				dispparams.rgvarg[0].lVal = -1;
// 				dispparams.rgvarg[0].vt = VT_INT;
// 				
// 			}	
// 		}
// 		else
// 		{
// 			dispparams.rgvarg[3].lVal = -1;   
// 			dispparams.rgvarg[3].vt = VT_INT;
// 			
// 			dispparams.rgvarg[1].lVal = -1;
// 			dispparams.rgvarg[1].vt = VT_INT;
// 			
// 			dispparams.rgvarg[0].lVal = -1;
// 			dispparams.rgvarg[0].vt = VT_INT;
// 		}
// 	}
// 	
// 	//JS第4个参数为模型
// 	bstr.CopyTo(&dispparams.rgvarg[2].bstrVal);
// 	dispparams.rgvarg[2].vt = VT_BSTR;
// 	
// 	//JS第2个参数为，当前光标在Client区域的Y位置
// 	dispparams.rgvarg[4].lVal = m_pVGSCore->vgsScene->InputDevice.mouseInfo.Y;
// 	dispparams.rgvarg[4].vt = VT_INT;
// 	
// 	//JS第1个参数为, 当前光标在Client区域的X位置
// 	dispparams.rgvarg[5].lVal = m_pVGSCore->vgsScene->InputDevice.mouseInfo.X;
// 	dispparams.rgvarg[5].vt = VT_INT;
// 	
// 	dispparams.cNamedArgs = 0;
// 	
// 	invokeJS->InvokeJSFunc(BSTR_VGSMouseMove, &dispparams);
// 	
// 	SAFE_DELETE(dispparams.rgvarg);
	
// 	return S_OK;
// }

//点击按钮，调用JS函数
//HRESULT	CVGS2Window::ButtonClickInvoke()
//{
// 	if (m_pVGSCore->vgsScene->pMouseOverButton == NULL) return E_FAIL;
// 	
// 	if (m_pVGSCore->vgsScene->pMouseOverButton->enable)
// 	{
// 		_bstr_t bst("");
// 		bst = m_pVGSCore->vgsScene->pMouseOverButton->callBackFunc;
// 		BSTR BSTR_BtnFunc = bst.copy();
// 		
// 		memset(&dispparams, 0, sizeof(dispparams));
// 		dispparams.cArgs = 0;
// 		dispparams.cNamedArgs = 0;
// 		invokeJS->InvokeJSFunc(BSTR_BtnFunc, &dispparams); 
// 		
// 		m_pVGSCore->vgsScene->pMouseOverButton = NULL;
// 	}
//  	return S_OK;
//  }

//下载回调函数
//HRESULT CVGS2Window::DownloadInvoke(CHAR* strPercent)
//{
// 	_bstr_t bst("OnDownload");
// 	BSTR BSTR_OnDownLoad = bst.copy();
// 	
// 	memset(&dispparams, 0, sizeof(dispparams));
// 	dispparams.cArgs = 1;
// 	
// 	dispparams.rgvarg = new VARIANT[dispparams.cArgs];
// 	CComBSTR bstr = strPercent;
// 	bstr.CopyTo(&dispparams.rgvarg[0].bstrVal);
// 	dispparams.rgvarg[0].vt = VT_BSTR;
// 	
// 	dispparams.cNamedArgs = 0;
// 	
// 	invokeJS->InvokeJSFunc(BSTR_OnDownLoad, &dispparams); 
// 	
// 	SAFE_DELETE_ARRAY(dispparams.rgvarg);
	
//	return S_OK;
//}

//-------------------------------------------------------------------
// 调用引擎开始的脚本
//-------------------------------------------------------------------
LRESULT CVGS2Window::InvokeStartEngine()
{
	if (g_BrowserType == WBT_IE && invokeJS)  // ie browser
	{	
		_bstr_t bst(sStartEngine.c_str());
		BSTR BSTR_Func = bst.copy();
		memset(&dispparams, 0, sizeof(dispparams));
		dispparams.cArgs = 0; 
		dispparams.cNamedArgs = 0;
		invokeJS->InvokeJSFunc(BSTR_Func, &dispparams); 
		return S_OK;
	}
	else if (_NOT_MSIE(g_BrowserType))  // 非ie
	{
		CComVariant var = CComVariant(sStartEngine.c_str());
		g_pVGS2->Fire_DoScriptFunc(&var, 1);
		return S_OK;
	}
	return E_FAIL;
}
//-------------------------------------------------------------------
/*
//
VOID    CVGS2Window::SortTheModelList()
{
  sort(m_pVGSCore->vgsScene->model.begin(), m_pVGSCore->vgsScene->model.end(), SortModleByName);
}
*/

//显示场景详细信息
/*VOID	CVGS2Window::ShowSceneDetailInfo()
{

if (m_pVGSCore->bShowDebugInfo)
{
//不透明模型
CMaterial** ppShader = m_pVGSCore->vgsScene->shader.begin();
int shaderSize = m_pVGSCore->vgsScene->shader.size();
int modelCnt = 0;
int ntriCnt = 0;
for (int i = 0; i< shaderSize; i++)
{
CMaterial* pMtrl = *(ppShader + i);
CModel** ppModel = (CModel**)(pMtrl->modelList.begin());
for (int j = 0; j< pMtrl->modelList.size(); j++)
{
CModel* pModel = *(ppModel + j);
if (!pModel->culled && 
pModel->visible && 
!pModel->isCollision && 
!pModel->isGround &&
pModel->shader->opacity == 100 &&
pModel->opacity == 100 &&
pModel->shader->compTex == NULL)
{
modelCnt++;
ntriCnt += pModel->faceCnt;
}
}
}
//透明模型
CModel** ppModel = m_pVGSCore->vgsScene->transModel.begin();
int transModelCnt = m_pVGSCore->vgsScene->transModel.size();
for (i = 0; i< transModelCnt; i++)
{
CModel* pModel = *(ppModel + i);
if (!pModel->culled && 
pModel->visible && 
!pModel->isCollision && 
!pModel->isGround)
{
modelCnt++;
ntriCnt += pModel->faceCnt;
}
}

		//模型数
		strcpy(str_modelCnt, "Render Model Count: ");
		char s_modelCnt[8];
		itoa(modelCnt, s_modelCnt, 10);
		strcat(str_modelCnt, s_modelCnt);
		m_pVGSCore->SunSetTextString(modelCntStr, str_modelCnt); 
		CText* pText = (CText*)modelCntStr;
		pText->visible = TRUE;
		
		  //三角面数量
		  strcpy(str_triCnt, "Visible Triangle Count: ");
		  char triCnt[8];
		  itoa(ntriCnt, triCnt, 10);
		  strcat(str_triCnt, triCnt);
		  m_pVGSCore->SunSetTextString(triCntStr, str_triCnt); 
		  pText = (CText*)triCntStr;
		  pText->visible = TRUE;
		  
			//材质数
			strcpy(str_shaderCnt, "Render Material Count: ");
			char shaderCnt[8];
			itoa(m_pVGSCore->vgsScene->shader.size(), shaderCnt, 10);
			strcat(str_shaderCnt, shaderCnt);
			m_pVGSCore->SunSetTextString(shaderCntStr, str_shaderCnt); 
			pText = (CText*)shaderCntStr;
			pText->visible = TRUE;
			
			  //贴图数量
			  strcpy(str_texCnt, "Render Texture Count: ");
			  char texCnt[8];
			  itoa(m_pVGSCore->vgsScene->texture.size(), texCnt, 10);
			  strcat(str_texCnt, texCnt);
			  m_pVGSCore->SunSetTextString(texCntStr, str_texCnt); 
			  pText = (CText*)texCntStr;
			  pText->visible = TRUE;
			  
				//相机from点
				sprintf(str_curCamFrom, "Current Camera From: x:%f, y:%f, z:%f",
				m_pVGSCore->vgsScene->currentCam->m_from.x,
				m_pVGSCore->vgsScene->currentCam->m_from.y,
				m_pVGSCore->vgsScene->currentCam->m_from.z);
				m_pVGSCore->SunSetTextString(curCamFromStr, str_curCamFrom); 
				pText = (CText*)curCamFromStr;
				pText->visible = TRUE;
				
				  //相机at点
				  
					sprintf(str_curCamAt, "Current Camera At: x:%f, y:%f, z:%f",
					m_pVGSCore->vgsScene->currentCam->m_at.x,
					m_pVGSCore->vgsScene->currentCam->m_at.y,
					m_pVGSCore->vgsScene->currentCam->m_at.z);
					
					  sprintf(str_curCamAt, "Current Frame: %d", m_pVGSCore->vgsScene->currentCam->kfrCurFrame);
					  m_pVGSCore->SunSetTextString(curCamAtStr, str_curCamAt); 
					  pText = (CText*)curCamAtStr;
					  pText->visible = TRUE;
					  }
					  else
					  {
					  CText* pText = (CText*)modelCntStr;
					  pText->visible = FALSE;
					  
						pText = (CText*)shaderCntStr;
						pText->visible = FALSE;
						
						  pText = (CText*)texCntStr;
						  pText->visible = FALSE;
						  
							pText = (CText*)triCntStr;
							pText->visible = FALSE;
							
							  pText = (CText*)curCamFromStr;
							  pText->visible = FALSE;
							  
								pText = (CText*)curCamAtStr;
								pText->visible = FALSE;
								}

}*/
//-------------------------------------------------------------------
// 判断窗口是否是flash窗口
//-------------------------------------------------------------------
bool CVGS2Window::IsFlashWnd(HWND wnd)
{
	/*UINT flashCnt = m_Flash.size();
	for (INT i = 0; i < flashCnt; i++)
	{
		if (m_Flash[i] && m_Flash[i]->getHandle() == wnd)
		{
			return true;
		}
	}*/
	return false;
}
//-------------------------------------------------------------------
//	Name:GetProgressbarFinishedBytes
//	Introduce:当前已经完成的进度
//-------------------------------------------------------------------	
DWORD CVGS2Window::GetProgressbarFinishedBytes()
{	
	DWORD dwTotal = 0;
	vector<CFileDwonloadInfo*>::iterator it = m_vFileDownloadInfoMT.begin();
	for (; it != m_vFileDownloadInfoMT.end(); ++it)
	{
		CFileDwonloadInfo *pInfo = *it;
		if (pInfo && pInfo->bDisplayProgress)
		{
			dwTotal += pInfo->dwFinishedBytes;
		}
	}

	return dwTotal;
}
//-------------------------------------------------------------------
//	Name:DeleteProgressbar
//	Introduce:删除进度条
//-------------------------------------------------------------------	
void CVGS2Window::DeleteProgressbar()
{
/*
	// 删除logo	
	DeleteLogo();	

	// 删除下载进度文字对象
	m_VGS2.SunDelText(m_downText);
	m_downText = NULL;
*/
	// 删除logo	
	m_pVGSCore->DeleteLogo();

	// 删除下载进度文字对象
	m_pVGSCore->SunDelText(m_downText);
	m_downText = NULL;
}
//-------------------------------------------------------------------
//	Name:ProcessProgress
//	Introduce:处理进度显示
//-------------------------------------------------------------------	
void CVGS2Window::ProcessProgress(float percent)
{
/*	
	//设置Download文字
	//if (m_percent <= 100)
	{
		ZeroMemory(m_strDownLoading, sizeof(m_strDownLoading));

		_gcvt(curPrecent, 3, m_strPercent);
		itoa(curPrecent, m_strPercent, 10);
		strncpy(m_strDownLoading, "下载进度-", sizeof(m_strDownLoading) - 1);
		strcat(m_strDownLoading, m_strPercent);
		strcat(m_strDownLoading, "%");
		m_VGS2.SunSetTextString(m_downText, m_strDownLoading);
		setDownloadbarPercent(curPrecent);

		// 调用JavaScript函数，把当前下载的百分比传到网页
		DownloadInvoke(m_strPercent);

		// 通知flash progressbar
		if (m_pFlashProgressBar && m_bFlashProgressEnable)
		{
			char tmp[32];
			ZeroMemory(tmp, sizeof(tmp));
			m_pFlashProgressBar->SetFlashVariable(m_sFlashProgreess, std::string(itoa(curPrecent, tmp, 10)));
		}

		// 渲染logo
		OnlyRenderLogo();
	}*/
}
//-------------------------------------------------------------------
