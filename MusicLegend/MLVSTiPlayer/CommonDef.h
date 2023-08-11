//--------------------------------------------------
//	File Name:CommonDef.h
//	Introduce:Common definition for VSTi Player
//	Date:2010-08-29
//--------------------------------------------------
#ifndef	_COMMON_DEF_H_
#define _COMMON_DEF_H_
//--------------------------------------------------
#define VST_FORCE_DEPRECATED 0 
//--------------------------------------------------
#include "audioeffectx.h"  
//--------------------------------------------------
#define	VSTI_BUFF_SIZE		4410
#define	VSTI_SAMPLE_RATE	44100
#define	VSTI_MAX_EVENTCNT	512

//	Audio master callback function definition
typedef long(VSTCALLBACK *AUDIOMASTERCALLBACKFUNC)(AEffect *effect, long opcode, long index, long value, void *ptr, float opt);
//--------------------------------------------------
//--------------------------------------------------
#endif	//_COMMON_DEF_H_
//--------------------------------------------------