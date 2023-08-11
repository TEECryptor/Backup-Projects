/***************************************************************************
 *	File name:init.c
 *	Introduce:The implement file for initialize functions
 *	Author:Syno common
 *	Date:2015/11/01
 *  Version:v1.0
 **************************************************************************/
#include "init.h"
#include "mutex.h"

CK_ULONG CK_I_global_flags = 0;

CK_I_EXT_FUNCTION_LIST CK_I_ext_functions;

CK_RV CI_Initialize(CK_VOID_PTR pInitArgs)
{
	CK_C_INITIALIZE_ARGS_PTR iargs = NULL_PTR;
	CK_RV rv = CKR_OK;
	int i;

    FUNC_BEGIN;

	/* make sure we have not been initialized before */
	if ((CK_I_global_flags & CK_IGF_INITIALIZED))
	{
		rv = CKR_CRYPTOKI_ALREADY_INITIALIZED;
        ICA_LogE("check initialization error!");
		return rv;
	}

	/* initialize all tokens */
	CI_TokenInit();

	if (pInitArgs == NULL_PTR)
	{
		/* assign default values */
		CK_I_ext_functions._CreateMutex  = &I_CreateMutex;
		CK_I_ext_functions._DestroyMutex = &I_DestroyMutex;
		CK_I_ext_functions._LockMutex    = &I_LockMutex;
		CK_I_ext_functions._UnlockMutex  = &I_UnlockMutex;

		rv = CI_ObjInitialize();
		if(rv != CKR_OK)
		{
			CI_LogEntry("C_Initialize",
			"failed to initialize global objects list", rv, 0);
			return rv;
		}

		/* Initialize the session hashtable : session_table (else only
		 * initialized when a session is created) */
		rv = CI_InitHashtable(&CK_I_app_table.session_table, CK_I_OBJ_INITIAL_SIZE);
		if(rv != CKR_OK)
		{
			CI_LogEntry("C_Initialize", "creating application session list",
			rv, 0);
			return rv;
		}

		/* Initalization successfull*/
		CK_I_global_flags |= CK_IGF_INITIALIZED;
		{
            ICA_LogD("Initialize completed!");
			return rv;
		}
	}

	iargs = (CK_C_INITIALIZE_ARGS_PTR)pInitArgs;

	/* pReseved must be NULL_PTR */
	if(iargs->pReserved != NULL_PTR)
	{
		rv = CKR_ARGUMENTS_BAD;
		CI_LogEntry("C_Initialize", "checking Parameters", rv, 1);
		return rv;
	}

	/* Check that either all or none of the mutex functions are supplied */
	i=0;
	(iargs->CreateMutex == NULL_PTR)?i:i++;
	(iargs->DestroyMutex == NULL_PTR)?i:i++;
	(iargs->LockMutex == NULL_PTR)?i:i++;
	(iargs->UnlockMutex == NULL_PTR)?i:i++;
	if((i!=0) && (i!=4))
	{
		rv = CKR_ARGUMENTS_BAD;
		CI_LogEntry("C_Initialize", "checking Mutex Fkt-Pointers", rv, 1);
		return rv;
	}

	/* Determine which type of multi-thread access we have */
	if(i==0) /* functions are not supplied */
	{
		if(iargs->flags & CKF_OS_LOCKING_OK)
		{
			/* 2. use default mutex functions suplies by library/OS */
			CK_I_ext_functions._CreateMutex  = &I_CreateMutex;
			CK_I_ext_functions._DestroyMutex = &I_DestroyMutex;
			CK_I_ext_functions._LockMutex    = &I_LockMutex;
			CK_I_ext_functions._UnlockMutex  = &I_UnlockMutex;
		}
		else
			/* 1. There will be no multiple thread access. */
			CK_I_global_flags |= CK_IGF_SINGLE_THREAD;
	}
	else /* functions are supplied */
    {
		CK_I_ext_functions._CreateMutex  = iargs->CreateMutex;
		CK_I_ext_functions._DestroyMutex = iargs->DestroyMutex;
		CK_I_ext_functions._LockMutex    = iargs->LockMutex;
		CK_I_ext_functions._UnlockMutex  = iargs->UnlockMutex;
    }

	rv = CI_ObjInitialize();
	if(rv != CKR_OK)
	{
		CI_LogEntry("C_Initialize",
			"failed to initialize global objects list", rv, 0);
		return rv;
	}

	/* Initalization successfull*/
	CK_I_global_flags |= CK_IGF_INITIALIZED;

    FUNC_END;
	return CKR_OK;
}


#if 0
/* }}} */
/* {{{ C_Finalize */
/* C_Finalize indicates that an application is done with the
 * Cryptoki library. */
CK_RV C_Finalize(
        CK_VOID_PTR pReserved
)
{
  CK_RV rv = CKR_OK;
  CK_SLOT_ID_PTR slot_list =NULL_PTR;
  CK_ULONG slot_num =0;
  CK_ULONG i;

  CI_LogEntry("C_Finalize", "starting...", rv, 1);

    /* make sure we are initialized */
  if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
    return CKR_CRYPTOKI_NOT_INITIALIZED;

  if(pReserved != NULL_PTR)
    return CKR_ARGUMENTS_BAD;

  /* there can only be sessions on slot that have token */
  rv = C_GetSlotList(TRUE,NULL_PTR,&slot_num);
  if(rv != CKR_OK)
    {
      CI_LogEntry("C_Finalize", "getting count of slots", rv, 0);
      return rv;
    }

  slot_list = TC_malloc(slot_num*sizeof(CK_SLOT_ID_PTR));
  if( slot_list == NULL_PTR)
    {
      rv = CKR_HOST_MEMORY;
      CI_LogEntry("C_Finalize", "getting space for slot list", rv, 0);
      return rv;
    }

  rv = C_GetSlotList(TRUE,slot_list,&slot_num);
  if(rv != CKR_OK)
    {
      CI_LogEntry("C_Finalize", "getting slots list", rv, 0);
      return rv;
    }

  for(i =0 ; i< slot_num ; i++)
    {
      /* Deallocate all Sessions */
      rv = C_CloseAllSessions(slot_list[i]);
      if(rv != CKR_OK)
	{
	  CI_VarLogEntry("C_Finalize", "removing all sessions in slot %i", rv, 0,
			 slot_list[i]);
	  return rv;
	}

      /* Call CI_FinalizeToken in all Tokens */
      rv = CI_RemoveToken(slot_list[i]);
      if(rv != CKR_OK)
	{
	  CI_VarLogEntry("C_Finalize", "finalizing token in slot %i", rv, 0,
			 slot_list[i]);
	  return rv;
	}
    }
  TC_free(slot_list);

  CI_DestroyHashtable(CK_I_app_table.session_table);
  CK_I_app_table.session_table = NULL_PTR;

  /**** assert that there are no remaining objects ****/
  rv = CI_ObjFinalize();
  if(rv != CKR_OK)
    {
       CI_LogEntry("CI_InternalCloseSession",
		  "failed to clear objects",rv,0);
      return rv;
    }

  CK_I_global_flags ^= CK_IGF_INITIALIZED;
  CI_LogEntry("C_Finalize", "...complete", rv, 1);
  return rv;
}
/* }}} */
/* {{{ C_InitToken */
CK_RV C_InitToken(
  CK_SLOT_ID     slotID,    /* ID of the token's slot */
  CK_CHAR_PTR    pPin,      /* the SO's initial PIN */
  CK_ULONG       ulPinLen,  /* length in bytes of the PIN */
  CK_CHAR_PTR    pLabel     /* 32-byte token label (blank padded) */
)
{
    CK_RV rv = CKR_OK;
    CK_I_SLOT_DATA_PTR slot_data = NULL_PTR;

    CI_LogEntry("C_InitToken", "starting...", rv, 1);

    /* make sure we are initialized */
    if (!(CK_I_global_flags & CK_IGF_INITIALIZED))
        return CKR_CRYPTOKI_NOT_INITIALIZED;

    rv = CI_GetSlotData(slotID,&slot_data);
    if(rv != CKR_OK)
    {
        return rv;
    }

    if (g_ucUseOpenssl)
    {
        if(slot_data->methods->InitToken != NULL_PTR)
        {
            rv = (slot_data->methods->InitToken)(pPin, ulPinLen, pLabel);
        }
    }
    else
    {
        rv = ICA_TOK_InitToken(slot_data, pPin, ulPinLen, pLabel);
    }

    CI_LogEntry("C_InitToken", "...complete", rv, 1);

    return rv;
}
/* }}} */

CK_RV CI_FindConfFile()
{
    CK_RV rv = CKR_OK;              /* function return value */

    FUNC_BEGIN;

    if(CK_I_config_fname == NULL_PTR)
    {
#if defined (CK_Win32)
        {
            CK_I_config_fname = ICA_MALLOC(strlen(CK_I_WINDOWS_RC_FNAME) + 1);
            if(CK_I_config_fname == NULL_PTR)
            {
                rv = CKR_HOST_MEMORY;
                ICA_LogE("Allocating Buffer Space");
                return rv;
            }

            strcpy(CK_I_config_fname, CK_I_WINDOWS_RC_FNAME);
        }
#else
        {
            CK_I_config_fname = ICA_MALLOC(strlen(CK_I_UNIX_RC_FNAME) + 1);
            if(CK_I_config_fname == NULL_PTR)
            {
                rv = CKR_HOST_MEMORY;
                ICA_LogE("Allocating Buffer Space");
                return rv;
            }

            strcpy(CK_I_config_fname, CK_I_UNIX_RC_FNAME);
        }
#endif
    }

  FUNC_END;
  return rv;
}

CK_I_DLL_INFO_PTR CK_I_DLL_INFO_new(
  void
)
{
  CK_I_DLL_INFO_PTR retval= TC_malloc(sizeof(CK_I_DLL_INFO));
  if(retval == NULL_PTR) return retval;

  /* init der Info-Structs */
  retval->handle = NULL_PTR;
  retval->dll_path = NULL_PTR;

  return retval;
}

static CK_I_HASHTABLE_PTR CK_I_dll_list = NULL_PTR;

CK_RV CI_InitDllTable()
{
    CK_RV rv = CKR_OK;

    FUNC_BEGIN;

    /* Initialisieren der DLL Hashtabelle */
    rv = CI_InitHashtable(&CK_I_dll_list,20);
    if(rv != CKR_OK)
    {
        ICA_LogE("could not init Hashtable");
        return rv;
    }

    FUNC_END;
    return CKR_OK;
}

void CI_TokenInit()
{
	token_init fkt_ptr;
	CK_I_SLOT_DATA_PTR pSlotData = NULL_PTR;
	CK_RV rv = CKR_OK;
	CK_CHAR_PTR config_list;                /* list of all token */
	CK_CHAR_PTR token_name = NULL_PTR;      /* token returned by strtok */
	CK_CHAR_PTR token_dll_name = NULL_PTR;  /* name of the token library */
	CK_CHAR_PTR init_fkt = NULL_PTR;        /* name of the initialisation function */
	char CK_PTR remainder = NULL_PTR;       /* temp for strtok, retaining rest of string */
	CK_CHAR aucTokenList[PATH_MAX] = {0};
	CK_ULONG TokenListLen = PATH_MAX;
	CK_ULONG slotID =0;

    FUNC_BEGIN;

	/**** Get Token List ****/
	if (!g_ucUseOpenssl)
	{
		rv = ICA_TOK_EnumToken(aucTokenList, &TokenListLen);
		if (CKR_OK != rv)
		{
			ICA_LogD("TokenInitialize,rv=<%d>", rv);
			return;
		}

		ICA_LogD("aucTokenList=<%s>", aucTokenList);

		config_list = (CK_CHAR_PTR)ICA_MALLOC(strlen((char *)aucTokenList) + 1);
		strcpy((char *)config_list, (char *)aucTokenList);
	}
	else
	{
		config_list = (CK_CHAR_PTR)ICA_MALLOC(strlen(g_pcTokenDllName) + 1);
		strcpy((char *)config_list, (char *)g_pcTokenDllName);
	}

    token_name = TC_strtok_r(config_list,",",&remainder);

	slotID = 0;
	while(token_name != NULL_PTR)
    {
        if (g_ucUseOpenssl)
        {
			pSlotData = NULL_PTR;
			//fkt_ptr(token_name, &pSlotData);
            ceayToken_init(token_name, &pSlotData);
			if(pSlotData == NULL_PTR)
			{
				goto next_token;
			}

            memcpy(pSlotData->token_data->token_info->serialNumber, g_pcSerialNumber, 16);

			CI_RegisterSlot(slotID, pSlotData);

            ICA_LogD("slot registered");
        }
        else
        {
            pSlotData = NULL_PTR;
            rv = ICA_TOK_TokenInitialize(token_name, &pSlotData);
            if (CKR_OK != rv)
            {
                ICA_LogE("ICA_TOK_TokenInitialize err!");
                return;
            }

            CI_RegisterSlot(slotID, pSlotData);
        }

	next_token:
		if (token_dll_name != NULL_PTR)
		{ TC_free(token_dll_name); token_dll_name= NULL_PTR; }

		if (init_fkt != NULL_PTR)
		{ TC_free(init_fkt); init_fkt= NULL_PTR; }

		token_name = TC_strtok_r(NULL_PTR,", ",&remainder);

		slotID++;
    }

	if (NULL != config_list)
	{
		TC_free(config_list);
		config_list = NULL_PTR;
	}

}

CK_RV CI_GetDllHandle(
  CK_CHAR_PTR pSymbolicName,
  CK_CHAR_PTR pLibraryName,
  CK_DLL_HANDLE_PTR pHandle
)
{
  CK_RV rv = CKR_OK;
  CK_I_DLL_INFO_PTR dll_info = NULL_PTR;

  CI_LogEntry("CI_GetDllHandle", "starting...", rv, 1);

  if(CK_I_dll_list == NULL_PTR)
    {
      rv = CI_InitDllTable();
      if(rv != CKR_OK)
	{
	  CI_LogEntry("CI_GetDllHandle", "could not init dll table", rv, 1);
	  return rv;
	}
    }

  if(pSymbolicName == NULL_PTR) pSymbolicName = pLibraryName;

  CI_LogEntry("CI_GetDllHandle", "done dll table init", rv, 1);

  rv = CI_HashGetEntry(CK_I_dll_list, CI_string_hash(pSymbolicName),
		       (CK_VOID_PTR CK_PTR)&dll_info);
  if((rv != CKR_OK) && (rv != CKR_ARGUMENTS_BAD))
    {
      CI_LogEntry("CI_GetDllHandle", "dll table retrieve", rv, 1);
      return rv;
    }

  /* new dll? */
  if (rv == CKR_ARGUMENTS_BAD)
    {
      dll_info = CK_I_DLL_INFO_new();
      if(dll_info == NULL_PTR)
	{
	  rv = CKR_HOST_MEMORY;
	  CI_LogEntry("CI_GetDllHandle", "could not allocate dll info", rv, 1);
	  return rv;
	}

      dll_info->dll_path=TC_malloc(strlen(pLibraryName)+1);
      if(dll_info->dll_path == NULL_PTR)
	{
	  rv = CKR_HOST_MEMORY;
	  CI_LogEntry("CI_GetDllHandle", "could not allocate lib name memory", rv, 1);
	  return rv;
	}
      strcpy(dll_info->dll_path,pLibraryName);

      rv = CI_HashPutEntry(CK_I_dll_list,CI_string_hash(pSymbolicName),dll_info);
      if(rv != CKR_OK)
	{
	  CI_LogEntry("CI_GetDllHandle", "could not put new dll in table", rv, 1);
	  return rv;
	}
    }

  /* dll not opened */
  if(dll_info->handle == NULL_PTR)
    {
      CK_CHAR_PTR reason = NULL_PTR;
#if defined(CK_Win32)
      if((dll_info->handle = LoadLibrary(dll_info->dll_path)) == NULL_PTR)
	{
	  CK_CHAR buff[1024];
	  rv = CKR_GENERAL_ERROR;

	  FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			 NULL, GetLastError(),
			 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			 (LPTSTR) &reason,
			 0, NULL);

	  sprintf(buff, "Opening Dynamic Library '%s' failed: %s (PATH: %s)", dll_info->dll_path,reason,getenv("PATH"));
	  MessageBox(NULL, buff, "TC PKCS#11",MB_OK|MB_ICONWARNING);

	  LocalFree(reason);

#else /* ! defined(CK_Win32) */
      if((dll_info->handle = dlopen(dll_info->dll_path, RTLD_LAZY)) == NULL_PTR)
	{

	  rv = CKR_GENERAL_ERROR;
	  reason = dlerror();

#endif /* ! defined(CK_Win32) */
	  CI_VarLogEntry("CI_GetDLLHandle", "Opening Dynamic Library '%s' failed: %s",
			 rv, 0,dll_info->dll_path,reason);
	  return rv;
	}
    }

  *pHandle = dll_info->handle;

  CI_LogEntry("CI_GetDllHandle", "...complete", rv, 1);

  return rv;
}
/* }}} */

/* {{{ CI_InitTokenDll */
token_init CI_InitTokenDll(
   CK_CHAR_PTR token_name,
   CK_CHAR_PTR symbol_name,
   CK_CHAR_PTR token_lib_name
)
{
  CK_DLL_HANDLE handle;
  token_init retval;
  CK_RV rv = CKR_OK;

  CI_LogEntry("CI_InitTokenDll", "starting...", rv, 1);

  rv = CI_GetDllHandle(token_name,token_lib_name,&handle);
    if(rv != CKR_OK)
    {
      CI_LogEntry("CI_InitTokenDll", "retrieving dll handle", rv, 0);
      return NULL_PTR;
    }

  CI_LogEntry("CI_InitTokenDll", "starting dref of lib handle", rv, 0);

  retval=DREF_DLL(handle, token_init, symbol_name);

  if(retval == NULL_PTR) rv = CKR_GENERAL_ERROR;
  CI_LogEntry("CI_InitTokenDll", "...complete", rv, 1);

  return retval;
}
/* }}} */
/* {{{ CI_UnloadDlls */

CK_RV CI_UnloadDlls()
{
  CK_I_HASH_ITERATOR_PTR iter;
  CK_RV rv = CKR_OK;
  CK_I_DLL_INFO_PTR pDllInfo = NULL_PTR;
  CK_CHAR_PTR reason = NULL_PTR;
  CK_ULONG key;

  CI_LogEntry("CI_UnloadDlls", "starting...", rv, 2);

  rv = CI_HashIterateInit(CK_I_dll_list,&iter);
  if( rv != CKR_OK)
    {
      CI_LogEntry("CI_UnloadDlls", "setting iterator", rv, 0);
      return rv;
    }

  for(;iter != NULL_PTR; )
    {
      rv = CI_HashIterateDeRef(iter, &key, (CK_VOID_PTR_PTR)(&pDllInfo));
      if(rv != CKR_OK)
	{
	  CI_LogEntry("CI_UnloadDlls", "retrieving dll_info", rv, 0);
	  return rv;
	}

      /* handle == NULL_PTR -> cannot be freed. handle is initialized
       * in CI_getDllHandle, called only for token and not all dlls specified
       * in the config file
       */
      if(pDllInfo->handle != NULL_PTR)
	{
#if defined(CK_Win32)
	  if(FreeLibrary(pDllInfo->handle) != 0)
	    {
	      reason = "How should I know? Windows doesn't tell me";
#elif defined(CK_GENERIC)
	  if(dlclose(pDllInfo->handle) != 0)
	    {
	      reason = dlerror();
#endif
	      rv = CKR_GENERAL_ERROR;
	      CI_VarLogEntry("CI_UnloadDlls", "could not release Dll '%s': %s",
			     rv, 0,pDllInfo->dll_path, reason);
	      return rv;
	    }
	}

      TC_free(pDllInfo->dll_path);

      /* type of handle : 'HMODULE' under Windows, 'void *' else */
#if defined(CK_Win32)
      TC_free(pDllInfo->handle);
#endif

      TC_free(pDllInfo);

      rv = CI_HashRemoveEntry(CK_I_dll_list, key);
      if(rv != CKR_OK)
	{
	  CI_LogEntry("CI_UnloadDlls", "delete failed", rv, 0);
	  return rv;
	}

      /* removin an element will void the iterator */
      rv = CI_HashIterateInit(CK_I_dll_list, &iter);
      if(rv != CKR_OK)
	{
	  CI_LogEntry("CI_UnloadDlls", "iterate failed", rv, 0);
	  return rv;
	}
    }

  CI_DestroyHashtable(CK_I_dll_list);
  CK_I_dll_list = NULL_PTR;

  TC_free(CK_I_config_fname);
  /* The configuration file name must be reinitialized to NULL_PTR.
   * It has been initialized in the function  CI_LoadDllTable, called by
   * CI_TokenInit, called by C_Initialize.
   */
  CK_I_config_fname = NULL_PTR;

  CI_LogEntry("CI_UnloadDlls", "...complete", rv, 2);

  return rv;
}
/* }}} */
/* {{{ CI_GetConfigString */
static CK_C_CHAR_PTR CK_I_init_fail_reasons[] = {"no error",
					 "File not found",
					 "File not found",
					 "Section not found",
					 "Field not found"};

CK_RV CI_GetConfigString(
  CK_CHAR_PTR pSectionName,
  CK_CHAR_PTR pFieldname,
  CK_CHAR_PTR CK_PTR ppValue
)
{
  CK_CHAR buff[512];
  CK_RV rv = CKR_OK;

  pSectionName=((pSectionName!=NULL_PTR)?pSectionName:(CK_CHAR_PTR)"PKCS11-DLL");

  if(CK_I_config_fname == NULL_PTR)
    {
		rv = CI_FindConfFile();
		if(rv != CKR_OK)
		{
      rv = CKR_GENERAL_ERROR;
      CI_VarLogEntry("CI_GetConfigString", "Reading config field failed: config file not set",
		     rv, 0,
		     pFieldname,
		     pSectionName,
		     CK_I_config_fname,
		     CK_I_init_fail_reasons[rv]);
      return rv;
		}
    }

  rv = TCU_GetProfileString(CK_I_config_fname, pSectionName, pFieldname,
			       buff, 510,FALSE);
  if(rv != 0)
    {
      CI_VarLogEntry("CI_GetConfigString", "Reading config field '%s' from section [%s] in file '%s' failed: %s",
		     CKR_GENERAL_ERROR, 0,
		     pFieldname,
		     pSectionName,
		     CK_I_config_fname,
		     CK_I_init_fail_reasons[rv]);
      return CKR_GENERAL_ERROR;
    }

  *ppValue = TC_malloc(strlen(buff)+1);
  if(*ppValue== NULL_PTR) return CKR_HOST_MEMORY;

  strcpy(*ppValue, buff);

  return CKR_OK;
}
/* }}} */
#if 0
/* #if defined(CK_Win32) */
/* {{{ DllMain */

BOOL __stdcall DllMain(HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
#if 0
  switch( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH:
      CI_TokenInit();
      CI_ReadLogLevel();
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      CI_UnloadDlls();
      break;
    default:
      break;
    }

#endif
   return TRUE;
}
/* }}} */
#endif

/* {{{ CI_GetSignals */
/* since netscape masks all signals we have to get them back in order to
 *   make debugging easier
 */
/* we need some unix specific defines to make this work, but only under

 * unix it will. so if there is not unistd.h the whole thing is moot */

#ifdef HAVE_UNISTD_H

typedef void (*sig_handler)(int);

sig_handler old_tab[MAX_SIG_NUM];

void CI_dummy_sig_handle(int sig)
{
  CI_VarLogEntry("CI_dummy_sig_handle", "caught signal '%d' in PID '%d'", CKR_OK, 0,sig,getpid());
  (old_tab[sig])(sig);
  return;
}

CK_RV CI_GetSignals()
{
  int i;
  CK_RV rv = CKR_OK;

  CI_LogEntry("CI_GetSignals", "starting...", rv, 0);

  for(i=1;i<MAX_SIG_NUM;i++)
    old_tab[i]=signal(i,&CI_dummy_sig_handle);

  CI_LogEntry("CI_GetSignals", "...ending", rv, 0);

  return CKR_OK;
}

#else /* !HAVE_UNISTD_H */

CK_RV CI_GetSignals(

)

{

  return CKR_OK;

}
#endif /* !HAVE_UNISTD_H */
 /* }}} */
/*
 * Local variables:
 * folded-file: t
 * end:
 */
#endif	//0