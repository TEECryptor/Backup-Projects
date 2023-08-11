#ifndef _SM2_H_
#define _SM2_H_

#define MAX_BYTE_LEN 32 //ECC ��˽Կ���������ֽڸ�����֧�ֵ����ECC�㷨����ΪMAX_BYTE_LEN*8 
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
/* ���߷��� : y^2 = x^3 + ax + b (mod p) */
typedef struct {
		UINT32 		len;	/*������  p ��NN_DIGIT��Ԫ��*/
		UINT32		p[MAX_DIG_LEN];		/* ������ p*/
		UINT32		a[MAX_DIG_LEN];		/* ϵ�� a, ͨ����Ϊ  -3 �� p-3 */
		UINT32		b[MAX_DIG_LEN];		/* ϵ�� b */
		UINT32      inv[MAX_DIG_LEN];
		Ep_Point	G;		/* ���� */

		/* �ڵ�һ��������,  ������Ⱥ���ɵĻ������ r | p+1 and p mod 12 = 11 */
		UINT32		rlen;	/*�������  r ��NN_DIGIT��Ԫ��*/
		UINT32		r[MAX_DIG_LEN];		/* �������  r  */
		UINT32		h;		/* h*r ΪȺ����, �ڵ�һ��������Ϊ b (���� Solinas ���� 2^a+2^b+1) */
	} Ep_Curve;
/* SM2 private key. */
typedef struct 
{ 	
    UINT8*		PrivateKey; //˽Կ���ݣ�32�ֽڳ���
}  SM2_PriKey;
/* SM2 public key. */
typedef struct 
{ 
    UINT8*		X; //��Կ����X,32�ֽڳ���
	UINT8*		Y; //��Կ����Y��32�ֽڳ���
}  SM2_PubKey;
/*******************************************************************************
����ԭ�ͣ�	UINT8 SM3_GetSignIDValue(SM3Value *pSm3v,
                        SM2_PubKey* PubKey, 
                        UINT8 *pID, 
                        UINT32 IDLen)
����˵����	�õ�ǩ����Ҫ��Hashֵ ��
����˵����	PubKey [I] --- SM2_PubKey�ṹ��Կ��Ϣ��
			sm3v [O] --- �Ӵ�ֵ��		 
			pID [I] --- �û�ID��
			IDLen [I] --- �û�ID����
�� �� ֵ��	RT_OK--�ɹ�      
			RT_FAIL--ʧ��
********************************************************************************/
UINT8 SM3_GetSignIDValue(SM3Value *pSm3v,
                        SM2_PubKey* PubKey, 
                        UINT8 *pID, 
                        UINT32 IDLen 
                        );
/*******************************************************************************
����ԭ�ͣ�	UINT8 SM2_GetHashValue(SM2_PubKey PubKey, SM3Value* sm3v, UINT8 *pMsg, UINT32 msgLen, UINT8 *pID, UINT32 IDLen)
����˵����	SM2 �Ӵ�ֵ���㡣
����˵����	PubKey [I] --- SM2_PubKey�ṹ��Կ��Ϣ��
			sm3v [O] --- �Ӵ�ֵ��
			pMsg [I] --- ��Ϣ��
			msgLen [I] --- ��Ϣ���ȡ�
			pID [I] --- �û�ID��
			IDLen [I] --- �û�ID����
�� �� ֵ��	RT_OK--�ɹ�      
			RT_FAIL--ʧ��
********************************************************************************/
UINT8  SM2_GetHashValue(SM2_PubKey* PubKey,
                       SM3Value* sm3v,
                       UINT8 *pMsg,
                       UINT32 msgLen,
                       UINT8 *pID,
                       UINT32 IDLen);

/*******************************************************************************
����ԭ�ͣ�	UINT8 SM2_GenKey(SM2_PriKey *pPriKey, SM2_PubKey *pPubKey,
                UINT32 nBits, BOOL bPriKeyExist)
����˵����	������Կ�� 
����˵����	pPriKey [I] --- SM2_PriKey�ṹ˽Կ��Ϣ��
			pPubKey [I][O] --- SM2_PubKey�ṹ��Կ������˽ԿpPriKeyͨ��SM2�㷨���ɡ�
			nBits [I] --- �������������ΪnBitsλ��SM2���ߣ�Ŀǰֻ֧��256��
			bPriKeyExist [I] --- BOOL������1����pPriKey���Ѿ����˽Կ���ݣ�0����˽Կ������Ҫ�������.
�� �� ֵ��	RT_OK--�ɹ� 
			RT_FAIL--ʧ��
********************************************************************************/
UINT8 SM2_GenKey(SM2_PriKey *pPriKey,
                 SM2_PubKey *pPubKey,
                 UINT32 nBits,
                 BOOL bPriKeyExist);
/*******************************************************************************
����ԭ�ͣ�	UINT8 SM2_Sign(SM2_PriKey PriKey, UINT8 *pHash, UINT32 HashLen, UINT8 *pResult,  UINT32 *pResultLen)
����˵����	����ǩ����
����˵����	PriKey [I] --- ECC_PriKey�ṹ˽Կ��Ϣ��
			pHash [I] --- �Ӵ�ֵ��
			HashLen [I] --- �Ӵ�ֵ���ȡ�
			pResult [O] --- ǩ����Ϣ��
			pResultLen [O] --- ǩ����Ϣ����
�� �� ֵ��	RT_OK--�ɹ�
			RT_FAIL--ʧ��

********************************************************************************/
UINT8 SM2_Sign(SM2_PriKey* PriKey, 
                    UINT8 *pHash, 
                    UINT32 HashLen,                  
                    UINT8 *pResult,
                    UINT32 *pResultLen                    
                    );
/*******************************************************************************
����ԭ�ͣ�	 UINT8 SM2_Verify(SM2_PubKey PubKey, UINT8 *pHash, UINT32 HashLen, UINT8 *pResult, UINT32 ResultLen)
����˵����	������֤��ͨ����Կ��ǩ��������֤
����˵����	PubKey [I] --- SM2_PubKey�ṹ��Կ��Ϣ��
			pHash [I] --- �Ӵ���Կ��
			HashLen [I] --- �Ӵ���Կ���ȡ�
			pResult [I] --- ǩ��������Ϣ��
			ResultLen [I] --- ǩ��������Ϣ�ĳ��ȡ�ǩ����Ϣ���ȡ�Ϊ2������Կ���ȣ�����ԿΪ256���򷵻صĳ���Ϊ2*256bit=64Bytes��
�� �� ֵ��	RT_OK--�ɹ� 
			RT_FAIL--ʧ��
********************************************************************************/
UINT8 SM2_Verify(SM2_PubKey* PubKey, 
                     UINT8 *pHash,
                     UINT32 HashLen,                  
                     UINT8 *pResult,
                     UINT32 ResultLen 
                    );
/*******************************************************************************
����ԭ�ͣ�	UINT8 SM2_PubKeyEncrypt( SM2_PubKey PubKey, UINT8 *Buf, UINT32 *pBufLen, UINT8 *pM, UINT32 nMLen)
����˵����	SM2��Կ�������㡣
����˵����	PubKey [I] --- SM2_PubKey�ṹ��Կ��Ϣ��
			Buf [I] [O] --- ������壬�������ΪNULL,��pBufLen������Ҫ�Ļ�������С��
			pBufLen [I] [O] --- ���뻺����Buf�Ĵ�С�����ʵ�ʵļ��ܺ����Ϣ���ȡ�
			pM [I] --- �û�������Ϣ��
			nMLen [I] --- �û�������Ϣ����
�� �� ֵ��	RT_OK--�ɹ�      
			RT_FAIL--ʧ�� 
********************************************************************************/
UINT8 SM2_PubKeyEncrypt(
                        SM2_PubKey* PubKey, 
                        UINT8 *Buf, 
                        UINT32 *pBufLen, 
                        UINT8 *pM, 
                        UINT32 nMLen
                        );
/*******************************************************************************
����ԭ�ͣ�	UINT8 SM2_PriKeyDecrypt(SM2_PriKey PriKey, UINT8   *Buf, UINT32  *pBufLen,     UINT8   *pEM, UINT32  nEMLen)
����˵����	SM2˽Կ�������㡣
����˵����	PriKey [I] --- SM2_PriKey�ṹ˽Կ��Ϣ��
			Buf[I] [O] --- ������壬�������ΪNULL����pBufLen������Ҫ�Ļ�������С��
			pBufLen [I] [O] --- ���뻺����Buf�Ĵ�С�����ʵ�ʵĽ��ܺ����Ϣ���ȡ�
			pEM [I] --- ������Ϣ��
			nEMLen [I] --- ������Ϣ����
�� �� ֵ��	RT_OK--�ɹ� 
			RT_FAIL--ʧ��
********************************************************************************/
UINT8 SM2_PriKeyDecrypt(
					    SM2_PriKey* PriKey,
					    UINT8   *Buf, 
					    UINT32  *pBufLen, 
					    UINT8   *pEM, 
					    UINT32  nEMLen
					    );

/*******************************************************************************
����ԭ�ͣ�	UINT8 SM2_KeyExc_Init( UINT32 *RandA, Ep_Point *epA)
����˵����	����������ԿЭ�̲�����
����˵����	RandA [O] --- ����� 
  		    epA [O] --- ��Բ���ߵ� 
�� �� ֵ��	RT_OK--�ɹ�      
            RT_FAIL--ʧ��

********************************************************************************/
UINT8 SM2_KeyExc_Init(UINT32 *RandA, 
                      Ep_Point *epA);
/*******************************************************************************
����ԭ�ͣ�	UINT8 SM2_KeyExc_Response(Ep_Point *epA, SM2_PriKey sm2PriKeyB, 
                         SM2_PubKey sm2PubKeyB, SM2_PubKey sm2PubKeyA,
                         Ep_Point *epB, SM3Value *sm3SB, SM3Value *sm3S2,
                         UINT8* pAID, UINT32 AIDLen, UINT8* pBID, UINT32 BIDLen,
                         UINT8* pKey, UINT32 KLen)
����˵����	��Ӧ������Э����Կ��Э�̲�����
����˵����	epA [I] --- ����Э�̲�������Բ���ߵ�
			sm2PriKeyB [I] --- ��Ӧ��˽Կ 
			sm2PubKeyB [I] --- ��Ӧ����Կ 
			sm2PubKeyA [I] --- ���𷽹�Կ 
			epB [O] --- ��Ӧ���̲�������Բ���ߵ�
			sm3SB [O] --- ��Ӧ���̲�������ϣֵ 
			sm3S2 [O] --- ��Ӧ���̲�������ϣֵ 
			pAID [I] --- �����û�ID 
			AIDLen [I] --- ����ID���� 
			pBID [I] --- ��Ӧ���û�ID 
			BIDLen [I] --- ��Ӧ��ID���� 
			pKey [O] --- Э����Կ
			KLen[I]-��ҪЭ�̵���Կ����
�� �� ֵ��	RT_OK--�ɹ� 
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
����ԭ�ͣ�	UINT8 SM2_KeyExc_Spons( BN_DIGIT *RandA, Ep_Point *epA, 
                    Ep_Point *epB, SM3Value *sm3SB,
                    SM2_PriKey sm2PriKeyA, SM2_PubKey sm2PubKeyA, 
SM2_PubKey sm2PubKeyB, SM3Value *sm3SA,
                    UINT8* pAID, UINT32 AIDLen, UINT8* pBID, UINT32 BIDLen,
                     UINT8* pKey, UINT32 KLen)
����˵����	��������Э����Կ��
����˵����	RandA [I] --- Э�̲���������� 
		 	epA [I] --- ����Э�̲�������Բ���ߵ�
		 	epB [I] --- ��Ӧ��Э�̲�������Բ���ߵ�
		 	sm3SB [I] --- ��Ӧ���̲�������ϣֵ 
		 	sm2PriKeyA [I] --- ����˽Կ
		 	sm2PubKeyA [I] --- ���𷽹�Կ
		 	m2PubKeyB [I] --- ��Ӧ����Կ
		 	sm3SA [O] --- ����Э�̲�������ϣֵ
		    pAID [I] --- �����û�ID
		    AIDLen [I] --- ����ID����
		 	pBID [I] --- ��Ӧ���û�ID
		 	BIDLen [I] --- ��Ӧ��ID����
		 	pKey [O] --- Э����Կ
		    KLen[I]-��ҪЭ�̵���Կ���ȡ�
�� �� ֵ��	RT_OK--�ɹ� 
			RT_FAIL--ʧ�� 
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
����ԭ�ͣ�UINT8 SM2_Key_Exchange( SM2_PriKey *sm2TmpPriA, SM2_PubKey *sm2TmpPubA, 						
                                  SM2_PubKey *sm2TmpPubB, SM2_PriKey *sm2PriKeyA,
								  SM2_PubKey *sm2PubKeyB, UINT8 *ZA, UINT8 *ZB,
                                  UINT8 KeyLen, UINT8 *pKey)
����˵����Э����Կ���������ڷ��𷽺���Ӧ��Э�����ɹ�ͬ�ĻỰ��Կ
����˵����
		sm2TmpPriA [I] ---������ʱ˽Կ          
		sm2TmpPubA [I] --- ������ʱ��Կ
		sm2TmpPubB [I] ---�Է���ʱ��Կ                     
		sm2PriKeyA [I] ---����˽Կ 
		sm2PubKeyB [I] ---�Է�����Կ 
		ZA [I] ---����Zֵ 
		ZB [I] ---��Ӧ��Zֵ  
		KeyLen [I] ---Ҫ��Э�̳��ĻỰ��Կ���� 
		pKey [O] ---Э����Կ 
����ֵ��RT_OK--�ɹ� 
		RT_FAIL--ʧ��
ע�⣺�����������Կ������A��������B����Է���
  	  ���������Zֵ������A�����𷽣�B������Ӧ����
  	  ��ע�����֡�
********************************************************************************/
UINT8 SM2_Key_Exchange( SM2_PriKey *sm2TmpPriA, SM2_PubKey *sm2TmpPubA, SM2_PubKey *sm2TmpPubB,
                    SM2_PriKey *sm2PriKeyA, SM2_PubKey *sm2PubKeyB,
                    UINT8 *ZA, UINT8 *ZB,
                    UINT8 KeyLen, UINT8 *pKey); 



#endif
