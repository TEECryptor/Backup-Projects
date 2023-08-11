/////////////////////////////////////////////////////////////////////////
// �޸ģ�lxqmail@gmail.com
// ���ڣ�2010-1-5
/////////////////////////////////////////////////////////////////////////

#ifndef _CMOVEAXIS_H
#define _CMOVEAXIS_H

#include "Axis.h"

class AxisMove: public   Axis
{
protected://members
	//����ƽ���ϵ����߶��㻺��
	LPDIRECT3DVERTEXBUFFER9  m_PlaneLineVBuffer;	
	//����ƽ���ϵ����߶��㻺���������ڴ�����
	VERTEXDECL_POSCOLOR			 *m_pPlaneLineVertices;	
	
	//���ͷ���㻺��
	LPDIRECT3DVERTEXBUFFER9  m_ArrowVBuffer;	
	//���ͷ���㻺���������ڴ�����
	VERTEXDECL_POSCOLOR			 *m_pArrowVertices;	
	
	//���ͷ��������
	LPDIRECT3DINDEXBUFFER9   m_ArrowIBuffer;	
	//���ͷ���������������ڴ�����
	UINT					 *m_pArrowIndices;	
	
	///Edit by bao 2007 1 21-----------------------------------------------------------------
	//ÿ������ƽ��Ķ��㻺��
	LPDIRECT3DVERTEXBUFFER9  m_PlaneVBuffer;	
	//ÿ������ƽ����������
    LPDIRECT3DINDEXBUFFER9   m_PlaneIBuffer;	
    	
	//��ͷ�������
	UINT					 m_ArrowVertCnt;
	UINT					 m_ArrowFaceCnt;//��ͷ�����
	UINT                     m_PlaneVertCnt;//������ƽ��Ķ������
	UINT                     m_PlaneFaceCnt;//������ƽ�������
	UINT                     m_PlaneLineVertCnt;//����ƽ������߶�����

	D3DCOLOR				 xArrowColor;	//x���ͷ��ɫ
	D3DCOLOR				 yArrowColor;	//y���ͷ��ɫ
	D3DCOLOR				 zArrowColor;	//z���ͷ��ɫ

	D3DCOLOR                 selectAxisPlaneColor;//ѡ����ƽ��ĵ���ɫ
	D3DCOLOR                 notSelectAxisPlaneColor;//û��ѡ����ƽ�����ɫ

	FLOAT					 m_fArrowRadius;	//��ͷ׶�İ뾶
	FLOAT					 m_fArrowHeight;	//��ͷ׶�ĸ߶�
	UINT					 arrowCornerCnt;//��ͷ����ı���
	
	//ÿ������ƽ��Ķ��㻺���������ڴ�����
	VERTEXDECL_POSCOLOR			 *m_pPlaneVertices;	
	//ÿ������ƽ�����������������ڴ�����
	UINT			         *m_pPlaneIndices;	
	UINT                     planesUnderCursor[2];

protected://methos
	void					 InitPlaneLineVertex();
	void					 InitArrowVertex();
	void                     InitPlaneVertex();

	//������ƽ��ı߽��ߵĶ��㻺��
	HRESULT					 CreatePlaneLineVertexBuf();
	//�������ͷ�Ķ��㻺��
	HRESULT					 CreateArrowVertexBuf();
	//�������ͷ����������
	HRESULT					 CreateArrowIndexBuf();
	//������ƽ��Ķ��㻺��
	HRESULT                  CreatePlaneVertexBuf();
	//������ƽ�����������
    HRESULT                  CreatePlaneIndexBuf();

	//������ƽ������߻�����
	void                     DrawPlaneLine();
	//�����ͷ������
	void					 DrawArrow();
	//������ƽ�滭����
	void                     DrawPlane();
	
	/**�ж�������ƽ����ཻ.
	@return:
	4-XY, 5-YZ, 6-ZX
	*/
	int IntersectAxisPlane(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir);

public:
	AxisMove(LPDIRECT3DDEVICE9	pd3dDevice);
	~AxisMove();

	void Create();
	void Restore();
	void Release();

	/**��Ⱦ����ϵ
	*/
	void Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);
	
	/** ���ñ�ѡ�����
	@param selectStatus in :0-��ѡ���κ���,1-ѡ��x�ᣬ2-ѡ��y�ᣬ3-ѡ��z�ᣬ4-XYƽ��, 5-YZƽ��, 6-XZƽ��.
	*/
    void SelectAxis(UINT selectStatus);

	/** ��ȡ�ض�����������ϵ��ѡ�е�״̬
	@param pos in: ����ԭ��
	@param dir in: ���߷���
	@return :0-��ѡ���κ���,1-ѡ��x�ᣬ2-ѡ��y�ᣬ3-ѡ��z�ᣬ4-XYƽ��, 5-YZƽ��, 6-XZƽ��.
	*/
	int GetModelAxisStatusWithRay(const D3DXVECTOR3& pos , const D3DXVECTOR3& dir);

};

#endif
