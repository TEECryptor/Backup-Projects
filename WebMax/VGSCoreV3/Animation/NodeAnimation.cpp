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

	m_totalLength = 0;				// ������ʱ������Ϊ��λ
	m_startPlayTime = 0;			// �ؼ�֡������ʼ���ŵ�ʱ��

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

// ����������
VGS_ANIMATIONTYPE CNodeAnimation::getType()
{
	return m_kfrType;
}
// ��ö����ڵ�
CSceneNode*  CNodeAnimation::getNode()
{
	return m_pNode;
}

// ����һ֡����
VOID CNodeAnimation::addKeyframe(CNodeKeyFrame *pNodeKfr)
{
	if (m_kfrType == VAT_ANIMATIONTYPE_NONE && pNodeKfr)
	{
		m_kfrType = pNodeKfr->getType();
	}
	m_vpNodeKfr.push_back(pNodeKfr);
}

// ���ö���ʱ�䳤��
void CNodeAnimation::setLength(double timeLength)
{
	m_totalLength = timeLength;
}         

// ���ÿ�ʼ���Ŷ�����ʱ��
void CNodeAnimation::setStartPlayTime(double timeStart)
{
	m_startPlayTime = timeStart;
}    

// �ƶ�ʱ��, ���ŵ�ǰʱ��passTime���һ֡
void CNodeAnimation::addTime(double passTime)
{	
	m_curTime += passTime;  // ���µ�ǰʱ��
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
	
	//�ҵ���ǰʱ���Ӧ��֡
	if (m_curTime <= m_startPlayTime)
	{
		m_curFrame = 0;
	}
	else
	{
		m_curFrame = (INT)((m_curTime - m_startPlayTime) * gkfrPlaySpeed);
	}

	// ��ǰ֡��ҪԽ��
	m_curFrame = max(0, min((INT)m_curFrame, m_vpNodeKfr.size() - 1)); 
	
	if (!m_bEnable) 
	{
		return;
	}

	if (m_vpNodeKfr[m_curFrame])
	{
		m_vpNodeKfr[m_curFrame]->update();  // ����֡״̬
	}
}            	

// �����Ƿ������ؼ�֡��������
void CNodeAnimation::setEnabled(bool enable)
{
	m_bEnable = enable;
}	

// �����Ƿ�ѭ������
void CNodeAnimation::setLoop(bool loop)
{
	m_bLoop = loop;
}     

// ����ʱ��λ��	
void CNodeAnimation::setTimePosition(double timepos)
{
	double delt = timepos - m_curTime;
	addTime(delt);
}  

void CNodeAnimation::setCurFrame(INT frameIdx)
{
	setTimePosition(frameIdx * gkfrPlaySpeed);
}

// �ͷ��ڴ�
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


