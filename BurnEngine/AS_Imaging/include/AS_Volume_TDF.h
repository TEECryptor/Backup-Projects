///////////////////////////////////////////////////////////////////////////////
// AS_Volume_TDF.h
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_Volume_TDF_h__
#define __AS_Volume_TDF_h__

#include "AS_StorageError.h"
#include "AS_Volume.h"


#ifdef __cplusplus
extern "C" {
#endif

// Format a blank (use AS_StorageDevice to detect) medium with an ISO image border up front (TDF)  (will mount medium)
AS_StorageError AS_API AS_Volume_CreateTDF(            // RETURNS: error number
   const AS_Volume & volume,                 // IN: volume to flush the FS to disc
   const AS_File::Path & name,                        // IN: Name to write as volume label
   const AS_File::Size & sizeOfIsoImage,                 // IN: Size of ISO image
   AS_Volume::StreamCallback isoPusher,            // IN: stream callback function for ISO image data
   void* isoUserdata,                        // IN: stream callback userdata
   AS_Volume::InfoCallback callback,        // IN: progress callback function
   void* progressUserdata);                     // IN: progress callback userdata
#ifdef __cplusplus
}
#endif

#endif //__AS_Volume_TDF_h__
