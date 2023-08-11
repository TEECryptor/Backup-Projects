/////////////////////////////////////////////////////////////////////////
// 名  称: CRenderBoundBox.cpp
// 描  述: 模型的渲染包围盒对象定义，用于显示物体的包围盒
// 作  者: baojiahui
// 日  期: 2006-12-27
/////////////////////////////////////////////////////////////////////////

#include "CRenderBoundBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC7460001
CRenderBoundBox::CRenderBoundBox(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_pRBBoxVertices = NULL;
    m_pRBBoxIndices= NULL;
	
	
	
	//设置包围盒的颜色
	RBBoxColor = D3DCOLOR_RGBA(255, 255, 255, 255);	
	//设置包围盒顶点的个数
	m_RBBoxVectCnt = 32;
	//设置包围盒边的个数
	m_RBBoxLineCnt = 24;

	if (FAILED( CreateRBBoxVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("创建包围盒顶点缓冲错误"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);
	if (FAILED(CreateRBBoxIndexBuf() ))
		MessageBox(GetActiveWindow(), TEXT("创建包围盒索引缓冲错误"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);

	
}

//##ModelId=47EBC7460003
CRenderBoundBox::~CRenderBoundBox()
{
	
}


//-----------------------------------------------------------------------------
// 方法名: InitLineVertex()
// 参  数: 
// 用  途: 初始化轴线顶点
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503C5
VOID CRenderBoundBox::InitRBBoxVertex()
{
	//box共32个顶点
	for(UINT i=0;i<32;i++){
    	m_pRBBoxVertices[i].position = D3DXVECTOR3(1.0f,1.0f,1.0f);
	    m_pRBBoxVertices[i].diffuse = RBBoxColor;
	}
}

//-----------------------------------------------------------------------------
// 方法名: CreateRBBoxIndexBuf()
// 参  数: 
// 用  途: 创建包围盒的索引缓冲
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503C9
HRESULT CRenderBoundBox::CreateRBBoxIndexBuf()
{	
	//建立当前模型的索引缓冲
	if( FAILED( m_pd3dDevice ->CreateIndexBuffer(m_RBBoxLineCnt * 2 * sizeof(UINT),
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_RBBoxIBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//锁定内存
	if( FAILED(m_RBBoxIBuffer->Lock(0, m_RBBoxLineCnt *2* sizeof(UINT), (VOID**)&m_pRBBoxIndices, 0) ))
	{
		return E_FAIL;
	}
	//创建索引数据
	UINT RBBoxindexData[] ={
		    0,1,
			0,2,
			0,3,
			4,5,
			4,6,
			4,7,
			8,9,
            8,10,
			8,11,
			12,13,
			12,14,
			12,15,
			16,17,
			16,18,
			16,19,
			20,21,
			20,22,
			20,23,
			24,25,
			24,26,
			24,27,
			28,29,
			28,30,
			28,31
	};
	//逐个点填充索引数据
	for(UINT i=0; i<sizeof(RBBoxindexData)/sizeof(UINT); i++)
	{
		m_pRBBoxIndices[i]=RBBoxindexData[i];
	}
	
	//Unlock索引缓冲
	m_RBBoxIBuffer->Unlock();	
	
	return S_OK;
}
//-----------------------------------------------------------------------------
// 方法名: CreateRBBoxVertexBuf()
// 参  数: 
// 用  途: 创建包围盒顶点缓冲
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503C8
HRESULT CRenderBoundBox::CreateRBBoxVertexBuf()
{
	//crate vertex buffer obj for axis line
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(m_RBBoxVectCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_RBBoxVBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//锁定内存
	if( FAILED(m_RBBoxVBuffer->Lock(0, m_RBBoxVectCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pRBBoxVertices, 0) ))
	{
		return E_FAIL;
	}
	
	//初始化轴顶点
	InitRBBoxVertex();
	
	//Unlock顶点缓冲
	m_RBBoxVBuffer->Unlock();
	
	return S_OK;
}


//-----------------------------------------------------------------------------
// 方法名: DrawRBBox()
// 参  数: 
// 用  途: 把包围盒画出来
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503CA
VOID CRenderBoundBox::DrawRBBox()
{
	//渲染模型
	m_pd3dDevice->SetStreamSource( 0, m_RBBoxVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//设置FVF
	m_pd3dDevice->SetFVF(FVF_POSCOLOR); //DX9为SetFVF,DX8用这个函数
	//设置IB
	m_pd3dDevice->SetIndices(m_RBBoxIBuffer);
	//改变索引缓冲区的面的数量将会较大幅度的提升运算速度**************
	   m_pd3dDevice->DrawIndexedPrimitive(D3DPT_LINELIST , 0, 0,
		   m_RBBoxVectCnt, //当前模型的顶点数
		   0, //当前模型的索引缓冲区起点
		   m_RBBoxLineCnt); //当前模型的面数
}



//-----------------------------------------------------------------------------
// 方法名: Draw()
// 参  数: 
// 用  途: 把坐标系画出来
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503CB
VOID CRenderBoundBox::Draw()
{
	//处理包围盒的移动和旋转
	// m_pd3dDevice->SetTransform( D3DTS_WORLD, &transform.worldMatrix);
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &transform.matrix);
	DrawRBBox();
	
}
//-----------------------------------------------------------------------------
// 方法名: SetRBBoxVertex()
// 参  数: BoundingBox bBox --包围盒的minCorner和maxCorner
// 用  途: 设置模型的包围盒的顶点
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503DE
HRESULT CRenderBoundBox::SetRBBoxVertex(D3DXVECTOR3 minCorner, D3DXVECTOR3 maxCorner)
{
  	//锁定内存
	if( FAILED(m_RBBoxVBuffer->Lock(0, m_RBBoxVectCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pRBBoxVertices, 0) ))
	{
		return E_FAIL;
	}
	
	//设置顶点
	m_pRBBoxVertices[0].position = D3DXVECTOR3(minCorner.x,minCorner.y,minCorner.z);
	m_pRBBoxVertices[1].position = D3DXVECTOR3(minCorner.x+(maxCorner.x-minCorner.x)/4.0f,minCorner.y,minCorner.z);
	m_pRBBoxVertices[2].position = D3DXVECTOR3(minCorner.x,minCorner.y+(maxCorner.y-minCorner.y)/4.0f,minCorner.z);
	m_pRBBoxVertices[3].position = D3DXVECTOR3(minCorner.x,minCorner.y,minCorner.z+(maxCorner.z-minCorner.z)/4.0f);
	
	m_pRBBoxVertices[4].position = D3DXVECTOR3(minCorner.x,maxCorner.y,minCorner.z);
    m_pRBBoxVertices[5].position = D3DXVECTOR3(minCorner.x+(maxCorner.x-minCorner.x)/4.0f,maxCorner.y,minCorner.z);
	m_pRBBoxVertices[6].position = D3DXVECTOR3(minCorner.x,maxCorner.y-(maxCorner.y-minCorner.y)/4.0f,minCorner.z);
    m_pRBBoxVertices[7].position = D3DXVECTOR3(minCorner.x,maxCorner.y,minCorner.z+(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[8].position = D3DXVECTOR3(maxCorner.x,maxCorner.y,minCorner.z);
	m_pRBBoxVertices[9].position = D3DXVECTOR3(maxCorner.x-(maxCorner.x-minCorner.x)/4.0f,maxCorner.y,minCorner.z);
	m_pRBBoxVertices[10].position = D3DXVECTOR3(maxCorner.x,maxCorner.y-(maxCorner.y-minCorner.y)/4.0f,minCorner.z);
    m_pRBBoxVertices[11].position = D3DXVECTOR3(maxCorner.x,maxCorner.y,minCorner.z+(maxCorner.z-minCorner.z)/4.0f);
	
	m_pRBBoxVertices[12].position = D3DXVECTOR3(maxCorner.x,minCorner.y,minCorner.z);
	m_pRBBoxVertices[13].position = D3DXVECTOR3(maxCorner.x-(maxCorner.x-minCorner.x)/4.0f,minCorner.y,minCorner.z);
	m_pRBBoxVertices[14].position = D3DXVECTOR3(maxCorner.x,minCorner.y+(maxCorner.y-minCorner.y)/4.0f,minCorner.z);
    m_pRBBoxVertices[15].position = D3DXVECTOR3(maxCorner.x,minCorner.y,minCorner.z+(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[16].position = D3DXVECTOR3(minCorner.x,minCorner.y,maxCorner.z);
	m_pRBBoxVertices[17].position = D3DXVECTOR3(minCorner.x+(maxCorner.x-minCorner.x)/4.0f,minCorner.y,maxCorner.z);
	m_pRBBoxVertices[18].position = D3DXVECTOR3(minCorner.x,minCorner.y+(maxCorner.y-minCorner.y)/4.0f,maxCorner.z);
	m_pRBBoxVertices[19].position = D3DXVECTOR3(minCorner.x,minCorner.y,maxCorner.z-(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[20].position = D3DXVECTOR3(minCorner.x,maxCorner.y,maxCorner.z);
    m_pRBBoxVertices[21].position = D3DXVECTOR3(minCorner.x+(maxCorner.x-minCorner.x)/4.0f,maxCorner.y,maxCorner.z);
    m_pRBBoxVertices[22].position = D3DXVECTOR3(minCorner.x,maxCorner.y-(maxCorner.y-minCorner.y)/4.0f,maxCorner.z);
    m_pRBBoxVertices[23].position = D3DXVECTOR3(minCorner.x,maxCorner.y,maxCorner.z-(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[24].position = D3DXVECTOR3(maxCorner.x,maxCorner.y,maxCorner.z);
    m_pRBBoxVertices[25].position = D3DXVECTOR3(maxCorner.x-(maxCorner.x-minCorner.x)/4.0f,maxCorner.y,maxCorner.z);
    m_pRBBoxVertices[26].position = D3DXVECTOR3(maxCorner.x,maxCorner.y-(maxCorner.y-minCorner.y)/4.0f,maxCorner.z);
    m_pRBBoxVertices[27].position = D3DXVECTOR3(maxCorner.x,maxCorner.y,maxCorner.z-(maxCorner.z-minCorner.z)/4.0f);

	m_pRBBoxVertices[28].position = D3DXVECTOR3(maxCorner.x,minCorner.y,maxCorner.z);
	m_pRBBoxVertices[29].position = D3DXVECTOR3(maxCorner.x-(maxCorner.x-minCorner.x)/4.0f,minCorner.y,maxCorner.z);
	m_pRBBoxVertices[30].position = D3DXVECTOR3(maxCorner.x,minCorner.y+(maxCorner.y-minCorner.y)/4.0f,maxCorner.z);
	m_pRBBoxVertices[31].position = D3DXVECTOR3(maxCorner.x,minCorner.y,maxCorner.z-(maxCorner.z-minCorner.z)/4.0f);
	
	//Unlock顶点缓冲
	m_RBBoxVBuffer->Unlock();
	
	return S_OK;

}
//-----------------------------------------------------------------------------
// 方法名: SetTransformMatrix()
// 参  数: 
// 用  途: 设置变换矩阵
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503CC
VOID CRenderBoundBox::SetTransformMatrix(D3DXMATRIX* pMat)
{
	transform.matrix = *pMat;
}	

//-----------------------------------------------------------------------------
// 方法名: SetScaleMatrix()
// 参  数: D3DXMATRIX* pMat
// 用  途: 设置变换矩阵
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503CE
VOID CRenderBoundBox::SetScaleMatrix(D3DXMATRIX* pMat)
{
	transform.scale_matrix = *pMat;
}

//-----------------------------------------------------------------------------
// 方法名: SetRotationMatrix()
// 参  数: D3DXMATRIX* pMat
// 用  途: 设置变换矩阵
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503D0
VOID CRenderBoundBox::SetRotationMatrix(D3DXMATRIX* pMat)
{
	transform.rotation_matrix = *pMat;
}

//-----------------------------------------------------------------------------
// 方法名: SetTranslateMatrix()
// 参  数: D3DXMATRIX* pMat
// 用  途: 设置变换矩阵
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503D8
VOID CRenderBoundBox::SetTranslateMatrix(D3DXMATRIX* pMat)
{
	transform.translate_matrix = *pMat;
}

VOID CRenderBoundBox::SetWorldMatrix(D3DXMATRIX* pMat)
{
    transform.worldMatrix = *pMat;
}

//-----------------------------------------------------------------------------
// 方法名: GetTransformMatrix()
// 参  数: 
// 用  途: 得到变换矩阵
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503DB
D3DXMATRIX* CRenderBoundBox::GetTransformMatrix()
{
	return &transform.matrix;
}	

//-----------------------------------------------------------------------------
// 方法名: SetScale()
// 参  数: 
// 用  途: 设置大小
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503DC
VOID CRenderBoundBox::SetScale(FLOAT scaleValue)
{
	D3DXMatrixScaling(&transform.scale_matrix, scaleValue, scaleValue, scaleValue);
	MultiplyMatrix();
}

//-----------------------------------------------------------------------------
// 方法名: MultiplyMatrix()
// 参  数: 
// 用  途: 
//-----------------------------------------------------------------------------
//##ModelId=47EBC74503DA
VOID CRenderBoundBox::MultiplyMatrix()
{
	//cal all matrix
	D3DXMatrixMultiply(&(transform.matrix), 
					   &(transform.scale_matrix), 
					   &(transform.rotation_matrix));
	D3DXMatrixMultiply(&(transform.matrix), 
					   &(transform.matrix), 
					   &(transform.translate_matrix ));
}

//释放资源
//##ModelId=47EBC7460000
VOID CRenderBoundBox::Release()
{
	SAFE_RELEASE_D3D(m_RBBoxVBuffer);
	SAFE_RELEASE_D3D(m_RBBoxIBuffer);
}
