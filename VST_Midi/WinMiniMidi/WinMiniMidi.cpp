// WinMiniMidi.cpp : Defines the entry point for the DLL application.
//
/*
#include "stdafx.h"
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

*/
/* 
**
**   WINMIDI version 1.0
**  A minimal VST instrument which acts as a wrapper for the Microsoft Wave-
**  table Synth.
**
**  Homepage: winmidi.sourceforge.net
**  Created by the Canton project. The source and executable are public
**  domain, and are provided without restrictions but equally without
**  guarantees of any sort.
**
*/

#include <windows.h>
#include <stdio.h>


/*
**
**  Include the header files from the VST SDK. These files are not freely
**  re-distributable, but can be obtained free of charge from Steinheiser, Inc.
** 
*/
#include "public.sdk/source/vst2.x/audioeffect.h"
#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "pluginterfaces/vst2.x/aeffect.h"
#include "pluginterfaces/vst2.x/aeffectx.h"



//#undef DEBUG 
#define DEBUG


#define CHORD_CHANNEL    11 

#if defined (__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define VST_EXPORT	__attribute__ ((visibility ("default")))
#else
    #define VST_EXPORT
#endif

class winmidiEffect;


class winmidiEffect: public AudioEffectX {
  public:
    winmidiEffect (audioMasterCallback);
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual VstInt32 processEvents (VstEvents* events);

    virtual bool getEffectName (char*);
    virtual bool getVendorString (char *);
    virtual bool getProductString (char *);
    virtual bool getProgramNameIndexed (VstInt32  category, VstInt32 index, char* text);
    virtual void open ();
    virtual void close ();
    VstInt32 getVstVersion ();
    VstInt32  getMidiProgramName (VstInt32 channel, MidiProgramName* midiProgramName);
    VstInt32  getCurrentMidiProgram (VstInt32 channel, MidiProgramName* currentProgram);
    VstInt32 getMidiProgramCategory (VstInt32 channel, MidiProgramCategory* category);
    bool hasMidiProgramsChanged (VstInt32 channel);
    bool getMidiKeyName (VstInt32 channel, MidiKeyName* keyName);

	virtual VstInt32 getProgram ();// { return curProgram; }					///< Return the index to the current program
	virtual void setProgram (VstInt32 program) ;//{ curProgram = program; }	///< Set the current program to \e program
	
	virtual void setProgramName (char* name) {}
	virtual void getProgramName (char* name) { *name = 0; }
	virtual VstInt32 canDo (char* text);
	
    char prodstr [128];
    HMIDIOUT h;
    int prog;
    FILE * x;
	int iEventCnt;
};


winmidiEffect::winmidiEffect (audioMasterCallback audioMaster) : AudioEffectX (audioMaster, midiOutGetNumDevs (), 0)
{
    isSynth (true);
    strcpy (prodstr, " - ");
    h = NULL;
    prog = -1;
    x = NULL;
    setUniqueID (CCONST ('c', 'n', 't', 'n'));
    setNumInputs (0) ;
    setNumOutputs (0);
    setInitialDelay (0);	
	iEventCnt = 0;
}


VstInt32 winmidiEffect::getProgram ()
{
    return prog;
}

void winmidiEffect::setProgram (VstInt32 program) 
{
    int i;
    if(  (h == NULL) || ( prog != program)  ) {
        if(h != NULL) {
            midiOutClose (h);
            h = NULL;
            prog = -1;
        }
        i = midiOutGetNumDevs ();
        if (program < i) {
            if (MMSYSERR_NOERROR   !=   midiOutOpen (&h, program, NULL, 0, CALLBACK_NULL) )   h = NULL;
            else prog = program;
        }
    }
}



VstInt32 winmidiEffect::getVstVersion ()
{
    return 2400;
}


bool winmidiEffect::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
    int i;
    i = midiOutGetNumDevs ();
    if (index < i) {
        MIDIOUTCAPS caps;
        if(  MMSYSERR_NOERROR   ==   midiOutGetDevCaps (index, &caps, sizeof(MIDIOUTCAPS))) {
            strcpy (text, caps.szPname);
            return true;
        }
    }  
    return false;
}
   
VstInt32  winmidiEffect::getMidiProgramName (VstInt32 channel, MidiProgramName* midiProgramName)
{
    midiProgramName->thisProgramIndex = 1;
    sprintf (midiProgramName->name, "Chan %d", channel);
    midiProgramName->midiProgram = 2;
    midiProgramName->midiBankMsb = 0;
    midiProgramName->midiBankLsb = 1;
    midiProgramName->reserved = 0;
    midiProgramName->parentCategoryIndex = -1;
    midiProgramName->flags = 0;
    return 1;
}

VstInt32  winmidiEffect::getCurrentMidiProgram (VstInt32 channel, MidiProgramName* currentProgram)
{
    return getMidiProgramName (channel, currentProgram);
}

VstInt32 winmidiEffect::getMidiProgramCategory (VstInt32 channel, MidiProgramCategory* category)
{
    category->thisCategoryIndex = 1;
    strcpy(category->name, "RR");
    category->parentCategoryIndex = -1;
    category->flags = 0;

	return 0;
}

VstInt32 winmidiEffect::canDo (char* text){
	if (!strcmp(text, "receiveVstEvents") ||
		!strcmp(text, "receiveVstMidiEvent"))
		return 1;
    return 0;
}

bool winmidiEffect::hasMidiProgramsChanged (VstInt32 channel)
{
     return false;
}

bool winmidiEffect::getMidiKeyName (VstInt32 channel, MidiKeyName* keyName)
{
    keyName->thisProgramIndex = 2;
    keyName->thisKeyNumber = 1;
    strcpy(keyName->keyName, "TT");
    keyName->reserved = 0;
    keyName->flags = 0;
    return true;
}

void winmidiEffect::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames) {}


#ifdef CHORD_CHANNEL
 #define CHORD_STRINGS 6 
 unsigned char guitar_barre_style [12] = {1,1,1,1,2,2,2,3,2,4,4,4};
   //  From C through B.


 char guitar_barre [2][4][CHORD_STRINGS] = {
                                            //  Majors 
                                        {{-1,12,16,19,24,28},  //  C#-style barre.
                                         { 0, 7,12,16,19,24},   //  E-style barre.
                                         { 0, 4, 7,12,16,24},   //  G chord.
                                         {-1, 0, 7,12,16,19}},  //  A-style barre.
                                             //  Minors 
                                        {{-1,12,19,24,27,31},  //  C#-style barre.
                                         { 0, 7,12,15,19,24},   //  E-style barre.
                                         { 0, 7,12,15,19,24},   //  G chord.
                                         {-1, 0, 7,12,15,19}}   //  A-style barre.
                            };   


 char g[6] = {0,4,7,12,16,24};


#endif


VstInt32 winmidiEffect::processEvents (VstEvents * v)
{
  int i;
#ifdef DEBUG
  DWORD y = GetTickCount();
#endif
  if (v->numEvents == 0)
	  return 0;

  iEventCnt += v->numEvents;

    for(i=0 ;   i < v->numEvents   ;  i++) {
        if(   v->events[i]->type == kVstMidiType  ) {

            VstMidiEvent * e;
            BYTE d[4];
            e = (VstMidiEvent *) v->events[i];
            {
                DWORD mm;
#ifdef CHORD_CHANNEL

                if (   (((e->midiData[0]) & 0xE0) == 0x80)     &&    (((e->midiData[0]) & 0x0F) == (CHORD_CHANNEL-1))) {
                    mm = 0;
                    mm |= ((e->midiData[0]) & 0x0F) | 0xB0;
                    mm &= 0xFF;
                    mm |= 123<<8;
                    mm &= 0xFFFF;
                    mm |= 0;
                    mm &= 0xFFFFFF;
                    midiOutShortMsg (h,mm);
                }

                if(   (((e->midiData[0]) & 0xF0) == 0x90)     &&    (((e->midiData[0]) & 0x0F) == (CHORD_CHANNEL-1))) {
                    int j;
                    unsigned char root;
                    unsigned char style;
                        unsigned char mode;
                        root = (e->midiData[1]) % 12;
                        mode = 0x01 & (e->midiData[1] / 12);
                        style = guitar_barre_style [root] - 1;
                        for(j = 0; j < CHORD_STRINGS ; j ++ ) {
                           mm = 0;
                           mm |= (e->midiData[0]);
                           mm &= 0xFF;
                           if (   guitar_barre [mode][style][j]  > 0  ) {
                               mm |= (0x07F & (  36 + root + guitar_barre[mode][style][j]     )) << 8;
                               mm &= 0xFFFF;
                               mm |= (e->midiData[2]) << 16;
                               mm &= 0xFFFFFF;
                               midiOutShortMsg (h,mm);
                           }
                        }
                    }
                    else 
                   #endif
                   {
                       mm = 0;
                       mm |= (e->midiData[0]);
                       mm &= 0xFF;
                       mm |= (e->midiData[1]) << 8;
                       mm &= 0xFFFF;
                       mm |= (e->midiData[2]) << 16;
                       mm &= 0xFFFFFF;
                       midiOutShortMsg (h,mm);
                   }
              }
        }
}

#ifdef DEBUG
if (x != NULL) {
   DWORD u;
   static DWORD last_y=0;
   u = y - last_y;
   fprintf (x, "Event count: %d %08X time=%d    delta=%d\n",  v->numEvents, h, y, u);
   last_y = y;
   for (i = 0; i < v->numEvents  ;  i++) {
       VstMidiEvent * e;
       e = (VstMidiEvent *) v->events[i];
       fprintf (x, "  D=%d, t=%d, f=%X, %d, %d, %X, %d, %d, %d\n", e->deltaFrames, e->type, e->flags, e->noteLength, e->noteOffset, e->midiData[0],e->midiData[1],e->midiData[2],e->midiData[3]);
   }
}
#endif

         return 0;
}

bool winmidiEffect::getEffectName (char * c)
{
     strcpy (c, "winmidi");
     return true;
}
bool winmidiEffect::getVendorString (char * c)
{
     strcpy (c, "Developed for Canton by Mike.");
     return true;
}

bool winmidiEffect::getProductString (char * c)
{
     c[0] = 'T';
     c[1] = 0;
     strcpy (c, "Using winmidi as a wrapper layer.");
     return true;
}

/*
bool winmidiEffect::getProgramNameIndexed (VstInt32  category, VstInt32 index, char* text)
{
     if(index == 0) {
              strcpy (text, prodstr);
              return true;
     }
     else return false;         
}
*/

void winmidiEffect::open ()
{ 
     int i = midiOutGetNumDevs ();
     if (i > 0) {
        int j = 0;
        MIDIOUTCAPS c;
        midiOutGetDevCaps (j, &c, sizeof (MIDIOUTCAPS));
        strcpy (prodstr, c.szPname);
        if (MMSYSERR_NOERROR   !=   midiOutOpen (&h, j, NULL, 0, CALLBACK_NULL) )   h = NULL;
        else prog = j;
     }
     else {
        strcpy (prodstr, " -- ");
        h = NULL;
     }
#ifdef DEBUG
     x = fopen ("c:\\log.txt", "w");
#endif
}

void winmidiEffect::close ()
{
     if (h != NULL) {
           midiOutClose (h);
           strcpy (prodstr, "YYY");
           h = NULL;
     }
     
#ifdef DEBUG
    if(x != NULL) {
         fclose (x);
         x = NULL;
    }
#endif
}




extern "C" {



__declspec(dllexport)
VST_EXPORT void * main (audioMasterCallback ac) {
           AEffect a;
           winmidiEffect * ae;
           ae = new winmidiEffect (ac);
           return ae->getAeffect ();
}

} // extern "C"



#if WIN32
void* hInstance;

extern "C" {
BOOL WINAPI DllMain (HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
	hInstance = hInst;
	return 1;
}
} // extern "C"
#endif

