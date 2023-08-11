/***************************************************************************
 *	File name:slots.c
 *	Introduce:The implement file for object slots functions
 *	Author:Syno common
 *	Date:2015/10/30
 *  Version:v1.0
 **************************************************************************/
#include "slots.h"
#include "SynoTFKeySKF.h"

CK_ULONG	g_ulSlotCount = 0;
LPSLOTINFO	g_pSlotList = NULL;	


/*
 *	获取某个Slot的信息
 */
CK_RV SLT_GetSlotInfo(CK_SLOT_ID ulSlotID, 
					  CK_SLOT_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulRes = SAR_OK;
	CK_ULONG ulIndex = 0;
	DEVHANDLE hDev = NULL;
	DEVINFO stuDevInfo = {0};

	// 参数检查
	if (!pInfo)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// 查找Slot
	for (ulIndex = 0; ulIndex < g_ulSlotCount; ulIndex++)
	{
		if (g_pSlotList[ulIndex].ulSlotID == ulSlotID)
		{
			break;
		}
	}
	if (ulIndex == g_ulSlotCount)
	{
		return CKR_SLOT_ID_INVALID;
	}

	// 连接设备
	ulRes = SKF_ConnectDev(g_pSlotList[ulIndex].csDevName, &hDev);
	if (SAR_OK != ulRes)
	{
		return CKR_DEVICE_ERROR;
	}

	// 获取设备信息
	ulRes = SKF_GetDevInfo(hDev, &stuDevInfo);
	if (SAR_OK != ulRes)
	{
		rv = CKR_DEVICE_ERROR;
		goto END;
	}
	
	memset(pInfo, 0, sizeof(CK_SLOT_INFO));

	// 固件版本
	pInfo->firmwareVersion.major = stuDevInfo.FirmwareVersion.major;
	pInfo->firmwareVersion.minor = stuDevInfo.FirmwareVersion.minor;

	// 硬件版本
	pInfo->hardwareVersion.major = stuDevInfo.HWVersion.major;
	pInfo->hardwareVersion.minor = stuDevInfo.HWVersion.minor;
	
	// 厂商信息
	strcpy_s(pInfo->manufacturerID, 32, stuDevInfo.Manufacturer);

	// 描述信息
	memset(pInfo->slotDescription, ' ', 64);

	// 保留
	pInfo->flags = 0;

END:
	SKF_DisConnectDev(hDev);
	hDev = NULL;

	return rv;
}
/*
 *	获取某个Slot中的Token信息
 */
CK_RV SLT_GetTokenInfo(CK_SLOT_ID ulSlotID, 
					   CK_TOKEN_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulRes = SAR_OK;
	CK_ULONG ulIndex = 0;
	DEVHANDLE hDev = NULL;
	DEVINFO stuDevInfo = {0};

	// 参数检查
	if (!pInfo)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// 查找Slot
	for (ulIndex = 0; ulIndex < g_ulSlotCount; ulIndex++)
	{
		if (g_pSlotList[ulIndex].ulSlotID == ulSlotID)
		{
			break;
		}
	}
	if (ulIndex == g_ulSlotCount)
	{
		return CKR_SLOT_ID_INVALID;
	}

	// 连接设备
	ulRes = SKF_ConnectDev(g_pSlotList[ulIndex].csDevName, &hDev);
	if (SAR_OK != ulRes)
	{
		return CKR_DEVICE_ERROR;
	}

	// 获取设备信息
	ulRes = SKF_GetDevInfo(hDev, &stuDevInfo);
	if (SAR_OK != ulRes)
	{
		rv = CKR_DEVICE_ERROR;
		goto END;
	}
	
	memset(pInfo, 0, sizeof(CK_TOKEN_INFO));

	// 设备标签
	memset(pInfo->label, ' ', 32);
	strcpy_s(pInfo->label, 32, stuDevInfo.Label);

	// 厂商信息
	memset(pInfo->manufacturerID, ' ', 32);
	memcpy_s(pInfo->manufacturerID, 32, stuDevInfo.Manufacturer, min(strlen(stuDevInfo.Manufacturer), 64));

	// 型号
	memset(pInfo->model, ' ', 16);

	// 序列号
	memset(pInfo->serialNumber, ' ', 16);
	memcpy_s(pInfo->serialNumber, 16, stuDevInfo.SerialNumber, min(strlen(stuDevInfo.SerialNumber), 32));
	
	// 标记
	pInfo->flags = 0;

	// 最大会话数
	pInfo->ulMaxSessionCount = 1;

	// 当前会话数
	pInfo->ulSessionCount = 0;

	// 最大R/W会话数
	pInfo->ulMaxRwSessionCount = 1;

	// 当前R/W会话数
	pInfo->ulRwSessionCount = 0;

	// PIN码最大长度
	pInfo->ulMaxPinLen = 16;

	// PIN码最小长度
	pInfo->ulMinPinLen = 6;

	// 公共存储区域大小
	pInfo->ulTotalPublicMemory = stuDevInfo.TotalSpace;

	// 可用公共存储区域大小
	pInfo->ulFreePublicMemory = stuDevInfo.FreeSpace;

	// 私有存储区域大小
	pInfo->ulTotalPrivateMemory = 0;

	// 可用私有存储区域大小
	pInfo->ulFreePrivateMemory = 0;

	// 硬件版本
	pInfo->hardwareVersion.major = stuDevInfo.HWVersion.major;
	pInfo->hardwareVersion.minor = stuDevInfo.HWVersion.minor;

	// 固件版本
	pInfo->firmwareVersion.major = stuDevInfo.FirmwareVersion.major;
	pInfo->firmwareVersion.minor = stuDevInfo.FirmwareVersion.minor;

	// 时间
	memset(pInfo->utcTime, 0, 16);

END:
	SKF_DisConnectDev(hDev);
	hDev = NULL;

	return rv;
}
/*
 *	获取某个Slot所支持的算法列表
 */
CK_RV SLT_GetMechanismList(CK_SLOT_ID slotID, 
						   CK_MECHANISM_TYPE_PTR pMechanismList, 
						   CK_ULONG_PTR pulCount)
{
	return CKR_OK;
}
/*
 *	获取某个Slot的算法信息
 */
CK_RV SLT_GetMechanismInfo(CK_SLOT_ID slotID, 
						   CK_MECHANISM_TYPE type, 
						   CK_MECHANISM_INFO_PTR pInfo)
{
	return CKR_OK;
}