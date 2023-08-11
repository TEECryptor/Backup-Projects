/////////////////////////////////////////////////////////////////////////
// 名称: CScene类型模型定义--场景对象
// 描述: 
// 作者: lixin	
// 日期: 2005-12-20
/////////////////////////////////////////////////////////////////////////
#include "CScene.h"


//播放电影文件的类
//#include "CMovieTexture.h"
//播放声音的类
//#include "CAudioPlayer.h"

#include "VGSSupport.h"

#include "VGSRender.h"
#include "SwapChain.h"
#include "VGSErrCode.h"
//#include "TextureManager.h"
//#include "MaterialManager.h"
//#include ".\Animation\AnimationManager.h"
#include "Root.h"

#include ".\fileio\GetS3DFileInfo.h"
#include ".\FileIO\IMportS3D.h"
#include ".\Fileio\SaveV3D.h"
#include ".\Fileio\GetV3DBase.h"
// #include "FileIO/N3DLoader.h"

//LensFlare的资源
#include "Resource\\LS_SunImage.h"
#include "Resource\\LS_Tex1Image.h"
#include "Resource\\LS_Tex2Image.h"
#include "Resource\\LS_Tex3Image.h"

#include "Animation/Skeleton.h"

#define SKIN_EFFECT 0

#include "AxisMove.h"
#include <algorithm> 
using namespace std;

// CVGSRender g_VGSRender;  // 全局的渲染器对象

static float lightY = 0.0;

D3DXVECTOR3				CamDir;							//相机方向
D3DXVECTOR3				CamPos;							//相机位置


//-----------------------------------------------------------------------------
// 方法名: SortFuncEx(),改进的排序算法
// 参  数: const VOID* arg1, const VOID* arg2
// 描  述: 对透明模型进行排序
// 要把P1排在前面，则返回false, 否则返回true
//-----------------------------------------------------------------------------
bool SortFuncEx(CModel* p1, CModel* p2)
{
	bool result = false;
	
	if (!p1 || !p2)
	{
		return false;
	}
	
	//if (!p1->visible)
	//{
	//	return true;  // p2在前
	//}
	//if (!p2->visible)
	//{
	//	return false;  // p1在前
	//}
	
	BOOL bInModel1 = p1->IsInsideBox(&CamPos);
	BOOL bInModel2 = p2->IsInsideBox(&CamPos);
	if (bInModel1)  // 相机在模型1内部
	{
		if (bInModel2)  // 相机同时在模型1和2内部
		{
			FLOAT dist1 = p1->getNearDistanceSq(&CamPos);
			FLOAT dist2 = p2->getNearDistanceSq(&CamPos);
			return dist1 > dist2;  // 哪个距离近哪个在前
		}
		else
		{
			return true; // 相机在模型1内部，但是不再模型2内部，模型2在前
		}
	}
	else
	{
		if (bInModel2)  // 相机在模型2内，但是不在模型1内，模型1在前
		{
			return false;
		}
		else  // 相机既不在模型1内，也不在模型2内，那个距离近，哪个在前
		{
			FLOAT dist1 = p1->getNearDistanceSq(&CamPos);
			FLOAT dist2 = p2->getNearDistanceSq(&CamPos);
			return dist1 > dist2;
		}
	}
	
	return result;
}

//------------------------------------------------------------------------------
CScene::CScene(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pD3DDevice) : m_GUIManager(pD3DDevice)
{
	m_pd3d = pD3D;
	m_pd3dDevice = pD3DDevice;
	
	m_pBackBuffer = NULL;
	m_pZBuffer = NULL;
	m_pFSMotionBlur = NULL;

	m_pRootNode = NULL;

	InitVar();

	m_pCurController = NULL;
}

CScene::~CScene()
{
	if(m_pAxis)
	{
		SAFE_DELETE(m_pAxis);
	}
	if (m_pRootNode)
	{
		SAFE_DELETE(m_pRootNode);		
	}
}

//-----------------------------------------------------------------------------
// 方法名: InitVar()
// 参  数: 
// 用  途: 初始化变量
//-----------------------------------------------------------------------------
VOID CScene::InitVar()
{
	m_pCurCamera = NULL;
	m_CurMtrl = NULL;
	m_CurDifTex = NULL;
	m_CurLightTex = NULL;
	m_CurCubeMtrl = NULL;
	m_CurCubeMap = NULL;
	m_CursorModel = NULL;
	m_CurModel = NULL;
	pCtrlOl = NULL;
	pRosOl = NULL;	
	m_pStartLogo = NULL;
	m_pUnitedLogo = NULL;
	m_pLoadingBg = NULL;
	m_pLoadingBar = NULL;

	m_projMode=0;
	
	D3DXMatrixIdentity(&m_MatWorld); 
	D3DXMatrixIdentity(&m_MatViewOri); 
	D3DXMatrixIdentity(&m_MatView); 
	D3DXMatrixIdentity(&m_MatProj); 
	D3DXMatrixIdentity(&m_MatIdentity); 
	D3DXMatrixIdentity(&m_MatViewProj); 
	
	userInput = TRUE;
	renderScene = TRUE;
	ZeroMemory(softKey, sizeof(softKey));
	
	m_bWindowed = TRUE;
	bActive = TRUE;
	
	pMouseOverButton = NULL;
	m_bFSMotionBlur = FALSE;
	
	m_pLensFlare = NULL;
	m_bLensFlare = TRUE;
	m_bLensFlareCoverd = FALSE;
	
	
	m_pFSGlow = NULL;
	m_bFSGlow = FALSE;
	FSGlowLevel = 0;
	
	m_bFog = FALSE;
	m_FogColor = D3DCOLOR_RGBA(255, 255, 255, 0);	
	m_FogStart = 0.5f;	
	m_FogEnd = 0.8f;
	m_FogDensity = 0.66f;
	m_FogMode = D3DFOG_LINEAR;	
	
	m_FrameCnt = 0;
	m_bLensFlare_CalCover = FALSE;
	m_bLensFlare_Inverse = FALSE;
	
	fbl = 0.0001f;
	fps = 30;
	m_lastTimer = 0;
	
	m_bRenderCurrentFrame = TRUE;
	m_bLastFrameBlur = FALSE;
	
	m_CurTexStage = 0;
	m_AvailableVideoMemSize = 0;
	
	antiLevel = 0;						//抗拒齿界别
	
	FSBlurLevel = 0;					//全屏动态模糊的级别
	FSGlowLevel = 0;					//全屏光晕的级别
	
	m_bRenderScene = 0;					//渲染场景
	m_bRenderGUI = 0;					//渲染界面
	
	m_KfrCnt = 0;
	m_KfrRepeatCnt = 1;
	m_KfrRepeatIndex = 0;
	m_CurrentFrame = 0;
	m_TotalFrameCnt = 0;
	m_bKfrPlaying = FALSE;
	m_kfrPlayspeed = 35;				//实际上就是30，有时间误差
	
	m_StartFrame = 0;				//关键帧动画播放的起始帧
	m_EndFrame = 0;					//关键帧动画播放的终
	
	m_TotalFrameCnt = 0;				//总的帧数
	
	controlType = -1;	
	currentWalker = NULL;	
	currentEditor = NULL;	
	currentFlyer = NULL;
	
	m_bAuthorized = TRUE;
    //初始化场景中模型编辑状态
	m_ModelEdit=FALSE;
    //初始化场景编辑模式
	sceneEditMode=0;
	
	//初始化8个默认的相机
	// InitDefaultCamera();
	
	mouseWorldPos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	
    
	m_pAxis = NULL;
	
    //初始化鼠标左键前次状态
	pLButton= FALSE;
	
	alphaTestLevel=(DWORD)0x1f;

	beReflect= TRUE;

	if (!m_pRootNode)
	{
		m_pRootNode = new CSceneNode();
		// m_pRootNode->setNodeType(VGS_MOVABLETYPE_LIGHT);
		m_pRootNode->SetRoot(GetRoot());
	}
	
	m_GUIManager.SetSceneManager((DWORD)this);

	m_backgroundColor = D3DCOLOR_RGBA(100, 100, 100, 0);  // 缺省的背景色

	m_pBackGround = 0;

	m_vTransModel.clear();  // 清空透明模型队列
}


//-----------------------------------------------------------------------------
// 方法名: UpdateRatio()
// 参  数: 
// 描  述: 更新场景窗口的长宽比
//-----------------------------------------------------------------------------
// VOID CScene::UpdateRatio()
// {
// 	m_SceneRatio = (float)(m_SceneRect.right - m_SceneRect.left) / (m_SceneRect.bottom - m_SceneRect.top);
// }

//获得材质投影矩阵
D3DXMATRIXA16 CScene::GetMatriceProjectionTexture()
{
	D3DXMATRIXA16 Matrice;
	float yScale = -0.5f / tan( m_pCurCamera->GetFov()*0.5f);
	float xScale = -yScale / m_CurViewport->getAspectRatio();/*m_SceneRatio*/;

	Matrice._11 = xScale; Matrice._12 =   0.0f; Matrice._13 = 0.0f; Matrice._14 = 0.0f;
	Matrice._21 =   0.0f; Matrice._22 = yScale; Matrice._23 = 0.0f; Matrice._24 = 0.0f;
	Matrice._31 =   0.5f; Matrice._32 =   0.5f; Matrice._33 = 1.0f; Matrice._34 = 1.0f;
	Matrice._41 =   0.0f; Matrice._42 =   0.0f; Matrice._43 = 0.0f; Matrice._44 = 0.0f;
	return Matrice;
}

//-----------------------------------------------------------------------------
// 方法名: RestoreDeviceObject()
// 参  数: 
// 描  述: 恢复D3D Device，当用户改变窗口大小时调用
//-----------------------------------------------------------------------------
HRESULT CScene::RestoreScene()
{			
	return 0;

	//重建Model 的VB 和 IB
	/*std::vector<CModel*>::iterator ppModel = model.begin();
	for (INT i = 0; i < (INT)model.size(); i++)
	{
		CModel* pModel = *(ppModel + i);
		if (pModel->pCloneModel == NULL)
		{
			pModel->createVBuffer();
			pModel->createIBuffer();
		}
	}*/

	//Restore shader movie texture	//Invalidate shader movie texture
	INT shaderCnt = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = m_vShader.begin();
	for (int i = 0; i < shaderCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		INT texLayerCnt = pMtrl->m_vpTexLayers.size();
		std::vector<CTextureLayer*>::iterator ppTexLayer =  pMtrl->m_vpTexLayers.begin();
		for (UINT m = 0; m < (UINT)texLayerCnt; m++)
		{
			CTextureLayer* pTexLayer =*(ppTexLayer + m);
			
			/*if(pTexLayer->type ==TLT_MOVIE)
			{
				CMovieTexture* pMovieTex = (CMovieTexture*)(pTexLayer->pTexture);
				if (pMovieTex->m_pMovieTexRender)
				{
					//重置电影贴图对象
					pMovieTex->Play();
					//pMovieTex->m_pMovieTexRender->Pause();
					pMovieTex->m_pMovieTexRender->CreateFrameTextureBuffer();
				}
			}else */if(pTexLayer->type ==TLT_DYNAMIC)
			{
				CDynamicTex* pDyTex = (CDynamicTex*)(pTexLayer->pTexture);
				if (pDyTex)
					//重置动态贴图对象
					pDyTex->Restore();	 
				//add for test
			}
	// 				else if(pTexLayer->type ==TLT_REALTIMEREF)
	// 				{
	//                     //反射的物体的材质一定是唯一的因此只要取材质所属的模型列表的第一个即可
	// 					UINT modelCnt = pMtrl->subMeshList.size();
	// 					INT refListIdx = -1 ;
	// 					RenderSubMesh*  pSubMesh = pMtrl->subMeshList.begin();
	// 					for(UINT t = 0; t< modelCnt; t++)
	// 					{
	// 					   RenderSubMesh subMesh = *(pSubMesh + i);				   
	// 					   CModel* pModel =(CModel*)subMesh.modelPointer;
	// 					   if(pModel->sceneRefInfoListIdx != -1)
	// 					   {
	// 						   refListIdx =pModel->sceneRefInfoListIdx;
	// 						   break;
	// 					   }	
	// 					}
	// 					if(refListIdx == -1 || refListIdx >= reflectInfoList.size())
	// 						return E_FAIL;
	//                     //创建反射材质和反射表面
	// 					CTexture* pTex =(CTexture*)(pTexLayer->pTexture);
	// 
	// 					if(FAILED(m_pd3dDevice->CreateTexture(pTex->width, 
	// 						pTex->height, 
	// 						1, D3DUSAGE_RENDERTARGET, 
	// 						m_d3dpp.BackBufferFormat, 
	// 						D3DPOOL_DEFAULT, 
	// 						&reflectInfoList[refListIdx].renderToTexInfo.curRenderTexture)))
	// 						return E_FAIL;
	// 					
	// 					
	//                     reflectInfoList[refListIdx].renderToTexInfo.curRenderTexture->GetSurfaceLevel(0,&reflectInfoList[refListIdx].renderToTexInfo.curRenderSurface);
	// 					if(FAILED(m_pd3dDevice->CreateDepthStencilSurface(pTex->width,  
	// 						pTex->height, 
	// 						D3DFMT_D16,
	// 						D3DMULTISAMPLE_NONE/*m_pd3dpp->MultiSampleType*/,
	// 						&reflectInfoList[refListIdx].renderToTexInfo.curRenderZSurface)))
	// 						return E_FAIL;
	// 					pTex->textureBuf = reflectInfoList[refListIdx].renderToTexInfo.curRenderTexture;
	// 			
	// 				}
		}
	}



	//Restore Light
	std::vector<CLight*>::iterator ppLight = m_vLight.begin();
	for (i = 0; i < (INT)m_vLight.size(); i++)
	{
		CLight* pLight = *(ppLight + i);
		m_pd3dDevice->SetLight(i, &(pLight->param));		
		m_pd3dDevice->LightEnable(i, TRUE);
	}

	//重建Overlay的VB和IB
	std::vector<COverlay*>::iterator ppOverlay = m_vOverlay.begin();
	for (i = 0; i< (INT)m_vOverlay.size(); i++)
	{
		COverlay* pOverlay = *(ppOverlay + i);
		pOverlay->Create();
		UpdateOverlayRect(pOverlay);
	}

	//Invalidate the m_vFont
// 	CFont** ppFont = m_vFont.begin();
// 	INT fontSize = m_vFont.size();
// 	for (i = 0; i< fontSize; i++)
// 	{
// 		CFont* pFont = *(ppFont + i);
// 		pFont->Restore();
// 	}
// 	pDefaultFont->Restore();

	//restore the m_vText
// 	CText** ppText = m_vText.begin();
// 	INT textCnt = m_vText.size();
// 	for (i = 0; i< textCnt; i++)
// 	{
// 		CText* pText = *(ppText + i);
// 		pText->m_pFont = pDefaultFont;
// 	}

	//Restore the FSMotionBlur
	if (m_pFSMotionBlur)
		m_pFSMotionBlur->InitEffect();

	//Restore the FSGlow
	if (m_pFSGlow)
		m_pFSGlow->InitEffect();


	//Add for real time reflect test 2008/10/24
	m_pd3dDevice->GetRenderTarget(0, &m_pBackBuffer);

	if (FAILED(m_pd3dDevice->GetDepthStencilSurface(&m_pZBuffer)))
		return FALSE;

// 	//重建BumpWater的renderTarget
// 	INT bumpWaterCnt = bumpWater.size();
// 	CBumpWater** ppBumpWater = bumpWater.begin();
// 	for (i = 0; i< bumpWaterCnt; i++)
// 	{
// 		CBumpWater* pBumpWater = *(ppBumpWater + i);
// 		if(pBumpWater->multiLayer)
// 			pBumpWater->CreateMultiLayerRenderTarget(m_d3dpp, 512,512);
// 	}

	//重建m_pStartLogo
	if (m_pStartLogo)
	{
		m_pStartLogo->Create();
		//更新startLogo
		RECT logoRect, sceneRect;
		m_CurViewport->GetRect(sceneRect);
		logoRect.left = sceneRect.left + ( sceneRect.right - sceneRect.left)/2 - 48;
		logoRect.top =  sceneRect.top + (sceneRect.bottom - sceneRect.top)/2 - 48;//- 8;
		logoRect.right = logoRect.left + 96;//128;
		logoRect.bottom = logoRect.top + 96;//128;
		m_pStartLogo->SetRect(logoRect);
	}
	//重建m_pLoadingBg
	if (m_pLoadingBg)
	{
		m_pLoadingBg->Create();
		//更新startLogo
		RECT logoRect, sceneRect;
		m_CurViewport->GetRect(sceneRect);
		logoRect.left = sceneRect.left + ( sceneRect.right - sceneRect.left)/2 - 64;
		logoRect.top =  sceneRect.top + (sceneRect.bottom - sceneRect.top)/2 + 64 - 8;
		logoRect.right = logoRect.left + 128;
		logoRect.bottom = logoRect.top + 8;
		m_pLoadingBg->SetRect(logoRect);
	}
	//重建m_pLoadingBar
	if (m_pLoadingBar)
	{
		m_pLoadingBar->Create();
		//更新startLogo
		RECT logoRect, sceneRect;
		m_CurViewport->GetRect(sceneRect);
		logoRect.left = sceneRect.left + ( sceneRect.right - sceneRect.left)/2 - 64;
		logoRect.top =  sceneRect.top + (sceneRect.bottom - sceneRect.top)/2 + 64 - 8;
		logoRect.right = logoRect.left;
		logoRect.bottom = logoRect.top + 8;
		m_pLoadingBar->SetRect(logoRect);
	}

	//重建 m_pUnitedLogo
	if (m_pUnitedLogo)
		m_pUnitedLogo->Create();

	if(m_pAxis)
		m_pAxis->Restore();




	//Fog Restore
	//颜色
	m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, m_FogColor);
	//模式
	m_pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, m_FogMode);
	//如果是线性模式
	if (m_FogMode == D3DFOG_LINEAR)
	{
		m_pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_FogStart));
		m_pd3dDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_FogEnd));
	}
	//浓度
	else
		m_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*)(&m_FogDensity)));

	bActive = TRUE;
	return S_OK;
}

//-----------------------------------------------------------------------------
// 方法名: InvalidateDeviceObject()
// 参  数: 
// 描  述: 释放D3D及设备对象，当用户改变窗口大小时调用
//-----------------------------------------------------------------------------
HRESULT CScene::InvalidateScene()
{
	return 0;

	//Invalidate model
	//std::vector<CModel*>::iterator ppModel = model.begin();
	//int modelCnt = model.size();
	//for (UINT i= 0; i < (UINT)modelCnt; i++)
	//{
	//	CModel* pModel = *(ppModel + i);
	//	
	//	if (pModel->m_VBuffer == NULL || pModel->subMesh[0].m_IBuffer == NULL)
	//		continue;
	//	
	//	//release VB & IB
	//	if (pModel->pCloneModel == NULL)
	//	{
	//		//从顶点缓冲取数据备份，用于释放后重新创建顶点缓冲
	//		//判断顶点的FVF,不同的FVF填充不同的数据
	//		pModel->vertex = new CVertex[pModel->vertexCnt];
	//		UINT j;
	//		switch(pModel->m_FVF)
	//		{
	//		case FVF_POSCOLOR:
	//			//逐个顶点填充数据
	//			for (j=0; j< pModel->vertexCnt; j++)
	//			{
	//				//填充坐标数据到顶点缓冲
	//				pModel->vertex[j].position = ((VERTEXDECL_POSCOLOR*)pModel->m_pVertices)[j].position;
	//				pModel->vertex[j].diffuse = ((VERTEXDECL_POSCOLOR*)pModel->m_pVertices)[j].diffuse;
	//			}
	//			break;
	//			
	//		case FVF_POSNORMALCOLOR:
	//			//逐个顶点填充数据
	//			for (j=0; j< pModel->vertexCnt; j++)
	//			{
	//				//填充坐标数据到顶点缓冲
	//				pModel->vertex[j].position = ((VERTEXDECL_POSNORMALCOLOR*)pModel->m_pVertices)[j].position;	
	//				pModel->vertex[j].normal = ((VERTEXDECL_POSNORMALCOLOR*)pModel->m_pVertices)[j].normal;
	//			}
	//			break;
	//			
	//		case FVF_POSCOLORCOORD1:
	//			//逐个顶点填充数据
	//			for (j=0; j< pModel->vertexCnt; j++)
	//			{
	//				//填充坐标数据到顶点缓冲
	//				pModel->vertex[j].position = ((VERTEXDECL_POSCOLORCOORD1*)pModel->m_pVertices)[j].position;
	//				pModel->vertex[j].uv1.u = ((VERTEXDECL_POSCOLORCOORD1*)pModel->m_pVertices)[j].su;
	//				pModel->vertex[j].uv1.v = ((VERTEXDECL_POSCOLORCOORD1*)pModel->m_pVertices)[j].sv;
	//			}
	//			break;
	//			
	//		case FVF_POSNORMALCOLORCOORD1:
	//			//逐个顶点填充数据
	//			for (j=0; j< pModel->vertexCnt; j++)
	//			{
	//				//填充坐标数据到顶点缓冲
	//				pModel->vertex[j].position = ((VERTEXDECL_POSNORMALCOLORCOORD1*)pModel->m_pVertices)[j].position;	
	//				pModel->vertex[j].normal = ((VERTEXDECL_POSNORMALCOLORCOORD1*)pModel->m_pVertices)[j].normal;
	//				pModel->vertex[j].uv1.u = ((VERTEXDECL_POSNORMALCOLORCOORD1*)pModel->m_pVertices)[j].su;
	//				pModel->vertex[j].uv1.v = ((VERTEXDECL_POSNORMALCOLORCOORD1*)pModel->m_pVertices)[j].sv;
	//			}
	//			break;
	//			
	//		case FVF_POSNORMALCOLORCOORD2:
	//			//逐个顶点填充数据
	//			for (j=0; j< pModel->vertexCnt; j++)
	//			{
	//				//填充坐标数据到顶点缓冲
	//				pModel->vertex[j].position = ((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[j].position;	
	//				pModel->vertex[j].uv1.u = ((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[j].su1;
	//				pModel->vertex[j].uv1.v = ((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[j].sv1;
	//				pModel->vertex[j].uv2.u = ((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[j].su2;
	//				pModel->vertex[j].uv2.u = ((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[j].sv2;
	//			}
	//			break;
	//			
	//		case FVF_POSNORMALCOLORCOORD3:
	//			//逐个顶点填充数据
	//			for (j=0; j< pModel->vertexCnt; j++)
	//			{
	//				//填充坐标数据到顶点缓冲
	//				pModel->vertex[j].position = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[j].position;	
	//				pModel->vertex[j].normal = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[j].normal;
	//				pModel->vertex[j].uv1.u = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[j].su1;
	//				pModel->vertex[j].uv1.v = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[j].sv1;
	//				pModel->vertex[j].uv2.u = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[j].su2;
	//				pModel->vertex[j].uv2.v = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[j].sv2;
	//			}
	//			break;
	//		case FVF_POSNORMALCOLORCOORD4:
	//			//逐个顶点填充数据
	//			for (j=0; j< pModel->vertexCnt; j++)
	//			{
	//				//填充坐标数据到顶点缓冲
	//				pModel->vertex[j].position = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].position;	
	//				pModel->vertex[j].normal = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].normal;
	//				pModel->vertex[j].uv1.u = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].su1;
	//				pModel->vertex[j].uv1.v = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].sv1;
	//				pModel->vertex[j].uv2.u = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].su2;
	//				pModel->vertex[j].uv2.v = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].sv2;
	//				pModel->vertex[j].uv3.u = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].su3;
	//				pModel->vertex[j].uv3.v = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].sv3;
	//				pModel->vertex[j].uv4.u = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].su4;
	//				pModel->vertex[j].uv4.v = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[j].sv4;
	//			}
	//			break;
	//		}
	//		SAFE_RELEASE_D3D(pModel->m_VBuffer);
	//		
	//		for(UINT i=0 ;i < pModel->subMesh.size(); i++)
	//		{
	//			//备份面数据
	//			pModel->subMesh[i].face = new CFace[pModel->subMesh[i].faceCnt];	
	//			if (pModel->subMesh[i].IBufferFormat == D3DFMT_INDEX16)
	//			{
	//				for (UINT k =0; k< pModel->subMesh[i].faceCnt; k++)
	//				{
	//					INT v0_index = ((unsigned short*)(pModel->subMesh[i].m_pIndices))[3*k];
	//					INT v1_index = ((unsigned short*)(pModel->subMesh[i].m_pIndices))[3*k+1];
	//					INT v2_index = ((unsigned short*)(pModel->subMesh[i].m_pIndices))[3*k+2];
	//					
	//					pModel->subMesh[i].face[k].vertices0 = &(pModel->vertex[v0_index]);
	//					pModel->subMesh[i].face[k].vertices1 = &(pModel->vertex[v1_index]);
	//					pModel->subMesh[i].face[k].vertices2 = &(pModel->vertex[v2_index]);
	//					
	//					pModel->subMesh[i].face[k].vertices0->index = v0_index;
	//					pModel->subMesh[i].face[k].vertices1->index = v1_index;
	//					pModel->subMesh[i].face[k].vertices2->index = v2_index;
	//				}
	//			}	
	//			if (pModel->subMesh[i].IBufferFormat == D3DFMT_INDEX32)
	//			{
	//				for (UINT k =0; k< pModel->subMesh[i].faceCnt; k++)
	//				{
	//					INT v0_index = ((unsigned int*)(pModel->subMesh[i].m_pIndices))[3*k];
	//					INT v1_index = ((unsigned int*)(pModel->subMesh[i].m_pIndices))[3*k+1];
	//					INT v2_index = ((unsigned int*)(pModel->subMesh[i].m_pIndices))[3*k+2];
	//					
	//					pModel->subMesh[i].face[k].vertices0 = &(pModel->vertex[v0_index]);
	//					pModel->subMesh[i].face[k].vertices1 = &(pModel->vertex[v1_index]);
	//					pModel->subMesh[i].face[k].vertices2 = &(pModel->vertex[v2_index]);
	//					
	//					pModel->subMesh[i].face[k].vertices0->index = v0_index;
	//					pModel->subMesh[i].face[k].vertices1->index = v1_index;
	//					pModel->subMesh[i].face[k].vertices2->index = v2_index;
	//				}
	//			}
	//			
	//			
	//			SAFE_RELEASE_D3D(pModel->subMesh[i].m_IBuffer);
	//			
	//		}
	//		
	//	}
	//}
	
	//Invalidate shader movie texture
	INT shaderCnt = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = m_vShader.begin();
	
	for (INT i = 0; i < (INT)shaderCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);

		INT texLayerCnt = pMtrl->m_vpTexLayers.size();
		std::vector<CTextureLayer*>::iterator ppTexLayer =  pMtrl->m_vpTexLayers.begin();
		for(INT m = 0; m < (INT)texLayerCnt; m++)
		{
		  CTextureLayer* pTexLayer =*(ppTexLayer + m);
// 		  if(pTexLayer->type ==TLT_MOVIE)
// 		  {
// 			  CMovieTexture* pMovieTex = (CMovieTexture*)(pTexLayer->pTexture);
// 			  if (pMovieTex->m_pMovieTexRender)
// 			  {
// 				  //释放电影贴图对象
// 				  pMovieTex->Pause();
// 				  //pMovieTex->m_pMovieTexRender->Pause();
// 				  pMovieTex->m_pMovieTexRender->ReleaseTextureBuf();
// 			  }
// 		  }else 
		  if(pTexLayer->type ==TLT_DYNAMIC)
		  {
			  CDynamicTex* pDyTex = (CDynamicTex*)(pTexLayer->pTexture);
			  if (pDyTex)
				  //释放动态贴图对象
				  pDyTex->Invalidate();		  		  
		  }				
		}		
	}

	
	//Invalidate Light
	std::vector<CLight*>::iterator ppLight = m_vLight.begin();
	int lightcnt = m_vLight.size();
	for (INT i = 0; i < (INT)lightcnt; i++)
	{
		CLight* pLight = *(ppLight + i);
		m_pd3dDevice->LightEnable(i, FALSE);
	}
	
	//Release Overlay的VB和IB
	std::vector<COverlay*>::iterator ppOverlay = m_vOverlay.begin();
	int overlayCnt = m_vOverlay.size();
	for (i = 0; i< overlayCnt; i++)
	{
		COverlay* pOverlay = *(ppOverlay + i);
		pOverlay->Invalidate();
	}
	
	//Invalidate the m_vFont
// 	CFont** ppFont = m_vFont.begin();
// 	INT fontSize = m_vFont.size();
// 	for (i = 0; i< fontSize; i++)
// 	{
// 		CFont* pFont = *(ppFont + i);
// 		pFont->Release();
// 	}
// 	m_vFont.clear();
// 	pDefaultFont->Release();

	//Add for Realtime Reflect effect test 2008/10/24 -------------------------------------
    INT reflectCnt =reflectInfoList.size();
	std::vector<ReflectInfo>::iterator pReflectInfo = reflectInfoList.begin();

    for (i = 0; i < reflectCnt; i++)
	{
		ReflectInfo refInfo =*(pReflectInfo+i);
		
		if(refInfo.renderToTexInfo.curRenderSurface !=NULL)
		{
			refInfo.renderToTexInfo.curRenderSurface->Release();
			refInfo.renderToTexInfo.curRenderSurface = NULL;
		}
		if (refInfo.renderToTexInfo.curRenderTexture != NULL)
		{
			refInfo.renderToTexInfo.curRenderTexture->Release();
			refInfo.renderToTexInfo.curRenderTexture = NULL;
		}
		if (refInfo.renderToTexInfo.curRenderZSurface != NULL)
		{
			refInfo.renderToTexInfo.curRenderZSurface->Release();
			refInfo.renderToTexInfo.curRenderZSurface = NULL;
		}
	}
	//-------------------------------------------------------------------------------------
	
	//Invalidate the FSMotionBlur
	if (m_pFSMotionBlur)
		m_pFSMotionBlur->Invalidate();
	
	//Invalidate the FSGlow
	if (m_pFSGlow)
		m_pFSGlow->Invalidate();
	
	//Release m_pStartLogo
	if (m_pStartLogo)
		m_pStartLogo->Invalidate();
	
	//Release m_pUnitedLogo
	if (m_pUnitedLogo)
		m_pUnitedLogo->Invalidate();
	//Release m_pLoadingBg
	if (m_pLoadingBg)
		m_pLoadingBg->Invalidate();
	//Release m_pLoadingBar
	if (m_pLoadingBar)
		m_pLoadingBar->Invalidate();

	if(m_pAxis){
		SAFE_RELEASE(m_pAxis);
	}

    //释放BumpWater的renderTarget
// 	INT bumpWaterCnt = bumpWater.size();
// 	CBumpWater** ppBumpWater = bumpWater.begin();
// 	for (i = 0; i< bumpWaterCnt; i++)
// 	{
// 		CBumpWater* pBumpWater = *(ppBumpWater + i);
// 		if(pBumpWater->multiLayer)
// 		 pBumpWater->ReleaseMultiLayerRenderTarget();
// 	}
	

	SAFE_RELEASE_D3D(m_pBackBuffer);
	SAFE_RELEASE_D3D(m_pZBuffer);
	
	//Invokes the resource manager to free memory. 
	//激活D3D资源管理器释放所有显存
	// m_pd3dDevice->ResourceManagerDiscardBytes(0);
	
	return S_OK;
}

//-----------------------------------------------------------------------------
// 方法名: UseDefaultCam()
// 参  数: 
// 描  述: 设置默认的摄像机
//-----------------------------------------------------------------------------
/*VOID CScene::UseDefaultCam()
{
	m_pCurCamera = &defaultCam;
	
	//摄像机From和At位置控制，处理摄像机移动
	D3DXMatrixLookAtLH(&m_MatView, m_pCurCamera->GetFrom(), 
					   m_pCurCamera->GetAt(), m_pCurCamera->GetUp());
	
}*/

//-----------------------------------------------------------------------------
// 方法名: ViewTransform()
// 参  数: 
// 描  述: 视角变换
//-----------------------------------------------------------------------------
VOID CScene::ViewTransform()
{	
	if (m_pCurCamera == NULL) 
	{
		return;
	}

	/*if (m_KfrRepeatCnt == -1 || m_KfrRepeatIndex < m_KfrRepeatCnt)
	{
		//相机的当前的播放帧数
		//INT currentFrame = m_CurrentFrame;
		
		//if (m_CurrentFrame >= m_pCurCamera->kfrCnt)
		//	currentFrame = m_pCurCamera->kfrCnt - 1;
		
		//设定摄像机关键帧动画
		if (m_pCurCamera->kfrCnt >1 && m_pCurCamera->playKfr)
		{
			//相机动画过程中的动态模糊控制
			if (m_pFSMotionBlur && m_pCurCamera->kfrFSMotionBlur)
			{
				m_pFSMotionBlur->m_bBlurFlag = m_pCurCamera->kfrFSMotionBlur[m_pCurCamera->kfrCurFrame];	
				
				//动态模糊由于在这里改变了渲染目标表面，所以当前帧不能渲染，应该直接渲染下一帧
				if (m_pFSMotionBlur->m_bBlurFlag != m_bLastFrameBlur)
					m_bRenderCurrentFrame = FALSE;
				else
					m_bRenderCurrentFrame = TRUE;
				
				m_bFSMotionBlur = m_pFSMotionBlur->m_bBlurFlag;
				m_pFSMotionBlur->SetBlurAmount(m_pCurCamera->kfrFSMotionBlurValue[m_pCurCamera->kfrCurFrame]);
				m_pFSMotionBlur->SetRenderTarget();
				
				//记录当前帧动态模糊的状态
				m_bLastFrameBlur = m_pFSMotionBlur->m_bBlurFlag;
				
				if (m_bRenderCurrentFrame == FALSE)
					return;
			}
			
			//摄像机From和At位置控制，处理摄像机移动
			m_pCurCamera->SetFrom(&D3DXVECTOR3(m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].from.x,
				                  m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].from.y, 
								  m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].from.z));
			m_pCurCamera->SetAt(&D3DXVECTOR3(m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].at.x,
								m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].at.y,
								m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].at.z));
			m_pCurCamera->SetRoll(m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].roll);
			m_pCurCamera->isPlaying = TRUE;
			
			//如果相机设置了lookat
			if (m_pCurCamera->lookAtModel != NULL)
				m_pCurCamera->SetAt(&m_pCurCamera->lookAtModel->pNode->worldPosition); //transform.position);
			
			D3DXMatrixLookAtLH(&m_MatView, 
				&D3DXVECTOR3(m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].from.x,
				             m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].from.y,
							 m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].from.z), 
				&D3DXVECTOR3(m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].at.x,
				             m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].at.y,
							 m_pCurCamera->kfrInfo[m_pCurCamera->kfrCurFrame].at.z), 
				&m_pCurCamera->m_up);
			
			//相机动画过程中的JS 函数回调处理
			if (m_pCurCamera->InvokeFuncName)
			{
				if (m_pCurCamera->InvokeFuncName[m_pCurCamera->kfrCurFrame].name)
					PostMessage(m_hWnd, WM_JSFUNC_PROC, (DWORD)m_pCurCamera, m_pCurCamera->kfrCurFrame);
			}
			
			//跳到第0证
			if(m_pCurCamera->kfrCurFrame < m_pCurCamera->kfrCnt-1)
				m_pCurCamera->kfrCurFrame++;
			else
			{
				m_pCurCamera->kfrCurFrame = 0;
				m_KfrRepeatIndex++;
			}
			
		}
		//如果没有关键帧动画
		else
		{
			//如果相机设置了lookat
			if (m_pCurCamera->lookAtModel != NULL)
				m_pCurCamera->SetAt(&m_pCurCamera->lookAtModel->pNode->worldPosition); //transform.position);
			
			D3DXMatrixLookAtLH(&m_MatView, &(m_pCurCamera->m_from), &(m_pCurCamera->m_at), &(m_pCurCamera->m_up));
			m_pCurCamera->isPlaying = FALSE;
		}
	}
	else*/
	{
		//如果相机设置了lookat
		if (m_pCurCamera->GetLookAtModel() != NULL)
		{
			m_pCurCamera->Lookat(&m_pCurCamera->GetLookAtModel()->m_pNode->getPosition(VTS_WORLD), VTS_WORLD); //transform.position);
		}

		//D3DXMatrixLookAtLH(&m_MatView, &(m_pCurCamera->m_from), &(m_pCurCamera->m_at), &(m_pCurCamera->m_up));
		/*m_pCurCamera->m_at.x = 1.05f;
		m_pCurCamera->m_at.y = 5.58f;
		m_pCurCamera->m_at.z = 0.93f;
		m_pCurCamera->m_up.x = 0;
		m_pCurCamera->m_up.y = 1;
		m_pCurCamera->m_up.z = 0;*/
		D3DXMatrixLookAtLH(&m_MatView, &(m_pCurCamera->GetPosition(VTS_WORLD)), &(m_pCurCamera->GetLookat(VTS_WORLD)), &(m_pCurCamera->GetUp(VTS_WORLD)));

		/*D3DXMatrixLookAtLH(&m_MatView, &D3DXVECTOR3(-516.68927, 421.87497, -516.68927), 
			               &D3DXVECTOR3(-295.09979, 240.94791, -295.09970), 
						   &D3DXVECTOR3(0.35355330, 0.86602545, 0.35355347));
		m_pCurCamera->setNearClip(1);
		m_pCurCamera->setFarClip(100000);*/

	}
	
// 	m_MatView._31 = -m_MatView._31;
// 	m_MatView._32 = -m_MatView._32;
// 	m_MatView._33 = -m_MatView._33;
// 	m_MatView._34 = -m_MatView._34; 
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_MatView);	

#ifdef _DEBUG
	/*{
		static bool bHasWritten = false;
		if (!bHasWritten)
		{
			FILE *pDugFile = fopen("d:\\log1.txt", "wt");
			std::string str = "MatView:\n";
			char tmp[1024];
			ZeroMemory(tmp, sizeof(tmp));
			D3DXVECTOR3 v3Pos, v3At, v3Up;
			v3Pos = m_pCurCamera->GetPosition(VTS_WORLD);
			v3At  = m_pCurCamera->GetLookat(VTS_WORLD);
			v3Up  = m_pCurCamera->GetUp(VTS_WORLD);
			sprintf(tmp, "\ncam: \nfrom %f, %f, %f, at %f, %f, %f, up %f, %f, %f", v3Pos.x, v3Pos.y, v3Pos.z, 
				v3At.x, v3At.y, v3At.z, 
				v3Up.x,v3Up.y, v3Up.z);
			fwrite(tmp, std::string(tmp).length(), 1, pDugFile);
			sprintf(tmp, "\nMatView:\n %f, %f, %f, %f, \n%f, %f, %f, %f, \n%f, %f, %f, %f, \n%f, %f, %f, %f",
				m_MatView._11, m_MatView._12, m_MatView._13, m_MatView._14, 
				m_MatView._21, m_MatView._22, m_MatView._23, m_MatView._24,
				m_MatView._31, m_MatView._32, m_MatView._33, m_MatView._34,
				m_MatView._41, m_MatView._42, m_MatView._43, m_MatView._44);
			fwrite(tmp, std::string(tmp).length(), 1, pDugFile);
			fclose(pDugFile);
			bHasWritten = true;
		}

	}*/
#endif
	
}

//------------------------------------------------------------------------------
//更新Overlay的Rect
//老版本的V3D文件Overlay包含对齐模式，新版本的没有了
//Overlay默认为和屏幕的相对尺寸，当设置了[与图像等大]时，Overlay的左上角为相对位置，
//宽度和高度为图像的原尺寸。
//------------------------------------------------------------------------------
VOID CScene::UpdateOverlayRect(COverlay* pOverlay)
{
}

//-----------------------------------------------------------------------------
// 方法名: ProjTransform()
// 参  数: 
// 描  述: 投影变换
//-----------------------------------------------------------------------------
VOID CScene::ProjTransform() 
{
	if (m_pCurCamera == NULL || m_CurViewport == NULL) 
	{
		return;
	}
	
	D3DXMatrixPerspectiveFovLH(&m_MatProj, 
								m_pCurCamera->GetFov(),  
								m_CurViewport->getAspectRatio(),
								m_pCurCamera->getNearClip(),
								m_pCurCamera->getFarClip());

	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_MatProj);


#ifdef _DEBUG
	/*char tmp[1024];
	ZeroMemory(tmp, sizeof(tmp));
	sprintf(tmp, "\nMatProj:\n %f, %f, %f, %f, \n%f, %f, %f, %f, \n%f, %f, %f, %f, \n%f, %f, %f, %f",
		m_MatProj._11, m_MatProj._12, m_MatProj._13, m_MatProj._14, 
		m_MatProj._21, m_MatProj._22, m_MatProj._23, m_MatProj._24,
		m_MatProj._31, m_MatProj._32, m_MatProj._33, m_MatProj._34,
		m_MatProj._41, m_MatProj._42, m_MatProj._43, m_MatProj._44);
	fwrite(tmp, std::string(tmp).length(), 1, m_pDugFile);*/
#endif
}

//播放modelModelTo动画
VOID CScene::PlayUserMoveModelKFR()
{
	//播放用户自定义帧移动动画
	m_CurModel->m_pNode->translate(&(m_CurModel->user_moveKFR[m_CurModel->user_moveCurFrame]), VTS_WORLD);
	m_CurModel->user_moveCurFrame++;
	if (m_CurModel->user_moveCurFrame == m_CurModel->user_moveFrameCnt)
	{
		SAFE_DELETE(m_CurModel->user_moveKFR);
		m_CurModel->user_moveFrameCnt = 0;
		m_CurModel->user_moveCurFrame = 0;
	}
}
//-----------------------------------------------------------------------------
// 方法名: UpdateParentFrame()
// 参  数: curModel
// 描  述: 播放模型动画
//-------------------------																																												----------------------------------------------------
VOID CScene::UpdateParentFrame(CModel** curModel)
{
// 	if((*curModel)->pNode->m_parent)
// 	{
// 		if((N3D_NODETYPE)((*curModel)->pNode->m_parent->m_Type)==NNT_MODEL)
// 		{
// 			CModel* pParModel = (CModel*)((*curModel)->pNode->m_parent->m_object);
// 			
// 			UpdateParentFrame(&pParModel);
// 			
// 			if (pParModel->kfrPlay == TRUE && pParModel->kfrCnt > 1)
// 			{
// 				
// 				INT currentFrame = m_CurrentFrame;
// 				if (currentFrame >= pParModel->kfrCnt)
// 				{
// 					//在关键帧主循环中继续循环播放模型动画
// 					if (pParModel->kfrRepeatCnt == -1 || 
// 						pParModel->kfrRepeatIndex < pParModel->kfrRepeatCnt)
// 					{
// 						currentFrame = m_CurrentFrame % pParModel->kfrCnt;
// 					}
// 					//只播放一次
// 					else
// 					{
// 						currentFrame = pParModel->kfrCnt - 1;
// 					}					
// 				}
// 				if(pParModel->pNode->currentFrame !=currentFrame)
// 				{
// 					pParModel->pNode->currentFrame=currentFrame;
// 					pParModel->pNode->curTransform.matrix=pParModel->pNode->kfrtransform[currentFrame];
// 					pParModel->pNode->updateFromParent();	
// 				}
// 			}
// 		}
// 	}
}
//-----------------------------------------------------------------------------
// 方法名: ProjTransform()
// 参  数: 
// 描  述: 播放模型动画
//-----------------------------------------------------------------------------
VOID CScene::PlayModelKeyFrame()
{	
// 	UpdateParentFrame(&m_CurModel);
// 	
// 	//如果不播放关键帧动画
// 	if (m_CurModel->kfrPlay == FALSE || m_CurModel->kfrCnt <= 1) 
// 	{
// 		m_CurModel->pNode->updateFromParent();
// 		return;
// 	}
// 	
// 	if (m_KfrRepeatCnt == -1 || m_KfrRepeatIndex < m_KfrRepeatCnt)
// 	{
// 		//播放相机动画
// 		
// 		INT currentFrame = m_CurrentFrame;
// 		if (currentFrame >= m_CurModel->kfrCnt)
// 		{
// 			//在关键帧主循环中继续循环播放模型动画
// 			if (m_CurModel->kfrRepeatCnt == -1 || 
// 				m_CurModel->kfrRepeatIndex < m_CurModel->kfrRepeatCnt)
// 			{
// 				currentFrame = m_CurrentFrame % m_CurModel->kfrCnt;
// 				m_CurModel->kfrRepeatIndex++;
// 			}
// 			//只播放一次
// 			else
// 			{
// 				currentFrame = m_CurModel->kfrCnt - 1;
// 			}
// 		}
//         
// 		//逐帧播放关键帧动画------------------------------------------------------------------		
//         if(m_CurModel->pNode->currentFrame !=currentFrame)
// 		{
// 			m_CurModel->pNode->currentFrame =currentFrame;
// 			m_CurModel->pNode->curTransform.matrix=m_CurModel->pNode->kfrtransform[currentFrame];
// 			m_CurModel->pNode->updateTransform();
// 		}
// 		
// 		m_CurModel->update();
// 		
// 	}
}

//-----------------------------------------------------------------------------
// 方法名: IsInFrustrum()
// 参  数: D3DXVECTOR3* pos
// 描  述: 检测顶点是否在视锥体内
//-----------------------------------------------------------------------------
BOOL CScene::IsInFrustrum(D3DXVECTOR3* pos)
{
/*
INT ScreenX, ScreenY;
CalcScreenCoords(pos, &ScreenX, &ScreenY);

  if (ScreenX > 0 && ScreenX < rect.right && ScreenY > 0 && ScreenY < rect.bottom)
		return TRUE;
		else
		return FALSE;
	*/
	
	//如果相机在当前模型包围球的外部，得到包围球中心在View空间中的位置
	D3DXVECTOR3 SCInView;
	D3DXVec3TransformCoord(&SCInView, pos, &m_MatView);
	
	//看物体是否在视锥体内
	//ZPlane
	//前后剪裁面
	if (SCInView.z > m_pCurCamera->getFarClip() || SCInView.z < m_pCurCamera->getNearClip())
		return FALSE;
	
	//YPlane-上下剪裁面
	if (SCInView.y/SCInView.z < -m_pCurCamera->m_tanHalfFieldOfViewV)
		return FALSE;
	if (SCInView.y/SCInView.z > m_pCurCamera->m_tanHalfFieldOfViewV)
		return FALSE;
	
	//XPlane
	float tanHalfFieldOfViewH = m_pCurCamera->m_tanHalfFieldOfViewV * m_CurViewport->getAspectRatio();
	if (SCInView.x/SCInView.z > tanHalfFieldOfViewH)
		return FALSE;
	if (SCInView.x/SCInView.z < -tanHalfFieldOfViewH)
		return FALSE;
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// 方法名: IsInFrustrum()
// 参  数: CModel* pModel
// 描  述: 对模型是否在视锥体内进行检测，(检测包围球)。
//		   0-不在视锥体内，1-完全或部分在视锥体内
//-----------------------------------------------------------------------------
BOOL CScene::IsInFrustrum(CModel* pModel)
{	
	D3DXVECTOR3 vCamDis = m_pCurCamera->GetPosition(VTS_WORLD) - pModel->boundingSphere.Center;
	FLOAT camDis = D3DXVec3Length(&vCamDis);
	
	//判断模型是否距离当前相机很远，而且由于模型太小可能根本无法看到
	//当模型的包围球的半径和模型的中心的距离小于某个比值时，则近似忽略，不显示这个模型了
	if (pModel->bUseFBL)
	{
		FLOAT sfbl = pModel->boundingSphere.Radius/camDis;
		if (sfbl < fbl)
		{
			pModel->culled = true;
			return !pModel->culled;
		}
	}
	
	/*
	//如果相机在当前模型包围盒的内部，则当前模型不能被裁剪
	if (m_pCurCamera->transform.position.x > pModel->boundingBox.MinCorner.x &&
	m_pCurCamera->transform.position.x < pModel->boundingBox.MaxCorner.x &&
	m_pCurCamera->transform.position.y > pModel->boundingBox.MinCorner.y &&
	m_pCurCamera->transform.position.y < pModel->boundingBox.MaxCorner.y &&
	m_pCurCamera->transform.position.z > pModel->boundingBox.MinCorner.z &&
	m_pCurCamera->transform.position.z < pModel->boundingBox.MaxCorner.z)
	{
	pModel->culled = false;
	return;
	}
	*/
	
	if (camDis < pModel->boundingSphere.Radius)
	{
		pModel->culled = false;
		return !pModel->culled;
	}
	
	//判断当前模型包围盒的8个顶点是否都不在视锥内
	D3DXVECTOR3 v0, v1, v2, v3, v4, v5, v6, v7;
	BOOL bv0, bv1, bv2, bv3, bv4, bv5, bv6, bv7;
	
	v0.x = pModel->boundingBox.minCorner.x;
	v0.y = pModel->boundingBox.minCorner.y;
	v0.z = pModel->boundingBox.minCorner.z;
	bv0 = IsInFrustrum(&v0);
	//只要有一个角点在视锥体内，就要渲染模型
	if (bv0) 
	{
		pModel->culled = false;	
		return !pModel->culled;
	}
	
	v1.x = pModel->boundingBox.minCorner.x;
	v1.y = pModel->boundingBox.minCorner.y;
	v1.z = pModel->boundingBox.maxCorner.z;
	bv1 = IsInFrustrum(&v1);
	if (bv1) 
	{
		pModel->culled = false;	
		return !pModel->culled;
	}
	
	v2.x = pModel->boundingBox.maxCorner.x;
	v2.y = pModel->boundingBox.minCorner.y;
	v2.z = pModel->boundingBox.maxCorner.z;
	bv2 = IsInFrustrum(&v2);
	if (bv2) 
	{
		pModel->culled = false;	
		return !pModel->culled;
	}
	
	v3.x = pModel->boundingBox.maxCorner.x;
	v3.y = pModel->boundingBox.minCorner.y;
	v3.z = pModel->boundingBox.minCorner.z;
	bv3 = IsInFrustrum(&v3);
	if (bv3) 
	{
		pModel->culled = false;	
		return !pModel->culled;
	}
	
	v4.x = pModel->boundingBox.minCorner.x;
	v4.y = pModel->boundingBox.maxCorner.y;
	v4.z = pModel->boundingBox.minCorner.z;
	bv4 = IsInFrustrum(&v4);
	if (bv4) 
	{
		pModel->culled = false;	
		return !pModel->culled;
	}
	
	v5.x = pModel->boundingBox.minCorner.x;
	v5.y = pModel->boundingBox.maxCorner.y;
	v5.z = pModel->boundingBox.maxCorner.z;
	bv5 = IsInFrustrum(&v5);
	if (bv5) 
	{
		pModel->culled = false;	
		return !pModel->culled;
	}
	
	v6 = D3DXVECTOR3(pModel->boundingBox.maxCorner.x, pModel->boundingBox.maxCorner.y, pModel->boundingBox.maxCorner.z);
	bv6 = IsInFrustrum(&v6);
	if (bv6) 
	{
		pModel->culled = false;	
		return !pModel->culled;
	}
	v7.x = pModel->boundingBox.maxCorner.x;
	v7.y = pModel->boundingBox.maxCorner.y;
	v7.z = pModel->boundingBox.minCorner.z;
	bv7 = IsInFrustrum(&v7);
	if (bv7) 
	{
		pModel->culled = false;	
		return !pModel->culled;
	}
	
	//如果当前模型包围盒所有的角点都在视锥体之外，还需要检测模型的包围盒是否横跨视锥体
	D3DXVECTOR3 SCInView;
	D3DXVec3TransformCoord(&SCInView, &pModel->boundingSphere.Center, &m_MatView);
	//看物体是否在视锥体内
	//ZPlane
	if (SCInView.z - pModel->boundingSphere.Radius > m_pCurCamera->getFarClip() || 
		SCInView.z + pModel->boundingSphere.Radius < m_pCurCamera->getNearClip())
	{
		pModel->culled = true;
		return !pModel->culled;
	}
	
	//XPlane
	float tXTest = SCInView.z * m_pCurCamera->m_tanHalfFieldOfViewV;
	if (SCInView.x - pModel->boundingSphere.Radius > tXTest ||
		SCInView.x + pModel->boundingSphere.Radius < -tXTest)
	{
		pModel->culled = true;
		return !pModel->culled;
	}
	
	//YPlane
	float tYTest = tXTest / m_CurViewport->getAspectRatio();
	if (SCInView.y - pModel->boundingSphere.Radius > tYTest ||
		SCInView.y + pModel->boundingSphere.Radius < -tYTest)
	{
		pModel->culled = true;
		return !pModel->culled;
	}
	
	pModel->culled = false;
	return !pModel->culled;
}

//剪裁计算
VOID CScene::FrustrumTest()
{
	return;

	INT mtrlCnt = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = m_vShader.begin();
	for (INT i = 0; i< mtrlCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		pMtrl->bCulled = TRUE;
		
		std::vector<VGS_RenderSubmesh>::iterator it = pMtrl->subMeshList.begin(); 
		for (; it != pMtrl->subMeshList.end(); ++it)
		{
			CModel* pModel =(CModel*)(it->modelPointer);

			//如果有天空模型
			if (pModel->isSkyBox)
			{
				pMtrl->bCulled = FALSE;
			}			
			if (IsInFrustrum(pModel))
			{
				pMtrl->bCulled = FALSE;
				break;
			}
		}
	}
}

//渲染Logo
VOID CScene::RenderLogo()
{
	//创图logo
	RenderOverlay(m_pStartLogo);
	//联盟Logo
	RenderOverlay(m_pUnitedLogo);
	//loading background m_vImage
	RenderOverlay(m_pLoadingBg);
	//loading bar m_vImage
	RenderOverlay(m_pLoadingBar);
}

//-----------------------------------------------------------------------------
// 方法名: RenderLensFlare()
// 参  数: 
// 描  述: 渲染LensFlare
//-----------------------------------------------------------------------------
VOID CScene::RenderLensFlare()
{
}

//-----------------------------------------------------------------------------
// 方法名: CalcScreenCoords()
// 参  数: D3DXVECTOR3* pos,int* ScreenX,int* ScreenY,int* ScreenW
// 描  述: 计算三维坐标的屏幕坐标
//-----------------------------------------------------------------------------
VOID CScene::CalcScreenCoords(CViewport* pViewport, D3DXVECTOR3* pos, INT* ScreenX, INT* ScreenY)
{
	D3DXVECTOR3 vTmp = *pos;
	D3DVIEWPORT9* viewportData = pViewport->GetD3DViewport(); 
	
	D3DXVec3Project(&vTmp, &vTmp, viewportData, &m_MatProj, &m_MatView, NULL); 	
	*ScreenX = (INT)(vTmp.x + 0.5f);
	*ScreenY = (INT)(vTmp.y + 0.5f); 
	
	/*
	INT width = rect.right -rect.left;
	INT height = rect.bottom - rect.top;
	
	  D3DXMatrixMultiply(&m_MatViewProj, &m_MatIdentity, &m_MatView);
	  D3DXMatrixMultiply(&m_MatViewProj, &m_MatViewProj, &m_MatProj);
	  
		D3DXVECTOR4 pos4(pos->x, pos->y, pos->z, 1);
		D3DXVec4Transform(&pos4, &pos4, &m_MatViewProj);
		if (pos4.w < 0)
		m_bLensFlare_Inverse = TRUE;
		else
		{
		pos4 /= pos4.w;
		*ScreenX = (pos4.x + 1) * width / 2;
		*ScreenY = (1 - pos4.y) * height / 2;
		m_bLensFlare_Inverse = FALSE;
		}
	*/
}
//Add by Baojiahui 2007/9/24-----------------------------------------------------------
//设置m_vOverlay的材质，以实现动态更换m_vOverlay的图像以及动态哦m_vOverlay的效果
VOID CScene::SetOverlayTexture()
{
// 
// 	//Overlay材质的不同与模型所使用的材质，其只有一层Texlayer
// 	if (m_CurMtrl != NULL)
// 	{
// 		m_CurTexStage = 0;
// 		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
// 		UINT texLayerCnt = m_CurMtrl->m_vpTexLayers.size();
// 		
// 		m_pd3dDevice->SetSamplerState( m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
// 		m_pd3dDevice->SetSamplerState( m_CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
// 		
// 		m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
// 		m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
// 		m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
// 		//没有贴图
// 		if (texLayerCnt==0)
// 		{
// 			m_pd3dDevice->SetTexture(m_CurTexStage, NULL);
// 			//color
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
// 			
// 			//Alpha半透材质吗?
// 			if ((m_CurMtrl->opacity < 100) || 
// 				(m_CurModel && m_CurModel->opacity < 100))
// 			{
// 				m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
// 				m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
// 			}
// 			else
// 			{
// 				m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
// 			}
// 		}else
// 		{
// 			//遍历贴图层列表逐层按类型贴图
// 			CTextureLayer** ppTexLayer = m_CurMtrl->m_vpTexLayers.begin();
// 			for(UINT i = 0; i<texLayerCnt; i++)
// 			{
// 				CTextureLayer* pTexLayer = *(ppTexLayer+i);
// 				N3D_TextureType type= pTexLayer->type;
// 				if(type == TLT_DYNAMIC)
// 				{
// 					CDynamicTex* pDyTex =(CDynamicTex*)pTexLayer->pTexture;
// 					INT dyFrameCnt = pDyTex->frameCnt;			   
// 					if (dyFrameCnt > 0)
// 					{
// 						pDyTex->Update();
// 						m_pd3dDevice->SetTexture(m_CurTexStage,pDyTex->texture->textureBuf);
// 					}		
// 				}else if(type ==TLT_MOVIE)
// 				{
// // 					CMovieTexture* pMovieTex = ((CMovieTexture*)(pTexLayer->pTexture));
// // 					if (pMovieTex && pMovieTex->m_pMovieTexRender)
// // 					{
// // 						pMovieTex->CheckMovieStatus();
// // 						if (pMovieTex->m_bCreated && pMovieTex->m_pMovieTexRender->m_bCreated)
// // 						{
// // 							//如果已经播放完毕
// // 							if(pMovieTex->m_currentRepeatIndex == pMovieTex->m_repeatCnt)
// // 								m_pd3dDevice->SetTexture(0, ((CTexture*)m_CurMtrl->texLayersList[0])->textureBuf);
// // 							else		
// // 								m_pd3dDevice->SetTexture(m_CurTexStage, pMovieTex->m_pMovieTexRender->m_textureBuf);
// // 						}
// // 					}
// 				}else
// 				{
// 					CTexture* pTex =(CTexture*)pTexLayer->pTexture; 
// 					m_pd3dDevice->SetTexture(m_CurTexStage,pTex->textureBuf);			
// 				}
// 				
// 				if(m_CurTexStage==0)
// 				{
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
// 					
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
// 				}else
// 				{
// 					//设置混色模式
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLOROP, pTexLayer->Blenderinfo.ColorBlendOption);
// 					
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAOP, pTexLayer->Blenderinfo.AlphaBlendOption);
// 					
// 					if(pTexLayer->Blenderinfo.ColorBlendOption==D3DTOP_BLENDFACTORALPHA)
// 						m_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pTexLayer->Blenderinfo.ColorBlendParam);
// 					
// 					if(pTexLayer->Blenderinfo.AlphaBlendOption==D3DTOP_BLENDFACTORALPHA)
// 						m_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pTexLayer->Blenderinfo.AlphaBlendParam);
// 					
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
// 					m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);				
// 				}
// 				m_CurTexStage++;
// 			}
// 			
// 		}	
// 	}else
// 		return;
}

//设置bumpwater材质
// VOID CScene::SetRenderWaterMaterial(CMaterial* pMtrl)
// {
// 	m_CurMtrl = pMtrl;
// 	m_CurTexStage =0;
// 
// 	//第一层为bump层
// 	//-------------------------------------------------------
// 	//设置BumpMap贴图
// 	//-------------------------------------------------------
// 	m_pd3dDevice->SetTexture(m_CurTexStage, bumpWater[m_CurMtrl->bumpWaterIndex]->pBumpTex);
// 	m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2); 
// 
// 	CBumpWater* pBumpWater = bumpWater[m_CurMtrl->bumpWaterIndex];
// 	//D3DXMATRIXA16 m_MatTransWater;
// 	//D3DXMatrixIdentity(&m_MatTransWater);
// 	//m_MatTransWater._32 = -12.0f/60.0f;
// 	//m_pd3dDevice->SetTransform(D3DTS_TEXTURE0, &GetMatriceProjectionTexture());
// 	//m_pd3dDevice->SetTransform(D3DTS_TEXTURE0, &m_MatTransWater);
// 	
// 	
// 
// 	DWORD _timer = GetTickCount();
// 	FLOAT r = 0.08f / pBumpWater->m_radius;
// 	FLOAT multiply = 0.005f * pBumpWater->m_speed;
// 	D3DXMATRIX _mat = pBumpWater->bumpMat;
// 	float value0 = _timer * multiply;
// 	float value1 = r * cosf(value0);
// 	float value2 = r * sinf(value0);
// 	_mat._11 = value1;
// 	_mat._12 = -value2;
// 	_mat._21 = value2;
// 	_mat._22 = value1;
// 	//TextureLayer* pTexLayer = m_CurMtrl->texLayersList[0];
// 	//将m_matBumpMat矩阵的变换到bumpmap里面。
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_BUMPENVMAT00, F2DW( _mat._11 ) );
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_BUMPENVMAT01, F2DW( _mat._12 ) );
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_BUMPENVMAT10, F2DW( _mat._21 ) );
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_BUMPENVMAT11, F2DW( _mat._22 ) );
// 
// //	m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_BUMPENVLSCALE, F2DW(1.0f) );
//  //   m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_BUMPENVLOFFSET, F2DW(0.3f) );
// 	//"using the environment map in the next texture stage, without luminance. 
// 	//This operation is supported only for color operations (D3DTSS_COLOROP). "
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
//     
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	
// 	//m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
//     m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
// 		
// 	m_CurTexStage++;
// 	//-------------------------------------------------------
// 	//设置Diffuse贴图
// 	//-------------------------------------------------------
// 	if(m_CurMtrl->m_vpTexLayers.size()>1)
// 	{
// 		m_pd3dDevice->SetTexture(m_CurTexStage, bumpWater[m_CurMtrl->bumpWaterIndex]->curRenderTexture);
// 		// 
// 		m_pd3dDevice->SetTextureStageState (m_CurTexStage, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
// 		m_pd3dDevice->SetTextureStageState (m_CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 		m_pd3dDevice->SetTextureStageState (m_CurTexStage, D3DTSS_ALPHAARG1,  D3DTA_TEXTURE);
// 		m_pd3dDevice->SetTextureStageState (m_CurTexStage, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
// 		//m_pd3dDevice->SetTextureStageState (m_CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
// 		// uv矩阵(投影矩阵,将贴图映射到水面用):
// 		m_pd3dDevice->SetTransform(D3DTS_TEXTURE1, &GetMatriceProjectionTexture());
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED |D3DTTFF_COUNT3);
// 		m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSU,  D3DTADDRESS_CLAMP);
// 		m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSV,  D3DTADDRESS_CLAMP); 
// 	}else
// 	{
// 	   if(m_CurMtrl->m_vpTexLayers.size() > 0)
// 	    SetTextureLayer(m_CurMtrl->m_vpTexLayers[0]);	   
// 	}
// }

//设置贴图层
// VOID CScene::SetTextureLayer(CTextureLayer* pTexLayer)
// {
// 	switch(pTexLayer->type)
// 	{
// 	//普通贴图通道
// 	case TLT_BASAL:
// 	//透明贴图通道
// 	case TLT_OPACITY:
// 		//设置贴图坐标的Wrap模式
// // 		if(bIsOverlayMtrl) //Overlay的贴图直接平铺
// // 		{
// // 			m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
// // 		    m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);		
// // 		}
// // 		else	//普通模型的贴图采用贴图坐标包裹
// 		{
// 		   m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
// 		   m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
// 		}
// 
// 		//设置该通道贴图使用的贴图坐标通道
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);
// 
// 		//设置贴图Buffer
// 		m_pd3dDevice->SetTexture(m_CurTexStage, ((CTexture*)(pTexLayer->pTexture))->textureBuf);
// 
// 		//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
// 		if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
// 		{
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
// 			MoveTextureUV(m_CurTexStage, pTexLayer->USpeed, pTexLayer->VSpeed, D3DTTFF_COUNT2);
// 		}
// 		else//关掉贴图的变换，防止上一个状态对贴图状态的影响
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
// 	
// 		break;
// 
// 	//凹凸贴图通道
// 	case TLT_BUMPMAP:
// 		{
// 			//凹凸贴图对象
// 			CBumpMap* pBumpMap = (CBumpMap*)pTexLayer->pTexture;
// 			//跳过，不在这里渲染，因为我们希望凹凸水面对所有贴图层起作用
// 			if (pBumpMap->m_type == BUMPWAVEMAP) return;
// 
// 			//设置贴图坐标的Wrap模式
// 			m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
// 			m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
// 
// 			//设置该通道贴图使用的贴图坐标通道
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);
// 
// 			//凹凸反射
// 			if (pBumpMap->m_type == BUMPENVMAP)
// 			{
// 				//设置贴图Buffer
// 				m_pd3dDevice->SetTexture(m_CurTexStage, pBumpMap->m_pBumpEnvMap);
// 
// 				//设置贴图混色的两个源，Bump贴图放在上一层
// 				m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE );
// 				m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_COLORARG2, D3DTA_CURRENT );
// 
// 				//设置混色模式
// 				if( pBumpMap->m_BumpMapFormat == D3DFMT_V8U8 )  //让材质上的环境反射图产生凹凸效果
// 					m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);	
// 				else //贴图上产生明暗效果
// 					m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_COLOROP, D3DTOP_BUMPENVMAPLUMINANCE);	
// 
// 				//设置Bump贴图通道的相关参数，产生凹凸效果
// 				m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_BUMPENVMAT00, F2DW(pBumpMap->m_Noise1));
// 				m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
// 				m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
// 				m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_BUMPENVMAT11, F2DW(pBumpMap->m_Noise2));
// 				m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_BUMPENVLSCALE, F2DW(4.0f));
// 				m_pd3dDevice->SetTextureStageState( m_CurTexStage, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f));
// 			}
// 			//Dot3Product 法线贴图
// 			else if (pBumpMap->m_type == BUMPNORMALMAP)
// 			{
// 				//设置贴图Buffer
// 				m_pd3dDevice->SetTexture(m_CurTexStage, pBumpMap->m_pNormalMap);
// 
// 				// 向量变换为RGB
// 				pBumpMap->VectortoRGBA(0);	
// 				//变换为RGB的向量登录为TextureFactor值
// 				m_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pBumpMap->m_dwFactor);
// 
// 				//纹理的RGB和光源向量内积运算
// 				m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);	
// 				//运算中使用内积	
// 				m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG2, D3DTA_TFACTOR);
// 				m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
// 			}
// 		}
// 		break;
// 
// 	//实时生成反射贴图通道
// 	case TLT_RTFMAP:
// 		{
// 			//设置贴图坐标的Wrap模式
// 			m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
// 			m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
// 			m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
// 			
// 			//设置该通道贴图使用的贴图坐标为相机空间贴图坐标
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
// 			//设置该贴图通道的矩阵变换
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3 | D3DTTFF_PROJECTED);
// 			//设置贴图Buffer
// 			CRTFMap* pRTFMap = (CRTFMap*)pTexLayer->pTexture;
// 			m_pd3dDevice->SetTexture(m_CurTexStage, pRTFMap->m_texBuf);
// 			
// 			//设置贴图的变换矩阵
// 			m_pd3dDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + m_CurTexStage), &(pRTFMap->m_texProjMatrix));
// 			
// 		}
// 		break;
// 		
// 	//静态反射贴图通道
// 	case TLT_REFLECT:
// 		//设置贴图坐标的Wrap模式
// 		m_pd3dDevice->SetSamplerState( m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
// 		m_pd3dDevice->SetSamplerState( m_CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
// 		m_pd3dDevice->SetSamplerState( m_CurTexStage, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
// 		
// 		//设置该通道贴图使用的贴图坐标为相机空间反射贴图坐标
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);
// 		//设置该贴图通道的矩阵变换
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, pTexLayer->transformFlag);
// 		//设置贴图Buffer
// 		m_pd3dDevice->SetTexture(m_CurTexStage, ((CTexture*)(pTexLayer->pTexture))->textureBuf);
// 
// 		//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
// 		if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
// 			MoveTextureUV(m_CurTexStage, pTexLayer->USpeed, pTexLayer->VSpeed, pTexLayer->transformFlag);
// 
// 		break;
// 
// 	//实时阴影贴图通道
// 	case TLT_REALTIMESHADOW:
// 		//暂未实现，保留接口
// 		break;
// 
// 	//立方图贴图通道
// 	case TLT_CUBEMAP:
// 		{
// 			//获得CubeMap对象
// 			CCubeMap* pCubeMap = (CCubeMap*)pTexLayer->pTexture;
// 			if (!pCubeMap) break;
// 
// 			//设置贴图坐标的Wrap模式
// 			m_pd3dDevice->SetSamplerState( m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
// 			m_pd3dDevice->SetSamplerState( m_CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
// 			m_pd3dDevice->SetSamplerState( m_CurTexStage, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
// 
// 			//设置该通道贴图使用的贴图坐标为相机空间反射贴图坐标
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
// 			//设置该贴图通道的矩阵变换
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
// 			//设置贴图Buffer
// 			m_pd3dDevice->SetTexture(m_CurTexStage, pCubeMap->m_pCubeTex);
// 
// 			//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
// 			if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
// 				MoveTextureUV(m_CurTexStage, pTexLayer->USpeed, pTexLayer->VSpeed, D3DTTFF_COUNT3);
// 		}
// 		break;
// 
// 	//动态贴图通道
// 	case TLT_DYNAMIC:
// 		{
// 			//设置贴图坐标的Wrap模式
// 			m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
// 			m_pd3dDevice->SetSamplerState(m_CurTexStage, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
// 			
// 			//设置该通道贴图使用的贴图坐标通道
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXCOORDINDEX, pTexLayer->UVIndex);
// 
// 			//新版引擎将动态UV设置为模型所在材质的每个textureLayer中
// 			if(pTexLayer->USpeed != 0 ||pTexLayer->VSpeed != 0)
// 			{
// 				m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
// 				MoveTextureUV(m_CurTexStage, pTexLayer->USpeed, pTexLayer->VSpeed, D3DTTFF_COUNT2);
// 			}
// 			else//关掉贴图的变换，防止上一个状态对贴图状态的影响
// 				m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
// 
// 			//设置动态贴图Buffer
// 			CDynamicTex* pDyTex =(CDynamicTex*)pTexLayer->pTexture;
// 			if (pDyTex->m_frameCnt > 0)
// 			{
// 				pDyTex->Update();
// 				m_pd3dDevice->SetTexture(m_CurTexStage, pDyTex->m_curTexBuf);
// 			}
// 
// 		}
// 		break;
// 
// 	/*
// 	//实时凹凸通道
// 	case TLT_BUMPWATER:
// 		//暂未实现，用于动态波动表面(譬如水、云)的渲染
// 		break;
// 		*/
// 
// 	//电影贴图通道
// 	case TLT_MOVIE:
// 		//暂未实现，用于实现电影贴图
// 		break;
// 
// 	//Flash贴图通道
// 	case TLT_FLASH:
// 		//暂未实现，用于实现Flash贴图
// 		break;
// 	
// 	//贴图层不存在
// 	case TLT_INVALID:
// 		break;
// 
// 	//暂不不支持的贴图通道
// 	default:
// 		break;
// 	}
// 	
// 	//设置混色模式
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_TEXTURE);
// 	//if(pTexLayer->type!=TLT_CUBEMAP)
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLOROP, pTexLayer->Blenderinfo.ColorBlendOption);
// 	
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	
// 	m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAOP, pTexLayer->Blenderinfo.AlphaBlendOption);
// 	
// 	if(pTexLayer->Blenderinfo.ColorBlendOption==D3DTOP_BLENDFACTORALPHA)//&& pTexLayer->type!=TLT_CUBEMAP)
// 		m_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pTexLayer->Blenderinfo.ColorBlendParam);
// 	
// 	if(pTexLayer->Blenderinfo.AlphaBlendOption==D3DTOP_BLENDFACTORALPHA)
// 		m_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, pTexLayer->Blenderinfo.AlphaBlendParam);
// 	
// 	if(m_CurTexStage ==0)
// 	{			   
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
// 		/*	if(m_CurMtrl->selfIllumination != 100)
// 		{
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG2, D3DTA_TEXTURE);
// 	}*/
// 	}
// 	else
// 	{			
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG2, D3DTA_CURRENT);
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
// 	}
// }
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//0通道--设置Diffuse漫射贴图
//设置第0层贴图
VOID CScene::SetMaterial(CMaterial* pMtrl)
{
	if (!pMtrl)
	{
		return;
	}

	m_CurMtrl = pMtrl;
	m_CurTexStage = 0;

	m_CurMtrl->SetToDevice(m_pd3dDevice, FALSE);
	//判断是否是双面材质
// 	if (m_CurMtrl->twoSide )
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
// 	}
// 	else
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
// 	}
// 
// 	//自发光材质
// 	if (m_CurMtrl->selfIllumination == 100)
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
// 		m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
// 	}																																																				
// 	else 
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
// 		m_pd3dDevice->SetMaterial(&(m_CurMtrl->d3dmtrl));
// 		m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
// 		m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, m_CurMtrl->type);
// 		// m_pd3dDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);   // fuxb 2008-09-07
// 	}
// 
// 	//如果模型是半透明模型，设置ZWRITEENABLE为False，并打开Alpha混色模式
// 	if (m_CurMtrl->bAlphaBlend) //opacity<100则bAlphaBlend为true
// 	{
// 		//渲染半透明物体的渲染状态
// 		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
// 		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
// 		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
// 		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
// 		
// 		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
// 		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
// 	}
// 	//如果是不透明物体，设置ZWRITEENABLE为True, 并关闭Alpha混色
// 	else
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
// 		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
// 	}
// 	
// 	//材质自发光状态的设置，Overlay默认为自发光材质
// 	if (m_CurMtrl->selfIllumination == 100)
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
// 		m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
// 	}																																																				
// 	else 
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
// 		m_pd3dDevice->SetMaterial(&m_CurMtrl->d3dmtrl);
// 		m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
// 		m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, m_CurMtrl->type);
// 	}
// 	
// 	//是否打开Alpha测试
// 	if(m_CurMtrl->bAlphaTest)
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
// 		m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, m_CurMtrl->AlpahaTestValue);
// 		m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
// 	}
// 	else
// 	{
// 		m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
// 	}
// 	
// //m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
// 
// 	//TextureLayer
// 	
// 	UINT texLayerCnt = m_CurMtrl->m_vpTexLayers.size();
//     //没有贴图
// 	if (texLayerCnt==0)
// 	{
// 		m_pd3dDevice->SetTexture(m_CurTexStage, NULL);
// 		//color
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
// 		m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
// 		
// 		//Alpha半透材质吗?
// 		if ((m_CurMtrl->opacity < 100) || 
// 			(m_CurModel && m_CurModel->opacity < 100))
// 		{
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
// 		}
// 		else
// 		{
// 			m_pd3dDevice->SetTextureStageState(m_CurTexStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
// 		}
// 	}else
// 	{
// 		//遍历贴图层列表逐层按类型贴图
// 		CTextureLayer** ppTexLayer = m_CurMtrl->m_vpTexLayers.begin();
// 
// 		for(UINT i = 0; i<texLayerCnt; i++)
// 		{
// 			CTextureLayer* pTexLayer = *(ppTexLayer+i);
// 			//设置贴图层状态
// 			SetTextureLayer(pTexLayer);
// 			m_CurTexStage++;
// 		}
// 	
// 	}	
}
//-----------------------------------------------------------------------------
// 方法名: RenderModels()
// 参  数: 
// 描  述: 渲染使用当前材质的模型
//-----------------------------------------------------------------------------
void CScene::RenderModels()
{
	//清除所有动态CubeMap的表面
	// ClearDynamicCubeMapSurface();
	
	//Turn on the zbuffer	
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	
	//渲染水面
	RenderRealWater();

	/*std::vector<CModel*>::iterator it = model.begin();
	for (DWORD i = 0; i< model.size(); i++)
	{
		CModel* pModel = *(it + i);

		pModel->render();
	}*/

	if (m_pAxis)
		m_pAxis->Draw(m_CurViewport->GetD3DViewport(), &m_MatView, &m_MatProj);
	//渲染模型的坐标系--2010-1-12
// 	std::vector<CModel*>::iterator it = model.begin(), end = model.end();
// 	for (; it != end; ++it)
// 	{
// 		if ((*it) != NULL)
// 		{
// 			(*it)->RenderAxis(m_CurViewport->GetD3DViewport(), &m_MatView, &m_MatProj);
// 		}
// 	}

	//渲染不透明模型
	RenderOpacModels(); 

	//渲染角色动画
	RenderA3D();


	///渲染透明模型
	RenderTransModels();	
	
	
	//切换动态CubeMap表面
	// SwtichDynamicCubeMapSurface();
}


//渲染角色动画
VOID CScene::RenderA3D()
{		
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTSS_COLORARG1 );//D3DTOP_MODULATE);
	

	// 骨骼动画
	m_A3DManager.render(30); 
}


//清除所有动态CubeMap的表面
VOID CScene::ClearDynamicCubeMapSurface()
{
	int cubeMapCnt = cubeMap.size();
	std::vector<CCubeMap*>::iterator ppCubeMap = cubeMap.begin();
	for (int i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* pCubeMap = *(ppCubeMap + i);
		//if (pCubeMap->m_Mode == 1)
		//	pCubeMap->ClearSurface(pCubeMap->m_pCubeMap_back, pCubeMap->m_pCubeMap_ZBuffer);
	}
}


//切换所有动态CubeMap的表面
VOID CScene::SwtichDynamicCubeMapSurface()
{
	int cubeMapCnt = cubeMap.size();
	std::vector<CCubeMap*>::iterator ppCubeMap = cubeMap.begin();
	for (int i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* pCubeMap = *(ppCubeMap + i);
		//		if (pCubeMap->m_Mode == 1)
		//			pCubeMap->Switch();
	}
}


//-----------------------------------------------------------------------------
// 方法名: RenderRealWater()
// 参  数: 
// 描  述: 渲染场景中的真实水面
//-----------------------------------------------------------------------------
VOID CScene::RenderRealWater()
{
	//释放RealWater的D3D对象
	int realWaterCnt = m_vRealWater.size();
	std::vector<CRealWater*>::iterator ppRealWater = m_vRealWater.begin();
	for (int i = 0; i< realWaterCnt; i++)
	{
		CRealWater* pRealWater = *(ppRealWater + i);
		pRealWater->SetViewCamera(m_pCurCamera);
		pRealWater->Render();
	}
}

//-----------------------------------------------------------------------------
// 方法名: RenderOpacModels()
// 参  数: 
// 描  述: 渲染不透明模型
//-----------------------------------------------------------------------------
VOID CScene::RenderOpacModels()
{	
	
	//渲染不透明物体的渲染状态
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	
	//-------------------------------------------------------------------------
	//把模型按照材质分组渲染，节省显示设备切换材质和贴图状态的开销
	//在这里设置材质和贴图
	//-------------------------------------------------------------------------
	INT shaderCnt = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = m_vShader.begin();	
	for (int i=0; i< shaderCnt; i++)
	{
		//设置材质
		m_CurMtrl = *(ppMtrl + i);	
		//透明材质，或AlphaTest材质
		// if (/*m_CurMtrl->bAlphaBlend || */m_CurMtrl->getOpacity() < 100)
		if (m_CurMtrl->IsTransparent())
			continue;
		
		//如果材质的所有模型都被剪裁
		if (m_CurMtrl->bCulled)
			continue;
		
		//判断贴图是否已经下载，如果没有下载完，则不显示
		if (!m_CurMtrl->IsReadyToRender())
			continue;
		
		// 设置到设备
		m_CurMtrl->SetToDevice(m_pd3dDevice, 0);

		//渲染使用当前材质的模型
		RenderModelsByMtrl(/*m_CurMtrl*/);
	}
	
	//初始化变量
	m_CurMtrl = NULL;
	m_CurModel = NULL;
}
//-----------------------------------------------------------------------------
// 方法名: RenderSubMeshToRTFTexture()
// 参  数: CRTFMAP* pRTFMap, RenderSubMesh* pSubMesh, CMaterial* pMtrl
// 描  述: 渲染SubMesh到实时贴图表面
//-----------------------------------------------------------------------------
VOID CScene::RenderSubMeshToRTFTexture(CRTFMap* pRTFMap, VGS_RenderSubmesh* pSubMesh/*, CMaterial* pMtrl*/)
{
	CModel* pModel = (CModel*)pSubMesh->modelPointer;

	//为凹凸材质渲染需要的其它贴图层合成矩阵初始化
	pRTFMap->m_texProjMatrix = GetTextureProjMatrix();

	//设置视角变换
	D3DXMATRIX matView, matViewSaved;
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matViewSaved );
	D3DXMatrixMultiply(&matView, &pRTFMap->m_refMatrix, &matViewSaved);
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	//设置新的渲染表面----------------------------------------------------------------
	m_pd3dDevice->SetRenderTarget(0, pRTFMap->m_pRenderTarget);
	m_pd3dDevice->SetDepthStencilSurface(pRTFMap->m_pZBuf);

	//判断点序是否反转，反射贴图则需要反转--------------------------------------------
	//if (!pMtrl->twoSide)
	//	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//设置渲染的剪切平面，剪掉不需要渲染的面片
	m_pd3dDevice->SetClipPlane( 0, pRTFMap->m_refPlane);
	m_pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1);

	//绘制模型到新的表面--------------------------------------------------------------
	pModel->RenderSubMesh(pSubMesh->subMeshID);

	//恢复			
	//设置回的原有渲染表面------------------------------------------------------------
	m_pd3dDevice->SetRenderTarget(0, m_pBackBuffer);
	m_pd3dDevice->SetDepthStencilSurface(m_pZBuffer);
	m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 0);
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matViewSaved);
	//if (!pMtrl->twoSide)
	//	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}
void CScene::RenderMeshToRTFTexture(CRTFMap* pRTFMap)
{
	pRTFMap->m_texProjMatrix = GetTextureProjMatrix();

	//设置视角变换
	D3DXMATRIX matView, matViewSaved;
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matViewSaved );
	D3DXMatrixMultiply(&matView, &pRTFMap->m_refMatrix, &matViewSaved);
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	
// 	D3DXMATRIX matProj, matOldProj;
// 	m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matOldProj );
//  	D3DXMatrixPerspectiveFovLH(&matProj,D3DX_PI * .25f,1,1,10000);
// // 	D3DXMatrixOrthoLH(&matProj, 1280, 800, 0, 100000);
// 	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	//设置新的渲染表面----------------------------------------------------------------
	m_pd3dDevice->SetRenderTarget(0, pRTFMap->m_pRenderTarget);
	m_pd3dDevice->SetDepthStencilSurface(pRTFMap->m_pZBuf);

	//对渲染目标进行清空--added by linxq 2009-6-24
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET,0X00FFFFFF , 1.0f, 0 ); 

	//判断点序是否反转，反射贴图则需要反转--------------------------------------------
	if (!pRTFMap->m_pMaterial->twoSide)
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//设置渲染的剪切平面，剪掉不需要渲染的面片
	m_pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 1);
	m_pd3dDevice->SetClipPlane( 0, pRTFMap->m_refPlane);

	//绘制所有被反射模型到新的表面--------------------------------------------------------------
	CModel* pModel = NULL;
	for (size_t i = 0; i < pRTFMap->GetReflectedModelCount(); i++)
	{
		pModel = (CModel*)(pRTFMap->GetReflectedModelByIndex(i));
		pModel->render();
	}
	pRTFMap->UpdateTexBuf();
// 	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matOldProj );
}

//-----------------------------------------------------------------------------
// 方法名: RenderSubMeshToBumpWaveCompositeTexture()
// 参  数: CBumpMap* pBumpMap, RenderSubMesh* pSubMesh, CMaterial* pMtrl)
// 描  述: 渲染SubMesh到凹凸混合贴图表面
//-----------------------------------------------------------------------------
VOID CScene::RenderSubMeshToBumpWaveCompositeTexture(CBumpMap* pBumpMap, VGS_RenderSubmesh* pSubMesh, CMaterial* pMtrl)
{
	CModel* pModel = (CModel*)pSubMesh->modelPointer;
	if (!pModel || !pMtrl)
	{
		return;
	}
	
	//为凹凸材质渲染需要的其它贴图层合成矩阵初始化
	pBumpMap->m_compositeTexPorjMatrix = GetTextureProjMatrix();

	//设置新的渲染表面----------------------------------------------------------------
	m_pd3dDevice->SetRenderTarget(0, pBumpMap->m_pRenderTarget);

	//设置材质，不渲染Bump贴图层
	pMtrl->SetToDevice(m_pd3dDevice, 0);

	//绘制模型到新的表面--------------------------------------------------------------
	pModel->RenderSubMesh(pSubMesh->subMeshID);
		
	//设置回的原有渲染表面------------------------------------------------------------
	m_pd3dDevice->SetRenderTarget(0, m_pBackBuffer);
}	

//-----------------------------------------------------------------------------
// 方法名: RenderModelToDynamicCubeMapSurface()
// 参  数: CModel* pModel, 开启动态CubeMap的模型，
//			RenderSubeMesh* pSubMesh, 本次渲染的Mesh
//			CCubeMap* pCubeMap, 渲染的目标
// 描  述: 如果该模型用到了动态的CubeMap，则在这里将该模型渲染到CubeMap的表面上
//-----------------------------------------------------------------------------
VOID CScene::RenderModelToDynamicCubeMapSurface(CModel* pModel, VGS_RenderSubmesh* pSubMesh, CCubeMap* pCubeMap)
{
	//保存当前的渲染目标和ZBuffer
	LPDIRECT3DSURFACE9 pLastRenderTarget;
	LPDIRECT3DSURFACE9 pLastZBuffer;
	m_pd3dDevice->GetRenderTarget(0, &pLastRenderTarget);
	m_pd3dDevice->GetDepthStencilSurface(&pLastZBuffer);

	//计算本次渲染的View矩阵
	//D3DXMATRIX cubeMatView;
	//cubeMatView = m_MatView;
	//cubeMatView._41 = 0; cubeMatView._42 = 0; cubeMatView._43 = 0;
	//位置变换
	D3DXMATRIX posMat;
	CModel* pCenterModel = (CModel*)(pCubeMap->m_pCenterModel);
	D3DXVECTOR3 camPos = pCenterModel->m_pNode->getMatrix(VTS_WORLD);// curTransform.position;
	D3DXMatrixTranslation(&posMat, camPos.x, camPos.y, camPos.z);
	//D3DXMatrixMultiply(&cubeMatView, &posMat, &cubeMatView);

	//设置投影矩阵90度
	D3DXMATRIX matProj;
	float nearClip = 0.5f;
	float farClip = 1000.0f;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/2.0, 1.0f, nearClip, farClip );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	
	//渲染CubeMap的6个表面，到BackCubeMap Surface
	for(UINT i = 0; i < 6; i++ )
	{	
		LPDIRECT3DSURFACE9 pCubeMapFace;
		//设置第i个表面的RenderTarget
		pCubeMap->m_pCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)i, 0, &pCubeMapFace );
		m_pd3dDevice->SetRenderTarget(0, pCubeMapFace);
		m_pd3dDevice->SetDepthStencilSurface(pCubeMap->m_pZBuffer);

		//设置第i个表面渲染的View矩阵
		D3DXMATRIX matView;
		matView = GetCubeMapViewMatrix( (D3DCUBEMAP_FACES)i ); 
		D3DXMatrixMultiply(&matView, &posMat, &matView);
		// D3DXMatrixMultiply( &matView, &cubeMatView, &matView);
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
		
		//判断模型是否被剪切
		//BOOL bCulled = IsInFrustrum(pModel, &camPos, &matView, 1, nearClip, farClip);
		//if (!bCulled)
		//{
		//	//改变索引缓冲区的面的数量将会较大幅度的提升运算速度**************
		//	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		//		pModel->vertexCnt, //当前模型的顶点数
		//		0/*m_CurModel->m_pIndices[0]*/, //当前模型的索引缓冲区起点
		//		pModel->subMesh[pSubMesh->subMeshID].faceCnt); //当前模型的面数						
		//}
		pModel->render();

		
		SAFE_RELEASE(pCubeMapFace);
	}
	
	//恢复变换矩阵
	// m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_MatWorld );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_MatView );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_MatProj );
	
	//恢复渲染表面
	m_pd3dDevice->SetRenderTarget(0, pLastRenderTarget);
	m_pd3dDevice->SetDepthStencilSurface(pLastZBuffer);

	// SAFE_RELEASE(pCubeMapFace);
	SAFE_RELEASE(pLastRenderTarget);
	SAFE_RELEASE(pLastZBuffer);
}

// 添加一个动态cubemap对象到列表
VOID CScene::addDynamicCubemap(CCubeMap* pCubemap)
{
	m_vpDynamicCubemap.push_back(pCubemap);
}

VOID CScene::addBumpmap(CBumpMap* pBumpmap)
{
	m_vBumpTexture.push_back(pBumpmap);
}

//-----------------------------------------------------------------------------
// 方法名: GetTextureProjMatrix()
// 参  数: 
// 描  述: 计算RenderToTexture的二次渲染贴图的投影矩阵
//-----------------------------------------------------------------------------
D3DXMATRIX CScene::GetTextureProjMatrix()
{
	D3DXMATRIX m_texProjMatrix;
	FLOAT yScale = -0.5f / tan(m_pCurCamera->GetFov() * 0.5f);
	FLOAT xScale = -yScale / m_CurViewport->getAspectRatio();
	
	m_texProjMatrix._11 = xScale; m_texProjMatrix._12 =   0.0f; m_texProjMatrix._13 = 0.0f; m_texProjMatrix._14 = 0.0f;
	m_texProjMatrix._21 =   0.0f; m_texProjMatrix._22 = yScale; m_texProjMatrix._23 = 0.0f; m_texProjMatrix._24 = 0.0f;
	m_texProjMatrix._31 =   0.5f; m_texProjMatrix._32 =   0.5f; m_texProjMatrix._33 = 1.0f; m_texProjMatrix._34 = 1.0f;
	m_texProjMatrix._41 =   0.0f; m_texProjMatrix._42 =   0.0f; m_texProjMatrix._43 = 0.0f; m_texProjMatrix._44 = 0.0f;
	
	return m_texProjMatrix;
}
// 
// VOID CScene::DrawOpacModel()
// {
// 	//使用该材质的模型
// 	RenderSubMesh* pSubMesh = (RenderSubMesh*)(m_CurMtrl->subMeshList.begin());
// 	int subMeshCnt = m_CurMtrl->subMeshList.size();
// 
// 	for (INT j = 0; j< subMeshCnt; j++)
// 	{
// 		RenderSubMesh subMesh = *(pSubMesh + j);
// 		
// 		m_CurModel = (CModel*)subMesh.modelPointer;	
// 
// 		if (!m_CurModel->visible)
// 		{
// 			continue;
// 		}
// 		
// 		//如果模型是半透明模型, 不能在这里渲染
// 		if (m_CurModel->opacity < 100)
// 		{
// 			m_CurMtrl = NULL;
// 			continue; //忽略，自动使用透明方法渲染
// 		}
// 
// 		//  设置是否重新计算模型法线，保证缩放模型能正常显示高光
// 		if (m_CurModel->isAutoNormalise)
// 		{
// 			m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, (BOOL)m_CurModel->isAutoNormalise);
// 		}
// 
// 		//如果是天空模型
// 		if (m_CurModel->isSkyBox)
// 		{
// 			//处理模型的移动和旋转、缩放
// 			m_pd3dDevice->SetTransform( D3DTS_WORLD, &(m_CurModel->pNode->curTransform.worldMatrix));
// 			//暂停雾渲染
// 			m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
// 			
// 			//for SKYBOX
// 			m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
// 			m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
// 			
// 			D3DXVECTOR3 t_CamAt = m_pCurCamera->ori_from + m_pCurCamera->m_at - m_pCurCamera->m_from;
// 			D3DXMatrixLookAtLH(&m_SkyBoxMatView, &(m_pCurCamera->ori_from), &t_CamAt, &(m_pCurCamera->m_up));
// 			m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_SkyBoxMatView);
// 			
// 			//proj matrix
// 			D3DXMatrixPerspectiveFovLH(&m_SkyBoxMatProj, 
// 				m_pCurCamera->GetFov(),  
// 				ratio,
// 				m_pCurCamera->nearClip,
// 				100000000.0f);		
// 			m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_SkyBoxMatProj);
// 		}
// 		
// 		//不是天空模型
// 		else
// 		{
// 			//关键帧动画
// // 			PlayModelKeyFrame();
// // 			
// // 			//设置模型的MoveModelTo, 必须放在这里，否则当物体离视点较远时，不处理了
// // 			if (m_CurModel->user_moveFrameCnt > 0)
// // 				PlayUserMoveModelKFR();
// // 			
// // 			
// // 			//模型的可见性
// // 			if( m_CurModel->visible != TRUE ||
// // 			m_CurModel->isCollision ||
// // 			m_CurModel->isGround)
// // 			continue;
// // 			
// // 			//模型是否在视锥体内
// // 			IsInFrustrum(m_CurModel);
// // 			if (m_CurModel->culled) 
// // 				continue;
// // 			
// // 			//模型是否是billboard模型
// // 			if (m_CurModel->isBillboard)
// // 				SetModelBillboard(m_CurModel);
// 			
// 			//设置该模型当前帧的矩阵
// 			//	if (m_CurModel->kfrCnt > 0 && m_CurModel->kfrRepeatCnt != 0)
// 			m_pd3dDevice->SetTransform( D3DTS_WORLD, &(m_CurModel->pNode->curTransform.worldMatrix));
// 			//	else
// 			//		m_pd3dDevice->SetTransform( D3DTS_WORLD, &(m_CurModel->transform.ori_matrix));
// 			
// 			//雾渲染
// // 			if(m_bFog)
// // 				m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
// // 			else
// // 				m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
// // 			
// // 			//模型是否是动态UV模型
// // 			if (m_CurModel->bDynamicUV)
// // 				m_CurModel->MoveTextureUV(m_CurModel->UStep, m_CurModel->VStep);
// // 			
// // 			//看看当前模型是否有索引模型，如果有索引模型，则用索引模型的顶点缓冲及索引缓冲
// // 			if (m_CurModel->pCloneModel != NULL)
// // 				m_CurModel = m_CurModel->pCloneModel;
// 			
// 			//m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_MatView);
// 			//m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_MatProj);	
// 			
// 
// 		}
// 		
// 		//渲染模型
// 		m_pd3dDevice->SetStreamSource( 0, m_CurModel->m_VBuffer, 0, m_CurModel->m_FVFSize );
// 		//设置FVF
// 		m_pd3dDevice->SetFVF(m_CurModel->m_FVF); //DX9为SetFVF,DX8用这个函数
// 		//渲染SubMesh
// 		//设置IB
// 		m_pd3dDevice->SetIndices(m_CurModel->subMesh[subMesh.subMeshID].m_IBuffer);
// 		
// 		int facecount = m_CurModel->subMesh[subMesh.subMeshID].faceCnt;
// 		//改变索引缓冲区的面的数量将会较大幅度的提升运算速度**************
// 		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST , 0, 0,
// 			m_CurModel->vertexCnt, //当前模型的顶点数
// 			0/*m_CurModel->m_VBuffer[0]*/, //当前模型的索引缓冲区起点
// 			m_CurModel->subMesh[subMesh.subMeshID].faceCnt); //当前模型的面数
// 		
// 		//判断是否需要反射运算
// 		//if(beReflect)
// 		//	RenderOpacReflect(subMesh);
// 	}		
// }

VOID CScene::RenderOpacReflect(const VGS_RenderSubmesh& subMesh)
{	
	if(m_CurModel->bReflect == TRUE)
	{
		// Save the view matrix
		D3DXMATRIX matViewSaved;
		m_pd3dDevice->GetTransform( D3DTS_VIEW, &matViewSaved );
		//被反射物体将其反射图像渲染到反射表面
		UINT refedModelsCnt = m_CurModel->refedModelIdxList.size();
		for(UINT i = 0; i< refedModelsCnt;  i++)
		{
			//更换渲染目标
			m_pd3dDevice->SetRenderTarget(0, reflectInfoList[m_CurModel->refedModelIdxList[i]].renderToTexInfo.curRenderZSurface);
			//获得反射矩阵
			D3DXMATRIX matView, matReflect;
			D3DXMatrixReflect( &matReflect, &reflectInfoList[m_CurModel->refedModelIdxList[i]].refPlane);
			D3DXMatrixMultiply( &matView, &matReflect, &matViewSaved );
			m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
			//设置反射纹理的贴图坐标
			
			UINT stage = reflectInfoList[m_CurModel->refedModelIdxList[i]].renderToTexInfo.texStage;
			m_pd3dDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage),&GetMatriceProjectionTexture());
			// Set a clip plane, so that only objects above the water are reflected
			m_pd3dDevice->SetClipPlane( 0, reflectInfoList[m_CurModel->refedModelIdxList[i]].refPlane );
			m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 0x01 );
			
			if (m_CurMtrl->twoSide )
				m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
			else
				m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			
			m_CurModel->render();
			//m_pd3dDevice->SetIndices(m_CurModel->subMesh[subMesh.subMeshID].m_IBuffer);	

			////改变索引缓冲区的面的数量将会较大幅度的提升运算速度**************
			//m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST , 0, 0,
			//	m_CurModel->vertexCnt, //当前模型的顶点数
			//	0/*m_CurModel->m_pIndices[0]*/, //当前模型的索引缓冲区起点
			//	m_CurModel->subMesh[subMesh.subMeshID].faceCnt); //当前模型的面数

		}
		//反射物体将上一帧的反射图像清空
		if(m_CurModel->sceneRefInfoListIdx != -1)
		{
			m_pd3dDevice->SetRenderTarget(0, reflectInfoList[m_CurModel->sceneRefInfoListIdx].renderToTexInfo.curRenderZSurface);
			//清空后缓冲
			m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_backgroundColor, 1.0f , 0L ); 					
			
		}
		// Restore render states
		if (m_CurMtrl->twoSide )
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
		else
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		m_pd3dDevice->SetRenderTarget(0, m_pZBuffer);
		m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE,  0x00 );
		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matViewSaved );
	}
	
}
//-----------------------------------------------------------------------------
// 方法名: RenderTransModels()
// 参  数: 
// 描  述: 渲染场景子对象-透明模型
//-----------------------------------------------------------------------------
VOID CScene::RenderTransModels()
{
	//渲染半透明物体的渲染状态
// 	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
// 	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
// 	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
// 	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
// 	
// 	//-------------------------------------------------------------------------
// 	//把模型按照材质分组渲染，节省显示设备切换材质和贴图状态的开销
// 	//在这里设置材质和贴图
// 	//-------------------------------------------------------------------------
// 	INT shaderCnt = m_vShader.size();
// 	CMaterial** ppMtrl = m_vShader.begin();	
// 	for (UINT i=0; i< shaderCnt; i++)
// 	{
// 		//清空贴图
// 		ClearTextureMem();
// 		
// 		//设置材质
// 		m_CurMtrl = *(ppMtrl + i);	
// 		//不透明材质，或AlphaTest材质
// 		if (!m_CurMtrl->bAlphaBlend && m_CurMtrl->getOpacity() == 100)
// 			continue;		
// 		
// 		//如果材质的所有模型都被剪裁
// 		if (m_CurMtrl->bCulled)
// 			continue;
// 		//判断贴图是否已经下载，如果没有下载完，则不显示
// 		if (!m_CurMtrl->IsReadyToRender())
// 			continue;
// 
// 		// 设置材质，渲染凹凸纹理
// 		m_CurMtrl->SetToDevice(m_pd3dDevice, 0);
// 		
// 		//渲染使用当前材质的模型
// 		RenderModelsByMtrl(/*m_CurMtrl*/);
// 	}

	
	INT transModelCnt = m_vTransModel.size();
	if (transModelCnt == 0) 
	{
		return;
	}

	//雾渲染
	m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, m_bFog);
	
	//对透明模型进行排序 //global var,用于对透明物体排序的计算参数
	if (m_pCurCamera && transModelCnt > 1)
	{
		D3DXMatrixInverse( &m_MatViewInverse, NULL, &m_MatView );
		CamDir = D3DXVECTOR3(m_MatViewInverse._31, m_MatViewInverse._32, m_MatViewInverse._33);
		CamPos = m_pCurCamera->GetPosition(VTS_WORLD);
		stable_sort(m_vTransModel.begin(), m_vTransModel.end(), SortFuncEx);
	}
	
	//渲染半透明物体的渲染状态
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    //	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	//逐个模型渲染
	std::vector<CModel*>::iterator it = m_vTransModel.begin();
	while (it != m_vTransModel.end())
	{
		RenderOneModel(*it); 
		++it;
	}
	
	//初始化变量
	m_CurMtrl = NULL;
	m_CurModel = NULL;
}


//-----------------------------------------------------------------------------
// 方法名: RenderModelsByMtrl()
// 参  数: CMaterial* pCurMtrl 
// 描  述: 渲染使用当前材质的模型
//-----------------------------------------------------------------------------
void CScene::RenderModelsByMtrl(/*CMaterial* pMtrl*/)
{
	//使用该材质的模型数量
	INT modelCnt = m_CurMtrl->subMeshList.size();
	//如果该材质没有模型，譬如Overlay和NavigaterMap
	if (modelCnt == 0 ) return;
	
	// 设置材质，渲染凹凸纹理
	m_CurMtrl->SetToDevice(m_pd3dDevice, 1);

	//使用该材质的Mesh
	std::vector<VGS_RenderSubmesh>::iterator pSubMesh = m_CurMtrl->subMeshList.begin();
	int subMeshCnt = m_CurMtrl->subMeshList.size();
	for (INT j = 0; j < subMeshCnt; j++)
	{
		VGS_RenderSubmesh subMesh = *(pSubMesh + j);
		m_CurModel = (CModel*)subMesh.modelPointer;	

		// 关键帧动画
		// PlayModelKeyFrame();
		
		//设置模型的MoveModelTo, 必须放在这里，否则当物体离视点较远时，不处理了
		if (m_CurModel->user_moveFrameCnt > 0)
			PlayUserMoveModelKFR();
		
		//模型是否是billboard模型
		if (m_CurModel->isBillboard)
			SetModelBillboard(m_CurModel);
        
		//如果模型不可见，不渲染
		if (!m_CurModel->visible) continue;
		//如果模型没有被剪切, 渲染模型的SubMesh
		if (m_CurModel->culled) continue;

		//判断是否是双面材质这个判断应该写在外面，因为涉及到反射渲染的问题
// 		if (pMtrl->twoSide )
// 			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
// 		else
// 			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			//设置是否重新计算模型法线，保证缩放模型能正常显示高光
			m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, m_CurModel->isAutoNormalise);
			
			//设置该模型当前帧的矩阵	
			m_pd3dDevice->SetTransform(D3DTS_WORLD, &(m_CurModel->m_pNode->getMatrix(VTS_WORLD)));
			m_CurModel->RenderSubMesh(subMesh.subMeshID);
		}


		//如果需要辅助表面，渲染到辅助表面------------------------------------------------------------
// 		INT addtionalRenderSurfCnt = m_CurModel->m_additionalRenderInfo.size();
// 		for (INT m = 0; m < addtionalRenderSurfCnt; m++)
// 		{
// 			ADDITIONALRENDERINFO* pRenderInfo = m_CurModel->m_additionalRenderInfo.begin() + m;
// 			switch (pRenderInfo->type)
// 			{
// 				//实时反射辅助表面
// 			case ADS_RTFMAP:
// 				{
// 					CRTFMap* pRTFMap = (CRTFMap*)(pRenderInfo->pRenderToTexture);
// 					RenderSubMeshToRTFTexture(pRTFMap, &subMesh/*, pMtrl*/);
// 				}
// 				break;
// 				
// 				//实时折射辅助表面
// 			case ADS_RTTMAP:
// 				break;
// 
// 				//凹凸贴图的合成表面
// 			case ADS_BUMPWAVEMAP:
// 				{
// 					CBumpMap* pBumpMap = (CBumpMap*)(pRenderInfo->pRenderToTexture);
// 					RenderSubMeshToBumpWaveCompositeTexture(pBumpMap, &subMesh, m_CurMtrl);
// 				}
// 				break;
// 			
// 			case ADS_CUBEMAP:
// 				{
// 					CCubeMap *pCubemap = (CCubeMap*)(pRenderInfo->pRenderToTexture);
// 					RenderModelToDynamicCubeMapSurface(m_CurModel, &subMesh, pCubemap);
// 				}
// 				break;
// 			}
// 		}
		//----------------------------------------------------------------------------------------------
	
}

//-----------------------------------------------------------------------------
// 方法名: RenderText()
// 参  数: 
// 描  述: 渲染文字的方法
//-----------------------------------------------------------------------------
VOID CScene::RenderText()
{
	std::vector<CText*>::iterator ppText = m_vText.begin();
	UINT textCnt = m_vText.size(); 
	for (UINT i = 0; i < (UINT)textCnt; i++)
	{	
		CText* pText = *(ppText + i);
		if (!pText->visible)
			continue;
		
		//画Text
		if (pText->m_pFont)
			pText->m_pFont->DrawText(pText->strText, &pText->rect, pText->uFormat, pText->color);
	}
}
//-----------------------------------------------------------------------------
// 方法名: RenderOverlay()
// 参  数: COverlay* pOverlay 
// 描  述: 渲染Overlay的方法
//-----------------------------------------------------------------------------
VOID CScene::RenderOverlay(COverlay* pOverlay)
{
	//创图Logos
	if (pOverlay && pOverlay->GetVisibility())
	{
		//清空贴图内存
		ClearTextureMem();
		
		m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
		
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		
		m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		
		//通过材质渲染Overlay
		m_CurMtrl = pOverlay->GetMaterial();		
		//设置漫色贴图
		SetOverlayTexture();
		
		pOverlay->Render();
	}
    
}
//-----------------------------------------------------------------------------
// 方法名: RenderOverlay()
// 参  数: 
// 描  述: 渲染Overlay的方法
//-----------------------------------------------------------------------------
VOID CScene::RenderOverlay()
{
	//for test 2010-6-1
// 	if (GetRoot()->getGfxMgrptr())
// 	{
// 		static DWORD MovieLastTime = timeGetTime();
// 		DWORD mtime = timeGetTime();
// 		float deltaTime = ((float)(mtime - MovieLastTime)) / 1000.0f;
// 		MovieLastTime = mtime;
// 
// 		GetRoot()->getGfxMgrptr()->render(deltaTime);
// 	}
	if (m_GUIManager.HasObjects())//added by linxq 09-4-9
	{
		//清空贴图内存
		ClearTextureMem();
		//Turn off the zbuffer	
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		//打开Alpha混合, 关闭Alpha测试
		m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_GUIManager.UpdateLayout();
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

// 	INT overlayCnt = m_vOverlay.size(); 
// 	if (overlayCnt == 0)
// 		return;
// 	
// 	//清空贴图内存
// 	ClearTextureMem();
// 	
// 	//Turn off the zbuffer	
// 	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
// 	//打开Alpha混合, 关闭Alpha测试
// 	m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
// 	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
// 	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
// 	
// 	COverlay** ppOverlay = m_vOverlay.begin();
// 	for (INT i = 0; i< overlayCnt; i++)
// 	{	
// 		COverlay* pOverlay = *(ppOverlay + i);
// 		
// 		if (!pOverlay->GetVisibility())
// 			continue;
// 		
// 		if (pOverlay->GetAsBackdrop())
// 			continue;
// 		
// 		if (pOverlay->GetFilterEnable())
// 			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
// 		else
// 			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
// 		
// 		// 检查是否已经可以渲染
// 		if (!m_CurMtrl->IsReadyToRender())
// 		{
// 			m_CurMtrl = NULL;
// 			continue;
// 		}
// 		//设置漫色贴图
// 		SetOverlayTexture();		
// 		//----------------------------------------------------------------		
// 		//画
// 		pOverlay->Render();
// 	}
// 	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//渲染backdrop
// VOID CScene::RenderBackdrop()
// {	
// 	INT overlayCnt = m_vOverlay.size(); 
// 	if (overlayCnt == 0)
// 		return;
// 	
// 	//清空贴图内存
// 	ClearTextureMem();
// 	
// 	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
// 
// 	COverlay** ppOverlay = m_vOverlay.begin();
// 	for (INT i = 0; i< overlayCnt; i++)
// 	{	
// 		COverlay* pOverlay = *(ppOverlay + i);
// 		
// 		if (!pOverlay->GetVisibility())
// 			continue;
// 		
// 		if (!pOverlay->GetAsBackdrop())
// 			continue;
// 		
// 		// fuxb 2007-10-18 ==>
// 		//通过材质渲染Overlay
// 		m_CurMtrl = pOverlay->GetMaterial();
// 
// 		//判断贴图是否已经下载，如果没有下载完，则不显示
// 		if (!m_CurMtrl->IsReadyToRender())
// 		{
// 			m_CurMtrl = NULL;
// 			continue;
// 		}
// 		
// 		//设置漫色贴图
// 		SetOverlayTexture();
// 		// <==
// 		
// 		//画Backdrop
// 		pOverlay->Render();
// 	}
// }
//-----------------------------------------------------------------------------
// 方法名: RenderAttach()
// 参  数: 
// 描  述: 渲染轴对象
//-----------------------------------------------------------------------------
VOID CScene::RenderAttach()
{	
	ClearTextureMem();
	
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//关闭Alpha混合
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	
	INT mtrlCnt = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = m_vShader.begin();
	for (INT i=0; i< mtrlCnt; i++)
	{
		//设置材质
		m_CurMtrl = *(ppMtrl + i);
		INT modelCnt = m_CurMtrl->subMeshList.size();

		//使用该材质的SubMesh
		std::vector<VGS_RenderSubmesh>::iterator pSubMesh = m_CurMtrl->subMeshList.begin();
		
		//CModel** ppModel = (CModel**)(pMtrl->modelList.begin());
		for (INT j = 0; j< modelCnt; j++)
		{
			VGS_RenderSubmesh subMesh = *(pSubMesh + j);
			
			m_CurModel =(CModel*)subMesh.modelPointer;	
			
			m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			//画BoundBox
			if (m_CurModel->pRBBox != NULL && m_CurModel->showRBBox)
			{
				m_CurModel->pRBBox->Draw();
			}
		}
	}
	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
//-----------------------------------------------------------------------------
// 方法名: GetCubeMapViewMatrix()
// 参  数: DWORD dwFace 
// 用  途: 
//-----------------------------------------------------------------------------
D3DXMATRIX CScene::GetCubeMapViewMatrix( DWORD dwFace )
{
    D3DXVECTOR3 vEyePt   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vLookDir;
    D3DXVECTOR3 vUpDir;
	
    switch( dwFace )
    {
	case D3DCUBEMAP_FACE_POSITIVE_X:
		vLookDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_X:
		vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Y:
		vLookDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Y:
		vLookDir = D3DXVECTOR3( 0.0f,-1.0f, 0.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	case D3DCUBEMAP_FACE_POSITIVE_Z:
		vLookDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
	case D3DCUBEMAP_FACE_NEGATIVE_Z:
		vLookDir = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
		vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		break;
    }
	
    // Set the view transform for this cubemap surface
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookDir, &vUpDir );
    return matView;
}

//-----------------------------------------------------------------------------
// 方法名: LightFrame()
// 参  数: 
// 描  述: 动态灯光
//-----------------------------------------------------------------------------
VOID CScene::LightFrame()
{
	UINT lightCnt = m_vLight.size();
	std::vector<CLight*>::iterator ppLight = m_vLight.begin();
	for (UINT i = 0; i < lightCnt; i++)
	{
		CLight* pLight = *(ppLight + i);
		m_pd3dDevice->SetLight(pLight->index, &pLight->param);
		m_pd3dDevice->LightEnable(pLight->index, pLight->enable);
	}
// 	UINT lightCnt = m_vLight.size();
// 	CLight** ppLight = m_vLight.begin();
// 	for (UINT i = 0; i < lightCnt; i++)
// 	{
// 		CLight* pLight = *(ppLight + i);
// 
// 		//如果enable m_vLight
// 		if (pLight->enable == FALSE) 	
// 			m_pd3dDevice->LightEnable(pLight->index, false);
// 		else
// 			m_pd3dDevice->LightEnable(pLight->index, true);
// 
// 		//灯光的位置
// 		pLight->param.Position = pLight->pNode->worldPosition;
// 
// 		//没有关键帧或不播放关键帧
// 		if ((m_KfrRepeatCnt == -1 || m_KfrRepeatIndex < m_KfrRepeatCnt) && pLight->playKfr && pLight->kfrCnt > 1)
// 		{
// 			//播放灯光动画
// 			INT currentFrame = m_CurrentFrame;
// 			if (currentFrame >= pLight->kfrCnt)
// 			{
// 				// 在关键帧主循环中继续循环播放模型动画
// 				if (pLight->kfrRepeatCnt == -1 || pLight->kfrRepeatIndex < pLight->kfrRepeatCnt)
// 				{
// 					currentFrame = m_CurrentFrame % pLight->kfrCnt;
// 					pLight->kfrRepeatIndex++;
// 				}			
// 				else //只播放一次
// 				{
// 					currentFrame = pLight->kfrCnt - 1;
// 				}
// 			}
// 			
// 			D3DXVECTOR3 vvpos;
// 			// 更新灯光位置
// 			if (pLight->pNode)
// 			{
// 				pLight->pNode->curTransform.matrix = pLight->pNode->kfrtransform[currentFrame];
// 				pLight->pNode->updateTransform();
// 				pLight->param.Position = pLight->pNode->worldPosition;
// 			}
// 
// 			//diffuse
// 			D3DCOLORVALUE diffuse;
// 			//点光源
// 			switch (pLight->param.Type)
// 			{
// 				case D3DLIGHT_POINT:
// 					if (pLight->pAnimOmniLight)
// 					{					
// 						//diffuse
// 						DWORD re = (pLight->pAnimOmniLight[currentFrame]).color;
// 						diffuse.r = (pLight->pAnimOmniLight[currentFrame].color >> 24 & 0xff) / 255.0;
// 						diffuse.g = (pLight->pAnimOmniLight[currentFrame].color >> 16 & 0xff) / 255.0;
// 						diffuse.b = (pLight->pAnimOmniLight[currentFrame].color >> 8 & 0xff) / 255.0;
// 						diffuse.a = (pLight->pAnimOmniLight[currentFrame].color & 0xff) / 255.0;
// 						pLight->param.Diffuse = pLight->pAnimOmniLight[currentFrame].multiply * diffuse;
// 
// 						pLight->param.Attenuation0 = pLight->pAnimOmniLight[currentFrame].attenuation0;
// 						pLight->param.Attenuation1 = pLight->pAnimOmniLight[currentFrame].attenuation1;
// 						pLight->param.Attenuation2 = pLight->pAnimOmniLight[currentFrame].attenuation2;
// 						pLight->param.Range = pLight->pAnimOmniLight[currentFrame].attenuationRange;
// 					}
// 					break;
// 				case D3DLIGHT_SPOT:
// 					if (pLight->pAnimSpotLight)
// 					{
// 						//diffuse
// 						diffuse.r = (pLight->pAnimSpotLight[currentFrame].color >> 24 & 0xff) / 255.0;
// 						diffuse.g = (pLight->pAnimSpotLight[currentFrame].color >> 16 & 0xff) / 255.0;
// 						diffuse.b = (pLight->pAnimSpotLight[currentFrame].color >> 8 & 0xff) / 255.0;
// 						diffuse.a = (pLight->pAnimSpotLight[currentFrame].color & 0xff) / 255.0;
// 						pLight->param.Diffuse = pLight->pAnimSpotLight[currentFrame].multiply * diffuse;
// 
// 						//direction
// 						pLight->param.Direction.x = pLight->pAnimSpotLight[currentFrame].direction.x;
// 						pLight->param.Direction.y = pLight->pAnimSpotLight[currentFrame].direction.y;
// 						pLight->param.Direction.z = pLight->pAnimSpotLight[currentFrame].direction.z;
// 						
// 
// 						//theta
// 						pLight->param.Theta = pLight->pAnimSpotLight[currentFrame].innerAngle;
// 						//phi
// 						pLight->param.Phi = pLight->pAnimSpotLight[currentFrame].outerAngle;
// 						pLight->param.Falloff = pLight->pAnimSpotLight[currentFrame].fallOff;
// 						pLight->param.Attenuation0 = pLight->pAnimSpotLight[currentFrame].attenuation0;
// 						pLight->param.Attenuation1 = pLight->pAnimSpotLight[currentFrame].attenuation1;
// 						pLight->param.Attenuation2 = pLight->pAnimSpotLight[currentFrame].attenuation2;
// 						pLight->param.Range = pLight->pAnimSpotLight[currentFrame].attenuationRange;
// 					}
// 					break;
// 				case D3DLIGHT_DIRECTIONAL:
// 					if (pLight->pAnimDirectionalLight)
// 					{
// 						//diffuse
// 						diffuse.r = (pLight->pAnimDirectionalLight[currentFrame].color >> 24 & 0xff) / 255.0;
// 						diffuse.g = (pLight->pAnimDirectionalLight[currentFrame].color >> 16 & 0xff) / 255.0;
// 						diffuse.b = (pLight->pAnimDirectionalLight[currentFrame].color >> 8 & 0xff) / 255.0;
// 						diffuse.a = (pLight->pAnimDirectionalLight[currentFrame].color & 0xff) / 255.0;
// 						pLight->param.Diffuse = pLight->pAnimDirectionalLight[currentFrame].multiply * diffuse;
// 						//direction
// 						pLight->param.Direction.x = pLight->pAnimDirectionalLight[currentFrame].direction.x;
// 						pLight->param.Direction.y = pLight->pAnimDirectionalLight[currentFrame].direction.y;
// 						pLight->param.Direction.z = pLight->pAnimDirectionalLight[currentFrame].direction.z;
// 
// 						pLight->param.Attenuation0 = pLight->pAnimDirectionalLight[currentFrame].attenuation0;
// 						pLight->param.Attenuation1 = pLight->pAnimDirectionalLight[currentFrame].attenuation1;
// 						pLight->param.Attenuation2 = pLight->pAnimDirectionalLight[currentFrame].attenuation2;
// 						pLight->param.Range = pLight->pAnimDirectionalLight[currentFrame].attenuationRange;
// 					}
// 					break;
// 				default:
// 					break;
// 			} // switch
// 		}// if
// 		
// 		m_pd3dDevice->SetLight(pLight->index, &(pLight->param));  // 使属性更改生效
// 
// 	} // for	
}

//-----------------------------------------------------------------------------
// 方法名: UpdateNavigaterPointer()
// 参  数: 
// 描  述: 更新导航器Pointer
//-----------------------------------------------------------------------------
VOID CScene::UpdateNavigaterPointer()
{
	UINT nvCnt = m_vNavigater.size();
	std::vector<CNavigator*>::iterator ppNavigater = m_vNavigater.begin();
	for (UINT i = 0; i< nvCnt;  i++)
	{
		CNavigator* pNavigator = *(ppNavigater + i);
		pNavigator->UpdatePointers();
	}
}

//-----------------------------------------------------------------------------
// 方法名: UpdateNavigaterMap()
// 参  数: 
// 描  述: 更新导航器Map
//-----------------------------------------------------------------------------
VOID CScene::UpdateNavigaterMap()
{
	UINT nvCnt = m_vNavigater.size();
	std::vector<CNavigator*>::iterator ppNavigater = m_vNavigater.begin();
	for (UINT i = 0; i< nvCnt;  i++)
	{
		CNavigator* pNavigater = *(ppNavigater + i);
		RECT vpRect;
		m_CurViewport->GetRect(vpRect);
		pNavigater->UpdateMap(vpRect);
	}
}



//-----------------------------------------------------------------------------
// 方法名: Render()
// 参  数: 
// 描  述: 渲染方法 
//-----------------------------------------------------------------------------
VOID CScene::Render(CCamera *pCamera, CViewport *pVP)
{
	m_pCurCamera = pCamera;  // 设置当前使用的相机
	m_CurViewport = pVP;     // 设置当前渲染的视口

	//用于动态模糊
	if (!m_bRenderCurrentFrame)
	{
		return;
	}
	//for test 2010-6-2
// 	if (GetRoot()->getGfxMgrptr())
// 	{
// 		static DWORD MovieLastTime = timeGetTime();
// 		DWORD mtime = timeGetTime();
// 		float deltaTime = ((float)(mtime - MovieLastTime)) / 1000.0f;
// 		MovieLastTime = mtime;
// 
// 		GetRoot()->getGfxMgrptr()->	renderTexture(deltaTime);
// 	}

	//渲染前的设置
	StartRender();

	//RenderadditionalTexture(); // 先渲染特殊材质

	//渲染backdrop
	RenderBackDrop();

	//渲染模型
	RenderModels();


	//渲染其它的对象
	RenderOthers();

	//结束本帧的渲染
	EndRender();	



// 	//检查音频的播放状态
// 	CheckAudioStatus();

	m_TotalFrameCnt++;
}

//-----------------------------------------------------------------------------
// 方法名: StartRender()
// 参  数: 
// 描  述: 渲染一帧图像的准备设置，包括矩阵变换和和其它的控制
//-----------------------------------------------------------------------------
VOID CScene::StartRender()
{	
	//场景中的灯光动画
	LightFrame();

	//视角变换
	ViewTransform(); 
	//剪裁测试
	FrustrumTest();
	//投影变换
	ProjTransform();
	//模型拖拽
	// setModelCtrl();
	
	RenderadditionalTexture(); // 先渲染特殊材质
	
	m_pCurSwapChain = NULL;
	if (SUCCEEDED( m_pd3dDevice->BeginScene() ) )			
	{
		//设置渲染到SwapChain表面
		LPDIRECT3DSURFACE9 pBackBuffer = NULL;
		m_pCurSwapChain = (CSwapChain *)m_CurViewport->GetRenderWindow();
		if (!m_pCurSwapChain)
		{
			return;
		}
		m_pCurSwapChain->m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		m_pd3dDevice->SetRenderTarget(0, pBackBuffer);  // 设置渲染目标
		D3DVIEWPORT9 *pVP = m_CurViewport->GetD3DViewport();         // 设置视口
		m_pd3dDevice->SetViewport(pVP);		
		
		//清空后缓冲
		D3DRECT rect;
		rect.x1 = pVP->X;
		rect.x2 = pVP->X + pVP->Width;
		rect.y1 = pVP->Y;
		rect.y2 = pVP->Y + pVP->Height;
		m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_backgroundColor, 1.0f, 0L);

#ifdef _DEBUG
		{
			//LPDIRECT3DSURFACE9 pBackBuffer = NULL;
			//m_pCurSwapChain->m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
			// D3DXSaveSurfaceToFile("c:\\backbuffer1.jpg", D3DXIFF_JPG, pBackBuffer, NULL, NULL);
			//SAFE_RELEASE_D3D(pBackBuffer);
		}
#endif
		
		SAFE_RELEASE_D3D(pBackBuffer);
	}
	
	// 清空场景中所有的辅助渲染表面
	// ClearAllAdditionalSurface();
}

// 先渲染特殊材质
VOID CScene::RenderadditionalTexture()
{
	bool bFlag = (!m_vBumpTexture.empty() || !m_vRtfTexture.empty());
	if (!bFlag)
		return;

	LPDIRECT3DSURFACE9 pRenderTarget, pZbuffer;
	//  保存渲染表面 
	m_pd3dDevice->GetRenderTarget(0, &pRenderTarget);
	m_pd3dDevice->GetDepthStencilSurface(&pZbuffer);
	D3DXMATRIX matViewSaved;
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matViewSaved );

	if (SUCCEEDED( m_pd3dDevice->BeginScene() ) )			
	{
		// BUMP Map
		INT BumpMapCnt = m_vBumpTexture.size();
		std::vector<CBumpMap*>::iterator ppBumpMap = m_vBumpTexture.begin();
		for (int i = 0; i< BumpMapCnt; i++)
		{
			CBumpMap* pBumpMap = *(ppBumpMap + i);
			// pBumpMap->UpdateTexBuf();
			if (pBumpMap && pBumpMap->m_pMaterial)
			{
				if (pBumpMap->m_pMaterial->subMeshList.size() > 0)
				{
					pBumpMap->m_pMaterial->SetToDevice(m_pd3dDevice, 0);  // 设置bump贴图
					RenderSubmeshToBumptexture(pBumpMap, pBumpMap->m_pMaterial->subMeshList);  // 将submesh渲染到bump texture
				}
			}
		}
		
		//实时反射---linxq-2009-6-23
		if (!m_vRtfTexture.empty())
		{
			m_pd3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
			std::vector<CRTFMap*>::iterator it = m_vRtfTexture.begin(), end = m_vRtfTexture.end();
			CRTFMap* ptrRtf = NULL;
			CModel* pModel = NULL;
			for (; it != end; ++it)
			{
				ptrRtf = *it;
				if (ptrRtf && ptrRtf->m_pMaterial && ptrRtf->GetReflectedModelCount() > 0)
				{
					RenderMeshToRTFTexture(ptrRtf);
				}
			}
			m_pd3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
		}

		//恢复---进行统一恢复设置，以提高效率			
		//  恢复渲染表面为显示器 
		m_pd3dDevice->SetDepthStencilSurface(pZbuffer);
		m_pd3dDevice->SetRenderTarget( 0  , pRenderTarget);
		m_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 0);
 		m_pd3dDevice->SetTransform( D3DTS_VIEW, &matViewSaved);

		m_pd3dDevice->EndScene();
	}
}

// 将submesh渲染到bump texture
VOID CScene::RenderSubmeshToBumptexture(CBumpMap* pBumpmap, const vector<VGS_RenderSubmesh> &vSubmeshList)
{
	if (!pBumpmap || vSubmeshList.size() == 0)
	{
		return;
	}

	std::vector<VGS_RenderSubmesh>::const_iterator pSubMesh = vSubmeshList.begin();
	int subMeshCnt = vSubmeshList.size();
	for (INT j = 0; j < subMeshCnt; ++j)
	{
		VGS_RenderSubmesh subMesh = *(pSubMesh + j);
		CModel* pModel = (CModel*)subMesh.modelPointer;	
		CMaterial *pMaterial = pBumpmap->m_pMaterial;
        
		//设置模型color
		if (pModel->bModelColor)
		{			
			D3DMATERIAL9 material;
			material.Diffuse.a = ((pModel->modelColor>>24)&0xff)/255.0f;
			material.Diffuse.r = ((pModel->modelColor>>16)&0xff)/255.0f;
			material.Diffuse.g = ((pModel->modelColor>>8)&0xff)/255.0f;
			material.Diffuse.b = ((pModel->modelColor)&0xff)/255.0f;
			material.Emissive.r = pMaterial->selfIllumination / 100.0f * m_CurMtrl->d3dmtrl.Diffuse.r;
			material.Emissive.g = pMaterial->selfIllumination / 100.0f * m_CurMtrl->d3dmtrl.Diffuse.g;
			material.Emissive.b = pMaterial->selfIllumination / 100.0f * m_CurMtrl->d3dmtrl.Diffuse.b;
			m_pd3dDevice->SetMaterial(&material);
		}
        
		//设置模型的MoveModelTo, 必须放在这里，否则当物体离视点较远时，不处理了
// 		if (pModel->user_moveFrameCnt > 0)
// 			PlayUserMoveModelKFR();
		
		//模型是否是billboard模型
		if (pModel->isBillboard)
		{
			SetModelBillboard(pModel);
        }

		//如果模型不可见，不渲染
		if (!pModel->visible) 
		{
			continue;
		}
		//如果模型没有被剪切, 渲染模型的SubMesh
		if (pModel->culled) 
		{
			continue;
		}

		//设置是否重新计算模型法线，保证缩放模型能正常显示高光
		m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, pModel->isAutoNormalise);

		//设置该模型当前帧的矩阵
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &(pModel->m_pNode->getMatrix(VTS_WORLD)));
		
		// 渲染到bump		
		RenderSubMeshToBumpWaveCompositeTexture(pBumpmap, &subMesh, pMaterial);
		
	}

	pBumpmap->UpdateTexBuf(); // 更新到渲染表面

}

//渲染backdrop
VOID CScene::RenderBackDrop()
{
	if (m_GUIManager.HasBackDrop())//added by linxq 09-4-20
	{
		//清空贴图内存
		ClearTextureMem();
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		 	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		 	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		 	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		 	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		 	
		 	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		 	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		 	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		 	
		 	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		 	m_pd3dDevice->SetSamplerState( 0,  D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		 	
		 	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		 	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		 	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
		 	
		 	//Turn off the zbuffer	
		 	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		 	//打开Alpha混合, 关闭Alpha测试
		 	m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		 	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		 	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		 	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		 	

		m_GUIManager.RenderBackDrop();
	}
}
//结束本帧的渲染
VOID CScene::EndRender()
{
	//更新所有RenderToTexture的表面
	// UpdateAllAdditionalSurface();

	m_pd3dDevice->EndScene();

	// 更新到前表面
	if (m_pCurSwapChain)
	{
#ifdef _DEBUG
		/*LPDIRECT3DSURFACE9 pBackBuffer = NULL;
		m_pCurSwapChain->m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		D3DXSaveSurfaceToFile("c:\\backbuffer.jpg", D3DXIFF_JPG, pBackBuffer, NULL, NULL);
		SAFE_RELEASE_D3D(pBackBuffer);*/
#endif
		m_pCurSwapChain->m_pSwapChain->Present(NULL, NULL, NULL, NULL, 0);

	}

	// 渲染完毕，关闭所有灯光。以免渲染其他场景时错误
	UINT lightCnt = m_vLight.size();
	std::vector<CLight*>::iterator ppLight = m_vLight.begin();
	for (UINT i = 0; i < lightCnt; i++)
	{
		CLight* pLight = *(ppLight + i);
		m_pd3dDevice->LightEnable(pLight->index, FALSE);
	}	
}
//-----------------------------------------------------------------------------
// 方法名: ClearAllAdditionalSurface()
// 参  数: 
// 描  述: 清空场景中所有的辅助渲染表面
//-----------------------------------------------------------------------------
VOID CScene::ClearAllAdditionalSurface()
{
	//RTF Map 
	INT RTFTexCnt = m_vRtfTexture.size();
	std::vector<CRTFMap*>::iterator ppRTFMap = m_vRtfTexture.begin();
	for (INT i = 0; i< RTFTexCnt; i++)
	{
		CRTFMap* pRTFMap = *(ppRTFMap + i);
		pRTFMap->Clear();
	}
	
	//BUMP Map
	INT BumpMapCnt = m_vBumpTexture.size();
	std::vector<CBumpMap*>::iterator ppBumpMap = m_vBumpTexture.begin();
	for (i = 0; i< BumpMapCnt; i++)
	{
		CBumpMap* pBumpMap = *(ppBumpMap + i);
		pBumpMap->Clear();
	}
}
//更新所有辅助渲染表面
VOID CScene::UpdateAllAdditionalSurface()
{
	//RTF Map 
	INT RTFTexCnt = m_vRtfTexture.size();
	std::vector<CRTFMap*>::iterator ppRTFMap = m_vRtfTexture.begin();
	for (INT i = 0; i< RTFTexCnt; i++)
	{
		CRTFMap* pRTFMap = *(ppRTFMap + i);
		pRTFMap->UpdateTexBuf();
	}
	
	//BUMP Map
	INT BumpMapCnt = m_vBumpTexture.size();
	std::vector<CBumpMap*>::iterator ppBumpMap = m_vBumpTexture.begin();
	for (i = 0; i< BumpMapCnt; i++)
	{
		CBumpMap* pBumpMap = *(ppBumpMap + i);
		pBumpMap->UpdateTexBuf();
	}

	INT CubeMapCnt = m_vpDynamicCubemap.size();
}


//-----------------------------------------------------------------------------
// 方法名: RenderOthers()
// 参  数: 
// 描  述: 渲染场景中的其它对象，譬如Overlay等
//-----------------------------------------------------------------------------
VOID CScene::RenderOthers()
{
	//开始渲染
	// m_pd3dDevice->BeginScene();
	//渲染镜头光斑
	RenderLensFlare();
	//渲染界面UI
	RenderOverlay();
	//更新Navigater
	UpdateNavigaterPointer();	
	//渲染轴对象
	RenderAttach();
	//渲染文字
	RenderText();	
	//渲染LOGO
	RenderLogo();
	//结束渲染
	// m_pd3dDevice->EndScene();

}

//-----------------------------------------------------------------------------
// 方法名: RenderSwapChain()
// 参  数: CSwapChain* pSwapChain
// 描  述: 渲染SwapChain
//-----------------------------------------------------------------------------
//##ModelId=47EBC73B006D
// VOID CScene::RenderGrapha()
// {
// 	//视角变换
// 	ViewTransform();
// 
// 	//投影变换
// 	ProjTransform();
// 
// 	CSwapChain *pCurSwapChain = NULL;
// 	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )			
// 	{
// 		//设置渲染到SwapChain表面
//  		LPDIRECT3DSURFACE9 pBackBuffer = NULL;
//  		LPDIRECT3DSURFACE9 pStencilBuffer = NULL; 
// 		pCurSwapChain = (CSwapChain *)m_CurViewport->GetRenderWindow();
// 		if (!pCurSwapChain)
// 		{
// 			return;
// 		}
// 		pCurSwapChain->m_pSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer); 
// 		//m_pd3dDevice->GetDepthStencilSurface(&pStencilBuffer); 
// 		m_pd3dDevice->SetRenderTarget(0, pBackBuffer);  // 设置渲染目标
// 		D3DVIEWPORT9 *pVP = m_CurViewport->GetD3DViewport();         // 设置视口
// 		m_pd3dDevice->SetViewport(pVP);
// 
// 
// 		//清空后缓冲
// 		D3DRECT rect;
// 		rect.x1 = pVP->X;
// 		rect.x2 = pVP->X + pVP->Width;
// 		rect.y1 = pVP->Y;
// 		rect.y2 = pVP->Y + pVP->Height;
// 		m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_background, 1.0f, 0L);
// 
//  		SAFE_RELEASE_D3D(pBackBuffer);
//  		SAFE_RELEASE_D3D(pStencilBuffer);
// 
// 		/*
// 		if (m_bRenderGUI)
// 		{
// 			//渲染Backdrop
// 			RenderBackdrop();
// 		}*/
// 
// 		
// 		// if (m_bRenderScene)
// 		{
// 			//渲染不透明模型
// 			RenderOpacModels(); 
// // 			INT oCnt = m_vOverlay.size(); 
// // 	         if (oCnt !=0)
// // 			 {
// // 			      oCnt=1;
// // 			 }
// 			///渲染透明模型
// 			RenderTransModels();
// 			//渲染镜头光斑
// 			//RenderLensFlare();
// 		}
// /*
// 		if (m_bRenderGUI)
// 		{
// 			//渲染文字
// 		//	RenderText();
// 			//渲染前景图
// 		//	RenderOverlay();
// 			//渲染按钮
// 			//RenderButton();
// 			//更新Navigater
// 			//UpdateNavigaterPointer();
// 		}
// */
// 		//if (m_bRenderScene)
// 		{
// 			//渲染附件
// 			RenderAttach();
// 
// 			//渲染视图左上角的文字
// 			// RenderDefaultViewText();
// 		}
// 
// 		// RenderSelectAxis();
// 
// 		m_pd3dDevice->EndScene();
// 	}
// 
// 	//present
// 	// m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
// 	HRESULT hr = pCurSwapChain->m_pSwapChain->Present(NULL, NULL, NULL, NULL, 0);
// 	/*if (D3DERR_INVALIDCALL == hr)
// 	{
// 		int kkk = 0;
// 	}
// 	else if (D3DERR_DEVICELOST == hr)
// 	{
// 		int kkk = 0;
// 	}*/		
// }


//检查音频的播放状态
//##ModelId=47EBC7360167
VOID CScene::CheckAudioStatus()
{/*
	//音频事件
	INT audioCnt = m_vAudio.size();
	CAudioPlayer** ppAudio = m_vAudio.begin();
	for (INT i = 0; i< audioCnt; i++)
	{
		CAudioPlayer* pAudio = *(ppAudio + i);
		if (pAudio->m_bCreated)
			pAudio->CheckAudioStatus();
	}*/
}


//-----------------------------------------------------------------------------
// 方法名: SetBackground()
// 参  数: 
// 描  述: 设置背景色
//-----------------------------------------------------------------------------
VOID CScene::SetBackground(BYTE red, BYTE green, BYTE blue)
{
	m_backgroundColor = D3DCOLOR_RGBA(red, green, blue, 0);
}

//-----------------------------------------------------------------------------
// 方法名: UpdateBackBuf()
// 参  数: RECT* pRect
// 描  述: 更新后缓冲
//-----------------------------------------------------------------------------
// BOOL CScene::UpdateBackBuf(RECT* pRect)
// {
// 	if(!bActive)
// 		return FALSE;
// 	
// 	if (pRect->left || pRect->top || pRect->right || pRect->bottom)
// 	{
// 		m_SceneRect = *pRect;
// 		UpdateRatio();
// 		if (FAILED(InvalidateScene()))
// 			return FALSE;
// 		
// 		if (FAILED(RestoreScene()))
// 			return FALSE;
// 		
// 		return TRUE;
// 	}
// 	return FALSE;
// }

//-----------------------------------------------------------------------------
// 方法名: CreateEmptyTex()
// 参  数: CTexture* pTex
// 描  述: 创建引擎默认的贴图
//-----------------------------------------------------------------------------
// HRESULT CScene::CreateEmptyTex(CTexture* pTex, INT mipLevel)
// {
// 	return (D3DXCreateTexture(m_pd3dDevice,
// 								pTex->width, 
// 								pTex->height, 
// 								mipLevel, 
// 								0, 
// 								pTex->BP,
// 								D3DPOOL_MANAGED,
// 								&pTex->textureBuf));
// }

//-----------------------------------------------------------------------------
// 方法名: CreateTexFromBuffer()
// 参  数: CTexture* pTex
// 描  述: 从缓冲创建贴图
//-----------------------------------------------------------------------------
// HRESULT CScene::CreateTexFromBuffer(CTexture* pTex, INT mipLevel)
// {
// 	//如果当前pTex的数据为空，则无法创建贴图。这将导致贴图列表缺失，造成材质索引贴图错误，程序将非法操作
// 	//为了解决这个问题: 如果当pTex的数据为空时，则创建一个默认的贴图，放到索引表中，这样贴图就不会缺失了。
// 	CImage* pImage = m_vImage[pTex->imageIdx];
// 	if (pImage->data == NULL) 
// 		pTex->textureBuf = NULL;
// 	
// 	if ( FAILED(D3DXCreateTextureFromFileInMemoryEx(m_pd3dDevice,
// 		pImage->data,
// 		pImage->dataSize,
// 		D3DX_DEFAULT,
// 		D3DX_DEFAULT,
// 		mipLevel,
// 		0,
// 		pTex->BP,
// 		D3DPOOL_MANAGED,
// 		D3DX_DEFAULT,
// 		D3DX_DEFAULT,
// 		NULL,
// 		&pTex->imageInfo,
// 		NULL,
// 		&pTex->textureBuf) ) )
// 		return E_FAIL;
// 	
// 	pTex->width = pTex->imageInfo.Width;
// 	pTex->height = pTex->imageInfo.Height;
// 	
// 	return S_OK;
// }


//-----------------------------------------------------------------------------
// 方法名: CreateTexFromBuffer()
// 参  数: CTexture* pTex, UINT width, UINT height
// 描  述: 从Data缓冲创建贴图，指定尺寸
//-----------------------------------------------------------------------------
// HRESULT	CScene::CreateTexFromBuffer(CTexture* pTex, UINT width, UINT height, INT mipLevel)
// {
// 	//如果当前pTex的数据为空，则无法创建贴图。这将导致贴图列表缺失，造成材质索引贴图错误，程序将非法操作
// 	//为了解决这个问题: 如果当pTex的数据为空时，则创建一个默认的贴图，放到索引表中，这样贴图就不会缺失了。
// 	if (pTex->imageIdx < 0 || pTex->imageIdx >= m_vImage.size())
// 	{
// 		return E_FAIL;
// 	}
// 	CImage *pImage = m_vImage[pTex->imageIdx];
// 	if (pImage->data == NULL) 
// 		pTex->textureBuf = NULL;
// 	
// 	if (width > 2048 || height > 2048) 
// 	{
// 		MessageBox(GetActiveWindow(), "the m_vImage size exceed 2048*2048.", "Create Texture...", MB_OK|MB_ICONERROR);
// 		return E_FAIL;
// 	}
// 	
// 	if ( FAILED(D3DXCreateTextureFromFileInMemoryEx(m_pd3dDevice,
// 		pImage->data,
// 		pImage->dataSize,
// 		width,
// 		height,
// 		mipLevel,
// 		0,
// 		pTex->BP,
// 		D3DPOOL_MANAGED,
// 		D3DX_DEFAULT,
// 		D3DX_DEFAULT,
// 		NULL,
// 		&(pTex->imageInfo),
// 		NULL,
// 		&(pTex->textureBuf)) ) )
// 	{
// 		MessageBox(GetActiveWindow(), "Create Texture Error.", "Create Texture...", MB_OK|MB_ICONERROR);
// 		return E_FAIL;
// 	}
// 	
// 	pTex->width = width;
// 	pTex->height = height;
// 	
// 	return S_OK;
// }

//-----------------------------------------------------------------------------
// 方法名: CreateTexFromRGBBuffer()
// 参  数: CTexture* pTex
// 描  述: 从RGB缓冲中创建贴图，实际上是创建空的贴图后拷贝表面
//-----------------------------------------------------------------------------
// HRESULT	CScene::CreateTexFromRGBBuffer(CTexture* pTex, INT mipLevel)
// {
// 	if (pTex->buffer == NULL) return E_FAIL;
// 	
// 	if (FAILED( CreateEmptyTex(pTex, mipLevel)) )
// 		return E_FAIL;
// 	
// 	//每个像素所占的字节数
// 	UINT bp_size; 
// 	if (pTex->BP == D3DFMT_R8G8B8 || pTex->BP == D3DFMT_A8R8G8B8)
// 		bp_size = 4;
// 	else
// 		bp_size = 2;
// 	
// 	//由于创建了mipmap链，所以用LockRect要对每个Level的表面进行重新填充，这个用手动的方法几乎无法解决，
// 	//还好，D3D提供了D3DXLoadSurfaceFromMemory的方法可以解决这个问题
// 	UINT totallevels = pTex->textureBuf->GetLevelCount();
// 	for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
// 	{
// 		LPDIRECT3DSURFACE9 surflevel;
// 		pTex->textureBuf->GetSurfaceLevel(miplevel,&surflevel);
// 		
// 		RECT SourceRect = {0,0,pTex->width, pTex->height};
// 		if (FAILED (D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
// 			pTex->buffer, pTex->BP,
// 			pTex->width * bp_size,
// 			NULL, &SourceRect, D3DX_FILTER_LINEAR, 0) ) )
// 			return E_FAIL;
// 		
// 		surflevel->Release();
// 	}
// 	
// 	return S_OK;
// }

//VOID CScene::CreateCubeMapBuffer()
//{
//   UINT cubMapCnt =cubeMap.size();
//   CCubeMap** ppCubeMap =cubeMap.begin();
//   for(UINT i =0; i<cubMapCnt; i++)
//   {  
//        CCubeMap* pCubMap =*(ppCubeMap+ i);
// 	   if(!pCubMap->bCreate)
// 	     pCubMap->InitCubeMapD3DTexture();
//   }
//}

// VOID CScene::CreateDyTexFrameImageBuffer()
// {
// 	//将场景的图片序列中的动态贴图数据缓存填入
// 	UINT dyTexCnt =dynamicTexture.size();
// 	CDynamicTex** ppDyTex =dynamicTexture.begin();
// 	for(UINT i =0;i< dyTexCnt; i++)
// 	{
// 		CDynamicTex* pDyTex = *(ppDyTex+ i);
// 		UINT dyTexImageCnt =pDyTex->imagesIdx.size();
// 		for(UINT n =0; n<dyTexImageCnt; n++)
// 		{
// 			_ImageData* imageData =new _ImageData();
// 			imageData->DataSize =m_vImage[pDyTex->imagesIdx[n]]->dataSize;
// 			imageData->Data =new BYTE[imageData->DataSize];
// 			memcpy(imageData->Data, m_vImage[pDyTex->imagesIdx[n]]->data, imageData->DataSize);		 
// 			pDyTex->imageData.push_back(imageData);
// 		}  
// 		//图像缓存填充后清空图像索引队列
// 		pDyTex->imagesIdx.clear();
// 	}
// }

//从内存创建动态贴图
// HRESULT CScene::CreateDyTexFromFileInMemory(CTexture* pTex)
// {
// 	CImage *pImage = m_vImage[pTex->imageIdx];
// 	if (pImage->data == NULL) 
// 		pTex->textureBuf = NULL;
// 	
// 	if ( FAILED(D3DXCreateTextureFromFileInMemoryEx(m_pd3dDevice,
// 		pImage->data,
// 		pImage->dataSize,
// 		pTex->width,
// 		pTex->height,
// 		pTex->MipCount,
// 		D3DUSAGE_DYNAMIC,
// 		pTex->BP,
// 		D3DPOOL_DEFAULT,
// 		D3DX_DEFAULT,
// 		D3DX_DEFAULT,
// 		NULL,
// 		&pTex->imageInfo,
// 		NULL,
// 		&pTex->textureBuf) ) )
// 		return E_FAIL;
// 	
// 	//SAFE_DELETE_ARRAY(pTex->data);
// 	
// 	return S_OK;
// }

//-----------------------------------------------------------------------------
// 方法名: CreateTexFromFile()
// 参  数: CHAR* strFileName, CTexture* pTex
// 描  述: 从文件创建贴图
//-----------------------------------------------------------------------------
// HRESULT	CScene::CreateTexFromFile(CHAR* strFileName, CTexture* pTex, INT mipLevel)
// {
// 	if (strcmp(strFileName, "") == 0) return E_FAIL;
// 	if (strFileName == NULL) return E_FAIL;
// 	
// 	UINT preTexCnt = m_vTexture.size();
// 	CHAR errInfo[256];
// 	
// 	std::string sFilePath = _GetPureFilePathStr(strFileName);
// 	// std::string sFileName = _GetPureFileNameStr(strFileName);
// 	
// 	//texture file path and name
// 	strcpy(pTex->path, sFilePath.c_str());
// 	strcat(pTex->pathAndName, strFileName);
// 	
// 	//zip quality
// 	pTex->ZipQuality = 55;
// 	
// 	//创建图像的Buffer,并从Buffer创建贴图
// 	FILE* pTexFile = fopen(pTex->pathAndName, "rb");
// 	if (pTexFile == NULL)
// 	{
// 		strcpy(errInfo, "can not find texture file:");
// 		strcat(errInfo, pTex->pathAndName);
// 		MessageBox(NULL, errInfo, "read texture file error", MB_OK | MB_ICONERROR);
// 		ZeroMemory(errInfo, sizeof(errInfo) );
// 		return E_FAIL;
// 	}
// 	else
// 	{
// 		CImage* pImage =new CImage();
// 		pImage->index =m_vImage.size();
// 		fseek(pTexFile, 0, SEEK_END);
// 		pImage->dataSize = ftell(pTexFile);
// 		fseek(pTexFile, 0, SEEK_SET);
// 		pImage->data = new BYTE[pImage->dataSize];
// 		fread(pImage->data, pImage->dataSize, 1, pTexFile);
// 		fclose(pTexFile);
// 		if (pImage->data == NULL)
// 		{
// 			strcpy(errInfo, "read texture file:");
// 			strcat(errInfo, pTex->pathAndName);
// 			strcat(errInfo, "error");
// 			MessageBox(NULL, errInfo, "read local texture file error", MB_OK | MB_ICONERROR);
// 			ZeroMemory(errInfo, sizeof(errInfo) );
// 			return E_FAIL;
// 		}
// 		pTex->imageIdx =pImage->index;
// 		m_vImage.push_back(pImage);
// 	}	
// 	
// 	CreateTexFromBuffer(pTex, mipLevel);
// 	ClearImageList();
// 	
// 	return S_OK;
// }

CTexture* CScene::CreateEmptyTex(const std::string &sName, D3DFORMAT format, INT width, INT height, INT mipLevel)
{
	if (!m_pRoot || m_pRoot->GetTextureManager().IsTextureExist(sName))
	{
		return 0; 
	}

	CTexture *pTexture = pTexture = (CTexture*)m_pRoot->GetTextureManager().createEmptyTexture(sName, format, width, height, mipLevel);
	if (pTexture)
	{
		pTexture->isOpacity = _IsPureAlphaFormat(format);
		pTexture->index = getTextureCount();
		m_vTexture.push_back(pTexture);
	}
	return pTexture;
}

// 从文件创建一个Texture对象
CTexture* CScene::CreateTexFromFile(const CHAR* strFileName, BOOL bOpacityOnly, const CHAR *strTextureName, INT mipLevel)
{
	if (!m_pRoot || m_pRoot->GetTextureManager().IsTextureExist(strTextureName))
	{
		return 0;
	}
	CTexture* pTexture = (CTexture*)m_pRoot->GetTextureManager().createTexture(strFileName, bOpacityOnly, mipLevel, strTextureName);
	if (pTexture)
	{
		pTexture->index = getTextureCount();
		m_vTexture.push_back(pTexture);
	}
	return pTexture;

// 	CTexture* pTex = new CTexture();
// 	if (pTex)
// 	{
// 		pTex->index = m_vTexture.size();
// 		// strncpy(pTex->name, strTextureName, 32);
// 		pTex->m_sName = std::string(strTextureName);
// 
// 		_D3DXIMAGE_INFO imageInfo;
// 		D3DXGetImageInfoFromFile(strFileName, &imageInfo);
// 
// 		pTex->BP = imageInfo.Format;
// 		HRESULT re = CreateTexFromFile((CHAR*)strFileName, pTex, mipLevel);
// 		
// 		if (pTex->BP == D3DFMT_A4R4G4B4 || 
// 			pTex->BP == D3DFMT_A1R5G5B5 || 
// 			pTex->BP == D3DFMT_A8R8G8B8)
// 		{
// 			pTex->bAlphaBlend = TRUE;
// 		}
// 	}
// 	return pTex;
}

// 从文件创建一个Texture对象
// CTexture* CScene::CreateTexFromBuffer(const VOID* pBuffer, BOOL bOpacityOnly, const CHAR *strTextureName, INT mipLevel)
// {
// 	if (CTextureManager::getSingleton().IsTextureExist(strTextureName))
// 	{
// 		return 0;
// 	}
// 	CTexture* pTexture = (CTexture*)CTextureManager::getSingleton().createTexture(strFileName, bOpacityOnly, mipLevel, strTextureName);
// 	if (pTexture)
// 	{
// 		m_vTexture.push_back(pTexture);
// 	}
// 	return pTexture;
// }
//-----------------------------------------------------------------------------
// 方法名: SetTexBrightness()
// 参  数: CTexture* pTex, UINT brightness
// 描  述: 这里是设置贴图的亮度，实际上是为了与其它贴图混色而设置的函数
// 当贴图为白色时，与其它贴图混色无效果, MODULATE
//-----------------------------------------------------------------------------
HRESULT CScene::SetTexBrightness(CTexture* pTex, UINT brightness)
{
	if (brightness <0 || brightness > 100) return E_FAIL;
	if (pTex == NULL) return E_FAIL;
	
	pTex->brightness = brightness;
	
	//设置Opacity透明度, 实际上是调节O
	UINT bp_dif = 0;
	UINT bufSize = 0;
	if (pTex->m_format == D3DFMT_R8G8B8)
		bp_dif = 4;
	else
		bp_dif = 2;
	bufSize = pTex->width * pTex->height * bp_dif;
	
	D3DLOCKED_RECT DifLockRect;
	RECT DifRect;
	DifRect.left = DifRect.top = 0;
	DifRect.right = pTex->width;
	DifRect.bottom = pTex->height;
	if (FAILED((pTex->m_textureBuf)->LockRect(0, &DifLockRect, &DifRect, D3DLOCK_DISCARD)))
		return E_FAIL;
	
	pTex->buffer = new BYTE[bufSize];
	memcpy(pTex->buffer, (BYTE*)DifLockRect.pBits, bufSize);
	//unlock texture rect
	pTex->m_textureBuf->UnlockRect(0);
	
	FLOAT alpha_red_step;
	FLOAT alpha_green_step;
	FLOAT alpha_blue_step;
	
	//R8G8B8
	if (pTex->m_format == D3DFMT_R8G8B8 )
	{	
		for (int m =0; m< DifRect.bottom; m++)
		{
			for (int n = 0; n< DifRect.right; n++)
			{
				UINT index = 4*(m*DifRect.right + n);
				if (brightness > 0)
				{
					alpha_red_step = (255 - pTex->buffer[index + 0])/100.0f;
					alpha_green_step = (255 - pTex->buffer[index + 1])/100.0f;
					alpha_blue_step = (255 - pTex->buffer[index + 1])/100.0f;
				}
				else
				{
					alpha_red_step = - pTex->buffer[index + 0]/100.0f;
					alpha_green_step =  - pTex->buffer[index + 1]/100.0f;
					alpha_green_step =  - pTex->buffer[index + 2]/100.0f;
				}
				
				BYTE dif_red = pTex->buffer[index + 0] + (BYTE)(alpha_red_step * brightness);
				if (dif_red > 255 ) dif_red = 255;
				if (dif_red < 0) dif_red = 0;
				BYTE dif_green = pTex->buffer[index + 1] + (BYTE)(alpha_green_step * brightness);
				if (dif_green > 255) dif_green = 255;
				if (dif_green < 0) dif_green = 0;
				BYTE dif_blue = pTex->buffer[index + 2] + (BYTE)(alpha_blue_step * brightness);
				if (dif_blue > 255) dif_blue = 255;
				if (dif_blue < 0) dif_blue = 0;
				
				pTex->buffer[index + 0] = dif_red;
				pTex->buffer[index + 1] = dif_green;
				pTex->buffer[index + 2] = dif_blue;
				//no alpha
			}
		}
	}
	
	if (pTex->m_format == D3DFMT_R5G6B5)
	{	
		for (int m =0; m< DifRect.bottom; m++)
		{
			for (int n = 0; n< DifRect.right; n++)
			{
				UINT index = 2*(m*DifRect.right + n);
				
				BYTE ori_red = (pTex->buffer[index+1]) & 0xf8;
				BYTE ori_green = ((pTex->buffer[index+1]) & 0x07)<<5 | (((pTex->buffer[index])>>3) & 0x1c);
				BYTE ori_blue = ((pTex->buffer[index])<<3) & 0xf8;
				
				if (brightness > 0)
				{
					alpha_red_step = (255 - ori_red)/100.0f;
					alpha_green_step = (255 - ori_green)/100.0f;
					alpha_blue_step = (255 - ori_blue)/100.0f;
				}
				else
				{
					alpha_red_step = - ori_red/100.0f;
					alpha_green_step = - ori_green/100.0f;
					alpha_blue_step = - ori_blue/100.0f;
				}
				
				BYTE dif_red = ori_red + (BYTE)(alpha_red_step * brightness);
				if (dif_red > 255) dif_red = 255;
				if (dif_red < 0) dif_red = 0;
				
				BYTE dif_green = ori_green + (BYTE)(alpha_green_step * brightness); 
				if (dif_green > 255) dif_green = 255;
				if (dif_green < 0) dif_green = 0;
				
				BYTE dif_blue = ori_blue + (BYTE)(alpha_blue_step * brightness);
				if (dif_blue > 255) dif_blue = 255;
				if (dif_blue < 0) dif_blue = 0;
				
				pTex->buffer[index+1] = (BYTE)(((dif_red >>3) & 0xf8) | ((dif_green >>5) & 0x07));
				pTex->buffer[index] = (BYTE)( ((dif_green <<5) &0xe0) | ((dif_blue >>3) & 0x1f));
			}
		}
	}
	
	//由于创建了mipmap链，所以用LockRect要对每个Level的表面进行重新填充，这个用手动的方法几乎无法解决，
	//还好，D3D提供了D3DXLoadSurfaceFromMemory的方法可以解决这个问题
	UINT totallevels = pTex->m_textureBuf->GetLevelCount();
	for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
	{
		LPDIRECT3DSURFACE9 surflevel;
		pTex->m_textureBuf->GetSurfaceLevel(miplevel,&surflevel);
		
		RECT SourceRect = {0,0,pTex->width, pTex->height};
		D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
			pTex->buffer, pTex->m_format,
			pTex->width * bp_dif,
			NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);
		
		surflevel->Release();
	}
	
	SAFE_DELETE_ARRAY(pTex->buffer);
	return S_OK;
}
//HRESULT CScene::CreateTransparentTexture(CTexture* pTex)
//{
// 	if(!pTex)
// 		return E_FAIL;
// 	
// 	if(pTex->isOpacity && (pTex->imageIdx!=-1) && m_vImage[pTex->imageIdx])
// 	{
// 		
// 		//获取图像信息
// 		D3DXIMAGE_INFO pSrcInfo,tSrcInfo;
// 		LPDIRECT3DTEXTURE9 pDTexture=NULL;
// 		CImage* pImage =m_vImage[pTex->imageIdx];
// 		
// 		if(FAILED(D3DXGetImageInfoFromFileInMemory(pImage->data, pImage->dataSize, &pSrcInfo)))
// 			return E_FAIL;
// 		
// 		//通过Opacity原图创建临时数据buffer,得到以A8R8G8B8格式存储的数据流
// 		//CImage* pImage =m_vImage[pTex->imageIdx];
// 		
// 		if (FAILED(D3DXCreateTextureFromFileInMemoryEx(m_pd3dDevice,
// 			pImage->data,
// 			pImage->dataSize,
// 			D3DX_DEFAULT,
// 			D3DX_DEFAULT,
// 			pTex->MipCount,
// 			0,
// 			D3DFMT_A8R8G8B8,
// 			D3DPOOL_MANAGED,
// 			D3DX_DEFAULT,
// 			D3DX_DEFAULT,
// 			NULL,
// 			&tSrcInfo,
// 			NULL,
// 			&pDTexture) ) )
// 			return E_FAIL;
// 		
// 		pTex->width = pSrcInfo.Width;
// 		pTex->height = pSrcInfo.Height;
// 		pTex->BP = D3DFMT_A8;
// 		
// 		//创建目标贴图缓存
// 		if (FAILED(CreateEmptyTex(pTex,pTex->MipCount)))
// 			return E_FAIL;
// 		
// 		UINT bufSize = pTex->width * pTex->height * 4;
// 		
// 		D3DLOCKED_RECT OriLockRect;
// 		RECT OriRect;
// 		OriRect.left = OriRect.top = 0;
// 		OriRect.right = pTex->width;
// 		OriRect.bottom = pTex->height;
// 		if (FAILED(pDTexture->LockRect(0, &OriLockRect, &OriRect, D3DLOCK_DISCARD)))
// 			return E_FAIL;
// 		BYTE* OriBuf = new BYTE[bufSize];
// 		BYTE* DesBuf = new BYTE[bufSize/4];
// 		memcpy(OriBuf, (BYTE*)OriLockRect.pBits, bufSize);
// 		//unlock texture rect
// 		pDTexture->UnlockRect(0);
// 		BOOL withAlpha =FALSE;
// 		if(pSrcInfo.Format ==D3DFMT_A8R8G8B8 || pSrcInfo.Format ==D3DFMT_A8 || pSrcInfo.Format ==D3DFMT_A1R5G5B5 || pSrcInfo.Format ==D3DFMT_A4R4G4B4 || pSrcInfo.Format == D3DFMT_A8R3G3B2 || pSrcInfo.Format == D3DFMT_A2B10G10R10)
// 			withAlpha =TRUE;
// 		bufSize=bufSize/4;
// 		/*for (int m =0; m< OriRect.bottom; m++)
// 		{
// 		for (int n = 0; n< OriRect.right; n++)
// 		{
// 		UINT index = 4*(m*OriRect.right + n);
// 		BYTE alpha = (UINT)(0.299*OriBuf[index+0] +0.587*OriBuf[index+1] +0.114*OriBuf[index+2]);
// 		DesBuf[index + 0] = OriBuf[index+0];  
// 		DesBuf[index + 1] = OriBuf[index+1]; 
// 		DesBuf[index + 2] = OriBuf[index+2];
// 		DesBuf[index + 3] = alpha;
// 		}
// 	}*/
// 		for(UINT i =0 ;i< bufSize; i++)
// 		{
// 			BYTE alpha;
// 			if(withAlpha)
// 				alpha =OriBuf[i*4+3];
// 			else
// 			{
// 				alpha = (UINT)(0.299*OriBuf[i*4+0] +0.587*OriBuf[i*4+1] +0.114*OriBuf[i*4+2]);
// 			}
// 			DesBuf[i] =alpha;
// 		}
// 		SAFE_RELEASE_D3D(pDTexture);
// 		
// 		//重写该层材质的贴图缓存
// 		UINT totallevels = 0;
// 		if (pTex->MipCount == 0)
// 			totallevels = pTex->textureBuf->GetLevelCount();
// 		else
// 			totallevels = pTex->MipCount;
// 		
// 		for (UINT miplevel = 0; miplevel< totallevels; miplevel++)
// 		{
// 			LPDIRECT3DSURFACE9 surflevel;
// 			pTex->textureBuf->GetSurfaceLevel(miplevel,&surflevel);
// 			
// 			RECT SourceRect = {0,0,pTex->width, pTex->height};
// 			D3DXLoadSurfaceFromMemory(surflevel, NULL, NULL,
// 				DesBuf, pTex->BP,
// 				pTex->width,
// 				NULL, &SourceRect, D3DX_FILTER_LINEAR, 0);
// 			
// 			surflevel->Release();
// 		}
// 		
// 		//回收过渡缓冲内存
// 		SAFE_DELETE_ARRAY(OriBuf);
// 		SAFE_DELETE_ARRAY(DesBuf);  
// 	}
// 	
// 	//if(pTex
// 	return S_OK;
//}

/*
//-----------------------------------------------------------------------------
// 方法名: CreateModelVBuffer()
// 参  数: CModel* pModel
// 描  述: 创建当前模型的顶点缓冲
//-----------------------------------------------------------------------------
HRESULT CScene::CreateModelVBuffer(CModel* pModel)
{
	if (pModel->vertex == NULL)
		return E_FAIL;
	
	if( FAILED( m_pd3dDevice->CreateVertexBuffer( pModel->vertexCnt * pModel->m_FVFSize,
		0,
		pModel->m_FVF,
		D3DPOOL_MANAGED,
		&(pModel->m_VBuffer) ) ) )
	{
#ifdef _DEBUG
		MessageBox(GetActiveWindow(), pModel->name, "ERROR!! CScene::CreateModelVBuffer(CModel* pModel)", MB_OK|MB_ICONERROR);
#endif
		return E_FAIL;
	}
	
	//锁定内存
	if( FAILED(pModel->m_VBuffer->Lock(0, pModel->vertexCnt * pModel->m_FVFSize,
		(BYTE**)&(pModel->m_pVertices), D3DLOCK_DISCARD) ))
	{
		return E_FAIL;
	}
	
	//判断顶点的FVF,不同的FVF填充不同的数据
	UINT i;
	switch(pModel->m_FVF)
	{
	case FVF_T1:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T1MESHVERTEX*)pModel->m_pVertices)[i].position = pModel->vertex[i].position;
			((T1MESHVERTEX*)pModel->m_pVertices)[i].diffuse = pModel->vertex[i].diffuse;
		}
		break;
		
	case FVF_T2:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T2MESHVERTEX*)pModel->m_pVertices)[i].position = pModel->vertex[i].position;	
			((T2MESHVERTEX*)pModel->m_pVertices)[i].normal = pModel->vertex[i].normal;
			((T2MESHVERTEX*)pModel->m_pVertices)[i].diffuse = pModel->vertex[i].diffuse;
		}
		break;
		
	case FVF_T3:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T3MESHVERTEX*)pModel->m_pVertices)[i].position = pModel->vertex[i].position;
			((T3MESHVERTEX*)pModel->m_pVertices)[i].diffuse = pModel->vertex[i].diffuse;
			((T3MESHVERTEX*)pModel->m_pVertices)[i].su = pModel->vertex[i].uv1.u;
			((T3MESHVERTEX*)pModel->m_pVertices)[i].sv = pModel->vertex[i].uv1.v;
		}
		break;
		
	case FVF_T4:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T4MESHVERTEX*)pModel->m_pVertices)[i].position = pModel->vertex[i].position;	
			((T4MESHVERTEX*)pModel->m_pVertices)[i].normal = pModel->vertex[i].normal;
			((T4MESHVERTEX*)pModel->m_pVertices)[i].diffuse = pModel->vertex[i].diffuse;
			((T4MESHVERTEX*)pModel->m_pVertices)[i].su = pModel->vertex[i].uv1.u;
			((T4MESHVERTEX*)pModel->m_pVertices)[i].sv = pModel->vertex[i].uv1.v;
		}
		break;
		
	case FVF_T5:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T5MESHVERTEX*)pModel->m_pVertices)[i].position = pModel->vertex[i].position;	
			((T5MESHVERTEX*)pModel->m_pVertices)[i].diffuse = pModel->vertex[i].diffuse;
			((T5MESHVERTEX*)pModel->m_pVertices)[i].su = pModel->vertex[i].uv1.u;
			((T5MESHVERTEX*)pModel->m_pVertices)[i].sv = pModel->vertex[i].uv1.v;
			((T5MESHVERTEX*)pModel->m_pVertices)[i].sw = pModel->vertex[i].uv2.u;
			((T5MESHVERTEX*)pModel->m_pVertices)[i].sz = pModel->vertex[i].uv2.v;
		}
		break;
		
	case FVF_T6:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T6MESHVERTEX*)pModel->m_pVertices)[i].position = pModel->vertex[i].position;	
			((T6MESHVERTEX*)pModel->m_pVertices)[i].normal = pModel->vertex[i].normal;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].diffuse = pModel->vertex[i].diffuse;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].su = pModel->vertex[i].uv1.u;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].sv = pModel->vertex[i].uv1.v;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].sw = pModel->vertex[i].uv2.u;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].sz = pModel->vertex[i].uv2.v;
		}
		break;
	case FVF_T7:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T7MESHVERTEX*)pModel->m_pVertices)[i].position = pModel->vertex[i].position;	
			((T7MESHVERTEX*)pModel->m_pVertices)[i].normal = pModel->vertex[i].normal;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].diffuse = pModel->vertex[i].diffuse;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].su1 = pModel->vertex[i].uv1.u;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].sv1 = pModel->vertex[i].uv1.v;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].su2 = pModel->vertex[i].uv2.u;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].sv2 = pModel->vertex[i].uv2.v;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].su3 = pModel->vertex[i].uv3.u;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].sv3 = pModel->vertex[i].uv3.v;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].su4 = pModel->vertex[i].uv4.u;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].sv4 = pModel->vertex[i].uv4.v;
		}
		break;
	}
	//Unlock顶点缓冲
	pModel->m_VBuffer->Unlock();
	
	return S_OK;
}

//-----------------------------------------------------------------------------
// 方法名: CloneModelVBuffer()
// 参  数: CModel* pModel,CModel* pOriModel
// 描  述: 复制模型的顶点缓冲
//-----------------------------------------------------------------------------

HRESULT CScene::CloneModelVBuffer(CModel*pModel,CModel* pOriModel)
{
	
	if( FAILED( m_pd3dDevice->CreateVertexBuffer( pModel->vertexCnt * pModel->m_FVFSize,
		0,
		pModel->m_FVF,
		D3DPOOL_MANAGED,
		&(pModel->m_VBuffer) ) ) )
	{
#ifdef _DEBUG
		MessageBox(GetActiveWindow(), pModel->name, "ERROR!! CScene::CreateModelVBuffer(CModel* pModel)", MB_OK|MB_ICONERROR);
#endif
		return E_FAIL;
	}
	
	//锁定内存
	if( FAILED(pModel->m_VBuffer->Lock(0, pModel->vertexCnt * pModel->m_FVFSize,
		(BYTE**)&(pModel->m_pVertices), D3DLOCK_DISCARD) ))
	{
		return E_FAIL;
	}
	
	//判断顶点的FVF,不同的FVF填充不同的数据
	UINT i;
	switch(pModel->m_FVF)
	{
	case FVF_T1:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T1MESHVERTEX*)pModel->m_pVertices)[i].position = ((T1MESHVERTEX*)pOriModel->m_pVertices)[i].position ;
			((T1MESHVERTEX*)pModel->m_pVertices)[i].diffuse = ((T1MESHVERTEX*)pOriModel->m_pVertices)[i].diffuse;
		}
		break;
		
	case FVF_T2:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T2MESHVERTEX*)pModel->m_pVertices)[i].position = ((T2MESHVERTEX*)pOriModel->m_pVertices)[i].position;	
			((T2MESHVERTEX*)pModel->m_pVertices)[i].normal = ((T2MESHVERTEX*)pOriModel->m_pVertices)[i].normal;
			((T2MESHVERTEX*)pModel->m_pVertices)[i].diffuse = ((T2MESHVERTEX*)pOriModel->m_pVertices)[i].diffuse;
		}
		break;
		
	case FVF_T3:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T3MESHVERTEX*)pModel->m_pVertices)[i].position = ((T3MESHVERTEX*)pOriModel->m_pVertices)[i].position;
			((T3MESHVERTEX*)pModel->m_pVertices)[i].diffuse = ((T3MESHVERTEX*)pOriModel->m_pVertices)[i].diffuse;
			((T3MESHVERTEX*)pModel->m_pVertices)[i].su = ((T3MESHVERTEX*)pOriModel->m_pVertices)[i].su;
			((T3MESHVERTEX*)pModel->m_pVertices)[i].sv = ((T3MESHVERTEX*)pOriModel->m_pVertices)[i].sv;
		}
		break;
		
	case FVF_T4:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T4MESHVERTEX*)pModel->m_pVertices)[i].position = ((T4MESHVERTEX*)pOriModel->m_pVertices)[i].position;	
			((T4MESHVERTEX*)pModel->m_pVertices)[i].normal = ((T4MESHVERTEX*)pOriModel->m_pVertices)[i].normal;
			((T4MESHVERTEX*)pModel->m_pVertices)[i].diffuse = ((T4MESHVERTEX*)pOriModel->m_pVertices)[i].diffuse;
			((T4MESHVERTEX*)pModel->m_pVertices)[i].su = ((T4MESHVERTEX*)pOriModel->m_pVertices)[i].su;
			((T4MESHVERTEX*)pModel->m_pVertices)[i].sv = ((T4MESHVERTEX*)pOriModel->m_pVertices)[i].sv;
		}
		break;
		
	case FVF_T5:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T5MESHVERTEX*)pModel->m_pVertices)[i].position = ((T5MESHVERTEX*)pOriModel->m_pVertices)[i].position;	
			((T5MESHVERTEX*)pModel->m_pVertices)[i].diffuse = ((T5MESHVERTEX*)pOriModel->m_pVertices)[i].diffuse;
			((T5MESHVERTEX*)pModel->m_pVertices)[i].su = ((T5MESHVERTEX*)pOriModel->m_pVertices)[i].su;
			((T5MESHVERTEX*)pModel->m_pVertices)[i].sv = ((T5MESHVERTEX*)pOriModel->m_pVertices)[i].sv;
			((T5MESHVERTEX*)pModel->m_pVertices)[i].sw = ((T5MESHVERTEX*)pOriModel->m_pVertices)[i].sw;
			((T5MESHVERTEX*)pModel->m_pVertices)[i].sz = ((T5MESHVERTEX*)pOriModel->m_pVertices)[i].sz;
		}
		break;
		
	case FVF_T6:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T6MESHVERTEX*)pModel->m_pVertices)[i].position = ((T6MESHVERTEX*)pOriModel->m_pVertices)[i].position;	
			((T6MESHVERTEX*)pModel->m_pVertices)[i].normal = ((T6MESHVERTEX*)pOriModel->m_pVertices)[i].normal;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].diffuse = ((T6MESHVERTEX*)pOriModel->m_pVertices)[i].diffuse;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].su = ((T6MESHVERTEX*)pOriModel->m_pVertices)[i].su;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].sv = ((T6MESHVERTEX*)pOriModel->m_pVertices)[i].sv;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].sw = ((T6MESHVERTEX*)pOriModel->m_pVertices)[i].sw;
			((T6MESHVERTEX*)pModel->m_pVertices)[i].sz = ((T6MESHVERTEX*)pOriModel->m_pVertices)[i].sz;
		}
		break;
	case FVF_T7:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
		{
			//填充坐标数据到顶点缓冲
			((T7MESHVERTEX*)pModel->m_pVertices)[i].position = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].position;	
			((T7MESHVERTEX*)pModel->m_pVertices)[i].normal = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].normal;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].diffuse = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].diffuse;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].su1 = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].su1;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].sv1 = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].sv1;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].su2 = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].su2;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].sv2 = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].sv2;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].su3 = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].su3;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].sv3 = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].sv3;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].su4 = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].su4;
			((T7MESHVERTEX*)pModel->m_pVertices)[i].sv4 = ((T7MESHVERTEX*)pOriModel->m_pVertices)[i].sv4;
		}
		break;
	}
	//Unlock顶点缓冲
	pModel->m_VBuffer->Unlock();
	
	return S_OK;
	
}

//-----------------------------------------------------------------------------
// 方法名: CreateModelIBuffer()
// 参  数: CModel* pModel
// 描  述: 创建当前模型的索引缓冲
//-----------------------------------------------------------------------------
HRESULT CScene::CreateModelIBuffer(CModel* pModel)
{
	
	int bufUnitSize = 0;
	for(UINT i=0;i<pModel->subMeshCnt;i++)
	{
	       
		_subMesh* pSubMesh =&(pModel->subMesh[i]);
		
		if (pSubMesh->face == NULL)
			return E_FAIL;
		
		if (pModel->vertexCnt < 65536)
		{
			pSubMesh->IBufferFormat = D3DFMT_INDEX16;
			bufUnitSize = sizeof(unsigned short);
		}
		else
		{
			pSubMesh->IBufferFormat = D3DFMT_INDEX32;
			bufUnitSize = sizeof(unsigned int);
		}
		//判断单个模型的顶点数量是否超过了16位索引缓冲的最大顶点容量
		
		
		//建立当前模型的索引缓冲
		if( FAILED( m_pd3dDevice ->CreateIndexBuffer(pSubMesh->faceCnt * 3 * bufUnitSize,
			0,
			pSubMesh->IBufferFormat,
			D3DPOOL_MANAGED,
			&pSubMesh->m_IBuffer) ) )
		{
			return E_FAIL;
		}
		//锁定内存
		if( FAILED(pSubMesh->m_IBuffer->Lock(0, pSubMesh->faceCnt * 3 * bufUnitSize, (BYTE**)&pSubMesh->m_pIndices, 0) ))
		{
			return E_FAIL;
		}
		
		//16位顶点索引缓冲
		if (pSubMesh->IBufferFormat == D3DFMT_INDEX16)
		{
			//逐个面填充索引数据
			for (UINT j =0; j< pSubMesh->faceCnt; j++)
			{
				((unsigned short*)pSubMesh->m_pIndices)[3*j] = pSubMesh->face[j].vertices0->index;
				((unsigned short*)pSubMesh->m_pIndices)[3*j+1] = pSubMesh->face[j].vertices1->index;
				((unsigned short*)pSubMesh->m_pIndices)[3*j+2] = pSubMesh->face[j].vertices2->index;
			}
		}
		
		//32位顶点索引缓冲
		if (pSubMesh->IBufferFormat == D3DFMT_INDEX32)
		{
			//逐个面填充索引数据
			for (UINT j =0; j< pSubMesh->faceCnt; j++)
			{
				((unsigned int*)pSubMesh->m_pIndices)[3*j] = pSubMesh->face[j].vertices0->index;
				((unsigned int*)pSubMesh->m_pIndices)[3*j+1] = pSubMesh->face[j].vertices1->index;
				((unsigned int*)pSubMesh->m_pIndices)[3*j+2] = pSubMesh->face[j].vertices2->index;
                
			}
		}		
		//Unlock索引缓冲
		pSubMesh->m_IBuffer->Unlock();
		
		//删除模型对象中的面数据，节省内存
		SAFE_DELETE_ARRAY(pSubMesh->face);		   
		pModel->SetMtrl(pModel->subMesh[i].shader,i);
		
	}

	// SmoothModelNormal(pModel);       // fuxb 2008-09-02	

	//删除模型对象中的顶点数据，节省内存
	SAFE_DELETE_ARRAY(pModel->vertex);

	
	return S_OK;
}
//-----------------------------------------------------------------------------
// 方法名: CloneModelIBuffer()
// 参  数: CModel* pModel,CModel* pOriModel
// 描  述: 复制模型的索引缓冲
//-----------------------------------------------------------------------------
HRESULT CScene::CloneModelIBuffer(CModel* pModel,CModel* pOriModel)
{
	int bufUnitSize = 0;
	for(UINT i=0;i<pModel->subMeshCnt;i++)
	{
	       
		_subMesh* pSubMesh =&(pModel->subMesh[i]);
		_subMesh* pOriSubMesh =&(pOriModel->subMesh[i]);
		
		if (pSubMesh->face == NULL)
			return E_FAIL;
		
		if (pModel->vertexCnt < 65536)
		{
			pSubMesh->IBufferFormat = D3DFMT_INDEX16;
			bufUnitSize = sizeof(unsigned short);
		}
		else
		{
			pSubMesh->IBufferFormat = D3DFMT_INDEX32;
			bufUnitSize = sizeof(unsigned int);
		}
		//判断单个模型的顶点数量是否超过了16位索引缓冲的最大顶点容量
		
		
		//建立当前模型的索引缓冲
		if( FAILED( m_pd3dDevice ->CreateIndexBuffer(pSubMesh->faceCnt * 3 * bufUnitSize,
			0,
			pSubMesh->IBufferFormat,
			D3DPOOL_MANAGED,
			&pSubMesh->m_IBuffer) ) )
		{
			return E_FAIL;
		}
		//锁定内存
		if( FAILED(pSubMesh->m_IBuffer->Lock(0, pSubMesh->faceCnt * 3 * bufUnitSize, (BYTE**)&pSubMesh->m_pIndices, 0) ))
		{
			return E_FAIL;
		}
		
		//16位顶点索引缓冲
		if (pSubMesh->IBufferFormat == D3DFMT_INDEX16)
		{
			//逐个面填充索引数据
			for (UINT i =0; i< pSubMesh->faceCnt; i++)
			{
				((unsigned short*)pSubMesh->m_pIndices)[3*i] = pOriSubMesh->face[i].vertices0->index;
				((unsigned short*)pSubMesh->m_pIndices)[3*i+1] = pOriSubMesh->face[i].vertices1->index;
				((unsigned short*)pSubMesh->m_pIndices)[3*i+2] = pOriSubMesh->face[i].vertices2->index;
			}
		}
		
		//32位顶点索引缓冲
		if (pSubMesh->IBufferFormat == D3DFMT_INDEX32)
		{
			//逐个面填充索引数据
			for (UINT i =0; i< pSubMesh->faceCnt; i++)
			{
				((unsigned int*)pSubMesh->m_pIndices)[3*i] = pOriSubMesh->face[i].vertices0->index;
				((unsigned int*)pSubMesh->m_pIndices)[3*i+1] = pOriSubMesh->face[i].vertices1->index;
				((unsigned int*)pSubMesh->m_pIndices)[3*i+2] = pOriSubMesh->face[i].vertices2->index;
			}
		}
		
		//Unlock索引缓冲
		pSubMesh->m_IBuffer->Unlock();
	}
	
	return S_OK;
}
*/

HRESULT CScene::CreateReflectEffect(CModel *pRefModel, UINT width, UINT height, float power)
 {
// 	if(pRefModel->sceneRefInfoListIdx != -1)
// 		return S_OK;
// 	
// 	ReflectInfo  refInfo;
// 
// 	pRefModel->sceneRefInfoListIdx = reflectInfoList.size();
// 	//获得物体Y轴的世界坐标系向量
//     D3DXVECTOR3 dir,desDir;
// 	dir= D3DXVECTOR3(0, 1, 0);
//     D3DXVec3TransformNormal(&desDir, &dir, &pRefModel->pNode->curTransform.worldMatrix);
// 	//设置反射平面
// 	D3DXPlaneFromPointNormal( &refInfo.refPlane , &pRefModel->pNode->worldPosition, &desDir );
// 	//创建反射材质
// 	CTexture* pRefTex =new CTexture();
// 	
// 	pRefTex->index = texture.size();
// 	pRefTex->BP = D3DFMT_R8G8B8;
// 	pRefTex->isOpacity = FALSE;
// 	
// 	pRefTex->width = width;
// 	pRefTex->height =height;
// 	//混色模式选择		   
// 	pRefTex->bAlphaBlend = FALSE;
// 	pRefTex->bAlphaTest= FALSE;
// 	
// 	
// 	//图像数据索引
// 	pRefTex->imageIdx = 0;		  
// 	texture.push_back(pRefTex);
// 	//创建D3D材质
// 	if(FAILED(m_pd3dDevice->CreateTexture(pRefTex->width, 
// 		pRefTex->height, 
// 		1, D3DUSAGE_RENDERTARGET, 
// 		m_d3dpp.BackBufferFormat, 
// 		D3DPOOL_DEFAULT, 
// 		&refInfo.renderToTexInfo.curRenderTexture)))
// 		return E_FAIL;
// 	//创建渲染到材质表面
// 	refInfo.renderToTexInfo.curRenderTexture->GetSurfaceLevel(0,&refInfo.renderToTexInfo.curRenderSurface);
// 	//创建深度缓存表面
// 	if (FAILED(m_pd3dDevice->CreateDepthStencilSurface(pRefTex->width,  
// 		pRefTex->height, 
// 		D3DFMT_D16,
// 		D3DMULTISAMPLE_NONE/*m_pd3dpp->MultiSampleType*/,
// 		&refInfo.renderToTexInfo.curRenderZSurface)))
// 		return E_FAIL;
// 	
// 	pRefTex->textureBuf =refInfo.renderToTexInfo.curRenderTexture;
// 	TextureLayer* pRefTexLayer =new TextureLayer();
// 	//创建贴图层
// 	ZeroMemory(pRefTexLayer->Name,sizeof(pRefTexLayer->Name));
// 	strcpy(pRefTexLayer->Name,"ReflectTex");
// 	pRefTexLayer->type =TLT_REALTIMEREF;
// 	pRefTexLayer->UVIndex =0;
// 	//设置混色模式
// 	pRefTexLayer->Blenderinfo.AlphaBlendOption=D3DTOP_MODULATE;
// 	pRefTexLayer->Blenderinfo.ColorBlendOption=D3DTOP_BLENDFACTORALPHA;
// 	//设置混色参数
// 	D3DCOLOR blendFactor = D3DCOLOR_COLORVALUE( 0, 0, 0, power);
// 	pRefTexLayer->Blenderinfo.ColorBlendParam = blendFactor;
// 	pRefTexLayer->pTexture = pRefTex;
// 	//
// 	refInfo.renderToTexInfo.texStage = pRefModel->subMesh[0].shader->texLayersList.size();
// 	//反射物体必须只有一个子物体
// 	pRefModel->subMesh[0].shader->texLayersList.push_back(pRefTexLayer);
// 	//添加到场景反射列表
// 	reflectInfoList.push_back(refInfo);
// 	pRefModel->beReflect =TRUE;
	
    return S_OK;
}
//设置物体的反射物体
VOID CScene::SetModelReflected(CModel* pRefModel,CModel* pRefedModel,BOOL checkFlag)
{
   if(checkFlag)
   {
      UINT RefedModelsCnt = pRefedModel->refedModelIdxList.size();
	  for(UINT i =0; i< RefedModelsCnt; i++)
	  {
	     if(pRefedModel->refedModelIdxList[i] == pRefModel->sceneRefInfoListIdx)
			 return;
	  }
   }
   pRefedModel->refedModelIdxList.push_back(pRefModel->sceneRefInfoListIdx);
       //设置反射物体类型为反射体
   pRefedModel->bReflect = TRUE;
}
//-----------------------------------------------------------------------------
// 方法名: IntersectModel()
// 参  数: UINT x, UINT y
// 描  述: 射线求交
//-----------------------------------------------------------------------------
BOOL CScene::IntersectModel(D3DXVECTOR3* orig, 
							D3DXVECTOR3* dir, 
							CModel* pModel, 
							D3DXVECTOR3* pNormal, 
							INT*   pFaceIndex,
							FLOAT* pSU,
							FLOAT* pSV,
							FLOAT* pDistance)
{
	//if (pModel->visible == false)
	//	return FALSE;
	
	//检测射线与模型的碰撞
	if (IntersectModelBound(orig, dir, pModel))
	{
		if (IntersectModelMesh(orig, dir, pModel, pFaceIndex, pSU, pSV, pDistance, pNormal))
			return TRUE;
	}
	
	return FALSE;
}

//射线和模型包围盒求交
BOOL CScene::IntersectModelBound(D3DXVECTOR3* orig, D3DXVECTOR3* dir, CModel* pModel)
{
	return (D3DXSphereBoundProbe(&(pModel->boundingSphere.Center), 
		pModel->boundingSphere.Radius, orig, dir));
}

//-----------------------------------------------------------------------------
// Name: CScene::IntersectTriangle(
// Desc: 给定起点，给定射线方向，看它是否与指定三角面相交
//-----------------------------------------------------------------------------
BOOL CScene::IntersectTriangle(D3DXVECTOR3* orig, D3DXVECTOR3* dir, 
							   D3DXVECTOR3* v1, D3DXVECTOR3* v2, 
							   D3DXVECTOR3* v3, //三个顶点
							   float *pU, float *pV, //UV坐标
							   float *pDist) //距离
{ 
	//下面是自己写的一种方法
	// Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = *v2 - *v1;
    D3DXVECTOR3 edge2 = *v3 - *v1;
	//先看看表面法线是否朝着射线方向,否则不检查
    D3DXVECTOR3 normal;
	D3DXVec3Cross( &normal, &edge1, &edge2 );
	D3DXVec3Normalize( &normal, &normal);
	FLOAT det = D3DXVec3Dot(&normal, dir);
	if (det >= 0.0f) 
	{
		return FALSE;
	}
	
	//距离
	float t,fTmp;
	D3DXVECTOR3  fTmpV;
	D3DXVec3Subtract(&fTmpV, v1, orig);
	t = D3DXVec3Dot(&fTmpV, &normal);
	fTmp = D3DXVec3Dot(dir, &normal);
	//
	if(fTmp != 0.0f)
		t /= fTmp;
	else
		return FALSE;
	
	//没有相交
	if( t < 0)
		return FALSE;
	
	//求出交点
	D3DXVECTOR3 ItePos;
	D3DXVec3Scale(&fTmpV, dir, t);
	D3DXVec3Add(&ItePos, orig, &fTmpV);
	
	//计算三个向量之间的夹角,判断点是不是在三角内部
	//三个向量	
	D3DXVECTOR3 m1, m2, m3;
	D3DXVec3Subtract(&m1, &ItePos, v1); 
	D3DXVec3Subtract(&m2, &ItePos, v2); 
	D3DXVec3Subtract(&m3, &ItePos, v3);
	
	//三向量单位化
	D3DXVec3Normalize(&m1, &m1);
	D3DXVec3Normalize(&m2, &m2);
	D3DXVec3Normalize(&m3, &m3);
	
	//三向量的点乘积
	float fTmpDot,fTotalAngle = 0.0f;
	fTmpDot = D3DXVec3Dot(&m1, &m2);
	if(fTmpDot < -1) fTmpDot = -1;
	if(fTmpDot > 1) fTmpDot = 1;
	fTotalAngle += (float)acos(fTmpDot);
	
	fTmpDot = D3DXVec3Dot(&m2, &m3);
	if(fTmpDot < -1) fTmpDot = -1;
	if(fTmpDot > 1) fTmpDot = 1;
	fTotalAngle += (float)acos(fTmpDot);
	
	fTmpDot = D3DXVec3Dot(&m3, &m1);
	if(fTmpDot < -1) fTmpDot = -1;
	if(fTmpDot > 1) fTmpDot = 1;
	fTotalAngle += (float)acos(fTmpDot);
	
	//如果三向量的夹角和为360度，则该点在三角形之内
	if(fabs(fTotalAngle - PI2) < 0.01f)
	{
		D3DXVECTOR3 tLength = ItePos - *orig;
		FLOAT fLength = D3DXVec3Length(&tLength);
		return TRUE;
	}
	else    
		return FALSE;
}

BOOL CScene::IntersectModelMesh(D3DXVECTOR3* orig, D3DXVECTOR3* dir, CModel* pModel, 
								INT* FaceIndex, FLOAT* pU, FLOAT* pV, 
								FLOAT* pDist, D3DXVECTOR3* vColNormal)
{	
	*pDist = 0;
	return TRUE;

	//if (!pModel->m_VBuffer )//|| !pModel->m_IBuffer)
	//	return FALSE;
	//
	//BOOL bResult = FALSE;
	//FLOAT tDist = 1000000.0f;
	//
	////如果该模型有索引物体，则该模型没有面表，而是索引的索引物体的面表，所以要用索引物体的面表
	//if (pModel->pCloneModel != NULL)
	//	pModel = pModel->pCloneModel;

	////逐个submesh测试
 //   for(UINT i= 0 ; i< pModel->subMesh.size(); i++)
	//{
	//	//逐个表面测试
	//	UINT startFIndex =0;
	//	while (startFIndex < pModel->subMesh[i].faceCnt)
	//	{
	//		//该表面的三个顶点为
	//		//顶点编号
	//		INT v0_index, v1_index, v2_index;
	//		if (pModel->subMesh[i].IBufferFormat == D3DFMT_INDEX16)
	//		{
	//			v0_index = ((unsigned short*)(pModel->subMesh[i].m_pIndices))[3*startFIndex];
	//			v1_index = ((unsigned short*)(pModel->subMesh[i].m_pIndices))[3*startFIndex+1];
	//			v2_index = ((unsigned short*)(pModel->subMesh[i].m_pIndices))[3*startFIndex+2];
	//		}
	//		else if (pModel->subMesh[i].IBufferFormat == D3DFMT_INDEX32)
	//		{
	//			v0_index = ((unsigned int*)(pModel->subMesh[i].m_pIndices))[3*startFIndex];
	//			v1_index = ((unsigned int*)(pModel->subMesh[i].m_pIndices))[3*startFIndex+1];
	//			v2_index = ((unsigned int*)(pModel->subMesh[i].m_pIndices))[3*startFIndex+2];
	//		}
	//		
	//		D3DXVECTOR3 Posa;
	//		D3DXVECTOR3 Posb;
	//		D3DXVECTOR3 Posc;
	//		
	//		//顶点
	//		switch(pModel->m_FVF)
	//		{
	//		case FVF_POSCOLOR:
	//			Posa = ((VERTEXDECL_POSCOLOR*)pModel->m_pVertices)[v0_index].position;
	//			Posb = ((VERTEXDECL_POSCOLOR*)pModel->m_pVertices)[v1_index].position;
	//			Posc = ((VERTEXDECL_POSCOLOR*)pModel->m_pVertices)[v2_index].position;
	//			break;
	//			
	//		case FVF_POSNORMALCOLOR:
	//			Posa = ((VERTEXDECL_POSNORMALCOLOR*)pModel->m_pVertices)[v0_index].position;
	//			Posb = ((VERTEXDECL_POSNORMALCOLOR*)pModel->m_pVertices)[v1_index].position;
	//			Posc = ((VERTEXDECL_POSNORMALCOLOR*)pModel->m_pVertices)[v2_index].position;
	//			break;
	//			
	//		case FVF_POSCOLORCOORD1:
	//			Posa = ((VERTEXDECL_POSCOLORCOORD1*)pModel->m_pVertices)[v0_index].position;
	//			Posb = ((VERTEXDECL_POSCOLORCOORD1*)pModel->m_pVertices)[v1_index].position;
	//			Posc = ((VERTEXDECL_POSCOLORCOORD1*)pModel->m_pVertices)[v2_index].position;
	//			break;
	//			
	//		case FVF_POSNORMALCOLORCOORD1:
	//			Posa = ((VERTEXDECL_POSNORMALCOLORCOORD1*)pModel->m_pVertices)[v0_index].position;
	//			Posb = ((VERTEXDECL_POSNORMALCOLORCOORD1*)pModel->m_pVertices)[v1_index].position;
	//			Posc = ((VERTEXDECL_POSNORMALCOLORCOORD1*)pModel->m_pVertices)[v2_index].position;
	//			break;
	//			
	//		case FVF_POSNORMALCOLORCOORD2:
	//			Posa = ((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[v0_index].position;
	//			Posb = ((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[v1_index].position;
	//			Posc = ((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[v2_index].position;
	//			break;
	//			
	//		case FVF_POSNORMALCOLORCOORD3:
	//			Posa = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[v0_index].position;
	//			Posb = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[v1_index].position;
	//			Posc = ((VERTEXDECL_POSNORMALCOLORCOORD3*)pModel->m_pVertices)[v2_index].position;
	//			break;
	//			
	//		case FVF_POSNORMALCOLORCOORD4:
	//			Posa = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[v0_index].position;
	//			Posb = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[v1_index].position;
	//			Posc = ((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[v2_index].position;
	//			break;
	//		case FVF_T8:
	//			Posa = ((T8MESHVERTEX*)pModel->m_pVertices)[v0_index].position;
	//			Posb = ((T8MESHVERTEX*)pModel->m_pVertices)[v1_index].position;
	//			Posc = ((T8MESHVERTEX*)pModel->m_pVertices)[v2_index].position;
	//			break;
	//			
	//		} // switch
	//		
	//		//变换矩阵
	//		//如果是动态模型
	//		/*	if ((pModel->kfrCnt > 0 && pModel->kfrPlay) || 
	//		pModel->isBillboard || 
	//		pModel->bMatrixChanged )
	//		{*/
	//		D3DXVec3TransformCoord(&Posa, &Posa, &(pModel->pNode->curTransform.worldMatrix));
	//		D3DXVec3TransformCoord(&Posb, &Posb, &(pModel->pNode->curTransform.worldMatrix));
	//		D3DXVec3TransformCoord(&Posc, &Posc, &(pModel->pNode->curTransform.worldMatrix));
	//		/*	}
	//		else
	//		{
	//		D3DXVec3TransformCoord(&Posa, &Posa, &(pModel->transform.ori_matrix));
	//		D3DXVec3TransformCoord(&Posb, &Posb, &(pModel->transform.ori_matrix));
	//		D3DXVec3TransformCoord(&Posc, &Posc, &(pModel->transform.ori_matrix));
	//	}*/
	//		
	//		D3DXVECTOR3 Normal;
	//		D3DXVec3Cross(&Normal, &(Posc - Posa), &(Posb - Posa));
	//		//如果表面的法线和射线方向同向，则不检测
	//		//dir= camAt - camFrom
	//		//if ( D3DXVec3Dot(dir, &Normal) >= 0.0f) 
	//		//{
	//		//	startFIndex++;
	//		//	continue;
	//		//}
	//		
	//		//D3D8.1方法
	//		if (D3DXIntersectTri(&Posa, &Posb, &Posc, orig, dir, pU, pV, pDist) )
	//		{
	//			if (*pDist < tDist)
	//			{
	//				*FaceIndex = startFIndex;
	//				*vColNormal = Normal; 
	//				tDist = *pDist;
	//			}
	//			bResult = TRUE;
	//		}
	//		
	//		startFIndex++;
	//	} // while
	//} // for
	//
	////取最小的pDist
	//*pDist = tDist;
	//
	//return bResult;
}
//-----------------------------------------------------------------------------
// 方法名: GetRayUnderPoint()
// 参  数: UINT x, UINT y
// 描  述: 得到光标下方的模型
//-----------------------------------------------------------------------------
//通过光标的位置的获得指定SwapChain的射线
// VOID CScene::GetRayUnderPoint(CSwapChain* pSwapChain, UINT x, UINT y, 
// 							  D3DXVECTOR3* pOrig, D3DXVECTOR3* pDir)
VOID CScene::GetRayUnderPoint(CViewport* pViewport, UINT x, UINT y, 
							  D3DXVECTOR3* pOrig, D3DXVECTOR3* pDir)
{
	if (!pViewport)
	{
		return;
	}
	RECT rect;
	pViewport->GetRect(rect);
	INT width  = rect.right - rect.left;
	INT height = rect.bottom - rect.top;
	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 v;
	switch (m_projMode)
	{
	case 0: // 透视
		v.x =  ( ( ( 2.0f * x) / width) - 1 ) / m_MatProj._11;
		v.y = -( ( ( 2.0f * y) / height) - 1 ) / m_MatProj._22;
		v.z =  1.0f;
		
		// Get the inverse view matrix
		D3DXMatrixInverse( &m_MatViewInverse, NULL, &m_MatView );
		
		// Transform the screen space pick ray into 3D space
		pDir->x  = v.x * m_MatViewInverse._11 + 
			v.y * m_MatViewInverse._21 + 
			v.z * m_MatViewInverse._31;
		pDir->y  = v.x * m_MatViewInverse._12 + 
			v.y * m_MatViewInverse._22 + 
			v.z * m_MatViewInverse._32;
		pDir->z  = v.x * m_MatViewInverse._13 + 
			v.y * m_MatViewInverse._23 + 
			v.z * m_MatViewInverse._33;
		
		pOrig->x = m_MatViewInverse._41;
		pOrig->y = m_MatViewInverse._42;
		pOrig->z = m_MatViewInverse._43;
		
		break;
	case 1:  // 轴侧
		v.x =  ( ( ( 2.0f * x) / width ) - 1 ) / m_MatProj._11;
		v.y = -( ( ( 2.0f * y) / height) - 1 ) / m_MatProj._22;
		v.z =  0.0f;
		
		// Get the inverse view matrix
		D3DXMatrixInverse( &m_MatViewInverse, NULL, &m_MatView);
		
		// Transform the screen space pick ray into 3D space
		pOrig->x  = m_MatViewInverse._41 + 
			v.x * m_MatViewInverse._11 + 
			v.y * m_MatViewInverse._21 + 
			v.z * m_MatViewInverse._31;
		
		pOrig->y  = m_MatViewInverse._42 + 
			v.x * m_MatViewInverse._12 + 
			v.y * m_MatViewInverse._22 + 
			v.z * m_MatViewInverse._32;
		
		pOrig->z  = m_MatViewInverse._43 + 
			v.x * m_MatViewInverse._13 + 
			v.y * m_MatViewInverse._23 + 
			v.z * m_MatViewInverse._33;
		
		pDir->x  = m_MatViewInverse._31;
		pDir->y  = m_MatViewInverse._32;
		pDir->z  = m_MatViewInverse._33;
		break;
	}
}

//-----------------------------------------------------------------------------
// 方法名: GetModelUnderCursor()
// 参  数: UINT x, UINT y
// 描  述: 得到光标下方的模型
//-----------------------------------------------------------------------------
// CModel* CScene::GetModelUnderCursor(CSwapChain* pSwapChain, UINT x, UINT y)
CModel* CScene::GetModelUnderCursor(CViewport* pViewport, UINT x, UINT y)
{
	// Compute the vector of the pick ray in screen space
	GetRayUnderPoint(pViewport, x, y, &m_rayOrig, &m_pickRayDir);
	
	//视点到与模型交点的距离
	FLOAT fDistance = 100000.0f;
	INT uColFaceIndex = -1;
	FLOAT pU = 0.0f;
	FLOAT pV = 0.0f;
	FLOAT pDist = 10000000.0f;
	D3DXVECTOR3 colNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	m_CursorModel = NULL;
	CModel* pModel;
	UINT modelCnt = model.size();
	std::vector<CModel*>::iterator ppModel = model.begin();
	for (UINT i = 0; i< modelCnt; i++)
	{
		pModel = *(ppModel + i);
		//忽略碰撞模型
		if (pModel->isCollision || pModel->visible == FALSE || pModel->isGround || pModel->culled) 
			continue;
		
		if (IntersectModelBound(&m_rayOrig, &m_pickRayDir, pModel)) 
		{
			if (IntersectModelMesh(&m_rayOrig, &m_pickRayDir, pModel, 
				&uColFaceIndex, &pU, &pV, &pDist, &colNormal))
			{
				if (fDistance > pDist) 
				{
					fDistance = pDist;
					m_CursorModel = pModel;
				}
			}
		}
	}
	
	return m_CursorModel;
}


//得到光标下方的MouseHint模型
CModel* CScene::GetMouseHintModelUnderCursor(CSwapChain* pSwapChain, UINT x, UINT y)
{
	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * x) / m_d3dpp.BackBufferWidth ) - 1 ) / m_MatProj._11;
	v.y = -( ( ( 2.0f * y) / m_d3dpp.BackBufferHeight) - 1 ) / m_MatProj._22;
	v.z =  1.0f;
	
	// Get the inverse view matrix
	D3DXMatrixInverse( &m_MatViewInverse, NULL, &m_MatView );
	
	// Transform the screen space pick ray into 3D space
	m_pickRayDir.x  = v.x*m_MatViewInverse._11 + v.y*m_MatViewInverse._21 + v.z*m_MatViewInverse._31;
	m_pickRayDir.y  = v.x*m_MatViewInverse._12 + v.y*m_MatViewInverse._22 + v.z*m_MatViewInverse._32;
	m_pickRayDir.z  = v.x*m_MatViewInverse._13 + v.y*m_MatViewInverse._23 + v.z*m_MatViewInverse._33;
	m_rayOrig.x = m_MatViewInverse._41;
	m_rayOrig.y = m_MatViewInverse._42;
	m_rayOrig.z = m_MatViewInverse._43;
	
	//视点到与模型交点的距离
	FLOAT fDistance = 100000.0f;
	INT uColFaceIndex = -1;
	FLOAT pU = 0.0f;
	FLOAT pV = 0.0f;
	FLOAT pDist = 10000000.0f;
	D3DXVECTOR3 colNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	m_CursorModel = NULL;
	CModel* pModel;
	for (UINT i = 0; i< model.size(); i++)
	{
		pModel = *(model.begin() + i);
		//忽略碰撞模型
		if (pModel->isCollision || pModel->visible == FALSE || pModel->isGround || pModel->culled) 
			continue;
		//忽略没有设置点击响应的模型
		if (pModel->mouseInOutable == FALSE) continue;
		
		if (IntersectModelBound(&m_rayOrig, &m_pickRayDir, pModel)) 
		{
			if (IntersectModelMesh(&m_rayOrig, &m_pickRayDir, pModel, 
				&uColFaceIndex, &pU, &pV, &pDist, &colNormal))
			{
				if (fDistance > pDist) 
				{
					fDistance = pDist;
					m_CursorModel = pModel;
				}
			}
		}
	}
	
	return m_CursorModel;
}

//-----------------------------------------------------------------------------
// 方法名: SmoothModelNormal()
// 参  数: CModel* pModel
// 描  述: 重新计算顶点法线
//-----------------------------------------------------------------------------
HRESULT CScene::SmoothModelNormal(CModel* pModel)
{
	//把位置相同的点的法线计算算术平均和
	//------------------------------------------------------------
	//为了减少运算量，采用的算法思路如下:
	//建立2个与顶点数目相同的数组BOOL HasFind[],CurrentFind[],
	//HasFind[]用于记录已经Smooth法线的顶点编号，
	//CurrentFind[]用于记录当前循环与被比较顶点位置相同的顶点编号
	//------------------------------------------------------------
	BOOL* HasFind = new BOOL[pModel->vertexCnt]; 
	BOOL* CurrentFind = new BOOL[pModel->vertexCnt]; 
	ZeroMemory(HasFind, sizeof(BOOL) * pModel->vertexCnt);
	
	for (UINT i = 0; i< pModel->vertexCnt; i++)
	{
		//如果该顶点已经被Smooth，则跳过
		if (HasFind[i]) continue;
		
		ZeroMemory(CurrentFind, sizeof(BOOL) * pModel->vertexCnt);
		CurrentFind[i] = TRUE;
		D3DXVECTOR3 pos0 = pModel->vertex[i].position;
		D3DXVECTOR3 smoothNormal = pModel->vertex[i].normal;
		//找出与pModel->vertex[i]位置相同的点编号
		for (UINT j = 0; j< pModel->vertexCnt; j++)
		{
			//如果该顶点已经被Smooth，或者是当前的被比较顶点，则跳过
			if (HasFind[j] || CurrentFind[j]) continue;
			
			if (pos0 == pModel->vertex[j].position)
			{
				CurrentFind[j] = TRUE;
				smoothNormal = (smoothNormal + pModel->vertex[j].normal)/2.0f;
				D3DXVec3Normalize(&smoothNormal, &smoothNormal);
			}
		}
		//填入新位置
		for (UINT k = 0; k< pModel->vertexCnt; k++)
		{
			if (CurrentFind[k])
			{
				pModel->vertex[k].normal = smoothNormal;
				HasFind[k] = TRUE;
			}
		}
		
	}
	SAFE_DELETE_ARRAY(HasFind);
	SAFE_DELETE_ARRAY(CurrentFind);
	
	//重写模型顶点缓冲
	//锁定内存
	if( FAILED(pModel->m_VBuffer->Lock(0, pModel->vertexCnt * pModel->m_FVFSize,
		(VOID**)&(pModel->m_pVertices), D3DLOCK_DISCARD) ))
	{
		return E_FAIL;
	}
	
	switch (pModel->m_FVF)
	{
	case FVF_POSCOLOR:
		break;
	case FVF_POSNORMAL:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
			((VERTEXDECL_POSNORMAL*)pModel->m_pVertices)[i].normal = pModel->vertex[i].normal;
		break;
	case FVF_POSCOLORCOORD1:
		break;
	case FVF_POSNORMALCOLORCOORD1:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
			((VERTEXDECL_POSNORMALCOLORCOORD1*)pModel->m_pVertices)[i].normal = pModel->vertex[i].normal;
		break;
	case FVF_POSNORMALCOLORCOORD2:
		break;
	case FVF_POSNORMALCOLORCOORD3:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
			((VERTEXDECL_POSNORMALCOLORCOORD2*)pModel->m_pVertices)[i].normal = pModel->vertex[i].normal;
		break;
	case FVF_POSNORMALCOLORCOORD4:
		//逐个顶点填充数据
		for (i=0; i< pModel->vertexCnt; i++)
			((VERTEXDECL_POSNORMALCOLORCOORD4*)pModel->m_pVertices)[i].normal = pModel->vertex[i].normal;
		break;
	}
	//Unlock顶点缓冲
	pModel->m_VBuffer->Unlock();
	
	return S_OK;
	
}

//-----------------------------------------------------------------------------
// 方法名: SetModelBillboard()
// 参  数: 
// 描  述: 设置模型Billboard
//-----------------------------------------------------------------------------
VOID CScene::SetModelBillboard(CModel* pModel)
{
	if (pModel && m_pCurCamera)
	{
		pModel->m_pNode->lookAt(m_pCurCamera->GetPosition(VTS_WORLD), VTS_WORLD, true, m_pCurCamera->GetUp(VTS_WORLD));
		//得到模型的自身Y方向, 默认为y方向正对相机
		//计算模型应该正对的方向
		//pModel->pointAtOrient.PointAtPos = m_pCurCamera->GetPosition(VTS_WORLD);
		//D3DXVECTOR3 v3LookAt = pModel->pointAtOrient.PointAtPos - pModel->m_pNode->getPosition(VTS_WORLD);//pModel->pNode->worldPosition;
		//v3LookAt.y = 0;
		//D3DXVec3Normalize(&v3LookAt, &v3LookAt);

		//// 绕世界坐标系的y轴旋转
		//D3DXVECTOR3 refRotAxis = D3DXVECTOR3(0, 1, 0);//pModel->billboardDirectionAxis;
		//D3DXVECTOR3 refYawAxis = D3DXVECTOR3(0, 1, 0);
		//pModel->m_pNode->setDirection(v3LookAt, VTS_WORLD, true, refYawAxis);
	}

}

//-----------------------------------------------------------------------------
// 方法名: SetModelCollision()
// 参  数: D3DXVECTOR3* pOri, D3DXVECTOR3* pRay, CModel* pModel, D3DXVECTOR3* pNormal, FLOAT distance
// 描  述: 如果有碰撞，则返回True, 如果没有则返回False, 关于碰撞的参数都返回到参数中
//-----------------------------------------------------------------------------
BOOL CScene::DetectCollision(D3DXVECTOR3* pOri, D3DXVECTOR3* pRay, 
							 CModel* pModel, D3DXVECTOR3* pNormal, 
							 FLOAT* distance)
{
	UINT modelCnt = model.size();
	std::vector<CModel*>::iterator ppModel = model.begin();
	
	CModel* _pModel = NULL;
	INT faceIndex  = -1;
	FLOAT SU = 0.0f;
	FLOAT SV = 0.0f;
	FLOAT tDistance = 1000000.0f;
	D3DXVECTOR3 tNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BOOL bResult = FALSE;
	
	for (UINT i =0 ; i< modelCnt; i++)
	{
		_pModel = *(ppModel + i);
		if (_pModel->isCollision == FALSE) continue;
		
		//先看pOri原点是不是在模型包围球外围一定的距离范围内
		FLOAT rDistance = D3DXVec3Length(&(m_pCurCamera->GetPosition(VTS_WORLD) - _pModel->boundingSphere.Center));
		if (rDistance > _pModel->boundingSphere.Radius + _pModel->colDetectDistance)
			continue;
		
		//检测射线与模型的碰撞
		if (IntersectModelBound(pOri, pRay, _pModel))
		{
			if (IntersectModelMesh(pOri, pRay, _pModel, &faceIndex, &SU, &SV, &tDistance, &tNormal))
			{
				if (_pModel->colDetectDistance > tDistance)
				{
					*distance = tDistance;
					*pNormal = tNormal;
					pModel = _pModel;
					bResult = TRUE;
					return TRUE;
				}
			}
		}
	}
	
	return bResult;
}

//-----------------------------------------------------------------------------
// 方法名: DetectGround()
// 参  数: D3DXVECTOR3* pOri, D3DXVECTOR3* pRay, 
//		   CModel* pModel, D3DXVECTOR3* pNormal, 
//         FLOAT* distance
// 描  述: 获得Walker下方地面的碰撞模型和法线
//-----------------------------------------------------------------------------
BOOL CScene::DetectGround(D3DXVECTOR3* pOri, D3DXVECTOR3* pRay, 
						  DWORD* pModelHandle, D3DXVECTOR3* pNormal, 
						  FLOAT* distance)
{
	UINT modelCnt = model.size();
	std::vector<CModel*>::iterator ppModel = model.begin();
	
	CModel* _pModel = NULL;
	INT faceIndex = -1;
	FLOAT SU = 0.0f;
	FLOAT SV = 0.0f;
	*distance = 1000000.0f;
	FLOAT tDistance;
	D3DXVECTOR3 tNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	BOOL bResult = FALSE;
	
	for (UINT i =0 ; i< modelCnt; i++)
	{
		_pModel = *(ppModel + i);
		if (_pModel->isGround == FALSE) continue;
		
		//检测射线与模型的碰撞
		if (IntersectModelBound(pOri, pRay, _pModel))
		{
			if (IntersectModelMesh(pOri, pRay, _pModel, &faceIndex, &SU, &SV, &tDistance, &tNormal))
			{
				if (*distance > tDistance)
				{
					*distance = tDistance;
					*pNormal = tNormal;
					*pModelHandle = (DWORD)_pModel;
					bResult = TRUE;
				}
			}
		}
	}
	
	return bResult;
}

VOID CScene::ClearImageList()
{
	//释放 m_vImage 资源
	UINT imageCnt = m_vImage.size();
	std::vector<CImage*>::iterator ppImage = m_vImage.begin();
	for (UINT i = 0; i < imageCnt; i++)
	{
		CImage* pImage = *(ppImage + i);
		SAFE_RELEASE(pImage);
		SAFE_DELETE(pImage);
	}
	m_vImage.clear();
}

// 获取场景中使用的图片名称列表
void CScene::getImageNameList(bool includePath, vector<std::string> &vNameList)
{
	vNameList.clear();

	std::vector<CMaterial*> vTotalMaterial;
	vTotalMaterial.assign(m_vShader.begin(), m_vShader.end());
	vTotalMaterial.insert(vTotalMaterial.end(), m_v2DMaterial.begin(), m_v2DMaterial.end());

	UINT matCount = vTotalMaterial.size();
	for (UINT iMat = 0; iMat < matCount; ++iMat)
	{
		CMaterial* pMaterial = vTotalMaterial[iMat];
		if (pMaterial)
		{
			UINT layerCount = pMaterial->getTexureLayerCount();
			for (UINT iLayer = 0; iLayer < layerCount; ++iLayer)
			{
				CTextureLayer *pLayer = pMaterial->getTextureLayerByIndex(iLayer);
				if (pLayer && pLayer->pTexture)
				{
					// 如果是动态贴图或者cubemap，要处理多张贴图
					// ...

					std::string strName = pLayer->pTexture->m_sFilePathandName;
					if (!includePath)
					{
						strName = _GetPureFileNameStr(pLayer->pTexture->m_sFilePathandName);
					}
					if (find(vNameList.begin(), vNameList.end(), strName) == vNameList.end())
					{
						vNameList.push_back(strName);
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
// 方法名: Reset()
// 参  数: 
// 描  述: 清空当前场景，并释放场景中所有对象的内存，保留D3D对象和设备对象
//		   该方法用于NewScene();
//-----------------------------------------------------------------------------
VOID CScene::Reset()
{	
	/*//获取当前相继的状态
	defaultCam.ori_from = m_pCurCamera->ori_from;
	defaultCam.ori_at = m_pCurCamera->ori_at;
	defaultCam.m_from = m_pCurCamera->m_from;
	defaultCam.m_at = m_pCurCamera->m_at;	
	defaultCam.nearClip = m_pCurCamera->nearClip;
	defaultCam.farClip = m_pCurCamera->farClip;
	defaultCam.m_fov = m_pCurCamera->m_fov;
	defaultCam.m_roll = m_pCurCamera->m_roll;
	defaultCam.m_up = m_pCurCamera->m_up;
	m_pCurCamera = &defaultCam;*/
	// InitDefaultCamera();

	// 销毁所有动画
	m_pRoot->GetAnimManager().destroyAllAnimation();
	
	//释放音频列表
	
	//释放m_vWalker资源
	INT walkerCnt = m_vWalker.size();
	std::vector<IController*>::iterator ppWalker = m_vWalker.begin();
	for (int i = 0; i< walkerCnt; i++)
	{
		CWalker* pWalker = (CWalker*)(*(ppWalker + i));
		SAFE_RELEASE(pWalker);
		SAFE_DELETE(pWalker);
	}// 	INT audioCnt = m_vAudio.size();
// 	CAudioPlayer** ppAudio = (CAudioPlayer**)m_vAudio.begin();
// 	for (INT i = 0; i< audioCnt; i++)
// 	{
// 		CAudioPlayer* pAudio = *(ppAudio + i);
// 		
// 		SAFE_RELEASE(pAudio);
// 		SAFE_DELETE(pAudio);
// 	}
// 	m_vAudio.clear();

	m_vWalker.clear();
	
	//释放editor资源
	INT editorCnt = m_vEditor.size();
	std::vector<IController*>::iterator ppEditor = m_vEditor.begin();
	for (i = 0; i< editorCnt; i++)
	{
		CEditor* pEditor = (CEditor*)(*(ppEditor + i));
		SAFE_RELEASE(pEditor);
		SAFE_DELETE(pEditor);
	}
	m_vEditor.clear();
	
	//释放flyer资源
	INT flyerCnt = m_vFlyer.size();
	std::vector<IController*>::iterator ppFlyer = m_vFlyer.begin();
	for (i = 0; i< flyerCnt; i++)
	{
		CFlyer* pFlyer = (CFlyer*)(*(ppFlyer + i));
		SAFE_RELEASE(pFlyer);
		SAFE_DELETE(pFlyer);
	}
	m_vFlyer.clear();
	
	//释放 m_vText 资源
	INT textCnt = m_vText.size();
	std::vector<CText*>::iterator ppText = m_vText.begin();
	for (i = 0; i< textCnt; i++)
	{
		CText* pText = *(ppText + i);
		SAFE_RELEASE(pText);
		SAFE_DELETE(pText);
	}
	m_vText.clear();
	
	//Invalidate the m_vFont
	std::vector<CFont*>::iterator ppFont = m_vFont.begin();
	INT fontSize = m_vFont.size();
	for (i = 0; i< fontSize; i++)
	{
		CFont* pFont = *(ppFont + i);
		SAFE_RELEASE(pFont);
		SAFE_DELETE(pFont);
	}
	m_vFont.clear();

	//释放粒子系统对象
// 	int particleCnt = m_Particles.size();
// 	CParticleEmitter** ppParticle = m_Particles.begin();
// 	for (i = 0; i< particleCnt; i++)
// 	{
// 		CParticleEmitter* pParticle = *(ppParticle + i);
// 		pParticle->InvalidateDeviceObjects();
// 	}
// 	m_Particles.clear();

	//删除电影贴图
// 	INT movieTexCnt = movieTexture.size();
// 	CMovieTexture** ppMovieTex = (CMovieTexture**)movieTexture.begin();
// 	for (i = 0; i< movieTexCnt; i++)
// 	{
// 		CMovieTexture* pMovieTex = *(ppMovieTex + i);
// 		
// 		SAFE_RELEASE(pMovieTex);
// 		SAFE_DELETE(pMovieTex);
// 	}
// 	movieTexture.clear();
	
	//删除动态贴图
	INT dyTexCnt = dynamicTexture.size();
	std::vector<CDynamicTex*>::iterator ppDyTex = dynamicTexture.begin();
	for (i = 0; i< dyTexCnt; i++)
	{
		CDynamicTex* pDyTex = *(ppDyTex + i);
		
		SAFE_RELEASE(pDyTex);
		SAFE_DELETE(pDyTex);
	}
	dynamicTexture.clear();

	//删除凹凸贴图
// 	INT bumpTexCnt = bumpTexture.size();
// 	CBumpMap** ppBpTex = bumpTexture.begin();
// 	for (i = 0; i< bumpTexCnt; i++)
// 	{
// 		CBumpMap* pBpTex = *(ppBpTex + i);
// 		
// 		SAFE_RELEASE(pBpTex);
// 		SAFE_DELETE(pBpTex);
// 	}
// 	bumpTexture.clear();
	
	//释放CubeMap
	INT cubeMapCnt = cubeMap.size();
	std::vector<CCubeMap*>::iterator ppCubeMap = cubeMap.begin();
	for (i = 0; i< cubeMapCnt; i++)
	{
		CCubeMap* pCubeMap = *(ppCubeMap + i);
		SAFE_RELEASE(pCubeMap);
		SAFE_DELETE(pCubeMap);
	}
	cubeMap.clear();
	
	//释放 m_vCamera 资源
	INT camCnt = m_vCamera.size();
	std::vector<CCamera*>::iterator ppCam = m_vCamera.begin();
	for (i = 0; i< camCnt; i++)
	{
		CCamera* pCamera = *(ppCam + i);
		
		//删除节点
		//SAFE_RELEASE(pCamera->m_pNode);
		//SAFE_DELETE(pCamera->m_pNode);

		//  设置视口的相机无效
		vector<DWORD> vViewport;
		pCamera->getViewport(vViewport);
		vector<DWORD>::iterator it = vViewport.begin();
		while (it != vViewport.end())
		{
			CViewport *pViewport = (CViewport*)(*it);
			if (pViewport)
			{
				pViewport->SetCamera(NULL);
			}
			++it;
		}
		//删除对象
		SAFE_RELEASE(pCamera);
		SAFE_DELETE(pCamera);
	}
	m_vCamera.clear();
	
	//Light
	std::vector<CLight*>::iterator ppLight = m_vLight.begin();
	int lightcnt = m_vLight.size();
	for (i = 0; i< lightcnt; i++)
	{
		CLight* pLight = *(ppLight + i);

		//删除节点
		//SAFE_RELEASE(pLight->pNode);
		//SAFE_DELETE(pLight->pNode);

		//删除对象
		SAFE_RELEASE(pLight);
		SAFE_DELETE(pLight);
	}
	m_vLight.clear();
	
	//释放 model 资源
	INT modelCnt = model.size();
	std::vector<CModel*>::iterator ppModel = model.begin();
	for (i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(ppModel + i);

		//删除节点
		//SAFE_RELEASE(pModel->pNode);
		//SAFE_DELETE(pModel->pNode);

		//删除对象
		if (pModel->pCloneModel == NULL)
			SAFE_RELEASE(pModel);
		SAFE_DELETE(pModel);
	}
	model.clear();

	
	//释放modelGroup资源
	for (i = 0; i< (int)modelGroup.size(); i++)
	{
		CModelGroup* pModelGroup = *(modelGroup.begin() + i);
		SAFE_RELEASE(pModelGroup);
		SAFE_DELETE(pModelGroup);
	}
	modelGroup.clear();
	
	//释放 shader 资源
	INT shaderCnt = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = m_vShader.begin();
	for (i = 0; i< shaderCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		m_pRoot->GetMaterialManager().removeResource(pMtrl->m_sName);
	}
	m_vShader.clear();
	
	//释放 m_vOverlay 资源
	INT overlayCnt = m_vOverlay.size();
	std::vector<COverlay*>::iterator ppOverlay = m_vOverlay.begin();
	for (i = 0; i< overlayCnt; i++)
	{
		COverlay* pOverlay = *(ppOverlay + i);
		SAFE_DELETE(pOverlay);
	}
	m_vOverlay.clear();
	
	//释放 m_vButton 资源
	INT buttonCnt = m_vButton.size();
	std::vector<CButton*>::iterator ppButton = m_vButton.begin();
	for (i = 0; i< buttonCnt; i++)
	{
		CButton* pButton = *(ppButton + i);
		SAFE_DELETE(pButton);
	}
	m_vButton.clear();
	
	//释放 m_vNavigater 资源
	INT navCnt = m_vNavigater.size();
	std::vector<CNavigator*>::iterator ppNav = m_vNavigater.begin();
	for (i = 0; i< navCnt; i++)
	{
		CNavigator* pNavigater = *(ppNav + i);
		SAFE_DELETE(pNavigater);
	}
	m_vNavigater.clear();
	
	/*	//释放 m_vImage 资源
	INT imageCnt = m_vImage.size();
	CImage** ppImage = m_vImage.begin();
	for (i = 0; i< imageCnt; i++)
	{
	CImage* pImage = *(ppImage + i);
	SAFE_RELEASE(pImage);
	SAFE_DELETE(pImage);
	}
	m_vImage.clear();*/
    ClearImageList();
	
	/*
	//释放SectionMap
	for (i = 0; i< sectionMap.size(); i++)
	{
		CSectionMap* pSectionMap = *(sectionMap.begin() + i);
		SAFE_RELEASE(pSectionMap);
		SAFE_DELETE(pSectionMap);
	}
	sectionMap.clear();
	*/
	
	//释放 texture 资源
	INT texCnt = m_vTexture.size();
	std::vector<CTexture*>::iterator ppTex = m_vTexture.begin();
	for (i = 0; i < texCnt; i++)
	{
		CTexture* pTex = *(ppTex + i);
		m_pRoot->GetTextureManager().removeResource(pTex);  // 释放资源
	}
	m_vTexture.clear();

	// 释放材质
	INT matCount = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMat = m_vShader.begin();
	for (i = 0; i < matCount; ++i)
	{
		CMaterial *pMaterail = *(ppMat + i);
		m_pRoot->GetMaterialManager().removeResource(pMaterail); // 释放材质
	}
	m_vShader.clear();

    //Edit by Baojiahui 2007/9/27
	//释放modelTrigger
	INT trigModelCnt =triggerModel.size();
	std::vector<TriggerCallBack*>::iterator ppTrigModel=triggerModel.begin();
	for (i = 0; i< trigModelCnt; i++)
	{
		TriggerCallBack* pTrigModel=*(ppTrigModel+i);
		if(pTrigModel->callBackFunc)
		{
			SAFE_DELETE_ARRAY(pTrigModel->callBackFunc);
		}
		SAFE_DELETE_ARRAY(pTrigModel);
	}
    triggerModel.clear();

	//RTF Map 
//     INT refTexCnt = rtfTexture.size();
// 	CRTFMap** ppRTFMap = rtfTexture.begin();
//     for (i = 0; i< refTexCnt; i++)
// 	{
// 		CRTFMap* pRTFMap =*(ppRTFMap + i);
// 		SAFE_RELEASE(pRTFMap);
// 	}

	//释放RealWater的D3D对象
// 	int realWaterCnt = realWater.size();
// 	CRealWater** ppRealWater = realWater.begin();
// 	for (i = 0; i< realWaterCnt; i++)
// 	{
// 		CRealWater* pRealWater = *(ppRealWater + i);
// 		pRealWater->Release();
// 	}
// 	realWater.clear();

	//删除动态模糊对象
	SAFE_RELEASE(m_pFSMotionBlur);
	SAFE_DELETE(m_pFSMotionBlur);
	
	//删除FSGlow对象
	SAFE_RELEASE(m_pFSGlow);
	SAFE_DELETE(m_pFSGlow);
		
	//删除ColorEdit对象
// 	SAFE_RELEASE(m_pColorEdit);
// 	SAFE_DELETE(m_pColorEdit);
	//删除镜头光斑对象
	SAFE_RELEASE(m_pLensFlare);
	SAFE_DELETE(m_pLensFlare);
	
	//清除StartLogo
	SAFE_DELETE(m_pStartLogo);
	
	//清除 UnitedLogo
	SAFE_DELETE(m_pUnitedLogo);
	//清除 UnitedLogo
	SAFE_DELETE(m_pLoadingBg);
    //清除 UnitedLogo
	SAFE_DELETE(m_pLoadingBar);

	//清楚缓冲
    SAFE_RELEASE_D3D(m_pBackBuffer);
	SAFE_RELEASE_D3D(m_pZBuffer);
	
	
	if (m_pAxis)
	{
		SAFE_RELEASE(m_pAxis);
	SAFE_DELETE(m_pAxis);
	}

	m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	
	//Invokes the resource manager to free memory. 
	//激活D3D资源管理器释放所有显存
	// m_pd3dDevice->ResourceManagerDiscardBytes(0);

	// 删除节点
	if (m_pRootNode)
	{
		SAFE_RELEASE(m_pRootNode);
		SAFE_DELETE(m_pRootNode);
	}

	//初始化所有变量
	InitVar();
}

//-----------------------------------------------------------------------------
// 方法名: Release()
// 参  数: 
// 描  述: 释放内存
//-----------------------------------------------------------------------------
VOID CScene::Release()
{		
	//释放场景中的资源
	Reset();
	
	//release and delete d3d device
	//DeleteD3DDevice();
	
	//删除字体默认对象
	//SAFE_RELEASE(pDefaultFont);
	//SAFE_DELETE(pDefaultFont);

}

//清理场景中的贴图资源,没有用到的删除
VOID  CScene::CleanTextureSource()
{
	
}

////Add by Baojiahui 4/12/2007---------------------------------------------------------------------------
//场景在编辑状态下使用的成员函数
//-----------------------------------------------------------------------------
// 方法名: MoveModel()
// 参  数: 
// 描  述: 通过鼠标移动物体
//-----------------------------------------------------------------------------
VOID CScene::MoveModel()
{/*
	D3DXVECTOR3* rayVec;
    D3DXVECTOR3* refRayVec;
	D3DXVECTOR3* curEditAxisOriPos;
	
	D3DXVECTOR3 curEditAxisDesPos;
	D3DXVECTOR3 curMouseWorldPos;
    D3DXVECTOR3 curModelDesPos;
	D3DXVECTOR3 deltaVec;
	
	rayVec=editAxis->GetyAxisRayDir();
	refRayVec=editAxis->GetxAxisRayDir();
	
	switch(sceneEditMode){
	case 0:
		break;
	case 1:
		//x轴
		if(m_ModelEdit==FALSE){
			
			rayVec=editAxis->GetyAxisRayDir();
			refRayVec=editAxis->GetxAxisRayDir();
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlaneAndRay(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,refRayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else{
			rayVec=editAxis->GetyAxisRayDir();
			refRayVec=editAxis->GetxAxisRayDir();
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlaneAndRay(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,refRayVec,&curMouseWorldPos);
		}
		break;
	case 2:
		//y轴
		if(m_ModelEdit==FALSE){
			rayVec=editAxis->GetzAxisRayDir();
			refRayVec=editAxis->GetyAxisRayDir();
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlaneAndRay(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,refRayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else{
			rayVec=editAxis->GetzAxisRayDir();
			refRayVec=editAxis->GetyAxisRayDir();
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlaneAndRay(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,refRayVec,&curMouseWorldPos);
		}
		break;
	case 3:
		//z轴
		if(m_ModelEdit==FALSE){
			rayVec=editAxis->GetxAxisRayDir();
			refRayVec=editAxis->GetzAxisRayDir();
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlaneAndRay(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,refRayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else{
			rayVec=editAxis->GetxAxisRayDir();
			refRayVec=editAxis->GetzAxisRayDir();
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlaneAndRay(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,refRayVec,&curMouseWorldPos);
		}
		break;
	case 4:
		//xy平面
		if(m_ModelEdit==FALSE){
			rayVec=editAxis->GetzAxisRayDir();
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else{
			rayVec=editAxis->GetzAxisRayDir();
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
		}
		break;
	case 5:
		//yz平面
		if(m_ModelEdit==FALSE){
			rayVec=editAxis->GetxAxisRayDir();
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else{
			rayVec=editAxis->GetxAxisRayDir();
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
		}
		break;
	case 6:
		//zx平面
		if(m_ModelEdit==FALSE){
			rayVec=editAxis->GetyAxisRayDir();
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else{
			rayVec=editAxis->GetyAxisRayDir();
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
		}
		break;
	}
	
	//计算移动差值	
	deltaVec=curMouseWorldPos-mouseWorldPos;
	//设置模型位置
	INT modelCnt = m_SelectedModel.size();
	CModel** ppCSetModel = m_SelectedModel.begin();
	for (INT i = 0; i< modelCnt; i++)
	{
		CModel* pCSetModel = *(ppCSetModel + i);
		//计算模型的目标位置
		curModelDesPos=pCSetModel->pNode->worldPosition+deltaVec;
		//设置位置
		pCSetModel->pNode->setPosition(&curModelDesPos);
		pCSetModel->update();
		pCSetModel->pRBBox->SetWorldMatrix(&(pCSetModel->pNode->curTransform.worldMatrix));
	} 
	curEditAxisDesPos=*curEditAxisOriPos+deltaVec;
	//设置坐标轴位置
	editAxis->SetPosition(&curEditAxisDesPos);	
	//更新鼠标的三维位置信息
	mouseWorldPos=curMouseWorldPos;
	*/
}
//Add by Baojiahui 5/12/2007---------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: RotateModel()
// 参  数: 
// 描  述: 移动物体
//-----------------------------------------------------------------------------
VOID CScene::RotateModel()
{
	
	D3DXVECTOR3 rayVec;
	D3DXVECTOR3* oriVecter = NULL;
	D3DXVECTOR3* curEditAxisOriPos = NULL;
	
    
	D3DXVECTOR3 curMouseWorldPos = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 curModelDesPos = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 oriVec = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 desVec = D3DXVECTOR3(0, 0, 0);
	
	FLOAT rotateAngle = 0;
	
 	m_pAxis->GetxAxisRayDir(rayVec);
	
    switch(sceneEditMode)
	{
		//没有选中
	case 0:
		return;
		//绕x轴旋转
	/*case 7:
		if(m_ModelEdit==FALSE)
		{   
			rayVec=editAxis->GetxAxisRayDir();
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else
		{
			rayVec=editAxis->GetxAxisRayDir();
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
		}
		break;
		//绕y轴旋转
	case 8:
		if(m_ModelEdit==FALSE){   
			rayVec=editAxis->GetyAxisRayDir();
			//*rayVec=-(*rayVec);
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else{
			rayVec=editAxis->GetyAxisRayDir();
			//*rayVec=-(*rayVec);
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
		}
		break;
		//绕z轴旋转
	case 9:
		if(m_ModelEdit==FALSE){   
			rayVec=editAxis->GetzAxisRayDir();
			//获取坐标轴的位置
			curEditAxisOriPos=editAxis->GetAxisPos();
			//获取鼠标位置下垂直于当前相机，以及经过物体中心位置的平面上的点坐标
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
			mouseWorldPos=curMouseWorldPos;
			m_ModelEdit=TRUE;
		}else{
			rayVec=editAxis->GetzAxisRayDir();
			curEditAxisOriPos=editAxis->GetAxisPos();
			GetCoordScreenToWorldFromPlane(InputDevice.mouseInfo.X,InputDevice.mouseInfo.Y,curEditAxisOriPos,rayVec,&curMouseWorldPos);
		}
		break;
	*/	
	default:
		return;
	}
	
	//获取现在鼠标位置到转动坐标轴原点的一条射线
	desVec=curMouseWorldPos-(*curEditAxisOriPos);
	
	/*
	if(pLButton==FALSE && InputDevice.mouseInfo.LButton==TRUE)
	{
		switch(sceneEditMode)
		{
		case 7:
			
			oriVecter=editAxis->GetzAxisRayDir();
			*oriVecter=-(*oriVecter);			
			break;
		case 8:
			oriVecter=editAxis->GetxAxisRayDir();  
			break;
		case 9:
			oriVecter=editAxis->GetxAxisRayDir();	
			break;	  
		}
		
		pLButton =TRUE;	
	}*/
	
	//获取上一帧鼠标位置到转动坐标轴原点的一条射线
	oriVec=mouseWorldPos-(*curEditAxisOriPos);
	//计算两条射线间的转角
	rotateAngle=-(FLOAT)GetRotateAngle(&oriVec,&desVec,&rayVec);
	//设置模型旋转的角度
	/*	 
	switch(rotateAxis->selectAxisStatus)
	{
	case 1:
	case 3:
	rotateAxis->rotateAngle=(INT)((rotateAxis->fAngle/3.14159f)*180);
	break;
	case 2:
	rotateAxis->rotateAngle=(INT)(-(rotateAxis->fAngle/3.14159f)*180);
	break;
	 }*/
	
	rotateAngle =rotateAngle*180/3.14159f;
	if(abs(rotateAngle)>=1.0f){
		//设置模型位置
		INT modelCnt = m_SelectedModel.size();
		std::vector<CModel*>::iterator ppCSetModel = m_SelectedModel.begin();
		for (INT i = 0; i< modelCnt; i++)
		{
			CModel* pCSetModel = *(ppCSetModel + i);
			
			FLOAT rAngle=rotateAngle*ATOS;
			//FLOAT //*= ATOS;
			//旋转物体
			pCSetModel->m_pNode->rotate(curEditAxisOriPos,&rayVec,rAngle, VTS_WORLD);
			pCSetModel->pRBBox->SetWorldMatrix(&(pCSetModel->m_pNode->getMatrix(VTS_WORLD)));
			pCSetModel->UpdateFromNode();
			//rotateAxis->Rotate(curRotateAxisOriPos,rayVec,rAngle);
			
		} 
	}
	
   	//更新鼠标的三维位置信息
	mouseWorldPos=curMouseWorldPos;
	
}
//-----------------------------------------------------------------------------
// 方法名: GetRotateAngle()
// 参  数: 
// 描  述: 计算旋转的角度
//-----------------------------------------------------------------------------
FLOAT CScene::GetRotateAngle(D3DXVECTOR3* oriVector,D3DXVECTOR3* desVector,D3DXVECTOR3* axisVector)	
{
	
	D3DXVECTOR3 refDir;
	D3DXVECTOR3 disVec;
	D3DXVec3Normalize( oriVector, oriVector);
	D3DXVec3Normalize( desVector, desVector);
	D3DXVec3Normalize( axisVector, axisVector);
	FLOAT x=D3DXVec3Dot(oriVector,desVector);
	if(x>1.0f){
		x=1.0f;	
	}else if(x<-1.0f){
		x=-1.0f;
	}
	FLOAT theta=acos(x);
	disVec=*desVector-(*oriVector);
	D3DXVec3Cross(&refDir,axisVector,oriVector);
	
	if(D3DXVec3Dot(&disVec,&refDir)>=0){
		theta=-theta;
	}
    return theta;
}
//-----------------------------------------------------------------------------
///Edit by baojiahui 2006 12 25------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: GetCoordScreenToWorldFromPlane()
// 参  数: 
//         UINT x                     --屏幕坐标x
//         UINT y                     --屏幕坐标y
//         D3DXVECTOR3* planePoint        --目标平面上一点坐标
//         D3DXVECTOR3* planeNormal       --目标平面的法线矢量
//         D3DXVECTOR3* pOutVec           --返回线面交点
// 用  途: 将屏幕坐标转换成目标平面的世界坐标
//-----------------------------------------------------------------------------
// VOID CScene::GetCoordScreenToWorldFromPlane(CSwapChain* pSwapChain, UINT x,UINT y,D3DXVECTOR3* planePoint,D3DXVECTOR3* planeNormal,D3DXVECTOR3* pOutVec)
VOID CScene::GetCoordScreenToWorldFromPlane(CViewport* pViewport, UINT x,UINT y, 
											const VECTOR3 &planePoint, const VECTOR3 &planeNormal,
											VECTOR3 &OutVec)
{
	//获得一个平面坐标下一点于在世界坐标下的一条投影射线
	GetRayUnderPoint(pViewport, x, y, &m_rayOrig, &m_pickRayDir);

	D3DXVECTOR3 *pPlanePoint = &D3DXVECTOR3(planePoint.x, planePoint.y, planePoint.z);
	D3DXVECTOR3 *pPlaneNormal = &D3DXVECTOR3(planeNormal.x, planeNormal.y, planeNormal.z);	

	//定义一个D3D平面
	D3DXPLANE pPlane;
	//利用平面方程AX+BY+CZ+D=0  (A,B,C)为平面的法线矢量
	pPlane.a = pPlaneNormal->x;
	pPlane.b = pPlaneNormal->y;
	pPlane.c = pPlaneNormal->z;
	pPlane.d =- (pPlaneNormal->x * pPlanePoint->x + pPlaneNormal->y * pPlanePoint->y + pPlaneNormal->z * pPlanePoint->z);

	//获取射线上的一点定为线面求交的直线的终点
	//利用射线方程p=p0+M0*t(t>0,p0为射线起点,M0为方向向量)求设想上一点
	D3DXVECTOR3 rayPoint=D3DXVECTOR3(m_rayOrig.x+m_pickRayDir.x*1000000.0f,m_rayOrig.y+m_pickRayDir.y*1000000.0f,m_rayOrig.z+m_pickRayDir.z*1000000.0f);
	
	//获取线面交点
	D3DXVECTOR3 D3DOutVec;
	D3DXPlaneIntersectLine(&D3DOutVec, &pPlane, &m_rayOrig, &rayPoint);
	OutVec = VECTOR3(D3DOutVec.x, D3DOutVec.y, D3DOutVec.z);
}
///----------------------------------------------------------------------------
///Edit by baojiahui 2007 1 18-------------------------------------------------
//-----------------------------------------------------------------------------
// 方法名: GetCoordScreenToWorldFromPlaneAndRay()
// 参  数: 
//         UINT x                     --屏幕坐标x
//         UINT y                     --屏幕坐标y
//         D3DXVECTOR3* planePoint        --目标平面上一点坐标
//         D3DXVECTOR3* planeNormal       --目标平面的法线矢量
//         D3DXVECTOR3* dipartRaydir      --用户自定义的一个平行于平面的方向向量
//         D3DXVECTOR3* pOutVec           --返回线面交点在给定方向上的分量坐标
// 用  途: 将屏幕坐标转换成目标平面的世界坐标
//-----------------------------------------------------------------------------
VOID CScene::GetCoordScreenToWorldFromPlaneAndRay(CViewport *pViewport/*CSwapChain* pSwapChain*/, UINT x,UINT y,D3DXVECTOR3* planePoint,D3DXVECTOR3* planeNormal,D3DXVECTOR3* dipartRaydir,D3DXVECTOR3* pOutVec)
{
	//获得一个平面坐标下一点于在世界坐标下的一条投影射线
	GetRayUnderPoint(pViewport, x, y, &m_rayOrig, &m_pickRayDir);

	//定义一个D3D平面
	D3DXPLANE pPlane;
	D3DXVECTOR3 refNormal=D3DXVECTOR3(planeNormal->x,planeNormal->y,planeNormal->z);
	D3DXVec3Normalize( &refNormal, &refNormal);
	D3DXVec3Normalize( &m_pickRayDir, &m_pickRayDir);
	FLOAT det = D3DXVec3Dot(&m_pickRayDir, &refNormal);
	if (det >= 0.0f) 
	{
		planeNormal->x=-planeNormal->x;
        planeNormal->y=-planeNormal->y;
		planeNormal->z=-planeNormal->z;
	}
	//利用平面方程AX+BY+CZ+D=0  (A,B,C)为平面的法线矢量
	pPlane.a=planeNormal->x;
	pPlane.b=planeNormal->y;
	pPlane.c=planeNormal->z;
	pPlane.d=-(planeNormal->x*planePoint->x+planeNormal->y*planePoint->y+planeNormal->z*planePoint->z);
	
	//获取射线上的一点定为线面求交的直线的终点
	//利用射线方程p=p0+M0*t(t>0,p0为射线起点,M0为方向向量)求设想上一点
	//D3DXVECTOR3 rayPoint=D3DXVECTOR3(pRayOrig.x+pRayOrig.x*1000000.0f,pRayOrig.y+pRayOrig.y*1000000.0f,pRayOrig.z+pRayOrig.z*1000000.0f);
	D3DXVECTOR3 rayPoint=D3DXVECTOR3(m_rayOrig.x+m_pickRayDir.x*1000000.0f,m_rayOrig.y+m_pickRayDir.y*1000000.0f,m_rayOrig.z+m_pickRayDir.z*1000000.0f);
	
	//获取线面交点
	D3DXVECTOR3 pIntersect;
	D3DXPlaneIntersectLine(&pIntersect,&pPlane,&m_rayOrig,&rayPoint);
	
	refNormal=D3DXVECTOR3(pIntersect.x-planePoint->x,pIntersect.y-planePoint->y,pIntersect.z-planePoint->z);
	D3DXVECTOR3 pRayDir=D3DXVECTOR3(dipartRaydir->x,dipartRaydir->y,dipartRaydir->z);
	
	D3DXVec3Normalize( &refNormal, &refNormal);
	D3DXVec3Normalize( &pRayDir, &pRayDir);
	
	det = D3DXVec3Dot(&pRayDir, &refNormal);
	if (det < 0.0f) 
	{
		dipartRaydir->x=-dipartRaydir->x;
		dipartRaydir->y=-dipartRaydir->y;
		dipartRaydir->z=-dipartRaydir->z;
	}
	//计算与获得的交点和自定义的方向向量垂直的平面
	pPlane.a=-dipartRaydir->x;
	pPlane.b=-dipartRaydir->y;
	pPlane.c=-dipartRaydir->z;
	pPlane.d=dipartRaydir->x*pIntersect.x+dipartRaydir->y*pIntersect.y+dipartRaydir->z*pIntersect.z;
	//获取自定义方向向量射线的终点 
	rayPoint.x=planePoint->x+dipartRaydir->x*1000000.0f;
	rayPoint.y=planePoint->y+dipartRaydir->y*1000000.0f;
	rayPoint.z=planePoint->z+dipartRaydir->z*1000000.0f;
	//rayPoint=D3DXVECTOR3(planePoint->x+dipartRaydir->x*10000.0f,planePoint->y+dipartRaydir->y*10000.0f,planePoint->z+dipartRaydir->z*10000.0f);
	//获取分量射线的产生的交点
	D3DXVECTOR3 pRayOrig=D3DXVECTOR3(planePoint->x,planePoint->y,planePoint->z);
	D3DXPlaneIntersectLine(&pIntersect,&pPlane,&pRayOrig,&rayPoint);
	//返回交点
	pOutVec->x=pIntersect.x;
	pOutVec->y=pIntersect.y;
	pOutVec->z=pIntersect.z;
}

//-----------------------------------------------------------------------------
// 方法名: setModelCtrl()
// 参  数: 
// 描  述: 设置物体的编辑状态
//-----------------------------------------------------------------------------
VOID CScene::setModelCtrl()
{
	/*
		}
		m_ModelEdit=FALSE;
	}else if(InputDevice.mouseInfo.LButton==TRUE){
		if(sceneEditMode >0 &&sceneEditMode <7){
			if(m_SelectedModel.size()>0)
			{
				MoveModel();
			}
		}else if(sceneEditMode >=7 &&sceneEditMode <=9){
			if(m_SelectedModel.size()>0)
			{
				RotateModel();
			}
		}else if(sceneEditMode >10){
			
			//ScaleModel(pSwapChain);
		}
		
	}*/
}

//-----------------------------------------------------------------------------
// 方法名: ConstructShaderSubMeshList()
// 参  数: 
// 描  述: 构造场景中所有材质的SubMesh列表
//-----------------------------------------------------------------------------
void CScene::ConstructShaderSubMeshList()
{
	int shaderCnt = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMtrl = m_vShader.begin();
	for (int i = 0; i< shaderCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		
		//将使用pMtrl材质的SubMesh规整到pMtrl->subMeshList中
		pMtrl->subMeshList.clear();
		PushSubMeshToShader(pMtrl);
	}
}

//-----------------------------------------------------------------------------
// 方法名: PushSubMeshToShader()
// 参  数: 
// 描  述: 将使用pMtrl材质的SubMesh规整到pMtrl->subMeshList中
//-----------------------------------------------------------------------------
void CScene::PushSubMeshToShader(CMaterial* pMtrl)
{
	int modelCnt = model.size();
	std::vector<CModel*>::iterator ppModel = model.begin();
	for (int i = 0; i< modelCnt; i++)
	{
		CModel* pModel = *(ppModel + i);
		
		int subMeshCnt = pModel->getSubModelCount();
		for (int j = 0; j< subMeshCnt; j++)
		{
			CMaterial *pMaterial = pModel->getSubModelMaterial(j);
			if (pMaterial && pMaterial == pMtrl)
			{
				VGS_RenderSubmesh subMesh;
				subMesh.modelPointer = (DWORD)pModel;
				subMesh.subMeshID = j;
				pMtrl->subMeshList.push_back(subMesh);
			}
		}
	}
}


//创建SwapChain
CSwapChain*  CScene::CreateSwapChain(HWND hWnd, D3DMULTISAMPLE_TYPE multiSampleType)
{
	//CSwapChain* pSwapChain = new CSwapChain
	CSwapChain* pSwapChain = new CSwapChain(m_pd3d, m_pd3dDevice, hWnd);
	pSwapChain->m_MultiSampleType = multiSampleType;
	pSwapChain->Create();
	m_vSwapChain.push_back(pSwapChain);
	return pSwapChain;
}

// 相机相关
CCamera*  CScene::CreateCamera(const std::string &sName)
{
	if (IsCameraExist(sName))
	{
		return 0;
	}

	CCamera *pCamera = new CCamera();
	pCamera->SetScene(this);

	if (!pCamera)
	{
		return 0;
	}

	pCamera->index = this->m_vCamera.size();
	pCamera->setName(sName);
	pCamera->SetFov(45.0f * ATOS);
	pCamera->setNearClip(1.0f);
	pCamera->setFarClip(10000.0f);

	this->m_vCamera.push_back(pCamera);

	CSceneNode *pNode = new CSceneNode();
	if (!pNode)
	{
		return 0;
	}
	//pNode->setNodeType(VGS_MOVABLETYPE_CAMERA);
	//pNode->m_object = pCamera;
	pNode->attachObject(pCamera);

	pNode->m_parent = m_pRootNode;
	m_pRootNode->m_child.push_back(pNode);


	//计算节点的变换矩阵
	D3DXMATRIX transMat;	
    D3DXVECTOR3 dir = D3DXVECTOR3(0, 0, 1);	
	//pos
	D3DXVECTOR3 pos = D3DXVECTOR3(0, 0, -100);
	//quat
	D3DXQUATERNION quat = D3DXQUATERNION(0, 0, 0, 1);
	
	//scale
	D3DXVECTOR3 scale = D3DXVECTOR3(1,1,1);
	D3DXVECTOR3 ZeroVector = D3DXVECTOR3(0,0,0);
	// D3DXQUATERNION ZeroQuat = D3DXQUATERNION(0, 0, 0, 0);
    //D3DXMatrixTransformation(&transMat, &ZeroVector, &ZeroQuat, &scale, &ZeroVector, &quat, &pos);
	//pNode->curTransform.matrix = transMat;
	//pNode->updateTransform();
	pNode->setScale(&scale, VTS_WORLD);
	pNode->setRotationByAngle(&ZeroVector, VTS_WORLD);
	pNode->setPosition(&ZeroVector, VTS_WORLD);

	D3DXVECTOR3 vFrom(0, 0, 0);
	D3DXVECTOR3 vAt(0, 0, 1);
	pCamera->SetPosition(&vFrom, VTS_WORLD);
	pCamera->Lookat(&vAt, VTS_WORLD);
	pCamera->SetRoll(0);

	return pCamera;
}

UINT CScene::getCameraCount()
{
	return m_vCamera.size();
}
CCamera* CScene::GetCameraByName(const std::string &sName)
{
	return _GetHandleByName(m_vCamera, sName);
}
CCamera* CScene::GetCameraByIndex(INT index)
{
	return _GetHandleByIndex(m_vCamera, index);
}
std::string CScene::GetCameraNameByIndex(INT index)
{	
	return _GetNameByIndex(m_vCamera, index);
}
INT CScene::GetCameraIndex(CCamera* pCamera)
{
	//  设置视口的相机无效
// 	set<DWORD> vViewport;
// 	pCamera->getViewport(vViewport);
// 	set<DWORD>::iterator it = vViewport.begin();
// 	while (it != vViewport.end())
// 	{
// 		CViewport *pViewport = (CViewport*)(*it);
// 		if (pViewport)
// 		{
// 			pViewport->SetCamera(NULL);
// 		}
// 		++it;
// 	}

	return _GetIndexByHandle(m_vCamera, pCamera);
}
VOID CScene::DeleteCamera(CCamera* pCamera)
{
	//  设置视口的相机无效
	vector<DWORD> vViewport;
	pCamera->getViewport(vViewport);
	vector<DWORD>::iterator it = vViewport.begin();
	while (it != vViewport.end())
	{
		CViewport *pViewport = (CViewport*)(*it);
		if (pViewport)
		{
			pViewport->SetCamera(NULL);
		}
		++it;
	}

	_DeleteByHandle(m_vCamera, pCamera);
}
VOID CScene::DeleteCamera(INT index)
{
	if (index >= 0 && index < (int)m_vCamera.size())
	{
		DeleteCamera(m_vCamera[index]);
	}
}

BOOL CScene::IsCameraExist(const std::string &sName)
{
	UINT count = m_vCamera.size();
	for (UINT i = 0; i < count; ++i)
	{
		if (m_vCamera[i]->getName().compare(sName.c_str()) == 0) // 有重名
		{
			return TRUE;
		}
	}
	return FALSE;
}


// 如果相机重名，重新命名
std::string CScene::renameCameraIfExist(std::string &sName)
{
	int count = m_vCamera.size();
	if (count < 1)
	{
		return sName;
	}
	
	std::string sNewName = sName;
	int idx = 0;
	while (TRUE)
	{
		bool bConflict = false;
		for (int i = 0; i < count; ++i)
		{
			if (m_vCamera[i]->getName().compare(sNewName.c_str()) == 0) // 有重名
			{
				char tmp[10];
				sNewName = sName + "_";
				if (0 == _itoa_s(idx, tmp, sizeof(tmp), 10))
				{
					sNewName += std::string(tmp);
					bConflict = true;
					break;
				}
			}
		}
		if (!bConflict)
		{
			break;
		}
		++idx;
	}
	return sNewName;
}

// 灯光相关
CLight *CScene::CreateLight(const std::string &sName)
{
	if (IsLightExist(sName))
	{
		return 0;
	}

	CLight *pLight = new CLight(m_pd3dDevice);

	if (!pLight)
	{
		return 0;
	}

	pLight->index = this->m_vLight.size();
	pLight->setName(sName);

	this->m_vLight.push_back(pLight);

	CSceneNode *pNode = new CSceneNode();
	if (!pNode)
	{
		return 0;
	}
	/*pNode->setNodeType(VGS_MOVABLETYPE_LIGHT);
	pNode->m_object = pLight;*/
	pNode->attachObject(pLight);

	pNode->m_parent = m_pRootNode;
	m_pRootNode->m_child.push_back(pNode);


	//计算节点的变换矩阵
	D3DXMATRIX transMat;	
	//pos
	D3DXVECTOR3 pos = D3DXVECTOR3(100, 100, -100);
	//quat
	D3DXQUATERNION quat = D3DXQUATERNION(0, 0, 0, 1);
	
	//scale
	D3DXVECTOR3 scale = D3DXVECTOR3(1,1,1);
	D3DXVECTOR3 ZeroVector = D3DXVECTOR3(0,0,0);
	//D3DXQUATERNION ZeroQuat = D3DXQUATERNION(0, 0, 0, 0);
    //D3DXMatrixTransformation(&transMat, &ZeroVector, &ZeroQuat, &scale, &ZeroVector, &quat, &pos);
	//pNode->curTransform.matrix = transMat;
	//pNode->updateTransform();
	pNode->setScale(&scale, VTS_WORLD);
	pNode->setRotationByAngle(&ZeroVector, VTS_WORLD);
	pNode->setPosition(&ZeroVector, VTS_WORLD);


	return pLight;
}
UINT CScene::getLightCount()
{
	return m_vLight.size();
}
CLight* CScene::GetLightByName(const std::string &sName)
{	
	return _GetHandleByName(m_vLight, sName);
}
CLight* CScene::GetLightByIndex(INT index)
{
	return _GetHandleByIndex(m_vLight, index);
}
std::string CScene::GetLightNameByIndex(INT index)
{
	return _GetNameByIndex(m_vLight, index);
}
INT CScene::GetLightIndex(CLight* pLight)
{
	return _GetIndexByHandle(m_vLight, pLight);
}
VOID CScene::DeleteLight(CLight* pLight)
{
	_DeleteByHandle(m_vLight, pLight);
}
VOID CScene::DeleteLight(INT index)
{
	if (index >= 0 && index < (INT)m_vLight.size())
	{
		DeleteLight(m_vLight[index]);
	}
}

BOOL CScene::IsLightExist(const std::string &sName)
{	
	UINT count = m_vLight.size();
	for (UINT i = 0; i < count; ++i)
	{
		if (m_vLight[i]->getName().compare(sName.c_str()) == 0) // 有重名
		{
			return TRUE;
		}
	}
	return FALSE;
}
////-----------     材质  --------------------------------------------------------------

BOOL CScene::IsMaterialExist(const std::string &sMatName)
{
// 	CMaterial *pMaterial = _GetHandleByName(m_vShader, sMatName);
// 	return (pMaterial != NULL);
	if (m_pRoot)
	{
		return m_pRoot->GetMaterialManager().IsResourceExist(sMatName);
	}
	return FALSE;
}

// add a meterial to list
CMaterial* CScene::AddMaterial(VGS_MATERIAL_TYPE type, CMaterial *pMaterial)
{	
	if (!pMaterial)
	{
		return 0;
	}

	if (type == VGS_MATERIAL_TYPE_3D)
	{
		std::vector<CMaterial*>::iterator it = m_vShader.begin();
		for (; it != m_vShader.end(); ++it)
		{
			if (*it == pMaterial)
			{
				return pMaterial;
			}
		}
		m_vShader.push_back(pMaterial);
	}
	else if (type == VGS_MATERIAL_TYPE_2D)
	{
		std::vector<CMaterial*>::iterator it = m_v2DMaterial.begin();
		for (; it != m_v2DMaterial.end(); ++it)
		{
			if (*it == pMaterial)
			{
				return pMaterial;
			}
		}
		m_v2DMaterial.push_back(pMaterial);
	}	
	return pMaterial;
}

CMaterial* CScene::CreateMaterial(const std::string &sMatName)
{
	if (!m_pRoot || m_pRoot->GetMaterialManager().IsResourceExist(sMatName))
	{
		return 0;
	}
	
//  CMaterial *pMaterial = new CMaterial();
// 	if (pMaterial)
// 	{
// 		pMaterial->setName(sMatName);
// 	}
//	return pMaterial;
	return (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(sMatName);
}
VOID CScene::DeleteMaterialIfNoUse(CMaterial *pMaterial)
{
	if (pMaterial)
	{
		if (pMaterial->subMeshList.size() == 0)
		{
			_DeleteByHandle(m_vShader, pMaterial);
			SAFE_RELEASE(pMaterial);
			SAFE_DELETE(pMaterial);
		}
	}
}

// 模型相关

// 创建模型
CModel* CScene::CreateModel(const std::string &sName)
{
	//模型名
	std::string RealName = RenameModelIfExist(sName);
	CModel *pModel = new CModel(m_pd3dDevice);
	if (pModel)
	{
		pModel->setName(RealName);
		pModel->index = model.size();
		model.push_back(pModel);
	}
	return pModel;
}

UINT CScene::GetModelCount()
{
	return model.size();
}
CModel* CScene::GetModelByName(const std::string &sName)
{	
	return _GetHandleByName(model, sName);
}
CModel* CScene::GetModelByIndex(INT index)
{
	return _GetHandleByIndex(model, index);
}
std::string CScene::GetModelNameByIndex(INT index)
{
	return _GetNameByIndex(model, index);
}
INT CScene::GetModelIndex(CModel* pModel)
{
	return _GetIndexByHandle(model, pModel);
}
VOID CScene::DeleteModel(CModel* pModel)
{
	_DeleteByHandle(model, pModel);
}

VOID CScene::DeleteModel(INT index)
{
	if (index >= 0 && index < (INT)model.size())
	{
		DeleteModel(model[index]);
	}
}

BOOL CScene::IsModelExist(const std::string &sName)
{	
	UINT count = model.size();
	for (UINT i = 0; i < count; ++i)
	{
		if (model[i]->getName().compare(sName.c_str()) == 0) // 有重名
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 将模型添加到场景
void CScene::AddModel(CModel *pModel)
{
	if (pModel && !IsModelExist(pModel->getName()))
	{
		model.push_back(pModel);
	}
	if (pModel && pModel->IsTransparent())
	{
		if (_GetIndexByHandle(m_vTransModel, pModel) == -1)
		{
			m_vTransModel.push_back(pModel);  // 透明模型
		}
	}
}
void CScene::AddModelToTransparentList(CModel *pModel)
{
	if (pModel)
	{
		int index = _GetIndexByHandle(m_vTransModel, pModel);
		if (index == -1)
		{
			m_vTransModel.push_back(pModel);
		}
	}
}
void CScene::RemoveModelFormTransparentList(CModel *pModel)
{
	if (pModel)
	{
		int index = _GetIndexByHandle(m_vTransModel, pModel);
		if (index >= 0 && index < (int)m_vTransModel.size())
		{
			m_vTransModel.erase(m_vTransModel.begin() + index);
		}
	}
}

// 资源重命名
std::string CScene::RenameMaterialIfExist(const std::string &sName) 
{
	int index = 0;
	std::string sNewName = sName;
	while (IsMaterialExist(sNewName))
	{
		char tmp[10];
		sNewName = sName;
		if (0 == _itoa_s(index++, tmp, sizeof(tmp), 10))
		{
			sNewName += std::string(tmp);
		}
	}
	return sNewName;
}

std::string CScene::RenameCameraIfExist(const std::string &sName) 
{
	int index = 0;
	std::string sNewName = sName;
	while (GetCameraByName(sNewName) != NULL)
	{
		char tmp[10];
		sNewName = sName;
		if (0 == _itoa_s(index++, tmp, sizeof(tmp), 10))
		{
			sNewName += std::string(tmp);
		}
	}
	return sNewName;
}

std::string CScene::RenameLightIfExist(const std::string &sName) 
{
	int index = 0;
	std::string sNewName = sName;
	while (IsLightExist(sNewName))
	{
		char tmp[10];
		sNewName = sName;
		if (0 == _itoa_s(index++, tmp, sizeof(tmp), 10))
		{
			sNewName += std::string(tmp);
		}
	}
	return sNewName;
}

std::string CScene::RenameModelIfExist(const std::string &sName) 
{
	int index = 0;
	std::string sNewName = sName;
	while (IsModelExist(sNewName))
	{
		char tmp[10];
		sNewName = sName;
		if (0 == _itoa_s(index++, tmp, sizeof(tmp), 10))
		{
			sNewName += std::string(tmp);
		}
	}
	return sNewName;
}

// 获取场景根基点
CSceneNode* CScene::GetRootSceneNode()
{
	if (!m_pRootNode)
	{
		m_pRootNode = new CSceneNode();
		// m_pRootNode->setNodeType(VGS_MOVABLETYPE_SCENEROOT);
	}
	return m_pRootNode;
}

#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
// 导入s3d数据
DWORD CScene::ImportS3DFile(const std::string &strS3DFileName, HWND hWnd)
{
	CGetS3DFileInfo S3DInfo;
	if (S3DInfo.ReadS3DFile(strS3DFileName, hWnd))
	{
		CImportS3D *pS3DLoader = new CImportS3D();
		if (pS3DLoader)
		{
			pS3DLoader->LoadS3DFileInfo(&S3DInfo, this, hWnd); //LoadS3DFileInfo
			// 设置灯光参数
			for (int iLight = 0; iLight < (int)m_vLight.size(); ++iLight)
			{
				CLight *pLight = m_vLight[iLight];
				m_pd3dDevice->SetLight(iLight, &pLight->param);
				m_pd3dDevice->LightEnable(iLight, TRUE);
			}
		}		
		return (DWORD)pS3DLoader;
	}
	return 0;
}
// 保存场景为v3d文件
DWORD CScene::SaveV3DFile(const std::string &sFile, HWND hWnd)
{
	CSaveV3D savev3d;
	return savev3d.SaveSceneAsV3D(this, sFile, hWnd, false);
}
// 加载v3d文件
DWORD CScene::OpenV3DFile(const std::string &sFileName, HWND hWnd)
{
	DWORD re = ERR_FILE_V3D_OPENFAIL;
	CGetV3DBase *pV3DLoader = _getV3DLoader(sFileName);
	if (pV3DLoader)
	{
		re = pV3DLoader->OpenV3DFile(this, sFileName, hWnd);
		SAFE_RELEASE(pV3DLoader);
		SAFE_DELETE(pV3DLoader);
	}

	return re;

	/*CGetV3DBase getV3D(m_pd3dDevice);
	if (getV3D.isMyVersion(sFileName))
	{
		return getV3D.OpenV3DFile(this, sFileName, hWnd);
	}
	return ERR_FILE_V3D_OPENFAIL;*/
}
#endif

// 准备渲染 
VOID CScene::InitRenderSatusFromSwapchain(CSwapChain* pSwapChain)  
{
	CViewport *pVP = NULL;
	if (pSwapChain)
	{
		pVP = pSwapChain->GetViewport(0);
		if (pVP)
		{
			m_pCurCamera = pVP->GetCamera();		
		}
	}
}

//-----------------------------------------------------------------------------
// 方法名: CreateTexFromFile()
// 参  数: CHAR* strFileName, CTexture* pTex
// 描  述: 从文件创建贴图
//-----------------------------------------------------------------------------
// HRESULT	CScene::CreateOpacityTexFromFile(CHAR* strFileName, CTexture* pTex, INT mipLevel)
// {
// 	
// 	return 0;
// }

// texture相关
UINT CScene::getTextureCount()
{
	return m_vTexture.size();
}

CTexture* CScene::getTextureByIndex(INT index)
{
	if (index >= 0 && index < (INT)m_vTexture.size())
	{
		return m_vTexture[index];
	}
	return 0;
}

CTexture* CScene::getTextureByName(const std::string &sName)
{
	if (m_pRoot)
	{
		return (CTexture*)m_pRoot->GetTextureManager().getResource(sName);
	}
	return 0;
}

UINT CScene::getMaterialCount()
{
	return m_vShader.size();
}

VOID CScene::getMaterialList(vector <CMaterial*> &vMaterial)
{
	vMaterial = m_vShader;
}

CMaterial* CScene::GetMaterialByName(const std::string &sName)
{
	UINT count = m_vShader.size();
	std::vector<CMaterial*>::iterator ppMaterial = m_vShader.begin();
	for (UINT i = 0; i < count; ++i)
	{
		CMaterial *pMaterial = *(ppMaterial + i);
		if (pMaterial && pMaterial->getName().compare(sName) == 0)
		{
			return pMaterial;
		}
	}
	return 0;
}
CMaterial* CScene::GetMaterialByIndex(INT index)
{
	if (index >= 0 && index < (INT)m_vShader.size())
	{
		return m_vShader[index];
	}
	return 0;
}

INT CScene::getMaterialIndex(CMaterial* pMaterial)
{
	return _GetIndexByHandle(m_vShader, pMaterial);
}

UINT CScene::getImageCount()
{
	return m_vImage.size();
}
// 根据索引获取图像
CImage* CScene::getImageByIndex(INT index)
{
	if (index >= 0 && index < (INT)m_vImage.size())
	{
		return m_vImage[index];
	}
	return 0;
}


///////////////////// 控制对象相关 ////////////////////////

// 创建walker控制器
IController* CScene::CreateWalker(const WalkerInfo &walkinfo)
{
	IController *pWalker = GetWalker(string(walkinfo.Name));
	if (pWalker)
	{
		string ss = "walker";
		ss.append(string(walkinfo.Name));
		ss.append("已经存在！");
		::MessageBox(NULL, (ss.c_str()), TEXT("Error"), 0);
		return pWalker;
	}
	/*CWalker **/pWalker = new CWalker(walkinfo);
	// pWalker->SetWalkerInfo(walkinfo);
	pWalker->SetCamera(this->GetCameraByName(walkinfo.Camera));
	m_vWalker.push_back(pWalker);
	return pWalker;
}
// 创建flyer控制器
IController* CScene::CreateFlyer(const FlyerInfo &flyerinfo)
{
	IController *pFlyer = GetFlyer(string(flyerinfo.Name));
	if (pFlyer)
	{
		string ss = "Flyer";
		ss.append(string(flyerinfo.Name));
		ss.append("已经存在！");
		MessageBox(NULL, (ss.c_str()), TEXT("Error"), 0);
		return pFlyer;
	}
	/*CFlyer **/pFlyer = new CFlyer(flyerinfo);
	//pFlyer->SetFlyerInfo(flyerinfo);
	pFlyer->SetCamera(GetCameraByName(flyerinfo.Camera));
	m_vFlyer.push_back(pFlyer);
	return pFlyer;
}
// 创建editor控制器
IController* CScene::CreateEditor(const EditorInfo &editorinfo)
{
	IController *pEditor = GetEditor(string(editorinfo.Name));
	if (pEditor)
	{
		string ss = "Editor" ;
		ss.append(string(editorinfo.Name));
		ss.append("已经存在！");
		MessageBox(NULL, (ss.c_str()), TEXT("Error"), 0);
		return pEditor;
	}
	/*CEditor **/pEditor = new CEditor(this, editorinfo); // ControllerFactory::CreateController(CT_EDITOR);
	// pEditor->SetEditorInfo(editorinfo);
	pEditor->SetCamera(GetCameraByName(editorinfo.Camera));
	m_vEditor.push_back(pEditor);
	return pEditor;
}

// 获取控制器数目
UINT CScene::GetControlCount()
{
	return m_vWalker.size() + m_vFlyer.size() + m_vEditor.size();
}
UINT CScene::GetWalkerCount()
{
	return m_vWalker.size();
}
UINT CScene::GetFlyerCount()
{
	return m_vFlyer.size();
}
UINT CScene::GetEditorCount()
{
	return m_vEditor.size();
}
VOID CScene::AddWalker(IController *pWalker)
{
	if (!_IsExist(m_vWalker, pWalker))
	{
		m_vWalker.push_back(pWalker);
	}
}
VOID CScene::AddEditor(IController *pEditor)
{
	if (!_IsExist(m_vEditor, pEditor))
	{
		m_vEditor.push_back(pEditor);
	}

}
VOID CScene::AddFlyer(IController *pFlyer)
{
	if (!_IsExist(m_vFlyer, pFlyer))
	{
		m_vFlyer.push_back(pFlyer);
	}
}
// 根据索引获取控制器
IController* CScene::GetControl(UINT idx)
{
	if (idx < 0)
	{
		return NULL;
	}
	else if (idx < m_vWalker.size())
	{
		return m_vWalker[idx];
	}
	else if (idx < m_vWalker.size() + m_vEditor.size())
	{
		return m_vEditor[idx - m_vWalker.size()];
	}
	else if (idx < m_vWalker.size() + m_vEditor.size() + m_vFlyer.size())
	{		
		return m_vFlyer[idx - m_vWalker.size() - m_vEditor.size()];
	}
	else
	{
		return NULL;
	}
}

IController* CScene::GetWalker(UINT idx)
{
	if (idx >= 0 && idx < m_vWalker.size())
	{
		return (m_vWalker[idx]);
	}
	return NULL;
}
IController* CScene::GetFlyer(UINT idx)
{
	if (idx >= 0 && idx < m_vFlyer.size())
	{
		return (m_vFlyer[idx]);
	}
	return NULL;
}
IController* CScene::GetEditor(UINT idx)
{
	if (idx >= 0 && idx < m_vEditor.size())
	{
		return (m_vEditor[idx]);
	}
	return NULL;
}

IController* CScene::GetWalker(const string sName)
{
	for (UINT i = 0; i < m_vWalker.size(); ++i)
	{
		if (0 == strcmp(sName.c_str(), m_vWalker[i]->GetName().c_str()))
		{
			return (m_vWalker[i]);
		}
	}
	return NULL;
}

IController* CScene::GetFlyer(const string sName)
{
	for (UINT i = 0; i < m_vFlyer.size(); ++i)
	{
		if (0 == strcmp(sName.c_str(), m_vFlyer[i]->GetName().c_str()))
		{
			return (m_vFlyer[i]);
		}
	}
	return NULL;
}

IController* CScene::GetEditor(const string sName)
{
	for (UINT i = 0; i < m_vEditor.size(); ++i)
	{
		if (0 == strcmp(sName.c_str(), m_vEditor[i]->GetName().c_str()))
		{
			return (m_vEditor[i]);
		}
	}
	return NULL;
}

INT CScene::GetWalkerIndex(IController *pWalker)
{
	return _GetIndexByHandle(m_vWalker, pWalker);
}
INT CScene::GetEditorIndex(IController *pEditor)
{
	return _GetIndexByHandle(m_vEditor, pEditor);
}
INT CScene::GetFlyerIndex(IController *pFlyer)
{
	return _GetIndexByHandle(m_vFlyer, pFlyer);
}

// 删除控制器
void CScene::DeleteWalker(const string &sName)
{
	bool bDeleteCurrent = false;  // 是否删除了当前控制器
	for (UINT i = 0; i < m_vWalker.size(); ++i)
	{
		if (0 == strcmp(sName.c_str(), m_vWalker[i]->GetName().c_str()))
		{
			if (m_pCurController == m_vWalker[i])
			{
				bDeleteCurrent = true;
			}
			delete m_vWalker[i];
			m_vWalker.erase(m_vWalker.begin() + i);
			break;
		}
	}

	// 如果删除了当前控制器，需要重新设置
	if (bDeleteCurrent)
	{
		if (m_vWalker.size() > 0)
		{
			m_pCurController = m_vWalker[0];
		}
		else if (m_vEditor.size() > 0)
		{
			m_pCurController = m_vEditor[0];
		}
		else if (m_vFlyer.size() > 0)
		{
			m_pCurController = m_vFlyer[0];
		}
		else
		{
			m_pCurController = NULL;
			MessageBox(NULL, TEXT("当前没有指定控制模式"), TEXT( "提醒"), 0);
		}
	}
}
void CScene::DeleteEditor(const string &sName)
{
	bool bDeleteCurrent = false;
	for (UINT i = 0; i < m_vEditor.size(); ++i)
	{
		if (0 == strcmp(sName.c_str(), m_vEditor[i]->GetName().c_str()))
		{
			if (m_pCurController == m_vEditor[i])
			{
				bDeleteCurrent = true;
			}
			delete m_vEditor[i];
			m_vEditor.erase(m_vEditor.begin() + i);
			break;
		}
	}
	// 如果删除了当前控制器，需要重新设置
	if (bDeleteCurrent)
	{
		if (m_vEditor.size() > 0)
		{
			m_pCurController = m_vEditor[0];
		}
		else if (m_vWalker.size() > 0)
		{
			m_pCurController = m_vWalker[0];
		}
		else if (m_vFlyer.size() > 0)
		{
			m_pCurController = m_vFlyer[0];
		}
		else
		{
			m_pCurController = NULL;
			MessageBox(NULL, TEXT("当前没有指定控制模式"), TEXT("提醒"), 0);
		}
	}
}
void CScene::DeleteFlyer(const string &sName)
{
	bool bDeleteCurrent = false;
	for (UINT i = 0; i < m_vFlyer.size(); ++i)
	{
		if (0 == strcmp(sName.c_str(), m_vFlyer[i]->GetName().c_str()))
		{
			if (m_pCurController == m_vFlyer[i])
			{
				bDeleteCurrent = true;
			}
			delete m_vFlyer[i];
			m_vFlyer.erase(m_vFlyer.begin() + i);
			break;
		}
	}
	// 如果删除了当前控制器，需要重新设置
	if (bDeleteCurrent)
	{
		if (m_vFlyer.size() > 0)
		{
			m_pCurController = m_vFlyer[0];
		}
		else if (m_vEditor.size() > 0)
		{
			m_pCurController = m_vEditor[0];
		}
		else if (m_vWalker.size() > 0)
		{
			m_pCurController = m_vWalker[0];
		}
		else
		{
			m_pCurController = NULL;
			MessageBox(NULL, TEXT("当前没有指定控制模式"), TEXT("提醒"), 0);
		}
	}
}

void CScene::DeleteWalker(DWORD handle)
{
	for (UINT i = 0; i < m_vWalker.size(); ++i)
	{
		if (handle == (DWORD)m_vWalker[i])
		{
			delete m_vWalker[i];
			m_vWalker.erase(m_vWalker.begin() + i);
			break;
		}
	}
	// 如果删除了当前控制器，需要重新设置
	if (handle == (DWORD)m_pCurController)
	{
		if (m_vWalker.size() > 0)
		{
			m_pCurController = m_vWalker[0];
		}
		else if (m_vEditor.size() > 0)
		{
			m_pCurController = m_vEditor[0];
		}
		else if (m_vFlyer.size() > 0)
		{
			m_pCurController = m_vFlyer[0];
		}
		else
		{
			m_pCurController = NULL;
			MessageBox(NULL, TEXT("当前没有指定控制模式"), TEXT("提醒"), 0);
		}
	}
}
void CScene::DeleteEditor(DWORD handle)
{
	for (UINT i = 0; i < m_vEditor.size(); ++i)
	{
		if (handle == (DWORD)m_vEditor[i])
		{
			delete m_vEditor[i];
			m_vEditor.erase(m_vEditor.begin() + i);
			break;
		}
	}
	// 如果删除了当前控制器，需要重新设置
	if (handle == (DWORD)m_pCurController)
	{
		if (m_vEditor.size() > 0)
		{
			m_pCurController = m_vEditor[0];
		}
		else if (m_vWalker.size() > 0)
		{
			m_pCurController = m_vWalker[0];
		}
		else if (m_vFlyer.size() > 0)
		{
			m_pCurController = m_vFlyer[0];
		}
		else
		{
			m_pCurController = NULL;
			MessageBox(NULL, TEXT("当前没有指定控制模式"), TEXT("提醒"), 0);
		}
	}
}
void CScene::DeleteFlyer(DWORD handle)
{
	for (UINT i = 0; i < m_vFlyer.size(); ++i)
	{
		if (handle == (DWORD)m_vFlyer[i])
		{
			delete m_vFlyer[i];
			m_vFlyer.erase(m_vFlyer.begin() + i);
			break;
		}
	}
	// 如果删除了当前控制器，需要重新设置
	if (handle == (DWORD)m_pCurController)
	{
		if (m_vFlyer.size() > 0)
		{
			m_pCurController = m_vFlyer[0];
		}
		else if (m_vEditor.size() > 0)
		{
			m_pCurController = m_vEditor[0];
		}
		else if (m_vWalker.size() > 0)
		{
			m_pCurController = m_vWalker[0];
		} 
		else
		{
			m_pCurController = NULL;
			MessageBox(NULL, TEXT("当前没有指定控制模式"), TEXT("提醒"), 0);
		}
	}
}

// 设置当前控制器
void CScene::SetCurrentController(IController* pController)
{
	m_pCurController = pController;
}

// 获取当前控制器
IController* CScene::GetCurrentController()
{
	return m_pCurController;
}

// 删除场景中的所有控制器
void CScene::DeleteAllController()
{
	for (UINT iWalker = 0; iWalker < m_vWalker.size(); ++iWalker)
	{
		if (m_vWalker[iWalker])
		{
			delete m_vWalker[iWalker];
			m_vWalker[iWalker] = NULL;
		}
	}
	m_vWalker.clear();
	
	for (UINT iFlyer = 0; iFlyer < m_vFlyer.size(); ++iFlyer)
	{
		if (m_vFlyer[iFlyer])
		{
			delete m_vFlyer[iFlyer];
			m_vFlyer[iFlyer] = NULL;
		}
	}
	m_vFlyer.clear();
	
	for (UINT iEditor = 0; iEditor < m_vEditor.size(); ++iEditor)
	{
		if (m_vEditor[iEditor])
		{
			delete m_vEditor[iEditor];
			m_vEditor[iEditor] = NULL;
		}
	}
	m_vEditor.clear();
	
	m_pCurController = NULL;
}

//清空贴图内存
VOID CScene::ClearTextureMem()
{
	//set tex-0, tex-1.. tex-8
	for (INT i = 0; i < 8; i++)
	{
		m_pd3dDevice->SetTexture(i, NULL);
		m_pd3dDevice->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_pd3dDevice->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, 0);
	}
}


// DWORD CScene::LoadSceneFromBuffer(DWORD version, const BYTE* pBuffer, HWND hWnd)
// {
// 	CN3DLoader N3DLoader;
// 	N3DLoader.ReadSceneProp();
// 	return 0;
// }
// DWORD CScene::LoadImageDataFromBuffer(DWORD version, const BYTE* pBuffer, HWND hWnd)
// {
// 	return 0;
// }

//Add by baojiahui 2009/2/11---------------------------------------------------
// 方法名: RenderOneModel()
// 参  数: 
// 描  述: 渲染一个物体，在渲染透明物体时使用
//-----------------------------------------------------------------------------
VOID CScene::RenderOneModel(CModel* pModel)
{ 
    //设置当前渲染物体 
	m_CurModel = pModel;

	//关键帧动画
	//if (m_bKfrPlaying)  // fuxb 2009-02-26 增加判断条件，避免误播动画
	{
		PlayModelKeyFrame();
	}

	//设置模型的MoveModelTo, 必须放在这里，否则当物体离视点较远时，不处理了
	if (m_CurModel->user_moveFrameCnt > 0)
		PlayUserMoveModelKFR();
	
	//模型是否是billboard模型
	if (m_CurModel->isBillboard)
		SetModelBillboard(m_CurModel);
	
	//如果模型不可见，不渲染
	if (!m_CurModel->visible) return;

	//如果模型没有被剪切, 渲染模型的SubMesh
	if (m_CurModel->culled) return;

	
	//设置是否重新计算模型法线，保证缩放模型能正常显示高光
	m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, m_CurModel->isAutoNormalise);

	//设置该模型当前帧的矩阵
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &(m_CurModel->m_pNode->getMatrix(VTS_WORLD)));

	INT subMeshCnt = m_CurModel->getSubModelCount();
	// std::vector<_subMesh>::iterator _pSubMesh = m_CurModel->subMesh.begin();
	for (INT i = 0; i< subMeshCnt; i++)
	{
		// _subMesh* pSubMesh = &(*(_pSubMesh + i));

		CMaterial* pMtrl = m_CurModel->getSubModelMaterial(i); // pSubMesh->shader;

		//清空贴图
		ClearTextureMem();

		//设置材质，渲染凹凸纹理
	    pMtrl->SetToDevice(m_pd3dDevice, 1);

		//判断是否是双面材质这个判断应该写在外面，因为涉及到反射渲染的问题
		if (pMtrl->twoSide )
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	
		else
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

        //构建渲染submesh--------------------------------------------------------------
		VGS_RenderSubmesh _curSubMesh;
		_curSubMesh.modelPointer =(DWORD)m_CurModel;
		_curSubMesh.subMeshID = i;
       //------------------------------------------------------------------------------
	   //渲染到后缓冲表面		
		// m_CurModel->RenderSubMesh(_curSubMesh.subMeshID);
		m_CurModel->RenderSubMesh(i);

		//如果需要辅助表面，渲染到辅助表面------------------------------------------------------------
		INT addtionalRenderSurfCnt = m_CurModel->m_additionalRenderInfo.size();
		for (INT m = 0; m < addtionalRenderSurfCnt; m++)
		{
			std::vector<ADDITIONALRENDERINFO>::iterator pRenderInfo = m_CurModel->m_additionalRenderInfo.begin() + m;
			switch (pRenderInfo->type)
			{
				//实时反射辅助表面
			case ADS_RTFMAP:
				{
					CRTFMap* pRTFMap = (CRTFMap*)(pRenderInfo->pRenderToTexture);
					// pRTFMap->UpdateTexBuf();
					RenderSubMeshToRTFTexture(pRTFMap, &_curSubMesh/*, pMtrl*/);
				}
				break;
				
				//实时折射辅助表面
			case ADS_RTTMAP:
				break;

				//凹凸贴图的合成表面
			case ADS_BUMPWAVEMAP:
				{
					CBumpMap* pBumpMap = (CBumpMap*)(pRenderInfo->pRenderToTexture);
					// pBumpMap->UpdateTexBuf();
					RenderSubMeshToBumpWaveCompositeTexture(pBumpMap, &_curSubMesh, pMtrl);
				}
				break;
			}
		}	
		
	}
}

VOID CScene::SetRoot(CRoot *pRoot)
{
	m_pRoot = pRoot;

	m_GUIManager.SetRoot(pRoot);
}