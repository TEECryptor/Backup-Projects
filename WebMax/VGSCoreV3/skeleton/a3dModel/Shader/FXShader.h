#ifndef __Fx_SHADER_D3D_H___
#define __Fx_SHADER_D3D_H___
/*===================================================================
* zeus Game Engine
* This library is a professional game engine
* Thank you for choose Zeus!

* creater : Stanly.Lee
* created : 11-25-2005
* filename: zFXShader.h

* purpose : a wrapper for CgFx(Use OpenGL) or Microsorft .FX(Direct3D)
            and intergrate the Shader System to the Editor and RenderSystem
=======================================================================*/
#include "D3DShader.h"

class CFxShader : public CShaderD3DBase
{
protected:
	ID3DXEffect*             m_pEffect;
	D3DXHANDLE               m_hTechLow;
	D3DXHANDLE               m_hTechBase;
	D3DXHANDLE               m_hTechHigh;

	struct xLoadInfo
	{
		std::wstring m_FileName;
		std::string  m_FileBuffer;
		UINT         m_ResID;
		HINSTANCE    m_hInstance;
	};

	xLoadInfo        m_LoadInfo;
    std::string      m_IncludeCode;
protected:
	eShaderParamClass __parseParamClass(D3DXPARAMETER_DESC& param_desc);
	int               __buildParamTalbe();
public:
	CFxShader(CA3DDeviceWrapper* pDevice);


	D3DXHANDLE          __getD3DXHandle(const char* pConstName);
public:
	int                 addRef(){return 1;}
	void                release(){delete this;}


	CShaderParamTable& getAutoSetParamTable() ;
	CShaderParamTable& getParamTable();

    void               cloneLoadInfoFrom(CFxShader* pFrom);
    void               appendInclude(const char* _code);
	eShaderType        type(){return ShaderType_Effect;}
	bool               setParamValue(const HShaderParamater&  hParam , float  value );
	bool               setParamValue(const HShaderParamater&  hParam , const float* value , int n, int baseIndex = 0);
										   
	bool               setParamValue(const HShaderParamater&  hParam , int    value );
	bool               setParamValue(const HShaderParamater&  hParam , const int*   value , int n, int baseIndex = 0);
										   
										   
	bool                setParamValue(const HShaderParamater&  hParam , bool   value );
	bool                setParamValue(const HShaderParamater&  hParam , const bool*  value , int n, int baseIndex = 0);
										   
	bool                setParamValue(const HShaderParamater&  hParam , const D3DXVECTOR4& vec);
	bool                setParamValue(const HShaderParamater&  hParam , const D3DXVECTOR4* vecs, int nVector, int baseIndex = 0);
										   
										   
	bool                setParamValue(const HShaderParamater&  hParam ,const  D3DXMATRIX& mat);
	bool                setParamValue(const HShaderParamater&  hParam ,const  D3DXMATRIX* mats, int nMat, int baseIndex = 0);


	//按照名字来设置参数
	bool                setParamValue(const char* Name , float    v  );
	bool                setParamValue(const char* Name , const float*   v , int Count, int baseIndex = 0);

	bool                setParamValue(const char* Name , bool     v  );
	bool                setParamValue(const char* Name , const bool*   v , int Count, int baseIndex = 0);

	bool                setParamValue(const char* Name , int      v  );
	bool                setParamValue(const char* Name , const int*     v , int Count, int baseIndex = 0);

	bool                setParamValue(const char* Name , const D3DXVECTOR4& vec);
	bool                setParamValue(const char* Name , const D3DXVECTOR4* vecs, int nVector, int baseIndex = 0);

	bool                setParamValue(const char* Name , const  D3DXMATRIX& mat);
	bool                setParamValue(const char* Name , const  D3DXMATRIX* mats, int nMat, int baseIndex = 0);

	bool                setTexture(const char* Name                , IDirect3DTexture9* pTexture);
	bool                setTexture(const HShaderParamater&  hParam , IDirect3DTexture9* pTexture);

	//创建删除等操作
	bool                load(HINSTANCE , UINT _ResourceID);
	bool                load(const wchar_t* file_name);
	bool                unload();
	bool                isLoaded(){return m_pEffect != NULL;}
	bool                reLoad();

	//渲染操作
	bool                begin(size_t *pPasses, unsigned long Flags);
	bool                beginPass(size_t Pass);
	bool                commitChanges();
	bool                endPass();
	bool                end();
	bool                setTechnique(const char * technique);
    bool                HasGpuSkin();
    void                bind()  ;
    void                unbind()  ;
private:
    HRESULT             _loadShader(BYTE* pBuf , ID3DXBuffer *&errorBuffer);
};



#endif