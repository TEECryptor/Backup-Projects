//--------------------------------------------------
// RussiaServerDlg.h : header file
//
//--------------------------------------------------
#if !defined(AFX_RUSSIASERVERDLG_H__A36785D8_A3D2_4EAD_A0B0_178E9A64CAB9__INCLUDED_)
#define AFX_RUSSIASERVERDLG_H__A36785D8_A3D2_4EAD_A0B0_178E9A64CAB9__INCLUDED_
//--------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------
#include "CommonDef.h"
#include "ADODatabase.h"
#include "ADOTable.h"
#include "ServerSocket.h"
#include <stdlib.h>
//--------------------------------------------------
using namespace std; 
//--------------------------------------------------
// define software version, initially is v1.0
#define SOFTWARE_VERSION	"V1.0"
// define system No, this is maybe increased later
#define	SYSTEM_NO			1
//--------------------------------------------------
#define MIN_CLIENT_NO			201		//The client ID starting index
#define CLIENT_NO_CNT			15		//The total clients count
#define MAX_CLIENT_NO			15
#define MAX_INNING_PER_ROUND	70
#define LOSS_RATIO				6		//When green result, the banker should pay for loss_ratio size to player
#define	BETMULTIPLE				10		//Bet Max = BETMULTIPLE * Bet Min
//
#define	SHOWRESULT_TIMELEN		3		//The time length for clients to show this game end
#define	SHOWWINVAL_TIMELEN		5		//The time length for clients to show win value information
//--------------------------------------------------
/*	Definitions for table turn animate*/
#define ANG_TO_RAD				3.1415926f/180.0f	
//
#define	TABLE_INIT_ANGLE_SPEED	8.0f
#define	TABLE_TOTAL_ANGLE		2400.0f	
#define	ANGLE_BALL_TO_STOP		6*360
//
#define	BALL_INIT_POS			512.0f
#define	BALL_INIT_SPEED			30.0f
#define	BALL_DOWN_FRM			30
#define	BALL_TURN_R				320
#define	BALL_TURN_MAX_R			350
#define	BALL_TURN_MIN_R			320
#define	BALL_TURN_POS			0.0f
#define	BALL_TURN_SPEED			20.0f
#define BALL_TURN_TOTAL_ANGLE	630
//--------------------------------------------------
//	Name:ClientState
//	Introduce:The client state type definition
//--------------------------------------------------
typedef enum eClientState
{
	CS_UNCONNECT= 0,		//This client has not connected
	CS_CONNECTED= 1,		//This client has been connected, means it can be actived
	CS_ACTIVED	= 2,		//This client has been actived, means has users use it now
	CS_SUBMITTED= 3,		//This client has submitted the bet information
	CS_RENDERING= 4,		//This client rendering
	CS_RENDEREND= 5,		//This client has rendered finish.
	CS_COUNT
}ClientState;
//--------------------------------------------------
//	Name:ServerState
//	Introduce:The server state type definition
//--------------------------------------------------
typedef enum eServerState
{
	SS_READY	= 0,		//Server is loaded
	SS_BETTING	= 1,		//Start to betting
	SS_TURNING	= 2,		//Start to table turning animate
	SS_SHOWING	= 3,		//Start to showing the game result
	SS_END		= 4,		//Current game is end, and ready to start the next
	SS_COUNT
}ServerState;
//--------------------------------------------------
//	Name:BetInfo
//	Introduce:The bet information definition
//---------------------------------------------------
typedef struct tagBetInfo
{
	int		iBetOnRed;
	int		iBetOnBlack;
	int		iBetOnGreen;
}BetInfo, *LPBETINFO;
//---------------------------------------------------
//	Name:ClientCurInfo
//	Introduce:The client current information definition
//---------------------------------------------------
typedef struct tagClientCurInfo
{
	UINT		uClientID;		//201 -- 215
	BetInfo		stuBetInfo;		//The current bet information
	int			iRemainVal;		//The current value
	ClientState	eState;			//The current state
}ClientCurInfo, *LPCLIENTCURINFO;
//--------------------------------------------------
//	Name:TimerType
//	Introduce:Timer type definition
//---------------------------------------------------
typedef enum eTimerType
{
	TT_CHECKTIMER	= 0,
	TT_BETTIMER		= 1,
	TT_RENDER		= 2,
	TT_SHOWRESULT	= 3,
	TT_SHOWWINVAL	= 4,
	TT_NEW_ROUND	= 5,
	TT_REST 		= 6,
	TT_COUNT
}TimerType;
//--------------------------------------------------
typedef enum
{
	ADD_SCORE = 0,		/* add score to user */
	SUB_SCORE,			/* subtract score to user */
	WIN_SCORE,			/* win score number, maybe<0 means lose */
	BET_SCORE_RED,		/* bet red */
	BET_SCORE_BLACK,	/* bet black */
	BET_SCORE_GREEN		/* bet green */
}UserActionType_t;
//--------------------------------------------------
typedef enum
{
	RED = 0,		
	BLACK,		
	GREEN
}GameRstColor_t;
//--------------------------------------------------
//	Name:CRussiaServerDlg
//	Introduce:CRussiaServerDlg dialog
//--------------------------------------------------
class CRussiaServerDlg : public CDialog
{
// Construction
public:
	//----------------------------------------------
	CRussiaServerDlg(CWnd* pParent = NULL);	
	virtual ~CRussiaServerDlg();
	//----------------------------------------------
// Dialog Data
	//{{AFX_DATA(CRussiaServerDlg)
	enum { IDD = IDD_RUSSIASERVER_DIALOG };
	CString	m_strAddValClientID;
	CString	m_strSubValClientID;
	int		m_nAddVal;
	int		m_nSubVal;
	int		m_iBetTime;
	int		m_iMaxBlackBet;
	int		m_iMaxGreenBet;
	int		m_iMaxRedBet;
	int		m_iPerInningTime;
	int		m_iBetSetIndex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRussiaServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	//----------------------------------------------
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRussiaServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonPause();
	afx_msg void OnButtonQueryRst();
	afx_msg void OnButtonPrintBill();
	afx_msg void OnButtonQueryScoreOP();
	afx_msg void OnButtonShutdownClient();
	afx_msg void OnButtonExitSystem();
	afx_msg void OnButtonAddscore();
	afx_msg void OnButtonSubtractScore();
	afx_msg void OnButtonConfirmPara();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//----------------------------------------------
public:
	void ReadData2QueryDlgList(CListCtrl* plst);
	//----------------------------------------------
	//SKT_MSG_CLIENTCONNECTED message handler
	HRESULT		OnClientConnected(WPARAM wParam, LPARAM lParam);
	//SKT_MSG_RECEIVEDMSG message handler
	HRESULT		OnRecievedMsg(WPARAM wParam, LPARAM lParam);
	//----------------------------------------------
private:
	// Initialise user data when boot
	void InitUserData();
	// update the listcontrl show	
	void UpdateList(UINT uClient);	
	// add data to GameResultInfo Table of dbase
	void SaveGameRstInfo2DB(CTime ct);
	// add data to ClientHistoryInfo Table of dbase
	void SaveHistroyInfo2DB(CString strClientID, int preVal, int winVal, CTime ct);
	int GenerateOneRound();
	//----------------------------------------------

	//----------------------------------------------
	//Change the server state
	void		ChangeServerState(UINT uState);
	//Update the client current value information
	BOOL		UpdateUserValue(UINT uClientID, UserActionType_t eActionType, int iActionVal);
	//Save current user value information to databse
	BOOL		SaveCurValuInfo2DB(CString strClientID, int iCurValue);
	//Add data to UserValueInfo Table of dbase
	BOOL		SaveUserValInfo2DB(CString strClientID, int iOldValue, UserActionType_t action, int iActionValue, int iNewValue, CTime ct);	
	//----------------------------------------------
	//To check a client ID is legal or not
	BOOL		ClientIDIsLegal(UINT uClientID);
	//To create the network connect
	BOOL		CreateNetwork();
	//To deal with client information message
	int			ProcClientInfo(LPCLIENTINFO lpClientInfo);
	//To deal with the client command that ask for server to active it, wangdapeng add 2007/01/19
	int			ProcAskActiveCmd(LPASKFORACTIVECMD lpAskActiveCmd);
	//To deal with the client has been actived command
	int			ProcHasActivedCmd(LPHASACTIVEDCMD lpHasActivedCmd);
	//To deal with client submitted bet information
	int			ProcSubmitBetInfo(LPSUBMITBETINFO lpSubmitBetInfo);
	//To deal with client render end command
	int			ProcRenderEndCmd(LPRENDERENDCMD lpRenderEndCmd);
	//Render game animate
	void		RenderTableTurn();
	//Render game result
	void		RenderGameResult();
	//To calc table turn params
	void		CalcTableTurnParams();
	//Calc current turn table angle
	BOOL		CalcCurTableAngle(float &fCurTableAngleSpeed, float &fAngle);
	//Calc current ball position
	void		CalcCurBallPos(float fCurTableAngleSpeed, float &fPosX, float &fPosY);
	//Print one round result
	void 		PrintOneRound();
	//----------------------------------------------
private:										
	int				m_curRoundNo;			// current round No.
	int				m_curInningNo;			// current inning No.
	int				m_nNumOfCurInning;		// total inning num in one round.
	int				m_curRoundRstArray[MAX_INNING_PER_ROUND];  //save all the result number of every inning
	int				m_curInningRstNo;
	int				m_iBetMin;				//下注额(小)
	int				m_iBetMax;				//下注额(大), 大 = 小*10
	//
	//
	CADODatabase	m_Connection;			//The database connect
	CADOTable		m_Recordset;			//A table record set
	//
	TCHAR			m_szDataBase[MAX_PATH];	//Database full file name string
	TCHAR			m_szIPAdress[16];		//Server computer IP address string
	const UINT		m_uPort;				//Port ID, can NOT been changed
	CServerSocket*	m_pServerSocket;		//The socket pointer
	//
	ServerState		m_eCurState;			//Current server state
	int				m_iCurBetTimeLen;		//Current betting operation time length
	BetInfo			m_stuTotalBetInfo;		//Current total bet in one inning, this value should be reset to 0 before a new inning begin
	ClientCurInfo	m_clientData[CLIENT_NO_CNT];	// temperary store player data and if one inning over, store to database.
	int				m_iTotalSubmitCnt;		//The count for clients has submitted in current game
	BOOL			m_bCanRenderNextFrame;	//
	int				m_iTotalRenderEndCnt;	//The count for clients has rendered end in current
	int				m_iCurShowResTimeLen;	//The time length for show game result
	int				m_iCurShowValTimeLen;	//The time length for show win value information
	int				m_iRestTimeLen;			//The time length that the server should wait before the next inning or round begin
	//----------------------------------------------
	BOOL			m_bRenderBusy;			//The render function is busy
	float			m_fTableTotalFrm;		//Table turn total frame count
	float			m_fAngleSpeedDelta;		//The angle speed delta for table
	float			m_fCurTableSpeed;		//The current angle speed for table
	float			m_fCurTableAngle;		//The current angle for table
	//
	float			m_fCurBallLineSpeed;	//The line speed for ball downing
	float			m_fBallLineSpeedDelta;	//The line speed delta for ball downing
	float			m_fCurBallPosY;			//The current ball pos for downing
	float			m_fBallAngleSpeedDelta;	//The angle speed delta for ball turning
	float			m_fCurBallAngleSpeed;	//The current angle speed for ball turning
	float			m_fCurBallAngle;		//The current angle for ball turning
	float			m_fCurBallFrm;			//Waiting for deleting
	float			m_fBallStopFrm;			//Waiting for deleting
	//
	BOOL			m_bGameResultIsSent;	//Current game result is sent to clients
	//
	float			m_fCurBallRadius;
//----------------------------------------------
};
//--------------------------------------------------
#endif // !defined(AFX_RUSSIASERVERDLG_H__A36785D8_A3D2_4EAD_A0B0_178E9A64CAB9__INCLUDED_)
//--------------------------------------------------
