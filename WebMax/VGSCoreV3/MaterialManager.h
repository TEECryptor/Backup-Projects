
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

	/** ��������
	@param sName in : Ҫ�����Ĳ�������
	*/
	CResource* createMaterial(const std::string &sName);


	// ����һ���µĲ�������
	std::string GenerateGuiMtrlName(const char* szGuiName);
};