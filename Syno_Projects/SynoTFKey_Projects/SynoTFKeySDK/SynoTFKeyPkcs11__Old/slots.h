/***************************************************************************
 *	File name:slots.h
 *	Introduce:Object slots functions definition file
 *	Author:Syno common
 *	Date:2015/10/30
 *  Version:v1.0
 **************************************************************************/
#ifndef _SLOTS_H_
#define _SLOTS_H_

#include "token.h"

#define	MAX_SLOT_CNT	16

typedef struct tagSlotInfo
{
	CK_SLOT_ID	ulSlotID;
	CK_CHAR		csDevName[64];

}SlotInfo, *LPSLOTINFO;


extern CK_ULONG		g_ulSlotCount;
extern LPSLOTINFO	g_pSlotList;	 

CK_RV SLT_GetSlotInfo(CK_SLOT_ID ulSlotID, CK_SLOT_INFO_PTR pInfo);
CK_RV SLT_GetTokenInfo(CK_SLOT_ID ulSlotID, CK_TOKEN_INFO_PTR pInfo);
CK_RV SLT_GetMechanismList(CK_SLOT_ID slotID, CK_MECHANISM_TYPE_PTR pMechanismList, CK_ULONG_PTR pulCount);
CK_RV SLT_GetMechanismInfo(CK_SLOT_ID slotID, CK_MECHANISM_TYPE type, CK_MECHANISM_INFO_PTR pInfo);


#endif	//_SLOTS_H_