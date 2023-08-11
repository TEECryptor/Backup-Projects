// VGSRender.cpp: implementation of the CVGSRender class.
//
//////////////////////////////////////////////////////////////////////

#include "VGSRender.h"
#include "CD3DDevice.h"

// CVGSRender g_VGSRender;  // ȫ�ֵ���Ⱦ������

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC72901AD
CVGSRender::CVGSRender()
{
	m_pd3d = NULL;							//Direct3D����
	m_pd3dDevice = NULL;					//D3D Device����

	m_DeviceInfo = NULL;					//��ǰϵͳ�������豸��Ϣ
	m_pBackBuffer = NULL;					//�Կ��󻺳����
	m_pZBuffer = NULL;	

	m_antiLevel = 0;

	m_pFont = NULL;

	m_BrowserType = WBT_IE;
	m_BrowserSubtype = WBST_IE;

}

//##ModelId=47EBC72901AE
CVGSRender::~CVGSRender()
{

}

//-----------------------------------------------------------------------------
// ������: InitD3DDevice()
// ��  ��: 
// ��  ��: ��ʼ��D3D Deviceʱ��ı��豸ʱ����
//-----------------------------------------------------------------------------
//##ModelId=47EBC72901AA
HRESULT CVGSRender::InitD3DDevice()
{		
	//-------------------------׼������D3D �豸-------------------------------------------------------
	//�����豸��Ϣ����
	m_DeviceInfo = new CD3DDevice(m_pd3d);
	m_DeviceInfo->GetAllAdapterAbility();
	m_DeviceInfo->ChooseBestD3DParam(&d3dParam);

	// ff�������Ҫ�ı䶨�����ľ�������
	if (m_BrowserType == WBT_FF)  // firefox
	{
		d3dParam.VertexProcessType = d3dParam.VertexProcessType | D3DCREATE_FPU_PRESERVE/*���ı䵥���Ȼ���˫��������*/;
	}
	else
	{
		d3dParam.VertexProcessType = d3dParam.VertexProcessType & ~D3DCREATE_FPU_PRESERVE/*���ı䵥���Ȼ���˫��������*/;
	}

	//����D3D����ĳ��ֲ��������ڴ���D3D�豸
    ZeroMemory(&m_d3dpp, sizeof(m_d3dpp) );
	m_d3dpp.Windowed = TRUE;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_pd3d->GetAdapterDisplayMode( 0, &m_d3dmm);
	m_d3dpp.BackBufferFormat = m_d3dmm.Format; //DX8�������������ΪD3DFMT_UNKNOWN�����򴴽��豸����
	m_d3dpp.MultiSampleType = d3dParam.MultiSampleType;
	
	//RECT rect;
	//GetClientRect(m_hWnd, &rect);
	//m_d3dpp.BackBufferWidth = rect.right - rect.left;
	//m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
	m_d3dpp.BackBufferWidth  = m_d3dmm.Width;  // ��ȫ���ߴ紴��
	m_d3dpp.BackBufferHeight = m_d3dmm.Height;
    
    m_d3dpp.EnableAutoDepthStencil = TRUE;
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.hDeviceWindow = m_hWnd;	

	//����D3D�豸
	DWORD Err = m_pd3d->CreateDevice(D3DADAPTER_DEFAULT, d3dParam.DevType, m_hWnd, 
		d3dParam.VertexProcessType, 
		&m_d3dpp, &m_pd3dDevice);



	//�ж��豸�����Ƿ��д������
	switch (Err)
	{
	case D3DERR_INVALIDCALL:
		MessageBox(NULL, TEXT("The method call is invalid. For example, a method's parameter may have an invalid value."),
			TEXT("����D3D�豸����"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	case D3DERR_NOTAVAILABLE:
		MessageBox(NULL, TEXT("This device does not support the queried technique."),
			TEXT("����D3D�豸����"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	case D3DERR_OUTOFVIDEOMEMORY:
		MessageBox(NULL, TEXT("Direct3D does not have enough display memory to perform the operation."),
			TEXT("����D3D�豸����"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	case D3D_OK:
		break;
	default:
		return E_FAIL;
	}	



	//set backcull visible
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//Turn on the zbuffer
    m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//set render state
	m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	//set fill mode
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//set texture format-tex0
	for (INT i = 0; i< 8; i++)
	{
		m_pd3dDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		m_pd3dDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		m_pd3dDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
		m_pd3dDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 2);
	}

	//�ر���Ч
	m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	
	//Ϊ��������Ĭ�ϵ�����
	m_pFont = new CFont(m_pd3dDevice);
	m_pFont->Init("����");

	return S_OK;
	
}

//��ʼ��d3d�豸����Դ
//##ModelId=47EBC72901A8
HRESULT CVGSRender::Init(HWND hWnd, WEBBROWSERTYPE type/* = WBT_IE*/)
{
	//������
	m_hWnd = hWnd;

	m_BrowserType = type;

	//����D3D����
	m_pd3d = Direct3DCreate9(D3D_SDK_VERSION);
    if( m_pd3d == NULL )
	{
		MessageBox(NULL, TEXT("����D3D�������"), TEXT("����D3D�������"), MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
		
	//��ʼ��D3D�豸
	if (FAILED (InitD3DDevice()) ) 
		return E_FAIL;

	return S_OK;
}

//������Ⱦ��
//##ModelId=47EBC72901AB
HRESULT CVGSRender::Update(CViewport *pViewport)
{	
	if (!pViewport)
	{
		return E_FAIL;
	}
	RECT rect;
	pViewport->GetRect(rect);
	// GetClientRect(m_hWnd, &rect);

	m_d3dpp.BackBufferWidth = rect.right - rect.left;
	m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
	m_d3dpp.BackBufferWidth = rect.right - rect.left;
	m_d3dpp.BackBufferHeight = rect.bottom - rect.top;
	m_d3dpp.Windowed = TRUE;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.MultiSampleType = d3dParam.MultiSampleType;
	m_d3dpp.hDeviceWindow = m_hWnd;
	if (m_d3dpp.BackBufferWidth == 0 || m_d3dpp.BackBufferHeight == 0)
	{
		return E_FAIL;
	}
	else
	{
		m_pd3dDevice->Reset(&m_d3dpp);	
		
		//set backcull visible
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
		//Turn on the zbuffer
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		//set render state
		m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

		//set texture format-tex0
		for (INT i = 0; i< 8; i++)
		{
			m_pd3dDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
			m_pd3dDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
			m_pd3dDevice->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
			m_pd3dDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 2);
		}
	}
	return S_OK;
}

//�ͷ�d3d�豸����Դ
//##ModelId=47EBC72901AC
VOID CVGSRender::Release()
{
	//delete pFont
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pFont);
	
	//release DeviceInfo Obj
	SAFE_RELEASE(m_DeviceInfo);
	SAFE_DELETE(m_DeviceInfo);
	
    SAFE_RELEASE_D3D(m_pBackBuffer);
	SAFE_RELEASE_D3D(m_pZBuffer);

	//�ͷ�D3D�����ڴ�
	SAFE_RELEASE_D3D(m_pd3dDevice);
	SAFE_RELEASE_D3D(m_pd3d);
}

// �Ƿ�֧��cubemap
BOOL CVGSRender::IsSupportCUBEMAP()
{
	if (m_DeviceInfo)
	{
		return d3dParam.IsSupportCUBEMAP;
	}
	return FALSE;
}

