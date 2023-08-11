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
// ������: GetAllAdapterAbility()
// ��  ��: 
// ��  ;: �õ���ǰϵͳ��������ʾ�豸����
//-----------------------------------------------------------------------------
HRESULT CD3DDevice::GetAllAdapterAbility()
{
	//�ȵõ��豸����
	adapterCnt = m_pd3d->GetAdapterCount();
	//��ʼ���豸�б�
	if (adapterCnt > 0)
		pAdapters = new _ADAPTER[adapterCnt];
	else
		return E_FAIL;
	
	for (UINT i = 0; i< adapterCnt; i++)
	{
		//Ӳ��������Ϣ
		m_pd3d->GetAdapterIdentifier(i, 0, &(pAdapters[i].HardDriveInfo));

		//��ʾģʽ
		m_pd3d->GetAdapterDisplayMode(i, &(pAdapters[i].DisplayMode));

		//��ʾ��
		pAdapters[i].Moniter = m_pd3d->GetAdapterMonitor(i);

		//HAL�豸����
		ZeroMemory(&(pAdapters[i].HALCap), sizeof(pAdapters[i].HALCap));
		m_pd3d->GetDeviceCaps(i, D3DDEVTYPE_HAL, &(pAdapters[i].HALCap));

		//REF�豸����
		ZeroMemory(&(pAdapters[i].REFCap), sizeof(pAdapters[i].REFCap));
		m_pd3d->GetDeviceCaps(i, D3DDEVTYPE_REF, &(pAdapters[i].REFCap));

		//ֻ�õ�Ӳ������ͼ��������
		pAdapters[i].MaxTextureBlendStages = pAdapters[i].HALCap.MaxTextureBlendStages;
		pAdapters[i].MaxSimultaneousTextures = pAdapters[i].HALCap.MaxSimultaneousTextures;

		//�����ʾ���Ƿ�֧�ֿ����
		pAdapters[i].MultiSampleTypeCnt = 0;
		for( UINT m = 0; m <= 16; m++ )
		{
			D3DFORMAT fmt = pAdapters[i].DisplayMode.Format;

			if ( m == 1 ) // 1 is not a valid multisample type
				continue;

			if( SUCCEEDED( m_pd3d->CheckDeviceMultiSampleType( i, D3DDEVTYPE_HAL, fmt, TRUE, (D3DMULTISAMPLE_TYPE)m, NULL )))
				pAdapters[i].MultiSampleTypeCnt++;
		}
		
		//��������ݹ����б�
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
        
		//�Ƿ�֧��A16R16G16B16��Float���͵�����
		if (SUCCEEDED(m_pd3d->CheckDeviceFormat( i,
                                   D3DDEVTYPE_HAL, pAdapters[i].DisplayMode.Format,
                                   0, D3DRTYPE_TEXTURE,
                                   D3DFMT_A16B16G16R16)))
			pAdapters[i].IsSupportTex64 = TRUE;
		else
			pAdapters[i].IsSupportTex64 = FALSE;

		//�Ƿ�֧�����ͷ�����ͼ
		if (pAdapters[i].HALCap.VertexProcessingCaps & D3DVTXPCAPS_TEXGEN_SPHEREMAP)
			pAdapters[i].IsSupportTexGenSphereMap = true;
		else
			pAdapters[i].IsSupportTexGenSphereMap = false;

		//�Ƿ�֧��Cube
		pAdapters[i].IsSupportCUBEMAP = ConfirmCubeMap(&pAdapters[i].HALCap);

		//�Ƿ�֧�ֶ�̬��ͼ
		pAdapters[i].IsSupportDynamicTexture = ConfirmDynamicTexture(&pAdapters[i].HALCap);
		
        //ȷ��Ӳ���Ƿ�֧��Normal��ͼ
		pAdapters[i].IsSupportGPUNormal = ConfirmGPUNormal(&(pAdapters[i]));

		//�Ƿ�֧��Bump��֧�ֵ��������
		pAdapters[i].SupportBestBumpFormat = ConfirmBumpUV(&pAdapters[i].HALCap, pAdapters[i].DisplayMode.Format);

		//֧�ֵ�Vertex Shader�İ汾��
		DWORD vsVersion = 0x0000ffff & (pAdapters[i].HALCap.VertexShaderVersion);
		int vsVersion1 = (vsVersion >> 8) & 0xff;
		int vsVersion2 = vsVersion & 0xff;
		pAdapters[i].SupportVS_Version = vsVersion1 + 0.1f * vsVersion2;

		//֧��Pixel Shader�İ汾��
		DWORD psVersion = 0x0000ffff & (pAdapters[i].HALCap.PixelShaderVersion);
		int psVersion1 = (psVersion >> 8) & 0xff;
		int psVersion2 = psVersion & 0xff;
		pAdapters[i].SupportPS_Version = psVersion1 + 0.1f * psVersion2;

	}

	return S_OK;

}

//-----------------------------------------------------------------------------
// ������: ChooseBestD3DParam()
// ��  ��: 
// ��  ;: �õ���ǰ����ʵ��豸����
//-----------------------------------------------------------------------------
HRESULT	CD3DDevice::ChooseBestD3DParam(D3D_PARAM* pD3DParam)
{
	INT m;
	//�ȿ���ǰϵͳ֧�ֵ��豸
	if (pAdapters[D3DADAPTER_DEFAULT].HALCap.DeviceType == D3DDEVTYPE_HAL)
		pD3DParam->DevType = D3DDEVTYPE_HAL;
	else
	{
		pD3DParam->DevType = D3DDEVTYPE_REF;
		//�Ѻ���ʾ:
		MessageBox(GetActiveWindow(), TEXT("����ϵͳ�ƺ���û�а�װ�Կ������������Ȱ�װ�Կ��������������VGS��ά��ҳ��"),
			TEXT("ϵͳ�Ѻ���ʾ���Կ���������û�а�װ"), MB_OK|MB_ICONEXCLAMATION);
	}

	//�õ����Ż��Ķ��㴦���ģʽ
	switch (pD3DParam->DevType)
	{
	case D3DDEVTYPE_HAL:
		//���㴦��ģʽ
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

		//���������
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
		//Ĭ�Ϲص����ܳ�
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
// ������: ConfirmDevice()
// ��  ��: D3DCAPS9* pCaps, DWORD dwBehavior, D3DFORMAT Format 
// ��  ;: ȷ���豸��ĳ��״̬�µĴ�������
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

//�ж��Ƿ�֧��GPU���ɷ�����ͼ
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

//����Ƿ�֧��Bump
D3DFORMAT CD3DDevice::ConfirmBumpUV(D3DCAPS9* pCaps, D3DFORMAT backBufFormat)
{
    //�ҵ���ǰ�豸֧�ֵ���õ�BumpFormat
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

//����Ƿ�֧��Cube
BOOL CD3DDevice::ConfirmCubeMap(D3DCAPS9* pCaps)
{
	//�Ƿ�֧��CubeMap
	if (pCaps->TextureCaps & D3DPTEXTURECAPS_CUBEMAP)
		return TRUE;
	else
		return FALSE;
}

//����Ƿ�֧��DynamicTexture
BOOL CD3DDevice::ConfirmDynamicTexture(D3DCAPS9* pCaps)
{
	//�Ƿ�֧��CubeMap
	if (pCaps->Caps2 & D3DCAPS2_DYNAMICTEXTURES)
		return TRUE;
	else
		return FALSE;
}

//-----------------------------------------------------------------------------
// ������: Release()
// ��  ��: 
// ��  ;: �ͷ��ڴ�������Դ
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