/******************************************************************************

                  ��Ȩ���� (C), 2013-2023, �����ƶ���Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : ica_token.h
  �� �� ��   : ����
  ��    ��   : ������
  ��������   : 2013��10��12��
  ����޸�   : 2013��10��12��
  ��������   :
  �����б�   :
  �޸���ʷ   :
  1.��    �� : 2013��10��12��
    ��    �� : ������
    �޸����� : �����ļ�

******************************************************************************/

#ifndef ICA_TOKEN_H
#define ICA_TOKEN_H

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "internal.h"
//#include "ica_types.h"
#include "common.h"
#include "pkcs11t.h"


#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/
ULONG ICA_TOK_EnumToken(CK_CHAR_PTR pucTokenList, ULONG *pulSize);

ULONG ICA_TOK_TokenInitialize(CK_CHAR_PTR token_name,
    CK_I_SLOT_DATA_PTR CK_PTR ppSlotData);

ULONG ICA_TOK_FinalizeToken(CK_I_SLOT_DATA_PTR pSlotData);

ULONG ICA_TOK_InitToken(CK_I_SLOT_DATA_PTR SlotData,
    UCHAR *pucPin, ULONG ulPinLen, UCHAR *pucLabel);

ULONG ICA_TOK_OpenSession(CK_I_SESSION_DATA_PTR pstSessionData);

ULONG ICA_TOK_Login(CK_I_SESSION_DATA_PTR pstSessionData,
    ULONG ulUserType, UCHAR *pucPin, ULONG ulPinLen);

ULONG ICA_TOK_InitPIN(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucPin, ULONG ulPinLen);

ULONG ICA_TOK_SetPIN(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucOldPin, ULONG ulOldLen, UCHAR *pucNewPin, ULONG ulNewLen);

ULONG ICA_TOK_Logout(CK_I_SESSION_DATA_PTR pstSessionData);

ULONG ICA_TOK_CreateObject(CK_I_SESSION_DATA_PTR pstSessionData,
	 CK_I_OBJ_PTR pstObj);

ULONG ICA_TOK_GenerateKey(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR pstMechanism, CK_I_OBJ_PTR pstKeyObj);

ULONG ICA_TOK_GenerateKeyPair(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR pstMechanism,
    CK_I_OBJ_PTR pstPublicKeyObj, CK_I_OBJ_PTR pstPrivateKeyObj);

ULONG ICA_TOK_WrapKey(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR pstMechanism,
    CK_I_OBJ_PTR pstWrapKeyObj, CK_I_OBJ_PTR pstKeyObj,
    CK_BYTE_PTR pucWrappedKey, CK_ULONG_PTR pulWrappedKeyLen);

ULONG ICA_TOK_UnwrapKey(CK_I_SESSION_DATA_PTR  pstSessionData,
    CK_MECHANISM_PTR pstMechanism, CK_I_OBJ_PTR pstUnwrapKeyObj,
    CK_BYTE_PTR pucWrappedKey, ULONG ulWrappedKeyLen,
    CK_I_OBJ_PTR pstKeyObj);

ULONG ICA_TOK_EncryptInit(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR pstMechanism, CK_I_OBJ_PTR pstKeyObj);

ULONG ICA_TOK_EncryptUpdate(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucPart, ULONG ulPartLen,
    UCHAR *pucEncryptedPart, ULONG *pulEncryptedPartLen);

ULONG ICA_TOK_EncryptFinal(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucLastEncryptedPart, ULONG *pulLastEncryptedPartLen);

ULONG ICA_TOK_DecryptInit(CK_I_SESSION_DATA_PTR pstSessionData,
    CK_MECHANISM_PTR  pstMechanism, CK_I_OBJ_PTR pstKeyObj);

ULONG ICA_TOK_DecryptUpdate(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucEncryptedPart, ULONG ulEncryptedPartLen,
    UCHAR *pucPart, ULONG *pulPartLen);

ULONG ICA_TOK_DecryptFinal(CK_I_SESSION_DATA_PTR pstSessionData,
    UCHAR *pucLastPart, ULONG *pulLastPartLen);

ULONG ICA_TOK_CloseSession(CK_I_SESSION_DATA_PTR pstSessionData);

#ifdef __cplusplus
}
#endif

#endif /* ICA_TOKEN_H */

