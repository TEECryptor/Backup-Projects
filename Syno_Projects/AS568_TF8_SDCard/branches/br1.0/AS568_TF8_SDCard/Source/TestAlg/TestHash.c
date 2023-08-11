#include "SY03.h"  

//----test MD5 Hash Alg ----
void TestMD5(void)
{
   UINT8 pBuf[256],Result[16];
   UINT16 i;
   MD5_CONTEXT ctx;

   for( i=0;i<256;i++)	
      pBuf[i]=i+1;
   
   MD5_Init(&ctx); 
   MD5_Update(&ctx,pBuf,256);
   MD5_Final(&ctx,Result);
    
}
//----test SHA1 Hash Alg ----
void TestSHA1(void)
{
   UINT8 pBuf[256],Result[20];
   UINT16 i;
   SHA1_CONTEXT ctx;

   for( i=0;i<256;i++)	
      pBuf[i]=i+1;
   
   SHA1_Init(&ctx); 
   SHA1_Update(&ctx,pBuf,256);
   SHA1_Final(&ctx,Result);
    
} 
//----test SHA256 Hash Alg ----
void TestSHA256(void)
{
   UINT8 pBuf[256],Result[32];
   UINT16 i;
   SHA256_CONTEXT ctx;

   for(i=0;i<256;i++)	
      pBuf[i]=i+1;
   
   SHA256_Init(&ctx); 
   SHA256_Update(&ctx,pBuf,256);
   SHA256_Final(&ctx,Result);
    
}

