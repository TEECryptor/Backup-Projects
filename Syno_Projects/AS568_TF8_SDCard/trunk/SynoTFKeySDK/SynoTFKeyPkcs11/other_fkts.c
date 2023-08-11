
 
static char RCSID[]="$Id: other_fkts.c,v 1.5 2000/01/31 18:09:03 lbe Exp $";
const char* Version_other_fkts_c(){return RCSID;}

#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include "internal.h"
#include "error.h"

/* Object management */
/* C_GetObjectSize gets the size of an object in bytes. */
CK_RV C_GetObjectSize(
  CK_SESSION_HANDLE hSession,  /* the session's handle */
  CK_OBJECT_HANDLE  hObject,   /* the object's handle */
  CK_ULONG_PTR      pulSize    /* receives size of object */
)
{
  CK_RV rv = CKR_OK;

  CI_LogEntry("C_GetObjectSize", "starting...", rv, 1);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;


  rv = CKR_FUNCTION_NOT_SUPPORTED;
  CI_LogEntry("C_GetObjectSize", "...complete", rv, 1);
  return rv;
}

/* Parallel function management */
/* C_GetFunctionStatus is a legacy function, it obtains an
 * updated status of a function running in parallel with an
 * application. */
CK_RV C_GetFunctionStatus(
  CK_SESSION_HANDLE hSession  /* the session's handle */
)
{
  CK_RV rv = CKR_OK;

  CI_LogEntry("C_GetFunctionStatus", "starting...", rv, 1);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;


  rv = CKR_FUNCTION_NOT_SUPPORTED;
  CI_LogEntry("C_GetFunctionStatus", "...complete", rv, 1);
  return rv;
}


/* C_CancelFunction is a legacy function,
 * it cancels a function running in parallel. */
CK_RV C_CancelFunction(
  CK_SESSION_HANDLE hSession  /* the session's handle */
)
{
  CK_RV rv = CKR_OK;

  CI_LogEntry("C_CancelFunction", "starting...", rv, 1);

  /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED)) 
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  rv = CKR_FUNCTION_NOT_SUPPORTED;
  CI_LogEntry("C_CancelFunction", "...complete", rv, 1);
  return rv;
}


/*
 * Local variables:
 * folded-file: t
 * end:
 */
