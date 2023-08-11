///////////////////////////////////////////////////////////////////////////////
// AS_Volume.h
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_Volume_h__
#define __AS_Volume_h__

#include "AS_StorageError.h"
#include "AS_File.h"
#include "AS_StorageDevice.h"


const SInt8 AS_Volume_Path_Delimiter = '/';


struct AS_Volume
{
   typedef UInt32 Handle; // Handles to internal objects
   // For AS_Volume_GetProperty & AS_Volume_SetProperty
   enum Property
   {
      Prop_FS_Type       = 1,    // OUT: AS_Volume::FS_Type
      Prop_Format_Type   = 2,    // OUT: AS_Volume::Format_Type detected existing on the mounted media
      Prop_State         = 3,    // OUT:    AS_Volume_State - return error if not existent
      Prop_Name          = 4,    // IN/OUT: Volume name IN as AS_File::Path, OUT as null-terminated UTF8 sequence of bytes
      Prop_BlockSize     = 5,    // OUT:    Block size of the volume in a 4byte UInt32
      Prop_FreeBlocks    = 6,    // OUT:    Volume free blocks in a 4byte UInt32
      Prop_DateModified  = 7,    // IN/OUT: Modification date of volume as AS_Volume::DateTime
      Prop_DateCreated   = 8,    // IN/OUT: Creation date of volume as AS_Volume::DateTime
      Prop_Delimiter     = 9,    // IN/OUT: Delimiter used for paths on media as SInt8
      Prop_ApplicationID = 10,   // IN/OUT: Application ID IN as AS_File::Path, OUT as null-terminated UTF8 sequence of bytes
      Prop_FastWrite     = 11,   // IN:     Write data in fast write mode (no read-after-write) as UInt32 0 = off, 1 = on
      Prop_NumFiles      = 12,   // OUT: UInt32 number of files currently on volume
      Prop_NumFolders    = 13,   // OUT: UInt32 number of folders currently on volume
      Prop_Protection    = 14,   // OUT: AS_Volume::Content_Protection_Type
      Prop_EstimatedFreeBlocks = 15, // OUT:  Volume estimated free blocks as 4byte UInt32
      Prop_EstimatedUsedBlocks = 16, // OUT:  Volume estimated used blocks as 4byte UInt32
      Prop_SessionToMount  = 17, // IN: UInt32 write-once session to mount, 1 based, 0 = load last recorded session
      Prop_VNRWrite        = 18, // IN: Write data using VerifyNotRequired mode.
      Prop_ContinueOnFileReadErrors = 19, // IN: Continue writing if source file is unreadable.
      Prop_RESERVED_01     = 256 // RESERVED FOR INTERNAL USE ONLY
   };

   // Volume format returned from AS_StorageDevice_OpenVolume or given to AS_Volume_Create
   typedef UInt32 FS_Type;

   static const FS_Type FS_None             = 0;         // OUT:    No existing file system
   static const FS_Type FS_Default          = (1 << 0);  // IN:     No file system specified. Use default.
   static const FS_Type FS_ISO_Level_1      = (1 << 1);  // IN/OUT: ISO 9660 Level 1
   static const FS_Type FS_ISO_Level_2      = (1 << 2);  // IN/OUT: ISO 9660 Level 2
   static const FS_Type FS_ISO_Level_2_Long = (1 << 3);  // IN/OUT: ISO 9660 Level 2 with long names and any valid ANSI char
   static const FS_Type FS_ISO_Versionless  = (1 << 4);  // IN/OUT: ISO 9660 No Version
   static const FS_Type FS_Joliet           = (1 << 5);  // IN/OUT: Joliet
   static const FS_Type FS_UDF102           = (1 << 10); // IN/OUT: UDF 1.02
   static const FS_Type FS_UDF15            = (1 << 11); // IN/OUT: UDF 1.5
   static const FS_Type FS_UDF2             = (1 << 12); // IN/OUT: UDF 2.0
   static const FS_Type FS_UDF25            = (1 << 13); // IN/OUT: UDF 2.5
   static const FS_Type FS_UDF26            = (1 << 14); // IN/OUT: UDF 2.6
   static const FS_Type FS_UDF201           = (1 << 15); // IN/OUT: UDF 2.01
   static const FS_Type FS_Audio            = (1 << 21); // IN/OUT: Audio
   static const FS_Type FS_TDF              = (1 << 25); // OUT:    custom interleaved format
   static const FS_Type FS_VRT              = (1 << 26); // OUT:    custom video format

   static const FS_Type FS_ISO              = FS_ISO_Level_1;
   static const FS_Type FS_Bridge           = FS_ISO | FS_UDF102;

   // High-level format types
   enum Format_Type
   {
      Type_Unknown   = 0,  // no known high-level format type detected
      Type_Data      = 1,  // Joliet/ISO on CD media, UDF 1.02/ISO on DVD media, UDF 2.0 on -RAM, UDF 2.5 on BD/HD, 2.6 on BDR
      Type_AudioCD   = 2,  // Audio CD AS_Format
      Type_VCD       = 3,  // VCD 2.0 AS_Format
      Type_SVCD      = 4,  // SVCD AS_Format
      Type_DVD       = 5,  // DVD-V AS_Format with UDF 1.02
      Type_DVDPlusVR = 6,  // NOT SUPPORTED
      Type_DVDDashVR = 7,  // NOT SUPPORTED
      Type_VRT       = 8,  // NOT SUPPORTED

      Type_BDAV      = 9,  // BDAV AS_Format (UDF 2.5 on BDRE or UDF 2.6 on BDR)
      Type_BDMV      = 10, // BDMV AS_Format (UDF 2.5 on BDRE or UDF 2.6 on BDR)
      Type_HDDVD     = 11, // HD DVD-V AS_Format with UDF 2.5

      Type_DDP20     = 12, // VIDEO_TS in an image file + DDP 2.0 files
      Type_DDP21     = 13, // NOT SUPPORTED
      Type_DDP30     = 14, // HD DVD-V in an image file + DDP 3.0 files
      Type_CMF20_HD  = 15, // HD DVD-V in an image file + CMF 2.0 files
      Type_CMF10_BD  = 16, // BDMV in an image file + BDCMF 1.0 file set

      // Aliases
      Type_MP3       = Type_Data // MP3 CD
   };

   enum Content_Protection_Type
   {
      Protection_None  = 0,
      Protection_CSS   = 1,
      Protection_AACS  = 2
   };

   // Volume access type passed into AS_Volume_Mount and AS_Volume_Create
   typedef UInt32 AccessFlag;

   static const AccessFlag ReadOnly  = (1 << 0); // IN: for AS_Volume_Mount; writing not allowed
   static const AccessFlag Mastering = (1 << 1); // IN: for AS_Volume_Create, AS_Volume_Mount; AS_File_Write not allowed
   static const AccessFlag RTTD      = (1 << 2); // IN: for AS_Volume_Create, AS_Volume_Mount; real-time to disc, allows AS_File_Write

   // Volume state returned from AS_Volume_GetProperty
   typedef UInt32 State;

   static const State File_IsMounted    = (1 << 0);
   static const State File_IsDirty      = (1 << 1);
   static const State File_IsAppendable = (1 << 2);

   // Border options passed into AS_Volume_Flush
   enum Flush_Border_Options 
   {
      // caller is REQUIRED to specify one of these border options:
      Border_Open  = 1, // does not write any border
      Border_Close = 2, // writes border and leaves disc appendable
      Border_Final = 3  // finalizes write-once media (making it read-only)
   };

   // Flags passed into AS_Volume_Flush.
   typedef UInt32 Flush_Flags;

   static const Flush_Flags Flush_Verify = (1 << 0); // does verify after write

   // Boot image flag used in AS_Volume_SetBootable.
   enum BootType
   {
      Boot_None        = 0,
      Boot_Floppy12    = 1,                  // for 1.2 floppy images
      Boot_Floppy144   = 2,                  // for 1.44 floppy images
      Boot_Floppy288   = 3,                  // for 2.88 floppy images
      Boot_HD          = 4,                  // for Hard Drive images
      Boot_NoEmulation = 5,                  // for a special boot image
      Boot_NoEmulationWithBootInfoTable = 6, // for a special boot image; also writes Boot Info table
      Boot_LoadLast    = 7                   // load the previous boot image
   };

   Handle myHandle;

   // Function typedef for streaming callback function. 
   // Used in AS_Volume_CreateFileFromStream
   // This function will be called repeatedly until all file data has been read.
   typedef UInt32 (AS_CALLBACK *StreamCallback)(SInt64 pos, SInt8 * pBuffer, 
                                                SInt32 bytesRequested, 
                                                SInt32 * pBytesGiven, void * callerUse);

   // Defines parameters returned through the InfoCallback method (below).
   enum Info
   {
      Info_ProgressUpdate         = 1, // infoDword = percent complete, infoPtr = Progress structure
      Info_PromptForMediaInDevice = 2, // infoDword = unused, infoPtr = unused
      Info_PromptForNextMedia     = 3  // infoDword = unused, infoPtr = unused
   };

   // Describes the type of progress being reported for Info_ProgressUpdate.
   typedef UInt32 ProgressType;

   static const ProgressType ProgressType_General = 0;
   static const ProgressType ProgressType_Format  = 1;
   static const ProgressType ProgressType_Mount   = 2;
   static const ProgressType ProgressType_Plan    = 3;
   static const ProgressType ProgressType_Write   = 4;
   static const ProgressType ProgressType_Close   = 5;
   static const ProgressType ProgressType_Verify  = 6;

   // additional progress info (beyond percent complete)
   struct Progress
   {
      Progress() : ValidBytes(sizeof(Progress)), CurrentSector(0), TotalSectors(0),
       ProgressType(ProgressType_General), ProgressStatus(0), DeviceHandle(0) {}
   
      UInt32 ValidBytes;      // size, in bytes, that follow that are valid
                              // so this structure can be expanded to return additional
                              // information in the future
      UInt32 CurrentSector;   // How many sectors have been processed (not the actual current
                              // sector number)
      UInt32 TotalSectors;    // Total number of sectors to process.
      UInt32 ProgressType;    // Type of progress being reported.
      UInt32 ProgressStatus;  // Job status (see AS_StorageDevice::JobStatus).
      UInt32 DeviceHandle;    // AS_StorageDevice handle, zero if none or N/A.
   };

   // Function typedef for reporting progress or other information
   // Used in AS_StorageDevice_OpenVolume, AS_Volume_Create, AS_Volume_CreateTDF,
   // AS_Volume_Mount and AS_Volume_Flush
   typedef AS_StorageError (AS_CALLBACK *InfoCallback)(const Info& info, 
                                                       UInt32 infoDword, void *infoPtr,
                                                       void *callerUse);

   typedef struct DateTime
   {
      UInt16      year;               // Year as an integer
      UInt8       month;              // Month from 1(Jan) to 12(Dec)
      UInt8       day;                // Day of month from 1 to 31
      UInt8       hour;               // Hour from 0 to 23
      UInt8       minute;             // Minute from 0 to 59
      UInt8       second;             // Second from 0 to 59
      SInt8       zone;               // Time offset from GMT in 30 minute increments
   };

   // Flags for AS_Volume_SetDevices. Currently reserved.
   typedef UInt32 SetDevicesFlags;
   // Flags for AS_BindFileToPath. Currently reserved
   typedef UInt32 BindFlags;
};

#ifdef __cplusplus
extern "C" {
#endif


//-----------------------------------------------------------------------------
// Volume API functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AS_StorageDevice_OpenVolume
//
// DESCRIPTION:
// Opens a volume, creating a data structure in RAM that is ready to receive file 
// structure information from the media in the specified device. The volume's 
// logical format (see AS_Volume_FS_Type) is output with the parameter kind 
// (if the media is blank the kind will be AS_Volume::FS_None). A reference 
// (AS_Volume struct) for the volume is output with volume, which may be used to 
// refer to the volume in subsequent calls.
//
// A callback with which the AuthorScript engine will report on the progress of 
// the operation is specified with callback. The parameter userdata allows the 
// host application to specify a pointer to a memory location that will be passed 
// back whenever AuthorScript calls the callback.
//
// NOTES: 
//      An error (AS_Error_NotSupported = 38) will result if the format of the volume is not supported by your version of AuthorScript Imaging. Please contact your Sonic representative if you have any questions about which formats are covered.
//      An error (AS_Error_NotSupported = 38) will result if the volume contains any sectors in Data Mode 2 format.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_DeviceNotReady   - if there is no media recognized in device
//
AS_StorageError AS_API AS_StorageDevice_OpenVolume(
   const AS_StorageDevice& device,     // IN:  A reference to the AS_StorageDevice struct specifying the device containing the volume to open.
   AS_Volume::FS_Type& kind,           // OUT: A reference to the AS_Volume::FS_Type object that is to be filled in with the kind of volume detected.
   AS_Volume::InfoCallback callback,   // IN:  progress callback function
   void* userdata,                     // IN:  progress callback userdata
   AS_Volume& volume);						// OUT: AS_Volume reference

//-----------------------------------------------------------------------------
// AS_Volume_Close
//
// DESCRIPTION:
// Releases the specified volume object and all associated memory.
//
// NOTES: 
//       To save changes before releasing the volume, first call AS_Volume_Flush.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//
AS_StorageError AS_API AS_Volume_Close(
   AS_Volume & volume);                                // IN/OUT:  volume reference

//-----------------------------------------------------------------------------
// AS_Volume_Mount
//
// DESCRIPTION:
// Mounts the specified volume, reading file structure information from the
// the volume into the data structure created in RAM when the volume was opened 
// with AS_StorageDevice_OpenVolume. Once the volume is mounted, changes (add/delete files, folders, etc.) 
// may be made to the structure in memory. (To write those changes back to the file system on the 
// actual disc, see AS_Volume_Flush.) The parameter access determines what limitations, if any, 
// will be placed on writing to the mounted volume; for details see AS_Volume::AccessFlag. 
//
// A callback with which the AuthorScript engine will report on the progress of the mount operation 
// is specified with callback. The parameter userdata allows the host application to specify a pointer 
// to a memory location that will be passed back whenever AuthorScript calls the callback.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidAccess    - results if more than one flag is passed for AccessFlag
//  AS_StorageError_InvalidOperation - results if volume has already been mounted
//  AS_StorageError_Fatal            - if filesystem cannot be mounted
//
AS_StorageError AS_API AS_Volume_Mount(
   const AS_Volume & volume,                           // IN: volume reference
   const AS_Volume::AccessFlag & access,               // IN: must be either ReadOnly, Mastering, or RTTD
   AS_Volume::InfoCallback callback,                   // IN: progress callback function
   void* userdata);                                    // IN: progress callback userdata

//-----------------------------------------------------------------------------
// AS_Volume_Create
//
// DESCRIPTION:
// Mounts the specified volume and formats it in the format (ISO, UDF 1.02, UDF 2.0, etc.) 
// specified with the parameter kind. A name for the newly-formatted volume is specified with name. 
// Limitations, if any, on writing to the formatted volume are specified with access; for details 
// see AS_Volume::AccessFlag. 
//
// A callback with which the AuthorScript engine will report on the progress of the operation is 
// specified with callback. The parameter userdata allows the host application to specify a pointer 
// to a memory location that will be passed back whenever AuthorScript calls the callback.
//
// NOTES: 
//       An error will result if kind specifies AS_Volume::Format_TDF.
//       To determine if the media is blank before formatting, see AS_StorageDevice_GetDeviceProperty.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidAccess    - results if more than one flag is passed for AccessFlag
//  AS_StorageError_InvalidOperation - results if volume has already been mounted
//  AS_StorageError_Fatal            - if filesystem cannot be formatted
//
AS_StorageError AS_API AS_Volume_Create(
   const AS_Volume & volume,                           // IN: volume to flush the FS to disc
   const AS_Volume::FS_Type & kind,                    // IN: kind of volume to format
   const AS_File::Path & name,                         // IN: Name to write as volume label
   const AS_Volume::AccessFlag & access,               // IN: must be either Mastering or RTTD
   AS_Volume::InfoCallback callback,                   // IN: progress callback function
   void* userdata);                                    // IN: progress callback userdata

//-----------------------------------------------------------------------------
// AS_Volume_Prepare
// 
// DESCRIPTION:
// Plans the volume data and returns any errors related to the planning of 
// metadata on the media.
//
// This call is optional. It may be called prior to AS_Volume_Flush. If it is
// omitted, then the planning will be done during the AS_Volume_Flush call.
// If any changes are made to the volume after calling AS_Volume_Prepare, then
// the volume must be prepared again, either by calling AS_Volume_Prepare, or 
// else it will occur automatically during AS_Volume_Flush.
//
// The 'options' and 'flags' parameters passed into AS_Volume_Flush must be
// the same as the ones passed into AS_Volume_Prepare. If they are not, then
// AS_Volume_Flush will return AS_StorageErrorInvalidParameter.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined or if no FlushOptions are provided
//  AS_StorageError_InvalidOperation - if the volume was not mounted/formatted or if there are open AS_File references
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//  AS_StorageError_Fatal            - if the filesystem data and queued files cannot be written
//
AS_StorageError AS_API AS_Volume_Prepare( // RETURNS: Error - app may react and try to Prepare again
   const AS_Volume& volume,                           // IN: AS_Volume reference
   AS_Volume::InfoCallback callback,                  // IN: UI handler for UI needs during
   void *callerUse,                                   // IN: progress callback function
   const AS_Volume::Flush_Border_Options & options,   // IN: caller is REQUIRED to specify one of 
                                                      // these mutually exclusive border options:
                                                      //     Border_Open
                                                      //     Border_Close
                                                      //     Border_Final
   const AS_Volume::Flush_Flags& flags = 0);          // IN: flush flags

//-----------------------------------------------------------------------------
// AS_Volume_Flush
//
// DESCRIPTION:
// Writes file system data from RAM to the file system on the disc containing the specified volume. 
// A callback with which the AuthorScript engine will report on the progress of the operation 
// is specified with callback. The parameter userdata allows the host application to specify a pointer 
// to a memory location that will be passed back whenever AuthorScript calls the callback.
//
// The flush operation can either leave the volume border open, write a border and leave the disc 
// appendable, or finalize write-once media (making it read-only). The parameter options is used to 
// specify one of these mutually-exclusive border options. 
//
// If AS_Volume_Prepare was previously called, and no changes have been made to the volume since
// it was called, then the 'options' and 'flags' parameters must match those passed into 
// AS_Volume_Prepare, or else AS_Volume_Flush will return AS_StorageError_InvalidParameter.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined or if no FlushOptions are provided
//  AS_StorageError_InvalidOperation - if the volume was not mounted/formatted or if there are open AS_File references
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//  AS_StorageError_Fatal            - if the filesystem data and queued files cannot be written
//
AS_StorageError AS_API AS_Volume_Flush(
   const AS_Volume & volume,                           // IN: volume to flush the FS to disc
   AS_Volume::InfoCallback callback,                   // IN: progress callback function
   void* callerUse,                                    // IN: progress callback userdata
   const AS_Volume::Flush_Border_Options & options,    // IN: caller is REQUIRED to specify one of 
                                                       // these mutually exclusive border options:
                                                       //     Border_Open
                                                       //     Border_Close
                                                       //     Border_Final
   const AS_Volume::Flush_Flags& flags = 0);           // IN: flush flags
   
//-----------------------------------------------------------------------------
// AS_Volume_SetDevices
//
// DESCRIPTION:
// Assigns a list of devices to be used by the specified volume. Each device
// must contain media of the same type and be either blank or re-writable.
// Re-writable media will be overwritten with no warning. It is the
// responsibility of the host application to ensure that re-writable media is
// either 'blank' or can be safely overwritten.
//
// Only volumes mounted for create (AS_Volume_Create or AS_Format_Create) may
// use this method. Volumes mounted on media with pre-existing data
// (AS_Volume_Mount) may not as their filesystem data is not transferable to
// the new device.
//
// NOTES:
//       The 'infoPtr' parameter passed to the callback method (if specified)
//        will currently be NULL.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - one or more of the given parameters are invalid
//  AS_StorageError_InvalidOperation - if the volume was not originally mounted for create
//  AS_StorageError_DeviceNotReady   - if one or more of the devices do not have media
//  AS_StorageError_InvalidMediaType - if one or more of the media are not of the same type
//                                     or are either not blank or not re-writable
//
AS_StorageError AS_API AS_Volume_SetDevices(
   const AS_Volume & volume,             // IN: volume reference
   AS_Volume::SetDevicesFlags flags,     // IN: flags reserved for future use
   UInt32 numDevices,                    // IN: number of devices in 'deviceList'
   const AS_StorageDevice * deviceList,  // IN: array of AS_StorageDevice objects
   AS_Volume::InfoCallback callback,     // IN: progress callback function
   void * userdata);                     // IN: progress callback userdata

//-----------------------------------------------------------------------------
// AS_Volume_GetProperty
//
// DESCRIPTION:
// Retrieves the value of a property of the specified volume. The parameter prop indicates the 
// property (format, state, name, block size, free blocks, date modified, date created, etc.) 
// for which to retrieve the current value. The value is output to a buffer pointed to with propData, 
// the size of which is specified with bufLen. The actual size of the data written to the buffer 
// is output with propSize.
//
// The extended optional parameters are provided for additional property data to be retrieved.
// If the extended info is a list, then the value of property prop for index PropertyIndex 
// is provided in extpropData, the size of which is specified with extpropSize.  
// If PropertyIndex = 0xffffffff, the whole list is written to extpropData.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// NOTES:
//       Some properties require volume to be formatted or mounted before being available to query their value.
//       Not all properties support additional property information.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined or if the property cannot be retrieved
//  AS_StorageError_BufferOverflow   - if the provided buffer length (bufLen) is less than required for this property
//  AS_StorageError_InvalidOperation - if the property cannot be obtained in the current volume state
//
AS_StorageError AS_API AS_Volume_GetProperty(
   const AS_Volume & volume,                           // IN:  volume to get property on
   const AS_Volume::Property & prop,                   // IN:  which property to get (see Prop_...)
   UInt32 bufLen,                                      // IN:  size available in propData buffer
   void * propData,                                    // OUT: buffer to copy property value into
   UInt32 * propSize,                                  // OUT: size of data returned in propData
   UInt32 PropertyIndex = 0,				           // IN (Optional):  index of list of property returned
   UInt32 extbufLen = 0,					           // IN (Optional): size available in extpropData buffer
   void*  extpropData = 0,					           // IN/OUT (Optional): extended buffer to copy property value into
   UInt32* extpropSize = 0);				           // OUT (Optional): size of data returned in extpropData 

//-----------------------------------------------------------------------------
// AS_Volume_SetProperty
//
// DESCRIPTION:
// Sets the value of a property of the specified volume. The parameter prop indicates the property 
// (e.g. name, date modified, date created, path delimiter) for which the value is to be set. 
// The value itself is given in the buffer pointed to with propData, the size of which is specified with propSize.
//
// The extended optional parameters are provided for additional property data to be set.
// If the extended info is a list, then the value of property prop for index PropertyIndex 
// is given in extpropData, the size of which is specified with extbufLen.  
// If PropertyIndex = 0xffffffff, the whole list is set from extpropData.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
//
// NOTES: 
//       All properties require volume to be formatted or mounted before being available to set their value.
//       Not all properties support additional property information.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the volume has not been formatted/mounted
//  AS_StorageError_BufferOverflow   - if the provided buffer length (propSize) is less than required for this property
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//
AS_StorageError AS_API AS_Volume_SetProperty(
   const AS_Volume & volume,                           // IN: volume to set property on
   const AS_Volume::Property & prop,                   // IN: which property to set (see Prop_...)
   UInt32 propSize,                                    // IN: size of data in propData buffer
   void * propData,							           // IN: property data
   UInt32 PropertyIndex = 0,				           // IN (Optional):  index of list of property returned
   UInt32 extbufLen = 0,					           // IN (Optional): size available in extpropData buffer
   void*  extpropData = 0);					           // IN/OUT (Optional): extended buffer to copy property value into

//-----------------------------------------------------------------------------
// AS_Volume_SetBootable
//
// DESCRIPTION:
// Sets the volume to be bootable using a boot file.  The path to the boot file is specified
// with the parameter inBootFileOnHD, the parameter inBootType is used to determine what type
// of boot image is referenced by inBootFileOnHD.  If the inBootType is set to 
// AS_Volume::Boot_NoEmulationWithBootInfoTable, then the parameter for inBootFileSizeInBytes
// must be provided.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the volume has not been opened
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//
AS_StorageError AS_API AS_Volume_SetBootable(
   const AS_Volume & volume,                           // IN:  volume reference
   const AS_File::Path & inBootFileOnHD,               // IN:  full path of the boot file
   const AS_Volume::BootType &inBootType,              // IN:  Type of boot image 
   UInt64 inBootFileSizeInBytes);                      // IN:  (Only used in HD_EMUL_WITH_BOOT_INFO_TABLE)

//-----------------------------------------------------------------------------
// AS_Volume_GetEntryInfo
//
// DESCRIPTION:
// Gets information about a file system entry (file or directory) in the specified volume. The path 
// to the object is specified with the parameter path, while the parameter kind is used to report 
// back on whether the object at that path is a file or a directory. If the object is a file, its 
// size is output with size. If it is a directory, size is used to output the number of entries in that directory.
//
//
// NOTES: 
//       For more detailed information about an entry, use AS_Volume_GetEntryProperty (files or directories) 
//         or AS_File_GetProperty (files only).
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_FileNotFound     - if the path doesn't exist in the volume
//  AS_StorageError_InvalidOperation - if the volume has not been formatted / mounted
//
AS_StorageError AS_API AS_Volume_GetEntryInfo(
   const AS_Volume & volume,                           // IN:  volume reference
   const AS_File::Path & path,                         // IN:  path to directory to list or file to get size of
   AS_File::Kind & kind,                               // OUT: kind of object path refers to
   AS_File::Size & size);                              // OUT: number of children in directory or logical size of file


//-----------------------------------------------------------------------------
// AS_Volume_GetEntry
//
// DESCRIPTION:
// Retrieves the name of the file system object (file or directory) at a given index (zero-based) in 
// a directory on the specified volume. The path to the directory is specified with inFullPath, while 
// the target object's index within that directory is specified with idx (the value of which must be 
// less than the directory's size as output from AS_Volume_GetEntryInfo).
//
// The amount of memory to set aside for the retrieved name is specified with bufLen. If that memory 
// is not sufficient for the actual name at the specified location, the amount needed for the name 
// will be output with pathLen. The path and name itself will be output with outFullPath.            
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the volume has not been formatted / mounted
//  AS_StorageError_InvalidIndex     - if index is greater than count of items in directory
//  AS_StorageError_FolderNotFound   - if the directory path could not be found
//  
//
AS_StorageError AS_API AS_Volume_GetEntry(             // RETURNS: error number
   const AS_Volume & volume,                           // IN:  volume reference
   const AS_File::Path & infullPath,                   // IN:  path to directory to list
   SInt32 idx,                                         // IN:  Number of item to list
   SInt32 * pathLen,                                   // OUT: NULL or pointer to spot for true length of name (for times when > bufLen)
   AS_File::Path& outFullPath);                        // OUT: found file/directory item

//-----------------------------------------------------------------------------
// File Creation API
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AS_Volume_CreateEntry
//
// DESCRIPTION:
// Creates a file system object on the specified volume. The 
// kind of file system object to create (file or directory) is specified with the parameter 
// kind. The name of the object and the directory location at which it is to be created are 
// specified with fullPath. If the parents in this path do not already exist in the volume, 
// they may be created automatically by setting createParents to true.
//
// NOTES: 
//       An error will result if createParents is given as false and any parent indicated in
//       fullPath does not already exist.
//       If AS_Volume_OpenFile is called with the mode parameter set to AS_File::Append 
//       and the parents in the path specified with that call's fullPath parameter already exist, 
//       then a file will be created automatically without the use of this call.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the parent directory doesn't exist in fullPath and createParents = false
//  AS_StorageError_InvalidOperation - if the volume has not been formatted / mounted
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//  AS_StorageError_Fatal            - if entry could not be created
//
AS_StorageError AS_API AS_Volume_CreateEntry(          // RETURNS: error number
   const AS_Volume & volume,                           // IN: volume reference
   const AS_File::Path & fullPath,                     // IN: path to / name of file
   const AS_File::Kind & kind,                         // IN: kind of FS object to create (dir, file)
   const AS_File::CreateFlags& flags);                 // IN: create flags


//-----------------------------------------------------------------------------
// AS_Volume_CreateEntryFromHD
//
// DESCRIPTION:
// Creates an entry (file or directory) on the specified volume from a source entry 
// on a hard drive. The source entry is specified with fullPathOnHD. The name of the newly-created 
// entry and the directory location at which it is to be created are specified with the 
// parameter fullPath. Space is allocated when the entry is created, but the space remains 
// empty until the volume is flushed with AS_Volume_Flush, at which point the source file or 
// directory data is copied to disc.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the volume has not been formatted / mounted
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//  AS_StorageError_FileExists       - if file referenced by fullPath already exists
//  AS_StorageError_Fatal            - if entry could not be created
//
AS_StorageError AS_API AS_Volume_CreateEntryFromHD(
   const AS_Volume & volume,                           // IN: volume reference
   const AS_File::Path & fullPath,                     // IN: Name of file to create in directory
   const AS_File::Path & fullPathOnHD,                 // IN: path to hard disc file from which to take all information - not copied to media until Volume is Flushed.
   const AS_File::CreateFlags& flags);                 // IN: create flags


//-----------------------------------------------------------------------------
// AS_Volume_CreateFileFromStream
//
// DESCRIPTION:
// Creates a file on the specified volume from a stream. The name of the newly-created file and 
// the directory location at which it is to be created are specified with the parameter fullPath. 
//
// Space for the file is allocated based on the size specified with streamLength when the file is created, 
// but the space remains empty until the volume is flushed with AS_Volume_Flush, at which point the stream
// data is requested using the callback specified with callback. The parameter userdata allows the host
// application to specify a pointer to a memory location that will be passed back whenever AuthorScript calls the callback.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the volume has not been formatted / mounted
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//  AS_StorageError_FileExists       - if file referenced by fullPath already exists
//  AS_StorageError_Fatal            - if entry could not be created
//
AS_StorageError AS_API AS_Volume_CreateFileFromStream(
   const AS_Volume & volume,                           // IN: volume reference
   const AS_File::Path & fullPath,                     // IN: Name of file to create in directory
   const AS_File::Size & streamLength,                 // IN: Size of stream (and ultimately file on media)
   AS_Volume::StreamCallback callback,                 // IN: stream callback function for file data
   void * userdata,                                    // IN: stream callback userdata
   const AS_File::CreateFlags& flags);                 // IN: create flags


//-----------------------------------------------------------------------------
// AS_Volume_DeleteEntry
//
// DESCRIPTION:
// Deletes one or more file system objects on the specified volume. The specific action performed by the call 
// depends on the kind (file or directory) of the object and, if the object is a directory, whether or not 
// that directory is empty:
//
//      if object specified with the parameter fullPath is a file, that file alone is deleted; 
//      if the object is a directory (empty or not) and deleteChildren is passed as "true" then 
//        that directory and its entire contents are all deleted; 
//      if the object is an empty directory and deleteChildren is passed as "false" then the directory is deleted; 
//      if the object is a directory that is not empty and deleteChildren is passed as "false" then the call results in an error.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_InvalidOperation - if the volume has not been formatted / mounted
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//  AS_StorageError_FileNotFound     - if entry referenced by fullPath doesn't exists
//  AS_StorageError_Fatal            - if entry could not be created
//
AS_StorageError AS_API AS_Volume_DeleteEntry(
   const AS_Volume & volume,                           // IN: volume reference
   const AS_File::Path & fullPath,                     // IN: full path to file
   const AS_File::DeleteFlags& flags);                 // IN: delete flags


//-----------------------------------------------------------------------------
// AS_Volume_GetEntryProperty
//
// DESCRIPTION:
// Retrieves the value of a property of a file or directory on the specified volume. The file is identified 
// by its full path, specified with the parameter path. To retrieve property information for a file using 
// that file's reference (AS_File struct) instead, see AS_File_GetProperty.
//
// The parameter prop indicates the property (end-of-file, state, date modified, date created, etc.) for 
// which to retrieve the current value. The value is output to a buffer specified with propData, the size 
// of which is specified with bufLen. The actual size of the data written to the buffer is output with propSize.
//
// The extended optional parameters are provided for additional property data to be retrieved.
// If the extended info is a list, then the value of property prop for index PropertyIndex 
// is provided in extpropData, the size of which is specified with extpropSize.  
// If PropertyIndex = 0xffffffff, the whole list is written to extpropData.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// NOTES:
//       Not all properties support additional property information.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_BufferOverflow   - if the provided buffer length (bufLen) is less than required for this property
//
AS_StorageError AS_API AS_Volume_GetEntryProperty(
   const AS_Volume & volume,                           // IN:  volume reference
   const AS_File::Path & path,                         // IN:  path to file
   const AS_File::Property & prop,                     // IN:  which property to get (see AS_File::Prop_...)
   UInt32 bufLen,							           // IN:  size available in propData buffer
   void * propData,                                    // OUT: buffer to copy property value into
   UInt32 * propSize,                                  // OUT: size of data returned in propData
   UInt32 PropertyIndex = 0,				           // IN (Optional):  index of list of property returned
   UInt32 extbufLen = 0,					           // IN (Optional): size available in extpropData buffer
   void*  extpropData = 0,					           // IN/OUT (Optional): extended buffer to copy property value into
   UInt32* extpropSize = 0);				           // OUT (Optional): size of data returned in extpropData 

//-----------------------------------------------------------------------------
// AS_Volume_SetEntryProperty
//
// DESCRIPTION:
// Sets the value of a property of a file or directory on the specified volume. The file is identified by its 
// full path, specified with the parameter path. To set a property for a file using that file's reference 
// (AS_File struct) instead, see AS_File_GetProperty.
//
// The parameter prop indicates the property (end-of-file, state, date modified, date created, etc.) 
// for which to set the value. The value itself is given in the buffer specified with propData, the size 
// of which is specified with propSize.
//
// The extended optional parameters are provided for additional property data to be set.
// If the extended info is a list, then the value of property prop for index PropertyIndex 
// is given in extpropData, the size of which is specified with extbufLen.  
// If PropertyIndex = 0xffffffff, the whole list is set from extpropData.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// NOTES: 
//       An error will result if you attempt to set the size of the entry (value of prop given as AS_File::Prop_Size = 2).
//       If the value of prop is AS_File::Prop_Name = 1, an error will result unless the path given with propData indicates the same directory as the existing path given with path.
//       Not all properties support additional property information.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_BufferOverflow   - if the provided buffer length (propSize) is less than required for this property
//  AS_StorageError_InvalidAccess    - if the volume's AccessFlag was read-only
//
AS_StorageError AS_API AS_Volume_SetEntryProperty(
   const AS_Volume & volume,                           // IN: volume reference
   const AS_File::Path & path,                         // IN: path to file
   const AS_File::Property & prop,                     // IN: which property to set (see AS_File::Prop_...)
   UInt32 propSize,                                    // IN: size of data in propData buffer
   void * propData,							           // IN: property data
   UInt32 PropertyIndex = 0,				           // IN (Optional):  index of list of property returned
   UInt32 extbufLen = 0,					           // IN (Optional): size available in extpropData buffer
   void*  extpropData = 0);					           // IN/OUT (Optional): extended buffer to copy property value into

//-----------------------------------------------------------------------------
// AS_Volume_BindFileToPath
//
// DESCRIPTION:
// Bind a path on the volume to an existing file on the native OS file system. This is
// For use with BD-J (Blu-ray Java) virtual file system, in which a file appears to be
// on the optical disc, but is in fact redirected to a file on the computer's (or player's)
// file system. 
//
// If 'virtualPath' does not match an existing file on the optical disc, then
// an entry will be created for it. 
//
// NOTES: 
//       The volume must be mounted before calling this function.
//
//       Getting following properties of a bound file will return values for the file on 
//       the local operating system:
//
//            AS_File::Prop_Size
//            AS_File::Prop_DateModified
//            AS_File::Prop_DateCreated
//            AS_File::Prop_CurrentPos (You may also set this property on a bound file)
//
//       All other properties will refer to the file on the optical disc.
//
//       It is not legal to attempt to write to a bound file.
//
//
// RETURN:
// AS_StorageError_None             - ok
// AS_StorageError_InvalidParameter - volume not correct
// AS_StorageError_InvalidOperation - a number of cases:
//      - the volume is not mounted
//      - attempting to bind a folder rather than a file
//      - attempting to bind a file that is currently open
//
AS_StorageError AS_API AS_Volume_BindFileToPath(
   const AS_Volume & volume,           //IN: volume reference
   const AS_File::Path & virtualPath,  // IN: path of file in AS_Volume
   const AS_File::Path & actualPath,   // IN: path to file which virtualPath will be bound to
   const AS_Volume::BindFlags& flags); // reserved for future (UInt32)


#ifdef __cplusplus
}
#endif
#endif //__AS_Volume_h__
