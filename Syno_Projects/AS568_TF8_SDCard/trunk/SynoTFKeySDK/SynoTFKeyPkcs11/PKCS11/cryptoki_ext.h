/***************************************************************************
 *	File name:cryptoki_ext.h
 *	Introduce:The extened definition for pkcs11 in Syno
 *	Author:Syno common
 *	Date:2015/11/20
 *  Version:v1.0
 **************************************************************************/

#ifndef __CRYPTOKI_EXT_H__
#define __CRYPTOKI_EXT_H__

#if defined(WIN32)
#include "cryptoki_win32.h"
#elif defined(linux) || defined(macintosh)
#include "cryptoki_linux.h"
#else
#	error "Support WIN32/Linux/MacOSx only. :)"
#endif

// to process the container name, add a defined property to PKCS#11
#define CKA_CONTAINER_NAME 		(CKA_VENDOR_DEFINED + 0x00000001)

#define CKR_CONTAINER_EXIST		(CKR_VENDOR_DEFINED + 0x00001000)
#define CKR_CONTAINER_NOT_EXIST	(CKR_VENDOR_DEFINED + 0x00001001)


#endif //__CRYPTOKI_EXT_H__