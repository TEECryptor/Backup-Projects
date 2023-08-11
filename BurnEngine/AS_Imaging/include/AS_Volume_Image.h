///////////////////////////////////////////////////////////////////////////////
// AS_Volume_Image.h - EmbC++ Version
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
//
//  APIs to read & write raw blocks on a media
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_Volume_Image_h__
#define __AS_Volume_Image_h__

#include "AS_Volume.h"

#ifdef __cplusplus
extern "C" 
{
#endif


// Open a volume on a device and determine the logical format
AS_StorageError AS_API AS_OpenVolumeImage(    // RETURNS: error number
   const AS_File::Path& pathToImageFile,		// IN: device reference
   AS_Volume::FS_Type& kind,			// OUT: kind of volume detected
   AS_Volume& volume);						// OUT: volume reference



#ifdef __cplusplus
}
#endif

#endif //__AS_Volume_Image_h__
