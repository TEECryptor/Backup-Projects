///////////////////////////////////////////////////////////////////////////////
// AS_StorageDevice.h
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#ifndef		__AS_StorageDevice_h__
#define		__AS_StorageDevice_h__

#include "AS_StorageTypes.h"
#include "AS_StorageError.h"

#ifdef __cplusplus
extern "C" 
{
#endif

struct AS_StorageDevice
{

	typedef UInt32 Handle;

	typedef UInt32 Type;
	static const Type Type_Device    = 0;
	static const Type Type_File      = (1 << 0);

	static const Type Type_Folder    = (1 << 2);

	Handle    myHandle;             // handle to device
	UInt32    reserved;             // reserved for future use

	AS_String devicePath;           // path to file device
	AS_String deviceXMLOptionsPath; // optional XML file describing Qflix options or other options
	Type      deviceType;

	static const Handle LastUsedDevice = 0;

	// Type of tray control
	enum TrayControls
	{
		Tray_Control_Lock       = 0,			// prevent tray from moving
		Tray_Control_Unlock     = 1,			// allow tray to be ejected/inserted
		Tray_Control_Open       = 2,			// asynchronous version of open tray
		Tray_Control_Close      = 3,			// asynchronous version of close tray
		Tray_Control_Open_Sync  = 4,			// synchronous version of open tray
		Tray_Control_Close_Sync = 5				// synchronous version of close tray
	};

	enum DeviceState    // device state
	{
		//State_InUse           = 1,			// device in use by another application  - note: not reported back from engine, not applicable for embedded applications
		State_Not_Ready       = 70,				// generally no media in device
		State_Ready           = 17,				// device is ready
		State_Becoming_Ready  = 71				// device is busy
	};

	enum Erase_Type    // types of erase
	{
		Quick_Erase = 0,
		Long_Erase  = 1
	};

	typedef wchar_t* UnicodeString;

	typedef UInt32 TrayStatus_Type;
	static const TrayStatus_Type TrayStatus_Unknown = 1;   // unknown status
	static const TrayStatus_Type TrayStatus_Open    = 2;   // tray is open
	static const TrayStatus_Type TrayStatus_Closed  = 3;   // tray is closed

	// Device scan type passed in to AS_GetStorageDeviceCount
	typedef UInt32 Scan_Flags;
	static const Scan_Flags Scan_All     = 0;
	static const Scan_Flags Scan_Optical = (1 << 0);
	static const Scan_Flags Scan_Tape    = (1 << 1);

	// Bus Kind / Bus Info query properties
	typedef UInt32  Bus_Kind;
	typedef struct
	{
		Bus_Kind			bus_kind; 
		UInt8				addr_valid;
		UInt8				addr_adapter_id;
		UInt8				addr_target_id;
		UInt8				addr_lun_id;
		UInt8				Reserved[128];
	} Bus_Info;

	static const Bus_Kind Bus_ATAPI   = 25;
	static const Bus_Kind Bus_SCSI    = 26;
	static const Bus_Kind Bus_USB     = 28;
	static const Bus_Kind Bus_USB2    = 31;
	static const Bus_Kind Bus_1394    = 32;
	static const Bus_Kind Bus_Unknown = 0xfffffff;

	// Authentication query properties
	typedef UInt32 Authentication_Type;
	static const Authentication_Type Authentication_None = 0;   
	static const Authentication_Type Authentication_CPPM = (1 << 0);
	static const Authentication_Type Authentication_CPRM = (1 << 1);
	static const Authentication_Type Authentication_AACS = (1 << 2);

	
	typedef UInt32 DevType;

	static const DevType IsFile    = (1 << 0);
	static const DevType IsOptical = (1 << 1);
	static const DevType IsTape    = (1 << 2);
    //----------------------------------------------------------------------------------------------------------------
	// query device properties
	enum DevProp_Type						        // Type                         Description
	{												//------------------------------------------------------------------
		DevProp_Description     = 0,				//UnicodeString	      OUT: string of Device Description
		DevProp_MakeName        = 1,
		DevProp_ModelName       = 2,
		DevProp_Version         = 3,
		DevProp_DrivePath       = 6,				//UnicodeString       OUT: string of Device Drive Letter or path of file device
		DevProp_DevType         = 7,				//DevType             OUT: bits of the device type (Tape, Optical, File)
		DevProp_MedTypesCapable = 11,				//MediaKindType       OUT: bits of media types supported (DVD-RW, DVD+RW, CD-RW, CD-R, etc)
		DevProp_TrayStatus      = 12,				//TrayStatus_Type     OUT: TrayStatus type (UInt32)
		DevProp_Bus_Info        = 17,				//Bus_Info            OUT: structure describes bus kind and adapter/target/lun
		DevProp_AuthenTypes = 0x30004,				//Authentication_Type OUT: UInt32
		DevProp_SerialNum   = 0x30005,				//SerialNum           OUT: UInt8[16 bytes max]
		DevProp_CopyProtection = 0x30006,			//ProtectionType      OUT: bitflag of CSS and GuardBlock types supported
		DevProp_MaxTransferSize = 18,				//UInt32              OUT: size in bytes of max transfer size of device
		DevProp_DeviceKind      = 99				//Type				  IN/OUT:  type of device (Type_File, Type_Folder, Type_Device)
	};

	typedef UInt64 MediaKind;
    static const MediaKind MediaIsCdRom    = (1 << 0);
    static const MediaKind MediaIsCdr      = (1 << 1);
    static const MediaKind MediaIsCdrw     = (1 << 2);
    static const MediaKind MediaIsDdCdrom  = (1 << 3);
    static const MediaKind MediaIsDdCdr    = (1 << 4);
    static const MediaKind MediaIsDdCdrw   = (1 << 5);
    static const MediaKind MediaIsDvdRom   = (1 << 6);
    static const MediaKind MediaIsDvdr     = (1 << 7);
    static const MediaKind MediaIsDvdrw    = (1 << 8);
    static const MediaKind MediaIsDvdpr    = (1 << 9);
    static const MediaKind MediaIsDvdprw   = (1 << 10);
    static const MediaKind MediaIsDvdRam   = (1 << 11);
    static const MediaKind MediaIsDvdpr9   = (1 << 12);
    static const MediaKind MediaIsDvdr9    = (1 << 13);
    static const MediaKind MediaIsBdr	   = (1 << 14);
    static const MediaKind MediaIsBdre	   = (1 << 15);
	static const MediaKind MediaIsHdDvdRom = (1 << 16);
	static const MediaKind MediaIsHdDvdr   = (1 << 17);
	static const MediaKind MediaIsHdDvdrw  = (1 << 18);
	static const MediaKind MediaIsBdrom	   = (1 << 19);
    static const MediaKind MediaIsSeqTape  = ((UInt64)1 << 31);

	// Media State Information is valid only if Device state reports ready
	// Incompatible media is represented by the following states: Readable = false, Writable = false
	typedef UInt32 MediaState;

	static const MediaState MediaIsReadable    = (1 << 0);
	static const MediaState MediaIsWritable    = (1 << 1);
	static const MediaState MediaIsInvisible   = (1 << 2);
	static const MediaState MediaIsAppendable  = (1 << 3);
	static const MediaState MediaIsRewritable  = (1 << 5);
	static const MediaState MediaIsErasable    = (1 << 7);
	static const MediaState MediaIsProtected   = (1 << 10);
	static const MediaState MediaIsBlank       = (1 << 12);// See notes above
	static const MediaState LayerJumpSettable  = (1 << 13);// report whether setting jump address is allowed


	typedef UInt32 SpeedSetting_Type;
	static const SpeedSetting_Type Speed_Min = 0;				// minimum speed
	static const SpeedSetting_Type Speed_Max = 0xffffffff;		// maximum speed
	static const SpeedSetting_Type Speed_Var = 0xfffffffe;		// variable speed

	typedef UInt32 ProtectionType;     // returned during query of DevProp_CopyProtection and MedProp_CopyProtection
	static const ProtectionType ProtectionType_None					= 0;         // drive/media offers no copy protection
	static const ProtectionType ProtectionType_Qflix_CSS			= (1 << 0);  // drive/media offers Qflix Pro v1.0 CSS protection.
	static const ProtectionType ProtectionType_Qflix_Consumer_CSS	= (1 << 3);  // drive/media offers Qflix CSS copy protection
	static const ProtectionType ProtectionType_DVDDownload_CSS		= (1 << 9);  // drive/media offers DVD Forum's CSS copy protection.
	static const ProtectionType ProtectionType_GuardBlock_BadECC	= (1 << 16); // drive/media offers GuardBlock "Bad ECC blocks"

	typedef UInt32 FileFormatType;
	static const FileFormatType FormatType_Unknown = 0;
	static const FileFormatType FormatType_Raw = 1;
	static const FileFormatType FormatType_OpticalDetect = 2;
	static const FileFormatType FormatType_Raw_2048 = 3;
	static const FileFormatType FormatType_Raw_2336 = 4;
	static const FileFormatType FormatType_Raw_2352 = 5;
	static const FileFormatType FormatType_GI = 6;

	// CD-Text languages to be used as the 'property_index' parameter of
	// AS_StorageDevice_SetMediaProperty and AS_StorageDevice_SetTrackProperty.
	typedef UInt32 CDTextLanguageType;
	static const CDTextLanguageType		CDTEXT_LANG_GERMAN                  = 0x08;
	static const CDTextLanguageType		CDTEXT_LANG_ENGLISH                 = 0x09;
	static const CDTextLanguageType		CDTEXT_LANG_SPANISH                 = 0x0a;
	static const CDTextLanguageType		CDTEXT_LANG_FRENCH                  = 0x0f;
	static const CDTextLanguageType		CDTEXT_LANG_ITALIAN                 = 0x15;
	static const CDTextLanguageType		CDTEXT_LANG_DUTCH                   = 0x1d;
	static const CDTextLanguageType		CDTEXT_LANG_RUSSIAN                 = 0x56;
	static const CDTextLanguageType		CDTEXT_LANG_KOREAN                  = 0x65;
	static const CDTextLanguageType		CDTEXT_LANG_JAPANESE                = 0x69;
	static const CDTextLanguageType		CDTEXT_LANG_CHINESE                 = 0x75;

	typedef UInt32 DataMode;
	static const DataMode DataMode_Audio		= 0;
	static const DataMode DataMode_Data			= 1;
	static const DataMode DataMode_DataMode2	= 2;
	static const DataMode DataMode_Blank		= 69;

	typedef UInt32 RecordMode;
	static const RecordMode RecordMode_None = 0;
	static const RecordMode RecordMode_TAO	= 1;
	static const RecordMode RecordMode_SAO	= 2;
	static const RecordMode RecordMode_FixedPacket = 3;
	static const RecordMode RecordMode_VarPacket = 4;
	static const RecordMode RecordMode_Incremental= 5;
	static const RecordMode RecordMode_RestrictOvr = 6;
	static const RecordMode RecordMode_Standard = 7;
	static const RecordMode RecordMode_SAO_Finalize = 8;
	static const RecordMode RecordMode_LayerJump = 9;
	static const RecordMode RecordMode_POW	= 10;
	static const RecordMode RecordMode_Incremental_Borderless = 11;

	typedef UInt32 LayerPropType;
	static const LayerPropType LayerProp_Symmetrical = 1;
	static const LayerPropType LayerProp_Contiguous = 2;

	typedef UInt32 LayerType;
	static const LayerType Layer_None = 0;
	static const LayerType Layer_OTP = 1;
	static const LayerType Layer_PTP = 2;

	//---------------------------------------------------------------------------------------------------------------
	// query media properties are valid if the device state is State_Ready
	enum MediaProp_Type										// Type							Description
	{														//----------------------------------------------------------------------
		MedProp_Num_Sessions					= 3,		// UInt32						OUT: number of sessions
		MedProp_Num_Tracks						= 4,		// UInt32						OUT: number of tracks
		MedProp_Num_Layers						= 5,		// UInt32						IN/OUT: 2 = dual layer, 1 = single layer
		MedProp_LayerType                       = 6,        // LayerType					OUT: type of layer  (OTP/PTP)
		MedProp_LayerProperties					= 7,		// LayerPropType				OUT: layer property type - add in consts for Symmetrical/Contiguous
		MedProp_MaxLayerJumpAddr				= 8,		// UInt32						IN/OUT: location of max layer jump address (using UInt32[] will return all layers info, otherwise will return layer 0 info)
		MedProp_LayerStart						= 9,		// UInt32						IN/OUT: location of start of layer (using UInt32[] will return all layers info, otherwise will return layer 0 info)
		MedProp_State							= 12,		// MediaState					OUT: State of media (writable, write-protected, etc).
		MedProp_Bytes_Used						= 18,		// UInt64						OUT: Media space used
		MedProp_Bytes_Free						= 19,		// UInt64						OUT: Media Capacity information 
		MedProp_Kind							= 30,		// MediaKind					OUT: DVD-RW, DVD+RW, CD-RW, CD-R, etc
		MedProp_FileFormat						= 31,       // FileFormat					IN/OUT: File Device format type
		MedProp_Manufacturer					= 22,		// UInt8[]						OUT: Media Vendor ID from disc		
		MedProp_CurRead_DataRates				= 23,		// UInt32						IN/OUT: KBps for the current read speed
		MedProp_CurWrite_DataRates				= 24,		// UInt32						IN: KBps for the current write speed
		MedProp_WriteList_DataRates				= 26,		// UInt32[]						OUT: The list of UInt32s of Write Data rates
		MedProp_LayerJumpAddr					= 37,		// UInt64						IN/OUT: location of layer jump address
		MedProp_MinLayerJumpAddr				= 45,		// UInt64						OUT: location of min layer jump address
		MedProp_CurRead_Hundredth_X				=0x00010000,// UInt32						IN/OUT: 1/100ths X for current read speed
		MedProp_CurWrite_Hundredth_X			=0x00010001,// UInt32						IN: 1/100ths X for current write speed
		MedProp_WriteList_Hundredth_X			=0x00010003,// UInt32[]						OUT: The list of UInt32s of Data rates in 1/100ths X

		MedProp_CDText_MediaCatalogNum          = 0x00020000,// UnicodeString				IN/OUT: string of CDText Media Catalog number
		MedProp_CDText_Title					= 0x00020001,// UnicodeString				IN/OUT: string of CDText 
		MedProp_CDText_Performer				= 0x00020002,// UnicodeString				IN/OUT: string of CDText 
		MedProp_CDText_Songwriter				= 0x00020003,// UnicodeString				IN/OUT: string of CDText 
		MedProp_CDText_Composer					= 0x00020004,// UnicodeString				IN/OUT: string of CDText 
		MedProp_CDText_Arranger					= 0x00020005,// UnicodeString				IN/OUT: string of CDText 
		MedProp_CDText_Message					= 0x00020006,// UnicodeString				IN/OUT: string of CDText 
		MedProp_CDText_Genre					= 0x00020007,// UnicodeString				IN/OUT: string of CDText 
		MedProp_CDText_Genre_Code				= 0x00020008,// UnicodeString				IN/OUT: string of CDText 
		MedProp_CDText_NumLanguages				= 0x00020009,// UInt32						OUT: number of languages with CDText on this media
		MedProp_CDText_Languages				= 0x0002000a,// UInt32[]					OUT: array of languages with CDText

		MedProp_CurAuthenType 					=0x00030004,// Authentication_Type			IN: get the authentication type of the media
		MedProp_CopyProtection                  =0x00030006,// ProtectionType				OUT: bitflag of ProtectionType supported by this media.
		MedProp_TestWrite						= 99,		// UInt32                       IN/OUT: Testwrite enabled (1) / disabled (0) - (reports invalid media if no media support).
		MedProp_RecordMode						= 100,		// UInt32						IN/OUT: RecordMode for specifying the recording mode.
		MedProp_DataMode						= 101,		// UInt32						IN/OUT: DataMode for specifying the data mode.
	};

	//---------------------------------------------------------------------------------------------------------------
	// query track properties
	typedef UInt32 TrackProp_TrackType;
	static const TrackProp_TrackType TrackType_Audio		= 0;
	static const TrackProp_TrackType TrackType_Data			= 1;
	static const TrackProp_TrackType TrackType_DataMode2	= 2;
	static const TrackProp_TrackType TrackType_Blank		= 69;

	typedef UInt32 TrackProp_RecordMode_Type;
	static const TrackProp_RecordMode_Type TrackMode_None	= 0;
	static const TrackProp_RecordMode_Type TrackMode_TAO	= 1;
	static const TrackProp_RecordMode_Type TrackMode_SAO	= 2;
	static const TrackProp_RecordMode_Type TrackMode_FixedPacket = 3;
	static const TrackProp_RecordMode_Type TrackMode_VarPacket = 4;
	static const TrackProp_RecordMode_Type TrackMode_Incremental= 5;
	static const TrackProp_RecordMode_Type TrackMode_RestrictOvr = 6;
	static const TrackProp_RecordMode_Type TrackMode_Standard = 7;
	static const TrackProp_RecordMode_Type TrackMode_SAO_Finalize = 8;
	static const TrackProp_RecordMode_Type TrackMode_LayerJump = 9;
	static const TrackProp_RecordMode_Type TrackMode_POW	= 10;
	static const TrackProp_RecordMode_Type TrackMode_Incremental_Borderless = 11;

	typedef UInt32 TrackProp_IOState;
	static const TrackProp_IOState Track_IOState_Writable		= 0x00000002;
	static const TrackProp_IOState Track_IOState_Invisible		= 0x00000004;
	static const TrackProp_IOState Track_IOState_Appendable		= 0x00000008;
	static const TrackProp_IOState Track_IOState_Open			= 0x00000010; // equivalent to appendable except for RO
	static const TrackProp_IOState Track_IOState_Rewritable		= 0x00000020;
	static const TrackProp_IOState Track_IOState_AutoJump		= 0x00000040; // note : different from media state
	static const TrackProp_IOState Track_IOState_Erasable		= 0x00000080;
	static const TrackProp_IOState Track_IOState_Formattable	= 0x00000100;
	static const TrackProp_IOState Track_IOState_Expandable		= 0x00000800; // note : different from media state
	static const TrackProp_IOState Track_IOState_Blank			= 0x00001000;

	//---------------------------------------------------------------------------------------------------------------
	// track properties are valid if the device state is State_Ready
	enum TrackProp_Type										// Type							Description
	{														//-------------------------------------------------------
		TrackProp_BlockSize						= 1,		// UInt32						IN/OUT: blocksize in bytes (only tape can select new blocksize)
		TrackProp_WriteFrameSize				= 2,		// UInt32						IN/OUT: frame size for write
		TrackProp_OptimalFrameSize				= 3,		// UInt32						IN/OUT: optimal frame size for write
		TrackProp_TrkType						= 4,		// TrackProp_TrackType			IN/OUT: type of track (audio, data, data Mode2, blank)
		TrackProp_RecordMode					= 5,		// TrackProp_RecordMode_Type	IN/OUT: recording mode
		TrackProp_SessionNum					= 6,		// UInt32						IN/OUT: session number
		TrackProp_StartAddr						= 7,		// UInt32						IN/OUT: start address of track
		TrackProp_Length						= 8,		// UInt32						IN/OUT: length of track
		TrackProp_RecLength						= 9,		// UInt32						IN/OUT: recorded length of track
		TrackProp_AppendAddr					=10,		// UInt32						IN/OUT: append address of track
		TrackProp_State							=11,		// TrackProp_IOState			IN/OUT: state of track
		TrackProp_CurFormatType					=12,		// UInt32						IN/OUT: currently formatted type of track
		TrackProp_FormatTypes					=13,		// UInt32[]						IN/OUT: format types for tracks
		TrackProp_Copyrighted					=14,		// UInt32						IN/OUT: copyrighted
		TrackProp_Pregap						=15,		// UInt32						IN/OUT: size of pregap (in blocks)

		// cdtext
		TrackProp_ISRC							=16,		// UnicodeString				IN/OUT: string of CDText ISRC
		TrackProp_CDText_Title					=17,		// UnicodeString				IN/OUT: string of CDText title
		TrackProp_CDText_Performer				=18,		// UnicodeString				IN/OUT: string of CDText Performer
		TrackProp_CDText_Songwriter				=19,		// UnicodeString				IN/OUT: string of CDText Songwriter
		TrackProp_CDText_Composer				=20,		// UnicodeString				IN/OUT: string of CDText Composer
		TrackProp_CDText_Arranger				=21,		// UnicodeString				IN/OUT: string of CDText Arranger
		TrackProp_CDText_Message				=22,		// UnicodeString				IN/OUT: string of CDText Message
		TrackProp_NumExtents					=23,		// UInt32						IN/OUT: number of extents in track
		TrackProp_Postgap						=24,		// UInt32						IN/OUT: size of postgap (in blocks)
		TrackProp_CDText_Genre					=25,		// UnicodeString				IN/OUT: string of CDText Genre
		TrackProp_CDText_GenreCode				=26,		// UnicodeString				IN/OUT: string of CDText Genre Code
		TrackProp_LastRecAddr					=27			// UInt32						IN/OUT: last recorded address
	};

	//---------------------------------------------------------------------------------------------------------------
	// Exclusive Access Types
	enum ExclusiveAccess_Type
	{
		ExclusiveAccess_Obtain       = 1,
		ExclusiveAccess_Quick_Obtain = 2,
		ExclusiveAccess_Release      = 3,
		ExclusiveAccess_Query        = 4
	};

	//---------------------------------------------------------------------------------------------------------------
	// Register for device arrival/departure notification types
	enum SetCallbackType
	{
		Callback_Register   = 1,
		Callback_Unregister = 2
	};

	enum Event_Type
	{
		EventMediaRemoval  = 1,	// media disappeared
		EventMediaArrival  = 2,	// new media or changed media is ready
		EventDeviceArrival = 6,	// drive appeared
		EventDeviceRemoval = 7	// drive disappeared
	};

	// Function typedef for reporting progress information
	// Used in AS_StorageDevice_EraseMedia and AS_StorageDevice_Copy
	enum Info
	{
		Info_ProgressUpdate             = 1,     // infoDword = percent complete, infoPtr = Progress structure
		Info_PromptForMediaInDevice     = 2,     // infoDword = unused, infoPtr = unused
	};

	// additional progress info (beyond percent complete)
	struct Progress
	{
		UInt32 ValidBytes;      // size, in bytes, that follow that are valid
								// so this structure can be expanded to return additional
								// information in the future
		UInt32 CurrentSector;   // How many sectors have been written (not the actual current
								// sector number)
		UInt32 TotalSectors;    // Question: should we return this every time, or should this
								// be a property?
		UInt32 DeviceInfo;      // returns media handle on which this progress is reported
		UInt32 ProgressStatus;  // reserved for future use
		UInt32 CurrentOperation;// reserved for future use
	};
	// progress consts for detailed callback info
	typedef UInt32 JobType;
	static const JobType JobType_Writing = 0x00000003;
	static const JobType JobType_Reading = 0x00000004;
	static const JobType JobType_Comparing = 0x00000005;
	static const JobType JobType_Formatting = 0x00000006;
	static const JobType JobType_Erasing = 0x00000007;
	static const JobType JobType_Initializing = 0x00000008;
	static const JobType JobType_Finalizing = 0x00000009;
	static const JobType JobType_Aborting = 0x0000000a;
	static const JobType JobType_WriteFilemark = 0x0000000b;
	static const JobType JobType_Seek = 0x0000000c;

	typedef UInt32 JobStatus;
	static const JobStatus JobStatus_Success = 0;
	static const JobStatus JobStatus_Running = 1;
	static const JobStatus JobStatus_BusyAborting = 2;
	static const JobStatus JobStatus_Idle = 0x10000;
	static const JobStatus JobStatus_PartialSuccess = 0x20000 | 0;
	static const JobStatus JobStatus_Aborted = ( 0x20000 | 0x0006);
	static const JobStatus JobStatus_Error = 0xfffe0000;

	typedef UInt32 CopyFlags;
	static const CopyFlags CopyFlags_Write = 1;
	static const CopyFlags CopyFlags_TestMode =  2;
	static const CopyFlags CopyFlags_Compare	=  3;
	static const CopyFlags CopyFlags_CompareData =  4;
	static const CopyFlags CopyFlags_VerifylessWrite = 5;		// warning: can create bad media without error
																// client must validate target media in this mode
	static const CopyFlags CopyFlags_VNR = 6;					// only works with BDR
	static const CopyFlags CopyFlags_Compare_CopyProtection = 7;// compare a source to a destination which has copy protection applied.
	static const CopyFlags CopyFlags_QflixPro = 0x100;			// Sonic Qflix Pro (CSS writing) is not enabled in most SDKs
	static const CopyFlags CopyFlags_Qflix = 0x100;				// Sonic Qflix (CSS writing) is not enabled in most SDKs
	static const CopyFlags CopyFlags_GuardBlock = 0x200;		// Sonic GuardBlock (Bad block writing) is not enabled in most SDKs


	// Function typedef for reporting progress or other information
	// Used in AS_StorageDevice_EraseMedia and other device functions taking callback parameter
	typedef AS_StorageError (AS_CALLBACK *InfoCallback)(const Info& info, 
														UInt32 infoDword, void *infoPtr,
														void *callerUse);

	// Function typedef for receiving device event information
	// used to alert client of device/media presence.
	typedef void   (AS_CALLBACK * EventCallback)
	(
		enum Event_Type Event,				// Event - device arrival, device departure.
		Handle Device,						// Handle to device in which the event occurred.
		void*  CallerUse					// Caller gets their PVOID back that they registered with.
	);


	typedef struct          // device error as reported by AS_StorageDevice_GetError
	{
		UInt8 valid;         // valid bit represents the following information is valid.  once queried the the valid bit shall be reset
		UInt8 cmd;           // These four bytes may be reported to Sonic for further information on the error
		UInt8 key;
		UInt8 code;
		UInt8 qual;
		UInt8 other[16];     // This may be dumped to a log file for more detail on the error
	} Error;


}; // internal representation of the device.


//-----------------------------------------------
// API functions
//-----------------------------------------------
//

//-----------------------------------------------------------------------------
// AS_GetStorageDeviceCount
//
// DESCRIPTION:
// Outputs a count of the devices on the system. 
// Depending on the value specified with the parameter flags, 
// the count will cover all devices, optical devices only, or tape devices only. 
// Optional for CE, where only one known device may exist.
// 'flag' is a bitfield, legal to scan for multiple types.
// When counting all devices on the system ('flag' parameter set to 
// AS_StorageDevice::Scan_All).
//
// The number of devices is output with parameter 'count'.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - if the flags are not defined.
//  AS_StorageError_DeviceSelected   - if there are devices currently in use by the SDK,
//                              reported to close all devices prior to scanning.
//  AS_StorageError_Fatal            - unknown software error.
//
AS_StorageError AS_API AS_GetStorageDeviceCount(
    AS_StorageDevice::Scan_Flags flag,      // IN: An object of type AS_StorageDevice::Scan_Flags specifying the type of count to perform.
	UInt32 &	count);						// OUT: A UInt32 that is to be filled in with the number of devices on the system.

//-----------------------------------------------------------------------------
//AS_OpenStorageDevice
//
// DESCRIPTION:
// Sets the default device for device-related calls to the OS-provided 
// drive index specified with the parameter index. A device reference (AS_StorageDevice struct) 
// for the device is output with device and used to specify the device in subsequent device-related calls.

// NOTE:
//      This function must be called before any other device-related calls.
//      The value of index must be less than the count output by AS_GetStorageDeviceCount.
// EXAMPLES:
//      There are 3 types of usage for AS_OpenStorageDevice: physical devices, file devices, and virtual devices
//      Here is an example call for each:
// Physical Device:
//      AS_OpenStorageDevice(DeviceIndex, MyDevice);
// File Device:
//      AS_OpenStorageDevice(0, MyDevice, AS_StringW(FileName));
// Virtual Device:
//      AS_OpenStorageDevice(0, MyDevice, AS_StringA(""));
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidIndex	 - if the index provided is greater than the count provided by AS_GetStorageDeviceCount
//  AS_StorageError_Fatal            - failed to create a list of valid drives
//
AS_StorageError AS_API AS_OpenStorageDevice(
    UInt32 index,							// IN:  A UInt32 specifying the index (from OS) for the drive to open.
    AS_StorageDevice & device,				// OUT: A reference to the AS_StorageDevice struct that is to be filled in for the selected device.
    const AS_String & path = AS_String());	// IN (Optional): An AS_String file path to use for files represented as devices

//-----------------------------------------------------------------------------
// AS_StorageDevice_Close 
//
// DESCRIPTION:
// Closes the specified device and releases all associated memory.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid.
//
AS_StorageError AS_API AS_StorageDevice_Close(
    AS_StorageDevice & device);				// IN/OUT:  A reference to the AS_StorageDevice struct specifying the device to close.

//-----------------------------------------------------------------------------
// AS_StorageDevice_GetState
//
// DESCRIPTION:
// Accesses the specified device and outputs the device state
//  (ready, busy, no media with the states - State_Becoming_Ready, State_Ready, State_Not_Ready)
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid.
//
AS_StorageError AS_API AS_StorageDevice_GetState(
    const AS_StorageDevice & device,			// IN:  A constant reference to the AS_StorageDevice struct specifying the device.
    enum AS_StorageDevice::DeviceState&	state);	// OUT: A reference to the object that is to be filled in with a value of type AS_StorageDevice::DeviceState indicating the state of the device.
   // NOTE: Call will access media in device.

//-----------------------------------------------------------------------------
// AS_StorageDevice_ExclusiveAccess
//
// DESCRIPTION:
// Exclusive Access - get/set/query Exclusive Access for the device
// Retrieve current status with AS_StorageDevice::ExclusiveAccess_Query
// Then, set and release with ExclusiveAccess_Obtain and ExclusiveAccess_Release
//
// NOTE: AS_StorageError_DeviceError = failed (if the drive is already in use when trying to obtain)
//
// RETURN: 
//  AS_StorageError_None             - ok
//  AS_StorageError_DeviceInUse      - if the drive is already in use when trying to obtain
//  AS_StorageError_InvalidParameter - invalid drive handle or invalid access_type flag
//
AS_StorageError AS_API AS_StorageDevice_ExclusiveAccess(
	const AS_StorageDevice& device,						// IN:  A constant reference to the AS_StorageDevice struct specifying the device.
	enum AS_StorageDevice::ExclusiveAccess_Type access_type,// IN:  desired access
	const AS_String &		access_name,				// IN:  name of your application
	AS_String &				app_in_use_name);			// OUT: name of application currently using drive, if in use.

//-----------------------------------------------------------------------------
// AS_SetStorageDeviceCallback
//
// DESCRIPTION:
// Register for callbacks - receive a notification when a new drive arrives,
// or when new media arrives in a drive.
//
// NOTE: Current drive which has been exclusively obtained will not report media notification
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid or invalid Callback flag
//  AS_StorageError_Fatal            - failed to create a list of valid drives

AS_StorageError AS_API AS_SetStorageDeviceCallback(	
	enum AS_StorageDevice::SetCallbackType RegType,		// IN: register or unregister
	UInt32					Reserved,					// IN: reserved
	AS_StorageDevice::EventCallback	Callback,			// IN: caller's function to receive notifications.
	void*					CallerUse,					// IN: caller receives this back during callback.
	void*					Reserved2);					// IN: reserved

//-----------------------------------------------------------------------------
// AS_StorageDevice_TrayControl 
//
// DESCRIPTION:
// Used to control operation (lock, unlock, open, close, etc.) of the media tray of the specified device.
//
// NOTE: An error will result if the device is currently in use.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_DeviceError      - the device reported an error.
//  AS_StorageError_InvalidParameter - the device handle provided is invalid.
//
AS_StorageError AS_API AS_StorageDevice_TrayControl(
	const AS_StorageDevice&	device,						// IN: A constant reference to the AS_StorageDevice struct specifying the device.
	enum  AS_StorageDevice::TrayControls tray_control_type);// IN: An object of type AS_StorageDevice::TrayControls specifying the desired tray control operation.

//-----------------------------------------------------------------------------
// AS_StorageDevice_GetDeviceProperty 
//
// DESCRIPTION:
// Retrieves the value of a property of the specified device. 
// The type of property (device type or supported media types) 
// is specified with the parameter device_property. 
// The buffer to which the property should be written is specified 
// with property_buffer, the size of which is specified with buffer_size. 
// When the call completes successfully, the actual size of the property's 
// available data is output with ret_data_size.
//
//
// The extended optional parameters are provided for additional property data to be retrieved.
// If the extended info is a list, then the value of property prop for index PropertyIndex 
// is provided in extpropData, the size of which is specified with extpropSize.  
// If PropertyIndex = 0xffffffff, the whole list is written to extpropData.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// NOTES:
//       To query only the size of the property data, specify property_buffer 
//       as NULL and buffer_size as 0; ret_data_size will output the number of available bytes.
//       Not all properties support additional property information.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid or buffer size can't hold return value.
//  AS_StorageError_DeviceError      - the device reported an error.
//
AS_StorageError AS_API AS_StorageDevice_GetDeviceProperty(
	const AS_StorageDevice& device,						// IN: A constant reference to the AS_StorageDevice struct specifying the device.
	enum AS_StorageDevice::DevProp_Type device_property,// IN:  information of the device type to be queried
	UInt32                  buffer_size,				// IN: A UInt32 specifying the size in bytes of the buffer to which the property data is to be written.
	void*                   property_buffer,			// OUT: A pointer to the buffer that is to be filled in with the data retrieved for the specified property.
	UInt32*                 ret_data_size,				// OUT: A pointer to a UInt32 that is to be filled in with the size in bytes of the property data available for the specified property.
	UInt32                  property_index = 0,			// IN (Optional):  A UInt32 specifying the index of list of property returned - only used for properties which are lists.
	UInt32                  ext_buffer_size = 0,		// IN (Optional): A UInt32 specifying the size in bytes of the extended buffer to which the property data is to be written.
	void*                   ext_property_buffer = 0,	// IN/OUT (Optional): A pointer to the extended buffer that is to be filled in with the data retrieved for the specified property.
	UInt32*                 ext_ret_data_size = 0);		// OUT (Optional): A pointer to a UInt32 that is to be filled in extended buffer with the size in bytes of the property data available for the specified property.

//-----------------------------------------------------------------------------
// AS_StorageDevice_SetDeviceProperty 
//
// DESCRIPTION:
// Sets the value of a property of the specified device. 
// The property to set is specified with the parameter device_property. 
// The buffer from which the new value of the property should be written 
// is specified with property_buffer, and the size of this buffer is specified with buffer_size.
//
// The extended optional parameters are provided for additional property data to be set.
// If the extended info is a list, then the value of property prop for index property_index 
// is given in extpropData, the size of which is specified with ext_buffer_size.  
// If PropertyIndex = 0xffffffff, the whole list is set from ext_property_buffer.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid or buffer size can't hold return value.
//  AS_StorageError_DeviceError      - the device reported an error.
//  AS_StorageError_DeviceNotReady   - device reports no media.
//
AS_StorageError AS_API AS_StorageDevice_SetDeviceProperty(
	AS_StorageDevice& device,							// IN: A constant reference to the AS_StorageDevice struct specifying the device.
	enum AS_StorageDevice::DevProp_Type	device_property,// IN: information of the device type to be set
	UInt32					buffer_size,				// IN: size of the buffer in bytes
	void*					property_buffer,			// IN: A pointer to the buffer that is to be filled in with the data retrieved for the specified property.
	UInt32					property_index = 0,			// IN (Optional): A UInt32 specifying the index of list of property returned - only used for properties which are lists.
	UInt32					ext_buffer_size = 0,		// IN (Optional): A UInt32 specifying the size in bytes of the extended buffer to which the property data is to be written.
	void*					ext_property_buffer = 0);	// IN/OUT (Optional): A pointer to the extended buffer that is to be filled in with the data retrieved for the specified property.

//-----------------------------------------------------------------------------
// AS_StorageDevice_GetMediaProperty 
//
// DESCRIPTION:
// Retrieves the current value for a property of the media in the specified device. 
// The type of property (number of layers, media type, state, bytes used, or bytes free) 
// is specified with the parameter media_property. The buffer to which the property should 
// be written is specified with property_buffer, the size of which is specified with buffer_size. 
// When the call completes successfully, the actual size of the property's available data is output with ret_data_size.
//
// The extended optional parameters are provided for additional property data to be retrieved.
// If the extended info is a list, then the value of property prop for index PropertyIndex 
// is provided in extpropData, the size of which is specified with extpropSize.  
// If PropertyIndex = 0xffffffff, the whole list is written to extpropData.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// NOTE: To query only the size of the property data, specify property_buffer
// as NULL and buffer_size as 0; ret_data_size will output the number of available bytes.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid or buffer size can't hold return value.
//  AS_StorageError_DeviceError      - the device reported an error.
//  AS_StorageError_DeviceNotReady   - device reports no media.
//
AS_StorageError AS_API AS_StorageDevice_GetMediaProperty(
	const AS_StorageDevice& device,						// IN: A constant reference to the AS_StorageDevice struct specifying the device.
	enum AS_StorageDevice::MediaProp_Type media_property, // IN: An object of type AS_StorageDevice::MediaProp_Type specifying the media property whose value is to be retrieved.
	UInt32                  buffer_size,				// IN: A UInt32 specifying the size in bytes of the buffer to which the property data is to be written.
	void*                   property_buffer,			// OUT: A pointer to the buffer that is to be filled in with the data retrieved for the specified property.
	UInt32*                 ret_data_size,				// OUT: A pointer to a UInt32 that is to be filled in with the size in bytes of the property data available for the specified property.
	UInt32                  property_index = 0,			// IN (Optional): A UInt32 specifying the index of list of property returned - only used for properties which are lists.
	UInt32                  ext_buffer_size = 0,		// IN (Optional): A UInt32 specifying the size in bytes of the extended buffer to which the property data is to be written.
	void*                   ext_property_buffer = 0,	// IN/OUT (Optional): A pointer to the extended buffer that is to be filled in with the data retrieved for the specified property.
	UInt32*                 ext_ret_data_size = 0);		// OUT (Optional): A pointer to a UInt32 that is to be filled in for extended buffer with the size in bytes of the property data available for the specified property. 

//-----------------------------------------------------------------------------
// AS_StorageDevice_SetMediaProperty 
//
// DESCRIPTION:
// Sets the value of a property of the media in the specified device. 
// The property to set is specified with the parameter media_property. 
// The buffer from which the new value of the property should be written 
// is specified with property_buffer, and the size of this buffer is specified with buffer_size.
//
// The extended optional parameters are provided for additional property data to be set.
// If the extended info is a list, then the value of property prop for index property_index 
// is given in extpropData, the size of which is specified with ext_buffer_size.  
// If PropertyIndex = 0xffffffff, the whole list is set from ext_property_buffer.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid or buffer size can't hold return value.
//  AS_StorageError_DeviceError      - the device reported an error.
//  AS_StorageError_DeviceNotReady   - device reports no media.
//
AS_StorageError AS_API AS_StorageDevice_SetMediaProperty(
	const AS_StorageDevice& device,						// IN: A constant reference to the AS_StorageDevice struct specifying the device.
	enum AS_StorageDevice::MediaProp_Type	media_property,	// IN: information of the media type to be set
	UInt32					buffer_size,				// IN: size of the buffer in bytes
	void*					property_buffer,			// IN: A pointer to the buffer that is to be filled in with the data retrieved for the specified property.
	UInt32					property_index = 0,			// IN (Optional): A UInt32 specifying the index of list of property returned - only used for properties which are lists.
	UInt32					ext_buffer_size = 0,		// IN (Optional): A UInt32 specifying the size in bytes of the extended buffer to which the property data is to be written.
	void*					ext_property_buffer = 0);	// IN/OUT (Optional): A pointer to the extended buffer that is to be filled in with the data retrieved for the specified property.

//-----------------------------------------------------------------------------
// AS_StorageDevice_SetTrackProperty
//
// DESCRIPTION:
// Sets the value of a property of the track on media in the specified device. 
// The property to set is specified with the parameter track_property. 
// The buffer from which the new value of the property should be written 
// is specified with property_buffer, and the size of this buffer is specified with buffer_size.
//
// The extended optional parameters are provided for additional property data to be set.
// If the extended info is a list, then the value of property prop for index property_index 
// is given in extpropData, the size of which is specified with ext_buffer_size.  
// If PropertyIndex = 0xffffffff, the whole list is set from ext_property_buffer.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid or buffer size can't hold return value.
//  AS_StorageError_DeviceError      - the device reported an error.
//  AS_StorageError_DeviceNotReady   - device reports no media.
//
AS_StorageError AS_API AS_StorageDevice_SetTrackProperty(
	const AS_StorageDevice&	device,						// IN: A constant reference to the AS_StorageDevice struct specifying the device.
    UInt32					trackNum,					// IN: track number
	enum AS_StorageDevice::TrackProp_Type	track_property,	// IN: information of the track to be set
	UInt32					buffer_size,				// IN: size of the buffer in bytes
	void*					property_buffer,			// IN: A pointer to the buffer that is to be filled in with the data retrieved for the specified property.
	UInt32					property_index = 0,			// IN (Optional):A UInt32 specifying the index of list of property returned - only used for properties which are lists.
	UInt32					ext_buffer_size = 0,		// IN (Optional): A UInt32 specifying the size in bytes of the extended buffer to which the property data is to be written.
	void*					ext_property_buffer = 0);	// IN/OUT (Optional): A pointer to the extended buffer that is to be filled in with the data retrieved for the specified property.

//-----------------------------------------------------------------------------
// AS_StorageDevice_GetTrackProperty
//
// DESCRIPTION:
// Retrieves the current value for a property of the track on media in the specified device. 
// The type of property (track length, track startAddress, track preGap, track RecordMode, etc.) 
// is specified with the parameter track_property. The buffer to which the property should 
// be written is specified with property_buffer, the size of which is specified with buffer_size. 
// When the call completes successfully, the actual size of the property's available data is output with ret_data_size.
//
//
// The extended optional parameters are provided for additional property data to be retrieved.
// If the extended info is a list, then the value of property prop for index property_index 
// is provided in extpropData, the size of which is specified with ext_ret_data_size.  
// If PropertyIndex = 0xffffffff, the whole list is written to ext_property_buffer.
//
// If extended info is not a list of items, then PropertyIndex value is ignored.
//
// NOTE: To query only the size of the property data, specify property_buffer
// as NULL and buffer_size as 0; ret_data_size will output the number of available bytes.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - device handle is invalid.
//
AS_StorageError AS_API AS_StorageDevice_GetTrackProperty(
   const AS_StorageDevice&	device,						// IN: A constant reference to the AS_StorageDevice struct specifying the device.
   UInt32					trackNum,					// IN: track number
   enum AS_StorageDevice::TrackProp_Type	track_property,	// IN: information of the track to be retrieved
   UInt32					buffer_size,				// IN: size of the buffer in bytes
   void*					property_buffer,			// OUT: A pointer to the buffer that is to be filled in with the data retrieved for the specified property.
   UInt32*					ret_data_size,				// OUT: size of the output buffer
   UInt32					property_index = 0,			// IN (Optional): A UInt32 specifying the index of list of property returned - only used for properties which are lists.
   UInt32					ext_buffer_size = 0,		// IN (Optional):  A UInt32 specifying the size in bytes of the extended buffer to which the property data is to be written.
   void*					ext_property_buffer = 0,	// IN/OUT (Optional): A pointer to the extended buffer that is to be filled in with the data retrieved for the specified property.
   UInt32*					ext_ret_data_size = 0);		// OUT (Optional): A pointer to a UInt32 that is to be filled in for extended buffer with the size in bytes of the property data available for the specified property.

//-----------------------------------------------------------------------------
// AS_StorageDevice_GetError
//
// DESCRIPTION:
// All AS_API which return an AS_StorageError may return device sense information.
// The device sense information is in the form of 0x7fXXYYZZ where XX is the Sense Key,
// YY is the Sense Code, and ZZ is the Sense Qualifier.
// In some cases, the caller would like more extended information about this sense error,
// such as the drive CDB command which originated this error.
// AS_StorageDevice_GetError can return this extended information if the last AS_API returned a sense code.
//
AS_StorageError AS_API AS_StorageDevice_GetError(		// RETURNS: last drive error number
   const AS_StorageDevice &	device,						// IN:  device reference
   UInt32					structure_version,			// IN:  Size of structure to fill (version) sizeof(Error)
   AS_StorageDevice::Error*	err							// OUT: Error structure filled
   );

//-----------------------------------------------------------------------------
// AS_StorageDevice_EraseMedia
//
// DESCRIPTION:
// Erases the media, if any, in the specified device. The type of erase operation to perform (quick or long) is specified with the parameter erase_type. A callback with which the AuthorScript engine will report on the progress of the erasure is specified with callback. The parameter userdata allows the host application to specify a pointer to a memory location that will be passed back whenever AuthorScript calls the callback.
//
// NOTES:
//      The device state must be ready (see AS_StorageDevice_GetState).
//      An error will result if the device is currently in use by an AuthorScript Imaging project.
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - invalid drive handle
//  AS_StorageError_DeviceError      - the device reported an error
//
AS_StorageError AS_API AS_StorageDevice_EraseMedia(
	const AS_StorageDevice&	device,						// IN: A constant reference to the AS_StorageDevice struct specifying the device.
	enum AS_StorageDevice::Erase_Type	erase_type,		// IN: type of erase Quick_Erase, Long_Erase,
	AS_StorageDevice::InfoCallback	callback,		// IN: progress callback proc
	void*					userdata);					// IN: progress callback userdata



//-----------------------------------------------------------------------------
// AS_StorageDevice_Copy
// 
// DESCRIPTION:
// Copies from a source to destination(s).  The type of copy is specified with the "flag" parameter.
// NOTES:
//       Currently only a single file format type can be specified for destination
//
// RETURN:
//  AS_StorageError_None             - ok
//  AS_StorageError_InvalidParameter - invalid drive handle
//  AS_StorageError_DeviceError      - the device reported an error
//
AS_StorageError AS_API AS_StorageDevice_Copy (
	const AS_StorageDevice	&           srcDevice,		// IN: source device
	AS_StorageDevice::FileFormatType    format,			// IN: format type setting for dest file(s)
	AS_StorageDevice::CopyFlags	        flag,			// IN: flags for copy params
	UInt32								trackNum,		// IN: optional param to copy specific track from source
	UInt32                              NumDevices,		// IN: num of destinations
	const AS_StorageDevice *            DeviceList,		// IN: array of destination devices
	AS_StorageDevice::InfoCallback		callbackList,	// IN: progress callback function
    void* callerUse);									// IN: progress callback userdata
#ifdef __cplusplus
}
#endif

#endif //__AS_StorageDevice_h__
