// TestD3DDlg.h : header file
//

#if !defined(AFX_TESTD3DDLG_H__4E3F8660_9B6A_4BA1_BDD2_174503A00D82__INCLUDED_)
#define AFX_TESTD3DDLG_H__4E3F8660_9B6A_4BA1_BDD2_174503A00D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestD3DDlg dialog
#include <d3d9.h>
#include "RectObject.h"
#include "D3DEngine.h"

class CTestD3DDlg : public CDialog
{
// Construction
public:
	CTestD3DDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestD3DDlg)
	enum { IDD = IDD_TESTD3D_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestD3DDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestD3DDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//----------------------------------------
private:
	HWND		CreateRenderWnd(int iLeft, int iTop, int iWidth, int iHeight);
	HRESULT		InitD3D(HWND hRenderWnd);
	HRESULT		SetProjectType(UINT uType, float fWidth, float fHeight);
	HRESULT		MoveCamera(float x, float y, float z);
	VOID		CalcCameraPos();
	VOID		Render();
	//----------------------------------------
private:
	HWND			m_hRenderWnd;
	CD3DEngine*		m_pD3DEngine;
	LPDIRECT3D9             m_pD3D;			// Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;	// Our rendering device
	//----------------------------------------
	CRectObject				m_RectObject;
	//----------------------------------------
	float				m_fCameraX;
	float				m_fCameraY;
	float				m_fCameraZ;
	//----------------------------------------
public:
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTD3DDLG_H__4E3F8660_9B6A_4BA1_BDD2_174503A00D82__INCLUDED_)
