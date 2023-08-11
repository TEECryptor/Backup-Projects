#ifndef __XTRANSCATION_ACTION_H__
#define __XTRANSCATION_ACTION_H__

#include "CoreAction.h"



class _A3D_API_ CTransitionAction : public CCoreAction
{
public:
	CTransitionAction(CCoreSkeleton* pSkeleton , int param);
	~CTransitionAction();
    void  setTransitTime(float _time);
    float getDurTime();
	bool  setAction(CBaseAction* pStart , CBaseAction* pEnd , float _time);

    //通过最后一帧，和下一个动作的起始帧来组成一个新的动作。方便做动作过渡.
    bool setAction(CCoreSkeleton* pSkeleton , CCoreActionFrame* pStart , CBaseAction* pEnd , float _time);

public:
	virtual bool            load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir) { return true ; }
	virtual bool            load(const wchar_t * _name , xcomdocstream* stream) { return true ; }
	virtual bool            load(const wchar_t * _name , istream& stream) { return true ; }
	virtual bool            load(xXmlNode* pCfgNode ) { return true ; }
public:
	static bool             IsTransitionName(const std::wstring& _name , std::wstring& _first , std::wstring& _second);
	static std::wstring     MakeName(const wchar_t* _first , const wchar_t* _second);
};


#endif
