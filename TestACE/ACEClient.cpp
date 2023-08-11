//-----------------------------------------
//	FileName:ACEClient.cpp
//	Introduce:The cpp file for class ACEClient
//	Creator:Youfang Yao
//	Date:07-05-07
//-----------------------------------------
#include "stdafx.h"
#include "ACEClient.h"
//-----------------------------------------
//	Name:CACEClient
//	Introduce:Constructor
//-----------------------------------------
CACEClient::CACEClient(HWND hParentWnd)
 : m_bConnected(FALSE),
   m_hParentWnd(hParentWnd)
{
	strcpy(m_szServer, "\0");
	strcpy(m_szPort, "\0");
	strcpy(m_szParentName, "\0");
	strcpy(m_szParentPSW, "\0");	
	strcpy(m_szKidName, "\0");
	strcpy(m_szKidPSW, "\0");

	memset(&m_stuKidLoginResMsg, 0, sizeof(KidLoginReMsg));
	memset(&m_stuGetTestReMsg, 0, sizeof(GetTestReMsg));	
}
//-----------------------------------------
//	Name:~CACEClient
//	Introduce:Distructor
//-----------------------------------------
CACEClient::~CACEClient()
{
	m_ACEStream.close();
}
//-----------------------------------------
//	Name:Connect
//	Introduce:Connect to the server
//-----------------------------------------
DWORD CACEClient::Connect(const char* lpszServer, const char* lpszPort)
{		
	if(NULL == lpszServer || NULL == lpszPort)
		return -1;

	strcpy(m_szServer, lpszServer);
	strcpy(m_szPort, lpszPort);

	int iRes = activate();

	m_ACEServerAddr.set(m_szPort, m_szServer);

	try
	{
		const ACE_TCHAR *rendezvous = ACE_TEXT ("58.37.195.192:10010");
		ACE_INET_Addr add(rendezvous);
		if(m_ACEConnect.connect(m_ACEStream, /*m_ACEServerAddr*/add) < 0)
		{
			ACE_ERROR ((LM_ERROR,
				ACE_TEXT ("(%t) %p\n"),
				ACE_TEXT ("connect")));
			return -1;
		} 
	}
	catch(...)
	{
		m_bConnected = FALSE;
		return -1;
	}

	m_bConnected = TRUE;

	return 0;
}
//-----------------------------------------
//	Name:Disconnect
//	Introduce:Disconnect the ACE connecting
//-----------------------------------------
DWORD CACEClient::Disconnect()
{
	m_ACEStream.close();
	return 0;
}
//-----------------------------------------
//	Name:Connect
//	Introduce:Parent login to the server
//-----------------------------------------
DWORD CACEClient::ParentLogin(const char* lpszUserName, const char* lpszPSW)
{	
	if(NULL == lpszUserName)
		return -1;
/*
	strcpy(m_szParentName, lpszUserName);
	strcpy(m_szParentPSW, lpszPSW);

	//Check server is connected or not
	if(!m_bConnected)
	{
		if(Connect(m_szServer, m_szPort) != 0)
			return -1;
	}

	//Send the parent login command
	Login_Command cmdLogin(ROLE_PARENT, m_szParentName, m_szParentPSW);
	cmdLogin.send(m_ACEStream);
	
	ParentLoginResponse(m_ACEStream);
*/
	return 0;
}
//-----------------------------------------
//	Name:Connect
//	Introduce:Kid login to the server
//-----------------------------------------
DWORD CACEClient::KidLogin(const char* lpszUserName, const char* lpszPSW)
{
	if(NULL == lpszUserName)
		return -1;

	strcpy(m_szKidName, lpszUserName);
	strcpy(m_szKidPSW, lpszPSW);

	//Check server is connected or not
	if(!m_bConnected)
	{
		if(Connect(m_szServer, m_szPort) != 0)
			return -1;
	}

	//Send the kin login command
	Login_Req cmdLogin(ROLE_KID, m_szKidName, m_szKidPSW);	
	cmdLogin.send(m_ACEStream);

	int current_test_ability_id = KidLoginResponse(m_ACEStream);
	
	return 0;
}
//-----------------------------------------
//	Name:GetPendingTest
//	Introduce:Get the current pending test ability id for the kid
//-----------------------------------------
DWORD CACEClient::GetPendingTest(const char* lpKidSessionID, DWORD dwAbilityID)
{
	if(NULL == lpKidSessionID || strlen(lpKidSessionID) == 0)
		return -1;

	//Send the command to server
	Get_Pending_Test_Req cmdGetPendingTest(lpKidSessionID);
	cmdGetPendingTest.send(m_ACEStream);	

	//Call the command response handler
	GetPendingTestResponse(m_ACEStream);

	return  0;
}
//-----------------------------------------
//	Name:StartTest
//	Introduce:Send the start test message to server
//-----------------------------------------
DWORD CACEClient::StartTest(const char* lpKidSessionID, DWORD dwAbilityID)
{
	if(NULL == lpKidSessionID || strlen(lpKidSessionID) == 0)
		return -1;

	//Tell server, test have been started
	Start_Test_Req cmdTestStart(lpKidSessionID, dwAbilityID);
	cmdTestStart.send(m_ACEStream);	
	
//	cmd_response(stream);	

	return  0;
}
//-----------------------------------------
//	Name:StartTest
//	Introduce:Send the finish test message to server
//-----------------------------------------
DWORD CACEClient::FinishTest(const char* lpKidSessionID, DWORD dwCurAbilityID, DWORD dwNextAbilityID,
	DWORD dwStartTime, DWORD dwEndTime, DWORD dwItemCnt, DWORD dwRightOnes)
{
	if(NULL == lpKidSessionID || strlen(lpKidSessionID) == 0)
		return -1;

	Finish_Test_Req cmdTestFinished(lpKidSessionID, dwCurAbilityID, dwNextAbilityID, 
						dwStartTime, dwEndTime, dwItemCnt, dwRightOnes);

	cmdTestFinished.send(m_ACEStream);		
//	cmd_response(stream);

	return  0;
}
//-----------------------------------------
//	Name:KidLoginOut
//	Introduce:Send the login out message to server
//-----------------------------------------
DWORD CACEClient::KidLoginOut(const char* lpKidSessionID, DWORD dwTime)
{	
	if(NULL == lpKidSessionID || strlen(lpKidSessionID) == 0)
		return -1;

	Logout_Req cmdLoginOut(lpKidSessionID, dwTime);
	cmdLoginOut.send(m_ACEStream);		

//	cmd_response(stream);
	
	return  0;
}
//-----------------------------------------


//-----------------------------------------
//	Name:ParentLoginResponse
//	Introduce:Parent login response handler
//-----------------------------------------
DWORD CACEClient::ParentLoginResponse(const ACE_SOCK_Stream& ACEPeer)
{/*
	Net_Income_Cmd income_cmd(ACEPeer);
	income_cmd.get_header();

	ACE_InputCDR& payload_cdr = *income_cmd.payload_cdr_p;

	ACE_CDR::Boolean login_success;
	ACE_CString session_id;
	ACE_CDR::ULong activies_num;
	Act_List acts;

	payload_cdr >> ACE_InputCDR::to_boolean (login_success);
	// Extract each field from input CDR stream into <log_record>.
	if (login_success && (payload_cdr >> session_id) 
		&& (payload_cdr >> activies_num) ) {
		
		if(activies_num)	{
			
			for(int i=0; i<activies_num; i++)	{

				Activity* act = new Activity;
				if(NULL == act)	{
					goto out;
				}
				//payload_cdr >> *act;
				
				acts.push_back(act);
			}
		}
	}

out:
	Act_List::iterator act_iter;
	
	for(act_iter = acts.begin(); act_iter != acts.end(); ++act_iter)	{
		delete *act_iter;
		*act_iter = NULL;
	}
	acts.remove(NULL);
*/
	return 0;
}
//-----------------------------------------
//	Name:KidLoginResponse
//	Introduce:Kid login response handler
//-----------------------------------------
DWORD CACEClient::KidLoginResponse(const ACE_SOCK_Stream& ACEPeer)
{	
	Net_Income_Cmd income_cmd(ACEPeer);
	income_cmd.get_header();

	ACE_InputCDR& payload_cdr = *income_cmd.payload_cdr_p;

	ACE_CDR::Boolean login_success;
	ACE_CString session_id;
	ACE_CDR::Long pending_test, current_test_ability_id;

	login_success = 1;
//	payload_cdr >> ACE_InputCDR::to_boolean (login_success);
	payload_cdr >> session_id;
	payload_cdr >> pending_test;
	payload_cdr >> current_test_ability_id;
	
	const char *kid_sid_ = session_id.c_str();

	//Send the kid login response message to main window
	m_stuKidLoginResMsg.bLoginSuccess = login_success == 0 ? false : true;
	strcpy(m_stuKidLoginResMsg.szSessionID, session_id.c_str());
	m_stuKidLoginResMsg.bHasPendingTest = pending_test == 0 ? false : true;
	m_stuKidLoginResMsg.lCurTestAblilityID = current_test_ability_id;

	PostMessage(m_hParentWnd, WM_KID_LOGIN_REMSG, (WPARAM)&m_stuKidLoginResMsg, 0);

	return 0;
}
//-----------------------------------------
//	Name:GetPendingTestResponse
//	Introduce:Get the current pending test response handler
//-----------------------------------------
DWORD CACEClient::GetPendingTestResponse(const ACE_SOCK_Stream& ACEPeer)
{
	Net_Income_Cmd income_cmd(ACEPeer);
	income_cmd.get_header();

	ACE_InputCDR& payload_cdr = *income_cmd.payload_cdr_p;

	ACE_CDR::Long	status;
	ACE_CDR::Long	error_code;
	ACE_CDR::Long	pending_test_num;

	payload_cdr >> status;
	if(0 != status)
	{
		m_stuGetTestReMsg.bSuccess = false;
		payload_cdr >> error_code;
	}
	else
	{
		m_stuGetTestReMsg.bSuccess = true;
		//
		payload_cdr >> pending_test_num;
		m_stuGetTestReMsg.lTestNum = pending_test_num;

		//The xml string content
		ACE_CString xml;
		payload_cdr >> xml;
		strcpy(m_stuGetTestReMsg.szXML, xml.c_str());
	}

	PostMessage(m_hParentWnd, WM_GET_TEST_REMSG, (WPARAM)&m_stuGetTestReMsg, 0);

	return  0;
}
//-----------------------------------------