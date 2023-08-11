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
 , m_iNumberOfCopies(0)
 , m_pASMedia(NULL)
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
, m_iNumberOfCopies(0)
, m_pASMedia(NULL)
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
//	Name:GetID
//	Introduce:Get this driver ID in system
//-----------------------------------------------
DWORD CASDrive::GetID()
{
	return m_uIndex;
}
//-----------------------------------------------
//	Name:GetDescription
//	Introduce:Return the device description string
//-----------------------------------------------
LONG CASDrive::GetDescription(LPWSTR lpwzDescStr, DWORD dwStrLen)
{
	if(NULL == lpwzDescStr || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	WCHAR wszDescStr[AS_STRING_LEN] = {L"\0"};
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_Description,
					AS_STRING_LEN,
					wszDescStr,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//Copy string to return buffer
	wcsncpy(lpwzDescStr, wszDescStr, dwStrLen);

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetDrivePath
//	Introduce:Return the device path string
//-----------------------------------------------
LONG CASDrive::GetDrivePath(LPWSTR lpwzPathStr, DWORD dwStrLen)
{
	if(NULL == lpwzPathStr || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	WCHAR wszPathStr[AS_STRING_LEN] = {L"\0"};
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_DrivePath,
					AS_STRING_LEN,
					wszPathStr,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//Copy string to return buffer
	wcsncpy(lpwzPathStr, wszPathStr, dwStrLen);

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetManufacturer
//	Introduce:Return manufacturer name for this device
//-----------------------------------------------
LONG CASDrive::GetManufacturer(LPWSTR lpwzManufacturerStr, DWORD dwStrLen)
{
	if(NULL == lpwzManufacturerStr || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	WCHAR wszManufacturerStr[AS_STRING_LEN] = {L"\0"};
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_MakeName,
					AS_STRING_LEN,
					wszManufacturerStr,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//Copy string to return buffer
	wcsncpy(lpwzManufacturerStr, wszManufacturerStr, dwStrLen);

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetModelName
//	Introduce:Return the model name for this device
//-----------------------------------------------
LONG CASDrive::GetModelName(LPWSTR lpwzModelName, DWORD dwStrLen)
{
	if(NULL == lpwzModelName || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	WCHAR wszModelName[AS_STRING_LEN] = {L"\0"};
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_ModelName,
					AS_STRING_LEN,
					wszModelName,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//Copy string to return buffer
	wcsncpy(lpwzModelName, wszModelName, dwStrLen);

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetVersion
//	Introduce:Return the version for this device
//-----------------------------------------------
LONG CASDrive::GetVersion(LPWSTR lpwzVersion, DWORD dwStrLen)
{
	if(NULL == lpwzVersion || dwStrLen <= 0)
		return AS_StorageError_InvalidParameter;

	UInt32 uSize = 0;
	WCHAR wszVersion[AS_STRING_LEN] = {L"\0"};
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty (m_ASDevice, 
					AS_StorageDevice::DevProp_Version,
					AS_STRING_LEN,
					wszVersion,
					&uSize);
	//An error
	if(AS_StorageError_None != err)
		return err;

	//Copy string to return buffer
	wcsncpy(lpwzVersion, wszVersion, dwStrLen);

	return AS_StorageError_None;
}
//-----------------------------------------------
//	Name:GetCapableTypes
//	Introduce:Return the device capable type flag
//-----------------------------------------------
DWORD CASDrive::GetCapableTypes()
{
	UInt32 uSize = 0;
	DWORD dwCapableTypes = 0;
	AS_Error err = AS_StorageError_None;
	err = AS_StorageDevice_GetDeviceProperty(m_ASDevice, 
		AS_StorageDevice::DevProp_MedTypesCapable,
		sizeof(dwCapableTypes),
		&dwCapableTypes,
		&uSize);
	//An error
	if(AS_StorageError_None != err)
		return 0;

	return dwCapableTypes;
}
//-----------------------------------------------
//	Name:GetDataRates
//	Introduce:Return data rates for this device
//-----------------------------------------------
LONG CASDrive::GetDataRates(DWORD dwDataRates[], DWORD dwArraySize)
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
	DWORD dwCnt = 0;
	while(0 != uDataRates[dwCnt] && dwCnt < dwArraySize)
	{
		dwDataRates[dwCnt] = uDataRates[dwCnt];
		dwCnt++;
	}

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
//	Name:Close
//	Introduce:Close this device
//-----------------------------------------------
LONG CASDrive::Close(BOOL bSybc)
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
LONG CASDrive::Eject(BOOL bSybc)
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
//	Name:GetMediaKind
//	Introduce:Get current media kind in this device.(MediaIsCdRom, MediaIsDvdRom, etc)
//-----------------------------------------------
DWORD CASDrive::GetMediaKind()
{
	DWORD dwMedKind = 0;
	AS_StorageError err = AS_StorageError_None;

	UInt32 ret_data_size = 0;
	err = AS_StorageDevice_GetMediaProperty(m_ASDevice,
					AS_StorageDevice::MedProp_Kind,
					sizeof(dwMedKind),
					&dwMedKind,
					&ret_data_size, 0, 0, 0, 0);
	if(AS_StorageError_None != err)
		return 0;

	return dwMedKind;
}
//-----------------------------------------------
//	Name:GetMediaState
//	Introduce:Get current media state in this device.(MediaIsReadable, MediaIsWritable,etc)
//-----------------------------------------------
DWORD CASDrive::GetMediaState()
{
	DWORD dwMedState = 0;
	AS_StorageError err = AS_StorageError_None;

	UInt32 ret_data_size = 0;
	err = AS_StorageDevice_GetMediaProperty(m_ASDevice,
					AS_StorageDevice::MedProp_State,
					sizeof(dwMedState),
					&dwMedState,
					&ret_data_size, 0, 0, 0, 0);
	if(AS_StorageError_None != err)
		return 0;

	return dwMedState;
}
//-----------------------------------------------
//	Name:GetMedia
//	Introduce:Get current media in this device
//-----------------------------------------------
CMedia* CASDrive::GetMedia()
{
	return m_pASMedia;
}
//-----------------------------------------------
//	Name:SetNumberOfCopies
//	Introduce:Set the number of copies for this device in burning project
//-----------------------------------------------
void CASDrive::SetNumberOfCopies(int iCopies)
{
	if(iCopies < 0)
		iCopies = 0;
	m_iNumberOfCopies = iCopies;
}
//-----------------------------------------------
//	Name:GetNumberOfCopies
//	Introduce:Get the number of copies for this device in burning project
//-----------------------------------------------
int CASDrive::GetNumberOfCopies()
{
	return m_iNumberOfCopies;
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
					AS_StorageDevice::ExclusiveAccess_Type(dwAccessType), 
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
//	Name:IsWriteable
//	Introduce:Return this device can write or not
//-----------------------------------------------
BOOL CASDrive::IsWriteable()
{
	return TRUE;
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



//-----------------------------------------------
//	Name:CanWriteDVD
//	Introduce:Return this drive can write DVD or not
//-----------------------------------------------
BOOL CASDrive::CanWriteDVD()
{
	DWORD dwCapableTypes = 0;
	dwCapableTypes = GetCapableTypes();

	BOOL bCanWriteDVD = FALSE;	
	if(dwCapableTypes & AS_StorageDevice::MediaIsDvdrw) bCanWriteDVD = TRUE;
	if(dwCapableTypes & AS_StorageDevice::MediaIsDvdprw) bCanWriteDVD = TRUE;
	if(dwCapableTypes & AS_StorageDevice::MediaIsDvdRam) bCanWriteDVD = TRUE;
	if(dwCapableTypes & AS_StorageDevice::MediaIsDvdpr9) bCanWriteDVD = TRUE;
	if(dwCapableTypes & AS_StorageDevice::MediaIsDvdr9) bCanWriteDVD = TRUE;
	if(dwCapableTypes & AS_StorageDevice::MediaIsHdDvdrw) bCanWriteDVD = TRUE;

	return bCanWriteDVD;
}
//-----------------------------------------------



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

	if(NULL == m_pASMedia)
		m_pASMedia = new CASMedia();
	err = m_pASMedia->Open(m_ASDevice, Callback, lpUserData); 
	if(AS_StorageError_None != err)
		return NULL;

	return m_pASMedia;
}
//-----------------------------------------------