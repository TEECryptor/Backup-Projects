//-------------------------------------------------
//	Name:CommonDef.h
//	Introduce:The application common definition file
//	Creator:Youfang Yao
//	Date:11-25-06
//	Version:1.0V
//-------------------------------------------------
#ifndef _COMMONDEF_H_
#define _COMMONDEF_H_
//-------------------------------------------------
#define		MAX_STRING_LEN		32
//-------------------------------------------------
//	Name:ResultFlg
//	Introduce:The Game result flag definition
//-------------------------------------------------
enum ResultFlg
{
	RF_RED		= 0,
	RF_BLACK	= 1,
	RF_GREEN	= 2,
	RF_COUNT
};
//-------------------------------------------------
//	Name:SocketMsgType
//	Introduce:The Socket message data type definition
//-------------------------------------------------
enum SocketMsgType
{
	SMT_NONE				= 0,	//A void message type
	SMT_STC_SERVERSETINFO	= 1,	//Send the current server setting information to all clients
	SMT_STC_GETCLIENTINFO	= 2,	//When a client connected, server send this message to the client to get its information
	SMT_CTS_CLIENTINFO		= 3,	//Client send itself information to server	
	SMT_CTS_ASKFORACTCMD	= 4,	//Client send this cmd to ask the server to active it
	SMT_STC_ACTIVECMD		= 5,	//When server going to start a client, send this message to the client
	SMT_CTS_HASACTIVED		= 6,	//After client has been actived,send this message to server
	SMT_STC_USERVALUEINFO	= 7,	//Server use this message to send the user value information to client
	SMT_STC_STARTBET		= 8,	//Server use this message to tall clients to start bet operation
	SMT_CTS_SUBMITBET		= 9,	//When user submit the bet operation,client send this message to server
	SMT_STC_TOTALBETINFO	= 10,	//Server use this message to send current bet information to all clients
	SMT_STC_ENDBET			= 11,	//Server use this message to tall clients to end bet operation
	SMT_STC_RENDERCMD		= 12,	//Server send current render information to all client with this message
	SMT_CTS_RENDEREND		= 13,	//When client render end, send this message to server
	SMT_STC_GAMERESULT		= 14,	//When a game is end, server send the game result to all clients with this message
	SMT_STC_SHOWRESULT		= 15,	//When table turning end, server send this message to clients for showwing game result
	SMT_STC_GAMEEND			= 16,	//When an inning game is end, send this message to clients
	SMT_STC_WINVALUE		= 17,	//The client win value message type
	SMT_STC_GAMEOVER		= 18,	//When game over, server send this message to all clients
	SMT_STC_CLOSE			= 19,	//To close all client
	//
	SMT_COUNT
};
//-------------------------------------------------



//-------------------------------------------------
//	Name:ServerSetInfo
//	Introduce:The SMT_STC_SERVERSETINFO message data struct definition
//-------------------------------------------------
typedef struct tagServerSetInfo
{
	UINT	uMsgType;					//Must be SMT_STC_SERVERSETINFO
	//
	int		iMinBet;					//Min bet value setting
	int		iMaxBet;					//Max bet value setting
	//
	int		iMaxRedBet;					//Max bet red value
	int		iMaxBlackBet;				//Max bet black value
	int		iMaxGreenBet;				//Max bet green value	
}ServerSetInfo, *LPSERVERSETINFO;
//-------------------------------------------------
//	Name:GetClientInfo
//	Introduce:The SMT_STC_GETCLIENTINFO message data struct definition
//-------------------------------------------------
typedef struct tagGetClientInfo
{
	UINT	uMsgType;					//Must be SMT_STC_GETCLIENTINFO
}GetClientInfo, *LPGETCLIENTINFO;
//-------------------------------------------------
//	Name:ReturnClientInfo
//	Introduce:The SMT_CTS_CLIENTINFO message data struct definition
//-------------------------------------------------
typedef struct tagClientInfo
{
	UINT	uMsgType;					//Must be SMT_CTS_CLIENTINFO
	//
	UINT	uClientID;					//The client ID
	TCHAR	szName[MAX_STRING_LEN];		//The client name string
}ClientInfo, *LPCLIENTINFO;
//-------------------------------------------------
//	Name:AskForActiveCmd
//	Introduce:The SMT_CTS_ASKFORACTIVECMD message data struct definition
//-------------------------------------------------
typedef struct tagAskForActiveInfo
{
	UINT	uMsgType;					//Must be SMT_STC_ACTIVECMD
	//
	UINT	uClientID;					//The client ID
}AskForActiveCmd, *LPASKFORACTIVECMD;
//-------------------------------------------------
//	Name:ActiveInfo
//	Introduce:The SMT_STC_ACTIVECMD message data struct definition
//-------------------------------------------------
typedef struct tagActiveInfo
{
	UINT	uMsgType;					//Must be SMT_STC_ACTIVECMD
	//
	UINT	uClientID;					//The client ID
	BOOL	bCanActived;				//If fValue <= 0, then this client can't be actived
	float	fValue;						//The user value for this client
}ActiveInfo, *LPACTIVEINFO;
//-------------------------------------------------
//	Name:HasActivedCmd
//	Introduce:The SMT_CTS_HASACTIVED message data struct definition
//-------------------------------------------------
typedef struct tagHasActivedCmd
{
	UINT	uMsgType;					//Must be SMT_CTS_HASACTIVED
	//
	UINT	uClientID;					//The client ID
}HasActivedCmd, *LPHASACTIVEDCMD;
//-------------------------------------------------
//	Name:UserValueInfo
//	Introduce:The SMT_STC_USERVALUEINFO message data struct definition
//-------------------------------------------------
typedef struct tagUserValueInfo
{
	UINT	uMsgType;					//Must be SMT_STC_USERVALUEINFO
	//
	UINT	uClientID;					//The client ID
	float	fValue;						//Current value for this client
}UserValueInfo, *LPUSERVALUEINFO;
//-------------------------------------------------
//	Name:StartBetCmd
//	Introduce:The SMT_STC_STARTBET message data struct definition
//-------------------------------------------------
typedef struct tagStartBetCmd
{
	UINT	uMsgType;					//Must be SMT_STC_STARTBET
	//
	int		iBetTimeLen;				//The betting time length
}StartBetCmd, *LPSTARTBETCMD;
//-------------------------------------------------
//	Name:SubmitBetInfo
//	Introduce:The SMT_CTS_SUBMITBET message data struct definition
//-------------------------------------------------
typedef struct tagSubmitBetInfo
{
	UINT	uMsgType;					//Must be SMT_CTS_SUBMITBET
	//
	UINT	uClientID;					//The client ID
	float	fBetRedVal;					//The value for betting red
	float	fBetBlackVal;				//The value for betting black
	float	fBetGreenVal;				//The value for betting green
}SubmitBetInfo, *LPSUBMITBETINFO;
//-------------------------------------------------
//	Name:TotalBetInfo
//	Introduce:The SMT_STC_TOTALBETINFO message data struct definition
//-------------------------------------------------
typedef struct tagTotalBetInfo
{
	UINT	uMsgType;					//Must be SMT_STC_TOTALBETINFO
	//
	float	fTotalBetRedVal;			//The total value for betting red
	float	fTotalBetBlackVal;			//The total value for betting black
	float	fTotalBetGreenVal;			//The total value for betting green
}TotalBetInfo, *LPTOTALBETINFO;
//-------------------------------------------------
//	Name:EndBetCmd
//	Introduce:The SMT_STC_ENDBET message data struct definition
//-------------------------------------------------
typedef struct tagEndBetCmd
{
	UINT	uMsgType;					//Must be SMT_STC_ENDBET
}EndBetCmd, *LPENDBETCMD;
//-------------------------------------------------
//	Name:RenderInfo
//	Introduce:The SMT_STC_RENDERCMD message data struct definition
//-------------------------------------------------
typedef struct tagRenderInfo
{
	UINT	uMsgType;					//Must be SMT_STC_RENDERCMD
	//
	float	fTableAngle;				//Current angle for table, in degree
	int		fBallImage;					//Current image index for ball
	float	fBallPosX;					//Current X position for ball
	float	fBallPosY;					//Current Y position for ball
}RenderInfo, *LPRENDERINFO;
//-------------------------------------------------
//	Name:RenderEndCmd
//	Introduce:The SMT_CTS_RENDEREND message data struct definition
//-------------------------------------------------
typedef struct tagRenderEndCmd
{
	UINT	uMsgType;					//Must be SMT_CTS_RENDEREND
	//
	UINT	uClientID;					//The client ID
}RenderEndCmd, *LPRENDERENDCMD;
//-------------------------------------------------
//	Name:GameResultInfo
//	Introduce:The SMT_STC_GAMERESULT message data struct definition
//-------------------------------------------------
typedef struct tagGameResultInfo
{
	UINT	uMsgType;					//Must be SMT_STC_GAMERESULT
	//
	int		iGameNo;					//The game No number
	UINT	uResult;					//The game result
	UINT	uFlg;						//The result flag,RF_RED/RF_BLACK/RF_GREEN
}GameResultInfo, *LPGAMERESULTINFO;
//-------------------------------------------------
//	Name:ShowResultCmd
//	Introduce:The SMT_STC_SHOWRESULT message data struct definition
//-------------------------------------------------
typedef struct tagShowResultCmd
{
	UINT		uMsgType;				//Must be SMT_STC_SHOWRESULT
}ShowResultCmd, *LPSHOWRESULTCMD;
//-------------------------------------------------
//	Name:GameEndCmd
//	Introduce:The SMT_STC_GAMEEND message data struct definition
//-------------------------------------------------
typedef struct tagGameEndCmd
{
	UINT		uMsgType;				//Must be SMT_STC_GAMEEND
	//
	float		fAnlgeShowRes;			//The angle to flag current game result on mini table
}GameEndCmd, *LPGAMEENDCMD;
//-------------------------------------------------
//	Name:GameOverCmd
//	Introduce:The SMT_STC_WINVALUE message data struct definition
//-------------------------------------------------
typedef struct tagWinValueInfo
{
	UINT	uMsgType;					//Must be SMT_STC_WINVALUE
	//
	UINT	uClientID;					//The specific client ID
	int		iWinValue;					//The client win value
}WinValueInfo, *LPWINVALUEINFO;
//-------------------------------------------------
//	Name:GameOverCmd
//	Introduce:The SMT_STC_GAMEOVER message data struct definition
//-------------------------------------------------
typedef struct tagGameOverCmd
{
	UINT	uMsgType;					//Must be SMT_STC_GAMEOVER
}GameOverCmd, *LPGAMEOVERCMD;
//-------------------------------------------------
//	Name:CloseClientCmd
//	Introduce:The SMT_STC_CLOSE message data struct definition
//-------------------------------------------------
typedef struct tagCloseClientCmd
{
	UINT	uMsgType;					//Must be SMT_STC_CLOSE
}CloseClientCmd, *LPCLOSECLIENTCMD;
//-------------------------------------------------




//-------------------------------------------------
//	Name:StatusInfo
//	Introduce:The SMT_CTS_STATUSINFO message data struct definition
//			  Only for testing!!!!
//-------------------------------------------------
typedef struct tagStatusInfo
{
	UINT	uMsgType;					//Must be SMT_CTS_STATUSINFO
	//
	UINT	uClientID;
	TCHAR	szStatus[MAX_STRING_LEN];
}StatusInfo, *LPSTATUSINFO;
//-------------------------------------------------



//-------------------------------------------------
#endif _COMMONDEF_H_
//-------------------------------------------------