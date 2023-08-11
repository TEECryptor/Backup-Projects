// TestEditDlg.h : ͷ�ļ�
//

#pragma once

#include "MyEdit.h"
#include "MyEdit1.h"
#include "InfoView.h"


// CTestEditDlg �Ի���
class CTestEditDlg : public CDialog
{
// ����
public:
	CTestEditDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTEDIT_DIALOG };

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
	CMyEdit*	m_pMyEdit;
	CMyEdit1*	m_pMyEdit1;
	CInfoView*	m_pInfoView;
public:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnFont();
	afx_msg void OnBnClickedBtnColor();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
};
