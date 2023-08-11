/***************************************************************************
 *	File name:SMARTCARD.h
 *	Introduce:Functions definition for smard card object
 *	Author:Syno common
 *	Date:2015/07/18
 *  Version:v1.0
 **************************************************************************/

#ifndef _SMARTCARD_H_
#define _SMARTCARD_H_

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "SynoHeaders.h"

/*----------------------------------------------*
 * ���������                                 *
 *----------------------------------------------*/
#define	CMD_SET_LABEL				        0x02 /* ���ñ�ǩ */
#define	CMD_GET_DEVINFO				        0x04 /* ��ȡ�豸��Ϣ */
/* ���ʿ��� */
#define	CMD_GET_AUTH				        0x10 /* �豸��֤ */
#define	CMD_CHANGE_DEVAUTHKEY	            0x12 /* �޸��豸��Կ */
#define	CMD_GET_PININFO				        0x14 /* ��ȡPIN��Ϣ */
#define	CMD_CHANGE_PIN				        0x16 /* �޸�PIN */
#define	CMD_VERIFY_PIN				        0x18 /* У��PIN */
#define	CMD_UNBLOCK_PIN				        0x1A /* ����PIN */
#define	CMD_CLEAR_SECURESTATE		        0x1C /* �����ȫ״̬ */
/* Ӧ�ù��� */
#define	CMD_CREATE_APPLICATION		        0x20 /* ����Ӧ�� */
#define	CMD_ENUM_APPLICATION		        0x22 /* ö��Ӧ�� */
#define	CMD_DELETE_APPLICATION		        0x24 /* ɾ��Ӧ�� */
#define	CMD_OPEN_APPLICATION		        0x26 /* ��Ӧ�� */
#define	CMD_CLOSE_APPLICATION		        0x28 /* �ر�Ӧ�� */
/* �ļ����� */
#define	CMD_CREATE_FILE				        0x30 /* �����ļ� */
#define	CMD_DELETE_FILE				        0x32 /* ɾ���ļ� */
#define	CMD_ENUM_FILE				        0x34 /* ö���ļ� */
#define	CMD_GET_FILEINFO			        0x36 /* ��ȡ�ļ���Ϣ */
#define	CMD_READ_FILE				        0x38 /* ���ļ� */
#define	CMD_WRITE_FILE				        0x3A /* д�ļ� */
/* �������� */
#define	CMD_CREATE_CONTAINER		        0x40 /* �������� */
#define	CMD_OPEN_CONTAINER			        0x42 /* ������ */
#define	CMD_CLOSE_CONTAINER		            0x44 /* �ر����� */
#define	CMD_ENUM_CONTAINER			        0x46 /* ö������ */
#define	CMD_DELETE_CONTAINER		        0x48 /* ɾ������ */
#define	CMD_GET_CONTAINERINFO				0x4A /* ��ȡ������Ϣ*/
#define CMD_CONTYPE					        0xE6 /* ��ȡ�������� */
#define	CMD_IMPORT_CERTIFICATE		        0x4C /* ����֤�� */
#define	CMD_EXPORT_CERTIFICATE		        0x4E /* ����֤�� */
/* ������� */
#define	CMD_GEN_RANDOM				        0x50 /* ��������� */
/* RSA */
#define	CMD_GEN_EXTRSAKEY			        0x52 /* �����ⲿRSA��Կ�� */
#define	CMD_GEN_RSAKEYPAIR			        0x54 /* ����RSAǩ����Կ�� */
#define	CMD_IMPORT_PLAINRSAKEY				0xD8 /* ��������RSA������Կ�� */
#define CMD_IMPORT_RSAPUBKEY				0xA5 /* ����һ��RSA��Կ(��P11ʹ��) */
#define CMD_IMPORT_RSAPRIKEY				0xA7 /* ����һ��RSA˽Կ(��P11ʹ��) */
#define	CMD_IMPORT_RSAKEYPAIR		        0x56 /* ����RSA������Կ�� */
#define	CMD_RSASIGNDATA				        0x58 /* RSAǩ�� */
#define CMD_RSAVERIFY				        0x5E /* RSA��ǩ */
#define	CMD_RSA_EXPORTSESSIONKEY	        0x5A /* RSA���ɲ������Ự��Կ */
#define	CMD_RSA_EXPORTSESSIONKEYEX			0x5C /* RSA�����Ự��Կ */
#define CMD_ExtRSA_PubOPT			        0xEA /* RSA������Կ���� */
#define CMD_ExtRSA_PriOPT			        0xE8 /* RSA����˽Կ���� */
#define	CMD_RSAEXTKEY_OPT			        0x60 /* RSA�������(���ܻ��߽���) */
/* ECC */
#define	CMD_GEN_ECCKEYPAIR			        0x70 /* ����ECCǩ����Կ�� */
#define	CMD_IMPORT_ECCKEYPAIR		        0x72 /* ����ECC������Կ�� */
#define	CMD_ECCSIGNDATA				        0x74 /* ECCǩ�� */
#define	CMD_ECCVERIFY				        0x76 /* ECC��ǩ */
#define	CMD_ECC_EXPORTSESSIONKEY	        0x78 /* ECC�����������Ự��Կ */
#define	CMD_ECC_EXPORTSESSIONKEYEX	        0x80 /* ECC�����������Ự��Կ */
#define	CMD_ECC_ENCRYPT				        0x7A /* �ⲿECC���� */
#define	CMD_ECC_DECRYPT				        0x7C /* �ⲿECC���� */
#define	CMD_EXT_ECCSIGN				        0x7E /* �ⲿECCǩ�� */
#define CMD_EXT_ECCVERIFY			        0x7F /* �ⲿECC��ǩ */
#define	CMD_GEN_AGREEDATA_ECC		        0x82 /* ECC������ԿЭ�̲��� */
#define	CMD_GEN_AGREEDATAKEY_ECC	        0x84 /* ECC����Э�̲���������Ự��Կ */
#define	CMD_GEN_KEY_ECC	        	        0x86 /* ECC����Ự��Կ */
/* ���ݼӽ��� */
#define	CMD_EXPORT_PUBKEY			        0x88 /* ������Կ */
#define	CMD_IMPORT_SESSIONKEY		        0xA0 /* ����Ự��Կ */
#define	CMD_IMPORT_SYMMKEY		            0xA2 /* ���ĵ���Ự��Կ */
#define	CMD_GENERATE_SYMMKEY                0xA3 /* ֱ������һ���Ự��Կ */
#define	CMD_ENCRYPT_INIT			        0xA4 /* ���ܳ�ʼ�� */
#define	CMD_ENCRYPT					        0xA6 /* �������ݼ��� */
#define	CMD_ENCRYPT_UPDATA			        0xA8 /* �������ݼ��� */
#define	CMD_ENCRYPT_FINAL			        0xAA /* �������� */
#define	CMD_DECRYPT_INIT			        0xAC /* ���ܳ�ʼ�� */
#define	CMD_DECRYPT					        0xAE /* �������ݽ��� */
#define	CMD_DECRYPT_UPDATA			        0xB0 /* �������ݽ��� */
#define	CMD_DECRYPT_FINAL			        0xB2 /* �������� */
#define	CMD_DIGEST_INIT				        0xB4 /* �����Ӵճ�ʼ�� */
#define	CMD_DIGEST					        0xB6 /* �������������Ӵ� */
#define	CMD_DIGEST_UPDATA			        0xB8 /* �������������Ӵ� */
#define	CMD_DIGEST_FINAL			        0xBA /* ���������Ӵ� */
#define	CMD_MAC_INIT				        0xBC /* ��Ϣ�����������ʼ�� */
#define	CMD_MAC						        0xBE /* ����������Ϣ���������� */
#define	CMD_MAC_UPDATA				        0xC0 /* ����������Ϣ���������� */
#define	CMD_MAC_FINAL				        0xC2 /* ������Ϣ���������� */
#define	CMD_DESTROY_SESSIONKEY	            0xC4 /* ���ٻỰ��Կ */


/*----------------------------------------------*
 * �����������Ͷ���                             *
 *----------------------------------------------*/

/* APDU����ṹ���� */
//typedef struct tagAPDUCmd
//{
//	UCHAR CLA;
//	UCHAR INS;
//	UCHAR P1;
//	UCHAR P2;
//	USHORT LC;
//	USHORT LE;
//} APDUCMD_S;

/* APDU����ͷ�ṹ���� */
typedef struct tagAPDUHeader
{
	UINT8 CLA;
	UINT8 INS;
	UINT8 P1;
	UINT8 P2;
}APDUHEADER_S;

/* ������������ */
typedef enum tagCmdType
{
	CMD,
	ENC,
	DEC
} CMDTYPE_E;

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/
#define CMD_HEAD_LEN        sizeof(APDUHEADER_S)	/* APDU����ͷ���� */
#define MAX_LC_LEN			(MAX_CMD_BUF - 6)		/* �����������󳤶� */
#define MAX_APDU_LC_LEN		(MAX_LC_LEN - (CMD_HEAD_LEN+3+2))	/* �����������󳤶� */

#ifdef __cplusplus
extern "C" {
#endif

ULONG APDUInterface(HANDLE hHandle,
    UCHAR *pInBuf, ULONG ulInLen, UCHAR *pOutBuf, ULONG *pulOutLen);

ULONG DataInterface(HANDLE hHandle,
    UCHAR *pInBuf, ULONG ulInLen, UCHAR *pOutBuf, ULONG *pulOutLen, CMDTYPE_E eCmdType);

ULONG CMD_Transmit(HANDLE hHandle, UCHAR INS, UCHAR P1, UCHAR P2,
    UCHAR *pucIn, ULONG ulInLen, UCHAR *pucOut, ULONG *pulOutLen);

ULONG CMD_TransmitEx(HANDLE hHandle, UCHAR INS, UCHAR P1, UCHAR P2,
    UCHAR *pucIn, ULONG ulInLen, UCHAR *pucOut, ULONG *pulOutLen);

#ifdef __cplusplus
}
#endif

#endif /* _SMARTCARD_H_ */

