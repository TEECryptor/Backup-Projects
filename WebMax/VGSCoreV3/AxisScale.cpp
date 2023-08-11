#include "AxisScale.h"

AxisScale::AxisScale(LPDIRECT3DDEVICE9 pd3dDevice)
	:Axis(pd3dDevice)
	,m_PlaneLineVBuffer(NULL)
	,m_pPlaneLineVertices(NULL)
	,m_pArrowVertices(NULL)
	,m_ArrowIBuffer(NULL)
	,m_pArrowIndices(NULL)
	,m_PlaneVBuffer(NULL)
	,m_PlaneIBuffer(NULL)
	,m_pPlaneVertices(NULL)
	,m_pPlaneIndices(NULL)
{
	m_pArrowVertices = NULL;
	m_pArrowIndices = NULL;

	m_fXScale = 1.0f;
	m_fYScale = 1.0f;
	m_fZScale = 1.0f;

	//�������ߺͼ�ͷ����ɫ
	xArrowColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	yArrowColor = D3DCOLOR_RGBA(0, 255, 0, 255);
	zArrowColor = D3DCOLOR_RGBA(0, 0, 255, 255);
	selectAxisPlaneColor =D3DCOLOR_RGBA(255,255,0,128);
	notSelectAxisPlaneColor=D3DCOLOR_RGBA(255,255,0,0);

	//���ü�ͷ�İ뾶
	m_fArrowRadius = 1.0f;
	//���ü�ͷ�ĸ߶�
	m_fArrowHeight = 5.0f;
	FLOAT tLength=m_fLineLength+m_fArrowHeight+5.0f;
	m_posAxisTextX=D3DXVECTOR3(tLength,0.0f,0.0f);
	m_posAxisTextY=D3DXVECTOR3(0.0f,tLength,0.0f);
	m_posAxisTextZ=D3DXVECTOR3(0.0f,0.0f,tLength);

	//���ü�ͷ�ĵ���Ķ���εı���
	arrowCornerCnt = 6;
	m_ArrowVertCnt = (arrowCornerCnt +1)*3;
	m_ArrowFaceCnt = (arrowCornerCnt + (arrowCornerCnt -2)) * 3; 

	//4���������12����
	m_PlaneLineVertCnt = 12;
	//ƽ�棨��4��ƽ�棩��15������.
	m_PlaneVertCnt = 15;
	//��4��ƽ����7��������
	m_PlaneFaceCnt = 7;

	//��İ�Χ�м���
	m_minAxisXBoundingBox=D3DXVECTOR3(m_fLineLength/5,-m_fArrowRadius,-m_fArrowRadius);
	m_maxAxisXBoundingBox=D3DXVECTOR3(m_fLineLength+m_fArrowHeight,m_fArrowRadius,m_fArrowRadius);	
	m_minAxisYBoundingBox= D3DXVECTOR3(-m_fArrowRadius,m_fLineLength/5,-m_fArrowRadius);
	m_maxAxisYBoundingBox= D3DXVECTOR3(m_fArrowRadius,m_fLineLength+m_fArrowHeight,m_fArrowRadius);
	m_minAxisZBoundingBox= D3DXVECTOR3(-m_fArrowRadius,-m_fArrowRadius,m_fLineLength/5);
	m_maxAxisZBoundingBox= D3DXVECTOR3(m_fArrowRadius,m_fArrowRadius,m_fLineLength+m_fArrowHeight);

	//x���Χ�ж��㻺��
	m_xAxisBoundBoxVertics[0] = D3DXVECTOR3(m_fLineLength/5,-m_fArrowRadius,-m_fArrowRadius);
	m_xAxisBoundBoxVertics[1] = D3DXVECTOR3(m_fLineLength/5, m_fArrowRadius,-m_fArrowRadius);
	m_xAxisBoundBoxVertics[2] = D3DXVECTOR3(m_fLineLength+m_fArrowHeight, m_fArrowRadius,-m_fArrowRadius);
	m_xAxisBoundBoxVertics[3] = D3DXVECTOR3(m_fLineLength+m_fArrowHeight,-m_fArrowRadius,-m_fArrowRadius);
	m_xAxisBoundBoxVertics[4] = D3DXVECTOR3(m_fLineLength/5,-m_fArrowRadius,m_fArrowRadius);
	m_xAxisBoundBoxVertics[5] = D3DXVECTOR3(m_fLineLength+m_fArrowHeight,-m_fArrowRadius,m_fArrowRadius);
	m_xAxisBoundBoxVertics[6] = D3DXVECTOR3(m_fLineLength+m_fArrowHeight,m_fArrowRadius,m_fArrowRadius);
	m_xAxisBoundBoxVertics[7] = D3DXVECTOR3(m_fLineLength/5, m_fArrowRadius, m_fArrowRadius); 
	//y���Χ�ж��㻺��
	m_yAxisBoundBoxVertics[0] = D3DXVECTOR3(-m_fArrowRadius,m_fLineLength/5,-m_fArrowRadius);
	m_yAxisBoundBoxVertics[1] = D3DXVECTOR3(-m_fArrowRadius,m_fLineLength+m_fArrowHeight,-m_fArrowRadius);
	m_yAxisBoundBoxVertics[2] = D3DXVECTOR3(m_fArrowRadius,m_fLineLength+m_fArrowHeight,-m_fArrowRadius);
	m_yAxisBoundBoxVertics[3] = D3DXVECTOR3(m_fArrowRadius,m_fLineLength/5,-m_fArrowRadius);
	m_yAxisBoundBoxVertics[4] = D3DXVECTOR3(-m_fArrowRadius,m_fLineLength/5,m_fArrowRadius);
	m_yAxisBoundBoxVertics[5] = D3DXVECTOR3(m_fArrowRadius,m_fLineLength/5, m_fArrowRadius);
	m_yAxisBoundBoxVertics[6] = D3DXVECTOR3(m_fArrowRadius,m_fLineLength+m_fArrowHeight,m_fArrowRadius);
	m_yAxisBoundBoxVertics[7] = D3DXVECTOR3(-m_fArrowRadius,m_fLineLength+m_fArrowHeight, m_fArrowRadius); 
	//z���Χ�ж��㻺��
	m_zAxisBoundBoxVertics[0] = D3DXVECTOR3(-m_fArrowRadius,-m_fArrowRadius,m_fLineLength/5);
	m_zAxisBoundBoxVertics[1] = D3DXVECTOR3(-m_fArrowRadius, m_fArrowRadius,m_fLineLength/5);
	m_zAxisBoundBoxVertics[2] = D3DXVECTOR3( m_fArrowRadius, m_fArrowRadius,m_fLineLength/5);
	m_zAxisBoundBoxVertics[3] = D3DXVECTOR3( m_fArrowRadius,-m_fArrowRadius,m_fLineLength/5);
	m_zAxisBoundBoxVertics[4] = D3DXVECTOR3(-m_fArrowRadius,-m_fArrowRadius,m_fLineLength+m_fArrowHeight);
	m_zAxisBoundBoxVertics[5] = D3DXVECTOR3(m_fArrowRadius,-m_fArrowRadius,m_fLineLength+m_fArrowHeight);
	m_zAxisBoundBoxVertics[6] = D3DXVECTOR3(m_fArrowRadius,m_fArrowRadius,m_fLineLength+m_fArrowHeight);
	m_zAxisBoundBoxVertics[7] = D3DXVECTOR3(-m_fArrowRadius,m_fArrowRadius,m_fLineLength+m_fArrowHeight); 

	planesUnderCursor[0]=0;
	planesUnderCursor[1]=0;

	Create();
}

AxisScale::~AxisScale()
{
	Release();
}

void AxisScale::Release()
{
	SAFE_RELEASE_D3D(m_ArrowVBuffer);
	SAFE_RELEASE_D3D(m_ArrowIBuffer);
	SAFE_RELEASE_D3D(m_PlaneLineVBuffer);
	SAFE_RELEASE_D3D(m_PlaneVBuffer);
	SAFE_RELEASE_D3D(m_PlaneIBuffer);
}
void AxisScale::Create()
{
	Axis::Create();

	if (FAILED( CreatePlaneLineVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ���㻺�����"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	if (FAILED( CreateArrowVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ���㻺�����"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	if (FAILED( CreateArrowIndexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ�����������"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	if (FAILED( CreatePlaneVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ�����������"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	if (FAILED( CreatePlaneIndexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ�����������"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);
}
void AxisScale::Restore()
{
	Axis::Restore();

	if (FAILED( CreatePlaneLineVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ���㻺�����"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	if (FAILED( CreateArrowVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ���㻺�����"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	if (FAILED( CreateArrowIndexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ�����������"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	if (FAILED( CreatePlaneVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ�����������"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);

	if (FAILED( CreatePlaneIndexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ�����������"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);
}
VOID AxisScale::InitPlaneLineVertex()
{
	//added by lxqmail@gmail.com --2010-1-11
	float offset = m_fLineLength*3/4.0f;
	D3DXVECTOR3 vX = D3DXVECTOR3(offset,0,0);
	D3DXVECTOR3 vY = D3DXVECTOR3(0,offset,0);
	D3DXVECTOR3 vZ = D3DXVECTOR3(0,0,offset);
	offset = m_fLineLength/2.0f;
	D3DXVECTOR3 vXoffset = D3DXVECTOR3(offset,0,0);
	D3DXVECTOR3 vYoffset = D3DXVECTOR3(0,offset,0);
	D3DXVECTOR3 vZoffset = D3DXVECTOR3(0,0,offset);

// 	D3DXVECTOR3 vXZMid = (vX + vZ)/2.0f;
// 	D3DXVECTOR3 vXYMid = (vX + vY)/2.0f;
// 	D3DXVECTOR3 vYZMid = (vY + vZ)/2.0f;
// 	float offset = m_fLineLength/12.0f;

// 	D3DXVECTOR3 vDirX = vYZMid - vX;
// 	D3DXVec3Normalize(&vDirX, &vDirX);
// 	D3DXVECTOR3 vXoffset = vX + vDirX * offset;

// 	D3DXVECTOR3 vDirY = vXZMid - vY;
// 	D3DXVec3Normalize(&vDirY, &vDirY);
// 	D3DXVECTOR3 vYoffset = vY + vDirY * offset;
// 
// 	D3DXVECTOR3 vDirZ = vXYMid - vZ;
// 	D3DXVec3Normalize(&vDirZ, &vDirZ);
// 	D3DXVECTOR3 vZoffset = vZ + vDirZ * offset;

	//�߶�2���㣬3���߶ι�6����
	//ǰ��6�����Ӧxyz���ߵĶ˵㣬0,1->x, 2,3->y, 4,5->z
	//xPlaneLine
	m_pPlaneLineVertices[0].position = vX;
	m_pPlaneLineVertices[1].position = vY;
	m_pPlaneLineVertices[0].diffuse = m_colorLineX;
	m_pPlaneLineVertices[1].diffuse = m_colorLineX;

	m_pPlaneLineVertices[2].position = vYoffset;
	m_pPlaneLineVertices[3].position = vXoffset;
	m_pPlaneLineVertices[2].diffuse = m_colorLineX;
	m_pPlaneLineVertices[3].diffuse = m_colorLineX;

	//yPlaneLine
	m_pPlaneLineVertices[4].position = vY;
	m_pPlaneLineVertices[5].position = vZ;
	m_pPlaneLineVertices[4].diffuse = m_colorLineY;
	m_pPlaneLineVertices[5].diffuse = m_colorLineY;

	m_pPlaneLineVertices[6].position = vZoffset;
	m_pPlaneLineVertices[7].position = vYoffset;
	m_pPlaneLineVertices[6].diffuse = m_colorLineY;
	m_pPlaneLineVertices[7].diffuse = m_colorLineY;

	//zPlaneLine
	m_pPlaneLineVertices[8].position = vZ;
	m_pPlaneLineVertices[9].position = vX;
	m_pPlaneLineVertices[8].diffuse = m_colorLineZ;
	m_pPlaneLineVertices[9].diffuse = m_colorLineZ;

	m_pPlaneLineVertices[10].position = vXoffset;
	m_pPlaneLineVertices[11].position = vZoffset;
	m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
	m_pPlaneLineVertices[11].diffuse = m_colorLineZ;

}
VOID AxisScale::InitPlaneVertex()
{
	//added by lxqmail@gmail.com --2010-1-11
	float offset = m_fLineLength*3/4.0f;
	D3DXVECTOR3 vX = D3DXVECTOR3(offset,0,0);
	D3DXVECTOR3 vY = D3DXVECTOR3(0,offset,0);
	D3DXVECTOR3 vZ = D3DXVECTOR3(0,0,offset);
	offset = m_fLineLength/2.0f;
	D3DXVECTOR3 vXoffset = D3DXVECTOR3(offset,0,0);
	D3DXVECTOR3 vYoffset = D3DXVECTOR3(0,offset,0);
	D3DXVECTOR3 vZoffset = D3DXVECTOR3(0,0,offset);

	m_pPlaneVertices[0].position = vX;
	m_pPlaneVertices[0].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[1].position = vY;
	m_pPlaneVertices[1].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[2].position = vYoffset;
	m_pPlaneVertices[2].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[3].position = vXoffset;
	m_pPlaneVertices[3].diffuse =  notSelectAxisPlaneColor; 

	m_pPlaneVertices[4].position = vY;
	m_pPlaneVertices[4].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[5].position = vZ;
	m_pPlaneVertices[5].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[6].position = vZoffset;
	m_pPlaneVertices[6].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[7].position = vYoffset;
	m_pPlaneVertices[7].diffuse =  notSelectAxisPlaneColor; 


	m_pPlaneVertices[8].position = vZ;
	m_pPlaneVertices[8].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[9].position = vX;
	m_pPlaneVertices[9].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[10].position = vXoffset;
	m_pPlaneVertices[10].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[11].position = vZoffset;
	m_pPlaneVertices[11].diffuse =  notSelectAxisPlaneColor; 

	m_pPlaneVertices[12].position = vXoffset;
	m_pPlaneVertices[12].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[13].position = vYoffset;
	m_pPlaneVertices[13].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[14].position = vZoffset;
	m_pPlaneVertices[14].diffuse =  notSelectAxisPlaneColor; 
}
VOID AxisScale::InitArrowVertex()
{
	//��ͷĬ����6��׶��һ����ͷ7���㣬������ͷ��21����
	//x-��ͷ�˵�
	m_pArrowVertices[0].position = D3DXVECTOR3(m_fLineLength+m_fArrowHeight,0,0);
	m_pArrowVertices[0].diffuse = xArrowColor;
	//x-��ͷ���涥��
	UINT index = 1;
	//����ÿ������ǶȲ���
	FLOAT stepCorner = PI2/arrowCornerCnt;
	for (UINT i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.x = m_fLineLength;
		m_pArrowVertices[i].position.y = (FLOAT)sin((i-index)*stepCorner)*m_fArrowRadius;
		m_pArrowVertices[i].position.z = -(FLOAT)cos((i-index)*stepCorner)*m_fArrowRadius;
		m_pArrowVertices[i].diffuse = xArrowColor;
	}
	index = i;

	//y-��ͷ�˵�
	m_pArrowVertices[index].position = D3DXVECTOR3(0,m_fLineLength+m_fArrowHeight,0);
	m_pArrowVertices[index].diffuse = yArrowColor;
	index++;
	//y-��ͷ���涥��
	//����ÿ������ǶȲ���
	for (i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.x = (FLOAT)cos((i-index)*stepCorner)*m_fArrowRadius;
		m_pArrowVertices[i].position.y = m_fLineLength;
		m_pArrowVertices[i].position.z = -(FLOAT)sin((i-index)*stepCorner)*m_fArrowRadius;
		m_pArrowVertices[i].diffuse = yArrowColor;
	}
	index = i;

	//z-��ͷ�˵�
	m_pArrowVertices[index].position = D3DXVECTOR3(0,0,m_fLineLength+m_fArrowHeight);
	m_pArrowVertices[index].diffuse = zArrowColor;
	index++;
	//z-��ͷ���涥��
	//����ÿ������ǶȲ���
	for (i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.x = (FLOAT)cos((i-index)*stepCorner)*m_fArrowRadius;
		m_pArrowVertices[i].position.y = (FLOAT)sin((i-index)*stepCorner)*m_fArrowRadius;
		m_pArrowVertices[i].position.z = m_fLineLength;
		m_pArrowVertices[i].diffuse = zArrowColor;
	}
}
VOID AxisScale::UpdateArrowVertex()
{
	//��ͷĬ����6��׶��һ����ͷ7���㣬������ͷ��21����
	//x-��ͷ�˵�
	m_pArrowVertices[0].position = D3DXVECTOR3(m_fLineLength*m_fXScale+m_fArrowHeight,0,0);
	//x-��ͷ���涥��
	UINT index = 1;
	//����ÿ������ǶȲ���
	FLOAT stepCorner = PI2/arrowCornerCnt;
	for (UINT i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.x = m_fLineLength*m_fXScale;
	}
	index = i;

	//y-��ͷ�˵�
	m_pArrowVertices[index].position = D3DXVECTOR3(0,m_fLineLength*m_fYScale+m_fArrowHeight,0);
	index++;
	//y-��ͷ���涥��
	//����ÿ������ǶȲ���
	for (i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.y = m_fLineLength*m_fYScale;
	}
	index = i;

	//z-��ͷ�˵�
	m_pArrowVertices[index].position = D3DXVECTOR3(0,0,m_fLineLength*m_fZScale+m_fArrowHeight);
	index++;
	//z-��ͷ���涥��
	//����ÿ������ǶȲ���
	for (i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.z = m_fLineLength*m_fZScale;
	}
}

HRESULT AxisScale::CreatePlaneLineVertexBuf()
{
	//crate vertex buffer obj for axis line
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_PlaneLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_PlaneLineVBuffer, NULL) ) )
	{
		return E_FAIL;
	}

	//�����ڴ�
	if( FAILED(m_PlaneLineVBuffer->Lock(0, m_PlaneLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pPlaneLineVertices, 0) ))
	{
		return E_FAIL;
	}

	//��ʼ���ᶥ��
	InitPlaneLineVertex();

	//Unlock���㻺��
	m_PlaneLineVBuffer->Unlock();

	return S_OK;
}
HRESULT AxisScale::CreateArrowVertexBuf()
{
	//crate vertex buffer obj
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_ArrowVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_ArrowVBuffer, NULL) ) )
	{
		return E_FAIL;
	}

	//�����ڴ�
	if( FAILED(m_ArrowVBuffer->Lock(0, m_ArrowVertCnt * sizeof(VERTEXDECL_POSCOLOR), (VOID**)&m_pArrowVertices, 0) ))
	{
		return E_FAIL;
	}

	//��ʼ����ͷ
	InitArrowVertex();

	//Unlock���㻺��
	m_ArrowVBuffer->Unlock();

	return S_OK;
}
HRESULT AxisScale::UpdateArrowVertexBuf()
{
	//�����ڴ�
	if( FAILED(m_ArrowVBuffer->Lock(0, m_ArrowVertCnt * sizeof(VERTEXDECL_POSCOLOR), (VOID**)&m_pArrowVertices, 0) ))
	{
		return E_FAIL;
	}

	//��ʼ����ͷ
	UpdateArrowVertex();

	//Unlock���㻺��
	m_ArrowVBuffer->Unlock();

	return S_OK;
}
HRESULT AxisScale::CreateArrowIndexBuf()
{	
	//������ǰģ�͵���������
	if( FAILED( m_pD3dDevice ->CreateIndexBuffer(m_ArrowFaceCnt * 3 * sizeof(UINT),
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_ArrowIBuffer, NULL) ) )
	{
		return E_FAIL;
	}

	//�����ڴ�
	if( FAILED(m_ArrowIBuffer->Lock(0, m_ArrowFaceCnt *3* sizeof(UINT), (void**)&m_pArrowIndices, 0) ))
	{
		return E_FAIL;
	}

	//����������������
	//x -arrow ��׶����
	UINT iCnt = 0;
	UINT iStart = 0;
	for (UINT i =0; i< arrowCornerCnt; i++)
	{
		m_pArrowIndices[3*i] = 0;
		if (i == arrowCornerCnt -1) 
			m_pArrowIndices[3*i+1] = 1;
		else
			m_pArrowIndices[3*i+1] = i+2;
		m_pArrowIndices[3*i+2] = i+1;
		iCnt += 3;
	}
	iStart = iCnt;
	//x -arrow ��׶����
	for (i = 0; i< arrowCornerCnt -2; i++)
	{
		m_pArrowIndices[iStart + 3*i] = 1;
		m_pArrowIndices[iStart + 3*i + 1] = 2 + i;
		m_pArrowIndices[iStart + 3*i + 2] = 3 + i;
		iCnt += 3;	
	}

	iStart = iCnt;
	//y -arrow ��׶����
	for (i =0; i< arrowCornerCnt; i++)
	{
		m_pArrowIndices[iStart + 3*i] = arrowCornerCnt + 1;
		if (i == arrowCornerCnt -1) 
			m_pArrowIndices[iStart + 3*i+1] = arrowCornerCnt + 2;
		else
			m_pArrowIndices[iStart + 3*i+1] = arrowCornerCnt + i + 3;
		m_pArrowIndices[iStart + 3*i+2] = arrowCornerCnt + i + 2;
		iCnt += 3;
	}

	iStart = iCnt;
	//y -arrow ��׶����
	for (i =0; i< arrowCornerCnt -2; i++)
	{
		m_pArrowIndices[iStart + 3*i] = arrowCornerCnt + 2;
		m_pArrowIndices[iStart + 3*i + 1] = arrowCornerCnt + 3 + i;
		m_pArrowIndices[iStart + 3*i + 2] = arrowCornerCnt + 4 + i;
		iCnt += 3;	
	}

	iStart = iCnt;
	//z -arrow ��׶����
	for (i =0; i< arrowCornerCnt; i++)
	{
		m_pArrowIndices[iStart + 3*i] = 2 * (arrowCornerCnt+ 1);
		if (i == arrowCornerCnt -1) 
			m_pArrowIndices[iStart + 3*i+1] = 2 * (arrowCornerCnt+ 1) + 1;
		else
			m_pArrowIndices[iStart + 3*i+1] = 2 * (arrowCornerCnt+ 1) + i + 2;
		m_pArrowIndices[iStart + 3*i+2] = 2 * (arrowCornerCnt+ 1) + i + 1;
		iCnt += 3;
	}
	iStart = iCnt;
	//z -arrow ��׶����
	for (i =0; i< arrowCornerCnt -2; i++)
	{
		m_pArrowIndices[iStart + 3*i] = 2 * (arrowCornerCnt+ 1) +1;
		m_pArrowIndices[iStart + 3*i + 1] = 2 * (arrowCornerCnt+ 1) + 2 + i;
		m_pArrowIndices[iStart + 3*i + 2] = 2 * (arrowCornerCnt+ 1) + 3 + i;
		iCnt += 3;	
	}

	//Unlock��������
	m_ArrowIBuffer->Unlock();	

	return S_OK;
}
HRESULT AxisScale::CreatePlaneVertexBuf()
{
	//crate vertex buffer obj
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_PlaneVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_PlaneVBuffer, NULL) ) )
	{
		return E_FAIL;
	}

	//�����ڴ�
	if( FAILED(m_PlaneVBuffer->Lock(0, m_PlaneVertCnt * sizeof(VERTEXDECL_POSCOLOR), (VOID**)&m_pPlaneVertices, 0) ))
	{
		return E_FAIL;
	}

	//��ʼ����ͷ
	InitPlaneVertex();

	//Unlock���㻺��
	m_PlaneVBuffer->Unlock();

	return S_OK;
}
HRESULT AxisScale::CreatePlaneIndexBuf()
{	
	//������ǰģ�͵���������
	if( FAILED( m_pD3dDevice ->CreateIndexBuffer(m_PlaneFaceCnt * 3 * sizeof(UINT),
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_PlaneIBuffer, NULL) ) )
	{
		return E_FAIL;
	}

	//�����ڴ�
	if( FAILED(m_PlaneIBuffer->Lock(0, m_PlaneFaceCnt *3* sizeof(UINT), (VOID**)&m_pPlaneIndices, 0) ))
	{
		return E_FAIL;
	}

	//
	m_pPlaneIndices[0]=0;
	m_pPlaneIndices[1]=1;
	m_pPlaneIndices[2]=2;

	m_pPlaneIndices[3]=2;
	m_pPlaneIndices[4]=3;
	m_pPlaneIndices[5]=0;

	m_pPlaneIndices[6]=4;
	m_pPlaneIndices[7]=5;
	m_pPlaneIndices[8]=6;

	m_pPlaneIndices[9]=6;
	m_pPlaneIndices[10]=7;
	m_pPlaneIndices[11]=4;

	m_pPlaneIndices[12]=8;
	m_pPlaneIndices[13]=9;
	m_pPlaneIndices[14]=10;

	m_pPlaneIndices[15]=10;
	m_pPlaneIndices[16]=11;
	m_pPlaneIndices[17]=8;

	m_pPlaneIndices[18]=12;
	m_pPlaneIndices[19]=13;
	m_pPlaneIndices[20]=14;

	//Unlock��������
	m_PlaneIBuffer->Unlock();	

	return S_OK;
}
VOID AxisScale::DrawPlaneLine()
{
	//��Ⱦģ��
	m_pD3dDevice->SetStreamSource( 0, m_PlaneLineVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pD3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//������
	m_pD3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 6);
}
VOID AxisScale::DrawArrow()
{	
	//��Ⱦģ��
	m_pD3dDevice->SetStreamSource( 0, m_ArrowVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pD3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//����IB
	m_pD3dDevice->SetIndices(m_ArrowIBuffer);

	//�ı������������������������ϴ���ȵ����������ٶ�**************
	m_pD3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST , 0, 0,
		m_ArrowVertCnt, //��ǰģ�͵Ķ�����
		0, //��ǰģ�͵��������������
		m_ArrowFaceCnt); //��ǰģ�͵�����		
}
VOID AxisScale::DrawPlane()
{	
	//����͸����ɫΪ��״̬
	//��Alpha���
	m_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pD3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE); 
	//��Ⱦģ��
	m_pD3dDevice->SetStreamSource( 0, m_PlaneVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pD3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//����IB
	m_pD3dDevice->SetIndices(m_PlaneIBuffer);
	m_pD3dDevice->SetStreamSource( 0, m_PlaneVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//�ı������������������������ϴ���ȵ����������ٶ�**************
	m_pD3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST , 0, 0,
		m_PlaneVertCnt, //��ǰģ�͵Ķ�����
		0, //��ǰģ�͵��������������
		m_PlaneFaceCnt); //��ǰģ�͵�����	
	m_pD3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW); 

	//�ر�Alpha���
	m_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}
VOID AxisScale::Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection)
{
	ClearPreDeviceSettings();
	//����Axis���ƶ�����ת
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &m_transform.matrix);
	DrawLine();
	DrawArrow();
	UpdateAxisTextsPos(pViewport, pView, pProjection);
	DrawAxisText();
	DrawPlaneLine();
	DrawPlane();
}
void AxisScale::SelectAxis(UINT selectStatus)
{
	//����Axis���ƶ�����ת
	//�����ڴ�
	if( FAILED(m_LineVBuffer->Lock(0, m_nLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pLineVertices, 0) ))
	{
		return;
	}
	if( FAILED(m_PlaneLineVBuffer->Lock(0, m_PlaneLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pPlaneLineVertices, 0) ))
	{
		return;
	}
	if( FAILED(m_PlaneVBuffer->Lock(0, m_PlaneVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pPlaneVertices, 0) ))
	{
		return;
	}
	UINT i;
	switch(selectStatus)
	{
	case 0:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = m_colorLineX;
		m_pLineVertices[1].diffuse = m_colorLineX;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = m_colorLineY;
		m_pLineVertices[3].diffuse = m_colorLineY;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = m_colorLineZ;
		m_pLineVertices[5].diffuse = m_colorLineZ;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = m_colorLineX;
		m_pPlaneLineVertices[1].diffuse = m_colorLineX;

		m_pPlaneLineVertices[2].diffuse = m_colorLineX;
		m_pPlaneLineVertices[3].diffuse = m_colorLineX;

		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse = m_colorLineY;

		m_pPlaneLineVertices[6].diffuse = m_colorLineY;
		m_pPlaneLineVertices[7].diffuse = m_colorLineY;

		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[9].diffuse = m_colorLineZ;

		m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[11].diffuse = m_colorLineZ;

		m_colorTextX=m_colorLineX;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorLineZ;

		for(i=0;i<m_PlaneVertCnt;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;

	case 1:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = m_colorSelectedAxis;
		m_pLineVertices[1].diffuse = m_colorSelectedAxis;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = m_colorLineY;
		m_pLineVertices[3].diffuse = m_colorLineY;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = m_colorLineZ;
		m_pLineVertices[5].diffuse = m_colorLineZ;

		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = m_colorLineX;
		m_pPlaneLineVertices[1].diffuse = m_colorLineX;

		m_pPlaneLineVertices[2].diffuse = m_colorLineX;
		m_pPlaneLineVertices[3].diffuse = m_colorLineX;

		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse = m_colorLineY;

		m_pPlaneLineVertices[6].diffuse = m_colorLineY;
		m_pPlaneLineVertices[7].diffuse = m_colorLineY;

		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[9].diffuse = m_colorLineZ;

		m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[11].diffuse = m_colorLineZ;


		m_colorTextX=m_colorSelectedAxis;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorLineZ;

		for(i=0;i<m_PlaneVertCnt;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;

	case 2:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = m_colorLineX;
		m_pLineVertices[1].diffuse = m_colorLineX;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = m_colorSelectedAxis;
		m_pLineVertices[3].diffuse = m_colorSelectedAxis;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = m_colorLineZ;
		m_pLineVertices[5].diffuse = m_colorLineZ;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = m_colorLineX;
		m_pPlaneLineVertices[1].diffuse = m_colorLineX;

		m_pPlaneLineVertices[2].diffuse = m_colorLineX;
		m_pPlaneLineVertices[3].diffuse = m_colorLineX;

		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse = m_colorLineY;

		m_pPlaneLineVertices[6].diffuse = m_colorLineY;
		m_pPlaneLineVertices[7].diffuse = m_colorLineY;

		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[9].diffuse = m_colorLineZ;

		m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[11].diffuse = m_colorLineZ;

		m_colorTextX=m_colorLineX;
		m_colorTextY=m_colorSelectedAxis;
		m_colorTextZ=m_colorLineZ;

		for(i=0;i<m_PlaneVertCnt;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;

	case 3:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = m_colorLineX;
		m_pLineVertices[1].diffuse = m_colorLineX;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = m_colorLineY;
		m_pLineVertices[3].diffuse = m_colorLineY;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = m_colorSelectedAxis;
		m_pLineVertices[5].diffuse = m_colorSelectedAxis;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = m_colorLineX;
		m_pPlaneLineVertices[1].diffuse = m_colorLineX;

		m_pPlaneLineVertices[2].diffuse = m_colorLineX;
		m_pPlaneLineVertices[3].diffuse = m_colorLineX;

		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse = m_colorLineY;

		m_pPlaneLineVertices[6].diffuse = m_colorLineY;
		m_pPlaneLineVertices[7].diffuse = m_colorLineY;

		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[9].diffuse = m_colorLineZ;

		m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[11].diffuse = m_colorLineZ;

		m_colorTextX=m_colorLineX;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorSelectedAxis;

		for(i=0;i<m_PlaneVertCnt;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;
	case 4:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = m_colorSelectedAxis;
		m_pLineVertices[1].diffuse = m_colorSelectedAxis;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = m_colorSelectedAxis;
		m_pLineVertices[3].diffuse = m_colorSelectedAxis;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = m_colorLineZ;
		m_pLineVertices[5].diffuse = m_colorLineZ;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = m_colorSelectedAxis;
		m_pPlaneLineVertices[1].diffuse = m_colorSelectedAxis;

		m_pPlaneLineVertices[2].diffuse = m_colorSelectedAxis;
		m_pPlaneLineVertices[3].diffuse = m_colorSelectedAxis;

		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse  = m_colorLineY;

		m_pPlaneLineVertices[6].diffuse  = m_colorLineY;
		m_pPlaneLineVertices[7].diffuse  = m_colorLineY;

		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = m_colorLineZ;
		m_pPlaneLineVertices[9].diffuse  = m_colorLineZ;

		m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[11].diffuse = m_colorLineZ;

		m_colorTextX=m_colorSelectedAxis;
		m_colorTextY=m_colorSelectedAxis;
		m_colorTextZ=m_colorLineZ;

		for(i=0;i<4;i++)
		{
			m_pPlaneVertices[i].diffuse =  selectAxisPlaneColor; 
		} 
		for(i=4;i<m_PlaneVertCnt;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 

		break;
	case 5:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = m_colorLineX;
		m_pLineVertices[1].diffuse = m_colorLineX;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = m_colorSelectedAxis;
		m_pLineVertices[3].diffuse = m_colorSelectedAxis;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = m_colorSelectedAxis;
		m_pLineVertices[5].diffuse = m_colorSelectedAxis;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = m_colorLineX;
		m_pPlaneLineVertices[1].diffuse = m_colorLineX;

		m_pPlaneLineVertices[2].diffuse = m_colorLineX;
		m_pPlaneLineVertices[3].diffuse = m_colorLineX;

		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = m_colorSelectedAxis;
		m_pPlaneLineVertices[5].diffuse  = m_colorSelectedAxis;

		m_pPlaneLineVertices[6].diffuse  = m_colorSelectedAxis;
		m_pPlaneLineVertices[7].diffuse  = m_colorSelectedAxis;

		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = m_colorLineZ;
		m_pPlaneLineVertices[9].diffuse  = m_colorLineZ;

		m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[11].diffuse = m_colorLineZ;

		m_colorTextX=m_colorLineX;
		m_colorTextY=m_colorSelectedAxis;
		m_colorTextZ=m_colorSelectedAxis;

		for(i=0;i<4;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		for(i=4;i<8;i++)
		{
			m_pPlaneVertices[i].diffuse =  selectAxisPlaneColor; 
		} 
		for(i=8;i<m_PlaneVertCnt;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;
	case 6:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = m_colorSelectedAxis;
		m_pLineVertices[1].diffuse = m_colorSelectedAxis;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = m_colorLineY;
		m_pLineVertices[3].diffuse = m_colorLineY;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = m_colorSelectedAxis;
		m_pLineVertices[5].diffuse = m_colorSelectedAxis;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = m_colorLineX;
		m_pPlaneLineVertices[1].diffuse = m_colorLineX;

		m_pPlaneLineVertices[2].diffuse = m_colorLineX;
		m_pPlaneLineVertices[3].diffuse = m_colorLineX;

		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse  = m_colorLineY;

		m_pPlaneLineVertices[6].diffuse  = m_colorLineY;
		m_pPlaneLineVertices[7].diffuse  = m_colorLineY;

		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = m_colorSelectedAxis;
		m_pPlaneLineVertices[9].diffuse  = m_colorSelectedAxis;

		m_pPlaneLineVertices[10].diffuse = m_colorSelectedAxis;
		m_pPlaneLineVertices[11].diffuse = m_colorSelectedAxis;

		m_colorTextX=m_colorSelectedAxis;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorSelectedAxis;

		for(i=0;i<8;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		for(i=8;i<12;i++)
		{
			m_pPlaneVertices[i].diffuse =  selectAxisPlaneColor; 
		} 
		for(i=12;i<m_PlaneVertCnt;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 

		break;
	case 7:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = m_colorSelectedAxis;
		m_pLineVertices[1].diffuse = m_colorSelectedAxis;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = m_colorSelectedAxis;
		m_pLineVertices[3].diffuse = m_colorSelectedAxis;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = m_colorSelectedAxis;
		m_pLineVertices[5].diffuse = m_colorSelectedAxis;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = m_colorLineX;
		m_pPlaneLineVertices[1].diffuse = m_colorLineX;

		m_pPlaneLineVertices[2].diffuse = m_colorSelectedAxis;
		m_pPlaneLineVertices[3].diffuse = m_colorSelectedAxis;

		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse  = m_colorLineY;

		m_pPlaneLineVertices[6].diffuse  = m_colorSelectedAxis;
		m_pPlaneLineVertices[7].diffuse  = m_colorSelectedAxis;

		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = m_colorLineZ;
		m_pPlaneLineVertices[9].diffuse  = m_colorLineZ;

		m_pPlaneLineVertices[10].diffuse = m_colorSelectedAxis;
		m_pPlaneLineVertices[11].diffuse = m_colorSelectedAxis;

		m_colorTextX=m_colorSelectedAxis;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorSelectedAxis;

		for(i=0;i<12;i++)
		{
			m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		for(i=12;i<m_PlaneVertCnt;i++)
		{
			m_pPlaneVertices[i].diffuse =  selectAxisPlaneColor; 
		} 

		break;
	}
	//Unlock���㻺��
	m_LineVBuffer->Unlock();
	m_PlaneLineVBuffer->Unlock();
	m_PlaneVBuffer->Unlock();
	//
	m_nSelectedAxisStatus=selectStatus;
}
VOID AxisScale::SetScale(FLOAT xScaleValue,FLOAT yScaleValue,FLOAT zScaleValue)
{
	D3DXMatrixScaling(&m_transform.scale_matrix,xScaleValue,yScaleValue,zScaleValue);
	m_fXScale=xScaleValue;
	m_fYScale=yScaleValue;
	m_fZScale=zScaleValue;   
	MultiplyMatrix();
	UpdateAxisBoundBoxsVectors();
}
