/*
	文件名：GMSKFFuncDef.h
	说明：定义国密SDK接口函数，用于动态加载国密SDK DLL时使用
*/
#ifndef _GMSKF_FUNCDEF_H_
#define _GMSKF_FUNCDEF_H_

#include "SKFAPI.h"

/* 设备管理函数 */
typedef ULONG (__stdcall *SKF_WaitForDevEventProc)(LPSTR, ULONG*, ULONG*);
typedef ULONG (__stdcall *SKF_CancelWaitForDevEventProc)();
typedef ULONG (__stdcall *SKF_EnumDevProc)(BOOL, LPSTR, ULONG*);
typedef ULONG (__stdcall *SKF_ConnectDevProc)(LPSTR, DEVHANDLE*);
typedef ULONG (__stdcall *SKF_DisConnectDevProc)(DEVHANDLE);
typedef ULONG (__stdcall *SKF_GetDevStateProc)(LPSTR, ULONG*);
typedef ULONG (__stdcall *SKF_SetLabelProc)(DEVHANDLE, LPSTR);
typedef ULONG (__stdcall *SKF_GetDevInfoProc)(DEVHANDLE, DEVINFO*);
typedef ULONG (__stdcall *SKF_LockDevProc)(DEVHANDLE, ULONG);
typedef ULONG (__stdcall *SKF_UnlockDevProc)(DEVHANDLE);

/* 权限控制函数 */ 
typedef ULONG (__stdcall *SKF_ChangeDevAuthKeyProc)(DEVHANDLE, BYTE*, ULONG);
typedef ULONG (__stdcall *SKF_DevAuthProc)(DEVHANDLE, BYTE*, ULONG);
typedef ULONG (__stdcall *SKF_ChangePINProc)(HAPPLICATION, ULONG, LPSTR, LPSTR, ULONG*);
typedef ULONG (__stdcall *SKF_GetPINInfoProc)(HAPPLICATION, ULONG, ULONG*, ULONG*, BOOL*);
typedef ULONG (__stdcall *SKF_VerifyPINProc)(HAPPLICATION, ULONG, LPSTR, ULONG*);
typedef ULONG (__stdcall *SKF_UnblockPINProc)(HAPPLICATION, LPSTR, LPSTR,  ULONG*);
typedef ULONG (__stdcall *SKF_ClearSecureStateProc)(HAPPLICATION);

/* 应用管理函数 */
typedef ULONG (__stdcall *SKF_CreateApplicationProc)(DEVHANDLE, LPSTR, LPSTR, DWORD, LPSTR, DWORD, DWORD, HAPPLICATION*);
typedef ULONG (__stdcall *SKF_EnumApplicationProc)(DEVHANDLE, LPSTR, ULONG*);
typedef ULONG (__stdcall *SKF_DeleteApplicationProc)(DEVHANDLE, LPSTR);
typedef ULONG (__stdcall *SKF_OpenApplicationProc)(DEVHANDLE, LPSTR, HAPPLICATION*);
typedef ULONG (__stdcall *SKF_CloseApplicationProc)(HAPPLICATION);

/* 文件管理函数 */
typedef ULONG (__stdcall *SKF_CreateFileProc)(HAPPLICATION, LPSTR, ULONG, ULONG, ULONG);
typedef ULONG (__stdcall *SKF_DeleteFileProc)(HAPPLICATION, LPSTR);
typedef ULONG (__stdcall *SKF_EnumFilesProc)(HAPPLICATION, LPSTR, ULONG*);
typedef ULONG (__stdcall *SKF_GetFileInfoProc)(HAPPLICATION, LPSTR, FILEATTRIBUTE*);
typedef ULONG (__stdcall *SKF_ReadFileProc)(HAPPLICATION,  LPSTR, ULONG, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_WriteFileProc)(HAPPLICATION, LPSTR, ULONG , BYTE*, ULONG);

/* 容器管理函数 */
typedef ULONG (__stdcall *SKF_CreateContainerProc)(HAPPLICATION, LPSTR, HCONTAINER*);
typedef ULONG (__stdcall *SKF_DeleteContainerProc)(HAPPLICATION, LPSTR);
typedef ULONG (__stdcall *SKF_OpenContainerProc)(HAPPLICATION, LPSTR,HCONTAINER*);
typedef ULONG (__stdcall *SKF_CloseContainerProc)(HCONTAINER);
typedef ULONG (__stdcall *SKF_EnumContainerProc)(HAPPLICATION,LPSTR,ULONG*);
typedef ULONG (__stdcall *SKF_GetConPropertyProc)(HCONTAINER, ULONG*);
typedef ULONG (__stdcall *SKF_GetContainerTypeProc)(HCONTAINER, ULONG*);

/* 证书管理函数 */ 
typedef ULONG (__stdcall *SKF_ImportCertificateProc)(HCONTAINER, BOOL, BYTE*, ULONG);
typedef ULONG (__stdcall *SKF_ExportCertificateProc)(HCONTAINER, BOOL, BYTE*, ULONG*);

/* RSA密钥管理函数 */
typedef ULONG (__stdcall *SKF_GenExtRSAKeyProc)(DEVHANDLE, ULONG, RSAPRIVATEKEYBLOB*);
typedef ULONG (__stdcall *SKF_GenRSAKeyPairProc)(HCONTAINER, ULONG, RSAPUBLICKEYBLOB*);
typedef ULONG (__stdcall *SKF_ImportRSAKeyPairProc)(HCONTAINER, ULONG,BYTE*, ULONG, BYTE*, ULONG);
typedef ULONG (__stdcall *SKF_RSASignDataProc)(HCONTAINER, BYTE*, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_RSAVerifyProc)(DEVHANDLE, RSAPUBLICKEYBLOB*, BYTE*, ULONG, BYTE*, ULONG);
typedef ULONG (__stdcall *SKF_RSAExportSessionKeyProc)(HCONTAINER, ULONG, RSAPUBLICKEYBLOB*, BYTE*, ULONG*, HANDLE*);
typedef ULONG (__stdcall *SKF_ExtRSAPubKeyOperationProc)(DEVHANDLE, RSAPUBLICKEYBLOB*, BYTE*, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_ExtRSAPriKeyOperationProc)(DEVHANDLE, RSAPRIVATEKEYBLOB*,BYTE*, ULONG, BYTE*, ULONG*);

/* ECC密钥管理函数 */
typedef ULONG (__stdcall *SKF_GenECCKeyPairProc)(HCONTAINER, ULONG,  ECCPUBLICKEYBLOB*);
typedef ULONG (__stdcall *SKF_ImportECCKeyPairProc)(HCONTAINER, PENVELOPEDKEYBLOB);
typedef ULONG (__stdcall *SKF_ECCSignDataProc)(HCONTAINER, BYTE*, ULONG, PECCSIGNATUREBLOB);
typedef ULONG (__stdcall *SKF_ECCVerifyProc)(DEVHANDLE, ECCPUBLICKEYBLOB*, BYTE*, ULONG, PECCSIGNATUREBLOB);
typedef ULONG (__stdcall *SKF_ECCExportSessionKeyProc)(HCONTAINER, ULONG, ECCPUBLICKEYBLOB*, PECCCIPHERBLOB, HANDLE*);
typedef ULONG (__stdcall *SKF_ExtECCEncryptProc)(DEVHANDLE, ECCPUBLICKEYBLOB*, BYTE*, ULONG, PECCCIPHERBLOB);
typedef ULONG (__stdcall *SKF_ExtECCDecryptProc)(DEVHANDLE, ECCPRIVATEKEYBLOB*, PECCCIPHERBLOB, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_ExtECCSignProc)(DEVHANDLE, ECCPRIVATEKEYBLOB*, BYTE*, ULONG, PECCSIGNATUREBLOB);
typedef ULONG (__stdcall *SKF_ExtECCVerifyProc)(DEVHANDLE, ECCPUBLICKEYBLOB*, BYTE*, ULONG, PECCSIGNATUREBLOB);
typedef ULONG (__stdcall *SKF_GenerateAgreementDataWithECCProc)(HCONTAINER, ULONG, ECCPUBLICKEYBLOB*, BYTE*, ULONG, HANDLE*);
typedef ULONG (__stdcall *SKF_GenerateKeyWithECCProc)(HANDLE, ECCPUBLICKEYBLOB*, ECCPUBLICKEYBLOB*, BYTE*, ULONG, HANDLE*);
typedef ULONG (__stdcall *SKF_GenerateAgreementDataAndKeyWithECCProc)(HANDLE, ULONG, ECCPUBLICKEYBLOB*, ECCPUBLICKEYBLOB*, ECCPUBLICKEYBLOB*, BYTE*, ULONG, BYTE*, ULONG, HANDLE*);

/* 对称密钥管理函数 */
typedef ULONG (__stdcall *SKF_GenRandomProc)(DEVHANDLE, BYTE*, ULONG);
typedef ULONG (__stdcall *SKF_ExportPublicKeyProc)(HCONTAINER, BOOL, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_ImportSessionKeyProc)(HCONTAINER, ULONG, BYTE*, ULONG, HANDLE*);
typedef ULONG (__stdcall *SKF_SetSymmKeyProc)(DEVHANDLE, BYTE*, ULONG, HANDLE*);

/* 加解密函数 */
typedef ULONG (__stdcall *SKF_EncryptInitProc)(HANDLE, BLOCKCIPHERPARAM);
typedef ULONG (__stdcall *SKF_EncryptProc)(HANDLE, BYTE*, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_EncryptUpdateProc)(HANDLE, BYTE*, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_EncryptFinalProc)(HANDLE, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_DecryptInitProc)(HANDLE, BLOCKCIPHERPARAM);
typedef ULONG (__stdcall *SKF_DecryptProc)(HANDLE, BYTE*, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_DecryptUpdateProc)(HANDLE, BYTE*, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_DecryptFinalProc)(HANDLE, BYTE*, ULONG*);

/* 杂凑函数 */
typedef ULONG (__stdcall *SKF_DigestInitProc)(DEVHANDLE, ULONG, ECCPUBLICKEYBLOB*, BYTE*, ULONG, HANDLE*);
typedef ULONG (__stdcall *SKF_DigestProc)(HANDLE, BYTE*, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_DigestUpdateProc)(HANDLE, BYTE*, ULONG);
typedef ULONG (__stdcall *SKF_DigestFinalProc)(HANDLE, BYTE*, ULONG *);

/* MAC函数 */
typedef ULONG (__stdcall *SKF_MacInitProc)(HANDLE, BLOCKCIPHERPARAM*, HANDLE*);
typedef ULONG (__stdcall *SKF_MacProc)(HANDLE, BYTE*, ULONG, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_MacUpdateProc)(HANDLE, BYTE*, ULONG);
typedef ULONG (__stdcall *SKF_MacFinalProc)(HANDLE, BYTE*, ULONG*);
typedef ULONG (__stdcall *SKF_CloseHandleProc)(HANDLE);
typedef ULONG (__stdcall *SKF_TransmitProc)(DEVHANDLE, BYTE*, ULONG, BYTE*, ULONG*);


#endif // _GMSKF_FUNCDEF_H_
