///////////////////////////////////////////////////////////////////////////////
// AS_StorageDevice_Authentication.h - EmbC++ Version
// Copyright (c) 2001 - 2005, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////


#ifndef		__AS_StorageDevice_Authentication_h__
#define		__AS_StorageDevice_Authentication_h__


#ifdef __cplusplus
extern "C" 
{
#endif

typedef UInt32 AS_StorageDevice_KeyInfo_Type;

// These correlate to READ DISC STRUCTURE field named "Format" in the CDB.
// If a define is missing, caller may send the correct "Format" field.
const AS_StorageDevice_KeyInfo_Type kAS_StorageDevice_KeyInfo_Media_Copyright      = 1;
const AS_StorageDevice_KeyInfo_Type kAS_StorageDevice_KeyInfo_Media_Id             = 6;
const AS_StorageDevice_KeyInfo_Type kAS_StorageDevice_KeyInfo_Media_Block          = 7;
const AS_StorageDevice_KeyInfo_Type kAS_StorageDevice_KeyInfo_Media_Key            = 2;
const AS_StorageDevice_KeyInfo_Type kAS_StorageDevice_KeyInfo_Media_AACS_Vol_Id    = 0x80; // AACS Volume Identifier
const AS_StorageDevice_KeyInfo_Type kAS_StorageDevice_KeyInfo_Media_AACS_Serial_No = 0x81; // Pre-Recorded AACS Media Serial Number
const AS_StorageDevice_KeyInfo_Type kAS_StorageDevice_KeyInfo_Media_AACS_Media_Id  = 0x82; // AACS Media Identifier


//-----------------------------------------------------------------------------
// Send Key - Authentication purposes
//
AS_StorageError AS_API AS_StorageDevice_SendKey(                 // RETURNS: error number
    const AS_StorageDevice & device,                      // IN: device reference
	UInt32 SendKeyDataSize,                         // IN: size of SendKeyData
	UInt8* SendKeyData,                             // IN: your Key Data for the drive.
	UInt8  KeyFormat,                               // IN: proprietary to Authentication
	UInt8  AGID,                                    // IN: proprietary to Authentication
	UInt8  KeyClass = 0                             // IN: proprietary to Authentication
	);

//-----------------------------------------------------------------------------
// Report Key - Authentication purposes
//
AS_StorageError AS_API AS_StorageDevice_ReportKey(                // RETURNS: error number
    const AS_StorageDevice & device,                       // IN: device reference
	UInt32 ReportKeyDataSize,                       // IN: size of SendKeyData
	UInt8* ReportKeyData,                           // OUT: your Key Data for the drive.
	UInt32 Lba,                                     // IN: Logical Block Address for report key.
	UInt8  KeyFormat,                               // IN: proprietary to Authentication
	UInt8  AGID,                                    // IN: proprietary to Authentication
	UInt8  KeyClass = 0,                            // IN: proprietary to Authentication
	UInt8  BlockCount = 0                           // IN: proprietary to Authentication
	);


//-----------------------------------------------------------------------------
// Get Key Info - Authentication purposes, same as READ DVD STRUCTURE or READ DISC STRUCTURE.
//
AS_StorageError AS_API AS_StorageDevice_GetKeyInfo (              // RETURNS: error number
	const AS_StorageDevice        &device,                 // IN: device reference
	UInt32                 KeyDataSize,             // IN: size of "KeyData"
	void                  *KeyData,                 // OUT: key info read from drive.
	AS_StorageDevice_KeyInfo_Type Type,                    // IN: type of read dvd struct.
	UInt32                 Address,                 // IN: address or LBA, if any 
	UInt32                 AGID = 0,                // IN: AGID you used for authentication, if any
	UInt32                 LayerNum = 0,            // IN: Layer number to read disc structure from.
	UInt32                 Reserved1 = 0, 
	UInt32                 Reserved2 = 0,
	UInt32                 Reserved3 = 0);

#ifdef __cplusplus
}
#endif

#endif //__AS_StorageDevice_Authentication_h__
