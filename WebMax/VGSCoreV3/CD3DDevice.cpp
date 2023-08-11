// CD3DDevice.cpp: implementation of the CDetectDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "CD3DDevice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD3DDevice::CD3DDevice(LPDIRECT3D9 pd3d)
{
	m_pd3d = pd3d;
	pAdapters = NULL;
}

CD3DDevice::~CD3DDevice()
{

}

//-----------------------------------------------------------------------------
// 方法名: GetAllAdapterAbility()
// 参  数: 
// 用  途: 得到当前系统的所有显示设备参数
//-----------------------------------------------------------------------------
HRESULT CD3DDevice::GetAllAdapterAbility()
{
	//先得到设备数量
	adapterCnt = m_pd3d->GetAdapterCount();
	//初始化设备列表
	if (adapterCnt > 0)
		pAdapters = new _ADAPTER[adapterCnt];
	else
		return E_FAIL;
	
	for (UINT i = 0; i< adapterCnt; i++)
	{
		//硬件驱动信息
		m_pd3d->GetAdapterIdentifier(i, 0, &(pAdapters[i].HardDriveInfo));

		//显示模式
		m_pd3d->GetAdapterDisplayMode(i, &(pAdapters[i].DisplayMode));

		//显示器
		pAdapters[i].Moniter = m_pd3d->GetAdapterMonitor(i);

		//HAL设备能力
		ZeroMemory(&(pAdapters[i].HALCap), sizeof(pAdapters[i].HALCap));
		m_pd3d->GetDeviceCaps(i, D3DDEVTYPE_HAL, &(pAdapters[i].HALCap));

		//REF设备能力
		ZeroMemory(&(pAdapters[i].REFCap), sizeof(pAdapters[i].REFCap));
		m_pd3d->GetDeviceCaps(i, D3DDEVTYPE_REF, &(pAdapters[i].REFCap));

		//只得到硬件的贴图处理能力
		pAdapters[i].MaxTextureBlendStages = pAdapters[i].HALCap.MaxTextureBlendStages;
		pAdapters[i].MaxSimultaneousTextures = pAdapters[i].HALCap.MaxSimultaneousTextures;

		//检查显示卡是否支持抗锯齿
		pAdapters[i].MultiSampleTypeCnt = 0;
		for( UINT m = 0; m <= 16; m++ )
		{
			D3DFORMAT fmt = pAdapters[i].DisplayMode.Format;

			if ( m == 1 ) // 1 is not a valid multisample type
				continue;

			if( SUCCEEDED( m_pd3d->CheckDeviceMultiSampleType( i, D3DDEVTYPE_HAL, fmt, TRUE, (D3DMULTISAMPLE_TYPE)m, NULL )))
				pAdapters[i].MultiSampleTypeCnt++;
		}
		
		//建立抗锯齿功能列表
		if (pAdapters[i].MultiSampleTypeCnt != 0)
		{
			pAdapters[i].pMultiSampleType = new D3DMULTISAMPLE_TYPE[pAdapters[i].MultiSampleTypeCnt];
			pAdapters[i].pMultiSampleQualityLevels = new DWORD[pAdapters[i].MultiSampleTypeCnt];
		}
		UINT sampleIndex = 0;
		for( m=0; m <= 16; m++ )
		{
			D3DFORMAT fmt = pAdapters[i].DisplayMode.Format;
			if ( m == 1 ) // 1 is not a valid multisample type
				continue;

			DWORD qualityLevel = 0;
			if( SUCCEEDED( m_pd3d->CheckDeviceMultiSampleType( i,
						   D3DDEVTYPE_HAL, fmt, TRUE, (D3DMULTISAMPLE_TYPE)m , &qualityLevel) ) )
			{
				pAdapters[i].pMultiSampleType[sampleIndex] = (D3DMULTISAMPLE_TYPE)m;
				pAdapters[i].pMultiSampleQualityLevels[sampleIndex] = qualityLevel - 1;
				sampleIndex++;
			}
		}
        
		//是否支持A16R16G16B16的Float类型的纹理
		if (SUCCEEDED(m_pd3d->CheckDeviceFormat( i,
                                   D3DDEVTYPE_HAL, pAdapters[i].DisplayMode.Format,
                                   0, D3DRTYPE_TEXTURE,
                                   D3DFMT_A16B16G16R16)))
			pAdapters[i].IsSupportTex64 = TRUE;
		else
			pAdapters[i].IsSupportTex64 = FALSE;

		//是否支持球型反射贴图
		if (pAdapters[i].HALCap.VertexProcessingCaps & D3DVTXPCAPS_TEXGEN_SPHEREMAP)
			pAdapters[i].IsSupportTexGenSphereMap = true;
		else
			pAdapters[i].IsSupportTexGenSphereMap = false;

		//是否支持Cube
		pAdapters[i].IsSupportCUBEMAP = ConfirmCubeMap(&pAdapters[i].HALCap);

		//是否支持动态贴图
		pAdapters[i].IsSupportDynamicTexture = ConfirmDynamicTexture(&pAdapters[i].HALCap);
		
        //确认硬件是否支持Normal贴图
		pAdapters[i].IsSupportGPUNormal = ConfirmGPUNormal(&(pAdapters[i]));

		//是否支持Bump和支持的最佳类型
		pAdapters[i].SupportBestBumpFormat = ConfirmBumpUV(&pAdapters[i].HALCap, pAdapters[i].DisplayMode.Format);

		//支持的Vertex Shader的版本号
		DWORD vsVersion = 0x0000ffff & (pAdapters[i].HALCap.VertexShaderVersion);
		int vsVersion1 = (vsVersion >> 8) & 0xff;
		int vsVersion2 = vsVersion & 0xff;
		pAdapters[i].SupportVS_Version = vsVersion1 + 0.1f * vsVersion2;

		//支持Pixel Shader的版本号
		DWORD psVersion = 0x0000ffff & (pAdapters[i].HALCap.PixelShaderVersion);
		int psVersion1 = (psVersion >> 8) & 0xff;
		int psVersion2 = psVersion & 0xff;
		pAdapters[i].SupportPS_Version = psVersion1 + 0.1f * psVersion2;

	}

	return S_OK;

}

//-----------------------------------------------------------------------------
// 方法名: ChooseBestD3DParam()
// 参  数: 
// 用  途: 得到当前最合适的设备参数
//-----------------------------------------------------------------------------
HRESULT	CD3DDevice::ChooseBestD3DParam(D3D_PARAM* pD3DParam)
{
	INT m;
	//先看当前系统支持的设备
	if (pAdapters[D3DADAPTER_DEFAULT].HALCap.DeviceType == D3DDEVTYPE_HAL)
		pD3DParam->DevType = D3DDEVTYPE_HAL;
	else
	{
		pD3DParam->DevType = D3DDEVTYPE_REF;
		//友好提示:
		MessageBox(GetActiveWindow(), TEXT("您的系统似乎还没有安装显卡驱动程序，请先安装显卡驱动程序再浏览VGS三维网页。"),
			TEXT("系统友好提示，显卡驱动程序没有安装"), MB_OK|MB_ICONEXCLAMATION);
	}

	//得到最优化的顶点处理的模式
	switch (pD3DParam->DevType)
	{
	case D3DDEVTYPE_HAL:
		//顶点处理模式
		if (pAdapters[D3DADAPTER_DEFAULT].HALCap.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		{
			if (pAdapters[D3DADAPTER_DEFAULT].HALCap.DevCaps & D3DDEVCAPS_PUREDEVICE)
			{
				pD3DParam->VertexProcessType = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
				if (FAILED (ConfirmDevice(&(pAdapters[D3DADAPTER_DEFAULT].HALCap), 
							pD3DParam->VertexProcessType, 
							pAdapters[D3DADAPTER_DEFAULT].DisplayMode.Format) ) )
				{
					pD3DParam->VertexProcessType = D3DCREATE_HARDWARE_VERTEXPROCESSING;
					if (FAILED (ConfirmDevice(&(pAdapters[D3DADAPTER_DEFAULT].HALCap), 
								pD3DParam->VertexProcessType, 
								pAdapters[D3DADAPTER_DEFAULT].DisplayMode.Format) ) )
					{
						pD3DParam->VertexProcessType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
					}
				}
			}
			else
				pD3DParam->VertexProcessType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		else
			pD3DParam->VertexProcessType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		//抗锯齿特性
		m = (pAdapters[D3DADAPTER_DEFAULT].MultiSampleTypeCnt+1)/2 -1;
		if (m > 0)
		{
			pD3DParam->MultiSampleType = pAdapters[0].pMultiSampleType[m];
			pD3DParam->MultiSampleQuality = pAdapters[0].pMultiSampleQualityLevels[m];
		}
		else
		{
			pD3DParam->MultiSampleType = D3DMULTISAMPLE_NONE;	
			pD3DParam->MultiSampleQuality = 0;
		}
		//默认关掉抗拒齿
		//pD3DParam->MultiSampleType = D3DMULTISAMPLE_NONE;
		
		pD3DParam->MaxTextureBlendStages = pAdapters[D3DADAPTER_DEFAULT].MaxTextureBlendStages;	
		pD3DParam->MaxSimultaneousTextures = pAdapters[D3DADAPTER_DEFAULT].MaxSimultaneousTextures;	

		pD3DParam->IsSupportTexGenSphereMap = pAdapters[D3DADAPTER_DEFAULT].IsSupportTexGenSphereMap;	
		pD3DParam->IsSupportCUBEMAP = pAdapters[D3DADAPTER_DEFAULT].IsSupportCUBEMAP;	
		pD3DParam->IsSupportDynamicTexture = pAdapters[D3DADAPTER_DEFAULT].IsSupportDynamicTexture;					
		pD3DParam->IsSupportTex64 = pAdapters[D3DADAPTER_DEFAULT].IsSupportTex64;				
		pD3DParam->IsSupportGPUNormal = pAdapters[D3DADAPTER_DEFAULT].IsSupportGPUNormal;	
		pD3DParam->SupportBestBumpFormat = pAdapters[D3DADAPTER_DEFAULT].SupportBestBumpFormat;				
		pD3DParam->SupportVS_Version = pAdapters[D3DADAPTER_DEFAULT].SupportVS_Version;			
		pD3DParam->SupportPS_Version = pAdapters[D3DADAPTER_DEFAULT].SupportPS_Version;
		break;

	case D3DDEVTYPE_REF:
			pD3DParam->VertexProcessType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		break;

	default:
		return E_FAIL;
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// 方法名: ConfirmDevice()
// 参  数: D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format 
// 用  途: 确认设备在某种状态下的处理能力
//-----------------------------------------------------------------------------
HRESULT CD3DDevice::ConfirmDevice( D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format )
{
    if( dwBehavior & D3DCREATE_PUREDEVICE )
        return E_FAIL; // Get Transform doesn't work on PUREDEVICE

    // This sample uses alpha textures and/or straight alpha. Make sure the
    // device supports them
    if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHAPALETTE )
        return S_OK;
    if( pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHA )
        return S_OK;

    return E_FAIL;
}

//判断是否支持GPU生成法线贴图
BOOL CD3DDevice::ConfirmGPUNormal(_ADAPTER* pAdapter)
{
	BOOL IsSupportGPUNormal = FALSE;

	std::string GPUName = (std::string)(pAdapter->HardDriveInfo.Description);
	UINT GPUType = 0;

    if (GPUName.find_first_of("NVIDIA")>=0)
        GPUType = 1;
	else if(GPUName.find_first_of("ATI")>=0)
		GPUType = 2;
	else if(GPUName.find_first_of("INTEL")>=0)
		GPUType = 3;

    INT result;
	switch (GPUType)
	{
	case 1:
		{	
			result = GPUName.find("TNT");
			if(result>= 0)
				IsSupportGPUNormal = FALSE;
			else
			{
				result = GPUName.find("GeForce FX 5");
				if( result >= 0)
					IsSupportGPUNormal = FALSE;
				else
				{
					result = GPUName.find("GeForce 7");
					if(result >= 0)			
						IsSupportGPUNormal = FALSE;
					else 
						IsSupportGPUNormal = TRUE;
				}
			}
			
		}
		break;
	case 2:
          IsSupportGPUNormal = TRUE;
		break;
	case 3:
          IsSupportGPUNormal = FALSE;
		break;
	}

    return IsSupportGPUNormal;
}

//检测是否支持Bump
D3DFORMAT CD3DDevice::ConfirmBumpUV(D3DCAPS9* pCaps, D3DFORMAT backBufFormat)
{
    //找到当前设备支持的最好的BumpFormat
    BOOL bCanDoV8U8   = SUCCEEDED( m_pd3d->CheckDeviceFormat( pCaps->AdapterOrdinal,
                                   pCaps->DeviceType, backBufFormat,
                                   0, D3DRTYPE_TEXTURE,
                                   D3DFMT_V8U8 ) ) &&
                        (pCaps->TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP );

    BOOL bCanDoL6V5U5 = SUCCEEDED( m_pd3d->CheckDeviceFormat( pCaps->AdapterOrdinal,
                                   pCaps->DeviceType, backBufFormat,
                                   0, D3DRTYPE_TEXTURE,
                                   D3DFMT_L6V5U5 ) ) &&
                        (pCaps->TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE );

    BOOL bCanDoL8V8U8 = SUCCEEDED( m_pd3d->CheckDeviceFormat( pCaps->AdapterOrdinal,
                                   pCaps->DeviceType, backBufFormat,
                                   0, D3DRTYPE_TEXTURE,
                                   D3DFMT_X8L8V8U8 ) ) &&
                        (pCaps->TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE );

    if(bCanDoV8U8 ) return D3DFMT_V8U8;
    else if(bCanDoL6V5U5 ) return D3DFMT_L6V5U5;
    else if(bCanDoL8V8U8) return D3DFMT_X8L8V8U8;
    else                    return D3DFMT_UNKNOWN;
}

//检测是否支持Cube
BOOL CD3DDevice::ConfirmCubeMap(D3DCAPS9* pCaps)
{
	//是否支持CubeMap
	if (pCaps->TextureCaps & D3DPTEXTURECAPS_CUBEMAP)
		return TRUE;
	else
		return FALSE;
}

//检测是否支持DynamicTexture
BOOL CD3DDevice::ConfirmDynamicTexture(D3DCAPS9* pCaps)
{
	//是否支持CubeMap
	if (pCaps->Caps2 & D3DCAPS2_DYNAMICTEXTURES)
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------------------------
// 方法名: Release()
// 参  数: 
// 用  途: 释放内存和相关资源
//-----------------------------------------------------------------------------
VOID CD3DDevice::Release()
{
	for (UINT i = 0; i< adapterCnt; i++)
	{
		SAFE_DELETE_ARRAY(pAdapters[i].pMultiSampleType);
		SAFE_DELETE_ARRAY(pAdapters[i].pMultiSampleQualityLevels);
	}
	SAFE_DELETE_ARRAY(pAdapters);
}