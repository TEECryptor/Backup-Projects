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

// �ڵ�Ĺؼ�֡����
class VGS_EXPORT CNodeAnimation  
{
public:
	CNodeAnimation(CSceneNode *pNode, const std::string &sName);
	virtual ~CNodeAnimation();
	
	virtual void   Release();   // �ͷ��ڴ�

	// ����������
	VGS_ANIMATIONTYPE getType();

public:

	std::string getName(){return m_sName;}

	CSceneNode*  getNode();  // ��ö����ڵ�

	VOID addKeyframe(CNodeKeyFrame *pNodeKfr);
	
	void   setLength(double timeLength);         // ���ö���ʱ�䳤��
	double getLength(){return m_totalLength;}  // ������ʱ�䳤��

	INT    getCurFrame() {return m_curFrame;}
	void   setTimePosition(double timepos);  // ����ʱ��λ��
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
	
	void setStartPlayTime(double timeStart);    // ���ÿ�ʼ���Ŷ�����ʱ��

	void addTime(double passTime);              // �ƶ�ʱ��, ���ŵ�ǰʱ��passTime���һ֡	
	virtual void update();

	void			setEnabled(bool enable);	// �����Ƿ������ؼ�֡��������
	bool            getEnabled(){return m_bEnable;}

	void			setLoop(bool loop);	        // �����Ƿ�ѭ������
	bool            getLoop(){return m_bLoop;}
		

public:

	std::string                   m_sName;
	CSceneNode*                   m_pNode;     // �����ڵ�
	std::vector <CNodeKeyFrame*>  m_vpNodeKfr;  // �ؼ�֡�б�

	double   m_totalLength;   // ��������ʱ�䳤��
	double   m_startPlayTime;     // ��ʼ���ŵ�ʱ��

	double   m_curTime;   // ��ǰʱ��
	INT      m_curFrame;  // ��ǰ֡

	bool     m_bEnable;   // �Ƿ���Բ���
	bool     m_bLoop;     // �Ƿ�ѭ������
	VGS_ANIMATIONTYPE m_kfrType;  // �ؼ�֡��������

};

#endif // !defined(AFX_NODEANIMATION_H__0943F806_7C2F_4731_93AE_505CE1EEB13B__INCLUDED_)
