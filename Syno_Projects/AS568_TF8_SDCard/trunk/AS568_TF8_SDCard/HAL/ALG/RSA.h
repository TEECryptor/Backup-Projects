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
	UINT8* modulus;				// modules Ïàµ±ÓÚn
	UINT8* exponent;            // public exponent Ïàµ±ÓÚe          
} R_RSA_PUBLIC_KEY;

/* RSA private key. */	
typedef struct {
	UINT16 bits;               // length in bits of modulus  	
	UINT8* modulus;            // modulus   Ïàµ±ÓÚn                	
	UINT8* publicExponent;     // public exponent  Ïàµ±ÓÚe           	
	UINT8* exponent;           // private exponent Ïàµ±ÓÚd,Èç¹ûÓÃCRTÄ£Ê½¼ÆËã£¬´Ë²ÎÊý¿ÉÒÔ´«NULL,ÒÔ½ÚÊ¡¶ÑÕ»¿Õ¼ä.        
	UINT8* prime[2];           // prime factors   Ïàµ±ÓÚp,q          
	UINT8* primeExponent[2];   // exponents for CRT Ïàµ±ÓÚdP,dQ        
	UINT8* coefficient;        // CRT coefficient  Ïàµ±ÓÚqInv        
} R_RSA_PRIVATE_KEY,R_RSA_KEYPAIR;

#define  RSAPublicEncrypt  RSA_PublicEncrypt
#define  RSAPublicDecrypt  RSA_PublicDecrypt
#define  RSAPrivateEncrypt RSA_PrivateEncrypt
#define  RSAPrivateDecrypt RSA_PrivateDecrypt  

/*******************************************************************************
º¯ÊýÔ­ÐÍ£º	UINT8 RSA_GenKeyPair(R_RSA_PROTO_KEY protoKey, 
								R_RSA_PRIVATE_KEY *pRSAKey)
º¯ÊýËµÃ÷£º	²úÉúRSAÃÜÔ¿¶Ô¡£
²ÎÊýËµÃ÷£º	protoKey [I] --- ²úÉúÊ±µÄ²ÎÊý£»
							µ±²úÉú1024Î»ÃÜÔ¿¶ÔÊ±£¬protoKey->bits=1024£»
							µ±²úÉú2048Î»ÃÜÔ¿¶ÔÊ±£¬protoKey->bits=2048
			protoKey ->PublicExponentÒ»°ãÎª3»ò65537£¬´ËÖµ»á´æ·Åµ½¶ÔÓ¦µÄRSAÃÜÔ¿¶Ô½á¹¹ÖÐµÄpublicExponentÖÐ£¬pRSAKeyÖÐpublicExponentµÄ´æ·Å¸ñÊ½Îª£¬publicExponent[0]Îª×î¸ß×Ö½Ú¡£ 
			pRSAKey [O] --- ²úÉúµÄÃÜÔ¿¶Ô¡£
×¢ÒâÊÂÏî:   pRSAKey½á¹¹ËùÖ¸ÏòµÄÖ¸ÕëÀàÐÍÊý×é£¬µØÖ·¶¼±ØÐëËÄ×Ö½Ú¶ÔÆë£¬Èç¹ûÊÇÈ«¾Ö±äÁ¿£¬½¨ÒéÔÚÊý×éÇ°¼Ó¸ö¶ÔÆë±ê¼Ç__align (4),±ÈÈç:
			__align UINT8 e[4];
			pRSAKey->publicExponent=e;               
			µ±Ç©ÃûÓÃCRT·½Ê½Ê±£¬pRSAKey-> exponent¿ÉÒÔÉèÖÃÎª¿ÕNULL
·µ»ØÖµ£º   RT_OK»òRT_FAIL	   
********************************************************************************/
UINT8 RSA_GenKeyPair(R_RSA_PROTO_KEY protoKey,R_RSA_PRIVATE_KEY *pRSAKey);

/*******************************************************************************
º¯ÊýÔ­ÐÍ£º	UINT8 RSA_PubKeyOpt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
								UINT32 inputLen, R_RSA_PUBLIC_KEY *RSAKey)
º¯ÊýËµÃ÷£º	¹«Ô¿²Ù×÷,´Ëº¯ÊýÃ»ÓÐ¶ÔÊý¾Ý½øÐÐPadding£¬ÐèÒª×Ô¼ºPadding¡£´Ë²Ù×÷¿ÉÒÔ¶ÔÊý¾Ý½øÐÐ¹«Ô¿¼ÓÃÜ»òÕß¶ÔË½Ô¿¼ÓÃÜµÄÊý¾Ý½øÐÐ½âÃÜ¡£º¯ÊýÓÃµ½ÁËR_RSA_PUBLIC_KEYÖÐµÄexponentºÍmodulus»òÕßR_RSA_PRIVATE_KEYÖÐµÄpublicExponentºÍmodulusÁ½¸öÊý×é¡£
²ÎÊýËµÃ÷£º	output [O] --- Êä³ö½á¹û£»
			outputLen [O] --- Êä³ö½á¹û³¤¶È£¬ÎªprotoKey->bits/8×Ö½Ú£»
			input [I] --- ÊäÈëÊý¾Ý£»
			InputLen [I] --- ÊäÈëÊý¾Ý³¤¶È£¬±ØÐëÎªprotoKey->bits/8×Ö½Ú£»
			RSAKey [I] --- ¹«Ô¿£¬Êµ¼ÊÉÏÖ»ÓÃµ½ÁËR_RSA_PRIVATE_KEYÖÐµÄpublicExponentºÍmodulusÁ½¸öÊý×é£¬ÆäËû¿ÉÒÔÎª¿Õ
·µ»ØÖµ£º	RT_OK»òRT_FAIL 
********************************************************************************/
UINT8 RSA_PubKeyOpt(UINT8 *output,	UINT32 *outputLen, UINT8 *input,UINT32 inputLen,R_RSA_PUBLIC_KEY *RSAKey)  ;

/*******************************************************************************
º¯ÊýÔ­ÐÍ£º	UINT8 RSA_PriKeyOpt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
								UINT32 inputLen, R_RSA_PRIVATE_KEY *RSAKey)
º¯ÊýËµÃ÷£º	Ë½Ô¿²Ù×÷£¬´Ëº¯ÊýÃ»ÓÐ¶ÔÊý¾Ý½øÐÐPadding£¬ÐèÒª×Ô¼ºPadding¡£´Ë²Ù×÷¿ÉÒÔ¶ÔÊý¾Ý½øÐÐË½Ô¿¼ÓÃÜ£¬»òÕß¶Ô¹«Ô¿¼ÓÃÜµÄÊý¾Ý½øÐÐ½âÃÜ¡£Ë½Ô¿ÖÐÊ¹ÓÃÁË½á¹¹ÌåR_RSA_PRIVATE_KEYÖÐµÄprime[0]¡¢prime[1]¡¢modulus¡¢primeExponent[0]¡¢primeExponent[1]ºÍcoefficient¡£
²ÎÊýËµÃ÷£º	output [O] --- Êä³ö½á¹û£»
			outputLen [O] --- Êä³ö½á¹û³¤¶È£¬ÎªprotoKey->bits/8×Ö½Ú£»
			input [I] --- ÊäÈëÊý¾Ý£»
			inputLen [I] --- ÊäÈëÊý¾Ý³¤¶È£¬±ØÐëÎªprotoKey->bits/8×Ö½Ú£»
			RSAKey [I] --- Ë½Ô¿£¬Ë½Ô¿ÖÐÊ¹ÓÃÁË½á¹¹ÌåÖÐµÄprime[0]¡¢prime[1]¡¢modulus¡¢primeExponent[0]¡¢primeExponent[1]ºÍcoefficient£
·µ»ØÖµ£º	RT_OK»òRT_FAIL 
********************************************************************************/
UINT8 RSA_PriKeyOpt(UINT8 *output,	UINT32 *outputLen, UINT8 *input,UINT32 inputLen,R_RSA_PRIVATE_KEY *RSAKey)  ;

/*******************************************************************************
º¯ÊýÔ­ÐÍ£º	UINT8 RSA_PriKeyOpt_NoCRT(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
									 UINT32 inputLen, R_RSA_PRIVATE_KEY *RSAKey)
º¯ÊýËµÃ÷£º	Ë½Ô¿²Ù×÷£¬´Ëº¯ÊýÃ»ÓÐ¶ÔÊý¾Ý½øÐÐPadding£¬ÐèÒª×Ô¼ºPadding¡£´Ë²Ù×÷¿ÉÒÔ¶ÔÊý¾Ý½øÐÐË½Ô¿¼ÓÃÜ£¬»òÕß¶Ô¹«Ô¿¼ÓÃÜµÄÊý¾Ý½øÐÐ½âÃÜ¡£Ë½Ô¿ÖÐÊ¹ÓÃÁË½á¹¹ÌåR_RSA_PRIVATE_KEYÖÐµÄmodulusºÍexponent
²ÎÊýËµÃ÷£º	output [O] --- Êä³ö½á¹û£»
			outputLen [O] --- Êä³ö½á¹û³¤¶È£¬ÎªprotoKey->bits/8×Ö½Ú£»
			input [I] --- ÊäÈëÊý¾Ý£»
			inputLen [I] --- ÊäÈëÊý¾Ý³¤¶È£¬±ØÐëÎªprotoKey->bits/8×Ö½Ú£»
			RSAKey [I] --- Ë½Ô¿£¬Ë½Ô¿ÖÐÊ¹ÓÃÁË½á¹¹ÌåÖÐµÄmodulusºÍexponent
·µ»ØÖµ£º	RT_OK»òRT_FAIL
********************************************************************************/
UINT8 RSA_PriKeyOpt_NoCRT(
                    UINT8 *output,        
                    UINT32 *outputLen,     
                    UINT8 *input,        
                    UINT32 inputLen,      
                    R_RSA_PRIVATE_KEY *privateKey);  

/*******************************************************************************
º¯ÊýÔ­ÐÍ£º	UINT8 RSA_PublicEncrypt (UINT8 *output,  UINT32 *outputLen, UINT8 *input, 
									UINT32 inputLen, R_RSA_PUBLIC_KEY * publicKey)
º¯ÊýËµÃ÷£º	¹«Ô¿¼ÓÃÜ£¬´Ëº¯Êý»á×Ô¼º¶ÔÊý¾Ý°´PKCS#1½øÐÐPadding£¬º¯ÊýÓÃµ½ÁËR_RSA_PUBLIC_KEYÖÐµÄexponentºÍmodulus»òÕßR_RSA_PRIVATE_KEYÖÐµÄpublicExponentºÍmodulusÁ½¸öÊý×é¡£
²ÎÊýËµÃ÷£º	output [O] --- Êä³ö½á¹û£»
			outputLen [O] --- Êä³ö½á¹û³¤¶È£¬ÎªprotoKey->bits/8×Ö½Ú£»
			input[I] --- ÊäÈëÊý¾Ý£»
			inputLen [I] --- ÊäÈëÊý¾Ý³¤¶È£¬¸ù¾ÝPKCS#1±ØÐëÐ¡ÓÚprotoKey->bits/8 - 11×Ö½Ú£»
			publicKey [I] --- ¹«Ô¿£ 
·µ»ØÖµ£º	RT_OK»òRT_FAIL
********************************************************************************/                                       
UINT8 RSA_PublicEncrypt(
						UINT8 *output,         
						UINT32 *outputLen,      
						UINT8 *input,          
						UINT32 inputLen,       
						R_RSA_PUBLIC_KEY *publicKey);  

/*******************************************************************************
º¯ÊýÔ­ÐÍ£º	UINT8 RSA_PublicDecrypt (UINT8 *output, UINT32 *outputLen, UINT8 *input, UINT32 inputLen, R_RSA_PUBLIC_KEY * publicKey);
º¯ÊýËµÃ÷£º	¹«Ô¿½âÃÜ£¬´Ëº¯Êý»á¶Ô½âÃÜµÄÊý¾Ý°´PKCS#1·½Ê½½øÐÐ½âÎö£¬·µ»ØµÄÊÇ½âÎöºÃµÄÊý¾Ý¡£º¯ÊýÓÃµ½ÁËR_RSA_PUBLIC_KEYÖÐµÄexponentºÍmodulus»òÕßR_RSA_PRIVATE_KEYÖÐµÄpublicExponentºÍmodulusÁ½¸öÊý×é¡£
²ÎÊýËµÃ÷£º	output [O] --- Êä³ö½á¹û£»
			outputLen [O] --- Êä³ö½á¹û³¤¶È£¬Ð¡ÓÚprotoKey->bits/8 - 11×Ö½Ú£»
			input [I] --- ÊäÈëÊý¾Ý£»
			inputLen [I] --- ÊäÈëÊý¾Ý³¤¶È£¬±ØÐëÎªprotoKey->bits/8×Ö½Ú£»
			publicKey [I] --- ¹«Ô¿
·µ»ØÖµ£º	RT_OK»òRT_FAIL 
********************************************************************************/
UINT8 RSA_PublicDecrypt(
						UINT8 *output,        
						UINT32 *outputLen,       
						UINT8 *input,          
						UINT32 inputLen,          
						R_RSA_PUBLIC_KEY *publicKey);   
/*******************************************************************************
º¯ÊýÔ­ÐÍ£º	UINT8 RSA_PrivateEncrypt(UINT8 *output, UINT32 *outputLen, UINT8 *input,
									UINT32 inputLen, R_RSA_PRIVATE_KEY *privateKey)
º¯ÊýËµÃ÷£º	Ë½Ô¿¼ÓÃÜ£¬´Ëº¯Êý»á×Ô¼º¶ÔÊý¾Ý°´PKCS#1½øÐÐPadding£¬Ë½Ô¿ÖÐÊ¹ÓÃÁË½á¹¹ÌåR_RSA_PRIVATE_KEYÖÐµÄprime[0]¡¢prime[1]¡¢modulus¡¢primeExponent[0]¡¢primeExponent[1]ºÍcoefficient¡£
²ÎÊýËµÃ÷£º	output [O] --- Êä³ö½á¹û£»
			outputLen [O] --- Êä³ö½á¹û³¤¶È£¬ÎªprotoKey->bits/8×Ö½Ú£»
			input [I] --- ÊäÈëÊý¾Ý£»
			inputLen [I] --- ÊäÈëÊý¾Ý³¤¶È£¬¸ù¾ÝPKCS#1±ØÐëÐ¡ÓÚprotoKey->bits/8 - 11×Ö½Ú£»
			privateKey [I] --- Ë½Ô¿£¬Ë½Ô¿ÖÐÊ¹ÓÃÁË½á¹¹ÌåÖÐµÄprime[0]¡¢prime[1]¡¢modulus¡¢primeExponent[0]¡¢primeExponent[1]ºÍcoefficient¡£
·µ»ØÖµ£º	RT_OK»òRT_FAIL	
********************************************************************************/
UINT8 RSA_PrivateEncrypt(
						UINT8 *output,        
						UINT32 *outputLen,     
						UINT8 *input,          
						UINT32 inputLen,       
						R_RSA_PRIVATE_KEY *privateKey);  

/*******************************************************************************
º¯ÊýÔ­ÐÍ£º	UINT8 RSA_PrivateDecrypt(UINT8 *output, UINT32 *outputLen, UINT8 *input, 
									UINT32 inputLen, R_RSA_PRIVATE_KEY *privateKey)
º¯ÊýËµÃ÷£º	Ë½Ô¿½âÃÜ£¬´Ëº¯Êý»á¶Ô½âÃÜµÄÊý¾Ý°´PKCS#1·½Ê½½øÐÐ½âÎö£¬·µ»ØµÄÊÇ½âÎöºÃµÄÊý¾Ý¡£Ë½Ô¿ÖÐÊ¹ÓÃÁË½á¹¹ÌåR_RSA_PRIVATE_KEYÖÐµÄprime[0]¡¢prime[1]¡¢modulus¡¢primeExponent[0]¡¢primeExponent[1]ºÍcoefficient¡£
²ÎÊýËµÃ÷£º	output [O] --- Êä³ö½á¹û£»
			outputLen [O] --- Êä³ö½á¹û³¤¶È£¬Ð¡ÓÚprotoKey->bits/8 - 11×Ö½Ú£»
			input [I] --- ÊäÈëÊý¾Ý£»
			inputLen [I] --- ÊäÈëÊý¾Ý³¤¶È£¬±ØÐëÎªprotoKey->bits/8×Ö½Ú
			privateKey [I] --- Ë½Ô¿£¬Ë½Ô¿ÖÐÊ¹ÓÃÁË½á¹¹ÌåÖÐµÄprime[0]¡¢prime[1]¡¢modulus¡¢primeExponent[0]¡¢primeExponent[1]ºÍcoefficient¡£
·µ»ØÖµ£º	RT_OK»òRT_FAIL	
********************************************************************************/
UINT8 RSA_PrivateDecrypt(
						UINT8 *output,        
						UINT32 *outputLen,      
						UINT8 *input,          
						UINT32 inputLen,         
						R_RSA_PRIVATE_KEY *privateKey);  


#endif
