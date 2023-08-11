/////////////////////////////////////////////////////////////////////////
// ��  ��: CMoveAxis.cpp
// ��  ��: XYZ��ģ�Ͷ����壬���ڵ�ѡ����ʱ��ʾ
// ��  ��: lixin
// ��  ��: 2006-05-15
/////////////////////////////////////////////////////////////////////////

#include "AxisMove.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74703A9
AxisMove::AxisMove(LPDIRECT3DDEVICE9 pd3dDevice)
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
	
    m_PlaneLineVertCnt = 12;
	//��������ƽ��ı���
	m_PlaneFaceCnt =6;
	m_ArrowVertCnt = (arrowCornerCnt +1)*3;
	m_ArrowFaceCnt = (arrowCornerCnt + (arrowCornerCnt -2)) * 3; 
    //��������ƽ�涥��ĸ���
	m_PlaneVertCnt =12;

	//��İ�Χ�м���
	m_minAxisXBoundingBox=D3DXVECTOR3(m_fLineLength/5,-m_fArrowRadius,-m_fArrowRadius);
    m_maxAxisXBoundingBox=D3DXVECTOR3(m_fLineLength+m_fArrowHeight,m_fArrowRadius,m_fArrowRadius);	
	m_minAxisYBoundingBox= D3DXVECTOR3(-m_fArrowRadius,m_fLineLength/5,-m_fArrowRadius);
	m_maxAxisYBoundingBox= D3DXVECTOR3(m_fArrowRadius,m_fLineLength+m_fArrowHeight,m_fArrowRadius);
	m_minAxisZBoundingBox= D3DXVECTOR3(-m_fArrowRadius,-m_fArrowRadius,m_fLineLength/5);
	m_maxAxisZBoundingBox= D3DXVECTOR3(m_fArrowRadius,m_fArrowRadius,m_fLineLength+m_fArrowHeight);
	m_vXBoxMin = m_minAxisXBoundingBox;
	m_vXBoxMax = m_maxAxisXBoundingBox;
	m_vYBoxMin = m_minAxisYBoundingBox;
	m_vYBoxMax = m_maxAxisYBoundingBox;
	m_vZBoxMin = m_minAxisZBoundingBox;
	m_vZBoxMax = m_maxAxisZBoundingBox;


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

AxisMove::~AxisMove()
{
	Release();
}
void AxisMove::Release()
{
	SAFE_RELEASE_D3D(m_ArrowVBuffer);
	SAFE_RELEASE_D3D(m_ArrowIBuffer);
	SAFE_RELEASE_D3D(m_PlaneLineVBuffer);
	SAFE_RELEASE_D3D(m_PlaneVBuffer);
	SAFE_RELEASE_D3D(m_PlaneIBuffer);
}

void AxisMove::Create()
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
	CreateAxisBoxMesh(1, abs(m_minAxisXBoundingBox.x - m_maxAxisXBoundingBox.x),
		abs(m_minAxisXBoundingBox.y - m_maxAxisXBoundingBox.y),
		abs(m_minAxisXBoundingBox.z - m_maxAxisXBoundingBox.z));
	CreateAxisBoxMesh(2, abs(m_minAxisYBoundingBox.x - m_maxAxisYBoundingBox.x),
		abs(m_minAxisYBoundingBox.y - m_maxAxisYBoundingBox.y),
		abs(m_minAxisYBoundingBox.z - m_maxAxisYBoundingBox.z));
	CreateAxisBoxMesh(3, abs(m_minAxisZBoundingBox.x - m_maxAxisZBoundingBox.x),
		abs(m_minAxisZBoundingBox.y - m_maxAxisZBoundingBox.y),
		abs(m_minAxisZBoundingBox.z - m_maxAxisZBoundingBox.z));
}
void AxisMove::Restore()
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

//-----------------------------------------------------------------------------
// ������: InitPlaneLineVertex()
// ��  ��: 
// ��  ;: ��ʼ�����߶���
//-----------------------------------------------------------------------------
//##ModelId=47EBC746036C
void AxisMove::InitPlaneLineVertex()
{
	//�߶�2���㣬3���߶ι�6����
	//ǰ��6�����Ӧxyz���ߵĶ˵㣬0,1->x, 2,3->y, 4,5->z
	//xPlaneLine
    m_pPlaneLineVertices[0].position = D3DXVECTOR3(m_fLineLength/2,0,0);
	m_pPlaneLineVertices[1].position = D3DXVECTOR3(m_fLineLength/2,m_fLineLength/2,0);
	m_pPlaneLineVertices[0].diffuse = m_colorLineX;
	m_pPlaneLineVertices[1].diffuse = m_colorLineX;

	m_pPlaneLineVertices[2].position = D3DXVECTOR3(m_fLineLength/2,0,0);
	m_pPlaneLineVertices[3].position = D3DXVECTOR3(m_fLineLength/2,0,m_fLineLength/2);
	m_pPlaneLineVertices[2].diffuse = m_colorLineX;
	m_pPlaneLineVertices[3].diffuse = m_colorLineX;

	//yPlaneLine
    m_pPlaneLineVertices[4].position = D3DXVECTOR3(0,m_fLineLength/2,0);
	m_pPlaneLineVertices[5].position = D3DXVECTOR3(m_fLineLength/2,m_fLineLength/2,0);
	m_pPlaneLineVertices[4].diffuse = m_colorLineY;
	m_pPlaneLineVertices[5].diffuse = m_colorLineY;

	m_pPlaneLineVertices[6].position = D3DXVECTOR3(0,m_fLineLength/2,0);
	m_pPlaneLineVertices[7].position = D3DXVECTOR3(0,m_fLineLength/2,m_fLineLength/2);
	m_pPlaneLineVertices[6].diffuse = m_colorLineY;
	m_pPlaneLineVertices[7].diffuse = m_colorLineY;

	//zPlaneLine
    m_pPlaneLineVertices[8].position = D3DXVECTOR3(0,0,m_fLineLength/2);
	m_pPlaneLineVertices[9].position = D3DXVECTOR3(m_fLineLength/2,0,m_fLineLength/2);
	m_pPlaneLineVertices[8].diffuse = m_colorLineZ;
	m_pPlaneLineVertices[9].diffuse = m_colorLineZ;

	m_pPlaneLineVertices[10].position = D3DXVECTOR3(0,0,m_fLineLength/2);
	m_pPlaneLineVertices[11].position = D3DXVECTOR3(0,m_fLineLength/2,m_fLineLength/2);
	m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
	m_pPlaneLineVertices[11].diffuse = m_colorLineZ;

}
//-----------------------------------------------------------------------------
// ������: InitPlaneVertex()
// ��  ��: 
// ��  ;: ��ʼ������ƽ�涥��
//-----------------------------------------------------------------------------
//##ModelId=47EBC746038A
void AxisMove::InitPlaneVertex()
{
	//�߶�2���㣬3���߶ι�6����
	//ǰ��6�����Ӧxyz���ߵĶ˵㣬0,1->x, 2,3->y, 4,5->z
	//xLine
	m_pPlaneVertices[0].position = D3DXVECTOR3(0,0,0);
	m_pPlaneVertices[0].diffuse =  notSelectAxisPlaneColor; 
    m_pPlaneVertices[1].position = D3DXVECTOR3(0,m_fLineLength/2,0);
	m_pPlaneVertices[1].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[2].position = D3DXVECTOR3(m_fLineLength/2,m_fLineLength/2,0);
	m_pPlaneVertices[2].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[3].position = D3DXVECTOR3(m_fLineLength/2,0,0);
	m_pPlaneVertices[3].diffuse =  notSelectAxisPlaneColor; 
	
	m_pPlaneVertices[4].position = D3DXVECTOR3(0,0,m_fLineLength/2);
	m_pPlaneVertices[4].diffuse =  notSelectAxisPlaneColor; 
    m_pPlaneVertices[5].position = D3DXVECTOR3(0,m_fLineLength/2,m_fLineLength/2);
	m_pPlaneVertices[5].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[6].position = D3DXVECTOR3(0,m_fLineLength/2,0);
	m_pPlaneVertices[6].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[7].position = D3DXVECTOR3(0,0,0);
	m_pPlaneVertices[7].diffuse =  notSelectAxisPlaneColor; 

	m_pPlaneVertices[8].position = D3DXVECTOR3(0,0,m_fLineLength/2);
	m_pPlaneVertices[8].diffuse =  notSelectAxisPlaneColor; 
    m_pPlaneVertices[9].position = D3DXVECTOR3(0,0,0);
	m_pPlaneVertices[9].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[10].position = D3DXVECTOR3(m_fLineLength/2,0,0);
	m_pPlaneVertices[10].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[11].position = D3DXVECTOR3(m_fLineLength/2,0,m_fLineLength/2);
	m_pPlaneVertices[11].diffuse =  notSelectAxisPlaneColor; 
}
//-----------------------------------------------------------------------------
// ������: InitArrowVertex()
// ��  ��: 
// ��  ;: ��ʼ�����ͷ
//-----------------------------------------------------------------------------
//##ModelId=47EBC746037A
void AxisMove::InitArrowVertex()
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

//-----------------------------------------------------------------------------
// ������: CreatePlaneLineVertexBuf()
// ��  ��: 
// ��  ;: �������߶��㻺��
//-----------------------------------------------------------------------------
HRESULT AxisMove::CreatePlaneLineVertexBuf()
{
	//crate vertex buffer obj for axis line
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_PlaneLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_PlaneLineVBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//�����ڴ�
	if( FAILED(m_PlaneLineVBuffer->Lock(0, m_PlaneLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(void**)&m_pPlaneLineVertices, 0) ))
	{
		return E_FAIL;
	}
	
	//��ʼ���ᶥ��
	InitPlaneLineVertex();
	
	//Unlock���㻺��
	m_PlaneLineVBuffer->Unlock();
	
	return S_OK;
}
//-----------------------------------------------------------------------------
// ������: CreateArrowVertexBuf()
// ��  ��: 
// ��  ;: �������ͷ���㻺��
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603A9
HRESULT AxisMove::CreateArrowVertexBuf()
{
	//crate vertex buffer obj
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_ArrowVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_ArrowVBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//�����ڴ�
	if( FAILED(m_ArrowVBuffer->Lock(0, m_ArrowVertCnt * sizeof(VERTEXDECL_POSCOLOR), (void**)&m_pArrowVertices, 0) ))
	{
		return E_FAIL;
	}
	
	//��ʼ����ͷ
	InitArrowVertex();
	
	//Unlock���㻺��
	m_ArrowVBuffer->Unlock();
	
	return S_OK;
}


//-----------------------------------------------------------------------------
// ������: CreateArrowIndexBuf()
// ��  ��: 
// ��  ;: �������ͷ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603AA
HRESULT AxisMove::CreateArrowIndexBuf()
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
//-----------------------------------------------------------------------------
// ������: CreatePlaneVertexBuf()
// ��  ��: 
// ��  ;: ��������ƽ��ġ����㻺��
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603B9
HRESULT AxisMove::CreatePlaneVertexBuf()
{
	//crate vertex buffer obj
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_PlaneVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_PlaneVBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//�����ڴ�
	if( FAILED(m_PlaneVBuffer->Lock(0, m_PlaneVertCnt * sizeof(VERTEXDECL_POSCOLOR), (void**)&m_pPlaneVertices, 0) ))
	{
		return E_FAIL;
	}
	
	//��ʼ����ͷ
	InitPlaneVertex();
	
	//Unlock���㻺��
	m_PlaneVBuffer->Unlock();
	
	return S_OK;
}
//-----------------------------------------------------------------------------
// ������: CreatePlaneIndexBuf()
// ��  ��: 
// ��  ;: �������ͷ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603BA
HRESULT AxisMove::CreatePlaneIndexBuf()
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
	if( FAILED(m_PlaneIBuffer->Lock(0, m_PlaneFaceCnt *3* sizeof(UINT), (void**)&m_pPlaneIndices, 0) ))
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

	
	//Unlock��������
	m_PlaneIBuffer->Unlock();	
	
	return S_OK;
}
void AxisMove::DrawPlaneLine()
{
	//��Ⱦģ��
	m_pD3dDevice->SetStreamSource( 0, m_PlaneLineVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pD3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//������
	m_pD3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 6);
}

void AxisMove::DrawArrow()
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

void AxisMove::DrawPlane()
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

void AxisMove::Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection)
{
	ClearPreDeviceSettings();
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &m_transform.matrix);
	DrawLine();
	DrawArrow();
	UpdateAxisTextsPos(pViewport, pView, pProjection);
	DrawAxisText();
// 	if (m_nSelectedAxisStatus > 0 && m_nSelectedAxisStatus < 4)
// 		DrawAxisBox(m_nSelectedAxisStatus);
	DrawPlaneLine();
	DrawPlane();

// 	m_pD3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
// 	m_pD3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
}
void AxisMove::SelectAxis(UINT selectStatus)
{
	//����Axis���ƶ�����ת
	//�����ڴ�
	if( FAILED(m_LineVBuffer->Lock(0, m_nLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(void**)&m_pLineVertices, 0) ))
	{
		return ;
	}
	if( FAILED(m_PlaneLineVBuffer->Lock(0, m_PlaneLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(void**)&m_pPlaneLineVertices, 0) ))
	{
		return ;
	}
	if( FAILED(m_PlaneVBuffer->Lock(0, m_PlaneVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(void**)&m_pPlaneVertices, 0) ))
	{
		return ;
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
        
		for(i=0;i<12;i++){
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

		for(i=0;i<12;i++){
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
		
		for(i=0;i<12;i++){
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
        
		for(i=0;i<12;i++){
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
		
		m_pPlaneLineVertices[2].diffuse = m_colorLineX;
		m_pPlaneLineVertices[3].diffuse = m_colorLineX;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = m_colorSelectedAxis;
		m_pPlaneLineVertices[5].diffuse  = m_colorSelectedAxis;
		
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
        
		for(i=0;i<4;i++){
		  m_pPlaneVertices[i].diffuse =  selectAxisPlaneColor; 
		} 
		for(i=4;i<12;i++){
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
		m_pPlaneLineVertices[4].diffuse  = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse  = m_colorLineY;
		
		m_pPlaneLineVertices[6].diffuse  = m_colorSelectedAxis;
		m_pPlaneLineVertices[7].diffuse  = m_colorSelectedAxis;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = m_colorLineZ;
		m_pPlaneLineVertices[9].diffuse  = m_colorLineZ;
		
		m_pPlaneLineVertices[10].diffuse = m_colorSelectedAxis;
		m_pPlaneLineVertices[11].diffuse = m_colorSelectedAxis;
        
		m_colorTextX=m_colorLineX;
		m_colorTextY=m_colorSelectedAxis;
		m_colorTextZ=m_colorSelectedAxis;
		
		for(i=0;i<4;i++){
		  m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		for(i=4;i<8;i++){
		  m_pPlaneVertices[i].diffuse =  selectAxisPlaneColor; 
		} 
		for(i=8;i<12;i++){
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
		
		m_pPlaneLineVertices[2].diffuse = m_colorSelectedAxis;
		m_pPlaneLineVertices[3].diffuse = m_colorSelectedAxis;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = m_colorLineY;
		m_pPlaneLineVertices[5].diffuse  = m_colorLineY;
		
		m_pPlaneLineVertices[6].diffuse  = m_colorLineY;
		m_pPlaneLineVertices[7].diffuse  = m_colorLineY;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = m_colorSelectedAxis;
		m_pPlaneLineVertices[9].diffuse  = m_colorSelectedAxis;
		
		m_pPlaneLineVertices[10].diffuse = m_colorLineZ;
		m_pPlaneLineVertices[11].diffuse = m_colorLineZ;

		m_colorTextX=m_colorSelectedAxis;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorSelectedAxis;
        
		for(i=0;i<8;i++){
		  m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		for(i=8;i<12;i++){
		  m_pPlaneVertices[i].diffuse =  selectAxisPlaneColor; 
		}
		break;
	}
	//Unlock���㻺��
	m_LineVBuffer->Unlock();
    m_PlaneLineVBuffer->Unlock();
	m_PlaneVBuffer->Unlock();

	m_nSelectedAxisStatus=selectStatus;
}

int AxisMove::IntersectAxisPlane(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir)
{	
	BOOL bResult = FALSE;
	FLOAT tDist = 1000000.0f;

	D3DXVECTOR3 pos[3];
	float dist = 0.0f;
	int n = 0;


	//XY
	for (int i = 0; n < 3; n++, i++)
	{
		pos[i] =  m_pPlaneVertices[m_pPlaneIndices[n]].position;
		D3DXVec3TransformCoord(&pos[i], &pos[i], &m_transform.matrix);
	}
	if (D3DXIntersectTri(&pos[0], &pos[1], &pos[2], &orig, &dir, 0, 0, &dist) )
	{
		return 4;
	}
	for (int i = 0; n < 6; n++, i++)
	{
		pos[i] =  m_pPlaneVertices[m_pPlaneIndices[n]].position;
		D3DXVec3TransformCoord(&pos[i], &pos[i], &m_transform.matrix);
	}
	if (D3DXIntersectTri(&pos[0], &pos[1], &pos[2], &orig, &dir, 0, 0, &dist) )
	{
		return 4;
	}
	
	//YZ
	for (int i = 0; n < 9; n++, i++)
	{
		pos[i] =  m_pPlaneVertices[m_pPlaneIndices[n]].position;
		D3DXVec3TransformCoord(&pos[i], &pos[i], &m_transform.matrix);
	}
	if (D3DXIntersectTri(&pos[0], &pos[1], &pos[2], &orig, &dir, 0, 0, &dist) )
	{
		return 5;
	}
	for (int i = 0; n < 12; n++, i++)
	{
		pos[i] =  m_pPlaneVertices[m_pPlaneIndices[n]].position;
		D3DXVec3TransformCoord(&pos[i], &pos[i], &m_transform.matrix);
	}
	if (D3DXIntersectTri(&pos[0], &pos[1], &pos[2], &orig, &dir, 0, 0, &dist) )
	{
		return 5;
	}

	//XZ
	for (int i = 0; n < 15; n++, i++)
	{
		pos[i] =  m_pPlaneVertices[m_pPlaneIndices[n]].position;
		D3DXVec3TransformCoord(&pos[i], &pos[i], &m_transform.matrix);
	}
	if (D3DXIntersectTri(&pos[0], &pos[1], &pos[2], &orig, &dir, 0, 0, &dist) )
	{
		return 6;
	}
	for (int i = 0; n < 18; n++, i++)
	{
		pos[i] =  m_pPlaneVertices[m_pPlaneIndices[n]].position;
		D3DXVec3TransformCoord(&pos[i], &pos[i], &m_transform.matrix);
	}
	if (D3DXIntersectTri(&pos[0], &pos[1], &pos[2], &orig, &dir, 0, 0, &dist) )
	{
		return 6;
	}

	return 0;
}
int AxisMove::GetModelAxisStatusWithRay(const D3DXVECTOR3& pos , const D3DXVECTOR3& dir)
{
	//ѡ����
	int n = IntersectAxisLine(pos, dir);
	if (n <= 0 || n > 3)
	{
		//��
		n =  IntersectAxisPlane(pos, dir);
	}
	SelectAxis(n);
	return n;
}
