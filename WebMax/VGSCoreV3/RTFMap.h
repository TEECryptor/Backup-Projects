// RTFMap.h: interface for the CRTFMap class.
// Real Time Reflection ʵʱ������ͼ�࣬���ڴ���������Ҫʵʱ�������ͼ
// 2009-01-02
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTFMAP_H__7F51C9E2_8B33_48B5_AEDA_A09F237DFFED__INCLUDED_)
#define AFX_RTFMAP_H__7F51C9E2_8B33_48B5_AEDA_A09F237DFFED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "CSysFunc.h"
#include "CTexture.h"

#include <vector>
using std::vector;

class VGS_EXPORT CRTFMap : public CTexture
{
public:
	LPDIRECT3DDEVICE9			m_pd3dDevice;	//D3D Device����
	D3DPRESENT_PARAMETERS*		m_pd3dpp;

	//��ͼ�ĳߴ�
	INT							m_width;
	INT							m_height;

	//��Ⱦ��������ͼ�õ����Դ���棬��ֱ��ʹ����ͼ�ı��棬��Ϊֱ����Ⱦ����ͼ���濹����޷�ʵ�֣�����ֻ������Ⱦ��ָ�����棬Ȼ�󿽱�����ͼ����
	LPDIRECT3DSURFACE9			m_pRenderTarget;
	//��Ⱦ��������ͼ�õ���ZBuffer
	LPDIRECT3DSURFACE9			m_pZBuf;

	//ÿ������֡����һ�Σ�Ĭ��Ϊ1
	INT							m_Interval;
	
	//����ƽ��				
	D3DXPLANE					m_refPlane;
	
	//����任����
	D3DXMATRIX					m_refMatrix;
	//������ͼ��ͶӰ����
	D3DXMATRIX					m_texProjMatrix;

	//��RTFMap�����ģ��
	vector<DWORD>				m_refModelList;

	CModel*                     m_pRefPlaneModel;  // ȷ������ƽ���ģ��
	CMaterial*                  m_pMaterial;  // �����Ĳ���

public://getter/setter
	size_t	GetReflectedModelCount() const { return m_refModelList.size(); }
	DWORD	GetReflectedModelByIndex(size_t idx);
	/** ������ͼ�Ŀ�ߣ�������Ҫ���´�����ͼ.
	*/
	void	SetSize(int nWidth, int nHeight);
public:
	//��ʼ����ͼ����
	HRESULT						Init();
	//ͨ����ͷ��������÷���ƽ��
	VOID						SetRefPlaneByModel(D3DXVECTOR3 position, D3DXVECTOR3 normal);
	//ͨ��ģ�����÷���ƽ��
	VOID						SetRefPlaneByModel(DWORD modelHandle);
	//Ϊ������ͼ���ӷ���ģ��
	VOID						AddRefModel(DWORD modelHandle);
	//����Ⱦ�����ͼ�񿽱�����ͼ����
	VOID						UpdateTexBuf();
	//��ձ���
	VOID						Clear();
	//�ͷ�
	VOID						Invalidate();
	//����
	HRESULT						Restore(D3DXMATRIX texProj);
	//����
	VOID						Release();

public:
	CRTFMap(const std::string & name, LPDIRECT3DDEVICE9 pd3dDevice, 
			D3DPRESENT_PARAMETERS* pd3dpp, INT width, INT height, D3DFORMAT format);
	virtual ~CRTFMap();

};

#endif // !defined(AFX_RTFMAP_H__7F51C9E2_8B33_48B5_AEDA_A09F237DFFED__INCLUDED_)
