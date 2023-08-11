#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "xStdInclude.h"
#include "../xcomdoc/xStringHash.h"
#include "xResPackageMgr.h"

#pragma warning (disable:4311)
#pragma warning (disable:4312)

#pragma comment(lib,"d3d9.lib")

#pragma comment(lib,"dxguid.lib")
#ifdef _USE_SUMMER2004_DXSDK_
#pragma comment(lib,"d3dx9.lib")
#else
#ifdef _DEBUG
#pragma comment(lib,"d3dx9.lib")
#else
#pragma comment(lib,"d3dx9.lib")
#endif
#endif

class _A3D_API_  xA3DTexture
{
public:
    IDirect3DTexture9* m_pTexture;
    D3DFORMAT          m_FileFormat;
    xA3DTexture()
    {
        m_pTexture = NULL;
        m_FileFormat = D3DFMT_UNKNOWN;
    }
    ~xA3DTexture()
    {
        if(m_pTexture)
        {
            m_pTexture->Release();
            m_pTexture = NULL;
        }
    }
};

typedef TResHandle      <xA3DTexture , CTextureName , CBaseTextureLoader> HBaseTexture;
typedef TLRUPolicyResMgr<xA3DTexture , CTextureName , CBaseTextureLoader> CBaseTextureMgr;
typedef std::vector<xMeshTexture > vMeshTextures; 
typedef std::vector<xMeshMaterial> vMeshMaterials; 

enum eTexLayerUsage
{
    tex_usage_none          = -1,
    tex_usage_ambient       = 0,  // ambient
    tex_usage_diffuse       = 1,  // diffuse
    tex_usage_specular      = 2,   // specular
    tex_usage_shininesNs    = 3,   // shininesNs
    tex_usage_shininess     = 4,   // shininess strength
    tex_usage_self_illu     = 5,   // self-illumination
    tex_usage_opacity       = 6,   // opacity
    tex_usage_filter_color  = 7,   // filter color
    tex_usage_bump          = 8,   // bump 
    tex_usage_reflection    = 9,   // reflection
    tex_usage_refraction    = 10,  // refraction 
    tex_usage_displacement  = 11,  // displacement 
    tex_usage_max
};

class _A3D_API_ xTextureOp
{
public:
    int32          m_iChannel;
    eTexLayerUsage m_iUsage;
    D3DTEXTUREOP   m_COLOROP;
    DWORD          m_COLORARG1;
    DWORD          m_COLORARG2;
    D3DTEXTUREOP   m_ALPHAOP;
    DWORD          m_ALPHAARG1;
    DWORD          m_ALPHAARG2;
    xTextureOp();
    void Build();
};


class _A3D_API_ xMeshTexture
{
public:
    std::wstring       m_TexName;
    HBaseTexture       m_hTexture;
    xTextureOp         m_TexOp;

};

class _A3D_API_ xMeshMaterial
{
public:
    D3DXCOLOR        m_Ambient ;
    D3DXCOLOR        m_Diffuse ;
    D3DXCOLOR        m_Speculer ;
    D3DXCOLOR        m_Emissive;
    std::wstring     m_Script;
    float            m_fShiness;
    float            m_fOpactiy;
    vMeshTextures    m_vTextures;

    std::wstring     m_ShaderName;
    CFxShader*       m_FxShader;
    std::wstring     m_NameID;
public:
    xMeshMaterial();
    void SetShader(CA3DDeviceWrapper* pWrapper , const wchar_t* shaderName);
    void Begin(IDirect3DDevice9* pDevice);    
    void End(IDirect3DDevice9* pDevice);
    void Build();

    std::string BuildTextureShader();

};

template <typename T> int  XSAFE_RELEASE(T& obj)
{
    if(obj)
    {
        int RefCount = obj->Release();
        obj=NULL;
        return RefCount;
    }
    return 0;
}

template <typename T> bool  XSAFE_DELETE(T* & pObj)
{
    if(pObj)
    {
        delete pObj;
        pObj=NULL;
        return true;
    }
    return false;
}

template <typename T> bool  XSAFE_DELETE_ARRAY(T* & pObj)
{
    if(pObj)
    {
        delete [] pObj;
        pObj=NULL;
        return true;
    }
    return false;
}

struct _A3D_API_ CBoundBox
{
    D3DXVECTOR3 m_Min;
    D3DXVECTOR3 m_Max;
	
    void AddPoint(const D3DXVECTOR3& v)
    {
        if(v.x > m_Max.x) m_Max.x = v.x;
        if(v.y > m_Max.y) m_Max.y = v.y;
        if(v.z > m_Max.z) m_Max.z = v.z;
		
        if(v.x < m_Min.x) m_Min.x = v.x;
        if(v.y < m_Min.y) m_Min.y = v.y;
        if(v.z < m_Min.z) m_Min.z = v.z;
    }
    void AddPoint(const D3DXVECTOR4& v)
    {
        if(v.x > m_Max.x) m_Max.x = v.x;
        if(v.y > m_Max.y) m_Max.y = v.y;
        if(v.z > m_Max.z) m_Max.z = v.z;
		
        if(v.x < m_Min.x) m_Min.x = v.x;
        if(v.y < m_Min.y) m_Min.y = v.y;
        if(v.z < m_Min.z) m_Min.z = v.z;
    }
	
    void Init(float len = 1000000.0f)
    {
        m_Min.x = len;	m_Max.x = -len;
        m_Min.y = len;	m_Max.y = -len;
        m_Min.z = len;	m_Max.z = -len;
    }
	
	
};

struct _A3D_API_ CCameraEyeInfo
{
public:
    D3DXVECTOR4 m_Up;
    D3DXVECTOR4 m_EyePos;
    D3DXVECTOR4 m_EyeDir;
    union
    {
        struct 
        {
            float           m_fNear;
            float           m_fFar;
            float           m_fFov;
            float           m_fAspect;
        };
        float       m_CameraArg[4];
    };
};

struct _A3D_API_ CTransformContext
{
public:
    D3DXMATRIX         m_matWorld;
    D3DXMATRIX         m_matView;
    D3DXMATRIX         m_matProject;
    D3DXMATRIX         m_matTexture;
    CCameraEyeInfo     m_CameraInfo;
    void* data(){  return &m_matWorld ; }
    DWORD dataLen(){ return sizeof(CTransformContext) ; }
};
#define MAX_LIGHT 4
struct _A3D_API_ CLightState
{   
    int             nLight[4];  
    D3DXCOLOR       m_Ambient [MAX_LIGHT];
    D3DXCOLOR       m_Diffuse [MAX_LIGHT];
    D3DXCOLOR       m_Speculer[MAX_LIGHT];
    D3DXVECTOR4     m_Position[MAX_LIGHT];
    D3DXVECTOR4     m_dir     [MAX_LIGHT];
    void* data(){  return &nLight ; }
    DWORD dataLen(){ return sizeof(CLightState) ; }
};

class _A3D_API_ CA3DDeviceWrapper
{
public:
    enum 
    {
        MAX_BONE = 72,
    };
    IDirect3DDevice9*   m_pDevice;
    CTransformContext   m_TransformBuffer;
    CLightState         m_LightState;
    D3DXVECTOR4         m_BoneFrames[ MAX_BONE * 3];

protected:
    typedef std::map<std::wstring , CFxShader*>    mapFxShaders;
public:
    CA3DDeviceWrapper();
    ~CA3DDeviceWrapper();
    void               copyMatrix(D3DXMATRIX& Source , D3DXVECTOR4* pDest);
    void               setBoneFrames(D3DXMATRIX* boneFrames , int nBone);
    bool               Init(IDirect3DDevice9* pDevice , HINSTANCE hInstance);
    //operator IDirect3DDevice9*(){ return m_pDevice ; }
    CFxShader*         FindShader(const wchar_t* _shader);
    CFxShader*         addShader(const wchar_t* shaderFile , const wchar_t* shaderName = NULL);
    CFxShader*         addShader(HINSTANCE hInstance , UINT ResID ,  const wchar_t* shaderName ) ;
    CFxShader*         addShader(const wchar_t* shaderName , int includeID , const char* _includeCode);
    bool               HasGpuSkin( int maxBone);
	bool               onDeviceLost();
	bool               afterDeviceLost();
    void               Uninit() ;
protected:
    mapFxShaders       m_vFxShaders;
    bool               m_HasGpuSkin;
    HINSTANCE          m_hInstance;
};