/////////////////////////////////////////////////////////////////////////
// ��  ��: CMoveAxis.cpp
// ��  ��: XYZ��ģ�Ͷ����壬���ڵ�ѡ����ʱ��ʾ
// ��  ��: lixin
// ��  ��: 2006-05-15
/////////////////////////////////////////////////////////////////////////

#include "CMoveAxis.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74703A9
CMoveAxis::CMoveAxis(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice = pd3dDevice;
	m_pLineVertices = NULL;
	m_pArrowVertices = NULL;
	m_pArrowIndices = NULL;

	m_Scale = 1.0f;
	AxisXTextScreenPos.left=-10;
    AxisXTextScreenPos.top=-10;
    AxisYTextScreenPos.left=-10;
    AxisYTextScreenPos.top=-10;
    AxisZTextScreenPos.left=-10;
    AxisZTextScreenPos.top=-10;
	
	//�������ߺͼ�ͷ����ɫ
	xLineColor = D3DCOLOR_RGBA(255, 0, 0, 255);	
	xArrowColor = D3DCOLOR_RGBA(255, 0, 0, 255);
	yLineColor = D3DCOLOR_RGBA(0, 255, 0, 255);
	yArrowColor = D3DCOLOR_RGBA(0, 255, 0, 255);
	zLineColor = D3DCOLOR_RGBA(0, 0, 255, 255);
	zArrowColor = D3DCOLOR_RGBA(0, 0, 255, 255);
	selectAxisColor =D3DCOLOR_RGBA(255,255,0,255);
	selectAxisPlaneColor =D3DCOLOR_RGBA(255,255,0,128);
	notSelectAxisPlaneColor=D3DCOLOR_RGBA(255,255,0,0);

	xAxisTextColor = xLineColor;
	yAxisTextColor = yLineColor;
	zAxisTextColor = zLineColor;
	//�������ߵĳ���
	lineLength = 30.0f;
	//���ü�ͷ�İ뾶
	arrowRadius = 2.0f;
	//���ü�ͷ�ĸ߶�
	arrowHeight = 10.0f;
	FLOAT tLength=lineLength+arrowHeight+5.0f;
	ori_AxisTextXpos=D3DXVECTOR3(tLength,0.0f,0.0f);
	ori_AxisTextYpos=D3DXVECTOR3(0.0f,tLength,0.0f);
	ori_AxisTextZpos=D3DXVECTOR3(0.0f,0.0f,tLength);
    ori_AxisPos=D3DXVECTOR3(0.0f,0.0f,0.0f);
	axisTextXpos=ori_AxisTextXpos;
    axisTextYpos=ori_AxisTextYpos;
	axisTextZpos=ori_AxisTextZpos;
    axisPos=ori_AxisPos;
	//���ü�ͷ�ĵ���Ķ���εı���
	arrowCornerCnt = 6;
	
	m_LineVertCnt = 6;
    m_PlaneLineVertCnt = 12;
	//��������ƽ��ı���
	m_PlaneFaceCnt =6;
	m_ArrowVertCnt = (arrowCornerCnt +1)*3;
	m_ArrowFaceCnt = (arrowCornerCnt + (arrowCornerCnt -2)) * 3; 
    //��������ƽ�涥��ĸ���
	m_PlaneVertCnt =12;

	ori_xAxisMinCorner=D3DXVECTOR3(lineLength/5,-arrowRadius,-arrowRadius);
    ori_xAxisMaxCorner=D3DXVECTOR3(lineLength+arrowHeight,arrowRadius,arrowRadius);
	
	ori_yAxisMinCorner= D3DXVECTOR3(-arrowRadius,lineLength/5,-arrowRadius);
	ori_yAxisMaxCorner= D3DXVECTOR3(arrowRadius,lineLength+arrowHeight,arrowRadius);
	
	ori_zAxisMinCorner= D3DXVECTOR3(-arrowRadius,-arrowRadius,lineLength/5);
	ori_zAxisMaxCorner= D3DXVECTOR3(arrowRadius,arrowRadius,lineLength+arrowHeight);
	
	xAxisMinCorner=ori_xAxisMinCorner;
    xAxisMaxCorner=ori_xAxisMaxCorner;
    yAxisMinCorner=ori_yAxisMinCorner;
	yAxisMaxCorner=ori_yAxisMaxCorner;
	zAxisMinCorner=ori_zAxisMinCorner;
	zAxisMaxCorner=ori_zAxisMaxCorner;

    //x���Χ�ж��㻺��
	xAxisBoundBoxVertics[0] = D3DXVECTOR3(lineLength/5,-arrowRadius,-arrowRadius);
	xAxisBoundBoxVertics[1] = D3DXVECTOR3(lineLength/5, arrowRadius,-arrowRadius);
	xAxisBoundBoxVertics[2] = D3DXVECTOR3(lineLength+arrowHeight, arrowRadius,-arrowRadius);
	xAxisBoundBoxVertics[3] = D3DXVECTOR3(lineLength+arrowHeight,-arrowRadius,-arrowRadius);
	xAxisBoundBoxVertics[4] = D3DXVECTOR3(lineLength/5,-arrowRadius,arrowRadius);
	xAxisBoundBoxVertics[5] = D3DXVECTOR3(lineLength+arrowHeight,-arrowRadius,arrowRadius);
    xAxisBoundBoxVertics[6] = D3DXVECTOR3(lineLength+arrowHeight,arrowRadius,arrowRadius);
	xAxisBoundBoxVertics[7] = D3DXVECTOR3(lineLength/5, arrowRadius, arrowRadius); 
    //y���Χ�ж��㻺��
    yAxisBoundBoxVertics[0] = D3DXVECTOR3(-arrowRadius,lineLength/5,-arrowRadius);
	yAxisBoundBoxVertics[1] = D3DXVECTOR3(-arrowRadius,lineLength+arrowHeight,-arrowRadius);
	yAxisBoundBoxVertics[2] = D3DXVECTOR3(arrowRadius,lineLength+arrowHeight,-arrowRadius);
	yAxisBoundBoxVertics[3] = D3DXVECTOR3(arrowRadius,lineLength/5,-arrowRadius);
	yAxisBoundBoxVertics[4] = D3DXVECTOR3(-arrowRadius,lineLength/5,arrowRadius);
	yAxisBoundBoxVertics[5] = D3DXVECTOR3(arrowRadius,lineLength/5, arrowRadius);
    yAxisBoundBoxVertics[6] = D3DXVECTOR3(arrowRadius,lineLength+arrowHeight,arrowRadius);
	yAxisBoundBoxVertics[7] = D3DXVECTOR3(-arrowRadius,lineLength+arrowHeight, arrowRadius); 
    //z���Χ�ж��㻺��
	zAxisBoundBoxVertics[0] = D3DXVECTOR3(-arrowRadius,-arrowRadius,lineLength/5);
	zAxisBoundBoxVertics[1] = D3DXVECTOR3(-arrowRadius, arrowRadius,lineLength/5);
	zAxisBoundBoxVertics[2] = D3DXVECTOR3( arrowRadius, arrowRadius,lineLength/5);
	zAxisBoundBoxVertics[3] = D3DXVECTOR3( arrowRadius,-arrowRadius,lineLength/5);
	zAxisBoundBoxVertics[4] = D3DXVECTOR3(-arrowRadius,-arrowRadius,lineLength+arrowHeight);
	zAxisBoundBoxVertics[5] = D3DXVECTOR3(arrowRadius,-arrowRadius,lineLength+arrowHeight);
    zAxisBoundBoxVertics[6] = D3DXVECTOR3(arrowRadius,arrowRadius,lineLength+arrowHeight);
	zAxisBoundBoxVertics[7] = D3DXVECTOR3(-arrowRadius,arrowRadius,lineLength+arrowHeight); 
    //���ᷨ�߷�������
	//face 1
	AxisBoundBoxNormal[0]= D3DXVECTOR3(0.0f,0.0f,-1.0f); 
    AxisBoundBoxNormal[1]= D3DXVECTOR3(0.0f,0.0f,-1.0f); 
    //face 2
	AxisBoundBoxNormal[2]= D3DXVECTOR3(0.0f,0.0f,1.0f); 
    AxisBoundBoxNormal[3]= D3DXVECTOR3(0.0f,0.0f,1.0f);
    //face 3
	AxisBoundBoxNormal[4]= D3DXVECTOR3(0.0f,-1.0f,0.0f);
	AxisBoundBoxNormal[5]= D3DXVECTOR3(0.0f,-1.0f,0.0f);
    //face 4
	AxisBoundBoxNormal[6]= D3DXVECTOR3(1.0f,0.0f,0.0f);
	AxisBoundBoxNormal[7]= D3DXVECTOR3(1.0f,0.0f,0.0f);
    //face 5
	AxisBoundBoxNormal[8]= D3DXVECTOR3(0.0f,1.0f,0.0f);
	AxisBoundBoxNormal[9]= D3DXVECTOR3(0.0f,1.0f,0.0f);
	//face 6
	AxisBoundBoxNormal[10]= D3DXVECTOR3(-1.0f,0.0f,0.0f);
	AxisBoundBoxNormal[11]= D3DXVECTOR3(-1.0f,0.0f,0.0f);
	//�������������
    // triangle 1
	AxisBoundBoxIndex[0]=0;
	AxisBoundBoxIndex[1]=1;
	AxisBoundBoxIndex[2]=2;
    // triangle 2
	AxisBoundBoxIndex[3]=2;
	AxisBoundBoxIndex[4]=3;
	AxisBoundBoxIndex[5]=0;
	// triangle 3
	AxisBoundBoxIndex[6]=4;
	AxisBoundBoxIndex[7]=5;
	AxisBoundBoxIndex[8]=6;
	// triangle 4
	AxisBoundBoxIndex[9]=6;
	AxisBoundBoxIndex[10]=7;
	AxisBoundBoxIndex[11]=4;
	// triangle 5
	AxisBoundBoxIndex[12]=0;
	AxisBoundBoxIndex[13]=3;
	AxisBoundBoxIndex[14]=5;
	// triangle 6
	AxisBoundBoxIndex[15]=5;
	AxisBoundBoxIndex[16]=4;
	AxisBoundBoxIndex[17]=0;
	// triangle 7
	AxisBoundBoxIndex[18]=3;
	AxisBoundBoxIndex[19]=2;
	AxisBoundBoxIndex[20]=6;
	// triangle 8
	AxisBoundBoxIndex[21]=6;
	AxisBoundBoxIndex[22]=5;
	AxisBoundBoxIndex[23]=3;
	// triangle 9
	AxisBoundBoxIndex[24]=2;
	AxisBoundBoxIndex[25]=1;
	AxisBoundBoxIndex[26]=7;
	// triangle 10
	AxisBoundBoxIndex[27]=7;
	AxisBoundBoxIndex[28]=6;
	AxisBoundBoxIndex[29]=2;
	// triangle 11
	AxisBoundBoxIndex[30]=1;
	AxisBoundBoxIndex[31]=0;
	AxisBoundBoxIndex[32]=4;
	// triangle 12
	AxisBoundBoxIndex[33]=4;
	AxisBoundBoxIndex[34]=7;
	AxisBoundBoxIndex[35]=1;
    //x�����߷�������
    xAxisRay=D3DXVECTOR3(0.0f,0.0f,0.0f);
	//y�����߷�������
    yAxisRay=D3DXVECTOR3(0.0f,0.0f,0.0f);
	//z�����߷�������
    zAxisRay=D3DXVECTOR3(0.0f,0.0f,0.0f);

	planesUnderCursor[0]=0;
    planesUnderCursor[1]=0;
	selectAxisStatus=0;
	
	pFont = NULL;

    Create();
}

//##ModelId=47EBC74703B9
CMoveAxis::~CMoveAxis()
{
   	SAFE_RELEASE(pFont);
	SAFE_DELETE(pFont);	
}

//##ModelId=47EBC747038A
VOID CMoveAxis::Create()
{
	pFont = new CFont(m_pd3dDevice);
    pFont->Init("MOVEAXIS");
	//������ģ��
	if (FAILED( CreateLineVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ���㻺�����"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);
	
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
//##ModelId=47EBC7470399
VOID CMoveAxis::Restore()
{
    if(pFont)
	    pFont->Restore();
	//������ģ��
	if (FAILED( CreateLineVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("����Arrow-XYZ���㻺�����"), TEXT("VGS2.0������Ϣ��ʾ"), MB_OK | MB_ICONERROR);
	
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
// ������: InitLineVertex()
// ��  ��: 
// ��  ;: ��ʼ�����߶���
//-----------------------------------------------------------------------------
//##ModelId=47EBC746036B
VOID CMoveAxis::InitLineVertex()
{
	//�߶�2���㣬3���߶ι�6����
	//ǰ��6�����Ӧxyz���ߵĶ˵㣬0,1->x, 2,3->y, 4,5->z
	//xLine
	m_pLineVertices[0].position = D3DXVECTOR3(lineLength/5,0,0);
	m_pLineVertices[1].position = D3DXVECTOR3(lineLength,0,0);
	m_pLineVertices[0].diffuse = xLineColor;
	m_pLineVertices[1].diffuse = xLineColor;
	
	//yLine
	m_pLineVertices[2].position = D3DXVECTOR3(0,lineLength/5,0);
	m_pLineVertices[3].position = D3DXVECTOR3(0,lineLength,0);
	m_pLineVertices[2].diffuse = yLineColor;
	m_pLineVertices[3].diffuse = yLineColor;
	
	//zLine
	m_pLineVertices[4].position = D3DXVECTOR3(0,0,lineLength/5);
	m_pLineVertices[5].position = D3DXVECTOR3(0,0,lineLength);
	m_pLineVertices[4].diffuse = zLineColor;
	m_pLineVertices[5].diffuse = zLineColor;
}
//-----------------------------------------------------------------------------
// ������: InitPlaneLineVertex()
// ��  ��: 
// ��  ;: ��ʼ�����߶���
//-----------------------------------------------------------------------------
//##ModelId=47EBC746036C
VOID CMoveAxis::InitPlaneLineVertex()
{
	//�߶�2���㣬3���߶ι�6����
	//ǰ��6�����Ӧxyz���ߵĶ˵㣬0,1->x, 2,3->y, 4,5->z
	//xPlaneLine
    m_pPlaneLineVertices[0].position = D3DXVECTOR3(lineLength/2,0,0);
	m_pPlaneLineVertices[1].position = D3DXVECTOR3(lineLength/2,lineLength/2,0);
	m_pPlaneLineVertices[0].diffuse = xLineColor;
	m_pPlaneLineVertices[1].diffuse = xLineColor;

	m_pPlaneLineVertices[2].position = D3DXVECTOR3(lineLength/2,0,0);
	m_pPlaneLineVertices[3].position = D3DXVECTOR3(lineLength/2,0,lineLength/2);
	m_pPlaneLineVertices[2].diffuse = xLineColor;
	m_pPlaneLineVertices[3].diffuse = xLineColor;

	//yPlaneLine
    m_pPlaneLineVertices[4].position = D3DXVECTOR3(0,lineLength/2,0);
	m_pPlaneLineVertices[5].position = D3DXVECTOR3(lineLength/2,lineLength/2,0);
	m_pPlaneLineVertices[4].diffuse = yLineColor;
	m_pPlaneLineVertices[5].diffuse = yLineColor;

	m_pPlaneLineVertices[6].position = D3DXVECTOR3(0,lineLength/2,0);
	m_pPlaneLineVertices[7].position = D3DXVECTOR3(0,lineLength/2,lineLength/2);
	m_pPlaneLineVertices[6].diffuse = yLineColor;
	m_pPlaneLineVertices[7].diffuse = yLineColor;

	//zPlaneLine
    m_pPlaneLineVertices[8].position = D3DXVECTOR3(0,0,lineLength/2);
	m_pPlaneLineVertices[9].position = D3DXVECTOR3(lineLength/2,0,lineLength/2);
	m_pPlaneLineVertices[8].diffuse = zLineColor;
	m_pPlaneLineVertices[9].diffuse = zLineColor;

	m_pPlaneLineVertices[10].position = D3DXVECTOR3(0,0,lineLength/2);
	m_pPlaneLineVertices[11].position = D3DXVECTOR3(0,lineLength/2,lineLength/2);
	m_pPlaneLineVertices[10].diffuse = zLineColor;
	m_pPlaneLineVertices[11].diffuse = zLineColor;

}
//-----------------------------------------------------------------------------
// ������: InitPlaneVertex()
// ��  ��: 
// ��  ;: ��ʼ������ƽ�涥��
//-----------------------------------------------------------------------------
//##ModelId=47EBC746038A
VOID CMoveAxis::InitPlaneVertex()
{
	//�߶�2���㣬3���߶ι�6����
	//ǰ��6�����Ӧxyz���ߵĶ˵㣬0,1->x, 2,3->y, 4,5->z
	//xLine
	m_pPlaneVertices[0].position = D3DXVECTOR3(0,0,0);
	m_pPlaneVertices[0].diffuse =  notSelectAxisPlaneColor; 
    m_pPlaneVertices[1].position = D3DXVECTOR3(0,lineLength/2,0);
	m_pPlaneVertices[1].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[2].position = D3DXVECTOR3(lineLength/2,lineLength/2,0);
	m_pPlaneVertices[2].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[3].position = D3DXVECTOR3(lineLength/2,0,0);
	m_pPlaneVertices[3].diffuse =  notSelectAxisPlaneColor; 
	
	m_pPlaneVertices[4].position = D3DXVECTOR3(0,0,lineLength/2);
	m_pPlaneVertices[4].diffuse =  notSelectAxisPlaneColor; 
    m_pPlaneVertices[5].position = D3DXVECTOR3(0,lineLength/2,lineLength/2);
	m_pPlaneVertices[5].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[6].position = D3DXVECTOR3(0,lineLength/2,0);
	m_pPlaneVertices[6].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[7].position = D3DXVECTOR3(0,0,0);
	m_pPlaneVertices[7].diffuse =  notSelectAxisPlaneColor; 

	m_pPlaneVertices[8].position = D3DXVECTOR3(0,0,lineLength/2);
	m_pPlaneVertices[8].diffuse =  notSelectAxisPlaneColor; 
    m_pPlaneVertices[9].position = D3DXVECTOR3(0,0,0);
	m_pPlaneVertices[9].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[10].position = D3DXVECTOR3(lineLength/2,0,0);
	m_pPlaneVertices[10].diffuse =  notSelectAxisPlaneColor; 
	m_pPlaneVertices[11].position = D3DXVECTOR3(lineLength/2,0,lineLength/2);
	m_pPlaneVertices[11].diffuse =  notSelectAxisPlaneColor; 
}
//-----------------------------------------------------------------------------
// ������: InitArrowVertex()
// ��  ��: 
// ��  ;: ��ʼ�����ͷ
//-----------------------------------------------------------------------------
//##ModelId=47EBC746037A
VOID CMoveAxis::InitArrowVertex()
{
	//��ͷĬ����6��׶��һ����ͷ7���㣬������ͷ��21����
	//x-��ͷ�˵�
	m_pArrowVertices[0].position = D3DXVECTOR3(lineLength+arrowHeight,0,0);
	m_pArrowVertices[0].diffuse = xArrowColor;
	//x-��ͷ���涥��
	UINT index = 1;
	//����ÿ������ǶȲ���
	FLOAT stepCorner = PI2/arrowCornerCnt;
	for (UINT i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.x = lineLength;
		m_pArrowVertices[i].position.y = (FLOAT)sin((i-index)*stepCorner)*arrowRadius;
		m_pArrowVertices[i].position.z = -(FLOAT)cos((i-index)*stepCorner)*arrowRadius;
		m_pArrowVertices[i].diffuse = xArrowColor;
	}
	index = i;
	
	//y-��ͷ�˵�
	m_pArrowVertices[index].position = D3DXVECTOR3(0,lineLength+arrowHeight,0);
	m_pArrowVertices[index].diffuse = yArrowColor;
	index++;
	//y-��ͷ���涥��
	//����ÿ������ǶȲ���
	for (i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.x = (FLOAT)cos((i-index)*stepCorner)*arrowRadius;
		m_pArrowVertices[i].position.y = lineLength;
		m_pArrowVertices[i].position.z = -(FLOAT)sin((i-index)*stepCorner)*arrowRadius;
		m_pArrowVertices[i].diffuse = yArrowColor;
	}
	index = i;
	
	//z-��ͷ�˵�
	m_pArrowVertices[index].position = D3DXVECTOR3(0,0,lineLength+arrowHeight);
	m_pArrowVertices[index].diffuse = zArrowColor;
	index++;
	//z-��ͷ���涥��
	//����ÿ������ǶȲ���
	for (i = index; i< index + arrowCornerCnt; i++)
	{
		m_pArrowVertices[i].position.x = (FLOAT)cos((i-index)*stepCorner)*arrowRadius;
		m_pArrowVertices[i].position.y = (FLOAT)sin((i-index)*stepCorner)*arrowRadius;
		m_pArrowVertices[i].position.z = lineLength;
		m_pArrowVertices[i].diffuse = zArrowColor;
	}
}

//-----------------------------------------------------------------------------
// ������: CreateLineVertexBuf()
// ��  ��: 
// ��  ;: �������߶��㻺��
//-----------------------------------------------------------------------------
//##ModelId=47EBC746038B
HRESULT CMoveAxis::CreateLineVertexBuf()
{
	//crate vertex buffer obj for axis line
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(m_LineVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_LineVBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//�����ڴ�
	if( FAILED(m_LineVBuffer->Lock(0, m_LineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pLineVertices, 0) ))
	{
		return E_FAIL;
	}
	
	//��ʼ���ᶥ��
	InitLineVertex();
	
	//Unlock���㻺��
	m_LineVBuffer->Unlock();
	
	return S_OK;
}
//-----------------------------------------------------------------------------
// ������: CreatePlaneLineVertexBuf()
// ��  ��: 
// ��  ;: �������߶��㻺��
//-----------------------------------------------------------------------------
//##ModelId=47EBC7460399
HRESULT CMoveAxis::CreatePlaneLineVertexBuf()
{
	//crate vertex buffer obj for axis line
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(m_PlaneLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),
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
//-----------------------------------------------------------------------------
// ������: CreateArrowVertexBuf()
// ��  ��: 
// ��  ;: �������ͷ���㻺��
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603A9
HRESULT CMoveAxis::CreateArrowVertexBuf()
{
	//crate vertex buffer obj
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(m_ArrowVertCnt * sizeof(VERTEXDECL_POSCOLOR),
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


//-----------------------------------------------------------------------------
// ������: CreateArrowIndexBuf()
// ��  ��: 
// ��  ;: �������ͷ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603AA
HRESULT CMoveAxis::CreateArrowIndexBuf()
{	
	//������ǰģ�͵���������
	if( FAILED( m_pd3dDevice ->CreateIndexBuffer(m_ArrowFaceCnt * 3 * sizeof(UINT),
		0,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_ArrowIBuffer, NULL) ) )
	{
		return E_FAIL;
	}
	
	//�����ڴ�
	if( FAILED(m_ArrowIBuffer->Lock(0, m_ArrowFaceCnt *3* sizeof(UINT), (VOID**)&m_pArrowIndices, 0) ))
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
HRESULT CMoveAxis::CreatePlaneVertexBuf()
{
	//crate vertex buffer obj
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(m_PlaneVertCnt * sizeof(VERTEXDECL_POSCOLOR),
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
//-----------------------------------------------------------------------------
// ������: CreatePlaneIndexBuf()
// ��  ��: 
// ��  ;: �������ͷ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603BA
HRESULT CMoveAxis::CreatePlaneIndexBuf()
{	
	//������ǰģ�͵���������
	if( FAILED( m_pd3dDevice ->CreateIndexBuffer(m_PlaneFaceCnt * 3 * sizeof(UINT),
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

	
	//Unlock��������
	m_PlaneIBuffer->Unlock();	
	
	return S_OK;
}
//-----------------------------------------------------------------------------
// ������: DrawLine()
// ��  ��: 
// ��  ;: �����߻�����
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603BB
VOID CMoveAxis::DrawLine()
{
	//��Ⱦģ��
	m_pd3dDevice->SetStreamSource( 0, m_LineVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pd3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//������
	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);
}
//-----------------------------------------------------------------------------
// ������: DrawPlaneLine()
// ��  ��: 
// ��  ;: �����߻�����
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603C8
VOID CMoveAxis::DrawPlaneLine()
{
	//��Ⱦģ��
	m_pd3dDevice->SetStreamSource( 0, m_PlaneLineVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pd3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//������
	m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 6);
}


//-----------------------------------------------------------------------------
// ������: DrawArrow()
// ��  ��: 
// ��  ;: �����ͷ������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603C9
VOID CMoveAxis::DrawArrow()
{	
	//��Ⱦģ��
	m_pd3dDevice->SetStreamSource( 0, m_ArrowVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pd3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//����IB
	m_pd3dDevice->SetIndices(m_ArrowIBuffer);
	
	//�ı������������������������ϴ���ȵ����������ٶ�**************
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST , 0, 0,
		m_ArrowVertCnt, //��ǰģ�͵Ķ�����
		0, //��ǰģ�͵��������������
		m_ArrowFaceCnt); //��ǰģ�͵�����		
}

//-----------------------------------------------------------------------------
// ������: DrawPlane()
// ��  ��: 
// ��  ;: �����ͷ������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603D8
VOID CMoveAxis::DrawPlane()
{	
	//����͸����ɫΪ��״̬
	//��Alpha���
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE); 
	//��Ⱦģ��
	m_pd3dDevice->SetStreamSource( 0, m_PlaneVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//����FVF
	m_pd3dDevice->SetFVF(FVF_POSCOLOR); //DX9ΪSetFVF,DX8���������
	//����IB
	m_pd3dDevice->SetIndices(m_PlaneIBuffer);
	m_pd3dDevice->SetStreamSource( 0, m_PlaneVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//�ı������������������������ϴ���ȵ����������ٶ�**************
	m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST , 0, 0,
		m_PlaneVertCnt, //��ǰģ�͵Ķ�����
		0, //��ǰģ�͵��������������
		m_PlaneFaceCnt); //��ǰģ�͵�����	
 	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW); 
	
	//�ر�Alpha���
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}
//-----------------------------------------------------------------------------
// ������: DrawAxisText()
// ��  ��: 
// ��  ;: �����ͷ������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603D9
VOID CMoveAxis::DrawAxisText()
{	
  pFont->DrawText("x", &AxisXTextScreenPos, DT_LEFT, xAxisTextColor);
  pFont->DrawText("y", &AxisYTextScreenPos, DT_LEFT, yAxisTextColor);
  pFont->DrawText("z", &AxisZTextScreenPos, DT_LEFT, zAxisTextColor);
}
//-----------------------------------------------------------------------------
// ������: Draw()
// ��  ��: 
// ��  ;: ������ϵ������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74603DA
VOID CMoveAxis::Draw()
{
	//����Axis���ƶ�����ת
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &transform.matrix);
	DrawLine();
	DrawArrow();
	DrawAxisText();
	DrawPlaneLine();
	DrawPlane();
}

//-----------------------------------------------------------------------------
// ������: SeleteAxis()
// ��  ��: UINT seleteStatus       1��ѡ��x�ᣬ2��ѡ��y�ᣬ3��ѡ��z�ᣬ0����ѡ���κ���
// ��  ;: ѡ����ķ���
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470070
HRESULT CMoveAxis::SelectAxis(UINT selectStatus)
{
	//����Axis���ƶ�����ת
	//�����ڴ�
	if( FAILED(m_LineVBuffer->Lock(0, m_LineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pLineVertices, 0) ))
	{
		return E_FAIL;
	}
	if( FAILED(m_PlaneLineVBuffer->Lock(0, m_PlaneLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pPlaneLineVertices, 0) ))
	{
		return E_FAIL;
	}
	if( FAILED(m_PlaneVBuffer->Lock(0, m_PlaneVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pPlaneVertices, 0) ))
	{
		return E_FAIL;
	}
	UINT i;
	switch(selectStatus){
	case 0:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = xLineColor;
		m_pLineVertices[1].diffuse = xLineColor;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = yLineColor;
		m_pLineVertices[3].diffuse = yLineColor;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = zLineColor;
		m_pLineVertices[5].diffuse = zLineColor;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = xLineColor;
		m_pPlaneLineVertices[1].diffuse = xLineColor;
		
		m_pPlaneLineVertices[2].diffuse = xLineColor;
		m_pPlaneLineVertices[3].diffuse = xLineColor;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse = yLineColor;
		m_pPlaneLineVertices[5].diffuse = yLineColor;
		
		m_pPlaneLineVertices[6].diffuse = yLineColor;
		m_pPlaneLineVertices[7].diffuse = yLineColor;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse = zLineColor;
		m_pPlaneLineVertices[9].diffuse = zLineColor;
		
		m_pPlaneLineVertices[10].diffuse = zLineColor;
		m_pPlaneLineVertices[11].diffuse = zLineColor;

		xAxisTextColor=xLineColor;
		yAxisTextColor=yLineColor;
		zAxisTextColor=zLineColor;
        
		for(i=0;i<12;i++){
		  m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;
		
	case 1:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = selectAxisColor;
		m_pLineVertices[1].diffuse = selectAxisColor;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = yLineColor;
		m_pLineVertices[3].diffuse = yLineColor;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = zLineColor;
		m_pLineVertices[5].diffuse = zLineColor;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = xLineColor;
		m_pPlaneLineVertices[1].diffuse = xLineColor;
		
		m_pPlaneLineVertices[2].diffuse = xLineColor;
		m_pPlaneLineVertices[3].diffuse = xLineColor;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse = yLineColor;
		m_pPlaneLineVertices[5].diffuse = yLineColor;
		
		m_pPlaneLineVertices[6].diffuse = yLineColor;
		m_pPlaneLineVertices[7].diffuse = yLineColor;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse = zLineColor;
		m_pPlaneLineVertices[9].diffuse = zLineColor;
		
		m_pPlaneLineVertices[10].diffuse = zLineColor;
		m_pPlaneLineVertices[11].diffuse = zLineColor;
        

	    xAxisTextColor=selectAxisColor;
		yAxisTextColor=yLineColor;
		zAxisTextColor=zLineColor;

		for(i=0;i<12;i++){
		  m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;
		
	case 2:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = xLineColor;
		m_pLineVertices[1].diffuse = xLineColor;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = selectAxisColor;
		m_pLineVertices[3].diffuse = selectAxisColor;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = zLineColor;
		m_pLineVertices[5].diffuse = zLineColor;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = xLineColor;
		m_pPlaneLineVertices[1].diffuse = xLineColor;
		
		m_pPlaneLineVertices[2].diffuse = xLineColor;
		m_pPlaneLineVertices[3].diffuse = xLineColor;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse = yLineColor;
		m_pPlaneLineVertices[5].diffuse = yLineColor;
		
		m_pPlaneLineVertices[6].diffuse = yLineColor;
		m_pPlaneLineVertices[7].diffuse = yLineColor;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse = zLineColor;
		m_pPlaneLineVertices[9].diffuse = zLineColor;
		
		m_pPlaneLineVertices[10].diffuse = zLineColor;
		m_pPlaneLineVertices[11].diffuse = zLineColor;
        
	    xAxisTextColor=xLineColor;
		yAxisTextColor=selectAxisColor;
		zAxisTextColor=zLineColor;
		
		for(i=0;i<12;i++){
		  m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;
		
	case 3:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = xLineColor;
		m_pLineVertices[1].diffuse = xLineColor;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = yLineColor;
		m_pLineVertices[3].diffuse = yLineColor;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = selectAxisColor;
		m_pLineVertices[5].diffuse = selectAxisColor;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = xLineColor;
		m_pPlaneLineVertices[1].diffuse = xLineColor;
		
		m_pPlaneLineVertices[2].diffuse = xLineColor;
		m_pPlaneLineVertices[3].diffuse = xLineColor;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse = yLineColor;
		m_pPlaneLineVertices[5].diffuse = yLineColor;
		
		m_pPlaneLineVertices[6].diffuse = yLineColor;
		m_pPlaneLineVertices[7].diffuse = yLineColor;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse = zLineColor;
		m_pPlaneLineVertices[9].diffuse = zLineColor;
		
		m_pPlaneLineVertices[10].diffuse = zLineColor;
		m_pPlaneLineVertices[11].diffuse = zLineColor;

		xAxisTextColor=xLineColor;
		yAxisTextColor=yLineColor;
		zAxisTextColor=selectAxisColor;
        
		for(i=0;i<12;i++){
		  m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
		break;
	case 4:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = selectAxisColor;
		m_pLineVertices[1].diffuse = selectAxisColor;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = selectAxisColor;
		m_pLineVertices[3].diffuse = selectAxisColor;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = zLineColor;
		m_pLineVertices[5].diffuse = zLineColor;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = selectAxisColor;
		m_pPlaneLineVertices[1].diffuse = selectAxisColor;
		
		m_pPlaneLineVertices[2].diffuse = xLineColor;
		m_pPlaneLineVertices[3].diffuse = xLineColor;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = selectAxisColor;
		m_pPlaneLineVertices[5].diffuse  = selectAxisColor;
		
		m_pPlaneLineVertices[6].diffuse  = yLineColor;
		m_pPlaneLineVertices[7].diffuse  = yLineColor;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = zLineColor;
		m_pPlaneLineVertices[9].diffuse  = zLineColor;
		
		m_pPlaneLineVertices[10].diffuse = zLineColor;
		m_pPlaneLineVertices[11].diffuse = zLineColor;

		xAxisTextColor=selectAxisColor;
		yAxisTextColor=selectAxisColor;
		zAxisTextColor=zLineColor;
        
		for(i=0;i<4;i++){
		  m_pPlaneVertices[i].diffuse =  selectAxisPlaneColor; 
		} 
		for(i=4;i<12;i++){
		  m_pPlaneVertices[i].diffuse =  notSelectAxisPlaneColor; 
		} 
        break;
	case 5:
		//����xline��ɫ
		m_pLineVertices[0].diffuse = xLineColor;
		m_pLineVertices[1].diffuse = xLineColor;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = selectAxisColor;
		m_pLineVertices[3].diffuse = selectAxisColor;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = selectAxisColor;
		m_pLineVertices[5].diffuse = selectAxisColor;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = xLineColor;
		m_pPlaneLineVertices[1].diffuse = xLineColor;
		
		m_pPlaneLineVertices[2].diffuse = xLineColor;
		m_pPlaneLineVertices[3].diffuse = xLineColor;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = yLineColor;
		m_pPlaneLineVertices[5].diffuse  = yLineColor;
		
		m_pPlaneLineVertices[6].diffuse  = selectAxisColor;
		m_pPlaneLineVertices[7].diffuse  = selectAxisColor;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = zLineColor;
		m_pPlaneLineVertices[9].diffuse  = zLineColor;
		
		m_pPlaneLineVertices[10].diffuse = selectAxisColor;
		m_pPlaneLineVertices[11].diffuse = selectAxisColor;
        
		xAxisTextColor=xLineColor;
		yAxisTextColor=selectAxisColor;
		zAxisTextColor=selectAxisColor;
		
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
		m_pLineVertices[0].diffuse = selectAxisColor;
		m_pLineVertices[1].diffuse = selectAxisColor;
		//����yline��ɫ
		m_pLineVertices[2].diffuse = yLineColor;
		m_pLineVertices[3].diffuse = yLineColor;
		//����zline��ɫ
		m_pLineVertices[4].diffuse = selectAxisColor;
		m_pLineVertices[5].diffuse = selectAxisColor;
		//xPlaneLine
		m_pPlaneLineVertices[0].diffuse = xLineColor;
		m_pPlaneLineVertices[1].diffuse = xLineColor;
		
		m_pPlaneLineVertices[2].diffuse = selectAxisColor;
		m_pPlaneLineVertices[3].diffuse = selectAxisColor;
		
		//yPlaneLine
		m_pPlaneLineVertices[4].diffuse  = yLineColor;
		m_pPlaneLineVertices[5].diffuse  = yLineColor;
		
		m_pPlaneLineVertices[6].diffuse  = yLineColor;
		m_pPlaneLineVertices[7].diffuse  = yLineColor;
		
		//zPlaneLine
		m_pPlaneLineVertices[8].diffuse  = selectAxisColor;
		m_pPlaneLineVertices[9].diffuse  = selectAxisColor;
		
		m_pPlaneLineVertices[10].diffuse = zLineColor;
		m_pPlaneLineVertices[11].diffuse = zLineColor;

		xAxisTextColor=selectAxisColor;
		yAxisTextColor=yLineColor;
		zAxisTextColor=selectAxisColor;
        
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

	selectAxisStatus=selectStatus;
	return S_OK;
}

//
//-----------------------------------------------------------------------------
// ������: SetTransformMatrix()
// ��  ��: 
// ��  ;: ���ñ任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470000
VOID CMoveAxis::SetTransformMatrix(D3DXMATRIX* pMat)
{
	transform.matrix = *pMat;
}	

//-----------------------------------------------------------------------------
// ������: SetScaleMatrix()
// ��  ��: D3DXMATRIX* pMat
// ��  ;: ���ñ任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470002
VOID CMoveAxis::SetScaleMatrix(D3DXMATRIX* pMat)
{
	transform.scale_matrix = *pMat;
}

//-----------------------------------------------------------------------------
// ������: SetRotationMatrix()
// ��  ��: D3DXMATRIX* pMat
// ��  ;: ���ñ任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470004
VOID CMoveAxis::SetRotationMatrix(D3DXMATRIX* pMat)
{
	transform.rotation_matrix = *pMat;
}


//-----------------------------------------------------------------------------
// ������: SetTranslateMatrix()
// ��  ��: D3DXMATRIX* pMat
// ��  ;: ���ñ任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC747000F
VOID CMoveAxis::SetTranslateMatrix(D3DXMATRIX* pMat)
{
	transform.translate_matrix = *pMat;
}
//-----------------------------------------------------------------------------
// ������: SetPosition()
// ��  ��: D3DXVECTOR3* pVec
// ��  ;: ����ģ�͵�λ��
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470013
VOID CMoveAxis::SetPosition(D3DXVECTOR3 *pVec)
{
  	//update Axis worldPosition
	transform.position = *pVec;
	//update matrix
	UpdateTranslateMatrix();
	MultiplyMatrix();
    UpdateAxisBoundBoxsVectors();
}
//-------------------------------------------------------------------
//����: Rotate()
//����: D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, FLOAT fAngle
//��;: ��ָ������ת
//-------------------------------------------------------------------
//##ModelId=47EBC7470022
VOID CMoveAxis::Rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, FLOAT fAngle)
{
	//��ת�任
	D3DXMATRIX mat_Ros;
	D3DXMatrixRotationAxis(&mat_Ros, pVec, -fAngle);
	D3DXMatrixMultiply(&transform.rotation_matrix, 
					   &transform.rotation_matrix, &mat_Ros);
	
	//�������ƶ�����ת������λ�ñ任
	D3DXMATRIX mat_Trans1, mat_Trans2;
	D3DXMatrixTranslation(&mat_Trans1, -pPos->x, -pPos->y, -pPos->z);
	D3DXMatrixTranslation(&mat_Trans2, pPos->x, pPos->y, pPos->z);
	//D3DXVec3TransformCoord()�������Խ���λ�ñ任
	D3DXVec3TransformCoord(&transform.position, &transform.position, &mat_Trans1);
	//D3DXVec3TransformNormal()ֻ�ܽ��нǶȱ任
	D3DXVec3TransformNormal(&transform.position, &transform.position, &mat_Ros);
	D3DXVec3TransformCoord(&transform.position, &transform.position, &mat_Trans2);
	UpdateTranslateMatrix();

	//�������任�������
	MultiplyMatrix();
	UpdateAxisBoundBoxsVectors();
	//�õ��µı任��Ӧ��ת��
	GetRotationVecByRT(&transform.rotation_matrix, &transform.rotation);
}

//##ModelId=47EBC7470020
VOID CMoveAxis::SetRotation(D3DXVECTOR3 *pVec)
{
	transform.rotation = *pVec;
	//update matrix
	UpdateRotateMatrix();
	MultiplyMatrix();
	UpdateAxisBoundBoxsVectors();
}

//##ModelId=47EBC747003E
D3DXVECTOR3* CMoveAxis::GetxAxisRayDir()
{
  D3DXVECTOR3 rayFrom;
  D3DXVECTOR3 rayTo;
  D3DXVec3TransformCoord(&rayFrom, &(m_pLineVertices[0].position), &(transform.matrix));	
  D3DXVec3TransformCoord(&rayTo, &(m_pLineVertices[1].position), &(transform.matrix));
  xAxisRay=rayTo-rayFrom;
  return &xAxisRay;
}
//##ModelId=47EBC747003F
D3DXVECTOR3* CMoveAxis::GetyAxisRayDir()
{
  D3DXVECTOR3 rayFrom;
  D3DXVECTOR3 rayTo;
  D3DXVec3TransformCoord(&rayFrom, &(m_pLineVertices[2].position), &(transform.matrix));	
  D3DXVec3TransformCoord(&rayTo, &(m_pLineVertices[3].position), &(transform.matrix));
  yAxisRay=rayTo-rayFrom;
  return &yAxisRay;
}
//##ModelId=47EBC7470040
D3DXVECTOR3* CMoveAxis::GetzAxisRayDir()
{
  D3DXVECTOR3 rayFrom;
  D3DXVECTOR3 rayTo;
  D3DXVec3TransformCoord(&rayFrom, &(m_pLineVertices[4].position), &(transform.matrix));	
  D3DXVec3TransformCoord(&rayTo, &(m_pLineVertices[5].position), &(transform.matrix));
  zAxisRay=rayTo-rayFrom;
  return &zAxisRay;
}
//-----------------------------------------------------------------------------
// ������: UpdateTranslateMatrix()
// ��  ��: 
// ��  ;: ����ģ�͵�ƽ�ƾ���
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470011
VOID CMoveAxis::UpdateTranslateMatrix()
{
	//cal translate matrix
	transform.translate_matrix._41 = transform.position.x;
	transform.translate_matrix._42 = transform.position.y;
	transform.translate_matrix._43 = transform.position.z;
}

//-----------------------------------------------------------------------------
// ������: UpdateRotateMatrix()
// ��  ��: 
// ��  ;: ����ģ�͵���ת����
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470012
VOID CMoveAxis::UpdateRotateMatrix()
{
	//cal rotate matrix
	GetRTByRotationVec( &(transform.rotation), &(transform.rotation_matrix));
}
//-----------------------------------------------------------------------------
// ������: UpdateAxisBoundBoxsVectors()
// ��  ��: 
// ��  ;: ������ģ�͵İ�Χ�����ֵ�������Сֵ�������
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470030
VOID CMoveAxis::UpdateAxisBoundBoxsVectors()
{
    D3DXVec3TransformCoord(&xAxisMinCorner, &(ori_xAxisMinCorner), &(transform.matrix));
	D3DXVec3TransformCoord(&xAxisMaxCorner, &(ori_xAxisMaxCorner), &(transform.matrix));
	D3DXVec3TransformCoord(&yAxisMinCorner, &(ori_yAxisMinCorner), &(transform.matrix));
	D3DXVec3TransformCoord(&yAxisMaxCorner, &(ori_yAxisMaxCorner), &(transform.matrix));
	D3DXVec3TransformCoord(&zAxisMinCorner, &(ori_zAxisMinCorner), &(transform.matrix));
	D3DXVec3TransformCoord(&zAxisMaxCorner, &(ori_zAxisMaxCorner), &(transform.matrix));
}
//-----------------------------------------------------------------------------
// ������: GetAxisPos()
// ��  ��: 
// ��  ;: �����X����������
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470044
D3DXVECTOR3* CMoveAxis::GetAxisPos()
{
    D3DXVec3TransformCoord(&axisPos, &(ori_AxisPos), &(transform.matrix));
	return &axisPos;
}
// ������: GetAxisXTextPos()
// ��  ��: 
// ��  ;: �����X����������
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470041
D3DXVECTOR3* CMoveAxis::GetAxisXTextPos()
{
    D3DXVec3TransformCoord(&axisTextXpos, &(ori_AxisTextXpos), &(transform.matrix));
	return &axisTextXpos;
}
//-----------------------------------------------------------------------------
// ������: GetAxisXTextPos()
// ��  ��: 
// ��  ;: �����X����������
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470042
D3DXVECTOR3* CMoveAxis::GetAxisYTextPos()
{
    D3DXVec3TransformCoord(&axisTextYpos, &(ori_AxisTextYpos), &(transform.matrix));
	return &axisTextYpos;
}
//-----------------------------------------------------------------------------
// ������: GetAxisXTextPos()
// ��  ��: 
// ��  ;: �����X����������
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470043
D3DXVECTOR3* CMoveAxis::GetAxisZTextPos()
{
    D3DXVec3TransformCoord(&axisTextZpos, &(ori_AxisTextZpos), &(transform.matrix));
	return &axisTextZpos;
}
//##ModelId=47EBC7470045
VOID CMoveAxis::UpdateAxisXTextPos(long pLeft,long pTop)
{
   AxisXTextScreenPos.left=pLeft;
   AxisXTextScreenPos.top=pTop;
}
//##ModelId=47EBC747004F
VOID CMoveAxis::UpdateAxisYTextPos(long pLeft,long pTop)
{
   AxisYTextScreenPos.left=pLeft;
   AxisYTextScreenPos.top=pTop;
}
//##ModelId=47EBC747005D
VOID CMoveAxis::UpdateAxisZTextPos(long pLeft,long pTop)
{
   AxisZTextScreenPos.left=pLeft;
   AxisZTextScreenPos.top=pTop;
}
//-----------------------------------------------------------------------------
// ������: GetTransformMatrix()
// ��  ��: 
// ��  ;: �õ��任����
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470061
D3DXMATRIX* CMoveAxis::GetTransformMatrix()
{
	return &transform.matrix;
}	
//-----------------------------------------------------------------------------
// ������: GetScale()
// ��  ��: 
// ��  ;: ��ô�С
//-----------------------------------------------------------------------------
//##ModelId=47EBC747006D
FLOAT CMoveAxis::GetScale()
{ 
    return m_Scale;
}
//-----------------------------------------------------------------------------
// ������: SetScale()
// ��  ��: 
// ��  ;: ���ô�С
//-----------------------------------------------------------------------------
//##ModelId=47EBC747006E
VOID CMoveAxis::SetScale(FLOAT scaleValue)
{
	m_Scale=scaleValue;
	D3DXMatrixScaling(&transform.scale_matrix, scaleValue, scaleValue, scaleValue);
	MultiplyMatrix();
	UpdateAxisBoundBoxsVectors();
}

//-----------------------------------------------------------------------------
// ������: MultiplyMatrix()
// ��  ��: 
// ��  ;: 
//-----------------------------------------------------------------------------
//##ModelId=47EBC7470060
VOID CMoveAxis::MultiplyMatrix()
{
	//cal all matrix
	D3DXMatrixMultiply(&(transform.matrix), 
					   &(transform.scale_matrix), 
					   &(transform.rotation_matrix));
	D3DXMatrixMultiply(&(transform.matrix), 
					   &(transform.matrix), 
					   &(transform.translate_matrix));
}
//�ͷ���Դ
//##ModelId=47EBC747039A
VOID CMoveAxis::Release()
{
	SAFE_RELEASE_D3D(m_LineVBuffer);
	SAFE_RELEASE_D3D(m_ArrowVBuffer);
	SAFE_RELEASE_D3D(m_ArrowIBuffer);
	SAFE_RELEASE_D3D(m_PlaneLineVBuffer);
	SAFE_RELEASE_D3D(m_PlaneVBuffer);
	SAFE_RELEASE_D3D(m_PlaneIBuffer);
	if(pFont)
	 pFont->Release();
}
