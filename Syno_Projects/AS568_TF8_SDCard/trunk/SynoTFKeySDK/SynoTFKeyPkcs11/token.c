/***************************************************************************
 *	File name:token.c
 *	Introduce:The implement file for Syno TF key functions
 *	Author:Syno common
 *	Date:2015/11/05
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

#include "token.h"
#include "slot.h"
#include "template.h"
#include "objects.h"
#include "utils.h"
#include "hash.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/
CK_BBOOL          s_ucTrue = TRUE;
CK_BBOOL          s_ucFalse = FALSE;

/* RSA��Կ��������ֵ */
CK_OBJECT_CLASS	  s_ulDataClass = CKO_DATA;
CK_OBJECT_CLASS   s_ulPubClass = CKO_PUBLIC_KEY;
CK_OBJECT_CLASS   s_ulPrivClass = CKO_PRIVATE_KEY;
CK_OBJECT_CLASS   s_ulCertClass = CKO_PRIVATE_KEY;
CK_KEY_TYPE       s_ulRSAKeyType = CKK_RSA;
CK_CHAR           s_aucDataObjLabel[] = "A data file object";
CK_CHAR           s_aucPublicKeyObjLabel[] = "A public key object";
CK_CHAR           s_aucPrivateKeyObjLabel[] = "A private key object";
CK_CHAR           s_aucCertObjLabel[] = "A certificate object";
CK_ULONG          s_ulModulusBits = 1024;
CK_BYTE           s_aucPublicExponent[4] = {0, 1, 0, 1};

/* Data File��������ģ�� */
static CK_ATTRIBUTE s_DataObjTemplate[ 7 ] = {
	{ CKA_CLASS, &s_ulDataClass, sizeof(int) },						//�������
	{ CKA_TOKEN, (unsigned char *)&s_ucTrue, sizeof(char) },		//��Token����
	{ CKA_PRIVATE, (unsigned char *)&s_ucFalse, sizeof(char) },		//�ǹ��ж���
	{ CKA_LABEL, s_aucDataObjLabel, sizeof(s_aucDataObjLabel) },	//�����ʾ��
	{ CKA_APPLICATION, NULL, 0 },									//����Ӧ����
	{ CKA_ID, NULL, 0 },											//�ļ��� 
	{ CKA_VALUE, NULL, 0 }											//�ļ�����
};

/* ��Կ����ģ�� */
static CK_ATTRIBUTE s_PublicKeyObjTemplate[ 12 ] = {
	{ CKA_CLASS, &s_ulPubClass, sizeof(s_ulPubClass) },						//�������
	{ CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue) },								//��Token����
	{ CKA_PRIVATE, (unsigned char *)&s_ucFalse, sizeof(char) },				//�ǹ��ж���
	{ CKA_KEY_TYPE, &s_ulRSAKeyType, sizeof(s_ulRSAKeyType) },				//��Կ�㷨����
	{ CKA_LABEL, s_aucPublicKeyObjLabel, sizeof(s_aucPublicKeyObjLabel) },	//�����ʾ��
	{ CKA_ID, NULL, 0 },													//��Կ����������
	{ CKA_VERIFY, &s_ucTrue, sizeof(s_ucTrue) },							//�Ƿ����������֤ǩ��
	{ CKA_ENCRYPT, &s_ucTrue, sizeof(s_ucTrue) },							//�Ƿ����������������
	{ CKA_WRAP, &s_ucTrue, sizeof(s_ucTrue) },								//�Ƿ��������Wrap Key
	{ CKA_MODULUS, NULL_PTR, 0 },											//ģ��n
	{ CKA_MODULUS_BITS, &s_ulModulusBits, sizeof(s_ulModulusBits) },		//ģ��n��λ����
	{ CKA_PUBLIC_EXPONENT, s_aucPublicExponent, sizeof(s_aucPublicExponent) }//����ָ��e
};

/* ˽Կ����ģ�� */
CK_ATTRIBUTE s_PrivateKeyObjTemplate[ 11 ] = {
    { CKA_CLASS, &s_ulPrivClass, sizeof(s_ulPrivClass) },					//�������
	{ CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue) },								//��Token����
	{ CKA_PRIVATE, (unsigned char *)&s_ucTrue, sizeof(char) },				//��˽�ж���
    { CKA_KEY_TYPE, &s_ulRSAKeyType, sizeof(s_ulRSAKeyType) },				//��Կ�㷨����
    { CKA_LABEL, s_aucPrivateKeyObjLabel, sizeof(s_aucPrivateKeyObjLabel) },//�����ʾ��
    { CKA_ID, NULL, 0 },													//��Կ����������
	{ CKA_SENSITIVE, &s_ucFalse, sizeof(s_ucFalse) },						//�Ƿ�����������
	{ CKA_SIGN, &s_ucTrue, sizeof(s_ucTrue) },								//�Ƿ��������ǩ��
	{ CKA_DECRYPT, &s_ucTrue, sizeof(s_ucTrue) },							//�Ƿ����������������
	{ CKA_UNWRAP, &s_ucTrue, sizeof(s_ucTrue) },							//�Ƿ��������Unwrap Key
    { CKA_EXTRACTABLE, &s_ucTrue, sizeof(s_ucTrue) },						//�Ƿ���Ե���(���۸�����Ϊ��ֵ��˽Կ���ݶ����ܵ���)
};

/* ֤�����ģ�� */
CK_ATTRIBUTE s_CertObjTemplate[ 13 ] = {
	{ CKA_CLASS, &s_ulPrivClass, sizeof(s_ulCertClass) },					//�������
	{ CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue) },								//��Token����
	{ CKA_PRIVATE, (unsigned char *)&s_ucFalse, sizeof(char) },				//��˽�ж���
    { CKA_KEY_TYPE, &s_ulRSAKeyType, sizeof(s_ulRSAKeyType) },				//��Կ�㷨����
	{ CKA_LABEL, s_aucCertObjLabel, sizeof(s_aucCertObjLabel) },			//�����ʾ��
    { CKA_ID, NULL, 0 },													//��Կ����������
	{ CKA_SIGN, &s_ucTrue, sizeof(s_ucTrue) },								//�Ƿ���ǩ��֤��
	{ CKA_VERIFY, &s_ucTrue, sizeof(s_ucTrue) },							//�Ƿ���ǩ��֤��
	{ CKA_ENCRYPT, &s_ucTrue, sizeof(s_ucTrue) },							//�Ƿ��Ǽ���֤��
	{ CKA_VALUE, NULL, 0 },													//֤������
	{ CKA_SUBJECT, NULL, 0 },												//֤������
	{ CKA_ISSUER, NULL, 0 },												//֤��䷢��
	{ CKA_SERIAL_NUMBER, NULL, 0 }											//֤�����к�
};


/* DES������Կ��������ֵ */
CK_OBJECT_CLASS s_ulSecretClass = CKO_SECRET_KEY;
CK_KEY_TYPE s_ulDESKeyType = CKK_DES;
CK_CHAR s_aucDESLabel[] = "DESSecretKey";
CK_ULONG s_ulEight = 8;   /* ��Կֵ(ʼ��Ϊ8�ֽڳ�) */
CK_BYTE s_aucValue[8] = {0};

/* DES������Կģ�� */
CK_ATTRIBUTE g_astDESKeyTemplate[10] = {
    {CKA_CLASS, &s_ulSecretClass, sizeof(s_ulSecretClass)},
    {CKA_KEY_TYPE, &s_ulDESKeyType, sizeof(s_ulDESKeyType)},
    {CKA_LABEL, s_aucDESLabel, sizeof(s_aucDESLabel)},
    {CKA_TOKEN, &s_ucFalse, sizeof(s_ucFalse)},
    {CKA_ENCRYPT, &s_ucTrue, sizeof(s_ucTrue)},
    {CKA_DECRYPT, &s_ucTrue, sizeof(s_ucTrue)},
    {CKA_EXTRACTABLE, &s_ucTrue, sizeof(s_ucTrue)},
    {CKA_NEVER_EXTRACTABLE, &s_ucFalse, sizeof(s_ucFalse)},
    {CKA_VALUE, s_aucValue, sizeof(s_aucValue)},
    {CKA_VALUE_LEN, &s_ulEight, sizeof(s_ulEight)}
};

CK_ATTRIBUTE g_astFindRSATemplate[2] = {
    {CKA_CLASS, NULL, 0},
	{CKA_TOKEN, &s_ucTrue, sizeof(s_ucTrue)},
};

CK_ATTRIBUTE g_astGetRSAKeyIDTemplate[1] = {
    {CKA_ID, NULL, 0},  /* ��Կ����Կ��ʶ�� */
};

CK_ATTRIBUTE g_astGetRSAPublicKeyTemplate[1] = {
    {CKA_MODULUS, NULL_PTR, 0},
};

/* ����RAS���� */
CK_MECHANISM g_stMechGenRSA = {CKM_RSA_PKCS_KEY_PAIR_GEN, NULL_PTR, 0};
CK_MECHANISM g_stMechRSA = {CKM_RSA_PKCS, NULL_PTR, 0};

/* DES���� */
CK_MECHANISM g_stMechGenDES = {CKM_DES_KEY_GEN, NULL_PTR, 0};
CK_MECHANISM g_stDESmech = {CKM_DES_ECB, NULL_PTR, 0};

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/
extern CK_RV CI_ObjTemplateInit(CK_I_OBJ_PTR CK_PTR ppObjectRef,
    CK_ATTRIBUTE_PTR pTemplate, CK_ULONG ulTemplateLen);

extern CK_RV CI_ObjDestroyObj(CK_I_OBJ_PTR  pObject);

extern CK_RV CI_ContainerAddObj(CK_I_HASHTABLE_PTR container,
    CK_ULONG key, CK_I_OBJ_PTR pObject);

extern CK_RV CI_NewHandle(CK_ULONG_PTR handle);


/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define TOK_ONCE_RW_LEN             2048

#define TOK_MAX_SESSION_COUNT       8
#define TOK_MAX_RW_SESSION_COUNT    8
#define TOK_MIN_PIN_LEN             6
#define TOK_MAX_PIN_LEN             15

#define TOK_SO_PIN_RETRY_COUNT      10
#define TOK_USER_PIN_RETRY_COUNT    10

#define TOK_INIT_APPNAME			"SynoInitApp"
#define TOK_INIT_CONTAINER			"SynoRSACon"
#define	TOK_INIT_USERPIN			"111111"

static CK_I_TOKEN_METHODS s_ck_i_token_methods = {0};
static CK_MECHANISM_TYPE s_ck_i_token_mechanism_types[] = {CKM_RSA_PKCS_KEY_PAIR_GEN, CKM_RSA_PKCS, CKM_MD5_RSA_PKCS, CKM_SHA1_RSA_PKCS, CKM_SHA256_RSA_PKCS,
														   CKM_DES_KEY_GEN, CKM_DES_ECB, CKM_DES_CBC, CKM_DES3_KEY_GEN, CKM_DES3_ECB, CKM_DES3_CBC,
														   CKM_AES_KEY_GEN, CKM_AES_ECB, CKM_AES_CBC, CKM_MD5, CKM_SHA_1, CKM_SHA256};

/***********************************************************************
 *	����:TOK_I_SetFunctionList
 *	˵��:����Token��صĺ�����ַ��
 *	����:��
 *	����:��
 ***********************************************************************/
static CK_RV TOK_I_SetFunctionList()
{
	FUNC_BEGIN;

	s_ck_i_token_methods.GetTokenInfo = TOK_GetTokenInfo;
	s_ck_i_token_methods.GetMechanismList = TOK_GetMechanismList;
	s_ck_i_token_methods.GetMechanismInfo = TOK_GetMechanismInfo;
	s_ck_i_token_methods.InitToken = TOK_InitToken;
	s_ck_i_token_methods.FinalizeToken = TOK_FinalizeToken;
	s_ck_i_token_methods.InitPIN = TOK_InitPIN;
	s_ck_i_token_methods.SetPIN = TOK_SetPIN;
	s_ck_i_token_methods.OpenSession = TOK_OpenSession;
	s_ck_i_token_methods.CloseSession = TOK_CloseSession;
	s_ck_i_token_methods.GetOperationState = TOK_GetOperationState;
	s_ck_i_token_methods.SetOperationState = TOK_SetOperationState;
	s_ck_i_token_methods.Login = TOK_Login;
	s_ck_i_token_methods.Logout = TOK_Logout;
	s_ck_i_token_methods.EncryptInit = TOK_EncryptInit;
	s_ck_i_token_methods.Encrypt = TOK_Encrypt;
	s_ck_i_token_methods.EncryptUpdate = TOK_EncryptUpdate;
	s_ck_i_token_methods.EncryptFinal = TOK_EncryptFinal;
	s_ck_i_token_methods.DecryptInit = TOK_DecryptInit;
	s_ck_i_token_methods.Decrypt = TOK_Decrypt;
	s_ck_i_token_methods.DecryptUpdate = TOK_DecryptUpdate;
	s_ck_i_token_methods.DecryptFinal = TOK_DecryptFinal;
	s_ck_i_token_methods.DigestInit = TOK_DigestInit;
	s_ck_i_token_methods.Digest = TOK_Digest;
	s_ck_i_token_methods.DigestUpdate = TOK_DigestUpdate;
	s_ck_i_token_methods.DigestKey = TOK_DigestKey;
	s_ck_i_token_methods.DigestFinal = TOK_DigestFinal;
	s_ck_i_token_methods.SignInit = TOK_SignInit;
	s_ck_i_token_methods.Sign = TOK_Sign;
	s_ck_i_token_methods.SignUpdate = TOK_SignUpdate;
	s_ck_i_token_methods.SignFinal = TOK_SignFinal;
	s_ck_i_token_methods.SignRecoverInit = TOK_SignRecoverInit;
	s_ck_i_token_methods.SignRecover = TOK_SignRecover;
	s_ck_i_token_methods.VerifyInit = TOK_VerifyInit;
	s_ck_i_token_methods.Verify = TOK_Verify;
	s_ck_i_token_methods.VerifyUpdate = TOK_VerifyUpdate;
	s_ck_i_token_methods.VerifyFinal = TOK_VerifyFinal;
	s_ck_i_token_methods.VerifyRecoverInit = TOK_VerifyRecoverInit;
	s_ck_i_token_methods.VerifyRecover = TOK_VerifyRecover;
	s_ck_i_token_methods.DigestEncryptUpdate = TOK_DigestEncryptUpdate;
	s_ck_i_token_methods.DecryptDigestUpdate = TOK_DecryptDigestUpdate;
	s_ck_i_token_methods.SignEncryptUpdate = TOK_SignEncryptUpdate;
	s_ck_i_token_methods.DecryptVerifyUpdate = TOK_DecryptVerifyUpdate;
	s_ck_i_token_methods.GenerateKey = TOK_GenerateKey;
	s_ck_i_token_methods.GenerateKeyPair = TOK_GenerateKeyPair;
	s_ck_i_token_methods.WrapKey = TOK_WrapKey;
	s_ck_i_token_methods.UnwrapKey = TOK_UnwrapKey;
	s_ck_i_token_methods.DeriveKey = TOK_DeriveKey;
	s_ck_i_token_methods.SeedRandom = TOK_SeedRandom;
	s_ck_i_token_methods.GenerateRandom = TOK_GenerateRandom;
	s_ck_i_token_methods.GetFunctionStatus = TOK_GetFunctionStatus;
	s_ck_i_token_methods.CancelFunction = TOK_CancelFunction;
	s_ck_i_token_methods.WaitForSlotEvent = TOK_WaitForSlotEvent;
	s_ck_i_token_methods.TokenObjAdd = TOK_TokenObjAdd;
	s_ck_i_token_methods.TokenObjCommit = TOK_TokenObjCommit;
	s_ck_i_token_methods.TokenObjDelete = TOK_TokenObjDelete;

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	����:TOK_I_StrFmtList
 *	˵��:�����ܸ�ʽ���ַ�������ת��Ϊ���ַ�','�ָ����ַ���List
 *	����:pcSrc:[IN]:������ַ�������
 *       pcDst:[OUT]:���ص��ַ���List����','�ָ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
void TOK_I_StrFmtList(CHAR *pcDst, CHAR *pcSrc)
{
    UCHAR *pucTmp = NULL;
	ULONG ulTmpLen = 0;
	ULONG ulLen = 0;

    if ((NULL == pcDst) || (NULL == pcSrc))
    {
        return;
    }

    pucTmp = (UCHAR *)pcSrc;
	while (pucTmp[0] != '\0')
	{
		strncpy((pcDst + ulLen), (char *)pucTmp, strlen((char *)pucTmp));
		ulTmpLen = strlen(((char *)pcDst + ulLen));
		pucTmp += (ulTmpLen + 1);
		ulLen += ulTmpLen;
		if (pucTmp[0] != '\0')
		{
			//sprintf(pcDst + ulLen, 2, "%s", ", ");
			memcpy(pcDst + ulLen, ",", 1);
			ulLen += 1;
		}
	}

    return;
}
/***********************************************************************
 *	����:TOK_I_GenUniqeContainerId
 *	˵��:ʹ�����������һ��Ψһ��������
 *	����:pPubKeyObj:[IN]:�ڲ���Կ����ָ��
 *       pstuRsaPubKey:[OUT]:���صĹ��깫Կ�ṹ��ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static LPSTR TOK_I_GenUniqeContainerId()
{
	ULONG ulIndex = 0;
	CHAR csBaseChar[26] = {0};
	CHAR csRandom[10 + 1] = {0};
	const ULONG ulRandomLen = 10;
	static CHAR csResult[32] = {0};
	
	for (ulIndex = 0; ulIndex < 26; ulIndex++)
	{
		csBaseChar[ulIndex] = (CHAR)('A' + ulIndex);
	}

    srand(time(NULL));

    for (ulIndex = 0; ulIndex < ulRandomLen; ulIndex++)
    {
		csRandom[ulIndex] = csBaseChar[rand() % sizeof(csBaseChar)];
    }
	csRandom[ulRandomLen] = '\0';

	memset(csResult, 0, sizeof(csResult));
	strcpy(csResult, "P11_");
	strcat(csResult, csRandom);

	return csResult;
}
/***********************************************************************
 *	����:TOK_I_ConvertPubKeyObj
 *	˵��:����Կ����ת��Ϊ�����еĽṹ����ʽ
 *	����:pPubKeyObj:[IN]:�ڲ���Կ����ָ��
 *       pstuRsaPubKey:[OUT]:���صĹ��깫Կ�ṹ��ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static CK_RV TOK_I_ConvertPubKeyObj(CK_I_OBJ_PTR pPubKeyObj, 
									PRSAPUBLICKEYBLOB pstuRsaPubKey)
{
	ULONG ulRet = CKR_OK;
    CK_ATTRIBUTE_PTR pObjAttr = NULL;

	FUNC_BEGIN;

	// �������
	if (NULL == pPubKeyObj)
	{
		SYNO_LogE("pPubKeyObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL == pstuRsaPubKey)
	{
		SYNO_LogE("pstuRsaPubKey is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	// �㷨���
	pstuRsaPubKey->AlgID = SGD_RSA;

	// ģBits
	pObjAttr = CI_ObjLookup(pPubKeyObj, CK_IA_MODULUS_BITS);
	if (NULL == pObjAttr || NULL == pObjAttr->pValue)
	{
		SYNO_LogE("CK_IA_MODULUS_BITS attribyte is NULL!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	pstuRsaPubKey->BitLen = *((CK_ULONG_PTR)pObjAttr->pValue);

	// ģ����
	pObjAttr = CI_ObjLookup(pPubKeyObj, CK_IA_MODULUS);
	if (NULL == pObjAttr || NULL == pObjAttr->pValue)
	{
		SYNO_LogE("CK_IA_MODULUS attribyte is NULL!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	if (pObjAttr->ulValueLen != pstuRsaPubKey->BitLen / 8)
	{
		SYNO_LogE("CK_IA_MODULUS attribyte value length is wrong! pObjAttr->ulValueLen = 0x%x", pObjAttr->ulValueLen);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	memcpy(pstuRsaPubKey->Modulus + (MAX_RSA_MODULUS_LEN - pstuRsaPubKey->BitLen/8), pObjAttr->pValue, pObjAttr->ulValueLen);

	// ָ��
	pObjAttr = CI_ObjLookup(pPubKeyObj, CK_IA_PUBLIC_EXPONENT);
	if (NULL == pObjAttr || NULL == pObjAttr->pValue)
	{
		SYNO_LogE("CK_IA_PUBLIC_EXPONENT attribyte is NULL!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	if (pObjAttr->ulValueLen != 4)
	{
		SYNO_LogE("CK_IA_PUBLIC_EXPONENT attribyte value length is wrong! pObjAttr->ulValueLen = 0x%x", pObjAttr->ulValueLen);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	memcpy(pstuRsaPubKey->PublicExponent, pObjAttr->pValue, pObjAttr->ulValueLen);

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	����:TOK_I_EnumDataFileObj
 *	˵��:ö��ָ��Ӧ���µ�����Data File����������Ӧ�Ķ���
 *	����:session_data:[IN]:��ǰ�Ự����
 *	     hApp:[IN]:Ӧ�ö�����
 *		 lpAppName:[IN]:Ӧ�ö�����hApp��Ӧ��Ӧ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static ULONG TOK_I_EnumDataFileObj(CK_I_SESSION_DATA_PTR session_data, 
								   HAPPLICATION hApp,
								   LPSTR lpAppName)
{
	ULONG ulRet = SAR_OK;
	ULONG ulCount = 0;
	ULONG ulFilesLen = 0;
	CHAR* lpFile = NULL;
	CHAR* lpFileNames = NULL;
	CHAR* lpFileNameList = NULL;
	CHAR* lpRemainder = NULL;
	FILEATTRIBUTE fileInfo = {0};
	CK_I_OBJ_PTR new_obj = NULL_PTR;
	CK_ATTRIBUTE_PTR pAttribute = NULL_PTR;
	CK_OBJECT_HANDLE hDataObject = NULL_PTR;

	// �������
	if (NULL == session_data)
	{
		SYNO_LogE("session_data is NULL!");
		return 0;
	}
	if (NULL == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return 0;
	}
	if (NULL == lpAppName || strlen(lpAppName) == 0)
	{
		SYNO_LogE("lpAppName is NULL!");
		return 0;
	}

	// ��ȡ�ļ����б��С
	ulRet = P11_EnumFiles(hApp, NULL, &ulFilesLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_EnumFiles() failed! ulRet = 0x%x", ulRet);
		return 0;
	}
	if (ulFilesLen <= 2)
	{
		SYNO_LogD("No any data files!");
		return 0;
	}

	// ö���ļ���
	lpFileNames = (CHAR*)SYNO_malloc(ulFilesLen + 2);
	memset(lpFileNames, 0, ulFilesLen + 2);
	ulRet = P11_EnumFiles(hApp, lpFileNames, &ulFilesLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_EnumFiles() failed! ulRet = 0x%x", ulRet);
		ulRet = 0;
		goto END;
	}

	// �����ص��ļ�������ת��Ϊ�ַ����б�
	lpFileNameList = (CHAR*)SYNO_malloc(ulFilesLen + 2);
	memset(lpFileNameList, 0, ulFilesLen + 2);
	TOK_I_StrFmtList(lpFileNameList, lpFileNames);

	// ��ÿ���ļ�����һ��CKO_DATA����	
	lpFile = TC_strtok_r(lpFileNameList, ",", &lpRemainder);
	while (lpFile && strlen(lpFile) > 0)
	{
		SYNO_LogD("Found data file:%s", lpFile);

		// ��ȡ�ļ�����
		ulRet = P11_GetFileInfo(hApp, lpFile, &fileInfo);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("P11_GetFileInfo() failed! ulRet = 0x%x", ulRet);
			goto NEXT;
		}
		if (fileInfo.FileSize == 0)
		{
			SYNO_LogE("fileInfo.FileSize = 0!");
			goto NEXT;
		}
		
		// ��������
		ulRet = CI_ObjCreateObj(&new_obj);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjCreateObj() failed! ulRet = 0x%x", ulRet);
			goto NEXT;
		}
		
		// ���ö�������ģ��
		ulRet = CI_ObjReadTemplate(new_obj, s_DataObjTemplate, sizeof(s_DataObjTemplate)/sizeof(s_DataObjTemplate[0]));
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjReadTemplate() failed! ulRet = 0x%x", ulRet);
			CI_ObjDestroyObj(new_obj);
			new_obj = NULL;
			goto NEXT;
		}
		
		// �����ļ�������
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ID));
		pAttribute->pValue = SYNO_calloc(1, strlen(fileInfo.FileName));
		strncpy((char*)pAttribute->pValue, fileInfo.FileName, strlen(fileInfo.FileName));
		pAttribute->ulValueLen = strlen(fileInfo.FileName);

		// �����ļ���������
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_VALUE));
		pAttribute->pValue = SYNO_calloc(1, fileInfo.FileSize);
		pAttribute->ulValueLen = fileInfo.FileSize;
		ulRet = P11_ReadFile(hApp, lpFile, 0, fileInfo.FileSize, (BYTE*)pAttribute->pValue, &pAttribute->ulValueLen);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("P11_ReadFile() failed! ulRet = 0x%x", ulRet);
			CI_ObjDestroyObj(new_obj);
			new_obj = NULL;
			goto NEXT;
		}
		
		// ���ö����������б�
		ulRet = CI_InternalAddObject(session_data, new_obj, &hDataObject);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_InternalAddObject() failed! ulRet = 0x%x", ulRet);
			CI_ObjDestroyObj(new_obj);
			new_obj = NULL;
			goto NEXT;
		}

		ulCount++;

NEXT:
		lpFile = TC_strtok_r(NULL, ",", &lpRemainder);
	}
	
	FUNC_END;

END:
	if (lpFileNameList)
	{
		SYNO_free(lpFileNameList);
		lpFileNameList = NULL;
	}
	if (lpFileNames)
	{
		SYNO_free(lpFileNames);
		lpFileNames = NULL;
	}
	
	return ulCount;
}
/***********************************************************************
 *	����:TOK_I_EnumPubKeyObj
 *	˵��:ö��ָ�������µ����й�Կ����������Ӧ�Ķ���
 *	����:session_data:[IN]:��ǰ�Ự����
 *	     hContainer:[IN]:����������
 *		 lpContainerName:[IN]:����������hContainer��Ӧ��������
 *		 lpContainerInfoEx:[IN]:����������hContainer��Ӧ����������
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static ULONG TOK_I_EnumPubKeyObj(CK_I_SESSION_DATA_PTR session_data, 
								 HCONTAINER hContainer,
								 LPSTR lpContainerName,
								 LPCONTAINERINFOEX lpContainerInfoEx)
{
	ULONG ulRet = SAR_OK;
	ULONG ulCount = 0;
	BOOL bHasPubKey[2] = {FALSE, FALSE};
	ULONG ulIndex = 0;
	ULONG ulRsaPubKeyLen = 0;	
	RSAPUBLICKEYBLOB stRsaPubKey = {0};
	CK_I_OBJ_PTR new_obj = NULL_PTR;
	CK_ATTRIBUTE_PTR pAttribute = NULL_PTR;
	CK_OBJECT_HANDLE hPubKeyObject = NULL_PTR;

	// �������
	if (NULL == session_data)
	{
		SYNO_LogE("session_data is NULL!");
		return 0;
	}
	if (NULL == hContainer)
	{
		SYNO_LogE("hContainer is NULL!");
		return 0;
	}
	if (NULL == lpContainerName || strlen(lpContainerName) == 0)
	{
		SYNO_LogE("lpContainerName is NULL!");
		return 0;
	}
	if (NULL == lpContainerInfoEx)
	{
		SYNO_LogE("lpContainerInfoEx is NULL!");
		return 0;
	}

	// �ж��Ƿ��й�Կ�ļ�����
	if (lpContainerInfoEx->signKeyPair)
	{
		bHasPubKey[0] = TRUE;
		SYNO_LogD("Has sign key pair!");
	}
	if (lpContainerInfoEx->exchKeyPair)
	{
		bHasPubKey[1] = TRUE;
		SYNO_LogD("Has exch key pair!");
	}
	if (lpContainerInfoEx->hasVerfiyPubKey)
	{
		bHasPubKey[0] = TRUE;
		SYNO_LogD("Has a verfiy public key!");
	}
	if (lpContainerInfoEx->hasEncryptPubKey)
	{
		bHasPubKey[1] = TRUE;
		SYNO_LogD("Has a encrypt public key!");
	}
	if (!bHasPubKey[0] && !bHasPubKey[1])
	{
		SYNO_LogD("No any public key!");
		return 0;
	}

	// ���ε�����Կ����������
	for (ulIndex = 0; ulIndex < 2; ulIndex++)
	{	
		if (!bHasPubKey[ulIndex])
		{
			continue;
		}

		// ������Կ����
		ulRsaPubKeyLen = sizeof(stRsaPubKey);
		ulRet = P11_ExportPublicKey(hContainer, (0 == ulIndex), (BYTE*)&stRsaPubKey, &ulRsaPubKeyLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExportPublicKey() failed! ulRet = 0x%x", ulRet);
			continue;
		}
	
		// ��������
		ulRet = CI_ObjCreateObj(&new_obj);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjCreateObj() failed! ulRet = 0x%x", ulRet);
			continue;
		}
		
		// ���ö�������ģ��
		ulRet = CI_ObjReadTemplate(new_obj, s_PublicKeyObjTemplate, sizeof(s_PublicKeyObjTemplate)/sizeof(s_PublicKeyObjTemplate[0]));
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjReadTemplate() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
	
		// ����������
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ID));
		pAttribute->pValue = SYNO_calloc(1, strlen(lpContainerName));
		strncpy((char*)pAttribute->pValue, lpContainerName, strlen(lpContainerName));
		pAttribute->ulValueLen = strlen(lpContainerName);
	
		// ���ù�Կ��;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_VERIFY));
		*(CK_BYTE_PTR)pAttribute->pValue = (0 == ulIndex) ? s_ucTrue : s_ucFalse;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_WRAP));
		*(CK_BYTE_PTR)pAttribute->pValue = (0 == ulIndex) ? s_ucFalse : s_ucTrue;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ENCRYPT));
		*(CK_BYTE_PTR)pAttribute->pValue = (0 == ulIndex) ? s_ucFalse : s_ucTrue;
	
		// ���ù�Կbits����
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_MODULUS_BITS));
		*(CK_ULONG_PTR)pAttribute->pValue = stRsaPubKey.BitLen;

		// ���ù�Կָ��
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_PUBLIC_EXPONENT));
		memcpy(pAttribute->pValue, stRsaPubKey.PublicExponent, 4);

		// ���ù�Կģ
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_MODULUS));
		pAttribute->pValue = SYNO_calloc(1, stRsaPubKey.BitLen / 8);
		memcpy(pAttribute->pValue, stRsaPubKey.Modulus + MAX_RSA_MODULUS_BITS/8-stRsaPubKey.BitLen/8, stRsaPubKey.BitLen / 8);
		pAttribute->ulValueLen = stRsaPubKey.BitLen / 8;

		// �����������
		new_obj->CtrHandle = hContainer;

		// ���ö����������б�
		ulRet = CI_InternalAddObject(session_data, new_obj, &hPubKeyObject);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_InternalAddObject() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
		else
		{
			// ��һ��
			ulCount++;
			continue;
		}

FREE_OBJ:
		CI_ObjDestroyObj(new_obj);
		new_obj = NULL;
	}
	
	FUNC_END;

	return ulCount;
}
/***********************************************************************
 *	����:TOK_I_EnumPriKeyObj
 *	˵��:ö��ָ�������µ�����˽Կ����������Ӧ�Ķ���
 *	����:session_data:[IN]:��ǰ�Ự����
 *	     hContainer:[IN]:����������
 *		 lpContainerName:[IN]:����������hContainer��Ӧ��������
 *		 lpContainerInfoEx:[IN]:����������hContainer��Ӧ����������
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static ULONG TOK_I_EnumPriKeyObj(CK_I_SESSION_DATA_PTR session_data, 
								 HCONTAINER hContainer,
								 LPSTR lpContainerName,
								 LPCONTAINERINFOEX lpContainerInfoEx)
{
	ULONG ulRet = SAR_OK;
	ULONG ulCount = 0;
	ULONG ulIndex = 0;
	BOOL bHasPriKey[2] = {FALSE, FALSE};	
	CK_I_OBJ_PTR new_obj = NULL_PTR;
	CK_ATTRIBUTE_PTR pAttribute = NULL_PTR;
	CK_OBJECT_HANDLE hPriKeyObject = NULL_PTR;

	// �������
	if (NULL == session_data)
	{
		SYNO_LogE("session_data is NULL!");
		return 0;
	}
	if (NULL == hContainer)
	{
		SYNO_LogE("hContainer is NULL!");
		return 0;
	}
	if (NULL == lpContainerName || strlen(lpContainerName) == 0)
	{
		SYNO_LogE("lpContainerName is NULL!");
		return 0;
	}
	if (NULL == lpContainerInfoEx)
	{
		SYNO_LogE("lpContainerInfoEx is NULL!");
		return 0;
	}

	// �ж��Ƿ��й�Կ�ļ�����
	if (lpContainerInfoEx->signKeyPair)
	{
		bHasPriKey[0] = TRUE;
		SYNO_LogD("Has sign key pair!");
	}
	if (lpContainerInfoEx->exchKeyPair)
	{
		bHasPriKey[1] = TRUE;
		SYNO_LogD("Has exch key pair!");
	}
	if (lpContainerInfoEx->hasSignPriKey)
	{
		bHasPriKey[0] = TRUE;
		SYNO_LogD("Has a sign private key!");
	}
	if (lpContainerInfoEx->hasDecryptPriKey)
	{
		bHasPriKey[1] = TRUE;
		SYNO_LogD("Has a decrypt private key!");
	}
	if (!bHasPriKey[0] && !bHasPriKey[1])
	{
		SYNO_LogD("No any private key!");
		return 0;
	}
		
	// ���δ���˽Կ����
	for (ulIndex = 0; ulIndex < 2; ulIndex++)
	{
		if (!bHasPriKey[ulIndex])
		{
			continue;
		}

		// ��������
		ulRet = CI_ObjCreateObj(&new_obj);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjCreateObj() failed! ulRet = 0x%x", ulRet);
			continue;
		}
		
		// ���ö�������ģ��
		ulRet = CI_ObjReadTemplate(new_obj, s_PrivateKeyObjTemplate, sizeof(s_PrivateKeyObjTemplate)/sizeof(s_PrivateKeyObjTemplate[0]));
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjReadTemplate() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
	
		// ����������
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ID));
		pAttribute->pValue = SYNO_calloc(1, strlen(lpContainerName));
		strncpy((char*)pAttribute->pValue, lpContainerName, strlen(lpContainerName));
		pAttribute->ulValueLen = strlen(lpContainerName);
	
		// ����˽Կ��;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_SIGN));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucTrue : s_ucFalse;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_UNWRAP));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucFalse : s_ucTrue;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_DECRYPT));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucFalse : s_ucTrue;
	
		// �����������
		new_obj->CtrHandle = hContainer;

		// ���ö����������б�
		ulRet = CI_InternalAddObject(session_data, new_obj, &hPriKeyObject);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_InternalAddObject() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
		else
		{
			// ��һ��
			ulCount++;
			continue;
		}

FREE_OBJ:
		CI_ObjDestroyObj(new_obj);
		new_obj = NULL;
	}

	FUNC_END;
	
	return ulCount;
}
/***********************************************************************
 *	����:TOK_I_EnumCertObj
 *	˵��:ö��ָ�������µ�����֤�飬��������Ӧ�Ķ���
 *	����:session_data:[IN]:��ǰ�Ự����
 *	     hContainer:[IN]:����������
 *		 lpContainerName:[IN]:����������hContainer��Ӧ��������
 *		 lpContainerInfoEx:[IN]:����������hContainer��Ӧ����������
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static ULONG TOK_I_EnumCertObj(CK_I_SESSION_DATA_PTR session_data, 
							   HCONTAINER hContainer,
								 LPSTR lpContainerName,
								 LPCONTAINERINFOEX lpContainerInfoEx)
{
	ULONG ulRet = SAR_OK;
	ULONG ulCount = 0;
	ULONG ulIndex = 0;
	ULONG ulCertDataLen = 0;
	BYTE* lpCertData = NULL;
	BOOL bHasCertificate[2] = {FALSE, FALSE};
	CK_I_OBJ_PTR new_obj = NULL_PTR;
	CK_ATTRIBUTE_PTR pAttribute = NULL_PTR;
	CK_OBJECT_HANDLE hCertObject = NULL_PTR;

	// �������
	if (NULL == session_data)
	{
		SYNO_LogE("session_data is NULL!");
		return 0;
	}
	if (NULL == hContainer)
	{
		SYNO_LogE("hContainer is NULL!");
		return 0;
	}
	if (NULL == lpContainerName || strlen(lpContainerName) == 0)
	{
		SYNO_LogE("lpContainerName is NULL!");
		return 0;
	}
	if (NULL == lpContainerInfoEx)
	{
		SYNO_LogE("lpContainerInfoEx is NULL!");
		return 0;
	}

	// �ж��Ƿ��й�Կ�ļ�����
	if (lpContainerInfoEx->hasSignCert)
	{
		bHasCertificate[0] = TRUE;
		SYNO_LogD("Has sign certificate!");
	}
	if (lpContainerInfoEx->hasExchCert)
	{
		bHasCertificate[1] = TRUE;
		SYNO_LogD("Has sign certificate!");
	}
	if (!bHasCertificate[0] && !bHasCertificate[0])
	{
		SYNO_LogD("No any certificate!");
		return CKR_OK;
	}
		
	// ���ε���֤�鲢����֤�����
	for (ulIndex = 0; ulIndex < 2; ulIndex++)
	{
		if (!bHasCertificate[ulIndex])
		{
			continue;
		}

		// ����֤��
		ulRet = P11_ExportCertificate(hContainer, (0 == ulIndex), NULL, &ulCertDataLen);
		if (ulRet != CKR_OK || 0 == ulCertDataLen)
		{
			SYNO_LogE("P11_ExportCertificate() failed! ulRet = 0x%x", ulRet);
			continue;
		}
		lpCertData = (BYTE*)SYNO_calloc(1, ulCertDataLen);
		ulRet = P11_ExportCertificate(hContainer, (0 == ulIndex), lpCertData, &ulCertDataLen);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("P11_ExportCertificate() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}

		// ��������
		ulRet = CI_ObjCreateObj(&new_obj);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjCreateObj() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
		
		// ���ö�������ģ��
		ulRet = CI_ObjReadTemplate(new_obj, s_CertObjTemplate, sizeof(s_CertObjTemplate)/sizeof(s_CertObjTemplate[0]));
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_ObjReadTemplate() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}

		// ����֤������
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_CERTIFICATE_TYPE));
		*(CK_ULONG_PTR)pAttribute->pValue = CKC_X_509;
	
		// ����������
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ID));
		pAttribute->pValue = SYNO_calloc(1, strlen(lpContainerName));
		strncpy((char*)pAttribute->pValue, lpContainerName, strlen(lpContainerName));
		pAttribute->ulValueLen = strlen(lpContainerName);
	
		// ����֤����;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_SIGN));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucTrue : s_ucFalse;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_VERIFY));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucTrue : s_ucFalse;
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_ENCRYPT));
		*(CK_BYTE_PTR)pAttribute->pValue = (0==ulIndex)? s_ucFalse : s_ucTrue;

		// ����֤������
		pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(new_obj, CK_IA_VALUE));
		pAttribute->pValue = SYNO_calloc(1, ulCertDataLen);
		memcpy(pAttribute->pValue, lpCertData, ulCertDataLen);
		pAttribute->ulValueLen = ulCertDataLen;
	
		// �����������
		new_obj->CtrHandle = hContainer;

		// ���ö����������б�
		ulRet = CI_InternalAddObject(session_data, new_obj, &hCertObject);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("CI_InternalAddObject() failed! ulRet = 0x%x", ulRet);
			goto FREE_OBJ;
		}
		else
		{
			// ��һ��
			ulCount++;
			continue;
		}

FREE_OBJ:
		SYNO_free(lpCertData);
		lpCertData = NULL;
		CI_ObjDestroyObj(new_obj);
		new_obj = NULL;
	}

	FUNC_END;
	
	return ulCount;
}

/***********************************************************************
 *	����:TOK_I_ImportPublicKey
 *	˵��:����Կ�������ݵ��뵽Key��
 *	����:hApp:[IN]:Ӧ�ö�����
 *	     pPubKeyObj:[IN]:��Կ����ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static CK_RV TOK_I_ImportPublicKey(HAPPLICATION hApp, 
								   CK_I_OBJ_PTR pPubKeyObj)
{
	ULONG ulRet = SAR_OK;
	ULONG ulValueLen = 0;
	BYTE bIsSign = FALSE;
	BYTE bIsExch = FALSE;
	BYTE bIsEncrypt = FALSE;
	BYTE bIsDecrypt = FALSE;
	BYTE btModulus[MAX_RSA_MODULUS_LEN] = {0};
	CHAR csContainer[64] = {0};
	LPSTR lpNewContainer = NULL;
	RSAPUBLICKEYBLOB stRsaPubKey = {0};
	HCONTAINER hContainer = NULL;

	// �������
	if (NULL_PTR == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pPubKeyObj)
	{
		SYNO_LogE("pPubKeyObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// �����㷨��ʾ
	stRsaPubKey.AlgID = SGD_RSA;

	// ��ȡ��Կģ
	ulValueLen = MAX_RSA_MODULUS_LEN;
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_MODULUS, btModulus, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_MODULUS failed! ulRet = 0x%x", ulRet);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	if (128 != ulValueLen && 256 != ulValueLen)
	{
		SYNO_LogE("CKA_MODULUS attribute data length is wrong! ulValueLen = 0x%x", ulValueLen);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	memcpy(stRsaPubKey.Modulus + (MAX_RSA_MODULUS_LEN - ulValueLen), btModulus, ulValueLen);

	// ���㹫Կbits(��ֱ�ӻ�ȡCKA_MODULUS_BITS���ԣ���Ϊ�ò���Ϊ��ѡ)
	stRsaPubKey.BitLen = ulValueLen * 8;

	ulValueLen = sizeof(stRsaPubKey.BitLen);
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_MODULUS_BITS, (CK_BYTE_PTR)&stRsaPubKey.BitLen, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_MODULUS_BITS failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}	
		
	// ��ȡ��Կָ��
	ulValueLen = sizeof(stRsaPubKey.PublicExponent);
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_PUBLIC_EXPONENT, stRsaPubKey.PublicExponent, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_MODULUS failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	// ��ȡ��Կ��;
	ulValueLen = sizeof(BYTE);
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_VERIFY, (CK_BYTE_PTR)&bIsSign, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VERIFY failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_WRAP, (CK_BYTE_PTR)&bIsExch, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_WRAP failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_ENCRYPT, (CK_BYTE_PTR)&bIsEncrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ENCRYPT failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_DECRYPT, (CK_BYTE_PTR)&bIsDecrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_DECRYPT failed! ulRet = 0x%x", ulRet);
	}
	if (!bIsSign && !bIsExch && !bIsEncrypt && !bIsDecrypt)
	{
		SYNO_LogE("All of CKA_VERIFY, CKA_WRAP, CKA_ENCRYPT and CKA_DECRYPT attribute is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// �����Կ��;��Ϣ
	if (bIsSign) SYNO_LogD("This public key can be use to verify signature.");
	if (bIsExch) SYNO_LogD("This public key can be use to wrap session key.");
	if (bIsEncrypt) SYNO_LogD("This public key can be use to encrypt data.");
	if (bIsDecrypt) SYNO_LogD("This public key can be use to decrypt data.");
	
	// ������,�Ȼ�ȡCKA_ID����ֵ,���û�и������򴴽�һ��������
	ulValueLen = 64;
	ulRet = CI_ObjGetAttributeValue(pPubKeyObj, CKA_ID, (CK_BYTE_PTR)csContainer, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ID failed!ulRet = 0x%x", ulRet);
	
		// ��������
		do
		{
			lpNewContainer = TOK_I_GenUniqeContainerId();
			ulRet = P11_CreateContainer(hApp, lpNewContainer, &hContainer);
		}while (SAR_COS_CONTAINER_ALREADY_EXISTS == ulRet);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		SYNO_LogD("New container: %s is created��", lpNewContainer);
		CI_ObjSetAttributeValue(pPubKeyObj, CKA_ID, (CK_BYTE_PTR)lpNewContainer, strlen(lpNewContainer));
	}
	else
	{
		SYNO_LogD("Try to open and create the set container: %s��", csContainer);
		ulRet = P11_OpenContainer(hApp, csContainer, &hContainer);
		if (SAR_OK != ulRet)
		{
			ulRet = P11_CreateContainer(hApp, csContainer, &hContainer);
			if (SAR_OK != ulRet)
			{
				SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
				return CKR_GENERAL_ERROR;
			}
		}
	}

	// ���빫Կ
	ulRet = P11_ImportPlainRSAPubKey(hContainer, bIsSign ? 1 : 0,  &stRsaPubKey);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_ImportPlainRSAKeyPair() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}
	pPubKeyObj->CtrHandle = hContainer;

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	����:TOK_I_ImportPrivateKey
 *	˵��:��˽Կ�������ݵ��뵽Key��
 *	����:hApp:[IN]:Ӧ�ö�����
 *       pPriKeyObj:[IN]:˽Կ����ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static CK_RV TOK_I_ImportPrivateKey(HAPPLICATION hApp,
									CK_I_OBJ_PTR pPriKeyObj)
{
	ULONG ulRet = SAR_OK;
	ULONG ulValueLen = 0;
	BYTE bIsSign = FALSE;
	BYTE bIsExch = FALSE;
	BYTE bIsEncrypt = FALSE;
	BYTE bIsDecrypt = FALSE;
	CHAR csContainer[64] = {0};
	LPSTR lpNewContainer = NULL;
	RSAPRIVATEKEYBLOB stRsaPriKey = {0};
	HCONTAINER hContainer = NULL;
	BYTE btData[256] = {0};

	// �������
	if (NULL_PTR == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pPriKeyObj)
	{
		SYNO_LogE("pPubKeyObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// �����㷨��ʾ
	stRsaPriKey.AlgID = SGD_RSA;

	// ��ȡ˽Կָ��PrivateExponent
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_PRIVATE_EXPONENT, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_PRIVATE_EXPONENT failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (128 != ulValueLen && 256 != ulValueLen)
	{
		SYNO_LogE("CKA_PRIVATE_EXPONENT attribute data length is wrong! ulValueLen = 0x%x", ulValueLen);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// ���㹫Կbits(��ֱ�ӻ�ȡCKA_MODULUS_BITS���ԣ���Ϊ�ò���Ϊ��ѡ)
	stRsaPriKey.BitLen = ulValueLen * 8;
	memcpy(stRsaPriKey.PrivateExponent + (MAX_RSA_MODULUS_LEN - stRsaPriKey.BitLen/8), btData, ulValueLen);
	
	// ��ȡ˽Կָ��Prime1
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_PRIME_1, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_PRIME_1 failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Prime1 + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);
	
	// ��ȡ˽Կָ��Prime2
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_PRIME_2, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_PRIME_2 failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Prime2 + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);
	
	// ��ȡ˽Կָ��Prime1Exponent
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_EXPONENT_1, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_EXPONENT_1 failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Prime1Exponent + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);
	
	// ��ȡ˽Կָ��Prime2Exponent
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_EXPONENT_2, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_EXPONENT_2 failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Prime2Exponent + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);

	// ��ȡ˽Կָ��Coefficient
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_COEFFICIENT, btData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_COEFFICIENT failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	memcpy(stRsaPriKey.Coefficient + (MAX_RSA_PRIME_LEN - stRsaPriKey.BitLen/16), btData, ulValueLen);

	// ��ȡ˽Կ��;
	ulValueLen = sizeof(BYTE);
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_SIGN, (CK_BYTE_PTR)&bIsSign, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_SIGN failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_UNWRAP, (CK_BYTE_PTR)&bIsExch, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_UNWRAP failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_ENCRYPT, (CK_BYTE_PTR)&bIsEncrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ENCRYPT failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_DECRYPT, (CK_BYTE_PTR)&bIsDecrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_DECRYPT failed! ulRet = 0x%x", ulRet);
	}
	if (!bIsSign && !bIsExch && !bIsEncrypt && !bIsDecrypt)
	{
		SYNO_LogE("All of CKA_VERIFY, CKA_UNWRAP, CKA_ENCRYPT and CKA_DECRYPT attribute is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// ���˽Կ��;��Ϣ
	if (bIsSign) SYNO_LogD("This private key can be use to sign data.");
	if (bIsExch) SYNO_LogD("This private key can be use to unwrap session key.");
	if (bIsEncrypt) SYNO_LogD("This private key can be use to encrypt data.");
	if (bIsDecrypt) SYNO_LogD("This private key can be use to decrypt data.");

	// ������,�Ȼ�ȡCKA_ID����ֵ,���û�и������򴴽�һ��������
	ulValueLen = 64;
	ulRet = CI_ObjGetAttributeValue(pPriKeyObj, CKA_ID, (CK_BYTE_PTR)csContainer, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ID failed!ulRet = 0x%x", ulRet);
	
		// ��������
		do
		{
			lpNewContainer = TOK_I_GenUniqeContainerId();
			ulRet = P11_CreateContainer(hApp, lpNewContainer, &hContainer);
		}while (SAR_COS_CONTAINER_ALREADY_EXISTS == ulRet);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		SYNO_LogD("New container: %s is created��", lpNewContainer);
		CI_ObjSetAttributeValue(pPriKeyObj, CKA_ID, (CK_BYTE_PTR)lpNewContainer, strlen(lpNewContainer));
	}
	else
	{
		SYNO_LogD("Try to open and create the set container: %s��", csContainer);
		ulRet = P11_OpenContainer(hApp, csContainer, &hContainer);
		if (SAR_OK != ulRet)
		{
			ulRet = P11_CreateContainer(hApp, csContainer, &hContainer);
			if (SAR_OK != ulRet)
			{
				SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
				return CKR_GENERAL_ERROR;
			}
		}
	}

	// ����˽Կ
	ulRet = P11_ImportPlainRSAPriKey(hContainer, bIsSign ? 1 : 0, &stRsaPriKey);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_ImportPlainRSAKeyPair() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}
	pPriKeyObj->CtrHandle = hContainer;

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	����:TOK_I_ImportSessionKey
 *	˵��:���Գ���Կ���ݵ��뵽Key��
 *	����:hDev:[IN]:�豸������
 *		 pKeyObj:[IN]:�Գ���Կ����ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static CK_RV TOK_I_ImportSessionKey(DEVHANDLE hDev,
									CK_I_OBJ_PTR pKeyObj)
{
	ULONG ulRet = 0;
	ULONG ulAlgID = 0;
	ULONG ulKeyLen = 0;
	ULONG ulValueLen = 0;
	BYTE* lpKeyData = NULL;	
	HANDLE hKey = NULL;

	FUNC_BEGIN;
		
	// �������
	if (NULL_PTR == hDev)
	{
		SYNO_LogE("hDev is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pKeyObj)
	{
		SYNO_LogE("pKeyObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	// ��ȡKey���㷨����
	ulValueLen = 4;
	ulRet = CI_ObjGetAttributeValue(pKeyObj, CKA_KEY_TYPE, (CK_BYTE_PTR)&ulAlgID, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_KEY_TYPE failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	switch (ulAlgID)
	{
	case CKK_AES:
		ulAlgID = SGD_AES128;
		break;
	case CKK_DES:
		ulAlgID = SGD_DES;
		break;
	case CKK_DES3:
		ulAlgID = SGD_DES3;
		break;
	default:
		SYNO_LogE("CKA_KEY_TYPE is not suppored! ulAlgID = 0x%x", ulAlgID);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	SYNO_LogD("Coverted alg id:0x%x", ulAlgID);
	
	// ��ȡKey����
	ulRet = CI_ObjGetAttributeValue(pKeyObj, CKA_VALUE_LEN, (CK_BYTE_PTR)&ulKeyLen, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE_LEN failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (ulKeyLen == 0)
	{
		SYNO_LogE("CKA_VALUE_LEN value is 0!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// ����ռ�
	lpKeyData = (BYTE*)SYNO_malloc(ulKeyLen);
	if (NULL == lpKeyData)
	{
		SYNO_LogE("SYNO_malloc() return failed!");
		return CKR_HOST_MEMORY;
	}

	// ��ȡKey����
	ulValueLen = ulKeyLen;
	ulRet = CI_ObjGetAttributeValue(pKeyObj, CKA_VALUE, lpKeyData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE failed! ulRet = 0x%x", ulRet);
		ulRet = ulRet;
		goto FAILED;
	}

	// ����Ự��Կ
	ulRet = P11_SetSymmKey(hDev, lpKeyData, ulAlgID, &hKey);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_SetSymmKey() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto FAILED;
	}
	pKeyObj->ObjHandle = hKey;

	FUNC_END;

FAILED:
	if (lpKeyData)
	{
		SYNO_free(lpKeyData);
		lpKeyData = NULL;
	}

	return ulRet;
}
/***********************************************************************
 *	����:TOK_I_ImportCertificate
 *	˵��:��֤��������ݵ��뵽Key��
 *	����:hApp:[IN]:Ӧ�þ��
 *       pObj:[IN]:����ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static CK_RV TOK_I_ImportCertificate(HAPPLICATION hApp, 
									 CK_I_OBJ_PTR pCertObj)
{
	ULONG ulRet = 0;
	ULONG ulValueLen = 0;
	ULONG ulCertDataLen = 0;
	BYTE* pCertData = NULL;
	CHAR csContainer[64] = {0};
	BOOL bIsSign = FALSE;
	BOOL bIsExch = FALSE;
	LPSTR lpNewContainer = NULL;
	HCONTAINER hContainer = NULL;

	FUNC_BEGIN;
		
	// �������
	if (NULL_PTR == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pCertObj)
	{
		SYNO_LogE("pCertObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// ��ȡ֤����;
	ulValueLen = sizeof(BOOL);
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_SIGN, (CK_BYTE_PTR)&bIsSign, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_SIGN failed! ulRet = 0x%x", ulRet);
	}
	ulValueLen = sizeof(BOOL);
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_ENCRYPT, (CK_BYTE_PTR)&bIsExch, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ENCRYPT failed! ulRet = 0x%x", ulRet);
	}
	if ((!bIsSign && !bIsExch) || (bIsSign && bIsExch))
	{
		SYNO_LogE("Certificate usage type is wrong! bIsSign = %d, bIsExch = %d", bIsSign, bIsExch);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	else
	{
		if (bIsSign) SYNO_LogD("This is a sign certificate!");
		if (bIsExch) SYNO_LogD("This is a exchange certificate!");
	}

	// ��ȡ֤�����ݳ���
	ulValueLen = 64;
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_VALUE_LEN, (CK_BYTE_PTR)&ulCertDataLen, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE_LEN failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (ulCertDataLen == 0)
	{
		SYNO_LogE("CKA_VALUE_LEN value is 0!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// ����ռ�
	pCertData = (BYTE*)SYNO_malloc(ulValueLen);
	if (NULL == pCertData)
	{
		SYNO_LogE("SYNO_malloc() return failed!");
		return CKR_HOST_MEMORY;
	}

	// ��ȡKey����
	ulValueLen = ulCertDataLen;
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_VALUE, pCertData, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE failed! ulRet = 0x%x", ulRet);
		goto FAILED;
	}
	
	// ������,�Ȼ�ȡCKA_ID����ֵ,���û�и������򴴽�һ��������
	ulValueLen = 64;
	ulRet = CI_ObjGetAttributeValue(pCertObj, CKA_ID, (CK_BYTE_PTR)csContainer, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ID failed!ulRet = 0x%x", ulRet);
	
		// ��������
		do
		{
			lpNewContainer = TOK_I_GenUniqeContainerId();
			ulRet = P11_CreateContainer(hApp, lpNewContainer, &hContainer);
		}while (SAR_COS_CONTAINER_ALREADY_EXISTS == ulRet);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		SYNO_LogD("New container: %s is created��", lpNewContainer);
		CI_ObjSetAttributeValue(pCertObj, CKA_ID, (CK_BYTE_PTR)lpNewContainer, strlen(lpNewContainer));
	}
	else
	{
		SYNO_LogD("The set container: %s is created��", csContainer);
		ulRet = P11_OpenContainer(hApp, csContainer, &hContainer);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_OpenContainer() failed! ulRet = 0x%x", ulRet);			
			return CKR_ATTRIBUTE_VALUE_INVALID;
		}
	}

	// ����֤��
	ulRet = P11_ImportCertificate(hContainer, bIsSign, pCertData, ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_ImportCertificate() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto FAILED;
	}
	pCertObj->CtrHandle = hContainer;

	FUNC_END;

FAILED:
	if (pCertData)
	{
		SYNO_free(pCertData);
		pCertData = NULL;
	}
	
	//CKA_ID:������
	//CKA_LABEL:ʹ����
	//CKA_SUBJECT:Subject
	//CKA_ISSUER:Issuer
	//CKA_SERIAL_NUMBER:SN

	return ulRet;
}
/***********************************************************************
 *	����:TOK_I_ImportDataFile
 *	˵��:��֤��������ݵ��뵽Key��
 *	����:hApp:[IN]:Ӧ�þ��
 *       pObj:[IN]:����ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static CK_RV TOK_I_ImportDataFile(HAPPLICATION hApp, 
								  CK_I_OBJ_PTR pDataObj)
{
	ULONG ulRet = 0;
	ULONG ulFileSize = 0;
	ULONG ulValueLen = 256;
	CHAR csFileName[256] = {0};
	
	FUNC_BEGIN;

	// �������
	if (NULL_PTR == hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (NULL_PTR == pDataObj)
	{
		SYNO_LogE("pDataObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
		
	// �ļ���
	ulValueLen = 256;
	ulRet = CI_ObjGetAttributeValue(pDataObj, CKA_ID, (CK_BYTE_PTR)csFileName, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ID failed! ulRet = 0x%x", ulRet);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// ��ȡ�ļ���С
	ulValueLen = sizeof(ULONG);
	ulRet = CI_ObjGetAttributeValue(pDataObj, CKA_VALUE_LEN, (CK_BYTE_PTR)&ulFileSize, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VALUE_LEN failed! ulRet = 0x%x", ulRet);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// ���������ļ�
	ulRet = P11_CreateFile(hApp, csFileName, ulFileSize, SECURE_USER_ACCOUNT, SECURE_USER_ACCOUNT);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_CreateFile() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_I_DeleteObjContainer
 *	˵��:ɾ���������ڵ�����
 *	����:hApp:[IN]:Ӧ�þ��
 *       pObj:[IN]:����ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
static CK_RV TOK_I_DeleteObjContainer(HAPPLICATION hApp, 
									  CK_I_OBJ_PTR pObj)
{
	ULONG ulRet = 0;
	CK_BYTE csObjContainer[64] = {0};
	CK_ULONG ulContainerLen = 64;
	
	// �������
	if (!hApp)
	{
		SYNO_LogE("hApp is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (!pObj)
	{
		SYNO_LogE("pObj is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	// �ر�����
	if (pObj->CtrHandle)
	{
		ulRet = P11_CloseContainer(pObj->CtrHandle);
		if (CKR_OK != ulRet)
		{
			SYNO_LogE("P11_CloseContainer() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}

	// ��ȡ������
	ulRet = CI_ObjGetAttributeValue(pObj, CKA_ID, csObjContainer, &ulContainerLen); 
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("CI_ObjGetIntAttributeValue() to get CKA_ID value failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}

	// ɾ������
	ulRet = P11_DeleteContainer(hApp, (LPSTR)csObjContainer);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("P11_DeleteContainer() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}

	return CKR_OK;
}


#if 0
static ULONG ICA_CopyObject(CK_I_HASHTABLE_PTR pstDestObjList,
    CK_I_HASHTABLE_PTR pstSrcObjList)
{
    CK_I_HASH_ITERATOR_PTR pIter;
    CK_ULONG key;
    CK_I_OBJ_PTR val;
    CK_RV rv = CKR_OK;

    /* copy all persistent objects into the session object list */
    for(CI_HashIterateInit(pstSrcObjList,&pIter);
        CI_HashIterValid(pIter); CI_HashIterateInc(pIter))
    {
        rv = CI_HashIterateDeRef(pIter,&key,(CK_VOID_PTR_PTR)&val);
        if(rv != CKR_OK)
        {
            return rv;
        }

        rv = CI_ContainerAddObj(pstDestObjList,key,val);
        if(rv != CKR_OK)
        {
            return rv;
        }
    }

    return rv;
}

ICA_STATIC ULONG ICA_GetObjectHandle(CK_I_SLOT_DATA_PTR pstSlotData, ULONG ulObjectType,
    CK_OBJECT_HANDLE_PTR phObject, CK_ULONG_PTR pulObjectCount)
{
	int i = 0;
    ULONG ulRet = CKR_OK;
    CK_ATTRIBUTE_PTR pstFindTemplate = NULL;

    if (NULL == pstSlotData)
    {
        return CKR_GENERAL_ERROR;
    }

	/* malloc template buffer */
	pstFindTemplate = (CK_ATTRIBUTE_PTR)SYNO_calloc(1, sizeof(g_astFindRSATemplate));
	if (NULL == pstFindTemplate)
	{
        return CKR_HOST_MEMORY;
	}
	memcpy(pstFindTemplate, g_astFindRSATemplate, sizeof(g_astFindRSATemplate));

	/* set template class attribute value based on key type */
	for (i = 0; i < CK_I_RSA_FIND_ATTR_COUNT; i++)
	{
		if (pstFindTemplate[i].type == CKA_CLASS)
		{
			pstFindTemplate[i].ulValueLen = sizeof(CK_OBJECT_CLASS);
			pstFindTemplate[i].pValue = (CK_OBJECT_CLASS_PTR)SYNO_calloc(1, sizeof(CK_OBJECT_CLASS));
			*((CK_OBJECT_CLASS_PTR)pstFindTemplate[i].pValue) = (ICA_OBJECT_TYPE_PUBLICE == ulObjectType) ? CKO_PUBLIC_KEY : CKO_PRIVATE_KEY;
			break;
		}
	}

    ulRet = cloudshield_FindObjectsInit(pstSlotData, g_astFindRSATemplate, CK_I_RSA_FIND_ATTR_COUNT);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

	ulRet = cloudshield_FindObjects(pstSlotData, phObject, ICA_FIND_OBJECT_MAX, pulObjectCount);
	if (ulRet != CKR_OK)
	{
		goto End;
	}

	ulRet = cloudshield_FindObjectsFinal(pstSlotData);
	if (ulRet != CKR_OK)
	{
		goto End;
	}

End:
	for (i = 0; i < CK_I_RSA_FIND_ATTR_COUNT; i++)
	{
		if (pstFindTemplate[i].type == CKA_CLASS)
		{
			ICA_FREE(pstFindTemplate[i].pValue);
			pstFindTemplate[i].pValue = NULL;
		}
	}
	ICA_FREE(pstFindTemplate);
	pstFindTemplate = NULL;
    return ulRet;
}



ICA_STATIC ULONG ICA_FillPublicKeyTemplate(CK_I_SLOT_DATA_PTR pstSlotData, CK_I_OBJ_PTR pstPublicObject)
{
	int i = 0;
	ULONG ulRet = CKR_OK;
	ULONG ulTemplateCnt = 0;
    CK_ATTRIBUTE_PTR pstPublicKeyTemplate = NULL;

    if ((NULL == pstSlotData) || (NULL == pstPublicObject))
    {
		return CKR_ARGUMENTS_BAD;
    }

	/* malloc attributes buffer */
	pstPublicKeyTemplate = (CK_ATTRIBUTE_PTR)SYNO_calloc(1, sizeof(g_astGetRSAPublicKeyTemplate));
	if (NULL == pstPublicKeyTemplate)
	{
		return CKR_HOST_MEMORY;
	}
	memcpy(pstPublicKeyTemplate, g_astGetRSAPublicKeyTemplate, sizeof(g_astGetRSAPublicKeyTemplate));
	ulTemplateCnt = sizeof(g_astGetRSAPublicKeyTemplate)/sizeof(g_astGetRSAPublicKeyTemplate[0]);

	/* get attributes value length */
	ulRet = cloudshield_GetAttributeValue(pstSlotData, pstPublicObject->obj_id,	pstPublicKeyTemplate, ulTemplateCnt);
	if (ulRet != CKR_OK)
    {
    	return ulRet;
    }

	/* malloc buffer for attributes value */
	for (i = 0; i < ulTemplateCnt; i++)
	{
		if (NULL == pstPublicKeyTemplate[i].pValue)
		{
			pstPublicKeyTemplate[i].pValue = (CK_BYTE_PTR)SYNO_calloc(1, pstPublicKeyTemplate[i].ulValueLen);
		}
	}

	/* get attributes value now */
	ulRet = cloudshield_GetAttributeValue(pstSlotData, pstPublicObject->obj_id,	pstPublicKeyTemplate, ulTemplateCnt);
	if (ulRet != CKR_OK)
    {
    	return ulRet;
    }

	/* fill attributes to object */
	for (i = 0; i < ulTemplateCnt; i++)
	{
		if (NULL != pstPublicKeyTemplate[i].pValue)
		{
			CI_ObjSetAttributeValue(pstPublicObject, pstPublicKeyTemplate[i].type, pstPublicKeyTemplate[i].pValue, pstPublicKeyTemplate[i].ulValueLen);

			if (pstPublicKeyTemplate[i].type == CKA_MODULUS)
			{
				ICA_FREE(pstPublicKeyTemplate[i].pValue);
				pstPublicKeyTemplate->pValue = NULL;
				pstPublicKeyTemplate->ulValueLen = 0;
			}
		}
	}

	/* free attributes buffer */
	ICA_FREE(pstPublicKeyTemplate);
	pstPublicKeyTemplate = NULL;

	return CKR_OK;
}

ICA_STATIC ULONG ICA_GetTokenObject(CK_I_SESSION_DATA_PTR pstSessionData, ULONG ulObjectType)
{
	ULONG ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    ULONG aulHObject[ICA_FIND_OBJECT_MAX] = {0};
    CK_ULONG ulObjectCount = 0;
    CK_I_OBJ_PTR pstPublicObject = NULL_PTR;
    CK_ATTRIBUTE_PTR pstKeyTemplate = NULL;
    ULONG ulKeyTemplateCount = 0;

    ULONG ulKey = 0;
    int i, j;

    if ((NULL == pstSessionData) || (NULL == pstSessionData->slot_data))
    {
        return CKR_ARGUMENTS_BAD;
    }

    pstSlotData = pstSessionData->slot_data;

    ulRet = ICA_GetObjectHandle(pstSlotData, ulObjectType, aulHObject, &ulObjectCount);
    if (ulRet != CKR_OK)
	{
		return ulRet;
	}

	if (0 == ulObjectCount)
	{
		return CKR_GENERAL_ERROR;
	}

    switch(ulObjectType)
    {
        case ICA_OBJECT_TYPE_PUBLICE:
        {
            pstKeyTemplate = (CK_ATTRIBUTE_PTR)SYNO_calloc(1, sizeof(g_astRSAPublicKeyTemplate));
            if (NULL == pstKeyTemplate)
        	{
                return CKR_HOST_MEMORY;
        	}
            memcpy(pstKeyTemplate, g_astRSAPublicKeyTemplate, sizeof(g_astRSAPublicKeyTemplate));
            ulKeyTemplateCount = CK_I_RSA_PUBLIC_KEY_ATTR_COUNT;
            break;
        }
        case ICA_OBJECT_TYPE_PRIVATE:
        {
            pstKeyTemplate = (CK_ATTRIBUTE_PTR)SYNO_calloc(1, sizeof(g_astRSAPrivateKeyTemplate));
            if (NULL == pstKeyTemplate)
        	{
				return CKR_HOST_MEMORY;
        	}
            memcpy(pstKeyTemplate, g_astRSAPrivateKeyTemplate, sizeof(g_astRSAPrivateKeyTemplate));
            ulKeyTemplateCount = CK_I_RSA_PRIVATE_KEY_ATTR_COUNT;
            break;
        }
        default:
        {
            return CKR_GENERAL_ERROR;
        }
    }

    for (i = 0; i < ulObjectCount; i++)
    {
		/* create and initialize object */
		pstPublicObject = NULL_PTR;
        ulRet = CI_ObjTemplateInit(&pstPublicObject, pstKeyTemplate, ulKeyTemplateCount);
        if (ulRet != CKR_OK)
    	{
    		goto End;
    	}

        pstPublicObject->obj_id = aulHObject[i];
		pstPublicObject->session = pstSessionData;

		/* for public key, get its attributes (MODULUS, BITS and EXPONENT) */
		if (ICA_OBJECT_TYPE_PUBLICE == ulObjectType)
		{
			//ICA_FillPublicKeyTemplate(pstSlotData, pstPublicObject);
		}

        ulRet = CI_NewHandle(&ulKey);
        if(ulRet != CKR_OK)
        {
            goto End;
        }

        ulRet = CI_ContainerAddObj(pstSlotData->token_data->object_list, ulKey, pstPublicObject);
        if(ulRet != CKR_OK)
        {
			CI_ObjDestroyObj(pstPublicObject);
			pstPublicObject = NULL;
            goto End;
        }
    }

End:
	ICA_FREE(pstKeyTemplate);
	pstKeyTemplate = NULL;
    return ulRet;
}
#endif

ULONG ICA_CreatePubKeyObj(CK_I_SESSION_DATA_PTR pstSessionData,
    CHAR *pcContainerName, HCONTAINER hContainer)
{
    ULONG ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CK_I_OBJ_PTR pstPublicObject = NULL_PTR;
    RSAPUBLICKEYBLOB stRSAPubBlob = {0};
    ULONG ulBlobLen = 0;
    CK_ATTRIBUTE_PTR pstTemplate = NULL;
    HANDLE hKey = NULL;
    ULONG ulKey = 0;
    CHAR *pcLabel = NULL;

    /* ������� */
    if ((NULL == pstSessionData)
        || (NULL == hContainer))
    {
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = pstSessionData->slot_data;
    if (NULL == pstSlotData)
    {
        return CKR_GENERAL_ERROR;
    }

    ulRet = CI_ObjTemplateInit(&pstPublicObject,
        g_astRSAPublicKeyTemplate, CK_I_RSA_PUBLIC_KEY_ATTR_COUNT);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

    ulBlobLen = sizeof(stRSAPubBlob);
    ulRet = P11_ExportPublicKey(hContainer, FALSE, (BYTE *)&stRSAPubBlob, &ulBlobLen);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPublicObject, CK_IA_ID));
    pstTemplate->pValue = SYNO_calloc(1, strlen((char *)pcContainerName) + 1);
    strncpy((char*)pstTemplate->pValue, pcContainerName, strlen((char *)pcContainerName));
    pstTemplate->ulValueLen = strlen((char *)pcContainerName) + 1;

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPublicObject, CK_IA_MODULUS));
    pstTemplate->pValue = SYNO_calloc(1, MAX_RSA_MODULUS_LEN);
    memcpy(pstTemplate->pValue, stRSAPubBlob.Modulus, MAX_RSA_MODULUS_LEN);
    pstTemplate->ulValueLen = MAX_RSA_MODULUS_LEN;

    //ulRet = P11_GetRSAKeyHandle(hContainer, AT_KEYEXCHANGE, &hKey);
    //if (ulRet != CKR_OK)
	//{
	//	goto End;
	//}

    pstPublicObject->ObjHandle = hKey;
    pstPublicObject->CtrHandle = hContainer;
	pstPublicObject->session = pstSessionData;

    ulRet = CI_NewHandle(&ulKey);
    if(ulRet != CKR_OK)
    {
        goto End;
    }

    ulRet = CI_ContainerAddObj(pstSlotData->token_data->object_list, ulKey, pstPublicObject);
    if(ulRet != CKR_OK)
    {
		CI_ObjDestroyObj(pstPublicObject);
		pstPublicObject = NULL;
        goto End;
    }

End:
    return ulRet;
}

ULONG ICA_CreatePrivKeyObj(CK_I_SESSION_DATA_PTR pstSessionData,
    CHAR *pcContainerName, HCONTAINER hContainer)
{
    ULONG ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CK_I_OBJ_PTR pstPrivObject = NULL_PTR;
    RSAPUBLICKEYBLOB stRSAPubBlob = {0};
    ULONG ulBlobLen = 0;
    CK_ATTRIBUTE_PTR pstTemplate = NULL;
    HANDLE hKey = NULL;
    ULONG ulKey = 0;
    CHAR *pcLabel = NULL;

    /* ������� */
    if ((NULL == pstSessionData)
        || (NULL == hContainer))
    {
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = pstSessionData->slot_data;
    if (NULL == pstSlotData)
    {
        return CKR_GENERAL_ERROR;
    }

    ulRet = CI_ObjTemplateInit(&pstPrivObject,
        g_astRSAPrivateKeyTemplate, CK_I_RSA_PRIVATE_KEY_ATTR_COUNT);
    if (ulRet != CKR_OK)
	{
		goto End;
	}

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPrivObject, CK_IA_ID));
    pstTemplate->pValue = SYNO_calloc(1, strlen((char *)pcContainerName) + 1);
    strncpy((char*)pstTemplate->pValue, (char *)pcContainerName, strlen((char *)pcContainerName));
    pstTemplate->ulValueLen = strlen((char *)pcContainerName) + 1;

    pstPrivObject->ObjHandle = hKey;
    pstPrivObject->CtrHandle = hContainer;
	pstPrivObject->session = pstSessionData;

    ulRet = CI_NewHandle(&ulKey);
    if(ulRet != CKR_OK)
    {
        goto End;
    }

    ulRet = CI_ContainerAddObj(pstSlotData->token_data->object_list, ulKey, pstPrivObject);
    if(ulRet != CKR_OK)
    {
		CI_ObjDestroyObj(pstPrivObject);
		pstPrivObject = NULL;
        goto End;
    }

End:
    return ulRet;
}

ULONG ICA_CheckName(CHAR *pcNameList, CHAR *pcName)
{
    ULONG ulRet = CKR_OK;
    CHAR acList[PATH_MAX] = {0};
    CHAR *pcCheckList = NULL;
    CHAR *pcRemainder = NULL;
    CHAR *pcCheckName = NULL;

    TOK_I_StrFmtList(acList, pcNameList);

    pcCheckList = (CHAR *)SYNO_malloc(strlen((char *)acList) + 1);
	strcpy(pcCheckList, acList);

    pcCheckName = (CHAR *)TC_strtok_r(pcCheckList, ",", &pcRemainder);
    while(pcCheckName != NULL)
    {
        ulRet = strcmp(pcCheckName, pcName);
        if (ulRet == 0)
        {
            break;
        }

        pcCheckName = TC_strtok_r(NULL, ",", &pcRemainder);
    }

    SYNO_free(pcCheckList);

    return ulRet;
}



/***********************************************************************
 *	����:TOK_EnumToken
 *	˵��:�����ҵ���Token���б�
 *	����:pucTokenList:[OUT]:���Token�����ַ�ָ�룬ÿ��Token��֮����'\0'�ָ��2��'\0'��ʾ������
 *       pulSize:[OUT]:���������б�buffer�ĳ��ȡ�
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_EnumToken(CK_CHAR_PTR pucTokenList, 
					ULONG *pulSize)
{
    ULONG ulRet = CKR_OK;
    UCHAR aucNameList[PATH_MAX] = {0};
	ULONG ulSize = PATH_MAX;
	UCHAR *pucTmp = NULL;
	ULONG ulTmpLen = 0;
	ULONG ulLen = 0;

    FUNC_BEGIN;

    // �������
    if ((NULL == pucTokenList) || (NULL == pulSize))
    {
		SYNO_LogE("pucTokenList or pulSize is NULL!");
        return CKR_GENERAL_ERROR;
    }

    // ö���豸
    ulRet = P11_EnumDev(TRUE, (LPSTR)aucNameList, &ulSize);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_EnumDev() failed! ulRet = 0x%x", ulRet);
        goto End;
    }
	
    TOK_I_StrFmtList((CHAR *)pucTokenList, (CHAR *)aucNameList);

End:
    FUNC_END;
    return ulRet;
}
/***********************************************************************
 *	����:TOK_TokenInitialize
 *	˵��:��ʼ��һ��Token���������
 *	����:pucTokenName:[IN]:Token��
 *       ppSlotData:[OUT]:���س�ʼ������������
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_TokenInitialize(CK_CHAR_PTR pucTokenName,
						  CK_I_SLOT_DATA_PTR CK_PTR ppSlotData)
{
    ULONG ulRet = CKR_GENERAL_ERROR;
    CK_SLOT_INFO_PTR pstSlotInfo = NULL;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CK_TOKEN_INFO_PTR pstTokenInfo = NULL;
    CK_I_TOKEN_DATA_PTR pstTokenData = NULL;
	CK_UTF8CHAR_PTR pUTF8 = NULL;
    DEVHANDLE hDev = NULL;
    DEVINFO DevInfo = {0};

    FUNC_BEGIN;
	
    // �����豸
    ulRet = P11_ConnectDev((CHAR *)pucTokenName, &hDev);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_ConnectDev() failed! ulRet = 0x%x", ulRet);
        return ulRet;
    }

    // ��ȡ�豸��Ϣ
    ulRet = P11_GetDevInfo(hDev, &DevInfo);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_GetDevInfo() failed! ulRet = 0x%x", ulRet);
        return ulRet;
    }

	// ����Slot Info
    pstSlotInfo = (CK_SLOT_INFO_PTR)SYNO_calloc(1, sizeof(CK_SLOT_INFO));
    if (NULL == pstSlotInfo)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto Err;
    }
	memset(pstSlotInfo, 0, sizeof(CK_SLOT_INFO));

    strcpy((char*)pstSlotInfo->slotDescription, "Syno TF Key");
	pUTF8 = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(DevInfo.Manufacturer);
	memcpy(pstSlotInfo->manufacturerID, pUTF8, strlen((char*)pUTF8));
	SYNO_free(pUTF8);
	//
	pstSlotInfo->hardwareVersion.major = DevInfo.HWVersion.major;
	pstSlotInfo->hardwareVersion.minor = DevInfo.HWVersion.minor;
	pstSlotInfo->firmwareVersion.major = DevInfo.FirmwareVersion.major;
    pstSlotInfo->firmwareVersion.minor = DevInfo.FirmwareVersion.minor;

    pstSlotData = (CK_I_SLOT_DATA_PTR)SYNO_calloc(1, sizeof(CK_I_SLOT_DATA));
    if (NULL == pstSlotData)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto FreeSlotInfo;
    }
	memset(pstSlotData, 0, sizeof(CK_I_SLOT_DATA));

    pstSlotData->flags = 0;
    pstSlotData->slot_info = pstSlotInfo;
    pstSlotData->config_section_name = (UCHAR *)SYNO_calloc(1, strlen((char *)pucTokenName) + 1);
    if (NULL == pstSlotData->config_section_name)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto FreeSlotData;
    }
	memset(pstSlotData->config_section_name, 0, strlen((char *)pucTokenName) + 1);
    strcpy((CHAR *)pstSlotData->config_section_name, (CHAR *)pucTokenName);

    pstTokenInfo = (CK_TOKEN_INFO_PTR)SYNO_calloc(1, sizeof(CK_TOKEN_INFO));
    if (NULL == pstTokenInfo)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto FreeSectionName;
    }
	memset(pstTokenInfo, 0, sizeof(CK_TOKEN_INFO));
	
	pUTF8 = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(DevInfo.Label);
	memcpy(pstTokenInfo->label, pUTF8, strlen((char*)pUTF8));
	SYNO_free(pUTF8);
	pUTF8 = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(DevInfo.Manufacturer);
	memcpy(pstTokenInfo->manufacturerID, pUTF8, strlen((char*)pUTF8));
	SYNO_free(pUTF8);
	pUTF8 = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(DevInfo.SerialNumber);
	memcpy(pstTokenInfo->serialNumber, pUTF8, strlen((char*)pUTF8));
	SYNO_free(pUTF8);
	//
    pstTokenInfo->ulMinPinLen = TOK_MIN_PIN_LEN;
    pstTokenInfo->ulMaxPinLen = TOK_MAX_PIN_LEN;
    pstTokenInfo->ulMaxSessionCount = TOK_MAX_SESSION_COUNT;
    pstTokenInfo->ulMaxRwSessionCount = TOK_MAX_RW_SESSION_COUNT;
	pstTokenInfo->ulTotalPublicMemory = DevInfo.TotalSpace;
	pstTokenInfo->ulFreePublicMemory = DevInfo.FreeSpace;
	pstTokenInfo->hardwareVersion.major = DevInfo.HWVersion.major;
	pstTokenInfo->hardwareVersion.minor = DevInfo.HWVersion.minor;
	pstTokenInfo->firmwareVersion.major = DevInfo.FirmwareVersion.major;
    pstTokenInfo->firmwareVersion.minor = DevInfo.FirmwareVersion.minor;

    pstSlotData->pDevHandle = hDev;

    pstSlotInfo->flags = CKF_TOKEN_PRESENT;

    pstTokenData = (CK_I_TOKEN_DATA_PTR)SYNO_calloc(1, sizeof(CK_I_TOKEN_DATA));
    if (NULL == pstTokenData)
    {
		SYNO_LogE("SYNO_calloc() failed!");
        goto FreeTokenInfo;
    }
	memset(pstTokenData, 0, sizeof(CK_I_TOKEN_DATA));

	// �����㷨��ʾ
	pstTokenData->asymalg_flags = DevInfo.AlgAsymCap;
	pstTokenData->symalg_flags = DevInfo.AlgSymCap;
	pstTokenData->hashalg_flags = DevInfo.AlgHashCap;

	// ����Token������ַ
	if (s_ck_i_token_methods.GetTokenInfo == NULL)
	{
		TOK_I_SetFunctionList();
	}
	pstSlotData->methods = &s_ck_i_token_methods;

    pstTokenData->token_info = pstTokenInfo;
    pstSlotData->token_data = pstTokenData;
    *ppSlotData = pstSlotData;

    FUNC_END;
    return CKR_OK;

FreeTokenInfo:
    SYNO_free(pstTokenInfo);
FreeSectionName:
    SYNO_free(pstSlotData->config_section_name);
FreeSlotData:
    SYNO_free(pstSlotData);
FreeSlotInfo:
    SYNO_free(pstSlotInfo);
Err:
    FUNC_END;
    return ulRet;
}
/***********************************************************************
 *	����:TOK_GetTokenInfo
 *	˵��:��ȡһ��Token����Ϣ
 *	����:slot_data:[IN]:��Token����Ӧ��Slot����
 *       pInfo:[OUT]:��ŷ���Token��Ϣ��ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_GetTokenInfo(CK_I_SLOT_DATA_PTR  slot_data, 
					   CK_TOKEN_INFO_PTR pInfo)
{
	ULONG ulRet = CKR_OK;
    CK_I_TOKEN_DATA_PTR token_data = NULL_PTR;

	FUNC_BEGIN;

	// ������
	if (slot_data == NULL_PTR)
	{
		SYNO_LogE("slot_data is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	if (pInfo == NULL_PTR)
	{
		SYNO_LogE("pInfo is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	// ���token data
    token_data = slot_data->token_data;
	if (token_data == NULL_PTR)
	{
		SYNO_LogE("slot_data->token_data is NULL!");
		return CKR_GENERAL_ERROR;
	}

	// ��������
    strncpy((char*)pInfo->label,           (char*)token_data->token_info->label, 32);
    strncpy((char*)pInfo->manufacturerID,  (char*)token_data->token_info->manufacturerID, 32);
    strncpy((char*)pInfo->model,           (char*)token_data->token_info->model, 16);
    strncpy((char*)pInfo->serialNumber,    (char*)token_data->token_info->serialNumber, 16);

    pInfo->flags                 = token_data->token_info->flags;
    pInfo->ulMaxSessionCount     = token_data->token_info->ulMaxSessionCount;
    pInfo->ulSessionCount        = token_data->token_info->ulSessionCount;
    pInfo->ulMaxRwSessionCount   = token_data->token_info->ulMaxRwSessionCount;
    pInfo->ulRwSessionCount      = token_data->token_info->ulRwSessionCount;
    pInfo->ulMaxPinLen           = token_data->token_info->ulMaxPinLen;
    pInfo->ulMinPinLen           = token_data->token_info->ulMinPinLen;
    pInfo->ulTotalPublicMemory   = token_data->token_info->ulTotalPublicMemory;
    pInfo->ulFreePublicMemory    = token_data->token_info->ulFreePublicMemory;
    pInfo->ulTotalPrivateMemory  = token_data->token_info->ulTotalPrivateMemory;
    pInfo->ulFreePrivateMemory   = token_data->token_info->ulFreePrivateMemory;
    pInfo->hardwareVersion.major = token_data->token_info->hardwareVersion.major;
    pInfo->hardwareVersion.minor = token_data->token_info->hardwareVersion.minor;
    pInfo->firmwareVersion.major = token_data->token_info->firmwareVersion.major;
    pInfo->firmwareVersion.minor = token_data->token_info->firmwareVersion.minor;

	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	����:TOK_GetMechanismList
 *	˵��:��ȡһ��Token��֧�ֵ��㷨�����б�
 *	����:pMechanismList:[OUT]:����㷨���͵�ָ��
 *       pulCount:[OUT]:������֧�ֵ��㷨���͸���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_GetMechanismList(CK_MECHANISM_TYPE_PTR pMechanismList, 
						   CK_ULONG_PTR pulCount)
{
	CK_ULONG types_count = 0;

	FUNC_BEGIN;

	// �������
	if (NULL_PTR == pulCount)
	{
		SYNO_LogE("pulCount is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	types_count = sizeof(s_ck_i_token_mechanism_types) / sizeof(s_ck_i_token_mechanism_types[0]);

	// ���ظ���
	if (pMechanismList)
	{
		*pulCount = types_count;
		SYNO_LogI("Return mechanism count:0x%x", types_count);
		return CKR_OK;
	}

	// ���Buffer�ռ��С
	if (*pulCount < types_count)
	{
		SYNO_LogI("Buffer is too small! Requested mechanism count:0x%x", types_count);
		return CKR_BUFFER_TOO_SMALL;
	}

	// �����㷨�б�
	memcpy(pMechanismList, s_ck_i_token_mechanism_types, sizeof(s_ck_i_token_mechanism_types));
	*pulCount = types_count;

	SYNO_LogI("Returned mechanism counts:0x%x", *pulCount);
	
	FUNC_END;

	return CKR_OK;
}
/***********************************************************************
 *	����:TOK_GetMechanismInfo
 *	˵��:��ȡĳ���㷨���͵ľ�����Ϣ
 *	����:type:[IN]:ָ�����㷨����
 *       pInfo:[OUT]:���ظ��㷨�������Ϣ
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_GetMechanismInfo(CK_MECHANISM_TYPE type, 
						   CK_MECHANISM_INFO_PTR pInfo)
{
	FUNC_BEGIN;

	// �������
	if (NULL_PTR == pInfo)
	{
		SYNO_LogE("pInfo is NULL!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// ���ظ����㷨����
	switch (type)
	{
	case CKM_RSA_PKCS_KEY_PAIR_GEN:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_GENERATE_KEY_PAIR;
		break;
	case CKM_RSA_PKCS:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_SIGN | CKF_VERIFY | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_MD5_RSA_PKCS:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_SIGN | CKF_VERIFY;
		break;
	case CKM_SHA1_RSA_PKCS:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_SIGN | CKF_VERIFY;
		break;
	case CKM_SHA256_RSA_PKCS:
		pInfo->ulMinKeySize = 0x400;
		pInfo->ulMaxKeySize = 0x800;
		pInfo->flags = CKF_HW | CKF_SIGN | CKF_VERIFY;
		break;		
	case CKM_AES_KEY_GEN:
		pInfo->ulMinKeySize = 0x80;
		pInfo->ulMaxKeySize = 0x80;
		pInfo->flags = CKF_HW | CKF_GENERATE;
		break;
	case CKM_AES_ECB:
		pInfo->ulMinKeySize = 0x80;
		pInfo->ulMaxKeySize = 0x80;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_AES_CBC:
		pInfo->ulMinKeySize = 0x40;
		pInfo->ulMaxKeySize = 0x40;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_DES_KEY_GEN:
		pInfo->ulMinKeySize = 0x40;
		pInfo->ulMaxKeySize = 0x40;
		pInfo->flags = CKF_HW | CKF_GENERATE;
		break;
	case CKM_DES_ECB:
		pInfo->ulMinKeySize = 0x40;
		pInfo->ulMaxKeySize = 0x40;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_DES_CBC:
		pInfo->ulMinKeySize = 0x40;
		pInfo->ulMaxKeySize = 0x40;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_DES3_KEY_GEN:
		pInfo->ulMinKeySize = 0xC0;
		pInfo->ulMaxKeySize = 0xC0;
		pInfo->flags = CKF_HW | CKF_GENERATE;
		break;
	case CKM_DES3_ECB:
		pInfo->ulMinKeySize = 0xC0;
		pInfo->ulMaxKeySize = 0xC0;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_DES3_CBC:
		pInfo->ulMinKeySize = 0xC0;
		pInfo->ulMaxKeySize = 0xC0;
		pInfo->flags = CKF_HW | CKF_ENCRYPT | CKF_DECRYPT | CKF_WRAP | CKF_UNWRAP;
		break;
	case CKM_MD5:
		pInfo->ulMinKeySize = 0x80;
		pInfo->ulMaxKeySize = 0x80;
		pInfo->flags = CKF_HW | CKF_DIGEST;
		break;
	case CKM_SHA_1:
		pInfo->ulMinKeySize = 0xA0;
		pInfo->ulMaxKeySize = 0xA0;
		pInfo->flags = CKF_HW | CKF_DIGEST;
		break;
	case CKM_SHA256:
		pInfo->ulMinKeySize = 0x100;
		pInfo->ulMaxKeySize = 0x100;
		pInfo->flags = CKF_HW | CKF_DIGEST;
		break;
	default:
		SYNO_LogE("This mechanism (0x%x) is not supported!");
		return CKR_MECHANISM_INVALID;
	};
	
	FUNC_END;

	return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_InitToken
 *	˵��:��ʼ��һ��Token��ɾ�������û����ݣ����½�һ����ʼ��Ӧ�á�
 *	����:slot_data:[IN]:��Token��Ӧ��Slot����
 *       pPin:[IN]:��ʼSO PIN
 *       ulPinLen:[IN]:SO PIN����
 *       pLabel:[IN]:Token��ǩ
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_InitToken(CK_I_SLOT_DATA_PTR slot_data,
					CK_CHAR_PTR pPin, 
					CK_ULONG ulPinLen, 
					CK_CHAR_PTR pLabel)
{
    ULONG ulRet = SAR_OK;
    CK_TOKEN_INFO_PTR pstTokenInfo = NULL;
    DEVHANDLE hDev = NULL;
    CHAR acApp[PATH_MAX] = {0};
    CHAR acAppList[PATH_MAX] = {0};
    CHAR acName[PATH_MAX] = {0};
    CHAR *pcAppNameList = NULL;
    CHAR *pcRemainder = NULL;
    CHAR *pcAppName = NULL;
    ULONG ulSize = PATH_MAX;
    HAPPLICATION hApp = NULL;
    HCONTAINER hContainer = NULL;
    ULONG ulRetryCount = 0;

	FUNC_BEGIN;

    // �������
    if ((NULL == slot_data)
        || (NULL == slot_data->token_data)
        || (NULL == pPin)
        || (NULL == pLabel))
    {
		SYNO_LogE("slot_data or slot_data->token_data or pPin or pLabel is NULL!");
        return CKR_GENERAL_ERROR;
    }

    // �������
    pstTokenInfo = slot_data->token_data->token_info;
    if (NULL == pstTokenInfo)
    {
		SYNO_LogE("pstTokenInfo is NULL!");
        return CKR_GENERAL_ERROR;
    }

    // ���PIN
    if ((ulPinLen < pstTokenInfo->ulMinPinLen)
        || (ulPinLen > pstTokenInfo->ulMaxPinLen))
    {
		SYNO_LogE("ulPinLen is wrong! ulSOPinLen = 0x%x", ulPinLen);
        return CKR_GENERAL_ERROR;
    }

    hDev = slot_data->pDevHandle;
	
	// �豸��֤(�ⲽ��������ʡ�Ե�)
    if (!P11_ExterAuth(hDev, NULL, 0))
    {
		SYNO_LogE("P11_DevAuth() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

    // ö��Ӧ��
    ulRet = P11_EnumApplication(hDev, acApp, &ulSize);
    if (ulRet != CKR_OK)
    {
		SYNO_LogE("P11_EnumApplication() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	// ɾ������Ӧ��
	TOK_I_StrFmtList(acAppList, acApp);
	pcAppNameList = (char*)SYNO_malloc(strlen((char *)acAppList) + 1);
	strcpy(pcAppNameList, acAppList);
	pcAppName = TC_strtok_r((char *)pcAppNameList, ",", &pcRemainder);
	while (pcAppName && strlen(pcAppName) > 0)
	{
		SYNO_LogD("Try to delete application:%s", pcAppName);
		ulRet = P11_DeleteApplication(hDev, pcAppName);
		if (ulRet != CKR_OK)
		{
			SYNO_LogE("P11_DeleteApplication() failed! ulRet = 0x%x", ulRet);
			goto End;
		}
		pcAppName = TC_strtok_r(NULL, ",", &pcRemainder);
	}

	// ������Ӧ��
	ulRet = P11_CreateApplication(hDev, TOK_INIT_APPNAME, (char*)pPin, TOK_SO_PIN_RETRY_COUNT, 
		TOK_INIT_USERPIN, TOK_USER_PIN_RETRY_COUNT, SECURE_ANYONE_ACCOUNT, &hApp);
	if (ulRet != CKR_OK)
	{
		SYNO_LogE("P11_CreateApplication() failed! ulRet = 0x%x", ulRet);
		goto End;
	}

	// ���ñ�ǩ
	ulRet = P11_SetLabel(hDev, (char*)pLabel);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("P11_SetLabel() failed! ulRet = 0x%x", ulRet);
		goto End;
	}

	FUNC_END;

End:
	if (pcAppNameList)
	{
		SYNO_free(pcAppNameList);
		pcAppNameList = NULL;
	}
	if (hApp)
	{
		P11_CloseApplication(hApp);
		hApp = NULL;
	}

    return ulRet;
}
 /***********************************************************************
 *	����:TOK_FinalizeToken
 *	˵��:�ͷ�һ��Token�������ͷ�������ص��ڴ����ݡ�
 *	����:slot_data:[IN]:��Token��Ӧ��Slot����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_FinalizeToken(CK_I_SLOT_DATA_PTR slot_data)
{
	FUNC_BEGIN;

    if (NULL == slot_data)
	{		
		SYNO_LogE("pstSlotData is NULL!");
		return CKR_GENERAL_ERROR;
	}

    if (NULL != slot_data->config_section_name)
    {
        SYNO_free(slot_data->config_section_name);
    }

    if (NULL != slot_data->pDevHandle)
    {
        (void)P11_DisConnectDev(slot_data->pDevHandle);
		slot_data->pDevHandle = NULL;
    }

    if (NULL != slot_data->slot_info)
    {
        SYNO_free(slot_data->slot_info);
    }

    if (NULL != slot_data->token_data)
    {
        if (NULL != slot_data->token_data->token_info)
        {
            SYNO_free(slot_data->token_data->token_info);
        }

        SYNO_free(slot_data->token_data);
    }

    SYNO_free(slot_data);

	FUNC_END;

	return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_InitPIN
 *	˵��:���ó�ʾ�û�PIN�롣
 *	����:session_data:[IN]:�Ự����
 *       pPin:[IN]:��ʼUser PIN
 *       ulPinLen:[IN]:User PIN����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_InitPIN(CK_I_SESSION_DATA_PTR session_data,
				  CK_CHAR_PTR pPin, 
				  CK_ULONG ulPinLen)
{
    CK_RV ulRet = CKR_OK;
    CK_ULONG ulRetryCount = 0;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;

	FUNC_BEGIN;

    // �������
    if ((NULL == session_data) || (NULL == pPin))
    {
        SYNO_LogE("session_data or pPin is null!");
        return CKR_GENERAL_ERROR;
    }

	// �޸�User PIN������TOK_InitPIN()�����ó�ʼUser PIN���������ʱ���Old User PINֻ����TOK_INIT_USERPIN
    ulRet = P11_ChangePIN (session_data->AppHandle, KT_USERPIN, TOK_INIT_USERPIN, (char*)pPin, &ulRetryCount);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_ChangePIN() failed! ulRet = 0x%x", ulRet);
		if (SAR_COS_PIN_LOCKED == ulRet || 0 == ulRetryCount)
		{
            return CKR_PIN_LOCKED;
		}

		return CKR_PIN_INCORRECT;
    }
    
	FUNC_END;

	return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_SetPIN
 *	˵��:�޸ĵ�ǰ��¼�û���PIN��
 *	����:session_data:[IN]:�Ự����
 *       pOldPin:[IN]:��PIN��
 *       ulOldLen:[IN]:��PIN�볤��
 *       pNewPin:[IN]:��PIN��
 *       ulNewLen:[IN]:��PIN�볤��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SetPIN(CK_I_SESSION_DATA_PTR session_data,
				 CK_CHAR_PTR pOldPin, 
				 CK_ULONG ulOldLen, 
				 CK_CHAR_PTR pNewPin, 
				 CK_ULONG ulNewLen)
{
    CK_RV ulRet = CKR_OK;
	CK_ULONG ulUserType = 0;
    CK_ULONG ulRetryCount = 0;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
	
	FUNC_BEGIN;
	
    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
        return CKR_GENERAL_ERROR;
    }
    if ((NULL == pOldPin) || (NULL == pNewPin))
    {
        SYNO_LogE("pucOldPin or pucNewPin is null!");
        return CKR_GENERAL_ERROR;
    }

	// �жϵ�ǰ��¼������
	switch (session_data->session_info->state)
	{
	case CKS_RW_USER_FUNCTIONS:
		ulUserType = KT_USERPIN;
		break;
	case CKS_RW_SO_FUNCTIONS:
		ulUserType = KT_SOPIN;
		break;
	default:
        SYNO_LogE("Current user is not login!");
		return CKR_USER_NOT_LOGGED_IN;
	}

    // �޸��û�PIN��ֵ
    ulRet = P11_ChangePIN(session_data->AppHandle, ulUserType, (char*)pOldPin, (char*)pNewPin, &ulRetryCount);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_ChangePIN() failed! ulRet = 0x%x", ulRet);
		if (SAR_COS_PIN_LOCKED == ulRet || 0 == ulRetryCount)
		{
            return CKR_PIN_LOCKED;
		}

        return CKR_PIN_INCORRECT;
    }

	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_OpenSession
 *	˵��:�򿪻Ự
 *	����:slot_data:[IN]:�Ự����ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_OpenSession(CK_I_SESSION_DATA_PTR session_data)
{
    ULONG ulRet = CKR_OK;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    CHAR acName[PATH_MAX] = {0};
    ULONG ulSize = PATH_MAX;
    CHAR acContainerList[PATH_MAX] = {0};
    CHAR *pcList = NULL;
    CHAR *pcContainerName = NULL;
    CHAR *pcRemainder = NULL;
    DEVHANDLE hDev = NULL;
    HAPPLICATION hApp = NULL;
    HCONTAINER hContainer = NULL;
	ContainerInfoEx stContainerInfo = {0};
    ULONG ulObjCount = 0;

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
		SYNO_LogE("session_data is NULL!");
        return CKR_GENERAL_ERROR;
    }
    pstSlotData = session_data->slot_data;
    if (NULL == pstSlotData)
    {
		SYNO_LogE("pstSlotData is NULL!");
        return CKR_GENERAL_ERROR;
    }

    hDev = pstSlotData->pDevHandle;

    // ö��Ӧ��
    ulRet = P11_EnumApplication(hDev, acName, &ulSize);
    if (ulRet != CKR_OK)
    {
		SYNO_LogE("P11_EnumApplication() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
    if (acName[0] == '\0')
    {
		SYNO_LogE("No any application!");
        return CKR_GENERAL_ERROR;
    }
	
    // ����ֻ��һ��Ӧ��,��Ӧ��
	SYNO_LogD("Use applicaiton: %s", acName);
    ulRet = P11_OpenApplication(hDev, acName, &hApp);
    if(ulRet != CKR_OK)
    {
		SYNO_LogE("P11_OpenApplication() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
	session_data->AppHandle = hApp;

	// ö��Ӧ���µ�Data file
	TOK_I_EnumDataFileObj(session_data, hApp, acName);
		
    // ö������
	ulSize = 512;
    ulRet = P11_EnumContainer(hApp, acContainerList, &ulSize);
    if (ulRet != CKR_OK)
    {
		SYNO_LogE("P11_OpenApplication() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
    if (acName[0] == '\0' || ulSize <= 2)
    {
		SYNO_LogD("Not any containers!");
        return CKR_OK;
    }

	// �ַ�������ת��
	pcList = (CHAR*)SYNO_calloc(1, ulSize);
	memset(pcList, 0, ulSize);
	TOK_I_StrFmtList(pcList, acContainerList);

	// ö��ÿ�������еĶ���	
    session_data->CtrCount = 0;
    pcContainerName = TC_strtok_r(pcList, ",", &pcRemainder);
    while (pcContainerName && strlen(pcContainerName) > 0)
    {
		SYNO_LogD("Try to open container: %s", pcContainerName);

		// ��ȡ��������
		ulRet = P11_GetContainerInfoEx(hApp, pcContainerName, &stContainerInfo);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_GetContainerInfoEx() failed! ulRet = 0x%x", ulRet);
			goto NEXT;
		}

		// ����ECC����
		if (2 == stContainerInfo.type)
		{
			SYNO_LogD("This is an ECC container, skip it!");
			goto NEXT;
		}

        // ��ָ������
        ulRet = P11_OpenContainer(hApp, pcContainerName, &hContainer);
        if (SAR_OK == ulRet)
        {
			ulObjCount = 0;
			ulObjCount = TOK_I_EnumPubKeyObj(session_data, hContainer, pcContainerName, &stContainerInfo);
			ulObjCount += TOK_I_EnumPriKeyObj(session_data, hContainer, pcContainerName, &stContainerInfo);
			ulObjCount += TOK_I_EnumCertObj(session_data, hContainer, pcContainerName, &stContainerInfo);
			if (0 == ulObjCount) 
			{
				SYNO_LogD("Not any objects in this container, close it!");
				P11_CloseContainer(hContainer);
				ulObjCount = NULL;
			}
			else
			{
				session_data->CtrCount++;
				SYNO_LogD("There are %d objects in this container!", ulObjCount);
			}
        }
		else
		{
			SYNO_LogE("P11_OpenContainer() failed! ulRet = 0x%x", ulRet);
		}

NEXT:
		pcContainerName = TC_strtok_r(NULL, ",", &pcRemainder);
	}
	
	FUNC_END;

	SYNO_free(pcList);
	pcList = NULL;

    return ulRet;
}
 /***********************************************************************
 *	����:TOK_CloseSession
 *	˵��:�رջỰ
 *	����:slot_data:[IN]:�Ự����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_CloseSession(CK_I_SESSION_DATA_PTR session_data)
{
    ULONG ulRet = CKR_OK;
    ULONG ulLen = TOK_ONCE_RW_LEN;
    UCHAR pucBuff[TOK_ONCE_RW_LEN] = {0};
    HCONTAINER hContainer = NULL;
    ULONG i = 0;

	FUNC_BEGIN;

	// ����Ƿ��ڽ��м��ܲ���
    if (NULL != session_data->encrypt_state)
    {
        ulRet = TOK_EncryptFinal(session_data, pucBuff, &ulLen);
        if (ulRet != CKR_OK)
        {
			SYNO_LogE("TOK_EncryptFinal() failed! ulRet = 0x%x", ulRet);
            return ulRet;
        }
    }
	
	// ����Ƿ��ڽ��н��ܲ���
    if (NULL != session_data->decrypt_state)
    {
        ulRet = TOK_DecryptFinal(session_data, pucBuff, &ulLen);
        if (ulRet != CKR_OK)
        {
			SYNO_LogE("TOK_DecryptFinal() failed! ulRet = 0x%x", ulRet);
            return ulRet;
        }
    }

	// �ر��������
    for (i = 0; i < session_data->CtrCount; i++)
    {
        hContainer = (HCONTAINER)session_data->aCtrHandle[i];
        (void)P11_CloseContainer(hContainer);
		session_data->aCtrHandle[i] = NULL;
    }

    // �ر�Ӧ��
    ulRet = P11_CloseApplication(session_data->AppHandle);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_CloseApplication() failed! ulRet = 0x%x", ulRet);
        return ulRet;
    }

    return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_GetOperationState
 *	˵��:���ز���״̬
 *	����:session_data:[IN]:�Ự����
 ��������pOperationState:[OUT]:��ŷ���״̬��ָ��
 ��������pulOperationStateLen:[OUT]:��ŷ���״̬���ݳ��ȵ�ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_GetOperationState(CK_I_SESSION_DATA_PTR session_data, 
							CK_BYTE_PTR pOperationState, 
							CK_ULONG_PTR pulOperationStateLen)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	����:TOK_SetOperationState
 *	˵��:���ò���״̬
 *	����:session_data:[IN]:�Ự����
 ��������pOperationState:[IN]:���״̬��Buffer
 ��������ulOperationStateLen:[IN]:Buffer����
 ��������encrypt_key_obj:[IN]:����Key����ָ��
 ��������auth_key_obj:[IN]:��֤Key����ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SetOperationState(CK_I_SESSION_DATA_PTR session_data, 
							CK_BYTE_PTR pOperationState, 
							CK_ULONG ulOperationStateLen, 
							CK_I_OBJ_PTR encrypt_key_obj, 
							CK_I_OBJ_PTR auth_key_obj)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	����:TOK_Login
 *	˵��:��¼�Ự
 *	����:session_data:[IN]:�Ự����
 ��������userType:[IN]:�û����ͣ�CKU_SO����CKU_USER��
 ��������pPin:[IN]:PIN��
 ��������ulPinLen:[IN]:PIN�볤��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_Login(CK_I_SESSION_DATA_PTR session_data,
				CK_USER_TYPE userType, 
				CK_CHAR_PTR pPin,
				CK_ULONG ulPinLen)
{
    ULONG ulRet = CKR_OK;
	CHAR acName[PATH_MAX] = {0};
	ULONG ulSize = PATH_MAX;
    CK_I_SLOT_DATA_PTR pstSlotData = NULL;
    ULONG ulPINType = 0;
    ULONG ulRetryCount = 0;

    FUNC_BEGIN;

    // �������
    if ((NULL == session_data) || (NULL == pPin))
    {
		SYNO_LogE("session_data or pPin is NULL!");
        return CKR_GENERAL_ERROR;
    }

    pstSlotData = session_data->slot_data;
    if (NULL == pstSlotData)
    {
        SYNO_LogE("pstSlotData is null!");
        return CKR_GENERAL_ERROR;
    }

    // ת����¼����
    if (CKU_SO == userType)
    {
        ulPINType = KT_SOPIN;
    }
    else
    {
        ulPINType = KT_USERPIN;
    }

    // У��PIN��
    ulRet = P11_VerifyPIN(session_data->AppHandle, ulPINType, (char*)pPin, &ulRetryCount);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_VerifyPIN() failed! ulRet = 0x%x", ulRet);
		if (SAR_COS_PIN_LOCKED == ulRet || 0 == ulRetryCount)
		{
            return CKR_PIN_LOCKED;
		}

		return CKR_PIN_INCORRECT;
    }

	FUNC_END;

    return ulRet;
}
 /***********************************************************************
 *	����:TOK_Logout
 *	˵��:�ǳ��Ự
 *	����:session_data:[IN]:�Ự����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_Logout(CK_I_SESSION_DATA_PTR session_data)
{
    ULONG ulRet = CKR_OK;

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
        return CKR_GENERAL_ERROR;
    }

    // ���Ӧ�õ�ǰ�İ�ȫ״̬
    ulRet = P11_ClearSecureState(session_data->AppHandle);
    if (CKR_OK != ulRet)
    {
        SYNO_LogE("P11_ClearSecureState() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_EncryptInit
 *	˵��:���ܳ�ʼ��
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[IN]:��Կ�㷨
 *       key_obj:[IN]:�����õ���Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_EncryptInit(CK_I_SESSION_DATA_PTR session_data,
					  CK_MECHANISM_PTR pMechanism, 
					  CK_I_OBJ_PTR key_obj)
{
    ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
    BLOCKCIPHERPARAM stEncryptParam = {0};

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == key_obj)
    {
        SYNO_LogE("key_obj is null!");
        return CKR_ARGUMENTS_BAD;
    }

	// ���������㷨
	switch (pMechanism->mechanism)
	{
	case CKM_DES_ECB:
		ulAlgId = SGD_DES_ECB;
		break;
	case CKM_DES_CBC:
		ulAlgId = SGD_DES_CBC;
		break;
	case CKM_DES3_ECB:
		ulAlgId = SGD_DES3_ECB;
		break;
	case CKM_DES3_CBC:
		ulAlgId = SGD_DES3_CBC;
		break;
	case CKM_AES_ECB:
		ulAlgId = SGD_AES128_ECB;
		break;
	case CKM_AES_CBC:
		ulAlgId = SGD_AES128_CBC;
		break;
	default:
	case CKM_RSA_PKCS:
		ulAlgId = SGD_RSA;
		break;
	}
	if (0 == ulAlgId)
	{
        SYNO_LogE("Key algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF alg id: 0x%x", pMechanism->mechanism, ulAlgId);

	// �������ܲ����������
	if (SGD_AES128_CBC == ulAlgId || CKM_DES_CBC == ulAlgId || CKM_DES3_CBC == ulAlgId)
	{
		if (!pMechanism->pParameter)
		{
			SYNO_LogE("pMechanism->pParameter is NULL! It should be IV!");
			return CKR_MECHANISM_PARAM_INVALID;
		}
		if (0 == pMechanism->ulParameterLen || pMechanism->ulParameterLen > 32)
		{
			SYNO_LogE("pMechanism->ulParameterLen is wrong! pMechanism->ulParameterLen = 0x%x", pMechanism->ulParameterLen);
			return CKR_MECHANISM_PARAM_INVALID;
		}
		memcpy(stEncryptParam.IV, pMechanism->pParameter, pMechanism->ulParameterLen);
	}
	
    // ���ݼ��ܳ�ʼ��,�������ݼ��ܵ��㷨��ز���
	if (SGD_RSA != ulAlgId)
	{
		stEncryptParam.PaddingType = 1;		//�Զ�����
		ulRet = P11_EncryptInit(key_obj->ObjHandle, ulAlgId, stEncryptParam);
		if (CKR_OK != ulRet)
		{
			SYNO_LogE("P11_EncryptInit() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		session_data->encrypt_state = key_obj->ObjHandle;
	}
	else
	{
		session_data->encrypt_state = key_obj;
	}

	session_data->encrypt_mechanism = pMechanism->mechanism;

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	����:TOK_Encrypt
 *	˵��:�����������
 *	����:session_data:[IN]:�Ự����
 *       pData:[IN]:��������
 *       ulDataLen:[IN]:�������ݳ���
 *       pEncryptedData:[OUT]:���ص���������
 *       pulEncryptedDataLen:[OUT]:���ص��������ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_Encrypt(CK_I_SESSION_DATA_PTR session_data,
				  CK_BYTE_PTR pData, 
				  CK_ULONG ulDataLen, 
				  CK_BYTE_PTR pEncryptedData, 
				  CK_ULONG_PTR pulEncryptedDataLen)
{   
	ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pData || 0 == ulDataLen)
    {
        SYNO_LogE("pucData is null or ulDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulEncryptedDataLen)
    {
        SYNO_LogE("pulEncryptedPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }
	
	// ��֧�ֲ���
	//if (ulDataLen % 16)
	//{
 //       SYNO_LogE("ulDataLen is not Nx16!");
	//	return CKR_ARGUMENTS_BAD;
	//}

    // �õ����ܾ��
    hKey = session_data->encrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }

	if (CKM_RSA_PKCS == session_data->encrypt_mechanism)
	{
		// RSA��Կ��������		
		ulRet = P11_ExtRSAEnDecrypt(((CK_I_OBJ_PTR)hKey)->CtrHandle, FALSE, TRUE, TRUE, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExtRSAEnDecrypt() failed! ulRet = 0x%x", ulRet);
			ulRet = CKR_GENERAL_ERROR;
			goto ERR;
		}
	}
	else
	{
		// �Գ���Կ��������
		ulRet = P11_Encrypt(hKey, pData, ulDataLen, pEncryptedData, pulEncryptedDataLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_Encrypt() failed! ulRet = 0x%x", ulRet);
			ulRet = CKR_GENERAL_ERROR;
			goto ERR;
		}
	}
	// ֱ�ӷ��س���
	if (NULL == pEncryptedData)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulEncryptedDataLen);
		return CKR_OK;
	}
	

	FUNC_END;

	// ���ܽ���
ERR:
	session_data->encrypt_state = NULL;
	session_data->encrypt_mechanism = 0;

    return ulRet;
}
/***********************************************************************
 *	����:TOK_EncryptUpdate
 *	˵��:�����������
 *	����:session_data:[IN]:�Ự����
 *       pPart:[IN]:���ķ�������
 *       ulPartLen:[IN]:���ķ������ݳ���
 *       pEncryptedPart:[OUT]:���ص���������
 *       pulEncryptedPartLen:[OUT]:���ص��������ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_EncryptUpdate(CK_I_SESSION_DATA_PTR session_data,
						CK_BYTE_PTR pPart, 
						CK_ULONG ulPartLen, 
						CK_BYTE_PTR pEncryptedPart, 
						CK_ULONG_PTR pulEncryptedPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pPart || 0 == ulPartLen)
    {
        SYNO_LogE("pPart is null or ulPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulEncryptedPartLen)
    {
        SYNO_LogE("pulEncryptedPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// ��֧�ֲ���
	//if (ulPartLen % 16)
	//{
 //       SYNO_LogE("ulPartLen is not Nx16!");
	//	return CKR_ARGUMENTS_BAD;
	//}

    // �õ����ܾ��
    hKey = session_data->encrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->encrypt_mechanism)
	{
		// RSA��Կ��������		
		ulRet = P11_ExtRSAEnDecrypt(((CK_I_OBJ_PTR)hKey)->CtrHandle, FALSE, TRUE, TRUE, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExtRSAEnDecrypt() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	else
	{
		// �Գ���Կ�����������
		ulRet = P11_EncryptUpdate(hKey, pPart, ulPartLen, pEncryptedPart, pulEncryptedPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_EncryptUpdate() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// ֱ�ӷ��س���
	if (NULL == pEncryptedPart)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulEncryptedPartLen);
		return CKR_OK;
	}

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	����:TOK_EncryptFinal
 *	˵��:������ܽ���
 *	����:session_data:[IN]:�Ự����
 *       pLastEncryptedPart:[OUT]:������������������
 *       pulLastEncryptedPartLen:[OUT]:�����������������ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_EncryptFinal(CK_I_SESSION_DATA_PTR session_data,
					   CK_BYTE_PTR pLastEncryptedPart, 
					   CK_ULONG_PTR pulLastEncryptedPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;
	
	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulLastEncryptedPartLen)
    {
        SYNO_LogE("pulLastEncryptedPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

    // �õ����ܾ��
    hKey = session_data->encrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("slot_data->encrypt_state is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->encrypt_mechanism)
	{
		//TBD: ...
	}
	else
	{
		// ��������������ݵļ���,����ʣ����ܽ��
		ulRet = P11_EncryptFinal(hKey, pLastEncryptedPart, pulLastEncryptedPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_EncryptFinal() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// ֱ�ӷ��س���
	if (NULL == pLastEncryptedPart)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulLastEncryptedPartLen);
		return CKR_OK;
	}
	
	// ���ܽ���
    session_data->encrypt_state = NULL;
	session_data->encrypt_mechanism = 0;

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	����:TOK_DecryptInit
 *	˵��:���ܳ�ʼ��
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[IN]:��Կ�㷨
 *       key_obj:[IN]:�����õ���Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DecryptInit(CK_I_SESSION_DATA_PTR session_data,
					  CK_MECHANISM_PTR pMechanism,
					  CK_I_OBJ_PTR pstKeyObj)
{
    ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
    BLOCKCIPHERPARAM stDecryptParam = {0};
	
	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pstKeyObj)
    {
        SYNO_LogE("pstKeyObj is null!");
        return CKR_ARGUMENTS_BAD;
    }

	// ���������㷨
	switch (pMechanism->mechanism)
	{
	case CKM_DES_ECB:
		ulAlgId = SGD_DES_ECB;
		break;
	case CKM_DES_CBC:
		ulAlgId = SGD_DES_CBC;
		break;
	case CKM_DES3_ECB:
		ulAlgId = SGD_DES3_ECB;
		break;
	case CKM_DES3_CBC:
		ulAlgId = SGD_DES3_CBC;
		break;
	case CKM_AES_ECB:
		ulAlgId = SGD_AES128_ECB;
		break;
	case CKM_AES_CBC:
		ulAlgId = SGD_AES128_CBC;
		break;
	case CKM_RSA_PKCS:
		ulAlgId = SGD_RSA;
		break;
	default:
		break;
	}
	if (0 == ulAlgId)
	{
        SYNO_LogE("Key algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF alg id: 0x%x", pMechanism->mechanism, ulAlgId);

	// �������ܲ����������
	if (SGD_AES128_CBC == ulAlgId || CKM_DES_CBC == ulAlgId || CKM_DES3_CBC == ulAlgId)
	{
		if (!pMechanism->pParameter)
		{
			SYNO_LogE("pMechanism->pParameter is NULL! It should be IV!");
			return CKR_MECHANISM_PARAM_INVALID;
		}
		if (0 == pMechanism->ulParameterLen || pMechanism->ulParameterLen > 32)
		{
			SYNO_LogE("pMechanism->ulParameterLen is wrong! pMechanism->ulParameterLen = 0x%x", pMechanism->ulParameterLen);
			return CKR_MECHANISM_PARAM_INVALID;
		}
		memcpy(stDecryptParam.IV, pMechanism->pParameter, pMechanism->ulParameterLen);
	}
	
	// ���ܳ�ʼ��
	if (SGD_RSA != ulAlgId)
	{
		stDecryptParam.PaddingType = 1;		//�Զ�ȥ������
		ulRet = P11_DecryptInit(pstKeyObj->ObjHandle, ulAlgId, stDecryptParam);
		if (CKR_OK != ulRet)
		{
			SYNO_LogE("P11_DecryptInit() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
		session_data->decrypt_state = pstKeyObj->ObjHandle;
	}
	else
	{
		session_data->decrypt_state = pstKeyObj;
	}

	session_data->decrypt_mechanism = pMechanism->mechanism;

	FUNC_END;

    return ulRet;

}
/***********************************************************************
 *	����:TOK_Decrypt
 *	˵��:�����������
 *	����:session_data:[IN]:�Ự����
 *       pEncryptedData:[IN]:��������
 *       ulEncryptedDataLen:[IN]:�������ݳ���
 *       pData:[OUT]:���ص���������
 *       pulDataLen:[OUT]:���ص��������ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_Decrypt(CK_I_SESSION_DATA_PTR session_data,
				  CK_BYTE_PTR pEncryptedData, 
				  CK_ULONG ulEncryptedDataLen, 
				  CK_BYTE_PTR pData, 
				  CK_ULONG_PTR pulDataLen)
{
	ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;
	
	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pEncryptedData || 0 == ulEncryptedDataLen)
    {
        SYNO_LogE("pEncryptedData is null or ulEncryptedDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulDataLen)
    {
        SYNO_LogE("pulDataLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// ��֧�ֲ���
	//if (ulEncryptedDataLen % 16)
	//{
 //       SYNO_LogE("ulEncryptedDataLen is not Nx16!");
	//	return CKR_ARGUMENTS_BAD;
	//}
	
    // �õ����ܾ��
    hKey = session_data->decrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->decrypt_mechanism)
	{
		// RSA˽Կ��������		
		ulRet = P11_ExtRSAEnDecrypt(((CK_I_OBJ_PTR)hKey)->CtrHandle, FALSE, FALSE, FALSE, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExtRSAEnDecrypt() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	else
	{
		// �������ݽ���
		ulRet = P11_Decrypt(hKey, pEncryptedData, ulEncryptedDataLen, pData, pulDataLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_Decrypt() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// ֱ�ӷ��س���
	if (NULL == pData)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulDataLen);
		return CKR_OK;
	}

	// ���ܽ���
    session_data->decrypt_state = NULL;
	session_data->decrypt_mechanism = 0;

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	����:TOK_DecryptUpdate
 *	˵��:�����������
 *	����:session_data:[IN]:�Ự����
 *       pEncryptedPart:[IN]:���ķ�������
 *       ulEncryptedPartLen:[IN]:���ķ������ݳ���
 *       pPart:[OUT]:���ص���������
 *       pulPartLen:[OUT]:���ص��������ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DecryptUpdate(CK_I_SESSION_DATA_PTR session_data,
						CK_BYTE_PTR pEncryptedPart, 
						CK_ULONG ulEncryptedPartLen, 
						CK_BYTE_PTR pPart, 
						CK_ULONG_PTR pulPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;
	
	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pEncryptedPart || 0 == ulEncryptedPartLen)
    {
        SYNO_LogE("pEncryptedPart is null or ulEncryptedPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulPartLen)
    {
        SYNO_LogE("pulPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }
	
	// ��֧�ֲ���
	//if (ulEncryptedPartLen % 16)
	//{
 //       SYNO_LogE("ulEncryptedDataLen is not Nx16!");
	//	return CKR_ARGUMENTS_BAD;
	//}

    // �õ����ܾ��
    hKey = session_data->decrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->decrypt_mechanism)
	{
		// RSA˽Կ��������		
		ulRet = P11_ExtRSAEnDecrypt(((CK_I_OBJ_PTR)hKey)->CtrHandle, FALSE, FALSE, FALSE, pEncryptedPart, ulEncryptedPartLen, pPart, pulPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_ExtRSAEnDecrypt() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	else
	{
		// �������ݽ���
		ulRet = P11_DecryptUpdate(hKey, pEncryptedPart, ulEncryptedPartLen, pPart, pulPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_DecryptUpdate() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// ֱ�ӷ��س���
	if (NULL == pPart)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulPartLen);
		return CKR_OK;
	}

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	����:TOK_DecryptFinal
 *	˵��:���ܽ���
 *	����:session_data:[IN]:�Ự����
 *       pLastEncryptedPart:[OUT]:������������������
 *       pulLastEncryptedPartLen:[OUT]:�����������������ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DecryptFinal(CK_I_SESSION_DATA_PTR session_data,
					   CK_BYTE_PTR pLastPart, 
					   CK_ULONG_PTR pulLastPartLen)
{
    ULONG ulRet = CKR_OK;
    HANDLE hKey = NULL;

	FUNC_BEGIN;

	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulLastPartLen)
    {
        SYNO_LogE("pulLastPartLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

    // �õ����ܾ��
    hKey = session_data->decrypt_state;
    if (NULL == hKey)
    {
        SYNO_LogE("hKey is null!");
        return CKR_GENERAL_ERROR;
    }
	
	if (CKM_RSA_PKCS == session_data->encrypt_mechanism)
	{
	}
	else
	{
		// ��������������ݵĽ���,����ʣ����ܽ��
		ulRet = P11_DecryptFinal(hKey, pLastPart, pulLastPartLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_DecryptFinal() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}
	}
	// ֱ�ӷ��س���
	if (NULL == pLastPart)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulLastPartLen);
		return CKR_OK;
	}

	// ���ܽ���
    session_data->decrypt_state = NULL;
	session_data->decrypt_mechanism = 0;
	
	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	����:TOK_DigestInit
 *	˵��:�Ӵճ�ʼ��
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[IN]:�Ӵ��㷨
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DigestInit(CK_I_SESSION_DATA_PTR session_data, 
					 CK_MECHANISM_PTR pMechanism)
{
	ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
	DEVHANDLE hDev = NULL;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// �豸���
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
        return CKR_GENERAL_ERROR;
	}

	// ����㷨
	switch(pMechanism->mechanism)
	{
	case CKM_MD5:
		ulAlgId = SGD_MD5;
		break;
	case CKM_SHA_1:
		ulAlgId = SGD_SHA1;
		break;
	case CKM_SHA256:
		ulAlgId = SGD_SHA256;
		break;
	}
	if (0 == ulAlgId)
	{
        SYNO_LogE("Digest algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF alg id: 0x%x", pMechanism->mechanism, ulAlgId);

	// �Ӵճ�ʼ��
	ulRet = P11_DigestInit(hDev, ulAlgId, NULL, NULL, 0, &hDigestKey);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestInit() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	// ������
	session_data->digest_state = hDigestKey;
	session_data->digest_mechanism = pMechanism->mechanism;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_Digest
 *	˵��:���������Ӵ�����
 *	����:session_data:[IN]:�Ự����
 *       pData:[IN]:ԭʼ����
 *       ulDataLen:[IN]:ԭʼ���ݳ���
 *       pDigest:[IN]:���ص��Ӵ�����
 *       pulDigestLen:[IN]:���ص��Ӵ����ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_Digest(CK_I_SESSION_DATA_PTR session_data, 
				 CK_BYTE_PTR pData, 
				 CK_ULONG ulDataLen, 
				 CK_BYTE_PTR pDigest, 
				 CK_ULONG_PTR pulDigestLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pData || 0 == ulDataLen)
    {
        SYNO_LogE("pData is null or ulDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
	if (pulDigestLen)
	{
        SYNO_LogE("pulDigestLen is null!");
		return CKR_ARGUMENTS_BAD;
	}

	// �Ӵվ��
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        return CKR_GENERAL_ERROR;
	}

	// �Ӵ�����
	ulRet = P11_Digest(hDigestKey, pData, ulDataLen, pDigest, pulDigestLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_Digest() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	// ���س���
	if (NULL == pDigest)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulDigestLen);
		return CKR_OK;		
	}

	// �Ӵս���
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	P11_CloseHandle(hDigestKey);
	hDigestKey = NULL;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_DigestUpdate
 *	˵��:���������Ӵ�����
 *	����:session_data:[IN]:�Ự����
 *       pPart:[IN]:����ԭʼ����
 *       ulPartLen:[IN]:����ԭʼ���ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DigestUpdate(CK_I_SESSION_DATA_PTR session_data, 
					   CK_C_BYTE_PTR pPart, 
					   CK_ULONG ulPartLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pPart || 0 == ulPartLen)
    {
        SYNO_LogE("pPart is null or ulPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }

	// �Ӵվ��
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        return CKR_GENERAL_ERROR;
	}

	// �Ӵ�����
	ulRet = P11_DigestUpdate(hDigestKey, (BYTE*)pPart, ulPartLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestUpdate() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
	
	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_DigestKey
 *	˵��:�Ӵ���Կ����
 *	����:session_data:[IN]:�Ự����
 *       key_obj:[IN]:��Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DigestKey(CK_I_SESSION_DATA_PTR  session_data, 
					CK_I_OBJ_PTR key_obj)
{
	return CKR_FUNCTION_FAILED;
}
/***********************************************************************
 *	����:TOK_DigestUpdate
 *	˵��:�Ӵս���
 *	����:session_data:[IN]:�Ự����
 *       pDigest:[OUT]:���ص��Ӵ�ֵ
 *       pulDigestLen:[OUT]:�Ӵ�ֵ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DigestFinal(CK_I_SESSION_DATA_PTR session_data,  
					  BYTE* pDigest, 
					  ULONG* pulDigestLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulDigestLen)
    {
        SYNO_LogE("pulDigestLen is NULL!");
		return CKR_ARGUMENTS_BAD;
    }

	// �Ӵվ��
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        return CKR_GENERAL_ERROR;
	}

	// �Ӵ�����
	ulRet = P11_DigestFinal(hDigestKey, pDigest, pulDigestLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestFinal() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }
	
	// ���س���
	if (NULL == pDigest)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulDigestLen);
		return CKR_OK;		
	}

	// �Ӵս���
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	P11_CloseHandle(hDigestKey);
	hDigestKey = NULL;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_SignInit
 *	˵��:ǩ����ʼ��
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[OUT]:ǩ���㷨
 *       key_obj:[OUT]:ǩ����Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SignInit(CK_I_SESSION_DATA_PTR session_data, 
				   CK_MECHANISM_PTR pMechanism, 
				   CK_I_OBJ_PTR key_obj)
{
	ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
	DEVHANDLE hDev = NULL;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == key_obj)
    {
        SYNO_LogE("key_obj is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// �豸���
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
        return CKR_GENERAL_ERROR;
	}

	// ����㷨
	switch(pMechanism->mechanism)
	{
	case CKM_RSA_PKCS:
		break;
	case CKM_MD5_RSA_PKCS:
		ulAlgId = SGD_MD5;
		break;
	case CKM_SHA1_RSA_PKCS:
		ulAlgId = SGD_SHA1;
		break;
	case CKM_SHA256_RSA_PKCS:
		ulAlgId = SGD_SHA256;
		break;
	default:
        SYNO_LogE("Digest algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	// �ж��Ƿ���Ҫ���Ӵ�����
	if (CKM_RSA_PKCS == pMechanism->mechanism)
	{
		SYNO_LogD("pMechanism->mechanism = 0x%x, don't need to do digest!");
		session_data->digest_state = NULL;
	}
	else
	{
		SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF digest alg id: 0x%x", pMechanism->mechanism, ulAlgId);

		// �Ӵճ�ʼ��
		ulRet = P11_DigestInit(hDev, ulAlgId, NULL, NULL, 0, &hDigestKey);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_DigestInit() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}

		// ������
		session_data->digest_state = hDigestKey;
	}

	session_data->sign_mechanism = pMechanism->mechanism;
	session_data->sign_state = key_obj;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_Sign
 *	˵��:ǩ����������
 *	����:session_data:[IN]:�Ự����
 *       pData:[IN]:��ǩ����ԭ������
 *       ulDataLen:[IN]:ԭ�����ݳ���
 *       pSignature:[OUT]:���ص�ǩ��ֵ
 *       pulSignatureLen:[OUT]:ǩ��ֵ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_Sign(CK_I_SESSION_DATA_PTR session_data, 
			    CK_BYTE_PTR pData, 
				CK_ULONG ulDataLen, 
				CK_BYTE_PTR pSignature, 
				CK_ULONG_PTR pulSignatureLen)
{
	ULONG ulRet = CKR_OK;
	ULONG ulModulusBits = 0;
	ULONG ulDigestLen = 256;
	BYTE btDigestData[256] = {0};
	HANDLE hDigestKey = NULL;
	HCONTAINER hContainer = NULL;
	CK_I_OBJ_PTR pstPriKeyObj = NULL;
	CK_ATTRIBUTE_PTR pstPriAttr = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pData || 0 == ulDataLen)
    {
        SYNO_LogE("pData is null or ulDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
	if (!pulSignatureLen)
	{
        SYNO_LogE("pulSignatureLen is null!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// ��ȡǩ��˽Կ��������
	pstPriKeyObj = (CK_I_OBJ_PTR)session_data->sign_state;
	if (NULL == pstPriKeyObj)
	{
        SYNO_LogE("pstPriKeyObj is null!");
		return CKR_GENERAL_ERROR;
	}
	hContainer = pstPriKeyObj->CtrHandle;
	if (NULL == hContainer)
	{
        SYNO_LogE("hContainer is null!");
		return CKR_GENERAL_ERROR;
	}

	// �ж��Ƿ���Ҫ���Ӵ�����
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{		
		// �ж����ݳ���
		pstPriAttr = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPriKeyObj, CK_IA_PRIVATE_EXPONENT));
		if (pstPriAttr)
		{
			ulModulusBits = pstPriAttr->ulValueLen * 8;
			if (ulDataLen > (ulModulusBits - 11))
			{
				SYNO_LogE("ulDataLen is wrong! ulDataLen = 0x%x", ulDataLen);
				ulRet = CKR_DATA_LEN_RANGE;
				goto ERR;
			}
		}
		else if (ulDataLen > (256-11))
		{
			SYNO_LogE("ulDataLen is wrong! ulDataLen = 0x%x", ulDataLen);
			ulRet = CKR_DATA_LEN_RANGE;
			goto ERR;
		}

		// ��������
		memcpy(btDigestData, pData, ulDataLen);
		ulDigestLen = ulDataLen;
	}
	else
	{
		// �Ӵվ��
		hDigestKey = session_data->digest_state;
		if (NULL == hDigestKey)
		{
			SYNO_LogE("hDigestKey is null!");
			ulRet = CKR_GENERAL_ERROR;
			goto ERR;
		}

		// �Ӵ�����
		ulRet = P11_Digest(hDigestKey, pData, ulDataLen, btDigestData, &ulDigestLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_Digest() failed! ulRet = 0x%x", ulRet);
			ulRet = CKR_GENERAL_ERROR;
			goto ERR;
		}
	}
	
	// ǩ������
	ulRet = P11_RSASignData(hContainer, btDigestData, ulDigestLen, pSignature, pulSignatureLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_RSASignData() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERR;
    }

	// ���س���
	if (NULL == pSignature)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulSignatureLen);
		return CKR_OK;		
	}
		
	FUNC_END;

	// ǩ������
ERR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->sign_state = NULL;
	session_data->sign_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_SignUpdate
 *	˵��:ǩ����������
 *	����:session_data:[IN]:�Ự����
 *       pPart:[IN]:��ǩ���ķ���ԭ������
 *       ulPartLen:[IN]:����ԭ�����ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SignUpdate(CK_I_SESSION_DATA_PTR session_data, 
					 CK_BYTE_PTR pPart, 
					 CK_ULONG ulPartLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pPart || 0 == ulPartLen)
    {
        SYNO_LogE("pPart is null or ulPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }

	// �ж��㷨
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{
        SYNO_LogE("This function doesn't support the sign mechanism! session_data->sign_mechanism = 0x%x", session_data->sign_mechanism);
        ulRet = CKR_MECHANISM_INVALID;
		goto ERROR;
	}

	// �Ӵվ��
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	
	// �Ӵ�����
	ulRet = P11_DigestUpdate(hDigestKey, pPart, ulPartLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestUpdate() failed! ulRet = 0x%x", ulRet);
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	FUNC_END;	

	// ǩ������
ERROR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->sign_state = NULL;
	session_data->sign_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_SignFinal
 *	˵��:ǩ������
 *	����:session_data:[IN]:�Ự����
 *       pSignature:[OUT]:���ص�ǩ��ֵ
 *       pulSignatureLen:[OUT]:ǩ��ֵ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SignFinal(CK_I_SESSION_DATA_PTR session_data, 
					CK_BYTE_PTR pSignature, 
					CK_ULONG_PTR pulSignatureLen)
{
	ULONG ulRet = CKR_OK;
	ULONG ulDigestLen = 256;
	BYTE btDigestData[256] = {0};
	HANDLE hDigestKey = NULL;
	HCONTAINER hContainer = NULL;
	CK_I_OBJ_PTR pstPriKeyObj = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pulSignatureLen)
    {
        SYNO_LogE("pulSignatureLen is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// �ж��㷨
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{
        SYNO_LogE("This function doesn't support the sign mechanism! session_data->sign_mechanism = 0x%x", session_data->sign_mechanism);
		ulRet = CKR_MECHANISM_INVALID;
		goto ERROR;
	}

	// �Ӵվ��
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	// ��ȡ�Ӵս��
	ulRet = P11_DigestFinal(hDigestKey, btDigestData, &ulDigestLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestFinal() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	// ��ȡǩ��˽Կ��������
	pstPriKeyObj = (CK_I_OBJ_PTR)session_data->sign_state;
	if (NULL == pstPriKeyObj)
	{
        SYNO_LogE("pstPriKeyObj is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	hContainer = pstPriKeyObj->CtrHandle;
	if (NULL == hContainer)
	{
        SYNO_LogE("hContainer is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	// ���Ӵս��ǩ��
	ulRet = P11_RSASignData(hContainer, btDigestData, ulDigestLen, pSignature, pulSignatureLen);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("P11_RSASignData() failed()! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	
	// ���س���
	if (NULL == pSignature)
	{
		SYNO_LogD("Retruen reqested output length: 0x%x", *pulSignatureLen);
		return CKR_OK;		
	}
	
	FUNC_END;

ERROR:
	// �Ӵս���
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	
	// ǩ������
	session_data->sign_state = NULL;
	session_data->sign_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_SignRecoverInit
 *	˵��:�ɻָ�ǩ�������ʼ��
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[OUT]:ǩ���㷨
 *       key_obj:[OUT]:ǩ����Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SignRecoverInit(CK_I_SESSION_DATA_PTR session_data, 
						  CK_MECHANISM_PTR pMechanism, 
						  CK_I_OBJ_PTR key_obj)
{
	return CKR_FUNCTION_FAILED;
}
/***********************************************************************
 *	����:TOK_SignRecover
 *	˵��:�ɻָ�ǩ������
 *	����:session_data:[IN]:�Ự����
 *       pData:[IN]:��ǩ����ԭ������
 *       ulDataLen:[IN]:ԭ�����ݳ���
 *       pSignature:[OUT]:���ص�ǩ��ֵ
 *       pulSignatureLen:[OUT]:ǩ��ֵ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SignRecover(CK_I_SESSION_DATA_PTR session_data,
					  CK_BYTE_PTR pData, 
					  CK_ULONG ulDataLen, 
					  CK_BYTE_PTR pSignature, 
					  CK_ULONG_PTR pulSignatureLen)
{
	return CKR_FUNCTION_FAILED;
}
/***********************************************************************
 *	����:TOK_VerifyInit
 *	˵��:��ǩ��ʼ��
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[OUT]:ǩ���㷨
 *       key_obj:[OUT]:ǩ����Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_VerifyInit(CK_I_SESSION_DATA_PTR session_data,
					 CK_MECHANISM_PTR pMechanism, 
					 CK_I_OBJ_PTR key_obj)
{
	ULONG ulRet = CKR_OK;
	ULONG ulAlgId = 0;
	DEVHANDLE hDev = NULL;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == key_obj)
    {
        SYNO_LogE("key_obj is null!");
		return CKR_ARGUMENTS_BAD;
    }

	// �豸���
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
        return CKR_GENERAL_ERROR;
	}

	// ����㷨
	switch(pMechanism->mechanism)
	{
	case CKM_RSA_PKCS:
		break;
	case CKM_MD5_RSA_PKCS:
		ulAlgId = SGD_MD5;
		break;
	case CKM_SHA1_RSA_PKCS:
		ulAlgId = SGD_SHA1;
		break;
	case CKM_SHA256_RSA_PKCS:
		ulAlgId = SGD_SHA256;
		break;
	default:
        SYNO_LogE("Digest algId in mechanism is wrong! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	// �ж��Ƿ���Ҫ���Ӵ�����
	if (CKM_RSA_PKCS == pMechanism->mechanism)
	{
		SYNO_LogD("pMechanism->mechanism = 0x%x, don't need to do digest!");
		session_data->digest_state = NULL;
	}
	else
	{
		SYNO_LogD("pMechanism->mechanism = 0x%x, the according SKF digest alg id: 0x%x", pMechanism->mechanism, ulAlgId);

		// �Ӵճ�ʼ��
		ulRet = P11_DigestInit(hDev, ulAlgId, NULL, NULL, 0, &hDigestKey);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_DigestInit() failed! ulRet = 0x%x", ulRet);
			return CKR_GENERAL_ERROR;
		}

		// ������
		session_data->digest_state = hDigestKey;
	}

	session_data->verify_mechanism = pMechanism->mechanism;
	session_data->verify_state = key_obj;

	FUNC_END;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_Verify
 *	˵��:��ǩ��������
 *	����:session_data:[IN]:�Ự����
 *       pData:[IN]:ǩ����ԭ������
 *       ulDataLen:[IN]:ԭ�����ݳ���
 *       pSignature:[IN]:ǩ��ֵ
 *       pulSignatureLen:[IN]:ǩ��ֵ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_Verify(CK_I_SESSION_DATA_PTR session_data, 
				 CK_BYTE_PTR pData, 
				 CK_ULONG ulDataLen, 
				 CK_BYTE_PTR pSignature, 
				 CK_ULONG ulSignatureLen)
{
	ULONG ulRet = CKR_OK;
	ULONG ulKeyBytes = 0;
	ULONG ulDigestLen = 256;
	BYTE btDigestData[256] = {0};
	HANDLE hDigestKey = NULL;
	DEVHANDLE hDev = NULL;
	CK_I_OBJ_PTR pstPubKeyObj = NULL;
	CK_ATTRIBUTE_PTR pstPubAttr = NULL;
	RSAPUBLICKEYBLOB stuRsaPubKey = {0};

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pData || 0 == ulDataLen)
    {
        SYNO_LogE("pData is null or ulDataLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }
	if (NULL == pSignature || 0 == ulSignatureLen)
	{
        SYNO_LogE("pSignature is null or ulSignatureLen is 0!");
		return CKR_ARGUMENTS_BAD;
	}
	
	// �豸���
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	// ��ȡǩ����Կ
	pstPubKeyObj = (CK_I_OBJ_PTR)session_data->verify_state;
	if (NULL == pstPubKeyObj)
	{
        SYNO_LogE("pstPubKeyObj is null!");
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	// �ж��Ƿ���Ҫ���Ӵ�����
	if (CKM_RSA_PKCS == session_data->verify_mechanism)
	{		
		// �ж����ݳ���
		pstPubAttr = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstPubKeyObj, CK_IA_MODULUS_BITS));
		ulKeyBytes = *(CK_ULONG_PTR)(pstPubAttr->pValue) / 8;
		if (ulDataLen > (ulKeyBytes - 11))
		{
			SYNO_LogE("ulDataLen is wrong! ulDataLen = 0x%x", ulDataLen);
			ulRet = CKR_DATA_LEN_RANGE;
			goto ERROR;
		}
		if (ulSignatureLen != ulKeyBytes)
		{
			SYNO_LogE("ulSignatureLen is wrong! ulSignatureLen = 0x%x", ulSignatureLen);
			ulRet = CKR_DATA_LEN_RANGE;
			goto ERROR;
		}

		// ��������
		memcpy(btDigestData, pData, ulDataLen);
		ulDigestLen = ulDataLen;
	}
	else
	{
		// �Ӵվ��
		hDigestKey = session_data->digest_state;
		if (NULL == hDigestKey)
		{
			SYNO_LogE("hDigestKey is null!");
			ulRet = CKR_GENERAL_ERROR;
			goto ERROR;
		}

		// �Ӵ�����
		ulRet = P11_Digest(hDigestKey, pData, ulDataLen, btDigestData, &ulDigestLen);
		if (SAR_OK != ulRet)
		{
			SYNO_LogE("P11_Digest() failed! ulRet = 0x%x", ulRet);
			ulRet = CKR_GENERAL_ERROR;
			goto ERROR;
		}
	}
		
	// ������ǩ��Կ
	ulRet = TOK_I_ConvertPubKeyObj(pstPubKeyObj, &stuRsaPubKey);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("TOK_I_ConvertPubKeyObj() failed! ulRet = 0x%x", ulRet);
		goto ERROR;
	}
	
	// ��ǩ����
	ulRet = P11_RSAVerify(hDev, &stuRsaPubKey, btDigestData, ulDigestLen, pSignature, ulSignatureLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_RSAVerify() failed! ulRet = 0x%x", ulRet);
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	FUNC_END;

	// ��ǩ����
ERROR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->verify_state = NULL;
	session_data->verify_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_VerifyUpdate
 *	˵��:��ǩ��������
 *	����:session_data:[IN]:�Ự����
 *       pPart:[IN]:ǩ���ķ���ԭ������
 *       ulPartLen:[IN]:����ԭ�����ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_VerifyUpdate(CK_I_SESSION_DATA_PTR session_data, 
					   CK_BYTE_PTR pPart, 
					   CK_ULONG ulPartLen)
{
	ULONG ulRet = CKR_OK;
	HANDLE hDigestKey = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pPart || 0 == ulPartLen)
    {
        SYNO_LogE("pPart is null or ulPartLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }

	// �ж��㷨
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{
        SYNO_LogE("This function doesn't support the sign mechanism! session_data->sign_mechanism = 0x%x", session_data->sign_mechanism);
		ulRet = CKR_MECHANISM_INVALID;
		goto ERROR;
	}
	
	// �Ӵվ��
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	
	// �Ӵ�����
	ulRet = P11_DigestUpdate(hDigestKey, pPart, ulPartLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestUpdate() failed! ulRet = 0x%x", ulRet);
        ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	FUNC_END;
	
	// ��ǩ����
ERROR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->verify_state = NULL;
	session_data->verify_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_VerifyFinal
 *	˵��:��ǩ����
 *	����:session_data:[IN]:�Ự����
 *       pSignature:[IN]:ǩ��ֵ
 *       pulSignatureLen:[IN]:ǩ��ֵ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_VerifyFinal(CK_I_SESSION_DATA_PTR session_data, 
					  CK_BYTE_PTR pSignature, 
					  CK_ULONG ulSignatureLen)
{
	ULONG ulRet = CKR_OK;
	ULONG ulDigestLen = 256;
	BYTE btDigestData[256] = {0};
	HANDLE hDigestKey = NULL;
	HCONTAINER hContainer = NULL;
	CK_I_OBJ_PTR pstPubKeyObj = NULL;
	RSAPUBLICKEYBLOB stuRsaPubKey = {0};

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pSignature || 0 == ulSignatureLen)
    {
        SYNO_LogE("pSignature is null or ulSignatureLen is 0!");
		return CKR_ARGUMENTS_BAD;
    }

	// �ж��㷨
	if (CKM_RSA_PKCS == session_data->sign_mechanism)
	{
        SYNO_LogE("This function doesn't support the sign mechanism! session_data->sign_mechanism = 0x%x", session_data->sign_mechanism);
		ulRet = CKR_MECHANISM_INVALID;
		goto ERROR;
	}

	// �Ӵվ��
	hDigestKey = session_data->digest_state;
	if (NULL == hDigestKey)
	{
        SYNO_LogE("hDigestKey is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	// ��ȡ�Ӵս��
	ulRet = P11_DigestFinal(hDigestKey, btDigestData, &ulDigestLen);
    if (SAR_OK != ulRet)
    {
        SYNO_LogE("P11_DigestFinal() failed! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
    }
	
	// ��ȡ��ǩ��Կ��������
	pstPubKeyObj = (CK_I_OBJ_PTR)session_data->sign_state;
	if (NULL == pstPubKeyObj)
	{
        SYNO_LogE("pstPubKeyObj is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
	hContainer = pstPubKeyObj->CtrHandle;
	if (NULL == hContainer)
	{
        SYNO_LogE("hContainer is null!");
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}
			
	// ������ǩ��Կ
	ulRet = TOK_I_ConvertPubKeyObj(pstPubKeyObj, &stuRsaPubKey);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("TOK_I_ConvertPubKeyObj() failed! ulRet = 0x%x", ulRet);
		goto ERROR;
	}

	// ��ǩ����
	ulRet = P11_RSAVerify(hContainer, &stuRsaPubKey, btDigestData, ulDigestLen, pSignature, ulSignatureLen);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("P11_RSAVerify() failed()! ulRet = 0x%x", ulRet);
		ulRet = CKR_GENERAL_ERROR;
		goto ERROR;
	}

	FUNC_END;	
	
	// ��ǩ����
ERROR:
	if (hDigestKey)
	{
		P11_CloseHandle(hDigestKey);
		hDigestKey = NULL;
	}
	session_data->digest_state = NULL;
	session_data->digest_mechanism = 0;
	session_data->verify_state = NULL;
	session_data->verify_mechanism = 0;

	return ulRet;
}
/***********************************************************************
 *	����:TOK_VerifyRecoverInit
 *	˵��:�ָ�ǩ�������ʼ��
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[OUT]:ǩ���㷨
 *       key_obj:[OUT]:ǩ����Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_VerifyRecoverInit(CK_I_SESSION_DATA_PTR session_data, 
							CK_MECHANISM_PTR pMechanism, 
							CK_I_OBJ_PTR key_obj)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	����:TOK_VerifyRecover
 *	˵��:�ָ�ǩ������
 *	����:session_data:[IN]:�Ự����
 *       pSignature:[IN]:ǩ��ֵ
 *       ulSignatureLen:[IN]:ǩ��ֵ����
 *       pData:[OUT]:���ص�ԭ������
 *       pulDataLen:[OUT]:ԭ�����ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_VerifyRecover(CK_I_SESSION_DATA_PTR session_data, 
						CK_BYTE_PTR pSignature,
						CK_ULONG ulSignatureLen,
						CK_BYTE_PTR pData, 
						CK_ULONG_PTR pulDataLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	����:TOK_DigestEncryptUpdate
 *	˵��:�Ӵռ���˫����
 *	����:session_data:[IN]:�Ự����
 *       pPart:[IN]:��������
 *       ulPartLen:[IN]:�������ݳ���
 *       pEncryptedPart:[OUT]:���ص�����
 *       pulEncryptedPartLen:[OUT]:���ĳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DigestEncryptUpdate(CK_I_SESSION_DATA_PTR session_data, 
							  CK_BYTE_PTR pPart, 
							  CK_ULONG ulPartLen, 
							  CK_BYTE_PTR pEncryptedPart, 
							  CK_ULONG_PTR pulEncryptedPartLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	����:TOK_DecryptDigestUpdate
 *	˵��:�����Ӵ�˫����
 *	����:session_data:[IN]:�Ự����
 *       pEncryptedPart:[IN]:����
 *       ulEncryptedPartLen:[IN]:���ĳ���
 *       pPart:[OUT]:���ܺ������
 *       pulPartLen:[OUT]:���ܺ�����ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DecryptDigestUpdate(CK_I_SESSION_DATA_PTR session_data, 
							  CK_BYTE_PTR pEncryptedPart, 
							  CK_ULONG ulEncryptedPartLen, 
							  CK_BYTE_PTR pPart, 
							  CK_ULONG_PTR pulPartLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	����:TOK_SignEncryptUpdate
 *	˵��:ǩ������˫����
 *	����:session_data:[IN]:�Ự����
 *       pPart:[IN]:��������
 *       ulPartLen:[IN]:�������ݳ���
 *       pEncryptedPart:[OUT]:���ص�����
 *       pulEncryptedPartLen:[OUT]:���ĳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SignEncryptUpdate(CK_I_SESSION_DATA_PTR session_data, 
							CK_BYTE_PTR pPart, 
							CK_ULONG ulPartLen, 
							CK_BYTE_PTR pEncryptedPart, 
							CK_ULONG_PTR pulEncryptedPartLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	����:TOK_DecryptVerifyUpdate
 *	˵��:������ǩ˫����
 *	����:session_data:[IN]:�Ự����
 *       pEncryptedPart:[IN]:����
 *       ulEncryptedPartLen:[IN]:���ĳ���
 *       pPart:[OUT]:���ܺ������
 *       pulPartLen:[OUT]:���ܺ�����ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DecryptVerifyUpdate(CK_I_SESSION_DATA_PTR session_data, 
							  CK_BYTE_PTR pEncryptedPart, 
							  CK_ULONG ulEncryptedPartLen, 
							  CK_BYTE_PTR pPart, 
							  CK_ULONG_PTR pulPartLen)
{
	return CKR_FUNCTION_NOT_SUPPORTED;
}
/***********************************************************************
 *	����:TOK_GenerateKey
 *	˵��:���ɻỰ��Կ
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[IN]:�Ự��Կ�㷨
 *       key:[IN]:��Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_GenerateKey(CK_I_SESSION_DATA_PTR session_data,
					  CK_MECHANISM_PTR pMechanism, 
					  CK_I_OBJ_PTR key)
{
    ULONG ulRet = CKR_OK;
    ULONG ulAlgId = 0;
	DEVHANDLE hDev = NULL;
    HANDLE hSessionKey = NULL;

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
        return CKR_GENERAL_ERROR;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_GENERAL_ERROR;
    }
    if (NULL == key)
    {
        SYNO_LogE("key is null!");
        return CKR_GENERAL_ERROR;
    }

	// ��ȡ�豸���
	if (!session_data->slot_data)
	{
        SYNO_LogE("session_data->slot_data is null!");
        return CKR_GENERAL_ERROR;
	}
	hDev = session_data->slot_data->pDevHandle;
	if (!hDev)
	{
        SYNO_LogE("hDev is null!");
        return CKR_GENERAL_ERROR;
	}

	// �㷨
	switch (pMechanism->mechanism)
	{
	case CKM_DES_KEY_GEN:
		ulAlgId = SGD_DES;
		break;
	case CKM_DES3_KEY_GEN:
		ulAlgId = SGD_DES3;
		break;
	case CKM_AES_KEY_GEN:
		ulAlgId = SGD_AES128;
		break;
	default:
        SYNO_LogE("pMechanism->mechanism is invalid! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}
	
    // ���ɻỰ��Կ
	ulRet = P11_GenSymmKey(hDev, ulAlgId, &hSessionKey);
    if (CKR_OK != ulRet)
    {
        SYNO_LogE("P11_GenSymmKey() failed! ulRet = 0x%x", ulRet);
        return CKR_GENERAL_ERROR;
    }

	// �������
	key->ObjHandle = hSessionKey;

	FUNC_END;

    return ulRet;
}
/***********************************************************************
 *	����:TOK_GenerateKeyPair
 *	˵��:������Կ��
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[IN]:��Կ�㷨
 *       public_key:[IN]:��Կ����
 *       private_key:[IN]:˽Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_GenerateKeyPair(CK_I_SESSION_DATA_PTR session_data,
						  CK_MECHANISM_PTR pMechanism,
						  CK_I_OBJ_PTR public_key,
						  CK_I_OBJ_PTR private_key)
{
    CK_RV ulRet = CKR_OK;
	ULONG ulValueLen = 0;
	CK_BBOOL bIsToken = FALSE;
	CK_BBOOL bIsSign = FALSE;
	CK_BBOOL bIsExch = FALSE;
	CK_BBOOL bIsEncrypt = FALSE;
	ULONG ulKeyType = 0;
	ULONG ulBits = 0;
	LPSTR lpNewContainer = NULL;
	HAPPLICATION hApp = NULL;
    HCONTAINER hContainer = NULL;
    RSAPUBLICKEYBLOB stBlob = {0};
	ULONG ulRsaPubKeyLen = 0;	
    ULONG ulContainerCount = 0;
    CHAR acName[PATH_MAX] = {0};
    ULONG ulSize = PATH_MAX;
	CK_ATTRIBUTE rsaPubKeyAttr = {CKA_MODULUS, NULL, 0};

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == public_key)
    {
        SYNO_LogE("public_key is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == private_key)
    {
        SYNO_LogE("private_key is null!");
        return CKR_ARGUMENTS_BAD;
    }
	
	// ��ȡӦ�þ��
	hApp = session_data->AppHandle;
	if (NULL == hApp)
	{
        SYNO_LogE("hApp is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}

	// ��鹫Կ��CKA_TOKEN����
	ulValueLen = 1;
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_TOKEN, (CK_BYTE_PTR)&bIsToken, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_TOKEN from public_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (!bIsToken)
	{
		SYNO_LogE("CKA_TOKEN value of public_key is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// ���˽Կ��CKA_TOKEN����
	bIsToken = FALSE;
	ulRet = CI_ObjGetAttributeValue(private_key, CKA_TOKEN, (CK_BYTE_PTR)&bIsToken, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_TOKEN from private_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (!bIsToken)
	{
		SYNO_LogE("CKA_TOKEN value of private_key is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// ��鹫Կ��CKA_KEY_TYPE����
	ulValueLen = 4;
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_KEY_TYPE, (CK_BYTE_PTR)&ulKeyType, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_KEY_TYPE from public_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (CKK_RSA != ulKeyType)
	{
		SYNO_LogE("CKA_TOKEN value of public_key is wrong! ulKeyType = 0x%x", ulKeyType);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// ���˽Կ��CKA_KEY_TYPE����
	ulKeyType = 0;
	ulRet = CI_ObjGetAttributeValue(private_key, CKA_KEY_TYPE, (CK_BYTE_PTR)&ulKeyType, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_KEY_TYPE from private_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (CKK_RSA != ulKeyType)
	{
		SYNO_LogE("CKA_TOKEN value of private_key is wrong! ulKeyType = 0x%x", ulKeyType);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
	
	// ��ȡ��Կģ��bits
	ulKeyType = 4;
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_MODULUS_BITS, (CK_BYTE_PTR)&ulBits, &ulValueLen);
	if (CKR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_MODULUS_BITS from public_key failed!ulRet = 0x%x", ulRet);
		return ulRet;
	}
	if (1024 != ulBits && 2048 != ulBits)
	{
		SYNO_LogE("CKA_MODULUS_BITS value of public_key is wrong! ulBits = 0x%x", ulBits);
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}
		
	// ��ȡ˽Կ��;
	ulValueLen = sizeof(CK_BBOOL);
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_VERIFY, (CK_BYTE_PTR)&bIsSign, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_VERIFY from public_key failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_WRAP, (CK_BYTE_PTR)&bIsExch, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_WRAP from public_key failed! ulRet = 0x%x", ulRet);
	}
	ulRet = CI_ObjGetAttributeValue(public_key, CKA_ENCRYPT, (CK_BYTE_PTR)&bIsEncrypt, &ulValueLen);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("Call CI_ObjGetAttributeValue() to get CKA_ENCRYPT from public_key failed! ulRet = 0x%x", ulRet);
	}
	if (!bIsSign && !bIsExch && !bIsEncrypt)
	{
		SYNO_LogE("All of CKA_VERIFY, CKA_WRAP and CKA_ENCRYPT attribute is FALSE!");
		return CKR_ATTRIBUTE_VALUE_INVALID;
	}

	// ���˽Կ��;��Ϣ
	if (bIsSign) SYNO_LogD("This key pair can be use to sign/verify data.");
	if (bIsExch) SYNO_LogD("This key pair can be use to wrap/unwrap session key.");
	if (bIsEncrypt) SYNO_LogD("This key pair can be use to encrypt/decrypt data.");
		
	// ��������
	do
	{
		lpNewContainer = TOK_I_GenUniqeContainerId();
		ulRet = P11_CreateContainer(hApp, lpNewContainer, &hContainer);
	}while (SAR_COS_CONTAINER_ALREADY_EXISTS == ulRet);
	if (SAR_OK != ulRet)
	{
		SYNO_LogE("P11_CreateContainer() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
	}
	SYNO_LogD("New container: %s is created��", lpNewContainer);

	// ����RSA��Կ��
    ulRet = P11_GenRSAKeyPair(hContainer, bIsSign ? 1 : 0, ulBits, &stBlob);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("P11_GenRSAKeyPair() failed! ulRet = 0x%x", ulRet);
        return ulRet;
    }

	// ���ù�Կģ����	
	rsaPubKeyAttr.ulValueLen = stBlob.BitLen / 8;
	rsaPubKeyAttr.pValue = SYNO_calloc(1, stBlob.BitLen / 8);
	memcpy(rsaPubKeyAttr.pValue, stBlob.Modulus + MAX_RSA_MODULUS_BITS/8-stBlob.BitLen/8, stBlob.BitLen / 8);
	ulRet = CI_ObjSetAttribute(public_key, &rsaPubKeyAttr);
    if (CKR_OK != ulRet)
    {
		SYNO_LogE("CI_ObjSetAttribute() failed! ulRet = 0x%x", ulRet);
		SYNO_free(rsaPubKeyAttr.pValue);
        return ulRet;
    }

	// �����������
	public_key->CtrHandle = hContainer;
	private_key->CtrHandle = hContainer;

	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_WrapKey
 *	˵��:����Ự��Կ
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[IN]:��Կ�㷨
 *       wrap_key_obj:[IN]:��������Ự��Կ�Ĺ�Կ����
 *       key_obj:[IN]:������ĻỰ��Կ����
 *       pWrappedKey:[OUT]:���صĴ���������
 *       pulWrappedKeyLen:[OUT]:������ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_WrapKey(CK_I_SESSION_DATA_PTR session_data,
				  CK_MECHANISM_PTR pMechanism,
				  CK_I_OBJ_PTR wrap_key_obj, 
				  CK_I_OBJ_PTR key_obj, 
				  BYTE* pWrappedKey, 
				  CK_RV* pulWrappedKeyLen)
{
    CK_RV ulRet = CKR_OK;
	RSAPUBLICKEYBLOB stRSAPubBlob = {0};
	BYTE EncSessionkey[128] = {0};
	HANDLE pSessionKey = NULL;
	ULONG ulRetLen = 0;

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == wrap_key_obj)
    {
        SYNO_LogE("wrap_key_obj is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == key_obj)
    {
        SYNO_LogE("key_obj is null!");
        return CKR_ARGUMENTS_BAD;
	}
    if (NULL == pWrappedKey)
    {
        SYNO_LogE("pucWrappedKey is null!");
        return CKR_ARGUMENTS_BAD;
	}

	// Wrap��ʽ���
	if (pMechanism->mechanism != CKM_RSA_PKCS)
	{
        SYNO_LogE("pMechanism->mechanism is wrong, only support CKM_RSA_PKCS! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}
	
	// ����Wrap Key�Ĺ�Կ
	ulRet = TOK_I_ConvertPubKeyObj(wrap_key_obj, &stRSAPubBlob);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("TOK_I_ConvertPubKeyObj() failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}

	// ����ָ����Key����
	ulRet = P11_RSAExportSessionKeyEx(key_obj->ObjHandle, &stRSAPubBlob, pWrappedKey, pulWrappedKeyLen);
	if (SAR_OK != ulRet)
	{
        SYNO_LogE("P11_RSAExportSessionKeyEx() failed! ulRet = 0x%x", ulRet);
		return ulRet;
	}

	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_UnwrapKey
 *	˵��:����Ự��Կ
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[IN]:��Կ�㷨
 *       unwrap_key_obj:[IN]:��������Ự��Կ��˽Կ����
 *       pWrappedKey:[IN]:�����Ļػ���Կ����
 *       pulWrappedKeyLen:[IN]:���ݳ���
 *       key_obj:[OUT]:���صĻỰ��Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_UnwrapKey(CK_I_SESSION_DATA_PTR  session_data,
					CK_MECHANISM_PTR pMechanism,
					CK_I_OBJ_PTR unwrap_key_obj, 
					BYTE* pWrappedKey, 
					ULONG ulWrappedKeyLen, 
					CK_I_OBJ_PTR key_obj)
{
    ULONG ulRet = CKR_OK;
    HCONTAINER hContainer = NULL;
    ULONG ulAlgId = 0;
    HANDLE hKey = NULL;
	CK_MECHANISM_TYPE_PTR pMechanismType = NULL;

	FUNC_BEGIN;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pMechanism)
    {
        SYNO_LogE("pMechanism is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == unwrap_key_obj)
    {
        SYNO_LogE("unwrap_key_obj is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pWrappedKey)
    {
        SYNO_LogE("pWrappedKey is null!");
        return CKR_ARGUMENTS_BAD;
	}
    if (NULL == key_obj)
    {
        SYNO_LogE("pstKeyObj is null!");
        return CKR_ARGUMENTS_BAD;
	}
	
	// Unwrap��ʽ���
	if (pMechanism->mechanism != CKM_RSA_PKCS)
	{
        SYNO_LogE("pMechanism->mechanism is wrong, only support CKM_RSA_PKCS! pMechanism->mechanism = 0x%x", pMechanism->mechanism);
		return CKR_MECHANISM_INVALID;
	}

	// ��ȡKey�㷨
	pMechanismType = (CK_MECHANISM_TYPE_PTR)CI_ObjLookup(key_obj, CK_IA_KEY_TYPE)->pValue;
	switch (*pMechanismType)
	{
	case CKK_DES:
		ulAlgId = SGD_DES;
		break;
	case CKK_DES3:
		ulAlgId = SGD_DES3;
		break;
	case CKK_AES:
		ulAlgId = SGD_AES128;
		break;
	default:
        SYNO_LogE("CKA_KEY_TYPE value in Unwrap key mechanism is wrong! Value = 0x%x", *pMechanism);
		return CKR_MECHANISM_PARAM_INVALID;
	}
	
    // ����Ự��Կ
    ulRet = P11_ImportSessionKey(unwrap_key_obj->CtrHandle, ulAlgId, pWrappedKey, ulWrappedKeyLen, &hKey);
    if (CKR_OK != ulRet)
    {
        SYNO_LogE("P11_ImportSessionKey() failed! ulRet = 0x%x", ulRet);
		return CKR_GENERAL_ERROR;
    }

    key_obj->ObjHandle = hKey;
	key_obj->CtrHandle = unwrap_key_obj->CtrHandle;
	
	FUNC_END;

    return CKR_OK;
}
 /***********************************************************************
 *	����:TOK_DeriveKey
 *	˵��:����һ���Ự��Կ
 *	����:session_data:[IN]:�Ự����
 *       pMechanism:[IN]:��Կ�㷨
 *       base_key:[IN]:������Կ����
 *       derived_ky:[OUT]:���صĻỰ��Կ����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_DeriveKey(CK_I_SESSION_DATA_PTR session_data, 
					CK_MECHANISM_PTR pMechanism, 
					CK_I_OBJ_PTR base_key, 
					CK_I_OBJ_PTR derived_ky)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	����:TOK_SeedRandom
 *	˵��:���������������
 *	����:session_data:[IN]:�Ự����
 *       pSeed:[IN]:��������
 *       ulSeedLen:[IN]:�������ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_SeedRandom(CK_I_SESSION_DATA_PTR session_data, 
					 CK_BYTE_PTR pSeed, 
					 CK_ULONG ulSeedLen)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	����:TOK_GenerateRandom
 *	˵��:���������
 *	����:session_data:[IN]:�Ự����
 *       pRandomData:[OUT]:�������������
 *       ulRandomLen:[IN]:���ݳ���
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_GenerateRandom(CK_I_SESSION_DATA_PTR session_data,
						 CK_BYTE_PTR pRandomData, 
						 CK_ULONG ulRandomLen)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	����:TOK_GetFunctionStatus
 *	˵��:���غ���״̬
 *	����:session_data:[IN]:�Ự����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_GetFunctionStatus(CK_I_SESSION_DATA_PTR session_data)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	����:TOK_GetFunctionStatus
 *	˵��:ȡ������
 *	����:session_data:[IN]:�Ự����
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_CancelFunction(CK_I_SESSION_DATA_PTR session_data)
{
	return CKR_FUNCTION_FAILED;
}
 /***********************************************************************
 *	����:TOK_WaitForSlotEvent
 *	˵��:�ȴ�Slotʵ��
 *	����:flags:[IN]:�ȴ����
 *      pSlot:[OUT]:�����¼���Slot ID
 *      pRserved:[OUT]:��������
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_WaitForSlotEvent(CK_FLAGS flags, 
						   CK_SLOT_ID_PTR pSlot, 
						   CK_VOID_PTR pRserved)
{
	CK_RV rv = CKR_OK;
	CK_BYTE devName[512] = {0};
	CK_ULONG nameLen = 512;
	CK_ULONG eventType = 0;
	CK_ULONG eventSlotID = 0;

	FUNC_BEGIN;
	
	//	�ݲ�֧�ַ�����ģʽ����Ϊ���õĹ��ܽӿ���������
	if (CKF_DONT_BLOCK == flags)
	{
        SYNO_LogE("flags is wrong! Only support block mode! flags = 0x%x", flags);
		return CKR_FUNCTION_NOT_SUPPORTED;
	}
	if (NULL == pSlot)
	{
        SYNO_LogE("pSlot is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	//	�ȴ��豸�¼�
	rv = P11_WaitForDevEvent((LPSTR)devName, &nameLen, &eventType);
	if (SAR_OK != rv)
	{
        SYNO_LogE("P11_WaitForDevEvent() failed! rv = 0x%x", rv);
		return CKR_FUNCTION_FAILED;
	}
	if (strlen((LPSTR)devName) == 0 || nameLen <= 2)
	{
        SYNO_LogE("P11_WaitForDevEvent() return with devName is NULL!");
		return CKR_GENERAL_ERROR;
	}

	//	�����豸������Ϣ
	if (1 == eventType)
	{
		CK_I_SLOT_DATA_PTR pSlotData = NULL_PTR;
        rv = TOK_TokenInitialize(devName, &pSlotData);
        if (CKR_OK != rv)
        {
            SYNO_LogE("TOK_TokenInitialize err!");
            return rv;
        }

		eventSlotID = CK_I_SLOT_ANZ;
        rv = CI_RegisterSlot(eventSlotID, pSlotData);
        if (CKR_OK != rv)
        {
			SYNO_free(pSlotData);
            SYNO_LogE("CI_RegisterSlot err!");
            return rv;
        }
	}
	//	���豸�Ƴ��¼�
	else if (2 == eventType)
	{	
		CK_SLOT_ID_PTR slot_list = NULL_PTR;
		CK_ULONG slot_num = 0;
		CK_ULONG i = 0;
		
		rv = C_GetSlotList(TRUE, NULL_PTR, &slot_num);
		if (rv != CKR_OK)
		{
			SYNO_LogE("C_GetSlotList() failed! rv = 0x%x", rv);
			return rv;
		}

		slot_list = (CK_SLOT_ID_PTR)SYNO_malloc(slot_num * sizeof(CK_SLOT_ID_PTR));
		if (slot_list == NULL_PTR)
		{
			SYNO_LogE("Getting space for slot list failed! rv = 0x%x", rv);
			return CKR_HOST_MEMORY;
		}
		rv = C_GetSlotList(TRUE, slot_list, &slot_num);
		if (rv != CKR_OK)
		{
			SYNO_free(slot_list);
			SYNO_LogE("C_GetSlotList() failed! rv = 0x%x", rv);
			return rv;
		}

		rv = CKR_NO_EVENT;
		for (i = 0 ; i < slot_num; i++)
		{
			CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;
			CI_GetSlotData(slot_list[i], &slot_data);
			if (slot_data && (strcmp((CHAR*)slot_data->config_section_name, (CHAR*)devName) == 0))
			{
				rv = CI_RemoveToken(slot_list[i]);
				if (rv != CKR_OK)
				{
					SYNO_LogE("CI_RemoveToken() failed in slot %d! rv = 0x%x", slot_list[i], rv);
					//return rv;
				}
				eventSlotID = slot_list[i];
				SYNO_free(slot_list);
				rv = CKR_OK;
				break;
			}
		}
	}
	else
	{
        SYNO_LogE("P11_WaitForDevEvent() return a wrong event type! eventType = 0x%x", eventType);
		return CKR_GENERAL_ERROR;
	}

	*pSlot = eventSlotID;
	if (pRserved)
	{
		*((ULONG*)pRserved) = eventType;
	}

	FUNC_END;
	
	return rv;
}
 /***********************************************************************
 *	����:TOK_TokenObjRetrieve
 *	˵��:�ָ�һ�����������??
 *	����:session_data:[IN]:�Ự����
 *      phObject:[IN]:������
 *      ppNewObject:[OUT]:���صĶ���ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_TokenObjRetrieve(CK_I_SESSION_DATA_PTR session_data, 
						   CK_OBJECT_HANDLE phObject, 
						   CK_I_OBJ_PTR CK_PTR ppNewObject)
{
	return CKR_FUNCTION_FAILED;
}
/***********************************************************************
 *	����:TOK_TokenObjCommit
 *	˵��:���ö������ԣ�����������CKO_DATA��ֵ��
 *	����:session_data:[IN]:�Ự����
 *      phObject:[IN]:������
 *      ppNewObject:[OUT]:���صĶ���ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_TokenObjCommit(CK_I_SESSION_DATA_PTR session_data, 
						 CK_OBJECT_HANDLE hObject, 
						 CK_I_OBJ_PTR pObject)
{
	CK_RV rv = CKR_OK;
	CK_BBOOL bIsToken = CK_FALSE;
	CK_ULONG ulIsTokenLen = sizeof(CK_BBOOL);
	CK_OBJECT_CLASS ulObjClass = CKO_VENDOR_DEFINED;  
	CK_ULONG ulObjClassLen = sizeof(CK_OBJECT_CLASS);
	CK_ATTRIBUTE_PTR pAttribute = NULL;
	HAPPLICATION hApp = NULL;
	LPSTR lpFileName = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == hObject)
    {
        SYNO_LogE("hObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pObject)
    {
        SYNO_LogE("pObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
	
	// ��ȡӦ�þ��
	hApp = session_data->AppHandle;
	if (NULL == hApp)
	{
        SYNO_LogE("hApp is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}
	
	// ���CKA_TOKEN�����Ƿ�ΪTRUE
	rv = CI_ObjGetIntAttributeValue(pObject, CK_IA_TOKEN, &bIsToken, &ulIsTokenLen);
	if (CKR_OK != rv || CK_FALSE == bIsToken)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() with CK_IA_TOKEN attribute value is FALSE! rv = 0x%x, bIsToken = %d", rv, bIsToken);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	// ��ȡ�������
	rv = CI_ObjGetIntAttributeValue(pObject, CK_IA_CLASS, (CK_BYTE_PTR)&ulObjClass, &ulObjClassLen);
	if (CKR_OK != rv)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() with CK_IA_CLASS failed! rv = 0x%x", rv);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}
	if (CKO_DATA != ulObjClass)
	{
        SYNO_LogE("Object class is wrong! ulObjClass = 0x%x", ulObjClass);
		return CKR_FUNCTION_NOT_SUPPORTED;
	}

	// ��ȡ�ļ���	
	pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pObject, CK_IA_ID));
	if (NULL == pAttribute)
	{
        SYNO_LogE("Attribute CK_IA_ID is not find!");
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}
	lpFileName = (LPSTR)SYNO_malloc(pAttribute->ulValueLen + 1);
	memcpy(lpFileName, pAttribute->pValue, pAttribute->ulValueLen);
	lpFileName[pAttribute->ulValueLen] = '\0';

	// ��ȡ�ļ�����	
	pAttribute = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pObject, CK_IA_VALUE));
	if (NULL == pAttribute)
	{
        SYNO_LogE("Attribute CK_IA_VALUE is not find!");
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	// д�ļ�
	rv = P11_WriteFile(hApp, lpFileName, 0, (BYTE*)pAttribute->pValue, pAttribute->ulValueLen);
	if (SAR_OK != rv)
	{
        SYNO_LogE("P11_WriteFile() failed! rv = 0x%x", rv);
		rv = CKR_FUNCTION_FAILED;
		goto FREE;
	}

	FUNC_END;

FREE:
	if (lpFileName)
	{
		SYNO_free(lpFileName);
		lpFileName = NULL;
	}

	return rv;
}
/***********************************************************************
 *	����:TOK_TokenObjAdd
 *	˵��:���һ������
 *	����:session_data:[IN]:�Ự����
 *      phObject:[IN]:������
 *      ppNewObject:[OUT]:���صĶ���ָ��
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_TokenObjAdd(CK_I_SESSION_DATA_PTR session_data, 
					  CK_OBJECT_HANDLE phObject, 
					  CK_I_OBJ_PTR pNewObject)
{
	CK_RV rv = CKR_OK;
	CK_BBOOL bIsToken = CK_FALSE;
	CK_ULONG ulIsTokenLen = sizeof(CK_BBOOL);
	CK_OBJECT_CLASS ulObjClass = CKO_VENDOR_DEFINED;  
	CK_ULONG ulObjClassLen = sizeof(CK_OBJECT_CLASS);
	DEVHANDLE hDev = NULL;
	HAPPLICATION hApp = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == phObject)
    {
        SYNO_LogE("phObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
    if (NULL == pNewObject)
    {
        SYNO_LogE("pNewObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
	
	// ��ȡ�豸���
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}

	// ��ȡӦ�þ��
	hApp = session_data->AppHandle;
	if (NULL == hApp)
	{
        SYNO_LogE("hApp is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}

	// ���CKA_TOKEN�����Ƿ�ΪTRUE
	rv = CI_ObjGetIntAttributeValue(pNewObject, CK_IA_TOKEN, &bIsToken, &ulIsTokenLen);
	if (CKR_OK != rv || CK_FALSE == bIsToken)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() or CK_IA_TOKEN attribute value is FALSE! rv = 0x%x, bIsToken = %d", rv, bIsToken);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	// ��ȡ�������
	rv = CI_ObjGetIntAttributeValue(pNewObject, CK_IA_CLASS, (CK_BYTE_PTR)&ulObjClass, &ulObjClassLen);
	if (CKR_OK != rv)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() failed! rv = 0x%x", rv);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}
	SYNO_LogD("Object class is: 0x%x", ulObjClass);

	// ���ݲ�ͬ�Ķ������ͣ����������ݵ���Token��
	switch (ulObjClass)
	{
	case CKO_PUBLIC_KEY:
		rv = TOK_I_ImportPublicKey(hApp, pNewObject);
		break;
	case CKO_PRIVATE_KEY:
		rv = TOK_I_ImportPrivateKey(hApp, pNewObject);
		break;
	case CKO_SECRET_KEY:
		rv = TOK_I_ImportSessionKey(hDev, pNewObject);
		break;
	case CKO_CERTIFICATE:
		rv = TOK_I_ImportCertificate(hApp, pNewObject);
		break;
	case CKO_DATA:
		rv = TOK_I_ImportDataFile(hApp, pNewObject);
		break;
	default:
		SYNO_LogE("This object class is can't add to token!");
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	FUNC_END;

	return rv;
}
/***********************************************************************
 *	����:TOK_TokenObjDelete
 *	˵��:ɾ��һ������
 *	����:session_data:[IN]:�Ự����
 *      phObject:[IN]:������
 *	����:�ɹ�����CR_OK�����򷵻ش�����롣
 ***********************************************************************/
CK_RV TOK_TokenObjDelete(CK_I_SESSION_DATA_PTR session_data, 
						 CK_OBJECT_HANDLE phObject)
{
	CK_RV rv = CKR_OK;
	CK_I_OBJ_PTR obj = NULL_PTR;
	CK_OBJECT_CLASS ulObjClass = CKO_VENDOR_DEFINED;  
	CK_ULONG ulObjClassLen = sizeof(CK_OBJECT_CLASS);
	DEVHANDLE hDev = NULL;
	HAPPLICATION hApp = NULL;

	FUNC_BEGIN;
	
	// �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
		return CKR_ARGUMENTS_BAD;
    }
    if (NULL == phObject)
    {
        SYNO_LogE("phObject is null!");
        return CKR_ARGUMENTS_BAD;
    }
	
	// ��ȡ�豸���
	hDev = session_data->slot_data->pDevHandle;
	if (NULL == hDev)
	{
        SYNO_LogE("hDev is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}

	// ��ȡӦ�þ��
	hApp = session_data->AppHandle;
	if (NULL == hApp)
	{
        SYNO_LogE("hApp is null!");
		return CKR_SESSION_HANDLE_INVALID;
	}
	
	// ��ȡObject����
	rv = CI_ReturnObj(session_data, phObject, &obj);
    if (CKR_OK != rv || NULL == obj)
    {
        SYNO_LogE("CI_ReturnObj() failed or obj is null! rv = 0x%x", rv);
        return rv;
    }

	// ��ȡ�������
	rv = CI_ObjGetIntAttributeValue(obj, CK_IA_CLASS, (CK_BYTE_PTR)&ulObjClass, &ulObjClassLen);
	if (CKR_OK != rv)
	{
        SYNO_LogE("CI_ObjGetIntAttributeValue() failed! rv = 0x%x", rv);
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}
	SYNO_LogD("Object class is: 0x%x", ulObjClass);

	// ���ݲ�ͬ�Ķ������ͣ����������ݵ���Token��
	switch (ulObjClass)
	{
	case CKO_PUBLIC_KEY:
	case CKO_PRIVATE_KEY:
	case CKO_CERTIFICATE:
		// ֱ��ɾ��������������
		rv = TOK_I_DeleteObjContainer(hApp, obj);
		if (CKR_OK != rv)
		{
			SYNO_LogE("TOK_I_DeleteObjContainer() failed! rv = 0x%x", rv);
			return rv;
		}
		break;
	case CKO_SECRET_KEY:
		rv = P11_CloseHandle(obj->ObjHandle);
		if (CKR_OK != rv)
		{
			SYNO_LogE("P11_CloseHandle() failed! rv = 0x%x", rv);
			return rv;
		}
		obj->ObjHandle = NULL;
		break;
	case CKO_DATA:
		{
			ULONG ulLen = 256;
			CHAR csFileName[256] = {0};
			rv = CI_ObjGetIntAttributeValue(obj, CK_IA_ID, (CK_BYTE_PTR)csFileName, &ulLen);
			if (CKR_OK != rv)
			{
				SYNO_LogE("CI_ObjGetIntAttributeValue() get CK_IA_ID failed! rv = 0x%x", rv);
				return rv;
			}
			rv = P11_DeleteFile(hApp, csFileName);
			if (CKR_OK != rv)
			{
				SYNO_LogE("P11_DeleteFile() file %s failed! rv = 0x%x", csFileName, rv);
				return rv;
			}
		}
		break;
	default:
		SYNO_LogE("This object class is can't add to token!");
		return CKR_ATTRIBUTE_TYPE_INVALID;
	}

	FUNC_END;

	return rv;
}

CK_RV TOK_CreateObject(CK_I_SESSION_DATA_PTR session_data,
					   CK_I_OBJ_PTR pstObj)
{
    ULONG ulRet = CKR_OK;
    CK_CHAR  aucLabel[PATH_MAX] = {0};
    CK_ATTRIBUTE_PTR pstTemplate = NULL;
    HCONTAINER hContainer = NULL;
    RSAPUBLICKEYBLOB stRSAPubBlob = {0};
    ULONG ulBlobLen = 0;
    HANDLE hKey = NULL;
    ULONG ulContainerCount = 0;

    // �������
    if (NULL == session_data)
    {
        SYNO_LogE("session_data is null!");
        return CKR_GENERAL_ERROR;
    }
    if (NULL == pstObj)
    {
        SYNO_LogE("pstObj is null!");
        return CKR_GENERAL_ERROR;
    }

	// ����ģ��
    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstObj, CK_IA_ID));
    if (NULL == pstTemplate)
    {
        return CKR_OK;
    }

    strncpy((char*)aucLabel, (char*)pstTemplate->pValue, pstTemplate->ulValueLen);

    /* �������� */
    ulRet = P11_CreateContainer(session_data->AppHandle, (CHAR *)aucLabel, &hContainer);
	if (SAR_COS_FILE_ALREADY_EXIST == ulRet)
	{
		ulRet = P11_OpenContainer(session_data->AppHandle, (CHAR *)aucLabel, &hContainer);
	}
    if (CKR_OK != ulRet)
    {
        return ulRet;
    }

    if(*((CK_OBJECT_CLASS CK_PTR)CI_ObjLookup(pstObj,CK_IA_CLASS)->pValue) == CKO_PUBLIC_KEY)
    {
        //stRSAPubBlob.AlgID = SGD_RSA;
    }

    stRSAPubBlob.BitLen = *((CK_ULONG_PTR)CI_ObjLookup(pstObj, CK_IA_MODULUS_BITS)->pValue);

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstObj, CK_IA_MODULUS));
    memcpy(stRSAPubBlob.Modulus, pstTemplate->pValue, pstTemplate->ulValueLen);

    pstTemplate = (CK_ATTRIBUTE_PTR)(CI_ObjLookup(pstObj, CK_IA_PUBLIC_EXPONENT));
    memcpy(stRSAPubBlob.PublicExponent, pstTemplate->pValue, pstTemplate->ulValueLen);

    ulBlobLen = sizeof(stRSAPubBlob);
	ulRet = P11_ImportPublicKey(hContainer, FALSE, (CK_BYTE_PTR)&stRSAPubBlob, ulBlobLen, &hKey);
    if (CKR_OK != ulRet)
    {
        return ulRet;
    }

    pstObj->ObjHandle = hKey;

    ulContainerCount = session_data->CtrCount;
    session_data->aCtrHandle[ulContainerCount] = hContainer;
    session_data->CtrCount++;

	FUNC_END;

    return ulRet;
}
