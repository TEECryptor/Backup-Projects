/////////////////////////////////////////////////////////////////////////
// 修改：lxqmail@gmail.com
// 日期：2010-1-5
/////////////////////////////////////////////////////////////////////////

#ifndef _CMOVEAXIS_H
#define _CMOVEAXIS_H

#include "Axis.h"

class AxisMove: public   Axis
{
protected://members
	//坐标平面上的轴线顶点缓冲
	LPDIRECT3DVERTEXBUFFER9  m_PlaneLineVBuffer;	
	//坐标平面上的轴线顶点缓冲锁定的内存区域
	VERTEXDECL_POSCOLOR			 *m_pPlaneLineVertices;	
	
	//轴箭头顶点缓冲
	LPDIRECT3DVERTEXBUFFER9  m_ArrowVBuffer;	
	//轴箭头顶点缓冲锁定的内存区域
	VERTEXDECL_POSCOLOR			 *m_pArrowVertices;	
	
	//轴箭头索引缓冲
	LPDIRECT3DINDEXBUFFER9   m_ArrowIBuffer;	
	//轴箭头索引缓冲锁定的内存区域
	UINT					 *m_pArrowIndices;	
	
	///Edit by bao 2007 1 21-----------------------------------------------------------------
	//每个坐标平面的顶点缓冲
	LPDIRECT3DVERTEXBUFFER9  m_PlaneVBuffer;	
	//每个坐标平面索引缓冲
    LPDIRECT3DINDEXBUFFER9   m_PlaneIBuffer;	
    	
	//箭头顶点个数
	UINT					 m_ArrowVertCnt;
	UINT					 m_ArrowFaceCnt;//箭头面个数
	UINT                     m_PlaneVertCnt;//坐标轴平面的顶点个数
	UINT                     m_PlaneFaceCnt;//坐标轴平面的面数
	UINT                     m_PlaneLineVertCnt;//坐标平面的轴线顶点数

	D3DCOLOR				 xArrowColor;	//x轴箭头颜色
	D3DCOLOR				 yArrowColor;	//y轴箭头颜色
	D3DCOLOR				 zArrowColor;	//z轴箭头颜色

	D3DCOLOR                 selectAxisPlaneColor;//选中轴平面的的颜色
	D3DCOLOR                 notSelectAxisPlaneColor;//没有选中轴平面的颜色

	FLOAT					 m_fArrowRadius;	//箭头锥的半径
	FLOAT					 m_fArrowHeight;	//箭头锥的高度
	UINT					 arrowCornerCnt;//箭头底面的边数
	
	//每个坐标平面的顶点缓冲锁定的内存区域
	VERTEXDECL_POSCOLOR			 *m_pPlaneVertices;	
	//每个坐标平面索引缓冲锁定的内存区域
	UINT			         *m_pPlaneIndices;	
	UINT                     planesUnderCursor[2];

protected://methos
	void					 InitPlaneLineVertex();
	void					 InitArrowVertex();
	void                     InitPlaneVertex();

	//创建轴平面的边界线的顶点缓冲
	HRESULT					 CreatePlaneLineVertexBuf();
	//创建轴箭头的顶点缓冲
	HRESULT					 CreateArrowVertexBuf();
	//创建轴箭头的索引缓冲
	HRESULT					 CreateArrowIndexBuf();
	//创建轴平面的顶点缓冲
	HRESULT                  CreatePlaneVertexBuf();
	//创建轴平面的索引缓冲
    HRESULT                  CreatePlaneIndexBuf();

	//把坐标平面的轴线画出来
	void                     DrawPlaneLine();
	//把轴箭头画出来
	void					 DrawArrow();
	//把坐标平面画出来
	void                     DrawPlane();
	
	/**判断与坐标平面的相交.
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

	/**渲染坐标系
	*/
	void Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);
	
	/** 设置被选择的轴
	@param selectStatus in :0-不选择任何轴,1-选择x轴，2-选择y轴，3-选择z轴，4-XY平面, 5-YZ平面, 6-XZ平面.
	*/
    void SelectAxis(UINT selectStatus);

	/** 获取特定射线下坐标系被选中的状态
	@param pos in: 射线原点
	@param dir in: 射线方向
	@return :0-不选择任何轴,1-选择x轴，2-选择y轴，3-选择z轴，4-XY平面, 5-YZ平面, 6-XZ平面.
	*/
	int GetModelAxisStatusWithRay(const D3DXVECTOR3& pos , const D3DXVECTOR3& dir);

};

#endif
