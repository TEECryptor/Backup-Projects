#include "SY03.h" 
 
//----test sm3 ----
void TestSM3(void)
{
   UINT8 pBuf[256],Result[32];
   UINT16 i;
   SM3_CONTEXT ctx;
   for(i=0;i<256;i++)	
      pBuf[i]=i+1;
   
   SM3_Init(&ctx); 
   SM3_Update(&ctx,pBuf,256);
   SM3_Final(&ctx,Result);
 
}
