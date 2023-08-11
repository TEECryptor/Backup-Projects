#include "StdAfx.h"
#include "Pkcs11SDKTester.h"
#include "Defs.h"
#include "../../Common/SynoConvert.h"

const unsigned char trueValue = 1, falseValue = 0;
const unsigned ckkRsa = CKK_RSA;
const unsigned ckoPubKey = CKO_PUBLIC_KEY;
const unsigned ckoPriKey = CKO_PRIVATE_KEY;
const unsigned ckoSecretKey = CKO_SECRET_KEY;


CPkcs11SDKTester::CPkcs11SDKTester(void)
 : m_hInstance(NULL)
 , m_ulSlotIDCnt(0)
 , m_pSlotIDList(NULL)
 , m_pFunctionList(NULL)
 , m_hSession(NULL)
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
	// �رջỰ
	rv = m_pFunctionList->C_CloseSession(hSession);
	hSession = NULL;
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
ULONG CPkcs11SDKTester::OpenSession(ULONG *phSession)
{
	CK_RV rv = CKR_OK;
	CK_SLOT_ID selSlotID = 0;
	
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

	// ��ǰ�д򿪵ĻỰ
	if (NULL != m_hSession)
	{
		USBKey_LogErr("m_hSession is not NULL!");
		return rv;
	}
		
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}
	
	// �򿪻Ự
	rv = m_pFunctionList->C_OpenSession(selSlotID, CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &m_hSession);
	if (CKR_SESSION_EXISTS == rv)
	{
		rv = m_pFunctionList->C_CloseAllSessions(selSlotID);
		rv |= m_pFunctionList->C_OpenSession(selSlotID, CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &m_hSession);
	}
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_OpenSession() failed! rv = 0x%x", rv);
		return rv;
	}

	return rv;
}

// ��ȡ�Ự��Ϣ
ULONG CPkcs11SDKTester::GetSessionInfo(ULONG hSession)
{
	CK_RV rv = CKR_OK;
	CK_SLOT_ID selSlotID = 0;
	CK_SESSION_INFO sessionInfo = {0};
	TCHAR tcInfo[512] = {0};
	
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

	// ��ǰû�д򿪵ĻỰ
	if (NULL == m_hSession)
	{
		USBKey_LogErr("m_hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
		
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}

	// �رջỰ
	rv = m_pFunctionList->C_GetSessionInfo(m_hSession, &sessionInfo);
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
	CK_SLOT_ID selSlotID = 0;
	
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

	// ��ǰû�д򿪵ĻỰ
	if (NULL == m_hSession)
	{
		USBKey_LogErr("m_hSession is NULL!");
		return rv;
	}
		
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}

	// �رջỰ
	rv = m_pFunctionList->C_CloseSession(m_hSession);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_CloseSession() failed! rv = 0x%x", rv);
		return rv;
	}

	m_hSession = NULL;

	return rv;
}

// ��¼�Ự
ULONG CPkcs11SDKTester::LoginSession(ULONG hSession, 
									 ULONG ulUserType, 
									 LPCTSTR lpcsPIN, 
									 ULONG* pulLeftTimes)
{
	CK_RV rv = CKR_OK;
	CK_SLOT_ID selSlotID = 0;
	CK_USER_TYPE userType = 0;
	CK_UTF8CHAR_PTR utf8Pin = NULL;
	
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

	// ��ǰû�д򿪵ĻỰ
	if (NULL == m_hSession)
	{
		USBKey_LogErr("m_hSession is NULL!");
		return USBKEY_ERR_INVALIDCALL;
	}
		
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
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
	rv = m_pFunctionList->C_Login(m_hSession, userType, utf8Pin, strlen((char*)utf8Pin));
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Login() failed! rv = 0x%x", rv);
		goto FREE_MEMORY;
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
	CK_SLOT_ID selSlotID = 0;
	
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

	// ��ǰû�д򿪵ĻỰ
	if (NULL == m_hSession)
	{
		USBKey_LogErr("m_hSession is NULL!");
		return rv;
	}
		
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}
		
	// ��¼
	rv = m_pFunctionList->C_Logout(m_hSession);
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
	CK_SLOT_ID selSlotID = 0;
	CK_USER_TYPE userType = 0;
	CK_UTF8CHAR_PTR utf8OldPin = NULL;
	CK_UTF8CHAR_PTR utf8NewPin = NULL;
	
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

	// ��ǰû�д򿪵ĻỰ
	if (NULL == m_hSession)
	{
		USBKey_LogErr("m_hSession is NULL!");
		return rv;
	}
		
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
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
	rv = m_pFunctionList->C_SetPIN(m_hSession, utf8OldPin, strlen((char*)utf8OldPin), utf8NewPin, strlen((char*)utf8NewPin));
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
	CK_SLOT_ID selSlotID = 0;
	CK_UTF8CHAR_PTR utf8SOPin = NULL;
	CK_UTF8CHAR_PTR utf8NewPin = NULL;
	
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

	// ��ǰû�д򿪵ĻỰ
	if (NULL == m_hSession)
	{
		USBKey_LogErr("m_hSession is NULL!");
		return rv;
	}
		
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}

#ifdef UNICODE
	utf8SOPin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsSOPIN);
	utf8NewPin = (CK_UTF8CHAR_PTR)ConvertUNICODE2UTF8(lpcsNewUserPIN);
#else
	utf8SOPin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsSOPIN);
	utf8NewPin = (CK_UTF8CHAR_PTR)ConvertASCII2UTF8(lpcsNewUserPIN);
#endif	//UNICODE

	// SO��¼
	rv = m_pFunctionList->C_Login(m_hSession, CKU_SO, utf8SOPin, strlen((char*)utf8SOPin));
	if (CKR_OK != rv && CKR_USER_ALREADY_LOGGED_IN != rv)
	{
		USBKey_LogErr("C_Login() failed! rv = 0x%x", rv);
		goto FREE_MEMORY;
	}

	// ����User��ʼPIN
	rv = m_pFunctionList->C_InitPIN(m_hSession, utf8NewPin, strlen((char*)utf8NewPin));
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

// ����ǩ����Կ��
ULONG CPkcs11SDKTester::GenerateKeyPair(ULONG hSession,
										ULONG ulAlgID, 
										ULONG ulBits)
{
	CK_RV rv = CKR_OK;
	CK_SLOT_ID selSlotID = 0;
	CK_SESSION_HANDLE hSession = NULL_PTR;
	CK_OBJECT_HANDLE hPubKeyObj = NULL_PTR;
	CK_OBJECT_HANDLE hPriKeyObj = NULL_PTR;
	CK_CHAR_PTR keyContainer = (CK_CHAR_PTR)"SynoRSAContainer";
	
	if (!lpcsDevName || _tcslen(lpcsDevName) == 0)
	{
		USBKey_LogErr("lpcsDevName is NULL!");
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
	
	// ����DevName���ҵ�Slot ID
	rv = _FindSlotIDByDevName(lpcsDevName, selSlotID);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("_FindSlotIDByDevName() failed! rv = 0x%x", rv);
		return rv;
	}
	
	// �򿪻Ự
	rv = m_pFunctionList->C_OpenSession(selSlotID, CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &hSession);
	if (CKR_SESSION_EXISTS == rv)
	{
		rv = m_pFunctionList->C_CloseAllSessions(selSlotID);
		rv |= m_pFunctionList->C_OpenSession(selSlotID, CKF_SERIAL_SESSION|CKF_RW_SESSION, NULL, NULL, &hSession);
	}
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_OpenSession() failed! rv = 0x%x", rv);
		return rv;
	}
	
	// ������Կ�Ե�ģ��
	CK_MECHANISM mech = { CKM_RSA_PKCS_KEY_PAIR_GEN, 0, 0 };
	unsigned char pubExponent[ 3 ] = { 0x01, 0x00, 0x01 };
	CK_ATTRIBUTE pubAttrs[ 7 ] = {
			{ CKA_KEY_TYPE, (unsigned *)&ckkRsa, sizeof(unsigned) },
			{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
			{ CKA_PRIVATE, (unsigned char *)&falseValue, 1 },
			{ CKA_VERIFY, (unsigned char *)&trueValue, 1 },
			{ CKA_PUBLIC_EXPONENT, &pubExponent, 3 },
			{ CKA_MODULUS_BITS, &ulBits, 4 },
			{ CKA_LABEL, (void *)keyContainer, strlen((char*)keyContainer) } };
	CK_ATTRIBUTE privAttrs[ 5 ] = {
			{ CKA_KEY_TYPE, (unsigned *)&ckkRsa, sizeof(unsigned) },
			{ CKA_TOKEN, (unsigned char *)&trueValue, 1 },
			{ CKA_PRIVATE, (unsigned char *)&trueValue, 1 },
			{ CKA_SIGN, (unsigned char *)&trueValue, 1 },
			{ CKA_LABEL, (void *)keyContainer, strlen((char*)keyContainer) } };

	// ������Կ��
	rv = m_pFunctionList->C_GenerateKeyPair(hSession, &mech, pubAttrs, 7, privAttrs, 5, &hPubKeyObj, &hPriKeyObj); 
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_GenerateKeyPair() failed! rv = 0x%x", rv);
		goto ERR;
	}

ERR:
	if (hSession)
	{
		m_pFunctionList->C_CloseSession(hSession);
		hSession = NULL;
	}
	if (hPubKeyObj)
	{
		m_pFunctionList->C_DestroyObject(hSession, hPubKeyObj);
		hPubKeyObj = NULL;
	}
	if (hPriKeyObj)
	{
		m_pFunctionList->C_DestroyObject(hSession, hPriKeyObj);
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
	const CHAR csContainer[] = {"P11_Con"};

	// �������
	if (!lpcsDevName || _tcslen(lpcsDevName) == 0)
	{
		USBKey_LogErr("lpcsDevName is NULL!");
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
	if (NULL == lpKeyPair)
	{
		USBKey_LogErr("lpKeyPair is NULL!");
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
	lpKeyPair += 4;
	
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
	rv = m_pFunctionList->C_CreateObject(m_hSession, publicKeyObjTemplate, 11, &hPubKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("Call C_CreateObject() to import public key failed! rv = 0x%x", rv);
		return rv;
	}
	
	//����˽Կ����
	rv = m_pFunctionList->C_CreateObject(m_hSession, privateKeyObjTemplate, 14, &hPriKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("Call C_CreateObject() to import private key failed! rv = 0x%x", rv);
		return rv;
	}
	
	return USBKEY_ERR_FAILED;
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

	if (!lpcsDevName || _tcslen(lpcsDevName) == 0)
	{
		USBKey_LogErr("lpcsDevName is NULL!");
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
	hPubKeyObj = _FindPubKeyObj(m_hSession, !bSign, CKA_KEY_TYPE, (LPBYTE)&ckkRsa, 4);
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
	rv = m_pFunctionList->C_GetAttributeValue(m_hSession, hPubKeyObj, PubKeyAttrs, 3);
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
	return USBKEY_ERR_OK;
}

// ���Դ�������ŷ�����-���������ŷ�
ULONG CPkcs11SDKTester::TestEnvelop(ULONG hSession, 
									ULONG ulAlgID, 
									LPCTSTR lpcsMsg)
{
	return USBKEY_ERR_OK;
}

// ����ǩ����ǩ
ULONG CPkcs11SDKTester::TestSign(ULONG hSession, 
								 ULONG ulAlgID, 
								 LPCTSTR lpcsMsg)
{
	return USBKEY_ERR_OK;
}

// ��������
ULONG CPkcs11SDKTester::Encrypt(ULONG hSession, 
								ULONG ulAlgID, 
								LPBYTE lpData, 
								ULONG ulDataLen, 
								LPBYTE lpEncrypted, 
								ULONG *pulEncryptedLen)
{
	CK_RV rv = 0;
	CK_ULONG ulSize = 0;
	CK_ULONG ulKeyLen = 0;
	CK_ULONG ulResultLen = 0;
	CK_KEY_TYPE ulKeyType = 0;
	CK_OBJECT_HANDLE hEncryptKey = NULL;
	CK_MECHANISM keyGenMech = { 0, 0, 0 };
	CK_MECHANISM keyEncMech = { 0, 0, 0 };

	if (!lpcsDevName || _tcslen(lpcsDevName) == 0)
	{
		USBKey_LogErr("lpcsDevName is NULL!");
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
	if (!lpData || 0 == ulDataLen)
	{
		USBKey_LogErr("lpData is NULL or ulDataLen is 0!");
		return USBKEY_ERR_INVALIDPARAM;
	}
	if (!lpEncrypted || !pulEncryptedLen)
	{
		USBKey_LogErr("lpEncrypted or pulEncryptedLen is NULL!");
		return USBKEY_ERR_INVALIDPARAM;
	}
		
	// ʹ��RSA��Կ����
	if (USBKEY_ASYMMALG_RSA == ulAlgID)
	{
		ULONG ulBits = 0;

		ulSize = 4;
		hEncryptKey = _FindPubKeyObj(m_hSession, TRUE, CK_KEY_TYPE, &ulKeyType, &ulSize); 
		if (!hEncryptKey)
		{
			USBKey_LogErr("No encrypt public key!");
			return USBKEY_ERR_NO_KEYSET;
		}
		
		CK_ATTRIBUTE pubKeyAttrs[ 1 ] = 
		{
			CKA_MODULUS_BITS, &ulBits, 4)
		};

		rv = C_GetAttributeValue(m_hSession, hEncryptKey, pubKeyAttrs, 1);
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_GetAttributeValue() failed! rv = 0x%x", rv);
			return rv;
		}
		if (ulDataLen != (ulBits/8) - 11)
		{
			USBKey_LogErr("Data length is wrong! ulDataLen = 0x%x", ulDataLen);
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
		rv = m_pFunctionList->C_GenerateKey(m_hSession, &keyGenMech, symmAttrs, 9, &hEncryptKey); 
		if (CKR_OK != rv)
		{
			USBKey_LogErr("C_GenerateKey() failed! rv = 0x%x", rv);
			goto ERR;
		}
	}

	rv = C_EncryptInit(m_hSession, keyEncMech, hEncryptKey);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_EncryptInit() failed! rv = 0x%x", rv);
		goto ERR;
	}

	rv = C_Encrypt(m_hSession, lpData, ulDataLen, NULL, &ulResultLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Encrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}
	if (*pulEncryptedLen < ulResultLen)
	{
		USBKey_LogErr("Buffer is to small! Requested lenght: 0x%x", ulResultLen);
		rv = USBKEY_ERR_BUFFER_TOO_SMALL;
		goto ERR;
	}
	
	rv = C_Encrypt(m_hSession, lpData, ulDataLen, lpEncrypted, *pulEncryptedLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Encrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}

	rv = C_EncryptFinal(m_hSession, lpEncrypted + *pulEncryptedLen, &ulResultLen);
	if (CKR_OK != rv)
	{
		USBKey_LogErr("C_Encrypt() failed! rv = 0x%x", rv);
		goto ERR;
	}
	*pulEncryptedLen += ulResultLen;
	
ERR:
	if (hEncryptKey && (CKK_RSA != ulKeyType))
	{
		C_DestroyObject(m_hSession, hEncryptKey);
	}
	hEncryptKey = NULL;

	return rv;
}

// ��������
ULONG CPkcs11SDKTester::Decrypt(LPCTSTR lpcsDevName, 
								ULONG ulAlgID, 
								LPBYTE lpEncrypted, 
								ULONG ulEncryptedLen, 
								LPBYTE lpData, 
								ULONG *pulDataLen)
{
	return USBKEY_ERR_OK;
}

// �������ŷ��ʽ��������
ULONG CPkcs11SDKTester::Envelop(LPCTSTR lpcsDevName, 
								ULONG ulAlgID, 
								LPBYTE lpData, 
								ULONG ulDataLen, 
								LPBYTE lpEnveloped, 
								ULONG *pulEnvelopedLen)
{
	return USBKEY_ERR_OK;
}

// ���������ŷ�����
ULONG CPkcs11SDKTester::Develop(LPCTSTR lpcsDevName, 
								ULONG ulAlgID, 
								LPBYTE lpEnveloped, 
								ULONG ulEnvelopedLen, 
								LPBYTE lpData, 
								ULONG *pulDataLen)
{
	return USBKEY_ERR_OK;
}

// ǩ������
ULONG CPkcs11SDKTester::Sign(LPCTSTR lpcsDevName, 
							 ULONG ulAlgID, 
							 LPBYTE lpData, 
							 ULONG ulDataLen, 
							 LPBYTE lpUserID, 
							 ULONG ulUserIDLen, 
							 LPBYTE lpSignature, 
							 ULONG *pulSignatureLen)
{
	return USBKEY_ERR_OK;
}

// ��ǩ����
ULONG CPkcs11SDKTester::Verify(LPCTSTR lpcsDevName, 
								ULONG ulAlgID, 
								LPBYTE lpData, 
								ULONG ulDataLen, 
								LPBYTE lpUserID, 
								ULONG ulUserIDLen, 
								LPBYTE lpSignature, 
								ULONG ulSignatureLen)
{
	return USBKEY_ERR_OK;
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