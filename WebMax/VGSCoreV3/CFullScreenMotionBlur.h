////////////////////////////////////////////////////////////////////////////////////////////////
// ����: CFullScreenMotionBlur���Ͷ���--ȫ����̬ģ����Ч
// ����: �������������ж�̬ģ����ʵ�֣�ʹ����DX8�е�Alpha��Ϻ���Ⱦ������(Render to texture)����
// ����: zyx
// ����: 2006-12-11
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CGLOBALMOTIONBLUR_H
#define _CGLOBALMOTIONBLUR_H

#include "VGSHeader.h"
#include "CSysFunc.h"

//##ModelId=47EBC74E003E
class VGS_EXPORT CFullScreenMotionBlur
{
private:
	//##ModelId=47EBC74E0040
	LPDIRECT3DDEVICE9	m_pd3dDevice;		
	//##ModelId=47EBC74E004F
	RECT*				m_pRect;
	//##ModelId=47EBC74E0054
	D3DPRESENT_PARAMETERS*	m_pd3dpp;
	//##ModelId=47EBC74E0058
	BOOL				m_bSupport;			//��ǰ��ʾ���Ƿ�֧��FSMotionBlur

	//##ModelId=47EBC74E005A
	D3DXMATRIX*			m_pMatWorld;
	//##ModelId=47EBC74E005F
	D3DXMATRIX*			m_pMatView;
	//##ModelId=47EBC74E0064
	D3DXMATRIX*			m_pMatProj;

	//##ModelId=47EBC74E0069
	D3DXMATRIX			m_MatIdentity;
	//##ModelId=47EBC74E006E
	D3DXMATRIX			m_MatView;
	//##ModelId=47EBC74E0073
	D3DXMATRIX			m_MatProj;

	//##ModelId=47EBC74E0078
	LPDIRECT3DSURFACE9	m_pCurrentBuffer;                    //��ǰ�󻺴����
	//##ModelId=47EBC74E007D
	LPDIRECT3DTEXTURE9	m_pCurrentRenderTexture;             //��ǰ�󻺳��Ӧ����Ⱦ������(Render-to-Texture)
	//##ModelId=47EBC74E0082
	LPDIRECT3DSURFACE9	m_pCurrentRenderSurface;             //��ǰ�������Ⱦ����
	//##ModelId=47EBC74E0087
	LPDIRECT3DSURFACE9	m_pDisplayZSurface;                  //Z�������
	//##ModelId=47EBC74E008C
	LPDIRECT3DSURFACE9	m_pZBuffer;						    //Z����
	//##ModelId=47EBC74E0091
	LPDIRECT3DVERTEXBUFFER9 m_pVB;                          //������ʾЧ���ľ��ζ��㻺��
	//##ModelId=47EBC74E0095
	BYTE m_Amount;											//��̬ģ���ĳ̶�,�������Alphaͨ��ֵ(0-255),��ֵԽС��Ч��Խǿ


public:
	//##ModelId=47EBC74E0096
	BOOL m_bBlurFlag;                                       //�����Ƿ�򿪶�̬ģ��
	
public:
	//�������㻺��
	//##ModelId=47EBC74E0097
	BOOL	CreateVB();
	//##ModelId=47EBC74E0098
	VOID	Invalidate();
	//##ModelId=47EBC74E0099
	BOOL InitEffect();								//��ʼ����Ч
	//##ModelId=47EBC74E009A
	VOID SetRenderTarget();
	//##ModelId=47EBC74E009B
	VOID RenderEffect();								//��Ⱦ��Ч
	//##ModelId=47EBC74E009C
	VOID SetBlurAmount(BYTE);                               //����ģ��ǿ��
	//##ModelId=47EBC74E009E
	VOID SetupNewMatrices();							//���þ��ε�����任
	//##ModelId=47EBC74E009F
	VOID Release();                                         //�����Դ

public:
	//##ModelId=47EBC74E00A0
	CFullScreenMotionBlur(LPDIRECT3DDEVICE9	pd3dDevice, 
							RECT* pRect,
							D3DPRESENT_PARAMETERS* pd3dpp,
							D3DXMATRIX*	pMatWorld,
							D3DXMATRIX* pMatView,
							D3DXMATRIX* pMatProj);
	//##ModelId=47EBC74E00AF
	~CFullScreenMotionBlur();
};

#endif
