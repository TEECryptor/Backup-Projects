#include "StdAfx.h"
#include "IeProtectModeAdapter.h"
#include <Shlobj.h>
#include <Commdlg.h>

typedef HRESULT (WINAPI *IEIsProtectedModeProcess)(BOOL*);
typedef HRESULT (WINAPI *IEGetWriteableFolderPath)(REFGUID, LPWSTR*);
typedef HRESULT (WINAPI *IEGetWriteableHKCU)(HKEY*);
typedef HRESULT (WINAPI *IEShowSaveFileDialog)(HWND, LPWSTR, LPWSTR, LPCWSTR, LPCWSTR, DWORD, DWORD, LPWSTR*, HANDLE);
typedef HRESULT (WINAPI *IESaveFile)(HANDLE, LPWSTR);

CIeProtectModeAdapter::CIeProtectModeAdapter(void)
{
}


CIeProtectModeAdapter::~CIeProtectModeAdapter(void)
{
}

RuntimeMode CIeProtectModeAdapter::GetRuntimeMode()
{
	RuntimeMode rtMode = RM_Normal;
	HMODULE hIeFrame = ::LoadLibrary(_T("ieframe.dll"));
	if (hIeFrame == NULL)
		return rtMode;

	IEIsProtectedModeProcess pIEIsProtectedModeProcess = (IEIsProtectedModeProcess)GetProcAddress(hIeFrame, "IEIsProtectedModeProcess");
	if (pIEIsProtectedModeProcess != NULL)
	{
		BOOL bProtected = FALSE;
		HRESULT hr = pIEIsProtectedModeProcess(&bProtected);
		if (hr == E_NOTIMPL)
			rtMode = RM_Normal;
		else
		{
			rtMode = bProtected? RM_IEPROTECT : RM_IENOTPROTECT;
		}
	}
	FreeLibrary(hIeFrame);
	return rtMode;
}

bool CIeProtectModeAdapter::GetWriteableFolder(LPTSTR pszFolder, DWORD dwMaxSize)
{
	if (pszFolder == NULL || dwMaxSize == 0)
	{
		SetLastError(E_INVALIDARG);
		return false;
	}

	if (GetRuntimeMode() != RM_IEPROTECT)
	{
		SetLastError(E_UNEXPECTED);
		return false;
	}

	bool bSucceed = false;
	HMODULE hIeFrame = ::LoadLibrary(_T("ieframe.dll"));
	if (hIeFrame == NULL)
		return false;

	LPWSTR pwszCacheDir = NULL;
	IEGetWriteableFolderPath pIEGetWriteableFolderPath = (IEGetWriteableFolderPath)GetProcAddress(hIeFrame, "IEGetWriteableFolderPath");
	if (pIEGetWriteableFolderPath != NULL)
	{
		HRESULT hr = pIEGetWriteableFolderPath(FOLDERID_InternetCache, &pwszCacheDir);
		if (SUCCEEDED(hr))
		{
			_tcscpy_s(pszFolder, dwMaxSize, pwszCacheDir);
			CoTaskMemFree(pwszCacheDir);
			bSucceed = true;
		}
	}
	
	FreeLibrary(hIeFrame);
	return bSucceed;
}

bool CIeProtectModeAdapter::GetWriteableRegKey(HKEY* phKey)
{
	if (phKey == NULL)
	{
		SetLastError(E_INVALIDARG);
		return false;
	}
	
	if (GetRuntimeMode() != RM_IEPROTECT)
	{
		SetLastError(E_UNEXPECTED);
		return false;
	}

	HMODULE hModule = ::LoadLibrary(_T("Ieframe.dll"));
	if (hModule == NULL)
		return false;

	bool bSucceed = false;
	IEGetWriteableHKCU pIEGetWriteableHKCU = (IEGetWriteableHKCU)GetProcAddress(hModule, "IEGetWriteableHKCU");
	if (pIEGetWriteableHKCU != NULL)
	{
		HRESULT hr = pIEGetWriteableHKCU(phKey);
		if (SUCCEEDED(hr))
			bSucceed = true;
	}

	FreeLibrary(hModule);
	return bSucceed;
}

bool CIeProtectModeAdapter::SaveFile(HWND hwndParent, LPCTSTR pszSourceFile, LPCTSTR pszFileFilter, LPCTSTR pszDefExt, LPTSTR pszDefFileName)
{
	if (GetRuntimeMode() != RM_IEPROTECT)
	{
		SetLastError(E_UNEXPECTED);
		return false;
	}

	HMODULE hModule = ::LoadLibrary(_T("Ieframe.dll"));
	if (hModule == NULL)
		return false;

	bool bSucceed = false;
	IEShowSaveFileDialog pIeShowSaveFileDialog = (IEShowSaveFileDialog)GetProcAddress(hModule, "IEShowSaveFileDialog");
	IESaveFile pIeSaveFile = (IESaveFile)GetProcAddress(hModule, "IESaveFile");
	if (pIeShowSaveFileDialog != NULL && pIeSaveFile != NULL)
	{
		HANDLE hState = NULL;
		LPTSTR szFileName = NULL;
		HRESULT hr = pIeShowSaveFileDialog(hwndParent, pszDefFileName, NULL, pszFileFilter, pszDefExt, 1, OFN_NONETWORKBUTTON | OFN_OVERWRITEPROMPT, &szFileName, &hState);
		if (hr == S_OK && szFileName != NULL)
		{
			TCHAR szSourceFile[MAX_PATH] = {0};
			_tcscpy_s(szSourceFile, pszSourceFile);
			pIeSaveFile(hState, szSourceFile);
			CoTaskMemFree(szFileName);
		}
	}
	return false;
}
