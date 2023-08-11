/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // GetV3D.cpp
* 内容摘要： // 读取V3D文件实现
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // 傅新波
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
* ---------------------------------------------------------------
* 
*
*********************************************************************/


#include "GetV3DImpV2.h"
#include "..\CSysFunc.h"
#include "..\VGSErrCode.h"
#include "..\VGSSupport.h"
#include "..\VGSDef.h"
#include "..\Root.h"

#pragma pack(push, 4) // 数据结构以4字节对齐
///定义Editor的数据结构
typedef struct EditorInfo_Old
{
	CHAR		Name[MAX_NAMESTR_LENGTH];							//Editor的名字
	// INT			cameraIndex;					//使用的相机编号
	CHAR        Camera[MAX_NAMESTR_LENGTH];						    //使用的相机名称

	BOOL		bIsAutoRoundRoll;					//Edit模式下是否自动翻滚

	FLOAT		autoSpeedX;						    // Edit模式下自动绕 X 轴旋转的速度，单位：周/分钟
	FLOAT		autoSpeedY;						    // Edit模式下自动绕 Y 轴旋转的速度，单位：周/分钟

	FLOAT		manualSpeedX;					    //Edit模式下手动翻滚相机的速度绕 X 轴
	FLOAT		manualSpeedY;					    //Edit模式下手动旋转相机的速度绕 Y 轴

	FLOAT		manualPanXSpeed;					//平移的X方向速度 
	FLOAT		manualPanYSpeed;					//平移的Y方向速度 

	FLOAT		dollySpeed;							//滚鼠标滚轮时移动相机的速度 [1, 100]
	FLOAT		dollyAcc;							//相机推拉加速度

	FLOAT		dollyNearDis;						//最近的Dolly
	FLOAT		dollyFarDis;						//最远的Dolly

	INT			rosAxis;							// 旋转轴标志位, 0: xy, 1 : x, 2 : y
	VECTOR3     LookAtPos;                          // Editor的目标点 

	DWORD        LookAtModelIndex;                   // fuxb 2008-12-15 如果正对某个模型，返回模型的索引。如果没有模型，该值为0xffffffff
}EditorInfo_Old;
#pragma pack(pop)

// texture的类型
enum V3D_TextureType_Old
{
	VTT_INVALID = 0,  // 无效贴图
	VTT_BASAL,		  // 基本贴图
	VTT_CUBIC,
	VTT_DYNAMIC,
	VTT_MOVIE,
	VTT_FLASH,
	VTT_BUMP
};

CGetV3DImpV2::CGetV3DImpV2(/*LPDIRECT3DDEVICE9 pd3dDevice*/)
{
	//m_pD3dDevice = pd3dDevice;

	m_pVGSSceneMgr = NULL;
	m_pFile = NULL;

	m_sV3DPath = "";
	m_sDefaultTexturePath = "";

	m_V3DFileVersion = CURRENT_V3DVERSION;

	m_pMaterailBuf = NULL;
	m_pMeshBuf = NULL;
	m_pEntityBuf = NULL;
	m_pCameraBuf = NULL;
	m_pLightBuf = NULL;
	m_pOverlayBuf = NULL;
	m_pButtonBuf = NULL;
	m_pNaviBuf = NULL;
	m_pTextBuf = NULL;
	m_pPrivacyBuf = NULL;
	m_pFlashBuf = NULL;
	m_pAudioBuf = NULL;
	m_pControlBuf = NULL;
	m_pAnimationBuf = NULL;
	m_pSceneGraphBuf = NULL;
	m_pTextureBuf = NULL;
	m_pBackdropBuf = NULL;

}

CGetV3DImpV2::~CGetV3DImpV2()
{
	m_pVGSSceneMgr = NULL;
	m_pFile = NULL;
	m_sV3DPath = "";

	DELETE_ARRAYPTR(m_pMeshBuf);
	DELETE_ARRAYPTR(m_pEntityBuf);
	DELETE_ARRAYPTR(m_pCameraBuf);
	DELETE_ARRAYPTR(m_pLightBuf);
	DELETE_ARRAYPTR(m_pOverlayBuf);
	DELETE_ARRAYPTR(m_pButtonBuf);
	DELETE_ARRAYPTR(m_pNaviBuf);
	DELETE_ARRAYPTR(m_pTextBuf);
	DELETE_ARRAYPTR(m_pPrivacyBuf);
	DELETE_ARRAYPTR(m_pFlashBuf);
	DELETE_ARRAYPTR(m_pAudioBuf);
	DELETE_ARRAYPTR(m_pControlBuf);
	DELETE_ARRAYPTR(m_pAnimationBuf);
	DELETE_ARRAYPTR(m_pSceneGraphBuf);
	DELETE_ARRAYPTR(m_pTextureBuf);
	DELETE_ARRAYPTR(m_pBackdropBuf);
}

void CGetV3DImpV2::Release()
{
	DELETE_ARRAYPTR(m_pMeshBuf);
	DELETE_ARRAYPTR(m_pEntityBuf);
	DELETE_ARRAYPTR(m_pCameraBuf);
	DELETE_ARRAYPTR(m_pLightBuf);
	DELETE_ARRAYPTR(m_pOverlayBuf);
	DELETE_ARRAYPTR(m_pButtonBuf);
	DELETE_ARRAYPTR(m_pNaviBuf);
	DELETE_ARRAYPTR(m_pTextBuf);
	DELETE_ARRAYPTR(m_pPrivacyBuf);
	DELETE_ARRAYPTR(m_pFlashBuf);
	DELETE_ARRAYPTR(m_pAudioBuf);
	DELETE_ARRAYPTR(m_pControlBuf);
	DELETE_ARRAYPTR(m_pBackdropBuf);
}

// 根据v3d文件名，得到当前的V3D目录
void CGetV3DImpV2::SetCurrentDirctory(const std::string &sFileName)
{
	int maxlen = sFileName.length();
	m_sV3DPath = _GetPureFilePathStr(sFileName);
	m_sDefaultTexturePath = m_sV3DPath;
	m_sDefaultTexturePath.append(std::string("resource\\texture\\"));
}

// 获取V3D文件的Table目录
bool CGetV3DImpV2::GetV3DTable(const std::string &strFileName, V3D_Table &Table)
{
	// 创建文件
	fopen_s(&m_pFile, strFileName.c_str(), "rb");
	if (NULL == m_pFile)
	{
		return false;
	}
	
	// 读文件头
	if (ERR_COMMON_OK != ReadHeader())
	{
		fclose(m_pFile);
		return false;
	}

	// 读table
	if (ERR_COMMON_OK != ReadFileTable())
	{
		fclose(m_pFile);
		return false;
	}

	fclose(m_pFile);

	Table = m_V3DTable;

	return true; 
}

// 是否是我支持的版本
BOOL CGetV3DImpV2::isMyVersion(const std::string &sFileName) 
{
	BOOL re = FALSE;
	FILE *pFile = NULL;
	fopen_s(&pFile, sFileName.c_str(), "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_SET); // 定位到文件开头

		V3D_Header V3DHeader;
		fread(&V3DHeader, sizeof(V3DHeader), 1, pFile);
		
		// 有公司标识，而且文件版本号小于等于当前引擎支持的版本号，就认为是支持的
		if (0 == strcmp(string(V3DHeader.corp).c_str(), SUNTOWARD_NAME) && V3DHeader.v3dVersion <= CURRENT_V3DVERSION)
		{
			re = TRUE;
		}
		fclose(pFile);
	}
	return re;
}

DWORD CGetV3DImpV2::OpenV3DFile(CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd)
{
	try
	{
		m_hWnd = hWnd;

		if (pVGSSceneMgr == NULL)
		{
			// ::MessageBox(NULL, LPCSTR("找不到场景"), LPCSTR("错误"), MB_OK | MB_ICONERROR);
			return ERR_SCENE_NOSCENE; // 无法获取场景！
		}

		m_pVGSSceneMgr = (CScene*)pVGSSceneMgr;

		SetRoot(m_pVGSSceneMgr->GetRoot());

		m_pVGSSceneMgr->Reset();  // 清空场景内容

		// 创建文件
		fopen_s(&m_pFile, strFileName.c_str(), "rb");
		if (NULL == m_pFile)
		{
			// ::MessageBox(NULL, LPCSTR("访问文件失败"), LPCSTR("错误"), MB_OK | MB_ICONERROR);
			return ERR_FILE_OPENFAIL;
		}
		
		SetCurrentDirctory(strFileName);  // 得到当前的V3D目录，便于加载贴图等资源

		// m_vCameraDetail.clear();
		m_vpCamera.clear();
		m_vpLight.clear();

		// 进度条
		m_ProcessTotalSize = 33;
		m_ProcessCurSize   = 0;

		// Read the header data
		DWORD result = ReadHeader();
		RETURN(result);

		result = ReadFileTable();
		RETURN(result);

		//Read the window prop
		result = ReadWindowProp();
		RETURN(result);


		// 读取所有数据
		ReadAllData();

		// 关闭文件
		fclose(m_pFile);


		/////////////// 加载内容 /////////////////////

		//Read the  camera data
		result = ReadCameraData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read the light data
		result = ReadLightData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read the texture data
		result = ReadTextureData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read the Material data
		result = ReadMaterialData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		// Read the Model data
		result = ReadMeshData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		// Read the Model data
		result = ReadModelData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read the Text data
		result = ReadTextData();
		RETURN(result);

		//Read the overlay data
		result = ReadOverlayData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read the navigater data
		result = ReadNavigaterData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read the button data
		result = ReadButtonData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read the backdrop
		result = ReadBackdropData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read AudioData
		result = ReadAudioData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read FlashData
		result = ReadFlashData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		// 动画
		result = ReadAnimationData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		//Read privacy data
		result = ReadPrivacyData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		// 控制
		result = ReadControlData(); 
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		result = ReadSceneGraph();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		PostOpen();  // 处理打开完成后的一些工作
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

		if (m_ProcessCurSize <= m_ProcessTotalSize)
		{
			PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessTotalSize, m_ProcessTotalSize);  // 开始进度
		}

		Release(); // 释放buf内存

		return ERR_COMMON_OK;
	}
	catch (...)
	{
		//fclose(m_pFile);
		return ERR_FILE_OPENFAIL;
	}
}

DWORD CGetV3DImpV2::ReadHeader()
{
	if (!m_pFile)
	{
		// throw Exception::exception("读文件头发生错误(01)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}

	fseek(m_pFile, 0, SEEK_SET); // 定位到文件开头
	V3D_Header V3DHeader;
	fread(&V3DHeader, sizeof(V3DHeader), 1, m_pFile);
	if (0 != strcmp(string(V3DHeader.corp).c_str(), SUNTOWARD_NAME))
	{
		// throw Exception::exception("文件格式不正确，无法读取！");
		return ERR_FILE_V3D_WRONGFORMAT;
	}
	
	m_V3DFileVersion = V3DHeader.v3dVersion;
	if (m_V3DFileVersion > CURRENT_V3DVERSION) // 文件版本号大于当前引擎支持的版本号
	{
		// throw Exception::exception("V3D文件格式太新，无法读取！");
		return ERR_FILE_V3D_NEWVERSION;
	}
	/*else if (m_V3DFileVersion < CURRENT_V3DVERSION)
	{
		return ERR_FILE_V3D_OLDVERSION;
	}*/

	return ERR_COMMON_OK;  // 正常读取
	
}


DWORD CGetV3DImpV2::ReadFileTable()
{
	if (!m_pFile)
	{
		// throw Exception::exception("读文件表发生错误(02)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}

	fseek(m_pFile, sizeof(V3D_Header), SEEK_SET);
	if (1 != fread(&m_V3DTable, sizeof(m_V3DTable), 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL;
	}
	return ERR_COMMON_OK;
}

DWORD CGetV3DImpV2::ReadWindowProp()
{
	if (!m_pFile)
	{
		// throw Exception::exception("读文件发生错误(03)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}	

	// 读取窗体属性	
	fseek(m_pFile, m_V3DTable.SceneproperyPtr, SEEK_SET);
	if (1 != fread(&m_SceneProperty, sizeof(m_SceneProperty), 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

	return ERR_COMMON_OK;
}

// 读取所有数据
DWORD CGetV3DImpV2::ReadAllData()
{	
	
	fseek(m_pFile, m_V3DTable.PrivacyDataPtr, SEEK_SET); // 移动到开始位置

	// 授权信息
	UINT PrivacyDataSize = m_V3DTable.cameraDataPtr - m_V3DTable.PrivacyDataPtr;
	m_pPrivacyBuf = new char[PrivacyDataSize];
	// fseek(m_pFile, m_V3DTable.PrivacyDataPtr, SEEK_SET);	
	if (1 != fread(m_pPrivacyBuf, PrivacyDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 相机数据
	UINT CameDataSize = m_V3DTable.lightDataPtr - m_V3DTable.cameraDataPtr;
	m_pCameraBuf = new char[CameDataSize];
	// fseek(m_pFile, m_V3DTable.cameraDataPtr, SEEK_SET);	
	if (1 != fread(m_pCameraBuf, CameDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 灯光
	UINT LightDataSize = m_V3DTable.texturePtr - m_V3DTable.lightDataPtr;
	m_pLightBuf = new char[LightDataSize];
	// fseek(m_pFile, m_V3DTable.lightDataPtr, SEEK_SET);
	if (1 != fread(m_pLightBuf, LightDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 贴图纹理数据
	UINT TextureDataSize = m_V3DTable.mtrlDataPtr - m_V3DTable.texturePtr;
	m_pTextureBuf = new char[TextureDataSize];
	// fseek(m_pFile, m_V3DTable.texturePtr, SEEK_SET);
	if (1 != fread(m_pTextureBuf, TextureDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 材质数据
	UINT MtrDataSize = m_V3DTable.entityDataPtr - m_V3DTable.mtrlDataPtr;	
	m_pMaterailBuf = new char[MtrDataSize];
	// fseek(m_pFile, m_V3DTable.mtrlDataPtr, SEEK_SET);
	if (1 != fread(m_pMaterailBuf, MtrDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// entity
	UINT EntityDataSize = m_V3DTable.meshDataPtr - m_V3DTable.entityDataPtr;
	m_pEntityBuf = new char[EntityDataSize];
	// fseek(m_pFile, m_V3DTable.meshDataPtr, SEEK_SET);
	if (1 != fread(m_pEntityBuf, EntityDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// mesh
 	UINT MeshDataSize = m_V3DTable.TextDataPtr - m_V3DTable.meshDataPtr;
 	m_pMeshBuf = new char[MeshDataSize];
 	// fseek(m_pFile, m_V3DTable.meshDataPtr, SEEK_SET);
 	if (1 != fread(m_pMeshBuf, MeshDataSize, 1, m_pFile))
 	{
 		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
 	}

	// 文字
	UINT TextDataSize = m_V3DTable.OverlayPtr - m_V3DTable.TextDataPtr;
	m_pTextBuf = new char[TextDataSize];
	// fseek(m_pFile, m_V3DTable.TextDataPtr, SEEK_SET);
	if (1 != fread(m_pTextBuf, TextDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// overlay
	UINT OverlayDataSize = m_V3DTable.NavigatotPtr - m_V3DTable.OverlayPtr;
	m_pOverlayBuf = new char[OverlayDataSize];
	// fseek(m_pFile, m_V3DTable.OverlayPtr, SEEK_SET);
	if (1 != fread(m_pOverlayBuf, OverlayDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 导航图
	UINT NaviSize = m_V3DTable.ButtonPtr - m_V3DTable.NavigatotPtr;
	m_pNaviBuf = new char[NaviSize];
	int ReadSize = fread(m_pNaviBuf, NaviSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 按钮
	UINT ButtonSize = m_V3DTable.BackdropPtr - m_V3DTable.ButtonPtr;
	m_pButtonBuf = new char[ButtonSize];
	ReadSize = fread(m_pButtonBuf, ButtonSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 背景图
	UINT BackdropSize = m_V3DTable.AudioPtr - m_V3DTable.BackdropPtr;
	m_pBackdropBuf = new char[BackdropSize];
	ReadSize = fread(m_pBackdropBuf, BackdropSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 声音
	UINT AudioSize = m_V3DTable.FlashPtr - m_V3DTable.AudioPtr;
	m_pAudioBuf = new char[AudioSize];
	ReadSize = fread(m_pAudioBuf, AudioSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// flash
	UINT FlashDataSize = m_V3DTable.AnimationPtr - m_V3DTable.FlashPtr;
	m_pFlashBuf = new char[FlashDataSize];
	ReadSize = fread(m_pFlashBuf, FlashDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 动画
	UINT AnimationDataSize = m_V3DTable.ControlPtr - m_V3DTable.AnimationPtr;
	m_pAnimationBuf = new char[AnimationDataSize];
	ReadSize = fread(m_pAnimationBuf, AnimationDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 控制信息
	UINT ControlDataSize = m_V3DTable.sceneGraphDataPtr - m_V3DTable.ControlPtr;
	m_pControlBuf = new char[ControlDataSize];
	fseek(m_pFile, m_V3DTable.ControlPtr, SEEK_SET);
	ReadSize = fread(m_pControlBuf, ControlDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	// 场景图
	fseek(m_pFile, 0, SEEK_END); // 文件结尾
	UINT FileSize = ftell(m_pFile);
	UINT DataSize = FileSize - m_V3DTable.sceneGraphDataPtr;
	m_pSceneGraphBuf = new char[DataSize];
	fseek(m_pFile, m_V3DTable.sceneGraphDataPtr, SEEK_SET);
	ReadSize = fread(m_pSceneGraphBuf, DataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // 开始进度

	return ERR_COMMON_OK;
}


DWORD CGetV3DImpV2::ReadTextureData()
{
	DELETE_ARRAYPTR(m_pTextureBuf);  // 删除字节流
	return ERR_COMMON_OK;
}

DWORD CGetV3DImpV2::ReadMaterialData()
{
	if (!m_pFile)
	{
		// throw Exception::exception("读文件发生错误(03)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pMaterailBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	m_vMaterial.clear();

	// 读取材质信息
	char* Offset = m_pMaterailBuf;          // 字节流中数据开始的位置
	DWORD MtrCount = 0;                     // 材质数目
	CPY_FROM_STREAM(MtrCount, Offset)       // 读取材质数目
	for (DWORD MtrIdx = 0; MtrIdx < MtrCount; ++MtrIdx)
	{
		DWORD len = 0;
		CPY_FROM_STREAM(len, Offset)        // Mtrl Name length
		std::string sMtlName = "";
		if (len > 0)
		{
			char *pName = new char[len + 1];
			ZeroMemory(pName, len + 1);
			memcpy(pName, Offset, len);         // Mtrl Name
			Offset += len;
			// pMtrl->setName(std::string(pName));	
			sMtlName = std::string(pName);
			delete [] pName;
			pName = NULL;
		}
		else
		{
			// 创建材质资源到指定资源组, Ogre会自动创建一个technique和一个pass
			// pMtrl = MaterialManager::getSingleton().create(pName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			MessageBox(NULL, TEXT("数据中没有为材质指定名称，可能会导致问题！"), TEXT("错误"), 0);
			continue;
		}

		//创建材质资源到指定资源组
		CMaterial *pMtrl = m_pVGSSceneMgr->CreateMaterial(sMtlName);
		if (!pMtrl)
		{
			continue;
		}

		bool bRecShadow = false;
		float USpeed, VSpeed;
		CPY_FROM_STREAM(bRecShadow, Offset);
		CPY_FROM_STREAM(USpeed, Offset);
		CPY_FROM_STREAM(VSpeed, Offset);

		// all technology
		UINT techCount;
		CPY_FROM_STREAM(techCount, Offset);
		for (UINT iTech = 0; iTech < techCount; ++iTech)
		{
			UINT nameLen;
			CPY_FROM_STREAM(nameLen, Offset);
			char *pName = new char[nameLen];
			CPY_PTR_FROM_STREAM(pName, nameLen, Offset);
			delete [] pName;
			pName = 0;

			// all pass
			UINT passCount = 0;
			CPY_FROM_STREAM(passCount, Offset);
			for (UINT iPass = 0; iPass < passCount; ++iPass)
			{
				UINT nameLen;
				CPY_FROM_STREAM(nameLen, Offset);
				Offset += nameLen;  // 跳过name

				// 材质的基本参数
				V3D_PassDetail PassDetail;
				CPY_FROM_STREAM(PassDetail, Offset)

				// 注意：旧版本V3D中保存的颜色是rgba格式
				pMtrl->m_bUseDiffuse     = PassDetail.bUseDiffuse;
				if (pMtrl->m_bUseDiffuse)
				{
					pMtrl->d3dmtrl.Diffuse = _Get_D3DCOLORVALUE_RGBA(PassDetail.Dif_RGBA); // D3DCOLOR_RGBA(PassDetail.basedetail.Dif_RGBA.r, PassDetail.basedetail.Dif_RGBA.g, PassDetail.basedetail.Dif_RGBA.b, PassDetail.basedetail.Dif_RGBA.a);
				}
				else
				{
					D3DCOLORVALUE color;
					color.r = color.g = color.b = 1.0f;
					color.a = (PassDetail.Dif_RGBA & 0x000000ff) / 255.0f; // 演示是rgba格式
					pMtrl->d3dmtrl.Diffuse = color;
				}
				pMtrl->d3dmtrl.Ambient = _Get_D3DCOLORVALUE_RGBA(PassDetail.Amb_RGBA); // D3DCOLOR_RGBA(PassDetail.basedetail.Amb_RGBA.r, PassDetail.basedetail.Amb_RGBA.g, PassDetail.basedetail.Amb_RGBA.b, PassDetail.basedetail.Amb_RGBA.a);		

				pMtrl->specular = _Get_D3DCOLORVALUE_RGBA(PassDetail.Spe_RGBA); // D3DCOLOR_RGBA(PassDetail.basedetail.Spe_RGBA.r, PassDetail.basedetail.Spe_RGBA.g, PassDetail.basedetail.Spe_RGBA.b, PassDetail.basedetail.Spe_RGBA.a);
				pMtrl->specularLevel = PassDetail.Spe_Level;
				float factor = pMtrl->specularLevel / 100.0f;
				pMtrl->d3dmtrl.Specular = _Multiply_D3DCOLORVALUE(pMtrl->specular, factor);
				pMtrl->d3dmtrl.Power    = PassDetail.shininess;
				
				pMtrl->selfIllumination = (UINT)PassDetail.selfIllumination;
				factor = pMtrl->selfIllumination / 100.0f;
				pMtrl->d3dmtrl.Emissive = _Multiply_D3DCOLORVALUE(pMtrl->d3dmtrl.Diffuse, factor);
				
				if (PassDetail.RejFunc == 1/*CMPF_ALWAYS_PASS*/)
				{
					pMtrl->bAlphaTest = false;
				}
				else
				{
					pMtrl->bAlphaTest = true;
				}
				pMtrl->AlpahaTestValue = PassDetail.AlphaValue;

				if (PassDetail.CullMode == 1 /*CULL_NONE*/)
				{
					pMtrl->twoSide = true;
				}
				else 
				{
					pMtrl->twoSide = false;
				}

				pMtrl->fillMode = (_D3DFILLMODE)PassDetail.PolygonMode;
				pMtrl->setOpacity((float)(pMtrl->d3dmtrl.Diffuse.a * 100/*/ 2.55f*/ + 0.5));  // 由[0, 255]换算成[0, 100]

				// 材质的贴图层
				UINT texLayerCount;
				CPY_FROM_STREAM(texLayerCount, Offset)
				for (UINT iLayer = 0; iLayer < texLayerCount; ++iLayer)
				{
					CTextureLayer *pLayer = new CTextureLayer;
					UINT NameLen;
					CPY_FROM_STREAM(NameLen, Offset)
					char *pTmp = new char[NameLen + 1];
					ZeroMemory(pTmp, NameLen + 1);
					memcpy(pTmp, Offset, NameLen);
					pLayer->m_sName = std::string(pTmp);
					delete [] pTmp;
					Offset += NameLen;

					UINT uvIndex;
					BYTE mipMap, minFilter, magFilter, mipFilter;
					FLOAT USpeed, VSpeed;
					int texType;
					CPY_FROM_STREAM(uvIndex, Offset)
					CPY_FROM_STREAM(minFilter, Offset)
					CPY_FROM_STREAM(magFilter, Offset)
					CPY_FROM_STREAM(mipFilter, Offset)
					CPY_FROM_STREAM(mipMap, Offset)
					CPY_FROM_STREAM(USpeed, Offset)
					CPY_FROM_STREAM(VSpeed, Offset)
					CPY_FROM_STREAM(texType, Offset)
					pLayer->UVIndex = uvIndex;
					pLayer->USpeed = USpeed;
					pLayer->VSpeed = VSpeed;
					pLayer->type = (VGS_TextureType)texType;  // 层类型

					int dynamicDuration;
					char reflectType;
					VGS_BumpType bumpType;
					VGS_BumpwaterTextureParam  bumpWaterParam;
					VGS_BumpEnvTextureParam    bumpEnvParam;
					VGS_BumpNormalTextureParam bumpNormalParam;
					VGS_RTF_PARAM              reflectionParam;  // 实时反射
					VGS_RTF_PARAM              refrationParam;   // 实时折射
					if (pLayer->type == TLT_DYNAMIC)
					{						
						CPY_FROM_STREAM(dynamicDuration, Offset);
					}
					else if (pLayer->type == TLT_REFLECT)
					{						
						CPY_FROM_STREAM(reflectType, Offset);
					}
					else if (pLayer->type == TLT_BUMPMAP)
					{						
						CPY_FROM_STREAM(bumpType, Offset);
						if (bumpType == VBT_BUMPWATER)
						{
							CPY_FROM_STREAM(bumpWaterParam.XScale, Offset);
							CPY_FROM_STREAM(bumpWaterParam.YScale, Offset);
							CPY_FROM_STREAM(bumpWaterParam.Radius, Offset);
							CPY_FROM_STREAM(bumpWaterParam.Speed, Offset);
							CPY_FROM_STREAM(bumpWaterParam.CenterX, Offset);
							CPY_FROM_STREAM(bumpWaterParam.CenterY, Offset);
						}
						else if (bumpType == VBT_BUMPENVIRONMENT)
						{
							CPY_FROM_STREAM(bumpEnvParam.noise1, Offset);
							CPY_FROM_STREAM(bumpEnvParam.noise2, Offset);
						}
						else if (bumpType == VBT_BUMPNORMARLMAP)
						{
							int lightIndex;
							CPY_FROM_STREAM(lightIndex, Offset);
							if (lightIndex >= 0 && lightIndex < m_vpLight.size())
							{
								bumpNormalParam.LightHandle = (DWORD)m_vpLight[lightIndex];
							}
							else
							{
								bumpNormalParam.LightHandle = 0;
							}
						}
					}
					else if (pLayer->type == TLT_RTFMAP) // 实时反射
					{
						ZeroMemory(&reflectionParam, sizeof(reflectionParam));
						int  modelIndex;
						UINT refCount;
						CPY_FROM_STREAM(reflectionParam.width, Offset);
						CPY_FROM_STREAM(reflectionParam.height, Offset);
						CPY_FROM_STREAM(modelIndex, Offset);
						if (modelIndex >= 0 && modelIndex <= m_vModel.size())
						{
							reflectionParam.refectionModel = (DWORD)m_vModel[modelIndex];
						}
						CPY_FROM_STREAM(refCount, Offset);
						for (UINT icount = 0; icount < refCount; ++icount)
						{
							CPY_FROM_STREAM(modelIndex, Offset);
							if (modelIndex >= 0 && modelIndex <= m_vModel.size())
							{
								reflectionParam.vrefectedModels.push_back((DWORD)m_vModel[modelIndex]);
							}
						}
					}
					else if (pLayer->type == TLT_RTTMAP) // 实时折射
					{
						ZeroMemory(&refrationParam, sizeof(refrationParam));
						int  modelIndex;
						UINT refCount;
						CPY_FROM_STREAM(refrationParam.width, Offset);
						CPY_FROM_STREAM(refrationParam.height, Offset);
						CPY_FROM_STREAM(modelIndex, Offset);
						if (modelIndex >= 0 && modelIndex <= m_vModel.size())
						{
							refrationParam.refectionModel = (DWORD)m_vModel[modelIndex];
						}
						CPY_FROM_STREAM(refCount, Offset);
						for (UINT icount = 0; icount < refCount; ++icount)
						{
							CPY_FROM_STREAM(modelIndex, Offset);
							if (modelIndex >= 0 && modelIndex <= m_vModel.size())
							{
								refrationParam.vrefectedModels.push_back((DWORD)m_vModel[modelIndex]);
							}
						}
					}

					// 写入颜色的混色模式
					VGS_BlenderInfo colorBlendInfo;
					CPY_FROM_STREAM(colorBlendInfo, Offset);

					// 写入alpah通道的混色模式
					VGS_BlenderInfo alphaBlendInfo;
					CPY_FROM_STREAM(alphaBlendInfo, Offset);

					// 帧数目
					unsigned short iFrameCount;
					CPY_FROM_STREAM(iFrameCount, Offset);

					// 读取每一张图片的名称
					vector <std::string> vImage;
					for (UINT iFrame = 0; iFrame < iFrameCount; ++iFrame)
					{
						UINT len;
						CPY_FROM_STREAM(len, Offset);
						char *pName = new char[len + 1];
						ZeroMemory(pName, len + 1);
						memcpy(pName, Offset, len);
						std::string strFileName(pName);
						strFileName = AddPathtoTextureFile(strFileName);
						Offset += len;
						vImage.push_back(strFileName);
						SAFE_DELETE_ARRAY(pName);
					}

					// 创建贴图层					
					pLayer->pTexture = 0;
					switch (pLayer->type)
					{
					case TLT_OPACITY:
						if (vImage.size() > 0)
						{
							pLayer->pTexture = m_pRoot->GetTextureManager().createTexture(vImage[0], TRUE, mipMap);
						}
						break;
					case TLT_BUMPMAP:
						{
							std::string strName = pLayer->m_sName;
							if (vImage.size() > 0)
							{
								strName = vImage[0];
							}
							pLayer->pTexture = m_pRoot->GetTextureManager().CreateBumpTexture(strName);
							CBumpMap *pBump = ((CBumpMap*)pLayer->pTexture);
							if (pBump && bumpType == VBT_BUMPWATER)
							{
								pBump->SetBumpAsWavemap(bumpWaterParam.width, bumpWaterParam.height, bumpWaterParam.XScale, bumpWaterParam.YScale,
									                    bumpWaterParam.Radius, bumpWaterParam.Speed, bumpWaterParam.CenterX, bumpWaterParam.CenterY);
							}
							else if (pBump && bumpType == VBT_BUMPENVIRONMENT)
							{
								pBump->SetBumpAsEnviromentmap(strName, bumpEnvParam.noise1, bumpEnvParam.noise2);
							}
							else if (pBump && bumpType == VBT_BUMPNORMARLMAP)
							{
								pBump->SetBumpAsNormalmap(strName, (CLight*)bumpNormalParam.LightHandle);
							}
							break;
						}
					case TLT_RTFMAP: // 实时反射
						{
							std::string strName = pLayer->m_sName;
							if (vImage.size() > 0)
							{
								strName = vImage[0];
							}
							pLayer->pTexture = m_pRoot->GetTextureManager().CreateRTreflectTexture(strName, refrationParam.width, reflectionParam.height);
							CRTFMap* pRTFMap = (CRTFMap*)pLayer->pTexture;
							if (pRTFMap)
							{
								pRTFMap->SetRefPlaneByModel(reflectionParam.refectionModel);
								for (UINT iModel = 0; iModel < reflectionParam.vrefectedModels.size(); ++iModel)
								{
									pRTFMap->AddRefModel(reflectionParam.vrefectedModels[iModel]);
								}
							}
						}
						break;
					case TLT_RTTMAP:  // 实时折射
						{
							/*std::string strName = pLayer->m_sName;
							if (vImage.size() > 0)
							{
								strName = vImage[0];
							}
							pLayer->pTexture = CTextureManager::getSingleton().CreateRTreflectTexture(strName, refrationParam.width, reflectionParam.height);
							CRTFMap* pRTFMap = (CRTFMap*)pLayer->pTexture;
							if (pRTFMap)
							{
								pRTFMap->SetRefPlaneByModel(reflectionParam.refectionModel);
								for (UINT iModel = 0; iModel < reflectionParam.vrefectedModels.size(); ++iModel)
								{
									pRTFMap->AddRefModel(reflectionParam.vrefectedModels[iModel]);
								}
							}*/
						}
						break;
					case TLT_BASAL:
					case TLT_REFLECT:
					default:
						if (vImage.size() > 0)
						{
							pLayer->pTexture = m_pRoot->GetTextureManager().createTexture(vImage[0], FALSE, mipMap);
						}
						break;
					}
					

					// 写入颜色的混色模式
					pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(colorBlendInfo.option);
					pLayer->Blenderinfo.ColorBlendParam  = D3DCOLOR_COLORVALUE(0, 0, 0, colorBlendInfo.param.BlenderOP_alpha);
					
					// 写入alpah通道的混色模式
					pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(alphaBlendInfo.option);
					pLayer->Blenderinfo.AlphaBlendParam  = D3DCOLOR_COLORVALUE(0, 0, 0, alphaBlendInfo.param.BlenderOP_alpha);


					// 保存到材质的层列表
					pMtrl->m_vpTexLayers.push_back(pLayer);

				} // for iLayer
			} // for ipass			
		} // for itech
		
		// m_pVGSSceneMgr->m_vShader.push_back(pMtrl);
		m_vMaterial.push_back(pMtrl);  //  save to temp list

	} // Material 

	DELETE_ARRAYPTR(m_pMaterailBuf);  // 删除字节流

	return ERR_COMMON_OK;
}	

DWORD CGetV3DImpV2::ReadMeshData()
{
	m_vMesh.clear();

	if (!m_pFile)
	{
		// throw Exception::exception("读文件发生错误(03)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pMeshBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 读取entity信息
	char* Offset = m_pMeshBuf;              // 字节流中数据开始的位置
	DWORD meshCount;                        // entity数目
	CPY_FROM_STREAM(meshCount, Offset)      // 读取entity数目

	m_vMesh.resize(meshCount);

	// 读取每一个mesh
	for (DWORD iMesh = 0; iMesh < meshCount; ++iMesh)
	{
		CVGSMesh &mesh = m_vMesh[iMesh];
		mesh.faceCnt = 0;

		UINT len;
		CPY_FROM_STREAM(len, Offset);
		char *pName = new char[len + 1];
		ZeroMemory(pName, len + 1);
		CPY_PTR_FROM_STREAM(pName, len, Offset);
		mesh.sName = std::string(pName);
		SAFE_DELETE_ARRAY(pName);

		UINT vertexElementCount;
		CPY_FROM_STREAM(vertexElementCount, Offset);  // 顶点声明的个数
		UINT verBufSize = sizeof(unsigned short) + sizeof(UINT) + 3 * sizeof(unsigned short);
		Offset += verBufSize * vertexElementCount;  // 跳过这段数据

		UINT startIndex;
		CPY_FROM_STREAM(mesh.vertexCnt, Offset);
		CPY_FROM_STREAM(startIndex, Offset); // 始终是0

		mesh.fvf = FVF_POSNORMALCOLORCOORD4;  // 固定格式
		mesh.pVertex = (void*)(new CVertex_POSNORMALCOLORCOORD4[mesh.vertexCnt]);
		memcpy(mesh.pVertex, Offset, mesh.vertexCnt * sizeof(CVertex_POSNORMALCOLORCOORD4));
		Offset += mesh.vertexCnt * sizeof(CVertex_POSNORMALCOLORCOORD4);

		//  modify vertex position and normal, because of the coord (right or left hand)
		for (DWORD iVer = 0; iVer < mesh.vertexCnt; ++iVer)
		{
			((CVertex_POSNORMALCOLORCOORD4*)mesh.pVertex)[iVer].position.z *= -1;
			((CVertex_POSNORMALCOLORCOORD4*)mesh.pVertex)[iVer].normal.z *= -1;
		}

		// 读每一个submesh
		unsigned short subMeshCount;
		CPY_FROM_STREAM(subMeshCount, Offset);
		mesh.subMeshCount = subMeshCount;
		mesh.pSubMesh = (void*)new CVGSSubMesh[mesh.subMeshCount];
		for (UINT iSubMesh = 0; iSubMesh < mesh.subMeshCount; ++iSubMesh)
		{
			CVGSSubMesh &subMesh = ((CVGSSubMesh*)mesh.pSubMesh)[iSubMesh];
			UINT len;
			CPY_FROM_STREAM(len, Offset);
			char *pMtrName = new char[len + 1];
			ZeroMemory(pMtrName, len + 1);
			CPY_PTR_FROM_STREAM(pMtrName, len, Offset);
			std::string sMtrName = std::string(pMtrName);  // submesh使用的材质名称
			//CMaterial* pMaterial = m_pVGSSceneMgr->GetMaterialByName(sMtrName);
			//subMesh.mtrIndex = m_pVGSSceneMgr->getMaterialIndex(pMaterial);
			subMesh.mtrIndex = _GetIndexByName(m_vMaterial, sMtrName);
			SAFE_DELETE_ARRAY(pName);

			// save material to scene list
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_3D, _GetHandleByName(m_vMaterial, sMtrName));

			UINT indexCount, startIndex;
			CPY_FROM_STREAM(indexCount, Offset);
			CPY_FROM_STREAM(startIndex, Offset); // 始终是0
			subMesh.faceCount = indexCount / 3;
			mesh.faceCnt += subMesh.faceCount; // 计算mesh的总面数
			subMesh.pFace = new CVGSFace[subMesh.faceCount];

			int indexsize = 0;
			if (mesh.vertexCnt > 65535)
			{
				indexsize = sizeof(DWORD);
				UINT iVertexIndex = 0;
				//memcpy((void*)subMesh.pFace, Offset, indexCount * indexsize);
				WORD *pIndexData = (WORD*)Offset;
				for (UINT nFace = 0; nFace < subMesh.faceCount; nFace++)
				{
					subMesh.pFace[nFace].a = pIndexData[iVertexIndex++];
					subMesh.pFace[nFace].b = pIndexData[iVertexIndex++];
					subMesh.pFace[nFace].c = pIndexData[iVertexIndex++];
				}
			}
			else
			{
				indexsize = sizeof(WORD);
				UINT iVertexIndex = 0;
				WORD *pIndexData = (WORD*)Offset;
				for (UINT nFace = 0; nFace < subMesh.faceCount; nFace++)
				{
					subMesh.pFace[nFace].a = pIndexData[iVertexIndex++];
					subMesh.pFace[nFace].b = pIndexData[iVertexIndex++];
					subMesh.pFace[nFace].c = pIndexData[iVertexIndex++];
				}
			}
			Offset += indexCount * indexsize;
		}

		// 包围盒和包围球
		vector3 boxNearConner, boxFarConner;
		float sphereRadius;
		CPY_FROM_STREAM(boxNearConner, Offset);
		CPY_FROM_STREAM(boxFarConner, Offset);
		CPY_FROM_STREAM(sphereRadius, Offset);
	}

	return ERR_COMMON_OK;
}

// 读取模型数据
DWORD CGetV3DImpV2::ReadModelData()
{
	if (!m_pFile)
	{
		// throw Exception::exception("读文件发生错误(03)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pEntityBuf)
	{
		return ERR_FILE_V3D_NODATA; 
	}

	// 读取entity信息
	char* Offset = m_pEntityBuf;              // 字节流中数据开始的位置
	DWORD EntityCount;                        // entity数目
	CPY_FROM_STREAM(EntityCount, Offset)      // 读取entity数目
	
	// 读取每个模型
	for (UINT idx = 0; idx < EntityCount; ++idx)
	{
		int namelen = 0;
		char sName[MAX_FILENAME_LENGTH];
		ZeroMemory(sName, MAX_FILENAME_LENGTH);
		CPY_FROM_STREAM(namelen, Offset)      // entity名称
		
		memcpy(sName, Offset, namelen);       // 读取名称
		Offset += namelen;

		CModel *pModel = m_pVGSSceneMgr->CreateModel(std::string(sName));

		CVGSMesh* pMesh = 0;
		int meshIndex;
		CPY_FROM_STREAM(meshIndex, Offset);
		if (meshIndex >= 0 && meshIndex < m_vMesh.size())
		{
			pMesh = &m_vMesh[meshIndex];
		}
		
		bool bAutoNormal, bBillborad, bClickable, bCollision, bIsGround, bReflection, bVisible;
		VGS_AXIS_TYPE billboardUp, billboardDir;
		CPY_FROM_STREAM(bAutoNormal, Offset);
		CPY_FROM_STREAM(bBillborad, Offset);
		CPY_FROM_STREAM(bClickable, Offset);
		CPY_FROM_STREAM(bCollision, Offset);
		CPY_FROM_STREAM(bIsGround, Offset);
		CPY_FROM_STREAM(billboardUp, Offset);
		CPY_FROM_STREAM(billboardDir, Offset);
		CPY_FROM_STREAM(bVisible, Offset);
		CPY_FROM_STREAM(bReflection, Offset)      // 是否反射其他模型
			
		pModel->isAutoNormalise = bAutoNormal;
		pModel->isBillboard   = bBillborad;
		pModel->m_isClickable = bClickable;
		pModel->isCollision = bCollision;
		pModel->isGround = bIsGround;
		pModel->visible = bVisible;

		pModel->bReflect = bReflection;
		if (pModel->bReflect)
		{
			UINT reflectionCount = 0;
			CPY_FROM_STREAM(reflectionCount, Offset)      // 所反射的模型数目
			for (UINT iModel = 0; iModel < reflectionCount; ++iModel)
			{
				int ModelIndex;
				CPY_FROM_STREAM(ModelIndex, Offset)      // 所反射的模型数目
				pModel->refedModelIdxList.push_back(ModelIndex);
			}
		} // if isReflection

		bool bIsRefration;
		CPY_FROM_STREAM(bIsRefration, Offset);      // 是否折射其他模型
		//pModel->bReflect
		//if (pModel->bReflect)
		//{
		//	UINT reflectionCount = 0;
		//	CPY_FROM_STREAM(reflectionCount, Offset)      // 所反射的模型数目
		//		for (UINT iModel = 0; iModel < reflectionCount; ++iModel)
		//		{
		//			int ModelIndex;
		//			CPY_FROM_STREAM(ModelIndex, Offset)      // 所反射的模型数目
		//				pModel->refedModelIdxList.push_back(ModelIndex);
		//		}
		//} // if isReflection

		DWORD dwMajorPriority = 0;
		DWORD dwMinorPriority = 0;
		if (CURRENT_V3DVERSION >= 50009)
		{
			CPY_FROM_STREAM(dwMajorPriority, Offset);
			CPY_FROM_STREAM(dwMinorPriority, Offset);
		}

		pModel->index = m_pVGSSceneMgr->GetModelCount();

		//vector<CMaterial*> vMaterial;
		//m_pVGSSceneMgr->getMaterialList(vMaterial);
		pModel->Create(pMesh, m_vMaterial);
		SAFE_RELEASE(pMesh);

		m_vModel.push_back(pModel);
	}

	m_vMesh.clear();
	DELETE_ARRAYPTR(m_pEntityBuf);  // 删除字节流
	DELETE_ARRAYPTR(m_pMeshBuf);    // 删除字节流

	return ERR_COMMON_OK;
}



//创建相机的参数动画
void CGetV3DImpV2::CreateCameraParamAnimation(CCamera* pCamera, VGS_V3D_Camera* pVGSCamera, int frameCnt)
{
	//关键帧数据
// 	CameraParamAnimation* pCameraParamAni = new CameraParamAnimation(pCamera);
// 
// 	pCameraParamAni->mCamera = pCamera; // fuxb 2008-4-18
// 	pCameraParamAni->frameCnt = frameCnt;
// 	pCameraParamAni->cameraInfoList = new VGS_V3D_CAMINFO[frameCnt];
// 	for (INT frameIndex = 0; frameIndex < frameCnt; frameIndex++)
// 	{
// 		pCameraParamAni->cameraInfoList[frameIndex] = pVGSCamera->v3dAnimationInfo[frameIndex];
// 	}
// 
// 	pCameraParamAni->setLength(Real(frameCnt)/gkfrPlaySpeed);
// 	
// 	//放入相机参数动画队列
// 	// cameraParamAnimationList.push_back(pCameraParamAni);
// 	m_pVGSSceneMgr->GetAnimationManager()->AddCameraAnimation(pCameraParamAni);
}
BOOL CGetV3DImpV2::CreateCameraNodeAnimation(CSceneNode* pSceneNode, CCamera* pCamera, const std::vector<CNodeTransform> &vTransform)
{
	if (NULL == pSceneNode || NULL == pCamera)
		return FALSE;

	//查找相继关键帧信息
	std::vector<CVGS_CAMERA_ANIMINFO> vCamAnimInfo;
	std::string strCameraName = pCamera->getName();
	for (std::vector<VGS_V3D_Camera>::iterator it = m_arCameraFrameInfo.begin();
		 it != m_arCameraFrameInfo.end();
		 it++)
	{
		std::string strCurName = (*it).name;
		if (stricmp(strCurName.c_str(), strCameraName.c_str()) == 0)
		{
			for (std::vector<VGS_V3D_CAMINFO>::iterator it2 = (*it).v3dAnimationInfo.begin();
				 it2 != (*it).v3dAnimationInfo.end();
				 it2++)
			{
				CVGS_CAMERA_ANIMINFO camAnimInfo;
				VGS_V3D_CAMINFO camFrameInfo = (*it2);
				camAnimInfo.farClip = camFrameInfo.farClipDis;
				camAnimInfo.nearClip = camFrameInfo.nearClipDis;
				camAnimInfo.fov = camFrameInfo.fov;
				vCamAnimInfo.push_back(camAnimInfo);
			}
		}
	}
	
	CNodeAnimation* pNodeAnimation = m_pRoot->GetAnimManager().CreateCameraAnimation(pSceneNode, vTransform, vCamAnimInfo);

	return pNodeAnimation ? TRUE : FALSE;
}


//读取相机数据并创建相机到场景
DWORD CGetV3DImpV2::ReadCameraData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}
	if (!m_pCameraBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 字节流中数据开始的位置
	char* Offset = m_pCameraBuf;   
	
	m_vpCamera.clear();  // 情况相机列表	
	m_arCameraFrameInfo.clear();

	//读入相机数量
	UINT cameraCnt = 0;                         
	CPY_FROM_STREAM(cameraCnt, Offset)       

	//逐个相机读入数据
	for (UINT i = 0; i < cameraCnt; i++)
	{
		//相机名称
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset); 
		char *pName = new char[len + 1];
		ZeroMemory(pName, len + 1);
		//strncpy_s(pName, sizeof(len + 1), Offset, len); // '\0' excluded
		//Offset += len;
		CPY_PTR_FROM_STREAM(pName, len, Offset);
		std::string sName = std::string(pName);
		delete pName;
		pName = NULL;

		// 创建相机
		CCamera *pCamera = m_pVGSSceneMgr->CreateCamera(sName);
		if (!pCamera)
		{
			return ERR_SCENE_CREATE;
		}

		// 相机的基本参数
		VGS_V3D_CAMINFO camInfo;
		// CPY_FROM_STREAM(camInfo, Offset)
		CPY_FROM_STREAM(camInfo.isOrtho, Offset);
		CPY_FROM_STREAM(camInfo.fov, Offset);
		CPY_FROM_STREAM(camInfo.nearClipDis, Offset);
		CPY_FROM_STREAM(camInfo.farClipDis, Offset);
		CPY_FROM_STREAM(camInfo.position, Offset);
		CPY_FROM_STREAM(camInfo.quat, Offset);

		// 投影类型
		if (camInfo.isOrtho)
		{
			pCamera->m_camType = VCT_ORTHO;
		}
		else
		{
			pCamera->m_camType = VCT_PROJECT;
		}
		
		// Fov
		pCamera->SetFov(camInfo.fov);

		//near far clip
		pCamera->setNearClip(camInfo.nearClipDis);
		pCamera->setFarClip(camInfo.farClipDis);

		pCamera->SetPosition(&D3DXVECTOR3(camInfo.position.x, camInfo.position.y, -camInfo.position.z), VTS_PARENT);
		pCamera->setRotation(D3DXQUATERNION(camInfo.quat.x, camInfo.quat.y, camInfo.quat.z, camInfo.quat.w)/*camInfo.quat*/, VTS_PARENT);
		// pCamera->Lookat(&D3DXVECTOR3(camInfo.at.x, camInfo.at.y, camInfo.at.z), VTS_PARENT);
		// pCamera->SetRoll(camInfo.roll);

		m_vpCamera.push_back(pCamera);   // 保存起来，后面可能会使用
		
		VGS_V3D_Camera cameraFrameInfo;
		ZeroMemory(&cameraFrameInfo, sizeof(cameraFrameInfo));
		cameraFrameInfo.name = pCamera->getName();
		cameraFrameInfo.nameLen = cameraFrameInfo.name.size();

		//读入参数动画帧数
		int frameCnt;
		CPY_FROM_STREAM(frameCnt, Offset);
		for (int iFrame = 0; iFrame < frameCnt; iFrame++)
		{
			VGS_V3D_CAMINFO camFrameInfo;
			CPY_FROM_STREAM(camFrameInfo, Offset);
			camFrameInfo.position.z *= -1;
			camFrameInfo.quat.x *= -1;
			camFrameInfo.quat.y *= -1;
			camFrameInfo.at.z *= -1;
			camFrameInfo.farClipDis = 100000;
			camFrameInfo.nearClipDis = 1;
			camFrameInfo.fov = 0.6;
			cameraFrameInfo.v3dAnimationInfo.push_back(camFrameInfo);
		}
		m_arCameraFrameInfo.push_back(cameraFrameInfo);
	}

	DELETE_ARRAYPTR(m_pCameraBuf);  // 删除字节流

	return ERR_COMMON_OK;
}


DWORD CGetV3DImpV2::ReadLightData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pLightBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}
	if (!m_pVGSSceneMgr)
	{
		return ERR_SCENE_NOSCENE;
	}

	//读取灯光的数量
	char* Offset = m_pLightBuf; 
	int LightCount = 0; 
	CPY_FROM_STREAM(LightCount, Offset);

	m_vpLight.clear();	
	m_arLightFrameInfo.clear();

	//逐个灯光读取数据
	for (int i = 0; i < LightCount; i++)
	{
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset);
		char pName[256];
		ZeroMemory(pName, 256);
		memcpy(pName, Offset, len);
		Offset += len;		
		
		CLight *pLight = m_pVGSSceneMgr->CreateLight(std::string(pName));
		if (!pLight)
		{
			return ERR_SCENE_CREATE;
		}

		//灯光类型
		VGS_LIGHT_TYPE type;
		CPY_FROM_STREAM(type, Offset)

		//灯光关键帧动画信息
		VGS_V3D_Light lightFrameInfo;
		ZeroMemory(&lightFrameInfo, sizeof(lightFrameInfo));
		lightFrameInfo.name = pLight->getName();
		lightFrameInfo.strLen = lightFrameInfo.name.size();
		lightFrameInfo.type = type;

		switch (type)
		{
		case VLT_OMNI:
			{
				//读入点光源信息
				VGS_V3D_OMNI omni;
				CPY_FROM_STREAM(omni, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((omni.color>>16) & 0xff)/255.0f;
				diffuse.g = ((omni.color>>8) & 0xff)/255.0f;
				diffuse.b = ((omni.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = omni.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * omni.multiply;
				pLight->param.Diffuse.g = diffuse.g * omni.multiply;
				pLight->param.Diffuse.b = diffuse.b * omni.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//环境光，用它来近似模拟全局光
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//高光
				pLight->param.Specular   = omni.multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
				pLight->param.Specular.a = 1.0f;
				
				//光线到达的最远距离
				pLight->param.Range        = omni.attenuationRange;
				pLight->param.Attenuation0 = omni.attenuation0;
				pLight->param.Attenuation1 = omni.attenuation1;
				pLight->param.Attenuation2 = omni.attenuation2;
				
				pLight->param.Type = D3DLIGHT_POINT;
				D3DXVECTOR3 vPos(omni.position.x, omni.position.y, -omni.position.z);  
				pLight->setPosition(&vPos);	

				//读入动画帧数
				int frameCnt = 0;
				CPY_FROM_STREAM(frameCnt, Offset);
				for (int iFrame = 0; iFrame < frameCnt; iFrame++)
				{
					VGS_V3D_OMNI omniLight;
					CPY_FROM_STREAM(omniLight, Offset);
					omniLight.position.z *= -1;
					lightFrameInfo.omniInfo.push_back(omniLight);
				}				
			}
			break;

		case VLT_DIR:
			{				
				//读入点光源信息
				VGS_V3D_DIRECTIONAL directionLight;
				CPY_FROM_STREAM(directionLight, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((directionLight.color>>16) & 0xff)/255.0f;
				diffuse.g = ((directionLight.color>>8) & 0xff)/255.0f;
				diffuse.b = ((directionLight.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = directionLight.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * directionLight.multiply;
				pLight->param.Diffuse.g = diffuse.g * directionLight.multiply;
				pLight->param.Diffuse.b = diffuse.b * directionLight.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//环境光，用它来近似模拟全局光
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//高光
				pLight->param.Specular   = directionLight.multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
				pLight->param.Specular.a = 1.0f;
				
				//光线到达的最远距离
				pLight->param.Range        = directionLight.attenuationRange;
				pLight->param.Attenuation0 = directionLight.attenuation0;
				pLight->param.Attenuation1 = directionLight.attenuation1;
				pLight->param.Attenuation2 = directionLight.attenuation2;
				
				pLight->param.Type = D3DLIGHT_DIRECTIONAL;

				//方向
				D3DXVECTOR3 dir(directionLight.direction.x, directionLight.direction.y, -directionLight.direction.z);
				pLight->setDirection(&dir);

				//读入动画帧数
				int frameCnt = 0;
				CPY_FROM_STREAM(frameCnt, Offset);
				for (int iFrame = 0; iFrame < frameCnt; iFrame++)
				{
					VGS_V3D_DIRECTIONAL dirLight;
					CPY_FROM_STREAM(dirLight, Offset);
					dirLight.direction.z *= -1;
					lightFrameInfo.directionLightInfo.push_back(dirLight);
				}
			}
			break;

		case VLT_SPOT:
			{
				//读入点光源信息
				VGS_V3D_SPOTLIGHT spotLight;
				CPY_FROM_STREAM(spotLight, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((spotLight.color>>16) & 0xff)/255.0f;
				diffuse.g = ((spotLight.color>>8) & 0xff)/255.0f;
				diffuse.b = ((spotLight.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = spotLight.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * spotLight.multiply;
				pLight->param.Diffuse.g = diffuse.g * spotLight.multiply;
				pLight->param.Diffuse.b = diffuse.b * spotLight.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//环境光，用它来近似模拟全局光
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb 把参数0,05改为了0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//高光
				pLight->param.Specular   = spotLight.multiply * pLight->diffuse;  // fuxb 2007-10-16 跟3ds max采用相同的处理方式
				pLight->param.Specular.a = 1.0f;
				
				//光线到达的最远距离
				pLight->param.Range        = spotLight.attenuationRange;
				pLight->param.Attenuation0 = spotLight.attenuation0;
				pLight->param.Attenuation1 = spotLight.attenuation1;
				pLight->param.Attenuation2 = spotLight.attenuation2;
				
				pLight->param.Type = D3DLIGHT_SPOT;
				
				//方向
				D3DXVECTOR3 dir(spotLight.direction.x, spotLight.direction.y, -spotLight.direction.z);
				pLight->setDirection(&dir);

				//张角
				pLight->param.Theta = spotLight.innerAngle;
				pLight->param.Phi = spotLight.outerAngle;
				pLight->param.Falloff = spotLight.fallOff;

				// 位置
				D3DXVECTOR3 vPos(spotLight.position.x, spotLight.position.y, -spotLight.position.z);  
				pLight->setPosition(&vPos);
				
				//读入动画帧数
				int frameCnt = 0;
				CPY_FROM_STREAM(frameCnt, Offset);
				for (int iFrame = 0; iFrame < frameCnt; iFrame++)
				{
					VGS_V3D_SPOTLIGHT spotLight;
					CPY_FROM_STREAM(spotLight, Offset);
					spotLight.direction.z *= -1;
					spotLight.position.z *= -1;
					lightFrameInfo.spotLightInfo.push_back(spotLight);
				}
			}
			break;
		}
		m_vpLight.push_back(pLight);  // 保存以及加载的灯光
		m_arLightFrameInfo.push_back(lightFrameInfo);
	}

	DELETE_ARRAYPTR(m_pLightBuf);  // 删除字节流

	return ERR_COMMON_OK;
}

//创建灯光参数动画
void CGetV3DImpV2::CreateLightParamAnimation(CLight* pLight, VGS_Light* pVGSLight, int frameCnt, char** Offset)
{
	//创建动画参数对象
// 	LightParamAnimation* pLightParamAni = new LightParamAnimation(pLight);
// 	pLightParamAni->mLight = pLight;
// 	pLightParamAni->frameCnt = frameCnt;
// 	//写入每帧的数据
// 	switch (pVGSLight->type)
// 	{
// 	case LIGHTTYPE::OMINI:
// 		{
// 			pLightParamAni->omniInfoList = new VGS_V3D_OMNI[frameCnt];
// 			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
// 				CPY_FROM_STREAM(pLightParamAni->omniInfoList[frameIndex], *Offset);
// 		}
// 		break;
// 
// 	case LIGHTTYPE::DIRECTIONLIGHT:
// 		{	
// 			pLightParamAni->directionLightInfoList = new VGS_V3D_DIRECTIONAL[frameCnt];
// 			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
// 				CPY_FROM_STREAM(pLightParamAni->directionLightInfoList[frameIndex], *Offset);
// 		}
// 		break;
// 
// 	case LIGHTTYPE::SPOTLIGHT:
// 		{
// 			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
// 				CPY_FROM_STREAM(pLightParamAni->spotLightInfoList[frameIndex], *Offset);
// 		}
// 		break;
// 	}
// 	
// 	pLightParamAni->setLength(Real(frameCnt)/gkfrPlaySpeed);
// 	
// 	//放入灯光参数动画队列
// 	// lightParamAnimationList.push_back(pLightParamAni);
// 	m_pVGSSceneMgr->GetAnimationManager()->AddLightAnimation(pLightParamAni);
}
BOOL CGetV3DImpV2::CreateLightNodeAnimation(CSceneNode* pSceneNode, CLight* pLight, const std::vector<CNodeTransform> &vTransform)
{	
	if (NULL == pSceneNode || NULL == pLight)
		return FALSE;

	//查找灯光关键帧信息
	std::vector<CVGS_LIGHT_INFO> vLightAnimInfo;
	std::string strLightName = pLight->getName();
	for (std::vector<VGS_V3D_Light>::iterator it = m_arLightFrameInfo.begin();
		 it != m_arLightFrameInfo.end();
		 it++)
	{
		std::string strCurName = (*it).name;
		if (stricmp(strCurName.c_str(), strLightName.c_str()) == 0)
		{
			switch(pLight->getType())
			{
			case VLT_OMNI:
				{
					for (std::vector<VGS_V3D_OMNI>::iterator it2 = (*it).omniInfo.begin();
						 it2 != (*it).omniInfo.end();
						 it2++)
					{
						CVGS_LIGHT_INFO lightAnimInfo;
						VGS_V3D_OMNI omniLightFrameInfo = (*it2);
						lightAnimInfo.color = omniLightFrameInfo.color;
						lightAnimInfo.range2 = omniLightFrameInfo.attenuationRange;
						lightAnimInfo.multiply = omniLightFrameInfo.multiply;
						vLightAnimInfo.push_back(lightAnimInfo);
					}
				}
				break;
			case VLT_DIR:
				{
					for (std::vector<VGS_V3D_DIRECTIONAL>::iterator it2 = (*it).directionLightInfo.begin();
						 it2 != (*it).directionLightInfo.end();
						 it2++)
					{
						CVGS_LIGHT_INFO lightAnimInfo;
						VGS_V3D_DIRECTIONAL dirLightFrameInfo = (*it2);
						lightAnimInfo.color = dirLightFrameInfo.color;
						lightAnimInfo.range2 = dirLightFrameInfo.attenuationRange;
						lightAnimInfo.multiply = dirLightFrameInfo.multiply;
						lightAnimInfo.dir = dirLightFrameInfo.direction;
						vLightAnimInfo.push_back(lightAnimInfo);
					}
				}
				break;
			case VLT_SPOT:
				{
					for (std::vector<VGS_V3D_SPOTLIGHT>::iterator it2 = (*it).spotLightInfo.begin();
						 it2 != (*it).spotLightInfo.end();
						 it2++)
					{
						CVGS_LIGHT_INFO lightAnimInfo;
						VGS_V3D_SPOTLIGHT spotLightFrameInfo = (*it2);
						lightAnimInfo.color = spotLightFrameInfo.color;
						lightAnimInfo.range2 = spotLightFrameInfo.attenuationRange;
						lightAnimInfo.multiply = spotLightFrameInfo.multiply;
						lightAnimInfo.dir = spotLightFrameInfo.direction;
						lightAnimInfo.theta = spotLightFrameInfo.outerAngle;
						lightAnimInfo.phi = spotLightFrameInfo.innerAngle;
						vLightAnimInfo.push_back(lightAnimInfo);
					}
				}
				break;
			}
		}
	}
	
	CNodeAnimation* pNodeAnimation = m_pRoot->GetAnimManager().CreateLightAnimation(pSceneNode, vTransform, pLight->getType(), vLightAnimInfo);

	return pNodeAnimation ? TRUE : FALSE;
}

//Read the Text data
DWORD CGetV3DImpV2::ReadTextData()
{
	return ERR_COMMON_OK;
}
//Read the overlay data
DWORD CGetV3DImpV2::ReadOverlayData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pOverlayBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}
	
 	char* Offset = m_pOverlayBuf;
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();
 
 	// 读取Overlay的数量
 	int iOverlayCnt = 0; 
 	CPY_FROM_STREAM(iOverlayCnt, Offset);

	// 读取Overlay属性并创建Overlay对象
	for (int iIndex = 0; iIndex < iOverlayCnt; iIndex++)
	{
 		V3D_OverlayInfo overlayInfo;
		CPY_FROM_STREAM(overlayInfo, Offset);

		std::string strBaseImage;
		std::string strOpacImage;
		if (strlen(overlayInfo.BaseImg) > 0) strBaseImage = AddPathtoTextureFile(overlayInfo.BaseImg);
		if (strlen(overlayInfo.OpacImg) > 0) strOpacImage = AddPathtoTextureFile(overlayInfo.OpacImg);

		CGUIObject* pOverlay = GUIMngr.CreateOverLayer(overlayInfo.Name, strBaseImage.c_str(), strOpacImage.c_str());
		if (NULL != pOverlay)
		{
 			// 视口大小
 			int iViewWidth = 0;
			int iViewHight = 0;
			GUIMngr.GetViewportSize(iViewWidth, iViewHight);
	 
			// overlay的实际坐标及大小
 			float fLeft = 0.0f;
			float fTop = 0.0f;
			float fWidth = 0.0f;
			float fHeight = 0.0f;
			// 转换成绝对坐标
 			if (overlayInfo.bRelativePos)
 			{
 				fLeft = iViewWidth * overlayInfo.Left;
 				fTop  = iViewHight * overlayInfo.Top;
 			}
 			else
 			{
 				fLeft = overlayInfo.Left;
 				fTop  = overlayInfo.Top;
 			}
			// 转换成绝对尺寸
 			if (overlayInfo.bRelativeSize)  
 			{
 				fWidth  = iViewWidth * overlayInfo.Width;
 				fHeight = iViewHight * overlayInfo.Height;
 			}
 			else
 			{
 				fWidth  = overlayInfo.Width;
 				fHeight = overlayInfo.Height;
 			}
			pOverlay->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);	 //先设置为左上角便于设置位置
			pOverlay->SetUsedRelativePos(overlayInfo.bRelativePos);
			pOverlay->SetUsedRelativeSize(overlayInfo.bRelativeSize);
			pOverlay->SetSize(fWidth, fHeight);
			pOverlay->SetOffsetOrigin(VGSOFFSETORIGIN(overlayInfo.Origin));	//设置offset origin
			pOverlay->SetPosition(fLeft, fTop);
			pOverlay->SetOpacity(overlayInfo.Opacity);
			pOverlay->SetVisibility(overlayInfo.bVisible);

			// save to scene lits
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pOverlay->GetMaterial());
		}
	}
 
	return ERR_COMMON_OK;
}
//Read the navigater data
DWORD CGetV3DImpV2::ReadNavigaterData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pNaviBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}
 
 	char* Offset = m_pNaviBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// 读取导航图的数量
 	int iNavCount = 0; 
 	CPY_FROM_STREAM(iNavCount, Offset);
 
	// 读取导航图属性并创建导航图对象
 	for (int iIndex = 0; iIndex < iNavCount; iIndex++)
 	{
		FRect mapRect;
		V3D_NavigatorInfo naviInfo; 
		ZeroMemory(&mapRect, sizeof(FRect));
		ZeroMemory(&naviInfo, sizeof(V3D_NavigatorInfo));
 
		// 基本属性
 		CPY_FROM_STREAM(naviInfo.NaviBaseInfo, Offset); 
 
		// map导航区域	
 		CPY_FROM_STREAM(mapRect, Offset); 
 
 		// 视口大小
		int iViewWidth = 0;
		int iViewHight = 0;
		GUIMngr.GetViewportSize(iViewWidth, iViewHight);
 
		// 转换成绝对坐标
 		FRect rect;
 		if (naviInfo.NaviBaseInfo.bRelativePos)  
 		{
 			rect.Left = iViewWidth * naviInfo.NaviBaseInfo.Left;
 			rect.Top  = iViewHight * naviInfo.NaviBaseInfo.Top;
 		}
 		else
 		{
 			rect.Left = naviInfo.NaviBaseInfo.Left;
 			rect.Top  = naviInfo.NaviBaseInfo.Top;
 		} 
		// 转换成绝对坐标
 		if (naviInfo.NaviBaseInfo.bRelativeSize)  
 		{
 			rect.Right  = rect.Left + iViewWidth * naviInfo.NaviBaseInfo.Width;
 			rect.Bottom = rect.Top + iViewHight * naviInfo.NaviBaseInfo.Height;
 		}
 		else
 		{
 			rect.Right  = rect.Left + naviInfo.NaviBaseInfo.Width;
 			rect.Bottom = rect.Top +naviInfo.NaviBaseInfo.Height;
 		}		
 
		// 导航点个数
 		CPY_FROM_STREAM(naviInfo.PointerCount, Offset); 
		if (naviInfo.PointerCount > 0)
		{
			// 导航点信息
			naviInfo.PointerList = new V3D_NaviPointerInfo[naviInfo.PointerCount];
			ZeroMemory(naviInfo.PointerList, naviInfo.PointerCount*sizeof(V3D_NaviPointerInfo));
 			for (int iPointer = 0; iPointer < naviInfo.PointerCount; iPointer++)
 			{
 				CPY_FROM_STREAM(naviInfo.PointerList[iPointer], Offset);
 			}
		}
 
 		// 创建导航图
		std::string strMapImage = AddPathtoTextureFile(naviInfo.NaviBaseInfo.MapImg);
		CNavigator* pNavigator = (CNavigator*)GUIMngr.CreateNavigator(naviInfo.NaviBaseInfo.Name, strMapImage.c_str());
		if (NULL != pNavigator)
		{
			pNavigator->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);					// 先设置为左上角便于设置位置
			pNavigator->SetUsedRelativePos(naviInfo.NaviBaseInfo.bRelativePos);
			pNavigator->SetUsedRelativeSize(naviInfo.NaviBaseInfo.bRelativeSize);
			pNavigator->SetSize(rect.Right - rect.Left, rect.Bottom - rect.Top);
			pNavigator->SetOffsetOrigin(VGSOFFSETORIGIN(naviInfo.NaviBaseInfo.Origin));  // 设置offset origin
			pNavigator->SetPosition(rect.Left, rect.Top);
			pNavigator->SetOpacity(naviInfo.NaviBaseInfo.Opacity);
			pNavigator->SetVisibility(naviInfo.NaviBaseInfo.bVisible);
			//
			pNavigator->SetNavRange(mapRect);

			// save to scene lits
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pNavigator->GetMaterial());

			//
			for (int iPointer = 0; iPointer < naviInfo.PointerCount; iPointer++)
			{
				std::string strPointerImg = AddPathtoTextureFile(naviInfo.PointerList[iPointer].PointerImg);
				CNavPointer* pPointer = (CNavPointer*)GUIMngr.CreateNaviPointer(naviInfo.PointerList[iPointer].sName, strPointerImg.c_str());
				if (NULL != pPointer)
				{					
 					DWORD dwPointedHandle = 0;
					if (0 == naviInfo.PointerList[iPointer].NavType &&
						naviInfo.PointerList[iPointer].PointedIndex >= 0 &&
						naviInfo.PointerList[iPointer].PointedIndex < m_vpCamera.size())	 // camera
					{
						dwPointedHandle = (DWORD)m_vpCamera[naviInfo.PointerList[iPointer].PointedIndex];
					}
					else if (1 == naviInfo.PointerList[iPointer].NavType &&
						naviInfo.PointerList[iPointer].PointedIndex >= 0 &&
						naviInfo.PointerList[iPointer].PointedIndex < m_vModel.size())		// 模型
					{
						dwPointedHandle = (DWORD)m_vModel[naviInfo.PointerList[iPointer].PointedIndex];
					}
					pPointer->SetPointerHandle(dwPointedHandle);
					pPointer->SetNavType(naviInfo.PointerList[iPointer].NavType);
					pPointer->SetVisibility(naviInfo.PointerList[iPointer].bVisible);
					pPointer->SetOpacity(naviInfo.PointerList[iPointer].Opacity);
					pPointer->SetSize(naviInfo.PointerList[iPointer].width, naviInfo.PointerList[iPointer].height);
					pPointer->SetPosition(rect.Left + naviInfo.PointerList[iPointer].left, rect.Top + naviInfo.PointerList[iPointer].top);

					pNavigator->AddNavPointer(pPointer);

					// save to scene lits
					m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pPointer->GetMaterial());
				}
			}
		}
			
		// 释放内存
		if (NULL != naviInfo.PointerList)
		{
			delete []naviInfo.PointerList;
			naviInfo.PointerList = NULL;
		}
 	}

	return ERR_COMMON_OK;
}

//Read the button data
DWORD CGetV3DImpV2::ReadButtonData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pButtonBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}
 
 	char* Offset = m_pButtonBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// 读取button的数量
 	int iBtnCount = 0; 
 	CPY_FROM_STREAM(iBtnCount, Offset);
 
	// 读取button属性并创建button对象
 	for (int iIndex = 0; iIndex < iBtnCount; iIndex++)
 	{
 		V3D_ButtonInfo BtnInfo;
 		CPY_FROM_STREAM(BtnInfo, Offset); 

 		// 创建button
		RECT rcBtn;
		std::string strUpImage;
		std::string strOverImage;
		std::string strDownImage;
		memset(&rcBtn, 0, sizeof(RECT));
		if (strlen(BtnInfo.UpImageName) > 0)
			strUpImage = AddPathtoTextureFile(BtnInfo.UpImageName);
		if (strlen(BtnInfo.OverImageName) > 0)
			strOverImage = AddPathtoTextureFile(BtnInfo.OverImageName);
		if (strlen(BtnInfo.DownImageName) > 0)
			strDownImage = AddPathtoTextureFile(BtnInfo.DownImageName);
		CButton* pButton = (CButton*)GUIMngr.CreateButton(BtnInfo.Name, rcBtn, strUpImage.c_str(), strOverImage.c_str(), strDownImage.c_str());
		if (NULL != pButton)
		{	
			// 视口大小
			int iViewWidth = 0;
			int iViewHight = 0;
			GUIMngr.GetViewportSize(iViewWidth, iViewHight);

 			float fLeft = 0.0f;
			float fTop = 0.0f;
			float fWidth = 0.0f;
			float fHeight = 0.0f; 

			// 转换成绝对坐标
 			if (BtnInfo.bRelativePos)  
 			{
 				fLeft = iViewWidth * BtnInfo.Left;
 				fTop  = iViewHight * BtnInfo.Top;
 			}
 			else
 			{
 				fLeft = BtnInfo.Left;
 				fTop  = BtnInfo.Top;
 			}
	 
			// 转换成绝对坐标
 			if (BtnInfo.bRelativeSize)  			
			{
 				fWidth  = iViewWidth * BtnInfo.Width;
 				fHeight = iViewHight * BtnInfo.Height;
 			}
 			else
 			{
 				fWidth  = BtnInfo.Width;
 				fHeight = BtnInfo.Height;
 			}

			pButton->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);   // 先设置为左上角便于设置位置
			pButton->SetUsedRelativePos(BtnInfo.bRelativePos);
			pButton->SetUsedRelativeSize(BtnInfo.bRelativeSize);
			pButton->SetSize(fWidth, fHeight);
			pButton->SetOffsetOrigin(VGSOFFSETORIGIN(BtnInfo.Origin));  // 设置offset origin
			pButton->SetPosition(fLeft, fTop);
			pButton->SetOpacity(BtnInfo.Opacity);
			pButton->SetVisibility(BtnInfo.bVisible);
			pButton->SetJsCallbackFunc(BtnInfo.CallbackFunName);

			// save to scene lits
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pButton->GetMaterial(0));
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pButton->GetMaterial(1));
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pButton->GetMaterial(2));
		} 		
 	}
	
	return ERR_COMMON_OK;
}

// 背景图
DWORD CGetV3DImpV2::ReadBackdropData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pBackdropBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}

 	char* Offset = m_pBackdropBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// 读取背景图片个数
 	int iBkgndCount = 0; 
 	CPY_FROM_STREAM(iBkgndCount, Offset);

	// 读取背景图片属性并创建背景图片对象
 	for (int iIndex = 0; iIndex < iBkgndCount; iIndex++)
 	{
 		V3D_BackdropInfo bkgndInfo;
 		CPY_FROM_STREAM(bkgndInfo, Offset); 
		//
		std::string strImage;
		if (strlen(bkgndInfo.ImgFile) > 0)
			strImage = AddPathtoTextureFile(bkgndInfo.ImgFile);
		//
		BackGround* pBkgrnd = GUIMngr.CreateBackGroudLayer(strImage.c_str());
		if (NULL != pBkgrnd)
		{
			pBkgrnd->SetVisibility(bkgndInfo.bVisible);
			pBkgrnd->SetRect(bkgndInfo.Left, bkgndInfo.Top, bkgndInfo.Width, bkgndInfo.Height);

			// save to scene lits
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pBkgrnd->GetMaterial());
		}
	}
 
	return ERR_COMMON_OK;
}

//Read AudioData
DWORD CGetV3DImpV2::ReadAudioData()
{
	return ERR_COMMON_OK;
}

//Read FlashData
DWORD CGetV3DImpV2::ReadFlashData()
{
	return ERR_COMMON_OK;
}

// 动画
DWORD CGetV3DImpV2::ReadAnimationData()
{
	return ERR_COMMON_OK;
}

//Read privacy data
DWORD CGetV3DImpV2::ReadPrivacyData()
{
	return ERR_COMMON_OK;
}

// 控制
DWORD CGetV3DImpV2::ReadControlData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pVGSSceneMgr || !m_pControlBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}	
 
 	char* Offset = m_pControlBuf; 
		
 	// 读取总的控制器数目
 	DWORD TotalCount = 0;
 	CPY_FROM_STREAM(TotalCount, Offset)	
 	if (TotalCount == 0)
 	{
 		return ERR_COMMON_OK;
 	}
 
	// walker数目
 	DWORD WalkerCount = 0;                        
 	CPY_FROM_STREAM(WalkerCount, Offset) 	
	// 读取walker数目
	for (UINT i = 0; i < WalkerCount; ++i)
 	{
 		WalkerInfo Info;
 		CPY_FROM_STREAM(Info, Offset)
 		m_pVGSSceneMgr->CreateWalker(Info);
 	}
 
	// flyer数目
 	DWORD FlyerCount = 0;
	// 读取flyer数目
 	CPY_FROM_STREAM(FlyerCount, Offset)
 	for (UINT i = 0; i < FlyerCount; ++i)
 	{
 		FlyerInfo Info;
 		CPY_FROM_STREAM(Info, Offset)
 		m_pVGSSceneMgr->CreateFlyer(Info);
 	}
 
	// editor数目
 	DWORD EditorCount = 0;
	// 读取editor数目
 	CPY_FROM_STREAM(EditorCount, Offset)
 	for (UINT i = 0; i < EditorCount; ++i)
 	{
 		if (m_V3DFileVersion < 50004)
 		{
 			EditorInfo Info;
 			EditorInfo_Old Info_Old;
 			CPY_FROM_STREAM(Info_Old, Offset)
 			memcpy(&Info, &Info_Old, sizeof(Info_Old));
 			Info.LookAtType = 0;
 			Info.LookAtModelIndex = 0xffffffff;
 			m_pVGSSceneMgr->CreateEditor(Info);
 		}
 		else
 		{
 			EditorInfo Info;
 			CPY_FROM_STREAM(Info, Offset)
			if (Info.LookAtModelIndex >= 0 && Info.LookAtModelIndex < m_vModel.size())
 			{
 				//Info.LookAtModelIndex = m_vModel[Info.LookAtModelIndex];  // 计算为真正的模型句柄
 			}
 			m_pVGSSceneMgr->CreateEditor(Info);
 		}
 	}
 
 	BYTE type;
 	UINT Index;
 	CPY_FROM_STREAM(type, Offset) 
 	CPY_FROM_STREAM(Index, Offset) 
 
 	m_pVGSSceneMgr->SetCurrentController(NULL);
 
 	if ((CONTROLTYPE)type == CT_EDITOR)
 	{
 		if (Index >= 0 && Index < m_pVGSSceneMgr->GetEditorCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetEditor(Index));
 			((CEditor*)(m_pVGSSceneMgr->GetEditor(Index)))->SetCamera(m_pVGSSceneMgr->GetEditor(Index)->GetCamera()); // 重设相机，让相机朝向目标点
 		}
 	}
 	else if ((CONTROLTYPE)type == CT_WALKER)
 	{
 		if (Index >= 0 && Index < m_pVGSSceneMgr->GetWalkerCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetWalker(Index));
 		}
 	}
 	else if ((CONTROLTYPE)type == CT_FLYER)
 	{
 		if (Index >= 0 && Index < m_pVGSSceneMgr->GetFlyerCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetFlyer(Index));
 		}
 	}
	
	return ERR_COMMON_OK;
}

//递归创建场景
void CGetV3DImpV2::ConstructSceneGraph(CSceneNode* pNode, char* offset)
{
	CSceneNode* pChildNode = NULL;
	bool	bHasSkeleton = false;
	VGS_Node node;
	string RealName;

	//读入node的类型
	CPY_FROM_STREAM(node.type, offset);
	
	//读入node的编号
	CPY_FROM_STREAM(node.index, offset);

	//读入node的名称
	CPY_FROM_STREAM(node.strLen, offset);
	node.name.reserve(node.strLen);
	memcpy((void*)(node.name.c_str()), offset, node.strLen);
	offset += node.strLen;
	// m_Offset += node.strLen;

	//读入并设置缩放\转角\位置
	VECTOR3 pos;
	CPY_FROM_STREAM(pos.x, offset);
	CPY_FROM_STREAM(pos.y, offset);
	CPY_FROM_STREAM(pos.z, offset);
	pos.z *= -1;

	D3DXQUATERNION quat;
	CPY_FROM_STREAM(quat.x, offset);
	CPY_FROM_STREAM(quat.y, offset);
	CPY_FROM_STREAM(quat.z, offset);
	CPY_FROM_STREAM(quat.w, offset);	//
	quat.x = -quat.x;
	quat.y = -quat.y;

	VECTOR3 scale;
	CPY_FROM_STREAM(scale.x, offset);
	CPY_FROM_STREAM(scale.y, offset);
	CPY_FROM_STREAM(scale.z, offset);

	std::vector<CNodeTransform> vNodeTransform;

	float animationLen = 0;
	CPY_FROM_STREAM(animationLen, offset);
	if (animationLen > 0)
	{
		int keyFrameCnt;
		CPY_FROM_STREAM(keyFrameCnt, offset);
		vNodeTransform.reserve(keyFrameCnt);
		for (INT kfrIndex = 0; kfrIndex < keyFrameCnt; kfrIndex++)
		{
			NodeTransform transform;
			CPY_FROM_STREAM(transform, offset);
			//
			CNodeTransform nodeTrans;
			nodeTrans.position = transform.position;
			nodeTrans.position.z *= -1;
			nodeTrans.quat = transform.quat;
			nodeTrans.quat.x *= -1;
			nodeTrans.quat.y *= -1;
			nodeTrans.scale = transform.scale;
			if (nodeTrans.scale.x <= 0.001)
			{
				nodeTrans.scale.x = 1;
			}
			if (nodeTrans.scale.y <= 0.001)
			{
				nodeTrans.scale.y = 1;
			}
			if (nodeTrans.scale.z <= 0.001)
			{
				nodeTrans.scale.z = 1;
			}
			nodeTrans.time = transform.timeElapsed;
			vNodeTransform.push_back(nodeTrans);
		}
	}

	//Node的变换, 相机的变换不在这里处理---------------------------------------------------------
	//位置//转化坐标系
	if (node.type == SCENEROOT)
	{		
		pChildNode = pNode;
		pChildNode->setScale(&D3DXVECTOR3(scale.x, scale.y, scale.z), VTS_WORLD);
		pChildNode->setRotationByQuat(&quat, VTS_WORLD);
		pChildNode->setPosition(&D3DXVECTOR3(pos.x, pos.y, pos.z), VTS_WORLD);
	}
	else
	{
		pChildNode = pNode->CreateChild();
		if (pChildNode)
		{
			//缩放//转化坐标系
			pChildNode->setScale(&D3DXVECTOR3(scale.x, scale.y, scale.z), VTS_PARENT);
			//pChildNode->updateTransform();
			//旋转//转化坐标系
			pChildNode->setRotationByQuat(&quat, VTS_PARENT);
			//pChildNode->updateTransform();
			//位置
			pChildNode->setPosition(&D3DXVECTOR3(pos.x, pos.y, pos.z), VTS_PARENT);
			//pChildNode->updateTransform();
			
			if (node.type == CAMERANODE)
			{
				if (node.index >= 0 && node.index < (int)m_vpCamera.size())
				{				
					CCamera *pCamera = m_vpCamera[node.index];
					if (pCamera)
					{
						D3DXVECTOR3 v3Pos = pCamera->GetPosition(VTS_PARENT);
						D3DXVECTOR3 v3At  = pCamera->GetLookat(VTS_PARENT);
						pChildNode->attachObject(pCamera);
						pCamera->m_pNode = pChildNode;
						pCamera->SetPosition(&v3Pos, VTS_PARENT);
						//
						if (animationLen > 0)
						{
							CreateCameraNodeAnimation(pChildNode, pCamera, vNodeTransform);
						}
					}
				}
			}
			else if (node.type == LIGHTNODE)
			{			
				if (node.index >= 0 && node.index < (int)m_vpLight.size())
				{
					CLight *pLight = m_vpLight[node.index];
					if (pLight)
					{
						pChildNode->attachObject(m_vpLight[node.index]);
						m_vpLight[node.index]->m_pNode = pChildNode;
						//
						if (animationLen > 0)
						{
							CreateLightNodeAnimation(pChildNode, pLight, vNodeTransform);
						}
					}
				}
			}
			else if (node.type == MESHNODE)
			{			
				if (node.index >= 0 && node.index < (int)m_vModel.size())
				{
					CModel *pModel = m_vModel[node.index];
					if (pModel)
					{
						pChildNode->attachObject(pModel);
						pModel->m_pNode = pChildNode;
						pModel->UpdateFromNode();
						//			
						if (animationLen > 0)
						{			
							m_pRoot->GetAnimManager().CreateModelAnimation(pChildNode, vNodeTransform);
						}
					}				
				}
			} // else if
		} // if pChildNode
			
	}  // node type

	m_Offset = offset - m_pSceneGraphBuf; // 重定位offset

	//读入子节点数量
	int childNodeCnt = 0;
	CPY_FROM_STREAM(childNodeCnt, offset);
	for (int i = 0; i< childNodeCnt; i++)
	{
		ConstructSceneGraph(pChildNode, m_pSceneGraphBuf + m_Offset);
	}
}

//创建节点的动画
void CGetV3DImpV2::ReadAndCreateNodeAnimation(CSceneNode* pNode, float animationLength, char** offset)
{
	//读入动画数据-------------------------------------------------
	//创建关键帧动画
	// Ogre::String strAnimName = "Anim_" + pNode->getName();
// 	Ogre::String strAnimName = _GetNodeAnimationName(pNode);  // 获取节点动画名称
// 	Animation* pAnim = m_pVGSSceneMgr->mSceneMgr->createAnimation(strAnimName, animationLength);
// 
// 	//读入关键帧动画的关键帧数量
// 	int keyframeCnt;
// 	CPY_FROM_STREAM(keyframeCnt, *offset);
// 
// 	//设置过渡方式
// 	pAnim->setInterpolationMode(Ogre::Animation::InterpolationMode::IM_LINEAR);
// 	
// 	//定义一段动画轨迹
// 	NodeAnimationTrack* pAnimTrack = pAnim->createNodeTrack(0, pNode);
// 	//逐帧读入数据
// 	for (int kfrIndex = 0; kfrIndex< keyframeCnt; kfrIndex++)
// 	{
// 		NodeTransform nodeTransform; 
// 		CPY_FROM_STREAM(nodeTransform, *offset);
// 		
// 		//当前帧对应的Transform对象
// 		TransformKeyFrame* pKfr = pAnimTrack->createNodeKeyFrame(nodeTransform.time);
// 
// 		//位置//坐标系转换
// 		Vector3 pos;
// 		pos.x = nodeTransform.position.x;
// 		pos.y = nodeTransform.position.y;
// 		pos.z = nodeTransform.position.z;
// 		pKfr->setTranslate(pos);
// 
// 		//转角//坐标系转换
// 		Ogre::Quaternion quat;
// 		quat.x = nodeTransform.quat.x;
// 		quat.y = nodeTransform.quat.y;
// 		quat.z = nodeTransform.quat.z;
// 		quat.w = nodeTransform.quat.w;
// 		pKfr->setRotation(quat);
// 
// 		//缩放
// 		Vector3 scale;
// 		scale.x = nodeTransform.scale.x;
// 		scale.y = nodeTransform.scale.y;
// 		scale.z = nodeTransform.scale.z;
// 		pKfr->setScale(scale);
// 	}
// 	
// 	//优化
// 	pAnimTrack->optimise();
// 
// 	//创建动画状态对象
// 	AnimationState* pAnimState = m_pVGSSceneMgr->mSceneMgr->createAnimationState(strAnimName);
// 	pAnimState->setEnabled(true);
// 	pAnimState->setLoop(true);
// 
// 	// 增加一个节点动画
// 	m_pVGSSceneMgr->GetAnimationManager()->AddNodeKeyfrmAnimation(pAnimState);
}


// 场景图
DWORD CGetV3DImpV2::ReadSceneGraph()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pSceneGraphBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}
		
	//递归读入场景图结构，并创建场景
	m_Offset = 0;
	CSceneNode* pRootNode = m_pVGSSceneMgr->GetRootSceneNode();
	ConstructSceneGraph(pRootNode, m_pSceneGraphBuf);	// 读取SceneGraph信息// 字节流中数据开始的位置

	// 删除字节流
	DELETE_ARRAYPTR(m_pSceneGraphBuf);  

	return ERR_COMMON_OK;
}

// 得到texture数据在文件中的范围
void CGetV3DImpV2::GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len)
{
	
	// 把所有其他位置信息从小到大排序，用texturePtr后面的位置减去texturePtr就是texure的长度
// 	std::vector<DWORD> v_ptrSortVector;
// 	
//     v_ptrSortVector.push_back(table.SceneproperyPtr);
// 	v_ptrSortVector.push_back(table.mtrlDataPtr);
// 	v_ptrSortVector.push_back(table.texturePtr);
// 	v_ptrSortVector.push_back(table.cameraDataPtr);
// 	v_ptrSortVector.push_back(table.lightDataPtr);
// 	v_ptrSortVector.push_back(table.skeletonDataPtr);
// 	v_ptrSortVector.push_back(table.meshDataPtr);
// 	v_ptrSortVector.push_back(table.shapeDataPtr);
// 	v_ptrSortVector.push_back(table.sceneGraphDataPtr);
// 	v_ptrSortVector.push_back(table.TextDataPtr);
// 	v_ptrSortVector.push_back(table.AnimationPtr);
// 	v_ptrSortVector.push_back(table.OverlayPtr);
// 	v_ptrSortVector.push_back(table.ButtonPtr);
// 	v_ptrSortVector.push_back(table.NavigatotPtr);
// 	v_ptrSortVector.push_back(table.AudioPtr);
// 	v_ptrSortVector.push_back(table.ControlPtr);
// 	v_ptrSortVector.push_back(table.FlashPtr);
// 	v_ptrSortVector.push_back(table.PrivacyDataPtr);
// 
// 	std::sort(v_ptrSortVector.begin(), v_ptrSortVector.end());
//     
// 	std::vector<DWORD>::iterator it = v_ptrSortVector.begin();
// 	for (; it != v_ptrSortVector.end(); ++it)
// 	{
// 		if ((*it) == table.texturePtr)
// 		{
// 			len = (*(it + 1)) - table.texturePtr;
// 		   break;
// 		}
// 	}
// 
// 	start = table.texturePtr;        // texture开始位置
}

// 给指定的纯文件名添加路径
std::string CGetV3DImpV2::AddPathtoTextureFile(const std::string &sPureFileName)
{
	// 检查文件是否在指定的搜索路径下或当前资源路径下
// 	Ogre::String str = "";
// 	if (_IsFileNameInResourcePath(Ogre::String(sPureFileName), str))
// 	{
// 		return str;
// 	}
	std::string str = m_sDefaultTexturePath;
	str.append(sPureFileName);
	return str;
}

// 更新法线贴图中的灯光句柄
VOID CGetV3DImpV2::UpdateBumpNormalmapLight()
{
// 	UINT count = m_pVGSSceneMgr->GetBumpmapCount();
// 	for (UINT i = 0; i < count; ++i)
// 	{
// 		VGSBumpmapInfo info;
// 		m_pVGSSceneMgr->GetBumpmapParam(i, info);
// 		if (info.m_Type == VBT_NORMARLMAP)
// 		{
// 			INT index = (INT)info.m_BumpNormalParam.LightHandle;
// 			if (index >= 0 && index < m_vpLight.size())
// 			{
// 				info.m_BumpNormalParam.LightHandle = (DWORD)m_vpLight[index];
// 			}
// 			else
// 			{
// 				info.m_BumpNormalParam.LightHandle = 0;
// 			}
// 		}
// 	}
}

// 处理打开完成后的一些工作
VOID CGetV3DImpV2::PostOpen()
{
	// 更新法线贴图中的灯光句柄
	// UpdateBumpNormalmapLight();
}