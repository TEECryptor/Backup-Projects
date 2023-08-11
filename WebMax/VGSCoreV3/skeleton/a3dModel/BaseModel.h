#ifndef __XBASE_MODEL_H__
#define __XBASE_MODEL_H__

#include "../miniXml/xXmlDocument.h"
#include "../xcomdoc/xcomdoc.h"
#include "TextureManager.h"
#include "ModelFWDecl.h"
#include "CoreSkeleton.h"
#include "Shader/FXShader.h"


class _A3D_API_ CBaseModel   : public CRefCountObject
{
public:
    CBaseModel(CA3DDeviceWrapper* pDevice , bool bSysMemCopy = false);
	virtual bool           load(const wchar_t* fileName , const BYTE* buf , size_t bufLen, unsigned long arg) = 0;
	virtual bool           load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg) = 0;
    virtual bool           addPackage(const wchar_t* fileName , const BYTE* buf , size_t bufLen) = 0;
	virtual bool           load(const wchar_t* fileName , unsigned long  arg);
	virtual bool           unload();
	virtual unsigned long  memUsage();
	virtual bool           isLoaded();
public:
	
    bool                   draw(CCoreActionFrame* pActionFrame = NULL); 
	bool                   drawMeshs(CCoreActionFrame* pActionFrame = NULL); 
    CCoreActionFrame*      createActionFrame();
	void                   destoryActionFrame(CCoreActionFrame* pAcFrame);
	bool                   updateFrame(CCoreActionFrame* pAcFrame, int actIdx , unsigned long actTime);
	bool                   updateFrame(unsigned long passedTime);
	CBoundBox&             boundBox();
	CCoreSkeleton*         skeleton(){ return m_pSkeleton ; }
	bool                   setAction(size_t idx);
    CCoreMesh*             getMesh(size_t idx);
	size_t                 nMesh();
	bool                   setupAnimationTime(int actIdx , float actTime);
    bool                   createBoneFrameBuffer();
	CBaseAction*           coreAction(size_t idx );
	size_t                 nAction();
    bool                   setupAnimation(CCoreActionFrame* pAcFrame);
    bool                   setupAnimation();
	bool                   attachMesh(CCoreMesh* pMesh);
	bool                   detachMesh(CCoreMesh* pMesh);

	void                   aabb(CBoundBox& _aabb){ _aabb = m_aabb ; }
	const CBoundBox&       aabb(){return m_aabb ;}
	const wchar_t*         name();
	void                   setName(const wchar_t* _name);
    bool                   HasGpuSkin();

public:	
    typedef std::vector<CBaseAction*>              vCoreActions;
    typedef std::map<int , int>                    mapMeshNameToIdx;

protected:
	CBoundBox         m_aabb;
	std::wstring      m_name;
	vCoreActions      m_Actions;
	CCoreSkeleton*    m_pSkeleton;
	CCoreActionFrame  m_ActionFrame;
	size_t            m_iActTime;
	size_t            m_iAction;
    CA3DDeviceWrapper*       m_DeviceWrapper;

	vCoreMeshs        m_Meshs;	
	mapMeshNameToIdx  m_EmbMeshNameIdx;
	CBaseTextureMgr   m_TexMgr;

};

#endif
