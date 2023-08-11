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

// ����SDK DLL�ļ�
ULONG CPkcs11SDKTester::LoadDll(LPCTSTR lpcsCspOrFile, 
								FuncMsgCallback pMsgFunc, 
								LPVOID lpParam)
{
	ULONG ulRes = 0;

	//	�������
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

	//	����	PKCS11�ӿ�SDK
	m_hInstance = LoadLibrary(lpcsCspOrFile);	
	if (NULL == m_hInstance)
	{
		ulRes = GetLastError();
		USBKey_LogErr("LoadLibrary() failed! dwRes = 0x%x", ulRes);
		return ulRes;
	}

	//	����PKCS11����	
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
	
	//	��ʼ��
	ulRes = m_pFunctionList->C_Initialize(NULL);
	if (CKR_OK != ulRes)
	{
		USBKey_LogErr("C_Initialize() failed! dwRes = 0x%x", ulRes);
		return ulRes;
	}	

	return ulRes;
}

// ж��DLL�ļ�
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

// ö���豸
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

// ��ȡ�豸��Ϣ
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
	
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}

	// ��ȡSlot Info
	rv = m_pFunctionList->C_GetSlotInfo(selSlotID, &slotInfo);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GetSlotInfo() failed when slot id = %d! rv = 0x%x", selSlotID, rv);
		return rv;
	}

	// ��ȡToken Info
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

// ��ʼ���豸
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
	
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}

	// �ص���ǰ���лỰ
	rv = m_pFunctionList->C_CloseAllSessions(selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CloseAllSessions() failed! rv = 0x%x", rv);
		return rv;
	}

	// ��ʼ��Token
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
	
	// �򿪻Ự
	rv = m_pFunctionList->C_OpenSession(selSlotID, 
		CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &hSession);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_OpenSession() failed! rv=0x%x", rv);
		goto FREE_MEMORY;
	}

	// У��SO PIN��
	rv = m_pFunctionList->C_Login(hSession, CKU_SO, utf8SOPin, strlen((CHAR*)utf8SOPin));
	if (CKR_OK != rv && CKR_USER_ALREADY_LOGGED_IN != rv)
	{
		USBKey_LogErr("C_Login() failed! rv=0x%x", rv);
		goto FREE_MEMORY;
	}

	// ���ó�ʼUser PIN
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

// ���ñ�ǩ(��֧��)
ULONG CPkcs11SDKTester::SetLabel(LPCTSTR lpcsDevName, 
								 LPCTSTR lpcsLabel)
{	

	return USBKEY_ERR_NOT_SUPPORT;
}

// �豸��֤(��֧��)
ULONG CPkcs11SDKTester::DevAuth(LPCTSTR lpcsDevName, 
								LPBYTE lpAuthKey, 
								ULONG ulKeyLen)
{
	return USBKEY_ERR_NOT_SUPPORT;
}

// �޸��豸��֤Key(��֧��)
ULONG CPkcs11SDKTester::ChangeAuthKey(LPCTSTR lpcsDevName, 
									  LPBYTE lpNewAuthKey, 
									  ULONG ulKeyLen)
{
	return USBKEY_ERR_NOT_SUPPORT;
}

// �򿪻Ự
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

	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}
	
	// �򿪻Ự
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

// ��ȡ�Ự��Ϣ
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

	// ��ǰû�д򿪵ĻỰ
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	
	// �رջỰ
	rv = m_pFunctionList->C_GetSessionInfo((CK_SESSION_HANDLE)hSession, &sessionInfo);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CloseSession() failed! rv = 0x%x", rv);
		return rv;
	}

	// �����Ϣ
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

// �رջỰ
ULONG CPkcs11SDKTester::CloseSession(ULONG hSession)
{
	CK_RV rv = CKR_OK;

	// ��ǰû�д򿪵ĻỰ
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}

	// �رջỰ
	rv = m_pFunctionList->C_CloseSession((CK_SESSION_HANDLE)hSession);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CloseSession() failed! rv = 0x%x", rv);
		return rv;
	}

	hSession = NULL;

	return rv;
}

// ��¼�Ự
ULONG CPkcs11SDKTester::LoginSession(ULONG hSession, 
									 ULONG ulUserType, 
									 LPCTSTR lpcsPIN, 
									 ULONG* pulLeftTimes)
{
	CK_RV rv = CKR_OK;
	CK_USER_TYPE userType = 0;
	CK_UTF8CHAR_PTR utf8Pin = NULL;
	
	// ��ǰû�д򿪵ĻỰ
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	
	// �û�����
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

	// ��¼
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

// �ǳ��Ự
ULONG CPkcs11SDKTester::LogOutSession(ULONG hSession)
{
	CK_RV rv = CKR_OK;
	
	// ��ǰû�д򿪵ĻỰ
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
			
	// ��¼
	rv = m_pFunctionList->C_Logout((CK_SESSION_HANDLE)hSession);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Logout() failed! rv = 0x%x", rv);
		return rv;
	}

	return rv;
}

// �޸�PIN��
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
	
	// ��ǰû�д򿪵ĻỰ
	if (NULL == hSession)
	{
		USBKey_LogErr("hSession is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
			
	// �û�����
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

	// �޸�PIN��
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

// �����û�PIN
ULONG CPkcs11SDKTester::UnlockPin(ULONG hSession, 
								  LPCTSTR lpcsSOPIN, 
								  LPCTSTR lpcsNewUserPIN, 
								  ULONG* pulLeftTimes)
{
	CK_RV rv = CKR_OK;
	CK_UTF8CHAR_PTR utf8SOPin = NULL;
	CK_UTF8CHAR_PTR utf8NewPin = NULL;
	
	// ��ǰû�д򿪵ĻỰ
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

	// SO��¼
	rv = m_pFunctionList->C_Login((CK_SESSION_HANDLE)hSession, CKU_SO, utf8SOPin, strlen((char*)utf8SOPin));
	if (CKR_OK != rv && CKR_USER_ALREADY_LOGGED_IN != rv)
	{
		USBKey_LogErr("C_Login() failed! rv = 0x%x", rv);
		goto FREE_MEMORY;
	}

	// ����User��ʼPIN
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

// ö���ļ�
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

	// ��ǰû�д򿪵ĻỰ
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
	
	// ����Data File����
	rv = m_pFunctionList->C_FindObjectsInit(hSession, file_Attrs, 3);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_FindObjectsInit() failed! rv = 0x%x", rv);
		return rv;
	}

	// �ֱ��ȡÿ��Data File������ļ���
	nameListLen = 0;
	while (CKR_OK == m_pFunctionList->C_FindObjects(hSession, &hFileObj, 1, &findCount) && (1 == findCount))
	{		
		memset(fileName, 0, 256);
		rv = m_pFunctionList->C_GetAttributeValue(hSession, hFileObj, &attr, 1);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_GetAttributeValue() failed�� ulRes = 0x%x", rv);
			continue;
		}

		memcpy(nameList + nameListLen, fileName, attr.ulValueLen);
		nameListLen += (attr.ulValueLen + 1);
	}
	nameListLen++;
	
	// ���ҽ���
	m_pFunctionList->C_FindObjectsFinal(hSession);

	// ���س���
	if (NULL == lpFilesName)
	{
		*pulLen = nameListLen;
		return USBKEY_ERR_OK;
	}
	if (*pulLen < nameListLen)
	{
		return USBKEY_ERR_BUFFER_TOO_SMALL;
	}

	// ��������
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

// �����ļ�
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

	// ������
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
		USBKey_LogErr("C_CreateObject() failed�� ulRes = 0x%x", rv);
		return rv;
	}

	return rv;
}

// ɾ���ļ�
ULONG CPkcs11SDKTester::DeleteFile(ULONG hSession, 
								   LPTSTR lpFilesName)
{
	CK_RV rv = CKR_OK;
	CK_BYTE fileName[256] = {0};
	CK_OBJECT_HANDLE hFileObj = 0;

	USES_CONVERSION;

	// ������
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

	// �����ļ�����
	hFileObj = _FindDataFileObj(hSession, (LPCSTR)fileName);
	if (NULL == hFileObj)
	{
		USBKey_LogErr("File %s is not found!", fileName);
		return USBKEY_ERR_FAILED; 
	}

	// ɾ���ļ�����
	rv = m_pFunctionList->C_DestroyObject(hSession, hFileObj);
	if (CKR_OK == rv)
	{
		USBKey_LogErr("C_DestroyObject() failed! rv = 0x%x", rv);
		return rv; 
	}

	return rv;
}

// д�ļ�
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

	// ������
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

	// �����ļ�����
	hFileObj = _FindDataFileObj(hSession, (LPCSTR)fileName);
	if (NULL == hFileObj)
	{
		USBKey_LogErr("File %s is not found!", fileName);
		return USBKEY_ERR_FAILED; 
	}

	// ɾ���ļ�����
	rv = m_pFunctionList->C_SetAttributeValue(hSession, hFileObj, file_Attrs, 1);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SetAttributeValue() failed! rv = 0x%x", rv);
		return rv; 
	}

	return rv;
}

// ���ļ�
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

	// ������
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

	// �����ļ�����
	hFileObj = _FindDataFileObj(hSession, (LPCSTR)fileName);
	if (NULL == hFileObj)
	{
		USBKey_LogErr("File %s is not found!", fileName);
		return USBKEY_ERR_FAILED; 
	}

	// ��ȡ�ļ���С
	rv = m_pFunctionList->C_GetAttributeValue(hSession, hFileObj, fileSize_Attrs, 1);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SetAttributeValue() failed! rv = 0x%x", rv);
		return rv; 
	}

	// �����ļ�����
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

	// ��ȡ�ļ�����
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

// ����֤��
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
		USBKey_LogErr("C_CreateObject() failed�� ulRes = 0x%x", rv);
		return rv;
	}

	return rv;
}

// ����֤��
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

	// ���Ҷ�Ӧ��֤�����
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
	
	// ��ȡ֤���С
	rv = m_pFunctionList->C_GetAttributeValue(hSession, hCertObj, certSize_Attrs, 1);
	if (CKR_OK == rv)
	{
		USBKey_LogErr("C_SetAttributeValue() failed! rv = 0x%x", rv);
		return rv; 
	}

	// �����ļ�����
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

	// ��ȡ�ļ�����
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

// ���ɶԳ���Կ
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

	// �����Ự��Կģ��
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

// ���ٶԳ���Կ
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

// ����ǩ����Կ��
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
		
	// ������Կ�Ե�ģ��
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

	// ������Կ��
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

// ���������Կ��
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

	// �������
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

	// ģ
	memcpy(btModulus, lpKeyPair + ulIndex, ulByteLen);
	ulIndex += ulByteLen;

	// ��Կָ��
	memcpy(btPubExponent, lpKeyPair + ulIndex, 4);
	ulIndex += 4;
	
	// ˽Կָ��
	memcpy(btPriExponent, lpKeyPair + ulIndex, ulByteLen);
	ulIndex += ulByteLen;

	// ˽ԿPrime1
	memcpy(btPrime1, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;

	// ˽ԿPrime2
	memcpy(btPrime2, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;
		
	// ˽ԿPrime1Exponent
	memcpy(btExponent1, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;

	// ˽ԿPrime1Exponent
	memcpy(btExponent2, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;
	
	// ˽ԿCoefficient
	memcpy(btCoefficient, lpKeyPair + ulIndex, ulPrimeLen);
	ulIndex += ulPrimeLen;

	// ������ݳ���
	if (ulIndex != ulKeyPairLen)
	{
		USBKey_LogErr("Key pair data lenght is wrong! ulKeyPairLen = 0x%x", ulKeyPairLen);
		return USBKEY_ERR_INDATAERROR;
	}

	// ������Կ�����ģ��	
	CK_ATTRIBUTE publicKeyObjTemplate[ 11 ] = {
		{ CKA_CLASS, (unsigned char *)&ckoPubKey, sizeof(ckoPubKey) },	//�������
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },					//��Token����
		{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },				//�ǹ��ж���
		{ CKA_KEY_TYPE, (unsigned char *)&ckkRsa, sizeof(ckkRsa) },		//��Կ�㷨����
		{ CKA_ID, (unsigned char *)csContainer, strlen(csContainer) },	//��Կ����������
		{ CKA_VERIFY, (unsigned char *)&falseValue, 1 },				//�Ƿ����������֤ǩ��
		{ CKA_ENCRYPT, (unsigned char *)&trueValue, 1 },				//�Ƿ����������������
		{ CKA_WRAP, (unsigned char *)&trueValue, 1 },					//�Ƿ��������Wrap Key
		{ CKA_MODULUS, btModulus, ulByteLen },							//ģ��n
		{ CKA_MODULUS_BITS, &ulBits, 4 },								//ģ��n��λ����
		{ CKA_PUBLIC_EXPONENT, btPubExponent, 4 }						//����ָ��e
	};
	
	// ����˽Կ����ģ��
	CK_ATTRIBUTE privateKeyObjTemplate[ 14 ] = {
		{ CKA_CLASS, (unsigned char *)&ckoPriKey, sizeof(ckoPriKey) },	//�������
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },					//��Token����
		{ CKA_PRIVATE, (unsigned char *)&trueValue, 1 },				//��˽�ж���
		{ CKA_KEY_TYPE, (unsigned char *)&ckkRsa, sizeof(ckkRsa) },		//��Կ�㷨����
		{ CKA_ID, (unsigned char *)csContainer, strlen(csContainer) },	//��Կ����������
		{ CKA_SIGN, (unsigned char *)&falseValue, 1 },					//�Ƿ��������ǩ��
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },				//�Ƿ����������������
		{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },					//�Ƿ��������Unwrap Key		
		{ CKA_PRIME_1, btPrime1, ulPrimeLen },							//Prime1
		{ CKA_PRIME_2, btPrime2, ulPrimeLen },							//Prime2
		{ CKA_PRIVATE_EXPONENT, btPriExponent, ulByteLen },				//Private Exponent
		{ CKA_EXPONENT_1, btExponent1, ulPrimeLen },					//Exponent1
		{ CKA_EXPONENT_2, btExponent2, ulPrimeLen },					//Exponent2
		{ CKA_COEFFICIENT, btCoefficient, ulPrimeLen }					//Coefficient
	};

	//������Կ����
	rv = m_pFunctionList->C_CreateObject((CK_SESSION_HANDLE)hSession, publicKeyObjTemplate, 11, &hPubKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("Call C_CreateObject() to import public key failed! rv = 0x%x", rv);
		return rv;
	}
	
	//����˽Կ����
	rv = m_pFunctionList->C_CreateObject((CK_SESSION_HANDLE)hSession, privateKeyObjTemplate, 14, &hPriKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("Call C_CreateObject() to import private key failed! rv = 0x%x", rv);
		return rv;
	}
	
	return rv;
}

// ������Կ����
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

	// ��������Ҫ��buffer����
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

	// ���ҹ�Կ����
	hPubKeyObj = _FindPubKeyObj((CK_SESSION_HANDLE)hSession, !bSign, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
	if (NULL == hPubKeyObj)
	{
		USBKey_LogErr("No any %s public key object!", bSign ? "sign" : "exch");
		return USBKEY_ERR_FAILED;
	}

	// ��ȡ��Կ����
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

	// ��������
	memcpy(lpPubKey, &ulBits, 4);
	memcpy(lpPubKey + 4, btModulus, ulBits / 8);
	memcpy(lpPubKey + 4 + ulBits / 8, btExponent, 4);

	return USBKEY_ERR_OK;
}

// ���Լ�������-��������
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
		
	// ʹ��RSA��Կ����
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
	// ����һ���Գ���Կ����
	else
	{		
		// �����Ự��Կģ��
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
		
	// ���ܳ�ʼ��
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

// ���Դ�������ŷ�����-���������ŷ�
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
			
	// ���Ҽ��ܹ�Կ
	hExchPubKey = _FindPubKeyObj((CK_SESSION_HANDLE)hSession, TRUE, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4); 
	if (!hExchPubKey)
	{
		USBKey_LogErr("No encrypt public key!");
		return USBKEY_ERR_NO_KEYSET;
	}

	// �����Ự��Կģ��
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
	
	// �����������ĵ�Buffer
	ulEnveloppedLen = 0;
	lpEnvelopped = new BYTE[strlen((CHAR*)lpData) + 512];

	// ʹ�ü��ܹ�ԿWrap�Ự��Կ
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
	
	// ʹ�ûỰ��Կ��������
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
		
	// ����Unwrap�Ự��Կģ��
	CK_ATTRIBUTE unwrapAttrs[ 8 ] = 
	{
		{ CKA_CLASS, (unsigned *)&ckoSecretKey, sizeof(unsigned) },
		{ CKA_KEY_TYPE, &ulKeyType, sizeof(int) },
		{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
		{ CKA_DECRYPT, (unsigned char *)&trueValue, 1 },
		{ CKA_UNWRAP, (unsigned char *)&trueValue, 1 },
	};
	
	// ���ҽ���˽Կ
	hExchPriKey = _FindPriKeyObj((CK_SESSION_HANDLE)hSession, FALSE, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
	if (NULL == hExchPriKey)
	{
		USBKey_LogErr("No key set!");
		rv = USBKEY_ERR_NO_KEYSET;
		goto ERR;
	}

	//�����ܻỰ��Կ
	memcpy(&ulWrappedLen, lpEnvelopped, 4);
	unwrapMech.mechanism = CKM_RSA_PKCS;
	rv = m_pFunctionList->C_UnwrapKey((CK_SESSION_HANDLE)hSession, &unwrapMech, hExchPriKey, lpEnvelopped + 4, ulWrappedLen, unwrapAttrs, 5, &hSymmKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_UnwrapKey() failed! rv = 0x%x", rv);
		goto ERR;
	}		

	// ʹ�ûỰ��Կ��������
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

// ����ǩ����ǩ
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

	// ���úϷ��Լ��
	if (!m_pFunctionList || !hSession)
	{
		USBKey_LogErr("m_pFunctionList or hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	
	//	�������
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

	// ����ǩ��˽Կ
	hSignPriKey = _FindPriKeyObj((CK_SESSION_HANDLE)hSession, TRUE, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
	if (!hSignPriKey)
	{
		USBKey_LogErr("No key set!");
		return USBKEY_ERR_NO_KEYSET;
	}
		
	// ǩ����ʼ��
	rv = m_pFunctionList->C_SignInit((CK_SESSION_HANDLE)hSession, &signMech, hSignPriKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_SignInit() failed! rv = 0x%x", rv);
		goto ERR;
	}
	
	// ʹ��ǩ��˽Կ��HASHֵǩ��
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

	// ����ǩ����Կ
	hVerifyPubKey = _FindPubKeyObj((CK_SESSION_HANDLE)hSession, FALSE, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
	if (NULL == hVerifyPubKey)
	{
		USBKey_LogErr("No key set!");
		rv = USBKEY_ERR_NO_KEYSET;
		goto ERR;
	}
	
	// ��ʼ��
	rv = m_pFunctionList->C_VerifyInit((CK_SESSION_HANDLE)hSession, &signMech, hVerifyPubKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_VerifyInit() failed! rv = 0x%x", rv);
		goto ERR;
	}
	
	// ʹ��ǩ��˽Կ��HASHֵǩ��	
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


// �ȴ��豸�¼�
ULONG CPkcs11SDKTester::WaitForDevEvent(LPTSTR szDevName, 
										ULONG *pulDevNameLen, 
										ULONG *pulSlotID, 
										ULONG *pulEvent)
{
	CK_RV rv = CKR_OK;
	CK_ULONG ulEventSlotId = 0;
	CK_ULONG ulEventType = 0;
	
	// ���úϷ��Լ��
	if (!m_pFunctionList)
	{
		USBKey_LogErr("m_pFunctionList is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
	
	//	�������
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
	if ((CKR_OK == rv) && (ulEventSlotId >= 0))	//���¼�����
	{
		*pulSlotID = ulEventSlotId;
		*pulEvent = ulEventType;
	}
	
	return rv;
}

// ȡ���ȴ�(PKCS11��֧��)
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

//	���ҹ�Կ����
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

//	����˽Կ����
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

// ����ָ�����ļ���������Data File����
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

//	����������ϣ����
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

	// ��ʼ��
	rv = m_pFunctionList->C_DigestInit((CK_SESSION_HANDLE)hSession, &hashMech);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DigestInit() is failed! rv = 0x%x", rv);
		return rv;
	}

	// ���Hash
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

	// ��ȡHash���
	rv = m_pFunctionList->C_DigestFinal(hSession, lpHashValue, pHashLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_DigestFinal() is failed! rv = 0x%x", rv);
		return rv;
	}
	
	return rv;
}