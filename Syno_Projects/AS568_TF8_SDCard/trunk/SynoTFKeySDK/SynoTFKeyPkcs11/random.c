
 
static char RCSID[]="$Id: random.c,v 1.4 2000/01/31 18:09:03 lbe Exp $";
const char* Version_random_c(){return RCSID;}

#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include "internal.h"
#include "error.h"
#include "objects.h"

/* {{{ C_SeedRandom */
CK_RV C_SeedRandom(
        CK_SESSION_HANDLE hSession,
        CK_BYTE_PTR pSeed,
        CK_ULONG ulSeedLen
      )
{
  CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
  CK_RV rv = CKR_OK;

  CI_LogEntry("C_SeedRandom", "starting...", rv, 1);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  /* get session info and make sure that this session exists */
  rv = CI_ReturnSession(hSession, &session_data);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_SeedRandom", "retrieve session data (hSession: %lu)", rv, 1,
                     hSession);
      return rv;
    }

  CK_I_CALL_TOKEN_METHOD(rv, SeedRandom, (session_data, pSeed, ulSeedLen));

  CI_LogEntry("C_SeedRandom", "...complete", rv, 1);
  return rv; 
}
/* }}} */

/* {{{ C_GenerateRandom */
CK_RV C_GenerateRandom(
        CK_SESSION_HANDLE hSession,
        CK_BYTE_PTR pRandomData,
        CK_ULONG ulRandomLen
      )
{
  CK_RV rv = CKR_OK;
  CK_I_SESSION_DATA_PTR session_data = NULL_PTR;

  CI_LogEntry("C_GenerateRandom", "starting...", rv, 1);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  /* get session info and make sure that this session exists */
  rv = CI_ReturnSession(hSession, &session_data);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_GenerateRandom", "retrieve session data (hSession: %lu)", rv, 1,
                     hSession);
      return rv;
    }

  CK_I_CALL_TOKEN_METHOD(rv, GenerateRandom, (session_data, pRandomData, ulRandomLen));

  CI_LogEntry("C_GenerateRandom", "...complete", rv, 1);

  return rv;
}
/* }}} */
/*
 * Local variables:
 * folded-file: t
 * end:
 */


