#ifndef __XCORE_ACTION_H__
#define __XCORE_ACTION_H__

#include "ModelFWDecl.h"
#include "CoreMeshDef.h"
#include "BaseAction.h"


class  _A3D_API_  xSkeletonAction : public CBaseAction
{
public:
    xSkeletonAction(CCoreSkeleton* pSkeleton , int param);
    virtual int                  memUsage();
    virtual bool                 setTime(float _time);
    virtual float                getDurTime();
    virtual const CActionInfo*   info() const { return &m_Info ; }
    virtual const CSkeletonID&   skeletonID() const { return m_SkeID ; }
    virtual const wchar_t*       name() const { return m_Name.c_str() ; }
    virtual const int            nBone() const{ return m_nBone ;}
    virtual void                 setBoneNumber(int _nBone);
    virtual CCoreActionAttr*     attribute(){ return &m_ActionAttr ; }
protected:
    CSkeletonID                  m_SkeID;
    CCoreActionAttr              m_ActionAttr;
    int32                        m_nBone;
    CBaseActionState             m_ActionState;
};

class  _A3D_API_  CCoreAction : public xSkeletonAction
{
	friend  class  CCoreSkeleton;
public:
    CCoreAction(CCoreSkeleton* pSkeleton , int param);
    ~CCoreAction();
	virtual bool                 getBoneTrans(int boneIndex, CBaseActionState* pActionState , CBoneTrans& boneTrans) ;
	//public interface
	virtual bool                 blend(long time_in_ms,CCoreActionFrame* skeletonFrame, CCoreSkeleton* pSkeleton);
	virtual bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir);
	virtual bool                 load(const wchar_t * _name , xcomdocstream* stream);
	virtual bool                 load(const wchar_t * _name , istream& stream);
	virtual bool                 load(xXmlNode* pCfgNode );
	virtual void                 unload();

	virtual const CBoneArray&    boneArray() const { return m_BoneFrame ; }

protected:
	//获取某一帧骨骼的位置;
    CBoneTrans*                  getBoneData(int boneIndex, int frame);
	CBoneArray                   m_BoneFrame;//m_BoneData[iBone][nFrame]

};


#endif
