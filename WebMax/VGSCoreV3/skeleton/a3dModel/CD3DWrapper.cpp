#include "TextureManager.h"
#include "CD3DInclude.h"
#include "Shader/FXShader.h"
#include "Shader/D3DShader.h"
#include "../A3DHelperFunction.h"
#include "ShaderResource.h"

CA3DDeviceWrapper::CA3DDeviceWrapper()
{
    m_HasGpuSkin = false;
    m_pDevice = NULL ;
    m_hInstance = GetModuleHandle(NULL);
}

bool CA3DDeviceWrapper::Init(IDirect3DDevice9* pDevice , HINSTANCE hInstance)
{
    m_pDevice = pDevice;

    if(hInstance != NULL )
    {
        m_hInstance = hInstance ;
    }

    if(m_pDevice != NULL)
    { 
        m_pDevice->AddRef();
        D3DCAPS9 Caps;
        m_pDevice->GetDeviceCaps(&Caps);
        int MaxVertexShaderConst = Caps.MaxVertexShaderConst;
        int VsVersion    = (LOWORD(Caps.VertexShaderVersion) & 0xff00) >> 8;
        int PsVersion    = (LOWORD(Caps.PixelShaderVersion ) & 0xff00) >> 8;
        int VsSubVersion = LOWORD(Caps.VertexShaderVersion)  & 0x00ff;
        int PsSubVersion = LOWORD(Caps.PixelShaderVersion )  & 0x00ff;


        if(VsVersion >= 3 && PsVersion >= 3 && MaxVertexShaderConst >= 248)
        {
            m_HasGpuSkin = true;
        }
        if(VsVersion >= 2 && PsVersion >= 2 )
        {

        }
        addShader(m_hInstance , IDFX_SKIN_PHONG ,  L"BasicGpuSkin");
        addShader(A3D_ABSPATH(L".\\shader\\PhongGpuSkin.fx")  , L"PhongGpuSkin");
        
    }
    return true;
}

CFxShader* CA3DDeviceWrapper::addShader(const wchar_t* shaderFile , const wchar_t* shaderName )
{
      if(shaderName == NULL) shaderName = shaderFile;
      CFxShader* _ret = FindShader(shaderName);
      if(_ret) return _ret;
      

      _ret = new CFxShader(this ); 
      if(true == _ret->load(A3D_ABSPATH(shaderFile) ) )
      {
          m_vFxShaders[shaderName] = _ret;
      }
      else
      {
          delete _ret;
          _ret = NULL;
      }
      return _ret;
}

CFxShader* CA3DDeviceWrapper::addShader(HINSTANCE hInstance , UINT ResID ,  const wchar_t* shaderName )
{
    CFxShader* _ret = FindShader(shaderName);
    if(_ret) return _ret;


    _ret = new CFxShader(this ); 
    if(true == _ret->load(hInstance , ResID  ) )
    {
        m_vFxShaders[shaderName] = _ret;
    }
    else
    {
        delete _ret;
        _ret = NULL;
    }
    return _ret;

}

CFxShader* CA3DDeviceWrapper::addShader(const wchar_t* shaderName , int includeID , const char* _includeCode)
{
     CFxShader* pShader = FindShader(shaderName);
     if(pShader == NULL)
         return NULL;

     wchar_t buf[32];
     swprintf(buf , L"0X%xH" , includeID);
     std::wstring _FullName = std::wstring(shaderName) + L"[" + buf + L"]";
     CFxShader* pNewShader = FindShader(_FullName.c_str() );
     if(pNewShader)
         return pNewShader;

     pNewShader = new CFxShader(this);
     pNewShader->appendInclude(_includeCode);
     pNewShader->cloneLoadInfoFrom(pShader);
     if(false == pNewShader->reLoad() )
     {
         delete pNewShader;
         return NULL;
     }
     m_vFxShaders[_FullName] = pNewShader;
     return pNewShader;
}

void CA3DDeviceWrapper::copyMatrix(D3DXMATRIX& Source , D3DXVECTOR4* pDest)
{
    D3DXVECTOR4& v1 = pDest[0];
    //转置
    D3DXMATRIX tMat;
    D3DXMatrixTranspose(&tMat , &Source);
    D3DXVECTOR4*    pSource = (D3DXVECTOR4*)&tMat;
    pDest[0] = pSource[0];
    pDest[1] = pSource[1];
    pDest[2] = pSource[2];
}
CFxShader*   CA3DDeviceWrapper::FindShader(const wchar_t* _shader)
{
    mapFxShaders::iterator pos = m_vFxShaders.find(_shader);
    if(pos != m_vFxShaders.end() )        
	{
		CFxShader*  pShader = pos->second;
		return pShader;
	}
    return NULL;
}

void CA3DDeviceWrapper::setBoneFrames(D3DXMATRIX* boneFrames , int nBone)
{
    //4x4矩阵转置后，取3x4
    D3DXMATRIX*   pSource = boneFrames;
    D3DXVECTOR4*   pDest   = m_BoneFrames;
    for(int i = 0 ; i < nBone && i < MAX_BONE ; i ++)
    {
        D3DXMATRIX&     Source = pSource[i];
        D3DXVECTOR4&    Dest   = pDest[i * 3];
        copyMatrix(Source , &Dest);
    }
}
void CA3DDeviceWrapper::Uninit() 
{
    //4x4矩阵转置后，取3x4
    mapFxShaders::iterator pos = m_vFxShaders.begin();
    for( ; pos != m_vFxShaders.end() ; pos ++)
    {
        CFxShader* pShader = pos->second;
        pShader->Release();
    }
    m_vFxShaders.clear();
    m_pDevice->Release();
}
CA3DDeviceWrapper::~CA3DDeviceWrapper()
{
}

bool CA3DDeviceWrapper::afterDeviceLost()
{
	mapFxShaders::iterator pos = m_vFxShaders.begin();
	for( ; pos != m_vFxShaders.end() ; pos ++)
	{
		CFxShader* pShader = pos->second;
		pShader->reLoad();
	}
	return true;  
}

bool CA3DDeviceWrapper::HasGpuSkin( int maxBone)
{ 
	return m_HasGpuSkin && maxBone <= 72; 
}

bool CA3DDeviceWrapper::onDeviceLost()
{
	mapFxShaders::iterator pos = m_vFxShaders.begin();
	for( ; pos != m_vFxShaders.end() ; pos ++)
	{
		CFxShader* pShader = pos->second;
		pShader->unload();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
void xTextureOp::Build()
{

}

xTextureOp::xTextureOp()
{
    m_iChannel  = 0;
    m_COLOROP   = D3DTOP_MODULATE;
    m_COLORARG1 = D3DTA_TEXTURE;
    m_COLORARG2 = D3DTA_CURRENT;
    m_ALPHAOP   = D3DTOP_SELECTARG1;
    m_ALPHAARG1 = D3DTA_TEXTURE;
    m_ALPHAARG2 = D3DTA_TEXTURE;
}

//////////////////////////////////////////////////////////////////////////
xMeshMaterial::xMeshMaterial()
{
    m_Ambient    = D3DXCOLOR(0.0f , 0.0f , 0.0f , 1.0f);
    m_Diffuse    = D3DXCOLOR(1.0f , 1.0f , 1.0f , 1.0f);
    m_Speculer   = D3DXCOLOR(1.0f , 1.0f , 1.0f , 1.0f);
    m_Emissive   = D3DXCOLOR(0.0f , 0.0f , 0.0f , 1.0f);
    m_fShiness   = 1.0f;
    m_fOpactiy   = 1.0f;
    m_FxShader   = NULL;
    m_ShaderName = L"";
}

std::string GetArgString(DWORD _arg)
{
     switch(_arg)    
     {
     case D3DTA_DIFFUSE  :  return "diffuse"  ;    break;// select diffuse color (read only)
     case D3DTA_CURRENT  :  return "_cl"      ;    break; // select stage destination register (read/write)
     case D3DTA_TEXTURE  :  return "_texColor";    break;  // select texture color (read only)
     default : return "_cl"      ;
     }
};

std::string _GetOpString(D3DTEXTUREOP _OP)
{
    switch( _OP)
    {
    case D3DTOP_MODULATE   : return " * " ; break;
    case D3DTOP_ADD        : return " + " ; break; 
    case D3DTOP_SELECTARG1 : return "arg1"; break;
    case D3DTOP_SELECTARG2 : return "arg2"; break;
    default:
        return " * ";
    }
}

std::string xMeshMaterial::BuildTextureShader()
{
    std::string _ret = "#define _NO_DEFAULT_TEXTURE_ \n";
    char buf[1024] = {0};
	int i = 0;
    for(i = 0 ; i < m_vTextures.size() ; i ++)
    {
        sprintf(buf , "sampler2D    _Texture%d : register(s%d);\n" , i , i);
        _ret += buf;
    }
    
    int iMaxChannel = 1;
    sprintf(buf , "float4       DoTexture(float2 _texLayer[%d], float4 diffuse)\n" , iMaxChannel);
    _ret += buf;
    _ret += "{\n";
    _ret += "    float4 _cl = diffuse;\n";
    for(i = 0 ; i < m_vTextures.size() ; i ++)
    {
        xTextureOp _op = m_vTextures[i].m_TexOp;
        std::string _clOp   = _GetOpString(_op.m_COLOROP);
        std::string _clArg1 = GetArgString(_op.m_COLORARG1);
        std::string _clArg2 = GetArgString(_op.m_COLORARG2);

        std::string _aOp    = _GetOpString(_op.m_ALPHAOP);
        std::string _aArg1  = GetArgString(_op.m_ALPHAARG1);
        std::string _aArg2  = GetArgString(_op.m_ALPHAARG2);

        //一个TextureStage
        _ret += "	{\n";
        sprintf(buf , "		 float4 _texColor = tex2D(_Texture%d , _texLayer[%d]);\n" , i , _op.m_iChannel);
        _ret += buf;
        //Alpha和Color操作相同
        if(_clOp == _aOp && _clArg1 == _aArg1 && _clArg2 == _aArg2)
        {
            std::string _stageString = "";
            switch(_op.m_COLOROP)
            {
            case D3DTOP_SELECTARG1: _stageString = std::string("		 _cl=") +  _clArg1 +  ";\n" ;  break;
            case D3DTOP_SELECTARG2: _stageString = std::string("		 _cl=") +  _clArg2 +  ";\n" ;  break;
            default:
                {
                    _stageString = std::string("		 _cl=") + _clArg1  +  _clOp + _clArg2 + ";\n" ;
                };
            break;
            }

            _ret += _stageString;
        }
        else
        {
            //RGB的混合
            std::string _stageString = "";
            switch(_op.m_COLOROP)
            {
            case D3DTOP_SELECTARG1: _stageString = std::string("		 _cl.xyz=") +  _clArg1 +  ".xyz;\n" ;  break;
            case D3DTOP_SELECTARG2: _stageString = std::string("		 _cl.xyz=") +  _clArg2 +  ".xyz;\n" ;  break;
            default:
                {
                    _stageString = std::string("		 _cl.xyz=") + _clArg1+ ".xyz"  +  _clOp + _clArg2 + ".xyz;\n" ;
                };
                break;
            }
            _ret += _stageString;

            //ALPHA的混合
            switch(_op.m_ALPHAOP)
            {
            case D3DTOP_SELECTARG1: _stageString = std::string("		 _cl.w=") +  _aArg1 +  ".w;\n" ;  break;
            case D3DTOP_SELECTARG2: _stageString = std::string("		 _cl.w=") +  _aArg2 +  ".w;\n" ;  break;
            default:
                {
                    _stageString = std::string("		 _cl.w=") + _aArg1+ ".w"  +  _aOp + _aArg2 + ".w;\n" ;
                };
                break;
            }
            _ret += _stageString;

        }
        _ret += "	}\n";
    }

    //结束
    _ret += "   return _cl;\n";
    _ret += "}\n";
    
    return _ret;

}

void xMeshMaterial::SetShader(CA3DDeviceWrapper* pDeviceWrapper , const wchar_t* wcsFileName)
{
    if(wcsFileName == NULL ) wcsFileName = L"";
    m_ShaderName = wcsFileName;
    if(m_ShaderName.length() == 0)
    {
        XSAFE_RELEASE(m_FxShader);
        return ;
    }
    //如果不是特定的Shader。则可以支持加载Shader
    int includeID = xStringHash(m_NameID.c_str());
    std::string includeCode = BuildTextureShader();
    CFxShader* pShader =  pDeviceWrapper->addShader(wcsFileName , includeID , includeCode.c_str() ); //pDeviceWrapper->FindShader(wcsFileName);//

    m_FxShader   =  pShader;
    if(m_FxShader) m_FxShader->AddRef();
}

void xMeshMaterial::Begin(IDirect3DDevice9* pDevice)
{
     if(m_FxShader)
     {
         m_FxShader->bind();
     }
     size_t nTex = m_vTextures.size() ;
     for(size_t iTex = 0 ; iTex < nTex ; iTex ++)
     {
         xMeshTexture& _texture = m_vTextures[iTex];
         xA3DTexture* pA3DTexture = _texture.m_hTexture.getResource();
         if(pA3DTexture)
         {
             pDevice->SetTexture( (int)iTex , pA3DTexture->m_pTexture );
         }
         else
         {
             pDevice->SetTexture( (int)iTex , NULL );
         }

         pDevice->SetTextureStageState(iTex , D3DTSS_TEXCOORDINDEX , _texture.m_TexOp.m_iChannel);
         pDevice->SetTextureStageState(iTex , D3DTSS_COLOROP   ,  _texture.m_TexOp.m_COLOROP  );
         pDevice->SetTextureStageState(iTex , D3DTSS_COLORARG1 ,  _texture.m_TexOp.m_COLORARG1);
         pDevice->SetTextureStageState(iTex , D3DTSS_COLORARG2 ,  _texture.m_TexOp.m_COLORARG2);

         pDevice->SetTextureStageState(iTex , D3DTSS_ALPHAOP   ,  _texture.m_TexOp.m_ALPHAOP);
         pDevice->SetTextureStageState(iTex , D3DTSS_ALPHAARG1 ,  _texture.m_TexOp.m_ALPHAARG1);
         pDevice->SetTextureStageState(iTex , D3DTSS_ALPHAARG2 ,  _texture.m_TexOp.m_ALPHAARG2);

     }
}


void xMeshMaterial::End(IDirect3DDevice9* pDevice)
{
    if(m_FxShader)
    {
        m_FxShader->unbind();
    }
	size_t nTex = m_vTextures.size() ;
	for(size_t iTex = 0 ; iTex < nTex ; iTex ++)
	{
		pDevice->SetTexture( (int)iTex , NULL );
	}
}

const wchar_t* GetOpString(D3DTEXTUREOP _OP)
{
    switch( _OP)
    {
    case D3DTOP_MODULATE : return L" * "; break;
           // Add
    case D3DTOP_ADD      : return L" + " ; break; 
    default:
        return L" * ";
    }
}
void xMeshMaterial::Build()
{
    std::wstring _idString;
    wchar_t _buf[128] = {0};
    swprintf(_buf , L"nLayer=%d\n" , m_vTextures.size() );
    _idString += _buf;
    for(size_t i = 0 ; i < m_vTextures.size() ; i ++)
    {
        xTextureOp& _op  = m_vTextures[i].m_TexOp;
        swprintf(_buf , L"iLayer=%d iChanel=%d [" , i , _op.m_iChannel );
        _idString += _buf;
        swprintf(_buf , L"c=%d%s%d , a=%d%s%d ]\n" , _op.m_COLORARG1 , GetOpString(_op.m_COLOROP) , _op.m_COLORARG2 
                                                 , _op.m_ALPHAARG1 , GetOpString(_op.m_ALPHAOP) , _op.m_ALPHAARG2 );

        _idString += _buf;
    }

    m_NameID = _idString;
}