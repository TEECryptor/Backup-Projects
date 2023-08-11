///////////////////////////////////////////////////////////////////////////////
// AS_File.h
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_File_h__
#define __AS_File_h__

#include "AS_StorageTypes.h"

struct AS_Volume;


struct AS_File
{
   typedef UInt16 Kind;

   static const Kind File_IsFile         = (1 << 0);
   static const Kind File_IsDir          = (1 << 1);
   static const Kind File_IsRealTime     = (1 << 3);   // Has some special limitations

   // For AS_File_GetProperty, AS_Volume_GetEntryProperty, AS_File_SetProperty, and AS_Volume_SetEntryProperty
   enum Property
   {
      Prop_Name            = 1,           // IN/OUT: File name IN as AS_File::Path, OUT as null-terminated UTF8 sequence of bytes
      Prop_Size            = 2,           // OUT:    file size in an 8byte UInt64
      Prop_DateModified    = 3,           // IN/OUT: Modification date of file as AS_Volume::DateTime
      Prop_DateCreated     = 4,           // IN/OUT: Creation date of file as AS_Volume::DateTime
      Prop_LogicalBegin    = 20,          // OUT:    LBA of file's first extent on disc
      Prop_CgmsInfo        = 21,          // IN/OUT: file's CGMS info as AS_File::CgmsInfo struct
      Prop_CurrentPos      = 22,          // OUT:    current file position
      Prop_UdfPermissions  = 23,          // IN/OUT: UDF permissions on file or directory as UInt32
                                          //
                                          //    defaults :
                                          //       file = OTHER_Read   | GROUP_Read   | OWNER_Read
                                          //            | OTHER_ChAttr | GROUP_ChAttr | OWNER_ChAttr
                                          //       directory = OTHER_Read    | GROUP_Read    | OWNER_Read 
                                          //                 | OTHER_ChAttr  | GROUP_ChAttr  | OWNER_ChAttr 
                                          //                 | OTHER_Execute | GROUP_Execute | OWNER_Execute
                                          //    
                                          //    Please refer to UDF 2.6 section 3.3.3.3 on page 70 (www.osta.org) for more information
                                          //    and recommendations on setting these permissions for different operating systems.
                                          //
                                          //    /* Definitions:                                                     */
                                          //    /* Bit      for a File                 for a Directory              */
                                          //    /* -------  ------------------------   ---------------------------- */
                                          //    /* Execute  May execute file           May search directory         */
                                          //    /* Write    May change file contents   May create and delete files  */
                                          //    /* Read     May examine file contents  May list files in directory  */
                                          //    /* ChAttr   May change file attributes May change dir attributes    */
                                          //    /* Delete   May delete file            May delete directory         */
                                          //
                                          //    #define OTHER_Execute 0x00000001
                                          //    #define OTHER_Write   0x00000002
                                          //    #define OTHER_Read    0x00000004
                                          //    #define OTHER_ChAttr  0x00000008
                                          //    #define OTHER_Delete  0x00000010
                                          //    #define GROUP_Execute 0x00000020
                                          //    #define GROUP_Write   0x00000040
                                          //    #define GROUP_Read    0x00000080
                                          //    #define GROUP_ChAttr  0x00000100
                                          //    #define GROUP_Delete  0x00000200
                                          //    #define OWNER_Execute 0x00000400
                                          //    #define OWNER_Write   0x00000800
                                          //    #define OWNER_Read    0x00001000
                                          //    #define OWNER_ChAttr  0x00002000
                                          //    #define OWNER_Delete  0x00004000
      Prop_MacExtProDOS    = 24,          // IN: MacExtensionProDOS struct
      Prop_MacExtHFS       = 25,          // IN: MacExtensionHFS struct
      Prop_Realtime        = 26           // IN: UInt32 to turn on streaming reads/writes (no ECC) typically for video record/playback. 0 = off, 1 = on
   };

   // See DVD-Video spec Part 2 section 3.6.4 for details about this structure
   struct CgmsInfo
   {
      UInt8 CGMSInformation;
      UInt8 DataStructureType;
      UInt8 ProtectionSystemInformation[4];
   };

   // See "Technical Note FL36 - Apple Extensions to ISO 9660" for details
   // about the following structures
   // http://developer.apple.com/technotes/fl/fl_36.html
   struct MacExtensionProDOS
   {
      UInt8 FileType;      // ProDOS file type
      UInt16 AuxType;      // ProDOS aux type
   };

   struct MacExtensionHFS
   {
      UInt32 FileType;     // HFS file type
      UInt32 FileCreator;  // HFS file creator
      UInt16 FinderFlags;  // HFS finder flags
   };

   // Open Modes for AS_Volume_OpenFile
   enum OpenMode // file state
   {
      Closed   = 0,  // Only an output state
      Read     = 1,
      Append   = 2
   };

   // Create Flags for AS_Volume_CreateEntry, AS_Volume_CreateEntryFromHD, AS_Volume_CreateFileFromStream
   typedef UInt32 CreateFlags;
   static const CreateFlags Create_Parents   = (1 << 0); // create full path down to file
   static const CreateFlags Create_Overwrite = (1 << 1); // overwrites existing files

   // Delete Flags for AS_Volume_DeleteEntry
   typedef UInt32 DeleteFlags;
   static const DeleteFlags Delete_Children  = (1 << 0); // If path is a directory, delete all children

   typedef UInt32 Handle;
   typedef AS_String Path;
   typedef UInt32    Count;                // Based on max RAM, amounts to R&W
   typedef UInt64    Size;                 //

   Handle	myHandle;
};


#ifdef __cplusplus
extern "C" {
#endif


// Interactive work with files
//
//-----------------------------------------------------------------------------
// AS_Volume_OpenFile
//
// DESCRIPTION:
// Opens a file on the specified volume and retrieves a file reference for that file.
// Output with the parameter file, the reference may be used in subsequent calls that
// seek within, read from or write to the file. The file to open is specified with fullPath;
// the mode in which the file is to be opened (read or append) is specified with mode. 

// When the open mode is specified as read, the file position defaults to 0 (beginning of file). 
// Use AS_File_Seek to change the location. When the open mode is append, the file position 
// defaults to EOF (end-of-file) unless the specified file does not exist on the volume, 
// in which case the file will be created. AS_File_Seek calls are not allowed while in append mode.
//
// NOTES: 
//      An error will result if the file indicated with fullPath is not 2048-aligned.
//      If mode is set to AS_File::Append and the parents in the path specified with 
//      fullPath already exist, then a new file will be created automatically if a file 
//      of the specified name does not already exist.
//      Once opened in read mode, a file must be closed (see AS_File_Close) and then 
//      reopened in append mode in order to write to it. Similarly, a file opened in 
//      append mode must be closed and then reopened in read mode in order to read from it.
//      To get or set the properties of a file opened with this call, see AS_File_GetProperty 
//      or AS_File_SetProperty.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidAccess    - if mode is set to append and writing in RTTD mode is not allowed for volume
//  AS_StorageError_InvalidOperation - if the volume was not mounted/formatted or if there are open AS_File references
//  AS_StorageError_FileNotFound     - if mode is set to read and file is not found in volume
//  AS_StorageError_InvalidOperation - if mode is set to append and file length is not multiple of block size
//  AS_StorageError_Fatal            - if mode is set to append and failed to create or write to the file
//
AS_StorageError AS_API AS_Volume_OpenFile(
   const AS_Volume & volume,			// IN:  volume reference
   const AS_File::Path & fullPath,		// IN:  full path to file
   const AS_File::OpenMode & mode,		// IN:  attributes on open file (read/append)
   AS_File & file);						// OUT: AS_File reference

//-----------------------------------------------------------------------------
// AS_File_Read
//
// DESCRIPTION:
// Reads data from the specified file into the buffer specified with 
// the parameter buffer, the size of which is specified with numBytes. When the call 
// executes, the amount of data actually read into the buffer is output with numRead.
//
// The file will be read from its current location. By default, that location is 
// set to 0 (beginning of file) when the file is opened in read mode (see AS_Volume_OpenFile). 
// Use AS_File_Seek to seek to a different location.
//
// NOTES: 
// The file must already be open and in read mode (see AS_Volume_OpenFile). 
// If the file was opened in append mode, it must first be closed and then reopened in read mode.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if file was opened with mode not set to read
//  AS_StorageError_Fatal            - if failed to read from file
//
AS_StorageError AS_API AS_File_Read(
   const AS_File & file,				// IN:  AS_File reference to read from current position
   const AS_File::Count & numBytes,		// IN:  Amount to read
   void * buffer,						// IN:  Buffer to put data in
   AS_File::Count * numRead);			// OUT: Number of bytes actually read

//-----------------------------------------------------------------------------
// AS_File_Write
//
// DESCRIPTION:
// Writes data to the end of the specified file from the buffer 
// specified with the parameter buffer. The amount of data to write from the buffer
// is specified with numBytes. When the call executes, the amount of data actually 
// written to the file is output with numWrote.
//
// NOTES: 
// The file must already be open and in append mode (see AS_Volume_OpenFile). 
// If the file was opened in read mode, it must first be closed and then reopened in append mode.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if file was opened with mode not set to append
//  AS_StorageError_Fatal            - if failed to write to file
//
AS_StorageError AS_API AS_File_Write(
   const AS_File & file,				// IN:  Open AS_File reference
   const AS_File::Count & numBytes,		// IN:  Amount to write
   void * buffer,						// IN:  Buffer to write data from
   AS_File::Count * numWrote);			// OUT: Number of bytes actually written

//-----------------------------------------------------------------------------
// AS_File_Seek
//
// DESCRIPTION:
// Sets the location in the specified file that will be the starting 
// position of the next read operation. The location, specified with the parameter 
// toPos, is given as an offset (in bytes) from the start of the file.
//
// NOTES: 
// An error will result if the file referenced with file was opened in append mode
// (see AS_Volume_OpenFile).
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the file was opened with mode not set to read
//
AS_StorageError AS_API AS_File_Seek(
   const AS_File & file,				// IN:  AS_File reference to seek in
   const AS_File::Size & toPos);		// IN:  offset in bytes from start of file

//-----------------------------------------------------------------------------
// AS_File_Close
//
// DESCRIPTION:
// Closes the specified file to further reading or writing 
// and releases the file's reference and associated memory.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_Fatal            - if file could not be closed
//
AS_StorageError AS_API AS_File_Close(
   AS_File & file);						// IN/OUT: AS_File reference to close access to


// Information about a file - see Prop_ ... constants

//-----------------------------------------------------------------------------
// AS_File_GetProperty
//
// DESCRIPTION:
// Retrieves the value of a property of a file. The file, 
// which must already be open (see AS_Volume_OpenFile), is identified by the 
// reference (AS_File struct) specified with the parameter file. To retrieve 
// property information for a file using that file's path instead, see AS_Volume_GetEntryProperty.
//
// The parameter prop indicates the property (end-of-file, state, date modified,
// date created, etc.) for which to retrieve the current value. The value is 
// output to a buffer specified with propData, the size of which is specified 
// with bufLen. The actual size of the data written to the buffer is output with propSize.
//
// The extended optional parameters are provided for additional property data to be retrieved.
// If the extended info is a list, then the value of property prop for index PropertyIndex 
// is provided in extpropData, the size of which is specified with extpropSize.  
// If PropertyIndex = 0xffffffff, the whole list is written to extpropData.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// NOTES: 
//       An error will result if the media kind of the currently mounted volume is CD-R/RW.
//       Not all properties support additional property information.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the volume has not been formatted/mounted
//  AS_StorageError_BufferOverflow   - if the provided buffer length (propSize) is less than required for this property
//
AS_StorageError AS_API AS_File_GetProperty(
   const AS_File & file,				// IN:  open AS_File reference to get property on
   const AS_File::Property & prop,		// IN:  which property to get (see Prop_...)
   UInt32 bufLen,						// IN:  size available in propData buffer
   void * propData,						// OUT: buffer to copy property value into
   UInt32 * propSize,					// OUT: size of data returned in propData
   UInt32 PropertyIndex = 0,			// IN (Optional):  index of list of property returned
   UInt32 extbufLen = 0,				// IN (Optional): size available in extpropData buffer
   void*  extpropData = 0,				// IN/OUT (Optional): extended buffer to copy 
                                        //                    property value into
   UInt32* extpropSize = 0);			// OUT (Optional): size of data returned in extpropData 

//-----------------------------------------------------------------------------
// AS_File_SetProperty
//
// DESCRIPTION:
// AS_File_SetProperty sets the value of a property of a file. The file, which 
// must already be open (see AS_Volume_OpenFile), is identified by the reference
// (AS_File struct) specified with the parameter file. To set property information
// for a file using that file's path instead, see AS_Volume_SetEntryProperty.

// The parameter prop indicates the property (end-of-file, state, date modified,
// date created, etc.) for which to set the value. The value itself is given
// in the buffer specified with propData, the size of which is specified with propSize.
//
// The extended optional parameters are provided for additional property data to be set.
// If the extended info is a list, then the value of property prop for index PropertyIndex 
// is given in extpropData, the size of which is specified with extbufLen.  
// If PropertyIndex = 0xffffffff, the whole list is set from extpropData.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// NOTES: 
//       An error will result if the media kind of the currently mounted volume is CD-R/RW.
//       An error will result if you attempt to set the size of the entry (value of prop 
//       given as AS_File::Prop_Size = 2).
//       If the value of prop is AS_File::Prop_Name = 1, an error will result 
//       unless the file path given with propData indicates the same directory as the 
//       path of the file referenced with file.
//       Not all properties support additional property information.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the volume has not been formatted/mounted
//  AS_StorageError_BufferOverflow   - if the provided buffer length (propSize) is less than required for this property
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//
AS_StorageError AS_API AS_File_SetProperty(
   const AS_File & file,				// IN: open file to set property on
   const AS_File::Property & prop,		// IN: which property to set (see Prop_...)
   UInt32 propSize,						// IN: size of data in propData buffer
   void * propData,						// IN: property data
   UInt32 PropertyIndex = 0,			// IN (Optional):  index of list of property returned
   UInt32 extbufLen = 0,				// IN (Optional): size available in extpropData buffer
   void*  extpropData = 0);				// IN/OUT (Optional): extended buffer to copy
                                        //                    property value into

#ifdef __cplusplus
}
#endif

#endif //__AS_File_h__
