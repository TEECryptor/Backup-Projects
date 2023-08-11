// TestPngImageDlg.h : ͷ�ļ�
//

#pragma once

#include "GDIPlusImage.h"
#include <atlImage.h>
#include "gdiplus.h"
using namespace Gdiplus;


// CTestPngImageDlg �Ի���
class CTestPngImageDlg : public CDialog
{
// ����
public:
	CTestPngImageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTPNGIMAGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
