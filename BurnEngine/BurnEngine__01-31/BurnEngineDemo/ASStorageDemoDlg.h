// ASStorageDemoDlg.h : header file
//--------------------------------------------------
#pragma once
//--------------------------------------------------
#include <string>
#include <iostream>
#include <fstream>
#include <list>

#include "BurnEngine.h"
/*
#include "ASDriveManager.h"
#include "AS_StorageDevice.h"
#include "AS_StorageError.h"
#include "AS_StringHelper.h"
#include "AS_Volume.h"
#include "ASDrives.h"*/
#include "afxcmn.h"
//--------------------------------------------------
using namespace std;
//--------------------------------------------------
typedef enum eBurnState
{
	BS_INIT		= 0,
	BS_ERASE	= 1,
	BS_WRITE	= 2,
	BS_CLOSE	= 3,
	BS_FINISH	= 4,
	BS_COUNT
};
//--------------------------------------------------
typedef struct tagBurnFileMap
{
#ifndef UNICODE
	string	strSrcFileName;
	string	strDesFileName;
#else
	wstring	strSrcFileName;
	wstring	strDesFileName;
#endif

}BurnFileMap, *LPBURNFILEMAP;
//--------------------------------------------------
typedef list<int>			IDArrayPtr;
typedef	list<BurnFileMap>	BurnFileArrayPtr;
//--------------------------------------------------
// CASStorageDemoDlg dialog
//--------------------------------------------------
class CASStorageDemoDlg : public CDialog
{
// Construction
public:
	CASStorageDemoDlg(CWnd* pParent = NULL);			// standard constructor

// Dialog Data
	enum { IDD = IDD_ASSTORAGEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//---------------------------------------------
public:
	//Burn files thread
	DWORD	ThreadBurnFiles();
	//Erase discs trhead
	DWORD	ThreadEraseDisc();
	//AS volume infomation callback function
	LONG	VolumeInfoCallback(DWORD dwInfoType, DWORD dwInfoData);
	//AS device infomation callback function
	LONG	DeviceInfoCallback(DWORD dwInfoData);
	//Progress information callbcak function
	LONG	ProgressInfoCallback(DWORD dwInfoType, DWORD dwInfoData, LPVOID lpInfoPtr);
	//---------------------------------------------
private:
	void	FillDevicesList();	
	//Burn files to disc
	LONG	BurnToDisc();
	//Burn files to image file
	LONG	BurnToImage();
	//Update current burn information
	void	UpdateBurnStateInfo();
	//Update the selected drive information
	void	UpdateSelDriveInfo(int iSelDriveID);
	//Get device capable type strings
	void    GetCapableTypeStr(DWORD dwDeviceCapable, LPTSTR lpszDeviceCapaleStr);
	//---------------------------------------------
private:
	HANDLE				m_hThread;
	CBurnEngine*		m_pBurnEngine;
	CDriveManager*		m_pDriveManager;
	//CASDriveManager*	m_pDriveManager;
	//CASDrives*			m_pTargetDrives;
	BOOL				m_bBurnToImage;
	eBurnState			m_eBurnState;		
	BurnFileArrayPtr	m_lstBurnFiles;
	IDArrayPtr			m_lstSelDrivesID;
	CProgressCtrl		m_ctlProgress;
	//---------------------------------------------
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnBurn();
	afx_msg void OnBnClickedCheckBurnimage();
	afx_msg void OnNMClickListDrives(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnErase();
};
//--------------------------------------------------
