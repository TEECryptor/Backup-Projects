/////////////////////////////////////////////////////////////////////////
// �޸ģ�lxqmail@gmail.com
// ���ڣ�2010-1-8
/////////////////////////////////////////////////////////////////////////

#ifndef AXIS_SCALE_HPP
#define AXIS_SCALE_HPP

#include "Axis.h"

class AxisScale: public Axis
{
protected:
	//����ƽ���ϵ����߶��㻺��
	LPDIRECT3DVERTEXBUFFER9	m_PlaneLineVBuffer;	
	//����ƽ���ϵ����߶��㻺���������ڴ�����
	VERTEXDECL_POSCOLOR*	m_pPlaneLineVertices;	

	//�ᶥ�˶��㻺��
	LPDIRECT3DVERTEXBUFFER9  m_ArrowVBuffer;	
	//�ᶥ�˶��㻺���������ڴ�����
	VERTEXDECL_POSCOLOR			 *m_pArrowVertices;	
	//���ͷ��������
	LPDIRECT3DINDEXBUFFER9   m_ArrowIBuffer;	
	//���ͷ���������������ڴ�����
	UINT					 *m_pArrowIndices;	

	//ÿ������ƽ��Ķ��㻺��
	LPDIRECT3DVERTEXBUFFER9  m_PlaneVBuffer;	
	//ÿ������ƽ����������
	LPDIRECT3DINDEXBUFFER9   m_PlaneIBuffer;	

	FLOAT                    m_fXScale;
	FLOAT                    m_fYScale;
	FLOAT                    m_fZScale;

	//��ͷ�������
	UINT					 m_ArrowVertCnt;
	//��ͷ�����
	UINT					 m_ArrowFaceCnt;
	//������ƽ��Ķ������
	UINT                     m_PlaneVertCnt;
	//������ƽ�������
	UINT                     m_PlaneFaceCnt;
	//����ƽ������߶�����
	UINT                     m_PlaneLineVertCnt;

	D3DCOLOR				 xArrowColor;	//x���ͷ��ɫ
	D3DCOLOR				 yArrowColor;	//y���ͷ��ɫ
	D3DCOLOR				 zArrowColor;	//z���ͷ��ɫ

	D3DCOLOR                 selectAxisPlaneColor;//ѡ����ƽ��ĵ���ɫ
	D3DCOLOR                 notSelectAxisPlaneColor;//û��ѡ����ƽ�����ɫ

	FLOAT					 m_fArrowRadius;	//��ͷ׶�İ뾶
	FLOAT					 m_fArrowHeight;	//��ͷ׶�ĸ߶�
	UINT					 arrowCornerCnt;//��ͷ����ı���

	CTransform               arrowTransform; //��ͷ�ı任����


	VERTEXDECL_POSCOLOR			 *m_pPlaneVertices;	//ÿ������ƽ��Ķ��㻺���������ڴ�����
	UINT			         *m_pPlaneIndices;	//ÿ������ƽ�����������������ڴ�����
	UINT                     planesUnderCursor[2];


	VOID					 InitPlaneLineVertex();
	VOID					 InitArrowVertex();
	VOID                     UpdateArrowVertex();
	VOID                     InitPlaneVertex();

	HRESULT					 CreatePlaneLineVertexBuf();
	HRESULT					 CreateArrowVertexBuf();
	HRESULT                  UpdateArrowVertexBuf();
	HRESULT					 CreateArrowIndexBuf();
	HRESULT                  CreatePlaneVertexBuf();
	HRESULT                  CreatePlaneIndexBuf();

	VOID                     DrawPlaneLine();
	//�����ͷ������
	VOID					 DrawArrow();
	//������ƽ�滭����
	VOID                     DrawPlane();

public:
	AxisScale(LPDIRECT3DDEVICE9	pd3dDevice);
	~AxisScale();

	void Create();
	void Restore();
	void Release();

	/**��Ⱦ����ϵ
	*/
	void Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);

	FLOAT					 GetXScale() const { return m_fXScale; }
	FLOAT					 GetYScale() const { return m_fZScale; }
	FLOAT					 GetZScale() const { return m_fZScale; }

	VOID					 SetScale(FLOAT xScaleValue,FLOAT yScaleValue,FLOAT zScaleValue);

	void SelectAxis(UINT selectStatus);

};
#endif