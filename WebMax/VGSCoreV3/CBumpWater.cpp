#include "CBumpWater.h"

CBumpWater::CBumpWater(LPDIRECT3DDEVICE9 pD3DDevice)
{
	pBumpTex = NULL;
	m_pd3dDevice = pD3DDevice;
	index = -1;
	curRenderTexture  = NULL;             //当前后缓冲对应的渲染到纹理(Render-to-Texture)
	curRenderSurface = NULL; 
	curRenderZSurface = NULL;
	multiLayer = FALSE; 
}

CBumpWater::~CBumpWater()
{
}

BOOL CBumpWater::CreateBumpMap(INT dwWidth, 
							   INT dwHeight,
							   INT xSize,
							   INT ySize,
							   FLOAT radius,
							   FLOAT speed)
{
	width = dwWidth;
	height = dwHeight;
	m_xSize = xSize;
	m_ySize = ySize;
	m_radius = radius;
	m_speed = speed;

    // Create the bump map texture
    if( FAILED( m_pd3dDevice->CreateTexture( dwWidth, dwHeight, 1, 0 ,
                                             D3DFMT_V8U8, D3DPOOL_MANAGED,
                                             &pBumpTex, NULL ) ) )
        return FALSE;

    // Lock the surface and write in some bumps for the waves
    D3DLOCKED_RECT d3dlr;
    pBumpTex->LockRect( 0, &d3dlr, 0, 0 );
    CHAR* pDst = (CHAR*)d3dlr.pBits;
    CHAR  iDu, iDv;

    for( INT y=0; y<dwHeight; y++ )
    {
        CHAR* pPixel = pDst;
        for( INT x=0; x<dwWidth; x++ )
        {
            
			FLOAT fx = x/(FLOAT)dwWidth; //- 0.5f;//中心点的位置
            FLOAT fy = y/(FLOAT)dwHeight;//-0.7f;//- 0.5f;

            FLOAT r = sqrtf( fx*fx + fy*fy );//r表示到圆心的距离

			iDu  = (CHAR)( 64 * cosf( 300.0f * r ) * expf( -r * 5.0f ) );
            iDu += (CHAR)( 32 * cosf( 150.0f * ( fx + fy ) ) );
            iDu += (CHAR)( 8 * cosf( 140.0f * ( fx * 0.85f - fy ) ) );

            iDv  = (CHAR)( 64 * sinf( 300.0f * r ) * expf( -r * 5.0f ) );
            iDv += (CHAR)( 32 * sinf( 150.0f * ( fx + fy ) ) );
            iDv += (CHAR)( 8 * sinf( 140.0f * ( fx * 0.85f - fy ) ) );

            *pPixel++ = xSize * iDu;
            *pPixel++ = ySize * iDv;
			//加入luminance
			/* WORD uL = (1-iDu) * 255 ;
			*pPixel++ = (CHAR)uL;
            *pPixel++ = (CHAR)0L;*/
        }
        pDst += d3dlr.Pitch;
    }
    pBumpTex->UnlockRect(0);

	return TRUE;
}
BOOL CBumpWater::CreateMultiLayerRenderTarget(D3DPRESENT_PARAMETERS	d3dpp, UINT width, UINT height)
{
	if(curRenderTexture == NULL)
	{
		//创建D3D材质
		if(FAILED(m_pd3dDevice->CreateTexture(width, 
			height, 
			1, D3DUSAGE_RENDERTARGET, 
			d3dpp.BackBufferFormat, 
			D3DPOOL_DEFAULT, 
			&curRenderTexture, NULL)))
			return FALSE;
		//创建渲染到材质表面
		curRenderTexture->GetSurfaceLevel(0,&curRenderSurface);
		//创建深度缓存表面
		if (FAILED(m_pd3dDevice->CreateDepthStencilSurface(width,  
			height, 
			D3DFMT_D16,
			/*D3DMULTISAMPLE_NONE*/d3dpp.MultiSampleType,
			0,
			FALSE,
			&curRenderZSurface, NULL)))
			return FALSE;
	}
	return TRUE;
}
//释放因bumpwater材质因为多层而添加的特定混色渲染表面
VOID CBumpWater::ReleaseMultiLayerRenderTarget()
{
	SAFE_RELEASE_D3D(curRenderTexture);             //当前后缓冲对应的渲染到纹理(Render-to-Texture)
	SAFE_RELEASE_D3D(curRenderSurface); 
	SAFE_RELEASE_D3D(curRenderZSurface);
}
//释放资源
VOID CBumpWater::Release()
{
	SAFE_RELEASE_D3D(pBumpTex);
	ReleaseMultiLayerRenderTarget();
	
}
