#pragma once

#include "..\VGSHeader.h"
#include "VGSMainFrameListener.h"
#include "..\Root.h"

// ȫ�ֵ�֡������
// CMainFrameListener* g_pMainFrameListener = NULL;


CMainFrameListener::CMainFrameListener() : CFrameListener()
{

}
CMainFrameListener::~CMainFrameListener()
{
}

// ��Ⱦһ֡ͼ��֮ǰ�����õĽӿ�
bool CMainFrameListener::frameStarted(const FrameEvent& evt)
{
	// ���Ŷ���
	m_pRoot->GetAnimManager().PlayOneFrame(evt.timeSinceLastFrame);

	return true;
}

// ��Ⱦ��һ֡ͼ��֮�󱻵��õĽӿ�
bool CMainFrameListener::frameEnded(const FrameEvent& evt)
{
	return true;
}
