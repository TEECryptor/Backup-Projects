///////////////////////////////////////////////////////////////////////////////
// AS_Format.h - ATGS Team
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
//
// PPG Notes
// Always create VIDEO_TS / HDDVD_TS folders, then add them to a corresponding Layout's volume
//
// AS_Format allows the creation of known logical formats destined for a variety of targets
//
// Who   Date     What
// GK 02-14-05 Initial
///////////////////////////////////////////////////////////////////////////////

#ifndef _AS_FormatH
#define _AS_FormatH

#include "AS_StorageError.h"
#include "AS_StorageDevice.h"
#include "AS_Volume.h"

typedef AS_String AS_URL;
typedef AS_String AS_PlanProject;      // XML Project File (Unicode)

struct AS_Format : public AS_Volume 
{
   // Properties of AS_Format Objects
   enum Property
   {
      Prop_Project          = 1,        // OUT: Flattened Layout Object as XML Unicode Text
      Prop_SourceVolume     = 2,        // IN/OUT: AS_Volume specifies source volume
      Prop_SourceAESKey     = 3,        // IN/OUT: specify 128 bits AES key which source is encrypted with (VOBs only).

      // Multi-Layer Properties
      Prop_Layer_Base       = 0x0100,
      Prop_LayerBlocks      = Prop_Layer_Base + 1, // OUT: UInt32 For multi-layer formats, the number of 
                                                   //      blocks in a layer (for multi-layer only after _Prepare())
                                                   // propertyIndex = layer number
      Prop_LogicalLayerBrk  = Prop_Layer_Base + 2, // IN/OUT: LogicalLayerBreak struct, specifies the byte offset
                                                   //         into a file where the LB should be
                                                   //         Call AS_Format_GetLayerBreakRange() to get range of 
                                                   //         valid LogicalLayerBreaks

      // CD-DA Specific Properties
      Prop_Audio_Base       = 0x0200,
      // CD-Text strings are of type wchar_t unless otherwise specified.
      Prop_Audio_MediaCatalogNum = Prop_Audio_Base + 1,  // IN/OUT: String CD-Text Media Catalog Number
      Prop_Audio_Title           = Prop_Audio_Base + 2,  // IN/OUT: String CD-Text Title
      Prop_Audio_Performer       = Prop_Audio_Base + 3,  // IN/OUT: String CD-Text Performer
      Prop_Audio_Songwriter      = Prop_Audio_Base + 4,  // IN/OUT: String CD-Text Songwriter
      Prop_Audio_Composer        = Prop_Audio_Base + 5,  // IN/OUT: String CD-Text Composer
      Prop_Audio_Arranger        = Prop_Audio_Base + 6,  // IN/OUT: String CD-Text Arranger
      Prop_Audio_Message         = Prop_Audio_Base + 7,  // IN/OUT: String CD-Text Message
      Prop_Audio_Genre           = Prop_Audio_Base + 8,  // IN/OUT: String CD-Text Genre
      Prop_Audio_Genre_Code      = Prop_Audio_Base + 9,  // IN/OUT: String CD-Text Genre Code
      Prop_Audio_NumLanguages    = Prop_Audio_Base + 10, // OUT: UInt32 Number of languages with CD-Text
      Prop_Audio_Languages       = Prop_Audio_Base + 11, // OUT: UInt32[] Array of CD-Text language codes. See CDTextLanguageType below.

      // VCD Specific Properties (reserved for future properties)
      Prop_VCD_Base         = 0x0300,

      // DVD-V Specific Properties (reserved for future properties)
      Prop_DVD_Base         = 0x0400,

      Prop_Protection_Base  = 0x550,                     //        special device target modes
      Prop_Protection_Type  = Prop_Protection_Base + 1,  //  IN/OUT:  UInt32 specifying protection type 

      // CSS Specific Properties
      Prop_CSS_Base         = 0x0500,                        // For DDP2 Targets ( CPR_MAI)
      Prop_CSS_KeySet       = Prop_CSS_Base + 1,  // IN: CSSKeyStruct for setting CSS keys
      Prop_CSS_TitleKeys    = Prop_CSS_Base + 2,  // IN: TitleKeySet for setting keys per title set

      // AACS Specific Properties
      Prop_AACS_Base        = 0x0600,      // For DDP3 Targets?

      // DDP/CMF General Properties
      Prop_DDP_Base         = 0x0700,
      Prop_DDP_MasterID    = Prop_DDP_Base + 1, // IN/OUT: In as AS_String containing ASCII characters, 
                                                //         out as null-terminated UTF8 sequence of bytes
      Prop_DDP_NumLayers   = Prop_DDP_Base + 2, // IN/OUT: UInt32 Number of layers on final disc
      Prop_DDP_NumSides    = Prop_DDP_Base + 3, // IN/OUT: UInt32 Number of sides on final disc (1 or 2)
      Prop_DDP_CurrentSide = Prop_DDP_Base + 4, // IN/OUT: UInt32 Current side of final disc (1 or 2)
      Prop_DDP_TrackPath   = Prop_DDP_Base + 5, // IN/OUT: UInt32 Direction of translation (only valid on dual-layer)
                                                //    0 = Opposite Track Path DVD-ROM Being requested
                                                //    1 = Parallel Track Path DVD-ROM being requested
      Prop_DDP_DiscSize    = Prop_DDP_Base + 6, // IN/OUT: UInt32 Size in cm of disc to be mastered (8 or 12)
      Prop_DDP_StorageMode = Prop_DDP_Base + 7, // IN/OUT: UInt32 Source Storage Mode
                                                //    0 = User data only, 2048 bytes
                                                //    1 = Complete 2054 bytes
                                                //    6 = (currently not supported) Incomplete 2064 bytes
                                                //    7 = (currently not supported) Complete 2064 bytes
      Prop_DDP_Span_AS_StorageDevice = Prop_DDP_Base + 8, // IN: AS_StorageDevice to span to 2nd DLT device for 2nd layer
      Prop_DDP_ChecksumFile = Prop_DDP_Base + 9, // IN/OUT: UInt32 indicates whether to write ImageIntegrity Checksum File
                                                 //   0 = Do not write checksum file
                                                 //   1 = Write checksum file (default)
      Prop_DDP_InputImage = Prop_DDP_Base + 10, // IN: AS_String containing full path of existing image file to be used in DDP(or CMF) file set
                                                //    This is optional and is only to be used when an image already exists, that the client
                                                //    wants to create a DDP (or CMF) set from. (image assumed to be only user data 2048 bytes per sector)

      // DDP2 Specific Properties
      Prop_DDP2_Base        = 0x0800,
      Prop_DDP2_SscrStatus  = Prop_DDP2_Base + 1,  // IN/OUT: UInt32 Security Scrambling Status
                                                   //    0 = The data for the disc contains no security scrambling. The final disc will not be scrambled for security.
                                                   //    1 = The data for the disc contains no security scrambling. The final disc will be scrambled for security.
                                                   //    2 = (currently not supported) The data for the disc contains security scrambling. The final disc will be scrambled for security.
                                                   //    3 = The data for the disc contains no security scrambling. The security scrambling keys are read from a floppy disk. Format the floppy according to CSS Site Guide Book. The final disc will be scrambled for security.

      // DDP3 Specific Properties (also applies to HD CMF)
      Prop_DDP3_Base             = 0x0900,      // DDP3 Properties
      Prop_DDP3_Title            = Prop_DDP3_Base + 1,   // IN: AS_String containing the title of the disc image contents
      Prop_DDP3_Author           = Prop_DDP3_Base + 2,   // IN: AS_String containing the name and version of the authoring system used to create the disc image
      Prop_DDP3_CopyrightNotice  = Prop_DDP3_Base + 3,   // IN: AS_String containing the copyright notice for the disc image contents
      Prop_DDP3_Abstract         = Prop_DDP3_Base + 4,   // IN: AS_String containing a description of the disc image contents
      Prop_DDP3_DiscID           = Prop_DDP3_Base + 5,   // IN: AS_String containing a GUID to uniquely identify the disc
      Prop_DDP3_MediaType        = Prop_DDP3_Base + 6,   // IN: UInt32 indicates the type of disc being described by the DDP file set
                                                         //    0 = HD DVD-ROM (default)
                                                         //    1 = HD DVD-ROM Twin Format - indicates that layer 0 will contain a Standard Density DVD image
                                                         //        and current DDP set is single layer that will go on layer 1 of destination disc
                                                         //    2 = 3X-SPEED DVD-ROM
      Prop_DDP3_CopyProtectionInfo = Prop_DDP3_Base + 7, // IN: AS_String containing entire CP node of discinfo.dat XML file

      // BD CMF Specific Properties
      Prop_BDCMF_Base               = 0x0A00,               // BD CMF Properties
      Prop_BDCMF_BD_Type            = Prop_BDCMF_Base + 1,  // IN: UInt32 indicates BD type
                                                            //    0 = BD (default)
                                                            //    1 = BD9 (BD content on DL DVD-ROM)
      Prop_BDCMF_HybridType         = Prop_BDCMF_Base + 2,  // IN: UInt32 indicates hybrid type
                                                            //    0 = no hybrid (default)
                                                            //    1 = BD with CD hybrid
                                                            //    2 = BD with DVD hybrid
      Prop_BDCMF_SelectionID        = Prop_BDCMF_Base + 3,  // IN: AS_String containing the optional SelectionID (256 characters max)
      Prop_BDCMF_TitleName          = Prop_BDCMF_Base + 4,  // IN: AS_String containing the title of the disc, such as movie title (256 characters max)
      Prop_BDCMF_ContentOwner       = Prop_BDCMF_Base + 5,  // IN: AS_String containing the name of the content owner (256 characters max)
      Prop_BDCMF_Category           = Prop_BDCMF_Base + 6,  // IN: UInt32 indicates category type
                                                            //    0 = BD-ROM Movie Media (default)
                                                            //    1 = BD-ROM Non-Commercial Audiovisual Media
                                                            //    2 = BD-ROM Data Media
      Prop_BDCMF_AuthorStudio       = Prop_BDCMF_Base + 7,  // IN: AS_String containing the name of the authoring studio (256 characters max)
      Prop_BDCMF_AuthoringSoftware  = Prop_BDCMF_Base + 8,  // IN: AS_String containing the name of the authoring software (64 characters max)
      Prop_BDCMF_AuthoringVersion   = Prop_BDCMF_Base + 9,  // IN: AS_String containing the version number of the authoring software (64 characters max)
      Prop_BDCMF_RegionControl      = Prop_BDCMF_Base + 10, // IN: UInt32 indicates region control status
                                                            //    0 = Allowed (default)
                                                            //    1 = Limited
                                                            //    2 = Prohibited
                                                            // PropertyIndex parameter indicates which Region
                                                            //    0 = Region A (default)
                                                            //    1 = Region B
                                                            //    2 = Region C
                                                            // These regions must all be set or none. By setting one of them (i.e., A), 
                                                            // the others (i.e., B and C) will all be all be set to the default (Allowed) 
                                                            // until set explicitly.

      // MovieDisc Specific Properties
      Prop_MD_Base               = 0x1000,               // MovieDisc Properties
      Prop_MD_TrailerReplaceInfo = Prop_MD_Base + 1,     // IN/OUT: TrailerReplaceInfo struct, specifies source path, and optional index and additional source folder path for title (trailer) replacement
      Prop_MD_TargetStartSector  = Prop_MD_Base + 2,     // IN/OUT: UInt32 specifying logical block offset to move layout on target
   };

   // Volume state returned from GetProperty
   typedef UInt32 State;
   static const State IsDirty        = (1 << 0);
   static const State IsAppendable   = (1 << 1);

   // Logical Layer Break passed to SetProperty(Prop_LogicalLayerBrk)
   // specifies a file and byte offset into file where the layer break will be
   struct LogicalLayerBreak
   {
      AS_String fileName;  // file on destination that will span the layer break
      UInt64 byteOffset;   // exact byte offset into file where layer break will be
   };

   //  Title Key set
   struct TitleKeySet
   {
      SInt32 titleIndex;
      UInt8 keyInfo[5];
      UInt8 uKeyInfo[5];
   };

   // TrailerReplaceInfo passed to SetProperty(Prop_MD_TrailerReplaceInfo)
   // specifies a specific VTS title set (trailer) and the source folder path of the replacement trailer set.
   // If no replacement is needed, the value of replaceIndex = 0.
   // The value of replaceIndex = -1 for last title, -2 for 2nd from last, etc.
   struct TrailerReplaceInfo
   {
      SInt32 replaceIndex;
      AS_String replacementFolder;
   };

   //  Title Key set
   struct CSSKeyStruct {
      UInt8 ACC; // lower 5 bits are ACC
      UInt8* discKeyBlock; // must be 2048 bytes
      UInt8* titleKeyBlock; // must be 2048 bytes
   };

   // CD-Text language support. Used as ExtendedInfo (see AS_Format_(Get/Set)Property)
   typedef UInt32 CDTextLanguageType;
   static const CDTextLanguageType CDTextLangGerman   = 0x08;
   static const CDTextLanguageType CDTextLangEnglish  = 0x09;
   static const CDTextLanguageType CDTextLangSpanish  = 0x0a;
   static const CDTextLanguageType CDTextLangFrench   = 0x0f;
   static const CDTextLanguageType CDTextLangItalian  = 0x15;
   static const CDTextLanguageType CDTextLangDutch    = 0x1d;
   static const CDTextLanguageType CDTextLangRussian  = 0x56;
   static const CDTextLanguageType CDTextLangKorean   = 0x65;
   static const CDTextLanguageType CDTextLangJapanese = 0x69;
   static const CDTextLanguageType CDTextLangChinese  = 0x75;
};


#ifdef __cplusplus
extern "C" {
#endif

//
// Basic Layout API functions
//

// Open a volume on a device and determine the logical format
AS_StorageError AS_API AS_StorageDevice_OpenFormat( // RETURNS: error number
   const AS_StorageDevice& device,     // IN:  A reference to the AS_StorageDevice struct specifying the device containing the volume to open.
   AS_Volume::FS_Type& kind,           // OUT: A reference to the AS_Volume::FS_Type object that is to be filled in with the kind of volume detected.
   AS_Volume::InfoCallback callback,   // IN:  progress callback function
   void* callerUse,                    // IN:  progress callback userdata
   AS_Format& format);                 // OUT: AS_Format reference

// Free the internal volume object - all changes lost if not Flushed first
AS_StorageError AS_API AS_Format_Close( // RETURNS: error number
   AS_Format& layout);                  // IN: layout reference

// Create a new AS_Format layout
AS_StorageError AS_API AS_Format_Create(
   const AS_Format& format,                  // IN: reference to AS_Format handle
   const AS_Volume::FS_Type& fsKind,         // IN: kind of file system to write
   const AS_Volume::Format_Type& formatKind, // IN: kind of format desired
   const AS_File::Path & name,               // IN: Name to write as volume label
   const AS_Volume::AccessFlag & access,     // IN: must be either Mastering or RTTD
   AS_Volume::InfoCallback callback,         // IN: progress callback function
   void* callerUse);                         // IN: progress callback userdata

// Get/Set Volume Properties - see AS_Volume::Prop_ ... constants
AS_StorageError AS_API AS_Format_GetProperty(
   const AS_Format& format,         // IN: volume to get property on
   const AS_Format::Property& prop, // IN: which property to get (see AS_Format::Prop_...)
   UInt32 bufLen,                   // IN: size available in propData buffer
   void* propData,                  // OUT: buffer to copy property value into
   UInt32* propSize,                // OUT: size of data returned in propData
   UInt32 PropertyIndex=0,          // IN: index of list of property returned
   UInt32 extbufLen=0,              // IN: size available in extpropData buffer
   void*  extpropData=0,            // IN/OUT: extended buffer to copy property value into
   UInt32* extpropSize=0);          // OUT: size of data returned in extpropData 

AS_StorageError AS_API AS_Format_SetProperty(
   const AS_Format& layout,          // IN: volume to set property on
   const AS_Format::Property& prop,  // IN: which property to set (see AS_Volume::Prop_...)
   UInt32 propSize,                  // IN: size of data in propData buffer
   void* propData,                   // IN: property data
   UInt32 PropertyIndex=0,           // IN:  index of list of property to set
   UInt32 extpropSize=0,             // IN: size available in extpropData buffer
   void*  extpropData=0);            // IN/OUT: extended buffer to copy property value into

// Find valid range of layer breaks for passing into 
// AS_Format_SetProperty(Prop_LogicalLayerBrk)
// applies to Type_DVD and Type_HDDVD
// (for example, lowest valid layer break location might be at
// byte offset 32768 into file VTS_02_1.VOB)
AS_StorageError AS_API AS_Format_GetLayerBreakRange(
   const AS_Format& format,            // IN: reference to AS_Format handle
   AS_File::Path& lowerLimitFilePath,  // OUT: full path of lower limit file
   UInt32& lowerLimitByteOffset,       // OUT: byte offset into lowerLimitFilePath
   AS_File::Path& upperLimitFilePath,  // OUT: full path of upper limit file
   UInt32& upperLimitByteOffset);      // OUT: byte offset into upperLimitFilePath

//
// Get/Set Layout XML
//

// Create a new Format from an XML Project (XML file provides target selection)
AS_StorageError AS_API AS_OpenFormatProject( // RETURNS: error number
   const AS_PlanProject& project,            // IN: project text
   AS_Format& layout);                       // OUT: layout reference

// Set Properties for Format from an XML Project
AS_StorageError AS_API AS_Format_SetProjectProperties( // RETURNS: error number
   const AS_Format& layout,                            // IN: layout reference
   const AS_PlanProject& projectInfo);                 // IN: project text

// Save current Format to an XML Project
AS_StorageError AS_API AS_Format_SaveProject( // RETURNS: error number
   const AS_Format& layout,                   // IN: layout reference
   const AS_PlanProject& projectInfo);        // IN: project text


#ifdef __cplusplus
}
#endif
#endif //_AS_FormatH
