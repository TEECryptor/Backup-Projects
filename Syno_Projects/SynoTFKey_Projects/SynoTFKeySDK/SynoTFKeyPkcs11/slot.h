/* -*- c -*- */
/*
 * This file is part of GPKCS11. 
 * (c) 1999,2000 TC TrustCenter for Security in DataNetworks GmbH 
 *
 * GPKCS11 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *  
 * GPKCS11 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with GPKCS11; see the file COPYING.  If not, write to the Free
 * Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111, USA.  
 */
/*
 * RCSID:       $Id: slot.h,v 1.6 2000/06/05 11:43:43 lbe Exp $
 * Source:      $Source: /usr/cvsroot/pkcs11/libgpkcs11/slot.h,v $
 * Last Delta:  $Date: 2000/06/05 11:43:43 $ $Revision: 1.6 $ $Author: lbe $
 * State:	$State: Exp $ $Locker:  $
 * NAME:	slot.h
 * SYNOPSIS:	-
 * DESCRIPTION: -
 * FILES:	-
 * SEE/ALSO:	-
 * AUTHOR:	lbe
 * BUGS: *	-
 * HISTORY:	$Log: slot.h,v $
 * HISTORY:	Revision 1.6  2000/06/05 11:43:43  lbe
 * HISTORY:	tcsc token breakup, return pSlotCount in SlotList, code for event handling deactivated
 * HISTORY:	
 * HISTORY:	Revision 1.5  2000/05/16 09:54:14  lbe
 * HISTORY:	checkin to test bonsai
 * HISTORY:	
 * HISTORY:	Revision 1.4  2000/05/12 13:13:15  lbe
 * HISTORY:	zwischen durchmal B-)
 * HISTORY:	
 * HISTORY:	Revision 1.3  2000/01/31 18:09:03  lbe
 * HISTORY:	lockdown prior to win_gdbm change
 * HISTORY:	
 * HISTORY:	Revision 1.2  1999/07/20 17:40:02  lbe
 * HISTORY:	fix bug in gdbm Makefile: there is not allways an 'install' around
 * HISTORY:	
 * HISTORY:	Revision 1.1  1999/06/16 09:46:11  lbe
 * HISTORY:	reorder files
 * HISTORY:	
 * HISTORY:	Revision 1.3  1999/06/04 14:58:35  lbe
 * HISTORY:	change to libtool/automake complete (except for __umoddi prob)
 * HISTORY:	
 * HISTORY:	Revision 1.2  1999/01/19 12:19:47  lbe
 * HISTORY:	first release lockdown
 * HISTORY:
 * HISTORY:	Revision 1.1  1998/10/12 10:08:45  lbe
 * HISTORY:	clampdown
 * HISTORY:
 */

#ifndef _SLOT_H
#define _SLOT_H 1

/** lookup token data in hash table.
*/
CK_DECLARE_FUNCTION(CK_RV, CI_GetTokenData)(
  CK_ULONG ulSlotID,
  CK_I_TOKEN_DATA_PTR CK_PTR ppTokenData
);

/** lookup slot data in hash table.
*/
CK_DECLARE_FUNCTION(CK_RV, CI_GetSlotData)(
  CK_ULONG ulSlotID,
  CK_I_SLOT_DATA_PTR CK_PTR ppSlotData
);

/** removal of token from library.
 *
 * Warning: this simply drops the token from the slot.
 * All types of clean up have to be finished by now
 */
CK_DECLARE_FUNCTION(CK_RV, CI_RemoveToken)(
 CK_ULONG slotID
);


/** structure with information about an event.
*/
typedef struct _CK_I_EVENT_INFO CK_I_EVENT_INFO; 
typedef CK_I_EVENT_INFO CK_PTR CK_I_EVENT_INFO_PTR; 

typedef CK_RV (*CK_I_EventActionCallback)(CK_SLOT_ID SLOT, CK_VOID_PTR user_data);

typedef struct _CK_I_EVENT_SLOT_INFO CK_I_EVENT_SLOT_INFO;
typedef CK_I_EVENT_SLOT_INFO CK_PTR CK_I_EVENT_SLOT_INFO_PTR; 

struct _CK_I_EVENT_INFO
{
CK_I_EVENT_INFO_PTR next; /* next event of the list */
CK_I_EVENT_INFO_PTR slot_next; /* next event on this slot */
CK_I_EVENT_SLOT_INFO_PTR slot_info;
CK_I_EventActionCallback action;
CK_VOID_PTR user_data;
};


struct _CK_I_EVENT_SLOT_INFO
{
CK_SLOT_ID slot;
CK_I_EVENT_INFO_PTR head; /* head of the list of events for this slot */
CK_I_EVENT_INFO_PTR tail; /* tail of the list of events for this slot */
CK_I_EVENT_SLOT_INFO_PTR next; /* next event list */
};

/** Initialize the event handling.
*/
CK_DECLARE_FUNCTION(CK_RV, CI_InitEventHandler)(
 void
);

/** Finalize the event handling.
*/
CK_DECLARE_FUNCTION(CK_RV, CI_FinalizeEventHandler)(
 void
);

/** Register a sink for a slot event.
 * when this function is called the first time for a given slot, the 
 * watching of events for that slot is activated and C_WaitForSlot Event 
 * will not return with a CKR_FUNCTION_NOT_SUPPORTED.
 *
 * @param slot      the slot to which this event sink is to be asigned. 
 *                  If slot is set to (-1)L, the event sink is accepted, 
 *                  but never called when an slot specific event is 
 *                  triggered.
 * @param action    callback function that will be called when the event
 *                  is triggered.
 * @param user_data pointer to arbitrary user data that will be set as the
 *                  argument to the action function, shout the raising of 
 *                  the event not set its own value.
 * @param pHandle   adress of the memory that recieves the handle that this
 *                  event is registered under.
 * @return          CKR_OK if the sink is registered successfull, 
 *                  CKR_HOST_MEMORY when allocating memory failed, 
 *                  CKR_GENERAL_ERROR in all other error conditions.
 */
CK_DECLARE_FUNCTION(CK_RV, CI_RegisterEventSink)(
 CK_SLOT_ID slot, 
 CK_I_EventActionCallback action,  
 CK_VOID_PTR user_data, 
 CK_I_EVENT_INFO_PTR CK_PTR pEvent
);

/** Remove an event sink from the event handler.
*/
CK_DECLARE_FUNCTION(CK_RV, CI_RemoveEventSink)(
 CK_I_EVENT_INFO_PTR event
);

/** Send an event to all event sinks registered for a port.
 * @param slot        Slot that all sinks are triggered for.
 * @param user_data   if non-NULL, this value will be set as argument 
 *                    in the action callbacks instead of the default.
 * @param event_label if non-NULL the string will be written to he log.
 * @return            CKR_OK if the sink is registered successfull, 
 *                    CKR_HOST_MEMORY when allocating memory failed, 
 *                    CKR_GENERAL_ERROR in all other error conditions.
 */
CK_DECLARE_FUNCTION(CK_RV, CI_SendSlotEvent)(
 CK_SLOT_ID slot, 
 CK_VOID_PTR user_data, 
 CK_CHAR_PTR event_label
);

/** Send an event to a single event sink.
 * @param event       Event handle as received from CI_RegisterEventSink.
 * @param user_data   if non-NULL, this value will be set as argument 
 *                    in the action callbacks instead of the default.
 * @param event_label if non-NULL the string will be written to he log.
 * @return            CKR_OK if the sink is registered successfull, 
 *                    CKR_HOST_MEMORY when allocating memory failed, 
 *                    CKR_GENERAL_ERROR in all other error conditions.
*/
CK_DECLARE_FUNCTION(CK_RV, CI_TriggerEvent)(
 CK_I_EVENT_INFO_PTR event, 
 CK_VOID_PTR user_data, 
 CK_CHAR_PTR event_label
);

#endif /* _SLOT_H */

