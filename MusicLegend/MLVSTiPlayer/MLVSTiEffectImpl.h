//----------------------------------------------------------------------------
//	File Name:MLVSTiEffectImpl.h
//	Introduce:VSTi Effect implement class defintion
//	Creator:Youfang Yao
//	Date:2010-08-16
//----------------------------------------------------------------------------
#ifndef	_ML_VSTIEFFECT_H_
#define	_ML_VSTIEFFECT_H_
//----------------------------------------------------------------------------
#include "MLVSTiPlayer.h"
#include "CommonDef.h"
//----------------------------------------------------------------------------
class CMLVSTiEffectImpl : public IMLVSTiEffect
{
public:
	CMLVSTiEffectImpl(void);
	~CMLVSTiEffectImpl(void);
	//------------------------------------------------------------------------
public:
	/*	IMLVSTiEffect interface functions */
	void	GetEffectInfo(VSTiEffectInfo &Info);
	void	GetEditWindowRect(RECT &rcEdit);
	void	ShowEditWindow(HWND hParentWnd);
	void	CloseEditWindow();
	//------------------------------------------------------------------------
public:
	BOOL	InitEffect(AUDIOMASTERCALLBACKFUNC lpAudioMasterCallback);
	BOOL	LoadEffect(char* lpszEffectFile);
	void	UnloadEffect();
    void	SetIndex(LONG lIndex);
    long	GetIndex();
	AEffect*GetObject();
	//
    long	EffDispatch(long lCode, long lIndex = 0, long lValue = 0, void *ptr = NULL, float fOpt = 0.0f);
	void	EffProcess(float **ppInputs, float **ppOutputs, long lSampleFrames);
	void	EffProcessReplacing(float **ppInputs, float **ppOutputs, long lSampleFrames);
	void	EffProcessDoubleReplacing(double** ppInputs, double** ppOutputs, long lSampleFrames);
	void	EffSetParameter(long lIndex, float fParam);
	float	EffGetParameter(long lIndex);
	int		EffGetInputNum();
	int		EffGetOutputNum();
	//	
    void	EffOpen();
    void	EffClose();
    void	EffSetProgram(long lValue);
    long	EffGetProgram();
    void	EffSetProgramName(char *ptr);
    void	EffGetProgramName(char *ptr);
    void	EffGetParamLabel(long lIndex, char *ptr);
    void	EffGetParamDisplay(long lIndex, char *ptr);
    void	EffGetParamName(long lIndex, char *ptr);
    void	EffSetSampleRate(float fSampleRate);
    void	EffSetBlockSize(long lValue);
	//
    void	EffSuspend();
    void	EffResume();
    float	EffGetVu();
    long	EffEditGetRect(ERect **ptr);
    long	EffEditOpen(void *ptr);
    void	EffEditClose();
    void	EffEditIdle();
	//VST 2.0 extensions
    long	EffProcessEvents(VstEvents* ptr);
    long	EffGetNumProgramCategories();
    long	EffGetProgramNameIndexed(long lCategory, long lIndex, char* text);
    long	EffGetPlugCategory();
    long	EffGetCurrentPosition();
    long	EffGetDestinationBuffer();
    long	EffSetBypass(bool onOff);
    long	EffGetEffectName(char *ptr);
    long	EffGetErrorText(char *ptr);
    long	EffGetVendorString(char *ptr);
    long	EffGetProductString(char *ptr);
    long	EffGetVendorVersion();
    long	EffVendorSpecific(long lIndex, long lValue, void *ptr, float fOpt);
    long	EffCanDo(const char *ptr);
    long	EffGetVstVersion();
    //VST 2.1 extensions
    long	EffGetMidiProgramName(long lChannel, MidiProgramName* midiProgramName);
    long	EffGetCurrentMidiProgram (long lChannel, MidiProgramName* currentProgram);
    long	EffGetMidiProgramCategory (long lChannel, MidiProgramCategory* category);
    long	EffHasMidiProgramsChanged (long lChannel);
    long	EffGetMidiKeyName(long lChannel, MidiKeyName* keyName);
    //VST 2.3 Extensions
    long	EffStartProcess();
    long	EffStopProcess();
	//VST 2.4 extensions
    long	EffSetProcessPrecision(long lPrecision);
    long	EffGetNumMidiInputChannels();
    long	EffGetNumMidiOutputChannels();
	//------------------------------------------------------------------------
private:
	void	CreateBuffers();
	void	ReleaseBuffers();
	//------------------------------------------------------------------------
private:
    AEffect*	m_pEffect;
	long		m_lIndex;  
    BOOL		m_bEditOpen;
	BOOL		m_bNeedIdle;
    BOOL		m_bInEditIdle;
    BOOL		m_bWantMidi;
	HMODULE		m_hModule;
    char		m_sFileName[VSTI_MAXSTR_LEN];
    char		m_sFileDir[VSTI_MAXSTR_LEN];		
	AUDIOMASTERCALLBACKFUNC m_lpAudioMasterCallback;
	//
	float**		m_fInputData;
	float**		m_fOutputData;
	//------------------------------------------------------------------------
};
//----------------------------------------------------------------------------
#endif	//_ML_VSTIEFFECT_H_
//----------------------------------------------------------------------------