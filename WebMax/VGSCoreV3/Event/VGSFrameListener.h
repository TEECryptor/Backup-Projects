
#pragma once

#include "..\VGSHeader.h"

struct FrameEvent
{
	// 距离上一次event的时间间隔
	double timeSinceLastEvent;
	
	// 距离上一帧的时间间隔
	double timeSinceLastFrame;
};


// VGS的事件监听器接口
class CFrameListener
{
public:

	// 渲染一帧图像之前被调用的接口
	virtual bool frameStarted(const FrameEvent& evt) { return true; }

	// 渲染完一帧图像之后被调用的接口
	virtual bool frameEnded(const FrameEvent& evt) { return true; }
	
	virtual ~CFrameListener() {}
	
 };