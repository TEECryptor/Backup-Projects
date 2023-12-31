/***************************************************************************************
* File name    :	SyRSA.h
* Function     :	The header of SyRSA.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SYRSA_H_
#define _SYRSA_H_
  

/* Error codes. */	
#define RE_OK                   0x00
#define RE_DATA                 0x01
#define RE_LEN                  0x02
#define RE_DATA_ALIGN           0x03 

/* RSA prototype key. */

typedef struct {
	UINT32 bits;                // length in bits of modulus 
	UINT32 PublicExponent;      // public exponent 
} R_RSA_PROTO_KEY;
	
/* RSA public key. */	
	
typedef struct {
	UINT16 bits;				// length in bits of modulus
	UINT8* modulus;				// modules 相当于n
	UINT8* exponent;            // public exponent 相当于e          
} R_RSA_PUBLIC_KEY;

/* RSA private key. */	
typedef struct {
	UINT16 bits;               // length in bits of modulus  	
	UINT8* modulus;            // modulus   相当于n                	
	UINT8* publicExponent;     // public exponent  相当于e           	
	UINT8* exponent;           // private exponent 相当于d,如果用CRT模式计算，此参数可以传NULL,以节省堆栈空间.        
	UINT8* prime[2];           // prime factors   相当于p,q          
	UINT8* primeExponent[2];   // exponents for CRT 相当于dP,dQ        
	UINT8* coefficient;        // CRT coefficient  相当于qInv        
} R_RSA_PRIVATE_KEY,R_RSA_KEYPAIR;

#define  RSAPublicEncrypt  RSA_PublicEncrypt
#define  RSAPublicDecrypt  RSA_PublicDecrypt
#define  RSAPrivateEncrypt RSA_PrivateEncrypt
#define  RSAPrivateDecrypt RSA_PrivateDecrypt  

/*******************************************************************************
函数原型：	UINT8 RSA_GenKeyPair(R_RSA_PROTO_KEY protoKey, 
								R_RSA_PRIVATE_KEY *pRSAKey)
函数说明：	产生RSA密钥对。
参数说明：	protoKey [I] --- 产生时的参数；
							当产生1024位密钥对时，protoKey->bits=1024；
							当产生2048位密钥对时，protoKey->bits=2048
			protoKey ->PublicExponent一般为3或65537，此值会存放到对应的RSA密钥对结构中的publicExponent中，pRSAKey中publicExponent的存放格式为，publicExponent[0]为最高字节。 
			pRSAKey [O] --- 产生的密钥对。
注意事项:   pRSAKey结构所指向的指针类型数组，地址都必须四字节对齐，如果是全局变量，建议在数组前加个对齐标记__align (4),比如:
			__align UINT8 e[4];
			pRSAKey->publicExponent=e;               
			当签名用CRT方式时，pRSAKey-> exponent可以设置为空NULL
返回值：   RT_OK或RT_FAIL	   
********************************************************************************/
UINT8 RSA_GenKeyPair(R_RSA_PROTO_KEY protoKey,R_RSA_PRIVATE_KEY *pRSAKey);

/*******************************************************************************
函数原型：	UINT8 RSA_PubKeyOpt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
								UINT32 inputLen, R_RSA_PUBLIC_KEY *RSAKey)
函数说明：	公钥操作,此函数没有对数据进行Padding，需要自己Padding。此操作可以对数据进行公钥加密或者对私钥加密的数据进行解密。函数用到了R_RSA_PUBLIC_KEY中的exponent和modulus或者R_RSA_PRIVATE_KEY中的publicExponent和modulus两个数组。
参数说明：	output [O] --- 输出结果；
			outputLen [O] --- 输出结果长度，为protoKey->bits/8字节；
			input [I] --- 输入数据；
			InputLen [I] --- 输入数据长度，必须为protoKey->bits/8字节；
			RSAKey [I] --- 公钥，实际上只用到了R_RSA_PRIVATE_KEY中的publicExponent和modulus两个数组，其他可以为空
返回值：	RT_OK或RT_FAIL 
********************************************************************************/
UINT8 RSA_PubKeyOpt(UINT8 *output,	UINT32 *outputLen, UINT8 *input,UINT32 inputLen,R_RSA_PUBLIC_KEY *RSAKey)  ;

/*******************************************************************************
函数原型：	UINT8 RSA_PriKeyOpt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
								UINT32 inputLen, R_RSA_PRIVATE_KEY *RSAKey)
函数说明：	私钥操作，此函数没有对数据进行Padding，需要自己Padding。此操作可以对数据进行私钥加密，或者对公钥加密的数据进行解密。私钥中使用了结构体R_RSA_PRIVATE_KEY中的prime[0]、prime[1]、modulus、primeExponent[0]、primeExponent[1]和coefficient。
参数说明：	output [O] --- 输出结果；
			outputLen [O] --- 输出结果长度，为protoKey->bits/8字节；
			input [I] --- 输入数据；
			inputLen [I] --- 输入数据长度，必须为protoKey->bits/8字节；
			RSAKey [I] --- 私钥，私钥中使用了结构体中的prime[0]、prime[1]、modulus、primeExponent[0]、primeExponent[1]和coefficient�
返回值：	RT_OK或RT_FAIL 
********************************************************************************/
UINT8 RSA_PriKeyOpt(UINT8 *output,	UINT32 *outputLen, UINT8 *input,UINT32 inputLen,R_RSA_PRIVATE_KEY *RSAKey)  ;

/*******************************************************************************
函数原型：	UINT8 RSA_PriKeyOpt_NoCRT(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
									 UINT32 inputLen, R_RSA_PRIVATE_KEY *RSAKey)
函数说明：	私钥操作，此函数没有对数据进行Padding，需要自己Padding。此操作可以对数据进行私钥加密，或者对公钥加密的数据进行解密。私钥中使用了结构体R_RSA_PRIVATE_KEY中的modulus和exponent
参数说明：	output [O] --- 输出结果；
			outputLen [O] --- 输出结果长度，为protoKey->bits/8字节；
			input [I] --- 输入数据；
			inputLen [I] --- 输入数据长度，必须为protoKey->bits/8字节；
			RSAKey [I] --- 私钥，私钥中使用了结构体中的modulus和exponent
返回值：	RT_OK或RT_FAIL
********************************************************************************/
UINT8 RSA_PriKeyOpt_NoCRT(
                    UINT8 *output,        
                    UINT32 *outputLen,     
                    UINT8 *input,        
                    UINT32 inputLen,      
                    R_RSA_PRIVATE_KEY *privateKey);  

/*******************************************************************************
函数原型：	UINT8 RSA_PublicEncrypt (UINT8 *output,  UINT32 *outputLen, UINT8 *input, 
									UINT32 inputLen, R_RSA_PUBLIC_KEY * publicKey)
函数说明：	公钥加密，此函数会自己对数据按PKCS#1进行Padding，函数用到了R_RSA_PUBLIC_KEY中的exponent和modulus或者R_RSA_PRIVATE_KEY中的publicExponent和modulus两个数组。
参数说明：	output [O] --- 输出结果；
			outputLen [O] --- 输出结果长度，为protoKey->bits/8字节；
			input[I] --- 输入数据；
			inputLen [I] --- 输入数据长度，根据PKCS#1必须小于protoKey->bits/8 - 11字节；
			publicKey [I] --- 公钥� 
返回值：	RT_OK或RT_FAIL
********************************************************************************/                                       
UINT8 RSA_PublicEncrypt(
						UINT8 *output,         
						UINT32 *outputLen,      
						UINT8 *input,          
						UINT32 inputLen,       
						R_RSA_PUBLIC_KEY *publicKey);  

/*******************************************************************************
函数原型：	UINT8 RSA_PublicDecrypt (UINT8 *output, UINT32 *outputLen, UINT8 *input, UINT32 inputLen, R_RSA_PUBLIC_KEY * publicKey);
函数说明：	公钥解密，此函数会对解密的数据按PKCS#1方式进行解析，返回的是解析好的数据。函数用到了R_RSA_PUBLIC_KEY中的exponent和modulus或者R_RSA_PRIVATE_KEY中的publicExponent和modulus两个数组。
参数说明：	output [O] --- 输出结果；
			outputLen [O] --- 输出结果长度，小于protoKey->bits/8 - 11字节；
			input [I] --- 输入数据；
			inputLen [I] --- 输入数据长度，必须为protoKey->bits/8字节；
			publicKey [I] --- 公钥
返回值：	RT_OK或RT_FAIL 
********************************************************************************/
UINT8 RSA_PublicDecrypt(
						UINT8 *output,        
						UINT32 *outputLen,       
						UINT8 *input,          
						UINT32 inputLen,          
						R_RSA_PUBLIC_KEY *publicKey);   
/*******************************************************************************
函数原型：	UINT8 RSA_PrivateEncrypt(UINT8 *output, UINT32 *outputLen, UINT8 *input,
									UINT32 inputLen, R_RSA_PRIVATE_KEY *privateKey)
函数说明：	私钥加密，此函数会自己对数据按PKCS#1进行Padding，私钥中使用了结构体R_RSA_PRIVATE_KEY中的prime[0]、prime[1]、modulus、primeExponent[0]、primeExponent[1]和coefficient。
参数说明：	output [O] --- 输出结果；
			outputLen [O] --- 输出结果长度，为protoKey->bits/8字节；
			input [I] --- 输入数据；
			inputLen [I] --- 输入数据长度，根据PKCS#1必须小于protoKey->bits/8 - 11字节；
			privateKey [I] --- 私钥，私钥中使用了结构体中的prime[0]、prime[1]、modulus、primeExponent[0]、primeExponent[1]和coefficient。
返回值：	RT_OK或RT_FAIL	
********************************************************************************/
UINT8 RSA_PrivateEncrypt(
						UINT8 *output,        
						UINT32 *outputLen,     
						UINT8 *input,          
						UINT32 inputLen,       
						R_RSA_PRIVATE_KEY *privateKey);  

/*******************************************************************************
函数原型：	UINT8 RSA_PrivateDecrypt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
									UINT32 inputLen, R_RSA_PRIVATE_KEY *privateKey)
函数说明：	私钥解密，此函数会对解密的数据按PKCS#1方式进行解析，返回的是解析好的数据。私钥中使用了结构体R_RSA_PRIVATE_KEY中的prime[0]、prime[1]、modulus、primeExponent[0]、primeExponent[1]和coefficient。
参数说明：	output [O] --- 输出结果；
			outputLen [O] --- 输出结果长度，小于protoKey->bits/8 - 11字节；
			input [I] --- 输入数据；
			inputLen [I] --- 输入数据长度，必须为protoKey->bits/8字节
			privateKey [I] --- 私钥，私钥中使用了结构体中的prime[0]、prime[1]、modulus、primeExponent[0]、primeExponent[1]和coefficient。
返回值：	RT_OK或RT_FAIL	
********************************************************************************/
UINT8 RSA_PrivateDecrypt(
						UINT8 *output,        
						UINT32 *outputLen,      
						UINT8 *input,          
						UINT32 inputLen,         
						R_RSA_PRIVATE_KEY *privateKey);  


#endif
