#pragma once

#include "..\\VGSHeader.h"
#include "..\\CScene.h"
#include "..\VGSErrCode.h"
#include "N3D_Def.h"

// 从Offset位置拷贝几个字节, 同时Offset指针先后移动sizeof(value)
#define CPY_FROM_STREAM(value, Offset)   {memcpy(&value, Offset, sizeof(value)); Offset += sizeof(value);} 
#define CPY_FROM_STREAM_EX(ptr, len, Offset)   {memcpy(ptr, Offset, len); Offset += len;} 

// 从offset位置拷贝一个vector3数据结构
#define CPY_VECTOR3_FROM_STRRAM(V3, Offset)\
{\
	CPY_FROM_STREAM((V3.x), Offset)\
	CPY_FROM_STREAM((V3.y), Offset)\
	CPY_FROM_STREAM((V3.z), Offset)\
}

// n3d 加载器实现类的基类
class VGS_EXPORT CN3DLoaderImpBase
{
public:

	// 加载一个N3D文件
	virtual DWORD LoadN3DFile(CScene *pScene, const std::string &sFileName, HWND hWnd) = 0;

	// 从buffer中加载场景
	virtual DWORD LoadSceneFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd) = 0;

	// 从buffer中加载贴图
	virtual DWORD LoadImageDataFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd) = 0;

public:

	CN3DLoaderImpBase()
	{
		m_minVersion = 0;
		m_maxVersion = 0;
	}
	virtual ~CN3DLoaderImpBase()
	{

	}

	// 是否支持该版本号的N3D文件
	BOOL IsSupportThisVersion(DWORD version, DWORD &errCode)
	{
		if (version < m_minVersion)
		{
			errCode = ERR_FILE_N3D_VERSIONLOW;
			return FALSE;
		}
		else if (version > m_maxVersion)
		{
			errCode = ERR_FILE_N3D_VERSIONHIGH;
			return FALSE;
		}
		return TRUE;
	}

protected:
	DWORD m_minVersion;  // 支持的最小版本号和最大版本号
	DWORD m_maxVersion;

	virtual void ReadHeader()    = 0;
	virtual void ReadFileTable() = 0;
	virtual void ReadSceneProp() = 0;
	virtual void ReadPrivacyData() = 0;
	virtual void ReadCameraData() = 0;
	virtual void ReadAudioData() = 0;
	virtual void ReadFlashData() = 0;
	virtual void ReadLightData() = 0;
	virtual void ReadMaterialData() = 0;
	virtual void ReadModelData() = 0;
	virtual void ReadModelGroupData() = 0;
	virtual void ReadTextData() = 0;
	virtual void ReadOverlayData() = 0;
	virtual void ReadNavigaterData() = 0;
	virtual void ReadButtonData() = 0;
	virtual void ReadControlData() = 0;
	virtual void ReadSceneGraph() = 0;	//读入场景图//读取图像信息
	virtual void ReadImageData() = 0;
};