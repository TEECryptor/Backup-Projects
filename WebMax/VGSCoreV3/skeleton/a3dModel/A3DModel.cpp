#include "../xcomdoc/xcomdoc.h"
#include "A3DModel.h"
#include "CoreMesh.h"
#include "CoreSkeleton.h"
#include "BaseModel.h"




CA3DModel::CA3DModel(CA3DDeviceWrapper* pDevice ,  bool bSysMemCopy)
:CBaseModel(pDevice , bSysMemCopy)
{
}

CA3DModel::~CA3DModel()
{

}
bool CA3DModel::addPackage(const wchar_t* fileName , const BYTE* buf , size_t bufLen)
{
    return true;
}
//====================
bool CA3DModel::load(const wchar_t* fileName , const BYTE* buf , size_t bufLen, unsigned long arg)
{
	xcomdoc doc;
	bool bRet = true;
	if(buf == NULL || bufLen == 0)
	{
		bRet = doc.open(fileName,  xcdm_read);
	}
	else
	{
		bRet = doc.open( (const _xcd_int8*)buf, (size_t)bufLen,  xcdm_read);
	}

	//如果不是打包的。
	if( bRet == false )
	{
		return false;
	}

	xcomdocstream* pDesc = doc.open_stream(L"desc.xml");
	if(pDesc != NULL)
	{
		bRet = load(doc , L"" , arg);
		doc.close_stream(pDesc);
	}
	return bRet;
}

bool CA3DModel::_loadModelMeshMateril( CCoreMesh* mesh , xXmlNode* pMeshNode)
{
	return false;
}

void ReadXml( xcomdocstream* pdescstream, xXmlDocument &xml );

bool CA3DModel::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
	bool bLoadAllMesh = arg != 0;
	//读入文件头
	wstring descName = wstring(_dir) + L"desc.xml";
	xcomdocstream* pdescstream = doc.open_stream(descName.c_str() );
	xXmlDocument xml;
    ReadXml(pdescstream , xml);
	doc.close_stream(pdescstream);

    _addFile(doc);
	//XML加载完毕
	xXmlNode* pRootNode = xml.root();

	m_name = pRootNode->value(L"name");
	xXmlNode* pAABBNode = pRootNode->findNode(L"BoundBox");
	m_aabb.m_Min = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
	m_aabb.m_Max = D3DXVECTOR3(1.0f , 1.0f , 1.0f);
	if(pAABBNode)
	{
		m_aabb.m_Min.x = pAABBNode->float_value(L"min_x");
		m_aabb.m_Min.y = pAABBNode->float_value(L"min_y");
		m_aabb.m_Min.z = pAABBNode->float_value(L"min_z");

		m_aabb.m_Max.x = pAABBNode->float_value(L"max_x");
		m_aabb.m_Max.y = pAABBNode->float_value(L"max_y");
		m_aabb.m_Max.z = pAABBNode->float_value(L"max_z");
	}

	//开始加载Skeleton
	loadSkeleton(doc , _dir);
    if(m_pSkeleton != NULL)
	{
		loadAction(doc , _dir);
	}

    createBoneFrameBuffer();
	if(bLoadAllMesh)
	{
		//开始加载Mesh
		loadEmbMeshs(doc , _dir , pRootNode);

		//加载SkinGroup
		loadEmbSkinGroup(doc , _dir , pRootNode); 
	}


	return true;
}

void CA3DModel::_addFile(xcomdoc& doc )
{
    //增加一个doc;
    m_FileLists.push_back(doc);
    m_TexMgr.addPackage(doc , L"texture/");
}

void CA3DModel::_setupMesh( CCoreMesh* pMesh, xXmlNode*pMeshNode ) 
{
	_loadModelMeshMateril(pMesh,pMeshNode);
	m_Meshs.push_back(pMesh);
}

bool CA3DModel::loadAction(xcomdoc& doc ,const wchar_t* _dir)
{
	wstring pActionDescName = wstring(_dir) + L"actions.xml";
	xcomdocstream* pActDescStream = doc.open_stream(pActionDescName.c_str() );
	if(pActDescStream != NULL)
	{
		xXmlDocument xml;
        ReadXml(pActDescStream , xml);
		xXmlNode* pRootNode = xml.root();
		xXmlNode::XmlNodes actNodes;
		pRootNode->findNode(actNodes);

		size_t _ActNodes = actNodes.size() ;
		for(size_t  i = 0 ;i < _ActNodes ; i ++)
		{
            xXmlNode* pNode = actNodes[i];
			CBaseAction* pAction = CBaseAction::createInstance( (eActionType)pNode->int_value(L"ActionType") , m_pSkeleton , 0 );
			pAction->load(actNodes[i]);
			m_Actions.push_back(pAction);
		}   

		doc.close_stream(pActDescStream);
		return true;
		//加载内置的动作
	}
	return false;
}

bool CA3DModel::loadSkeleton(xcomdoc& doc , const wchar_t* _dir)
{
	m_pSkeleton = NULL;
	wstring skeletonName = wstring(_dir) + L"skeleton/skeleton";
	xcomdocstream* pSkelStream = doc.open_stream(skeletonName.c_str() );
	if(pSkelStream != NULL)
	{
		m_pSkeleton = new CCoreSkeleton;
		if( false == m_pSkeleton->load(doc , _dir ) )
		{
			delete m_pSkeleton;
			m_pSkeleton = NULL;
		}
		doc.close_stream(pSkelStream);
		//加载内置的动作
	}

	wstring pActionDescName = wstring(_dir) + L"skeleton/actions.xml";
	xcomdocstream* pActDescStream = doc.open_stream(pActionDescName.c_str() );
	if(pActDescStream != NULL)
	{
        xXmlDocument xml;
        ReadXml(pActDescStream , xml);

		wstring actDir = wstring(_dir) + L"skeleton/";
		xXmlNode* pRootNode = xml.root();
		xXmlNode::XmlNodes actNodes;
		pRootNode->findNode(actNodes);

		size_t _ActNodes = actNodes.size() ;
		for(size_t  i = 0 ;i < _ActNodes ; i ++)
		{
            xXmlNode* pNode = actNodes[i];
            CBaseAction* pAction = CBaseAction::createInstance( (eActionType)pNode->int_value(L"ActionType") , m_pSkeleton , 0 );
			if(false == pAction->load(pNode , doc , actDir.c_str()) )
			{
				delete pAction;
			}
			else
			{
				m_Actions.push_back(pAction);
			}
		}   

		doc.close_stream(pActDescStream);
		//加载内置的动作
	}
	return true;
}

bool CA3DModel::loadEmbMeshs(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode)
{
	xXmlNode::XmlNodes meshNodes;
	pRootNode->findNode(L"mesh" , meshNodes);
	size_t _MeshNodes = meshNodes.size() ;
	for(size_t i = 0 ; i <  _MeshNodes ; i ++)
	{
		xXmlNode* pMeshNode = meshNodes[i];
		const wchar_t* name = pMeshNode->value(L"name");
		CCoreMesh* pMesh = new CCoreMesh(&m_TexMgr , this );
		wstring meshName = wstring(_dir) + L"/" + name + L"/";
		if( pMesh->load(doc , meshName.c_str(), 0) == false)
		{
			delete pMesh;
			continue;
		}
		_setupMesh(pMesh, pMeshNode);

	}
	return true;
}

bool CA3DModel::loadEmbSkinGroup(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode)
{
	xXmlNode::XmlNodes skinGroupNodes;
	pRootNode->findNode(L"skinGroup" , skinGroupNodes);

	size_t _nSkinGroupNodes = skinGroupNodes.size() ;
	for(size_t i = 0 ; i < _nSkinGroupNodes ; i ++)
	{
		xXmlNode* pSkinGrouNode = skinGroupNodes[i];
		const wchar_t* skinGroupName = pSkinGrouNode->value(L"name");
		CCoreMesh* pMesh = new CCoreMesh(&m_TexMgr  , this );
		wstring skinGroupDesName = wstring(_dir) + L"/" + skinGroupName + L"/skins.xml";
		xcomdocstream* pdescstream = doc.open_stream(skinGroupDesName.c_str() );
		xXmlDocument xml;
		ReadXml(pdescstream , xml);
		xXmlNode* pRootNode = xml.root();

		xXmlNode::XmlNodes meshNodes;
		pRootNode->findNode(L"mesh" , meshNodes);
		size_t _nMeshNodes = meshNodes.size() ;
		for(size_t i = 0 ; i < _nMeshNodes;  i ++)
		{
			xXmlNode* pMeshNode = meshNodes[i];
			const wchar_t* name = pMeshNode->value(L"name");
			CCoreMesh* pMesh = new CCoreMesh( &m_TexMgr , this );
			wstring meshName = wstring(_dir) + L"/" + skinGroupName + L"/" + name + L"/" ;
			if( pMesh->load(doc , meshName.c_str(), 0) == false)
			{
				delete pMesh;
				continue;
			}
			_setupMesh(pMesh, pMeshNode);
		}

	}
	return true;
}



