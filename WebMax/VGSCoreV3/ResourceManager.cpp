#include "resourceManager.h"
#include "CSysFunc.h"

CResourceManager::CResourceManager()
{

}
CResourceManager::~CResourceManager()
{
	Release();
}

VOID CResourceManager::Release()
{
	MAP_RESOURSE::iterator it = m_vpResource.begin();
	while (it != m_vpResource.end())
	{
		if (it->second)
		{
			SAFE_RELEASE(it->second);
			SAFE_DELETE(it->second);
		}
		++it;
	}
	
	m_vpResource.clear();
}

VOID CResourceManager::addResource(CResource *pRes)
{
	if (pRes && !pRes->m_sName.empty())
	{
		m_vpResource[CVGSString(pRes->m_sName)] = pRes;
	}
}
VOID CResourceManager::removeResource(CResource *pRes)
{
	if (pRes)
	{
		removeResource(pRes->m_sName);
	}
	// 	UINT count = m_vpResource.size();
	// 	for (UINT i = 0; i < count; ++i)
	// 	{
	// 		if (m_vpResource[i] && m_vpResource[i] == pRes)
// 		{
// 			SAFE_RELEASE(m_vpResource[i]);
// 			SAFE_DELETE(m_vpResource[i]);
// 			m_vpResource.erase(m_vpResource.begin() + i);
// 			break;
// 		}
// 	}
}

// 如果资源还在被使用，就不会被remove掉
VOID CResourceManager::removeResource(const std::string &sName)
{
	MAP_RESOURSE::iterator it = m_vpResource.find(CVGSString(sName));
	if (it != m_vpResource.end())
	{
		it->second->Release();
		if (!it->second->IsInUsed())
		{
			SAFE_DELETE(it->second);
			m_vpResource.erase(it);
		}
	}


// 	if (index >= 0 && index < m_vpResource.size())
// 	{
// 		SAFE_RELEASE(m_vpResource[index]);
// 		SAFE_DELETE(m_vpResource[index]);
// 		m_vpResource.erase(m_vpResource.begin() + index);
// 	}
}

CResource* CResourceManager::getResource(size_t index)
{
	if (index >= 0 && index < m_vpResource.size())
	{
		MAP_RESOURSE::iterator it = m_vpResource.begin();
		for (size_t i = 0; i < index; ++i)
		{
			++it;
		}
		return it->second;
	}
	return 0;
}

CResource* CResourceManager::getResource(const std::string &sName)
{
	MAP_RESOURSE::iterator it = m_vpResource.find(CVGSString(sName));
	if (it != m_vpResource.end())
	{
		return it->second;
	}
 	return 0;


// 	UINT count = m_vpResource.size();
// 	for (UINT i = 0; i < count; ++i)
// 	{
// 		if (m_vpResource[i] && m_vpResource[i]->m_sName.compare(sName) == 0)
// 		{
// 			return m_vpResource[i];
// 		}
// 	}
// 	return 0;
}
UINT CResourceManager::getResourceCount()
{
	return m_vpResource.size();
}

bool CResourceManager::IsResourceExist(const std::string &sName)
{
	return (getResource(sName) != NULL);
}