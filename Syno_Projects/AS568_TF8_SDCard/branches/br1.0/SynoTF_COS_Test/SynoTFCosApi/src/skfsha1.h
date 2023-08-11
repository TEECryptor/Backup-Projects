/***************************************************************************
 *	File name:skfsha1.h
 *	Introduce:Head file for sha1 definition
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _SKFSHA1_H_
#define _SKFSHA1_H_

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "skfsms4.h"

#undef BIG_ENDIAN_HOST

#define rol(x,n) ( ((x) << (n)) | ((x) >> (32-(n))) )

typedef struct {
    UINT32  h0,h1,h2,h3,h4;
    UINT32  nblocks;
    UINT8   buf[64];
    UINT32  count;
} SHA1_CONTEXT;

#ifdef __cplusplus
extern "C" {
#endif

void SKFSHA1_Init(SHA1_CONTEXT *hd);
void SKFSHA1_Update(SHA1_CONTEXT *hd, UINT8 *inbuf, UINT32 inlen);
void SKFSHA1_Final(SHA1_CONTEXT *hd,UINT8 output[20]);
void SKFSHA1(UINT8 *pIn, UINT32 nLen,  UINT8 *pOut);

#ifdef __cplusplus
}
#endif

#endif /* _SKFSHA1_H_ */

