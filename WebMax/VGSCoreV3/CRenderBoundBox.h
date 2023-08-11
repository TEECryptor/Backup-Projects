/////////////////////////////////////////////////////////////////////////
// 名  称: CAxis.h
// 描  述: XYZ轴模型对象定义，用于点选物体时显示
// 作  者: lixin
// 日  期: 2006-05-15
/////////////////////////////////////////////////////////////////////////

#ifndef _CRENDERBOUNDBOX_H
#define _CRENDERBOUNDBOX_H

#include "CManyFVF.h"
#include "CSysFunc.h"
#include "CTransform.h"

//##ModelId=47EBC7450399
class VGS_EXPORT CRenderBoundBox  
{
private:
	//##ModelId=47EBC745039B
	LPDIRECT3DDEVICE9		m_pd3dDevice;

	//##ModelId=47EBC74503AA
	LPDIRECT3DVERTEXBUFFER9  m_RBBoxVBuffer;		//轴线顶点缓冲
	//##ModelId=47EBC74503AF
    VERTEXDECL_POSCOLOR			 *m_pRBBoxVertices;	//轴线顶点缓冲锁定的内存区域
	//##ModelId=47EBC74503B4
	D3DCOLOR				 RBBoxColor;	//包围盒颜色
	//##ModelId=47EBC74503B8
    UINT                     m_RBBoxVectCnt;  //包围盒顶点个数
	//##ModelId=47EBC74503B9
	UINT                     m_RBBoxLineCnt;   //包围盒边数

	//##ModelId=47EBC74503BB
	LPDIRECT3DINDEXBUFFER9   m_RBBoxIBuffer;	//包围盒索引缓冲
	//##ModelId=47EBC74503BF
	UINT					 *m_pRBBoxIndices;	//包围盒索引缓冲锁定的内存区域

	//##ModelId=47EBC74503C1
	CTransform				 transform;		//变换矩阵

	//初始化顶点
	//##ModelId=47EBC74503C5
	VOID					InitRBBoxVertex();
	//创建顶点缓冲
	//##ModelId=47EBC74503C8
	HRESULT					CreateRBBoxVertexBuf();
	//创建索引缓冲
	//##ModelId=47EBC74503C9
	HRESULT					CreateRBBoxIndexBuf();

	//把包围盒画出来
	//##ModelId=47EBC74503CA
	VOID					DrawRBBox();


public:
	//draw
	//##ModelId=47EBC74503CB
	VOID					Draw();
	//设置变换矩阵
	//##ModelId=47EBC74503CC
	VOID					SetTransformMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC74503CE
	VOID					SetScaleMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC74503D0
	VOID					SetRotationMatrix(D3DXMATRIX* pMat);
	//##ModelId=47EBC74503D8
	VOID					SetTranslateMatrix(D3DXMATRIX* pMat);
	VOID                    SetWorldMatrix(D3DXMATRIX* pMat);
	//update matrix
	//##ModelId=47EBC74503DA
	VOID					MultiplyMatrix();
	//得到变换矩阵
	//##ModelId=47EBC74503DB
	D3DXMATRIX*				GetTransformMatrix();
	//设置大小
	//##ModelId=47EBC74503DC
	VOID					SetScale(FLOAT scaleValue);
	//设置顶点
	//##ModelId=47EBC74503DE
	HRESULT                 SetRBBoxVertex(D3DXVECTOR3 minCorner, D3DXVECTOR3 maxCorner);
	//释放资源
	//##ModelId=47EBC7460000
	VOID					Release();

public:
	//##ModelId=47EBC7460001
	CRenderBoundBox(LPDIRECT3DDEVICE9	pd3dDevice);
	//##ModelId=47EBC7460003
	~CRenderBoundBox();

};

#endif
