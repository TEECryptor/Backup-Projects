//----------------------------------------------------------------------------
//	File Name:MLVSTiEffectImpl.cpp
//	Introduce:Class CMLVSTiEffectImpl implement file
//	Creator:Youfang Yao
//	Date:2010-08-16
//----------------------------------------------------------------------------
#include "StdAfx.h"
#include ".\mlvstieffectimpl.h"
//----------------------------------------------------------------------------
CMLVSTiEffectImpl::CMLVSTiEffectImpl(void)
: m_lpAudioMasterCallback(NULL)
, m_pEffect(NULL)
, m_lIndex(-1)
, m_bEditOpen(FALSE)
, m_bNeedIdle(FALSE)
, m_bInEditIdle(FALSE)
, m_bWantMidi(FALSE)
, m_hModule(NULL)
, m_fInputData(NULL)
, m_fOutputData(NULL)
{
}
//----------------------------------------------------------------------------
CMLVSTiEffectImpl::~CMLVSTiEffectImpl(void)
{
	ReleaseBuffers();
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//	Name:GetEffectInfo
//	Introdcue:Return information of this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::GetEffectInfo(VSTiEffectInfo &Info)
{
	strcpy(Info.csFileName, m_sFileName);
	EffGetEffectName(Info.csEffectName);
}
//----------------------------------------------------------------------------
//	Name:GetEditWindowRect
//	Introdcue:Return the edit window rect of this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::GetEditWindowRect(RECT &rcEdit)
{
	ERect *prc = NULL;
	EffEditGetRect(&prc);

	if (NULL != prc)
	{
		rcEdit.left = prc->left;
		rcEdit.right = prc->right;
		rcEdit.top = prc->top;
		rcEdit.bottom = prc->bottom;
	}
}
//----------------------------------------------------------------------------
//	Name:ShowEditWindow
//	Introdcue:Show the edit window of this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::ShowEditWindow(HWND hParentWnd)
{
	EffEditOpen(hParentWnd);
}
//----------------------------------------------------------------------------
//	Name:CloseEditWindow
//	Introdcue:Close the edit window of this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::CloseEditWindow()
{
	EffEditClose();
}
//----------------------------------------------------------------------------


    
//----------------------------------------------------------------------------
//	Name:InitEffect
//	Introdcue:Initliaze effect,set audio master callback function pointer
//----------------------------------------------------------------------------
BOOL CMLVSTiEffectImpl::InitEffect(AUDIOMASTERCALLBACKFUNC lpAudioMasterCallback)
{
	if (NULL == lpAudioMasterCallback)
		return FALSE;

	m_lpAudioMasterCallback = lpAudioMasterCallback;
	return TRUE;
}
//----------------------------------------------------------------------------
//	Name:LoadEffect
//	Introdcue:Load VSTi effect by effect file name
//----------------------------------------------------------------------------
BOOL CMLVSTiEffectImpl::LoadEffect(char* lpszEffectFile)
{
	if (NULL == lpszEffectFile || strlen(lpszEffectFile) == 0)
		return FALSE;

	if (NULL == m_lpAudioMasterCallback)
		return FALSE;
	
	UnloadEffect();

	try
	{
		m_hModule = ::LoadLibrary(lpszEffectFile);
	}
	catch(...)
	{
		m_hModule = NULL;
	}

	AEffect *(*pMain)(long (*audioMaster)(AEffect *effect,
                                  long opcode,
                                  long index,
                                  long value,
                                  void *ptr,
                                  float opt)) = 0;
	if (NULL != m_hModule)
	{
		pMain = (AEffect * (*)(long (*)(AEffect *,long,long,long,void *,float)))::GetProcAddress(m_hModule, "VSTPluginMain");
		if (!pMain)
			pMain = (AEffect * (*)(long (*)(AEffect *,long,long,long,void *,float)))::GetProcAddress(m_hModule, "main");
	}

	//Initialize effect
	if (pMain)
	{
		try
		{
			m_pEffect = pMain(m_lpAudioMasterCallback);
		}
		catch(...)
		{
			m_pEffect = NULL;
		}
	}

	//Check for correctness
	if (m_pEffect && (m_pEffect->magic != kEffectMagic))
		m_pEffect = NULL;

	if (NULL != m_pEffect)
	{
		// Create buffers
		CreateBuffers();

		// Save file name
		strcpy(m_sFileName, lpszEffectFile);
		const char *p = strrchr(lpszEffectFile, '\\');
		if (p)
		{
			memcpy(m_sFileDir, lpszEffectFile, p - lpszEffectFile);
		}
	}

	return !!m_pEffect;
}
//----------------------------------------------------------------------------
//	Name:UnloadEffect
//	Introdcue:Unload this VSTi effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::UnloadEffect()
{
	ReleaseBuffers();

	EffClose();
	m_pEffect = NULL;

	if (m_hModule)
	{
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}
//----------------------------------------------------------------------------
//	Name:SetIndex
//	Introdcue:Set this effect index in VSTi Host effect arrary
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::SetIndex(long lIndex)
{
	m_lIndex = lIndex;
}
//----------------------------------------------------------------------------
//	Name:GetIndex
//	Introdcue:Return this effect index in VSTi Host effect arrary
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::GetIndex()
{
	return m_lIndex;
}
//----------------------------------------------------------------------------
//	Name:GetObject
//	Introdcue:Return this effect object pointer
//----------------------------------------------------------------------------
AEffect* CMLVSTiEffectImpl::GetObject()
{
	return m_pEffect;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	Name:EffDispatch
//	Introdcue:Calls the effect's dispatcher
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffDispatch(long lCode, long lIndex, long lValue, void *ptr, float fOpt)
{
	if (NULL == m_pEffect)
		return 0;

	return m_pEffect->dispatcher(m_pEffect, lCode, lIndex, lValue, ptr, fOpt);
}
//----------------------------------------------------------------------------
//	Name:EffProcess
//	Introdcue:Calls the effect's process() function  
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffProcess(float **ppInputs, float **ppOutputs, long lSampleFrames)
{
	if (NULL == m_pEffect)
		return;

	m_pEffect->process(m_pEffect, ppInputs, ppOutputs, lSampleFrames);
}
//----------------------------------------------------------------------------
//	Name:EffProcessReplacing
//	Introdcue:Calls the effect's processReplacing() function  
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffProcessReplacing(float **ppInputs, float **ppOutputs, long lSampleFrames)
{
	if (NULL != m_pEffect || !(m_pEffect->flags & effFlagsCanReplacing))
		return;

	m_pEffect->processReplacing(m_pEffect, ppInputs, ppOutputs, lSampleFrames);
}
//----------------------------------------------------------------------------
//	Name:EffProcessReplacing
//	Introdcue:Calls the effect's processDoubleReplacing() function  
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffProcessDoubleReplacing(double** ppInputs, double** ppOutputs, long lSampleFrames)
{
	if (NULL != m_pEffect || !(m_pEffect->flags & effFlagsCanDoubleReplacing))
		return;

#if defined(VST_2_4_EXTENSIONS)
	m_pEffect->processDoubleReplacing(m_pEffect, ppInputs, ppOutputs, lSampleFrames);
#endif
}
//----------------------------------------------------------------------------
//	Name:EffSetParameter
//	Introdcue:Calls the effect's setParameter() function  
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffSetParameter(long lIndex, float fParam)
{
	if (NULL == m_pEffect)
		return;

	m_pEffect->setParameter(m_pEffect, lIndex, fParam);
}
//----------------------------------------------------------------------------
//	Name:EffGetParameter
//	Introdcue:Calls the effect's getParameter() function  
//----------------------------------------------------------------------------
float CMLVSTiEffectImpl::EffGetParameter(long lIndex)
{
	if (NULL == m_pEffect)
		return 0.0f;

	float fResult = m_pEffect->getParameter(m_pEffect, lIndex);

	return fResult;
}
//----------------------------------------------------------------------------
//	Name:EffGetInputNum
//	Introdcue:Return the effect's input number
//----------------------------------------------------------------------------
int CMLVSTiEffectImpl::EffGetInputNum()
{
	if (NULL == m_pEffect)
		return 0;

	return m_pEffect->numInputs;
}
//----------------------------------------------------------------------------
//	Name:EffGetOutputNum
//	Introdcue:Return the effect's output number
//----------------------------------------------------------------------------
int CMLVSTiEffectImpl::EffGetOutputNum()
{
	if (NULL == m_pEffect)
		return 0;

	return m_pEffect->numOutputs;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//	Name:EffOpen
//	Introdcue:Open this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffOpen()
{
	EffDispatch(effOpen);
}
//----------------------------------------------------------------------------
//	Name:EffClose
//	Introdcue:Close this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffClose()
{
	EffDispatch(effClose);
}
//----------------------------------------------------------------------------
//	Name:EffSetProgram
//	Introdcue:Set programe value to this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffSetProgram(long lValue)
{
	EffDispatch(effBeginSetProgram); 
	EffDispatch(effSetProgram, 0, lValue); 
	EffDispatch(effEndSetProgram);
}
//----------------------------------------------------------------------------
//	Name:EffGetProgram
//	Introdcue:Get programe value from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetProgram()
{
	return EffDispatch(effGetProgram);
}
//----------------------------------------------------------------------------
//	Name:EffSetProgramName
//	Introdcue:Set programe name to this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffSetProgramName(char *ptr)
{
	EffDispatch(effSetProgramName, 0, 0, ptr); 
}
//----------------------------------------------------------------------------
//	Name:EffGetProgramName
//	Introdcue:Return programe name from this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffGetProgramName(char *ptr)
{
	EffDispatch(effGetProgramName, 0, 0, ptr); 
}
//----------------------------------------------------------------------------
//	Name:EffGetParamLabel
//	Introdcue:Return parameter label from this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffGetParamLabel(long lIndex, char *ptr)
{
	EffDispatch(effGetParamLabel, lIndex, 0, ptr);
}
//----------------------------------------------------------------------------
//	Name:EffGetParamDisplay
//	Introdcue:Return parameter display from this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffGetParamDisplay(long lIndex, char *ptr)
{
	EffDispatch(effGetParamDisplay, lIndex, 0, ptr);
}
//----------------------------------------------------------------------------
//	Name:EffGetParamName
//	Introdcue:Return parameter display from this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffGetParamName(long lIndex, char *ptr)
{
	EffDispatch(effGetParamName, lIndex, 0, ptr);
}
//----------------------------------------------------------------------------
//	Name:EffSetSampleRate
//	Introdcue:Set sample rate to this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffSetSampleRate(float fSampleRate)
{
	EffDispatch(effSetSampleRate, 0, 0, 0, fSampleRate);
}
//----------------------------------------------------------------------------
//	Name:EffSetBlockSize
//	Introdcue:Set block size to this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffSetBlockSize(long lValue)
{
	EffDispatch(effSetBlockSize, 0, lValue);
}
//----------------------------------------------------------------------------
//	Name:EffSuspend
//	Introdcue:Suspend this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffSuspend()
{
	EffDispatch(effMainsChanged, 0, false);
}
//----------------------------------------------------------------------------
//	Name:EffResume
//	Introdcue:Resume this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffResume()
{
	EffDispatch(effMainsChanged, 0, true);
}
//----------------------------------------------------------------------------
//	Name:EffGetVu
//	Introdcue:Return Vu from this effect
//----------------------------------------------------------------------------
float CMLVSTiEffectImpl::EffGetVu()
{
	return (float)EffDispatch(effGetVu) / (float)32767.0f;
}
//----------------------------------------------------------------------------
//	Name:EffEditGetRect
//	Introdcue:Return effect window rect from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffEditGetRect(ERect **ptr)
{
	return EffDispatch(effEditGetRect, 0, 0, ptr);
}
//----------------------------------------------------------------------------
//	Name:EffEditOpen
//	Introdcue:Open effect window from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffEditOpen(void *ptr)
{
	long lRes = EffDispatch(effEditOpen, 0, 0, ptr); 
	m_bEditOpen = TRUE; 
	return lRes;
}
//----------------------------------------------------------------------------
//	Name:EffEditClose
//	Introdcue:Close effect window from this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffEditClose()
{
	EffDispatch(effEditClose); 
	m_bEditOpen = FALSE; 
}
//----------------------------------------------------------------------------
//	Name:EffEditIdle
//	Introdcue:Set effect edit idle to this effect
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::EffEditIdle()
{
	if (FALSE == m_bEditOpen || TRUE ==  m_bInEditIdle)
		return; 

	m_bInEditIdle = TRUE; 
	EffDispatch(effEditIdle); 
	m_bInEditIdle = FALSE; 
}
//----------------------------------------------------------------------------
//	Name:EffProcessEvents
//	Introdcue:Process events by this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffProcessEvents(VstEvents* ptr)
{
	return EffDispatch(effProcessEvents, 0, 0, ptr);
}		
//----------------------------------------------------------------------------
//	Name:EffGetNumProgramCategories
//	Introdcue:Return programe categories number from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetNumProgramCategories()
{
	return EffDispatch(effGetNumProgramCategories);
}		
//----------------------------------------------------------------------------
//	Name:EffGetProgramNameIndexed
//	Introdcue:Return programe name index from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetProgramNameIndexed(long lCategory, long lIndex, char* text)
{
	return EffDispatch(effGetProgramNameIndexed, lIndex, lCategory, text);
}		
//----------------------------------------------------------------------------
//	Name:EffGetPlugCategory
//	Introdcue:Return plug category from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetPlugCategory()
{
	return EffDispatch(effGetPlugCategory);
}		
//----------------------------------------------------------------------------
//	Name:EffGetCurrentPosition
//	Introdcue:Return current position from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetCurrentPosition()
{
	return EffDispatch(effGetCurrentPosition);
}		
//----------------------------------------------------------------------------
//	Name:EffGetDestinationBuffer
//	Introdcue:Return destination buffer from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetDestinationBuffer()
{
	return EffDispatch(effGetDestinationBuffer);
}		
//----------------------------------------------------------------------------
//	Name:EffSetBypass
//	Introdcue:Set by pass or not to this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffSetBypass(bool onOff)
{
	return EffDispatch(effSetBypass, 0, onOff);
}		
//----------------------------------------------------------------------------
//	Name:EffGetEffectName
//	Introdcue:Get effect name string from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetEffectName(char *ptr)
{
	return EffDispatch(effGetEffectName, 0, 0, ptr);
}		
//----------------------------------------------------------------------------
//	Name:EffGetErrorText
//	Introdcue:Get error string from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetErrorText(char *ptr)
{
	return EffDispatch(effGetErrorText, 0, 0, ptr);
}		
//----------------------------------------------------------------------------
//	Name:EffGetVendorString
//	Introdcue:Get vendor string from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetVendorString(char *ptr)
{
	return EffDispatch(effGetVendorString, 0, 0, ptr);
}		
//----------------------------------------------------------------------------
//	Name:EffGetProductString
//	Introdcue:Get product string from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetProductString(char *ptr)
{
	return EffDispatch(effGetProductString, 0, 0, ptr);
}		
//----------------------------------------------------------------------------
//	Name:EffGetVendorVersion
//	Introdcue:Get vendor version from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetVendorVersion()
{
	return EffDispatch(effGetVendorVersion);
}		
//----------------------------------------------------------------------------
//	Name:EffVendorSpecific
//	Introdcue:Get vendor specific from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffVendorSpecific(long lIndex, long lValue, void *ptr, float fOpt)
{
	return EffDispatch(effVendorSpecific, lIndex, lValue, ptr, fOpt);
}		
//----------------------------------------------------------------------------
//	Name:EffCanDo
//	Introdcue:Return Can Do what from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffCanDo(const char *ptr)
{
	return EffDispatch(effCanDo, 0, 0, (void *)ptr);
}		
//----------------------------------------------------------------------------
//	Name:EffGetVstVersion
//	Introdcue:Return VST version from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetVstVersion()
{
	return EffDispatch(effGetVstVersion);
}		
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//	Name:EffGetMidiProgramName
//	Introdcue:Return Mdid programe name from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetMidiProgramName(long lChannel, MidiProgramName* midiProgramName)
{
	return EffDispatch(effGetMidiProgramName, lChannel, 0, midiProgramName);
}
//----------------------------------------------------------------------------
//	Name:EffGetCurrentMidiProgram
//	Introdcue:Return current Mdid programe name  from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetCurrentMidiProgram (long lChannel, MidiProgramName* currentProgram)
{
	return EffDispatch(effGetCurrentMidiProgram, lChannel, 0, currentProgram);
}
//----------------------------------------------------------------------------
//	Name:EffGetMidiProgramCategory
//	Introdcue:Return Midi programe category from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetMidiProgramCategory (long lChannel, MidiProgramCategory* category)
{
	return EffDispatch(effGetMidiProgramCategory, lChannel, 0, category);
}
//----------------------------------------------------------------------------
//	Name:EffHasMidiProgramsChanged
//	Introdcue:Return has midi programs changed from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffHasMidiProgramsChanged (long lChannel)
{
	return EffDispatch(effHasMidiProgramsChanged, lChannel);
}
//----------------------------------------------------------------------------
//	Name:EffGetMidiKeyName
//	Introdcue:Return Midi key name from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetMidiKeyName(long lChannel, MidiKeyName* keyName)
{
	return EffDispatch(effGetMidiKeyName, lChannel, 0, keyName);
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//	Name:EffStartProcess
//	Introdcue:Start this effect process
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffStartProcess()
{
	return EffDispatch(effStartProcess);
}
//----------------------------------------------------------------------------
//	Name:EffStopProcess
//	Introdcue:Stop this effect process
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffStopProcess()
{
	return EffDispatch(effStopProcess);
}
//----------------------------------------------------------------------------
//	Name:EffSetProcessPrecision
//	Introdcue:Return process precision from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffSetProcessPrecision(long lPrecision)
{
	return EffDispatch(effSetProcessPrecision, 0, lPrecision, 0);
}
//----------------------------------------------------------------------------
//	Name:EffGetNumMidiInputChannels
//	Introdcue:Return Midi input channels number from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetNumMidiInputChannels()
{
	return EffDispatch(effGetNumMidiInputChannels, 0, 0, 0);
}
//----------------------------------------------------------------------------
//	Name:EffGetNumMidiOutputChannels
//	Introdcue:Return Midi output channels number from this effect
//----------------------------------------------------------------------------
long CMLVSTiEffectImpl::EffGetNumMidiOutputChannels()
{
	return EffDispatch(effGetNumMidiOutputChannels, 0, 0, 0);
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::CreateBuffers()
{
	// Release the old buffers at first
	ReleaseBuffers();

	int iInputNum = EffGetInputNum();
	int iOutputNum = EffGetOutputNum();

	if (iInputNum > 0)
	{
		m_fInputData = new float *[iInputNum];
		for(int iNum = 0; iNum < iInputNum; iNum++)
		{
			m_fInputData[iNum] = new float[VSTI_BUFF_SIZE];
			memset(m_fInputData[iNum], 0, sizeof(float)*VSTI_BUFF_SIZE);
		}
	}
	if (iOutputNum > 0)
	{
		m_fOutputData = new float *[iOutputNum];
		for(int iNum = 0; iNum < iOutputNum; iNum++)
		{
			m_fOutputData[iNum] = new float[VSTI_BUFF_SIZE];
			memset(m_fOutputData[iNum], 0, sizeof(float)*VSTI_BUFF_SIZE);
		}
	}
}
//----------------------------------------------------------------------------
void CMLVSTiEffectImpl::ReleaseBuffers()
{
	int iInputNum = EffGetInputNum();
	int iOutputNum = EffGetOutputNum();

	if (iInputNum > 0)
	{
		for(int iNum = 0; iNum < iInputNum; iNum++)
			delete []m_fInputData[iNum];
		delete []m_fInputData;
	}
	if (iOutputNum > 0)
	{
		for(int iNum = 0; iNum < iOutputNum; iNum++)
			delete []m_fOutputData[iNum];
		delete []m_fOutputData;
	}
}
//----------------------------------------------------------------------------