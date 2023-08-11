/***************************************************************************
 *	File name:SynoTFKeyP11.h
 *	Introduce:Syno TF Key command for P11 functions definition
 *	Author:Syno common
 *	Date:2015/11/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _SYNO_TFKEY_P11_H_
#define	_SYNO_TFKEY_P11_H_

#include "SynoHeaders.h"
#include "defines.h"

/*----------------------------------------------*
 * PKCS11�ӿ���Ҫ֧�ֵķ��������㷨��ʶ            *
 *----------------------------------------------*/ 
#define SGD_DES						  0x00000600 /* DES 64λ(8bytes) �����㷨��������Կʱʹ�� */
#define SGD_DES_ECB					  0x00000601 /* DES 64λ(8bytes) ECB�����㷨 */
#define SGD_DES_CBC	                  0x00000602 /* DES 64λ(8bytes) CBC�����㷨 */
#define SGD_DES3					  0x00000610 /* DES 64λ(8bytes) �����㷨��������Կʱʹ�� */ 
#define SGD_DES3_ECB	              0x00000611 /* DES 192λ(24bytes) ECB�����㷨 */
#define SGD_DES3_CBC	              0x00000612 /* DES 192λ(24bytes) CBC�����㷨 */

/*----------------------------------------------*
 * PKCS11�ӿ���Ҫ֧�ֵ��Ӵ��㷨��ʶ               *
 *----------------------------------------------*/  
#define SGD_MD5						  0x00000004 /* MD5�����Ӵ��㷨 */


           
/*----------------------------------------------*
 * PKCS11�ӿ���Ҫ֧�ֵ���չ����               *
 *----------------------------------------------*/  
#define CMD_GET_CONTAINERINFOEX		0xEC



/*----------------------------------------------*
 * PKCS11�ӿ���Ҫ֧�ֵ�RSA��Կ����;����         *
 *----------------------------------------------*/  
#define KT_SIGNATURE				  0x10
#define KT_ENCRYPT					  0x20
#define KT_EXCHANGE					  0x40

#pragma pack(push)
#pragma pack(1)		      
/*----------------------------------------------*
 * PKCS11�ӿ���Ҫ֧�ֵ��������Խṹ��              *
 *----------------------------------------------*/  
typedef struct tagContainerInfoEx
{
	BYTE	type;				//�������ͣ�RSA-1;ECC-2��
	ULONG	signKeyBits;		//ǩ��KeyPair��Bitsֵ�������������Ϊ0.
	ULONG	exchKeyBits;		//����KeyPair��Bitsֵ�������������Ϊ0.
	BYTE	signKeyPair;		//�Ƿ���ǩ��KeyPair
	BYTE	exchKeyPair;		//�Ƿ��м���KeyPair
	BYTE	hasSignCert;		//�Ƿ���ǩ��֤��
	BYTE	hasExchCert;		//�Ƿ��м���֤��
	BYTE	hasSignPriKey;		//�Ƿ�ǩ��˽Կ�����signKeyPairΪTRUE����һ��ΪTRUE�������ǵ�P11����ֻ����˽Կ��û�й�Կ�������
	BYTE	hasVerfiyPubKey;	//�Ƿ�ǩ����Կ�����signKeyPairΪTRUE����һ��ΪTRUE�������ǵ�P11����ֻ���빫Կ��û��˽Կ�������
	BYTE	hasDecryptPriKey;	//�Ƿ��н���˽Կ�����exchKeyPairΪTRUE����һ��ΪTRUE�������ǵ�P11����ֻ����˽Կ��û�й�Կ�������
	BYTE	hasEncryptPubKey;	//�Ƿ��м��ܹ�Կ�����exchKeyPairΪTRUE����һ��ΪTRUE�������ǵ�P11����ֻ���빫Կ��û��˽Կ�������
}ContainerInfoEx, *LPCONTAINERINFOEX;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------*
 * �豸����                                     *
 *----------------------------------------------*/

ULONG P11_WaitForDevEvent(LPSTR szDevName, ULONG *pulDevNameLen, ULONG *pulEvent);

ULONG P11_CancelWaitForDevEvent();

ULONG P11_EnumDev(BOOL bPresent, LPSTR szNameList, ULONG *pulSize);

ULONG P11_ConnectDev(LPSTR szName, DEVHANDLE *phDev);

ULONG P11_DisConnectDev(DEVHANDLE hDev);

ULONG P11_GetDevState(LPSTR szDevName, ULONG *pulDevState);

ULONG P11_SetLabel(DEVHANDLE hDev, CHAR *pcLabel);

ULONG P11_GetDevInfo(DEVHANDLE hDev, DEVINFO *pDevInfo);

ULONG P11_LockDev(DEVHANDLE hDev, ULONG ulTimeOut);

ULONG P11_UnlockDev (DEVHANDLE hDev);

ULONG P11_Transmit(DEVHANDLE hDev, BYTE* pbCommand, ULONG ulCommandLen,BYTE* pbData, ULONG* pulDataLen);

/*----------------------------------------------*
 * ���ʿ���                                     *
 *----------------------------------------------*/

ULONG P11_ChangeDevAuthKey(DEVHANDLE hDev,
    BYTE *pbKeyValue, ULONG ulKeyLen);

ULONG P11_DevAuth(DEVHANDLE hDev, BYTE *pbAuthData, ULONG ulLen);

ULONG P11_ChangePIN(HAPPLICATION hApplication,
    ULONG ulPinType, CHAR *pcOldPin, CHAR *pcNewPin, ULONG *pulRetryCount);

ULONG P11_GetPINInfo(HAPPLICATION hApplication,
    ULONG ulPINType, ULONG *pulMaxRetryCount,
    ULONG *pulRemainRetryCount, BOOL *pbDefaultPin);

ULONG P11_VerifyPIN(HAPPLICATION hApplication,
    ULONG ulPinType, CHAR *pcPin, ULONG *pulRetryCount);

ULONG P11_UnblockPIN(HAPPLICATION hApplication,
    CHAR *pcAdminPIN, CHAR *pcNewUserPIN, ULONG *pulRetryCount);

ULONG P11_ClearSecureState(HAPPLICATION hApplication);

/*----------------------------------------------*
 * Ӧ�ù���                                     *
 *----------------------------------------------*/

ULONG P11_CreateApplication(DEVHANDLE hDev, CHAR *pcAppName,
    CHAR *pcAdminPin, ULONG ulAdminPinRetryCount,
    CHAR *pcUserPin, ULONG ulUserPinRetryCount,
    ULONG ulCreateFileRights, HAPPLICATION *phApplication);

ULONG P11_OpenApplication(DEVHANDLE hDev,
    CHAR *pcAppName, HAPPLICATION *phApplication);

ULONG P11_DeleteApplication(DEVHANDLE hDev, LPSTR szAppName);

ULONG P11_EnumApplication(DEVHANDLE hDev, CHAR *pcAppName, ULONG *pulSize);

ULONG P11_CloseApplication(HAPPLICATION hApplication);

/*----------------------------------------------*
 * �ļ�����                                     *
 *----------------------------------------------*/

ULONG P11_CreateFile(HAPPLICATION hApplication, LPSTR szFileName,
    ULONG ulFileSize, ULONG ulReadRights, ULONG ulWriteRights);

ULONG P11_DeleteFile(HAPPLICATION hApplication, CHAR *pcFileName);

ULONG P11_EnumFiles(HAPPLICATION hApplication,
    CHAR *pcFileList, ULONG *pulSize);

ULONG P11_GetFileInfo(HAPPLICATION hApplication,
    CHAR *pcFileName, FILEATTRIBUTE *pstFileInfo);

ULONG P11_ReadFile(HAPPLICATION hApplication,
    LPSTR szFileName, ULONG ulOffset, ULONG ulSize,
    BYTE *pbOutData, ULONG *pulOutLen);

ULONG P11_WriteFile (HAPPLICATION hApplication,
	LPSTR szFileName, ULONG  ulOffset, BYTE *pbData, ULONG ulSize);

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

ULONG P11_CreateContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, HCONTAINER *phContainer);

ULONG P11_DeleteContainer(HAPPLICATION hApplication, LPSTR szContainerName);

ULONG P11_OpenContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, HCONTAINER *phContainer);

ULONG P11_CloseContainer(HCONTAINER hContainer);

ULONG P11_EnumContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, ULONG *pulSize);

ULONG P11_GetContainerType(HCONTAINER hContainer, ULONG *pulContainerType);

ULONG P11_GetContainerInfo(HCONTAINER hContainer, ULONG *pulContainerType);

ULONG P11_GetContainerInfoEx(HAPPLICATION hApplication, 
	LPSTR szContainerName, LPCONTAINERINFOEX lpContainerInfoEx);

ULONG P11_ImportCertificate(HCONTAINER hContainer,
    BOOL bSignFlag, BYTE *pbCert, ULONG ulCertLen);

ULONG P11_ExportCertificate(HCONTAINER hContainer,
    BOOL bSignFlag, BYTE *pbCert, ULONG *pulCertLen);

/*----------------------------------------------*
 * �������                                     *
 *----------------------------------------------*/

ULONG P11_GenRandom(DEVHANDLE hDev, BYTE *pbRandom, ULONG ulRandomLen);

ULONG P11_GenExtRSAKey(DEVHANDLE hDev, ULONG ulBitsLen, PRSAPRIVATEKEYBLOB pBlob);

ULONG P11_GenRSAKeyPair(HCONTAINER hContainer, UCHAR ucUsage, ULONG ulBitsLen, PRSAPUBLICKEYBLOB pBlob);

ULONG P11_ImportPlainRSAKeyPair(HCONTAINER hContainer, UCHAR ucUsage, PRSAPRIVATEKEYBLOB pBlob);

ULONG P11_ImportPlainRSAPubKey(HCONTAINER hContainer, UCHAR ucUsage, PRSAPUBLICKEYBLOB pBlob);

ULONG P11_ImportPlainRSAPriKey(HCONTAINER hContainer, UCHAR ucUsage, PRSAPRIVATEKEYBLOB pBlob);

ULONG P11_ImportRSAKeyPair(HCONTAINER hContainer, ULONG ulSymAlgId,
    BYTE *pbWrappedKey, ULONG ulWrappedKeyLen,
    BYTE *pbEncryptedData, ULONG ulEncryptedDataLen);

ULONG P11_RSASignData(HCONTAINER hContainer,
    BYTE *pbData, ULONG ulDataLen,
    BYTE *pbSignature, ULONG *pulSignLen);

ULONG P11_RSAVerify(DEVHANDLE hDev,
    RSAPUBLICKEYBLOB *pRSAPubKeyBlob,
    BYTE *pbData, ULONG  ulDataLen,
    BYTE *pbSignature, ULONG ulSignLen);

ULONG P11_RSAExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId,
    RSAPUBLICKEYBLOB *pPubKey, BYTE *pbData, ULONG *pulDataLen, HANDLE *phSessionKey);

ULONG P11_RSAExportSessionKeyEx(HANDLE hSessionKey,
    RSAPUBLICKEYBLOB *pPubKey, BYTE *pbData, ULONG *pulDataLen);

ULONG P11_ExtRSAPubKeyOperation(DEVHANDLE hDev,
    PRSAPUBLICKEYBLOB pRSAPubKeyBlob, BYTE *pbInput, ULONG ulInputLen,
    BYTE *pbOutput, ULONG *pulOutputLen);

ULONG P11_ExtRSAPriKeyOperation(DEVHANDLE hDev,
    PRSAPRIVATEKEYBLOB pRSAPriKeyBlob, BYTE *pbInput, ULONG ulInputLen,
    BYTE *pbOutput, ULONG *pulOutputLen);

ULONG P11_ExtRSAEnDecrypt(HCONTAINER hContainer, BOOL bUseSignKeyPair, BOOL bUsePubKey, BOOL bEncrypt, 
	BYTE *pbInput, ULONG ulInputLen, BYTE *pbOutput, ULONG *pulOutputLen);

ULONG P11_GenECCKeyPair(HCONTAINER hContainer, ULONG ulAlgId, PECCPUBLICKEYBLOB pBlob);

ULONG P11_ImportECCKeyPair(HCONTAINER hContainer, PENVELOPEDKEYBLOB pEnvelopedKeyBlob);

ULONG P11_ECCSignData(HCONTAINER hContainer,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG P11_ECCVerify(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG P11_ECCExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId,
    PECCPUBLICKEYBLOB pPubKey, PECCCIPHERBLOB pData, HANDLE *phSessionKey);

ULONG P11_ECCExportSessionKeyEx(HANDLE hSessionKey,
    PECCPUBLICKEYBLOB pPubKey, PECCCIPHERBLOB pData);

ULONG P11_ExtECCEncrypt(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbPlainText, ULONG ulPlainTextLen, PECCCIPHERBLOB pCipherText);

ULONG P11_ExtECCDecrypt(DEVHANDLE hDev, PECCPRIVATEKEYBLOB pECCPriKeyBlob,
    PECCCIPHERBLOB pCipherText, BYTE *pbPlainText, ULONG *pulPlainTextLen);

ULONG P11_ExtECCSign(DEVHANDLE hDev, PECCPRIVATEKEYBLOB pECCPriKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG P11_ExtECCVerify(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG P11_GenerateAgreementDataWithECC(HCONTAINER hContainer,
    ULONG ulAlgId, PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, HANDLE *phAgreementHandle);

ULONG P11_GenerateAgreementDataAndKeyWithECC(HANDLE hContainer, ULONG ulAlgId,
    PECCPUBLICKEYBLOB pSponsorECCPubKeyBlob,
    PECCPUBLICKEYBLOB pSponsorTempECCPubKeyBlob,
    PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, BYTE *pbSponsorID, ULONG ulSponsorIDLen,
    HANDLE *phKeyHandle);

ULONG P11_GenerateKeyWithECC(HANDLE hAgreementHandle,
    PECCPUBLICKEYBLOB pECCPubKeyBlob,
    PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, HANDLE *phKeyHandle);

ULONG P11_ExportPublicKey(HCONTAINER hContainer, BOOL bSignFlag, BYTE *pbBlob, ULONG *pulBlobLen);

ULONG P11_ImportPublicKey(HCONTAINER hContainer, BOOL bSignFlag, BYTE *pbBlob, ULONG ulBlobLen, HANDLE *phKey);

ULONG P11_ImportSessionKey(HCONTAINER hContainer,
    ULONG ulAlgId, BYTE *pbWrapedData, ULONG ulWrapedLen, HANDLE *phKey);

ULONG P11_SetSymmKey(DEVHANDLE hDev,
    BYTE *pbKey, ULONG ulAlgID, HANDLE *phKey);

ULONG P11_GenSymmKey(DEVHANDLE hDev, ULONG ulAlgID, HANDLE *phKey);

ULONG P11_EncryptInit(HANDLE hKey, ULONG ulAlgID, BLOCKCIPHERPARAM EncryptParam);

ULONG P11_Encrypt(HANDLE hKey,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG P11_EncryptUpdate(HANDLE hKey,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG P11_EncryptFinal(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG *pulEncryptedDataLen);

ULONG P11_DecryptInit(HANDLE hKey, ULONG ulAlgID, BLOCKCIPHERPARAM DecryptParam);

ULONG P11_Decrypt(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG ulEncryptedLen, BYTE *pbData, ULONG *pulDataLen);

ULONG P11_DecryptUpdate(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG ulEncryptedLen, BYTE *pbData, ULONG *pulDataLen);

ULONG P11_DecryptFinal(HANDLE hKey,
    BYTE *pbDecryptedData, ULONG *pulDecryptedDataLen);

ULONG P11_DigestInit(DEVHANDLE hDev, ULONG ulAlgID,
    ECCPUBLICKEYBLOB *pPubKey, BYTE *pbID, ULONG ulIDLen, HANDLE *phHash);

ULONG P11_Digest(HANDLE hHash,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbHashData, ULONG *pulHashLen);

ULONG P11_DigestUpdate(HANDLE hHash, BYTE *pbData, ULONG ulDataLen);

ULONG P11_DigestFinal(HANDLE hHash, BYTE *pbHashData, ULONG *pulHashLen);

ULONG P11_MacInit(HANDLE hKey, BLOCKCIPHERPARAM *pMacParam, HANDLE *phMac);

ULONG P11_Mac(HANDLE hMac,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbMacData, ULONG *pulMacLen);

ULONG P11_MacUpdate(HANDLE hMac, BYTE *pbData, ULONG ulDataLen);

ULONG P11_MacFinal(HANDLE hMac, BYTE *pbMacData, ULONG *pulMacDataLen);

ULONG P11_CloseHandle(HANDLE hHandle);

/*----------------------------------------------*
 * ��չ�ӿ�                                     *
 *----------------------------------------------*/
BOOL P11_ExterAuth(HANDLE hHandle, UCHAR *pucKey, ULONG ulLen);

ULONG P11_EncryptTunnelUpdate(DEVHANDLE hDev,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG P11_DecryptTunnelUpdate(DEVHANDLE hDev,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbDecryptedData, ULONG *pulDecryptedLen);

ULONG P11_DevTransmit(DEVHANDLE hDev, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG P11_AppTransmit(HAPPLICATION hApplication, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG P11_ConTransmit(HCONTAINER hContainer, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG P11_SetDevPath(BYTE *pbDevPath);

ULONG P11_ClearDevPath();

#ifdef __cplusplus
}
#endif

#endif	//_SYNO_TFKEY_P11_H_

