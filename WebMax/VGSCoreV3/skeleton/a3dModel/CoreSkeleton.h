#ifndef __XCORE_SKELETON_H__
#define __XCORE_SKELETON_H__

#include "ModelFWDecl.h"
#include "CoreMeshDef.h"
#include "BaseAction.h"


//============Action==================
class _A3D_API_ xBoneHiberarchy
{
public:
	xBoneHiberarchy();
	~xBoneHiberarchy();
	void clear();
	bool add_child(int index );
	void push_back(int index );
	int  childIndex(int iChild){ return m_Childrens[iChild] ; }
	int  nChildren(){ return m_nChildren ; }
	friend CCoreSkeleton;
protected:
	typedef std::vector<int> sBoneIndexs_t;
	short                 m_nChildren;
	sBoneIndexs_t         m_Childrens;
};

typedef vector<xBoneHiberarchy>  vBoneHiberarchys;

class _A3D_API_ CCoreSkeleton : public CRefCountObject
{
public:
	CCoreSkeleton();
	~CCoreSkeleton();
	int                nBone(){return m_nBone;}
	bool               unload();
	xBoneHiberarchy&   boneHiberarchy(int iBone);
	bool               load(xcomdoc& doc,const wchar_t* skeleton_dir);
	bool               loadHiberarchy(xcomdocstream* _in);
	bool               loadBoneList(xcomdocstream* _in);
	int                findBoneIndex(const wchar_t* boneName);
	const CSkeletonID& id() const { return m_SkeletonID ; }
    const xSkinBone&   getBone(int idx) { return m_Bones[idx] ; }
	CCoreActionFrame*  createActionFrame();
	void               destoryActionFrame(CCoreActionFrame* pAcFrame);
public:
	bool              blendSlerp(CBaseAction* pAction, CBaseActionState* pActionState , CCoreActionAttr* pAttr ,  CCoreActionFrame& skeletonFrame,int parent = -1);
    bool              blendSlerp(CBaseAction* pAction, CBaseActionState* pActionState , CCoreActionFrame& skeletonFrame , int parent = -1);
protected:
	//骨架层次结构
	xBoneHiberarchy     m_RootHibers;
	vBoneHiberarchys    m_BoneHibers;
	//骨架信息
	int                 m_nBone;
	vSkinBones          m_Bones;
	CSkeletonID         m_SkeletonID;
};


#endif
