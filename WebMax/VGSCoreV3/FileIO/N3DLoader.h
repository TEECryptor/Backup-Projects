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

// ���а汾N3D�ļ��ļ��������
class VGS_EXPORT CN3DLoader  
{
public:

	// ����һ��N3D�ļ�
	DWORD LoadN3DFile(CScene *pScene, const std::string &sFileName, HWND hWnd);

	// ��buffer�м��س���
	DWORD LoadSceneFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd);

	// ��buffer�м�����ͼ
	DWORD LoadImageDataFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd);

public:
	CN3DLoader();
	~CN3DLoader();

private:
	CN3DLoaderImpBase* getN3DLoader(const std::string &sFileName);
	CN3DLoaderImpBase* getN3DLoader(DWORD version);	


};
