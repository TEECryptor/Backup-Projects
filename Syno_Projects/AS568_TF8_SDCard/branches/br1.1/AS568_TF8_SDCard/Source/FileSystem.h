#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#define FLAG_END  0xfff8//�ļ�������־ 

#define MF_CLUSTER 0//MF���ڵĴء�

#define FILEID   1          //change on 2012/4/5 @ ѡ���ļ�ʱ��������ȷ����ͨ��ID��NAME����Ŀ¼/�ļ� 
//#define FILENAME 0

#define FT_MF    0x01  //MF
#define FT_DF    0x11  //DF
#define FT_EF    0x20  //FT_EF�������漸������
#define FT_BIN   0x21  //�������ļ�
#define FT_CERT  0x22  //֤���ļ� 
#define FT_KEY   0x25  //��Կ�ļ�
#define FT_PUB   0x30  //��Կ�ļ�
#define FT_PRI   0x31  //˽Կ�ļ�
#define FT_SK    0x32  //�Գ���Կ
#define FT_ECC   0x33

//--------Key�������--------------
#define KI_MC        0x00  //������Կ���ڵ�����
#define KI_SOPIN     0x01  //����ԱPIN���ڵ�����
#define KI_USERPIN   0x02  //�û�PIN���ڵ�����
#define KI_IA        0x03  //�ڲ���֤��KEY
#define KI_EA        0x04  //�ⲿ��֤��KEY



#define MIN_FILE_NAME_LEN 0x01
#define MAX_FILE_NAME_LEN 48

struct _FILECONTROL{
    ULONG  ReadControl;//��Ȩ�ޣ�
	ULONG  WriteControl;//дȨ��;
	UINT8  DelControl;//ɾ��Ȩ�ޣ�
	UINT8  UseControl;//ʹ��Ȩ��;
    };
union FILECONTROL
{
	UINT32 lAccessControl;
	struct _FILECONTROL FileControl;
};

#define APP_NORMAL    0//Ӧ������
#define APP_LOCKTEMP  1//Ӧ�ñ���ʱ����
#define APP_LOCKEVER  2//Ӧ�ñ���������

typedef struct _FILEATTRIBUTE
{
	INT8 Name[MAX_FILE_NAME_LEN];//�ļ���Ŀ¼����
	UINT16 FileID;//�ļ�ID����MFΪ0x3f00,Key�ļ�Ϊ0x0000
	UINT16 nStatus;//0x00,������0x01������0x02�������� 
 
    union FILECONTROL fControl; 
		
}FILEATTRIBUTE; 

 
typedef struct _tFileNode
{    
     UINT16 Cluster;        //�ļ��洢�ڴ����е���ʼ�غ�;���Ե��������ļ���ΨһID��
	 INT16  Parent;         //ָ��ǰ�ڵ�ĸ���㣬MF���ڵ�ĸ����ΪNull;
     INT16  FirstChild;     //ָ��ǰ�ڵ�ĵ�һ�����ӽڵ㣬EF�ļ�Ҷ�ڵ��ֵΪ��
     INT16  NextSibling;    //ָ��ǰ�ڵ���һ���ֵܽڵ�;�����ں�������ļ�;     
     UINT8  FileType;       //EF,DF,MF
     UINT8  ContainerType;  //container type: RSA OR ECC
     UINT16 FileSize;       //�ļ���ռ��С�����ļ�����FCPռ�ÿռ�ͼ�¼��͸�����ݿռ�; 	 
     UINT16 Param;          //���������������ļ��Ĳ�����������RSA����Կ���ȡ� 
  	 FILEATTRIBUTE     FCP;
}tFileNode;

#define NULL_NODE -1

extern UINT16 gCurrentDirectoryCluster;//��ǰ����Ŀ¼���ڵĴ�
extern UINT16 gCurrentFileCluster;//��ǰ�ļ����ڵĴ�
extern UINT16 gMFCluster;//MF���ڵĴ�

#define CLUSTER_SIZE 128//ÿһ���СΪ128 �ֽ�
//#define FAT_SIZE     480//FAT���С.
#define FAT_SIZE     448//FAT���С.
//#define FAT_SIZE     400//FAT���С.

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
