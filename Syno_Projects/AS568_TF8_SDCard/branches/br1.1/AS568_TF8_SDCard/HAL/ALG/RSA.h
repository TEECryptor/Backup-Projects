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
	UINT8* modulus;				// modules �൱��n
	UINT8* exponent;            // public exponent �൱��e          
} R_RSA_PUBLIC_KEY;

/* RSA private key. */	
typedef struct {
	UINT16 bits;               // length in bits of modulus  	
	UINT8* modulus;            // modulus   �൱��n                	
	UINT8* publicExponent;     // public exponent  �൱��e           	
	UINT8* exponent;           // private exponent �൱��d,�����CRTģʽ���㣬�˲������Դ�NULL,�Խ�ʡ��ջ�ռ�.        
	UINT8* prime[2];           // prime factors   �൱��p,q          
	UINT8* primeExponent[2];   // exponents for CRT �൱��dP,dQ        
	UINT8* coefficient;        // CRT coefficient  �൱��qInv        
} R_RSA_PRIVATE_KEY,R_RSA_KEYPAIR;

#define  RSAPublicEncrypt  RSA_PublicEncrypt
#define  RSAPublicDecrypt  RSA_PublicDecrypt
#define  RSAPrivateEncrypt RSA_PrivateEncrypt
#define  RSAPrivateDecrypt RSA_PrivateDecrypt  

/*******************************************************************************
����ԭ�ͣ�	UINT8 RSA_GenKeyPair(R_RSA_PROTO_KEY protoKey, 
								R_RSA_PRIVATE_KEY *pRSAKey)
����˵����	����RSA��Կ�ԡ�
����˵����	protoKey [I] --- ����ʱ�Ĳ�����
							������1024λ��Կ��ʱ��protoKey->bits=1024��
							������2048λ��Կ��ʱ��protoKey->bits=2048
			protoKey ->PublicExponentһ��Ϊ3��65537����ֵ���ŵ���Ӧ��RSA��Կ�Խṹ�е�publicExponent�У�pRSAKey��publicExponent�Ĵ�Ÿ�ʽΪ��publicExponent[0]Ϊ����ֽڡ� 
			pRSAKey [O] --- ��������Կ�ԡ�
ע������:   pRSAKey�ṹ��ָ���ָ���������飬��ַ���������ֽڶ��룬�����ȫ�ֱ���������������ǰ�Ӹ�������__align (4),����:
			__align UINT8 e[4];
			pRSAKey->publicExponent=e;               
			��ǩ����CRT��ʽʱ��pRSAKey-> exponent��������Ϊ��NULL
����ֵ��   RT_OK��RT_FAIL	   
********************************************************************************/
UINT8 RSA_GenKeyPair(R_RSA_PROTO_KEY protoKey,R_RSA_PRIVATE_KEY *pRSAKey);

/*******************************************************************************
����ԭ�ͣ�	UINT8 RSA_PubKeyOpt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
								UINT32 inputLen, R_RSA_PUBLIC_KEY *RSAKey)
����˵����	��Կ����,�˺���û�ж����ݽ���Padding����Ҫ�Լ�Padding���˲������Զ����ݽ��й�Կ���ܻ��߶�˽Կ���ܵ����ݽ��н��ܡ������õ���R_RSA_PUBLIC_KEY�е�exponent��modulus����R_RSA_PRIVATE_KEY�е�publicExponent��modulus�������顣
����˵����	output [O] --- ��������
			outputLen [O] --- ���������ȣ�ΪprotoKey->bits/8�ֽڣ�
			input [I] --- �������ݣ�
			InputLen [I] --- �������ݳ��ȣ�����ΪprotoKey->bits/8�ֽڣ�
			RSAKey [I] --- ��Կ��ʵ����ֻ�õ���R_RSA_PRIVATE_KEY�е�publicExponent��modulus�������飬��������Ϊ��
����ֵ��	RT_OK��RT_FAIL 
********************************************************************************/
UINT8 RSA_PubKeyOpt(UINT8 *output,	UINT32 *outputLen, UINT8 *input,UINT32 inputLen,R_RSA_PUBLIC_KEY *RSAKey)  ;

/*******************************************************************************
����ԭ�ͣ�	UINT8 RSA_PriKeyOpt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
								UINT32 inputLen, R_RSA_PRIVATE_KEY *RSAKey)
����˵����	˽Կ�������˺���û�ж����ݽ���Padding����Ҫ�Լ�Padding���˲������Զ����ݽ���˽Կ���ܣ����߶Թ�Կ���ܵ����ݽ��н��ܡ�˽Կ��ʹ���˽ṹ��R_RSA_PRIVATE_KEY�е�prime[0]��prime[1]��modulus��primeExponent[0]��primeExponent[1]��coefficient��
����˵����	output [O] --- ��������
			outputLen [O] --- ���������ȣ�ΪprotoKey->bits/8�ֽڣ�
			input [I] --- �������ݣ�
			inputLen [I] --- �������ݳ��ȣ�����ΪprotoKey->bits/8�ֽڣ�
			RSAKey [I] --- ˽Կ��˽Կ��ʹ���˽ṹ���е�prime[0]��prime[1]��modulus��primeExponent[0]��primeExponent[1]��coefficient�
����ֵ��	RT_OK��RT_FAIL 
********************************************************************************/
UINT8 RSA_PriKeyOpt(UINT8 *output,	UINT32 *outputLen, UINT8 *input,UINT32 inputLen,R_RSA_PRIVATE_KEY *RSAKey)  ;

/*******************************************************************************
����ԭ�ͣ�	UINT8 RSA_PriKeyOpt_NoCRT(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
									 UINT32 inputLen, R_RSA_PRIVATE_KEY *RSAKey)
����˵����	˽Կ�������˺���û�ж����ݽ���Padding����Ҫ�Լ�Padding���˲������Զ����ݽ���˽Կ���ܣ����߶Թ�Կ���ܵ����ݽ��н��ܡ�˽Կ��ʹ���˽ṹ��R_RSA_PRIVATE_KEY�е�modulus��exponent
����˵����	output [O] --- ��������
			outputLen [O] --- ���������ȣ�ΪprotoKey->bits/8�ֽڣ�
			input [I] --- �������ݣ�
			inputLen [I] --- �������ݳ��ȣ�����ΪprotoKey->bits/8�ֽڣ�
			RSAKey [I] --- ˽Կ��˽Կ��ʹ���˽ṹ���е�modulus��exponent
����ֵ��	RT_OK��RT_FAIL
********************************************************************************/
UINT8 RSA_PriKeyOpt_NoCRT(
                    UINT8 *output,        
                    UINT32 *outputLen,     
                    UINT8 *input,        
                    UINT32 inputLen,      
                    R_RSA_PRIVATE_KEY *privateKey);  

/*******************************************************************************
����ԭ�ͣ�	UINT8 RSA_PublicEncrypt (UINT8 *output,  UINT32 *outputLen, UINT8 *input, 
									UINT32 inputLen, R_RSA_PUBLIC_KEY * publicKey)
����˵����	��Կ���ܣ��˺������Լ������ݰ�PKCS#1����Padding�������õ���R_RSA_PUBLIC_KEY�е�exponent��modulus����R_RSA_PRIVATE_KEY�е�publicExponent��modulus�������顣
����˵����	output [O] --- ��������
			outputLen [O] --- ���������ȣ�ΪprotoKey->bits/8�ֽڣ�
			input[I] --- �������ݣ�
			inputLen [I] --- �������ݳ��ȣ�����PKCS#1����С��protoKey->bits/8 - 11�ֽڣ�
			publicKey [I] --- ��Կ� 
����ֵ��	RT_OK��RT_FAIL
********************************************************************************/                                       
UINT8 RSA_PublicEncrypt(
						UINT8 *output,         
						UINT32 *outputLen,      
						UINT8 *input,          
						UINT32 inputLen,       
						R_RSA_PUBLIC_KEY *publicKey);  

/*******************************************************************************
����ԭ�ͣ�	UINT8 RSA_PublicDecrypt (UINT8 *output, UINT32 *outputLen, UINT8 *input, UINT32 inputLen, R_RSA_PUBLIC_KEY * publicKey);
����˵����	��Կ���ܣ��˺�����Խ��ܵ����ݰ�PKCS#1��ʽ���н��������ص��ǽ����õ����ݡ������õ���R_RSA_PUBLIC_KEY�е�exponent��modulus����R_RSA_PRIVATE_KEY�е�publicExponent��modulus�������顣
����˵����	output [O] --- ��������
			outputLen [O] --- ���������ȣ�С��protoKey->bits/8 - 11�ֽڣ�
			input [I] --- �������ݣ�
			inputLen [I] --- �������ݳ��ȣ�����ΪprotoKey->bits/8�ֽڣ�
			publicKey [I] --- ��Կ
����ֵ��	RT_OK��RT_FAIL 
********************************************************************************/
UINT8 RSA_PublicDecrypt(
						UINT8 *output,        
						UINT32 *outputLen,       
						UINT8 *input,          
						UINT32 inputLen,          
						R_RSA_PUBLIC_KEY *publicKey);   
/*******************************************************************************
����ԭ�ͣ�	UINT8 RSA_PrivateEncrypt(UINT8 *output, UINT32 *outputLen, UINT8 *input,
									UINT32 inputLen, R_RSA_PRIVATE_KEY *privateKey)
����˵����	˽Կ���ܣ��˺������Լ������ݰ�PKCS#1����Padding��˽Կ��ʹ���˽ṹ��R_RSA_PRIVATE_KEY�е�prime[0]��prime[1]��modulus��primeExponent[0]��primeExponent[1]��coefficient��
����˵����	output [O] --- ��������
			outputLen [O] --- ���������ȣ�ΪprotoKey->bits/8�ֽڣ�
			input [I] --- �������ݣ�
			inputLen [I] --- �������ݳ��ȣ�����PKCS#1����С��protoKey->bits/8 - 11�ֽڣ�
			privateKey [I] --- ˽Կ��˽Կ��ʹ���˽ṹ���е�prime[0]��prime[1]��modulus��primeExponent[0]��primeExponent[1]��coefficient��
����ֵ��	RT_OK��RT_FAIL	
********************************************************************************/
UINT8 RSA_PrivateEncrypt(
						UINT8 *output,        
						UINT32 *outputLen,     
						UINT8 *input,          
						UINT32 inputLen,       
						R_RSA_PRIVATE_KEY *privateKey);  

/*******************************************************************************
����ԭ�ͣ�	UINT8 RSA_PrivateDecrypt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
									UINT32 inputLen, R_RSA_PRIVATE_KEY *privateKey)
����˵����	˽Կ���ܣ��˺�����Խ��ܵ����ݰ�PKCS#1��ʽ���н��������ص��ǽ����õ����ݡ�˽Կ��ʹ���˽ṹ��R_RSA_PRIVATE_KEY�е�prime[0]��prime[1]��modulus��primeExponent[0]��primeExponent[1]��coefficient��
����˵����	output [O] --- ��������
			outputLen [O] --- ���������ȣ�С��protoKey->bits/8 - 11�ֽڣ�
			input [I] --- �������ݣ�
			inputLen [I] --- �������ݳ��ȣ�����ΪprotoKey->bits/8�ֽ�
			privateKey [I] --- ˽Կ��˽Կ��ʹ���˽ṹ���е�prime[0]��prime[1]��modulus��primeExponent[0]��primeExponent[1]��coefficient��
����ֵ��	RT_OK��RT_FAIL	
********************************************************************************/
UINT8 RSA_PrivateDecrypt(
						UINT8 *output,        
						UINT32 *outputLen,      
						UINT8 *input,          
						UINT32 inputLen,         
						R_RSA_PRIVATE_KEY *privateKey);  


#endif
