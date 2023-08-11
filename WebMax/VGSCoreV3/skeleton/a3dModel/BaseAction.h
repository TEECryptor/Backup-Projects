#ifndef __XBASE_ACTION_H__
#define __XBASE_ACTION_H__
#include "../miniXml/xXmlDocument.h"
#include "ModelFWDecl.h"
#include "CoreMeshDef.h"



class  _A3D_API_ CCoreActionFrame : public CRefCountObject
{
public:
	CCoreActionFrame();
	~CCoreActionFrame();
	int                nBone();
	D3DXMATRIX&        operator[](int index);
    D3DXMATRIX&        wsMat(int index);//拿到世界空间变换矩阵
    CBoneTrans&        boneTrans(int index);
	bool               load(int nBone);
	void               unload();
	void*              data(){return m_FrameMat ; }
protected:
	int                m_nBone;
	D3DXMATRIX*        m_FrameMat;
    D3DXMATRIX*        m_WSMats;//世界空间中的
    CBoneTrans*        m_vBoneTrans;
	friend   class     CCoreSkeleton;
};

class _A3D_API_ CBoneArray
{
public:
    class CBoneArray1D
    {
    public:
        CBoneArray1D(CBoneTrans* pData , int Len)
        {
            m_pData = pData;
            m_Len = Len;
        }
        CBoneTrans&  operator[](int index)
        {
            if(index >= m_Len || index < 0)
            {
                assert(0);
            }
            return m_pData[index];
        }
    protected:
        CBoneTrans* m_pData;
        int         m_Len;
    };

public:
    CBoneArray();
    ~CBoneArray();
    void         create(int* DimSize);
    void         create(int Row , int Col);
#ifndef _DEBUG
    CBoneTrans*  operator[](int index);
#else
    CBoneArray1D operator[](int index);
#endif
    void         free();
public:
    CBoneTrans*      m_Data;
    int              m_Col;
    int              m_Row;
};

class _A3D_API_ CCoreActionAttr
{
public:
	enum 
	{
		eBoneInclude = 0,
		eBoneExclude  ,
	};
public:
	int       boneAttribute(int boneIndex){ return m_BoneAttribtes[boneIndex] ; }
	void      setBoneAttribute(int attr );
	void      setBoneAttribute(int attr , int idx);
	void      setBoneAttribute(int* pAttributes , int nAttr , int iStart = 0);
	void      init(int nBone , int Attri = eBoneInclude);
	bool      boneUsed(int boneIndex);

   int&       operator [](int idx){ return m_BoneAttribtes[idx] ; }
private:
	std::vector<int>  m_BoneAttribtes;
};


struct _A3D_API_ CBaseActionState
{
    long  m_lTime;
    int   m_Frame1;
    int   m_Frame2;
    float m_fTime;
};


class _A3D_API_ CBaseAction : public CRefCountObject
{
public:
    //public interface
	CBaseAction(CCoreSkeleton* pSkeleton , int param);
	virtual CCoreActionAttr*     attribute() = 0;
    virtual bool                 blend(long time_in_ms,CCoreActionFrame* skeletonFrame, CCoreSkeleton* pSkeleton) = 0;
    virtual bool                 load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir) = 0;
    virtual bool                 load(const wchar_t * _name , xcomdocstream* stream) = 0;
    virtual bool                 load(const wchar_t * _name , istream& stream) = 0;
    virtual bool                 load(xXmlNode* pCfgNode ) = 0;
    virtual void                 unload() = 0;
    virtual int                  memUsage() = 0;
    virtual bool                 setTime(float _time) = 0;
    virtual float                getDurTime() = 0;
    virtual const CActionInfo*   info() const = 0;
    virtual const CSkeletonID&   skeletonID() const = 0;
    virtual const wchar_t*       name() const = 0;
    virtual const int            nBone() const  = 0;

    static CBaseAction*          createInstance(eActionType _type , CCoreSkeleton* pSkeleton = NULL , int param = 0 );
    static CBaseAction*          createInstance(const wchar_t*  _class , CCoreSkeleton* pSkeleton = NULL , int param = 0 );
public:
	//这个是专门用来给非常规动作用的
	virtual CBaseActionState*    getActionState(int boneIndex, CBaseActionState* pActionState) { return pActionState; }
    virtual void                 setLoop(bool bLoop) { m_bLoopAction  = bLoop ;}
    virtual bool                 bLoop(){ return m_bLoopAction ; }

	//默认的实现，可以重载
	virtual bool                 getBoneTrans(int boneIndex, CBaseActionState* pActionState , CBoneTrans& boneTrans) = 0;
    virtual void                 updateState(long _time_in_ms , CBaseActionState& _as , bool bLoop);
    virtual void                 updateState(long _time_in_ms , CBaseActionState& _as);
protected:
    void          _getBlendFrameLoop(const CActionInfo* pInfo  , long _time_in_ms , CBaseActionState& _as );
    void          _getBlendFrameClamp(const CActionInfo* pInfo  , long _time_in_ms , CBaseActionState& _as);
	

protected:
    bool              m_bLoopAction;
    std::wstring   m_Name;      //Name
    CActionInfo       m_Info;
};


//////////////////////////////////////////////////////////////////////////
struct _A3D_API_ CTrackActionInfo
{
	CBaseAction* m_pAction;
	float        m_DesireTime;
	float        m_OriginTime;

	CTrackActionInfo()
	{
		m_pAction = NULL;
	}

	void setNULL()
	{
		XSAFE_RELEASE(m_pAction);
		m_DesireTime = 0.0f;
		m_OriginTime = 0.0f;
	}
	CTrackActionInfo(CBaseAction*  pAction)
	{
		m_pAction = pAction;
		m_pAction->AddRef();
		m_DesireTime = m_pAction->getDurTime();
		m_OriginTime = m_DesireTime;
	}

	CTrackActionInfo(CBaseAction*  pAction , float desireTime)
	{
		m_pAction = pAction;
		m_pAction->AddRef();
		m_DesireTime = desireTime;
		m_OriginTime = pAction->getDurTime();
	}

	CTrackActionInfo(CBaseAction*  pAction , CBaseAction*  pSyncAction)
	{
		m_pAction = pAction;
		m_pAction->AddRef();
		m_DesireTime = pSyncAction->getDurTime();
		m_OriginTime = pAction->getDurTime();
	}

	void setDesireTime()
	{
		if(m_pAction )
		{
			m_pAction->setTime(m_DesireTime);
		}
	}

	void setOriginTime()
	{
		if(m_pAction)
		{
			m_pAction->setTime(m_OriginTime);
		}
	}
};


enum eActionTrackEvent
{
    eActionChange,
    eActionBegin,
    eActionFinish,
};

class _A3D_API_ IActionTrackListenner
{
public:
    virtual void onActionTrackEvent(eActionTrackEvent _event, int idx ,  CBaseAction* pAction) = 0;
};

typedef std::deque<CTrackActionInfo>  vActionQueue;
typedef std::vector<IActionTrackListenner*> vActionEventListenners;
typedef std::deque<CTrackActionInfo>  vActionQueue;
typedef std::vector<CTrackActionInfo> vActionList;

class _A3D_API_ CActionTrackController
{
    int m_TimeA;
    int m_TimeB;
    int m_iLoopTime;
public:
    void setLoopCtrl(int _frameA , int _frameB = 0x7ffffff , int _loopTime = -1)
    {
        m_TimeA    = _frameA;
        m_TimeB    = _frameB;
        m_iLoopTime = _loopTime;
    }

    //返回初始帧
    int  begin()
    {
        return m_TimeA;
    }

    int  update(int actTime , unsigned long passedTime)
    {
          actTime += passedTime;
          //时间到了
          if(actTime >= m_TimeB)
          {
              if(m_iLoopTime > 0)
                  m_iLoopTime -- ;

              //循环次数到头
              if(m_iLoopTime == 0)
              {
                  actTime = m_TimeB;
              }
              else
              {
                  //循环次数没有到头
                  actTime = (m_TimeA + passedTime) % (m_TimeB- m_TimeA);
              }
          }
          return actTime;
        
    }


};

class _A3D_API_ CActionTrack 
{
public:
	void              setTimeScale(float _scale);
	float             timeScale();
	void              setTransitionTime(int _time);
	bool              setActionList(vActionList& actions, int iLoopTime, bool bTransit = true , CBaseAction* pLastAction = NULL);
	CBaseAction*      getCurrentAction();
	bool              doNextAction(int _time);
	bool              update(unsigned long passedTime , CCoreSkeleton* pSkeleton);
	bool              enterStandby();
	int               actionTime();
	int               hashName();
	bool              bLoop() { return m_iLoopTime != 0 && m_iLoopTime != 1 ; }
	CActionTrack(CBaseModel* pModel , const wchar_t* _name , int iSlot);
 
    void              addListenner(IActionTrackListenner* pListenner);
    void              removeListenner(IActionTrackListenner* pListenner);
    void                    fireEvent(eActionTrackEvent _event,  int iTrackIdx , CBaseAction* pAction);
    CActionTrackController* TimeCtller(){ return &m_TimeCtller ; }
protected:
    vActionEventListenners m_vListenners;
	int                    m_actTime;
	int                    m_iTransitionTime;
	float                  m_fTimeScale;
	vActionQueue           m_vActions;
	vActionList            m_vActionLists;
	CTrackActionInfo       m_CurrentAction;
	CBaseModel*            m_hModel;
	int                    m_iSlot;
	std::wstring           m_name;
	int                    m_HashName;
	int                    m_iLoopTime;
    CActionTrackController m_TimeCtller;
};


#endif
