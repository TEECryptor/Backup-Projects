//----------------------------------------------------------------------------------
//CEnvMap.cpp ʵ���ļ�
//������������ͼ
//����: zyx
//����: 2007-1-12
//-----------------------------------------------------------------------------------
#include "CSysFunc.h"
#include "CCubeMap.h"
#include "CModel.h"
#include "VGSRender.h" 
#include "VGSSupport.h"
#include "Root.h"

//���캯��
CCubeMap::CCubeMap(LPDIRECT3DDEVICE9 pDevice, INT mode, D3DFORMAT format, INT width, INT mipCount)
{
	m_textureType = TLT_CUBEMAP;

	//��ó����豸
	m_pd3dDevice = pDevice;
	m_pCubeTex = NULL;
	m_pZBuffer = NULL;
	
	m_Mode = mode;
	m_Format = format;
	m_Size = width;
	m_MipCount = mipCount;

	isRendering = FALSE;
	bCreated = FALSE;
	createdTexCount = 0;

	m_ReflectionModels.clear();
	m_vecFileName.clear();

	//��ʼ��CubeMap
	if (m_Mode == 0)
		InitStaticCubeMap();
	else
		InitDynamicCubeMap();
}

CCubeMap::~CCubeMap()
{
}

//-----------------------------------------------------------------------------
// ������: InitStaticCubeMap()
// ��  ��: 
// ��  ;: ��ʼ����̬CubeMap
//-----------------------------------------------------------------------------
HRESULT CCubeMap::InitStaticCubeMap()
{
	return D3DXCreateCubeTexture(m_pd3dDevice, m_Size, m_MipCount, 0, m_Format, D3DPOOL_MANAGED, &m_pCubeTex);
}

//-----------------------------------------------------------------------------
// ������: FillImageToCubeTexBuffer()
// ��  ��: INT faceIndex, DWORD dataSize, BYTE* pData
// ��  ;: ���ͼ����ͼ����
//-----------------------------------------------------------------------------
HRESULT	CCubeMap::FillImageToCubeTexBuffer(INT faceIndex, DWORD dataSize, BYTE* pData)
{
	HRESULT hr;
	INT totallevels = m_pCubeTex->GetLevelCount();
	for (INT miplevel = 0; miplevel< totallevels; miplevel++)
	{
		LPDIRECT3DSURFACE9 pDestCubeMapSurface;
		m_pCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)faceIndex, miplevel, &pDestCubeMapSurface);
		hr = D3DXLoadSurfaceFromFileInMemory(pDestCubeMapSurface, NULL, NULL,
											  pData, dataSize, 
											  NULL, D3DX_DEFAULT, 0, NULL);

		SAFE_RELEASE_D3D(pDestCubeMapSurface);
	}
	
	//�ж��Ƿ�6��ͼ�Ѿ��������
	createdTexCount++;
	if (createdTexCount == 6)
		bCreated = true;

	return hr;
}
HRESULT	 CCubeMap::FillImagesToTexture(const std::vector<std::string>& vecFileName)
{
	if (vecFileName.size() != 6)
		return E_FAIL;
	createdTexCount = 0;
	for (std::size_t i = 0; i < 6; ++i)
	{
		CImage *pImage = new CImage();
		if (pImage)
		{
			_loadImageFromFile(vecFileName[i], pImage);
			FillImageToCubeTexBuffer(i, pImage->dataSize, pImage->data);
			SAFE_RELEASE(pImage);
			SAFE_DELETE(pImage);
		}
	}
	m_vecFileName = vecFileName;
	return S_OK;
}
const char*	CCubeMap::GetImageByIndex(size_t nIdx)
{
	if (nIdx <0 || nIdx >= m_vecFileName.size())
		return "";
	return m_vecFileName[nIdx].c_str();
}


//-----------------------------------------------------------------------------
// ������: InitDynamicCubeMap()
// ��  ��: 
// ��  ;: ��ʼ����̬CubeMap
//-----------------------------------------------------------------------------
HRESULT CCubeMap::InitDynamicCubeMap()
{
	// Create the cubemap, with a format that matches the backbuffer
	//������D3DUSAGE_RENDERTARGET����ΪҪ��̬����CubeMap
	//������D3DPOOL_DEFAULT����Ϊ�ñ�������D3DUSAGE_RENDERTARGET
	if( FAILED( D3DXCreateCubeTexture( m_pd3dDevice, m_Size, 1,
									   D3DUSAGE_RENDERTARGET, m_Format, D3DPOOL_DEFAULT, 
									   &m_pCubeTex ) ) )
		return E_FAIL;

	
	// We create a separate Z buffer for the cube faces, because user could 
	// resize rendering window so that it is smaller than a cube face. In 
	// this case we cannot use the rendering window Z buffer for cube faces.
	if( FAILED(m_pd3dDevice->CreateDepthStencilSurface(m_Size, m_Size, 
														D3DFMT_D24S8, 
														/*g_VGSRender*/GetRoot()->getVGSRender().d3dParam.MultiSampleType/*D3DMULTISAMPLE_NONE*/,
														0,
														FALSE, 
														&m_pZBuffer, NULL) ) )
		return E_FAIL;

	return S_OK;
}


//-----------------------------------------------------------------------------
// ������: AddEnvModel()
// ��  ��: DWORD modelHandle
// ��  ;: ���ӷ���ģ�͵�CubeMap
//-----------------------------------------------------------------------------
VOID CCubeMap::AddEnvModel(DWORD modelHandle)
{
	CModel* pModel = (CModel*)modelHandle;
	if (pModel == NULL) return;

	pModel->addCubemap(this);	
	m_ReflectionModels.push_back(modelHandle);
}

VOID CCubeMap::removeEnvModel(DWORD modelHandle)
{
	CModel* pModel = (CModel*)modelHandle;
	if (pModel == NULL) return;

	pModel->removeCubemap(this);
	// pModel->useMeCubeMap.push_back(this);

	vector<DWORD>::iterator it = std::find(m_ReflectionModels.begin(), m_ReflectionModels.end(), modelHandle);
	if (it != m_ReflectionModels.end())
		m_ReflectionModels.erase(it);
}
DWORD CCubeMap::GetEnvModelHandle(size_t nIdx)
{
	if (nIdx <0 || nIdx >= m_ReflectionModels.size())
		return 0;
	return m_ReflectionModels[nIdx];
}
void CCubeMap::RemoveAllEnvModels()
{
	for (size_t i = 0; i <m_ReflectionModels.size(); i++ )
	{
		CModel* pModel = (CModel*)m_ReflectionModels[i];
		if (pModel != NULL) 
			pModel->removeCubemap(this);
	}
	m_ReflectionModels.clear();
}

//-----------------------------------------------------------------------------
// ������: Release()
// ��  ��: 
// ��  ;: �ͷ��ڴ漰�����Դ
//-----------------------------------------------------------------------------
VOID CCubeMap::Release()
{
	SAFE_RELEASE_D3D(m_pCubeTex);
	SAFE_RELEASE_D3D(m_pZBuffer);
}

// std::string CCubeMap::getTextrueFileName(int index/* = 0*/)
// {
// 
// }