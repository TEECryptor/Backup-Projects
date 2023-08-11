/***************************************************************************
 *	File name:SynoTFKeySKF.h
 *	Introduce:SKF API functions definition
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _SYNO_TFKEY_SKF_H_
#define	_SYNO_TFKEY_SKF_H_

#include "SynoHeaders.h"
#include "defines.h"

#if defined(WIN32) && defined(SYNOTFKEYSKF_EXPORTS)
#define DEVAPI						   __declspec(dllexport) /* 输出函数 */
#else
#define DEVAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------*
 * 设备管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_WaitForDevEvent(LPSTR szDevName, ULONG *pulDevNameLen, ULONG *pulEvent);

ULONG DEVAPI SKF_CancelWaitForDevEvent();

ULONG DEVAPI SKF_EnumDev(BOOL bPresent, LPSTR szNameList, ULONG *pulSize);

ULONG DEVAPI SKF_ConnectDev(LPSTR szName, DEVHANDLE *phDev);

ULONG DEVAPI SKF_DisConnectDev(DEVHANDLE hDev);

ULONG DEVAPI SKF_GetDevState(LPSTR szDevName, ULONG *pulDevState);

ULONG DEVAPI SKF_SetLabel(DEVHANDLE hDev, CHAR *pcLabel);

ULONG DEVAPI SKF_GetDevInfo(DEVHANDLE hDev, DEVINFO *pDevInfo);

ULONG DEVAPI SKF_LockDev(DEVHANDLE hDev, ULONG ulTimeOut);

ULONG DEVAPI SKF_UnlockDev (DEVHANDLE hDev);

ULONG DEVAPI SKF_Transmit(DEVHANDLE hDev, BYTE* pbCommand, ULONG ulCommandLen,BYTE* pbData, ULONG* pulDataLen);

/*----------------------------------------------*
 * 访问控制                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_ChangeDevAuthKey(DEVHANDLE hDev,
    BYTE *pbKeyValue, ULONG ulKeyLen);

ULONG DEVAPI SKF_DevAuth(DEVHANDLE hDev, BYTE *pbAuthData, ULONG ulLen);

ULONG DEVAPI SKF_ChangePIN(HAPPLICATION hApplication,
    ULONG ulPinType, CHAR *pcOldPin, CHAR *pcNewPin, ULONG *pulRetryCount);

ULONG DEVAPI SKF_GetPINInfo(HAPPLICATION hApplication,
    ULONG ulPINType, ULONG *pulMaxRetryCount,
    ULONG *pulRemainRetryCount, BOOL *pbDefaultPin);

ULONG DEVAPI SKF_VerifyPIN(HAPPLICATION hApplication,
    ULONG ulPinType, CHAR *pcPin, ULONG *pulRetryCount);

ULONG DEVAPI SKF_UnblockPIN(HAPPLICATION hApplication,
    CHAR *pcAdminPIN, CHAR *pcNewUserPIN, ULONG *pulRetryCount);

ULONG DEVAPI SKF_ClearSecureState(HAPPLICATION hApplication);

/*----------------------------------------------*
 * 应用管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_CreateApplication(DEVHANDLE hDev, CHAR *pcAppName,
    CHAR *pcAdminPin, ULONG ulAdminPinRetryCount,
    CHAR *pcUserPin, ULONG ulUserPinRetryCount,
    ULONG ulCreateFileRights, HAPPLICATION *phApplication);

ULONG DEVAPI SKF_OpenApplication(DEVHANDLE hDev,
    CHAR *pcAppName, HAPPLICATION *phApplication);

ULONG DEVAPI SKF_DeleteApplication(DEVHANDLE hDev, LPSTR szAppName);

ULONG DEVAPI SKF_EnumApplication(DEVHANDLE hDev, CHAR *pcAppName, ULONG *pulSize);

ULONG DEVAPI SKF_CloseApplication(HAPPLICATION hApplication);

/*----------------------------------------------*
 * 文件管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_CreateFile(HAPPLICATION hApplication, LPSTR szFileName,
    ULONG ulFileSize, ULONG ulReadRights, ULONG ulWriteRights);

ULONG DEVAPI SKF_DeleteFile(HAPPLICATION hApplication, CHAR *pcFileName);

ULONG DEVAPI SKF_EnumFiles(HAPPLICATION hApplication,
    CHAR *pcFileList, ULONG *pulSize);

ULONG DEVAPI SKF_GetFileInfo(HAPPLICATION hApplication,
    CHAR *pcFileName, FILEATTRIBUTE *pstFileInfo);

ULONG DEVAPI SKF_ReadFile(HAPPLICATION hApplication,
    LPSTR szFileName, ULONG ulOffset, ULONG ulSize,
    BYTE *pbOutData, ULONG *pulOutLen);

ULONG DEVAPI SKF_WriteFile (HAPPLICATION hApplication,
	LPSTR szFileName, ULONG  ulOffset, BYTE *pbData, ULONG ulSize);

/*----------------------------------------------*
 * 容器管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_CreateContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, HCONTAINER *phContainer);

ULONG DEVAPI SKF_DeleteContainer(HAPPLICATION hApplication, LPSTR szContainerName);

ULONG DEVAPI SKF_OpenContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, HCONTAINER *phContainer);

ULONG DEVAPI SKF_CloseContainer(HCONTAINER hContainer);

ULONG DEVAPI SKF_EnumContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, ULONG *pulSize);

ULONG DEVAPI SKF_GetContainerType(HCONTAINER hContainer, ULONG *pulContainerType);

ULONG DEVAPI SKF_ImportCertificate(HCONTAINER hContainer,
    BOOL bSignFlag, BYTE *pbCert, ULONG ulCertLen);

ULONG DEVAPI SKF_ExportCertificate(HCONTAINER hContainer,
    BOOL bSignFlag, BYTE *pbCert, ULONG *pulCertLen);

/*----------------------------------------------*
 * 密码管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_GenRandom(DEVHANDLE hDev, BYTE *pbRandom, ULONG ulRandomLen);

ULONG DEVAPI SKF_GenExtRSAKey(DEVHANDLE hDev, ULONG ulBitsLen, PRSAPRIVATEKEYBLOB pBlob);

ULONG DEVAPI SKF_GenRSAKeyPair(HCONTAINER hContainer, ULONG ulBitsLen, PRSAPUBLICKEYBLOB pBlob);

ULONG DEVAPI SKF_ImportPlainRSAKeyPair(HCONTAINER hContainer, PRSAPRIVATEKEYBLOB pBlob, BOOL bSignKeyPair);

ULONG DEVAPI SKF_ImportRSAKeyPair(HCONTAINER hContainer, ULONG ulSymAlgId,
    BYTE *pbWrappedKey, ULONG ulWrappedKeyLen,
    BYTE *pbEncryptedData, ULONG ulEncryptedDataLen);

ULONG DEVAPI SKF_RSASignData(HCONTAINER hContainer,
    BYTE *pbData, ULONG ulDataLen,
    BYTE *pbSignature, ULONG *pulSignLen);

ULONG DEVAPI SKF_RSAVerify(DEVHANDLE hDev,
    RSAPUBLICKEYBLOB *pRSAPubKeyBlob,
    BYTE *pbData, ULONG  ulDataLen,
    BYTE *pbSignature, ULONG ulSignLen);

ULONG DEVAPI SKF_RSAExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId,
    RSAPUBLICKEYBLOB *pPubKey, BYTE *pbData, ULONG *pulDataLen, HANDLE *phSessionKey);

ULONG DEVAPI SKF_RSAExportSessionKeyEx(HANDLE hSessionKey,
    RSAPUBLICKEYBLOB *pPubKey, BYTE *pbData, ULONG *pulDataLen);

ULONG DEVAPI SKF_ExtRSAPubKeyOperation(DEVHANDLE hDev,
    PRSAPUBLICKEYBLOB pRSAPubKeyBlob, BYTE *pbInput, ULONG ulInputLen,
    BYTE *pbOutput, ULONG *pulOutputLen);

ULONG DEVAPI SKF_ExtRSAPriKeyOperation(DEVHANDLE hDev,
    PRSAPRIVATEKEYBLOB pRSAPriKeyBlob, BYTE *pbInput, ULONG ulInputLen,
    BYTE *pbOutput, ULONG *pulOutputLen);

ULONG DEVAPI SKF_GenECCKeyPair(HCONTAINER hContainer, ULONG ulAlgId, PECCPUBLICKEYBLOB pBlob);

ULONG DEVAPI SKF_ImportECCKeyPair(HCONTAINER hContainer, PENVELOPEDKEYBLOB pEnvelopedKeyBlob);

ULONG DEVAPI SKF_ECCSignData(HCONTAINER hContainer,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG DEVAPI SKF_ECCVerify(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG DEVAPI SKF_ECCExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId,
    PECCPUBLICKEYBLOB pPubKey, PECCCIPHERBLOB pData, HANDLE *phSessionKey);

ULONG DEVAPI SKF_ECCExportSessionKeyEx(HANDLE hSessionKey,
    PECCPUBLICKEYBLOB pPubKey, PECCCIPHERBLOB pData);

ULONG DEVAPI SKF_ExtECCEncrypt(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbPlainText, ULONG ulPlainTextLen, PECCCIPHERBLOB pCipherText);

ULONG DEVAPI SKF_ExtECCDecrypt(DEVHANDLE hDev, PECCPRIVATEKEYBLOB pECCPriKeyBlob,
    PECCCIPHERBLOB pCipherText, BYTE *pbPlainText, ULONG *pulPlainTextLen);

ULONG DEVAPI SKF_ExtECCSign(DEVHANDLE hDev, PECCPRIVATEKEYBLOB pECCPriKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG DEVAPI SKF_ExtECCVerify(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature);

ULONG DEVAPI SKF_GenerateAgreementDataWithECC(HCONTAINER hContainer,
    ULONG ulAlgId, PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, HANDLE *phAgreementHandle);

ULONG DEVAPI SKF_GenerateAgreementDataAndKeyWithECC(HANDLE hContainer, ULONG ulAlgId,
    PECCPUBLICKEYBLOB pSponsorECCPubKeyBlob,
    PECCPUBLICKEYBLOB pSponsorTempECCPubKeyBlob,
    PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, BYTE *pbSponsorID, ULONG ulSponsorIDLen,
    HANDLE *phKeyHandle);

ULONG DEVAPI SKF_GenerateKeyWithECC(HANDLE hAgreementHandle,
    PECCPUBLICKEYBLOB pECCPubKeyBlob,
    PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, HANDLE *phKeyHandle);

ULONG DEVAPI SKF_ExportPublicKey(HCONTAINER hContainer, BOOL bSignFlag, BYTE *pbBlob, ULONG *pulBlobLen);

ULONG DEVAPI SKF_ImportPublicKey(HCONTAINER hContainer, BOOL bSignFlag, BYTE *pbBlob, ULONG ulBlobLen, HANDLE *phKey);

ULONG DEVAPI SKF_ImportSessionKey(HCONTAINER hContainer,
    ULONG ulAlgId, BYTE *pbWrapedData, ULONG ulWrapedLen, HANDLE *phKey);

ULONG DEVAPI SKF_SetSymmKey(DEVHANDLE hDev,
    BYTE *pbKey, ULONG ulAlgID, HANDLE *phKey);

ULONG DEVAPI SKF_EncryptInit(HANDLE hKey, BLOCKCIPHERPARAM EncryptParam);

ULONG DEVAPI SKF_Encrypt(HANDLE hKey,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG DEVAPI SKF_EncryptUpdate(HANDLE hKey,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG DEVAPI SKF_EncryptFinal(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG *pulEncryptedDataLen);

ULONG DEVAPI SKF_DecryptInit(HANDLE hKey, BLOCKCIPHERPARAM DecryptParam);

ULONG DEVAPI SKF_Decrypt(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG ulEncryptedLen, BYTE *pbData, ULONG *pulDataLen);

ULONG DEVAPI SKF_DecryptUpdate(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG ulEncryptedLen, BYTE *pbData, ULONG *pulDataLen);

ULONG DEVAPI SKF_DecryptFinal(HANDLE hKey,
    BYTE *pbDecryptedData, ULONG *pulDecryptedDataLen);

ULONG DEVAPI SKF_DigestInit(DEVHANDLE hDev, ULONG ulAlgID,
    ECCPUBLICKEYBLOB *pPubKey, BYTE *pbID, ULONG ulIDLen, HANDLE *phHash);

ULONG DEVAPI SKF_Digest(HANDLE hHash,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbHashData, ULONG *pulHashLen);

ULONG DEVAPI SKF_DigestUpdate(HANDLE hHash, BYTE *pbData, ULONG ulDataLen);

ULONG DEVAPI SKF_DigestFinal(HANDLE hHash, BYTE *pbHashData, ULONG *pulHashLen);

ULONG DEVAPI SKF_MacInit(HANDLE hKey, BLOCKCIPHERPARAM *pMacParam, HANDLE *phMac);

ULONG DEVAPI SKF_Mac(HANDLE hMac,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbMacData, ULONG *pulMacLen);

ULONG DEVAPI SKF_MacUpdate(HANDLE hMac, BYTE *pbData, ULONG ulDataLen);

ULONG DEVAPI SKF_MacFinal(HANDLE hMac, BYTE *pbMacData, ULONG *pulMacDataLen);

ULONG DEVAPI SKF_CloseHandle(HANDLE hHandle);

/*----------------------------------------------*
 * 扩展接口                                     *
 *----------------------------------------------*/
BOOL DEVAPI SKF_ExterAuth(HANDLE hHandle, UCHAR *pucKey, ULONG ulLen);

ULONG DEVAPI SKF_EncryptTunnelUpdate(DEVHANDLE hDev,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen);

ULONG DEVAPI SKF_DecryptTunnelUpdate(DEVHANDLE hDev,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbDecryptedData, ULONG *pulDecryptedLen);

ULONG DEVAPI SKF_DevTransmit(DEVHANDLE hDev, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG DEVAPI SKF_AppTransmit(HAPPLICATION hApplication, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG DEVAPI SKF_ConTransmit(HCONTAINER hContainer, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen);

ULONG DEVAPI SKF_SetDevPath(BYTE *pbDevPath);

ULONG DEVAPI SKF_ClearDevPath();


#ifdef __cplusplus
}
#endif

#endif	//_SYNO_TFKEY_SKF_H_

