/***************************************************************************
 *  File name:Command.h
 *  Function:Header of Command.c
 *  Author:Syno Common
 *  Date:2015/06/20
 *  Version:v1.0
****************************************************************************/
#ifndef _COMMAND_H_
#define _COMMAND_H_

#include    "SM2.h"

typedef UINT8  	BYTE;
typedef INT8  	CHAR;
typedef INT16 	SHORT;
typedef UINT16 	USHORT;
typedef INT32 	LONG;
typedef UINT32  ULONG;
typedef UINT32  UINT;
typedef UINT16 	WORD;
typedef UINT32	DWORD;
typedef UINT32 	FLAGS;
typedef CHAR * 	LPSTR;
typedef void * 	HANDLE;
typedef HANDLE 	DEVHANDLE;
typedef HANDLE 	HAPPLICATION;
typedef HANDLE 	HCONTAINER;

#define DEFAULT_PIN_VERSION 0

//��Կ�ļ�ID 
#define EXCERTID    0xFFF0
#define SIGNCERTID  0xFFF1
#define EXKEYID     0xFFF2
#define SIGNKEYID   0xFFF3
#define PINFILEID   0xFFF4
        
//�豸��ʼ����Ϣ
#define CARD_LOCKED    0x02         //���Ѿ�����������ʹ��
#define CARD_INITED    0x01         //���Ѿ���ʼ��(���м�����Կ��)
#define CARD_NOTINITED 0x00        	//��û�г�ʼ��(�޼�����Կ��)��������ʹ��

//�������Ͷ���                                               
#define RSA_CON         1
#define ECC_CON         2        
#define	ECC_KEY_BITS	256

// ���������㷨��ʶ
#define SGD_SM1_ECB  		0x00000101  //SM1 �㷨 ECB����ģʽ 
#define SGD_SM1_CBC  		0x00000102  //SM1 �㷨 CBC����ģʽ 
#define SGD_SM1_CFB  		0x00000104  //SM1 �㷨 CFB����ģʽ 
#define SGD_SM1_OFB  		0x00000108  //SM1 �㷨 OFB����ģʽ 
#define SGD_SM1_MAC  		0x00000110  //SM1 �㷨 MAC���� 
#define SGD_SSF33_ECB  	    0x00000201  //SSF33�㷨ECB����ģʽ  
#define SGD_SSF33_CBC  	    0x00000202  //SSF33�㷨CBC����ģʽ 
#define SGD_SSF33_CFB  	    0x00000204  //SSF33�㷨CFB����ģʽ 
#define SGD_SSF33_OFB  	    0x00000208  //SSF33�㷨OFB����ģʽ 
#define SGD_SSF33_MAC  	    0x00000210  //SSF33�㷨MAC���� 
#define SGD_SMS4_ECB  	    0x00000401  //SMS4�㷨 ECB����ģʽ 
#define SGD_SMS4_CBC  	    0x00000402  //SMS4�㷨 CBC����ģʽ 
#define SGD_SMS4_CFB  	    0x00000404  //SMS4�㷨 CFB����ģʽ 
#define SGD_SMS4_OFB  	    0x00000408  //SMS4�㷨 OFB����ģʽ 
#define SGD_SMS4_MAC  	    0x00000410  //SMS4�㷨 MAC����
#define	SGD_AES128_CBC	    0x00000501	//AES 128λ(16bytes) CBC�����㷨
#define	SGD_AES128_ECB	    0x00000502	//AES 128λ(16bytes) ECB�����㷨
#define	SGD_AES192_CBC	    0x00000504  //AES 192λ(24bytes) CBC�����㷨 
#define	SGD_AES192_ECB	    0x00000508  //AES 192λ(24bytes) ECB�����㷨 
#define	SGD_AES256_CBC	    0x00000510	//AES 256λ(32bytes) CBC�����㷨 
#define	SGD_AES256_ECB	    0x00000511	//AES 256λ(32bytes) ECB�����㷨
#define SGD_3DES  	        0x00000410  //SMS4�㷨 MAC���� 
#define SGD_SM1  			0x0000011F  //SM1 �㷨 ECB����ģʽ
#define SGD_SMS4  			0x0000041F  //SMS4�㷨 MAC����

// �ǶԳ������㷨��ʶ
#define SGD_RSA  			0x00010000  //RSA �㷨 
#define SGD_SM2_1  			0x00020100  //��Բ����ǩ���㷨 
#define SGD_SM2_2  			0x00020200  //��Բ������Կ����Э
#define SGD_SM2_3  			0x00020400  //��Բ���߼����㷨 

// �����Ӵ��㷨��ʶ
#define SGD_SM3  			0x00000001  //SM3 �����Ӵ��㷨 
#define SGD_SHA1  			0x00000002  //SHA1�����Ӵ��㷨 
#define SGD_SHA256  		0x00000004  //SHA256 �����Ӵ��㷨 
          
//�Գ��㷨����
#define ALG_NO		        0
#define ALG_SM1_ECB         1
#define ALG_SM1_CBC         2
#define ALG_DES_ECB         3
#define ALG_DES_CBC         4
#define ALG_DES3_ECB        5
#define ALG_DES3_CBC        6
#define ALG_AES128_ECB      7
#define ALG_AES128_CBC      8
#define ALG_AES192_ECB      9
#define ALG_AES192_CBC      10
#define ALG_AES256_ECB      11 
#define ALG_AES256_CBC      12 

//Ȩ������
#define SECURE_NEVER_ACCOUNT  	0x00000000  //������ 
#define SECURE_ADM_ACCOUNT  	0x00000001  //����ԱȨ��
#define SECURE_USER_ACCOUNT  	0x00000010  //�û�Ȩ�� 
#define SECURE_ANYONE_ACCOUNT  	0x000000FF  //�κ��� 

//�豸״̬
#define DEV_ABSENT_STATE  		0x00000000  // �豸������ 
#define DEV_PRESENT_STATE  		0x00000001  // �豸���� 
#define DEV_UNKNOW_STATE  		0x00000002  // �豸״̬δ֪

//PIN������
#define KT_MC		            0x00	//
#define KT_SOPIN	            0x00	//����Ա
#define KT_USERPIN	            0x01	//�û�    
        
//Ȩ��״̬
#define SA_SOLOGIN    	        12//����Ա��½ 
#define SA_USERLOGIN  	        6 //�û���½ 
#define SA_EXTERAUTH  	        1 //�ⲿ��֤
#define SA_NO         	        0 //��Ȩ�� 

//����״̬
#define CON_CARDLOCK            0x01
#define CON_APPLOCKTEMP         0x02
#define CON_APPLOCKEVER         0x04
#define CON_SA			        0x08
#define CON_ALL                 0x0F 

//��������
#define MAX_APDU_LEN                2048
#define DEFAULTSESSIONKEYLEN        32
#define DEFAULTDEVKEYLEN 		    16
#define DEVICEID 				    0xA0A0  //��ԿЭ��ʱʹ��
#define MAX_IV_LEN                  32
#define SESSIONKEY_MAX              0x08
#define APP_MAX                     0x08
#define CON_MAX		                0x0A
#define SESSIONKEY_EXTDATA_ADDR	    0x20008800	//Session key ��չ���ݵ�ַ
#define	SESSIONKEY_EXTDATA_LEN	    240			//Session key ��չ�������ݳ��� 
//
#define CHIP_SERIALNUMBER		    32
#define	MAX_LABLE_LEN				32
#define	MAX_APPLICATIONNAME_LEN		32 
#define	MAX_FILENAME_LEN			32
#define	MAX_CONTAINERNAME_LEN		34
//
#define MASK_CHK_APP_ID				0x01
#define MASK_CHK_CONT_ID			0x02
#define MASK_CHK_KEY_ID				0x04
#define SANITY_CHK_APP_ID			0x01
#define SANITY_CHK_APP_CONT_ID		0x03
#define SANITY_CHK_APP_CONT_KEY_ID	0x07

//��ȫ�洢����ǰ��50K���ļ�ϵͳ  �ļ�ϵͳ���濪ʼΪ	FLASH_START���洢���ݡ�
#define FLASHPage       1024   //1k�ռ�
#define HalfFLASHPage   512    //512�ռ�
#define FLASH_START 	57*FLASHPage 

#define SYSINFO_OFFSET  FLASH_START       //ϵͳ��������ʼ��ַ 
#define SYSINFO_SIZE    0				  //ϵͳ�������� 

#define HAGREEMENTKEY_OFFSET (SYSINFO_OFFSET+SYSINFO_SIZE)//AgreementKey�����ʼ��ַ 
#define HAGREEMENTKEY_SIZE   sizeof(HAGREEMENTKEY)//AgreementKey�ܵĳ��� 

#define SESSIONKEY_OFFSET (HAGREEMENTKEY_OFFSET+HAGREEMENTKEY_SIZE)//SessionKey�����ʼ��ַ 
#define SESSIONKEY_SIZE   SESSIONKEY_MAX*32 //SessionKey�ܵĳ��� 

 /*
 *  �Գ���Կ��;
 */                     
typedef	enum	
{
    CT_UNKNOWN  = 0x00, //Init value
    CT_ENC      = 0x01, //Encrypt
    CT_DEC      = 0x02, //Decrypt
    CT_DIGEST   = 0x03, //DIGEST
    CT_MAC      = 0x04, //MAC
    CT_MAX              //MAX CT
}CRYPTO_TYPE;

/*
 *  APDU��ͷ�ṹ
 */
typedef struct tagAPDUHeader
{
	UINT8 CLA;
	UINT8 INS;
	UINT8 P1;
	UINT8 P2;
}APDUHEADER_S;

/*
 *  APDU����ṹ
 */
typedef struct tagAPDUBody
{
	UINT32  LC;
	UINT32  LE;
	UINT8   *DATA;
}APDUBODY_S;

/*
 *  APDU��Ӧ���ṹ
 */
typedef struct tagAPDUResp
{
	UINT16 LEN;
    UINT16 SW;
	UINT8  *DATA;
	UINT8 SW1;
}APDURESP_S;

/*
 *  �Գ���Կ���ݽṹ��
 */ 
typedef struct _SESSIONKEYDATA
{
	UINT8 	flag;			//0:ready,1:in using; 2: init done
    CRYPTO_TYPE crypto_type;// DEC/ENC/MAC
	UINT16 	keyID;	  		//key handle
	UINT32 	algID;			//alg ID 
	UINT16	keyLen;			//key data length
	UINT8 	keyData[32];	//session key data
	UINT8 	extData[SESSIONKEY_EXTDATA_LEN]; 	//session key extern data
	UINT8 	IV[16];
}SESSIONKEYDATA;

/*
 *  Ӧ��״̬�ṹ��
 */
typedef struct _APPSTATE
{
	UINT16 AppID;
	UINT16 ContainerID[CON_MAX];
    UINT8  AppState;
	UINT8  AppRand[16];	
}APPSTATE;

/*
 *  ��Կ�ļ����Խṹ��
 */
typedef struct _KEYFILE{
	UINT8 Index;
	UINT8 KeyType;
	UINT8 Version;
	UINT32 AlgFlag;
	UINT8 UseControl;
	UINT8 ModifyControl;
	UINT32 RetryTimes;
	UINT32 LeftTryTimes;
	UINT8 Key[32];
}KEYFILE;

/*
 *  ���ܲ����ṹ��
 */
typedef struct _BLOCKCIPERPARAM
{
	BYTE IV[MAX_IV_LEN];
	UINT32 IVLen;
	UINT32 PaddingType;
	ULONG  FeedBitLen;
}BLOCKCIPHERPARAM,*PBLOCKCIPHERPARAM;

/*
 *  Ӧ�����Խṹ��
 */
typedef struct _APPINFO{
   	KEYFILE key[2];	//0:SO_PIN;1:USER_PIN
   	ULONG 	dwCreateFileRights;
	BYTE    byContainerNum;
	BYTE    byCertNum;
	WORD    wFileNum; 
}APPINFO;

typedef struct{
    UINT32 RandData[16];
    Ep_Point epSponsor;
    UINT8    pbID[32];
    UINT32   ulIDLen;
    UINT32   hID;       
}HAGREEMENTKEY;

/*
 *  �����ļ����Խṹ��
 */
typedef struct _FILEATT{
   	BYTE    name[48];
   	ULONG 	filesize;
	ULONG   readright;
	ULONG   writeright;
}FILEATT;

/*
 *  COS�汾��Ϣ
 */
typedef struct Struct_Version{
	BYTE major; 
	BYTE minor; 
}VERSION;

/*
 *  ����Ӧ�ò����ṹ��
 */
typedef struct Struct_cosAPPLICATIONINFO{ 
	CHAR 	szApplicatinName[48];      //Ӧ�����ƣ����� 32 �ֽ������� 0x00 ��ȫ 
	CHAR 	szAdminPin[16];            //����Ա������� 16�ֽ������� 0x00 ��ȫ 
	ULONG   dwAdminPinRetryCount;      //����Ա�������Դ��� 
	CHAR 	szUserPin[16];             //�û�������� 16 �ֽ������� 0x00 ��ȫ 
	ULONG   dwUserPinRetryCount;       //�û��������Դ��� 
	ULONG 	dwCreateFileRights;        //��Ӧ���´����ļ���Ȩ�� 
	BYTE    byContainerNum;            //Ҫ��Ӧ��֧�ֵ��������� 
	BYTE    byCertNum;                 //Ҫ��Ӧ��֧�ֵ�֤������ 
	WORD    wFileNum;                  //Ҫ��Ӧ��֧�ֵ��ļ����� 
} cosAPPLICATIONINFO; 
               
/*
 *  COS��Ϣ�ṹ��
 */
#pragma pack(push) 	//�������״̬
#pragma pack(1)		//�趨Ϊ1�ֽڶ���
typedef struct struct_devprv{
	VERSION    Version;
	CHAR       Manufacturer[64];
	CHAR       Issuer[64];
	CHAR       Label[32];
	CHAR       SerialNumber[32];
	VERSION    HWVersion;
	VERSION    FirmwareVersion;
	ULONG      AlgSymCap;
	ULONG	   AlgAsymCap;
	ULONG	   AlgHashCap;
	ULONG      DevAuthAlgId;
	ULONG      TotalSpace;
	ULONG      FreeSpace;
	BYTE	   Rev[64];
}DEVINFOPRI;

typedef struct struct_cosDEVINFO{
	VERSION    StructVersion;
	VERSION    SpecificationVersion;
	CHAR       Manufacturer[64];
	CHAR       Issuer[64];
	CHAR       Label[32];
	CHAR       SerialNumber[32];
	VERSION    HWVersion;
	VERSION    FirmwareVersion;
	ULONG      AlgSymCap;
	ULONG	   AlgAsymCap;
	ULONG	   AlgHashCap;
	ULONG      DevAuthAlgId;
	ULONG      TotalSpace;
	ULONG      FreeSpace;
    WORD       MaxApduDataLen;
    WORD       UserAuthMethod;
    WORD       DeviceType;
    BYTE       MaxContainerNum;
    BYTE       MaxCertNum;
    WORD       MaxFileNum;
	BYTE	   Rev[54];
}cosDEVINFO;
#pragma pack(pop)//�ָ�����״̬

/*
 *�����豸��Ϣ���豸��Կ��Ϣ����״̬
 */ 
typedef struct struct_DEV{
	DEVINFOPRI dev;
	UINT8      CardStatus;
	KEYFILE    DevKey[2];
}DEVINFO;
 
/*
 *  �������
 */
#define SAR_OK                                  0x9000	//�ɹ�
#define	SAR_PIN_INVALID							0x63C0	//��֤ʧ��,��ʣX�λ���(XΪ���һλֵ���趯̬���)
#define	SAR_WRITEFILEERR						0x6581	//д�ļ�ʧ��
#define	SAR_INDATALENERR						0x6700	//LC���ȴ���
#define SAR_GENRSAKEYERR                        0x6915  //������Կ��ʧ��
#define	SAR_SASTATUSERR							0x6982	//�����㰲ȫ״̬ 
#define	SAR_PIN_LOCKED							0x6983	//��֤��������
#define	SAR_INDATAERR							0x6984	//����������Ч
#define	SAR_INVCONDERR							0x6985	//ʹ������������
#define	SAR_INVALID_CMD							0x6986	//���������
#define	SAR_INVALIDSM_OBJECT					0x6988	//SM���ݶ�����ȷ
#define	SAR_APPLICATION_NOT_OPEN				0x698A	//û�д�Ӧ��
#define	SAR_INVALIDDATA							0x6A80	//��������ȷ(��PIN����ܵĲ������) 
#define	SAR_UNSUPPORTFUNC						0x6A81	//���ܲ���֧��
#define	SAR_NO_ROOM								0x6A84	//�豸�ռ䲻��   
#define	SAR_INVALIDPARAMERR						0x6A86	//P1P2��������ȷ 
#define	SAR_APPLICATION_NOT_EXISTS				0x6A88	//ָ����Ӧ�ò�����
#define	SAR_APPLICATION_EXISTS					0x6A89	//Ӧ���Ѿ����� 
#define	SAR_APPLICATION_ALREADY_OPEN			0x6A8A	//Ӧ���Ѵ�  	  
#define	SAR_APPLICATIONNAME_NOT_EXISTS			0x6A8B	//���õ�Ӧ����δ�ҵ� 
#define	SAR_SYMM_KEY_NOT_EXISTS					0x6A8C	//�Գ���Կ������
#define	SAR_DATA_ERR							0x6A8D	//���ݴ���
#define	SAR_UNSUPPORT_MULTI_APP					0x6A90	//�Ѵ�Ӧ�ã���֧�ֶ��Ӧ��ͬʱ��
#define	SAR_CONTAINERNAME_NOT_EXISTS			0x6A91	//ָ����������������
#define	SAR_FILE_ALREADY_EXIST					0x6A92	//�ļ��Ѵ���
#define	SAR_FILE_NOT_EXIST						0x6A93	//�ļ�������
#define	SAR_CONTAINERID_NOT_EXISTS				0x6A94	//ָ��������ID������ 
#define	SAR_KEYNOTFOUNTERR						0x6A95	//������û�ж�Ӧ����Կ���ļ�
#define	SAR_CERTNOTFOUNTERR						0x6A96	//ָ�����͵�֤�鲻����
#define	SAR_WRITEDATAERR						0x6A97	//����д��ʧ��
#define	SAR_SIGNVERYFIERR						0x6A98	//��֤ǩ��ʧ��/��Կ����ʧ��
#define	SAR_UNSUPPORT_SESSION_KEY				0x6A99	//��֧�ֵĻỰ��Կ�㷨��ʶ
#define	SAR_ASYMM_ENC_FAIL						0x6A9A	//�ǶԳƼ���ʧ��
#define	SAR_ASYMM_DEC_FAIL						0x6A9B	//�ǶԳƽ���ʧ��
#define	SAR_PRI_KEY_SIGN_FAIL					0x6A9C	//˽Կǩ��ʧ��
#define	SAR_UNSUPPORT_HASH_ALG					0x6A9D	//��֧��ժҪ�㷨
#define	SAR_NEED_MORE_DATA						0x6A9E	//���и��������ϴ�
#define	SAR_KEY_NOT_EXISTS						0x6A9F	//���õ���Կδ�ҵ�
#define	SAR_FILEOFFSETERR						0x6B00	//ƫ���������ļ�����
#define	SAR_KEY_EXCHANGE_DATA_FAIL				0x6B01	//������ԿЭ������ʧ��
#define	SAR_GEN_KEY_EXCHANGE_FAIL				0x6B02	//����Э����Կʧ��
#define	SAR_OUTDATALENERR						0x6C00	//LE����ȷ
#define	SAR_CLAERR								0x6E00	//CLA����ȷ
#define	SAR_READSIZE_OVERFLOW					0x6E01	//�����ص����ݳ��ȳ�����󻺳������� 
#define	SAR_CONTAINER_ALREADY_EXISTS			0x6E02	//ָ���������Ѿ�����

 
/*
 *  ���ܱ�׼�����
 */
#define	CMD_SET_LABEL				0x02	//���ñ�ǩ
#define	CMD_GET_COSINFO				0x04	//��ȡ�豸��Ϣ
#define	CMD_GET_AUTH				0x10	//�豸��֤
#define	CMD_CHANGE_DEVAUTHKEY	    0x12	//�޸��豸��Կ
#define	CMD_GET_PININFO				0x14	//��ȡPIN��Ϣ
#define	CMD_CHANGE_PIN				0x16	//�޸�PIN
#define	CMD_VERIFY_PIN				0x18	//У��PIN
#define	CMD_UNBLOCK_PIN				0x1A	//����PIN
#define	CMD_CLEAR_SECURESTATE		0x1C	//�����ȫ״̬
#define	CMD_CREATE_APPLICATION		0x20	//����Ӧ��
#define	CMD_ENUM_APPLICATION		0x22	//ö��Ӧ��
#define	CMD_DELETE_APPLICATION		0x24	//ɾ��Ӧ��
#define	CMD_OPEN_APPLICATION		0x26	//��Ӧ��
#define	CMD_CLOSE_APPLICATION		0x28	//�ر�Ӧ��
#define	CMD_CREATE_FILE				0x30	//�����ļ�
#define	CMD_DELETE_FILE				0x32	//ɾ���ļ�
#define	CMD_ENUM_FILE				0x34	//ö���ļ�
#define	CMD_GET_FILEINFO			0x36	//��ȡ�ļ���Ϣ
#define	CMD_READ_FILE				0x38	//���ļ�
#define	CMD_WRITE_FILE				0x3A	//д�ļ�
#define	CMD_CREATE_CONTAINER		0x40	//��������
#define	CMD_OPEN_CONTAINER			0x42	//������
#define	CMD_CLOSE_CONTAINER		    0x44	//�ر�����
#define	CMD_ENUM_CONTAINER			0x46	//ö������
#define	CMD_DELETE_CONTAINER		0x48	//ɾ������
#define	CMD_GET_CONTAINERINFO		0x4A	//��ȡ������Ϣ
#define	CMD_IMPORT_CERTIFICATE		0x4C	//����֤��
#define	CMD_EXPORT_CERTIFICATE		0x4E	//����֤��
#define	CMD_GEN_RANDOM				0x50	//���������
#define	CMD_GEN_RSAKEYPAIR			0x54	//����RSA��Կ��
#define	CMD_IMPORT_RSAKEYPAIR		0x56	//����RSA��Կ��
#define	CMD_RSASIGNDATA				0x58	//RSAǩ��
#define	CMD_RSAVERIFY				0x5E	//RSA��ǩ
#define	CMD_RSA_EXPORTSESSIONKEY	0x5A	//RSA���ɲ������Ự��Կ	
#define	CMD_RSA_EXPORTSESSIONKEYEX	0x5C	//RSA�����Ự��Կ
#define	CMD_RSAEXTKEY_OPT			0x60	//RSA������˽Կ����	
#define	CMD_GEN_ECCKEYPAIR			0x70	//����ECC��Կ��
#define	CMD_IMPORT_ECCKEYPAIR		0x72	//����ECC��Կ��
#define	CMD_ECCSIGNDATA				0x74	//ECCǩ��
#define	CMD_ECCVERIFY				0x76	//ECC��֤
#define	CMD_ECC_EXPORTSESSIONKEY	0x78	//ECC���ɲ������Ự��Կ
#define	CMD_ECC_EXPORTSESSIONKEYEX	0x80	//ECC�����Ự��Կ
#define	CMD_ECC_ENCRYPT				0x7A	//�ⲿECC����  
#define	CMD_ECC_DECRYPT				0x7C	//�ⲿECC����
#define	CMD_EXT_ECCSIGN				0x7E	//�ⲿECCǩ��
#define CMD_EXT_ECCVERIFY			0x7F
#define	CMD_GEN_AGREEDATA_ECC		0x82	//ECC������ԿЭ�̲���
#define	CMD_GEN_AGREEDATAKEY_ECC	0x84	//ECC����Э�̲���������Ự��Կ
#define	CMD_GEN_KEY_ECC	        	0x86	//ECC����Ự��Կ
#define	CMD_EXPORT_PUBKEY			0x88	//������Կ
#define	CMD_IMPORT_SESSIONKEY		0xA0	//����Ự��Կ
#define	CMD_ENCRYPT_INIT			0xA4	//���ܳ�ʼ��
#define	CMD_ENCRYPT					0xA6	//�������ݼ���
#define	CMD_ENCRYPT_UPDATA			0xA8	//�������ݼ���
#define	CMD_ENCRYPT_FINAL			0xAA	//��������
#define	CMD_DECRYPT_INIT			0xAC	//���ܳ�ʼ��
#define	CMD_DECRYPT					0xAE	//�������ݽ���
#define	CMD_DECRYPT_UPDATA			0xB0	//�������ݽ���
#define	CMD_DECRYPT_FINAL			0xB2	//��������
#define	CMD_DIGEST_INIT				0xB4	//�����Ӵճ�ʼ��
#define	CMD_DIGEST					0xB6	//�������������Ӵ�
#define	CMD_DIGEST_UPDATA			0xB8	//�������������Ӵ�
#define	CMD_DIGEST_FINAL			0xBA	//���������Ӵ�
#define	CMD_MAC_INIT				0xBC	//��Ϣ�����������ʼ��
#define	CMD_MAC						0xBE	//����������Ϣ����������
#define	CMD_MAC_UPDATA				0xC0	//����������Ϣ����������
#define	CMD_MAC_FINAL				0xC2	//������Ϣ����������
#define	CMD_DESTROY_SESSIONKEY	    0xC4	//���ٻỰ��Կ 			
/*
 *  ����������������
 */
#define	CMD_GEN_EXTRSAKEY			0x52	//�����ⲿRSA��Կ
#define	CMD_IMPORT_PLAINRSAKEY		0xD8	//���ĵ���RSA��Կ
#define CMD_ExtRSA_PubOPT			0xEA    //�ⲿRSA��Կ����
#define CMD_ExtRSA_PriOPT			0xE8	//�ⲿRSA˽Կ����		   
#define	CMD_IMPORT_SYMMKEY		    0xA2	//���ĵ���Գ���Կ
//                                                    
#define CMD_CONTYPE					0xE6
#define CMD_CLEARPROG				0x07	//��������

/*
 *  �������
 */
void CMD_SetLable(void);
void CMD_GetCosInfo(void);
void CMD_DevAuth(void);
void CMD_ChangeDevAuthkey(void);
void CMD_GetPinInfo(void);
void CMD_ChangePin(void);
void CMD_VerifyPin(void);
void CMD_UnLockPin(void);
void CMD_ClearSecureState(void);
void CMD_CreateApplication(void);
void CMD_EnumApplication(void);
void CMD_DeleteApplication(void);
void CMD_OpenApplication(void);
void CMD_CloseApplication(void);
void CMD_CreateFile(void);
void CMD_DeleteFile(void);
void CMD_EnumFile(void);
void CMD_GetFileInfo(void);
void CMD_ReadFile(void);
void CMD_WriteFile(void);
void CMD_CreateContainer(void);
void CMD_OpenContainer(void);
void CMD_CloseContainer(void);
void CMD_EnumContainer(void);
void CMD_DeleteContainer(void);
void CMD_GetContainerInfo(void);
void CMD_ImportCert(void);
void CMD_ExportCert(void);
void CMD_GenRandom(void);
//
#ifdef ECCSUPPORT				
void CMD_GenECCKeyPair(void);
void CMD_ImportECCkeyPair(void);
void CMD_ECCSignData(void);
void CMD_ECCVerify(void);
void CMD_ECCExportSessionKey(void);
void CMD_ECCExportSessionKeyEx(void);
void CMD_ExtECCEncrypt(void);
void CMD_GenerateAgreementDataWithECC(void);
void CMD_GenerateAgreementDataAndKeyWithECC(void);
void CMD_GenerateKeyWithECC(void);
void CMD_ExtECCDecrypt(void);
void CMD_ExtECCSign(void);
void CMD_ExtECCVerify(void);	
#endif
//
#ifdef RSASUPPORT
void CMD_GenRSAKeyPair(void);
void CMD_ImportRSAKeyPair(void);
void CMD_RSASignData(void);
void CMD_RSAVerify(void);
void CMD_RSAExportSessionKey(void);
void CMD_RSAExportSessionKeyEx(void); 
void CMD_ExtRSAPubKeyOPT(void);
void CMD_GenExtRSAkey(void);
void CMD_ExtRSAEnDecrypt(void);
void CMD_ImportRSAKey(void); 
void CMD_ExtRSAPubKeyOPT(void);
void CMD_ExtRSAPriKeyOPT(void);
void CMD_ExportRSASessionKey(void);
void CMD_ExportRSAKeyPair(void);
#endif     
//
void CMD_ExportPublicKey(void);
void CMD_ImportSessionKey(void);
void CMD_ImportSymmKey(void);
void CMD_EncryptInit(void);
void CMD_Encrypt(void);
void CMD_EncryptUpdate(void);
void CMD_EncryptFinal(void);
void CMD_DecryptInit(void);
void CMD_Decrypt(void);
void CMD_DecryptUpdate(void);
void CMD_DecryptFinal(void);
void CMD_DigestInit(void);
void CMD_Digest(void);
void CMD_DigestUpdate(void);
void CMD_DigestFinal(void);
void CMD_MACInit(void);
void CMD_Mac(void);
void CMD_MACUpdate(void);
void CMD_MACFinal(void);
void CMD_DestroySessionKey(void);
//                      
void CMD_GetCONType(void);
void CMD_ClearProg(void);

/*
 *  ˽�а�������
 */
void CMD_DeleteContainerByName(INT8 *name); 
UINT16 CMD_GetAppStatus(void); 

/*
 *  ������ں���
 */                     
void DoAS5xxCMD(void);

#endif  //_COMMAND_H_
