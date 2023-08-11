//-----------------------------------------------------------------------------
// AS_StorageTest.cpp
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#include <vector>
#include <string>

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#if defined (USE_FRAMEWORK)
#include <AS_Storage/AS_Storage.h>
#else
#include "AS_StorageError.h"
#include "AS_StorageTypes.h"
#include "AS_StorageDevice.h"
#include "AS_File.h"
#include "AS_Volume.h"
#include "AS_StringHelper.h"
#endif

#include <iostream>
#include <fstream>
#include <cassert>

#if defined (_WIN32)
#include <windows.h>
#endif


//*************************************************************************************************//
// helper functions for sample app
AS_StorageError DoMenuOperation(AS_StorageDevice & device, AS_Volume & volume, std::string & menuInput);
void OutputError(AS_StorageError errorCode);
AS_StorageError GetDirInfo(const AS_Volume & volume, const AS_File::Path& inPath, bool readFileContents);
AS_StorageError GetFileInfo(const AS_Volume & volume, const AS_File::Path & filePath, bool readFileContents);
void OutputDate(const AS_Volume::DateTime & fileDate);
AS_StorageError GetNewDate(AS_Volume::DateTime & newDate);
//AS_Volume::FS_Type GetFormatTypeFromSelection(UInt32 formatSelection);
//static void GetFileName(UInt32 idx, std::string & filename, const std::string& folderPath, bool stripDriveName=false);

// utilities to write from source folders (on hdd) or to dst folders (on hdd) using OS read/write routines
UInt32 GetFileList(std::vector<AS_File::Path>& list, const AS_File::Path & dir, const AS_File::Path & extension);
AS_StorageError CreateEntryOnHD(const AS_Volume & vol, const AS_File::Kind & kind, const AS_File::Path & srcpath, const AS_File::Path & dstpath);
bool IsDirectoryPath(const AS_File::Path & fullPath);
AS_StorageError VerifyFile(const AS_Volume & vol, const AS_File::Path & srcPath, const AS_File::Path & dstPath, UInt32 compareMethod=0, UInt32 dstcompareMethod=0, bool verifyContents = true, bool verifyPermissions = false);
AS_StorageError VerifyDirectory(const AS_Volume & vol, const AS_File::Path & srcPath, const AS_File::Path & dstPath, UInt32 compareMethod=0, UInt32 dstcompareMethod=0, bool verifyPermissions = false);

//*************************************************************************************************//
// AS_String conversion functions
std::string GetACPName(const AS_File::Path & filenamebuf);
std::wstring GetWideName(const AS_File::Path & filenamebuf);
std::string GetStringFromUTF8(char* utf8String);

//*************************************************************************************************//
// helper enum -> string conversion functions
std::string GetVolumeFormatStr(UInt32 PropertyVal);
std::string GetMediaPropString(UInt32 property, UInt32 PropVal);
std::string GetPropertyStateString(UInt32 PropertyVal);
std::string GetTrackPropertyStateString(UInt32 PropertyVal);
std::string GetDevicePropStr(UInt32 Property, UInt32 PropVal);
std::string GetDeviceStateStr(UInt32 PropertyVal);
std::string GetDeviceTrayStr(UInt32 PropertyVal);
std::string GetMediaTypeCapableStr(UInt32 PropertyVal);
std::string GetDevicePropTypeStr(UInt32 PropertyVal);
std::string GetPropertyKindString(UInt32 PropertyVal);
std::string GetVolumeStateStr(UInt32 PropertyVal);
std::string GetVolumeFormatStr(UInt32 PropertyVal);
std::string GetDeviceBusStr(const AS_StorageDevice::Bus_Info & busInfo);
std::string GetDeviceAuthTypesStr(AS_StorageDevice::Authentication_Type & authInfo);


static std::vector<std::ifstream*> filestreams;
static int listCtr=0;
const int MAX_FILES=100;
static int autorun = 0;

static UInt32 cancelProgress = 0xffffffff;

SInt8* gBuffer;
SInt32 gBufferNum;
size_t gFileSize;
// globals for estimation in RTTD
AS_File::Count gNumFolders = 0;
AS_File::Count gNumFiles = 0;
AS_File::Size gVolumeSize = 0;
AS_File::Size gCapacity=0;

struct SummaryInfo
{
	UInt32 totalNum;
	UInt32 passed;
	UInt32 failed;
	UInt32 failedtoProcess;

};
//*************************************************************************************************//
/// AS_Imaging Callback routines for AS_Volume and AS_StorageDevice and stream callback for filling data
//*************************************************************************************************//


// helper function to fake progress for RTTD reading/writing
void OutputProgress(AS_File::Size pos, AS_File::Size totalSize)
{

	static AS_File::Size lastRTTDProgress = 1000;

	// don't do anything for small files
	if (pos < 32768)
		return;

	AS_File::Size posProgress = pos*100/totalSize;

	// only report every 10%
	if ((posProgress%10) != 0)
		return;

	if (posProgress != lastRTTDProgress)
	{
		lastRTTDProgress = posProgress;
		std::cout << "Progress RTTD operation: " << posProgress << " %" << std::endl;
	}

	return;

}
// Function typedef for reporting progress information
AS_StorageError AS_CALLBACK ProgressCallback(UInt32 percentcomplete, void *context)
{
      std::cout << "Progress:" << percentcomplete << "% complete" << std::endl << std::flush;

	  context;

   return AS_StorageError_None;
}

std::string GetCallbackOperationString(UInt32 op)
{
	char retStr[2048] = "";
	switch (op)
	{
	case AS_StorageDevice::JobType_Comparing:
		return "Comparing";
	case AS_StorageDevice::JobType_Writing:
		return "Writing";
	case AS_StorageDevice::JobType_Finalizing:
		return "Finalizing";
	case AS_StorageDevice::JobType_Aborting:
		return "Aborting";
	case AS_StorageDevice::JobType_Erasing:
		return "Erasing";
	case AS_StorageDevice::JobStatus_Idle:
		return "Idle";
	case AS_StorageDevice::JobType_Initializing:
		return "Initializing";
	default: 
		sprintf(retStr, "[%d]",op);
		break;
	};
	return retStr;
}

std::string GetCallbackStatusString(UInt32 status)
{
	char retStr[2048] = "";
	switch (status)
	{
	case AS_StorageDevice::JobStatus_Success:
		return "Success";
	case AS_StorageDevice::JobStatus_Running:
		return "Running";
	case AS_StorageDevice::JobStatus_BusyAborting:
		return "Busy Aborting";
	case AS_StorageDevice::JobStatus_PartialSuccess:
		return "Partial Success";
	case AS_StorageDevice::JobStatus_Idle:
		return "Idle";
	case AS_StorageDevice::JobStatus_Aborted:
		return "Aborted";
	default: 
		if (status & AS_StorageDevice::JobStatus_PartialSuccess)
			return "Partial Success";
		else if ((status & AS_StorageDevice::JobStatus_Error) == 0)
			return "Job Error - TODO: Query error status for more info";
		else
			sprintf(retStr, "[%d]",status);
		break;
	};
	return retStr;
}

AS_StorageError AS_CALLBACK DeviceInfo_Callback(const AS_StorageDevice::Info& info, UInt32 infoDword, void* infoPtr, void* callerUse)
{
   switch(info)
   {
   case AS_StorageDevice::Info_PromptForMediaInDevice:   // infoDword = unused, infoPtr = unused
      std::cout << "Prompt For Media In Device" << std::endl << std::flush;
      // TODO: wait for user to put media in and press key
      break;

   case AS_StorageDevice::Info_ProgressUpdate:   // infoDword = percent complete, infoPtr = Progress structure

	  if (infoPtr != 0)
	  {
		  AS_StorageDevice::Progress * ProgressInfo = reinterpret_cast<AS_StorageDevice::Progress *>(infoPtr);
		  if (ProgressInfo->ValidBytes > 0 && ProgressInfo->ValidBytes <= sizeof(AS_StorageDevice::Progress))
		  {
			  std::cout << "Detailed Status Info:" << std::endl;
			  std::cout << "--------------------------------------------------------" << std::endl;
			  std::cout << "Media ID is: " << ProgressInfo->DeviceInfo << std::endl;
			  std::cout << "Status of this media is: " << GetCallbackStatusString(ProgressInfo->ProgressStatus);
			  std::cout << ", Current operation is: " << GetCallbackOperationString(ProgressInfo->CurrentOperation) << std::endl;

			  if (ProgressInfo->CurrentOperation == AS_StorageDevice::JobType_Writing ||
				  ProgressInfo->CurrentOperation == AS_StorageDevice::JobType_Comparing)
			  {

				  	    if (callerUse != 0)
						{
							// for these sample copy operations, we assume 
							// we sent in dest(s) AS_STorageDevice ptrs
							AS_StorageDevice* ourDestList = (AS_StorageDevice*)(callerUse);
							if (ourDestList != 0)
								std::cout << "List of handles is "  << ourDestList->myHandle << std::endl;
						// find which device is being reported on (need to change callback strcut to use device handle so we can match
						//	while (ourDestList->myHandle != ProgressInfo->DeviceInfo)
						//	{
						//		++(ourDestList);
						//	}
						}
			  }

			  std::cout << std::endl << " CurrentBlock = " << ProgressInfo->CurrentSector;
			  std::cout << " out of Total = " << ProgressInfo->TotalSectors << std::endl;
			  std::cout << "--------------------------------------------------------" << std::endl;

			  // send cancel if set by user
			  if (infoDword > cancelProgress)
				  return AS_StorageError_UserCancelled;

		  }
	  }


	  std::cout << "Progress:" << infoDword << "% complete" << std::endl << std::flush;
      break;
   }

   return AS_StorageError_None;

}

AS_StorageError AS_CALLBACK Info_Callback(const AS_Volume::Info& info, UInt32 infoDword, void* infoPtr, void* callerUse)
{
   switch(info)
   {
   case AS_Volume::Info_PromptForMediaInDevice:   // infoDword = unused, infoPtr = unused
      std::cout << "Prompt For Media In Device" << std::endl << std::flush;
      // TODO: wait for user to put media in and press key
      break;

   case AS_Volume::Info_ProgressUpdate:   // infoDword = percent complete, infoPtr = Progress structure
      std::cout << "Progress:" << infoDword << "% complete" << std::endl << std::flush;
      // TODO: output infoPtr's Progress structure information
      break;

   case AS_Volume::Info_PromptForNextMedia: // infoDword = unused, infoPtr = unused
      std::cout << "Insert next media into the device, then press <enter>" << std::endl << std::flush;
      std::string s;
      std::cin >> s;
      break;
   }

   return AS_StorageError_None;
}

AS_StorageError AS_CALLBACK Device_ProgressCallback(UInt32 percentcomplete, void *context)
{
   std::cout << "Progress:" << percentcomplete << "% complete" << std::endl << std::flush;
   context;

    return AS_StorageError_None;
}

// callback simply reads from the file - should use a ring buffer to eliminate latency
UInt32 AS_CALLBACK StreamDataCallback(SInt64 pos, SInt8 * pBuffer, 
                                      SInt32 bytesRequested, 
                                      SInt32 * pBytesGiven, void * context)
{
   std::ifstream* ifstrm = static_cast<std::ifstream*>(context);

   ifstrm->seekg((std::streamoff)pos);
   std::cout << "streaming data from pos: " << pos << std::endl;
   ifstrm->read((char*)pBuffer, bytesRequested);
   *pBytesGiven = ifstrm->gcount();

   return 0;
}

//*************************************************************************************************//
// AS_StorageDevice.h sample implementation functions
//*************************************************************************************************//
AS_StorageError GetDeviceMediaState(const AS_StorageDevice & device, bool & Eraseable)
{
	AS_StorageError err;
	AS_StorageDevice::DeviceState State;
	err = AS_StorageDevice_TrayControl(device, AS_StorageDevice::Tray_Control_Close);

	AS_StorageDevice_GetState(device, State);
	if (State == AS_StorageDevice::State_Not_Ready)
	{
		puts("Please insert a media.");
		err = AS_StorageDevice_TrayControl(device, AS_StorageDevice::Tray_Control_Open);
		return AS_StorageError_None;
    }

	AS_StorageDevice::MediaState MedState;

	UInt32 ret_data_size = 0;
	AS_StorageError Err = AS_StorageDevice_GetMediaProperty(device,
                                    AS_StorageDevice::MedProp_State,
                                    sizeof(MedState),
                                    &MedState,
                                    &ret_data_size, 0, 0, 0, 0);

	if (Err == AS_StorageError_None)
	{
		char Display[128];
		sprintf(Display,"ASTest: Media States: ");

		if (MedState & AS_StorageDevice::MediaIsReadable)
			strcat(Display, "Readable ");

		if (MedState & AS_StorageDevice::MediaIsWritable)
			strcat(Display, "Writable ");

		if (MedState & AS_StorageDevice::MediaIsAppendable)
			strcat(Display, "Appendable ");

		if (MedState & AS_StorageDevice::MediaIsRewritable)
			strcat(Display, "Rewrite ");

		if (MedState & AS_StorageDevice::MediaIsErasable)
		{
			strcat(Display, "Erasable ");
			Eraseable=true;
		}
		if (MedState & AS_StorageDevice::MediaIsProtected)
		{
			strcat(Display, "Protected ");
		}
		if (MedState & AS_StorageDevice::MediaIsBlank)
		{
			strcat(Display, "Blank ");
		}
		std::cout << Display << std::endl;
	}
   
	if (Err != AS_StorageError_None)
		return Err;

	return AS_StorageError_None;

}

AS_StorageError GetDevicePropertyInfo(const AS_StorageDevice & device)
{
	AS_StorageError err = AS_StorageError_None;
	wchar_t DeviceInfo[256];
	AS_StorageDevice::UnicodeString UnicodeDeviceInfo[256];
	char strDeviceInfo[256];
	size_t wstrLength=0;
	UInt32 ret_data_size=0;

	err = AS_StorageDevice_GetDeviceProperty(device,AS_StorageDevice::DevProp_Description,sizeof(UnicodeDeviceInfo),
						&UnicodeDeviceInfo, &ret_data_size, 0, 0, 0, 0);

	if (err == AS_StorageError_None)
	{
		memset(strDeviceInfo,0,sizeof(strDeviceInfo));
		wstrLength=wcslen((wchar_t*)(UnicodeDeviceInfo));
		if (wcslen > 0)
			wcstombs(strDeviceInfo, (wchar_t*)(UnicodeDeviceInfo), wstrLength);

		std::cout << "Device description is: " << strDeviceInfo << std::endl;
		
		err = AS_StorageDevice_GetDeviceProperty(device,AS_StorageDevice::DevProp_DrivePath,sizeof(DeviceInfo),
						&DeviceInfo, &ret_data_size, 0, 0, 0, 0);

		if (err == AS_StorageError_None)
		{
			memset(strDeviceInfo,0,sizeof(strDeviceInfo));
			wstrLength=wcslen(DeviceInfo);
			if (wcslen > 0)
				wcstombs(strDeviceInfo, DeviceInfo, wstrLength);
			std::cout << "Device drive letter is: " << strDeviceInfo << std::endl;
		}
		else
			return err;
	}
	else
		return err;

	UInt32 PropVal=0;
	std::string StringVal;

	err = AS_StorageDevice_GetDeviceProperty(device,AS_StorageDevice::DevProp_DevType,sizeof(PropVal),
						&PropVal, &ret_data_size, 0, 0, 0, 0);

	if (err == AS_StorageError_None)
	{
		StringVal = GetDevicePropStr(AS_StorageDevice::DevProp_DevType, PropVal);
		std::cout << "Device type is: " << StringVal << std::endl;
	}
	else
		return err;

	err = AS_StorageDevice_GetDeviceProperty(device,AS_StorageDevice::DevProp_MedTypesCapable,sizeof(PropVal),
						&PropVal, &ret_data_size, 0, 0, 0, 0);

	if (err == AS_StorageError_None)
	{
		StringVal = GetDevicePropStr(AS_StorageDevice::DevProp_MedTypesCapable, PropVal);
		std::cout << "Device media types capable are: " << StringVal << std::endl;
	}
	else
		return err;

	err = AS_StorageDevice_GetDeviceProperty(device,AS_StorageDevice::DevProp_TrayStatus,sizeof(PropVal),
						&PropVal, &ret_data_size, 0, 0, 0, 0);
	
	if (err == AS_StorageError_None)	
	{
		StringVal = GetDevicePropStr(AS_StorageDevice::DevProp_TrayStatus, PropVal);
		std::cout << "Device tray status is: " << StringVal << std::endl;
	}
	else
		return err;

	AS_StorageDevice::DeviceState state;

	err = AS_StorageDevice_GetState(device, state);
	
	if (err == AS_StorageError_None)
	{
		StringVal = GetDeviceStateStr(state);
		std::cout << "Device state is: " << StringVal << std::endl;
	}
	else
		return err;

	AS_StorageDevice::Bus_Info busInfo;

	err = AS_StorageDevice_GetDeviceProperty(device, AS_StorageDevice::DevProp_Bus_Info, sizeof(busInfo),
		&busInfo, &ret_data_size, 0, 0, 0, 0);

	if (err == AS_StorageError_None)
	{
		StringVal = GetDeviceBusStr(busInfo);
		std::cout << "Device businfo is: " << StringVal << std::endl;
	}


	err = AS_StorageDevice_GetDeviceProperty(device,AS_StorageDevice::DevProp_MaxTransferSize,sizeof(PropVal),
						&PropVal, &ret_data_size, 0, 0, 0, 0);
	if (err == AS_StorageError_None)
	{
		std::cout << "Device max transfer size is: " << PropVal << std::endl;
	}

	AS_StorageDevice::Authentication_Type deviceAuthTypes;

	err = AS_StorageDevice_GetDeviceProperty(device, AS_StorageDevice::DevProp_AuthenTypes, sizeof(deviceAuthTypes),
		&deviceAuthTypes, &ret_data_size, 0, 0, 0, 0);

	if (err == AS_StorageError_None)
	{
		StringVal = GetDeviceAuthTypesStr(deviceAuthTypes);
		std::cout << "Device authentication support is: " << StringVal << std::endl;
	}
	

	return err;

}

AS_StorageError SetDevicePropertyInfo(const AS_StorageDevice & device, AS_StorageDevice::MediaProp_Type Property, UInt32 propertyVal)
{
	AS_StorageError err = AS_StorageError_None;

	err = AS_StorageDevice_SetMediaProperty(device, Property, sizeof(propertyVal),&propertyVal, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	return err;
}


UInt32 CycleDrives()
{
	UInt32 idx=0;
	AS_StorageError err = AS_StorageError_None;
	AS_StorageDevice device;

	while (err == AS_StorageError_None)
	{
		err = AS_OpenStorageDevice(idx,device);
		if (err == AS_StorageError_None)
		{
			std::cout << "Device# : " << idx << std::endl;

			GetDevicePropertyInfo(device);

			err = AS_StorageDevice_Close(device);
			idx++;
		}
		else
			return (idx-1);

	}
	std::cout << "We didn't find any optical recording devices" << std::endl;
	return idx;

}

AS_StorageError GetMediaSize(const AS_StorageDevice& device, UInt64 & FreeBytes)
{

	AS_StorageError err;

	UInt32 outdatasize=0;
	err = AS_StorageDevice_GetMediaProperty(device, AS_StorageDevice::MedProp_Bytes_Free, sizeof(FreeBytes), &FreeBytes, &outdatasize, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	std::cout << "Currently " << FreeBytes << " bytes free on media" << std::endl;

	return err;

}

AS_StorageError GetMediaTrackInfo(const AS_StorageDevice & device)
{
	AS_StorageError err = AS_StorageError_None;

	UInt32 PropertyInfo, numTracks, numSessions =0;
	UInt32 ret_data_size=0;

	std::cout << std::endl;
	std::cout << "Media Track Information: "  << std::endl;
	
	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Num_Tracks,sizeof(numTracks),
					&numTracks, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	std::cout << "Tracks: " << numTracks << std::endl;


	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Num_Sessions,sizeof(numSessions),
					&numSessions, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	std::cout << "Sessions: " << numSessions << std::endl;


	std::string propString;

	UInt32 mediaState;
	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_State,sizeof(mediaState),
					&mediaState, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	if (mediaState & AS_StorageDevice::MediaIsInvisible)
		numTracks++;

	for (UInt32 trackCtr=1; trackCtr <= numTracks; trackCtr++)
	{

		std::cout << "Track# " << trackCtr << ": ";
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_BlockSize,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;

		std::cout << "BlockSize: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_WriteFrameSize,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;

		std::cout << "WriteFrameSize: " << PropertyInfo << std::endl;

		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_OptimalFrameSize,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;

		std::cout << "OptimalFrameSize: " << PropertyInfo << std::endl;

		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_TrkType,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;

		std::cout << "TrackType: " << PropertyInfo << std::endl;

		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_RecordMode,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;

		std::cout << "Record Mode: " << PropertyInfo << std::endl;

		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_SessionNum,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;

		std::cout << "Session #: " << PropertyInfo << std::endl;

		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_StartAddr,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;

		std::cout << "Start Addr: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_Length,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;

		std::cout << "Length: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_RecLength,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;
		std::cout << "Rec Length: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_LastRecAddr,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;
		std::cout << "Last Rec Addr: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_AppendAddr,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;
		std::cout << "Append Addr: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_State,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;
		std::cout << "IOState: " << PropertyInfo << std::endl;
		std::cout << "IOStates: " << GetTrackPropertyStateString(PropertyInfo) << std::endl;

		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_CurFormatType,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;
		std::cout << "CurFormatType: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_FormatTypes,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;
		std::cout << "FormatTypes: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_Copyrighted,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;
		std::cout << "Copyrighted: " << PropertyInfo << std::endl;
		err = AS_StorageDevice_GetTrackProperty(device, trackCtr, AS_StorageDevice::TrackProp_Pregap,sizeof(PropertyInfo),
						&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
		if (err != AS_StorageError_None)
			return err;
		std::cout << "Pregap: " << PropertyInfo << std::endl;
	}
   return err;
}
	
AS_StorageError GetDeviceMediaInfo(const AS_StorageDevice & device)
{
	AS_StorageError err = AS_StorageError_None;


	UInt32 PropertyInfo=0;
	UInt64 bytesFree=0;
	UInt64 bytesUsed=0;
	UInt32 WriteSpeed=0;
	UInt32 WriteSpeedHundreths=0;
	UInt32 ReadSpeed=0;
	UInt32 ReadSpeedHundreths=0;
	UInt32 ret_data_size=0;
	std::string propString;

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Num_Layers,sizeof(PropertyInfo),
					&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	std::cout << "Layers: " << PropertyInfo << std::endl;

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Kind,sizeof(PropertyInfo),
					&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;
	propString = GetMediaPropString(AS_StorageDevice::MedProp_Kind, PropertyInfo);
	std::cout << "Media Kind: " << propString << std::endl;
	
	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_State,sizeof(PropertyInfo),
					&PropertyInfo, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;
	propString = GetMediaPropString(AS_StorageDevice::MedProp_State, PropertyInfo);
	
	std::cout << "Media State: " << propString << std::endl;

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Bytes_Used,sizeof(bytesUsed),
					&bytesUsed, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;
	std::cout << "Media Bytes used: " << bytesUsed << std::endl;

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Bytes_Free,sizeof(bytesFree),
					&bytesFree, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;
	std::cout << "Media Bytes free: " << bytesFree << std::endl;

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_CurRead_DataRates,sizeof(ReadSpeed),
					&ReadSpeed, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;
	std::cout << "Read speed (kBps): " << ReadSpeed << std::endl;

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_CurRead_Hundredth_X,sizeof(ReadSpeedHundreths),
					&ReadSpeedHundreths, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;
	std::cout << "Read Speed (100th's X): " << ReadSpeedHundreths << std::endl;
   


	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_CurWrite_DataRates,sizeof(WriteSpeed),
					&WriteSpeed, &ret_data_size, 0, 0, 0, 0);
//	if (err != AS_StorageError_None)
//		return err;
	std::cout << "Write speed (kBps): " << WriteSpeed << std::endl;

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_CurWrite_Hundredth_X,sizeof(WriteSpeedHundreths),
					&WriteSpeedHundreths, &ret_data_size, 0, 0, 0, 0);
//	if (err != AS_StorageError_None)
//		return err;
	std::cout << "Write Speed (100th's X): " << WriteSpeedHundreths << std::endl;
   

	UInt32 WriteDataRates[128];
	memset(WriteDataRates, 0, 128);

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_WriteList_DataRates,sizeof(WriteDataRates),
					&WriteDataRates, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;
	int i=0;
	while (WriteDataRates[i] != 0)
	{
		std::cout << "Write rates list(kBps): " << WriteDataRates[i] << std::endl;
		i++;
	}

	UInt32 WriteDataRateHundreths[128];
	memset(WriteDataRateHundreths, 0, 128);
	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_WriteList_Hundredth_X,sizeof(WriteDataRateHundreths),
					&WriteDataRateHundreths, &ret_data_size, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;
	
	i=0;
	while (WriteDataRateHundreths[i] != 0)
	{
		std::cout << "Write rate list 100th's: " << WriteDataRateHundreths[i] << std::endl;
		i++;
	}

	AS_StorageDevice::Authentication_Type curAuthType;

	err = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_CurAuthenType,sizeof(curAuthType),
					&curAuthType, &ret_data_size, 0, 0, 0, 0);
	
	if (err == AS_StorageError_None)
	{
		std::string StringVal = GetDeviceAuthTypesStr(curAuthType);
		std::cout << "Current media authentication support is: " << StringVal << std::endl;
	}

	return err;
}

//*************************************************************************************************//
// AS_Volume.h sample implementation functions
//*************************************************************************************************//

AS_StorageError OpenVolume(const AS_StorageDevice & device, AS_Volume & volume)
{
	AS_StorageError retVal = AS_StorageError_None;
	AS_Volume::FS_Type volType;

	std::cout << "Opening volume" << std::endl;
	retVal = AS_StorageDevice_OpenVolume(device, volType, Info_Callback, NULL, volume);
	if (retVal != AS_StorageError_None)
		return retVal;

	if (volType == AS_Volume::FS_None)
	{
		std::cout << "**********Volume type is None after OpenVol???************" << std::endl;
		return retVal;
	}
	return AS_StorageError_None;
}

AS_StorageError FormatVolume(const AS_Volume & volume, AS_Volume::FS_Type FormatType, AS_Volume::AccessFlag volumeAccessType = AS_Volume::Mastering)
{
	AS_StorageError retVal = AS_StorageError_None;

	// Volume Format
	std::cout << "Format volume" << std::endl;

	retVal = AS_Volume_Create(volume,FormatType, AS_StringA("AS_ImagingVolume"), volumeAccessType, Info_Callback, NULL);
	if (retVal != AS_StorageError_None)
		return retVal;

	std::cout << "Done Format volume" << std::endl;
	return AS_StorageError_None;
}

AS_StorageError CloseVolume(AS_Volume & volume)
{
	AS_StorageError retVal = AS_StorageError_None;

	std::cout << "Closing volume" << std::endl;

	retVal = AS_Volume_Close(volume);
	if (retVal != AS_StorageError_None)
		return retVal;
	std::cout << "Done closing volume" << std::endl;
	return AS_StorageError_None;
}

AS_StorageError FlushVolume(const AS_Volume & volume, AS_Volume::Flush_Border_Options options = AS_Volume::Border_Open)
{
	AS_StorageError retVal = AS_StorageError_None;

	// Flush / commit changes to disc

	std::cout << "Flush volume" << std::endl;
	retVal = AS_Volume_Flush(volume, Info_Callback, NULL, options);
	if (retVal != AS_StorageError_None)
		return retVal;
	std::cout << "Done Flush volume" << std::endl;
  
  	for (std::vector<std::ifstream*>::const_iterator itr =  filestreams.begin();
  		itr != filestreams.end(); ++itr)
  	{
  		(*itr)->close();
  		delete *itr;
  	}
  	filestreams.clear();
  
	return AS_StorageError_None;
}

AS_StorageError MountExistingDisc(const AS_Volume & volume, AS_Volume::AccessFlag volumeAccessType = AS_Volume::Mastering)
{
	AS_StorageError retVal = AS_StorageError_None;

	std::cout << "Mount volume" << std::endl;

	retVal = AS_Volume_Mount(volume, volumeAccessType, Info_Callback, NULL);
	if (retVal != AS_StorageError_None)
		return retVal;
	std::cout << "Done Mount volume" << std::endl;
	return AS_StorageError_None;
}

AS_StorageError GetDirInfo(const AS_Volume & volume, const AS_File::Path& inPath, bool readFileContents)
{
   AS_StorageError err;

	// Get root dir info
   AS_File::Kind fileType = AS_File::File_IsDir;
	AS_File::Size numChildren;
	AS_File::Size dummyChildNum;
	err = AS_Volume_GetEntryInfo(volume, inPath, fileType, numChildren);
	if (err != AS_StorageError_None)
		return err;

	for (SInt32 idx = 0; idx < numChildren; idx++)
	{
		// Walk a directory
		SInt32 pathLen;
		char namebuf[260];
		memset(namebuf, 0, sizeof(namebuf));
		AS_File::Path outFullPath;
		outFullPath.type = AS_String::Type_UTF8;
		outFullPath.size = sizeof(namebuf);
		outFullPath.data = namebuf;
		err = AS_Volume_GetEntry(volume, inPath,idx,&pathLen,outFullPath);
		if (err != AS_StorageError_None)
			return err;

		err = AS_Volume_GetEntryInfo(volume, outFullPath, fileType, dummyChildNum);
		if (err != AS_StorageError_None)
			return err;

      std::string name;
	  name = GetACPName(outFullPath);
//	  name = to_string(string8(static_cast<char8*>(outFullPath.data)));
	  if (fileType == AS_File::File_IsDir)
		{
			std::cout << "Dir entry is " << name << std::endl;
			GetDirInfo(volume, outFullPath, readFileContents);
		}
		else
		{
//			std::cout << "File entry is %s\n",outFullPath);
			std::cout << "Name               Size                Created              Mod " << std::endl;
			err = GetFileInfo(volume, outFullPath, readFileContents);
			if (err != AS_StorageError_None)
				return err;
			std::cout << std::endl;
		}
	}
	if (err != AS_StorageError_None)
		return err;

	return AS_StorageError_None;
}

AS_StorageError GetDirectoryInfo(const AS_Volume & volume, const std::string & dirPath)
{
	AS_StorageError retVal = AS_StorageError_None;

	// Get directory listing from media
	AS_File::Kind fileType = AS_File::File_IsDir;
	AS_File::Size numChildren;
	retVal = AS_Volume_GetEntryInfo(volume, AS_StringA(dirPath), fileType, numChildren);
	if (retVal != AS_StorageError_None)
		return retVal;

	std::cout << "Found " << numChildren << " number of children to root dir" << std::endl;

	if (fileType != AS_File::File_IsDir)
		return retVal;

	std::cout << "Open and read file from volume" << std::endl;
	AS_StringA rootPath = "/";

	retVal = GetDirInfo(volume, rootPath, false);
	if (retVal != AS_StorageError_None)
		return retVal;

	std::cout << "Done Open and read file from volume" << std::endl;
	return AS_StorageError_None;
}

AS_StorageError GetVolumeProperty(const AS_Volume& volume, AS_Volume::Property PropertyType)
{

	AS_StorageError err;
	UInt32 outdatasize=0;
	std::string PropertyString;
	switch (PropertyType)
	{
      case AS_Volume::Prop_FS_Type:
		{
			AS_Volume::FS_Type VolFormat;
			err = AS_Volume_GetProperty(volume, PropertyType, sizeof(VolFormat), &VolFormat, &outdatasize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;

			PropertyString = GetVolumeFormatStr(VolFormat);
			std::cout << "Volume format: " << PropertyString << std::endl;
		}
		break;

		case AS_Volume::Prop_State:
		{
			AS_Volume::State VolState;
			err = AS_Volume_GetProperty(volume, PropertyType, sizeof(VolState), &VolState, &outdatasize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;
			PropertyString = GetVolumeStateStr(VolState);
			std::cout << "Volume State: " << PropertyString << std::endl;
		}
		break;

		case AS_Volume::Prop_Name:
		{
			char VolumeName[1024];
			err = AS_Volume_GetProperty(volume, PropertyType, 256, VolumeName, &outdatasize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;

         // convert to std::string for output		 
			std::string name = GetStringFromUTF8(VolumeName);
			std::cout << "Volume Name: " << name << std::endl;
		}
		break;

		case AS_Volume::Prop_BlockSize:

			UInt32 blocksize;
			err = AS_Volume_GetProperty(volume, PropertyType, sizeof(blocksize), &blocksize, &outdatasize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;
			std::cout << "Block size: " << blocksize << std::endl;

		break;

		case AS_Volume::Prop_FreeBlocks:
			UInt32 FreeBlocks;
			err = AS_Volume_GetProperty(volume, PropertyType, sizeof(FreeBlocks), &FreeBlocks, &outdatasize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;
			std::cout << "Free Blocks: " << FreeBlocks << std::endl;

		break;

		case AS_Volume::Prop_DateCreated:
			AS_Volume::DateTime volumeCreateDate;
			err = AS_Volume_GetProperty(volume, PropertyType, sizeof(volumeCreateDate), &volumeCreateDate, &outdatasize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;
			std::cout << "Date Created: ";
			OutputDate(volumeCreateDate);
			std::cout << "" << std::endl;;
		break;

		case AS_Volume::Prop_DateModified:
			AS_Volume::DateTime volumeModDate;

			err = AS_Volume_GetProperty(volume, PropertyType, sizeof(volumeModDate), &volumeModDate, &outdatasize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;
			std::cout << "Date Modified: ";
			OutputDate(volumeModDate);
			std::cout << std::endl;
		break;
		case AS_Volume::Prop_Delimiter:
		{
			SInt8 VolumeDelimiter;
			err = AS_Volume_GetProperty(volume, PropertyType, 256, &VolumeDelimiter, &outdatasize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;
			std::cout << "Volume delimiter: " << VolumeDelimiter << std::endl;
		}
		break;

		

		default:
			std::cout << "Invalid property" << std::endl;
			break;
	};

	return err;
}


AS_StorageError GetVolumeSize(const AS_Volume& volume, UInt64 & freeBytes)
{

	AS_StorageError err;
	UInt32 outdatasize;
	UInt64 FreeBlocks;
	UInt32 blocksize;

	err = AS_Volume_GetProperty(volume, AS_Volume::Prop_BlockSize, sizeof(blocksize), &blocksize, &outdatasize, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	err = AS_Volume_GetProperty(volume, AS_Volume::Prop_FreeBlocks, sizeof(FreeBlocks), &FreeBlocks, &outdatasize, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	freeBytes = FreeBlocks * blocksize;

	std::cout << "Currently " << freeBytes << " bytes free on media\n" << std::endl;

	return err;
}

AS_StorageError GetEntryInfo(const AS_Volume & volume, const std::string & fullPath)
{

	AS_StorageError err = AS_StorageError_None;
	AS_StringA filePath = fullPath;
	UInt32 propSize = 0;

	char fileName[1024];
	err = AS_Volume_GetEntryProperty(volume, filePath, AS_File::Prop_Name, 256, &fileName, &propSize, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

   // convert to std::string
   std::string name = GetStringFromUTF8(fileName);
	std::cout << name << "     ";

	UInt64 fileSize;
	err = AS_Volume_GetEntryProperty(volume, filePath, AS_File::Prop_Size, sizeof(fileSize),&fileSize,&propSize, 0, 0, 0, 0); 
	if (err != AS_StorageError_None)
		return err;

	std::cout << fileSize << "     ";

	AS_Volume::DateTime fileCreateDate;
	err = AS_Volume_GetEntryProperty(volume, filePath, AS_File::Prop_DateCreated, sizeof(fileCreateDate),&fileCreateDate,&propSize, 0, 0, 0, 0); 
	if (err != AS_StorageError_None)
		return err;
	OutputDate(fileCreateDate);

	AS_Volume::DateTime fileModDate;
	err = AS_Volume_GetEntryProperty(volume, filePath, AS_File::Prop_DateModified, sizeof(fileModDate),&fileModDate,&propSize, 0, 0, 0, 0); 
	if (err != AS_StorageError_None)
		return err;

	OutputDate(fileModDate);
	std::cout << std::endl << std::endl;
	return err;
}



AS_StorageError GetVolumeInfo(const AS_Volume& volume)
{
	AS_StorageError err;
	err = GetVolumeProperty(volume,AS_Volume::Prop_FS_Type);
	if (err != AS_StorageError_None)
		return err;
	err = GetVolumeProperty(volume,AS_Volume::Prop_State );
	if (err != AS_StorageError_None)
		return err;
	err = GetVolumeProperty(volume,AS_Volume::Prop_Name );
	if (err != AS_StorageError_None)
		return err;
	err = GetVolumeProperty(volume,AS_Volume::Prop_BlockSize );
	if (err != AS_StorageError_None)
		return err;
	err = GetVolumeProperty(volume,AS_Volume::Prop_FreeBlocks );
	if (err != AS_StorageError_None)
		return err;
	err = GetVolumeProperty(volume,AS_Volume::Prop_DateModified );
	if (err != AS_StorageError_None)
		return err;
	err = GetVolumeProperty(volume,AS_Volume::Prop_DateCreated );
	if (err != AS_StorageError_None)
		return err;
	err = GetVolumeProperty(volume,AS_Volume::Prop_Delimiter );
	if (err != AS_StorageError_None)
		return err;

	return err;
}


AS_StorageError SetVolumeInfo(const AS_Volume & volume, AS_Volume::Property PropertyType, const std::string & newVolName, const UInt64 newFileSize, const AS_Volume::DateTime & date, const SInt8 newDelimiter)
{

	AS_StorageError err = AS_StorageError_None;
	UInt32 propSize=0;

	switch (PropertyType)
	{
		case AS_Volume::Prop_Name:
		{
			AS_StringA fileName = newVolName;
			err = AS_Volume_SetProperty(volume, PropertyType, sizeof(fileName), &fileName, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;
		}
		break;
		case AS_Volume::Prop_Delimiter:
		{
			SInt8 delim;
			delim = newDelimiter;
			err = AS_Volume_SetProperty(volume, PropertyType, sizeof(delim),&delim, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;
		}
		break;
		case AS_Volume::Prop_DateCreated:
		{			
			AS_Volume::DateTime fileCreateDate;
			fileCreateDate = date;
			err = AS_Volume_SetProperty(volume, PropertyType, sizeof(fileCreateDate),&fileCreateDate, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;
		}
		break;
		case AS_Volume::Prop_DateModified:
		{
			AS_Volume::DateTime fileModDate;
			fileModDate = date;
			err = AS_Volume_SetProperty(volume, PropertyType, sizeof(fileModDate),&fileModDate, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;
		}
		break;
	};

	return err;
}

AS_StorageError SetEntryInfo(const AS_Volume & volume, const std::string & fullPath, UInt32 PropertyType, const std::string & newFileName, const UInt64 newFileSize, const AS_Volume::DateTime & date)
{

	AS_StorageError err = AS_StorageError_None;
	AS_StringA filePath = fullPath;
	UInt32 propSize=0;

	switch (PropertyType)
	{
		case AS_File::Prop_Name:
		{
			AS_StringA fileName = newFileName;
			err = AS_Volume_SetEntryProperty(volume, filePath, AS_File::Prop_Name, 256, &fileName, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;
		}
		break;
		case AS_File::Prop_Size:
		{
			UInt64 fileSize;
			fileSize = newFileSize;
			err = AS_Volume_SetEntryProperty(volume, filePath, AS_File::Prop_Size, sizeof(fileSize),&fileSize, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;
		}
		break;
		case AS_File::Prop_DateCreated:
		{			
			AS_Volume::DateTime fileCreateDate;
			fileCreateDate = date;
			err = AS_Volume_SetEntryProperty(volume, filePath, AS_File::Prop_DateCreated, sizeof(fileCreateDate),&fileCreateDate, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;
		}
		break;
		case AS_File::Prop_DateModified:
		{
			AS_Volume::DateTime fileModDate;
			fileModDate = date;
			err = AS_Volume_SetEntryProperty(volume, filePath, AS_File::Prop_DateModified, sizeof(fileModDate),&fileModDate, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;
		}
		break;
	};

	return err;
}



AS_StorageError CreateEntry(const AS_Volume & volume, const std::string & entryFullPath, const AS_File::Kind & fileKind)
{
   AS_StorageError err;

   err = AS_Volume_CreateEntry(volume, AS_StringA(entryFullPath), fileKind, true);
   if (err != AS_StorageError_None)
      return err;

   return AS_StorageError_None;

}

AS_StorageError DeleteItem(const AS_Volume & volume, const std::string & deletePathName)
{
   AS_StorageError err;
   AS_StringA fpone = deletePathName;
   
   err = AS_Volume_DeleteEntry(volume, fpone, true);
   if (err != AS_StorageError_None)
      return err;

   return AS_StorageError_None;

}

size_t GetFileSize(const AS_File::Path & srcPath)
{
	std::ifstream ifstrm;
	size_t sizeOfFile=0;
	ifstrm.open(reinterpret_cast<char*>(srcPath.data),std::ios_base::binary);
	if (ifstrm.is_open())
	{
		ifstrm.seekg(0,std::ios_base::end);
		sizeOfFile = ifstrm.tellg();
		ifstrm.seekg(0, std::ios_base::beg);
	}

	ifstrm.close();

	return sizeOfFile;


}

bool CanFitFile(UInt64 filesize)
{
	UInt64 metaDataEstimate = (1900 + (((2* gNumFolders+1) + gNumFiles+1) * 120 / 100)) * 2048;// 120/100 is a 20% overestimation factor.
	if (metaDataEstimate < 1504)
		metaDataEstimate = 1504;
	UInt64 freespace = gCapacity - metaDataEstimate - gVolumeSize;
	if (freespace < filesize)
		return false;

	return true;


}
bool CanFitFile(const AS_File::Path & srcPath)
{
	return CanFitFile(GetFileSize(srcPath));
}

bool GetEntryThatFits(std::vector<AS_File::Path> & filelist, AS_File::Count* foundIndex)
{
	std::vector<AS_File::Path>::const_iterator itr = filelist.begin();
	bool found = false;
	*foundIndex = 0;
	AS_File::Count index = 0;
	std::cout << "Searching list of size= " << (UInt32)filelist.size() << std::endl;
	while (!found && (itr != filelist.end()))
	{
		if (!IsDirectoryPath(*itr))
		{
			if (CanFitFile(*itr))
			{
				found = true;
				*foundIndex = index;
			}
		}

		++index;++itr;

	}

	return found;
}

AS_StorageError CopyHDFilesToMedia(const AS_StorageDevice & device, const AS_Volume & vol, const AS_File::Path & srcpath, const AS_File::Path & dstpath)
{
	AS_StorageError err = AS_StorageError_None;

	AS_File dstFile;
	
	AS_File::Size curPos = 0;
	AS_File::Count numWritten;

	UInt32 outDataSize=0;


	size_t sizeOfFile = GetFileSize(srcpath);
	UInt64 freespace=0xffffffff;

	if (gCapacity > 0)
	{
		UInt64 metaDataEstimate = ((1900 + (2 * gNumFolders) + gNumFiles) * 120 / 100) * 2048;// + (800*1024); //pad estimate with 800k for testing
		freespace = gCapacity - metaDataEstimate - gVolumeSize;

		std::cout << "Formula estimation is " << metaDataEstimate << " bytes for metadata" << std::endl;
		std::cout << "Data written to volume is " << gVolumeSize << "bytes for data" << std::endl;
		std::cout << "Freespace is now: " << freespace << " bytes" << std::endl;

		if (freespace < metaDataEstimate+sizeOfFile)
		{

			UInt32 FreeBlocks, ret_size = 0;
			err = AS_Volume_GetProperty(vol, AS_Volume::Prop_FreeBlocks, sizeof(FreeBlocks), &FreeBlocks, &ret_size);
			if (err != AS_StorageError_None)
				std::cout << "failed to retrieve freeblocks" << std::endl;
			else
				std::cout << "Volume property reports freeblocks = " << (UInt64)FreeBlocks *2048<< "  bytes" << std::endl;

			if (sizeOfFile > (UInt64)(FreeBlocks*2048))
				return AS_StorageError_NotEnoughStorage;
		}

	}
	if (sizeOfFile > freespace)
	{
		std::cout << " failing to write further files - sizeOfFile = " << (UInt32)sizeOfFile << " FreeSpace = " << freespace << std::endl;
		return AS_StorageError_NotEnoughStorage;
	}

	std::ifstream ifstrm;
	ifstrm.open(reinterpret_cast<char*>(srcpath.data),std::ios_base::binary);

	if (!ifstrm.is_open())
		return AS_StorageError_ReadError;

	err = AS_Volume_OpenFile(vol, dstpath, AS_File::Append, dstFile);
	if (err != AS_StorageError_None)
	{
		if (err == AS_StorageError_InvalidOperation)
		{
			std::cout << "must be adding same named file more than once - skipping file appends" << std::endl;
			return AS_StorageError_None;		
		}
		return err;
	}

	while (curPos < sizeOfFile)
	{

		size_t numToRead = 32768;
		if (curPos + numToRead > sizeOfFile)
			numToRead = sizeOfFile - (size_t)curPos;

	    char * readBuffer = new char[(const int)numToRead];
		ifstrm.read(readBuffer, (std::streamsize)numToRead);
		if (err != AS_StorageError_None)
		{
			std::cout << "Write error encountered, close file and return error" << std::endl;
			ifstrm.close();
			AS_File_Close(dstFile);
			gVolumeSize += curPos;
			return err;
		}

		err = AS_File_Write(dstFile, numToRead, readBuffer, &numWritten);
		delete[] readBuffer;
		if (err != AS_StorageError_None)
		{
			std::cout << "Write error encountered, close file and return error" << std::endl;
			ifstrm.close();
			AS_File_Close(dstFile);
			gVolumeSize += curPos;
			return err;
		}
		
		curPos += numToRead;

		OutputProgress(curPos, sizeOfFile);
	
	}

	gVolumeSize += sizeOfFile;
	ifstrm.close();

	err = AS_File_Close(dstFile);
	if (err != AS_StorageError_None)
		return err;

	
	return err;


}

AS_StorageError FillFilesListing(std::vector<AS_File::Path> & list, const AS_File::Path& srcPath)
{
   UInt32 numFilesFound = 0;
   std::vector<AS_File::Path> loclist;
   numFilesFound = GetFileList(loclist, srcPath, AS_StringA("*"));

   std::cout << "Found " << numFilesFound << " items in directory " << GetACPName(srcPath)<< std::endl;
   // we must've been given a file not a folder so let's just write that one
   if (numFilesFound == 0)
   {
	   list.push_back(srcPath);
   }

   for (std::vector<AS_File::Path>::const_iterator itr = loclist.begin();
	   itr != loclist.end(); ++itr)
   {
	   list.push_back(*itr);
	   bool isDir = IsDirectoryPath(*itr);
	   if (isDir)
	   {
		   // add in current dir to list
		   FillFilesListing( list, *itr);
	   }

   }

   return AS_StorageError_None;
}


AS_StorageError VerifyVolume(const AS_Volume & volume, std::vector<AS_File::Path> & filesList, const AS_File::Path & destPath, UInt32 srcCompareMethod, UInt32 dstCompareMethod, UInt32 externalAppUsed, UInt32 dstexternalAppUsed, UInt32 readContents, UInt32 verifyPermissions, SummaryInfo & info)
{
	AS_StorageError err = AS_StorageError_None;

	bool verifyContents = true;
	if (readContents == 0)
		verifyContents = false;

	// TODO: do something for external apps 
	externalAppUsed;
	dstexternalAppUsed;

	for (std::vector<AS_File::Path>::const_iterator itr = filesList.begin();
		itr != filesList.end(); ++itr)
	{

		bool isDir = IsDirectoryPath(*itr);
		if (isDir)
		{
			std::string newDstPath(reinterpret_cast<char*>(destPath.data));
			char newSrcStr[256];
			memcpy(newSrcStr, (char*)(itr->data), itr->size);
			newSrcStr[itr->size] = '\0';
			std::string newSrcString((char*)newSrcStr);

			size_t firstSlashPos = newSrcString.find_first_of((char*)("\\"));
			if (firstSlashPos == std::string::npos )
				firstSlashPos = newSrcString.find_first_of((char*)("/"));

			// support dest path as "/"
			if (newDstPath != "/")
				newDstPath += std::string((char*)("/"));

			newDstPath += std::string(newSrcString).substr(firstSlashPos+1);
			char newDstStr[256];
			memcpy(newDstStr, newDstPath.c_str(), newDstPath.length());
			newDstStr[newDstPath.size()] = '\0';
			std::cout << "Verifying src dir: " << GetACPName(*itr) << " to dest: " << newDstStr << std::endl;

			err = VerifyDirectory(volume, *itr, AS_StringA(newDstStr), srcCompareMethod, dstCompareMethod);
			if (err != AS_StorageError_None)
			{

				if (err == AS_StorageError_DataMiscompare)
					info.failed++;
				else
					info.failedtoProcess++;
			}
			else	
				info.passed++;

		}
		else
		{
			std::string newDstPath(reinterpret_cast<char*>(destPath.data));
			char newSrcStr[256];
			memcpy(newSrcStr, (char*)(itr->data), itr->size);
			newSrcStr[itr->size] = '\0';
			std::string newSrcString((char*)newSrcStr);

			size_t firstSlashPos = newSrcString.find_first_of((char*)("\\"));
			if (firstSlashPos == std::string::npos )
				firstSlashPos = newSrcString.find_first_of((char*)("/"));

			// support dest path as "/"
			if (newDstPath != "/")
				newDstPath += std::string((char*)("/"));

			newDstPath += std::string(newSrcString).substr(firstSlashPos+1);
			char newDstStr[256];
			memcpy(newDstStr, newDstPath.c_str(), newDstPath.length());
			newDstStr[newDstPath.size()] = '\0';

			std::cout << "Verifying srcfile: " << GetACPName(*itr) << " to dest: " << newDstStr << std::endl;

			err = VerifyFile(volume, *itr, AS_StringA(newDstStr), srcCompareMethod, dstCompareMethod, verifyContents);
			if (err != AS_StorageError_None)
			{

				if (err == AS_StorageError_DataMiscompare)
					info.failed++;
				else
					info.failedtoProcess++;
			}
			else	
				info.passed++;
		}
	}
    


	return AS_StorageError_None;

}

AS_StorageError WriteFolderRT(std::vector<AS_File::Path> & list, const AS_StorageDevice & device, const AS_Volume & volume, const AS_File::Path & dstFullPath, size_t* lastReadFile)
{
	AS_StorageError err = AS_StorageError_None;

	AS_File::Kind fileType = AS_File::File_IsDir;
	std::cout << "Current filelist size is: " << (UInt32)list.size() << std::endl;

	size_t index = 0;
	if (lastReadFile != 0)
		index = *lastReadFile;
	size_t vectorCtr = index;
	std::vector<AS_File::Path>::const_iterator itr = list.begin()+index;
	while (itr != list.end())
	{

		if (itr->size > 0)
		{
			bool isDir = IsDirectoryPath(*itr);

			if (isDir)
			{
					std::string newDstPath(reinterpret_cast<char*>(dstFullPath.data));
					char newSrcStr[256];
					memcpy(newSrcStr, (char*)(itr->data), itr->size);
					newSrcStr[itr->size] = '\0';

					std::string newSrcString((char*)newSrcStr);
					size_t firstSlashPos = newSrcString.find_first_of((char*)("\\"));
					if (firstSlashPos == std::string::npos )
						firstSlashPos = newSrcString.find_first_of((char*)"/");
					if (firstSlashPos != std::string::npos)
					{
						// support dest path as "/"
						if (newDstPath != "/")
							newDstPath += std::string((char*)("/"));

						newDstPath += (newSrcString.substr(firstSlashPos+1));
					}

					char newDstString[256];
					memcpy(newDstString, newDstPath.c_str(), newDstPath.size());
					newDstString[newDstPath.size()] = '\0';

					
					AS_StringA newDstStr((char*)(newDstPath.c_str()));


					UInt32 ret_size = 0;
					char mediaName[256];
					err = AS_Volume_GetEntryProperty(volume, newDstStr, AS_File::Prop_Name, sizeof(mediaName), &mediaName, &ret_size, 0, 0, 0, 0);
					if (err == AS_StorageError_FileNotFound)
					{

						std::cout << "Queueing folder: " << GetACPName(newDstStr) << std::endl;
						err = CreateEntry(volume, newDstStr, fileType);
						if (err != AS_StorageError_None)
						{
							if (err != AS_StorageError_FileExists)
								return err;
							else
							{
								std::cout << "Trying to add already existing directory: " << newDstPath << std::endl;
							}
						}
						else
							gNumFolders++;
					}
					else
						std::cout << "Trying to add already existing directory - we skipped trying to readd it: " << newDstPath << std::endl;

					std::cout << "Removing dir item " << (UInt32)vectorCtr << " from vector, current size of list is: " << (UInt32)list.size() << std::endl;
					list.erase(list.begin()+index);

			}
			else
			{
					std::string newDstPath(reinterpret_cast<char*>(dstFullPath.data));
					char newSrcStr[256];
					memcpy(newSrcStr, (char*)(itr->data), itr->size);
					newSrcStr[itr->size] = '\0';
					std::string newSrcString((char*)newSrcStr);

					size_t firstSlashPos = newSrcString.find_first_of((char*)("\\"));
					if (firstSlashPos == std::string::npos )
						firstSlashPos = newSrcString.find_first_of((char*)("/"));

					// support dest path as "/"
					if (newDstPath != "/")
						newDstPath += std::string((char*)("/"));

					newDstPath += std::string(newSrcString).substr(firstSlashPos+1);
					char newDstStr[256];
					memcpy(newDstStr, newDstPath.c_str(), newDstPath.length());
					newDstStr[newDstPath.size()] = '\0';

					std::cout << "Writing srcfile: " << GetACPName(*itr) << " to dest: " << newDstStr << std::endl;

					err = CopyHDFilesToMedia(device, volume, *itr, AS_StringA(newDstStr));
					if (err == AS_StorageError_NotEnoughStorage)
					{
						// send back our index in vector to restart next media or find alternative files
						*lastReadFile = vectorCtr;
						return err;
					}
					else if (err == AS_StorageError_None)
					{
						std::cout << "Removing file item  " << (UInt32)vectorCtr << " from vector, current size of list is: " << (UInt32)list.size() << std::endl;
						list.erase(list.begin()+index);
					}
					else
						return err;

					gNumFiles++;
			}
		}
		else
			std::cout << "Found corrupted item in list" << std::endl;

		itr = list.begin();
		vectorCtr++;
		std::cout << "Wrote: " << (UInt32)vectorCtr << " items, Remaining items: " << (UInt32)list.size() << std::endl;


	}

	*lastReadFile = vectorCtr;

	return AS_StorageError_None;



}


AS_StorageError CopyFolder(const AS_Volume & volume, const std::string & srcEntry, const std::string & destEntry)  // entry can be file or folder
{
	AS_StorageError err;
	
	// Copy Folder to Volume
	std::cout << "Add a folder to volume" << std::endl;

	std::string targetPathNoDrive = destEntry;

	// in case no target path supplied - make one from src path without drive letter
	if (targetPathNoDrive == "/" || targetPathNoDrive == "")
	{
		size_t position = srcEntry.find_first_of('\\');
		if (position == std::string::npos)
			targetPathNoDrive = srcEntry;
		else
			targetPathNoDrive = srcEntry.substr(position);
	}
	AS_File::CreateFlags flags = AS_File::Create_Parents | AS_File::Create_Overwrite;
	err = AS_Volume_CreateEntryFromHD(volume, AS_StringA(targetPathNoDrive), AS_StringA(srcEntry), flags);

	std::cout << "Done adding a folder to volume" << std::endl;

	if (err != AS_StorageError_None)
      return err;

	return AS_StorageError_None;

}


AS_StorageError CopyFileFromStream(const AS_Volume & volume, const std::string & srcPath, const std::string & destPath, UInt32 startLBA)
{
	static char gNameBuffer[1024]; // static so its still around during callback

	char destName[1024];
	   
	strcpy(destName, destPath.c_str());
	strcpy(gNameBuffer, srcPath.c_str()); // store source file path
	AS_StorageError err = AS_StorageError_None;

    // calculate stream size by seeking to end and asking stream its position
    std::ifstream* ifstrm = new std::ifstream;
	ifstrm->open(srcPath.c_str(),std::ios_base::binary);
	if (!ifstrm->is_open())
		return AS_StorageError_FileNotFound;

	ifstrm->seekg(0, std::ios_base::end);
	size_t streamsize = ifstrm->tellg();
    ifstrm->seekg(0, std::ios_base::beg);
    filestreams.push_back(ifstrm); // store stream until done writing

	if (startLBA != 0)
	{
		return AS_StorageError_NotImplemented;
	}
	else
	{
		AS_File::CreateFlags flags = AS_File::Create_Parents | AS_File::Create_Overwrite;
		err = AS_Volume_CreateFileFromStream(volume, AS_StringA(destPath), streamsize, StreamDataCallback, (void*)ifstrm, flags);
		if (err != AS_StorageError_None)
		{
			std::cout << "Failed to create file from stream" << std::endl;
			return err;
		}
		else
			std::cout << "Created file from stream " << std::endl;
	}

	return err;
}

//*************************************************************************************************//
// AS_File.h sample implementation functions
//*************************************************************************************************//
AS_StorageError ReadFileFromPos(const AS_Volume & volume, const AS_File::Path& filePath, AS_File::Size & startPos, AS_File::Size & endPos)
{

	AS_StorageError err;
	AS_File oneFileRead;

	AS_File::Size sizeOfFile;
	AS_File::Count numRead;

	UInt32 outDataSize=0;

	err = AS_Volume_OpenFile(volume, filePath, AS_File::Read, oneFileRead);
	if (err != AS_StorageError_None)
		return err;

	err = AS_File_GetProperty(oneFileRead, AS_File::Prop_Size, sizeof(sizeOfFile), &sizeOfFile, &outDataSize, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	if (endPos > sizeOfFile)
		std::cout << " Reading past End of file: size = " << sizeOfFile << ", endpos = " << endPos << std::endl;
	else if (endPos < sizeOfFile)
		std::cout << " Reading less than end of file: size = " << sizeOfFile << ", endpos = " << endPos << std::endl;
	
	AS_File::Size curPos = startPos;
	err = AS_File_Seek(oneFileRead, startPos);
	if (err != AS_StorageError_None)
		return err;
   while (curPos < endPos)
   {
      const UInt64 blockSize = 2048;
      char readBuffer[blockSize];
      UInt64 dataLeft = sizeOfFile - curPos;
      AS_File::Count numReq;

      if (blockSize < dataLeft)
         numReq =  static_cast<AS_File::Count>(blockSize);
      else
         numReq =  static_cast<AS_File::Count>(dataLeft);

      err = AS_File_Read(oneFileRead, numReq, readBuffer, &numRead);
		if (err != AS_StorageError_None)
		{
			AS_File_Close(oneFileRead);
	        return err;
      }
		std::cout << "Read " << numRead << " From: " << curPos << std::endl;

      curPos += numRead;
   }
	err = AS_File_Close(oneFileRead);


	return err;


}

AS_StorageError GetFileInfo(const AS_Volume & volume, const AS_File::Path & filePath, bool readFileContents)
{

	AS_File fileHdl;
	AS_Volume_OpenFile(volume,filePath, AS_File::Read, fileHdl);  

	AS_StorageError err;
	UInt32 propSize=0;

	char namebuf[1024];
	err = AS_File_GetProperty( fileHdl, AS_File::Prop_Name, sizeof(namebuf), namebuf, &propSize, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

   // convert to std::string for output
   std::string asciiName = GetStringFromUTF8(namebuf);
   std::cout << asciiName << "   ";

	AS_File::Size fileSize;
	err = AS_File_GetProperty( fileHdl, AS_File::Prop_Size, sizeof(fileSize),&fileSize,&propSize, 0, 0, 0, 0); 
	if (err != AS_StorageError_None)
		return err;

	std::cout << fileSize << "   ";

	AS_Volume::DateTime fileCreateDate;
	err = AS_File_GetProperty( fileHdl, AS_File::Prop_DateCreated, sizeof(fileCreateDate),&fileCreateDate,&propSize, 0, 0, 0, 0); 
	if (err != AS_StorageError_None)
		return err;
	OutputDate(fileCreateDate);

	AS_Volume::DateTime fileModDate;
	err = AS_File_GetProperty( fileHdl, AS_File::Prop_DateModified, sizeof(fileModDate),&fileModDate,&propSize, 0, 0, 0, 0); 
	if (err != AS_StorageError_None)
		return err;

	OutputDate(fileModDate);
	std::cout << std::endl << "--------------------------------------------------" << std::endl;

	err = AS_File_Close(fileHdl);
	if (err != AS_StorageError_None)
		return err;

	AS_File::Size startPos = 0;
	if (readFileContents)
	{
		err = ReadFileFromPos(volume, filePath, startPos, fileSize);
		if (err != AS_StorageError_None)
			return err;
	}
	
	return err;
}


AS_StorageError OpenOneFileAndRead(const AS_Volume & volume)
{
	AS_StorageError err;
	AS_StringA fpone = "/test/one.dat";
	AS_File adatFile;

	err = AS_Volume_OpenFile(volume, fpone, AS_File::Read, adatFile);
   
    if (err != AS_StorageError_None)
       return err;
     
	AS_File::Count numRead;
	char buffer[2048];
   
	for (int i = 0; i <20; i++)
	{
		err = AS_File_Read(adatFile, sizeof(buffer), buffer, &numRead);
		if (err != AS_StorageError_None)
		{
			AS_File_Close(adatFile);
			return err;
		}
	}
   
    err = AS_File_Close(adatFile);
	if (err != AS_StorageError_None)
       return err;
    
	return AS_StorageError_None;


}

AS_StorageError CopyHDFilesToMedia(const AS_Volume & vol, const AS_File::Path & srcpath, const AS_File::Path & dstpath)
{
	AS_StorageError err = AS_StorageError_None;

	AS_File dstFile;
	
//	AS_File::Size sizeOfFile;
	AS_File::Size curPos = 0;
	AS_File::Count numWritten;
    char readBuffer[2048];

	UInt32 outDataSize=0;
	size_t sizeOfFile = GetFileSize(srcpath);

	std::ifstream ifstrm;
	ifstrm.open(reinterpret_cast<char*>(srcpath.data),std::ios_base::binary);

	err = AS_Volume_OpenFile(vol, dstpath, AS_File::Append, dstFile);
	if (err != AS_StorageError_None)
		return err;

	while (curPos <= sizeOfFile)
	{
		ifstrm.read(readBuffer, sizeof(readBuffer));
		if (err != AS_StorageError_None)
			return err;

		err = AS_File_Write(dstFile, sizeof(readBuffer), readBuffer, &numWritten);
		if (err != AS_StorageError_None)
			return err;
		
		curPos += sizeof(readBuffer);
	}

	ifstrm.close();

	err = AS_File_Close(dstFile);
	if (err != AS_StorageError_None)
		return err;

	return err;
}

AS_StorageError OpenOneFileAndWrite(const AS_Volume & volume, const std::string & filepath, int writeNum=0)
{
   AS_StorageError err;
   AS_StringA fpone = filepath;

   AS_File oneFile;

   err = AS_Volume_OpenFile(volume, fpone, AS_File::Append, oneFile);

   if (err != AS_StorageError_None)
      return err;

   AS_File::Count numWritten;

   char buffer[2048]="Well, hello, Mrs. Chalmers::";
   char writeBuf[2048];

   char tmpNum[10];

   for (int i = 0; i <5; i++)
   {
	   strcpy(writeBuf, buffer);
	   strcat(writeBuf, "Update# ");
	   sprintf(tmpNum,"%d",i);
	   strcat(writeBuf, tmpNum);
	   strcat(writeBuf, "Rewrite# ");
	   sprintf(tmpNum,"%d",writeNum);
	   strcat(writeBuf, tmpNum);
      err = AS_File_Write(oneFile, sizeof(writeBuf), writeBuf, &numWritten);
      if (err != AS_StorageError_None)
         break;
   }

   err = AS_File_Close(oneFile);
   if (err != AS_StorageError_None)
		return err;

   return AS_StorageError_None;
}

static void GetFileName(UInt32 idx, std::string & filename, const std::string& folderPath, bool stripDriveName)
{
	char tmpNum[10];
	sprintf(tmpNum,"%d",idx);
	filename = folderPath;
	filename +=  "test";
	filename += tmpNum;
	filename += ".out";
	if (stripDriveName)
	{
		size_t position= filename.find_first_of("\\");
		filename=filename.substr(position);
	}	
}

AS_StorageError WriteUpdatedFiles(const AS_Volume & volume, UInt32 numIters, UInt32 numFiles, const std::string& folderPath)
{

	AS_StorageError err;

	for (UInt32 j=0; j<numIters; j++)
	{
		for (UInt32 i=0; i<numFiles; i++)
		{
			std::string filename;
			
			GetFileName(i, filename, folderPath, true);
			std::cout << "Writing file: " << i+1 << std::endl;
			//err = OneFileReadWrite(volume, filename);
			err = OpenOneFileAndWrite(volume, filename, i*j);
			if (err != AS_StorageError_None)
				return err;

			err = GetFileInfo(volume, AS_StringA(filename), false);
			if (err != AS_StorageError_None)
				return err;


		}
		std::cout << "Rewrote file: " << j+1 << " times " << std::endl;
	}
	return err;

}

AS_StorageError ReadFileContents(const AS_File& oneFileRead, AS_File::Size & curPos)
{

	AS_StorageError err;
	
	AS_File::Size sizeOfFile;
	AS_File::Count numRead;
    char readBuffer[2048];

	UInt32 outDataSize=0;

	err = AS_File_GetProperty(oneFileRead, AS_File::Prop_Size, sizeof(sizeOfFile), &sizeOfFile, &outDataSize, 0, 0, 0, 0);
	if (err != AS_StorageError_None)
		return err;

	// Uncomment to not read back entire file - just the block that was just written
//		if (sizeOfFile > 0)
//			curPos = sizeOfFile - sizeof(readBuffer);

	
	err = AS_File_Seek(oneFileRead, curPos);
	if (err != AS_StorageError_None)
		return err;
	while (curPos <= sizeOfFile)
	{
		err = AS_File_Read(oneFileRead, sizeof(readBuffer), readBuffer, &numRead);
		if (err != AS_StorageError_None)
			return err;

		if (err == AS_StorageError_None)
		{
			std::cout << "Read " << numRead << " From: " << curPos << std::endl;
		}
		curPos += sizeof(readBuffer);
	}
	return err;

}


AS_StorageError OpenTwoFilesAndWrite(const AS_Volume & volume)
{
   AS_StorageError err;
   AS_StringA fpone = "/test/one.dat";
   AS_StringA fptwo = "/test/two.dat";

   AS_File twoFile;
   AS_File oneFile;

   err = AS_Volume_OpenFile(volume, fpone, AS_File::Append, oneFile);

   if (err != AS_StorageError_None)
      return err;

   err = AS_Volume_OpenFile(volume, fptwo, AS_File::Append, twoFile);

   if (err != AS_StorageError_None)
   {
      AS_File_Close(oneFile);
      return err;
   }

   AS_File::Count numWritten;
   char buffer[2048]="Well, hello, Mrs. Chalmers";

   for (int i = 0; i <20; i++)
   {
      err = AS_File_Write(oneFile, sizeof(buffer), buffer, &numWritten);
      if (err != AS_StorageError_None)
         break;
      err = AS_File_Write(twoFile, sizeof(buffer), buffer,  &numWritten);
      if (err != AS_StorageError_None)
         break;
   }

   AS_File::Kind fileType = AS_File::File_IsFile;
   AS_File::Size VolumeFileSize;
   AS_StringA outFullPath = "/test/one.dat";
   AS_StringA inDirfullPath = "/";

   SInt32 pathLen;
   char namebuf[1024];
	AS_StringA outerFullPath = namebuf;

   // try to getEntry on a file that exists on the volume
   err = AS_Volume_GetEntry(volume,inDirfullPath,0,&pathLen,outerFullPath);   
   if (err != AS_StorageError_None)
   {
	   std::cout << "Failed to get entry with ASVolume_GetEntryInfo for file that should exist" << std::endl;
	   return err;
   }
   // try to getInfo on a file that doesn't exist on the media
   AS_StringA wrongFullPath = "wrongpath";
   err = AS_Volume_GetEntryInfo(volume, wrongFullPath, fileType, VolumeFileSize);
   if (err != AS_StorageError_None)
	   std::cout << "Failed to get file size with ASVolume_GetEntryInfo for file that doesn't exist" << std::endl;
   else
   {
	   std::cout << "ASVolume_GetEntryInfo File size of file one is " << VolumeFileSize << std::endl;
	   return err;
   }

   err = AS_Volume_GetEntryInfo(volume, outFullPath, fileType, VolumeFileSize);
   if (err != AS_StorageError_None)
	   std::cout << "Failed to get file size with ASVolume_GetEntryInfo " << std::endl;
   else
   {
	   std::cout << "ASVolume_GetEntryInfo File size of file one is " << VolumeFileSize << std::endl;
	   return err;
   }

   AS_File::Size filesize;
   UInt32 propsize=0;
   err = AS_File_GetProperty(oneFile,AS_File::Prop_Size,sizeof(filesize),&filesize,&propsize, 0, 0, 0, 0);           // OUT: size of data returned in propData

   if (err != AS_StorageError_None)
	   std::cout << "Failed to get file size" << std::endl;
   else
   {
	   std::cout << "File size of file one is " << filesize << std::endl;
	   return err;
   }

   err = AS_File_Close(oneFile);
   if (err == AS_StorageError_None)
	   std::cout << "File size of file one is " << filesize << std::endl;
   else
   {
	   std::cout << "Failed to close file " << std::endl;
	   return err;
   }
   err = AS_File_Close(twoFile);
   if (err == AS_StorageError_None)
	   std::cout << "File size of file one is " << filesize << std::endl;
   else
   {
	   std::cout << "Failed to get file " << twoFile.myHandle << std::endl;
	   return err;
   }


   return err;
}

AS_StorageError OpenOneFileWriteAndRead(const AS_Volume & volume, const AS_File::Count bytesToWrite)
{
   AS_StorageError err;
   AS_StringA fpone = "/test1.dat";

   AS_File oneFile;

   err = AS_Volume_OpenFile(volume, fpone, AS_File::Append, oneFile);
   if (err != AS_StorageError_None)
      return err;

   AS_File::Count numWritten;

   AS_File::Count numBytes = 0;

   while (numBytes <= bytesToWrite)
   {
		char * buffer = new char[2048];
		memset(buffer,0x41,bytesToWrite);
		 
		err = AS_File_Write(oneFile, bytesToWrite, buffer, &numWritten);
		delete [] buffer;
		if (err != AS_StorageError_None)
				return err;
//		if (numWritten != 2048)
		printf("Wrote: %ld bytes so far\n",numBytes);

		numBytes += 2048;
   }

   err = AS_File_Close(oneFile);
   if (err != AS_StorageError_None)
		return err;


   AS_File oneFileRead;
   err = AS_Volume_OpenFile(volume, fpone, AS_File::Read, oneFileRead);
   if (err != AS_StorageError_None)
      return err;

   UInt32 bytesToRead = bytesToWrite;
   AS_File::Count numRead = 0;
   char * readBuffer = new char[bytesToWrite];
   memset(readBuffer, 0, sizeof(readBuffer));

   err = AS_File_Read(oneFileRead, bytesToRead, readBuffer, &numRead);
   if (err != AS_StorageError_None)
   {
      delete [] readBuffer;
      return err;
   }

   std::cout << "Bytes in file: " << readBuffer << std::endl;
   delete [] readBuffer;

   err = AS_File_Close(oneFileRead);
   if (err != AS_StorageError_None)
		return err;

   return AS_StorageError_None;

}

AS_StorageError GetNewDate(AS_Volume::DateTime & ourDate)
{

	AS_Volume::DateTime newDate;
	AS_StorageError err = AS_StorageError_None;
	
	std::string DateString, timeString;
	std::string Month, Day, Year, Hour, Min, Sec;

	std::cout << "Enter mm/dd/yy" << std::endl;
    std::cin >> DateString;

	Day = Year = Month = DateString;

	std::string tmpStr;

	tmpStr = Month;
	size_t position=tmpStr.find_first_of("/");

	Month = tmpStr.erase(position);
	newDate.month = (UInt8)(atol(Month.c_str()));

	tmpStr = Day;
	position=Day.find_first_of("/",0);
	tmpStr = Day.substr(position+1);
	position = tmpStr.find_first_of("/");
	Day = tmpStr.erase(position);
	newDate.day = (UInt8)(atol(Day.c_str()));
	
	tmpStr = Year;
	position=tmpStr.find_last_of("/");
	Year = tmpStr.substr(position+1);
	newDate.year = (UInt16)(atol(Year.c_str()));
	
	std::cout << "Enter hh:mm:ss " << std::endl;
	std::cin >> timeString;

	Hour = Min = Sec = timeString;

	tmpStr = Hour;
	position=tmpStr.find_first_of(":");
	Hour = tmpStr.erase(position);
	newDate.hour = (UInt8)(atol(Hour.c_str()));

	tmpStr = Min;
	position=Min.find_first_of(":",0);
	tmpStr = Min.substr(position+1);
	position = tmpStr.find_first_of(":");
	Min = tmpStr.erase(position);
	newDate.minute = (UInt8)(atol(Min.c_str()));
	
	tmpStr = Sec;
	position=tmpStr.find_last_of(":");
	Sec = tmpStr.substr(position+1);
	newDate.second = (UInt8)(atol(Sec.c_str()));

	ourDate = newDate;
	return err;
}

AS_StorageError SetFileInfo(const AS_Volume & volume, const AS_File::Path & filePath, UInt32 PropertyType, const std::string & newFileName, const UInt64 newFileSize, const AS_Volume::DateTime & date)
{
	AS_File fileHdl;
	AS_Volume_OpenFile(volume,filePath, AS_File::Read, fileHdl);  

	AS_StorageError err = AS_StorageError_None;
	UInt32 propSize=0;
	switch (PropertyType)
	{
		case AS_File::Prop_Name:
		{
			char fileName[1024];
			err = AS_File_GetProperty( fileHdl, AS_File::Prop_Name, sizeof(fileName), fileName, &propSize, 0, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			std::cout << " Filename was: " << GetStringFromUTF8(fileName) << "     ";

			AS_StringA newFName = newFileName;
			err = AS_File_SetProperty( fileHdl, AS_File::Prop_Name, sizeof(AS_File::Path), &newFName, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			err = AS_File_GetProperty( fileHdl, AS_File::Prop_Name, sizeof(fileName), fileName, &propSize, 0, 0, 0, 0);
			if (err != AS_StorageError_None)
				return err;

			std::cout << " Filename is now: " << GetStringFromUTF8(fileName) << "     ";
		}
		break;
		case AS_File::Prop_Size:
		{
			UInt64 fileSize;
			err = AS_File_GetProperty( fileHdl, AS_File::Prop_Size, sizeof(fileSize),&fileSize,&propSize, 0, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			std::cout << "File size was: " << fileSize << "     ";;

			fileSize = newFileSize;

			err = AS_File_SetProperty( fileHdl, AS_File::Prop_Size, sizeof(fileSize),&fileSize, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			err = AS_File_GetProperty( fileHdl, AS_File::Prop_Size, sizeof(fileSize),&fileSize,&propSize, 0, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			std::cout << "File size is now: " << fileSize << "     ";;
		}
		break;
		case AS_File::Prop_DateCreated:
		{	
			AS_Volume::DateTime fileCreateDate;
			err = AS_File_GetProperty( fileHdl, AS_File::Prop_DateCreated, sizeof(fileCreateDate),&fileCreateDate,&propSize, 0, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			std::cout << "Date was: " << std::endl;

			OutputDate(fileCreateDate);

			fileCreateDate = date;

			err = AS_File_SetProperty( fileHdl, AS_File::Prop_DateCreated, sizeof(fileCreateDate), &fileCreateDate, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			err = AS_File_GetProperty( fileHdl, AS_File::Prop_DateCreated, sizeof(fileCreateDate),&fileCreateDate,&propSize, 0, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;
			std::cout << "Date is now: " << std::endl;
			OutputDate(fileCreateDate);

		}
		break;
		case AS_File::Prop_DateModified:
		{
			AS_Volume::DateTime fileModDate;
			err = AS_File_GetProperty( fileHdl, AS_File::Prop_DateModified, sizeof(fileModDate),&fileModDate,&propSize, 0, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			std::cout << "Date was: " << std::endl;
			OutputDate(fileModDate);
			std::cout << std::endl << std::endl;

			fileModDate = date;

			err = AS_File_SetProperty( fileHdl, AS_File::Prop_DateModified, sizeof(fileModDate),&fileModDate, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			err = AS_File_GetProperty( fileHdl, AS_File::Prop_DateModified, sizeof(fileModDate),&fileModDate,&propSize, 0, 0, 0, 0); 
			if (err != AS_StorageError_None)
				return err;

			std::cout << "Date is: " << std::endl;
			OutputDate(fileModDate);
			std::cout << std::endl << std::endl;
			
		}
	};

	err = AS_File_Close(fileHdl);
	if (err != AS_StorageError_None)
		return err;

	return err;
}


int main(int argc, char * argv[])
{
	bool UsingCmdLine=false;
	std::string srcRoot, targetRoot;
	// read command args or print syntax
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << "FOLDERTOCOPY <FOLDERDESTINATION> Will copy folder to media in first optical drive" << std::endl;
		UsingCmdLine=false;
	}
	else
	{
		if (argc < 3)
		{
			srcRoot = argv[1];
			targetRoot = "/BACKUP";
		}
		else
		{
			srcRoot = argv[1];
			targetRoot = argv[2];
		}
		UsingCmdLine=true;

		std::cout << "Copying " << argv[1] << " to first optical drive into folder" << targetRoot << std::endl;
	}

	AS_StorageError retVal = AS_StorageError_None;


	AS_StorageDevice device;
	AS_Volume volume;
	
//	UInt32 NumDevices = CycleDrives();
	// open 1st optical device
	UInt32 OpticalCount = 0;
	retVal = AS_GetStorageDeviceCount(AS_StorageDevice::Scan_Optical | AS_StorageDevice::Scan_Tape, OpticalCount);
	std::cout << "We found " << OpticalCount << " optical and/or tape drive(s)\n" << std::endl;
	retVal = AS_OpenStorageDevice(1, device);
   	if (retVal != AS_StorageError_None)
	{
		OutputError(retVal);
		return retVal;
	}

	char AppName[80];
	AS_StringA currentUsedApp(AppName);
	retVal = AS_StorageDevice_ExclusiveAccess(device, AS_StorageDevice::ExclusiveAccess_Obtain, AS_StringA("AS_StorageTest"), currentUsedApp);
	if (retVal != AS_StorageError_None) 
	{
		OutputError(retVal);
		return retVal;
	}
	
	printf("Done setting exclusive access\n");

	bool isEraseable=false;
	
	// Check to make sure media is ready and find out if Media is eraseable
	retVal = GetDeviceMediaState(device, isEraseable);
	if (retVal != AS_StorageError_None)
	{
		OutputError(retVal);
		return retVal;
	}

	retVal = GetDevicePropertyInfo(device);
	if (retVal != AS_StorageError_None)
	{
		OutputError(retVal);
		return retVal;
	}

	if (UsingCmdLine)
	{

		std::cout << "We got user input for folder to burn" << std::endl;

		AS_Volume::FS_Type formatType;
		// open vol
		retVal = OpenVolume(device, volume);
		if (retVal != AS_StorageError_None)
			return retVal;
		UInt32 ret_data_size=0;
		AS_StorageDevice::MediaKind MediaTypeUsed;
		retVal = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Kind,sizeof(MediaTypeUsed),
					&MediaTypeUsed, &ret_data_size, 0, 0, 0, 0);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		switch (MediaTypeUsed)
		{
			case AS_StorageDevice::MediaIsCdRom:
			case AS_StorageDevice::MediaIsCdr:
			case AS_StorageDevice::MediaIsCdrw:
				formatType = AS_Volume::FS_ISO_Level_2_Long;
				break;
			case AS_StorageDevice::MediaIsDvdr:
			case AS_StorageDevice::MediaIsDvdpr:
			case AS_StorageDevice::MediaIsDvdrw:
			case AS_StorageDevice::MediaIsDvdprw:
			case AS_StorageDevice::MediaIsDvdRam:
				formatType = AS_Volume::FS_UDF15;
				break;
			default:
				formatType = AS_Volume::FS_UDF15;
				break;
		};

		// create blank FS
		retVal = FormatVolume(volume,formatType);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		// add folders
		retVal = CopyFolder(volume, srcRoot, targetRoot);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		// Commit changes
		retVal = FlushVolume(volume);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		// close volume
		retVal = CloseVolume(volume);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		// open vol
		retVal = OpenVolume(device, volume);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		retVal = MountExistingDisc(volume);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		retVal = GetDirectoryInfo(volume, "/");
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		// close volume
		retVal = CloseVolume(volume);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
			return retVal;
		}

		char OtherAppName[80];
		AS_StringA currentUsedApp(OtherAppName);

		retVal = AS_StorageDevice_ExclusiveAccess(device, AS_StorageDevice::ExclusiveAccess_Release, AS_StringA("AS_StorageTest"), currentUsedApp);
		if (retVal != AS_StorageError_None) 
		{
			OutputError(retVal);
			return retVal;
		}
		printf("Done releasing exclusive access\n");

		retVal = AS_StorageDevice_Close(device);
		if (retVal != AS_StorageError_None)
			return retVal;

		return 0;


	}

	std::string menuinput="";
	while (menuinput != "99")
	{

		std::cout << "1 - Select Format Type for FileSystem" << std::endl;;
		std::cout << "2 - Write two files simultaneously to disc" << std::endl;
		std::cout << "3 - Copy Folder to Media" << std::endl;
		std::cout << "4 - Get Media directory listing" << std::endl;
		std::cout << "5 - Erase disc" << std::endl;
		std::cout << "6 - Write and append to file(s)" << std::endl;
		std::cout << "7 - CreateFile from stream" << std::endl;
		std::cout << "8 - Read all file contents from media" << std::endl;
		std::cout << "9 - Write n bytes and read them back before flush" << std::endl;
		std::cout << "10 - Get Volume Properties" << std::endl;
		std::cout << "11 - Get Media properties" << std::endl;
		std::cout << "12 - Get Volume Entry Properties" << std::endl;
		std::cout << "13 - Set Volume Entry Properties" << std::endl;
		std::cout << "14 - Set Volume Properties" << std::endl;
		std::cout << "15 - Set Device Properties" << std::endl;
		std::cout << "16 - Set File Properties" << std::endl;
		std::cout << "17 - Get File Properties" << std::endl;
		std::cout << "18 - Write Entries from sourcein RTTD mode" << std::endl;
		std::cout << "19 - Read a file" << std::endl;
		std::cout << "20 - Delete file from Media" << std::endl;
		std::cout << "21 - Copy" << std::endl;
		std::cout << "23 - Get Track Properties" << std::endl;
		std::cout << "25 - Commit changes" << std::endl;
		std::cout << "26 - Open Volume from media" << std::endl;
		std::cout << "27 - Close Volume" << std::endl;
		std::cout << "28 - Mount Volume" << std::endl;
		std::cout << "29 - Commit changes - close session" << std::endl;
		std::cout << "30 - Commit changes - finalize write-once discs" << std::endl;
		std::cout << "33 - Verify a volume's contents" << std::endl;
		std::cout << "40 - Change to another drive index" << std::endl;
		std::cout << "66 - Create entry on volume" << std::endl;
		std::cout << "67 - Make volume bootable" << std::endl;
		std::cout << "99 - Quit" << std::endl;

		std::cin >> menuinput;

		
		retVal = DoMenuOperation(device, volume, menuinput);
		if (retVal != AS_StorageError_None)
		{
			OutputError(retVal);
		}

	}

	char OtherAppName[80];
	AS_StringA currUsedApp(OtherAppName);

	retVal = AS_StorageDevice_ExclusiveAccess(device, AS_StorageDevice::ExclusiveAccess_Release, AS_StringA("AS_StorageTest"), currUsedApp);
	if (retVal != AS_StorageError_None) 
	{
		OutputError(retVal);
		return retVal;
	}

	printf("Done releasing exclusive access\n");

	retVal = AS_StorageDevice_Close(device);
	if (retVal != AS_StorageError_None)
	{
		OutputError(retVal);
		return retVal;
	}

	return 0;
}

AS_Volume::FS_Type GetFormatTypeFromSelection(UInt32 formatSelection)
{

	switch (formatSelection)
	{
	case 1:
		return AS_Volume::FS_ISO_Level_1;
		break;
	case 2:
		return AS_Volume::FS_ISO_Level_2;
		break;
	case 3:
		return AS_Volume::FS_ISO_Level_2_Long;
		break;
	case 4:
		return AS_Volume::FS_ISO_Versionless;
		break;
	case 5:
		return AS_Volume::FS_Joliet;
		break;
	case 6:
		return AS_Volume::FS_UDF102;
		break;
	case 7:
		return AS_Volume::FS_UDF15;
		break;
	case 8:
		return AS_Volume::FS_UDF2;
		break;
	case 9:
		return AS_Volume::FS_Audio;
		break;
	case 10:
		return AS_Volume::FS_UDF25;
		break;
	case 11:
		return AS_Volume::FS_Bridge;
		break;
	case 12:
		return AS_Volume::FS_UDF26;
		break;
	default:
		return AS_Volume::FS_UDF15;
		break;

	};

}

AS_StorageError DoMenuOperation(AS_StorageDevice & device, AS_Volume & volume, std::string & menuInput)
{

//	AS_Volume volume;
	AS_Volume::FS_Type formatType = AS_Volume::FS_None;
	AS_StorageError retVal = AS_StorageError_None;

	char* pEnd;
	UInt32 menuOption = strtol(menuInput.c_str(), &pEnd, 0);
	switch (menuOption)
	{
	case 1:		// set filesystem format type (call after erase for new disc)
		{

			UInt32 formatSelection;
			UInt32 numFSs=1;
			std::cout << "Enter # of filesystems: " << std::endl;
			std::cin >> numFSs;

			for (UInt32 ctr=0; ctr<numFSs; ctr++)
			{
				std::cout << "Select format type (default is UDF 1.50)" << std::endl;
				std::cout << "1-ISO 9660 Level 1" << std::endl;
				std::cout << "2-Iso 9660 Level2" << std::endl;
				std::cout << "3-Iso 9660 Level2 Long" << std::endl;
				std::cout << "4-Iso Versonless" << std::endl;
				std::cout << "5-Joliet" << std::endl;
				std::cout << "6-UDF 1.02" << std::endl;
				std::cout << "7-UDF 1.50" << std::endl;
				std::cout << "8-UDF 2.00" << std::endl;
				std::cout << "9-Audio" << std::endl;
				std::cout << "10-UDF 2.5" << std::endl;
				std::cout << "11 - DVD-V (Iso 9660 with UDF.102)" << std::endl;
				std::cout << "12-UDF 2.6" << std::endl;
				std::cout << "13-VCD" << std::endl;
				std::cout << "99-Return to main menu" << std::endl;

				std::cin >> formatSelection;

				if (formatSelection == 99)
					return AS_StorageError_None;

				if (formatType == AS_Volume::FS_None)
					formatType = GetFormatTypeFromSelection(formatSelection);
				else
					formatType |= GetFormatTypeFromSelection(formatSelection);
			}

			UInt32 volumeAccessSelection;

			std::cout << "Select volume access type (default is Mastering)" << std::endl;
			std::cout << "1-Mastering" << std::endl;
			std::cout << "2-RTTD" << std::endl;

			std::cin >> volumeAccessSelection;

			AS_Volume::AccessFlag volumeAccessType = AS_Volume::Mastering;

			switch (volumeAccessSelection)
			{
			case 1:
				volumeAccessType = AS_Volume::Mastering;
				break;

			case 2:
				volumeAccessType = AS_Volume::RTTD;
				break;

			};

			// create blank FS (joliet)
			if (formatType == AS_Volume::FS_None)
				formatType = AS_Volume::FS_UDF15;

			retVal = FormatVolume(volume, formatType, volumeAccessType);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;

	case 2:		// write two 40k files at a time using AS_File_Write
		{
			std::string PathToBurn;

			std::cout << "Please enter fullpath of file to archive on media:" << std::endl;

			std::cin >> PathToBurn;

			//OpenOneFileAndWrite(volume, PathToBurn);
			retVal = OpenTwoFilesAndWrite(volume);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;

	case 3:		// copy folder using AS_Volume_CreateFileFromHD with a foldername
		{
			std::string PathToBurn;
			std::string PathOnMedia;

			std::cout << "Please enter fullpath of source directory to archive on media:" << std::endl;
			
			// override to handle spaces since cin won't do it by default
			std::cin.ignore();
			getline(std::cin, PathToBurn);

			std::cout << "Please enter fullpath of destination directory to archive on media:" << std::endl;
			
			getline(std::cin, PathOnMedia);

			// add folders
			retVal = CopyFolder(volume, PathToBurn, PathOnMedia);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;

	case 4:		// Get Media directory listing
			retVal = GetDirectoryInfo(volume, "/");
			if (retVal != AS_StorageError_None)
				return retVal;
		break;

	case 5:		// erase disc
		{
			bool isEraseable=false;
			// Check to make sure media is ready and find out if Media is eraseable
			retVal = GetDeviceMediaState(device, isEraseable);
			if (retVal != AS_StorageError_None)
				return retVal;

			if (isEraseable)
			{
				std::cout << "Start erasing disc" << std::endl;

				retVal = AS_StorageDevice_EraseMedia(device,AS_StorageDevice::Quick_Erase, DeviceInfo_Callback, NULL);
				std::cout << "Done erasing disc" << std::endl;
			}

		}	
		break;

	case 6:		// write #n files and update those files #n times
	{			// each write is a 40k update of 20 AS_File_Write calls of 2048 bytes
		UInt32 numFiles=1;
		UInt32 numIters=1;
		std::cout << "Please enter # of files:" << std::endl;
		std::cin >> numFiles;
		std::cout << "Please enter # of rewrites" << std::endl;
		std::cin >> numIters;

		std::string tempFolderPath;
		std::cout << "Enter Folder to use to create temp files" << std::endl;
		std::cin >> tempFolderPath;
		
		retVal = WriteUpdatedFiles(volume, numIters, numFiles, tempFolderPath);
		if (retVal != AS_StorageError_None)
			return retVal;


	}
    break;

	case 7:
		{		// write file to media using AS_Volume_CreateFileFromStream
			std::string PathToBurn, DestPath;
			std::cout << "Please enter fullpath of file to archive on media:" << std::endl;
			std::cin >> PathToBurn;
			std::cout << "Please enter destination fullpath of file to archive on media:" << std::endl;
			std::cin >> DestPath;

			UInt32 startLBA = 0;
			std::cout << "Please enter location to write to (0 to write last loc):" << std::endl;
			std::cin >> startLBA;
			
			//OpenOneFileAndWrite(volume, PathToBurn);
			retVal = CopyFileFromStream(volume, PathToBurn, DestPath, startLBA);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;
	
	case 8:
		{		// Read all files and all file contents on media
			retVal = GetDirInfo(volume, AS_StringA("/"), true);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;
	case 9:
		{		// Write a file of #n bytes using AS_File_Write and read back those written bytes prior to flush

			AS_File::Count numBytes=10;
			std::cout << "Please enter # of bytes to write:" << std::endl;
			std::cin >> numBytes;

			retVal = OpenOneFileWriteAndRead(volume, numBytes);
			if (retVal != AS_StorageError_None)
				return retVal;
			
		}
		break;
	case 10:
		{		// Get Volume info with AS_Volume_GetProperty
			UInt32 infoOption=0;
			UInt64 FreeBytes=0;
			std::cout << "0 - Get Volume info" << std::endl;
			std::cout << "1 - Get Media freespace" << std::endl;
			std::cout << "2 - Get Volume freespace" << std::endl;
			std::cin >> infoOption;

			switch (infoOption)
			{
			case 0:
				retVal = GetVolumeInfo(volume);
				break;
			break;
			case 1: 
				GetMediaSize(device, FreeBytes);
				break;
			case 2:
				retVal = GetVolumeSize(volume, FreeBytes);
				if (retVal != AS_StorageError_None)
					return retVal;
				break;
			default:
				break;
			}
		}
		break;

	case 11:
		{	
			// Get media info with AS_StorageDevice_GetMediaProperty
			retVal = GetDeviceMediaInfo(device);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;

	case 12:
		{		// Get file info with AS_Volume_GetEntryProperty for an entry
			std::string fullPath;

			std::cout << "Please enter fullpath of file on media to get properties of:" << std::endl;

			std::cin >> fullPath;
			retVal = GetEntryInfo(volume, fullPath);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;
	case 13:
		{	// Set file info with AS_Volume_SetEntry for a file
			std::string fullPath;
			UInt32 infoOption=0;
			std::cout << "Please enter fullpath of file on media to get properties of:" << std::endl;

			std::cin >> fullPath;

			std::cout << "0 - Set File name" << std::endl;
			std::cout << "1 - Set File size" << std::endl;
			std::cout << "2 - Set File Creation Date" << std::endl;
			std::cout << "3 - Set File Modification Date" << std::endl;

			std::cin >> infoOption;

			switch (infoOption)
			{
			case 0:
				{
					std::string newfullPath;

					std::cout << "Please enter new fullpath for file on media:" << std::endl;

					std::cin >> newfullPath;
					retVal = SetEntryInfo(volume, fullPath, AS_File::Prop_Name, newfullPath, 0, AS_Volume::DateTime());
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 1:
				{
					UInt64 newfileSize;
					std::cout << "Please enter new filesize for file on media:" << std::endl;

					std::cin >> newfileSize;
					retVal = SetEntryInfo(volume, fullPath, AS_File::Prop_Size, "", newfileSize, AS_Volume::DateTime());
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 2:
				{
					AS_Volume::DateTime newDate;
					GetNewDate(newDate);
					retVal = SetEntryInfo(volume, fullPath, AS_File::Prop_DateCreated , "", 0, newDate);
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 3:
				{
					AS_Volume::DateTime newDate;
					GetNewDate(newDate);
					retVal = SetEntryInfo(volume, fullPath, AS_File::Prop_DateModified, "", 0, newDate);
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			};

		}
		break;
	case 14:
		{

			UInt32 infoOption=0;
			std::cout << "0 - Set Volume name" << std::endl;
			std::cout << "1 - Set Volume delimiter" << std::endl;
			std::cout << "2 - Set Volume Creation Date" << std::endl;
			std::cout << "3 - Set Volume Modification Date" << std::endl;

			std::cin >> infoOption;

			switch (infoOption)
			{
			case 0:
				{

					std::string newVolumeName;

					std::cout << "Please enter new volume name:" << std::endl;

					std::cin >> newVolumeName;
					retVal = SetVolumeInfo(volume, AS_Volume::Prop_Name, newVolumeName, 0, AS_Volume::DateTime(), 0);
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 1:
				{
					SInt8 newDelimiter;
					std::cout << "Please enter new delimiter for volume:" << std::endl;

					std::cin >> newDelimiter;
					retVal = SetVolumeInfo(volume, AS_Volume::Prop_Delimiter, "",0,AS_Volume::DateTime(), newDelimiter);
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 2:
				{
					AS_Volume::DateTime newDate;
					GetNewDate(newDate);
					retVal = SetVolumeInfo(volume, AS_Volume::Prop_DateCreated, "", 0, newDate, 0);
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 3:
				{
					AS_Volume::DateTime newDate;
					GetNewDate(newDate);
					retVal = SetVolumeInfo(volume, AS_Volume::Prop_DateModified, "", 0, newDate, 0);
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			};

		}
		break;

	case 15:
		{

			UInt32 PropertySelection=0;
			std::cout << "1 - Current write speed" << std::endl;;
			std::cout << "2 - Current write speed (in 100th's X)" << std::endl;
			std::cout << "3 - Current read speed " << std::endl;
			std::cout << "4 - Current read speed (in 100th's X)" << std::endl;
			std::cout << "Please enter property to set: " << std::endl;
			std::cin >> PropertySelection;

			AS_StorageDevice::MediaProp_Type Property;
			UInt32 PropertyValue;
			switch (PropertySelection)
			{
			case 1:
				Property = AS_StorageDevice::MedProp_CurWrite_DataRates;
				break;
			case 2:
				Property = AS_StorageDevice::MedProp_CurWrite_Hundredth_X;
				break;
			case 3:
				Property = AS_StorageDevice::MedProp_CurRead_DataRates;
				break;
			case 4:
				Property = AS_StorageDevice::MedProp_CurRead_Hundredth_X;
				break;
			default:
				break;
			};

			std::cout << "Enter value to set: " << std::endl;
			std::cin >> PropertyValue;

				if (PropertyValue == 0)
					PropertyValue = AS_StorageDevice::Speed_Min;
				else if (PropertyValue == 0xffffffff)
					PropertyValue = AS_StorageDevice::Speed_Max;

			retVal = SetDevicePropertyInfo(device, Property, PropertyValue);
			if (retVal != AS_StorageError_None)
				return retVal;

		}
		break;
	case 16:
		{
			// Set file info with AS_File_SetProperty for a file
			std::string fullPath;
			UInt32 infoOption=0;
			std::cout << "Please enter fullpath of file on media to get properties of:" << std::endl;

			std::cin >> fullPath;

			std::cout << "0 - Set File name" << std::endl;
			std::cout << "1 - Set File size" << std::endl;
			std::cout << "2 - Set File Creation Date" << std::endl;
			std::cout << "3 - Set File Modification Date" << std::endl;

			std::cin >> infoOption;

			switch (infoOption)
			{
			case 0:
				{
					std::string newfullPath;

					std::cout << "Please enter new fullpath for file on media:" << std::endl;

					std::cin >> newfullPath;

					AS_StringA asFullPath = fullPath;
					retVal = SetFileInfo(volume, asFullPath, AS_File::Prop_Name, newfullPath, 0, AS_Volume::DateTime());
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 1:
				{
					UInt64 newfileSize;
					std::cout << "Please enter new filesize for file on media:" << std::endl;

					std::cin >> newfileSize;

					AS_StringA asFullPath = fullPath;
					retVal = SetFileInfo(volume, asFullPath, AS_File::Prop_Size, "", newfileSize, AS_Volume::DateTime());
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 2:
				{
					AS_Volume::DateTime newDate;
					GetNewDate(newDate);

					AS_StringA asFullPath = fullPath;
					retVal = SetFileInfo(volume, asFullPath, AS_File::Prop_DateCreated , "", 0, newDate);
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			case 3:
				{
					AS_Volume::DateTime newDate;
					GetNewDate(newDate);

					AS_StringA asFullPath = fullPath;
					retVal = SetFileInfo(volume, asFullPath, AS_File::Prop_DateModified, "", 0, newDate);
					if (retVal != AS_StorageError_None)
						return retVal;
				}
				break;
			};

		}
		break;
		case 17:
		{		// Get file info with AS_File_GetProperty for an path
			std::string fullPath;

			std::cout << "Please enter fullpath of file on media to get properties of:" << std::endl;

			std::cin >> fullPath;

			AS_StringA asciiFullpath = fullPath;
			retVal = GetFileInfo(volume, asciiFullpath, false);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;
	case 18:
		{
			// set global for capacity (use free+used for writeonce media)
			UInt32 ret_data_size=0;
			UInt64 bytesFree = 0;
			UInt64 bytesUsed = 0;
   			retVal = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Bytes_Free,sizeof(bytesFree),
							&bytesFree, &ret_data_size);
			if (retVal != AS_StorageError_None)
	 			return retVal;
   			
			retVal = AS_StorageDevice_GetMediaProperty(device,AS_StorageDevice::MedProp_Bytes_Used,sizeof(bytesUsed),
							&bytesUsed, &ret_data_size);
			if (retVal != AS_StorageError_None)
	 			return retVal;

			gCapacity = bytesFree + bytesUsed;

			char srcfullPath[256];
			std::cout << "Please enter fullpath of source folder to write to media in real-time:" << std::endl;
			std::cin >> srcfullPath;

			char dstfullPath[256];
			std::cout << "Please enter fullpath of destination to write to media:" << std::endl;
			std::cin >> dstfullPath;

			
			std::vector<AS_File::Path> filesList;
			retVal = FillFilesListing(filesList, AS_StringA(srcfullPath));

			UInt32 writeThisIndexNext = 0;
			retVal = WriteFolderRT(filesList, device, volume, AS_StringA(dstfullPath), &writeThisIndexNext);
			if (retVal != AS_StorageError_None)
				return retVal;
			
			if (writeThisIndexNext != filesList.size())
				std::cout << "We wrote " << writeThisIndexNext << " files, expected to write: " << (UInt32)filesList.size() << std::endl;

			filesList.clear();

		}
		break;
	case 19:
		{
			std::string filePathName;
			std::cout << "Please enter file to read:" << std::endl;
			std::cin >> filePathName;

			AS_File::Size startpos, endpos = 0;
			std::cout << "Please enter start position:" << std::endl;
			std::cin >> startpos;

			std::cout << "Please enter end position:" << std::endl;
			std::cin >> endpos;

			retVal = ReadFileFromPos(volume, AS_StringA(filePathName), startpos, endpos);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;
	case 20:
		{

			std::string deletePathName;

			std::cout << "Please enter file / directory name to delete:" << std::endl;

			std::cin >> deletePathName;

			retVal = DeleteItem(volume, deletePathName);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;
 case 21:
      {

         char OtherAppName[80];
         AS_StringA currUsedApp(OtherAppName);
		 // close any existing devices and allow reopening from this function
         if (device.deviceType != AS_StorageDevice::Type_File)
         {

            retVal = AS_StorageDevice_ExclusiveAccess(device, AS_StorageDevice::ExclusiveAccess_Release, AS_StringA("AS_StorageTest"), currUsedApp);
            if (retVal != AS_StorageError_None) 
            {
               OutputError(retVal);
               return retVal;
            }

            printf("Done releasing exclusive access\n");
         }
         retVal = AS_StorageDevice_Close(device);
         if (retVal != AS_StorageError_None)
         {
            OutputError(retVal);
            return retVal;
         }

         UInt32 defaultIndex = 1; // reset to 1st destination after copy is over

         std::string srcPathName;
         std::vector<AS_StorageDevice> DriveDstList;
         std::vector<UInt32>    DeviceDriveIndex;
         std::vector<std::string> DevicePathList;
         UInt32 numDests = 1;
         
         std::cout << "Please enter file / directory name of source (# for drive index):" << std::endl;
         std::cin >> srcPathName;

         std::cout << "Please enter # of destinations:" << std::endl;
         std::cin >> numDests;

         for (UInt32 i=0; i<numDests;i++)
         {
            UInt32 dstDriveIndex=0;
            std::string dstPathName;
            std::cout << "Please enter destination drive index (-1 for file):" << std::endl;
            std::cin >> dstDriveIndex;

            if (dstDriveIndex == 0xffffffff)
            {
               std::cout << "Please enter fullpath of destination file:" << std::endl;
               std::cin >> dstPathName;
               DevicePathList.push_back(dstPathName);
            }
            else
            {
               // save device index for 1st destination so it is opened when complete
               if (i == 0)
                  defaultIndex = dstDriveIndex;

               DeviceDriveIndex.push_back(dstDriveIndex);
            }
         }

         AS_StorageDevice::CopyFlags copyFlag = AS_StorageDevice::CopyFlags_Write;

         UInt32 opType = 0;
         std::cout << "Enter advanced operation type:" << std::endl;
         std::cout << "0 - Write src to dst(s)" << std::endl;
         std::cout << "1 - Compare src to dst(s)" << std::endl;
         std::cout << "2 - Compare src to dst(s) - Data only" << std::endl;
         std::cout << "3 - TestCopy src to dst(s)" << std::endl;
         std::cin >> opType;

         if (opType < 1 || opType > 3)
            opType = 0;

         switch (opType)
         {
            case 1:
                  copyFlag = AS_StorageDevice::CopyFlags_Compare;
               break;
            case 2:
                  copyFlag = AS_StorageDevice::CopyFlags_CompareData;
               break;
            case 3:
                  copyFlag = AS_StorageDevice::CopyFlags_TestMode;
               break;
            default:
               copyFlag = AS_StorageDevice::CopyFlags_Write;
               break;

         };

         AS_StorageDevice srcDevice;
         srcDevice.myHandle = 0xffffffff;
         char * devPath = new char[256];
         memset(devPath, 0, sizeof(devPath));
         UInt32 srcDriveIndex=0;          
         AS_StringA srcFullPath = srcPathName;
         if (isdigit(srcPathName[0]))
         {
            srcDriveIndex = atoi(srcPathName.c_str());

            retVal = AS_OpenStorageDevice(srcDriveIndex, srcDevice);
            if (retVal != AS_StorageError_None)
               return retVal;

            if (srcDevice.myHandle == 0xffffffff)
            {
               OutputError(AS_StorageError_InvalidHandle);
               return AS_StorageError_InvalidHandle;
            }

            retVal = AS_StorageDevice_ExclusiveAccess(srcDevice, AS_StorageDevice::ExclusiveAccess_Obtain, AS_StringA("AS_StorageTest"), currUsedApp);
            if (retVal != AS_StorageError_None) 
            {
               OutputError(retVal);
               return retVal;
            }
            
            printf("Done setting exclusive access\n");

            bool eraseable = false;
            retVal = GetDeviceMediaState(srcDevice, eraseable);
            if (retVal != AS_StorageError_None)
               return retVal;
         }
         else
         {
            retVal = AS_OpenStorageDevice(srcDriveIndex, srcDevice, srcFullPath);
            if (retVal != AS_StorageError_None)
               return retVal;

            if (srcDevice.myHandle == 0xffffffff)
            {
               OutputError(AS_StorageError_InvalidHandle);
               return AS_StorageError_InvalidHandle;
            }

            // update source flag type
            UInt32 sourceFileType = 0;
            std::cout << "Source file types:" << std::endl;
            std::cout << "0 - ISO Image file" << std::endl;
            std::cout << "1 - GI Image file" << std::endl;
            std::cout << "Enter type of source file: " << std::endl;
            std::cin >> sourceFileType;
            switch (sourceFileType)
            {  
               case 0:
                  formatType = AS_StorageDevice::FormatType_OpticalDetect;
                  break;
               case 1:
                  formatType = AS_StorageDevice::FormatType_GI;
                  break;
               default:
                  formatType = AS_StorageDevice::FormatType_OpticalDetect;
                  break;
            };

         }

         AS_StorageDevice * driveDests = new AS_StorageDevice[numDests];
         UInt32 destCtr = 0;

         for (std::vector<UInt32>::const_iterator itr = DeviceDriveIndex.begin();
            itr != DeviceDriveIndex.end();++itr)
         {
            AS_StorageDevice newdevice;
            newdevice.myHandle = 0xffffffff;
            retVal = AS_OpenStorageDevice(*itr, newdevice);
            if (retVal != AS_StorageError_None)
               return retVal;

            if (newdevice.myHandle == 0xffffffff)
            {
               OutputError(AS_StorageError_InvalidHandle);
               return AS_StorageError_InvalidHandle;
            }
            
            retVal = AS_StorageDevice_ExclusiveAccess(newdevice, AS_StorageDevice::ExclusiveAccess_Obtain, AS_StringA("AS_StorageTest"), currUsedApp);
            if (retVal != AS_StorageError_None) 
            {
               OutputError(retVal);
               return retVal;
            }
            
            printf("Done setting exclusive access\n");

            bool eraseable = false;
            retVal = GetDeviceMediaState(newdevice, eraseable);
            if (retVal != AS_StorageError_None)
               return retVal;

            driveDests[destCtr] = newdevice;
            destCtr++;

            // only set write speed for write operations - not compare
            if (copyFlag  == AS_StorageDevice::CopyFlags_Write || 
               copyFlag == AS_StorageDevice::CopyFlags_TestMode)
            {
               // get dest device(s) speeds list and output to console
               retVal = GetDeviceMediaInfo(newdevice);
               if (retVal != AS_StorageError_None)
                  OutputError(retVal);

               UInt32 PropertySelection=0;
               std::cout << "1 - Current write speed " << std::endl;
               std::cout << "2 - Current write speed (in 100th's X)" << std::endl;

               std::cout << "Please enter property to set: " << std::endl;
               std::cin >> PropertySelection;

               AS_StorageDevice::MediaProp_Type Property;
               UInt32 PropertyValue=0xffffffff;
               switch (PropertySelection)
               {
               case 1:
                  Property = AS_StorageDevice::MedProp_CurWrite_DataRates;
                  break;
               case 2:
                  Property = AS_StorageDevice::MedProp_CurWrite_Hundredth_X;
                  break;
               default:
                  break;
               };

               // set speed for dest device(s)
               std::cout << "Enter writing speed: " << std::endl;
               std::cin >> PropertyValue;
               if (PropertyValue == 0)
                  PropertyValue = AS_StorageDevice::Speed_Min;
               else if (PropertyValue == 0xffffffff)
                  PropertyValue = AS_StorageDevice::Speed_Max;

               retVal = SetDevicePropertyInfo(newdevice, Property, PropertyValue);
               if (retVal != AS_StorageError_None)
                  return retVal;
            }
         }

         if (DevicePathList.size() > 0)
         {
            // update source flag type
            UInt32 destFileType = 0;

            std::cout << "Destination file types:" << std::endl;
            std::cout << "0 - ISO Image file" << std::endl;
            std::cout << "1 - GI Image file" << std::endl;
            std::cout << "2 - MD Image file" << std::endl;
            std::cout << "Enter type of source file: " << std::endl;
            std::cin >> destFileType;
            switch (destFileType)
            {  
               case 0:
                  formatType = AS_StorageDevice::FormatType_OpticalDetect;
                  break;
               case 1:
                  formatType = AS_StorageDevice::FormatType_GI;
                  break;

            };

         }
         AS_String destPathStr;
         for (std::vector<std::string>::const_iterator itr = DevicePathList.begin();
            itr != DevicePathList.end();++itr)
         {
            AS_StorageDevice newdevice;
            newdevice.myHandle = 0xffffffff;
            destPathStr.data = (void*)(itr->c_str());
            destPathStr.type = AS_String::Type_ASCII;
            destPathStr.size = itr->size();
            retVal = AS_OpenStorageDevice(0, newdevice, destPathStr);
            if (retVal != AS_StorageError_None)
               return retVal;

            if (newdevice.myHandle == 0xffffffff)
            {
               AS_StorageDevice_Close(newdevice);
            }
            else
            {
               driveDests[destCtr] = newdevice;
               destCtr++;
            }
         }

         UInt32 trackNum, srcTrackNum = 0;
         std::cout << "Enter track to copy from source to dst(s) - 0 for all tracks" << std::endl;
         std::cin >> trackNum;


         std::cout << "Starting copy operation" << std::endl;
         retVal = AS_StorageDevice_Copy(srcDevice, formatType, copyFlag, trackNum, numDests, driveDests, DeviceInfo_Callback, driveDests);
         if (retVal != AS_StorageError_None)
         {
            OutputError(retVal);
            std::cout << "Failed to copy source -> dest(s)" << std::endl;
         }
         else
         {
            std::cout << "Copy succeeded, starting verify operation for all destinations" << std::endl;
            //compare
            copyFlag = AS_StorageDevice::CopyFlags_Compare;
			retVal = AS_StorageDevice_Copy(srcDevice, formatType, copyFlag, trackNum, numDests, driveDests, DeviceInfo_Callback, driveDests);
            if (retVal != AS_StorageError_None)
                std::cout << "Failed to verify dest(s) -> source " << std::endl;

         }

         if (srcDevice.deviceType != AS_StorageDevice::Type_File)
         {

            retVal = AS_StorageDevice_ExclusiveAccess(srcDevice, AS_StorageDevice::ExclusiveAccess_Release, AS_StringA("AS_StorageTest"), currUsedApp);
            if (retVal != AS_StorageError_None) 
            {
               OutputError(retVal);
            }

            printf("Done releasing exclusive access\n");
         }

         retVal = AS_StorageDevice_Close(srcDevice);
         if (retVal != AS_StorageError_None)
         {
            OutputError(retVal);
         }

         for (UInt32 i=0; i<destCtr; i++)
         {

            if (driveDests[i].deviceType != AS_StorageDevice::Type_File)
            {

               retVal = AS_StorageDevice_ExclusiveAccess(driveDests[i], AS_StorageDevice::ExclusiveAccess_Release, AS_StringA("AS_StorageTest"), currUsedApp);
               if (retVal != AS_StorageError_None) 
               {
                  OutputError(retVal);
               }

               printf("Done releasing exclusive access\n");
            }

            retVal = AS_StorageDevice_Close(driveDests[i]);
            if (retVal != AS_StorageError_None)
            {
               OutputError(retVal);
            }
         }


         if (driveDests != 0)
            delete[] driveDests;

         if (devPath != 0)
            delete devPath;

         // finish copy operation by opening device #1
         retVal = AS_OpenStorageDevice(defaultIndex, device);
         if (retVal != AS_StorageError_None)
            return retVal;

         retVal = AS_StorageDevice_ExclusiveAccess(device, AS_StorageDevice::ExclusiveAccess_Obtain, AS_StringA("AS_StorageTest"), currUsedApp);
         if (retVal != AS_StorageError_None) 
         {
            OutputError(retVal);
            return retVal;
         }
         
         printf("Done setting exclusive access\n");

      }
      break;
	case 23:
		{	
			// Get track info 
			retVal = GetMediaTrackInfo(device);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;
	case 25:
		retVal = FlushVolume(volume);
		if (retVal != AS_StorageError_None)
			return retVal;
		break;
	case 26:
		retVal = OpenVolume(device, volume);
		if (retVal != AS_StorageError_None)
			return retVal;
		break;
	case 27:
		retVal = CloseVolume(volume);
		if (retVal != AS_StorageError_None)
			return retVal;
		break;
	case 28:
		{

			UInt32 volumeAccessSelection;

			std::cout << "Select volume access type (default is Mastering)" << std::endl;
			std::cout << "1-Mastering" << std::endl;
			std::cout << "2-RTTD" << std::endl;
			std::cout << "3-Read only" << std::endl;

			std::cin >> volumeAccessSelection;

			AS_Volume::AccessFlag volumeAccessType = AS_Volume::Mastering;

			switch (volumeAccessSelection)
			{
			case 1:
				volumeAccessType = AS_Volume::Mastering;
				break;
			case 2:
				volumeAccessType = AS_Volume::RTTD;
				break;
			case 3:
				volumeAccessType = AS_Volume::ReadOnly;
				break;

			};

			retVal = MountExistingDisc(volume, volumeAccessType);
			if (retVal != AS_StorageError_None)
				return retVal;
		}
		break;
	case 29:
		retVal = FlushVolume(volume, AS_Volume::Border_Close);
		if (retVal != AS_StorageError_None)
			return retVal;
		break;
	case 30:
		retVal = FlushVolume(volume, AS_Volume::Border_Final);
		if (retVal != AS_StorageError_None)
			return retVal;
		break;
	case 33:
		{

			char destPath[256];
			char srcPath[256];

			std::cout << "Please enter src folder path to compare" << std::endl;
			std::cin >> srcPath;

			std::cout << "Please enter dest media path to verify" << std::endl;

			std::cin >> destPath;

			UInt32 srcCompareMethod, dstCompareMethod = 0;// default to using our engine to read media files

			std::cout << "Please enter method to use to read source" << std::endl;
			std::cout << "0 - AS_Storage to read (media only)" << std::endl;
			std::cout << "1 - Native OS to read (std c++)" << std::endl;
			std::cout << "2 - Native OS to read (win32 / C api)" << std::endl;
			std::cout << "3 - Use external app to compare" << std::endl;
			std::cin >> srcCompareMethod;

			UInt32 externalAppUsed = 0;  
			if ( srcCompareMethod == 3)
			{

				std::cout << "Please select external app to use to compare" << std::endl;
				std::cout << "0 - PbComp" << std::endl;
				std::cout << "1 - xcomp32" << std::endl;

				std::cin >> externalAppUsed;

			}
			std::cout << "Please enter method to use to read destination" << std::endl;
			std::cout << "0 - AS_Storage to read (media only)" << std::endl;
			std::cout << "1 - Native OS to read (std c++)" << std::endl;
			std::cout << "2 - Native OS to read (win32 / C api)" << std::endl;
			std::cout << "3 - Use external app to compare" << std::endl;
			std::cin >> dstCompareMethod;
			UInt32 dstexternalAppUsed = 0;  
			if ( srcCompareMethod == 3)
			{

				std::cout << "Please select external app to use to compare" << std::endl;
				std::cout << "1 - PbComp" << std::endl;
				std::cout << "2 - xcomp32" << std::endl;

				std::cin >> dstexternalAppUsed;

			}

			UInt32 readContents = 1;
			
			std::cout << "Please enter '0' to only read file entries (skip reading contents)" << std::endl;
			std::cin >> readContents;

			if (readContents > 1)
				readContents = 1;
			UInt32 readPermissions = 0;
			std::cout << "Please enter '0' to skip matching permissions" << std::endl;
			std::cin >> readPermissions;

			if (readPermissions > 1)
				readPermissions = 1;


			std::vector<AS_File::Path> filesList;
			retVal = FillFilesListing(filesList, AS_StringA(srcPath));
			if (retVal != AS_StorageError_None)
			{
				OutputError(retVal);
				return retVal;
			}

			// 
			UInt32 failedCases = 0;
			UInt32 totalSize = (UInt32)filesList.size();

			SummaryInfo summaryinfo;
			summaryinfo.failed = 0;
			summaryinfo.failedtoProcess = 0;
			summaryinfo.passed = 0;
			summaryinfo.totalNum = totalSize;

			retVal = VerifyVolume(volume, filesList, AS_StringA(destPath), srcCompareMethod, dstCompareMethod, externalAppUsed, dstexternalAppUsed, readContents, readPermissions, summaryinfo);
			if (retVal != AS_StorageError_None)
			{
				OutputError(retVal);
			}

			// some stats
			std::cout << "Total Number of entries compared: " << summaryinfo.totalNum << std::endl;
			std::cout << "Number of entries that matched: " << summaryinfo.passed << std::endl;
			std::cout << "Number of entries that failed: " << summaryinfo.failed << std::endl;
			std::cout << "Number of entries that failed to process: " << summaryinfo.failedtoProcess << std::endl;
			if (failedCases > 0)
				return AS_StorageError_DataMiscompare;

			if (retVal != AS_StorageError_None)
				return retVal;

		}
		break;
	case 40:
		{
			UInt32 driveIndex=0;

			std::cout << "Please enter drive index to use:" << std::endl;

			std::cin >> driveIndex;
	
			char OtherAppName[80];
			AS_StringA currUsedApp(OtherAppName);

			retVal = AS_StorageDevice_ExclusiveAccess(device, AS_StorageDevice::ExclusiveAccess_Release, AS_StringA("AS_StorageTest"), currUsedApp);
			if (retVal != AS_StorageError_None) 
			{
				OutputError(retVal);
				return retVal;
			}

			printf("Done releasing exclusive access\n");
			// Close existing device handle
			retVal = AS_StorageDevice_Close(device);
			if (retVal != AS_StorageError_None)
				return retVal;


			// open another optical device
			retVal = AS_OpenStorageDevice(driveIndex, device);
   			if (retVal != AS_StorageError_None)
				return retVal;
				char AppName[80];

			AS_StringA currentUsedApp(AppName);
			retVal = AS_StorageDevice_ExclusiveAccess(device, AS_StorageDevice::ExclusiveAccess_Obtain, AS_StringA("AS_StorageTest"), currentUsedApp);
			if (retVal != AS_StorageError_None) 
			{
				OutputError(retVal);
				return retVal;
			}
			
			printf("Done setting exclusive access\n");


			bool isEraseable=false;
			// Check to make sure media is ready and find out if Media is eraseable
			retVal = GetDeviceMediaState(device, isEraseable);
			if (retVal != AS_StorageError_None)
				return retVal;
			GetDevicePropertyInfo(device);

		}
		break;
	case 66:
		{
			std::string entryPath;
			std::cout << "Please enter path of entry" << std::endl;

			std::cin >> entryPath;
			
			AS_File::Kind fileType = AS_File::File_IsDir;
			retVal = CreateEntry(volume, entryPath, fileType);
			if (retVal != AS_StorageError_None)
 				return retVal;
		}
		break;

	case 67: //Make bootable
		{
			//std::string bootStr;
			char bootStr[200] ;
			std::cout << "Please enter path of boot file on hard disc" << std::endl;
			std::cin >> bootStr;


    		AS_File::Path bootPath;

			bootPath.type = AS_String::Type_ASCII;
			bootPath.size = strlen(bootStr);
			bootPath.data = bootStr;

			std::cout << "Please enter type of boot image:" << std::endl;
			std::cout << "0 - Boot 1.2 floppy images" << std::endl;
			std::cout << "1 - Boot 1.44 floppy images" << std::endl;
			std::cout << "2 - Boot 2.88 floppy images" << std::endl;
			std::cout << "3 - Boot Hard Drive images" << std::endl;
			std::cout << "4 - Boot a no emulation boot image" << std::endl;
			std::cout << "5 - Boot a no emulation boot image; also writes Boot Info table " << std::endl;

			UInt32 bootType;
			AS_Volume::BootType bootImageType;
			std::cin >> bootType;
			switch (bootType)
			{
			case 0: 
				bootImageType = AS_Volume::Boot_Floppy12;
			break;
			case 1: 
				bootImageType = AS_Volume::Boot_Floppy144;
			break;
			case 2: 
				bootImageType = AS_Volume::Boot_Floppy288;
			break;
			case 3: 
				bootImageType = AS_Volume::Boot_HD;
			break;
			case 4: 
				bootImageType = AS_Volume::Boot_NoEmulation;
			break;
			case 5: 
				bootImageType = AS_Volume::Boot_NoEmulationWithBootInfoTable;
			break;
			default: 
				bootImageType = AS_Volume::Boot_None;
			break;
			};

			retVal = AS_Volume_SetBootable(volume,bootPath, bootImageType, 992);
			
			if (retVal != AS_StorageError_None)
 				return retVal;
		}
		break;

	case 99:
		break;

	};

	return retVal;

}

//////   Enum to string conversion functions for console output

std::string GetVolumeFormatStr(UInt32 PropertyVal)
{
	std::string retString;

	if (PropertyVal == AS_Volume::FS_None)
			retString = "None";

   if (PropertyVal&AS_Volume::FS_ISO_Level_1)
			retString += " ISO Level 1";

   if (PropertyVal&AS_Volume::FS_ISO_Level_2)
			retString += " ISO Level 2";

   if (PropertyVal&AS_Volume::FS_ISO_Level_2_Long)
			retString += " ISO Level 2 Long";

   if (PropertyVal&AS_Volume::FS_ISO_Versionless)
			retString += " ISO Versionless";

   if (PropertyVal&AS_Volume::FS_Joliet)
			retString += " Joliet";

   if (PropertyVal&AS_Volume::FS_UDF102)
			retString += " UDF 1.02";

   if (PropertyVal&AS_Volume::FS_UDF15)
			retString += " UDF 1.5";

   if (PropertyVal&AS_Volume::FS_UDF2)
			retString += " UDF 2.0";

   if (PropertyVal&AS_Volume::FS_UDF25)
			retString += " UDF 2.5";

   if (PropertyVal&AS_Volume::FS_UDF26)
			retString += " UDF 2.6";

   if (PropertyVal&AS_Volume::FS_Audio)
			retString += " Audio";

   if (PropertyVal&AS_Volume::FS_TDF)
			retString += " TDF";

   if (PropertyVal&AS_Volume::FS_VRT)
			retString += " VRT";

	return retString;
}
std::string GetVolumeStateStr(UInt32 PropertyVal)
{
	std::string retString;
	if (PropertyVal&AS_Volume::File_IsMounted)
      retString += "Mounted ";
   if (PropertyVal&AS_Volume::File_IsDirty)
      retString += "Dirty ";
   if (PropertyVal&AS_Volume::File_IsAppendable)
      retString += "Appendable ";

	return retString;
}

std::string GetPropertyKindString(UInt32 PropertyVal)
{

	std::string retString;

	switch (PropertyVal)
	{
		case AS_StorageDevice::MediaIsCdRom:
			retString = "CD-Rom";
			break;
		case AS_StorageDevice::MediaIsCdr:
			retString = "CD-R";
			break;
		case AS_StorageDevice::MediaIsCdrw:
			retString = "CD-RW";
			break;
		case AS_StorageDevice::MediaIsDdCdrom:
			retString = "DD-CDRom";
			break;
		case AS_StorageDevice::MediaIsDdCdr:
			retString = "DD-CDR";
			break;
		case AS_StorageDevice::MediaIsDdCdrw:
			retString = "DDCD-RW";
			break;
		case AS_StorageDevice::MediaIsDvdRom:
			retString = "DVD-Rom";
			break;
		case AS_StorageDevice::MediaIsDvdr:
			retString = "DVD-R";
			break;
		case AS_StorageDevice::MediaIsDvdrw:
			retString = "DVD-RW";
			break;
		case AS_StorageDevice::MediaIsDvdpr:
			retString = "DVD+R";
			break;
		case AS_StorageDevice::MediaIsDvdprw:
			retString = "DVD+RW";
			break;
		case AS_StorageDevice::MediaIsDvdRam:
			retString = "DVD-RAM";
			break;
		case AS_StorageDevice::MediaIsDvdpr9:
			retString = "DVD +R Dual layer";
			break;
		case AS_StorageDevice::MediaIsDvdr9:
			retString = "DVD-R Dual Layer";
			break;
		case AS_StorageDevice::MediaIsBdr:
			retString = "BD-R";
			break;
		case AS_StorageDevice::MediaIsBdre:
			retString = "BD-RE";
			break;
		case AS_StorageDevice::MediaIsHdDvdRom:
			retString = "HD-DVDRom";
			break;
		case AS_StorageDevice::MediaIsHdDvdr:
			retString = "HD-DVDR";
			break;
		case AS_StorageDevice::MediaIsHdDvdrw:
			retString = "HD-DVDRW";
			break;
		case AS_StorageDevice::MediaIsBdrom:		
			retString = "BD-Rom";
			break;
		case AS_StorageDevice::MediaIsSeqTape:
			retString = "SeqTape";
			break;

		default:
			retString = "Unknown media type";
	};
	return retString;
}

std::string GetMediaTypeCapableStr(UInt32 PropertyVal)
{
	std::string retString;
	if (PropertyVal & AS_StorageDevice::MediaIsCdRom)
		retString += "CD-Rom ";
	if (PropertyVal & AS_StorageDevice::MediaIsCdr)
		retString += "CD-R ";
	if (PropertyVal & AS_StorageDevice::MediaIsCdrw)
		retString += "CD-RW ";
	if (PropertyVal & AS_StorageDevice::MediaIsDdCdrom)
		retString += "DD-CDRom ";
	if (PropertyVal & AS_StorageDevice::MediaIsDdCdr)
		retString += "DD-CDR ";
	if (PropertyVal & AS_StorageDevice::MediaIsDdCdrw)
		retString += "DDCD-RW ";
	if (PropertyVal & AS_StorageDevice::MediaIsDvdRom)
		retString += "DVD-Rom ";
	if (PropertyVal & AS_StorageDevice::MediaIsDvdr)
		retString += "DVD-R ";
	if (PropertyVal & AS_StorageDevice::MediaIsDvdrw)
		retString += "DVD-RW ";
	if (PropertyVal & AS_StorageDevice::MediaIsDvdpr)
		retString += "DVD+R ";
	if (PropertyVal & AS_StorageDevice::MediaIsDvdprw)
		retString += "DVD+RW ";
	if (PropertyVal & AS_StorageDevice::MediaIsDvdRam)
		retString += "DVD-RAM ";
	if (PropertyVal & AS_StorageDevice::MediaIsDvdpr9)
		retString += "DVD +R Dual layer ";
	if (PropertyVal & AS_StorageDevice::MediaIsDvdr9)
		retString += "DVD-R Dual Layer ";
	if (PropertyVal & AS_StorageDevice::MediaIsBdr)
		retString += "BD-R";
	if (PropertyVal & AS_StorageDevice::MediaIsBdre)
		retString += "BD-RE";
	if (PropertyVal & AS_StorageDevice::MediaIsSeqTape)
		retString += "Sequential Tape";
	return retString;

}

std::string GetDevicePropTypeStr(UInt32 PropertyVal)
{
	std::string retString;

	if (PropertyVal == AS_StorageDevice::IsOptical)
		retString = "Optical";
	if (PropertyVal == AS_StorageDevice::IsTape)
		retString = "Tape";


	return retString;

}

std::string GetDeviceTrayStr(UInt32 PropertyVal)
{
	std::string retString;

	if (PropertyVal == AS_StorageDevice::TrayStatus_Open)
		retString += "TrayOpened";
	else if (PropertyVal == AS_StorageDevice::TrayStatus_Closed)
		retString += "TrayClosed";
	else if (PropertyVal == AS_StorageDevice::TrayStatus_Unknown)
		retString += "TrayStatus_Unknown";


	return retString;

}

std::string GetDeviceStateStr(UInt32 PropertyVal)
{
	std::string retString;

	if (PropertyVal == AS_StorageDevice::State_Not_Ready)
		retString = "Not Ready";
	else if (PropertyVal == AS_StorageDevice::State_Ready)
		retString = "Ready";
	else if (PropertyVal == AS_StorageDevice::State_Becoming_Ready)
		retString = "Becoming Ready";
	return retString;

}

std::string GetDevicePropStr(UInt32 Property, UInt32 PropVal)
{

	std::string retString;
	switch(Property)
	{
	case AS_StorageDevice::DevProp_MedTypesCapable:
		retString = GetMediaTypeCapableStr(PropVal);
		break;
	case AS_StorageDevice::DevProp_DevType:
		retString = GetDevicePropTypeStr(PropVal);
		break;
	case AS_StorageDevice::DevProp_TrayStatus:
		retString = GetDeviceTrayStr(PropVal);
		break;
	default:
		retString = "Property not found";
	};

	return retString;
}

std::string GetTrackPropertyStateString(UInt32 PropertyVal)
{
	std::string retString;

	if (PropertyVal & AS_StorageDevice::Track_IOState_Writable)
		retString += "Writable ";
	if (PropertyVal & AS_StorageDevice::Track_IOState_Appendable)
		retString += "Appendable ";
	if (PropertyVal & AS_StorageDevice::Track_IOState_Rewritable)
		retString += "Rewrite ";
	if (PropertyVal & AS_StorageDevice::Track_IOState_Erasable)
		retString += "Erasable ";			
	if (PropertyVal & AS_StorageDevice::Track_IOState_Blank)
		retString += "Blank ";
	if (PropertyVal & AS_StorageDevice::Track_IOState_Invisible)
		retString += "Invisible ";
	if (PropertyVal & AS_StorageDevice::Track_IOState_Open)
		retString += "Open ";
	if (PropertyVal & AS_StorageDevice::Track_IOState_AutoJump)
		retString += "AutoJump ";
	if (PropertyVal & AS_StorageDevice::Track_IOState_Formattable)
		retString += "Formattable ";
	if (PropertyVal & AS_StorageDevice::Track_IOState_Expandable)
		retString += "Expandable ";

	return retString;

}

std::string GetPropertyStateString(UInt32 PropertyVal)
{
	std::string retString;

	if (PropertyVal & AS_StorageDevice::MediaIsReadable)
		retString += "Readable ";
	if (PropertyVal & AS_StorageDevice::MediaIsWritable)
		retString += "Writable ";
	if (PropertyVal & AS_StorageDevice::MediaIsAppendable)
		retString += "Appendable ";
	if (PropertyVal & AS_StorageDevice::MediaIsRewritable)
		retString += "Rewrite ";
	if (PropertyVal & AS_StorageDevice::MediaIsErasable)
		retString += "Erasable ";			
	if (PropertyVal & AS_StorageDevice::MediaIsBlank)
		retString += "Blank ";
	return retString;
}

std::string GetDeviceAuthTypesStr(AS_StorageDevice::Authentication_Type & authInfo)
{
	std::string retString;

	if (authInfo & AS_StorageDevice::Authentication_None)
		retString += "None / ";
	if (authInfo & AS_StorageDevice::Authentication_CPPM)
		retString += "CPPM / ";
	if (authInfo & AS_StorageDevice::Authentication_CPRM)
		retString += "CPRM / ";
	if (authInfo & AS_StorageDevice::Authentication_AACS)
		retString += "AACS / ";

	return retString;

}
std::string GetDeviceBusStr(const AS_StorageDevice::Bus_Info & busInfo)
{
	std::string retString;

	// get bus kind
	if (busInfo.bus_kind == AS_StorageDevice::Bus_ATAPI)
		retString += "ATAPI";
	if (busInfo.bus_kind == AS_StorageDevice::Bus_SCSI)
		retString += "SCSI";
	if (busInfo.bus_kind == AS_StorageDevice::Bus_USB)
		retString += "USB";
	if (busInfo.bus_kind == AS_StorageDevice::Bus_USB2)
		retString += "USB2";
	if (busInfo.bus_kind == AS_StorageDevice::Bus_1394)
		retString += "1394";
	if (busInfo.bus_kind == AS_StorageDevice::Bus_Unknown)
		retString += "Unknown";


	// get bus ID
	if (busInfo.addr_valid)
	{
		char buf[10];
		retString += " - Adapter/Target/Lun:";
		sprintf(buf,"%d",busInfo.addr_adapter_id);
		retString += std::string(buf);
		retString += "/";
		sprintf(buf,"%d",busInfo.addr_target_id);
		retString += std::string(buf);
		retString += "/";
		sprintf(buf,"%d",busInfo.addr_lun_id);
		retString += std::string(buf);
	}


	return retString;
}
std::string GetMediaPropString(UInt32 property, UInt32 PropVal)
{
	std::string retString;
	switch(property)
	{
	case AS_StorageDevice::MedProp_Kind:
		retString = GetPropertyKindString(PropVal);
		break;
	case AS_StorageDevice::MedProp_State:
		retString = GetPropertyStateString(PropVal);
		break;

	default:
		retString = "Invalid property type";
	};

	return retString;
}




std::string GetStringFromUTF8(char* utf8String)
{
#ifdef _WIN32

   size_t size = (size_t) MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) utf8String, -1, 0, 0);
   wchar_t * buf = new wchar_t[size];
   MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) utf8String, -1, (LPWSTR) buf, static_cast<int>(size));
   size = (size_t) WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) buf, -1, 0, 0, 0, 0);
   char * cBuf = new char[size];
   WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) buf, -1, cBuf, static_cast<int>(size), 0, 0);
   return std::string(cBuf);

#else
   return std::string(utf8String);
#endif
}
#if !defined (__bcm__)
std::wstring GetWideStringFromUTF8(UInt8* utf8String)
{
#ifdef _WIN32

   size_t size = (size_t) MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) utf8String, -1, 0, 0);
   wchar_t * buf = new wchar_t[size];
   MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) utf8String, -1, (LPWSTR) buf, static_cast<int>(size));
   return std::wstring(buf);
#else

   // not implemented
   assert(0);
   return std::wstring();
#endif
}
std::wstring GetWideName(const AS_File::Path & filenamebuf)
{
#ifdef _WIN32

   size_t size = 0;
   switch (filenamebuf.type)
   {
   case AS_String::Type_UTF8:
   {
      return GetWideStringFromUTF8(static_cast<UInt8*>(filenamebuf.data));
   }
   break;
   case AS_String::Type_Wide:
   {
      return (std::wstring(reinterpret_cast<wchar_t*>(filenamebuf.data)));

   }
   break;
   case AS_String::Type_ASCII:
      {
         size_t size = (size_t) MultiByteToWideChar(CP_ACP, 0, (LPCSTR) filenamebuf.data, -1, 0, 0);
         wchar_t * buf = new wchar_t[size];
         MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) filenamebuf.data, -1, (LPWSTR) buf, static_cast<int>(size));
         return (std::wstring(reinterpret_cast<wchar_t*>(buf)));

      }
   break;
   };
   return (std::wstring(L""));
#else
   size_t size;
   switch (filenamebuf.type)
   {
   case AS_String::Type_UTF8:
   {
      return GetWideStringFromUTF8(static_cast<UInt8*>(filenamebuf.data));
   }
   break;
   case AS_String::Type_Wide:
   {
      return (std::wstring(reinterpret_cast<wchar_t*>(filenamebuf.data)));
   }
   break;
   case AS_String::Type_ASCII:
   {
      size_t size = mbstowcs(0, reinterpret_cast<char*>(filenamebuf.data), 0);
      wchar_t * buf = new wchar_t[size];
      mbstowcs(buf, reinterpret_cast<char*>(filenamebuf.data), static_cast<int>(size));
      return (std::wstring(reinterpret_cast<wchar_t*>(buf)));
   }
   break;
   };

   return std::wstring();
#endif
}
#endif

std::string GetACPName(const AS_File::Path & filenamebuf)
{
#ifdef _WIN32

	size_t size = 0;
	switch (filenamebuf.type)
	{
	case AS_String::Type_UTF8:
	{
		return GetStringFromUTF8(static_cast<char*>(filenamebuf.data));
	}
	break;
	case AS_String::Type_Wide:
	{
		size = (size_t) WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) filenamebuf.data, -1, 0, 0, 0, 0);
		char * cBuf = new char[size];
		WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) filenamebuf.data, -1, cBuf, static_cast<int>(size), 0, 0);
		return (std::string(cBuf));
	}
	break;
	case AS_String::Type_ASCII:
		{
			return (std::string(reinterpret_cast<char*>(filenamebuf.data)));

		}
	break;
	};
	return "";
#else
	size_t size;
	switch (filenamebuf.type)
	{
	case AS_String::Type_UTF8:
	{
		return GetStringFromUTF8(static_cast<char*>(filenamebuf.data));
	}
	break;
	case AS_String::Type_Wide:
	{
		char * cBuf = new char[size];
		size = wcstombs(cBuf, (wchar_t*)(filenamebuf.data), size);
		return (std::string(cBuf));
	}
	break;
	case AS_String::Type_ASCII:
	{
		return (std::string(reinterpret_cast<char*>(filenamebuf.data)));

	}
	break;
	};

	return "";
#endif
}

// get String from Error code

void OutputError(AS_StorageError errorCode)
{

	std::string ErrorString;

	switch (errorCode)
	{
		case AS_StorageError_None:
			ErrorString = "None";
			break;
		case AS_StorageError_BufferOverflow:
			ErrorString = "BufferOverflow";
			break;
		case AS_StorageError_DeviceError:
			ErrorString = "DeviceError";
			break;
		case AS_StorageError_DeviceNotReady:
			ErrorString = "DeviceNotReady";
			break;
		case AS_StorageError_DeviceSelected:
			ErrorString = "DeviceSelected";
			break;
		case AS_StorageError_Fatal:
			ErrorString = "Fatal";
			break;
		case AS_StorageError_FileExists:
			ErrorString = "FileExists";
			break;
		case AS_StorageError_FileNotFound:
			ErrorString = "FileNotFound";
			break;
		case AS_StorageError_FolderExists:
			ErrorString = "FolderExists";
			break;
		case AS_StorageError_FolderNotFound:
			ErrorString = "FolderNotFound";
			break;
      case AS_StorageError_InvalidAccess:
         ErrorString = "InvalidAccess";
         break;
		case AS_StorageError_InvalidFile:
			ErrorString = "InvalidFile";
			break;
		case AS_StorageError_InvalidFolder:
			ErrorString = "InvalidFolder";
			break;
		case AS_StorageError_InvalidIndex:
			ErrorString = "InvalidIndex";
			break;
		case AS_StorageError_InvalidLanguageCode:
			ErrorString = "InvalidLanguageCode";
			break;
		case AS_StorageError_InvalidMediaType:
			ErrorString = "InvalidMediaType";
			break;
		case AS_StorageError_InvalidMetadata:
			ErrorString = "InvalidMetadata";
			break;
		case AS_StorageError_InvalidMetadataName:
			ErrorString = "InvalidMetadataName";
			break;
		case AS_StorageError_InvalidOperation:
			ErrorString = "InvalidOperation";
			break;
		case AS_StorageError_InvalidParameter:
			ErrorString = "InvalidParameter";
			break;

		case AS_StorageError_InvalidProjectType:
			ErrorString = "InvalidProjectType";
			break;

		case AS_StorageError_InvalidStringType:
			ErrorString = "InvalidStringType";
			break;

		case AS_StorageError_InvalidTime:
			ErrorString = "InvalidTime";
			break;
		case AS_StorageError_InvalidTimeUnit:
			ErrorString = "InvalidTimeUnit";
			break;
		case AS_StorageError_InvalidTrackType:
			ErrorString = "InvalidTrackType";
			break;
		case AS_StorageError_MediaFull:
			ErrorString = "MediaFull";
			break;
		case AS_StorageError_MetadataNotFound:
			ErrorString = "MetadataNotFound";
			break;
		case AS_StorageError_MetadataTypeMismatch:
			ErrorString = "MetadataTypeMismatch";
			break;
		case AS_StorageError_NotEnoughStorage:
			ErrorString = "NotEnoughStorage";
			break;
		case AS_StorageError_NotFound:
			ErrorString = "NotFound";
			break;
		case AS_StorageError_NotImplemented:
			ErrorString = "NotImplemented";
			break;
		case AS_StorageError_NotInitialized:
			ErrorString = "NotInitialized";
			break;
		case AS_StorageError_NotSupported:
			ErrorString = "NotSupported";
			break;
		case AS_StorageError_ReentrantCall:
			ErrorString = "ReentrantCall";
			break;
		case AS_StorageError_TrackCountOverflow:
			ErrorString = "TrackCountOverflow";
			break;
		case AS_StorageError_UnsupportedVersion:
			ErrorString = "UnsupportedVersion";
			break;
		case AS_StorageError_UserCancelled:
			ErrorString = "UserCancelled";
			break;


	};

	std::cout << "Error: " << ErrorString << std::endl;

}


void OutputDate(const AS_Volume::DateTime & fileDate)
{
	char buf[256];
	sprintf(buf," %2d/%02d/%02d on %2d:%02d:%02d %d",fileDate.day, fileDate.month, fileDate.year, fileDate.hour, fileDate.minute, fileDate.second, fileDate.zone);
	std::cout << buf << std::endl;
}
