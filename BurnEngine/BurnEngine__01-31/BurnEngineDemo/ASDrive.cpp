//-----------------------------------------------
//	ASDrive.cpp
//	The implement file for CASDrive
//-----------------------------------------------
#include "StdAfx.h"
#include ".\asdrive.h"
//-----------------------------------------------
//	Name:CASDrive
//	Introduce:Construction
//-----------------------------------------------
CASDrive::CASDrive(DWORD dwIndex, LPCTSTR lpAppName)
 : m_uIndex(dwIndex)
{
	m_ASDevice.myHandle = 0xffffffff;
	lstrcpy(m_szImageFile, _T("\0"));

	if(NULL == lpAppName || lstrlen(lpAppName))
	{
		strcpy(m_scAppName, "\0");
	}
	else
	{
#ifdef UNICODE	
		WideCharToMultiByte(CP_ACP, 0, lpAppName, -1, m_scAppName, AS_STRING_LEN, NULL, NULL);
#else    
		strcpy(m_scAppName, lpAppName);
#endif	//UNICODE
	}
}
//-----------------------------------------------
//	Name:CASDrive
//	Introduce:Construction
//-----------------------------------------------
CASDrive::CASDrive(LPCTSTR lpszImageFile, LPCTSTR lpAppName)
: m_uIndex(0)
{
	m_ASDevice.myHandle = 0xffffffff;
	lstrcpy(m_szImageFile, lpszImageFile);

	if(NULL == lpAppName || lstrlen(lpAppName))
	{
		strcpy(m_scAppName, "\0");
	}
	else
	{
#ifdef UNICODE	
		WideCharToMultiByte(CP_ACP, 0, lpAppName, -1, m_scAppName, AS_STRING_LEN, NULL, NULL);
#else    
		strcpy(m_scAppName, lpAppName);
#endif	//UNICODE
	}
}
//-----------------------------------------------
//	Name:~CASDrive
//	Introduce:Distruction
//-----------------------------------------------
CASDrive::~CASDrive(void)
{
}
//-----------------------------------------------


//-----------------------------------------------
//	Name:Reserve
//	Introduce:Reserve this device
//-----------------------------------------------
SInt32 CASDrive::Reserve(DWORD dwAccessType)
{
	char AppName[80];
	AS_StringA currentUsedApp(AppName);
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_ExclusiveAccess(m_ASDevice, 
					(AS_StorageDevice::ExclusiveAccess_Type)dwAccessType, 
					AS_StringA(m_scAppName), 
					currentUsedApp);
	if(AS_StorageError_None != err)
		return err;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:Release
//	Introduce:Relese this device
//-----------------------------------------------
SInt32 CASDrive::Release()
{
	AS_Error err = AS_StorageError_None;

	char AppName[80];
	AS_StringA currentUsedApp(AppName);
	err = AS_StorageDevice_ExclusiveAccess(m_ASDevice, 
					AS_StorageDevice::ExclusiveAccess_Release, 
					AS_StringA(m_scAppName), 
					currentUsedApp);
	if(AS_StorageError_None != err)
		return err;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:IsReady
//	Introduce:Return drive is ready or not
//-----------------------------------------------
BOOL CASDrive::IsReady()
{
	AS_Error err = AS_StorageError_None;
	AS_StorageDevice::DeviceState eState;
	err = AS_StorageDevice_GetState(m_ASDevice, eState);

	//An error
	if(AS_StorageError_None != err)
		return FALSE;

	//The current state is not ready
	if(AS_StorageDevice::State_Ready != eState)
		return FALSE;

	return TRUE;
}
//-----------------------------------------------
//	Name:IsWriteable
//	Introduce:Return this device can write or not
//-----------------------------------------------
BOOL CASDrive::IsWriteable()
{
	return TRUE;
}
//-----------------------------------------------
//	Name:Close
//	Introduce:Close this device
//-----------------------------------------------
SInt32 CASDrive::Close(BOOL bSybc)
{
	AS_StorageError err = AS_StorageError_None;

	//Close this device by Synchronism
	if(TRUE == bSybc)
		err = AS_StorageDevice_TrayControl(m_ASDevice, 
		AS_StorageDevice::Tray_Control_Close_Sync);
	//Close this device by Asynchronous
	else
		err = AS_StorageDevice_TrayControl(m_ASDevice, 
		AS_StorageDevice::Tray_Control_Close);

	return err;
}
//-----------------------------------------------
//	Name:Eject
//	Introduce:Eject this device
//-----------------------------------------------
SInt32 CASDrive::Eject(BOOL bSybc)
{
	AS_StorageError err = AS_StorageError_None;

	//Open this device by Synchronism
	if(TRUE == bSybc)
		err = AS_StorageDevice_TrayControl(m_ASDevice, 
						AS_StorageDevice::Tray_Control_Open_Sync);
	//Open this device by Asynchronous
	else
		err = AS_StorageDevice_TrayControl(m_ASDevice, 
						AS_StorageDevice::Tray_Control_Open);

	return err;
}
//-----------------------------------------------
//	Name:Lock
//	Introduce:Lock this device
//-----------------------------------------------
SInt32 CASDrive::Lock()
{
	AS_StorageError err = AS_StorageError_None;
	err = AS_StorageDevice_TrayControl(m_ASDevice, 
					AS_StorageDevice::Tray_Control_Lock);
	return err;
}
//-----------------------------------------------
//	Name:Unlock
//	Introduce:Unlock this device
//-----------------------------------------------
SInt32 CASDrive::Unlock()
{
	AS_StorageError err = AS_StorageError_None;
	err = AS_StorageDevice_TrayControl(m_ASDevice, 
					AS_StorageDevice::Tray_Control_Unlock);
	return err;
}
//-----------------------------------------------



//-----------------------------------------------
//	Name:GetID
//	Introduce:Get this driver ID in system
//-----------------------------------------------
UInt32 CASDrive::GetID()
{
	return m_uIndex;
}
//-----------------------------------------------
//	Name:GetImageFile
//	Introduce:Get the image file name of this drive.Only for image drive
//-----------------------------------------------
LPCTSTR CASDrive::GetImageFile()
{
	return m_szImageFile;
}
//-----------------------------------------------
//	Name:GetHandle
//	Introduce:Get the handle of AS_Storage device
//-----------------------------------------------
PULONG CASDrive::GetHandle()
{
	return (PULONG)&m_ASDevice;
}
//-----------------------------------------------
//	Name:GetDescription
//	Introduce:Return the device description string
//-----------------------------------------------
SInt32 CASDrive::GetDescription(LPWSTR lpwString, DWORD dwStrLen)
{
	if(NULL == lpwString || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_Description,
					(UInt32)dwStrLen,
					lpwString,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//The description string length is larger than dwStrLen
	if(uSize >= dwStrLen)
		return AS_StorageError_InvalidParameter;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetDrivePath
//	Introduce:Return the device path string
//-----------------------------------------------
SInt32 CASDrive::GetDrivePath(LPWSTR lpwString, DWORD dwStrLen)
{
	if(NULL == lpwString || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_DrivePath,
					(UInt32)dwStrLen,
					lpwString,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//The path string length is larger than dwStrLen
	if(uSize >= dwStrLen)
		return AS_StorageError_InvalidParameter;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetManufacturer
//	Introduce:Return manufacturer name for this device
//-----------------------------------------------
SInt32 CASDrive::GetManufacturer(LPWSTR lpwString, DWORD dwStrLen)
{
	if(NULL == lpwString || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_MakeName,
					(UInt32)dwStrLen,
					lpwString,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//The manufacturer name string length is larger than dwStrLen
	if(uSize >= dwStrLen)
		return AS_StorageError_InvalidParameter;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetModelName
//	Introduce:Return the model name for this device
//-----------------------------------------------
SInt32 CASDrive::GetModelName(LPWSTR lpwString, DWORD dwStrLen)
{
	if(NULL == lpwString || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_ModelName,
					(UInt32)dwStrLen,
					lpwString,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//The model name string length is larger than dwStrLen
	if(uSize >= dwStrLen)
		return AS_StorageError_InvalidParameter;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetVersion
//	Introduce:Return the version for this device
//-----------------------------------------------
SInt32 CASDrive::GetVersion(LPWSTR lpwString, DWORD dwStrLen)
{
	if(NULL == lpwString || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_Version,
					(UInt32)dwStrLen,
					lpwString,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//The version string length is larger than dwStrLen
	if(uSize >= dwStrLen)
		return AS_StorageError_InvalidParameter;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetCapableTypes
//	Introduce:Return the device capable type string
//-----------------------------------------------
SInt32 CASDrive::GetCapableTypes(LPWSTR lpwString, DWORD dwStrLen)
{
	if(NULL == lpwString || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	UInt32 uCapableTypes;
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty(m_ASDevice, 
					AS_StorageDevice::DevProp_MedTypesCapable,
					sizeof(uCapableTypes),
					&uCapableTypes,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	wcscpy(lpwString, L"\0");

	//Construct the capable types string
	if (uCapableTypes & AS_StorageDevice::MediaIsCdRom)
		wcscat(lpwString, L"CD-Rom");
	if (uCapableTypes & AS_StorageDevice::MediaIsCdr)
		wcscat(lpwString, L", CD-R");
	if (uCapableTypes & AS_StorageDevice::MediaIsCdrw)
		wcscat(lpwString, L", CD-RW");
	if (uCapableTypes & AS_StorageDevice::MediaIsDdCdrom)
		wcscat(lpwString, L", DD-CDRom");
	if (uCapableTypes & AS_StorageDevice::MediaIsDdCdr)
		wcscat(lpwString, L", DD-CDR");
	if (uCapableTypes & AS_StorageDevice::MediaIsDdCdrw)
		wcscat(lpwString, L", DDCD-RW");
	if (uCapableTypes & AS_StorageDevice::MediaIsDvdRom)
		wcscat(lpwString, L", DVD-Rom");
	if (uCapableTypes & AS_StorageDevice::MediaIsDvdr)
		wcscat(lpwString, L", DVD-R");
	if (uCapableTypes & AS_StorageDevice::MediaIsDvdrw)
		wcscat(lpwString, L", DVD-RW");
	if (uCapableTypes & AS_StorageDevice::MediaIsDvdpr)
		wcscat(lpwString, L", DVD+R");
	if (uCapableTypes & AS_StorageDevice::MediaIsDvdprw)
		wcscat(lpwString, L", DVD+RW");
	if (uCapableTypes & AS_StorageDevice::MediaIsDvdRam)
		wcscat(lpwString, L", DVD+RAM");
	if (uCapableTypes & AS_StorageDevice::MediaIsDvdpr9)
		wcscat(lpwString, L", DVD+R Dual layer");
	if (uCapableTypes & AS_StorageDevice::MediaIsDvdr9)
		wcscat(lpwString, L", DVD-R Dual Layer");
	if (uCapableTypes & AS_StorageDevice::MediaIsBdr)
		wcscat(lpwString, L", BD-R");
	if (uCapableTypes & AS_StorageDevice::MediaIsBdre)
		wcscat(lpwString, L", BD-RE");
	if (uCapableTypes & AS_StorageDevice::MediaIsSeqTape)
		wcscat(lpwString, L", Sequential Tape");

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetDataRates
//	Introduce:Return data rates for this device
//-----------------------------------------------
SInt32 CASDrive::GetDataRates(DWORD dwDataRates[])
{
	AS_Error err = AS_StorageError_None;

	UInt32 uSize = 0;
	UInt32 uDataRates[128] = {0};
	err = AS_StorageDevice_GetMediaProperty(m_ASDevice,
					AS_StorageDevice::MedProp_WriteList_DataRates,
					sizeof(uDataRates),
					&uDataRates, 
					&uSize);
	if(AS_StorageError_None != err)
		return err;

	//Copy datas
	int i = 0;
	while(0 != uDataRates[i])
	{
		dwDataRates[i] = uDataRates[i];
		i++;
	}

	return AS_StorageError_None;
}
//-----------------------------------------------



//-----------------------------------------------
//	Name:GetMediaKind
//	Introduce:Return the media kind in this driver
//-----------------------------------------------
UInt64 CASDrive::GetMediaKind()
{
	AS_StorageDevice::MediaKind MedKind;
	AS_StorageError err = AS_StorageError_None;

	UInt32 ret_data_size = 0;
	err = AS_StorageDevice_GetMediaProperty(m_ASDevice,
					AS_StorageDevice::MedProp_Kind,
					sizeof(MedKind),
					&MedKind,
					&ret_data_size, 0, 0, 0, 0);
	if(AS_StorageError_None != err)
		return 0;

	return MedKind;
}
//-----------------------------------------------
//	Name:GetMediaState
//	Introduce:Return the media state in this driver
//-----------------------------------------------
UInt32 CASDrive::GetMediaState()
{
	AS_StorageDevice::MediaState MedState;
	AS_StorageError err = AS_StorageError_None;

	UInt32 ret_data_size = 0;
	err = AS_StorageDevice_GetMediaProperty(m_ASDevice,
					AS_StorageDevice::MedProp_State,
					sizeof(MedState),
					&MedState,
					&ret_data_size, 0, 0, 0, 0);
	if(AS_StorageError_None != err)
		return 0;

	return MedState;
}
//-----------------------------------------------
//	Name:EraseMedia
//	Introduce:Erase the media in this driver
//-----------------------------------------------
SInt32 CASDrive::EraseMedia(AS_StorageDevice::InfoCallback Callback, LPVOID lpUserData)
{
	AS_StorageError err = AS_StorageError_None;

	err = AS_StorageDevice_EraseMedia(m_ASDevice,
					AS_StorageDevice::Quick_Erase, 
					Callback, 
					lpUserData);
	if (AS_StorageError_None != err)
		return err;

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetMedia
//	Introduce:Open current media object in this driver
//-----------------------------------------------
CASMedia* CASDrive::OpenMedia(AS_Volume::InfoCallback Callback, LPVOID lpUserData)
{
	AS_StorageError err = AS_StorageError_None;

	err = m_Media.Open(m_ASDevice, Callback, lpUserData); 
	if(AS_StorageError_None != err)
		return NULL;

	return &m_Media;
}
//-----------------------------------------------