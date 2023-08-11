#ifndef __XMIX_ACTION_H__
#define __XMIX_ACTION_H__

#include "BaseAction.h"


//��ϵĶ������������������ϵĶ����������϶����ȵ�
/***********************************************
������ϵ�ԭ��
1. ��ϵĶ�������Ҫ����һ�����������ļ�����չ��Ϊ.mix
   ����ļ�һ��������Ķ����ļ�����һ���ն���������
   �Ĺ�����������Ҫ��xActionNameָ��TypeΪ"Mix";

2. ���������ļ���ָ�����飬�����ΪCActionSlot��ÿ��
   Slot�����ֺ����Ա�

3.CActionSlot����һ��Attribute���顣����.mix�ļ�����ʵ
  ����������ЩSlot�е�Attribute���飬ͬʱ����CMixedAction
  �е�m_BoneActionMap��Map����ָ��ĳ����������ĸ�Slot���ơ�
  ��Slot��Attribute�У�ֻ�����ڸ�Slot��Bone.��Attributeֵ
  �Ż���0.

5. ��.mix�ļ��У����ĳ��slotûָ����Щ���������ʹ������
   ǰ�����ʣ�µĹ����� ���ָ���˹����������ѡ���ǲ���
   �ݹ�������е��ӹ�����һ���������true)��Ҳ����������
   ֻ�������ӹ�����

6.��϶�����ʱ��getBoneData�����Bone�����ĸ�Slot������
  getBoneData�ĵ���ת����Slot�Ϲҽӵ�xCoreAction��ȥ��

7.��϶�����MixedAction���и�ר�ú���������Ϊÿ��Slot�ϵ�
  ����ָ��һ��ʱ���ᡣע��time_in_ms�Ǹ����顣


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
