//---------------------------------------------------------------
//	Name:RussiaClientDlg.h
//	Introduce: The CRussiaClientDlg class definition
//	Creator:Youfang Yao
//	Date:12-10-06
//---------------------------------------------------------------
#if !defined(AFX_RUSSIACLIENTDLG_H__F0D57093_8179_4C26_8356_5E2B0D287209__INCLUDED_)
#define AFX_RUSSIACLIENTDLG_H__F0D57093_8179_4C26_8356_5E2B0D287209__INCLUDED_
//---------------------------------------------------------------
#include "MediaLib.h"
#include "RussiaClientDef.h"
#include "CommonDef.h"
#include "ClientSocket.h"
#include <vector>
//---------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------
#define	SCENE_WIDTH			1280
#define	SCENE_HEIGHT		1024
#define ANG_TO_RAD			3.1415926f/180.0f			
//---------------------------------------------------------------
// CRussiaClientDlg dialog
//---------------------------------------------------------------
class CRussiaClientDlg : public CDialog
{
	//-----------------------------------------------------------
public:
	CRussiaClientDlg(CWnd* pParent = NULL);
	virtual ~CRussiaClientDlg();
	//-----------------------------------------------------------
// Dialog Data
	//{{AFX_DATA(CRussiaClientDlg)
	enum { IDD = IDD_RUSSIACLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRussiaClientDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	//-----------------------------------------------------------
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRussiaClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//-----------------------------------------------------------
public:
	//SKT_MSG_RECEIVEDMSG message handle
	HRESULT		OnReceiveMsg(WPARAM wParam, LPARAM lParam);
	//-----------------------------------------------------------
private:	
	//When received a SMT_STC_GETCLIENTINFO message data, call this function to return the client information
	int			ProcGetClientInfoCmdData(LPGETCLIENTINFO lpGetClientInfo);
	//When received a SMT_STC_SERVERSETINFO message data, call this function to deal with it
	int			ProcServerSetInfoData(LPSERVERSETINFO lpServerSetInfo);
	//When received a SMT_STC_ACTIVECMD message data, call this function to deal with this data buffer
	int			ProcActiveCmdData(LPACTIVEINFO lpActiveInfo);
	//When received a SMT_STC_USERVALUEINFO message, call this function to deal with this data buffer
	int			ProcUserValueInfo(LPUSERVALUEINFO lpUserValueInfo);
	//When received a SMT_STC_STARTBET message, call this function to deal with this command
	int			ProcStartBetCmd(LPSTARTBETCMD lpStartBetCmd);
	//When received a SMT_STC_TOTALBETINFO message, call this function to deal with this data buffer
	int			ProcTotalBetInfo(LPTOTALBETINFO lpTotalBetInfo);
	//When received a SMT_STC_ENDBET message, call this function to deal with this command
	int			ProcEndBetCmd(LPENDBETCMD lpEndBetCmd);
	//When received a SMT_STC_RENDERCMD message, call this function to deal with this data
	int			ProcRenderCmdData(LPRENDERINFO lpRenderInfo);
	//When received a SMT_STC_GAMERESULT message, call this function to deal with this data
	int			ProcGameResultInfo(LPGAMERESULTINFO lpGameResult);
	//When received a SMT_STC_SHOWSULT message, call this function to deal with this data
	int			ProcShowResultCmd(LPSHOWRESULTCMD pShowResCmd);
	//When received a SMT_STC_GAMEEND cmd, call this function to deal with this data
	int			ProcGameEndCmd(LPGAMEENDCMD lpGameEndCmd);
	//When received a SMT_STC_WINVALUE message, call this function to deal with this data
	int			ProcWinValueInfo(LPWINVALUEINFO lpWinValueInfo);
	//When received a SMT_STC_GAMEOVER message, call this function to deal with this command
	int			ProcGameOver(LPGAMEOVERCMD lpGameOverCmd);
	//-----------------------------------------------------------
	//User selected OK button
	void		ClickOKBtn();
	//User selected bet operation
	void		BetValOperation(UINT uBetType);
	//Undo the bet value operation
	void		UndoBetValOperation();
	//Update the result flag information on UI
	void		UdpdateResultFlg(int iIndex, UINT uFlg, BOOL bVisible);
	//Update the bet value limit information on UI
	void		UpdateLimitBetInfo(int iMaxBetRed, int iMaxBetBlack, int iMaxBetGreen);
	//Update the current total bet value number information on UI
	void		UpdateTotalBetInfo(int iTotalBetRed, int iTotalBetBlack, int iTotalBetGreen);
	//Update the current time number object information on UI
	void		UpdateTimeNum(int iTimeNum);
	//Update the current bet value number information on UI
	void		UpdateCurBetInfo(int iCurBetRed, int iCurBetBlack, int iCurBetGreen);
	//Update the chip objects image on UI
	void		UpdateChipObjs(UINT uObjType, UINT uImageType);
	//Get the chip values type
	UINT		GetChipValType(int iChipVal);
	//Update current value number information on UI
	void		UpdateCurValue(UINT eValType, int iCurVal);
	//-----------------------------------------------------------
private:
	//convert point (X) in window to scene
	int			WndPosToScene_X(int iWndX);
	//convert point (Y) in window to scene
	int			WndPosToScene_Y(int iWndY);
	//Read the config information
	void		ReadConfigInfo(LPCTSTR lpszIniFile);
	//Try to connect the server
	BOOL		ConnectServer();
	//To load the scene resurce
	BOOL		LoadScene();
	//To create the head flash
	BOOL		CreateHeadFlash();
	//Create the betting scene
	BOOL		CreateBetScene();
	//Create the turn table scene
	BOOL		CreateTurnScene();
	//Get the num texture file name string
	void		GetNumTexFile(int iType, int iIndex, LPTSTR lpszName);
	//Load number object texture files
	void		LoadNumTextures();
	//To play the head flash
	void		PlayHead();
	//To render the bet scene 
	void		RenderBetScene();
	//To render the turn table scene
	void		RenderTurnScene();
	//Update the ball images
	void		UpdateBallImage(BOOL bReset = FALSE);
	//-----------------------------------------------------------
	//To return the result flag objects rect
	void		GetResultFlgObjRect(int iIndex, RECT &rc);
	//Create the result flag objcets
	void		CreateResultFlgObj();
	//To return the limit number object name string
	void		GetLimitNumObjName(Color_Type eColor, int iIndex, LPTSTR lpszName);
	//Create the limit number objects
	void		CreateLimitNumObj();
	//To return current total bet value number object name string
	void		GetTotalBetNumObjName(Color_Type eColor, int iIndex, LPTSTR lpszName);
	//Create current total bet value number objects
	void		CreateTotalBetNumObj();
	//Create time number objects
	void		CreateTimeNumObj();
	//To return current bet value number object name string
	void		GetBetValNumObjName(Color_Type eColor, int iIndex, LPTSTR lpszName);
	//Create current bet value number objects
	void		CreateBetValNumObj();
	//Create the chips objects
	void		CreateChipsObj();
	//Load the chips object textures
	void		LoadChipsTextures();
	//To return current user value number object name string
	void		GetUserValNumObjName(UINT uType, int iIndex, LPTSTR lpszName);
	//Create current user value number objects
	void		CreateUserValNumObj();
	//Create client ID number objects
	void		CreateClientIDNumObj();
	//-----------------------------------------------------------
private:
	CClientSocket*	m_pSocket;
	TCHAR			m_szServerIP[16];
	UINT			m_uServerPort;
	UINT			m_uClientID;
	//
	TCHAR			m_szSoundPath[MAX_PATH];
	CFlashPlayer*	m_pHeadFlash;
	CSurfaceScene*	m_pBetScene;
	CSurfaceScene*	m_pTurnScene;
	Client_State	m_eCurState;
	//
	UINT			m_uUIStype;					//The stype of UI, horizontal or vertical
	ServerSetInfo	m_stuServerSetting;			//The server params setting
	int				m_iCurTimeNum;				//Current time number for betting
	float			m_fCurValue;
	float			m_fWinValue;
	TotalBetInfo	m_stuCurTotalBetInfo;
	SubmitBetInfo	m_stuUserBetInfo;
	RenderInfo		m_stuCurRenderInfo;
	//
	BOOL			m_bIsAddStep;
	float			m_fAngleStep;
	float			m_fCurAngle;
	//
	GameResultInfo	m_stuGameResult;				//Current game result
	UINT			m_uGameResult;					//Current game result
	ResultFlg		m_eGameResultFlg;				//Current game result flag, Red/Black/Green
	//-----------------------------------------------------------
	RECT			m_stuMiniTurnRect[UIS_COUNT];			//The rect for mini turntable object
	RECT			m_stuBigTurnRect[UIS_COUNT];			//The rect for big turntable object
	int				m_iLimitObjWidth[UIS_COUNT];			//Limit number objects size definition
	int				m_iLimitObjHeight[UIS_COUNT];			//Limit number objects size definition
	int				m_iLimitObjRight[UIS_COUNT];			//Limit number objects right definition
	int				m_iLimitObjTop[UIS_COUNT][CT_COUNT];	//Limit number objects top definition
	//
	int				m_iTotalBetObjWidth[UIS_COUNT];			//Total bet value number objects size definition
	int				m_iTotalBetObjHeight[UIS_COUNT];		//Total bet value number objects size definition
	int				m_iTotalBetObjRight[UIS_COUNT];			//TotaL bet value number objects right definition
	int				m_iTotalBetObjTop[UIS_COUNT][CT_COUNT];	//TotaL bet value number objects top definition
	//
	int				m_iTimeNumObjRight[UIS_COUNT];			//Time number objects right definition
	int				m_iTimeNumObjTop[UIS_COUNT];			//Time number objects top definition
	int				m_iBetValObjTop[UIS_COUNT];				//Current bet value number objects top definition
	int				m_iBetValObjRight[UIS_COUNT][CT_COUNT];	//Current bet value number objects right definition
	int				m_iChipObjTop[UIS_COUNT];				//The chip objects top definition
	int				m_iChipObjLeft[UIS_COUNT][CHIPOBJ_CNT];	//The chip objects left definition
	int				m_iUserValObjTop[UIS_COUNT];			//Current user value number objects top definition
	int				m_iUserValObjRight[UIS_COUNT][CVT_COUNT];//Current user value number objects right definition
	int				m_iClientIDObjTop[UIS_COUNT];			//Client ID number object top definition
	int				m_iClientIDObjRight[UIS_COUNT];			//Client ID number object right definition
	//
	RECT			m_stuMiniBallRect[UIS_COUNT];			//The mini ball rect
	RECT			m_stuBigBallRect[UIS_COUNT];			//The big ball rect
	RECT			m_stuResCharRect[UIS_COUNT];			//The game result chars rect
	//-----------------------------------------------------------
	CMusic*			m_pBKMusic;			//The BK music
	CMusic*			m_pRollMusic;		//The roll ball music
	CMusic*			m_pBetCoinMusic;	//The bet coin music
	CMusic*			m_pClockMusic;		//The clock music
	//-----------------------------------------------------------
	vector<LPBETUNDO>	m_lstBetUndo;
	//-----------------------------------------------------------
};
//---------------------------------------------------------------
#endif // !defined(AFX_RUSSIACLIENTDLG_H__F0D57093_8179_4C26_8356_5E2B0D287209__INCLUDED_)
//---------------------------------------------------------------
