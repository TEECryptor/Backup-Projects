/////////////////////////////////////////////////////////////////////////////////////////
// ��  ��: CLensFlare
// ��  ��: ��ͷ�����Ч��, ��߶�����COverlay������ʵ��
//			ʵ���Ͼ�ͷ�������һ��Overlay������һ����ǰ���ϵ��ƽ��λ�÷��úã�
//			Ȼ������ӽǶ�̬�ı�Overlay��λ�ü��ɡ�
//			Overlayû�з��볡��Overlay�б�
// ��  ��: zhangfan
// ��  ��: 2006-12-15
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CLENSFLARE_H
#define _ClENSFLARE_H

#include ".\GUI\COverlay.h"
#include <vector>
using std::vector;

//##ModelId=47EBC74900AB
class VGS_EXPORT CLensFlare  
{
private:
	//D3D�������--------------------------------------------------------
	//##ModelId=47EBC74900AD
	LPDIRECT3DDEVICE9			m_pd3dDevice;			//d3d�豸
	//##ModelId=47EBC74900B2
	RECT*						m_pRect;				//����Rectָ��

	//��ͼ����--------------------------------------------------------
	//##ModelId=47EBC74900BC
	LPDIRECT3DTEXTURE9			m_pSunTex;				//��Դ����ͼ
	//##ModelId=47EBC74900C1
	LPDIRECT3DTEXTURE9			m_pTex1;				//��һ��Spot����ͼ
	//##ModelId=47EBC74900C6
	LPDIRECT3DTEXTURE9			m_pTex2;				//�ڶ���Spot����ͼ
	//##ModelId=47EBC74900CB
	LPDIRECT3DTEXTURE9			m_pTex3;				//������Spot����ͼ

	//##ModelId=47EBC74900D0
	RECT						m_formatRect;			//��׼Rect

public:
	//##ModelId=47EBC74900D5
	vector<COverlay*>			m_Spot;					//���Spot Overlay�б�

	//̫����λ��--��ά��������ϵ�е�λ��
	//##ModelId=47EBC74900DA
	D3DXVECTOR3                 m_vSunPos;				//��Դλ��
	//ͶӰ����Ļ�����е�λ��
	//##ModelId=47EBC74900DE
	int                         m_fSunScreenPosX;		//��Դ����Ļ�����xֵ
	//##ModelId=47EBC74900DF
	int                         m_fSunScreenPosY;		//��Դ����Ļ�����yֵ
	//##ModelId=47EBC74900E0
	int                         m_fSunScreenPosZ;		//��Դ����Ļ�����zֵ

private:
	//����Spot
	//##ModelId=47EBC74900E1
	COverlay*	CreateSpot(FLOAT relativeSize,  
						FLOAT refLineY,
						LPDIRECT3DTEXTURE9 pTex,
						D3DCOLOR color);
	//����LensFlare
	//##ModelId=47EBC74900EC
	HRESULT	Create(BYTE* pSunTex, 
					   INT	sunTexSize,
					   BYTE* pTex1,
					   INT tex1Size,
					   BYTE* pTex2, 
					   INT tex2Size,
					   BYTE* pTex3,
					   INT tex3Size);

public:
	//##ModelId=47EBC74900FD
	CLensFlare(LPDIRECT3DDEVICE9 pD3DDevice,
					   RECT* pRect,
					   BYTE* pSunTex, 
					   INT	sunTexSize,
					   BYTE* pTex1,
					   INT tex1Size,
					   BYTE* pTex2, 
					   INT tex2Size,
					   BYTE* pTex3,
					   INT tex3Size,
					   D3DXVECTOR3* pSunPos);
	//##ModelId=47EBC7490110
	~CLensFlare();

	//�ͷ��ڴ�
	//##ModelId=47EBC7490119
	VOID		Release();
};

#endif











































