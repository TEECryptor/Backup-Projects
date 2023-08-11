//-------------------------------------------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SUNMSGSENDER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SUNMSGSENDER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//-------------------------------------------------------------------------
#ifdef SUNMSGSENDER_EXPORTS
#define SUNMSGSENDER_API __declspec(dllexport)
#else
#define SUNMSGSENDER_API __declspec(dllimport)
#endif
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
/*
	Name:SMS_Init
	Introduct:Initialize this DLL, this function should be called before any 
			  other interfaces
	Return:Return 0 if successfully, otherwise return a Winsocket error.
	
*/
SUNMSGSENDER_API int SMS_Init(void);
//-------------------------------------------------------------------------
/*
	Name:SMS_ConnectServer
	Introduct:Try to connect to a server
	Return:Return 0 if successfully, otherwise return a Winsocket error.
	
*/
SUNMSGSENDER_API int SMS_ConnectServer(const char* lpszHost, int iPort);
//-------------------------------------------------------------------------
/*
	Name:SMS_SendMsg
	Introduct:Try to send a string message to the server
	Return:Return 0 if successfully, otherwise return a Winsocket error.
	
*/
SUNMSGSENDER_API int SMS_SendMsg(const char* lpszMsg, int iMsgLen);
//-------------------------------------------------------------------------
/*
	Name:SMS_Disconnect
	Introduct:Disconnect the connect from the server
	Return:Return 0 if successfully, otherwise return a Winsocket error.
	
*/
SUNMSGSENDER_API int SMS_Disconnect(void);
//-------------------------------------------------------------------------
/*
	Name:SMS_Exit
	Introduct:Exit this DLL, this function should be called when you use 
	          any interface anymore.
	Return:Return 0 if successfully, otherwise return a Winsocket error.
	
*/
SUNMSGSENDER_API int SMS_Exit(void);
//-------------------------------------------------------------------------
