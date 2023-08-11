#ifndef _SMS4_H_
#define _SMS4_H_
 
#define SMS4_ENCRYPT 1
#define SMS4_DECRYPT 2 

#define SMS4_CBC 1
#define SMS4_ECB 2
 
/*******************************************************************************
函数原型：	UINT8 SMS4_Init(UINT8* pEK);
函数说明：	初始化SMS4密钥 。
参数说明：	pEK [I] --- 子密钥，长度为16字节。
返 回 值：	RT_OK--成功      
            RT_FAIL--失败
********************************************************************************/
UINT8 SMS4_Init(UINT8* pEK);

/*******************************************************************************
函数原型：	UINT8 SMS4_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV)
函数说明：	SMS4加密或解密。
参数说明：	nType [I] --- 加密或解密，定义如下：
							#define 	SMS4_ENCRYPT 		1
							#define 	SMS4_DECRYPT 		2
			nMode [I] --- ECB、CBC、OFB或CFB，定义如下：
							#define SMS4_CBC 1
							#define SMS4_ECB 2				 
			pIN[I] --- 要加密或解密的数据。
			pOut [O] --- 输出结果。
			nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding。
			PIV[I][O] --- 输入的IV，并返回的IV值，用于CBC模式的下次运算，长度为16字节。
			           当为ECB模式时，该值为NULL.
注意事项：	因为传入的IV缓冲，同时要接收返回的IV，因此要备份原始的IV。重新运行时需要原始的IV。
返 回 值：	RT_OK--成功 
			RT_FAIL--失败
********************************************************************************/
UINT8 SMS4_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
函数原型：	UINT8 SMS4_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
函数说明：	SMS4 ECB模式加密或解密。
参数说明：	nType [I] --- 加密或解密，定义如下：
			#define 	SMS4_ENCRYPT 		1
			#define 	SMS4_DECRYPT 		2
			pIN[I] --- 要加密或解密的数据。
			pOut [O] --- 输出结果。
			nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding.
返 回 值：	RT_OK--成功 
			RT_FAIL--失败

********************************************************************************/
UINT8 SMS4_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen);

/*******************************************************************************
函数原型：	UINT8 SMS4_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,
                               UINT8* pIV)
函数说明：	SMS4 CBC模式加密或解密。
参数说明：	nType [I] --- 加密或解密，定义如下：
							#define 	SMS4_ENCRYPT 		1
							#define 	SMS4_DECRYPT 		2
			pIN[I] --- 要加密或解密的数据。
			pOut [O] --- 输出结果。
			nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding。
			PIV[I][O] --- 输入的IV，并返回的IV值，用于CBC模式的下次运算，长度为16字节
注意事项：	因为传入的IV缓冲，同时要接收返回的IV，因此要备份原始的IV。重新运行时需要原始的IV。
返 回 值：	RT_OK--成功 
            RT_FAIL--失败 
********************************************************************************/
UINT8 SMS4_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);
 
#endif
