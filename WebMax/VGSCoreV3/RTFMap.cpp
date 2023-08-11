// RTFMap.cpp: implementation of the CRTFMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RTFMap.h"

#include "CModel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTFMap::CRTFMap(const std::string & name, LPDIRECT3DDEVICE9 pd3dDevice, 
				 D3DPRESENT_PARAMETERS* pd3dpp, INT width, INT height, D3DFORMAT format):
	m_pd3dDevice(pd3dDevice)
{
	m_sName = name;
	m_pd3dpp = pd3dpp;
	m_width = width;
	m_height = height;
	m_format = format;
	m_textureBuf = NULL;

	m_pRefPlaneModel = 0;
	m_pMaterial = NULL;

	//初始化
	Init();
}

CRTFMap::~CRTFMap()
{
	Release();
}

//初始化贴图对象
//渲染本反射贴图用到的显存表面，不直接使用贴图的表面，
//因为直接渲染到贴图表面抗锯齿无法实现，所以只能先渲染到指定表面，然后拷贝到贴图表面
HRESULT	CRTFMap::Init()
{
	HRESULT hr;

	//创建用于渲染RTF反射贴图的表面
	hr = m_pd3dDevice->CreateRenderTarget(m_width, m_height, m_pd3dpp->BackBufferFormat, 
										  m_pd3dpp->MultiSampleType, m_pd3dpp->MultiSampleQuality,
										  FALSE, &m_pRenderTarget, NULL);

	//创建用于渲染RTF反射贴图的ZBuffer
	hr = m_pd3dDevice->CreateDepthStencilSurface(m_width, m_height, m_pd3dpp->AutoDepthStencilFormat, 
											     m_pd3dpp->MultiSampleType, m_pd3dpp->MultiSampleQuality,
												 FALSE, &m_pZBuf, NULL);
	if (FAILED(hr)) return E_FAIL;

	//创建用于RTF反射贴图的显存中的贴图缓存对象
	hr = D3DXCreateTexture(m_pd3dDevice, m_width, m_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5/*m_format*/, D3DPOOL_DEFAULT, &m_textureBuf);
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

//通过点和方向量设置反射平面
VOID CRTFMap::SetRefPlaneByModel(D3DXVECTOR3 position, D3DXVECTOR3 normal)
{
	//设置反射平面
	D3DXPlaneFromPointNormal(&m_refPlane, &position, &normal);
	//计算反射矩阵
	D3DXMatrixReflect(&m_refMatrix, &m_refPlane);
}

//通过模型设置反射平面
VOID CRTFMap::SetRefPlaneByModel(DWORD modelHandle)
{
	m_pRefPlaneModel = (CModel*)modelHandle;

	//获得物体Y轴的世界坐标系向量
	D3DXVECTOR3 normal;
	D3DXVec3TransformNormal(&normal, &D3DXVECTOR3(0, 1, 0), &m_pRefPlaneModel->m_pNode->getMatrix(VTS_WORLD));
	//设置反射平面
	D3DXPlaneFromPointNormal(&m_refPlane, &m_pRefPlaneModel->boundingSphere.Center, &normal);
	//计算反射矩阵
	D3DXMatrixReflect(&m_refMatrix, &m_refPlane);
}

//为反射贴图增加反射模型
VOID CRTFMap::AddRefModel(DWORD modelHandle)
{
	m_refModelList.push_back(modelHandle);
}

//将渲染表面的图像拷贝到贴图表面
VOID CRTFMap::UpdateTexBuf()
{
	LPDIRECT3DSURFACE9 pDestTexSurface;
	HRESULT hr = m_textureBuf->GetSurfaceLevel(0, &pDestTexSurface);
	hr = D3DXLoadSurfaceFromSurface(pDestTexSurface, NULL, NULL, m_pRenderTarget, NULL, NULL, D3DX_FILTER_NONE, 0); 

// 	D3DXSaveSurfaceToFile("C:\\1.jpg", D3DXIFF_JPG, pDestTexSurface, NULL, NULL);

	SAFE_RELEASE_D3D(pDestTexSurface);
}

//清空表面
VOID CRTFMap::Clear()
{
	//先得到当前渲染目标表面
	LPDIRECT3DSURFACE9 pBackBuf, pZBuf;
	m_pd3dDevice->GetRenderTarget(0, &pBackBuf);
	m_pd3dDevice->GetDepthStencilSurface(&pZBuf);

	//清空表面
	m_pd3dDevice->SetRenderTarget(0, m_pRenderTarget);
	m_pd3dDevice->SetDepthStencilSurface(m_pZBuf);
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1.0f, 0 ); 

	//设置回的原有渲染表面------------------------------------------------------------
	m_pd3dDevice->SetRenderTarget(0, pBackBuf);
	m_pd3dDevice->SetDepthStencilSurface(pZBuf);

	SAFE_RELEASE_D3D(pBackBuf);
	SAFE_RELEASE_D3D(pZBuf);
}

//释放
VOID CRTFMap::Invalidate()
{
	SAFE_RELEASE_D3D(m_pRenderTarget);
	SAFE_RELEASE_D3D(m_pZBuf);
	SAFE_RELEASE_D3D(m_textureBuf);
}

//重置
HRESULT	CRTFMap::Restore(D3DXMATRIX texProj)
{	
	m_texProjMatrix = texProj;
	return Init();
}

//销毁
VOID CRTFMap::Release()
{
	SAFE_RELEASE_D3D(m_pRenderTarget);
	SAFE_RELEASE_D3D(m_pZBuf);
	SAFE_RELEASE_D3D(m_textureBuf);
	m_refModelList.clear();
}
DWORD CRTFMap::GetReflectedModelByIndex(size_t idx)
{
	if (idx >= m_refModelList.size())
		return 0;
	return m_refModelList[idx];
}
void CRTFMap::SetSize(int nWidth, int nHeight)
{
	if (m_textureBuf && (m_width != nWidth || m_height != nHeight))
	{
		m_width = nWidth;
		m_height = nHeight;

		Release();
		
		Init();
	}
}
