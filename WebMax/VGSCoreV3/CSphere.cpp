//////////////////////////////////////////////////////////////////////   

#include   "CSphere.h"   

//////////////////////////////////////////////////////////////////////   
//   Construction/Destruction   
//////////////////////////////////////////////////////////////////////   

//##ModelId=47EBC72F003E
CSphere::CSphere(LPDIRECT3DDEVICE9   pD3DDevice,   int   nRings,   int   nSegments,FLOAT nRadius)   
{   
	m_pD3DDevice   =   pD3DDevice;   
	m_pVertexBuffer   =   NULL;   
	m_pIndexBuffer   =   NULL;   
	m_pTexture   =   NULL;   
	pVertex= NULL;
    
	m_nRings   =   nRings;   
	m_nSegments   =   nSegments;   
	m_nRadius   =  nRadius;
    
	//Setup   counts   for   this   object   
	m_dwNumOfVertices   =   (m_nRings   +   1)   *   (m_nSegments   +   1);   
	m_dwNumOfIndices     =   2   *   m_nRings   *   (m_nSegments   +   1);   
	m_dwNumOfPolygons   =   m_dwNumOfIndices   -   2;   
    
	//Set   material   default   values   (R,   G,   B,   A)   
	D3DCOLORVALUE   rgbaDiffuse     =   {1.0f,   0.0f,   0.0f,   1.0f,};   
	D3DCOLORVALUE   rgbaAmbient     =   {1.0,   0.0,   0.0,   1.0,};   
	D3DCOLORVALUE   rgbaSpecular   =   {0.0,   0.0,   0.0,   1.0,};   
	D3DCOLORVALUE   rgbaEmissive   =   {0.0,   0.0,   0.0,   1.0,};   
    
	SetMaterial(rgbaDiffuse,   rgbaAmbient,   rgbaSpecular,   rgbaEmissive,   0);   
    
	//Initialize   Vertex   Buffer   
	if(CreateVertexBuffer())   
	{   
		if(CreateIndexBuffer())   
		{   
			if(UpdateVertices())   
			{   
				//LogInfo("<li>Sphere   created   OK");   
				return;   
			}   
		}   
	}   
    
	//LogError("<li>Sphere   failed   to   create");   
}   

//##ModelId=47EBC72F0043
CSphere::~CSphere()   
{   
/*	if(m_pTexture)
	  SAFE_RELEASE_D3D(m_pTexture);
	if(m_pIndexBuffer)
	  SAFE_RELEASE_D3D(m_pIndexBuffer); 
	if(m_pVertexBuffer)
	  SAFE_RELEASE_D3D(m_pVertexBuffer); 
	
    pVertex= NULL;*/
    
	//LogInfo("<li>Sphere   destroyed   OK");   
}   

//##ModelId=47EBC72F003A
VOID    CSphere::Release()
{
	SAFE_RELEASE_D3D(m_pTexture);   
	SAFE_RELEASE_D3D(m_pIndexBuffer);   
	SAFE_RELEASE_D3D(m_pVertexBuffer);   
    pVertex= NULL;   
}

//##ModelId=47EBC72F003B
VOID   CSphere::Restore()
{
	//Initialize   Vertex   Buffer   
	if(CreateVertexBuffer())   
	{   
		if(CreateIndexBuffer())   
		{   
			if(UpdateVertices())   
			{   
				//LogInfo("<li>Sphere   created   OK");   
				return;   
			}   
		}   
	}   
}

//##ModelId=47EBC72F0039
DWORD   CSphere::Render()   
{   
	m_pD3DDevice->SetStreamSource(0,   m_pVertexBuffer,  0,  sizeof(SPHERE_CUSTOMVERTEX));   
	m_pD3DDevice->SetFVF(SPHERE_D3DFVF_CUSTOMVERTEX);   
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    
	if(m_pTexture   !=   NULL)   
	{   
		//A   texture   has   been   set.   We   want   our   texture   to   be   shaded   based   
		//on   the   current   light   levels,   so   used   D3DTOP_MODULATE.   
		m_pD3DDevice->SetTexture(0,   m_pTexture);   
		m_pD3DDevice->SetTextureStageState(0,   D3DTSS_COLOROP,   D3DTOP_MODULATE);   
		m_pD3DDevice->SetTextureStageState(0,   D3DTSS_COLORARG1,   D3DTA_TEXTURE);   
		m_pD3DDevice->SetTextureStageState(0,   D3DTSS_COLORARG2,   D3DTA_CURRENT);   
	}   
	else   
	{   
		//No   texture   has   been   set   
		m_pD3DDevice->SetTextureStageState(0,   D3DTSS_COLOROP,   D3DTOP_SELECTARG2);   
		m_pD3DDevice->SetTextureStageState(0,   D3DTSS_COLORARG1,   D3DTA_TEXTURE);   
		m_pD3DDevice->SetTextureStageState(0,   D3DTSS_COLORARG2,   D3DTA_CURRENT);   
	}   
    
	//Select   the   material   to   use   
	m_pD3DDevice->SetMaterial(&m_matMaterial);   
    
	//Select   index   buffer   
	m_pD3DDevice->SetIndices(m_pIndexBuffer);   
    
	//Render   polygons   from   index   buffer   
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,   0,  0,  m_dwNumOfVertices,   0,   m_dwNumOfPolygons);   
    
	m_pD3DDevice->SetTexture(0,   0);   
	m_pD3DDevice->SetStreamSource(0,   0,  0,  0);   
	m_pD3DDevice->SetIndices(0);   
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//Return   the   number   of   polygons   rendered   
	return   m_dwNumOfPolygons;   
}   

//##ModelId=47EBC72F0046
bool   CSphere::CreateVertexBuffer()   
{   
	//Create   the   vertex   buffer   from   our   device.   
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(m_dwNumOfVertices   *   sizeof(SPHERE_CUSTOMVERTEX),   
		0,   SPHERE_D3DFVF_CUSTOMVERTEX,   
		D3DPOOL_DEFAULT,   &m_pVertexBuffer, NULL)))   
	{   
//		LogError("<li>CSphere:   Unable   to   create   vertex   buffer.");   
		return   false;   
	}   
    
	return   true;   
}   

//##ModelId=47EBC72F0044
bool   CSphere::CreateIndexBuffer()   
{   
	//Create   the   index   buffer   from   our   device   
	if(FAILED(m_pD3DDevice->CreateIndexBuffer(m_dwNumOfIndices   *   sizeof(WORD),     
		0,   D3DFMT_INDEX16,   D3DPOOL_MANAGED,   
		&m_pIndexBuffer, NULL)))   
	{   
		//LogError("<li>CSphere:   Unable   to   create   index   buffer.");   
		return   false;   
	}   
    
	return   true;   
}   

//##ModelId=47EBC72F0045
bool   CSphere::UpdateVertices()   
{       
	WORD*   pIndices;   
	   
	WORD   wVertexIndex   =   0;   
	int   nCurrentRing;   
	int   nCurrentSegment;   
	D3DXVECTOR3   vNormal;   
    
	//Lock   the   vertex   buffer   
	if(FAILED(m_pVertexBuffer->Lock(0,   0,   (VOID**)&pVertex,   0)))   
	{   
		//LogError("<li>CSphere:   Unable   to   lock   vertex   buffer.");   
		return   false;   
	}   
    
	//Lock   the   index   buffer     
	if(FAILED(m_pIndexBuffer->Lock(0,   m_dwNumOfIndices,   (VOID**)&pIndices,   0)))   
	{   
		//LogError("<li>CSphere:   Unable   to   lock   index   buffer.");   
		return   false;   
	}   
    
	//Establish   constants   used   in   sphere   generation   
	FLOAT   rDeltaRingAngle   =   (D3DX_PI   /   m_nRings);   
	FLOAT   rDeltaSegAngle   =   (2.0f   *   D3DX_PI   /   m_nSegments);   
    
	//Generate   the   group   of   rings   for   the   sphere   
	for(nCurrentRing   =   0;   nCurrentRing   <   m_nRings   +   1;   nCurrentRing++)   
	{   
		FLOAT   r0   =   sinf(nCurrentRing   *   rDeltaRingAngle);   
		FLOAT   y0   =   cosf(nCurrentRing   *   rDeltaRingAngle);   
		
		//Generate   the   group   of   segments   for   the   current   ring   
		for(nCurrentSegment   =   0;   nCurrentSegment   <   m_nSegments   +   1;   nCurrentSegment++)   
		{   
			FLOAT   x0   =   r0   *   sinf(nCurrentSegment   *   rDeltaSegAngle);   
			FLOAT   z0   =   r0   *   cosf(nCurrentSegment   *   rDeltaSegAngle);   
			
			vNormal.x   =   x0;   
			vNormal.y   =   y0;   
			vNormal.z   =   z0;   
			
			D3DXVec3Normalize(&vNormal,   &vNormal);   
			
			//Add   one   vertex   to   the   strip   which   makes   up   the   sphere   
			pVertex->x   =   x0*m_nRadius;   
			pVertex->y   =   y0*m_nRadius;   
			pVertex->z   =   z0*m_nRadius;   
			pVertex->nx   =   vNormal.x;   
			pVertex->ny   =   vNormal.y;   
			pVertex->nz   =   vNormal.z;   
			pVertex->diffuse=D3DCOLOR_RGBA(255, 255, 255, 5);	
			//pVertex->tu   =   1.0f   -   ((FLOAT)nCurrentSegment   /   (FLOAT)m_nSegments);   
			//pVertex->tv   =   (FLOAT)nCurrentRing   /   (FLOAT)m_nRings;   
			
			pVertex++;   
			
			//Add   two   indices   except   for   the   last   ring     
			if(nCurrentRing   !=   m_nRings)     
			{   
				*pIndices   =   wVertexIndex;     
				pIndices++;   
				
				*pIndices   =   wVertexIndex   +   (WORD)(m_nSegments   +   1);     
				pIndices++;   
				
				wVertexIndex++;     
			}   
		}   
	}   
    
	if(FAILED(m_pIndexBuffer->Unlock()))   
	{   
	//	LogError("<li>CSphere:   Unable   to   unlock   index   buffer.");   
		return   false;   
	}   
    
	if(FAILED(m_pVertexBuffer->Unlock()))   
	{   
		//LogError("<li>CSphere:   Unable   to   unlock   vertex   buffer.");   
		return   false;   
	}   
    
	return   true;     
}   

//##ModelId=47EBC72F0035
bool   CSphere::SetTexture(const   char   *szTextureFilePath)   
{   
	if(FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, (szTextureFilePath),   &m_pTexture)))   
	{   
		return   false;   
	}   
    
	return   true;   
}   

//##ModelId=47EBC72F002F
bool   CSphere::SetMaterial(D3DCOLORVALUE   rgbaDiffuse,   D3DCOLORVALUE   rgbaAmbient,   D3DCOLORVALUE   rgbaSpecular,   D3DCOLORVALUE   rgbaEmissive,   float   rPower)   
{   
	//Set   the   RGBA   for   diffuse   light   reflected   from   this   material.     
	m_matMaterial.Diffuse   =   rgbaDiffuse;     
    
	//Set   the   RGBA   for   ambient   light   reflected   from   this   material.     
	m_matMaterial.Ambient   =   rgbaAmbient;     
    
	//Set   the   color   and   sharpness   of   specular   highlights   for   the   material.     
	m_matMaterial.Specular   =   rgbaSpecular;     
	m_matMaterial.Power   =   rPower;   
    
	//Set   the   RGBA   for   light   emitted   from   this   material.     
	m_matMaterial.Emissive   =   rgbaEmissive;   
    
	return   true;   
}

//##ModelId=47EBC72F0037
bool  CSphere::SetVertexColor(D3DCOLOR rgbaDiffuse)
{
	if(m_pVertexBuffer){
		//Lock   the   vertex   buffer   
		if(FAILED(m_pVertexBuffer->Lock(0,   0,   (VOID**)&pVertex,   0)))   
		{   
			//LogError("<li>CSphere:   Unable   to   lock   vertex   buffer.");   
			return   false;   
		} 
		if(pVertex){
			for(UINT i =0 ;i<m_dwNumOfVertices; i++){
				pVertex[i].diffuse=rgbaDiffuse;
			}
		}
		
		if(FAILED(m_pVertexBuffer->Unlock()))   
		{   
			//LogError("<li>CSphere:   Unable   to   unlock   vertex   buffer.");   
			return   false;   
		}   
	}
	return true;
	
	
}
