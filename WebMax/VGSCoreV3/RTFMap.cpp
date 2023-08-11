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

	//��ʼ��
	Init();
}

CRTFMap::~CRTFMap()
{
	Release();
}

//��ʼ����ͼ����
//��Ⱦ��������ͼ�õ����Դ���棬��ֱ��ʹ����ͼ�ı��棬
//��Ϊֱ����Ⱦ����ͼ���濹����޷�ʵ�֣�����ֻ������Ⱦ��ָ�����棬Ȼ�󿽱�����ͼ����
HRESULT	CRTFMap::Init()
{
	HRESULT hr;

	//����������ȾRTF������ͼ�ı���
	hr = m_pd3dDevice->CreateRenderTarget(m_width, m_height, m_pd3dpp->BackBufferFormat, 
										  m_pd3dpp->MultiSampleType, m_pd3dpp->MultiSampleQuality,
										  FALSE, &m_pRenderTarget, NULL);

	//����������ȾRTF������ͼ��ZBuffer
	hr = m_pd3dDevice->CreateDepthStencilSurface(m_width, m_height, m_pd3dpp->AutoDepthStencilFormat, 
											     m_pd3dpp->MultiSampleType, m_pd3dpp->MultiSampleQuality,
												 FALSE, &m_pZBuf, NULL);
	if (FAILED(hr)) return E_FAIL;

	//��������RTF������ͼ���Դ��е���ͼ�������
	hr = D3DXCreateTexture(m_pd3dDevice, m_width, m_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5/*m_format*/, D3DPOOL_DEFAULT, &m_textureBuf);
	if (FAILED(hr)) return E_FAIL;

	return S_OK;
}

//ͨ����ͷ��������÷���ƽ��
VOID CRTFMap::SetRefPlaneByModel(D3DXVECTOR3 position, D3DXVECTOR3 normal)
{
	//���÷���ƽ��
	D3DXPlaneFromPointNormal(&m_refPlane, &position, &normal);
	//���㷴�����
	D3DXMatrixReflect(&m_refMatrix, &m_refPlane);
}

//ͨ��ģ�����÷���ƽ��
VOID CRTFMap::SetRefPlaneByModel(DWORD modelHandle)
{
	m_pRefPlaneModel = (CModel*)modelHandle;

	//�������Y�����������ϵ����
	D3DXVECTOR3 normal;
	D3DXVec3TransformNormal(&normal, &D3DXVECTOR3(0, 1, 0), &m_pRefPlaneModel->m_pNode->getMatrix(VTS_WORLD));
	//���÷���ƽ��
	D3DXPlaneFromPointNormal(&m_refPlane, &m_pRefPlaneModel->boundingSphere.Center, &normal);
	//���㷴�����
	D3DXMatrixReflect(&m_refMatrix, &m_refPlane);
}

//Ϊ������ͼ���ӷ���ģ��
VOID CRTFMap::AddRefModel(DWORD modelHandle)
{
	m_refModelList.push_back(modelHandle);
}

//����Ⱦ�����ͼ�񿽱�����ͼ����
VOID CRTFMap::UpdateTexBuf()
{
	LPDIRECT3DSURFACE9 pDestTexSurface;
	HRESULT hr = m_textureBuf->GetSurfaceLevel(0, &pDestTexSurface);
	hr = D3DXLoadSurfaceFromSurface(pDestTexSurface, NULL, NULL, m_pRenderTarget, NULL, NULL, D3DX_FILTER_NONE, 0); 

// 	D3DXSaveSurfaceToFile("C:\\1.jpg", D3DXIFF_JPG, pDestTexSurface, NULL, NULL);

	SAFE_RELEASE_D3D(pDestTexSurface);
}

//��ձ���
VOID CRTFMap::Clear()
{
	//�ȵõ���ǰ��ȾĿ�����
	LPDIRECT3DSURFACE9 pBackBuf, pZBuf;
	m_pd3dDevice->GetRenderTarget(0, &pBackBuf);
	m_pd3dDevice->GetDepthStencilSurface(&pZBuf);

	//��ձ���
	m_pd3dDevice->SetRenderTarget(0, m_pRenderTarget);
	m_pd3dDevice->SetDepthStencilSurface(m_pZBuf);
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1.0f, 0 ); 

	//���ûص�ԭ����Ⱦ����------------------------------------------------------------
	m_pd3dDevice->SetRenderTarget(0, pBackBuf);
	m_pd3dDevice->SetDepthStencilSurface(pZBuf);

	SAFE_RELEASE_D3D(pBackBuf);
	SAFE_RELEASE_D3D(pZBuf);
}

//�ͷ�
VOID CRTFMap::Invalidate()
{
	SAFE_RELEASE_D3D(m_pRenderTarget);
	SAFE_RELEASE_D3D(m_pZBuf);
	SAFE_RELEASE_D3D(m_textureBuf);
}

//����
HRESULT	CRTFMap::Restore(D3DXMATRIX texProj)
{	
	m_texProjMatrix = texProj;
	return Init();
}

//����
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
