#include "StdAfx.h"
#include "Pkcs11SDKTester.h"
#include "Defs.h"
#include "../../Common/SynoConvert.h"
#include <atlconv.h>

const unsigned char trueValue = 1, falseValue = 0;
const unsigned ckkRsa = CKK_RSA;
const unsigned ckoPubKey = CKO_PUBLIC_KEY;
const unsigned ckoPriKey = CKO_PRIVATE_KEY;
const unsigned ckoSecretKey = CKO_SECRET_KEY;
const unsigned ckoCertificate = CKO_CERTIFICATE;
const unsigned ckoDataFile = CKO_DATA;
const unsigned ckcX509 = CKC_X_509;


CPkcs11SDKTester::CPkcs11SDKTester(void)
 : m_hInstance(NULL)
 , m_ulSlotIDCnt(0)
 , m_pSlotIDList(NULL)
 , m_pFunctionList(NULL)
 , m_pMsgCallbackFunc(NULL)
 , m_pCallbackParam(NULL)
{
}

CPkcs11SDKTester::~CPkcs11SDKTester(void)
{
	if (m_pSlotIDList)
	{
		delete []m_pSlotIDList;
		m_pSlotIDList = NULL;
	}
}

// 加载SDK DLL文件
ULONG CPkcs11SDKTester::LoadDll(LPCTSTR lpcsCspOrFile, 
								FuncMsgCallback pMsgFunc, 
								LPVOID lpParam)
{
	ULONG ulRes = 0;

	//	参数检查
	if (!lpcsCspOrFile || _tcslen(lpcsCspOrFile) == 0)
	{
		USBKey_LogErr("lpcsDll is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pMsgFunc)
	{
		USBKey_LogErr("pMsgFunc is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (m_hInstance)
	{
		UnloadDll();
	}

	m_pMsgCallbackFunc = pMsgFunc;
	m_pCallbackParam = lpParam;
	g_ErrMsgCallback = pMsgFunc;
	g_pCallbackParam = lpParam;

	//	加载	PKCS11接口SDK
	m_hInstance = LoadLibrary(lpcsCspOrFile);	
	if (NULL == m_hInstance)
	{
		ulRes = GetLastError();
		USBKey_LogErr("LoadLibrary() failed! dwRes = 0x%x", ulRes);
		return ulRes;
	}

	//	加载PKCS11函数	
	CK_C_GetFunctionList pCGetFunctionList = (CK_C_GetFunctionList)::GetProcAddress(m_hInstance, "C_GetFunctionList");
	if (NULL == pCGetFunctionList)
	{
		USBKey_LogErr("Interface C_GetFunctionList() is NULL!");
		return USBKEY_ERR_NO_INTERFACE;
	}
	ulRes = pCGetFunctionList(&m_pFunctionList);
	if (CKR_OK != ulRes || !m_pFunctionList)
	{
		USBKey_LogErr("C_GetFunctionList() failed! dwRes = 0x%x", ulRes);
		return USBKEY_ERR_NO_INTERFACE;
	}
	
	//	初始化
	ulRes = m_pFunctionList->C_Initialize(NULL);
	if (CKR_OK != ulRes)
	{
		USBKey_LogErr("C_Initialize() failed! dwRes = 0x%x", ulRes);
		return ulRes;
	}	

	return ulRes;
}

// 卸载DLL文件
ULONG CPkcs11SDKTester::UnloadDll()
{
	ULONG ulRes = USBKEY_ERR_OK;
	
	m_ulSlotIDCnt = 0;
	if (m_pSlotIDList)
	{
		delete []m_pSlotIDList;
		m_pSlotIDList = NULL;
	}
	if (m_pFunctionList)
	{
		m_pFunctionList->C_Finalize(NULL);
	}
	if (m_hInstance)
	{
		FreeLibrary(m_hInstance);
		m_hInstance = NULL;
	}

	return ulRes;
}

// 枚举设备
ULONG CPkcs11SDKTester::EnumDev(LPTSTR lpsDevName, 
								ULONG *pulSize)
{
	CK_RV rv = 0;
	CK_ULONG ulIndex = 0;
	CK_ULONG ulNameSize = 0;
	CK_TOKEN_INFO tokenInfo = {0};
	TCHAR tcTokenLabel[1024] = {0};
	LPTSTR lpcsLabel = NULL;

	USES_CONVERSION;

	if (!m_pFunctionList)
	{
		USBKey_LogErr("m_pFunctionList is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	if (!pulSize)
	{
		USBKey_LogErr("pulSize is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	rv = m_pFunctionList->C_GetSlotList(CK_TRUE, NULL, &m_ulSlotIDCnt);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GetSlotList() failed! rv = 0x%x", rv);
		return rv;
	}

	if (m_ulSlotIDCnt == 0)
	{
		USBKey_LogErr("C_GetSlotList() return count = 0!");
		return CKR_OK;
	}

	if (m_pSlotIDList) delete []m_pSlotIDList;
	m_pSlotIDList = new CK_SLOT_ID[m_ulSlotIDCnt];
	rv = m_pFunctionList->C_GetSlotList(CK_TRUE, m_pSlotIDList, &m_ulSlotIDCnt);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GetSlotList() failed! rv = 0x%x", rv);
		delete []m_pSlotIDList;
		m_pSlotIDList = NULL;
		return rv;
	}

	ulNameSize = 0;
	for (CK_ULONG ulIndex = 0; ulIndex < m_ulSlotIDCnt; ulIndex++)
	{
		rv = m_pFunctionList->C_GetTokenInfo(m_pSlotIDList[ulIndex], &tokenInfo);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_GetTokenInfo() failed when slot id = %d! rv = 0x%x", m_pSlotIDList[ulIndex], rv);
			continue;
		}
#ifdef UNICODE
		lpcsLabel = ConvertUTF82UNICODE((char*)tokenInfo.label);
#else
		lpcsLabel = ConvertUTF82ASCII((char*)tokenInfo.label);
#endif	//UNICODE
		ULONG uPos = ulNameSize*sizeof(TCHAR);
		memcpy((LPBYTE)tcTokenLabel + uPos, (LPBYTE)lpcsLabel, _tcslen(lpcsLabel)*sizeof(TCHAR));
		ulNameSize += _tcslen(lpcsLabel);
		memset((LPBYTE)tcTokenLabel + ulNameSize*sizeof(TCHAR), 0, sizeof(TCHAR));
		ulNameSize += 1;
		free(lpcsLabel);
	}
	memset((LPBYTE)tcTokenLabel + ulNameSize*sizeof(TCHAR), 0, sizeof(TCHAR));
	ulNameSize += 1;

	if (!lpsDevName)
	{
		*pulSize = ulNameSize;
		//USBKey_LogErr("Return requested size:0x%x", ulNameSize);
		return USBKEY_ERR_OK;
	}
	memcpy_s((LPBYTE)lpsDevName, sizeof(TCHAR)*(*pulSize), (LPBYTE)tcTokenLabel, sizeof(TCHAR)*ulNameSize);
	
	return USBKEY_ERR_OK;
}

// 获取设备信息
ULONG CPkcs11SDKTester::GetDevInfo(LPCTSTR lpcsDevName)
{
	CK_RV rv = 0;
	CK_ULONG ulIndex = 0;
	CK_SLOT_ID selSlotID = 0;
	CK_SLOT_INFO slotInfo = {0};
	CK_TOKEN_INFO tokenInfo = {0};
	LPCTSTR lpcsLabel = NULL;
	TCHAR tcInfo[512] = {0};
	
	USES_CONVERSION;

	if (!lpcsDevName || _tcslen(lpcsDevName) == 0)
	{
		USBKey_LogErr("lpcsDevName is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!m_pSlotIDList || m_ulSlotIDCnt == 0)
	{
		USBKey_LogErr("m_pSlotIDList is NULL or m_ulSlotIDCnt is 0!");
		return USBKEY_ERR_NO_DEVICE;
	}
	
	// 根据DevName查找到Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}

	// 获取Slot Info
	rv = m_pFunctionList->C_GetSlotInfo(selSlotID, &slotInfo);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GetSlotInfo() failed when slot id = %d! rv = 0x%x", selSlotID, rv);
		return rv;
	}

	// 获取Token Info
	rv = m_pFunctionList->C_GetTokenInfo(selSlotID, &tokenInfo);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GetTokenInfo() failed when slot id = %d! rv = 0x%x", selSlotID, rv);
		return rv;
	}

	// Slot Info
	m_pMsgCallbackFunc(_T("Slot Info:"), m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  slotDescription:%s"), A2W((char*)slotInfo.slotDescription));
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  manufacturerID:%s"), A2W((char*)slotInfo.manufacturerID));
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  flags:0x%x"), slotInfo.flags);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  hardwareVersion:%d.%d"), slotInfo.hardwareVersion.major, slotInfo.hardwareVersion.minor);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  firmwareVersion:%d.%d"), slotInfo.firmwareVersion.major, slotInfo.firmwareVersion.minor);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
		
	// Token Info
	m_pMsgCallbackFunc(_T("Token Info:"), m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  label:%s"), A2W((char*)tokenInfo.label));
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  manufacturerID:%s"), A2W((char*)tokenInfo.manufacturerID));
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  model:%s"), A2W((char*)tokenInfo.model));
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  serialNumber:%s"), A2W((char*)tokenInfo.serialNumber));
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  flags:0x%x"), tokenInfo.flags);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulMaxSessionCount:%d"), tokenInfo.ulMaxSessionCount);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulSessionCount:%d"), tokenInfo.ulSessionCount);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulMaxRwSessionCount:%d"), tokenInfo.ulMaxRwSessionCount);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulRwSessionCount:%d"), tokenInfo.ulRwSessionCount);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulMaxPinLen:%d"), tokenInfo.ulMaxPinLen);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulMinPinLen:%d"), tokenInfo.ulMinPinLen);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulTotalPublicMemory:%d"), tokenInfo.ulTotalPublicMemory);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulFreePublicMemory:%d"), tokenInfo.ulFreePublicMemory);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulTotalPrivateMemory:%d"), tokenInfo.ulTotalPrivateMemory);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  ulFreePrivateMemory:%d"), tokenInfo.ulFreePrivateMemory);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512,  _T("  hardwareVersion:%d.%d"), tokenInfo.hardwareVersion.major, tokenInfo.hardwareVersion.minor);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  firmwareVersion:%d.%d"), tokenInfo.firmwareVersion.major, tokenInfo.firmwareVersion.minor);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	//
	_stprintf_s(tcInfo, 512, _T("  utcTime:%s"), A2W((char*)tokenInfo.utcTime));
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);

	return USBKEY_ERR_OK;
}

// 初始化设备
ULONG CPkcs11SDKTester::Initialize(LPCTSTR lpcsDevName, 
								   LPCTSTR lpcsLabel, 
								   LPCTSTR lpcsAppName,   
								   LPCTSTR lpcsSOPIN, 
								   ULONG ulSOPINRetryTime, 
								   LPCTSTR lpcsUserPIN, 
								   ULONG ulUserPINRetryTime)
{
	CK_RV rv = CKR_OK;
	CK_SLOT_ID selSlotID = 0;
	CK_UTF8CHAR_PTR utf8SOPin = NULL;
	CK_UTF8CHAR_PTR utf8UserPin = NULL;
	CK_UTF8CHAR_PTR utf8Label = NULL;
	CK_SESSION_HANDLE hSession = NULL;
	
	if (!lpcsDevName || _tcslen(lpcsDevName) == 0)
	{
		USBKey_LogErr("lpcsDevName is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpcsLabel || _tcslen(lpcsLabel) == 0)
	{
		USBKey_LogErr("lpcsLabel is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpcsAppName || _tcslen(lpcsAppName) == 0)
	{
		USBKey_LogErr("lpcsAppName is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpcsSOPIN || _tcslen(lpcsSOPIN) == 0)
	{
		USBKey_LogErr("lpcsSOPIN is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpcsUserPIN || _tcslen(lpcsUserPIN) == 0)
	{
		USBKey_LogErr("lpcsUserPIN is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!m_pSlotIDList || m_ulSlotIDCnt == 0)
	{
		USBKey_LogErr("m_pSlotIDList is NULL or m_ulSlotIDCnt is 0!");
		return USBKEY_ERR_NO_DEVICE;
	}
	
	// 根据DevName查找到Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}

	// 关掉当前所有会话
	rv = m_pFunctionList->C_CloseAllSessions(selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CloseAllSessions() failed! rv = 0x%x", rv);
		return rv;
	}

	// 初始化Token
#ifdef UNICODE
	utf8SOPin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsSOPIN);
	utf8Label = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsLabel);
#else
	utf8SOPin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsSOPIN);
	utf8Label = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsLabel);
#endif	//UNICODE
	rv = m_pFunctionList->C_InitToken(selSlotID, utf8SOPin, strlen((CHAR*)utf8SOPin), utf8Label);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_InitToken() failed! rv = 0x%x", rv);
		goto FREE_MEMORY;
	}
	
	// 打开会话
	rv = m_pFunctionList->C_OpenSession(selSlotID, 
		CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &hSession);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_OpenSession() failed! rv=0x%x", rv);
		goto FREE_MEMORY;
	}

	// 校验SO PIN码
	rv = m_pFunctionList->C_Login(hSession, CKU_SO, utf8SOPin, strlen((CHAR*)utf8SOPin));
	if (CKR_OK != rv && CKR_USER_ALREADY_LOGGED_IN != rv)
	{
		USBKey_LogErr("C_Login() failed! rv=0x%x", rv);
		goto FREE_MEMORY;
	}

	// 设置初始User PIN
#ifdef UNICODE
	utf8UserPin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsUserPIN);
#else
	utf8UserPin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsUserPIN);
#endif	//UNICODE
	rv = m_pFunctionList->C_InitPIN(hSession, utf8UserPin, strlen((CHAR*)utf8UserPin));
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_InitToken() failed! rv = 0x%x", rv);
		goto FREE_MEMORY;
	}
		
FREE_MEMORY:
	if (utf8SOPin)
	{
		free(utf8SOPin);
		utf8SOPin = NULL;
	}
	if (utf8UserPin)
	{
		free(utf8UserPin);
		utf8UserPin = NULL;
	}
	if (utf8Label)
	{
		free(utf8Label);
		utf8Label = NULL;
	}

	return rv;
}

// 设置标签(不支持)
ULONG CPkcs11SDKTester::SetLabel(LPCTSTR lpcsDevName, 
								 LPCTSTR lpcsLabel)
{	

	return USBKEY_ERR_NOT_SUPPORT;
}

// 设备认证(不支持)
ULONG CPkcs11SDKTester::DevAuth(LPCTSTR lpcsDevName, 
								LPBYTE lpAuthKey, 
								ULONG ulKeyLen)
{
	return USBKEY_ERR_NOT_SUPPORT;
}

// 修改设备认证Key(不支持)
ULONG CPkcs11SDKTester::ChangeAuthKey(LPCTSTR lpcsDevName, 
									  LPBYTE lpNewAuthKey, 
									  ULONG ulKeyLen)
{
	return USBKEY_ERR_NOT_SUPPORT;
}

// 打开会话
ULONG CPkcs11SDKTester::OpenSession(LPCTSTR lpcsDevName, ULONG *phSession)
{
	CK_RV rv = CKR_OK;
	CK_SLOT_ID selSlotID = 0;
	CK_SESSION_HANDLE hNewSession = NULL;
	
	if (!lpcsDevName || _tcslen(lpcsDevName) == 0)
	{
		USBKey_LogErr("lpcsDevName is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!phSession)
	{
		USBKey_LogErr("phSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!m_pSlotIDList || m_ulSlotIDCnt == 0)
	{
		USBKey_LogErr("m_pSlotIDList is NULL or m_ulSlotIDCnt is 0!");
		return USBKEY_ERR_NO_DEVICE;
	}

	// 根据DevName查找到Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}
	
	// 打开会话
	rv = m_pFunctionList->C_OpenSession(selSlotID, CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &hNewSession);
	if (CKR_SESSION_EXISTS == rv)
	{
		rv = m_pFunctionList->C_CloseAllSessions(selSlotID);
		rv |= m_pFunctionList->C_OpenSession(selSlotID, CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &hNewSession);
	}
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_OpenSession() failed! rv = 0x%x", rv);
		return rv;
	}

	*phSession = (ULONG)hNewSession;

	return rv;
}

// 获取会话信息
ULONG CPkcs11SDKTester::GetSessionInfo(ULONG hSession)
{
	CK_RV rv = CKR_OK;
	CK_SESSION_INFO sessionInfo = {0};
	TCHAR tcInfo[512] = {0};
	
	if (!m_pSlotIDList || m_ulSlotIDCnt == 0)
	{
		USBKey_LogErr("m_pSlotIDList is NULL or m_ulSlotIDCnt is 0!");
		return USBKEY_ERR_NO_DEVICE;
	}

	// 当前没有打开的会话
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	
	// 关闭会话
	rv = m_pFunctionList->C_GetSessionInfo((CK_SESSION_HANDLE)hSession, &sessionInfo);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CloseSession() failed! rv = 0x%x", rv);
		return rv;
	}

	// 输出信息
	m_pMsgCallbackFunc(_T("Session Info:"), m_pCallbackParam);
	_stprintf_s(tcInfo, 512, _T("  slotID:0x%x"), sessionInfo.slotID);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	_stprintf_s(tcInfo, 512, _T("  flags:0x%x"), sessionInfo.flags);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	_stprintf_s(tcInfo, 512, _T("  state:0x%x"), sessionInfo.state);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);
	_stprintf_s(tcInfo, 512, _T("  ulDeviceError:0x%x"), sessionInfo.ulDeviceError);
	m_pMsgCallbackFunc(tcInfo, m_pCallbackParam);

	return rv;
}

// 关闭会话
ULONG CPkcs11SDKTester::CloseSession(ULONG hSession)
{
	CK_RV rv = CKR_OK;

	// 当前没有打开的会话
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	// 关闭会话
	rv = m_pFunctionList->C_CloseSession((CK_SESSION_HANDLE)hSession);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CloseSession() failed! rv = 0x%x", rv);
		return rv;
	}

	hSession = NULL;

	return rv;
}

// 登录会话
ULONG CPkcs11SDKTester::LoginSession(ULONG hSession, 
									 ULONG ulUserType, 
									 LPCTSTR lpcsPIN, 
									 ULONG* pulLeftTimes)
{
	CK_RV rv = CKR_OK;
	CK_USER_TYPE userType = 0;
	CK_UTF8CHAR_PTR utf8Pin = NULL;
	
	// 当前没有打开的会话
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	// 用户类型
	switch(ulUserType)
	{
	case USER_TYPE_SO:
		userType = CKU_SO;
		break;
	case USER_TYPE_USER:
		userType = CKU_USER;
		break;
	default:
		USBKey_LogErr("ulUserType is wrong! ulUserType = 0x%x", ulUserType);
		return USBKEY_ERR_INVALIDPARAM;
	}
	
#ifdef UNICODE
	utf8Pin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsPIN);
#else
	utf8Pin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsPIN);
#endif	//UNICODE

	// 登录
	rv = m_pFunctionList->C_Login((CK_SESSION_HANDLE)hSession, userType, utf8Pin, strlen((char*)utf8Pin));
	if (CKR_OK != rv)
	{
		if (CKR_USER_ALREADY_LOGGED_IN == rv)
		{
			USBKey_LogErr("User has logged in!");
			rv = CKR_OK;
		}
		else
		{
			USBKey_LogErr("C_Login() failed! rv = 0x%x", rv);
			goto FREE_MEMORY;
		}
	}

FREE_MEMORY:
	if (utf8Pin)
	{
		free(utf8Pin);
		utf8Pin = NULL;
	}

	return rv;
}

// 登出会话
ULONG CPkcs11SDKTester::LogOutSession(ULONG hSession)
{
	CK_RV rv = CKR_OK;
	
	// 当前没有打开的会话
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
			
	// 登录
	rv = m_pFunctionList->C_Logout((CK_SESSION_HANDLE)hSession);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Logout() failed! rv = 0x%x", rv);
		return rv;
	}

	return rv;
}

// 修改PIN码
ULONG CPkcs11SDKTester::ChangePin(ULONG hSession, 
								  ULONG ulUserType, 
								  LPCTSTR lpcsOldPIN, 
								  LPCTSTR lpcsNewPIN, 
								  ULONG* pulLeftTimes)
{
	CK_RV rv = CKR_OK;
	CK_USER_TYPE userType = 0;
	CK_UTF8CHAR_PTR utf8OldPin = NULL;
	CK_UTF8CHAR_PTR utf8NewPin = NULL;
	
	// 当前没有打开的会话
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
			
	// 用户类型
	switch(ulUserType)
	{
	case USER_TYPE_SO:
		userType = CKU_SO;
		break;
	case USER_TYPE_USER:
		userType = CKU_USER;
		break;
	default:
		USBKey_LogErr("ulUserType is wrong! ulUserType = 0x%x", ulUserType);
		return USBKEY_ERR_INVALIDPARAM;
	}
	
#ifdef UNICODE
	utf8OldPin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsOldPIN);
	utf8NewPin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsNewPIN);
#else
	utf8OldPin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsOldPIN);
	utf8NewPin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsNewPIN);
#endif	//UNICODE

	// 修改PIN码
	rv = m_pFunctionList->C_SetPIN((CK_SESSION_HANDLE)hSession, utf8OldPin, strlen((char*)utf8OldPin), utf8NewPin, strlen((char*)utf8NewPin));
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SetPIN() failed! rv = 0x%x", rv);
		goto FREE_MEMORY;
	}

FREE_MEMORY:
	if (utf8OldPin)
	{
		free(utf8OldPin);
		utf8OldPin = NULL;
	}
	if (utf8NewPin)
	{
		free(utf8NewPin);
		utf8NewPin = NULL;
	}

	return rv;
}

// 解锁用户PIN
ULONG CPkcs11SDKTester::UnlockPin(ULONG hSession, 
								  LPCTSTR lpcsSOPIN, 
								  LPCTSTR lpcsNewUserPIN, 
								  ULONG* pulLeftTimes)
{
	CK_RV rv = CKR_OK;
	CK_UTF8CHAR_PTR utf8SOPin = NULL;
	CK_UTF8CHAR_PTR utf8NewPin = NULL;
	
	// 当前没有打开的会话
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

#ifdef UNICODE
	utf8SOPin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsSOPIN);
	utf8NewPin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsNewUserPIN);
#else
	utf8SOPin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsSOPIN);
	utf8NewPin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsNewUserPIN);
#endif	//UNICODE

	// SO登录
	rv = m_pFunctionList->C_Login((CK_SESSION_HANDLE)hSession, CKU_SO, utf8SOPin, strlen((char*)utf8SOPin));
	if (CKR_OK != rv && CKR_USER_ALREADY_LOGGED_IN != rv)
	{
		USBKey_LogErr("C_Login() failed! rv = 0x%x", rv);
		goto FREE_MEMORY;
	}

	// 设置User初始PIN
	rv = m_pFunctionList->C_InitPIN((CK_SESSION_HANDLE)hSession, utf8NewPin, strlen((char*)utf8NewPin));
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_InitPIN() failed! rv = 0x%x", rv);
		goto FREE_MEMORY;
	}
	
FREE_MEMORY:
	if (utf8SOPin)
	{
		free(utf8SOPin);
		utf8SOPin = NULL;
	}
	if (utf8NewPin)
	{
		free(utf8NewPin);
		utf8NewPin = NULL;
	}

	return rv;
}

// 枚举文件
ULONG CPkcs11SDKTester::EnumFiles(ULONG hSession, 
								  LPTSTR lpFilesName, 
								  ULONG* pulLen)
{
	CK_RV rv = CKR_OK;
	CK_ULONG findCount = 0;
	CK_ULONG nameListLen = 0;
	CK_BYTE fileName[256] = {0};
	CK_BYTE nameList[2048] = {0};
	CK_OBJECT_HANDLE hFileObj = 0;	
	CK_ATTRIBUTE file_Attrs[ 3 ] = {
		{ CKA_CLASS, (unsigned *)&ckoDataFile, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 }
	};
	CK_ATTRIBUTE attr = {CKA_ID, fileName, 256};
	LPSTR lpFileName = NULL;

	USES_CONVERSION;

	// 当前没有打开的会话
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == pulLen)
	{
		USBKey_LogErr("pulLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	// 查找Data File对象
	rv = m_pFunctionList->C_FindObjectsInit(hSession, file_Attrs, 3);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_FindObjectsInit() failed! rv = 0x%x", rv);
		return rv;
	}

	// 分别获取每个Data File对象的文件名
	nameListLen = 0;
	while (CKR_OK == m_pFunctionList->C_FindObjects(hSession, &hFileObj, 1, &findCount) && (1 == findCount))
	{		
		memset(fileName, 0, 256);
		rv = m_pFunctionList->C_GetAttributeValue(hSession, hFileObj, &attr, 1);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_GetAttributeValue() failed！ ulRes = 0x%x", rv);
			continue;
		}

		memcpy(nameList + nameListLen, fileName, attr.ulValueLen);
		nameListLen += (attr.ulValueLen + 1);
	}
	nameListLen++;
	
	// 查找结束
	m_pFunctionList->C_FindObjectsFinal(hSession);

	// 返回长度
	if (NULL == lpFilesName)
	{
		*pulLen = nameListLen;
		return USBKEY_ERR_OK;
	}
	if (*pulLen < nameListLen)
	{
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// 返回内容
	nameListLen = 0;
	lpFileName = (LPSTR)nameList;
	while (lpFileName && strlen(lpFileName) > 0)
	{
		LPTSTR lptcFile = NULL;
		LPTSTR lpCurPos = lpFilesName + nameListLen;
#ifdef UNICODE
		lptcFile = A2W((LPSTR)lpFileName);
#else
		lptcFile = (LPSTR)lpFileName;
#endif	//UNICODE
		_tcscpy(lpCurPos, lptcFile);
		nameListLen += _tcslen(lptcFile) + 1;

		lpFileName += (strlen(lpFileName) + 1);
	}
	lpFilesName[nameListLen++] = _T('\0');
	*pulLen = nameListLen;

	return USBKEY_ERR_OK;
}

// 创建文件
ULONG CPkcs11SDKTester::CreateFile(ULONG hSession, 
								   LPTSTR lpFilesName, 
								   ULONG ulSize)
{
	CK_RV rv = CKR_OK;
	CK_ULONG nameLen = 0;
	CK_BYTE fileName[256] = {0};
	CK_OBJECT_HANDLE hFileObj = 0;
	CK_ATTRIBUTE file_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned char *)&ckoDataFile, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
		{ CKA_ID, NULL, 0 },
		{ CKA_VALUE_LEN, &ulSize, 4 }
	};

	USES_CONVERSION;

	// 检查参数
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == lpFilesName || _tcslen(lpFilesName) == 0)
	{
		USBKey_LogErr("pulLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (0 == ulSize)
	{
		USBKey_LogErr("ulSize is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}

#ifdef UNICODE
	strcpy_s((CHAR*)fileName, 256, W2A(lpFilesName));
#else
	strcpy_s((CHAR*)fileName, 256, lpFilesName);
#endif	//UNICODE
	nameLen = strlen((CHAR*)fileName);

	file_Attrs[3].pValue = fileName;
	file_Attrs[3].ulValueLen = nameLen;
	rv = m_pFunctionList->C_CreateObject(hSession, file_Attrs, 5, &hFileObj);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CreateObject() failed！ ulRes = 0x%x", rv);
		return rv;
	}

	return rv;
}

// 删除文件
ULONG CPkcs11SDKTester::DeleteFile(ULONG hSession, 
								   LPTSTR lpFilesName)
{
	CK_RV rv = CKR_OK;
	CK_BYTE fileName[256] = {0};
	CK_OBJECT_HANDLE hFileObj = 0;

	USES_CONVERSION;

	// 检查参数
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == lpFilesName || _tcslen(lpFilesName) == 0)
	{
		USBKey_LogErr("pulLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
#ifdef UNICODE
	strcpy_s((CHAR*)fileName, 256, W2A(lpFilesName));
#else
	strcpy_s((CHAR*)fileName, 256, lpFilesName);
#endif	//UNICODE

	// 查找文件对象
	hFileObj = _FindDataFileObj(hSession, (LPCSTR)fileName);
	if (NULL == hFileObj)
	{
		USBKey_LogErr("File %s is not found!", fileName);
		return USBKEY_ERR_FAILED; 
	}

	// 删除文件对象
	rv = m_pFunctionList->C_DestroyObject(hSession, hFileObj);
	if (CKR_OK == rv)
	{
		USBKey_LogErr("C_DestroyObject() failed! rv = 0x%x", rv);
		return rv; 
	}

	return rv;
}

// 写文件
ULONG CPkcs11SDKTester::WriteFile(ULONG hSession, 
								  LPTSTR lpFilesName, 
								  LPBYTE lpData, 
								  ULONG ulLen)
{
	CK_RV rv = CKR_OK;
	CK_BYTE fileName[256] = {0};
	CK_OBJECT_HANDLE hFileObj = 0;
	CK_ATTRIBUTE file_Attrs[ 1 ] = 
	{ 
		{CKA_VALUE, lpData, ulLen }
	};

	USES_CONVERSION;

	// 检查参数
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == lpFilesName || _tcslen(lpFilesName) == 0)
	{
		USBKey_LogErr("pulLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == lpData || 0 == ulLen)
	{
		USBKey_LogErr("lpData is NULL or ulLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
#ifdef UNICODE
	strcpy_s((CHAR*)fileName, 256, W2A(lpFilesName));
#else
	strcpy_s((CHAR*)fileName, 256, lpFilesName);
#endif	//UNICODE

	// 查找文件对象
	hFileObj = _FindDataFileObj(hSession, (LPCSTR)fileName);
	if (NULL == hFileObj)
	{
		USBKey_LogErr("File %s is not found!", fileName);
		return USBKEY_ERR_FAILED; 
	}

	// 删除文件对象
	rv = m_pFunctionList->C_SetAttributeValue(hSession, hFileObj, file_Attrs, 1);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SetAttributeValue() failed! rv = 0x%x", rv);
		return rv; 
	}

	return rv;
}

// 读文件
ULONG CPkcs11SDKTester::ReadFile(ULONG hSession, 
								 LPTSTR lpFilesName, 
								 LPBYTE lpData, 
								 ULONG* pulLen)
{
	CK_RV rv = CKR_OK;
	CK_ULONG fileSize = 0;
	CK_BYTE fileName[256] = {0};
	CK_OBJECT_HANDLE hFileObj = 0;	
	CK_ATTRIBUTE fileSize_Attrs[ 1 ] = 
	{
		{CKA_VALUE, NULL, 0}
	};		
	CK_ATTRIBUTE fileData_Attrs[ 1 ] = 
	{
		{CKA_VALUE, lpData, 0 }
	};

	USES_CONVERSION;

	// 检查参数
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == lpFilesName || _tcslen(lpFilesName) == 0)
	{
		USBKey_LogErr("pulLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == pulLen)
	{
		USBKey_LogErr("pulLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
#ifdef UNICODE
	strcpy_s((CHAR*)fileName, 256, W2A(lpFilesName));
#else
	strcpy_s((CHAR*)fileName, 256, lpFilesName);
#endif	//UNICODE

	// 查找文件对象
	hFileObj = _FindDataFileObj(hSession, (LPCSTR)fileName);
	if (NULL == hFileObj)
	{
		USBKey_LogErr("File %s is not found!", fileName);
		return USBKEY_ERR_FAILED; 
	}

	// 获取文件大小
	rv = m_pFunctionList->C_GetAttributeValue(hSession, hFileObj, fileSize_Attrs, 1);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SetAttributeValue() failed! rv = 0x%x", rv);
		return rv; 
	}

	// 返回文件长度
	if (NULL == lpData)
	{
		*pulLen = fileSize_Attrs[0].ulValueLen;
		return USBKEY_ERR_OK;
	}
	if (*pulLen < fileSize_Attrs[0].ulValueLen)
	{
		USBKey_LogErr("Buffer is too small! Requested length: 0x%x", fileSize_Attrs[0].ulValueLen);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// 获取文件内容
	fileData_Attrs[0].ulValueLen = *pulLen;
	rv = m_pFunctionList->C_GetAttributeValue(hSession, hFileObj, fileData_Attrs, 1);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SetAttributeValue() failed! rv = 0x%x", rv);
		return rv; 
	}
	*pulLen = fileData_Attrs[0].ulValueLen;

	return rv;
}

// 导入证书
ULONG CPkcs11SDKTester::ImportCert(ULONG hSession, 
								   LPCTSTR lpContainer,
								   ULONG ulCertAlg, 
								   ULONG ulCertUsage, 
								   LPBYTE lpCert, 
								   ULONG ulLen)
{
	CK_RV rv = 0;
	CK_BYTE container[256] = {0};
	CK_OBJECT_HANDLE hCertObj = NULL;
	CK_ATTRIBUTE attrs[ 11 ] = {
		{ CKA_CLASS, (unsigned char *)&ckoCertificate, sizeof(unsigned) },
		{ CKA_CERTIFICATE_TYPE, (unsigned char *)&ckcX509, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_KEY_TYPE, &ulCertAlg, sizeof(unsigned) },
		{ CKA_SIGN, (USAGE_TYPE_SIGN == ulCertAlg)?(unsigned char *)&trueValue:(unsigned char *)&falseValue, 1 },
		{ CKA_VERIFY, (USAGE_TYPE_SIGN == ulCertAlg)?(unsigned char *)&trueValue:(unsigned char *)&falseValue, 1 },
		{ CKA_ENCRYPT, (USAGE_TYPE_EXCH == ulCertAlg)?(unsigned char *)&trueValue:(unsigned char *)&falseValue, 1 },
		{ CKA_DECRYPT, (USAGE_TYPE_EXCH == ulCertAlg)?(unsigned char *)&trueValue:(unsigned char *)&falseValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
		{ CKA_ID, container, 0 },
		{ CKA_VALUE, lpCert, ulLen }
	};
	
	USES_CONVERSION;

	if (!hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == lpContainer || _tcslen(lpContainer) == 0)
	{
		USBKey_LogErr("lpContainer is NULL!", ulCertAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_ASYMMALG_RSA != ulCertAlg)
	{
		USBKey_LogErr("ulAlgID is wrong! ulAlgID = 0x%x", ulCertAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if ((USAGE_TYPE_SIGN != ulCertAlg) && (USAGE_TYPE_EXCH != ulCertAlg))
	{
		USBKey_LogErr("ulCertAlg is wrong! ulCertAlg = 0x%x", ulCertAlg);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == lpCert || ulLen <= 0)
	{
		USBKey_LogErr("lpCert is NULL or ulLen = 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}

#ifdef UNICODE
	strcpy_s((CHAR*)container, 256, W2A(lpContainer));
#else
	strcpy_s((CHAR*)container, 256, lpContainer);
#endif	//UNICODE
	attrs[9].ulValueLen = strlen((CHAR*)container);
	
	rv = m_pFunctionList->C_CreateObject(hSession, attrs, 11, &hCertObj);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CreateObject() failed！ ulRes = 0x%x", rv);
		return rv;
	}

	return rv;
}

// 导出证书
ULONG CPkcs11SDKTester::ExportCert(ULONG hSession, 
								   ULONG ulCertAlg, 
								   ULONG ulCertUsage, 
								   LPBYTE lpCert, 
								   ULONG* pulLen)
{
	CK_RV rv = CKR_OK;
	CK_ULONG findCount = 0;
	CK_OBJECT_HANDLE hCertObj = 0;
	CK_ATTRIBUTE cert_Attrs[ 7 ] = {
		{ CKA_CLASS, (unsigned char *)&ckoCertificate, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_KEY_TYPE, &ulCertAlg, sizeof(unsigned) },
		{ CKA_SIGN, (USAGE_TYPE_SIGN == ulCertAlg)?(unsigned char *)&trueValue:(unsigned char *)&falseValue, 1 },
		{ CKA_VERIFY, (USAGE_TYPE_SIGN == ulCertAlg)?(unsigned char *)&trueValue:(unsigned char *)&falseValue, 1 },
		{ CKA_ENCRYPT, (USAGE_TYPE_EXCH == ulCertAlg)?(unsigned char *)&trueValue:(unsigned char *)&falseValue, 1 },
		{ CKA_DECRYPT, (USAGE_TYPE_EXCH == ulCertAlg)?(unsigned char *)&trueValue:(unsigned char *)&falseValue, 1 }
	};
	CK_ATTRIBUTE certSize_Attrs[ 1 ] = 
	{
		{CKA_VALUE, NULL, 0}
	};		
	CK_ATTRIBUTE certData_Attrs[ 1 ] = 
	{
		{CKA_VALUE, lpCert, 0 }
	};

	// 查找对应的证书对象
	rv = m_pFunctionList->C_FindObjectsInit( hSession, cert_Attrs, 7);
	if (CKR_OK == rv)
	{
		USBKey_LogErr("C_FindObjectsInit() failed! rv = 0x%x", rv);
		return rv; 
	}
	rv = m_pFunctionList->C_FindObjects( hSession, &hCertObj, 1, &findCount);
	m_pFunctionList->C_FindObjectsFinal( hSession );
	if (CKR_OK == rv || NULL == hCertObj)
	{
		USBKey_LogErr("C_FindObjects() failed! rv = 0x%x", rv);
		return rv;
	}
	
	// 获取证书大小
	rv = m_pFunctionList->C_GetAttributeValue(hSession, hCertObj, certSize_Attrs, 1);
	if (CKR_OK == rv)
	{
		USBKey_LogErr("C_SetAttributeValue() failed! rv = 0x%x", rv);
		return rv; 
	}

	// 返回文件长度
	if (NULL == lpCert)
	{
		*pulLen = certSize_Attrs[0].ulValueLen;
		return USBKEY_ERR_OK;
	}
	if (*pulLen < certSize_Attrs[0].ulValueLen)
	{
		USBKey_LogErr("Buffer is too small! Requested length: 0x%x", certSize_Attrs[0].ulValueLen);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// 获取文件内容
	certData_Attrs[0].ulValueLen = *pulLen;
	rv = m_pFunctionList->C_GetAttributeValue(hSession, hCertObj, certData_Attrs, 1);
	if (CKR_OK == rv)
	{
		USBKey_LogErr("C_SetAttributeValue() failed! rv = 0x%x", rv);
		return rv; 
	}
	*pulLen = certData_Attrs[0].ulValueLen;

	return rv;
}

// 生成对称密钥
ULONG CPkcs11SDKTester::GenerateSessionKey(ULONG hSession, 
										   ULONG ulAlgID, 
										   ULONG* phSessionKey)
{
	CK_RV rv = 0;
	CK_ULONG ulKeyLen = 0;
	CK_KEY_TYPE ulKeyType = 0;
	CK_MECHANISM symmGenMech = { 0, 0, 0 };
	CK_OBJECT_HANDLE hSymmKey = NULL;
	
	if (NULL == phSessionKey)
	{
		USBKey_LogErr("phSessionKey is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	if (USBKEY_SYMMALG_3DES == ulAlgID)
	{
		ulKeyLen = 24;
		ulKeyType = CKK_DES3;
		symmGenMech.mechanism = CKM_DES3_KEY_GEN;
	}
	else if (USBKEY_SYMMALG_DES == ulAlgID)
	{
		ulKeyLen = 8;
		ulKeyType = CKK_DES;
		symmGenMech.mechanism = CKM_DES_KEY_GEN;
	}
	else
	{
		USBKey_LogErr("ulAlgID is not supported! ulAlgID = 0x%x", ulAlgID);
		return USBKEY_ERR_INVALIDPARAM;
	}

	// 创建会话密钥模板
	CK_ATTRIBUTE symmAttrs[ 9 ] = 
	{
		{ CKA_CLASS, (unsigned *)&ckoSecretKey, sizeof(unsigned) },
		{ CKA_KEY_TYPE, &ulKeyType, 4 },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_ENCRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },
		{ CKA_WRAP, (unsigned char *)&trueValue, 1 },
		{ CKA_EXTRACTABLE, (unsigned char *)&trueValue, 1 },
		{ CKA_VALUE_LEN, &ulKeyLen, 4 } 
	};
	rv = m_pFunctionList->C_GenerateKey((CK_SESSION_HANDLE)hSession, &symmGenMech, symmAttrs, 9, &hSymmKey); 
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GenerateKey() failed! rv = 0x%x", rv);
		return rv;
	}

	*phSessionKey = hSymmKey;

	return CKR_OK;
}

// 销毁对称密钥
ULONG CPkcs11SDKTester::CloseSessionKey(ULONG hSession, 
										ULONG hSessionKey)
{
	CK_RV rv = 0;
	
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (NULL == hSessionKey)
	{
		USBKey_LogErr("hSessionKey is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	rv = m_pFunctionList->C_DestroyObject((CK_SESSION_HANDLE)hSession, hSessionKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DestroyObject() failed! rv = 0x%x", rv);
		return rv;
	}
	
	hSessionKey = NULL;

	return CKR_OK;
}

// 生成签名密钥对
ULONG CPkcs11SDKTester::GenerateKeyPair(ULONG hSession,
										ULONG ulAlgID, 
										ULONG ulBits)
{
	CK_RV rv = CKR_OK;
	CK_OBJECT_HANDLE hPubKeyObj = NULL_PTR;
	CK_OBJECT_HANDLE hPriKeyObj = NULL_PTR;
	CK_CHAR_PTR keyContainer = (CK_CHAR_PTR)"SynoRSAContainer";
	
	if (!hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_ASYMMALG_RSA != ulAlgID)
	{
		USBKey_LogErr("ulAlgID is wrong! ulAlgID = 0x%x", ulAlgID);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (1024 != ulBits && 2048 != ulBits)
	{
		USBKey_LogErr("ulBits is wrong! ulBits = 0x%x", ulBits);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!m_pSlotIDList || m_ulSlotIDCnt == 0)
	{
		USBKey_LogErr("m_pSlotIDList is NULL or m_ulSlotIDCnt is 0!");
		return USBKEY_ERR_NO_DEVICE;
	}
		
	// 生成密钥对的模板
	CK_MECHANISM mech = { CKM_RSA_PKCS_KEY_PAIR_GEN, 0, 0 };
	unsigned char pubExponent[ 3 ] = { 0x01, 0x00, 0x01 };
	CK_ATTRIBUTE pubAttrs[ 8 ] = {
			{ CKA_CLASS, (unsigned *)&ckoPubKey, sizeof(ckoPubKey) },	
			{ CKA_KEY_TYPE, (unsigned *)&ckkRsa, sizeof(unsigned) },
			{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
			{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
			{ CKA_VERIFY, (unsigned char *)&trueValue, 1 },
			{ CKA_PUBLIC_EXPONENT, &pubExponent, 3 },
			{ CKA_MODULUS_BITS, &ulBits, 4 },
			{ CKA_LABEL, (void *)keyContainer, strlen((char*)keyContainer) } };
	CK_ATTRIBUTE privAttrs[ 6 ] = {
			{ CKA_CLASS, (unsigned *)&ckoPriKey, sizeof(ckoPriKey) },	
			{ CKA_KEY_TYPE, (unsigned *)&ckkRsa, sizeof(unsigned) },
			{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
			{ CKA_PRIVATE, (unsigned char *)&trueValue, 1 },
			{ CKA_SIGN, (unsigned char *)&trueValue, 1 },
			{ CKA_LABEL, (void *)keyContainer, strlen((char*)keyContainer) } };

	// 生成密钥对
	rv = m_pFunctionList->C_GenerateKeyPair((CK_SESSION_HANDLE)hSession, &mech, pubAttrs, 8, privAttrs, 6, &hPubKeyObj, &hPriKeyObj); 
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GenerateKeyPair() failed! rv = 0x%x", rv);
		goto ERR;
	}

	return CKR_OK;

ERR:
	if (hPubKeyObj)
	{
		m_pFunctionList->C_DestroyObject((CK_SESSION_HANDLE)hSession, hPubKeyObj);
		hPubKeyObj = NULL;
	}
	if (hPriKeyObj)
	{
		m_pFunctionList->C_DestroyObject((CK_SESSION_HANDLE)hSession, hPriKeyObj);
		hPriKeyObj = NULL;
	}

	return rv;
}

// 导入加密密钥对
ULONG CPkcs11SDKTester::ImportKeyPair(ULONG hSession,
									  ULONG ulAlgID, 
									  LPBYTE lpKeyPair, 
									  ULONG ulKeyPairLen)
{
	CK_RV rv = 0;
	CK_ULONG ulIndex = 0;
	CK_ULONG ulBits = 0;
	CK_ULONG ulByteLen = 0;
	CK_ULONG ulPrimeLen = 0;
	CK_BYTE btModulus[256] = {0};
	CK_BYTE btPubExponent[4] = {0};
	CK_BYTE btPrime1[128] = {0};
	CK_BYTE btPrime2[128] = {0};
	CK_BYTE btPriExponent[256] = {0};
	CK_BYTE btExponent1[128] = {0};
	CK_BYTE btExponent2[128] = {0};
	CK_BYTE btCoefficient[128] = {0};
	CK_OBJECT_HANDLE hPubKey = NULL;
	CK_OBJECT_HANDLE hPriKey = NULL;
	const CHAR csContainer[] = {"P11_Con\0"};

	// 参数检查
	if (!hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_ASYMMALG_RSA != ulAlgID)
	{
		USBKey_LogErr("P11 only support RSA key pair! ulAlgID = 0x%x", ulAlgID);
		return USBKEY_ERR_NOT_SUPPORT;
	}
	if (NULL == lpKeyPair)
	{
		USBKey_LogErr("lpKeyPair is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (0 == ulKeyPairLen)
	{
		USBKey_LogErr("ulKeyPairLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	// Key Bits
	ulBits = (lpKeyPair[1] << 8) | lpKeyPair[0];
	ulIndex += 2;
	ulByteLen = ulBits / 8;
	ulPrimeLen = ulByteLen / 2;

	// 模
	memcpy(btModulus, lpKeyPair + ulIndex, ulByteLen);
	ulIndex += ulByteLen;

	// 公钥指数
	memcpy(btPubExponent, lpKeyPair + ulIndex, 4);
	ulIndex += 4;
	
	// 私钥指数
	memcpy(btPriExponent, lpKeyPair + ulIndex, ulByteLen);
	ulIndex += ulByteLen;

	// 私钥Prime1
	memcpy(btPrime1, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;

	// 私钥Prime2
	memcpy(btPrime2, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;
		
	// 私钥Prime1Exponent
	memcpy(btExponent1, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;

	// 私钥Prime1Exponent
	memcpy(btExponent2, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;
	
	// 私钥Coefficient
	memcpy(btCoefficient, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;

	// 检查数据长度
	if (ulIndex != ulKeyPairLen)
	{
		USBKey_LogErr("Key pair data lenght is wrong! ulKeyPairLen = 0x%x", ulKeyPairLen);
		return USBKEY_ERR_INDATAERROR;
	}

	// 创建公钥对象的模板	
	CK_ATTRIBUTE publicKeyObjTemplate[ 11 ] = {
		{ CKA_CLASS, (unsigned char *)&ckoPubKey, sizeof(ckoPubKey) },	//对象类别
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },					//是Token对象
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },				//是公有对象
		{ CKA_KEY_TYPE, (unsigned char *)&ckkRsa, sizeof(ckkRsa) },		//密钥算法类型
		{ CKA_ID, (unsigned char *)csContainer, strlen(csContainer) },	//密钥所在容器名
		{ CKA_VERIFY, (unsigned char *)&falseValue, 1 },				//是否可以用来验证签名
		{ CKA_ENCRYPT, (unsigned char *)&trueValue, 1 },				//是否可以用来加密数据
		{ CKA_WRAP, (unsigned char *)&trueValue, 1 },					//是否可以用来Wrap Key
		{ CKA_MODULUS, btModulus, ulByteLen },							//模数n
		{ CKA_MODULUS_BITS, &ulBits, 4 },								//模数n的位长度
		{ CKA_PUBLIC_EXPONENT, btPubExponent, 4 }						//公用指数e
	};
	
	// 创建私钥对象模板
	CK_ATTRIBUTE privateKeyObjTemplate[ 14 ] = {
		{ CKA_CLASS, (unsigned char *)&ckoPriKey, sizeof(ckoPriKey) },	//对象类别
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },					//是Token对象
		{ CKA_PRIVATE, (unsigned char *)&trueValue, 1 },				//是私有对象
		{ CKA_KEY_TYPE, (unsigned char *)&ckkRsa, sizeof(ckkRsa) },		//密钥算法类型
		{ CKA_ID, (unsigned char *)csContainer, strlen(csContainer) },	//密钥所在容器名
		{ CKA_SIGN, (unsigned char *)&falseValue, 1 },					//是否可以用来签名
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },				//是否可以用来解密数据
		{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },					//是否可以用来Unwrap Key		
		{ CKA_PRIME_1, btPrime1, ulPrimeLen },							//Prime1
		{ CKA_PRIME_2, btPrime2, ulPrimeLen },							//Prime2
		{ CKA_PRIVATE_EXPONENT, btPriExponent, ulByteLen },				//Private Exponent
		{ CKA_EXPONENT_1, btExponent1, ulPrimeLen },					//Exponent1
		{ CKA_EXPONENT_2, btExponent2, ulPrimeLen },					//Exponent2
		{ CKA_COEFFICIENT, btCoefficient, ulPrimeLen }					//Coefficient
	};

	//创建公钥对象
	rv = m_pFunctionList->C_CreateObject((CK_SESSION_HANDLE)hSession, publicKeyObjTemplate, 11, &hPubKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("Call C_CreateObject() to import public key failed! rv = 0x%x", rv);
		return rv;
	}
	
	//创建私钥对象
	rv = m_pFunctionList->C_CreateObject((CK_SESSION_HANDLE)hSession, privateKeyObjTemplate, 14, &hPriKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("Call C_CreateObject() to import private key failed! rv = 0x%x", rv);
		return rv;
	}
	
	return rv;
}

// 导出公钥数据
ULONG CPkcs11SDKTester::ExportPublicKey(ULONG hSession, 
									    ULONG ulAlgID, 
									    BOOL bSign, 
									    LPBYTE lpPubKey, 
									    ULONG *pulLen)
{
	const ULONG MAX_PUBKEY_LEN = (4 + 256 + 4);
	CK_OBJECT_HANDLE hPubKeyObj = NULL;
	CK_RV rv = CKR_OK;
	CK_ULONG ulBits = 0;
	CK_BYTE btModulus[256] = {0};
	CK_BYTE btExponent[4] ={0};

	if (!hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_ASYMMALG_RSA != ulAlgID)
	{
		USBKey_LogErr("ulAlgID is wrong! ulAlgID = 0x%x", ulAlgID);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulLen)
	{
		USBKey_LogErr("pulLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	// 返回所需要的buffer长度
	if (!lpPubKey)
	{
		*pulLen = MAX_PUBKEY_LEN;
		return USBKEY_ERR_OK;
	}
	if (*pulLen < MAX_PUBKEY_LEN)
	{
		USBKey_LogErr("Buffer is too small! Requested length: 0x%x", MAX_PUBKEY_LEN);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// 查找公钥对象
	hPubKeyObj = _FindPubKeyObj((CK_SESSION_HANDLE)hSession, !bSign, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
	if (NULL == hPubKeyObj)
	{
		USBKey_LogErr("No any %s public key object!", bSign ? "sign" : "exch");
		return USBKEY_ERR_FAILED;
	}

	// 获取公钥属性
	CK_ATTRIBUTE PubKeyAttrs[ 3 ] = 
	{
		{ CKA_MODULUS_BITS, &ulBits, 4 },
		{ CKA_MODULUS, btModulus, 256 },
		{ CKA_PUBLIC_EXPONENT, btExponent, 4 }
	};
	rv = m_pFunctionList->C_GetAttributeValue((CK_SESSION_HANDLE)hSession, hPubKeyObj, PubKeyAttrs, 3);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GetAttributeValue() failed! rv = 0x%x", rv);
		return rv;
	}

	// 拷贝数据
	memcpy(lpPubKey, &ulBits, 4);
	memcpy(lpPubKey + 4, btModulus, ulBits / 8);
	memcpy(lpPubKey + 4 + ulBits / 8, btExponent, 4);

	return USBKEY_ERR_OK;
}

// 测试加密数据-解密数据
ULONG CPkcs11SDKTester::TestEncrypt(ULONG hSession, 
									ULONG ulAlgID, 
									LPCTSTR lpcsMsg)
{	
	CK_RV rv = 0;
	CK_ULONG ulSize = 0;
	CK_ULONG ulKeyLen = 0;
	CK_ULONG ulDataLen = 0;
	CK_ULONG ulEncryptedLen = 0;
	CK_ULONG ulDecryptedLen = 0;
	CK_KEY_TYPE ulKeyType = 0;
	CK_BYTE_PTR lpData = NULL;
	CK_BYTE_PTR lpEncrypted = NULL;
	CK_BYTE_PTR lpDecrypted = NULL;
	CK_OBJECT_HANDLE hEncryptKey = NULL;
	CK_OBJECT_HANDLE hDecryptKey = NULL;
	CK_MECHANISM keyGenMech = { 0, 0, 0 };
	CK_MECHANISM keyEncMech = { 0, 0, 0 };
	LPTSTR lptcHexString = NULL;
	
	USES_CONVERSION;

	if (!hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_ASYMMALG_RSA == ulAlgID)
	{
		ulKeyType = CKK_RSA;
		keyEncMech.mechanism = CKM_RSA_PKCS;
	}
	else if (USBKEY_SYMMALG_3DES == ulAlgID)
	{
		ulKeyLen = 24;
		ulKeyType = CKK_DES3;
		keyGenMech.mechanism = CKM_DES3_KEY_GEN;
		keyEncMech.mechanism = CKM_DES3_ECB;
	}
	else if (USBKEY_SYMMALG_DES == ulAlgID)
	{
		ulKeyLen = 8;
		ulKeyType = CKK_DES;
		keyGenMech.mechanism = CKM_DES_KEY_GEN;
		keyEncMech.mechanism = CKM_DES_ECB;
	}
	else
	{
		USBKey_LogErr("ulAlgID is not supported! ulAlgID = 0x%x", ulAlgID);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpcsMsg || 0 == _tcslen(lpcsMsg))
	{
		USBKey_LogErr("lpcsMsg is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
		
	// 使用RSA公钥加密
	if (USBKEY_ASYMMALG_RSA == ulAlgID)
	{
		ULONG ulBits = 0;

		ulSize = 4;
		hEncryptKey = _FindPubKeyObj((CK_SESSION_HANDLE)hSession, TRUE, CKA_KEY_TYPE, (LPBYTE)&ulKeyType, ulSize); 
		if (!hEncryptKey)
		{
			USBKey_LogErr("No encrypt public key!");
			return USBKEY_ERR_NO_KEYSET;
		}
		hDecryptKey = _FindPriKeyObj((CK_SESSION_HANDLE)hSession, FALSE, CKA_KEY_TYPE, (LPBYTE)&ulKeyType, ulSize); 
		if (!hDecryptKey)
		{
			USBKey_LogErr("No decrypt private key!");
			return USBKEY_ERR_NO_KEYSET;
		}
		
		CK_ATTRIBUTE pubKeyAttrs[ 1 ] = 
		{
			{CKA_MODULUS_BITS, &ulBits, 4}
		};

		rv = m_pFunctionList->C_GetAttributeValue((CK_SESSION_HANDLE)hSession, hEncryptKey, pubKeyAttrs, 1);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_GetAttributeValue() failed! rv = 0x%x", rv);
			return rv;
		}
		if (_tcslen(lpcsMsg) > (ulBits/8) - 11)
		{
			USBKey_LogErr("Message length is wrong! The max message length is: 0x%x", (ulBits/8) - 11);
			return USBKEY_ERR_INDATAERROR;
		}
	}
	// 生成一个对称密钥加密
	else
	{		
		// 创建会话密钥模板
		CK_ATTRIBUTE symmAttrs[ 9 ] = 
		{
			{ CKA_CLASS, (unsigned *)&ckoSecretKey, sizeof(unsigned) },
			{ CKA_KEY_TYPE, &ulKeyType, 4 },
			{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
			{ CKA_ENCRYPT, (unsigned char *)&trueValue, 1 },
			{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
			{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },
			{ CKA_WRAP, (unsigned char *)&trueValue, 1 },
			{ CKA_EXTRACTABLE, (unsigned char *)&trueValue, 1 },
			{ CKA_VALUE_LEN, &ulKeyLen, 4 } 
		};
		rv = m_pFunctionList->C_GenerateKey((CK_SESSION_HANDLE)hSession, &keyGenMech, symmAttrs, 9, &hEncryptKey); 
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_GenerateKey() failed! rv = 0x%x", rv);
			goto ERR;
		}
		hDecryptKey = hEncryptKey;
	}
	
#ifdef UNICODE
	lpData = (CK_BYTE_PTR)W2A(lpcsMsg);
#else
	lpData = (CK_BYTE_PTR)(lpcsMsg);
#endif	//UNICODE
	ulDataLen = strlen((CHAR*)lpData);

	m_pMsgCallbackFunc(_T("Plain Text:"), m_pCallbackParam);
	m_pMsgCallbackFunc(lpcsMsg, m_pCallbackParam);

	rv = m_pFunctionList->C_EncryptInit((CK_SESSION_HANDLE)hSession, &keyEncMech, hEncryptKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_EncryptInit() failed! rv = 0x%x", rv);
		goto ERR;
	}

	rv = m_pFunctionList->C_Encrypt((CK_SESSION_HANDLE)hSession, lpData, ulDataLen, NULL, &ulEncryptedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Encrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}

	lpEncrypted = new BYTE[ulEncryptedLen];
	rv = m_pFunctionList->C_Encrypt((CK_SESSION_HANDLE)hSession, lpData, ulDataLen, lpEncrypted, &ulEncryptedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Encrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}

	USBKey_ConvertBufToHexStr(lpEncrypted, ulEncryptedLen, &lptcHexString);
	m_pMsgCallbackFunc(_T("Encrypted data:"), m_pCallbackParam);
	m_pMsgCallbackFunc(lptcHexString, m_pCallbackParam);
		
	// 解密初始化
	rv = m_pFunctionList->C_DecryptInit((CK_SESSION_HANDLE)hSession, &keyEncMech, hDecryptKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DecryptInit() failed! rv = 0x%x", rv);
		goto ERR;
	}
	
	rv = m_pFunctionList->C_Decrypt((CK_SESSION_HANDLE)hSession, lpEncrypted, ulEncryptedLen, NULL, &ulDecryptedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Decrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}

	lpDecrypted = new BYTE[ulDecryptedLen + 1];
	rv = m_pFunctionList->C_Decrypt((CK_SESSION_HANDLE)hSession, lpEncrypted, ulEncryptedLen, lpDecrypted, &ulDecryptedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Decrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}
	lpDecrypted[ulDecryptedLen] = 0;

	m_pMsgCallbackFunc(_T("Decrypted Text:"), m_pCallbackParam);
#ifdef UNICODE
	m_pMsgCallbackFunc(A2W((CHAR*)lpDecrypted), m_pCallbackParam);
#else
	m_pMsgCallbackFunc((CHAR*)lpDecrypted, m_pCallbackParam);
#endif	//UNICODE
	
ERR:
	if (lptcHexString)
	{
		delete []lptcHexString;
		lptcHexString = NULL;
	}
	if (lpDecrypted)
	{
		delete []lpDecrypted;
		lpDecrypted = NULL;
	}
	if (lpEncrypted)
	{
		delete []lpEncrypted;
		lpEncrypted = NULL;
	}
	if (hEncryptKey && (CKK_RSA != ulKeyType))
	{
		m_pFunctionList->C_DestroyObject((CK_SESSION_HANDLE)hSession, hEncryptKey);
	}
	hEncryptKey = NULL;

	return rv;
}

// 测试打包数字信封数据-解密数字信封
ULONG CPkcs11SDKTester::TestEnvelop(ULONG hSession, 
									ULONG ulAlgID, 
									LPCTSTR lpcsMsg)
{
	CK_RV rv = 0;
	CK_ULONG ulSize = 0;
	CK_ULONG ulKeyLen = 0;
	CK_ULONG ulPlainIndex = 0;
	CK_ULONG ulWrappedLen = 0;
	CK_ULONG ulEnveloppedLen = 0;
	CK_ULONG ulDeveloppedLen = 0;
	CK_KEY_TYPE ulKeyType = 0;
	CK_BYTE_PTR lpData = NULL;
	CK_BYTE_PTR lpEnvelopped = NULL;
	CK_BYTE_PTR lpDevelopped = NULL;
	CK_OBJECT_HANDLE hExchPubKey = NULL;
	CK_OBJECT_HANDLE hExchPriKey = NULL;
	CK_OBJECT_HANDLE hSymmKey = NULL;
	CK_MECHANISM symmGenMech = { 0, 0, 0 };
	CK_MECHANISM symmEncMech = { 0, 0, 0 };
	CK_MECHANISM wrapMech = { 0, 0, 0 };
	CK_MECHANISM unwrapMech = { 0, 0, 0 };
	LPTSTR lptcHexString = NULL;
	BYTE btWrappedData[512] = {0};
	
	USES_CONVERSION;

	if (!hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_SYMMALG_3DES == ulAlgID)
	{
		ulKeyLen = 24;
		ulKeyType = CKK_DES3;
		symmGenMech.mechanism = CKM_DES3_KEY_GEN;
		symmEncMech.mechanism = CKM_DES3_ECB;
	}
	else if (USBKEY_SYMMALG_DES == ulAlgID)
	{
		ulKeyLen = 8;
		ulKeyType = CKK_DES;
		symmGenMech.mechanism = CKM_DES_KEY_GEN;
		symmEncMech.mechanism = CKM_DES_ECB;
	}
	else
	{
		USBKey_LogErr("ulAlgID is not supported! ulAlgID = 0x%x", ulAlgID);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpcsMsg || 0 == _tcslen(lpcsMsg))
	{
		USBKey_LogErr("lpcsMsg is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
#ifdef UNICODE
	lpData = (CK_BYTE_PTR)W2A(lpcsMsg);
#else
	lpData = (CK_BYTE_PTR)(lpcsMsg);
#endif	//UNICODE

	m_pMsgCallbackFunc(_T("Plain Text:"), m_pCallbackParam);
	m_pMsgCallbackFunc(lpcsMsg, m_pCallbackParam);
			
	// 查找加密公钥
	hExchPubKey = _FindPubKeyObj((CK_SESSION_HANDLE)hSession, TRUE, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4); 
	if (!hExchPubKey)
	{
		USBKey_LogErr("No encrypt public key!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// 创建会话密钥模板
	CK_ATTRIBUTE symmAttrs[ 9 ] = 
	{
		{ CKA_CLASS, (unsigned *)&ckoSecretKey, sizeof(unsigned) },
		{ CKA_KEY_TYPE, &ulKeyType, 4 },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_ENCRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },
		{ CKA_WRAP, (unsigned char *)&trueValue, 1 },
		{ CKA_EXTRACTABLE, (unsigned char *)&trueValue, 1 },
		{ CKA_VALUE_LEN, &ulKeyLen, 4 } 
	};
	rv = m_pFunctionList->C_GenerateKey((CK_SESSION_HANDLE)hSession, &symmGenMech, symmAttrs, 9, &hSymmKey); 
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GenerateKey() failed! rv = 0x%x", rv);
		goto ERR;
	}
	
	// 创建保存密文的Buffer
	ulEnveloppedLen = 0;
	lpEnvelopped = new BYTE[strlen((CHAR*)lpData) + 512];

	// 使用加密公钥Wrap会话密钥
	ulWrappedLen = 512;
	wrapMech.mechanism = CKM_RSA_PKCS;
	rv = m_pFunctionList->C_WrapKey((CK_SESSION_HANDLE)hSession, &wrapMech, hExchPubKey, hSymmKey, btWrappedData, &ulWrappedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_WrapKey() failed! rv = 0x%x", rv);
		goto ERR;
	}
	memcpy(lpEnvelopped, &ulWrappedLen, 4);
	ulEnveloppedLen = 4;
	memcpy(lpEnvelopped + ulEnveloppedLen, btWrappedData, ulWrappedLen);
	ulEnveloppedLen += ulWrappedLen;
	
	// 使用会话密钥加密数据
	rv = m_pFunctionList->C_EncryptInit((CK_SESSION_HANDLE)hSession, &symmEncMech, hSymmKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_EncryptInit() failed! rv = 0x%x", rv);
		goto ERR;
	}	
	rv = m_pFunctionList->C_Encrypt((CK_SESSION_HANDLE)hSession, lpData, strlen((CHAR*)lpData), lpEnvelopped + ulEnveloppedLen, &ulWrappedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Encrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}
	ulEnveloppedLen += ulWrappedLen;
	m_pFunctionList->C_DestroyObject((CK_SESSION_HANDLE)hSession, hSymmKey);
	hSymmKey = NULL;
	
	USBKey_ConvertBufToHexStr(lpEnvelopped, ulEnveloppedLen, &lptcHexString);
	m_pMsgCallbackFunc(_T("Envelopped data:"), m_pCallbackParam);
	m_pMsgCallbackFunc(lptcHexString, m_pCallbackParam);
		
	// 创建Unwrap会话密钥模板
	CK_ATTRIBUTE unwrapAttrs[ 8 ] = 
	{
		{ CKA_CLASS, (unsigned *)&ckoSecretKey, sizeof(unsigned) },
		{ CKA_KEY_TYPE, &ulKeyType, sizeof(int) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },
	};
	
	// 查找解密私钥
	hExchPriKey = _FindPriKeyObj((CK_SESSION_HANDLE)hSession, FALSE, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
	if (NULL == hExchPriKey)
	{
		USBKey_LogErr("No key set!");
		rv = USBKEY_ERR_NO_KEYSET;
		goto ERR;
	}

	//　解密会话密钥
	memcpy(&ulWrappedLen, lpEnvelopped, 4);
	unwrapMech.mechanism = CKM_RSA_PKCS;
	rv = m_pFunctionList->C_UnwrapKey((CK_SESSION_HANDLE)hSession, &unwrapMech, hExchPriKey, lpEnvelopped + 4, ulWrappedLen, unwrapAttrs, 5, &hSymmKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_UnwrapKey() failed! rv = 0x%x", rv);
		goto ERR;
	}		

	// 使用会话密钥解密数据
	rv = m_pFunctionList->C_DecryptInit((CK_SESSION_HANDLE)hSession, &symmEncMech, hSymmKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DecryptInit() failed! rv = 0x%x", rv);
		goto ERR;
	}	
	rv = m_pFunctionList->C_Decrypt((CK_SESSION_HANDLE)hSession, lpEnvelopped + (4+ulWrappedLen), ulEnveloppedLen-(4+ulWrappedLen), NULL, &ulDeveloppedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Decrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}
	ulDeveloppedLen += 1;
	lpDevelopped = new BYTE[ulDeveloppedLen];
	memset(lpDevelopped, 0, ulDeveloppedLen);
	rv = m_pFunctionList->C_Decrypt((CK_SESSION_HANDLE)hSession, lpEnvelopped + (4+ulWrappedLen), ulEnveloppedLen-(4+ulWrappedLen), lpDevelopped, &ulDeveloppedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Decrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}

	m_pMsgCallbackFunc(_T("Developped Text:"), m_pCallbackParam);
#ifdef UNICODE
	m_pMsgCallbackFunc(A2W((CHAR*)lpDevelopped), m_pCallbackParam);
#else
	m_pMsgCallbackFunc((CHAR*)lpDevelopped, m_pCallbackParam);
#endif	//UNICODE
	
ERR:
	if (lptcHexString)
	{
		delete []lptcHexString;
		lptcHexString = NULL;
	}
	if (lpDevelopped)
	{
		delete []lpDevelopped;
		lpDevelopped = NULL;
	}
	if (lpEnvelopped)
	{
		delete []lpEnvelopped;
		lpEnvelopped = NULL;
	}
	if (hSymmKey)
	{
		m_pFunctionList->C_DestroyObject((CK_SESSION_HANDLE)hSession, hSymmKey);
		hSymmKey = NULL;
	}

	return rv;
}

// 测试签名验签
ULONG CPkcs11SDKTester::TestSign(ULONG hSession, 
								 ULONG ulAsymID, 
								 ULONG ulHashID, 
								 LPCTSTR lpcsMsg)
{
	CK_RV rv = CKR_OK;
	CK_BYTE btSignature[256] = {0};
	CK_ULONG ulSignatureLen = 0;
	CK_ULONG ulOrgLen = 0;
	CK_BYTE_PTR lpOrgData = NULL;
	CK_MECHANISM signMech = {0, 0, 0};
	CK_OBJECT_HANDLE hSignPriKey = 0;
	CK_OBJECT_HANDLE hVerifyPubKey = 0;
	LPTSTR lptcHexString = NULL;
	
	USES_CONVERSION;

	// 调用合法性检查
	if (!m_pFunctionList || !hSession)
	{
		USBKey_LogErr("m_pFunctionList or hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	
	//	参数检查
	if (!lpcsMsg || _tcslen(lpcsMsg) == 0)
	{
		USBKey_LogErr("lpcsMsg is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_ASYMMALG_RSA != ulAsymID)
	{
		USBKey_LogErr("ulAsymID is wrong! ulAsymID = 0x%x", ulAsymID);
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (USBKEY_HASHALG_MD5 == ulHashID)
	{
		signMech.mechanism = CKM_MD5_RSA_PKCS;
	}
	else if (USBKEY_HASHALG_SHA1 == ulHashID)
	{
		signMech.mechanism = CKM_SHA1_RSA_PKCS;
	}
	else if (USBKEY_HASHALG_SHA256 == ulHashID)
	{
		signMech.mechanism = CKM_SHA256_RSA_PKCS;
	}
	else
	{
		if (ulOrgLen> 256)
		{
			USBKey_LogErr("ulOrgLen is wrong! ulOrgLen = 0x%x", ulOrgLen);
			return USBKEY_ERR_INVALIDPARAM;
		}
		signMech.mechanism = CKM_RSA_PKCS;
	}
	
#ifdef UNICODE
	lpOrgData = (CK_BYTE_PTR)W2A(lpcsMsg);
#else
	lpOrgData = (CK_BYTE_PTR)(lpcsMsg);
#endif	//UNICODE
	ulOrgLen = strlen((CHAR*)lpOrgData);

	m_pMsgCallbackFunc(_T("Plain Text:"), m_pCallbackParam);
	m_pMsgCallbackFunc(lpcsMsg, m_pCallbackParam);			

	// 查找签名私钥
	hSignPriKey = _FindPriKeyObj((CK_SESSION_HANDLE)hSession, TRUE, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
	if (!hSignPriKey)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}
		
	// 签名初始化
	rv = m_pFunctionList->C_SignInit((CK_SESSION_HANDLE)hSession, &signMech, hSignPriKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SignInit() failed! rv = 0x%x", rv);
		goto ERR;
	}
	
	// 使用签名私钥对HASH值签名
	ulSignatureLen = 256;
	rv = m_pFunctionList->C_Sign((CK_SESSION_HANDLE)hSession, /*btHashValue, ulHashLen*/lpOrgData, ulOrgLen, btSignature, &ulSignatureLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Sign() failed! rv = 0x%x", rv);
		goto ERR;
	}

	delete []lptcHexString;
	lptcHexString = NULL;
	USBKey_ConvertBufToHexStr(btSignature, ulSignatureLen, &lptcHexString);
	m_pMsgCallbackFunc(_T("Signature data:"), m_pCallbackParam);
	m_pMsgCallbackFunc(lptcHexString, m_pCallbackParam);

	// 查找签名公钥
	hVerifyPubKey = _FindPubKeyObj((CK_SESSION_HANDLE)hSession, FALSE, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
	if (NULL == hVerifyPubKey)
	{
		USBKey_LogErr("No key set!");
		rv = USBKEY_ERR_NO_KEYSET;
		goto ERR;
	}
	
	// 初始化
	rv = m_pFunctionList->C_VerifyInit((CK_SESSION_HANDLE)hSession, &signMech, hVerifyPubKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_VerifyInit() failed! rv = 0x%x", rv);
		goto ERR;
	}
	
	// 使用签名私钥对HASH值签名	
	rv = m_pFunctionList->C_Verify((CK_SESSION_HANDLE)hSession, /*btHashValue, ulHashLen*/lpOrgData, ulOrgLen, btSignature, ulSignatureLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Verify() failed! rv = 0x%x", rv);
		goto ERR;
	}
		
	m_pMsgCallbackFunc(_T("Verify signature successfully!"), m_pCallbackParam);
	
ERR:
	if (lptcHexString)
	{
		delete []lptcHexString;
		lptcHexString = NULL;
	}

	return rv;
}


// 等待设备事件
ULONG CPkcs11SDKTester::WaitForDevEvent(LPTSTR szDevName, 
										ULONG *pulDevNameLen, 
										ULONG *pulSlotID, 
										ULONG *pulEvent)
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulEventSlotId = 0;
	CK_ULONG ulEventType = 0;
	
	// 调用合法性检查
	if (!m_pFunctionList)
	{
		USBKey_LogErr("m_pFunctionList is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	
	//	参数检查
	if (!pulSlotID)
	{
		USBKey_LogErr("pulSlotID is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!pulEvent)
	{
		USBKey_LogErr("pulEvent is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	rv = m_pFunctionList->C_WaitForSlotEvent(0, &ulEventSlotId, &ulEventType);
	if ((CKR_OK == rv) && (ulEventSlotId >= 0))	//有事件发生
	{
		*pulSlotID = ulEventSlotId;
		*pulEvent = ulEventType;
	}
	
	return rv;
}

// 取消等待(PKCS11不支持)
ULONG CPkcs11SDKTester::CancelWaitForDevEvent()
{
	return USBKEY_ERR_NOT_SUPPORT;
}


ULONG CPkcs11SDKTester::_FindSlotIDByDevName(LPCTSTR lpcsDevName, ULONG &ulSlotID)
{
	CK_RV rv = 0;
	CK_ULONG ulIndex = 0;
	CK_SLOT_INFO slotInfo = {0};
	CK_TOKEN_INFO tokenInfo = {0};
	LPCTSTR lpcsLabel = NULL;
	TCHAR tcInfo[512] = {0};
	
	USES_CONVERSION;

	if (!lpcsDevName || _tcslen(lpcsDevName) == 0)
	{
		USBKey_LogErr("lpcsDevName is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!m_pSlotIDList || m_ulSlotIDCnt == 0)
	{
		USBKey_LogErr("m_pSlotIDList is NULL or m_ulSlotIDCnt is 0!");
		return USBKEY_ERR_NO_DEVICE;
	}

	for (ulIndex = 0; ulIndex < m_ulSlotIDCnt; ulIndex++)
	{
		rv = m_pFunctionList->C_GetTokenInfo(m_pSlotIDList[ulIndex], &tokenInfo);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_GetTokenInfo() failed when slot id = %d! rv = 0x%x", m_pSlotIDList[ulIndex], rv);
			continue;
		}
#ifdef UNICODE
		lpcsLabel = A2W((char*)tokenInfo.label);
#else
		lpcsLabel = (char*)tokenInfo.label;
#endif	//UNICODE

		if (_tcscmp(lpcsLabel, lpcsDevName) == 0)
		{
			ulSlotID = m_pSlotIDList[ulIndex];
			break;
		}
	}
	if (ulIndex == m_ulSlotIDCnt)
	{
		USBKey_LogErr("The device name is not foud!");
		return USBKEY_ERR_NO_DEVICE;
	}

	return CKR_OK;
}

//	查找公钥对象
CK_OBJECT_HANDLE CPkcs11SDKTester::_FindPubKeyObj(CK_SESSION_HANDLE hSession, 
												  BOOL bIsExchKey, 
												  ULONG ckaType, 
												  LPBYTE data, 
												  ULONG size)
{
	CK_RV rv = CKR_OK;
	CK_ULONG findCount = 0;
	CK_OBJECT_HANDLE hKey = 0;
	CK_ATTRIBUTE encryptKey_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned *)&ckoPubKey, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
		{ CKA_ENCRYPT/*CKA_WRAP*/, (unsigned char *)&trueValue, 1 },
		{ ckaType, data, size },
	};
	CK_ATTRIBUTE verifyKey_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned *)&ckoPubKey, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
		{ CKA_VERIFY, (unsigned char *)&trueValue, 1 },
		{ ckaType, data, size },
	};

	rv = m_pFunctionList->C_FindObjectsInit( hSession, bIsExchKey ? encryptKey_Attrs : verifyKey_Attrs, data ? 5 : 4);
	rv = m_pFunctionList->C_FindObjects( hSession, &hKey, 1, &findCount);
	rv = m_pFunctionList->C_FindObjectsFinal( hSession );

	return hKey;
}

//	查找私钥对象
CK_OBJECT_HANDLE CPkcs11SDKTester::_FindPriKeyObj(CK_SESSION_HANDLE hSession, 
												BOOL bIsSignKey, 
												ULONG ckaType, 
												LPBYTE data, 
												ULONG size)
{
	CK_RV rv = CKR_OK;
	CK_ULONG findCount = 0;
	CK_OBJECT_HANDLE hKey = 0;
	CK_ATTRIBUTE decryptKey_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned *)&ckoPriKey, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&trueValue, 1 },
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
		{ ckaType, data, size },
	};
	CK_ATTRIBUTE signKey_Attrs[ 5 ] = {
		{ CKA_CLASS, (unsigned *)&ckoPriKey, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&trueValue, 1 },
		{ CKA_SIGN, (unsigned char *)&trueValue, 1 },
		{ ckaType, data, size },
	};
	rv = m_pFunctionList->C_FindObjectsInit( hSession, bIsSignKey ? signKey_Attrs : decryptKey_Attrs, data ? 5 : 3);
	rv = m_pFunctionList->C_FindObjects( hSession, &hKey, 1, &findCount);
	rv = m_pFunctionList->C_FindObjectsFinal( hSession );

	return hKey;
}

// 根据指定的文件名，查找Data File对象
CK_OBJECT_HANDLE CPkcs11SDKTester::_FindDataFileObj(CK_SESSION_HANDLE hSession, 
													LPCSTR lpcsFileName)
{
	CK_RV rv = CKR_OK;
	CK_ULONG findCount = 0;
	CK_OBJECT_HANDLE hDataFile = 0;
	CK_ATTRIBUTE dataFile_Attrs[ 4 ] = {
		{ CKA_CLASS, (unsigned *)&ckoDataFile, sizeof(unsigned) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
		{ CKA_ID, (LPBYTE)lpcsFileName, strlen(lpcsFileName) },
	};
	rv = m_pFunctionList->C_FindObjectsInit( hSession, dataFile_Attrs, 4);
	rv = m_pFunctionList->C_FindObjects( hSession, &hDataFile, 1, &findCount);
	rv = m_pFunctionList->C_FindObjectsFinal( hSession );

	return hDataFile;
}

//	对数据做哈希运算
ULONG CPkcs11SDKTester::_GetHashValue(CK_SESSION_HANDLE hSession, 
									ULONG ulCkm, 
									LPBYTE lpOrgData, 
									ULONG ulOrgLen, 
									LPBYTE lpHashValue, 
									ULONG* pHashLen)
{
	CK_RV rv = CKR_OK;
	CK_MECHANISM hashMech = {ulCkm, 0, 0};
	CK_ULONG ulIndex = 0;
	
	if (!m_pFunctionList || !hSession)
	{
		USBKey_LogErr("m_pFunctionList or hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	if (!lpOrgData || ulOrgLen == 0)
	{
		USBKey_LogErr("lpOrgData is NULL or ulOrgLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpHashValue || !pHashLen)
	{
		USBKey_LogErr("lpHashValue or pHashLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (*pHashLen < 32)
	{
		USBKey_LogErr("Buffer is to small! Request length:0x%x", *pHashLen);
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// 初始化
	rv = m_pFunctionList->C_DigestInit((CK_SESSION_HANDLE)hSession, &hashMech);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DigestInit() is failed! rv = 0x%x", rv);
		return rv;
	}

	// 多次Hash
	ulIndex = 0;
	do
	{
		ULONG ulLenThisTime = 0;
		if (ulOrgLen - ulIndex > USBKEY_CRYPTLEN_PERTIME)
		{
			ulLenThisTime = USBKEY_CRYPTLEN_PERTIME;
		}
		else
		{
			ulLenThisTime = ulOrgLen - ulIndex;
		}
		rv = m_pFunctionList->C_DigestUpdate(hSession, lpOrgData + ulIndex, ulLenThisTime);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_DigestUpdate() is failed! rv = 0x%x", rv);
			return rv;
		}
		ulIndex += ulLenThisTime;
	}while(ulIndex < ulOrgLen);

	// 获取Hash结果
	rv = m_pFunctionList->C_DigestFinal(hSession, lpHashValue, pHashLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DigestFinal() is failed! rv = 0x%x", rv);
		return rv;
	}
	
	return rv;
}