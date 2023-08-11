/***************************************************************************
 *	File name:SynoError.h
 *	Introduce:All Syno error code definition
 *	Author:Syno common
 *	Date:2015/11/04
 *  Version:v1.0
 **************************************************************************/

#ifndef _SYNO_ERROR_H_
#define _SYNO_ERROR_H_

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/


/*----------------------------------------------*
 * 错误代码定义                                 *
 *----------------------------------------------*/
#define SAR_COS_OK									0x9000	//成功
#define	SAR_COS_PIN_INVALID							0x63C0	//验证失败,还剩X次机会(X为最后一位值，需动态填充)
#define	SAR_COS_WRITEFILEERR						0x6581	//写文件失败
#define	SAR_COS_INDATALENERR						0x6700	//LC长度错误
#define SAR_COS_GENRSAKEYERR                        0x6915  //生成密钥对失败
#define	SAR_COS_SASTATUSERR							0x6982	//不满足安全状态 
#define	SAR_COS_PIN_LOCKED							0x6983	//认证方法锁定
#define	SAR_COS_INDATAERR							0x6984	//引用数据无效
#define	SAR_COS_INVCONDERR							0x6985	//使用条件不满足
#define	SAR_COS_INVALID_CMD							0x6986	//命令不被允许
#define	SAR_COS_INVALIDSM_OBJECT					0x6988	//SM数据对象不正确
#define	SAR_COS_APPLICATION_NOT_OPEN				0x698A	//没有打开应用
#define	SAR_COS_INVALIDDATA							0x6A80	//数据域不正确(新PIN码加密的补足错误) 
#define	SAR_COS_UNSUPPORTFUNC						0x6A81	//功能不被支持
#define	SAR_COS_NO_ROOM								0x6A84	//设备空间不足   
#define	SAR_COS_INVALIDPARAMERR						0x6A86	//P1P2参数不正确 
#define	SAR_COS_APPLICATION_NOT_EXISTS				0x6A88	//指定的应用不存在
#define	SAR_COS_APPLICATION_EXISTS					0x6A89	//应用已经存在 
#define	SAR_COS_APPLICATION_ALREADY_OPEN			0x6A8A	//应用已打开  	  
#define	SAR_COS_APPLICATIONNAME_NOT_EXISTS			0x6A8B	//引用的应用名未找到 
#define	SAR_COS_SYMM_KEY_NOT_EXISTS					0x6A8C	//对称密钥不存在
#define	SAR_COS_DATA_ERR							0x6A8D	//数据错误
#define	SAR_COS_UNSUPPORT_MULTI_APP					0x6A90	//已打开应用，不支持多个应用同时打开
#define	SAR_COS_CONTAINERNAME_NOT_EXISTS			0x6A91	//指定的容器名不存在
#define	SAR_COS_FILE_ALREADY_EXIST					0x6A92	//文件已存在
#define	SAR_COS_FILE_NOT_EXIST						0x6A93	//文件不存在
#define	SAR_COS_CONTAINERID_NOT_EXISTS				0x6A94	//指定的容器ID不存在 
#define	SAR_COS_KEYNOTFOUNTERR						0x6A95	//容器中没有对应的密钥对文件
#define	SAR_COS_CERTNOTFOUNTERR						0x6A96	//指定类型的证书不存在
#define	SAR_COS_WRITEDATAERR						0x6A97	//数据写入失败
#define	SAR_COS_SIGNVERYFIERR						0x6A98	//验证签名失败/公钥运算失败
#define	SAR_COS_UNSUPPORT_SESSION_KEY				0x6A99	//不支持的会话密钥算法标识
#define	SAR_COS_ASYMM_ENC_FAIL						0x6A9A	//非对称加密失败
#define	SAR_COS_ASYMM_DEC_FAIL						0x6A9B	//非对称解密失败
#define	SAR_COS_PRI_KEY_SIGN_FAIL					0x6A9C	//私钥签名失败
#define	SAR_COS_UNSUPPORT_HASH_ALG					0x6A9D	//不支持摘要算法
#define	SAR_COS_NEED_MORE_DATA						0x6A9E	//还有更多数据上传
#define	SAR_COS_KEY_NOT_EXISTS						0x6A9F	//引用的密钥未找到
#define	SAR_COS_FILEOFFSETERR						0x6B00	//偏移量超出文件长度
#define	SAR_COS_KEY_EXCHANGE_DATA_FAIL				0x6B01	//生成密钥协商数据失败
#define	SAR_COS_GEN_KEY_EXCHANGE_FAIL				0x6B02	//生成协商密钥失败
#define	SAR_COS_OUTDATALENERR						0x6C00	//LE不正确
#define	SAR_COS_CLAERR								0x6E00	//CLA不正确
#define	SAR_COS_READSIZE_OVERFLOW					0x6E01	//待返回的数据长度超出最大缓冲区长度 
#define	SAR_COS_CONTAINER_ALREADY_EXISTS			0x6E02	//指定的容器已经存在
//
#define SAR_OK					      0x00000000 /* 成功 */
#define SAR_FAIL				      0x0B000001 /* 失败 */
#define SAR_NOTSUPPORT_SYS	          0x0B000002 /* 不支持的系统平台 */
#define SAR_FILEERR				      0x0B000003 /* 文件操作错误 */
#define SAR_INVALIDPARAMERR		      0x0B000004 /* 无效的参数 */
#define SAR_NAMELENERR			      0x0B000005 /* 名称长度错误 */
#define SAR_OBJERR                    0x0B000006 /* 对象错误 */
#define SAR_MEMORYERR		          0x0B000007 /* 内存错误 */
#define SAR_INDATALENERR		      0x0B000008 /* 输入数据长度错误 */
#define SAR_INDATAERR			      0x0B000009 /* 输入数据错误 */
#define SAR_BUFFER_TOO_SMALL	      0x0B00000A /* 缓冲区不足 */
#define SAR_PIN_LEN_RANGE             0x0B00000B /* PIN长度错误 */
#define SAR_DEVICEAUTHERR             0x0B00000C /* 设备认证失败 */
#define SAR_INVALIDDEVNAME            0x0B00000D /* 无效的设备名 */
#define SAR_INVALIDDEVPATH            0x0B00000E /* 无效的设备路径 */
#define SAR_USER_NOT_LOGGED_IN        0x0A00002D /* 用户没有登录 */
#define SAR_PIN_INVALID				  0x0A000026 /* PIN无效 */
#define SAR_MODULUSLENERR	          0x0A00000B /* 模的长度错误 */
#define SAR_OPENFILEERR				  0x0B000101 /* 打开文件错误 */
#define SAR_WRITEFILEERR			  0x0B000102 /* 写文件错误 */
#define SAR_READFILEERR				  0x0B000103 /* 读文件错误 */
#define SAR_DELFILEERR				  0x0B000104 /* 删除文件错误 */
#define SAR_RENAMEFILEERR			  0x0B000105 /* 文件重命名错误 */
#define SAR_FILEENCRPYTED			  0x0B000106 /* 文件已加密 */
#define SAR_FILENOENCRPYT			  0x0B000107 /* 文件未加密 */
#define SAR_FILESIZEERR				  0x0B000108 /* 文件大小错误 */
#define SAR_NOFINDKEYERR			  0x0B000109 /* 未找到加密KEY */
#define SAR_FILE_ENCRYPT_CANCEL	      0x0B000110  /*加密过程中取消加密*/
#define SAR_FILE_DENCRYPT_CANCEL	  0x0B000110  /*解密过程中取消加密*/
#define SAR_DEVICEDISCONNECT		  0x0B00010C /* 设备已断开 */
#define SAR_NOTLOGIN                  0x0B00010D /* 用户没有登录 */
#define SAR_CREATEDIRFAIL             0x0B00010E /* 创建文件夹失败 */
#define SAR_COPYFILEFAIL              0x0B00010F /* 拷贝文件失败 */
#define SAR_OPERATION_CANCEL	      0x0B000110 /* 操作取消 */
#define SAR_DEV_STATE_ERR		      0x0B000111 /* 设备状态错误 */
#define SAR_NOFIND_LICENSE            0x0B000112 /* 未找到授权文件 */
#define SAR_VERIFY_ACTIVATION_FAIL    0x0B000113 /* 认证激活失败 */
#define SAR_SOFT_VERSION_ERR          0x0B000114 /* 软件版本认证错误 */
#define SAR_LOSE_EFFICACY_ERR         0x0B000114 /* 软件激活失效 */


#ifdef __cplusplus
extern "C" {
#endif

char *GetErrorString(unsigned int ulError);

#ifdef __cplusplus
}
#endif

#define SAR_ErrToStr(Err)       GetErrorString(Err)

#endif	//_SYNO_ERROR_H_

