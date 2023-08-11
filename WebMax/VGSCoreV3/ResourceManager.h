
#pragma once

#include "VGSHeader.h"
#include <map>
//#include "VGSSingelton.h"
#include "VGSString.h"
#include "VGSGetRoot.h"
using namespace std;

typedef map<CVGSString, CResource*> MAP_RESOURSE;

class VGS_EXPORT CResource:public CVGSGetRoot
{
public:
	CResource(){m_refCount = 0;}
	virtual ~CResource()
	{
	}
	virtual VOID Release()
	{
		if (--m_refCount <= 0)
		{
			m_sName = "";
		}
	}

	std::string m_sName;

	bool operator == (const CResource &res)
	{
		return m_sName.compare(res.m_sName) == 0;
	}

	VOID AddRef(){++m_refCount;}  // �������ü���
	bool IsInUsed(){return m_refCount > 0;}

private:
	int m_refCount; // ���ü���
};

class VGS_EXPORT CResourceManager:public CVGSGetRoot
{
public:
	CResourceManager();
	virtual ~CResourceManager();
	virtual VOID Release();

	VOID addResource(CResource *pRes);

	// �����Դ���ڱ�ʹ�ã��Ͳ��ᱻremove��
	VOID removeResource(CResource *pRes);
	VOID removeResource(const std::string &sName);
	// VOID removeResource(INT index);
	CResource *getResource(size_t index);
	CResource *getResource(const std::string &sName);
	UINT getResourceCount();

	bool IsResourceExist(const std::string &sName);

protected:
	MAP_RESOURSE m_vpResource;

};



