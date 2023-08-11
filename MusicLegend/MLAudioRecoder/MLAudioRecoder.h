// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MLAUDIORECODER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MLAUDIORECODER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MLAUDIORECODER_EXPORTS
#define MLAUDIORECODER_API __declspec(dllexport)
#else
#define MLAUDIORECODER_API __declspec(dllimport)
#endif

// This class is exported from the MLAudioRecoder.dll
class MLAUDIORECODER_API CMLAudioRecoder {
public:
	CMLAudioRecoder(void);
	// TODO: add your methods here.
};

extern MLAUDIORECODER_API int nMLAudioRecoder;

MLAUDIORECODER_API int fnMLAudioRecoder(void);
