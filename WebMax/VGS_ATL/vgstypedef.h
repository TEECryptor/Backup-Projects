#pragma once
//-------------------------------------------------------------------
#define MAX_TEXTURE 8

// ���������׵�ַ
#define GET_PTR(v) ( v.size() != 0 ?&(v[0]) : NULL )
//-------------------------------------------------------------------
struct stDebugInfo
{
	long	face;
	long	a3dface;
	double  fps;
	int     other1; //��������log��Ϣ�Ļ�� 
	int     other2; //��������log��Ϣ�Ļ�� 
};	
//-------------------------------------------------------------------
// Ҫ���ص��ļ�����
//-------------------------------------------------------------------
enum downloadType
{
	TYPE_LOGOFILE = 0,
	TYPE_IMAGEFILE,   // ��ͼ�ļ�
	TYPE_A3DFILE,     // a3d�����ļ�
	TYPE_OTHERFILE
};
//-------------------------------------------------------------------
// Ҫ���ص��ļ�����
//-------------------------------------------------------------------
enum VGS_DOWNLOADFILE_TYPE
{
	VGS_DOWNLOADFILE_TYPE_N3D = 0,
	VGS_DOWNLOADFILE_TYPE_IMAGE,   // ��ͼ�ļ�
	VGS_DOWNLOADFILE_TYPE_A3D,     // a3d�����ļ�
	VGS_DOWNLOADFILE_TYPE_LOGO,
	VGS_DOWNLOADFILE_TYPE_OTHER
};
//-------------------------------------------------------------------
// �ļ���ʹ������
//-------------------------------------------------------------------
enum VGS_FILE_USAGE
{
	VGS_FILE_USAGE_N3D = 0,
	VGS_FILE_USAGE_A3D,
	VGS_FILE_USAGE_FLASH2D,
	VGS_FILE_USAGE_FLASHTEXTURE,
	VGS_FILE_USAGE_IMAGETEXTURE
};
//-------------------------------------------------------------------
/*
//���ڷ��͸���������Ϣʱ�����Ĳ���
//-------------------------------------------------------------------
enum
{
	DOWN_TYPE,					//�������ݵ����ͣ�ǰ̨0�ͺ�̨1����
	HEADER_ISDOWN,				//Header��Ϣ�����Ƿ����
	SCENE_ISDOWN,				//������Ϣ�����Ƿ����
	ONETEX_ISDOWN,				//һ����ͼ��Ϣ�����Ƿ����
	ALLTEX_ISDOWN,				//������ͼ��Ϣ�����Ƿ����
	ONEIMAGE_ISDOWN,			//һ��Image��Ϣ�����Ƿ����
	ALLIMAGE_ISDOWN,			//����Image��Ϣ�����Ƿ����
	ALL_ISDOWN,					//����N3D��Ϣ�����Ƿ����
	IS_DOWN,					//�����������ݣ���ʾ���ؽ���
	TERMINATED					//�����̹߳ر�
};*/
//-------------------------------------------------------------------
// ����״̬
//-------------------------------------------------------------------
/*
enum VGS_DOWNLOAD_STATUS
{	
	VGS_DOWNLOAD_STATUS_NONE = 0,
	VGS_DOWNLOAD_STATUS_START,
	VGS_DOWNLOAD_STATUS_VERSION,            // ���ڻ�ȡ�ļ��汾��
	VGS_DOWNLOAD_STATUS_HEADER,				// ���������ļ�ͷ
	VGS_DOWNLOAD_STATUS_SCENE,				// �������س���
	VGS_DOWNLOAD_STATUS_IMAGE,			    // ��������ͼ��
	VGS_DOWNLOAD_STATUS_ALLISDOWN,			// ����N3D��Ϣ�������
	VGS_DOWNLOAD_STATUS_TERMINATED			// �����̹߳ر�
};*/
//-------------------------------------------------------------------
// N3D���ݵļ�������
//-------------------------------------------------------------------
enum VGS_N3DLOAD_TYPE
{
	VGS_N3DLOAD_TYPE_LOAD = 0,    // ���غ���ص��ڴ�
	VGS_N3DLOAD_TYPE_PRELOAD      // ���غ󲻼��ص��ڴ�
};
//-------------------------------------------------------------------

//////////////////////// vgs�Զ�����Ϣ //////////////////////////////////////////

#define WM_VGSMSG_BEGIN         (WM_USER + 200)


///////////////////// �ļ�������Ϣ��ʼ ////////////////////////////
#define WM_DOWNLOAD_MSG_BEGIN   (WM_VGSMSG_BEGIN)   
// vgs2 2.0���߳�������Ϣ
#define WM_DOWNLOADFILE_PROC	(WM_DOWNLOAD_MSG_BEGIN + 1)		// �ļ����ع����з��͸������ڵ���Ϣ
#define WM_DOWNLOADN3D_PROC		(WM_DOWNLOADFILE_PROC + 1)		// �ļ����ع����з��͸������ڵ���Ϣ
// ���߳�������������������Ϣ
#define WM_DOWNLOAD_MSG_STATUSCHANGE             (WM_DOWNLOADN3D_PROC + 1)					  // status change
#define WM_DOWNLOAD_MSG_DOWNLOADPROGRESS         (WM_DOWNLOAD_MSG_STATUSCHANGE + 1)			  // download progress
#define WM_DOWNLOAD_MSG_FILE_DOWNLOADED          (WM_DOWNLOAD_MSG_DOWNLOADPROGRESS + 1)       // һ���ļ��������
#define WM_DOWNLOAD_MSG_N3DONEIMG_DOWNLOADED     (WM_DOWNLOAD_MSG_FILE_DOWNLOADED + 1)         // n3d�ļ��е�һ��ͼ�����ݶ��������
#define WM_DOWNLOAD_MSG_N3DSCENE_DOWNLOADED      (WM_DOWNLOAD_MSG_N3DONEIMG_DOWNLOADED + 1)    // n3d�ļ��еĳ��������������
#define WM_DOWNLOAD_MSG_END                      (WM_VGSMSG_BEGIN + 50)                        // �ļ�������Ϣ����
///////////////////// �ļ�������Ϣ��ʼ ////////////////////////////


//////////////// js��Ҫ����Ϣ /////////////////////
#define WM_JSFUNC_MSG_BEGIN             (WM_DOWNLOAD_MSG_END + 1)   // js��Ϣ��ʼ
#define WM_JSFUNC_PROC		            (WM_JSFUNC_MSG_BEGIN + 1)	//����������ŵ�ĳһ֡ʱ���͸������ڵ���Ϣ������JS Func�ص�
#define WM_JSFUNC_PROC_CAMMOVETO		(WM_JSFUNC_PROC + 1)	  // �����moveto����ÿ֡�ص�
#define WM_JSFUNC_PROC_MODELKEYFRAME    (WM_JSFUNC_PROC_CAMMOVETO + 1)	  // ģ�Ͷ�������ʱ��ÿ֡�ص�
#define WM_ZTRIGFUNC_PROC               (WM_JSFUNC_PROC_MODELKEYFRAME + 1)
#define WM_JSFUNC_MSG_SCENEISDOWN       (WM_ZTRIGFUNC_PROC + 1)   // scene is down
#define WM_JSFUNC_MSG_ALLISDOWN         (WM_JSFUNC_MSG_SCENEISDOWN + 1)   // all is down
#define WM_JSFUNC_MSG_END               (WM_JSFUNC_MSG_BEGIN + 20)   // js��Ϣend
//////////////// js��Ҫ����Ϣ /////////////////////


//////////////// flash��Ҫ����Ϣ ////////////////////////
#define WM_FLASH_MSG_BEGIN           (WM_JSFUNC_MSG_END + 1)     // flash��Ϣ��ʼ
#define WM_FLEXCALLBACK              (WM_FLASH_MSG_BEGIN + 1)    //���ϵͳ��Ϣʵ��ͨ��C++ֱ�ӵ���flash�ĺ����ķ���
#define WM_FLEXCALLBACK_RETURN       (WM_FLEXCALLBACK + 1)
#define WM_FLASH_MSG_END             (WM_FLASH_MSG_BEGIN + 10)   // flash��Ϣend
//////////////// flash��Ҫ����Ϣ ////////////////////////


#define WM_VGSMSG_END         (WM_USER + 2000)
////////////////// vgs�Զ�����Ϣ /////////////////////////////////////////////////