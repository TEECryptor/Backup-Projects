// TestMidiPlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTestMidiPlayerApp:
// �йش����ʵ�֣������ TestMidiPlayer.cpp
//

class CTestMidiPlayerApp : public CWinApp
{
public:
	CTestMidiPlayerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestMidiPlayerApp theApp;
