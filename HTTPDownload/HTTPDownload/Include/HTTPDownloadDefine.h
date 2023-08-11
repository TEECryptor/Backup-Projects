//--------------------------------------------
//	File Name:HTTPDownloadDefine.h
//	Introduce:Commom definition for Http download
//	Date:2010-05-10
//--------------------------------------------
#ifndef	_HTTPDOWNLOAD_DEFINE_H
#define _HTTPDOWNLOAD_DEFINE_H
//--------------------------------------------
#include <windows.h>
//--------------------------------------------
/*Downloading project file name extend string*/
#define	HTTP_DOWNLOADING_EXT		_T(".wmd!")

/*Http download callback function definiton*/
typedef long(*HttpDownload_CallbackFunc)(LONG lTaskID, LONG lThreadID, DWORD dwEvent, DWORD dwParam1, DWORD dwParam2, LPVOID lpUser);
//--------------------------------------------
/*The error code definition*/
#define	HTTP_E_NOERROR				0x0			//No errors
//
#define HTTP_E_NOTCALL_INITIALIZE	0x80000000
#define	HTTP_E_MAXTASKCNT_ERROR		0x80000001
#define	HTTP_E_MAXTHREADCNT_ERROR	0x80000002
#define	HTTP_E_URL_ERROR			0x80000003
#define	HTTP_E_TARGET_ERROR			0x80000004
#define	HTTP_E_HOSTNAME_ERROR		0x80000005
#define	HTTP_E_NETFILE_ERROR		0x80000006
#define	HTTP_E_THREADCNT_ERROR		0x80000007
#define	HTTP_E_TASKID_NOTFOUND		0x80000008
#define	HTTP_E_CANTSET_FORRUNNING	0x80000009
//
#define	HTTP_E_OPENNET_ERROR		0x80000050
#define	HTTP_E_CLOSENET_ERROR		0x80000051
#define	HTTP_E_CONNECTNET_ERROR		0x80000052
#define	HTTP_E_DISCONNECTNET_ERROR	0x80000053
#define	HTTP_E_OPENNETFILE_ERROR	0x80000054
#define	HTTP_E_CLOSENETFILE_ERROR	0x80000055
#define HTTP_E_SENDREQUEST_ERROR	0x80000056
#define	HTTP_E_NETFILESIZE_ERROR	0x80000057
#define	HTTP_E_DISCFREESPACE_ERROR	0x80000058
#define	HTTP_E_DISCWRITE_ERROR		0x80000059
#define	HTTP_E_OPENTARGET_ERROR		0x8000005A
#define	HTTP_E_CLOSETARGET_ERROR	0x8000005B
#define	HTTP_E_NOMULTI_THREAD		0x8000005C
#define	HTTP_E_DOWNLOAD_FAILED		0x8000005D
#define	HTTP_E_FILE_ISDOWNLOADED	0x8000005E
//
#define	HTTP_E_CREATETHREAD_ERROR	0x80000100
#define	HTTP_E_RUNNINGCNT_MEETMAX	0x80000101
#define	HTTP_E_INVALIDARG			0x80000102
#define	HTTP_E_CREATTHREAD_FAIL		0x80000103
#define	HTTP_E_SETFILEPOINTER_ERROR	0x80000104
//
#define	HTTP_E_THREADID_ISEXIST		0x80000200
#define	HTTP_E_THREADID_NOTFOUND	0x80000201
#define	HTTP_E_THREADRANGE_ERROR	0x80000203
#define	HTTP_E_CANTSET_THREADCNT	0x80000204
//--------------------------------------------
//	Name:eHTTPTaskStatus
//	Introcue:The definition of task status
//--------------------------------------------
enum eHTTPTaskStatus
{
	eHttpTaskStatus_Ready			= 0,		//Created, but not started
	eHttpTaskStatus_Waitting		= 1,		//Started, but the running task count meet max. It'll be auto-run when a running task complete
	eHttpTaskStatus_Running			= 2,		//In running and downloading
	eHttpTaskStatus_Stopped			= 3,		//Has not completed, but stopped by user
	eHttpTaskStatus_Completed		= 4,		//All data has downloaded successfully
	eHttpTaskStatus_Cnt
};
//--------------------------------------------
//	Name:eHTTPCBEvent
//	Introcue:The definition of HTTP callback funtion event code
//--------------------------------------------
enum eHTTPCBEvent
{
	eHTTPCBEvent_StatusChanged		= 0,		//Task status has changed. dwParam1 is the new status, see eHTTPTaskStatus
	eHTTPCBEvent_ProgressChanged	= 1,		//Task progress infromation updated. dwParam1 is finished size (in bytes) and dwParam2 is current speed (in bytes/sec)
	eHTTPCBEvent_Error				= 2,		//Occurred an error in progress. dwParam1 is an error code
	eHTTPCBEvent_Cnt
};
//--------------------------------------------
//	Name:HttpDownloadCfg
//	Introcue:The definition of http download configure data
//--------------------------------------------
typedef struct tagHttpDownloadCfg
{
	DWORD	dwMaxTaskCnt;						//The max count of running task
	DWORD	dwMaxThreadCnt;						//The max thread count of a task, thread count can be deferent for each task
	LPVOID	lpHttpCallbackFunc;					//Callback funtion, see HttpDownload_CallbackFunc. It can be NULL, and user can call GetTaskStatus() and GetTaskPercent() to check task current status in this case.
	LPVOID	lpUser;								//User data pointer, it be returned in callback function.
	BOOL	bForceUseSingleThread;				//Force use single thread for a task if TRUE
}HttpDownloadCfg, *LPHTTPDOWNLOADCFG;
//--------------------------------------------
#endif	//_HTTPDOWNLOAD_DEFINE_H
//--------------------------------------------