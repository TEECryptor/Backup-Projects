

#include "../xcomdoc/xcomdoc.h"
#include "../xcomdoc/xI18N.h"
#include "BaseAction.h"
#include "CoreSkeleton.h"
#include "CoreMesh.h"
#include "CoreAction.h"
#include "MixAction.h"
#include "TransitionAction.h"
#include "BaseModel.h"


///动作轨道
int CActionTrack::actionTime()
{ 
	return m_actTime ;
}

int  CActionTrack::hashName()
{ 
	return m_HashName ; 
}

void  CActionTrack::setTimeScale(float _scale) 
{
	m_fTimeScale = _scale;
}

float CActionTrack::timeScale()
{ 
	return m_fTimeScale ;
}

void CActionTrack::setTransitionTime(int _time) 
{
	m_iTransitionTime = _time;
}

 

CBaseAction*  CActionTrack::getCurrentAction()
{
	if(m_CurrentAction.m_pAction  ) 
		return m_CurrentAction.m_pAction ;
	return NULL;
}



bool   CActionTrack::setActionList(vActionList& actions , int iLoopTime , bool bTransit, CBaseAction* pLastAction)
{
	CBaseModel* pModel = m_hModel ;
	if(pModel == NULL)
		return false;

    m_iLoopTime = iLoopTime;
	m_vActions.clear();
	CBaseAction* lastAction  = pLastAction;//actions[0].m_pAction;
	CBaseAction* beginAction = lastAction;

	for(size_t i = 0 ; i < actions.size() ; i ++)
	{
		if(bTransit && lastAction && m_iTransitionTime > 0)
		{
			CTransitionAction* hTransAction = new CTransitionAction(pModel->skeleton() , 0 );
			hTransAction->setAction(lastAction ,  actions[i].m_pAction ,  0.5f);
			hTransAction->setTime(m_iTransitionTime/1000.0f);
			m_vActions.push_back( CTrackActionInfo(hTransAction, m_iTransitionTime/1000.0f ) );
            lastAction = hTransAction;
            if(beginAction == NULL)
            {
                beginAction = hTransAction;
            }
		}

		//加入一个Action
		m_vActions.push_back( actions[i] );
        if(beginAction == NULL)
        {
            beginAction = actions[i].m_pAction;
        }

		lastAction = actions[i].m_pAction;
	}

	//回归
	if( bLoop() )
	{
		if(lastAction && beginAction && m_iTransitionTime > 0)
		{
			CTransitionAction* hTransAction = new CTransitionAction(pModel->skeleton() , 0 );
			hTransAction->setAction(lastAction ,  beginAction ,  0.5f);
			hTransAction->setTime(m_iTransitionTime/1000.0f);
			m_vActions.push_back( CTrackActionInfo(hTransAction, m_iTransitionTime/1000.0f ) );
		}
	}

    m_actTime = 0;
	m_vActionLists = actions;
    doNextAction(0);
	return true;
}

bool CActionTrack::doNextAction(int _time)
{
	CBaseModel* pModel = m_hModel ;
	if(pModel == NULL)
		return false;
	if(m_vActions.size() == 0)
	{
		if(m_iLoopTime > 0 ) 
			m_iLoopTime -- ;
		if(m_iLoopTime != 0   )
		{
			setActionList(m_vActionLists , m_iLoopTime , NULL);
			return m_vActionLists.size() > 0;
		}
		return false;
	}
	m_CurrentAction.setOriginTime();
	CTrackActionInfo trackActionInfo = *m_vActions.begin();
	m_CurrentAction = trackActionInfo;

    int timeRemoved = m_actTime - _time;
	m_actTime = _time;
	m_vActions.pop_front();
	m_CurrentAction.setDesireTime();
    fireEvent(eActionChange , m_iSlot , m_CurrentAction.m_pAction );

    m_actTime = m_TimeCtller.begin();
	return true;

}

void CActionTrack::addListenner(IActionTrackListenner* pListenner)
{
    size_t nL = m_vListenners.size();
    for(size_t i = 0 ; i < nL ; i ++)
    {
        IActionTrackListenner* pL = m_vListenners[i];
        if(pL == pListenner) return ;
    }
    m_vListenners.push_back( pListenner );
}

void CActionTrack::removeListenner(IActionTrackListenner* pListenner)
{
    vActionEventListenners::iterator pos = m_vListenners.begin();
    for( ; pos != m_vListenners.end() ; pos ++)
    {
        if(*pos == pListenner)
        {
            m_vListenners.erase(pos);
            return ;
        }
    }
    return ;
}

void CActionTrack::fireEvent(eActionTrackEvent _event, int iTrackIdx , CBaseAction* pAction)
{
    size_t nL = m_vListenners.size();
    for(size_t i = 0 ; i < nL ; i ++)
    {
        IActionTrackListenner* pL = m_vListenners[i];
        pL->onActionTrackEvent(_event , iTrackIdx , pAction);
    }
}
CActionTrack::CActionTrack(CBaseModel*  hModel , const wchar_t* _name , int iSlot)
{
	m_iLoopTime  = -1;
	m_hModel     = hModel;
	m_name       = _name;
	m_HashName = xStringHash(_name);
	m_actTime = 0;
	m_CurrentAction.setNULL();;
	m_iTransitionTime = 400; //过渡时间为400毫秒
	m_fTimeScale      = 1.0f;
	m_iSlot = iSlot;
    m_TimeCtller.setLoopCtrl(0);
    m_actTime = m_TimeCtller.begin();
}

bool CActionTrack::update(unsigned long passedTime , CCoreSkeleton* pSkeleton)
{
	passedTime = (unsigned long)(m_fTimeScale * passedTime);
	CBaseAction* pAction = getCurrentAction();
	if(pAction && pSkeleton)
	{
        m_actTime = m_TimeCtller.update(m_actTime , passedTime);
		int lefTime = 0;
		const CActionInfo* pActInfo = pAction->info();
		int actionTime = pActInfo->m_lTime;
		if(m_actTime > actionTime)
		{
			lefTime = m_actTime - actionTime;
		}

		if(lefTime > 0 )
		{
			if( false == doNextAction(lefTime) )
			{
				fireEvent(eActionFinish , m_iSlot , m_CurrentAction.m_pAction );
				m_actTime = actionTime;
			}
			else
			{
				//if(m_pListenner) m_pListenner->onActionChange(m_CurrentAction.m_hCoreAction->name() );
			}
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
CBoneArray::CBoneArray()
{
    m_Data = NULL;
}

CBoneArray::~CBoneArray()
{
    free();
}


void CBoneArray::create(int Row , int Col)
{
    int size = Col * Row;
    m_Col = Col ;
    m_Row = Row ;
    m_Data = new CBoneTrans[size];
}

void CBoneArray::create(int* DimSize)
{
    create(DimSize[0],DimSize[1]);
}
#ifndef _DEBUG
CBoneTrans* CBoneArray::operator[](int index)
{
    assert(index < m_Row && index >= 0);
    return &m_Data[index * m_Col];
}
#else
CBoneArray::CBoneArray1D CBoneArray::operator[](int index)
{
    assert(index < m_Row && index >= 0);
    return CBoneArray1D(&m_Data[index * m_Col] , m_Col );
}
#endif
void CBoneArray::free()
{
    if( m_Data)
    {
        delete [] m_Data;
        m_Data = NULL;
    }
}


//骨架的一个Frame
CCoreActionFrame::CCoreActionFrame()
{
    m_nBone = 0;
    m_FrameMat   = NULL;
    m_WSMats     = NULL;
    m_vBoneTrans = NULL;
}

CCoreActionFrame::~CCoreActionFrame()
{
    unload();
}

int    CCoreActionFrame::nBone()
{
    return m_nBone;
}

D3DXMATRIX& CCoreActionFrame::operator[](int index)
{
    assert(index < m_nBone);
    assert(index >= 0);
    return m_FrameMat[index];
}

D3DXMATRIX&  CCoreActionFrame::wsMat(int index)
{
    assert(index < m_nBone);
    assert(index >= 0);
    return m_WSMats[index];
}

CBoneTrans&  CCoreActionFrame::boneTrans(int index)
{
    assert(index < m_nBone);
    assert(index >= 0);
    return m_vBoneTrans[index];
}

bool CCoreActionFrame::load(int _nBone)
{
    unload();
    m_FrameMat   = new D3DXMATRIX[_nBone];
    m_WSMats     = new D3DXMATRIX[_nBone];
    m_vBoneTrans = new CBoneTrans[_nBone];
    assert(m_FrameMat);
    m_nBone = _nBone;
    return m_FrameMat != NULL;
}

void CCoreActionFrame::unload()
{
    XSAFE_DELETE_ARRAY(m_FrameMat);
    XSAFE_DELETE_ARRAY(m_WSMats);
    XSAFE_DELETE_ARRAY(m_vBoneTrans);
}

//////////////////////////////////////////////////////////////////////////
void CCoreActionAttr::init(int nBone , int Attri )
{
	m_BoneAttribtes.resize(nBone);
	for(int i = 0 ; i < nBone ; i ++)
	{
		m_BoneAttribtes[i] = Attri;
	}
}

bool CCoreActionAttr::boneUsed(int boneIndex)
{
	return m_BoneAttribtes[boneIndex] == eBoneInclude;
}

void CCoreActionAttr::setBoneAttribute(int attr )
{
	size_t nBone = m_BoneAttribtes.size();
	for(size_t i = 0 ; i < nBone ; i ++) m_BoneAttribtes[i] = attr;
}

void CCoreActionAttr::setBoneAttribute(int attr , int idx)
{
	m_BoneAttribtes[idx] = attr;
}

void CCoreActionAttr::setBoneAttribute(int* pAttributes , int nAttr , int iStart)
{
	for(int i = iStart ; i < iStart + nAttr ; i ++)
	{
		m_BoneAttribtes[i] = pAttributes[i - iStart];
	}
}

//////////////////////////////////////////////////////////////////////////
void CBaseAction::_getBlendFrameLoop(const CActionInfo* pInfo  , long _time_in_ms , CBaseActionState& _as)
{
	int& frame1 = _as.m_Frame1 ;
	int& frame2 = _as.m_Frame2 ;

	int lTime = pInfo->m_lTime;
	if(lTime <=0 ) lTime = 1;
    long t = _time_in_ms % lTime;

    int nFrame = pInfo->m_iLastFrame - pInfo->m_iFirstFrame + 1;
    float fFrame = (nFrame ) *  t / float( lTime); 

    frame1 = (int)fFrame;
    frame2 = frame1 + 1;
    if(frame2 >= nFrame)
    {
        frame2 = frame2%nFrame;
    }
    if(frame1 > nFrame)
    {
        frame1 = frame1%nFrame;
    }
    _as.m_fTime =  fFrame - frame1;
}

void CBaseAction::_getBlendFrameClamp(const CActionInfo* pInfo  , long _time_in_ms , CBaseActionState& _as)
{
	int& frame1 = _as.m_Frame1 ;
	int& frame2 = _as.m_Frame2 ;
    if(_time_in_ms > pInfo->m_lTime) _time_in_ms = pInfo->m_lTime;
    long t = _time_in_ms ;//tt % m_Info.m_lTime;

    int nFrame = pInfo->m_iLastFrame - pInfo->m_iFirstFrame + 1;
    float lTime = float( pInfo->m_lTime); 
    if(lTime == 0.0f ) lTime = 30.0f;
    float fFrame = (nFrame ) *  t / float( pInfo->m_lTime); 

    frame1 = (int)fFrame;
    frame2 = frame1 + 1;
    if(frame2 >= nFrame)
    {
        frame2 = nFrame - 1;//frame2%nFrame;
    }
    if(frame1 >= nFrame)
    {
        frame1 = nFrame - 1; //frame1%nFrame;
    }
    _as.m_fTime =  fFrame - frame1;
}

void CBaseAction::updateState(long _time_in_ms , CBaseActionState& _as , bool bLoop)
{
	if(bLoop)
	{
		_getBlendFrameLoop(info() , _time_in_ms , _as);
	}
	else
	{
		_getBlendFrameClamp(info() ,_time_in_ms , _as);
	}
}

void CBaseAction::updateState(long _time_in_ms , CBaseActionState& _as)
{
    if(m_bLoopAction)
    {
        _getBlendFrameLoop(info() , _time_in_ms , _as);
    }
    else
    {
        _getBlendFrameClamp(info() ,_time_in_ms , _as);
    }
}

CBaseAction::CBaseAction(CCoreSkeleton* pSkeleton , int param)
{

}


CBaseAction* CBaseAction::createInstance(eActionType _type , CCoreSkeleton* pSkeleton  , int param  )
{
     std::wstring _className = L"";
     switch(_type)
     {
     case eActType_Skeleton   : _className = L"CCoreAction"      ;  break;
     case eActType_Keyframe   : _className = L"xKeyFrameAction"  ;  break;
     case eActType_Frame      : assert(0)                        ;  break;
     case eActType_Blend      : assert(0)                        ;  break;
     case eActType_Transition : _className = L"CTransitionAction";  break;
     case eActType_Mix        : _className = L"CMixedAction"     ;  break;
     }
     return createInstance(_className.c_str() , pSkeleton , param);
}

CBaseAction* CBaseAction::createInstance(const wchar_t*  _class , CCoreSkeleton* pSkeleton  , int param )
{
    std::wstring _className = _class;
    if(_className == L"CCoreAction")
        return new CCoreAction(pSkeleton , param);
    if(_className == L"CTransitionAction" )
        return new CTransitionAction(pSkeleton , param);
    if(_className == L"CMixedAction" )
        return new CMixedAction(pSkeleton , param);
    return NULL;
}

//
//bool CBaseAction::getBoneData(xBoneData& boneData , int boneIndex, long _time )
//{
//	CBaseActionState _ActionState;
//	updateState(_time , _ActionState , m_bLoopAction);
//	CBoneTrans boneTrans;
//	getBoneTrans(boneIndex , &_ActionState , boneTrans);
//	boneData.fromBoneTrans(boneTrans);
//	return true;
//}


