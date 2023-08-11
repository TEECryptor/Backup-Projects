// RealWater.h: interface for the CRealWater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALWATER_H__2FBBE04B_55AE_456E_896C_79381783A409__INCLUDED_)
#define AFX_REALWATER_H__2FBBE04B_55AE_456E_896C_79381783A409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"

#include "CSysFunc.h"
#include "CCamera.h"
#include "CModel.h"

#include "surface.h"
#include "parameterhandler.h"
#include "software_noisemaker.h"

class VGS_EXPORT CRealWater  
{
public:
	std::string				m_Name;					//ˮ����������

	LPDIRECT3DSURFACE9		g_depthstencil;			//��������Ȼ���
	LPD3DXEFFECT			m_terrain_effect;		//������Ч����
	CCamera*				m_pCameraView;			//��ǰ���
	vector<CModel*>			m_RTTModels;			//��Ҫ���������ģ��
	vector<CModel*>			m_RTFModels;			//��Ҫ�����ģ��


	int						m_gridsize_x;			//ˮ��X�����������
	int						m_gridsize_y;			//ˮ��Y�����������	
	CSurface*				m_pSea;					//����Surface����
	parameterhandler*		m_prm;					//������ڵĲ���
	D3DLIGHT9				sun;					//̫���Ĺ���

	BOOL                    m_bSupportGPUNormal;		//�Ƿ�֧��64λ������ͼ


public:
	//����RTT����ģ�ͣ���Ϊˮ����Ⱦ�÷������������
	void					addRTTModel(CModel* pModel);
	//�Ƴ�RTT����ģ��
	void					eraseRTTModel(CModel* pModel);
	
	//����RTF����ģ��
	void					addRTFModel(CModel* pModel);
	void					eraseRTFModel(CModel* pModel);

public:
	LPDIRECT3DDEVICE9		m_pd3dDevice;

public:
	//��ʼ��
	HRESULT Init(LPDIRECT3DDEVICE9 pd3dDevice, CCamera* pCamera, D3DXVECTOR3* position, int gridsize_x, int gridsize_y, BOOL bSupportTex64);
	//����Ч��
	HRESULT CRealWater::InitEffect();
	//�������
	VOID	SetViewCamera(CCamera* pCamera);

	VOID	Render();
	VOID	SetupMatrices();
	void	render_refracted_scene();
	void	render_reflected_scene();
	void	render_scene(int mode);
	void	set_clipplane_height(float height);

	void	Invalidate();
	void	RestoreDeviceObject();
	void	Release();


public:
	CRealWater(CHAR* name);
	virtual ~CRealWater();

private:

	D3DXMATRIX m_MatWorld, m_MatView, m_MatProj;

};

#endif // !defined(AFX_REALWATER_H__2FBBE04B_55AE_456E_896C_79381783A409__INCLUDED_)
