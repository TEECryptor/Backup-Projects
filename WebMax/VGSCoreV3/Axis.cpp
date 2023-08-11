#include "Axis.h"
#include "Viewport.h"
#include "CCamera.h"

Axis::Axis(LPDIRECT3DDEVICE9 pD3dDevice)
	:m_pD3dDevice(pD3dDevice)
	,m_LineVBuffer(NULL)
	,m_pLineVertices(NULL)
	,m_pFont(NULL)
	,m_pXaxisBoxMesh(NULL)
	,m_pYaxisBoxMesh(NULL)
	,m_pZaxisBoxMesh(NULL)
{
	Initialise();
}
Axis::~Axis()
{
	Release();
}
void Axis::Initialise()
{
	::memset(&m_mtrl, 0, sizeof(m_mtrl));
// 	m_mtrl.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
// 	m_mtrl.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtrl.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//设置轴线的长度
	m_fLineLength = 30.0f;
	m_nLineVertCnt = 6;

	m_colorLineX = D3DCOLOR_RGBA(255, 0, 0, 255);	
	m_colorLineY = D3DCOLOR_RGBA(0, 255, 0, 255);
	m_colorLineZ = D3DCOLOR_RGBA(0, 0, 255, 255);

	m_colorSelectedAxis = D3DCOLOR_RGBA(255,255,0,255);

	m_colorTextX = m_colorLineX;
	m_colorTextY = m_colorLineY;
	m_colorTextZ = m_colorLineZ;

	m_rcTextXScreenPos.left=10;
	m_rcTextXScreenPos.top=10;
	m_rcTextXScreenPos.right = 30;
	m_rcTextXScreenPos.bottom = 20;
	m_rcTextYScreenPos.left=40;
	m_rcTextYScreenPos.top=10;
	m_rcTextYScreenPos.right = 60;
	m_rcTextYScreenPos.bottom = 20;
	m_rcTextZScreenPos.left=70;
	m_rcTextZScreenPos.top=10;
	m_rcTextZScreenPos.right = 90;
	m_rcTextZScreenPos.bottom = 20;

	m_posAxis=D3DXVECTOR3(0.0f,0.0f,0.0f);

	m_nSelectedAxisStatus = 0;
	m_fScale = 1.0f;

	////各轴法线法线向量
	//face 1
	m_AxisBoundBoxNormal[0]= D3DXVECTOR3(0.0f,0.0f,-1.0f); 
	m_AxisBoundBoxNormal[1]= D3DXVECTOR3(0.0f,0.0f,-1.0f); 
	//face 2
	m_AxisBoundBoxNormal[2]= D3DXVECTOR3(0.0f,0.0f,1.0f); 
	m_AxisBoundBoxNormal[3]= D3DXVECTOR3(0.0f,0.0f,1.0f);
	//face 3
	m_AxisBoundBoxNormal[4]= D3DXVECTOR3(0.0f,-1.0f,0.0f);
	m_AxisBoundBoxNormal[5]= D3DXVECTOR3(0.0f,-1.0f,0.0f);
	//face 4
	m_AxisBoundBoxNormal[6]= D3DXVECTOR3(1.0f,0.0f,0.0f);
	m_AxisBoundBoxNormal[7]= D3DXVECTOR3(1.0f,0.0f,0.0f);
	//face 5
	m_AxisBoundBoxNormal[8]= D3DXVECTOR3(0.0f,1.0f,0.0f);
	m_AxisBoundBoxNormal[9]= D3DXVECTOR3(0.0f,1.0f,0.0f);
	//face 6
	m_AxisBoundBoxNormal[10]= D3DXVECTOR3(-1.0f,0.0f,0.0f);
	m_AxisBoundBoxNormal[11]= D3DXVECTOR3(-1.0f,0.0f,0.0f);
	//各轴的索引缓冲
	// triangle 1
	m_AxisBoundBoxIndex[0]=0;
	m_AxisBoundBoxIndex[1]=1;
	m_AxisBoundBoxIndex[2]=2;
	// triangle 2
	m_AxisBoundBoxIndex[3]=2;
	m_AxisBoundBoxIndex[4]=3;
	m_AxisBoundBoxIndex[5]=0;
	// triangle 3
	m_AxisBoundBoxIndex[6]=4;
	m_AxisBoundBoxIndex[7]=5;
	m_AxisBoundBoxIndex[8]=6;
	// triangle 4
	m_AxisBoundBoxIndex[9]=6;
	m_AxisBoundBoxIndex[10]=7;
	m_AxisBoundBoxIndex[11]=4;
	// triangle 5
	m_AxisBoundBoxIndex[12]=0;
	m_AxisBoundBoxIndex[13]=3;
	m_AxisBoundBoxIndex[14]=5;
	// triangle 6
	m_AxisBoundBoxIndex[15]=5;
	m_AxisBoundBoxIndex[16]=4;
	m_AxisBoundBoxIndex[17]=0;
	// triangle 7
	m_AxisBoundBoxIndex[18]=3;
	m_AxisBoundBoxIndex[19]=2;
	m_AxisBoundBoxIndex[20]=6;
	// triangle 8
	m_AxisBoundBoxIndex[21]=6;
	m_AxisBoundBoxIndex[22]=5;
	m_AxisBoundBoxIndex[23]=3;
	// triangle 9
	m_AxisBoundBoxIndex[24]=2;
	m_AxisBoundBoxIndex[25]=1;
	m_AxisBoundBoxIndex[26]=7;
	// triangle 10
	m_AxisBoundBoxIndex[27]=7;
	m_AxisBoundBoxIndex[28]=6;
	m_AxisBoundBoxIndex[29]=2;
	// triangle 11
	m_AxisBoundBoxIndex[30]=1;
	m_AxisBoundBoxIndex[31]=0;
	m_AxisBoundBoxIndex[32]=4;
	// triangle 12
	m_AxisBoundBoxIndex[33]=4;
	m_AxisBoundBoxIndex[34]=7;
	m_AxisBoundBoxIndex[35]=1;

}
void	Axis::InitLineVertex()
{
	//线段2个点，3条线段共6个点
	//前面6个点对应xyz轴线的端点，0,1->x, 2,3->y, 4,5->z
	//xLine
	m_pLineVertices[0].position = D3DXVECTOR3(m_fLineLength/5,0,0);
	m_pLineVertices[1].position = D3DXVECTOR3(m_fLineLength,0,0);
	m_pLineVertices[0].diffuse = m_colorLineX;
	m_pLineVertices[1].diffuse = m_colorLineX;

	//yLine
	m_pLineVertices[2].position = D3DXVECTOR3(0,m_fLineLength/5,0);
	m_pLineVertices[3].position = D3DXVECTOR3(0,m_fLineLength,0);
	m_pLineVertices[2].diffuse = m_colorLineY;
	m_pLineVertices[3].diffuse = m_colorLineY;

	//zLine
	m_pLineVertices[4].position = D3DXVECTOR3(0,0,m_fLineLength/5);
	m_pLineVertices[5].position = D3DXVECTOR3(0,0,m_fLineLength);
	m_pLineVertices[4].diffuse = m_colorLineZ;
	m_pLineVertices[5].diffuse = m_colorLineZ;
}

HRESULT	Axis::CreateLineVertexBuf()
{
	//crate vertex buffer obj for axis line
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_nLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_LineVBuffer, NULL) ) )
	{
		return E_FAIL;
	}

	//锁定内存
	if( FAILED(m_LineVBuffer->Lock(0, m_nLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(void**)&m_pLineVertices, 0) ))
	{
		return E_FAIL;
	}

	//初始化轴顶点
	InitLineVertex();

	//Unlock顶点缓冲
	m_LineVBuffer->Unlock();

	return S_OK;
}

HRESULT Axis::CreateAxisBoxMesh(int axisIndex, float w, float h, float d)
{	
	ID3DXMesh* pMesh = NULL;
	switch (axisIndex)
	{
	case 1:
		if (FAILED(D3DXCreateBox(m_pD3dDevice, w, h, d, &m_pXaxisBoxMesh, NULL))) 
			return E_FAIL;
		pMesh = m_pXaxisBoxMesh;
		break;
	case 2:
		if (FAILED(D3DXCreateBox(m_pD3dDevice, w, h, d, &m_pYaxisBoxMesh, NULL))) 
			return E_FAIL;
		pMesh = m_pYaxisBoxMesh;
		break;
	case 3:
		if (FAILED(D3DXCreateBox(m_pD3dDevice, w, h, d, &m_pZaxisBoxMesh, NULL))) 
			return E_FAIL;
		pMesh = m_pZaxisBoxMesh;
		break;
	default:
		return E_FAIL;
	}
	//生成Mesh，顶点只有位置和法线
	LPDIRECT3DVERTEXBUFFER9 pVBuf;
	if (SUCCEEDED(pMesh->GetVertexBuffer(&pVBuf))) 
	{
		VERTEXDECL_POSNORMAL* pData;
		if (SUCCEEDED(pVBuf->Lock(0,0,(LPVOID *) &pData,D3DLOCK_DISCARD)))
		{
			DWORD numVerts=pMesh->GetNumVertices();
			switch (axisIndex)
			{
			case 1:
				for (DWORD i=0;i<numVerts;i++) 
				{
					pData->position.x += w/2.0f;
					pData++;
				}
				break;
			case 2:
				for (DWORD i=0;i<numVerts;i++) 
				{
					pData->position.y += h/2.0f;
					pData++;
				}
				break;
			case 3:
				for (DWORD i=0;i<numVerts;i++) 
				{
					pData->position.z += d/2.0f;
					pData++;
				}
				break;
			}
			pVBuf->Unlock();
		}
		pVBuf->Release();
		return S_OK;
	}
	return E_FAIL;
}


void Axis::Create()
{
	m_pFont = new CFont(m_pD3dDevice);
	m_pFont->Init("Arial");
	//创建轴模型
	if (FAILED( CreateLineVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("创建Arrow-XYZ顶点缓冲错误"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);
}

void Axis::Restore()
{
	if(m_pFont)
		m_pFont->Restore();

	//创建轴模型
	if (FAILED( CreateLineVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("创建Arrow-XYZ顶点缓冲错误"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);
}

void	Axis::Release()
{
	SAFE_RELEASE_D3D(m_LineVBuffer);
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pFont);	
	SAFE_RELEASE_D3D(m_pXaxisBoxMesh);
	SAFE_RELEASE_D3D(m_pYaxisBoxMesh);
	SAFE_RELEASE_D3D(m_pZaxisBoxMesh);
}
void Axis::UpdateAxisTextsPos(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection)
{
	float len = m_fLineLength * 0.95;
	D3DXVECTOR3 x(len,0,0);

	D3DXVECTOR3 sceen;
	D3DXVec3Project(&sceen, &x, pViewport, pProjection, pView, &m_transform.matrix);
	m_rcTextXScreenPos.left=(int)sceen.x;
	m_rcTextXScreenPos.top= (int)sceen.y;
	m_rcTextXScreenPos.right = m_rcTextXScreenPos.left + 20;
	m_rcTextXScreenPos.bottom = m_rcTextXScreenPos.top + 15;

	D3DXVECTOR3 y(0,len,0);
	D3DXVec3Project(&sceen, &y, pViewport, pProjection, pView, &m_transform.matrix);
	m_rcTextYScreenPos.left= (int)sceen.x;
	m_rcTextYScreenPos.top= (int)sceen.y;
	m_rcTextYScreenPos.right = m_rcTextYScreenPos.left + 20;
	m_rcTextYScreenPos.bottom = m_rcTextYScreenPos.top + 15;

	D3DXVECTOR3 z(0,0,len);
	D3DXVec3Project(&sceen, &z, pViewport, pProjection, pView, &m_transform.matrix);
	m_rcTextZScreenPos.left= (int)sceen.x;
	m_rcTextZScreenPos.top= (int)sceen.y;
	m_rcTextZScreenPos.right = m_rcTextZScreenPos.left + 20;
	m_rcTextZScreenPos.bottom = m_rcTextZScreenPos.top + 15;
}

void Axis::DrawAxisText()
{
	if (NULL == m_pFont)
		return;
	m_pFont->DrawText("x", &m_rcTextXScreenPos, DT_LEFT, m_colorTextX);
	m_pFont->DrawText("y", &m_rcTextYScreenPos, DT_LEFT, m_colorTextY);
	m_pFont->DrawText("z", &m_rcTextZScreenPos, DT_LEFT, m_colorTextZ);
}
void Axis::DrawLine()
{
	//渲染模型
	m_pD3dDevice->SetStreamSource( 0, m_LineVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//设置FVF
	m_pD3dDevice->SetFVF(FVF_POSCOLOR); 
	//画轴线
	m_pD3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);
}
void Axis::DrawAxisBox(int axisIndex)
{
	ID3DXMesh* pMesh = NULL;
	switch (axisIndex)
	{
	case 1:
		pMesh = m_pXaxisBoxMesh;
		break;
	case 2:
		pMesh = m_pYaxisBoxMesh;
		break;
	case 3:
		pMesh = m_pZaxisBoxMesh;
		break;
	default:
		return;
	}
	if (pMesh)
	{
		pMesh->DrawSubset(0);
	}
}

void Axis::Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection)
{
	ClearPreDeviceSettings();

	m_pD3dDevice->SetTransform( D3DTS_WORLD, &m_transform.matrix);
	DrawLine();
	UpdateAxisTextsPos(pViewport, pView, pProjection);
	DrawAxisText();
}
void Axis::ClearPreDeviceSettings()
{
	m_pD3dDevice->SetMaterial(&m_mtrl);
	m_pD3dDevice->SetRenderState(D3DRS_COLORVERTEX,TRUE);
	m_pD3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);
	m_pD3dDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE,D3DMCS_COLOR1);
	//set tex-0, tex-1.. tex-8
	for (INT i = 0; i < 8; i++)
	{
//		m_pD3dDevice->LightEnable(i, FALSE);
		m_pD3dDevice->SetTexture(i, NULL);
		m_pD3dDevice->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_pD3dDevice->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, 0);
	}
	m_pD3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_pD3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
}

void Axis::GetxAxisRayDir(D3DXVECTOR3& dir)
{
	D3DXVECTOR3 rayFrom;
	D3DXVECTOR3 rayTo;
	D3DXVec3TransformCoord(&rayFrom, &(m_pLineVertices[0].position), &(m_transform.matrix));	
	D3DXVec3TransformCoord(&rayTo, &(m_pLineVertices[1].position), &(m_transform.matrix));
	dir=rayTo-rayFrom;
}
void Axis::GetyAxisRayDir(D3DXVECTOR3& dir)
{
	D3DXVECTOR3 rayFrom;
	D3DXVECTOR3 rayTo;
	D3DXVec3TransformCoord(&rayFrom, &(m_pLineVertices[2].position), &(m_transform.matrix));	
	D3DXVec3TransformCoord(&rayTo, &(m_pLineVertices[3].position), &(m_transform.matrix));
	dir=rayTo-rayFrom;
}
void Axis::GetzAxisRayDir(D3DXVECTOR3& dir)
{
	D3DXVECTOR3 rayFrom;
	D3DXVECTOR3 rayTo;
	D3DXVec3TransformCoord(&rayFrom, &(m_pLineVertices[4].position), &(m_transform.matrix));	
	D3DXVec3TransformCoord(&rayTo, &(m_pLineVertices[5].position), &(m_transform.matrix));
	dir=rayTo-rayFrom;
}

void Axis::UpdateAxisXTextPos(long pLeft,long pTop)
{
	m_rcTextXScreenPos.left=pLeft;
	m_rcTextXScreenPos.top=pTop;
}
void Axis::UpdateAxisYTextPos(long pLeft,long pTop)
{
	m_rcTextYScreenPos.left=pLeft;
	m_rcTextYScreenPos.top=pTop;
}
void Axis::UpdateAxisZTextPos(long pLeft,long pTop)
{
	m_rcTextZScreenPos.left=pLeft;
	m_rcTextZScreenPos.top=pTop;
}

void  Axis::GetAxisXTextPos(D3DXVECTOR3& pos)
{
	D3DXVec3TransformCoord(&pos, &(m_posAxisTextX), &(m_transform.matrix));
}
void  Axis::GetAxisYTextPos(D3DXVECTOR3& pos)
{
	D3DXVec3TransformCoord(&pos, &(m_posAxisTextY), &(m_transform.matrix));
}
void  Axis::GetAxisZTextPos(D3DXVECTOR3& pos)
{
	D3DXVec3TransformCoord(&pos, &(m_posAxisTextZ), &(m_transform.matrix));
}
void  Axis::GetAxisPos(D3DXVECTOR3& pos)
{
	D3DXVec3TransformCoord(&pos, &(m_posAxis), &(m_transform.matrix));
}
void Axis::UpdateAxisBoundBoxsVectors()
{
// 	D3DXVec3TransformCoord(&xAxisMinCorner, &(m_minAxisXBoundingBox), &(m_transform.matrix));
// 	D3DXVec3TransformCoord(&xAxisMaxCorner, &(m_maxAxisXBoundingBox), &(m_transform.matrix));
// 	D3DXVec3TransformCoord(&yAxisMinCorner, &(m_minAxisYBoundingBox), &(m_transform.matrix));
// 	D3DXVec3TransformCoord(&yAxisMaxCorner, &(m_maxAxisYBoundingBox), &(m_transform.matrix));
// 	D3DXVec3TransformCoord(&zAxisMinCorner, &(m_minAxisZBoundingBox), &(m_transform.matrix));
// 	D3DXVec3TransformCoord(&zAxisMaxCorner, &(m_maxAxisZBoundingBox), &(m_transform.matrix));
}
void Axis::SetScale(float scaleValue)
{
	m_fScale=scaleValue;
	D3DXMatrixScaling(&m_transform.scale_matrix, scaleValue, scaleValue, scaleValue);
	MultiplyMatrix();
	UpdateAxisBoundBoxsVectors();
}
void Axis::MultiplyMatrix()
{
	//cal all matrix
	D3DXMatrixMultiply(&(m_transform.matrix), 
		&(m_transform.scale_matrix), 
		&(m_transform.rotation_matrix));
	D3DXMatrixMultiply(&(m_transform.matrix), 
		&(m_transform.matrix), 
		&(m_transform.translate_matrix));
}
void Axis::UpdateTranslateMatrix()
{
	//cal translate matrix
	m_transform.translate_matrix._41 = m_transform.position.x;
	m_transform.translate_matrix._42 = m_transform.position.y;
	m_transform.translate_matrix._43 = m_transform.position.z;
}
void Axis::UpdateRotateMatrix()
{
	//cal rotate matrix
	GetRTByRotationVec( &(m_transform.rotation), &(m_transform.rotation_matrix));
}
void Axis::SetPosition(D3DXVECTOR3 *pVec)
{
	//update Axis worldPosition
	m_transform.position = *pVec;
	//update matrix
	UpdateTranslateMatrix();
	MultiplyMatrix();
	UpdateAxisBoundBoxsVectors();
}
void Axis::Rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, FLOAT fAngle)
{
	//旋转变换
	D3DXMATRIX mat_Ros;
	D3DXMatrixRotationAxis(&mat_Ros, pVec, -fAngle);
	D3DXMatrixMultiply(&m_transform.rotation_matrix, 
		&m_transform.rotation_matrix, &mat_Ros);

	//计算因绕定轴旋转产生的位置变换
	D3DXMATRIX mat_Trans1, mat_Trans2;
	D3DXMatrixTranslation(&mat_Trans1, -pPos->x, -pPos->y, -pPos->z);
	D3DXMatrixTranslation(&mat_Trans2, pPos->x, pPos->y, pPos->z);
	//D3DXVec3TransformCoord()方法可以进行位置变换
	D3DXVec3TransformCoord(&m_transform.position, &m_transform.position, &mat_Trans1);
	//D3DXVec3TransformNormal()只能进行角度变换
	D3DXVec3TransformNormal(&m_transform.position, &m_transform.position, &mat_Ros);
	D3DXVec3TransformCoord(&m_transform.position, &m_transform.position, &mat_Trans2);
	UpdateTranslateMatrix();

	//和其它变换矩阵相乘
	MultiplyMatrix();
	UpdateAxisBoundBoxsVectors();
	//得到新的变换对应的转角
	GetRotationVecByRT(&m_transform.rotation_matrix, &m_transform.rotation);
}
void Axis::SetRotation(D3DXVECTOR3 *pVec)
{
	m_transform.rotation = *pVec;
	//update matrix
	UpdateRotateMatrix();
	MultiplyMatrix();
	UpdateAxisBoundBoxsVectors();
}
void	Axis::SetTransformMatrix(D3DXMATRIX* pMat) 
{
	m_transform.matrix = *pMat;
}

int Axis::IntersectAxisLine(const D3DXVECTOR3& pos , const D3DXVECTOR3& dir)
{
	D3DXVec3TransformCoord(&m_vXBoxMin, &m_minAxisXBoundingBox, &m_transform.matrix);
	D3DXVec3TransformCoord(&m_vXBoxMax, &m_maxAxisXBoundingBox, &m_transform.matrix);
	D3DXVec3TransformCoord(&m_vYBoxMin, &m_minAxisYBoundingBox, &m_transform.matrix);
	D3DXVec3TransformCoord(&m_vYBoxMax, &m_maxAxisYBoundingBox, &m_transform.matrix);
	D3DXVec3TransformCoord(&m_vZBoxMin, &m_minAxisZBoundingBox, &m_transform.matrix);
	D3DXVec3TransformCoord(&m_vZBoxMax, &m_maxAxisZBoundingBox, &m_transform.matrix);
// 	BOOL bHit = FALSE;
// 	D3DXIntersect(m_pXaxisBoxMesh, &pos, &dir, &bHit, NULL, NULL, NULL, NULL, NULL, NULL);
// 	if (bHit)
	//轴相交
	if (D3DXBoxBoundProbe(&m_vXBoxMin, &m_vXBoxMax, &pos, &dir))
	{
		return 1;
	}
	else if (D3DXBoxBoundProbe(&m_vYBoxMin, &m_vYBoxMax, &pos, &dir))
	{
		return 2;
	}
	else if (D3DXBoxBoundProbe(&m_vZBoxMin, &m_vZBoxMax, &pos, &dir))
	{
		return 3;
	}


// 	D3DXVECTOR3 newpos, newdir;
// 	D3DXVec3TransformCoord(&newpos, &pos, &m_transform.matrix);
// 	D3DXVec3TransformCoord(&newdir, &dir, &m_transform.matrix);
// 	if (D3DXBoxBoundProbe(&m_vXBoxMin, &m_vXBoxMax, &newpos, &newdir))
// 	{
// 		return 1;
// 	}
// 	else if (D3DXBoxBoundProbe(&m_vYBoxMin, &m_vYBoxMax, &newpos, &newdir))
// 	{
// 		return 2;
// 	}
// 	else if (D3DXBoxBoundProbe(&m_vZBoxMin, &m_vZBoxMax, &newpos, &newdir))
// 	{
// 		return 3;
// 	}
	return 0;
}
int Axis::GetModelAxisStatusWithRay(const D3DXVECTOR3& pos , const D3DXVECTOR3& dir)
{
	return IntersectAxisLine(pos, dir);
}
