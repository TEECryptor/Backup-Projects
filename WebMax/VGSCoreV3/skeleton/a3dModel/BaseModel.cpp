#include "../xcomdoc/xcomdoc.h"
#include "CoreMesh.h"
#include "CoreSkeleton.h"
#include "BaseModel.h"
#include "../A3DHelperFunction.h"
#include "../xcomdoc/xI18N.h"





HRESULT STDMETHODCALLTYPE CRefCountObject::QueryInterface(  REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)
{
	return E_NOTIMPL;
}
ULONG   STDMETHODCALLTYPE CRefCountObject::AddRef( void)
{
	m_RefCount ++;
	return m_RefCount;
}
ULONG   STDMETHODCALLTYPE CRefCountObject::Release( void)
{
	int refCount = m_RefCount;
	m_RefCount--;
	refCount -- ;
	if(refCount == 0)
		delete this;
	return refCount;
}


CBaseModel::CBaseModel(CA3DDeviceWrapper* pDevice , bool bSysMemCopy)
:m_TexMgr(L"ModelTextureMgr")
{
	m_pSkeleton  = NULL;
	m_iAction    = 0;
	m_iActTime   = 0;
    m_TexMgr.SetDevice(pDevice);
    m_DeviceWrapper = pDevice;

}

bool CBaseModel::load(const wchar_t* fileName , unsigned long  arg)
{
	return load(fileName , NULL , 0 , arg);
}

const wchar_t* CBaseModel::name()
{
    return m_name.c_str();
}

void CBaseModel::setName(const wchar_t* _name)
{
    m_name = _name;
}

bool CBaseModel::isLoaded()
{
	return m_Meshs.size() > 0;
}

bool CBaseModel::unload()
{
	size_t _nMesh = m_Meshs.size() ;
	for(size_t iMesh = 0 ; iMesh < _nMesh ; iMesh ++)
	{
		XSAFE_DELETE( m_Meshs[iMesh] );
	}
	m_Meshs.clear();
    
	size_t _nAction =  m_Actions.size();
	for(size_t iAction = 0 ; iAction < _nAction ; iAction ++)
	{
		m_Actions[iAction]->unload();
		delete m_Actions[iAction];
	}
	m_Actions.clear();
	m_ActionFrame.unload();


	XSAFE_DELETE(m_pSkeleton);
	m_aabb.m_Max = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_aabb.m_Min = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_name = L"";
	return true;
}

unsigned long CBaseModel::memUsage()
{
	return 1;
}



bool CBaseModel::attachMesh(CCoreMesh* pMesh)
{
	if(pMesh == NULL)
		return false;

     m_Meshs.push_back(pMesh);
	 return true;
}

bool CBaseModel::detachMesh(CCoreMesh* pMesh)
{
	//É¾³ýMesh
	vCoreMeshs::iterator pos2 = m_Meshs.begin();
	for( ; pos2 != m_Meshs.end() ; pos2 ++)
	{
		if(*pos2 == pMesh)
		{
			m_Meshs.erase(pos2);
			return true;
		}
	}
	return false;
}

bool CBaseModel::setAction(size_t idx)
{
	 if(idx >= m_Actions.size() )
		 return false;

	 m_iActTime = 0;
     m_iAction  = idx;
	 return true;
}

CBaseAction*  CBaseModel::coreAction(size_t idx )
{
	return m_Actions[idx];
}

size_t CBaseModel::nAction()
{
    return m_Actions.size();
}

CCoreActionFrame*  CBaseModel::createActionFrame()
{
	if(m_pSkeleton)
	{
		CCoreActionFrame* pActFrame = new CCoreActionFrame;
		pActFrame->load(m_pSkeleton->nBone() );
        return pActFrame;
	}
	return NULL;
}

void  CBaseModel::destoryActionFrame(CCoreActionFrame* pAcFrame)
{
	delete pAcFrame;
}

bool  CBaseModel::updateFrame(CCoreActionFrame* pAcFrame, int actIdx , unsigned long actTime)
{
	if(m_pSkeleton)
	{
		m_Actions[actIdx]->blend( (long)actTime,pAcFrame,m_pSkeleton);
		return true;
	}
	return false;
}

bool CBaseModel::updateFrame(unsigned long passedTime)
{
	m_iActTime += passedTime;
	if(m_pSkeleton)
	{
		m_Actions[m_iAction]->blend( (long)m_iActTime, &m_ActionFrame,m_pSkeleton);
		return true;
	}
	return true;
}

bool CBaseModel::setupAnimationTime(int actIdx , float actTime)
{
    if(actIdx >= (int)m_Actions.size() )
		return false;
	if(m_pSkeleton)
		return true;

	CBaseAction* _action = m_Actions[actIdx];
    const CActionInfo* pActInfo = _action->info();
	float frameTime = pActInfo->m_lTime /(float) pActInfo->m_nFrame;
	int frame = (int)(actTime/frameTime);
	frame = frame % pActInfo->m_nFrame;

	size_t _nMeshs = m_Meshs.size();
	for(size_t i = 0 ; i <  _nMeshs; i ++)
	{
		m_Meshs[i]->setCurrentFrame(frame);
	}

	return true;
}

bool  CBaseModel::createBoneFrameBuffer()
{


    if(m_pSkeleton)
    {
        m_ActionFrame.load(m_pSkeleton->nBone() );
    }	
    return true;
}

CCoreMesh* CBaseModel::getMesh(size_t idx)
{
	size_t nMeshPtr =  m_Meshs.size();
	if(idx < nMeshPtr )
		return m_Meshs[idx];
	return NULL;
}

size_t  CBaseModel::nMesh()
{
	size_t nMeshPtr    = m_Meshs.size();
	return nMeshPtr;
}

bool  CBaseModel::drawMeshs(CCoreActionFrame* pActionFrame)
{
    if(pActionFrame == NULL) pActionFrame = &m_ActionFrame;
	size_t _nMeshs = m_Meshs.size();
	for(size_t i = 0 ; i <  _nMeshs; i ++)
	{
		m_Meshs[i]->draw(m_pSkeleton , pActionFrame , false);
	}

	return true;
}

bool CBaseModel::setupAnimation(CCoreActionFrame* pAcFrame)
{
    if(m_pSkeleton && pAcFrame)
    {
        //void* pData = NULL;
        //memcpy(pData , pAcFrame->data() , sizeof(D3DXMATRIX) * pAcFrame->nBone() );
        return true;
    }
    return false;
}

bool CBaseModel::setupAnimation()
{
    return setupAnimation(&m_ActionFrame);
}

bool  CBaseModel::draw(CCoreActionFrame* pActionFrame)
{
    setupAnimation();
	return drawMeshs(pActionFrame);
}

CBoundBox&  CBaseModel::boundBox()
{
	return m_aabb;
}

bool  CBaseModel::HasGpuSkin()
{
    if(m_pSkeleton == NULL)
        return false;
    return m_DeviceWrapper->HasGpuSkin( m_pSkeleton->nBone() );
}


