//--------------------------------------------
//	File Name:InternalCommon.h
//	Introduce:Internal common definition for Http download
//	Date:2010-05-10
//--------------------------------------------
#ifndef	_INTERNAL_COMMON_H_
#define	_INTERNAL_COMMON_H_
//--------------------------------------------
#include <windows.h>
#include <Wininet.h>
#include <stdio.h>
#include <TCHAR.h>
//--------------------------------------------
#define	HTTP_MAX_BUFF_LEN			1024
#define	HTTP_TASKID_BASE			100000
#define	HTTP_STREAMTASKID_BASE		500000
#define	HTTP_DOWNLOADING_FLAG		_T("WMDF")
//#define	HTTP_SEGMENT_BEGIN			_T("WMSB")
//#define	HTTP_SEGMENT_END			_T("WMSE")
#define	GIF(x) {lRes=x; if (lRes < 0) return lRes;}
//--------------------------------------------


//--------------------------------------------
//	Name:eProcessThreadMsg
//	Introduce:Task working thread messages
//--------------------------------------------
enum eProcessThreadMsg
{
	PROC_MSG_START		= (WM_USER + 100),
	PROC_MSG_STOP		= (WM_USER + 101),
	PROC_MSG_RESTART	= (WM_USER + 102)
};
//--------------------------------------------
//	Name:TaskInfo
//	Introduce:Task download configure data
//--------------------------------------------
typedef	struct tagTaskInfo
{
	TCHAR		tcHostName[HTTP_MAX_BUFF_LEN]; 
	TCHAR		tcNetFile[HTTP_MAX_BUFF_LEN]; 
	TCHAR		tcTargetFile[MAX_PATH]; 
	DWORD		dwPort;
	TCHAR		tcUserName[128]; 
	TCHAR		tcPassword[128];
	DWORD		dwThreadCnt;
}TaskInfo, *LPTASKINFO;
//--------------------------------------------
//	Name:TempFileTailer
//	Introduce:Downloading temp file tailer defintion
//--------------------------------------------
typedef	struct tagTempFileTailer
{
	TCHAR		tcTailFlg[5];		// Our file flag, must be HTTP_DOWNLOADING_FLAG
	DWORD		dwNetFileSize;		// Net file size of this task, in bytes
	DWORD		dwFinishedSize;		// Finished size of last downloaded
	TaskInfo	stuTaskInfo;
}TempFileTailer, *LPTEMPFILETAILER;
//--------------------------------------------
//	Name:DataSegmentInfo
//	Introduce:Data segment information
//--------------------------------------------
typedef struct tagDataSegmentInfo
{
	DWORD		dwStartPos;			// Data segment start position
	DWORD		dwDataLength;		// Data segment lenght, in bytes
	DWORD		dwFinished;			// Finished lenght of this segment
}DataSegmentInfo, *LPDATASEGMENTINFO;
//--------------------------------------------
//	Name:ThreadInfo
//	Introduce:Thread download configure data
//--------------------------------------------
typedef	struct tagThreadInfo
{
	TCHAR		tcHost[HTTP_MAX_BUFF_LEN]; 
	TCHAR		tcNetFile[HTTP_MAX_BUFF_LEN]; 
	DWORD		dwFinishedLenght;	// Curent downloaded data size, in bytes
	DWORD		dwCurSegmentIndex;	// Current downloading segment index
	DWORD		dwTailInfoOffset;	// File offset of tail information for this thread
}ThreadInfo, *LPTHREADINFO;
//--------------------------------------------
#endif	//_INTERNAL_COMMON_H_
//--------------------------------------------