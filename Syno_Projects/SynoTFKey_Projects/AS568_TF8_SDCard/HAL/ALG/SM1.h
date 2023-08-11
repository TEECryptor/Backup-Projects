#ifndef _SM1_H_
#define _SM1_H_


#define SM1_ENCRYPT 1
#define SM1_DECRYPT 2 

#define SM1_CBC 1
#define SM1_ECB 2
 
 
/*******************************************************************************
函数原型：	UINT8 SM1_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen )
函数说明：	SM1 ECB模式加密或解密。
参数说明：  nType [I] --- 加密或解密，定义如下：
#define 	SM1_ENCRYPT 		1
#define 	SM1_DECRYPT 		2
				pIN [I] ---  要加密或解密的数据。
				pOut [O] ---  输出结果。
nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding。
返 回 值：	RT_OK或者RT_FAIL   
********************************************************************************/
UINT8 SM1_Run_ECB(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen );

/*******************************************************************************
函数原型：	UINT8 SM1_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,
									UINT16 nDataLen,UINT8* pIV)
函数说明：	SM1 CBC模式加密或解密。
参数说明：	nType [I] - 加密或解密，定义如下：
						#define 	SM1_ENCRYPT 		1
						#define 	SM1_DECRYPT 		2
			pIN[I] --- 要加密或解密的数据。
						pOut [O] --- 输出结果。
						nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding.	
			PIV[I][O] --- 输入的IV，并返回的IV值，用于CBC模式的下次运算，长度为16字节
注意事项：	因为传入的IV缓冲，同时要接收返回的IV，因此要备份原始的IV。重新运行时需要原始的IV。
返 回 值：	RT_OK--成功
            RT_FAIL--失败

********************************************************************************/
UINT8 SM1_Run_CBC(UINT8 nType,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
函数原型：	UINT8 SM1_Init(UINT8* pEK, UINT8* pAK)
函数说明：	初始化SM1密钥。
参数说明：	pEK, pAK [I] --- 两个子密钥，每个长度为16字节；
返回值：	RT_OK或RT_FAIL 
********************************************************************************/
UINT8 SM1_Init(UINT8* pEK,UINT8* pAK);

/*******************************************************************************
函数原型：	UINT8 SM1_Run(UINT8 nType, UINT8 nMode, UINT8* pIn, UINT8* pOut, 
								UINT16 nDataLen,UINT8* pIV)
函数说明：	SM1加密或解密。
参数说明：	nType [I] --- 加密或解密，定义如下：
							#define 	SM1_ENCRYPT 		1
							#define 	SM1_DECRYPT 		2
			nMode [I] --- ECB或CBC，定义如下：
							#define SM1_CBC 1
							#define SM1_ECB 2	
			pIN [I] --- 要加密或解密的数据；
			pOut [O] --- 输出结果；
			nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding；
			PIV[IO] --- 输入的IV，并返回IV值，用于CBC模式的下次运算，长度为16字节。当为ECB模式时，该值为NULL
注意事项：因为传入的IV缓冲，同时要接收返回的IV，因此要备份原始的IV。重新运行时需要原始的IV。
返回值：	RT_OK或RT_FAIL	
********************************************************************************/
UINT8 SM1_Run(UINT8 nType,UINT8 nMode,UINT8* pIn,UINT8* pOut,UINT16 nDataLen,UINT8* pIV);

/*******************************************************************************
函数原型：	UINT8 _SM_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen)
函数说明：	SM加密或解密。
参数说明：	nType [I] - 加密或解密，定义如下：
						#define 	SM1_ENCRYPT 		1
						#define 	SM1_DECRYPT 		2
			nAddr [I][O] --- 要加密或解密的数据起始地址（可用空间为0x20004000~0x20008000）；
							 处理结果的数据也存放在以该地址为起始的空间中；
			nDataLen [I] --- 数据长度，必须为16的倍数，不够请先Padding；
注意事项：  该函数直接调用算法加速器，因此需要用户将处理数据放到运算空间地址中。
			该函数只做ECB模式运算。
返回值：	RT_OK或RT_FAIL
********************************************************************************/
UINT8 _SM_Run(UINT8 nType,UINT32 nAddr,UINT16 nDataLen);
 
#endif
