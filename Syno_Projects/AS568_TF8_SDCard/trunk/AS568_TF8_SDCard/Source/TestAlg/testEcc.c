#include "SY03.h"
//----test ECDSA mode ----
void TestECDSA(void)
{  
    UINT8  Msg[20]="Yin-HongWei"; /*ÏûÏ¢*/  
    UINT8  R[80];  
    UINT8 sv[20];
	UINT8 tmpBuf[96];
    UINT8 ret ;
    UINT32 nLen;
   
    ECC_PriKey ecPriKey;
    ECC_PubKey ecPubKey;
    SHA1_CONTEXT ctx;
 
	ecPriKey.PrivateKey = (UINT8*)tmpBuf;
	ecPubKey.X = (UINT8*)(tmpBuf+32);
	ecPubKey.Y = (UINT8*)(tmpBuf+64);

    ret = ECDSA_GenKey(&ecPriKey,&ecPubKey,256,0);
	if(ret != RT_OK)
	{
		print("ECDSA Gen key fail!\n");
		ret = 0;
	}

    SHA1_Init(&ctx);
    SHA1_Update(&ctx,Msg, strlen((char *)Msg));
    SHA1_Final(&ctx,sv); 
    	  
    ret = ECDSA_Sign(&ecPriKey, sv, sizeof(sv), R,&nLen);
	if(ret != RT_OK)
	{
		print("ECDSA SIGN fail!\n");
		
	}
    ret = ECDSA_Verify(&ecPubKey, sv, sizeof(sv),R,nLen);
    if(ret != RT_OK)
	{
		print("ECDSA SIGN fail!\n");
		
	}
	print("ECDSA SIGN OK!\n");
    
}
