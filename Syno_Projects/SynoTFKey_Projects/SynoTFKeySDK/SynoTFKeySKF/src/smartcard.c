/***************************************************************************
 *	File name:SMARTCARD.c
 *	Introduce:Functions definition for smard card object
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "smartcard.h"
#include "device.h"
#include "skfsha1.h"
#include "skfsms4.h"
#include "Synolog.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_LEN 16

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
UCHAR g_SM4_KYE[16] = {0x23,0x45,0xbb,0x5d,0x67,0x8a,0x9c,0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
UCHAR g_aucDefWorkKey[16] = {0x23,0x45,0xbb,0x5d,0x67,0x8a,0x9c,0xdd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*----------------------------------------------*
 * 外部函数                                     *
 *----------------------------------------------*/
extern ULONG SKF_GenRandom(HANDLE hDev, BYTE *pbRandom, ULONG ulRandomLen);

/*----------------------------------------------*
 * 模块内部函数                                 *
 *----------------------------------------------*/

ULONG SendPackage(HANDLE hHandle,
    UCHAR *pucData, ULONG ulDataLen, CMDTYPE_E eCmdType)
{
    ULONG ulRet = SAR_OK;
	ULONG ulOffset = 0;
	ULONG ulWritLen = ulDataLen;
    IODRIVER_S *pstIODriver = NULL;
    RWDEVICE_S stRWDev = {0};

    /* 参数检查 */
    if ((NULL == hHandle) || (NULL == pucData))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    pstIODriver = (IODRIVER_S *)hHandle;
/*  Youfang 2015-09-07
	if (CMD == eCmdType)
	{
		stRWDev.iRWFd = pstIODriver->stVirFile.iCmdFd;
		stRWDev.pucIOBuf = pstIODriver->pucCmdBuf;
	}
	else if (ENC == eCmdType)
	{
        stRWDev.iRWFd = pstIODriver->stVirFile.iEncFd;
        stRWDev.pucIOBuf = pstIODriver->pucDataBuf;
	}
    else if (DEC == eCmdType)
	{
        stRWDev.iRWFd = pstIODriver->stVirFile.iDecFd;
        stRWDev.pucIOBuf = pstIODriver->pucDataBuf;
	}
*/
	/* 三种不同的功能使用同一个文件,只是写数据的偏移位置不同 */
	stRWDev.iRWFd = pstIODriver->stVirFile.iCmdFd;
	if (CMD == eCmdType)
	{
		ulOffset = 0;
		stRWDev.pucIOBuf = pstIODriver->pucCmdBuf;
	}
	else if (DEC == eCmdType)
	{
		ulOffset = MAX_CMD_BUF;
		stRWDev.pucIOBuf = pstIODriver->pucDataBuf;
	}
	else if (ENC == eCmdType)
	{
		ulOffset = MAX_CMD_BUF + MAX_DATA_BUF;
		stRWDev.pucIOBuf = pstIODriver->pucDataBuf;
	}
	ulRet = DEV_Write(&stRWDev, ulOffset, pucData, &ulWritLen);

	return ulRet;
}

ULONG RecvPackage(HANDLE hHandle,
    UCHAR *pucData, ULONG *pulDataLen, CMDTYPE_E eCmdType)
{
    ULONG ulRet = SAR_OK;
	ULONG ulOffset = 0;
    IODRIVER_S *pstIODriver = NULL;
    RWDEVICE_S stRWDev = {0};

    /* 参数检查 */
    if ((NULL == hHandle) || (NULL == pucData) || (NULL == pulDataLen))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    if ((CMD != eCmdType) && (*pulDataLen < MAX_DATA_BUF))
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

	pstIODriver = (IODRIVER_S *)hHandle;
/* Youfang 2015-09-07
	if (CMD == eCmdType)
	{
        stRWDev.iRWFd = pstIODriver->stVirFile.iCmdFd;
        stRWDev.pucIOBuf = pstIODriver->pucCmdBuf;
        *pulDataLen = MAX_CMD_BUF;
	}
	else if (ENC == eCmdType)
	{
        stRWDev.iRWFd = pstIODriver->stVirFile.iEncFd;
        stRWDev.pucIOBuf = pstIODriver->pucDataBuf;
        *pulDataLen = MAX_DATA_BUF;
	}
    else if (DEC == eCmdType)
	{
        stRWDev.iRWFd = pstIODriver->stVirFile.iDecFd;
        stRWDev.pucIOBuf = pstIODriver->pucDataBuf;
        *pulDataLen = MAX_DATA_BUF;
	}
*/
	/* 三种不同的功能使用同一个文件,只是读数据的偏移位置不同 */
	stRWDev.iRWFd = pstIODriver->stVirFile.iCmdFd;
	if (CMD == eCmdType)
	{
		ulOffset = 0;
        stRWDev.pucIOBuf = pstIODriver->pucCmdBuf;
        *pulDataLen = MAX_CMD_BUF;
	}
	else if (DEC == eCmdType)
	{
		ulOffset = MAX_CMD_BUF;
        stRWDev.pucIOBuf = pstIODriver->pucDataBuf;
        *pulDataLen = MAX_DATA_BUF;
	}
	else if (ENC == eCmdType)
	{
		ulOffset = MAX_CMD_BUF + MAX_DATA_BUF;
        stRWDev.pucIOBuf = pstIODriver->pucDataBuf;
        *pulDataLen = MAX_DATA_BUF;
	}
    ulRet = DEV_Read(&stRWDev, ulOffset, pucData, pulDataLen);

	return ulRet;
}

ULONG SM4_MAC(BYTE *pbKey, BYTE *pbRandom,
    BYTE *pbInData, ULONG ulInLen, BYTE *pbOutData, ULONG *pulOutLen)
{
    BYTE bRandom[BLOCK_LEN] = {0};
    BYTE pIN[BLOCK_LEN]={0};
    BYTE bMac[MAX_CMD_BUF] = {0};
    INT32 iMacLen = 0;
    INT32 i, j;

    memcpy(bRandom, pbRandom, BLOCK_LEN/2);
    memcpy(pIN, bRandom, BLOCK_LEN);

    memcpy(bMac, pbInData, ulInLen);
    bMac[ulInLen] = 0x80;
    iMacLen = ulInLen + 1;
    iMacLen += (BLOCK_LEN - (ulInLen + 1) % BLOCK_LEN);

	for(i = 0;i < iMacLen/BLOCK_LEN; i++)
	{
          for(j = 0; j < BLOCK_LEN; j++)
          {
			  pIN[j] = pIN[j] ^ bMac[i*BLOCK_LEN+j];
          }

		  SMS4_ENC(pbKey, pIN, pIN, BLOCK_LEN);
	}

	memcpy(pbOutData, pIN, BLOCK_LEN);
	*pulOutLen = BLOCK_LEN;

    return SAR_OK;
}

ULONG APDUInterface(HANDLE hHandle,
    UCHAR *pInBuf, ULONG ulInLen, UCHAR *pOutBuf, ULONG *pulOutLen)
{
    ULONG ulRet = SAR_OK;
    UCHAR acBuf[MAX_CMD_BUF] = {0};
    ULONG ulBufLen = 0;
    USHORT usLC = 0;
	APDUHEADER_S stCmdHeader = {0};
    CMDTYPE_E eType = CMD;
	UCHAR ucSW = 0;
	USHORT usRespApduLen = 0;
	ULONG ulIndex = 0;
	ULONG ulSendLen = 0;
	ULONG ulRecvLen = 0;
    UCHAR acRandom[16] = {0};
    UCHAR acMac[16] = {0};
    ULONG ulMacLen = 0;

	/* 参数检查 */
	if ((NULL == hHandle) || (pInBuf == NULL))
	{
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
	}
	memcpy(&stCmdHeader, pInBuf, CMD_HEAD_LEN);

	/* 命令标记 */
	acBuf[0] = 0xEF;
	acBuf[1] = 0x01;
	acBuf[2] = 0xE1;
	acBuf[3] = 0x00;
	ulBufLen = 4;

	/* 整个APDU的长度 */
	usLC = (USHORT)ulInLen;
	if (stCmdHeader.CLA & 0x04)
	{
		usLC += 4;		//MAC长度
	}
	acBuf[ulBufLen] = ((USHORT)usLC) & 0x00FF;
	acBuf[ulBufLen + 1] = ((USHORT)usLC >> 8) & 0x00FF;		//small endian mode
	ulBufLen += 2;

	/* 需要计算MAC */
    if (stCmdHeader.CLA & 0x04)
    {
		USHORT usApduLc = 0;
		UCHAR tSM4Buf[64] = {0};
		USHORT tLen = 0;

        ulRet = SKF_GenRandom(hHandle, acRandom, 8);
        if (SAR_OK != ulRet)
    	{
    		return ulRet;
    	}
		
		/* 拷贝APDU头 */
		memcpy(tSM4Buf, pInBuf, CMD_HEAD_LEN);
		tLen += CMD_HEAD_LEN;
		//
		memcpy(acBuf + ulBufLen, pInBuf, CMD_HEAD_LEN);
		ulBufLen += CMD_HEAD_LEN;

		/* APDU的数据长度Lc */
		memcpy(&usApduLc, pInBuf + (CMD_HEAD_LEN + 1), 2); 
		tSM4Buf[tLen] = (usApduLc >> 8) & 0x00FF;
		tSM4Buf[tLen + 1] = usApduLc & 0x00FF;
		tLen += 2;
		tSM4Buf[tLen] = 0x00;
		tSM4Buf[tLen + 1] = 0x00;
		tLen += 2;
		memcpy(tSM4Buf + tLen, pInBuf + CMD_HEAD_LEN + 3, ulInLen - CMD_HEAD_LEN - 3);
		tLen += (ulInLen - CMD_HEAD_LEN - 3);
		//
		usApduLc += 4;
		acBuf[ulBufLen] = 0x00;
		acBuf[ulBufLen + 1] = (usApduLc >> 8) & 0x00FF;
		acBuf[ulBufLen + 2] = usApduLc & 0x00FF;
		ulBufLen += 3;

		/* APDU的数据和Le(实际上做MAC运算的命令都没有Le) */
		memcpy(acBuf + ulBufLen, pInBuf + (CMD_HEAD_LEN + 3), (ulInLen - CMD_HEAD_LEN - 3));
		ulBufLen += (ulInLen - CMD_HEAD_LEN - 3);
		
		/* 计算MAC */  
		SM4_MAC(g_SM4_KYE, acRandom, tSM4Buf, tLen, acMac, &ulMacLen);		//change by comon @ 20150907, SM4_MAC only calc the apdu data.

		/* 将MAC数据添加至末尾(因为没有Le) */
		memcpy(acBuf + ulBufLen, acMac, 4);
		ulBufLen += 4;
    }
	else
	{
		memcpy(acBuf + ulBufLen, pInBuf, ulInLen);
		ulBufLen += ulInLen;
	}

	/* 计算发送数据的长度(必须为512的整数倍) */
	ulSendLen = 0;
	do
	{
		ulSendLen += 512;
		if (ulSendLen >= ulBufLen)
		{
			break;
		}
	}while(ulSendLen < MAX_CMD_BUF);

    /* 发送数据 */
    ulRet = SendPackage(hHandle, acBuf, ulSendLen, eType);
	if (SAR_OK != ulRet)
	{
		return ulRet;
	}

    /* 接收数据 */
	ulRecvLen = MAX_CMD_BUF;
    ulRet = RecvPackage(hHandle, acBuf, &ulRecvLen, eType);
	if (SAR_OK != ulRet)
	{
		return ulRet;
	}

	/* 解析收到的数据 */
	if (acBuf[0] != 0xEF || acBuf[1] != 0x01 || acBuf[2] != 0xE1)
	{
		return SAR_FAIL;
	}
	ulIndex = 3;
	//
	ucSW = acBuf[ulIndex];
	ulIndex += 1;
	//
	usRespApduLen = (acBuf[ulIndex + 1] << 8) | acBuf[ulIndex];
	ulIndex += 2;
	//
	if (*pulOutLen < usRespApduLen)
	{
        ICA_LogD("pulOutLen=%d, usRespApduLen=%d", *pulOutLen, usRespApduLen);
        LOG_ErrNo(SAR_BUFFER_TOO_SMALL);
        return SAR_BUFFER_TOO_SMALL;
	}
	memcpy(pOutBuf, acBuf + ulIndex, usRespApduLen);
	*pulOutLen = usRespApduLen;
	ulIndex += usRespApduLen;

    ICA_LogD("APDUInterface ucSW = <%x>", ucSW);

	return (ucSW & 0xffffffff);
}

#if defined(SYNO_ASIDE)
ULONG DataInterface(HANDLE hHandle,
    UCHAR *pInBuf, ULONG ulInLen, UCHAR *pOutBuf, ULONG *pulOutLen, CMDTYPE_E eCmdType)
{
    return SAR_NOTSUPPORT_SYS;
}
#else
ULONG DataInterface(HANDLE hHandle,
    UCHAR *pInBuf, ULONG ulInLen, UCHAR *pOutBuf, ULONG *pulOutLen, CMDTYPE_E eCmdType)
{
    ULONG ulRet = SAR_OK;
    IODRIVER_S *pstIODriver = NULL;

    /* 参数检查 */
    if (NULL == hHandle)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
        return SAR_INVALIDPARAMERR;
    }

    pstIODriver = (IODRIVER_S *)hHandle;
    if (NULL == pstIODriver->pucDataBuf)
    {
        ulRet = CreateIOBuf(&pstIODriver->pucDataBuf, MAX_DATA_BUF);
        if (SAR_OK != ulRet)
        {
            return ulRet;
        }
    }

    /* 发送数据 */
    ulRet = SendPackage(hHandle, pInBuf, ulInLen, eCmdType);
	if (SAR_OK != ulRet)
	{
		return ulRet;
	}

    /* 接收数据 */
    ulRet = RecvPackage(hHandle, pOutBuf, pulOutLen, eCmdType);
	if (SAR_OK != ulRet)
	{
		return ulRet;
	}

	/* 输出数据长度一定等于输入数据长度 */
	if (*pulOutLen != ulInLen)
	{
		*pulOutLen = ulInLen;
	}

    return ulRet;
}
#endif

ULONG CMD_Transmit(HANDLE hHandle, UCHAR INS, UCHAR P1, UCHAR P2,
    UCHAR *pucIn, ULONG ulInLen, UCHAR *pucOut, ULONG *pulOutLen)
{
    ULONG ulRet = SAR_OK;
	USHORT usLc = 0;
	USHORT usLe = 0;
	USHORT usRespSW = 0;
	APDUHEADER_S stCmdHeader = {0};
	ULONG ulApduLen = 0;
	ULONG ulRespLen = MAX_LC_LEN;
	UCHAR acApduBuf[MAX_LC_LEN] = {0};

    /* 参数检查 */
    if (MAX_APDU_LC_LEN < ulInLen)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	/* 拷贝命令头 */
	stCmdHeader.CLA = 0x80;
	stCmdHeader.INS = INS;
	stCmdHeader.P1 = P1;
	stCmdHeader.P2 = P2;
	memcpy(acApduBuf, &stCmdHeader, sizeof(stCmdHeader));
	ulApduLen = sizeof(stCmdHeader);

	/* 拷贝数据长度Lc和数据，如果有 */
	usLc = (USHORT)ulInLen;
	if (usLc > 0)
	{
		acApduBuf[ulApduLen] = 0x00;
		acApduBuf[ulApduLen + 1] = (usLc >> 8) & 0x00ff;
		acApduBuf[ulApduLen + 2] = usLc & 0x00ff;
		ulApduLen += 3;
		memcpy(acApduBuf + ulApduLen, pucIn, usLc);
		ulApduLen += usLc;
	}

	/* 拷贝Le，如果有 */
	usLe = pulOutLen ? (USHORT)*pulOutLen : 0;
	if (usLe > 0)
	{
		if (usLc == 0)
		{
			acApduBuf[ulApduLen] = 0x00;
			ulApduLen += 1;
		}
		acApduBuf[ulApduLen] = (usLe >> 8) & 0x00ff;
		acApduBuf[ulApduLen + 1] = usLe & 0x00ff;
		ulApduLen += 2;
	}

	ulRet = APDUInterface(hHandle, acApduBuf, ulApduLen, acApduBuf, &ulRespLen);

	/* 检查APDU响应包中的SW */
	//memcpy(&usRespSW, acApduBuf + (ulRespLen - 2), 2);
	usRespSW = (acApduBuf[ulRespLen - 2] << 8) | acApduBuf[(ulRespLen - 2) + 1];
	if (usRespSW != 0x9000)
	{
		ulRet = usRespSW;
		return ulRet;
	}

	/* 返回APDU响应数据 */
	if (NULL != pulOutLen)
	{
		if (ulRespLen > 2)
		{
			if (*pulOutLen < (ulRespLen-2))
			{
				return SAR_BUFFER_TOO_SMALL;
			}
			memcpy(pucOut, acApduBuf, ulRespLen-2);
			*pulOutLen = ulRespLen-2;
		}
		else
		{
			*pulOutLen = 0;
		}
	}

	return SAR_OK;
}

ULONG CMD_TransmitEx(HANDLE hHandle, UCHAR INS, UCHAR P1, UCHAR P2,
    UCHAR *pucIn, ULONG ulInLen, UCHAR *pucOut, ULONG *pulOutLen)
{
    ULONG ulRet = SAR_OK;
	USHORT usLc = 0;
	USHORT usLe = 0;
	USHORT usRespSW = 0;
	APDUHEADER_S stCmdHeader = {0};
	ULONG ulApduLen = 0;
	ULONG ulRespLen = MAX_LC_LEN;
	UCHAR acApduBuf[MAX_LC_LEN] = {0};

    /* 参数检查 */
    if (MAX_APDU_LC_LEN < ulInLen)
    {
        LOG_ErrNo(SAR_INVALIDPARAMERR);
		return SAR_INVALIDPARAMERR;
    }

	/* 拷贝命令头 */
	stCmdHeader.CLA = 0x84;
	stCmdHeader.INS = INS;
	stCmdHeader.P1 = P1;
	stCmdHeader.P2 = P2;
	memcpy(acApduBuf, &stCmdHeader, sizeof(stCmdHeader));
	ulApduLen = sizeof(stCmdHeader);

	/* 拷贝数据长度Lc和数据，如果有 */
	usLc = (USHORT)ulInLen;
	if (usLc > 0)
	{
		acApduBuf[ulApduLen] = 0x00;
		acApduBuf[ulApduLen + 1] = usLc & 0x00ff;
		acApduBuf[ulApduLen + 2] = (usLc >> 8) & 0x00ff;
		ulApduLen += 3;
		memcpy(acApduBuf + ulApduLen, pucIn, usLc);
		ulApduLen += usLc;
	}

	/* 拷贝Le，如果有 */
	usLe = pulOutLen ? (USHORT)*pulOutLen : 0;
	if (usLe > 0)
	{
		if (usLc == 0)
		{
			acApduBuf[ulApduLen] = 0x00;
			ulApduLen += 1;
		}
		acApduBuf[ulApduLen] = usLe & 0x00ff;
		acApduBuf[ulApduLen + 1] = (usLe >> 8) & 0x00ff;
		ulApduLen += 2;
	}

	ulRet = APDUInterface(hHandle, acApduBuf, ulApduLen, acApduBuf, &ulRespLen);

	/* 检查APDU响应包中的SW */
	//memcpy(&usRespSW, acApduBuf + (ulRespLen-2), 2);
	usRespSW = (acApduBuf[ulRespLen - 2] << 8) | acApduBuf[(ulRespLen - 2) + 1];
	if (usRespSW != 0x9000)
	{
		ulRet = usRespSW;
		return ulRet;
	}

	/* 返回APDU响应数据 */
	if (ulRespLen > 2)
	{
		if (*pulOutLen < (ulRespLen-2))
		{
			return SAR_BUFFER_TOO_SMALL;
		}
		memcpy(pucOut, acApduBuf, ulRespLen-2);
		*pulOutLen = ulRespLen-2;
	}

	return SAR_OK;
}


