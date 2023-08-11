/////////////////////////////////////////////////////////////////////////
// �޸ģ�lxqmail@gmail.com
// ���ڣ�2010-1-8
/////////////////////////////////////////////////////////////////////////

#ifndef AXIS_ROTATE_HPP
#define AXIS_ROTATE_HPP

#include "Axis.h"
#include "CSphere.h"

class AxisRotate: public Axis
{
protected:
	LPDIRECT3DVERTEXBUFFER9  m_CircleLineVBuffer;		//����Բ���㻺��
	VERTEXDECL_POSCOLOR			 *m_pCircleLineVertices;	//����Բ���㻺���������ڴ�����

	LPDIRECT3DVERTEXBUFFER9  m_RotateFansVBuffer;		//���߶��㻺��
	VERTEXDECL_POSCOLOR			 *m_pRotateFansVertices;	//���߶��㻺���������ڴ�����

	CSphere*                  m_Sphere;
	UINT					 m_CircleLineCnt;	//ģ������Բ�ıߵĸ���
	UINT                     m_CircleLineVertCnt; //����Բ�������

	D3DCOLOR				 xCircleLineColor;	//x������ɫ
	D3DCOLOR				 yCircleLineColor;	//y������ɫ
	D3DCOLOR				 zCircleLineColor;	//z������ɫ

	D3DCOLOR                 selectCircleAxisColor;//ѡ������Բ����ɫ
	D3DCOLOR                 selectSphereColor;
	D3DCOLOR                 notSelectSphereColor;
	D3DMATERIAL9             m_matMaterial;

	FLOAT					 circleRadius;	//����Բ�İ뾶

	D3DXMATRIX               rotateRefTrans;   //����תʱ�ı任����

	INT                      rotateAngle;
	INT                      startAngle;

	FLOAT					 sphereRadius;	//����Բ�İ뾶  
	FLOAT                    ori_sphereRadius;
	//ת�ǿ�ʼ
	FLOAT                    startRotateAngle;
	//ת�������еĽǶ�
	FLOAT                    fAngle;

protected:
	//��ʼ������Բ����
	VOID					 InitCircleLineVertex();
	//��������Բ���㻺��
	HRESULT					 CreateCircleLineVertexBuf();

	//������Բ������
	VOID					 DrawCircleLine();

	//����rotate�����е���ת����
	HRESULT                  DrawRotateFans(INT startAngle,INT rotateAngle,UINT rotateAxis);
	VOID                     SetRotateStart();

public:
	AxisRotate(LPDIRECT3DDEVICE9	pd3dDevice);
	~AxisRotate();

	void Create();
	void Restore();
	void Release();

	/**��Ⱦ����ϵ
	*/
	void Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);

	void SelectAxis(UINT selectStatus);

};
#endif