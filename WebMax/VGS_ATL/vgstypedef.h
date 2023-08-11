#pragma once
//-------------------------------------------------------------------
#define MAX_TEXTURE 8

// 获得数组的首地址
#define GET_PTR(v) ( v.size() != 0 ?&(v[0]) : NULL )
//-------------------------------------------------------------------
struct stDebugInfo
{
	long	face;
	long	a3dface;
	double  fps;
	int     other1; //用于其他log信息的获得 
	int     other2; //用于其他log信息的获得 
};	
//-------------------------------------------------------------------
// 要下载的文件类型
//-------------------------------------------------------------------
enum downloadType
{
	TYPE_LOGOFILE = 0,
	TYPE_IMAGEFILE,   // 贴图文件
	TYPE_A3DFILE,     // a3d动画文件
	TYPE_OTHERFILE
};
//-------------------------------------------------------------------
// 要下载的文件类型
//-------------------------------------------------------------------
enum VGS_DOWNLOADFILE_TYPE
{
	VGS_DOWNLOADFILE_TYPE_N3D = 0,
	VGS_DOWNLOADFILE_TYPE_IMAGE,   // 贴图文件
	VGS_DOWNLOADFILE_TYPE_A3D,     // a3d动画文件
	VGS_DOWNLOADFILE_TYPE_LOGO,
	VGS_DOWNLOADFILE_TYPE_OTHER
};
//-------------------------------------------------------------------
// 文件的使用类型
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
//用于发送给主窗口消息时附带的参数
//-------------------------------------------------------------------
enum
{
	DOWN_TYPE,					//下载数据的类型，前台0和后台1下载
	HEADER_ISDOWN,				//Header信息下载是否完成
	SCENE_ISDOWN,				//场景信息下载是否完成
	ONETEX_ISDOWN,				//一个贴图信息下载是否完成
	ALLTEX_ISDOWN,				//所有贴图信息下载是否完成
	ONEIMAGE_ISDOWN,			//一个Image信息下载是否完成
	ALLIMAGE_ISDOWN,			//所有Image信息下载是否完成
	ALL_ISDOWN,					//所有N3D信息下载是否完成
	IS_DOWN,					//正在下载数据，显示下载进度
	TERMINATED					//下载线程关闭
};*/
//-------------------------------------------------------------------
// 下载状态
//-------------------------------------------------------------------
/*
enum VGS_DOWNLOAD_STATUS
{	
	VGS_DOWNLOAD_STATUS_NONE = 0,
	VGS_DOWNLOAD_STATUS_START,
	VGS_DOWNLOAD_STATUS_VERSION,            // 正在获取文件版本号
	VGS_DOWNLOAD_STATUS_HEADER,				// 正在下载文件头
	VGS_DOWNLOAD_STATUS_SCENE,				// 正在下载场景
	VGS_DOWNLOAD_STATUS_IMAGE,			    // 正在下载图像
	VGS_DOWNLOAD_STATUS_ALLISDOWN,			// 所有N3D信息下载完成
	VGS_DOWNLOAD_STATUS_TERMINATED			// 下载线程关闭
};*/
//-------------------------------------------------------------------
// N3D数据的加载类型
//-------------------------------------------------------------------
enum VGS_N3DLOAD_TYPE
{
	VGS_N3DLOAD_TYPE_LOAD = 0,    // 下载后加载到内存
	VGS_N3DLOAD_TYPE_PRELOAD      // 下载后不加载到内存
};
//-------------------------------------------------------------------

//////////////////////// vgs自定义消息 //////////////////////////////////////////

#define WM_VGSMSG_BEGIN         (WM_USER + 200)


///////////////////// 文件下载消息开始 ////////////////////////////
#define WM_DOWNLOAD_MSG_BEGIN   (WM_VGSMSG_BEGIN)   
// vgs2 2.0单线程下载消息
#define WM_DOWNLOADFILE_PROC	(WM_DOWNLOAD_MSG_BEGIN + 1)		// 文件下载过程中发送给主窗口的消息
#define WM_DOWNLOADN3D_PROC		(WM_DOWNLOADFILE_PROC + 1)		// 文件下载过程中发送给主窗口的消息
// 多线程下载器发出的下载消息
#define WM_DOWNLOAD_MSG_STATUSCHANGE             (WM_DOWNLOADN3D_PROC + 1)					  // status change
#define WM_DOWNLOAD_MSG_DOWNLOADPROGRESS         (WM_DOWNLOAD_MSG_STATUSCHANGE + 1)			  // download progress
#define WM_DOWNLOAD_MSG_FILE_DOWNLOADED          (WM_DOWNLOAD_MSG_DOWNLOADPROGRESS + 1)       // 一个文件下载完成
#define WM_DOWNLOAD_MSG_N3DONEIMG_DOWNLOADED     (WM_DOWNLOAD_MSG_FILE_DOWNLOADED + 1)         // n3d文件中的一个图像数据段下载完成
#define WM_DOWNLOAD_MSG_N3DSCENE_DOWNLOADED      (WM_DOWNLOAD_MSG_N3DONEIMG_DOWNLOADED + 1)    // n3d文件中的场景数据下载完成
#define WM_DOWNLOAD_MSG_END                      (WM_VGSMSG_BEGIN + 50)                        // 文件下载消息结束
///////////////////// 文件下载消息开始 ////////////////////////////


//////////////// js需要的消息 /////////////////////
#define WM_JSFUNC_MSG_BEGIN             (WM_DOWNLOAD_MSG_END + 1)   // js消息开始
#define WM_JSFUNC_PROC		            (WM_JSFUNC_MSG_BEGIN + 1)	//相机动画播放到某一帧时发送给主窗口的消息，用于JS Func回调
#define WM_JSFUNC_PROC_CAMMOVETO		(WM_JSFUNC_PROC + 1)	  // 相机的moveto方法每帧回调
#define WM_JSFUNC_PROC_MODELKEYFRAME    (WM_JSFUNC_PROC_CAMMOVETO + 1)	  // 模型动画播放时的每帧回调
#define WM_ZTRIGFUNC_PROC               (WM_JSFUNC_PROC_MODELKEYFRAME + 1)
#define WM_JSFUNC_MSG_SCENEISDOWN       (WM_ZTRIGFUNC_PROC + 1)   // scene is down
#define WM_JSFUNC_MSG_ALLISDOWN         (WM_JSFUNC_MSG_SCENEISDOWN + 1)   // all is down
#define WM_JSFUNC_MSG_END               (WM_JSFUNC_MSG_BEGIN + 20)   // js消息end
//////////////// js需要的消息 /////////////////////


//////////////// flash需要的消息 ////////////////////////
#define WM_FLASH_MSG_BEGIN           (WM_JSFUNC_MSG_END + 1)     // flash消息开始
#define WM_FLEXCALLBACK              (WM_FLASH_MSG_BEGIN + 1)    //添加系统消息实现通过C++直接调用flash的函数的方法
#define WM_FLEXCALLBACK_RETURN       (WM_FLEXCALLBACK + 1)
#define WM_FLASH_MSG_END             (WM_FLASH_MSG_BEGIN + 10)   // flash消息end
//////////////// flash需要的消息 ////////////////////////


#define WM_VGSMSG_END         (WM_USER + 2000)
////////////////// vgs自定义消息 /////////////////////////////////////////////////