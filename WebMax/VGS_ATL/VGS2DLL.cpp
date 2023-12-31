//----------------------------------------------------------------------------------
//VGS2DLL.cpp CVGS2DLL类实现文件
//描述：VGS2版本引擎动态链接库
//作者: lixin
//日期: 2006 -4 -1
//-----------------------------------------------------------------------------------
#include "stdafx.h"
#include "VGS2DLL.h"
#include <algorithm>
using namespace std;

#include "VGSATLHeader.h" 

//#if _VGS_BUILDING_DLL
//# define VGS_EXPORT __declspec (dllexport)
//#else 
//# define VGS_EXPORT __declspec (dllimport)
//#endif

#ifdef _DEBUG
#pragma comment(lib, "VGSCore\\VGSCoreLibD.lib")
//#pragma comment(lib, "..\\bin\\VGSCoreDLL.lib")
#else
#pragma comment(lib, "VGSCore\\VGSCoreLib.lib")
#endif 


//使用静态链接
//#if defined(DEBUG) || defined(_DEBUG)
//#pragma comment(lib, "..//luaplus//LuaPlusLibD_1100.lib")//static lib
//#else
//#pragma comment(lib, "luaplus//LuaPlusLib_1100.lib")//static lib
//#endif  

//LensFlare的资源
// #include "Resource\\LS_SunImage.h"
// #include "Resource\\LS_Tex1Image.h"
// #include "Resource\\LS_Tex2Image.h"
// #include "Resource\\LS_Tex3Image.h"
#include "Resource\\LogoImage.h"
#include "Resource\\LoadingBarImage.h"
#include "Resource\\LoadingBgImage.h"

//音频
//#include "CAudioPlayer.h"
//vector<CAudioPlayer*> audioPlayer;
//#include "CMovieTexture.h"

//template <> CVGS2DLL* Singleton<CVGS2DLL>::ms_Singleton = 0;
//
//CVGS2DLL* CVGS2DLL::getSingletonPtr()
//{
//	return ms_Singleton;
//}
//CVGS2DLL& CVGS2DLL::getSingleton()
//{
//	assert(ms_Singleton);  
//	return (*ms_Singleton);
//}

CVGS2DLL::CVGS2DLL() 
{
 	m_vgsScene     = NULL;					//场景对象
	m_RenderTarget = 0;
	m_Viewport     = 0;

	controlStatus = CT_WALKER;

	m_bRender = FALSE;

	m_defaultViewport  = 0;
	m_defaultRenderWnd = 0;

// 	controlStatus = CT_NONE;				//当前用户控制状态值
// 	fAxisScale = 1.0f;					//缺省轴缩放值
// 	fZoomSpeed = 1.0f;
// 	
// 	bCollision = TRUE;					//碰撞是否有效
// 	bIsCollision = FALSE;				//当前是否有碰撞发生
// 	bWeight = TRUE;						//重力是否有效
// 	colDistance = 50.0f;
// 	
// 	walker_realMoveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
// 	walker_realRotateVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
// 	
// 	bFrameMoveCamera = FALSE;
// 	bShowDebugInfo = FALSE;
// 
// 	bCanZoom = TRUE;
// 	bCanMove = TRUE;
// 	bCanLift = TRUE;
// 	bCanRos = TRUE;
// 
// 	curColModel = NULL;	
// 	curGroundModel = NULL;	
// 	preGroundModel = NULL;
// 
 	// 第二套控制键
 	m_AddKeyLeft  = 'a';                  
 	m_AddKeyRight = 'd';
 	m_AddKeyUp    = 'w';
 	m_AddKeyDown  = 's';
 
 	m_bAddtionalKeysEnable = TRUE;  // 缺省状态下，第二套按键有效

	walker_realMoveVec = VECTOR3(0.0f, 0.0f, 0.0f);
	walker_realRotateVec = VECTOR3(0.0f, 0.0f, 0.0f);


}

CVGS2DLL::~CVGS2DLL()
{
	m_VGSCore.ExitVGS();
}

// 初始化缺省的渲染窗口
BOOL CVGS2DLL::InitDefaultRenderForm(HWND hWnd)
{
	if (SunInitVGS(hWnd))
	{
		m_vgsScene = SunCreateScene();
		// DWORD defaultCam = CreatePerspectiveCamera(m_vgsScene);
		if (m_vgsScene != 0)
		{
			m_defaultRenderWnd = m_VGSCore.CreateRenderWindow(m_vgsScene, m_hwnd, "VGSDefaultRenderWindow");
			if (m_defaultRenderWnd != 0)
			{
				m_defaultViewport = m_VGSCore.AddViewport(m_defaultRenderWnd, NULL, 0, 0.0f, 0.0f, 1.0f, 1.0f);
				m_Viewport = m_defaultViewport;
				return TRUE;
			}
		}
	}
	return FALSE;
}

// 获得缺省使用的视口
DWORD CVGS2DLL::SunGetDefaultViewport()
{
	return m_defaultViewport;
}

// 初始化GUI状态
VOID CVGS2DLL::SunInitGUIStatus()
{
	m_VGSCore.InitGUIStatus(m_vgsScene, m_defaultRenderWnd, m_Viewport);
}

// 初始化引擎
BOOL CVGS2DLL::SunInitVGS(HWND hWnd)
{
#ifdef _DEBUG
	BOOL bWriteLog = TRUE;
#else
	BOOL bWriteLog = FALSE;
#endif
	m_hwnd = hWnd;

	return m_VGSCore.InitVGS(hWnd, (INT)g_BrowserType, bWriteLog);
}

VOID CVGS2DLL::SunRelease()
{

}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunCreateScene()
// 参  数: 
// 用  途: 创建VGS场景
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunCreateScene(/*HWND hWnd, RECT* rect*/)
{	
	if (m_vgsScene)
	{
		m_VGSCore.DeleteScene(m_vgsScene);
	}

	// m_hwnd = hWnd;
	m_vgsScene = m_VGSCore.CreateScene("");

	m_mouseX = m_inputDevice.mouseInfo.X;
	m_mouseY = m_inputDevice.mouseInfo.Y;

	return m_vgsScene;

// 	if (vgsScene != NULL) 
// 	{
// 		vgsScene->ChangeDeviceWindow(hWnd, rect);
// 		return (DWORD)vgsScene;	
// 	}
// 	
// 	//construct scene obj
// 	vgsScene = new CScene();
// 	if (FAILED( vgsScene->AttachWindow(hWnd, rect)) ) 
// 	{
// 		SAFE_DELETE(vgsScene);
// 		MessageBox(GetActiveWindow(), "系统显存不足，无法打开新的三维窗体，请关闭其它三维窗体后再试。", "系统友好提示", MB_ICONEXCLAMATION | MB_OK);
// 		return 0;
// 	}
// 	
// 	//初始化默认的walker
// 	walker.m_pCamera = &(vgsScene->defaultCam);
// 	
// 	mouseX = m_inputDevice.mouseInfo.X;
// 	mouseY = m_inputDevice.mouseInfo.Y;

	// return (DWORD)vgsScene;
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunDeleteScene()
// 参  数: 
// 用  途: 清除VGS场景
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDeleteScene()
{
	if (m_vgsScene)
	{
		m_VGSCore.DeleteScene(m_vgsScene);
	}
	//release and delete d3d graphics obj
// 	SAFE_RELEASE(vgsScene);
// 	SAFE_DELETE(vgsScene);

	//清空场景资源列表
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunDeleteScene()
// 参  数: 
// 用  途:Reset VGS场景
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunResetScene()
{
	//vgsScene->Reset();
	if (m_vgsScene)
	{
		m_VGSCore.ResetScene(m_vgsScene);
	}
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetBackground()
// 参  数: 
// 用  途: 设置场景的背景色
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetBackground(VGSRGB* color)
{
	//if (vgsScene)
	//	vgsScene->SetBackground(color->red, color->green, color->blue);
	if (m_vgsScene)
	{
		
	}
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelCount()
// 参  数: 
// 用  途: 得到场景中模型的数量
//-----------------------------------------------------------------------------
UINT CVGS2DLL::SunGetModelCount()
{
	return m_VGSCore.GetModelCount(m_vgsScene);
	//return vgsScene->model.size();
	// return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelName()
// 参  数: DWORD modelHandle, CHAR* strModelName
// 用  途: 根据模型的句柄得到模型的名称
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetModelNameByHandle(DWORD modelHandle, CHAR* strModelName)
{
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	strcpy(strModelName, pModel->name.c_str());
	m_VGSCore.GetModelNameByHandle(m_vgsScene, modelHandle, strModelName);
}	

//根据模型的名称得到模型的指针
//CModel* GetModelByName(CModel** start, CModel** end, CHAR* name)
//{
// 	INT size = end - start + 1;
// 	//只有一个模型
// 	if (size == 1)
// 	{
// 		if (strcmp((*start)->name.c_str(), name) == 0)
// 			return *start;
// 		else
// 			return NULL;
// 	}
// 	//只有两个模型
// 	if (size == 2)
// 	{
// 		if (strcmp((*start)->name.c_str(), name) == 0)
// 			return *start;
// 		if (strcmp((*end)->name.c_str(), name) == 0)
// 			return *end;
// 		return NULL;
// 	}
// 	//超过两个模型
// 	if (size > 2)
// 	{
// 		//先找到中间的那个模型，比较大小，然后在重新指定start 和 end
// 		int newsize = size/2;
// 		CModel* pModel = *(start + newsize);
// 		int result = strcmp(name, pModel->name.c_str());
// 		
// 		//如果正好是中间的这个模型
// 		if (result == 0)
// 			return pModel;
// 		
// 		//如果在前半段
// 		if (result > 0)
// 			end = start + newsize;
// 		//如果在后半段
// 		if (result < 0)
// 			start = start + newsize;
// 		
// 		//递归
// 		GetModelByName(start, end, name);
// 	}
//	return NULL;
//}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelHandleByName()
// 参  数: CHAR* strModelName, DWORD* modelHandle
// 用  途: 根据模型的名称得到模型的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetModelHandleByName(const CHAR* strModelName)
{
	return m_VGSCore.GetModelHandleByName(m_vgsScene, strModelName);
// 	if (vgsScene == NULL || strModelName == NULL) return 0;
// 	
// 	CHAR capName[32];
// 	strcpy(capName, strModelName);
// 	StrToCaption(capName);
// 	
// 	//找到模型
// 	CModel* pModel = GetModelByName(vgsScene->model.begin(), vgsScene->model.end()-1,strModelName);
// 	return (DWORD)pModel;
	// return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelHandleByIndex()
// 参  数: UINT index
// 用  途: 根据模型的编号得到模型的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetModelHandleByIndex(UINT index)
{
	return m_VGSCore.GetModelHandleByIndex(m_vgsScene, index);
// 	if (vgsScene == NULL || index < 0) return 0;
// 	
// 	CModel** ppModel = vgsScene->model.begin();
// 	CModel* pModel = *(ppModel + index);
// 	return (DWORD)pModel;
	// return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetModelVisibility()
// 参  数: DWORD modelHandle, BOOL visible
// 用  途: 设置模型的可见性
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetModelVisibility(DWORD modelHandle, BOOL visible)
{
	m_VGSCore.SetModelVisibility(m_vgsScene, modelHandle, visible);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	if((INT)visible ==0)
// 		pModel->visible =FALSE;
// 	else
// 		pModel->visible =TRUE;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetModelVisibility()
// 参  数: DWORD modelHandle, BOOL visible
// 用  途: 得到模型的可见性
//-----------------------------------------------------------------------------
BOOL CVGS2DLL::SunGetModelVisibility(DWORD modelHandle)
{
// 	if (modelHandle <= 0) return FALSE;
// 	CModel* pModel = (CModel*)modelHandle;
	// 	return (pModel->visible);
	return m_VGSCore.GetModelVisibility(m_vgsScene, modelHandle);
}

//判断贴图是否被场景所用到
BOOL CVGS2DLL::IsTextureInScene(DWORD texHandle)
{
// 	CTexture* pTex = (CTexture*)texHandle;
// 
// 	if (pTex == NULL)
// 		return FALSE;
// 
// 	INT mtrlCnt = vgsScene->shader.size();
// 	CMaterial** ppMtrl = vgsScene->shader.begin();
// 	for (INT i = 0; i< mtrlCnt; i++)
// 	{
// 		CMaterial* pMtrl = *(ppMtrl + i);
// 		UINT texLayerCnt = pMtrl->texLayersList.size();
//         TextureLayer** ppTexLayer = pMtrl->texLayersList.begin();
// 		for(UINT n = 0; n<texLayerCnt; n++)
// 		{
// 			TextureLayer* pTexLayer = *(ppTexLayer+ n);
// 			if(pTexLayer->type !=TLT_CUBEMAP ||pTexLayer->type !=TLT_DYNAMIC ||pTexLayer->type !=TLT_MOVIE ||pTexLayer->type !=TLT_FLASH)
// 			{
// 				if(pTexLayer->pTexture ==texHandle)
// 					return TRUE;
// 			}
// 		}
// 	}
	return FALSE;
}

//判断CubeMap对象是否被场景用到
BOOL CVGS2DLL::IsCubeMapInScene(DWORD pCubeMap)
{
// 	if (pCubeMap == NULL)
// 		return FALSE;
// 
// 	INT mtrlCnt = vgsScene->shader.size();
// 	CMaterial** ppMtrl = vgsScene->shader.begin();
// 	for (INT i = 0; i< mtrlCnt; i++)
// 	{
// 		CMaterial* pMtrl = *(ppMtrl + i);
// 		UINT texLayerCnt = pMtrl->texLayersList.size();
//         TextureLayer** ppTexLayer = pMtrl->texLayersList.begin();
// 		for(UINT n = 0; n<texLayerCnt; n++)
// 		{
// 			TextureLayer* pTexLayer = *(ppTexLayer+ n);
// 			if(pTexLayer->type ==TLT_CUBEMAP)
// 			{
// 				if(pTexLayer->pTexture ==pCubeMap)
// 					return TRUE;
// 			}
// 		}
// 	}
	return FALSE;
}

//判断动态贴图对象是否被场景用到
BOOL CVGS2DLL::IsDyTexInScene(DWORD pDyTex)
{
// 	if (pDyTex == NULL)
// 		return FALSE;
// 
// 	INT mtrlCnt = vgsScene->shader.size();
// 	CMaterial** ppMtrl = vgsScene->shader.begin();
// 	for (INT i = 0; i< mtrlCnt; i++)
// 	{
// 		CMaterial* pMtrl = *(ppMtrl + i);
// 		UINT texLayerCnt = pMtrl->texLayersList.size();
//         TextureLayer** ppTexLayer = pMtrl->texLayersList.begin();
// 		for(UINT n = 0; n<texLayerCnt; n++)
// 		{
// 			TextureLayer* pTexLayer = *(ppTexLayer+ n);
// 			if(pTexLayer->type ==TLT_DYNAMIC)
// 			{
// 				if(pTexLayer->pTexture ==pDyTex)
// 					return TRUE;
// 			}
// 		}
// 	}
	return FALSE;
}

//判断电影贴图是否被场景用到
BOOL CVGS2DLL::IsMovieTexInScene(DWORD pMovieTex)
{
// 	if (pMovieTex == NULL)
// 		return FALSE;
// 
// 	INT mtrlCnt = vgsScene->shader.size();
// 	CMaterial** ppMtrl = vgsScene->shader.begin();
// 	for (INT i = 0; i< mtrlCnt; i++)
// 	{
// 		CMaterial* pMtrl = *(ppMtrl + i);
// 		UINT texLayerCnt = pMtrl->texLayersList.size();
//         TextureLayer** ppTexLayer = pMtrl->texLayersList.begin();
// 		for(UINT n = 0; n<texLayerCnt; n++)
// 		{
// 			TextureLayer* pTexLayer = *(ppTexLayer+ n);
// 			if(pTexLayer->type ==TLT_MOVIE)
// 			{
// 				if(pTexLayer->pTexture ==pMovieTex)
// 					return TRUE;
// 			}
// 		}
// 	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelHandleByIndex()
// 参  数: UINT index
// 用  途: 删除场景中的模型
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDeleteModelByHandle(DWORD modelHandle)
{
	m_VGSCore.DeleteModelbyHandle(m_vgsScene, modelHandle);
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	
// 	if (vgsScene == NULL || modelHandle <= 0 ) return;
// 
// 	CModel* pModel = (CModel*)modelHandle;
// 
// 	//删除模型需要进行以下动作：
// 	//1. 如果是透明模型，将模型从透明模型列表中删除。
// 	//2. 将模型从模型总列表中删除。
// 	//3. 把模型从其材质的模型列表中删除。
// 	//4. 找到模型所用到的材质，如果该材质没有被其它模型所用，则需考虑将该材质从材质队列删除，
// 	//并释放该材质所占用的资源。
// 	//5. 考虑该材质用到的贴图，如果该贴图没有被其它模型所用，则先将该贴图从贴图队列删除，并
// 	//释放该贴图所占用的资源。
// 	//6. 释放该模型所占用的资源。
// 	
// 
// 	//如果是透明模型，把模型从透明模型列表删除
// 	if (/*pModel->shader->bAlphaBlend || */pModel->opacity < 100)
// 	{
// 		INT modelCnt = vgsScene->transModel.size();
// 		CModel** ppModel = vgsScene->transModel.begin();
// 		for (INT i = 0; i< modelCnt; i++)
// 		{
// 			CModel* pTModel = *(ppModel + i);
// 			if (pTModel == pModel)
// 			{
// 				//把模型从透明模型队列中删除
// 				vgsScene->transModel.erase(ppModel + i);
// 				break;
// 			}
// 		}
// 	}
// 	
// 	//把从总模型队列中删除
// 	INT modelCnt = vgsScene->model.size();
// 	CModel** ppModel = vgsScene->model.begin();
// 	for (INT i = 0; i< modelCnt; i++)
// 	{
// 		CModel* pTModel = *(ppModel + i);
// 		if (pTModel == pModel)
// 		{
// 			//从模型队列删除
// 			vgsScene->model.erase(ppModel + i);
// 			break;
// 		}
// 	}	
// 
// 	//把模型从其材质列表删除
// 	INT subMeshCount = pModel->subMesh.size();
// 	for (INT iMesh = 0; iMesh < subMeshCount; ++iMesh)
// 	{
// 		INT shaderMeshCount = pModel->subMesh[iMesh].shader->subMeshList.size();
// 		RenderSubMesh* pShaderMesh = (RenderSubMesh*)pModel->subMesh[iMesh].shader->subMeshList.begin();
// 		for (INT j = 0; j < shaderMeshCount; j++)
// 		{
// 			RenderSubMesh shaderMesh = *(pShaderMesh + j);
// 			if (shaderMesh.modelPointer == (DWORD)pModel)
// 			{
// 				pModel->subMesh[iMesh].shader->subMeshList.erase(pShaderMesh + j);
// 				break;
// 			}
// 		}
// 	}
// 
// 	//释放模型使用的材质和贴图
// // 	if (pModel->shader->modelList.size() == 0)
// // 	{
// // 		//把材质从材质列表中删除
// // 		INT shaderCnt = vgsScene->shader.size();
// // 		CMaterial** ppMtrl = vgsScene->shader.begin();
// // 		for (INT i = 0; i< shaderCnt; i++)
// // 		{
// // 			CMaterial* pMtrl = *(ppMtrl + i);
// // 			if (pMtrl == pModel->shader)
// // 			{
// // 				vgsScene->shader.erase(ppMtrl + i);
// // 				break;
// // 			}
// // 		}
// // 
// // 		if (pModel->shader->diffuseTex && 
// // 			!IsTextureInScene((DWORD)pModel->shader->diffuseTex))
// // 			SunDeleteTexture((DWORD)(pModel->shader->diffuseTex));
// // 
// // 		if (pModel->shader->opacityTex && 
// // 			!IsTextureInScene((DWORD)pModel->shader->opacityTex))
// // 			SunDeleteTexture((DWORD)(pModel->shader->opacityTex));
// // 
// // 		if (pModel->shader->compTex && 
// // 			!IsTextureInScene((DWORD)pModel->shader->compTex))
// // 			SunDeleteTexture((DWORD)(pModel->shader->compTex));
// // 
// // 		if (pModel->shader->lightTex && 
// // 			!IsTextureInScene((DWORD)pModel->shader->lightTex))
// // 			SunDeleteTexture((DWORD)(pModel->shader->lightTex));
// // 
// // 		if (pModel->shader->specularTex && 
// // 			!IsTextureInScene((DWORD)pModel->shader->specularTex))
// // 			SunDeleteTexture((DWORD)(pModel->shader->specularTex));
// // 
// // 		if (pModel->shader->reflectionTex && 
// // 			!IsTextureInScene((DWORD)pModel->shader->reflectionTex))
// // 			SunDeleteTexture((DWORD)(pModel->shader->reflectionTex));
// // 
// // 		if (pModel->shader->bumpTex && 
// // 			!IsTextureInScene((DWORD)pModel->shader->bumpTex))
// // 			SunDeleteTexture((DWORD)(pModel->shader->bumpTex));
// // 		
// // 		if (pModel->shader->pBumpWater && 
// // 			!IsBumpWaterInScene((DWORD)pModel->shader->pBumpWater))
// // 			DeleteBumpWater((DWORD)(pModel->shader->pBumpWater));
// // 
// // 		if (pModel->shader->pCubeMap && 
// // 			!IsCubeMapInScene((DWORD)pModel->shader->pCubeMap))
// // 			DeleteStaticCubeMap((DWORD)(pModel->shader->pCubeMap));
// // 
// // 		if (pModel->shader->pSpecularCubeMap && 
// // 			!IsCubeMapInScene((DWORD)pModel->shader->pSpecularCubeMap))
// // 			DeleteStaticCubeMap((DWORD)(pModel->shader->pSpecularCubeMap));
// // 
// // 		if (pModel->shader->dynamicTex && 
// // 			!IsDyTexInScene((DWORD)pModel->shader->dynamicTex))
// // 			DeleteDynamicTexture((DWORD)(pModel->shader->dynamicTex));
// // 
// // 		if (pModel->shader->pMovieTexture && 
// // 			!IsDyTexInScene((DWORD)pModel->shader->pMovieTexture))
// // 			DeleteMovieTexture((DWORD)(pModel->shader->pMovieTexture));
// // 		
// // 	
// // 		//把材质删除
// // 		SAFE_RELEASE(pModel->shader);
// // 		SAFE_DELETE(pModel->shader);
// // 	}
// 
// 	//释放模型的资源
// 	SAFE_RELEASE(pModel);
// 	SAFE_DELETE(pModel);
	
//-------------------------------------------------------------------------------------------
}

//删除动态贴图
VOID CVGS2DLL::DeleteDynamicTexture(DWORD dynamicTexHandle)
{
// 	CDynamicTex* sDyTex = (CDynamicTex*)dynamicTexHandle;
// 	if (sDyTex == NULL) return;
// 
// 	INT dyTexCnt = vgsScene->dynamicTexture.size();
// 	CDynamicTex** ppDyTex = vgsScene->dynamicTexture.begin();
// 	for (INT i = 0; i< dyTexCnt; i++)
// 	{
// 		CDynamicTex* pDyTex = *(ppDyTex + i);
// 		if (sDyTex == pDyTex)
// 		{
// 			SAFE_RELEASE(pDyTex);
// 			SAFE_DELETE(pDyTex);
// 			vgsScene->dynamicTexture.erase(ppDyTex + i);
// 			break;
// 		}
// 	}
}

/** 移动模型的位置
@param in modelHandle   : 相机句柄
@param in pVec          ：移动的相对向量
@param in flag          ：移动的参考坐标系
						   0 - 在世界坐标系平移
						   1 - 在自身坐标系平移
*/
VOID CVGS2DLL::SunMoveModel(DWORD modelHandle, VECTOR3* pVec, UINT flag)
{
	m_VGSCore.MoveModel(m_vgsScene, modelHandle, pVec, flag);
}
//移动指定的模型到某个位置
//acctype 0-匀速运动，1-匀加速运动, 2-匀减速运动
VOID CVGS2DLL::MoveModelTo(DWORD modelHandle, VECTOR3* pVec, INT frameCnt, INT accType)
{

}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelPosition()
// 参  数: DWORD modelHandle, VECTOR3* pVec
// 用  途: 得到模型的位置
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetModelPosition(DWORD modelHandle, VECTOR3* pVec)
{
	m_VGSCore.GetModelPosition(m_vgsScene, modelHandle, pVec);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pVec->x = pModel->pNode->worldPosition.x;
// 	pVec->y = pModel->pNode->worldPosition.y;
// 	pVec->z = pModel->pNode->worldPosition.z;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetModelPosition()
// 参  数: DWORD modelHandle, VECTOR3* pVec
// 用  途: 设置模型的位置
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetModelPosition(DWORD modelHandle, VECTOR3* pVec)
{
	m_VGSCore.SetModelPosition(m_vgsScene, modelHandle, pVec);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pModel->pNode->setPosition(&(D3DXVECTOR3(pVec->x, pVec->y, pVec->z)));
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunRotateModel()
// 参  数: DWORD modelHandle, VECTOR3* pVec, UINT flag
// 用  途: 旋转模型 
// flag == 0 绕穿过自身坐标中心，并与世界坐标系平行的坐标轴旋转
// flag == 1 绕自身坐标轴旋转
// flag == 2 绕父坐标系旋转
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunRotateModel(DWORD modelHandle, VECTOR3* pVec, UINT flag)
{
	m_VGSCore.RotateModel(m_vgsScene, modelHandle, pVec, flag);
// 	if (modelHandle <= 0) return;
// 	if (pVec->x == 0.0f && pVec->y == 0.0f && pVec->z == 0.0f) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	D3DXVECTOR3 rosVec = D3DXVECTOR3(pVec->x*ATOS, pVec->y*ATOS, pVec->z*ATOS);
// 	pModel->pNode->rotate(&rosVec, flag);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunRotateModelByAxis()
// 参  数: DWORD modelHandle, VECTOR3* pPos, VECTOR3* pVec, FLOAT fAngle
// 用  途: 模型绕指定的轴旋转
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunRotateModelByAxis(DWORD modelHandle, VECTOR3* pPos, VECTOR3* pVec, FLOAT fAngle)
{
// 	if (modelHandle <= 0) return;
// 	if (fAngle == 0.0f) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	D3DXVECTOR3 vPos = D3DXVECTOR3(pPos->x, pPos->y, pPos->z);
// 	D3DXVECTOR3 vVec = D3DXVECTOR3(pVec->x, pVec->y, pVec->z);
// 	fAngle *= ATOS;
// 	pModel->pNode->rotate(&vPos, &vVec, fAngle,0);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetModelRotation()
// 参  数: DWORD modelHandle, VECTOR3* pRot
// 用  途: 设置模型的转角
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetModelRotation(DWORD modelHandle, VECTOR3* pRot)
{
	m_VGSCore.SetModelRotation(m_vgsScene, modelHandle, pRot);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pRot->x *= ATOS;
// 	pRot->y *= ATOS;
// 	pRot->z *= ATOS;
// 	pModel->pNode->setRotationByAngle(&D3DXVECTOR3(pRot->x, pRot->y, pRot->z));
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelRotation()
// 参  数: DWORD modelHandle, VECTOR3* pRot
// 用  途: 得到模型的转角
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetModelRotation(DWORD modelHandle, VECTOR3* pRot)
{
	m_VGSCore.GetModelRotation(m_vgsScene, modelHandle, pRot);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pRot->x = pModel->pNode->worldRotation.x*STOA;
// 	pRot->y = pModel->pNode->worldRotation.y*STOA;
// 	pRot->z = pModel->pNode->worldRotation.z*STOA;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunScaleModel()
// 参  数: DWORD modelHandle, VECTOR3* pVec, UINT flag
// 用  途: 缩放模型
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunScaleModel(DWORD modelHandle, VECTOR3* pScale, UINT flag)
{
	m_VGSCore.ScaleModel(m_vgsScene, modelHandle, pScale, (VGS_TRANSFORMSPACE)flag);
// 	if (modelHandle <= 0) return;
// 	if (pScale->x == 1.0f && pScale->y == 1.0f && pScale->z == 1.0f) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	D3DXVECTOR3 scaleVec = D3DXVECTOR3(pScale->x, pScale->y, pScale->z);
// 	pModel->pNode->scale(&scaleVec, flag);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunScaleModelByPoint()
// 参  数: DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale
// 用  途: 以固定点为中心缩放模型
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunScaleModelByPoint(DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale)
{
	m_VGSCore.ScaleModelByPoint(m_vgsScene, modelHandle, pPos, pScale);
	//
// 	if (modelHandle <= 0) return;
// 	if (pScale->x == 1.0f && pScale->y == 1.0f && pScale->z == 1.0f) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	D3DXVECTOR3 vPos = D3DXVECTOR3(pPos->x, pPos->y, pPos->z);
// 	D3DXVECTOR3 vScale = D3DXVECTOR3(pScale->x, pScale->y, pScale->z);
// 	pModel->pNode->scaleByPoint(&vScale, &vPos);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetModelScale()
// 参  数: DWORD modelHandle, VECTOR3* pScale
// 用  途: 设置模型的缩放
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetModelScale(DWORD modelHandle, VECTOR3* pScale)
{
	m_VGSCore.SetModelScale(m_vgsScene, modelHandle, pScale);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pModel->pNode->scale(&D3DXVECTOR3(pScale->x, pScale->y, pScale->z),0);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelScale()
// 参  数: DWORD modelHandle, VECTOR3* pScale
// 用  途: 得到模型的缩放
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetModelScale(DWORD modelHandle, VECTOR3* pScale)
{
	m_VGSCore.GetModelScale(m_vgsScene, modelHandle, pScale);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pScale->x = pModel->pNode->worldScale.x;
// 	pScale->y = pModel->pNode->worldScale.y;
// 	pScale->z = pModel->pNode->worldScale.z;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetModelUnderLoc()
// 参  数: UINT x, UINT y
// 用  途: 得到光标下方的模型句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetModelUnderLoc(UINT x, UINT y)
{
	return m_VGSCore.GetModelUnderLoc(m_vgsScene, m_Viewport, x, y);
// 	CModel* pModel = vgsScene->GetModelUnderCursor(x, y);
	// 	return (DWORD)pModel;
}

//-----------------------------------------------------------------------------
// 方法名: SunGetMouseHintModelUnderLoc()
// 参  数: UINT x, UINT y
// 用  途: 得到光标下方的有鼠标响应的模型句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetMouseHintModelUnderLoc(UINT x, UINT y)
{
// 	CModel* pModel = vgsScene->GetMouseHintModelUnderCursor(x, y);
	// 	return (DWORD)pModel;
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetModelReference()
// 参  数: DWORD curModelHandle, DWORD refModelHandle
// 用  途: 设置模型的引用模型
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetModelReference(DWORD curModelHandle, DWORD refModelHandle)
{
	/*
	if (curModelHandle <= 0 || refModelHandle <= 0) return;
	CModel* curModel = (CModel*)curModelHandle;
	CModel* refModel = (CModel*)refModelHandle;
	//如果refModelHandle自己也索引其他物体，则不能索引
	if (refModel->refModel != NULL)
	{
		CHAR modelName[32];
		CVGS2DLL::SunGetModelNameByHandle(refModelHandle, modelName);
		CHAR errInfo[256];
		strcpy(errInfo, "Model ");
		strcat(errInfo, modelName);
		strcat(errInfo, " is parent model, can not indexed by other model。");
		MessageBox(GetActiveWindow(), errInfo, "set index3d model...error", MB_OK|MB_ICONERROR);
	}
	curModel->refModel = refModel;
	*/
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetCameraCount()
// 参  数: DWORD modelHandle
// 用  途: 重新计算模型的法线，让模型看起来非常光滑Smooth,计算量较大
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSmoothModelNormal(DWORD modelHandle)
{
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
	// 	vgsScene->SmoothModelNormal(pModel);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetBillBoardStatus()
// 参  数: DWORD modelHandle, BOOL status
// 用  途: 设置模型正对Billboard状态
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetBillBoardStatus(DWORD modelHandle, BOOL status)
{
	m_VGSCore.SetModelBillboardStatus(m_vgsScene, modelHandle, status);
// 	if (modelHandle <= 0) return;
// 	
// 	CModel* pModel = (CModel*)modelHandle;
	// 	pModel->isBillboard = status;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetBillboard()
// 参  数: DWORD modelHandle, VECTOR3* pModelRay, VECTOR3* pAt, VECTOR3* pUp
//modelHandle ->模型的指针
//pModelRay->模型要正对目标点的自身坐标方向量, 如Z方向正对目标点，则pModelRay = &VECTOR3(0,0,1)
//pAt ->模型正对的目标点
//pUp ->模型的上方是什么方向，譬如Tree，要求Z方向正对目标点，但up方向始终为(0,1,0), 否则就倒了
// 用  途: 设置模型正对Billboard
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetBillboardUp(DWORD modelHandle, VECTOR3* pUp)
{
	m_VGSCore.SetBillboardUp(m_vgsScene, modelHandle, pUp);
// 	if (modelHandle <= 0) return;
// 	
// 	CModel* pModel = (CModel*)modelHandle;
// 	pModel->pointAtOrient.PointAtUp.x = pUp->x;
// 	pModel->pointAtOrient.PointAtUp.y = pUp->y;
// 	pModel->pointAtOrient.PointAtUp.z = pUp->z;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunEnableCollision()
// 参  数: 
// 用  途: 设置场景碰撞的有效状态
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunEnableCollision(BOOL status)
{
	//SetModelCollisionStatus(m_vgsScene)
	//bCollision = status;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetColDistance()
// 参  数: FLOAT distance
// 用  途: 设置碰撞发生的有效距离, 默认为1.0
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetColDistance(FLOAT distance)
{
// 	if (vgsScene == NULL || distance <= 0) return;
// 	
// 	UINT modelCnt = vgsScene->model.size();
// 	CModel** ppModel = vgsScene->model.begin();
// 	
// 	colDistance = distance;
// 	
// 	for (UINT i = 0; i< modelCnt; i++)
// 	{
// 		CModel* pModel = *(ppModel + i);
// 		pModel->colDetectDistance = distance;
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCollisionModel()
// 参  数: 
// 用  途: 设置模型是否可以碰撞
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCollisionModel(DWORD modelHandle, BOOL status)
{
	m_VGSCore.SetModelCollisionStatus(m_vgsScene, modelHandle, status);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pModel->isCollision = status;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunEnableWeight()
// 参  数: BOOL status
// 用  途: 设置重力有效状态
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunEnableWeight(BOOL status)
{
	// bWeight = status;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetGroundModel()
// 参  数: DWORD modelHandle, BOOL status
// 用  途: 设置模型为地面模型
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetGroundModel(DWORD modelHandle, BOOL status)
{
	m_VGSCore.SetModelAsGround(m_vgsScene, modelHandle, status);
// 	CModel* pModel = (CModel*)modelHandle;
// 	pModel->isGround = status;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunMoveModelUV()
// 参  数: DWORD modelHandle, FLOAT x, FLOAT y
// 用  途: 移动模型Diffuse贴图的UV
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunMoveModelUV(DWORD modelHandle, FLOAT x, FLOAT y)
{
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
//	pModel->MoveTextureUV(x, y);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetDynamicUVStatus()
// 参  数: DWORD modelHandle, BOOL status
// 用  途: 设置模型是否使用自动转动UV
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetDynamicUVStatus(DWORD modelHandle, BOOL status)
{
// 	if (modelHandle <= 0) return;
// 
// 	CModel* pModel = (CModel*)modelHandle;
// 	if (pModel)
// 		pModel->bDynamicUV = status;
// 
// 	//恢复模型的UV
// 	if (pModel->bDynamicUV == FALSE)
// 	{
// 		//从顶点缓冲取数据备份，用于释放后重新创建顶点缓冲
// 		
// 		//判断顶点的FVF,不同的FVF填充不同的数据
// 		pModel->vertex = new CVertex[pModel->vertexCnt];
// 		UINT j;
// 		switch(pModel->m_FVF)
// 		{
// 		case FVF_T1:
// 			//逐个顶点填充数据
// 			for (j=0; j< pModel->vertexCnt; j++)
// 			{
// 				//填充坐标数据到顶点缓冲
// 				pModel->vertex[j].position = ((T1MESHVERTEX*)pModel->m_pVertices)[j].position;
// 				pModel->vertex[j].diffuse = ((T1MESHVERTEX*)pModel->m_pVertices)[j].diffuse;
// 			}
// 			break;
// 			
// 		case FVF_T2:
// 			//逐个顶点填充数据
// 			for (j=0; j< pModel->vertexCnt; j++)
// 			{
// 				//填充坐标数据到顶点缓冲
// 				pModel->vertex[j].position = ((T2MESHVERTEX*)pModel->m_pVertices)[j].position;	
// 				pModel->vertex[j].normal = ((T2MESHVERTEX*)pModel->m_pVertices)[j].normal;
// 				pModel->vertex[j].diffuse = ((T2MESHVERTEX*)pModel->m_pVertices)[j].diffuse;
// 			}
// 			break;
// 			
// 		case FVF_T3:
// 			//逐个顶点填充数据
// 			for (j=0; j< pModel->vertexCnt; j++)
// 			{
// 				//填充坐标数据到顶点缓冲
// 				pModel->vertex[j].position = ((T3MESHVERTEX*)pModel->m_pVertices)[j].position;
// 				pModel->vertex[j].diffuse = ((T3MESHVERTEX*)pModel->m_pVertices)[j].diffuse;
// 				pModel->vertex[j].uv1.u = ((T3MESHVERTEX*)pModel->m_pVertices)[j].su;
// 				pModel->vertex[j].uv1.v = ((T3MESHVERTEX*)pModel->m_pVertices)[j].sv;
// 			}
// 			break;
// 			
// 		case FVF_T4:
// 			//逐个顶点填充数据
// 			for (j=0; j< pModel->vertexCnt; j++)
// 			{
// 				//填充坐标数据到顶点缓冲
// 				pModel->vertex[j].position = ((T4MESHVERTEX*)pModel->m_pVertices)[j].position;	
// 				pModel->vertex[j].normal = ((T4MESHVERTEX*)pModel->m_pVertices)[j].normal;
// 				pModel->vertex[j].diffuse = ((T4MESHVERTEX*)pModel->m_pVertices)[j].diffuse;
// 				pModel->vertex[j].uv1.u = ((T4MESHVERTEX*)pModel->m_pVertices)[j].su;
// 				pModel->vertex[j].uv1.v = ((T4MESHVERTEX*)pModel->m_pVertices)[j].sv;
// 			}
// 			break;
// 			
// 		case FVF_T5:
// 			//逐个顶点填充数据
// 			for (j=0; j< pModel->vertexCnt; j++)
// 			{
// 				//填充坐标数据到顶点缓冲
// 				pModel->vertex[j].position = ((T5MESHVERTEX*)pModel->m_pVertices)[j].position;	
// 				pModel->vertex[j].diffuse = ((T5MESHVERTEX*)pModel->m_pVertices)[j].diffuse;
// 				pModel->vertex[j].uv1.u = ((T5MESHVERTEX*)pModel->m_pVertices)[j].su;
// 				pModel->vertex[j].uv1.v = ((T5MESHVERTEX*)pModel->m_pVertices)[j].sv;
// 				pModel->vertex[j].uv2.u = ((T5MESHVERTEX*)pModel->m_pVertices)[j].sw;
// 				pModel->vertex[j].uv2.v = ((T5MESHVERTEX*)pModel->m_pVertices)[j].sz;
// 			}
// 			break;
// 			
// 		case FVF_T6:
// 			//逐个顶点填充数据
// 			for (j=0; j< pModel->vertexCnt; j++)
// 			{
// 				//填充坐标数据到顶点缓冲
// 				pModel->vertex[j].position = ((T6MESHVERTEX*)pModel->m_pVertices)[j].position;	
// 				pModel->vertex[j].normal = ((T6MESHVERTEX*)pModel->m_pVertices)[j].normal;
// 				pModel->vertex[j].diffuse = ((T6MESHVERTEX*)pModel->m_pVertices)[j].diffuse;
// 				pModel->vertex[j].uv1.u = ((T6MESHVERTEX*)pModel->m_pVertices)[j].su;
// 				pModel->vertex[j].uv1.v = ((T6MESHVERTEX*)pModel->m_pVertices)[j].sv;
// 				pModel->vertex[j].uv2.u = ((T6MESHVERTEX*)pModel->m_pVertices)[j].sw;
// 				pModel->vertex[j].uv2.v = ((T6MESHVERTEX*)pModel->m_pVertices)[j].sz;
// 			}
// 			break;
// 		case FVF_T7:
// 			//逐个顶点填充数据
// 			for (j=0; j< pModel->vertexCnt; j++)
// 			{
// 				//填充坐标数据到顶点缓冲
// 				pModel->vertex[j].position = ((T7MESHVERTEX*)pModel->m_pVertices)[j].position;	
// 				pModel->vertex[j].normal = ((T7MESHVERTEX*)pModel->m_pVertices)[j].normal;
// 				pModel->vertex[j].diffuse = ((T7MESHVERTEX*)pModel->m_pVertices)[j].diffuse;
// 				pModel->vertex[j].uv1.u = ((T7MESHVERTEX*)pModel->m_pVertices)[j].su1;
// 				pModel->vertex[j].uv1.v = ((T7MESHVERTEX*)pModel->m_pVertices)[j].sv1;
// 				pModel->vertex[j].uv2.u = ((T7MESHVERTEX*)pModel->m_pVertices)[j].su2;
// 				pModel->vertex[j].uv2.v = ((T7MESHVERTEX*)pModel->m_pVertices)[j].sv2;
// 				pModel->vertex[j].uv3.u = ((T7MESHVERTEX*)pModel->m_pVertices)[j].su3;
// 				pModel->vertex[j].uv3.v = ((T7MESHVERTEX*)pModel->m_pVertices)[j].sv3;
// 				pModel->vertex[j].uv4.u = ((T7MESHVERTEX*)pModel->m_pVertices)[j].su4;
// 				pModel->vertex[j].uv4.v = ((T7MESHVERTEX*)pModel->m_pVertices)[j].sv4;
// 			}
// 			break;
// 		}
// 		SAFE_RELEASE_D3D(pModel->m_VBuffer);
// 		
// 		pModel->createVBuffer();
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetDynamicUVProp()
// 参  数: DWORD modelHandle, FLOAT UStep, FLOAT VStep, FLOAT speed
// 用  途: 设置模型的自动转动UV的参数
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetDynamicUVProp(DWORD modelHandle, FLOAT UStep, FLOAT VStep, FLOAT speed)
{
// 	if (modelHandle <= 0) return;
/*	CModel* pModel = (CModel*)modelHandle;
	pModel->UStep = UStep*speed;
	pModel->VStep = VStep*speed;*/
}

//-----------------------------------------------------------------------------
// 方法名: SunSetModelClickable
// 参  数: DWORD modelHandle, BOOL status
// 用  途: 设置模型是否可以点击
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetModelClickable(DWORD modelHandle, BOOL status)
{
	m_VGSCore.SetModelClickable(m_vgsScene, modelHandle, status);
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pModel->clickable = status;
}

//设置模型的opacity
VOID CVGS2DLL::SunSetModelOpacity(DWORD modelHandle, UINT opacity)
{
	m_VGSCore.SetModelOpacity(m_vgsScene, modelHandle, opacity);
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*
	if (modelHandle <= 0) return;
	if (opacity > 100) opacity = 100;
	if (opacity < 0) opacity = 0;
		CModel* pModel = (CModel*)modelHandle;

	if (pModel)
	{
		//如果模型当前为不透明
		if (pModel->opacity == 100 && pModel->shader->opacity == 100)
		{
			//如果设置为透明,插入透明队列
			if (opacity < 100)
			{
				//找到材质队列中的模型
				CMaterial** ppMtrl = vgsScene->shader.begin();
				INT shaderCnt = vgsScene->shader.size();
				for (INT i = 0; i< shaderCnt; i++)
				{
					CMaterial* pMtrl = *(ppMtrl + i);
					CModel** ppModel = (CModel**)(pMtrl->modelList.begin());
					INT modelCnt = pMtrl->modelList.size();
					for (INT j = 0; j< modelCnt; j++)
					{	
						CModel* _pModel = *(ppModel + j);
						if (_pModel == pModel)
						{
							//插入透明队列
							vgsScene->transModel.push_back(pModel);
							pModel->SetOpacity(opacity);
							return;
						}
					}
				}
			}
			pModel->SetOpacity(opacity);
		}
		//如果当前模型为透明
		else
		{
			//改为不透明
			if (opacity == 100)
			{
				CModel** ppModel = vgsScene->transModel.begin();
				INT modelSize = vgsScene->transModel.size();
				for (INT i = 0; i< modelSize; i++)
				{
					CModel* _pModel = *(ppModel + i);
					if (_pModel == pModel)
					{
						//从透明模型队列删除
						vgsScene->transModel.erase(ppModel + i);
						break;
					}
				}
			}
			pModel->SetOpacity(opacity);
		}
	}*/
	//--------------------------------------------------------------------------------------------
	return;
}

//设置模型的顶点颜色
VOID CVGS2DLL::SunSetModelColor(DWORD modelHandle, BYTE red, BYTE green, BYTE blue, BYTE alpha)
{
	m_VGSCore.SetModelColor(m_vgsScene, modelHandle, red, green, blue, alpha);
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	
// 	if (alpha > 255) alpha =  255;
// 	if (alpha < 0) alpha = 0;
// 	
// 	CModel* pModel = (CModel*)modelHandle;
// 	if (pModel)
// 	{
// 		pModel->SetColor(red, green, blue, alpha);
// 	}
// 	
	//--------------------------------------------------------------------------------------------------------------
}


//-------------------------------------------------------------------------------------------
//***材质相关的方法
//-------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetMaterialCount()
// 参  数: 
// 用  途: 得到场景中材质的数量
//-----------------------------------------------------------------------------
UINT CVGS2DLL::SunGetMaterialCount()
{
	return m_VGSCore.GetMaterialCount(m_vgsScene);
	// return vgsScene->shader.size();
	// return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetMaterialNameByHandle()
// 参  数: DWORD mtrlHandle, CHAR* strMtrlName
// 用  途: 根据材质的句柄得到材质的名称
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetMaterialNameByHandle(DWORD mtrlHandle, CHAR* strMtrlName)
{
	m_VGSCore.GetMaterialNameByHandle(m_vgsScene, mtrlHandle, strMtrlName);
// 	if (mtrlHandle <= 0) return;
// 	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
// 	//strcpy(strMtrlName, pMtrl->name);
// 	pMtrl->name =std::string(strMtrlName);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetMaterialHandleByName()
// 参  数: CHAR* strMtrlName
// 用  途: 根据材质的名称得到材质的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetMaterialHandleByName(const CHAR* strMtrlName)
{
	return m_VGSCore.GetModelHandleByName(m_vgsScene, strMtrlName);
// 	UINT mtrlCnt = vgsScene->shader.size();
// 	CMaterial** ppMtrl = vgsScene->shader.begin();
// 	
// 	for (UINT i= 0; i< mtrlCnt; i++)
// 	{
// 		CMaterial* pMtrl = *(ppMtrl + i);
// 		if (pMtrl->name == (std::string(strMtrlName)))
// 			return (DWORD)pMtrl;
// 	}

}


//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetMaterialHandleByIndex()
// 参  数: UINT index
// 用  途: 根据材质的编号得到材质的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetMaterialHandleByIndex(UINT index)
{
	return 0;
// 	if (vgsScene == NULL) return 0;
// 	
// 	CMaterial** ppMtrl = vgsScene->shader.begin();
// 	CMaterial* pMtrl = *(ppMtrl + index);
// 	return (DWORD)pMtrl;
	// return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SunCreateEmptyMtrl()
// 参  数: CHAR* strMtrlName
// 用  途: 创建材质
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunCreateEmptyMtrl(CHAR* strMtrlName)
{
	return m_VGSCore.CreateMaterial(m_vgsScene, strMtrlName);
// 	if (vgsScene == NULL) return 0;
// 	
// 	CMaterial* pMtrl = new CMaterial(vgsScene->m_pd3dDevice);
// 	pMtrl->name =std::string(strMtrlName);
// 	if (pMtrl == NULL) return 0;
// 	
// 	vgsScene->shader.push_back(pMtrl);
// 	
	// 	return (DWORD)pMtrl;
	// return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SunDeleteMtrl()
// 参  数: DWORD mtrlHandle
// 用  途: 删除材质
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDeleteMtrl(DWORD mtrlHandle)
{
	m_VGSCore.DeleteMaterialByHandle(m_vgsScene, mtrlHandle);
// 	if (vgsScene == NULL) return;
// 	
// 	INT shaderCnt = vgsScene->shader.size();
// 	
// 	CMaterial** ppMtrl = vgsScene->shader.begin();
// 	
// 	for (INT i = 0; i< shaderCnt; i++)
// 	{
// 		CMaterial* pMtrl = *(ppMtrl + i);
// 		if ((DWORD)pMtrl == mtrlHandle)
// 		{
// 			SAFE_RELEASE(pMtrl);
// 			SAFE_DELETE(pMtrl);
// 			vgsScene->shader.erase(ppMtrl + i);
// 			break;
// 		}
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: SunGetMtrlByModel()
// 参  数: DWORD modelHandle
// 用  途: 通过模型得到材质
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetMtrlByModel(DWORD modelHandle)
{
//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*
	if (vgsScene == NULL) return 0;
	if (modelHandle == NULL) return 0;
	
	CModel* pModel = (CModel*)modelHandle;
	return (DWORD)pModel->shader;
	*/
	return 0;
	//-------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// 方法名: SunSetModelMtrl()
// 参  数: DWORD modelHandle, DWORD mtrlHandle
// 用  途: 设置某个模型的材质
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetModelMtrl(DWORD modelHandle, INT subMeshID, DWORD mtrlHandle)
{
// 	if (vgsScene == NULL || modelHandle == NULL || mtrlHandle == NULL) return;
// 	
// 	CModel* pModel = (CModel*)modelHandle;	
// 	CMaterial* pOldMtrl = pModel->subMesh[subMeshID].shader;
// 	CMaterial* pNewMtrl = (CMaterial*)mtrlHandle;
// 	if (pNewMtrl == NULL || pNewMtrl == pOldMtrl) 
// 		return;
// 
// 	if(subMeshID ==-1)
// 	{
// 		//将材质设置给模型所有的子模型
// 		for (int iMesh = 0; iMesh < pModel->subMesh.size(); iMesh++)
// 			SunSetModelMtrl(modelHandle, iMesh, mtrlHandle);
// 		return;
// 	}
// 
// 	//将材质设置给指定的子模型
// 	pModel->SetMtrl(pNewMtrl, subMeshID);
	
	//重新构造材质的SubMeshList列表??????
	//vgsScene->ConstructShaderSubMeshList();
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetSkyBox()
// 参  数: DWORD modelHandle, BOOL status
// 用  途: 设置模型为SkyBox模型
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetSkyBox(DWORD modelHandle, BOOL status)
{
	
// 	if (modelHandle <= 0) return;
// 	CModel* pModel = (CModel*)modelHandle;
// 	pModel->isSkyBox = status;
}

//获得材质信息
VOID CVGS2DLL::SunGetMtrlInfo(DWORD mtrlHandle, MtrlInfo* pMtrlInfo)
{
	//GetMaterialNameByHandle()
// 	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
// 	if (pMtrl)
// 	{
// 		pMtrlInfo->Name =pMtrl->name;
// 		
// 		//Ambient
// 		pMtrlInfo->Ambient.red = pMtrl->d3dmtrl.Ambient.r * 255.0f;
// 		pMtrlInfo->Ambient.green = pMtrl->d3dmtrl.Ambient.g * 255.0f;
// 		pMtrlInfo->Ambient.blue = pMtrl->d3dmtrl.Ambient.b * 255.0f;
// 		
// 		//Diffuse
// 		pMtrlInfo->Diffuse.red = pMtrl->d3dmtrl.Diffuse.r * 255.0f;
// 		pMtrlInfo->Diffuse.green = pMtrl->d3dmtrl.Diffuse.g * 255.0f;
// 		pMtrlInfo->Diffuse.blue = pMtrl->d3dmtrl.Diffuse.b * 255.0f;
// 
// 		//Specular
// 		pMtrlInfo->Specular.red = pMtrl->specular.r * 255.0f;
// 		pMtrlInfo->Specular.green = pMtrl->specular.g * 255.0f;
// 		pMtrlInfo->Specular.blue = pMtrl->specular.b * 255.0f;
// 
// 		//SpecularLevel
// 		pMtrlInfo->SpecularLevel = pMtrl->specularLevel;
// 
// 		//SpecularPower
// 		pMtrlInfo->SpecularPower = pMtrl->d3dmtrl.Power;
// 		
// 		//Emissive
// 		pMtrlInfo->Emissive.red = pMtrl->d3dmtrl.Emissive.r * 255.0f;
// 		pMtrlInfo->Emissive.green = pMtrl->d3dmtrl.Emissive.g * 255.0f;
// 		pMtrlInfo->Emissive.blue = pMtrl->d3dmtrl.Emissive.b * 255.0f;
// 
// 		//透明度
// 		pMtrlInfo->Opacity = pMtrl->opacity;
// 
// 		//是否双面
// 		pMtrlInfo->bTwoSide = pMtrl->twoSide;
// 
// 		//是否进行Alpha测试
// 		pMtrlInfo->bAlphaTest = pMtrl->bAlphaTest;
// 	}
}

//设置材质信息
VOID CVGS2DLL::SunSetMtrlInfo(DWORD mtrlHandle, MtrlInfo* pMtrlInfo)
{
// 	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
// 	if (pMtrl)
// 	{
// 		pMtrl->name = pMtrlInfo->Name;
// 		
// 		//Ambient
// 		pMtrl->d3dmtrl.Ambient.r = pMtrlInfo->Ambient.red /255.0f;
// 		pMtrl->d3dmtrl.Ambient.g = pMtrlInfo->Ambient.green /255.0f;
// 		pMtrl->d3dmtrl.Ambient.b = pMtrlInfo->Ambient.blue /255.0f;
// 		
// 		//Diffuse
// 		pMtrl->d3dmtrl.Diffuse.r = pMtrlInfo->Diffuse.red /255.0f;
// 		pMtrl->d3dmtrl.Diffuse.g = pMtrlInfo->Diffuse.green /255.0f;
// 		pMtrl->d3dmtrl.Diffuse.b = pMtrlInfo->Diffuse.blue /255.0f;
// 
// 		//Specular
// 		pMtrl->specular.r = pMtrlInfo->Specular.red /255.0f;
// 		pMtrl->specular.g = pMtrlInfo->Specular.green /255.0f;
// 		pMtrl->specular.b = pMtrlInfo->Specular.blue /255.0f;
// 		//SpecularLevel
// 		pMtrl->specularLevel = pMtrlInfo->SpecularLevel;
// 		pMtrl->d3dmtrl.Specular = pMtrl->specularLevel * pMtrl->specular;
// 
// 		//SpecularPower
// 		pMtrl->d3dmtrl.Power = pMtrlInfo->SpecularPower;
// 
// 		//Emissive
// 		pMtrl->d3dmtrl.Emissive.r = pMtrlInfo->Emissive.red /255.0f;
// 		pMtrl->d3dmtrl.Emissive.g = pMtrlInfo->Emissive.green /255.0f;
// 		pMtrl->d3dmtrl.Emissive.b = pMtrlInfo->Emissive.blue /255.0f;
// 
// 		//透明度
// 		pMtrl->opacity = pMtrlInfo->Opacity;
// 
// 		//是否双面
// 		pMtrl->twoSide = pMtrlInfo->bTwoSide;
// 
// 		//是否进行Alpha测试
// 		pMtrl->bAlphaTest = pMtrlInfo->bAlphaTest;
// 
// 		//设置用到该材质的模型的顶点的颜色
// 		SunSetMtrlColor(mtrlHandle, pMtrlInfo->Diffuse.red,
// 			pMtrlInfo->Diffuse.green,
// 			pMtrlInfo->Diffuse.blue, 255);
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: CreateBumpTexture()
// 参  数: INT width, INT height
// 用  途: 创建Bump贴图
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::CreateBumpTexture(CHAR* name, VGS_BumpType type, INT width, INT height, INT xSize, INT ySize, 
									   FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY)
{
// 	if (vgsScene->d3dParam.SupportBestBumpFormat == D3DFMT_UNKNOWN)
// 	{
// 		::MessageBox(GetActiveWindow(), "您的显卡不支持凹凸贴图，无法创建凹凸水面", "系统友好提示", MB_ICONEXCLAMATION | MB_OK);
// 		return 0;
// 	}
// 
// 	if (width == 0 ) width = 256;
// 	if (height == 0) height = 256;
// 
// 	//创建凹凸贴图
// 	CBumpMap* pBumpMap = new CBumpMap(vgsScene->m_pd3dDevice, &vgsScene->m_d3dpp, BUMPWAVEMAP, vgsScene->d3dParam.SupportBestBumpFormat);
// 	pBumpMap->m_name = std::string(name);
// 	pBumpMap->m_type = type;
// 	vgsScene->bumpTexture.push_back(pBumpMap);
// 
// 	switch (pBumpMap->m_type)
// 	{
// 	case BUMPWAVEMAP:
// 		pBumpMap->InitBumpWaveMap(width, height, xSize, ySize, radius, speed, centerX, centerY);
// 		break;
// 	case BUMPENVMAP:
// 		pBumpMap->InitBumpEnvMap();
// 		break;
// 	case BUMPNORMALMAP:
// 		pBumpMap->InitBumpNormalMap();
// 		break;
// 	}
// 
	// 	return (DWORD)pBumpMap;
	return 0;
}

//删除BumpWater
VOID CVGS2DLL::DeleteBumpTexture(DWORD bumpTexHandle)
{
// 	CBumpMap* sBumpTex = (CBumpMap*)bumpTexHandle;
// 	if (sBumpTex)
// 	{
// 		INT bumpWaterCnt = vgsScene->bumpTexture.size();
// 		CBumpMap** ppBumpMap = vgsScene->bumpTexture.begin();
// 		for (INT i = 0; i< bumpWaterCnt; i++)
// 		{
// 			CBumpMap* pBumpMap = *(ppBumpMap + i);
// 			if (sBumpTex == pBumpMap)
// 			{
// 				SAFE_RELEASE(pBumpMap);
// 				SAFE_DELETE(pBumpMap);
// 				vgsScene->bumpTexture.erase(ppBumpMap + i);
// 				return;
// 			}
// 		}
// 	}
}

//通过BumpMap的名字获得指针
DWORD CVGS2DLL::GetBumpTextureHandleByName(CHAR* strName)
{
// 	INT bumpMapCnt = vgsScene->bumpTexture.size();
// 	CBumpMap** ppBumpMap = vgsScene->bumpTexture.begin();
// 	for (INT i = 0; i< bumpMapCnt; i++)
// 	{
// 		CBumpMap* pBumpMap = *(ppBumpMap + i);
// 		if (strcmp(pBumpMap->m_name.c_str(), strName) == 0)
// 			return (DWORD)(pBumpMap);
// 	}
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SetMtrlTexture()
// 参  数: CHAR* mtrlName, 材质名称
//INT layerIndex, 贴图层的编号，-1 - 8， -1表示放在最后一层
//TEXTURELAYERTYPE type, 贴图的类型
//textureName, 贴图的名称
// 用  途: 设置材质的某层贴图
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SetMtrlTexture(CHAR* mtrlName, INT layerIndex, VGS_TextureType type, CHAR* textureName)
{
// 	CMaterial* pMtrl = (CMaterial*)SunGetMaterialHandleByName(mtrlName);
// 	if (pMtrl == NULL) return;
// 
// 	switch (type)
// 	{
// 	case TLT_INVALID:
// 		break;
// 
// 		// 基础贴图
// 	case TLT_BASAL:
// 		// 透明贴图					
// 	case TLT_OPACITY:
// 		// 反射贴图	            
// 	case TLT_REFLECT:
// 		{
// 			CTexture* pTex = (CTexture*)SunGetTextureHandleByName(textureName);
// 			if (pTex == NULL) break;
// 
// 			//贴图层
// 			TextureLayer* pTexLayer = new TextureLayer();
// 			pTexLayer->type = type;
// 			pTexLayer->pTexture = (DWORD)pTex;
// 			pMtrl->texLayersList.push_back(pTexLayer);
// 		}
// 		break;    
// 		
// 		// 实时阴影
// 	case TLT_REALTIMESHADOW:
// 		break;    
// 		
// 		// 立方图
// 	case TLT_CUBEMAP:
// 		break;  
// 		
// 		 // 动态贴图
// 	case TLT_DYNAMIC:
// 		break;    
// 
// 		// 实时反射
// 	case TLT_RTFMAP:
// 		{
// 			CRTFMap* pRTFMap = (CRTFMap*)GetRTFTextureHandleByName(textureName);
// 			if (pRTFMap == NULL) break;
// 
// 			//贴图层
// 			TextureLayer* pTexLayer = new TextureLayer();
// 			pTexLayer->type = type;
// 			pTexLayer->pTexture = (DWORD)pRTFMap;
//              
// 			//添加手动混合模式
// 			if(pRTFMap->m_power> 0.0f)
// 			{
// 			   pTexLayer->Blenderinfo.ColorBlendOption = D3DTOP_BLENDFACTORALPHA;
// 			   pTexLayer->Blenderinfo.ColorBlendParam = D3DCOLOR_COLORVALUE(0,0,0, pRTFMap->m_power);
// 			}
// 			pMtrl->texLayersList.push_back(pTexLayer);
// 		}
// 		break;
// 		
// 		// 电影贴图
// 	case TLT_MOVIE:
// 		break;      
// 		
// 		// Flash贴图
// 	case TLT_FLASH:
// 		break;	
// 		
// 		// 凹凸贴图，包括凹凸波纹，凹凸环境反射，法线贴图等
// 	case TLT_BUMPMAP:
// 		{
// 			CBumpMap* pBumpMap = (CBumpMap*)GetBumpTextureHandleByName(textureName);
// 			if (pBumpMap == NULL) break;
// 
// 			TextureLayer* pTexLayer = new TextureLayer();
// 			pTexLayer->Name = pBumpMap->m_name;
// 			pTexLayer->type = TLT_BUMPMAP;
// 			pTexLayer->pTexture = (DWORD)pBumpMap;
// 			pMtrl->texLayersList.push_back(pTexLayer);
// 
// 			//所有用到该材质的模型，都需要渲染到BumpMap的合成贴图表面，其它贴图层先渲染，结果用于Bump和混色，才能看到正确的Bump效果
// 			INT subMeshCnt = pMtrl->subMeshList.size();
// 			RenderSubMesh* _pSubMesh = pMtrl->subMeshList.begin();
// 			for (INT i = 0; i< subMeshCnt; i++)
// 			{
// 				RenderSubMesh* pSubMesh = &(*(_pSubMesh + i));
// 				CModel* pModel = (CModel*)(pSubMesh->modelPointer);
// 
// 				//判断辅助渲染表面是否已经加入到模型辅助渲染列表，防止重复加入
// 				BOOL isExist = false;
// 				INT addRenderInfoModelCnt = pModel->m_additionalRenderInfo.size();
// 				for (INT j = 0; j < addRenderInfoModelCnt; j++)
// 				{
// 					if (pModel->m_additionalRenderInfo[j].pRenderToTexture == (DWORD)pBumpMap)
// 					{
// 						isExist = true;
// 						break;
// 					}
// 				}
// 				//该模型需要渲染到凹凸合成贴图表面，所以增加附加渲染信息
// 				if (isExist == false)
// 				{
// 					ADDITIONALRENDERINFO renderInfo;
// 					renderInfo.type = ADS_BUMPWAVEMAP;
// 					renderInfo.pRenderToTexture = (DWORD)pBumpMap;
// 					pModel->m_additionalRenderInfo.push_back(renderInfo);
// 				}
// 			}
// 		}
// 		break;		
// 		
// 		// 高度贴图
// 	case TLT_HEIGHTMAP:
// 		break;		
// 		
// 		// 实时折射
// 	case TLT_RTTMAP:
// 		break;					
// 	}
}

//-------------------------------------------------------------------------------------------
//***贴图相关的方法
//-------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetTextureCount()
// 参  数: 
// 用  途: 得到场景中贴图的数量
//-----------------------------------------------------------------------------
UINT CVGS2DLL::SunGetTextureCount()
{
	// return vgsScene->texture.size();
	return 0;
}
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetTextureNameByHandle()
// 参  数: 
// 用  途: 根据贴图的句柄得到贴图的名称
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetTextureNameByHandle(DWORD texHandle, CHAR* strTexName)
{
//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
	/*
	if (texHandle <= 0) return;
	CTexture* pTex = (CTexture*)texHandle;
	strcpy(strTexName, pTex->name);*/
}
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetTextureHandleByName()
// 参  数: CHAR* strTexName
// 用  途: 根据贴图的名称得到贴图的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetTextureHandleByName(CHAR* strTexName)
{
//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
	/*
	if (vgsScene == NULL || strTexName == NULL) return 0;
	
	UINT texCnt = vgsScene->texture.size();
	CTexture** ppTex = vgsScene->texture.begin();
	
	for (UINT i = 0; i< texCnt; i++)
	{
		CTexture* pTex = *(ppTex + i);
		if (strcmp(strTexName, pTex->name) == 0)
			return (DWORD)pTex;
	}*/
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetTextureHandleByIndex()
// 参  数: UINT index
// 用  途: 根据贴图的编号得到贴图的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetTextureHandleByIndex(UINT index)
{
// 	if(vgsScene == NULL) return 0;
// 	
// 	CTexture** ppTex = vgsScene->texture.begin();
// 	CTexture* pTex = *(ppTex + index);
// 	
	// 	return (DWORD)pTex;
	return 0;
}



//-----------------------------------------------------------------------------
// 方法名: CreateMovieTexture()
// 参  数: char* name, char* movieFilePath, INT mediaType, BOOL withAudio, CHAR* mtrlName
// 用  途: 创建电影贴图
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::CreateMovieTexture(CHAR* name, 
								   CHAR* movieFilePath, 
								   INT mediaType, 
								   BOOL withAudio, 
								   INT	repeatCnt,
								   CHAR* mtrlName,	
								   CRITICAL_SECTION* pRenderCS,
								   CRITICAL_SECTION* pCtrlCS) 
{
//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14

	/*
	//创建CTexture3D对象
	CMovieTexture* pMovieTex = new CMovieTexture(vgsScene->m_pd3dDevice, 
												pRenderCS,
												pCtrlCS);
	DWORD mtrlHandle = SunGetMaterialHandleByName(mtrlName);
	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	if (pMovieTex && pMtrl)
	{
		TextureLayer *pTextureLayer = new TextureLayer();
		strcpy(pMovieTex->name, name);
		pMovieTex->m_repeatCnt = repeatCnt;
		// pMtrl->pMovieTexture = (DWORD)pMovieTex;
		if (pMovieTex->Create(movieFilePath, 
								mediaType, 
								withAudio, 
								repeatCnt,
								mtrlHandle) == false)
		{
			SAFE_RELEASE_D3D(pMovieTex);
			SAFE_DELETE(pMovieTex);
		}

		strcpy(pTextureLayer->Name, name);
		pTextureLayer->type = TLT_MOVIE;
		pTextureLayer->Blenderinfo.ColorBlendOption = D3DTOP_SELECTARG1;
		pTextureLayer->Blenderinfo.ColorBlendParam = 1;
		pTextureLayer->Blenderinfo.AlphaBlendOption = D3DTOP_SELECTARG1;
		pTextureLayer->Blenderinfo.AlphaBlendParam = 1;
		pTextureLayer->UVIndex = 0;
		pTextureLayer->pTexture = (DWORD)pMovieTex;

		pMtrl->texLayersList.push_back(pTextureLayer);

		vgsScene->movieTexture.push_back(DWORD(pMovieTex));		


	}
	return (DWORD)pMovieTex;
	*/
	
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: GetMovieTextureHandleByName()
// 参  数: char* name
// 用  途: 通过名称得到电影贴图对象
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::GetMovieTextureHandleByName(CHAR* name)
{
	/*
	INT movieTexCnt = vgsScene->movieTexture.size();
	CMovieTexture** ppMovieTex = (CMovieTexture**)(vgsScene->movieTexture.begin());
	for (INT i = 0; i< movieTexCnt; i++)
	{
		CMovieTexture* pMovieTex = *(ppMovieTex + i);
		if (strcmp(pMovieTex->name, name) == 0)
			return (DWORD)pMovieTex;
	}
	*/
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: GetMovieTextureHandleByIndex()
// 参  数: INT index
// 用  途: 通过编号得到电影贴图对象
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::GetMovieTextureHandleByIndex(INT index)
{
	/*
	INT movieTexCnt = vgsScene->movieTexture.size();
	if (index >= movieTexCnt) return 0;

	CMovieTexture** ppMovieTex = (CMovieTexture**)(vgsScene->movieTexture.begin());
	CMovieTexture* pMovieTex = *(ppMovieTex + index);
		return (DWORD)pMovieTex;
		*/
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SetMovieMtrl
// 参  数: DWORD mtrlHandle, DWORD movieTexHandle
// 用  途: 设置材质为电影贴图材质
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SetMovieMtrl(DWORD mtrlHandle, DWORD movieTexHandle)
{
//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
	/*CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	if (pMtrl)
		pMtrl->pMovieTexture = movieTexHandle;
	else
		return;

	CMovieTexture* pMovieTex = (CMovieTexture*)(pMtrl->pMovieTexture);
	if (!pMovieTex->m_pMovieTexRender)
		return;*/
	
}

//删除电影贴图
VOID	CVGS2DLL::DeleteMovieTexture(DWORD movieTexHandle)
{
	/*
	INT movieTexCnt = vgsScene->movieTexture.size();
	CMovieTexture** ppMovieTex = (CMovieTexture**)(vgsScene->movieTexture.begin());
	for (INT i = 0; i< movieTexCnt; i++)
	{
		CMovieTexture* pMovieTex = *(ppMovieTex + i);

		if ((DWORD)pMovieTex == movieTexHandle)
		{	
			SAFE_RELEASE(pMovieTex);
			SAFE_DELETE(pMovieTex);
			vgsScene->movieTexture.erase((DWORD*)(ppMovieTex + i));
			break;
		}
	}
	*/
}

//删除电影贴图
VOID CVGS2DLL::DeleteMovieTextureByName(CHAR* name)
{
/*
	INT movieTexCnt = vgsScene->movieTexture.size();
	CMovieTexture** ppMovieTex = (CMovieTexture**)(vgsScene->movieTexture.begin());
	for (INT i = 0; i< movieTexCnt; i++)
	{
		CMovieTexture* pMovieTex = *(ppMovieTex + i);

		if (strcmp(pMovieTex->name, name) == 0)
		{	
			SAFE_RELEASE(pMovieTex);
			SAFE_DELETE(pMovieTex);
			vgsScene->movieTexture.erase((DWORD*)(ppMovieTex + i));
			break;
		}
	}
	*/
}

//删除电影贴图
VOID CVGS2DLL::DeleteMovieTextureByIndex(INT index)
{
	/*
	INT movieTexCnt = vgsScene->movieTexture.size();
	if (index < 0 || index > movieTexCnt)
		return;

	CMovieTexture** ppMovieTex = (CMovieTexture**)(vgsScene->movieTexture.begin());
	CMovieTexture* pMovieTex = *(ppMovieTex + index);

	SAFE_RELEASE(pMovieTex);
	SAFE_DELETE(pMovieTex);
	vgsScene->movieTexture.erase((DWORD*)(ppMovieTex + index));
	*/
}

//删除所有的电影贴图
VOID CVGS2DLL::DeleteAllMovieTexture()
{
	/*
	INT movieTexCnt = vgsScene->movieTexture.size();
	CMovieTexture** ppMovieTex = (CMovieTexture**)(vgsScene->movieTexture.begin());
	for (INT i = 0; i< movieTexCnt; i++)
	{
		CMovieTexture* pMovieTex = *(ppMovieTex + i);
		SAFE_RELEASE(pMovieTex);
		SAFE_DELETE(pMovieTex);
		vgsScene->movieTexture.erase((DWORD*)(ppMovieTex + i));
	}
	vgsScene->movieTexture.clear();
	*/
}


//-------------------------------------------------------------------------------------------
//***播放声音相关的方法
//-------------------------------------------------------------------------------------------
DWORD CVGS2DLL::CreateAudio(CHAR* name, CHAR* AudioFilePath, INT repeatCnt)
{
	/*
	//创建AudioPlayer对象
	CAudioPlayer* pAudio = new CAudioPlayer();
	if (pAudio)
	{
		strcpy(pAudio->name, name);
		pAudio->index = audioPlayer.size();
		pAudio->m_repeatCnt = repeatCnt;
		audioPlayer.push_back(pAudio);
		if (pAudio->Create(AudioFilePath, repeatCnt) == false)
		{
			SAFE_RELEASE_D3D(pAudio);
			SAFE_DELETE(pAudio);
		}
	}
	return (DWORD)pAudio;
	*/
	return 0;
}

VOID CVGS2DLL::DeleteAudio(DWORD audioHandle)
{
	/*
	CAudioPlayer* pCurAudio = (CAudioPlayer*)audioHandle;
	if (pCurAudio)
	{
		INT audioCnt = audioPlayer.size();
		CAudioPlayer** ppAudio = audioPlayer.begin();
		for (INT i = 0; i< audioCnt; i++)
		{
			CAudioPlayer* pAudio = *(ppAudio + i);
			if (pCurAudio == pAudio)
			{
				SAFE_RELEASE(pAudio);
				SAFE_DELETE(pAudio);
				audioPlayer.erase(ppAudio + i);
				return;
			}
		}
	}
	*/
}

VOID CVGS2DLL::DeleteAudioByName(CHAR* name)
{
	/*
	INT audioCnt = audioPlayer.size();
	CAudioPlayer** ppAudio = audioPlayer.begin();
	for (INT i = 0; i< audioCnt; i++)
	{
		CAudioPlayer* pAudio = *(ppAudio + i);
		if (strcmp(pAudio->name, name) == 0)
		{
			SAFE_RELEASE(pAudio);
			SAFE_DELETE(pAudio);
			audioPlayer.erase(ppAudio + i);
			return;
		}
	}	
	*/
}

//给AudioPlayer重新编号
VOID ResetAudioPlayerIndex()
{
	/*
	INT audioCnt = audioPlayer.size();
	CAudioPlayer** ppAudio = audioPlayer.begin();
	for (INT i = 0; i< audioCnt; i++)
	{
		CAudioPlayer* pAudio = *(ppAudio + i);	
		pAudio->index = i;
	}
	*/
}

VOID CVGS2DLL::DeleteAudioByIndex(INT index)
{
	/*
	INT audioCnt = audioPlayer.size();
	if (index >= audioCnt)
		return;

	ResetAudioPlayerIndex();

	CAudioPlayer** ppAudio = audioPlayer.begin();

	CAudioPlayer* pAudio = *(ppAudio + index);
	SAFE_RELEASE(pAudio);
	SAFE_DELETE(pAudio);
	audioPlayer.erase(ppAudio + index);
	return;
	*/
}

VOID CVGS2DLL::DeleteAllAudio()
{
	/*
	INT audioCnt = audioPlayer.size();
	CAudioPlayer** ppAudio = audioPlayer.begin();
	for (INT i = 0; i< audioCnt; i++)
	{
		CAudioPlayer* pAudio = *(ppAudio + i);	
		SAFE_RELEASE(pAudio);
		SAFE_DELETE(pAudio);	
	}	
	audioPlayer.clear();
	*/
}


INT CVGS2DLL::GetAudioCount()
{
	/*
	return audioPlayer.size();
	*/
	return 0;
}


DWORD CVGS2DLL::GetAudioHandleByName(CHAR* name)
{
	/*
	INT audioCnt = audioPlayer.size();
	CAudioPlayer** ppAudio = audioPlayer.begin();
	for (INT i = 0; i< audioCnt; i++)
	{
		CAudioPlayer* pAudio = *(ppAudio + i);
		if (strcmp(pAudio->name, name) == 0)
			return (DWORD)pAudio;
	}	
	*/
	return 0;
}

DWORD CVGS2DLL::GetAudioHandleByIndex(INT index)
{
	/*
	INT audioCnt = audioPlayer.size();
	if (index >= audioCnt)
		return 0;

	CAudioPlayer** ppAudio = audioPlayer.begin();
	CAudioPlayer* pAudio = *(ppAudio + index);
	return (DWORD)pAudio;
	*/
	return 0;
}


VOID CVGS2DLL::GetAudioNameByHandle(DWORD audioHandle, CHAR* name)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		strcpy(name, pAudio->name);
		*/
}

INT CVGS2DLL::GetAudioIndexByHandle(DWORD audioHandle)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
	{
		ResetAudioPlayerIndex();
		return pAudio->index;
	}
	*/
	return -1;
}


VOID CVGS2DLL::SetAudioFilePath(DWORD audioHandle, CHAR* AudioFilePath)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		pAudio->SetAudioFilePath(AudioFilePath);
		*/
}

VOID CVGS2DLL::GetAudioFilePath(DWORD audioHandle, CHAR* AudioFilePath)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		pAudio->GetAudioFilePath(AudioFilePath);
		*/
}


VOID CVGS2DLL::SetAudioRepeatCount(DWORD audioHandle, INT repeatCount)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		pAudio->m_repeatCnt = repeatCount;
		*/
}

INT	CVGS2DLL::GetAudioRepeatCount(DWORD audioHandle)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		return pAudio->m_repeatCnt;
*/
	return 0;
}

VOID CVGS2DLL::PlayAudio(DWORD audioHandle)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		pAudio->Play();
		*/
}

VOID CVGS2DLL::PauseAudio(DWORD audioHandle)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		pAudio->Pause();
		*/
}

VOID CVGS2DLL::StopAudio(DWORD audioHandle)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		pAudio->Stop();
		*/
}

VOID CVGS2DLL::RewindAudio(DWORD audioHandle)
{
	/*
	CAudioPlayer* pAudio = (CAudioPlayer*)audioHandle;
	if (pAudio)
		pAudio->Rewind();
		*/
}

//-------------------------------------------------------------------------------------------
//***模型成组相关的方法
//-------------------------------------------------------------------------------------------
DWORD	CVGS2DLL::CreateModelGroup(CHAR* groupName)
{
// 	CModelGroup* pModelGroup = new CModelGroup();
// 	if (pModelGroup)
// 	{
// 		strcpy(pModelGroup->name, groupName);
// 		pModelGroup->index = vgsScene->modelGroup.size();
// 		vgsScene->modelGroup.push_back(pModelGroup);
// 	}
	// 	return (DWORD)pModelGroup;
	return 0;
}

VOID	CVGS2DLL::DeleteModelGroup(DWORD ModelGroupHandle)
{
// 	CModelGroup* sModelGroup = (CModelGroup*)ModelGroupHandle;
// 	if (sModelGroup)
// 	{
// 		INT modelCnt = vgsScene->modelGroup.size();
// 		CModelGroup** ppModelGroup = vgsScene->modelGroup.begin();
// 		for (INT i = 0; i< modelCnt; i++)
// 		{
// 			CModelGroup* pModelGroup = *(ppModelGroup + i);
// 			if (sModelGroup == pModelGroup)
// 			{
// 				SAFE_RELEASE(pModelGroup);
// 				SAFE_DELETE(pModelGroup);
// 				vgsScene->modelGroup.erase(ppModelGroup + i);
// 				break;
// 			}
// 		}
	// 	}
	
}

VOID	CVGS2DLL::DeleteModelGroupByName(CHAR* groupName)
{
// 	if (groupName)
// 	{
// 		INT modelCnt = vgsScene->modelGroup.size();
// 		CModelGroup** ppModelGroup = vgsScene->modelGroup.begin();
// 		for (INT i = 0; i< modelCnt; i++)
// 		{
// 			CModelGroup* pModelGroup = *(ppModelGroup + i);
// 			if (strcmp(groupName, pModelGroup->name) == 0)
// 			{
// 				pModelGroup->model.clear();
// 				SAFE_RELEASE(pModelGroup);
// 				SAFE_DELETE(pModelGroup);
// 				vgsScene->modelGroup.erase(ppModelGroup + i);
// 				break;
// 			}
// 		}
// 	}
}	

VOID	CVGS2DLL::DeleteModelGroupByIndex(INT index)
{
// 	INT modelCnt = vgsScene->modelGroup.size();
// 	if (index >= modelCnt || index < 0)
// 		return;
// 
// 	CModelGroup** ppModelGroup = vgsScene->modelGroup.begin();
// 	CModelGroup* pModelGroup = *(ppModelGroup + index);
// 
// 	pModelGroup->model.clear();
// 	SAFE_RELEASE(pModelGroup);
// 	SAFE_DELETE(pModelGroup);
// 	vgsScene->modelGroup.erase(ppModelGroup + index);
}

VOID	CVGS2DLL::DeleteAllModelGroup()
{	
// 	INT modelCnt = vgsScene->modelGroup.size();
// 	CModelGroup** ppModelGroup = vgsScene->modelGroup.begin();
// 	for (INT i = 0; i< modelCnt; i++)
// 	{
// 		CModelGroup* pModelGroup = *(ppModelGroup + i);
// 		SAFE_RELEASE(pModelGroup);
// 		SAFE_DELETE(pModelGroup);
// 	}
// 	vgsScene->modelGroup.clear();
}

INT		CVGS2DLL::GetModelGroupCount()
{
	// return vgsScene->modelGroup.size();
	return 0;
}

DWORD	CVGS2DLL::GetModelGroupHandleByName(CHAR* groupName)
{
// 	INT modelCnt = vgsScene->modelGroup.size();
// 	CModelGroup** ppModelGroup = vgsScene->modelGroup.begin();
// 	for (INT i = 0; i< modelCnt; i++)
// 	{
// 		CModelGroup* pModelGroup = *(ppModelGroup + i);
// 		if (strcmp(groupName, pModelGroup->name) == 0)
// 			return (DWORD)pModelGroup;
// 	}
	return 0;
}

DWORD	CVGS2DLL::GetModelGroupHandleByIndex(INT index)
{
// 	INT modelCnt = vgsScene->modelGroup.size();
// 	if (index >= modelCnt || index < 0)
// 		return 0;
// 
// 	CModelGroup** ppModelGroup = vgsScene->modelGroup.begin();
// 	CModelGroup* pModelGroup = *(ppModelGroup + index);
	// 	return (DWORD)pModelGroup;
	return 0;
}

VOID	CVGS2DLL::GetModelGroupNameByHandle(DWORD modelGroupHandle, CHAR* groupName)
{
// 	CModelGroup* pModelGroup = (CModelGroup*)modelGroupHandle;
// 	if (pModelGroup)
// 		strcpy(groupName, pModelGroup->name);
}

VOID	CVGS2DLL::ResetModelGroupIndex()
{
// 	INT modelCnt = vgsScene->modelGroup.size();
// 	CModelGroup** ppModelGroup = vgsScene->modelGroup.begin();
// 	for (INT i = 0; i< modelCnt; i++)
// 	{
// 		CModelGroup* pModelGroup = *(ppModelGroup + i);
// 		pModelGroup->index = i;
// 	}
}

INT		CVGS2DLL::GetModelGroupIndexByHandle(DWORD ModelGroupHandle)
{
// 	CModelGroup* pModelGroup = (CModelGroup*)ModelGroupHandle;
// 	if (pModelGroup)
// 	{
// 		ResetModelGroupIndex();
// 		return pModelGroup->index;
// 	}
	return -1;
}

VOID	CVGS2DLL::AddModelToModelGroup(DWORD modelGroupHandle, DWORD modelHandle)
{
// 	CModelGroup* pModelGroup = (CModelGroup*)modelGroupHandle;
// 	CModel* pModel = (CModel*)modelHandle;
// 	if (pModelGroup && pModel)
// 	{
// 		pModelGroup->model.push_back(pModel);
// 		pModel->modelGroup = (DWORD)pModelGroup;
// 	}
}

VOID	CVGS2DLL::DeleteModelFromModelGroup(DWORD modelGroupHandle, DWORD modelHandle)
{
// 	CModelGroup* pModelGroup = (CModelGroup*)modelGroupHandle;
// 	CModel* sModel = (CModel*)modelHandle;
// 	if (pModelGroup && sModel)
// 	{
// 		CModel** ppModel = pModelGroup->model.begin();
// 		INT modelCnt = pModelGroup->model.size();
// 		for (INT i = 0; i< modelCnt; i++)
// 		{
// 			CModel* pModel = *(ppModel + i);
// 			if (pModel == sModel)
// 			{
// 				pModelGroup->model.erase(ppModel + i);
// 				pModel->modelGroup = 0;
// 				return;
// 			}
// 		}
// 	}
}

VOID	CVGS2DLL::ClearModelGroup(DWORD modelGroupHandle)
{
// 	CModelGroup* pModelGroup = (CModelGroup*)modelGroupHandle;
// 	if (pModelGroup)
// 	{
// 		CModel** ppModel = pModelGroup->model.begin();
// 		INT modelCnt = pModelGroup->model.size();
// 		for (INT i = 0; i< modelCnt; i++)
// 		{
// 			CModel* pModel = *(ppModel + i);
// 			pModel->modelGroup = 0;
// 		}
// 	}
// 	pModelGroup->model.clear();
}


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunCreateCubeMap()
// 参  数: DWORD modelHandle, UINT length, UINT mode);
// 用  途: 创建CubeMap, 6副图片，size为cubeMap的尺寸，
// power为CubeMap反射的强度，
// 返回CubeMap对象的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::CreateStaticCubeMap(CHAR* name, DWORD frontImage, DWORD backImage, 
									  DWORD leftImage, DWORD rightImage, 
									  DWORD topImage, DWORD bottomImage,
									  INT size)
{
	/*
	//---------------------------------------------------------------------------------------------------
	//commend by baojiahui 2008/8/14
	
	//判断是否支持
	if (!vgsScene->d3dParam.IsSupportCUBEMAP)
		return 0;

	if (!frontImage || !backImage || 
		!leftImage || !rightImage ||
		!topImage || !bottomImage)
		return NULL;

	CCubeMap* pCubeMap = new CCubeMap(vgsScene->m_pd3dDevice, 0, D3DFMT_R5G6B5, size);
	if(pCubeMap)
	{
		strcpy(pCubeMap->name, name);
		if (pCubeMap->AddStaticEnvTexture(frontImage, backImage, 
										leftImage, rightImage, 
										topImage, bottomImage))
		{
			if (pCubeMap->InitCubeMap())
				return (DWORD)pCubeMap;
			else
			{
				SAFE_DELETE(pCubeMap);
				return 0;
			}
		}
		else
		{
			SAFE_DELETE(pCubeMap);
			return 0;
		}
	}
	*/
	return 0;
}

//删除CubeMap
VOID  CVGS2DLL::DeleteStaticCubeMap(DWORD cubeMapHandle)
{
	/*
	INT cubeMapCnt = vgsScene->cubeMap.size();
	CCubeMap** ppCubeMap = vgsScene->cubeMap.begin();
	for (INT i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* pCubeMap = *(ppCubeMap + i);
		if ((DWORD)pCubeMap == cubeMapHandle)
		{
			//删除CubeMap用的Texture
			if (!IsTextureInScene((DWORD)(pCubeMap->m_pFrontTex)))
				SunDeleteTexture((DWORD)(pCubeMap->m_pFrontTex));

			if (!IsTextureInScene((DWORD)(pCubeMap->m_pBackTex)))
				SunDeleteTexture((DWORD)(pCubeMap->m_pBackTex));

			if (!IsTextureInScene((DWORD)(pCubeMap->m_pLeftTex)))
				SunDeleteTexture((DWORD)(pCubeMap->m_pLeftTex));
			
			if (!IsTextureInScene((DWORD)(pCubeMap->m_pRightTex)))
				SunDeleteTexture((DWORD)(pCubeMap->m_pRightTex));
			
			if (!IsTextureInScene((DWORD)(pCubeMap->m_pTopTex)))
				SunDeleteTexture((DWORD)(pCubeMap->m_pTopTex));

			if (!IsTextureInScene((DWORD)(pCubeMap->m_pBottomTex)))
				SunDeleteTexture((DWORD)(pCubeMap->m_pBottomTex));

			//删除CubeMap用的贴图
			SAFE_RELEASE(pCubeMap);
			SAFE_DELETE(pCubeMap);
			vgsScene->cubeMap.erase(ppCubeMap + i);
		}
	}
	*/
}

//-----------------------------------------------------------------------------

//返回CubeMap对象的句柄
DWORD CVGS2DLL::CreateDynamicCubeMap(CHAR* cubeMapName, CHAR* centerModelName, UINT size, FLOAT power)
{
	/*
	//判断是否支持
	if (!vgsScene->d3dParam.IsSupportCUBEMAP) return 0;
	if (size == 0)	return NULL;

	CCubeMap* pCubeMap = new CCubeMap(vgsScene->m_pd3dDevice, 1, D3DFMT_R5G6B5, size);
	if (pCubeMap == NULL) return NULL;

	CModel* pModel = (CModel*)SunGetModelHandleByName(centerModelName);
	if (pModel == NULL) return NULL;

	strcpy(pCubeMap->name, cubeMapName);
	if (pCubeMap->InitCubeMap())
	{
		pCubeMap->m_pCenterModel = (DWORD)pModel;
		pCubeMap->cubeMapPower = power;
		return (DWORD)pCubeMap;
	}
	else
	{
		SAFE_DELETE(pCubeMap);
		return 0;
	}
	*/

	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::AddDyCubeMapReflectionModel()
// 参  数: DWORD cubeMapHandle, DWORD modelHandle
// 用  途: 设置生成CubeMap需要渲染的模型
//-----------------------------------------------------------------------------
VOID CVGS2DLL::AddDyCubeMapReflectionModel(DWORD cubeMapHandle, DWORD modelHandle)
{
// 	CCubeMap* pCubeMap = (CCubeMap*)cubeMapHandle;
// 	if (pCubeMap == NULL) return;
// 	//如果是静态的CubeMap-----------------------------------------------------
// 	if (pCubeMap->m_Mode == 0) return;
// 	pCubeMap->AddEnvModel(modelHandle);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::DeleteDyCubeMapReflectionModel()
// 参  数: DWORD cubeMapHandle, DWORD modelHandle
// 用  途: 设置生成CubeMap需要渲染的模型
//-----------------------------------------------------------------------------
VOID CVGS2DLL::DeleteDyCubeMapReflectionModel(DWORD cubeMapHandle, DWORD modelHandle)
{
// 	CCubeMap* pCubeMap = (CCubeMap*)cubeMapHandle;
// 	if (pCubeMap == NULL) return;
// 	//如果是静态的CubeMap-----------------------------------------------------
// 	if (pCubeMap->m_Mode == 0) return;
// 
// 	CModel* pModel = (CModel*)modelHandle;
// 	if (pModel == NULL) return;
// 
// 	int useMeCubeMapCnt = pModel->useMeCubeMap.size();
// 	CCubeMap** ppCubeMap = pModel->useMeCubeMap.begin();
// 	for (int i = 0; i< useMeCubeMapCnt; i++)
// 	{
// 		//将CubeMap对象从模型的useMeCubeMap列表中删除
// 		CCubeMap* _pCubeMap = *(ppCubeMap + i);
// 		if (_pCubeMap == pCubeMap)
// 		{
// 			pModel->useMeCubeMap.erase(ppCubeMap + i);
// 			return;
// 		}
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: SunCreateTexture()
// 参  数: CHAR* strImageFile, CHAR* texName, TEXFORMAT format
// 用  途: 创建贴图
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunCreateTexture(CHAR* texName, CHAR* strImageFile, TEXFORMAT format)
{
//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
/*	if (vgsScene == NULL) return 0;
	if (strImageFile == NULL || strcmp(strImageFile, "") == 0) return 0;
	
	CTexture* pTex = new CTexture();
	if (pTex != NULL)
	{
		switch (format)
		{
		case RGBA5650:
			pTex->BP = D3DFMT_R5G6B5;
			break;
		case RGBA5551:
			pTex->BP = D3DFMT_A1R5G5B5;
			break;
		case RGBA4444:
			pTex->BP = D3DFMT_A4R4G4B4;
			break;
		case RGBA8888:
			pTex->BP = D3DFMT_A8R8G8B8;
			break;
		case RGBA8880:
			pTex->BP = D3DFMT_R8G8B8;
			break;
		}
		
		pTex->index = vgsScene->texture.size();
		strcpy(pTex->name, texName);
		vgsScene->CreateTexFromFile(strImageFile, pTex, 1);
		//将当前贴图对象放入场景贴图列表
		vgsScene->texture.push_back(pTex);
	}
	
	return (DWORD)pTex;*/
	return 0;

}

//-----------------------------------------------------------------------------
// 方法名: SunCreateTextureFromImage()
// 参  数: CHAR* texName, CHAR* strImageName, TEXFORMAT format
// 用  途: 从Image对象创建贴图, 不自动放入texture对列
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunCreateTextureFromImage(CHAR* texName, CHAR* strImageName, TEXFORMAT format, INT mipLevel)
{
//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
	/*
	if (vgsScene == NULL) return 0;
	if (strImageName == NULL || strcmp(strImageName, "") == 0 || strcmp(strImageName, "0") == 0) return 0;
	
	DWORD ImageHandle = SunGetImageHandleByName(strImageName);
	CImage* pImage = (CImage*)ImageHandle;
	
	CTexture* pTex = new CTexture();
	pTex->index = vgsScene->texture.size();
	strcpy(pTex->name, texName);
	pTex->dataSize = pImage->dataLength;
	pTex->data = new BYTE[pTex->dataSize];
	memcpy(pTex->data, pImage->data, pImage->dataLength);
	
	switch (format)
	{
	case RGBA5650:
		pTex->BP = D3DFMT_R5G6B5;
		break;
	case RGBA5551:
		pTex->BP = D3DFMT_A1R5G5B5;
		break;
	case RGBA4444:
		pTex->BP = D3DFMT_A4R4G4B4;
		break;
	case RGBA8888:
		pTex->BP = D3DFMT_A8R8G8B8;
		break;
	case RGBA8880:
		pTex->BP = D3DFMT_R8G8B8;
		break;
	}
	
	if (SUCCEEDED(vgsScene->CreateTexFromBuffer(pTex, mipLevel)))
	{
		SAFE_DELETE(pTex->data);
		return (DWORD)pTex;
	}

	SAFE_RELEASE(pTex);
	SAFE_DELETE(pTex);
	*/
	return 0;
}

// 从Image buffer更新使用该Image的所有texture
VOID CVGS2DLL::SunFillTexturesFromBuffer(UINT len, CHAR* pBuffer, INT imgIndex)
{
	m_VGSCore.FillTexturesFromBuffer(len, pBuffer, imgIndex);
}
//-----------------------------------------------------------------------------
// 方法名: SunDeleteTexture()
// 参  数: DWORD texHandle
// 用  途: 删除贴图
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDeleteTexture(DWORD texHandle)
{
// 	if (vgsScene == NULL || texHandle <= 0) return;
// 	
// 	INT texCnt = vgsScene->texture.size();
// 	CTexture** ppTex = vgsScene->texture.begin();
// 	for (INT i = 0; i< texCnt; i++)
// 	{
// 		CTexture* pTex = *(ppTex + i);
// 		if ((DWORD)pTex == texHandle)
// 		{
// 			vgsScene->texture.erase(ppTex + i);
// 			SAFE_RELEASE(pTex);
// 			SAFE_DELETE(pTex);
// 			break;
// 		}
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: SunSetTextureFormat()
// 参  数: DWORD texHandle, TEXFORMAT format
// 用  途: 设置贴图的FORMAT
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetTextureFormat(DWORD texHandle, TEXFORMAT format)
{
// 	if (vgsScene == NULL) return;
// 	CTexture* pTex = (CTexture*)texHandle;
// 	if (pTex == NULL) return;
// 	
// 	switch (format)
// 	{
// 	case RGBA5650:
// 		pTex->BP = D3DFMT_R5G6B5;
// 		break;
// 	case RGBA5551:
// 		pTex->BP = D3DFMT_R5G6B5;
// 		break;
// 	case RGBA4444:
// 		pTex->BP = D3DFMT_A4R4G4B4;
// 		break;
// 	case RGBA8880:
// 		pTex->BP = D3DFMT_R8G8B8;
// 		break;
// 	case RGBA8888:
// 		pTex->BP = D3DFMT_A8R8G8B8;
// 		break;
// 	}
}



//-----------------------------------------------------------------------------
//摄像机相关方法
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetCameraCount()
// 参  数: 
// 用  途: 得到场景中摄像机的数量
//-----------------------------------------------------------------------------
UINT CVGS2DLL::SunGetCameraCount()
{
	// return vgsScene->camera.size();
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetCameraNameByHandle()
// 参  数: DWORD camHandle, CHAR* strCameraName
// 用  途: 根据像机的句柄得到像机的名称
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetCameraNameByHandle(DWORD camHandle, CHAR* strCameraName)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	strcpy(strCameraName, pCamera->name.c_str());
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetCameraHandleByName()
// 参  数: CHAR* strCameraName, DWORD* camHandle
// 用  途: 根据像机的名称得到像机的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetCameraHandleByName(CHAR* strCameraName)
{
// 	if (vgsScene == NULL || strCameraName == NULL) return 0;
// 	
// 	CHAR camName[32];
// 	strcpy(camName, strCameraName);
// 	StrToCaption(camName);
// 	
// 	UINT cameraCnt = vgsScene->camera.size();
// 	CCamera** ppCamera = vgsScene->camera.begin();
// 	
// 	for(UINT i = 0; i< cameraCnt; i++)
// 	{
// 		CCamera* pCamera = *(ppCamera + i);
// 		if (strcmp(pCamera->name.c_str(), camName) == 0)
// 			return (DWORD)pCamera;
// 	}
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetCameraHandleByIndex()
// 参  数: UINT index
// 用  途: 根据摄像机的编号得到像机的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetCameraHandleByIndex(UINT index)
{
// 	if (vgsScene == NULL) return 0;
// 	
// 	CCamera** ppCamera = vgsScene->camera.begin();
// 	if (ppCamera == NULL) return 0;
// 	
// 	CCamera* pCamera = *(ppCamera + index);
// 	
	// 	return (DWORD)pCamera;
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCurrentCamera()
// 参  数: DWORD camHandle
// 用  途: 设置当前摄像机
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCurrentCamera(DWORD camHandle)
{
	DWORD controller = SunGetCurrentController();
	m_VGSCore.SetControllerCamera(m_vgsScene, controller, camHandle);
// 	if (vgsScene == NULL ||camHandle <= 0) return;
// 	vgsScene->currentCam = (CCamera*)camHandle;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetCurrentCamera()
// 参  数: 
// 用  途: 得到当前摄像机句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetCurrentCamera()
{
	DWORD controller = SunGetCurrentController();
	return m_VGSCore.GetControllerCamera(m_vgsScene, controller);
	// 	return (DWORD)vgsScene->currentCam;
//	return 0;
}

/** 相机目标点不动，沿相机方向前后推拉相机。
@param in sceneHandle : 场景指针
@param in camHandle : 相机指针
@param dollyValue  in   : 相机推拉的距离，> 0 向前， < 1向后
*/
VOID CVGS2DLL::SunDollyCamera(DWORD camHandle, FLOAT value)
{
	// DollyCamera(m_vgsScene, camHandle, value);
}
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunMoveCamera()
// 参  数: DWORD camHandle, VECTOR3* pVec, UINT coordEnum
// 用  途: 平移摄像机
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunMoveCamera(DWORD camHandle, VECTOR3* pVec, UINT coordEnum)
{
	m_VGSCore.MoveCamera(m_vgsScene, camHandle, pVec, coordEnum);
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->Translate(&D3DXVECTOR3(pVec->x, pVec->y, pVec->z), coordEnum);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunRotateCamera()
// 参  数: DWORD camHandle, VECTOR3* pVec, UINT coordEnum
// 用  途: 旋转摄像机
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunRotateCamera(DWORD camHandle, VECTOR3* pVec, UINT coordEnum)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->Rotate(&D3DXVECTOR3(pVec->x*ATOS, pVec->y*ATOS, pVec->z*ATOS), coordEnum);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunRoundCamera()
// 参  数: DWORD camHandle, VECTOR3* pAxisVec, FLOAT fAngle
// 用  途: 以目标点为中心，以pAxisVec为轴，旋转From点fAngle弧度
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunRoundCamera(DWORD camHandle, VECTOR3* pAxisVec, FLOAT fAngle) 
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->Round(&D3DXVECTOR3(pAxisVec->x, pAxisVec->y, pAxisVec->z), fAngle*ATOS);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCameraLookAt()
// 参  数: DWORD camHandle, DWORD modelHandle
// 用  途: 摄像机正对
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCameraLookAt(DWORD camHandle, DWORD modelHandle)
{
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	if (camHandle > 0)
// 	{
// 		CModel* pModel = (CModel*)modelHandle;
// 		pCamera->CreateLookAt(pModel);
// 	}
// 	else
// 		pCamera->DeleteLookAt();
	
}

//-----------------------------------------------------------------------------
// 方法名: CreateCamera
// 参  数: 
// 用  途: 创建像机
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunCreateCamera(const CHAR* name, D3DXVECTOR3* pFrom, D3DXVECTOR3* pAt)
{	
	DWORD camHanlde = m_VGSCore.CreateCamera(m_vgsScene, name);
	m_VGSCore.SetCameraPosition(m_vgsScene, camHanlde, &VECTOR3(pFrom->x, pFrom->y, pFrom->z));
	m_VGSCore.SetCameraAt(m_vgsScene, camHanlde, &VECTOR3(pAt->x, pAt->y, pAt->z));
// 	CCamera* pCamera = new CCamera();
// 	if (pCamera)
// 	{
// 		pCamera->name = std::string(name);
// 		pCamera->SetFrom(pFrom);
// 		pCamera->SetAt(pAt);
// 		return (DWORD)pCamera;
// 	}
	return camHanlde;
}

// 创建一个缺省相机
DWORD CVGS2DLL::SunCreatePredefinedCamera(DEFAULTCAMERA_TYPE type)
{
	DWORD cam = 0;
	switch (type)
	{
	case PERSPECTIVE_CAMERA:
		cam = m_VGSCore.CreatePerspectiveCamera(m_vgsScene);
		break;
	default:
		break;
	}
	return cam;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCameraFrom()
// 参  数: DWORD camHandle, VECTOR3* pPos
// 用  途: 设置摄像机的起始点
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCameraFrom(DWORD camHandle, VECTOR3* pPos)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->SetFrom(&D3DXVECTOR3(pPos->x, pPos->y, pPos->z));
	
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCameraAt()
// 参  数: DWORD camHandle, VECTOR3* pPos
// 用  途: 设置摄像机的目标点
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCameraAt(DWORD camHandle, VECTOR3* pPos)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->SetAt(&D3DXVECTOR3(pPos->x, pPos->y, pPos->z));
	m_VGSCore.SetCameraAt(m_vgsScene, camHandle, pPos);
}

VOID CVGS2DLL::SunGetCameraAt(DWORD camHandle, VECTOR3* pPos)
{
	m_VGSCore.GetCameraAt(m_vgsScene, camHandle, pPos);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCameraFov()
// 参  数: DWORD camHandle, FLOAT fov
// 用  途: 设置摄像机的FOV
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCameraFov(DWORD camHandle, FLOAT fov)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->SetFov(fov);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCameraNearClip()
// 参  数: DWORD camHandle, FLOAT nearClip
// 用  途: 设置摄像机的nearClip
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCameraNearClip(DWORD camHandle, FLOAT nearClip)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->m_nearClip = nearClip;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCameraFarClip()
// 参  数: DWORD camHandle, FLOAT farClip
// 用  途: 设置摄像机的farClip
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCameraFarClip(DWORD camHandle, FLOAT farClip)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->m_farClip = farClip;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCameraRoll()
// 参  数: DWORD camHandle, FLOAT rollAngle
// 用  途: 设置摄像机的Roll
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCameraRoll(DWORD camHandle, FLOAT rollAngle)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;
// 	pCamera->SetRoll(rollAngle*ATOS);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetCameraInfo()
// 参  数: DWORD camHandle, CameraInfo* pCamInfo
// 用  途: 一次设置摄像机的属性
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetCameraInfo(DWORD camHandle, CameraInfo* pCamInfo)
{
// 	if (camHandle <= 0) return;
// 	CCamera* pCamera = (CCamera*)camHandle;	
// 	pCamera->SetFrom(&D3DXVECTOR3(pCamInfo->from.x, pCamInfo->from.y, pCamInfo->from.z));
// 	pCamera->SetAt(&D3DXVECTOR3(pCamInfo->at.x, pCamInfo->at.y, pCamInfo->at.z));
// 	pCamera->SetRoll(pCamInfo->roll);
// 	pCamera->SetFov(pCamInfo->fov);
// 	pCamera->m_nearClip = pCamInfo->nearClip;
// 	pCamera->m_farClip = pCamInfo->farClip;
}

//-----------------------------------------------------------------------------
// 方法名: SetAutoPitchRoundRollSpeed()
// 参  数: FLOAT fPitchValue, FLOAT fRoundValue, FLOAT fRollValue
// 用  途: 设置EDIT模式下摄像机旋转的速度，只有在EDIT模式下生效
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SetAutoPitchRoundRollSpeed(FLOAT fPitchValue, FLOAT fRoundValue, FLOAT fRollValue)
{
// 	editor.autoPitchSpeed = fPitchValue;
// 	editor.autoRoundSpeed = fRoundValue;
// 	editor.autoRollSpeed = fRollValue;
}

//-----------------------------------------------------------------------------
// 方法名: SetManualPitchRoundSpeed()
// 参  数: FLOAT fPitchValue, FLOAT fRollValue
// 用  途: 设置EDIT模式下当前摄像机手动旋转的速度，只有在EDIT模式下生效
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SetManualPitchRoundSpeed(FLOAT fPitchValue, FLOAT fRoundValue)
{
// 	editor.manualPitchSpeed = fPitchValue;
// 	editor.manualRoundSpeed = fRoundValue;
}

//-----------------------------------------------------------------------------
// 方法名: PlayCameraKeyFrame()
// 参  数: DWORD camHandle
// 用  途: 播放摄像机关键帧动画
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunPlayCameraKeyFrame(DWORD camHandle)
{
// 	if(camHandle <= 0) return;
// 	CCamera* pCam = (CCamera*)camHandle;
// 	pCam->Play();
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunPauseCameraKeyFrame()
// 参  数: DWORD camHandle
// 用  途: 暂停摄像机关键帧动画
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunPauseCameraKeyFrame(DWORD camHandle)
{
// 	if(camHandle <= 0) return;
// 	CCamera* pCam = (CCamera*)camHandle;
// 	pCam->Pause();
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunStopCameraKeyFrame()
// 参  数: DWORD camHandle
// 用  途: 回退摄像机关键帧动画
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunStopCameraKeyFrame(DWORD camHandle)
{
// 	if(camHandle <= 0) return;
// 	CCamera* pCam = (CCamera*)camHandle;
// 	pCam->Stop();
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGotoCameraFrame()
// 参  数: DWORD camHandle, INT frame
// 用  途: 调到摄像机动画的某一帧
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGotoCameraFrame(DWORD camHandle, INT frame)
{
// 	if(camHandle <= 0) return;
// 	CCamera* pCam = (CCamera*)camHandle;
// 	pCam->GotoFrame(frame);
}

//-------------------------------------------------------------------------------------------
//***灯光相关的方法
//-------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetLightCount()
// 参  数: 
// 用  途: 得到场景中灯光的数量
//-----------------------------------------------------------------------------
UINT CVGS2DLL::SunGetLightCount()
{
	// return vgsScene->light.size();
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetLightNameByHandle()
// 参  数: DWORD lightHandle, CHAR* strLightName
// 用  途: 根据灯光的句柄得到灯光的名称
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetLightNameByHandle(DWORD lightHandle, CHAR* strLightName)
{
// 	if (lightHandle <= 0) return;
// 	CLight* pLight = (CLight*)lightHandle;
// 	strcpy(strLightName, pLight->name.c_str());
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetLightHandleByName()
// 参  数: CHAR* strLightName
// 用  途: 根据灯光的名称得到灯光的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetLightHandleByName(CHAR* strLightName)
{
// 	INT lightCnt = vgsScene->light.size();
// 	CLight** ppLight = vgsScene->light.begin();
// 	
// 	for (INT i = 0; i< lightCnt; i++)
// 	{
// 		CLight* pLight = *(ppLight + i);
// 		if (strcmp(pLight->name.c_str(), strLightName) == 0)
// 			return (DWORD)pLight;
// 	}
	return 0;
}

VOID CVGS2DLL::ResetLightIndex()
{	
// 	INT lightCnt = vgsScene->light.size();
// 	CLight** ppLight = vgsScene->light.begin();
// 	
// 	for (INT i = 0; i< lightCnt; i++)
// 	{
// 		CLight* pLight = *(ppLight + i);
// 		pLight->index = i;
// 	}
}

//得到灯光的编号
INT CVGS2DLL::SunGetLightIndexByHandle(DWORD lightHandle)
{
// 	CLight* pLight = (CLight*)lightHandle;
// 	if (pLight)
// 	{
// 		ResetLightIndex();
// 		return pLight->index;
// 	}
	return -1;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetLightHandleByIndex()
// 参  数: UINT index
// 用  途: CVGS2DLL::SunGetLightHandleByIndex
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetLightHandleByIndex(UINT index)
{	
// 	if (vgsScene == NULL || index < 0) return 0;
// 	
// 	CLight** ppLight = vgsScene->light.begin();
// 	CLight* pLight = *(ppLight + index);
	// 	return (DWORD)pLight;
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunEnableLight()
// 参  数: DWORD lightHandle, BOOL isEnable
// 用  途: 设置灯光变是否有效
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunEnableLight(DWORD lightHandle, BOOL isEnable)
{
// 	if (vgsScene == NULL || lightHandle <= 0) return;
// 	
// 	CLight** ppLight = vgsScene->light.begin();
// 	CLight* pLight = (CLight*)lightHandle;
// 	int index = ppLight - &pLight;
// 	
// 	//set light param
// 	vgsScene->m_pd3dDevice->LightEnable(index, isEnable);
// 	pLight->enable = isEnable;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunMoveLight()
// 参  数: DWORD lightHandle, VECTOR3* pVec
// 用  途: 移动灯光
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunMoveLight(DWORD lightHandle, VECTOR3* pVec)
{
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*
	if (vgsScene == NULL || lightHandle <= 0) return;
	
	CLight** ppLight = vgsScene->light.begin();
	CLight* pLight = (CLight*)lightHandle;
	int index = ppLight - &pLight;
	
	D3DXVECTOR3 moveVec;
	moveVec.x = pVec->x;
	moveVec.y = pVec->y;
	moveVec.z = pVec->z;
	pLight->move(&moveVec);
	//set light param
	vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	*/
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunRotateLight()
// 参  数: DWORD lightHandle, VECTOR3* pRos
// 用  途: 旋转灯光
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunRotateLight(DWORD lightHandle, VECTOR3* pRos)
{
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*
	if (vgsScene == NULL || lightHandle <= 0) return;
	
	CLight** ppLight = vgsScene->light.begin();
	CLight* pLight = (CLight*)lightHandle;
	int index = ppLight - &pLight;
	
	D3DXVECTOR3 rosVec;
	rosVec.x = pRos->x;
	rosVec.y = pRos->y;
	rosVec.z = pRos->z;
	pLight->rotate(&rosVec);
	//set light param
	vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	*/
	//------------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetLightPosition()
// 参  数: DWORD lightHandle, VECTOR3* pPos
// 用  途: 设置灯光的位置
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetLightPosition(DWORD lightHandle, VECTOR3* pPos)
{
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*
	if (vgsScene == NULL || lightHandle <= 0) return;
	
	CLight** ppLight = vgsScene->light.begin();
	CLight* pLight = (CLight*)lightHandle;
	int index = ppLight - &pLight;
	
	pLight->setPosition(&D3DXVECTOR3(pPos->x, pPos->y, pPos->z));
	//set light param
	vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
	*/
	//----------------------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetLightDirection()
// 参  数: DWORD lightHandle, VECTOR3* pDir
// 用  途: 设置灯光的方向
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetLightDirection(DWORD lightHandle, VECTOR3* pDir)
{
// 	if (vgsScene == NULL || lightHandle <= 0) return;
// 	
// 	CLight** ppLight = vgsScene->light.begin();
// 	CLight* pLight = (CLight*)lightHandle;
// 	int index = ppLight - &pLight;
// 	
// 	pLight->setDirection(&D3DXVECTOR3(pDir->x, pDir->y, pDir->z));
// 	//set light param
// 	vgsScene->m_pd3dDevice->SetLight(index, &pLight->param);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetLightInfo()
// 参  数: DWORD lightHandle, LightInfo* pLightInfo
// 用  途: 得到灯光的属性
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetLightInfo(DWORD lightHandle, LightInfo* pLightInfo)
{
// 	if (vgsScene == NULL || lightHandle <= 0) return;
// 	
// 	CLight** ppLight = vgsScene->light.begin();
// 	CLight* pLight = (CLight*)lightHandle;
// 	int index = ppLight - &pLight;
// 	
// 	//type
// 	pLightInfo->Type = pLight->param.Type;
// 	//position
// 	pLightInfo->Position.x = pLight->param.Position.x;
// 	pLightInfo->Position.y = pLight->param.Position.y;
// 	pLightInfo->Position.z = pLight->param.Position.z;
// 	//direction
// 	if (pLight->param.Type != D3DLIGHT_POINT)
// 	{
// 		pLight->param.Direction.x = pLight->pNode->worldRotation.x;
// 		pLight->param.Direction.y = pLight->pNode->worldRotation.y;
// 		pLight->param.Direction.z = pLight->pNode->worldRotation.z ;
// 	}
// 	else
// 	{
// 		pLight->param.Direction.x = 0.0f;
// 		pLight->param.Direction.y = 0.0f;
// 		pLight->param.Direction.z = 0.0f;
// 	}
// 
// 	//diffuse
// 	pLightInfo->Diffuse.red = (BYTE)(pLight->diffuse.r * 255);
// 	pLightInfo->Diffuse.green = (BYTE)(pLight->diffuse.g * 255);
// 	pLightInfo->Diffuse.blue = (BYTE)(pLight->diffuse.b * 255);
// 
// 	//ambient
// 	pLightInfo->Ambient.red = (BYTE)(pLight->ambient.r * 255);
// 	pLightInfo->Ambient.green = (BYTE)(pLight->ambient.g * 255);
// 	pLightInfo->Ambient.blue = (BYTE)(pLight->ambient.b * 255);
// 
// 	//multiply
// 	pLightInfo->Multiply = pLight->multiply;
// 
// 	//range
// 	if (pLight->param.Type != D3DLIGHT_DIRECTIONAL)
// 		pLightInfo->Range = pLight->param.Range;
// 	else
// 		pLightInfo->Range = 0;
// 
// 	//attenuation
// 	pLightInfo->Attenuation0 = pLight->param.Attenuation0;
// 	pLightInfo->Attenuation1 = pLight->param.Attenuation1;
// 	pLightInfo->Attenuation2 = pLight->param.Attenuation2;
// 	
// 	if (pLight->param.Type == D3DLIGHT_SPOT)
// 	{
// 		//fallof
// 		pLightInfo->Falloff = pLight->param.Falloff;
// 		//theta
// 		pLightInfo->Theta = pLight->param.Theta;
// 		//phi
// 		pLightInfo->Phi = pLight->param.Phi;
// 	}
// 	else
// 	{
// 		//fallof
// 		pLightInfo->Falloff = 0.0f;
// 		//theta
// 		pLightInfo->Theta = 0.0f;
// 		//phi
// 		pLightInfo->Phi = 0.0f;
// 	}
	
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetLightInfo()
// 参  数: DWORD lihgtHandle, LightInfo* pLightInfo
// 用  途: 设置灯光的属性
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetLightInfo(DWORD lightHandle, LightInfo* pLightInfo)
{
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*
	if (vgsScene == NULL || lightHandle <= 0) return;
	
	CLight** ppLight = vgsScene->light.begin();
	CLight* pLight = (CLight*)lightHandle;
	int index = ppLight - &pLight;
	
	D3DCOLORVALUE diffuse, ambient;
	switch (pLightInfo->Type)
	{
	//omni
	case 1:
		pLight->param.Type = D3DLIGHT_POINT;
		//position
		pLight->position.x =  pLightInfo->Position.x;
		pLight->position.y =  pLightInfo->Position.y;
		pLight->position.z =  pLightInfo->Position.z;

		pLight->param.Position.x = pLightInfo->Position.x;
		pLight->param.Position.y = pLightInfo->Position.y;
		pLight->param.Position.z = pLightInfo->Position.z;

		//diffuse
		diffuse.r = pLightInfo->Diffuse.red/255.0f;
		diffuse.g = pLightInfo->Diffuse.green/255.0f;
		diffuse.b = pLightInfo->Diffuse.blue/255.0f;
		diffuse.a = 1.0f;
		pLight->diffuse = diffuse;

		//ambient
		ambient.r = pLightInfo->Ambient.red/255.0f;
		ambient.g = pLightInfo->Ambient.green/255.0f;
		ambient.b = pLightInfo->Ambient.blue/255.0f;
		ambient.a = 1.0f;
		pLight->ambient = ambient;

		//multiply
		pLight->multiply = pLightInfo->Multiply;

		pLight->param.Diffuse = pLight->multiply * pLight->diffuse;
		pLight->param.Ambient = pLight->multiply * pLight->ambient;

		//attenuation
		pLight->param.Attenuation0 = pLightInfo->Attenuation0;
		pLight->param.Attenuation1 = pLightInfo->Attenuation1;
		pLight->param.Attenuation2 = pLightInfo->Attenuation2;
		break;
	//spotLight
	case 2:
		pLight->param.Type = D3DLIGHT_SPOT;
		//position
		pLight->param.Position.x = pLightInfo->Position.x;
		pLight->param.Position.y = pLightInfo->Position.y;
		pLight->param.Position.z = pLightInfo->Position.z;
		pLight->position.x = pLight->param.Position.x;
		pLight->position.y = pLight->param.Position.y;
		pLight->position.z = pLight->param.Position.z;
		//direction
		pLight->param.Direction.x = pLightInfo->Direction.x;
		pLight->param.Direction.y = pLightInfo->Direction.y;
		pLight->param.Direction.z = pLightInfo->Direction.z;
		pLight->direction.x = pLight->param.Direction.x;
		pLight->direction.y = pLight->param.Direction.y;
		pLight->direction.z = pLight->param.Direction.z;
		//diffuse
		diffuse.r = pLightInfo->Diffuse.red/255.0f;
		diffuse.g = pLightInfo->Diffuse.green/255.0f;
		diffuse.b = pLightInfo->Diffuse.blue/255.0f;
		diffuse.a = 1.0f;
		pLight->diffuse = diffuse;

		//ambient
		ambient.r = pLightInfo->Ambient.red/255.0f;
		ambient.g = pLightInfo->Ambient.green/255.0f;
		ambient.b = pLightInfo->Ambient.blue/255.0f;
		ambient.a = 1.0f;
		pLight->ambient = ambient;

		//multiply
		pLight->multiply = pLightInfo->Multiply;

		pLight->param.Diffuse = pLight->multiply * pLight->diffuse;
		pLight->param.Ambient = pLight->multiply * pLight->ambient;

		//attenuation
		pLight->param.Attenuation0 = pLightInfo->Attenuation0;
		pLight->param.Attenuation1 = pLightInfo->Attenuation1;
		pLight->param.Attenuation2 = pLightInfo->Attenuation2;
		//range
		pLight->param.Range = pLightInfo->Range;
		//fallof
		pLight->param.Falloff = pLightInfo->Falloff;
		//theta
		pLight->param.Theta = pLightInfo->Theta;
		//phi
		pLight->param.Phi = pLightInfo->Phi;
		break;
	//directional light
	case 3:
		pLight->param.Type = D3DLIGHT_DIRECTIONAL;
		//direction
		pLight->param.Direction.x = pLightInfo->Direction.x;
		pLight->param.Direction.y = pLightInfo->Direction.y;
		pLight->param.Direction.z = pLightInfo->Direction.z;
		pLight->direction.x = pLight->param.Direction.x;
		pLight->direction.y = pLight->param.Direction.y;
		pLight->direction.z = pLight->param.Direction.z;
		//diffuse
		diffuse.r = pLightInfo->Diffuse.red/255.0f;
		diffuse.g = pLightInfo->Diffuse.green/255.0f;
		diffuse.b = pLightInfo->Diffuse.blue/255.0f;
		diffuse.a = 1.0f;
		pLight->diffuse = diffuse;

		//ambient
		ambient.r = pLightInfo->Ambient.red/255.0f;
		ambient.g = pLightInfo->Ambient.green/255.0f;
		ambient.b = pLightInfo->Ambient.blue/255.0f;
		ambient.a = 1.0f;
		pLight->ambient = ambient;

		//multiply
		pLight->multiply = pLightInfo->Multiply;

		pLight->param.Diffuse = pLight->multiply * pLight->diffuse;
		pLight->param.Ambient = pLight->multiply * pLight->ambient;

		//attenuation
		pLight->param.Attenuation0 = pLightInfo->Attenuation0;
		pLight->param.Attenuation1 = pLightInfo->Attenuation1;
		pLight->param.Attenuation2 = pLightInfo->Attenuation2;
		break;
	}
	//set light param
	vgsScene->m_pd3dDevice->SetLight(pLight->index, &pLight->param);
	*/
//----------------------------------------------------------------------------------------------------
}
/** 设置灯光是否有效
*/
VOID CVGS2DLL::SunSetLightEnableStatus(DWORD lightHandle, BOOL bEnable)
{
	m_VGSCore.SetLightEnableStatus(m_vgsScene, lightHandle, bEnable);
}
//设置是否播放灯光的关键帧动画
VOID CVGS2DLL::SetLightKfrPlayStatus(DWORD lightHandle, BOOL bFlag)
{
// 	CLight* pLight = (CLight*)lightHandle;
// 	if (pLight)
// 		pLight->playKfr = bFlag;
}

VOID CVGS2DLL::SetAllLightKfrPlayStatus(BOOL bFlag)
{
// 	INT lightCnt = vgsScene->light.size();
// 	CLight** ppLight = vgsScene->light.begin();
// 	for (INT i = 0; i< lightCnt; i++)
// 	{
// 		CLight* pLight = *(ppLight + i);
// 		pLight->playKfr = bFlag;
// 	}
}

//设置灯光关键帧动画的播放次数
VOID CVGS2DLL::SetLightKfrRepeatCnt(DWORD lightHandle, INT repeatCnt)
{
// 	CLight* pLight = (CLight*)lightHandle;
// 	if (pLight)
// 		pLight->kfrRepeatCnt = repeatCnt;
}

VOID CVGS2DLL::SetAllLightKfrRepeatCnt(INT repeatCnt)
{
// 	INT lightCnt = vgsScene->light.size();
// 	CLight** ppLight = vgsScene->light.begin();
// 	for (INT i = 0; i< lightCnt; i++)
// 	{
// 		CLight* pLight = *(ppLight + i);
// 		pLight->kfrRepeatCnt = repeatCnt;
// 	}
}


//-----------------------------------------------------------------------------
//文字相关方法
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunNewText()
// 参  数: CHAR* strText
// 用  途: 新建文字对象
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunNewText(CHAR* strText)
{
// 	CText* pText = new CText(vgsScene->pDefaultFont);
// 	SAFE_DELETE_ARRAY(pText->strText);
// 	
// 	int len = 0;
// 	while(strText[len] != '\0')
// 		len++;
// 	
// 	if (len == 0) return E_FAIL;
// 	pText->strText = new CHAR[len+1];
// 	strcpy(pText->strText, strText);
// 	pText->strText[len] = '\0';
// 	
// 	vgsScene->text.push_back(pText);
	// 	return (DWORD)pText;
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunDelText()
// 参  数: DWORD textHandle
// 用  途: 删除文字对象
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDelText(DWORD textHandle)
{
// 	if (vgsScene == NULL || textHandle <= 0) return;
// 	
// 	CText* pText = (CText*)textHandle;
// 	INT textCnt = vgsScene->text.size();
// 	CText** ppText = vgsScene->text.begin();
// 	for (INT i = 0; i< textCnt; i++)
// 	{
// 		CText* pTText = *(ppText + i);
// 		if (pTText == pText)
// 		{
// 			vgsScene->text.erase(ppText + i);
// 			SAFE_RELEASE(pText);
// 			SAFE_DELETE(pText);
// 			return;
// 		}
// 	}
	
}

//-----------------------------------------------------------------------------
// 方法名: SetMtrlOpacity
// 参  数: DWORD mtrlHandle, UINT opacity
// 用  途: 设置材质的opacity
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SetMtrlOpacity(DWORD mtrlHandle, UINT opacity)
{	
	MtrlInfo info;
	m_VGSCore.GetMaterialInfo(m_vgsScene, mtrlHandle, &info);
	info.Opacity = opacity;
	m_VGSCore.SetMaterialInfo(m_vgsScene, mtrlHandle, &info);
//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
/*
	if (mtrlHandle <= 0) return;
	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	if (opacity > 100) opacity = 100;
	if (opacity < 0) opacity = 0;
	
	//如果是不透明材质
	if (pMtrl->opacity == 100 || pMtrl->compTex || !pMtrl->bAlphaBlend)
	{
		pMtrl->opacity = opacity;
		INT modelCnt = pMtrl->modelList.size();
		CModel** ppModel = (CModel**)(pMtrl->modelList.begin());
		for (INT i = 0; i< modelCnt; i++)
		{
			CModel* pModel = *(ppModel + i);
			//从不透明模型变成透明模型
			if (pModel->opacity == 100 && opacity < 100)
				vgsScene->transModel.push_back(pModel);
			pModel->SetOpacity(opacity);
		}
	}
	//如果透明材质
	else
	{
		INT transModelCnt = vgsScene->transModel.size();
		CModel** ppModel = vgsScene->transModel.begin();
		for (INT j = 0; j< transModelCnt; j++)
		{
			CModel* pModel = *(ppModel + j);
			if (pModel->shader == pMtrl)
			{
				pMtrl->opacity = opacity;
				pModel->SetOpacity(opacity);
				//如果变成不透明模型
				if (opacity == 100)
					vgsScene->transModel.erase(ppModel + j);
			}
		}
	}
	*/
	//---------------------------------------------------------------------------------
}


//设置材质的颜色
VOID CVGS2DLL::SunSetMtrlColor(DWORD mtrlHandle, INT red, INT green, INT blue, INT alpha)
{
	m_VGSCore.SetMaterialColor(m_vgsScene, mtrlHandle, red, green, blue, alpha);
	//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
	/*
	CMaterial* pMtrl = (CMaterial*)mtrlHandle;
	if (pMtrl)
	{
		pMtrl->d3dmtrl.Diffuse.r = red/255.0f;
		pMtrl->d3dmtrl.Diffuse.g = green/255.0f;
		pMtrl->d3dmtrl.Diffuse.b = blue/255.0f;
		pMtrl->d3dmtrl.Diffuse.a = alpha/255.0f;
		pMtrl->d3dmtrl.Ambient = pMtrl->d3dmtrl.Diffuse;
		pMtrl->d3dmtrl.Specular = pMtrl->d3dmtrl.Diffuse;

		//如果是不透明材质
		if (pMtrl->opacity == 100)
		{
			CModel** ppModel = (CModel**)(pMtrl->modelList.begin());
			INT modelCnt = pMtrl->modelList.size();
			for (INT i = 0; i< modelCnt; i++)
			{
				pMtrl->opacity = alpha/255.0f * 100;
				CModel* pModel = *(ppModel + i);
				pModel->SetMtrl(pMtrl);
				//如果模型为不透明模型，将材质设置为透明材质，则将模型加入到透明模型队列
				if (pMtrl->opacity < 100)
				{
					if (pModel->opacity == 100)
						vgsScene->transModel.push_back(pModel);
					//从不透明模型队列清空
					pMtrl->modelList.clear();
				}

			}
		}
		//如果是透明材质
		else
		{
			pMtrl->opacity = alpha/255.0f * 100;
			for (INT j = 0; j < vgsScene->transModel.size(); j++)
			{
				CModel** ppModel = vgsScene->transModel.begin();
				CModel* pModel = *(ppModel + j);
				if (pModel->shader == pMtrl)
				{
					pModel->SetMtrl(pMtrl);
					//如果将材质设置为不透明材质，则将模型从透明模型队列删除
					if (pMtrl->opacity == 100)
					{
						vgsScene->transModel.erase(ppModel + j);
						//pModel->shader->modelList.push_back((DWORD)pModel);
						if (j > 0) j--;
					}
				}
			}
		}
	}*/
	//---------------------------------------------------------------------------------------
}

//-------------------------------------------------------------------------------------------
//***贴图相关的方法
//-------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetTextCount()
// 参  数: 
// 用  途: 得到场景中文字对象的数量
//-----------------------------------------------------------------------------
UINT CVGS2DLL::SunGetTextCount()
{
	// return vgsScene->text.size();
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetTextHandleByIndex()
// 参  数: UINT index
// 用  途: 通过文字对象的编号得到文字对象的句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetTextHandleByIndex(UINT index)
{
// 	if (vgsScene == NULL || index < 0) return 0;
// 	
// 	CText** ppText = vgsScene->text.begin();
// 	CText* pText = *(ppText + index);
	// 	return (DWORD)pText;
	return 0;
}
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetTextColor()
// 参  数: DWORD textHandle, DWORD color
// 用  途: 设置文字对象的颜色
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetTextColor(DWORD textHandle, DWORD color)
{
// 	if (textHandle <= 0) return;
// 	CText* pText = (CText*)textHandle;	
	// 	pText->color = color;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetTextPosition()
// 参  数: DWORD textHandle, UINT x, UINT y
// 用  途: 设置文字对象的位置
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetTextPosition(DWORD textHandle, UINT x, UINT y)
{
// 	if (textHandle <= 0) return;
// 	CText* pText = (CText*)textHandle;
// 	if (pText && pText->m_pFont)
// 	{
// 		pText->rect.left = x;
// 		pText->rect.top = y;
// 		pText->rect.right = pText->rect.left + pText->m_pFont->GetHeight() * 32;
// 		pText->rect.bottom = pText->rect.top + pText->m_pFont->GetHeight();
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetTextString()
// 参  数: DWORD textHandle, CHAR* strText
// 用  途: 设置文字的内容
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetTextString(DWORD textHandle, CHAR* strText)
{
// 	if (textHandle == 0 || !strText) return;
// 	CText* pText = (CText*)textHandle;
// 	if (pText)
// 	{
// 		pText->strText = strText;
// 	}
}



//-------------------------------------------------------------------------------------------
//***Overlay相关的方法
//-------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: SunNewOverlay()
// 参  数: CHAR* name, CHAR* strOriImageFile, CHAR* strAlphaImageFile, INT left, INT top, INT width, INT height
// 用  途: 从本地文件新建 Overlay
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunNewOverlay(CHAR* name, CHAR* strOriImageFile, CHAR* strAlphaImageFile, 
							  ALIGNMODE alignMode, RECT* pMapRect)
{
	//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
	/*
	if (vgsScene == NULL) return 0;
	
	COverlay* pOverlay = new COverlay(vgsScene->m_pd3dDevice);
	if (pOverlay == NULL) return NULL;
	
	strcpy(pOverlay->name, name);
	pOverlay->alignMode = alignMode;
	
	//对齐模式
	RECT mapRect;
	INT mapWidth = pMapRect->right - pMapRect->left;
	INT mapHeight = pMapRect->bottom - pMapRect->top;
	
	switch (alignMode)
	{
	case ALIGN_NONE:
		mapRect = *pMapRect;
		break;
	case ALIGN_LEFT:
		mapRect = *pMapRect;
		mapRect.left = vgsScene->rect.left;
		mapRect.right = mapRect.left + (pMapRect->right - pMapRect->left);
		break;
	case ALIGN_RIGHT:
		mapRect = *pMapRect;
		mapRect.right = vgsScene->rect.right;
		mapRect.left = mapRect.right - (pMapRect->right - pMapRect->left);
		break;
	case ALIGN_TOP:
		mapRect = *pMapRect;
		mapRect.top = vgsScene->rect.top;
		mapRect.bottom = mapRect.top + (pMapRect->bottom - pMapRect->top);
		break;
	case ALIGN_BOTTOM:
		mapRect = *pMapRect;
		mapRect.bottom = vgsScene->rect.bottom;
		mapRect.top = mapRect.bottom - (pMapRect->bottom - pMapRect->top);
		break;
	case ALIGN_LEFT_TOP:
		mapRect.left = vgsScene->rect.left;
		mapRect.top = vgsScene->rect.top;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_MIDDLE_TOP:
		mapRect.left = (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
		mapRect.top = vgsScene->rect.top;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_RIGHT_TOP:
		mapRect.left = vgsScene->rect.right - mapWidth;
		mapRect.top = vgsScene->rect.top;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_LEFT_BOTTOM:
		mapRect.left = vgsScene->rect.left;
		mapRect.top = vgsScene->rect.bottom - mapHeight;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_MIDDLE_BOTTOM:
		mapRect.left = (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
		mapRect.top = vgsScene->rect.bottom - mapHeight;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_RIGHT_BOTTOM:
		mapRect.left = vgsScene->rect.right - mapWidth;
		mapRect.top = vgsScene->rect.bottom - mapHeight;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_CENTER:
		mapRect.left =  (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
		mapRect.top = (vgsScene->rect.top + vgsScene->rect.bottom)/2 - mapHeight/2;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	}
	
	pOverlay->SetRect(&mapRect);
	
	CMaterial* pMtrl = new CMaterial();
	int tIndex = vgsScene->shader.size();
	strcpy(pMtrl->name, "OverlayMtrl_");
	CHAR num[8];
	itoa(tIndex, num, 10);
	strcat(pMtrl->name, num);
	
	pMtrl->m_FVF = pOverlay->m_FVF;
	pMtrl->m_FVFSize = pOverlay->m_FVFSize;
	
	//根据strImageFile创建贴图
	if (strOriImageFile != NULL)
	{
		pMtrl->diffuseTex = new CTexture();
		pMtrl->diffuseTex->BP = D3DFMT_A8R8G8B8;
		if (FAILED( vgsScene->CreateTexFromFile(strOriImageFile, pMtrl->diffuseTex, 1)))
		{
			SAFE_DELETE(pMtrl->diffuseTex);
			return NULL;
		}
	}
	
	if (strAlphaImageFile != NULL)
	{
		pMtrl->opacityTex = new CTexture();
		pMtrl->opacityTex->BP = D3DFMT_A8R8G8B8;
		if (FAILED( vgsScene->CreateTexFromFile(strAlphaImageFile, pMtrl->opacityTex, 1)))
		{
			SAFE_DELETE(pMtrl->opacityTex);
			return NULL;
		}
		pMtrl->compTex = new CTexture();
		pMtrl->compTex->BP = D3DFMT_A8R8G8B8;
		pMtrl->compTex->width = pMtrl->diffuseTex->width;
		pMtrl->compTex->height = pMtrl->diffuseTex->height;
		if (pMtrl->compTex->width > 4048 || pMtrl->compTex->height > 4048) return NULL;
		
		UINT bufsize = pMtrl->diffuseTex->width * pMtrl->diffuseTex->height * 4;
		pMtrl->compTex->buffer = new BYTE[bufsize];
		if (FAILED( vgsScene->CompositeAlphaMtrl(pMtrl, 1)))
		{
			SAFE_DELETE(pMtrl->compTex->buffer);
			SAFE_DELETE(pMtrl->opacityTex);
			return NULL;
		}
		
	}
	
	pOverlay->SetMaterial(pMtrl);
	//push into overlay list
	vgsScene->overlay.push_back(pOverlay);
	
	return (DWORD)pOverlay;
	*/
    return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SunNewOverlayFromBuf()
// 参  数: CHAR* name, BYTE* pOriBuf, DWORD oriDataSize, BYTE* pAlphaBuf, DWORD alphaDataSize, RECT* pRect
// 用  途: 通过Buffer新建Overlay
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunNewOverlayFromBuf(CHAR* name, BYTE* pOriBuf, DWORD oriDataSize, BYTE* pAlphaBuf, DWORD alphaDataSize, ALIGNMODE alignMode, RECT* pMapRect)
{
//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
	/*
	COverlay* pOverlay = new COverlay(vgsScene->m_pd3dDevice);
	if (pOverlay == NULL) return NULL;
	
	strcpy(pOverlay->name, name);
	pOverlay->index = vgsScene->overlay.size();
	pOverlay->alignMode = alignMode;
	
	//对齐模式
	RECT mapRect;
	INT mapWidth = pMapRect->right - pMapRect->left;
	INT mapHeight = pMapRect->bottom - pMapRect->top;
	pOverlay->m_imageWidth = mapWidth;
	pOverlay->m_imageHeight = mapHeight;
	pOverlay->useRelativePos = FALSE;
	
	switch (alignMode)
	{
	case ALIGN_NONE:
		mapRect = *pMapRect;
		break;
	case ALIGN_LEFT:
		mapRect = *pMapRect;
		mapRect.left = vgsScene->rect.left;
		mapRect.right = mapRect.left + (pMapRect->right - pMapRect->left);
		break;
	case ALIGN_RIGHT:
		mapRect = *pMapRect;
		mapRect.right = vgsScene->rect.right;
		mapRect.left = mapRect.right - (pMapRect->right - pMapRect->left);
		break;
	case ALIGN_TOP:
		mapRect = *pMapRect;
		mapRect.top = vgsScene->rect.top;
		mapRect.bottom = mapRect.top + (pMapRect->bottom - pMapRect->top);
		break;
	case ALIGN_BOTTOM:
		mapRect = *pMapRect;
		mapRect.bottom = vgsScene->rect.bottom;
		mapRect.top = mapRect.bottom - (pMapRect->bottom - pMapRect->top);
		break;
	case ALIGN_LEFT_TOP:
		mapRect.left = vgsScene->rect.left;
		mapRect.top = vgsScene->rect.top;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_MIDDLE_TOP:
		mapRect.left = (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
		mapRect.top = vgsScene->rect.top;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_RIGHT_TOP:
		mapRect.left = vgsScene->rect.right - mapWidth;
		mapRect.top = vgsScene->rect.top;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_LEFT_BOTTOM:
		mapRect.left = vgsScene->rect.left;
		mapRect.top = vgsScene->rect.bottom - mapHeight;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_MIDDLE_BOTTOM:
		mapRect.left = (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
		mapRect.top = vgsScene->rect.bottom - mapHeight;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_RIGHT_BOTTOM:
		mapRect.left = vgsScene->rect.right - mapWidth;
		mapRect.top = vgsScene->rect.bottom - mapHeight;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	case ALIGN_CENTER:
		mapRect.left =  (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
		mapRect.top = (vgsScene->rect.top + vgsScene->rect.bottom)/2 - mapHeight/2;
		mapRect.right = mapRect.left + mapWidth;
		mapRect.bottom = mapRect.top + mapHeight;
		break;
	}
	
	mapRect.left += pMapRect->left;
	mapRect.top += pMapRect->top;
	mapRect.right = mapRect.left + mapWidth;
	mapRect.bottom = mapRect.top + mapHeight;
	
	pOverlay->SetRect(&mapRect);
	
	CMaterial* pMtrl = new CMaterial();
	int tIndex = vgsScene->shader.size();
	strcpy(pMtrl->name, "OverlayMtrl_");
	pMtrl->index = vgsScene->shader.size();
	pMtrl->bIsOverlayMtrl = TRUE;
	
	CHAR num[8];
	itoa(tIndex, num, 10);
	strcat(pMtrl->name, num);
	
	pMtrl->m_FVF = pOverlay->m_FVF;
	pMtrl->m_FVFSize = pOverlay->m_FVFSize;
	
	if (pOriBuf != NULL)
	{
		pMtrl->diffuseTex = new CTexture();
		pMtrl->diffuseTex->index = vgsScene->texture.size();
		pMtrl->diffuseTexIndex = pMtrl->diffuseTex->index;
		pMtrl->diffuseTex->bIsOverlayTexture = TRUE;
		//拷贝数据
		pMtrl->diffuseTex->dataSize = oriDataSize;
		pMtrl->diffuseTex->data = new BYTE[oriDataSize];
		memcpy(pMtrl->diffuseTex->data, pOriBuf, oriDataSize);
		pMtrl->diffuseTex->BP = D3DFMT_A8R8G8B8;
		if (FAILED( vgsScene->CreateTexFromBuffer(pMtrl->diffuseTex, 1)))
		{
			SAFE_DELETE(pMtrl->diffuseTex->data);
			SAFE_DELETE(pMtrl->diffuseTex);
			return NULL;
		}

		if (pAlphaBuf == NULL)
		{
			vgsScene->texture.push_back(pMtrl->diffuseTex);
			SAFE_DELETE(pMtrl->diffuseTex->data);
		}
	}
	
	if (pAlphaBuf != NULL)
	{
		pMtrl->opacityTex = new CTexture();
		pMtrl->opacityTex->index = vgsScene->texture.size();
		pMtrl->opacityTex->bIsOverlayTexture = TRUE;
		pMtrl->opacityTex->dataSize = alphaDataSize;
		pMtrl->opacityTex->data = new BYTE[alphaDataSize];
		memcpy(pMtrl->opacityTex->data, pAlphaBuf, oriDataSize);
		pMtrl->opacityTex->BP = D3DFMT_A8R8G8B8;
		if (FAILED( vgsScene->CreateTexFromBuffer(pMtrl->opacityTex, 1)))
		{
			SAFE_DELETE(pMtrl->diffuseTex->data);
			SAFE_DELETE(pMtrl->diffuseTex);
			
			SAFE_DELETE(pMtrl->opacityTex->data);
			SAFE_DELETE(pMtrl->opacityTex);
			
			SAFE_DELETE(pMtrl);
			return NULL;
		}
		
		pMtrl->compTex = new CTexture();
		pMtrl->compTex->index = vgsScene->texture.size();
		pMtrl->compTex->bIsOverlayTexture = TRUE;
		pMtrl->compTex->BP = D3DFMT_A8R8G8B8;
		pMtrl->compTex->width = pMtrl->diffuseTex->width;
		pMtrl->compTex->height = pMtrl->diffuseTex->height;
		if (pMtrl->compTex->width > 4048 || pMtrl->compTex->height > 4048) return NULL;
		
		UINT bufsize = pMtrl->diffuseTex->width * pMtrl->diffuseTex->height * 4;
		pMtrl->compTex->buffer = new BYTE[bufsize];
		if (FAILED( vgsScene->CompositeAlphaMtrl(pMtrl, 1)))
		{
			SAFE_DELETE(pMtrl->diffuseTex->data);
			SAFE_DELETE(pMtrl->diffuseTex);
			
			SAFE_DELETE(pMtrl->opacityTex->data);
			SAFE_DELETE(pMtrl->opacityTex);
			
			SAFE_DELETE(pMtrl->compTex->buffer);
			SAFE_DELETE(pMtrl->opacityTex);
			
			SAFE_DELETE(pMtrl);
			return NULL;
		}
		vgsScene->texture.push_back(pMtrl->compTex);
		
		SunDeleteTexture((DWORD)pMtrl->diffuseTex);
		SunDeleteTexture((DWORD)pMtrl->opacityTex);
	}
	pOverlay->SetMaterial(pMtrl);
	//push into shader list
	vgsScene->shader.push_back(pMtrl);
	//push into overlay list
	vgsScene->overlay.push_back(pOverlay);
	
	return (DWORD)pOverlay;
	*/
return 0;

}

//-----------------------------------------------------------------------------
// 方法名: SunNewOverlayFromImage()
// 参  数: CHAR* overlayName, CHAR* pOriImageName, CHAR* pAlphaImageName, 
//         INT left, INT top, INT width, INT height
// 用  途: 从Image对象新建Overlay
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunNewOverlayFromImage(CHAR* overlayName, CHAR* pOriImageName, CHAR* pAlphaImageName, 
									   ALIGNMODE alignMode, RECT* pMapRect)
{
	//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
/*	DWORD overlayHandle;
	DWORD oriImageHandle = SunGetImageHandleByName(pOriImageName);
	DWORD alphaImageHandle = SunGetImageHandleByName(pAlphaImageName);
	
	CImage* pOriImage = (CImage*)oriImageHandle;
	if(pOriImage == NULL) 
		return 0;
	
	CImage* pAlphaImage = (CImage*)alphaImageHandle;
	BYTE* pAlphaData = NULL;
	DWORD alphaDataSize = 0;
	if (pAlphaImage != NULL)
	{
		pAlphaData = pAlphaImage->data;
		alphaDataSize = pAlphaImage->dataLength;
	}
	overlayHandle= SunNewOverlayFromBuf(overlayName, pOriImage->data, pOriImage->dataLength,
		pAlphaData, alphaDataSize, alignMode, pMapRect);
	
	return overlayHandle;*/
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SunDelOverlay()
// 参  数: DWORD overlayHandle
// 用  途: 删除 Overlay
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDelOverlay(DWORD overlayHandle)
{	
	//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
	/*
	if (overlayHandle <= 0) return;
	
	COverlay* pOverlay = (COverlay*)overlayHandle;
	
	//删除Overlay
	INT overlayCnt = vgsScene->overlay.size();
	COverlay** ppOverlay = vgsScene->overlay.begin();
	
	for (INT i = 0; i< overlayCnt; i++)
	{
		COverlay* pTOverlay = *(ppOverlay + i);
		if (pTOverlay == pOverlay)
		{	
			CMaterial* pMtrl = pOverlay->m_pMtrl;	
			
			if (pMtrl != NULL)
			{
				//释放材质用到的贴图对象
				if (pMtrl->diffuseTex)
					SunDeleteTexture((DWORD)pMtrl->diffuseTex);
				if (pMtrl->opacityTex)
					SunDeleteTexture((DWORD)pMtrl->opacityTex);
				if (pMtrl->compTex)
					SunDeleteTexture((DWORD)pMtrl->compTex);
				if (pMtrl->lightTex)
					SunDeleteTexture((DWORD)pMtrl->lightTex);
				if (pMtrl->reflectionTex)
					SunDeleteTexture((DWORD)pMtrl->reflectionTex);
				if (pMtrl->bumpTex)
					SunDeleteTexture((DWORD)pMtrl->bumpTex);
				if (pMtrl->specularTex)
					SunDeleteTexture((DWORD)pMtrl->specularTex);
				
				pMtrl->diffuseTex = NULL;
				pMtrl->opacityTex = NULL;
				pMtrl->compTex = NULL;
				pMtrl->lightTex = NULL;
				pMtrl->reflectionTex = NULL;
				pMtrl->bumpTex = NULL;
				pMtrl->specularTex = NULL;
				
				//从渲染队列中找到Overlay的材质，删除它
				SunDeleteMtrl((DWORD)pMtrl);
			}
			
			//删除Overlay 
			vgsScene->overlay.erase(ppOverlay + i);
			SAFE_RELEASE(pTOverlay);
			SAFE_DELETE(pTOverlay);
			
			break;
		}
	}
	
	return;
	*/
	
}

//得到Ovelay的句柄
DWORD CVGS2DLL::SunGetOverlayHandleByName(CHAR* strName)
{
// 	if (vgsScene == NULL || strName == NULL) return 0;
// 	
// 	UINT overlayCnt = vgsScene->overlay.size();
// 	COverlay** ppOverlay = vgsScene->overlay.begin();
// 	for (UINT i =0; i< overlayCnt; i++)
// 	{
// 		COverlay* pOverlay = *(ppOverlay + i);
// 		if (strcmp(pOverlay->name, strName) == 0)
// 			return (DWORD)pOverlay;
// 	}
	return 0;
}

//得到Overlay的名字
VOID CVGS2DLL::SunGetOverlayName(DWORD overlayHandle, CHAR* strName)
{
// 	if (vgsScene == NULL || overlayHandle <= 0) return;
// 	
// 	COverlay* pOverlay = (COverlay*)overlayHandle;
// 	strcpy(strName, pOverlay->name);
}

//得到Overlay的编号
DWORD CVGS2DLL::SunGetOverlayHandleByIndex(INT index)
{
// 	if (vgsScene == NULL || index <= 0) return 0;
// 	
// 	COverlay** ppOverlay = vgsScene->overlay.begin();
// 	COverlay* pOverlay = *(ppOverlay + index);
	// 	return (DWORD)pOverlay;
	return 0;
}

//得到Overlay的index
INT CVGS2DLL::SunGetOverlayIndex(DWORD overlayHandle)
{
// 	if (vgsScene == NULL || overlayHandle <= 0) return -1;
// 	
// 	COverlay** ppOverlay = vgsScene->overlay.begin();
// 	COverlay* pOverlay = (COverlay*)overlayHandle;
// 	int index = ppOverlay - &pOverlay;
	// 	return index;
	return 0;
}

//得到 Overlay 的文件名
VOID CVGS2DLL::SunGetOverlayImage(DWORD overlayHandle, CHAR* strImageFile)
{
	
}

//设置 Overlay 的文件名
VOID CVGS2DLL::SunSetOverlayImage(DWORD overlayHandle, CHAR* strImageFile)
{
	
}

//得到 Overlay 的Rect
VOID CVGS2DLL::SunGetOverlayRect(DWORD overlayHandle, RECT* rect)
{
// 	if (overlayHandle <= 0) return;
// 	COverlay* pOverlay = (COverlay*)overlayHandle;
// 	pOverlay->GetRect(rect);
}

//设置 Overlay 的Rect
VOID CVGS2DLL::SunSetOverlayRect(DWORD overlayHandle, RECT* rect)
{
// 	if (overlayHandle <= 0) return;
// 	COverlay* pOverlay = (COverlay*)overlayHandle;
// 	pOverlay->SetRect(rect);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetOverlayOpacity()
// 参  数: DWORD overlayHandle
// 用  途: 得到 Overlay 的透明度
//-----------------------------------------------------------------------------
UINT CVGS2DLL::SunGetOverlayOpacity(DWORD overlayHandle)
{
// 	if (overlayHandle <= 0) return 0;
// 	COverlay* pOverlay = (COverlay*)overlayHandle;
	// 	return pOverlay->GetOpacity();
	return m_VGSCore.GetGUIObjectOpacity(m_vgsScene, overlayHandle);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetOverlayOpacity()
// 参  数: DWORD overlayHandle, UINT opacity
// 用  途: 设置 Overlay 的透明度
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetOverlayOpacity(DWORD overlayHandle, UINT opacity)
{
	m_VGSCore.SetGUIObjectOpacity(m_vgsScene, overlayHandle, opacity);
// 	if (overlayHandle <= 0) return;
// 	COverlay* pOverlay = (COverlay*)overlayHandle;
// 	pOverlay->SetOpacity(opacity);
}

//------------------------------------------------------------
//SunGetOverlayFormat
//------------------------------------------------------------
VOID CVGS2DLL::SunGetOverlayFormat(DWORD overlayHandle, UINT enumStatus)
{
	
}

//------------------------------------------------------------
//SunSetOverlayFormat
//------------------------------------------------------------
UINT CVGS2DLL::SunSetOverlayFormat(DWORD overlayHandle)
{
	
	return NULL;
}

//------------------------------------------------------------
//SunMoveOverlay
//------------------------------------------------------------
VOID CVGS2DLL::SunMoveOverlay(DWORD overlayHandle, INT x, INT y)
{
	
}

//------------------------------------------------------------
//SunRotateOverlay
//------------------------------------------------------------
VOID CVGS2DLL::SunRotateOverlay(DWORD overlayHandle, FLOAT angle)
{
	
}

//------------------------------------------------------------
//SunGetOverlayRotation
//------------------------------------------------------------
FLOAT CVGS2DLL::SunGetOverlayRotation(DWORD overlayHandle)
{
	
	return NULL;
}

//------------------------------------------------------------
//SunSetOverlayRotation
//------------------------------------------------------------
VOID CVGS2DLL::SunSetOverlayRotation(DWORD overlayHandle, FLOAT angle)
{
	
}

//------------------------------------------------------------
//得到 鼠标点击的Overlay
//------------------------------------------------------------
DWORD CVGS2DLL::SunGetOverlayUnderLoc(INT x, INT y)
{

// 	RECT tRect;
// 	
// 	INT bp_dif = 0;
// 	INT bufSize = 0;
// 	D3DLOCKED_RECT DifLockRect;
// 	RECT DifRect;
// 	
// 	INT overlayCnt = vgsScene->overlay.size();
// 	COverlay** ppOverlay = vgsScene->overlay.begin();
// 	
// 	for (INT i = 0; i< overlayCnt; i++)
// 	{
// 		//从最上面开始遍历
// 		COverlay* pOverlay = *(ppOverlay + overlayCnt - 1 - i);
// 		if (pOverlay->isBackdrop)
// 			continue;
// 
// 		if (!pOverlay->m_bVisible)
// 			continue;
// 
// 		pOverlay->GetRect(&tRect);
// 		if (pOverlay != NULL)
// 		{
// 			//先看看鼠标是否点击到了Overlay的Rect内部
// 			if (x > tRect.left && x < tRect.right && y > tRect.top && y < tRect.bottom)
// 			{
// 				//再确定透明区域的问题，找到x,y位置对应的贴图的某个像素
// 				//先找到光标在Overlay中的相对位置
// 				INT ox = x - tRect.left;
// 				INT oy = y - tRect.top;
// 				//找到其贴图UV的相对位置
// 				FLOAT su = (FLOAT)ox/(tRect.right - tRect.left);
// 				FLOAT sv = (FLOAT)oy/(tRect.bottom - tRect.top);
// 				//找到对应的像素
// 				INT px, py;
// 				if (pOverlay->m_pMtrl != NULL)
// 				{
// 					UINT texLayerCnt =pOverlay->m_pMtrl->texLayersList.size();
// 					TextureLayer** ppTexLayer =pOverlay->m_pMtrl->texLayersList.begin();
// 					for(UINT m =0; m < texLayerCnt; m++)
// 					{
// 						TextureLayer* pTexLayer = *(ppTexLayer + m);
// 						if(pTexLayer->type ==TLT_OPACITY)
// 						{
// 							CTexture* pTex = (CTexture*)pTexLayer->pTexture;
// 							px = (INT)(su * pTex->width);
// 							py = (INT)(sv * pTex->height);
// 							bufSize = pTex->width * pTex->height;
// 							
// 							DifRect.left = DifRect.top = 0;
// 							DifRect.right = pTex->width;
// 							DifRect.bottom = pTex->height;
// 							
// 							if (FAILED((pTex->textureBuf)->LockRect(0, &DifLockRect, &DifRect, D3DLOCK_DISCARD)))
// 							{
// 								pTex->textureBuf->UnlockRect(0);
// 								continue;
// 							}
// 							DWORD pixPos = pTex->width * py + px;
// 							BYTE poxAlpha = (((BYTE*)(DifLockRect.pBits))[pixPos]);
// 							pTex->textureBuf->UnlockRect(0);
// 							
// 							if (poxAlpha > 16)
// 								return (DWORD)pOverlay;
// 							else
// 								continue;
// 							
// 						}
// 						else
// 							return (DWORD)pOverlay;
// 						
// 					}
// 
// 					/*if (pOverlay->m_pMtrl->compTex != NULL)
// 					{
// 						px = (INT)(su * pOverlay->m_pMtrl->compTex->width);
// 						py = (INT)(sv * pOverlay->m_pMtrl->compTex->height);
// 						
// 						//看看对应的像素是否是全透明的
// 						bp_dif = 4;
// 						bufSize = pOverlay->m_pMtrl->compTex->width * pOverlay->m_pMtrl->compTex->height * bp_dif;
// 						
// 						DifRect.left = DifRect.top = 0;
// 						DifRect.right = pOverlay->m_pMtrl->compTex->width;
// 						DifRect.bottom = pOverlay->m_pMtrl->compTex->height;
// 						if (FAILED((pOverlay->m_pMtrl->compTex->textureBuf)->LockRect(0, &DifLockRect, &DifRect, D3DLOCK_DISCARD)))
// 						{
// 							pOverlay->m_pMtrl->compTex->textureBuf->UnlockRect(0);
// 							continue;
// 						}
// 						
// 						DWORD pixPos = pOverlay->m_pMtrl->compTex->width * py + px;
// 						BYTE poxAlpha = (((BYTE*)(DifLockRect.pBits))[pixPos * 4 +3] & 0xf0);
// 						pOverlay->m_pMtrl->compTex->textureBuf->UnlockRect(0);
// 
// 						if (poxAlpha > 16)
// 							return (DWORD)pOverlay;
// 						else
// 							continue;
// 					}
// 					else
// 						return (DWORD)pOverlay;*/
// 				}
// 			}
// 		}
// 	}
	return 0;
}

//-----------------------------------------------
//设置Overlay的对齐模式
//-----------------------------------------------
VOID CVGS2DLL::SetOverlayAlignMode(DWORD overlayHandle, INT alignMode)
{
// 	if (overlayHandle <= 0) return;
// 	COverlay* pOverlay = (COverlay*)overlayHandle;
// 	
// 	//当前rect
// 	RECT rect;
// 	SunGetOverlayRect(overlayHandle, &rect);
// 	
// 	//对齐模式
// 	RECT mapRect;
// 	INT mapWidth = rect.right - rect.left;
// 	INT mapHeight = rect.bottom - rect.top;
// 	
// 	switch (alignMode)
// 	{
// 	case ALIGN_NONE:
// 		mapRect = rect;
// 		break;
// 	case ALIGN_LEFT:
// 		mapRect = rect;
// 		mapRect.left = vgsScene->rect.left;
// 		mapRect.right = mapRect.left + (rect.right - rect.left);
// 		break;
// 	case ALIGN_RIGHT:
// 		mapRect = rect;
// 		mapRect.right = vgsScene->rect.right;
// 		mapRect.left = mapRect.right - (rect.right - rect.left);
// 		break;
// 	case ALIGN_TOP:
// 		mapRect = rect;
// 		mapRect.top = vgsScene->rect.top;
// 		mapRect.bottom = mapRect.top + (rect.bottom - rect.top);
// 		break;
// 	case ALIGN_BOTTOM:
// 		mapRect = rect;
// 		mapRect.bottom = vgsScene->rect.bottom;
// 		mapRect.top = mapRect.bottom - (rect.bottom - rect.top);
// 		break;
// 	case ALIGN_LEFT_TOP:
// 		mapRect.left = vgsScene->rect.left;
// 		mapRect.top = vgsScene->rect.top;
// 		mapRect.right = mapRect.left + mapWidth;
// 		mapRect.bottom = mapRect.top + mapHeight;
// 		break;
// 	case ALIGN_MIDDLE_TOP:
// 		mapRect.left = (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
// 		mapRect.top = vgsScene->rect.top;
// 		mapRect.right = mapRect.left + mapWidth;
// 		mapRect.bottom = mapRect.top + mapHeight;
// 		break;
// 	case ALIGN_RIGHT_TOP:
// 		mapRect.left = vgsScene->rect.right - mapWidth;
// 		mapRect.top = vgsScene->rect.top;
// 		mapRect.right = mapRect.left + mapWidth;
// 		mapRect.bottom = mapRect.top + mapHeight;
// 		break;
// 	case ALIGN_LEFT_BOTTOM:
// 		mapRect.left = vgsScene->rect.left;
// 		mapRect.top = vgsScene->rect.bottom - mapHeight;
// 		mapRect.right = mapRect.left + mapWidth;
// 		mapRect.bottom = mapRect.top + mapHeight;
// 		break;
// 	case ALIGN_MIDDLE_BOTTOM:
// 		mapRect.left = (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
// 		mapRect.top = vgsScene->rect.bottom - mapHeight;
// 		mapRect.right = mapRect.left + mapWidth;
// 		mapRect.bottom = mapRect.top + mapHeight;
// 		break;
// 	case ALIGN_RIGHT_BOTTOM:
// 		mapRect.left = vgsScene->rect.right - mapWidth;
// 		mapRect.top = vgsScene->rect.bottom - mapHeight;
// 		mapRect.right = mapRect.left + mapWidth;
// 		mapRect.bottom = mapRect.top + mapHeight;
// 		break;
// 	case ALIGN_CENTER:
// 		mapRect.left =  (vgsScene->rect.left + vgsScene->rect.right)/2 - mapWidth/2;
// 		mapRect.top = (vgsScene->rect.top + vgsScene->rect.bottom)/2 - mapHeight/2;
// 		mapRect.right = mapRect.left + mapWidth;
// 		mapRect.bottom = mapRect.top + mapHeight;
// 		break;
// 	}
// 	
// 	mapRect.left += rect.left;
// 	mapRect.top += rect.top;
// 	mapRect.right = mapRect.left + mapWidth;
// 	mapRect.bottom = mapRect.top + mapHeight;
// 	
// 	pOverlay->SetRect(&mapRect);
}


//将Overlay移到下一层[Overlay有前后关系，后面的会被前面的挡住]
VOID CVGS2DLL::PushOverlayBack(DWORD overlayHandle)
{
// 	if (vgsScene == NULL) return;
// 	
// 	COverlay* sOverlay = (COverlay*)overlayHandle;
// 	if (sOverlay)
// 	{
// 		//下移一层则和前一个Overlay对调位置即可
// 		COverlay** ppOverlay = vgsScene->overlay.begin();
// 		INT overlayCnt = vgsScene->overlay.size();
// 		for (INT i = 0; i< overlayCnt; i++)
// 		{
// 			COverlay* pOverlay = *(ppOverlay + i);
// 			if (pOverlay == sOverlay)
// 			{
// 				//如果已经是最后一层
// 				if (i == 0) return;
// 				
// 				COverlay* preOverlay = *(ppOverlay + i - 1);
// 				vgsScene->overlay[i] = preOverlay;
// 				vgsScene->overlay[i -1] = pOverlay;
// 
// 				return;
// 			}
// 		}
// 	}
}

//将Overlay上移一层
VOID CVGS2DLL::PushOverlayFront(DWORD overlayHandle)
{
// 	if (vgsScene == NULL) return;
// 	
// 	COverlay* sOverlay = (COverlay*)overlayHandle;
// 	if (sOverlay)
// 	{
// 		//下移一层则和后一个Overlay对调位置即可
// 		COverlay** ppOverlay = vgsScene->overlay.begin();
// 		INT overlayCnt = vgsScene->overlay.size();
// 		for (INT i = 0; i< overlayCnt; i++)
// 		{
// 			COverlay* pOverlay = *(ppOverlay + i);
// 			if (pOverlay == sOverlay)
// 			{
// 				//如果已经是最前面一层
// 				if (i == overlayCnt -1) return;
// 
// 				COverlay* nextOverlay = *(ppOverlay + i + 1);
// 				vgsScene->overlay[i] = nextOverlay;
// 				vgsScene->overlay[i + 1] = pOverlay;
// 				return;
// 			}
// 		}
// 	}
}


//-------------------------------------------------------------------------------------------
//***Button相关的方法, 新建的Button对象将作为资源保存在E3D文件中，可以动态调用
//-------------------------------------------------------------------------------------------
//从本地文件创建Button
//name-Button的名称
//mouseUpImageFile-鼠标松开的源图像文件
//mouseUpAlphaImageFile-鼠标松开的Alpha图像文件
//mouseOverImageFile-鼠标移上的源图像文件
//mouseOverAlphaImageFile-鼠标移上的Alpha图像文件
//mouseDownImageFile-鼠标按下的源图像文件
//mouseDownAlphaImageFile-鼠标按下的Alpha图像文件
//alignMode-对齐模式，当为NONE时pButtonRect->top和pButtonRect->top自动失效
//pButtonRect-按钮的Rect
//callBackFunc-按下按钮的回调函数
DWORD CVGS2DLL::SunNewButton(CHAR* name, 
							 CHAR* mouseUpImageFile, CHAR* mouseUpAlphaImageFile,
							 CHAR* mouseOverImageFile, CHAR* mouseOverAlphaImageFile,
							 CHAR* mouseDownImageFile, CHAR* mouseDownAlphaImageFile,
							 ALIGNMODE  alignMode,
							 RECT* pButtonRect,
							 CHAR* callBackFunc)
{
		//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
	/*
	if (vgsScene == NULL) return 0;
	
	if (mouseUpImageFile == NULL || strcmp(mouseUpImageFile, "") == 0) return 0;
	
	CButton* pButton = new CButton();
	
	strcpy(pButton->name, name);
	int tIndex = vgsScene->button.size();
	pButton->alignMode = alignMode;
	strcpy(pButton->callBackFunc, callBackFunc);
	
	//创建按钮的Overlay
	CHAR btOverlayName[32] = "";
	CHAR btIndex[4] = "";
	itoa(tIndex, btIndex, 10);
	strcpy(btOverlayName, "bt_Overlay_");
	strcat(btOverlayName, btIndex);
	
	DWORD btOverlay = SunNewOverlay(btOverlayName, mouseUpImageFile, mouseUpAlphaImageFile, alignMode, pButtonRect);
	if (btOverlay != 0)
		pButton->btnOverlay = (COverlay*)btOverlay;
	else
	{
		SAFE_DELETE(pButton);
		return 0;
	}
	
	//创建光标移上去的材质
	CMaterial* pMouseOverMtrl = new CMaterial();
	strcpy(pMouseOverMtrl->name, "BtOver_Mtrl_");
	strcat(pMouseOverMtrl->name, btIndex);
	
	DWORD mouseOverOriTex = SunCreateTexture("btTexture", mouseOverImageFile, RGBA8880);
	DWORD mouseOverAlphaTex = SunCreateTexture("btTexture", mouseOverAlphaImageFile, RGBA8880);
	
	if (mouseOverOriTex != 0)
	{
		pMouseOverMtrl->diffuseTex = (CTexture*)mouseOverOriTex;
		if (mouseOverAlphaTex != 0)
		{
			pMouseOverMtrl->opacityTex = (CTexture*)mouseOverAlphaTex;
			
			//新建comp合成贴图
			pMouseOverMtrl->compTex = new CTexture();
			//设置合成后的贴图的格式
			if (pMouseOverMtrl->diffuseTex->BP == D3DFMT_R8G8B8 || pMouseOverMtrl->opacityTex->BP == D3DFMT_R8G8B8)
				pMouseOverMtrl->compTex->BP = D3DFMT_A8R8G8B8;
			else
				pMouseOverMtrl->compTex->BP = D3DFMT_A4R4G4B4;
			
			pMouseOverMtrl->compTex->width = pMouseOverMtrl->diffuseTex->width;
			pMouseOverMtrl->compTex->height = pMouseOverMtrl->diffuseTex->height;
			pMouseOverMtrl->compTex->withAlpha = TRUE;
			
			UINT bufSize;
			if (pMouseOverMtrl->compTex->BP == D3DFMT_A8R8G8B8)
				bufSize = pMouseOverMtrl->compTex->width * pMouseOverMtrl->compTex->height * 4;
			else
				bufSize = pMouseOverMtrl->compTex->width * pMouseOverMtrl->compTex->height * 2;
			pMouseOverMtrl->compTex->buffer = new BYTE[bufSize];
			
			vgsScene->CompositeAlphaMtrl(pMouseOverMtrl, 1);
			
			vgsScene->shader.push_back(pMouseOverMtrl);
		}
	}
	
	//看看是否为空设置，如果为空设置，则使用MouseOver材质为Over材质
	if (pMouseOverMtrl->diffuseTex != NULL)
		pButton->pMtrl_MouseOver = pMouseOverMtrl;
	else
	{
		SAFE_DELETE(pMouseOverMtrl);
		pButton->pMtrl_MouseOver = pButton->btnOverlay->m_pMtrl;
	}
	
	//创建光标按下去的材质
	CMaterial* pMouseDownMtrl = new CMaterial();
	strcpy(pMouseDownMtrl->name, "BtDown_Mtrl_");
	strcat(pMouseDownMtrl->name, btIndex);
	
	DWORD mouseDownOriTex = SunCreateTexture("btTexture", mouseDownImageFile, RGBA8880);
	DWORD mouseDownAlphaTex = SunCreateTexture("btTexture", mouseDownAlphaImageFile, RGBA8880);
	
	if (mouseDownOriTex != 0)
	{
		pMouseDownMtrl->diffuseTex = (CTexture*)mouseDownOriTex;
		if (mouseOverAlphaTex != 0)
		{
			pMouseDownMtrl->opacityTex = (CTexture*)mouseDownAlphaTex;
			
			//新建comp合成贴图
			pMouseDownMtrl->compTex = new CTexture();
			//设置合成后的贴图的格式
			if (pMouseDownMtrl->diffuseTex->BP == D3DFMT_R8G8B8 || pMouseOverMtrl->opacityTex->BP == D3DFMT_R8G8B8)
				pMouseDownMtrl->compTex->BP = D3DFMT_A8R8G8B8;
			else
				pMouseDownMtrl->compTex->BP = D3DFMT_A4R4G4B4;
			
			pMouseDownMtrl->compTex->width = pMouseDownMtrl->diffuseTex->width;
			pMouseDownMtrl->compTex->height = pMouseDownMtrl->diffuseTex->height;
			pMouseDownMtrl->compTex->withAlpha = TRUE;
			
			UINT bufSize;
			if (pMouseDownMtrl->compTex->BP == D3DFMT_A8R8G8B8)
				bufSize = pMouseDownMtrl->compTex->width * pMouseDownMtrl->compTex->height * 4;
			else
				bufSize = pMouseDownMtrl->compTex->width * pMouseDownMtrl->compTex->height * 2;
			pMouseDownMtrl->compTex->buffer = new BYTE[bufSize];
			
			vgsScene->CompositeAlphaMtrl(pMouseDownMtrl, 1);
			
			vgsScene->shader.push_back(pMouseDownMtrl);
		}
	}
	
	//看看是否为空设置，如果为空设置，则使用MouseUp材质为Over材质
	if (pMouseDownMtrl->diffuseTex != NULL)
		pButton->pMtrl_MouseDown = pMouseDownMtrl;
	else
	{
		SAFE_DELETE(pMouseDownMtrl);
		pButton->pMtrl_MouseDown = pButton->btnOverlay->m_pMtrl;
	}
	
	vgsScene->button.push_back(pButton);
	
	//返回按钮的句柄
	return (DWORD)pButton;
	*/
    return 0;
}


//从Image对象创建Button
//name-Button的名称
//mouseUpImage-鼠标松开的源图像对象
//mouseUpImage_alpha-鼠标松开的Alpha图像对象
//mouseOverImage-鼠标移上的源图像对象
//mouseOverImage_alpha-鼠标移上的Alpha图像对象
//mouseDownImage-鼠标按下的源图像对象
//mouseDownImage_alpha-鼠标按下的Alpha图像对象
//ALIGNMODE alignMode-对齐模式
//pButtonRect-Button的Rect，当对齐align模式设置为相应模式时，相应参数是小
DWORD CVGS2DLL::SunNewButtonFromImage(CHAR* name, 
									  CHAR* mouseUpImage, CHAR* mouseUpImage_alpha,
									  CHAR* mouseOverImage, CHAR* mouseOverImage_alpha,
									  CHAR* mouseDownImage, CHAR* mouseDownImage_alpha,
									  ALIGNMODE alignMode,
									  RECT* pButtonRect,
									  CHAR* callBackFunc)
{
	//---------------------------------------------------------------------------------------------------
//commend by baojiahui 2008/8/14
/*
	if (vgsScene == NULL) return 0;
	if (mouseUpImage == 0 || strcmp(mouseUpImage, "") == 0) return 0;
	
	CButton* pButton = new CButton();
	
	strcpy(pButton->name, name);
	int tIndex = vgsScene->button.size();
	pButton->alignMode = alignMode;
	strcpy(pButton->callBackFunc, callBackFunc);
	
	//创建按钮的Overlay
	CHAR btOverlayName[32] = "";
	CHAR btIndex[4] = "";
	itoa(tIndex, btIndex, 10);
	strcpy(btOverlayName, "bt_Overlay_");
	strcat(btOverlayName, btIndex);
	
	DWORD btOverlay = SunNewOverlayFromImage(btOverlayName, mouseUpImage, mouseUpImage_alpha, alignMode, pButtonRect);
	if (btOverlay != 0)
	{
		pButton->btnOverlay = (COverlay*)btOverlay;
		pButton->pMtrl_MouseUp = pButton->btnOverlay->m_pMtrl;
	}
	else
	{
		SAFE_DELETE(pButton);
		return 0;
	}
	
	//创建光标移上去的材质------------------------------
	if (mouseOverImage != 0 && strcmp(mouseOverImage, "") != 0)
	{
		CMaterial* pMouseOverMtrl = new CMaterial();
		strcpy(pMouseOverMtrl->name, "BtOver_Mtrl_");
		strcat(pMouseOverMtrl->name, btIndex);
		pMouseOverMtrl->index = vgsScene->shader.size();
		pMouseOverMtrl->bIsOverlayMtrl = TRUE;
		
		DWORD mouseOverOriTex = SunCreateTextureFromImage("btTexture", mouseOverImage, RGBA8880, 1);
		DWORD mouseOverAlphaTex = SunCreateTextureFromImage("btTexture", mouseOverImage_alpha, RGBA8880, 1);
		
		if (mouseOverOriTex != 0)
		{
			pMouseOverMtrl->diffuseTex = (CTexture*)mouseOverOriTex;
			pMouseOverMtrl->diffuseTex->index = vgsScene->texture.size();
			pMouseOverMtrl->diffuseTex->bIsOverlayTexture = TRUE;
			vgsScene->texture.push_back(pMouseOverMtrl->diffuseTex);
			
			if (mouseOverAlphaTex != 0)
			{
				//opacity
				pMouseOverMtrl->opacityTex = (CTexture*)mouseOverAlphaTex;
				pMouseOverMtrl->opacityTex->index = vgsScene->texture.size();
				pMouseOverMtrl->opacityTex->bIsOverlayTexture = TRUE;
				vgsScene->texture.push_back(pMouseOverMtrl->opacityTex);
				
				//新建comp合成贴图
				pMouseOverMtrl->compTex = new CTexture();	
				pMouseOverMtrl->compTex->index = vgsScene->texture.size();
				pMouseOverMtrl->compTex->bIsOverlayTexture = TRUE;
				
				//设置合成后的贴图的格式
				if (pMouseOverMtrl->diffuseTex->BP == D3DFMT_R8G8B8 || pMouseOverMtrl->opacityTex->BP == D3DFMT_R8G8B8)
					pMouseOverMtrl->compTex->BP = D3DFMT_A8R8G8B8;
				else
					pMouseOverMtrl->compTex->BP = D3DFMT_A4R4G4B4;
				
				pMouseOverMtrl->compTex->width = pMouseOverMtrl->diffuseTex->width;
				pMouseOverMtrl->compTex->height = pMouseOverMtrl->diffuseTex->height;
				pMouseOverMtrl->compTex->withAlpha = TRUE;
				
				UINT bufSize;
				if (pMouseOverMtrl->compTex->BP == D3DFMT_A8R8G8B8)
					bufSize = pMouseOverMtrl->compTex->width * pMouseOverMtrl->compTex->height * 4;
				else
					bufSize = pMouseOverMtrl->compTex->width * pMouseOverMtrl->compTex->height * 2;
				pMouseOverMtrl->compTex->buffer = new BYTE[bufSize];
				
				vgsScene->CompositeAlphaMtrl(pMouseOverMtrl, 1);
				vgsScene->texture.push_back(pMouseOverMtrl->compTex);
				
				SunDeleteTexture((DWORD)pMouseOverMtrl->diffuseTex);
				SunDeleteTexture((DWORD)pMouseOverMtrl->opacityTex);
			}
		}
		//看看是否为空贴图，如果为空贴图，则使用MouseUp材质为Over材质
		if (pMouseOverMtrl->diffuseTex != NULL)
			pButton->pMtrl_MouseOver = pMouseOverMtrl;
		else
		{
			SAFE_DELETE(pMouseOverMtrl);
			pButton->pMtrl_MouseOver = pButton->btnOverlay->m_pMtrl;
		}
		vgsScene->shader.push_back(pMouseOverMtrl);
	}
	
	//创建光标按下去的材质---------------------------------
	if (mouseDownImage != 0 && strcmp(mouseDownImage, "") != 0)
	{
		CMaterial* pMouseDownMtrl = new CMaterial();
		strcpy(pMouseDownMtrl->name, "BtDown_Mtrl_");
		strcat(pMouseDownMtrl->name, btIndex);
		pMouseDownMtrl->index = vgsScene->shader.size();
		pMouseDownMtrl->bIsOverlayMtrl = TRUE;
		
		DWORD mouseDownOriTex = SunCreateTextureFromImage("btTexture", mouseDownImage, RGBA8880, 1);
		DWORD mouseDownAlphaTex = SunCreateTextureFromImage("btTexture", mouseDownImage_alpha, RGBA8880, 1);
		
		if (mouseDownOriTex != 0)
		{
			pMouseDownMtrl->diffuseTex = (CTexture*)mouseDownOriTex;
			pMouseDownMtrl->diffuseTex->index = vgsScene->texture.size();
			pMouseDownMtrl->diffuseTex->bIsOverlayTexture = TRUE;
			vgsScene->texture.push_back(pMouseDownMtrl->diffuseTex);
			
			if (mouseDownAlphaTex != 0)
			{
				pMouseDownMtrl->opacityTex = (CTexture*)mouseDownAlphaTex;
				pMouseDownMtrl->opacityTex->index = vgsScene->texture.size();
				pMouseDownMtrl->opacityTex->bIsOverlayTexture = TRUE;
				vgsScene->texture.push_back(pMouseDownMtrl->diffuseTex);
				
				//新建comp合成贴图
				pMouseDownMtrl->compTex = new CTexture();
				pMouseDownMtrl->compTex->index = vgsScene->texture.size();
				pMouseDownMtrl->compTex->bIsOverlayTexture = TRUE;
				
				//设置合成后的贴图的格式
				if (pMouseDownMtrl->diffuseTex->BP == D3DFMT_R8G8B8 || pMouseDownMtrl->opacityTex->BP == D3DFMT_R8G8B8)
					pMouseDownMtrl->compTex->BP = D3DFMT_A8R8G8B8;
				else
					pMouseDownMtrl->compTex->BP = D3DFMT_A4R4G4B4;
				
				pMouseDownMtrl->compTex->width = pMouseDownMtrl->diffuseTex->width;
				pMouseDownMtrl->compTex->height = pMouseDownMtrl->diffuseTex->height;
				pMouseDownMtrl->compTex->withAlpha = TRUE;
				
				UINT bufSize;
				if (pMouseDownMtrl->compTex->BP == D3DFMT_A8R8G8B8)
					bufSize = pMouseDownMtrl->compTex->width * pMouseDownMtrl->compTex->height * 4;
				else
					bufSize = pMouseDownMtrl->compTex->width * pMouseDownMtrl->compTex->height * 2;
				pMouseDownMtrl->compTex->buffer = new BYTE[bufSize];
				
				vgsScene->CompositeAlphaMtrl(pMouseDownMtrl, 1);
				vgsScene->texture.push_back(pMouseDownMtrl->compTex);
				
				SunDeleteTexture((DWORD)pMouseDownMtrl->diffuseTex);
				SunDeleteTexture((DWORD)pMouseDownMtrl->opacityTex);
			}
		}
		
		//看看是否为空贴图，如果为空贴图，则使用MouseUp材质为Over材质
		if (pMouseDownMtrl->diffuseTex != NULL)
			pButton->pMtrl_MouseDown = pMouseDownMtrl;
		else
		{
			SAFE_DELETE(pMouseDownMtrl);
			pButton->pMtrl_MouseDown = pButton->btnOverlay->m_pMtrl;
		}
		vgsScene->shader.push_back(pMouseDownMtrl);
	}
	
	vgsScene->button.push_back(pButton);
	
	//返回按钮的句柄
	return (DWORD)pButton;
	*/
return 0;
}


//通过名称得到Button
DWORD CVGS2DLL::SunGetButtonByName(CHAR* name)
{
// 	if (vgsScene == NULL) return 0;
// 	
// 	UINT btCnt = vgsScene->button.size();
// 	CButton** ppButton = vgsScene->button.begin();
// 	
// 	for (UINT i= 0; i< btCnt; i++)
// 	{
// 		CButton* pButton = *(ppButton + i);
// 		if (strcmp(pButton->name, name) == 0)
// 			return (DWORD)pButton;
// 	}
	
	return 0;
}

//通过编号得到Button
DWORD CVGS2DLL::SunGetButtonByIndex(UINT index)
{
// 	if (vgsScene == NULL || index < 0) return 0;
// 	
// 	CButton** ppButton = vgsScene->button.begin();
// 	CButton* pButton = *(ppButton + index);
// 	
	// 	return (DWORD)pButton;
	return 0;
}

//删除按钮
VOID CVGS2DLL::SunDeleteButton(DWORD buttonHandle)
{
/*	if (vgsScene == NULL || buttonHandle <= 0) return;
	
	CButton* pButton = (CButton*)buttonHandle;
	
	//把button用到的overlay从overlay列表中删除
	if (pButton->btnOverlay != NULL)
	{
		//删除button用到的overlay
		SunDelOverlay((DWORD)pButton->btnOverlay);
		
		//把button用到的材质从材质列表中删除
		if (pButton->pMtrl_MouseUp)
		{
			CMaterial* pMtrl = pButton->pMtrl_MouseUp;
			SunDeleteTexture((DWORD)pMtrl->diffuseTex);
			SunDeleteTexture((DWORD)pMtrl->opacityTex);
			SunDeleteTexture((DWORD)pMtrl->compTex);
			SunDeleteMtrl((DWORD)pMtrl);
		}
		if (pButton->pMtrl_MouseOver)
		{
			CMaterial* pMtrl = pButton->pMtrl_MouseOver;
			SunDeleteTexture((DWORD)pMtrl->diffuseTex);
			SunDeleteTexture((DWORD)pMtrl->opacityTex);
			SunDeleteTexture((DWORD)pMtrl->compTex);
			SunDeleteMtrl((DWORD)pMtrl);
		}
		if (pButton->pMtrl_MouseDown)
		{
			CMaterial* pMtrl = pButton->pMtrl_MouseDown;
			SunDeleteTexture((DWORD)pMtrl->diffuseTex);
			SunDeleteTexture((DWORD)pMtrl->opacityTex);
			SunDeleteTexture((DWORD)pMtrl->compTex);
			SunDeleteMtrl((DWORD)pMtrl);
		}
		
	}
	
	//把button从button列表中删除
	UINT btnCnt = vgsScene->button.size();
	CButton** ppButton = vgsScene->button.begin();
	for (UINT i = 0; i< btnCnt; i++)
	{
		CButton* pTButton = *(ppButton + i);
		if (pTButton == pButton)
		{
			vgsScene->button.erase(ppButton + i);
			SAFE_RELEASE(pButton);
			SAFE_DELETE(pButton);
			break;
		}
	}*/
}

//-------------------------------------------------------------------------------------------
//***Image相关的方法, 新建的Image对象将作为资源保存在E3D文件中，可以动态调用
//-------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: SunNewImage()
// 参  数: CHAR* name, CHAR* fileName
// 用  途: 新建Image资源
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunNewImage(CHAR* name, CHAR* fileName)
{
/*	if (vgsScene == NULL || name == NULL || fileName == NULL) return 0;
	
	CImage* pImage = new CImage();
	
	pImage->index = vgsScene->image.size();
	strcpy(pImage->name, name);
	
	_D3DXIMAGE_INFO imageInfo;
	if (FAILED(D3DXGetImageInfoFromFile(fileName, &imageInfo)))
	{
		SAFE_DELETE(pImage);
		return 0;
	}
	
	pImage->width = imageInfo.Width;
	pImage->height = imageInfo.Height;
	pImage->format = imageInfo.Format;
	
	FILE* pFile = fopen(fileName, "rb");
	if (pFile == NULL)
	{
		SAFE_DELETE(pImage);
		return 0;
	}
	fseek(pFile, 0, SEEK_END);
	pImage->dataLength = ftell(pFile);
	if (pImage->dataLength == 0) 
	{
		SAFE_DELETE(pImage);
		return 0;
	}
	
	fseek(pFile, 0, SEEK_SET);
	pImage->data = new BYTE[pImage->dataLength];
	if (pImage->data == NULL)
	{
		SAFE_DELETE(pImage);
		return 0;
	}
	fread(pImage->data, pImage->dataLength, 1, pFile);
	
	//将Image对象放入链表
	vgsScene->image.push_back(pImage);
	
	return (DWORD)pImage;
	*/
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SunGetImageHandleByIndex()
// 参  数: INT index
// 用  途: 通过编号得到Image对象
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetImageHandleByIndex(INT index)
{
// 	if (vgsScene == NULL || index < 0) return 0;
// 	
// 	INT imageCnt = vgsScene->image.size();
// 	if (index > imageCnt) return 0;
// 	
// 	CImage** ppImage = vgsScene->image.begin();
// 	CImage* pImage = *(ppImage + index);
	// 	return (DWORD)pImage;
	return 0;
}

//-----------------------------------------------------------------------------
// 方法名: SunGetImageHandleByName()
// 参  数: CHAR* name
// 用  途: 通过图像名称得到Image对象
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetImageHandleByName(CHAR* name)
{
// 	if (vgsScene == NULL || name == NULL) return 0;
// 	
// 	UINT imageCnt = vgsScene->image.size();
// 	CImage** ppImage = vgsScene->image.begin();
// 	for (UINT i = 0; i< imageCnt; i++)
// 	{
// 		CImage* pImage = *(ppImage + i);
// 		if (pImage != NULL && strcmp(pImage->name, name) == 0)
// 			return (DWORD)pImage;
// 	}
	return 0;	
}
//-----------------------------------------------------------------------------
// 方法名: SunDeleteImageByIndex()
// 参  数: INT index
// 用  途: 通过图像编号删除Image对象
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDeleteImageByIndex(INT index)
{
// 	if (vgsScene == NULL || index <0) return;
// 	
// 	CImage** ppImage = vgsScene->image.begin();
// 	CImage* pImage = *(ppImage + index);
// 	
// 	vgsScene->image.erase(ppImage + index);
// 	SAFE_RELEASE(pImage);
// 	SAFE_DELETE(pImage);
	
}
//-----------------------------------------------------------------------------
// 方法名: SunDeleteImageByName()
// 参  数: CHAR* name
// 用  途: 通过图像名称删除Image资源
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDeleteImageByName(CHAR* name)
{
// 	if (vgsScene == NULL || name == NULL) return;
// 	
// 	INT imageCnt = vgsScene->image.size();
// 	CImage** ppImage = vgsScene->image.begin();
// 	
// 	for (UINT i = 0; i< imageCnt; i++)
// 	{
// 		CImage* pImage = *(ppImage + i);
// 		if (pImage != NULL && strcmp(pImage->name, name) == 0)
// 		{
// 			vgsScene->image.erase(ppImage + i);
// 			SAFE_RELEASE(pImage);
// 			SAFE_DELETE(pImage);
// 			return;
// 		}
// 	}	
}



//-------------------------------------------------------------------------------------------
//***导航图相关的方法
//-------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 方法名: SunDeleteNavigater()
// 参  数: DWORD navigterHandle
// 用  途: 删除Navigater
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunDeleteNavigater(DWORD navigaterHandle)
{
// 	if (vgsScene == NULL && navigaterHandle <= 0) return;
// 	
// 	CNavigater* pNavigater = (CNavigater*)navigaterHandle;
// 	
// 	UINT nvCnt = vgsScene->navigater.size();
// 	CNavigater** ppNavigater = vgsScene->navigater.begin();
// 	for (UINT i = 0; i< nvCnt; i++)
// 	{
// 		CNavigater* pNV = *(ppNavigater + i);
// 		if (pNV == pNavigater)
// 		{
// 			vgsScene->navigater.erase(ppNavigater + i);
// 			SAFE_RELEASE(pNavigater);
// 			SAFE_RELEASE(pNavigater);
// 			return;
// 		}
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: SunDeleteNavigater()
// 参  数: DWORD navigterHandle
// 用  途: 通过Navigater的名称得到句柄
//-----------------------------------------------------------------------------
DWORD CVGS2DLL::SunGetNavigaterByName(CHAR* name)
{
// 	if (vgsScene == NULL && name == NULL) return 0;
// 	
// 	UINT nvCnt = vgsScene->navigater.size();
// 	CNavigater** ppNavigater = vgsScene->navigater.begin();
// 	
// 	for (UINT i = 0; i< nvCnt; i++)
// 	{
// 		CNavigater* pNV = *(ppNavigater + i);
// 		if (strcmp(pNV->name, name) == 0)
// 			return (DWORD)pNV;
// 	}
	return 0;
}

//增加导航指针对象
//navigaterHandle - 导航图对象的句柄
//navPointerName - 导航图指针对象的名称
//pointerType - 导航图指针的类型，0为相机导航，1为模型导航
//pointerHandle - 导航图指针的句柄，如果导航图指针类型为相机，则为相机句柄，否则为模型句柄
//oriPointerFile - 导航图指针图片
//alphaPointerFile - 导航图指针alpha图片
//pointerWidth, pointerHeight - 导航图指针的尺寸
//返回导航图指针对向的句柄
DWORD CVGS2DLL::AddNavigaterPointer(DWORD navigaterHandle, 
									CHAR* navPointerName, 
									INT	  pointerType,	//camera or model 
									DWORD pointerHandle, //camera or model handle
									CHAR* oriPointerImageName, 
									CHAR* alphaPointerImageName, 
									INT pointerWidth, 
									INT pointerHeight)
{
// 	CNavigater* pNav = (CNavigater*)navigaterHandle;
// 	if (pNav == NULL) return 0;
// 
// 	if (pointerType < 0 || pointerType > 1)
// 		return 0;
// 
// 	//创建导航图指针对向
// 	NavPointer* pNavPointer = new NavPointer;
// 	strcpy(pNavPointer->Name, navPointerName);
// 	pNavPointer->NavPointerType = pointerType;
// 	pNavPointer->NavPointerHandle = pointerHandle;
// 	
// 	//创建导航图指针的Overlay对象
// 	//Pointer Overlay
// 	RECT pointerRect;
// 	pointerRect.left = 0;
// 	pointerRect.top = 0;
// 	pointerRect.right = pointerRect.left + pointerWidth;
// 	pointerRect.bottom = pointerRect.top + pointerHeight;
// 
// 	DWORD dwPointer = SunNewOverlayFromImage(navPointerName, 
// 											oriPointerImageName, 
// 											alphaPointerImageName, 
// 											ALIGN_NONE,	
// 											&pointerRect);
// 	if (dwPointer == 0)
// 		SAFE_DELETE(pNavPointer);
// 
// 	pNavPointer->pPointerOverlay = (COverlay*)dwPointer;
// 	
// 	pNav->m_NavPointer.push_back(pNavPointer);
// 
	// 	return (DWORD)pNavPointer;
	return 0;
}


//-------------------------------------------------------------------------------------------
//***剖面图相关的方法
//-------------------------------------------------------------------------------------------
//创建剖面图对象
//sectionMapName-剖面图对象名称
//sliceModelHandle-切片模型
//leftTopX, leftTopY, leftTopZ 侧面左上角坐标
//rightTopX, rightTopY, rightTopZ 侧面右上角坐标
//leftBottomX, leftBottomY, leftBottom 侧面左下角坐标
//rightBottomX, rightBottomY, rightBottomZ 侧面右下角坐标
//imageHandle image对象
DWORD CVGS2DLL::CreateSectionMap(CHAR* sectionMapName, 
								 INT width, INT height,
								 DWORD sliceModelHandle, 
								 FLOAT leftTopX, FLOAT leftTopY, FLOAT leftTopZ, 
								 FLOAT rightTopX, FLOAT rightTopY, FLOAT rightTopZ, 
								 FLOAT leftBottomX, FLOAT leftBottomY, FLOAT leftBottomZ, 
								 FLOAT rightBottomX, FLOAT rightBottomY, FLOAT rightBottomZ, 
								 DWORD imageHandle)
{
//Notes by Baojiahui 2008/7/16-----------------------------------------------------------
/*	//-----------------------------------------------------------------------------------
	if (sectionMapName == NULL ||
		sliceModelHandle == 0 ||
		imageHandle == 0)
		return 0;

	CSectionMap* pSectionMap = new CSectionMap((DWORD)vgsScene);
	//剖面图名称 
	strcpy(pSectionMap->name, sectionMapName);
	pSectionMap->index = vgsScene->sectionMap.size() + 1;
	
	//创建Image贴图对象
	CHAR strSectionImageTex[32];
	strcpy(strSectionImageTex, "SectionMap_Tex");
	CHAR strIndex[16];
	itoa(pSectionMap->index, strIndex, 10);
	strcat(strSectionImageTex, strIndex);

	CImage* pImage = (CImage*)imageHandle;
	pSectionMap->m_pImage = pImage;
	pSectionMap->m_pImageTexture = (CTexture*)SunCreateTextureFromImage(strSectionImageTex,
										pImage->name, RGBA8880, 1);	
	vgsScene->texture.push_back(pSectionMap->m_pImageTexture);

	CModel* pModel = (CModel*)sliceModelHandle;
	pSectionMap->m_pSectionOriTexture = pModel->shader->diffuseTex;

	//创建剖面图对象
	pSectionMap->Create(pModel,
						&(D3DXVECTOR3(leftTopX, leftTopY, leftTopZ)),
						&(D3DXVECTOR3(rightTopX, rightTopY, rightTopZ)),
						&(D3DXVECTOR3(leftBottomX, leftBottomY, leftBottomZ)),
						&(D3DXVECTOR3(rightBottomX, rightBottomY, rightBottomZ)),
						width, height);

	vgsScene->sectionMap.push_back(pSectionMap);

	
	return (DWORD)pSectionMap;*/
//---------------------------------------------------------------------------------------
	return 0;
}

//执行切片
VOID CVGS2DLL::ExecuteSlice(DWORD sectionMapHandle)
{
// 	CSectionMap* pSectionMap = (CSectionMap*)sectionMapHandle;
// 	if (pSectionMap)
// 	{
// 		pSectionMap->Slice();
// 	}
}

//通过SectionMap的名称得到句柄
DWORD CVGS2DLL::GetSectionMapHandleByName(CHAR* sectionMapName)
{
// 	INT sectionMapCnt = vgsScene->sectionMap.size();
// 	CSectionMap** ppSectionMap = vgsScene->sectionMap.begin();
// 	for (INT i = 0; i< sectionMapCnt; i++)
// 	{
// 		CSectionMap* pSectionMap = *(ppSectionMap + i);
// 		if (strcmp(pSectionMap->name, sectionMapName) == 0)
// 			return (DWORD)pSectionMap;
// 	}
	return 0;
}

//-------------------------------------------------------------------------------------------
//***控制相关的方法
//-------------------------------------------------------------------------------------------

// 获得当前使用的控制对象
DWORD CVGS2DLL::SunGetCurrentController()
{
	return m_VGSCore.GetCurrentController(m_vgsScene);
}

// 设置当前使用的控制对象
VOID CVGS2DLL::SunSetCurrentController(DWORD controllerHandle)
{
	m_VGSCore.SetCurrentController(m_vgsScene, controllerHandle);

	// 获取当前控制对象信息
	GetCurrentControllerInfo();
}

// 设置控制器的相机
VOID CVGS2DLL::SunSetControllerCamera(DWORD ControllerHandle, DWORD CameraHandle)
{
	m_VGSCore.SetControllerCamera(m_vgsScene, ControllerHandle, CameraHandle);
	
	// 获取当前控制对象信息
	GetCurrentControllerInfo();
}

// 获取当前控制对象使用的相机
DWORD CVGS2DLL::SunGetControllerCamera(DWORD ControllerHandle)
{
	return m_VGSCore.GetControllerCamera(m_vgsScene, ControllerHandle);
	// GetPerspectiveCamera(0);
}

// 获取当前控制对象使用的相机
DWORD CVGS2DLL::SunGetCurrentControllerCamera()
{
	return m_VGSCore.GetCurrentControllerCamera(m_vgsScene);
	// GetPerspectiveCamera(0);
}



BOOL CVGS2DLL::GetShiftStatus()
{
	//----------------------------------------------------------------------------------------
	//键盘控制
	//----------------------------------------------------------------------------------------
// 	::GetKeyboardState(m_inputDevice.keyBoardBuf);
// 	if (m_inputDevice.keyBoardBuf[VK_LSHIFT] >64 || 
// 		m_inputDevice.keyBoardBuf[VK_RSHIFT] >64 )
// 	{
// 		return TRUE;
// 	}
// 	else
// 	{
// 	    return FALSE;
// 	}
	
	return 0;
}
//-----------------------------------------------------------------------------
// 方法名: EditControl()
// 参  数: 
// 用  途: 用户控制物件展示的方法在这里
//-----------------------------------------------------------------------------
VOID CVGS2DLL::EditControl()
{
// 	FLOAT realPitchSpeed = 0;
// 	FLOAT realRoundSpeed = 0;
// 	FLOAT realRollSpeed = 0;
// 	
// 	//是否自动旋转
// 	if (editor.bIsAutoRoundRoll)
// 	{
// 		realRoundSpeed = editor.autoRoundSpeed;
// 		realPitchSpeed = editor.autoPitchSpeed;
// 		realRollSpeed = editor.autoRollSpeed;
// 	}
// 	else
// 	{
// 		//默认的鼠标控制----------------------------------------------------------------------------------------------
// 		//按下鼠标左键，左右拖拽鼠标，控制镜头绕着目标点旋转
// 		if (bCanRos)
// 		{
// 			static INT disX = 0;
// 			static INT disY = 0;
// 			if (m_inputDevice.mouseInfo.LButton)
// 			{
// 				disX = m_inputDevice.mouseInfo.X - vgsScene->lastClickPos.x;
// 				disY = m_inputDevice.mouseInfo.Y - vgsScene->lastClickPos.y;
// 			}
// 			
// 			//拖拽鼠标后松开鼠标键，模型继续旋转，逐渐减速，到一定速度，匀速旋转
// 			if (editor.rosAxis == 0 || editor.rosAxis == 2)
// 			{
// 				if(disX != 0)
// 					realRoundSpeed = -(FLOAT)disX/1000.0f; //1200表示重量感
// 				disX = (INT)(disX * 0.9f); //0.9表示阻尼
// 			}
// 			
// 			if (editor.rosAxis == 0 || editor.rosAxis == 1)
// 			{
// 				if(disY != 0)
// 					realPitchSpeed = -(FLOAT)disY/1500.0f;
// 				disY = (INT)(disY * 0.9f);
// 			}
// 		}
// 		
// 		//----------------------------------------------------------------------------------------
// 		//键盘控制
// 		//----------------------------------------------------------------------------------------
// 		::GetKeyboardState(m_inputDevice.keyBoardBuf);
// 		
// 		if (bCanRos)
// 		{
// #ifdef _DEBUG
// 			//调试状态信息控制变量
// 			if (m_inputDevice.keyBoardBuf['k'] || 
// 				m_inputDevice.keyBoardBuf['K']>64)
// 				bShowDebugInfo = !bShowDebugInfo;
// #endif
// 
// 			//得到速度值，有阻尼效果
// 			if (/*m_inputDevice.keyBoardBuf['a'] > 64 || 
// 				m_inputDevice.keyBoardBuf['A'] > 64 || 
// 				m_inputDevice.keyBoardBuf['q'] > 64 ||
// 				m_inputDevice.keyBoardBuf['Q'] > 64 ||*/
// 				IsAlphaKeyBuffered(m_AddKeyLeft) || 
// 				m_inputDevice.keyBoardBuf[VK_LEFT] > 64 || 
// 				vgsScene->softKey[2])
// 				realRoundSpeed = editor.manualRoundSpeed / 50.0f;
// 			
// 			if (/*m_inputDevice.keyBoardBuf['d'] > 64 ||
// 				m_inputDevice.keyBoardBuf['D'] > 64 || 
// 				m_inputDevice.keyBoardBuf['e'] > 64 ||
// 				m_inputDevice.keyBoardBuf['E'] > 64 ||*/
// 				IsAlphaKeyBuffered(m_AddKeyRight) || 
// 				m_inputDevice.keyBoardBuf[VK_RIGHT] > 64 || 
// 				vgsScene->softKey[3])
// 				realRoundSpeed = -editor.manualRoundSpeed / 50.0f;	
// 			
// 			if (/*m_inputDevice.keyBoardBuf['w'] > 64 || 
// 				m_inputDevice.keyBoardBuf['W'] > 64 || 
// 				m_inputDevice.keyBoardBuf['r'] > 64 ||
// 				m_inputDevice.keyBoardBuf['R'] > 64 ||*/
// 				IsAlphaKeyBuffered(m_AddKeyUp) || 
// 				m_inputDevice.keyBoardBuf[VK_UP] > 64 || 
// 				vgsScene->softKey[0])
// 				realPitchSpeed = editor.manualPitchSpeed / 50.0f;
// 			
// 			if (/*m_inputDevice.keyBoardBuf['s'] > 64 || 
// 				m_inputDevice.keyBoardBuf['S'] > 64 || 
// 				m_inputDevice.keyBoardBuf['f'] > 64 ||
// 				m_inputDevice.keyBoardBuf['F'] > 64 || */
// 				IsAlphaKeyBuffered(m_AddKeyDown) || 
// 				m_inputDevice.keyBoardBuf[VK_DOWN] > 64 || 
// 				vgsScene->softKey[1])
// 				realPitchSpeed = -editor.manualPitchSpeed / 50.0f;
// 		}
// 	}
// 
// 	//限制低头和抬头的角度
// 	if (realPitchSpeed || realRoundSpeed || realRollSpeed)
// 	{
// 		D3DXVECTOR3 vUp = vgsScene->currentCam->GetUp();
// 		FLOAT fAngleCosValue = D3DXVec3Dot(&vUp, &D3DXVECTOR3(0,1,0));
// 		if (fAngleCosValue > editor.maxDownCosValue) 
// 			vgsScene->currentCam->RoundInCameraSpace(realPitchSpeed, realRoundSpeed, realRollSpeed);
// 
// 		vUp = vgsScene->currentCam->GetUp();
// 		fAngleCosValue = D3DXVec3Dot(&vUp, &D3DXVECTOR3(0,1,0));
// 		if (fAngleCosValue < editor.maxUpCosValue)
// 			vgsScene->currentCam->RoundInCameraSpace(-realPitchSpeed, realRoundSpeed, realRollSpeed);
// 
// 	}
// 
// 	/*
// 	if (realPitchSpeed || realRoundSpeed || realRollSpeed)
// 		vgsScene->currentCam->RoundInCameraSpace(realPitchSpeed, realRoundSpeed, realRollSpeed);
// 	*/
// 	
// 	//----------------------------------------------------------------------
// 	//推拉Editor镜头
// 	//----------------------------------------------------------------------
// 	//镜头推近
// 	if (bCanZoom)
// 	{
// 		if (m_inputDevice.keyBoardBuf[VK_END] > 64 || 
// 			vgsScene->softKey[11])
// 		{
// 			//判断是否超过了DollyNearDis和DollyFarDis
// 			
// 			D3DXVECTOR3 from = vgsScene->currentCam->GetFrom();
// 			D3DXVECTOR3 at = vgsScene->currentCam->GetAt();
// 			D3DXVECTOR3 dis = at - from;
// 			FLOAT len = D3DXVec3Length(&dis);
// 			FLOAT dollySpeed = 0.03f * len;
// 			FLOAT dollyValue = -dollySpeed;
// 			
// 			FLOAT dollyNearDis = editor.dollyNearDis;
// 			FLOAT dollyFarDis = editor.dollyFarDis;
// 
// 			if (len > dollyNearDis + dollyValue)
// 			{
// 				if (len < dollyFarDis + dollyValue)
// 					vgsScene->currentCam->Dolly(-dollyValue);
// 			}
// 		}
// 		//镜头拉远
// 		if (m_inputDevice.keyBoardBuf[VK_HOME] > 64 || 
// 			vgsScene->softKey[10])
// 		{
// 		
// 			//判断是否超过了DollyNearDis和DollyFarDis
// 			D3DXVECTOR3 from = vgsScene->currentCam->GetFrom();
// 			D3DXVECTOR3 at = vgsScene->currentCam->GetAt();
// 			D3DXVECTOR3 dis = at - from;
// 			FLOAT len = D3DXVec3Length(&dis);
// 			FLOAT dollySpeed = 0.03f * len;
// 			FLOAT dollyValue = dollySpeed;
// 			
// 			FLOAT dollyNearDis = editor.dollyNearDis;
// 			FLOAT dollyFarDis = editor.dollyFarDis;
// 			
// 			if (len > dollyNearDis + dollyValue)
// 			{
// 				if (len < dollyFarDis + dollyValue)
// 					vgsScene->currentCam->Dolly(-dollyValue);
// 			}
// 		}
// 
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: FlyControl()
// 参  数: 
// 用  途: GIS控制模式的方法在这里
//-----------------------------------------------------------------------------
VOID CVGS2DLL::FlyControl()
{
	//默认的飞行控制----------------------------------------------------------------
	//平移
// 	static FLOAT realMoveSpeedX = 0.0f;
// 	static FLOAT realMoveSpeedZ = 0.0f;
// 	FLOAT moveSpeedX = 0.0f;
// 	FLOAT moveSpeedZ = 0.0f;
// 	
// 	//根据相机的高度，自动调整Pan的速度
// 	D3DXVECTOR3 from, at, dis;
// 	from = vgsScene->currentCam->GetFrom();
// 	at = vgsScene->currentCam->GetAt();
// 	dis = at - from;
// 	FLOAT len = D3DXVec3Length(&dis);
// 	flyer.panSpeed = 0.002f * len;
// 	
// 	::GetKeyboardState(m_inputDevice.keyBoardBuf);
// 	
// 	//调试状态信息控制变量
// #ifdef _DEBUG
// 	if (m_inputDevice.keyBoardBuf['k'] || m_inputDevice.keyBoardBuf['K']>64)
// 		bShowDebugInfo = !bShowDebugInfo;
// #endif
// 	
// 	// fuxb 2009-03-17 ==>
// 	//前后平移
// // 	if (m_inputDevice.keyBoardBuf['w'] > 64 || m_inputDevice.keyBoardBuf['W'] > 64 || m_inputDevice.keyBoardBuf[VK_UP] > 64 || vgsScene->softKey[0])
// // 		moveSpeedZ = flyer.panSpeed;
// // 	
// // 	if (m_inputDevice.keyBoardBuf['s'] > 64 || m_inputDevice.keyBoardBuf['S'] > 64 ||  m_inputDevice.keyBoardBuf[VK_DOWN] > 64 || vgsScene->softKey[1])
// // 		moveSpeedZ = -flyer.panSpeed;
// 	if (IsAlphaKeyBuffered(m_AddKeyUp) || m_inputDevice.keyBoardBuf[VK_UP] > 64 || vgsScene->softKey[0])
// 	{
// 		moveSpeedZ = flyer.panSpeed;
// 	}
// 	if (IsAlphaKeyBuffered(m_AddKeyDown) || m_inputDevice.keyBoardBuf[VK_DOWN] > 64 || vgsScene->softKey[0])
// 	{
// 		moveSpeedZ = -flyer.panSpeed;
// 	}
// 	// <==
// 
// 	realMoveSpeedZ = realMoveSpeedZ * flyer.panAcc + moveSpeedZ;
// 	
// 	// fuxb 2009-03-17 ==>
// 	//左右平移
// // 	if (m_inputDevice.keyBoardBuf['a'] > 64 || m_inputDevice.keyBoardBuf['A'] > 64 || m_inputDevice.keyBoardBuf[VK_LEFT] > 64 || vgsScene->softKey[2])
// // 		moveSpeedX = -flyer.panSpeed;
// // 	
// // 	if (m_inputDevice.keyBoardBuf['d'] > 64 || m_inputDevice.keyBoardBuf['D'] > 64 || m_inputDevice.keyBoardBuf[VK_RIGHT] > 64 || vgsScene->softKey[3])
// // 		moveSpeedX = flyer.panSpeed;
// 	if (IsAlphaKeyBuffered(m_AddKeyLeft) || m_inputDevice.keyBoardBuf[VK_LEFT] > 64 || vgsScene->softKey[2])
// 	{
// 		moveSpeedX = -flyer.panSpeed;
// 	}
// 	if (IsAlphaKeyBuffered(m_AddKeyRight) || m_inputDevice.keyBoardBuf[VK_RIGHT] > 64 || vgsScene->softKey[3])
// 	{
// 		moveSpeedX = flyer.panSpeed;
// 	}
// 	// <==
// 	
// 	realMoveSpeedX = realMoveSpeedX * flyer.panAcc + moveSpeedX;
// 	
// 	if (fabs(realMoveSpeedX) > 0.01f || fabs(realMoveSpeedZ) > 0.01f)
// 		vgsScene->currentCam->Translate(&D3DXVECTOR3(realMoveSpeedX, 0.0f, realMoveSpeedZ), 0);
// 	
// 	//默认的鼠标控制----------------------------------------------------------------------------------------------
// 	//按下鼠标左键，左右拖拽鼠标，控制镜头的视角
// 	FLOAT rosStep = 0.004f;
// 	
// 	mouseX = m_inputDevice.mouseInfo.X;
// 	mouseY = m_inputDevice.mouseInfo.Y;
// 	
// 	if (m_inputDevice.mouseInfo.LButton)
// 	{
// 		INT disX = m_inputDevice.mouseInfo.X - mouseX;
// 		INT disZ = m_inputDevice.mouseInfo.Y - mouseY;
// 		
// 		vgsScene->currentCam->Translate(&D3DXVECTOR3(-disX, 0.0f, disZ), 2);
// 		
// 		mouseX = m_inputDevice.mouseInfo.X;
// 		mouseY = m_inputDevice.mouseInfo.Y;   //计算当前鼠标位置和上一帧鼠标位置的距离
// 	}
// 	//鼠标左键松开
// 	else
// 	{
// 		mouseX = m_inputDevice.mouseInfo.X;
// 		mouseY = m_inputDevice.mouseInfo.Y;   //计算当前鼠标位置和上一帧鼠标位置的距离
// 		m_inputDevice.mouseInfo.LButton = FALSE;
// 	}
// 	
// 	/*
// 	//旋转
// 	static FLOAT realRosSpeed = 0.0f;
// 	float rosSpeed = 0.0f, rosAcc = 0.0f;
// 	if (m_inputDevice.keyBoardBuf['a'] > 64 || m_inputDevice.keyBoardBuf['A'] > 64 || m_inputDevice.keyBoardBuf[VK_LEFT] > 64 || vgsScene->softKey[2])
// 	rosSpeed = walker.rosSpeed;
// 	
// 	  if (m_inputDevice.keyBoardBuf['d'] > 64 || m_inputDevice.keyBoardBuf['D'] > 64 || m_inputDevice.keyBoardBuf[VK_RIGHT] > 64 || vgsScene->softKey[3])
// 	  rosSpeed = -walker.rosSpeed;
// 	  
// 		realRosSpeed = realRosSpeed*walker.rosAcc + rosSpeed;
// 		if (fabs(realRosSpeed) > 0.001f)
// 		{
// 		vgsScene->currentCam->Rotate(&D3DXVECTOR3(0.0f, realRosSpeed, 0.0f), 2);
// 		
// 		  //控制软件盘指南针的方向
// 		  if (vgsScene->pRosOl != NULL)
// 		  vgsScene->pRosOl->Rotate(realRosSpeed);
// 		  }
// 	*/
}

//-----------------------------------------------------------------------------
// 方法名: SetPanSpeed()
// 参  数: FLOAT speed, FLOAT acc
// 用  途: GIS控制模式的方法在这里，设置相机平移的平移速度
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SetFlyCtrl_PanParam(FLOAT speed, FLOAT acc)
{
// 	flyer.panSpeed = speed;
// 	flyer.panAcc = acc;
}

//-----------------------------------------------------------------------------
// 方法名: SetDollySpeed()
// 参  数: FLOAT speed, FLOAT acc
// 用  途: GIS控制模式的方法在这里，设置相机推拉速度
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SetFlyCtrl_DollyParam(FLOAT speed, FLOAT acc)
{
// 	flyer.dollySpeed = speed;
// 	flyer.dollyAcc = acc;
}

//循环调用方法, 需要在帧循环中调用。
//frameCnt为到达目标点的总帧数，accType为运动过程中的加速方法
VOID CVGS2DLL::MoveCurrentCameraTo(D3DXVECTOR3* vFrom, D3DXVECTOR3* vAt, INT frameCnt, BOOL accType)
{
// 	if (vgsScene == NULL || vgsScene->currentCam == NULL)
// 		return;
// 	
// 	if (frameCnt > 0) 
// 		bFrameMoveCamera = true;
// 	
// 	//计算帧移动相机的变量
// 	FrameMoveCamera_From = *vFrom;		
// 	FrameMoveCamera_At = *vAt;			
// 	FrameMoveCamera_FrameCnt = frameCnt;	
// 	FrameMoveCamera_FrameIndex = 0;	
// 	FrameMoveCamera_AccMode = accType;	
// 	
// 	//当前相机的位置
// 	D3DXVECTOR3 curCamFrom = vgsScene->currentCam->m_from;
// 	D3DXVECTOR3 curCamAt = vgsScene->currentCam->m_at;
// 	
// 	//与目标点的距离
// 	D3DXVECTOR3 FromDis = *vFrom - curCamFrom;
// 	D3DXVECTOR3 AtDis = *vAt - curCamAt;
// 	
// 	//计算速度&加速度
// 	switch (accType)
// 	{
// 	case 0: //匀速运动
// 		FrameMoveCamera_FromAcc = D3DXVECTOR3(0, 0, 0);	
// 		FrameMoveCamera_AtAcc = D3DXVECTOR3(0, 0, 0);
// 		FrameMoveCamera_FromVel = FromDis/frameCnt;
// 		FrameMoveCamera_AtVel = AtDis/frameCnt;
// 		break;
// 	case 1: //匀加速运动
// 		FrameMoveCamera_FromAcc = 2*FromDis/(frameCnt * (frameCnt-1));	
// 		FrameMoveCamera_AtAcc = 2*AtDis/(frameCnt * (frameCnt-1));
// 		break;
// 	case 2: //匀减速运动
// 		FrameMoveCamera_FromAcc = 2*FromDis/(frameCnt * (frameCnt+1));	
// 		FrameMoveCamera_AtAcc = 2*AtDis/(frameCnt * (frameCnt+1));	
// 		break;
// 	}		
}	

//渲染帧自动调用的移动相机的方法，每帧调用一次
VOID CVGS2DLL::FrameMoveCurrentCamera()
{
// 	if (vgsScene->currentCam == NULL) return;
// 	
// 	if (FrameMoveCamera_FrameIndex < FrameMoveCamera_FrameCnt)
// 	{
// 		//计算速度
// 		switch (FrameMoveCamera_AccMode)
// 		{
// 		case 0: //匀速运动
// 			break;
// 		case 1: //匀加速运动
// 			FrameMoveCamera_FromVel = FrameMoveCamera_FromAcc * FrameMoveCamera_FrameIndex;
// 			FrameMoveCamera_AtVel = FrameMoveCamera_AtAcc * FrameMoveCamera_FrameIndex;
// 			break;
// 		case 2: //匀减速运动
// 			FrameMoveCamera_FromVel = FrameMoveCamera_FromAcc * (FrameMoveCamera_FrameCnt - FrameMoveCamera_FrameIndex);
// 			FrameMoveCamera_AtVel = FrameMoveCamera_AtAcc * (FrameMoveCamera_FrameCnt - FrameMoveCamera_FrameIndex);
// 			break;
// 		}
// 		
// 		vgsScene->currentCam->m_from += FrameMoveCamera_FromVel;
// 		vgsScene->currentCam->SetFrom(&(vgsScene->currentCam->m_from));
// 
// 		vgsScene->currentCam->m_at += FrameMoveCamera_AtVel;
// 		vgsScene->currentCam->SetAt(&(vgsScene->currentCam->m_at));
// 		
// 		vgsScene->currentCam->m_up = D3DXVECTOR3(0, 1, 0);
// 		FrameMoveCamera_FrameIndex++;
// 	}
// 	else
// 		bFrameMoveCamera = false;
}

//-------------------------------------------------------------------------------------------
//***关键帧动画相关的方法
//-------------------------------------------------------------------------------------------
//播放关键帧动画
VOID CVGS2DLL::PlayKeyframe()
{
	m_VGSCore.PlayAllAnimation(m_vgsScene);
	//播放关键帧动画的同时，激活相机、模型、灯光动画
// 	vgsScene->m_bKfrPlaying = TRUE;
// 
// 	//从头开始
// 	vgsScene->m_KfrRepeatIndex = 0;
// 	vgsScene->m_KfrRepeatCnt = -1;
// 	
// 	//相机
// 	for (INT i = 0; i< vgsScene->light.size(); i++)
// 	{
// 		(*(vgsScene->light.begin() + i))->playKfr = TRUE;
// 		(*(vgsScene->light.begin() + i))->kfrRepeatCnt = -1;
// 	}
// 
// 	//模型
// 	for (i = 0; i< vgsScene->model.size(); i++)
// 	{
// 		(*(vgsScene->model.begin() + i))->kfrPlay = TRUE;
// 		(*(vgsScene->model.begin() + i))->kfrRepeatCnt = -1;
// 	}
// 
// 	//灯光
// 	for (i = 0; i< vgsScene->light.size(); i++)
// 	{
// 		(*(vgsScene->light.begin() + i))->playKfr = TRUE;
// 		(*(vgsScene->light.begin() + i))->kfrRepeatCnt = -1;
// 	}

}

//暂停关键帧动画
VOID CVGS2DLL::PauseKeyframe()
{
	// vgsScene->m_bKfrPlaying = FALSE;
}

//停止关键帧动画
VOID CVGS2DLL::StopKeyframe()
{
// 	vgsScene->m_bKfrPlaying = FALSE;
// 	vgsScene->m_CurrentFrame = 0;
}

//得到关键帧动画的帧数
INT CVGS2DLL::GetKeyframeCount()
{
	// return vgsScene->m_KfrCnt;
	return 0;
}

//跳到某一帧
VOID CVGS2DLL::GotoFrame(INT index)
{
// 	if (index < 0) index = 0;
// 	if (index > vgsScene->m_KfrCnt) index = vgsScene->m_KfrCnt;
// 
// 	vgsScene->m_CurrentFrame = index;
}

//得到当前播放的帧
INT CVGS2DLL::GetCurrentFrame()
{
	// 	return vgsScene->m_CurrentFrame;
	return 0;
}

//设置关键帧动画播放的帧率
VOID CVGS2DLL::SetKfrPlaySpeed(INT index)
{
// 	if (index < 0) return;
// 
// 	if (index > vgsScene->fps) 
// 		index = vgsScene->fps;
// 
// 	vgsScene->m_kfrPlayspeed = index;
}

//获得关键帧动画播放的帧率
INT CVGS2DLL::GetKfrPlaySpeed()
{
	// 	return vgsScene->m_kfrPlayspeed;
	return 0;
}

//设置关键帧动画循环播放的次数
VOID CVGS2DLL::SetKfrRepeatCount(INT index)
{
	// vgsScene->m_KfrRepeatCnt = index;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunRenderScene()
// 参  数: 
// 用  途: 渲染整个场景
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunRenderScene()
{	
	//render fps control
 	// if (vgsScene->currentCam == NULL) return;
	DWORD pController = m_VGSCore.GetCurrentController(m_vgsScene);
	CONTROLTYPE controlStatus = m_VGSCore.GetControllerType(pController);
 	//frame control	
 	switch (controlStatus)
 	{
 	case CT_WALKER:
 		WalkControl();
 		break;
 	/*case EDIT:
 		EditControl();
 		break;
 	case FLY:
 		FlyControl();
 		break;*/
 	case CT_NONE:
 		break;
	 	}
	m_VGSCore.RenderAll();
// 
// 	//-------------------------------------------------------------------------------------------------------------
// 	//启动渲染
// 	vgsScene->StartRender();
// 	// vgsScene->m_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, vgsScene->background, 1.0f, 0 ); 
// 	//渲染背景图
// 	vgsScene->RenderBackDrop();
// 	//渲染场景中的模型到后表面
// 	vgsScene->RenderModels();
// 	//停止本帧渲染
// 	vgsScene->EndRender();
// 	
// 	//后期颜色特效处理
// 	if (vgsScene->m_pColorEdit  && vgsScene->m_pColorEdit->m_bSupport && vgsScene->m_pColorEdit->m_bColorEditFlag)
// 		vgsScene->m_pColorEdit->RenderEffect();
// 
// 	//全屏光晕
// 	if (vgsScene->m_pFSGlow  && vgsScene->m_pFSGlow->m_bSupport && vgsScene->m_pFSGlow->m_bFSGlowFlag)
// 		vgsScene->m_pFSGlow->RenderEffect();
// 	
// 	// fuxb 2009-03-23 ==>
// 	//全屏动态模糊
// 	if (vgsScene->m_pFSMotionBlur && vgsScene->m_bFSMotionBlur)
// 	{
// 		// vgsScene->m_pFSMotionBlur->setMatrix(&vgsScene->m_MatWorld, &vgsScene->m_MatView, &vgsScene->m_MatProj);
// 		vgsScene->m_pFSMotionBlur->RenderEffect();
// 	}
// 	// <==	
// 
// 	//渲染其它的对象，这些对象不需要使用后期特效
// 	vgsScene->RenderOthers();
// 
// 	// 呈现到屏幕
// 	vgsScene->m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
// 
// 	//相机侦移动
// 	if (bFrameMoveCamera)
// 	{
// 		FrameMoveCurrentCamera();
// 	}
// 
// 	/*
// 	//音频事件
// 	INT audioCnt = audioPlayer.size();
// 	CAudioPlayer** ppAudio = audioPlayer.begin();
// 	for (INT i = 0; i< audioCnt; i++)
// 	{
// 		CAudioPlayer* pAudio = *(ppAudio + i);
// 		if (pAudio->m_bCreated)
// 			pAudio->CheckAudioStatus();
// 	}
// 	*/
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetBackColor()
// 参  数: BYTE red, BYTE green, BYTE blue
// 用  途: 设置背景色
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetBackColor(BYTE red, BYTE green, BYTE blue)
{
	// vgsScene->SetBackground(red, green, blue);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetRenderFPS()
// 参  数: UINT fps
// 用  途: 设置渲染速度
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetRenderFPS(UINT fps)
{
// 	if(vgsScene == NULL) return;
// 	vgsScene->fps = fps;
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetViewPort()
// 参  数: UINT left, UINT top, UINT width, UINT height
// 用  途: 设置视口
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetViewPort(RECT* rect)
{
	// vgsScene->SetViewPort(rect);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunGetViewPort()
// 参  数: RECT* rect
// 用  途: 得到视口大小
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunGetViewPort(RECT* rect)
{
// 	D3DVIEWPORT9* pViewPort = vgsScene->GetViewPort();
// 	rect->left = pViewPort->X;
// 	rect->top = pViewPort->Y;
// 	rect->right = pViewPort->X + pViewPort->Width;
// 	rect->bottom = pViewPort->Y + pViewPort->Height; 
}

// 获取/设置视口的相机
DWORD CVGS2DLL::SunGetViewportCamera(DWORD vp)
{
	return m_VGSCore.GetViewportCamera(vp);
}
VOID CVGS2DLL::SunSetViewportCamera(DWORD vp, DWORD cam)
{
	m_VGSCore.SetViewportCamera(vp, cam);
}

//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunUpdateBackBuffer()
// 参  数: RECT* rect
// 用  途: 更新后缓冲区
//-----------------------------------------------------------------------------
BOOL CVGS2DLL::SunUpdateBackBuffer()
{
	BOOL re = m_VGSCore.UpdateBackBuffer(m_defaultRenderWnd);
	return re;
}


//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetRenderStatus()
// 参  数: BOOL renderIt
// 用  途: 设置当前场景是否渲染
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetRenderStatus(BOOL renderIt)
{
	m_bRender = renderIt;
	m_VGSCore.SetSceneRenderStatus(m_vgsScene, m_bRender);

// 	if (vgsScene == NULL) return;
// 
// 	if (renderIt > 0)
// 	{
// 		//vgsScene->bActive = TRUE;
// 		vgsScene->renderScene = TRUE;
// 	}
// 	else
// 	{
// 		//vgsScene->bActive = FALSE;
// 		vgsScene->renderScene = FALSE;
// 	}
}


//-------------------------------------------------------------------------------------------
//***特效相关的方法
//-------------------------------------------------------------------------------------------


//创建动态模糊特效对象
DWORD CVGS2DLL::CreateFSMotionBlur()
{
// 	vgsScene->m_pFSMotionBlur = new CFullScreenMotionBlur(vgsScene->m_pd3dDevice,
// 															&vgsScene->rect,
// 															&vgsScene->m_d3dpp,
// 															&vgsScene->m_MatWorld,
// 															&vgsScene->m_MatView,
// 															&vgsScene->m_MatProj);
// 	if (!(vgsScene->m_pFSMotionBlur->InitEffect()))
// 	{
// 		//MessageBox(NULL, "Your system don't support full screen motion blur effect!", "Device support error information", MB_OK);
// 		return NULL;
// 	}
// 	vgsScene->m_bFSMotionBlur = TRUE;
	// 	return vgsScene->m_pFSMotionBlur;
	return 0;
}

//打开动态模糊特效
VOID CVGS2DLL::OpenFSMotionBlur()
{
// 	if (vgsScene->m_pFSMotionBlur)
// 	{
// 		vgsScene->m_pFSMotionBlur->m_bBlurFlag = TRUE;
// 		vgsScene->m_bFSMotionBlur = TRUE;
// 	}
}

//关闭动态模糊特效
VOID CVGS2DLL::CloseFSMotionBlur()
{
// 	if (vgsScene->m_pFSMotionBlur)
// 	{
// 		// vgsScene->m_pFSMotionBlur->m_bBlurFlag = FALSE;
// 		vgsScene->m_pFSMotionBlur->disableBlur();
// 		vgsScene->m_bFSMotionBlur = FALSE;
// 	}
}

//设置动态模糊的强度
VOID CVGS2DLL::SetFSMotionBlurPower(INT value)
{
	// vgsScene->m_pFSMotionBlur->SetBlurAmount(value);
}

//创建全屏光晕对象
DWORD CVGS2DLL::CreateFSGlow()
{
	//判断当前系统是否支持PS2.0
// 	if (vgsScene->d3dParam.SupportPS_Version < 2)
// 	{
// 		::MessageBox(::GetActiveWindow(), "您使用的系统不支持高级全屏光晕特效渲染，该效果将会自动被忽略。", "友情提示", MB_OK);
// 		return NULL;
// 	}
// 
// 	vgsScene->m_pFSGlow = new CFSGlow(vgsScene->m_pd3dDevice,
// 		&vgsScene->rect,
// 		&vgsScene->m_d3dpp,
// 		&vgsScene->m_MatWorld,
// 		&vgsScene->m_MatView,
// 		&vgsScene->m_MatProj);
// 	if(!(vgsScene->m_pFSGlow->InitEffect()))
// 	{
// 		SAFE_RELEASE(vgsScene->m_pFSGlow);
// 		SAFE_DELETE(vgsScene->m_pFSGlow);
// 		return NULL;
// 	}
// 
// 	vgsScene->m_bFSGlow = TRUE;
// 
	// 	return vgsScene->m_pFSGlow;	
	return 0;
}

//打开全屏光晕对象
VOID CVGS2DLL::OpenFSGlow()
{
	// vgsScene->m_pFSGlow->m_bFSGlowFlag = TRUE;
}

//关闭全屏光晕对象
VOID CVGS2DLL::CloseFSGlow()
{
	// vgsScene->m_pFSGlow->m_bFSGlowFlag = FALSE;
}

//设置全屏光晕的强度
VOID CVGS2DLL::SetFSGlowValue(FLOAT BloomValue, FLOAT BlurValue)
{
	// vgsScene->m_pFSGlow->SetGlowValue(BloomValue, BlurValue);
}

//创建LensFlare镜头光斑对象
DWORD	CVGS2DLL::CreateLensFlare(VECTOR3* sunPos)
{
// 	D3DXVECTOR3 tempVector;
// 	tempVector.x = sunPos->x;
// 	tempVector.y = sunPos->y;
// 	tempVector.z = sunPos->z;
// 	
// 	CLensFlare* pFlare = new CLensFlare(vgsScene->m_pd3dDevice,
// 		&vgsScene->rect,
// 		pLensFlare_Sun_Image,
// 		sizeof(pLensFlare_Sun_Image),
// 		pLensFlare_Tex1_Image,
// 		sizeof(pLensFlare_Tex1_Image),
// 		pLensFlare_Tex2_Image,
// 		sizeof(pLensFlare_Tex2_Image),
// 		pLensFlare_Tex3_Image,
// 		sizeof(pLensFlare_Tex3_Image),
// 		&tempVector);
// 	vgsScene->m_pLensFlare = pFlare;
	// 	return pFlare;
	return 0;
}

//fogMode, 0-LINEAR, 1-EXP, 2-EXP2
VOID CVGS2DLL::SunCreateFog(INT fogMode, BYTE red, BYTE green, BYTE blue, FLOAT density, FLOAT start, FLOAT end)
{
	m_VGSCore.CreateFog(m_vgsScene, fogMode, red, green, blue, density, start, end);
}

VOID CVGS2DLL::SunSetFogMode(int mode)
{

}
VOID CVGS2DLL::SunSetFogDensity(FLOAT density)
{

}
VOID CVGS2DLL::SunSetFogFiled(FLOAT start, FLOAT end)
{

}

VOID CVGS2DLL::SunSetFogColor(BYTE red, BYTE green, INT blue)
{

}
VOID CVGS2DLL::SunSetFogEnableStatus(BOOL enable)
{

}
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetControlStatus()
// 参  数: UINT controlEnum
// 用  途: 设置当前用户交互的状态值
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SunSetControlStatus(CONTROLTYPE controlEnum)
{
	switch (controlEnum)
	{
	case CONTROLTYPE::CT_WALKER:
		if (m_VGSCore.GetWalkerCount(m_vgsScene) > 0)
		{
			DWORD pWalker = m_VGSCore.GetWalkerHandleByIndex(m_vgsScene, 0);
			m_VGSCore.SetCurrentController(m_vgsScene, pWalker);
		}
		break;
	case CONTROLTYPE::CT_EDITOR:
		if (m_VGSCore.GetEditorCount(m_vgsScene) > 0)
		{
			DWORD pEditor = m_VGSCore.GetEditorHandleByIndex(m_vgsScene, 0);
			m_VGSCore.SetCurrentController(m_vgsScene, pEditor);
		}
		break;
	case CONTROLTYPE::CT_FLYER:
		if (m_VGSCore.GetFlyerCount(m_vgsScene) > 0)
		{
			DWORD pFlyer = m_VGSCore.GetFlyerHandleByIndex(m_vgsScene, 0);
			m_VGSCore.SetCurrentController(m_vgsScene, pFlyer);
		}
		break;
	default:
		break;
	}
	// controlStatus = controlEnum;
}
CONTROLTYPE CVGS2DLL::SunGetControllerType(DWORD controller)
{
	return m_VGSCore.GetControllerType(controller);
}
//-----------------------------------------------------------------------------
// 方法名: CVGS2DLL::SunSetWalkSpeed()
// 参  数: FLOAT speed
// 用  途: 设置行走的速度
//-----------------------------------------------------------------------------
VOID CVGS2DLL::SetWalkCtrl_MoveParam(FLOAT speed, FLOAT acc)
{
// 	if (vgsScene == NULL) return;
// 	
// 	walker.walkSpeed = speed;
// 	walker.walkAcc = acc;
}

// DWORD CVGS2DLL::CreateOverlayFromFile()
// {
// 
// }
DWORD CVGS2DLL::CreateOverlayFromMemory(RECT* pRect, const CHAR* overlayName, const CHAR* overlayTexName, const BYTE* pData ,UINT dataSize)
{
	//create  Overlay
// 	COverlay* pOverlay = new COverlay(vgsScene->m_pd3dDevice);
// 	if (pOverlay == NULL) 
// 		return 0;
// 	
// 	//vgsScene->m_pStartLogo->UpdateRect(&(vgsScene->rect));
// 	pOverlay->SetRect(pRect);
// 	strcpy(pOverlay->name, overlayName);
// 	//vgsScene->m_pStartLogo->bIsFilter = TRUE;
// 	
// 	CMaterial* pMtrl = new CMaterial(vgsScene->m_pd3dDevice);
// 	pMtrl->bIsOverlayMtrl = TRUE;
// 	
//     TextureLayer* pTexLayer = new TextureLayer();
// 	
// 	//ZeroMemory(pTexLayer->Name,sizeof(pTexLayer->Name));
// 	pTexLayer->Name =std::string(overlayTexName);
// 	
// 	pTexLayer->type = TLT_BASAL;
// 	
// 	pTexLayer->UVIndex = 0;
// 	
// 	pTexLayer->Blenderinfo.AlphaBlendOption = D3DTOP_MODULATE;
//     pTexLayer->Blenderinfo.ColorBlendOption = D3DTOP_MODULATE;
// 	
// 	CTexture* pTex = new CTexture();
// 	pTex->BP = D3DFMT_A8R8G8B8;
// 	pTex->width = pRect->right - pRect->left;
// 	pTex->height = pRect->bottom - pRect->top;
// 	pTex->mipCount = 1;
// 	
//     CImage* pImage = new CImage();
// 	pImage->index = vgsScene->image.size();
// 	pImage->dataLength = dataSize;
// 	pImage->data = new BYTE[pImage->dataLength];
// 	
// 	memcpy(pImage->data, pData, pImage->dataLength);
// 	pTex->imageIdx = pImage->index;
// 	vgsScene->image.push_back(pImage);
// 	
// 	pTex->bAlphaBlend =TRUE;
// 	pTexLayer->pTexture =(DWORD)pTex;
// 	
// 	if (FAILED( vgsScene->CreateTexFromBuffer(pTex, pImage->dataLength, pImage->data)))
// 	{
// 		SAFE_DELETE(pTex);
// 		SAFE_DELETE(pTexLayer);
// 		return 0;
// 	}
// 	pMtrl->bAlphaBlend = TRUE;
//     pMtrl->texLayersList.push_back(pTexLayer);
//     //清除图像对象
// 	vgsScene->image.pop_back();
// 	SAFE_RELEASE(pImage);
// 	SAFE_DELETE(pImage);
// 	
// 	pOverlay->SetMaterial(pMtrl);
	// 	return pOverlay;
	return 0;
}
//-----------------------------------------------------------------------------
// 方法名: CreateLogo
// 参  数: 
// 用  途: 创建Logo
//-----------------------------------------------------------------------------
BOOL CVGS2DLL::CreateLogo()
{

// 	RECT OverlayRect;
// 	OverlayRect.left = vgsScene->rect.left + (vgsScene->rect.right-vgsScene->rect.left )/2 - 48;
// 	OverlayRect.top =  vgsScene->rect.top + (vgsScene->rect.bottom - vgsScene->rect.top)/2 - 48;//-8 ;
// 	OverlayRect.right = OverlayRect.left + 96;
// 	OverlayRect.bottom = OverlayRect.top + 96;
// 	
//     UINT dataSize = sizeof(pLogoImage);
//     //create Suntoward Logo Overlay
//     vgsScene->m_pStartLogo = CreateOverlayFromMemory(&OverlayRect, "VGSLogo","VGSLogoTex", pLogoImage, dataSize);
// 
// 	OverlayRect.left = vgsScene->rect.left + (vgsScene->rect.right-vgsScene->rect.left )/2 - 64;
// 	OverlayRect.top =  vgsScene->rect.top + (vgsScene->rect.bottom - vgsScene->rect.top)/2 - 64 - 8;
//     OverlayRect.right = OverlayRect.left + 128;
// 	OverlayRect.bottom = OverlayRect.top + 8;
// 
//     dataSize = sizeof(pLoadingBgImage);
//     //create loading background Overlay
//     vgsScene->m_pLoadingBg = CreateOverlayFromMemory(&OverlayRect, "VGSLodingBg","VGSLodingBgTex", pLoadingBgImage ,dataSize);
// 
// 	OverlayRect.left = vgsScene->rect.left + (vgsScene->rect.right-vgsScene->rect.left )/2 - 64;
// 	OverlayRect.top =  vgsScene->rect.top + (vgsScene->rect.bottom - vgsScene->rect.top)/2 - 64 - 8;
//     OverlayRect.right = OverlayRect.left;
// 	OverlayRect.bottom = OverlayRect.top + 8;
// 
// 	dataSize = sizeof(pLoadingBarImage);
//     //create loading background Overlay
//     vgsScene->m_pLoadingBar = CreateOverlayFromMemory(&OverlayRect, "VGSLodingBar","VGSLodingBarTex", pLoadingBarImage ,dataSize);

	  return TRUE;
}

//-----------------------------------------------------------------------------
// 方法名: DeleteLogo
// 参  数: 
// 用  途: 删除Logo
//-----------------------------------------------------------------------------
VOID CVGS2DLL::DeleteLogo()
{

// 	if (vgsScene && vgsScene->m_pStartLogo)
// 	{	
//         TextureLayer* pTexLayer =vgsScene->m_pStartLogo->m_pMtrl->texLayersList[0];
// 		CTexture* pTex =(CTexture*)pTexLayer->pTexture;       
// 	    SAFE_RELEASE(pTex);
// 		SAFE_DELETE(pTex);
// 		SAFE_RELEASE(vgsScene->m_pStartLogo->m_pMtrl);
// 		SAFE_DELETE(vgsScene->m_pStartLogo->m_pMtrl);
// 
// 		SAFE_RELEASE(vgsScene->m_pStartLogo);
// 		SAFE_DELETE(vgsScene->m_pStartLogo);
// 	}
// 
// 	if (vgsScene && vgsScene->m_pLoadingBg)
// 	{	
//         TextureLayer* pTexLayer =vgsScene->m_pLoadingBg->m_pMtrl->texLayersList[0];
// 		CTexture* pTex =(CTexture*)pTexLayer->pTexture;       
// 	    SAFE_RELEASE(pTex);
// 		SAFE_DELETE(pTex);
// 		SAFE_RELEASE(vgsScene->m_pLoadingBg->m_pMtrl);
// 		SAFE_DELETE(vgsScene->m_pLoadingBg->m_pMtrl);
// 
// 		SAFE_RELEASE(vgsScene->m_pLoadingBg);
// 		SAFE_DELETE(vgsScene->m_pLoadingBg);
// 	}
// 
// 	if (vgsScene && vgsScene->m_pLoadingBar)
// 	{	
//         TextureLayer* pTexLayer =vgsScene->m_pLoadingBar->m_pMtrl->texLayersList[0];
// 		CTexture* pTex =(CTexture*)pTexLayer->pTexture;       
// 	    SAFE_RELEASE(pTex);
// 		SAFE_DELETE(pTex);
// 		SAFE_RELEASE(vgsScene->m_pLoadingBar->m_pMtrl);
// 		SAFE_DELETE(vgsScene->m_pLoadingBar->m_pMtrl);
// 
// 		SAFE_RELEASE(vgsScene->m_pLoadingBar);
// 		SAFE_DELETE(vgsScene->m_pLoadingBar);
// 	}
}

//Invalidate Scene
VOID CVGS2DLL::InvalidateScene()
{
// 	if (vgsScene)
// 		vgsScene->InvalidateDeviceObject();
}

//Restore Scene
VOID CVGS2DLL::RestoreScene()
{
// 	if (vgsScene)
// 		vgsScene->RestoreDeviceObject();
}

//设置抗拒齿的级别，
//当Level等于0，则没有抗拒齿，
//当Level等于-1，则中度抗拒齿（默认情形）
//当Level大于0，则根据不同的系统选择抗拒齿的参数
VOID CVGS2DLL::SetAntiAliasLevel(INT level)
{
// 	if(vgsScene)
// 	{
// 		vgsScene->InvalidateDeviceObject();
// 
// 		//选择级别
// 		INT antiAliasLevelCnt = 
// 			vgsScene->m_DeviceInfo->pAdapters[0].MultiSampleTypeCnt;
// 		if (level > antiAliasLevelCnt -1)
// 			level = antiAliasLevelCnt - 1;
// 		if (level < 0)
// 			level = (antiAliasLevelCnt + 1)/2 -1;
// 
// 		//AntiAlias type
// 		vgsScene->d3dParam.MultiSampleType = 
// 			vgsScene->m_DeviceInfo->pAdapters[0].pMultiSampleType[level];
// 
// 		vgsScene->RestoreDeviceObject();
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: CreateSceneResource
// 参  数: std::string sceneResourceName - 场景资源对象的名称
		 //D3DXMATRIX* pTransform - 场景的变换矩阵
// 用  途: 创建场景资源对象
//-----------------------------------------------------------------------------
//DWORD CVGS2DLL::CreateSceneResource(std::string sceneResourceName, D3DXMATRIX* pTransform)
//{
// 	CSceneResource* pSceneResource = new CSceneResource(sceneResourceName, vgsScene);
// 	pSceneResource->m_transform = *(pTransform);
// 	sceneResource.push_back(pSceneResource);
// 	return pSceneResource;
//}

//-----------------------------------------------------------------------------
// 方法名: DeleteSceneResource
// 参  数: CSceneResource* pSceneResource - 场景资源对象指针
// 用  途: 删除指定场景资源对象，也删除了资源本身
//-----------------------------------------------------------------------------
//void CVGS2DLL::DeleteSceneResource(DWORD pSceneResource)
//{
// 	if (pSceneResource == NULL) return;
// 
// 	int sceneResourceCnt = sceneResource.size();
// 	CSceneResource** ppSceneResource = sceneResource.begin();
// 	for (int i = 0; i< sceneResourceCnt; i++)
// 	{
// 		CSceneResource* _pSceneResource = *(ppSceneResource + i);
// 		if (_pSceneResource == pSceneResource)
// 		{
// 			sceneResource.erase(ppSceneResource + i);
// 			SAFE_RELEASE(pSceneResource);
// 			SAFE_DELETE(pSceneResource);
// 			return;
// 		}
// 	}
//}

//-----------------------------------------------------------------------------
// 方法名: GetSceneResource
// 参  数: std::string sceneResourceName -场景资源对象的名字
// 用  途: 找到指定的场景资源对象
//-----------------------------------------------------------------------------
//DWORD CVGS2DLL::GetSceneResource(std::string sceneResourceName)
//{
// 	int sceneResourceCnt = sceneResource.size();
// 	CSceneResource** ppSceneResource = sceneResource.begin();
// 	for (int i = 0; i< sceneResourceCnt; i++)
// 	{
// 		CSceneResource* pSceneResource = *(ppSceneResource + i);
// 		if (strcmp(pSceneResource->name.c_str(), sceneResourceName.c_str()) == 0)
// 			return pSceneResource;
// 	}
//	return NULL;
//}
//-----------------------------------------------------------------------------
// 方法名: CloneScene
// 参  数: std::string destSceneName, 目标场景的名称
//			std::string srcSceneName, 原场景的名称
//			std::string parentSceneName, 父级场景的名称
//			D3DXMatrix* pTransformMat, 相对父级场景的空间变换
// 用  途: 克隆指定的场景
//-----------------------------------------------------------------------------
//DWORD CVGS2DLL::CloneScene(std::string destSceneName, std::string srcSceneName, 
//									 std::string parentSceneName, D3DXMATRIX* pTransformMat)

//{
// 	CSceneResource* pSceneResource = GetSceneResource(srcSceneName);
// 	if (pSceneResource == NULL) return NULL;
// 
// 	CSceneResource* pNewSceneResource = pSceneResource->Clone(destSceneName);
// 	sceneResource.push_back(pNewSceneResource);
// 
// 
// 	//设置变换
// 	pNewSceneResource->m_pRootNode->curTransform.worldMatrix = *pTransformMat;
// 	pNewSceneResource->m_pRootNode->updateTransform();
//     //更新模型的信息
// 	UINT modelCnt = pNewSceneResource->model.size();
// 	CModel** ppModel = pNewSceneResource->model.begin();
// 	for(UINT i = 0; i< modelCnt; i++)
// 	{
// 		CModel* pModel =*(ppModel + i);
// 		pModel->update();
// 	}
// 	return pNewSceneResource;
//	return 0;
//}


//---------------------------------------------------------------------------
//最新的水特效
//---------------------------------------------------------------------------
VOID CVGS2DLL::CreateRealWater(CHAR* name, FLOAT x, FLOAT y, FLOAT z, INT gridX, INT gridY)
{
	//判断当前系统是否支持PS2.0
// 	if (vgsScene->d3dParam.SupportPS_Version < 2 || vgsScene->d3dParam.SupportVS_Version < 1.1f)
// 	{
// 		::MessageBox(::GetActiveWindow(), "您的电脑系统不支持高级水面特效渲染，无法看到水面效果。", "友情提示", MB_OK);
// 		return;
// 	}
//     BOOL supportGPU = vgsScene->d3dParam.IsSupportTex64 && vgsScene->d3dParam.IsSupportGPUNormal; 
// 
// 	CRealWater* pRealWater = new CRealWater(name);
// 	pRealWater->Init(vgsScene->m_pd3dDevice, 
// 					vgsScene->currentCam, 
// 					&D3DXVECTOR3(x, y, z),
// 					gridX, 
// 					gridY,
// 					supportGPU);
// 	vgsScene->realWater.push_back(pRealWater);
// 
// 	//提示用户 
// 	if(supportGPU == FALSE)
// 	{
// 		::MessageBox(::GetActiveWindow(), "您的电脑系统不支持高级水面特效渲染，无法看到理想的水面渲染效果，并可能会导致运行速度较慢。", 
// 			"友情提示", MB_OK);
// 	}
}

//通过水面名称获得对象
DWORD CVGS2DLL::GetRealWaterHandleByName(CHAR* name)
{
// 	int realWaterCnt = vgsScene->realWater.size();
// 	CRealWater** ppRealWater = vgsScene->realWater.begin();
// 	for (int i = 0; i< realWaterCnt; i++)
// 	{
// 		CRealWater* pRealWater = *(ppRealWater + i);
// 		if (strcmp(pRealWater->m_Name.c_str(), name) == 0)
// 			return (DWORD)pRealWater;
// 	}
	return 0;
}

//增加水面的折射物体
VOID CVGS2DLL::AddRealWaterRTTModel(DWORD realWaterHandle, DWORD modelHandle)
{
// 	CRealWater* pRealWater = (CRealWater*)realWaterHandle;
// 	CModel* pModel = (CModel*)modelHandle;
// 	if (pRealWater == NULL || pModel == NULL)
// 		return;

	// pRealWater->addRTTModel(pModel);
}

//增加水面的反射物体
VOID CVGS2DLL::AddRealWaterRTFModel(DWORD realWaterHandle, DWORD modelHandle)
{
// 	CRealWater* pRealWater = (CRealWater*)realWaterHandle;
// 	CModel* pModel = (CModel*)modelHandle;
// 	if (pRealWater == NULL || pModel == NULL)
// 		return;
// 
// 	pRealWater->addRTFModel(pModel);
}

//设置水面用到的CubeMap反射图
VOID CVGS2DLL::SetRealWaterCubeMap(DWORD realWaterHandle, DWORD cubeMapHandle)
{
// 	CRealWater* pRealWater = (CRealWater*)realWaterHandle;
// 	CCubeMap* pCubeMap = (CCubeMap*)cubeMapHandle;
// 	if (pRealWater == NULL || pCubeMap == NULL)
// 		return;
// 
// 	pRealWater->m_pSea->sky_cubemap = pCubeMap->m_pCubeTex;
}

//---------------------------------------------------------------------------
//实时反射和折射
//---------------------------------------------------------------------------
//创建RTF贴图对象
DWORD CVGS2DLL::CreateRTFTexture(CHAR* name, CHAR* refPlaneModel, INT width, INT height, FLOAT power)
{
	//获得模型句柄
// 	DWORD modelHandle = SunGetModelHandleByName(refPlaneModel);
// 	if (modelHandle == 0) return 0;
// 
// 	//创建一个RTFMap
// 	CRTFMap* pRTFMap = new CRTFMap(std::string(name), vgsScene->m_pd3dDevice, &vgsScene->m_d3dpp, width, height, D3DFMT_R8G8B8, power);
// 	pRTFMap->SetRefPlaneByModel(modelHandle);
// 	pRTFMap->m_texProjMatrix = vgsScene->GetTextureProjMatrix();
// 	
// 	//放入RTFMap贴图队列
// 	vgsScene->rtfTexture.push_back(pRTFMap);
	// 	return (DWORD)pRTFMap;
	return 0;
}

//通过RTF贴图的名字获得对象
DWORD CVGS2DLL::GetRTFTextureHandleByName(CHAR* RTFTextureName)
{
// 	INT rtfTexCnt = vgsScene->rtfTexture.size();
// 	CRTFMap** ppRTFMap = vgsScene->rtfTexture.begin();
// 	for (INT i = 0; i< rtfTexCnt; i++)
// 	{
// 		CRTFMap* pRTFMap = *(ppRTFMap + i);
// 		if (strcmp(pRTFMap->m_name.c_str(), RTFTextureName) == 0)
// 			return (DWORD)pRTFMap;
// 	}
	return 0;
}

//为RTF贴图对象增加需要反射的模型
VOID CVGS2DLL::AddModelToRTFTexture(CHAR* RTFTextureName, CHAR* modelName)
{	
// 	DWORD modelHandle = SunGetModelHandleByName(modelName);
// 	if (modelHandle == 0) return;
// 
// 	DWORD RTFTexHandle = GetRTFTextureHandleByName(RTFTextureName);
// 	CRTFMap* pRTFMap = (CRTFMap*)RTFTexHandle;
// 	if (pRTFMap)
// 	{
// 		//将模型加入到反射贴图的反射模型队列
// 		pRTFMap->AddRefModel(modelHandle);
// 
// 		//该模型需要渲染到反射贴图表面，所以增加附加渲染信息
// 		ADDITIONALRENDERINFO renderInfo;
// 		renderInfo.type = ADS_RTFMAP;
// 		renderInfo.pRenderToTexture = RTFTexHandle;
// 		
// 		//放入模型的附加渲染信息队列
// 		CModel* pModel = (CModel*)modelHandle;
// 		pModel->m_additionalRenderInfo.push_back(renderInfo);
// 	}
}

//---------------------------------------------------------------------------
//最新的画面后期特效
//---------------------------------------------------------------------------
//创建颜色编辑器
VOID CVGS2DLL::CreateColorEffect()
{
	//判断当前系统是否支持PS2.0
// 	if (vgsScene->d3dParam.SupportPS_Version < 2)
// 	{
// 		::MessageBox(::GetActiveWindow(), "您的电脑系统不支持后期颜色特效渲染，可能会导致画面效果失真。", "友情提示", MB_OK);
// 		return;
// 	}
// 
// 	vgsScene->m_pColorEdit = new CColorEdit(vgsScene->m_pd3dDevice,
// 											&vgsScene->rect,
// 											&vgsScene->m_d3dpp,
// 											&vgsScene->m_MatWorld,
// 											&vgsScene->m_MatView,
// 											&vgsScene->m_MatProj);
// 	
// 	if(!(vgsScene->m_pColorEdit->InitEffect()))
// 	{
// 		SAFE_RELEASE(vgsScene->m_pColorEdit);
// 		SAFE_DELETE(vgsScene->m_pColorEdit);
// 		return;
// 	}
// 	vgsScene->m_bColorEdit = TRUE;
}

//设置画面的亮度，对比度，颜色调整
VOID CVGS2DLL::SetColorEffect(FLOAT brightness, FLOAT contrast, FLOAT red, FLOAT green, FLOAT blue)
{
// 	if (!vgsScene->m_pColorEdit)
// 		return;
// 
// 	vgsScene->m_pColorEdit->SetColorEditValue(brightness, contrast, red, green, blue);
}

//---------------------------------------------------------------------------
//粒子系统
//---------------------------------------------------------------------------
//创建粒子系统
DWORD CVGS2DLL::CreateParticles(CHAR* name)
{
// 	CParticleEmitter* pParticle = new CParticleEmitter(vgsScene->m_pd3dDevice);
// 	pParticle->m_Name = std::string(name);
// 	vgsScene->m_Particles.push_back(pParticle);
	// 	return (DWORD)pParticle;
	return 0;
}


// 设置第二套控制键是否可用"aswd"
VOID CVGS2DLL::SetAddtionalKeysStatus(BOOL bEnable)
{
// 	m_bAddtionalKeysEnable = bEnable;
}

// 设置第二套控制键
VOID CVGS2DLL::SetAddtionalKeys(BYTE left, BYTE right, BYTE up, BYTE down)
{

// 宏 把keySrc转换成小写，赋值给keyDest
// #define _CONVERTTOLOWCASE(keySrc, keyDest)\
// if (keySrc >= 0 && keySrc < 256)\
// {\
// 	if (keySrc >= 'A' && keySrc <= 'Z')\
// 	{\
// 		keySrc += 32; \
// 	}\
// 	keyDest = keySrc;\
// }
// 
// 	_CONVERTTOLOWCASE(left, m_AddKeyLeft)
// 	_CONVERTTOLOWCASE(right, m_AddKeyRight)
// 	_CONVERTTOLOWCASE(up, m_AddKeyUp)
// 	_CONVERTTOLOWCASE(down, m_AddKeyDown)
}

// 某个字符键是否被触发
BOOL CVGS2DLL::IsAlphabetKeyBuffered(BYTE key, BOOL caseSensitive/* = FALSE*/)
{
// 	if (!m_bAddtionalKeysEnable)
// 	{
// 		return FALSE;
// 	}
// 
// 	if (key >= 0 && key < 256)
// 	{
// 		if (m_inputDevice.keyBoardBuf[key] > 64)
// 		{
// 			return TRUE;
// 		}
// 		
// 		if (!caseSensitive)
// 		{
// 			bool bIsAlpha = FALSE;
// 			if (key >= 'A' && key <= 'Z')
// 			{
// 				key += 32; // 转换成小写
// 				bIsAlpha = TRUE;
// 			}
// 			else if (key >= 'a' && key <= 'z')
// 			{
// 				key -= 32;  // 转换成大写
// 				bIsAlpha = TRUE;
// 			}
// 			if (bIsAlpha && m_inputDevice.keyBoardBuf[key] > 64)
// 			{
// 				return TRUE;
// 			}
// 		}
// 	}
	return FALSE;	
}

/////////////////////////////// 数据相关 ///////////////////////////////////////////

// 从buffer中读取场景信息
BOOL CVGS2DLL::SunLoadSceneFromBuffer(DWORD version, const BYTE* pBuffer)
{
	if (0 == m_VGSCore.LoadSceneFromBuffer(m_vgsScene, version, pBuffer, m_hwnd))
	{
		// 获取当前控制对象信息
		GetCurrentControllerInfo();

		return TRUE;
	}
	return FALSE;
}
// 从buffer中读取图像信息
BOOL CVGS2DLL::SunLoadImageDataFromBuffer(DWORD version, const BYTE* pBuffer)
{
	if (0 == m_VGSCore.LoadImageDataFromBuffer(m_vgsScene, version, pBuffer, m_hwnd))
	{
		return TRUE;
	}
	return FALSE;
}

// 由一张图片的buffer创建纹理
BOOL CVGS2DLL::SunCreateImageTextureFromBuffer(const BYTE* pBuffer, DWORD len, int imgIdx)
{
	if (0 == m_VGSCore.CreateImageTextureFromBuffer(m_vgsScene, pBuffer, len, imgIdx, m_hwnd))
	{
		return TRUE;
	}
	return FALSE;
}

//-------------------------------------------------------------------------------------------
//***控制相关的方法
//-------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: WalkControl()
// 参  数: 
// 用  途: 用户控制行走的方法在这里
//-----------------------------------------------------------------------------
VOID CVGS2DLL::WalkControl()
{
	DWORD curCamera = m_VGSCore.GetCurrentControllerCamera(m_vgsScene);  // 当前使用的相机

	VECTOR3 vCamDir, vCamUp, vCamRight, v3CamPosition, v3CamAt;   //  相机的三个参数
	m_VGSCore.GetCameraDirection(m_vgsScene, curCamera, vCamDir);
	m_VGSCore.GetCameraUp(m_vgsScene, curCamera, vCamUp);
	m_VGSCore.GetCameraRight(m_vgsScene, curCamera, vCamRight);	
	m_VGSCore.GetCameraPosition(m_vgsScene, curCamera, &v3CamPosition);
	m_VGSCore.GetCameraAt(m_vgsScene, curCamera, &v3CamAt);


	GetCurrentControllerInfo();  // 获取当前控制对象的信息
	
	//默认的行走控制----------------------------------------------------------------
	//前后走动
	float moveAcc = 0.0f;

	//行进矢量
	VECTOR3 pWalkVec = VECTOR3(0,0,0);
	VECTOR3 pWalkRay = VECTOR3(0,0,0);

	::GetKeyboardState(m_inputDevice.keyBoardBuf);  // 当前的鼠标键盘控制信息

	//----------------------------------------------------------------------
	//移动Walker
	//----------------------------------------------------------------------
	//得到速度值，有阻尼效果-----------------------------------------
	// if (bCanMove)
	{
		//***向前移动
		if (m_inputDevice.keyBoardBuf[VK_UP] > 64 || /*vgsScene->softKey[0] ||*/ IsAlphaKeyBuffered(m_AddKeyUp))
		{
			pWalkVec.z += m_curWalkerInfo.walkSpeed;
			pWalkRay += vCamDir;
		}
		//***向后移动
		if (IsAlphaKeyBuffered(m_AddKeyDown) ||
			m_inputDevice.keyBoardBuf[VK_DOWN] > 64 /*|| 
			vgsScene->softKey[1]*/)
		{
			pWalkVec.z += -m_curWalkerInfo.walkSpeed;
			pWalkRay   = pWalkRay - vCamDir;
		}
		//***向左移动
		if (IsAlphaKeyBuffered(m_AddKeyLeft) ||
			m_inputDevice.keyBoardBuf[VK_LEFT] > 64/* || 
			vgsScene->softKey[2]*/)
		{
			pWalkVec.x += -m_curWalkerInfo.walkSpeed;
			pWalkRay   += -vCamRight;
		}
		//***向右移动
		if (IsAlphaKeyBuffered(m_AddKeyRight) ||  
			m_inputDevice.keyBoardBuf[VK_RIGHT] > 64/* || 
			vgsScene->softKey[3]*/)
		{
			pWalkVec.x += m_curWalkerInfo.walkSpeed;		
			pWalkRay   += vCamRight;
		}
	}

	// if (bCanLift)
	{
		//***向上移动
		if (m_inputDevice.keyBoardBuf[VK_PRIOR] > 64 /*||
			vgsScene->softKey[8]*/)
		{
			pWalkVec.y += m_curWalkerInfo.walkSpeed;
			pWalkRay   += vCamUp;
		}
		//***向下移动
		if (m_inputDevice.keyBoardBuf[VK_NEXT] > 64/* ||
			vgsScene->softKey[9]*/)
		{
			pWalkVec.y += -m_curWalkerInfo.walkSpeed;
			pWalkRay   -= vCamUp;
		}
	}

	//行进矢量
	walker_realMoveVec = walker_realMoveVec * m_curWalkerInfo.walkAcc + pWalkVec;
	// pWalkRay = vgsScene->currentCam->m_forward;
	float moveStep = D3DXVec3Length(&D3DXVECTOR3(walker_realMoveVec.x, walker_realMoveVec.y, walker_realMoveVec.z));

	//-----------------------------------------竖直方向的碰撞检测，作为重力------------------------------------
	//if (bWeight)
	//{
	//	if (moveStep > 0.0001f)
	//	{
	//		D3DXVECTOR3 pWeightRay = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	//		DWORD curModelHandle = 0;
	//		D3DXVECTOR3 colNormal;
	//		FLOAT distance;
	//		preGroundModel = curGroundModel;
	//		D3DXVECTOR3 camPos = D3DXVECTOR3(v3CamPosition.x, v3CamPosition.y, v3CamPosition.z);
	//		//如果有碰撞，返回碰撞参数，去掉与碰撞面垂直的分量，计算行走矢量
	//		if (vgsScene->DetectGround(&camPos,
	//			&pWeightRay,&curModelHandle,&colNormal, &distance))
	//		{
	//			FLOAT downDis = distance - m_curWalkerInfo.height;
	//			vgsScene->currentCam->Translate(&D3DXVECTOR3(0.0f, -downDis, 0.0f), 0);
	//			//当前地面踏板

	//			curGroundModel = (CModel*)curModelHandle;
	//		}
	//		else
	//			curGroundModel = NULL;

	//		INT triggerModelCnt = vgsScene->triggerModel.size();
	//		if(triggerModelCnt>0)
	//		{
	//			TriggerCallBack** ppTriggerModel = vgsScene->triggerModel.begin();
	//			for (INT i = 0; i< triggerModelCnt; i++)
	//			{
	//				TriggerCallBack* pTriggerModel = *(ppTriggerModel + i);

	//				if (pTriggerModel->triggerHandle == (DWORD)curGroundModel && curGroundModel!=NULL && curGroundModel!=preGroundModel)
	//				{	
	//					//JS 函数回调处理
	//					if (pTriggerModel->callBackFunc)
	//						PostMessage(vgsScene->m_hWnd, WM_ZTRIGFUNC_PROC, (DWORD)pTriggerModel, i);						
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}
	//else
	//	curGroundModel = NULL;

	//----------------------------------------------------------------------
	//推拉Walker镜头
	//----------------------------------------------------------------------
	if (moveStep < 0.0001f/* && bCanZoom*/)  // 移动的时候不推拉镜头
	{
		//镜头拉远
		if (m_inputDevice.keyBoardBuf[VK_END] > 64/* || 
			vgsScene->softKey[11]*/)
		{
			//判断是否超过了DollyNearDis和DollyFarDis
			D3DXVECTOR3 dis = D3DXVECTOR3(v3CamAt.x - v3CamPosition.x, v3CamAt.y - v3CamPosition.y, v3CamAt.z - v3CamPosition.z);
			FLOAT len = D3DXVec3Length(&dis);
			FLOAT dollySpeed = 0.03f * len;
			// FLOAT dollyValue = -dollySpeed * fZoomSpeed;
			FLOAT dollyValue = -max(2.0, min(1000.0, dollySpeed /** fZoomSpeed*/));

			FLOAT dollyNearDis; //=  vgsScene->currentCam->m_nearClip;
			FLOAT dollyFarDis;// = vgsScene->currentCam->m_farClip;
			m_VGSCore.GetCameraClip(m_vgsScene, curCamera, dollyNearDis, dollyFarDis);
			//FLOAT maxViewDis = dollyFarDis - dollyNearDis;

			if (len > dollyNearDis + dollyValue)
			{
				if (len < dollyFarDis + dollyValue)
				{
					//D3DXVECTOR3 newPos = from - dis * dollyValue;
					//CModel* pModel = NULL;
					//D3DXVECTOR3 colNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//FLOAT distance = 1000000.0f;
					//D3DXVECTOR3 direction = -dis;
					//if (!vgsScene->DetectCollision(&newPos, &direction, pModel, &colNormal, &distance))
					{
						m_VGSCore.DollyCamera(m_vgsScene, curCamera, dollyValue);
						// vgsScene->currentCam->Dolly(dollyValue);
					}
				}
			}
		}
		// 镜头推进
		if (m_inputDevice.keyBoardBuf[VK_HOME] > 64 /*|| 
			vgsScene->softKey[10]*/)
		{
			//判断是否超过了DollyNearDis和DollyFarDis
			VECTOR3 from;
			VECTOR3 at;
			m_VGSCore.GetCameraPosition(m_vgsScene, curCamera, &from);
			m_VGSCore.GetCameraAt(m_vgsScene, curCamera, &at);
			D3DXVECTOR3 dis = D3DXVECTOR3(at.x - from.x, at.y - from.y, at.z - from.z);
			
			FLOAT len = D3DXVec3Length(&dis);
			FLOAT dollySpeed = 0.03f * len;
			FLOAT dollyValue = max(2.0, min(1000.0, dollySpeed/* * fZoomSpeed*/));

			FLOAT dollyNearDis;
			FLOAT dollyFarDis;
			m_VGSCore.GetCameraClip(m_vgsScene, curCamera, dollyNearDis, dollyFarDis);

			if (len > dollyNearDis + dollyValue)
			{
				if (len < dollyFarDis + dollyValue)
				{
					//D3DXVECTOR3 newPos = from + dis * dollyValue;
					//CModel* pModel = NULL;
					//D3DXVECTOR3 colNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//FLOAT distance = 1000000.0f;
					//D3DXVECTOR3 direction = dis;
					//if (!vgsScene->DetectCollision(&newPos, &direction, pModel, &colNormal, &distance))
					{
						m_VGSCore.DollyCamera(m_vgsScene, curCamera, dollyValue);
						//vgsScene->currentCam->Dolly(dollyValue);
					}
				}
			}
		}
	}

	////-----------------------------------------水平方向的碰撞检测--------------------------------------------
	////如果系统允许碰撞检测
	//if ( bCollision)
	//{
	//	if (moveStep > 0.0001f)
	//	{
	//		CModel* pModel = NULL;
	//		D3DXVECTOR3 colNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//		FLOAT distance = 1000000.0f;
	//		D3DXVECTOR3 camPos = D3DXVECTOR3(vgsScene->currentCam->m_from.x, 
	//			vgsScene->currentCam->m_from.y, 
	//			vgsScene->currentCam->m_from.z);
	//		// fuxb 2009-03-03 ==>
	//		// pWalkRay.y = 0.0f;
	//		D3DXVec3Normalize(&pWalkRay, &pWalkRay);
	//		D3DXVECTOR3 destCamPos = camPos + pWalkRay * moveStep;  // 计算下一步最远到达的距离，以进行碰撞侦测
	//		// <==

	//		//如果有碰撞，返回碰撞参数，去掉与碰撞面垂直的分量，计算行走矢量		
	//		if (vgsScene->DetectCollision(&destCamPos,
	//			&pWalkRay, pModel, &colNormal, &distance))
	//		{
	//			pWalkRay.y = 0.0f;
	//			D3DXVec3Normalize(&pWalkRay, &pWalkRay);
	//			colNormal.y = 0.0f;
	//			D3DXVec3Normalize(&colNormal, &colNormal);

	//			FLOAT cosAngle = D3DXVec3Dot(&pWalkRay, &(-colNormal));			
	//			D3DXVECTOR3 vWalkAlong = (pWalkRay + cosAngle * colNormal);
	//			D3DXVec3Normalize(&vWalkAlong, &vWalkAlong);
	//			vWalkAlong.y = 0.0f;
	//			vWalkAlong *= moveStep;
	//			bIsCollision = TRUE; 
	//			walker_realMoveVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//			//如果碰撞一直在发生，处于滑行状态，还要判断在滑行方向上是否有碰撞物，如果有，则停止运动，发生卡住状态
	//			// 计算下一步最远到达的距离，以进行碰撞侦测
	//			destCamPos = camPos + vWalkAlong;  
	//			D3DXVec3Normalize(&vWalkAlong, &vWalkAlong);
	//			if (vgsScene->DetectCollision(&destCamPos, &vWalkAlong, pModel, &colNormal, &distance) == FALSE)
	//			{
	//				if (bWeight)
	//				{
	//					vWalkAlong.y = 0;  //  如果有重力，那么不向上向下滑行
	//				}
	//				vgsScene->currentCam->Translate(&vWalkAlong, 0);
	//			}

	//			//和Walker发生碰撞的模型
	//			curColModel = pModel;
	//		}
	//		//如果没有碰撞，则按照原行走矢量方向行进
	//		else
	//		{
	//			if (pWalkVec.y == 0) //水平运动
	//				vgsScene->currentCam->Translate(&walker_realMoveVec, 2);
	//			else//上升下降
	//			{
	//				vgsScene->currentCam->Translate(&walker_realMoveVec, 0);
	//				walker_realMoveVec = D3DXVECTOR3(0,0,0);
	//			}

	//			bIsCollision = FALSE;
	//			curColModel = NULL;
	//		}						  
	//	}
	//	//如果系统不允许碰撞检测
	//	else
	//	{
	//		if (pWalkVec.y == 0) //水平运动
	//			vgsScene->currentCam->Translate(&walker_realMoveVec, 2);
	//		else//上升下降
	//		{
	//			vgsScene->currentCam->Translate(&walker_realMoveVec, 0);
	//			walker_realMoveVec = D3DXVECTOR3(0,0,0);
	//		}

	//		bIsCollision = FALSE;
	//		curColModel = NULL;
	//	}
	//}

	//----------------------------------------------------------------------
	//旋转Walker
	//----------------------------------------------------------------------
	//****左旋转
	float rosAcc = 0.0f;
	VECTOR3 rosVec = VECTOR3(0,0,0);
	// if (bCanRos)
	{
		if (m_inputDevice.keyBoardBuf['q'] > 64 || 
			m_inputDevice.keyBoardBuf['Q'] > 64/* ||
			vgsScene->softKey[4]*/)
			rosVec.y = m_curWalkerInfo.rosSpeed;
		//****右旋转
		if (m_inputDevice.keyBoardBuf['e'] > 64 || 
			m_inputDevice.keyBoardBuf['E'] > 64/* || 
			vgsScene->softKey[5]*/)
			rosVec.y = -m_curWalkerInfo.rosSpeed;
		//****上旋转
		if (m_inputDevice.keyBoardBuf['r'] > 64 || 
			m_inputDevice.keyBoardBuf['R'] > 64/* ||  
			vgsScene->softKey[6]*/)
			rosVec.x = m_curWalkerInfo.rosSpeed;
		//****下旋转
		if (m_inputDevice.keyBoardBuf['f'] > 64 || 
			m_inputDevice.keyBoardBuf['F'] > 64/* ||
			vgsScene->softKey[7]*/)
			rosVec.x = -m_curWalkerInfo.rosSpeed;

		walker_realRotateVec = walker_realRotateVec * m_curWalkerInfo.rosAcc + rosVec;
		float rosDis = D3DXVec3Length(&D3DXVECTOR3(walker_realRotateVec.x, walker_realRotateVec.y, walker_realRotateVec.z));
		if (rosDis > 0.0001f)
		{
			VECTOR3 vy = VECTOR3(0, walker_realRotateVec.y, 0);
			VECTOR3 vx = VECTOR3(walker_realRotateVec.x, 0, 0);
			//RotateCamera(m_vgsScene, curCamera, &vy, 2);
			//RotateCamera(m_vgsScene, curCamera, &vx, 1);

			DWORD dwModel = m_VGSCore.GetModelHandleByIndex(m_vgsScene, 0);
			VECTOR3 v3Model;
			m_VGSCore.GetModelPosition(m_vgsScene, dwModel, &v3Model);
			VECTOR3 v3Angle;
			v3Angle.x = walker_realRotateVec.x;
			v3Angle.y = walker_realRotateVec.y;
			v3Angle.z = 0;
			m_VGSCore.RoundInCameraSpace(m_vgsScene, curCamera, &v3Model, &v3Angle);

			//控制软件盘指南针的方向
			/*if (vgsScene->pRosOl != NULL)
				vgsScene->pRosOl->Rotate(walker_realRotateVec.y);*/
		}
	}


	//默认的鼠标控制----------------------------------------------------------------------------------------------
	//按下鼠标左键，左右拖拽鼠标，控制镜头的视角
	FLOAT rosStep = 0.004f;
	// if (bCanRos)
	//{
	//	if (m_inputDevice.mouseInfo.LButton)
	//	{
	//		INT disX = m_inputDevice.mouseInfo.X - m_mouseX;
	//		INT disY = m_inputDevice.mouseInfo.Y - m_mouseY;

	//		if (disX != 0)
	//		{
	//			RotateCamera(m_vgsScene, curCamera, &VECTOR3(0.0f, -(FLOAT)disX*rosStep, 0.0f), 2);

	//			//控制软件盘指南针的方向
	//			/*if (vgsScene->pRosOl != NULL)
	//				vgsScene->pRosOl->Rotate(-(FLOAT)disX*rosStep);*/
	//		}

	//		if (disY != 0)
	//		{
	//			//限制低头和抬头的角度
	//			GetCameraUp(m_vgsScene, curCamera, vCamUp);
	//			FLOAT fAngleCosValue = D3DXVec3Dot(&D3DXVECTOR3(vCamUp.x, vCamUp.y, vCamUp.z), &D3DXVECTOR3(0,1,0));
	//			if (fAngleCosValue > 0.3f) 
	//				RotateCamera(m_vgsScene, curCamera, &VECTOR3(-(FLOAT)disY*rosStep, 0.0f, 0.0f), 1);

	//			GetCameraUp(m_vgsScene, curCamera, vCamUp);
	//			fAngleCosValue = D3DXVec3Dot(&D3DXVECTOR3(vCamUp.x, vCamUp.y, vCamUp.z), &D3DXVECTOR3(0,1,0));
	//			if (fAngleCosValue < 0.3f) 
	//				RotateCamera(m_vgsScene, curCamera, &VECTOR3((FLOAT)disY*rosStep, 0.0f, 0.0f), 1);
	//		}

	//		m_mouseX = m_inputDevice.mouseInfo.X;
	//		m_mouseY = m_inputDevice.mouseInfo.Y;   //计算当前鼠标位置和上一帧鼠标位置的距离
	//	}
	//	//鼠标左键松开
	//	else
	//	{
	//		m_mouseX = m_inputDevice.mouseInfo.X;
	//		m_mouseY = m_inputDevice.mouseInfo.Y;   //计算当前鼠标位置和上一帧鼠标位置的距离
	//		m_inputDevice.mouseInfo.LButton = FALSE;
	//	}
	//}

	//ZeroMemory(vgsScene->softKey, sizeof(vgsScene->softKey)); // fuxb 2009-04-07 清空虚拟按键
}

// 某个字符键是否被触发
BOOL CVGS2DLL::IsAlphaKeyBuffered(BYTE key, BOOL caseSensitive/* = FALSE*/)
{
	if (!m_bAddtionalKeysEnable)
	{
		return FALSE;
	}

	if (key >= 0 && key < 256)
	{
		if (m_inputDevice.keyBoardBuf[key] > 64)
		{
			return TRUE;
		}

		if (!caseSensitive)
		{
			bool bIsAlpha = FALSE;
			if (key >= 'A' && key <= 'Z')
			{
				key += 32; // 转换成小写
				bIsAlpha = TRUE;
			}
			else if (key >= 'a' && key <= 'z')
			{
				key -= 32;  // 转换成大写
				bIsAlpha = TRUE;
			}
			if (bIsAlpha && m_inputDevice.keyBoardBuf[key] > 64)
			{
				return TRUE;
			}
		}
	}
	return FALSE;	
}

// 获取当前控制对象信息
VOID CVGS2DLL::GetCurrentControllerInfo()
{
	// 获取当前控制对象信息
	DWORD pController = m_VGSCore.GetCurrentController(m_vgsScene);
	CONTROLTYPE type = m_VGSCore.GetControllerType(pController);
	switch (type)
	{
	case CT_WALKER:
		m_VGSCore.GetWalkerProp(m_vgsScene, pController, &m_curWalkerInfo);
		break;
	case CT_EDITOR:
		m_VGSCore.GetEditorProp(m_vgsScene, pController, &m_curEditorInfo);
		break;
	case CT_FLYER:
		m_VGSCore.GetFlyerProp(m_vgsScene, pController, &m_curFlyerInfo);
		break;
	default:
		break;
	}
}