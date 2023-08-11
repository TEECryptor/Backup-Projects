#ifndef __XMIX_ACTION_H__
#define __XMIX_ACTION_H__

#include "BaseAction.h"


//混合的动作。可以有上下身混合的动作，表情混合动作等等
/***********************************************
动作混合的原理：
1. 混合的动作，需要加载一个动作配置文件。扩展名为.mix
   这个文件一般跟其它的动作文件放在一起。收动作管理器
   的管理，加载是需要给xActionName指定Type为"Mix";

2. 动作配置文件中指定分组，分组称为CActionSlot。每个
   Slot有名字和属性表。

3.CActionSlot中有一个Attribute数组。读入.mix文件，其实
  就是设置这些Slot中的Attribute数组，同时设置CMixedAction
  中的m_BoneActionMap。Map将会指出某块骨骼属于哪个Slot控制。
  在Slot的Attribute中，只有属于该Slot的Bone.其Attribute值
  才会是0.

5. 在.mix文件中，如果某个slot没指定哪些骨骼，则会使用所有
   前面分配剩下的骨骼。 如果指定了骨骼，则可以选择是不是
   递归分配所有的子骨骼（一般情况下是true)。也可以设置是
   只分配其子骨骼。

6.混合动作的时候，getBoneData会根据Bone属于哪个Slot。而将
  getBoneData的调用转到该Slot上挂接的xCoreAction上去。

7.混合动作在MixedAction中有个专用函数。可以为每个Slot上的
  动作指定一个时间轴。注意time_in_ms是个数组。


***********************************************/
class _A3D_API_ CMixedAction : public CBaseAction
{
public:

	struct CActionSlot
	{
		std::wstring       m_strName;
		int                m_hashName;
		CCoreActionAttr*   m_pAttribute;
    public:
        CActionSlot()
        {
            m_hAction = NULL;
        }
        CBaseAction*          getAction() const
        {
            return m_hAction;
        }

        void                  setAction(CBaseAction* pAction)
        {
            m_hAction = pAction;
        }

    protected:
        CBaseAction*          m_hAction;
	};

public:
	CMixedAction(CCoreSkeleton* pSkeleton , int param);
	~CMixedAction();
public:
    bool                 setMainAction(CBaseAction* pAction);
    bool                 setAction(const wchar_t* slotName , CBaseAction* pAction);
    bool                 setAction(int iSlot , CBaseAction* pAction);

	CActionSlot&         actionSlot(int iSlot){ return m_ActionSlots[iSlot] ; }
    int                  nActionSlot() const { return (int)m_ActionSlots.size() ; }
    bool                 getBoneTrans(int boneIndex, CBaseActionState* _pActionState , CBoneTrans& boneTrans);
public:
	bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	bool                 load(const wchar_t * _name , xcomdocstream* stream);
	bool                 load(const wchar_t * _name , istream& stream);
	bool                 load(xXmlNode* pCfgNode );

    CBaseActionState*    getActionState(int boneIndex, CBaseActionState* pActionState);
	CCoreActionAttr*     attribute();
	
	bool                 blend(long time_in_ms  , CCoreActionFrame* skeletonFrame, CCoreSkeleton* pSkeleton);
    bool                 blend(long time_in_ms[], CCoreActionFrame* skeletonFrame, CCoreSkeleton* pSkeleton);
    bool                 blend(CBaseActionState* pActionSlotState , CCoreActionFrame* skeletonFrame, CCoreSkeleton* pSkeleton );
	void                 unload();
	int                  memUsage();
	bool                 setTime(float _time);
    float                getDurTime();
	const CActionInfo*   info() const;
	const CSkeletonID&   skeletonID() const;
	const wchar_t*       name() const;
	const int            nBone() const;
	

protected:
	bool    setBoneToActionSlot(int boneIdx , int iSlot , bool bRecursive , bool onlyChild);
    bool    excludeBone(int boneIdx , int iSlotOwnner);
protected:
    typedef std::vector<CActionSlot>  xActionSlots;
	typedef std::vector<int>          xBoneActionMap;

	xActionSlots      m_ActionSlots;
	xBoneActionMap    m_BoneActionMap;
	CCoreSkeleton*    m_pSkeleton;
	CBaseActionState* m_pActionSlotState;
};


#endif
