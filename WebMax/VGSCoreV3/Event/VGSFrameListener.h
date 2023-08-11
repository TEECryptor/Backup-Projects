
#pragma once

#include "..\VGSHeader.h"

struct FrameEvent
{
	// ������һ��event��ʱ����
	double timeSinceLastEvent;
	
	// ������һ֡��ʱ����
	double timeSinceLastFrame;
};


// VGS���¼��������ӿ�
class CFrameListener
{
public:

	// ��Ⱦһ֡ͼ��֮ǰ�����õĽӿ�
	virtual bool frameStarted(const FrameEvent& evt) { return true; }

	// ��Ⱦ��һ֡ͼ��֮�󱻵��õĽӿ�
	virtual bool frameEnded(const FrameEvent& evt) { return true; }
	
	virtual ~CFrameListener() {}
	
 };