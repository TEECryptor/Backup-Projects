/***************************************************************************
 *	File name:SynoTFKeySKF.c
 *	Introduce:The implement file for SKF API functions
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

#include "SynoTFKeySKF.h"
#include "Synolog.h"

#include "smartcard.h"
#include "device.h"
#include "skfsha1.h"
#include "skfsms4.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------*
 * 应用信息结构定义                             *
 *----------------------------------------------*/
typedef struct tagAppLicationInfo
{
	CHAR	szApplicationName[48];
	CHAR	szAdminPin[16];
	UINT32	dwAdminPinRetryCount;
	CHAR	szUserPin[16];
	UINT32	dwUserPinRetryCount;
	UINT32	dwCreateFileRight;
	UCHAR	byContainerNum;
	UCHAR	byCertNum;
	USHORT	wFileNum;
} APPLICATIONINFO_S;

typedef struct tagKeyHandle
{
	ULONG KeyHandle;                             /* 密钥句柄 */
	ULONG ulAlgID;	                             /* 密钥支持算法ID */
	ULONG KeyType;	                             /* 0 cipher key,1 plain key */
	CHAR  Key[32];
	USHORT KeyID;                                /* 密钥存储位置ID */
	USHORT ContainerID;	                         /* 容器ID */
	USHORT AppID;			                     /* 应用ID */
	HANDLE phDev;		                         /* 设备句柄 */
} KEYHANDLE_S;


extern HANDLE m_phDev;

ULONG gContainerType = 0;
ULONG gStartTime = 0;
ULONG gTern = 0;

ULONG g_ulPaddingType = 0;
UCHAR g_aucPadByte[16]={0};
ULONG g_ulPadLen = 0;

//#pragma data_seg(".ICAKeyShare")
HANDLE  syHandle = 0;
//#pragma data_seg()
//#pragma comment(linker, "/SECTION:.ICAKeyShare,RWS")

extern UCHAR g_SM4_KYE[];
extern UCHAR g_aucDefWorkKey[]; //默认设备密钥

BOOL g_bWaitFlag = TRUE;

/*----------------------------------------------*
 * 设备管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_WaitForDevEvent(LPSTR szDevName,
    ULONG *pulDevNameLen, ULONG *pulEvent)
{
	BOOL bExistDevice = FALSE;
	HANDLE hHandle = NULL;
	DEVINFO stMyDev = {0};
	g_bWaitFlag = TRUE;

    /* 参数检查 */
	if ((NULL == szDevName) || (NULL == pulDevNameLen) || (NULL == pulEvent))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
	}

    /* 说明设备已经存在了,那后面就检测拔除事件 */
	if(DEV_OpenDevice(0, &hHandle))
	{
		bExistDevice = TRUE;
		SKF_GetDevInfo(hHandle, &stMyDev);
		DEV_CloseDevice(hHandle);
	}

	while (1)
	{
        /* 检查是否取消等待设备插拔 */
		if (!g_bWaitFlag)
		{
			return SAR_FAIL;
		}

		if(bExistDevice)
		{
            /* 检测拔除事件 */
			if (!DEV_OpenDevice(0, &hHandle))
			{
				*pulEvent = 2;
				//strcpy(szDevName, stMyDev.Label);
				//*pulDevNameLen = strlen(stMyDev.Label);
				break;
			}
		}
		else
		{
            /* 检测插入事件 */
			if(DEV_OpenDevice(0, &hHandle))
			{
				SKF_GetDevInfo(hHandle, &stMyDev);
				DEV_CloseDevice(hHandle);
				*pulEvent = 1;
				//strcpy(szDevName, stMyDev.Label);
				//*pulDevNameLen = strlen(stMyDev.Label);
				break;
			}
		}
	}

    DEV_CloseDevice(hHandle);

    return SAR_OK;
}

ULONG DEVAPI SKF_CancelWaitForDevEvent()
{
	g_bWaitFlag = FALSE;
	return SAR_OK;
}

ULONG DEVAPI SKF_EnumDev(BOOL bPresent, LPSTR szNameList, ULONG *pulSize)
{
    ULONG ulRet = SAR_OK;
	HANDLE hDev = NULL;
    DEVINFO stMyDev = {0};
    CHAR acTmp[512] = {0};
    ULONG ulLen = 0;
	ULONG ulTmpLen = 0;
	INT32 i = 0;

    FUNC_BEGIN;

    /* 参数检查 */
	if (NULL == pulSize)
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
	}

    /* 这里返回szNameList需要的空间大小 */
    *pulSize = 512;

    if (NULL == szNameList)
    {
        return SAR_OK;
    }

    if (bPresent)
    {
    	for(i = 0; i < MAX_DEV_CNT; i++)
    	{
    		hDev = NULL;
            ulRet = DEV_OpenDevice(i, &hDev);
            if(SAR_OK == ulRet)
            {
    			if (NULL == hDev)
    			{
    				continue;
    			}

                ulRet = SKF_GetDevInfo(hDev, &stMyDev);
                DEV_CloseDevice(hDev);
                if(SAR_OK != ulRet)
                {
                    break;
                }

				memcpy(acTmp, stMyDev.Label, 32);
                ulTmpLen = (ULONG)strlen(acTmp);
                memcpy(szNameList + ulLen, acTmp, ulTmpLen + 1);
                ulLen += ulTmpLen + 1;
            }
            else
            {
                break;
            }

            if (g_bDefinedPath)
            {
                break;
            }
    	}
    }
    else
    {
		return SAR_OK;
    }

    if (0 != ulLen)
    {
        memcpy(szNameList + ulLen, "\0", 1);
		ulLen++;
		*pulSize = ulLen;
    }
    else
    {
        *pulSize = 0;
    }

    FUNC_END;

	return ulRet;
}

ULONG DEVAPI SKF_ConnectDev(LPSTR szName, DEVHANDLE *phDev)
{
    ULONG ulRet = SAR_OK;
    DEVINFO stMyDev = {0};
	CHAR acTmp[512] = {0};
	INT32 i = 0;

    /* 参数检查 */
    if ((NULL == szName) || (NULL == phDev))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	for(i = 0; i < MAX_DEV_CNT; i++)
	{
        *phDev = NULL;
        ulRet = DEV_OpenDevice(i, phDev);
        if (SAR_OK == ulRet)
        {
            if (NULL == *phDev)
            {
               continue;
            }

            ulRet = SKF_GetDevInfo(*phDev, &stMyDev);
            if (SAR_OK != ulRet)
            {
                LOG_ErrNo(ulRet);
                return ulRet;
            }

			memcpy(acTmp, stMyDev.Label, 32);

            if(!strncmp(szName, acTmp, 32))
            {
                return SAR_OK;
            }
        }
        else
        {
            return ulRet;
        }

        if (g_bDefinedPath)
        {
            break;
        }
	}

	return SAR_INVALIDDEVNAME;
}

ULONG DEVAPI SKF_DisConnectDev(DEVHANDLE hDev)
{
	DEV_CloseDevice(hDev);
	return SAR_OK;
}

ULONG DEVAPI SKF_GetDevState(LPSTR szDevName, ULONG *pulDevState)
{
	ULONG ulRet = SAR_OK;
    HANDLE hDev = NULL;
    DEVINFO stMyDev = {0};
	CHAR acTmp[512] = {0};
	INT32 i = 0;

    for(i = 0; i < MAX_DEV_CNT; i++)
	{
        hDev = NULL;
        ulRet = DEV_OpenDevice(i, &hDev);
        if (SAR_OK == ulRet)
        {
            if (NULL == hDev)
            {
               continue;
            }

            if (SKF_ExterAuth(hDev, NULL, 0))
            {
                ulRet = SKF_GetDevInfo(hDev, &stMyDev);

				memcpy(acTmp, stMyDev.Label, 32);

                if(!strncmp(szDevName, acTmp, 32))
                {
                    DEV_CloseDevice(hDev);
    				*pulDevState = DEV_PRESENT_STATE;
        			return SAR_OK;
                }
                else
                {
                    DEV_CloseDevice(hDev);
                }
            }
        }
        else
        {
            *pulDevState = DEV_ABSENT_STATE;
			break;
        }
	}

	 return SAR_OK;
}

ULONG DEVAPI SKF_SetLabel(DEVHANDLE hDev, CHAR *pcLabel)
{
    ULONG ulRet = SAR_OK;
	ULONG ulLabelLen = 0;

    /* 参数检查 */
	if ((NULL == hDev) || (NULL == pcLabel))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulLabelLen = (ULONG)strlen(pcLabel);
	if(ulLabelLen > 32)
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    ulRet = CMD_Transmit(hDev, CMD_SET_LABEL, 0x00, 0x00,
        (UCHAR *)pcLabel, ulLabelLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_GetDevInfo(DEVHANDLE hDev, DEVINFO *pDevInfo)
{
	ULONG ulRet = SAR_OK;
	ULONG ulOutLen = 0;

    FUNC_BEGIN;

    /* 参数检查 */
    if ((NULL == hDev) || (NULL == pDevInfo))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }
	ulOutLen = sizeof(DEVINFO);
    ulRet = CMD_Transmit(hDev, CMD_GET_DEVINFO, 0x00, 0x00,
        NULL, 0, (UCHAR *)pDevInfo, &ulOutLen);

    FUNC_END;

	return ulRet;
}

ULONG DEVAPI SKF_LockDev(DEVHANDLE hDev, ULONG ulTimeOut)
{
	syHandle = hDev;
	gTern = ulTimeOut;
	//gStartTime = GetTickCount();
	return SAR_OK;
}

ULONG DEVAPI SKF_UnlockDev (DEVHANDLE hDev)
{
	syHandle = 0;
	gStartTime = 0;
	return SAR_OK;
}

ULONG DEVAPI SKF_Transmit(DEVHANDLE hDev, BYTE *pbCommand, ULONG ulCommandLen, BYTE *pbData, ULONG *pulDataLen)
{
    ULONG lRet = 0;
    BYTE pCmdBuf[MAX_BUF] = {0,};
    ULONG ulDataLen = 0;

    /* 参数检查 */
    if (MAX_CMD_BUF < ulCommandLen)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    //memcpy(&ulDataLen,pbCommand+ulCommandLen-2,2);
    //*pulDataLen = ulDataLen;

    if (pbData == NULL)
    {
       return SAR_OK;
    }

	ulDataLen = *pulDataLen;
    memcpy(pCmdBuf,pbCommand,ulCommandLen);
    lRet = APDUInterface(hDev, pCmdBuf, ulCommandLen, pbData, &ulDataLen);
    *pulDataLen = ulDataLen;
    return lRet;
}

/*----------------------------------------------*
 * 访问控制                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_ChangeDevAuthKey(DEVHANDLE hDev,
    BYTE *pbKeyValue, ULONG ulKeyLen)
{
    ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[512] = {0};
    UCHAR aucTmpKey[32] = {0};

	if (ulKeyLen > 16)
	{
        LOG_ErrNo(SAR_INDATALENERR);
		return SAR_INDATALENERR;
	}

	memset(aucTmpKey, 0, 32);
	memcpy(aucTmpKey, pbKeyValue, ulKeyLen);
    memcpy(g_SM4_KYE, g_aucDefWorkKey, 16);
    SMS4_ENC(g_SM4_KYE, aucTmpKey, aucCmdBuf, 16);

    /* AlgID=0x02表示使用SMS4算法 */
    ulRet = CMD_TransmitEx(hDev, CMD_CHANGE_DEVAUTHKEY, 0x00, 0x02,
        aucCmdBuf, 16, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_DevAuth(DEVHANDLE hDev, BYTE *pbAuthData, ULONG ulLen)
{
    ULONG ulRet = SAR_OK;

    /* 参数检查 */
	if ((NULL == hDev)
        || (NULL == pbAuthData)
        || (ulLen%16))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    ulRet = CMD_Transmit(hDev, CMD_GET_AUTH, 0x00, 0x02,
        pbAuthData, ulLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_ChangePIN(HAPPLICATION hApplication,
    ULONG ulPinType, CHAR *pcOldPin, CHAR *pcNewPin, ULONG *pulRetryCount)
{
	ULONG ulRet = SAR_OK;
    UCHAR aucCmdBuf[128] = {0};
	UCHAR aucHashOldPin[20] = {0};
    UCHAR aucTmpOldKey[16] = {0};
	UCHAR rawNewPin[32] = {0};

	APPHANDLE_S * phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle)
        || (NULL == pcNewPin)
        || (NULL == pulRetryCount))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    /* 检查PIN的有效性 */
    if (MAX_PIN_LEN < strlen(pcNewPin))
    {
        LOG_ErrNo(SAR_PIN_LEN_RANGE);
		return SAR_PIN_LEN_RANGE;
    }

    /* 校验PIN码 */
	ulRet = SKF_VerifyPIN(hApplication, ulPinType, pcOldPin, pulRetryCount);
	if (SAR_OK != ulRet)
	{
		return ulRet;
	}

    /* 使用哈希算法加密PIN */
	memset(aucTmpOldKey, 0, 16);
	memcpy(aucTmpOldKey, pcOldPin, strlen(pcOldPin));
	SKFSHA1(aucTmpOldKey,  16, aucHashOldPin);

	/* 旧hashkey 传递到接口作为加密密码用 */
	memcpy(g_SM4_KYE, aucHashOldPin, 16);

	memset(rawNewPin,0, 16);
	memcpy(rawNewPin, pcNewPin, strlen(pcNewPin));
	SMS4_ENC(g_SM4_KYE, rawNewPin, rawNewPin, 16);

	/* 组命令报文 */
	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
	memcpy(aucCmdBuf + 2, rawNewPin, 16);

    ulRet = CMD_TransmitEx(phAppHandle->phDev, CMD_CHANGE_PIN, 0x00, (UCHAR)ulPinType,
        aucCmdBuf, 16 + 2, NULL, NULL);

    return ulRet;
}

ULONG DEVAPI SKF_GetPINInfo(HAPPLICATION hApplication,
    ULONG ulPINType, ULONG *pulMaxRetryCount,
    ULONG *pulRemainRetryCount, BOOL *pbDefaultPin)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
	UCHAR aucBuf[256] = {0};
	UCHAR aucCmdBuf[512] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == hApplication)
        || (NULL == pulMaxRetryCount)
        || (NULL == pulRemainRetryCount)
        || (NULL == pbDefaultPin))
	{
		LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
    ulCmdLen += 2;

    ulOutLen = 4;
    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_GET_PININFO, 0x00, (UCHAR)ulPINType,
        aucCmdBuf, ulCmdLen, aucBuf, &ulOutLen);
	if(SAR_OK != ulRet)
	{
		return ulRet;
	}

	*pulMaxRetryCount = *(UCHAR *)aucBuf;
	*pulRemainRetryCount = *(UCHAR *)(aucBuf + 1);
	if (NULL != pbDefaultPin)
	{
		*pbDefaultPin = *(UCHAR *)(aucBuf + 1 + 1);
	}

	return ulRet;
}

ULONG DEVAPI SKF_VerifyPIN(HAPPLICATION hApplication,
    ULONG ulPinType, CHAR *pcPin, ULONG *pulRetryCount)
{
	ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[128] = {0};
	UCHAR aucHashPin[20] = {0};
    UCHAR auctmpPin[16] = {0};
	UCHAR aucRandom[16] = {0};
    UCHAR aucEncRandom[16] = {0};

	APPHANDLE_S *phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle) || (NULL == pcPin) || (NULL == pulRetryCount))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    /* 检查PIN的有效性 */
    if (MAX_PIN_LEN < strlen(pcPin))
    {
        LOG_ErrNo(SAR_PIN_LEN_RANGE);
		return SAR_PIN_LEN_RANGE;
    }

	/* 随机数产生为8位,但sms4数据要16的倍数,因此后边补8个0 */
	memset(aucRandom, 0, sizeof(aucRandom));

    /* 产生8位的随机数 */
	ulRet = SKF_GenRandom(phAppHandle->phDev, aucRandom, 8);
	if(ulRet != SAR_OK)
	{
        LOG_ErrNo(ulRet);
		return ulRet;
	}

	memcpy(auctmpPin, pcPin, strlen(pcPin));

    /* 使用哈希算法加密PIN */
	SKFSHA1(auctmpPin, 16, aucHashPin);

    /* 使用SMS4对随机数加密 */
	SMS4_ENC(aucHashPin, aucRandom, aucEncRandom, 16);

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
	memcpy(aucCmdBuf + 2, aucEncRandom, 16);

    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_VERIFY_PIN, 0x00, (UCHAR)ulPinType,
        aucCmdBuf, 2+16, NULL, NULL);
	if(ulRet != SAR_OK)
	{
		*pulRetryCount = (ulRet & 0x0f00) >> 8;
	}

	ulRet = ulRet & 0x0A0000FF;

	return ulRet;
}

ULONG DEVAPI SKF_UnblockPIN(HAPPLICATION hApplication,
    CHAR *pcAdminPIN, CHAR *pcNewUserPIN, ULONG *pulRetryCount)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
	UCHAR aucHashPin[20] = {0};
    UCHAR aucTempPIN[16] = {0};
	UCHAR aucTempNewPIN[16] = {0};
	UCHAR aucCmdBuf[128] = {0};

    /* 先校验管理员PIN码 */
	ulRet = SKF_VerifyPIN(hApplication, KT_SOPIN, pcAdminPIN, pulRetryCount);
	if (SAR_OK != ulRet)
	{
		return ulRet;
	}

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle) || (NULL == pcNewUserPIN))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	memset(aucTempPIN, 0, 16);
	memcpy(aucTempPIN, pcAdminPIN, strlen(pcAdminPIN));
	SKFSHA1(aucTempPIN, 16, aucHashPin);

	memcpy(g_SM4_KYE, aucHashPin, 16);

	memset(aucTempNewPIN, 0,16);
	memcpy(aucTempNewPIN, pcNewUserPIN, strlen(pcNewUserPIN));
    SMS4_ENC(g_SM4_KYE, aucTempNewPIN, aucTempNewPIN, 16);

    /* 组命令报文 */
	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
	memcpy(aucCmdBuf + 2, aucTempNewPIN, 16);

    ulRet = CMD_TransmitEx(phAppHandle->phDev, CMD_UNBLOCK_PIN, 0x00, 0x00,
        aucCmdBuf, 2 + 16, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_ClearSecureState(HAPPLICATION hApplication)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
    UCHAR aucCmdBuf[16] = {0};
	ULONG ulCmdLen = 0;

	phAppHandle = (APPHANDLE_S *)hApplication;

	/* 参数检查 */
	if (NULL == phAppHandle)
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
	ulCmdLen += 2;

    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_CLEAR_SECURESTATE, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

/*----------------------------------------------*
 * 应用管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_CreateApplication(DEVHANDLE hDev, LPSTR szAppName,
    LPSTR szAdminPin, ULONG ulAdminPinRetryCount,
    LPSTR szUserPin, ULONG ulUserPinRetryCount,
    ULONG ulCreateFileRights, HAPPLICATION *phApplication)
{
	ULONG ulRet = SAR_OK;
	APPLICATIONINFO_S stAppInfo = {0};

	/* 参数检查 */
	if ((NULL == hDev)
        || (NULL == szAppName)
        || (NULL == szAdminPin)
        || (NULL == szUserPin)
        || (NULL == phApplication))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    if ((48 < (strlen(szAppName) + 1)) || (0 >= strlen(szAppName)))
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

	memset(&stAppInfo, 0, sizeof(APPLICATIONINFO_S));
	strcpy(stAppInfo.szApplicationName, szAppName);
	strcpy(stAppInfo.szAdminPin, szAdminPin);
	stAppInfo.dwAdminPinRetryCount = ulAdminPinRetryCount;

	strcpy(stAppInfo.szUserPin, szUserPin);
	stAppInfo.dwUserPinRetryCount = ulUserPinRetryCount;
	stAppInfo.dwCreateFileRight = ulCreateFileRights;

	stAppInfo.byContainerNum = (UCHAR)512; //最大支持容器数量
	stAppInfo.byCertNum = (UCHAR)16; //最大证书数量
	stAppInfo.wFileNum = (USHORT)16; //最大文件数量

    ulRet = CMD_Transmit(hDev, CMD_CREATE_APPLICATION, 0x00, 0x00,
        (UCHAR *)&stAppInfo, sizeof(APPLICATIONINFO_S), NULL, NULL);
    if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	ulRet = SKF_OpenApplication(hDev, szAppName, phApplication);

	return ulRet;
}

ULONG DEVAPI SKF_DeleteApplication(DEVHANDLE hDev, LPSTR szAppName)
{
	ULONG ulRet = SAR_OK;
	ULONG ulAppNameLen = 0;

	/* 参数检查 */
	if ((NULL == hDev) || (NULL == szAppName))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulAppNameLen = (ULONG)strlen(szAppName);
	if ((48 < (ulAppNameLen + 1)) || (0 >= ulAppNameLen))
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

    ulRet = CMD_Transmit(hDev, CMD_DELETE_APPLICATION, 0x00, 0x00,
        (UCHAR *)szAppName, ulAppNameLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_EnumApplication(DEVHANDLE hDev, CHAR *pcAppName, ULONG *pulSize)
{
	ULONG ulRet = SAR_OK;

	/* 参数检查 */
	if ((NULL == hDev) || (NULL == pulSize))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    *pulSize = 1024;
	if (pcAppName == NULL)
	{
		return SAR_OK;
	}

    ulRet = CMD_Transmit(hDev, CMD_ENUM_APPLICATION, 0x00, 0x00,
        NULL, 0, (UCHAR *)pcAppName, pulSize);

	return ulRet;
}

ULONG DEVAPI SKF_OpenApplication(DEVHANDLE hDev,
    CHAR *pcAppName, HAPPLICATION *phApplication)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
	UCHAR aucOutBuf[128] = {0};
	ULONG ulAppNameLen = 0;
    ULONG ulOutLen = 0;

    /* 参数检查 */
	if ((NULL == hDev) || (NULL == pcAppName) || (NULL == phApplication))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulAppNameLen = (ULONG)strlen(pcAppName);
	if ((48 < (ulAppNameLen + 1)) || (0 >= ulAppNameLen))
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

    ulOutLen = 0x0A;
    ulRet = CMD_Transmit(hDev, CMD_OPEN_APPLICATION, 0x00, 0x00,
        (UCHAR *)pcAppName, ulAppNameLen, aucOutBuf, &ulOutLen);
    if(SAR_OK != ulRet)
	{
		return ulRet;
	}

    phAppHandle = (APPHANDLE_S *)ICA_calloc(1, sizeof(APPHANDLE_S));
    if (NULL == phAppHandle)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

    strcpy(phAppHandle->szName, pcAppName);
    memcpy(&phAppHandle->usAppID, aucOutBuf + 8, 2);
    phAppHandle->phDev = hDev;

	*phApplication = (HAPPLICATION)phAppHandle;

	return ulRet;
}

ULONG DEVAPI SKF_CloseApplication(HAPPLICATION hApplication)
{
	ULONG ulRet;
	APPHANDLE_S *phAppHandle = NULL;
	UCHAR aucCmdBuf[512] = {0};
    ULONG ulCmdLen = 0;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if (NULL == phAppHandle)
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
    ulCmdLen += 2;

    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_CLOSE_APPLICATION, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

    ICA_free(phAppHandle);

	return ulRet;
}

/*----------------------------------------------*
 * 文件管理                                     *
 *----------------------------------------------*/
#define MAX_PACKAGE                         1024

ULONG DEVAPI SKF_CreateFile(HAPPLICATION hApplication, LPSTR szFileName,
    ULONG ulFileSize, ULONG ulReadRights, ULONG ulWriteRights)
{
    ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
    FILEATTRIBUTE stFAttribute = {0};
	UCHAR P1, P2;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle) || (NULL == szFileName))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    if ((48 < (strlen(szFileName) + 1)) || (0 >= strlen(szFileName)))
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

	memset(&stFAttribute, 0, sizeof(FILEATTRIBUTE));
	stFAttribute.FileSize = ulFileSize;
	stFAttribute.ReadRights = ulReadRights;
	stFAttribute.WriteRights = ulWriteRights;
	strcpy(stFAttribute.FileName, szFileName);

	P1 = (phAppHandle->usAppID & 0xff00) >> 8;
	P2 = phAppHandle->usAppID & 0xff;

    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_CREATE_FILE, P1, P2,
        (UCHAR *)&stFAttribute, sizeof(FILEATTRIBUTE), NULL, NULL);

    return ulRet;
}

/*****************************************************************************
 函 数 名  : SKF_DeleteFile
 功能描述  : 删除指定文件,文件删除后,文件中写入的所有信息将丢失,
             文件在设备中的占用的空间将被释放
 输入参数  : hApplication 应用句柄
             pcFileName 文件名称,长度不得大于48个字节
 输出参数  : 无
 返 回 值  : SAR_OK 成功 或 错误码

 修改历史      :
  1.日    期   : 2014年01月16日
    作    者   : 张晓垒
    修改内容   : 新生成函数
*****************************************************************************/
ULONG DEVAPI SKF_DeleteFile(HAPPLICATION hApplication, CHAR *pcFileName)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
	UCHAR P1, P2;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle) || (NULL == pcFileName))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    if ((48 < (strlen(pcFileName) + 1)) || (0 >= strlen(pcFileName)))
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

	P1 = (phAppHandle->usAppID & 0xff00) >> 8;
	P2 = phAppHandle->usAppID & 0xff;

	ulRet = CMD_Transmit(phAppHandle->phDev, CMD_DELETE_FILE, P1, P2,
        (UCHAR *)pcFileName, (ULONG)strlen(pcFileName), NULL, NULL);
    return ulRet;
}

/*****************************************************************************
 函 数 名  : SKF_EnumFiles
 功能描述  : 枚举一个应用下存在的所有文件
 输入参数  : hApplication 应用句柄
 输出参数  : pcFileList 返回文件名称列表,每个文件名称以单个'\0'结束,
             以双'\0'表示列表的结束
             pulSize 输入时表示数据缓冲区的大小,返回实际文件名称列表的长度
 返 回 值  : SAR_OK 成功 或 错误码

 修改历史      :
  1.日    期   : 2014年01月16日
    作    者   : 张晓垒
    修改内容   : 新生成函数
*****************************************************************************/
ULONG DEVAPI SKF_EnumFiles(HAPPLICATION hApplication,
    CHAR *pcFileList, ULONG *pulSize)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
	UCHAR P1, P2;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle) || (NULL == pcFileList) || (NULL == pulSize))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	if(32*16 > *pulSize)
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

	P1 = (phAppHandle->usAppID & 0xff00) >> 8;
	P2 = phAppHandle->usAppID & 0xff;

    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_ENUM_FILE, P1, P2,
        NULL, 0, pcFileList, pulSize);

	return ulRet;
}

/*****************************************************************************
 函 数 名  : SKF_GetFileInfo
 功能描述  : 获取应用文件的属性信息,例如文件的大小,权限等
 输入参数  : hApplication 应用句柄
             pcFileName 文件名称
 输出参数  : pstFileInfo 文件信息,指向文件属性结构的指针
 返 回 值  : SAR_OK 成功 或 错误码

 修改历史      :
  1.日    期   : 2014年01月16日
    作    者   : 张晓垒
    修改内容   : 新生成函数
*****************************************************************************/
ULONG DEVAPI SKF_GetFileInfo(HAPPLICATION hApplication,
    CHAR *pcFileName, FILEATTRIBUTE *pstFileInfo)
{
    ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
    UCHAR aucOutBuf[128] = {0};
    ULONG ulFileLen = 0;
    ULONG ulOutLen = 0;
	UCHAR P1, P2;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle) || (NULL == pcFileName) || (NULL == pstFileInfo))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulFileLen = (ULONG)strlen(pcFileName);
	if(ulFileLen > 48)
	{
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

	P1 = (phAppHandle->usAppID & 0xff00) >> 8;
	P2 = phAppHandle->usAppID & 0xff;

    ulOutLen = 12;
    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_GET_FILEINFO, P1, P2,
        (UCHAR *)pcFileName, ulFileLen, aucOutBuf, &ulOutLen);
    if(ulRet == SAR_OK)
	{
		memcpy(&pstFileInfo->FileName,pcFileName,ulFileLen);
		memcpy(&pstFileInfo->FileSize, aucOutBuf, 4);
		memcpy(&pstFileInfo->ReadRights, aucOutBuf + 4, 4);
		memcpy(&pstFileInfo->WriteRights, aucOutBuf + 4 + 4, 4);
	}

	return ulRet;
}

ULONG DEVAPI SKF_ReadFile(HAPPLICATION hApplication, LPSTR szFileName,
    ULONG ulOffset, ULONG ulSize, BYTE *pbOutData, ULONG *pulOutLen)
{
    ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
    UCHAR aucCmdBuf[64] = {0};
    FILEATTRIBUTE stFAttri = {0};
    ULONG ulFileSize = 0;
	ULONG ulNameLen = 0;
    ULONG ulTmpLen = 0;
	ULONG ulTimes = 0;
	ULONG ulLen = 0;
    ULONG ulLen2 = 0;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle)
        || (NULL == szFileName)
        || (NULL == pbOutData)
        || (NULL == pulOutLen))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulNameLen = (ULONG)strlen(szFileName);
	if(ulNameLen > 48)
	{
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

	//获得文件属性(文件大小)
	ulLen = sizeof(FILEATTRIBUTE);
	ulFileSize = ulSize;

	ulRet = SKF_GetFileInfo(hApplication, szFileName, (FILEATTRIBUTE *)&stFAttri);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	if(ulFileSize + ulOffset > stFAttri.FileSize)
	{
		ulFileSize = stFAttri.FileSize - ulOffset;
	}

	ulLen = ulFileSize;
	ulSize = ulFileSize;

	/*
    CMD_BUF传入的数据流：

	WORD wAppID;
	WORD wOffset;
	WORD wFileNameLen;
	CHAR chFileName			//长度：wFileNameLen
	*/

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
	//偏移在下面赋值
	memcpy(aucCmdBuf + 2 + 2, &ulNameLen, 2);
	memcpy(aucCmdBuf + 2 + 2 + 2, szFileName, ulNameLen);

	ulTmpLen = 2 + 2 + 2 + ulNameLen;
	ulTimes = 0;
    while(ulFileSize > MAX_PACKAGE)
	{
		memcpy(aucCmdBuf + 2, (void *)&ulOffset, 2);

        ulLen2 = MAX_PACKAGE;
        ulRet = CMD_Transmit(phAppHandle->phDev, CMD_READ_FILE, 0x00, 0x00,
            aucCmdBuf, ulTmpLen, pbOutData+ulTimes*MAX_PACKAGE, &ulLen2);
		if(ulRet != SAR_OK)
		{
			return ulRet;
		}

		ulOffset += MAX_PACKAGE;
		ulFileSize -= MAX_PACKAGE;
		ulTimes++;
	}

	if(ulFileSize > 0)
	{
		memcpy(aucCmdBuf + 2, (void *)&ulOffset, 2);
        ulRet = CMD_Transmit(phAppHandle->phDev, CMD_READ_FILE, 0x00, 0x00,
            aucCmdBuf, (2+2+2+ulNameLen), pbOutData+ulTimes*MAX_PACKAGE, &ulFileSize);

		if(ulRet != SAR_OK)
		{
			return ulRet;
		}
	}

	*pulOutLen = ulLen;

	return SAR_OK;
}

ULONG DEVAPI SKF_WriteFile (HAPPLICATION hApplication,
	LPSTR szFileName, ULONG  ulOffset, BYTE *pbData, ULONG ulSize)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S * phAppHandle = NULL;
    UCHAR aucCmdBuf[1024 + 64] = {0};
	ULONG ulNameLen = 0;
    ULONG ulTmpLen = 0;
    ULONG ulBufLen = 0;
    ULONG ulTimes = 0;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
	if ((NULL == phAppHandle) || (NULL == szFileName) || (NULL == pbData))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulNameLen = (ULONG)strlen(szFileName);
	if(ulNameLen > 48)
	{
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

/*CMD_BUF存储内容
	wAppID	2
	wOffset 2
	wFileNameLen	2
	chFileName		//wFileNameLen
	wDataLen		2
	pucData			//wDataLen
*/

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
	memcpy(aucCmdBuf + 2 + 2, &ulNameLen, 2);
	memcpy(aucCmdBuf + 2 + 2 + 2, szFileName, ulNameLen);

	ulTimes=0;
	while(ulSize > MAX_PACKAGE)
	{
		ulBufLen = MAX_PACKAGE;

		memcpy(aucCmdBuf + 2, &ulOffset, 2);
		memcpy(aucCmdBuf + 2 + 2 + 2 + ulNameLen, &ulBufLen, 2);
		memcpy(aucCmdBuf + 2 + 2 + 2 + ulNameLen + 2, pbData + ulTimes * MAX_PACKAGE,  MAX_PACKAGE);
		ulTmpLen = 2 + 2 + 2 + ulNameLen + 2 + MAX_PACKAGE;

        ulRet = CMD_Transmit(phAppHandle->phDev, CMD_WRITE_FILE, 0x00, 0x00,
            aucCmdBuf, ulTmpLen, NULL, NULL);
		if(ulRet != SAR_OK)
		{
			return ulRet;
		}

		ulOffset+=MAX_PACKAGE;
		ulSize-=MAX_PACKAGE;
		ulTimes++;
	}

	if(ulSize>0)
	{
		ulBufLen = ulSize;
		memcpy(aucCmdBuf + 2, &ulOffset, 2);
		memcpy(aucCmdBuf + 2 + 2 + 2 + ulNameLen, &ulBufLen, 2);
		memcpy(aucCmdBuf + 2 + 2 + 2 + ulNameLen + 2, pbData + ulTimes * MAX_PACKAGE,  ulBufLen);
		ulTmpLen = 2 + 2 + 2 + ulNameLen + 2 + ulBufLen;

        ulRet = CMD_Transmit(phAppHandle->phDev, CMD_WRITE_FILE, 0x00, 0x00,
            aucCmdBuf, ulTmpLen, NULL, NULL);

		if(ulRet != SAR_OK)
		{
			return ulRet;
		}
	}

	return SAR_OK;
}

/*----------------------------------------------*
 * 容器管理                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_CreateContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, HCONTAINER *phContainer)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
    CONTAINERHANDLE_S *pContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulNameLen = 0;

    /* 参数检查 */
	if ((NULL == hApplication)
        || (NULL == szContainerName)
        || (NULL == phContainer))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulNameLen = (ULONG)strlen(szContainerName);
	if(ulNameLen > 64)
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

    phAppHandle = (APPHANDLE_S *)hApplication;

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, szContainerName, ulNameLen);
    ulCmdLen += ulNameLen;

    ulOutLen = 2;
    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_CREATE_CONTAINER, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    pContainer = (CONTAINERHANDLE_S *)ICA_calloc(1, sizeof(CONTAINERHANDLE_S));
    if (NULL == pContainer)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	memcpy(&pContainer->ContainerID, aucOutBuf, 2);
	pContainer->AppID = phAppHandle->usAppID;
	pContainer->phDev = phAppHandle->phDev;

    *phContainer = (HCONTAINER)pContainer;

	return ulRet;
}

ULONG DEVAPI SKF_DeleteContainer(HAPPLICATION hApplication, LPSTR szContainerName)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
	ULONG ulNameLen = 0;

    /* 参数检查 */
	if ((NULL == hApplication)
        || (NULL == szContainerName))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulNameLen = (ULONG)strlen(szContainerName);
	if(ulNameLen > 64)
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

    phAppHandle = (APPHANDLE_S *)hApplication;

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, szContainerName, ulNameLen);
    ulCmdLen += ulNameLen;

    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_DELETE_CONTAINER, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_OpenContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, HCONTAINER *phContainer)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
    CONTAINERHANDLE_S *pContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulNameLen = 0;

    /* 参数检查 */
	if ((NULL == hApplication)
        || (NULL == szContainerName)
        || (NULL == phContainer))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	ulNameLen = (ULONG)strlen(szContainerName);
	if(ulNameLen > 64)
    {
        LOG_ErrNo(SAR_NAMELENERR);
		return SAR_NAMELENERR;
    }

    phAppHandle = (APPHANDLE_S *)hApplication;

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + 2, szContainerName, ulNameLen);
    ulCmdLen += ulNameLen;

    ulOutLen = 2;
    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_OPEN_CONTAINER, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
    if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    pContainer = (CONTAINERHANDLE_S *)ICA_calloc(1, sizeof(CONTAINERHANDLE_S));
    if (NULL == pContainer)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	memcpy(&pContainer->ContainerID, aucOutBuf, 2);
	pContainer->AppID = phAppHandle->usAppID;
	pContainer->phDev = phAppHandle->phDev;

    *phContainer = (HCONTAINER)pContainer;

	return ulRet;
}

ULONG DEVAPI SKF_CloseContainer(HCONTAINER hContainer)
{
	ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *pContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
	if (NULL == hContainer)
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    pContainer = (CONTAINERHANDLE_S *)hContainer;

	memcpy(aucCmdBuf, &pContainer->AppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pContainer->ContainerID, 2);
    ulCmdLen += 2;

    ulRet = CMD_Transmit(pContainer->phDev, CMD_CLOSE_CONTAINER, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	ICA_free(pContainer);

	return ulRet;
}

ULONG DEVAPI SKF_EnumContainer(HAPPLICATION hApplication,
    LPSTR szContainerName, ULONG *pulSize)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
	if ((NULL == hApplication)
        || (NULL == pulSize))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	*pulSize = MAX_BUF/2;
	if (NULL == szContainerName)
	{
		return SAR_OK;
	}

    phAppHandle = (APPHANDLE_S *)hApplication;

	memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
    ulCmdLen += 2;

    ulRet = CMD_Transmit(phAppHandle->phDev, CMD_ENUM_CONTAINER, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, szContainerName, pulSize);

	return ulRet;
}

ULONG DEVAPI SKF_GetContainerType(HCONTAINER hContainer, ULONG *pulContainerType)
{
	ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *pContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 1;

    /* 参数检查 */
	if ((NULL == hContainer)
        || (NULL == pulContainerType))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

    pContainer = (CONTAINERHANDLE_S *)hContainer;

	memcpy(aucCmdBuf, &pContainer->ContainerID, 2);
    ulCmdLen = 2;

    ulRet = CMD_Transmit(pContainer->phDev, CMD_CONTYPE, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
    if (SAR_OK == ulRet)
    {
    	*pulContainerType = aucOutBuf[0];
    }

	return ulRet;
}

ULONG DEVAPI SKF_ImportCertificate(HCONTAINER hContainer,
    BOOL bSignFlag, BYTE *pbCert, ULONG ulCertLen)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *pContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF*2] = {0};
    ULONG ulCmdLen = 0;
    UCHAR ucCertType = 0;

	/* 参数检查 */
	if ((NULL == hContainer)
        || (NULL == pbCert))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	pContainer = (CONTAINERHANDLE_S *)hContainer;

    /* TRUE表示签名证书,FALSE表示加密证书 */
	ucCertType = bSignFlag;
	memcpy(aucCmdBuf, &pContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pContainer->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &ucCertType, 1);
	ulCmdLen += 1;
	memcpy(aucCmdBuf + ulCmdLen, &ulCertLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbCert, ulCertLen);
	ulCmdLen += ulCertLen;

    ulRet = CMD_Transmit(pContainer->phDev, CMD_IMPORT_CERTIFICATE, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_ExportCertificate(HCONTAINER hContainer,
    BOOL bSignFlag, BYTE *pbCert, ULONG *pulCertLen)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *pContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulOutLen = MAX_BUF;

    /* 参数检查 */
	if ((NULL == hContainer)
        || (NULL == pulCertLen))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	*pulCertLen = MAX_BUF;
	if (NULL == pbCert)
	{
		return SAR_OK;
	}

	pContainer = (CONTAINERHANDLE_S *)hContainer;

	memcpy(aucCmdBuf, &pContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pContainer->ContainerID, 2);
	ulCmdLen += 2;

    ulRet = CMD_Transmit(pContainer->phDev, CMD_EXPORT_CERTIFICATE, (UCHAR)bSignFlag, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);

    memcpy(pulCertLen, aucOutBuf, 4);
    memcpy(pbCert, aucOutBuf + 4, *pulCertLen);

	return ulRet;
}

/*----------------------------------------------*
 * 密码服务                                     *
 *----------------------------------------------*/

ULONG DEVAPI SKF_GenRandom(DEVHANDLE hDev, BYTE *pbRandom, ULONG ulRandomLen)
{
	ULONG ulRet = SAR_OK;
    BYTE *pData = NULL;
    ULONG ulRecLen = 0;
    ULONG ulOutLen = 256;
    ULONG i = 0;

    /* 参数检查 */
    if ((NULL == hDev) || (NULL == pbRandom))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	pData = pbRandom;
	ulRecLen = ulRandomLen%256;
	ulRandomLen -= ulRecLen;

	for (i = 0; i < ulRandomLen/256; i++)
	{
        ulRet = CMD_Transmit(hDev, CMD_GEN_RANDOM, 0x00, 0x00,
        NULL, 0, pData, &ulOutLen);
		if (ulRet != SAR_OK)
		{
			return ulRet;
		}

		pData += 256;
	}

    ulRet = CMD_Transmit(hDev, CMD_GEN_RANDOM, 0x00, 0x00,
        NULL, 0, pData, &ulRecLen);

	return ulRet;
}

ULONG DEVAPI SKF_GenExtRSAKey(DEVHANDLE hDev, ULONG ulBitsLen, PRSAPRIVATEKEYBLOB pBlob)
{
	ULONG ulRet = SAR_OK;
    UCHAR aucCmdBuf[16] = {0};
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	USHORT usBitLen = 0;
    USHORT usByteLen = 0;
    USHORT usWordLen = 0;

    /* 参数检查 */
    if ((NULL == hDev) || (NULL == pBlob))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	usBitLen = (USHORT)ulBitsLen;
	usByteLen = (USHORT)(ulBitsLen/8);
	usWordLen = (USHORT)(ulBitsLen/16);

	memcpy(aucCmdBuf, (USHORT *)&usBitLen, 2);
    ulCmdLen = 2;
    ulOutLen = sizeof(RSAPRIVATEKEYBLOB);

    ulRet = CMD_Transmit(hDev, CMD_GEN_EXTRSAKEY, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	memset(pBlob, 0x00, sizeof(RSAPRIVATEKEYBLOB));
	pBlob->BitLen = usBitLen;
	memcpy(pBlob->Modulus + MAX_RSA_MODULUS_LEN - usByteLen, aucOutBuf, usByteLen);
	memcpy(pBlob->PublicExponent, aucOutBuf + usByteLen, 4);
	memcpy(pBlob->PrivateExponent + MAX_RSA_MODULUS_LEN - usByteLen, aucOutBuf + usByteLen + 4, usByteLen);
	memcpy(pBlob->Prime1 + MAX_RSA_PRIME_LEN - usWordLen, aucOutBuf + usByteLen * 2 + 4, usWordLen);
	memcpy(pBlob->Prime2 + MAX_RSA_PRIME_LEN - usWordLen, aucOutBuf + 5 * usWordLen + 4, usWordLen);
	memcpy(pBlob->Prime1Exponent + MAX_RSA_PRIME_LEN - usWordLen, aucOutBuf + 6 * usWordLen + 4, usWordLen);
	memcpy(pBlob->Prime2Exponent + MAX_RSA_PRIME_LEN - usWordLen, aucOutBuf + 7 * usWordLen + 4, usWordLen);
	memcpy(pBlob->Coefficient + MAX_RSA_PRIME_LEN - usWordLen, aucOutBuf + 8 * usWordLen + 4, usWordLen);

	return ulRet;
}

ULONG DEVAPI SKF_GenRSAKeyPair(HCONTAINER hContainer,
    ULONG ulBitsLen, PRSAPUBLICKEYBLOB pBlob)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
    ULONG ulBits = 0;
    UCHAR aucCmdBuf[16] = {0};
    ULONG ulCmdLen = 0;
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulOutLen = 0;

    /* 参数检查 */
    if ((NULL == hContainer) || (NULL == pBlob))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	phContainer = (CONTAINERHANDLE_S *)hContainer;
	ulBits = ulBitsLen;
    ulOutLen = ulBits/8 + 4;

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, (USHORT *)&ulBits, 2);
    ulCmdLen += 2;

    ulRet = CMD_Transmit(phContainer->phDev, CMD_GEN_RSAKEYPAIR, 0x01, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
    if(ulRet != SAR_OK)
    {
		return ulRet;
    }

	memset(pBlob, 0x00, sizeof(RSAPUBLICKEYBLOB));
	pBlob->BitLen = ulBits;
	memcpy(pBlob->Modulus + MAX_RSA_MODULUS_LEN - ulBits/8, aucOutBuf, ulBits/8);
	memcpy(pBlob->PublicExponent, aucOutBuf + ulBits/8, 4);

	return ulRet;
}

ULONG DEVAPI SKF_ImportPlainRSAKeyPair(HCONTAINER hContainer, 
									   PRSAPRIVATEKEYBLOB pBlob, 
									   BOOL bSignKeyPair)
{
	ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	ULONG ulCmdLen = 0;
	USHORT usByteLen = 0;
	USHORT usPrimeLen = 0;	

	phContainer = (CONTAINERHANDLE_S *)hContainer;

    /* 参数检查 */
    if ((NULL == phContainer)
        || (NULL == pBlob))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	usByteLen = (USHORT)pBlob->BitLen / 8;
	usPrimeLen = usByteLen / 2;
	
	memcpy(aucCmdBuf, &phContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, (USHORT *)&pBlob->BitLen, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pBlob->Modulus + MAX_RSA_MODULUS_LEN - usByteLen, usByteLen);
    ulCmdLen += usByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pBlob->PublicExponent, 4);
    ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pBlob->PrivateExponent + MAX_RSA_MODULUS_LEN - usByteLen, usByteLen);
    ulCmdLen += usByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pBlob->Prime1 + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulCmdLen += usPrimeLen;
	memcpy(aucCmdBuf + ulCmdLen, pBlob->Prime2 + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulCmdLen += usPrimeLen;
	memcpy(aucCmdBuf + ulCmdLen, pBlob->Prime1Exponent + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulCmdLen += usPrimeLen;
	memcpy(aucCmdBuf + ulCmdLen, pBlob->Prime2Exponent + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulCmdLen += usPrimeLen;
	memcpy(aucCmdBuf + ulCmdLen, pBlob->Coefficient + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulCmdLen += usPrimeLen;
	
    ulRet = CMD_Transmit(phContainer->phDev, CMD_IMPORT_PLAINRSAKEY, bSignKeyPair ? 0x01 : 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;

}

ULONG DEVAPI SKF_ImportRSAKeyPair(HCONTAINER hContainer, ULONG ulSymAlgId,
    BYTE *pbWrappedKey, ULONG ulWrappedKeyLen,
    BYTE *pbEncryptedData, ULONG ulEncryptedDataLen)
{
	ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	ULONG ulCmdLen = 0;

	phContainer = (CONTAINERHANDLE_S *)hContainer;

    /* 参数检查 */
    if ((NULL == phContainer)
        || (NULL == pbWrappedKey)
        || (NULL == pbEncryptedData))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &ulSymAlgId, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &ulWrappedKeyLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbWrappedKey, ulWrappedKeyLen);
	ulCmdLen += ulWrappedKeyLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulEncryptedDataLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbEncryptedData, ulEncryptedDataLen);
	ulCmdLen += ulEncryptedDataLen;

    ulRet = CMD_Transmit(phContainer->phDev, CMD_IMPORT_RSAKEYPAIR, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_RSASignData(HCONTAINER hContainer,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbSignature, ULONG *pulSignLen)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	ULONG ulCmdLen = 0;

	phContainer = (CONTAINERHANDLE_S *)hContainer;

    /* 参数检查 */
    if ((NULL == phContainer)
        || (NULL == pbData)
        || (NULL == pulSignLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }
	if (ulDataLen > 32)		//只支持HASH数据
	{
		return SAR_INDATALENERR;
	}

    *pulSignLen = MAX_RSA_MODULUS_LEN;
	if (NULL == pbSignature)
	{
		return SAR_OK;
	}

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pbData, ulDataLen);
    ulCmdLen += ulDataLen;

    ulRet = CMD_Transmit(phContainer->phDev, CMD_RSASIGNDATA, 0x02, 0x00,
        aucCmdBuf, ulCmdLen, pbSignature, pulSignLen);

	return ulRet;
}

ULONG DEVAPI SKF_RSAVerify(DEVHANDLE hDev, PRSAPUBLICKEYBLOB pRSAPubKeyBlob,
	BYTE *pbData, ULONG ulDataLen, BYTE *pbSignature, ULONG ulSignLen)
{
    ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
	ULONG ulByteLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pRSAPubKeyBlob)
        || (NULL == pbData)
        || (NULL == pbSignature))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }
	if (ulDataLen > 32)		//只支持HASH数据
	{
		return SAR_INDATALENERR;
	}

	ulByteLen = pRSAPubKeyBlob->BitLen/8;

	memcpy(aucCmdBuf, &pRSAPubKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPubKeyBlob->Modulus + MAX_RSA_MODULUS_LEN - ulByteLen, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPubKeyBlob->PublicExponent, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &ulDataLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbData, ulDataLen);
	ulCmdLen += ulDataLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulSignLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbSignature, ulSignLen);
	ulCmdLen += ulSignLen;

    ulRet = CMD_Transmit(hDev, CMD_RSAVERIFY, 0x02, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_RSAExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId,
	PRSAPUBLICKEYBLOB pPubKey, BYTE *pbData, ULONG *pulDataLen, HANDLE *phSessionKey)
{
	ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulByteLen = 0;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (NULL == pPubKey)
        || (NULL == pulDataLen)
        || (NULL == phSessionKey))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    ulByteLen = pPubKey->BitLen/8;
    *pulDataLen = ulByteLen;
	if (NULL == pbData)
	{
		return SAR_OK;
	}

    phContainer = (CONTAINERHANDLE_S *)hContainer;

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &ulAlgId, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &pPubKey->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pPubKey->Modulus + MAX_RSA_MODULUS_LEN - ulByteLen, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pPubKey->PublicExponent, 4);
	ulCmdLen += 4;

    ulOutLen = ulByteLen + 2;
    ulRet = CMD_Transmit(phContainer->phDev, CMD_RSA_EXPORTSESSIONKEY, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
    if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    pKeyHandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (pKeyHandle == NULL)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	pKeyHandle->ulAlgID = ulAlgId;
	pKeyHandle->phDev = phContainer->phDev;
	pKeyHandle->AppID = phContainer->AppID;
	pKeyHandle->ContainerID = phContainer->ContainerID;
	pKeyHandle->KeyID = *(USHORT *)aucOutBuf;
	memcpy(pbData, aucOutBuf + 2, ulByteLen);
	*pulDataLen = ulByteLen;
    *phSessionKey = (HANDLE)pKeyHandle;

	return ulRet;
}

ULONG DEVAPI SKF_ExtRSAPubKeyOperation(DEVHANDLE hDev,
    PRSAPUBLICKEYBLOB pRSAPubKeyBlob, BYTE *pbInput, ULONG ulInputLen,
    BYTE *pbOutput, ULONG *pulOutputLen)
{
    ULONG ulRet = SAR_OK;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
	ULONG ulByteLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pRSAPubKeyBlob)
        || (NULL == pbInput)
        || (NULL == pulOutputLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    ulByteLen = pRSAPubKeyBlob->BitLen/8;
	*pulOutputLen = ulByteLen;
	if (NULL == pbOutput)
	{
		return SAR_OK;
	}

	memcpy(aucCmdBuf, &pRSAPubKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPubKeyBlob->Modulus + MAX_RSA_MODULUS_LEN - ulByteLen, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPubKeyBlob->PublicExponent, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &ulInputLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbInput, ulInputLen);
	ulCmdLen += ulInputLen;

    ulRet = CMD_Transmit(hDev, CMD_ExtRSA_PubOPT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbOutput, pulOutputLen);

	return ulRet;
}

ULONG DEVAPI SKF_ExtRSAPriKeyOperation(DEVHANDLE hDev,
    PRSAPRIVATEKEYBLOB pRSAPriKeyBlob, BYTE *pbInput, ULONG ulInputLen,
    BYTE *pbOutput, ULONG *pulOutputLen)
{
    ULONG ulRet = SAR_OK;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
	ULONG ulByteLen = 0;
	ULONG ulPrimLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pRSAPriKeyBlob)
        || (NULL == pbInput)
        || (NULL == pulOutputLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    ulByteLen = pRSAPriKeyBlob->BitLen/8;
	*pulOutputLen = ulByteLen;
	if (NULL == pbOutput)
	{
		return SAR_OK;
	}

	ulPrimLen = pRSAPriKeyBlob->BitLen/16;

	memcpy(aucCmdBuf, &pRSAPriKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPriKeyBlob->Modulus + MAX_RSA_MODULUS_LEN - ulByteLen, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPriKeyBlob->PublicExponent, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPriKeyBlob->PrivateExponent + MAX_RSA_MODULUS_LEN - ulByteLen, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPriKeyBlob->Prime1 + MAX_RSA_PRIME_LEN - ulPrimLen, ulPrimLen);
	ulCmdLen += ulPrimLen;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPriKeyBlob->Prime2 + MAX_RSA_PRIME_LEN - ulPrimLen, ulPrimLen);
	ulCmdLen += ulPrimLen;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPriKeyBlob->Prime1Exponent + MAX_RSA_PRIME_LEN - ulPrimLen, ulPrimLen);
	ulCmdLen += ulPrimLen;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPriKeyBlob->Prime2Exponent+MAX_RSA_PRIME_LEN-ulPrimLen, ulPrimLen);
	ulCmdLen += ulPrimLen;
	memcpy(aucCmdBuf + ulCmdLen, pRSAPriKeyBlob->Coefficient + MAX_RSA_PRIME_LEN - ulPrimLen, ulPrimLen);
	ulCmdLen += ulPrimLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulInputLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbInput, ulInputLen);
	ulCmdLen += ulInputLen;

    ulRet = CMD_Transmit(hDev, CMD_ExtRSA_PriOPT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbOutput, pulOutputLen);

	return ulRet;
}

ULONG DEVAPI SKF_GenECCKeyPair(HCONTAINER hContainer, ULONG ulAlgId, PECCPUBLICKEYBLOB pBlob)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
	UCHAR aucCmdBuf[16] = {0};
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
    ULONG ulBits = 0;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (ulAlgId != SGD_SM2_1)
        || (NULL == pBlob))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	phContainer = (CONTAINERHANDLE_S *)hContainer;

	ulBits = 256;
	memcpy(aucCmdBuf, &phContainer->AppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, (UCHAR *)&ulBits, 4);
    ulCmdLen += 4;

    ulOutLen = ulBits/4;
    ulRet = CMD_Transmit(phContainer->phDev, CMD_GEN_ECCKEYPAIR, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	memset(pBlob, 0x00, sizeof(ECCPUBLICKEYBLOB));
	pBlob->BitLen = ulBits;
	memcpy(pBlob->XCoordinate + 32, aucOutBuf, ulBits/8);
	memcpy(pBlob->YCoordinate + 32, aucOutBuf + ulBits/8, ulBits/8);

	return ulRet;
}

ULONG DEVAPI SKF_ImportECCKeyPair(HCONTAINER hContainer, PENVELOPEDKEYBLOB pEnvelopedKeyBlob)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulKeyByteLen = 0;
	ULONG ulEncryptedPriKeyLen = 64;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (NULL == pEnvelopedKeyBlob))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

 	phContainer = (CONTAINERHANDLE_S *)hContainer;

	ulKeyByteLen = pEnvelopedKeyBlob->ulBits/8;

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pEnvelopedKeyBlob->Version, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &pEnvelopedKeyBlob->ulSymmAlgID, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &pEnvelopedKeyBlob->ulBits, 4);
	ulCmdLen += 4;	
	memcpy(aucCmdBuf + ulCmdLen, pEnvelopedKeyBlob->ECCCipherBlob.XCoordinate + 32,ulKeyByteLen);
	ulCmdLen += ulKeyByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pEnvelopedKeyBlob->ECCCipherBlob.YCoordinate + 32,ulKeyByteLen);
	ulCmdLen += ulKeyByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pEnvelopedKeyBlob->ECCCipherBlob.HASH, 32);
	ulCmdLen += 32;
	memcpy(aucCmdBuf + ulCmdLen, &pEnvelopedKeyBlob->ECCCipherBlob.CipherLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pEnvelopedKeyBlob->ECCCipherBlob.Cipher, pEnvelopedKeyBlob->ECCCipherBlob.CipherLen);
	ulCmdLen += pEnvelopedKeyBlob->ECCCipherBlob.CipherLen;
	memcpy(aucCmdBuf + ulCmdLen, &pEnvelopedKeyBlob->ulBits, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pEnvelopedKeyBlob->PubKey.XCoordinate + 32, ulKeyByteLen);
	ulCmdLen += ulKeyByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pEnvelopedKeyBlob->PubKey.YCoordinate + 32, ulKeyByteLen);
	ulCmdLen += ulKeyByteLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulEncryptedPriKeyLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pEnvelopedKeyBlob->cbEncryptedPriKey, 64);
	ulCmdLen += 64;

    ulRet = CMD_Transmit(phContainer->phDev, CMD_IMPORT_ECCKEYPAIR, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_ECCSignData(HCONTAINER hContainer,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulBitLen = 0;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (NULL == pbData)
        || (NULL == pSignature))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }
	if (ulDataLen > 32)		//只支持HASH数据
	{
		return SAR_INDATALENERR;
	}

	phContainer = (CONTAINERHANDLE_S *)hContainer;

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pbData, ulDataLen);
	ulCmdLen += ulDataLen;

    ulOutLen = 68;
    ulRet = CMD_Transmit(phContainer->phDev, CMD_ECCSIGNDATA, 0x02, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	ulBitLen = *(ULONG *)aucOutBuf;
	if(ulBitLen > ECC_MAX_XCOORDINATE_BITS_LEN)
	{
		LOG_ErrNo(SAR_INDATALENERR);
		return SAR_INDATALENERR;
	}

	memcpy(pSignature->r+32, aucOutBuf + 4, ulBitLen/8);
	memcpy(pSignature->s+32, aucOutBuf + 4 + ulBitLen/8, ulBitLen/8);

	return ulRet;
}

ULONG DEVAPI SKF_ECCVerify(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature)
{
    ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
	ULONG ulByteLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pECCPubKeyBlob)
        || (NULL == pbData)
        || (NULL == pSignature))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }
	if (ulDataLen > 32)		//只支持HASH数据
	{
		return SAR_INDATALENERR;
	}

    ulByteLen = pECCPubKeyBlob->BitLen/8;

	memcpy(aucCmdBuf, &pECCPubKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pECCPubKeyBlob->XCoordinate+32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pECCPubKeyBlob->YCoordinate+32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulDataLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbData, ulDataLen);
	ulCmdLen += ulDataLen;
	memcpy(aucCmdBuf + ulCmdLen, pSignature->r+32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pSignature->s+32, ulByteLen);
	ulCmdLen += ulByteLen;

    ulRet = CMD_Transmit(hDev, CMD_ECCVERIFY, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_ECCExportSessionKey(HCONTAINER hContainer, ULONG ulAlgId,
    PECCPUBLICKEYBLOB pPubKey, PECCCIPHERBLOB pData, HANDLE *phSessionKey)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulByteLen = 0;
	ULONG ulBitLen = 0;
    ULONG ulCipherLen = 0;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (NULL == pPubKey)
        || (NULL == pData)
        || (NULL == phSessionKey))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    phContainer = (CONTAINERHANDLE_S *)hContainer;

	ulBitLen = pPubKey->BitLen;
	ulByteLen = pPubKey->BitLen/8;

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pPubKey->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pPubKey->XCoordinate+32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pPubKey->YCoordinate+32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulAlgId, 4);
	ulCmdLen += 4;

    ulOutLen = MAX_BUF;
    ulRet = CMD_Transmit(phContainer->phDev, CMD_ECC_EXPORTSESSIONKEY, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    pKeyHandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (pKeyHandle == NULL)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	// Bits
	memcpy(&ulBitLen, aucOutBuf, 4);
	ulByteLen = ulBitLen / 8;
	// X
	memcpy(pData->XCoordinate + (ECC_MAX_XCOORDINATE_BITS_LEN/8 - ulByteLen), aucOutBuf + 4, ulByteLen);
	// Y
	memcpy(pData->YCoordinate + (ECC_MAX_XCOORDINATE_BITS_LEN/8 - ulByteLen), aucOutBuf + 4 + ulByteLen, ulByteLen);
	// HASH
	memcpy(pData->HASH, aucOutBuf + 4 + 2 * ulByteLen, 32);
	// Cipher Len
	memcpy(&pData->CipherLen, aucOutBuf + 4 + 2 * ulByteLen + 32, 4);
	// Cipher
	memcpy(pData->Cipher, aucOutBuf + 4 + 2 * ulByteLen + 32 + 4, pData->CipherLen);
	// Key ID
	memcpy(&pKeyHandle->KeyID, aucOutBuf + 4 + 2 * ulByteLen + 32 + 4 + pData->CipherLen, 2);

	pKeyHandle->ulAlgID = ulAlgId;
	pKeyHandle->phDev = phContainer->phDev;
	pKeyHandle->AppID = phContainer->AppID;
	pKeyHandle->ContainerID = phContainer->ContainerID;	

    *phSessionKey = (HANDLE)pKeyHandle;

	return ulRet;
}

ULONG DEVAPI SKF_ECCExportSessionKeyEx(HANDLE hSessionKey,
    PECCPUBLICKEYBLOB pPubKey, PECCCIPHERBLOB pData)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulByteLen = 0;
	ULONG ulBitLen = 0;
    ULONG ulBits = 0;

    /* 参数检查 */
    if ((NULL == hSessionKey)
        || (NULL == pPubKey)
        || (NULL == pData))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    pKeyHandle = (KEYHANDLE_S *)hSessionKey;

	ulBitLen = pPubKey->BitLen;
	ulByteLen = pPubKey->BitLen/8;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
    memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pPubKey->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pPubKey->XCoordinate + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pPubKey->YCoordinate + 32, ulByteLen);
	ulCmdLen += ulByteLen;

    ulOutLen = 136;
	ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_ECC_EXPORTSESSIONKEYEX, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    memcpy(&ulBits, aucOutBuf, 4);
    if (ulBits != ulBitLen)
    {
        LOG_ErrNo(SAR_MODULUSLENERR);
		return SAR_MODULUSLENERR;
    }
    memcpy(pData->XCoordinate + 32, aucOutBuf + 4, ulBits/8);
	memcpy(pData->YCoordinate + 32, aucOutBuf + ulBits/8 + 4, ulBits/8);
	memcpy(pData->HASH, aucOutBuf + ulBits/4 + 4, 32);
    memcpy(&pData->CipherLen, aucOutBuf + ulBits/4 + 36, 4);
    if (pData->CipherLen > 2048)
    {
        LOG_ErrNo(SAR_MODULUSLENERR);
		return SAR_MODULUSLENERR;
    }
	memcpy(pData->Cipher, aucOutBuf + ulBits/4 + 40, pData->CipherLen);

	return ulRet;
}


ULONG DEVAPI SKF_ExtECCEncrypt(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbPlainText, ULONG ulPlainTextLen, PECCCIPHERBLOB pCipherText)
{
	ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulByteLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pECCPubKeyBlob)
        || (NULL == pbPlainText)
        || (NULL == pCipherText))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	ulByteLen = pECCPubKeyBlob->BitLen/8;

	memcpy(aucCmdBuf, &pECCPubKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pECCPubKeyBlob->XCoordinate + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pECCPubKeyBlob->YCoordinate + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulPlainTextLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbPlainText, ulPlainTextLen);
	ulCmdLen += ulPlainTextLen;

    ulOutLen = 97 + ulPlainTextLen;
    ulRet = CMD_Transmit(hDev, CMD_ECC_ENCRYPT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    memcpy(pCipherText->XCoordinate + 32, aucOutBuf + 1, ulByteLen);
	memcpy(pCipherText->YCoordinate + 32, aucOutBuf + 1 + ulByteLen, ulByteLen);
	memcpy(pCipherText->HASH, aucOutBuf + 1 + ulByteLen*2, 32);
	pCipherText->CipherLen = ulPlainTextLen;
	memcpy(pCipherText->Cipher, aucOutBuf + 1 + ulByteLen*2 + 32, pCipherText->CipherLen);

	return ulRet;
}

ULONG DEVAPI SKF_ExtECCDecrypt(DEVHANDLE hDev, PECCPRIVATEKEYBLOB pECCPriKeyBlob,
    PECCCIPHERBLOB pCipherText, BYTE *pbPlainText, ULONG *pulPlainTextLen)
{
    ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulByteLen = 0;
	ULONG ulBitLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pECCPriKeyBlob)
        || (NULL == pCipherText)
        || (NULL == pulPlainTextLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	*pulPlainTextLen = pCipherText->CipherLen;
	if(pbPlainText == NULL)
	{
		return SAR_OK;
	}

	ulBitLen = pECCPriKeyBlob->BitLen;
	ulByteLen = pECCPriKeyBlob->BitLen/8;

	memcpy(aucCmdBuf, &pECCPriKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pECCPriKeyBlob->PrivateKey + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pCipherText->XCoordinate + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pCipherText->YCoordinate + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pCipherText->HASH, 32);
	ulCmdLen += 32;
	memcpy(aucCmdBuf + ulCmdLen, &pCipherText->CipherLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pCipherText->Cipher, pCipherText->CipherLen);
	ulCmdLen += pCipherText->CipherLen;

    ulOutLen = pCipherText->CipherLen + 4;
    ulRet = CMD_Transmit(hDev, CMD_ECC_DECRYPT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    memcpy(pbPlainText, aucOutBuf + 4, ulOutLen - 4);
	*pulPlainTextLen = ulOutLen - 4;

	return ulRet;
}

ULONG DEVAPI SKF_ExtECCSign(DEVHANDLE hDev, PECCPRIVATEKEYBLOB pECCPriKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature)
{
    ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulByteLen = 0;
	ULONG ulBitLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pECCPriKeyBlob)
        || (NULL == pbData)
        || (NULL == pSignature))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    ulByteLen = pECCPriKeyBlob->BitLen/8;

	memcpy(aucCmdBuf, &pECCPriKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + 4, pECCPriKeyBlob->PrivateKey + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + 4 + ulByteLen, &ulDataLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + 4 + ulByteLen + 4, pbData, ulDataLen);
	ulCmdLen += ulDataLen;

    ulOutLen = 64;
    ulRet = CMD_Transmit(hDev, CMD_EXT_ECCSIGN, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	memcpy(pSignature->r + 32, aucOutBuf, ulByteLen);
	memcpy(pSignature->s + 32, aucOutBuf + ulByteLen, ulByteLen);

	return ulRet;
}

ULONG DEVAPI SKF_ExtECCVerify(DEVHANDLE hDev, PECCPUBLICKEYBLOB pECCPubKeyBlob,
    BYTE *pbData, ULONG ulDataLen, PECCSIGNATUREBLOB pSignature)
{
    ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
	ULONG ulByteLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pECCPubKeyBlob)
        || (NULL == pbData)
        || (NULL == pSignature))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	ulByteLen = pECCPubKeyBlob->BitLen/8;

	memcpy(aucCmdBuf, &pECCPubKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf+ulCmdLen, pECCPubKeyBlob->XCoordinate + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pECCPubKeyBlob->YCoordinate + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulDataLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbData,ulDataLen);
	ulCmdLen += ulDataLen;
	memcpy(aucCmdBuf + ulCmdLen, pSignature->r + 32, ulByteLen);
	ulCmdLen += ulByteLen;
	memcpy(aucCmdBuf + ulCmdLen, pSignature->s + 32, ulByteLen);
	ulCmdLen += ulByteLen;

    ulRet = CMD_Transmit(hDev, CMD_EXT_ECCVERIFY, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_GenerateAgreementDataWithECC(HCONTAINER hContainer,
    ULONG ulAlgId, PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, HANDLE *phAgreementHandle)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
    KEYHANDLE_S *pAgreementHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulBitLen = 0;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (NULL == pTempECCPubKeyBlob)
        || (NULL == pbID)
        || (NULL == phAgreementHandle))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	phContainer = (CONTAINERHANDLE_S *)hContainer;

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &ulAlgId, 4);
    ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &ulIDLen, 4);
    ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbID, ulIDLen);
    ulCmdLen += ulIDLen;

    ulOutLen = 72;
    ulRet = CMD_Transmit(phContainer->phDev, CMD_GEN_AGREEDATA_ECC, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	ulBitLen = pTempECCPubKeyBlob->BitLen = *(ULONG *)aucOutBuf;
	memcpy(pTempECCPubKeyBlob->XCoordinate + 32, aucOutBuf + 4, ulBitLen/8);
	memcpy(pTempECCPubKeyBlob->YCoordinate + 32, aucOutBuf + 4 + ulBitLen/8, ulBitLen/8);

    pAgreementHandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (pAgreementHandle == NULL)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	pAgreementHandle->ulAlgID = ulAlgId;
	pAgreementHandle->AppID = phContainer->AppID;
	pAgreementHandle->ContainerID = phContainer->ContainerID;
	pAgreementHandle->phDev = phContainer->phDev;
	pAgreementHandle->KeyHandle = *(ULONG *)(aucOutBuf + 4 + ulBitLen/4);
    *phAgreementHandle = (HANDLE)pAgreementHandle;

	return ulRet;
}

ULONG DEVAPI SKF_GenerateAgreementDataAndKeyWithECC(HANDLE hContainer, ULONG ulAlgId,
    PECCPUBLICKEYBLOB pSponsorECCPubKeyBlob,
    PECCPUBLICKEYBLOB pSponsorTempECCPubKeyBlob,
    PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, BYTE *pbSponsorID, ULONG ulSponsorIDLen,
    HANDLE *phKeyHandle)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
    KEYHANDLE_S *pkeyhandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulBitLen = 0;
    ULONG ulTmpBitLen = 0;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (NULL == pSponsorECCPubKeyBlob)
        || (NULL == pSponsorTempECCPubKeyBlob)
        || (NULL == pTempECCPubKeyBlob)
        || (NULL == pbID)
        || (NULL == pbSponsorID)
        || (NULL == phKeyHandle))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	phContainer = (CONTAINERHANDLE_S *)hContainer;

    ulBitLen = pSponsorECCPubKeyBlob->BitLen;

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &ulAlgId, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &ulBitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pSponsorECCPubKeyBlob->XCoordinate + 32, ulBitLen/8);
	ulCmdLen += ulBitLen/8;
	memcpy(aucCmdBuf + ulCmdLen, pSponsorECCPubKeyBlob->YCoordinate + 32, ulCmdLen);
	ulCmdLen += ulBitLen/8;

	/* 发起放临时会话密钥 */
	ulTmpBitLen = pSponsorTempECCPubKeyBlob->BitLen;
	memcpy(aucCmdBuf + ulCmdLen, &ulTmpBitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pSponsorTempECCPubKeyBlob->XCoordinate + 32, ulTmpBitLen/8);
	ulCmdLen += ulTmpBitLen/8;
	memcpy(aucCmdBuf + ulCmdLen, pSponsorTempECCPubKeyBlob->YCoordinate + 32, ulTmpBitLen/8);
	ulCmdLen += ulTmpBitLen/8;

	/* 发起方ID */
	memcpy(aucCmdBuf + ulCmdLen, &ulSponsorIDLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbSponsorID,  ulSponsorIDLen);
	ulCmdLen += ulSponsorIDLen;

	/* 响应方ID */
	memcpy(aucCmdBuf + ulCmdLen, (UCHAR *)&ulIDLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen,  pbID, ulIDLen);
	ulCmdLen += ulIDLen;

    ulOutLen = 72;
    ulRet = CMD_Transmit(phContainer->phDev, CMD_GEN_AGREEDATAKEY_ECC, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	ulBitLen = *(ULONG *)aucOutBuf;
	pTempECCPubKeyBlob->BitLen = ulBitLen;
	memcpy(pTempECCPubKeyBlob->XCoordinate + 32, aucOutBuf + 4, ulBitLen/8);
	memcpy(pTempECCPubKeyBlob->YCoordinate + 32, aucOutBuf + 4 + ulBitLen/8, ulBitLen/8);

	/* 返回密钥句柄 */
    pkeyhandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (pkeyhandle == NULL)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	pkeyhandle->KeyID = (USHORT)*(ULONG *)(aucOutBuf + 4 + ulBitLen/4);
	pkeyhandle->AppID = phContainer->AppID;
	pkeyhandle->ContainerID = phContainer->ContainerID;
	pkeyhandle->ulAlgID = ulAlgId;
	pkeyhandle->phDev = phContainer->phDev;
    *phKeyHandle = (HANDLE)pkeyhandle;

	return ulRet;
}

ULONG DEVAPI SKF_GenerateKeyWithECC(HANDLE hAgreementHandle,
    PECCPUBLICKEYBLOB pECCPubKeyBlob,
    PECCPUBLICKEYBLOB pTempECCPubKeyBlob,
    BYTE *pbID, ULONG ulIDLen, HANDLE *phKeyHandle)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *phAgreementHandle = NULL;
    KEYHANDLE_S *pkeyhandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulBitLen = 0;
    ULONG ulTmpBitLen = 0;

    /* 参数检查 */
    if ((NULL == hAgreementHandle)
        || (NULL == pECCPubKeyBlob)
        || (NULL == pTempECCPubKeyBlob)
        || (NULL == pbID)
        || (NULL == phKeyHandle))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	phAgreementHandle = (KEYHANDLE_S *)hAgreementHandle;

	ulBitLen = pECCPubKeyBlob->BitLen;

	memcpy(aucCmdBuf, (UCHAR *)&phAgreementHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phAgreementHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phAgreementHandle->KeyHandle, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &pECCPubKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &pECCPubKeyBlob->XCoordinate + 32, ulBitLen/8);
	ulCmdLen += ulBitLen/8;
	memcpy(aucCmdBuf + ulCmdLen, &pECCPubKeyBlob->YCoordinate + 32, ulBitLen/8);
	ulCmdLen += ulBitLen/8;

	/* 临时密钥 */
	ulTmpBitLen = pTempECCPubKeyBlob->BitLen;
	memcpy(aucCmdBuf + ulCmdLen, &pTempECCPubKeyBlob->BitLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &pTempECCPubKeyBlob->XCoordinate+32, ulTmpBitLen/8);
	ulCmdLen += ulTmpBitLen/8;
	memcpy(aucCmdBuf + ulCmdLen, &pTempECCPubKeyBlob->YCoordinate+32, ulTmpBitLen/8);
	ulCmdLen += ulTmpBitLen/8;

	memcpy(aucCmdBuf + ulCmdLen, &ulIDLen, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pbID,  ulIDLen);
	ulCmdLen+=ulIDLen;

    ulOutLen = 4;
    ulRet = CMD_Transmit(phAgreementHandle->phDev, CMD_GEN_KEY_ECC, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    /* 返回密钥句柄 */
    pkeyhandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (pkeyhandle == NULL)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	pkeyhandle->KeyID = (USHORT)*(ULONG*)aucOutBuf;
	pkeyhandle->AppID = phAgreementHandle->AppID;
	pkeyhandle->ContainerID = phAgreementHandle->ContainerID;
	pkeyhandle->ulAlgID = phAgreementHandle->ulAlgID;
	pkeyhandle->phDev = phAgreementHandle->phDev;
    *phKeyHandle = (HANDLE)pkeyhandle;

	return ulRet;
}

ULONG DEVAPI SKF_ExportPublicKey(HCONTAINER hContainer, BOOL bSignFlag,
    BYTE* pbBlob, ULONG* pulBlobLen)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulBitLen = 0;
    ULONG ulContainerType = 0;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (NULL == pulBlobLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	SKF_GetContainerType(hContainer, &ulContainerType);

	if (ulContainerType == 1)
	{
		*pulBlobLen = sizeof(RSAPUBLICKEYBLOB);
	}
    else if (ulContainerType == 2)
	{
		*pulBlobLen = sizeof(ECCPUBLICKEYBLOB);
	}

	if (pbBlob == NULL)
	{
		return SAR_OK;
	}

	phContainer = (CONTAINERHANDLE_S *)hContainer;

	memcpy(aucCmdBuf, &phContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
	ulCmdLen += 2;

    ulRet = CMD_Transmit(phContainer->phDev, CMD_EXPORT_PUBKEY, (UCHAR)bSignFlag, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, pulBlobLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

	if (ulContainerType == 1)
	{
		RSAPUBLICKEYBLOB *pRSAPubBlob = (RSAPUBLICKEYBLOB*)pbBlob;
		memset(pRSAPubBlob, 0x00, sizeof(RSAPUBLICKEYBLOB));
		memcpy(&pRSAPubBlob->BitLen, aucOutBuf, 2);
		memcpy(pRSAPubBlob->PublicExponent, aucOutBuf + 2, 4);
		memcpy(pRSAPubBlob->Modulus + MAX_RSA_MODULUS_LEN - pRSAPubBlob->BitLen/8, aucOutBuf + 6, pRSAPubBlob->BitLen/8);
		*pulBlobLen = sizeof(RSAPUBLICKEYBLOB);
	}
	else if(ulContainerType == 2)
	{
		ECCPUBLICKEYBLOB *pEccBlob = (ECCPUBLICKEYBLOB *)pbBlob;
		memset(pEccBlob, 0x00, sizeof(ECCPUBLICKEYBLOB));
		memcpy(&pEccBlob->BitLen, aucOutBuf, 4);
		memcpy(pEccBlob->XCoordinate + 32, aucOutBuf + 4, pEccBlob->BitLen/8);
		memcpy(pEccBlob->YCoordinate + 32, aucOutBuf + 4 + pEccBlob->BitLen/8, pEccBlob->BitLen/8);
		*pulBlobLen = sizeof(ECCPUBLICKEYBLOB);//4
	}

	return ulRet;
}


ULONG DEVAPI SKF_ImportPublicKey(HCONTAINER hContainer, BOOL bSignFlag, 
	BYTE *pbBlob, ULONG ulBlobLen, HANDLE *phKey)
{
	return SAR_FAIL;
}

ULONG DEVAPI SKF_ImportSessionKey(HCONTAINER hContainer,
    ULONG ulAlgId, BYTE *pbWrapedData, ULONG ulWrapedLen, HANDLE *phKey)
{
    ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	ULONG ulBitLen = 0;
	ULONG ulConType = 0;

    /* 参数检查 */
    if ((NULL == hContainer)
        || (NULL == pbWrapedData)
        || (NULL == phKey))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	phContainer = (CONTAINERHANDLE_S *)hContainer;

	// 获取容器类型
	ulRet = SKF_GetContainerType(hContainer, &ulConType);
	if (0 != ulRet || (1!=ulConType && 2!=ulConType))
	{
		return ulRet;
	}

	// 填充数据
	memcpy(aucCmdBuf, &phContainer->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &ulAlgId, 4);
	ulCmdLen += 4;
	if (1 == ulConType)		//RSA
	{
		memcpy(aucCmdBuf + ulCmdLen, &ulWrapedLen, 4);
		ulCmdLen += 4;
		memcpy(aucCmdBuf + ulCmdLen, pbWrapedData, ulWrapedLen);
		ulCmdLen += ulWrapedLen;
	}
	else					//ECC
	{		
		ULONG ulKeyBits = 256;
		ULONG ulKeyBytes = 32;
		PECCCIPHERBLOB pEccCipherBlob = (PECCCIPHERBLOB)pbWrapedData;

		ulWrapedLen = 4 + 2 * ulKeyBytes + 32 + 4 + pEccCipherBlob->CipherLen;
		memcpy(aucCmdBuf + ulCmdLen, &ulWrapedLen, 4);
		ulCmdLen += 4;
		memcpy(aucCmdBuf + ulCmdLen, &ulKeyBits, 4);
		ulCmdLen += 4;
		memcpy(aucCmdBuf + ulCmdLen, pEccCipherBlob->XCoordinate + (ECC_MAX_XCOORDINATE_BITS_LEN/8 - ulKeyBytes), ulKeyBytes);
		ulCmdLen += ulKeyBytes;
		memcpy(aucCmdBuf + ulCmdLen, pEccCipherBlob->YCoordinate + (ECC_MAX_XCOORDINATE_BITS_LEN/8 - ulKeyBytes), ulKeyBytes);
		ulCmdLen += ulKeyBytes;
		memcpy(aucCmdBuf + ulCmdLen, pEccCipherBlob->HASH, 32);
		ulCmdLen += 32;
		memcpy(aucCmdBuf + ulCmdLen, &pEccCipherBlob->CipherLen, 4);
		ulCmdLen += 4;
		memcpy(aucCmdBuf + ulCmdLen, pEccCipherBlob->Cipher, pEccCipherBlob->CipherLen);
		ulCmdLen += pEccCipherBlob->CipherLen;
	}

    ulOutLen = 2;
    ulRet = CMD_Transmit(phContainer->phDev, CMD_IMPORT_SESSIONKEY, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    pKeyHandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (pKeyHandle == NULL)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	pKeyHandle->ulAlgID = ulAlgId;
	pKeyHandle->phDev = phContainer->phDev;
	pKeyHandle->AppID = phContainer->AppID;
	pKeyHandle->ContainerID = phContainer->ContainerID;
	pKeyHandle->KeyID = (aucOutBuf[1]<<8) + aucOutBuf[0];
    *phKey = (HANDLE)pKeyHandle;

	return ulRet;
}

ULONG DEVAPI SKF_SetSymmKey(DEVHANDLE hDev,
    BYTE *pbKey, ULONG ulAlgID, HANDLE *phKey)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
	UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	USHORT usKeyLen = 16;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pbKey)
        || (NULL == phKey))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	usKeyLen = 16;
    if (ulAlgID == SGD_AES192_ECB || ulAlgID == SGD_AES192_CBC)
    {
        usKeyLen = 24;
    }
    else if (ulAlgID == SGD_AES256_ECB || ulAlgID == SGD_AES256_CBC ||
		     ulAlgID == SGD_SM1_ECB || ulAlgID == SGD_SM1_CBC )
    {
        usKeyLen = 32;
    }

	// AppID和ConID在COS内会被忽略，故为0
	memset(aucCmdBuf, 0, 4);
	ulCmdLen = 4;
	// 密钥算法
	memcpy(aucCmdBuf + ulCmdLen, &ulAlgID, 4);
	ulCmdLen += 4;
	// 密钥长度
	memcpy(aucCmdBuf + ulCmdLen, &usKeyLen, 2);
    ulCmdLen += 2;
	// 密钥数据
	memcpy(aucCmdBuf + ulCmdLen, pbKey, usKeyLen);
    ulCmdLen += usKeyLen;

    ulOutLen = 2;
    ulRet = CMD_Transmit(hDev, CMD_IMPORT_SYMMKEY, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, aucOutBuf, &ulOutLen);
	if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    pKeyHandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (NULL == pKeyHandle)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	pKeyHandle->ulAlgID = ulAlgID;
	pKeyHandle->phDev = hDev;
	pKeyHandle->AppID = 0;
	pKeyHandle->ContainerID = 0;
	pKeyHandle->KeyID = (aucOutBuf[1]<<8) + aucOutBuf[0];

    *phKey = (HANDLE)pKeyHandle;

	return ulRet;
}

ULONG DEVAPI SKF_EncryptInit(HANDLE hKey, BLOCKCIPHERPARAM EncryptParam)
{
	ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
	if(NULL == hKey)
	{
		LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	g_ulPaddingType = EncryptParam.PaddingType;

    pKeyHandle = (KEYHANDLE_S *)hKey;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ulAlgID, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, (UCHAR *)&EncryptParam, sizeof(BLOCKCIPHERPARAM));
	ulCmdLen += sizeof(BLOCKCIPHERPARAM);

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_ENCRYPT_INIT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_Encrypt(HANDLE hKey,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen)
{
	ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
	UCHAR aucPad[16] = {0};
	ULONG ulPadByte = 0;
    ULONG i = 0;

    /* 参数检查 */
    if ((NULL == hKey)
        || (NULL == pbData)
        || (NULL == pulEncryptedLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	memset(aucPad, 0x10, 16); /* 没有使用 */
	if (g_ulPaddingType && (ulDataLen%16))
	{
		ulPadByte = 16 - (ulDataLen%16);

		for (i=0; i<ulPadByte; i++)
		{
			memcpy(pbData + ulDataLen + i, (UCHAR *)&ulPadByte, 1);
		}

		ulDataLen += ulPadByte;
	}

	*pulEncryptedLen = ulDataLen;
	if (NULL == pbEncryptedData)
	{
		return SAR_OK;
	}

    pKeyHandle = (KEYHANDLE_S *)hKey;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pbData, ulDataLen);
	ulCmdLen += ulDataLen;

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_ENCRYPT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbEncryptedData, pulEncryptedLen);

	g_ulPaddingType = 0;

	return ulRet;
}

ULONG DEVAPI SKF_EncryptUpdate(HANDLE hKey,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulPadDataLen = 0;
	UCHAR aucPad[16] = {0};
	ULONG ulPadByte = 0;
    ULONG i = 0;

    /* 参数检查 */
    if ((NULL == hKey)
        || (NULL == pbData)
        || (NULL == pulEncryptedLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    pKeyHandle = (KEYHANDLE_S *)hKey;

	if (g_ulPaddingType)
	{
		if (ulDataLen%16)
		{

			ulPadDataLen = ulDataLen - (ulDataLen%16);
			ulPadByte = 16 - (ulDataLen%16);
			g_ulPadLen = ulDataLen%16;
			memcpy(aucPad, pbData + ulPadDataLen, ulDataLen%16);
			for (i=0; i<ulPadByte; i++)
			{
				memcpy(aucPad + g_ulPadLen + i, (UCHAR*)&ulPadByte, 1);
			}
			memcpy(g_aucPadByte, aucPad, 16);

		}
		else
		{
			ulPadDataLen = ulDataLen;
			g_ulPadLen = 16;
		}
	}
	else
	{
		if (ulDataLen%16)
		{
            LOG_ErrNo(SAR_INDATAERR);
			return SAR_INDATAERR;
		}

		ulPadDataLen = ulDataLen;
		g_ulPadLen = 0;

	}

	*pulEncryptedLen = ulPadDataLen;
	if (NULL == pbEncryptedData)
	{
		return SAR_OK;
	}

    memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pbData, ulPadDataLen);
	ulCmdLen += ulPadDataLen;

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_ENCRYPT_UPDATA, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbEncryptedData, pulEncryptedLen);

	return ulRet;
}

ULONG DEVAPI SKF_EncryptFinal(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG *pulEncryptedDataLen)
{
	ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;

    /* 参数检查 */
    if ((NULL == hKey)
        || (NULL == pulEncryptedDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	if ((g_ulPaddingType) && (g_ulPadLen == 16))
	{
		memset(g_aucPadByte, 0x10, 16);
		ulOutLen = 16;
	}
	else
	{
		ulOutLen = 0;
	}

    *pulEncryptedDataLen = ulOutLen;

	if (pbEncryptedData == NULL)
	{
		return SAR_OK;
	}

    pKeyHandle = (KEYHANDLE_S *)hKey;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, g_aucPadByte, ulOutLen);
	ulCmdLen += ulOutLen;

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_ENCRYPT_FINAL, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbEncryptedData, pulEncryptedDataLen);

	g_ulPaddingType = 0;

	return ulRet;
}

ULONG DEVAPI SKF_DecryptInit(HANDLE hKey, BLOCKCIPHERPARAM DecryptParam)
{
	ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

	/* 参数检查 */
	if(NULL == hKey)
	{
		LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
	}

	g_ulPaddingType = DecryptParam.PaddingType;

    pKeyHandle = (KEYHANDLE_S *)hKey;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ulAlgID, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, &DecryptParam, sizeof(BLOCKCIPHERPARAM));
	ulCmdLen += sizeof(BLOCKCIPHERPARAM);

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_DECRYPT_INIT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_Decrypt(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG ulEncryptedLen, BYTE *pbData, ULONG *pulDataLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

	/* 参数检查 */
    if ((NULL == hKey)
        || (NULL == pbEncryptedData)
        || (ulEncryptedLen%16)
        || (ulEncryptedLen == 0)
        || (NULL == pulDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	*pulDataLen = ulEncryptedLen;

	if (NULL == pbData)
	{
		return SAR_OK;
	}

    pKeyHandle = (KEYHANDLE_S *)hKey;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pbEncryptedData, ulEncryptedLen);
	ulCmdLen += ulEncryptedLen;

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_DECRYPT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbData, pulDataLen);

	return 	ulRet;
}

ULONG DEVAPI SKF_DecryptUpdate(HANDLE hKey,
    BYTE *pbEncryptedData, ULONG ulEncryptedLen, BYTE *pbData, ULONG *pulDataLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
    if ((NULL == hKey)
        || (NULL == pbEncryptedData)
        || (ulEncryptedLen%16)
        || (ulEncryptedLen == 0)
        || (NULL == pulDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	*pulDataLen = ulEncryptedLen;
	if (NULL == pbData)
	{
		return SAR_OK;
	}

    pKeyHandle = (KEYHANDLE_S *)hKey;

    memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pbEncryptedData, ulEncryptedLen);
	ulCmdLen += ulEncryptedLen;

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_DECRYPT_UPDATA, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbData, pulDataLen);

    return ulRet;
}

ULONG DEVAPI SKF_DecryptFinal(HANDLE hKey,
    BYTE *pbDecryptedData, ULONG *pulDecryptedDataLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;

	/* 参数检查 */
    if ((NULL == hKey)
        || (NULL == pulDecryptedDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }


	if ((g_ulPaddingType) && (g_ulPadLen == 16))
	{
		memset(g_aucPadByte, 0x10, 16);
		ulOutLen = 16;
	}
	else
	{
		ulOutLen = 0;
	}

    pKeyHandle = (KEYHANDLE_S *)hKey;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
    ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, g_aucPadByte, ulOutLen);
	ulCmdLen += ulOutLen;

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_DECRYPT_FINAL, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbDecryptedData, pulDecryptedDataLen);

	g_ulPaddingType = 0;

	return ulRet;
}

ULONG DEVAPI SKF_DigestInit(DEVHANDLE hDev, ULONG ulAlgID,
    ECCPUBLICKEYBLOB *pPubKey, BYTE *pbID, ULONG ulIDLen, HANDLE *phHash)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pHashHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;
    ULONG ulOutLen = 0;
    ULONG ulByteLen = 0;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == phHash))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	if(ulAlgID == SGD_SM3)
	{
		if(ulIDLen != 0)
		{
			if (NULL != pPubKey)
			{
				ulByteLen = pPubKey->BitLen/8;
				memcpy(aucCmdBuf, &pPubKey->BitLen, 4);
				ulCmdLen += 4;
				memcpy(aucCmdBuf + ulCmdLen, pPubKey->XCoordinate + 32, ulByteLen);
				ulCmdLen += ulByteLen;
				memcpy(aucCmdBuf + ulCmdLen, pPubKey->YCoordinate + 32, ulByteLen);
				ulCmdLen += ulByteLen;
			}

			if (NULL == pbID)
			{
				return SAR_OK;
			}

            /* 4字节id, 加32个0 (id值) */
			memcpy(aucCmdBuf + ulCmdLen, &ulIDLen, 4);
			ulCmdLen += 4;
			memcpy(aucCmdBuf + ulCmdLen, pbID, ulIDLen);
			ulCmdLen += ulIDLen;
		}
	}

    ulRet = CMD_Transmit(hDev, CMD_DIGEST_INIT, 0x00, (UCHAR)ulAlgID,
        aucCmdBuf, ulCmdLen, NULL, NULL);
    if (ulRet != SAR_OK)
    {
		return ulRet;
    }

    pHashHandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (NULL == pHashHandle)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	pHashHandle->ulAlgID = ulAlgID;
	pHashHandle->phDev = hDev;
    *phHash = (HANDLE)pHashHandle;

	return ulRet;
}

ULONG DEVAPI SKF_Digest(HANDLE hHash,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbHashData, ULONG *pulHashLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pHashHandle = NULL;
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulOutLen = 0;

    /* 参数检查 */
    if ((NULL == hHash)
        || (NULL == pbData)
        || (NULL == pulHashLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	*pulHashLen = 32;
	if (NULL == pbHashData)
	{
		return SAR_OK;
	}

    pHashHandle = (KEYHANDLE_S *)hHash;

    ulOutLen = 32 + 4;
    ulRet = CMD_Transmit(pHashHandle->phDev, CMD_DIGEST, 0x00, 0x00,
        pbData, ulDataLen, aucOutBuf, &ulOutLen);
    if (ulRet != SAR_OK)
    {
		return ulRet;
    }

	*pulHashLen = *(ULONG *)aucOutBuf;
	memcpy(pbHashData, aucOutBuf + 4, *pulHashLen);

	return ulRet;
}

ULONG DEVAPI SKF_DigestUpdate(HANDLE hHash, BYTE *pbData, ULONG ulDataLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pHashHandle = NULL;

    /* 参数检查 */
    if ((NULL == hHash)
        || (NULL == pbData))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	pHashHandle = (KEYHANDLE_S *)hHash;

    ulRet = CMD_Transmit(pHashHandle->phDev, CMD_DIGEST_UPDATA, 0x00, 0x00,
        pbData, ulDataLen, NULL, NULL);

	return ulRet;
}

ULONG DEVAPI SKF_DigestFinal(HANDLE hHash, BYTE *pbHashData, ULONG *pulHashLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pHashHandle = NULL;
    UCHAR aucOutBuf[MAX_BUF] = {0};
    ULONG ulOutLen = 0;

    /* 参数检查 */
    if ((NULL == hHash)
        || (NULL == pulHashLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	*pulHashLen = 32;
	if (NULL == pbHashData)
	{
		return SAR_OK;
	}

    pHashHandle = (KEYHANDLE_S *)hHash;

    ulOutLen = 32 + 4;
    ulRet = CMD_Transmit(pHashHandle->phDev, CMD_DIGEST_FINAL, 0x00, 0x00,
        NULL, 0, aucOutBuf, &ulOutLen);
    if (ulRet != SAR_OK)
    {
		return ulRet;
    }

	*pulHashLen = *(ULONG*)aucOutBuf;
	memcpy(pbHashData, aucOutBuf + 4, *pulHashLen);

	return ulRet;
}

ULONG DEVAPI SKF_MacInit(HANDLE hKey, BLOCKCIPHERPARAM *pMacParam, HANDLE *phMac)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
    KEYHANDLE_S *pMacHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
    if ((NULL == hKey)
        || (NULL == pMacParam)
        || (NULL == phMac))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	pKeyHandle = (KEYHANDLE_S *)hKey;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ulAlgID, 4);
	ulCmdLen += 4;
	memcpy(aucCmdBuf + ulCmdLen, pMacParam, sizeof(BLOCKCIPHERPARAM));
	ulCmdLen += sizeof(BLOCKCIPHERPARAM);

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_MAC_INIT, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);
    if(ulRet != SAR_OK)
	{
		return ulRet;
	}

    pMacHandle = (KEYHANDLE_S *)ICA_calloc(1, sizeof(KEYHANDLE_S));
    if (NULL == pMacHandle)
    {
        LOG_ErrNo(SAR_MEMORYERR);
		return SAR_MEMORYERR;
    }

	pMacHandle->AppID = pKeyHandle->AppID;
	pMacHandle->ContainerID = pKeyHandle->ContainerID;
	pMacHandle->KeyID = pKeyHandle->KeyID;
	pMacHandle->phDev = pKeyHandle->phDev;
    *phMac = (HANDLE)pMacHandle;

	return ulRet;
}

ULONG DEVAPI SKF_Mac(HANDLE hMac,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbMacData, ULONG *pulMacLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pMacHandle = NULL;
	UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
    if ((NULL == hMac)
        || (NULL == pbData)
        || (NULL == pulMacLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	*pulMacLen = 16;
	if (NULL == pbMacData)
	{
		return SAR_OK;
	}

    pMacHandle = (KEYHANDLE_S *)hMac;

	memcpy(aucCmdBuf, &pMacHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pMacHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pMacHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pbData, ulDataLen);
	ulCmdLen += ulDataLen;

    ulRet = CMD_Transmit(pMacHandle->phDev, CMD_MAC, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbMacData, pulMacLen);

	return ulRet;
}

ULONG DEVAPI SKF_MacUpdate(HANDLE hMac, BYTE *pbData, ULONG ulDataLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pMacHandle = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
    if ((NULL == hMac)
        || (NULL == pbData)
        || (ulDataLen%16))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	pMacHandle = (KEYHANDLE_S *)hMac;

	memcpy(aucCmdBuf, &pMacHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pMacHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pMacHandle->KeyID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, pbData, ulDataLen);
	ulCmdLen += ulDataLen;

    ulRet = CMD_Transmit(pMacHandle->phDev, CMD_MAC_UPDATA, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

	return 	ulRet;
}

ULONG DEVAPI SKF_MacFinal(HANDLE hMac, BYTE *pbMacData, ULONG *pulMacDataLen)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pMacHandle = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
    if ((NULL == hMac)
        || (NULL == pulMacDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	*pulMacDataLen = 16;
	if (NULL == pbMacData)
	{
		return SAR_OK;
	}

    pMacHandle = (KEYHANDLE_S *)hMac;

	memcpy(aucCmdBuf, &pMacHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pMacHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pMacHandle->KeyID, 2);
	ulCmdLen += 2;

    ulRet = CMD_Transmit(pMacHandle->phDev, CMD_MAC_FINAL, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, pbMacData, pulMacDataLen);

	return ulRet;
}

ULONG DEVAPI SKF_CloseHandle(HANDLE hHandle)
{
    ULONG ulRet = SAR_OK;
    KEYHANDLE_S *pKeyHandle = NULL;
    UCHAR aucCmdBuf[MAX_BUF] = {0};
    ULONG ulCmdLen = 0;

    /* 参数检查 */
    if (NULL == hHandle)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	pKeyHandle = (KEYHANDLE_S *)hHandle;

	memcpy(aucCmdBuf, &pKeyHandle->AppID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->ContainerID, 2);
	ulCmdLen += 2;
	memcpy(aucCmdBuf + ulCmdLen, &pKeyHandle->KeyID, 2);
	ulCmdLen += 2;

    ulRet = CMD_Transmit(pKeyHandle->phDev, CMD_DESTROY_SESSIONKEY, 0x00, 0x00,
        aucCmdBuf, ulCmdLen, NULL, NULL);

    ICA_free(pKeyHandle);

	return ulRet;
}

/*----------------------------------------------*
 * 扩展接口                                     *
 *----------------------------------------------*/

BOOL DEVAPI SKF_ExterAuth(HANDLE hHandle, UCHAR *pucKey, ULONG ulLen)
{
    ULONG ulRet = SAR_OK;
	BYTE  bKey[16] = {0};
	BYTE  bData[16] = {0};
    UCHAR pOutData[16] = {0};

    /* 参数检查 */
    if (NULL == hHandle)
    {
        return FALSE;
    }

	ulRet = SKF_GenRandom(hHandle, bData, 8);
    if (SAR_OK != ulRet)
    {
        return FALSE;
    }

	if (NULL == pucKey)
	{
		SMS4_ENC(g_aucDefWorkKey, bData, pOutData, 16);
	}
	else
	{
		memset(bKey, 0, 16);
		memcpy(bKey, pucKey, ulLen);
		SMS4_ENC(bKey, bData, pOutData, 16);
	}

    ulRet = SKF_DevAuth(hHandle, pOutData, 16);
    if (SAR_OK != ulRet)
    {
        return FALSE;
    }

    return TRUE;
}

ULONG DEVAPI SKF_EncryptTunnelUpdate(DEVHANDLE hDev,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbEncryptedData, ULONG *pulEncryptedLen)
{
	ULONG ulRet = SAR_OK;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pbData)
		|| (ulDataLen == 0)
        || (NULL == pulEncryptedLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	/* 数据长度只能为16整数倍 */
	if (ulDataLen % 16 != 0 || ulDataLen > MAX_DATA_BUF)
	{
        return SAR_INDATALENERR;
	}

	if (NULL == pbEncryptedData)
	{
		*pulEncryptedLen = ulDataLen;
		return SAR_OK;
	}
	if (*pulEncryptedLen < ulDataLen)
	{
        return SAR_INDATALENERR;
	}

	/* 采用数据通道加密 */
    ulRet = DataInterface(hDev, pbData, ulDataLen, pbEncryptedData, pulEncryptedLen, ENC);

	return ulRet;
}

ULONG DEVAPI SKF_DecryptTunnelUpdate(DEVHANDLE hDev,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbDecryptedData, ULONG *pulDecryptedLen)
{
	ULONG ulRet = SAR_OK;

    /* 参数检查 */
    if ((NULL == hDev)
        || (NULL == pbData)
        || (ulDataLen == 0)
        || (NULL == pulDecryptedLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }
	
	/* 数据长度只能为16整数倍 */
	if (ulDataLen % 16 != 0 || ulDataLen > MAX_DATA_BUF)
	{
        return SAR_INDATALENERR;
	}
	
	if (NULL == pbDecryptedData)
	{
		*pulDecryptedLen = ulDataLen;
		return SAR_OK;
	}
	if (*pulDecryptedLen < ulDataLen)
	{
        return SAR_INDATALENERR;
	}

    ulRet = DataInterface(hDev, pbData, ulDataLen, pbDecryptedData, pulDecryptedLen, DEC);

    return ulRet;
}


ULONG DEVAPI SKF_DevTransmit(DEVHANDLE hDev, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen)
{
	ULONG ulRet = SAR_OK;
	UCHAR aucCmdBuf[MAX_APDU_LC_LEN] = {0};
	ULONG ulCmdLen = 0;

    /* 参数检查 */
    if ((NULL == hDev) || (MAX_APDU_LC_LEN < ulDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    if (0 < ulDataLen)
    {
        if (NULL == pbData)
        {
            LOG_ErrNo(SAR_INVALIDPARAMERR);
		    return SAR_INVALIDPARAMERR;
        }

        memcpy(aucCmdBuf, pbData, ulDataLen);
        ulCmdLen += ulDataLen;
    }

	ulRet = CMD_Transmit(hDev, bCmd, (UCHAR)iType, (UCHAR)0x00,
        aucCmdBuf, ulCmdLen, pbOutData, pulOutLen);

	return ulRet;
}

ULONG DEVAPI SKF_AppTransmit(HAPPLICATION hApplication, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen)
{
	ULONG ulRet = SAR_OK;
    APPHANDLE_S *phAppHandle = NULL;
	UCHAR aucCmdBuf[MAX_APDU_LC_LEN] = {0};
	ULONG ulCmdLen = 0;

	phAppHandle = (APPHANDLE_S *)hApplication;

    /* 参数检查 */
    if ((NULL == phAppHandle) || (MAX_APDU_LC_LEN < ulDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    memcpy(aucCmdBuf, &phAppHandle->usAppID, 2);
    ulCmdLen += 2;

    if (0 < ulDataLen)
    {
        if (NULL == pbData)
        {
            LOG_ErrNo(SAR_INVALIDPARAMERR);
		    return SAR_INVALIDPARAMERR;
        }

        memcpy(aucCmdBuf + 2, pbData, ulDataLen);
        ulCmdLen += ulDataLen;
    }

	ulRet = CMD_Transmit(phAppHandle->phDev, bCmd, (UCHAR)iType, (UCHAR)0x00,
        aucCmdBuf, ulCmdLen, pbOutData, pulOutLen);

	return ulRet;
}

ULONG DEVAPI SKF_ConTransmit(HCONTAINER hContainer, BYTE bCmd, INT32 iType,
    BYTE *pbData, ULONG ulDataLen, BYTE *pbOutData, ULONG *pulOutLen)
{
	ULONG ulRet = SAR_OK;
    CONTAINERHANDLE_S *phContainer = NULL;
	UCHAR aucCmdBuf[MAX_APDU_LC_LEN] = {0};
	ULONG ulCmdLen = 0;

    /* 参数检查 */
    if ((NULL == hContainer) || (MAX_APDU_LC_LEN < ulDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

    phContainer = (CONTAINERHANDLE_S *)hContainer;

    memcpy(aucCmdBuf, &phContainer->AppID, 2);
    ulCmdLen += 2;
    memcpy(aucCmdBuf + ulCmdLen, &phContainer->ContainerID, 2);
    ulCmdLen += 2;

    if (0 < ulDataLen)
    {
        if (NULL == pbData)
        {
            LOG_ErrNo(SAR_INVALIDPARAMERR);
		    return SAR_INVALIDPARAMERR;
        }

        memcpy(aucCmdBuf + ulCmdLen, pbData, ulDataLen);
        ulCmdLen += ulDataLen;
    }

	ulRet = CMD_Transmit(phContainer->phDev, bCmd, (UCHAR)iType, (UCHAR)0x00,
        aucCmdBuf, ulCmdLen, pbOutData, pulOutLen);

	return ulRet;
}


ULONG DEVAPI SKF_SetDevPath(BYTE *pbDevPath)
{
    ULONG ulRet = SAR_OK;

    ulRet = DEV_SetDevicePath(pbDevPath);

    return ulRet;
}

ULONG DEVAPI SKF_ClearDevPath()
{
    DEV_ClearDevicePath();
    return SAR_OK;
}


ULONG DEVAPI SKF_ExtKeyOperation(HCONTAINER hContainer,BYTE AlgID, BYTE KeyType, BYTE OpType,
	BYTE* pbInput, ULONG ulInputLen, BYTE * pbOutput, ULONG* pulOutputLen)
{
    return 0;
}

ULONG DEVAPI SKF_FormatCD( HANDLE hHandle,
      char* psFileName)
{
	return SAR_OK;
}

ULONG DEVAPI SYNO_GetErrorString(INT32 nError, CHAR* strError)
{
	return SAR_OK;
}

/*-------------------------------------------------------------------
INT WINAPI FU_RandXN(HANDLE hHandle,UINT nMB)
函数功能：测试随机数性能
返回值：成功-RT_OK
失败-其他
--------------------------------------------------------------------*/
ULONG DEVAPI SYNO_RandXN(DEVHANDLE hHandle,UINT nMB)
{
	return SAR_OK;
}

/*--------------------------------------------------
Function: 产生素数
Param: hDev-the handle opened
Return: 参考RETURN CODE
-----------------------------------------------------*/
ULONG DEVAPI SYNO_GenPrime(DEVHANDLE hHandle,BYTE* pPrime,UINT nLen)
{
	return  SAR_OK;
}

/*《《测试函数,不对返回数据组成结构体，直接原始返回(测试用)》》*/
ULONG DEVAPI GenExtRSAKey_Raw(DEVHANDLE hDev, ULONG ulBitsLen , BYTE *pBlob)
{
	return SAR_OK;
}

//void Log(const char* fmt, ...)
//{
//}

ULONG DEVAPI SYNO_Clear(HANDLE hHandle, BYTE type)
{
    return SAR_OK;
}

void hex_dump_mem(unsigned char * PackBuf, int BufLen)
{
    return;
}

ULONG DEVAPI SKF_ImportRSAKey(HCONTAINER hContainer, RSAPRIVATEKEYBLOB *pRSAPriKey, BYTE KeyType)
{
    return SAR_OK;
}

//------设备初始化信息，PC上设置的信息----------
typedef struct _DEVINITINFO
{
	VER_NUM   AppVersion;			//应用版本
	char      Manufacturer[64];		//设备厂商信息
	char      Label[64];			//Label 标志
	char      Issuer[64];			//发行厂商序列号
}DEVINITINFO;

typedef struct _tDevInitParam
{
	DEVINITINFO tInit;
	char       EAkey[32];
	USHORT      CardStatus;			//卡的状态.
}tDevInitParam;

ULONG DEVAPI DevInit(DEVHANDLE hDev, tDevInitParam *pstDevInfo)
{
	return SAR_OK;
}



