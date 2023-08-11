#pragma once

#include "..\VGSHeader.h"
#include "VGSMainFrameListener.h"
#include "..\Root.h"

// 全局的帧监听器
// CMainFrameListener* g_pMainFrameListener = NULL;


CMainFrameListener::CMainFrameListener() : CFrameListener()
{

}
CMainFrameListener::~CMainFrameListener()
{
}

// 渲染一帧图像之前被调用的接口
bool CMainFrameListener::frameStarted(const FrameEvent& evt)
{
	// 播放动画
	m_pRoot->GetAnimManager().PlayOneFrame(evt.timeSinceLastFrame);

	return true;
}

// 渲染完一帧图像之后被调用的接口
bool CMainFrameListener::frameEnded(const FrameEvent& evt)
{
	return true;
}
