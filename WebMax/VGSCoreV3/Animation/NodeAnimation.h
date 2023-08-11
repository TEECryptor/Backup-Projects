// NodeAnimation.h: interface for the CNodeAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEANIMATION_H__0943F806_7C2F_4731_93AE_505CE1EEB13B__INCLUDED_)
#define AFX_NODEANIMATION_H__0943F806_7C2F_4731_93AE_505CE1EEB13B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\VGSHeader.h"
#include "NodeKeyFrame.h"
#include "..\Node.h"

#include <vector>

using namespace std;

// 节点的关键帧动画
class VGS_EXPORT CNodeAnimation  
{
public:
	CNodeAnimation(CSceneNode *pNode, const std::string &sName);
	virtual ~CNodeAnimation();
	
	virtual void   Release();   // 释放内存

	// 动画的类型
	VGS_ANIMATIONTYPE getType();

public:

	std::string getName(){return m_sName;}

	CSceneNode*  getNode();  // 获得动画节点

	VOID addKeyframe(CNodeKeyFrame *pNodeKfr);
	
	void   setLength(double timeLength);         // 设置动画时间长度
	double getLength(){return m_totalLength;}  // 动画的时间长度

	INT    getCurFrame() {return m_curFrame;}
	void   setTimePosition(double timepos);  // 设置时间位置
	void   setCurFrame(INT frameIdx);
	INT    getFrameCount(){return m_vpNodeKfr.size();}

	CNodeKeyFrame* getFrameData(UINT frameIdx)
	{
		if (frameIdx >= 0 && frameIdx < m_vpNodeKfr.size())
		{
			return m_vpNodeKfr[frameIdx];
		}
		return 0;
	}
	
	void setStartPlayTime(double timeStart);    // 设置开始播放动画的时间

	void addTime(double passTime);              // 移动时间, 播放当前时间passTime后的一帧	
	virtual void update();

	void			setEnabled(bool enable);	// 设置是否驱动关键帧动画播放
	bool            getEnabled(){return m_bEnable;}

	void			setLoop(bool loop);	        // 设置是否循环播放
	bool            getLoop(){return m_bLoop;}
		

public:

	std::string                   m_sName;
	CSceneNode*                   m_pNode;     // 动画节点
	std::vector <CNodeKeyFrame*>  m_vpNodeKfr;  // 关键帧列表

	double   m_totalLength;   // 动画的总时间长度
	double   m_startPlayTime;     // 开始播放的时间

	double   m_curTime;   // 当前时间
	INT      m_curFrame;  // 当前帧

	bool     m_bEnable;   // 是否可以播放
	bool     m_bLoop;     // 是否循环播放
	VGS_ANIMATIONTYPE m_kfrType;  // 关键帧动画类型

};

#endif // !defined(AFX_NODEANIMATION_H__0943F806_7C2F_4731_93AE_505CE1EEB13B__INCLUDED_)
