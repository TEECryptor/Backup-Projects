#include "SY03.h"  

void TestSSF33ECB(void)
{
	UINT8 cKey[16]={0x67,0xbe,0x03,0x7c,0x41,0x96,0x6d,0xdb,0x8c,0x36,0x27,0x48,0x5a,0x05,0x93,0xa5};
	UINT8 cData[16]={0xa9,0x37,0x07,0x49,0xfc,0x06,0xaf,0xe6,0x4e,0x30,0x68,0x01,0xd2,0x31,0xb3,0xac};

	UINT8 cOut[16],DecData[16];
	UINT8 OKRes[16]={0x9a,0xb7,0x1c,0xcc,0x22,0x7e,0x9e,0x58,0x7a,0xa0,0xe6,0xcf,0x49,0x08,0x5d,0x1f};

  	SSF33_Init(cKey);


	SSF33_Run(SSF33_ENCRYPT,SSF33_ECB,cData,cOut,16,NULL);

	if(memcmp(cOut,OKRes,16)!=0)
	{
	 	 print("SSF33 ecb encry fail!\n");
	 	 return;
	}

	SSF33_Run(SSF33_DECRYPT,SSF33_ECB,cOut,DecData,16,NULL);
 	
	if(memcmp(cData,DecData,16)!=0)
	{
	 	 print("SSF33 ecb encry fail!\n");
	 	 return;
	}
	 print("SSF33 ecb ok!\n"); 

}


void TestSSF33CBC(void)
{
	UINT8 cKey[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	UINT8 cData[16]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};

	UINT8 cOut[16],DecData[16],cSrcData[16];
	UINT8 OKRes[16]={0x76,0xA3,0xFB,0x07,0xF6,0xA4,0x18,0xA5,0x80,0xF4,0xD8,0x71,0x26,0xDE,0xAC,0xA6};
	UINT8 pIV[16]={0x10};
	UINT8 pIV2[16]={0x10};
	

	SSF33_Init(cKey);
	memcpy(cSrcData,cData,16);
	SSF33_Run(SSF33_ENCRYPT,SSF33_CBC,cData,cOut,16,pIV);
	
	if(memcmp(cOut,OKRes,16)!=0)
	{
	 	 print("SSF33 cbc encry fail!\n");
	 	 return;
	}

	SSF33_Run(SSF33_DECRYPT,SSF33_CBC,cOut,DecData,16,pIV2);

	if(memcmp(cSrcData,DecData,16)!=0)
	{
	 	 print("SSF33 cbc encry fail!\n");
	 	 return;
	}
	 print("SSF33 cbc ok!\n"); 
}
