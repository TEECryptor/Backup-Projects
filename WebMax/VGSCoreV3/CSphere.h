#ifndef _CSPHERE_H
#define _CSPHERE_H  


#include "CManyFVF.h"
#include "CSysFunc.h"
#include "CTransform.h"
#define SPHERE_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE)

//##ModelId=47EBC72F002E
class VGS_EXPORT   CSphere       
  {   
  private:   
  //Define   a   custom   vertex   for   our   sphere   
	//##ModelId=47EBC72F008C
  struct   SPHERE_CUSTOMVERTEX   
  {   
  FLOAT   x,   y,   z; //Position   of   vertex   in   3D   space   
  FLOAT   nx,   ny,   nz; //Lighting   Normal   
		//##ModelId=47EBC72F008F
  D3DCOLOR diffuse;//Vertex Color;
  };   
    
  public:   
	//##ModelId=47EBC72F002F
  bool   SetMaterial(D3DCOLORVALUE   rgbaDiffuse,   D3DCOLORVALUE   rgbaAmbient,   D3DCOLORVALUE   rgbaSpecular,   D3DCOLORVALUE   rgbaEmissive,   float   rPower);   
	//##ModelId=47EBC72F0035
  bool   SetTexture(const   char*   szTextureFilePath);  
	//##ModelId=47EBC72F0037
  bool   SetVertexColor(D3DCOLOR rgbaDiffuse);
	//##ModelId=47EBC72F0039
  DWORD   Render(); 
	//##ModelId=47EBC72F003A
  VOID    Release();
	//##ModelId=47EBC72F003B
  VOID    Restore();
	//##ModelId=47EBC72F003E
  CSphere(LPDIRECT3DDEVICE9   pD3DDevice,   int   nRings   =   20,   int   nSegments   =   20, FLOAT nRadius = 1.0f);   
	//##ModelId=47EBC72F0043
  ~CSphere();   
    
  private:   
	//##ModelId=47EBC72F0044
  bool   CreateIndexBuffer();   
	//##ModelId=47EBC72F0045
  bool   UpdateVertices();   
	//##ModelId=47EBC72F0046
  bool   CreateVertexBuffer();   
	//##ModelId=47EBC72F0048
  LPDIRECT3DDEVICE9   m_pD3DDevice;   
	//##ModelId=47EBC72F004D
  LPDIRECT3DVERTEXBUFFER9   m_pVertexBuffer;   
	//##ModelId=47EBC72F0052
  LPDIRECT3DTEXTURE9   m_pTexture;   
	//##ModelId=47EBC72F0057
  D3DMATERIAL9   m_matMaterial;   
	//##ModelId=47EBC72F005C
  LPDIRECT3DINDEXBUFFER9   m_pIndexBuffer;  
	//##ModelId=47EBC72F0061
  SPHERE_CUSTOMVERTEX*   pVertex;  
    
	//##ModelId=47EBC72F0065
  int   m_nRings;   
	//##ModelId=47EBC72F0066
  int   m_nSegments; 
	//##ModelId=47EBC72F0067
  FLOAT m_nRadius;
    
	//##ModelId=47EBC72F0068
  DWORD   m_dwNumOfVertices;   
	//##ModelId=47EBC72F0069
  DWORD   m_dwNumOfIndices;   
	//##ModelId=47EBC72F006A
  DWORD   m_dwNumOfPolygons;   
  };

#endif
