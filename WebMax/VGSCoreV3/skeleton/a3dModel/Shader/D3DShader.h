/*===================================================================
* zeus Game Engine
* This library is a professional game engine
* Thank you for choose Zeus!

* creater : flymemory
* created : 26-3-2005
* filename: zShader_D3D.h

* purpose : 
=======================================================================*/
#ifndef __D3D__INCLUDED_
#define __D3D__INCLUDED_
#include "../CD3DInclude.h"
using namespace std;
std::wstring ToWcs(const char* _name);
std::string  ToAnsi(const wchar_t* _name);

enum eShaderType
{
    ShaderType_Pixel  = 1,
    ShaderType_Vertex = 2,
    ShaderType_Effect = 3,
};


enum eShaderParamSemantic
{
	SPS_POSITION   = 0                    ,   //   position
	SPS_DIRECTION                         ,   //   direction
	SPS_DIFFUSE                           ,   //   diffuse
	SPS_SPECULAR                          ,   //   specular
	SPS_AMBIENT                           ,   //   ambient
	SPS_SPECULARPOWER                     ,   //   specularpower
	SPS_EMISSIVE                          ,   //   emissive
	SPS_NORMAL                            ,   //   normal
	SPS_HEIGHT                            ,   //   height
	SPS_REFRACTION                        ,   //   refraction
	SPS_OPACITY                           ,   //   opacity
	SPS_ENVIRONMENT                       ,   //   environment
	SPS_ENVIROMENTNORMAL                  ,   //   environmentnormal
	SPS_FRESNEL                           ,   //   fresnel
	SPS_WORLDMAT                          ,   //   world
	SPS_WORLDTRANS_MAT                    ,   //   worldtranspose
	SPS_WORLDINV_MAT                      ,   //   worldinverse
	SPS_WORLDINVTRANS_MAT                 ,   //   worldinversetranspose
	SPS_WORLDVIEW_MAT                     ,   //   worldview
	SPS_WORLDVIEWINV_MAT                  ,   //   worldviewinverse
	SPS_WORLDVIEWINVTRANS_MAT             ,   //   worldviewinversetranspose
	SPS_WVP_MAT                           ,   //   worldviewprojection
	SPS_WVPINV_MAT                        ,   //   worldviewprojectioninverse
	SPS_WVPINVTRANS_MAT                   ,   //   worldviewprojectioninversetranspose
	SPS_VIEW_MAT                          ,   //   view
	SPS_VIEWTRANS_MAT                     ,   //   viewtranspose
	SPS_VIEWPROJECTION_MAT                ,   //   viewprojection
	SPS_VIEWINV_MAT                       ,   //   viewinverse
	SPS_VIEWINVTRANS_MAT                  ,   //   viewinversetranspose
	SPS_VIEWPROJECTIONTRANS_MAT           ,   //   viewprojectiontranspose           
	SPS_WORLDVIEWTRANS_MAT                ,   //   worldviewtranspose
	SPS_VIEWPROJECTIONINV_MAT             ,   //   viewprojectioninverse
	SPS_VIEWPROJECTIONINVTRANS_MAT        ,   //   viewprojectioninversetranspose
	SPS_PROJECTION_MAT                    ,   //   projection
	SPS_PROJECTIONINV_MAT                 ,   //   projectioninverse
	SPS_PROJECTIONINVTRANS_MAT            ,   //   projectioninversetranspose
	SPS_TEXTURE_MAT                       ,   //   texturematrix

	SPS_VIEWPORTPIXEL_SIZE                ,   //   viewportpixelsize
	SPS_VIEWPORTCLIPPING                  ,   //   viewportclipping

	SPS_TIME                              ,   //   time
	SPS_LAST_TIME                         ,   //   lasttime
	SPS_ELAPSED_TIME                      ,   //   elapsedtime

	SPS_ATTENUATION                       ,   //   attenuation
	SPS_RENDERTARGET_COLOR                ,   //   rendercolortarget
	SPS_RENDERTARGET_DEPTH                ,   //   renderdepthstenciltarget
	SPS_RANDOM                            ,   //   random
	SPS_UNITSSCALE                        ,   //   unitsscale

	SPS_CAMERAPOSITION                    ,   //   cameraposition

	SPS_LIGHT_UMBRA                       ,   //   lightumbra 
	SPS_LIGHT_PENMUBAR                    ,   //   lightpenumbra
	SPS_LIGHT_RANGE                       ,   //   lightrange
	SPS_LIGHT_FALLOFF                     ,   //   lightfalloff
	SPS_LIGHT_PROJECTION                  ,   //   lightprojection
	SPS_LIGHT_POSITION                    ,   //   lightposition
	SPS_LIGHT_DIFFUSE                     ,   //   lightdiffuse
	SPS_LIGHT_SPECALAR                    ,   //   lightspecular
	SPS_LIGHT_AMBIENT                     ,   //   lightambient
	SPS_LIGHT_DIRECTION                   ,   //   lightdirection

	SPS_COUNTER                           ,   //计数器,不使用
	SPS_NOT_USED,

	
	//以下为给编辑器用的semantic 
	SPS_EDITOR_COLOR,                    //EDITOR_COLOR
	
};										  

/*
enum eShaderParamSemantic
{
SPS_POSITION   = 0                    ,      position
SPS_DIRECTION                         ,      direction
SPS_DIFFUSE                           ,      diffuse
SPS_SPECULAR                          ,      specular
SPS_AMBIENT                           ,      ambient
SPS_SPECULARPOWER                     ,      specularpower
SPS_EMISSIVE                          ,      emissive
SPS_NORMAL                            ,      normal
SPS_HEIGHT                            ,      height
SPS_REFRACTION                        ,      refraction
SPS_OPACITY                           ,      opacity
SPS_ENVIRONMENT                       ,      environment
SPS_ENVIROMENTNORMAL                  ,      environmentnormal
SPS_FRESNEL                           ,      fresnel
SPS_WORLDMAT                          ,      world
SPS_WORLDTRANS_MAT                    ,      worldtranspose
SPS_WORLDINV_MAT                      ,      worldinverse
SPS_WORLDINVTRANS_MAT                 ,      worldinversetranspose
SPS_WORLDVIEW_MAT                     ,      worldview
SPS_WORLDVIEWINV_MAT                  ,      worldviewinverse
SPS_WORLDVIEWINVTRANS_MAT             ,      worldviewinversetranspose
SPS_WVP_MAT                           ,      worldviewprojection
SPS_WVPINV_MAT                        ,      worldviewprojectioninverse
SPS_WVPINVTRANS_MAT                   ,      worldviewprojectioninversetranspose
SPS_VIEW_MAT                          ,      view
SPS_VIEWTRANS_MAT                     ,      viewtranspose
SPS_VIEWPROJECTION_MAT                ,      viewprojection
SPS_VIEWINV_MAT                       ,      viewinverse
SPS_VIEWINVTRANS_MAT                  ,      viewinversetranspose
SPS_VIEWPROJECTIONTRANS_MAT           ,      viewprojectiontranspose           
SPS_WORLDVIEWTRANS_MAT                ,      worldviewtranspose
SPS_VIEWPROJECTIONINV_MAT             ,      viewprojectioninverse
SPS_VIEWPROJECTIONINVTRANS_MAT        ,      viewprojectioninversetranspose
SPS_PROJECTION_MAT                    ,      projection
SPS_PROJECTIONINV_MAT                 ,      projectioninverse
SPS_PROJECTIONINVTRANS_MAT            ,      projectioninversetranspose
SPS_TEXTURE_MAT                       ,      texturematrix

SPS_VIEWPORTPIXEL_SIZE                ,      viewportpixelsize
SPS_VIEWPORTCLIPPING                  ,      viewportclipping

SPS_TIME                              ,      time
SPS_LAST_TIME                         ,      lasttime
SPS_ELAPSED_TIME                      ,      elapsedtime

SPS_ATTENUATION                       ,      attenuation
SPS_RENDERTARGET_COLOR                ,      rendercolortarget
SPS_RENDERTARGET_DEPTH                ,      renderdepthstenciltarget
SPS_RANDOM                            ,      random
SPS_UNITSSCALE                        ,      unitsscale

SPS_CAMERAPOSITION                    ,      cameraposition

SPS_LIGHT_UMBRA                       ,      lightumbra 
SPS_LIGHT_PENMUBAR                    ,      lightpenumbra
SPS_LIGHT_RANGE                       ,      lightrange
SPS_LIGHT_FALLOFF                     ,      lightfalloff
SPS_LIGHT_PROJECTION                  ,      lightprojection
SPS_LIGHT_POSITION                    ,      lightposition
SPS_LIGHT_DIFFUSE                     ,      lightdiffuse
SPS_LIGHT_SPECALAR                    ,      lightspecular
SPS_LIGHT_AMBIENT                     ,      lightambient
SPS_LIGHT_DIRECTION                   ,      lightdirection

};	

*/

/*
the paramater's class 
include these class : Matrix float vector eg.
*/
enum eShaderParamClass 
{
	eSPClass_MATRIX4x4,
	eSPClass_MATRIX4x3,
	eSPClass_MATRIX3x3,
	eSPClass_FLOAT,
	eSPClass_FLOAT2,
	eSPClass_FLOAT3,
	eSPClass_FLOAT4,
	eSPClass_INT,
	eSPClass_INT2,
	eSPClass_INT3,
	eSPClass_INT4,
	eSPClass_BOOL,
	eSPClass_BOOL2,
	eSPClass_BOOL3,
	eSPClass_BOOL4,

	eSPClass_TEXTURE, 
	eSPClass_TEXTURE1D,
	eSPClass_TEXTURE2D,
	eSPClass_TEXTURE3D,
	eSPClass_TEXTURECUBE,
	eSPClass_SAMPLER,
	eSPClass_SAMPLER1D,
	eSPClass_SAMPLER2D,
	eSPClass_SAMPLER3D,
	eSPClass_SAMPLERCUBE,

	eSPClass_NotSupported,

};

struct  HShaderTextureParam
{
	IDirect3DTexture9* m_pTexture;
	char               m_TextureName[64];
};

class   HShaderParamater
{
protected:
	void __innerCopy(const HShaderParamater& val)
	{
		m_ParamClass = val.m_ParamClass;
		m_ParamSemantic= val.m_ParamSemantic;
		m_Name = val.m_Name;
		strncpy(m_strName,val.m_strName,32);
		m_nElement= val.m_nElement;//数组里的元素个数
		m_PramaData = NULL;
		m_DataBytes = val.m_DataBytes;
		_createParamData();
	}

public:
	eShaderParamClass       m_ParamClass;
	eShaderParamSemantic    m_ParamSemantic;
	int                     m_Name;
	char                    m_strName[32];
	int                     m_nElement;//数组里的元素个数
	int                     m_DataBytes;
	void*                   m_PramaData;
	void*                   m_hHandle;


public:
	HShaderParamater();
	HShaderParamater(const HShaderParamater& val)
	{
		__innerCopy(val);
	}

	HShaderParamater& operator = (const HShaderParamater& val)
	{
		__innerCopy(val);
		return *this;
	}

	virtual ~HShaderParamater();
	void*               _createParamData();
	void                _releasePramaData();

	std::wstring                     _getValueString();
	void                        _setValueString(const std::wstring& strValue);

	void*                       _getParamData();
	const eShaderParamClass&    _class()const         {return m_ParamClass;   }
	const eShaderParamSemantic& _semantic()const      {return m_ParamSemantic;}
	const int                   _name()const          {return m_Name;         }
	virtual const unsigned long _get_location() const = 0;
};


//Shader的参数
class CShaderParam: public HShaderParamater
{
public:

	CShaderParam():HShaderParamater(){m_hHandle = NULL;}
	CShaderParam(const CShaderParam& val)
	{
		m_hHandle = val.m_hHandle;
		__innerCopy(val);
	}

	CShaderParam& operator = (const CShaderParam& val)
	{
		m_hHandle = val.m_hHandle;
		__innerCopy(val);
		return *this;
	}
	const unsigned long    _get_location() const {return (const unsigned long)m_hHandle;}

};

//Shader的参数列表
class CFxShader;
class IStreamIO;
class CShaderParamTable
{
	friend class CFxShader;
	friend class CD3DShader;
	friend class CShaderD3DBase;
public:
    int    numOfParamater(){ return m_nParamater;}
	HShaderParamater& operator[](size_t index)
	{
		assert(index < (size_t) m_nParamater);
		return m_Paramaters[(unsigned int)index];
	}
	int    find(const char* name) ;
	bool   load(IStreamIO* pReader);
	bool   save(IStreamIO* pWriter);
	bool   release();
protected:
    vector<CShaderParam>   m_Paramaters;
    int                       m_nParamater;
};

//所有的D3D类的Shader包括 Effect(.fx) VertexShader/PixelShader都将从这里派生
class   CShaderD3DBase : public CRefCountObject
{
	//把EffectShader的类的操作屏蔽掉
public:   
	virtual bool         begin(size_t *pPasses, unsigned long Flags){return true;}
	virtual bool         beginPass(size_t Pass){return true;}
	virtual bool         commitChanges(){return true;}
	virtual bool         endPass(){return true;}
	virtual bool         end(){return true;}
	virtual bool         setTechnique(const char * technique){return true;}
	virtual bool         commitParamTable() ;
	virtual bool         commitParamTable(CShaderParamTable& table) ;

	virtual D3DXHANDLE   __getD3DXHandle(const char* pConstName) = 0;

    eShaderParamSemantic parseEditorSemantic(const char* semantic);
    eShaderParamSemantic parseSemantic(const char* semantic);

    //参数表的控制
    virtual bool     setParamValue(const HShaderParamater&  hParam , float  value ) = 0 ;
    virtual bool     setParamValue(const HShaderParamater&  hParam , const float* value , int n, int baseIndex = 0) = 0 ;

    virtual bool     setParamValue(const HShaderParamater&  hParam , int    value ) = 0 ;
    virtual bool     setParamValue(const HShaderParamater&  hParam , const int*   value , int n, int baseIndex = 0) = 0 ;


    virtual bool     setParamValue(const HShaderParamater&  hParam , bool   value ) = 0 ;
    virtual bool     setParamValue(const HShaderParamater&  hParam , const bool*  value , int n, int baseIndex = 0) = 0 ;

    virtual bool     setParamValue(const HShaderParamater&  hParam , const D3DXVECTOR4& vec) = 0 ;
    virtual bool     setParamValue(const HShaderParamater&  hParam , const D3DXVECTOR4* vecs, int nVector, int baseIndex = 0) = 0 ;


    virtual bool     setParamValue(const HShaderParamater&  hParam ,const  D3DXMATRIX& mat) = 0 ;
    virtual bool     setParamValue(const HShaderParamater&  hParam ,const  D3DXMATRIX* mats, int nMat, int baseIndex = 0) = 0 ;



    //按照名字来设置参数
    virtual bool     setParamValue(const char* Name , float    v  ) = 0 ;
    virtual bool     setParamValue(const char* Name , const float*   v , int Count, int baseIndex = 0) = 0 ;

    virtual bool     setParamValue(const char* Name , bool    v  ) = 0 ;
    virtual bool     setParamValue(const char* Name , const bool*   v , int Count, int baseIndex = 0) = 0 ;

    virtual bool     setParamValue(const char* Name , int      v  ) = 0 ;
    virtual bool     setParamValue(const char* Name , const int*     v , int Count, int baseIndex = 0) = 0 ;

    virtual bool     setParamValue(const char* Name , const D3DXVECTOR4& vec) = 0 ;
    virtual bool     setParamValue(const char* Name , const D3DXVECTOR4* vecs, int nVector, int baseIndex = 0) = 0 ;

    virtual bool     setParamValue(const char* Name , const  D3DXMATRIX& mat) = 0 ;
    virtual bool     setParamValue(const char* Name , const  D3DXMATRIX* mats, int nMat, int baseIndex = 0) = 0 ;


    virtual bool     setTexture(const char* Name                , IDirect3DTexture9* pTexture) = 0;
    virtual bool     setTexture(const HShaderParamater&  hParam , IDirect3DTexture9* pTexture) = 0;

    //基础加载代码
    virtual bool     load(const wchar_t* _Shader) = 0;
    virtual void     bind() = 0;
    virtual void     unbind() = 0;
    virtual bool     unload() = 0;

    static   float ElaspedTime();
    static   float frameTime();
public:	
	//自动设置那                    些有确定Semantic含义的变量
	bool                     _autoSetParamter();
protected:
    CShaderParamTable     m_ParamTable;
    CShaderParamTable     m_AutoSetParamTable;
    CA3DDeviceWrapper*    m_DeviceWrapper;
};
class   CD3DShader : public CShaderD3DBase
{

protected:
	LPD3DXCONSTANTTABLE   m_pConstTable;
	int                   __build_const_table();
	eShaderParamClass     __parseParamClass(D3DXCONSTANT_DESC& param_desc);
public:
	D3DXHANDLE            __getD3DXHandle(const char* pConstName);
	int                   addRef() ;
	void                  release();

    
	CShaderParamTable& getAutoSetParamTable();
	CShaderParamTable& getParamTable();

	bool     setParamValue(const HShaderParamater& hParam , float  value );
	bool     setParamValue(const HShaderParamater& hParam , const  float* value , int n, int baseIndex = 0);

	bool     setParamValue(const HShaderParamater& hParam , int    value );
	bool     setParamValue(const HShaderParamater& hParam , const  int*   value , int n, int baseIndex = 0);


	bool     setParamValue(const HShaderParamater& hParam , bool   value );
	bool     setParamValue(const HShaderParamater& hParam , const  bool*  value , int n, int baseIndex = 0);
										   
	bool     setParamValue(const HShaderParamater& hParam , const D3DXVECTOR4& vec);
	bool     setParamValue(const HShaderParamater& hParam , const D3DXVECTOR4* vecs, int nVector, int baseIndex = 0);


	bool     setParamValue(const HShaderParamater& hParam ,const  D3DXMATRIX& mat);
	bool     setParamValue(const HShaderParamater& hParam ,const  D3DXMATRIX* mats, int nMat, int baseIndex = 0);


	//按照名字来设置参数
	bool     setParamValue(const char* Name , float    v  );
	bool     setParamValue(const char* Name , const float*   v , int Count, int baseIndex = 0);

	bool     setParamValue(const char* Name , bool    v  );
	bool     setParamValue(const char* Name , const bool*   v , int Count, int baseIndex = 0);

	bool     setParamValue(const char* Name , int      v  );
	bool     setParamValue(const char* Name , const int*     v , int Count, int baseIndex = 0);

	bool     setParamValue(const char* Name , const D3DXVECTOR4& vec);
	bool     setParamValue(const char* Name , const D3DXVECTOR4* vecs, int nVector, int baseIndex = 0);

	bool     setParamValue(const char* Name , const  D3DXMATRIX& mat);
	bool     setParamValue(const char* Name , const  D3DXMATRIX* mats, int nMat, int baseIndex = 0);

	bool     setTexture(const char* Name                , IDirect3DTexture9* pTexture);
	bool     setTexture(const HShaderParamater&  hParam , IDirect3DTexture9* pTexture);


};

class  CPixelShader : public CD3DShader
{
	IDirect3DPixelShader9* m_pPixelShader;
public:
	static bool bValidPS(DWORD PSversion);
	CPixelShader();
	bool               load(const wchar_t* pixelShader);
	void               bind();
	void               unbind();
	bool               unload();
	bool               isLoaded(){return m_pPixelShader != NULL;}

	bool               isSurpport(){return m_pPixelShader!= NULL;};
	eShaderType        type(){return ShaderType_Pixel;}
};


class   CVertexShaderD3D : public CD3DShader
{
	IDirect3DVertexShader9* m_pVertexShader;
public:
	static bool bValidVS(DWORD VSversion);
	CVertexShaderD3D();
	bool load(const wchar_t* vertexShader);
	void bind();
	void unbind();
	bool unload();
	bool isLoaded(){return m_pVertexShader != NULL;}
	bool isSurpport(){return m_pVertexShader!= NULL;};
	eShaderType        type(){return ShaderType_Vertex;}
};



void  strToMatrix3x3(const wchar_t* str , D3DXMATRIX& out);
void  strToMatrix4x4(const wchar_t* str , D3DXMATRIX& out);
void  strToVector4(const wchar_t* str   , D3DXVECTOR4& point);
void  strToFloatArray(const wchar_t* str, float* out, int nValue);
void  strToIntArray(const wchar_t* str, int* out, int nValue);
void  strToBOOLArray(const wchar_t* str, BOOL* out, int nValue);


std::wstring    strFromMatrix4x4( D3DXMATRIX& out);
std::wstring    strFromVector4(D3DXVECTOR4& point);
std::wstring    strFromFloatArray(float* out, int nValue);
std::wstring    strFromIntArray(int* out, int nValue);
std::wstring    strFromBOOLArray(BOOL* out, int nValue);



#endif