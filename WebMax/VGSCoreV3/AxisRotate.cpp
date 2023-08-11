#include "AxisRotate.h"

AxisRotate::AxisRotate(LPDIRECT3DDEVICE9 pd3dDevice)
	:Axis(pd3dDevice)
	,m_CircleLineVBuffer(NULL)
	,m_pCircleLineVertices(NULL)
	,m_RotateFansVBuffer(NULL)
	,m_pRotateFansVertices(NULL)
	,m_Sphere(NULL)
{
	xCircleLineColor = D3DCOLOR_RGBA(255, 0, 0, 64);	
	yCircleLineColor = D3DCOLOR_RGBA(0, 255, 0, 64);
	zCircleLineColor = D3DCOLOR_RGBA(0, 0, 255, 64);

	selectCircleAxisColor =D3DCOLOR_RGBA(255,255,0,255);

	selectSphereColor= D3DCOLOR_RGBA(255,255,255,32);
	notSelectSphereColor= D3DCOLOR_RGBA(255,255,255,5);

	//设置轴线圆的半径
	circleRadius = 30.0f;
	sphereRadius=circleRadius;
	ori_sphereRadius=sphereRadius;
	rotateAngle=0;
	startAngle=0;
	rotateRefTrans=m_transform.matrix;
	//rotateTrans=m_transform.rotation_matrix;

	//设置轴线圆的边数	
	m_CircleLineCnt = 72;
	m_CircleLineVertCnt = m_CircleLineCnt*3+3;

	//设置转角
	startRotateAngle= 0.0f;
	//转动过程中的角度
	fAngle =0.0f;

	Create();
}
AxisRotate::~AxisRotate()
{
	Release();
}

void AxisRotate::Release()
{
	SAFE_RELEASE_D3D(m_CircleLineVBuffer);
	SAFE_RELEASE_D3D(m_LineVBuffer);
	if(m_RotateFansVBuffer)
		SAFE_RELEASE_D3D(m_RotateFansVBuffer);
	SAFE_RELEASE(m_Sphere);
	SAFE_DELETE(m_Sphere);
}
VOID AxisRotate::Create()
{
	Axis::Create();

	if (FAILED( CreateCircleLineVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("创建Arrow-XYZ顶点缓冲错误"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);

	m_Sphere=new CSphere(m_pD3dDevice,20,20,circleRadius);
}
VOID AxisRotate::Restore()
{ 
	Axis::Restore();

	if (FAILED( CreateCircleLineVertexBuf()) )
		MessageBox(GetActiveWindow(), TEXT("创建Arrow-XYZ顶点缓冲错误"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);

	m_Sphere->Restore();
}
VOID AxisRotate::InitCircleLineVertex()
{
	//箭头默认是6棱锥，一个箭头7个点，三个箭头共21个点
	//x-箭头端点

	//x-箭头底面顶点
	UINT index = 0;
	//底面每个顶点角度步长
	FLOAT stepCorner = PI2/m_CircleLineCnt;
	for (UINT i = index; i< index + m_CircleLineCnt; i++)
	{
		m_pCircleLineVertices[i].position.x = 0.0f;
		m_pCircleLineVertices[i].position.y = (FLOAT)sin((i-index)*stepCorner)*circleRadius;
		m_pCircleLineVertices[i].position.z = -(FLOAT)cos((i-index)*stepCorner)*circleRadius;
		m_pCircleLineVertices[i].diffuse = xCircleLineColor;
	}
	m_pCircleLineVertices[i].position.x = m_pCircleLineVertices[index].position.x;
	m_pCircleLineVertices[i].position.y = m_pCircleLineVertices[index].position.y;
	m_pCircleLineVertices[i].position.z = m_pCircleLineVertices[index].position.z;
	m_pCircleLineVertices[i].diffuse = xCircleLineColor;
	i++;
	index = i;
	//y-箭头底面顶点
	//底面每个顶点角度步长
	for (i = index; i< index + m_CircleLineCnt; i++)
	{
		m_pCircleLineVertices[i].position.x = (FLOAT)cos((i-index)*stepCorner)*circleRadius;
		m_pCircleLineVertices[i].position.y = 0.0f;
		m_pCircleLineVertices[i].position.z = -(FLOAT)sin((i-index)*stepCorner)*circleRadius;
		m_pCircleLineVertices[i].diffuse = yCircleLineColor;
	}
	m_pCircleLineVertices[i].position.x = m_pCircleLineVertices[index].position.x;
	m_pCircleLineVertices[i].position.y = m_pCircleLineVertices[index].position.y;
	m_pCircleLineVertices[i].position.z = m_pCircleLineVertices[index].position.z;
	m_pCircleLineVertices[i].diffuse = yCircleLineColor;
	i++;
	index = i;
	//z-箭头底面顶点
	//底面每个顶点角度步长
	for (i = index; i< index + m_CircleLineCnt; i++)
	{
		m_pCircleLineVertices[i].position.x = (FLOAT)cos((i-index)*stepCorner)*circleRadius;
		m_pCircleLineVertices[i].position.y = (FLOAT)sin((i-index)*stepCorner)*circleRadius;
		m_pCircleLineVertices[i].position.z = 0.0f;
		m_pCircleLineVertices[i].diffuse = zCircleLineColor;
	}
	m_pCircleLineVertices[i].position.x = m_pCircleLineVertices[index].position.x;
	m_pCircleLineVertices[i].position.y = m_pCircleLineVertices[index].position.y;
	m_pCircleLineVertices[i].position.z = m_pCircleLineVertices[index].position.z;
	m_pCircleLineVertices[i].diffuse = zCircleLineColor;
}
HRESULT AxisRotate::CreateCircleLineVertexBuf()
{
	//crate vertex buffer obj
	if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_CircleLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),
		0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_CircleLineVBuffer, NULL) ) )
	{
		return E_FAIL;
	}

	//锁定内存
	if( FAILED(m_CircleLineVBuffer->Lock(0, m_CircleLineVertCnt * sizeof(VERTEXDECL_POSCOLOR), (VOID**)&m_pCircleLineVertices, 0) ))
	{
		return E_FAIL;
	}

	//初始化箭头
	InitCircleLineVertex();

	//Unlock顶点缓冲
	m_CircleLineVBuffer->Unlock();

	return S_OK;
}
void AxisRotate::DrawCircleLine()
{
	//打开Alpha混合
	m_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//渲染模型
	m_pD3dDevice->SetStreamSource( 0, m_CircleLineVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
	//设置FVF
	m_pD3dDevice->SetFVF(FVF_POSCOLOR); //DX9为SetFVF,DX8用这个函数
	//画轴线
	m_pD3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, m_CircleLineCnt);
	m_pD3dDevice->DrawPrimitive(D3DPT_LINESTRIP, m_CircleLineCnt+1,m_CircleLineCnt);
	m_pD3dDevice->DrawPrimitive(D3DPT_LINESTRIP, 2*(m_CircleLineCnt+1), m_CircleLineCnt);
	//关闭Alpha混合
	m_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}
void AxisRotate::SetRotateStart()
{
	rotateRefTrans=m_transform.matrix;
	//	rotateTrans=m_transform.rotation_matrix;
}
HRESULT AxisRotate::DrawRotateFans(INT startAngle,INT rotateAngle,UINT rotateAxis)
{
	if(m_RotateFansVBuffer)
	{
		m_pRotateFansVertices=NULL;	
		m_RotateFansVBuffer->Release();
		m_RotateFansVBuffer=NULL;
	}
	D3DCOLOR rotateFansColor=D3DCOLOR_RGBA(255,255,0,64);
	D3DCOLOR circleFansColor=D3DCOLOR_RGBA(255,255,0,128);

	if(abs(rotateAngle)>=360)
	{
		INT circleCnt=(abs(rotateAngle)-1)/360;
		if (circleCnt%2==0)
		{
			rotateFansColor=D3DCOLOR_RGBA(255,255,0,64);
			circleFansColor=D3DCOLOR_RGBA(255,255,0,128);
		}
		else
		{
			rotateFansColor=D3DCOLOR_RGBA(255,255,0,128);
			circleFansColor=D3DCOLOR_RGBA(255,255,0,64);
		}
		INT a=1;
		if (rotateAngle>0)
		{
			a=1;
		}
		else
		{
			a=-1;
		}
		rotateAngle=abs(rotateAngle%360);
		if (rotateAngle==0)
		{
			rotateAngle=360;
		}


		INT rotateFans5Cnt = rotateAngle/5;
		INT rotateFans1Cnt = rotateAngle%5;
		INT circleFans5Cnt = (360-rotateAngle)/5;
		INT circleFansleft = (360-rotateAngle)%5;
		INT circleFansleftCnt =0;
		if (circleFansleft>0)
			circleFansleftCnt=1;

		UINT m_FansVertCnt= 0;
		if (circleFans5Cnt!=0||circleFansleftCnt!=0)
			m_FansVertCnt=rotateFans5Cnt+rotateFans1Cnt+2+circleFans5Cnt+circleFansleftCnt+2;
		else
			m_FansVertCnt=rotateFans5Cnt+rotateFans1Cnt+2;

		//crate vertex buffer obj
		if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_FansVertCnt * sizeof(VERTEXDECL_POSCOLOR),
			0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_RotateFansVBuffer, NULL) ) )
		{
			MessageBox(NULL, TEXT("creatVertexBuffer"),TEXT(""),MB_OK);
			return E_FAIL;
		}
		//锁定内存
		if( FAILED(m_RotateFansVBuffer->Lock(0, m_FansVertCnt * sizeof(VERTEXDECL_POSCOLOR), (VOID**)&m_pRotateFansVertices, 0) ))
		{
			MessageBox(NULL,TEXT("lockBuffer"),TEXT(""),MB_OK);
			return E_FAIL;
		}


		INT index=1;
		INT i=0;
		//初始化箭头
		m_pRotateFansVertices[0].position.x = 0.0f;
		m_pRotateFansVertices[0].position.y = 0.0f;
		m_pRotateFansVertices[0].position.z = 0.0f;
		m_pRotateFansVertices[0].diffuse = rotateFansColor;
		FLOAT stepCorner = PI2/m_CircleLineCnt;
		FLOAT startAng=(FLOAT)(startAngle/180.0f)*PI;
		if(rotateFans5Cnt>0)
		{
			for (i = index; i<= index + rotateFans5Cnt; i++)
			{
				switch(rotateAxis)
				{
				case 1:
					m_pRotateFansVertices[i].position.x = 0.0f;
					m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;

					break;					
				case 2:
					m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.y = 0.0f;
					m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					break;
				case 3:
					m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.z = 0.0f;
					break;								
				}
				m_pRotateFansVertices[i].diffuse = rotateFansColor;
			}
			startAng=startAng+a*(i-index-1)*stepCorner;
			index=i-1;
			stepCorner=PI2/360;
			if(rotateFans1Cnt>0){
				for (i = index+1; i< index+1 + rotateFans1Cnt; i++)
				{
					switch(rotateAxis)
					{
					case 1:
						m_pRotateFansVertices[i].position.x = 0.0f;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						break;					
					case 2:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = 0.0f;
						m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						break;
					case 3:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = 0.0f;
						break;								
					}
					m_pRotateFansVertices[i].diffuse = rotateFansColor;
				}
			}
		}
		else
		{
			stepCorner=PI2/360;
			for (i = index; i<= index + rotateFans1Cnt; i++)
			{
				switch(rotateAxis)
				{
				case 1:
					m_pRotateFansVertices[i].position.x = 0.0f;
					m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					break;					
				case 2:
					m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.y = 0.0f;
					m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					break;
				case 3:
					m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.z = 0.0f;
					break;								
				}
				m_pRotateFansVertices[i].diffuse = rotateFansColor;
			}			
		}
		startAng=startAng+a*(i-index-1)*stepCorner;
		index=i;

		if(circleFans5Cnt!=0 || circleFansleftCnt!=0){
			//初始化箭头
			m_pRotateFansVertices[index].position.x = 0.0f;
			m_pRotateFansVertices[index].position.y = 0.0f;
			m_pRotateFansVertices[index].position.z = 0.0f;
			m_pRotateFansVertices[index].diffuse = circleFansColor;
			index=index+1;
			if(circleFans5Cnt!=0){	
				stepCorner=PI2/m_CircleLineCnt;
				for (i = index; i<= index + circleFans5Cnt; i++)
				{
					switch(rotateAxis)
					{
					case 1:
						m_pRotateFansVertices[i].position.x = 0.0f;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						break;					
					case 2:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = 0.0f;
						m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						break;
					case 3:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = 0.0f;
						break;								
					}
					m_pRotateFansVertices[i].diffuse = circleFansColor;
				}
				startAng=startAng+a*(i-index-1)*stepCorner;
				index=i;
				stepCorner=PI2/360;
				if(circleFansleftCnt!=0)
				{
					switch(rotateAxis)
					{
					case 1:
						m_pRotateFansVertices[i].position.x = 0.0f;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*circleFansleft*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*circleFansleft*stepCorner)*circleRadius;
						break;					
					case 2:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*circleFansleft*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = 0.0f;
						m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*circleFansleft*stepCorner)*circleRadius;
						break;
					case 3:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*circleFansleft*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*circleFansleft*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = 0.0f;
						break;								
					}	
					m_pRotateFansVertices[i].diffuse = circleFansColor;
				}		
			}
			else
			{
				stepCorner=PI2/360;
				for(i=index;i<=index+circleFansleftCnt;i++)
				{
					switch(rotateAxis)
					{
					case 1:
						m_pRotateFansVertices[i].position.x = 0.0f;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						break;					
					case 2:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = 0.0f;
						m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						break;
					case 3:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = 0.0f;
						break;								
					}
					m_pRotateFansVertices[i].diffuse = circleFansColor;
				}
			}
		}
		//Unlock顶点缓冲
		m_RotateFansVBuffer->Unlock();	

		m_pD3dDevice->SetTransform( D3DTS_WORLD, &rotateRefTrans);
		m_pD3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE); 
		//打开Alpha混合
		m_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//渲染模型
		m_pD3dDevice->SetStreamSource( 0, m_RotateFansVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
		//设置FVF
		m_pD3dDevice->SetFVF(FVF_POSCOLOR); //DX9为SetFVF,DX8用这个函数
		//画轴线
		m_pD3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN , 0, rotateFans5Cnt+rotateFans1Cnt);
		if(circleFans5Cnt!=0 || circleFansleftCnt!=0){
			m_pD3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN , rotateFans5Cnt+rotateFans1Cnt+2, circleFans5Cnt+circleFansleftCnt);	
		}

		//关闭Alpha混合
		m_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pD3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW); 

	}else{
		INT a=1;
		if (rotateAngle>0){
			a=1;
		}else{
			a=-1;
		}
		rotateAngle=abs(rotateAngle);
		INT rotateFans5Cnt = rotateAngle/5;
		INT rotateFans1Cnt = rotateAngle%5;		

		UINT m_FansVertCnt= 0;

		m_FansVertCnt=rotateFans5Cnt+rotateFans1Cnt+2;
		//crate vertex buffer obj
		if( FAILED( m_pD3dDevice->CreateVertexBuffer(m_FansVertCnt * sizeof(VERTEXDECL_POSCOLOR),
			0, FVF_POSCOLOR, D3DPOOL_MANAGED, &m_RotateFansVBuffer, NULL) ) )
		{
			MessageBox(NULL, TEXT("creatVertexBuffer"),TEXT(""),MB_OK);
			return E_FAIL;
		}
		//锁定内存
		if( FAILED(m_RotateFansVBuffer->Lock(0, m_FansVertCnt * sizeof(VERTEXDECL_POSCOLOR), (VOID**)&m_pRotateFansVertices, 0) ))
		{
			MessageBox(NULL,TEXT("lockBuffer"),TEXT(""),MB_OK);
			return E_FAIL;
		}
		INT index=1;
		INT i=0;
		//初始化箭头
		m_pRotateFansVertices[0].position.x = 0.0f;
		m_pRotateFansVertices[0].position.y = 0.0f;
		m_pRotateFansVertices[0].position.z = 0.0f;
		m_pRotateFansVertices[0].diffuse = rotateFansColor;
		FLOAT stepCorner = PI2/m_CircleLineCnt;
		FLOAT startAng=(FLOAT)(startAngle/180.0f)*PI;
		if(rotateFans5Cnt>0){
			for (i = index; i<= index + rotateFans5Cnt; i++)
			{
				switch(rotateAxis)
				{
				case 1:
					m_pRotateFansVertices[i].position.x = 0.0f;
					m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					break;					
				case 2:
					m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.y = 0.0f;
					m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					break;
				case 3:
					m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.z = 0.0f;
					break;								
				}
				m_pRotateFansVertices[i].diffuse = rotateFansColor;
			}
			startAng=startAng+a*(i-index-1)*stepCorner;
			index=i-1;
			stepCorner=PI2/360;
			if(rotateFans1Cnt>0){
				for (i = index+1; i< index+1 + rotateFans1Cnt; i++)
				{
					switch(rotateAxis)
					{
					case 1:
						m_pRotateFansVertices[i].position.x = 0.0f;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						break;					
					case 2:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = 0.0f;
						m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						break;
					case 3:
						m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
						m_pRotateFansVertices[i].position.z = 0.0f;
						break;								
					}
					m_pRotateFansVertices[i].diffuse = rotateFansColor;
				}
			}
		}else{
			stepCorner=PI2/360;
			for (i = index; i<= index + rotateFans1Cnt; i++)
			{
				switch(rotateAxis)
				{
				case 1:
					m_pRotateFansVertices[i].position.x = 0.0f;
					m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.z = -(FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					break;					
				case 2:
					m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.y = 0.0f;
					m_pRotateFansVertices[i].position.z = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					break;
				case 3:
					m_pRotateFansVertices[i].position.x = (FLOAT)cos(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.y = (FLOAT)sin(startAng+a*(i-index)*stepCorner)*circleRadius;
					m_pRotateFansVertices[i].position.z = 0.0f;
					break;								
				}
				m_pRotateFansVertices[i].diffuse = rotateFansColor;
			}			
		}

		//Unlock顶点缓冲
		m_RotateFansVBuffer->Unlock();	
		m_pD3dDevice->SetTransform( D3DTS_WORLD, &rotateRefTrans);
		m_pD3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE); 
		//打开Alpha混合
		m_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//渲染模型
		m_pD3dDevice->SetStreamSource( 0, m_RotateFansVBuffer, 0, sizeof(VERTEXDECL_POSCOLOR) );
		//设置FVF
		m_pD3dDevice->SetFVF(FVF_POSCOLOR); //DX9为SetFVF,DX8用这个函数
		//画轴线
		m_pD3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0, rotateFans5Cnt+rotateFans1Cnt);
		//关闭Alpha混合
		m_pD3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		m_pD3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW); 
		//清空vertices指针



	}

	return S_OK;
}
void AxisRotate::Draw(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection)
{
	ClearPreDeviceSettings();
	//处理Axis的移动和旋转
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &m_transform.matrix);
	m_Sphere->Render();
	DrawLine();
	DrawCircleLine();
	UpdateAxisTextsPos(pViewport, pView, pProjection);
	DrawAxisText();

	if(rotateAngle!=0){
		switch(m_nSelectedAxisStatus)
		{
		case 1:
			if (FAILED(DrawRotateFans(startAngle,rotateAngle,1)) )
				MessageBox(GetActiveWindow(), TEXT("DrawRotateFans Error"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);
			break;
		case 2:
			if (FAILED(DrawRotateFans(startAngle,rotateAngle,2)) )
				MessageBox(GetActiveWindow(), TEXT("DrawRotateFans Error"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);
			break;
		case 3:
			if (FAILED(DrawRotateFans(startAngle,rotateAngle,3)) )
				MessageBox(GetActiveWindow(), TEXT("DrawRotateFans Error"), TEXT("VGS2.0错误信息提示"), MB_OK | MB_ICONERROR);
			break;

		}	
	}
}
void AxisRotate::SelectAxis(UINT selectStatus)
{
	//处理Axis的移动和旋转
	//锁定内存
	if( FAILED(m_LineVBuffer->Lock(0, m_nLineVertCnt * sizeof(VERTEXDECL_POSCOLOR),	(VOID**)&m_pLineVertices, 0) ))
	{
		return;
	}
	if( FAILED(m_CircleLineVBuffer->Lock(0, m_CircleLineVertCnt * sizeof(VERTEXDECL_POSCOLOR), (VOID**)&m_pCircleLineVertices, 0) ))
	{
		return;
	}
	UINT i;
	UINT index=0;
	switch(selectStatus){
	case 0:
		//设置xline颜色
		m_pLineVertices[0].diffuse = m_colorLineX;
		m_pLineVertices[1].diffuse = m_colorLineX;
		//设置yline颜色
		m_pLineVertices[2].diffuse = m_colorLineY;
		m_pLineVertices[3].diffuse = m_colorLineY;
		//设置zline颜色
		m_pLineVertices[4].diffuse = m_colorLineZ;
		m_pLineVertices[5].diffuse = m_colorLineZ;
		//设置x，y，z轴线圆的颜色
		for (i = index; i< index + m_CircleLineCnt; i++)
		{	
			m_pCircleLineVertices[i].diffuse = xCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = xCircleLineColor;
		i++;
		index = i;
		//y-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = yCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = yCircleLineColor;
		i++;
		index = i;
		//z-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = zCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = zCircleLineColor;

		m_Sphere->SetVertexColor(notSelectSphereColor);


		m_colorTextX=m_colorLineX;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorLineZ;
		break;

	case 1:
		//设置xline颜色
		m_pLineVertices[0].diffuse = selectCircleAxisColor;
		m_pLineVertices[1].diffuse = selectCircleAxisColor;
		//设置yline颜色
		m_pLineVertices[2].diffuse = m_colorLineY;
		m_pLineVertices[3].diffuse = m_colorLineY;
		//设置zline颜色
		m_pLineVertices[4].diffuse = m_colorLineZ;
		m_pLineVertices[5].diffuse = m_colorLineZ;

		//设置x，y，z轴线圆的颜色
		for (i = index; i< index + m_CircleLineCnt; i++)
		{	
			m_pCircleLineVertices[i].diffuse = selectCircleAxisColor;
		}
		m_pCircleLineVertices[i].diffuse = selectCircleAxisColor;
		i++;
		index = i;
		//y-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = yCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = yCircleLineColor;
		i++;
		index = i;
		//z-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = zCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = zCircleLineColor;

		m_Sphere->SetVertexColor(notSelectSphereColor);

		m_colorTextX=selectCircleAxisColor;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorLineZ;
		break;

	case 2:
		//设置xline颜色
		m_pLineVertices[0].diffuse = m_colorLineX;
		m_pLineVertices[1].diffuse = m_colorLineX;
		//设置yline颜色
		m_pLineVertices[2].diffuse = selectCircleAxisColor;
		m_pLineVertices[3].diffuse = selectCircleAxisColor;
		//设置zline颜色
		m_pLineVertices[4].diffuse = m_colorLineZ;
		m_pLineVertices[5].diffuse = m_colorLineZ;

		//设置x，y，z轴线圆的颜色
		for (i = index; i< index + m_CircleLineCnt; i++)
		{	
			m_pCircleLineVertices[i].diffuse = xCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = xCircleLineColor;
		i++;
		index = i;
		//y-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = selectCircleAxisColor;
		}
		m_pCircleLineVertices[i].diffuse = selectCircleAxisColor;
		i++;
		index = i;
		//z-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = zCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = zCircleLineColor;

		m_Sphere->SetVertexColor(notSelectSphereColor);

		m_colorTextX=m_colorLineX;
		m_colorTextY=selectCircleAxisColor;
		m_colorTextZ=m_colorLineZ;

		break;

	case 3:
		//设置xline颜色
		m_pLineVertices[0].diffuse = m_colorLineX;
		m_pLineVertices[1].diffuse = m_colorLineX;
		//设置yline颜色
		m_pLineVertices[2].diffuse = m_colorLineY;
		m_pLineVertices[3].diffuse = m_colorLineY;
		//设置zline颜色
		m_pLineVertices[4].diffuse = selectCircleAxisColor;
		m_pLineVertices[5].diffuse = selectCircleAxisColor;

		//设置x，y，z轴线圆的颜色
		for (i = index; i< index + m_CircleLineCnt; i++)
		{	
			m_pCircleLineVertices[i].diffuse = xCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = xCircleLineColor;
		i++;
		index = i;
		//y-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = yCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = yCircleLineColor;
		i++;
		index = i;
		//z-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = selectCircleAxisColor;
		}
		m_pCircleLineVertices[i].diffuse = selectCircleAxisColor;

		m_Sphere->SetVertexColor(notSelectSphereColor);

		m_colorTextX=m_colorLineX;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=selectCircleAxisColor;

		break;
	case 4:
		//设置xline颜色
		m_pLineVertices[0].diffuse = m_colorLineX;
		m_pLineVertices[1].diffuse = m_colorLineX;
		//设置yline颜色
		m_pLineVertices[2].diffuse = m_colorLineY;
		m_pLineVertices[3].diffuse = m_colorLineY;
		//设置zline颜色
		m_pLineVertices[4].diffuse = m_colorLineZ;
		m_pLineVertices[5].diffuse = m_colorLineZ;
		//设置x，y，z轴线圆的颜色
		for (i = index; i< index + m_CircleLineCnt; i++)
		{	
			m_pCircleLineVertices[i].diffuse = xCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = xCircleLineColor;
		i++;
		index = i;
		//y-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = yCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = yCircleLineColor;
		i++;
		index = i;
		//z-箭头底面顶点
		for (i = index; i< index + m_CircleLineCnt; i++)
		{
			m_pCircleLineVertices[i].diffuse = zCircleLineColor;
		}
		m_pCircleLineVertices[i].diffuse = zCircleLineColor;

		m_Sphere->SetVertexColor(selectSphereColor);


		m_colorTextX=m_colorLineX;
		m_colorTextY=m_colorLineY;
		m_colorTextZ=m_colorLineZ;
		break;
	}
	//Unlock顶点缓冲
	m_LineVBuffer->Unlock();
	m_CircleLineVBuffer->Unlock();
	//设置Axis状态
	m_nSelectedAxisStatus=selectStatus;
}
