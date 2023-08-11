/***************************************************************************
 *  File name:Command.c
 *  Function:The functions for Command
 *  Author:Syno Common
 *  Date:2015/06/20
 *  Version:v1.0
****************************************************************************/
#include "SYglobal.h"
#include "FileSystem.h"
#include "SM2.h" 
#include "SHA-1.h"
#include "SM3.h"		  
#include "SdMemAcc.h"
#include "log.h"
#include <alloca.h>
					  

/**************************************************************************
* Macro  Declaration	 
***************************************************************************/
//#define CON_FLAGCARDLOCK 0x01
//#define CON_FLAGCARDSA   0x05
//#define CON_FLAGALL      0x0F

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
VERSION     gCosVersion = {1, 1};       //当前COS版本
UINT8       gCurSAStatus = 0;           //当前安全状态，1管理员PIN验证通过，2用户PIN验证通过，3两个都通过. 
UINT8       gCardStatus = CARD_INITED;  //当前安全状态，1管理员PIN验证通过，2用户PIN验证通过，3两个都通过. 
UINT32      gHashAlgID = 0;             //当前正在使用的HASH算法
BOOL        gHashAlgInUse = FALSE;      //HASH算法是否已经初始化
APPSTATE    gAPPState[APP_MAX] = {0};   //所有应用的状态数据
UINT8       gRandData[16];              //存放内部认证等的随机数；
//
//HAGREEMENTKEY AgreementKey;
UINT8           AgreementKeyFlag = 0;
SM3_CONTEXT     gSm3Context;
SHA1_CONTEXT    gSHA1Context;
SHA256_CONTEXT  gSHA256Context; 
//
UINT16          gLastSessionKeyID = 0;              //上次使用的会话密钥ID
SESSIONKEYDATA  gSessionKey[SESSIONKEY_MAX] = {0};  //所有生成/导入的会话密钥

//设备认证缺省密钥
const UINT8 gPwd[32] = {0x23,0x45,0xbb,0x5d,0x67,0x8a,0x9c,0xdd,0}; 

//以下所有变量，必须按四字节对齐 
__align(4)	UINT8   n[256],e[4],p[128], q[128], dp[128],dq[128],qinv[128];

						 
/***************************************************************************
* Subroutine:	SetCmdResult
* Function:		设置命令处理结果.
* Input:		nSW -- 状态码
*               nLen -- 响应数据长度
* Output:		None
* Description:	
* Date:			2015/06/20
* ModifyRecord:
* *************************************************************************/
void SetCmdResult(UINT16 nSW, UINT16 nLen)
{      
    gApduResp.SW = nSW;
    gApduResp.LEN = nLen;
} 
/***************************************************************************
* Subroutine:	IsHasControl
* Function:		判断当前用户权限 
* Input:        nMin -- 当前允许最小权限，如判断用户权限时此值为SA_USERLOGIN 
                nMax -- 最大权限 
                flag -- 标志位	
* Output:		SAR_OK-成功
                SAR_USER_NOT_LOGGED_IN-用户未登录 
* Version:  V1.0 
* Date:		2015.06.20
* ModifyRecord:
* ************************************************************************/
UINT16 IsHasControl(UINT8 nMin, UINT8 nMax, UINT8 flag)
{
	if (flag & 0x08)
    {
	    if (nMin == 0)
        {
	        return SAR_OK;
        }
		if (nMin <= gCurSAStatus && nMax >= gCurSAStatus)
		{
			return SAR_OK;
		}
		return SAR_SASTATUSERR;
	}
	return SAR_OK;
}
/***************************************************************************
* Subroutine:	SMS4MAC()
* Function:		Compute MAC 
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
UINT8 SMS4MAC(UINT8 *key, UINT8 *IV)
{
	int i = 0, j = 0;
    int newlen = 0;
    UINT16 nApduLen = 0;
	UINT8 tempIV[16] = {0};
	UINT8 OldMac[4] = {0};
    UINT16 MacDataLen = (6 + gApduBody.LC + sizeof(APDUHEADER_S) + 7 + 16 - 4)*sizeof(UINT8);
	UINT8* MacData = (UINT8 *) alloca(MacDataLen);
    memset(MacData, 0, MacDataLen);	

    DBG_FUNC_BEGIN;
    //DBG_INFO("Key data:");
    //DBG_BUF(key, 16);  
    
	memcpy(tempIV, IV, 8);
	memcpy(OldMac, gApduBody.DATA + (gApduBody.LC-4), 4);   
    //DBG_INFO("IV data:");
    //DBG_BUF(tempIV, 16);
    //DBG_INFO("Old Mac data:");
    //DBG_BUF(OldMac, 4);
	
    // 组装整个APDU数据包
    newlen = 0;
    nApduLen = sizeof(APDUHEADER_S) + gApduBody.LC + 4;
	memcpy(MacData, &gApduHeader, sizeof(APDUHEADER_S));
    newlen += sizeof(APDUHEADER_S);
    //
	gApduBody.LC -= 4;
	MacData[newlen] = (gApduBody.LC >> 8) & 0x00FF;				
	MacData[newlen + 1] = gApduBody.LC & 0x00FF;																	
    newlen += 2;
	MacData[newlen] = 0x00;				
	MacData[newlen + 1] = 0x00;																	
    newlen += 2;
    //  
	memcpy(MacData + newlen, gApduBody.DATA, gApduBody.LC);
    newlen += gApduBody.LC; 
    // 
	MacData[newlen] = 0x80;
    newlen += 1;
    //DBG_INFO("APDU data:");
    //DBG_BUF(MacData, newlen);
	
    // 若长度不为16整数倍，则需要补齐
	if (newlen % 16)
    {
		memset(MacData + newlen, 0, 16-newlen%16);
	}
	newlen += (16 - newlen%16);

    // 计算MAC
    for (i = 0; i < newlen / 16; i++)
    {
        for (j = 0; j < 16; j++)
        {         
			tempIV[j] = tempIV[j] ^ MacData[i*16 + j];
        } 
        SMS4_Init(key);		
		SMS4_Run(SMS4_ENCRYPT, SMS4_ECB, tempIV, tempIV, 16, NULL);
    }
                              
    //DBG_INFO("New Mac data:");
    //DBG_BUF(tempIV, 16);

    // 比较结果
	if (memcmp(OldMac, tempIV, 4) != 0)
    {
        DBG_ERR("Mac is not same!");
		return FALSE;
	}

    DBG_FUNC_END;
	return TRUE;
}
/***************************************************************************
* Subroutine:	GetAppStateInfoById
* Function:		根据appid返回app的状态信息
* Input:        appid -- 需要判断的app id
* Output:		app的当前的状态信息 
* Version:  V1.21 
* Date:		2013.12.16
* ModifyRecord:
* ************************************************************************/
APPSTATE* GetAppStateInfoById(UINT16 appid)
{
	UINT8 i;
	for (i = 0; i < APP_MAX; i++)
    {
		if (gAPPState[i].AppID == appid)
        {
			return &gAPPState[i]; 	
		}
	}
	return (APPSTATE*)0;
}
/***************************************************************************
* Subroutine:	SaveContainerId
* Function:		保存Container id
* Input:        appid -- 应用id
*               containerid -- 要保存的容器id
* Output:		SAR_OK -- 成功
*               SAR_NO_ROOM -- 空间不够
*               SAR_APPLICATION_NAME_INVALID -- 应用无效
*               SAR_INVALIDPARAMERR -- 其他参数错误 
* Version:  V1.21 
* Date:		2013.12.16
* ModifyRecord:
* ************************************************************************/
UINT16 SaveContainerId(UINT16 appid, UINT16 containerid)
{
	UINT8 i = 0;
    UINT8 j = 0;
	for (i = 0; i < APP_MAX; i++)
    {
		if (gAPPState[i].AppID == appid)
        {	
			for (j = 0; j < CON_MAX; j++)
            {
				if (gAPPState[i].ContainerID[j] == containerid)
                {
					return SAR_OK;
				}
			}
			for (j = 0; j < CON_MAX; j++)
            {
				if (gAPPState[i].ContainerID[j] == 0)
                {
					gAPPState[i].ContainerID[j] = containerid;
					return SAR_OK;
				}
			}
		}	
	}
	if (j == CON_MAX)
    {
		return SAR_NO_ROOM;
    }
	if (i == APP_MAX)
    {
		return SAR_APPLICATION_NOT_EXISTS;
    }

	return SAR_INVALIDPARAMERR;
}
/***************************************************************************
* Subroutine:	ClearContainerId
* Function:		清除Container id
* Input:        appid -- 应用id
*               containerid -- 要保存的容器id
* Output:		SAR_OK -- 成功
* Version:  V1.21 
* Date:		2013.12.16
* ModifyRecord:
* ************************************************************************/
UINT16 ClearContainerId(UINT16 appid, UINT16 containerid)
{
	UINT8 i,j;
	for (i = 0; i < APP_MAX; i++)
	{
		if (gAPPState[i].AppID == appid)
		{	
			for (j = 0; j < CON_MAX; j++)
			{
				if (gAPPState[i].ContainerID[j] == containerid)
				{
					gAPPState[i].ContainerID[j] = 0;
					return SAR_OK;
				}
			}
		}
	}
	return SAR_INDATAERR;
}
/***************************************************************************
* Subroutine:	IsContainerIdValid
* Function:		判断containerid是否合法
* Input:        appid -- 容器所在的app id
*				containerid -- 要判断的container id
* Output:		SAR_OK--容器合法
                SAR_FILEERR--容器无效 
* Version:  V1.21 
* Date:		2013.12.16
* ModifyRecord:
* ************************************************************************/
UINT16 IsContainerIdValid(UINT16 appid, UINT16 containerid)
{
	UINT8 i,j;
	for (i = 0; i < APP_MAX; i++)
    {	
		if (gAPPState[i].AppID == appid)
        {
			for (j = 0; j < CON_MAX; j++)
            {
				if (gAPPState[i].ContainerID[j] == containerid)
                {
					return SAR_OK;
				}
			}
		}
	}
	return SAR_CONTAINERID_NOT_EXISTS;
}
/***************************************************************************
* Subroutine:	SaveSessionKey
* Function:		保存会话密钥数据，并返回会话密钥句柄
* Input:        sessionKey -- 会话密钥数据
*				len -- 会话密钥数据长度
* Output:		0-SESSIONKEY_MAX--合法的会话密钥句柄
                SAR_NO_ROOM--会话密钥个数达上限 
* Version:  V1.21 
* Date:		2013.12.16
* ModifyRecord:
* ************************************************************************/
UINT16 SaveSessionKey(UINT8* keyData,UINT8 len,UINT32 algID)
{
	UINT8 i;
	for (i = 0; i < SESSIONKEY_MAX; i++)
    {
		if (gSessionKey[i].flag == 0)
        {
			gSessionKey[i].flag = 1; 
			gSessionKey[i].keyID = i;	
			gSessionKey[i].algID = algID;
			gSessionKey[i].keyLen = (len <= 32) ? len : 32;
			memset(gSessionKey[i].keyData, 0, 32);
			memcpy(gSessionKey[i].keyData, keyData,gSessionKey[i].keyLen);
			return i;
		}
	}

	return SAR_NO_ROOM;
}
/***************************************************************************
* Subroutine:	CleanSessionKeyState
* Function:		清除会话密钥状态
* Input:        keyid -- 会话密钥ID
* Output:		无
* Version:  V1.21 
* Date:		2013.12.16
* ModifyRecord:
* ************************************************************************/
void CleanSessionKeyState(UINT16 keyid)
{
    if (keyid < SESSIONKEY_MAX)
    {
        gSessionKey[keyid].flag = 1;
        gSessionKey[keyid].crypto_type = CT_UNKNOWN;
    }
}
/***************************************************************************
* Subroutine:	ReleaseSessionKey
* Function:		释放会话密钥数据
* Input:        sessionId -- 会话密钥句柄
* Output:		SAR_OK--释放会话密钥成功
                SAR_INVALIDPARAMERR--会话密钥句柄错误 
* Version:  V1.21 
* Date:		2013.12.16
* ModifyRecord:
* ************************************************************************/
UINT32 ReleaseSessionKey(UINT16 keyid)
{
	if (keyid < SESSIONKEY_MAX)
    {
    	gSessionKey[keyid].flag = 0;
    	memset(gSessionKey[keyid].keyData, 0, 32);
    	memset(gSessionKey[keyid].extData, 0, 240);
    }
	return SAR_OK;
}


void DoAS5xxCMD()
{  		
	//DBG_INFO("DoAS5xxCMD():gApduHeader.CLA = 0x%x, gApduHeader.INS = 0x%x", gApduHeader.CLA, gApduHeader.INS);
		
	if ((gApduHeader.CLA & 0xFB) != 0x80)
    {
		SetCmdResult(SAR_CLAERR,0);
        DBG_ERR("gApduHeader.CLA is wrong! gApduHeader.CLA = 0x%x", gApduHeader.CLA);
		return;
	}
	switch(gApduHeader.INS)
	{         	   
        case CMD_SET_LABEL:
        	CMD_SetLable();	break;
		case CMD_GET_COSINFO:
			CMD_GetCosInfo(); break;
		case CMD_GET_AUTH:
			CMD_DevAuth(); break;
		case CMD_CHANGE_DEVAUTHKEY:
			CMD_ChangeDevAuthkey(); break;
		case CMD_GET_PININFO:
			CMD_GetPinInfo(); break;	
		case CMD_CHANGE_PIN:
			CMD_ChangePin(); break;
		case CMD_VERIFY_PIN:
			CMD_VerifyPin(); break;
		case CMD_UNBLOCK_PIN:
			CMD_UnLockPin(); break;
		case CMD_CLEAR_SECURESTATE:
			CMD_ClearSecureState(); break;
		case CMD_CREATE_APPLICATION:
			CMD_CreateApplication(); break;
		case CMD_ENUM_APPLICATION:
			CMD_EnumApplication(); break;
		case CMD_DELETE_APPLICATION:
			CMD_DeleteApplication(); break;
		case CMD_OPEN_APPLICATION:
			CMD_OpenApplication(); break;
		case CMD_CLOSE_APPLICATION:
			CMD_CloseApplication(); break;
		case CMD_CREATE_FILE:
			CMD_CreateFile();break;
		case CMD_DELETE_FILE:
			CMD_DeleteFile();break;
		case CMD_ENUM_FILE:
			CMD_EnumFile();break;
		case CMD_GET_FILEINFO:
			CMD_GetFileInfo();break;
		case CMD_READ_FILE:
			CMD_ReadFile(); break;
		case CMD_WRITE_FILE:
			CMD_WriteFile();break;
		case CMD_CREATE_CONTAINER:
			CMD_CreateContainer();break;
		case CMD_OPEN_CONTAINER:
			CMD_OpenContainer();break;
		case CMD_CLOSE_CONTAINER:
			CMD_CloseContainer();break;
		case CMD_ENUM_CONTAINER:
			CMD_EnumContainer();break;
		case CMD_DELETE_CONTAINER:
			CMD_DeleteContainer();break;
		case CMD_GET_CONTAINERINFO:
            CMD_GetContainerInfo();break;
		case CMD_CONTYPE:
            CMD_GetCONType();break;
		case CMD_IMPORT_CERTIFICATE:
			CMD_ImportCert();break;
		case CMD_EXPORT_CERTIFICATE:
			CMD_ExportCert();break;
		case CMD_GEN_RANDOM:
			CMD_GenRandom();break;
#ifdef RSASUPPORT
		case CMD_GEN_EXTRSAKEY:
			CMD_GenExtRSAkey();break;
		case CMD_GEN_RSAKEYPAIR:
			CMD_GenRSAKeyPair();break;
		case CMD_IMPORT_RSAKEYPAIR:
			CMD_ImportRSAKeyPair();break;
		case CMD_IMPORT_PLAINRSAKEY: 
            CMD_ImportRSAKey();break;                    
        case CMD_RSASIGNDATA:
			CMD_RSASignData();break;
		case CMD_RSA_EXPORTSESSIONKEY:
			CMD_RSAExportSessionKey();break;
		case CMD_RSA_EXPORTSESSIONKEYEX:
			CMD_RSAExportSessionKeyEx();break;	
        case CMD_RSAEXTKEY_OPT:	
            CMD_ExtRSAEnDecrypt(); break;    
        case CMD_ExtRSA_PubOPT:
            CMD_ExtRSAPubKeyOPT();break;
        case CMD_ExtRSA_PriOPT:
            CMD_ExtRSAPriKeyOPT();break;
        case CMD_RSAVERIFY:
            CMD_RSAVerify();break;
#endif	                     
#ifdef ECCSUPPORT
		case CMD_GEN_ECCKEYPAIR:
			CMD_GenECCKeyPair();break;
		case CMD_IMPORT_ECCKEYPAIR:
			CMD_ImportECCkeyPair();break;
		case CMD_ECCSIGNDATA:
			CMD_ECCSignData();break;
		case CMD_ECCVERIFY:
			CMD_ECCVerify();break;		
		case CMD_ECC_EXPORTSESSIONKEY:
			CMD_ECCExportSessionKey();break;
		case CMD_ECC_EXPORTSESSIONKEYEX:
			CMD_ECCExportSessionKeyEx();break;
		case CMD_ECC_ENCRYPT:
			CMD_ExtECCEncrypt();break;
		case CMD_ECC_DECRYPT:
			CMD_ExtECCDecrypt();break;
		case CMD_EXT_ECCSIGN:
			CMD_ExtECCSign();break;
		case CMD_EXT_ECCVERIFY:
            CMD_ExtECCVerify();break;
		case CMD_GEN_AGREEDATA_ECC:
			CMD_GenerateAgreementDataWithECC();break;
		case CMD_GEN_AGREEDATAKEY_ECC:
			CMD_GenerateAgreementDataAndKeyWithECC();break;
		case CMD_GEN_KEY_ECC:
			CMD_GenerateKeyWithECC();break;	
#endif 			            			         	
		case CMD_EXPORT_PUBKEY:
			CMD_ExportPublicKey();break;
        case CMD_IMPORT_SESSIONKEY:
			CMD_ImportSessionKey();break;	            			
		case CMD_IMPORT_SYMMKEY:
			CMD_ImportSymmKey();break;
        case CMD_ENCRYPT_INIT:
			CMD_EncryptInit();break;
		case CMD_ENCRYPT:
			CMD_Encrypt();break;
		case CMD_ENCRYPT_UPDATA:
			CMD_EncryptUpdate();break;
		case CMD_ENCRYPT_FINAL:
			CMD_EncryptFinal();break;
		case CMD_DECRYPT_INIT:
			CMD_DecryptInit();break;
		case CMD_DECRYPT:
			CMD_Decrypt();break;
		case CMD_DECRYPT_UPDATA:
			CMD_DecryptUpdate();break;
		case CMD_DECRYPT_FINAL:
			CMD_DecryptFinal();break;			
		case CMD_DIGEST_INIT:
			CMD_DigestInit();break;
		case CMD_DIGEST:
			CMD_Digest();break;
		case CMD_DIGEST_UPDATA:
			CMD_DigestUpdate();break;
		case CMD_DIGEST_FINAL:
			CMD_DigestFinal();break;	
    	case CMD_MAC_INIT:
			CMD_MACInit();break;
		case CMD_MAC:
			CMD_Mac();break;
		case CMD_MAC_UPDATA:
			CMD_MACUpdate();break;
		case CMD_MAC_FINAL:
			CMD_MACFinal();break;
		case CMD_DESTROY_SESSIONKEY:
			CMD_DestroySessionKey();break;
        case CMD_CLEARPROG:
            CMD_ClearProg();break;
        default:
            SetCmdResult(SAR_UNSUPPORTFUNC,0);
    }
}
/***************************************************************************
* Subroutine:	CMD_SetLable()
* Function:		Set device lable 
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_SetLable()
{			  
	UINT32 nRet = SAR_OK;
	DEVINFO tDev = {0};

    DBG_FUNC_BEGIN;

    // Label长度检查
    if ((strlen((const char*)gApduBody.DATA) == 0) || (gApduBody.LC > MAX_LABLE_LEN))
	{
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

    // 读取设备信息文件0xEF01
	nRet = FS_SelectFileByCluster(MF_CLUSTER, MF_CLUSTER);
	nRet |= FS_SelectFileByID(0xEF01);
	nRet |= FS_ReadFile((UINT8*)&tDev, 0, sizeof(tDev));
	if (nRet != RT_OK)
	{                                      
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}

    // 将新的Label写入设备信息文件0xEF01
	memset((UINT8*)&tDev.dev.Label, 0, 32);
	memcpy((UINT8*)&tDev.dev.Label, gApduBody.DATA, strlen((const char*)gApduBody.DATA));
	nRet = FS_UpdateFile((UINT8*)&tDev, 0, sizeof(tDev));
	if (nRet != RT_OK)
	{                                 
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_GetCosInfo()
* Function:		Get COS info 
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GetCosInfo()
{
	UINT16 nRet = 0;
	UINT32 DevFreeSpace = 0;
	UINT16 FAT[FAT_SIZE_HALF] = {0};
	DEVINFOPRI* Dev = (DEVINFOPRI*)&FAT;
    cosDEVINFO cosDevInfo = {0};												 
                      
    DBG_FUNC_BEGIN;

    // 响应数据长度检查
	if (gApduBody.LE != 0x120)
    {                                   
        DBG_ERR("Le is wrong! gApduBody.LE = 0x%x", gApduBody.LE);
		SetCmdResult(SAR_OUTDATALENERR, 0);
		return;
	}

	memset((UINT8*)FAT, 0x00, FAT_SIZE_HALF*2);
	DevFreeSpace = FS_GetFreeSpace(FAT);

    // 定位信息文件0xEF01
	nRet = FS_SelectFileByCluster(MF_CLUSTER,MF_CLUSTER);
	nRet |= FS_SelectFileByID(0xEF01);
    if (nRet != RT_OK)
    {                            
        DBG_ERR("FS_SelectFileByID(0xEF01) failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0); 
		return;
    }
     
    // 读取设备信息文件0xEF01
	memset((UINT8*)Dev, 0x00, sizeof(DEVINFOPRI));
	nRet |= FS_ReadFile((UINT8*)Dev, 0, sizeof(DEVINFOPRI));
	if (nRet != RT_OK)
	{                                 
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0); 
		return;
	}
	
    // 返回数据
	Dev->FreeSpace = DevFreeSpace; 
    Dev->Version.major = gCosVersion.major;
    Dev->Version.minor = gCosVersion.minor;
    //   
    cosDevInfo.StructVersion.major = 1;
    cosDevInfo.StructVersion.minor = 1;
    memcpy(&(cosDevInfo.SpecificationVersion), &(Dev->Version), sizeof(VERSION));
    memcpy(cosDevInfo.Manufacturer, Dev->Manufacturer, 64);
    memcpy(cosDevInfo.Issuer, Dev->Issuer,64);
    memcpy(cosDevInfo.Label, Dev->Label,32);                      
    memcpy(cosDevInfo.SerialNumber, Dev->SerialNumber, 32);
    memcpy(&cosDevInfo.HWVersion, &(Dev->HWVersion), sizeof(VERSION));
    memcpy(&cosDevInfo.FirmwareVersion, &(Dev->FirmwareVersion), sizeof(VERSION));
    cosDevInfo.AlgSymCap = Dev->AlgSymCap;
    cosDevInfo.AlgAsymCap = Dev->AlgAsymCap;
    cosDevInfo.AlgHashCap = Dev->AlgHashCap;
    cosDevInfo.DevAuthAlgId = Dev->DevAuthAlgId;
    cosDevInfo.TotalSpace = Dev->TotalSpace;
    cosDevInfo.FreeSpace = Dev->FreeSpace;
    cosDevInfo.MaxApduDataLen = MAX_APDU_LEN;
    cosDevInfo.UserAuthMethod = 1;
    cosDevInfo.DeviceType = 2;
    cosDevInfo.MaxContainerNum = CON_MAX;
    cosDevInfo.MaxCertNum = 0;
    cosDevInfo.MaxFileNum = 0;	
    memcpy(gApduResp.DATA, (UINT8*)&cosDevInfo, sizeof(cosDEVINFO));

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, sizeof(cosDEVINFO));
}
/***************************************************************************
* Subroutine:	CMD_DevAuth()
* Function:		Device ExterAuth
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DevAuth()
{
	UINT16 nRet = SAR_OK;
	UINT8 RandDataTemp[16] = {0};
	DEVINFO tDev = {0};
	 
    DBG_FUNC_BEGIN;

	if (gApduBody.LC > 32)
	{                         
        DBG_ERR("Le is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

    // 读取设备认证密钥
	nRet = FS_SelectFileByCluster(MF_CLUSTER,MF_CLUSTER);
	nRet |= FS_SelectFileByID(0xEF01); 
	nRet |= FS_ReadFile((UINT8*)&tDev,0,sizeof(tDev));
	if (nRet != RT_OK)
	{                                   
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	if (tDev.DevKey[0].LeftTryTimes == 0)
	{                              
        DBG_ERR("PIN is locked!");
		SetCmdResult(SAR_PIN_LOCKED, 0);
		return;
	}

    // 解密随机数
    if (gApduHeader.P2 == 0x02) //SMS4_ECB
    {    
    	SMS4_Init(tDev.DevKey[0].Key);
    	SMS4_Run(SMS4_DECRYPT, SMS4_ECB, gApduBody.DATA, RandDataTemp, gApduBody.LC, NULL);
    }
    else
    {                                       
        DBG_ERR("P2 is wrong! gApduHeader.P2 = 0x%x", gApduHeader.P2);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
    }                 

    // 结果不一致
	if (memcmp(gRandData, RandDataTemp, 8) !=0 )
	{																 
		tDev.DevKey[0].LeftTryTimes--;
		nRet = FS_UpdateFile((UINT8*)&tDev, 0, sizeof(tDev));
		if (nRet != RT_OK)
		{                                    
            DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0);
			return;
		}                      
        DBG_ERR("Verify key failed! LeftTryTimes = 0x%x", tDev.DevKey[0].LeftTryTimes);
		SetCmdResult(SAR_PIN_INVALID | (tDev.DevKey[0].LeftTryTimes&0x0000000F), 0);
		return;
	}
	
    // 认证成功，重试次数复位
	if (tDev.DevKey[0].LeftTryTimes != tDev.DevKey[0].RetryTimes)
	{
		tDev.DevKey[0].LeftTryTimes = tDev.DevKey[0].RetryTimes;
		nRet = FS_UpdateFile((UINT8*)&tDev, 0, sizeof(tDev));
		if (nRet != RT_OK)
		{                                     
            DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0);
			return;
		}
	}

    // 设置当前状态
	if (gCurSAStatus < SA_EXTERAUTH || gCurSAStatus > SA_SOLOGIN)
    {
	   gCurSAStatus = SA_EXTERAUTH;
    }

    DBG_FUNC_END;
	SetCmdResult(SAR_OK,0);		
}
/***************************************************************************
* Subroutine:	CMD_ChangeDevAuthkey()
* Function:		Change dev auth key 
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ChangeDevAuthkey()
{
	UINT16 nRet = SAR_OK;
	UINT16 nStatus = 0;
	UINT8 *data = NULL;
	DEVINFO tDev = {0};

    DBG_FUNC_BEGIN;

    // 安全状态检查
	nStatus = IsHasControl(SA_EXTERAUTH, SA_SOLOGIN, CON_SA|CON_CARDLOCK);
	if (nStatus != SAR_OK)
    {
        DBG_ERR("The user is not login!");
		SetCmdResult(nStatus, 0);
    	return;
	}
	if (gApduBody.LC == 0)
	{                               
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	data = gApduBody.DATA;

    // 读取设备信息文件0xEF01
	nRet = FS_SelectFileByCluster(MF_CLUSTER,MF_CLUSTER);
	nRet |= FS_SelectFileByID(0xEF01);
	nRet |= FS_ReadFile((UINT8*)&tDev,0,sizeof(tDev));
	if (nRet != RT_OK)
	{                         
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}

    // 校验MAC
	nRet = SMS4MAC((UINT8*)gPwd, gRandData);        //tDev.DevKey[0].Key ---> gPwd
	if (!nRet)
	{                                     
        DBG_ERR("SMS4MAC() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_INVALIDDATA, 0);
		return;
	}

    // 解密数据
	switch(gApduHeader.P2)
	{
		case 0x00:  //SM1_ECB
			SM1_Init((UINT8*)gPwd, (UINT8*)gPwd+16);//tDev.DevKey[0].Key ---> gPwd
			SM1_Run(SM1_DECRYPT, SM1_ECB, data, data, 16, NULL);  //32 change to 16
			break;
		case 0x02:  //SMS4_ECB
			SMS4_Init((UINT8*)gPwd);                //tDev.DevKey[0].Key ---> gPwd
			SMS4_Run(SMS4_DECRYPT, SMS4_ECB, data, data, 16, NULL); //32 change to 16
			break;
		case 0x01:  //SSF33_ECB
		default:               
            DBG_ERR("P2 is wrong! gApduHeader.P2 = 0x%x", gApduHeader.P2);
			SetCmdResult(SAR_INVALIDPARAMERR, 0);
			return;
	}

	memcpy(tDev.DevKey[0].Key, data, DEFAULTDEVKEYLEN);
	nRet = FS_UpdateFile((UINT8*)&tDev, 0, sizeof(tDev));
	if (nRet != RT_OK)
	{                           
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_GetPinInfo()
* Function:		Get application pin key info 
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GetPinInfo()
{                   
    UINT16 i = 0;
    UINT16 nRet = SAR_OK;
	UINT16 APPID = 0;
	KEYFILE tKey[2] = {0};

    DBG_FUNC_BEGIN;    

    // 长度判断
	if (gApduBody.LC != 2)
	{              
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	if (gApduBody.LE != 3)
	{                                
        DBG_ERR("Le is wrong! gApduBody.LE = 0x%x", gApduBody.LE);
		SetCmdResult(SAR_OUTDATALENERR, 0);
		return;
	}

    // 获取应用状态
	APPID = gApduBody.DATA[1]<<8 | gApduBody.DATA[0];
	if (!GetAppStateInfoById(APPID))
	{                              
        DBG_ERR("The application is not open! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_OPEN, 0);
		return;
	}
	FS_SelectFileByCluster(APPID, APPID);

    // 读取应用信息文件
	nRet = FS_SelectFileByID(PINFILEID);
	nRet |= FS_ReadFile((UINT8*)&tKey, 0, sizeof(tKey));
	if (nRet != RT_OK)
	{                                  
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	
    //admin pin information
	if (gApduHeader.P2 == 0x00)       
	{
		i=0;
	}
    // user pin
	else if (gApduHeader.P2 == 0x01)  
	{
		i=1;
	}
	else
	{                                        
        DBG_ERR("P2 is wrong! gApduHeader.P2 = 0x%x", gApduHeader.P2);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
	}

    // 返回数据
	gApduResp.DATA[0] = tKey[i].RetryTimes;
	gApduResp.DATA[1] = tKey[i].LeftTryTimes;
	if (tKey[i].Version != DEFAULT_PIN_VERSION)
	{
		gApduResp.DATA[2] = 0x00;
	}
	else
	{
		gApduResp.DATA[2] = 0x01;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, gApduBody.LE);	
}
/***************************************************************************
* Subroutine:	CMD_ChangePin()
* Function:		Change app pin 
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ChangePin()
{                   
    UINT16 i = 0;
    UINT16 nRet = SAR_OK;
    UINT16 nStatus = 0;
	UINT16 APPID = 0;
	UINT8 *data = NULL;
	UINT8 SHA1Hash[20] = {0};
	UINT8* TempKey = SHA1Hash;
	KEYFILE tKey[2] = {0};
	APPSTATE* AppState = NULL; 	

    DBG_FUNC_BEGIN;

    // 数据长度检查
	if (gApduBody.LC == 0)
	{                                  
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

    //admin pin information
	if (gApduHeader.P2 == 0)      
	{
    	nStatus = IsHasControl(SA_SOLOGIN, SA_SOLOGIN, CON_ALL);
    	if (nStatus != SAR_OK)
        {                            
            DBG_ERR("SO pin is not login!");
    		SetCmdResult(nStatus, 0);
        	return;
    	}
		i = 0;
	}
    // user pin
	else if (gApduHeader.P2 == 0x01) 
	{	
    	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
    	if (nStatus != SAR_OK)
        {                           
            DBG_ERR("User pin is not login!");
    		SetCmdResult(nStatus, 0);
        	return;
    	}
		i = 1;
	}
	else
	{	                         
        DBG_ERR("P2 is wrong! gApduHeader.P2 = 0x%x", gApduHeader.P2);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
	}         
       
	APPID = gApduBody.DATA[1]<<8 | gApduBody.DATA[0];
	data = gApduBody.DATA + 2;
	  
    // 获取应用状态信息 
	AppState = GetAppStateInfoById(APPID);
	if(!AppState)
	{                                             
        DBG_ERR("The application is not open! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_OPEN, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;

    // 读取应用PIN文件
	FS_SelectFileByCluster(APPID, APPID);
	nRet = FS_SelectFileByID(PINFILEID);
	nRet |= FS_ReadFile((UINT8*)&tKey, 0, sizeof(tKey));
	if (nRet != RT_OK)
	{                        
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	if (tKey[i].LeftTryTimes == 0)
	{                                   
        DBG_ERR("The pin is locked!");
		SetCmdResult(SAR_PIN_LOCKED, 0);
		return;
	}

    // Hash Key
	SHA1_Init(&gSHA1Context);
	SHA1_Update(&gSHA1Context, tKey[i].Key, DEFAULTDEVKEYLEN);
	SHA1_Final(&gSHA1Context, SHA1Hash);

    // 校验MAC
	nRet = SMS4MAC(TempKey, gRandData);
	if(!nRet)
	{                            
        DBG_ERR("SMS4MAC() is failed!");
		SetCmdResult(SAR_INVALIDDATA,0);
		return;
	}
	
    // 解码数据				 
//	DBG_INFO("Encrypted data=");
//	DBG_BUF(data,DEFAULTDEVKEYLEN);
	SMS4_Init(TempKey);
	SMS4_Run(SMS4_DECRYPT, SMS4_ECB, data, data, DEFAULTDEVKEYLEN, NULL);
//	DBG_INFO("Decrypted data=");
//	DBG_BUF(data,DEFAULTDEVKEYLEN);
	tKey[i].Version = 1;
	memcpy(tKey[i].Key, data, DEFAULTDEVKEYLEN);
//	DBG_INFO("tKey[i].Key=");
//	DBG_BUF(tKey[i].Key,DEFAULTDEVKEYLEN);

    // 保存新的PIN
	nRet = FS_UpdateFile((UINT8*)tKey, 0, sizeof(tKey));
	if (nRet != RT_OK)
	{                                  
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_VerifyPin()
* Function:		Verify pin  
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_VerifyPin()
{                 
    UINT16 nRet = SAR_OK;
	UINT16 APPID = 0;
    UINT16 i = 0;
    UINT16 j = 0;
	UINT8 *data = NULL;
	UINT8 SHA1Hash[20] = {0};
	UINT8 RandDataTemp[16] = {0};
	KEYFILE tKey[2] = {0}; 	

    DBG_FUNC_BEGIN;   	

    // 长度检查		
	if (gApduBody.LC != 0x12)
	{                                       
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
    
    //admin pin information	
	if (gApduHeader.P2 == 0x00)       
	{
		i = 0;
	}
    // user pin
	else if (gApduHeader.P2 == 0x01)  
	{
		i = 1;
	}
	else
	{                                   
        DBG_ERR("P2 is wrong! gApduHeader.P2 = 0x%x", gApduHeader.P2);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
	}
                      
	APPID = gApduBody.DATA[1]<<8 | gApduBody.DATA[0];
	data = gApduBody.DATA+2;

    // 检查应用状态
	if (!GetAppStateInfoById(APPID))
	{ 										
        DBG_ERR("The application is not open! APPID = 0x%x", APPID);		
		SetCmdResult(SAR_APPLICATION_NOT_OPEN, 0);
		return;
	}	
	FS_SelectFileByCluster(APPID, APPID);
  
    // 读取应用PIN文件
	nRet = FS_SelectFileByID(PINFILEID);     
	nRet |= FS_ReadFile((UINT8*)&tKey, 0, sizeof(tKey));	
	if (nRet != RT_OK)
	{                               
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	if (tKey[i].LeftTryTimes == 0)
	{                                
        DBG_ERR("The pin is locked!");
		SetCmdResult(SAR_PIN_LOCKED, 0);
		return;
	}	

    // 解码数据
	SHA1_Init(&gSHA1Context);
	SHA1_Update(&gSHA1Context, tKey[i].Key, DEFAULTDEVKEYLEN);
	SHA1_Final(&gSHA1Context, SHA1Hash);
	SMS4_Init(SHA1Hash);
	SMS4_Run(SMS4_DECRYPT, SMS4_ECB, data, RandDataTemp, DEFAULTDEVKEYLEN, NULL);
	
    // 数据比较失败
	nRet = memcmp(gRandData, RandDataTemp, 8);
	if (nRet != 0)
	{
		tKey[i].LeftTryTimes--;
		nRet = FS_UpdateFile((UINT8*)&tKey, 0, sizeof(tKey));
		if (nRet != RT_OK)
		{                                  
            DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0);
			return;
		}
		SetCmdResult(SAR_PIN_INVALID | ((tKey[i].LeftTryTimes&0x0F)<<8), 0);
		return;
	}

    // 验证PIN码成功，改变该应用状态
	if (i == 0)
	{  
		gCurSAStatus = SA_SOLOGIN; 
	}
	else
	{
		gCurSAStatus = SA_USERLOGIN;
	}
 	for (j = 0; j < APP_MAX; j++)
	{
		if (gAPPState[j].AppID == APPID)
		{
			gAPPState[j].AppState = gCurSAStatus;
			break;
		}
	}

    // 重试次数复位
	if (tKey[i].LeftTryTimes != tKey[i].RetryTimes)
	{
		tKey[i].LeftTryTimes = tKey[i].RetryTimes;
		nRet = FS_UpdateFile((UINT8*)&tKey, 0, sizeof(tKey));
		if (nRet != RT_OK)
		{                                
            DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0);
			return;
		}
	}
	
    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_UnLockPin()
* Function:		Unlock pin(just when pin locked valid) 
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_UnLockPin()
{                 
    UINT16 nRet = SAR_OK;
	UINT16 APPID = 0;
    UINT16 nStatus = 0;
	UINT8 *data = NULL;
	UINT8 SHA1Hash[20] = {0};
	UINT8* TempKey = SHA1Hash;
	KEYFILE tKey[2] = {0};
   	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;	

    // 长度检查			
	if (gApduBody.LC == 0)
	{                           
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	APPID = gApduBody.DATA[1]<<8 | gApduBody.DATA[0];
	data = gApduBody.DATA + 2;
    
    // 检查应用状态	   
	AppState = GetAppStateInfoById(APPID);
	if (!AppState)
	{                                            
        DBG_ERR("The application is not open! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_OPEN, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_SOLOGIN, SA_SOLOGIN, CON_SA|CON_CARDLOCK);
	if (nStatus != SAR_OK)
    {                         
        DBG_ERR("SO pin is not login!");
		SetCmdResult(nStatus, 0);
    	return;
	}

	FS_SelectFileByCluster(APPID, APPID);
	nRet = FS_SelectFileByID(PINFILEID);
	if (nRet != RT_OK)
	{                                 
        DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}	
	nRet = FS_ReadFile((UINT8*)&tKey, 0, sizeof(tKey));
	if (nRet != RT_OK)
	{                              
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
//	if(tKey[1].LeftTryTimes!=0)
//	{
//		SetCmdResult(SAR_FILEERR,0);
//		return;
//	}
	
    // Hash Key
	//DBG_INFO("tKey[i].Key=");
	//DBG_BUF(tKey[i].Key,DEFAULTDEVKEYLEN);
	SHA1_Init(&gSHA1Context);
	SHA1_Update(&gSHA1Context, tKey[0].Key, DEFAULTDEVKEYLEN);
	SHA1_Final(&gSHA1Context, SHA1Hash);
	
    // 校验MAC
	nRet = SMS4MAC(TempKey, gRandData);
	if (!nRet)
	{                   
        DBG_ERR("SMS4MAC() failed!");
		SetCmdResult(SAR_INVALIDDATA, 0);
		return;
	}
	
    // 解密数据					  
	//DBG_INFO("Encrypted data=");
	//DBG_BUF(data,DEFAULTDEVKEYLEN);
	SMS4_Init(TempKey);
	SMS4_Run(SMS4_DECRYPT, SMS4_ECB, data, data, DEFAULTDEVKEYLEN, NULL);   
	//DBG_INFO("Decrypted data=");
	//DBG_BUF(data,DEFAULTDEVKEYLEN);
	tKey[1].Version = 1;
	memcpy(tKey[1].Key, data, DEFAULTDEVKEYLEN);
	tKey[1].LeftTryTimes = tKey[1].RetryTimes; 
	//DBG_INFO("tKey[i].Key=");
	//DBG_BUF(tKey[i].Key,DEFAULTDEVKEYLEN);

    // 更新PIN文件
	nRet = FS_UpdateFile((UINT8*)tKey, 0, sizeof(tKey));
	if (nRet != RT_OK)
	{                       
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_ClearSecureState()
* Function:		Clear curente secure state 
* Author:   Syno Common		 
* Date:		2015.6.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ClearSecureState()
{                   
    UINT16 i = 0;
	UINT16 APPID = 0;
    
    DBG_FUNC_BEGIN;    

    // 数据长度检查   
	if (gApduBody.LC != 0x02)
	{                           
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
    	
    // 应用状态检查
	APPID = gApduBody.DATA[1]<<8 | gApduBody.DATA[0];
	if (!GetAppStateInfoById(APPID))
	{                                      
        DBG_ERR("The application is not open! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_OPEN, 0);
		return;
	}	 	

    // 清除状态
	gCurSAStatus = SA_EXTERAUTH;
    for (i= 0; i < APP_MAX; i++)
    {
    	if (gAPPState[i].AppID == APPID)
    	{
    		gAPPState[i].AppState = gCurSAStatus;
    		break;
    	}    		       
    }
    
    DBG_FUNC_END; 
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_CreateApplication()
* Function:		Create app
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_CreateApplication()
{                       
	APPINFO appinfo = {0};
    cosAPPLICATIONINFO app = {0};
	tFileNode* Node = (tFileNode*)&appinfo;
	UINT8 tempbuf[48] = {0};
    UINT16 nRet = SAR_OK;
    UINT16 nStatus = 0;
    UINT32 retrytimes = 0;
    
    DBG_FUNC_BEGIN;
      
	if (gApduBody.LC != sizeof(cosAPPLICATIONINFO))
	{                                   
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	nStatus = IsHasControl(SA_EXTERAUTH, SA_SOLOGIN, CON_SA|CON_CARDLOCK);
	if (nStatus != SAR_OK)
    {                 
        DBG_ERR("Device is not authenticated!");
		SetCmdResult(nStatus, 0);
    	return;
	}

	nRet = FS_SelectFileByCluster(MF_CLUSTER, MF_CLUSTER);
	if (nRet != RT_OK)
	{                                  
        DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}	
	memcpy(&app, gApduBody.DATA, sizeof(cosAPPLICATIONINFO));
	memcpy(tempbuf, app.szApplicatinName, MAX_APPLICATIONNAME_LEN);
	tempbuf[MAX_APPLICATIONNAME_LEN + 1] = '\0';
	nRet = FS_SelectDirOrFileByName(FT_DF, (INT8*)tempbuf);
	if (RT_OK == nRet)
	{                                         
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_APPLICATION_EXISTS, 0);
		return;
	}
	
	memset((UINT8*)Node, 0, sizeof(tFileNode));
    Node->FileType = FT_DF;
    Node->FileSize = 0; 
    strcpy(Node->FCP.Name, (INT8*)tempbuf);
	nRet = FS_CreateDir(Node);
	if (RT_OK != nRet)
	{                            
        DBG_ERR("FS_CreateDir() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

	FS_SelectDirOrFileByName(FT_DF, (INT8*)tempbuf);
	memset((UINT8*)Node, 0, sizeof(tFileNode));
    Node->FileType = FT_KEY;
    Node->FileSize = sizeof(APPINFO);
    Node->FCP.FileID = PINFILEID;
    strcpy(Node->FCP.Name, "AppInfoFile");
	nRet = FS_CreateFile(Node);
	if (RT_OK != nRet)
	{						   
        DBG_ERR("FS_CreateFile() failed! nRet = 0x%x", nRet);
		goto E_CreateKeyFileNode;
	}
	memset((UINT8*)&appinfo, 0, sizeof(appinfo));
	appinfo.key[0].Index = KI_SOPIN;
	appinfo.key[0].KeyType = KT_SOPIN;
	appinfo.key[0].Version = 1;
	appinfo.key[0].AlgFlag = 0;
	appinfo.key[0].UseControl = 0xFF;
	appinfo.key[0].ModifyControl = 0xCC;
	
	retrytimes = app.dwAdminPinRetryCount;
	appinfo.key[0].RetryTimes = retrytimes;
	appinfo.key[0].LeftTryTimes = appinfo.key[0].RetryTimes;

	memcpy(appinfo.key[0].Key, app.szAdminPin, 16);

	appinfo.key[1].Index = KI_USERPIN;
	appinfo.key[1].KeyType = KT_USERPIN;
	appinfo.key[1].Version = 1;
	appinfo.key[1].AlgFlag = 0;
	appinfo.key[1].UseControl = 0xCC;
	appinfo.key[1].ModifyControl = 0xCC;
	
	retrytimes = app.dwUserPinRetryCount;
	appinfo.key[1].RetryTimes = retrytimes;
	appinfo.key[1].LeftTryTimes = appinfo.key[1].RetryTimes;
	memcpy(appinfo.key[1].Key, app.szUserPin, 16);

	appinfo.byCertNum = app.byCertNum;
	appinfo.byContainerNum = app.byContainerNum;
	appinfo.wFileNum = app.wFileNum;
	appinfo.dwCreateFileRights = app.dwCreateFileRights;
	nRet = FS_UpdateFile((UINT8*)&appinfo, 0, sizeof(appinfo));
	if (RT_OK != nRet)
	{						
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		goto E_CreateKeyFile;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
	return;
	
E_CreateKeyFile:
	//release key file
	FS_DeleteFile();
E_CreateKeyFileNode:
	FS_DeleteFile();
E_CreateAPP:
	SetCmdResult(SAR_WRITEFILEERR, 0);
}
/***************************************************************************
* Subroutine:	CMD_EnumApplication()
* Function:		Enum App 
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_EnumApplication()
{
    UINT16 nRet = SAR_OK;
    UINT16 rlen = 0;

    DBG_FUNC_BEGIN;
	
	if (gApduBody.LC != 0x00)
	{                            
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	nRet = FS_SelectFileByCluster(MF_CLUSTER, MF_CLUSTER); 
	if (RT_OK != nRet)
	{                                   
        DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	memset(gApduResp.DATA, 0x00, 100);
	FS_EnumDir(gApduResp.DATA, &rlen);

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, rlen);
}
/***************************************************************************
* Subroutine:	CMD_DeleteApplication()
* Function:		Delete application
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DeleteApplication()
{
	UINT16 nRet = SAR_OK;
    UINT16 rlen = 0;
    UINT16 templen = 0;
    UINT16 nStatus = 0;
	UINT8 FileName[36] = {0};
	UINT8* tempbuf = NULL;
                 
    DBG_FUNC_BEGIN;

	nStatus = IsHasControl(SA_EXTERAUTH, SA_SOLOGIN, CON_SA|CON_CARDLOCK);
	if (nStatus != SAR_OK)
    {                           
        DBG_ERR("Device is not authenticated!");
		SetCmdResult(nStatus, 0);
    	return;
	}
	if (gApduBody.LC == 0 || gApduBody.LC > MAX_APPLICATIONNAME_LEN || gApduBody.LC != strlen((INT8*)gApduBody.DATA))
	{                                   
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR,0);
		return;
	}	
	nRet = FS_SelectFileByCluster(MF_CLUSTER, MF_CLUSTER);
	if (RT_OK != nRet)
	{                             
        DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}

	memcpy(FileName, gApduBody.DATA, MAX_APPLICATIONNAME_LEN);
	FileName[gApduBody.LC]='\0';

	nRet = FS_SelectDirOrFileByName(FT_DF, (INT8*)FileName);
	if (RT_OK != nRet)
	{                                    
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_APPLICATIONNAME_NOT_EXISTS, 0);
		return;
	}

	nRet = FS_SelectFileByID(PINFILEID);
	FS_DeleteFile();
	tempbuf = gApduBody.DATA;
    FS_EnumFile(FT_BIN, tempbuf, &rlen);

	templen = 0;
	while (rlen > templen)
	{
		if (tempbuf[templen] == 0) break;
		nRet = FS_SelectDirOrFileByName(FT_BIN, (INT8*)(tempbuf+templen));
		if (RT_OK == nRet)
		{
			FS_DeleteFile();
		}
		templen += (strlen((INT8 *)(tempbuf+templen)) + 1);
	}
	
	FS_Enum(FT_DF, tempbuf, &rlen);
	templen = 0;
	while (rlen > templen)
	{
		if (tempbuf[templen] == 0) break;
		CMD_DeleteContainerByName((INT8*)(tempbuf + templen));
		templen += (strlen((INT8 *)(tempbuf + templen)) + 1);
	}               

	nRet = FS_SelectDirOrFileByName(FT_DF, (INT8*)FileName);   
	if (nRet != RT_OK)
	{                                
        DBG_ERR("FS_SelectDirOrFileByName() failed! FileName = %s, nRet = 0x%x", FileName, nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}		
	nRet = FS_DeleteFile();
	if (nRet != RT_OK)
	{                                
        DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}		
    
    DBG_FUNC_END;	
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_OpenApplication()
* Function:		Open an application 
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_OpenApplication()
{
	UINT16 nRet = SAR_OK;
    UINT16 i = 0;
    UINT16 AppID = 0;
	UINT8 FileName[36] = {0};

    DBG_FUNC_BEGIN;

	if (gApduBody.LC == 0 || gApduBody.LC > MAX_APPLICATIONNAME_LEN)
	{                     
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	memcpy(FileName, gApduBody.DATA, MAX_APPLICATIONNAME_LEN);
	FileName[gApduBody.LC] = '\0';
	
	nRet = FS_SelectFileByCluster(MF_CLUSTER, MF_CLUSTER);
	if (nRet != RT_OK)
	{                             
        DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	
	nRet = FS_SelectDirOrFileByName(FT_DF, (INT8*)FileName);
	if (RT_OK != nRet)
	{                          
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}

	nRet = FS_SelectFileByID(PINFILEID);
	if (RT_OK != nRet)
	{                             
        DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	
	nRet = FS_ReadFile(gApduResp.DATA, 2*sizeof(KEYFILE), 8);
	if (RT_OK != nRet)
	{                         
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}	
	AppID = gCurrentDirectoryCluster;

 	for (i = 0; i < APP_MAX; i++)
	{
    	if (gAPPState[i].AppID == AppID)
    	{
			if (gAPPState[i].AppState >= SA_EXTERAUTH)
    			gCurSAStatus = gAPPState[i].AppState;
        	break;
		}
	}
	if (i == APP_MAX)
	{	
		for (i = 0; i < APP_MAX; i++)
		{
	    	if (gAPPState[i].AppID == 0)
	    	{				    	
	        	gAPPState[i].AppID = AppID;
	        	gAPPState[i].AppState = 0;	  
				memset(gAPPState[i].ContainerID, 0, sizeof(UINT16)*CON_MAX);
	        	break;
			}
		} 
	    if (i == APP_MAX)
	    {                   
            DBG_ERR("The application count meets the max!");
	        SetCmdResult(SAR_NO_ROOM, 0);
	        return;   	
	    }  
	} 
	gApduResp.DATA[8] = AppID & 0x00FF;
	gApduResp.DATA[9] = (AppID>>8) & 0x00FF;

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 10);	
}
/***************************************************************************
* Subroutine:	CMD_CloseApplication()
* Function:		Close app
* Author:   Syno Common		 
* Date:		2015.06.20
* Version:  V1.21
* ModifyRecord:
* ************************************************************************/
void CMD_CloseApplication()
{
	UINT16 APPID = 0;
    UINT16 i = 0;

    DBG_FUNC_BEGIN;

	if (gApduBody.LC != 2)
	{                                  
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	APPID = (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if (!GetAppStateInfoById(APPID))
	{                                       
        DBG_ERR("This application id is not exist! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	for (i = 0; i < APP_MAX; i++)
	{
		if (gAPPState[i].AppID == APPID)
		{
			gAPPState[i].AppID = 0;
			memset(&gAPPState[i].ContainerID, 0, sizeof(UINT16)*CON_MAX);
			if (gAPPState[i].AppState < SA_EXTERAUTH)
				gCurSAStatus = SA_EXTERAUTH;
			break;
		}
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_CreateFile()
* Function:		Create file 
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_CreateFile()
{
	UINT16 nRet = SAR_OK;
    UINT16 nStatus = SAR_OK;
    UINT16 APPID = 0;
	FILEATT fp = {0};
	APPINFO app = {0};
	tFileNode* Node = (tFileNode*)&app;
	UINT8 tempbuf[50] = {0};
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;

	if (gApduBody.LC != sizeof(FILEATT))
	{                        
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR,0);
		return;
	}
	memcpy(&fp, gApduBody.DATA, sizeof(FILEATT));
	APPID = (gApduHeader.P1<<8) | gApduHeader.P2;

	AppState = GetAppStateInfoById(APPID);
	if(!AppState)
	{                                         
        DBG_ERR("This application id is not exist! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
	nRet = FS_SelectFileByCluster(APPID, APPID);
	nRet |= FS_SelectFileByID(PINFILEID);
	nRet |= FS_ReadFile((UINT8*)&app, 0, sizeof(APPINFO));
	switch(app.dwCreateFileRights)
	{
    	case SECURE_ADM_ACCOUNT:
            nStatus = IsHasControl(SA_SOLOGIN, SA_SOLOGIN, CON_ALL);
    	    if (nStatus != SAR_OK)
			{                     
                DBG_ERR("SO PIN is not login!");
				SetCmdResult(nStatus, 0);
				return;
    	    }
    	    break;
    	case SECURE_USER_ACCOUNT:
            nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
    	    if (nStatus != SAR_OK)
			{                       
                DBG_ERR("User PIN is not login!");
		      	SetCmdResult(nStatus, 0);
    	       	return;
    	    }
    	    break;            
        case SECURE_NEVER_ACCOUNT:
            DBG_ERR("No any user can do this!");
            SetCmdResult(SAR_UNSUPPORTFUNC, 0);
            return;
        case SECURE_ANYONE_ACCOUNT:
            DBG_ERR("Any one can do this!");
            break;		
	}
	strcpy((char*)tempbuf, (char*)fp.name); 
	FS_SelectFileByCluster(APPID, APPID);
	nRet = FS_SelectDirOrFileByName(FT_BIN, (INT8*)tempbuf);
	if (nRet == RT_OK)
	{                                
        DBG_ERR("The same name file has exist!");
		SetCmdResult(SAR_FILE_ALREADY_EXIST, 0);
		return;
	}
	memset((UINT8*)Node, 0, sizeof(tFileNode));
    Node->FileType = FT_BIN;
    Node->FileSize = fp.filesize;
    Node->FCP.FileID = 0x0001;
    Node->FCP.fControl.FileControl.ReadControl = fp.readright;
    Node->FCP.fControl.FileControl.WriteControl = fp.writeright;  
    strcpy(Node->FCP.Name, (INT8*)tempbuf);
	nRet = FS_CreateFile(Node);
	if (RT_OK != nRet)
	{                                   
        DBG_ERR("FS_CreateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_DeleteFile()
* Function:		Delete file
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.20
* ModifyRecord:
* ************************************************************************/
void CMD_DeleteFile()
{
	UINT16 APPID = 0;
    UINT16 nRet = SAR_OK;
    UINT16 nStatus = 0;
	INT8 *filename = NULL;
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;

	if (gApduBody.LC == 0 || gApduBody.LC > MAX_FILENAME_LEN)
	{    
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	
    filename = (INT8*)gApduBody.DATA;
	filename[gApduBody.LC] = '\0';    
	APPID = (gApduHeader.P1<<8) | gApduHeader.P2;

	AppState = GetAppStateInfoById(APPID);
	if (!AppState)
	{                          
        DBG_ERR("This application id is not exist! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
    {                 
        DBG_ERR("User PIN is not login!");
		SetCmdResult(nStatus, 0);
    	return;
	}

	nRet = FS_SelectFileByCluster(APPID, APPID);
	nRet |= FS_SelectDirOrFileByName(FT_BIN, filename);
	if (nRet != RT_OK)
	{                           
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	nRet = FS_DeleteFile();
	if (nRet != RT_OK)
	{                                
        DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_EnumFile()
* Function:		Enum file
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_EnumFile()
{
    UINT16 rlen = 0;
    UINT16 APPID = 0;
    UINT16 nRet = SAR_OK;
    UINT16 nStatus = 0;
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;	

	APPID = (gApduHeader.P1<<8) | gApduHeader.P2;
	AppState = GetAppStateInfoById(APPID);
	if (!AppState)
	{                                            
        DBG_ERR("This application id is not exist! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
    {                           
        DBG_ERR("User PIN is not login!");
		SetCmdResult(nStatus, 0);
    	return;
	}

	nRet = FS_SelectFileByCluster(APPID, APPID);
	if (nRet != RT_OK)
	{                                      
        DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}	
	FS_EnumFile(FT_BIN, gApduResp.DATA, &rlen);
    
    DBG_FUNC_END;  
	SetCmdResult(SAR_OK, rlen);
}
/***************************************************************************
* Subroutine:	CMD_GetFileInfo()
* Function:		Get file info
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GetFileInfo()
{
	UINT16 nRet = SAR_OK;
    UINT16 nStatus = SAR_OK;
	UINT16 APPID = 0;
	INT8 *filename = NULL;
	tFileNode TempNode = {0};	
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;

	if (gApduBody.LC == 0 || gApduBody.LC > MAX_FILENAME_LEN)
	{                  
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	filename = (INT8*)gApduBody.DATA;
	filename[gApduBody.LC] = '\0';	
	APPID = (gApduHeader.P1<<8) | gApduHeader.P2;

	AppState = GetAppStateInfoById(APPID);
	if (!AppState)
	{
        DBG_ERR("This application id is not exist! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;

	nRet = FS_SelectFileByCluster(APPID, APPID);	 
	nRet |= FS_SelectDirOrFileByName(FT_BIN, filename);
	if (nRet != RT_OK)
	{                                
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	FS_ReadFileNodeInfo((UINT8*)&TempNode);    
    if (TempNode.FCP.fControl.FileControl.ReadControl == SECURE_USER_ACCOUNT)
    {
    	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
    	if (nStatus != SAR_OK)
		{                          
            DBG_ERR("User PIN is not login!");
    		SetCmdResult(nStatus, 0);
        	return;
    	}  
    } 

	memcpy(gApduResp.DATA, &TempNode.FileSize, 4);
    memcpy(gApduResp.DATA + 4, &TempNode.FCP.fControl.FileControl.ReadControl, 4);
    memcpy(gApduResp.DATA + 8, &TempNode.FCP.fControl.FileControl.WriteControl, 4);
    
    DBG_FUNC_END; 
	SetCmdResult(SAR_OK, 12);
}
/***************************************************************************
* Subroutine:	CMD_ReadFile()
* Function:		Read file
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ReadFile()
{
	UINT16 offset = 0;
    UINT16 filenamelen = 0;
    UINT16 readsize = 0;
    UINT16 APPID = 0;
    UINT16 nRet = SAR_OK;
    UINT16 nStatus = SAR_OK;  
	tFileNode TempNode = {0};
	INT8 *filename = NULL;	
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;

	if ((gApduBody.LC & 0xFF) < 8)
	{                                 
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	APPID = (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	AppState = GetAppStateInfoById(APPID);
	if (!AppState)
	{                                   
        DBG_ERR("This application id is not exist! APPID = 0x%x", APPID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;

	offset = (gApduBody.DATA[3]<<8) | gApduBody.DATA[2];
	filenamelen = (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	if (filenamelen > MAX_FILENAME_LEN)
	{                       
        DBG_ERR("File name length is wrong! filenamelen = 0x%x", filenamelen);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	filename = (INT8*)(gApduBody.DATA + 6);
	filename[filenamelen] = '\0';
		
	FS_SelectFileByCluster(APPID, APPID);
	nRet = FS_SelectDirOrFileByName(FT_BIN, filename);
	if (nRet != RT_OK)
	{                                    
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	
    FS_ReadFileNodeInfo((UINT8*)&TempNode);  
    switch(TempNode.FCP.fControl.FileControl.ReadControl)  
    {
    	case SECURE_ADM_ACCOUNT:
            nStatus = IsHasControl(SA_SOLOGIN, SA_SOLOGIN, CON_ALL);
    	    if(nStatus != SAR_OK)
			{                      
                DBG_ERR("SO PIN is not login!");
				SetCmdResult(nStatus, 0);
				return;
    	    }
    	    break;
    	case SECURE_USER_ACCOUNT:
            nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
    	    if(nStatus != SAR_OK)
			{            
                DBG_ERR("User PIN is not login!");
				SetCmdResult(nStatus, 0);
				return;
    	    }
    	    break;            
        case SECURE_NEVER_ACCOUNT: 
            DBG_ERR("No any user can do this!");
            SetCmdResult(SAR_UNSUPPORTFUNC, 0);
            return;
        case SECURE_ANYONE_ACCOUNT:  
            DBG_ERR("Any user can do this!");
            break;
    }    
	if (offset > TempNode.FileSize)
	{
        DBG_ERR("File offset is wrong! offset = 0x%x", offset);
		SetCmdResult(SAR_FILEOFFSETERR,0);
		return;
	}
	if ((offset+gApduBody.LE) > TempNode.FileSize)
	{
		readsize = TempNode.FileSize - offset;
	}
	else
	{
		readsize = gApduBody.LE;
	}
	if (readsize > SD_CMDBUF_LEN)
	{	                            
        DBG_ERR("Read size is wrong! readsize = 0x%x", readsize);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	FS_ReadFile(gApduResp.DATA, offset, readsize);
	
    DBG_FUNC_END;
	SetCmdResult(SAR_OK, readsize);
}
/***************************************************************************
* Subroutine:	CMD_WriteFile()
* Function:		Write file
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_WriteFile()
{
	UINT16 appid = 0;
    UINT16 offset = 0;
    UINT16 filenamelen = 0;
    UINT16 datalen = 0;
    UINT16 nRet = SAR_OK;
    UINT16 nStatus = SAR_OK;
	UINT8 *data = NULL;
	UINT8 tempbuf[MAX_FILENAME_LEN + 1] = {0};
	tFileNode TempNode = {0};
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;

	if ((gApduBody.LC & 0xFF) < 8)
	{                               
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{                        
        DBG_ERR("This application id is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
	
	offset = (gApduBody.DATA[3]<<8) | gApduBody.DATA[2];
	filenamelen = (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	datalen = (gApduBody.DATA[7+filenamelen] << 8) | gApduBody.DATA[6+filenamelen];
	if (filenamelen > MAX_FILENAME_LEN)
	{                                
        DBG_ERR("File name length is wrong! filenamelen = 0x%x", filenamelen);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	memcpy(tempbuf, gApduBody.DATA+6, filenamelen);
	data = gApduBody.DATA + 8 + filenamelen;
	tempbuf[filenamelen] = '\0';

	FS_SelectFileByCluster(appid, appid); 
	nRet = FS_SelectDirOrFileByName(FT_BIN, (INT8*)tempbuf);
	if (nRet != RT_OK)
	{                          
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	FS_ReadFileNodeInfo((UINT8*)&TempNode);        
    switch(TempNode.FCP.fControl.FileControl.WriteControl)  
    {
    	case SECURE_ADM_ACCOUNT:
            nStatus = IsHasControl(SA_SOLOGIN, SA_SOLOGIN, CON_ALL);
    	    if(nStatus != SAR_OK)
			{                   
                DBG_ERR("SO PIN is not login!");
				SetCmdResult(nStatus, 0);
				return;
    	    }
    	    break;
    	case SECURE_USER_ACCOUNT:
            nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
    	    if(nStatus != SAR_OK)
			{                       
                DBG_ERR("User PIN is not login!");
				SetCmdResult(nStatus, 0);
				return;
    	    }
    	    break;            
        case SECURE_NEVER_ACCOUNT: 
            DBG_ERR("No any user can do this!");
            SetCmdResult(SAR_UNSUPPORTFUNC, 0);
            return;           
            DBG_ERR("Any user can do this!");
        case SECURE_ANYONE_ACCOUNT:
            break;
    } 

	if ((offset+datalen) > TempNode.FileSize)
	{                          
        DBG_ERR("Write size is wrong! offset = 0x%x, datalen = 0x%x", offset, datalen);
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	}

	nRet = FS_UpdateFile(data, offset, datalen);
	if (nRet != RT_OK)
	{                                
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_CreateContainer()
* Function:		Create Container
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_CreateContainer()
{
	INT8 *container_name = NULL;
	UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 nStatus = 0;
    UINT16 nRet = SAR_OK;
	tFileNode node = {0};	
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;

	if (gApduBody.LC<3 || gApduBody.LC > (2+MAX_CONTAINERNAME_LEN))
	{                         
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	appid =((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	container_name = (INT8*)(gApduBody.DATA + 2);
	container_name[gApduBody.LC - 2] = '\0';

	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{                                           
        DBG_ERR("This application id is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
    nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
    if (nStatus != SAR_OK)
	{
        DBG_ERR("User PIN is not login!");
		SetCmdResult(nStatus, 0);
		return;
    }
	
	FS_SelectFileByCluster(appid, appid);
		
	memset((UINT8*)&node, 0, sizeof(node));
    node.FileType = FT_DF;
    node.FileSize = 0;
    strcpy(node.FCP.Name, container_name);
	nRet = FS_CreateFile(&node);
	if (nRet != RT_OK)
	{                                 
        DBG_ERR("FS_CreateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

	containerid = gCurrentFileCluster;
	gApduResp.DATA[0] = containerid & 0x00FF;
	gApduResp.DATA[1] = (containerid>>8) & 0x00FF;

	nRet = SaveContainerId(appid, containerid);
	if (SAR_OK != nRet)
	{                         
        DBG_ERR("SaveContainerId() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 2);	
}
/***************************************************************************
* Subroutine:	CMD_OpenContainer()
* Function:		Open Container
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_OpenContainer()
{
	INT8 *container_name = NULL;
	UINT16 nRet = SAR_OK;
    UINT16 appid = 0;
    UINT16 containerid = 0;
    
    DBG_FUNC_BEGIN;	

	if (gApduBody.LC<3 || gApduBody.LC > (2+MAX_CONTAINERNAME_LEN))
	{        
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);	
	container_name = (INT8*)(gApduBody.DATA + 2);
	container_name[gApduBody.LC-2] = '\0';
	
	if (!GetAppStateInfoById(appid))
	{                                         
        DBG_ERR("This application id is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(appid, appid);
	nRet = FS_SelectDirOrFileByName2(FT_DF, container_name);
	if (nRet != RT_OK)
	{                               
        DBG_ERR("FS_SelectDirOrFileByName2() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	containerid = gCurrentDirectoryCluster;
	gApduResp.DATA[0] = containerid & 0x00FF;
	gApduResp.DATA[1] = (containerid>>8) & 0x00FF;
   
	nRet = SaveContainerId(appid, containerid);
	if (SAR_OK != nRet)
	{                        
        DBG_ERR("SaveContainerId() failed! nRet = 0x%x", nRet);
		SetCmdResult(nRet, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 2);	
}
/***************************************************************************
* Subroutine:	CMD_CloseContainer()
* Function:		Close container
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_CloseContainer()
{
	UINT16 appid = 0;
    UINT16 containerid = 0;

    DBG_FUNC_BEGIN;
	if (gApduBody.LC != 4)
	{                   
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	if (!GetAppStateInfoById(appid))
	{                                
        DBG_ERR("This application id is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(appid, appid);

	ClearContainerId(appid, containerid);

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_EnumContainer()
* Function:		Enum container
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_EnumContainer()
{
	UINT16 appid = 0;
    UINT16 rlen = 0;

    DBG_FUNC_BEGIN;

	appid = (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if ((gApduBody.LC & 0xFF) != 2)
	{                 
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	if (!GetAppStateInfoById(appid))
	{                           
        DBG_ERR("This application id is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(appid, appid);

	FS_EnumDir(gApduResp.DATA, &rlen);

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, rlen);
}
/***************************************************************************
* Subroutine:	CMD_DeleteContainerByName()
* Function:		Delete Container by name
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DeleteContainerByName(INT8 *name)
{
    UINT16 nRet = SAR_OK;
	UINT16 rlen = 0;
    UINT16 templen = 0;
	UINT8 tempbuf[512] = {0};

    DBG_FUNC_BEGIN;	

    DBG_INFO("%s", name);
    nRet = FS_SelectDirOrFileByName(FT_DF, name);
	if (nRet != RT_OK)
	{              
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
		SetCmdResult(nRet, 0);
		return;
	}

	nRet = FS_SelectFileByID(EXCERTID);
	if (nRet == RT_OK)
	{                     
        DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		FS_DeleteFile();
	}
	nRet = FS_SelectFileByID(SIGNCERTID);
	if (nRet == RT_OK)
	{
		FS_DeleteFile();
	}
	nRet = FS_SelectFileByID(EXKEYID);
	if (nRet == RT_OK)
	{
		nRet = FS_DeleteFile();
	}
	nRet = FS_SelectFileByID(SIGNKEYID);
	if (nRet == RT_OK)
	{
		FS_DeleteFile();
	}
	FS_EnumFile(FT_KEY, tempbuf, &rlen);        
	templen = 0;
	while (rlen > templen)
	{
		if (tempbuf[templen] == 0) break;
		nRet = FS_SelectDirOrFileByName(FT_KEY, (INT8*)(tempbuf + templen));
		if (RT_OK == nRet)
		{
			FS_DeleteFile();
		}
		templen += (strlen((INT8 *)(tempbuf + templen)) + 1);
	}
	FS_EnumFile(FT_BIN, tempbuf, &rlen);        
	templen = 0;
	while (rlen > templen)
	{
		if (tempbuf[templen] == 0) break;
		nRet = FS_SelectDirOrFileByName(FT_BIN, (INT8*)(tempbuf + templen));
		if (RT_OK == nRet)
		{
			FS_DeleteFile();
		}
		templen += (strlen((INT8 *)(tempbuf + templen)) + 1);
	}
	nRet = FS_SelectDirOrFileByName(FT_DF, name);
	if (nRet == RT_OK)
	{
		nRet = FS_DeleteFile();
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_DeleteContainer()
* Function:		Delete Container
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DeleteContainer()
{	
	UINT16 appid = 0;
    UINT16 nStatus = 0;
	INT8 *containername = NULL;
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;

	if (gApduBody.LC < 3 || gApduBody.LC > (2 + MAX_CONTAINERNAME_LEN))
	{                                      
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{            
        DBG_ERR("This application id is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
	{   
        DBG_ERR("User PIN is not login!");
		SetCmdResult(nStatus, 0);
    	return;
	}
	FS_SelectFileByCluster(appid, appid);

	containername = (INT8*)(gApduBody.DATA + 2);
	containername[gApduBody.LC - 2] = '\0';

    DBG_FUNC_END;
	CMD_DeleteContainerByName(containername);
}
/***************************************************************************
* Subroutine:	CMD_GetContainerInfo()
* Function:		Get container information by container name
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GetContainerInfo()
{
	UINT16 nRet = SAR_OK;
    UINT16 AppID = 0;
	UINT32 nKeyBits = 0;
	tFileNode Node = {0};
	UINT8* Container_Name = NULL;

    DBG_FUNC_BEGIN;	

	if (gApduBody.LC < 3 || gApduBody.LC > (2 + MAX_CONTAINERNAME_LEN))
	{                                       
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	AppID = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	if (!GetAppStateInfoById(AppID))
	{                                           
        DBG_ERR("This application id is not exist! AppID = 0x%x", AppID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(AppID, AppID);
	
	//Find the container								
	Container_Name = gApduBody.DATA + 2;
	gApduBody.DATA[gApduBody.LC] = '\0';
	nRet = FS_SelectDirOrFileByName(FT_DF, (INT8*)Container_Name);
	if (nRet != RT_OK)
	{                                 
        DBG_ERR("FS_SelectDirOrFileByName() failed! nRet = 0x%x", nRet);
	    SetCmdResult(SAR_FILE_NOT_EXIST, 0);
	    return;
	}

	//Get container type
	nRet = FS_ReadDirNodeInfo((UINT8*)&Node);          
	if (nRet == RT_OK)
	{
		gApduResp.DATA[0] = Node.ContainerType;
	}	
	else
	{			 
		gApduResp.DATA[0] = 0;
	}

	//Get sign key file information
	nKeyBits = 0;
	nRet = FS_SelectFileByID(SIGNKEYID);
	nRet |= FS_ReadFileNodeInfo((UINT8*)&Node);
	if (RT_OK == nRet)
	{
		if (gApduResp.DATA[0] == RSA_CON)
		{
			nKeyBits = 8 * ((Node.FileSize-6) / 4.5);	
		}
		else if (gApduResp.DATA[0] == ECC_CON)
		{
			nKeyBits = 8 * (Node.FileSize / 3);
		}
	}
	memcpy(gApduResp.DATA + 1, &nKeyBits, 4);

	//Get exchg key file information
	nKeyBits = 0;
	nRet = FS_SelectFileByID(EXKEYID);
	nRet |= FS_ReadFileNodeInfo((UINT8*)&Node);
	if (RT_OK == nRet)
	{ 
		if (gApduResp.DATA[0] == RSA_CON)
		{					 
			nKeyBits = 8 * ((Node.FileSize-6) / 4.5);	
		}
		else if (gApduBody.DATA[0] == ECC_CON)
		{	
			nKeyBits = 8 * (Node.FileSize / 3);
		}
	}
	memcpy(gApduResp.DATA + 5, &nKeyBits, 4);

	//Get sign cert file information
	nRet = FS_SelectFileByID(SIGNKEYID); 
	if (RT_OK == nRet)
	{	 
		gApduResp.DATA[9] = 1;
	}
	else
	{
		gApduResp.DATA[9] = 0;
	}

	//Get exchg cert file information
	nRet = FS_SelectFileByID(EXCERTID); 
	if (RT_OK == nRet)
	{  
		gApduResp.DATA[10] = 1;
	}
	else
	{
		gApduResp.DATA[10] = 0;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 11);
}
/***************************************************************************
* Subroutine:	CMD_GetCONType()
* Function:		Get container type(RSA or ECC container)
* Author:   Syno Comon	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GetCONType()
{                     
	UINT16 nRet = SAR_OK;
	UINT16 ConID = 0;
	tFileNode Node = {0};

    DBG_FUNC_BEGIN;	

	ConID = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);

	nRet = FS_SelectFileByCluster(ConID, ConID);
	if (RT_OK != nRet)
    {                                        
        DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}

	nRet = FS_ReadDirNodeInfo((UINT8*)&Node);          
	if (RT_OK != nRet)
	{                                 
	    gApduResp.DATA[0] = 0;     
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
        DBG_ERR("FS_ReadDirNodeInfo() failed! nRet = 0x%x", nRet);   
	    return;
	}
              
	gApduResp.DATA[0] = Node.ContainerType;

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 1);
}
/***************************************************************************
* Subroutine:	CMD_ImportCert()
* Function:		Import cert to container
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ImportCert()
{
	UINT16 nRet = SAR_OK;
    UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 certtype = 0;
    UINT16 certlen = 0;
	UINT8 *cert = NULL;
	tFileNode Node = {0};

    DBG_FUNC_BEGIN;

	if (gApduBody.LC < 9 || gApduBody.LC > (9 + 2048))
	{                        
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	certtype = gApduBody.DATA[4];
	certlen = (gApduBody.DATA[8]<<24) | (gApduBody.DATA[7]<<16) | (gApduBody.DATA[6]<<8) | gApduBody.DATA[5];
	cert = gApduBody.DATA + 9;

	if (!GetAppStateInfoById(appid))
	{                                      
        DBG_ERR("This application id is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}         
	FS_SelectFileByCluster(appid, appid);
	nRet = IsContainerIdValid(appid, containerid);
	if (nRet != SAR_OK)
	{                                  
        DBG_ERR("This container id is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	nRet = FS_SelectFileByCluster(containerid, containerid);
	if (nRet != RT_OK)
	{                                 
        DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	
	memset((UINT8*)&Node, 0, sizeof(Node));
    Node.FileType = FT_CERT;
    Node.FileSize = certlen;	
    if (certtype == 0x00)
	{						  
		nRet = FS_SelectFileByID(EXKEYID);
		if (nRet != RT_OK)
		{			           
            DBG_ERR("FS_SelectFileByID(EXKEYID) failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
			return;
		}
		Node.FCP.FileID = EXCERTID;
		strcpy(Node.FCP.Name, "ExchgCert");		
	}
	else if (certtype == 0x01)
	{
		nRet = FS_SelectFileByID(SIGNKEYID);
		if (nRet != RT_OK)
		{			                      
            DBG_ERR("FS_SelectFileByID(EXKEYID) failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
			return;
		}
		Node.FCP.FileID = SIGNCERTID;
		strcpy(Node.FCP.Name, "SignCert");		
	}
	nRet = FS_SelectFileByID(Node.FCP.FileID);
	if (nRet == RT_OK)
	{
		nRet = FS_DeleteFile();
		if (nRet != RT_OK)
		{                         
            DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0);
			return;
		}
	}

	nRet = FS_CreateFile(&Node);
	if (nRet != RT_OK)
	{                        
        DBG_ERR("FS_CreateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}
	
	nRet = FS_UpdateFile(cert, 0, certlen);
	if (nRet != RT_OK)
	{                            
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}
    
    DBG_FUNC_END;	
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_ExportCert()
* Function:		Export cert from container
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExportCert()
{              
	UINT16 nRet = SAR_OK;
    UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT32 certlen = 0;
	UINT8 *cert = NULL;
	tFileNode Node = {0};
	                    
    DBG_FUNC_BEGIN;

    if ((gApduBody.LC & 0xff) != 4)			//change @ 21050831 by comon, as the pc send 3 byte lc length 	  (gApduBody.LC != 4) || (
	{                        
        DBG_ERR("Lc is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);

	if (!GetAppStateInfoById(appid))
	{                                
        DBG_ERR("This application id is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}                       
	FS_SelectFileByCluster(appid, appid);
	nRet = IsContainerIdValid(appid, containerid);
	if (nRet != SAR_OK)
	{                                          
        DBG_ERR("This container id is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	nRet = FS_SelectFileByCluster(containerid, containerid);
	if (nRet != RT_OK)
	{                                      
        DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	
	if (gApduHeader.P1 == 0x00)
	{
		nRet = FS_SelectFileByID(EXCERTID);
	}
	else if (gApduHeader.P1 == 0x01)
	{
		nRet = FS_SelectFileByID(SIGNCERTID);
	}
	else
	{		                  
        DBG_ERR("P1 is wrong! gApduHeader.P1 = 0x%x", gApduHeader.P1);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
	}
	if (nRet != RT_OK)
	{                          
        DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_CERTNOTFOUNTERR, 0);
		return;
	}
	FS_ReadFileNodeInfo((UINT8*)&Node);        
	cert = gApduResp.DATA + 4;
	certlen = Node.FileSize;
	nRet = FS_ReadFile(cert, 0, certlen);
	if (nRet != RT_OK)
	{                          
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}
	gApduResp.DATA[0] = certlen & 0x000000FF;
	gApduResp.DATA[1] = (certlen>>8) & 0x000000FF;
	gApduResp.DATA[2] = (certlen>>16) & 0x000000FF;
	gApduResp.DATA[3] = (certlen>>24) & 0x000000FF;

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, certlen + 4);
}
/***************************************************************************
* Subroutine:	CMD_GenRandom()
* Function:		Generate rand number
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GenRandom()
{
    DBG_FUNC_BEGIN;

	memset(gRandData, 0, 16);

	Trng_GenRandom(gApduResp.DATA, gApduBody.LE);
	memcpy(gRandData, gApduResp.DATA, 16);
    
    DBG_FUNC_END; 
	SetCmdResult(SAR_OK, gApduBody.LE);   
}
#ifdef RSASUPPORT
/***************************************************************************
* Subroutine:	CMD_GenExtRSAkey()
* Function:		Generate extern rsa key pair
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GenExtRSAkey()
{
	UINT16 nByteLen = 0;
    UINT16 nPrimeLen = 0;
    UINT16 index = 0;
    UINT16 keybitlen = 0;
	UINT16 nRet = SAR_OK;
	R_RSA_PROTO_KEY ProtoKey = {0};
    R_RSA_KEYPAIR RSAKey = {0};   
 	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], e[4], d[256], p[128], q[128], dp[128], dq[128], qinv[128];	

    DBG_FUNC_BEGIN;
     	
	if ((gApduBody.LC & 0xff) != 2)		//hange @ 21050831 by comon, as the pc send 3 byte lc length 	(gApduBody.LC != 2) || (		  )
	{
        DBG_ERR("LC is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	keybitlen = (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if (keybitlen != 1024 && keybitlen != 512 && keybitlen != 2048)
    {                                  
        DBG_ERR("Key bits is wrong! keybitlen = 0x%x", keybitlen);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	
	ProtoKey.bits = keybitlen;
	ProtoKey.PublicExponent = 65537; 
	RSAKey.modulus = n;
	RSAKey.publicExponent =	e;
	RSAKey.exponent = d;
	RSAKey.prime[0] = p;
	RSAKey.prime[1] = q;
	RSAKey.primeExponent[0] = dp;
	RSAKey.primeExponent[1] = dq;
	RSAKey.coefficient = qinv;		       
    nRet = RSA_GenKeyPair(ProtoKey, &RSAKey);     
    if (nRet != RT_OK)
    {                          
        DBG_ERR("RSA_GenKeyPair() failed! nRet = 0x%x", nRet);
    	SetCmdResult(SAR_GENRSAKEYERR, 0);
    	return;
    } 
    
    nByteLen = ProtoKey.bits / 8;
    nPrimeLen = ProtoKey.bits / 16;
	//CHANGE @ 0720 MAX_RSA_MODULUS_LEN
	index = 0;
    memcpy(gApduResp.DATA, RSAKey.modulus, nByteLen);
	index += nByteLen;
	memcpy(gApduResp.DATA + index, RSAKey.publicExponent, 4);
	index += 4;
	memcpy(gApduResp.DATA + index, RSAKey.exponent, nByteLen);
	index += nByteLen;
    memcpy(gApduResp.DATA + index, RSAKey.prime[0], nPrimeLen);
	index += nPrimeLen;
    memcpy(gApduResp.DATA + index, RSAKey.prime[1], nPrimeLen);
	index += nPrimeLen;
    memcpy(gApduResp.DATA + index, RSAKey.primeExponent[0], nPrimeLen);
	index += nPrimeLen;
	memcpy(gApduResp.DATA + index, RSAKey.primeExponent[1], nPrimeLen); 
	index += nPrimeLen;
	memcpy(gApduResp.DATA + index, RSAKey.coefficient, nPrimeLen);
	index += nPrimeLen;

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, index);
}
/***************************************************************************
* Subroutine:	CMD_GenRSAKeyPair()
* Function:		Generate rsa key pair
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GenRSAKeyPair()
{
	UINT16 nRet = SAR_OK;
    UINT16 nStatus = 0;
    UINT16 appid = 0;
    UINT16 containerid = 0;
	UINT16 nByteLen = 0;
    UINT16 nPrimeLen = 0;
    UINT16 index = 0;
    UINT16 keybitlen = 0;         
	tFileNode Node = {0};
	R_RSA_PROTO_KEY ProtoKey = {0};
	R_RSA_KEYPAIR RSAKey = {0};
	APPSTATE* AppState = NULL;
	 //以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], e[4], d[256], p[128], q[128], dp[128], dq[128], qinv[128]; 

    DBG_FUNC_BEGIN;

    //检查数据长度
	if ((gApduBody.LC & 0xFF) != 6)
	{                             
        DBG_ERR("LC is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
    //检查Key bits
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	keybitlen = (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	if (keybitlen != 1024 && keybitlen != 512 && keybitlen != 2048)
	{                                               
        DBG_ERR("Key bits is wrong! keybitlen = 0x%x", keybitlen);
		SetCmdResult(SAR_INDATAERR, 0) ;
		return;
	}
    //检查应用状态
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{                                  
        DBG_ERR("This application ID is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
    //检查安全状态
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
	{                            
        DBG_ERR("The user is not logined!");
		SetCmdResult(nStatus, 0);
    	return;
	}
    //检查容器ID	
	if (SAR_OK != IsContainerIdValid(appid, containerid))
	{                                     
        DBG_ERR("This container ID is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	} 
	FS_SelectFileByCluster(containerid, containerid);
	
	//Generate RSA key pair	
	ProtoKey.bits = keybitlen;
	ProtoKey.PublicExponent = 65537;
	RSAKey.modulus = n;
	RSAKey.publicExponent =	e;
	RSAKey.exponent = d;
	RSAKey.prime[0] = p;
	RSAKey.prime[1] = q;
	RSAKey.primeExponent[0] = dp;
	RSAKey.primeExponent[1] = dq;
	RSAKey.coefficient = qinv;	
	__disable_irq();		
	nRet = RSA_GenKeyPair(ProtoKey, &RSAKey);
	__enable_irq();	
	if (nRet != 0)
	{                                        
		DBG_ERR("RSA_GenKeyPair() failed! nRet = 0x%x,bits = 0x%x", nRet, keybitlen);
		SetCmdResult(SAR_GENRSAKEYERR, 0);
		return;
	}	
	nByteLen = ProtoKey.bits / 8;
	nPrimeLen = ProtoKey.bits / 16;
	
	//Write RSA key pair data to file
	memset((UINT8*)&Node, 0, sizeof(Node));
	Node.FileType = FT_KEY;
	Node.FileSize = nByteLen * 2 + nPrimeLen * 5 + 6;
	Node.FCP.FileID = SIGNKEYID;
	strcpy(Node.FCP.Name, "SignRSAKey");		
	nRet = FS_SelectFileByID(SIGNKEYID);
	if (nRet == RT_OK)
	{
		nRet = FS_DeleteFile();
		if (nRet != RT_OK)
		{                                      
			DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0); 
			return;
		}
	}	
	nRet = FS_CreateFile(&Node);
	if (nRet != RT_OK)
	{                                    
		DBG_ERR("FS_CreateFile() failed! nRet = 0x%x, Node.FileSize = 0x%x", nRet, Node.FileSize);
		SetCmdResult(SAR_WRITEFILEERR, 0); 
		return;
	}
	index = 0;
	memcpy(gApduBody.DATA, (UINT8*)&RSAKey.bits, 2);
	index += 2;
	memcpy(gApduBody.DATA + index, RSAKey.modulus, nByteLen);
	index += nByteLen;
	memcpy(gApduBody.DATA + index, RSAKey.publicExponent, 4);
	index += 4;
	memcpy(gApduBody.DATA + index, RSAKey.exponent, nByteLen);
	index += nByteLen;
    memcpy(gApduBody.DATA + index, RSAKey.prime[0], nPrimeLen);
	index += nPrimeLen;
    memcpy(gApduBody.DATA + index, RSAKey.prime[1], nPrimeLen);
	index += nPrimeLen;
    memcpy(gApduBody.DATA + index, RSAKey.primeExponent[0], nPrimeLen);
	index += nPrimeLen;
	memcpy(gApduBody.DATA + index, RSAKey.primeExponent[1], nPrimeLen); 
	index += nPrimeLen;
	memcpy(gApduBody.DATA + index, RSAKey.coefficient, nPrimeLen);
	index += nPrimeLen;
	nRet = FS_UpdateFile((UINT8 *)gApduBody.DATA, 0, index);
	if (nRet != RT_OK)
	{                             
		DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0); 
		return;
	}
	
	//Set this container type 													 
	FS_SelectFileByCluster(containerid, containerid);
	memset((UINT8*)&Node, 0, sizeof(tFileNode));
	FS_ReadDirNodeInfo((UINT8*)&Node);
	Node.ContainerType = RSA_CON;
	FS_WriteDirNodeInfo((UINT8*)&Node);
	if (nRet != RT_OK)
	{                                     
		DBG_ERR("FS_WriteDirNodeInfo() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}
	
	//Ouput RSA public key
	index = 0;
	memcpy(gApduResp.DATA, RSAKey.modulus, nByteLen);
	index += nByteLen;
	memcpy(gApduResp.DATA + index, RSAKey.publicExponent, 4);
	index += 4;

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, index);
}
/***************************************************************************
* Subroutine:	CMD_ImportRSAKey()
* Function:		Import rsa key which be plain mode
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ImportRSAKey()
{
    UINT16 AppID = 0;
    UINT16 ConID = 0;
    UINT16 nByteLen = 0;
    UINT16 nPrimeLen = 0;
    UINT16 keybitlen = 0; 
    UINT16 nRet = 0;
	UINT16 nStatus = SAR_OK;
    tFileNode Node = {0};
	APPSTATE* AppState = NULL;
   
    DBG_FUNC_BEGIN;

    memcpy(&AppID, gApduBody.DATA, 2);
    memcpy(&ConID, gApduBody.DATA + 2, 2);
	keybitlen = (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	nByteLen = keybitlen / 8;
	nPrimeLen = keybitlen / 16;
	
    //检查Bit
    if (keybitlen != 1024 && keybitlen != 512 && keybitlen != 2048)
    {
        DBG_ERR("Key bits is wrong! bits = 0x%x", keybitlen);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}
    //检查数据长度
	if ((nByteLen*2 + nPrimeLen*5 + 6) != (gApduBody.LC - 4))
    {                                         
        DBG_ERR("Data length is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
    //检查应用状态
	AppState = GetAppStateInfoById(AppID);
	if(!AppState)
    {                          
        DBG_ERR("This application ID is not exist! appid = 0x%x", AppID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
    //检查容器ID
	if (SAR_OK != IsContainerIdValid(AppID, ConID))
    {                                              
        DBG_ERR("This container ID is not exist! containerid = 0x%x", ConID);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(ConID, ConID);	

    //是签名密钥对
	if(gApduHeader.P1 == TRUE)
	{
		memset((UINT8*)&Node, 0, sizeof(Node));
		Node.FileType = FT_KEY;
		Node.FileSize = gApduBody.LC - 4;
		Node.FCP.FileID = SIGNKEYID;
		strcpy(Node.FCP.Name, "SignRSAKey");
		
		nRet = FS_SelectFileByID(SIGNKEYID);
		if (nRet == RT_OK)
		{
			nRet = FS_DeleteFile();
			if (nRet != RT_OK)
			{                        
                DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
				SetCmdResult(SAR_WRITEFILEERR, 0);
				return;
			}
		}
	}
    //是加密密钥对
	else if(gApduHeader.P1 == FALSE)
	{
		memset((UINT8*)&Node, 0, sizeof(Node));
		Node.FileType = FT_KEY;
		Node.FileSize = gApduBody.LC - 4;
		Node.FCP.FileID = EXKEYID;
		strcpy(Node.FCP.Name, "ExtRSAKey");
		
		nRet = FS_SelectFileByID(EXKEYID);
		if (nRet == RT_OK)
		{
			nRet = FS_DeleteFile();
			if (nRet != RT_OK)
			{                                  
                DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
				SetCmdResult(SAR_WRITEFILEERR, 0);
				return;
			}
		}
	}
    //创建密钥文件
	nRet = FS_CreateFile(&Node);
	if (nRet != RT_OK)
	{  
        DBG_ERR("FS_CreateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}
    //写密钥数据
	nRet = FS_UpdateFile(gApduBody.DATA + 4, 0, gApduBody.LC - 4);
	if (nRet != RT_OK)
	{                                     
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}
	//修改容器类型												 
	FS_SelectFileByCluster(ConID, ConID);
	memset((UINT8*)&Node, 0, sizeof(tFileNode));
	FS_ReadDirNodeInfo((UINT8*)&Node);
	Node.ContainerType = RSA_CON;
	FS_WriteDirNodeInfo((UINT8*)&Node);
	if (nRet != RT_OK)
	{	                                  
        DBG_ERR("FS_WriteDirNodeInfo() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}	 

    DBG_FUNC_END;

	SetCmdResult(SAR_OK, 0);            
}
/***************************************************************************
* Subroutine:	CMD_ImportRSAKeyPair()
* Function:		Import rsa key pair which be encrypted
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ImportRSAKeyPair()
{
	UINT16 nRet = SAR_OK; 
	UINT16 nStatus = SAR_OK;
    UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 keybitlen = 0;
    UINT16 nByteLen = 0;
    UINT16 nPrimeLen = 0;
    UINT16 index = 0;
	UINT16 tempindex = 0;   
	UINT32 keylen = 0; 
	ULONG symmAlgID = 0;
    ULONG encypherprvkeylen = 0;
    ULONG sessionkeylen = 0;
	UINT8 *encypherprvkey = NULL;
	UINT8 *sessionkey = NULL; 
	UINT8* RSAKeydata = NULL;   
	tFileNode Node = {0};
	UINT8* key = (UINT8*)&Node;
    R_RSA_KEYPAIR RSAKey = {0};  
	APPSTATE* AppState = NULL;
	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], p[128], q[128], dp[128], dq[128], qinv[128]; 

    DBG_FUNC_BEGIN;

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
    //对称密钥算法ID
	symmAlgID = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
    //加密后的对称密钥长度
	sessionkeylen =(gApduBody.DATA[11]<<24) | (gApduBody.DATA[10]<<16) | (gApduBody.DATA[9]<<8) | gApduBody.DATA[8];
    //加密后的对称密钥起始地址
	sessionkey = gApduBody.DATA + 12;
    //RSA密钥bits
	tempindex = 12 + sessionkeylen;
    keybitlen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
    //加密后的密钥对长度
	encypherprvkeylen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
    //加密后的私钥起始地址
	encypherprvkey = gApduBody.DATA + tempindex;
	tempindex += encypherprvkeylen;

    //检查数据长度
	if (tempindex != gApduBody.LC)
	{                                  
        DBG_ERR("Data length is wrong! Data length = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
    //检查应用状态
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{                        
        DBG_ERR("This application ID is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
    //检查安全状态
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
	{                               
        DBG_ERR("The user is not logined!");
		SetCmdResult(nStatus, 0);
    	return;
	}
    //检查容器ID	
	if (SAR_OK != IsContainerIdValid(appid, containerid))
	{                         
        DBG_ERR("This container ID is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
    //读取签名私钥
	FS_SelectFileByCluster(containerid, containerid);
	nRet = FS_SelectFileByID(SIGNKEYID);
	if (nRet != RT_OK)
	{                        
        DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
		return;		
	}
	RSAKeydata = gApduResp.DATA;
	nRet = FS_ReadFile(RSAKeydata, 0, 2);
	keybitlen = (RSAKeydata[1]<<8) | RSAKeydata[0];
	nByteLen = keybitlen/8;
	nPrimeLen = keybitlen/16;
	nRet |= FS_ReadFile(RSAKeydata, 0, nByteLen*2 + nPrimeLen*5 + 6);
	if (nRet != RT_OK)
	{                                 
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
		
	}
	memset(key, 0, 32);
	RSAKey.modulus = n;
	RSAKey.publicExponent =	NULL;
	RSAKey.exponent = NULL;
	RSAKey.prime[0] = p;
	RSAKey.prime[1] = q;
	RSAKey.primeExponent[0] = dp;
	RSAKey.primeExponent[1] = dq;
	RSAKey.coefficient = qinv;
	index = 0;
	memcpy((UINT8*)&RSAKey.bits, RSAKeydata, 2);
	index += 2;
	memcpy(RSAKey.modulus, RSAKeydata + index, nByteLen);
	index += nByteLen;
	//memcpy(RSAKey.publicExponent, RSAKeydata + index, 4);
	index += 4;
	//memcpy(RSAKey.exponent,RSAKeydata+index,nByteLen);
	index += nByteLen;
    memcpy(RSAKey.prime[0], RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
    memcpy(RSAKey.prime[1], RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
    memcpy(RSAKey.primeExponent[0], RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
	memcpy(RSAKey.primeExponent[1], RSAKeydata + index, nPrimeLen); 
	index += nPrimeLen;
	memcpy(RSAKey.coefficient, RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
	
    //解密会话密钥
    nRet = RSA_PrivateDecrypt(key, &keylen, sessionkey, sessionkeylen, &RSAKey);
	if (nRet != RT_OK)
	{                                      
        DBG_ERR("RSA_PrivateDecrypt() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_DEC_FAIL, 0);
		return;
	}
	
    //使用会话密钥解密密钥对
	switch(symmAlgID)
	{
		case SGD_SM1_ECB:
			 SM1_Init(key, key + 16);
			 SM1_Run(SM1_DECRYPT, SM1_ECB, encypherprvkey, encypherprvkey, encypherprvkeylen, NULL);
			 break;
		case SGD_SMS4_ECB:
			 SMS4_Init(key);
			 SMS4_Run(SMS4_DECRYPT, SMS4_ECB, encypherprvkey, encypherprvkey, encypherprvkeylen, NULL);
			 break;
		case SGD_AES128_ECB:
			 Aes_Init(key, 16);
			 Aes_Run(SMS4_DECRYPT, AES128_ECB, encypherprvkey, encypherprvkey, encypherprvkeylen, NULL);
			 break; 
		case SGD_AES192_ECB:
			 Aes_Init(key, 24);
			 Aes_Run(SMS4_DECRYPT, AES192_ECB, encypherprvkey, encypherprvkey, encypherprvkeylen, NULL);
			 break;
		case SGD_AES256_ECB:
			 Aes_Init(key, 32);
			 Aes_Run(SMS4_DECRYPT, AES256_ECB, encypherprvkey, encypherprvkey, encypherprvkeylen, NULL);
			 break;
		default:    
             DBG_ERR("The session key alg is not supported! symmAlgID = 0x%x", symmAlgID);
			 SetCmdResult(SAR_UNSUPPORT_SESSION_KEY, 0);
			 return;
	}
	keybitlen = (encypherprvkey[1]<<8) | encypherprvkey[0];
	nByteLen = keybitlen / 8;
	nPrimeLen = keybitlen / 16;

    //删除老的加密密钥对文件
	FS_SelectFileByCluster(containerid, containerid);
	memset((UINT8*)&Node, 0, sizeof(Node));
	Node.FileType = FT_KEY;
	Node.FileSize = nByteLen*2 + nPrimeLen*5 + 6;
	Node.FCP.FileID = EXKEYID;
	strcpy(Node.FCP.Name, "ExtRSAKey");	
	nRet = FS_SelectFileByID(EXKEYID);
	if (nRet == RT_OK)
	{
		nRet = FS_DeleteFile();
		if (nRet != RT_OK)
		{                                  
            DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0);
			return;
		}
	}
    
    //创建新的加密密钥对文件    
    nRet = FS_CreateFile(&Node);
	if (nRet != RT_OK)
	{                              
        DBG_ERR("FS_CreateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR,0);
		return;
	}
    //写数据
	nRet = FS_UpdateFile(encypherprvkey, 0, nByteLen*2 + nPrimeLen*5 + 6);
	if (nRet != RT_OK)
	{                                    
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_RSASignData()
* Function:		Signature by rsa prv key
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_RSASignData()
{
	UINT16 nRet = SAR_OK;
	UINT16 nStatus = SAR_OK;
    UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 datalen = 0;
    UINT16 keybitlen = 0;
    UINT16 nByteLen = 0;
    UINT16 nPrimeLen = 0;
    UINT16 index = 0;
    UINT32 hashdatalen = 0;
    UINT32 signaturelen = 0;
	UINT8 HashData[32] = {0}; 
	UINT8 val[256] = {0};
	UINT8* data = NULL;
	UINT8* RSAKeydata = NULL;
    R_RSA_KEYPAIR RSAKey = {0};  
	APPSTATE* AppState = NULL;	
	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], p[128], q[128], dp[128], dq[128], qinv[128]; 

    DBG_FUNC_BEGIN;

    //检查数据长度
	if (gApduBody.LC <= 4)
	{                                
        DBG_ERR("The data length is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	datalen = gApduBody.LC - 4;
	data = gApduBody.DATA + 4;

    //检查应用状态
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{                                   
        DBG_ERR("This application ID is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
    //检查安全状态
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
	{                                    
        DBG_ERR("The user is not logined!");
		SetCmdResult(nStatus, 0);
    	return;
	}
    //检查容器ID	
	if (SAR_OK != IsContainerIdValid(appid, containerid))
	{                                          
        DBG_ERR("This container ID is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}

	FS_SelectFileByCluster(containerid, containerid);

    //读取签名私钥
	nRet = FS_SelectFileByID(SIGNKEYID);
	if (nRet != RT_OK)
	{                              
        DBG_ERR("FS_SelectFileByID(SIGNKEYID) failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
		return;
	}
	RSAKeydata = data + datalen + (4 - datalen % 4);	  //四字节对齐
	nRet = FS_ReadFile(RSAKeydata, 0, 2);
	keybitlen = (RSAKeydata[1]<<8) | RSAKeydata[0];
	nByteLen = keybitlen / 8;
	nPrimeLen = keybitlen / 16;
	nRet |= FS_ReadFile(RSAKeydata, 0, nByteLen*2 +  nPrimeLen*5 + 6);
	if (nRet != RT_OK)
	{                             
        DBG_ERR("FS_ReadFile failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_INDATAERR,0);
		return;
	}
	RSAKey.modulus = n;
	RSAKey.publicExponent =	NULL;
	RSAKey.exponent = NULL;
	RSAKey.prime[0] = p;
	RSAKey.prime[1] = q;
	RSAKey.primeExponent[0] = dp;
	RSAKey.primeExponent[1] = dq;
	RSAKey.coefficient = qinv;
	index = 0;
	memcpy((UINT8*)&RSAKey.bits, RSAKeydata, 2);
	index += 2;
	memcpy(RSAKey.modulus, RSAKeydata + index, nByteLen);
	index += nByteLen;
	//memcpy(RSAKey.publicExponent, RSAKeydata + index, 4);
	index += 4;
	//memcpy(RSAKey.exponent, RSAKeydata + index, nByteLen);
	index += nByteLen;
    memcpy(RSAKey.prime[0], RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
    memcpy(RSAKey.prime[1], RSAKeydata + index,nPrimeLen);
	index += nPrimeLen;
    memcpy(RSAKey.primeExponent[0], RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
	memcpy(RSAKey.primeExponent[1], RSAKeydata + index, nPrimeLen); 
	index += nPrimeLen;
	memcpy(RSAKey.coefficient, RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;

	memset(HashData, 0, 32);
	if (gApduHeader.P1 == 0x01)		//原文
	{           
        DBG_INFO("Input data is plain text!");
		switch(gApduHeader.P2)
		{
		case 0x01:				//使用SM3杂凑
            SM3_Init(&gSm3Context);	
			SM3_Update(&gSm3Context, data, datalen);
            SM3_Final(&gSm3Context, HashData);
            hashdatalen = 32;
			break;
		case 0x02:				//使用SHA1杂凑
			SHA1_Init(&gSHA1Context);
			SHA1_Update(&gSHA1Context, data, datalen);
			SHA1_Final(&gSHA1Context, HashData);
			hashdatalen = 20;
			break;
		case 0x03:				//使用SHA256杂凑
			{
			   SHA256_CONTEXT ctx;
			   SHA256_Init(&ctx); 
			   SHA256_Update(&ctx, data, datalen);
			   SHA256_Final(&ctx, HashData);				
			}
            hashdatalen = 32;
			break;
		default:  
            DBG_ERR("Hash alg is not supported! gApduHeader.P2 = 0x%x", gApduHeader.P2);
			SetCmdResult(SAR_UNSUPPORT_HASH_ALG, 0);
			return;
		}
	}
	else if (gApduHeader.P1 == 0x02)	//摘要数据		
	{               
        DBG_INFO("Input data is hash data!");
		if (datalen > 32)
		{	                 
            DBG_ERR("Hash data lenght is wrong! datalen = 0x%x", datalen);
			SetCmdResult(SAR_INDATALENERR, 0);
			return;
		}
		hashdatalen = datalen;
		memcpy(HashData, data, datalen);
	}
	else
	{	       
        DBG_ERR("P1 is wrong! gApduHeader.P1 = 0x%x", gApduHeader.P1);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
	}

    //签名数据
	__disable_irq();
	nRet = RSA_PrivateEncrypt(val, &signaturelen, HashData, hashdatalen, &RSAKey);
	__enable_irq();
	if (nRet != RT_OK)
	{		                
        DBG_ERR("RSA_PrivateEncrypt() failed! nRet = 0x%x", nRet);
	    SetCmdResult(SAR_ASYMM_ENC_FAIL, 0);
	    return;
    }

	memcpy(gApduResp.DATA, val, signaturelen);

    DBG_FUNC_END;
	SetCmdResult(SAR_OK,signaturelen);
}
/***************************************************************************
* Subroutine:	CMD_RSAVerify()
* Function:		Verify by rsa pub key
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_RSAVerify()
{
    UINT16 nRet = SAR_OK;
    UINT16 nByteLen = 0;
    UINT16 tempindex = 0;
	UINT32 datalen = 0;
    UINT32 hashdatalen = 0;
    UINT32 signatuerlen = 0;
	UINT8 HashData[32] = {0}; 
	UINT8 DecryptedHashData[32] = {0}; 
    UINT8 signature[256] = {0};	
	UINT8 *data = NULL;
    R_RSA_KEYPAIR RSAKey = {0};
	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256],  e[4]; 

    DBG_FUNC_BEGIN;	
                              
    //检查公钥bits
	RSAKey.bits = (gApduBody.DATA[3]<<24) | (gApduBody.DATA[2]<<16) | (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if (RSAKey.bits != 1024 && RSAKey.bits != 2048)
	{                            
        DBG_ERR("The RSA public key bits is wrong! RSAKey.bits = 0x%x", RSAKey.bits);
		SetCmdResult(SAR_INDATALENERR, 0);
		return; 
	}

    //读取公钥数据，原始数据和签名数据
	nByteLen = RSAKey.bits / 8;
	RSAKey.modulus = n;
	RSAKey.publicExponent =	e;
	RSAKey.exponent = NULL;
	RSAKey.prime[0] = NULL;
	RSAKey.prime[1] = NULL;
	RSAKey.primeExponent[0] = NULL;
	RSAKey.primeExponent[1] = NULL;
	RSAKey.coefficient = NULL;
	tempindex = 4;
	memcpy((UINT8*)RSAKey.modulus, gApduBody.DATA + tempindex, nByteLen);
	tempindex += nByteLen;
	memcpy((UINT8*)RSAKey.publicExponent, gApduBody.DATA + tempindex, 4);
	tempindex += 4;
	datalen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
	data = gApduBody.DATA + tempindex;
	tempindex += datalen;
	signatuerlen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16)|(gApduBody.DATA[tempindex+1]<<8)|gApduBody.DATA[tempindex];
	tempindex += 4;
	memcpy(signature, gApduBody.DATA + tempindex, signatuerlen);
	tempindex += signatuerlen;
	if (tempindex != gApduBody.LC)
	{                                     
        DBG_ERR("The data length is wrong! Data length = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	if(signatuerlen != nByteLen)
	{			                 
        DBG_ERR("The signature length is wrong! Signature length = 0x%x", signatuerlen);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}

	memset(HashData, 0, 32);
	if (gApduHeader.P1 == 0x01)		//原文
	{                        
        DBG_INFO("Input data is plain text!");
		switch(gApduHeader.P2)
		{
		case 0x01:				//使用SM3杂凑
            SM3_Init(&gSm3Context);	
			SM3_Update(&gSm3Context, data, datalen);
            SM3_Final(&gSm3Context, HashData);
            hashdatalen = 32;
			break;
		case 0x02:				//使用SHA1杂凑
			SHA1_Init(&gSHA1Context);
			SHA1_Update(&gSHA1Context, data, datalen);
			SHA1_Final(&gSHA1Context, HashData);
			hashdatalen = 20;
			break;
		case 0x03:				//使用SHA256杂凑
			{
			   SHA256_CONTEXT ctx;
			   SHA256_Init(&ctx); 
			   SHA256_Update(&ctx, data, datalen);
			   SHA256_Final(&ctx, HashData);				
			}
            hashdatalen = 32;
			break;
		default:
            DBG_ERR("Hash alg is not supported! gApduHeader.P2 = 0x%x", gApduHeader.P2);
			SetCmdResult(SAR_UNSUPPORT_HASH_ALG, 0);
			return;
		}
	}
	else if (gApduHeader.P1 == 0x02)	//摘要数据		
	{                            
        DBG_INFO("Input data is hash data!");
		if (datalen > 32)
		{	                                  
            DBG_ERR("Hash data lenght is wrong! datalen = 0x%x", datalen);
			SetCmdResult(SAR_INDATALENERR, 0);
			return;
		}
		hashdatalen = datalen;
		memcpy(HashData, data, datalen);
	}
	else
	{	                        
        DBG_ERR("P1 is wrong! gApduHeader.P1 = 0x%x", gApduHeader.P1);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
	}
    
    //签验数据	
	nRet = RSA_PublicDecrypt(DecryptedHashData, &datalen, signature, signatuerlen, (R_RSA_PUBLIC_KEY*)&RSAKey);//RSA_PubKeyOpt
	if (nRet != 0)
	{                                
        DBG_ERR("RSA_PublicDecrypt() failed! nRet = 0x%x", nRet);
	    SetCmdResult(SAR_ASYMM_DEC_FAIL, 0);
	    return;
	}
	if (memcmp(DecryptedHashData, HashData, hashdatalen) != 0)
	{                               
        DBG_ERR("The signature verify failed!");
		SetCmdResult(SAR_SIGNVERYFIERR, 0);
		return;
	}	
	
    DBG_FUNC_END;                          
	SetCmdResult(SAR_OK, 0);   
}
/***************************************************************************
* Subroutine:	CMD_RSAExportSessionKey()
* Function:		Generate and export session key which be encrypted by rsa pub key
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_RSAExportSessionKey()
{
	UINT16 nRet = SAR_OK;
    UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 tempindex = 0;
    UINT16 nByteLen = 0;
    UINT16 SessionKeyID = 0;
	UINT32 AlgID = 0;
    UINT32 keylen = 0;
	UINT8 OutData[256] = {0};
	UINT8 tempbuf[32] = {0}; 
    R_RSA_KEYPAIR RSAKey = {0};	
	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], e[4]; 

    DBG_FUNC_BEGIN;

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	AlgID = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	RSAKey.bits = (gApduBody.DATA[11]<<24) | (gApduBody.DATA[10]<<16) | (gApduBody.DATA[9]<<8) | gApduBody.DATA[8];

    //检查公钥bits
	if (RSAKey.bits != 1024 && RSAKey.bits != 2048)
	{                            
        DBG_ERR("The RSA public key bits is wrong! RSAKey.bits = 0x%x", RSAKey.bits);
		SetCmdResult(SAR_INDATALENERR, 0);
		return; 
	}
    //读取公钥数据
	nByteLen = RSAKey.bits / 8;
	RSAKey.modulus = n;
	RSAKey.publicExponent =	e;
	RSAKey.exponent = NULL;
	RSAKey.prime[0] = NULL;
	RSAKey.prime[1] = NULL;
	RSAKey.primeExponent[0] = NULL;
	RSAKey.primeExponent[1] = NULL;
	RSAKey.coefficient = NULL;
	tempindex = 12;		
	memcpy((UINT8*)RSAKey.modulus, gApduBody.DATA + tempindex, nByteLen);
	tempindex += nByteLen;
	memcpy((UINT8*)RSAKey.publicExponent, gApduBody.DATA + tempindex, 4);
	tempindex += 4;
    if (tempindex != gApduBody.LC)
	{                             
        DBG_ERR("The data length is wrong! Data length = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
    
    //检查应用状态 
	if (!GetAppStateInfoById(appid))
	{                                     
        DBG_ERR("This application ID is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
    //检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
	{                                      
        DBG_ERR("This container ID is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	switch(AlgID)
	{
		case SGD_SM1_ECB:
		case SGD_SM1_CBC:  
		case SGD_AES256_ECB:
		case SGD_AES256_CBC:
			 Trng_GenRandom(tempbuf, DEFAULTSESSIONKEYLEN);
			 keylen = DEFAULTSESSIONKEYLEN;
			 break;
		case SGD_SMS4_ECB:
		case SGD_SMS4_CBC:	
		case SGD_AES128_ECB:
		case SGD_AES128_CBC:
			 Trng_GenRandom(tempbuf, 16);
			 keylen = 16;
			 break;	   
		case SGD_AES192_ECB:
		case SGD_AES192_CBC: 
			 Trng_GenRandom(tempbuf, 24);
			 keylen = 24;
			 break;	
		default:                                         
             DBG_INFO("Session alg is not supported! AlgID = 0x%x", AlgID);
			 SetCmdResult(SAR_UNSUPPORT_SESSION_KEY, 0);
			 return;
	}

    //保存会话密钥
	SessionKeyID = SaveSessionKey(tempbuf, keylen, AlgID);
	if (SessionKeyID == SAR_NO_ROOM)
	{                       
        DBG_ERR("The session cout meet the max!");
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	}
	
    //加密公钥加密会话密钥	
	nRet = RSA_PublicEncrypt(OutData, &keylen, tempbuf, keylen, (R_RSA_PUBLIC_KEY*)&RSAKey);
	if (nRet != RT_OK)
	{                                       
        DBG_ERR("RSA_PublicEncrypt() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_ENC_FAIL, 0);
		return;
	}

    //返回数据
	tempindex = 0;
	gApduResp.DATA[tempindex] = SessionKeyID & 0x00ff;
	gApduResp.DATA[tempindex + 1] = (SessionKeyID>>8) & 0x00ff;
	tempindex += 2;
	memcpy(gApduResp.DATA + tempindex, OutData, keylen); 
	tempindex += keylen;

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, tempindex);
}
/***************************************************************************
* Subroutine:	CMD_RSAExportSessionKeyEx()
* Function:		Export an exists session key which be encrypted by rsa pub key
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_RSAExportSessionKeyEx()
{
	UINT16 nRet = SAR_OK;
    UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 tempindex = 0;
    UINT16 nByteLen = 0;
    UINT16 SessionKeyID = 0;
    UINT32 keylen = 0;
	UINT8 OutData[256] = {0};
    R_RSA_KEYPAIR RSAKey = {0};	
	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], e[4]; 

    DBG_FUNC_BEGIN;    

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	SessionKeyID = ((gApduBody.DATA[5]<<8) | gApduBody.DATA[4]);
	RSAKey.bits = (gApduBody.DATA[9]<<24) | (gApduBody.DATA[8]<<16) | (gApduBody.DATA[7]<<8) | gApduBody.DATA[6];
	
    //检查公钥bits
	if (RSAKey.bits != 1024 && RSAKey.bits != 2048)
	{                            
        DBG_INFO("The RSA public key bits is wrong! RSAKey.bits = 0x%x", RSAKey.bits);
		SetCmdResult(SAR_INDATALENERR,0);
		return;	
	}
    //检查会话密钥ID
	if (SessionKeyID >= SESSIONKEY_MAX)
	{                                            
        DBG_ERR("SessionKeyID is wrong! SessionKeyID = 0x%x", SessionKeyID);
		SetCmdResult(SAR_INDATAERR, 0);
		return;	
	}

    //读取公钥数据
	nByteLen = RSAKey.bits/8;
	RSAKey.modulus = n;
	RSAKey.publicExponent =	e;
	RSAKey.exponent = NULL;
	RSAKey.prime[0] = NULL;
	RSAKey.prime[1] = NULL;
	RSAKey.primeExponent[0] = NULL;
	RSAKey.primeExponent[1] = NULL;
	RSAKey.coefficient = NULL;
	tempindex = 12;		
	memcpy((UINT8*)RSAKey.modulus, gApduBody.DATA + tempindex, nByteLen);
	tempindex += nByteLen;
	memcpy((UINT8*)RSAKey.publicExponent, gApduBody.DATA + tempindex, 4);
	tempindex += 4;
	if (tempindex != gApduBody.LC)
	{                                      
        DBG_ERR("The data length is wrong! Data length = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	} 
    //检查应用状态
	if (!GetAppStateInfoById(appid))
	{                
        DBG_ERR("This application ID is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
    //检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
	{
        DBG_ERR("This container ID is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
        return;
	}
	
    //加密会话密钥数据	
	keylen = 256;
	nRet = RSA_PublicEncrypt(OutData, &keylen, gSessionKey[SessionKeyID].keyData, gSessionKey[SessionKeyID].keyLen, (R_RSA_PUBLIC_KEY*)&RSAKey);
	if (nRet != RT_OK)
	{                                     
        DBG_ERR("RSA_PublicEncrypt() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_ENC_FAIL, 0);
		return;
	}

	memcpy(gApduResp.DATA, OutData, keylen);
    
    DBG_FUNC_END; 
	SetCmdResult(SAR_OK, keylen);
}
/***************************************************************************
* Subroutine:	CMD_ExtRSAPubKeyOPT()
* Function:		Extern rsa pub key opt
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExtRSAPubKeyOPT()
{
    UINT16 nRet = SAR_OK;
    UINT32 uDataLen = 0;
    UINT32 nByteLen = 0;
    UINT32 tmpIndex = 0;
    UINT32 rLen = 0;
    UINT8* pInData = NULL;
	R_RSA_PUBLIC_KEY RsaPubKey = {0};
	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], e[4]; 

    DBG_FUNC_BEGIN;	

	RsaPubKey.bits = (gApduBody.DATA[3]<<24)|(gApduBody.DATA[2]<<16)|(gApduBody.DATA[1]<<8)|gApduBody.DATA[0];
	RsaPubKey.modulus = n;
	RsaPubKey.exponent = e;
	tmpIndex += 4;

    //外来公钥数据
	nByteLen = RsaPubKey.bits/8;
	memcpy((UINT8*)RsaPubKey.modulus, gApduBody.DATA + tmpIndex, nByteLen);
	tmpIndex += nByteLen;
	memcpy((UINT8*)RsaPubKey.exponent, gApduBody.DATA + tmpIndex, 4);
	tmpIndex += 4;	

    //输入数据
	uDataLen = (gApduBody.DATA[tmpIndex+3]<<24) | (gApduBody.DATA[tmpIndex+2]<<16) | (gApduBody.DATA[tmpIndex+1]<<8) | gApduBody.DATA[tmpIndex];
	tmpIndex += 4;
	pInData = gApduBody.DATA + tmpIndex;
    
    //使用公钥处理数据
    nRet = RSA_PubKeyOpt(gApduResp.DATA, &rLen, pInData, uDataLen, &RsaPubKey);
    if (nRet != RT_OK)
    {                            
        DBG_ERR("RSA_PubKeyOpt() failed! nRet = 0x%x", nRet);
    	SetCmdResult(SAR_SIGNVERYFIERR, 0);
    	return;
    }    
    
    DBG_FUNC_END;
    SetCmdResult(SAR_OK, rLen);	
}
/***************************************************************************
* Subroutine:	CMD_ExtRSAPriKeyOPT()
* Function:		Extern rsa pri key opt
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExtRSAPriKeyOPT()
{
    UINT16 nRet = SAR_OK;
    UINT32 uDataLen = 0;
    UINT32 nByteLen = 0;
    UINT32 nPrimeLen = 0;
    UINT32 tmpIndex = 0;
    UINT32 rLen = 0;
    UINT8* pInData = NULL;  
	R_RSA_PRIVATE_KEY RsaPriKey = {0};
	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], e[4], d[256], p[128], q[128], dp[128], dq[128], qinv[128];
	
    DBG_FUNC_BEGIN;

    //读取私钥数据
	RsaPriKey.bits = (gApduBody.DATA[3]<<24) | (gApduBody.DATA[2]<<16) | (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	RsaPriKey.modulus = n;
	RsaPriKey.publicExponent =	e;
	RsaPriKey.exponent = d;
	RsaPriKey.prime[0] = p;
	RsaPriKey.prime[1] = q;
	RsaPriKey.primeExponent[0] = dp;
	RsaPriKey.primeExponent[1] = dq;
	RsaPriKey.coefficient = qinv;
	tmpIndex += 4;
	nByteLen = RsaPriKey.bits / 8;
	nPrimeLen = nByteLen / 2;
	//change @ 0720 +MAX_RSA_MODULUS_LEN-nByteLen
	memcpy((UINT8*)RsaPriKey.modulus, gApduBody.DATA + tmpIndex, nByteLen);
	tmpIndex += nByteLen;
	memcpy((UINT8*)RsaPriKey.publicExponent, gApduBody.DATA + tmpIndex, 4);
	tmpIndex += 4;
    memcpy((UINT8*)RsaPriKey.exponent, gApduBody.DATA + tmpIndex, nByteLen);
	tmpIndex += nByteLen;
	memcpy((UINT8*)RsaPriKey.prime[0], gApduBody.DATA + tmpIndex, nPrimeLen);
	tmpIndex += nPrimeLen;
	memcpy((UINT8*)RsaPriKey.prime[1], gApduBody.DATA + tmpIndex, nPrimeLen);
	tmpIndex += nPrimeLen;
	memcpy((UINT8*)RsaPriKey.primeExponent[0], gApduBody.DATA + tmpIndex, nPrimeLen);
	tmpIndex += nPrimeLen;
    memcpy((UINT8*)RsaPriKey.primeExponent[1], gApduBody.DATA + tmpIndex, nPrimeLen);
	tmpIndex += nPrimeLen;
	memcpy((UINT8*)RsaPriKey.coefficient, gApduBody.DATA + tmpIndex, nPrimeLen);
	tmpIndex += nPrimeLen;

    //输入数据
   	uDataLen = (gApduBody.DATA[tmpIndex+3]<<24) | (gApduBody.DATA[tmpIndex+2]<<16) | (gApduBody.DATA[tmpIndex+1]<<8) | gApduBody.DATA[tmpIndex];
	tmpIndex += 4;
	pInData = gApduBody.DATA + tmpIndex;
   
    //私钥运算
    nRet = RSA_PriKeyOpt(gApduResp.DATA, &rLen, pInData, uDataLen, &RsaPriKey);
    if (nRet != RT_OK)
    {                                       
        DBG_ERR("RSA_PriKeyOpt() failed! nRet = 0x%x", nRet);
    	SetCmdResult(SAR_ASYMM_ENC_FAIL, 0);
    	return;
    }    
    
    DBG_FUNC_END;
    SetCmdResult(SAR_OK, rLen);    	
}
/***************************************************************************
* Subroutine:	CMD_ExtRSAEnDecrypt()
* Function:		Extern rsa key opt(encrypt or decrypt)
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExtRSAEnDecrypt()
{
    UINT16 nRet = SAR_OK; 
	UINT16 nStatus = SAR_OK;
	UINT16 AlgID = 0;
    UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 index = 0;
    UINT16 keybitlen = 0;
    UINT16 nByteLen = 0;
    UINT16 nPrimeLen = 0;
	UINT32 datalen = 0;
    UINT32 rlen = 0;
	UINT8 *data = NULL;
    UINT8 *OutData = NULL;
	UINT8* RSAKeydata = NULL;  
    R_RSA_KEYPAIR RSAKey = {0};
	APPSTATE* AppState = NULL;
	//以下所有变量，必须按四字节对齐 
	__align(4) UINT8 n[256], e[4], d[256], p[128], q[128], dp[128], dq[128], qinv[128]; 

    DBG_FUNC_BEGIN;

    //检查数据长度
	datalen = gApduBody.LC - 6;
	if(datalen > 512)
    {                          
        DBG_ERR("The data length is wrong! Data length = 0x%x, gApduBody.LC = 0x%x", datalen, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	data = gApduBody.DATA;

    //app id 和 container id 以及操作flag在数据之后！
	appid = ((gApduBody.DATA[datalen + 1]<<8) | gApduBody.DATA[datalen]);
	containerid = ((gApduBody.DATA[datalen + 3]<<8) | gApduBody.DATA[datalen + 2]);
	AlgID = ((gApduBody.DATA[datalen + 5]<<8) | gApduBody.DATA[datalen + 4]);
    DBG_INFO("appid = 0x%x, containerid = 0x%x, AlgID = 0x%x", appid, containerid, AlgID);

    //检查应用状态
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
    {                      
        DBG_ERR("This application ID is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
    //检查安全状态
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
    {                        
        DBG_ERR("The user has not logined!");
		SetCmdResult(nStatus, 0);
    	return;
	}
    //检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
    {                         
        DBG_ERR("This container ID is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS,0);
		return;
	}

    //从文件读取密钥数据
	nRet = FS_SelectFileByCluster(containerid,containerid);
	if(gApduHeader.P1)
    {                        
        DBG_INFO("Sign key is selected!");
		nRet = FS_SelectFileByID(SIGNKEYID);
	}
    else
    {                              
        DBG_INFO("Exch key is selected!");
		nRet = FS_SelectFileByID(EXKEYID);
	}
	if (nRet != RT_OK)
    {                        
        DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	RSAKeydata = gApduBody.DATA + datalen + (4 - datalen%4) + 8;	 	//确保取址4字节对齐；
	nRet = FS_ReadFile(RSAKeydata, 0, 2);
	keybitlen = (RSAKeydata[1]<<8) | RSAKeydata[0];
	nByteLen = keybitlen / 8;
	nPrimeLen = keybitlen / 16;
	nRet |= FS_ReadFile(RSAKeydata, 0, nByteLen*2 + nPrimeLen*5 + 6);
	if (nRet != RT_OK)
	{                            
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}
	RSAKey.modulus = n;
	RSAKey.publicExponent =	e;
	RSAKey.exponent = d;
	RSAKey.prime[0] = p;
	RSAKey.prime[1] = q;
	RSAKey.primeExponent[0] = dp;
	RSAKey.primeExponent[1] = dq;
	RSAKey.coefficient = qinv;
	index = 0;
	memcpy((UINT8*)&RSAKey.bits, RSAKeydata, 2);
	index += 2;
	memcpy(RSAKey.modulus, RSAKeydata + index, nByteLen);
	index += nByteLen;
	memcpy(RSAKey.publicExponent, RSAKeydata + index, 4);
	index += 4;
	memcpy(RSAKey.exponent, RSAKeydata + index, nByteLen);
	index += nByteLen;
    memcpy(RSAKey.prime[0], RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
    memcpy(RSAKey.prime[1], RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
    memcpy(RSAKey.primeExponent[0], RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;
	memcpy(RSAKey.primeExponent[1], RSAKeydata + index, nPrimeLen); 
	index += nPrimeLen;
	memcpy(RSAKey.coefficient, RSAKeydata + index, nPrimeLen);
	index += nPrimeLen;

	OutData = RSAKeydata;
	if (AlgID)  //AlgID=true 执行加密操作 
	{
    	if(gApduHeader.P2)
        {                   
            DBG_INFO("Use public key to encrypt data!");
    		nRet = RSA_PublicEncrypt(OutData, &rlen, data, datalen, (R_RSA_PUBLIC_KEY*)&RSAKey);
    	}
        else
        {              
            DBG_INFO("Use private key to encrypt data!");
    		nRet = RSA_PrivateEncrypt(OutData, &rlen, data, datalen, &RSAKey);
    	}		
	}
    else    //AlgID=false 执行解密操作 
	{
    	if(gApduHeader.P2)
        {                           
            DBG_INFO("Use public key to decrypt data!");
    		nRet = RSA_PublicDecrypt(OutData, &rlen, data, datalen, (R_RSA_PUBLIC_KEY*)&RSAKey);
    	}
        else
        {                                    
            DBG_INFO("Use private key to decrypt data!");
    		nRet = RSA_PrivateDecrypt(OutData, &rlen, data, datalen, &RSAKey);
    	}		
	}
	if (nRet != RT_OK)
    {                         
        DBG_ERR("RSA operation failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_DEC_FAIL, 0);
		return;
	}

	memcpy(gApduResp.DATA, OutData, rlen);

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, rlen);
}
/***************************************************************************
* Subroutine:	CMD_ExportRSAKeyPair()
* Function:		Export rsa key pair with encrypted by a session key
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExportRSAKeyPair()
{	 
	UINT16 nRet = SAR_OK;  
	UINT16 nStatus = SAR_OK;
	UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 tempindex = 0;
    UINT16 nByteLen = 0;
    UINT16 nPrimeLen = 0;
    UINT16 SessionKeyID = 0;
	UINT32 AlgID = 0;
    UINT32 RSAbits = 0;
    UINT32 RSAKeydatalen = 0;
	UINT8* RSAKeydata = NULL;	   	
	APPSTATE* AppState = NULL;
		
    DBG_FUNC_BEGIN;
        							
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);			 
	AlgID = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	SessionKeyID = ((gApduBody.DATA[9]<<8) | gApduBody.DATA[8]);  
	RSAbits = ((gApduBody.DATA[11]<<8) | gApduBody.DATA[10]); 
	tempindex = 12; 	
	
    //检查数据长度   
	if (tempindex != gApduBody.LC)
    {                                 
        DBG_ERR("The data length is wrong! Data length = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR,0);
		return;
	}	  
    //检查公钥bits
	if (RSAbits != 1024 && RSAbits != 2048)
	{                            
        DBG_ERR("The RSA public key bits is wrong! RSAbits = 0x%x", RSAbits);
		SetCmdResult(SAR_INDATAERR, 0);
		return;	
	}
    //检查会话密钥ID
	if (SessionKeyID >= SESSIONKEY_MAX)
    {	
		DBG_ERR("The key ID is error! key id = 0x%x", SessionKeyID);  
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}

    //检查应用状态
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
    {                        
        DBG_ERR("This application ID is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
    //检查安全状态
	gCurSAStatus = AppState->AppState;
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
    {                         
        DBG_ERR("The user has not logined!");
		SetCmdResult(nStatus, 0);
    	return;
	}
    //检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
    {                      
        DBG_ERR("This container ID is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(containerid, containerid);  		
 
    //读取加密密钥对数据
	nRet = FS_SelectFileByID(EXKEYID);
	if (nRet != RT_OK)
    {                         
        DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST,0);
		return;
	}
	RSAKeydata = gApduBody.DATA + tempindex;
	nRet = FS_ReadFile(RSAKeydata, 0, 2);
	nByteLen = RSAbits / 8;
	nPrimeLen = RSAbits / 16;
	RSAKeydatalen = nByteLen*2 + nPrimeLen*5 + 6;
	nRet |= FS_ReadFile(RSAKeydata, 0, RSAKeydatalen);
	if (nRet != RT_OK)
	{                                   
        DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
		
	}
	tempindex += RSAKeydatalen;
		
	switch(AlgID)
    {
		case SGD_SM1_ECB:
			 SM1_Init(gSessionKey[SessionKeyID].keyData, gSessionKey[SessionKeyID].keyData + 16);
			 SM1_Run(SM1_ENCRYPT, SM1_ECB, RSAKeydata, RSAKeydata, RSAKeydatalen, NULL);
			 break;
		case SGD_SMS4_ECB:
			 SMS4_Init(gSessionKey[SessionKeyID].keyData);
			 SMS4_Run(SM1_ENCRYPT, SMS4_ECB, RSAKeydata, RSAKeydata, RSAKeydatalen, NULL);
			 break;		 
		case SGD_AES128_ECB:
			 Aes_Init(gSessionKey[SessionKeyID].keyData, 16);
			 Aes_Run(AES_ENCRYPT, AES128_ECB, RSAKeydata, RSAKeydata, RSAKeydatalen, NULL);
			 break;	
		case SGD_AES192_ECB:
			 Aes_Init(gSessionKey[SessionKeyID].keyData, 24);
			 Aes_Run(AES_ENCRYPT, AES192_ECB, RSAKeydata, RSAKeydata, RSAKeydatalen,NULL);
			 break;		 
		case SGD_AES256_ECB:
			 Aes_Init(gSessionKey[SessionKeyID].keyData, 32);
			 Aes_Run(AES_ENCRYPT, AES256_ECB, RSAKeydata, RSAKeydata, RSAKeydatalen, NULL);
			 break;
		default:                   
             DBG_ERR("AlgID is wrong! AlgID = 0x%x", AlgID);
			 SetCmdResult(SAR_UNSUPPORT_SESSION_KEY, 0);
			 return;
	}

	tempindex = 0;
	gApduResp.DATA[tempindex] = RSAKeydatalen & 0x00ff;
	gApduResp.DATA[tempindex+1] = (RSAKeydatalen>>8) & 0x00ff;
	tempindex += 2;
	memcpy(gApduResp.DATA + tempindex, RSAKeydata, RSAKeydatalen); 
	tempindex += RSAKeydatalen;

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, tempindex);
}	 
#endif

#ifdef ECCSUPPORT
/***************************************************************************
* Subroutine:	CMD_GenECCKeyPair()
* Function:		Generate Ecc key pair
* Author:   Syno Common	
* Date:		2015.06.20
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GenECCKeyPair()
{                   
	UINT16 nRet = SAR_OK;
    UINT16 nStatus = SAR_OK;
	UINT16 appid = 0;
    UINT16 containerid = 0;
    UINT16 keylen = 0;
    UINT16 keybitlen = 0;
	tFileNode Node = {0};
	SM2_PriKey prv = {0};
	SM2_PubKey pub = {0};		   
	APPSTATE* AppState = NULL;

    DBG_FUNC_BEGIN;		

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	keybitlen = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	if (keybitlen != ECC_KEY_BITS)
	{
        DBG_ERR("ECC key bits is wrong! keybitlen = 0x%x", keybitlen);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}
	if ((gApduBody.LC & 0xFF) != 8)
	{                          
        DBG_ERR("LC is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
    //检查应用状态
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{                                            
        DBG_ERR("This application ID is not exist! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}       
	FS_SelectFileByCluster(appid, appid);
    //检查登录状态              
	gCurSAStatus = AppState->AppState; 
	nStatus=IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
	{                                   
        DBG_ERR("The user has not logined!");
		SetCmdResult(nStatus, 0);
    	return;
	}
    //检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
	{                         
        DBG_ERR("This container ID is not exist! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	} 
	FS_SelectFileByCluster(containerid, containerid);

    //生成ECC密钥对
	keylen = (keybitlen/8);
	pub.X = gApduResp.DATA;
	pub.Y = gApduResp.DATA + keylen;
	prv.PrivateKey = gApduResp.DATA + keylen*2; 		
	nRet = SM2_GenKey(&prv, &pub, keybitlen, 0);
	if (nRet != RT_OK)
	{                   
        DBG_ERR("SM2_GenKey() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_DATA_ERR, 0);
		return;
	}	

    //删除老的密钥对文件
	memset((UINT8*)&Node, 0, sizeof(Node)); 
	Node.FileType = FT_KEY;
	Node.FileSize = keylen * 3;
	Node.FCP.FileID = SIGNKEYID;
	strcpy(Node.FCP.Name, "SignECCKey");	
	nRet = FS_SelectFileByID(SIGNKEYID);
	if (nRet == RT_OK)
	{
		nRet = FS_DeleteFile();
		if (nRet != RT_OK)
		{                         
            DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0);
			return;
		}
	}
    //创建密钥对文件		
	nRet = FS_CreateFile(&Node);
	if (nRet != RT_OK)
	{                                  
        DBG_ERR("FS_CreateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}
    //写数据         
	nRet = FS_UpdateFile((UINT8*)prv.PrivateKey, 0, keylen);
	nRet |= FS_UpdateFile((UINT8*)pub.X, keylen, keylen*2);
	if (nRet != RT_OK)
	{                                       
        DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}
    //更新容器类型
	FS_SelectFileByCluster(containerid, containerid);
	memset((UINT8*)&Node, 0, sizeof(Node));
	FS_ReadDirNodeInfo((UINT8*)&Node);        
	Node.ContainerType = ECC_CON;
	nRet = FS_WriteDirNodeInfo((UINT8*)&Node);
	if (nRet != RT_OK)
	{                                    
        DBG_ERR("FS_WriteDirNodeInfo() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

    DBG_FUNC_END;
	SetCmdResult(SAR_OK, keylen*2);
}
/***************************************************************************
* Subroutine:	CMD_ImportECCkeyPair()
* Function:		Import ECC key pair which be encrypted
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ImportECCkeyPair()
{
	UINT16 nRet = SAR_OK;
	UINT16 nStatus = SAR_OK;
	UINT16 appid = 0;
	UINT16 containerid = 0;
	UINT16 tempindex = 0;
	UINT16 encypherprvkeylen = 0;
	UINT32 ulAsymmAlgID = 0;
	UINT32 ulSymmAlgID = 0;
	UINT32 ulC1Bits = 0;
	UINT32 ulBits = 0;
	UINT32 CipherLen = 0;
	UINT32 eccCipherLen = 0;
	UINT32 keylen = 0;	
    UINT8 cbEncryptedPriKey[64] = {0};
	UINT8 KEY[32] = {0};
    UINT8 Cipher[144] = {0};
	UINT8 tempbuf[200] = {0};

	tFileNode Node = {0};
	SM2_PriKey INprv = {0};
	SM2_PriKey prv = {0};
	SM2_PubKey INpub = {0};
	APPSTATE* AppState = NULL;

	DBG_FUNC_BEGIN;

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	tempindex = 4;
	//Version
	ulAsymmAlgID = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
	//Symm alg ID
	ulSymmAlgID = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;

    //C1 bits				   
    ulC1Bits = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
    tempindex += 4;
	if (ulC1Bits != ECC_KEY_BITS)
	{
		DBG_ERR("ECC key bits is wrong! ulC1Bits = 0x%x", ulC1Bits);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}
	Cipher[0] = 0x04;
	//CX
	memcpy(Cipher + 1, gApduBody.DATA + tempindex, ulC1Bits/8);
	eccCipherLen = 1 + ulC1Bits/8;
	tempindex += ulC1Bits/8;
	//CY
	memcpy(Cipher + eccCipherLen, gApduBody.DATA + tempindex, ulC1Bits/8);
	eccCipherLen += ulC1Bits/8;
	tempindex += ulC1Bits/8;
	//HASH
	memcpy(Cipher + eccCipherLen, gApduBody.DATA + tempindex, 32);
	eccCipherLen += 32;
	tempindex += 32;
	//Cipher Len
	CipherLen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
	//Cipher
	memcpy(Cipher + eccCipherLen, gApduBody.DATA + tempindex, CipherLen);
	eccCipherLen += CipherLen;
	tempindex += CipherLen;

	//Bits
    ulBits = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
    tempindex += 4;
	if(ulBits != ECC_KEY_BITS)
	{
		DBG_ERR("ECC key bits is wrong! ulBits = 0x%x", ulBits);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}

	INpub.X = tempbuf + 64;
	INpub.Y = tempbuf + 64 + ulBits / 8;
	//Public key X
	memcpy(INpub.X, gApduBody.DATA + tempindex, ulBits/8);
	tempindex += ulBits/8;
	//Public key Y
	memcpy(INpub.Y, gApduBody.DATA + tempindex, ulBits/8);
	tempindex += ulBits/8;		

	//Encrypted private key length	
    encypherprvkeylen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
    tempindex += 4;
	if (encypherprvkeylen != 64)
	{
		DBG_ERR("Encypher prv key len is wrong! encypherprvkeylen = 0x%x", encypherprvkeylen);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
	}
	//Encrypted private key data 
	memcpy(cbEncryptedPriKey, gApduBody.DATA + tempindex, encypherprvkeylen);
    tempindex += encypherprvkeylen;
	if (tempindex != gApduBody.LC)
	{
		DBG_ERR("The input data lenght is wrong! tempindex = 0x%xm gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}	

	//检查应用状态
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS,  0);
		return;
	}
	gCurSAStatus = AppState->AppState; 
	FS_SelectFileByCluster(appid, appid);
	//检查登录状态
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
	{
		DBG_ERR("The user is not login!");
		SetCmdResult(nStatus, 0);
    	return;
	}
	//检查容器状态
	if (SAR_OK != IsContainerIdValid(appid, containerid))
	{
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	} 
	FS_SelectFileByCluster(containerid, containerid);

	//读取签名私钥数据
	nRet = FS_SelectFileByID(SIGNKEYID);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;		
	}
	prv.PrivateKey = tempbuf + 64 + 2*ulBits/8;
	nRet = FS_ReadFile((UINT8*)prv.PrivateKey, 0, ulBits/8);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_INDATAERR, 0);
		return;
		
	}
	//签名私钥解密会话密钥
	keylen = 32;
	memset(KEY, 0, keylen);
	nRet = SM2_PriKeyDecrypt(&prv, KEY, &keylen, Cipher, eccCipherLen);
	if (nRet != RT_OK)
	{
		DBG_ERR("SM2_PriKeyDecrypt() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_DEC_FAIL, 0);
		return;
	}	                  

	//会话密钥解密加密私钥
	switch(ulSymmAlgID)
	{
	case SGD_SM1_ECB:
		 SM1_Init(KEY, KEY + 16);
		 SM1_Run(SM1_DECRYPT, SM1_ECB, cbEncryptedPriKey, tempbuf, 64, NULL);
		 break;
	case SGD_SMS4_ECB:
		 SMS4_Init(KEY);
		 SMS4_Run(SMS4_DECRYPT, SMS4_ECB, cbEncryptedPriKey, tempbuf, 64, NULL);
		 break;
	default:
		 DBG_ERR("ulSymmAlgID is not supported! ulSymmAlgID = 0x%x", ulSymmAlgID);
		 SetCmdResult(SAR_UNSUPPORT_SESSION_KEY, 0);
		 return;
	}

	//删除老的加密密钥对文件
    INprv.PrivateKey = tempbuf + 32;  
    DBG_BUF(tempbuf, 64);
	memset((UINT8*)&Node, 0, sizeof(Node));
	Node.FileType = FT_KEY;
	Node.FileSize = 3 * ulBits/8;
	Node.FCP.FileID = EXKEYID;
	strcpy(Node.FCP.Name, "ExchgECCKey");	
	nRet = FS_SelectFileByID(EXKEYID);
	if (nRet == RT_OK)
	{
		nRet = FS_DeleteFile();
		if (nRet != RT_OK)
		{
			DBG_ERR("FS_DeleteFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_WRITEFILEERR, 0);
			return;
		}
	}
	//创建新的加密密钥对文件	
	nRet=FS_CreateFile(&Node);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_CreateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}
	//写数据
	nRet = FS_UpdateFile((UINT8*)INprv.PrivateKey, 0, ulBits/8);
	nRet = FS_UpdateFile((UINT8*)INpub.X, ulBits/8, 2*ulBits/8);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_UpdateFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_WRITEFILEERR, 0);
		return;
	}

	DBG_FUNC_END;

	SetCmdResult(SAR_OK, 0);	
}
/***************************************************************************
* Subroutine:	CMD_ECCSignData()
* Function:		Signature by Ecc prv key
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ECCSignData()
{
	UINT16 nRet = SAR_OK;
	UINT16 nStatus = 0;
	UINT16 appid = 0;
	UINT16 containerid = 0;
	UINT16 datalen = 0;
	UINT16 tempindex = 0;
    UINT32 signaturelen = 0;
	UINT32 ECCKeyBytes = 0;
	UINT8* data = NULL;
	UINT8 sm3vdata[32] = {0};
	UINT8 prvdata[32] = {0};
	UINT8 pubdata[64] = {0};
	UINT8 tempbuf[100] = {0};
	SM3Value *sm3 = NULL;
	SM2_PriKey prv = {0};
	SM2_PubKey pub = {0};	
	APPSTATE* AppState = NULL;
	
	DBG_FUNC_BEGIN;

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	if (gApduBody.LC < 4)
	{
		DBG_ERR("Data length is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	//检查应用状态
	AppState = GetAppStateInfoById(appid);
	if (!AppState)
	{
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
	FS_SelectFileByCluster(appid, appid);
	//检查登录状态
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if (nStatus != SAR_OK)
	{
		DBG_ERR("The user is not login!");
		SetCmdResult(nStatus, 0);
    	return;
	}
	//检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
	{
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	} 
	FS_SelectFileByCluster(containerid, containerid);
	
	//读取签名密钥对数据
	nRet = FS_SelectFileByID(SIGNKEYID);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_SelectFileByID(SIGNKEYID) failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
		return;
	}
	prv.PrivateKey = prvdata;
	pub.X = pubdata;
	pub.Y = pubdata + 32;
	sm3 = (SM3Value *)sm3vdata;	 
	nRet = FS_ReadFile((UINT8*)prv.PrivateKey, 0, 32);
	nRet |= FS_ReadFile((UINT8*)pub.X, 32, 64);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
		return;
	}
	
	//使用私钥签名
	if (gApduHeader.P1 == 1)      // 原始数据
	{
		UINT8* userID = NULL;
		UINT32 userIDLen = 0;
    	userIDLen = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
		datalen = gApduBody.LC - (4 + 4 + userIDLen);
		if (userIDLen > 32 || datalen > 2048)
		{
			DBG_ERR("User ID or data length is wrong! userIDLen = 0x%x, datalen = 0x%x", userIDLen, datalen);
			SetCmdResult(SAR_INDATALENERR, 0);
			return;
		}
		userID = gApduBody.DATA + (4 + 4);
		data = gApduBody.DATA + (4 + 4 + userIDLen);
		nRet = SM2_GetHashValue(&pub, sm3, data, datalen, userID, userIDLen);
		nRet |= SM2_Sign(&prv, (UINT8*)sm3, 32, tempbuf, &signaturelen); 
	}
	else if (gApduHeader.P1 == 2)
	{						
		data = gApduBody.DATA + 4;
		datalen = gApduBody.LC - 4;
		if(datalen != 32)
		{
			DBG_ERR("Data length is wrong! datalen = 0x%x", datalen);
			SetCmdResult(SAR_INDATALENERR, 0);
			return;
		}
		nRet = SM2_Sign(&prv, data, datalen, tempbuf, &signaturelen);
	}
	else
	{
		DBG_ERR("P1 is wrong! gApduHeader.P1 = 0x%x", gApduHeader.P1);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
	}	
	if (nRet != RT_OK)
	{
		DBG_ERR("SM2_Sign() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_PRI_KEY_SIGN_FAIL, 0);
		return;
	}

	//返回签名数据
	ECCKeyBytes = ECC_KEY_BITS;
	gApduResp.DATA[0] = ECCKeyBytes & 0x000000ff;
	gApduResp.DATA[1] = (ECCKeyBytes>>8) & 0x000000ff;
	gApduResp.DATA[2] = (ECCKeyBytes>>16) & 0x000000ff;
	gApduResp.DATA[3] = (ECCKeyBytes>>24) & 0x000000ff;
	tempindex = 4;
	memcpy(gApduResp.DATA + tempindex, tempbuf, signaturelen);	
	tempindex += signaturelen;

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, tempindex);
}
/***************************************************************************
* Subroutine:	CMD_ECCVerify()
* Function:		Verify by ECC pub key
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ECCVerify()
{	
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 tempindex = 0;
	UINT16 ECCKeyBytes = ECC_KEY_BITS/2;
    UINT32 signaturelen = 0;
	UINT32 ECCKeynBits = ECC_KEY_BITS;
	UINT8 *data = NULL;
	SM3Value *sm3 = NULL;
	UINT8 pubdata[64] = {0};
	UINT8 sm3Value[32] = {0};
	UINT8 signature[64] = {0};	
	SM2_PubKey ECCPubkey = {0};

	DBG_FUNC_BEGIN;

	sm3 = (SM3Value *)sm3Value; 
	ECCPubkey.X = pubdata;
	ECCPubkey.Y = pubdata + 32;	
	ECCKeynBits = (gApduBody.DATA[3]<<24) | (gApduBody.DATA[2]<<16) | (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if (ECCKeynBits != ECC_KEY_BITS)
	{
		DBG_ERR("ECC key bits error! bits = 0x%x", ECCKeynBits);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}
	tempindex = 4;
	ECCKeyBytes = ECCKeynBits/8;
	memcpy(ECCPubkey.X, gApduBody.DATA+tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	memcpy(ECCPubkey.Y, gApduBody.DATA+tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
    //
	datalen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;	
    data = gApduBody.DATA + tempindex;
	tempindex += datalen;
    //                    
	signaturelen = ECCKeyBytes + ECCKeyBytes;
    memcpy(signature, gApduBody.DATA + tempindex, signaturelen);
	tempindex += signaturelen;
	if (tempindex != gApduBody.LC)
	{
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	if (datalen > 32)   //只支持HASH数据
	{						
		DBG_ERR("Data length is wrong! datalen = 0x%x", datalen);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}
	memcpy(sm3, data, datalen);
	nRet = SM2_Verify(&ECCPubkey, (UINT8*)sm3, datalen, signature, signaturelen);
	if (nRet != RT_OK)
	{
		DBG_ERR("SM2_Verify() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_SIGNVERYFIERR, 0);
		return;
	}

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);	
}
/***************************************************************************
* Subroutine:	CMD_ECCExportSessionKey()
* Function:		Generate and export sessionkey which be encrypted by ecc pub key
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ECCExportSessionKey()
{
	UINT16 nRet = SAR_OK;
	UINT16 appid = 0;
	UINT16 containerid = 0;
	UINT16 tempindex = 0;
	UINT16 ECCKeyBytes = 0;
	UINT16 SessionKeyID = 0;
	UINT32 AlgID = 0;
	UINT32 keylen = 0;
	UINT32 cypherlen = 0;
	UINT32 ECCKeynBits = 0;
	UINT8 *tempbuf = NULL;
	UINT8 pubdata[64] = {0};
	SM2_PubKey pub = {0};
	pub.X =	pubdata;
	pub.Y =	pubdata + 32;
	
	DBG_FUNC_BEGIN;

    appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	ECCKeynBits = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	if (ECCKeynBits != ECC_KEY_BITS)
    {
		DBG_ERR("ECC key bits error! bits = 0x%x", ECCKeynBits);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}
	ECCKeyBytes = ECCKeynBits/8;
	tempindex = 8;
	memcpy(pub.X, gApduBody.DATA+tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	memcpy(pub.Y, gApduBody.DATA+tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	AlgID = (gApduBody.DATA[tempindex+3]<<24)|(gApduBody.DATA[tempindex+2]<<16)|(gApduBody.DATA[tempindex+1]<<8)|gApduBody.DATA[tempindex];
	tempindex += 4;
	if (tempindex != gApduBody.LC)
    {
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	//检查应用状态
	if (!GetAppStateInfoById(appid))
    {
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}                  
	FS_SelectFileByCluster(appid,appid);
	//检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
    {
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	nRet = FS_SelectFileByCluster(containerid, containerid);
	if (nRet != RT_OK)
    {
		DBG_ERR("FS_SelectFileByCluster()! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	//生成会话密钥
	tempbuf = gApduBody.DATA + 512;
	switch(AlgID)
    {
	case SGD_SM1_ECB:
	case SGD_SM1_CBC:
    case SGD_SM1_MAC:
		 Trng_GenRandom(tempbuf, DEFAULTSESSIONKEYLEN);
		 keylen = DEFAULTSESSIONKEYLEN;
		 break;
	case SGD_SMS4_MAC:	 
	case SGD_SMS4_ECB:
	case SGD_SMS4_CBC:
		 Trng_GenRandom(tempbuf, 16);
		 keylen = 16;
		 break;
	default:
		 DBG_ERR("Session key alg ID is wrong! AlgID = 0x%x", AlgID);
		 SetCmdResult(SAR_UNSUPPORT_SESSION_KEY, 0);
		 return;
	}
	SessionKeyID = SaveSessionKey(tempbuf, keylen, AlgID);
	if(SessionKeyID == SAR_NO_ROOM)
    {
		DBG_ERR("Session key count meet the max!");
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	} 

	//使用公钥加密会话密钥
    cypherlen = 97 + keylen; 
	nRet = SM2_PubKeyEncrypt(&pub, tempbuf, &cypherlen, gSessionKey[SessionKeyID].keyData, keylen);
	if (nRet != RT_OK)
    {
		DBG_ERR("Session key alg ID is wrong! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_ENC_FAIL, 0);
		return;
	}
	
    //C1 bits
    gApduResp.DATA[0] = ECCKeynBits & 0x00ff;
    gApduResp.DATA[1] = (ECCKeynBits>>8) & 0x00ff;
	gApduResp.DATA[2] = (ECCKeynBits>>16) & 0x000000ff;
	gApduResp.DATA[3] = (ECCKeynBits>>24) & 0x000000ff;
    //C1X
    tempindex = 4;
    memcpy(gApduResp.DATA+tempindex, tempbuf + 1, ECCKeynBits/8);
    tempindex += ECCKeynBits/8;           
    //C1Y
    memcpy(gApduResp.DATA+tempindex, tempbuf + (1+ECCKeynBits/8), ECCKeynBits/8);
    tempindex += ECCKeynBits/8;          
    //HASH
    memcpy(gApduResp.DATA+tempindex, tempbuf + (1+2*ECCKeynBits/8), 32);
    tempindex += 32;                    
    //CipherLen
    memcpy(gApduResp.DATA+tempindex, &keylen, 4);
    tempindex += 4;                    
    //Cipher
    memcpy(gApduResp.DATA+tempindex, tempbuf + (1+2*ECCKeynBits/8+32), keylen);
    tempindex += keylen;
    //Session key ID
	gApduResp.DATA[tempindex++] = SessionKeyID & 0x000000ff;
	gApduResp.DATA[tempindex++] = (SessionKeyID>>8) & 0x000000ff;
	
	DBG_FUNC_END;

	SetCmdResult(SAR_OK, tempindex);	
}
/***************************************************************************
* Subroutine:	CMD_ExportSessionKeyEx()
* Function:		Export an exist sessionkey which be encrypted by ecc pub key
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ECCExportSessionKeyEx()
{
	UINT16 nRet = SAR_OK;
	UINT16 appid = 0;
	UINT16 containerid = 0;
	UINT16 tempindex = 0;
	UINT16 ECCKeyBytes = 0;
	UINT16 SessionKeyID = 0;
	UINT32 cypherlen = 0;
	UINT32 ECCKeynBits = 0;
	UINT8 *tempbuf = NULL;
	UINT8 pubdata[64] = {0};
	SM2_PubKey pub = {0};
	pub.X =	pubdata;
	pub.Y =	pubdata + 32;
	
	DBG_FUNC_BEGIN;

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	SessionKeyID = ((gApduBody.DATA[5]<<8) | gApduBody.DATA[4]);
	ECCKeynBits = (gApduBody.DATA[9]<<24) | (gApduBody.DATA[8]<<16) | (gApduBody.DATA[7]<<8) | gApduBody.DATA[6];
	if (ECCKeynBits != ECC_KEY_BITS)
    {
		DBG_ERR("ECC key bits error! bits = 0x%x", ECCKeynBits);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}
	if (SessionKeyID >= SESSIONKEY_MAX || !gSessionKey[SessionKeyID].flag)
    {
		DBG_ERR("Session key id is wrong! SessionKeyID = 0x%x", SessionKeyID);
		SetCmdResult(SAR_INDATAERR, 0);
		return;	
	}
	ECCKeyBytes = ECCKeynBits/8;
	tempindex = 10;
	memcpy(pub.X, gApduBody.DATA + tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	memcpy(pub.Y, gApduBody.DATA + tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	if (tempindex != gApduBody.LC)
    {
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	//检查应用状态
	if (!GetAppStateInfoById(appid))
    {
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}               
	FS_SelectFileByCluster(appid, appid);
	//检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
    {
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	nRet = FS_SelectFileByCluster(containerid, containerid);
	if (nRet != RT_OK)
    {
		DBG_ERR("FS_SelectFileByCluster()! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	//公钥加密会话密钥数据				
    cypherlen = 97 + gSessionKey[SessionKeyID].keyLen;
	tempbuf = gApduBody.DATA + 512;
	nRet = SM2_PubKeyEncrypt(&pub, tempbuf, &cypherlen, gSessionKey[SessionKeyID].keyData, gSessionKey[SessionKeyID].keyLen);
	if (nRet != RT_OK)
    {
		DBG_ERR("Session key alg ID is wrong! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_ENC_FAIL, 0);
		return;
	}
	
	//Bits
	gApduResp.DATA[0] = ECCKeynBits & 0x000000ff;
	gApduResp.DATA[1] = (ECCKeynBits>>8) & 0x000000ff;
	gApduResp.DATA[2] = (ECCKeynBits>>16) & 0x000000ff;
	gApduResp.DATA[3] = (ECCKeynBits>>24) & 0x000000ff;	
	tempindex = 4;
	//X	
	memcpy(gApduResp.DATA+tempindex, tempbuf + 1, ECCKeynBits/8);	
	tempindex += (ECCKeynBits/8);
	//Y
	memcpy(gApduResp.DATA+tempindex, tempbuf+ 1 +ECCKeynBits/8, ECCKeynBits/8);	
	tempindex += (ECCKeynBits/8);
	//HASH
	memcpy(gApduResp.DATA+tempindex, tempbuf + 1 + 2*(ECCKeynBits/8), 32);	
	tempindex += 32;
	//Cypher length
	cypherlen = gSessionKey[SessionKeyID].keyLen;
	memcpy(gApduResp.DATA+tempindex, &cypherlen, 4);
	tempindex += 4;
	//Cypher
	memcpy(gApduResp.DATA+tempindex, tempbuf + 1 + 2*(ECCKeynBits/8) + 32, gSessionKey[SessionKeyID].keyLen);
	tempindex += gSessionKey[SessionKeyID].keyLen;
	
	DBG_FUNC_END;

	SetCmdResult(SAR_OK, tempindex);	
}
/***************************************************************************
* Subroutine:	CMD_ExtECCEncrypt()
* Function:		Encrypt by extern ecc pub key 
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExtECCEncrypt()
{	
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 tempindex = 0;
	UINT16 ECCKeyBytes = 0;
    UINT32 cypherlen = 0;
	UINT32 ECCKeynBits = 0;
	UINT8  *data = NULL;	
	UINT8 pubdata[64] = {0};
	SM2_PubKey pub = {0};

	DBG_FUNC_BEGIN;

	ECCKeynBits = (gApduBody.DATA[3]<<24) | (gApduBody.DATA[2]<<16) | (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if (ECCKeynBits != ECC_KEY_BITS)
    {
		DBG_ERR("ECC key pair bits length is wrong! ECCKeynBits = 0x%x", ECCKeynBits);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}	
	ECCKeyBytes = ECCKeynBits/8;
	pub.X =	pubdata;
	pub.Y =	pubdata + ECCKeyBytes;	
	tempindex = 4;
	memcpy(pub.X, gApduBody.DATA + tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	memcpy(pub.Y, gApduBody.DATA + tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	datalen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	if (datalen>2048 || datalen <= 0)
    {
		DBG_ERR("Plain data length is wrong! datalen=0x%x", datalen);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}
	tempindex += 4;
	data = gApduBody.DATA + tempindex;
	tempindex += datalen;
	if (tempindex != gApduBody.LC)
    {
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

    cypherlen = 97 + datalen;
    nRet = SM2_PubKeyEncrypt(&pub, gApduResp.DATA, &cypherlen, data, datalen);
	if (nRet != RT_OK)
    {
		DBG_ERR("Session key alg ID is wrong! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_ENC_FAIL, 0);
		return;
	}

	DBG_FUNC_END; 
	SetCmdResult(SAR_OK, cypherlen);
}
/***************************************************************************
* Subroutine:	CMD_ExtECCDecrypt()
* Function:		Decrypt by extern ecc prv key
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExtECCDecrypt()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 tempindex = 0;
	UINT16 ECCKeyBytes = 0;
	UINT16 index = 0;
	UINT8 *rdata = NULL;	
    UINT32 rdatalen = 0;
	UINT32 ECCKeynBits = 0;
	UINT8 prvdata[32] = {0};
	UINT8 tempbuf[512] = {0};
	SM2_PriKey prv = {0};
	prv.PrivateKey = prvdata;
	
	DBG_FUNC_BEGIN;

	ECCKeynBits = (gApduBody.DATA[3]<<24) | (gApduBody.DATA[2]<<16) | (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if (ECCKeynBits != ECC_KEY_BITS)
	{
		DBG_ERR("ECC key pair bits length is wrong! ECCKeynBits = 0x%x", ECCKeynBits);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}

	//读取私钥数据
	ECCKeyBytes = ECCKeynBits/8;
	tempindex = 4;
	memcpy(prv.PrivateKey, gApduBody.DATA+tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	
	//CX
	index = 1;
	tempbuf[0] = 0x04;
	memcpy(tempbuf + index, gApduBody.DATA + tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	index += ECCKeyBytes;
	//CY
	memcpy(tempbuf + index, gApduBody.DATA + tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;
	index += ECCKeyBytes;
	//HASH
	memcpy(tempbuf + index, gApduBody.DATA + tempindex, 32);
	tempindex += 32;
	index += 32;
	//Cipher len
	datalen= (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	if (datalen > 2048)
	{
		DBG_ERR("The cipher data length is wrong! datalen = 0x%x", datalen);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}
	tempindex += 4;
	//Cipher data
	memcpy(tempbuf + index, gApduBody.DATA + tempindex, datalen);
	index += datalen;
	tempindex += datalen;		
	if (tempindex != gApduBody.LC)
	{
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	//解密数据
	rdata = gApduResp.DATA + 4;
	rdatalen = index - 97;
	nRet = SM2_PriKeyDecrypt(&prv, rdata, &rdatalen, tempbuf, index);
	if (nRet != RT_OK)
	{
		DBG_ERR("SM2_PriKeyDecrypt() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_ASYMM_DEC_FAIL, 0);
		return;
	}
	tempindex = 0;
	gApduResp.DATA[tempindex] = rdatalen & 0x000000ff;
	gApduResp.DATA[tempindex + 1] = (rdatalen>>8) & 0x000000ff;
	gApduResp.DATA[tempindex + 2] = (rdatalen>>16) & 0x000000ff;
	gApduResp.DATA[tempindex + 3] = (rdatalen>>24) & 0x000000ff;
	tempindex = 4+rdatalen;

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, tempindex);
}
/***************************************************************************
* Subroutine:	CMD_ExtECCSign()
* Function:		Signature by extern ecc prv key
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExtECCSign()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 tempindex = 0;
	UINT16 ECCKeyBytes = 0;
    UINT32 signaturelen = 0;
	UINT32 ECCKeynBits = 0;
	UINT8  *data = NULL;
	UINT8 prvdata[32] = {0};
	SM2_PriKey prv = {0};
	UINT8 tempbuf[100] = {0};
	prv.PrivateKey = prvdata;
		
	DBG_FUNC_BEGIN;

	ECCKeynBits = (gApduBody.DATA[3]<<24) | (gApduBody.DATA[2]<<16) | (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if (ECCKeynBits != ECC_KEY_BITS)
	{
		DBG_ERR("ECC key pair bits length is wrong! ECCKeynBits = 0x%x", ECCKeynBits);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;	
	}

	//读取私钥苏局
	ECCKeyBytes = ECCKeynBits/8;
	tempindex = 4;
	memcpy(prv.PrivateKey, gApduBody.DATA + tempindex, ECCKeyBytes);
	tempindex += ECCKeyBytes;

	//读取数据长度和内容
	datalen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	if (datalen > 32)
	{
		DBG_ERR("Data length length is wrong! datalen = 0x%x", datalen);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	tempindex += 4;
	data = gApduBody.DATA + tempindex;
	tempindex += datalen;	
	if (tempindex != gApduBody.LC)
	{
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	
	//签名
	nRet = SM2_Sign(&prv, data, datalen, tempbuf, &signaturelen);
	if (nRet != RT_OK)
	{
		DBG_ERR("SM2_Sign() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_PRI_KEY_SIGN_FAIL, 0);
		return;
	}
	memcpy(gApduResp.DATA, tempbuf, signaturelen);

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, signaturelen);
}
/***************************************************************************
* Subroutine:	CMD_ExtECCVerify()
* Function:		Verify by extern ecc pub key
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExtECCVerify()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 tempindex = 0;
	UINT16 ECCKeyBytes = 0;
    UINT32 ECCKeynBits = 0;
	UINT8 data[32] = {0};
	UINT8 tempbuf[100] = {0};
	UINT8 pubdata[64] = {0};
	SM2_PubKey pub = {0};

	DBG_FUNC_BEGIN;

	pub.X = pubdata;
	pub.Y = pubdata + 32;	
	ECCKeynBits = (gApduBody.DATA[3]<<24) | (gApduBody.DATA[2]<<16) | (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
	if (ECCKeynBits != 256)
	{
		DBG_ERR("ECC key bits error! bits = 0x%x", ECCKeynBits);
		SetCmdResult(SAR_INDATALENERR,0);
		return;	
	}
	//读取公钥数据
	ECCKeyBytes = ECCKeynBits/8;
	tempindex = 4;
    memcpy(pub.X, gApduBody.DATA + tempindex, ECCKeyBytes);
    tempindex += ECCKeyBytes;
    memcpy(pub.Y, gApduBody.DATA + tempindex, ECCKeyBytes);
    tempindex += ECCKeyBytes;
	
	//读取数据长度和内容
    datalen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];	
    tempindex += 4;
    if (datalen > 32)   //只支持HASH数据
    {         
		DBG_ERR("HASH data length is wrong! datalen = 0x%x", datalen);
		SetCmdResult(SAR_INDATALENERR,0);
		return;	
    }
    memcpy(data, gApduBody.DATA + tempindex, datalen);
    tempindex += datalen;
	
	//读取签名
    memcpy(tempbuf, gApduBody.DATA + tempindex, ECCKeyBytes);
    tempindex += ECCKeyBytes;
    memcpy(tempbuf + ECCKeyBytes, gApduBody.DATA + tempindex, ECCKeyBytes);
    tempindex += ECCKeyBytes;
    if (tempindex != gApduBody.LC)
	{
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	//验证签名
	nRet = SM2_Verify(&pub, data, datalen, tempbuf, ECCKeyBytes*2);
	if (nRet != RT_OK)
	{
		DBG_ERR("SM2_Verify() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_SIGNVERYFIERR, 0);
		return;
	}	

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_GenerateAgreementDataWithECC()
* Function:		Generate agreement data for exchange key,operate by sponser
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GenerateAgreementDataWithECC()
{
	UINT16 nRet = SAR_OK;
	UINT16 appid = 0;
	UINT16 containerid = 0;
	UINT16 tempindex = 0;
	UINT32 AlgID = 0;
	UINT32 IDLen = 0;
	UINT32 ECCKeynBits = 0;
	UINT8 *IDdata = NULL;
	UINT8 pubdata[64] = {0};
	HAGREEMENTKEY AgreementKey = {0};    
	SM2_PubKey TempECCPubKey = {0};
	Ep_Point epSponsor = {0};
	UINT8 tempbuf[64] = {0};
    
	DBG_FUNC_BEGIN;
    memset(&epSponsor, 0, sizeof(Ep_Point));

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	AlgID = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	IDLen = (gApduBody.DATA[11]<<24) | (gApduBody.DATA[10]<<16) | (gApduBody.DATA[9]<<8) | gApduBody.DATA[8];
	IDdata = gApduBody.DATA + 12;
 
	//检查应用状态
	if (!GetAppStateInfoById(appid))
    {
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}				  
	FS_SelectFileByCluster(appid, appid);
	//检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
    {
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(containerid, containerid);
  			
	AgreementKeyFlag = 0;
    nRet = SM2_KeyExc_Init((UINT32*)tempbuf, &epSponsor);
	if (SAR_OK != nRet)
    {		
		DBG_ERR("SM2_KeyExc_Init() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEY_EXCHANGE_DATA_FAIL, 0); 
		return;
	}

	TempECCPubKey.X = pubdata;
	TempECCPubKey.Y = pubdata + 32;
	AgreementKeyFlag = 0;
    NN_Encode(TempECCPubKey.X, 32, epSponsor.x, 8);
    NN_Encode(TempECCPubKey.Y, 32, epSponsor.y, 8);
	ECCKeynBits = 256;

    memset(&AgreementKey, 0, sizeof(AgreementKey));
	memcpy(&AgreementKey.RandData, tempbuf, 32);
    memcpy(&AgreementKey.epSponsor, &epSponsor, sizeof(Ep_Point));
    AgreementKey.ulIDLen = IDLen;
    memcpy(AgreementKey.pbID, IDdata, IDLen);
	AgreementKey.hID = DEVICEID;
 	
    //Bits
	gApduResp.DATA[0] = ECCKeynBits & 0x000000ff;
	gApduResp.DATA[1] = (ECCKeynBits>>8) & 0x000000ff;
	gApduResp.DATA[2] = (ECCKeynBits>>16) & 0x000000ff;
	gApduResp.DATA[3] = (ECCKeynBits>>24) & 0x000000ff;
    //X
	memcpy(gApduResp.DATA + 4, TempECCPubKey.X, 32);
	tempindex = 4 + ECCKeynBits/8;
    //Y
	memcpy(gApduResp.DATA + tempindex, TempECCPubKey.Y, 32);
	tempindex += ECCKeynBits/8;
	//Handle
    gApduResp.DATA[tempindex] = AgreementKey.hID & 0x000000ff;
	gApduResp.DATA[tempindex + 1] = (AgreementKey.hID>>8) & 0x000000ff;
	gApduResp.DATA[tempindex + 2] = (AgreementKey.hID>>16) & 0x000000ff;
	gApduResp.DATA[tempindex + 3] = (AgreementKey.hID>>24) & 0x000000ff;
	tempindex += 4;

	SF_WriteData((UINT8*)&AgreementKey, HAGREEMENTKEY_OFFSET, HAGREEMENTKEY_SIZE);//将AgreementKey写入;
	AgreementKeyFlag = 1;

	DBG_FUNC_END;
    SetCmdResult(SAR_OK, tempindex);
}
/***************************************************************************
* Subroutine:	CMD_GenerateAgreementDataAndKeyWithECC()
* Function:		Use the sponser's agreement data to generate agreement data and key 
                for agreement key, operate by answer
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GenerateAgreementDataAndKeyWithECC()
{
	UINT8* pbSponsorID = NULL;
	UINT8* pbID = NULL;
	UINT16 nRet = 0;
	UINT16 appid = 0;
	UINT16 containerid = 0;
	UINT16 tempindex = 0;
	UINT16 ECCkeyBytes = 0;
	UINT16 ulSponsorIDLen = 0;
	UINT16 ulIDLen = 0;
	UINT16 SessionKeyID = 0;
	UINT32 Algflag = 0;
	UINT32 SponsorTempECCKeynBits = 0;
	UINT32 SponsorECCKeynBits = 0;
	UINT32 TempECCKeynBits = 0;
	UINT8  SessionKey[32] = {0};
	UINT8 SponsorTempECCPubKeydata[64] = {0};
	UINT8 SponsorECCPubKeydata[64] = {0};
	UINT8 TempECCPubKeydata[64] = {0};
	UINT8 prvdata[32] = {0};
	UINT8 pubdata[64] = {0};
	UINT8 TmpBuf[64];
	SM2_PubKey SponsorTempECCPubKey = {0};
	SM2_PubKey SponsorECCPubKey = {0};
	SM2_PubKey TempECCPubKey = {0};                 
    SM3Value *ZA = NULL;
	SM3Value *ZB = NULL;
	Ep_Point epResponse = {0};
	Ep_Point epSponser = {0}; 
	SM2_PriKey prv = {0};
    SM2_PubKey pub = {0};
    
	DBG_FUNC_BEGIN;

    memset(&epResponse, 0, sizeof(Ep_Point));
    memset(&epSponser, 0, sizeof(Ep_Point));
	ZA = (SM3Value*)TmpBuf;
	ZB = (SM3Value*)(TmpBuf + 32);
	SponsorTempECCPubKey.X = SponsorTempECCPubKeydata;
	SponsorTempECCPubKey.Y = SponsorTempECCPubKeydata + 32;  
	SponsorECCPubKey.X = SponsorECCPubKeydata;
	SponsorECCPubKey.Y = SponsorECCPubKeydata + 32;  
	TempECCPubKey.X = TempECCPubKeydata;
	TempECCPubKey.Y = TempECCPubKeydata + 32;  
	prv.PrivateKey = prvdata; 
	pub.X = pubdata;
	pub.Y = pubdata + 32;  
    
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	Algflag = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	SponsorECCKeynBits = (gApduBody.DATA[11]<<24) | (gApduBody.DATA[10]<<16) | (gApduBody.DATA[9]<<8) | gApduBody.DATA[8];
	//
	tempindex = 12;
	ECCkeyBytes = SponsorECCKeynBits/8;
	memcpy(SponsorECCPubKey.X, gApduBody.DATA + tempindex, ECCkeyBytes);
	tempindex += ECCkeyBytes;
	memcpy(SponsorECCPubKey.Y, gApduBody.DATA + tempindex, ECCkeyBytes);
	tempindex += ECCkeyBytes;
	//
	SponsorTempECCKeynBits = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	ECCkeyBytes = SponsorTempECCKeynBits/8;
	tempindex += 4;
	memcpy(SponsorTempECCPubKey.X, gApduBody.DATA + tempindex, ECCkeyBytes);
	tempindex += ECCkeyBytes;
	memcpy(SponsorTempECCPubKey.Y, gApduBody.DATA + tempindex, ECCkeyBytes);
	tempindex += ECCkeyBytes;	
	//
	ulSponsorIDLen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
	pbSponsorID = gApduBody.DATA + tempindex;
	tempindex += ulSponsorIDLen;
	//
	ulIDLen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
	pbID = gApduBody.DATA + tempindex;
	tempindex += ulIDLen;
    if (tempindex != gApduBody.LC)
	{
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	//检查应用状态
 	if (!GetAppStateInfoById(appid))
    {
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}				  
	FS_SelectFileByCluster(appid, appid);
	//检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
    {
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(containerid, containerid);    
    
	//读取签名密钥对
    nRet = FS_SelectFileByID(SIGNKEYID);
	if(nRet != RT_OK)
    {
		DBG_ERR("FS_SelectFileByID(SIGNKEYID) failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	nRet = FS_ReadFile((UINT8*)prv.PrivateKey, 0, ECCkeyBytes);
	nRet |= FS_ReadFile((UINT8*)pub.X, ECCkeyBytes, ECCkeyBytes*2);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_SelectFileByID(SIGNKEYID) failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}

    NN_Decode(epSponser.x, 8, SponsorTempECCPubKey.X, 32);  
    NN_Decode(epSponser.y, 8, SponsorTempECCPubKey.Y, 32);
    SM3_GetSignIDValue(ZA, &SponsorECCPubKey, pbSponsorID, ulSponsorIDLen);
    SM3_GetSignIDValue(ZB, &pub, pbID, ulIDLen);
    nRet = SM2_KeyExc_Response(&epSponser, &prv, &SponsorECCPubKey, &epResponse, NULL, NULL, ZA, ZB, SessionKey, 16);
	if (nRet != RT_OK)
	{
		DBG_ERR("SM2_KeyExc_Response() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEY_EXCHANGE_DATA_FAIL, 0);
		return;
	}
		
	SessionKeyID = SaveSessionKey((UINT8*)SessionKey, 32, Algflag);
	if (SessionKeyID == SAR_NO_ROOM)
    {
		DBG_ERR("The session count meet the max!");
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	}
    NN_Encode(TempECCPubKey.X, 32, epResponse.x, 8); 
    NN_Encode(TempECCPubKey.Y, 32, epResponse.y, 8);
	TempECCKeynBits = SponsorECCKeynBits;  

    //Bits
    gApduResp.DATA[0] = TempECCKeynBits & 0x000000ff;
	gApduResp.DATA[1] = (TempECCKeynBits>>8) & 0x000000ff;
	gApduResp.DATA[2] = (TempECCKeynBits>>16) & 0x000000ff;
	gApduResp.DATA[3] = (TempECCKeynBits>>24) & 0x000000ff;
    //X
	memcpy(gApduResp.DATA+4, TempECCPubKey.X, TempECCKeynBits/8);
	tempindex = 4 + TempECCKeynBits/8;
    //Y
	memcpy(gApduResp.DATA+tempindex, TempECCPubKey.Y, TempECCKeynBits/8);
	tempindex += TempECCKeynBits/8;
    //Session key ID
    gApduResp.DATA[tempindex] = SessionKeyID & 0x000000ff;
	gApduResp.DATA[tempindex + 1] = (SessionKeyID>>8) & 0x000000ff;
	gApduResp.DATA[tempindex + 2] = (SessionKeyID>>16) & 0x000000ff;
	gApduResp.DATA[tempindex + 3] = (SessionKeyID>>24) & 0x000000ff;
	tempindex += 4;
	
	DBG_FUNC_END;
    SetCmdResult(SAR_OK, tempindex);
}
/***************************************************************************
* Subroutine:	CMD_GenerateKeyWithECC()
* Function:		Generate agreement key, operate by sponser
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_GenerateKeyWithECC()
{  
    UINT32 ulResponserIDLen = 0;
	UINT32 AgreementKeyID = 0;
	UINT32 sm2ResponseKeynBits = 0;
	UINT32 sm2TempResponseKeynBits = 0;
	UINT8* pbResponserID = NULL;
	UINT16 nRet = 0;
	UINT32 appid = 0;
	UINT32 containerid = 0;
	UINT32 SessionID = 0;
	UINT32 tempindex = 0;
	UINT32 ECCkeyBytes = 0;
	UINT8 SessionKey[32] = {0};
	UINT8 sm2ResponsePubKeydata[64] = {0};
	UINT8 sm2TempResponsePubKeydata[64] = {0};
	UINT8 prvdata[32] = {0};
	UINT8 pubdata[64] = {0};
	UINT8 TmpBuf[64] = {0};
    SM3Value *ZA = NULL, *ZB = NULL; 
    HAGREEMENTKEY AgreementKey = {0};    
    Ep_Point epResponser = {0};
	SM2_PubKey sm2ResponsePubKey = {0};      
    SM2_PubKey sm2TempResponsePubKey = {0};
    SM2_PriKey prv = {0};
    SM2_PubKey pub = {0};
       
	DBG_FUNC_BEGIN;

    memset(&epResponser, 0, sizeof(Ep_Point));               
	ZA = (SM3Value*)TmpBuf;
	ZB = (SM3Value*)(TmpBuf + 32);	
	sm2ResponsePubKey.X = sm2ResponsePubKeydata;
	sm2ResponsePubKey.Y = sm2ResponsePubKeydata + 32;  
	sm2TempResponsePubKey.X = sm2TempResponsePubKeydata;
	sm2TempResponsePubKey.Y = sm2TempResponsePubKeydata + 32;    
	prv.PrivateKey = prvdata; 
	pub.X = pubdata;
	pub.Y = pubdata + 32;
	
	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	//Agreement key ID
	AgreementKeyID = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	//Response bits
	sm2ResponseKeynBits = (gApduBody.DATA[11]<<24) | (gApduBody.DATA[10]<<16) | (gApduBody.DATA[9]<<8) | gApduBody.DATA[8];
	tempindex = 12;
	ECCkeyBytes = sm2ResponseKeynBits / 8;
	//Response X
	memcpy(sm2ResponsePubKey.X, gApduBody.DATA + tempindex, ECCkeyBytes);
	tempindex += ECCkeyBytes;
	//Response Y
	memcpy(sm2ResponsePubKey.Y, gApduBody.DATA + tempindex, ECCkeyBytes);
	tempindex += ECCkeyBytes;
	//Response temp bits
	sm2TempResponseKeynBits = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	ECCkeyBytes = sm2TempResponseKeynBits/8;
	tempindex += 4;
	//Response temp X
	memcpy(sm2TempResponsePubKey.X, gApduBody.DATA + tempindex, ECCkeyBytes);
	tempindex += ECCkeyBytes;
	//Response temp Y
	memcpy(sm2TempResponsePubKey.Y, gApduBody.DATA + tempindex, ECCkeyBytes);
	tempindex += ECCkeyBytes;
	//Response ID length
	ulResponserIDLen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
	pbResponserID = gApduBody.DATA + tempindex;
	tempindex += ulResponserIDLen;
    if (tempindex != gApduBody.LC)
	{
		DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}

	//检查应用状态
 	if (!GetAppStateInfoById(appid))
    {
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}				  
	FS_SelectFileByCluster(appid,appid);
	//检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
    {
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(containerid, containerid); 

	//读取Agreement key data
    memset((UINT8*)&AgreementKey, 0, sizeof(AgreementKey));
	if (AgreementKeyFlag == 1)//AgreementKey 
	{
		SF_ReadData((UINT8*)&AgreementKey, HAGREEMENTKEY_OFFSET, HAGREEMENTKEY_SIZE);//read AgreementKey;
	}			
    if (AgreementKeyID != AgreementKey.hID)
    {
		DBG_ERR("Agreement key ID is wrong! AgreementKey.hID = 0x%x, AgreementKeyID = 0x%x", AgreementKey.hID, AgreementKeyID);
        SetCmdResult(SAR_INVALIDPARAMERR, 0);
    }
    
	//读取签名密钥对
    nRet = FS_SelectFileByID(SIGNKEYID);
	if (nRet != RT_OK)
    {
		DBG_ERR("FS_SelectFileByID(SIGNKEYID) failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
		return;
	}
	nRet = FS_ReadFile((UINT8*)prv.PrivateKey, 0, ECCkeyBytes);
	nRet |= FS_ReadFile((UINT8*)pub.X, ECCkeyBytes, ECCkeyBytes*2);
    if (nRet != RT_OK)
	{
		DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}      
  	
	//保存会话密钥
    NN_Decode(epResponser.x, 8, sm2TempResponsePubKey.X, 32);
    NN_Decode(epResponser.y, 8, sm2TempResponsePubKey.Y, 32); 
    SM3_GetSignIDValue(ZA, &pub, AgreementKey.pbID, AgreementKey.ulIDLen);
    SM3_GetSignIDValue(ZB, &sm2ResponsePubKey, pbResponserID, ulResponserIDLen);
	nRet = SM2_KeyExc_Spons(AgreementKey.RandData, &AgreementKey.epSponsor, &epResponser,NULL, &prv, &sm2ResponsePubKey, NULL, ZA, ZB, SessionKey, 16);
	if (SAR_OK != nRet)
    {    
		DBG_ERR("SM2_KeyExc_Spons() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEY_EXCHANGE_DATA_FAIL, 0);
		return;
    }
    SessionID = SaveSessionKey((UINT8*)SessionKey, 32, SGD_SM1_CBC);
	if (SessionID == SAR_NO_ROOM)
    {
		DBG_ERR("The session key count met the max!");
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	}
    
	gApduResp.DATA[0] = SessionID & 0x000000ff;
	gApduResp.DATA[1] = (SessionID >> 8) & 0x000000ff;
	gApduResp.DATA[2] = (SessionID >> 16) & 0x000000ff;
	gApduResp.DATA[3] = (SessionID >> 24) & 0x000000ff;
	
	DBG_FUNC_END;
    SetCmdResult(SAR_OK, 4);
}
#endif
/***************************************************************************
* Subroutine:	CMD_ExportPublicKey()
* Function:		Export pub key 
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ExportPublicKey()
{
	UINT16 nRet = 0;
	UINT16 appid = 0;
	UINT16 containerid = 0;
	UINT16 ECCKeybytes = 0;
	UINT16 tempindex = 0;
	UINT32 ECCKeynBits = 0;
	tFileNode Node = {0};
	
	DBG_FUNC_BEGIN;

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);  
	if ((gApduBody.LC & 0xFF) != 4)
	{
		DBG_ERR("LC is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INVALIDPARAMERR, 0);
		return;
	}
	//检查应用状态
	if (!GetAppStateInfoById(appid))
    {
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}      
	FS_SelectFileByCluster(appid, appid);
	//检查容器ID
	if (SAR_OK != IsContainerIdValid(appid, containerid))
    {
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	FS_SelectFileByCluster(containerid, containerid);

	//读取容器类型
	nRet = FS_ReadDirNodeInfo((UINT8*)&Node);      
    if (nRet != RT_OK)
    {
		DBG_ERR("FS_ReadDirNodeInfo() failed! nRet = 0x%x", nRet);
    	SetCmdResult(SAR_INDATAERR, 0);
    	return;    	
    }    	
	if (gApduHeader.P1 == 0x01)
    {
		nRet = FS_SelectFileByID(SIGNKEYID);
	}
	else if (gApduHeader.P1 == 0x00)
	{
		nRet = FS_SelectFileByID(EXKEYID);
	}
	if (nRet != RT_OK)
    {
		DBG_ERR("FS_SelectFileByID() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
		return;
	}

	//导出ECC公钥
    if (Node.ContainerType == ECC_CON)
    {
		SM2_PubKey pub = {0};
		ECCKeynBits = ECC_KEY_BITS;
		ECCKeybytes = ECCKeynBits/8;
		pub.X = gApduBody.DATA + 4;
		pub.Y =  gApduBody.DATA + 4 + ECCKeybytes;        
    	nRet = FS_ReadFile((UINT8*)pub.X, ECCKeybytes, ECCKeybytes*2);
    	if (nRet != RT_OK)
    	{
			DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
    		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
    		return;
    	}

    	gApduResp.DATA[0] = ECCKeynBits & 0x000000ff;
    	gApduResp.DATA[1] = (ECCKeynBits>>8) & 0x000000ff;
    	gApduResp.DATA[2] = (ECCKeynBits>>16) & 0x000000ff;
    	gApduResp.DATA[3] = (ECCKeynBits>>24) & 0x000000ff;
    	tempindex = 4;
    	memcpy(gApduResp.DATA + tempindex, pub.X, ECCKeybytes);
    	tempindex += ECCKeybytes;
    	memcpy(gApduResp.DATA + tempindex, pub.Y, ECCKeybytes);
    	tempindex += ECCKeybytes;

		DBG_FUNC_END;
    	SetCmdResult(SAR_OK, tempindex);	
        return;	    	
    }    	
	//导出RSA公钥
    else if (Node.ContainerType == RSA_CON)
    {
        UINT16 nByteLen = 0;
		UINT8* RSAKeydata = gApduBody.DATA + 300;
    	nRet = FS_ReadFile((UINT8*)RSAKeydata, 0, 2);
		nByteLen = (RSAKeydata[0] + (RSAKeydata[1] << 8)) / 8;
		nRet |= FS_ReadFile((UINT8*)RSAKeydata, 0, nByteLen+6);
        if (nRet != RT_OK)
        {  
			DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
    		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
        	return;
        }         
        memcpy(gApduResp.DATA, RSAKeydata, 2);   
		memcpy(gApduResp.DATA + 2, RSAKeydata+2+nByteLen, 4);
        memcpy(gApduResp.DATA + 6, RSAKeydata+2, nByteLen);

		DBG_FUNC_END;
        SetCmdResult(SAR_OK, 6 + nByteLen);
        return;
    }    
	
	DBG_ERR("Can't find the public key file!");
    SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
}
/***************************************************************************
* Subroutine:	CMD_ImportSessionKey()
* Function:		Import encrypted sessionkey
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ImportSessionKey()
{
	UINT16 nRet = 0;
	UINT16 nStatus = 0;
	UINT16 appID = 0;
	UINT16 containerID = 0;
	UINT16 tempindex = 0;
	UINT16 SessionKeyID = 0;
	UINT32 AlgID = 0;
	UINT32 SessionCypherLen = 0;
	UINT32 rlen = 0;
	UINT8 *cipher = NULL;
	UINT8* tempbuf = NULL;
    UINT8 key[48] = {0};
	tFileNode Node = {0}; 
	APPSTATE* AppState = NULL;

	DBG_FUNC_BEGIN;

	appID = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerID = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	tempindex = 4;
	AlgID = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
	SessionCypherLen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
    tempindex += 4;
	//memcpy(cipher, gApduBody.DATA + tempindex, SessionCypherLen);
    cipher = gApduBody.DATA + tempindex;
	tempindex += SessionCypherLen;
    if (tempindex != gApduBody.LC)
    {             
		DBG_ERR("LC is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
    }  
	//检查应用状态
	AppState = GetAppStateInfoById(appID);
	if (!AppState)
    {
		DBG_ERR("This application is not opened! appid = 0x%x", appID);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	gCurSAStatus = AppState->AppState;
	//检查登录状态
    FS_SelectFileByCluster(appID, appID);
	nStatus = IsHasControl(SA_USERLOGIN, SA_SOLOGIN, CON_ALL);
	if(nStatus != SAR_OK)
	{
		DBG_ERR("The user is not login!");
		SetCmdResult(nStatus, 0);
    	return;
	}
	//检查容器ID
	if (SAR_OK != IsContainerIdValid(appID, containerID))
	{
		DBG_ERR("This container is not opened! containerid = 0x%x", containerID);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
	nRet =  FS_SelectFileByCluster(containerID, containerID);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_SelectFileByCluster() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}
	//读取容器类型
	nRet = FS_ReadDirNodeInfo((UINT8*)&Node);
	if (nRet != RT_OK)
	{
		DBG_ERR("FS_ReadDirNodeInfo() failed! nRet = 0x%x", nRet);
		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
		return;
	}

	//RSA容器
	if (Node.ContainerType == RSA_CON)
	{
		UINT16 index = 0;
		UINT16 keybitlen = 0;
		UINT16 nByteLen = 0;
		UINT16 nPrimeLen = 0;
		UINT8* RSAKeydata = NULL;
        UINT8* KeyCipherData = gApduResp.DATA;       //KeyCipherData必须为4的整数倍
		R_RSA_KEYPAIR RSAKey = {0};
		//以下所有变量，必须按四字节对齐 
		__align(4) UINT8 n[256], p[128], q[128], dp[128], dq[128], qinv[128]; 

		//读取加密私钥
		FS_SelectFileByCluster(containerID, containerID);
		nRet = FS_SelectFileByID(EXKEYID);
		if (nRet != RT_OK)
        {
			DBG_ERR("FS_SelectFileByID(EXKEYID) failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
			return;
		}
	   	RSAKeydata = gApduBody.DATA + tempindex;
		nRet = FS_ReadFile(RSAKeydata, 0, 2);
		keybitlen = (RSAKeydata[1]<<8) | RSAKeydata[0];
		nByteLen = keybitlen/8;
		nPrimeLen = keybitlen/16;
		nRet |= FS_ReadFile(RSAKeydata, 0, nByteLen*2+nPrimeLen*5+6);
		if (nRet != RT_OK)
		{
			DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_FILE_NOT_EXIST, 0);
			return;
		}
		RSAKey.modulus = n;
		RSAKey.publicExponent =	NULL;
		RSAKey.exponent = NULL;
		RSAKey.prime[0] = p;
		RSAKey.prime[1] = q;
		RSAKey.primeExponent[0] = dp;
		RSAKey.primeExponent[1] = dq;
		RSAKey.coefficient = qinv;
		index = 0;
		memcpy((UINT8*)&RSAKey.bits, RSAKeydata, 2);
		index += 2;
		memcpy(RSAKey.modulus, RSAKeydata+index, nByteLen);
		index += nByteLen;
		//memcpy(RSAKey.publicExponent, RSAKeydata+index, 4);
		index += 4;
		//memcpy(RSAKey.exponent, RSAKeydata+index, nByteLen);
		index += nByteLen;
    	memcpy(RSAKey.prime[0], RSAKeydata+index, nPrimeLen);
		index += nPrimeLen;
    	memcpy(RSAKey.prime[1], RSAKeydata+index, nPrimeLen);
		index += nPrimeLen;
    	memcpy(RSAKey.primeExponent[0], RSAKeydata+index, nPrimeLen);
		index += nPrimeLen;
		memcpy(RSAKey.primeExponent[1], RSAKeydata+index, nPrimeLen); 
		index += nPrimeLen;
		memcpy(RSAKey.coefficient, RSAKeydata+index, nPrimeLen);
		index += nPrimeLen;
        memcpy(KeyCipherData, cipher, SessionCypherLen);
        nRet = RSA_PrivateDecrypt(key, &rlen, KeyCipherData, SessionCypherLen, (R_RSA_PRIVATE_KEY*)&RSAKey);
		if (nRet != RT_OK)
        {
			DBG_ERR("RSA_PrivateDecrypt() failed! nRet = 0x%x", nRet);
			SetCmdResult(SAR_ASYMM_DEC_FAIL, 0);
			return;
		}
	}
	//ECC容器
	else if (Node.ContainerType == ECC_CON)
	{
		SM2_PriKey prv = {0};
		UINT8 prvdata[32] = {0};
        UINT16 ECCkeybytes = 0;
		UINT16 index = 0;
        UINT32 cipherlen = 0;
        UINT8 *hash = NULL;
		UINT8 *x = NULL;
		UINT8 *y = NULL;

        //Bits
	    ECCkeybytes = ((cipher[3]<<24)|(cipher[2]<<16)|(cipher[1]<<8)|cipher[tempindex]) / 8;
        index = 4;
        //X
	    x = cipher + index;
        index += ECCkeybytes;
        //Y
	    y = cipher + index;
        index += ECCkeybytes;
        //HASH
        hash = cipher + index;
        index += 32;
        //Cipher len
	    cipherlen = (cipher[index+3]<<24)|(cipher[index+2]<<16)|(cipher[index+1]<<8)|cipher[index];
	    index += 4;
	    if (cipherlen > 32)
        {
			DBG_ERR("Cipher data length is wrong! cipherlen = 0x%x", cipherlen);
	       	SetCmdResult(SAR_INDATALENERR, 0);
		    return;		
	    }
        //Cipher
	    cipher = cipher + index;
	    index += cipherlen;
	
		//读取加密私钥
    	nRet = FS_SelectFileByCluster(containerID, containerID);
    	nRet |= FS_SelectFileByID(EXKEYID);
    	if (nRet != RT_OK)
        {
			DBG_ERR("FS_SelectFileByID(EXKEYID) failed! nRet = 0x%x", nRet);
    		SetCmdResult(SAR_KEYNOTFOUNTERR, 0);
    		return;
    	}
		prv.PrivateKey = prvdata;
    	nRet = FS_ReadFile((UINT8*)prv.PrivateKey, 0, ECC_KEY_BITS/8);
    	if (nRet != RT_OK)
    	{
			DBG_ERR("FS_ReadFile() failed! nRet = 0x%x", nRet);
    		SetCmdResult(SAR_FILE_NOT_EXIST, 0);
    		return;
    	}
		tempbuf = gApduBody.DATA + 512;
       	tempbuf[0] = 0x04;
    	index = 1;
    	memcpy(tempbuf+index, x, ECCkeybytes);
    	index += ECCkeybytes;
    	memcpy(tempbuf+index, y, ECCkeybytes);
    	index += ECCkeybytes;
    	memcpy(tempbuf+index, hash, 32);
    	index += 32;
    	memcpy(tempbuf+index, cipher, cipherlen);
    	index += cipherlen;
		rlen = index + 16; 
    	nRet = SM2_PriKeyDecrypt(&prv, key, &rlen, tempbuf, index);
	    if (nRet != RT_OK)
        {
			DBG_ERR("SM2_PriKeyDecrypt() failed! nRet = 0x%x", nRet);
    		SetCmdResult(SAR_ASYMM_DEC_FAIL, 0);
    		return;
    	}
	}

	//保存会话密钥
	SessionKeyID = SaveSessionKey(key, rlen, AlgID);
	if (SessionKeyID == SAR_NO_ROOM)
    {
		DBG_ERR("The session key count meets the max!");
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	}

	gApduResp.DATA[0] = SessionKeyID & 0x00FF;
	gApduResp.DATA[1] = (SessionKeyID>>8) & 0x00FF; 

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, 2);		
}
/***************************************************************************
* Subroutine:	CMD_GenSessionKey()
* Function:		Generate sessionkey (AES key) and return key id
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/		
void CMD_GenSessionKey()
{
	UINT16 appid = 0;
	UINT16 containerid = 0;
	UINT16 tempindex = 0;
	UINT16 SessionKeyID = 0;
	UINT32 AlgID = 0;
	UINT32 keylen = 0;
	UINT8 tempbuf[32] = {0};

	DBG_FUNC_BEGIN;

	appid = ((gApduBody.DATA[1]<<8) | gApduBody.DATA[0]);
	containerid = ((gApduBody.DATA[3]<<8) | gApduBody.DATA[2]);
	AlgID = (gApduBody.DATA[7]<<24) | (gApduBody.DATA[6]<<16) | (gApduBody.DATA[5]<<8) | gApduBody.DATA[4];
	tempindex = 8;
	if (tempindex != gApduBody.LC)
	{
		DBG_ERR("The input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	if (!GetAppStateInfoById(appid))
	{
		DBG_ERR("This application is not opened! appid = 0x%x", appid);
		SetCmdResult(SAR_APPLICATION_NOT_EXISTS, 0);
		return;
	}
	if (SAR_OK != IsContainerIdValid(appid,containerid))
	{
		DBG_ERR("This container is not opened! containerid = 0x%x", containerid);
		SetCmdResult(SAR_CONTAINERID_NOT_EXISTS, 0);
		return;
	}
    
	//生成会话密钥
	switch(AlgID)
	{
	case SGD_SM1_ECB:
	case SGD_SM1_CBC: 
	case SGD_AES256_ECB:
	case SGD_AES256_CBC:
		 Trng_GenRandom(tempbuf, DEFAULTSESSIONKEYLEN);
		 keylen = DEFAULTSESSIONKEYLEN;
		 break;
	case SGD_SMS4_ECB:
	case SGD_SMS4_CBC:	
	case SGD_AES128_ECB:
	case SGD_AES128_CBC:
		 Trng_GenRandom(tempbuf, 16);
		 keylen = 16;
		 break;	  
	case SGD_AES192_ECB:
	case SGD_AES192_CBC: 
		 Trng_GenRandom(tempbuf, 24);
		 keylen = 24;
		 break;	
	default:
		 DBG_ERR("The sessison key alg is not support! AlgID = 0x%x", AlgID);
		 SetCmdResult(SAR_UNSUPPORT_SESSION_KEY, 0);
		 return;
	}
	//保存会话密钥数据
	SessionKeyID = SaveSessionKey(tempbuf, keylen, AlgID);
	if (SessionKeyID == SAR_NO_ROOM)
	{
		DBG_ERR("The sessison count is meet the max!");
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	}

	tempindex = 0;
	gApduResp.DATA[tempindex] = SessionKeyID & 0x00ff;
	gApduResp.DATA[tempindex + 1] = (SessionKeyID>>8) & 0x00ff;
	tempindex += 2;

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, tempindex);
}	
/***************************************************************************
* Subroutine:	CMD_ImportSymmKey()
* Function:		Import plain sessionkey
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_ImportSymmKey()
{
    UINT16 SessionkeyId = 0;
	UINT32 AlgID = 0;
    UINT32 tempindex = 0;
	UINT32 SessionkeyLen = 0;

	DBG_FUNC_BEGIN;
	
    tempindex = 4;  //Shift 2Bytes appid and 2Bytes containerid
	AlgID = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 4;
    SessionkeyLen = (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 2;
	if (SessionkeyLen > 32)
	{
		DBG_ERR("The sessison key length is wrong! SessionkeyLen = 0x%x", SessionkeyLen);
		SetCmdResult(SAR_INDATALENERR, 0);
		return;
	}
	SessionkeyId = SaveSessionKey(gApduBody.DATA + tempindex, SessionkeyLen, AlgID);
	if (SessionkeyId == SAR_NO_ROOM)
	{
		DBG_ERR("The sessison count is meet the max!");
		SetCmdResult(SAR_NO_ROOM, 0);
		return;
	}

	gApduResp.DATA[0] = SessionkeyId & 0x00FF;
	gApduResp.DATA[1] = (SessionkeyId>>8) & 0x00FF;

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, 2);
}
/***************************************************************************
* Subroutine:	CMD_EncryptDecryptInit()
* Function:		Init for symmetric encrypt or decrypt
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
UINT16 CMD_EncryptDecryptInit(CRYPTO_TYPE crypto_type)
{
	UINT16 keyid = 0;
	UINT16 tempindex = 0;
	UINT16 algId = 0;
	BLOCKCIPHERPARAM pBlockCipher = {0};

	DBG_FUNC_BEGIN;

	tempindex = 4;//shift 2B for AppID, 2B for ContID
	keyid = (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
	tempindex += 2;
    algId = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
    tempindex += 4;
	//读取BLOCKCIPHERPARAM
    memcpy((UINT8*)&pBlockCipher, gApduBody.DATA + tempindex, sizeof(BLOCKCIPHERPARAM));
	if (pBlockCipher.IVLen > 32)
	{
		return SAR_INDATAERR;
	}
    tempindex += sizeof(BLOCKCIPHERPARAM);
	if (tempindex != gApduBody.LC)
	{
		DBG_ERR("The input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
		return SAR_INDATALENERR;
	}

	//检查Key id 和 alg id的合法性
	if (keyid >= SESSIONKEY_MAX)
	{
		DBG_ERR("Session key id is wrong! keyid = 0x%x", keyid);
		return SAR_INDATAERR;
	}
	if (gSessionKey[keyid].algID != algId)
	{
		DBG_ERR("The algID of this session key is wrong! Session key ID = 0x%x, alg ID = 0x%x, input alg ID = 0x%x", keyid, gSessionKey[keyid].algID, algId);
		return SAR_INDATAERR;
	}

	//初始化会话密钥数据
	switch(gSessionKey[keyid].algID)
	{
    case SGD_SM1_MAC:
	case SGD_SM1_CBC:
		 memcpy(gSessionKey[keyid].IV, pBlockCipher.IV, pBlockCipher.IVLen);
	case SGD_SM1_ECB:
		 SM1_Init(gSessionKey[keyid].keyData, gSessionKey[keyid].keyData + 16);
		 break;		
    case SGD_SMS4_MAC:	 
	case SGD_SMS4_CBC:
		 memcpy(gSessionKey[keyid].IV, pBlockCipher.IV, pBlockCipher.IVLen);
	case SGD_SMS4_ECB:
		 SMS4_Init(gSessionKey[keyid].keyData);
		 break;
	case SGD_AES128_ECB:
		 Aes_Init(gSessionKey[keyid].keyData, 16);
		 memcpy(gSessionKey[keyid].extData, (void*)SESSIONKEY_EXTDATA_ADDR, SESSIONKEY_EXTDATA_LEN);
		 break;
	case SGD_AES192_ECB:
		 Aes_Init(gSessionKey[keyid].keyData, 24);
		 memcpy(gSessionKey[keyid].extData, (void*)SESSIONKEY_EXTDATA_ADDR, SESSIONKEY_EXTDATA_LEN);
		 break;
	case SGD_AES256_ECB:
		 Aes_Init(gSessionKey[keyid].keyData, 32);
		 memcpy(gSessionKey[keyid].extData, (void*)SESSIONKEY_EXTDATA_ADDR, SESSIONKEY_EXTDATA_LEN);	
		 break;
	case SGD_SSF33_ECB:
		 SSF33_Init(gSessionKey[keyid].keyData);	
		 break;
	case SGD_AES128_CBC: 
	case SGD_AES192_CBC:
	case SGD_AES256_CBC:
	default:
		 DBG_ERR("Session key id is not supported! algID = 0x%x", gSessionKey[keyid].algID);
		 return SAR_UNSUPPORT_SESSION_KEY;
	}
	gLastSessionKeyID = gSessionKey[keyid].keyID;
    gSessionKey[keyid].crypto_type = crypto_type;
    gSessionKey[keyid].flag = 2;

	DBG_FUNC_END;
	return SAR_OK;
}
/***************************************************************************
* Subroutine:	CMD_EncryptDecryptUpdate()
* Function:		Symmtric encrypt or decrypt
* Input:        flag -- 需要check的类型
*				pDataLen -- 输入的数据地址
* Output:		nRet -- 返回结果
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
UINT16 CMD_EncryptDecryptUpdate(CRYPTO_TYPE flag, UINT16* pDataLen)
{
	UINT16 nRet = SAR_OK;
	UINT16 keyid = 0;
	UINT16 tempindex = 0;
	UINT8 *data = gSDSIBuffAddr;	//待处理的数据地址必须是SDSI的地址空间，且首地址必须为4的整数倍!!!

    DBG_FUNC_BEGIN;	

	if (!pDataLen)
	{
        DBG_ERR("pDataLen is NULL!");
		nRet = SAR_INDATAERR;
		goto errExit;
	}

	tempindex = 4;//shift 2B for AppID, 2B for ContID
	keyid = ((gApduBody.DATA[5]<<8) | gApduBody.DATA[4]);
	tempindex += 2;

	//检查数据长度
	*pDataLen = gApduBody.LC - tempindex;
	if (*pDataLen % 16)
	{
		*pDataLen = 0;	
		nRet = SAR_INDATALENERR;  
        DBG_ERR("Data length is wrong! DataLen = 0x%x", *pDataLen);
		goto errExit;
	}
	memcpy(data, gApduBody.DATA + tempindex, *pDataLen);

	//检查会话密钥ID
	if (keyid >= SESSIONKEY_MAX)
	{	
		DBG_ERR("The key ID is wrong! keyid = 0x%x", keyid);
		nRet = SAR_INDATAERR;
		goto errExit;
	}

	//会话密钥在交替使用，需要重新初始化密钥
	if (keyid != gLastSessionKeyID)
	{
		//DBG_INFO("The last key ID=0x%x,new using key id=0x%x",mLastSessionKeyID,keyid);
		switch(gSessionKey[keyid].algID)
		{	 
	    case SGD_SM1_MAC:
		case SGD_SM1_CBC:
		case SGD_SM1_ECB: 
			SM1_Init(gSessionKey[keyid].keyData, gSessionKey[keyid].keyData + 16);
			break;
		case SGD_SMS4_ECB:
			SMS4_Init(gSessionKey[keyid].keyData);
			break;	 
		case SGD_SSF33_ECB:
  			SSF33_Init(gSessionKey[keyid].keyData);	
			break;
		case SGD_AES128_ECB: 
		case SGD_AES192_ECB:
		case SGD_AES256_ECB:
			memcpy((void*)SESSIONKEY_EXTDATA_ADDR, gSessionKey[keyid].extData, SESSIONKEY_EXTDATA_LEN);
			break;		  
		default:
			DBG_ERR("Session key alg ID is not supported! algID = 0x%x", gSessionKey[keyid].algID);
			nRet = SAR_UNSUPPORT_SESSION_KEY;
			goto errExit;
		}
		gLastSessionKeyID = keyid;
	}

	//加密数据
	switch(gSessionKey[keyid].algID)
	{
	case SGD_SM1_ECB:
		_SM_Run(flag != CT_DEC ? SM1_ENCRYPT : SM1_DECRYPT, (UINT32)data, *pDataLen);
		break;
    case SGD_SM1_MAC:
	case SGD_SM1_CBC:
		SM1_Run(flag != CT_DEC ? SM1_ENCRYPT : SM1_DECRYPT, SM1_CBC, data, data, *pDataLen, gSessionKey[keyid].IV);
		break;
	case SGD_SMS4_MAC:
	case SGD_SMS4_CBC: 
		SMS4_Run(flag != CT_DEC ? SMS4_ENCRYPT:SMS4_DECRYPT, SMS4_CBC, data, data, *pDataLen, gSessionKey[keyid].IV);
		break;
	case SGD_SMS4_ECB:
		SMS4_Run(flag != CT_DEC ? SMS4_ENCRYPT:SMS4_DECRYPT, SMS4_ECB, data, data, *pDataLen, NULL);
		break;
	case SGD_SSF33_ECB:
		SSF33_Run(flag != CT_DEC ? SSF33_ENCRYPT:SSF33_DECRYPT, SSF33_ECB, data, data, *pDataLen, NULL);
		break;
	case SGD_AES128_ECB:
		_Aes_Run(flag != CT_DEC ? AES_ENCRYPT:AES_DECRYPT, AES128_ECB, (UINT32)data, *pDataLen);
		break;
	case SGD_AES192_ECB:
		_Aes_Run(flag != CT_DEC ? AES_ENCRYPT:AES_DECRYPT, AES192_ECB, (UINT32)data, *pDataLen);
		break;
	case SGD_AES256_ECB:
		_Aes_Run(flag != CT_DEC ? AES_ENCRYPT:AES_DECRYPT, AES256_ECB, (UINT32)data, *pDataLen);
		break;
	default:
		DBG_ERR("Session key alg ID is not supported! algID = 0x%x", gSessionKey[keyid].algID);
		nRet = SAR_UNSUPPORT_SESSION_KEY;
	}
    memcpy(gApduResp.DATA, data, *pDataLen);

errExit:
    DBG_FUNC_END;
	return nRet;
}
/***************************************************************************
* Subroutine:	CMD_EncryptInit()
* Function:		Command excute to CMD_EncryptDecryptInit()
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_EncryptInit()
{
    UINT16 nRet = SAR_OK;
	
	DBG_FUNC_BEGIN;

	nRet = CMD_EncryptDecryptInit(CT_ENC);

	DBG_FUNC_END;
	SetCmdResult(nRet, 0);
}
/***************************************************************************
* Subroutine:	CMD_Encrypt()
* Function:		Command excute to CMD_EncryptDecryptUpdate()
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_Encrypt()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 keyid = 0;

	DBG_FUNC_BEGIN;

    keyid = ((gApduBody.DATA[5]<<8) | gApduBody.DATA[4]);
    if ((gSessionKey[keyid].crypto_type != CT_ENC) || (gSessionKey[keyid].flag != 2))
    {
		datalen = 0;
		nRet = SAR_INVCONDERR;
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
		goto END;
    }

	nRet = CMD_EncryptDecryptUpdate(CT_ENC, &datalen);
	if (nRet!= SAR_OK || datalen <= 0)
    {
		datalen = 0;
		DBG_INFO("CMD_EncryptDecryptUpdate() failed! nRet = 0x%x", nRet);
		goto END;
	}

END:
    CleanSessionKeyState(keyid);

	DBG_FUNC_END;
	SetCmdResult(nRet, datalen);
}
/***************************************************************************
* Subroutine:	CMD_EncryptUpdate()
* Function:		Command excute to CMD_EncryptDecryptUpdate()
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_EncryptUpdate()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 keyid = 0;
	
	DBG_FUNC_BEGIN;

    keyid = ((gApduBody.DATA[5] << 8) | gApduBody.DATA[4]);
    if ((gSessionKey[keyid].crypto_type != CT_ENC) || (gSessionKey[keyid].flag != 2))
    {
        CleanSessionKeyState(keyid);
    	SetCmdResult(SAR_INVCONDERR, 0);
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
		return;
    }

	nRet = CMD_EncryptDecryptUpdate(CT_ENC, &datalen);
	if (nRet != SAR_OK || datalen <= 0)
    {
        CleanSessionKeyState(keyid);
    	SetCmdResult(nRet, 0);
		DBG_INFO("CMD_EncryptDecryptUpdate() failed! nRet = 0x%x", nRet);
		return;
	}  

	DBG_FUNC_END;
	SetCmdResult(nRet, datalen);
}
/***************************************************************************
* Subroutine:	CMD_EncryptFinal()
* Function:		Command excute to CMD_EncryptDecryptUpdate()
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_EncryptFinal()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 keyid = 0;
	
	DBG_FUNC_BEGIN;

    keyid = ((gApduBody.DATA[5] << 8) | gApduBody.DATA[4]);
    if ((gSessionKey[keyid].crypto_type != CT_ENC) || (gSessionKey[keyid].flag != 2))
    {
        CleanSessionKeyState(keyid);
    	SetCmdResult(SAR_INVCONDERR, 0);
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
		return;
    }

	nRet = CMD_EncryptDecryptUpdate(CT_ENC, &datalen);
	if (nRet != SAR_OK || datalen <= 0)
    {
        CleanSessionKeyState(keyid);
    	SetCmdResult(nRet, 0);
		DBG_INFO("CMD_EncryptDecryptUpdate() failed! nRet = 0x%x", nRet);
		return;
	}
        
    CleanSessionKeyState(keyid);  

	DBG_FUNC_END;
	SetCmdResult(nRet, datalen);
}
/***************************************************************************
* Subroutine:	CMD_DecryptInit()
* Function:		Command excute to CMD_EncryptDecryptInit()
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DecryptInit()
{
    UINT32 nRet = SAR_OK;
	
	DBG_FUNC_BEGIN;

	nRet = CMD_EncryptDecryptInit(CT_DEC);
	
	DBG_FUNC_END;
	SetCmdResult(nRet,0);
}
/***************************************************************************
* Subroutine:	CMD_Decrypt()
* Function:		Command excute to CMD_EncryptDecryptUpdate()
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_Decrypt()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 keyid = 0;
	
	DBG_FUNC_BEGIN;

    keyid = ((gApduBody.DATA[5] << 8) | gApduBody.DATA[4]);    
    if ((gSessionKey[keyid].crypto_type != CT_DEC)  || (gSessionKey[keyid].flag != 2))
    { 
        CleanSessionKeyState(keyid);
    	SetCmdResult(SAR_INVCONDERR, 0);
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
		return;
    }

	nRet = CMD_EncryptDecryptUpdate(CT_DEC, &datalen);
	if (nRet != SAR_OK ||datalen<=0)
    {
        CleanSessionKeyState(keyid);
    	SetCmdResult(nRet, 0);
		DBG_INFO("CMD_EncryptDecryptUpdate() failed! nRet = 0x%x", nRet);
		return;
	}
         
    CleanSessionKeyState(keyid);

	DBG_FUNC_END;
	SetCmdResult(nRet, datalen);
}
/***************************************************************************
* Subroutine:	CMD_DecryptUpdate()
* Function:		Command excute to CMD_EncryptDecryptUpdate()
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DecryptUpdate()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 keyid = 0;
	
	DBG_FUNC_BEGIN;

    keyid = ((gApduBody.DATA[5] << 8) | gApduBody.DATA[4]);    
    if((gSessionKey[keyid].crypto_type != CT_DEC) || (gSessionKey[keyid].flag != 2))
    {                                   
        CleanSessionKeyState(keyid);
    	SetCmdResult(SAR_INVCONDERR, 0);
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
		return;
    }

	nRet = CMD_EncryptDecryptUpdate(CT_DEC, &datalen);
	if (nRet != SAR_OK || datalen <= 0)
    {
        CleanSessionKeyState(keyid);
    	SetCmdResult(nRet, 0);
		DBG_INFO("CMD_EncryptDecryptUpdate() failed! nRet = 0x%x", nRet);
		return;
	}
     
	DBG_FUNC_END;  
	SetCmdResult(nRet, datalen);
}
/***************************************************************************
* Subroutine:	CMD_DecryptFinal()
* Function:		Command excute to CMD_EncryptDecryptUpdate()
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DecryptFinal()
{ 
	UINT16 keyid = 0;
	
	DBG_FUNC_BEGIN;

    keyid = ((gApduBody.DATA[5]<<8) | gApduBody.DATA[4]);    
    if ((gSessionKey[keyid].crypto_type != CT_DEC) || (gSessionKey[keyid].flag != 2))
    {
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
        SetCmdResult(SAR_INVCONDERR, 0);
    }
    else
    {
        SetCmdResult(SAR_OK,0);
    }
    CleanSessionKeyState(keyid);

	DBG_FUNC_END;
}
/***************************************************************************
* Subroutine:	CMD_DigestInit()
* Function:		Init for digest
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DigestInit()
{
	UINT16 nRet = 0;
	UINT16 tempindex = 0;
	UINT16 keybytes = 0;
	UINT16 IDLen = 0;
	UINT16 hashIdx = 0;
	UINT32 keynBits = 0;
    UINT32 hash_tbl[3] = {SGD_SM3, SGD_SHA1, SGD_SHA256};
	UINT8 *ID = NULL;

	DBG_FUNC_BEGIN;

	hashIdx = gApduHeader.P2; // P2 must be value among {1:SM3,2:SHA1,3:SHA256}
    if ((hashIdx > 3) || (hashIdx <= 0))
    {
		DBG_ERR("P2 is wrong! gApduHeader.P2 = 0x%x", gApduHeader.P2);
        SetCmdResult(SAR_INDATAERR, 0);
        return;
    }

	DBG_INFO("Hash alg ID: 0x%x", hash_tbl[hashIdx - 1]);

	switch(hash_tbl[hashIdx-1])
	{
    #ifdef ECCSUPPORT
	case SGD_SM3:
		if (gApduBody.LC > 0)
		{
			SM2_PubKey pub = {0};
			SM3Value* sm3value = NULL;
			UINT8 sm3valuedata[32] = {0};
			UINT8 pubdata[64] = {0};
			pub.X =  pubdata;
			pub.Y =  pubdata+32;
			sm3value = (SM3Value*)sm3valuedata;

			tempindex = 0;
			keynBits = (gApduBody.DATA[3]<<24) | (gApduBody.DATA[2]<<16) | (gApduBody.DATA[1]<<8) | gApduBody.DATA[0];
			if (keynBits != ECC_KEY_BITS)
			{
				DBG_ERR("Public key bits is wrong! keynBits = 0x%x", keynBits);
				SetCmdResult(SAR_INDATAERR, 0);
				return;
			}
			keybytes = keynBits/8;
			tempindex += 4;
			memcpy(pub.X, gApduBody.DATA + tempindex, keybytes);
			tempindex += keybytes;
			memcpy(pub.Y, gApduBody.DATA + tempindex, keybytes);
			tempindex += keybytes;
			IDLen = (gApduBody.DATA[tempindex+3]<<24) | (gApduBody.DATA[tempindex+2]<<16) | (gApduBody.DATA[tempindex+1]<<8) | gApduBody.DATA[tempindex];
			if (IDLen > 32)
			{
				DBG_ERR("User ID length is wrong! IDLen = 0x%x", IDLen);
				SetCmdResult(SAR_INDATALENERR, 0);
				return;
			}
			tempindex += 4;
			ID = gApduBody.DATA + tempindex;
			tempindex += IDLen;
			if (tempindex != gApduBody.LC)
			{
				DBG_ERR("Input data length is wrong! tempindex = 0x%x, gApduBody.LC = 0x%x", tempindex, gApduBody.LC);
				SetCmdResult(SAR_INDATALENERR, 0);
				return;
			}
            #warning "need check below if use SM2_GetHashValue or NOT!!!!"
			nRet = SM3_GetSignIDValue(sm3value,&pub, ID, IDLen);
			if (0 != nRet)
			{
				DBG_ERR("SM3_GetSignIDValue() failed! nRet = 0x%x", nRet);
				SetCmdResult(SAR_INVCONDERR, 0);
				return;
			}
			memset((UINT8*)&gSm3Context, 0, sizeof(SM3_CONTEXT));
			SM3_Init(&gSm3Context);
			SM3_Update(&gSm3Context, (UINT8*)sm3value, 32);
			break;
		}
        else 
		{
            SM3_Init(&gSm3Context);
            break;            	
		}
	#endif                        			
	case SGD_SHA1:
		SHA1_Init(&gSHA1Context);
		break;
	case SGD_SHA256:
		SHA256_Init(&gSHA256Context);
		break;	 
	default:
		DBG_ERR("HASH alg ID is not support! algID = 0x%x", hash_tbl[hashIdx-1]);
		SetCmdResult(SAR_UNSUPPORT_HASH_ALG, 0);
		return;
	}
	gHashAlgInUse = CT_DIGEST;
    gHashAlgID = hash_tbl[hashIdx-1];

	DBG_FUNC_END;
	SetCmdResult(SAR_OK,0);
}
/***************************************************************************
* Subroutine:	CMD_Digest()
* Function:		Digest
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_Digest()
{
	UINT16 datalen = 0;
	UINT8 *data = NULL;

	DBG_FUNC_BEGIN;

	datalen = gApduBody.LC;
	data = gApduBody.DATA;
	if (datalen > 2048)
	{
        gHashAlgID = 0;
        gHashAlgInUse = CT_UNKNOWN;
		SetCmdResult(SAR_INDATALENERR, 0);
		DBG_ERR("Data length is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		return;	
	}

	//检查HASH算法是否已经初始化
    if (gHashAlgInUse != CT_DIGEST)
    {
        gHashAlgID = 0;
        gHashAlgInUse = CT_UNKNOWN;
		SetCmdResult(SAR_INVCONDERR, 0);
		DBG_ERR("CMD_DigestInit() is not called!");
		return;
    }
	
	//HASH运算
	switch(gHashAlgID)
	{
    #ifdef ECCSUPPORT
	case SGD_SM3:
		SM3_Update(&gSm3Context, data, datalen);
        SM3_Final(&gSm3Context, gApduResp.DATA + 4);
        datalen = 32;
		break;
	#endif
	case SGD_SHA1:
		SHA1_Update(&gSHA1Context, data, datalen);
		SHA1_Final(&gSHA1Context, gApduResp.DATA + 4);
		datalen = 20;
		break;
    case SGD_SHA256:
		SHA256_Update(&gSHA256Context, data, datalen);
		SHA256_Final(&gSHA256Context, gApduResp.DATA + 4);
		datalen = 32;
        break;	 
	default:
        gHashAlgID = 0;
        gHashAlgInUse = CT_UNKNOWN;
		SetCmdResult(SAR_UNSUPPORT_HASH_ALG, 0);
		DBG_ERR("HASH alg ID is not support! gHashAlgID = 0x%x", gHashAlgID);
		return;
	}
    gHashAlgID = 0;
    gHashAlgInUse = CT_UNKNOWN;

	gApduResp.DATA[0] = datalen & 0x000000ff;
	gApduResp.DATA[1] = (datalen>>8) & 0x000000ff;
    gApduResp.DATA[2] = (datalen>>16) & 0x000000ff;
	gApduResp.DATA[3] = (datalen>>24) & 0x000000ff;

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, datalen + 4);
}
/***************************************************************************
* Subroutine:	CMD_DigestUpdate()
* Function:		Digest update
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DigestUpdate()
{
	UINT16 datalen = 0;
	UINT8 *data = NULL;
	
	DBG_FUNC_BEGIN;

	datalen = gApduBody.LC;
	data = gApduBody.DATA;
	if (datalen > 2048)
	{
        gHashAlgID = 0;
        gHashAlgInUse = CT_UNKNOWN;
		SetCmdResult(SAR_INDATALENERR, 0);
		DBG_ERR("Data length is wrong! gApduBody.LC = 0x%x", gApduBody.LC);
		return;	
	}
	
	//检查HASH算法是否已经初始化
    if (gHashAlgInUse != CT_DIGEST)
    {
        gHashAlgID = 0;
        gHashAlgInUse = CT_UNKNOWN;
		SetCmdResult(SAR_INVCONDERR, 0);
		DBG_ERR("CMD_DigestInit() is not called!");
		return;
    }
	
	//HASH运算
	switch(gHashAlgID)
	{
    #ifdef ECCSUPPORT
	case SGD_SM3:
		SM3_Update(&gSm3Context, data, datalen);
        break;
	#endif
	case SGD_SHA1:
		SHA1_Update(&gSHA1Context, data, datalen);
		break;	 
	case SGD_SHA256:
		SHA256_Update(&gSHA256Context, data, datalen);
		break;	 
	default:
        gHashAlgID = 0;
        gHashAlgInUse = CT_UNKNOWN;
		SetCmdResult(SAR_UNSUPPORT_HASH_ALG, 0);
		DBG_ERR("HASH alg ID is not support! gHashAlgID = 0x%x", gHashAlgID);
		return;
	}

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_DigestFinal()
* Function:		Digest final
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DigestFinal()
{
    UINT16 datalen = 0;

	DBG_FUNC_BEGIN;

	//检查状态
    if (gHashAlgInUse != CT_DIGEST)
    {
		DBG_ERR("CMD_DigestInit() is not called!");
		SetCmdResult(SAR_INVCONDERR, 0);
		return;
    }

	switch(gHashAlgID)
	{
    #ifdef ECCSUPPORT
	case SGD_SM3:
		SM3_Final(&gSm3Context, gApduResp.DATA + 4);
		datalen = 32;
		break;
	#endif
	case SGD_SHA1:
		SHA1_Final(&gSHA1Context, gApduResp.DATA + 4);
		datalen = 20;
		break;	 
	case SGD_SHA256:
		SHA256_Final(&gSHA256Context, gApduResp.DATA + 4);
		datalen = 32;
		break;	 
	default:
		SetCmdResult(SAR_UNSUPPORT_HASH_ALG, 0);
		DBG_ERR("HASH alg ID is not support! gHashAlgID = 0x%x", gHashAlgID);
		return;
	}
    gHashAlgID = 0;			
	gHashAlgInUse=CT_UNKNOWN;

	gApduResp.DATA[0] = datalen & 0x000000ff;
	gApduResp.DATA[1] = (datalen>>8) & 0x000000ff;
    gApduResp.DATA[2] = (datalen>>16) & 0x000000ff;
	gApduResp.DATA[3] = (datalen>>24) & 0x000000ff;

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, datalen + 4);
}
/***************************************************************************
* Subroutine:	CMD_MACInit()
* Function:		Init for MAC
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_MACInit()
{
    UINT16 nRet = SAR_OK;

	DBG_FUNC_BEGIN;

	nRet = CMD_EncryptDecryptInit(CT_MAC);

	DBG_FUNC_END;
    SetCmdResult(nRet, 0);
}
/***************************************************************************
* Subroutine:	CMD_Mac()
* Function:		Compute mac
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_Mac()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 keyid = 0;
	
	DBG_FUNC_BEGIN;

    keyid = ((gApduBody.DATA[5]<<8) | gApduBody.DATA[4]);
    if (gSessionKey[keyid].crypto_type != CT_MAC)
    {                                 
        CleanSessionKeyState(keyid);
		SetCmdResult(SAR_INVCONDERR, 0);
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
		return;
    }

	nRet = CMD_EncryptDecryptUpdate(CT_MAC, &datalen);
	if (nRet != SAR_OK || datalen <= 0)
    {                                 
        CleanSessionKeyState(keyid);
		SetCmdResult(nRet, 0);
		DBG_INFO("CMD_EncryptDecryptUpdate() failed! nRet = 0x%x", nRet);
		return;
	}

    memcpy(gApduResp.DATA, gSessionKey[keyid].IV, 16);  
    CleanSessionKeyState(keyid);

	DBG_FUNC_END;
	SetCmdResult(nRet, datalen);
}
/***************************************************************************
* Subroutine:	CMD_MACUpdate()
* Function:		Copute mac by update
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_MACUpdate()
{
	UINT16 nRet = SAR_OK;
	UINT16 datalen = 0;
	UINT16 keyid = 0;
	
	DBG_FUNC_BEGIN;
	
    keyid = ((gApduBody.DATA[5]<<8) | gApduBody.DATA[4]);
    if (gSessionKey[keyid].crypto_type != CT_MAC)
    {
		CleanSessionKeyState(keyid);
		SetCmdResult(SAR_INVCONDERR, 0);
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
		return;
    }

	nRet = CMD_EncryptDecryptUpdate(CT_MAC, &datalen);
	if (nRet!= SAR_OK || datalen <= 0)
    {
		CleanSessionKeyState(keyid);
		SetCmdResult(nRet, 0);
		DBG_INFO("CMD_EncryptDecryptUpdate() failed! nRet = 0x%x", nRet);
		return;
	}

    memcpy(gSessionKey[keyid].IV, gApduResp.DATA + datalen - 16, 16);

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, 0);
}
/***************************************************************************
* Subroutine:	CMD_MACFinal()
* Function:		Mac final
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_MACFinal()
{
	UINT16 keyid = 0;
	
	DBG_FUNC_BEGIN;
	
    keyid = ((gApduBody.DATA[5]<<8) | gApduBody.DATA[4]);
    if (gSessionKey[keyid].crypto_type != CT_MAC)
    {
		CleanSessionKeyState(keyid);
		SetCmdResult(SAR_INVCONDERR, 0);
		DBG_INFO("The session key status is wrong! keyid = 0x%x", keyid);
		return;
    }
	                         
    memcpy(gApduResp.DATA, gSessionKey[keyid].IV, 16);
    CleanSessionKeyState(keyid);

	DBG_FUNC_END;
	SetCmdResult(SAR_OK, 16);
}
/***************************************************************************
* Subroutine:	CMD_DestroySessionKey()
* Function:		Destroy sessionkey
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
void CMD_DestroySessionKey()
{
    UINT16 nRet = SAR_OK;
	UINT16 keyid = 0;

	DBG_FUNC_BEGIN;

	keyid = (gApduBody.DATA[5]<<8 | gApduBody.DATA[4]);
	nRet = ReleaseSessionKey(keyid);

	DBG_FUNC_END;
	SetCmdResult(nRet, 0);
}
/***************************************************************************
* Subroutine:	CMD_GetAppStatus()
* Function:		Get current App card status
* Author:   Syno common		 
* Date:		2015.7.10
* Version:  V1.0
* ModifyRecord:
* ************************************************************************/
UINT16 CMD_GetAppStatus()
{
	tFileNode Node = {0};

	DBG_FUNC_BEGIN;
	
	FS_ReadDirNodeInfo((UINT8*)&Node);     
	
	DBG_FUNC_END;

	return Node.FCP.nStatus;	
}
/***************************************************************************
* Subroutine:	CMD_ClearProg()
* Function:		擦除标志位或者设备格式化 
* Author:   Comon		 
* Date:		2010.5.30
* Version:  V1.21
* ModifyRecord:
* ************************************************************************/
void CMD_ClearProg()
{
    DBG_FUNC_BEGIN;

    if (gApduHeader.P1 == 0)
    {			
        DBG_INFO("Format data!");
        				  
    	FS_Format();
    }    
	else if (gApduHeader.P1 == 1)
	{		 	
        DBG_INFO("Format data and clean programm!");
        			  		  
    	FS_Format();
    	ClrProgFlag(AREA_PARA_WE);
	}
	else if (gApduHeader.P1 == 2)
	{	           
		UINT32 lFlag = 0;
         
        DBG_INFO("Erase all flash data!");
		EFC_WrProtect(AREA_PARA_WE);
		memset(gApduResp.DATA, 0x00, 12);
		memset(gApduResp.DATA, 0xff, 4);
		lFlag = 0x20070805;
		memcpy(gApduResp.DATA + 8,(UINT8*)&lFlag, 4);
		EFC_WriteInfo(gApduResp.DATA, 256*FLASH_PAGE_SIZE, 12);
		EFC_WrProtect(AREA_PROT_DI);
	}
	
    DBG_FUNC_END;   
	SetCmdResult(SAR_OK, 0);
}

