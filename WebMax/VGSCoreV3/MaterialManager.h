
#pragma once

#include "VGSHeader.h"
#include "ResourceManager.h"
#include "VGSGetRoot.h"

class VGS_EXPORT CMaterialManager : public CResourceManager// , public CVGSGetRoot, public Singleton<CMaterialManager>
{
public:
	CMaterialManager();
	~CMaterialManager();
	VOID Release();

	/** 创建材质
	@param sName in : 要创建的材质名称
	*/
	CResource* createMaterial(const std::string &sName);


	// 生成一个新的材质名称
	std::string GenerateGuiMtrlName(const char* szGuiName);
};