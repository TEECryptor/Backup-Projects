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


#include "GetV3D.h"
#include "VGSErrCode.h"

#include "vgsSupport.h"

#include "VGSCommonDef.h"
#include "AnimationManager.h"

#include"CEditor.h"

using namespace Ogre;

#define RETURN(RE)                       {if (RE != ERR_COMMON_OK)return RE;} 

// 删除数组
#define DELETE_ARRAYPTR(Arrayptr)        {if (Arrayptr) {delete [] Arrayptr; Arrayptr = NULL;}}                                

// 从Offset位置拷贝几个字节
#define CPY_FROM_STREAM(value, Offset)   {memcpy(&value, Offset, sizeof(value)); Offset += sizeof(value);  m_Offset += sizeof(value);} 

// 从offset位置拷贝一个vector3数据结构
#define CPY_VECTOR3_FROM_STRRAM(V3, Offset)\
{\
	CPY_FROM_STREAM((V3.x), Offset)\
	CPY_FROM_STREAM((V3.y), Offset)\
	CPY_FROM_STREAM((V3.z), Offset)\
}

// 从文件中读一个Vector3
#define READ_VECTOR3(V3, pFile) {fread(&(V3.x), sizeof(V3.x), 1, pFile); fread(&(V3.y), sizeof(V3.y), 1, pFile); fread(&(V3.z), sizeof(V3.z), 1, pFile);}

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

CGetV3D::CGetV3D()
{
	m_pVGSSceneMgr = NULL;
	m_pFile = NULL;

	m_sV3DPath = "";
	sDefaultTexturePath = "";

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

CGetV3D::~CGetV3D()
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

void CGetV3D::Release()
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
void CGetV3D::SetCurrentDirctory(const char *sFileName)
{
	int maxlen = strlen(sFileName);
	char *pPath = new char[maxlen];
	ZeroMemory(pPath, maxlen);
	GetPureFilePath(sFileName, pPath);
	m_sV3DPath = String(pPath);
	sDefaultTexturePath = m_sV3DPath + "resource\\texture\\";
}

// 获取V3D文件的Table目录
bool CGetV3D::GetV3DTable(const CHAR* strFileName, V3D_Table &Table)
{
	// 创建文件
	if (NULL == (m_pFile = fopen(strFileName, "rb")) )
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

DWORD CGetV3D::OpenV3DFile(CVGS_SceneManager* pVGSSceneMgr, const CHAR* strFileName)
{
	try
	{
		if (pVGSSceneMgr == NULL)
		{
			// ::MessageBox(NULL, LPCSTR("找不到场景"), LPCSTR("错误"), MB_OK | MB_ICONERROR);
			return ERR_SCENE_NOSCENE; // 无法获取场景！
		}
		m_pVGSSceneMgr = (CVGS_SceneManager*)pVGSSceneMgr;

		// 创建文件
		if (NULL == (m_pFile = fopen(strFileName, "rb")) )
		{
			// ::MessageBox(NULL, LPCSTR("访问文件失败"), LPCSTR("错误"), MB_OK | MB_ICONERROR);
			return ERR_FILE_OPENFAIL;
		}
		
		SetCurrentDirctory(strFileName);  // 得到当前的V3D目录，便于加载贴图等资源

		// m_vCameraDetail.clear();
		m_vpCamera.clear();
		m_vpLight.clear();

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

		if (m_pVGSSceneMgr->ogreResGroupName.empty())
		{
			m_pVGSSceneMgr->CreateResGroupName();

			//增加系统提供的资源组及资源路径，用于特效的渲染
			ResourceGroupManager::getSingleton().createResourceGroup(m_pVGSSceneMgr->ogreResGroupName);
			ResourceGroupManager::getSingleton().addResourceLocation(m_sV3DPath + "Resource/materials", "FileSystem");
			ResourceGroupManager::getSingleton().addResourceLocation(m_sV3DPath + "Resource/texture/", "FileSystem");
			ResourceGroupManager::getSingleton().addResourceLocation(m_sV3DPath + "Resource/scripts", "FileSystem");
			ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		}

		/////////////// 加载内容 /////////////////////

		//Read the  camera data
		result = ReadCameraData();
		RETURN(result);

		//Read the texture data
		result = ReadTextureData();
		RETURN(result);

		//Read the Material data
		result = ReadMaterialData();
		RETURN(result);

		//Read the light data
		result = ReadLightData();
		RETURN(result);

		// 读取模型资源数据，注意要先读mesh，再读entity
		result = ReadMeshData();
		RETURN(result);

		// Read the Model data
		result = ReadEnityData();
		RETURN(result);

		//Read the Text data
		result = ReadTextData();
		RETURN(result);

		//Read the overlay data
		result = ReadOverlayData();
		RETURN(result);

		//Read the navigater data
		result = ReadNavigaterData();
		RETURN(result);

		//Read the button data
		result = ReadButtonData();
		RETURN(result);

		//Read the backdrop
		result = ReadBackdropData();
		RETURN(result);

		//Read AudioData
		result = ReadAudioData();
		RETURN(result);

		//Read FlashData
		result = ReadFlashData();
		RETURN(result);

		// 动画
		result = ReadAnimationData();
		RETURN(result);

		//Read privacy data
		result = ReadPrivacyData();
		RETURN(result);

		// 控制
		result = ReadControlData();
		RETURN(result);

		result = ReadSceneGraph();
		RETURN(result);

		PostOpen();  // 处理打开完成后的一些工作
 
		Release(); // 释放buf内存

		return ERR_COMMON_OK;
	}
	catch (Ogre::Exception &e)
	{
		//fclose(m_pFile);
		return ERR_FILE_OPENFAIL;
	}
}

DWORD CGetV3D::ReadHeader()
{
	if (!m_pFile)
	{
		// throw Exception::exception("读文件头发生错误(01)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}

	fseek(m_pFile, 0, SEEK_SET); // 定位到文件开头
	V3D_Header V3DHeader;
	fread(&V3DHeader, sizeof(V3DHeader), 1, m_pFile);
	if (0 != strcmp(String(V3DHeader.corp).c_str(), SUNTOWARD_NAME))
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


DWORD CGetV3D::ReadFileTable()
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

DWORD CGetV3D::ReadWindowProp()
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
DWORD CGetV3D::ReadAllData()
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

	// 相机数据
	UINT CameDataSize = m_V3DTable.lightDataPtr - m_V3DTable.cameraDataPtr;
	m_pCameraBuf = new char[CameDataSize];
	// fseek(m_pFile, m_V3DTable.cameraDataPtr, SEEK_SET);	
	if (1 != fread(m_pCameraBuf, CameDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

	// 灯光
	UINT LightDataSize = m_V3DTable.texturePtr - m_V3DTable.lightDataPtr;
	m_pLightBuf = new char[LightDataSize];
	// fseek(m_pFile, m_V3DTable.lightDataPtr, SEEK_SET);
	if (1 != fread(m_pLightBuf, LightDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

	// 贴图纹理数据
	UINT TextureDataSize = m_V3DTable.mtrlDataPtr - m_V3DTable.texturePtr;
	m_pTextureBuf = new char[TextureDataSize];
	// fseek(m_pFile, m_V3DTable.texturePtr, SEEK_SET);
	if (1 != fread(m_pTextureBuf, TextureDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

	// 材质数据
	UINT MtrDataSize = m_V3DTable.entityDataPtr - m_V3DTable.mtrlDataPtr;
	m_pMaterailBuf = new char[MtrDataSize];
	// fseek(m_pFile, m_V3DTable.mtrlDataPtr, SEEK_SET);
	if (1 != fread(m_pMaterailBuf, MtrDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

	// entity
	UINT EntityDataSize = m_V3DTable.meshDataPtr - m_V3DTable.entityDataPtr;
	m_pEntityBuf = new char[EntityDataSize];
	// fseek(m_pFile, m_V3DTable.meshDataPtr, SEEK_SET);
	if (1 != fread(m_pEntityBuf, EntityDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

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
	// overlay
	UINT OverlayDataSize = m_V3DTable.NavigatotPtr - m_V3DTable.OverlayPtr;
	m_pOverlayBuf = new char[OverlayDataSize];
	// fseek(m_pFile, m_V3DTable.OverlayPtr, SEEK_SET);
	if (1 != fread(m_pOverlayBuf, OverlayDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	// 导航图
	UINT NaviSize = m_V3DTable.ButtonPtr - m_V3DTable.NavigatotPtr;
	m_pNaviBuf = new char[NaviSize];
	int ReadSize = fread(m_pNaviBuf, NaviSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	// 按钮
	UINT ButtonSize = m_V3DTable.BackdropPtr - m_V3DTable.ButtonPtr;
	m_pButtonBuf = new char[ButtonSize];
	ReadSize = fread(m_pButtonBuf, ButtonSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

	// 背景图
	UINT BackdropSize = m_V3DTable.AudioPtr - m_V3DTable.BackdropPtr;
	m_pBackdropBuf = new char[BackdropSize];
	ReadSize = fread(m_pBackdropBuf, BackdropSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

	// 声音
	UINT AudioSize = m_V3DTable.FlashPtr - m_V3DTable.AudioPtr;
	m_pAudioBuf = new char[AudioSize];
	ReadSize = fread(m_pAudioBuf, AudioSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	// flash
	UINT FlashDataSize = m_V3DTable.AnimationPtr - m_V3DTable.FlashPtr;
	m_pFlashBuf = new char[FlashDataSize];
	ReadSize = fread(m_pFlashBuf, FlashDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	// 动画
	UINT AnimationDataSize = m_V3DTable.ControlPtr - m_V3DTable.AnimationPtr;
	m_pAnimationBuf = new char[AnimationDataSize];
	ReadSize = fread(m_pAnimationBuf, AnimationDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}
	// 控制信息
	UINT ControlDataSize = m_V3DTable.sceneGraphDataPtr - m_V3DTable.ControlPtr;
	m_pControlBuf = new char[ControlDataSize];
	fseek(m_pFile, m_V3DTable.ControlPtr, SEEK_SET);
	ReadSize = fread(m_pControlBuf, ControlDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // 读取预订的字节数错误
	}

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

	return ERR_COMMON_OK;
}


DWORD CGetV3D::ReadTextureData()
{
	DELETE_ARRAYPTR(m_pTextureBuf);  // 删除字节流
	return ERR_COMMON_OK;
}

DWORD CGetV3D::ReadMaterialData()
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

	// 读取材质信息
	char* Offset = m_pMaterailBuf;          // 字节流中数据开始的位置
	DWORD MtrCount = 0;                     // 材质数目
	CPY_FROM_STREAM(MtrCount, Offset)       // 读取材质数目
	for (DWORD MtrIdx = 0; MtrIdx < MtrCount; ++MtrIdx)
	{
		DWORD len = 0;
		CPY_FROM_STREAM(len, Offset)        // Mtrl Name length
		MaterialPtr pMtrl;
		if (len > 0)
		{
			char *pName = new char[len + 1];
			ZeroMemory(pName, len + 1);
			memcpy(pName, Offset, len);         // Mtrl Name
			Offset += len;
			// 创建材质资源到指定资源组, Ogre会自动创建一个technique和一个pass
			pMtrl = MaterialManager::getSingleton().create(pName, m_pVGSSceneMgr->ogreResGroupName);
			delete [] pName;
			pName = NULL;
		}
		else
		{
			// 创建材质资源到指定资源组, Ogre会自动创建一个technique和一个pass
			// pMtrl = MaterialManager::getSingleton().create(pName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			MessageBox(NULL, LPCSTR("数据中没有为材质指定名称，可能会导致问题！"), LPCSTR("错误"), 0);
			continue;
		}

		bool  bRecShadow;
		DWORD TechCount;
		float USpeed, VSpeed;  // 动态UV速度
		CPY_FROM_STREAM(bRecShadow, Offset)   // Mtrl Receive Shadow
		CPY_FROM_STREAM(USpeed, Offset)   // Mtrl Receive Shadow
		CPY_FROM_STREAM(VSpeed, Offset)    // Mtrl Technique count

		pMtrl->setReceiveShadows(bRecShadow); // set Mtrl receive shadow
		
		CPY_FROM_STREAM(TechCount, Offset)    // Mtrl Technique count

		// 由于创建材质时，会自动创建一个technique和pass，所以，如果仅有一个technique时，就不用创建了
		if (TechCount == 1)
		{
			Technique * pTech = pMtrl->getTechnique(0);		

			UINT namelen;
			CPY_FROM_STREAM(namelen, Offset)               // length of tech name
			if (namelen > 0)
			{
				char *pName = new char[namelen + 1];
				ZeroMemory(pName, namelen + 1);
				memcpy(pName, Offset, namelen);                 // tech name
				Offset += namelen;
				pTech->setName(pName);                           // 设置技术的名字				
				_AddTextureLayerName(pName);
				delete [] pName;
				pName = NULL;
			}

			UINT passCount;
			CPY_FROM_STREAM(passCount, Offset)              // pass count of this technique
			
			if (passCount == 1)  // 如果只有一个pass， 直接赋值即可，不需要新创建了
			{
				//设置材质的参数
				Pass* pass = pTech->getPass(0);

				UINT namelen;
				CPY_FROM_STREAM(namelen, Offset)                // length of pass name
				if (namelen > 0)
				{
					char *pName = new char[namelen + 1];
					ZeroMemory(pName, namelen + 1);
					memcpy(pName, Offset, namelen);                 // pass name
					Offset += namelen;
					pass->setName(pName);      // set pass name
					_AddTextureLayerName(pName);
					delete [] pName;
					pName = NULL;
				}

				ReadPass(pass, &Offset);         // 读取pass数据
			}
			
			// 如果有多个pass，先创建pass后赋值
			for (UINT iPass = 1; iPass < passCount; ++iPass)
			{
				Pass *pass = pTech->createPass();

				UINT namelen;
				CPY_FROM_STREAM(namelen, Offset)   // pass 名字长度
				if (namelen > 0)
				{
					char *pName = new char[namelen] + 1;
					ZeroMemory(pName, namelen + 1);
					memcpy(pName, Offset, namelen);               // pass 名字 
					Offset += namelen;
					pass->setName(pName);
					_AddTextureLayerName(pName);
					delete [] pName;
					pName = NULL;
				}

				ReadPass(pass, &Offset);         // 读取pass数据

			} // 多个pass	
		} // end of TechCount == 1

		// 加载材质
		pMtrl->load();

		// _SetMaterialUVSpeed(pMtrl.get(), USpeed, VSpeed);  // 设置动态UV速度

	} // Material 

	DELETE_ARRAYPTR(m_pMaterailBuf);  // 删除字节流

	return ERR_COMMON_OK;
}	
// 从字节流中读取Pass数据
void CGetV3D::ReadPass(Pass *pPass, char** OffsetPtr)
{
	char *Offset = *OffsetPtr;	

	//bool bTransparent;
	//UINT Amb_RGBA, Dif_RGBA, Spe_RGBA, sel_RGBA;
	//float shininess, bDepthCheck, bLighting, bDepthWrite;
	//BYTE ShadingMode, sBlendFactor, dBlendFactor, CullMode, PolygonMode, RejFunc, AlphaValue;
	Ogre::ColourValue thisColourValue;	

	V3D_PassDetail_Ex PassDetail;
	CPY_FROM_STREAM(PassDetail.basedetail, Offset)  // 一次性读取pass的参数
	if ((int)m_V3DFileVersion >= 50001)
	{
		CPY_FROM_STREAM(PassDetail.bUseDiffuse, Offset)  // 读取是否使用材质diffuse颜色
	}

	// 赋值到pass
	pPass->setShadingMode((Ogre::ShadeOptions)(PassDetail.basedetail.ShadingMode));
	thisColourValue.setAsRGBA(PassDetail.basedetail.Amb_RGBA);
	pPass->setAmbient(thisColourValue);
	thisColourValue.setAsRGBA(PassDetail.basedetail.Dif_RGBA);

	if (PassDetail.bUseDiffuse)  // 如果使用材质diffuse
	{
		pPass->setDiffuse(thisColourValue);
	}
	else
	{
		pPass->setDiffuse(1.0f, 1.0f, 1.0f, thisColourValue.a); // 使用白色
	}

	// fuxb 2008-10-30 ==>
	// thisColourValue.setAsRGBA(PassDetail.basedetail.Spe_RGBA);
	// pPass->setSpecular(ColourValue);
	thisColourValue.setAsRGBA(PassDetail.basedetail.Spe_RGBA);
	thisColourValue.r *= PassDetail.basedetail.Spe_Level / 100.0;
	thisColourValue.g *= PassDetail.basedetail.Spe_Level / 100.0;
	thisColourValue.b *= PassDetail.basedetail.Spe_Level / 100.0;
	thisColourValue.a *= PassDetail.basedetail.Spe_Level / 100.0;
	pPass->setSpecular(thisColourValue);
	// <==

	pPass->setShininess(Real(PassDetail.basedetail.shininess));
	thisColourValue.setAsRGBA(PassDetail.basedetail.sel_RGBA);
	pPass->setSelfIllumination(thisColourValue);
	pPass->setSceneBlending((const Ogre::SceneBlendFactor)(PassDetail.basedetail.sBlendFactor), (const Ogre::SceneBlendFactor)(PassDetail.basedetail.dBlendFactor));
	pPass->setCullingMode((Ogre::CullingMode)(PassDetail.basedetail.CullMode));
	pPass->setDepthCheckEnabled((PassDetail.basedetail.bDepthCheck));	
	pPass->setDepthWriteEnabled(PassDetail.basedetail.bDepthWrite);
	pPass->setLightingEnabled(PassDetail.basedetail.bLighting);
	pPass->setPolygonMode((Ogre::PolygonMode)(PassDetail.basedetail.PolygonMode));
	pPass->setAlphaRejectSettings((Ogre::CompareFunction)(PassDetail.basedetail.RejFunc), (PassDetail.basedetail.AlphaValue));

	// fuxb ==> 2008-08-31 加入到VGS场景管理器
	CVGSMaterialInfo VGSMatInfo;
	VGSMatInfo.sVGSName = pPass->getParent()->getParent()->getName();
	VGSMatInfo.SpecularLevel = PassDetail.basedetail.Spe_Level; // [0, 100]
	VGSMatInfo.Emissive = PassDetail.basedetail.selfIllumination; // [0, 100]
	// VGSMatInfo.USpeed = PassDetail.basedetail.USpeed;
	// VGSMatInfo.VSpeed = PassDetail.basedetail.VSpeed;
	VGSMatInfo.bUseDiffuse = PassDetail.bUseDiffuse;  // 2008-10-29, 是否使用材质的diffuse颜色
	VGSMatInfo.Diffuse.setAsRGBA(PassDetail.basedetail.Dif_RGBA);
	VGSMatInfo.Specular.setAsRGBA(PassDetail.basedetail.Spe_RGBA);
	m_pVGSSceneMgr->AddVGSMaterialInfo(VGSMatInfo);   // 保存VGSMaterial信息
	// <==

	*OffsetPtr = Offset;  // updata offsetptr to current positon

	// 读取贴图层
	if (m_V3DFileVersion < 50005)
	{
		ReadPassTexture_old(pPass, OffsetPtr);
	}
	else
	{
		ReadPassTexture(pPass, OffsetPtr);
	}

	// 设置混色
	if (pPass->getDiffuse().a < 1.0f || _HasOpactityTexture(pPass))
	{
		pPass->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);
	}
	else
	{
		pPass->setSceneBlending(Ogre::SceneBlendType::SBT_REPLACE);
	}	
}
// 从字节流中读取texture数据
void CGetV3D::ReadPassTexture(Pass *pPass, char** OffsetPtr)
{	
	char *Offset = *OffsetPtr;	 
	UINT UnitStateCount;
	memcpy(&UnitStateCount, Offset, sizeof(UnitStateCount));  // get texture count
	Offset += sizeof(UnitStateCount);
	for (UINT iUnit = 0; iUnit < UnitStateCount; ++iUnit) // pass用到的纹理信息
	{
		TextureUnitState *pUnit = pPass->createTextureUnitState();
		UINT namelen;
		CPY_FROM_STREAM(namelen, Offset)                // length of unit name
		if (namelen > 0)
		{
			char *pName = new char[namelen + 1];
			ZeroMemory(pName, namelen + 1);
			memcpy(pName, Offset, namelen);                 // unit name
			Offset += namelen;
			pUnit->setName(pName);                          // set unit name
			_AddTextureLayerName(pName);
			delete [] pName;	
			pName = NULL;
		}

		UINT CoordSet;
		BYTE minFilter, magFilter, mipFilter, mipMap ;
		CPY_FROM_STREAM(CoordSet, Offset)                // CoordSet 
		CPY_FROM_STREAM(minFilter, Offset)               // texture filter 
		CPY_FROM_STREAM(magFilter, Offset)               // texture filter  
		CPY_FROM_STREAM(mipFilter, Offset)               // texture filter  
		CPY_FROM_STREAM(mipMap, Offset)					 // texture mipMap  

		// fuxb 读取每个texture的UV Speed
		FLOAT USpeed, VSpeed;
		CPY_FROM_STREAM(USpeed, Offset) 
		CPY_FROM_STREAM(VSpeed, Offset) 

		int texType;
		CPY_FROM_STREAM(texType, Offset)               // texture 类型
		
		int duration = 0;				// 如果是动态贴图，记录播放周期

		VGS_REFLECT_TYPE reflectType;
		VGSBumpmapInfo bumpInfo;   // bump参数

		if (texType == VGS_TextureType::TLT_DYNAMIC)
		{
			CPY_FROM_STREAM(duration, Offset);       
		}
		else if (texType == VGS_TextureType::TLT_REFLECT)
		{			
			char type;
			CPY_FROM_STREAM(type, Offset)		// 反射方式，平面还是曲面
			reflectType = (VGS_REFLECT_TYPE)type;
		}
		else if (texType == VGS_TextureType::TLT_BUMPMAP)
		{
			CPY_FROM_STREAM(bumpInfo.m_Type, Offset);   // bump的类型
			bumpInfo.m_pTUS = pUnit;
			if (bumpInfo.m_Type == VBT_WATER)
			{
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.XScale, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.YScale, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.Radius, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.Speed, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.CenterX, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.CenterY, Offset);
			}
			else if (bumpInfo.m_Type == VBT_ENVIRONMENT)
			{
				CPY_FROM_STREAM(bumpInfo.m_BumpEnvParam.noise1, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpEnvParam.noise2, Offset);
			}
			else if (bumpInfo.m_Type == VBT_NORMARLMAP)
			{
				INT index;
				CPY_FROM_STREAM(index, Offset);
				bumpInfo.m_BumpNormalParam.LightHandle = (DWORD)index;
			} 
		}
		else if (texType == VGS_TextureType::TLT_CUBEMAP)
		{
			// ...
		}

		// 读入层的混色方式
		VGS_BlenderInfo ColorBlendInfo, AlphaBlendInfo;
		CPY_FROM_STREAM(ColorBlendInfo, Offset)
		CPY_FROM_STREAM(AlphaBlendInfo, Offset) 

		// texture使用的图片数目
		unsigned short FrmCount;
		CPY_FROM_STREAM(FrmCount, Offset)

		vector <Ogre::String> vsFrameTexture;  // texture用到的所有帧图像
		for (unsigned short iFrm = 0; iFrm < FrmCount; ++iFrm)
		{
			CPY_FROM_STREAM(namelen, Offset)                // length of texture name 

			if (namelen > 0)  
			{
				char* pName = new char[namelen + 1];
				ZeroMemory(pName, namelen + 1);
				memcpy(pName, Offset, namelen);                 // texture name, 不包含路径
				Offset += namelen;
				vsFrameTexture.push_back(String(pName));		// 暂时保存

				delete [] pName;
				pName = NULL;
			}
		} // for iFrm

		// 材质名称
		Ogre::String sMatName = pPass->getParent()->getParent()->getName();		

		// 如果是cubicmap，需要从pUnit的名字中解析出图片名称
		if (texType == VGS_TextureType::TLT_CUBEMAP) // cubic map
		{
			if (FrmCount == 6)
			{
				for (int idx = 0; idx < FrmCount; ++idx)
				{
					// 增加贴图和材质的映射
					m_pVGSSceneMgr->NotifyTextureUpdate();  // 通知场景中的texture列表发生了变化
					vsFrameTexture[idx] = AddPathtoTextureFile(vsFrameTexture[idx].c_str());  // 添加文件路径
				}
				_SetCubicTextureImages((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, vsFrameTexture);  // 创建cubicmap
			}
		}
		else  // 非cubic texture, 直接读取所有图片
		{
			// 设置图像文件
			int FrmCount = vsFrameTexture.size();
			for (int i = 0; i < FrmCount; ++i)
			{
				// 增加贴图和材质的映射
				m_pVGSSceneMgr->NotifyTextureUpdate();  // 通知场景中的texture列表发生了变化
				Ogre::String sTextureFile = AddPathtoTextureFile(vsFrameTexture[i].c_str());
				pUnit->addFrameTextureName(sTextureFile); // 纹理都放在texture目录下
			}

			// 设置texture
			if (texType == VGS_TextureType::TLT_DYNAMIC)
			{
				SetDynamicTextureDuration((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, duration);  
			}
			else if (texType == VGS_TextureType::TLT_REFLECT)
			{
				if ((VGS_REFLECT_TYPE)reflectType != VGS_REFLECT_TYPE::VRT_INVALID)
				{
					_SetTextureAsReflect(pUnit, (VGS_REFLECT_TYPE)reflectType);
				}
			}
			else if (texType == VGS_TextureType::TLT_OPACITY)
			{
				SetTextureAsOpacity((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, TRUE); 
			}
			else if (texType == VGS_TextureType::TLT_BUMPMAP)
			{
				//if (bumpInfo.m_Type == VGS_BumpType::VBT_WATER)
				//{
				//	pUnit->setTextureNameAlias(g_BumpWaterNamePrefix + pUnit->getName());  // 设置别名以记录是bump
				//}
				//else if (bumpInfo.m_Type == VGS_BumpType::VBT_ENVIRONMENT)
				//{
				//	pUnit->setTextureNameAlias(g_BumpEnvNamePrefix + pUnit->getName());  // 设置别名以记录是bump
				//}
				//else if (bumpInfo.m_Type == VGS_BumpType::VBT_NORMARLMAP)
				//{
				//	pUnit->setTextureNameAlias(g_BumpNormalNamePrefix + pUnit->getName());  // 设置别名以记录是bump
				//}
				
				m_pVGSSceneMgr->UpdateBumpmapParam(pUnit, bumpInfo);
			}
		}

		if (CoordSet < 0 || CoordSet > 3)
		{
			CoordSet = 0;
		}

		pUnit->setTextureCoordSet(CoordSet);  // 设置texture使用的UV坐标编号
		pUnit->setNumMipmaps(mipMap);         // mipMap

		pUnit->setTextureFiltering((Ogre::FilterOptions)minFilter, (Ogre::FilterOptions)magFilter, (Ogre::FilterOptions)mipFilter); // 设置texture filter
			
		// 颜色混色方式
		pUnit->setColourOperationEx(_GetOgreBlendOperation(ColorBlendInfo.option), Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
									ColourValue::White, ColourValue::White,
									ColorBlendInfo.param.BlenderOP_alpha);
		pUnit->setColourOpMultipassFallback(SBF_DEST_COLOUR, SBF_ZERO);

		// alpha通道的混色方式
		pUnit->setAlphaOperation(_GetOgreBlendOperation(AlphaBlendInfo.option), Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
									1.0f, 1.0f,
									AlphaBlendInfo.param.BlenderOP_alpha);
		 
		_SetTextureUVSpeed(pUnit, USpeed, VSpeed);  // 设置UV Speed

	} // for iUnit

	*OffsetPtr = Offset;  // updata offsetptr to current positon
}
// 从字节流中读取texture数据, 50004及其之前版本
void CGetV3D::ReadPassTexture_old(Pass *pPass, char** OffsetPtr)
{
	char *Offset = *OffsetPtr;	 
	UINT UnitStateCount;
	memcpy(&UnitStateCount, Offset, sizeof(UnitStateCount));  // get texture count
	Offset += sizeof(UnitStateCount);
	for (UINT iUnit = 0; iUnit < UnitStateCount; ++iUnit) // pass用到的纹理信息
	{
		TextureUnitState *pUnit = pPass->createTextureUnitState();
		UINT namelen;
		CPY_FROM_STREAM(namelen, Offset)                // length of unit name
		if (namelen > 0)
		{
			char *pName = new char[namelen + 1];
			ZeroMemory(pName, namelen + 1);
			memcpy(pName, Offset, namelen);                 // unit name
			Offset += namelen;
			pUnit->setName(pName);                          // set unit name
			_AddTextureLayerName(pName);
			delete [] pName;	
			pName = NULL;
		}

		UINT CoordSet;
		BYTE minFilter, magFilter, mipFilter, mipMap ;
		CPY_FROM_STREAM(CoordSet, Offset)                // CoordSet 
		CPY_FROM_STREAM(minFilter, Offset)               // texture filter 
		CPY_FROM_STREAM(magFilter, Offset)               // texture filter  
		CPY_FROM_STREAM(mipFilter, Offset)               // texture filter  
		CPY_FROM_STREAM(mipMap, Offset)					 // texture mipMap  

		int texType;
		CPY_FROM_STREAM(texType, Offset)               // texture 类型
		
		int duration = 0;				// 如果是动态贴图，记录播放周期			
		bool bOpacity = false;                  // 记录是否是透明贴图
		char reflectType;               // 反射类型
		if (texType == V3D_TextureType_Old::VTT_DYNAMIC)
		{
			CPY_FROM_STREAM(duration, Offset);       
		}
		else if (texType == V3D_TextureType_Old::VTT_BASAL)
		{
			CPY_FROM_STREAM(reflectType, Offset)		// 是否被用作反射贴图
			CPY_FROM_STREAM(bOpacity, Offset)			// 是否被用作透明贴图	

			// 50001及以前的版本，保存bool值区别是否是反射贴图，新版本用char记录反射类型，-1表示不是反射贴图
			if (m_V3DFileVersion < 50002 && reflectType == 0)
			{
				reflectType = -1;  // 不是反射贴图
			}
		}
		else if (texType == V3D_TextureType_Old::VTT_BUMP)
		{
			// nothing
		}

		// 读入层的混色方式
		VGS_BlenderInfo ColorBlendInfo, AlphaBlendInfo;
		CPY_FROM_STREAM(ColorBlendInfo, Offset)
		CPY_FROM_STREAM(AlphaBlendInfo, Offset) 

		// texture使用的图片数目
		unsigned short FrmCount;
		CPY_FROM_STREAM(FrmCount, Offset)

		vector <Ogre::String> vsFrameTexture;  // texture用到的所有帧图像
		for (unsigned short iFrm = 0; iFrm < FrmCount; ++iFrm)
		{
			CPY_FROM_STREAM(namelen, Offset)                // length of texture name 

			if (namelen > 0)  
			{
				char* pName = new char[namelen + 1];
				ZeroMemory(pName, namelen + 1);
				memcpy(pName, Offset, namelen);                 // texture name, 不包含路径
				Offset += namelen;
				vsFrameTexture.push_back(String(pName));  // 暂时保存

				delete [] pName;
				pName = NULL;
			}
		} // for iFrm

		// 材质名称
		Ogre::String sMatName = pPass->getParent()->getParent()->getName();		

		// 如果是cubicmap，需要从pUnit的名字中解析出图片名称
		if (texType == V3D_TextureType_Old::VTT_CUBIC) // cubic map
		{
			// 低于50002版本的从texturename中解析文件名称
			if (m_V3DFileVersion < 50002)
			{
				// 解析字符串，获取文件列表
				Ogre::String sFilelist = pUnit->getName();
				_GetFileListFormString(sFilelist, vsFrameTexture);
				FrmCount = vsFrameTexture.size();
			} 
			
			if (FrmCount == 6)
			{
				for (int idx = 0; idx < FrmCount; ++idx)
				{
					// 增加贴图和材质的映射
					m_pVGSSceneMgr->NotifyTextureUpdate();  // 通知场景中的texture列表发生了变化
					vsFrameTexture[idx] = AddPathtoTextureFile(vsFrameTexture[idx].c_str());  // 添加文件路径
				}
				_SetCubicTextureImages((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, vsFrameTexture);  // 创建cubicmap
			}
		}
		else  // 非cubic texture, 直接读取所有图片
		{
			// 设置图像文件
			int FrmCount = vsFrameTexture.size();
			for (int i = 0; i < FrmCount; ++i)
			{
				// 增加贴图和材质的映射
				m_pVGSSceneMgr->NotifyTextureUpdate();  // 通知场景中的texture列表发生了变化
				Ogre::String sTextureFile = AddPathtoTextureFile(vsFrameTexture[i].c_str());
				pUnit->addFrameTextureName(sTextureFile); // 纹理都放在texture目录下

			}

			// 设置texture
			if (texType == V3D_TextureType_Old::VTT_DYNAMIC)
			{
				SetDynamicTextureDuration((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, duration);  
			}
			else if (texType == V3D_TextureType_Old::VTT_BASAL)
			{
				if ((VGS_REFLECT_TYPE)reflectType != VGS_REFLECT_TYPE::VRT_INVALID)
				{
					_SetTextureAsReflect(pUnit, (VGS_REFLECT_TYPE)reflectType);
				}
				if (bOpacity)
				{
					SetTextureAsOpacity((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, TRUE); 
				}
			}
			else if (texType == V3D_TextureType_Old::VTT_BUMP)
			{
				pUnit->setTextureNameAlias(g_BumpEnvNamePrefix + pUnit->getName());  // 设置别名以记录是bump
			}
		}

		if (CoordSet < 0 || CoordSet > 3)
		{
			CoordSet = 0;
		}

		pUnit->setTextureCoordSet(CoordSet);  // 设置texture使用的UV坐标编号
		pUnit->setNumMipmaps(mipMap);         // mipMap

		pUnit->setTextureFiltering((Ogre::FilterOptions)minFilter, (Ogre::FilterOptions)magFilter, (Ogre::FilterOptions)mipFilter); // 设置texture filter
			
		// 颜色混色方式
		pUnit->setColourOperationEx(_GetOgreBlendOperation(ColorBlendInfo.option), Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
									ColourValue::White, ColourValue::White,
									ColorBlendInfo.param.BlenderOP_alpha);
		pUnit->setColourOpMultipassFallback(SBF_DEST_COLOUR, SBF_ZERO);

		// alpha通道的混色方式
		pUnit->setAlphaOperation(_GetOgreBlendOperation(AlphaBlendInfo.option), Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
									1.0f, 1.0f,
									AlphaBlendInfo.param.BlenderOP_alpha);
		 
		
	} // for iUnit

	*OffsetPtr = Offset;  // updata offsetptr to current positon
}

// 读取模型数据
DWORD CGetV3D::ReadEnityData()
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
	
	// 读取每个mesh
	for (UINT idx = 0; idx < EntityCount; ++idx)
	{
		int namelen = 0;
		char sName[256];
		ZeroMemory(sName, 256);
		CPY_FROM_STREAM(namelen, Offset)      // entity名称
		
		memcpy(sName, Offset, namelen);       // 读取名称
		Offset += namelen;

		int meshIndex = -1;
		CPY_FROM_STREAM(meshIndex, Offset)      // mesh索引

		VGS_EntityInfo modelInfo;
		if (m_V3DFileVersion < 50003)  // 50001版本保存的V3D有问题，这里需要读取52个字节
		{
			char tmp[52];
			memcpy(tmp, Offset, sizeof(tmp));
			// m_Offset += sizeof(tmp);
			Offset += sizeof(tmp);
			
			// 设置Model属性
			modelInfo.isAutoNormalise = true;			
		}
		else
		{
			CPY_FROM_STREAM(modelInfo.isAutoNormalise, Offset);
			CPY_FROM_STREAM(modelInfo.isBillboard, Offset);
			CPY_FROM_STREAM(modelInfo.isClickable, Offset);
			CPY_FROM_STREAM(modelInfo.isCollision, Offset);
			CPY_FROM_STREAM(modelInfo.isGround, Offset);
			CPY_FROM_STREAM(modelInfo.upAxis, Offset);
			CPY_FROM_STREAM(modelInfo.dirctionAxis, Offset);
			CPY_FROM_STREAM(modelInfo.isReflection, Offset)      // 是否反射其他模型

			if (modelInfo.isReflection)
			{
				UINT reflectionCount = 0;
				CPY_FROM_STREAM(reflectionCount, Offset)      // 所反射的模型数目
				for (int iModel = 0; iModel < reflectionCount; ++iModel)
				{
					int ModelIndex;
					CPY_FROM_STREAM(ModelIndex, Offset)      // 所反射的模型数目
					modelInfo.vReflectionList.push_back(ModelIndex);
				}
			} // if isReflection
			CPY_FROM_STREAM(modelInfo.isRefraction, Offset)      // 是否折射其他模型
			if (modelInfo.isReflection)
			{
				UINT refractionCount = 0;
				CPY_FROM_STREAM(refractionCount, Offset)      // 所折射的模型数目
				for (int iModel = 0; iModel < refractionCount; ++iModel)
				{
					int ModelIndex;
					CPY_FROM_STREAM(ModelIndex, Offset)      // 所反射的模型数目
					modelInfo.vRefractionList.push_back(ModelIndex);
				}
			} // if isReflection
		
		} // m_V3DFileVersion

		if (meshIndex >= 0 && meshIndex < m_vMesh.size())
		{
			//创建实体
			Entity* pEntity = m_pVGSSceneMgr->mSceneMgr->createEntity(Ogre::String(sName), m_vMesh[meshIndex]->getName());
			// pEntity->setNormaliseNormals(true);
			pEntity->_initialise();

			//加入VGS场景管理器
			m_pVGSSceneMgr->AddEntityNameMap(Ogre::String(sName), Ogre::String(sName));
			m_pVGSSceneMgr->SetEntityInfo((DWORD)pEntity, modelInfo);                      // 保存

			m_vEntityList.push_back((DWORD)pEntity);   // 保存Entity列表，后面可能会使用
		}
	} // for

	DELETE_ARRAYPTR(m_pEntityBuf);  // 删除字节流

	// 将模型反射折射模型的索引，改为模型的句柄
	// 以后补上


	return ERR_COMMON_OK;
}

// 读取模型资源数据
DWORD CGetV3D::ReadMeshData()
{
	if (!m_pFile)
	{
		// throw Exception::exception("读文件发生错误(03)，无法读取文件");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pMeshBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	m_vMesh.clear();   // 清空Mesh列表

	// 读取mesh信息
	char* Offset = m_pMeshBuf;              // 字节流中数据开始的位置
	DWORD MeshCount;                        // mesh数目
	CPY_FROM_STREAM(MeshCount, Offset)      // 读取mesh数目
	
	// 读取每个mesh
	for (UINT iMesh = 0; iMesh < MeshCount; ++iMesh)
	{	
		UINT NameLen;
		CPY_FROM_STREAM(NameLen, Offset)

		MeshPtr pMesh;
		if (NameLen > 0)
		{
			char *sName = new char[NameLen + 1];
			ZeroMemory(sName, NameLen + 1);
			memcpy(sName, Offset, NameLen);
			Offset += NameLen;					

			//创建Mesh对象
			try
			{
				int idx = 0;
				Ogre::String sNewName = Ogre::String(sName);
				while (MeshManager::getSingleton().getByName(sNewName).get() != NULL)
				{					
					char tmp[10];
					sNewName = Ogre::String(sName) + Ogre::String(itoa(idx++, tmp, 10));
				}
				pMesh = MeshManager::getSingleton().createManual(sNewName, /*ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME*/m_pVGSSceneMgr->ogreResGroupName);
			}
			catch (Ogre::Exception &e)
			{
				// MessageBox(NULL, LPCSTR("有重名的对象，可能会导致问题！"), LPCSTR("错误"), 0);
				MessageBox(NULL, LPCSTR(e.getDescription().c_str()), LPCSTR("错误"), 0);
				return ERR_SCENE_CREATE;
			}
			DELETE_ARRAYPTR(sName)

			m_vMesh.push_back(pMesh.get());  // 保存到Mesh列表，创建Entity时要用
		}
		else
		{
			//创建Mesh对象
			// pMesh = MeshManager::getSingleton().createManual(sName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			MessageBox(NULL, LPCSTR("数据中没有为mesh指定名称，可能会导致问题！"), LPCSTR("错误"), 0);
			continue;
		}

		//向Mesh中填充顶点和面数据
		//使用单一的共享缓冲声明顶点缓冲对应的内存指针
		pMesh->sharedVertexData = new VertexData();
		VertexData* pVertexData = pMesh->sharedVertexData;
		
		//设置定点声明
		VertexDeclaration* pVertexDecl = pVertexData->vertexDeclaration;

		UINT ElementCount;
		CPY_FROM_STREAM(ElementCount, Offset)  // elment数目

		// 读入所有Element数据, Element中存储的是顶点的格式说明
		for (UINT iElement = 0; iElement < ElementCount; ++iElement) 
		{
			unsigned short source;
			UINT offsetIdx;
			unsigned short type, Semantic, index;
			CPY_FROM_STREAM(source, Offset)
			CPY_FROM_STREAM(offsetIdx, Offset)
			CPY_FROM_STREAM(type, Offset)
			CPY_FROM_STREAM(Semantic, Offset)
			CPY_FROM_STREAM(index, Offset)
			pVertexDecl->addElement(source, offsetIdx, (Ogre::VertexElementType)type, (Ogre::VertexElementSemantic)Semantic, index);
		}

		UINT vertexCount;
		CPY_FROM_STREAM(vertexCount, Offset)         // 顶点数目
		UINT StartIdx;
		CPY_FROM_STREAM(StartIdx, Offset)            // 顶点数据开始位置，如果每个mesh都自己保存顶点缓冲数据的话，那么这个值就等于0
		pVertexData->vertexCount = vertexCount;
		pVertexData->vertexStart = StartIdx;

		//分配顶点缓冲
		INT vertexSize = pVertexDecl->getVertexSize(0);  // 得到存储一个顶点数据的字节数目
		HardwareVertexBufferSharedPtr vBuf;
		//if (pVGSMesh->bindingSkeletonIndex == -1) // 暂时还没有读取骨骼数据
		{
			vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexSize,
				   pVertexData->vertexCount, Ogre::HardwareBuffer::Usage::HBU_STATIC_WRITE_ONLY, false);
		}
		/*else
		{
			vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexSize,
				   pVertexData->vertexCount, HardwareBuffer::Usage::HBU_DYNAMIC_WRITE_ONLY, false);
		}*/
		VertexBufferBinding* pBinding = pVertexData->vertexBufferBinding;
		pBinding->setBinding(0, vBuf);

		//填充顶点数据
		UINT VertexBytes = vertexSize * pVertexData->vertexCount;
		char *pVertexBuf = static_cast<char*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		memcpy(pVertexBuf, Offset, VertexBytes);  // 一次性读取所有顶点数据
		Offset += VertexBytes;

		vBuf->unlock();

		unsigned short SubMeshCount;
		CPY_FROM_STREAM(SubMeshCount, Offset)    // sub mesh count
		for (UINT iSubMesh = 0; iSubMesh < SubMeshCount; ++iSubMesh)
		{
			//创建SubMesh对象
			SubMesh* pSubMesh = pMesh->createSubMesh();
			//创建索引缓冲，生成面表
			pSubMesh->useSharedVertices = true;			
			
			UINT NameLen;
			CPY_FROM_STREAM(NameLen, Offset)    // submesh使用的材质名称长度
			if (NameLen > 0)  // submesh 可能没有名字
			{
				char *sName = new char[NameLen + 1];
				ZeroMemory(sName, NameLen + 1);
				memcpy(sName, Offset, NameLen);     // submesh使用的材质名称	
				pSubMesh->setMaterialName(String(sName));   // 设置材质
				Offset += NameLen;
				DELETE_ARRAYPTR(sName)
			}

			UINT IndexCount, IndexStart;
			CPY_FROM_STREAM(IndexCount, Offset)  // submesh的索引数目
			CPY_FROM_STREAM(IndexStart, Offset)  // 在索引缓冲中的开始位置，如果每个submesh自己保存索引缓冲的话，这个值就等于0
			pSubMesh->indexData->indexCount = IndexCount;
			pSubMesh->indexData->indexStart = IndexStart;

			if (IndexCount > 65536)  // 用四个字节的缓冲
			{
				pSubMesh->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IndexType::IT_32BIT,
				                                   pSubMesh->indexData->indexCount, Ogre::HardwareBuffer::Usage::HBU_STATIC_WRITE_ONLY, false);
		
				HardwareIndexBufferSharedPtr iBuf = pSubMesh->indexData->indexBuffer;

				//向索引缓冲中填充数据
				char *pIndexData = static_cast<char*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));
				
				// 一次性读取缓冲数据
				memcpy(pIndexData, Offset, 4 * IndexCount);
				Offset += 4 * IndexCount;

				iBuf->unlock();
			}
			else  // 用两个字节的缓冲
			{
				pSubMesh->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IndexType::IT_16BIT,
				                                   pSubMesh->indexData->indexCount, Ogre::HardwareBuffer::Usage::HBU_STATIC_WRITE_ONLY, false);
		
				HardwareIndexBufferSharedPtr iBuf = pSubMesh->indexData->indexBuffer;

				//向索引缓冲中填充数据
				char *pIndexData = static_cast<char*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));
				
				// 一次性读取缓冲数据
				memcpy(pIndexData, Offset, 2 * IndexCount);
				Offset += 2 * IndexCount;

				iBuf->unlock();
			}			
		}

		// 包围盒
		Vector3 NearConner, FarConner;
		CPY_VECTOR3_FROM_STRRAM(NearConner, Offset)
		CPY_VECTOR3_FROM_STRRAM(FarConner, Offset)
		Ogre::AxisAlignedBox AABB;
		AABB.setExtents(NearConner, FarConner);
		pMesh->_setBounds(AABB, false);

		// 包围球
		float radius;
		CPY_FROM_STREAM(radius, Offset);
		pMesh->_setBoundingSphereRadius((Ogre::Real)radius);

		pMesh->load();  // 加载mesh

		// 创建骨骼
		// ...

		////创建实体
		//Entity* pEntity = m_pVGSSceneMgr->mSceneMgr->createEntity(pMesh->getName(), pMesh->getName());
		//pEntity->setNormaliseNormals(true);
		//pEntity->_initialise();

		// 创建骨骼动画
		
		
		//加入VGS场景管理器
		m_pVGSSceneMgr->AddMeshNameMap(pMesh->getName(), pMesh->getName());

		//加入VGS场景管理器
		//m_pVGSSceneMgr->AddEntityNameMap(pEntity->getName(), pEntity->getName());

	} // mesh

	DELETE_ARRAYPTR(m_pMeshBuf);  // 删除字节流

	return ERR_COMMON_OK;
}

//创建相机的参数动画
void CGetV3D::CreateCameraParamAnimation(Ogre::Camera* pCamera, VGS_V3D_Camera* pVGSCamera, int frameCnt)
{
	//关键帧数据
	CameraParamAnimation* pCameraParamAni = new CameraParamAnimation(pCamera);

	pCameraParamAni->mCamera = pCamera; // fuxb 2008-4-18
	pCameraParamAni->frameCnt = frameCnt;
	pCameraParamAni->cameraInfoList = new VGS_V3D_CAMINFO[frameCnt];
	for (INT frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		pCameraParamAni->cameraInfoList[frameIndex] = pVGSCamera->v3dAnimationInfo[frameIndex];
	}

	pCameraParamAni->setLength(Real(frameCnt)/gkfrPlaySpeed);
	
	//放入相机参数动画队列
	// cameraParamAnimationList.push_back(pCameraParamAni);
	m_pVGSSceneMgr->GetAnimationManager()->AddCameraAnimation(pCameraParamAni);
}

//读取相机数据并创建相机到场景
DWORD CGetV3D::ReadCameraData()
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

	//读入相机数量
	int cameraCnt = 0;                         
	CPY_FROM_STREAM(cameraCnt, Offset)       

	//逐个相机读入数据
	for (int i = 0; i< cameraCnt; i++)
	{
		VGS_V3D_Camera VGSCam;
		ZeroMemory((void*)&VGSCam, sizeof(VGSCam));

		//相机名称
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset);
		char *pName = new char[len + 1];
		ZeroMemory(pName, len + 1);
		strncpy(pName, Offset, len); // '\0' excluded
		Offset += len;
		VGSCam.name = std::string(pName);
		VGSCam.nameLen = VGSCam.name.length();  // include '\0'
		delete pName;
		pName = NULL;

		//渲染同步对象
		//EnterCriticalSection(m_render_cs);
		//创建相机
		Ogre::Camera* pCamera = m_pVGSSceneMgr->CreateCamera(Ogre::String(VGSCam.name.c_str()), false);
		//渲染同步对象
		//LeaveCriticalSection(m_render_cs);

		//相机的基本参数
		CPY_FROM_STREAM(VGSCam.v3dBaseInfo, Offset);

		//相机的投影类型
		// CPY_FROM_STREAM(VGSCam.isOrtho, Offset);
		//是否是轴侧
		if (VGSCam.v3dBaseInfo.isOrtho)
			pCamera->setProjectionType(PT_ORTHOGRAPHIC);
		else
			pCamera->setProjectionType(PT_PERSPECTIVE);

		//pCamera->setFixedYawAxis(false);  // fuxb 2008-08-29


		//Fov
		pCamera->setFOVy(Radian(VGSCam.v3dBaseInfo.fov));

		//near far clip
		pCamera->setNearClipDistance(VGSCam.v3dBaseInfo.nearClipDis);
		pCamera->setFarClipDistance(VGSCam.v3dBaseInfo.farClipDis);

		// 相机位置和四元数
		pCamera->setPosition(Vector3(VGSCam.v3dBaseInfo.position.x, VGSCam.v3dBaseInfo.position.y, VGSCam.v3dBaseInfo.position.z));
		pCamera->setOrientation(Quaternion(VGSCam.v3dBaseInfo.quat.w, VGSCam.v3dBaseInfo.quat.x, VGSCam.v3dBaseInfo.quat.y, VGSCam.v3dBaseInfo.quat.z));

		m_vpCamera.push_back(pCamera);   // 保存起来，后面可能会使用

		//读入参数动画帧数
		int frameCnt;
		CPY_FROM_STREAM(frameCnt, Offset);
		if (frameCnt == 0) continue;

		//写入参数动画数据
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			VGS_V3D_CAMINFO camInfo;
			//读入相机的动画信息
			CPY_FROM_STREAM(camInfo, Offset);
			//加入队列
			VGSCam.v3dAnimationInfo.push_back(camInfo);
		}

		//创建相机的参数动画
		CreateCameraParamAnimation(pCamera, &VGSCam, frameCnt);		
	}

	DELETE_ARRAYPTR(m_pCameraBuf);  // 删除字节流

	return ERR_COMMON_OK;
}


DWORD CGetV3D::ReadLightData()
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
	
	//逐个灯光读取数据
	for (int i = 0; i < LightCount; i++)
	{
		//读入灯光的名称
		VGS_Light vgsLight;
		ZeroMemory((void*)&vgsLight, sizeof(vgsLight)); 

		UINT len = 0;
		CPY_FROM_STREAM(len, Offset);
		vgsLight.name.reserve(len + 1);
		strncpy((char*)vgsLight.name.c_str(), Offset, len); // 不包括'\0'
		//vgsLight.name[len] = '\0';
		vgsLight.strLen = len + 1; // 包括'\0'
		Offset += len;

		//创建灯光
		Light* pLight = m_pVGSSceneMgr->CreateLight(Ogre::String(vgsLight.name.c_str()), false);

		//读入灯光的基本参数
		CPY_FROM_STREAM(vgsLight.type, Offset);
		//灯光类型
		switch (vgsLight.type)
		{
		case LIGHTTYPE::OMINI:
			{
				pLight->setType(Ogre::Light::LT_POINT); // 点光源

				VGS_V3D_OMNI omni;
				//读入点光源信息
				CPY_FROM_STREAM(omni, Offset);
				
				//颜色
				Ogre::ColourValue color;
				color.r = ((omni.color >> 24) & 0xff)/255.0f;
				color.g = ((omni.color >> 16) & 0xff)/255.0f;
				color.b = ((omni.color >> 8) & 0xff)/255.0f;
				color.a = ((omni.color) & 0xff)/255.0f;
				
				//强度
				color *= omni.multiply;
				pLight->setDiffuseColour(color);

				// 保存灯光参数到场景管理器
				// pLight->setPowerScale(omni.multiply);
				CVGSLightInfo VGSLightInfo;
				VGSLightInfo.m_sVGSName = Ogre::String(vgsLight.name.c_str());
				VGSLightInfo.m_Multiply = omni.multiply;
				m_pVGSSceneMgr->AddVGSLightInfo(VGSLightInfo);

				//高光
				pLight->setSpecularColour(color);
				
				//衰减
				pLight->setAttenuation(omni.attenuationRange, omni.attenuation0, omni.attenuation1, omni.attenuation2);

				pLight->setPosition(Vector3(omni.position.x, omni.position.y, omni.position.z));
			}
			break;

		case LIGHTTYPE::DIRECTIONLIGHT:
			{
				pLight->setType(Ogre::Light::LT_DIRECTIONAL); // 方向灯

				VGS_V3D_DIRECTIONAL directionLight;
				//读入方向光源信息
				CPY_FROM_STREAM(directionLight, Offset);

				//颜色
				Ogre::ColourValue color;
				color.r = ((directionLight.color>>24) & 0xff)/255.0f;
				color.g = ((directionLight.color>>16) & 0xff)/255.0f;
				color.b = ((directionLight.color>>8) & 0xff)/255.0f;
				color.a = (directionLight.color & 0xff) / 255.0f;
				color *= directionLight.multiply;

				//灯光颜色
				pLight->setDiffuseColour(color);

				// 保存灯光参数到场景管理器
				// pLight->setPowerScale(omni.multiply);
				CVGSLightInfo VGSLightInfo;
				VGSLightInfo.m_sVGSName = Ogre::String(vgsLight.name.c_str());
				VGSLightInfo.m_Multiply = directionLight.multiply;
				m_pVGSSceneMgr->AddVGSLightInfo(VGSLightInfo);

				//高光
				pLight->setSpecularColour(color);
				
				//方向
				Ogre::Vector3 direction;
				direction.x = directionLight.direction.x;
				direction.y = directionLight.direction.y;
				direction.z = directionLight.direction.z;
				pLight->setDirection(direction);

				//衰减
				pLight->setAttenuation(directionLight.attenuationRange, directionLight.attenuation0, directionLight.attenuation1, directionLight.attenuation2);

				// 位置
				//pLight->setPosition(Vector3(directionLight.position.x, directionLight.position.y, directionLight.position.z));
			}
			break;

		case LIGHTTYPE::SPOTLIGHT:
			{
				pLight->setType(Ogre::Light::LT_SPOTLIGHT); // 聚光灯

				VGS_V3D_SPOTLIGHT spotLight;
				//读入聚光灯信息
				CPY_FROM_STREAM(spotLight, Offset);
				
				//颜色
				Ogre::ColourValue color;
				color.r = ((spotLight.color>>24) & 0xff)/255.0f;
				color.g = ((spotLight.color>>16) & 0xff)/255.0f;
				color.b = ((spotLight.color>>8) & 0xff)/255.0f;
				color.a = (spotLight.color & 0xff) / 255.0f;
				color *= spotLight.multiply;

				//灯光颜色
				pLight->setDiffuseColour(color);

				// 保存灯光参数到场景管理器
				CVGSLightInfo VGSLightInfo;
				VGSLightInfo.m_sVGSName = Ogre::String(vgsLight.name.c_str());
				VGSLightInfo.m_Multiply = spotLight.multiply;
				m_pVGSSceneMgr->AddVGSLightInfo(VGSLightInfo);

				//高光
				pLight->setSpecularColour(color);
				
				//方向
				Ogre::Vector3 direction;
				direction.x = spotLight.direction.x;
				direction.y = spotLight.direction.y;
				direction.z = spotLight.direction.z;
				pLight->setDirection(direction);

				//衰减
				pLight->setAttenuation(spotLight.attenuationRange, spotLight.attenuation0, spotLight.attenuation1, spotLight.attenuation2);

				//张角
				pLight->setSpotlightInnerAngle(Radian(spotLight.innerAngle));
				pLight->setSpotlightOuterAngle(Radian(spotLight.outerAngle));
				pLight->setSpotlightFalloff(spotLight.fallOff);

				// 位置
				pLight->setPosition(Vector3(spotLight.position.x, spotLight.position.y, spotLight.position.z));

			}
			break;
		}

		m_vpLight.push_back(pLight);  // 保存以及加载的灯光

		//灯光动画
		//读入动画帧数
		int frameCnt = 0;
		CPY_FROM_STREAM(frameCnt, Offset);
		if (frameCnt == 0) continue;

		//创建灯光参数动画-----------------------------------------------------------------
		CreateLightParamAnimation(pLight, &vgsLight, frameCnt, &Offset);
	}

	DELETE_ARRAYPTR(m_pLightBuf);  // 删除字节流

	return ERR_COMMON_OK;
}

//创建灯光参数动画
void CGetV3D::CreateLightParamAnimation(Light* pLight, VGS_Light* pVGSLight, int frameCnt, char** Offset)
{
	//创建动画参数对象
	LightParamAnimation* pLightParamAni = new LightParamAnimation(pLight);
	pLightParamAni->mLight = pLight;
	pLightParamAni->frameCnt = frameCnt;
	//写入每帧的数据
	switch (pVGSLight->type)
	{
	case LIGHTTYPE::OMINI:
		{
			pLightParamAni->omniInfoList = new VGS_V3D_OMNI[frameCnt];
			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
				CPY_FROM_STREAM(pLightParamAni->omniInfoList[frameIndex], *Offset);
		}
		break;

	case LIGHTTYPE::DIRECTIONLIGHT:
		{	
			pLightParamAni->directionLightInfoList = new VGS_V3D_DIRECTIONAL[frameCnt];
			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
				CPY_FROM_STREAM(pLightParamAni->directionLightInfoList[frameIndex], *Offset);
		}
		break;

	case LIGHTTYPE::SPOTLIGHT:
		{
			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
				CPY_FROM_STREAM(pLightParamAni->spotLightInfoList[frameIndex], *Offset);
		}
		break;
	}
	
	pLightParamAni->setLength(Real(frameCnt)/gkfrPlaySpeed);
	
	//放入灯光参数动画队列
	// lightParamAnimationList.push_back(pLightParamAni);
	m_pVGSSceneMgr->GetAnimationManager()->AddLightAnimation(pLightParamAni);
}
/*
//读取相机数据并创建相机到场景
DWORD CGetV3D::ReadCameraData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pCameraBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 读取mesh信息
	char* Offset = m_pCameraBuf;            // 字节流中数据开始的位置
	DWORD CamCount;                         // Camera数目
	CPY_FROM_STREAM(CamCount, Offset)       // 读取Camera数目
	
	// 读取每个Camera
	for (UINT iCam = 0; iCam < CamCount; ++iCam)
	{
		UINT NameLen;
		CPY_FROM_STREAM(NameLen, Offset)
		Camera* pCamera = NULL;
		if (NameLen > 0)
		{
			char *sName = new char[NameLen + 1];
			ZeroMemory(sName, NameLen + 1);
			memcpy(sName, Offset, NameLen);
			Offset += NameLen;

			//创建相机
			pCamera = m_pVGSSceneMgr->CreateCamera(String(sName), false);
			DELETE_ARRAYPTR(sName)
		}
		else
		{
			MessageBox(NULL, LPCSTR("数据中没有为相机指定名称，可能会导致问题！"), LPCSTR("错误"), 0);
			continue;
		}

		V3D_CameraDetail CamDetail;
		CPY_FROM_STREAM(CamDetail, Offset)  // 一次性读取相机参数

		m_vCameraDetail.push_back(CamDetail);  // 暂时保存，等创建场景图的时候用到


		//创建相机的参数动画
		// CreateCameraParamAnimation(pCamera, pVGSCamera);

		//加入VGS场景管理器
		m_pVGSSceneMgr->AddCamNameMap(pCamera->getName(), pCamera->getName());

	}

	DELETE_ARRAYPTR(m_pCameraBuf);  // 删除字节流

	return ERR_COMMON_OK;
}
DWORD CGetV3D::ReadLightData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pLightBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 读取mesh信息
	char* Offset = m_pLightBuf;               // 字节流中数据开始的位置
	DWORD LightCount;                         // Light数目
	CPY_FROM_STREAM(LightCount, Offset)       // 读取Light数目
	
	// 读取每个Light
	for (UINT iLight = 0; iLight < LightCount; ++iLight)
	{
		UINT NameLen;
		CPY_FROM_STREAM(NameLen, Offset)
		Light* pLight = NULL;
		if (NameLen > 0)
		{
			char *sName = new char[NameLen + 1];
			ZeroMemory(sName, NameLen + 1);
			memcpy(sName, Offset, NameLen);
			Offset += NameLen;
			//创建灯光
			pLight = m_pVGSSceneMgr->CreateLight(sName, false);
			DELETE_ARRAYPTR(sName)
		}
		else
		{
			MessageBox(NULL, LPCSTR("数据中没有为灯光指定名称，可能会导致问题！"), LPCSTR("错误"), 0);
			//创建灯光
			// pLight = m_pVGSSceneMgr->mSceneMgr->createLight(sName);
			continue;
		}

		V3D_LightDetail LightDetail;
		CPY_FROM_STREAM(LightDetail, Offset)  // 一次性读取light参数

		Ogre::ColourValue color;
		pLight->setType(Ogre::Light::LightTypes(LightDetail.type));
		color.setAsRGBA(Ogre::RGBA(LightDetail.Dif_RGBA));
		pLight->setDiffuseColour(color);
		color.setAsRGBA(Ogre::RGBA(LightDetail.Spe_RGBA));
		pLight->setSpecularColour(color);
		pLight->setAttenuation(LightDetail.fAttenuationRange, LightDetail.fAttenuationConst, LightDetail.fAttenuationLinear, LightDetail.fAttenuationQuadric);
		pLight->setPosition(LightDetail.PositionX,  LightDetail.PositionY, LightDetail.PositionZ);
		pLight->setDirection(LightDetail.DirectionX, LightDetail.DirectionY, LightDetail.DirectionZ);

		// 如果是聚光灯，需要读取聚光灯的内、外锥形的角度,falloff
		if (pLight->getType() == Ogre::Light::LightTypes::LT_SPOTLIGHT)
		{
			float innerAngle, outerAngle, falloff;
			CPY_FROM_STREAM(innerAngle, Offset)
			CPY_FROM_STREAM(outerAngle, Offset)
			CPY_FROM_STREAM(falloff, Offset)
	
			pLight->setSpotlightFalloff(Ogre::Real(falloff));
			pLight->setSpotlightInnerAngle(Ogre::Radian(Ogre::Real(innerAngle)));
			pLight->setSpotlightOuterAngle(Ogre::Radian(Ogre::Real(outerAngle)));
			
		}
	} // for light

	DELETE_ARRAYPTR(m_pLightBuf);  // 删除字节流

	return ERR_COMMON_OK;
}*/

//Read the Text data
DWORD CGetV3D::ReadTextData()
{
	return ERR_COMMON_OK;
}
//Read the overlay data
DWORD CGetV3D::ReadOverlayData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pOverlayBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 读取导航图的数量
	char* Offset = m_pOverlayBuf; 
	int OverlayCount = 0; 
	CPY_FROM_STREAM(OverlayCount, Offset);

	for (int i = 0; i < OverlayCount; ++i)
	{
		V3D_OverlayInfo Info;
		CPY_FROM_STREAM(Info, Offset);  // Overlay基本属性

		// 创建Overlay
		RECT rect;
		rect.left = rect.right = rect.top = rect.bottom = 10;

		Ogre::String sBaseIMg = "";
		Ogre::String sOpacImg = "";
		if (!Ogre::String(Info.BaseImg).empty())
		{
			sBaseIMg = AddPathtoTextureFile(Info.BaseImg);
		}
		if (!Ogre::String(Info.OpacImg).empty())
		{
			sOpacImg = AddPathtoTextureFile(Info.OpacImg);
		}

		// 创建Overlay
		DWORD overlyaHandle = CreateOverlayFromFile((DWORD)m_pVGSSceneMgr, Info.Name, sBaseIMg.c_str(), sOpacImg.c_str(), rect);

		// 视口大小
		float vpwidth, vpheight;
		GUI_GetViewportSize(vpwidth, vpheight);

		float left, top, width, height;  // overlay的实际坐标及大小

		if (Info.bRelativePos)  // 转换成绝对坐标
		{
			left = vpwidth * Info.Left;
			top  = vpheight * Info.Top;
		}
		else
		{
			left = Info.Left;
			top  = Info.Top;
		}

		if (Info.bRelativeSize)  // 转换成绝对坐标
		{
			width  = vpwidth * Info.Width;
			height = vpheight * Info.Height;
		}
		else
		{
			width  = Info.Width;
			height = Info.Height;
		}

		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, overlyaHandle, VGSOFFSETORIGIN::VOO_TOP_LEFT);   // 先设置为左上角便于设置位置
		SetGUIObjUseRelativePos((DWORD)m_pVGSSceneMgr, overlyaHandle, Info.bRelativePos);
		SetGUIObjUseRelativeSize((DWORD)m_pVGSSceneMgr, overlyaHandle, Info.bRelativeSize);
		SetGUIObjectSize((DWORD)m_pVGSSceneMgr, overlyaHandle, width, height, Info.bRelativeSize);
		SetGUIObjectPosition((DWORD)m_pVGSSceneMgr, overlyaHandle, left, top, Info.bRelativePos);
		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, overlyaHandle, VGSOFFSETORIGIN(Info.Origin));  // 设置offset origin
		SetGUIObjectOpacity((DWORD)m_pVGSSceneMgr, overlyaHandle, Info.Opacity);
		SetGUIObjectVisibility((DWORD)m_pVGSSceneMgr, overlyaHandle, Info.bVisible);

	}

	return ERR_COMMON_OK;
}
//Read the navigater data
DWORD CGetV3D::ReadNavigaterData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pNaviBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 读取导航图的数量
	char* Offset = m_pNaviBuf; 
	int NavCount = 0; 
	CPY_FROM_STREAM(NavCount, Offset);

	for (int i = 0; i < NavCount; ++i)
	{
		V3D_NavigatorInfo NaviInfo;  // 一个导航图的属性

		CPY_FROM_STREAM(NaviInfo.NaviBaseInfo, Offset);  // 基本属性

		FRect maprect;
		CPY_FROM_STREAM(maprect, Offset);                 // map导航区域	

		// 视口大小
		float vpwidth, vpheight;
		GUI_GetViewportSize(vpwidth, vpheight);

		FRect rect;
		if (NaviInfo.NaviBaseInfo.bRelativePos)  // 转换成绝对坐标
		{
			rect.Left = vpwidth * NaviInfo.NaviBaseInfo.Left;
			rect.Top  = vpheight * NaviInfo.NaviBaseInfo.Top;
		}
		else
		{
			rect.Left = NaviInfo.NaviBaseInfo.Left;
			rect.Top  = NaviInfo.NaviBaseInfo.Top;
		}

		if (NaviInfo.NaviBaseInfo.bRelativeSize)  // 转换成绝对坐标
		{
			rect.Right  = rect.Left + vpwidth * NaviInfo.NaviBaseInfo.Width;
			rect.Bottom = rect.Top + vpheight * NaviInfo.NaviBaseInfo.Height;
		}
		else
		{
			rect.Right  = rect.Left + NaviInfo.NaviBaseInfo.Width;
			rect.Bottom = rect.Top +NaviInfo.NaviBaseInfo.Height;
		}

		// 创建导航图
		DWORD Navhandle = CreateNavigater((DWORD)m_pVGSSceneMgr, NaviInfo.NaviBaseInfo.Name, 
			AddPathtoTextureFile(NaviInfo.NaviBaseInfo.MapImg).c_str(), rect);   

		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, Navhandle, VGSOFFSETORIGIN::VOO_TOP_LEFT);   // 先设置为左上角便于设置位置
		SetGUIObjUseRelativePos((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.bRelativePos);
		SetGUIObjUseRelativeSize((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.bRelativeSize);
		SetGUIObjectSize((DWORD)m_pVGSSceneMgr, Navhandle, rect.Right - rect.Left, rect.Bottom - rect.Top, NaviInfo.NaviBaseInfo.bRelativeSize);
		SetGUIObjectPosition((DWORD)m_pVGSSceneMgr, Navhandle, rect.Left, rect.Top, NaviInfo.NaviBaseInfo.bRelativePos);
		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, Navhandle, VGSOFFSETORIGIN(NaviInfo.NaviBaseInfo.Origin));  // 设置offset origin
		SetGUIObjectOpacity((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.Opacity);
		SetGUIObjectVisibility((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.bVisible);

		// SetNavigaterMap((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.MapImg);   // 设置导航地图
		SetNavigaterSceneRect((DWORD)m_pVGSSceneMgr, Navhandle, maprect);  // 设置导航区域

		CPY_FROM_STREAM(NaviInfo.PointerCount, Offset);   // 导航点个数

		for (int i = 0; i < NaviInfo.PointerCount; ++i)
		{
			V3D_NaviPointerInfo PointerInfo;
			CPY_FROM_STREAM(PointerInfo, Offset);   // 导航点属性

			// 获取导航点导航的对象句柄
			DWORD pointedHandle = 0;
			if (PointerInfo.NavType == 0 && PointerInfo.PointedIndex >=0 && PointerInfo.PointedIndex < m_vpCamera.size())  // camera
			{
				pointedHandle = (DWORD)m_vpCamera[PointerInfo.PointedIndex];
			}
			else if (PointerInfo.NavType == 1 && PointerInfo.PointedIndex >=0 && PointerInfo.PointedIndex < m_vEntityList.size())  // 模型
			{
				pointedHandle = m_vEntityList[PointerInfo.PointedIndex];
			}

			// 创建导航点
			bool re = AddNavigaterPointer((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, AddPathtoTextureFile(PointerInfo.PointerImg).c_str(), 
								PointerInfo.NavType, pointedHandle);
			// 设置导航点参数
			if (re)
			{
				SetNavPointerVisiblilty((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, PointerInfo.bVisible);
				SetNavPointerOpacity((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, PointerInfo.Opacity);
				SetNavPointerSize((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, PointerInfo.width, PointerInfo.height);
				SetPointerPosition((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, PointerInfo.left, PointerInfo.top);
			} // if

		} // for 每个导航点

	} // for 每个导航图

	return ERR_COMMON_OK;
}

//Read the button data
DWORD CGetV3D::ReadButtonData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pButtonBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 读取导航图的数量
	char* Offset = m_pButtonBuf; 
	int BtnCount = 0; 
	CPY_FROM_STREAM(BtnCount, Offset);

	for (int i = 0; i < BtnCount; ++i)
	{
		V3D_ButtonInfo BtnInfo;  // 一个按钮的属性
		CPY_FROM_STREAM(BtnInfo, Offset);  // 基本属性

		// 创建按钮
		RECT rect;
		rect.bottom = rect.left = rect.right = rect.top = 0;

		Ogre::String sUpImg = "";
		Ogre::String sDownImg = "";
		Ogre::String sOverImg = "";
		if (!Ogre::String(BtnInfo.UpImageName).empty())
		{
			sUpImg = AddPathtoTextureFile(BtnInfo.UpImageName);
		}
		if (!Ogre::String(BtnInfo.DownImageName).empty())
		{
			sDownImg = AddPathtoTextureFile(BtnInfo.DownImageName);
		}
		if (!Ogre::String(BtnInfo.OverImageName).empty())
		{
			sOverImg = AddPathtoTextureFile(BtnInfo.OverImageName);
		}
		DWORD BtnHandle = CreateButtonFromFile((DWORD)m_pVGSSceneMgr, BtnInfo.Name, rect, 
			                                    sUpImg.c_str(), sOverImg.c_str(), sDownImg.c_str());
		
		// 计算按钮的实际位置和实际大小

		// 视口大小
		float vpwidth, vpheight;
		GUI_GetViewportSize(vpwidth, vpheight);

		float left, top, width, height;  // overlay的实际坐标及大小

		if (BtnInfo.bRelativePos)  // 转换成绝对坐标
		{
			left = vpwidth * BtnInfo.Left;
			top  = vpheight * BtnInfo.Top;
		}
		else
		{
			left = BtnInfo.Left;
			top  = BtnInfo.Top;
		}

		if (BtnInfo.bRelativeSize)  // 转换成绝对坐标
		{
			width  = vpwidth * BtnInfo.Width;
			height = vpheight * BtnInfo.Height;
		}
		else
		{
			width  = BtnInfo.Width;
			height = BtnInfo.Height;
		}

		// 设置按钮的属性
		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, BtnHandle, VGSOFFSETORIGIN::VOO_TOP_LEFT);   // 先设置为左上角便于设置位置
		SetGUIObjUseRelativePos((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.bRelativePos);
		SetGUIObjUseRelativeSize((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.bRelativeSize);
		SetGUIObjectSize((DWORD)m_pVGSSceneMgr, BtnHandle, width, height, BtnInfo.bRelativeSize);
		SetGUIObjectPosition((DWORD)m_pVGSSceneMgr, BtnHandle, left, top, BtnInfo.bRelativePos);
		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, BtnHandle, VGSOFFSETORIGIN(BtnInfo.Origin));  // 设置offset origin
		SetGUIObjectOpacity((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.Opacity);
		SetGUIObjectVisibility((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.bVisible);
		SetButtonCallBack((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.CallbackFunName);   // 按钮的回调函数
		
	}
	
	return ERR_COMMON_OK;
}

// 背景图
DWORD CGetV3D::ReadBackdropData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pBackdropBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// 读取导航图的数量
	char* Offset = m_pBackdropBuf; 
	int Count = 0; 
	CPY_FROM_STREAM(Count, Offset);

	if (Count > 0)
	{
		V3D_BackdropInfo Info;
		CPY_FROM_STREAM(Info, Offset);  // 基本属性

		Ogre::String str = AddPathtoTextureFile(Info.ImgFile);
		SetBackground((DWORD)m_pVGSSceneMgr, str.c_str());
		SetBackgroundVisibility((DWORD)m_pVGSSceneMgr, Info.bVisible);
		SetBackgroundRect((DWORD)m_pVGSSceneMgr, Info.Left, Info.Top, Info.Width, Info.Height);
	}

	return ERR_COMMON_OK;
}

//Read AudioData
DWORD CGetV3D::ReadAudioData()
{
	return ERR_COMMON_OK;
}

//Read FlashData
DWORD CGetV3D::ReadFlashData()
{
	return ERR_COMMON_OK;
}

// 动画
DWORD CGetV3D::ReadAnimationData()
{
	return ERR_COMMON_OK;
}

//Read privacy data
DWORD CGetV3D::ReadPrivacyData()
{
	return ERR_COMMON_OK;
}

// 控制
DWORD CGetV3D::ReadControlData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pVGSSceneMgr || !m_pControlBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}	

	// 读取总的控制器数目
	char* Offset = m_pControlBuf;                 // 字节流中controller数据开始的位置
	DWORD TotalCount = 0;                         // controller总数目
	CPY_FROM_STREAM(TotalCount, Offset)           // 读取controller总数目
	
	if (TotalCount == 0)
	{
		return ERR_COMMON_OK;
	}

	DWORD WalkerCount = 0;                         // walker数目
	CPY_FROM_STREAM(WalkerCount, Offset)           // 读取walker数目
	for (UINT i = 0; i < WalkerCount; ++i)
	{
		WalkerInfo Info;
		CPY_FROM_STREAM(Info, Offset)
		m_pVGSSceneMgr->CreateWalker(Info);
	}

	DWORD FlyerCount = 0;                          // flyer数目
	CPY_FROM_STREAM(FlyerCount, Offset)           // 读取flyer数目
	for (UINT i = 0; i < FlyerCount; ++i)
	{
		FlyerInfo Info;
		CPY_FROM_STREAM(Info, Offset)
		m_pVGSSceneMgr->CreateFlyer(Info);
	}

	DWORD EditorCount = 0;                          // editor数目
	CPY_FROM_STREAM(EditorCount, Offset)            // 读取editor数目
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
			if (Info.LookAtModelIndex >= 0 && Info.LookAtModelIndex < m_vEntityList.size())
			{
				Info.LookAtModelIndex = m_vEntityList[Info.LookAtModelIndex];  // 计算为真正的模型句柄
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
void CGetV3D::ConstructSceneGraph(SceneNode* pNode, char* offset)
{
	SceneNode* pChildNode = NULL;
	bool	bHasSkeleton = false;
	VGS_Node node;
	String RealName;

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

	//读入并设置位置\转角\缩放
	Ogre::Vector3 pos;
	CPY_FROM_STREAM(pos.x, offset);
	CPY_FROM_STREAM(pos.y, offset);
	CPY_FROM_STREAM(pos.z, offset);

	Ogre::Quaternion quat;
	CPY_FROM_STREAM(quat.x, offset);
	CPY_FROM_STREAM(quat.y, offset);
	CPY_FROM_STREAM(quat.z, offset);
	CPY_FROM_STREAM(quat.w, offset);

	Vector3 scale;
	CPY_FROM_STREAM(scale.x, offset);
	CPY_FROM_STREAM(scale.y, offset);
	CPY_FROM_STREAM(scale.z, offset);

	//读入该Node关键帧动画的长度
	float animationLength;
	CPY_FROM_STREAM(animationLength, offset);

	//找到对应的场景对象
	switch (node.type)
	{
	case SCENEROOT:
		pChildNode = pNode;
		break;

	case CAMERANODE:
		{
			if (node.index >= 0 && node.index < m_vpCamera.size())
			{
				Ogre::Camera* pCam = m_vpCamera[node.index];/*m_pVGSSceneMgr->GetCamera(node.index)*/;
				Vector3 vPos = pCam->getPosition();
				Quaternion q = pCam->getOrientation();

				RealName = pCam->getName();
				pChildNode = pNode->createChildSceneNode(RealName);
				pChildNode->attachObject(pCam);
				pCam->setPosition(vPos);
				pCam->setOrientation(q);
			}
			else
			{
				pChildNode = pNode->createChildSceneNode();
			}
		}
		break;

	case LIGHTNODE:
		{
			//if (node.index >= 0 && node.index < m_vpLight.size())
			{
				Ogre::Light* pLight = /*m_vpLight[node.index];*/m_pVGSSceneMgr->GetLight(node.index);

				Vector3 vPos = pLight->getPosition();
				Vector3 vDir = pLight->getDirection();

				RealName = pLight->getName();
				pChildNode = pNode->createChildSceneNode(RealName);
				pChildNode->attachObject(pLight);

				pLight->setPosition(vPos);

				if (pLight->getType() != Ogre::Light::LT_POINT)
				{
					pLight->setDirection(vDir);
				}
			}
			/*else
			{
				pChildNode = pNode->createChildSceneNode();
			}*/
		}
		break;

	case MESHNODE:
		{
			Ogre::Entity* pEntity = m_pVGSSceneMgr->GetEntity(node.index);
			RealName = pEntity->getName();
			pChildNode = pNode->createChildSceneNode(RealName);
			pChildNode->attachObject(pEntity);
			bHasSkeleton = pEntity->hasSkeleton();
		}
		break;

	case BACKDROP:
		{
			pChildNode = m_pVGSSceneMgr->GetBackgroundNode();
			break;
		}
	}

	//Node的变换, 相机的变换不在这里处理---------------------------------------------------------
	//位置//转化坐标系
	if (node.type == SCENEROOT)
	{
		pChildNode->setPosition(Vector3(0,0,0));
	}
	else
	{
		//位置
		pChildNode->setPosition(pos);
		//旋转//转化坐标系
		pChildNode->setOrientation(quat);
		//缩放//转化坐标系
		pChildNode->setScale(scale);
		
		//加入VGS场景管理器
		m_pVGSSceneMgr->AddNodeNameMap(pChildNode->getName(), pChildNode->getName());
	}

	//渲染同步对象
	//EnterCriticalSection(m_render_cs);
	//Node的关键帧动画----------------------------------------------------
	if (animationLength > 0 && !bHasSkeleton) //有骨骼动画则忽略关键帧动画
		ReadAndCreateNodeAnimation(pChildNode, animationLength, &offset);
	//渲染同步对象
	//LeaveCriticalSection(m_render_cs);

	m_Offset = offset - m_pSceneGraphBuf; // 重定位offset

	//读入子节点数量
	int childNodeCnt = 0;
	CPY_FROM_STREAM(childNodeCnt, offset);
	for (int i = 0; i< childNodeCnt; i++)
	{
		ConstructSceneGraph(pChildNode, m_pSceneGraphBuf + m_Offset);
		// ConstructSceneGraph(pChildNode, m_Offset);
	}

}

//创建节点的动画
void CGetV3D::ReadAndCreateNodeAnimation(SceneNode* pNode, float animationLength, char** offset)
{
	//读入动画数据-------------------------------------------------
	//创建关键帧动画
	// Ogre::String strAnimName = "Anim_" + pNode->getName();
	Ogre::String strAnimName = _GetNodeAnimationName(pNode);  // 获取节点动画名称
	Animation* pAnim = m_pVGSSceneMgr->mSceneMgr->createAnimation(strAnimName, animationLength);

	//读入关键帧动画的关键帧数量
	int keyframeCnt;
	CPY_FROM_STREAM(keyframeCnt, *offset);

	//设置过渡方式
	pAnim->setInterpolationMode(Ogre::Animation::InterpolationMode::IM_LINEAR);
	
	//定义一段动画轨迹
	NodeAnimationTrack* pAnimTrack = pAnim->createNodeTrack(0, pNode);
	//逐帧读入数据
	for (int kfrIndex = 0; kfrIndex< keyframeCnt; kfrIndex++)
	{
		NodeTransform nodeTransform; 
		CPY_FROM_STREAM(nodeTransform, *offset);
		
		//当前帧对应的Transform对象
		TransformKeyFrame* pKfr = pAnimTrack->createNodeKeyFrame(nodeTransform.time);

		//位置//坐标系转换
		Vector3 pos;
		pos.x = nodeTransform.position.x;
		pos.y = nodeTransform.position.y;
		pos.z = nodeTransform.position.z;
		pKfr->setTranslate(pos);

		//转角//坐标系转换
		Ogre::Quaternion quat;
		quat.x = nodeTransform.quat.x;
		quat.y = nodeTransform.quat.y;
		quat.z = nodeTransform.quat.z;
		quat.w = nodeTransform.quat.w;
		pKfr->setRotation(quat);

		//缩放
		Vector3 scale;
		scale.x = nodeTransform.scale.x;
		scale.y = nodeTransform.scale.y;
		scale.z = nodeTransform.scale.z;
		pKfr->setScale(scale);
	}
	
	//优化
	pAnimTrack->optimise();

	//创建动画状态对象
	AnimationState* pAnimState = m_pVGSSceneMgr->mSceneMgr->createAnimationState(strAnimName);
	pAnimState->setEnabled(true);
	pAnimState->setLoop(true);

	// 增加一个节点动画
	m_pVGSSceneMgr->GetAnimationManager()->AddNodeKeyfrmAnimation(pAnimState);
}


// 场景图
DWORD CGetV3D::ReadSceneGraph()
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
	SceneNode* pRootNode = m_pVGSSceneMgr->mSceneMgr->getRootSceneNode();
	ConstructSceneGraph(pRootNode, m_pSceneGraphBuf);	// 读取SceneGraph信息// 字节流中数据开始的位置

	// 删除字节流
	DELETE_ARRAYPTR(m_pSceneGraphBuf);  

	return ERR_COMMON_OK;
}


/*
// 场景图
DWORD CGetV3D::ReadSceneGraph()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pSceneGraphBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	std::vector <SceneGraphNode> vSceneGraph;  // 场景图数据结构

	// 读取SceneGraph信息
	char* Offset = m_pSceneGraphBuf;               // 字节流中数据开始的位置
	DWORD NodeCount;                               // 节点数目
	CPY_FROM_STREAM(NodeCount, Offset)             // 读取节点数目
	
	if (NodeCount == 0)
	{
		return ERR_COMMON_OK;
	}

	vSceneGraph.reserve(NodeCount); // 预留空间

	// 读取根节点下的节点
	for (UINT iNode = 0; iNode < NodeCount; ++iNode)
	{
		SceneGraphNode SGNode;
		UINT ObjCount = 0;

		memcpy(&SGNode, Offset, sizeof(NodeSaveParam));  // 直接读取sizeof(NodeSaveParam)大小的数据块到SGNode
		Offset += sizeof(NodeSaveParam);

		CPY_FROM_STREAM(ObjCount, Offset)               // 读取节点下附属对象的个数
		for (UINT iObj = 0; iObj < ObjCount; ++iObj)
		{
			NodeObject obj;
			CPY_FROM_STREAM(obj.type, Offset)   // 节点对象的类型
			CPY_FROM_STREAM(obj.Index, Offset)  // 节点对象在各自存储队列中的索引
			SGNode.vObject.push_back(obj);
		}		
		vSceneGraph.push_back(SGNode);
		// vSceneGraph[iNode] = SGNode;
	} // for iNode

	SceneManager *pSceneMgr = m_pVGSSceneMgr->mSceneMgr;   // Ogre场景管理器

	std::vector <Ogre::SceneNode*> vSceneNodeList;       // 保存已经创建的ogre场景节点
	vSceneNodeList.reserve(NodeCount);                    // 预分配空间
	// vSceneNodeList.resize(NodeCount);
	// vSceneNodeList[0]  = pSceneMgr->getRootSceneNode();   // 第一个位置存储根节点  
	vSceneNodeList.push_back(pSceneMgr->getRootSceneNode());
	pSceneMgr->getRootSceneNode()->setPosition(0, 0, 0);

	// 创建每一个场景节点
	for (DWORD iNode = 0; iNode < NodeCount; ++iNode)
	{
		Ogre::SceneNode *pNode = NULL; //pSceneMgr->createSceneNode();    //  create secene node
		SceneGraphNode &SGNode = vSceneGraph[iNode];              // scene graph node
		if (SGNode.ParentIdx != -1 && (DWORD)SGNode.ParentIdx <= iNode)  // 父节点一定是已经创建过的节点
		{
			// vSceneNodeList[SGNode.ParentIdx]->addChild(pNode);    // 设置父亲节点
			pNode = vSceneNodeList[SGNode.ParentIdx]->createChildSceneNode();
		}
		if (!pNode)
		{
			continue;
		}
		vSceneNodeList.push_back(pNode);   // 

		// set node transform
		pNode->setPosition(SGNode.PosX, SGNode.PosY, SGNode.PosZ);
		pNode->setScale(SGNode.ScaleX, SGNode.ScaleY, SGNode.ScaleZ);
		pNode->setOrientation(SGNode.OrientW, SGNode.OrientX, SGNode.OrientY, SGNode.OrientZ);

		// 把节点的所有对象attach到节点上
		UINT ObjCount = SGNode.vObject.size();
		for (UINT iObj = 0; iObj < ObjCount; ++iObj)
		{
			NodeObject &Obj = SGNode.vObject[iObj];
			if (Obj.type == NBT_CAMERA) // camera node
			{
				Camera *pCamera = m_pVGSSceneMgr->GetCamera(Obj.Index);
				if (pCamera)
				{
					pNode->attachObject(pCamera);
					V3D_CameraDetail &CamDetail = m_vCameraDetail[Obj.Index];
					pCamera->setProjectionType(Ogre::ProjectionType(CamDetail.PrjType));
					pCamera->setPosition(CamDetail.FromX, CamDetail.FromY, CamDetail.FromZ);
					Ogre::Quaternion orient(CamDetail.OrientW, CamDetail.OrientX, CamDetail.OrientY, CamDetail.OrientZ);
					pCamera->setOrientation(orient);
					pCamera->setPolygonMode(Ogre::PolygonMode(CamDetail.PolygonMode));
					pCamera->setFOVy(Ogre::Radian(Real(CamDetail.fFOVY)));
					pCamera->setNearClipDistance(Ogre::Real(CamDetail.nearClip));
					pCamera->setFarClipDistance(Ogre::Real(CamDetail.farClip));
					pCamera->setDirection(CamDetail.DirectionX, CamDetail.DirectionY, CamDetail.DirectionZ);

				}
			}
			else if (Obj.type == NBT_LIGHT) // camera node
			{
				Light* pLight = m_pVGSSceneMgr->GetLight(Obj.Index);
				if (pLight)
				{
					pNode->attachObject(pLight);
					pLight->setPosition(0, 0, 0);
				}
			}
			else if (Obj.type == NBT_ENTITY) // entity
			{
				Entity *pEntity = m_pVGSSceneMgr->GetEntity(Obj.Index);
				if (pEntity)
				{
					pNode->attachObject(pEntity);
				}		
			}
		} // for obj
	} // for iNode

	DELETE_ARRAYPTR(m_pSceneGraphBuf);  // 删除字节流

	return ERR_COMMON_OK;
}*/


// 得到texture数据在文件中的范围
void CGetV3D::GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len)
{
	
	// 把所有其他位置信息从小到大排序，用texturePtr后面的位置减去texturePtr就是texure的长度
	std::vector<DWORD> v_ptrSortVector;
	
    v_ptrSortVector.push_back(table.SceneproperyPtr);
	v_ptrSortVector.push_back(table.mtrlDataPtr);
	v_ptrSortVector.push_back(table.texturePtr);
	v_ptrSortVector.push_back(table.cameraDataPtr);
	v_ptrSortVector.push_back(table.lightDataPtr);
	v_ptrSortVector.push_back(table.skeletonDataPtr);
	v_ptrSortVector.push_back(table.meshDataPtr);
	v_ptrSortVector.push_back(table.shapeDataPtr);
	v_ptrSortVector.push_back(table.sceneGraphDataPtr);
	v_ptrSortVector.push_back(table.TextDataPtr);
	v_ptrSortVector.push_back(table.AnimationPtr);
	v_ptrSortVector.push_back(table.OverlayPtr);
	v_ptrSortVector.push_back(table.ButtonPtr);
	v_ptrSortVector.push_back(table.NavigatotPtr);
	v_ptrSortVector.push_back(table.AudioPtr);
	v_ptrSortVector.push_back(table.ControlPtr);
	v_ptrSortVector.push_back(table.FlashPtr);
	v_ptrSortVector.push_back(table.PrivacyDataPtr);

	std::sort(v_ptrSortVector.begin(), v_ptrSortVector.end());
    
	std::vector<DWORD>::iterator it = v_ptrSortVector.begin();
	for (; it != v_ptrSortVector.end(); ++it)
	{
		if ((*it) == table.texturePtr)
		{
			len = (*(it + 1)) - table.texturePtr;
		   break;
		}
	}

	start = table.texturePtr;        // texture开始位置
}

// 给指定的纯文件名添加路径
Ogre::String CGetV3D::AddPathtoTextureFile(const char * sPureFileName)
{
	// 检查文件是否在指定的搜索路径下或当前资源路径下
	Ogre::String str = "";
	if (_IsFileNameInResourcePath(Ogre::String(sPureFileName), str))
	{
		return str;
	}
	return (sDefaultTexturePath + Ogre::String(sPureFileName));
}

// 更新法线贴图中的灯光句柄
VOID CGetV3D::UpdateBumpNormalmapLight()
{
	UINT count = m_pVGSSceneMgr->GetBumpmapCount();
	for (UINT i = 0; i < count; ++i)
	{
		VGSBumpmapInfo info;
		m_pVGSSceneMgr->GetBumpmapParam(i, info);
		if (info.m_Type == VBT_NORMARLMAP)
		{
			INT index = (INT)info.m_BumpNormalParam.LightHandle;
			if (index >= 0 && index < m_vpLight.size())
			{
				info.m_BumpNormalParam.LightHandle = (DWORD)m_vpLight[index];
			}
			else
			{
				info.m_BumpNormalParam.LightHandle = 0;
			}
		}
	}
}

// 处理打开完成后的一些工作
VOID CGetV3D::PostOpen()
{
	// 更新法线贴图中的灯光句柄
	UpdateBumpNormalmapLight();
}