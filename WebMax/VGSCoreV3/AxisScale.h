/////////////////////////////////////////////////////////////////////////
// 修改：lxqmail@gmail.com
// 日期：2010-1-8
/////////////////////////////////////////////////////////////////////////

#ifndef AXIS_SCALE_HPP
#define AXIS_SCALE_HPP

#include "Axis.h"

class AxisScale: public Axis
{
protected:
	//坐标平面上的轴线顶点缓冲
	LPDIRECT3DVERTEXBUFFER9	m_PlaneLineVBuffer;	
	//坐标平面上的轴线顶点缓冲锁定的内存区域
	VERTEXDECL_POSCOLOR*	m_pPlaneLineVertices;	

	//轴顶端顶点缓冲
	LPDIRECT3DVERTEXBUFFER9  m_ArrowVBuffer;	
	//轴顶端顶点缓冲锁定的内存区域
	VERTEXDECL_POSCOLOR			 *m_pArrowVertices;	
	//轴箭头索引缓冲
	LPDIRECT3DINDEXBUFFER9   m_ArrowIBuffer;	
	//轴箭头索引缓冲锁定的内存区域
	UINT					 *m_pArrowIndices;	

	//每个坐标平面的顶点缓冲
	LPDIRECT3DVERTEXBUFFER9  m_PlaneVBuffer;	
	//每个坐标平面索引缓冲
	LPDIRECT3DINDEXBUFFER9   m_PlaneIBuffer;	

	FLOAT                    m_fXScale;
	FLOAT                    m_fYScale;
	FLOAT                    m_fZScale;

	//箭头顶点个数
	UINT					 m_ArrowVertCnt;
	//箭头面个数
	UINT					 m_ArrowFaceCnt;
	//坐标轴平面的顶点个数
	UINT                     m_PlaneVertCnt;
	//坐标轴平面的面数
	UINT                     m_PlaneFaceCnt;
	//坐标平面的轴线顶点数
	UINT                     m_PlaneLineVertCnt;

	D3DCOLOR				 xArrowColor;	//x轴箭头颜色
	D3DCOLOR				 yArrowColor;	//y轴箭头颜色
	D3DCOLOR				 zArrowColor;	//z轴箭头颜色

	D3DCOLOR                 selectAxisPlaneColor;//选中轴平面的的颜色
	D3DCOLOR                 notSelectAxisPlaneColor;//没有选中轴平面的颜色

	FLOAT					 m_fArrowRadius;	//箭头锥的半径
	FLOAT					 m_fArrowHeight;	//箭头锥的高度
	UINT					 arrowCornerCnt;//箭头底面的边数

	CTransform               arrowTransform; //箭头的变换矩阵


	VERTEXDECL_POSCOLOR			 *m_pPlaneVertices;	//每个坐标平面的顶点缓冲锁定的内存区域
	UINT			         *m_pPlaneIndices;	//每个坐标平面索引缓冲锁定的内存区域
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
	//把轴箭头画出来
	VOID					 DrawArrow();
	//把坐标平面画出来
	VOID                     DrawPlane();

public:
	AxisScale(LPDIRECT3DDEVICE9	pd3dDevice);
	~AxisScale();

	void Create();
	void Restore();
	void Release();

	/**渲染坐标系
	*/
	void Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);

	FLOAT					 GetXScale() const { return m_fXScale; }
	FLOAT					 GetYScale() const { return m_fZScale; }
	FLOAT					 GetZScale() const { return m_fZScale; }

	VOID					 SetScale(FLOAT xScaleValue,FLOAT yScaleValue,FLOAT zScaleValue);

	void SelectAxis(UINT selectStatus);

};
#endif