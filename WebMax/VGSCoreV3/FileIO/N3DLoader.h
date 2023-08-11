// GetN3DFileInfo_5.h: interface for the CGetN3DFileInfo_5 class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\VGSHeader.h"
#include "N3DLoaderImpBase.h"
#include "N3DLoaderImpV2.h"
#include "N3DLoaderImpV3.h"

// 所有版本N3D文件的加载器入口
class VGS_EXPORT CN3DLoader  
{
public:

	// 加载一个N3D文件
	DWORD LoadN3DFile(CScene *pScene, const std::string &sFileName, HWND hWnd);

	// 从buffer中加载场景
	DWORD LoadSceneFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd);

	// 从buffer中加载贴图
	DWORD LoadImageDataFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd);

public:
	CN3DLoader();
	~CN3DLoader();

private:
	CN3DLoaderImpBase* getN3DLoader(const std::string &sFileName);
	CN3DLoaderImpBase* getN3DLoader(DWORD version);	


};
