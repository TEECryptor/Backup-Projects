

static char RCSID[]="$Id: hash.c,v 1.4 2000/01/31 18:09:02 lbe Exp $";
const char* Version_hash_c(){return RCSID;}

#ifndef CK_I_library_build
#define CK_I_library_build
#endif

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "internal.h"
#include "mutex.h"
#include "error.h"

/* extremely simple but should suffice */
#define CI_HASH(_table,_key)  ((_key) % (_table->tab_size))


CK_RV CI_InitHashtable(CK_I_HASHTABLE_PTR_PTR ppHashTable, CK_ULONG size)
{
    assert(size!=0);

    *ppHashTable = TC_calloc(1,sizeof(CK_I_HASHTABLE));
    if(*ppHashTable == NULL_PTR)
    {
        return CKR_HOST_MEMORY;
    }

    (*ppHashTable)->tab_size = size;
    (*ppHashTable)->table = TC_calloc(size,sizeof(CK_I_HASH_BUCKET));
    if((*ppHashTable)->table == NULL_PTR)
    {
        TC_free(*ppHashTable);
        *ppHashTable = NULL_PTR;
        return CKR_HOST_MEMORY;
    }

    return CKR_OK;
}
/* }}} */
/* {{{ CI_DestroyHashtable */
CK_RV CI_DestroyHashtable(
  CK_I_HASHTABLE_PTR pHashTable  /* hash table to be destroyed */
)
{
  unsigned int i;
  CK_I_HASH_BUCKET_PTR next = NULL_PTR;
  CK_I_HASH_BUCKET_PTR tmp = NULL_PTR;

  if(pHashTable == NULL_PTR) return CKR_GENERAL_ERROR;

  if(pHashTable->entries != 0)
    CI_LogEntry("CI_DestroyHashtable","**** Hashtable contains data! ****",0,1);

  for(i=0;i<pHashTable->tab_size;i++)
    {
      tmp= pHashTable->table[i];
      while(tmp != NULL_PTR)
	{
	  next = tmp->next;
	  TC_free(tmp);
	  tmp = next;
	}
    }

  TC_free(pHashTable->table);
  TC_free(pHashTable);

  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashPutEntry */
CK_RV CI_HashPutEntry(
  CK_I_HASHTABLE_PTR pHashTable,  /* returned ptr to hash table */
  CK_ULONG key,                /* key of data, base for hash */
  CK_VOID_PTR val             /* data to be inserted */
)
{
  CK_ULONG index;
  CK_I_HASH_BUCKET_PTR pBucket = NULL_PTR;
  CK_I_HASH_BUCKET_PTR tmpBucket = NULL_PTR;

  if(pHashTable == NULL_PTR) return CKR_GENERAL_ERROR;

  pBucket = TC_calloc(1,sizeof(CK_I_HASH_BUCKET));
  if(pBucket == NULL_PTR)
    return CKR_HOST_MEMORY;

  index = CI_HASH(pHashTable, key);

  /* set bucket values */
  pBucket->key = key;
  pBucket->val = val;
  pBucket->index = index;
  pBucket->next = NULL_PTR; /* sollte eigentlich schon mit dem calloc abgehandelt sein */

  if(pHashTable->table[index] == NULL_PTR)
    {
      pHashTable->table[index]=pBucket;
      pHashTable->entries++;
    }
  else
    {
      tmpBucket= pHashTable->table[index];
      while(TRUE)
	{
	  if(key == tmpBucket->key) /* same key -> replace value */
	    {
	      tmpBucket->val = pBucket->val;
	      TC_free(pBucket);
	      break;
	    }
	  else if(tmpBucket->next == NULL) /* no further bucket -> place as next */
	    {
	      tmpBucket->next = pBucket;
	      pHashTable->entries++;
	      break;
	    }
	  else   /* iterate to next bucket */
	    tmpBucket = tmpBucket->next;
	}
    }

  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashGetEntry */
CK_RV CI_HashGetEntry(
  CK_I_HASHTABLE_PTR pHashTable,  /* returned ptr to hash table */
  CK_ULONG key,                /* key of data, base for hash */
  CK_VOID_PTR_PTR val         /* returned pointer to data*/
)
{
  CK_I_HASH_BUCKET_PTR tmpBucket = NULL_PTR;
  CK_ULONG index;

  /* Test if the table is init'ed at all. */
  if(pHashTable == NULL_PTR) return CKR_GENERAL_ERROR;

  index = CI_HASH(pHashTable,key);

  if(pHashTable->table[index] == NULL_PTR)
      return CKR_ARGUMENTS_BAD; /* no such key */
  else
    {
      tmpBucket= pHashTable->table[index];
      while(TRUE)
	{
	  if(key == tmpBucket->key) /* same key -> return value */
	    {
	      *val = tmpBucket->val;
	      break;
	    }
	  else if(tmpBucket->next == NULL) /* no furter bucker -> error! */
	    return CKR_ARGUMENTS_BAD; /* no such key */
	  else   /* iterate to next bucket */
	    tmpBucket = tmpBucket->next;
	}
    }

  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashRemoveEntry */
CK_RV CI_HashRemoveEntry(
  CK_I_HASHTABLE_PTR pHashTable,  /* returned ptr to hash table */
  CK_ULONG key                /* key of data to be deleted */
)
{
  CK_ULONG index;
  CK_I_HASH_BUCKET_PTR currBucket = NULL_PTR;
  CK_I_HASH_BUCKET_PTR lastBucket = NULL_PTR;

  if(pHashTable == NULL_PTR) return CKR_GENERAL_ERROR;

  index = CI_HASH(pHashTable, key);

  if(pHashTable->table[index] == NULL_PTR)
    {
      return CKR_ARGUMENTS_BAD; /* no such key */
    }
  else
    {
      currBucket = pHashTable->table[index];
      lastBucket = NULL_PTR;

      while(TRUE)
	{
	  if(key == currBucket->key) /* same key -> remove bucket */
	    {
	      if(lastBucket == NULL_PTR) /* Entry in the table array */
		/* this should set the table entry to NULL if there is no further element */
		pHashTable->table[index] = currBucket->next;
	      else
		lastBucket->next = currBucket->next;

	      TC_free(currBucket);
	      pHashTable->entries--;

	      break;
	    }
	  else if(currBucket->next == NULL) /* no furter bucket -> no such key */
	    {
	      return CKR_ARGUMENTS_BAD;
	      break;
	    }
	  else   /* iterate to next bucket */
	    {
	      lastBucket = currBucket;
	      currBucket = currBucket->next;
	    }
	}
    }

  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashEntryExists */
CK_RV CI_HashEntryExists(
  CK_I_HASHTABLE_PTR pHashTable,  /* returned ptr to hash table */
  CK_ULONG key                 /* key of data to be deleted */
)
{
  CK_ULONG index;
  CK_I_HASH_BUCKET_PTR currBucket = NULL_PTR;
  CK_RV rv = CKR_OK;

  if(pHashTable == NULL_PTR) return CKR_GENERAL_ERROR;

  index = CI_HASH(pHashTable, key);

  if(pHashTable->table[index] == NULL_PTR)
    {
      return CKR_ARGUMENTS_BAD; /* no such key */
    }
  else
    {
      currBucket = pHashTable->table[index];

      while(TRUE)
	{
	  if(key == currBucket->key) /* same key -> return all ok */
	    {
	      break;
	    }
	  else if(currBucket->next == NULL) /* no furter bucket -> no such key */
	    {
	      rv = CKR_ARGUMENTS_BAD;
	      break;
	    }
	  else   /* iterate to next bucket */
	    {
	      currBucket = currBucket->next;
	    }
	}
    }

  return rv;
}
/* }}} */
/* {{{ CI_HashtableToString */

/* handle , space, pointer, newline + new line */
#define CK_IH_TEXTSIZE 3+1+8+1+1

CK_CHAR_PTR CI_HashtableToString(CK_I_HASHTABLE_PTR pHashTable)
{
  CK_I_HASH_ITERATOR_PTR iter;
  CK_CHAR_PTR buff = NULL_PTR;
  CK_CHAR_PTR curr_p = NULL_PTR;
  CK_ULONG key, chars=0;
  CK_VOID_PTR val = NULL_PTR;

  if(pHashTable == NULL_PTR) return NULL_PTR;


  curr_p = buff = TC_malloc((CK_IH_TEXTSIZE*pHashTable->entries)+1);
  if(buff == NULL_PTR)
    return NULL_PTR;

  CI_HashIterateInit(pHashTable, &iter);
  assert( CI_HashIterValid(iter));
  for(; CI_HashIterValid(iter) ; CI_HashIterateInc(iter), curr_p+=chars)
    {
      CI_HashIterateDeRef(iter, &key, &val);

      chars = sprintf(curr_p, "%3lu:0x%p\n", key, val);
    }

  return buff;
}
/* }}} */
/* {{{ CI_string_hash */
CK_ULONG CI_string_hash(CK_CHAR_PTR string)
{
  CK_ULONG retval =0;
  for(;*string != '\0' ; string++)
    {
      retval += *string;
      retval = retval << 1;
    }

  return retval;
}
/* }}} */

/* {{{ CI_HashIterateInit */
CK_RV CI_HashIterateInit(
 CK_I_HASHTABLE_PTR pHashTable,
 CK_I_HASH_ITERATOR_PTR CK_PTR pIterator
)
{
  CK_ULONG i;

  if(pHashTable == NULL_PTR)
    {
      *pIterator = NULL_PTR;
      return CKR_GENERAL_ERROR;
    }

  /* any elements at all? */
  if(pHashTable->entries == 0)
    {
      *pIterator=NULL_PTR;
      return CKR_OK;
    }

  /* get the mem */
  if( (*pIterator = TC_calloc(1,sizeof(CK_I_HASH_ITERATOR))) == NULL_PTR)
    return CKR_HOST_MEMORY;

  (*pIterator)->table = pHashTable;

  for(i=0;(i < pHashTable->tab_size) && (pHashTable->table[i] == NULL_PTR);i++);
  if(i < pHashTable->tab_size)
    (*pIterator)->curr_bucket=pHashTable->table[i];
  else
    (*pIterator)=NULL_PTR;

  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashIterateInc */
/* pIterator is set to NULL_PTR if there is no further element. */
CK_RV CI_HashIterateInc(
 CK_I_HASH_ITERATOR_PTR pIterator
)
{
  CK_ULONG i;

  /* paranoia test */
  if(pIterator == NULL_PTR)
    return CKR_GENERAL_ERROR;

  if(pIterator->table == NULL_PTR)
    return CKR_GENERAL_ERROR;

  if(pIterator->curr_bucket == NULL_PTR)
    return CKR_GENERAL_ERROR;

  if(pIterator->curr_bucket->next != NULL_PTR)
    {
      pIterator->curr_bucket = pIterator->curr_bucket->next;
      return CKR_OK;
    }

  /* OK we have to walk along the table to find the next filled bucket */
  for(i = (pIterator->curr_bucket->index)+1; ((i< pIterator->table->tab_size) &&
				 (pIterator->table->table[i] == NULL_PTR) ); i++);
  if(i < pIterator->table->tab_size)
    pIterator->curr_bucket=pIterator->table->table[i];
  else
    {
    pIterator->table=NULL_PTR;
    pIterator->curr_bucket=NULL_PTR;
    }

  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashIterateDeRef */
CK_RV CI_HashIterateDeRef(
 CK_I_HASH_ITERATOR_PTR pIterator,
 CK_ULONG_PTR pKey,
 CK_VOID_PTR_PTR ppVal
)
{
  /* paranoia test */
  if((pIterator == NULL_PTR) ||
     (pIterator->table == NULL_PTR) ||
     (pIterator->curr_bucket == NULL_PTR))
    return CKR_GENERAL_ERROR;

  if(pKey != NULL_PTR)
    *pKey = pIterator->curr_bucket->key;

  if(ppVal != NULL_PTR)
    *ppVal = pIterator->curr_bucket->val;

  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashIterateDel */
CK_RV CI_HashIterateDel(CK_I_HASH_ITERATOR_PTR pIterator)
{
  CK_I_HASH_ITERATOR_PTR tmp_iter = NULL_PTR;
  CK_ULONG tmp_key;
  CK_RV rv = CKR_OK;

  if((pIterator == NULL_PTR) ||
     (pIterator->table == NULL_PTR) ||
     (pIterator->curr_bucket == NULL_PTR))
    return CKR_GENERAL_ERROR;

  rv = CI_HashIterateDup(pIterator,&tmp_iter);
  if(rv != CKR_OK) return rv;

  rv = CI_HashIterateInc(pIterator);
  if(rv != CKR_OK) return rv;

  rv = CI_HashIterateDeRef(tmp_iter, &tmp_key, NULL_PTR);
  if(rv != CKR_OK) return rv;

  rv = CI_HashRemoveEntry(tmp_iter->table, tmp_key);

  TC_free(tmp_iter); /* the iter was dup'ed */

  return rv;
}
/* }}} */
/* {{{ CI_HashIterateDup*/
CK_RV CI_HashIterateDup(
 CK_I_HASH_ITERATOR_PTR pIterator,
 CK_I_HASH_ITERATOR_PTR CK_PTR  ppNewIterator
)
{
  /* get the mem */
  if( (*ppNewIterator = TC_calloc(1,sizeof(CK_I_HASH_ITERATOR))) == NULL_PTR)
    return CKR_HOST_MEMORY;

  (*ppNewIterator)->table = (pIterator)->table;
  (*ppNewIterator)->curr_bucket = (pIterator)->curr_bucket;

  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashIterateDelete */
/* clear the iterator */
CK_RV CI_HashIterateDelete(
 CK_I_HASH_ITERATOR_PTR pIterator
)
{
  free(pIterator);
  return CKR_OK;
}
/* }}} */
/* {{{ CI_HashIterValid */
CK_BBOOL CI_HashIterValid(
 CK_I_HASH_ITERATOR_PTR pIterator
)
{
  if((pIterator == NULL) ||
     (pIterator->table == NULL_PTR) ||
     (pIterator->curr_bucket == NULL_PTR))
    return FALSE;

  return TRUE;
}
/* }}} */

/* handle handling functions */
static CK_ULONG CK_I_handle_counter = 1 ; /* must not start with 0 due to CK_INVALID_HANDLE */
/* static CK_ULONG CK_I_handle_counter = 0xfe000000L ; */ /* must not start with 0 due to CK_INVALID_HANDLE */

/* {{{ CI_NewHandle */
CK_RV CI_NewHandle(
  CK_ULONG_PTR handle                 /* handle returned */
)
{
  CK_RV rv = CKR_OK;
   CK_VOID_PTR mutex = NULL_PTR;

  /* get mutex, there are some synchronized areas in here */
  CI_CreateMutex(&mutex);

  _LOCK(mutex);
  if(CK_I_handle_counter == I_MAX_HANDLE)
    rv = CKR_GENERAL_ERROR;
  else
    *handle = (++CK_I_handle_counter);
  _UNLOCK(mutex);

  CI_DestroyMutex(mutex);
  return rv;
}
/* }}} */

/*
 * Local variables:
 * folded-file: t
 * end:
 */
