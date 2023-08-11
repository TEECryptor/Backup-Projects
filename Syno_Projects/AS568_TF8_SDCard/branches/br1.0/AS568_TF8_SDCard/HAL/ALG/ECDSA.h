#ifndef ELLIPTIC_ZP_H_
#define ELLIPTIC_ZP_H_ 1
 
#define MAX_BYTE_LEN 32 //ECC ��˽Կ���������ֽڸ�����֧�ֵ����ECC�㷨����ΪMAX_BYTE_LEN*8 


typedef struct 
{ 
	UINT32      nBits;	//�㷨λ����Ŀǰֻ֧��256��192		
    UINT8*		PrivateKey; //˽Կ���� ��32�ֽڳ���
}  ECC_PriKey;

typedef struct 
{
 
	UINT32      nBits;	//�㷨λ����Ŀǰֻ֧��256��192		
    UINT8*		X; //��Կ����X ��32�ֽڳ���
	UINT8*		Y; //��Կ����Y ��32�ֽڳ���
}  ECC_PubKey; 
/*******************************************************************************
����ԭ�ͣ�	UINT8 ECDSA_GenKey(ECC_PriKey *pPriKey, ECC_PubKey *pPubKey,
                UINT32 nBits, BOOL bPriKeyExist)
����˵����	������Կ�� 
����˵����	pPriKey [I] --- ECC_PriKey�ṹ˽Կ��Ϣ��
            pPubKey [I][O] --- ECC_PubKey�ṹ��Կ������˽ԿpPriKeyͨ��ECDSA�㷨���ɡ�
            nBits [I] --- �������������ΪnBitsλ��ECDSA���ߣ�Ŀǰ֧��192��256��
            bPriKeyExist [I] --- BOOL������1����pPriKey���Ѿ����˽Կ���ݣ�0����˽Կ������Ҫ������ɡ�
�� �� ֵ��	RT_OK--�ɹ� 
            RT_FAIL--ʧ�� 
********************************************************************************/ 
UINT8 ECDSA_GenKey(ECC_PriKey *pPriKey,ECC_PubKey *pPubKey,UINT32 nBits,BOOL bPriKeyExist);

/*******************************************************************************
����ԭ�ͣ�	UINT8 ECDSA_Sign(ECC_PriKey PriKey, UINT8 *pHash, UINT32 HashLen,
 				UINT8 *pResult, UINT32 *pResultLen)
����˵����	����ǩ����
����˵����	pPriKey [I] --- ECC_PriKey�ṹ˽Կ��Ϣ��
			pHash [I] --- �Ӵ�ֵ��
			HashLen [I] --- �Ӵ�ֵ���ȣ�һ��Ϊ32�ֽڻ�20�ֽڳ���
			pResult [O] --- ǩ����Ϣ��
			pResultLen [O] --- ǩ����Ϣ���ȡ�Ϊ2������Կ���ȣ�����ԿΪ192���򷵻صĳ���Ϊ2*192bit=48Bytes
�� �� ֵ��	RT_OK--�ɹ� 
            RT_FAIL--ʧ��
********************************************************************************/
UINT8 ECDSA_Sign(ECC_PriKey* PriKey, 
                     UINT8 *pHash,  
                     UINT32 HashLen, 
                     UINT8 *pResult,
                     UINT32 *pResultLen
                    );

/*******************************************************************************
����ԭ�ͣ�	UINT8 ECDSA_Verify(ECC_PubKey PubKey, UINT8 *pHash, UINT32 HashLen, 
                               UINT8 *pResult, UINT32 ResultLen)
����˵����	������֤��ͨ����Կ��ǩ��������֤��
����˵����	pPubKey [I] --- ECC_PubKey�ṹ��Կ��Ϣ��
			pHash [I] --- �Ӵ�ֵ��
			HashLen [I] --- �Ӵ�ֵ���ȡ�
			pResult [I] --- ǩ��������Ϣ��
			ResultLen [I] --- ǩ����Ϣ����
�� �� ֵ��	RT_OK--�ɹ� 
             RT_FAIL--ʧ�� 
********************************************************************************/
UINT8 ECDSA_Verify(ECC_PubKey* PubKey,  
                     UINT8 *pHash, 
                     UINT32 HashLen,  
                     UINT8 *pResult,
                     UINT32 ResultLen
                     );
 
/***************************************************************************
* Subroutine:	ECDSA_PubKeyEncrypt
* Function:		��Կ���ܣ�ֻ֧��һ����ܣ�ÿ���СΪECC���㷨���ȣ�nBits/8��*2�ֽ�
* Param:	 
            ��IN��  PubKey: ��Կ         
            ��OUT��  Buf: ���ܺ�Ľ��������
            ��OUT��  pBufLen: �����������С����СΪECC���㷨���ȣ�nBits/8��*4�ֽڣ�����ECC 192������Ϊ(192/8)*4=96�ֽ�
            ��IN��  pM: ����           
            ��IN��  nMLen: ���ĳ��ȣ��̶�Ϊ�㷨���ȣ�nBits/8��*2�ֽڣ�����ECC 192������Ϊ(192/8)*2=48�ֽڣ��������Լ����䡣
             
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
* Function:		˽Կ���ܣ�ֻ֧��һ����ܣ�ÿ���СΪECC���㷨���ȣ�nBits/8��*4�ֽ�
* Param:	 
            ��IN��  PriKey: ˽Կ         
            ��OUT��  Buf: ���ܺ�Ľ��������
            ��OUT��  pBufLen: �����������С����СΪECC���㷨���ȣ�nBits/8��*2�ֽڣ�����ECC 192������Ϊ(192/8)*2=48�ֽ�
            ��IN��  pEM: ���ܵ�����           
            ��IN��  nEMLen: ���ĳ��ȣ��̶�Ϊ�㷨���ȣ�nBits/8��*2�ֽڣ�����ECC 192������Ϊ(192/8)*4=96�ֽ�
             
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
* Function:		��Կ��������
* Param:	 
            ��IN��  PubKey: �Է��Ĺ�Կ 
			��IN��  PriKey: �Լ���˽Կ   
            
            ��OUT��  pKey: ��Կ������                 
            ��OUT��  pKeyLen: ��Կ���ȣ��̶�Ϊ�㷨���ȣ�nBits/8��*2�ֽڣ�����ECC 192������Ϊ(192/8)*2=48�ֽ�
             
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
