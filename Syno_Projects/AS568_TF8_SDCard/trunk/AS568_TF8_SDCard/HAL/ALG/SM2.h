#ifndef _SM2_H_
#define _SM2_H_

#define MAX_BYTE_LEN 32 //ECC 公私钥数组的最大字节个数，支持的最大ECC算法长度为MAX_BYTE_LEN*8 
#define MAX_DIG_LEN 	8 

/* Point type*/ 
typedef struct {
		UINT32		x[MAX_DIG_LEN];
		UINT32		y[MAX_DIG_LEN];
	} Ep_Point;
/* SM3 hash value type. */	 
typedef struct _sm3Value
{
	UINT8*  Buf;
}   SM3Value; 
/* 曲线方程 : y^2 = x^3 + ax + b (mod p) */
typedef struct {
		UINT32 		len;	/*大素数  p 的NN_DIGIT单元数*/
		UINT32		p[MAX_DIG_LEN];		/* 大素数 p*/
		UINT32		a[MAX_DIG_LEN];		/* 系数 a, 通常设为  -3 或 p-3 */
		UINT32		b[MAX_DIG_LEN];		/* 系数 b */
		UINT32      inv[MAX_DIG_LEN];
		Ep_Point	G;		/* 基点 */

		/* 在第一类曲线中,  素数子群构成的基点的秩 r | p+1 and p mod 12 = 11 */
		UINT32		rlen;	/*基点的秩  r 的NN_DIGIT单元数*/
		UINT32		r[MAX_DIG_LEN];		/* 基点的秩  r  */
		UINT32		h;		/* h*r 为群的秩, 在第一类曲线中为 b (比如 Solinas 素数 2^a+2^b+1) */
	} Ep_Curve;
/* SM2 private key. */
typedef struct 
{ 	
    UINT8*		PrivateKey; //私钥数据，32字节长度
}  SM2_PriKey;
/* SM2 public key. */
typedef struct 
{ 
    UINT8*		X; //公钥坐标X,32字节长度
	UINT8*		Y; //公钥坐标Y，32字节长度
}  SM2_PubKey;
/*******************************************************************************
函数原型：	UINT8 SM3_GetSignIDValue(SM3Value *pSm3v,
                        SM2_PubKey* PubKey, 
                        UINT8 *pID, 
                        UINT32 IDLen)
函数说明：	得到签名需要的Hash值 。
参数说明：	PubKey [I] --- SM2_PubKey结构公钥信息。
			sm3v [O] --- 杂凑值。		 
			pID [I] --- 用户ID。
			IDLen [I] --- 用户ID长度
返 回 值：	RT_OK--成功      
			RT_FAIL--失败
********************************************************************************/
UINT8 SM3_GetSignIDValue(SM3Value *pSm3v,
                        SM2_PubKey* PubKey, 
                        UINT8 *pID, 
                        UINT32 IDLen 
                        );
/*******************************************************************************
函数原型：	UINT8 SM2_GetHashValue(SM2_PubKey PubKey, SM3Value* sm3v, UINT8 *pMsg, UINT32 msgLen, UINT8 *pID, UINT32 IDLen)
函数说明：	SM2 杂凑值计算。
参数说明：	PubKey [I] --- SM2_PubKey结构公钥信息。
			sm3v [O] --- 杂凑值。
			pMsg [I] --- 消息。
			msgLen [I] --- 消息长度。
			pID [I] --- 用户ID。
			IDLen [I] --- 用户ID长度
返 回 值：	RT_OK--成功      
			RT_FAIL--失败
********************************************************************************/
UINT8  SM2_GetHashValue(SM2_PubKey* PubKey,
                       SM3Value* sm3v,
                       UINT8 *pMsg,
                       UINT32 msgLen,
                       UINT8 *pID,
                       UINT32 IDLen);

/*******************************************************************************
函数原型：	UINT8 SM2_GenKey(SM2_PriKey *pPriKey, SM2_PubKey *pPubKey,
                UINT32 nBits, BOOL bPriKeyExist)
函数说明：	生成密钥。 
参数说明：	pPriKey [I] --- SM2_PriKey结构私钥信息。
			pPubKey [I][O] --- SM2_PubKey结构公钥，利用私钥pPriKey通过SM2算法生成。
			nBits [I] --- 定义采用素数域为nBits位的SM2曲线，目前只支持256。
			bPriKeyExist [I] --- BOOL变量，1代表pPriKey中已经存放私钥数据，0代表私钥数据需要随机生成.
返 回 值：	RT_OK--成功 
			RT_FAIL--失败
********************************************************************************/
UINT8 SM2_GenKey(SM2_PriKey *pPriKey,
                 SM2_PubKey *pPubKey,
                 UINT32 nBits,
                 BOOL bPriKeyExist);
/*******************************************************************************
函数原型：	UINT8 SM2_Sign(SM2_PriKey PriKey, UINT8 *pHash, UINT32 HashLen, UINT8 *pResult,  UINT32 *pResultLen)
函数说明：	数字签名。
参数说明：	PriKey [I] --- ECC_PriKey结构私钥信息。
			pHash [I] --- 杂凑值。
			HashLen [I] --- 杂凑值长度。
			pResult [O] --- 签名信息。
			pResultLen [O] --- 签名信息长度
返 回 值：	RT_OK--成功
			RT_FAIL--失败

********************************************************************************/
UINT8 SM2_Sign(SM2_PriKey* PriKey, 
                    UINT8 *pHash, 
                    UINT32 HashLen,                  
                    UINT8 *pResult,
                    UINT32 *pResultLen                    
                    );
/*******************************************************************************
函数原型：	 UINT8 SM2_Verify(SM2_PubKey PubKey, UINT8 *pHash, UINT32 HashLen, UINT8 *pResult, UINT32 ResultLen)
函数说明：	数字认证。通过公钥对签名进行认证
参数说明：	PubKey [I] --- SM2_PubKey结构公钥信息。
			pHash [I] --- 杂凑密钥。
			HashLen [I] --- 杂凑密钥长度。
			pResult [I] --- 签过名的信息。
			ResultLen [I] --- 签过名的信息的长度。签名信息长度。为2倍的密钥长度，如密钥为256，则返回的长度为2*256bit=64Bytes。
返 回 值：	RT_OK--成功 
			RT_FAIL--失败
********************************************************************************/
UINT8 SM2_Verify(SM2_PubKey* PubKey, 
                     UINT8 *pHash,
                     UINT32 HashLen,                  
                     UINT8 *pResult,
                     UINT32 ResultLen 
                    );
/*******************************************************************************
函数原型：	UINT8 SM2_PubKeyEncrypt( SM2_PubKey PubKey, UINT8 *Buf, UINT32 *pBufLen, UINT8 *pM, UINT32 nMLen)
函数说明：	SM2公钥加密运算。
参数说明：	PubKey [I] --- SM2_PubKey结构公钥信息。
			Buf [I] [O] --- 输出缓冲，如果输入为NULL,则pBufLen返回需要的缓冲区大小。
			pBufLen [I] [O] --- 输入缓冲区Buf的大小，输出实际的加密后的信息长度。
			pM [I] --- 用户明文信息。
			nMLen [I] --- 用户明文信息长度
返 回 值：	RT_OK--成功      
			RT_FAIL--失败 
********************************************************************************/
UINT8 SM2_PubKeyEncrypt(
                        SM2_PubKey* PubKey, 
                        UINT8 *Buf, 
                        UINT32 *pBufLen, 
                        UINT8 *pM, 
                        UINT32 nMLen
                        );
/*******************************************************************************
函数原型：	UINT8 SM2_PriKeyDecrypt(SM2_PriKey PriKey, UINT8   *Buf, UINT32  *pBufLen,     UINT8   *pEM, UINT32  nEMLen)
函数说明：	SM2私钥解密运算。
参数说明：	PriKey [I] --- SM2_PriKey结构私钥信息。
			Buf[I] [O] --- 输出缓冲，如果输入为NULL，则pBufLen返回需要的缓冲区大小。
			pBufLen [I] [O] --- 输入缓冲区Buf的大小，输出实际的解密后的信息长度。
			pEM [I] --- 密文信息。
			nEMLen [I] --- 密文信息长度
返 回 值：	RT_OK--成功 
			RT_FAIL--失败
********************************************************************************/
UINT8 SM2_PriKeyDecrypt(
					    SM2_PriKey* PriKey,
					    UINT8   *Buf, 
					    UINT32  *pBufLen, 
					    UINT8   *pEM, 
					    UINT32  nEMLen
					    );

/*******************************************************************************
函数原型：	UINT8 SM2_KeyExc_Init( UINT32 *RandA, Ep_Point *epA)
函数说明：	发起生成密钥协商参数。
参数说明：	RandA [O] --- 随机数 
  		    epA [O] --- 椭圆曲线点 
返 回 值：	RT_OK--成功      
            RT_FAIL--失败

********************************************************************************/
UINT8 SM2_KeyExc_Init(UINT32 *RandA, 
                      Ep_Point *epA);
/*******************************************************************************
函数原型：	UINT8 SM2_KeyExc_Response(Ep_Point *epA, SM2_PriKey sm2PriKeyB, 
                         SM2_PubKey sm2PubKeyB, SM2_PubKey sm2PubKeyA,
                         Ep_Point *epB, SM3Value *sm3SB, SM3Value *sm3S2,
                         UINT8* pAID, UINT32 AIDLen, UINT8* pBID, UINT32 BIDLen,
                         UINT8* pKey, UINT32 KLen)
函数说明：	响应方生成协商密钥和协商参数。
参数说明：	epA [I] --- 发起方协商参数，椭圆曲线点
			sm2PriKeyB [I] --- 响应方私钥 
			sm2PubKeyB [I] --- 响应方公钥 
			sm2PubKeyA [I] --- 发起方公钥 
			epB [O] --- 响应方商参数，椭圆曲线点
			sm3SB [O] --- 响应方商参数，哈希值 
			sm3S2 [O] --- 响应方商参数，哈希值 
			pAID [I] --- 发起方用户ID 
			AIDLen [I] --- 发起方ID长度 
			pBID [I] --- 响应方用户ID 
			BIDLen [I] --- 响应方ID长度 
			pKey [O] --- 协商密钥
			KLen[I]-需要协商的密钥长度
返 回 值：	RT_OK--成功 
********************************************************************************/
UINT8 SM2_KeyExc_Response(Ep_Point *epA,
                         SM2_PriKey* sm2PriKeyB, 
                         SM2_PubKey* sm2PubKeyA,
                         Ep_Point *epB, 
						 SM3Value *sm3SB, 
						 SM3Value *sm3S2, 
                         SM3Value* ZA,
						 SM3Value* ZB,
                         UINT8* pKey, 
						 UINT32 KLen);
/*******************************************************************************
函数原型：	UINT8 SM2_KeyExc_Spons( BN_DIGIT *RandA, Ep_Point *epA, 
                    Ep_Point *epB, SM3Value *sm3SB,
                    SM2_PriKey sm2PriKeyA, SM2_PubKey sm2PubKeyA, 
SM2_PubKey sm2PubKeyB, SM3Value *sm3SA,
                    UINT8* pAID, UINT32 AIDLen, UINT8* pBID, UINT32 BIDLen,
                     UINT8* pKey, UINT32 KLen)
函数说明：	发起方生成协商密钥。
参数说明：	RandA [I] --- 协商参数，随机数 
		 	epA [I] --- 发起方协商参数，椭圆曲线点
		 	epB [I] --- 响应方协商参数，椭圆曲线点
		 	sm3SB [I] --- 响应方商参数，哈希值 
		 	sm2PriKeyA [I] --- 发起方私钥
		 	sm2PubKeyA [I] --- 发起方公钥
		 	m2PubKeyB [I] --- 响应方公钥
		 	sm3SA [O] --- 发起方协商参数，哈希值
		    pAID [I] --- 发起方用户ID
		    AIDLen [I] --- 发起方ID长度
		 	pBID [I] --- 响应方用户ID
		 	BIDLen [I] --- 响应方ID长度
		 	pKey [O] --- 协商密钥
		    KLen[I]-需要协商的密钥长度。
返 回 值：	RT_OK--成功 
			RT_FAIL--失败 
********************************************************************************/                         
UINT8 SM2_KeyExc_Spons( UINT32 *RandA, 
                        Ep_Point *epA, 
	                    Ep_Point *epB, 
						SM3Value *sm3SB,
	                    SM2_PriKey* sm2PriKeyA, 
						SM2_PubKey* sm2PubKeyB,
	                    SM3Value *sm3SA, 
	                    SM3Value* ZA,
						SM3Value* ZB,
	                    UINT8* pKey, 
						UINT32 KLen); 
/*******************************************************************************
函数原型：UINT8 SM2_Key_Exchange( SM2_PriKey *sm2TmpPriA, SM2_PubKey *sm2TmpPubA, 						
                                  SM2_PubKey *sm2TmpPubB, SM2_PriKey *sm2PriKeyA,
								  SM2_PubKey *sm2PubKeyB, UINT8 *ZA, UINT8 *ZB,
                                  UINT8 KeyLen, UINT8 *pKey)
函数说明：协商密钥函数，用于发起方和响应方协商生成共同的会话密钥
参数说明：
		sm2TmpPriA [I] ---己方临时私钥          
		sm2TmpPubA [I] --- 己方临时公钥
		sm2TmpPubB [I] ---对方临时公钥                     
		sm2PriKeyA [I] ---己方私钥 
		sm2PubKeyB [I] ---对方方公钥 
		ZA [I] ---发起方Z值 
		ZB [I] ---响应方Z值  
		KeyLen [I] ---要求协商出的会话密钥长度 
		pKey [O] ---协商密钥 
返回值：RT_OK--成功 
		RT_FAIL--失败
注意：输入参数中密钥参数，A代表己方，B代表对方；
  	  输入参数，Z值参数，A代表发起方，B代表响应方；
  	  请注意区分。
********************************************************************************/
UINT8 SM2_Key_Exchange( SM2_PriKey *sm2TmpPriA, SM2_PubKey *sm2TmpPubA, SM2_PubKey *sm2TmpPubB,
                    SM2_PriKey *sm2PriKeyA, SM2_PubKey *sm2PubKeyB,
                    UINT8 *ZA, UINT8 *ZB,
                    UINT8 KeyLen, UINT8 *pKey); 



#endif
