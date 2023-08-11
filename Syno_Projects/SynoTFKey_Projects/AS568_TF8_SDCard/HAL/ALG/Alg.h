#ifndef _ALG_H_
#define _ALG_H_
#include "SHA-1.h"
#include "SHA-256.h"
#include "MD5.h"
#include "SM3.h"

#include "SM1.h"
#include "SSF33.h"
#include "DES.h"
#include "SMS4.h"
#include "AES.h"

#include "RSA.h"
#include "SM2.h"
#include "ECDSA.h"

/*******************************************************************************
函数原型：	UINT8 AlgLib_GetVersion(INT8* pVer,UINT32 nLen)
函数说明：	获取算法库版本
参数说明：  pVer[O]--输出版本信息，数组大小为16字节
			nLen [I] --- 开辟的缓冲大小，必须为16字节 
返 回 值：	RT_OK或者RT_FAIL
********************************************************************************/
UINT8 AlgLib_GetVersion(INT8* pVer,UINT32 nLen);


#endif
