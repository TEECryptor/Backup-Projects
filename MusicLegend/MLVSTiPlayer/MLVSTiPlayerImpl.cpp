//----------------------------------------------------------------------------
//	File Name:MLVSTiPlayerImpl.cpp
//	Introduce:Class CMLVSTiPlayerImpl implement file
//	Creator:Youfang Yao
//	Date:2010-08-16
//----------------------------------------------------------------------------
#include "StdAfx.h"
#include ".\mlvstiplayerimpl.h"
//----------------------------------------------------------------------------
CMLVSTiPlayerImpl*	g_pVSTiPlayer = NULL;
//----------------------------------------------------------------------------
//	Name:WaitFor
//	Introdcue:Waiting a event with message loop
//----------------------------------------------------------------------------
BOOL WaitFor(HANDLE hEvent)
{
	DWORD nRet = WAIT_TIMEOUT;

	Sleep(100);

	while(WAIT_OBJECT_0 != nRet)
	{
		nRet = WaitForSingleObject(hEvent, 100);

		Sleep(100);

		MSG msg;
		while (PeekMessage (&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage  (&msg);
		}
	}
	return TRUE;
}
//----------------------------------------------------------------------------
//	Name:AudioMasterCallback
//	Introdcue:Audio master callback function
//----------------------------------------------------------------------------
long VSTCALLBACK AudioMasterCallback(AEffect *pEffect, long lCode, long lIndex, long lValue, void *ptr, float fOpt)
{
	if (NULL != g_pVSTiPlayer)
		return g_pVSTiPlayer->MidiMasterCallback(pEffect, lCode, lIndex, lValue, ptr, fOpt);

	return 0;
}	
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//	Name:CMLVSTiPlayerImpl
//	Introdcue:Construction
//----------------------------------------------------------------------------
CMLVSTiPlayerImpl::CMLVSTiPlayerImpl(void)
 : m_pOutDevManager(NULL)
 , m_dwWorkingThreadID(0)
 , m_hWorkingThread(NULL)
 , m_hProcessEvent(NULL)
{
	g_pVSTiPlayer = this;
}
//----------------------------------------------------------------------------
//	Name:~CMLVSTiPlayerImpl
//	Introdcue:Distruction
//----------------------------------------------------------------------------
CMLVSTiPlayerImpl::~CMLVSTiPlayerImpl(void)
{
	g_pVSTiPlayer = NULL;
	if (NULL != m_pOutDevManager)
	{
		delete m_pOutDevManager;
		m_pOutDevManager = NULL;
	}
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//	Name:Initialize
//	Introdcue:Initialize VSTi player
//----------------------------------------------------------------------------
BOOL CMLVSTiPlayerImpl::Initialize()
{
	// Create device manager
	if (NULL == m_pOutDevManager)
		m_pOutDevManager = new CVSTiOutDeviceManager();
	m_pOutDevManager->InitDeviceManager();
	
	// Create process thread and event	
	m_hProcessEvent = CreateEvent(	NULL,
									FALSE,
									FALSE,
									_T("WorkingProgEvent")
									);
	m_hWorkingThread = CreateThread( 0, 
									 0, 
									 WorkingThread, 
									 (LPVOID)this, 
									 0, 
									 &m_dwWorkingThreadID);
	//SetThreadPriority(m_hProcessThread, THREAD_PRIORITY_HIGHEST);
	WaitFor(m_hProcessEvent);
	PostThreadMessage(m_dwWorkingThreadID, PROC_MSG_START, 0, 0);

	return TRUE;
}
//----------------------------------------------------------------------------
DWORD CMLVSTiPlayerImpl::GetOutDeviceCnt()
{
	DWORD dwDeviceCnt = 0;
	if (NULL != m_pOutDevManager)
		dwDeviceCnt = m_pOutDevManager->GetDeviceCnt();

	return dwDeviceCnt;
}
//----------------------------------------------------------------------------
char* CMLVSTiPlayerImpl::GetDeviceName(int iIndex)
{
	LPCTSTR lpszDevName = NULL;
	if (NULL != m_pOutDevManager)
		lpszDevName = m_pOutDevManager->GetDeviceName(iIndex);

	static char scDevName[VSTI_MAXSTR_LEN] = {0};
#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, lpszDevName, -1, scDevName, VSTI_MAXSTR_LEN, NULL, NULL);
#else
	strcpy(scDevName, lpszDevName);
#endif	//UNICODE

	return scDevName;
}
//----------------------------------------------------------------------------
void CMLVSTiPlayerImpl::SetCurOutDevice(int iIndex)
{
	if (NULL != m_pOutDevManager)
	{
		m_pOutDevManager->SetCurrentDevice(iIndex);
	}
}
//----------------------------------------------------------------------------
//	Name:LoadVSTiEffect
//	Introdcue:Add a VSTi effect to player
//----------------------------------------------------------------------------
IMLVSTiEffect* CMLVSTiPlayerImpl::LoadVSTiEffect(const char* lpszEffect)
{
	if (NULL == lpszEffect || _tcslen(lpszEffect) == 0)
		return FALSE;

	CMLVSTiEffectImpl* pNewEffect = new CMLVSTiEffectImpl();
	pNewEffect->InitEffect(AudioMasterCallback);
	if (FALSE == pNewEffect->LoadEffect((char*)lpszEffect))
	{
		delete pNewEffect;
		return NULL;
	}
	m_arVSTiEffect.push_back(pNewEffect);

	pNewEffect->EffOpen();
	pNewEffect->EffSetSampleRate(VSTI_SAMPLE_RATE);
	pNewEffect->EffSetBlockSize(VSTI_BUFF_SIZE);
	pNewEffect->EffResume();
	pNewEffect->EffSetProgram(0);
	pNewEffect->EffStartProcess();

	return pNewEffect;
}
//----------------------------------------------------------------------------
//	Name:UnLoadVSTiEffect
//	Introdcue:Remove a VSTi effect from player, remove all VSTi effects if iIndex = -1
//----------------------------------------------------------------------------
void CMLVSTiPlayerImpl::UnLoadVSTiEffect(int iIndex)
{
	int iCurIndex = 0;
	int iTotalEffectCnt = (int)m_arVSTiEffect.size();
	for (PtrArraryEffect::iterator it = m_arVSTiEffect.begin();
		 it != m_arVSTiEffect.end() && iCurIndex < iTotalEffectCnt;
		 it++, iCurIndex++)
	{
		if (iCurIndex == iIndex || -1 == iIndex)
		{
			CMLVSTiEffectImpl* pEffect = *it;
			if (NULL != pEffect)
			{
				pEffect->UnloadEffect();
				delete pEffect;
				pEffect = NULL;
			}
			m_arVSTiEffect.erase(it);
			iTotalEffectCnt--;
		}
	}
}
//----------------------------------------------------------------------------
//	Name:GetVSTiEffectCnt
//	Introdcue:Return current VSTi Effects count in player
//----------------------------------------------------------------------------
DWORD CMLVSTiPlayerImpl::GetVSTiEffectCnt()
{
	DWORD dwEffectCnt = (DWORD)m_arVSTiEffect.size();
	return dwEffectCnt;
}
//----------------------------------------------------------------------------
//	Name:GetVSTiEffect
//	Introdcue:Return a VSTi effect pointer by index
//----------------------------------------------------------------------------
IMLVSTiEffect* CMLVSTiPlayerImpl::GetVSTiEffectByIndex(int iIndex)
{
	if (iIndex < 0 || iIndex >= (int)m_arVSTiEffect.size())
		return NULL;

	CMLVSTiEffectImpl* pEffect = NULL;

	int iCurIndex = 0;
	for (PtrArraryEffect::iterator it = m_arVSTiEffect.begin();
		 it != m_arVSTiEffect.end();
		 it++, iCurIndex++)
	{
		if (iCurIndex == iIndex)
		{
			pEffect = *it;
			break;
		}
	}

	return pEffect;
}
//----------------------------------------------------------------------------
//	Name:GetVSTiEffectIndex
//	Introdcue:Return a VSTi effect index by pointer
//----------------------------------------------------------------------------
int CMLVSTiPlayerImpl::GetVSTiEffectIndex(IMLVSTiEffect* pVSTiEffect)
{
	int iIndex = -1;
	int iCurIndex = 0;
	for (PtrArraryEffect::iterator it = m_arVSTiEffect.begin();
		 it != m_arVSTiEffect.end();
		 it++, iCurIndex++)
	{
		if (pVSTiEffect == *it)
		{
			iIndex = iCurIndex;
			break;
		}
	}

	return iIndex;
}
//----------------------------------------------------------------------------
//	Name:LoadMidiFile
//	Introdcue:Add a midi file to player, and specficed its related effect index
//----------------------------------------------------------------------------
IMLVSTiMidiFile* CMLVSTiPlayerImpl::LoadMidiFile(const char* lpszMidi)
{
	if (NULL == lpszMidi || _tcslen(lpszMidi) == 0)
		return FALSE;

	CMLVSTiMidiFileImpl* pNewMidiFile = new CMLVSTiMidiFileImpl();
	if (FALSE == pNewMidiFile->LoadMidiFile(lpszMidi))
	{
		delete pNewMidiFile;
		return NULL;
	}
	m_arVSTiMidiFile.push_back(pNewMidiFile);

	return pNewMidiFile;
}
//----------------------------------------------------------------------------
//	Name:UnLoadMidiFile
//	Introdcue:Remove a midi file from player, remove all midi files if iIndex = -1
//----------------------------------------------------------------------------
void CMLVSTiPlayerImpl::UnLoadMidiFile(int iIndex)
{
	int iCurIndex = 0;
	for (PtrArraryMidiFile::iterator it = m_arVSTiMidiFile.begin();
		 it != m_arVSTiMidiFile.end();
		 it++, iCurIndex++)
	{
		if (iCurIndex == iIndex || -1 == iIndex)
		{
			CMLVSTiMidiFileImpl* pMidiFile = *it;
			if (NULL != pMidiFile)
			{
				delete pMidiFile;
				pMidiFile = NULL;
			}
			m_arVSTiMidiFile.erase(it);
		}
	}
}
//----------------------------------------------------------------------------
//	Name:GetMidiFileCnt
//	Introdcue:Return current midi files count in player
//----------------------------------------------------------------------------
DWORD CMLVSTiPlayerImpl::GetMidiFileCnt()
{
	DWORD dwMidiFileCnt = (DWORD)m_arVSTiMidiFile.size();
	return dwMidiFileCnt;
}
//----------------------------------------------------------------------------
//	Name:GetMidiFile
//	Introdcue:Return a midi files pointer by index
//----------------------------------------------------------------------------
IMLVSTiMidiFile* CMLVSTiPlayerImpl::GetMidiFileByIndex(int iIndex)
{
	if (iIndex < 0 || iIndex >= (int)m_arVSTiMidiFile.size())
		return NULL;

	CMLVSTiMidiFileImpl* pMidiFile = NULL;

	int iCurIndex = 0;
	for (PtrArraryMidiFile::iterator it = m_arVSTiMidiFile.begin();
		 it != m_arVSTiMidiFile.end();
		 it++, iCurIndex++)
	{
		if (iCurIndex == iIndex)
		{
			pMidiFile = *it;
			break;
		}
	}

	return pMidiFile;
}
//----------------------------------------------------------------------------
//	Name:GetMidiFileIndex
//	Introdcue:Return a midi files index by pointer
//----------------------------------------------------------------------------
int	CMLVSTiPlayerImpl::GetMidiFileIndex(IMLVSTiMidiFile* pMidiFile)
{
	int iIndex = -1;
	int iCurIndex = 0;
	for (PtrArraryMidiFile::iterator it = m_arVSTiMidiFile.begin();
		 it != m_arVSTiMidiFile.end();
		 it++, iCurIndex++)
	{
		if (pMidiFile == *it)
		{
			iIndex = iCurIndex;
			break;
		}
	}

	return iIndex;
}
//----------------------------------------------------------------------------
//	Name:PlayMidiFile
//	Introdcue:Play a midi file by index
//----------------------------------------------------------------------------
BOOL CMLVSTiPlayerImpl::PlayMidiFile(int iIndex, int iEffectIndex)
{
	if (iEffectIndex >= (int)m_arVSTiEffect.size())
		return FALSE;

	if (iEffectIndex < 0)
		iEffectIndex = -1;

	return TRUE;
}
//----------------------------------------------------------------------------
//	Name:StopMidiFile
//	Introdcue:Play a midi file by index
//----------------------------------------------------------------------------
void CMLVSTiPlayerImpl::StopMidiFile(int iMidiIndex)
{
}
//----------------------------------------------------------------------------
BOOL CMLVSTiPlayerImpl::GetMidiPlayingInfo(int iIndex, MidiPlayingInfo &Info)
{
	return TRUE;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//	Name:MidiMasterCallback
//	Introdcue:Audio master callback function
//----------------------------------------------------------------------------
long CMLVSTiPlayerImpl::MidiMasterCallback(AEffect *pEffect, long lCode, long lIndex, long lValue, void *ptr, float fOpt)
{
	switch (lCode)
	{		
	case audioMasterProcessEvents:
		break;
	case audioMasterAutomate:
		return 0;
	case audioMasterVersion:
		#if defined(VST_2_4_EXTENSIONS)
			return 2400L;
		#elif defined(VST_2_3_EXTENSIONS)
			return 2300L;
		#elif defined(VST_2_2_EXTENSIONS)
			return 2200L;
		#elif defined(VST_2_1_EXTENSIONS)
			return 2100L;
		#else 
			return 2L;
		#endif
			break;
	case audioMasterWantMidi:
		return 1;
	case audioMasterNeedIdle:
		return 1;
	case audioMasterCanDo :
		return OnCanDo((const char *)ptr);
	case audioMasterIdle :
		{
			CMLVSTiEffectImpl* pVSTiEffect = GetEffectPtrByHandle(pEffect);
			if (NULL == pVSTiEffect)
			{
				pVSTiEffect->EffEditIdle();
			}
		}
		break;
	}

	return 0;
}
//----------------------------------------------------------------------------
//	Name:OnCanDo
//	Introdcue:Handler of audioMasterCanDo
//----------------------------------------------------------------------------
bool CMLVSTiPlayerImpl::OnCanDo(const char *ptr)
{
	if ((!strcmp(ptr, "sendVstEvents")) ||
		(!strcmp(ptr, "sendVstMidiEvent")) ||
		(!strcmp(ptr, "receiveVstEvents")) ||
		(!strcmp(ptr, "receiveVstMidiEvent")) ||
		(!strcmp(ptr, "sizeWindow")) ||
		(!strcmp(ptr, "sendVstMidiEventFlagIsRealtime")))
		return true;

	return false;
}
//----------------------------------------------------------------------------
//	Name:WorkingThread
//	Introdcue:The working thread function
//----------------------------------------------------------------------------
DWORD WINAPI CMLVSTiPlayerImpl::WorkingThread(LPVOID lpParam)
{
	CMLVSTiPlayerImpl *pVSTiPlayer = (CMLVSTiPlayerImpl*)lpParam;
	if (NULL == pVSTiPlayer)
		return 1;

	MSG msg;
	BOOL bCallCoUninitialzeOnExit = FALSE;
/*	HRESULT hrCoInit = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(SUCCEEDED(hrCoInit))
	{
		bCallCoUninitialzeOnExit = TRUE;
	}*/

	SetEvent(pVSTiPlayer->m_hProcessEvent);

	const int c_iTimeInterval = 85;
	const int c_iTimeInterval2 = 70;
	
	BOOL bIsStarted = FALSE;
	BOOL bExitThread = FALSE;
	DWORD dwCurrentTime = 0;
	eMidiPlayState eCurPlayState = eMidiPlayState_Ready;
	do
	{
		while(PeekMessage(&msg, 0, 0, 0,PM_REMOVE|PM_QS_POSTMESSAGE))
		{
			switch(msg.message)
			{
			case PROC_MSG_START:
				bIsStarted = TRUE;   
				break;	
			case PROC_MSG_PLAY:
				eCurPlayState = eMidiPlayState_Playing;
				//midiOutOpen(&pMidiDlg->m_hMidiOut,(UINT)-1, 0, 0, CALLBACK_NULL);  
				dwCurrentTime = ::GetTickCount(); 
				break;	
			case PROC_MSG_PAUSE:
				eCurPlayState = eMidiPlayState_Paused;
				break;	
			case PROC_MSG_STOP:
				eCurPlayState = eMidiPlayState_Stopped;
				/*pMidiDlg->GetEventsByDeltaTime(-1);
				if (pMidiDlg->m_hMidiOut)
					midiOutClose(pMidiDlg->m_hMidiOut); */
				break;	
			case PROC_MSG_END:
				eCurPlayState = eMidiPlayState_Stopped;
				bExitThread = TRUE;
				break;	
			default:
				{
					::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
					break;
				}
			}
		}

		Sleep(c_iTimeInterval);

	}while (!bExitThread);

	if (eMidiPlayState_Playing == eCurPlayState)
	{
		PtrArraryMidiFile::iterator it;
		DWORD dwDeltaTime = ::GetTickCount() - dwCurrentTime;

		// Perpare events based on current time
		for (it = m_arVSTiMidiFile.begin(); it != m_arVSTiMidiFile.end(); it++)
		{
			(*it)->PerpareEventsByDeltaTime(dwDeltaTime*1000);
			(*it)->SendCurEventsToEffect();
		}

		// Send audio data to device
		for (it = m_arVSTiMidiFile.begin(); it != m_arVSTiMidiFile.end(); it++)
		{
			(*it)->SendAudioDataToDevice();
		}
	}


/*	if(bCallCoUninitialzeOnExit)
	{
		::CoUninitialize();
	}*/

	return 0;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	Name:GetEffectPtrByHandle
//	Introdcue:Return a VSTi Effect object pointer by AEffect pointer
//----------------------------------------------------------------------------
CMLVSTiEffectImpl* CMLVSTiPlayerImpl::GetEffectPtrByHandle(AEffect *pEffect)
{
	CMLVSTiEffectImpl* pVSTiEffect = NULL;
	DWORD dwEffectCnt = GetVSTiEffectCnt();
	for (DWORD dwIndex = 0; dwIndex < dwEffectCnt; dwIndex++)
	{
		CMLVSTiEffectImpl* pTmpEffect = (CMLVSTiEffectImpl*)GetVSTiEffectByIndex(dwIndex);
		if (pTmpEffect->GetObject() == pEffect)
		{
			pVSTiEffect = pTmpEffect;
			break;
		}
	}

	return pVSTiEffect;
}
//----------------------------------------------------------------------------