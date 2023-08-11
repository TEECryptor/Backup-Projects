#pragma once

#include "..\VGSHeader.h"
#include "VGSFrameListener.h"
#include "..\VGSGetRoot.h"

class CMainFrameListener : public CFrameListener, public CVGSGetRoot
{
public:
	
	// ��Ⱦһ֡ͼ��֮ǰ�����õĽӿڡ��������false�����򽫲������������Ⱦ
	bool frameStarted(const FrameEvent& evt);
	
	// ��Ⱦ��һ֡ͼ��֮�󱻵��õĽӿڡ��������false�����򽫲������������Ⱦ
	bool frameEnded(const FrameEvent& evt);

public:
	
	CMainFrameListener();
	~CMainFrameListener();
};

// extern CMainFrameListener* g_pMainFrameListener;