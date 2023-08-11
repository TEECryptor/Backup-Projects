/***************************************************************************
* File name    :	Trng.c
* Function     :	Trng hardware
* Author       : 	howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :     
* ModifyRecord :
****************************************************************************/
#include  "SY03.h" 

UINT8 Trng_WaitDone (void);

/***************************************************************************
* Subroutine:	ReadRandData
* Function:		Read trng data from register
* Input:		None
* Output:		None;
* Description:	 
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/ 
void ReadRandData(UINT32* pBuf)
{   
   *pBuf=RngRegs.RAND0;
  
   *(pBuf+1)=RngRegs.RAND1;
    
   *(pBuf+2)=RngRegs.RAND2;
    
   *(pBuf+3)=RngRegs.RAND3;
    
} 
 
/***************************************************************************
* Subroutine:	Trng_Init
* Function:		init trng context
* Input:		None
* Output:		engine status;
* Description:	 
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/ 
UINT8 Trng_Init (void)
{  
	ScmRegs.MODCLKCTRL |= 1<<ID_TRNG;
	ScmRegs.MODRSTCTRL |= 1<<ID_TRNG; 
	
   RngRegs.POWERDOWN =1;
	
	RngRegs.CTRL.all = TRNG_RAND_RESEED ;
	
	RngRegs.CTRL.all = RngRegs.CTRL.all & (~TRNG_RAND_RESEED) ;
	RngRegs.INTEN.all = 0;  
	
	return Trng_WaitDone();   
}
 
/***************************************************************************
* Subroutine:	Trng_WaitDone
* Function:		internal use only, used after set mode and go 
* Input:		None
* Output:		engine status;
* Description:	 
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/ 
UINT8 Trng_WaitDone (void)
{
	UINT32 wait;    
	
	wait =  LOOP_WAIT;
  
    while ((wait > 0) && (!((TRNG_READ_RDY_IRQ_STAT)  & TRNG_RDY_IRQ ))) {
     // --wait;
    }
	if (!wait)
	return RT_FAIL;
    
    if((TRNG_READ_RDY_IRQ_STAT)  & TRNG_RDY_IRQ)
    TRNG_CLEAR_RDY_IRQ_STAT;
  
  return RT_OK;
}
 
 
/***************************************************************************
* Subroutine:	Trng_GenRandom
* Function:		get the random number after go
* Input:		*pRandBuf and it size, the size should be > 0, s/w support bytes access even if h/w word (4 bytes) access
                nSize-the size of random.
* Output:		None;
* Description:	 
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/ 
UINT8 Trng_GenRandom (UINT8 * pRandBuf, UINT16 nSize)
{
	UINT16 ret = RT_OK;
    UINT8 pBuf[16];
    UINT16 i;
    UINT32 lVal;

    if ((!pRandBuf) || (nSize <= 0)) {
    ret = RT_PARAM_ERR;
    } else {
    for (; nSize;) {
      i = nSize > 16 ? 16 : nSize;

      //go
      lVal=RngRegs.CTRL.all;     
      lVal |= TRNG_GET_NEW;

      RngRegs.CTRL.all=lVal;
      
      if (Trng_WaitDone () != RT_OK) {
        ret = RT_FAIL;
        break;
      }
      ReadRandData((UINT32*)pBuf);
      memcpy (pRandBuf, pBuf, i);
      pRandBuf += i;
      nSize -= i;
    }
  }

  return ret;
}

UINT8 Prng_GenRandom(UINT8 *pRandBuf,UINT16 nSize)
{  
	int i;

	ScmRegs.MODCLKCTRL |= 1<<ID_TRNG;
	ScmRegs.MODRSTCTRL |= 1<<ID_TRNG; 
    
    for(  i=0;i<nSize;i++)
            pRandBuf[i]=RngRegs.PRCH;

	return RT_OK;
}
 
