/******************************************************************************/
/* Serial.c: SY0301 Low Level Serial Routines            */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2011 Keil - An ARM Company. All rights reserved.             */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
#include <stdint.h>
#include "SY03_Device.h"


//#define	SIMULATE_FOR_IC

// define print function baud rate
#define	PRT_UART		Uart0Regs	
//#define	PRT_BAUD_RATE	57600
#define	PRT_BAUD_RATE	115200
#define	PRT_UART_PORT_GPA4	0
#define	PRT_UART_PORT_GPB2	1
#define	PRT_UART_PORT_SEL	PRT_UART_PORT_GPA4

extern uint32_t	SystemFrequency;
volatile unsigned char SerInit = 0;

/*----------------------------------------------------------------------------
  Initialize UART, Baudrate
 *----------------------------------------------------------------------------*/
void SER_init(void)
{
	ScmRegs.MODCLKCTRL |= 1<<ID_UART0;
	ScmRegs.MODRSTCTRL |= 1<<ID_UART0;
	#if(PRT_UART_PORT_SEL == PRT_UART_PORT_GPA4)
	GpioRegs.GPAUSE0.bit.GPA4 = MUX_UART;
	#else
	GpioRegs.GPBUSE0.bit.GPB2 = MUX_UART;
	#endif
	PRT_UART.GCTL.all = 0;
	PRT_UART.INTEN.all = 0;
	PRT_UART.INTCLR.all = 0xFFFF;
	PRT_UART.CCTL.all = 0x01c0; 	// 8bit, 2 stop, no parity
	#ifdef	SIMULATE_FOR_IC
		PRT_UART.SPBRG = 1; 	// baud rate
	#else
		PRT_UART.SPBREG =  SystemFrequency/(16 * PRT_BAUD_RATE);
	#endif
	PRT_UART.GCTL.bit.TxEn = 1;

	SerInit = 1;
  	return;
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int sendchar(int c)
{
	if(SerInit==0)
	{
		SER_init();
	}
	while(PRT_UART.CSTAT.bit.TxFull)	// wait FIFO is not full
	{
		
	};
	PRT_UART.TXREG = c;
	while(!PRT_UART.CSTAT.bit.TxEpt)	// wait FIFO is empty
	{
		
	};
  	return (c);
}
/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int getkey (void) 
{
	int c;
	if(SerInit==0)
	{
		SER_init();
	}
	while(!PRT_UART.CSTAT.bit.RxAvl);	// wait for receiving data valid
	c = PRT_UART.RXREG;
   	return c;
}

/*----------------------------------------------------------------------------
  Exit Simulate for IC Design
 *----------------------------------------------------------------------------*/
void ExitSimulate(void)
{
	#ifdef	SIMULATE_FOR_IC
	unsigned int i = 1000;
	while(i--);
	ScmRegs.MODCLKCTRL &= ~(1<<ID_SCM);	
	#endif	
}




