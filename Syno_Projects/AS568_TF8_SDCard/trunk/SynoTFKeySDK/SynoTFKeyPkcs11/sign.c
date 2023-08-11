
 
static char RCSID[]="$Id: sign.c,v 1.5 2000/09/19 09:14:55 lbe Exp $";
const char* Version_sign_c(){return RCSID;}

#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include "internal.h"
#include "error.h"
#include "objects.h"

#include <stdlib.h>

/* {{{ C_SignInit */
CK_RV C_SignInit(
        CK_SESSION_HANDLE hSession,
        CK_MECHANISM_PTR pMechanism,
        CK_OBJECT_HANDLE hKey
      )
{
  CK_RV rv = CKR_OK;
  CK_I_OBJ_PTR key_obj = NULL_PTR;  /* key to be used */
  CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
  CK_BYTE_PTR tmp = NULL_PTR;
  
  CI_LogEntry("C_SignInit", "starting...",rv,1);
  CI_CodeFktEntry("C_SignInit", "%i,%s,%i", 
		  hSession,
		  tmp = CI_ScanableMechanism(pMechanism),
		  hKey);
  TC_free(tmp);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    {
      rv = CKR_CRYPTOKI_NOT_INITIALIZED;
      CI_LogEntry("C_SignInit", "library status", rv, 1);
      return rv;
    }

  /* get session info and make sure that this session exists */
   rv = CI_ReturnSession(hSession, &session_data);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_SignInit", "retrieve session data (hSession: %lu)", rv, 1,
                     hSession);
      return rv;
    }
 
  /* Is there an active signing? */
  if(session_data->sign_state != NULL_PTR)
    {
      rv = CKR_OPERATION_ACTIVE;
      CI_LogEntry("C_SignInit", "check for active session", rv, 1);
      return rv;
    }

  rv = CI_ReturnObj(session_data,hKey, &key_obj);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_SignInit", "retrieve object list (hSession: %lu, hKey: %lu)", rv, 1,
                     hSession, hKey);
      return rv;
    }
 
  /* Does the key allow signing? */
  /*
   * no need to check if mechanism allows signing. 
   * Only those that allow signing are implemented below 
   * TODO: check wether the default is sign or no sign
   */
  if((CI_ObjLookup(key_obj,CK_IA_SIGN) != NULL_PTR) && 
     (*((CK_BBOOL CK_PTR)CI_ObjLookup(key_obj,CK_IA_SIGN)->pValue) != TRUE))
    {
      rv = CKR_KEY_TYPE_INCONSISTENT;
      CI_LogEntry("C_SignInit", "checking sign flag", rv, 1);
      return rv;
    }
  
  CK_I_CALL_TOKEN_METHOD(rv, SignInit, (session_data, pMechanism, key_obj));
  
  CI_LogEntry("C_SignInit", "...complete", rv, 1);

  return rv;
}
/* }}} */
/* {{{ C_Sign */
CK_RV C_Sign(
        CK_SESSION_HANDLE hSession,            /* session's handle */
        CK_BYTE_PTR       pData,               /* gets plaintext */
        CK_ULONG          ulDataLen,           /* gets p-text size */
        CK_BYTE_PTR       pSignature,          /* the Signature */
        CK_ULONG_PTR      pulSignatureLen      /* bytes of Signature */
      )
{
  CK_RV rv = CKR_OK;
  CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
  CK_BYTE_PTR tmp = NULL_PTR;
  
  CI_VarLogEntry("C_Sign", "starting... Session: %i", rv, 1,hSession);
  CI_CodeFktEntry("C_Sign", "%i,%s,%i,%p,%p", 
		  hSession,
		  tmp = CI_ScanableByteStream(pData, ulDataLen),
		  ulDataLen,
		  pSignature,
		  pulSignatureLen);
  TC_free(tmp);
  if(pSignature != NULL_PTR)
    CI_VarLogEntry("C_Sign", "*pulSignatureLen: %i", rv, 1, *pulSignatureLen);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  /* get session info and make sure that this session exists */
  rv = CI_ReturnSession(hSession, &session_data);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_Sign", "retrieve session data (hSession: %lu)", rv, 1,
                     hSession);
      return rv;
    }

  /* Is there an active signing at all? */
  if(session_data->sign_state == NULL_PTR)
    {
      rv = CKR_OPERATION_NOT_INITIALIZED;
      CI_LogEntry("C_Sign", "checking operation status", rv, 0);
      return rv;
    }
  
  CK_I_CALL_TOKEN_METHOD(rv, Sign, (session_data, pData, ulDataLen, pSignature, pulSignatureLen));

  CI_LogEntry("C_Sign", "...complete", rv, 1);

  return rv;
}
/* }}} */
/* {{{ C_SignUpdate */
CK_RV C_SignUpdate(
        CK_SESSION_HANDLE hSession,  /* the session's handle */
        CK_BYTE_PTR pPart,           /* the data to sign */
        CK_ULONG ulPartLen           /* count of bytes to sign */
      )
{
  CK_RV rv = CKR_OK;
  CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
  CK_BYTE_PTR tmp = NULL_PTR;

  CI_LogEntry("C_SignUpdate", "starting...", rv, 1);
  CI_CodeFktEntry("C_SignUpdate", "%i,%s,%i", 
		  hSession,
		  tmp = CI_ScanableByteStream(pPart, ulPartLen),
		  ulPartLen);
  TC_free(tmp);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  /* get session info and make sure that this session exists */
  rv = CI_ReturnSession(hSession, &session_data);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_SignUpdate", "retrieve session data (hSession: %lu)", rv, 1,
                     hSession);
      return rv;
    }

  /* Is there an active signing at all? */
  if(session_data->sign_state == NULL_PTR)
    {
      rv = CKR_OPERATION_NOT_INITIALIZED;
      CI_LogEntry("C_SignUpdate", "checking operation status", rv, 0); 
      return rv;
    }

  CK_I_CALL_TOKEN_METHOD(rv, SignUpdate, (session_data, pPart, ulPartLen));

  CI_LogEntry("C_SignUpdate", "...complete", rv, 1);

  return rv;
}
/* }}} */
/* {{{ C_SignFinal */
CK_RV C_SignFinal(
        CK_SESSION_HANDLE hSession,
        CK_BYTE_PTR pSignature,
        CK_ULONG_PTR pulSignatureLen
      )
{
  CK_RV rv = CKR_OK;
  CK_I_SESSION_DATA_PTR session_data = NULL_PTR;

  CI_LogEntry("C_SignFinal", "starting...", rv, 1);
  CI_CodeFktEntry("C_SignFinal", "%i,%p,%p", 
		  hSession,
		  pSignature,
		  pulSignatureLen);
  CI_VarLogEntry("C_SignFinal", "*pulSignatureLen: %i", rv, 1, *pulSignatureLen);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  /* get session info and make sure that this session exists */
  rv = CI_ReturnSession(hSession, &session_data);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_SignFinal", "retrieve session data (hSession: %lu)", rv, 1,
                     hSession);
      return rv;
    }
 
  CI_VarLogEntry("C_SignFinal", "using object with mem-handle %x", rv, 1,
		 session_data);

  /* Is there an active signing at all? */
  if(session_data->sign_state == NULL_PTR)
    {
      rv = CKR_OPERATION_NOT_INITIALIZED;
      CI_LogEntry("C_SignFinal", "check operation status", rv, 1);
      return rv;
    }

  CK_I_CALL_TOKEN_METHOD(rv, SignFinal, (session_data, pSignature, pulSignatureLen));

  if(session_data->sign_state != NULL_PTR)
    CI_LogEntry("C_SignFinal", "sign state not cleared!", rv, 1);

  CI_LogEntry("C_SignFinal", "...complete", rv, 1);

  return rv;
}
/* }}} */
/* {{{ C_SignRecoverInit */
/* C_SignRecoverInit initializes a signature operation, where
 * the data can be recovered from the signature. */
CK_RV C_SignRecoverInit(
  CK_SESSION_HANDLE hSession,   /* the session's handle */
  CK_MECHANISM_PTR  pMechanism, /* the signature mechanism */
  CK_OBJECT_HANDLE  hKey        /* handle of the signature key */
)
{
  CK_RV rv = CKR_OK;
  CK_I_OBJ_PTR key_obj = NULL_PTR;  /* key to be used */
  CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
  CK_BYTE_PTR tmp = NULL_PTR;

  CI_LogEntry("C_SignRecoverInit", "starting", rv, 1);
  CI_CodeFktEntry("C_SignRecoverInit", "%i,%s,%i", 
		  hSession,
		  tmp = CI_ScanableMechanism(pMechanism),
		  hKey);
  TC_free(tmp);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  /* get session info and make sure that this session exists */
  rv = CI_ReturnSession(hSession, &session_data);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_SignRecoverInit", "retrieve session data (hSession: %lu)", rv, 1,
                     hSession);
      return rv;
    }
 
  /* Is there an active signing? */
  if(session_data->sign_state != NULL_PTR)
    {
      rv = CKR_OPERATION_ACTIVE;
      CI_LogEntry("C_SignRecoverInit", "checking operation status", rv, 0);
      return rv;
    }

  rv = CI_ReturnObj(session_data,hKey, &key_obj);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_SignRecoverInit", "retrieve object list (hSession: %lu, hKey: %lu)", rv, 1,
                     hSession, hKey);
      return rv;
    }
  
 
  /* Does the key allow sign-recovery? */
  /*
   * no need to check if mechanism allows signing. 
   * Only those that allow signing are implemented below 
   * TODO: check the default for flag
   */
  if((CI_ObjLookup(key_obj,CK_IA_SIGN_RECOVER) != NULL_PTR) &&
     (*((CK_BBOOL CK_PTR)CI_ObjLookup(key_obj,CK_IA_SIGN_RECOVER)->pValue) != TRUE))
    {
      rv = CKR_KEY_TYPE_INCONSISTENT;
      CI_LogEntry("C_SignRecoverInit", "checking sign-recover flag", rv, 1);
      return rv;
    }
  
  CK_I_CALL_TOKEN_METHOD(rv, SignRecoverInit, (session_data, pMechanism, key_obj));
  
  CI_LogEntry("C_SignRecoverInit", "...complete", rv, 1); 

  return rv;
}
/* }}} */
/* {{{ C_SignRecover */
/* C_SignRecover signs data in a single operation, where the
 * data can be recovered from the signature. */
CK_RV C_SignRecover(
  CK_SESSION_HANDLE hSession,        /* the session's handle */
  CK_BYTE_PTR       pData,           /* the data to sign */
  CK_ULONG          ulDataLen,       /* count of bytes to sign */
  CK_BYTE_PTR       pSignature,      /* gets the signature */
  CK_ULONG_PTR      pulSignatureLen  /* gets signature length */
)
{
  CK_RV rv = CKR_OK;
  CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
  CK_BYTE_PTR tmp = NULL_PTR;

  CI_LogEntry("C_SignRecover", "starting...", rv, 1);
  CI_CodeFktEntry("C_SignRecover", "%i,%s,%i,%p,%p", 
		  hSession,
		  tmp = CI_ScanableByteStream(pData, ulDataLen),
		  ulDataLen,
		  pSignature,
		  pulSignatureLen);
  CI_VarLogEntry("C_SignRecover", "*pulSignatureLen: %i", rv, 1, *pulSignatureLen);
  TC_free(tmp);
  
  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  /* get session info and make sure that this session exists */
  rv = CI_ReturnSession(hSession, &session_data);
  if(rv != CKR_OK)
    {
      CI_VarLogEntry("C_SignRecover", "retrieve session data (hSession: %lu)", rv, 1,
                     hSession);
      return rv;
    }
 
  /* Is there an active signing at all? */
  if(session_data->sign_state == NULL_PTR)
    {
      rv = CKR_OPERATION_NOT_INITIALIZED;
      CI_LogEntry("C_SignRecover", "checking operation status", rv, 1);
      return rv;
    }

  CK_I_CALL_TOKEN_METHOD(rv, SignRecover, (session_data, pData, ulDataLen, pSignature, pulSignatureLen));

  CI_LogEntry("C_SignRecover", "complete", rv, 1);

  return rv;
}
/* }}} */

/*
 * Local variables:
 * folded-file: t
 * end:
 */
