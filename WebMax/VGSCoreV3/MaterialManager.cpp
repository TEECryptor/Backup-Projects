#include "MaterialManager.h"
#include "CMaterial.h"
#include <sstream>

//template <> CMaterialManager* Singleton<CMaterialManager>::ms_Singleton = 0;

CMaterialManager::CMaterialManager()
{

}
CMaterialManager::~CMaterialManager()
{
	Release();
}
VOID CMaterialManager::Release()
{

}

CResource* CMaterialManager::createMaterial(const std::string &sName)
{
	if (IsResourceExist(sName))
	{
		return 0;
	}
	CMaterial *pMaterial = new CMaterial();
	if (pMaterial)
	{
		pMaterial->SetRoot(m_pRoot);
		pMaterial->setName(sName);
		m_vpResource[sName] =  pMaterial;
		// m_vpResource.push_back(pMaterial);
	}
	return pMaterial;
}

// 生成一个新的材质名称
std::string CMaterialManager::GenerateGuiMtrlName(const char* szGuiName)
{
	std::string strMtrl = szGuiName;
	strMtrl += "_mtrl";
	int idx = 0;
	std::ostringstream os;
	os<<idx;
	while (IsResourceExist(strMtrl + os.str()))
	{
		os.clear();
		idx++;
		os<<idx;
	}
	return strMtrl;
}