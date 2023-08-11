//-----------------------------------------
//	FileName:ACEClient.h
//	Introduce:The client class definition use ACE
//	Creator:Youfang Yao
//	Date:07-05-07
//-----------------------------------------
#ifndef	_ACE_CLIENT_H_
#define	_ACE_CLIENT_H_
//-----------------------------------------
/*	Net command define files*/
#include "net_command.h"
#include "income_msg.h"
#include "cmd_handler.h"

#include "session.h"
#include "test_req.h"
#include "training_req.h"

/*	ACE file including */
#include "ace/Task.h"
#include "ace/SOCK_Connector.h"
#include "ace/OS_NS_time.h"
//-----------------------------------------
#define	MAX_NAME_LEN				32
#define	MAX_STR_LEN					1024
//
#define	WM_BASE_MSG				WM_USER
#define	WM_PARENT_LOGIN_REMSG	WM_BASE_MSG + 1		//The message type definition for parent login response
#define	WM_KID_LOGIN_REMSG		WM_BASE_MSG + 2		//The message type definition for kid login response
#define	WM_GET_TEST_REMSG		WM_BASE_MSG + 3		//The message type definition for get pending tests response
//-----------------------------------------
typedef struct tagParentLoginReMsg
{
	bool	bLoginSuccess;
}ParentLoginReMsg, *LPPARENTLOGINREMSG;
//-----------------------------------------
//	Name:KidLoginReMsg
//	Introduce:Kid login response message data definition
//-----------------------------------------
typedef struct tagKidLoginReMsg
{
	bool	bLoginSuccess;
	char	szSessionID[MAX_NAME_LEN];
	bool	bHasPendingTest;
	long	lCurTestAblilityID;
}KidLoginReMsg, *LPLIDLOGINREMSG;
//-----------------------------------------
//	Name:GetTestReMsg
//	Introduce:Get pending tests response message data definition
//-----------------------------------------
typedef struct tagGetTestReMsg
{
	bool	bSuccess;
	long	lTestNum;
	char	szXML[MAX_STR_LEN];
}GetTestReMsg, *LPGETTESTREMSG;
//-----------------------------------------


//-----------------------------------------
//	Name:CACEClient
//	Introduce:The ACE client class definition
//-----------------------------------------
class CACEClient : public ACE_Task_Base
{
public:
	CACEClient(HWND hParentWnd);
	~CACEClient();
	//-------------------------------------
public:
	//Connect to the server
	DWORD	Connect(const char* lpszServer, const char* lpszPort);
	//Disconnect the ACE connecting
	DWORD	Disconnect();
	//Parent login to the server
	DWORD	ParentLogin(const char* lpszUserName, const char* lpszPSW);
	//Kid login to the server
	DWORD	KidLogin(const char* lpszUserName, const char* lpszPSW = NULL);
	//Get the current pending test ability id for the kid
	DWORD	GetPendingTest(const char* lpKidSessionID, DWORD dwAbilityID);
	//Send the start test message to server
	DWORD	StartTest(const char* lpKidSessionID, DWORD dwAbilityID);
	//Send the finish test message to server
	DWORD	FinishTest(const char* lpKidSessionID, DWORD dwCurAbilityID, DWORD dwNextAbilityID, DWORD dwStartTime, DWORD dwEndTime, DWORD dwItemCnt, DWORD dwRightOnes);
	//Send the login out message to server
	DWORD	KidLoginOut(const char* lpKidSessionID, DWORD dwTime);
	//-------------------------------------
private:
	//Parent login response handler
	DWORD	ParentLoginResponse(const ACE_SOCK_Stream& ACEPeer);
	//Kid login response handler
	DWORD	KidLoginResponse(const ACE_SOCK_Stream& ACEPeer);
	//Get the current pending test response handler
	DWORD	GetPendingTestResponse(const ACE_SOCK_Stream& ACEPeer);
	//-------------------------------------
private:
	HWND				m_hParentWnd;
	ACE_INET_Addr		m_ACEServerAddr;
	ACE_SOCK_Connector	m_ACEConnect;
	ACE_SOCK_Stream		m_ACEStream;
	BOOL				m_bConnected;
	//	
	char				m_szServer[24];
	char				m_szPort[16];
	char				m_szParentName[32];
	char				m_szParentPSW[32];
	char				m_szKidName[32];
	char				m_szKidPSW[32];
	//-------------------------------------
	KidLoginReMsg		m_stuKidLoginResMsg;
	GetTestReMsg		m_stuGetTestReMsg;
	//-------------------------------------
};
//-----------------------------------------
#endif	//_ACE_CLIENT_H_
//-----------------------------------------