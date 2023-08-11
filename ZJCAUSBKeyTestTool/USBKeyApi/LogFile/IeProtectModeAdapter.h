#pragma once

#include <windows.h>
#include <tchar.h>

enum RuntimeMode
{
	RM_Normal		= 0,
	RM_IENOTPROTECT	= 1,
	RM_IEPROTECT	= 2,
};
class CIeProtectModeAdapter
{
public:
	CIeProtectModeAdapter(void);
	virtual ~CIeProtectModeAdapter(void);

	static RuntimeMode GetRuntimeMode();
	static bool GetWriteableFolder(LPTSTR pszFolder, DWORD dwMaxSize);
	static bool GetWriteableRegKey(HKEY* phKey);
	static bool SaveFile(HWND hwndParent, LPCTSTR pszSourceFile, LPCTSTR pszFileFilter, LPCTSTR pszDefExt, LPTSTR pszDefFileName = _T(""));
};

