/***************************************************************************************
* File name    :	UsbProtocol.h
* Function     :	The header of UsbProtocol.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _USBPROTOCOL_H_
#define _USBPROTOCOL_H_

#define	USB_SUSPENDED	(0x80)

/* Interrupt register bit masks */ 
#define M_INTR_SUSPEND 0x01 
#define M_INTR_RESUME 0x02 
#define M_INTR_RESET 0x04 
#define M_INTR_EP0 0x0001 
#define M_INTR_EP1 0x0002
#define M_INTR_EP2 0x0004
#define M_INTR_EP3 0x0008
#define M_INTR_EP4 0x0010
/*************************************************************************************
 * System Fuctions Declaration														 *
 ************************************************************************************/
void USB_DoWithPackage(void);
void USB_BulkPoll(void);

#endif
