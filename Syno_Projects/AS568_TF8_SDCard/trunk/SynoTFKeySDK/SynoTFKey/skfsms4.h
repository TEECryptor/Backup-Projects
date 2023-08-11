/***************************************************************************
 *	File name:skfsms4.h
 *	Introduce:Head file for sms4 definition
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _SKFSMS4_H_
#define _SKFSMS4_H_

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "SynoHeaders.h"

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define SMS4_ENCRYPT 1
#define SMS4_DECRYPT 2

#define SMS4_CBC 1
#define SMS4_ECB 2
#define SMS4_CFB 3
#define SMS4_OFB 4

#define RT_PARAM_ERR  -1
#define RT_OK 0

#ifdef __cplusplus
extern "C" {
#endif

UINT8 SMS4_Init(UINT8* pEK);
UINT8 SMS4_Run(UINT8 nType, UINT8 nMode, UINT8 *pIn, UINT8 *pucOut, UINT16 nDataLen, UINT8* pIV);

UINT8 SMS4_Run_ECB(UINT8 nType, UINT8* pIn,UINT8 *pucOut, UINT16 nDataLen);
UINT8 SMS4_Run_CBC(UINT8 nType, UINT8* pIn,UINT8 *pucOut, UINT16 nDataLen, UINT8 *pIV);
UINT8 SMS4_Run_CFB(UINT8 nType, UINT8* pIn,UINT8 *pucOut, UINT16 nDataLen, UINT8 *pIV);
UINT8 SMS4_Run_OFB(UINT8 nType, UINT8* pIn,UINT8 *pucOut, UINT16 nDataLen, UINT8 *pIV);

UINT8 SMS4_ENC(UINT8 *pKey, UINT8 *pIn, UINT8 *pucOut, UINT16 nDatalen);
UINT8 SMS4_DEC(UINT8 *pKey, UINT8 *pIn, UINT8 *pucOut, UINT16 nDatalen);

#ifdef __cplusplus
}
#endif

#endif /* _SKFSMS4_H_ */

