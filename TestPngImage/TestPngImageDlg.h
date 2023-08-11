// TestPngImageDlg.h : 头文件
//

#pragma once

#include "GDIPlusImage.h"
#include <atlImage.h>
#include "gdiplus.h"
using namespace Gdiplus;


// CTestPngImageDlg 对话框
class CTestPngImageDlg : public CDialog
{
// 构造
public:
	CTestPngImageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTPNGIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	ULONG_PTR		m_GdiplusToken;
	CGDIPlusImage*	m_pImage;
public:
	afx_msg void OnDestroy();
};
