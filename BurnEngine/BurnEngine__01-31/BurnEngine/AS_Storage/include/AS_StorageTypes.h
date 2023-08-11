///////////////////////////////////////////////////////////////////////////////
// AS_StorageTypes.h
// Copyright (c) 2003 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef _AS_StorageTypes_H_
#define _AS_StorageTypes_H_

#if defined(__APPLE__)
   #define AS_APPLE
   #define AS_CALLBACK
   #ifndef AS_API
      #ifdef __GNUC__
         #define AS_API __attribute__((visibility("default")))
      #endif // #ifdef __GNUC__
   #endif // #ifndef AS_API
#endif // #if defined(__APPLE__)

#include "AS_BaseTypes.h"

typedef // message callback proc
void AS_CALLBACK AS_MessageProc(
	const void * context,				// IN: message proc context
	AS_String & text,					// OUT: message text
	SInt32 & number);					// OUT: message number

#endif //_AS_StorageTypes_H_
