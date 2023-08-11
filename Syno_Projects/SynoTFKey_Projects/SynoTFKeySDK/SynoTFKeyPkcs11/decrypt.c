

static char RCSID[]="$Id: decrypt.c,v 1.5 2000/03/08 09:59:07 lbe Exp $";
const char* Version_decrypt_c(){return RCSID;}

#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include "internal.h"
#include "error.h"
#include "objects.h"
#include "ica_token.h"

#include <stdlib.h>

/* {{{ C_DecryptInit */
CK_RV C_DecryptInit(
    CK_SESSION_HANDLE hSession,
    CK_MECHANISM_PTR pMechanism,
    CK_OBJECT_HANDLE hKey
)
{
    CK_RV rv = CKR_OK;
    CK_I_OBJ_PTR key_obj = NULL_PTR;  /* key to be used */
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;

    CI_VarLogEntry("C_DecryptInit", "starting...Session: %i", rv, 0, hSession);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
        return CKR_CRYPTOKI_NOT_INITIALIZED;

    /* get session info and make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_DecryptInit", "retrieve session data (hSession: %lu)", rv, 1,
             hSession);
        return rv;
    }

    CI_LogEntry("C_DecryptInit", "get here 1", rv, 0);

    /* Is there an active decryption? */
    if(session_data->decrypt_state != NULL_PTR)
    {
        rv = CKR_OPERATION_ACTIVE;
        CI_LogEntry("C_DecryptInit", "check operation status", rv , 0);
        return rv;
    }

    CI_LogEntry("C_DecryptInit", "get here 2", rv, 0);

    rv = CI_ReturnObj(session_data,hKey, &key_obj);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_DecryptInit", "retrieve object (hSession: %lu, hKey: %lu)", rv, 1,
             hSession, hKey);
        return rv;
    }

    CI_LogEntry("C_DecryptInit", "get here 3", rv, 0);

    /* Does the key allow decryption? */
    /*
    * no need to check if mechanism allows decryption.
    * Only those that allow decryption are implemented below
    */
    if((CI_ObjLookup(key_obj,CK_IA_DECRYPT) == NULL_PTR)||
        (*((CK_BBOOL CK_PTR)CI_ObjLookup(key_obj,CK_IA_DECRYPT)->pValue) != TRUE))
    {
        rv = CKR_KEY_TYPE_INCONSISTENT;
        CI_LogEntry("C_DecryptInit", "ensure that key supports decrypt", rv , 0);
        return rv;
    }

    CI_LogEntry("C_DecryptInit", "get here 4", rv, 0);


    /* TODO: check read-only constraints */

    /* All Checked. Set decryption object */
    if (g_ucUseOpenssl)
    {
        CK_I_CALL_TOKEN_METHOD(rv, DecryptInit, (session_data, pMechanism, key_obj));
    }
    else
    {
        rv = ICA_TOK_DecryptInit(session_data, pMechanism, key_obj);
    }

    CI_LogEntry("C_DecryptInit", "...complete", rv , 1);

    return rv;
}
/* }}} */
/* {{{ C_Decrypt */
CK_RV C_Decrypt(
        CK_SESSION_HANDLE hSession,            /* session's handle */
        CK_BYTE_PTR       pEncryptedData,      /* gets ciphertext */
        CK_ULONG          ulEncryptedDataLen,  /* gets c-text size */
        CK_BYTE_PTR       pData,               /* the plaintext data */
        CK_ULONG_PTR      pulDataLen           /* bytes of plaintext */
      )
{
    CK_RV rv = CKR_OK;
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;

    CI_VarLogEntry("C_Decrypt", "starting... Session: %i", rv , 0, hSession);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
        return CKR_CRYPTOKI_NOT_INITIALIZED;

    /* get session info and make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_Decrypt", "retrieve session data (hSession: %lu)", rv, 1,
             hSession);
        return rv;
    }

    /* Is there an active decryption at all? */
    if(session_data->decrypt_state == NULL_PTR)
    {
        rv = CKR_OPERATION_NOT_INITIALIZED;
        CI_LogEntry("C_Decrypt", "check operation status", rv , 0);
        return rv;
    }

    CK_I_CALL_TOKEN_METHOD(rv, Decrypt, (session_data, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen));

    CI_LogEntry("C_Decrypt", "...complete", rv , 0);

    return rv;
}
/* }}} */
/* {{{ C_DecryptUpdate */
CK_RV C_DecryptUpdate(
    CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pEncryptedPart,
    CK_ULONG ulEncryptedPartLen,
    CK_BYTE_PTR pPart,
    CK_ULONG_PTR pulPartLen
)
{
    CK_RV rv = CKR_OK;
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;

    CI_VarLogEntry("C_DecryptUpdate", "starting... Session: %i", rv , 0, hSession);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
        return CKR_CRYPTOKI_NOT_INITIALIZED;

    /* get session info and make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_DecryptUpdate", "retrieve session data (hSession: %lu)", rv, 1,
             hSession);
        return rv;
    }

    /* Is there an active decryption at all? */
    if(session_data->decrypt_state == NULL_PTR)
    {
        rv = CKR_OPERATION_NOT_INITIALIZED;
        CI_LogEntry("C_DecryptUpdate", "check operation status", rv , 0);
        return rv;
    }

    if (g_ucUseOpenssl)
    {
        CK_I_CALL_TOKEN_METHOD(rv, DecryptUpdate, (session_data, pEncryptedPart,
            ulEncryptedPartLen, pPart, pulPartLen));
    }
    else
    {
        rv = ICA_TOK_DecryptUpdate(session_data, pEncryptedPart,
            ulEncryptedPartLen, pPart, pulPartLen);
    }

    CI_LogEntry("C_DecryptUpdate", "...complete", rv , 0);

    return rv;
}
/* }}} */
/* {{{ C_DecryptFinal */
CK_RV C_DecryptFinal(
    CK_SESSION_HANDLE hSession,
    CK_BYTE_PTR pLastPart,
    CK_ULONG_PTR pulLastPartLen
)
{
    CK_RV rv = CKR_OK;
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;

    CI_VarLogEntry("C_DecryptFinal", "starting... Session: %i", rv , 0, hSession);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
        return CKR_CRYPTOKI_NOT_INITIALIZED;

    /* get session info and make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if(rv != CKR_OK)
    {
        CI_VarLogEntry("C_DecryptFinal", "retrieve session data (hSession: %lu)", rv, 1,
             hSession);
        return rv;
    }

    /* Is there an active decryption at all? */
    if(session_data->decrypt_state == NULL_PTR)
    {
        rv = CKR_OPERATION_NOT_INITIALIZED;
        CI_LogEntry("C_DecryptFinal", "check operation status", rv , 0);
        return rv;
    }

    if (g_ucUseOpenssl)
    {
        CK_I_CALL_TOKEN_METHOD(rv, DecryptFinal, (session_data, pLastPart, pulLastPartLen));
    }
    else
    {
		rv = ICA_TOK_DecryptFinal(session_data, pLastPart, pulLastPartLen);
    }

    CI_LogEntry("C_DecryptFinal", "...complete", rv , 0);

    return rv;
}
/* }}} */
/*
 * Local variables:
 * folded-file: t
 * end:
 */
