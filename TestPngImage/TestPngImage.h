// TestPngImage.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CTestPngImageApp:
// �йش����ʵ�֣������ TestPngImage.cpp
//

class CTestPngImageApp : public CWinApp
{
public:
	CTestPngImageApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestPngImageApp theApp;
