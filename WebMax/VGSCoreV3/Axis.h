#ifndef AXIS_HPP
#define AXIS_HPP

#include "CManyFVF.h"
#include "CSysFunc.h"
#include "CFont.h"
#include "CText.h"
#include "CTransform.h"

/** 坐标系基类.
author:lxqmail@gmail.com
date:2010-1-7
update:2010-1-112
*/
/** 坐标系基类.
	如果直接使用本类进行渲染，则在创建对象后需要手动调用Create()方法进行构建.
*/
class Axis
{
protected://members
	//D3d 设备
	LPDIRECT3DDEVICE9	m_pD3dDevice;

	//轴线顶点缓冲
	LPDIRECT3DVERTEXBUFFER9  m_LineVBuffer;		
	//轴线顶点缓冲锁定的内存区域
	VERTEXDECL_POSCOLOR*	m_pLineVertices;	
	//轴线顶点个数
	UINT	m_nLineVertCnt;	
	
	//xyz轴线的长度
	float	m_fLineLength;	
	
	//x,y,z轴线颜色
	D3DCOLOR	m_colorLineX, m_colorLineY, m_colorLineZ;
	//选中轴的颜色
	D3DCOLOR	m_colorSelectedAxis;

	//x,y,z轴文字的颜色
	D3DCOLOR	m_colorTextX, m_colorTextY, m_colorTextZ;
	//x,y,z轴文字的屏幕位置
	RECT		m_rcTextXScreenPos, m_rcTextYScreenPos, m_rcTextZScreenPos;
	//轴文字位置
	D3DXVECTOR3	m_posAxisTextX, m_posAxisTextY, m_posAxisTextZ,m_posAxis;

	//文字信息
	CFont*                   m_pFont;

	/**当前选中坐标轴的状态.
	*/
	UINT	m_nSelectedAxisStatus;

	//变换矩阵
	CTransform	m_transform;		


	//轴包围盒顶点缓冲
	ID3DXMesh*	m_pXaxisBoxMesh;
	ID3DXMesh*	m_pYaxisBoxMesh;
	ID3DXMesh*	m_pZaxisBoxMesh;

	//轴原始包围盒
	D3DXVECTOR3	m_minAxisXBoundingBox, m_maxAxisXBoundingBox;
	D3DXVECTOR3	m_minAxisYBoundingBox, m_maxAxisYBoundingBox;
	D3DXVECTOR3	m_minAxisZBoundingBox, m_maxAxisZBoundingBox;
	//变换后的轴包围盒
	D3DXVECTOR3 m_vXBoxMin, m_vXBoxMax;
	D3DXVECTOR3 m_vYBoxMin, m_vYBoxMax;
	D3DXVECTOR3 m_vZBoxMin, m_vZBoxMax;

	//轴包围盒顶点
	D3DXVECTOR3	m_xAxisBoundBoxVertics[8];
	D3DXVECTOR3	m_yAxisBoundBoxVertics[8];
	D3DXVECTOR3	m_zAxisBoundBoxVertics[8];
	//轴包围盒法线向量
	D3DXVECTOR3              m_AxisBoundBoxNormal[8];
	//轴包围盒索引缓冲
	DWORD                    m_AxisBoundBoxIndex[36];

	/** 缩放因子.
	*/
	float                    m_fScale;

	/**默认材质参数.
	*/
	D3DMATERIAL9	m_mtrl;
protected://methods
	void Initialise();
	//初始化轴线顶点
	void	InitLineVertex();
	//创建轴线顶点缓冲
	HRESULT	CreateLineVertexBuf();

	/** 创建坐标轴的包围盒.
	@param axisIndex in:1-x轴，2-y轴，3-z轴
	@param w in:Width of the box, along the x-axis. 
	@param h in:Height of the box, along the y-axis.
	@param d in:Depth of the box, along the z-axis. 
	*/
	HRESULT CreateAxisBoxMesh(int axisIndex, float w, float h, float d);

	//更新变换矩阵
	void UpdateTranslateMatrix();
	/** 更新模型的旋转矩阵
	*/
	void UpdateRotateMatrix();
	//update matrix
	void MultiplyMatrix();
	//更新轴包围盒
	void UpdateAxisBoundBoxsVectors();

	/**根据当前的变换更新3维坐标为屏幕坐标.
	*/
	void UpdateAxisTextsPos(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);
	/** 清空之前的材质与纹理等设置.
	*/
	void ClearPreDeviceSettings();
	//画出文字
	void DrawAxisText();
	//绘制坐标轴的线
	void DrawLine();
	/** 渲染坐标轴的包围盒.
	*/
	void DrawAxisBox(int axisIndex);

	/**判断与坐标轴包围的相交.
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

	/**渲染坐标系
	*/
	virtual void Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);

	/** 设置被选择的轴.
		由具体的子类实现实现功能.
	*/
	virtual void SelectAxis(UINT selectStatus) {};
	UINT GetSelectedAxis() const { return m_nSelectedAxisStatus; }

	/**设置变换矩阵
	*/
	void	SetTransformMatrix(D3DXMATRIX* pMat);
	/**设置缩放变换矩阵
	*/
	void	SetScaleMatrix(D3DXMATRIX* pMat) { m_transform.scale_matrix = *pMat;}
	/**设置旋转变换矩阵
	*/
	void	SetRotationMatrix(D3DXMATRIX* pMat) { m_transform.rotation_matrix = *pMat; }
	/**设置平移变换矩阵
	*/
	void	SetTranslateMatrix(D3DXMATRIX* pMat) { m_transform.translate_matrix = *pMat; }
	/**得到变换矩阵
	*/
	D3DXMATRIX* GetTransformMatrix() { return &m_transform.matrix; }

	//设置位置向量
	void                     SetPosition(D3DXVECTOR3 *pVec);
	//设置旋转轴
	void                     SetRotation(D3DXVECTOR3 *pVec);
	//绕指定轴旋转
	void                     Rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, float fAngle);

	//获取缩放因子
	float					 GetScale() const { return m_fScale; }
	//设置大小
	void					 SetScale(float scaleValue);

	//获得各轴的方向向量
	void GetxAxisRayDir(D3DXVECTOR3& dir);
	void GetyAxisRayDir(D3DXVECTOR3& dir);
	void GetzAxisRayDir(D3DXVECTOR3& dir);

	/** 更新坐标轴上文字的位置.
	*/
	void UpdateAxisXTextPos(long pLeft,long pTop);
	void UpdateAxisYTextPos(long pLeft,long pTop);
	void UpdateAxisZTextPos(long pLeft,long pTop);

	void  GetAxisXTextPos(D3DXVECTOR3& pos);
	void  GetAxisYTextPos(D3DXVECTOR3& pos);
	void  GetAxisZTextPos(D3DXVECTOR3& pos); 
	void  GetAxisPos(D3DXVECTOR3& pos); 

	/** 获取特定射线下坐标系被选中的状态
	@param pos in: 射线原点
	@param dir in: 射线方向
	@return :0-不选择任何轴,1-选择x轴，2-选择y轴，3-选择z轴，4-XY平面, 5-YZ平面, 6-XZ平面.
	*/
	virtual int GetModelAxisStatusWithRay(const D3DXVECTOR3& pos , const D3DXVECTOR3& dir);
};
#endif