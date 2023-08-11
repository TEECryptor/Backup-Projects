/***************************************************************************************
* File name    :	Trng.h
* Function     :	The header of Trng.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _TRNG_H_
#define _TRNG_H_


/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define TRNG_RAND_RESEED (1UL<<31)			//W1-Initiate reseed
#define TRNG_NONCE_RESEED (1UL<<30)			//W1-Initiate nonce reseed;
#define TRNG_NONCE_RESEED_LD    (1UL<<29)	//Load TRNG_DATA[0-3] registers into portion of nonce selected by the NONCE_SEED_SELECT bit
#define TRNG_NONCE_RESEED_SELECT (1UL<<28)	//W1-select 127 bit upper portion of nonce to reseed

#define TRNG_GET_NEW	 		(1UL << 0)
#define TRNG_BUSY				(1UL <<0)

#define TRNG_RAND_RESEED_MODE	1
#define TRNG_NONCE_RESEED_MODE	2
#define TRNG_SIZE_BYTES 		16
#define TRNG_SIZE_WORDS 		4 

#define TRNG_RDY_IRQ   			(1L<<27)
#define TRNG_CLEAR_RDY_IRQ_STAT  RngRegs.STATUS.all=0x08000000
#define TRNG_READ_RDY_IRQ_STAT  RngRegs.STATUS.all
#define LOOP_WAIT      80000000

/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 Trng_Init(void);
UINT8 Trng_GenRandom (UINT8 * pRandBuf, UINT16 nSize);
UINT8 Prng_GenRandom(UINT8 *pRandBuf,UINT16 nSize);

#endif
