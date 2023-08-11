/****************************************************************************
* File name    :	system_SY03.c
* Function     :	the system initialization functions
* Author       : 	coins
* Date         :	2012/06/15
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "SY03.h"

//#define	ENABLE_SRAM_ENC	 	//if SRAM encrypt function is used, please open ENABLE_SRAM_ENC	
/**********************************************************************************
* Local Macro Definition
**********************************************************************************/


/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/


/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
//the default system clock frequency
#define	FREQ_DEF_SYSCLK		12000000UL
volatile uint32_t SystemFrequency = FREQ_DEF_SYSCLK;    /*!< System Clock Frequency (Core Clock)  */

UINT32 SYS_GenSramKey(void)
{
	unsigned int rand = 0;

#ifdef	ENABLE_SRAM_ENC
	Trng_Init();
	while(rand==0)
		Trng_GenRandom((unsigned char *)&rand, 4);
#endif

	return rand;			
}

/****************************************************************************
 * Subroutine:	SystemInit
 * Function:	to initialize the system according to the core/board
 * Input:		None
 * Output:		None
 * Description:	to initialize the system clock, power, reset, memory and so on 
 * Date:		2012.06.15
 * ModifyRecord:
 * *************************************************************************/
void SystemInit (void)
{
	memset((UINT8 *)0x20000000,0x00,ROM_SPACE_SIZE);
	*((UINT32 *)ROM_SYSFREQ_ADD) = FREQ_DEF_SYSCLK;
	*((UINT32 *)ROM_GCNT_MS_ADD) = 0xBB8;
	SCM_VectReMapTo(VT_MEM_FLASH);
	#if(USED_FOR_TF == 0)	
		RomFuncInit(0);//AS568 chip
	#else
		RomFuncInit(1);//TF Card
	#endif
	SCM_LoadHFRoscTrim();
}
