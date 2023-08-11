// NodeAnimation.cpp: implementation of the CNodeAnimation class.
//
//////////////////////////////////////////////////////////////////////

#include "NodeAnimation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeAnimation::CNodeAnimation(CSceneNode *pNode, const std::string &sName)
{
	m_pNode = pNode;

	m_totalLength = 0;				// 动画总时长，秒为单位
	m_startPlayTime = 0;			// 关键帧动画开始播放的时间

	m_curTime  = 0;
	m_curFrame = 0;

	m_bEnable = true;
	m_bLoop   = true;

	m_kfrType = VAT_ANIMATIONTYPE_NONE;

	m_sName = sName;
}

CNodeAnimation::~CNodeAnimation()
{
	Release();
}

// 动画的类型
VGS_ANIMATIONTYPE CNodeAnimation::getType()
{
	return m_kfrType;
}
// 获得动画节点
CSceneNode*  CNodeAnimation::getNode()
{
	return m_pNode;
}

// 增加一帧动画
VOID CNodeAnimation::addKeyframe(CNodeKeyFrame *pNodeKfr)
{
	if (m_kfrType == VAT_ANIMATIONTYPE_NONE && pNodeKfr)
	{
		m_kfrType = pNodeKfr->getType();
	}
	m_vpNodeKfr.push_back(pNodeKfr);
}

// 设置动画时间长度
void CNodeAnimation::setLength(double timeLength)
{
	m_totalLength = timeLength;
}         

// 设置开始播放动画的时间
void CNodeAnimation::setStartPlayTime(double timeStart)
{
	m_startPlayTime = timeStart;
}    

// 移动时间, 播放当前时间passTime后的一帧
void CNodeAnimation::addTime(double passTime)
{	
	m_curTime += passTime;  // 更新当前时间
}  
 
void CNodeAnimation::update()
{
	while (m_curTime > m_totalLength)
	{
		if (m_bLoop)
		{
			m_curTime = m_curTime - m_totalLength;
		}
		else
		{
			m_curTime = m_totalLength;
		}
	}
	
	//找到当前时间对应的帧
	if (m_curTime <= m_startPlayTime)
	{
		m_curFrame = 0;
	}
	else
	{
		m_curFrame = (INT)((m_curTime - m_startPlayTime) * gkfrPlaySpeed);
	}

	// 当前帧不要越界
	m_curFrame = max(0, min((INT)m_curFrame, m_vpNodeKfr.size() - 1)); 
	
	if (!m_bEnable) 
	{
		return;
	}

	if (m_vpNodeKfr[m_curFrame])
	{
		m_vpNodeKfr[m_curFrame]->update();  // 更新帧状态
	}
}            	

// 设置是否驱动关键帧动画播放
void CNodeAnimation::setEnabled(bool enable)
{
	m_bEnable = enable;
}	

// 设置是否循环播放
void CNodeAnimation::setLoop(bool loop)
{
	m_bLoop = loop;
}     

// 设置时间位置	
void CNodeAnimation::setTimePosition(double timepos)
{
	double delt = timepos - m_curTime;
	addTime(delt);
}  

void CNodeAnimation::setCurFrame(INT frameIdx)
{
	setTimePosition(frameIdx * gkfrPlaySpeed);
}

// 释放内存
void CNodeAnimation::Release()
{
	for (UINT i = 0; i < m_vpNodeKfr.size(); ++i)
	{
		SAFE_RELEASE(m_vpNodeKfr[i]);
		SAFE_DELETE(m_vpNodeKfr[i]);
	}
	m_vpNodeKfr.clear();
	m_pNode = NULL;
} 


