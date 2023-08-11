// Sun3D360 ToolDlg.h : header file
//
//----------------------------------------------------------------
#if !defined(AFX_SUN3D360TOOLDLG_H__64C6501A_D52E_4464_A0B9_925EC3062FD8__INCLUDED_)
#define AFX_SUN3D360TOOLDLG_H__64C6501A_D52E_4464_A0B9_925EC3062FD8__INCLUDED_
//----------------------------------------------------------------
#include <fstream>
#include "Sun3D360Define.h"
//------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------
class CSun3D360ToolDlg : public CDialog
{
// Construction
public:
	CSun3D360ToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSun3D360ToolDlg)
	enum { IDD = IDD_SUN3D360TOOL_DIALOG };
	int		m_iTotalFileCnt;
	CString	m_strFileName;
	CString	m_strFloder;
	int		m_iHeight;
	int		m_iWidth;
	CString	m_strCurNum;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSun3D360ToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSun3D360ToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnFloder();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnOk();
	afx_msg void OnEditchangeCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//------------------------------------------------------------
public:
	//------------------------------------------------------------
private:
	//写S3D文件头
	BOOL		WriteS3DFileHeader(fstream &out);
	//------------------------------------------------------------	
	//将图片id转换为字符串
	void		GetCharFormInt(int num, char* pstring);
	//------------------------------------------------------------
	//To get the file size 
	int			GetImageFileSize(int id, int face, const char* path);
	//To get the file data
	bool		LoadImageFileData(int id, int face, const char* path, char*& data, long &cnt);
	//Write to one VGS file
	void		WriteToVGSFile(fstream& out, long size, char* data);
	//------------------------------------------------------------
	//显示进度
	void		ShowProgressInfo(int iTotal, int iCurCnt);
	//------------------------------------------------------------
private:
	//------------------------------------------------------------
	int			m_iFormat;			//纹理格式,0-RGB,1-RGBA
	//------------------------------------------------------------
	int			m_iExType;			//0:To one VGS file, 1:To six VGS files
	//------------------------------------------------------------
};
//----------------------------------------------------------------
#endif // !defined(AFX_SUN3D360TOOLDLG_H__64C6501A_D52E_4464_A0B9_925EC3062FD8__INCLUDED_)
//----------------------------------------------------------------
