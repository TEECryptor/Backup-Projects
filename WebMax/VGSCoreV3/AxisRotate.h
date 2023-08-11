/////////////////////////////////////////////////////////////////////////
// 修改：lxqmail@gmail.com
// 日期：2010-1-8
/////////////////////////////////////////////////////////////////////////

#ifndef AXIS_ROTATE_HPP
#define AXIS_ROTATE_HPP

#include "Axis.h"
#include "CSphere.h"

class AxisRotate: public Axis
{
protected:
	LPDIRECT3DVERTEXBUFFER9  m_CircleLineVBuffer;		//轴线圆顶点缓冲
	VERTEXDECL_POSCOLOR			 *m_pCircleLineVertices;	//轴线圆顶点缓冲锁定的内存区域

	LPDIRECT3DVERTEXBUFFER9  m_RotateFansVBuffer;		//轴线顶点缓冲
	VERTEXDECL_POSCOLOR			 *m_pRotateFansVertices;	//轴线顶点缓冲锁定的内存区域

	CSphere*                  m_Sphere;
	UINT					 m_CircleLineCnt;	//模拟轴线圆的边的个数
	UINT                     m_CircleLineVertCnt; //轴线圆顶点个数

	D3DCOLOR				 xCircleLineColor;	//x轴线颜色
	D3DCOLOR				 yCircleLineColor;	//y轴线颜色
	D3DCOLOR				 zCircleLineColor;	//z轴线颜色

	D3DCOLOR                 selectCircleAxisColor;//选中轴线圆的颜色
	D3DCOLOR                 selectSphereColor;
	D3DCOLOR                 notSelectSphereColor;
	D3DMATERIAL9             m_matMaterial;

	FLOAT					 circleRadius;	//轴线圆的半径

	D3DXMATRIX               rotateRefTrans;   //轴旋转时的变换矩阵

	INT                      rotateAngle;
	INT                      startAngle;

	FLOAT					 sphereRadius;	//轴线圆的半径  
	FLOAT                    ori_sphereRadius;
	//转角开始
	FLOAT                    startRotateAngle;
	//转动过程中的角度
	FLOAT                    fAngle;

protected:
	//初始化轴线圆顶点
	VOID					 InitCircleLineVertex();
	//创建轴线圆顶点缓冲
	HRESULT					 CreateCircleLineVertexBuf();

	//把轴线圆画出来
	VOID					 DrawCircleLine();

	//画出rotate过程中的旋转扇形
	HRESULT                  DrawRotateFans(INT startAngle,INT rotateAngle,UINT rotateAxis);
	VOID                     SetRotateStart();

public:
	AxisRotate(LPDIRECT3DDEVICE9	pd3dDevice);
	~AxisRotate();

	void Create();
	void Restore();
	void Release();

	/**渲染坐标系
	*/
	void Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);

	void SelectAxis(UINT selectStatus);

};
#endif