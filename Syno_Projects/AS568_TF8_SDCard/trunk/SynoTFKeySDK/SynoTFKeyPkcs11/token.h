/***************************************************************************
 *	File name:token.h
 *	Introduce:Syno TF key token object functions definiton
 *	Author:Syno common
 *	Date:2015/11/04
 *  Version:v1.0
 **************************************************************************/
#ifndef _TOKEN_H_
#define _TOKEN_H_

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "internal.h"
#include "pkcs11t.h"

#include "SynoHeaders.h"
#include "SynoTFKeyP11.h"


#ifdef __cplusplus
extern "C" {
#endif
	
/*----------------------------------------------*
 * 初始化函数定义                               *
 *----------------------------------------------*/
CK_RV TOK_EnumToken(CK_CHAR_PTR pucTokenList, ULONG *pulSize);
CK_RV TOK_TokenInitialize(CK_CHAR_PTR token_name, CK_I_SLOT_DATA_PTR CK_PTR ppSlotData);

/*----------------------------------------------*
 * 管理函数定义                                 *
 *----------------------------------------------*/
CK_RV TOK_GetTokenInfo(CK_I_SLOT_DATA_PTR slot_data, CK_TOKEN_INFO_PTR pInfo);
CK_RV TOK_GetMechanismList(CK_MECHANISM_TYPE_PTR pMechanismList, CK_ULONG_PTR pulCount);
CK_RV TOK_GetMechanismInfo(CK_MECHANISM_TYPE type, CK_MECHANISM_INFO_PTR pInfo);
CK_RV TOK_InitToken(CK_I_SLOT_DATA_PTR slot_data, CK_CHAR_PTR pPin, CK_ULONG ulPinLen, CK_CHAR_PTR pLabel);
CK_RV TOK_FinalizeToken(CK_I_SLOT_DATA_PTR slot_data);
CK_RV TOK_InitPIN(CK_I_SESSION_DATA_PTR session_data, CK_CHAR_PTR pPin, CK_ULONG ulPinLen);
CK_RV TOK_SetPIN(CK_I_SESSION_DATA_PTR session_data, CK_CHAR_PTR pOldPin, CK_ULONG ulOldLen, CK_CHAR_PTR pNewPin, CK_ULONG ulNewLen);
CK_RV TOK_OpenSession(CK_I_SESSION_DATA_PTR session_data);
CK_RV TOK_CloseSession(CK_I_SESSION_DATA_PTR session_data);
CK_RV TOK_GetOperationState(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pOperationState, CK_ULONG_PTR pulOperationStateLen);
CK_RV TOK_SetOperationState(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pOperationState, CK_ULONG ulOperationStateLen, CK_I_OBJ_PTR encrypt_key_obj, CK_I_OBJ_PTR auth_key_obj);
CK_RV TOK_Login(CK_I_SESSION_DATA_PTR session_data, CK_USER_TYPE userType, CK_CHAR_PTR pPin, CK_ULONG ulPinLen);
CK_RV TOK_Logout(CK_I_SESSION_DATA_PTR session_data);

/*----------------------------------------------*
 * 加密函数定义                                 *
 *----------------------------------------------*/
CK_RV TOK_EncryptInit(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR key_obj);
CK_RV TOK_Encrypt(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pEncryptedData, CK_ULONG_PTR pulEncryptedDataLen);
CK_RV TOK_EncryptUpdate(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen);
CK_RV TOK_EncryptFinal(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pLastEncryptedPart, CK_ULONG_PTR pulLastEncryptedPartLen);

/*----------------------------------------------*
 * 解密函数定义                                  *
 *----------------------------------------------*/
CK_RV TOK_DecryptInit(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR key_obj);
CK_RV TOK_Decrypt(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pEncryptedData, CK_ULONG ulEncryptedDataLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen);
CK_RV TOK_DecryptUpdate(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen);
CK_RV TOK_DecryptFinal(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pLastPart, CK_ULONG_PTR pulLastPartLen);

/*----------------------------------------------*
 * 杂凑函数定义                                  *
 *----------------------------------------------*/
CK_RV TOK_DigestInit(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism);
CK_RV TOK_Digest(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen);
CK_RV TOK_DigestUpdate(CK_I_SESSION_DATA_PTR session_data, CK_C_BYTE_PTR pPart, CK_ULONG ulPartLen);
CK_RV TOK_DigestKey(CK_I_SESSION_DATA_PTR  session_data, CK_I_OBJ_PTR key_obj);
CK_RV TOK_DigestFinal(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pDigest, CK_ULONG_PTR pulDigestLen);

/*----------------------------------------------*
 * 签名函数定义                                  *
 *----------------------------------------------*/
CK_RV TOK_SignInit(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR key_obj);
CK_RV TOK_Sign(CK_I_SESSION_DATA_PTR pstSessionData, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);
CK_RV TOK_SignUpdate(CK_I_SESSION_DATA_PTR pstSessionData, CK_BYTE_PTR pPart, CK_ULONG ulPartLen);
CK_RV TOK_SignFinal(CK_I_SESSION_DATA_PTR pstSessionData, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);
CK_RV TOK_SignRecoverInit(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR key_obj);
CK_RV TOK_SignRecover(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG_PTR pulSignatureLen);

/*----------------------------------------------*
 * 验签函数定义                                  *
 *----------------------------------------------*/
CK_RV TOK_VerifyInit(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR key_obj);
CK_RV TOK_Verify(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pData, CK_ULONG ulDataLen, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen);
CK_RV TOK_VerifyUpdate(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pPart, CK_ULONG ulPartLen);
CK_RV TOK_VerifyFinal(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen);
CK_RV TOK_VerifyRecoverInit(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR key_obj);
CK_RV TOK_VerifyRecover(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pSignature, CK_ULONG ulSignatureLen, CK_BYTE_PTR pData, CK_ULONG_PTR pulDataLen);

/*----------------------------------------------*
 * 双加密函数定义                                 *
 *----------------------------------------------*/
CK_RV TOK_DigestEncryptUpdate(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen);
CK_RV TOK_DecryptDigestUpdate(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen);
CK_RV TOK_SignEncryptUpdate(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pPart, CK_ULONG ulPartLen, CK_BYTE_PTR pEncryptedPart, CK_ULONG_PTR pulEncryptedPartLen);
CK_RV TOK_DecryptVerifyUpdate(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pEncryptedPart, CK_ULONG ulEncryptedPartLen, CK_BYTE_PTR pPart, CK_ULONG_PTR pulPartLen);

/*----------------------------------------------*
 * 密钥管理函数定义                               *
 *----------------------------------------------*/
CK_RV TOK_GenerateKey(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR key);
CK_RV TOK_GenerateKeyPair(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR public_key, CK_I_OBJ_PTR private_key);
CK_RV TOK_WrapKey(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR wrap_key_obj, CK_I_OBJ_PTR key_obj, BYTE* pWrappedKey, CK_RV* pulWrappedKeyLen);
CK_RV TOK_UnwrapKey(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR unwrap_key_obj, BYTE* pWrappedKey, ULONG ulWrappedKeyLen, CK_I_OBJ_PTR key_obj);
CK_RV TOK_DeriveKey(CK_I_SESSION_DATA_PTR session_data, CK_MECHANISM_PTR pMechanism, CK_I_OBJ_PTR base_key, CK_I_OBJ_PTR derived_ky);
CK_RV TOK_SeedRandom(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pSeed, CK_ULONG ulSeedLen);
CK_RV TOK_GenerateRandom(CK_I_SESSION_DATA_PTR session_data, CK_BYTE_PTR pRandomData, CK_ULONG ulRandomLen);

/*----------------------------------------------*
 * 对象函数定义                                  *
 *----------------------------------------------*/
CK_RV TOK_GetFunctionStatus(CK_I_SESSION_DATA_PTR session_data);
CK_RV TOK_CancelFunction(CK_I_SESSION_DATA_PTR session_data);
CK_RV TOK_WaitForSlotEvent(CK_FLAGS flags, CK_SLOT_ID_PTR pSlot, CK_VOID_PTR pRserved);
CK_RV TOK_TokenObjRetrieve(CK_I_SESSION_DATA_PTR session_data, CK_OBJECT_HANDLE phObject, CK_I_OBJ_PTR CK_PTR ppNewObject);
CK_RV TOK_TokenObjCommit(CK_I_SESSION_DATA_PTR session_data, CK_OBJECT_HANDLE hObject, CK_I_OBJ_PTR pObject);
CK_RV TOK_TokenObjAdd(CK_I_SESSION_DATA_PTR session_data, CK_OBJECT_HANDLE phObject, CK_I_OBJ_PTR pNewObject);
CK_RV TOK_TokenObjDelete(CK_I_SESSION_DATA_PTR session_data, CK_OBJECT_HANDLE phObject);


#ifdef __cplusplus
}
#endif

#endif /* _TOKEN_H_ */

