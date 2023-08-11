#include "../TextureManager.h"
#include "FXShader.h"
#include "../../xcomdoc/xStringHash.h"
CFxShader::CFxShader(CA3DDeviceWrapper* pDevice)
{
    m_pEffect    = NULL;
	m_DeviceWrapper = pDevice;
	m_hTechLow   = NULL;
	m_hTechBase  = NULL;
	m_hTechHigh  = NULL;
    m_IncludeCode = "";
}

void CFxShader::appendInclude(const char* _code)
{
    m_IncludeCode += _code;
    m_IncludeCode += "\n";
}

bool CFxShader::setParamValue(const HShaderParamater& hParam , float  value )
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	if(FAILED(m_pEffect->SetFloat(hD3DXHandle,value)) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const HShaderParamater& hParam , const float* value , int n, int baseIndex)
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;

	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;
	if(hD3DXHandle == NULL) 
		return false;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;


	if(FAILED(m_pEffect->SetFloatArray(hD3DXHandle,value,n)) )
	{
		return false;
	}
	return true;
}
bool CFxShader::setParamValue(const HShaderParamater& hParam , int    value )
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	if(FAILED(m_pEffect->SetInt(hD3DXHandle,value)) )
	{
		return false;
	}
	return true;
}
bool CFxShader::setParamValue(const HShaderParamater& hParam , const int*   value , int n, int baseIndex)
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;

	if(FAILED(m_pEffect->SetIntArray(hD3DXHandle,value,n)) )
	{
		return false;
	}
	return true;
}


bool CFxShader::setParamValue(const HShaderParamater& hParam , bool   value )
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	if(FAILED(m_pEffect->SetBool(hD3DXHandle,value) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const HShaderParamater& hParam , const bool*  value , int n, int baseIndex)
{
	BOOL* array = new BOOL[n];
	for(int i = 0 ; i < n ; ++i) array[i] =  value[i]?TRUE:FALSE;

	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;

	if(FAILED(m_pEffect->SetBoolArray(hD3DXHandle,array , n ) ) )
	{
		return false;
	}
	return true;

}

bool CFxShader::setParamValue(const HShaderParamater& hParam , const D3DXVECTOR4& vec)
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	if(FAILED(m_pEffect->SetVector( hD3DXHandle, (D3DXVECTOR4*)&vec  ) ) )
	{
		return false;
	}
	return true;
}


bool CFxShader::setParamValue(const HShaderParamater& hParam , const D3DXVECTOR4* vecs, int nVector, int baseIndex)
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;

	if(FAILED(m_pEffect->SetVectorArray( hD3DXHandle, (D3DXVECTOR4*)vecs , nVector  ) ) )
	{
		return false;
	}
	return true;
}


bool CFxShader::setParamValue(const HShaderParamater& hParam ,const  D3DXMATRIX& mat)
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	if(FAILED(m_pEffect->SetMatrix(   hD3DXHandle, (D3DXMATRIX*)&mat ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const HShaderParamater& hParam ,const  D3DXMATRIX* mats, int nMat, int baseIndex)
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;

	if(FAILED(m_pEffect->SetMatrixArray( hD3DXHandle, (D3DXMATRIX*)mats , nMat  ) ) )
	{
		return false;
	}
	return true;
}


//按照名字来设置参数
bool CFxShader::setParamValue(const char* Name , float    v  )
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;

	if(FAILED(m_pEffect->SetFloat(hD3DXHandle,v  ) ) )
	{
		return false;
	}
	return true;
}
bool CFxShader::setParamValue(const char* Name , const float*   v , int Count, int baseIndex)
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;

	if(FAILED(m_pEffect->SetFloatArray(hD3DXHandle,v , Count ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const char* Name , bool    v  )
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;

	if(FAILED(m_pEffect->SetBool(hD3DXHandle, v ) ) )
	{
		return false;
	}
	return true;
}
bool CFxShader::setParamValue(const char* Name ,const  bool*   v , int Count, int baseIndex)
{
	BOOL* array = new BOOL[Count];
	for(int i = 0 ; i < Count ; ++i) array[i] =  v[i]?TRUE:FALSE;

	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;
    
	if(FAILED(m_pEffect->SetBoolArray(hD3DXHandle,array , Count ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const char* Name , int      v  )
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;
	if(FAILED(m_pEffect->SetInt( hD3DXHandle,   v ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const char* Name ,const  int*     v , int Count, int baseIndex)
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;

	if(FAILED(m_pEffect->SetIntArray( hD3DXHandle,  v , Count ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const char* Name , const D3DXVECTOR4& vec)
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;
	if(FAILED(m_pEffect->SetVector( hD3DXHandle,  (D3DXVECTOR4*)&vec ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const char* Name , const D3DXVECTOR4* vecs, int nVector, int baseIndex)
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;

	if(FAILED(m_pEffect->SetVectorArray( hD3DXHandle,  (D3DXVECTOR4*)vecs , nVector ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const char* Name , const  D3DXMATRIX& mat)
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;
	if(FAILED(m_pEffect->SetMatrix( hD3DXHandle,  (D3DXMATRIX*)&mat ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setParamValue(const char* Name , const  D3DXMATRIX* mats, int nMat, int baseIndex)
{
	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;

	hD3DXHandle = m_pEffect->GetParameterElement(hD3DXHandle,baseIndex);
	if(hD3DXHandle == NULL) 
		return false;

	if(FAILED(m_pEffect->SetMatrixArray( hD3DXHandle, (D3DXMATRIX*)mats , nMat  ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setTexture(const char* Name                , IDirect3DTexture9* pTexture)
{

	D3DXHANDLE hD3DXHandle = m_pEffect->GetParameterByName(NULL,Name);
	if(hD3DXHandle == NULL)
		return false;

	if(FAILED(m_pEffect->SetTexture(hD3DXHandle, pTexture ) ) )
	{
		return false;
	}
	return true;
}

bool CFxShader::setTexture(const HShaderParamater&  hParam , IDirect3DTexture9* pTexture)
{
	const CShaderParam* pParamD3D = (const CShaderParam*)(&hParam);
	if(pParamD3D == NULL)
		return false;
	D3DXHANDLE hD3DXHandle = (D3DXHANDLE)pParamD3D->m_hHandle;

	HRESULT hRes = m_pEffect->SetTexture(hD3DXHandle, pTexture );

	if(FAILED(hRes) )
	{
		return false;
	}
	return true;
}

bool  CFxShader::reLoad()
{
	if(m_LoadInfo.m_hInstance && m_LoadInfo.m_ResID)
		return load(m_LoadInfo.m_hInstance , m_LoadInfo.m_ResID);
	else if(m_LoadInfo.m_FileName.length() > 0)
		return load(m_LoadInfo.m_FileName.c_str() );

	return false;
}

bool  CFxShader::load(HINSTANCE hInst , UINT _ResourceID)
{
    HRSRC  hRsrc = ::FindResourceW(hInst, MAKEINTRESOURCEW(_ResourceID) ,  L"FX");
    if(hRsrc == NULL)
    {
        hRsrc = ::FindResourceW(hInst, MAKEINTRESOURCEW(_ResourceID) ,  L"RCDATA");
    }

    ID3DXBuffer *errorBuffer = NULL;
    HRESULT mResult = S_OK;
    if (hRsrc == NULL)
    {
        mResult = D3DXCreateEffectFromResourceA(m_DeviceWrapper->m_pDevice , hInst , MAKEINTRESOURCEA(_ResourceID) , NULL , NULL , 0 , NULL , &m_pEffect , & errorBuffer );
    }
    else
    {
        HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
        if (hGlobal == NULL)
            return false;

        int _len  =  SizeofResource(hInst , hRsrc);
        char* pData =  (char*)LockResource(hGlobal);

        mResult = _loadShader((BYTE*)pData , errorBuffer);
        UnlockResource(hGlobal);
        FreeResource(hGlobal);
    }

    // If an error occurred								   
    if(mResult != D3D_OK)
    {
        // Get the error string
        const char *str = NULL;

        if(errorBuffer)
        { 
            str = (const char*)errorBuffer->GetBufferPointer();
        }
        else
        {
            str = "Error loading effect resource";
        }
        OutputDebugStringA(str);
        OutputDebugStringA("\n");
        return false;
    }

	m_LoadInfo.m_FileBuffer = "";
	m_LoadInfo.m_FileName = L"";
	m_LoadInfo.m_hInstance = hInst;
	m_LoadInfo.m_ResID = _ResourceID;
    //加载参数表
    __buildParamTalbe();
    return true;
}

HRESULT  CFxShader::_loadShader(BYTE* fBuf , ID3DXBuffer *&errorBuffer)
{
    std::string _code = (char*)fBuf;
    if(_code.find("_NO_DEFAULT_TEXTURE_") != std::string::npos && m_IncludeCode.find("_NO_DEFAULT_TEXTURE_") != std::string::npos )
    {
         _code = m_IncludeCode + _code;
         return D3DXCreateEffect(m_DeviceWrapper->m_pDevice , (LPVOID)_code.c_str() , _code.length() ,  NULL , NULL , 0 , NULL , &m_pEffect , & errorBuffer );
    }
    else
    {
        return D3DXCreateEffect(m_DeviceWrapper->m_pDevice , (LPVOID)_code.c_str() , _code.length() ,  NULL , NULL , 0 , NULL , &m_pEffect , & errorBuffer );
    }
}
void CFxShader::cloneLoadInfoFrom(CFxShader* pFrom)
{
    m_LoadInfo = pFrom->m_LoadInfo;
}
//创建删除等操作
bool CFxShader::load(const wchar_t* file_name)
{
	ID3DXBuffer *errorBuffer = NULL; // A buffer for an error string if our effect file fails
	// to load
    std::wstring _filename = file_name;
	if(_filename.find(L".fx") == std::wstring::npos )
       _filename += L".fx";
	file_name = _filename.c_str();

	assert(m_DeviceWrapper);
	// Load the effect file and check for errors
    FILE* fp = _wfopen(file_name , L"rb");
    if(fp == NULL)
    {
        OutputDebugStringA("Shader file ");
        OutputDebugStringW(file_name);
        OutputDebugStringA(" not found\n");
        return false;
    }
    fseek(fp , 0 , SEEK_END);
    int len = ftell(fp);
    fseek(fp , 0 , SEEK_SET);
    BYTE* fBuf = new BYTE[len];
    fread(fBuf , 1 , len , fp);
    fclose(fp);
    
    HRESULT mResult = _loadShader(fBuf , errorBuffer);

	// If an error occurred								   
	if(mResult != D3D_OK)
	{
		// Get the error string
		const char *str = NULL;

		if(errorBuffer)
			str = (const char*)errorBuffer->GetBufferPointer();
		else
			str = "Error loading effect file : ";

        {
            OutputDebugStringA(str);
			OutputDebugStringW(file_name);
            OutputDebugStringA("\n");
        }

		//devLog(str);
		// Print it to the screen
		//MessageBox(hwnd, str, "Shader Compilation Error", MB_OK | MB_ICONERROR);
		return false;
	}

	m_LoadInfo.m_FileBuffer = "";
	m_LoadInfo.m_FileName = file_name;
	m_LoadInfo.m_hInstance = NULL;
	m_LoadInfo.m_ResID = 0;
	OutputDebugStringA("Load fx ok: ");
	OutputDebugStringW(file_name);
	OutputDebugStringA("\n");
	//加载参数表
	__buildParamTalbe();
    return true;
}



//渲染操作
bool CFxShader::begin(size_t *pPasses, unsigned long Flags)
{
	m_pEffect->Begin(pPasses,Flags);
    return true;
}

bool CFxShader::beginPass(size_t Pass)
{
	m_pEffect->BeginPass((UINT)Pass);
    return true;
}

bool CFxShader::commitChanges()
{
	m_pEffect->CommitChanges();
    return true;
}

bool CFxShader::endPass()
{
	m_pEffect->EndPass();
    return true;
}

bool CFxShader::end()
{
	m_pEffect->End();
    return true;
}

bool  CFxShader::HasGpuSkin()
{
	if(m_pEffect == NULL)
		return false;
    D3DXHANDLE hBoneFrame = m_pEffect->GetParameterByName(NULL , "BoneFrame");
    return hBoneFrame != NULL;
}

void CFxShader::bind()
{
	if(m_pEffect == NULL && reLoad() == false)
		return ;

    HRESULT hResult = S_OK;


    //骨骼动画
    D3DXHANDLE hBoneFrame = m_pEffect->GetParameterByName(NULL , "BoneFrame");
    if(hBoneFrame) 
        hResult = m_pEffect->SetValue(hBoneFrame , m_DeviceWrapper->m_BoneFrames , CA3DDeviceWrapper::MAX_BONE * 3 * sizeof(D3DXVECTOR4) );

    
    //变换矩阵
    D3DXHANDLE hTransBuffer = m_pEffect->GetParameterByName( NULL , "TransformBuffer");
    if(hTransBuffer) 
        hResult = m_pEffect->SetValue(hTransBuffer , m_DeviceWrapper->m_TransformBuffer.data() , m_DeviceWrapper->m_TransformBuffer.dataLen() );
   
    
    //灯光信息
    D3DXHANDLE hLightState  = m_pEffect->GetParameterByName( NULL , "xLightState");
    if(hLightState) 
        hResult = m_pEffect->SetValue(hLightState , m_DeviceWrapper->m_LightState.data() , m_DeviceWrapper->m_LightState.dataLen() );

    //float4x4 matWorld;
    //float4x4 matView;
    //float4x4 matProject;

    //////////////////////////////////////////////////////////////////////////
    {
        D3DXHANDLE hMatrix = m_pEffect->GetParameterByName( NULL , "matWorld");
        if(hMatrix) 
            hResult = m_pEffect->SetMatrix(hMatrix , &(m_DeviceWrapper->m_TransformBuffer.m_matWorld) );
    }

    //////////////////////////////////////////////////////////////////////////
    {
        D3DXHANDLE hMatrix = m_pEffect->GetParameterByName( NULL , "matView");
        if(hMatrix) 
            hResult = m_pEffect->SetMatrix(hMatrix , &(m_DeviceWrapper->m_TransformBuffer.m_matView) );
    }

    //////////////////////////////////////////////////////////////////////////
    {
        D3DXHANDLE hMatrix = m_pEffect->GetParameterByName( NULL , "matProject");
        if(hMatrix) 
            hResult = m_pEffect->SetMatrix(hMatrix , &(m_DeviceWrapper->m_TransformBuffer.m_matProject) );
    }
    
    //////////////////////////////////////////////////////////////////////////
    {
        D3DXHANDLE hAlphaRef = m_pEffect->GetParameterByName( NULL , "AlphaRef");
        if(hAlphaRef) 
        {
            DWORD  AlphaRef = 0;
            m_DeviceWrapper->m_pDevice->GetRenderState(D3DRS_ALPHAREF , &AlphaRef);
            hResult = m_pEffect->SetFloat(hAlphaRef , AlphaRef/255.0f ) ;
        }
    }

    commitChanges();
    return ;
}

void CFxShader::unbind()
{
    return ;
}


bool CFxShader::setTechnique(const char * technique)
{
     if(FAILED(m_pEffect->SetTechnique(technique)  ) )
		 return false;
	 return true;
}

bool CFxShader::unload()
{
	m_pEffect->Release();
	m_pEffect = NULL;
	m_ParamTable.release();
	m_AutoSetParamTable.release();
	return true;
}


CShaderParamTable&  CFxShader::getAutoSetParamTable()
{
	return m_AutoSetParamTable;
}

CShaderParamTable& CFxShader::getParamTable()
{
	return m_ParamTable;
}

D3DXHANDLE CFxShader::__getD3DXHandle(const char* pConstName)
{
	return m_pEffect->GetParameterByName(NULL,pConstName);
}


eShaderParamClass CFxShader::__parseParamClass(D3DXPARAMETER_DESC& param_desc)
{
	switch(param_desc.Class)
	{
		//提取矩阵
	case D3DXPC_MATRIX_COLUMNS:
	case D3DXPC_MATRIX_ROWS:
		{
			if(param_desc.Type != D3DXPT_FLOAT)
				return eSPClass_NotSupported;
			if(param_desc.Columns == 4 && param_desc.Rows == 4)
			{
				return eSPClass_MATRIX4x4;
			}
			if(param_desc.Columns == 4 && param_desc.Rows == 3)
			{
				return eSPClass_MATRIX4x3;
			}
			if(param_desc.Columns == 3 && param_desc.Rows == 3)
			{
				return eSPClass_MATRIX3x3;
			}
			return eSPClass_NotSupported;
		}
		break;

		//提取向量
	case D3DXPC_SCALAR:
		switch(param_desc.Type)
		{
		case D3DXPT_BOOL:
			return eSPClass_BOOL ;
			break;
		case D3DXPT_INT:
			return eSPClass_INT ;
			break;
		case D3DXPT_FLOAT:
			return eSPClass_FLOAT ;
			break;
		default:
			return eSPClass_NotSupported;
		}

		//提取向量
	case D3DXPC_VECTOR:
		switch(param_desc.Type)
		{
		case D3DXPT_BOOL:
			return (eShaderParamClass) ((int)eSPClass_BOOL + (param_desc.Columns - 1)  );
			break;
		case D3DXPT_INT:
			return (eShaderParamClass) ((int)eSPClass_INT + (param_desc.Columns - 1)   );
			break;
		case D3DXPT_FLOAT:
			return (eShaderParamClass) ((int)eSPClass_FLOAT + (param_desc.Columns - 1) );
			break;
		default:
			return eSPClass_NotSupported;
		}
	case D3DXPC_OBJECT:
		{
			switch(param_desc.Type)
			{
			case D3DXPT_TEXTURE:
				return eSPClass_TEXTURE;
				break;
			case D3DXPT_TEXTURE1D:
				return eSPClass_TEXTURE1D;
				break;
			case D3DXPT_TEXTURE2D:
				return eSPClass_TEXTURE2D;
				break;
			case D3DXPT_TEXTURE3D:
				return eSPClass_TEXTURE3D;
				break;
			case D3DXPT_TEXTURECUBE:
				return eSPClass_TEXTURECUBE;
				break;

			case D3DXPT_SAMPLER:
				return eSPClass_SAMPLER;
				break;
			case D3DXPT_SAMPLER1D:
				return eSPClass_SAMPLER1D;
				break;
			case D3DXPT_SAMPLER2D:
				return eSPClass_SAMPLER2D;
				break;
			case D3DXPT_SAMPLER3D:
				return eSPClass_SAMPLER3D;
				break;
			case D3DXPT_SAMPLERCUBE:
				return eSPClass_SAMPLERCUBE;
				break;
			default:
				return eSPClass_NotSupported;
			}
		}
		break;
	}
	return eSPClass_NotSupported;
}

int CFxShader::__buildParamTalbe()
{
	D3DXEFFECT_DESC dxeff_desc;
	m_pEffect->GetDesc(&dxeff_desc);


	int nParam = dxeff_desc.Parameters;
	int nTech  = dxeff_desc.Techniques;


	//提取所有的Technique
	for(int iTech = 0 ; iTech < nTech ; iTech ++)
	{
		D3DXHANDLE  hTech = m_pEffect->GetTechnique(iTech);
		D3DXTECHNIQUE_DESC t_desc;
		m_pEffect->GetTechniqueDesc(hTech,&t_desc);
	}

	//提取所有的Paramater
	for(int i = 0 ; i < nParam ; i ++)
	{
		D3DXHANDLE  hParam = m_pEffect->GetParameter(NULL,i);
		D3DXPARAMETER_DESC  p_desc;
		m_pEffect->GetParameterDesc(hParam,&p_desc);

		CShaderParam d3dShaderParam;
		d3dShaderParam.m_ParamSemantic = parseSemantic(p_desc.Semantic);
		d3dShaderParam.m_Name = xStringHash(p_desc.Name);
		strncpy(d3dShaderParam.m_strName,p_desc.Name,32);
		d3dShaderParam.m_ParamClass = __parseParamClass(p_desc);
		d3dShaderParam.m_hHandle = (void*)hParam;
		d3dShaderParam.m_nElement = p_desc.Elements==0?1:p_desc.Elements;
		d3dShaderParam.m_DataBytes = p_desc.Bytes;
		d3dShaderParam._createParamData();

		m_pEffect->GetValue(hParam,d3dShaderParam._getParamData(),d3dShaderParam.m_DataBytes);
		if( d3dShaderParam.m_ParamClass <= eSPClass_SAMPLERCUBE && 
			d3dShaderParam.m_ParamClass >= eSPClass_TEXTURE)
		{
			d3dShaderParam.m_DataBytes = sizeof(HShaderTextureParam);
		}

		if(d3dShaderParam._semantic() != SPS_NOT_USED)
		{
			m_AutoSetParamTable.m_Paramaters.push_back(d3dShaderParam);
		}
		else
		{
			d3dShaderParam.m_ParamSemantic = parseEditorSemantic(p_desc.Semantic);
			m_ParamTable.m_Paramaters.push_back(d3dShaderParam);
		}
	}
	m_AutoSetParamTable.m_nParamater = (int)m_AutoSetParamTable.m_Paramaters.size();
	m_ParamTable.m_nParamater        = (int)m_ParamTable.m_Paramaters.size();

	return m_ParamTable.numOfParamater() + m_AutoSetParamTable.numOfParamater();
}


