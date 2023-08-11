#ifndef ELLIPTIC_ZP_H_
#define ELLIPTIC_ZP_H_ 1
 
#define MAX_BYTE_LEN 32 //ECC 公私钥数组的最大字节个数，支持的最大ECC算法长度为MAX_BYTE_LEN*8 


typedef struct 
{ 
	UINT32      nBits;	//算法位数，目前只支持256或192		
    UINT8*		PrivateKey; //私钥数据 ，32字节长度
}  ECC_PriKey;

typedef struct 
{
 
	UINT32      nBits;	//算法位数，目前只支持256或192		
    UINT8*		X; //公钥坐标X ，32字节长度
	UINT8*		Y; //公钥坐标Y ，32字节长度
}  ECC_PubKey; 
/*******************************************************************************
函数原型：	UINT8 ECDSA_GenKey(ECC_PriKey *pPriKey, ECC_PubKey *pPubKey,
                UINT32 nBits, BOOL bPriKeyExist)
函数说明：	生成密钥。 
参数说明：	pPriKey [I] --- ECC_PriKey结构私钥信息。
            pPubKey [I][O] --- ECC_PubKey结构公钥，利用私钥pPriKey通过ECDSA算法生成。
            nBits [I] --- 定义采用素数域为nBits位的ECDSA曲线，目前支持192和256。
            bPriKeyExist [I] --- BOOL变量，1代表pPriKey中已经存放私钥数据，0代表私钥数据需要随机生成。
返 回 值：	RT_OK--成功 
            RT_FAIL--失败 
********************************************************************************/ 
UINT8 ECDSA_GenKey(ECC_PriKey *pPriKey,ECC_PubKey *pPubKey,UINT32 nBits,BOOL bPriKeyExist);

/*******************************************************************************
函数原型：	UINT8 ECDSA_Sign(ECC_PriKey PriKey, UINT8 *pHash, UINT32 HashLen,
 				UINT8 *pResult, UINT32 *pResultLen)
函数说明：	数字签名。
参数说明：	pPriKey [I] --- ECC_PriKey结构私钥信息。
			pHash [I] --- 杂凑值。
			HashLen [I] --- 杂凑值长度，一般为32字节或20字节长。
			pResult [O] --- 签名信息。
			pResultLen [O] --- 签名信息长度。为2倍的密钥长度，如密钥为192，则返回的长度为2*192bit=48Bytes
返 回 值：	RT_OK--成功 
            RT_FAIL--失败
********************************************************************************/
UINT8 ECDSA_Sign(ECC_PriKey* PriKey, 
                     UINT8 *pHash,  
                     UINT32 HashLen, 
                     UINT8 *pResult,
                     UINT32 *pResultLen
                    );

/*******************************************************************************
函数原型：	UINT8 ECDSA_Verify(ECC_PubKey PubKey, UINT8 *pHash, UINT32 HashLen, 
                               UINT8 *pResult, UINT32 ResultLen)
函数说明：	数字认证。通过公钥对签名进行认证。
参数说明：	pPubKey [I] --- ECC_PubKey结构公钥信息。
			pHash [I] --- 杂凑值。
			HashLen [I] --- 杂凑值长度。
			pResult [I] --- 签过名的信息。
			ResultLen [I] --- 签名信息长度
返 回 值：	RT_OK--成功 
             RT_FAIL--失败 
********************************************************************************/
UINT8 ECDSA_Verify(ECC_PubKey* PubKey,  
                     UINT8 *pHash, 
                     UINT32 HashLen,  
                     UINT8 *pResult,
                     UINT32 ResultLen
                     );
 
/***************************************************************************
* Subroutine:	ECDSA_PubKeyEncrypt
* Function:		公钥加密，只支持一块加密，每块大小为ECC的算法长度（nBits/8）*2字节
* Param:	 
            【IN】  PubKey: 公钥         
            【OUT】  Buf: 加密后的结果缓冲区
            【OUT】  pBufLen: 结果缓冲区大小，大小为ECC的算法长度（nBits/8）*4字节，对于ECC 192，长度为(192/8)*4=96字节
            【IN】  pM: 明文           
            【IN】  nMLen: 明文长度，固定为算法长度（nBits/8）*2字节，对于ECC 192，长度为(192/8)*2=48字节，不够请自己补充。
             
* Description:	 
* Date:			2014.08.04
* ModifyRecord:
* *************************************************************************/
UINT8 ECDSA_PubKeyEncrypt(ECC_PubKey* PubKey,
                          UINT8 *Buf, 
    					  UINT32 *pBufLen, 
    					  UINT8 *pM, 
                          UINT32 nMLen
                     );
/***************************************************************************
* Subroutine:	ECDSA_PriKeyDecrypt
* Function:		私钥解密，只支持一块解密，每块大小为ECC的算法长度（nBits/8）*4字节
* Param:	 
            【IN】  PriKey: 私钥         
            【OUT】  Buf: 解密后的结果缓冲区
            【OUT】  pBufLen: 结果缓冲区大小，大小为ECC的算法长度（nBits/8）*2字节，对于ECC 192，长度为(192/8)*2=48字节
            【IN】  pEM: 加密的密文           
            【IN】  nEMLen: 明文长度，固定为算法长度（nBits/8）*2字节，对于ECC 192，长度为(192/8)*4=96字节
             
* Description:	 
* Date:			2014.08.04
* ModifyRecord:
* *************************************************************************/
UINT8 ECDSA_PriKeyDecrypt(
    ECC_PriKey* PriKey,
    UINT8   *Buf, 
    UINT32  *pBufLen, 
    UINT8   *pEM, 
    UINT32  nEMLen
    );
/***************************************************************************
* Subroutine:	ECDSA_KeyExchange
* Function:		密钥交换函数
* Param:	 
            【IN】  PubKey: 对方的公钥 
			【IN】  PriKey: 自己的私钥   
            
            【OUT】  pKey: 密钥缓冲区                 
            【OUT】  pKeyLen: 密钥长度，固定为算法长度（nBits/8）*2字节，对于ECC 192，长度为(192/8)*2=48字节
             
* Description:	 
* Date:			2014.08.04
* ModifyRecord:
* *************************************************************************/
UINT8 ECDSA_KeyExchange(
    ECC_PubKey* PubKey,
    ECC_PriKey* PriKey,
    UINT8   *pKey, 
    UINT32  *pKeyLen    
    );   
#endif
