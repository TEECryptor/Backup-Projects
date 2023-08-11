//-----------------------------------------------------------------------------
// QflixSample.cpp
// Copyright (c) 2006, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

/*

Demonstrate usage of AS_Storage SDK to create Qflix disc.
Also demonstrates usage of AS_Storage SDK to just write a regular ISO to regular media.

Requirements to build:
1) multiple headers from the AS_Storage SDK or AS_Storage MOD SDK
2) Link with AS_Storage_w32.lib or AS_Storage_pro_w32.lib
3) compile as /MT or /MTd

Requirements to run:
1) needs MSXML 4.0 or greater; found on the web at www.microsoft.com
2) needs Sonic's MSXML wrapper - SonicWinPlugin.DLL (should be included with this file)
3) needs Sonic's burn engine already installed - an included EXE will do this.
4) needs AS_Storage_w32.dll or AS_Storage_pro_w32.dll available in the directory.
5) needs StoragePConfig.dcf available in the directory.


*/

#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <vector>


#include "AS_StorageTypes.h"
#include "AS_StorageError.h"
#include "AS_StringHelper.h"
#include "AS_StorageDevice.h"

#include "AS_Format.h"


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//
// function:   WriteIsoUsingCopy_intheclear
//
// desc:       Use AS_StorageDevice to write an image with no modifications,
//             including no CSS.  Just write an image file from block 0 to N.  
//             Regular media is required, not Qflix media.
//
// entry:      device = AS_StorageDevice which is already open.
//
// return:     0=SUCCESS; all others are some failure.
//
// notes :     example of copying in the clear is slightly different from
//             writing a qflix disc with CSS.  We open the source image. No
//             XML file would be required in this case.
//
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
AS_StorageError WriteIsoUsingCopy_intheclear(AS_StorageDevice& device)
{
	AS_StorageError ASrc;		// return codes can be looked up in AS_StorageError.h

	UInt32 srcDriveIndex = 0;	// The file has 0 index.
	AS_StringA srcpath("dvdshort.iso");	// the source FILENAME. (modify to your image name).
	AS_StorageDevice srcDevice;	// need a new device to represent the source image file.
	srcDevice.deviceType = AS_StorageDevice::Type_File;	// the source is type FILE.
	srcDevice.myHandle = 0xffffffff;

	// Open the source file on the harddrive.
	ASrc = AS_OpenStorageDevice(srcDriveIndex, srcDevice, srcpath);
	if (ASrc != AS_StorageError_None)
		return ASrc; // We cannot even open your source image file, we have to exit now.

	if (srcDevice.myHandle == 0xffffffff)
		return AS_StorageError_InvalidHandle;  // AS_Storage couldn't open this file.

	// The destination device is already open. You passed it in this way.
	// We should be ready to write now.
	AS_StorageDevice::FileFormatType formatType = AS_StorageDevice::FormatType_OpticalDetect;
	AS_StorageDevice::CopyFlags copyFlag = AS_StorageDevice::CopyFlags_Write;

	ASrc = AS_StorageDevice_Copy(srcDevice, 
		formatType, // File Format Type , always FormatType_OpticalDetect for ISO image.
		copyFlag,	// copyflags have some options, see header file.
		1,			// track number to write, usually 1 (GI files can contain multiple tracks, ISO cannot)
		1,			// Number of destination drives for simultaneous write, example is 1
		&device,	// array of destination drives; in our case only one drive.
		0,			// callback function not shown for example
		0);			// array of caller-use parameters for the callback.

	// Regardless of our completion status, we should close the source file.
	AS_StorageDevice_Close(srcDevice);

	return(ASrc);
}


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//
// function:   WriteIsoUsingCopy
//
// desc:       Use AS_StorageDevice to write an image with no modifications.
//             Qflix 2.0: This example will work fine for Qflix 2.0.
//
// entry:      device = AS_StorageDevice which is already open.
//
// return:     0=SUCCESS; all others are some failure.
//
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
AS_StorageError WriteIsoUsingCopy(AS_StorageDevice& device)
{
	AS_StorageError ASrc;		// return codes can be looked up in AS_StorageError.h

	UInt32 srcDriveIndex = 0;	// The file has 0 index.
	AS_StorageDevice srcDevice;	// need a new device to represent the source image file.

	// The API for Copy wants an AS_StorageDevice for the source ISO image.  But for Qflix,
	// our source image is in the XML, and we already set all that up.  We can send it
	// a dummy device in that case.  We dont set any parameters of srcDevice here.

	// We already know our destination drive; it was passed in.
	// We do need to add an XML file to the destination drive to describe the copy job a little better.
	AS_StringA XMLfile = AS_StringA("QflixSample.xml");
	device.deviceXMLOptionsPath = XMLfile; // associate this XML file with the destination device.

	ASrc = AS_StorageDevice_Copy(srcDevice, // our dummy source device.  The XML really knows where the source is.
				AS_StorageDevice::FormatType_Unknown,
				AS_StorageDevice::CopyFlags_QflixPro,  // with this flag, it knows to look at the XML parameter.
				1, // tracknum
				1, // num devices - only one target is allowed at the moment.
				&device, // pointer to devicelist (our 1 device), with deviceXMLOptionsPath parameter set.
#ifdef USING_CALLBACKS
				DeviceInfo_Callback, /*Device_ProgressCallback*/ //callback list
				(void*)ThisJobCallback  // "ThisJobCallback" is the caller use field.
#else
				0,
				0
#endif
				); // caller use list

	//
	// How to Verify disc with copy protection such as CSS or ripguard (or both).
	//
	if (ASrc == AS_StorageError_None)
		ASrc = AS_StorageDevice_Copy(srcDevice, // our source device is set up as type "file" with a path.
					AS_StorageDevice::FormatType_Unknown,
					AS_StorageDevice::CopyFlags_Compare_CopyProtection, // with this flag, it knows to look at the XML parameter.
					1, // tracknum
					1, // num devices - only one target is allowed at the moment.
					&device, // pointer to devicelist (our 1 device)
#ifdef USING_CALLBACKS
					DeviceInfo_Callback, /*Device_ProgressCallback*/ //callback list
					(void*)ThisJobCallback  // "ThisJobCallback" is the caller use field.
#else
					0,
					0
#endif
					); // caller use list

	return(ASrc);
}


//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//
// function:   WriteIsoUsingASFormat
//
// desc:       Use AS_StorageDevice to write an image , but reformat it to fit
//             on Qflix Pro v1.0 media.  This modification moves files to 
//             different offsets.  The original image is expected to be 4100mb
//             or less, due to the space requirements of Qflix Pro 1.0 media.
//             This example mostly serves for Qflix Pro 1.0, and is not needed
//             for version 2.
//             This method will work on any write-once media, however.
//
// entry:      device = AS_StorageDevice which is already open.
//
// return:     0=SUCCESS; all others are some failure.
//
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
AS_StorageError WriteIsoUsingASFormat(AS_StorageDevice& device)
{
	AS_StorageError ASrc;		// return codes can be looked up in AS_StorageError.h
	AS_Format format;			// we use an AS_Format type of job
	AS_Volume::FS_Type fsType;

	ASrc = AS_StorageDevice_OpenFormat(device, fsType, 0/*Info_Callback*/, 0, format);
	if (ASrc != AS_StorageError_None)
	{
		// We failed to openformat.  should exit now.
		return(ASrc);
	}

	ASrc = AS_Format_Create(format, // Create a new disc with some defaults.
			AS_Volume::FS_Default, 
			AS_Volume::Type_DVD, 
			AS_StringA("DVD_VIDEO"), 
			AS_Volume::Mastering, 
			0/*Info_Callback*/, 
			0/* caller use */);
	if (ASrc != AS_StorageError_None)
	{
		// We failed to create a new format.  Should exit now.
		AS_Format_Close(format);
		return(ASrc);
	}

	// We load the rest of the parameters for this job from XML file.
	AS_StringA XMLfile = AS_StringA("QflixSample.xml"); // generally you use full paths...
	ASrc = AS_Format_SetProjectProperties(format, XMLfile);
	if (ASrc != AS_StorageError_None)
	{
		// We failed to set project properties.
		// We need MSXML4.0 or above, and we need "SonicWinPlugin.DLL" available for use to parse XML.
		AS_Format_Close(format);
		return(ASrc);
	}

	// Prepare to write this new volume.
	ASrc = AS_Volume_Prepare(format, 0/*Info_Callback*/, 0, AS_Volume::Border_Close, 0);
	if (ASrc != AS_StorageError_None)
	{
		// We failed to prepare to write.  Should exit now.
		AS_Format_Close(format);
		return(ASrc);
	}

	// Write this volume now.  Optionally, there are callbacks with progress complete.
	ASrc = AS_Volume_Flush(format, 
#ifdef USING_CALLBACKS
				Info_Callback,
				(void*)ThisJobCallback,  // "ThisJobCallback" is the caller use field.
#else
				0,
				0,
#endif
				AS_Volume::Border_Close, 
				0);
	if (ASrc != AS_StorageError_None)
	{
		// We failed to write this disc.  Cleanup before you exit.
		AS_Format_Close(format);
		return(ASrc);
	}

	// Done, close it.
	ASrc = AS_Format_Close(format);
	if (ASrc != AS_StorageError_None)
	{
		// Error was really not expected here.
		return(ASrc);
	}

	// The resulting disc does not resemble the original image.  We cannot verify this image.

	return(AS_StorageError_None);
}



//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//
// function:   WinMain
//
// desc:       Find devices and check their properties.  
//             Use AS_StorageDevice to write an image.
//
//
// return:     0=SUCCESS; all others are some failure.
//
//ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hInstance; hPrevInstance; lpCmdLine; nCmdShow;

	AS_StorageError ASrc = 0; // return codes can be looked up in AS_StorageError.h
	UInt32 OpticalCount = 0;

	// Ask how many devices AS_Storage has found.
	ASrc = AS_GetStorageDeviceCount(AS_StorageDevice::Scan_Optical, OpticalCount);

	if (ASrc != AS_StorageError_None || !OpticalCount)
	{
		// there has been an error with the engine or zero drives were found.
		return(ASrc);
	}

	AS_StorageDevice device;

	// Loop through the devices we found and check the serial number of each one.
	// This is useful to match up drives on a multi-recorder machine with several of the same exact drive model.
	for (UInt32 DeviceCount = 1; DeviceCount <= OpticalCount; DeviceCount++)
	{
		ASrc = AS_OpenStorageDevice(DeviceCount, device);
		if (ASrc != AS_StorageError_None)
			continue;   // Drive could not be opened.  Is it busy ?  Something wrong with this drive, keep going.

		UInt32 retsize = 0;
		UInt8 ASSerialNumber[16];
		UInt8 Len = 16;
		memset(ASSerialNumber, 0, Len);

		ASrc = AS_StorageDevice_GetDeviceProperty(device, 
				AS_StorageDevice::DevProp_Type::DevProp_SerialNum, // there are many properties to query; see AS_StorageDevice.h
				Len, 
				ASSerialNumber, 
				&retsize,
				0, 0, 0, 0);

		if (ASrc == AS_StorageError_None)
		{
			// ASSerialNumber can be compared to find the drive required for the job.
			// Keep looping until you've found the drive required.
			break; // leave the drive in the 'open' state.
		}

		AS_StorageDevice_Close(device); // loops from here to the next drive.
	}

	// Assure ourselves this drive is ready to go.
	AS_StorageDevice::DeviceState theState;
	AS_StorageDevice_GetState(device, theState);
	if (theState != AS_StorageDevice::State_Ready)
	{
		// This drive doesnt have media, should take care of that.
		return(1);
	}

    
	// If we've found a drive suitable to write on, Write our image.  Get exclusive access to this drive first.
	AS_String currentUsedApp;
	ASrc = AS_StorageDevice_ExclusiveAccess(
			device,						// IN:  A constant reference to the AS_StorageDevice struct specifying the device.
			AS_StorageDevice::ExclusiveAccess_Obtain,	// IN:  desired access
			AS_StringA("Our example application"),		// IN:  name of your application
			currentUsedApp);			// OUT: name of application currently using drive, if in use.

	if (ASrc != AS_StorageError_None)
	{
		// We could not get exclusive access for some reason.  You sould probably exit.
		AS_StorageDevice_Close(device); // loops from here to the next drive.
		return(1);
	}

	// Enable one of the following examples ; all 3 together doesnt work.

	// example 1
	// We demonstrate how to write an image with some modifications to the image, to produce a new disc, by using AS_Format.
	// This example pertains to Qflix Pro 1.0, but will work for any media type.
	//ASrc = WriteIsoUsingASFormat(device);

	// example 2
	// We could demonstrate how to write an image using AS_StorageDevice_Copy(), too.  No modifications are done, the raw image is written.
	// Enable one type of copy or the other, dont need both.
	// This example pertains to Qflix 2.0.
	ASrc = WriteIsoUsingCopy(device);

	// example 3
	// We can demonstrate how to write an image "in the clear" using AS_StorageDevice_Copy().  No modifications are done, and no CSS is added.
	// A generic example with no copy protection.
	//ASrc = WriteIsoUsingCopy_intheclear(device);


	// Release exclusive access on this drive.
	AS_StorageDevice_ExclusiveAccess(
			device,						// IN:  A constant reference to the AS_StorageDevice struct specifying the device.
			AS_StorageDevice::ExclusiveAccess_Release,	// IN:  desired access
			AS_StringA("Our example application"),		// IN:  name of your application
			currentUsedApp);			// OUT: name of application currently using drive, if in use.

	// Close the drive when we finished with it.
	AS_StorageDevice_Close(device);

	return(ASrc);
}

