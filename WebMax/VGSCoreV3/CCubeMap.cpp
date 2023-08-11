//----------------------------------------------------------------------------------
//CEnvMap.cpp 实现文件
//描述：环境贴图
//作者: zyx
//日期: 2007-1-12
//-----------------------------------------------------------------------------------
#include "CSysFunc.h"
#include "CCubeMap.h"
#include "CModel.h"
#include "VGSRender.h" 
#include "VGSSupport.h"
#include "Root.h"

//构造函数
CCubeMap::CCubeMap(LPDIRECT3DDEVICE9 pDevice, INT mode, D3DFORMAT format, INT width, INT mipCount)
{
	m_textureType = TLT_CUBEMAP;

	//获得场景设备
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

	//初始化CubeMap
	if (m_Mode == 0)
		InitStaticCubeMap();
	else
		InitDynamicCubeMap();
}

CCubeMap::~CCubeMap()
{
}

//-----------------------------------------------------------------------------
// 方法名: InitStaticCubeMap()
// 参  数: 
// 用  途: 初始化静态CubeMap
//-----------------------------------------------------------------------------
HRESULT CCubeMap::InitStaticCubeMap()
{
	return D3DXCreateCubeTexture(m_pd3dDevice, m_Size, m_MipCount, 0, m_Format, D3DPOOL_MANAGED, &m_pCubeTex);
}

//-----------------------------------------------------------------------------
// 方法名: FillImageToCubeTexBuffer()
// 参  数: INT faceIndex, DWORD dataSize, BYTE* pData
// 用  途: 填充图像到贴图缓冲
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
	
	//判断是否6副图已经创建完毕
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
// 方法名: InitDynamicCubeMap()
// 参  数: 
// 用  途: 初始化动态CubeMap
//-----------------------------------------------------------------------------
HRESULT CCubeMap::InitDynamicCubeMap()
{
	// Create the cubemap, with a format that matches the backbuffer
	//必须用D3DUSAGE_RENDERTARGET，因为要动态生成CubeMap
	//必须用D3DPOOL_DEFAULT，因为该表面用于D3DUSAGE_RENDERTARGET
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
// 方法名: AddEnvModel()
// 参  数: DWORD modelHandle
// 用  途: 增加反射模型到CubeMap
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
// 方法名: Release()
// 参  数: 
// 用  途: 释放内存及相关资源
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