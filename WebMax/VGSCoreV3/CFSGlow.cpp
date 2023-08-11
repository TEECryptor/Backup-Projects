//----------------------------------------------------------------------------------
//CFSGlow.cpp ʵ���ļ�
//������ȫ��������Ч
//����: ����
//����: 2007-04-11
//-----------------------------------------------------------------------------------

#include "CFSGlow.h"

struct FSGLOW_CUSTOMVERTEX
{
	FLOAT			x, y, z;         //x,y,z����
	FLOAT			u, v;            //��ͼ����
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

CFSGlow::CFSGlow(LPDIRECT3DDEVICE9	pd3dDevice, RECT* pRect,
											D3DPRESENT_PARAMETERS* pd3dpp,
											D3DXMATRIX*	pMatWorld,
											D3DXMATRIX* pMatView,
											D3DXMATRIX* pMatProj)
{
	m_pd3dDevice = pd3dDevice;
	m_pRect = pRect;
	m_pd3dpp = pd3dpp,
	m_pMatWorld = pMatWorld;
	m_pMatView = pMatView;
	m_pMatProj = pMatProj;
    D3DXMatrixIdentity(&m_MatIdentity);

	m_pCurrentBuffer = NULL;
	m_pCurrentRenderTexture = NULL;
	m_pCurrentRenderSurface = NULL;
	m_pDisplayZSurface = NULL;
	m_pZBuffer = NULL;
	m_pVB = NULL;

	m_bGlowFlag = FALSE;
	m_bSupport = TRUE;
	m_GlowAmount = 100;
}

CFSGlow::~CFSGlow()
{
}

//���ù��ε�ǿ��
VOID CFSGlow::SetGlowAmount(INT amount)
{
	INT value = (INT)(amount /100.0f * 256);
	if (value < 5) value = 5;
	if (value > 250) value = 250;
	m_GlowAmount = 256 - value;
}

VOID  CFSGlow::SetupNewMatrices()
{
	/*
    m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_MatIdentity );
    D3DXMatrixLookAtLH( &m_MatView, &D3DXVECTOR3( 0.0f, 0.0f,-5.0f ),
								  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
								  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_MatView );

    D3DXMatrixPerspectiveFovLH( &m_MatProj, 
								D3DX_PI/8, 
								1.0f * (m_pRect->right - m_pRect->left)/(m_pRect->bottom - m_pRect->top), 
								3.0f, 
								100.0f );
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_MatProj );	
	*/

    m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_MatIdentity );
    D3DXMatrixLookAtLH( &m_MatView, &D3DXVECTOR3( 0.0f, 0.0f,-1.0f ),
								  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
								  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
    m_pd3dDevice->SetTransform( D3DTS_VIEW, &m_MatView );

	D3DXMatrixOrthoLH(&m_MatProj, 
		1.0f * (m_pRect->right - m_pRect->left),  
		1.0f * (m_pRect->bottom - m_pRect->top), 
		-10000000.0f,
		100000000.0f);
    m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &m_MatProj );	

}

//�ͷŶ��㻺��
VOID CFSGlow::Invalidate()
{
	//�ͷŶ��㻺��
	SAFE_RELEASE_D3D(m_pCurrentBuffer);
	SAFE_RELEASE_D3D(m_pCurrentRenderTexture);
	SAFE_RELEASE_D3D(m_pCurrentRenderSurface);
	SAFE_RELEASE_D3D(m_pDisplayZSurface);
	SAFE_RELEASE_D3D(m_pZBuffer);
	SAFE_RELEASE_D3D(m_pVB);
}

//�������㻺��
BOOL CFSGlow::CreateVB()
{
	//FLOAT ratio = 1.0f * (m_pRect->right - m_pRect->left) / (m_pRect->bottom - m_pRect->top);
	FLOAT width = 1.0f * (m_pRect->right + m_pRect->left);
	FLOAT height = 1.0f* (m_pRect->top + m_pRect->bottom);
	//���ζ�������,��������Ϻ��������ʾ����Ļ
	FSGLOW_CUSTOMVERTEX m_Vertices[] =
	{
		//  x           y        z	    u     v    
		{ -width/2, -height/2, 0.0f, 0.0f, 1.0f},
		{ +width/2, -height/2, 0.0f, 1.0f, 1.0f},
		{ +width/2, height/2, 0.0f, 1.0f, 0.0f},
		{ -width/2, height/2, 0.0f, 0.0f, 0.0f},
	};

	//�������㻺��
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( 4 * sizeof(FSGLOW_CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVB, NULL) ) )
    {
        return FALSE;
    }
	
    //��䶥�㻺��.
    VOID* pVertices;
    if( FAILED( m_pVB->Lock( 0, sizeof(m_Vertices), (VOID**)&pVertices, 0 ) ) )
        return FALSE;
    memcpy( pVertices, m_Vertices, sizeof(m_Vertices) );
    m_pVB->Unlock();

	return TRUE;
}

BOOL CFSGlow::InitEffect()
{
	//������Ϊ��ȾĿ�������
	if (FAILED(m_pd3dDevice->CreateTexture(m_pRect->right - m_pRect->left, 
								m_pRect->bottom - m_pRect->top, 
								1, D3DUSAGE_RENDERTARGET, 
								m_pd3dpp->BackBufferFormat, 
								D3DPOOL_DEFAULT, 
								&m_pCurrentRenderTexture, NULL)))
	{
		m_bSupport = FALSE;
		return FALSE;
	}
	//����������
	m_pCurrentRenderTexture->GetSurfaceLevel(0, &m_pCurrentRenderSurface);
	
	//������Ȼ������
	if (FAILED(m_pd3dDevice->CreateDepthStencilSurface(m_pRect->right - m_pRect->left,  
							m_pRect->bottom - m_pRect->top, 
							D3DFMT_D16,
							D3DMULTISAMPLE_NONE, 
							0,
							FALSE,
							&m_pDisplayZSurface,
							NULL)))
		return FALSE;
	//�����Ȼ���
	if (FAILED(m_pd3dDevice->GetDepthStencilSurface(&m_pZBuffer)))
		return FALSE;

	//�������㻺��
	if (!CreateVB())
		return FALSE;
	
	//�״�����ʱ��պ󻺴�,��Ϊ����Ⱦʱ�������
	m_pd3dDevice->GetRenderTarget(1, &m_pCurrentBuffer);
    m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0L );
	
	return TRUE;
}

VOID CFSGlow::SetRenderTarget()
{
	if (!m_bSupport) return;

	//�ж���Ч�Ƿ��
	if(m_bGlowFlag)
	{
		//��Ⱦԭʼ�������������
		m_pd3dDevice->SetRenderTarget(0, m_pDisplayZSurface);	
	}
	else
		m_pd3dDevice->SetRenderTarget(0, m_pZBuffer);		

	//m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
	//					 0xff000000, 1.0f, 0L );
}

//��Ⱦ��Ч
VOID CFSGlow::RenderEffect()
{
	if (!m_bSupport) return;

	//set tex-0, tex-1.. tex-8
	for (INT i = 0; i< 8; i++)
	{
		m_pd3dDevice->SetTexture(i, NULL);
		m_pd3dDevice->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_pd3dDevice->SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}

	//�ڴ˻�ԭʼ����,�������ֵ���Ļ
	//�ж���Ч�Ƿ��
	if(m_bGlowFlag)
	{
		//����ǰ����ͺ󻺴����Alpha���,����պ󻺴�,�ۻ���Ⱦ���������Ļ
		if(SUCCEEDED(m_pd3dDevice->BeginScene()))
		{
			m_pd3dDevice->SetRenderTarget(0, m_pZBuffer);
		
			m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
								 0xff000000, 1.0f, 0L );

			// Setup the world, view, and projection matrices
			SetupNewMatrices();

			//����Diffuse��ͼ�İ�����ʽ
			m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			m_pd3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
			//����Diffuse��ͼ�ı任
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

			//������ͼ���˾�
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC );
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC );
			m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 2);

			//�رյƹ�,ʹ�ö����������ɫ
			m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
			
			//��Alpha���
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);	

			//���ո���Ⱦ�ĳ�����Ϊ0ͨ����ͼ
			m_pd3dDevice->SetTexture(0, m_pCurrentRenderTexture);
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);

			INT level = 4;
			// Render the vertex buffer contents
			for (INT i = 1; i<= level; i++)
			{
				INT step = 78;//(i + 1);
				//����ALPHA͸����
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
				m_pd3dDevice->SetRenderState(D3DRS_TEXTUREFACTOR, step<<24);
				m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);

				VOID* pVertices;
				if( FAILED( m_pVB->Lock( 0, 4 * sizeof(FSGLOW_CUSTOMVERTEX), (VOID**)&pVertices, 0 ) ) )
					return;

				//ƫ����
				FLOAT offset = 0.005f;
				//vertex 0
				((FSGLOW_CUSTOMVERTEX*)pVertices)[0].u = 0.0f + i * offset;
				((FSGLOW_CUSTOMVERTEX*)pVertices)[0].v = 1.0f - i * offset;

				//vertex 1
				((FSGLOW_CUSTOMVERTEX*)pVertices)[1].u = 1.0f - i * offset;
				((FSGLOW_CUSTOMVERTEX*)pVertices)[1].v = 1.0f - i * offset;

				//vertex 2
				((FSGLOW_CUSTOMVERTEX*)pVertices)[2].u = 1.0f - i * offset;
				((FSGLOW_CUSTOMVERTEX*)pVertices)[2].v = 0.0f + i * offset;

				//vertex 3
				((FSGLOW_CUSTOMVERTEX*)pVertices)[3].u = 0.0f + i * offset;
				((FSGLOW_CUSTOMVERTEX*)pVertices)[3].v = 0.0f + i * offset;

				m_pVB->Unlock();

				m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(FSGLOW_CUSTOMVERTEX) );
				m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
				m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2 );
			}

			// End the scene
			m_pd3dDevice->EndScene();

			//�ָ�Ĭ����Ⱦ״̬
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE );
			m_pd3dDevice->SetTexture(0, NULL);
			//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			
			//�ָ�ԭʼ����
			m_pd3dDevice->SetTransform(D3DTS_WORLD, m_pMatWorld);
			m_pd3dDevice->SetTransform(D3DTS_VIEW, m_pMatView);
			m_pd3dDevice->SetTransform(D3DTS_PROJECTION, m_pMatProj);
		}
	}
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

VOID CFSGlow::Release()
{
	//�ͷŸ�����Դ
	if (m_pVB != NULL)
	{
		m_pVB->Release();
		m_pVB = NULL;
	}
	if (m_pCurrentRenderSurface != NULL)
	{
		m_pCurrentRenderSurface->Release();
		m_pCurrentRenderSurface = NULL;
	}
	if (m_pCurrentRenderTexture != NULL)
	{
		m_pCurrentRenderTexture->Release();
		m_pCurrentRenderTexture = NULL;
	}
	if (m_pCurrentBuffer != NULL)
	{
		m_pCurrentBuffer->Release();
		m_pCurrentBuffer = NULL;
	}
	if (m_pDisplayZSurface != NULL)
	{
		m_pDisplayZSurface->Release();
		m_pDisplayZSurface = NULL;
	}
	if (m_pZBuffer != NULL)
	{
		m_pZBuffer->Release();
		m_pZBuffer = NULL;
	}	
}
