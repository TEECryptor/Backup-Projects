/////////////////////////////////////////////////////////////////////////
// ��  ��: CBumpWater
// ��  ��: ˮ��Ч�࣬ͨ������CBumpWater�����ʵ��������ˮ�����Ч
// ��  ��: lixin
// ��  ��: 2006-11-20
/////////////////////////////////////////////////////////////////////////

#ifndef _CBUMPWATER_H
#define _CBUMPWATER_H

#include "VGSHeader.h"
#include "CSysFunc.h"


inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
#define _T(x)       __T(x)
#define __T(x)      x

#define D3DAPPERR_MEDIANOTFOUND       0x8200000b


class VGS_EXPORT CBumpWater  
{
private:
	LPDIRECT3DDEVICE9			m_pd3dDevice;			//d3d�豸

public:
	BOOL CreateBumpMap(INT dwWidth, 
					   INT dwHeight,
					   INT xSize,
					   INT ySize,
					   FLOAT radius,
					   FLOAT speed);
	//���������ʻ�ϵ���Ⱦ����
    BOOL                        CreateMultiLayerRenderTarget(D3DPRESENT_PARAMETERS	d3dpp, UINT width, UINT height);
	VOID                        ReleaseMultiLayerRenderTarget();
	VOID						Release();

public:
	CHAR						name[32];
	INT							index;

	INT							width;					//bumpmap�ĳߴ�
	INT							height;
	D3DMATRIX					bumpMat;
    UINT						m_n;               // Number of vertices in the ground grid along X
    UINT						m_m;               // Number of vertices in the ground grid along Z
	UINT						m_nTriangles;      // Number of triangles in the ground grid
	float                       m_fTime;
	LPDIRECT3DTEXTURE9			pBumpTex;            //BumpMap��ͼ 
	INT							m_xSize;
	INT							m_ySize;			//�����ܶ�
	FLOAT						m_speed;			//�����˶����ٶ�
	FLOAT						m_radius;			//�������
	BOOL                        multiLayer;         //�Ƿ��Ƕ������
    //����Ƕ�������bumpwaterЧ�����򽫶����������Ⱦ��һ�����������bumpmap���л�ɫ
	UINT                        renderWidth;               //��������Ⱦ����Ŀ��
	UINT                        renderHeight;              //��������Ⱦ����ĸ߶�
    
	LPDIRECT3DTEXTURE9			curRenderTexture;             //��ǰ�󻺳��Ӧ����Ⱦ������(Render-to-Texture)
	LPDIRECT3DSURFACE9			curRenderSurface; 
	LPDIRECT3DSURFACE9          curRenderZSurface;

	std::string                 getTextrueFileName(){return std::string(name);}           

public:
	CBumpWater(LPDIRECT3DDEVICE9 pD3DDevice);
	~CBumpWater();

};

#endif




