//--------------------------------------------------
//	File Name:VSTiOutDeviceManager.h
//	Introduce:Class CVSTiOutDeviceManager implement file
//	Date:2010-09-2
//--------------------------------------------------
#include "StdAfx.h"
#include ".\vstioutdevicemanager.h"
//--------------------------------------------------
CVSTiOutDeviceManager::CVSTiOutDeviceManager(void)
{
}
//--------------------------------------------------
CVSTiOutDeviceManager::~CVSTiOutDeviceManager(void)
{
}
//--------------------------------------------------


//--------------------------------------------------
void CVSTiOutDeviceManager::InitDeviceManager()
{
	// Wave out device count
	DWORD dwWaveOutDevCnt = waveOutGetNumDevs();
	for (DWORD dwIndex = 0; dwIndex < dwWaveOutDevCnt; dwIndex++)
	{
		WAVEOUTCAPS caps;
		MMRESULT mmhr = waveOutGetDevCaps(0, &caps, sizeof(caps));
		if (MMSYSERR_NOERROR == mmhr)
		{
			CWaveOutDevice* pDevice = new CWaveOutDevice();
			pDevice->SetSystemIndex(dwIndex);
			pDevice->SetFriendName(caps.szPname);
			//
			m_arWaveOutDevices.push_back(pDevice);
		}
	}

	// Add "Wave Mapper" in any time
	CWaveOutDevice* pMapperDevice = new CWaveOutDevice();
	pMapperDevice->SetSystemIndex(-1);
	pMapperDevice->SetFriendName(_T("Wave Mapper"));
	m_arWaveOutDevices.push_back(pMapperDevice);
}
//--------------------------------------------------
DWORD CVSTiOutDeviceManager::GetDeviceCnt()
{
	DWORD dwDeviceCnt = (DWORD)m_arWaveOutDevices.size();
	return dwDeviceCnt;
}
//--------------------------------------------------
LPCTSTR CVSTiOutDeviceManager::GetDeviceName(int iIndex)
{
	LPCTSTR lpDevName = NULL;
	CWaveOutDevice* pDevcie = NULL;
	pDevcie = _GetWaveOutDeviceByIndex(iIndex);
	if (NULL != pDevcie)
	{
		lpDevName = pDevcie->GetFriendName();
	}
	return lpDevName;
}
//--------------------------------------------------
BOOL CVSTiOutDeviceManager::SetCurrentDevice(int iIndex)
{
	return FALSE;
}
//--------------------------------------------------



//--------------------------------------------------
CWaveOutDevice* CVSTiOutDeviceManager::_GetWaveOutDeviceByIndex(int iIndex)
{
	int iCurIndex = 0;
	CWaveOutDevice* pDevice = NULL;
	for(PtrArraryWaveOutDevice::iterator it = m_arWaveOutDevices.begin();
		it != m_arWaveOutDevices.end();
		it++, iCurIndex++)
	{
		if (iCurIndex == iIndex)
		{
			pDevice = *it;
			break;
		}
	}

	return pDevice;
}
//--------------------------------------------------
CWaveOutDevice* CVSTiOutDeviceManager::_GetWaveOutDeviceByName(LPCTSTR lpszFriendName)
{
	CWaveOutDevice* pDevice = NULL;
	for(PtrArraryWaveOutDevice::iterator it = m_arWaveOutDevices.begin();
		it != m_arWaveOutDevices.end();
		it++)
	{
		LPCTSTR lpszDevName = (*it)->GetFriendName();
		if (_tcsicmp(lpszDevName, lpszFriendName) == 0)
		{
			pDevice = *it;
			break;
		}
	}

	return pDevice;
}
//--------------------------------------------------