// RussiaServerDlg.cpp : implementation file
//
//--------------------------------------------------
#include "stdafx.h"
#include "RussiaServer.h"
#include "RussiaServerDlg.h"
#include "QueryDlg.h"
//--------------------------------------------------
#include <math.h>
#include <algorithm>
#include <functional>
//--------------------------------------------------
/*Const strings definition*/
//--------------------------------------------------
const TCHAR	g_szAppTitle[]			= _T("俄国风情服务器端        版本号：%s		          系统编号：%d\0");
//
const TCHAR	g_szDBUserName[]		= _T("admin");
const TCHAR	g_szDBPassword[]		= _T("russiagame");
const TCHAR	g_szDBConnectString[]	= _T("Driver={Microsoft Access Driver (*.mdb)};Dbq=%s;Exclusive=1;Uid=%s;Pwd=%s");
//
const TCHAR	g_szDBFile[]			= _T("Database\\dbplayer.mdb\0");
const TCHAR	g_szCurValueInfoTable[]	= _T("CurUserValueInfo\0");
const TCHAR	g_szUserValueTable[]	= _T("UserValueInfo\0");
const TCHAR	g_szClientHistroyTable[]= _T("ClientHistroyInfo\0");
const TCHAR	g_szGameResultTable[]	= _T("GameResultInfo\0");
//Player Bet Base: means min bet score
const int  g_BetBaseScore[] = {5, 10, 20, 50, 100, 200, 500};
//--------------------------------------------------
/*The game result color definition*/
//--------------------------------------------------
const ResultFlg g_RestColor[37] = {	RF_GREEN, 													    	
									RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_BLACK, 
									RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_RED, RF_BLACK, 
									RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_BLACK, RF_RED, 
									RF_BLACK, RF_RED, RF_BLACK, RF_RED, RF_BLACK, RF_RED};
//--------------------------------------------------
/**The game result angle for table definition*/
const float ANGLE_DELTA			= 9.73f;	//360/37
const float g_fStartAngle[37]	= { 0.0f,
									23*ANGLE_DELTA, 6*ANGLE_DELTA, 35*ANGLE_DELTA, 4*ANGLE_DELTA, 19*ANGLE_DELTA, 10*ANGLE_DELTA, 
									31*ANGLE_DELTA, 16*ANGLE_DELTA, 27*ANGLE_DELTA, 18*ANGLE_DELTA, 14*ANGLE_DELTA, 5*ANGLE_DELTA,
									12*ANGLE_DELTA, 25*ANGLE_DELTA, 2*ANGLE_DELTA, 21*ANGLE_DELTA, 8*ANGLE_DELTA, 29*ANGLE_DELTA,
									3*ANGLE_DELTA, 24*ANGLE_DELTA, 5*ANGLE_DELTA, 28*ANGLE_DELTA, 17*ANGLE_DELTA, 20*ANGLE_DELTA,
									7*ANGLE_DELTA, 36*ANGLE_DELTA, 11*ANGLE_DELTA, 32*ANGLE_DELTA, 30*ANGLE_DELTA, 15*ANGLE_DELTA,
									26*ANGLE_DELTA, 1*ANGLE_DELTA, 22*ANGLE_DELTA, 9*ANGLE_DELTA, 34*ANGLE_DELTA, 13*ANGLE_DELTA}; 
//--------------------------------------------------
// local func
//--------------------------------------------------
static int SumOfBet(BetInfo betData)
{
	return (betData.iBetOnRed + betData.iBetOnBlack + betData.iBetOnGreen);
}
//--------------------------------------------------
// according the game result and user bet, calculate the user's win score
//--------------------------------------------------
static int WinOfBet(BetInfo betData, ResultFlg gameRst)
{
	int income = 0;
	
	if (gameRst == RED)
	{
		income = betData.iBetOnRed - (betData.iBetOnBlack + betData.iBetOnGreen);
	}
	else if(gameRst == BLACK)
	{
		income = betData.iBetOnBlack - (betData.iBetOnRed + betData.iBetOnGreen);
	}
	else if(gameRst == GREEN)
	{
		income = (betData.iBetOnGreen*LOSS_RATIO) - (betData.iBetOnRed + betData.iBetOnBlack);
	}

	return income;
}
//--------------------------------------------------
//	Name:GetWinVal
//	Introduce:Get the win value based on beted
//--------------------------------------------------
int	GetWinVal(BetInfo betData, ResultFlg gameRst)
{
	int iTotalWinVal = 0;
	
	switch(gameRst)
	{
	case RED:
		iTotalWinVal = 2*betData.iBetOnRed;
		break;
	case BLACK:
		iTotalWinVal = 2*betData.iBetOnBlack;
		break;
	case GREEN:
		iTotalWinVal = betData.iBetOnGreen * (LOSS_RATIO+1);
		break;
	}

	return iTotalWinVal;
}
//--------------------------------------------------
//	Name:GetClientStateStr
//	Introduce:To return the client state string
//--------------------------------------------------
LPCTSTR GetClientStateStr(UINT uState)
{
	switch(uState)
	{
	case CS_UNCONNECT:
		return _T("未连接");
		break;
	case CS_CONNECTED:
		return _T("已连接");
		break;
	case CS_ACTIVED:
		return _T("已开通");
		break;	
	case CS_SUBMITTED:
		return _T("下注完成");
		break;
	}

	return _T("\0");
}
//--------------------------------------------------
//	Name:GetServerStateStr
//	Introduce:To return the server state string
//--------------------------------------------------
LPCTSTR GetServerStateStr(UINT uState)
{
	switch(uState)
	{
	case SS_READY:
		return _T("就绪");
	case SS_BETTING:
		return _T("正在下注...");
		break;
	case SS_TURNING:
		return _T("正在摇奖...");
		break;	
	case SS_SHOWING:
		return _T("正在显示结果...");
		break;	
	case SS_END:
		return _T("正在显示得分情况...");
	}

	return _T("\0");
}
//--------------------------------------------------


//--------------------------------------------------
// CAboutDlg dialog used for App About
//--------------------------------------------------
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//--------------------------------------------------
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
//--------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//--------------------------------------------------



//--------------------------------------------------
//	Name:CRussiaServerDlg
//	Introduce:Constructor
//--------------------------------------------------
CRussiaServerDlg::CRussiaServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRussiaServerDlg::IDD, pParent),
	  m_uPort(200)
{
	//{{AFX_DATA_INIT(CRussiaServerDlg)
	m_strAddValClientID = _T("201");
	m_strSubValClientID = _T("201");
	m_nAddVal = 0;
	m_nSubVal = 0;
	m_iBetTime = 20;
	m_iMaxBlackBet = 5000;
	m_iMaxGreenBet = 2000;
	m_iMaxRedBet = 5000;
	m_iPerInningTime = 60;
	m_iBetSetIndex = 0;
	m_iBetMin = g_BetBaseScore[m_iBetSetIndex];  // 5;
	m_iBetMax = BETMULTIPLE * m_iBetMin;
	m_curRoundNo = 0;
	m_curInningNo = 0;
	m_curInningRstNo = -1;
	m_nNumOfCurInning =  0;	
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	lstrcpy(m_szDataBase, _T("\0"));
	lstrcpy(m_szIPAdress, _T("\0"));
	m_pServerSocket = NULL;
	m_eCurState = SS_READY;
	m_iCurBetTimeLen = m_iBetTime;
	memset(&m_stuTotalBetInfo, 0, sizeof(BetInfo));
	memset(m_clientData, 0, sizeof(m_clientData));
	m_iTotalSubmitCnt = 0;
	m_bCanRenderNextFrame = TRUE;
	m_iTotalRenderEndCnt = 0;
	m_iCurShowResTimeLen = SHOWRESULT_TIMELEN;
	m_iCurShowValTimeLen = SHOWWINVAL_TIMELEN;
	m_iRestTimeLen = 0;
	//
	m_bRenderBusy = FALSE;
	m_fTableTotalFrm = 0.0f;
	m_fAngleSpeedDelta = 0.0f;
	m_fCurTableSpeed = TABLE_INIT_ANGLE_SPEED;
	m_fCurTableAngle = 0.0f;
	//
	m_fCurBallLineSpeed = BALL_INIT_SPEED;
	m_fBallLineSpeedDelta = 0.0f;
	m_fCurBallPosY = BALL_INIT_POS;
	m_fBallAngleSpeedDelta = 0.0f;
	m_fCurBallAngleSpeed = 0.0f;	
	m_fCurBallAngle = 0.0f;
	m_fCurBallFrm = 0;
	m_fBallStopFrm = 0;
	m_bGameResultIsSent = FALSE;
}
//--------------------------------------------------
//	Name:~CRussiaServerDlg
//	Introduce:Distructor
//--------------------------------------------------
CRussiaServerDlg::~CRussiaServerDlg()
{
	if(NULL != m_pServerSocket)
	{
		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}
}
//--------------------------------------------------
void CRussiaServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRussiaServerDlg)
	DDX_CBString(pDX, IDC_COMBO_ADD, m_strAddValClientID);
	DDX_CBString(pDX, IDC_COMBO_SUB, m_strSubValClientID);
	DDX_Text(pDX, IDC_EDIT_ADDVAULE, m_nAddVal);
	DDX_Text(pDX, IDC_EDIT_SUBVALUE, m_nSubVal);
	DDX_Text(pDX, IDC_EDIT_BETTIME, m_iBetTime);
	DDV_MinMaxInt(pDX, m_iBetTime, 20, 35);
	DDX_Text(pDX, IDC_EDIT_BLACKLIMIT, m_iMaxBlackBet);
	DDX_Text(pDX, IDC_EDIT_GREENLIMIT, m_iMaxGreenBet);
	DDX_Text(pDX, IDC_EDIT_REDLIMIT, m_iMaxRedBet);
	DDX_Text(pDX, IDC_EDIT_INNINGTIME, m_iPerInningTime);
	DDV_MinMaxInt(pDX, m_iPerInningTime, 50, 80);
	DDX_CBIndex(pDX, IDC_COMBO_BETMIN, m_iBetSetIndex);
	//}}AFX_DATA_MAP
}
//--------------------------------------------------
BEGIN_MESSAGE_MAP(CRussiaServerDlg, CDialog)
	//{{AFX_MSG_MAP(CRussiaServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_QUERYRST, OnButtonQueryRst)
	ON_BN_CLICKED(IDC_BUTTON_PRINTBILL, OnButtonPrintBill)
	ON_BN_CLICKED(IDC_BUTTON_QUERYSCORE, OnButtonQueryScoreOP)
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, OnButtonShutdownClient)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExitSystem)
	ON_BN_CLICKED(IDC_BUTTON_ADDSCORE, OnButtonAddscore)
	ON_BN_CLICKED(IDC_BUTTON_SUBSCORE, OnButtonSubtractScore)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, OnButtonConfirmPara)
	ON_WM_TIMER()
	ON_MESSAGE(SKT_MSG_CLIENTCONNECTED, OnClientConnected)
	ON_MESSAGE(SKT_MSG_RECEIVEDMSG, OnRecievedMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//--------------------------------------------------
//	Name:OnInitDialog
//	Introduce:WM_INITDIALOG message handlers
//--------------------------------------------------
BOOL CRussiaServerDlg::OnInitDialog()
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

	
	//To initliaze the COM	
	::CoInitialize(NULL);

	//Set the application title string
	CString str;
	str.Format(g_szAppTitle, SOFTWARE_VERSION, SYSTEM_NO);
	SetWindowText(str);

	//Get the Database full file name string
	TCHAR szExePath[MAX_PATH] = {_T("\0")};
	::GetModuleFileName(NULL, szExePath, MAX_PATH);
	LPTSTR lpTmp = szExePath;
	while(*lpTmp != '\0') lpTmp++;
	while(*lpTmp != '\\') 
	{
		*lpTmp = '\0';
		lpTmp--;
	};
	lstrcpy(m_szDataBase, szExePath);
	lstrcat(m_szDataBase, g_szDBFile);	
	
	//Initilise the rand seed
	srand( (unsigned)time( NULL ) );

	//now we should check the user's validity, maybe it could be use the user machine's harddisk,netSerialNumber
	//need to add, robin...

	//Read data from database stored in harddisk
	//need to add, robin...

	//Init the Grid control to show data
	//need to add, robin...
	CListCtrl* plst = (CListCtrl*)GetDlgItem(IDC_LIST);
	plst->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	plst->InsertColumn(0, "分机号", LVCFMT_LEFT, 50);
	plst->InsertColumn(1, "剩余分", LVCFMT_LEFT, 80);
	plst->InsertColumn(2, "压红", LVCFMT_LEFT, 80);
	plst->InsertColumn(3, "压黑", LVCFMT_LEFT, 80);
	plst->InsertColumn(4, "压绿", LVCFMT_LEFT, 80);
	plst->InsertColumn(5, "当前状态", LVCFMT_LEFT, 150);

	//Get the initialize client value information
	InitUserData();
	
	//Update the client value information list
	for(UINT uClientID = MIN_CLIENT_NO; uClientID < MIN_CLIENT_NO+CLIENT_NO_CNT; uClientID++) 
		UpdateList(uClientID);

	//Create the network connect
	CreateNetwork();

	//Set the default server state
	ChangeServerState(SS_READY);

	//Calc the table and ball turn params
	CalcTableTurnParams();
	
	return TRUE;  
}
//--------------------------------------------------
//	Name:OnSysCommand
//	Introduce:OnSysCommand
//--------------------------------------------------
void CRussiaServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//--------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//--------------------------------------------------
void CRussiaServerDlg::OnPaint() 
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
//--------------------------------------------------
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//--------------------------------------------------
HCURSOR CRussiaServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//--------------------------------------------------



//--------------------------------------------------
//	Name:UpdateList
//	Introduce:To update the list ctrl
//--------------------------------------------------
void CRussiaServerDlg::UpdateList(UINT uClientID)
{	
	if(FALSE == ClientIDIsLegal(uClientID))
		return;

	CListCtrl* pValueLst = (CListCtrl*)GetDlgItem(IDC_LIST);
	if(NULL == pValueLst)
		return;

	int iIndex = -1;
	int iCurCnt = pValueLst->GetItemCount();

	//To check this client is in list or not
	for(int iCnt = 0; iCnt < iCurCnt; iCnt++)
	{
		TCHAR szClientID[8] = {_T("\0")};
		pValueLst->GetItemText(iCnt, 0, szClientID, 8);
		UINT uCurID = _ttoi(szClientID);

		if(uCurID == uClientID)
		{
			iIndex = iCnt;
			break;
		}
	}

	//This client is not in list, MUST add a new one
	TCHAR szValue[32] = {_T("\0")};
	if(-1 == iIndex)
	{
		//Client ID
		sprintf(szValue, "%d", uClientID);
		pValueLst->InsertItem(iCurCnt, szValue);
		iIndex = iCurCnt;
	}

	//Client current remain value
	sprintf(szValue, "%d", m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal);
	pValueLst->SetItemText(iIndex, 1, szValue);

	//The bet red value
	sprintf(szValue, "%d", m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnRed);
	pValueLst->SetItemText(iIndex, 2, szValue);

	//The bet black value
	sprintf(szValue, "%d", m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnBlack);
	pValueLst->SetItemText(iIndex, 3, szValue);

	//The bet green value
	sprintf(szValue, "%d", m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnGreen);
	pValueLst->SetItemText(iIndex, 4, szValue);

	//The state
	pValueLst->SetItemText(iIndex, 5, GetClientStateStr(m_clientData[uClientID-MIN_CLIENT_NO].eState));
}
//--------------------------------------------------
/*
 * Initialse the user data, include user id, Remain Val, bet on red/black/green
 * We can get these value in database files, and if not, we suggest it's initial value is 0
 */ 
//--------------------------------------------------
void CRussiaServerDlg::InitUserData()
{
	HRESULT hr = S_OK;

	//To open the databse
	CString strConnect;
	strConnect.Format(g_szDBConnectString, m_szDataBase, g_szDBUserName, g_szDBPassword);
	hr = m_Connection.Open(strConnect);
	if(FAILED(hr))
	{
		MessageBox(_T("Error Open DB!"));		
		return;
	}

	//To open the current User Value Information table
	CADOTable curValeTable;
	hr = curValeTable.Open(&m_Connection, g_szCurValueInfoTable);	
	if(FAILED(hr))
	{
		CString strError;
		strError.Format(_T("Error Open %s Table!"), g_szCurValueInfoTable);
		MessageBox(strError);
		return;
	}

	//Get the all clients value for history
	try
	{
		int iClientID = 0;
		int iRemainVal = 0;
		TCHAR szValue[32] = {_T("\0")};

		curValeTable.MoveFirst();
		while(!curValeTable.IsEOF())
		{
			//The client ID
			curValeTable.GetValue(_T("ClientID"), szValue);
			iClientID = atoi(szValue);
			if(ClientIDIsLegal(iClientID))
			{
				m_clientData[iClientID-MIN_CLIENT_NO].uClientID = iClientID;

				//Get the client value for history
				curValeTable.GetValue(_T("RemainVal"), szValue);
				m_clientData[iClientID-MIN_CLIENT_NO].iRemainVal = atoi(szValue);
			}

			curValeTable.MoveNext();
		}		
	}
	catch(_com_error e)
	{
		curValeTable.Close();
	}
	
	curValeTable.Close();
}
//--------------------------------------------------
//	Name:GenerateOneRound
//	Introduce:To create a rand number
//--------------------------------------------------
int CRussiaServerDlg::GenerateOneRound()
{
	int i;
	
	 /* Seed the random-number generator with current time so that
	 * the numbers will be different every time we run.
	 */
   	srand( (unsigned)time( NULL ) );

	for(i=0;i<MAX_INNING_PER_ROUND;i++)
	{
		m_curRoundRstArray[i] = (int)(rand()*1.0*37/RAND_MAX); 
	}
	m_nNumOfCurInning = 50+(rand() % 20);

	random_shuffle(m_curRoundRstArray, m_curRoundRstArray+MAX_INNING_PER_ROUND);

	// reduce a 37
	//for(i=0;i<MAX_INNING_PER_ROUND;i++)
	{
		
	}

	//m_curRoundNo++;

	return 0;
}
//--------------------------------------------------
//	Name:OnButtonStart
//	Introduce:To start the game
//--------------------------------------------------
void CRussiaServerDlg::OnButtonStart() 
{
	// after start, some button should not be pressed
	::EnableWindow(GetDlgItem(IDC_BUTTON_START)->m_hWnd,FALSE);
	::EnableWindow(GetDlgItem(IDC_BUTTON_STOP)->m_hWnd,FALSE);
	::EnableWindow(GetDlgItem(IDC_BUTTON_PAUSE)->m_hWnd,FALSE);
	//::EnableWindow(GetDlgItem(IDC_BUTTON_QUERYRST)->m_hWnd,FALSE);

	// we can check the pwd when a new round begin
	// checkPwd();

	// 在这里可以发送消息通知客户端，新的一轮即将开始
	// 客户端可以根据情况做出或不做反应
	/*
	StartBetCmd stuBetCmd;
	stuBetCmd.uMsgType = SMT_STC_STARTBET;
	stuBetCmd.iBetTimeLen = m_iBetTime;
	if(NULL != m_pServerSocket)
	{
		m_pServerSocket->SendData(&stuBetCmd, sizeof(StartBetCmd));
	}
	*/

	//启动定时器，定时到产生新一轮结果，打印路单
	SetTimer(TT_NEW_ROUND, 10*1000, NULL); 
}
//--------------------------------------------------
//	Name:OnButtonStop
//	Introduce:To stop current game
//--------------------------------------------------
void CRussiaServerDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here

	//now for my test
/*
	SubmitBetInfo testBet;
	unsigned char msgInfo[30]={0};
	testBet.uClientID = 201+(rand() % 15);  //201 -215
	testBet.fBetBlackVal = 50 + (rand() % 50); // 50-100
	testBet.fBetGreenVal = (rand() % 15); // 0-14
	testBet.fBetRedVal = 100 + (rand() % 50); // 100 - 150
	testBet.uMsgType = SMT_CTS_SUBMITBET;
	memcpy(msgInfo, &testBet, sizeof(SubmitBetInfo));

	m_curRoundNo = 1;
	m_curInningNo = 2;
	m_curRoundRstArray[m_curInningNo-1] = (rand() % 36) ;
	memset(&m_curTotalBet, 0, sizeof(UserBet_t));
	for (int i=0; i<MAX_CLIENT_NO; i++)
	{
		m_clientData[i].ClientID = 201+i;
		memset(&(m_clientData[i].betData), 0, sizeof(UserBet_t));
	}
	
	CString strNotify;
	strNotify.Format("用户[%d], Red[%d] Black[%d] Green[%d] Rst[%d]",testBet.uClientID, (int)testBet.fBetRedVal,
					(int)testBet.fBetBlackVal, (int)testBet.fBetGreenVal, m_curRoundRstArray[m_curInningNo-1]);
	MessageBox(strNotify,"提示",MB_OK);		

	OnReceive(msgInfo);
*/
}
//--------------------------------------------------
//	Name:OnButtonPause
//	Introduce:To pause current game
//--------------------------------------------------
void CRussiaServerDlg::OnButtonPause() 
{
	// TODO: Add your control notification handler code here

	// now for my test
/*	
	unsigned char msgInfo[30]={0};
	msgInfo[0]=SMT_CTS_RENDEREND;
	OnReceive(msgInfo);
*/
}
//--------------------------------------------------
//	Name:OnButtonQueryRst
//	Introduce:To query the game results
//--------------------------------------------------
void CRussiaServerDlg::OnButtonQueryRst() 
{
	//if( (ShowPassword())==FALSE )	return;

	if(m_curRoundNo == 0)
		return;

	FILE *fp;
	fp=fopen("C:\\Result.txt","wt");
	for (int i = 0; i < m_nNumOfCurInning; i++)
	{
		int rstNum = m_curRoundRstArray[i];
		
		if(g_RestColor[rstNum] == RED)
			fprintf(fp,"第%-3d局： 红 (%d)\n",i+1, rstNum);
		else if(g_RestColor[rstNum] == BLACK)
			fprintf(fp,"第%-3d局： 黑 (%d)\n",i+1, rstNum);
		else	
			(fp,"第%-3d局： 绿 (%d)\n",i+1, rstNum);
	}
	
	fclose(fp);
	system("notepad.exe  C:\\Result.txt");	
}
//--------------------------------------------------
//	Name:OnButtonPrintBill
//	Introduce:To print the current bill
//--------------------------------------------------
void CRussiaServerDlg::OnButtonPrintBill() 
{
	//firstly, check pwd, need add later
	//if( (ShowPassword())==FALSE )	return;

	HDC hdcprint; // 定义一个设备环境句柄
	CDC* pPrintDC=new CDC;
	CString str,str1;
	TEXTMETRIC tm;
	int nHeight;
	CPoint point=CPoint(720,-720);//距离左边0.5英寸的位置
	CFont font;	
	DOCINFO di={sizeof(DOCINFO),"printer",NULL};  //定义一个打印作业
	CPrintDialog dlg(FALSE,PD_ALLPAGES|PD_ALLPAGES|PD_NOPAGENUMS|PD_COLLATE &~PD_USEDEVMODECOPIESANDCOLLATE,NULL); //需要设置PD_RETURNDC，见MSDN

	dlg.m_pd.nCopies = 1; // 2
	
	//显示打印对话框
	if(dlg.DoModal()==IDOK)		
		hdcprint=dlg.CreatePrinterDC();
	else hdcprint=NULL;

	if( hdcprint == NULL )
	{
		MessageBox("没有默认打印机!");
		delete pPrintDC;
		return;
	}
	
	pPrintDC->Attach(hdcprint);
	//a point is 1/72 inch, a twip is 1/1440 inch，即1点为20个twip
	//y坐标向上为正方向，即顶端y是0，底端y是负的
	pPrintDC->SetMapMode(MM_TWIPS);

	// 14-point fixed-pitch font
	font.CreateFont(-240, 0, 0, 0, 700, FALSE, FALSE,
	                0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                DEFAULT_PITCH | FF_MODERN, "宋体");

	// Courier New is a TrueType font
	CFont* pOldFont=(CFont*)( pPrintDC->SelectObject(&font) );	
	CPen pen(PS_SOLID,-20,RGB(0,0,0));
	CPen* pOldPen=(CPen*)( pPrintDC->SelectObject(&pen) );

	if(pPrintDC->StartDoc(&di)>0)
	{ 	
		//开始执行一个打印作业
		pPrintDC->StartPage(); //打印机走纸,一页开始

		int nYear,nMonth,nDay,nHour,nMinute,nSecond;
		CTime t = CTime::GetCurrentTime();
		nYear=t.GetYear();
		nMonth=t.GetMonth();
		nDay=t.GetDay(); 
		nHour=t.GetHour();
		nMinute=t.GetMinute();
		nSecond=t.GetSecond(); 
		
		str.Format("俄国风情帐目表       打表时间: %d年%d月%d日%d时%d分%d秒       ",
			nYear,nMonth,nDay,nHour,nMinute,nSecond);
		pPrintDC->TextOut(point.x,point.y,str); 
		
		font.DeleteObject();
		font.CreateFont(-220, 0, 0, 0, 700, FALSE, FALSE,
						0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_MODERN, "宋体");
		pPrintDC->SelectObject(&font);
		pPrintDC->GetTextMetrics(&tm);
		nHeight = tm.tmHeight + tm.tmExternalLeading;
		nHeight+=40;
		
		point.y-=720;
		str="分机号        总上分        总退分        总押分        所得彩金";
		pPrintDC->TextOut(point.x,point.y,str); 

		int nSumOfBuy[MAX_CLIENT_NO], nSumOfUntread[MAX_CLIENT_NO];
		int nSumOfBet[MAX_CLIENT_NO], nSumOfWin[MAX_CLIENT_NO];
		memset(nSumOfBuy, 0, MAX_CLIENT_NO*sizeof(int));
		memset(nSumOfUntread, 0, MAX_CLIENT_NO*sizeof(int));
		memset(nSumOfBet, 0, MAX_CLIENT_NO*sizeof(int));
		memset(nSumOfWin, 0, MAX_CLIENT_NO*sizeof(int));
		
		TCHAR szCurDay[20] = {_T("\0")};
		sprintf(szCurDay, "%04d-%02d-%02d", nYear, nMonth, nDay);

		int iClientID = 0;
		TCHAR szValue[32] = {_T("\0")};
		TCHAR szID[32] = {_T("\0")};
		TCHAR szTime[20] = {_T("\0")};

		//打开用户表，查询当天玩家下注情况并统计结果
		CADOTable historyTable;
		HRESULT hr = historyTable.Open(&m_Connection, g_szClientHistroyTable);		
		if(FAILED(hr))
		{
			CString strError;
			strError.Format(_T("打开表:%s 错误!"), g_szClientHistroyTable);
			MessageBox(strError);
			return;
		}
		
		try
		{
			historyTable.MoveFirst();
			while(!historyTable.IsEOF())
			{
				//The client ID
				historyTable.GetValue(_T("DTime"), szTime);
				//only compare the former 9 byte
				if (memcmp(szTime, szCurDay, 10) == 0)
				{
					historyTable.GetValue(_T("ClientID"), szID);
					iClientID = atoi(szID);
					if(ClientIDIsLegal(iClientID))
					{
						historyTable.GetValue(_T("BetOnRed"), szValue);
						nSumOfBet[iClientID-MIN_CLIENT_NO] += atoi(szValue);

						historyTable.GetValue(_T("BetOnBlack"), szValue);
						nSumOfBet[iClientID-MIN_CLIENT_NO] += atoi(szValue);

						historyTable.GetValue(_T("BetOnGreen"), szValue);
						nSumOfBet[iClientID-MIN_CLIENT_NO] += atoi(szValue);

						historyTable.GetValue(_T("WinVal"), szValue);
						nSumOfWin[iClientID-MIN_CLIENT_NO] += atoi(szValue); // 这里计算结果是该用户所有赢分，打印时对庄家而言应取反
					}
				}
				historyTable.MoveNext();
			}		
		}
		
		catch(_com_error e)
		{
			historyTable.Close();
		}	
		historyTable.Close();

		CADOTable userValTable;
		hr = userValTable.Open(&m_Connection, g_szUserValueTable);		
		if(FAILED(hr))
		{
			CString strError;
			strError.Format(_T("打开表:%s 错误!"), g_szUserValueTable);
			MessageBox(strError);
			return;
		}
		
		try
		{
			userValTable.MoveFirst();
			while(!userValTable.IsEOF())
			{
				//The client ID
				userValTable.GetValue(_T("DTime"), szTime);
				//only compare the former 9 byte
				if (memcmp(szTime, szCurDay, 10) == 0)
				{
					userValTable.GetValue(_T("ClientID"), szID);
					iClientID = atoi(szID);
					if(ClientIDIsLegal(iClientID))
					{						
						userValTable.GetValue(_T("ActionType"), szValue);
						int nAcType = atoi(szValue);
						if (nAcType == ADD_SCORE)
						{
							userValTable.GetValue(_T("ActionVal"), szValue);
							nSumOfBuy[iClientID-MIN_CLIENT_NO] += atoi(szValue);
						}
						else if(nAcType == SUB_SCORE)
						{
							userValTable.GetValue(_T("ActionVal"), szValue);
							nSumOfUntread[iClientID-MIN_CLIENT_NO] += atoi(szValue);
						}						
					}
				}
				userValTable.MoveNext();
			}		
		}
		
		catch(_com_error f)
		{
			userValTable.Close();
		}
	
		userValTable.Close();

		int nTotalBuy, nTotalUntreated, nTotalBet, nTotalWin;
		nTotalBuy = nTotalBet = nTotalUntreated = nTotalWin = 0;
		
		for (int i = 0; i < CLIENT_NO_CNT; i++) 
		{
			point.y -= nHeight;
			// 在打印时对庄家而言应赢分取反
			str.Format(" %-13d%-14d%-14d%-14d%-14d", 
				i+MIN_CLIENT_NO, nSumOfBuy[i], nSumOfUntread[i], nSumOfBet[i], -nSumOfWin[i]);
						
			pPrintDC->TextOut(point.x, point.y, str);
			nTotalBuy += nSumOfBuy[i];
			nTotalBet  += nSumOfBet[i];
			nTotalUntreated += nSumOfUntread[i];
			nTotalWin += (-nSumOfWin[i]);	
		}
		point.y -= nHeight*2;
		str.Format("合  计         %-14d%-14d%-14d%-14d", 
			nTotalBuy, nTotalUntreated, nTotalBet, nTotalWin);
		pPrintDC->TextOut(point.x, point.y, str);
		
		point.y -= 2*nHeight;
		str.Format("系统编号：%d ",SYSTEM_NO);
		pPrintDC->TextOut(point.x, point.y, str);
		pPrintDC->EndPage(); //打印机停纸,一页结束 
		pPrintDC->EndDoc(); //结束一个打印作业
	}
	
	pPrintDC->SelectObject(pOldFont);
	pPrintDC->SelectObject(pOldPen);
	pPrintDC->Detach();
	delete pPrintDC;
	DeleteDC(hdcprint);
}
//--------------------------------------------------
//	Name:OnButtonQueryScoreOP
//	Introduce:To query score
//--------------------------------------------------
void CRussiaServerDlg::OnButtonQueryScoreOP() 
{
	QueryDlg dlg;
	dlg.DoModal();
}
//--------------------------------------------------
//	Name:OnButtonShutdownClient
//	Introduce:To shut down the clients
//--------------------------------------------------
void CRussiaServerDlg::OnButtonShutdownClient() 
{	
	CloseClientCmd stuCloseClient;
	stuCloseClient.uMsgType = SMT_STC_CLOSE;
	if(NULL != m_pServerSocket)
		m_pServerSocket->SendData(&stuCloseClient, sizeof(CloseClientCmd));
}
//--------------------------------------------------
//	Name:OnButtonExitSystem
//	Introduce:To exit
//--------------------------------------------------
void CRussiaServerDlg::OnButtonExitSystem() 
{
	PostQuitMessage(0);
}
//--------------------------------------------------
//	Name:OnButtonConfirmPara
//	Introduce:To confirm the params
//--------------------------------------------------
void CRussiaServerDlg::OnButtonConfirmPara() 
{
	UpdateData(TRUE);

	//Check the bet time setting
	if(m_iBetTime <= 0)
	{
		MessageBox(_T("下注时间设置错误!"));
		return;
	}

	//Check the max red bet setting
	if(m_iMaxRedBet <= 0)
	{
		MessageBox(_T("押红限制设置错误!"));
		return;
	}

	//Check the max black bet setting
	if(m_iMaxBlackBet <= 0)
	{
		MessageBox(_T("押黑限制设置错误!"));
		return;
	}

	//Check the max green bet setting
	if(m_iMaxGreenBet <= 0)
	{
		MessageBox(_T("押绿限制设置错误!"));
		return;
	}

	m_iBetMin = g_BetBaseScore[m_iBetSetIndex];
	m_iBetMax = BETMULTIPLE * m_iBetMin;
	
	//Send the setting to all client
	if(NULL != m_pServerSocket)
	{
		ServerSetInfo stuSetInfo;
		stuSetInfo.uMsgType = SMT_STC_SERVERSETINFO;

		stuSetInfo.iMinBet = m_iBetMin;
		stuSetInfo.iMaxBet = m_iBetMax;
		//
		stuSetInfo.iMaxRedBet = m_iMaxRedBet;
		stuSetInfo.iMaxBlackBet = m_iMaxBlackBet;
		stuSetInfo.iMaxGreenBet = m_iMaxGreenBet;

		m_pServerSocket->SendData(&stuSetInfo, sizeof(stuSetInfo));
	}	
}
//--------------------------------------------------
//	Name:OnButtonAddscore
//	Introduce:To add the score
//--------------------------------------------------
void CRussiaServerDlg::OnButtonAddscore() 
{
	UpdateData(TRUE);

	//Check the select is connected or not
	UINT uClientID = _ttoi(m_strAddValClientID);
	if(FALSE == ClientIDIsLegal(uClientID))
	{
		CString strError;
		strError.Format(_T("编号为:%s 的客户端无效!"), m_strAddValClientID);
		MessageBox(strError);
		return;
	}
	if(CS_UNCONNECT == m_clientData[uClientID-MIN_CLIENT_NO].eState)
	{
		CString strError;
		strError.Format(_T("编号为:%s 的客户端尚未连接服务器!"), m_strAddValClientID);
		MessageBox(strError);
		return;
	}

	//Check the input value
	if((m_nAddVal <= 0) || (m_nAddVal >= 100000))
	{
		MessageBox("请输入1—100000之间的数字", "提示", MB_ICONWARNING);
		return;
	}

	//To confirm the operation
	if(IDOK != MessageBox("确认买分吗？","提示",MB_OKCANCEL))
		return;

	//Olde value
	int iOldeValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;

	//Update the user value in memory
	UpdateUserValue(uClientID, ADD_SCORE, m_nAddVal);

	//Save this operation to user value information table
	int iNewValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;
	CTime curtime = CTime::GetCurrentTime();
	if(FALSE == SaveUserValInfo2DB(m_strAddValClientID, iOldeValue, ADD_SCORE, m_nAddVal, iNewValue, curtime))
	{
		MessageBox(_T("保存数据失败,请重试!"));
		return;
	}

	//Save the current value to Current Value Table
	if(FALSE == SaveCurValuInfo2DB(m_strAddValClientID, iNewValue))
	{
		MessageBox(_T("保存数据失败,请重试!"));
		return;
	}

	//Send msg to client	
	if(NULL != m_pServerSocket)
	{
		//The first time to add value, active the client
		if(CS_ACTIVED != m_clientData[uClientID-MIN_CLIENT_NO].eState)
		{
			ActiveInfo stuActive;
			stuActive.uMsgType = SMT_STC_ACTIVECMD;
			stuActive.uClientID = uClientID;
			stuActive.fValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;

			m_pServerSocket->SendData(&stuActive, sizeof(stuActive));
		}
		//The client is actived, just add value
		else
		{
			UserValueInfo stuUserValue;
			stuUserValue.uMsgType = SMT_STC_USERVALUEINFO;
			stuUserValue.uClientID = uClientID;
			stuUserValue.fValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;
		
			m_pServerSocket->SendData(&stuUserValue, sizeof(stuUserValue));
		}
	}		

	CString strNotify;
	strNotify.Format("提示：用户%s成功上分%d", m_strAddValClientID, m_nAddVal);
	MessageBox(strNotify, "提示", MB_OK);
}
//--------------------------------------------------
//	Name:OnButtonSubtractScore
//	Introduce:To submit the score
//--------------------------------------------------
void CRussiaServerDlg::OnButtonSubtractScore() 
{
	UpdateData(TRUE);

	//Check the select is connected or not
	UINT uClientID = _ttoi(m_strAddValClientID);
	if(FALSE == ClientIDIsLegal(uClientID))
	{
		CString strError;
		strError.Format(_T("编号为:%s 的客户端无效!"), m_strAddValClientID);
		MessageBox(strError);
		return;
	}

	//Check the input value
	if((m_nSubVal<=0) || (m_nSubVal>=100000))
	{
		MessageBox("请输入1—100000之间的数字", "提示", MB_ICONWARNING);
		return;
	}

	//To confirm the operation
	if(IDOK != MessageBox("确认退分吗？", "提示", MB_OKCANCEL))
		return;

	//The sub value is wrong
	if(m_nSubVal > m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal)
	{
		CString str;
		str.Format(_T("用户[%s]当前剩余分数不足%d，请重新操作"), m_strSubValClientID, m_nSubVal);
		MessageBox(str, _T("错误"), MB_ICONWARNING);
		return;
	}

	//Olde value
	int iOldeValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;

	//Update the user value in memory
	UpdateUserValue(uClientID, SUB_SCORE, m_nSubVal);

	//Save the operation to User Value Information table
	int iNewValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;
	CTime curtime = CTime::GetCurrentTime();
	if(FALSE == SaveUserValInfo2DB(m_strSubValClientID, iOldeValue, SUB_SCORE, m_nSubVal, iNewValue, curtime))
	{
		MessageBox(_T("保存数据失败, 请重试!"));
		return;
	}

	//Save the current value to Current Value Table
	if(FALSE == SaveCurValuInfo2DB(m_strSubValClientID, m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal))
	{
		MessageBox(_T("保存数据失败, 请重试!"));
		return;
	}

	//Send msg to client	
	if(NULL != m_pServerSocket)
	{
		UserValueInfo stuUserValue;
		stuUserValue.uMsgType = SMT_STC_USERVALUEINFO;
		stuUserValue.uClientID = uClientID;
		stuUserValue.fValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;
	
		m_pServerSocket->SendData(&stuUserValue, sizeof(stuUserValue));
	}

	CString strNotify;
	strNotify.Format("提示：用户%s成功退分%d",m_strSubValClientID, m_nSubVal);
	MessageBox(strNotify,"提示",MB_OK);	
}
//--------------------------------------------------
//	Name:OnTimer
//	Introduce:WM_TIMER message handler 
//--------------------------------------------------
void CRussiaServerDlg::OnTimer(UINT nIDEvent) 
{	
	switch(nIDEvent)
	{
	case TT_CHECKTIMER:		//The checking timer, for server state
		break;

	case TT_NEW_ROUND:		//产生新一轮结果，打印路单，发送下注命令和当前局数
		{
			//Modified by Youfang Yao
			KillTimer(TT_NEW_ROUND);

			GenerateOneRound();
			m_curRoundNo++;
			m_curInningNo = 0;
			
			// set cur round & inning label
			CString strShow;
			strShow.Format("%d", m_curRoundNo);
			SetDlgItemText(IDC_STATIC_ROUND, strShow);
			strShow.Format("%d", m_nNumOfCurInning);
			SetDlgItemText(IDC_STATIC_INNING ,strShow);
			
			PrintOneRound();
			
			//Send Msg to Client for inform them to bet and set bet timer in change state
			ChangeServerState(SS_BETTING);
		}
		break;		
		
	case TT_BETTIMER:		//Betting operation timer
		{
			m_iCurBetTimeLen--;
			if(m_iCurBetTimeLen <= 0)
			{
				KillTimer(TT_BETTIMER);
				m_iCurBetTimeLen = m_iBetTime;
				//
				ChangeServerState(SS_TURNING);
			}
		}
		break;

	case TT_RENDER:			//Render game animate
		{
			if(FALSE == m_bRenderBusy && m_bCanRenderNextFrame)
			{
				m_bRenderBusy = TRUE;
				RenderTableTurn();
				m_bRenderBusy = FALSE;
				m_bCanRenderNextFrame = FALSE;
			}
		}
		break;

	case TT_SHOWRESULT:		//The timer for clients to show game result
		{
			m_iCurShowResTimeLen--;
			if(m_iCurShowResTimeLen <= 0)
			{				
				//Reset
				KillTimer(TT_SHOWRESULT);
				m_iCurShowResTimeLen = SHOWRESULT_TIMELEN;

				//Change the state to this game end
				ChangeServerState(SS_END);
			}
		}
		break;
		
	case TT_SHOWWINVAL:	//For clients to show win value information time over
		{		
			m_iCurShowValTimeLen--;
			if(m_iCurShowValTimeLen <= 0)
			{
				//Reset
				KillTimer(TT_SHOWWINVAL);
				m_iCurShowValTimeLen = SHOWWINVAL_TIMELEN;

				//当前局到此真正结束
				//此时允许人工中断本轮
				::EnableWindow(GetDlgItem(IDC_BUTTON_STOP)->m_hWnd,TRUE);
				::EnableWindow(GetDlgItem(IDC_BUTTON_PAUSE)->m_hWnd,TRUE);

				//Change the state to READY for next game
				ChangeServerState(SS_READY);
				
				//根据当前轮情况设置timer	
				if (m_curInningNo < m_nNumOfCurInning)
				{
					//当前一轮未结束,继续下一局投注
					//等待时间= 每局时间-下注时间-动画时间-5s显示下注结果预留时间
					//必须>0, if <=0, set to 1
					m_iRestTimeLen = m_iPerInningTime - m_iBetTime - 15 -5;
					if (m_iRestTimeLen <= 0)
						m_iRestTimeLen = 1;
				}
				else
				{
					//可设计成停留30s左右,启动下一轮游戏的定时器
					m_iRestTimeLen = 30;
				}
								
				SetTimer(TT_REST, 1000,NULL);
			}
		}
		break;

	case TT_REST:
		{			
			m_iRestTimeLen--;
			if(m_iRestTimeLen <= 0)
			{
				KillTimer(TT_REST);	
				if (m_curInningNo < m_nNumOfCurInning)
				{
					//当前一轮未结束,继续下一局投注
					ChangeServerState(SS_BETTING);
				}
				else
				{
					// 启动下一轮游戏
					OnButtonStart();
				}
			}		
		}		
		break;

	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
//--------------------------------------------------
//	Name:SaveGameRstInfo2DB
//	Introduce:add data to GameResultInfo Table of dbase
//--------------------------------------------------
void CRussiaServerDlg::SaveGameRstInfo2DB(CTime ct)
{
	int curIncome, i;
	LPCTSTR lpTitles[] = {{_T("RoundNo")}, {_T("InningNo")}, {_T("Result")}, {_T("RedBetVal")}, {_T("BlackBetVal")}, {_T("GreenBetVal")}, {_T("Income")}, {_T("DTime")}};		
	LPCTSTR lpValues[8];	
	CString   str[8];

	//Open the Game Result Information table
	CADOTable gameRstInfoTable;
	HRESULT hr = gameRstInfoTable.Open(&m_Connection, g_szGameResultTable);		
	if(FAILED(hr))
	{
		CString strError;
		strError.Format(_T("打开表:%s 错误!"), g_szGameResultTable);
		MessageBox(strError);
		return;
	}

	//玩家的收入是WinOfBet,则场地收入应为-winofbet
	curIncome = 0-WinOfBet(m_stuTotalBetInfo, g_RestColor[m_curInningRstNo]);
	
	str[0].Format(_T("%d"), m_curRoundNo);
	str[1].Format(_T("%d"), m_curInningNo);
	str[2].Format(_T("%d"), m_curInningRstNo);
	str[3].Format(_T("%d"), m_stuTotalBetInfo.iBetOnRed);
	str[4].Format(_T("%d"), m_stuTotalBetInfo.iBetOnBlack);
	str[5].Format(_T("%d"), m_stuTotalBetInfo.iBetOnGreen);
	str[6].Format(_T("%d"), curIncome);	
	str[7].Format("%04d-%02d-%02d %02d:%02d:%02d", ct.GetYear(), ct.GetMonth(), ct.GetDay(),
										   ct.GetHour(), ct.GetMinute(), ct.GetSecond());
	for (i=0; i<8; i++)
	{
		lpValues[i] = str[i];
	}

	hr = gameRstInfoTable.AddRecord(lpTitles, lpValues, 8);
	if(FAILED(hr))
	{
		gameRstInfoTable.Close();
		return;
	}	

	gameRstInfoTable.Close();
	return;
}
//--------------------------------------------------
//	Name:SaveHistroyInfo2DB
//	Introduce:add data to ClientHistoryInfo Table of dbase
//--------------------------------------------------
void CRussiaServerDlg::SaveHistroyInfo2DB(CString strClientID, int preVal, int winVal, CTime ct)
{
	int i;
	LPCTSTR lpTitles[] = {{_T("ClientID")}, {_T("RoundNo")}, {_T("InningNo")}, {_T("Result")}, {_T("BetOnRed")}, {_T("BetOnBlack")}, {_T("BetOnGreen")}, 
						{_T("ValPreBet")}, {_T("WinVal")}, {_T("ValAfterBet")}, {_T("DTime")}};		
	LPCTSTR lpValues[11];	
	CString   str[11];

	//Open the Game Result Information table
	CADOTable clientHistoryTable;
	HRESULT hr = clientHistoryTable.Open(&m_Connection, g_szClientHistroyTable);		
	if(FAILED(hr))
	{
		CString strError;
		strError.Format(_T("打开表:%s 错误!"), g_szClientHistroyTable);
		MessageBox(strError);
		return;
	}
			
	int nClientID = atoi(strClientID);
	
	str[1].Format(_T("%d"), m_curRoundNo);
	str[2].Format(_T("%d"), m_curInningNo);
	str[3].Format(_T("%d"), m_curInningRstNo);
	str[4].Format(_T("%d"), m_clientData[nClientID-201].stuBetInfo.iBetOnRed);
	str[5].Format(_T("%d"), m_clientData[nClientID-201].stuBetInfo.iBetOnBlack);
	str[6].Format(_T("%d"), m_clientData[nClientID-201].stuBetInfo.iBetOnGreen);
	str[7].Format(_T("%d"), preVal);
	str[8].Format(_T("%d"), winVal);
	str[9].Format(_T("%d"), preVal+winVal);
	str[10].Format("%04d-%02d-%02d %02d:%02d:%02d", ct.GetYear(), ct.GetMonth(), ct.GetDay(),
										   ct.GetHour(), ct.GetMinute(), ct.GetSecond());

	lpValues[0] = strClientID;
	for (i=1; i<11; i++)
	{
		lpValues[i] = str[i];
	}

	hr = clientHistoryTable.AddRecord(lpTitles, lpValues, 11);
	if(FAILED(hr))
	{
		clientHistoryTable.Close();
		return;
	}	

	clientHistoryTable.Close();
	return;
}
//--------------------------------------------------
//	Name:ReadData2QueryDlgList
//	Introduce:
//--------------------------------------------------
void CRussiaServerDlg::ReadData2QueryDlgList(CListCtrl* plst)
{
	TCHAR szValue[32] = {_T("\0")};
	HRESULT hr = S_OK;
	int curType;

	//Open the Game Result Information table
	CADOTable userValTable;
	hr = userValTable.Open(&m_Connection, g_szUserValueTable);		
	if(FAILED(hr))
	{
		CString strError;
		strError.Format(_T("打开表:%s 错误!"), g_szUserValueTable);
		MessageBox(strError);
		return;
	}
	
	try
	{
		int iCnt = 0;
		userValTable.MoveFirst();
		while(!userValTable.IsEOF())
		{
			TCHAR szType[32] = {_T("\0")};
			TCHAR szDay[11] = {_T("\0")};
			TCHAR szTime[11] = {_T("\0")};
			userValTable.GetValue(_T("ActionType"), szType);
			curType = atoi(szType);  // it should be 1 byte

			// query add/subtract score, the win operation is missed
			if ((curType == ADD_SCORE) ||(curType == SUB_SCORE))
			{				
				userValTable.GetValue(_T("ClientID"), szValue);
				plst->InsertItem(iCnt, szValue);
				
				//Time Format: 2006/12/20 15:30:30
				userValTable.GetValue(_T("DTime"), szValue);
				memset(szDay, 0, 11);
				memcpy(szDay, szValue, 10);
				plst->SetItemText(iCnt, 1, szDay);

				memset(szTime, 0, 11);
				memcpy(szTime, szValue+10, 10);
				plst->SetItemText(iCnt, 2, szTime);
				
				if (curType == ADD_SCORE)
					plst->SetItemText(iCnt, 3, "上分");	
				else if (curType == SUB_SCORE)
					plst->SetItemText(iCnt, 3, "退分");	
				//else if (curType == WIN_SCORE)
				//	plst->SetItemText(iCnt, 3, "赢分");

				userValTable.GetValue(_T("ActionVal"), szValue);
				plst->SetItemText(iCnt, 4, szValue);	

				iCnt++;
			}

			userValTable.MoveNext();
			
		}
	}
	
	catch(_com_error f)
	{
		userValTable.Close();
	}

	userValTable.Close();
}
//--------------------------------------------------



//--------------------------------------------------
//	Name:ClientIDIsLegal
//	Introduce:To check a client ID is legal or not
//--------------------------------------------------
BOOL CRussiaServerDlg::ClientIDIsLegal(UINT uClientID)
{
	if(uClientID >= MIN_CLIENT_NO && uClientID < (MIN_CLIENT_NO+CLIENT_NO_CNT))
		return TRUE;
	else
		return FALSE;
}
//--------------------------------------------------
//	Name:UpdateUserValue
//	Introduce:Change the server state
//--------------------------------------------------
void CRussiaServerDlg::ChangeServerState(UINT uState)
{
	if(uState >= SS_COUNT)
		return;

	switch(uState)
	{
	case SS_READY:
		break;
		
	case SS_BETTING:		//Is start to betting
		{
			//The Inning count add 1
			m_curInningNo++;

			//The game result for this Inning
			m_curInningRstNo = m_curRoundRstArray[m_curInningNo-1];

			// set cur round & inning label
			CString strShow;
			strShow.Format("%d", m_curInningNo);
			SetDlgItemText(IDC_STATIC_CURINNING, strShow);

			//before notify, we should reset the para in internal memory
			memset(&m_stuTotalBetInfo, 0, sizeof(BetInfo));
			for(UINT uClientID = MIN_CLIENT_NO; uClientID < MIN_CLIENT_NO+CLIENT_NO_CNT; uClientID++)
			{
				m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnBlack = 0;
				m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnGreen = 0;
				m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnRed = 0;
				
				//Send current value to this client
				if(CS_ACTIVED == m_clientData[uClientID-MIN_CLIENT_NO].eState)
				{
					UserValueInfo stuUserValue;
					stuUserValue.uMsgType = SMT_STC_USERVALUEINFO;
					stuUserValue.uClientID = uClientID;
					stuUserValue.fValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;
				
					m_pServerSocket->SendData(&stuUserValue, sizeof(stuUserValue));
				}
			}
			
			//Send start bet message to all clients
			StartBetCmd stuBetCmd;
			stuBetCmd.uMsgType = SMT_STC_STARTBET;
			stuBetCmd.iBetTimeLen = m_iBetTime;
			if(NULL != m_pServerSocket)
			{
				m_pServerSocket->SendData(&stuBetCmd, sizeof(StartBetCmd));
			}

			//Start betting timer
			m_iCurBetTimeLen = m_iBetTime;
			SetTimer(TT_BETTIMER, 1000, NULL);			
		}
		break;
		
	case SS_TURNING:			//Betting operation is end, start the table turning animate
		{
			//Send end betting message to all clients
			EndBetCmd stuEndBetCmd;
			stuEndBetCmd.uMsgType = SMT_STC_ENDBET;
			if(NULL != m_pServerSocket)
			{
				m_pServerSocket->SendData(&stuEndBetCmd, sizeof(EndBetCmd));
			}

			//Reset the table turn params			
			m_bCanRenderNextFrame = TRUE;
			m_bRenderBusy = FALSE;
			m_fTableTotalFrm = 0.0f;
			m_fAngleSpeedDelta = 0.0f;
			m_fCurTableSpeed = TABLE_INIT_ANGLE_SPEED;
			m_fCurTableAngle = g_fStartAngle[m_curInningRstNo];
			//
			m_fCurBallLineSpeed = BALL_INIT_SPEED;
			m_fBallLineSpeedDelta = 0.0f;
			m_fCurBallPosY = BALL_INIT_POS;
			m_fBallAngleSpeedDelta = 0.0f;
			m_fCurBallAngleSpeed = 0.0f;	
			m_fCurBallAngle = 0.0f;
			m_fCurBallFrm = 0;
			m_fBallStopFrm = 0;
			m_bGameResultIsSent = FALSE;
			m_fCurBallRadius = BALL_TURN_MAX_R;

			//Recalc the table turn params
			CalcTableTurnParams();

			//Start the render timer
			SetTimer(TT_RENDER, 25, NULL);
		}
		break;

	case SS_SHOWING:		//Table turning animate is end, now to showing the game result for SHOWRESULT_TIMELEN
		{
			//Stop the table turning animate render timer
			KillTimer(TT_RENDER);

			//Send the new status to all clients
			ShowResultCmd stuShowResult; 
			stuShowResult.uMsgType = SMT_STC_SHOWRESULT;			
			if(NULL != m_pServerSocket)
			{
				m_pServerSocket->SendData(&stuShowResult, sizeof(ShowResultCmd));
			}

			//Start the Game Showwing timer for SHOWRESULT_TIMELEN
			SetTimer(TT_SHOWRESULT, 1000, NULL);
		}
		break;
		
	case SS_END:			//Current game is end, to show result and win value
		{
			CTime curtime = CTime::GetCurrentTime();			
			
			//Send game end message to all clients
			GameEndCmd stuGameEndCmd;
			stuGameEndCmd.uMsgType = SMT_STC_GAMEEND;
			stuGameEndCmd.fAnlgeShowRes = g_fStartAngle[m_curInningRstNo];
			m_pServerSocket->SendData(&stuGameEndCmd, sizeof(GameEndCmd));				

			// 1. add GameResultInfo table to db
			SaveGameRstInfo2DB(curtime);
			
			//To calc this game result information for all clients
			for(UINT uClientID = MIN_CLIENT_NO; uClientID < MIN_CLIENT_NO+CLIENT_NO_CNT; uClientID++)
			{	
				//Only calc the submitted clients result
				if(CS_SUBMITTED == m_clientData[uClientID-MIN_CLIENT_NO].eState)
				{	
					WinValueInfo stuWinValue;					
					stuWinValue.uMsgType = SMT_STC_WINVALUE;
					stuWinValue.uClientID = uClientID;
					stuWinValue.iWinValue = GetWinVal(m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo, g_RestColor[m_curInningRstNo]);
					m_pServerSocket->SendData(&stuWinValue, sizeof(WinValueInfo));

					//The REAL win value of this game
					int iWinValue = WinOfBet(m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo, g_RestColor[m_curInningRstNo]);

					// 下注之前分= 当前分 + 压注分
					int iPreBetValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal + SumOfBet(m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo);

					CString strClientID;						
					strClientID.Format(_T("%d"), uClientID);

					// 2. add UserValueInfo table to db
					SaveCurValuInfo2DB(strClientID, iPreBetValue + iWinValue);
					
					// 3. add UserValueInfo table to db
					SaveUserValInfo2DB(strClientID, iPreBetValue, WIN_SCORE, iWinValue, iPreBetValue + iWinValue, curtime);

					// 4. add ClientHistroyInfo table to db
					SaveHistroyInfo2DB(strClientID, iPreBetValue, iWinValue, curtime);

					// 5. update the internal memory 
					m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal = iPreBetValue+iWinValue;
					memset(&m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo, 0, sizeof(BetInfo));

					m_clientData[uClientID-MIN_CLIENT_NO].eState = CS_ACTIVED;
					UpdateList(uClientID);
				}
			}		
			
			//Reset server flags
			m_iTotalSubmitCnt = 0;
			memset(&m_stuTotalBetInfo, 0, sizeof(m_stuTotalBetInfo));

			//Here, we let clients to show win value information for SHOWWINVAL_TIMELEN seconds
			SetTimer(TT_SHOWWINVAL, 1000, NULL);
		}
		break;
	}

	//Show the state information on UI
	LPCTSTR lpStateStr = GetServerStateStr(uState);
	SetDlgItemText(IDC_STATIC_STATE, lpStateStr);

	m_eCurState = (ServerState)uState;
}
//--------------------------------------------------
//	Name:UpdateUserValue
//	Introduce:Update the client current value information
//--------------------------------------------------
BOOL CRussiaServerDlg::UpdateUserValue(UINT uClientID, UserActionType_t eActionType, int iActionVal)
{
	if(FALSE == ClientIDIsLegal(uClientID))
		return FALSE;

	int iIndex = uClientID-MIN_CLIENT_NO;
	switch(eActionType)
	{
	case ADD_SCORE:
		m_clientData[iIndex].iRemainVal += iActionVal;
		break;
	case SUB_SCORE:
		m_clientData[iIndex].iRemainVal -= iActionVal;
		break;		
	case WIN_SCORE:
		m_clientData[iIndex].iRemainVal += iActionVal;
		break;		
	case BET_SCORE_RED:
		m_clientData[iIndex].iRemainVal -= iActionVal;
		m_clientData[iIndex].stuBetInfo.iBetOnRed += iActionVal;
		m_stuTotalBetInfo.iBetOnRed += iActionVal;
		break;	
	case BET_SCORE_BLACK:
		m_clientData[iIndex].iRemainVal -= iActionVal;
		m_clientData[iIndex].stuBetInfo.iBetOnBlack += iActionVal;
		m_stuTotalBetInfo.iBetOnBlack += iActionVal;
		break;
	case BET_SCORE_GREEN:
		m_clientData[iIndex].iRemainVal -= iActionVal;
		m_clientData[iIndex].stuBetInfo.iBetOnGreen += iActionVal;
		m_stuTotalBetInfo.iBetOnGreen += iActionVal;
		break;
	default:
		return FALSE;
	}

	UpdateList(uClientID);
	return TRUE;
}
//--------------------------------------------------
//	Name:SaveCurValuInfo2DB
//	Introduce:Save current user value information to databse
//--------------------------------------------------
BOOL CRussiaServerDlg::SaveCurValuInfo2DB(CString strClientID, int iCurValue)
{
	if(iCurValue < 0)
		return FALSE;

	UINT uClientID = atoi(strClientID);

	//Open the User Current Value Information table
	CADOTable curValueTable;
	HRESULT hr = curValueTable.Open(&m_Connection, g_szCurValueInfoTable);		
	if(FAILED(hr))
	{
		CString strError;
		strError.Format(_T("打开表:%s 错误!"), g_szCurValueInfoTable);
		MessageBox(strError);
		return FALSE;
	}

	//Prepare save data to database
	LPCTSTR lpTitles[] = {{_T("ClientID")}, {_T("RemainVal")}};		
	LPCTSTR lpValues[2];

	//ClientID
	lpValues[0] = strClientID;

	//RemainVal
	CString strValue;
	strValue.Format(_T("%d"), iCurValue);	
	lpValues[1] = strValue;

	//Find the Client ID record
	CString strFind;
	strFind.Format(_T("ClientID=%d"), uClientID);
	hr = curValueTable.FindRecord(strFind);		
	if(FAILED(hr))
	{
		curValueTable.Close();
		return FALSE;
	}

	//To fix the record
	if(curValueTable.GetRecordsetCnt() > 0)
	{
		hr = curValueTable.EditRecord(lpTitles, lpValues, 2);
		if(FAILED(hr))
		{
			curValueTable.Close();
			return FALSE;
		}	
	}
	//To add a new record
	else
	{
		hr = curValueTable.AddRecord(lpTitles, lpValues, 2);
		if(FAILED(hr))
		{
			curValueTable.Close();
			return FALSE;
		}	
	}

	curValueTable.Close();
	return TRUE;
}
//--------------------------------------------------
//	Name:SaveUserValInfo2DB
//	Introduce:add data to table UserValueInfo of dbase
//--------------------------------------------------
BOOL CRussiaServerDlg::SaveUserValInfo2DB(CString strClientID, int iOldValue, UserActionType_t action, int iActionValue, int iNewValue, CTime ct)
{
	//Open the User Value Information table
	CADOTable userValueTable;
	HRESULT hr = userValueTable.Open(&m_Connection, g_szUserValueTable);		
	if(FAILED(hr))
	{
		CString strError;
		strError.Format(_T("打开表:%s 错误!"), g_szUserValueTable);
		MessageBox(strError);
		return FALSE;
	}

	//Prepare save the information to databse
	LPCTSTR lpTitles[] = {{_T("ClientID")}, {_T("RemainVal")}, {_T("ActionType")}, {_T("ActionVal")}, {_T("NewVal")}, {_T("DTime")}};		
	LPCTSTR lpValues[6];
	CString s1, s2, s3, s4, s5;	
	
	//ClientID
	lpValues[0] = strClientID;

	//RemainVal
	int nIPAddr = atoi(strClientID);
	s1.Format(_T("%d"), iOldValue);	
	lpValues[1] = s1;

	//ActionType
	s2.Format(_T("%d"), action);	
	lpValues[2] = s2;

	//ActionVal
	s3.Format(_T("%d"), iActionValue);	
	lpValues[3] = s3;

	//NewVal
	s4.Format(_T("%d"), iNewValue);	
	lpValues[4] = s4;

	//DTime
	s5.Format("%04d-%02d-%02d %02d:%02d:%02d", ct.GetYear(), ct.GetMonth(), ct.GetDay(),
										   ct.GetHour(), ct.GetMinute(), ct.GetSecond());
	lpValues[5] = s5;

	hr = userValueTable.AddRecord(lpTitles, lpValues, 6);
	if(FAILED(hr))
	{
		userValueTable.Close();
		return FALSE;
	}	

	userValueTable.Close();
	return TRUE;
}
//--------------------------------------------------
//	Name:CreateNetwork
//	Introduce:To create the network connect
//--------------------------------------------------
BOOL CRussiaServerDlg::CreateNetwork()
{	
	//Get the server IP address	
	WSADATA stuWSData;
	PHOSTENT pHostInfo = NULL; 
	TCHAR szName[255] = {_T("\0")};
	TCHAR szIPAdress[16] = {_T("\0")};
	if(WSAStartup(MAKEWORD(2, 0), &stuWSData) == 0)
	{
		 if(gethostname(szName, sizeof(szName)) == 0) 
		 { 
			if((pHostInfo = gethostbyname(szName)) != NULL) 
			{ 
				LPCTSTR lpIP = inet_ntoa (*(struct in_addr *)*pHostInfo->h_addr_list); 
				lstrcpy(szIPAdress, lpIP);
			} 
		 }
	}

	//Create the server socket
	m_pServerSocket = new CServerSocket();
	if(!m_pServerSocket->Create(m_hWnd, m_uPort))
	{
		MessageBox(_T("Create the server socket error!"));
		return FALSE;
	}

	return TRUE;
}
//--------------------------------------------------
//	Name:OnClientConnected
//	Introduce:SKT_MSG_CLIENTCONNECTED message handler
//---------------------------------------------------
HRESULT CRussiaServerDlg::OnClientConnected(WPARAM wParam, LPARAM lParam)
{
	//A new client has connected, and send command to get its information
	int iClientID = wParam;
	if(NULL != m_pServerSocket)
	{
		GetClientInfo stuGetClientInfo;
		stuGetClientInfo.uMsgType = SMT_STC_GETCLIENTINFO;

		m_pServerSocket->SendData(&stuGetClientInfo, sizeof(stuGetClientInfo), iClientID);
	}

	return S_OK;
}
//---------------------------------------------------
//	Name:OnRecievedMsg
//	Introduce:SKT_MSG_RECEIVEDMSG message handler
//---------------------------------------------------
HRESULT CRussiaServerDlg::OnRecievedMsg(WPARAM wParam, LPARAM lParam)
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
		case SMT_CTS_CLIENTINFO:	//Client information data message
			iReadCnt = ProcClientInfo((LPCLIENTINFO)pDataBuff);
			break;
		case SMT_CTS_ASKFORACTCMD:	//The client send msg to ask for server to active itself, wangdapeng add 2007/01/19
			iReadCnt = ProcAskActiveCmd((LPASKFORACTIVECMD)pDataBuff);
			break;	
		case SMT_CTS_HASACTIVED:	//The client has been active message
			iReadCnt = ProcHasActivedCmd((LPHASACTIVEDCMD)pDataBuff);
			break;
		case SMT_CTS_SUBMITBET:		//The client submitted bet information message
			iReadCnt = ProcSubmitBetInfo((LPSUBMITBETINFO)pDataBuff);
			break;
		case SMT_CTS_RENDEREND:		//The client render end message
			iReadCnt = ProcRenderEndCmd((LPRENDERENDCMD)pDataBuff);
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
//---------------------------------------------------
//	Name:ProcClientInfo
//	Introduce:To deal with client information message
//---------------------------------------------------
int CRussiaServerDlg::ProcClientInfo(LPCLIENTINFO lpClientInfo)
{
	if(NULL == lpClientInfo)
		return 0;

	UINT uClientID = lpClientInfo->uClientID;
	if(ClientIDIsLegal(uClientID))
	{
		//Change the client state to Connected
		if(CS_UNCONNECT == m_clientData[uClientID-MIN_CLIENT_NO].eState)
		{
			m_clientData[uClientID-MIN_CLIENT_NO].eState = CS_CONNECTED;			
			UpdateList(uClientID);
		}
	}

	return sizeof(ClientInfo);
}

//---------------------------------------------------
//	Name:ProcAskActiveCmd
//	Introduce:To deal with the client ask for active itself
//			 need to notice the client: remain value, BetBaseLine, RedMax, BlackMax, GreenMax
//	wangdapeng add 2007/01/19
//---------------------------------------------------
int CRussiaServerDlg::ProcAskActiveCmd(LPASKFORACTIVECMD lpAskActiveCmd)
{
	if(NULL == lpAskActiveCmd)
		return 0;

	UINT uClientID = lpAskActiveCmd->uClientID;
	if(ClientIDIsLegal(uClientID))
	{
		//Change the client state to Actived
		if(CS_CONNECTED == m_clientData[uClientID-MIN_CLIENT_NO].eState)
		{
			//Send the Active Cmd to shis client
			ActiveInfo stuActive;
			stuActive.uMsgType = SMT_STC_ACTIVECMD;
			stuActive.uClientID = uClientID;
			stuActive.fValue = m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal;
			if(m_clientData[uClientID-MIN_CLIENT_NO].iRemainVal > 0)
			{
				stuActive.bCanActived = TRUE;
				m_pServerSocket->SendData(&stuActive, sizeof(stuActive));

				//Active this client, then send the server setting to it
				ServerSetInfo stuSetInfo;
				stuSetInfo.uMsgType = SMT_STC_SERVERSETINFO;

				stuSetInfo.iMinBet = m_iBetMin;
				stuSetInfo.iMaxBet = m_iBetMax;
				//
				stuSetInfo.iMaxRedBet = m_iMaxRedBet;
				stuSetInfo.iMaxBlackBet = m_iMaxBlackBet;
				stuSetInfo.iMaxGreenBet = m_iMaxGreenBet;

				m_pServerSocket->SendData(&stuSetInfo, sizeof(stuSetInfo));
			}
			else
			{
				stuActive.bCanActived = FALSE;
				m_pServerSocket->SendData(&stuActive, sizeof(stuActive));
			}
		}
	}

	return sizeof(AskForActiveCmd);
}

//---------------------------------------------------
//	Name:ProcHasActivedCmd
//	Introduce:To deal with the client has been actived command
//---------------------------------------------------
int CRussiaServerDlg::ProcHasActivedCmd(LPHASACTIVEDCMD lpHasActivedCmd)
{
	if(NULL == lpHasActivedCmd)
		return 0;

	UINT uClientID = lpHasActivedCmd->uClientID;
	if(ClientIDIsLegal(uClientID))
	{
		//Change the client state to Actived
		if(CS_CONNECTED == m_clientData[uClientID-MIN_CLIENT_NO].eState)
		{
			m_clientData[uClientID-MIN_CLIENT_NO].eState = CS_ACTIVED;			
			UpdateList(uClientID);
		}
	}

	return sizeof(HasActivedCmd);
}
//---------------------------------------------------
//	Name:ProcSubmitBetInfo
//	Introduce:To deal with client submitted bet information
//---------------------------------------------------
int CRussiaServerDlg::ProcSubmitBetInfo(LPSUBMITBETINFO lpSubmitBetInfo)
{
	if(NULL == lpSubmitBetInfo)
		return 0;

	UINT uClientID = lpSubmitBetInfo->uClientID;

	//The client ID is not legal
	if(FALSE == ClientIDIsLegal(uClientID))
		return sizeof(SubmitBetInfo);

	//The client state should be CS_ACTIVED(还没下注) or CS_SUBMITTED(已经下注)
	//wangdapeng modify
	if((CS_ACTIVED != m_clientData[uClientID-MIN_CLIENT_NO].eState) 
		&& (CS_SUBMITTED != m_clientData[uClientID-MIN_CLIENT_NO].eState))
	{
		return sizeof(SubmitBetInfo);
	}
	
	//The new state for this client
	if (m_clientData[uClientID-MIN_CLIENT_NO].eState != CS_SUBMITTED)
	{
		m_clientData[uClientID-MIN_CLIENT_NO].eState = CS_SUBMITTED;
		m_iTotalSubmitCnt++;
	}

	//Modified by Youfang Yao
	float fCurBetVal = lpSubmitBetInfo->fBetRedVal - m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnRed;
	UpdateUserValue(uClientID, BET_SCORE_RED, fCurBetVal);
	//
	fCurBetVal = lpSubmitBetInfo->fBetBlackVal - m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnBlack;
	UpdateUserValue(uClientID, BET_SCORE_BLACK, fCurBetVal);
	//
	fCurBetVal = lpSubmitBetInfo->fBetGreenVal - m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnGreen;
	UpdateUserValue(uClientID, BET_SCORE_GREEN, fCurBetVal);

/*	
	//The client has betted on red
	if(lpSubmitBetInfo->fBetRedVal > m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnRed)
	{
		UpdateUserValue(uClientID, BET_SCORE_RED, lpSubmitBetInfo->fBetRedVal-m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnRed);
	}
	//The client has betted on black
	if(lpSubmitBetInfo->fBetBlackVal > m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnBlack)
	{
		UpdateUserValue(uClientID, BET_SCORE_BLACK, lpSubmitBetInfo->fBetBlackVal - m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnBlack);
	}
	//The client has betted on green
	if(lpSubmitBetInfo->fBetGreenVal > m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnGreen)
	{
		UpdateUserValue(uClientID, BET_SCORE_GREEN, lpSubmitBetInfo->fBetGreenVal - m_clientData[uClientID-MIN_CLIENT_NO].stuBetInfo.iBetOnGreen);
	}
*/	
	//Send the new total bet information to all clients
	TotalBetInfo stuCurBetInfo;
	stuCurBetInfo.uMsgType = SMT_STC_TOTALBETINFO;
	stuCurBetInfo.fTotalBetRedVal = m_stuTotalBetInfo.iBetOnRed;
	stuCurBetInfo.fTotalBetBlackVal = m_stuTotalBetInfo.iBetOnBlack;
	stuCurBetInfo.fTotalBetGreenVal = m_stuTotalBetInfo.iBetOnGreen;
	//
	m_pServerSocket->SendData(&stuCurBetInfo, sizeof(stuCurBetInfo));

	return sizeof(SubmitBetInfo);
}
//---------------------------------------------------
//	Name:ProcRenderEndCmd
//	Introduce:To deal with client render end command
//---------------------------------------------------
int	CRussiaServerDlg::ProcRenderEndCmd(LPRENDERENDCMD lpRenderEndCmd)
{
	if(NULL == lpRenderEndCmd)
		return 0;

	UINT uClientID = lpRenderEndCmd->uClientID;

	//The client ID is not legal
	if(FALSE == ClientIDIsLegal(uClientID))
		return sizeof(RenderEndCmd);

	//The client state is wrong
	if(CS_SUBMITTED != m_clientData[uClientID-MIN_CLIENT_NO].eState)
		return sizeof(RenderEndCmd);

	m_iTotalRenderEndCnt++;

	//All correct client has render end, render next frame
	if(m_iTotalRenderEndCnt >= m_iTotalSubmitCnt)
	{
		m_iTotalRenderEndCnt = 0;
		m_bCanRenderNextFrame = TRUE;
	}

	return sizeof(RenderEndCmd);
}
//---------------------------------------------------
//	Name:RenderTableTurn
//	Introduce:Render game animate
//---------------------------------------------------
void CRussiaServerDlg::RenderTableTurn()
{
	float fCurTableAngleSpeed = 0.0f;
	float fCurAngle = 0.0f;
	float fBallPosx = 0.0f;
	float fBallPosy = 0.0f;

	//Game animate is running
	if(TRUE == CalcCurTableAngle(fCurTableAngleSpeed, fCurAngle))
	{
		//Get the ball position
		CalcCurBallPos(fCurTableAngleSpeed, fBallPosx, fBallPosy);

		//Send the render information to all clients
		RenderInfo stuRenderInfo;
		stuRenderInfo.uMsgType = SMT_STC_RENDERCMD;
		stuRenderInfo.fTableAngle = fCurAngle;
		stuRenderInfo.fBallImage = 0;
		stuRenderInfo.fBallPosX = fBallPosx;
		stuRenderInfo.fBallPosY = fBallPosy;
		if(NULL != m_pServerSocket)
		{
			m_pServerSocket->SendData(&stuRenderInfo, sizeof(RenderInfo));
		}

		//If the ball stops, send the game result to clients
		if(m_fCurBallAngleSpeed <= 0.0f)
		{
			if(FALSE == m_bGameResultIsSent)
			{

				GameResultInfo stuResultInfo;
				stuResultInfo.uMsgType = SMT_STC_GAMERESULT;
				stuResultInfo.iGameNo = m_curInningNo;
				stuResultInfo.uResult = m_curInningRstNo;	
				stuResultInfo.uFlg = g_RestColor[m_curInningRstNo];
				if(NULL != m_pServerSocket)
				{
					m_pServerSocket->SendData(&stuResultInfo, sizeof(GameResultInfo));
					m_bGameResultIsSent = TRUE;
				}
			}
		}
	}
	//本局游戏动画结束，做必要的计算和处理后，开始下一局游戏的下注
	//The table turning animate is end, now change the state to show game result
	else
	{
		//Stop control client render
		KillTimer(TT_RENDER);
	
		//Change the server state to show game result
		ChangeServerState(SS_SHOWING);		
	}
}
//---------------------------------------------------
//	Name:CalcTableTurnParams
//	Introduce:To calc table turn params
//---------------------------------------------------
void CRussiaServerDlg::CalcTableTurnParams()
{
	//The total frame count
	m_fTableTotalFrm = 2.0f*TABLE_TOTAL_ANGLE / TABLE_INIT_ANGLE_SPEED;

	//The speed delta for the angle
	m_fAngleSpeedDelta = TABLE_INIT_ANGLE_SPEED / m_fTableTotalFrm;

	//To calc while frame for the ball stop
	float fDelta = (2.0f*TABLE_INIT_ANGLE_SPEED)*(2.0f*TABLE_INIT_ANGLE_SPEED) - 4.0f*m_fAngleSpeedDelta*(2.0f*ANGLE_BALL_TO_STOP);
	m_fBallStopFrm = (2.0f*TABLE_INIT_ANGLE_SPEED - sqrt(fDelta)) / (2.0f*m_fAngleSpeedDelta);

/*
	//Calc the ball angle speed delta
	float fBallRotateFrmCnt = m_fBallStopFrm - BALL_DOWN_FRM;
	m_fBallAngleSpeedDelta = (2.0f*BALL_TURN_TOTAL_ANGLE) / (fBallRotateFrmCnt*fBallRotateFrmCnt);
//	m_fBallAngleSpeedDelta = BALL_TURN_TOTAL_ANGLE/(2.0f*fBallRotateFrmCnt*fBallRotateFrmCnt);

	//Calc the ball init angle speed
	m_fCurBallAngleSpeed = m_fBallAngleSpeedDelta*fBallRotateFrmCnt;
	m_fCurBallAngle = 180.0f;

	//To calc the line speed delta for ball down
	float fBallTurnSpeed = (m_fCurBallAngleSpeed*ANG_TO_RAD)*BALL_TURN_R;
	m_fBallLineSpeedDelta = (fBallTurnSpeed-BALL_INIT_SPEED) / BALL_DOWN_FRM;
*/

	//To calc the line speed delta for ball down
	float fBallDownFrmCnt = (2.0f*(BALL_INIT_POS-BALL_TURN_POS)) / (BALL_INIT_SPEED + BALL_TURN_SPEED);
	m_fBallLineSpeedDelta = (BALL_TURN_SPEED-BALL_INIT_SPEED) / fBallDownFrmCnt;

	//Calc the ball angle speed delta
	float fBallRotateFrmCnt = m_fBallStopFrm - fBallDownFrmCnt;
	m_fBallAngleSpeedDelta = (2.0f*BALL_TURN_TOTAL_ANGLE) / (fBallRotateFrmCnt*fBallRotateFrmCnt);

	//Calc the ball init angle speed
	m_fCurBallAngleSpeed = m_fBallAngleSpeedDelta*fBallRotateFrmCnt;
	m_fCurBallAngle = 180.0f;
}
//---------------------------------------------------
//	Name:CalcCurTableAngle
//	Introduce:Calc current turn table angle
//---------------------------------------------------
BOOL CRussiaServerDlg::CalcCurTableAngle(float &fCurTableAngleSpeed, float &fAngle)
{
	m_fCurTableSpeed -= m_fAngleSpeedDelta;
	if(m_fCurTableSpeed <= 0.0f)
	{
		m_fCurTableSpeed = 0.0f;
		return FALSE;
	}

	m_fCurTableAngle -= m_fCurTableSpeed;
	if(m_fCurTableAngle < -360)
		m_fCurTableAngle += 360;

	fCurTableAngleSpeed = m_fCurTableSpeed;
	fAngle = m_fCurTableAngle;

	return TRUE;
}
//---------------------------------------------------
//	Name:CalcCurBallPos
//	Introduce:Calc current ball position
//---------------------------------------------------
void CRussiaServerDlg::CalcCurBallPos(float fCurTableAngleSpeed, float &fPosX, float &fPosY)
{
	m_fCurBallFrm++;

	static long lDownFrm = 0;

	//1.Down
	if(m_fCurBallPosY > BALL_TURN_POS)
	{
		lDownFrm++;

		m_fCurBallPosY -= m_fCurBallLineSpeed;
		//
		fPosX = -m_fCurBallRadius;//-BALL_TURN_R;
		fPosY = m_fCurBallPosY;
		//
		m_fCurBallLineSpeed += m_fBallLineSpeedDelta;
		if(m_fCurBallLineSpeed <= 0)
			m_fCurBallLineSpeed = 0.0f;
	}
	//2.Roll
	else if(m_fCurBallAngleSpeed > 0)
	{
		float fRadiusDelta = (BALL_TURN_MAX_R - BALL_TURN_MIN_R)/(m_fBallStopFrm - lDownFrm);
		m_fCurBallRadius -= fRadiusDelta;

		m_fCurBallAngle += m_fCurBallAngleSpeed;
		if(m_fCurBallAngle > 360)
			m_fCurBallAngle -= 360;
		//
		fPosX = m_fCurBallRadius/*BALL_TURN_R*/ * cosf(m_fCurBallAngle*ANG_TO_RAD);
		fPosY = m_fCurBallRadius/*BALL_TURN_R*/ * sinf(m_fCurBallAngle*ANG_TO_RAD);

		m_fCurBallAngleSpeed -= m_fBallAngleSpeedDelta;
		if(m_fCurBallAngleSpeed <= 0)
			m_fCurBallAngleSpeed = 0;

		if(m_fCurBallFrm >= m_fBallStopFrm)
		{
			m_fCurBallAngleSpeed = 0.0f;
			m_fCurBallAngle = 90;
			m_fCurBallFrm = 0.0f;
		}
	}
	//3.Move with table
	else
	{
		m_fCurBallAngle -= fCurTableAngleSpeed;
		if(m_fCurBallAngle < 360)
			m_fCurBallAngle += 360;

		if(m_fCurBallFrm >= m_fBallStopFrm)
		{
			m_fCurBallAngleSpeed = 0.0f;
			m_fCurBallAngle = 90;
			m_fCurBallFrm = 0.0f;
		}

		//
		fPosX = /*BALL_TURN_R*/m_fCurBallRadius * cosf(m_fCurBallAngle*ANG_TO_RAD);
		fPosY = /*BALL_TURN_R*/m_fCurBallRadius * sinf(m_fCurBallAngle*ANG_TO_RAD);
	}
}
//---------------------------------------------------




//---------------------------------------------------
void CRussiaServerDlg::PrintOneRound() 
{
	HDC hdcprint; // 定义一个设备环境句柄
	CDC* pPrintDC=new CDC;
	CString str,str1;
	TEXTMETRIC tm;
	int nHeight;
	int posStartX1, posStartX2, posStartY;
	ResultFlg rstColor;

	//定义一个打印作业
	static DOCINFO di={sizeof(DOCINFO),"printer",NULL}; 
	
	//获得打印机设备句柄
	CPrintDialog dlg(FALSE,PD_ALLPAGES|PD_ALLPAGES|PD_NOPAGENUMS,NULL); //需要设置PD_RETURNDC，见MSDN
	dlg.GetDefaults();//不显示打印对话框
	hdcprint=dlg.m_pd.hDC;
	
	if( hdcprint == NULL )
	{
		MessageBox("没有默认打印机!");
		return;
	}
	
	pPrintDC->Attach(hdcprint);
	
	//a point is 1/72 inch, a twip is 1/1440 inch，即1点为20个twip
	//y坐标向上为正方向，即顶端y是0，底端y是负的
	pPrintDC->SetMapMode(MM_TWIPS);	

	CFont font;
	// 14-point fixed-pitch font
	font.CreateFont(-280, 0, 0, 0, 700, FALSE, FALSE,
	                0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	                DEFAULT_PITCH | FF_MODERN, "宋体");
	                // Courier New is a TrueType font

	CFont* pOldFont=(CFont*)( pPrintDC->SelectObject(&font) );
	
	CPen pen(PS_SOLID,-20,RGB(0,0,0));
	CPen* pOldPen=(CPen*)( pPrintDC->SelectObject(&pen) );

	int nMonth,nDay,nHour,nMinute;
	CTime t;
	t= CTime::GetCurrentTime();
	
	if(pPrintDC->StartDoc(&di)>0)
	{  
		//开始执行一个打印作业
		pPrintDC->StartPage(); //打印机走纸,一页开始

		if(1) //m_bWinMore==FALSE)
		{
			nMonth=t.GetMonth();
			nDay=t.GetDay(); 
			nHour=t.GetHour();
			nMinute=t.GetMinute();
			
			str.Format("第 %d 靴路单                              打印路单时间: %d月%d日%d时%d分",m_curRoundNo,nMonth,nDay,nHour,nMinute);
			pPrintDC->TextOut(0,0,str); 
			font.DeleteObject();
			font.CreateFont(-220, 0, 0, 0, 500, FALSE, FALSE,
							0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
							CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
							DEFAULT_PITCH | FF_MODERN, "宋体");
			CPoint point=CPoint(180*7,-360);//距离左边2英寸的位置
	
			pPrintDC->SelectObject(&font);
			pPrintDC->GetTextMetrics(&tm);
			nHeight = tm.tmHeight + tm.tmExternalLeading;
			str="       局 数   		开奖结果  ";		

			//向下窜1行
			point.y -= nHeight; 
			posStartY = point.y;
			posStartX1 = point.x /2;
			posStartX2 = 360*15;

			//打印左右二行抬头	
			pPrintDC->TextOut(posStartX1,posStartY,str); 
			pPrintDC->TextOut(posStartX2,posStartY,str); 

			for (int i = 0; i < m_nNumOfCurInning; i++)
			{
				rstColor = g_RestColor[m_curRoundRstArray[i]];
				if(rstColor == RED)
					str1="红";
				else if(rstColor == BLACK)
					str1="黑";
				else	
					str1="绿";

				str.Format("%10d %10s", i+1,str1);

				//左右各打印一半
				
				if (i < m_nNumOfCurInning/2)
				{
					posStartY = point.y - (i+1)*nHeight;
					pPrintDC->TextOut(posStartX1, posStartY, str);
				}
				else
				{
					posStartY = point.y - (i+1-(m_nNumOfCurInning/2))*nHeight;
					pPrintDC->TextOut(posStartX2, posStartY, str);
				}				
			}
		}

		pPrintDC->EndPage(); //打印机停纸,一页结束 
		pPrintDC->EndDoc(); //结束一个打印作业
	}
	
	pPrintDC->SelectObject(pOldFont);
	pPrintDC->SelectObject(pOldPen);
	pPrintDC->Detach();
	delete pPrintDC;
	DeleteDC(hdcprint);
}
//---------------------------------------------------
