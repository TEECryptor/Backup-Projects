// BumpMap.cpp: implementation of the CBumpMap class.
//
//////////////////////////////////////////////////////////////////////

#include "BumpMap.h"
#include "VGSSupport.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBumpMap::CBumpMap(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dpp, D3DFORMAT format)//, BUMPMAPTYPE type, D3DFORMAT format, INT width)
{
	m_textureType = TLT_BUMPMAP;
    m_pd3dDevice = pd3dDevice;
	m_pd3dpp = pd3dpp;
	m_format = D3DFMT_R8G8B8;	

	m_pBumpEnvMap = NULL;       //环境凹凸贴图
	m_pNormalMap = NULL;		//法线贴图
	m_pBumpWaveMap = NULL;
    m_BumpMapFormat = format;
	m_Light = NULL;
	m_compositeTexForBump = NULL;

	m_type = VBT_BUMPWATER;  // 缺省值

	// bump water的初始值
	m_MapWidth  = 128;
	m_MapHeight = 128;
	m_xSize     = 5;
	m_ySize     = 5;
	m_radius    = 1;
	m_speed     = 0.5;
	m_CenterX   = 0;
	m_CenterY   = 0;

	m_Noise1    = 0.5;
	m_Noise2    = 0.5;

	m_pMaterial = NULL;

	// 初始化合成渲染贴图
	InitCompositeRenderTarget();
}

CBumpMap::~CBumpMap()
{
	Release();
}

HRESULT CBumpMap::SetEnvironmentMapNoise(float fNoise1, float fNoise2)
{
	if (NULL != m_textureBuf && NULL != m_pBumpEnvMap)
	{
		m_Noise1 = fNoise1;
		m_Noise2 = fNoise2;
		return S_OK;
	}
	return E_FAIL;
}	
HRESULT CBumpMap::ChangeEnvironmentMapImageFile(const std::string &sFileName, float noise1, float noise2)
{
	// 如果贴图变了，需要初始化
	if (!m_pBumpEnvMap || strcmp(sFileName.c_str(), this->m_sFilePathandName.c_str()) != 0)
	{
		SAFE_RELEASE_D3D(m_pBumpEnvMap);
		return InitBumpEnvMap(sFileName, noise1, noise2);
	}
	return E_FAIL;
}

// 设置bump wave的参数
HRESULT CBumpMap::SetBumpwaveParam(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY)
{
	if (!m_pBumpWaveMap || dwWidth != m_MapWidth || dwHeight != m_MapHeight)
	{
		SAFE_RELEASE_D3D(m_pBumpWaveMap);
		return InitBumpWaveMap(dwWidth, dwHeight, xSize, ySize, radius, speed, centerX, centerY);
	}
	else if (m_pBumpWaveMap)
	{
		m_MapWidth  = dwWidth;
		m_MapHeight = dwHeight;
		m_xSize     = xSize;
		m_ySize     = ySize;
		m_radius    = radius;
		m_speed     = speed;
		m_CenterX   = centerX;
		m_CenterY   = centerY;	
		 // 计算bumpwave
		ComputeBumpWavemap();
	}

	return S_OK;
}


HRESULT CBumpMap::SetBumpNormalParam(const std::string &sFileName, CLight* pLight)
{
	if (!m_pNormalMap || strcmp(sFileName.c_str(), this->m_sFilePathandName.c_str()) != 0)
	{
		SAFE_RELEASE_D3D(m_pNormalMap);
		return InitBumpNormalMap(sFileName, pLight);
	}
	else if (m_pNormalMap)
	{
		m_Light = pLight;
//注释下面一行，这里不需要重新计算		
// 		ComputeBumpNormalmap(); // 计算bump
	}

	return S_OK;
}

//更新波纹贴图
VOID CBumpMap::UpdateBumpWaveMap(INT stageIndex)
{
	DWORD _timer = GetTickCount();
	FLOAT r = 0.08f / m_radius;
	FLOAT multiply = 0.005f * m_speed;

	D3DXMATRIX _mat;
	float value0 = _timer * multiply;
	float value1 = r * cosf(value0);
	float value2 = r * sinf(value0);
	_mat._11 = value1;
	_mat._12 = -value2;
	_mat._21 = value2;
	_mat._22 = value1;

	m_pd3dDevice->SetTextureStageState(stageIndex, D3DTSS_BUMPENVMAT00, F2DW( _mat._11 ) );
	m_pd3dDevice->SetTextureStageState(stageIndex, D3DTSS_BUMPENVMAT01, F2DW( _mat._12 ) );
	m_pd3dDevice->SetTextureStageState(stageIndex, D3DTSS_BUMPENVMAT10, F2DW( _mat._21 ) );
	m_pd3dDevice->SetTextureStageState(stageIndex, D3DTSS_BUMPENVMAT11, F2DW( _mat._22 ) );
}


//设置计算法线贴图凹凸效果使用的光源
VOID CBumpMap::SetBumpMapLight(CLight* pLight)
{
	m_Light = pLight;
}

//光照方向（对点光源可为光源位置到模型中心的向量）转化为颜色
DWORD CBumpMap::VectortoRGBA(FLOAT fHeight)
{		
	if (m_Light)
	{
		if (VLT_OMNI == m_Light->getType())
		{
			//计算光源位置到模型中心的向量
			if (m_pMaterial)
			{
				if (NULL != m_pMaterial->m_pCurModel)
				{
					//使用模型的渲染方法
					D3DXVec3Normalize(&m_LightDir, &(*m_Light->getPosition() - m_pMaterial->m_pCurModel->m_pNode->getPosition(VTS_WORLD)));
				}
				else if (m_pMaterial->subMeshList.size() > 0)
				{	
					//以材质分组进行渲染时
					CModel *pModel = (CModel*)(m_pMaterial->subMeshList[0].modelPointer);
					D3DXVec3Normalize(&m_LightDir, &(*m_Light->getPosition() - pModel->m_pNode->getPosition(VTS_WORLD)));
				}
			}
		}
		else
		{
			D3DXVec3Normalize(&m_LightDir, m_Light->getDirection());
		}
	}

    DWORD r = (DWORD)( 127.0f * m_LightDir.x + 128.0f );
    DWORD g = (DWORD)( 127.0f * m_LightDir.y + 128.0f );
    DWORD b = (DWORD)( 127.0f * m_LightDir.z + 128.0f );
    DWORD a = (DWORD)( 255.0f * fHeight );
    
	m_dwFactor = (a<<24L) + (r<<16L) + (g<<8L) + (b<<0L);
    return m_dwFactor;
}

//将渲染表面的图像拷贝到贴图表面
VOID CBumpMap::UpdateTexBuf()
{
	LPDIRECT3DSURFACE9 pDestTexSurface;
	HRESULT hr = m_compositeTexForBump->GetSurfaceLevel(0, &pDestTexSurface);
	hr = D3DXLoadSurfaceFromSurface(pDestTexSurface, NULL, NULL, m_pRenderTarget, NULL, NULL, D3DX_FILTER_NONE, 0); 

	SAFE_RELEASE_D3D(pDestTexSurface);
}

//清空表面
VOID CBumpMap::Clear()
{
	//先得到当前渲染目标表面
	LPDIRECT3DSURFACE9 pBackBuf;
	m_pd3dDevice->GetRenderTarget(0, &pBackBuf);

	//清空表面
	m_pd3dDevice->SetRenderTarget(0, m_pRenderTarget);
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_TARGET, 0, 1.0f, 0 ); 

	//设置回的原有渲染表面------------------------------------------------------------
	m_pd3dDevice->SetRenderTarget(0, pBackBuf);

	SAFE_RELEASE_D3D(pBackBuf);
}

HRESULT CBumpMap::SetBumpAsWavemap(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY)
{
	if (m_type == VBT_BUMPWATER && m_pBumpWaveMap)
	{
		return SetBumpwaveParam(dwWidth, dwHeight, xSize, ySize, radius, speed, centerX, centerY);
	}

	SAFE_RELEASE_D3D(m_pBumpEnvMap);
	SAFE_RELEASE_D3D(m_pNormalMap);
	SAFE_RELEASE_D3D(m_pBumpWaveMap); 

	m_type = VBT_BUMPWATER;
	return InitBumpWaveMap(dwWidth, dwHeight, xSize, ySize, radius, speed, centerX, centerY);
}

HRESULT CBumpMap::SetBumpAsEnviromentmap(const std::string &sFileName, float noise1, float noise2)
{
	// 如果已经是环境反射类型，并且对象已经存在，就不需要初始化了
	if (m_type == VBT_BUMPENVIRONMENT  && m_pBumpEnvMap)
	{
		return ChangeEnvironmentMapImageFile(sFileName, noise1, noise2);
	}

	SAFE_RELEASE_D3D(m_pBumpEnvMap);
	SAFE_RELEASE_D3D(m_pNormalMap);
	SAFE_RELEASE_D3D(m_pBumpWaveMap); 

	m_type = VBT_BUMPENVIRONMENT;
	return InitBumpEnvMap(sFileName, noise1, noise2);
}

HRESULT CBumpMap::SetBumpAsNormalmap(const std::string &sFileName, CLight* pLight)
{
	if (m_type == VBT_BUMPNORMARLMAP  && m_pNormalMap)
	{
		return SetBumpNormalParam(sFileName, pLight);
	}
	
	SAFE_RELEASE_D3D(m_pBumpEnvMap);
	SAFE_RELEASE_D3D(m_pNormalMap);
	SAFE_RELEASE_D3D(m_pBumpWaveMap); 
	
	m_type = VBT_BUMPNORMARLMAP;
	return InitBumpNormalMap(sFileName, pLight);
}

//释放 
VOID CBumpMap::Invalidate()
{
	SAFE_RELEASE_D3D(m_pRenderTarget);
	SAFE_RELEASE_D3D(m_compositeTexForBump);
}

//重置
VOID CBumpMap::Restore(D3DXMATRIX matrix)
{
	m_compositeTexPorjMatrix = matrix;
	InitCompositeRenderTarget();
}

VOID CBumpMap::Release()
{
	SAFE_RELEASE_D3D(m_textureBuf);
	SAFE_DELETE_ARRAY(buffer);

	SAFE_RELEASE_D3D(m_pBumpEnvMap);
	SAFE_RELEASE_D3D(m_pNormalMap);
	SAFE_RELEASE_D3D(m_pBumpWaveMap); 
	SAFE_RELEASE_D3D(m_compositeTexForBump);
	SAFE_RELEASE_D3D(m_pRenderTarget);	
}



////////////////////////////// private //////////////////////////////////////////


//创建用于合成其它图层的表面和贴图
HRESULT CBumpMap::InitCompositeRenderTarget()
{
	HRESULT hr;
	
	//创建用于渲染合成贴图的表面
	hr = m_pd3dDevice->CreateRenderTarget(m_pd3dpp->BackBufferWidth, m_pd3dpp->BackBufferHeight, m_pd3dpp->BackBufferFormat, 
		m_pd3dpp->MultiSampleType, m_pd3dpp->MultiSampleQuality,
		FALSE, &m_pRenderTarget, NULL);
	if (FAILED(hr)) return E_FAIL;
	
	//创建用于合成贴图的显存中的贴图缓存对象
	hr = D3DXCreateTexture(m_pd3dDevice, m_pd3dpp->BackBufferWidth, m_pd3dpp->BackBufferHeight, 1, D3DUSAGE_RENDERTARGET,
		m_pd3dpp->BackBufferFormat, D3DPOOL_DEFAULT, &m_compositeTexForBump);
	if (FAILED(hr)) return E_FAIL;
	
	return S_OK;
}
HRESULT CBumpMap::CreateBumpBaseTexture(const std::string& strFileName)
{
	if (strFileName.empty())
		return E_FAIL;
	CImage *pImage = new CImage();
	if (pImage)
	{
		_loadImageFromFile(strFileName, pImage);

		SAFE_RELEASE_D3D(m_textureBuf);

		if ( FAILED(D3DXCreateTextureFromFileInMemoryEx(m_pd3dDevice,
			pImage->data,
			pImage->dataSize,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1,
			0,
			m_format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			NULL,
			&imageInfo,
			NULL,
			&m_textureBuf) ) )
		return E_FAIL;

		width  = imageInfo.Width;
		height = imageInfo.Height;
		MipCount = 1;

		pImage->Release();
		SAFE_DELETE(pImage);

		m_sFilePathandName = strFileName;

		return S_OK;
	}
	return E_FAIL;
}

//初始化BumpEnv贴图
HRESULT	CBumpMap::InitBumpEnvMap(const std::string &sFileName, float noise1, float noise2)
{
	if (FAILED(CreateBumpBaseTexture(sFileName)))
		return E_FAIL;		

	m_Noise1 = noise1;
	m_Noise2 = noise2;

	LPDIRECT3DTEXTURE9 psBumpSrc = m_textureBuf;
    D3DSURFACE_DESC    d3dsd;

    psBumpSrc->GetLevelDesc( 0, &d3dsd );
    // Create the bumpmap's surface and texture objects
    if( FAILED( m_pd3dDevice->CreateTexture( d3dsd.Width, d3dsd.Height, 1, 0, 
        m_BumpMapFormat, D3DPOOL_MANAGED, &m_pBumpEnvMap, NULL ) ) )
    {
        return E_FAIL;
    }

	ComputeBumpEnviromentmap();    

    return S_OK;
}

// 计算环境贴图
VOID CBumpMap::ComputeBumpEnviromentmap()
{
	LPDIRECT3DTEXTURE9 psBumpSrc = m_textureBuf;

	D3DSURFACE_DESC    d3dsd;
    D3DLOCKED_RECT     d3dlr;
	psBumpSrc->GetLevelDesc( 0, &d3dsd );

	// Fill the bits of the new texture surface with bits from
    // a private format.
    psBumpSrc->LockRect( 0, &d3dlr, 0, 0 );
    DWORD dwSrcPitch = (DWORD)d3dlr.Pitch;
    BYTE* pSrcTopRow = (BYTE*)d3dlr.pBits;
    BYTE* pSrcCurRow = pSrcTopRow;
    BYTE* pSrcBotRow = pSrcTopRow + (dwSrcPitch * (d3dsd.Height - 1) );
	
    m_pBumpEnvMap->LockRect( 0, &d3dlr, 0, 0 );
    DWORD dwDstPitch = (DWORD)d3dlr.Pitch;
    BYTE* pDstTopRow = (BYTE*)d3dlr.pBits;
    BYTE* pDstCurRow = pDstTopRow;
    BYTE* pDstBotRow = pDstTopRow + (dwDstPitch * (d3dsd.Height - 1) );
	
    for( DWORD y=0; y<d3dsd.Height; y++ )
    {
        BYTE* pSrcB0; // addr of current pixel
        BYTE* pSrcB1; // addr of pixel below current pixel, wrapping to top if necessary
        BYTE* pSrcB2; // addr of pixel above current pixel, wrapping to bottom if necessary
        BYTE* pDstT;  // addr of dest pixel;
		
        pSrcB0 = pSrcCurRow;
		
        if( y == d3dsd.Height - 1)
            pSrcB1 = pSrcTopRow;
        else
            pSrcB1 = pSrcCurRow + dwSrcPitch;
		
        if( y == 0 )
            pSrcB2 = pSrcBotRow;
        else
            pSrcB2 = pSrcCurRow - dwSrcPitch;
		
        pDstT = pDstCurRow;
		
        for( DWORD x=0; x<d3dsd.Width; x++ )
        {
            LONG v00; // Current pixel
            LONG v01; // Pixel to the right of current pixel, wrapping to left edge if necessary
            LONG vM1; // Pixel to the left of current pixel, wrapping to right edge if necessary
            LONG v10; // Pixel one line below.
            LONG v1M; // Pixel one line above.
			
            v00 = *(pSrcB0+0);
            
            if( x == d3dsd.Width - 1 )
                v01 = *(pSrcCurRow);
            else
                v01 = *(pSrcB0+4);
            
            if( x == 0 )
                vM1 = *(pSrcCurRow + (4 * (d3dsd.Width - 1)));
            else
                vM1 = *(pSrcB0-4);
            v10 = *(pSrcB1+0);
            v1M = *(pSrcB2+0);
			
            LONG iDu = (vM1-v01); // The delta-u bump value
            LONG iDv = (v1M-v10); // The delta-v bump value
			
            // The luminance bump value (land masses are less shiny)
            WORD uL = ( v00>1 ) ? 63 : 127;
			
            switch( m_BumpMapFormat )
            {
			case D3DFMT_V8U8:
				*pDstT++ = (BYTE)iDu;
				*pDstT++ = (BYTE)iDv;
				break;
				
			case D3DFMT_L6V5U5:
				*(WORD*)pDstT  = (WORD)( ( (iDu>>3) & 0x1f ) <<  0 );
				*(WORD*)pDstT |= (WORD)( ( (iDv>>3) & 0x1f ) <<  5 );
				*(WORD*)pDstT |= (WORD)( ( ( uL>>2) & 0x3f ) << 10 );
				pDstT += 2;
				break;
				
			case D3DFMT_X8L8V8U8:
				*pDstT++ = (BYTE)iDu;
				*pDstT++ = (BYTE)iDv;
				*pDstT++ = (BYTE)uL;
				*pDstT++ = (BYTE)0L;
				break;
            }
			
            // Move one pixel to the right (src is 32-bpp)
            pSrcB0+=4;
            pSrcB1+=4;
            pSrcB2+=4;
        }
		
        // Move to the next line
        pSrcCurRow += dwSrcPitch;
        pDstCurRow += dwDstPitch;
    }
	
    m_pBumpEnvMap->UnlockRect(0);
    psBumpSrc->UnlockRect(0);
}

//初始化Bump法线贴图
HRESULT	CBumpMap::InitBumpNormalMap(const std::string &sFileName, CLight* pLight)
{
	if (FAILED(CreateBumpBaseTexture(sFileName)))
		return E_FAIL;		

	m_pNormalMap=m_textureBuf;
	m_Light = pLight;

//注释下面，直接使用一张法线图.---added by linxq --2009-6-10
// 	//从源图创建法线贴图
//     if(FAILED(D3DXCreateTexture(m_pd3dDevice, width, height, 
// 								0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pNormalMap)))
//         return E_FAIL;
//     
// 	ComputeBumpNormalmap();

	return S_OK;
}

VOID CBumpMap::ComputeBumpNormalmap()
{
	if (m_pNormalMap && m_textureBuf)
    {
		D3DXComputeNormalMap(m_pNormalMap, m_textureBuf, NULL, 0, D3DX_CHANNEL_RED, /*10*/1.0f);
	}
}

//初始化Bump波纹贴图
HRESULT	CBumpMap::InitBumpWaveMap(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY)
{
	m_MapWidth  = dwWidth;
	m_MapHeight = dwHeight;
	m_xSize     = xSize;
	m_ySize     =  ySize;
	m_radius    = radius;
	m_speed     = speed;
	m_CenterX   = centerX;
	m_CenterY   = centerY;
	
    // Create the bump map texture
    if (FAILED( m_pd3dDevice->CreateTexture( dwWidth, dwHeight, 1, 0,
		D3DFMT_V8U8, D3DPOOL_MANAGED,
		&m_pBumpWaveMap, NULL)))
    {
		return FALSE;
	}

	ComputeBumpWavemap();   
	
	return S_OK;
}

// 计算bump wave
VOID CBumpMap::ComputeBumpWavemap()
{
	// Lock the surface and write in some bumps for the waves
    D3DLOCKED_RECT d3dlr;
    m_pBumpWaveMap->LockRect( 0, &d3dlr, 0, 0 );
    CHAR* pDst = (CHAR*)d3dlr.pBits;
    CHAR  iDu, iDv;
	
	CHAR* pPixel = NULL;
	float fWidth = (float)m_MapWidth, fHeight = (float)m_MapHeight;
	float fx = 0.0f, fy = 0.0f, r = 0.0f;

    for( INT y=0; y<m_MapHeight; y++ )
    {
        pPixel = pDst;
        for( INT x=0; x<m_MapWidth; x++ )
        {
            fx = x/fWidth - m_CenterX;//中心点的位置
            fy = y/fHeight - m_CenterY;
			
            r = sqrtf( fx*fx + fy*fy );//r表示到圆心的距离
			
			iDu  = (CHAR)( 128 * cosf( 300.0f * r ) * expf( -r * 5.0f ) );
            iDu += (CHAR)( 64 * cosf( 150.0f * ( fx + fy ) ) );
            iDu += (CHAR)( 16 * cosf( 140.0f * ( fx * 0.85f - fy ) ) );
			
            iDv  = (CHAR)( 128 * sinf( 300.0f * r ) * expf( -r * 5.0f ) );
            iDv += (CHAR)( 64 * sinf( 150.0f * ( fx + fy ) ) );
            iDv += (CHAR)( 16 * sinf( 140.0f * ( fx * 0.85f - fy ) ) );
			
            *pPixel++ = m_xSize * iDu;
            *pPixel++ = m_ySize * iDv;
        }
        pDst += d3dlr.Pitch;
    }
    m_pBumpWaveMap->UnlockRect(0);
}