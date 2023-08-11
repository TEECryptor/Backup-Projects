//---------------------------------------------------------------	
//	Name:RussiaClientDlg.cpp
//	Introduce:The implementation file for CRussiaClientDlg
//	Creator:Youfang Yao
//	Date:12-10-06
//---------------------------------------------------------------
#include "stdafx.h"
#include "RussiaClient.h"
#include "RussiaClientDlg.h"
#include <math.h>
//---------------------------------------------------------------
/*	Scene Object Name Definition*/
const TCHAR	g_szTurntable[][16]			= {_T("RT_mini"), _T("RT_big")};
const TCHAR g_szTimeNumName[][16]		= {_T("TimeNum_0"), _T("TimeNum_1")};
const TCHAR g_szChipObjName[][16]		= {_T("Chip_Min"), _T("Chip_Max")};
const TCHAR g_szBallObjName[]			= _T("Ball");
const TCHAR g_szResCharObjName[]		= _T("ResChar");
//---------------------------------------------------------------
/*	Scene Object Texture image path Definition*/
TCHAR		g_szImagePath[MAX_PATH]		= _T("\0");
const TCHAR	g_szNumberPath[]			= _T("Number\\%d\\%d.png");
//---------------------------------------------------------------
/*	Scene Object Texture File Name Definition*/
const TCHAR	g_szHeadFlash[]				= _T("Head.swf");
const TCHAR	g_szBKImage[][MAX_PATH]		= {_T("BK_1.jpg"), _T("BK_2.jpg")};
const TCHAR	g_szBigBKImage[][MAX_PATH]	= {_T("BigBK_1.jpg"), _T("BigBK_2.jpg")};
const TCHAR	g_szTurnImage[][MAX_PATH]	= {_T("russia_mini.png"), _T("russia_big.png")};
const TCHAR g_szStaticChip[][MAX_PATH]  = {_T("Chips\\5\\0.png"), _T("Chips\\10\\0.png"), _T("Chips\\20\\0.png"), 
										   _T("Chips\\50\\0.png"), _T("Chips\\100\\0.png"), _T("Chips\\200\\0.png"),
										   _T("Chips\\500\\0.png"), _T("Chips\\1000\\0.png"), _T("Chips\\2000\\0.png")};
const TCHAR g_szAnimateChip[][MAX_PATH] = {_T("Chips\\5.dtx"), _T("Chips\\10.dtx"), _T("Chips\\20.dtx"), 
										   _T("Chips\\50.dtx"), _T("Chips\\100.dtx"), _T("Chips\\200.dtx"), 
										   _T("Chips\\500.dtx"), _T("Chips\\1000.dtx"), _T("Chips\\2000.dtx")};
const TCHAR g_szResultFlg[][MAX_PATH]	= {_T("ResultFlg\\red.PNG"), _T("ResultFlg\\black.PNG"), _T("ResultFlg\\green.PNG")};
const TCHAR g_szBallImage[][MAX_PATH]	= {_T("Ball\\ball1.png"), _T("Ball\\ball2.png"), _T("Ball\\ball3.png"), _T("Ball\\ball4.png"), 
										   _T("Ball\\ball5.png"), _T("Ball\\ball6.png"), _T("Ball\\ball7.png"), _T("Ball\\ball8.png"),
										   _T("Ball\\ball9.png"), _T("Ball\\ball10.png")};
const TCHAR g_szResultChar[][MAX_PATH]	= {_T("ResultChar\\Red.dtx"), _T("ResultChar\\Black.dtx"), _T("ResultChar\\Green.dtx")};
//---------------------------------------------------------------
/*	Sound files name definition*/
TCHAR		g_szSoundPath[]				= _T("\0");
const TCHAR g_szBKSound[]				= _T("BK.wav"); 
const TCHAR g_szRollSound[]				= _T("Roll.wav"); 
const TCHAR g_szBetCoinSound[]			= _T("SelCoin.wav"); 
const TCHAR g_szClockSound[]			= _T("Clock.wav"); 
//---------------------------------------------------------------


//---------------------------------------------------------------
//	Name:CRussiaClientDlg
//	Introduce:Constructor
//---------------------------------------------------------------
CRussiaClientDlg::CRussiaClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRussiaClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRussiaClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pSocket = NULL;
	lstrcpy(m_szServerIP, _T("\0"));
	m_uServerPort = 200;
	m_uClientID = 0;

	lstrcpy(g_szImagePath, _T("\0"));
	lstrcpy(m_szSoundPath, _T("\0"));
	m_pHeadFlash = NULL;
	m_pBetScene = NULL;
	m_pTurnScene = NULL;
	m_eCurState = CS_NONE;
	m_uUIStype = UIS_H;

	m_stuServerSetting.iMinBet = 10;
	m_stuServerSetting.iMaxBet = 50;
	m_stuServerSetting.iMaxRedBet = 10000;
	m_stuServerSetting.iMaxBlackBet = 10000;
	m_stuServerSetting.iMaxGreenBet = 10000;

	m_iCurTimeNum = 0;
	m_fCurValue = 0.0f;
	m_fWinValue = 0.0f;
	memset(&m_stuCurTotalBetInfo, 0, sizeof(TotalBetInfo));
	memset(&m_stuUserBetInfo, 0, sizeof(SubmitBetInfo));	
	memset(&m_stuCurRenderInfo, 0, sizeof(RenderInfo));

	m_bIsAddStep = TRUE;
	m_fAngleStep = 0.0f;
	m_fCurAngle = 0.0f;

	memset(&m_stuGameResult, 0, sizeof(GameResultInfo));
	m_uGameResult = 0;
	m_eGameResultFlg = RF_RED;

	/*Image object rect definintion*/
	m_stuMiniTurnRect[UIS_H].left = 459;
	m_stuMiniTurnRect[UIS_H].top = 327;
	m_stuMiniTurnRect[UIS_H].right = 821;
	m_stuMiniTurnRect[UIS_H].bottom = 689;
	m_stuMiniTurnRect[UIS_V].left = 459;
	m_stuMiniTurnRect[UIS_V].top = 291;
	m_stuMiniTurnRect[UIS_V].right = 821;
	m_stuMiniTurnRect[UIS_V].bottom = 653;
	//	
	m_stuBigTurnRect[UIS_H].left = 267;
	m_stuBigTurnRect[UIS_H].top = 144;
	m_stuBigTurnRect[UIS_H].right = 1012;
	m_stuBigTurnRect[UIS_H].bottom = 888;
	m_stuBigTurnRect[UIS_V].left = 267;
	m_stuBigTurnRect[UIS_V].top = 144;
	m_stuBigTurnRect[UIS_V].right = 1012;
	m_stuBigTurnRect[UIS_V].bottom = 888;
	//
	m_iLimitObjWidth[UIS_H] = 10;
	m_iLimitObjWidth[UIS_V] = 12;
	m_iLimitObjHeight[UIS_H] = 16;
	m_iLimitObjHeight[UIS_V] = 20;
	m_iLimitObjRight[UIS_H] = 345;
	m_iLimitObjRight[UIS_V] = 330;
	m_iLimitObjTop[UIS_H][CT_RED] = 452;
	m_iLimitObjTop[UIS_H][CT_BLACK] = 504;
	m_iLimitObjTop[UIS_H][CT_GREEN] = 556;
	m_iLimitObjTop[UIS_V][CT_RED] = 252;
	m_iLimitObjTop[UIS_V][CT_BLACK] = 312;
	m_iLimitObjTop[UIS_V][CT_GREEN] = 372;
	//	
	m_iTotalBetObjWidth[UIS_H] = 10;
	m_iTotalBetObjWidth[UIS_V] = 12;
	m_iTotalBetObjHeight[UIS_H] = 16;
	m_iTotalBetObjHeight[UIS_V] = 20;
	m_iTotalBetObjRight[UIS_H] = 1190;
	m_iTotalBetObjRight[UIS_V] = 330;
	m_iTotalBetObjTop[UIS_H][CT_RED] = 452;
	m_iTotalBetObjTop[UIS_H][CT_BLACK] = 504;
	m_iTotalBetObjTop[UIS_H][CT_GREEN] = 556;
	m_iTotalBetObjTop[UIS_V][CT_RED] = 552;
	m_iTotalBetObjTop[UIS_V][CT_BLACK] = 612;
	m_iTotalBetObjTop[UIS_V][CT_GREEN] = 673;
	//
	m_iTimeNumObjTop[UIS_H] = 798;
	m_iTimeNumObjTop[UIS_V] = 785;
	m_iTimeNumObjRight[UIS_H] = 660;
	m_iTimeNumObjRight[UIS_V] = 660;
	//	
	m_iBetValObjTop[UIS_H] = 915;
	m_iBetValObjTop[UIS_V] = 905;
	m_iBetValObjRight[UIS_H][CT_RED] = 170;
	m_iBetValObjRight[UIS_H][CT_BLACK] = 325;
	m_iBetValObjRight[UIS_H][CT_GREEN] = 480;
	m_iBetValObjRight[UIS_V][CT_RED] = 198;
	m_iBetValObjRight[UIS_V][CT_BLACK] = 351;
	m_iBetValObjRight[UIS_V][CT_GREEN] = 503;
	//
	m_iChipObjTop[UIS_H] = 868;
	m_iChipObjTop[UIS_V] = 856;
	m_iChipObjLeft[UIS_H][CT_OBJMIN] = 540;
	m_iChipObjLeft[UIS_H][CT_OBJMAX] = 645;
	m_iChipObjLeft[UIS_V][CT_OBJMIN] = 562;
	m_iChipObjLeft[UIS_V][CT_OBJMAX] = 666;
	//
	m_iUserValObjTop[UIS_H] = 915;
	m_iUserValObjTop[UIS_V] = 905;
	m_iUserValObjRight[UIS_H][CVT_REMAIN] = 1110;
	m_iUserValObjRight[UIS_H][CVT_WINVAL] = 953;
	m_iUserValObjRight[UIS_V][CVT_REMAIN] = 1095;
	m_iUserValObjRight[UIS_V][CVT_WINVAL] = 940;
	//
	m_iClientIDObjTop[UIS_H] = 915;
	m_iClientIDObjTop[UIS_V] = 905;
	m_iClientIDObjRight[UIS_H] = 1207;
	m_iClientIDObjRight[UIS_V] = 1187;
	//
	m_stuMiniBallRect[UIS_H].left = 630;
	m_stuMiniBallRect[UIS_H].top = 630;
	m_stuMiniBallRect[UIS_H].right = 652;
	m_stuMiniBallRect[UIS_H].bottom = 652;
	m_stuMiniBallRect[UIS_V].left = 630;
	m_stuMiniBallRect[UIS_V].top = 594;
	m_stuMiniBallRect[UIS_V].right = 652;
	m_stuMiniBallRect[UIS_V].bottom = 616;
	//
	m_stuBigBallRect[UIS_H].left = 623;
	m_stuBigBallRect[UIS_H].top = 832;
	m_stuBigBallRect[UIS_H].right = 663;
	m_stuBigBallRect[UIS_H].bottom = 872;
	m_stuBigBallRect[UIS_V].left = 623;
	m_stuBigBallRect[UIS_V].top = 832;
	m_stuBigBallRect[UIS_V].right = 663;
	m_stuBigBallRect[UIS_V].bottom = 872;
	//
	m_stuResCharRect[UIS_H].left = 415;
	m_stuResCharRect[UIS_H].top = 287;
	m_stuResCharRect[UIS_H].right = 865;
	m_stuResCharRect[UIS_H].bottom = 737;
	m_stuResCharRect[UIS_V].left = 415;
	m_stuResCharRect[UIS_V].top = 287;
	m_stuResCharRect[UIS_V].right = 865;
	m_stuResCharRect[UIS_V].bottom = 737;
	//
	m_pBKMusic = NULL;		
	m_pRollMusic = NULL;	
	m_pBetCoinMusic = NULL;
	m_pClockMusic = NULL;	
}
//---------------------------------------------------------------
//	Name:~CRussiaClientDlg
//	Introduce:Distructor
//---------------------------------------------------------------
CRussiaClientDlg::~CRussiaClientDlg()
{
}
//---------------------------------------------------------------
void CRussiaClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRussiaClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//---------------------------------------------------------------
BEGIN_MESSAGE_MAP(CRussiaClientDlg, CDialog)
	//{{AFX_MSG_MAP(CRussiaClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(SKT_MSG_RECEIVEDMSG, OnReceiveMsg)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//---------------------------------------------------------------



//---------------------------------------------------------------
//	WM_INITDIALOG
//---------------------------------------------------------------
BOOL CRussiaClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	int iWidth = GetSystemMetrics(SM_CXSCREEN);
	int iHeight = GetSystemMetrics(SM_CYSCREEN);	
	//SetWindowPos(NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
	SetWindowPos(NULL, 0, 0, 800, 636, SWP_SHOWWINDOW);
	//CenterWindow();

	//Get the exe file path
	TCHAR szExePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szExePath, MAX_PATH);
	LPTSTR lpFind = szExePath;
	while(*lpFind != '\0') lpFind++;
	while(*lpFind != '\\') 
	{
		*lpFind = '\0';
		lpFind--;
	}
	lstrcpy(g_szImagePath, szExePath);
	lstrcat(g_szImagePath, _T("Images\\"));
	//
	lstrcpy(g_szSoundPath, szExePath);
	lstrcat(g_szSoundPath, _T("Sounds\\"));

	//Read the config information
	TCHAR szIniFile[MAX_PATH] = {_T("\0")};
	lstrcpy(szIniFile, szExePath);
	lstrcat(szIniFile, _T("RussiaClient.ini"));
	ReadConfigInfo(szIniFile);

	//To connect the server
	if(!ConnectServer())
	{
		MessageBox(_T("Connect server failed!"));
	}

	//Load scene
	if(TRUE == LoadScene())
	{		
		m_eCurState = CS_LOADED;

		UpdateLimitBetInfo(m_stuServerSetting.iMaxRedBet, m_stuServerSetting.iMaxBlackBet, m_stuServerSetting.iMaxGreenBet);
		UpdateTotalBetInfo((int)m_stuCurTotalBetInfo.fTotalBetRedVal, (int)m_stuCurTotalBetInfo.fTotalBetBlackVal, (int)m_stuCurTotalBetInfo.fTotalBetGreenVal);
		UpdateTimeNum(m_iCurTimeNum);
		UpdateCurBetInfo((int)m_stuUserBetInfo.fBetRedVal, (int)m_stuUserBetInfo.fBetBlackVal,(int)m_stuUserBetInfo.fBetGreenVal);
		UpdateChipObjs(CT_OBJMIN, 0);
		UpdateChipObjs(CT_OBJMAX, 0);
		UpdateCurValue(CVT_REMAIN, (int)m_fCurValue);
		UpdateCurValue(CVT_WINVAL, (int)m_fWinValue);
		
		PlayHead();
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//---------------------------------------------------------------
//	WM_DESTROY
//---------------------------------------------------------------
void CRussiaClientDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if(NULL != m_pHeadFlash)
	{
		MediaLib_DestroyFlash(m_pHeadFlash);
		m_pHeadFlash = NULL;
	}
	//
	if(NULL != m_pBetScene)
	{
		MediaLib_DestroyScene(m_pBetScene);
		m_pBetScene = NULL;
	}
	
	if(NULL != m_pBKMusic)
	{
		MediaLib_DestroyMusic(m_pBKMusic);
		m_pBKMusic = NULL;
	}	
	if(NULL != m_pRollMusic)
	{
		MediaLib_DestroyMusic(m_pRollMusic);
		m_pRollMusic = NULL;
	}		
	if(NULL != m_pBetCoinMusic)
	{
		MediaLib_DestroyMusic(m_pBetCoinMusic);
		m_pBetCoinMusic = NULL;
	}		
	if(NULL != m_pClockMusic)
	{
		MediaLib_DestroyMusic(m_pClockMusic);
		m_pClockMusic = NULL;
	}			
}
//---------------------------------------------------------------
void CRussiaClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}
//---------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//---------------------------------------------------------------
void CRussiaClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
//---------------------------------------------------------------
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//---------------------------------------------------------------
HCURSOR CRussiaClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//---------------------------------------------------------------
BOOL CRussiaClientDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			ClickOKBtn();
			break;
		case 'Q':
		case 'q':			//Bet red min value
			BetValOperation(BT_RED_MIN);
			break;
		case 'W':
		case 'w':			//Bet red max value
			BetValOperation(BT_RED_MAX);
			break;
		case 'E':
		case 'e':			//Bet black min value
			BetValOperation(BT_BLACK_MIN);
			break;
		case 'R':
		case 'r':			//Bet black max value
			BetValOperation(BT_BLACK_MAX);
			break;
		case 'T':
		case 't':			//Bet green min value
			BetValOperation(BT_GREEN_MIN);
			break;
		case 'Y':
		case 'y':			//Bet green max value
			BetValOperation(BT_GREEN_MAX);
			break;
		case 'u':
		case 'U':			//Undo bet value
			UndoBetValOperation();
			break;	
		default:
			return CDialog::PreTranslateMessage(pMsg);
		}
		return 1;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
//---------------------------------------------------------------
//	Name:OnKeyDown
//	Introduce:WN_KEYDOWN
//---------------------------------------------------------------
void CRussiaClientDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
//---------------------------------------------------------------
//	Name:OnTimer
//	Introduce:WN_TIMER
//---------------------------------------------------------------
void CRussiaClientDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TT_BETTIMER:		//To change the betting time length
		{
			m_iCurTimeNum -= 1;
			if(m_iCurTimeNum >= 0)
			{
				UpdateTimeNum(m_iCurTimeNum);
			}
			else
			{
				KillTimer(TT_BETTIMER);
			}
		}
		break;		
	case TT_RENDERBET:			//To render the betting scene
		{
			RenderBetScene();
		}
		break;
	case TT_RENDERRES:			//To render the game result in table turning  scene
		{
			RenderTurnScene();
		}
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}
//---------------------------------------------------------------
//	Name:OnReceiveMsg
//	Introduce:SKT_MSG_RECEIVEDMSG message handle
//---------------------------------------------------------------
HRESULT CRussiaClientDlg::OnReceiveMsg(WPARAM wParam, LPARAM lParam)
{
	LPBYTE pDataBuff = (LPBYTE)wParam;
	int	iDataSize = lParam;
	if(NULL == pDataBuff || iDataSize <= 0)
		return S_FALSE;

	//Backup the message data
	LPBYTE pMsgDataBuff = new BYTE[iDataSize];
	memcpy(pMsgDataBuff, pDataBuff, iDataSize);
	pDataBuff = pMsgDataBuff;

	//To read the message data
	int iCurReadCnt = 0;
	while(iCurReadCnt < iDataSize)
	{
		UINT uMsgType = SMT_NONE;
		memcpy(&uMsgType, pDataBuff, sizeof(UINT));

		int iReadCnt = 0;
		switch(uMsgType)
		{
		case SMT_STC_GETCLIENTINFO:		//
			iReadCnt = ProcGetClientInfoCmdData((LPGETCLIENTINFO)pDataBuff);
			break;
		case SMT_STC_SERVERSETINFO:
			iReadCnt = ProcServerSetInfoData((LPSERVERSETINFO)pDataBuff);
			break;
		case SMT_STC_ACTIVECMD:
			iReadCnt = ProcActiveCmdData((LPACTIVEINFO)pDataBuff);
			break;
		case SMT_STC_USERVALUEINFO:
			iReadCnt = ProcUserValueInfo((LPUSERVALUEINFO)pDataBuff);
			break;
		case SMT_STC_STARTBET:
			iReadCnt = ProcStartBetCmd((LPSTARTBETCMD)pDataBuff);
			break;
		case SMT_STC_TOTALBETINFO:
			iReadCnt = ProcTotalBetInfo((LPTOTALBETINFO)pDataBuff);
			break;
		case SMT_STC_ENDBET:
			iReadCnt = ProcEndBetCmd((LPENDBETCMD)pDataBuff);
			break;
		case SMT_STC_RENDERCMD:
			iReadCnt = ProcRenderCmdData((LPRENDERINFO)pDataBuff);
			break;
		case SMT_STC_GAMERESULT:
			iReadCnt = ProcGameResultInfo((LPGAMERESULTINFO)pDataBuff);
			break;
		case SMT_STC_SHOWRESULT:
			iReadCnt = ProcShowResultCmd((LPSHOWRESULTCMD)pDataBuff);
			break;
		case SMT_STC_GAMEEND:
			iReadCnt = ProcGameEndCmd((LPGAMEENDCMD)pDataBuff);
			break;
		case SMT_STC_WINVALUE:
			iReadCnt = ProcWinValueInfo((LPWINVALUEINFO)pDataBuff);
			break;
		case SMT_STC_GAMEOVER:
			iReadCnt = ProcGameOver((LPGAMEOVERCMD)pDataBuff);
			break;
		case SMT_STC_CLOSE:
			{
				iReadCnt = sizeof(CloseClientCmd);

				KillTimer(TT_BETTIMER);
				PostQuitMessage(0);
			}
			break;
		default:
			iReadCnt = sizeof(BYTE);
			break;
		}

		iCurReadCnt += iReadCnt;
		pDataBuff += iReadCnt;
	}

	delete []pMsgDataBuff;
	pMsgDataBuff = NULL;

	return S_OK;
}
//---------------------------------------------------------------
//	Name:ProcGetClientInfoCmdData
//	Introduce:When received a SMT_STC_GETCLIENTINFO message data, call this function 
//			  to return the client information
//---------------------------------------------------------------
int CRussiaClientDlg::ProcGetClientInfoCmdData(LPGETCLIENTINFO lpGetClientInfo)
{
	if(NULL == lpGetClientInfo)
		return 0;

	if(m_pSocket)
	{
		ClientInfo stuClientInfo;
		stuClientInfo.uMsgType = SMT_CTS_CLIENTINFO;

		stuClientInfo.uClientID = m_uClientID;
		lstrcpy(stuClientInfo.szName, _T("Client\0"));
		m_pSocket->SendData(&stuClientInfo, sizeof(stuClientInfo));

		//Set current state to connected		
		m_eCurState = CS_CONNECTED;
	}	

	return sizeof(GetClientInfo);
}
//---------------------------------------------------------------
//	Name:ProcActiveCmdData
//	Introduce:When received a SMT_STC_SERVERSETINFO message data, call this function 
//			  to deal with it
//---------------------------------------------------------------
int	CRussiaClientDlg::ProcServerSetInfoData(LPSERVERSETINFO lpServerSetInfo)
{
	if(NULL == lpServerSetInfo)
		return 0;

	memcpy(&m_stuServerSetting, lpServerSetInfo, sizeof(ServerSetInfo));

	//Update the new coin images
	UpdateChipObjs(CT_OBJMIN, 0);
	UpdateChipObjs(CT_OBJMAX, 0);

	//Update the limit bet information
	UpdateLimitBetInfo(m_stuServerSetting.iMaxRedBet, m_stuServerSetting.iMaxBlackBet, m_stuServerSetting.iMaxGreenBet);

	return sizeof(ServerSetInfo);
}
//---------------------------------------------------------------
//	Name:ProcActiveCmdData
//	Introduce:When received a SMT_STC_ACTIVECMD message data, call this function 
//		      to deal with this data buffer
//---------------------------------------------------------------
int	CRussiaClientDlg::ProcActiveCmdData(LPACTIVEINFO lpActiveInfo)
{
	if(NULL == lpActiveInfo)
		return 0;

	if(m_uClientID == lpActiveInfo->uClientID)
	{
		if(m_eCurState == CS_CONNECTED)
		{
			//This client can be actived
			if(lpActiveInfo->bCanActived)
			{
				m_eCurState = CS_ACTIVED;	

				//Send message to server, this client is actived!
				if(NULL != m_pSocket)
				{
					HasActivedCmd stuHasActived;
					stuHasActived.uMsgType = SMT_CTS_HASACTIVED;
					stuHasActived.uClientID = m_uClientID;
					m_pSocket->SendData(&stuHasActived, sizeof(stuHasActived));
				}

				//Show the current user value
				m_fCurValue = lpActiveInfo->fValue;
				UpdateCurValue(CVT_REMAIN, (int)m_fCurValue);

				//This client is actived, show the betting UI
				if(NULL != m_pHeadFlash)
				{
					m_pHeadFlash->Stop();
					MediaLib_DestroyFlash(m_pHeadFlash);
					m_pHeadFlash = NULL;

					//To play the bk music
					if(NULL != m_pBKMusic)
						m_pBKMusic->Play(TRUE);
					SetTimer(TT_RENDERBET, 25, NULL);
				}
			}
			//This client can't be actived
			else
			{
				MessageBox("请先交费,然后再开通该机!");
			}
		}
	}

	return sizeof(ActiveInfo);
}
//---------------------------------------------------------------
//	Name:ProcuserValueInfo
//	Introduce:When received a SMT_STC_USERVALUEINFO message, call this function 
//			  to deal with this data buffer
//---------------------------------------------------------------
int CRussiaClientDlg::ProcUserValueInfo(LPUSERVALUEINFO lpUserValueInfo)
{
	if(NULL == lpUserValueInfo)
		return 0;

	if(m_uClientID == lpUserValueInfo->uClientID)
	{
		m_fCurValue = lpUserValueInfo->fValue;

		UpdateCurValue(CVT_REMAIN, (int)m_fCurValue);
	}

	return sizeof(UserValueInfo);
}
//---------------------------------------------------------------
//	Name:ProcStartBetCmd
//	Introduce:When received a SMT_STC_STARTBET message, call this function 
//			  to deal with this command
//---------------------------------------------------------------
int CRussiaClientDlg::ProcStartBetCmd(LPSTARTBETCMD lpStartBetCmd)
{
	if(NULL == lpStartBetCmd)
		return 0;

	//Only has actived clients can do betting operation
	if(CS_ACTIVED == m_eCurState || CS_GAMEEND == m_eCurState)
	{
		m_eCurState = CS_BETTING;	
		
		//Clear the undo list
		m_lstBetUndo.clear();

		//Bet time number
		m_iCurTimeNum = lpStartBetCmd->iBetTimeLen;
		UpdateTimeNum(m_iCurTimeNum);
		
		//Reset total betting value
		memset(&m_stuCurTotalBetInfo, 0, sizeof(m_stuCurTotalBetInfo));
		UpdateTotalBetInfo(0, 0, 0);

		//Reset betting value
		memset(&m_stuUserBetInfo, 0, sizeof(m_stuUserBetInfo));
		UpdateCurBetInfo(0, 0, 0);

		//Reset win value
		m_fWinValue = 0;
		UpdateCurValue(CVT_WINVAL, (int)m_fWinValue);

		SetTimer(TT_BETTIMER, 1000, NULL);
	}

	return sizeof(StartBetCmd);
}
//---------------------------------------------------------------
//	Name:ProcTotalBetInfo
//	Introduce:When received a SMT_STC_TOTALBETINFO message, call this function 
//			  to deal with this data buffer
//---------------------------------------------------------------
int CRussiaClientDlg::ProcTotalBetInfo(LPTOTALBETINFO lpTotalBetInfo)
{
	if(NULL == lpTotalBetInfo)
		return 0;
	
	memcpy(&m_stuCurTotalBetInfo, lpTotalBetInfo, sizeof(TotalBetInfo));

	UpdateTotalBetInfo((int)m_stuCurTotalBetInfo.fTotalBetRedVal, (int)m_stuCurTotalBetInfo.fTotalBetBlackVal, (int)m_stuCurTotalBetInfo.fTotalBetGreenVal);

	return sizeof(TotalBetInfo);
}
//---------------------------------------------------------------
//	Name:ProcEndBetCmd
//	Introduce:When received a SMT_STC_ENDBET message, call this function
//			  to deal with this coomand
//---------------------------------------------------------------
int CRussiaClientDlg::ProcEndBetCmd(LPENDBETCMD lpEndBetCmd)
{
	if(NULL == lpEndBetCmd)
		return 0;

	//Only has submitted betting information clients can deal with this message
	if((CS_SUBMITED == m_eCurState) || (CS_BETTING == m_eCurState))
	{
		m_eCurState = CS_BETTED;
		KillTimer(TT_RENDERBET);

		//To play the roll ball music
		if(NULL != m_pRollMusic)
			m_pRollMusic->Play(TRUE);
	}

	return sizeof(EndBetCmd);
}
//---------------------------------------------------------------
//	Name:ProcRenderCmdData
//	Introduce:When received a SMT_STC_RENDERCMD message, call this function 
//			  to deal with this data
//---------------------------------------------------------------
int CRussiaClientDlg::ProcRenderCmdData(LPRENDERINFO lpRenderInfo)
{
	if(NULL == lpRenderInfo)
		return 0;

	//Only has end betting operation clients can deal with this message
	if(CS_BETTED == m_eCurState || CS_TURNING == m_eCurState)
	{
		m_eCurState = CS_TURNING;

		//Save  current frame render information data
		memcpy(&m_stuCurRenderInfo, lpRenderInfo, sizeof(RenderInfo));	

		m_fCurAngle = m_stuCurRenderInfo.fTableAngle;

		//Render turn table scene
		RenderTurnScene();

		//Tell server this frame render end
		RenderEndCmd stuRenderEnd;
		stuRenderEnd.uMsgType = SMT_CTS_RENDEREND;
		stuRenderEnd.uClientID = m_uClientID;
		if(NULL != m_pSocket)
			m_pSocket->SendData(&stuRenderEnd, sizeof(RenderEndCmd));
	}

	return sizeof(RenderInfo);
}
//---------------------------------------------------------------
//	Name:ProcGameResultInfo
//	Introduce:When received a SMT_STC_GAMERESULT message, call this function
//			  to deal with this data
//---------------------------------------------------------------
int CRussiaClientDlg::ProcGameResultInfo(LPGAMERESULTINFO lpGameResult)
{
	if(NULL == lpGameResult)
		return 0;

	memcpy(&m_stuGameResult, lpGameResult, sizeof(GameResultInfo));

	//Show the result char object
	m_pTurnScene->ChangeObjTexture(g_szResCharObjName, SOS_NORMAL, g_szResultChar[m_stuGameResult.uFlg], TRUE);
	m_pTurnScene->SetObjVisible(g_szResCharObjName, TRUE);

	//Stop the ball roll music
	if(NULL != m_pRollMusic)
		m_pRollMusic->Stop();

	UdpdateResultFlg(m_stuGameResult.iGameNo-1, m_stuGameResult.uFlg, TRUE);
	
	return sizeof(GameResultInfo);
}
//---------------------------------------------------------------
//	Name:ProcShowResultCmd
//	Introduce:When received a SMT_STC_SHOWSULT message, call this function
//			  to deal with this data
//---------------------------------------------------------------
int CRussiaClientDlg::ProcShowResultCmd(LPSHOWRESULTCMD pShowResCmd)
{
	if(NULL == pShowResCmd)
		return 0;

	if(CS_TURNING == m_eCurState)
	{	
		m_eCurState = CS_SHOWRESULT;
		SetTimer(TT_RENDERRES, 25, NULL);		
	}

	return sizeof(ShowResultCmd);
}
//---------------------------------------------------------------
//	Name:ProcGameEndCmd
//	Introduce:When received a SMT_STC_GAMEEND cmd, call this function 
//			  to deal with this data
//---------------------------------------------------------------
int CRussiaClientDlg::ProcGameEndCmd(LPGAMEENDCMD lpGameEndCmd)
{
	if(NULL == lpGameEndCmd)
		return 0;

//	if(CS_SHOWRESULT == m_eCurState)
	{
		m_eCurState = CS_GAMEEND;

		//Hide the result char object
		m_pTurnScene->SetObjVisible(g_szResCharObjName, FALSE);

		//Set the new angle for the mini table to flag the game result
		m_pBetScene->RotateObject(g_szTurntable[0], lpGameEndCmd->fAnlgeShowRes*ANG_TO_RAD);
	}

	//End to render the game result on table turning scene
	KillTimer(TT_RENDERRES);

	//Render the betting scene
	SetTimer(TT_RENDERBET, 25, NULL);		

	UdpdateResultFlg(m_stuGameResult.iGameNo-1, m_stuGameResult.uFlg, TRUE);

	return sizeof(GameEndCmd);
}
//---------------------------------------------------------------
//	Name:ProcGameOver
//	Introduce:When received a SMT_STC_WINVALUE message, call this function 
//			  to deal with this data
//---------------------------------------------------------------
int CRussiaClientDlg::ProcWinValueInfo(LPWINVALUEINFO lpWinValueInfo)
{
	if(NULL == lpWinValueInfo)
		return 0;

	if(CS_GAMEEND == m_eCurState)
	{
		//The win value for this game
		m_fWinValue = (float)lpWinValueInfo->iWinValue;
		UpdateCurValue(CVT_WINVAL, (int)m_fWinValue);
	}	

	return sizeof(WinValueInfo);
}
//---------------------------------------------------------------
//	Name:ProcGameOver
//	Introduce:When received a SMT_STC_GAMEOVER message, call this function
//			  to deal with this command
//---------------------------------------------------------------
int CRussiaClientDlg::ProcGameOver(LPGAMEOVERCMD lpGameOverCmd)
{
	if(NULL == lpGameOverCmd)
		return 0;

	//Here we deal the game over event

	return sizeof(GameOverCmd);
}
//---------------------------------------------------------------



//---------------------------------------------------------------
//	Name:ClickOKBtn
//	Introduce:User selected OK button
//---------------------------------------------------------------
void CRussiaClientDlg::ClickOKBtn()
{
	switch(m_eCurState)
	{
/*
	case CS_LOADED:
		{
			MessageBox(_T("该客户机没有开通，请联系管理员开通该机!"));
			return;
		}
*/
		break;
	case CS_CONNECTED: 
		{
			AskForActiveCmd stuAskActive;
			stuAskActive.uMsgType = SMT_CTS_ASKFORACTCMD;
			stuAskActive.uClientID = m_uClientID;
			if(NULL != m_pSocket)
				m_pSocket->SendData(&stuAskActive, sizeof(AskForActiveCmd));
		}
		break;
	case CS_BETTING:
	case CS_SUBMITED:
		{		
			//Submit the bet information to server
			m_stuUserBetInfo.uMsgType = SMT_CTS_SUBMITBET;
			m_stuUserBetInfo.uClientID = m_uClientID;
			if(NULL != m_pSocket)
			{
				m_pSocket->SendData(&m_stuUserBetInfo, sizeof(m_stuUserBetInfo));
			
				m_eCurState = CS_SUBMITED;

				//Clear the undo list
				m_lstBetUndo.clear();
			}
		}
		break;
	default:
		return;
	}
}
//---------------------------------------------------------------
//	Name:BetValOperation
//	Introduce:User selected bet operation
//---------------------------------------------------------------
void CRussiaClientDlg::BetValOperation(UINT uBetType)
{
	//User can submit the bet values more than one times
	if(CS_BETTING != m_eCurState && CS_SUBMITED != m_eCurState)
		return;

	LPBETUNDO pUndo = new BetUndo;

	int iValDelta = 0;
	int iMaxBetVal = 0;
	float* pChangeVal = NULL;
	switch(uBetType)
	{
	case BT_RED_MIN:
		iValDelta = m_stuServerSetting.iMinBet;
		iMaxBetVal = m_stuServerSetting.iMaxRedBet;
		pChangeVal = &m_stuUserBetInfo.fBetRedVal;
		pUndo->uBetColor = RF_RED;
		break;
	case BT_RED_MAX:
		iValDelta = m_stuServerSetting.iMaxBet;
		iMaxBetVal = m_stuServerSetting.iMaxRedBet;
		pChangeVal = &m_stuUserBetInfo.fBetRedVal;
		pUndo->uBetColor = RF_RED;
		break;	
	case BT_BLACK_MIN:
		iValDelta = m_stuServerSetting.iMinBet;
		iMaxBetVal = m_stuServerSetting.iMaxBlackBet;
		pChangeVal = &m_stuUserBetInfo.fBetBlackVal;
		pUndo->uBetColor = RF_BLACK;
		break;
	case BT_BLACK_MAX:
		iValDelta = m_stuServerSetting.iMaxBet;
		iMaxBetVal = m_stuServerSetting.iMaxBlackBet;
		pChangeVal = &m_stuUserBetInfo.fBetBlackVal;
		pUndo->uBetColor = RF_BLACK;
		break;
	case BT_GREEN_MIN:
		iValDelta = m_stuServerSetting.iMinBet;
		iMaxBetVal = m_stuServerSetting.iMaxGreenBet;
		pChangeVal = &m_stuUserBetInfo.fBetGreenVal;
		pUndo->uBetColor = RF_GREEN;
		break;
	case BT_GREEN_MAX:
		iValDelta = m_stuServerSetting.iMaxBet;
		iMaxBetVal = m_stuServerSetting.iMaxGreenBet;
		pChangeVal = &m_stuUserBetInfo.fBetGreenVal;
		pUndo->uBetColor = RF_GREEN;
		break;
	default:
		return;
	}

	//Save bet undo
	pUndo->fBetValue = iValDelta;
	m_lstBetUndo.push_back(pUndo);

	//Check the betting operation
	if(m_fCurValue < iValDelta)
	{
		MessageBox(_T("您的分值不够!"));
		return;
	}
	if((*pChangeVal+iValDelta) > iMaxBetVal)
	{
		MessageBox(_T("该颜色的押注您已经达到上限!"));
		return;
	}

	//Update the information
	*pChangeVal += iValDelta;
	m_fCurValue -= iValDelta;

	if(iValDelta == m_stuServerSetting.iMinBet)
		UpdateChipObjs(CT_OBJMIN, 1);
	else
		UpdateChipObjs(CT_OBJMAX, 1);

	UpdateCurValue(CVT_REMAIN, (int)m_fCurValue);
	UpdateCurBetInfo((int)m_stuUserBetInfo.fBetRedVal, (int)m_stuUserBetInfo.fBetBlackVal,(int)m_stuUserBetInfo.fBetGreenVal);

	//To play the bet coin music
	if(NULL != m_pBetCoinMusic)
		m_pBetCoinMusic->Play();
}
//---------------------------------------------------------------
//	Name:UndoBetValOperation
//	Introduce:Undo the bet value operation
//---------------------------------------------------------------
void CRussiaClientDlg::UndoBetValOperation()
{
	if(m_lstBetUndo.size() <= 0)
		return;

	LPBETUNDO lpBetUndo = m_lstBetUndo[m_lstBetUndo.size()-1];
	switch(lpBetUndo->uBetColor)
	{
	case RF_RED:
		m_stuUserBetInfo.fBetRedVal -= lpBetUndo->fBetValue;
		break;
	case RF_BLACK:
		m_stuUserBetInfo.fBetBlackVal -= lpBetUndo->fBetValue;
		break;
	case RF_GREEN:
		m_stuUserBetInfo.fBetGreenVal -= lpBetUndo->fBetValue;
		break;
	}

	m_fCurValue += lpBetUndo->fBetValue;

	UpdateCurValue(CVT_REMAIN, (int)m_fCurValue);
	UpdateCurBetInfo((int)m_stuUserBetInfo.fBetRedVal, (int)m_stuUserBetInfo.fBetBlackVal,(int)m_stuUserBetInfo.fBetGreenVal);

	m_lstBetUndo.pop_back();
	delete lpBetUndo;
	lpBetUndo = NULL;
}
//---------------------------------------------------------------
//	Name:UdpdateResultFlg
//	Introduce:Update the result flag information on UI
//---------------------------------------------------------------
void CRussiaClientDlg::UdpdateResultFlg(int iIndex, UINT uFlg, BOOL bVisible)
{
	if(iIndex < 0 || iIndex >= RESULTFLG_CNT)
		return;

	TCHAR szName[16] = {_T("\0")};
	_stprintf(szName, _T("ResultFlg_%d"), iIndex);

	m_pBetScene->ChangeObjTexture(szName, SOS_NORMAL, g_szResultFlg[uFlg], FALSE);
	m_pBetScene->SetObjVisible(szName, bVisible);
}
//---------------------------------------------------------------
//	Name:UpdateLimitBetInfo
//	Introduce:Update the bet value limit information on UI
//---------------------------------------------------------------
void CRussiaClientDlg::UpdateLimitBetInfo(int iMaxBetRed, int iMaxBetBlack, int iMaxBetGreen)
{
	int iMaxBetVal[CT_COUNT];
	iMaxBetVal[CT_RED] = iMaxBetRed;
	iMaxBetVal[CT_BLACK] = iMaxBetBlack;
	iMaxBetVal[CT_GREEN] = iMaxBetGreen;

	TCHAR szObjName[32] = {_T("\0")};
	TCHAR szObjTex[32] = {_T("\0")};
	for(UINT uColor = CT_RED; uColor < CT_COUNT; uColor++)
	{
		//At first, set all object visible is FALSE
		for(int iCnt = 0; iCnt < LIMITNUM_CNT; iCnt++)
		{
			GetLimitNumObjName((Color_Type)uColor, iCnt, szObjName);
			m_pBetScene->SetObjVisible(szObjName, FALSE);
		}

		//Show the new limit numbers
		int iNum = 0;
		int iCurIndex = 0;
		int iCurNum = iMaxBetVal[uColor];
		do
		{
			iNum = iCurNum % 10;
			iCurNum = iCurNum / 10;

			GetLimitNumObjName((Color_Type)uColor, iCurIndex, szObjName);
			GetNumTexFile(2, iNum, szObjTex);
			//
			m_pBetScene->ChangeObjTexture(szObjName, SOS_NORMAL, szObjTex, FALSE);
			m_pBetScene->SetObjVisible(szObjName, TRUE);

			iCurIndex++;

		}while(iCurNum != 0);
	}	
}
//---------------------------------------------------------------
//	Name:UpdateTotalBetInfo
//	Introduce:Update current total bet value number information on UI
//---------------------------------------------------------------
void CRussiaClientDlg::UpdateTotalBetInfo(int iTotalBetRed, int iTotalBetBlack, int iTotalBetGreen)
{
	int iTotalBetVal[CT_COUNT];
	iTotalBetVal[CT_RED] = iTotalBetRed;
	iTotalBetVal[CT_BLACK] = iTotalBetBlack;
	iTotalBetVal[CT_GREEN] = iTotalBetGreen;

	TCHAR szObjName[32] = {_T("\0")};
	TCHAR szObjTex[32] = {_T("\0")};
	for(UINT uColor = CT_RED; uColor < CT_COUNT; uColor++)
	{
		//At first, set all object visible is FALSE
		for(int iCnt = 0; iCnt < TOTALBETNUM_CNT; iCnt++)
		{
			GetTotalBetNumObjName((Color_Type)uColor, iCnt, szObjName);
			m_pBetScene->SetObjVisible(szObjName, FALSE);
		}

		//Show the new current total bet value numbers
		int iNum = 0;
		int iCurIndex = 0;
		int iCurNum = iTotalBetVal[uColor];
		do
		{
			iNum = iCurNum % 10;
			iCurNum = iCurNum / 10;

			GetTotalBetNumObjName((Color_Type)uColor, iCurIndex, szObjName);
			GetNumTexFile(2, iNum, szObjTex);
			//
			m_pBetScene->ChangeObjTexture(szObjName, SOS_NORMAL, szObjTex, FALSE);
			m_pBetScene->SetObjVisible(szObjName, TRUE);

			iCurIndex++;

		}while(iCurNum != 0);
	}	
}
//---------------------------------------------------------------
//	Name:UpdateTimeNum
//	Introduce:Update the current time number object information on UI
//---------------------------------------------------------------
void CRussiaClientDlg::UpdateTimeNum(int iTimeNum)
{
	if(iTimeNum < 0 || iTimeNum > 99)
		return;

	int iNum[TIMENUM_CNT];
	iNum[0] = iTimeNum % 10;
	iNum[1] = iTimeNum / 10;

	TCHAR szObjTex[32] = {_T("\0")};
	for(int iCnt = 0; iCnt < TIMENUM_CNT; iCnt++)
	{
		//Get the number texture file name
		GetNumTexFile(2, iNum[iCnt], szObjTex);

		//Change
		m_pBetScene->ChangeObjTexture(g_szTimeNumName[iCnt], SOS_NORMAL, szObjTex, FALSE);
		m_pBetScene->SetObjVisible(g_szTimeNumName[iCnt], TRUE);
	}
}
//---------------------------------------------------------------
//	Name:UpdateCurBetInfo
//	Introduce:Update the current bet value number information on UI
//---------------------------------------------------------------
void CRussiaClientDlg::UpdateCurBetInfo(int iCurBetRed, int iCurBetBlack, int iCurBetGreen)
{
	int iCurBetVal[CT_COUNT];
	iCurBetVal[CT_RED] = iCurBetRed;
	iCurBetVal[CT_BLACK] = iCurBetBlack;
	iCurBetVal[CT_GREEN] = iCurBetGreen;

	TCHAR szObjName[32] = {_T("\0")};
	TCHAR szObjTex[32] = {_T("\0")};
	for(UINT uColor = CT_RED; uColor < CT_COUNT; uColor++)
	{
		//At first, set all object visible is FALSE
		for(int iCnt = 0; iCnt < BETVALNUM_CNT; iCnt++)
		{
			GetBetValNumObjName((Color_Type)uColor, iCnt, szObjName);
			m_pBetScene->SetObjVisible(szObjName, FALSE);
		}

		//Show the new current bet value numbers
		int iNum = 0;
		int iCurIndex = 0;
		int iCurNum = iCurBetVal[uColor];
		do
		{
			iNum = iCurNum % 10;
			iCurNum = iCurNum / 10;

			GetBetValNumObjName((Color_Type)uColor, iCurIndex, szObjName);
			GetNumTexFile(1, iNum, szObjTex);
			//
			m_pBetScene->ChangeObjTexture(szObjName, SOS_NORMAL, szObjTex, FALSE);
			m_pBetScene->SetObjVisible(szObjName, TRUE);

			iCurIndex++;

		}while(iCurNum != 0);
	}	
}
//---------------------------------------------------------------
//	Name:UpdateChipObjs
//	Introduce:Update the chip objects image on UI
//---------------------------------------------------------------
void CRussiaClientDlg::UpdateChipObjs(UINT uObjType, UINT uImageType)
{
	int iChipImage = 0;
	if(CT_OBJMIN == uObjType)
	{
		iChipImage = GetChipValType(m_stuServerSetting.iMinBet);
	}
	else
	{
		iChipImage = GetChipValType(m_stuServerSetting.iMaxBet);
	}
	if(iChipImage >= CT_VALCOUNT)
		return;

	TCHAR szChipImage[MAX_PATH] = {_T("\0")};
	if(0 == uImageType)
	{
		lstrcpy(szChipImage, g_szStaticChip[iChipImage]);
	}
	else
	{
		lstrcpy(szChipImage, g_szAnimateChip[iChipImage]);
	}

	m_pBetScene->ChangeObjTexture(g_szChipObjName[uObjType], SOS_NORMAL, szChipImage, FALSE);
	m_pBetScene->SetObjVisible(g_szChipObjName[uObjType], TRUE);	
}
//---------------------------------------------------------------
//	Name:GetChipValType
//	Introduce:Get the chip values type
//---------------------------------------------------------------
UINT CRussiaClientDlg::GetChipValType(int iChipVal)
{
	if(5 == iChipVal)
		return CT_VAL5;
	else if(10 == iChipVal)
		return CT_VAL10;
	else if(20 == iChipVal)
		return CT_VAL20;
	else if(50 == iChipVal)
		return CT_VAL50;
	else if(100 == iChipVal)
		return CT_VAL100;
	else if(200 == iChipVal)
		return CT_VAL200;
	else if(500 == iChipVal)
		return CT_VAL500;
	else if(1000 == iChipVal)
		return CT_VAL1000;
	else if(2000 == iChipVal)
		return CT_VAL2000;
	else
		return CT_VALCOUNT + 1;
}
//---------------------------------------------------------------
//	Name:UpdateCurValue
//	Introduce:Update current value number information on UI
//---------------------------------------------------------------
void CRussiaClientDlg::UpdateCurValue(UINT eValType, int iCurVal)
{
	TCHAR szObjName[32] = {_T("\0")};
	TCHAR szObjTex[32] = {_T("\0")};

	//At first, set all object visible is FALSE
	for(int iCnt = 0; iCnt < USERVALNUM_CNT; iCnt++)
	{
		GetUserValNumObjName(eValType, iCnt, szObjName);
		m_pBetScene->SetObjVisible(szObjName, FALSE);
	}

	//Show the new limit numbers
	int iNum = 0;
	int iCurIndex = 0;
	int iCurNum = iCurVal;
	do
	{
		iNum = iCurNum % 10;
		iCurNum = iCurNum / 10;

		GetUserValNumObjName(eValType, iCurIndex, szObjName);
		GetNumTexFile(1, iNum, szObjTex);
		//
		m_pBetScene->ChangeObjTexture(szObjName, SOS_NORMAL, szObjTex, FALSE);
		m_pBetScene->SetObjVisible(szObjName, TRUE);

		iCurIndex++;

	}while(iCurNum != 0);
}
//---------------------------------------------------------------



//---------------------------------------------------------------
//	Name:WndPosToScene_X
//	Introduce:convert point (X) in window to scene
//---------------------------------------------------------------
int CRussiaClientDlg::WndPosToScene_X(int iWndX)
{
	int iSceneX = -SCENE_WIDTH/2 + iWndX;

	return iSceneX;
}
//---------------------------------------------------------------
//	Name:WndPosToScene_Y
//	Introduce:convert point (Y) in window to scene
//---------------------------------------------------------------
int CRussiaClientDlg::WndPosToScene_Y(int iWndY)
{
	int iSceneY = SCENE_HEIGHT/2 - iWndY;

	return iSceneY;
}
//---------------------------------------------------------------
//	Name:ReadConfigInfo
//	Introduce:Read the config information
//---------------------------------------------------------------
void CRussiaClientDlg::ReadConfigInfo(LPCTSTR lpszIniFile)
{
	if(NULL == lpszIniFile)
		return;

	GetPrivateProfileString(_T("Server"), _T("IP"), _T("\0"), m_szServerIP, 16, lpszIniFile);
	m_uServerPort = GetPrivateProfileInt(_T("Server"), _T("Port"), 200, lpszIniFile);
	//
	m_uClientID = GetPrivateProfileInt(_T("Client"), _T("ID"), 0, lpszIniFile);
	m_uUIStype = GetPrivateProfileInt(_T("Client"), _T("UIType"), UIS_H, lpszIniFile);
}
//---------------------------------------------------------------
//	Name:ConnectServer
//	Introduce:Try to connect the server
//---------------------------------------------------------------
BOOL CRussiaClientDlg::ConnectServer()
{
	if(NULL != m_pSocket)
		delete m_pSocket;

	//Create the client socket
	m_pSocket = new CClientSocket(m_hWnd);
	if(!m_pSocket->Create(m_uClientID))
		return FALSE;
	
	if(!m_pSocket->Connect(m_szServerIP, m_uServerPort))
		return FALSE;

	return TRUE;
}
//---------------------------------------------------------------
//	Name:LoadScene
//	Introduce:To load the scene resurce
//---------------------------------------------------------------
BOOL CRussiaClientDlg::LoadScene()
{
	HRESULT hr = S_OK;

	//create the head flash
	hr = CreateHeadFlash();
	if(FAILED(hr))
		return FALSE;

	//To create the bet scene
	hr = CreateBetScene();
	if(FAILED(hr))
		return FALSE;

	//To create the turn table scene
	hr = CreateTurnScene();
	if(FAILED(hr))
		return FALSE;

	//Load the BK music
	TCHAR szMusic[MAX_PATH] = {"\0"};
	if(NULL == m_pBKMusic)
		m_pBKMusic = MediaLib_CreateMusic();
	lstrcpy(szMusic, g_szSoundPath);
	lstrcat(szMusic, g_szBKSound);
	m_pBKMusic->LoadFromFile(szMusic);

	//Load the roll ball music
	if(NULL == m_pRollMusic)
		m_pRollMusic = MediaLib_CreateMusic();
	lstrcpy(szMusic, g_szSoundPath);
	lstrcat(szMusic, g_szRollSound);
	m_pRollMusic->LoadFromFile(szMusic);

	//Load the bet coin music
	if(NULL == m_pBetCoinMusic)
		m_pBetCoinMusic = MediaLib_CreateMusic();
	lstrcpy(szMusic, g_szSoundPath);
	lstrcat(szMusic, g_szBetCoinSound);
	m_pBetCoinMusic->LoadFromFile(szMusic);

	//Load the bet coin music
	if(NULL == m_pClockMusic)
		m_pClockMusic = MediaLib_CreateMusic();
	lstrcpy(szMusic, g_szSoundPath);
	lstrcat(szMusic, g_szClockSound);
	m_pClockMusic->LoadFromFile(szMusic);

	return TRUE;
}
//---------------------------------------------------------------
//	Name:CreateHeadFlash
//	Introduce:To create the head flash
//---------------------------------------------------------------
BOOL CRussiaClientDlg::CreateHeadFlash()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(&rc);

	//Create the head flash player
	if(NULL != m_pHeadFlash)
		MediaLib_DestroyFlash(m_pHeadFlash);
	m_pHeadFlash = MediaLib_CreateFlash();
	if(NULL == m_pHeadFlash)
		return FALSE;

	hr = m_pHeadFlash->Create(m_hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE, 0);
	if(FAILED(hr))
		return FALSE;

	return TRUE;
}
//---------------------------------------------------------------
//	Name:CreateBetScene
//	Introduce:Create the betting scene
//---------------------------------------------------------------
BOOL CRussiaClientDlg::CreateBetScene()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(&rc);

	//Create the scene object pointer
	if(m_pBetScene)
		MediaLib_DestroyScene(m_pBetScene);
	m_pBetScene = MediaLib_CreateScene();
	if(NULL == m_pBetScene)
		return FALSE;	

	//Set the scene params
	hr = m_pBetScene->InitScene(m_hWnd, SCENE_WIDTH, SCENE_HEIGHT, rc);
	if(FAILED(hr))
		return FALSE;

	//Set the texture file path
	m_pBetScene->SetTextureFilePath(g_szImagePath);

	//Load bk images
	for(UINT uCnt= 0; uCnt < UIS_COUNT; uCnt++)
		m_pBetScene->LoadTextureFile(g_szBKImage[uCnt]);

	//Load the mini turn table image
	m_pBetScene->LoadTextureFile(g_szTurnImage[0]);

	//Load the result flag images
	for(UINT uFlg = 0; uFlg < RF_COUNT; uFlg++)
		m_pBetScene->LoadTextureFile(g_szResultFlg[uFlg]);

	//Load chips object images
	LoadChipsTextures();

	//Load the num object image files
	LoadNumTextures();

	//Load the ball object image files
	for(int iCnt = 0; iCnt < BALLIMAGE_CNT; iCnt++)
		m_pBetScene->LoadTextureFile(g_szBallImage[iCnt]);

	//Add the scene background image
	float fLeft = (float)WndPosToScene_X(0);
	float fTop = (float)WndPosToScene_Y(0);
	float fRight = (float)WndPosToScene_X(SCENE_WIDTH);
	float fBottom = (float)WndPosToScene_Y(SCENE_HEIGHT);
	m_pBetScene->AddBackground(_T("BK"), fLeft, fTop, fRight, fBottom, g_szBKImage[m_uUIStype], false);
	if(FAILED(hr))
		return FALSE;

	//Add the mini runtable object
	fLeft = (float)WndPosToScene_X(m_stuMiniTurnRect[m_uUIStype].left);
	fTop = (float)WndPosToScene_Y(m_stuMiniTurnRect[m_uUIStype].top);
	fRight = (float)WndPosToScene_X(m_stuMiniTurnRect[m_uUIStype].right);
	fBottom = (float)WndPosToScene_Y(m_stuMiniTurnRect[m_uUIStype].bottom);
	hr = m_pBetScene->AddRectObject(g_szTurntable[0], fLeft, fTop, fRight, fBottom, g_szTurnImage[0]);
	if(FAILED(hr))
		return FALSE;

	//Add the ball object
	fLeft = (float)WndPosToScene_X(m_stuMiniBallRect[m_uUIStype].left);
	fTop = (float)WndPosToScene_Y(m_stuMiniBallRect[m_uUIStype].top);
	fRight = (float)WndPosToScene_X(m_stuMiniBallRect[m_uUIStype].right);
	fBottom = (float)WndPosToScene_Y(m_stuMiniBallRect[m_uUIStype].bottom);
	hr = m_pBetScene->AddRectObject(g_szBallObjName, fLeft, fTop, fRight, fBottom, g_szBallImage[0]);
	if(FAILED(hr))
		return FALSE;

	//Create the result
	CreateResultFlgObj();

	//Create the limit number objects
	CreateLimitNumObj();

	//Create the total bet value number objects
	CreateTotalBetNumObj();

	//Create the time number objects
	CreateTimeNumObj();

	//Create the current bet value number objects
	CreateBetValNumObj();

	//Create the chip value objects
	CreateChipsObj();

	//Create the current user value number objects
	CreateUserValNumObj();

	//Create the client ID number objects
	CreateClientIDNumObj();

	return TRUE;
}
//---------------------------------------------------------------
//	Name:GetLimitNumObjName
//	Introduce:Load the chips object textures
//---------------------------------------------------------------
void CRussiaClientDlg::LoadChipsTextures()
{
	TCHAR szTex[MAX_PATH] = {_T("\0")};
	for(UINT uType = 0; uType < CT_VALCOUNT; uType++)
	{
		lstrcpy(szTex, g_szStaticChip[uType]);
		m_pBetScene->LoadTextureFile(szTex);
		//
		lstrcpy(szTex, g_szAnimateChip[uType]);
		m_pBetScene->LoadTextureFile(szTex);
	}
}
//---------------------------------------------------------------
//	Name:GetLimitNumObjName
//	Introduce:To return the limit number object name string
//---------------------------------------------------------------
void CRussiaClientDlg::GetLimitNumObjName(Color_Type eColor, int iIndex, LPTSTR lpszName)
{
	if(NULL == lpszName)
		return;

	TCHAR szName[16] = {_T("\0")};
	switch(eColor)
	{
	case CT_RED:
		_stprintf(szName, _T("LimitRed_%d\0"), iIndex);
		break;
	case CT_BLACK:
		_stprintf(szName, _T("LimitBlack_%d\0"), iIndex);
		break;
	case CT_GREEN:
		_stprintf(szName, _T("LimitGreen_%d\0"), iIndex);
		break;
	}

	lstrcpy(lpszName, szName);	
}
//---------------------------------------------------------------
//	Name:CreateTurnScene
//	Introduce:Create the turn table scene
//---------------------------------------------------------------
BOOL CRussiaClientDlg::CreateTurnScene()
{
	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(&rc);

	//Create the scene object pointer
	if(m_pTurnScene)
		MediaLib_DestroyScene(m_pTurnScene);
	m_pTurnScene = MediaLib_CreateScene();
	if(NULL == m_pTurnScene)
		return FALSE;	

	//Set the scene params
	hr = m_pTurnScene->InitScene(m_hWnd, SCENE_WIDTH, SCENE_HEIGHT, rc);
	if(FAILED(hr))
		return FALSE;

	//Set the texture file path
	m_pTurnScene->SetTextureFilePath(g_szImagePath);

	//Load bk images
	for(UINT uCnt= 0; uCnt < UIS_COUNT; uCnt++)
		m_pTurnScene->LoadTextureFile(g_szBigBKImage[uCnt]);

	//Load the big turn table image
	m_pTurnScene->LoadTextureFile(g_szTurnImage[2]);
	
	//Load the ball object image files
	for(int iCnt = 0; iCnt < BALLIMAGE_CNT; iCnt++)
		m_pTurnScene->LoadTextureFile(g_szBallImage[iCnt]);
	
	//Load the result char image files
	for(UINT uChar = 0; uChar < RF_COUNT; uChar++)
		m_pTurnScene->LoadTextureFile(g_szResultChar[uChar]);		

	//Add the scene background image
	float fLeft = (float)WndPosToScene_X(0);
	float fTop = (float)WndPosToScene_Y(0);
	float fRight = (float)WndPosToScene_X(SCENE_WIDTH);
	float fBottom = (float)WndPosToScene_Y(SCENE_HEIGHT);
	m_pTurnScene->AddBackground(_T("BK"), fLeft, fTop, fRight, fBottom, g_szBigBKImage[m_uUIStype], false);
	if(FAILED(hr))
		return FALSE;

	//Add the big runtable object
	fLeft = (float)WndPosToScene_X(m_stuBigTurnRect[m_uUIStype].left);
	fTop = (float)WndPosToScene_Y(m_stuBigTurnRect[m_uUIStype].top);
	fRight = (float)WndPosToScene_X(m_stuBigTurnRect[m_uUIStype].right);
	fBottom = (float)WndPosToScene_Y(m_stuBigTurnRect[m_uUIStype].bottom);
	hr = m_pTurnScene->AddRectObject(g_szTurntable[1], fLeft, fTop, fRight, fBottom, g_szTurnImage[1]);
	if(FAILED(hr))
		return FALSE;

	//Add the ball object
	fLeft = (float)WndPosToScene_X(m_stuBigBallRect[m_uUIStype].left);
	fTop = (float)WndPosToScene_Y(m_stuBigBallRect[m_uUIStype].top);
	fRight = (float)WndPosToScene_X(m_stuBigBallRect[m_uUIStype].right);
	fBottom = (float)WndPosToScene_Y(m_stuBigBallRect[m_uUIStype].bottom);
	hr = m_pTurnScene->AddRectObject(g_szBallObjName, fLeft, fTop, fRight, fBottom, g_szBallImage[0]);
	if(FAILED(hr))
		return FALSE;

	//Add the result char object
	fLeft = (float)WndPosToScene_X(m_stuResCharRect[m_uUIStype].left);
	fTop = (float)WndPosToScene_Y(m_stuResCharRect[m_uUIStype].top);
	fRight = (float)WndPosToScene_X(m_stuResCharRect[m_uUIStype].right);
	fBottom = (float)WndPosToScene_Y(m_stuResCharRect[m_uUIStype].bottom);
	hr = m_pTurnScene->AddRectObject(g_szResCharObjName, fLeft, fTop, fRight, fBottom, NULL, NULL, NULL, NULL, FALSE, FALSE);
	if(FAILED(hr))
		return FALSE;

	return TRUE;
}
//---------------------------------------------------------------
//	Name:PlayHead
//	Introduce:To play the head flash
//---------------------------------------------------------------
void CRussiaClientDlg::PlayHead()
{
	if(NULL == m_pHeadFlash)
		return;

	//The head flash file name
	TCHAR szHeadFlash[MAX_PATH] = {_T("\0")};
	lstrcpy(szHeadFlash, g_szImagePath);
	lstrcat(szHeadFlash, g_szHeadFlash);

	HRESULT hr = S_OK;
	hr = m_pHeadFlash->Play(szHeadFlash, TRUE);
}
//---------------------------------------------------------------
//	Name:RenderBetScene
//	Introduce:To render the bet scene 
//---------------------------------------------------------------
void CRussiaClientDlg::RenderBetScene()
{
	m_pBetScene->Render();
}
//---------------------------------------------------------------
//	Name:RenderTurnScene
//	Introduce:To render the turn table scene 
//---------------------------------------------------------------
void CRussiaClientDlg::RenderTurnScene()
{
	m_pTurnScene->RotateObject(g_szTurntable[1], m_fCurAngle*ANG_TO_RAD);

	m_pTurnScene->MoveObjectTo(g_szBallObjName, m_stuCurRenderInfo.fBallPosX, m_stuCurRenderInfo.fBallPosY);

	m_pTurnScene->Render();
}
//---------------------------------------------------------------
//	Name:UpdateBallImage
//	Introduce:Update the ball images
//---------------------------------------------------------------
void CRussiaClientDlg::UpdateBallImage(BOOL bReset)
{
	if(TRUE == bReset)
	{
		m_pTurnScene->ChangeObjTexture(g_szBallObjName, SOS_NORMAL, g_szBallImage[0], FALSE);
	}
	else
	{
		static int iIndex = 0;
		//m_pTurnScene->ChangeObjTexture(g_szBallObjName, SOS_NORMAL, g_szBallImage[iIndex], FALSE);
		if(++iIndex > BALLIMAGE_CNT)
			iIndex = 0;
	}
}
//---------------------------------------------------------------



//---------------------------------------------------------------
//	Name:LoadNumTextures
//	Introduce:Get the num texture file name string
//---------------------------------------------------------------
void CRussiaClientDlg::GetNumTexFile(int iType, int iIndex, LPTSTR lpszName)
{
	if(NULL == lpszName)
		return;

	if(1 != iType && 2 != iType)
		return;

	TCHAR szFileName[32] = {_T("\0")};
	_stprintf(szFileName, g_szNumberPath, iType, iIndex);

	lstrcpy(lpszName, szFileName);
}
//---------------------------------------------------------------
//	Name:LoadNumTextures
//	Introduce:Load number object texture files
//---------------------------------------------------------------
void CRussiaClientDlg::LoadNumTextures()
{
	TCHAR szFileName[MAX_PATH] = {_T("\0")};
	for(int iType = 0; iType < 2; iType++)
	{
		for(int iNum = 0; iNum < 10; iNum++)
		{
			GetNumTexFile(iType+1, iNum, szFileName);
			m_pBetScene->LoadTextureFile(szFileName);
		}
	}
}
//---------------------------------------------------------------
//	Name:GetResultFlgObjName
//	Introduce:To return the result flag objects rect
//---------------------------------------------------------------
void CRussiaClientDlg::GetResultFlgObjRect(int iIndex, RECT &rc)
{
	const int iWidth = 24;
	const int iHeight = 24;

	//UI is horizontal type
	if(UIS_H == m_uUIStype)
	{
		const int iBaseLeft = 120;
		const int iBaseTop1 = 125;
		const int iBaseTop2 = 179;
		const int iDeltaX = 8;

		//Row 1
		if(iIndex < 35)
		{
			rc.left = iBaseLeft + iIndex*(iWidth+iDeltaX);
			rc.right = rc.left + iWidth;
			rc.top = iBaseTop1;
			rc.bottom = rc.top + iHeight;
		}
		//Row 2
		else
		{
			rc.left = iBaseLeft + (iIndex-35)*(iWidth+iDeltaX);
			rc.right = rc.left + iWidth;
			rc.top = iBaseTop2;
			rc.bottom = rc.top + iHeight;
		}
	}
	//UI is vertical type
	else
	{
		const int iBaseTop = 161;
		const int iBaseLeft1 = 1029;
		const int iBaseLeft2 = 1089;
		const int iBaseLeft3 = 1150;
		const int iDeltaY = 2;

		//Column 1
		if(iIndex < 24)
		{
			rc.left = iBaseLeft1;
			rc.right = rc.left + iWidth;
			rc.top = iBaseTop + iIndex*(iHeight+iDeltaY);
			rc.bottom = rc.top + iHeight;
		}
		//Column 2
		else if(iIndex < 48)
		{
			rc.left = iBaseLeft2;
			rc.right = rc.left + iWidth;
			rc.top = iBaseTop + (iIndex-24)*(iHeight+iDeltaY);
			rc.bottom = rc.top + iHeight;
		}
		//Column 3
		else
		{
			rc.left = iBaseLeft3;
			rc.right = rc.left + iWidth;
			rc.top = iBaseTop + (iIndex-48)*(iHeight+iDeltaY);
			rc.bottom = rc.top + iHeight;
		}
	}
}
//---------------------------------------------------------------
//	Name:CreateResultFlgObj
//	Introduce:Create the result flag objcets
//---------------------------------------------------------------
void CRussiaClientDlg::CreateResultFlgObj()
{
	RECT rc;
	TCHAR szName[16] = {_T("\0")};
	for(UINT uCnt = 0; uCnt < RESULTFLG_CNT; uCnt++)
	{
		//Name
		_stprintf(szName, _T("ResultFlg_%d"), uCnt);

		//Rect
		GetResultFlgObjRect(uCnt, rc);
		float fLeft = (float)WndPosToScene_X(rc.left);
		float fTop = (float)WndPosToScene_Y(rc.top);
		float fRight = (float)WndPosToScene_X(rc.right);
		float fBottom = (float)WndPosToScene_Y(rc.bottom);
			
		m_pBetScene->AddRectObject(szName, fLeft, fTop, fRight, fBottom, NULL, NULL, NULL, NULL, FALSE, FALSE);
	}
}
//---------------------------------------------------------------
//	Name:CreateLimitNumObj
//	Introduce:Create the limit number objects
//---------------------------------------------------------------
void CRussiaClientDlg::CreateLimitNumObj()
{
	TCHAR szObjName[32] = {_T("\0")};
	for(UINT uColor = CT_RED; uColor < CT_COUNT; uColor++)
	{
		for(int iCnt = 0; iCnt < LIMITNUM_CNT; iCnt++)
		{
			//Name string
			GetLimitNumObjName((Color_Type)uColor, iCnt, szObjName);

			//The rect
			float fLeft = (float)WndPosToScene_X(m_iLimitObjRight[m_uUIStype] - (iCnt+1)*m_iLimitObjWidth[m_uUIStype]);
			float fTop = (float)WndPosToScene_Y(m_iLimitObjTop[m_uUIStype][uColor]);
			float fRight = (float)WndPosToScene_X(m_iLimitObjRight[m_uUIStype] - iCnt*m_iLimitObjWidth[m_uUIStype]);
			float fBottom = (float)WndPosToScene_Y(m_iLimitObjTop[m_uUIStype][uColor] + m_iLimitObjHeight[m_uUIStype]);
				
			m_pBetScene->AddRectObject(szObjName, fLeft, fTop, fRight, fBottom, NULL, NULL, NULL, NULL, FALSE, FALSE);
		}
	}
}
//---------------------------------------------------------------
//	Name:GetTotalBetNumObjName
//	Introduce:To return current total bet value number object name string
//---------------------------------------------------------------
void CRussiaClientDlg::GetTotalBetNumObjName(Color_Type eColor, int iIndex, LPTSTR lpszName)
{
	if(NULL == lpszName)
		return;

	TCHAR szName[16] = {_T("\0")};
	switch(eColor)
	{
	case CT_RED:
		_stprintf(szName, _T("TotalBetRed_%d\0"), iIndex);
		break;
	case CT_BLACK:
		_stprintf(szName, _T("TotalBetBlack_%d\0"), iIndex);
		break;
	case CT_GREEN:
		_stprintf(szName, _T("TotalBetGreen_%d\0"), iIndex);
		break;
	}

	lstrcpy(lpszName, szName);	
}
//---------------------------------------------------------------
//	Name:CreateTotalBetNumObj
//	Introduce:Create current total bet value number objects
//---------------------------------------------------------------
void CRussiaClientDlg::CreateTotalBetNumObj()
{
	TCHAR szObjName[32] = {_T("\0")};
	for(UINT uColor = CT_RED; uColor < CT_COUNT; uColor++)
	{
		for(int iCnt = 0; iCnt < TOTALBETNUM_CNT; iCnt++)
		{
			//Name string
			GetTotalBetNumObjName((Color_Type)uColor, iCnt, szObjName);

			//The rect
			float fLeft = (float)WndPosToScene_X(m_iTotalBetObjRight[m_uUIStype] - (iCnt+1)*m_iTotalBetObjWidth[m_uUIStype]);
			float fTop = (float)WndPosToScene_Y(m_iTotalBetObjTop[m_uUIStype][uColor]);
			float fRight = (float)WndPosToScene_X(m_iTotalBetObjRight[m_uUIStype] - iCnt*m_iTotalBetObjWidth[m_uUIStype]);
			float fBottom = (float)WndPosToScene_Y(m_iTotalBetObjTop[m_uUIStype][uColor] + m_iTotalBetObjHeight[m_uUIStype]);
				
			m_pBetScene->AddRectObject(szObjName, fLeft, fTop, fRight, fBottom, NULL, NULL, NULL, NULL, FALSE, FALSE);
		}
	}
}
//---------------------------------------------------------------
//	Name:CreateTimeNumObj
//	Introduce:Create time number objects
//---------------------------------------------------------------
void CRussiaClientDlg::CreateTimeNumObj()
{
	for(int iCnt = 0; iCnt < TIMENUM_CNT; iCnt++)
	{
		//The rect
		float fLeft = (float)WndPosToScene_X(m_iTimeNumObjRight[m_uUIStype] - (iCnt+1)*TIMENUM_WIDTH);
		float fTop = (float)WndPosToScene_Y(m_iTimeNumObjTop[m_uUIStype]);
		float fRight = (float)WndPosToScene_X(m_iTimeNumObjRight[m_uUIStype] - iCnt*TIMENUM_WIDTH);
		float fBottom = (float)WndPosToScene_Y(m_iTimeNumObjTop[m_uUIStype] + TIMENUM_HEIGHT);
			
		//add the objects to scene
		m_pBetScene->AddRectObject(g_szTimeNumName[iCnt], fLeft, fTop, fRight, fBottom, NULL, NULL, NULL, NULL, FALSE, FALSE);
	}
}
//---------------------------------------------------------------
//	Name:GetBetValNumObjName
//	Introduce:To return current bet value number object name string
//---------------------------------------------------------------
void CRussiaClientDlg::GetBetValNumObjName(Color_Type eColor, int iIndex, LPTSTR lpszName)
{
	if(NULL == lpszName)
		return;

	TCHAR szName[16] = {_T("\0")};
	switch(eColor)
	{
	case CT_RED:
		_stprintf(szName, _T("CurBetRed_%d\0"), iIndex);
		break;
	case CT_BLACK:
		_stprintf(szName, _T("CurBetBlack_%d\0"), iIndex);
		break;
	case CT_GREEN:
		_stprintf(szName, _T("CurBetGreen_%d\0"), iIndex);
		break;
	}

	lstrcpy(lpszName, szName);	
}
//---------------------------------------------------------------
//	Name:CreateBetValNumObj
//	Introduce:Create current bet value number objects
//---------------------------------------------------------------
void CRussiaClientDlg::CreateBetValNumObj()
{
	TCHAR szObjName[32] = {_T("\0")};
	for(UINT uColor = CT_RED; uColor < CT_COUNT; uColor++)
	{
		for(int iCnt = 0; iCnt < BETVALNUM_CNT; iCnt++)
		{
			//Name string
			GetBetValNumObjName((Color_Type)uColor, iCnt, szObjName);

			//The rect
			float fLeft = (float)WndPosToScene_X(m_iBetValObjRight[m_uUIStype][uColor] - (iCnt+1)*BETVALNUM_WIDTH);
			float fTop = (float)WndPosToScene_Y(m_iBetValObjTop[m_uUIStype]);
			float fRight = (float)WndPosToScene_X(m_iBetValObjRight[m_uUIStype][uColor] - iCnt*BETVALNUM_WIDTH);
			float fBottom = (float)WndPosToScene_Y(m_iBetValObjTop[m_uUIStype] + BETVALNUM_HEIGHT);
				
			m_pBetScene->AddRectObject(szObjName, fLeft, fTop, fRight, fBottom, NULL, NULL, NULL, NULL, FALSE, FALSE);
		}
	}
}
//---------------------------------------------------------------
//	Name:CreateChipsObj
//	Introduce:Create the chips objects
//---------------------------------------------------------------
void CRussiaClientDlg::CreateChipsObj()
{	
	for(UINT uCnt = 0; uCnt < CHIPOBJ_CNT; uCnt++)
	{
		float fLeft = (float)WndPosToScene_X(m_iChipObjLeft[m_uUIStype][uCnt]);
		float fTop = (float)WndPosToScene_Y(m_iChipObjTop[m_uUIStype]);
		float fRight = (float)WndPosToScene_X(m_iChipObjLeft[m_uUIStype][uCnt] + CHIPOBJ_WIDTH);
		float fBottom = (float)WndPosToScene_Y(m_iChipObjTop[m_uUIStype] + CHIPOBJ_HEIGHT);
		
		m_pBetScene->AddRectObject(g_szChipObjName[uCnt], fLeft, fTop, fRight, fBottom, NULL, NULL, NULL, NULL, FALSE, FALSE);
	}
}
//---------------------------------------------------------------
//	Name:GetUserValNumObjName
//	Introduce:To return current user value number object name string
//---------------------------------------------------------------
void CRussiaClientDlg::GetUserValNumObjName(UINT uType, int iIndex, LPTSTR lpszName)
{
	if(NULL == lpszName)
		return;

	TCHAR szName[16] = {_T("\0")};
	switch(uType)
	{
	case CVT_REMAIN:
		_stprintf(szName, _T("RemainVal_%d\0"), iIndex);
		break;
	case CVT_WINVAL:
		_stprintf(szName, _T("WinVal_%d\0"), iIndex);
		break;
	}

	lstrcpy(lpszName, szName);	
}
//---------------------------------------------------------------
//	Name:CreateUserValNumObj
//	Introduce:Create current user value number objects
//---------------------------------------------------------------
void CRussiaClientDlg::CreateUserValNumObj()
{
	TCHAR szObjName[32] = {_T("\0")};
	for(UINT uType = CVT_REMAIN; uType < CVT_COUNT; uType++)
	{
		for(int iCnt = 0; iCnt < USERVALNUM_CNT; iCnt++)
		{
			//Name string
			GetUserValNumObjName(uType, iCnt, szObjName);

			//The rect
			float fLeft = (float)WndPosToScene_X(m_iUserValObjRight[m_uUIStype][uType] - (iCnt+1)*USERVALNUM_WIDTH);
			float fTop = (float)WndPosToScene_Y(m_iUserValObjTop[m_uUIStype]);
			float fRight = (float)WndPosToScene_X(m_iUserValObjRight[m_uUIStype][uType] - iCnt*USERVALNUM_WIDTH);
			float fBottom = (float)WndPosToScene_Y(m_iUserValObjTop[m_uUIStype] + USERVALNUM_HEIGHT);
				
			m_pBetScene->AddRectObject(szObjName, fLeft, fTop, fRight, fBottom, NULL, NULL, NULL, NULL, FALSE, FALSE);
		}
	}
}
//---------------------------------------------------------------
//	Name:CreateClientIDNumObj
//	Introduce:Create client ID number objects
//---------------------------------------------------------------
void CRussiaClientDlg::CreateClientIDNumObj()
{
	int iCurNum = m_uClientID;
	TCHAR szObjName[32] = {_T("\0")};
	TCHAR szObjTex[32] = {_T("\0")};
	for(int iCnt = 0; iCnt < CLIENTIDNUM_CNT; iCnt++)
	{
		//Name string
		_stprintf(szObjName,_T("ClientID_%d"), iCnt);

		//The rect
		float fLeft = (float)WndPosToScene_X(m_iClientIDObjRight[m_uUIStype] - (iCnt+1)*CLIENTIDNUM_WIDTH);
		float fTop = (float)WndPosToScene_Y(m_iClientIDObjTop[m_uUIStype]);
		float fRight = (float)WndPosToScene_X(m_iClientIDObjRight[m_uUIStype] - iCnt*CLIENTIDNUM_WIDTH);
		float fBottom = (float)WndPosToScene_Y(m_iClientIDObjTop[m_uUIStype] + CLIENTIDNUM_HEIGHT);

		//Texture file name string
		int iNum = iCurNum % 10;
		iCurNum = iCurNum / 10;
		GetNumTexFile(1, iNum, szObjTex);
			
		m_pBetScene->AddRectObject(szObjName, fLeft, fTop, fRight, fBottom, szObjTex);
	}
}
//---------------------------------------------------------------
