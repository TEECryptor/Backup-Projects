/***************************************************************************
* File name    :	SY03_RNG.h
* Function     :	Header of SY03 True/Pseudo Random Number Generate
* 					Registers
* Author       : 	synochip
* Date         :	2009/03/31
* Version      :    v0.1
* Description  :    the RNG register header for SY03
* ModifyRecord :
****************************************************************************/
#ifndef SY03_RNG_H_
#define SY03_RNG_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/**************************************************************************
* Global Type Definition
***************************************************************************/
// TRNG Control Register Bit Definition
struct TRNG_CTRL_BITS	{		//	bits	description
	  UINT32 GenNewRandom:1; 	//	0 		Generate new random
	  UINT32 Rsvd:27;			//	27:1 	reserved
	  UINT32 NonceLfsrSelect:1;	//	28 		Select Nonce
	  UINT32 NonceReseedLD:1;	//	29 		load nonce seed
	  UINT32 NonceReseed:1;		//	30 		nonce reseed
	  UINT32 RandReseed:1;		//	31 		random seed
};
union TRNG_CTRL_REG{
	UINT32 all;
	struct TRNG_CTRL_BITS bit;
};

// TRNG Status Register Bit Definition
struct TRNG_STATUS_BITS	{	//	bits		description
	  UINT32 Rsvd1:27;		//	26:0 		reserved
	  UINT32 TrngRdyIrq:1;	//	27 			trng data is ready
	  UINT32 Rsvd2:4;		//	31:28 		reserved
};
union TRNG_STATUS_REG{
	  UINT32 all;
	  struct TRNG_STATUS_BITS bit;
};

// TRNG Interrupt Enable Register Bit Definition
struct TRNG_INTEN_BITS{		//	bits		description
	  UINT32 Rsvd1:27;		//	26:0 		reserved
	  UINT32 TrngIrqEn:1;	//	27 			enable trng irq
	  UINT32 Rsvd2:4;		//	31:28 		reserved
};
union TRNG_INTEN_REG{
	  UINT32 all;
	  struct TRNG_INTEN_BITS bit;
};

//-----all TRNG registers--
struct	RNG_REGS{
	union TRNG_CTRL_REG         CTRL; 	// Control register 0
	union TRNG_STATUS_REG       STATUS;	// Status register  4
	union TRNG_INTEN_REG        INTEN;	// Int enable register 8
	UINT32 POWERDOWN;						// reserved   c
	UINT32 RAND0;						// 128bit random bit[31:0] 10
	UINT32 RAND1;						// 128bit random bit[63:32] 14
	UINT32 RAND2;						// 128bit random bit[95:64] 18
	UINT32 RAND3;						// 128bit random bit[127:96] 1c
	UINT32 PRCL;						// Low 16 bit for prng 20
	UINT32 PRCH;						// High 16 bit for prng 24
	UINT32 PRSD;						// seed of prng 28
};

/**************************************************************************
* Global Variable Declaration
***************************************************************************/
extern	volatile struct RNG_REGS RngRegs;

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
 

#endif /*SY03_RNG_H_*/
