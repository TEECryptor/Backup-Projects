#pragma once

#include "..\VGSHeader.h"
#include "VGSFrameListener.h"
#include "..\VGSGetRoot.h"

class CMainFrameListener : public CFrameListener, public CVGSGetRoot
{
public:
	
	// 渲染一帧图像之前被调用的接口。如果返回false，程序将不继续下面的渲染
	bool frameStarted(const FrameEvent& evt);
	
	// 渲染完一帧图像之后被调用的接口。如果返回false，程序将不继续下面的渲染
	bool frameEnded(const FrameEvent& evt);

public:
	
	CMainFrameListener();
	~CMainFrameListener();
};

// extern CMainFrameListener* g_pMainFrameListener;