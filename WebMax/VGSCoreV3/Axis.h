#ifndef AXIS_HPP
#define AXIS_HPP

#include "CManyFVF.h"
#include "CSysFunc.h"
#include "CFont.h"
#include "CText.h"
#include "CTransform.h"

/** ����ϵ����.
author:lxqmail@gmail.com
date:2010-1-7
update:2010-1-112
*/
/** ����ϵ����.
	���ֱ��ʹ�ñ��������Ⱦ�����ڴ����������Ҫ�ֶ�����Create()�������й���.
*/
class Axis
{
protected://members
	//D3d �豸
	LPDIRECT3DDEVICE9	m_pD3dDevice;

	//���߶��㻺��
	LPDIRECT3DVERTEXBUFFER9  m_LineVBuffer;		
	//���߶��㻺���������ڴ�����
	VERTEXDECL_POSCOLOR*	m_pLineVertices;	
	//���߶������
	UINT	m_nLineVertCnt;	
	
	//xyz���ߵĳ���
	float	m_fLineLength;	
	
	//x,y,z������ɫ
	D3DCOLOR	m_colorLineX, m_colorLineY, m_colorLineZ;
	//ѡ�������ɫ
	D3DCOLOR	m_colorSelectedAxis;

	//x,y,z�����ֵ���ɫ
	D3DCOLOR	m_colorTextX, m_colorTextY, m_colorTextZ;
	//x,y,z�����ֵ���Ļλ��
	RECT		m_rcTextXScreenPos, m_rcTextYScreenPos, m_rcTextZScreenPos;
	//������λ��
	D3DXVECTOR3	m_posAxisTextX, m_posAxisTextY, m_posAxisTextZ,m_posAxis;

	//������Ϣ
	CFont*                   m_pFont;

	/**��ǰѡ���������״̬.
	*/
	UINT	m_nSelectedAxisStatus;

	//�任����
	CTransform	m_transform;		


	//���Χ�ж��㻺��
	ID3DXMesh*	m_pXaxisBoxMesh;
	ID3DXMesh*	m_pYaxisBoxMesh;
	ID3DXMesh*	m_pZaxisBoxMesh;

	//��ԭʼ��Χ��
	D3DXVECTOR3	m_minAxisXBoundingBox, m_maxAxisXBoundingBox;
	D3DXVECTOR3	m_minAxisYBoundingBox, m_maxAxisYBoundingBox;
	D3DXVECTOR3	m_minAxisZBoundingBox, m_maxAxisZBoundingBox;
	//�任������Χ��
	D3DXVECTOR3 m_vXBoxMin, m_vXBoxMax;
	D3DXVECTOR3 m_vYBoxMin, m_vYBoxMax;
	D3DXVECTOR3 m_vZBoxMin, m_vZBoxMax;

	//���Χ�ж���
	D3DXVECTOR3	m_xAxisBoundBoxVertics[8];
	D3DXVECTOR3	m_yAxisBoundBoxVertics[8];
	D3DXVECTOR3	m_zAxisBoundBoxVertics[8];
	//���Χ�з�������
	D3DXVECTOR3              m_AxisBoundBoxNormal[8];
	//���Χ����������
	DWORD                    m_AxisBoundBoxIndex[36];

	/** ��������.
	*/
	float                    m_fScale;

	/**Ĭ�ϲ��ʲ���.
	*/
	D3DMATERIAL9	m_mtrl;
protected://methods
	void Initialise();
	//��ʼ�����߶���
	void	InitLineVertex();
	//�������߶��㻺��
	HRESULT	CreateLineVertexBuf();

	/** ����������İ�Χ��.
	@param axisIndex in:1-x�ᣬ2-y�ᣬ3-z��
	@param w in:Width of the box, along the x-axis. 
	@param h in:Height of the box, along the y-axis.
	@param d in:Depth of the box, along the z-axis. 
	*/
	HRESULT CreateAxisBoxMesh(int axisIndex, float w, float h, float d);

	//���±任����
	void UpdateTranslateMatrix();
	/** ����ģ�͵���ת����
	*/
	void UpdateRotateMatrix();
	//update matrix
	void MultiplyMatrix();
	//�������Χ��
	void UpdateAxisBoundBoxsVectors();

	/**���ݵ�ǰ�ı任����3ά����Ϊ��Ļ����.
	*/
	void UpdateAxisTextsPos(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);
	/** ���֮ǰ�Ĳ��������������.
	*/
	void ClearPreDeviceSettings();
	//��������
	void DrawAxisText();
	//�������������
	void DrawLine();
	/** ��Ⱦ������İ�Χ��.
	*/
	void DrawAxisBox(int axisIndex);

	/**�ж����������Χ���ཻ.
	@return:
	1-X, 2-Y, 3-Z
	*/
	int IntersectAxisLine(const D3DXVECTOR3& pos , const D3DXVECTOR3& dir);

public://methods
	Axis(LPDIRECT3DDEVICE9 pD3dDevice);
	virtual ~Axis();

	virtual void    Create();
	virtual void    Restore();
	virtual void	Release();

	/**��Ⱦ����ϵ
	*/
	virtual void Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);

	/** ���ñ�ѡ�����.
		�ɾ��������ʵ��ʵ�ֹ���.
	*/
	virtual void SelectAxis(UINT selectStatus) {};
	UINT GetSelectedAxis() const { return m_nSelectedAxisStatus; }

	/**���ñ任����
	*/
	void	SetTransformMatrix(D3DXMATRIX* pMat);
	/**�������ű任����
	*/
	void	SetScaleMatrix(D3DXMATRIX* pMat) { m_transform.scale_matrix = *pMat;}
	/**������ת�任����
	*/
	void	SetRotationMatrix(D3DXMATRIX* pMat) { m_transform.rotation_matrix = *pMat; }
	/**����ƽ�Ʊ任����
	*/
	void	SetTranslateMatrix(D3DXMATRIX* pMat) { m_transform.translate_matrix = *pMat; }
	/**�õ��任����
	*/
	D3DXMATRIX* GetTransformMatrix() { return &m_transform.matrix; }

	//����λ������
	void                     SetPosition(D3DXVECTOR3 *pVec);
	//������ת��
	void                     SetRotation(D3DXVECTOR3 *pVec);
	//��ָ������ת
	void                     Rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, float fAngle);

	//��ȡ��������
	float					 GetScale() const { return m_fScale; }
	//���ô�С
	void					 SetScale(float scaleValue);

	//��ø���ķ�������
	void GetxAxisRayDir(D3DXVECTOR3& dir);
	void GetyAxisRayDir(D3DXVECTOR3& dir);
	void GetzAxisRayDir(D3DXVECTOR3& dir);

	/** ���������������ֵ�λ��.
	*/
	void UpdateAxisXTextPos(long pLeft,long pTop);
	void UpdateAxisYTextPos(long pLeft,long pTop);
	void UpdateAxisZTextPos(long pLeft,long pTop);

	void  GetAxisXTextPos(D3DXVECTOR3& pos);
	void  GetAxisYTextPos(D3DXVECTOR3& pos);
	void  GetAxisZTextPos(D3DXVECTOR3& pos); 
	void  GetAxisPos(D3DXVECTOR3& pos); 

	/** ��ȡ�ض�����������ϵ��ѡ�е�״̬
	@param pos in: ����ԭ��
	@param dir in: ���߷���
	@return :0-��ѡ���κ���,1-ѡ��x�ᣬ2-ѡ��y�ᣬ3-ѡ��z�ᣬ4-XYƽ��, 5-YZƽ��, 6-XZƽ��.
	*/
	virtual int GetModelAxisStatusWithRay(const D3DXVECTOR3& pos , const D3DXVECTOR3& dir);
};
#endif