#include "stdafx.h"
#include ".\effect.h"

/*****************************************************************************/
/* CEffect : constructor                                                     */
/*****************************************************************************/

CEffect::CEffect(AUDIOMATSERCALLBACKFUNC lpAudioMasterCallback)
{
m_lpAudioMasterCallback = lpAudioMasterCallback;
pEffect = NULL;
sName = NULL;
bEditOpen = false;
bNeedIdle = false;
bInEditIdle = false;
bWantMidi = false;
bInSetProgram = false;
nIndex = -1;
nUniqueId = 0;
pMasterEffect = NULL;

#ifdef WIN32

hModule = NULL;
sDir = NULL;

#elif MAC

// yet to be done

#endif
}

/*****************************************************************************/
/* ~CEffect : destructor                                                     */
/*****************************************************************************/

CEffect::~CEffect()
{
Unload();

#ifdef WIN32

#elif MAC

#endif
}

/*****************************************************************************/
/* Load : loads the effect module                                            */
/*****************************************************************************/

bool CEffect::Load(const char *name)
{
if (!m_lpAudioMasterCallback)           /* if VST Host undefined             */
  return false;                         /* return without action             */

#ifdef WIN32

#elif MAC 

#endif

Unload();                               /* make sure nothing else is loaded  */
                                        /* pointer to main function          */
AEffect *(*pMain)(long (*audioMaster)(AEffect *effect,
                                  long opcode,
                                  long index,
                                  long value,
                                  void *ptr,
                                  float opt)) = 0;

#ifdef WIN32

//__try
try
  {
  hModule = ::LoadLibrary(name);          /* try to load the DLL               */
  }
//__except(EXCEPTION_EXECUTE_HANDLER)
catch(...)
  {
  hModule = NULL;
  }
if (hModule)                            /* if there, get its main() function */
  {
  pMain = (AEffect * (*)(long (*)(AEffect *,long,long,long,void *,float)))
          ::GetProcAddress(hModule, "VSTPluginMain");
  if (!pMain)
    pMain = (AEffect * (*)(long (*)(AEffect *,long,long,long,void *,float)))
            ::GetProcAddress(hModule, "main");
  }

if (pMain)                              /* initialize effect                 */
  {
//  __try
  try
    {
    pEffect = pMain(m_lpAudioMasterCallback);
    }
//  __except(EXCEPTION_EXECUTE_HANDLER)
  catch(...)
    {
    pEffect = NULL;
    }
  }

#elif MAC

// yet to be done

if (pMain)                              /* initialize effect                 */
  pEffect = pMain(m_lpAudioMasterCallback);

#endif

                                        /* check for correctness             */
if (pEffect && (pEffect->magic != kEffectMagic))
  pEffect = NULL;

if (pEffect)
  {
  sName = new char[strlen(name) + 1];
  if (sName)
    strcpy(sName, name);
#ifdef WIN32
  const char *p = strrchr(name, '\\');
  if (p)
    {
    sDir = new char[p - name + 1];
    if (sDir)
      {
      memcpy(sDir, name, p - name);
      sDir[p - name] = '\0';
      }
    }
#elif MAC

// yet to be done
#endif

  }

#ifdef WIN32

#elif MAC 

#endif

return !!pEffect;
}

/*****************************************************************************/
/* Unload : unloads effect module                                            */
/*****************************************************************************/

bool CEffect::Unload()
{
//if (!pHost)                             /* if no VST Host there              */
//  return false;

EffClose();                             /* make sure it's closed             */
pEffect = NULL;                         /* and reset the pointer             */

#ifdef WIN32

if (hModule)                            /* if DLL instance available         */
  {
  ::FreeLibrary(hModule);               /* remove it.                        */
  hModule = NULL;                       /* and reset the handle              */
  }

if (sDir)                               /* reset module directory            */
  {
  delete[] sDir;
  sDir = NULL;
  }

#elif MAC

// yet to be done!

#endif

if (sName)                              /* reset module name                 */
  {
  delete[] sName;
  sName = NULL;
  }

return true;
}

/*****************************************************************************/
/* LoadBank : loads a .fxb file ... IF it's for this effect                  */
/*****************************************************************************/

bool CEffect::LoadBank(char *name)
{
//try
//  {
//  CFxBank fx(name);                     /* load the bank                     */
//  if (!fx.IsLoaded())                   /* if error loading                  */
//    throw (int)1;
//  }
//catch(...)                              /* if any error occured              */
//  {
//  return false;                         /* return NOT!!!                     */
//  }
//  
return true;                            /* pass back OK                      */
}

/*****************************************************************************/
/* SaveBank : saves current sound bank to a .fxb file                        */
/*****************************************************************************/

bool CEffect::SaveBank(char *name)
{
return false;                           /* return error for now              */
}

/*****************************************************************************/
/* OnGetDirectory : returns the plug's directory (char* on pc, FSSpec on mac)*/
/*****************************************************************************/

void * CEffect::OnGetDirectory()
{
#ifdef WIN32

return sDir;

#elif MAC

// yet to be done

#endif
}

/*****************************************************************************/
/* EffDispatch : calls an effect's dispatcher                                */
/*****************************************************************************/

long CEffect::EffDispatch
    (
    long opCode,
    long index,
    long value,
    void *ptr,
    float opt
    )
{
if (!pEffect)
  return 0;

return pEffect->dispatcher(pEffect, opCode, index, value, ptr, opt);
}

/*****************************************************************************/
/* EffProcess : calles an effect's process() function                        */
/*****************************************************************************/

void CEffect::EffProcess(float **inputs, float **outputs, long sampleframes)
{
if (!pEffect)
  return;

pEffect->process(pEffect, inputs, outputs, sampleframes);
}

/*****************************************************************************/
/* EffProcessReplacing : calls an effect's processReplacing() function       */
/*****************************************************************************/

void CEffect::EffProcessReplacing(float **inputs, float **outputs, long sampleframes)
{
if ((!pEffect) ||
    (!(pEffect->flags & effFlagsCanReplacing)))
  return;

pEffect->processReplacing(pEffect, inputs, outputs, sampleframes);

}

/*****************************************************************************/
/* EffProcessDoubleReplacing : calls an effect's processDoubleReplacing() f. */
/*****************************************************************************/

void CEffect::EffProcessDoubleReplacing(double **inputs, double **outputs, long sampleFrames)
{
if ((!pEffect) ||
    (!(pEffect->flags & effFlagsCanDoubleReplacing)))
  return;

#if defined(VST_2_4_EXTENSIONS)
pEffect->processDoubleReplacing(pEffect, inputs, outputs, sampleFrames);
#endif
}

/*****************************************************************************/
/* EffSetParameter : calls an effect's setParameter() function               */
/*****************************************************************************/

void CEffect::EffSetParameter(long index, float parameter)
{
if (!pEffect)
  return;

pEffect->setParameter(pEffect, index, parameter);
}

/*****************************************************************************/
/* EffGetParameter : calls an effect's getParameter() function               */
/*****************************************************************************/

float CEffect::EffGetParameter(long index)
{
if (!pEffect)
  return 0.;

float frc = pEffect->getParameter(pEffect, index);

return frc;
}