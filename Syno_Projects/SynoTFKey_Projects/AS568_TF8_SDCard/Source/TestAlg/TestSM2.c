#include "SY03.h"
//----test SM2 sign ----
void TestSM2Sign(void)
{
	UINT8 UserID[16] = {    //用户ID 
		0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38
	};
	UINT8 UserInfo[32] = {    //待签名信息
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	};
	UINT8 PriKey[] = {    //私钥
		0xC5, 0x6A, 0x2B, 0x58, 0xA0, 0x94, 0xEF, 0x24, 0x41, 0x03, 0x79, 0x45, 0xBA, 0xB1, 0x39, 0x8C,
		0xC0, 0xDF, 0x9F, 0xC4, 0xF9, 0x9E, 0x9A, 0x60, 0x2C, 0xD8, 0x6F, 0xC2, 0xC3, 0x88, 0xAD, 0x0C
	}; 
 

	UINT8  R[100],tmpBuf[128];
	UINT32 nLen;
	
	UINT32 ret ;
	SM3Value *sm3;
	SM2_PriKey sm2PriKey;
	SM2_PubKey sm2PubKey;

	sm3 = (SM3Value*)tmpBuf;
	sm2PubKey.Y = (UINT8*)(tmpBuf+32);
	sm2PubKey.X = (UINT8*)(tmpBuf+64);
	sm2PriKey.PrivateKey = (UINT8*)(tmpBuf+96); 

	print("test sm2 sign\n");
	
	memcpy(sm2PriKey.PrivateKey,PriKey,32);

	ret = SM2_GenKey(&sm2PriKey,&sm2PubKey,256,1);

	if(ret==0)
	   print("sm2 gen key ok\n");
	
	ret = SM2_GetHashValue(&sm2PubKey,sm3,UserInfo, 32, (UINT8*)UserID, 32);
	


	ret = SM2_Sign(&sm2PriKey,(UINT8*)sm3,32,R,&nLen);

	if(ret!=RT_OK)
	{
		print("TestSM2Sign: sign fail\n");
	
	}
	ret =SM2_Verify(&sm2PubKey, (UINT8*)sm3,32, R ,64);
	if(ret!=RT_OK)
	{
		print("TestSM2Sign: verify fail\n");
	
	} 

	print("TestSM2Sign: OK\n");

}

//----test SM2 Encry mode ----	
void TestSM2Encry(void)
{
	UINT8 PriKey[] = {
		0x3B, 0xC2, 0xFC, 0xFC, 0xA4, 0x4A, 0x0D, 0x2C, 0x67, 0xFD, 0xF0, 0xD2, 0x50, 0xB8, 0x93, 0x06,
		0xA2, 0x1F, 0x59, 0xAF, 0xE7, 0x76, 0x81, 0xBA, 0xB1, 0x6F, 0x4A, 0xFD, 0xC6, 0xFC, 0x2B, 0xBB
	};
 
	UINT8 Plain[] = {
		0xFA, 0xE1, 0x6E, 0xF6, 0x6B, 0xE9, 0x53, 0xA7, 0x70, 0x3B, 0x08, 0x97, 0xBD, 0x9E, 0xBD, 0x07,
		0xF2, 0x25, 0x08, 0xE5, 0x7A, 0xF4, 0x39, 0x34, 0x8F, 0x67, 0xB3, 0x5E, 0x5F, 0xD1, 0xC9, 0x3F
	};
	UINT8 Encipher[] = {
		0x04,0xD2, 0x8C, 0xDA, 0x17, 0x6B, 0xE9, 0xBB, 0xE9, 0x23, 0x6A, 0xB8, 0xAA, 0xAC, 0xFE, 0x3A,
		0xE7, 0x53, 0x5D, 0x29, 0x34, 0x96, 0x14, 0x92, 0x56, 0x00, 0xE7, 0xF8, 0x16, 0xA9, 0x86, 0x5D,
		0xC5, 0xD8, 0x27, 0x95, 0x67, 0x82, 0xCD, 0xEB, 0xDE, 0x05, 0x52, 0xB2, 0xB1, 0x88, 0x02, 0x78,
		0xCE, 0x32, 0xE0, 0xC1, 0xA9, 0xF5, 0xE9, 0xA9, 0x72, 0x76, 0x74, 0xF4, 0x40, 0x48, 0x7C, 0x1E,
		0x4B, 0x20, 0x9E, 0x5B, 0x24, 0x00, 0xE5, 0x1F, 0xF9, 0x4D, 0xF4, 0xB4, 0xC6, 0xDC, 0xE6, 0xDB,
		0xBC, 0xC0, 0x17, 0x2C, 0x0A, 0x88, 0x28, 0xE6, 0xE1, 0xD7, 0x94, 0xA8, 0xD4, 0xC3, 0xFF, 0xB3,
		0x86, 0xBA, 0x89, 0xF1, 0x78, 0x3F, 0x0D, 0x42, 0xA3, 0x34, 0xA5, 0x50, 0x99, 0x73, 0xE8, 0x57,
		0x4A, 0x87, 0xCE, 0xA9, 0xC9, 0x47, 0xFE, 0x37, 0xE9, 0xC5, 0xAE, 0x54, 0x27, 0xE7, 0x3D, 0x06,
		0xB8
	};  
	UINT8 Msg[200], result[150],tmpBuf[96];
	UINT32 BufLen, MLen;
	UINT8 ret ;
	SM2_PriKey sm2PriKey;
	SM2_PubKey sm2PubKey;

	sm2PubKey.Y = (UINT8*)(tmpBuf);
	sm2PubKey.X = (UINT8*)(tmpBuf+32);
	sm2PriKey.PrivateKey = (UINT8*)(tmpBuf+64);

	memcpy(sm2PriKey.PrivateKey , PriKey,sizeof(PriKey));
 
	ret=SM2_GenKey(&sm2PriKey,&sm2PubKey,256,1);
	if(ret!=RT_OK)
	{
		print("TestSM2Encry: Gen key fail\n");
	
	}	

	MLen =200;
	ret=SM2_PriKeyDecrypt(&sm2PriKey,Msg, &MLen, Encipher,sizeof(Encipher));
	if(ret!=RT_OK)
	{
		print("TestSM2Encry: Pri key decrypt fail\n");

	}

	ret=SM2_GenKey(&sm2PriKey,&sm2PubKey,256,0);
	if(ret!=RT_OK)
	{
		print("TestSM2Encry: Gen key fail\n");

	}
	BufLen =200;
	ret = SM2_PubKeyEncrypt(&sm2PubKey, result, &BufLen, Plain, sizeof(Plain));
	if(ret!=RT_OK)
	{
		print("TestSM2Encry: Pub key encrypt fail\n");

	}
	
	MLen =200;
	ret = SM2_PriKeyDecrypt(&sm2PriKey, Msg, &MLen, result, BufLen);
	if(ret!=RT_OK)
	{
		print("TestSM2Encry: Pri key decrypt fail\n");

	}
	print("TestSM2Encry: OK\n");
}
//----test SM2 key exchange ----	
void Test_SM2_KeyExchange(void)
{
    SM2_PubKey PubKeyA, PubKeyB, tmpPubA, tmpPubB;
    SM2_PriKey PriKeyA, PriKeyB, tmpPriA, tmpPriB;
	UINT8 KA[48], KB[48], PA[64], PB[64], RA[64], RB[64], dA[32], dB[32], rA[32], rB[32];
    
    UINT8 ret;    
    UINT8 Aid[]={0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
    UINT8 Bid[]={0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};

   	UINT8 ZA[32],ZB[32];   
       
    PriKeyA.PrivateKey = (UINT8*)dA;	
	PriKeyB.PrivateKey = (UINT8*)dB;	
	tmpPriA.PrivateKey = (UINT8*)rA;	
	tmpPriB.PrivateKey = (UINT8*)rB;

	PubKeyA.X = (UINT8*)PA;
	PubKeyA.Y = (UINT8*)(PA+32);
	PubKeyB.X = (UINT8*)(PB);
	PubKeyB.Y = (UINT8*)(PB+32);
	tmpPubA.X = (UINT8*)RA;
	tmpPubA.Y = (UINT8*)(RA+32);
	tmpPubB.X = (UINT8*)(RB);
	tmpPubB.Y = (UINT8*)(RB+32);

	print("SM2_exchangeKey(only sm2_keyExc_spons) begin Gen key!\n");
	//发起方公私钥，sender 1
    SM2_GenKey(&PriKeyA, &PubKeyA, 256, 0);
    //响应方公私钥，receiver 0 
    SM2_GenKey(&PriKeyB, &PubKeyB, 256, 0);
    
    //rA_S and RA_S be equal to tmpPriA and tmpPubA ; SM2_GenKey function is SM2_KeyExc_Init.
    //生成发起方的临时公私钥 
    SM2_GenKey(&tmpPriA, &tmpPubA, 256, 0);
    SM2_GenKey(&tmpPriB, &tmpPubB, 256,0);
	print("SM2_exchangeKey gen key ok!\n");  
	

	//获取发起方和响应方用户信息的签名值ZA，ZB
	SM3_GetSignIDValue((SM3Value*)ZA,&PubKeyA,Aid,strlen((char*)Aid));
	SM3_GetSignIDValue((SM3Value*)ZB,&PubKeyB,Bid,strlen((char*)Bid));
	 

	ret = SM2_Key_Exchange(&tmpPriA, &tmpPubA, &tmpPubB, &PriKeyA, &PubKeyB, ZA, ZB, 32, KA);
	if(ret != RT_OK)
	{
	    print("KEY_EXCANGE_Test: KeyExc fail\n");
	 	return;
	}
	ret = SM2_Key_Exchange(&tmpPriB, &tmpPubB, &tmpPubA, &PriKeyB, &PubKeyA, ZA, ZB, 32, KB);
	if(ret != RT_OK)
	{
	    print("KEY_EXCANGE_Test: KeyExc fail\n");
	 	return;
	}	
	if(memcmp(KA,KB,32))
	{
	    print("KEY_EXCANGE_Test: KeyExc RESULT fail\n");
		return; 
	}
	print("SM2 KeyChange OK !\n");
}
//----the other way for test SM2 key exchange ----
void Test_SM2_KeyExchange2(void)
{

	UINT8 ret;
	UINT8 RandA[64];
	UINT32 KLen = 32;
		
	SM2_PriKey sm2PriKeyA, sm2PriKeyB;
	SM2_PubKey sm2PubKeyA, sm2PubKeyB;	
	SM3Value *sm3SB, *sm3SA, *sm3S2;
 	SM3Value *ZA,*ZB;
	Ep_Point epA, epB;
	UINT8 KA[48], KB[48], PA[64], PB[64], RA[64], RB[64], dA[32], dB[32], rA[32], rB[32];

	UINT8 UserA[16] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	UINT8 UserB[16] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	


	print("Begin KeyExtChangeTest !\n");
	
	sm2PriKeyA.PrivateKey = (UINT8*)rA;	
	sm2PriKeyB.PrivateKey = (UINT8*)rB;

	sm2PubKeyA.X = (UINT8*)PA;
	sm2PubKeyA.Y = (UINT8*)(PA+32);
	sm2PubKeyB.X = (UINT8*)(PB);
	sm2PubKeyB.Y = (UINT8*)(PB+32);
	sm3SB = (SM3Value*)dA;
	sm3SA = (SM3Value*)dB;
	sm3S2 = (SM3Value*)RA;
	ZA = (SM3Value*)RB;
	ZB = (SM3Value*)(RB+32);

	ret=SM2_GenKey(&sm2PriKeyA,&sm2PubKeyA,256,0);
	if(ret!=RT_OK)
	{

		print("KeyExtChangeTest: Gen key fail\n");
 		return;
	}

	ret=SM2_GenKey(&sm2PriKeyB,&sm2PubKeyB,256,0);
	if(ret!=RT_OK)
	{

		print("KeyExtChangeTest: Gen key fail\n");
 		return;
	}
	
	//获取发起方和响应方用户信息的签名值ZA，ZB
	SM3_GetSignIDValue(ZA,&sm2PubKeyA,UserA,strlen((char*)UserA));//ENTLA
	SM3_GetSignIDValue(ZB,&sm2PubKeyB,UserB,strlen((char*)UserB));//  ENTLB
	
	
	//1、发起方调用函数生成协商参数 RandA、epA
	ret = SM2_KeyExc_Init((UINT32*)RandA,&epA);
	if(ret!=RT_OK)
	{

		print("KeyExtChangeTest: KeyExc Init fail\n");
 		return;
	}
	
	////2、响应方接收到发起方的参数，生成对应的密钥并返回协商参数
	ret = SM2_KeyExc_Response(&epA,&sm2PriKeyB,&sm2PubKeyA,&epB,sm3SB,sm3S2,ZA , ZB ,KB, KLen);//UserA,strlen((char*)UserA),UserB,strlen((char*)UserB)
	if(ret!=RT_OK)
	{

	 	print("KeyExtChangeTest: KeyExc Response fail\n");
 		return;
	}
	//    //3、发起方受到响应方的协商参数，生成会话密钥
	ret = SM2_KeyExc_Spons((UINT32*)RandA,&epA,&epB,sm3SB,&sm2PriKeyA,&sm2PubKeyB,sm3SA,ZA , ZB ,KA, KLen); // UserA,strlen((char*)UserA),UserB,strlen((char*)UserB)
	if(ret!=RT_OK)
	{
		print("KeyExtChangeTest: KeyExc sponse fail\n");
	 	return;
	}
	//比较判断，当 sm3S2.Buf=sm3SA.Buf时，协商密钥成功，EqualFlag = 1
	 if(memcmp(KA,KB,32))
    {
		print("The exchange key not equal!\n");
     	return;
    }
 
	print("SM2 KeyChange OK !\n");
}



