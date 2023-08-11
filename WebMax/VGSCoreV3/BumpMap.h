// BumpMap.h: interface for the CBumpMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUMPMAP_H__AD3EF571_366D_46D4_AF05_17574CE7C6CC__INCLUDED_)
#define AFX_BUMPMAP_H__AD3EF571_366D_46D4_AF05_17574CE7C6CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VGSDef.h"
#include "CTexture.h"
#include "CLight.h"

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
//#define _T(x)       __T(x)
//#define __T(x)      x

//Bump��ͼ������
typedef VGS_BumpType BUMPMAPTYPE;

class VGS_EXPORT CBumpMap : public CTexture
{
public:
	LPDIRECT3DDEVICE9			m_pd3dDevice;
	D3DPRESENT_PARAMETERS*		m_pd3dpp;

    LPDIRECT3DTEXTURE9			m_pBumpEnvMap;        // The ������͹��ͼ bumpmap
    LPDIRECT3DTEXTURE9			m_pNormalMap;        // The actual bumpmap

	BUMPMAPTYPE					m_type;				//Bump��ͼ������
    D3DFORMAT					m_BumpMapFormat;    // Bumpmap texture format

	CLight*						m_Light;			//������ͼʹ�õĵƹ�
	D3DXVECTOR3					m_LightDir;			//�ƹ�ķ���
	DWORD						m_dwFactor;			//������ͼ�õ���Factorֵ

	CMaterial*                  m_pMaterial;  // �����Ĳ���
	//--------------------------------------------------------------------------
	//BumpWave����ز���
    LPDIRECT3DTEXTURE9			m_pBumpWaveMap;        // The actual bumpmap
	INT							m_MapWidth;				//���ư�͹��ͼ�Ŀ��
	INT							m_MapHeight;			//���ư�͹��ͼ�Ŀ��
	INT							m_xSize;			//���Ƶ��ܶ�
	INT							m_ySize;
	FLOAT						m_radius;			//����
	FLOAT						m_speed;			//����

	FLOAT                       m_CenterX;          //�����ĵ�����
    FLOAT                       m_CenterY;          //�����ĵ�����

	//BumpEnv����ز���
	FLOAT                       m_Noise1;
	FLOAT                       m_Noise2;

	//��ֱ��ʹ����ͼ�ı��棬��Ϊֱ����Ⱦ����ͼ���濹����޷�ʵ�֣�����ֻ������Ⱦ��ָ�����棬Ȼ�󿽱�����ͼ����
	LPDIRECT3DSURFACE9			m_pRenderTarget;
	LPDIRECT3DTEXTURE9			m_compositeTexForBump;	//Ϊ��͹��ͼ�ϳɵ���ͼ�������ͼ�����а�͹�����Ƚ���������Ⱦ��һ����ͼ���ٽ�����ͼ�밼͹��ɫ
	D3DXMATRIX					m_compositeTexPorjMatrix;	//�ϳɺ����ͼ��ͶӰ����

	
	//��ʼ��BumpEnv��ͼ
	HRESULT						InitBumpEnvMap(const std::string &sFileName, float noise1, float noise2);
	
	//��ʼ��Bump������ͼ
	HRESULT						InitBumpNormalMap(const std::string &sFileName, CLight* pLight);
	
	//��ʼ��Bump������ͼ
	HRESULT						InitBumpWaveMap(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);
	
	//�������ںϳ�����ͼ��ı������ͼ
	HRESULT						InitCompositeRenderTarget();

	// ���㻷����ͼ
	VOID ComputeBumpEnviromentmap();

	VOID ComputeBumpNormalmap();

	// ����bump wave
	VOID ComputeBumpWavemap();
public:
	HRESULT SetBumpAsWavemap(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);
	HRESULT SetBumpAsEnviromentmap(const std::string &sFileName, float noise1, float noise2);
	HRESULT SetBumpAsNormalmap(const std::string &sFileName, CLight* pLight);
	
	/** ��һ��ͼ�񴴽�bump�Ļ�����ͼ������.��Ӧ����CTexture���У���ı�ϴ󣬵�ǰ�ȷ�����.--added by linxq --2009-6-9
	@param strFileName in: ����ͼ����ļ���.
	@return : ����E_FAIL���򴴽�ʧ��.
	*/
	HRESULT CreateBumpBaseTexture(const std::string& strFileName);
	/** �ı�Bump������ͼ��ͼ���ļ�.
	@param strFileName in: ����ͼ����ļ���.
	@param nNoise1 in: ����1.
	@param nNoise2 in: ����2.
	@return : S_OK�������óɹ�.
	*/
	HRESULT ChangeEnvironmentMapImageFile(const std::string &sFileName, float noise1, float noise2);
	/** ����Bump������ͼ����������.
	@param nNoise1 in: ����1.
	@param nNoise2 in: ����2.
	@return : S_OK�������óɹ�.
	*/
	HRESULT SetEnvironmentMapNoise(float fNoise1, float fNoise2);

	
	// ����bump wave�Ĳ���
	HRESULT  SetBumpwaveParam(INT dwWidth, INT dwHeight, INT xSize, INT ySize, FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);
	HRESULT  SetBumpNormalParam(const std::string &sFileName, CLight* pLight);
	
	//���²�����ͼ
	VOID						UpdateBumpWaveMap(INT stageIndex);	

	//���ü��㷨����ͼ��͹Ч��ʹ�õĹ�Դ
	VOID						SetBumpMapLight(CLight* pLight);
	//��Դ��λ��ת��Ϊ��ɫ
	DWORD						VectortoRGBA(FLOAT fHeight);
	//����Ⱦ�����ͼ�񿽱�����ͼ����
	VOID						UpdateTexBuf();
	//��ձ���
	VOID						Clear();
	//�ͷ� 
	VOID						Invalidate();
	//����
	VOID						Restore(D3DXMATRIX matrix);
	//�ͷ���ͼ��Դ
	VOID						Release();

	LPDIRECT3DTEXTURE9          getBumpWavemap(){return m_pBumpWaveMap;}
	LPDIRECT3DTEXTURE9          getBumpEnviromentmap(){return m_pBumpEnvMap;}
	LPDIRECT3DTEXTURE9          getBumpNormalmap(){return m_pNormalMap;}


public:
	CBumpMap(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dpp, D3DFORMAT format);//, INT width);
	~CBumpMap();


};

#endif // !defined(AFX_BUMPMAP_H__AD3EF571_366D_46D4_AF05_17574CE7C6CC__INCLUDED_)
