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
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "SynoHeaders.h"

/*----------------------------------------------*
 * 基础命令定义                                 *
 *----------------------------------------------*/
#define	CMD_SET_LABEL				        0x02 /* 设置标签 */
#define	CMD_GET_DEVINFO				        0x04 /* 获取设备信息 */
/* 访问控制 */
#define	CMD_GET_AUTH				        0x10 /* 设备认证 */
#define	CMD_CHANGE_DEVAUTHKEY	            0x12 /* 修改设备密钥 */
#define	CMD_GET_PININFO				        0x14 /* 获取PIN信息 */
#define	CMD_CHANGE_PIN				        0x16 /* 修改PIN */
#define	CMD_VERIFY_PIN				        0x18 /* 校验PIN */
#define	CMD_UNBLOCK_PIN				        0x1A /* 解锁PIN */
#define	CMD_CLEAR_SECURESTATE		        0x1C /* 清除安全状态 */
/* 应用管理 */
#define	CMD_CREATE_APPLICATION		        0x20 /* 创建应用 */
#define	CMD_ENUM_APPLICATION		        0x22 /* 枚举应用 */
#define	CMD_DELETE_APPLICATION		        0x24 /* 删除应用 */
#define	CMD_OPEN_APPLICATION		        0x26 /* 打开应用 */
#define	CMD_CLOSE_APPLICATION		        0x28 /* 关闭应用 */
/* 文件管理 */
#define	CMD_CREATE_FILE				        0x30 /* 创建文件 */
#define	CMD_DELETE_FILE				        0x32 /* 删除文件 */
#define	CMD_ENUM_FILE				        0x34 /* 枚举文件 */
#define	CMD_GET_FILEINFO			        0x36 /* 获取文件信息 */
#define	CMD_READ_FILE				        0x38 /* 读文件 */
#define	CMD_WRITE_FILE				        0x3A /* 写文件 */
/* 容器管理 */
#define	CMD_CREATE_CONTAINER		        0x40 /* 创建容器 */
#define	CMD_OPEN_CONTAINER			        0x42 /* 打开容器 */
#define	CMD_CLOSE_CONTAINER		            0x44 /* 关闭容器 */
#define	CMD_ENUM_CONTAINER			        0x46 /* 枚举容器 */
#define	CMD_DELETE_CONTAINER		        0x48 /* 删除容器 */
#define	CMD_GET_CONTAINERINFO				0x4A /* 获取容器信息*/
#define CMD_CONTYPE					        0xE6 /* 获取容器类型 */
#define	CMD_IMPORT_CERTIFICATE		        0x4C /* 导入证书 */
#define	CMD_EXPORT_CERTIFICATE		        0x4E /* 导出证书 */
/* 密码服务 */
#define	CMD_GEN_RANDOM				        0x50 /* 生成随机数 */
/* RSA */
#define	CMD_GEN_EXTRSAKEY			        0x52 /* 生成外部RSA密钥对 */
#define	CMD_GEN_RSAKEYPAIR			        0x54 /* 生成RSA签名密钥对 */
#define	CMD_IMPORT_PLAINRSAKEY				0xD8 /* 导入明文RSA加密密钥对 */
#define CMD_IMPORT_RSAPUBKEY				0xA5 /* 导入一个RSA公钥(供P11使用) */
#define CMD_IMPORT_RSAPRIKEY				0xA7 /* 导入一个RSA私钥(供P11使用) */
#define	CMD_IMPORT_RSAKEYPAIR		        0x56 /* 导入RSA加密密钥对 */
#define	CMD_RSASIGNDATA				        0x58 /* RSA签名 */
#define CMD_RSAVERIFY				        0x5E /* RSA验签 */
#define	CMD_RSA_EXPORTSESSIONKEY	        0x5A /* RSA生成并导出会话密钥 */
#define	CMD_RSA_EXPORTSESSIONKEYEX			0x5C /* RSA导出会话密钥 */
#define CMD_ExtRSA_PubOPT			        0xEA /* RSA外来公钥运算 */
#define CMD_ExtRSA_PriOPT			        0xE8 /* RSA外来私钥运算 */
#define	CMD_RSAEXTKEY_OPT			        0x60 /* RSA额外操作(加密或者解密) */
/* ECC */
#define	CMD_GEN_ECCKEYPAIR			        0x70 /* 生成ECC签名密钥对 */
#define	CMD_IMPORT_ECCKEYPAIR		        0x72 /* 导入ECC加密密钥对 */
#define	CMD_ECCSIGNDATA				        0x74 /* ECC签名 */
#define	CMD_ECCVERIFY				        0x76 /* ECC验签 */
#define	CMD_ECC_EXPORTSESSIONKEY	        0x78 /* ECC生产并导出会话密钥 */
#define	CMD_ECC_EXPORTSESSIONKEYEX	        0x80 /* ECC生产并导出会话密钥 */
#define	CMD_ECC_ENCRYPT				        0x7A /* 外部ECC加密 */
#define	CMD_ECC_DECRYPT				        0x7C /* 外部ECC解密 */
#define	CMD_EXT_ECCSIGN				        0x7E /* 外部ECC签名 */
#define CMD_EXT_ECCVERIFY			        0x7F /* 外部ECC验签 */
#define	CMD_GEN_AGREEDATA_ECC		        0x82 /* ECC生成密钥协商参数 */
#define	CMD_GEN_AGREEDATAKEY_ECC	        0x84 /* ECC生成协商参数并计算会话密钥 */
#define	CMD_GEN_KEY_ECC	        	        0x86 /* ECC计算会话密钥 */
/* 数据加解密 */
#define	CMD_EXPORT_PUBKEY			        0x88 /* 导出公钥 */
#define	CMD_IMPORT_SESSIONKEY		        0xA0 /* 导入会话密钥 */
#define	CMD_IMPORT_SYMMKEY		            0xA2 /* 明文导入会话密钥 */
#define	CMD_GENERATE_SYMMKEY                0xA3 /* 直接生成一个会话密钥 */
#define	CMD_ENCRYPT_INIT			        0xA4 /* 加密初始化 */
#define	CMD_ENCRYPT					        0xA6 /* 单组数据加密 */
#define	CMD_ENCRYPT_UPDATA			        0xA8 /* 多组数据加密 */
#define	CMD_ENCRYPT_FINAL			        0xAA /* 结束加密 */
#define	CMD_DECRYPT_INIT			        0xAC /* 解密初始化 */
#define	CMD_DECRYPT					        0xAE /* 单组数据解密 */
#define	CMD_DECRYPT_UPDATA			        0xB0 /* 多组数据解密 */
#define	CMD_DECRYPT_FINAL			        0xB2 /* 结束解密 */
#define	CMD_DIGEST_INIT				        0xB4 /* 密码杂凑初始化 */
#define	CMD_DIGEST					        0xB6 /* 单组数据密码杂凑 */
#define	CMD_DIGEST_UPDATA			        0xB8 /* 多组数据密码杂凑 */
#define	CMD_DIGEST_FINAL			        0xBA /* 结束密码杂凑 */
#define	CMD_MAC_INIT				        0xBC /* 消息鉴别码运算初始化 */
#define	CMD_MAC						        0xBE /* 单组数据消息鉴别码运算 */
#define	CMD_MAC_UPDATA				        0xC0 /* 多组数据消息鉴别码运算 */
#define	CMD_MAC_FINAL				        0xC2 /* 结束消息鉴别码运算 */
#define	CMD_DESTROY_SESSIONKEY	            0xC4 /* 销毁会话密钥 */


/*----------------------------------------------*
 * 复合数据类型定义                             *
 *----------------------------------------------*/

/* APDU命令结构定义 */
//typedef struct tagAPDUCmd
//{
//	UCHAR CLA;
//	UCHAR INS;
//	UCHAR P1;
//	UCHAR P2;
//	USHORT LC;
//	USHORT LE;
//} APDUCMD_S;

/* APDU命令头结构定义 */
typedef struct tagAPDUHeader
{
	UINT8 CLA;
	UINT8 INS;
	UINT8 P1;
	UINT8 P2;
}APDUHEADER_S;

/* 定义命令类型 */
typedef enum tagCmdType
{
	CMD,
	ENC,
	DEC
} CMDTYPE_E;

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define CMD_HEAD_LEN        sizeof(APDUHEADER_S)	/* APDU命令头长度 */
#define MAX_LC_LEN			(MAX_CMD_BUF - 6)		/* 发送命令的最大长度 */
#define MAX_APDU_LC_LEN		(MAX_LC_LEN - (CMD_HEAD_LEN+3+2))	/* 发送命令的最大长度 */

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

