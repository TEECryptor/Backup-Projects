//----------------------------------------------------------------------------
//	File Name:MLVSTiPlayer.h
//	Introduce:VSTi Player interface defintion
//	Creator:Youfang Yao
//	Date:2010-08-14
//----------------------------------------------------------------------------
#ifndef	_ML_VSTIPLAYER_H_
#define	_ML_VSTIPLAYER_H_
//----------------------------------------------------------------------------
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MLVSTIPLAYER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MLVSTIPLAYER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
//----------------------------------------------------------------------------
#ifdef MLVSTIPLAYER_EXPORTS
#define MLVSTIPLAYER_API __declspec(dllexport)
#else
#define MLVSTIPLAYER_API __declspec(dllimport)
#endif
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
#define	VSTI_MAXSTR_LEN		256
//----------------------------------------------------------------------------
//	Name:eMidiPlayState
//	Introduce:Midi file playing status definition
//----------------------------------------------------------------------------
enum eMidiPlayState
{
	eMidiPlayState_Ready	= 0,
	eMidiPlayState_Playing	= 1,
	eMidiPlayState_Paused	= 2,
	eMidiPlayState_Stopped	= 3,
	eMidiPlayState_Cnt
};
//----------------------------------------------------------------------------
//	Name:VSTiEffectInfo
//	Introduce:VSTi effect information data definition
//----------------------------------------------------------------------------
typedef struct tagVSTiEffectInfo
{
	char	csFileName[MAX_PATH];
	char	csEffectName[VSTI_MAXSTR_LEN];
}VSTiEffectInfo, *LPVSTIEFFECTINFO;
//----------------------------------------------------------------------------
//	Name:MidiFileInfo
//	Introduce:Midi file information data definition
//----------------------------------------------------------------------------
typedef struct tagMidiFileInfo
{
	DWORD dwTracksNum;
	DWORD dwTicksNum;
	DWORD dwTempo;
	DWORD dwDuration;
	char  csFileName[MAX_PATH];
}MidiFileInfo, *LPMIDIFILEINFO;
//----------------------------------------------------------------------------
//	Name:MidiPlayingInfo
//	Introduce:Midi file playing status information data definition
//----------------------------------------------------------------------------
typedef struct tagMidiPlayingInfo
{
	eMidiPlayState	eCurState;
	DWORD			dwCurTime;
}MidiPlayingInfo, *LPMIDIPLAYINGINFO;
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//	Name:IMLVSTiEffect
//	Introduce:VSTi Effect interface definition
//----------------------------------------------------------------------------
class MLVSTIPLAYER_API IMLVSTiEffect
{
public:
	IMLVSTiEffect(void){};
	virtual ~IMLVSTiEffect(void){};
	//------------------------------------------------------------------------
	virtual void GetEffectInfo(VSTiEffectInfo &Info) = 0;
	virtual void GetEditWindowRect(RECT &rcEdit) = 0;
	virtual void ShowEditWindow(HWND hParentWnd) = 0;
	virtual void CloseEditWindow() = 0;
	//------------------------------------------------------------------------
};
//----------------------------------------------------------------------------
//	Name:IMLVSTiMidiFile
//	Introduce:Midi file interface definition
//----------------------------------------------------------------------------
class MLVSTIPLAYER_API IMLVSTiMidiFile
{
public:
	IMLVSTiMidiFile(void){};
	virtual ~IMLVSTiMidiFile(void){};
public:
	//------------------------------------------------------------------------
	virtual void GetMidiFileInfo(MidiFileInfo &Info) = 0;
	//------------------------------------------------------------------------
};
//----------------------------------------------------------------------------
//	Name:IMLVSTiPlayer
//	Introduce:VSTi Midi player interface definition
//----------------------------------------------------------------------------
class MLVSTIPLAYER_API IMLVSTiPlayer 
{
public:
	IMLVSTiPlayer(void){};
	virtual ~IMLVSTiPlayer(void){};
	//------------------------------------------------------------------------	
public:
	virtual BOOL Initialize() = 0;
	virtual DWORD GetOutDeviceCnt() = 0;
	virtual char* GetDeviceName(int iIndex) = 0;
	virtual void SetCurOutDevice(int iIndex) = 0;
	virtual IMLVSTiEffect* LoadVSTiEffect(const char* lpszEffect) = 0;
	virtual void UnLoadVSTiEffect(int iIndex) = 0;
	virtual DWORD GetVSTiEffectCnt() = 0;
	virtual IMLVSTiEffect* GetVSTiEffectByIndex(int iIndex) = 0;
	virtual int	GetVSTiEffectIndex(IMLVSTiEffect* pVSTiEffect) = 0;
	virtual IMLVSTiMidiFile*  LoadMidiFile(const char*  lpszMidi) = 0;
	virtual void UnLoadMidiFile(int iIndex) = 0;
	virtual DWORD GetMidiFileCnt() = 0;
	virtual IMLVSTiMidiFile* GetMidiFileByIndex(int iIndex) = 0;
	virtual int	GetMidiFileIndex(IMLVSTiMidiFile* pMidiFile) = 0;
	virtual BOOL PlayMidiFile(int iMidiIndex, int iEffectIndex) = 0;
	virtual void StopMidiFile(int iMidiIndex) = 0;
	virtual BOOL GetMidiPlayingInfo(int iIndex, MidiPlayingInfo &Info) = 0;
	//------------------------------------------------------------------------
};
//----------------------------------------------------------------------------

MLVSTIPLAYER_API IMLVSTiPlayer* MLVSTiPlayer_Create();
MLVSTIPLAYER_API void MLVSTiPlayer_Destroy(IMLVSTiPlayer* pPlayer);
//----------------------------------------------------------------------------
#endif	//_ML_VSTIPLAYER_H_
//----------------------------------------------------------------------------