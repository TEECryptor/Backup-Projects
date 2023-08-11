#pragma once

#define VST_FORCE_DEPRECATED 0 

#include "audioeffectx.h"               /* VST header files                  */

typedef long(VSTCALLBACK *AUDIOMATSERCALLBACKFUNC)(AEffect *effect, long opcode, long index, long value, void *ptr, float opt);

class CEffect
{
public:
    CEffect(AUDIOMATSERCALLBACKFUNC lpAudioMasterCallback);
    virtual ~CEffect();

public:
    //CVSTHost *pHost;
	AUDIOMATSERCALLBACKFUNC m_lpAudioMasterCallback;
    AEffect *pEffect;
    char *sName;
    bool bEditOpen;
	bool bNeedIdle;
    bool bInEditIdle;
    bool bWantMidi;
    bool bInSetProgram;
    long nIndex;                        /* index in VSTHost plugin array     */
    long nUniqueId;                     /* unique plugin ID (shell plugin)   */
    CEffect *pMasterEffect;             /* for Shell-type plugins            */

#ifdef WIN32

    HMODULE hModule;
    char *sDir;

#elif MAC

// yet to do
// no idea how things look here...

#endif

    virtual bool Load(const char *name);
    virtual bool Unload();

    void SetIndex(int nNewIndex) { nIndex = nNewIndex; }
    long GetIndex() { return nIndex; }

    virtual bool LoadBank(char *name);
    virtual bool SaveBank(char *name);

    virtual long EffDispatch(long opCode, long index=0, long value=0, void *ptr=0, float opt=0.);
	virtual void EffProcess(float **inputs, float **outputs, long sampleframes);
	virtual void EffProcessReplacing(float **inputs, float **outputs, long sampleframes);
	virtual void EffProcessDoubleReplacing(double** inputs, double** outputs, long sampleFrames);
	virtual void EffSetParameter(long index, float parameter);
	virtual float EffGetParameter(long index);

    virtual void EffOpen() { EffDispatch(effOpen); }
    virtual void EffClose() { EffDispatch(effClose); }
    virtual void EffSetProgram(long lValue) { EffBeginSetProgram(); EffDispatch(effSetProgram, 0, lValue); EffEndSetProgram(); }
    virtual long EffGetProgram() { return EffDispatch(effGetProgram); }
    virtual void EffSetProgramName(char *ptr) { EffDispatch(effSetProgramName, 0, 0, ptr); }
    virtual void EffGetProgramName(char *ptr) { EffDispatch(effGetProgramName, 0, 0, ptr); }
    virtual void EffGetParamLabel(long index, char *ptr) { EffDispatch(effGetParamLabel, index, 0, ptr); }
    virtual void EffGetParamDisplay(long index, char *ptr) { EffDispatch(effGetParamDisplay, index, 0, ptr); }
    virtual void EffGetParamName(long index, char *ptr) { EffDispatch(effGetParamName, index, 0, ptr); }
    virtual void EffSetSampleRate(float fSampleRate) { EffDispatch(effSetSampleRate, 0, 0, 0, fSampleRate); }
    virtual void EffSetBlockSize(long value) { EffDispatch(effSetBlockSize, 0, value); }
    virtual void EffMainsChanged(bool bOn) { EffDispatch(effMainsChanged, 0, bOn); }
    virtual void EffSuspend() { EffDispatch(effMainsChanged, 0, false); }
    virtual void EffResume() { EffDispatch(effMainsChanged, 0, true); }
    virtual float EffGetVu() { return (float)EffDispatch(effGetVu) / (float)32767.; }
    virtual long EffEditGetRect(ERect **ptr) { return EffDispatch(effEditGetRect, 0, 0, ptr); }
    virtual long EffEditOpen(void *ptr) { long l = EffDispatch(effEditOpen, 0, 0, ptr); /* if (l > 0) */ bEditOpen = true; return l; }
    virtual void EffEditClose() { EffDispatch(effEditClose); bEditOpen = false; }
    virtual void EffEditIdle() { if ((!bEditOpen) || (bInEditIdle)) return; bInEditIdle = true; EffDispatch(effEditIdle); bInEditIdle = false; }
#if MAC
    virtual void EffEditDraw(void *ptr) { EffDispatch(nEffect, effEditDraw, 0, 0, ptr); }
    virtual long EffEditMouse(long index, long value) { return EffDispatch(nEffect, effEditMouse, index, value); }
    virtual long EffEditKey(long value) { return EffDispatch(effEditKey, 0, value); }
    virtual void EffEditTop() { EffDispatch(effEditTop); }
    virtual void EffEditSleep() { EffDispatch(effEditSleep); }
#endif
    virtual long EffIdentify() { return EffDispatch(effIdentify); }
    virtual long EffGetChunk(void **ptr, bool isPreset = false) { return EffDispatch(effGetChunk, isPreset, 0, ptr); }
    virtual long EffSetChunk(void *data, long byteSize, bool isPreset = false) { EffBeginSetProgram(); long lResult = EffDispatch(effSetChunk, isPreset, byteSize, data); EffEndSetProgram(); return lResult;}
                                        /* VST 2.0                           */
    virtual long EffProcessEvents(VstEvents* ptr) { return EffDispatch(effProcessEvents, 0, 0, ptr); }
    virtual long EffCanBeAutomated(long index) { return EffDispatch(effCanBeAutomated, index); }
    virtual long EffString2Parameter(long index, char *ptr) { return EffDispatch(effString2Parameter, index, 0, ptr); }
    virtual long EffGetNumProgramCategories() { return EffDispatch(effGetNumProgramCategories); }
    virtual long EffGetProgramNameIndexed(long category, long index, char* text) { return EffDispatch(effGetProgramNameIndexed, index, category, text); }
    virtual long EffCopyProgram(long index) { return EffDispatch(effCopyProgram, index); }
    virtual long EffConnectInput(long index, bool state) { return EffDispatch(effConnectInput, index, state); }
    virtual long EffConnectOutput(long index, bool state) { return EffDispatch(effConnectOutput, index, state); }
    virtual long EffGetInputProperties(long index, VstPinProperties *ptr) { return EffDispatch(effGetInputProperties, index, 0, ptr); }
    virtual long EffGetOutputProperties(long index, VstPinProperties *ptr) { return EffDispatch(effGetOutputProperties, index, 0, ptr); }
    virtual long EffGetPlugCategory() { return EffDispatch(effGetPlugCategory); }
    virtual long EffGetCurrentPosition() { return EffDispatch(effGetCurrentPosition); }
    virtual long EffGetDestinationBuffer() { return EffDispatch(effGetDestinationBuffer); }
    virtual long EffOfflineNotify(VstAudioFile* ptr, long numAudioFiles, bool start) { return EffDispatch(effOfflineNotify, start, numAudioFiles, ptr); }
    virtual long EffOfflinePrepare(VstOfflineTask *ptr, long count) { return EffDispatch(effOfflinePrepare, 0, count, ptr); }
    virtual long EffOfflineRun(VstOfflineTask *ptr, long count) { return EffDispatch(effOfflineRun, 0, count, ptr); }
    virtual long EffProcessVarIo(VstVariableIo* varIo) { return EffDispatch(effProcessVarIo, 0, 0, varIo); }
    virtual long EffSetSpeakerArrangement(VstSpeakerArrangement* pluginInput, VstSpeakerArrangement* pluginOutput) { return EffDispatch(effSetSpeakerArrangement, 0, (long)pluginInput, pluginOutput); }
    virtual long EffSetBlockSizeAndSampleRate(long blockSize, float sampleRate) { return EffDispatch(effSetBlockSizeAndSampleRate, 0, blockSize, 0, sampleRate); }
    virtual long EffSetBypass(bool onOff) { return EffDispatch(effSetBypass, 0, onOff); }
    virtual long EffGetEffectName(char *ptr) { return EffDispatch(effGetEffectName, 0, 0, ptr); }
    virtual long EffGetErrorText(char *ptr) { return EffDispatch(effGetErrorText, 0, 0, ptr); }
    virtual long EffGetVendorString(char *ptr) { return EffDispatch(effGetVendorString, 0, 0, ptr); }
    virtual long EffGetProductString(char *ptr) { return EffDispatch(effGetProductString, 0, 0, ptr); }
    virtual long EffGetVendorVersion() { return EffDispatch(effGetVendorVersion); }
    virtual long EffVendorSpecific(long index, long value, void *ptr, float opt) { return EffDispatch(effVendorSpecific, index, value, ptr, opt); }
    virtual long EffCanDo(const char *ptr) { return EffDispatch(effCanDo, 0, 0, (void *)ptr); }
    virtual long EffGetTailSize() { return EffDispatch(effGetTailSize); }
    virtual long EffIdle() { if (bNeedIdle) return EffDispatch(effIdle); else return 0; }
    virtual long EffGetIcon() { return EffDispatch(effGetIcon); }
    virtual long EffSetViewPosition(long x, long y) { return EffDispatch(effSetViewPosition, x, y); }
    virtual long EffGetParameterProperties(long index, VstParameterProperties* ptr) { return EffDispatch(effGetParameterProperties, index, 0, ptr); }
    virtual long EffKeysRequired() { return EffDispatch(effKeysRequired); }
    virtual long EffGetVstVersion() { return EffDispatch(effGetVstVersion); }
                                        /* VST 2.1 extensions                */
    virtual long EffKeyDown(VstKeyCode &keyCode) { return EffDispatch(effEditKeyDown, keyCode.character, keyCode.virt, 0, keyCode.modifier); }
    virtual long EffKeyUp(VstKeyCode &keyCode) { return EffDispatch(effEditKeyUp, keyCode.character, keyCode.virt, 0, keyCode.modifier); }
    virtual void EffSetKnobMode(long value) { EffDispatch(effSetEditKnobMode, 0, value); }
    virtual long EffGetMidiProgramName(long channel, MidiProgramName* midiProgramName) { return EffDispatch(effGetMidiProgramName, channel, 0, midiProgramName); }
    virtual long EffGetCurrentMidiProgram (long channel, MidiProgramName* currentProgram) { return EffDispatch(effGetCurrentMidiProgram, channel, 0, currentProgram); }
    virtual long EffGetMidiProgramCategory (long channel, MidiProgramCategory* category) { return EffDispatch(effGetMidiProgramCategory, channel, 0, category); }
    virtual long EffHasMidiProgramsChanged (long channel) { return EffDispatch(effHasMidiProgramsChanged, channel); }
    virtual long EffGetMidiKeyName(long channel, MidiKeyName* keyName) { return EffDispatch(effGetMidiKeyName, channel, 0, keyName); }
    virtual long EffBeginSetProgram() { bInSetProgram = !!EffDispatch(effBeginSetProgram); return bInSetProgram; }
    virtual long EffEndSetProgram() { bInSetProgram = false; return EffDispatch(effEndSetProgram); }
                                        /* VST 2.3 Extensions                */
    virtual long EffGetSpeakerArrangement(VstSpeakerArrangement** pluginInput, VstSpeakerArrangement** pluginOutput) { return EffDispatch(effGetSpeakerArrangement, 0, (long)pluginInput, pluginOutput); }
    virtual long EffSetTotalSampleToProcess (long value) { return EffDispatch(effSetTotalSampleToProcess, 0, value); }
    virtual long EffGetNextShellPlugin(char *name) { return EffDispatch(effShellGetNextPlugin, 0, 0, name); }
    virtual long EffStartProcess() { return EffDispatch(effStartProcess); }
    virtual long EffStopProcess() { return EffDispatch(effStopProcess); }
    virtual long EffSetPanLaw(long type, float val) { return EffDispatch(effSetPanLaw, 0, type, 0, val); }
    virtual long EffBeginLoadBank(VstPatchChunkInfo* ptr) { return EffDispatch(effBeginLoadBank, 0, 0, ptr); }
    virtual long EffBeginLoadProgram(VstPatchChunkInfo* ptr) { return EffDispatch(effBeginLoadProgram, 0, 0, ptr); }
                                        /* VST 2.4 extensions                */
    virtual long EffSetProcessPrecision(long precision) { return EffDispatch(effSetProcessPrecision, 0, precision, 0); }
    virtual long EffGetNumMidiInputChannels() { return EffDispatch(effGetNumMidiInputChannels, 0, 0, 0); }
    virtual long EffGetNumMidiOutputChannels() { return EffDispatch(effGetNumMidiOutputChannels, 0, 0, 0); }

// overridables
public:
    virtual long OnGetUniqueId() { return nUniqueId; }
	virtual void * OnGetDirectory();
    virtual void OnSizeEditorWindow(long width, long height) { }
    virtual bool OnUpdateDisplay() { return false; }
    virtual void * OnOpenWindow(VstWindow* window) { return 0; }
    virtual bool OnCloseWindow(VstWindow* window) { return false; }
    virtual bool OnIoChanged() { return false; }
    virtual long OnGetNumAutomatableParameters() { return (pEffect) ? pEffect->numParams : 0; }
	virtual int	 GetInputNums() {return pEffect ? pEffect->numInputs : 0;}
	virtual int	 GetOutputNums() {return pEffect ? pEffect->numOutputs : 0;}

};
