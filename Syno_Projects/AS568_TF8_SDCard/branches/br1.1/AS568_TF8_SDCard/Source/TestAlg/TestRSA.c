#include "SY03.h"

//----test RSA 1024 bit mode ----
//--以下变量在Command.c中测试RSA密钥对生成的时候已经定义了。 
extern UINT8   n[256],e[4],p[128], q[128], dp[128],dq[128],qinv[128]; 

UINT32 TestRSA1024(void)
{ 	
	UINT8 pOutData[128];	
	UINT8 pInData[128];		
	UINT32 nOutDataLen,nInDataLen; 	
	UINT32 DecLen=0; 	 
	R_RSA_PROTO_KEY ProtoKey;   
    R_RSA_PRIVATE_KEY PrivateKey; 
	R_RSA_PUBLIC_KEY  PublicKey;
	UINT32 nFlag=0;		
	UINT32 i;
 
	PrivateKey.modulus = n;
	PrivateKey.publicExponent = e;
	PrivateKey.exponent = NULL;//d;
	PrivateKey.prime[0] = p;
	PrivateKey.prime[1] = q;
	PrivateKey.primeExponent[0] = dp; 
	PrivateKey.primeExponent[1] = dq;
	PrivateKey.coefficient = qinv; 	

	PublicKey.modulus = n;
	PublicKey.exponent = e;
	PublicKey.bits=1024;

	ProtoKey.bits = 1024;	
	ProtoKey.PublicExponent = 65537; 
 
	for(i=0;i<128;i++)		
		pInData[i]=i+1;		 
	 
    print("Test RSA 1024\n");

	nFlag= RSA_GenKeyPair(ProtoKey,&PrivateKey);          
  
	if(nFlag!=RT_OK)//failed
	{
	     print("Gen RSA key Fail\n");
		 return nFlag;
	} 
	
    print("Gen RSA key OK\n");
	nInDataLen=128;      
     	
    nFlag=RSA_PubKeyOpt(pOutData,&nOutDataLen,pInData,nInDataLen,&PublicKey); 
	 
	if(nFlag!=0) 
	{
		 print("RSA PubOpt fail\n");
	     return nFlag; //failed
    }

    print("RSA PubOpt OK\n");     

    nFlag=RSA_PriKeyOpt(pInData,&DecLen,pOutData,nOutDataLen,&PrivateKey);  
    
    for(i=0;i<128;i++)	
    {    	  	
		if(pInData[i]!=i+1)
		{
			print("Data decry error\n");
			break;
		}		
    }
	 
    if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PriOpt OK\n");  
    
	//RSA Private Key Encry;
	nFlag=RSA_PriKeyOpt(pOutData,&nOutDataLen,pInData,nInDataLen,&PrivateKey);  
	 
	if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PriOpt OK\n"); 
 
	//RSA Public Key Decry;
	nFlag=RSA_PubKeyOpt(pInData,&DecLen,pOutData,nOutDataLen,&PublicKey); 	 

	if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }

     for(i=0;i<128;i++)	
    {    	  	
		if(pInData[i]!=i+1)
		{
			print("Data decry error\n");
			break;
		}		
    }

    print("RSA PriOpt OK\n");   

	nInDataLen=32;     
	//RSA Public Key Encry;
	
    nFlag=RSA_PublicEncrypt(pOutData,&nOutDataLen,pInData,nInDataLen,&PublicKey); 
	 
	if(nFlag!=0) return nFlag; //failed
   
	//RSA Private Key Decry;	 
    nFlag=RSA_PrivateDecrypt(pInData,&DecLen,pOutData,nOutDataLen,&PrivateKey);  
	 
    if(nFlag!=0) return nFlag;   
    
	//RSA Private Key Encry;
	nFlag=RSA_PrivateEncrypt(pOutData,&nOutDataLen,pInData,nInDataLen,&PrivateKey);  
	 
	if(nFlag!=0) return nFlag;
 
	//RSA Public Key Decry;
	nFlag=RSA_PublicDecrypt(pInData,&DecLen,pOutData,nOutDataLen,&PublicKey); 	 

	if(nFlag!=0) return nFlag;
	 
     return 0;
}

//----test RSA 2048 bit mode ---- 
		
UINT32 TestRSA2048(void)
{
   
    R_RSA_PROTO_KEY ProtoKey;
    R_RSA_PRIVATE_KEY PrivateKey;
	R_RSA_PUBLIC_KEY  PublicKey;
	UINT8 pOutData[256];	
	UINT8 pInData[256];	
	UINT32 nOutDataLen,nInDataLen; 		
	UINT32 DecLen=0; 	
	UINT32 nFlag=0;		
	UINT32 i;    
		

	PrivateKey.modulus = n;
	PrivateKey.publicExponent = e;
	PrivateKey.exponent = NULL;//d;
	PrivateKey.prime[0] = p;
	PrivateKey.prime[1] = q;
	PrivateKey.primeExponent[0] = dp; 
	PrivateKey.primeExponent[1] = dq;
	PrivateKey.coefficient = qinv; 	

	ProtoKey.bits = 2048;	
	ProtoKey.PublicExponent = 65537; 
	
	PublicKey.modulus = n;
	PublicKey.exponent = e;
	PublicKey.bits=2048;	 
	
	for(i=0;i<256;i++)		
		pInData[i]=i; 

	print("Test RSA 2048\n");

	 nFlag= RSA_GenKeyPair(ProtoKey,&PrivateKey);
	 
    if(nFlag!=RT_OK)//failed
	{
	   print("Gen RSA key Fail\n");
		return nFlag;
	} 
	
    print("Gen RSA key OK\n"); 	

	nInDataLen=256;      
     	
    nFlag=RSA_PubKeyOpt(pOutData,&nOutDataLen,pInData,nInDataLen,(R_RSA_PUBLIC_KEY*)(void*)&PrivateKey); 
	if(nFlag!=0) 
	{
		 print("RSA PubOpt fail\n");
	     return nFlag; //failed
    }
   print("RSA PubOpt OK\n");      
   
	//RSA Private Key Decry; 
    nFlag=RSA_PriKeyOpt(pInData,&DecLen,pOutData,nOutDataLen,&PrivateKey);  
	 
    if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PriOpt OK\n");  
     
     for(i=0;i<256;i++)	
    {    	  	
		if(pInData[i]!=i)
		{
		 	print("Data decry error\n");
			return RT_FAIL;
		}		
    }
	//RSA Private Key Encry;
	nFlag=RSA_PriKeyOpt(pOutData,&nOutDataLen,pInData,nInDataLen,&PrivateKey);  
	 
	if(nFlag!=0) 
	{
		 print("RSA PubOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PubOpt OK\n");
   
	//RSA Public Key Decry;
	nFlag=RSA_PubKeyOpt(pInData,&DecLen,pOutData,nOutDataLen,(R_RSA_PUBLIC_KEY*)(void*)&PrivateKey); 	 

	if(nFlag!=0) 
	{
		 print("RSA PriOpt fail\n");
	     return nFlag; //failed
    }
    print("RSA PriOpt OK\n");  
   
	 for(i=0;i<256;i++)	
    {    	  	
		if(pInData[i]!=i)
		{
			print("Data decry error\n");
			return RT_FAIL;
		}		
    }

    nInDataLen=32;     
	//RSA Public Key Encry;
	
    nFlag=RSA_PublicEncrypt(pOutData,&nOutDataLen,pInData,nInDataLen,&PublicKey); 
	 
	if(nFlag!=0) return nFlag; //failed
   
	//RSA Private Key Decry;	 
    nFlag=RSA_PrivateDecrypt(pInData,&DecLen,pOutData,nOutDataLen,&PrivateKey);  
	 
    if(nFlag!=0) return nFlag;   
    
	//RSA Private Key Encry;
	nFlag=RSA_PrivateEncrypt(pOutData,&nOutDataLen,pInData,nInDataLen,&PrivateKey);  
	 
	if(nFlag!=0) return nFlag;
 
	//RSA Public Key Decry;
	nFlag=RSA_PublicDecrypt(pInData,&DecLen,pOutData,nOutDataLen,&PublicKey); 	 

	if(nFlag!=0) return nFlag;

     return 0;
}
