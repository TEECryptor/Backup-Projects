/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef __TEMPLATE_PKG_RESOURCE_MGR_H__
#define __TEMPLATE_PKG_RESOURCE_MGR_H__
#include "xResourceMgr.h"
#include "../xcomdoc/xcomdoc.h"
struct xResPkgPathItem
{
    std::wstring   m_ResDir;
    xcomdoc        m_Package;
};

class xResPathManager
{
public:
    void clearAllSearchPath()
    {
        m_Packages.clear();
    }

    xResPathManager(const wchar_t* _name = NULL)
    {
        m_strPathMgrName[0]=0;
        setPathMgrName(_name);		
    }

    void setPathMgrName(const wchar_t* _name)
    {
        if(_name)
        {
            wcscpy(m_strPathMgrName , _name);
        }
    }
    const wchar_t* pathMgrName()
    {
        return m_strPathMgrName;
    }
    ~xResPathManager()
    {
        clearAllSearchPath();
    }

    bool addPackage(xcomdoc& package,const wchar_t* ResDir)
    {
        size_t index = this->m_Packages.size();
        this->m_Packages.push_back(xResPkgPathItem());
        xResPkgPathItem& item = m_Packages[index];
        item.m_Package = package;
        item.m_ResDir  = ResDir;
        return true;
    }

    bool addPackage(const wchar_t* package_file,const wchar_t* ResDir)
    {
        xResPkgPathItem item;
        item.m_Package.open(package_file,xcdm_read,0,false,true);
        if(item.m_Package.failed())
            return false;
        item.m_ResDir  = ResDir;
        this->m_Packages.push_back(item);
        return true;
    }

    bool addPath(const wchar_t* ResDir)
    {
        size_t nPackages = m_Packages.size() ;
        for(size_t i = 0 ; i < nPackages;  i ++)
        {
            if(m_Packages[i].m_Package.failed() == false)
                continue;

            if(m_Packages[i].m_ResDir == ResDir)
                return false;
        }

        xResPkgPathItem item;
        item.m_ResDir  = ResDir;
        this->m_Packages.push_back(item);
        return true;
    }

    xResPkgPathItem& packageItem(int index)
    {
        return m_Packages[index];
    }

    size_t nPackageItem()
    {
        return m_Packages.size();
    }
protected:
    wchar_t      m_strPathMgrName[32];
    typedef std::vector<xResPkgPathItem > ResPkgItems;
    ResPkgItems  m_Packages;

};

//-----------------------------------------------------------------------------------
//一个通用的资源加载器，
//-----------------------------------------------------------------------------------
template <typename ResType  , typename KeyType = std::wstring , typename _TCreator = TSimpleResCreator<ResType> , typename _TBaseLoader = xResPathManager > 
class TResPkgLoader: public _TBaseLoader , public _TCreator
{
protected:
	virtual bool _isResLoaded(ResType* pRes)
	{
		return pRes != NULL;
	}

	virtual bool _preLoadResource(const KeyType& strResName,ResType* & pRes , int& ResSize , unsigned int arg)
	{
		return true ;
	}

	virtual bool _postLoadResource(const KeyType& strResName,ResType* & pRes , int& ResSize , unsigned int arg)
	{
		return true ;
	}

	virtual bool _loadResource(const KeyType& strResName,ResType* & pRes , int& ResSize , unsigned int arg)
	{
		KeyType full_name;
		ResSize = 0;
		bool needDeletePRes = false;
		if(pRes == NULL)
		{
			needDeletePRes = true;
			pRes = this->newInstance(strResName.c_str() , arg );
			if(pRes == NULL)
				return false;
		}
		if(XEvol_IsAbsPath( strResName  ) )
		{
			if(pRes->load(strResName.c_str() , arg) )
			{
				ResSize = pRes->memUsage();
				XEVOL_LOG(eXL_DEBUG_NORMAL,L"Load from abs path name = %s \n",strResName.c_str() );
				return true;
			}
			else
			{
				pRes->unload();
			}
		}
		else
		{
			int nResItem = (int)this->m_Packages.size();
			for(int i = 0 ; i < nResItem ; i++)
			{
				xResPkgPathItem& item = this->m_Packages[i];
				if(item.m_Package.failed())
				{
					full_name = KeyType(item.m_ResDir.c_str() ) + KeyType( strResName.c_str() ) + m_ext;
					if(pRes->load(full_name.c_str() , arg) )
					{
						ResSize = pRes->memUsage();
						XEVOL_LOG(eXL_DEBUG_NORMAL,L"Load from path, search index = %d , name = %s \n",i,strResName.c_str() );
						return true;
					}
					else
					{
						pRes->unload();
					}
				}
				else
				{
					full_name = KeyType(item.m_ResDir.c_str() ) + KeyType( strResName.c_str() ) + m_ext;
					xcomdocstream* pstream = item.m_Package.open_stream( (const wchar_t*)full_name.c_str());
					if(pstream)
					{
						int len = (int)pstream->data_len();
						unsigned int8* buf = new unsigned int8[len + 1];

						//读到内存里来
						int offs = pstream->stream_tellr();
						pstream->stream_seekr(xcdsd_beg,0);
						pstream->read(buf,len);
						pstream->stream_seekr(xcdsd_beg,offs);
						buf[len] = 0;

						if(pRes->load(strResName.c_str(), buf,len  , arg ))
						{
							ResSize = pRes->memUsage();
							delete[] buf;
							XEVOL_LOG(eXL_DEBUG_NORMAL,  L"Load from package, search index = %d , name = %s\n",i,strResName.c_str() );
							pstream->Release();
							return true;
						}
						else
						{
							pRes->unload();
							delete[]  buf;
						}
						pstream->Release();
					}

				}
			}
		}


		//如果是创建出来的就删除掉
		if(needDeletePRes)
		{
			this->deleteInstance(pRes);
			pRes = NULL;
		}

		return false;
	}
	virtual bool _unloadResource(const KeyType& strResName, ResType* & pRes , unsigned int& TotalResSize)
	{
		TotalResSize -= pRes->memUsage();
		pRes->unload();
		this->deleteInstance(pRes);
		pRes = NULL;
		return true;
	}

	virtual void _deleteResource(const KeyType& strResName, ResType* pRes)
	{
		if(pRes == NULL)
			return ;
		pRes->unload();
		this->deleteInstance(pRes);
		pRes = NULL;
	}

public:
	const wchar_t*     ext()
	{

		return m_ext.c_str();
	}

	void setDefExt(const wchar_t* _ext)
	{
		m_ext = _ext;
	}

protected:
	std::wstring   m_ext;
};

//该类不会delete掉pRes指针
template <typename ResType  , typename KeyType = std::ds_wstring, typename _TCreator = TSimpleResCreator<ResType> , typename _TBaseLoader = xResPathManager > 
class TResPkgLoader2: public TResPkgLoader<ResType , KeyType , _TCreator , _TBaseLoader>
{
protected:
	virtual bool       _unloadResource(const KeyType& strResName, ResType* & pRes, unsigned int& TotalResSize)
	{
		TotalResSize -= pRes->memUsage();
		pRes->unload();
		return true;
	}
};


//*******************************************************


#endif

