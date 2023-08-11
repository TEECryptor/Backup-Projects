/******************************************************
*	File Name: USBKeySKFImp.h
*	Author: Youfang Yao
*	Date: 13/08/2015
*	Introduce: The class CUSBKeySKFImpl definition file
******************************************************/

#ifndef _USBKEY_SKF_IMPL_H_
#define _USBKEY_SKF_IMPL_H_

#include "../Include/IUSBKeyApi.h"
#include "../Include/CommonDef.h"
#include "./ThirdParty/GuoMi/GMSKFFuncDef.h"

class CUSBKeySKFImpl :	public IUSBKeyApi
{
public:
	CUSBKeySKFImpl(void);
	virtual ~CUSBKeySKFImpl(void);

public:	
	virtual ULONG Create(LPCSTR lpcsCspOrFile);
	virtual ULONG Initialize(ULONG ulAsymmAlg, ULONG ulSymmAlg, ULONG ulHashAlg, LPSTR lpUserPIN);
	virtual ULONG WaitForDevEvent(BOOL bNotBlock, ULONG *pulSoltID, LPSTR lpDevName, ULONG *pulNameLen, ULONG *pulEventType);
	virtual ULONG CancelWaitForDevEvent();
	virtual ULONG ConnectDev();
	virtual ULONG Envelop(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	virtual ULONG Develop(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	virtual ULONG Encrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	virtual ULONG Decrypt(LPBYTE lpInData, ULONG ulInLen, LPBYTE lpOutData, ULONG* pulOutLen);
	virtual ULONG SignP7(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG* pulSignLen);
	virtual ULONG VerifyP7(LPBYTE lpOrgData, ULONG ulOrgLen, LPBYTE lpSignature, ULONG ulSignLen);
	virtual ULONG SignP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPSTR lpUserID, LPBYTE lpSignature, ULONG* pulSignLen);
	virtual ULONG VerifyP1(LPBYTE lpOrgData, ULONG ulOrgLen, LPSTR lpUserID, LPBYTE lpSignature, ULONG ulSignLen);
	virtual ULONG DisconnectDev(void);
	virtual ULONG Release(void);

private:
	LPSTR		_GetApplitionName();
	HCONTAINER	_OpenContainer(ULONG ulAlgID, BOOL bSign);

private:
	/* 设备管理函数 */
	SKF_WaitForDevEventProc			SKF_WaitForDevEvent;
	SKF_CancelWaitForDevEventProc	SKF_CancelWaitForDevEvent;
	SKF_EnumDevProc					SKF_EnumDev;
	SKF_ConnectDevProc				SKF_ConnectDev;
	SKF_DisConnectDevProc			SKF_DisConnectDev;
	SKF_GetDevStateProc				SKF_GetDevState;
	SKF_SetLabelProc				SKF_SetLabel;
	SKF_GetDevInfoProc				SKF_GetDevInfo;
	SKF_LockDevProc					SKF_LockDev;
	SKF_UnlockDevProc				SKF_UnlockDev;
		
	/* 权限控制函数 */ 
	SKF_ChangeDevAuthKeyProc		SKF_ChangeDevAuthKey;
	SKF_DevAuthProc					SKF_DevAuth;
	SKF_ChangePINProc				SKF_ChangePIN;
	SKF_GetPINInfoProc				SKF_GetPINInfo;
	SKF_VerifyPINProc				SKF_VerifyPIN;
	SKF_UnblockPINProc				SKF_UnblockPIN;
	SKF_ClearSecureStateProc		SKF_ClearSecureState;

	/* 应用管理函数 */
	SKF_CreateApplicationProc		SKF_CreateApplication;
	SKF_EnumApplicationProc			SKF_EnumApplication;
	SKF_DeleteApplicationProc		SKF_DeleteApplication;
	SKF_OpenApplicationProc			SKF_OpenApplication;
	SKF_CloseApplicationProc		SKF_CloseApplication;

	/* 文件管理函数 */
	SKF_CreateFileProc				SKF_CreateFile;
	SKF_DeleteFileProc				SKF_DeleteFile;
	SKF_EnumFilesProc				SKF_EnumFiles;
	SKF_GetFileInfoProc				SKF_GetFileInfo;
	SKF_ReadFileProc				SKF_ReadFile;
	SKF_WriteFileProc				SKF_WriteFile;

	/* 容器管理函数 */
	SKF_CreateContainerProc			SKF_CreateContainer;
	SKF_DeleteContainerProc			SKF_DeleteContainer;
	SKF_OpenContainerProc			SKF_OpenContainer;
	SKF_CloseContainerProc			SKF_CloseContainer;
	SKF_EnumContainerProc			SKF_EnumContainer;
	SKF_GetContainerTypeProc		SKF_GetContainerType;

	/* 证书管理函数 */ 
	SKF_ImportCertificateProc		SKF_ImportCertificate;
	SKF_ExportCertificateProc		SKF_ExportCertificate;
	
	/* RSA密钥管理函数 */
	SKF_GenExtRSAKeyProc			SKF_GenExtRSAKey;
	SKF_GenRSAKeyPairProc			SKF_GenRSAKeyPair;
	SKF_ImportRSAKeyPairProc		SKF_ImportRSAKeyPair;
	SKF_RSASignDataProc				SKF_RSASignData;
	SKF_RSAVerifyProc				SKF_RSAVerify;
	SKF_RSAExportSessionKeyProc		SKF_RSAExportSessionKey;
	SKF_ExtRSAPubKeyOperationProc	SKF_ExtRSAPubKeyOperation;
	SKF_ExtRSAPriKeyOperationProc	SKF_ExtRSAPriKeyOperation;

	/* ECC密钥管理函数 */
	SKF_GenECCKeyPairProc			SKF_GenECCKeyPair;
	SKF_ImportECCKeyPairProc		SKF_ImportECCKeyPair;
	SKF_ECCSignDataProc				SKF_ECCSignData;
	SKF_ECCVerifyProc				SKF_ECCVerify;
	SKF_ECCExportSessionKeyProc		SKF_ECCExportSessionKey;
	SKF_ExtECCEncryptProc			SKF_ExtECCEncrypt;
	SKF_ExtECCDecryptProc			SKF_ExtECCDecrypt;
	SKF_ExtECCSignProc				SKF_ExtECCSign;
	SKF_ExtECCVerifyProc			SKF_ExtECCVerify;
	SKF_GenerateAgreementDataWithECCProc		SKF_GenerateAgreementDataWithECC;
	SKF_GenerateKeyWithECCProc					SKF_GenerateKeyWithECC;
	SKF_GenerateAgreementDataAndKeyWithECCProc	SKF_GenerateAgreementDataAndKeyWithECC;
	
	/* 对称密钥管理函数 */
	SKF_GenRandomProc				SKF_GenRandom;
	SKF_ExportPublicKeyProc			SKF_ExportPublicKey;
	SKF_ImportSessionKeyProc		SKF_ImportSessionKey;
	SKF_SetSymmKeyProc				SKF_SetSymmKey;

	/* 加解密函数 */
	SKF_EncryptInitProc				SKF_EncryptInit;
	SKF_EncryptProc					SKF_Encrypt;
	SKF_EncryptUpdateProc			SKF_EncryptUpdate;
	SKF_EncryptFinalProc			SKF_EncryptFinal;
	SKF_DecryptInitProc				SKF_DecryptInit;
	SKF_DecryptProc					SKF_Decrypt;
	SKF_DecryptUpdateProc			SKF_DecryptUpdate;
	SKF_DecryptFinalProc			SKF_DecryptFinal;

	/* 杂凑函数 */
	SKF_DigestInitProc				SKF_DigestInit;
	SKF_DigestProc					SKF_Digest;
	SKF_DigestUpdateProc			SKF_DigestUpdate;
	SKF_DigestFinalProc				SKF_DigestFinal;

	/* MAC函数 */
	SKF_MacInitProc					SKF_MacInit;
	SKF_MacProc						SKF_Mac;
	SKF_MacUpdateProc				SKF_MacUpdate;
	SKF_MacFinalProc				SKF_MacFinal;
	SKF_CloseHandleProc				SKF_CloseHandle;
	SKF_TransmitProc				SKF_Transmit;
	
private:
	HINSTANCE		m_hInstance;
	CHAR			m_csDLLFile[256];
	CHAR			m_csUserPIN[64];
	ULONG			m_ulAsymmAlg;
	ULONG			m_ulSymmAlg;
	ULONG			m_ulHashAlg;
	BOOL			m_bSDKLoaded;
	HANDLE			m_hDev;	
	HAPPLICATION	m_hApplication;
};
#endif	//_USBKEY_SKF_IMPL_H_

