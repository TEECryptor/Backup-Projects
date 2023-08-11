#include "../xcomdoc/xI18N.h"
#include "TransitionAction.h"
#include "CoreSkeleton.h"
#include "CoreMesh.h"

//两个动作之间做插值的过渡动作
CTransitionAction::CTransitionAction(CCoreSkeleton* pSkeleton , int param) : CCoreAction(pSkeleton , param)
{
	m_Info.m_eActType = eActType_Transition;
	m_Info.m_iFirstFrame = 0;
	m_Info.m_iLastFrame = 1;
	m_Info.m_lTime = 500; //0.5秒
	m_Info.m_nFrame = 2;
}

CTransitionAction::~CTransitionAction()
{

}

void CTransitionAction::setTransitTime(float _time)
{
   m_Info.m_lTime = (int)(_time * 1000);//毫秒记
}

float CTransitionAction::getDurTime()
{
    return m_Info.m_lTime/1000.0f;
}

bool CTransitionAction::setAction(CBaseAction* pStart , CBaseAction* pEnd , float _time)
{
    int nBone = pStart->nBone(); //
	if(pStart->skeletonID() != pEnd->skeletonID())
		return false;

	m_BoneFrame.free();

	m_Info.m_lTime = (int)(_time * 1000);
	m_nBone = pStart->nBone();
	int dim[2];
	dim[0] = m_nBone;
	dim[1] = m_Info.m_nFrame;

	m_SkeID = pStart->skeletonID();
	m_Name  = MakeName(pStart->name() , pEnd->name() );
	m_nBone = pStart->nBone();
	m_BoneFrame.create(dim);
	int nLastFrame = pStart->info()->m_nFrame - 1;
	m_ActionAttr.init(m_nBone);
	CBaseActionState _State1;
	CBaseActionState _State2;
	_State1.m_lTime = pStart->info()->m_lTime;
	_State2.m_lTime = 0;
    
	CBoneTrans boneTrans;
	for(int iBone = 0 ; iBone < m_nBone ; iBone++)
	{
        CCoreActionAttr* pStartAttri = pStart->attribute();
		m_ActionAttr[iBone] = pStartAttri ? pStartAttri->boneAttribute(iBone) : CCoreActionAttr::eBoneInclude;
		pStart->updateState(_State1.m_lTime , _State1);
		pEnd  ->updateState(_State2.m_lTime , _State2);

        pStart->getBoneTrans(iBone , &_State1 , m_BoneFrame[iBone][0] ); //第一帧就是前一个动作的最后一帧
		pEnd  ->getBoneTrans(iBone , &_State2 , m_BoneFrame[iBone][1] ); //第二帧就是后一个动作的第一帧
	}
	return true;
}

bool CTransitionAction::setAction(CCoreSkeleton* pSkeleton , CCoreActionFrame* pStart , CBaseAction* pEnd , float _time)
{
    int nBone = pEnd->nBone(); //
    if(pEnd->skeletonID() != pSkeleton->id())
        return false;

    m_BoneFrame.free();

    m_Info.m_lTime = (int)(_time * 1000);
    m_nBone = pStart->nBone();
    int dim[2];
    dim[0] = m_nBone;
    dim[1] = m_Info.m_nFrame;

    m_SkeID = pEnd->skeletonID();
    m_Name  = MakeName(L"LastFrame" , pEnd->name() );
    m_nBone = pStart->nBone();
    m_BoneFrame.create(dim);
    int nLastFrame = pEnd->info()->m_nFrame - 1;
    m_ActionAttr.init(m_nBone);
	CBaseActionState _State2;
	_State2.m_lTime = 0;

    for(int iBone = 0 ; iBone < m_nBone ; iBone++)
    {
        CCoreActionAttr* pEndAttri = pEnd->attribute();
        m_ActionAttr[iBone] = pEndAttri ? pEndAttri->boneAttribute(iBone) : CCoreActionAttr::eBoneInclude;

        const xSkinBone& SkinBone = pSkeleton->getBone(iBone);
        CBoneTrans&      trans    = pStart->boneTrans( iBone );

        m_BoneFrame[iBone][0] = trans ;// , pStart->wsMat(iBone) , SkinBone.m_InitMTInv );
		pEnd  ->getBoneTrans(iBone , &_State2 , m_BoneFrame[iBone][1] );
        //m_BoneFrame[iBone][1] = *pEnd->getBoneData(iBone   ,  0         ) ; //第二帧就是后一个动作的第一帧
    }
    return true;
}

bool CTransitionAction::IsTransitionName(const std::wstring& _name , std::wstring& _first , std::wstring& _second)
{
	if(_name.find(L"->") == std::wstring::npos)
		return false;
	int pos = (int)_name.find(L"->");
	wchar_t buf[1024] = {0};
	wcsncpy(buf , _name.c_str() , 1024);
	buf[pos] = 0;
    _first = buf;
	_second = buf + pos + 2;
	return true;
}

std::wstring  CTransitionAction::MakeName(const wchar_t* _first , const wchar_t* _second)
{
	return std::wstring(_first) + L"->" + _second;
}


