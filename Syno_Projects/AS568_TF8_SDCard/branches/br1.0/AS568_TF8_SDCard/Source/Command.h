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

//密钥文件ID 
#define EXCERTID    0xFFF0
#define SIGNCERTID  0xFFF1
#define EXKEYID     0xFFF2
#define SIGNKEYID   0xFFF3
#define PINFILEID   0xFFF4
        
//设备初始化信息
#define CARD_LOCKED    0x02         //卡已经锁定，不能使用
#define CARD_INITED    0x01         //卡已经初始化(已有加密密钥对)
#define CARD_NOTINITED 0x00        	//卡没有初始化(无加密密钥对)，还不能使用

//容器类型定义                                               
#define RSA_CON         1
#define ECC_CON         2        
#define	ECC_KEY_BITS	256

// 分组密码算法标识
#define SGD_SM1_ECB  		0x00000101  //SM1 算法 ECB加密模式 
#define SGD_SM1_CBC  		0x00000102  //SM1 算法 CBC加密模式 
#define SGD_SM1_CFB  		0x00000104  //SM1 算法 CFB加密模式 
#define SGD_SM1_OFB  		0x00000108  //SM1 算法 OFB加密模式 
#define SGD_SM1_MAC  		0x00000110  //SM1 算法 MAC运算 
#define SGD_SSF33_ECB  	    0x00000201  //SSF33算法ECB加密模式  
#define SGD_SSF33_CBC  	    0x00000202  //SSF33算法CBC加密模式 
#define SGD_SSF33_CFB  	    0x00000204  //SSF33算法CFB加密模式 
#define SGD_SSF33_OFB  	    0x00000208  //SSF33算法OFB加密模式 
#define SGD_SSF33_MAC  	    0x00000210  //SSF33算法MAC运算 
#define SGD_SMS4_ECB  	    0x00000401  //SMS4算法 ECB加密模式 
#define SGD_SMS4_CBC  	    0x00000402  //SMS4算法 CBC加密模式 
#define SGD_SMS4_CFB  	    0x00000404  //SMS4算法 CFB加密模式 
#define SGD_SMS4_OFB  	    0x00000408  //SMS4算法 OFB加密模式 
#define SGD_SMS4_MAC  	    0x00000410  //SMS4算法 MAC运算
#define	SGD_AES128_CBC	    0x00000501	//AES 128位(16bytes) CBC加密算法
#define	SGD_AES128_ECB	    0x00000502	//AES 128位(16bytes) ECB加密算法
#define	SGD_AES192_CBC	    0x00000504  //AES 192位(24bytes) CBC加密算法 
#define	SGD_AES192_ECB	    0x00000508  //AES 192位(24bytes) ECB加密算法 
#define	SGD_AES256_CBC	    0x00000510	//AES 256位(32bytes) CBC加密算法 
#define	SGD_AES256_ECB	    0x00000511	//AES 256位(32bytes) ECB加密算法
#define SGD_3DES  	        0x00000410  //SMS4算法 MAC运算 
#define SGD_SM1  			0x0000011F  //SM1 算法 ECB加密模式
#define SGD_SMS4  			0x0000041F  //SMS4算法 MAC运算

// 非对称密码算法标识
#define SGD_RSA  			0x00010000  //RSA 算法 
#define SGD_SM2_1  			0x00020100  //椭圆曲线签名算法 
#define SGD_SM2_2  			0x00020200  //椭圆曲线密钥交换协
#define SGD_SM2_3  			0x00020400  //椭圆曲线加密算法 

// 密码杂凑算法标识
#define SGD_SM3  			0x00000001  //SM3 密码杂凑算法 
#define SGD_SHA1  			0x00000002  //SHA1密码杂凑算法 
#define SGD_SHA256  		0x00000004  //SHA256 密码杂凑算法 
          
//对称算法类型
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

//权限类型
#define SECURE_NEVER_ACCOUNT  	0x00000000  //不允许 
#define SECURE_ADM_ACCOUNT  	0x00000001  //管理员权限
#define SECURE_USER_ACCOUNT  	0x00000010  //用户权限 
#define SECURE_ANYONE_ACCOUNT  	0x000000FF  //任何人 

//设备状态
#define DEV_ABSENT_STATE  		0x00000000  // 设备不存在 
#define DEV_PRESENT_STATE  		0x00000001  // 设备存在 
#define DEV_UNKNOW_STATE  		0x00000002  // 设备状态未知

//PIN码类型
#define KT_MC		            0x00	//
#define KT_SOPIN	            0x00	//管理员
#define KT_USERPIN	            0x01	//用户    
        
//权限状态
#define SA_SOLOGIN    	        12//管理员登陆 
#define SA_USERLOGIN  	        6 //用户登陆 
#define SA_EXTERAUTH  	        1 //外部认证
#define SA_NO         	        0 //无权限 

//容器状态
#define CON_CARDLOCK            0x01
#define CON_APPLOCKTEMP         0x02
#define CON_APPLOCKEVER         0x04
#define CON_SA			        0x08
#define CON_ALL                 0x0F 

//常量定义
#define MAX_APDU_LEN                2048
#define DEFAULTSESSIONKEYLEN        32
#define DEFAULTDEVKEYLEN 		    16
#define DEVICEID 				    0xA0A0  //密钥协商时使用
#define MAX_IV_LEN                  32
#define SESSIONKEY_MAX              0x08
#define APP_MAX                     0x08
#define CON_MAX		                0x0A
#define SESSIONKEY_EXTDATA_ADDR	    0x20008800	//Session key 扩展数据地址
#define	SESSIONKEY_EXTDATA_LEN	    240			//Session key 扩展数据数据长度 
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

//安全存储区的前面50K是文件系统  文件系统后面开始为	FLASH_START，存储数据。
#define FLASHPage       1024   //1k空间
#define HalfFLASHPage   512    //512空间
#define FLASH_START 	57*FLASHPage 

#define SYSINFO_OFFSET  FLASH_START       //系统参数表起始地址 
#define SYSINFO_SIZE    0				  //系统参数表长度 

#define HAGREEMENTKEY_OFFSET (SYSINFO_OFFSET+SYSINFO_SIZE)//AgreementKey存放起始地址 
#define HAGREEMENTKEY_SIZE   sizeof(HAGREEMENTKEY)//AgreementKey总的长度 

#define SESSIONKEY_OFFSET (HAGREEMENTKEY_OFFSET+HAGREEMENTKEY_SIZE)//SessionKey存放起始地址 
#define SESSIONKEY_SIZE   SESSIONKEY_MAX*32 //SessionKey总的长度 

 /*
 *  对称密钥用途
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
 *  APDU包头结构
 */
typedef struct tagAPDUHeader
{
	UINT8 CLA;
	UINT8 INS;
	UINT8 P1;
	UINT8 P2;
}APDUHEADER_S;

/*
 *  APDU包体结构
 */
typedef struct tagAPDUBody
{
	UINT32  LC;
	UINT32  LE;
	UINT8   *DATA;
}APDUBODY_S;

/*
 *  APDU响应包结构
 */
typedef struct tagAPDUResp
{
	UINT16 LEN;
    UINT16 SW;
	UINT8  *DATA;
	UINT8 SW1;
}APDURESP_S;

/*
 *  对称密钥数据结构体
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
 *  应用状态结构体
 */
typedef struct _APPSTATE
{
	UINT16 AppID;
	UINT16 ContainerID[CON_MAX];
    UINT8  AppState;
	UINT8  AppRand[16];	
}APPSTATE;

/*
 *  密钥文件属性结构体
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
 *  加密参数结构体
 */
typedef struct _BLOCKCIPERPARAM
{
	BYTE IV[MAX_IV_LEN];
	UINT32 IVLen;
	UINT32 PaddingType;
	ULONG  FeedBitLen;
}BLOCKCIPHERPARAM,*PBLOCKCIPHERPARAM;

/*
 *  应用属性结构体
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
 *  数据文件属性结构体
 */
typedef struct _FILEATT{
   	BYTE    name[48];
   	ULONG 	filesize;
	ULONG   readright;
	ULONG   writeright;
}FILEATT;

/*
 *  COS版本信息
 */
typedef struct Struct_Version{
	BYTE major; 
	BYTE minor; 
}VERSION;

/*
 *  创建应用参数结构体
 */
typedef struct Struct_cosAPPLICATIONINFO{ 
	CHAR 	szApplicatinName[48];      //应用名称，不足 32 字节数据以 0x00 不全 
	CHAR 	szAdminPin[16];            //管理员口令，不足 16字节数据以 0x00 不全 
	ULONG   dwAdminPinRetryCount;      //管理员口令重试次数 
	CHAR 	szUserPin[16];             //用户口令，不足 16 字节数据以 0x00 不全 
	ULONG   dwUserPinRetryCount;       //用户口令重试次数 
	ULONG 	dwCreateFileRights;        //在应用下创建文件的权限 
	BYTE    byContainerNum;            //要求应用支持的容器数量 
	BYTE    byCertNum;                 //要求应用支持的证书数量 
	WORD    wFileNum;                  //要求应用支持的文件数量 
} cosAPPLICATIONINFO; 
               
/*
 *  COS信息结构体
 */
#pragma pack(push) 	//保存对齐状态
#pragma pack(1)		//设定为1字节对齐
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
#pragma pack(pop)//恢复对齐状态

/*
 *包含设备信息和设备密钥信息，卡状态
 */ 
typedef struct struct_DEV{
	DEVINFOPRI dev;
	UINT8      CardStatus;
	KEYFILE    DevKey[2];
}DEVINFO;
 
/*
 *  错误代码
 */
#define SAR_OK                                  0x9000	//成功
#define	SAR_PIN_INVALID							0x63C0	//验证失败,还剩X次机会(X为最后一位值，需动态填充)
#define	SAR_WRITEFILEERR						0x6581	//写文件失败
#define	SAR_INDATALENERR						0x6700	//LC长度错误
#define SAR_GENRSAKEYERR                        0x6915  //生成密钥对失败
#define	SAR_SASTATUSERR							0x6982	//不满足安全状态 
#define	SAR_PIN_LOCKED							0x6983	//认证方法锁定
#define	SAR_INDATAERR							0x6984	//引用数据无效
#define	SAR_INVCONDERR							0x6985	//使用条件不满足
#define	SAR_INVALID_CMD							0x6986	//命令不被允许
#define	SAR_INVALIDSM_OBJECT					0x6988	//SM数据对象不正确
#define	SAR_APPLICATION_NOT_OPEN				0x698A	//没有打开应用
#define	SAR_INVALIDDATA							0x6A80	//数据域不正确(新PIN码加密的补足错误) 
#define	SAR_UNSUPPORTFUNC						0x6A81	//功能不被支持
#define	SAR_NO_ROOM								0x6A84	//设备空间不足   
#define	SAR_INVALIDPARAMERR						0x6A86	//P1P2参数不正确 
#define	SAR_APPLICATION_NOT_EXISTS				0x6A88	//指定的应用不存在
#define	SAR_APPLICATION_EXISTS					0x6A89	//应用已经存在 
#define	SAR_APPLICATION_ALREADY_OPEN			0x6A8A	//应用已打开  	  
#define	SAR_APPLICATIONNAME_NOT_EXISTS			0x6A8B	//引用的应用名未找到 
#define	SAR_SYMM_KEY_NOT_EXISTS					0x6A8C	//对称密钥不存在
#define	SAR_DATA_ERR							0x6A8D	//数据错误
#define	SAR_UNSUPPORT_MULTI_APP					0x6A90	//已打开应用，不支持多个应用同时打开
#define	SAR_CONTAINERNAME_NOT_EXISTS			0x6A91	//指定的容器名不存在
#define	SAR_FILE_ALREADY_EXIST					0x6A92	//文件已存在
#define	SAR_FILE_NOT_EXIST						0x6A93	//文件不存在
#define	SAR_CONTAINERID_NOT_EXISTS				0x6A94	//指定的容器ID不存在 
#define	SAR_KEYNOTFOUNTERR						0x6A95	//容器中没有对应的密钥对文件
#define	SAR_CERTNOTFOUNTERR						0x6A96	//指定类型的证书不存在
#define	SAR_WRITEDATAERR						0x6A97	//数据写入失败
#define	SAR_SIGNVERYFIERR						0x6A98	//验证签名失败/公钥运算失败
#define	SAR_UNSUPPORT_SESSION_KEY				0x6A99	//不支持的会话密钥算法标识
#define	SAR_ASYMM_ENC_FAIL						0x6A9A	//非对称加密失败
#define	SAR_ASYMM_DEC_FAIL						0x6A9B	//非对称解密失败
#define	SAR_PRI_KEY_SIGN_FAIL					0x6A9C	//私钥签名失败
#define	SAR_UNSUPPORT_HASH_ALG					0x6A9D	//不支持摘要算法
#define	SAR_NEED_MORE_DATA						0x6A9E	//还有更多数据上传
#define	SAR_KEY_NOT_EXISTS						0x6A9F	//引用的密钥未找到
#define	SAR_FILEOFFSETERR						0x6B00	//偏移量超出文件长度
#define	SAR_KEY_EXCHANGE_DATA_FAIL				0x6B01	//生成密钥协商数据失败
#define	SAR_GEN_KEY_EXCHANGE_FAIL				0x6B02	//生成协商密钥失败
#define	SAR_OUTDATALENERR						0x6C00	//LE不正确
#define	SAR_CLAERR								0x6E00	//CLA不正确
#define	SAR_READSIZE_OVERFLOW					0x6E01	//待返回的数据长度超出最大缓冲区长度 
#define	SAR_CONTAINER_ALREADY_EXISTS			0x6E02	//指定的容器已经存在

 
/*
 *  国密标准命令定义
 */
#define	CMD_SET_LABEL				0x02	//设置标签
#define	CMD_GET_COSINFO				0x04	//获取设备信息
#define	CMD_GET_AUTH				0x10	//设备认证
#define	CMD_CHANGE_DEVAUTHKEY	    0x12	//修改设备密钥
#define	CMD_GET_PININFO				0x14	//获取PIN信息
#define	CMD_CHANGE_PIN				0x16	//修改PIN
#define	CMD_VERIFY_PIN				0x18	//校验PIN
#define	CMD_UNBLOCK_PIN				0x1A	//解锁PIN
#define	CMD_CLEAR_SECURESTATE		0x1C	//清除安全状态
#define	CMD_CREATE_APPLICATION		0x20	//创建应用
#define	CMD_ENUM_APPLICATION		0x22	//枚举应用
#define	CMD_DELETE_APPLICATION		0x24	//删除应用
#define	CMD_OPEN_APPLICATION		0x26	//打开应用
#define	CMD_CLOSE_APPLICATION		0x28	//关闭应用
#define	CMD_CREATE_FILE				0x30	//创建文件
#define	CMD_DELETE_FILE				0x32	//删除文件
#define	CMD_ENUM_FILE				0x34	//枚举文件
#define	CMD_GET_FILEINFO			0x36	//获取文件信息
#define	CMD_READ_FILE				0x38	//读文件
#define	CMD_WRITE_FILE				0x3A	//写文件
#define	CMD_CREATE_CONTAINER		0x40	//创建容器
#define	CMD_OPEN_CONTAINER			0x42	//打开容器
#define	CMD_CLOSE_CONTAINER		    0x44	//关闭容器
#define	CMD_ENUM_CONTAINER			0x46	//枚举容器
#define	CMD_DELETE_CONTAINER		0x48	//删除容器
#define	CMD_GET_CONTAINERINFO		0x4A	//获取容器信息
#define	CMD_IMPORT_CERTIFICATE		0x4C	//导入证书
#define	CMD_EXPORT_CERTIFICATE		0x4E	//导出证书
#define	CMD_GEN_RANDOM				0x50	//生成随机数
#define	CMD_GEN_RSAKEYPAIR			0x54	//生成RSA密钥对
#define	CMD_IMPORT_RSAKEYPAIR		0x56	//导入RSA密钥对
#define	CMD_RSASIGNDATA				0x58	//RSA签名
#define	CMD_RSAVERIFY				0x5E	//RSA验签
#define	CMD_RSA_EXPORTSESSIONKEY	0x5A	//RSA生成并导出会话密钥	
#define	CMD_RSA_EXPORTSESSIONKEYEX	0x5C	//RSA导出会话密钥
#define	CMD_RSAEXTKEY_OPT			0x60	//RSA外来公私钥操作	
#define	CMD_GEN_ECCKEYPAIR			0x70	//生成ECC密钥对
#define	CMD_IMPORT_ECCKEYPAIR		0x72	//导入ECC密钥对
#define	CMD_ECCSIGNDATA				0x74	//ECC签名
#define	CMD_ECCVERIFY				0x76	//ECC验证
#define	CMD_ECC_EXPORTSESSIONKEY	0x78	//ECC生成并导出会话密钥
#define	CMD_ECC_EXPORTSESSIONKEYEX	0x80	//ECC导出会话密钥
#define	CMD_ECC_ENCRYPT				0x7A	//外部ECC加密  
#define	CMD_ECC_DECRYPT				0x7C	//外部ECC解密
#define	CMD_EXT_ECCSIGN				0x7E	//外部ECC签名
#define CMD_EXT_ECCVERIFY			0x7F
#define	CMD_GEN_AGREEDATA_ECC		0x82	//ECC生成密钥协商参数
#define	CMD_GEN_AGREEDATAKEY_ECC	0x84	//ECC生成协商参数并计算会话密钥
#define	CMD_GEN_KEY_ECC	        	0x86	//ECC计算会话密钥
#define	CMD_EXPORT_PUBKEY			0x88	//导出公钥
#define	CMD_IMPORT_SESSIONKEY		0xA0	//导入会话密钥
#define	CMD_ENCRYPT_INIT			0xA4	//加密初始化
#define	CMD_ENCRYPT					0xA6	//单组数据加密
#define	CMD_ENCRYPT_UPDATA			0xA8	//多组数据加密
#define	CMD_ENCRYPT_FINAL			0xAA	//结束加密
#define	CMD_DECRYPT_INIT			0xAC	//解密初始化
#define	CMD_DECRYPT					0xAE	//单组数据解密
#define	CMD_DECRYPT_UPDATA			0xB0	//多组数据解密
#define	CMD_DECRYPT_FINAL			0xB2	//结束解密
#define	CMD_DIGEST_INIT				0xB4	//密码杂凑初始化
#define	CMD_DIGEST					0xB6	//单组数据密码杂凑
#define	CMD_DIGEST_UPDATA			0xB8	//多组数据密码杂凑
#define	CMD_DIGEST_FINAL			0xBA	//结束密码杂凑
#define	CMD_MAC_INIT				0xBC	//消息鉴别码运算初始化
#define	CMD_MAC						0xBE	//单组数据消息鉴别码运算
#define	CMD_MAC_UPDATA				0xC0	//多组数据消息鉴别码运算
#define	CMD_MAC_FINAL				0xC2	//结束消息鉴别码运算
#define	CMD_DESTROY_SESSIONKEY	    0xC4	//销毁会话密钥 			
/*
 *  扩充密码服务命令定义
 */
#define	CMD_GEN_EXTRSAKEY			0x52	//生成外部RSA密钥
#define	CMD_IMPORT_PLAINRSAKEY		0xD8	//明文导入RSA密钥
#define CMD_ExtRSA_PubOPT			0xEA    //外部RSA公钥运算
#define CMD_ExtRSA_PriOPT			0xE8	//外部RSA私钥运算		   
#define	CMD_IMPORT_SYMMKEY		    0xA2	//明文导入对称密钥
//                                                    
#define CMD_CONTYPE					0xE6
#define CMD_CLEARPROG				0x07	//擦除程序

/*
 *  命令处理函数
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
 *  私有帮助函数
 */
void CMD_DeleteContainerByName(INT8 *name); 
UINT16 CMD_GetAppStatus(void); 

/*
 *  命令入口函数
 */                     
void DoAS5xxCMD(void);

#endif  //_COMMAND_H_
