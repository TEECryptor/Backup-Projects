// Sun3D360 ToolDlg.h : header file
//
//----------------------------------------------------------------
#if !defined(AFX_SUN3D360TOOLDLG_H__64C6501A_D52E_4464_A0B9_925EC3062FD8__INCLUDED_)
#define AFX_SUN3D360TOOLDLG_H__64C6501A_D52E_4464_A0B9_925EC3062FD8__INCLUDED_
//----------------------------------------------------------------
#include <fstream>
//------------------------------------------------------------------
#define MAX_DIV_CNT			8*8			//The max count of division
//------------------------------------------------------------------
//	The box faces define 
//------------------------------------------------------------------
enum
{
	IMAGE_F = 0,			//The front face of the box
	IMAGE_B,				//The black face of the box
	IMAGE_L,				//The left face of the box
	IMAGE_R,				//The right face of the box
	IMAGE_T,				//The top face of the box
	IMAGE_U,				//The under face of the box
	IMAGE_CNT				//The count of the box, is 6
};
//------------------------------------------------------------------
//	How division the big image to small images
//------------------------------------------------------------------
enum
{
	DIS1_1 = 0,				//Do not to divisin the big image
	DIS2_2,					//Division to 2x2 small images
	DIS4_4,					//Division to 4x4 small images
	DIS8_8,					//Division to 8x8 small images
	DIS_CNT					//The count of the division type
};
//------------------------------------------------------------------
//	Name:VGSFileHeader
//	Introduce:The define for *.VGS file header
//------------------------------------------------------------------
typedef struct tagVGSFileHeader
{
	char	strType[4];		//"JPG","ZLB" or "BMP";
	int		iImageWidth;	//The image width in the *.VGS file
	int		iImageHeight;	//The image height in the *.VGS file
	int		iImageCnt;		//The image count in the *.VGS file
	int		iDivType;		//The division type
}VGSFileHeader, *LPVGSFILEHEADER;
//------------------------------------------------------------------
//	Name:FrameDataOffset
//	Introduce:Define the offset of frame data
//------------------------------------------------------------------
typedef struct tagFrameDataOffset
{
	DWORD			bId;		//Specifies the id of frame
	__int64			bOffset;	//Specifies the total offset of the frame data
}FrameDataOffset, *LPFRAMEDATAOFFSET;
//----------------------------------------------------------------
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
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//------------------------------------------------------------
public:
	//------------------------------------------------------------
private:
	//------------------------------------------------------------
	//将图片id转换为字符串
	void		GetCharFormInt(int num, char* pstring);
	//------------------------------------------------------------
	//To get the small images size and width, height
	void		GetSmallImageInfo(int &iCnt, int &width, int &height, int &size);
	//To decode one face image to BMP
	bool		DecodeOneFaceToBmp(int iFrame, int iFace, const char* path, BYTE*& pBMPData);
	//To get the small image BMP data
	void		GetSmallImageData(BYTE* pBmpData, int iBigWidth, int iBigHeight, int iID, int iSmallWidth, int iSmallHeight, BYTE*& pSmallData);
	//Encode the small image data from BMP to JPG
	void		EncodeBmpToJpgBuff(BYTE*& pImage, int iWidth, int iHeight, int &iSize);
	//------------------------------------------------------------
	//显示进度
	void		ShowProgressInfo(int iTotal, int iCurCnt);
	//------------------------------------------------------------
private:
	//------------------------------------------------------------
	int			m_iFormat;			//纹理格式,0-RGB,1-RGBA
	int			m_iDivType;			//How to division the big image to small images
	char		m_strExePath[MAX_PATH];
	//------------------------------------------------------------
};
//----------------------------------------------------------------
#endif // !defined(AFX_SUN3D360TOOLDLG_H__64C6501A_D52E_4464_A0B9_925EC3062FD8__INCLUDED_)
//----------------------------------------------------------------
