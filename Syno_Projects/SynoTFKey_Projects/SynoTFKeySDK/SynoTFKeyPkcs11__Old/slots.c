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
 *	��ȡĳ��Slot����Ϣ
 */
CK_RV SLT_GetSlotInfo(CK_SLOT_ID ulSlotID, 
					  CK_SLOT_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulRes = SAR_OK;
	CK_ULONG ulIndex = 0;
	DEVHANDLE hDev = NULL;
	DEVINFO stuDevInfo = {0};

	// �������
	if (!pInfo)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// ����Slot
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

	// �����豸
	ulRes = SKF_ConnectDev(g_pSlotList[ulIndex].csDevName, &hDev);
	if (SAR_OK != ulRes)
	{
		return CKR_DEVICE_ERROR;
	}

	// ��ȡ�豸��Ϣ
	ulRes = SKF_GetDevInfo(hDev, &stuDevInfo);
	if (SAR_OK != ulRes)
	{
		rv = CKR_DEVICE_ERROR;
		goto END;
	}
	
	memset(pInfo, 0, sizeof(CK_SLOT_INFO));

	// �̼��汾
	pInfo->firmwareVersion.major = stuDevInfo.FirmwareVersion.major;
	pInfo->firmwareVersion.minor = stuDevInfo.FirmwareVersion.minor;

	// Ӳ���汾
	pInfo->hardwareVersion.major = stuDevInfo.HWVersion.major;
	pInfo->hardwareVersion.minor = stuDevInfo.HWVersion.minor;
	
	// ������Ϣ
	strcpy_s(pInfo->manufacturerID, 32, stuDevInfo.Manufacturer);

	// ������Ϣ
	memset(pInfo->slotDescription, ' ', 64);

	// ����
	pInfo->flags = 0;

END:
	SKF_DisConnectDev(hDev);
	hDev = NULL;

	return rv;
}
/*
 *	��ȡĳ��Slot�е�Token��Ϣ
 */
CK_RV SLT_GetTokenInfo(CK_SLOT_ID ulSlotID, 
					   CK_TOKEN_INFO_PTR pInfo)
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulRes = SAR_OK;
	CK_ULONG ulIndex = 0;
	DEVHANDLE hDev = NULL;
	DEVINFO stuDevInfo = {0};

	// �������
	if (!pInfo)
	{
		return CKR_ARGUMENTS_BAD;
	}

	// ����Slot
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

	// �����豸
	ulRes = SKF_ConnectDev(g_pSlotList[ulIndex].csDevName, &hDev);
	if (SAR_OK != ulRes)
	{
		return CKR_DEVICE_ERROR;
	}

	// ��ȡ�豸��Ϣ
	ulRes = SKF_GetDevInfo(hDev, &stuDevInfo);
	if (SAR_OK != ulRes)
	{
		rv = CKR_DEVICE_ERROR;
		goto END;
	}
	
	memset(pInfo, 0, sizeof(CK_TOKEN_INFO));

	// �豸��ǩ
	memset(pInfo->label, ' ', 32);
	strcpy_s(pInfo->label, 32, stuDevInfo.Label);

	// ������Ϣ
	memset(pInfo->manufacturerID, ' ', 32);
	memcpy_s(pInfo->manufacturerID, 32, stuDevInfo.Manufacturer, min(strlen(stuDevInfo.Manufacturer), 64));

	// �ͺ�
	memset(pInfo->model, ' ', 16);

	// ���к�
	memset(pInfo->serialNumber, ' ', 16);
	memcpy_s(pInfo->serialNumber, 16, stuDevInfo.SerialNumber, min(strlen(stuDevInfo.SerialNumber), 32));
	
	// ���
	pInfo->flags = 0;

	// ���Ự��
	pInfo->ulMaxSessionCount = 1;

	// ��ǰ�Ự��
	pInfo->ulSessionCount = 0;

	// ���R/W�Ự��
	pInfo->ulMaxRwSessionCount = 1;

	// ��ǰR/W�Ự��
	pInfo->ulRwSessionCount = 0;

	// PIN����󳤶�
	pInfo->ulMaxPinLen = 16;

	// PIN����С����
	pInfo->ulMinPinLen = 6;

	// �����洢�����С
	pInfo->ulTotalPublicMemory = stuDevInfo.TotalSpace;

	// ���ù����洢�����С
	pInfo->ulFreePublicMemory = stuDevInfo.FreeSpace;

	// ˽�д洢�����С
	pInfo->ulTotalPrivateMemory = 0;

	// ����˽�д洢�����С
	pInfo->ulFreePrivateMemory = 0;

	// Ӳ���汾
	pInfo->hardwareVersion.major = stuDevInfo.HWVersion.major;
	pInfo->hardwareVersion.minor = stuDevInfo.HWVersion.minor;

	// �̼��汾
	pInfo->firmwareVersion.major = stuDevInfo.FirmwareVersion.major;
	pInfo->firmwareVersion.minor = stuDevInfo.FirmwareVersion.minor;

	// ʱ��
	memset(pInfo->utcTime, 0, 16);

END:
	SKF_DisConnectDev(hDev);
	hDev = NULL;

	return rv;
}
/*
 *	��ȡĳ��Slot��֧�ֵ��㷨�б�
 */
CK_RV SLT_GetMechanismList(CK_SLOT_ID slotID, 
						   CK_MECHANISM_TYPE_PTR pMechanismList, 
						   CK_ULONG_PTR pulCount)
{
	return CKR_OK;
}
/*
 *	��ȡĳ��Slot���㷨��Ϣ
 */
CK_RV SLT_GetMechanismInfo(CK_SLOT_ID slotID, 
						   CK_MECHANISM_TYPE type, 
						   CK_MECHANISM_INFO_PTR pInfo)
{
	return CKR_OK;
}