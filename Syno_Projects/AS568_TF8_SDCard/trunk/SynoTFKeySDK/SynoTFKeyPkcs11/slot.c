

static char RCSID[]="$Id: slot.c,v 1.14 2000/09/19 09:14:55 lbe Exp $";
const char* Version_slot_c(){return RCSID;}

#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "internal.h"
#include "error.h"
#include "objects.h"
#include "slot.h"
#include "token.h"


/* {{{ Data Structures */
/* the table containing the references to the slot data informations
 * is dynamically allocated in CI_RegisterToken, each time a slot registers
 * CK_I_SLOT_ANZ counts the number of slots loaded in the library
 * TODO : put the table and the counter in a private structure
 */
static CK_I_SLOT_DATA **CK_I_slot_info_table = NULL_PTR;
CK_ULONG CK_I_SLOT_ANZ = 0;

CK_RV C_GetSlotList(
	CK_BBOOL tokenPresent,
	CK_SLOT_ID_PTR pSlotList,
	CK_ULONG_PTR pulCount
	)
{
    CK_ULONG i,count = 0;
    CK_RV rv = CKR_OK;

    FUNC_BEGIN;

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}

	if (pulCount == NULL_PTR)
	{
        SYNO_LogE("pulCount is NULL!");
		return CKR_ARGUMENTS_BAD;
	}	
    if (tokenPresent)
	{
		SYNO_LogD("Listing only slots with present tokens");
	}
    else
	{
        SYNO_LogD("Listing all slots");
	}

    /* the slots are loaded dynamically at initialization
    * the slot_info_table records now contain NULL_PTR if the slot
    *  is not loaded
    * to test whether a token is present or not, use flag CKF_TOKEN_PRESENT
    */
    for (i = 0; i< CK_I_SLOT_ANZ; i++)
    {
        if (CK_I_slot_info_table[i] != NULL_PTR)
        {
            if (CK_I_slot_info_table[i]->slot_info == NULL_PTR)
            {
                rv = CKR_GENERAL_ERROR;
				SYNO_LogE("No slot_info set in slot_data! i = 0x%x", i);
            }

            if ((tokenPresent == FALSE)
                || (CK_I_slot_info_table[i]->slot_info->flags & CKF_TOKEN_PRESENT))
            {
                count++;
            }
        }
    }

    if (pSlotList == NULL_PTR) /* only query the number of slots */
    {
        *pulCount= count;
		SYNO_LogD("Return slot count:0x%x", count);

		FUNC_END;
        return CKR_OK;
    }

    /* Ok this is the real thing: return the list of slots */
    /* first check that the size of buffer is sufficient for the availiable slots */
    /* number of slot was computed in count ; comparing with pulCount */
    if (*pulCount < count)
    {
        rv = CKR_BUFFER_TOO_SMALL;
		SYNO_LogD("Checking size of list buffer failed! Request slot count:0x%x", count);
        return rv;
    }

    /* copy the slot number into the list */
    count = 0;
    for (i = 0; i< CK_I_SLOT_ANZ; i++)
    {
        /* be sure that the slot_info_table record isn't NULL_PTR */
        if (CK_I_slot_info_table[i] != NULL_PTR)
        {
            if ((tokenPresent == FALSE)
                || (CK_I_slot_info_table[i]->slot_info->flags & CKF_TOKEN_PRESENT))
            {
                pSlotList[count++] = i;
            }
        }
    }

    *pulCount= count;
	SYNO_LogD("Returning slot count::0x%x", i);

	FUNC_END;

    return CKR_OK;
}

CK_RV C_GetSlotInfo(
	CK_SLOT_ID slotID,
	CK_SLOT_INFO_PTR pInfo
	)
{
	CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;
	CK_RV rv = CKR_OK;

	FUNC_BEGIN;

	SYNO_LogD("Try to get slot info for ID:0x%x", slotID);

	/* make sure we are initialized */
	if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}
	
	if (pInfo == NULL_PTR)
	{
        SYNO_LogE("pInfo is NULL!");
		return CKR_ARGUMENTS_BAD;
	}	

	/* TODO: get information wether the token is removable / hardware */
	pInfo->flags = 0;

	rv = CI_GetSlotData(slotID, &slot_data);
	if (rv != CKR_OK)
	{
        SYNO_LogE("CI_GetSlotData() failed! rv = 0x%x", rv);
		return rv;
	}

	strncpy((char*)pInfo->slotDescription,
		(char*)slot_data->slot_info->slotDescription, 64);
	strncpy((char*)pInfo->manufacturerID,
		(char*)slot_data->slot_info->manufacturerID, 32);

	if (CK_I_slot_info_table[slotID]->token_data != NULL_PTR)
	{
		SYNO_LogD("Token present!");
		pInfo->flags |= CKF_TOKEN_PRESENT;
	}
	else
	{
		SYNO_LogD("Token not present!");
	}

	pInfo->hardwareVersion.major = slot_data->slot_info->hardwareVersion.major;
	pInfo->hardwareVersion.minor = slot_data->slot_info->hardwareVersion.minor;
	pInfo->firmwareVersion.major = slot_data->slot_info->firmwareVersion.major;
	pInfo->firmwareVersion.minor = slot_data->slot_info->firmwareVersion.minor;

	FUNC_END;

	return rv;
}

CK_RV C_GetTokenInfo(
	CK_SLOT_ID slotID,
	CK_TOKEN_INFO_PTR pInfo
	)
{
    CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;
    CK_RV rv = CKR_OK;

    FUNC_BEGIN;
	
	SYNO_LogD("Try to get token info for ID:0x%x", slotID);

    /* make sure we are initialized */
	if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}
	
	if (pInfo == NULL_PTR)
	{
        SYNO_LogE("pInfo is NULL!");
		return CKR_ARGUMENTS_BAD;
	}	

    rv = CI_GetSlotData(slotID, &slot_data);
    if (rv != CKR_OK)
    {
        SYNO_LogE("CI_GetSlotData() failed! rv = 0x%x", rv);
		return rv;
    }

    /* check for slot existence */
    if (!(slot_data->slot_info->flags & CKF_TOKEN_PRESENT))
    {
        rv = CKR_TOKEN_NOT_PRESENT;
        SYNO_LogE("Checking token flags whit CKF_TOKEN_PRESENT failed!");
        return rv;
    }
	
    /* check for token data existence */
    if (slot_data->token_data == NULL_PTR)
    {
        rv = CKR_TOKEN_NOT_PRESENT;
        SYNO_LogE("Token not present!");
        return rv;
    }
	
    /* call token method to get token info */
	rv = slot_data->methods->GetTokenInfo(slot_data, pInfo);
	if (CKR_OK != rv)
    {
        SYNO_LogE("Call GetTokenInfo() failed! rv = 0x%x", rv);
        return rv;
    }

    FUNC_END;

    return rv;
}

CK_RV C_GetMechanismList(
        CK_SLOT_ID slotID,
        CK_MECHANISM_TYPE_PTR pMechanismList,
        CK_ULONG_PTR pulCount
      )
{
	CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;
	CK_RV rv = CKR_OK;

	FUNC_BEGIN;
	
	SYNO_LogD("Try to get mechanism list for ID:0x%x", slotID);
	
	/* make sure we are initialized */
	if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}
	
	if (pulCount == NULL_PTR)
	{
        SYNO_LogE("pulCount is NULL!");
		return CKR_ARGUMENTS_BAD;
	}	

	/* make sure that the slot is valid */
	if (slotID >= CK_I_SLOT_ANZ)
	{
		rv = CKR_SLOT_ID_INVALID;
        SYNO_LogE("Invalid SlotID %ul, library only contains %ul slots!", slotID, CK_I_SLOT_ANZ);
		return rv;
	}

	rv = CI_GetSlotData(slotID, &slot_data);
	if (rv != CKR_OK)
	{
        SYNO_LogE("CI_GetSlotData() failed! rv = 0x%x", rv);
		return rv;
	}

	rv = (slot_data->methods->GetMechanismList)(pMechanismList, pulCount);
	if (rv != CKR_OK)
	{
        SYNO_LogE("GetMechanismList() failed! rv = 0x%x", rv);
		return rv;
	}

	return rv;
}

CK_RV C_GetMechanismInfo(
        CK_SLOT_ID slotID,
        CK_MECHANISM_TYPE type,
        CK_MECHANISM_INFO_PTR pInfo
      )
{
	CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;
	CK_RV rv=CKR_OK;

	FUNC_BEGIN;
	
	SYNO_LogD("Try to get mechanism info for slot ID:0x%x and type = 0x%x", slotID, type);

	/* make sure we are initialized */
	if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}
	
	if (pInfo == NULL_PTR)
	{
        SYNO_LogE("pInfo is NULL!");
		return CKR_ARGUMENTS_BAD;
	}	

	rv = CI_GetSlotData(slotID, &slot_data);
	if (rv != CKR_OK)
	{
        SYNO_LogE("CI_GetSlotData() failed! rv = 0x%x", rv);
		return rv;
	}

	rv = (slot_data->methods->GetMechanismInfo)(type, pInfo);
	if (rv != CKR_OK)
	{
        SYNO_LogE("GetMechanismInfo() failed! rv = 0x%x", rv);
		return rv;
	}

	FUNC_END;

	return rv;
}

/* C_InitPIN initializes the normal user's PIN. */
CK_RV C_InitPIN(
	CK_SESSION_HANDLE hSession,  /* the session's handle */
	CK_CHAR_PTR       pPin,      /* the normal user's PIN */
	CK_ULONG          ulPinLen   /* length in bytes of the PIN */
	)
{
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
    CK_RV rv = CKR_OK;

    FUNC_BEGIN;

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}

    /* make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if (rv != CKR_OK)
    {
        SYNO_LogE("CI_ReturnSession() failed! rv = 0x%x", rv);
        return rv;
    }

    /* check the status of the session */
    if (session_data->session_info->state != CKS_RW_SO_FUNCTIONS)
    {
        SYNO_LogE("Function may only be called by the SO!");
        return CKR_USER_NOT_LOGGED_IN;
    }
	
    /* set initialize user PIN */
	CK_I_CALL_TOKEN_METHOD(rv, InitPIN, (session_data, pPin, ulPinLen));
    if (rv != CKR_OK)
    {
        SYNO_LogE("TOK_InitPIN() failed! rv = 0x%x", rv);
        return rv;
    }
	
	/* set the CKF_TOKEN_INITIALIZED flag to token */
	session_data->slot_data->token_data->token_info->flags |= CKF_USER_PIN_TO_BE_CHANGED;

	FUNC_END;

    return rv;
}

/* C_SetPIN modifies the PIN of the user who is logged in. */
CK_RV C_SetPIN(
	CK_SESSION_HANDLE hSession,  /* the session's handle */
	CK_CHAR_PTR       pOldPin,   /* the old PIN */
	CK_ULONG          ulOldLen,  /* length of the old PIN */
	CK_CHAR_PTR       pNewPin,   /* the new PIN */
	CK_ULONG          ulNewLen   /* length of the new PIN */
	)
{
    CK_I_SESSION_DATA_PTR session_data = NULL_PTR;
    CK_RV rv = CKR_OK;

    FUNC_BEGIN;

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}

    /* make sure that this session exists */
    rv = CI_ReturnSession(hSession, &session_data);
    if (rv != CKR_OK)
    {
        SYNO_LogE("CI_ReturnSession() failed! rv = 0x%x", rv);
        return rv;
    }

    /* check the state of the session */
    if((session_data->session_info->state != CKS_RW_PUBLIC_SESSION) &&
        (session_data->session_info->state != CKS_RW_USER_FUNCTIONS) &&
        (session_data->session_info->state != CKS_RW_SO_FUNCTIONS))
    {
        rv =  CKR_SESSION_READ_ONLY;
        SYNO_LogE("Function may only be called in a user-RW or SO-RW session state!");
        return rv;
    }
	
    /* change user PIN */
	CK_I_CALL_TOKEN_METHOD(rv, SetPIN, (session_data, pOldPin, ulOldLen, pNewPin, ulNewLen));
    if (rv != CKR_OK)
    {
        SYNO_LogE("TOK_SetPIN() failed! rv = 0x%x", rv);
        return rv;
    }

    FUNC_END;

    return rv;
}

/** called by init to make the token known to the library.
 * the function actually registers a slot, and not a token
 * The data structure CK_I_SLOT_DATA is then defined in the so-called
 * token implementation.
 */
CK_RV CI_RegisterSlot(
	CK_ULONG slotID,
	/* the variable allocated here is freed in C_Finalize */
	CK_I_SLOT_DATA_PTR slot_data
	)
{
    CK_RV rv = CKR_OK;

    FUNC_BEGIN;

    /* tell the token which slot holds it */
    slot_data->token_data->slot = slotID;

	/* update the table of slots */
	CK_I_slot_info_table = (CK_I_SLOT_DATA_PTR CK_PTR)realloc(CK_I_slot_info_table,	(CK_I_SLOT_ANZ+1) * sizeof(CK_I_SLOT_DATA_PTR));
	if (CK_I_slot_info_table == NULL_PTR)
	{
        SYNO_LogE("unable to allocate slot info table");
		return CKR_HOST_MEMORY;
	}

	CK_I_slot_info_table[CK_I_SLOT_ANZ] = slot_data;
	/* increment the slot counter */
	CK_I_SLOT_ANZ++;

	/* *** do some housekeeping on the new token ** */
	/* alloc the token object list  (the define for the lenght is set in internal.h) */
	rv = CI_InitHashtable(&(slot_data->token_data->object_list), CK_I_object_list_size);
	if (rv != CKR_OK)
	{
        SYNO_LogE("Failure to init token object list! rv = 0x%x", rv);
		return rv;
	}

	FUNC_END;

	return rv;
}

CK_RV CI_UnregisterSlot(
	CK_ULONG ulSlotID
	)
{
	CK_ULONG ulSlotCount;
	CK_RV rv;

	FUNC_BEGIN;

	if (ulSlotID >= CK_I_SLOT_ANZ)
	{
        SYNO_LogE("ulSlotID is wrong! ulSlotID = 0x%x", ulSlotID);
		return CKR_SLOT_ID_INVALID;
	}

	if (CK_I_slot_info_table[ulSlotID] == NULL_PTR)
	{
        SYNO_LogE("CK_I_slot_info_table[ulSlotID] is NULL! ulSlotID = 0x%x", ulSlotID);
		return CKR_SLOT_ID_INVALID;
	}

	CK_I_slot_info_table[ulSlotID] = NULL_PTR;

	/* free the allocated table of dynamically loaded slots */
	ulSlotCount = 0;
	rv = C_GetSlotList(FALSE, NULL_PTR, &ulSlotCount);
	if ( (rv == CKR_OK) && (ulSlotCount == 0) ) 
	{
		free(CK_I_slot_info_table);
		CK_I_slot_info_table = NULL_PTR; /* reinitialize global table */
		CK_I_SLOT_ANZ = 0;               /* reinitialize counter */
	}

	FUNC_END;
	return CKR_OK;
}

/* removal of token from library */
CK_RV CI_RemoveToken(
	CK_ULONG slotID)
{
	CK_RV rv = CKR_OK;
	CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;

	/* make sure we are initialized */
	if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	{
        SYNO_LogE("Cryptoki has not initialized!");
		return CKR_CRYPTOKI_NOT_INITIALIZED;
	}

	rv = CI_GetSlotData(slotID,&slot_data);
	if (rv != CKR_OK)
	{
        SYNO_LogE("CI_GetSlotData() failed! rv = 0x%x", rv);
		return rv;
	}

	/* free the memory for the token object list */
	rv = CI_DestroyHashtable(slot_data->token_data->object_list);
	if (rv != CKR_OK)
	{
        SYNO_LogE("CI_DestroyHashtable() failed! rv = 0x%x", rv);
		return rv;
	}

	//rv = TOK_FinalizeToken(slot_data);
	rv = (slot_data->methods->FinalizeToken)(slot_data);
	if (rv != CKR_OK)
	{
        SYNO_LogE("Call FinalizeToken() failed! rv = 0x%x", rv);
		return rv;
	}

	#if 0
	/* TODO: Separate RemoveSlot and RemoveToken */
	/* set the flags */
	slot_data->slot_info->flags &= ~CKF_TOKEN_PRESENT;
	#endif

	rv = CI_UnregisterSlot(slotID);
	if (rv != CKR_OK)
	{
        SYNO_LogE("CI_UnregisterSlot() failed! rv = 0x%x", rv);
		return rv;
	}

	FUNC_END;

	return CKR_OK;
}

CK_RV CI_GetSlotData(
	CK_ULONG ulSlotID,
	CK_I_SLOT_DATA_PTR CK_PTR ppSlotData
	)
{
	FUNC_BEGIN;
	
	if (ulSlotID >= CK_I_SLOT_ANZ)
	{
        SYNO_LogE("ulSlotID is wrong! ulSlotID = 0x%x", ulSlotID);
		return CKR_SLOT_ID_INVALID;
	}

	*ppSlotData = CK_I_slot_info_table[ulSlotID];
	
	if (*ppSlotData == NULL_PTR)
	{
        SYNO_LogE("Slot data is NULL!");
		return CKR_SLOT_ID_INVALID;
	}

	FUNC_END;
	return CKR_OK;
}

CK_RV CI_GetTokenData(
	CK_ULONG ulSlotID,
	CK_I_TOKEN_DATA_PTR CK_PTR ppTokenData
	)
{
	CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;

	FUNC_BEGIN;

	if (ppTokenData == NULL_PTR)
	{
        SYNO_LogE("ppSlotData is NULL!");
		return CKR_ARGUMENTS_BAD;
	}

	if (ulSlotID > CK_I_SLOT_ANZ)
	{
        SYNO_LogE("ulSlotID is wrong! ulSlotID = 0x%x", ulSlotID);
		return CKR_SLOT_ID_INVALID;
	}

	slot_data = CK_I_slot_info_table[ulSlotID];
	if (slot_data == NULL_PTR)
	{
        SYNO_LogE("ulSlotID is wrong! ulSlotID = 0x%x", ulSlotID);
		return CKR_SLOT_ID_INVALID;
	}

	*ppTokenData = slot_data->token_data;

	if (*ppTokenData == NULL_PTR)
	{
        SYNO_LogE("Token not present! ulSlotID = 0x%x", ulSlotID);
		return CKR_TOKEN_NOT_PRESENT;
	}

	FUNC_END;

	return CKR_OK;
}

/* Functions added in for Cryptoki Version 2.01 or later */
/* C_WaitForSlotEvent waits for a slot event (token insertion,
 * removal, etc.) to occur. */
CK_RV C_WaitForSlotEvent(
	CK_FLAGS flags,        /* blocking/nonblocking flag */
	CK_SLOT_ID_PTR pSlot,  /* location that receives the slot ID */
	CK_VOID_PTR pReserved   /* reserved.  Should be NULL_PTR */
	)
{
	CK_RV rv = CKR_OK;

	FUNC_BEGIN;
  
	/* make sure we are initialized */
	if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
	return CKR_CRYPTOKI_NOT_INITIALIZED;

//#ifndef HAVE_THREADS
//	rv = CKR_FUNCTION_NOT_SUPPORTED;
//#else
	/* while event handlign doesn't work */
	/* 1: check the flags */
	//if(!(flags == 0)||(flags&CKF_DONT_BLOCK))
	if ((flags != 0) && (flags != 1))
	{
		rv = CKR_ARGUMENTS_BAD;
		CI_VarLogEntry("C_WaitForSlotEvent", "%x not a valid flags", rv, 0,flags);
		return rv;
	}

	rv = TOK_WaitForSlotEvent(flags, pSlot, pReserved);
    if (rv != CKR_OK)
    {
        SYNO_LogE("TOK_SetPIN() failed! rv = 0x%x", rv);
        return rv;
    }

//#endif /* HAVE_THREADS */

	FUNC_END;
	return rv;
}
/* }}} */

/* Data structure for all events. */
/*
 * This event sinks are linked allong two lines: one single linked list
 * that lists all events, and for each slot a line that links all events
 * of one slot together. The slot-based line is referenced in turn from
 * a linked list. using a hash table for the slot numbers would be too
 * expensive, as there usually only a small number of slots to a system.
 */
static CK_I_EVENT_SLOT_INFO_PTR slot_event_head = NULL_PTR;
static CK_I_EVENT_SLOT_INFO_PTR slot_event_tail = NULL_PTR;

static CK_I_EVENT_INFO_PTR event_list_head = NULL_PTR;
static CK_I_EVENT_INFO_PTR event_list_tail = NULL_PTR;

/* {{{ CI_RegisterEventSink */
/** Register a sink for a slot event.
 * when this function is called the first time for a given slot, the
 * watching of events for that slot is activated and C_WaitForSlot Event
 * will not return with a CKR_FUNCTION_NOT_SUPPORTED.
 *
 * TODO: in a low memery situation, this function will not roll back
 * properly should any of the allocations fail. Fix that.
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
CK_RV CI_RegisterEventSink(
 CK_SLOT_ID slot,
 CK_I_EventActionCallback action,
 CK_VOID_PTR user_data,
 CK_I_EVENT_INFO_PTR CK_PTR pEvent
)
{
  CK_RV rv = CKR_OK;
  CK_I_EVENT_INFO_PTR new_event=NULL_PTR;
  CK_I_EVENT_SLOT_INFO_PTR curr_slot=NULL_PTR;

  CI_LogEntry("CI_RegisterEventSink", "starting...", rv, 2);

  /* plausibility checks */
  if(action == NULL_PTR)
    {
      rv = CKR_GENERAL_ERROR;
      CI_LogEntry("CI_RegisterEventSink", "no action supplied", rv, 0);
      return rv;
    }
  if(pEvent == NULL_PTR)
    {
      rv = CKR_GENERAL_ERROR;
      CI_LogEntry("CI_RegisterEventSink", "no pHandle supplied", rv, 0);
      return rv;
    }

  /* allocate space for the event info */
  new_event=TC_malloc(sizeof(CK_I_EVENT_INFO));
  if(new_event == NULL_PTR)
    {
      rv = CKR_HOST_MEMORY;
      CI_LogEntry("CI_RegisterEventSink", "unable to allocat memory for event info entry", rv, 0);
      return rv;
    }

  /* set the values */
  new_event->action = action;
  new_event->user_data = user_data;
  new_event->next = NULL_PTR;

  /* insert into list */

  if(event_list_tail != NULL_PTR)
    event_list_tail->next = new_event;
  event_list_tail = new_event;

  if(event_list_head == NULL_PTR)
    event_list_head = new_event;


  /* check for slot in list... */
  for(curr_slot = slot_event_head;
      (curr_slot != NULL_PTR) && (curr_slot->slot != slot) ;
      curr_slot = curr_slot->next);
  if(curr_slot == NULL_PTR) /* we found no entry for this slot */
    {
      /* ... put int a new slot event list */
      curr_slot = TC_malloc(sizeof(CK_I_EVENT_SLOT_INFO));
      if(curr_slot == NULL_PTR)
	{
	  rv = CKR_HOST_MEMORY;
	  CI_LogEntry("CI_RegisterEventSink",
		      "unable to allocate memory for slot event info entry", rv, 0);
	  return rv;
	}

      /* set the values of the new slot */
      curr_slot->head = NULL_PTR;
      curr_slot->tail = NULL_PTR;
      curr_slot->next = NULL_PTR;
      curr_slot->slot = slot;

      /* insert at the end of the event slot list */
      if(slot_event_head == NULL_PTR)
	slot_event_head = curr_slot;
      if(slot_event_tail != NULL_PTR)
	slot_event_tail->next = curr_slot;
      slot_event_tail = curr_slot;

      /* ... activate slot event blocking */

    }
  /* ... and append to slot event list */
  if(curr_slot->tail != NULL_PTR)
    curr_slot->tail->slot_next=new_event;

  curr_slot->tail=new_event;

  new_event->slot_next = NULL_PTR;
  new_event->slot_info = curr_slot;

  /* return the new event info handle */
  *pEvent = new_event;

  CI_LogEntry("CI_RegisterEventSink", "...complete", rv, 2);

  return rv;
}
/* }}} */

/* {{{ CI_RemoveEventSink */
/** Remove an event sink from the event handler.
*/
CK_RV CI_RemoveEventSink(
 CK_I_EVENT_INFO_PTR event
)
{
  CK_I_EVENT_INFO_PTR tmp1_event= event_list_head;
  CK_I_EVENT_SLOT_INFO_PTR slot_info= event->slot_info;
  CK_I_EVENT_INFO_PTR tmp2_event = slot_info->head;
  CK_RV rv = CKR_OK;

  /* find the predecessor in the global list */
  while((tmp1_event->next != event) && (tmp1_event != NULL_PTR))
    tmp1_event = tmp1_event->next;

  if(tmp1_event == NULL_PTR)
    {
      if(event_list_head == event)   /* in case the event pointer is the head */
	{
	  event_list_head = event->next;
	  if(event_list_head == NULL_PTR)
	    event_list_tail = NULL_PTR;
	}
      else  /* this event is not a valid, registered one */
	{
	  rv = CKR_GENERAL_ERROR;
	  CI_LogEntry("CI_RemoveEventSink", "invalid event pointer", rv, 0);
	  return rv;
	}
    }
  else /* this is a normal event, somewhere in the middle of the list */
    tmp1_event->next = event->next;

  /* clean up the tail */
  if(event_list_tail == event)
    event_list_tail = tmp1_event;

  /* find the predecessor in the slot event list */
  while((tmp2_event->slot_next != event) && (tmp2_event != NULL_PTR))
    tmp2_event = tmp2_event->slot_next;

  if(tmp2_event == NULL_PTR)
    {
      if(slot_info->head == event)   /* in case the event pointer is the head */
	{
	  slot_info->head = event->slot_next;
	  if(slot_info->head == NULL_PTR)
	    slot_info->tail = NULL_PTR;
	}
      else  /* this event is not a valid, registered one */
	{
	  rv = CKR_GENERAL_ERROR;
	  CI_LogEntry("CI_RemoveEventSink", "invalid event pointer", rv, 0);
	  return rv;
	}
    }
  else  /* this is a normal event, somewhere in the middle of the list */
    tmp2_event->slot_next = event->slot_next;

  /* clean up the tail */
  if(slot_info->tail == event)
    slot_info->tail = tmp2_event;

  /* free the space for the event info */
  free(event);

  return rv;
}
/* }}} */

/* {{{ CI_SendSlotEvent */
/** Send an event to all event sinks registered for a slot.
 * @param slot        Slot that all sinks are triggered for.
 * @param user_data   if non-NULL, this value will be set as argument
 *                    in the action callbacks instead of the default.
 * @param event_label if non-NULL the string will be written to he log.
 * @return            CKR_HOST_MEMORY when allocating memory failed,
 *                    CKR_OK otherwise.
 */
CK_RV CI_SendSlotEvent(
 CK_SLOT_ID slot,
 CK_VOID_PTR user_data,
 CK_CHAR_PTR event_label
)
{
  CK_I_EVENT_INFO_PTR curr_event;
  CK_I_EVENT_SLOT_INFO_PTR curr_slot;

  /* find the slot event list */
  for(curr_slot = slot_event_head;
      (curr_slot != NULL_PTR) && (curr_slot->slot != slot) ;
      curr_slot = curr_slot->next);
  if(curr_slot != NULL_PTR) /* we have found an entry for this slot */
    {
      curr_event = curr_slot->head;
      /* send one event to each sink in the slot event list */
      while(curr_event)
	{
	  CI_TriggerEvent(curr_event,user_data,event_label);
	  curr_event = curr_event->next;
	}
    }
  return CKR_OK;
}
/* }}} */

/* {{{ CI_TriggerEvent */
/** Send an event to a single event sink.
 * @param event       Event handle as received from CI_RegisterEventSink.
 * @param user_data   if non-NULL, this value will be set as argument
 *                    in the action callbacks instead of the default.
 * @param event_label if non-NULL the string will be written to he log.
 * @return            If one of the internal pointers used or event
 *                    is equal to NULL_PTR the function returns
 *                    CKR_GENERAL_ERROR; otherwise the return values of the
 *                    action callback is returned.
*/
CK_RV CI_TriggerEvent(
 CK_I_EVENT_INFO_PTR event,
 CK_VOID_PTR user_data,
 CK_CHAR_PTR event_label
)
{
  CK_RV rv = CKR_OK;

  if((event == NULL_PTR) ||
     (event->slot_info == NULL_PTR) ||
     (event->action == NULL_PTR))
    return CKR_GENERAL_ERROR;

  if(event_label == NULL_PTR)
    event_label = "<no label>";

  if(user_data == NULL_PTR)
    rv = event->action(event->slot_info->slot, event->user_data);
  else
    rv = event->action(event->slot_info->slot, user_data);

  CI_VarLogEntry("CI_TriggerEvent", "trigger event: %s", rv, 2,event_label);

  return rv;
}
/* }}} */

/* {{{ CI_InitEventHandler */
/** Initialize the event handling.
*/
CK_RV CI_InitEventHandler(void)
{
  return CKR_OK;
}
/* }}} */

/* {{{ CI_FinalizeEventHandler */
/** Finalize the event handling.
*/
CK_RV CI_FinalizeEventHandler(void)
{
  CK_I_EVENT_INFO_PTR old_event,curr_event=event_list_head;
  CK_I_EVENT_SLOT_INFO_PTR old_slot, curr_slot = slot_event_head;

  /* Enter Critical Region !!! */

  /* free all entries in the event list */
  while(curr_event != NULL_PTR)
    {
      old_event = curr_event;
      curr_event = curr_event->next;
      free(old_event);
    }

  while(curr_slot != NULL_PTR)
    {
      old_slot = curr_slot;
      curr_slot = curr_slot->next;
      free(old_slot);
    }

  /* Exit Critical Region !!! */
  return CKR_OK;
}
/* }}} */


/*
 * Local variables:
 * folded-file: t
 * end:
 */







