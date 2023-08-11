#include "N3DLoader.h"

CN3DLoader::CN3DLoader()
{

}
CN3DLoader::~CN3DLoader()
{

}

// 加载一个N3D文件
DWORD CN3DLoader::LoadN3DFile(CScene *pScene, const std::string &sFileName, HWND hWnd)
{
	return 0;
}

// 从buffer中加载场景
DWORD CN3DLoader::LoadSceneFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd)
{
	CN3DLoaderImpBase* pN3DLoader = getN3DLoader(version);

	if (pN3DLoader)
	{
		return pN3DLoader->LoadSceneFromBuffer(pScene, version, pBuffer, hWnd);
	}

	return ERR_FILE_N3D_LOAD;
}

// 从buffer中加载贴图
DWORD CN3DLoader::LoadImageDataFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd)
{
	CN3DLoaderImpBase* pN3DLoader = getN3DLoader(version);

	if (pN3DLoader)
	{
		return pN3DLoader->LoadImageDataFromBuffer(pScene, version, pBuffer, hWnd);
	}
	return ERR_FILE_N3D_LOAD;

}

CN3DLoaderImpBase* CN3DLoader::getN3DLoader(const std::string &sFileName)
{
	return 0;
}
CN3DLoaderImpBase* CN3DLoader::getN3DLoader(DWORD version)
{
	DWORD errCode;
	CN3DLoaderImpV3 impV3;
	if (impV3.IsSupportThisVersion(version, errCode))
	{
		return new CN3DLoaderImpV3;
	}

	CN3DLoaderImpV2 impV2;
	if (impV2.IsSupportThisVersion(version, errCode))
	{
		return new CN3DLoaderImpV2;
	}
	return 0;
}