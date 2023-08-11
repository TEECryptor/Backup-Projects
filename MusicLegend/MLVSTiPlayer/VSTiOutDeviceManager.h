//--------------------------------------------------
//	File Name:VSTiOutDeviceManager.h
//	Introduce:Class CVSTiOutDeviceManager definition file
//	Date:2010-09-2
//--------------------------------------------------
#ifndef	_VSTI_OUTDEVICE_MANAGER_H_
#define	_VSTI_OUTDEVICE_MANAGER_H_
//--------------------------------------------------
#include "WaveOutDevice.h"
#include <vector>
//--------------------------------------------------
using namespace std;
typedef vector<CWaveOutDevice*> PtrArraryWaveOutDevice;
//--------------------------------------------------
class CVSTiOutDeviceManager
{
public:
	CVSTiOutDeviceManager(void);
	~CVSTiOutDeviceManager(void);
	//----------------------------------------------
public:
	void	InitDeviceManager();
	DWORD	GetDeviceCnt();
	LPCTSTR	GetDeviceName(int iIndex);
	BOOL	SetCurrentDevice(int iIndex);
	//----------------------------------------------
private:
	CWaveOutDevice*		_GetWaveOutDeviceByIndex(int iIndex);
	CWaveOutDevice*		_GetWaveOutDeviceByName(LPCTSTR lpszFriendName);
	//----------------------------------------------
private:
	PtrArraryWaveOutDevice	m_arWaveOutDevices;
	//----------------------------------------------
};
//--------------------------------------------------
#endif	//_VSTI_OUTDEVICE_MANAGER_H_
//--------------------------------------------------