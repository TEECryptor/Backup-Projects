////////////////////////////////////////////////////////////////////////////////////////////////
// ����: CFSGLow���Ͷ���--ȫ��������Ч
// ����: �������������ж�̬ģ������ʵ�֣�ʹ����DX8�е�Alpha��Ϻ���Ⱦ������(Render to texture)����
// ����: ����
// ����: 2007-04-11
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CFSGLOW_H
#define _CFSGLOW_H

#include "VGSHeader.h"
#include "CSysFunc.h"
#include <d3d9types.h>

class VGS_EXPORT CFSGlow
{
private:
	LPDIRECT3DDEVICE9			m_pd3dDevice;		
	RECT*						m_pRect;
	D3DPRESENT_PARAMETERS*		m_pd3dpp;
	BOOL						m_bSupport;			//��ǰ��ʾ���Ƿ�֧��FSMotionBlur

	D3DXMATRIX*					m_pMatWorld;
	D3DXMATRIX*					m_pMatView;
	D3DXMATRIX*					m_pMatProj;

	D3DXMATRIX					m_MatIdentity;
	D3DXMATRIX					m_MatView;
	D3DXMATRIX					m_MatProj;

	LPDIRECT3DSURFACE9			m_pCurrentBuffer;                    //��ǰ�󻺴����
	LPDIRECT3DTEXTURE9			m_pCurrentRenderTexture;             //��ǰ�󻺳��Ӧ����Ⱦ������(Render-to-Texture)
	LPDIRECT3DSURFACE9			m_pCurrentRenderSurface;             //��ǰ�������Ⱦ����
	LPDIRECT3DSURFACE9			m_pDisplayZSurface;                  //Z�������
	LPDIRECT3DSURFACE9			m_pZBuffer;						    //Z����
	LPDIRECT3DVERTEXBUFFER9		m_pVB;								//������ʾЧ���ľ��ζ��㻺��
	
	//ȫ�����ε�ǿ�Ȳ���
	//ģ����-�����Alphaͨ��ֵ(0-255),��ֵԽС��Ч��Խǿ
	BYTE						m_GlowAmount;	
	//͸����
	FLOAT						m_OpacityValue;	
	//�Աȶ�


public:
	BOOL						m_bGlowFlag;                         //�����Ƿ��ȫ���߹�Ч��
	
public:
	//�������㻺��
	BOOL						CreateVB();
	//�ͷŶ��㻺��
	VOID						Invalidate();
	//��ʼ����Ч
	BOOL						InitEffect();
	//������Ⱦ����
	VOID						SetRenderTarget();
	//��Ⱦ��Ч
	VOID						RenderEffect();	
	//����ģ��ǿ��
	VOID						SetGlowAmount(INT amount);    
	//���þ��ε�����任
	VOID						SetupNewMatrices();
	//�����Դ
	VOID						Release();                                         

public:
	CFSGlow(LPDIRECT3DDEVICE9	pd3dDevice, 
			RECT* pRect,
			D3DPRESENT_PARAMETERS* pd3dpp,
			D3DXMATRIX*	pMatWorld,
			D3DXMATRIX* pMatView,
			D3DXMATRIX* pMatProj);
	~CFSGlow();
};

#endif
