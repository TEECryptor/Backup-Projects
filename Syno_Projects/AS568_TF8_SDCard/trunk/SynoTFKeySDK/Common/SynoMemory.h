/***************************************************************************
 *	File name:SynoMemory.h
 *	Introduce:Syno common memory operation functions and macro definition
 *	Author:Syno common
 *	Date:2015/11/04
 *  Version:v1.0
 **************************************************************************/

#ifndef _SYNO_MEMORY_H_
#define _SYNO_MEMORY_H_

/*----------------------------------------------*
 * 关于内存操作宏                               *
 *----------------------------------------------*/
#define SYNO_free(handle) do{if(NULL == (handle))break; free(handle); (handle) = NULL;}while(0)

#define SYNO_calloc(nelem, elsize) calloc(nelem, elsize)

#define SYNO_malloc(size) malloc(size)

#endif /* _SYNO_MEMORY_H_ */


