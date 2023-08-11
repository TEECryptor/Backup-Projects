#include "SY03.h"


// ---Test true random data---
void TestTrueRand(void)
{
	UINT8 pBuf[32];
	UINT8 i;

	Trng_Init();
	
	for(i=0;i<10;i++)
	{
		Trng_GenRandom(pBuf,32); 
	}

	print("trng ok\n");	 	
}

 
