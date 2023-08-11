// CDynamicTex.cpp: implementation of the CDynamicTex class.
//
//////////////////////////////////////////////////////////////////////

#include "CDynamicTex.h"

using std::vector;
using std::string;

#include "VGSSupport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74E0243
CDynamicTex::CDynamicTex(LPDIRECT3DDEVICE9 pd3dDevice,int nWidth, int nHeight, D3DFORMAT fmt , int framecount)
:m_pd3dDevice(pd3dDevice)
{ 
	m_textureType = TLT_DYNAMIC;

	width = nWidth;
	height = nHeight;
	m_format = fmt;
	isDynamic = TRUE;

	m_vecTextures.clear();
	m_pCurTexture = NULL;
	m_vecFileName.clear();

	m_nCurFrame = 0;
	m_nLstFrame = 0;
	m_bPlay = true;
	m_bLoop = true;
	m_nCreatedCount = 0;
	m_nFPS = 30;
	m_fDuration = 3;

	m_frameCnt = framecount ;


	//初始化
	Init();
}

CDynamicTex::~CDynamicTex()
{
	Release();
}

void CDynamicTex::Init()
{
	//创建显存中的贴图缓存对象
	HRESULT hr = D3DXCreateTexture(m_pd3dDevice, width, height, 1, D3DUSAGE_DYNAMIC, m_format, D3DPOOL_DEFAULT, &m_pCurTexture);
	if (FAILED(hr)) 
		return;

	//创建内存中的贴图缓存对象
 	for (int i = 0; i< m_frameCnt; i++)
 	{ 
 		LPDIRECT3DTEXTURE9 pSysMemTexBuf;
 		hr = D3DXCreateTexture(m_pd3dDevice, width, height, 1, 0, m_format, D3DPOOL_SYSTEMMEM, &pSysMemTexBuf);
 		if (FAILED(hr)) 
 			return;
 		m_vecTextures.push_back(pSysMemTexBuf);
 	}

}

//填充图像到贴图缓冲
HRESULT CDynamicTex::FillImageToTextureBuffer(size_t imageIndex, DWORD dataSize, BYTE* pData)
{	
	if (imageIndex < 0 || imageIndex >= m_vecTextures.size())
		return E_FAIL;

	LPDIRECT3DSURFACE9	pTexSurface;
	m_vecTextures[imageIndex]->GetSurfaceLevel(0, &pTexSurface);
	HRESULT hr = D3DXLoadSurfaceFromFileInMemory(pTexSurface, NULL, NULL,
		pData, dataSize, 
		NULL, D3DX_DEFAULT, 0, NULL);
	SAFE_RELEASE_D3D(pTexSurface);

	//判断是否所有的贴图已经创建完成
	m_nCreatedCount++;
	if (m_nCreatedCount == GetImageCount())
		bCreated = TRUE;

	return hr;
}

//改变贴图到某一帧
HRESULT CDynamicTex::ChangeFrameTexture(INT frameIndex)
{
	//从内存拷贝到显存
	LPDIRECT3DSURFACE9	pSrcTexSurface, pDestTexSurface;
	m_vecTextures[frameIndex]->GetSurfaceLevel(0, &pSrcTexSurface);
	m_pCurTexture->GetSurfaceLevel(0, &pDestTexSurface);
	HRESULT hr = D3DXLoadSurfaceFromSurface(pDestTexSurface, NULL, NULL, pSrcTexSurface, NULL, NULL, D3DX_FILTER_NONE, 0); 
	if (FAILED(hr))
		return E_FAIL;

	SAFE_RELEASE_D3D(pSrcTexSurface);
	SAFE_RELEASE_D3D(pDestTexSurface);

	return S_OK;
}

//更新贴图
void CDynamicTex::Update()
{
	if (m_vecTextures.empty() || !m_bPlay) 
		return;
	static DWORD dwLastTime = ::GetTickCount();
	DWORD dwCurTime = ::GetTickCount();
	if ((dwCurTime - dwLastTime) >= m_nFPS)
	{
		if (m_nCurFrame < GetImageCount() - 1)
			m_nCurFrame++;
		else
		{
			if (m_bLoop)
				m_nCurFrame = 0;
		}

		if (m_nLstFrame != m_nCurFrame)
			ChangeFrameTexture(m_nCurFrame);

		m_nLstFrame = m_nCurFrame;
		dwLastTime = GetTickCount();
	}
}

//播放
void CDynamicTex::Play()
{
	if (GetImageCount() == 0) 
		return;
	m_bPlay = true;
}

//暂停
void CDynamicTex::Pause()
{
	if (GetImageCount() == 0) 
		return;

	m_bPlay = false;
}

//停止
void CDynamicTex::Stop()
{
	if (GetImageCount() == 0) return;

	ChangeFrameTexture(0);
	m_bPlay = false;
	m_nCurFrame = 0;
}

//释放贴图缓冲
void CDynamicTex::Invalidate()
{
	SAFE_RELEASE_D3D(m_pCurTexture);
}

//重置
HRESULT CDynamicTex::Restore()
{
	//创建显存中的贴图缓存对象
	return D3DXCreateTexture(m_pd3dDevice, width, height, 1, D3DUSAGE_DYNAMIC, m_format, D3DPOOL_DEFAULT, &m_pCurTexture);
}

//释放资源
void CDynamicTex::Release()
{
	//释放动态贴图缓存对象
	SAFE_RELEASE_D3D(m_pCurTexture);

	//动态贴图的贴图队列，在内存中
	vector<LPDIRECT3DTEXTURE9>::iterator it = m_vecTextures.begin(), end = m_vecTextures.end();
	for (; it != end; ++it)
	{
		SAFE_RELEASE_D3D((*it));
	}	
	m_vecTextures.clear();
}

std::string CDynamicTex::GetFileName(size_t nIdx)
{
	if (nIdx < 0 || nIdx >= m_vecFileName.size())
		return "";
	return m_vecFileName[nIdx];
}
HRESULT	CDynamicTex::ChangeImage(size_t nIdx, const std::string & strFileName)
{
	if (nIdx < 0 || nIdx >= m_vecFileName.size() || strFileName.empty())
		return E_FAIL;
	CImage *pImage = new CImage();
	if (pImage)
	{
		_loadImageFromFile(strFileName, pImage);
		FillImageToTextureBuffer(nIdx, pImage->dataSize, pImage->data);
		m_vecFileName[nIdx] = strFileName;
		pImage->Release();
		SAFE_DELETE(pImage);
		return S_OK;
	}
	return E_FAIL;
}
HRESULT	CDynamicTex::AddImage(const std::string & strFileName)
{
	if (strFileName.empty())
		return E_FAIL;
	LPDIRECT3DTEXTURE9 pSysMemTexBuf;
	if (FAILED(D3DXCreateTexture(m_pd3dDevice, width, height, 1, 0, m_format, D3DPOOL_SYSTEMMEM, &pSysMemTexBuf))) 
		return E_FAIL;
	m_vecTextures.push_back(pSysMemTexBuf);
	m_vecFileName.push_back(strFileName);

	CImage *pImage = new CImage();
	if (pImage)
	{
		_loadImageFromFile(strFileName, pImage);
		FillImageToTextureBuffer(m_vecTextures.size() - 1, pImage->dataSize, pImage->data);
		pImage->Release();
		SAFE_DELETE(pImage);
		return S_OK;
	}
	return E_FAIL;
}
bool CDynamicTex::DeleteImage(size_t nIdx)
{
	if (nIdx < 0 || nIdx >= m_vecFileName.size())
		return false;
	SAFE_RELEASE_D3D(m_vecTextures[nIdx]);
	m_vecTextures.erase(m_vecTextures.begin() + nIdx);
	m_vecFileName.erase(m_vecFileName.begin() + nIdx);
	return true;
}

void CDynamicTex::ClearImages()
{
	vector<LPDIRECT3DTEXTURE9>::iterator it = m_vecTextures.begin(), end = m_vecTextures.end();
	for (; it != end; ++it)
	{
		SAFE_RELEASE_D3D((*it));
	}	
	m_vecTextures.clear();
	m_vecFileName.clear();
}
