

static char RCSID[]="$Id: mutex.c,v 1.4 2000/01/31 18:09:02 lbe Exp $";
const char* Version_mutex_c(){return RCSID;}

/* Needed for Win32-isms in cryptoki.h */
#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include "internal.h"
#include "error.h"
#include "mutex.h"

/* TODO: the mutexes are not working at all at the moment! Fix Me */

/******************************************************************
 *                  Unix Mutex                                    *
 ******************************************************************/
#ifdef CK_GENERIC

#if defined(HAVE_PTHREAD_H)
# include <pthread.h>
#else
# include <thread.h>
# include <synch.h>
#endif
#include <stdlib.h>

CK_RV I_CreateMutex(CK_VOID_PTR_PTR ppMutex)
{
#if defined(HAVE_PTHREAD_H)
    *ppMutex = TC_calloc(1,sizeof(pthread_mutex_t));
#else
    *ppMutex = TC_calloc(1,sizeof(mutex_t));
#endif

    if(*ppMutex == NULL_PTR) return CKR_HOST_MEMORY;

#if defined(HAVE_PTHREAD_H)
    pthread_mutex_init((pthread_mutex_t CK_PTR)*ppMutex, NULL_PTR);
#else
    mutex_init((mutex_t CK_PTR)*ppMutex, USYNC_THREAD, NULL_PTR);
#endif

    return CKR_OK;
}


CK_RV I_DestroyMutex(
  CK_VOID_PTR pMutex  /* pointer to mutex */
)
{
#if defined(HAVE_PTHREAD_H)
    pthread_mutex_destroy((pthread_mutex_t CK_PTR)pMutex);
#else
    mutex_destroy((mutex_t CK_PTR)pMutex);
#endif

    TC_free(pMutex);
    return CKR_OK;
}

CK_RV I_LockMutex(
  CK_VOID_PTR pMutex  /* pointer to mutex */
)
{
    CK_RV rv;
#if defined(HAVE_PTHREAD_H)
    rv = pthread_mutex_lock((pthread_mutex_t CK_PTR)pMutex);
    if(rv != 0) return CKR_MUTEX_BAD;
#else
    rv = mutex_lock((mutex_t CK_PTR)pMutex);
    if(rv != 0) return CKR_MUTEX_BAD;
#endif

  return CKR_OK;
}

CK_RV I_UnlockMutex(
  CK_VOID_PTR pMutex  /* pointer to mutex */
)
{
    CK_RV rv;
#if defined(HAVE_PTHREAD_H)
    rv = pthread_mutex_unlock((pthread_mutex_t CK_PTR)pMutex);
    if(rv != 0) return CKR_MUTEX_BAD;
#else
    rv = mutex_unlock((mutex_t CK_PTR)pMutex);
    if(rv != 0) return CKR_MUTEX_BAD;
#endif
    return CKR_OK;
}

#elif defined(CK_Win32)
/********************************************************
 *               Win32 Mutex                            *
 ********************************************************/
#include <windows.h>
#include <stdio.h>
static int depth = 0;

CK_RV I_CreateMutex(
  CK_VOID_PTR_PTR ppMutex  /* location to receive ptr to mutex */
)
{
#if 0
  DWORD err_code;
  CK_RV rv = CKR_OK;
  HANDLE CK_PTR mutex_field = NULL_PTR;

  if(depth++ >0)
    {
      FILE* log;
      log = fopen("C:\\DebugDll\\mutex.log","a");
      fprintf(log,"CreateMutex(Win32): depth:%i",depth);
      fclose(log);
    }

  mutex_field = TC_malloc(sizeof(int));
  if(!mutex_field)
    {
      rv = CKR_HOST_MEMORY;
      CI_LogEntry("I_CreateMutex" ,"mutex creation failed",
		  rv, 0);
      return rv;
    }

  *mutex_field = (CK_VOID_PTR)CreateMutex(NULL_PTR, FALSE, NULL_PTR);

  if(*mutex_field == 0)
    {
      err_code = GetLastError();
      if((err_code == ERROR_NOT_ENOUGH_MEMORY) || ( err_code == ERROR_OUTOFMEMORY ))
	rv = CKR_HOST_MEMORY;
      else
	rv = CKR_GENERAL_ERROR;

      CI_VarLogEntry("I_CreateMutex" ,"mutex creation: GetLastError(): %i",
		     rv, 0, err_code);
    }

  *ppMutex = mutex_field;

  return rv;
#else
  *ppMutex = NULL_PTR;
  return CKR_OK;
#endif
}


CK_RV I_DestroyMutex(
  CK_VOID_PTR pMutex  /* mutex itself */
)
{
#if 0
  DWORD err_code;

  CK_RV rv = CKR_OK;

if(--depth >0)
  {
    FILE* log;
    log = fopen("C:\\DebugDll\\mutex.log","a");
    fprintf(log,"DestroyMutex(Win32): depth:%i",depth);
    fclose(log);
  }

  CloseHandle(*((HANDLE CK_PTR)pMutex));
  err_code = GetLastError();

  if(err_code != ERROR_SUCCESS)
    switch(err_code)
      {
      case ERROR_INVALID_HANDLE:
	rv = CKR_MUTEX_BAD;
	break;
      case ERROR_NOT_ENOUGH_MEMORY:
      case ERROR_OUTOFMEMORY:
	rv = CKR_HOST_MEMORY;
	break;
      default:
	rv = CKR_GENERAL_ERROR;
      }

  TC_free(pMutex);

  if(rv != CKR_OK)
    CI_VarLogEntry("I_DestroyMutex" ,"mutex destruction: GetLastError(): %i",
		   rv, 1, err_code);

  return rv;
#else
  return CKR_OK;
#endif
}

CK_RV I_LockMutex(
  CK_VOID_PTR pMutex  /* pointer to mutex */
)
{
#if 0
  CK_RV rv =CKR_OK;
  DWORD err_code;

  if(WAIT_FAILED == WaitForSingleObject((HANDLE)pMutex, INFINITE))
    {
      err_code = GetLastError();
      switch(err_code)
	{
	case ERROR_INVALID_HANDLE:
	rv = CKR_OK;
	/*	  rv = CKR_MUTEX_BAD; */
	  break;
	case ERROR_NOT_ENOUGH_MEMORY:
	case ERROR_OUTOFMEMORY:
	  rv = CKR_HOST_MEMORY;
	  break;
	default:
	  rv = CKR_GENERAL_ERROR;
	}
    }

  if(rv != CKR_OK)
    CI_VarLogEntry("I_LockMutex" ,"mutex locking: GetLastError(): %i",
		   rv, 1, err_code);

  return rv;
#else
  return CKR_OK;
#endif
}

CK_RV I_UnlockMutex(
  CK_VOID_PTR pMutex  /* pointer to mutex */
)
{
#if 0
    DWORD err_code;

    CK_RV rv = CKR_OK;

  if(!ReleaseMutex((HANDLE)pMutex))
    {
      err_code = GetLastError();
      switch(err_code)
	{
	case ERROR_INVALID_HANDLE:
	rv = CKR_OK;
	/* rv = CKR_MUTEX_BAD; */
	  break;
	case ERROR_NOT_ENOUGH_MEMORY:
	case ERROR_OUTOFMEMORY:
	  rv = CKR_HOST_MEMORY;
	  break;
	default:
	  rv = CKR_GENERAL_ERROR;
	}
    }


  if(rv != CKR_OK)
    CI_VarLogEntry("I_UnlockMutex" ,"mutex unlocking: GetLastError(): %i",
		   rv, 1, err_code);

  return rv;
#else
  return CKR_OK;
#endif
}

#else /* ! Win32 */
#error diese Architectur sind keine Mutexes implementiert!
#endif

CK_RV CI_CreateMutex(
  CK_VOID_PTR_PTR ppMutex  /* location to receive ptr to mutex */
)
{
  return CK_I_ext_functions._CreateMutex(ppMutex);
}

CK_RV CI_DestroyMutex(
  CK_VOID_PTR pMutex  /* mutex itself */
)
{
  return CK_I_ext_functions._DestroyMutex(pMutex);
}

CK_RV CI_LockMutex(
  CK_VOID_PTR pMutex  /* ptr to mutex */
)
{
  return CK_I_ext_functions._LockMutex(pMutex);
}

CK_RV CI_UnlockMutex(
  CK_VOID_PTR pMutex  /* ptr to mutex */
)
{
  return CK_I_ext_functions._UnlockMutex(pMutex);
}


/*
 * Local variables:
 * folded-file: t
 * end:
 */





