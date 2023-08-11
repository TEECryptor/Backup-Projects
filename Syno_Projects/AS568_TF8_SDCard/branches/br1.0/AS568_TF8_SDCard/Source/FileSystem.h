#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#define FLAG_END  0xfff8//文件结束标志 

#define MF_CLUSTER 0//MF所在的簇。

#define FILEID   1          //change on 2012/4/5 @ 选择文件时根据类型确定是通过ID或NAME查找目录/文件 
//#define FILENAME 0

#define FT_MF    0x01  //MF
#define FT_DF    0x11  //DF
#define FT_EF    0x20  //FT_EF包含下面几种类型
#define FT_BIN   0x21  //二进制文件
#define FT_CERT  0x22  //证书文件 
#define FT_KEY   0x25  //密钥文件
#define FT_PUB   0x30  //公钥文件
#define FT_PRI   0x31  //私钥文件
#define FT_SK    0x32  //对称密钥
#define FT_ECC   0x33

//--------Key存放索引--------------
#define KI_MC        0x00  //主控密钥所在的索引
#define KI_SOPIN     0x01  //管理员PIN所在的索引
#define KI_USERPIN   0x02  //用户PIN所在的索引
#define KI_IA        0x03  //内部验证的KEY
#define KI_EA        0x04  //外部验证的KEY



#define MIN_FILE_NAME_LEN 0x01
#define MAX_FILE_NAME_LEN 48

struct _FILECONTROL{
    ULONG  ReadControl;//读权限；
	ULONG  WriteControl;//写权限;
	UINT8  DelControl;//删除权限；
	UINT8  UseControl;//使用权限;
    };
union FILECONTROL
{
	UINT32 lAccessControl;
	struct _FILECONTROL FileControl;
};

#define APP_NORMAL    0//应用正常
#define APP_LOCKTEMP  1//应用被临时锁定
#define APP_LOCKEVER  2//应用被永久锁定

typedef struct _FILEATTRIBUTE
{
	INT8 Name[MAX_FILE_NAME_LEN];//文件或目录名称
	UINT16 FileID;//文件ID，如MF为0x3f00,Key文件为0x0000
	UINT16 nStatus;//0x00,正常，0x01锁定，0x02永久锁定 
 
    union FILECONTROL fControl; 
		
}FILEATTRIBUTE; 

 
typedef struct _tFileNode
{    
     UINT16 Cluster;        //文件存储在簇链中的起始簇号;可以当作区分文件的唯一ID。
	 INT16  Parent;         //指向当前节点的父结点，MF根节点的父结点为Null;
     INT16  FirstChild;     //指向当前节点的第一个孩子节点，EF文件叶节点该值为空
     INT16  NextSibling;    //指向当前节点下一个兄弟节点;方便于横向检索文件;     
     UINT8  FileType;       //EF,DF,MF
     UINT8  ContainerType;  //container type: RSA OR ECC
     UINT16 FileSize;       //文件体空间大小，含文件参数FCP占用空间和纪录或透明数据空间; 	 
     UINT16 Param;          //保留，当作保存文件的参数。比如存放RSA的密钥长度。 
  	 FILEATTRIBUTE     FCP;
}tFileNode;

#define NULL_NODE -1

extern UINT16 gCurrentDirectoryCluster;//当前操作目录所在的簇
extern UINT16 gCurrentFileCluster;//当前文件所在的簇
extern UINT16 gMFCluster;//MF所在的簇

#define CLUSTER_SIZE 128//每一族大小为128 字节
//#define FAT_SIZE     480//FAT表大小.
#define FAT_SIZE     448//FAT表大小.
//#define FAT_SIZE     400//FAT表大小.

#if(FAT_SIZE >= 400)
#define FAT_SIZE_HALF     FAT_SIZE/2  
#else
#define FAT_SIZE_HALF     FAT_SIZE    
#endif  
 
void   FS_Init(void);
void   FS_Format(void);
UINT32 FS_SelectFileByCluster(UINT16 CurrentDirrectoryCluster,UINT16 SelCluster);
UINT32 FS_UpdateFile(UINT8*pData,UINT32 lOffset,UINT16 iLength);
UINT32 FS_ReadFile(UINT8*pData,UINT32 lOffset,UINT16 iLength);
UINT32 FS_DeleteFile(void);
UINT32 FS_CreateFile(tFileNode* newFile);
UINT32 FS_GetFreeSpace(UINT16* FAT);
UINT32 FS_SelectDirOrFileByName(UINT8 nType,INT8* cName);
UINT32 FS_SelectDirOrFileByName2(UINT8 nType,INT8* cName); //miaox+
UINT32 FS_SelectFileByID(UINT16 nFileID);
 
UINT8 FS_ReadDirNodeInfo(UINT8 *NodeInfo);
UINT8 FS_ReadFileNodeInfo(UINT8 *NodeInfo);
UINT8 FS_WriteDirNodeInfo(UINT8 *NodeInfo);
UINT8 FS_WriteFileNodeInfo(UINT8 *NodeInfo);
UINT32 FS_CreateDir (tFileNode* newDir);
void FS_EnumDir(UINT8* pBuf,UINT16* pnLen);
void FS_EnumFile(UINT8 FileType,UINT8* pBuf,UINT16* pnLen);
#endif
