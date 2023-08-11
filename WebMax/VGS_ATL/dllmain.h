// dllmain.h : 模块类的声明。

class CVGSATL2Module : public CAtlDllModuleT< CVGSATL2Module >
{
public :
	DECLARE_LIBID(LIBID_VGSATL2Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_VGSATL2, "{149D3F1B-BE09-48A1-96E8-8EB72F30476E}")
};

extern class CVGSATL2Module _AtlModule;
