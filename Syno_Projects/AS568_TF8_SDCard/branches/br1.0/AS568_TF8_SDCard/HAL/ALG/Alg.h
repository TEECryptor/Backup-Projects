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
����ԭ�ͣ�	UINT8 AlgLib_GetVersion(INT8* pVer,UINT32 nLen)
����˵����	��ȡ�㷨��汾
����˵����  pVer[O]--����汾��Ϣ�������СΪ16�ֽ�
			nLen [I] --- ���ٵĻ����С������Ϊ16�ֽ� 
�� �� ֵ��	RT_OK����RT_FAIL
********************************************************************************/
UINT8 AlgLib_GetVersion(INT8* pVer,UINT32 nLen);


#endif
