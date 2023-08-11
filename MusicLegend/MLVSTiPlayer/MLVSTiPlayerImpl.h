//----------------------------------------------------------------------------
//	File Name:MLVSTiPlayerImpl.h
//	Introduce:VSTi Player implement class defintion
//	Creator:Youfang Yao
//	Date:2010-08-16
//----------------------------------------------------------------------------
#ifndef	_ML_VSTIPLAYERIMPL_H_
#define	_ML_VSTIPLAYERIMPL_H_
//----------------------------------------------------------------------------
#include "MLVSTiPlayer.h"
#include "MLVSTiEffectImpl.h"
#include "MLVSTiMidiFileImpl.h"
#include "VSTiOutDeviceManager.h"
#include <vector>
//----------------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------------
typedef vector<CMLVSTiEffectImpl*> PtrArraryEffect;
typedef vector<CMLVSTiMidiFileImpl*> PtrArraryMidiFile;
//----------------------------------------------------------------------------
#define	PROC_MSG_START			WM_USER + 100
#define	PROC_MSG_PLAY			WM_USER + 101
#define	PROC_MSG_PAUSE			WM_USER + 102
#define	PROC_MSG_STOP			WM_USER + 103
#define	PROC_MSG_END			WM_USER + 104
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	Name:CMLVSTiPlayerImpl
//	Introduce:The implement of IMLVSTiPlayer interface
//----------------------------------------------------------------------------
class CMLVSTiPlayerImpl : public IMLVSTiPlayer
{
public:
	CMLVSTiPlayerImpl(void);
	~CMLVSTiPlayerImpl(void);
	//------------------------------------------------------------------------
public:
	BOOL	Initialize();
	DWORD	GetOutDeviceCnt();
	char*	GetDeviceName(int iIndex);
	void	SetCurOutDevice(int iIndex);
	//
	IMLVSTiEffect* LoadVSTiEffect(const char* lpszEffect);
	void	UnLoadVSTiEffect(int iIndex);
	DWORD	GetVSTiEffectCnt();
	IMLVSTiEffect* GetVSTiEffectByIndex(int iIndex);
	int		GetVSTiEffectIndex(IMLVSTiEffect* pVSTiEffect);
	//
	IMLVSTiMidiFile* LoadMidiFile(const char* lpszMidi);
	void	UnLoadMidiFile(int iIndex);
	DWORD	GetMidiFileCnt();
	IMLVSTiMidiFile* GetMidiFileByIndex(int iIndex);
	int		GetMidiFileIndex(IMLVSTiMidiFile* pMidiFile);
	//
	BOOL	PlayMidiFile(int iMidiIndex, int iEffectIndex);
	void	StopMidiFile(int iMidiIndex);
	BOOL	GetMidiPlayingInfo(int iIndex, MidiPlayingInfo &Info);
	//------------------------------------------------------------------------
public:
	long	MidiMasterCallback(AEffect *pEffect, long lCode, long lIndex, long lValue, void *ptr, float fOpt);
	bool	OnCanDo(const char *ptr);
	//------------------------------------------------------------------------
public:
	static DWORD WINAPI WorkingThread(LPVOID lpParam);
	//------------------------------------------------------------------------
private:
	CMLVSTiEffectImpl*	GetEffectPtrByHandle(AEffect *pEffect);
	//------------------------------------------------------------------------
private:
	PtrArraryEffect			m_arVSTiEffect;
	PtrArraryMidiFile		m_arVSTiMidiFile;
	CVSTiOutDeviceManager*	m_pOutDevManager;
	//
	DWORD					m_dwWorkingThreadID;
	HANDLE					m_hWorkingThread;
	HANDLE					m_hProcessEvent;
	//------------------------------------------------------------------------
};
//----------------------------------------------------------------------------
#endif	//_ML_VSTIPLAYERIMPL_H_
//----------------------------------------------------------------------------