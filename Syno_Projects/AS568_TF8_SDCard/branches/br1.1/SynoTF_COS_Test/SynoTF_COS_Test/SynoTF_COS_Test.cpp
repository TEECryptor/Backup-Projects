// SynoTF_COS_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include "SynoTFCosApi.h"

#define	MAX_DEV_CNT		10
#define	MAX_APP_CNT		8
#define	RSA_CON_NAME	"RSA_CON"
#define	ECC_CON_NAME	"ECC_CON"
#define ECC_MAX_BITS	512
#define	TOTAL_DATA_LEN		(1024*1024)
#define PROC_DATA_LEN		MAX_DATA_BUF
#define HASH_DATA_LEN		2048
#define PADDED_DATA_LEN		64

#define Random(x)			(rand()%x)

CHAR		g_csDevNames[MAX_DEV_CNT][128] = {0};
CHAR		g_csAppNames[MAX_APP_CNT][64] = {0};
ULONG		g_ulSelDevIndex = 0;
ULONG		g_ulSelAppIndex = 0;
BYTE		g_btPlainData[TOTAL_DATA_LEN] = {0};
BYTE		g_btCipherData[TOTAL_DATA_LEN + 32] = {0};
BYTE		g_btDecryptedData[TOTAL_DATA_LEN + 32] = {0};

void test_DoDevAuth(DEVHANDLE hDev);
void test_ChangeDevAuthKey(DEVHANDLE hDev);
void test_SetLabel(DEVHANDLE hDev);
void test_Initliaze(DEVHANDLE hDev);
void test_DeleteApp(DEVHANDLE hDev);
void test_ChangePIN(DEVHANDLE hDev);
void test_UnblockPIN(DEVHANDLE hDev);
void test_FormatDev(DEVHANDLE hDev);
//
void test_GenExtRsaKeyPair(DEVHANDLE hDev);
void test_ImportPlainRsaKeyPair(DEVHANDLE hDev);
void test_RsaEncryptDecrypt(DEVHANDLE hDev);
void test_RsaSignVerify(DEVHANDLE hDev);
void test_ExtRsaKeyOPT(DEVHANDLE hDev);
//
void test_ImportEccKeyPair(DEVHANDLE hDev);
void test_EccEncryptDecrypt(DEVHANDLE hDev);
void test_EccSignVerify(DEVHANDLE hDev);
void test_ExtEccKeyOPT(DEVHANDLE hDev);
//
void test_SymmKeyData(DEVHANDLE hDev);
void test_HashData(DEVHANDLE hDev);
//
ULONG helper_SelectDevice();
ULONG helper_SelectApplication(DEVHANDLE hDev);
ULONG helper_WriteData(FILE* file, BYTE* lpData, ULONG ulDataLen);
ULONG helper_ReadData(FILE* file, BYTE* lpData, ULONG *pulDataLen);

int _tmain(int argc, _TCHAR* argv[])
{
	ULONG ulRes = 0;
	ULONG ulDevCount = 0;
	DEVHANDLE hDev = NULL;
	BYTE btInput = 0;

	//	选择设备
	g_ulSelDevIndex = helper_SelectDevice();
	if (-1 == g_ulSelDevIndex)
	{
		goto END;
	}

	//	打开设备
	ulRes = SKF_ConnectDev(g_csDevNames[g_ulSelDevIndex], &hDev);
	if (0 != ulRes || NULL == hDev)
	{
		printf("SKF_ConnectDev() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	选择测试项
	printf("----------------Test Case--------------\n");
	printf("  1: Do device authentication\n");
	printf("  2: Change device authentication key\n");
	printf("  3: Set label to device\n");
	printf("  4: Intilaize device\n");
	printf("  5: Delete application\n");
	printf("  6: Change PIN\n");
	printf("  7: Unblock PIN\n");
	printf("  8: Generate an extern RSA key pair in plain text\n");
	printf("  9: Import an extern RSA key pair in plain text\n");
	printf("  A: Test RSA and SM1_ECB data encrypt/decrypt\n");
	printf("  B: Test RSA and SHA1 data sign/verify\n");
	printf("  C: Test extern RSA key pair options\n");
	printf("  D: Import ECC key pair\n");
	printf("  E: Test ECC and SM1_ECB data encrypt/decrypt\n");
	printf("  F: Test ECC and SM3 data sign/verify\n");
	printf("  G: Test extern ECC key pair options\n");
	printf("  I: Test symm key encrypt/decrypt data\n");
	printf("  J: Test hash data\n");
	printf("  R: Refresh device name list\n");
	printf("  X: Format device\n");
	printf("  H: Help\n");
	printf("  Q: Exit\n");
	printf("--------------------------------------\n");
	printf("Please select a test option:");
	do
	{
		btInput = _getch();
		printf("%c\n", btInput);
		switch(btInput)
		{
		case '1':
			test_DoDevAuth(hDev); break;
		case '2':
			test_ChangeDevAuthKey(hDev); break;
		case '3':
			test_SetLabel(hDev); break;
		case '4':
			test_Initliaze(hDev); break;
		case '5':
			test_DeleteApp(hDev); break;
		case '6':
			test_ChangePIN(hDev); break;
		case '7':
			test_UnblockPIN(hDev); break;
		case '8':
			test_GenExtRsaKeyPair(hDev); break;
		case '9':
			test_ImportPlainRsaKeyPair(hDev); break;
		case 'A':
		case 'a':
			test_RsaEncryptDecrypt(hDev); break;
		case 'B':
		case 'b':
			test_RsaSignVerify(hDev); break;
		case 'C':
		case 'c':
			test_ExtRsaKeyOPT(hDev); break;
		case 'D':
		case 'd':
			test_ImportEccKeyPair(hDev); break;
		case 'E':
		case 'e':
			test_EccEncryptDecrypt(hDev); break;
		case 'F':
		case 'f':
			test_EccSignVerify(hDev); break;
		case 'G':
		case 'g':
			test_ExtEccKeyOPT(hDev); break;
		case 'I':
		case 'i':
			test_SymmKeyData(hDev); break;
		case 'J':
		case 'j':
			test_HashData(hDev); break;
		case 'X':
		case 'x':
			test_FormatDev(hDev); break;
		case 'r':
		case 'R':
			g_ulSelDevIndex = helper_SelectDevice();
			if (-1 == g_ulSelDevIndex)
			{
				goto END;
			}
			if (hDev)
			{
				ulRes = SKF_DisConnectDev(hDev);
				hDev = NULL;
			}
			ulRes |= SKF_ConnectDev(g_csDevNames[g_ulSelDevIndex], &hDev);
			if (0 != ulRes || NULL == hDev)
			{
				printf("SKF_ConnectDev() failed! ulRes = 0x%x\n", ulRes);
				goto END;
			}
		case 'h':
		case 'H':			
			printf("----------------Test Case--------------\n");
			printf("  1: Do device authentication\n");
			printf("  2: Change device authentication key\n");
			printf("  3: Set label to device\n");
			printf("  4: Intilaize device\n");
			printf("  5: Delete application\n");
			printf("  6: Change PIN\n");
			printf("  7: Unblock PIN\n");
			printf("  8: Generate an extern RSA key pair in plain text\n");
			printf("  9: Import an extern RSA key pair in plain text\n");
			printf("  A: Test RSA and SM1_ECB data encrypt/decrypt\n");
			printf("  B: Test RSA and SHA1 data sign/verify\n");
			printf("  C: Test extern RSA key pair options\n");
			printf("  D: Import ECC key pair\n");
			printf("  E: Test ECC and SM1_ECB data encrypt/decrypt\n");
			printf("  F: Test ECC and SM3 data sign/verify\n");
			printf("  G: Test extern ECC key pair options\n");
			printf("  I: Test symm key encrypt/decrypt data\n");
			printf("  J: Test hash data\n");
			printf("  R: Refresh device name list\n");
			printf("  X: Format device\n");
			printf("  H: Help\n");
			printf("--------------------------------------\n");
			break;
		case 'q':
		case 'Q':
			goto END;
		}
		printf("\nPlease select a test option:");

	}while(1);


END:
	if (hDev)
	{
		SKF_DisConnectDev(hDev);
		hDev = NULL;
	}

	printf("Test is end!\n");
	getchar();

	return 0;
}

void test_DoDevAuth(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	CHAR csAuthKey[32] = {0};

	printf("Enter the device authentication key (use the default by passing enter key):\n");
	gets_s(csAuthKey, 32);

	if (!SKF_ExterAuth(hDev, strlen(csAuthKey) ? (UCHAR*)csAuthKey : NULL, strlen(csAuthKey)))
	{
		printf("SKF_ExterAuth() failed!\n");
		return;
	}

	printf("test_DoDevAuth() pass!\n");
}

void test_ChangeDevAuthKey(DEVHANDLE hDev)
{	
	ULONG ulRes = 0;
	CHAR csAuthKey[32] = {0};

	printf("Please enter the new device authentication key:");
	gets_s(csAuthKey, 32);
	if (strlen(csAuthKey) == 0)
	{
		printf("The input key should not be NULL!\n");
		return;
	}

	ulRes = SKF_ChangeDevAuthKey(hDev, (UCHAR*)csAuthKey, strlen(csAuthKey));
	if (0 != ulRes)
	{
		printf("SKF_ChangeDevAuthKey() failed! ulRes = 0x%x\n", ulRes);
		return;
	}

	printf("test_ChangeDevAuthKey() pass!\n");
}

void test_SetLabel(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	CHAR csLabel[32 + 1] = {0};

	printf("Please enter the new device label:\n");
	gets_s(csLabel, 32);
	if (strlen(csLabel) == 0 || strlen(csLabel) > 32)
	{
		printf("The input Label length should be (0, 32]!\n");
		return;
	}

	ulRes = SKF_SetLabel(hDev, csLabel);
	if (0 != ulRes)
	{
		printf("SKF_SetLabel() failed! ulRes = 0x%x\n", ulRes);
		return;
	}

	printf("test_SetLabel() pass!\n");
}

void test_Initliaze(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	BYTE btAppType = 0;
	CHAR csAppName[64] = {0};
	CHAR csSOPin[16] = {0};
	CHAR csUserPin[16] = {0};
	ULONG ulRetryTime = 0;
	HAPPLICATION hApp = NULL;
	HCONTAINER hRsaContainer = NULL;
	HCONTAINER hEccContainer = NULL;
	RSAPUBLICKEYBLOB stRsaPubKey = {0};
	ECCPUBLICKEYBLOB stEccPubKey = {0};
	
	printf("Please enter the application name:");
	gets_s(csAppName, 48);
	if (strlen(csAppName) == 0 || strlen(csAppName) > 48)
	{
		printf("The input application name length should be (0, 48]!\n");
		return;
	}

	printf("Please enter SO PIN:");
	gets_s(csSOPin, 16);
	if (strlen(csSOPin) == 0 || strlen(csSOPin) > 16)
	{
		printf("The input SO PIN length should be (0, 16]!\n");
		return;
	}

	printf("Please enter User PIN:");
	gets_s(csUserPin, 16);
	if (strlen(csUserPin) == 0 || strlen(csUserPin) > 16)
	{
		printf("The input user PIN length should be (0, 16]!\n");
		return;
	}

	//	创建应用
	ulRes = SKF_CreateApplication(hDev, csAppName, csSOPin, 15, csUserPin, 15, SECURE_USER_ACCOUNT, &hApp);
	if (0 != ulRes)
	{
		printf("SKF_CreateApplication() failed! ulRes = 0x%x\n", ulRes);
		return;
	}

	//	登录应用
	ulRes = SKF_VerifyPIN(hApp, KT_USERPIN, csUserPin, &ulRetryTime);
	if (0 != ulRes)
	{
		printf("SKF_VerifyPIN() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	创建RSA容器
	ulRes = SKF_CreateContainer(hApp, RSA_CON_NAME, &hRsaContainer);
	if (0 != ulRes)
	{
		printf("SKF_CreateContainer() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	生成RSA签名密钥对
	ulRes = SKF_GenRSAKeyPair(hRsaContainer, 1024, &stRsaPubKey);
	if (0 != ulRes)
	{
		printf("SKF_GenRSAKeyPair() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	创建ECC容器
	ulRes = SKF_CreateContainer(hApp, ECC_CON_NAME, &hEccContainer);
	if (0 != ulRes)
	{
		printf("SKF_CreateContainer() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	生成ECC签名密钥对
	ulRes = SKF_GenECCKeyPair(hEccContainer, SGD_SM2_1, &stEccPubKey);
	if (0 != ulRes)
	{
		printf("SKF_GenECCKeyPair() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	printf("test_Initliaze() pass!\n");
END:
	if (hEccContainer)
	{
		SKF_CloseContainer(hEccContainer);
		hEccContainer = NULL;
	}
	if (hRsaContainer)
	{
		SKF_CloseContainer(hRsaContainer);
		hRsaContainer = NULL;
	}
	if (hApp)
	{
		SKF_CloseApplication(hApp);
		hApp = NULL;
	}

	return;
}

void test_DeleteApp(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulSelAppIndex = 0;

	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}

	ulRes = SKF_DeleteApplication(hDev, g_csAppNames[ulSelAppIndex]);
	if (0 != ulRes)
	{
		printf("SKF_DeleteApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	printf("test_DeleteApp() pass!\n");
END:
	return;
}

void test_ChangePIN(DEVHANDLE hDev)
{	
	ULONG ulRes = 0;
	ULONG ulSelAppIndex = 0;
	BYTE btPINType = 0;
	CHAR csAppName[64] = {0};
	CHAR csOldPin[16] = {0};
	CHAR csNewPin[16] = {0};
	ULONG ulRetryTime = 0;
	HAPPLICATION hApp = NULL;

	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}

	printf("Select the PIN type [0-SO PIN;1-User PIN]:");
	btPINType = _getch();
	printf("%c\n", btPINType);
	btPINType -= (BYTE)('30');
	if (btPINType != 0 && btPINType != 1)
	{
		printf("The input PIN type should be 0 or 1!\n");
		return;
	}

	printf("Please enter the old PIN:");
	gets_s(csOldPin, 16);
	if (strlen(csOldPin) == 0 || strlen(csOldPin) > 16)
	{
		printf("The input old PIN length should be (0, 16]!\n");
		return;
	}

	printf("Please enter the new PIN:");
	gets_s(csNewPin, 16);
	if (strlen(csNewPin) == 0 || strlen(csNewPin) > 16)
	{
		printf("The input new PIN length should be (0, 16]!\n");
		return;
	}

	//	打开应用
	ulRes = SKF_OpenApplication(hDev, g_csAppNames[ulSelAppIndex], &hApp);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	修改PIN
	ulRes = SKF_ChangePIN(hApp, btPINType == 0 ? KT_SOPIN : KT_USERPIN, 
							csOldPin, csNewPin, &ulRetryTime);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		if ((ulRes & 0x0000FFF0) == 0x000063c0)
		{
			printf("The left retry times: 0x%x\n", ulRetryTime);
		}
		goto END;
	}

	printf("test_ChangePIN() pass!\n");
END:
	SKF_CloseApplication(hApp);
	hApp = NULL;

	return;
}

void test_UnblockPIN(DEVHANDLE hDev)
{	
	ULONG ulRes = 0;
	ULONG ulSelAppIndex = 0;
	BYTE btPINType = 0;
	CHAR csAppName[64] = {0};
	CHAR csSOPin[16] = {0};
	CHAR csUserPin[16] = {0};
	ULONG ulRetryTime = 0;
	HAPPLICATION hApp = NULL;

	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}

	printf("Please enter the SO PIN:");
	gets_s(csSOPin, 16);
	if (strlen(csSOPin) == 0 || strlen(csSOPin) > 16)
	{
		printf("The input SO PIN length should be (0, 16]!\n");
		return;
	}

	printf("Please enter the new user PIN:");
	gets_s(csUserPin, 16);
	if (strlen(csUserPin) == 0 || strlen(csUserPin) > 16)
	{
		printf("The input new user PIN length should be (0, 16]!\n");
		return;
	}

	//	打开应用
	ulRes = SKF_OpenApplication(hDev, g_csAppNames[ulSelAppIndex], &hApp);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	解锁用户PIN
	ulRes = SKF_UnblockPIN(hApp, csSOPin, csUserPin, &ulRetryTime);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		if ((ulRes & 0x0000FFF0) == 0x000063c0)
		{
			printf("The left retry times: 0x%x\n", ulRetryTime);
		}
		goto END;
	}

	printf("test_UnblockPIN() pass!\n");
END:
	SKF_CloseApplication(hApp);
	hApp = NULL;

	return;
}

void test_FormatDev(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	BYTE btInput = 0;
	BYTE btCmd[4] = {0};

	printf("Please select format option[0-Format data;1-Clean programm;2-Erase full flash!!]:");
	btInput = _getch();
	printf("%c\n", btInput);
	btInput = (btInput - 0x30);
	if (btInput != 0 && btInput != 1 && btInput != 2)
	{
		printf("Input option is wrong!\n");
		return;
	}

	ulRes = SKF_DevTransmit(hDev, 0x07, btInput, NULL, 0, NULL, NULL);
	if (0 != ulRes)
	{
		printf("SKF_DeleteApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	printf("test_FormatDev() pass!\n");
END:
	return;
}

void test_GenExtRsaKeyPair(DEVHANDLE hDev)
{	
	ULONG ulRes = 0;
	ULONG ulKeyBits = 0;
	ULONG ulKeyPairLen = 0;
	USHORT usByteLen = 0;
	USHORT usPrimeLen = 0;
	BYTE btInput = 0;
	BYTE *pbtKeyPairData = NULL;
	CHAR csKeyPairFile[256] = {0};
	FILE* pKeypairFile = NULL;
	RSAPRIVATEKEYBLOB stRsaPriKeyBlob = {0};
	
	//	输入Rsa Key bits
	printf("Please selct RSA key bits [1-1024;2-2048]:");	
	btInput = _getch();
	printf("%c\n", btInput);
	btInput -= (BYTE)('30');
	if (1 == btInput)
	{
		ulKeyBits = 1024;
	}
	else if (2 == btInput)
	{
		ulKeyBits = 2048;
	}
	else
	{
		printf("Key bit selection is wrong!\n");
		goto END;
	}

	//	生成并导出Rsa密钥对
	ulRes = SKF_GenExtRSAKey(hDev, ulKeyBits, &stRsaPriKeyBlob);
	if (0 != ulRes)
	{
		printf("SKF_GenExtRSAKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	拼装Rsa密钥对数据
	usByteLen = (USHORT)stRsaPriKeyBlob.BitLen / 8;
	usPrimeLen = usByteLen / 2;
	ulKeyPairLen = usByteLen*2 + usPrimeLen*5 + 6;
	pbtKeyPairData = new BYTE[ulKeyPairLen];
	memset(pbtKeyPairData, 0, ulKeyPairLen);
	//
	ulKeyPairLen = 0;
	memcpy(pbtKeyPairData + ulKeyPairLen, (USHORT *)&stRsaPriKeyBlob.BitLen, 2);
    ulKeyPairLen += 2;
	memcpy(pbtKeyPairData + ulKeyPairLen, stRsaPriKeyBlob.Modulus + MAX_RSA_MODULUS_LEN - usByteLen, usByteLen);
    ulKeyPairLen += usByteLen;
	memcpy(pbtKeyPairData + ulKeyPairLen, stRsaPriKeyBlob.PublicExponent, 4);
    ulKeyPairLen += 4;
	memcpy(pbtKeyPairData + ulKeyPairLen, stRsaPriKeyBlob.PrivateExponent + MAX_RSA_MODULUS_LEN - usByteLen, usByteLen);
    ulKeyPairLen += usByteLen;
	memcpy(pbtKeyPairData + ulKeyPairLen, stRsaPriKeyBlob.Prime1 + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulKeyPairLen += usPrimeLen;
	memcpy(pbtKeyPairData + ulKeyPairLen, stRsaPriKeyBlob.Prime2 + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulKeyPairLen += usPrimeLen;
	memcpy(pbtKeyPairData + ulKeyPairLen, stRsaPriKeyBlob.Prime1Exponent + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulKeyPairLen += usPrimeLen;
	memcpy(pbtKeyPairData + ulKeyPairLen, stRsaPriKeyBlob.Prime2Exponent + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulKeyPairLen += usPrimeLen;
	memcpy(pbtKeyPairData + ulKeyPairLen, stRsaPriKeyBlob.Coefficient + MAX_RSA_PRIME_LEN - usPrimeLen, usPrimeLen);
    ulKeyPairLen += usPrimeLen;

	//	输入保存密钥数据的文件
	printf("Please enter a file name to save key pair data:");
	gets_s(csKeyPairFile, 256);
	ulRes = fopen_s(&pKeypairFile, csKeyPairFile, "w");
	if (!pKeypairFile)
	{
		printf("Open file failed!\n ulRes = 0x%x", ulRes);
		goto END;
	}

	//	写数据
	ulRes = helper_WriteData(pKeypairFile, pbtKeyPairData, ulKeyPairLen);
	if (0 == ulRes)
	{
		printf("Wriate file failed!\n");
		goto END;
	}
	
	printf("test_GenExtRsaKeyPair() pass!\n");
END:
	if (pKeypairFile)
	{
		fclose(pKeypairFile);
		pKeypairFile = NULL;
	}
	if (pbtKeyPairData)
	{
		delete []pbtKeyPairData;
		pbtKeyPairData = NULL;
	}

	return;
}

void test_ImportPlainRsaKeyPair(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulSelAppIndex = 0;
	ULONG ulDataIndex = 0;
	ULONG ulKeyPairLen = 2048;
	BOOL bSignKey = FALSE;
	USHORT usByteLen = 0;
	USHORT usPrimeLen = 0;
	BYTE btInput = 0;
	BYTE btKeyPairData[2048] = {0};
	CHAR csKeyPairFile[256] = {0};
	FILE* pKeypairFile = NULL;
	RSAPRIVATEKEYBLOB stRsaPriKeyBlob = {0};
	HAPPLICATION hApp = NULL;
	HCONTAINER hCon = NULL;

	//	输入密钥对数据文件
	printf("Please enter the plain RSA key pair file:");
	gets_s(csKeyPairFile, 256);
	ulRes = fopen_s(&pKeypairFile, csKeyPairFile, "r");
	if (!pKeypairFile)
	{
		printf("Open file failed!\n ulRes = 0x%x", ulRes);
		goto END;
	}	
	
	//	输入密钥对类型
	printf("Please enter the plain RSA key pair type [1-Sign key; 2-Exch key]:");
	btInput = _getch();
	printf("%c\n", btInput);
	btInput -= (BYTE)('30');
	if (1 == btInput)
	{
		bSignKey = TRUE;
	}
	else if (2 == btInput)
	{
		bSignKey = FALSE;
	}
	else
	{
		printf("Key type selection is wrong!\n");
		goto END;
	}

	//	读取文件数据
	if (!helper_ReadData(pKeypairFile, btKeyPairData, &ulKeyPairLen))
	{
		printf("Read file failed!\n");
		goto END;
	}

	//	构造RSA密钥结构体
	ulDataIndex = 0;
	stRsaPriKeyBlob.BitLen = (btKeyPairData[1] << 8) | btKeyPairData[0];
	ulDataIndex += 2;
	usByteLen = (USHORT)stRsaPriKeyBlob.BitLen / 8;
	usPrimeLen = usByteLen / 2;
	memcpy(stRsaPriKeyBlob.Modulus + MAX_RSA_MODULUS_LEN - usByteLen, btKeyPairData + ulDataIndex, usByteLen);
	ulDataIndex += usByteLen;
	memcpy(stRsaPriKeyBlob.PublicExponent, btKeyPairData + ulDataIndex, 4);
	ulDataIndex += 4;
	memcpy(stRsaPriKeyBlob.PrivateExponent + MAX_RSA_MODULUS_LEN - usByteLen, btKeyPairData + ulDataIndex, usByteLen);
	ulDataIndex += usByteLen;
	memcpy(stRsaPriKeyBlob.Prime1 + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;
	memcpy(stRsaPriKeyBlob.Prime2 + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;
	memcpy(stRsaPriKeyBlob.Prime1Exponent + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;
	memcpy(stRsaPriKeyBlob.Prime2Exponent + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;
	memcpy(stRsaPriKeyBlob.Coefficient + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;

	//	选择应用
	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}

	//	打开应用
	ulRes = SKF_OpenApplication(hDev, g_csAppNames[ulSelAppIndex], &hApp);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	打开容器
	ulRes = SKF_OpenContainer(hApp, RSA_CON_NAME, &hCon);
	if (0 != ulRes)
	{
		printf("SKF_OpenContainer() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	导入明文的Rsa密钥对
	ulRes = SKF_ImportPlainRSAKeyPair(hCon, &stRsaPriKeyBlob, bSignKey);
	if (0 != ulRes)
	{
		printf("SKF_ImportPlainRSAKeyPair() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	printf("test_ImportPlainRsaKeyPair() pass!\n");
END:
	if (pKeypairFile)
	{
		fclose(pKeypairFile);
		pKeypairFile = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}
	if (hApp)
	{
		SKF_CloseApplication(hApp);
		hApp = NULL;
	}

	return;
}

void test_RsaEncryptDecrypt(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulDataLen = 0;
	ULONG ulSelAppIndex = 0;
	ULONG ulSessionKeyLen = 256;
	ULONG ulRsaPubKeyLen = 0;
	ULONG ulRetryTimes = 0;
	CHAR csUserPin[16] = {0};
	BYTE btSessionKey[256] = {0};
	HAPPLICATION hApp = NULL;
	HCONTAINER hCon = NULL;
	HANDLE hSessionKey = 0;
	RSAPUBLICKEYBLOB stRsaPubKey = {0}; 
	BLOCKCIPHERPARAM stEncryptParam = {0};
	time_t tStart = 0;
	time_t tEnd = 0;
	//
	const ULONG DATA_LEN = 100*1024 + 6;
	const ULONG ONCE_LEN = 1024;
	ULONG ulEncryptedLen = ONCE_LEN + PADDED_DATA_LEN;
	ULONG ulDecryptedLen = ONCE_LEN + PADDED_DATA_LEN;
	BYTE btSrcData[DATA_LEN] = {0};
	BYTE btEncryptedData[DATA_LEN + PADDED_DATA_LEN] = {0};
	BYTE btDecryptedData[DATA_LEN + PADDED_DATA_LEN] = {0};
	BYTE btFinalData[ONCE_LEN] = {0};

	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}
	
	//	输入用户PIN
	printf("Please enter the user PIN:");
	gets_s(csUserPin, 16);
	if (strlen(csUserPin) == 0 || strlen(csUserPin) >= 16)
	{
		printf("User PIN is wrong!");
		goto END;
	}	

	//	打开应用
	ulRes = SKF_OpenApplication(hDev, g_csAppNames[ulSelAppIndex], &hApp);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	用户登录
	ulRes = SKF_VerifyPIN(hApp, KT_USERPIN, csUserPin, &ulRetryTimes);
	if (0 != ulRes)
	{
		printf("SKF_VerifyPIN() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	打开容器
	ulRes = SKF_OpenContainer(hApp, RSA_CON_NAME, &hCon);
	if (0 != ulRes)
	{
		printf("SKF_OpenContainer() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	导出加密公钥
	ulRsaPubKeyLen = sizeof(RSAPUBLICKEYBLOB);
	ulRes = SKF_ExportPublicKey(hCon, FALSE, (BYTE*)&stRsaPubKey, &ulRsaPubKeyLen);
	if (0 != ulRes)
	{
		printf("SKF_ExportPublicKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	生成并导出会话密钥
	ulRes = SKF_RSAExportSessionKey(hCon, SGD_SM1_ECB, &stRsaPubKey, btSessionKey, &ulSessionKeyLen, &hSessionKey);
	if (0 != ulRes)
	{
		printf("SKF_RSAExportSessionKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//	生成原文数据
	for (ulIndex = 0; ulIndex < DATA_LEN; ulIndex++)
	{
		btSrcData[ulIndex] = Random(255);
	}
	
	

	//////////////////////////////单组加解密数据测试开始//////////////////////////////////
	//	单组加密初始化
	stEncryptParam.PaddingType = 1;
	ulRes = SKF_EncryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_EncryptInit() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	单组加密数据
	ulEncryptedLen = 50 + PADDED_DATA_LEN;
	ulRes = SKF_Encrypt(hSessionKey, btSrcData, 50, btEncryptedData, &ulEncryptedLen);
	if (0 != ulRes)
	{
		printf("SKF_Encrypt() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//	单组解密初始化
	ulRes = SKF_DecryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_DecryptInit() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	} 

	//	单组解密数据
	ulDecryptedLen = ulEncryptedLen;
	ulRes = SKF_Decrypt(hSessionKey, btEncryptedData, ulEncryptedLen, btDecryptedData, &ulDecryptedLen);
	if (0 != ulRes)
	{
		printf("SKF_Decrypt() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	对比结果
	if (ulDecryptedLen != 50 || memcmp(btSrcData, btDecryptedData, ulDecryptedLen) != 0)
	{
		printf("The decrypted result is not same as plain data!\n");
		goto END;
	}
	stEncryptParam.PaddingType = 0;
	//////////////////////////////单组加解密数据测试结束//////////////////////////////////
	

	
	//////////////////////////////多组加解密数据测试开始//////////////////////////////////
	//	加密初始化
	printf("Encrypt is starting...\n");
	tStart = GetCurrentTime();
	stEncryptParam.PaddingType = 1;
	ulRes = SKF_EncryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_EncryptInit() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	加密数据
	ulDataLen = 0;
	ulIndex = 0;
	do
	{
		ULONG ulInLen = ONCE_LEN;
		if (DATA_LEN - ulDataLen < ONCE_LEN)
		{
			ulInLen = DATA_LEN - ulDataLen;
		}
		ulEncryptedLen = ONCE_LEN + PADDED_DATA_LEN;
		ulRes = SKF_EncryptUpdate(hSessionKey, btSrcData + ulDataLen, ulInLen, btEncryptedData + ulIndex, &ulEncryptedLen);
		if (0 != ulRes)
		{
			printf("SKF_EncryptUpdate() failed! dwRes = 0x%x\n", ulRes);
			goto END;
		}
		ulDataLen += ulInLen;
		ulIndex += ulEncryptedLen;
	}while (ulDataLen < DATA_LEN);
	
	//	加密结束
	ulEncryptedLen = ONCE_LEN;
	ulRes = SKF_EncryptFinal(hSessionKey, btFinalData, &ulEncryptedLen);
	if (ulEncryptedLen > 0)
	{
		memcpy_s(btEncryptedData + ulIndex, DATA_LEN, btFinalData, ulEncryptedLen);
		ulIndex += ulEncryptedLen;
	}
	ulEncryptedLen = ulIndex;
	tEnd = GetCurrentTime();
	printf("Encrypt is end! \nTotal time: %.02fs \nSize: %dKBytes \nSpeed: %.02fKBytes/s\n", (tEnd - tStart) / 1000.0f, (DATA_LEN / 1024), (DATA_LEN / 1024) * 1000.0f / (tEnd - tStart));

	//	关闭会话密钥
	SKF_CloseHandle(hSessionKey);
	hSessionKey = NULL;

	//	导入会话密钥
	ulRes = SKF_ImportSessionKey(hCon, SGD_SM1_ECB, btSessionKey, ulSessionKeyLen, &hSessionKey);
	if (0 != ulRes)
	{
		printf("SKF_ImportSessionKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//解密初始化
	printf("Decrypt is starting...\n");
	tStart = GetCurrentTime();
	ulRes = SKF_DecryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_DecryptInit() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	} 

	//解密数据
	ulDataLen = 0;
	ulIndex = 0;
	do
	{
		ULONG ulInLen = ONCE_LEN;
		if (ulEncryptedLen - ulDataLen < ONCE_LEN)
		{
			ulInLen = ulEncryptedLen - ulDataLen;
		}
		ulDecryptedLen = ONCE_LEN + PADDED_DATA_LEN;
		ulRes = SKF_DecryptUpdate(hSessionKey, btEncryptedData + ulDataLen, ulInLen, btDecryptedData + ulIndex, &ulDecryptedLen);
		if (0 != ulRes)
		{
			printf("SKF_DecryptUpdate() failed! dwRes = 0x%x\n", ulRes);
			goto END;
		}
		ulDataLen += ulInLen;
		ulIndex += ulDecryptedLen;
	}while (ulDataLen < ulEncryptedLen);
	
	//解密结束
	ulDecryptedLen = ONCE_LEN;
	ulRes = SKF_DecryptFinal(hSessionKey, btFinalData, &ulDecryptedLen);
	if (ulDecryptedLen > 0)
	{
		memcpy_s(btDecryptedData + ulIndex, DATA_LEN, btFinalData, ulDecryptedLen);
		ulIndex += ulDecryptedLen;
	}
	tEnd = GetCurrentTime();
	printf("Decrypt is end! \nTotal time: %.02fs \nSize: %dKBytes \nSpeed: %.02fKBytes/s\n", (tEnd - tStart) / 1000.0f, (DATA_LEN / 1024), (DATA_LEN / 1024) * 1000.0f / (tEnd - tStart));

	//比较结果
	if (ulIndex != DATA_LEN || memcmp(btSrcData, btDecryptedData, DATA_LEN) != 0)
	{
		printf("The decrypted result is not same as plain data!\n");
		goto END;
	}	
	//////////////////////////////多组加解密数据测试结束//////////////////////////////////
	
	
	printf("test_RsaEncryptDecrypt() pass!\n");
END:
	if (hSessionKey)
	{
		SKF_CloseHandle(hSessionKey);
		hSessionKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}
	if (hApp)
	{
		SKF_CloseApplication(hApp);
		hApp = NULL;
	}

	return;
}

void test_RsaSignVerify(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulDataLen = 1024;
	ULONG ulHashLen = 48;
	ULONG ulSignatureLen = 256;
	ULONG ulRsaPubKeyLen = 0;
	ULONG ulSelAppIndex = 0;
	ULONG ulRetryTimes = 0;	
	CHAR csUserPin[16] = {0};
	HAPPLICATION hApp = NULL;
	HCONTAINER hCon = NULL;
	HANDLE hHashKey = 0;
	RSAPUBLICKEYBLOB stRsaPubKeyBlob = {0};
	BYTE btSrcData[1024] = {0};
	BYTE btHashData[48] = {0};
	BYTE btSignature[256] = {0};

	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}
	
	//	输入用户PIN
	printf("Please enter the user PIN:");
	gets_s(csUserPin, 16);
	if (strlen(csUserPin) == 0 || strlen(csUserPin) >= 16)
	{
		printf("User PIN is wrong!");
		goto END;
	}	

	//	打开应用
	ulRes = SKF_OpenApplication(hDev, g_csAppNames[ulSelAppIndex], &hApp);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	用户登录
	ulRes = SKF_VerifyPIN(hApp, KT_USERPIN, csUserPin, &ulRetryTimes);
	if (0 != ulRes)
	{
		printf("SKF_VerifyPIN() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	打开容器
	ulRes = SKF_OpenContainer(hApp, RSA_CON_NAME, &hCon);
	if (0 != ulRes)
	{
		printf("SKF_OpenContainer() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	生成原文数据
	for (ulIndex = 0; ulIndex < ulDataLen; ulIndex++)
	{
		btSrcData[ulIndex] = Random(255);
	}

	//	对数据做Hash
	ulRes = SKF_DigestInit(hDev, SGD_SHA1, NULL, NULL, 0, &hHashKey);
	if (0 != ulRes)
	{
		printf("SKF_DigestInit() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	ulRes = SKF_Digest(hHashKey, btSrcData, ulDataLen, btHashData, &ulHashLen);
	if (0 != ulRes)
	{
		printf("SKF_Digest() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	签名
	ulRes = SKF_RSASignData(hCon, btHashData, ulHashLen, btSignature, &ulSignatureLen);
	if (0 != ulRes)
	{
		printf("SKF_RSASignData() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	导出签名公钥
	ulRsaPubKeyLen = sizeof(stRsaPubKeyBlob);
	ulRes = SKF_ExportPublicKey(hCon, TRUE, (LPBYTE)&stRsaPubKeyBlob, &ulRsaPubKeyLen);
	if (0 != ulRes)
	{
		printf("SKF_ExportPublicKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	验签
	ulRes = SKF_RSAVerify(hDev, &stRsaPubKeyBlob, btHashData, ulHashLen, btSignature, ulSignatureLen);
	if (0 != ulRes)
	{
		printf("SKF_RSAVerify() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	printf("test_RsaSignVerify() pass!\n");
END:
	if (hHashKey)
	{
		SKF_CloseHandle(hHashKey);
		hHashKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}
	if (hApp)
	{
		SKF_CloseApplication(hApp);
		hApp = NULL;
	}

	return;
}

void test_ExtRsaKeyOPT(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	CHAR csKeyPairFile[256] = {0};
	FILE* pKeypairFile = NULL;
	ULONG ulKeyPairLen = 2048;
	BYTE btKeyPairData[2048] = {0};
	ULONG ulDataIndex = 0;
	USHORT usByteLen = 0;
	USHORT usPrimeLen = 0;
	RSAPRIVATEKEYBLOB stRsaPriKeyBlob = {0};
	RSAPUBLICKEYBLOB* pRsaPubKey = NULL;	
	ULONG ulSrcDataLen = 256;
	ULONG ulOutDataLen = 256;
	ULONG ulResDataLen = 256;
	BYTE btSrcData[256] = {0};
	BYTE btOutData[256] = {0};
	BYTE btResData[256] = {0};
	
	//	输入密钥对数据文件
	printf("Please enter the plain RSA key pair file:");
	gets_s(csKeyPairFile, 256);
	ulRes = fopen_s(&pKeypairFile, csKeyPairFile, "r");
	if (!pKeypairFile)
	{
		printf("Open file failed!\n ulRes = 0x%x", ulRes);
		goto END;
	}	
	
	//	读取文件数据
	if (!helper_ReadData(pKeypairFile, btKeyPairData, &ulKeyPairLen))
	{
		printf("Read file failed!\n");
		goto END;
	}
	
	//	构造RSA密钥结构体
	stRsaPriKeyBlob.AlgID = SGD_RSA;
	ulDataIndex = 0;
	stRsaPriKeyBlob.BitLen = (btKeyPairData[1] << 8) | btKeyPairData[0];
	ulDataIndex += 2;
	usByteLen = (USHORT)stRsaPriKeyBlob.BitLen / 8;
	usPrimeLen = usByteLen / 2;
	memcpy(stRsaPriKeyBlob.Modulus + MAX_RSA_MODULUS_LEN - usByteLen, btKeyPairData + ulDataIndex, usByteLen);
	ulDataIndex += usByteLen;
	memcpy(stRsaPriKeyBlob.PublicExponent, btKeyPairData + ulDataIndex, 4);
	ulDataIndex += 4;
	memcpy(stRsaPriKeyBlob.PrivateExponent + MAX_RSA_MODULUS_LEN - usByteLen, btKeyPairData + ulDataIndex, usByteLen);
	ulDataIndex += usByteLen;
	memcpy(stRsaPriKeyBlob.Prime1 + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;
	memcpy(stRsaPriKeyBlob.Prime2 + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;
	memcpy(stRsaPriKeyBlob.Prime1Exponent + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;
	memcpy(stRsaPriKeyBlob.Prime2Exponent + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;
	memcpy(stRsaPriKeyBlob.Coefficient + MAX_RSA_PRIME_LEN - usPrimeLen, btKeyPairData + ulDataIndex, usPrimeLen);
	ulDataIndex += usPrimeLen;

	//	RSA公钥
	pRsaPubKey = (RSAPUBLICKEYBLOB*)&stRsaPriKeyBlob;
	ulSrcDataLen = stRsaPriKeyBlob.BitLen / 8;		//测试数据长度必须等于Modulus长度，因为COS中的固件函数RSA_PubKeyOpt()/RSA_PriKeyOpt()不对数据补齐
	
	//	生成原文数据
	for (ulDataIndex = 0; ulDataIndex < ulSrcDataLen; ulDataIndex++)
	{
		btSrcData[ulDataIndex] = Random(255);
	}

	//	公钥操作
	ulRes = SKF_ExtRSAPubKeyOperation(hDev, pRsaPubKey, btSrcData, ulSrcDataLen, btOutData, &ulOutDataLen);
	if (0 != ulRes)
	{
		printf("SKF_ExtRSAPubKeyOperation() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	私钥操作
	ulRes = SKF_ExtRSAPriKeyOperation(hDev, &stRsaPriKeyBlob, btOutData, ulOutDataLen, btResData, &ulResDataLen);
	if (0 != ulRes)
	{
		printf("SKF_ExtRSAPriKeyOperation() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	比较结果
	if ((ulResDataLen != ulSrcDataLen) || (memcmp(btSrcData, btResData, ulResDataLen) != 0))
	{
		printf("Result data is wrong!\n");
		goto END;
	}
	
	//	私钥操作
	ulOutDataLen = 256;
	ulRes = SKF_ExtRSAPriKeyOperation(hDev, &stRsaPriKeyBlob, btSrcData, ulSrcDataLen, btOutData, &ulOutDataLen);
	if (0 != ulRes)
	{
		printf("SKF_ExtRSAPriKeyOperation() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//	公钥操作
	ulResDataLen = 256;
	ulRes = SKF_ExtRSAPubKeyOperation(hDev, pRsaPubKey, btOutData, ulOutDataLen, btResData, &ulResDataLen);
	if (0 != ulRes)
	{
		printf("SKF_ExtRSAPubKeyOperation() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
		
	//	比较结果
	if ((ulResDataLen != ulSrcDataLen) || (memcmp(btSrcData, btResData, ulResDataLen) != 0))
	{
		printf("Result data is wrong!\n");
		goto END;
	}

	printf("test_ExtRsaKeyOPT() pass!\n");
END:
	if (pKeypairFile)
	{
		fclose(pKeypairFile);
		pKeypairFile = NULL;
	}
}

void test_ImportEccKeyPair(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulBits = 0;
	ULONG ulIndex = 0;
	ULONG ulSelAppIndex = 0;
	ULONG ulSymmAlg = SGD_SMS4_ECB;
	ULONG ulEccPubKeyBlobLen = 0;
	ULONG ulEncryptedPriKeyLen = 64;
	ULONG ulRetryTimes = 0;
	CHAR csUserPin[16] = {0};
	CHAR csEccKeyFile[256] = {0};
	FILE* pFile = NULL;
	ULONG ulEccKeyPairLen = 0;
	BYTE btEccKeyPair[1024] = {0};
	BYTE btSessionKey[32] = {0};
	BYTE btEncryptedPriKey[64] = {0};
	HAPPLICATION hApp = NULL;
	HCONTAINER hCon = NULL;
	HANDLE hSessionKey = NULL;
	ECCPUBLICKEYBLOB stEccSignPubKeyBlob = {0};
	ECCPUBLICKEYBLOB stEccPubKeyBlob = {0};
	ECCPRIVATEKEYBLOB stEccPriKeyBlob = {0};
	ECCCIPHERBLOB stEccCipherBlob = {0};
	BLOCKCIPHERPARAM stEncryptParam = {0};	
	ENVELOPEDKEYBLOB stEnvelopedKeyBlob = {0};
	
	//	输入密钥对数据文件
	printf("Please enter the plain SM2 key pair file:");
	gets_s(csEccKeyFile, 256);
	ulRes = fopen_s(&pFile, csEccKeyFile, "r");
	if (!pFile)
	{
		printf("Open file failed!\n ulRes = 0x%x", ulRes);
		goto END;
	}	
	
	//	读取文件数据
	ulEccKeyPairLen = 1024;
	if (!helper_ReadData(pFile, btEccKeyPair, &ulEccKeyPairLen))
	{
		printf("Read file failed!\n");
		goto END;
	}
	if (ulEccKeyPairLen != (4 + 2*ECC_MAX_BITS/8 + 4 + ECC_MAX_BITS/8))
	{
		printf("SM2 key pair data length is wrong! ulEccKeyPairLen = 0x%x\n", ulEccKeyPairLen);
		goto END;
	}

	//	解析SM2密钥对数据
	memcpy(&stEccPubKeyBlob.BitLen, btEccKeyPair, 4);
	ulIndex = 4;
	memcpy(stEccPubKeyBlob.XCoordinate, btEccKeyPair + ulIndex, 64);
	ulIndex += 64;
	memcpy(stEccPubKeyBlob.YCoordinate, btEccKeyPair + ulIndex, 64);
	ulIndex += 64;
	memcpy(&stEccPriKeyBlob.BitLen, btEccKeyPair + ulIndex, 4);
	ulIndex += 4;
	memcpy(stEccPriKeyBlob.PrivateKey, btEccKeyPair + ulIndex, 64);
	ulIndex += 64;

	//	选择应用
	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}

	//	输入用户PIN
	printf("Please enter the user PIN:");
	gets_s(csUserPin, 16);
	if (strlen(csUserPin) == 0 || strlen(csUserPin) >= 16)
	{
		printf("User PIN is wrong!");
		goto END;
	}		

	//	打开应用
	ulRes = SKF_OpenApplication(hDev, g_csAppNames[ulSelAppIndex], &hApp);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	用户登录
	ulRes = SKF_VerifyPIN(hApp, KT_USERPIN, csUserPin, &ulRetryTimes);
	if (0 != ulRes)
	{
		printf("SKF_VerifyPIN() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	打开容器
	ulRes = SKF_OpenContainer(hApp, ECC_CON_NAME, &hCon);
	if (0 != ulRes)
	{
		printf("SKF_OpenContainer() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	生成会话密钥数据
	for (ulIndex = 0; ulIndex < 32; ulIndex++)
	{
		btSessionKey[ulIndex] = Random(255);
	}

	//	导入会话密钥
	ulRes = SKF_SetSymmKey(hDev, btSessionKey, ulSymmAlg, &hSessionKey);
	if (0 != ulRes || NULL == hSessionKey)
	{
		printf("SKF_SetSymmKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	导出签名公钥
	ulEccPubKeyBlobLen = sizeof(stEccSignPubKeyBlob);
	ulRes = SKF_ExportPublicKey(hCon, TRUE, (LPBYTE)&stEccSignPubKeyBlob, &ulEccPubKeyBlobLen);
	if (0 != ulRes)
	{
		printf("SKF_ExportPublicKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	使用签名公钥对会话密钥加密
	ulRes = SKF_ExtECCEncrypt(hDev, &stEccSignPubKeyBlob, btSessionKey, 32, &stEccCipherBlob);
	if (0 != ulRes)
	{
		printf("SKF_ExtECCEncrypt() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	使用会话密钥加密私钥数据
	ulRes = SKF_EncryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_ExtECCEncrypt() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	ulRes = SKF_Encrypt(hSessionKey, stEccPriKeyBlob.PrivateKey, 64, btEncryptedPriKey, &ulEncryptedPriKeyLen);
	if (0 != ulRes)
	{
		printf("SKF_ExtECCEncrypt() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	组装数字信封数据
	stEnvelopedKeyBlob.Version = 0x01;
	stEnvelopedKeyBlob.ulSymmAlgID = ulSymmAlg;
	stEnvelopedKeyBlob.ulBits = stEccPubKeyBlob.BitLen;
	memcpy(stEnvelopedKeyBlob.cbEncryptedPriKey, btEncryptedPriKey, ulEncryptedPriKeyLen);
	memcpy(&stEnvelopedKeyBlob.PubKey, &stEccPubKeyBlob, sizeof(ECCPUBLICKEYBLOB));
	memcpy(&stEnvelopedKeyBlob.ECCCipherBlob, &stEccCipherBlob, sizeof(ECCCIPHERBLOB));

	//	导入SM2加密密钥对
	ulRes = SKF_ImportECCKeyPair(hCon, &stEnvelopedKeyBlob);
	if (0 != ulRes)
	{
		printf("SKF_ExtECCEncrypt() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}	

	printf("test_ImportEccKeyPair() pass!\n");
END:
	if (hSessionKey)
	{
		SKF_CloseHandle(hSessionKey);
		hSessionKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}
	if (hApp)
	{
		SKF_CloseApplication(hApp);
		hApp = NULL;
	}
	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
}

void test_EccEncryptDecrypt(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulDataLen = 0;
	ULONG ulSelAppIndex = 0;
	ULONG ulEccPubKeyLen = 0;
	ULONG ulRetryTimes = 0;
	CHAR csUserPin[16] = {0};
	HAPPLICATION hApp = NULL;
	HCONTAINER hCon = NULL;
	HANDLE hSessionKey = 0;
	ECCPUBLICKEYBLOB stEccPubKeyBlob = {0};
	ECCCIPHERBLOB stEccCipberBlob = {0};
	BLOCKCIPHERPARAM stEncryptParam = {0};
	time_t tStart = 0;
	time_t tEnd = 0;
	//
	const ULONG DATA_LEN = 100*1024;
	const ULONG ONCE_LEN = 1024;
	ULONG ulEncryptedLen = ONCE_LEN + PADDED_DATA_LEN;
	ULONG ulDecryptedLen = ONCE_LEN + PADDED_DATA_LEN;
	BYTE btSrcData[DATA_LEN] = {0};
	BYTE btEncryptedData[DATA_LEN] = {0};
	BYTE btDecryptedData[DATA_LEN] = {0};
	BYTE btFinalData[ONCE_LEN] = {0};

	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}
	
	//	输入用户PIN
	printf("Please enter the user PIN:");
	gets_s(csUserPin, 16);
	if (strlen(csUserPin) == 0 || strlen(csUserPin) >= 16)
	{
		printf("User PIN is wrong!");
		goto END;
	}	

	//	打开应用
	ulRes = SKF_OpenApplication(hDev, g_csAppNames[ulSelAppIndex], &hApp);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	用户登录
	ulRes = SKF_VerifyPIN(hApp, KT_USERPIN, csUserPin, &ulRetryTimes);
	if (0 != ulRes)
	{
		printf("SKF_VerifyPIN() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	打开容器
	ulRes = SKF_OpenContainer(hApp, ECC_CON_NAME, &hCon);
	if (0 != ulRes)
	{
		printf("SKF_OpenContainer() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	导出加密公钥
	ulEccPubKeyLen = sizeof(ECCPUBLICKEYBLOB);
	ulRes = SKF_ExportPublicKey(hCon, FALSE, (BYTE*)&stEccPubKeyBlob, &ulEccPubKeyLen);
	if (0 != ulRes)
	{
		printf("SKF_ExportPublicKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	生成并导出会话密钥
	ulRes = SKF_ECCExportSessionKey(hCon, SGD_SM1_ECB, &stEccPubKeyBlob, &stEccCipberBlob, &hSessionKey);
	if (0 != ulRes)
	{
		printf("SKF_ECCExportSessionKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//	生成原文数据
	for (ulIndex = 0; ulIndex < DATA_LEN; ulIndex++)
	{
		btSrcData[ulIndex] = Random(255);
	}

	//	加密初始化
	printf("Encrypt is starting...\n");
	tStart = GetCurrentTime();
	ulRes = SKF_EncryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_EncryptInit() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	加密数据
	ulDataLen = 0;
	ulIndex = 0;
	do
	{
		ulEncryptedLen = ONCE_LEN + PADDED_DATA_LEN;
		ulRes = SKF_EncryptUpdate(hSessionKey, btSrcData + ulDataLen, ONCE_LEN, btEncryptedData + ulIndex, &ulEncryptedLen);
		if (0 != ulRes)
		{
			printf("SKF_EncryptUpdate() failed! dwRes = 0x%x\n", ulRes);
			goto END;
		}
		ulDataLen += ONCE_LEN;
		ulIndex += ulEncryptedLen;
	}while (ulDataLen < DATA_LEN);
	
	//	加密结束
	ulEncryptedLen = ONCE_LEN;
	ulRes = SKF_EncryptFinal(hSessionKey, btFinalData, &ulEncryptedLen);
	if (ulEncryptedLen > 0)
	{
		memcpy_s(btEncryptedData + ulIndex, DATA_LEN, btFinalData, ulEncryptedLen);
		ulIndex += ulEncryptedLen;
	}
	tEnd = GetCurrentTime();
	printf("Encrypt is end! \nTotal time: %.02fs \nSize: %dKBytes \nSpeed: %.02fKBytes/s\n", (tEnd - tStart) / 1000.0f, (DATA_LEN / 1024), (DATA_LEN / 1024) * 1000.0f / (tEnd - tStart));

	//	关闭会话密钥
	SKF_CloseHandle(hSessionKey);
	hSessionKey = NULL;

	//	导入会话密钥
	ulRes = SKF_ImportSessionKey(hCon, SGD_SM1_ECB, (LPBYTE)&stEccCipberBlob, sizeof(stEccCipberBlob), &hSessionKey);
	if (0 != ulRes)
	{
		printf("SKF_ImportSessionKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//解密初始化
	printf("Decrypt is starting...\n");
	tStart = GetCurrentTime();
	ulRes = SKF_DecryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_DecryptInit() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	} 

	//解密数据
	ulDataLen = 0;
	ulIndex = 0;
	do
	{
		ulDecryptedLen = ONCE_LEN + PADDED_DATA_LEN;
		ulRes = SKF_DecryptUpdate(hSessionKey, btEncryptedData + ulDataLen, ONCE_LEN, btDecryptedData + ulIndex, &ulDecryptedLen);
		if (0 != ulRes)
		{
			printf("SKF_DecryptUpdate() failed! dwRes = 0x%x\n", ulRes);
			goto END;
		}
		ulDataLen += ONCE_LEN;
		ulIndex += ulDecryptedLen;
	}while (ulDataLen < DATA_LEN);
	
	//解密结束
	ulDecryptedLen = ONCE_LEN;
	ulRes = SKF_DecryptFinal(hSessionKey, btFinalData, &ulDecryptedLen);
	if (ulDecryptedLen > 0)
	{
		memcpy_s(btDecryptedData + ulIndex, DATA_LEN, btFinalData, ulDecryptedLen);
		ulIndex += ulDecryptedLen;
	}
	tEnd = GetCurrentTime();
	printf("Decrypt is end! \nTotal time: %.02fs \nSize: %dKBytes \nSpeed: %.02fKBytes/s\n", (tEnd - tStart) / 1000.0f, (DATA_LEN / 1024), (DATA_LEN / 1024) * 1000.0f / (tEnd - tStart));

	//比较结果
	if (ulIndex != ulDataLen || memcmp(btSrcData, btDecryptedData, ulDataLen) != 0)
	{
		printf("The decrypted result is not same as plain data!\n");
		goto END;
	}	
	
	printf("test_EccEncryptDecrypt() pass!\n");
END:
	if (hSessionKey)
	{
		SKF_CloseHandle(hSessionKey);
		hSessionKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}
	if (hApp)
	{
		SKF_CloseApplication(hApp);
		hApp = NULL;
	}

	return;
}

void test_EccSignVerify(DEVHANDLE hDev)
{	
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulDataLen = 1024;
	ULONG ulHashLen = 48;
	ULONG ulSignatureLen = 256;
	ULONG ulEccPubKeyLen = 0;
	ULONG ulSelAppIndex = 0;
	ULONG ulRetryTimes = 0;	
	CHAR csUserPin[16] = {0};
	HAPPLICATION hApp = NULL;
	HCONTAINER hCon = NULL;
	HANDLE hHashKey = 0;
	ECCPUBLICKEYBLOB stEccPubKeyBlob = {0};
	ECCSIGNATUREBLOB stEccSignature = {0};
	BYTE btUserID[16] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38}; 
	BYTE btSrcData[1024] = {0};
	BYTE btHashData[48] = {0};
	BYTE btSignature[256] = {0};

	ulSelAppIndex = helper_SelectApplication(hDev);
	if (-1 == ulSelAppIndex)
	{
		goto END;
	}
	
	//	输入用户PIN
	printf("Please enter the user PIN:");
	gets_s(csUserPin, 16);
	if (strlen(csUserPin) == 0 || strlen(csUserPin) >= 16)
	{
		printf("User PIN is wrong!");
		goto END;
	}	

	//	打开应用
	ulRes = SKF_OpenApplication(hDev, g_csAppNames[ulSelAppIndex], &hApp);
	if (0 != ulRes)
	{
		printf("SKF_OpenApplication() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	用户登录
	ulRes = SKF_VerifyPIN(hApp, KT_USERPIN, csUserPin, &ulRetryTimes);
	if (0 != ulRes)
	{
		printf("SKF_VerifyPIN() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	打开容器
	ulRes = SKF_OpenContainer(hApp, ECC_CON_NAME, &hCon);
	if (0 != ulRes)
	{
		printf("SKF_OpenContainer() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}	

	//	导出签名公钥
	ulEccPubKeyLen = sizeof(stEccPubKeyBlob);
	ulRes = SKF_ExportPublicKey(hCon, TRUE, (LPBYTE)&stEccPubKeyBlob, &ulEccPubKeyLen);
	if (0 != ulRes)
	{
		printf("SKF_ExportPublicKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	生成原文数据
	for (ulIndex = 0; ulIndex < ulDataLen; ulIndex++)
	{
		btSrcData[ulIndex] = Random(255);
	}

	//	对数据做Hash
	ulRes = SKF_DigestInit(hDev, SGD_SM3, &stEccPubKeyBlob, btUserID, 16, &hHashKey);
	if (0 != ulRes)
	{
		printf("SKF_DigestInit() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	ulRes = SKF_Digest(hHashKey, btSrcData, ulDataLen, btHashData, &ulHashLen);
	if (0 != ulRes)
	{
		printf("SKF_Digest() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	签名
	ulRes = SKF_ECCSignData(hCon, btHashData, ulHashLen, &stEccSignature);
	if (0 != ulRes)
	{
		printf("SKF_ECCSignData() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//	验签
	ulRes = SKF_ECCVerify(hDev, &stEccPubKeyBlob, btHashData, ulHashLen, &stEccSignature);
	if (0 != ulRes)
	{
		printf("SKF_ECCVerify() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	printf("test_EccSignVerify() pass!\n");
END:
	if (hHashKey)
	{
		SKF_CloseHandle(hHashKey);
		hHashKey = NULL;
	}
	if (hCon)
	{
		SKF_CloseContainer(hCon);
		hCon = NULL;
	}
	if (hApp)
	{
		SKF_CloseApplication(hApp);
		hApp = NULL;
	}

	return;
}

void test_ExtEccKeyOPT(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulDecryptedLen = 64;
	CHAR csEccKeyFile[256] = {0};
	FILE* pFile = NULL;
	ULONG ulEccKeyPairLen = 1024;
	BYTE btEccKeyPair[1024] = {0};
	BYTE btSrcData[32] = {0};
	BYTE btDecryptedData[64] = {0};
	ECCPUBLICKEYBLOB stEccPubKeyBlob = {0};
	ECCPRIVATEKEYBLOB stEccPriKeyBlob = {0};
	ECCCIPHERBLOB stEccCipherBlob = {0};
	ECCSIGNATUREBLOB stEccSignature = {0};
	
	//	输入密钥对数据文件
	printf("Please enter the plain SM2 key pair file:");
	gets_s(csEccKeyFile, 256);
	ulRes = fopen_s(&pFile, csEccKeyFile, "r");
	if (!pFile)
	{
		printf("Open file failed!\n ulRes = 0x%x", ulRes);
		goto END;
	}	
	
	//	读取文件数据
	if (!helper_ReadData(pFile, btEccKeyPair, &ulEccKeyPairLen))
	{
		printf("Read file failed!\n");
		goto END;
	}
	if (ulEccKeyPairLen != (4 + 2*ECC_MAX_BITS/8 + 4 + ECC_MAX_BITS/8))
	{
		printf("SM2 key pair data length is wrong! ulEccKeyPairLen = 0x%x\n", ulEccKeyPairLen);
		goto END;
	}

	//	解析SM2密钥对数据
	memcpy(&stEccPubKeyBlob.BitLen, btEccKeyPair, 4);
	ulIndex = 4;
	memcpy(stEccPubKeyBlob.XCoordinate, btEccKeyPair + ulIndex, 64);
	ulIndex += 64;
	memcpy(stEccPubKeyBlob.YCoordinate, btEccKeyPair + ulIndex, 64);
	ulIndex += 64;
	memcpy(&stEccPriKeyBlob.BitLen, btEccKeyPair + ulIndex, 4);
	ulIndex += 4;
	memcpy(stEccPriKeyBlob.PrivateKey, btEccKeyPair + ulIndex, 64);
	ulIndex += 64;

	//	公钥加密
	ulRes = SKF_ExtECCEncrypt(hDev, &stEccPubKeyBlob, btSrcData, 32, &stEccCipherBlob);
	if (0 != ulRes)
	{
		printf("SKF_ExtECCEncrypt() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//	私钥解密
	ulRes = SKF_ExtECCDecrypt(hDev, &stEccPriKeyBlob, &stEccCipherBlob, btDecryptedData, &ulDecryptedLen);
	if (0 != ulRes)
	{
		printf("SKF_ExtECCDecrypt() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//	比较结果
	if (32 != ulDecryptedLen || memcmp(btSrcData, btDecryptedData, 32) != 0)
	{
		printf("The decrypted data is worng!");
		goto END;
	}

	//	私钥签名
	ulRes = SKF_ExtECCSign(hDev, &stEccPriKeyBlob, btSrcData, 32, &stEccSignature);
	if (0 != ulRes)
	{
		printf("SKF_ExtECCSign() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	公钥验签
	ulRes = SKF_ExtECCVerify(hDev, &stEccPubKeyBlob, btSrcData, 32, &stEccSignature);
	if (0 != ulRes)
	{
		printf("SKF_ExtECCVerify() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	printf("test_ExtEccKeyOPT() pass!\n");
END:
	if (pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
}

void test_SymmKeyData(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulDataLen = 0;
	ULONG ulSymmAlg = 0;
	BYTE btInput = 0;
	time_t tStart = 0;
	time_t tEnd = 0;
	HANDLE hSessionKey = NULL;
	BLOCKCIPHERPARAM stEncryptParam = {0};
	//
	BYTE btSessionKey[32] = {0};	
	ULONG ulEncryptedLen = PROC_DATA_LEN + PADDED_DATA_LEN;
	ULONG ulDecryptedLen = PROC_DATA_LEN + PADDED_DATA_LEN;
	BYTE btEncryptedData[PROC_DATA_LEN + PADDED_DATA_LEN] = {0};
	BYTE btDecryptedData[PROC_DATA_LEN + PADDED_DATA_LEN] = {0};	

	printf("Please select symm key alg: [0-SM1_ECB;1-SMS4_ECB]:");
	btInput = _getch();
	printf("%c\n", btInput);
	btInput = (btInput - 0x30);
	if (btInput == 0)
	{
		ulSymmAlg = SGD_SM1_ECB;
	}
	else if (btInput == 1)
	{
		ulSymmAlg = SGD_SMS4_ECB;
	}
	else
	{
		printf("Input option is wrong!\n");
		return;
	}

	//	生成原文数据
	for (ulIndex = 0; ulIndex < TOTAL_DATA_LEN; ulIndex++)
	{
		g_btPlainData[ulIndex] = Random(255);
	}

	//	导入会话密钥 
	ulRes  = SKF_SetSymmKey(hDev, btSessionKey, ulSymmAlg, &hSessionKey);
	if (0 != ulRes)
	{
		printf("SKF_SetSymmKey() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//////////////////////////////带Padding的加解密数据测试开始//////////////////////////////////
	stEncryptParam.PaddingType = 1;
	ulRes = SKF_EncryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_EncryptInit() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	ulIndex = 0;
	ulEncryptedLen = TOTAL_DATA_LEN;
	ulRes = SKF_EncryptTunnelUpdate(hDev, g_btPlainData, 100, g_btCipherData, &ulEncryptedLen);
	if (0 != ulRes)
	{
		printf("SKF_EncryptTunnelUpdate() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	}
	ulIndex = ulEncryptedLen;
	ulEncryptedLen = TOTAL_DATA_LEN;
	ulRes = SKF_EncryptFinal(hSessionKey, g_btCipherData + ulIndex, &ulEncryptedLen);
	if (0 != ulRes)
	{
		printf("SKF_EncryptFinal() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	}
	ulIndex += ulEncryptedLen;
	//
	ulRes = SKF_DecryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_DecryptInit() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	} 
	ulDataLen = 0;
	ulDecryptedLen = TOTAL_DATA_LEN;
	ulRes = SKF_DecryptTunnelUpdate(hDev, g_btCipherData, ulIndex, g_btDecryptedData, &ulDecryptedLen);
	if (0 != ulRes)
	{
		printf("SKF_DecryptTunnelUpdate() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	}
	ulDataLen = ulDecryptedLen;
	ulDecryptedLen = TOTAL_DATA_LEN;
	ulRes = SKF_DecryptFinal(hSessionKey, g_btDecryptedData + ulDataLen, &ulDecryptedLen);
	if (0 != ulRes)
	{
		printf("SKF_DecryptFinal() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	}
	ulDataLen += ulDecryptedLen;
	//
	if (100 != ulDataLen || memcmp(g_btPlainData, g_btDecryptedData, ulDataLen) != 0)
	{
		printf("The decrypted result is not same as plain data!\n");
		goto END;
	}
	stEncryptParam.PaddingType = 0;
	//////////////////////////////带Padding的加解密数据测试结束//////////////////////////////////
	

	//	加密初始化
	printf("Encrypt is starting...\n");
	tStart = GetCurrentTime();
	ulRes = SKF_EncryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_EncryptInit() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	加密数据
	ulDataLen = 0;
	ulIndex = 0;
	do
	{
		ULONG ulThisTimeLen = 0;
		if (TOTAL_DATA_LEN - ulDataLen > PROC_DATA_LEN)
		{
			ulThisTimeLen = PROC_DATA_LEN;
		}
		else
		{
			ulThisTimeLen = (TOTAL_DATA_LEN - ulDataLen);
		}
		ulEncryptedLen = PROC_DATA_LEN + PADDED_DATA_LEN;
		ulRes = SKF_EncryptTunnelUpdate(hDev, g_btPlainData + ulDataLen, ulThisTimeLen, btEncryptedData, &ulEncryptedLen);
		if (0 != ulRes)
		{
			printf("SKF_EncryptTunnelUpdate() failed! dwRes = 0x%x\n", ulRes);
			goto END;
		}
		ulDataLen += ulThisTimeLen;
		if (ulEncryptedLen > 0)
		{
			memcpy_s(g_btCipherData + ulIndex, TOTAL_DATA_LEN, btEncryptedData, ulEncryptedLen);
			ulIndex += ulEncryptedLen;
		}
	}while (ulDataLen < TOTAL_DATA_LEN);
	
	//	加密结束
	ulEncryptedLen = PROC_DATA_LEN + PADDED_DATA_LEN;
	ulRes = SKF_EncryptFinal(hSessionKey, btEncryptedData, &ulEncryptedLen);
	if (ulEncryptedLen > 0)
	{
		memcpy_s(g_btCipherData + ulIndex, TOTAL_DATA_LEN, btEncryptedData, ulEncryptedLen);
		ulIndex += ulEncryptedLen;
	}
	tEnd = GetCurrentTime();
	printf("Encrypt is end! \nTotal time: %.02fs \nSize: %dKBytes \nSpeed: %.02fKBytes/s\n", (tEnd - tStart) / 1000.0f, (TOTAL_DATA_LEN/ 1024), (TOTAL_DATA_LEN/ 1024) * 1000.0f / (tEnd - tStart));

	//解密初始化
	printf("Decrypt is starting...\n");
	tStart = GetCurrentTime();
	ulRes = SKF_DecryptInit(hSessionKey, stEncryptParam);
	if (0 != ulRes)
	{
		printf("SKF_DecryptInit() failed! dwRes = 0x%x\n", ulRes);
		goto END;
	} 

	//解密数据
	ulDataLen = 0;
	ulIndex = 0;
	do
	{
		ULONG ulThisTimeLen = 0;
		if (TOTAL_DATA_LEN - ulDataLen > PROC_DATA_LEN)
		{
			ulThisTimeLen = PROC_DATA_LEN;
		}
		else
		{
			ulThisTimeLen = (TOTAL_DATA_LEN - ulDataLen);
		}
		ulDecryptedLen = PROC_DATA_LEN + PADDED_DATA_LEN;
		ulRes = SKF_DecryptTunnelUpdate(hDev, g_btCipherData + ulDataLen, ulThisTimeLen, btDecryptedData, &ulDecryptedLen);
		if (0 != ulRes)
		{
			printf("SKF_DecryptTunnelUpdate() failed! dwRes = 0x%x\n", ulRes);
			goto END;
		}
		ulDataLen += ulThisTimeLen;
		if (ulDecryptedLen > 0)
		{
			memcpy_s(g_btDecryptedData + ulIndex, TOTAL_DATA_LEN, btDecryptedData, ulDecryptedLen);
			ulIndex += ulDecryptedLen;
		}
	}while (ulDataLen < TOTAL_DATA_LEN);
	
	//解密结束
	ulDecryptedLen = PROC_DATA_LEN + PADDED_DATA_LEN;
	ulRes = SKF_DecryptFinal(hSessionKey, btDecryptedData, &ulDecryptedLen);
	if (ulDecryptedLen > 0)
	{
		memcpy_s(g_btDecryptedData + ulIndex, TOTAL_DATA_LEN, btDecryptedData, ulDecryptedLen);
		ulIndex += ulDecryptedLen;
	}
	tEnd = GetCurrentTime();
	printf("Decrypt is end! \nTotal time: %.02fs \nSize: %dKBytes \nSpeed: %.02fKBytes/s\n", (tEnd - tStart) / 1000.0f, (TOTAL_DATA_LEN/1024), (TOTAL_DATA_LEN/1024) * 1000.0f / (tEnd - tStart));

	//比较结果
	if (ulIndex != ulDataLen || memcmp(g_btPlainData, g_btDecryptedData, ulDataLen) != 0)
	{
		printf("The decrypted result is not same as plain data!\n");
		goto END;
	}	
	
	printf("test_SymmKeyData() pass!\n");
END:
	if (hSessionKey)
	{
		SKF_CloseHandle(hSessionKey);
		hSessionKey = NULL;
	}

	return;
}

void test_HashData(DEVHANDLE hDev)
{
	ULONG ulRes = 0;
	ULONG ulIndex = 0;
	ULONG ulDataLen = 0;
	ULONG ulHashLen = 48;
	ULONG ulHashAlg = 0;
	BYTE btInput = 0;
	HANDLE hHashKey = 0;
	ECCPUBLICKEYBLOB stEccPubKeyBlob = {0};
	BYTE btUserID[16] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	BYTE btHashData[48] = {0};
	time_t tStart = 0;
	time_t tEnd = 0;
	
	//	选择HASH算法
	printf("Please select HASH alg: [0-SHA1; 1-SM3]:");
	btInput = _getch();
	printf("%c\n", btInput);
	btInput = (btInput - 0x30);
	if (btInput == 0)
	{
		ulHashAlg = SGD_SHA1;
	}
	else if (btInput == 1)
	{
		ulHashAlg = SGD_SM3;
	}
	else
	{
		printf("Input HASH alg is wrong!\n");
		return;
	}

	//	如果是SM3，则需要读入ECC公钥数据
	if (SGD_SM3 == ulHashAlg)
	{
		CHAR csEccKeyFile[256] = {0};
		FILE* pFile = NULL;
		BYTE btEccKeyPair[1024] = {0};
		ULONG ulEccKeyPairLen = 1024;

		//	输入密钥对数据文件
		printf("Please enter the plain SM2 key pair file:");
		gets_s(csEccKeyFile, 256);
		ulRes = fopen_s(&pFile, csEccKeyFile, "r");
		if (!pFile)
		{
			printf("Open file failed!\n ulRes = 0x%x", ulRes);
			return;
		}	
	
		//	读取文件数据
		if (!helper_ReadData(pFile, btEccKeyPair, &ulEccKeyPairLen))
		{
			printf("Read file failed!\n");
			fclose(pFile);
			pFile = NULL;			
			return;
		}
		if (ulEccKeyPairLen != (4 + 2*ECC_MAX_BITS/8 + 4 + ECC_MAX_BITS/8))
		{
			printf("SM2 key pair data length is wrong! ulEccKeyPairLen = 0x%x\n", ulEccKeyPairLen);
			fclose(pFile);
			pFile = NULL;			
			return;
		}

		//	解析SM2密钥对数据,获取公钥
		memcpy(&stEccPubKeyBlob.BitLen, btEccKeyPair, 4);
		ulIndex = 4;
		memcpy(stEccPubKeyBlob.XCoordinate, btEccKeyPair + ulIndex, 64);
		ulIndex += 64;
		memcpy(stEccPubKeyBlob.YCoordinate, btEccKeyPair + ulIndex, 64);
		ulIndex += 64;

		if (pFile)
		{
			fclose(pFile);
			pFile = NULL;
		}
	}
		
	//	生成原文数据
	for (ulIndex = 0; ulIndex < TOTAL_DATA_LEN; ulIndex++)
	{
		g_btPlainData[ulIndex] = Random(255);
	}
	
	printf("Hash is starting...\n");
	tStart = GetCurrentTime();

	//	HASH初始化
	if (SGD_SM3 == ulHashAlg)
	{
		ulRes = SKF_DigestInit(hDev, SGD_SM3, &stEccPubKeyBlob, btUserID, 16, &hHashKey);
	}
	else
	{
		ulRes = SKF_DigestInit(hDev, SGD_SHA1, NULL, NULL, 0, &hHashKey);
	}
	if (0 != ulRes)
	{
		printf("SKF_DigestInit() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}

	//	对数据做Hash
	ulDataLen = 0;
	do
	{
		ulRes = SKF_DigestUpdate(hHashKey, g_btPlainData + ulDataLen, HASH_DATA_LEN);
		if (0 != ulRes)
		{
			printf("SKF_Digest() failed! ulRes = 0x%x\n", ulRes);
			goto END;
		}
		ulDataLen += HASH_DATA_LEN;
	}
	while(ulDataLen < TOTAL_DATA_LEN);

	//	获取HASH结果
	ulRes = SKF_DigestFinal(hHashKey, btHashData, &ulHashLen);
	if (0 != ulRes)
	{
		printf("SKF_DigestFinal() failed! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	tEnd = GetCurrentTime();
	printf("Hash is end! \nTotal time: %.02fs \nSize: %dKBytes \nSpeed: %.02fKBytes/s\n", (tEnd - tStart) / 1000.0f, (TOTAL_DATA_LEN/ 1024), (TOTAL_DATA_LEN/ 1024) * 1000.0f / (tEnd - tStart));

	//	输出结果	
	printf("Hash length = 0x%x, data:\n", ulHashLen);
	for (ulIndex = 0; ulIndex < ulHashLen; ulIndex++)
	{
		printf("%02X ", btHashData[ulIndex]);
	}
	printf("\n");
	printf("test_EccSignVerify() pass!\n");
END:
	if (hHashKey)
	{
		SKF_CloseHandle(hHashKey);
		hHashKey = NULL;
	}

	return;
}

ULONG helper_SelectDevice()
{	
	ULONG ulRes = 0;
	ULONG ulSelDev = -1;
	ULONG ulDevCount = 0;
	ULONG ulDevNamesLen = 0;
	BYTE btInput = 0;
	LPSTR lpName = NULL;
	LPSTR lpscDevNames = NULL;

	ulRes = SKF_EnumDev(TRUE, NULL, &ulDevNamesLen);
	if (SAR_OK != ulRes || ulDevNamesLen <= 2)
	{
		printf("No any device has found! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	lpscDevNames = new CHAR[ulDevNamesLen];
	memset(lpscDevNames, 0, ulDevNamesLen);
	ulRes = SKF_EnumDev(TRUE, lpscDevNames, &ulDevNamesLen);
	if (SAR_OK != ulRes || ulDevNamesLen <= 2)
	{
		printf("No any device has found! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	//	显示设备列表
	printf("Please select a device:\n");
	lpName = lpscDevNames;
	do
	{
		strcpy_s(g_csDevNames[ulDevCount], 128, lpName);
		printf("%d : %s\n", ++ulDevCount, lpName);
		lpName += strlen(lpName) + 1;
		if (*lpName == '\0' || ulDevCount >= MAX_DEV_CNT)
		{
			break;
		}

	}while(1);

	btInput = _getch();
	printf("%c\n", btInput);
	ulSelDev = (btInput-0x30) - 1;
	if (ulSelDev < 0 || ulSelDev >= ulDevCount)
	{
		printf("The inputed device index is wrong!\n");
		ulSelDev = -1;
		goto END;
	}

END:
	if (lpscDevNames)
	{
		delete []lpscDevNames;
		lpscDevNames = NULL;
	}
	return ulSelDev;
}

ULONG helper_SelectApplication(DEVHANDLE hDev)
{	
	ULONG ulRes = 0;
	ULONG ulSelApp = -1;
	ULONG ulAppCount = 0;
	ULONG ulAppNamesLen = 0;
	BYTE btInput = 0;
	LPSTR lpName = NULL;
	LPSTR lpscAppNames = NULL;

	ulRes = SKF_EnumApplication(hDev, NULL, &ulAppNamesLen);
	if (SAR_OK != ulRes || ulAppNamesLen <= 2)
	{
		printf("No any applcation has found! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	lpscAppNames = new CHAR[ulAppNamesLen];
	memset(lpscAppNames, 0, ulAppNamesLen);
	ulRes = SKF_EnumApplication(hDev, lpscAppNames, &ulAppNamesLen);
	if (SAR_OK != ulRes || ulAppNamesLen <= 2)
	{
		printf("No any applcation has found! ulRes = 0x%x\n", ulRes);
		goto END;
	}
	
	printf("Please select a applcation:\n");
	lpName = lpscAppNames;
	do
	{
		strcpy_s(g_csAppNames[ulAppCount], 64, lpName);
		printf("%d : %s\n", ++ulAppCount, lpName);
		lpName += strlen(lpscAppNames) + 1;
		if (*lpName == '\0' || ulAppCount >= MAX_APP_CNT)
		{
			break;
		}

	}while(1);

	btInput = _getch();
	printf("%c\n", btInput);
	ulSelApp = (btInput-0x30) - 1;
	if (ulSelApp < 0 || ulSelApp >= ulAppCount)
	{
		printf("The inputed applcation index is wrong!\n");
		ulSelApp = -1;
		goto END;
	}

END:
	if (lpscAppNames)
	{
		delete []lpscAppNames;
		lpscAppNames = NULL;
	}
	return ulSelApp;
}

ULONG helper_WriteData(FILE* file, BYTE* lpData, ULONG ulDataLen)
{
	ULONG ulIndex = 0;
	ULONG ulWritten = 0;

	if (!file || !lpData || 0 == ulDataLen)
	{
		return 0;
	}

	for (ulIndex = 0; ulIndex < ulDataLen; ulIndex++)
	{
		fprintf(file, "%02X ", lpData[ulIndex]);
	}
	
	return ulIndex;
}

ULONG helper_ReadData(FILE* file, BYTE* lpData, ULONG *pulDataLen)
{	
	ULONG ulIndex = 0;
	ULONG ulWritten = 0;
	ULONG ulData = 0;

	if (!file || !lpData || !pulDataLen || 0 == *pulDataLen)
	{
		return 0;
	}

	while (!feof(file))
	{
		if (ulIndex == *pulDataLen)
		{
			break;
		}
		int n = fscanf_s(file, "%02X ", &ulData, 4);
		if (1 == n)
		{
			lpData[ulIndex++] = (BYTE)ulData;
		}
		else
		{
			return 0;
		}
	}
	*pulDataLen = ulIndex;
	
	return ulIndex;
}




