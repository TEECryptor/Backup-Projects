///////////////////////////////////////////////////////////////////////////////
// AS_Storage.h
// Copyright (c) 2003 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////


#ifndef _AS_Storage_H_
#define _AS_Storage_H_

#include "AS_StorageError.h"
#include "AS_StorageTypes.h"

struct AS_Storage
{


   // constant bitmask for adding logging features
   typedef UInt64 LogLevelType;
   static const LogLevelType LogLevel_Minimum = (1 << 1);

   // modules
   static const LogLevelType LogLevel_StorageDevice = (1 << 4);
      // reserved for submodules
   static const LogLevelType LogLevel_Volume = (1 << 8);
      // reserved for submodules
   static const LogLevelType LogLevel_File = (1 << 12);
      // reserved for submodules
   static const LogLevelType LogLevel_Format = (1 << 16);
      // reserved for submodules

   // add additional modules as needed
   // ...

   // other misc modules and common utilities
   static const LogLevelType LogLevel_OSFileIO = (1 << 20);


   static const LogLevelType LogLevel_Maximum = 0xffffffff;

   // Driver selections
   typedef UInt32 DriverType;
   static const DriverType Driver_Best = 1;
   static const DriverType Driver_Any = 4;
   static const DriverType Driver_Pxhelp = 2;
   static const DriverType Driver_Imapi = 92;


   enum Property
   {
      Prop_LogPath         = 1,           //IN/OUT:  Debug logging file name IN as AS_String, OUT as null-terminated UTF8 sequence of bytes
      Prop_ConfigPath      = 2,           //IN/OUT:  Configuation file name IN as AS_String, OUT as null-terminated UTF8 sequence of bytes
	  Prop_LogFileSize     = 3,			  //IN/OUT:  UInt64 for debug logging file size (defaults to 10 MB)
	  Prop_LogLevel		   = 4,			  //IN/OUT:  LogLevelType for debug logging level       
      Prop_Driver          = 5,           //IN/OUT:  DriverType for transport filter driver
	  Prop_Version         = 6,           //OUT:     UInt16[4] Detailed version info
      // ...   
      // add more properties as needed for general API usage
   };


};

#ifdef __cplusplus
extern "C" {
#endif


AS_StorageError AS_API AS_Storage_GetProperty(			// RETURNS: error number
   const AS_Storage::Property & prop,	// IN: which property to get (see AS_Storage::Prop_...)
   UInt32 bufLen,						// IN: size available in propData buffer
   void * propData,						// OUT: buffer to copy property value into
   UInt32 * propSize,					// OUT: size of data returned in propData
   UInt32 PropertyIndex,				// IN: index of list of property returned
   UInt32 extbufLen,					// IN: size available in extpropData buffer
   void*  extpropData,					// IN/OUT: extended buffer to copy property value into
   UInt32* extpropSize);				// OUT: size of data returned in extpropData 

AS_StorageError AS_API AS_Storage_SetProperty(			// RETURNS: error number
   const AS_Storage::Property & prop,	// IN: which property to set (see AS_Storage::Prop_...)
   UInt32 propSize,						// IN: size of data in propData buffer
   void * propData,						// IN: property data
   UInt32 PropertyIndex,				// IN: index of list of property returned
   UInt32 extbufLen,					// IN: size available in extpropData buffer
   void*  extpropData);					// IN/OUT: extended buffer to copy property value into
   


#ifdef __cplusplus
}
#endif


#endif // _AS_Storage_H_
