/////////////////////////////////////////////////////////////////////////
// ��  ��: CMoveAxis.h
// ��  ��: XYZ��ģ�Ͷ����壬���ڵ�ѡ����ʱ��ʾ
// ��  ��: lixin
// ��  ��: 2006-05-15
/////////////////////////////////////////////////////////////////////////

#ifndef _CMOVEAXIS_H
#define _CMOVEAXIS_H

#include "CManyFVF.h"
#include "CSysFunc.h"
#include "CTransform.h"
#include "CFont.h"
#include "CText.h"

//##ModelId=47EBC746029F
class CMoveAxis  
{
private:
	//##ModelId=47EBC74602A1
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	
	//##ModelId=47EBC74602B0
	LPDIRECT3DVERTEXBUFFER9  m_LineVBuffer;		//���߶��㻺��
	//##ModelId=47EBC74602B5
    VERTEXDECL_POSCOLOR			 *m_pLineVertices;	//���߶��㻺���������ڴ�����

	//##ModelId=47EBC74602BA
	LPDIRECT3DVERTEXBUFFER9  m_PlaneLineVBuffer;	//����ƽ���ϵ����߶��㻺��
	//##ModelId=47EBC74602BF
    VERTEXDECL_POSCOLOR			 *m_pPlaneLineVertices;	//����ƽ���ϵ����߶��㻺���������ڴ�����
	
	//##ModelId=47EBC74602C4
	LPDIRECT3DVERTEXBUFFER9  m_ArrowVBuffer;	//���ͷ���㻺��
	//##ModelId=47EBC74602C9
    VERTEXDECL_POSCOLOR			 *m_pArrowVertices;	//���ͷ���㻺���������ڴ�����
	
	//##ModelId=47EBC74602CE
	LPDIRECT3DINDEXBUFFER9   m_ArrowIBuffer;	//���ͷ��������
	//##ModelId=47EBC74602D2
	UINT					 *m_pArrowIndices;	//���ͷ���������������ڴ�����
	
	///Edit by bao 2007 1 21-----------------------------------------------------------------
	//##ModelId=47EBC74602D4
	LPDIRECT3DVERTEXBUFFER9  m_PlaneVBuffer;	//ÿ������ƽ��Ķ��㻺��
	
	
	//##ModelId=47EBC74602D9
    LPDIRECT3DINDEXBUFFER9   m_PlaneIBuffer;	//ÿ������ƽ����������
    
	//##ModelId=47EBC74602DD
	FLOAT                    m_Scale;
	
	//##ModelId=47EBC74602DE
	UINT					 m_LineVertCnt;	//���߶������
	//##ModelId=47EBC74602DF
	UINT					 m_ArrowVertCnt;//��ͷ�������
	//##ModelId=47EBC74602E0
	UINT					 m_ArrowFaceCnt;//��ͷ�����
	//##ModelId=47EBC74602E1
	UINT                     m_PlaneVertCnt;//������ƽ��Ķ������
	//##ModelId=47EBC74602EE
	UINT                     m_PlaneFaceCnt;//������ƽ�������
	//##ModelId=47EBC74602EF
	UINT                     m_PlaneLineVertCnt;//����ƽ������߶�����
	//##ModelId=47EBC74602F1
	D3DCOLOR				 xLineColor;	//x������ɫ
	//##ModelId=47EBC74602F6
	D3DCOLOR				 xArrowColor;	//x���ͷ��ɫ
	//##ModelId=47EBC74602FE
	D3DCOLOR				 yLineColor;	//y������ɫ
	//##ModelId=47EBC7460303
	D3DCOLOR				 yArrowColor;	//y���ͷ��ɫ
	//##ModelId=47EBC7460308
	D3DCOLOR				 zLineColor;	//z������ɫ
	//##ModelId=47EBC746030E
	D3DCOLOR				 zArrowColor;	//z���ͷ��ɫ
	//##ModelId=47EBC7460313
	D3DCOLOR                 selectAxisColor;//ѡ�м�ͷ��ɫ
	//##ModelId=47EBC7460318
	D3DCOLOR                 selectAxisPlaneColor;//ѡ����ƽ��ĵ���ɫ
	//##ModelId=47EBC746031D
	D3DCOLOR                 notSelectAxisPlaneColor;//û��ѡ����ƽ�����ɫ

	//##ModelId=47EBC7460322
    D3DCOLOR                 xAxisTextColor; //x���ʾ��ɫ
	//##ModelId=47EBC7460327
	D3DCOLOR                 yAxisTextColor; //y���ʾ��ɫ
	//##ModelId=47EBC746032D
	D3DCOLOR                 zAxisTextColor; //z���ʾ��ɫ
	//xyz���ǵȳ����ȴ��
	//##ModelId=47EBC7460331
	FLOAT					 lineLength;	//xyz���ߵĳ���
	//##ModelId=47EBC746033C
	FLOAT					 arrowRadius;	//��ͷ׶�İ뾶
	//##ModelId=47EBC746033D
	FLOAT					 arrowHeight;	//��ͷ׶�ĸ߶�
	//##ModelId=47EBC746033E
	UINT					 arrowCornerCnt;//��ͷ����ı���

	//##ModelId=47EBC746034C
	RECT                     AxisXTextScreenPos;
	//##ModelId=47EBC7460351
    RECT                     AxisYTextScreenPos;
	//##ModelId=47EBC746035C
	RECT                     AxisZTextScreenPos;

	//##ModelId=47EBC7460361
	CTransform				 transform;		//�任����
	
	//��ʼ������
	//##ModelId=47EBC746036B
	VOID					 InitLineVertex();
	//##ModelId=47EBC746036C
	VOID					 InitPlaneLineVertex();
	//##ModelId=47EBC746037A
	VOID					 InitArrowVertex();
	//##ModelId=47EBC746038A
	VOID                     InitPlaneVertex();
	//�������㻺��
	//##ModelId=47EBC746038B
	HRESULT					 CreateLineVertexBuf();
	//##ModelId=47EBC7460399
	HRESULT					 CreatePlaneLineVertexBuf();
	//##ModelId=47EBC74603A9
	HRESULT					 CreateArrowVertexBuf();
	//##ModelId=47EBC74603AA
	HRESULT					 CreateArrowIndexBuf();
	//##ModelId=47EBC74603B9
	HRESULT                  CreatePlaneVertexBuf();
	//##ModelId=47EBC74603BA
    HRESULT                  CreatePlaneIndexBuf();
	//�����߻�����
	//##ModelId=47EBC74603BB
	VOID					 DrawLine();
	//������ƽ������߻�����
	//##ModelId=47EBC74603C8
	VOID                     DrawPlaneLine();
	//�����ͷ������
	//##ModelId=47EBC74603C9
	VOID					 DrawArrow();
	//������ƽ�滭����
	//##ModelId=47EBC74603D8
	VOID                     DrawPlane();
	//��������
	//##ModelId=47EBC74603D9
	VOID                     DrawAxisText();
	
public:
	//draw
	//##ModelId=47EBC74603DA
	VOID					 Draw();
	//���ñ任����
	//##ModelId=47EBC7470000
	VOID					 SetTransformMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC7470002
	VOID					 SetScaleMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC7470004
	VOID					 SetRotationMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC747000F
	VOID					 SetTranslateMatrix(D3DXMATRIX* pMat);
	//���±任����
	//##ModelId=47EBC7470011
	VOID                     UpdateTranslateMatrix();
	//##ModelId=47EBC7470012
	VOID                     UpdateRotateMatrix();
	//����λ��
	//##ModelId=47EBC7470013
	VOID                     SetPosition(D3DXVECTOR3 *pVec);
	//##ModelId=47EBC7470020
	VOID                     SetRotation(D3DXVECTOR3 *pVec);
	//-------------------------------------------------------------------

	//##ModelId=47EBC7470022
    VOID                     Rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, FLOAT fAngle);


	//���¸������Χ��
	//##ModelId=47EBC7470030
	VOID                     UpdateAxisBoundBoxsVectors();
	//��ø���ķ�������
	//##ModelId=47EBC747003E
	D3DXVECTOR3*             GetxAxisRayDir();
	//##ModelId=47EBC747003F
	D3DXVECTOR3*             GetyAxisRayDir();
	//##ModelId=47EBC7470040
	D3DXVECTOR3*             GetzAxisRayDir();
	//##ModelId=47EBC7470041
	D3DXVECTOR3*             GetAxisXTextPos();
	//##ModelId=47EBC7470042
	D3DXVECTOR3*             GetAxisYTextPos();
	//##ModelId=47EBC7470043
	D3DXVECTOR3*             GetAxisZTextPos(); 
	//##ModelId=47EBC7470044
	D3DXVECTOR3*             GetAxisPos(); 
	//##ModelId=47EBC7470045
    VOID                     UpdateAxisXTextPos(long pLeft,long pTop);
	//##ModelId=47EBC747004F
	VOID                     UpdateAxisYTextPos(long pLeft,long pTop);
	//##ModelId=47EBC747005D
	VOID                     UpdateAxisZTextPos(long pLeft,long pTop);
	//update matrix
	//##ModelId=47EBC7470060
	VOID					 MultiplyMatrix();
	//�õ��任����
	//##ModelId=47EBC7470061
	D3DXMATRIX*			     GetTransformMatrix();
	//��ô�С
	//##ModelId=47EBC747006D
	FLOAT					 GetScale();
	//���ô�С
	//##ModelId=47EBC747006E
	VOID					 SetScale(FLOAT scaleValue);
	//���ñ�ѡ�����
	//##ModelId=47EBC7470070
    HRESULT                  SelectAxis(UINT selectStatus);
	//���ʾλ�ñ���
	//##ModelId=47EBC747007E
	D3DXVECTOR3              ori_AxisTextXpos;
	//##ModelId=47EBC7470083
    D3DXVECTOR3              ori_AxisTextYpos;
	//##ModelId=47EBC747008D
	D3DXVECTOR3              ori_AxisTextZpos;
	//##ModelId=47EBC7470092
    D3DXVECTOR3              ori_AxisPos;

	//##ModelId=47EBC7470097
	D3DXVECTOR3              axisTextXpos;
	//##ModelId=47EBC747009D
    D3DXVECTOR3              axisTextYpos;
	//##ModelId=47EBC74700A2
	D3DXVECTOR3              axisTextZpos;
	//##ModelId=47EBC74700AC
	D3DXVECTOR3              axisPos;
    //��û�б任��Χ�б���
	//##ModelId=47EBC74700B1
    D3DXVECTOR3              ori_xAxisMinCorner;
	//##ModelId=47EBC74700BC
	D3DXVECTOR3              ori_xAxisMaxCorner;
	//##ModelId=47EBC74700C1
	D3DXVECTOR3              ori_yAxisMinCorner;
	//##ModelId=47EBC74700CC
	D3DXVECTOR3              ori_yAxisMaxCorner;
	//##ModelId=47EBC74700D1
	D3DXVECTOR3              ori_zAxisMinCorner;
	//##ModelId=47EBC74700DA
	D3DXVECTOR3              ori_zAxisMaxCorner;
	//���Χ�б���
	//##ModelId=47EBC74700DF
	D3DXVECTOR3              xAxisMinCorner;
	//##ModelId=47EBC74700EB
	D3DXVECTOR3              xAxisMaxCorner;
	//##ModelId=47EBC74700F0
	D3DXVECTOR3              yAxisMinCorner;
	//##ModelId=47EBC74700FB
	D3DXVECTOR3              yAxisMaxCorner;
	//##ModelId=47EBC7470100
	D3DXVECTOR3              zAxisMinCorner;
	//##ModelId=47EBC747010A
	D3DXVECTOR3              zAxisMaxCorner;
    //���Χ�ж��㻺��
	//##ModelId=47EBC747010F
	D3DXVECTOR3              xAxisBoundBoxVertics[8];
	//##ModelId=47EBC747011A
    D3DXVECTOR3              yAxisBoundBoxVertics[8];
	//##ModelId=47EBC747011F
    D3DXVECTOR3              zAxisBoundBoxVertics[8];
	
	//##ModelId=47EBC7470129
    D3DXVECTOR3              xAxisRay;
	//##ModelId=47EBC747012E
    D3DXVECTOR3              yAxisRay;
	//##ModelId=47EBC7470139
	D3DXVECTOR3              zAxisRay;
	//##ModelId=47EBC7470149
	VERTEXDECL_POSCOLOR			 *m_pPlaneVertices;	//ÿ������ƽ��Ķ��㻺���������ڴ�����
	//##ModelId=47EBC747014D
	UINT			         *m_pPlaneIndices;	//ÿ������ƽ�����������������ڴ�����
	//���Χ�з�������
	//##ModelId=47EBC7470158
	D3DXVECTOR3              AxisBoundBoxNormal[8];
	//���Χ����������
	//##ModelId=47EBC7470167
	DWORD                    AxisBoundBoxIndex[36];
	//##ModelId=47EBC7470168
    UINT                     planesUnderCursor[2];
	//������Ϣ
	//##ModelId=47EBC7470178
	CFont*                   pFont;
public:
	//##ModelId=47EBC7470186
	UINT                     selectAxisStatus;
public:
	//##ModelId=47EBC747038A
	VOID    Create();
	//##ModelId=47EBC7470399
	VOID    Restore();
	//##ModelId=47EBC747039A
	VOID	Release();
	//##ModelId=47EBC74703A9
	CMoveAxis(LPDIRECT3DDEVICE9	pd3dDevice);
	//##ModelId=47EBC74703B9
	~CMoveAxis();
	
};

#endif
