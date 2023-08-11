

static char RCSID[]="$Id: encrypt.c,v 1.4 2000/01/31 18:09:02 lbe Exp $";
const char* Version_encrypt_c(){return RCSID;}

#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include "internal.h"
#include "error.h"
#include "objects.h"
#include "token.h"

/* {{{ C_EncryptInit */
CK_RV C_EncryptInit(
    CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_OBJECT_HANDLE hKey
)
{
    CK_RV rv = CKR_OK;
    CK_I_OBJ_PTR key_obj = NULL_PTR;  /* key to be used */
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
    CK_BYTE_PTR tmp = NULL_PTR;

    CI_VarLogEntry("C_EncryptInit", "starting... Session: %i, Key: %i", rv, 1,
        hSession,hKey);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
    {
        rv = CKR_CRYPTOKI_NOT_INITIALIZED;
        CI_LogEntry("C_EncryptInit", "library initialization", rv, 1);
        return rv;
    }

    /* get session info and make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_EncryptInit", "retrieve session data (hSession: %lu)", rv, 1,
             hSession);
        return rv;
    }

    /* Is there an active encryption? */
    if(session_data->encrypt_state != NULL_PTR)
    {
        rv = CKR_OPERATION_ACTIVE;
        CI_LogEntry("C_EncryptInit", "check for active encryption", rv, 1);
        return rv;
    }


    rv = CI_ReturnObj(session_data,hKey, &key_obj);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_EncryptInit", "retrieve object (hSession: %lu, hKey: %lu)", rv, 1,
            hSession, hKey);
        return rv;
    }

    /* Does the key allow encryption? */
    /*
    * no need to check if mechanism allows encryption.
    * Only those that allow encryption are implemented below
    */
    if((CI_ObjLookup(key_obj,CK_IA_ENCRYPT) == NULL_PTR) ||
        (*((CK_BBOOL CK_PTR)CI_ObjLookup(key_obj,CK_IA_ENCRYPT)->pValue) != TRUE))
    {
        rv = CKR_KEY_TYPE_INCONSISTENT;
        CI_LogEntry("C_EncryptInit", "test key encrypt attribute", rv, 1);
        return rv;
    }
	
    /* All Checked. Set encryption object */
	CK_I_CALL_TOKEN_METHOD(rv, EncryptInit, (session_data, pMechanism, key_obj));

    CI_LogEntry("C_EncryptInit", "...complete", rv, 1);

    return rv;
}
/* }}} */
/* {{{ C_Encrypt */
CK_RV C_Encrypt(
        CK_SESSION_HANDLE hSession,            /* session's handle */
        CK_BYTE_PTR       pData,               /* the plaintext data */
        CK_ULONG          ulDataLen,           /* bytes of plaintext */
        CK_BYTE_PTR       pEncryptedData,      /* gets ciphertext */
        CK_ULONG_PTR      pulEncryptedDataLen  /* gets c-text size */
      )
{
    CK_RV rv = CKR_OK;
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
    CK_BYTE_PTR tmp = NULL_PTR;

    CI_VarLogEntry("C_Encrypt", "starting... Session: %i", rv, 1, hSession);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
        return CKR_CRYPTOKI_NOT_INITIALIZED;

    /* get session info and make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_Encrypt", "retrieve session data (hSession: %lu)", rv, 1,
             hSession);
        return rv;
    }

    /* Is there an active encryption at all? */
    if(session_data->encrypt_state == NULL_PTR)
    {
        rv = CKR_OPERATION_NOT_INITIALIZED;
        CI_LogEntry("C_Encrypt", "check operation status", rv, 1);
        return rv;
    }

    CK_I_CALL_TOKEN_METHOD(rv, Encrypt, (session_data, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen));

    CI_LogEntry("C_Encrypt", "...complete", rv, 1);

    return rv;
}
/* }}} */
/* {{{ C_EncryptUpdate */
CK_RV C_EncryptUpdate(
    CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pPart,
    CK_ULONG ulPartLen,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG_PTR pulEncryptedPartLen
)
{
    CK_RV rv = CKR_OK;
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
    CK_BYTE_PTR tmp = NULL_PTR;

    CI_VarLogEntry("C_EncryptUpdate", "starting... Session: %i", rv, 1, hSession);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
        return CKR_CRYPTOKI_NOT_INITIALIZED;

    /* get session info and make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_EncryptUpdate", "retrieve session data (hSession: %lu)", rv, 1,
             hSession);
        return rv;
    }

    /* Is there an active encryption at all? */
    if(session_data->encrypt_state == NULL_PTR)
    {
        rv = CKR_OPERATION_NOT_INITIALIZED;
        CI_LogEntry("C_EncryptUpdate", "check operation status", rv, 1);
        return rv;
    }

    //rv = TOK_EncryptUpdate(session_data, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen);
	CK_I_CALL_TOKEN_METHOD(rv, EncryptUpdate, (session_data, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen));

    CI_LogEntry("C_EncryptUpdate", "...complete", rv, 1);

    return rv;
}
/* }}} */
/* {{{ C_EncryptFinal */
CK_RV C_EncryptFinal(
    CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pLastEncryptedPart,
    CK_ULONG_PTR pulLastEncryptedPartLen
)
{
    CK_RV rv =CKR_OK;
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;

    CI_VarLogEntry("C_EncryptFinal", "starting... Session: %i", rv, 1, hSession);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
        return CKR_CRYPTOKI_NOT_INITIALIZED;

    /* get session info and make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_EncryptFinal", "retrieve session data (hSession: %lu)", rv, 1,
             hSession);
        return rv;
    }

    /* Is there an active encryption at all? */
    if(session_data->encrypt_state == NULL_PTR)
    {
        rv = CKR_OPERATION_NOT_INITIALIZED;
        CI_LogEntry("C_EncryptFinal", "check operation status", rv, 1);
        return rv;
    }

    //rv = TOK_EncryptFinal(session_data, pLastEncryptedPart, pulLastEncryptedPartLen);
	CK_I_CALL_TOKEN_METHOD(rv, EncryptFinal, (session_data, pLastEncryptedPart, pulLastEncryptedPartLen));

    CI_LogEntry("C_EncryptFinal", "...complete", rv, 1);

    return rv;
}
/* }}} */
/*
 * Local variables:
 * folded-file: t
 * end:
 */

