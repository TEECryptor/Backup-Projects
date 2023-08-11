;/*
; *-----------------------------------------------------------------------------
; * The confidential and proprietary information contained in this file may
; * only be used by a person authorised under and to the extent permitted
; * by a subsisting licensing agreement from ARM Limited.
; *
; *            (C) COPYRIGHT 2010-2011 ARM Limited.
; *                ALL RIGHTS RESERVED
; *
; * This entire notice must be reproduced on all copies of this file
; * and copies of this file may only be made by a person if such person is
; * permitted to do so under the terms of a subsisting license agreement
; * from ARM Limited.
; *
; *      SVN Information
; *
; *      Checked In          : $Date: 2011-03-17 11:25:01 +0000 (Thu, 17 Mar 2011) $
; *
; *      Revision            : $Revision: 164919 $
; *
; *      Release Information : BP200-r0p0-00rel0
; *-----------------------------------------------------------------------------
; */

;/*****************************************************************************/
;/* Startup.s: Startup file for ARM Cortex-M0 Device Family                   */
;/*****************************************************************************/


;// <h> Stack Configuration
;//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

;Stack_Size      EQU     0x00003000

;               AREA    STACK, NOINIT, READWRITE, ALIGN=3
;Stack_Mem       SPACE   Stack_Size
;__initial_sp

; modified for scatter file, 2013.04.07, coins
				AREA    STACK_TOP, NOINIT, READWRITE, ALIGN=3
__initial_sp


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;// </h>

;Heap_Size       EQU     0x00000000

;                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
;__heap_base
;Heap_Mem        SPACE   Heap_Size
;__heap_limit

; modified for scatter file, 2013.04.07, coins				
				AREA    HEAP_BOTTOM, NOINIT, READWRITE, ALIGN=3
__heap_base
			    AREA    HEAP_TOP, NOINIT, READWRITE, ALIGN=3
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler
				; Interrupt
                DCD     SCM_Handler           	;0 	SCM Handler
                DCD     GPIO_Handler           	;1 	GPIO Handler
                DCD     T0_Handler           	;2 	Timer 0 Handler
				DCD     SEA_Handler           	;3 	SEA Handler
				DCD     AEA_Handler        		;4 	AEA Handler
				DCD     SPI0_Handler            ;5 	SPI handler
				DCD     TRNG_Handler           	;6 	True Random Number Generater Handler
				DCD     USB_Handler           	;7 	USB Handler    
				DCD     T1_Handler           	;8 	Timer 1 Handler
                DCD     T2_Handler           	;9 	Timer 2 Handler
                DCD     SSI_Handler           	;10 SSI Handler
                DCD     UART0_Handler           ;11 UART handler
                DCD     I2C0_Handler        	;12 I2C Handler
                DCD     SWPS_Handler         	;13 SWP Slave handler
                DCD     SWPM_Handler          	;14 SWP Master handler
                DCD     ISO7816_Handler         ;15 ISO7816 Handler
                DCD     SCI_Handler            	;16 ISO7816 Master handler
                DCD     ACMP_Handler           	;17 Analog Compare Handler	
				DCD		VPWM_Handler  			;18 Voice PWM Handler
				DCD     SDSI_Handler          	;19 SD Bus Slave Handler
				DCD		RTC_Handler				;20	RTC Handler
				DCD		0						;21	Reserved
				DCD		0 						;22	Reserved
                DCD     0           			;23 Reserved
                DCD     0           			;24 Reserved
                DCD     0			  			;25 Reserved
                DCD     SDI_Handler           	;26 Reserved
                DCD     SPI1_Handler          	;27 Reserved
                DCD     0          				;28 Reserved
                DCD     0          				;29 Reserved
                DCD     0          				;30 Reserved
                DCD     0          				;31 Reserved
              
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     __heap_base          	;Reserved
				DCD     __heap_limit         	;Reserved
__Vectors_End

__Vectors_Size         EQU     __Vectors_End - __Vectors
                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                IMPORT  SystemInit

				IMPORT  SYS_GenSramKey
				; ro write SRAM encrypt key
				LDR     R0, =SYS_GenSramKey
                BLX     R0
				LDR		R1, SCM_BASE_ADDR
				LDR		R2, REG_PROT_VAL
				STR		R2, [R1, #0x5c]
				STR		R0, [R1, #0x50]
				LSRS	R0, R0, #8
				STR		R0, [R1, #0x54]
				MOVS	R2, #0
				STR		R2, [R1, #0x5c]

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

SCM_BASE_ADDR		DCD		0x40000000		; SCM
REG_PROT_VAL		DCD		0x32107654

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
               EXPORT  SysTick_Handler            [WEAK]
               B       .
               ENDP
Default_Handler PROC               
                EXPORT SCM_Handler             		[WEAK]
                EXPORT GPIO_Handler            		[WEAK]
                EXPORT T0_Handler              		[WEAK]
				EXPORT SEA_Handler            		[WEAK]
                EXPORT AEA_Handler        			[WEAK]
				EXPORT SPI0_Handler             	[WEAK]
				EXPORT TRNG_Handler            		[WEAK]
				EXPORT USB_Handler             		[WEAK]
                EXPORT T1_Handler              		[WEAK]
				EXPORT T2_Handler              		[WEAK]
                EXPORT SSI_Handler             		[WEAK]
                EXPORT UART0_Handler            	[WEAK]
                EXPORT I2C0_Handler        			[WEAK]      
                EXPORT SWPS_Handler            		[WEAK]
                EXPORT SWPM_Handler            		[WEAK]
				EXPORT ISO7816_Handler            	[WEAK]
                EXPORT SCI_Handler             		[WEAK]
				EXPORT ACMP_Handler            		[WEAK]
				EXPORT VPWM_Handler            		[WEAK]
                EXPORT SDSI_Handler            		[WEAK]   
				EXPORT RTC_Handler            		[WEAK]
				EXPORT SDI_Handler            		[WEAK]
				EXPORT SPI1_Handler            		[WEAK]
               
SCM_Handler             		
GPIO_Handler            	
T0_Handler              	
SEA_Handler            	
AEA_Handler        			
SPI0_Handler             	
TRNG_Handler            	
USB_Handler             	
T1_Handler              	
T2_Handler              	
SSI_Handler             	
UART0_Handler            
I2C0_Handler        		  
SWPS_Handler            	
SWPM_Handler            	
ISO7816_Handler            	
SCI_Handler             		
ACMP_Handler            	
VPWM_Handler            	
SDSI_Handler            	
RTC_Handler 
SDI_Handler 
SPI1_Handler     
               B       .
               ENDP

                ALIGN



; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                ;LDR     R0, =  Heap_Mem
                ;LDR     R1, =(Stack_Mem + Stack_Size)
                ;LDR     R2, = (Heap_Mem +  Heap_Size)
                ;LDR     R3, = Stack_Mem
                ;BX      LR
				; modified for scatter file, 2013.04.07, coins
				IMPORT |Image$$STACK$$Base|
				IMPORT |Image$$STACK$$ZI$$Limit|
				IMPORT |Image$$HEAP$$Base|
				IMPORT |Image$$HEAP$$ZI$$Limit|
				LDR     R0, = |Image$$HEAP$$Base|
				LDR     R1, = |Image$$STACK$$ZI$$Limit|
                LDR     R2, = |Image$$HEAP$$ZI$$Limit|
                LDR     R3, = |Image$$STACK$$Base|
                BX      LR

                ALIGN

                ENDIF
                END
