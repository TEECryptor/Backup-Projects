#include "../xcomdoc/xcomdoc.h"
#include "../xcomdoc/xI18N.h"
#include "MixAction.h"
#include "CoreSkeleton.h"
#include "CoreMesh.h"

CMixedAction::CMixedAction(CCoreSkeleton* pSkeleton , int param) : CBaseAction(pSkeleton , param)
{
	m_Info.m_eActType    = eActType_Mix;
	m_Info.m_iFirstFrame = 0;
	m_Info.m_iLastFrame  = 200;
	m_Info.m_lTime       = 5000; //5秒
	m_Info.m_nFrame      = 200;
	m_pSkeleton          = pSkeleton;
	m_pActionSlotState   = NULL;
}


CMixedAction::~CMixedAction()
{

}

bool CMixedAction::setMainAction(CBaseAction* pAction)
{
	return true;
}

bool CMixedAction::setAction(int slotName , CBaseAction* pAction)
{
    CActionSlot& slot = m_ActionSlots[slotName];
    slot.setAction(pAction);
    return true;
}

bool CMixedAction::load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir)
{
	std::wstring fullName = std::wstring(actionDir) + pCfgNode->name() + L".xra";
	xcomdocstream* stream =doc.open_stream(fullName.c_str() , true);
	if(stream == NULL)
	{
		//找没扩展名的
		fullName = std::wstring(actionDir) + pCfgNode->name();
		stream = doc.open_stream(fullName.c_str() , true);
	}

	if(stream == NULL)
		return false;

	bool ret= load(pCfgNode->value(L"Name") , stream );
	doc.close_stream(stream);
	return ret;
}

bool CMixedAction::load(const wchar_t * _name , xcomdocstream* stream)
{
	BYTE* buf = new BYTE[stream->data_len()];
	stream->read(buf , stream->data_len() );
	xXmlDocument doc;
	if(false == doc.load(buf , stream->data_len() , true) )
	    return false;
	return load(doc.root());
}

bool CMixedAction::load(const wchar_t * _name , istream& stream)
{
	xXmlDocument doc;
	if( doc.load(stream , true) == false )
		return false;
	return load(doc.root());
}

bool CMixedAction::load(xXmlNode* pXmlNode )
{
	unload();
	int _nBone = m_pSkeleton->nBone();
    m_BoneActionMap.resize(_nBone);
	for(int i = 0 ; i <  _nBone; i ++)
	{
		m_BoneActionMap[i] = -1;
	}

	if(pXmlNode->name() == std::wstring(L"slots") )
	{
		xXmlNode::XmlNodes slotNodes;
		pXmlNode->findNode(L"slot" , slotNodes);
		size_t nSlot = slotNodes.size();
        m_ActionSlots.resize(nSlot);
		for(size_t iSlot = 0 ; iSlot < nSlot ; iSlot ++)
		{
			xXmlNode* pSlotNode = slotNodes[iSlot];
			CActionSlot& slot = m_ActionSlots[iSlot];
			slot.m_strName = pSlotNode->value(L"name");
			slot.m_pAttribute = new CCoreActionAttr();
			slot.m_pAttribute->init(_nBone , CCoreActionAttr::eBoneExclude );
			slot.m_hashName = xStringHash( slot.m_strName.c_str() );
			//初始化骨骼;
			xXmlNode::XmlNodes boneNodes;
			pSlotNode->findNode(L"bone" , boneNodes);
			size_t nBoneInSlot = boneNodes.size();

            //如果没有bone列表。则表示这个Slot瓜分所有剩下的.
			if(boneNodes.size() == 0 && iSlot != 0)
			{
				//分配所有没有分配过的
				for(int i = 0 ; i <  _nBone; i ++)
				{
					if(m_BoneActionMap[i] == -1) 
					{
						m_BoneActionMap[i] = (int)iSlot;
						(*slot.m_pAttribute)[i] = CCoreActionAttr::eBoneInclude ;
					}
				}
			}

			//分配所有的Bone;
			for(size_t iBone = 0 ; iBone < nBoneInSlot ; iBone ++)
			{
				xXmlNode* pBoneNode = boneNodes[iBone];
				bool bRecursive = pBoneNode->bool_value(L"recursive");
				bool onlyChild = pBoneNode->bool_value(L"onlyChild");

				int boneIdx = m_pSkeleton->findBoneIndex(pBoneNode->value(L"name"));
				setBoneToActionSlot(boneIdx , (int)iSlot , bRecursive , onlyChild);
			}

		}

		//将所有没有分配过的骨骼，全部分配到第一个节点上。
		for(int i = 0 ; i <  _nBone; i ++)
		{
			if(m_BoneActionMap[i] == -1) 
			{
				m_BoneActionMap[i] = 0;
				CActionSlot& slot = m_ActionSlots[0];
				(*slot.m_pAttribute)[i] = CCoreActionAttr::eBoneInclude ;
			}
		}
	}
	return true;
}

bool CMixedAction::excludeBone(int boneIdx , int iSlotOwnner)
{
    int _nSlot = (int)m_ActionSlots.size();
    for(int i = 0 ; i < iSlotOwnner ; i ++)
    {
        if(iSlotOwnner == i) continue ;
        CActionSlot& slot = m_ActionSlots[i];
        (*slot.m_pAttribute)[boneIdx] = CCoreActionAttr::eBoneExclude;
    }
    return true;
}

bool CMixedAction::setBoneToActionSlot(int boneIdx , int iSlot , bool bRecursive , bool onlyChild)
{
	if(!onlyChild)
	{
		m_BoneActionMap[boneIdx] = iSlot;
		CActionSlot& slot = m_ActionSlots[iSlot];
		(*slot.m_pAttribute)[boneIdx] = CCoreActionAttr::eBoneInclude;
        excludeBone(boneIdx , iSlot);
	}

	if(bRecursive == false)
		return true;

	xBoneHiberarchy& boneH = m_pSkeleton->boneHiberarchy(boneIdx);

	for(int i = 0 ; i < boneH.nChildren(); ++i)
	{
		int     childIndex = boneH.childIndex(i);
		setBoneToActionSlot(childIndex , iSlot , bRecursive , false);
	}
	return true;
}

bool CMixedAction::getBoneTrans(int boneIndex, CBaseActionState* _pActionState , CBoneTrans& boneTrans)
{
    int iSlot = m_BoneActionMap[boneIndex];
    CBaseActionState* pActionState = _pActionState + iSlot;
    CBaseAction* pAction = m_ActionSlots[iSlot].getAction();
    return pAction->getBoneTrans(boneIndex , pActionState , boneTrans);
}

CBaseActionState* CMixedAction::getActionState(int boneIndex, CBaseActionState* pActionState)
{
    int iSlot = m_BoneActionMap[boneIndex];
    return (pActionState + iSlot);
}

CCoreActionAttr*  CMixedAction::attribute()
{
	return NULL;
}

bool CMixedAction::blend(CBaseActionState* pActionSlotState , CCoreActionFrame* _skeletonFrame, CCoreSkeleton* pSkeleton )
{
    CCoreActionFrame& skeletonFrame = *_skeletonFrame;
    size_t nSlot = m_ActionSlots.size();
    for(size_t i = 0 ; i < nSlot ; i ++)
    {
        CMixedAction::CActionSlot& slot = m_ActionSlots[i];
        CBaseActionState& SlotState = pActionSlotState[i];
        const CActionInfo* pActionInfo = slot.getAction()->info();
		slot.getAction()->updateState( SlotState.m_lTime , SlotState );
    }

    return pSkeleton->blendSlerp(this , pActionSlotState , skeletonFrame );

}

bool  CMixedAction::blend(long _time_in_ms,CCoreActionFrame* _skeletonFrame, CCoreSkeleton* pSkeleton)
{
	CCoreActionFrame& skeletonFrame = *_skeletonFrame;	
	size_t nSlot = m_ActionSlots.size();
	if(m_pActionSlotState == NULL)
		m_pActionSlotState = new CBaseActionState[nSlot];

	for(size_t i = 0 ; i < nSlot ; i ++)
	{
		CBaseActionState& SlotState = m_pActionSlotState[i];
		SlotState.m_lTime = _time_in_ms;
	}
	return blend(m_pActionSlotState, _skeletonFrame , pSkeleton);
}



bool  CMixedAction::blend(long _time_in_ms[], CCoreActionFrame* _skeletonFrame, CCoreSkeleton* pSkeleton)
{
	CCoreActionFrame& skeletonFrame = *_skeletonFrame;	
	size_t nSlot = m_ActionSlots.size();
	if(m_pActionSlotState == NULL)
		m_pActionSlotState = new CBaseActionState[nSlot];

	for(size_t i = 0 ; i < nSlot ; i ++)
	{
		CBaseActionState& SlotState = m_pActionSlotState[i];
		SlotState.m_lTime = _time_in_ms[i];
	}
	return blend(m_pActionSlotState, _skeletonFrame , pSkeleton);
}

void CMixedAction::unload()
{
	size_t nSlot = m_ActionSlots.size();
    for(size_t i = 0 ; i < nSlot ; i ++)
	{
		CMixedAction::CActionSlot& slot = m_ActionSlots[i];
		XSAFE_DELETE(slot.m_pAttribute);
	}
	XSAFE_DELETE_ARRAY(m_pActionSlotState);
	m_ActionSlots.clear();
	m_BoneActionMap.clear();
}

int CMixedAction::memUsage()
{
    return 1;
}

bool CMixedAction::setTime(float _time)
{
    return false;
}

float CMixedAction::getDurTime()
{
    return 0.0f;
}

const CActionInfo* CMixedAction::info() const
{
	const CMixedAction::CActionSlot& slot = m_ActionSlots[0];
    const CBaseAction* pAction = slot.getAction();
    if(pAction == NULL) return NULL;
	return pAction->info();
}

const CSkeletonID& CMixedAction::skeletonID() const
{
      return m_pSkeleton->id();
}

const wchar_t* CMixedAction::name() const
{
     return m_Name.c_str();
}

const int CMixedAction::nBone() const
{
	return m_pSkeleton->nBone();
}


