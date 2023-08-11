#include "SY03.h" 

extern void TestSM1ECB(void);
extern void TestSM1CBC(void);
   
extern void TestSMS4ECB(void);
extern void TestSMS4CBC(void);
 
extern void TestAes128ECB(void);
extern void TestAes192ECB(void);
extern void TestAes256ECB(void);
	
extern void TestAes128CBC(void);
extern void TestAes192CBC(void);
extern void TestAes256CBC(void);
 
extern void TestDesCBC(void);
extern void TestDesECB(void);    
    
extern void Test3DesCBC(void);
extern void Test3DesECB(void);
 
extern void TestTrueRand(void);
extern void TestRSA1024(void);
extern void TestRSA2048(void);
 
extern void TestMD5(void);
extern void TestSHA1(void);
 
extern void TestSHA256(void);
extern void TestSM3(void);  
 
extern void TestSM2Sign(void);
extern void TestSM2Encry(void);      
extern void TestECDSA(void);  

extern void TestSSF33ECB(void);
extern void TestSSF33CBC(void);

extern void Test_SM2_KeyExchange(void);
extern void Test_SM2_KeyExchange2(void);

void TestAlg(void)
{
    //---ªÒ»°À„∑®ø‚∞Ê±æ---
	INT8 pVer[16];
	AlgLib_GetVersion(pVer,16);
	print("%s",pVer);
	//--≤‚ ‘SSF33---
	TestSSF33ECB();
	TestSSF33CBC();
  	//--≤‚ ‘SM1----    
    TestSM1ECB();
	TestSM1CBC();
  
	//--≤‚ ‘SMS4----    
    TestSMS4ECB();
	TestSMS4CBC();
	//--≤‚ ‘AES---- 
 	TestAes128ECB();
	TestAes192ECB();
	TestAes256ECB();
	
	TestAes128CBC();
	TestAes192CBC();
	TestAes256CBC();
 
	//--≤‚ ‘DES---- 
	TestDesCBC();
	TestDesECB();    
    
	Test3DesCBC();
	Test3DesECB();
    
    //--≤‚ ‘’ÊÀÊª˙ ˝---- 
	TestTrueRand();
 
    //--≤‚ ‘RSA---- 
	TestRSA1024();
	TestRSA2048();
	 
	//---≤‚ ‘HASH---
    TestMD5();
    TestSHA1();
 
    TestSHA256();
    TestSM3();   
   
    //---≤‚ ‘SM2---- 
     TestSM2Sign();
     TestSM2Encry();  
	 
	 Test_SM2_KeyExchange();
	 Test_SM2_KeyExchange2();   
     
	//---≤‚ ‘ECC----
     TestECDSA();  
    
	//--≤‚ ‘Ω· ¯---- 
}
