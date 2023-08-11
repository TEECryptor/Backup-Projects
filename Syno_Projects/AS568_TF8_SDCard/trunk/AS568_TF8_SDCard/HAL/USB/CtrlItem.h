/***************************************************************************************
* File name    :	CtrlItem.h
* Function     :	The header of CtrlItem.c
* Author       : 	Howard
* Date         :	2010/09/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _CTRLITEM_H_
#define _CTRLITEM_H_

/*************************************************************************************
 * System Fuctions Declaration														 *
 ************************************************************************************/
extern	void Chap9_GetStatus(SETUPPACK* SetupPack);
extern	void Chap9_SetFeature(SETUPPACK* SetupPack);
extern	void Chap9_ClearFeature(SETUPPACK* SetupPack);
extern	void Chap9_GetConfig(SETUPPACK* SetupPack);
extern	void Chap9_GetInterface(SETUPPACK* SetupPack);
extern	void Chap9_SetInterface(SETUPPACK* SetupPack);

extern	void Chap9_SetAddress( SETUPPACK* SetupPack);
extern	void Chap9_SetConfiguration( SETUPPACK* SetupPack);
extern	void Chap9_GetDescriptor( SETUPPACK* SetupPack) ;
extern	void Chap9_SendACK (SETUPPACK* SetupPack);

extern	void Class_SendMaxLun( SETUPPACK* SetupPack);

#endif
