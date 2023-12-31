#include "SY03.h"

//----test SMS4 ECB ---- 
void TestSMS4ECB(void)
{
	UINT8 cKey[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	UINT8 cData[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};

	UINT8 cOut[16];//68 1e df 34 d2 06 96 5e 86 b3 e9 4f 53 6e 42 46

	SMS4_Init(cKey);
	SMS4_Run(SMS4_ENCRYPT,SMS4_ECB,cData,cOut,16,NULL);
	SMS4_Run(SMS4_DECRYPT,SMS4_ECB,cOut,cData,16,NULL); 

}

//----test SMS4 CBC ---- 
void TestSMS4CBC(void)
{
	UINT8 cKey[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	UINT8 cData[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};

	UINT8 cOut[16];
	UINT8 pIV[16]={0x10};
	UINT8 pIV2[16]={0x10};

	SMS4_Init(cKey);
	SMS4_Run(SMS4_ENCRYPT,SMS4_CBC,cData,cOut,16,pIV);
	SMS4_Run(SMS4_DECRYPT,SMS4_CBC,cOut,cData,16,pIV2);
   
}
