#pragma once

#include "V3D_Def.h"
#include "..\VGSHeader.h"
#include "..\CScene.h"

class CGetV3DBase
{
public:
	CGetV3DBase(void);
	virtual ~CGetV3DBase(void);

public:
	virtual void Release() = 0; // �ͷ��ڴ�

	// ��V3D�ļ������ؽ�����
	virtual DWORD OpenV3DFile(CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd) = 0;

	// ��ȡV3D�ļ���TableĿ¼
	virtual bool GetV3DTable(const std::string &strFileName, V3D_Table &Table) = 0;

	// �õ�texture�������ļ��еķ�Χ, start: ��ʼλ��, len:���ݳ���
	virtual void GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len) = 0;

	virtual BOOL isMyVersion(const std::string &sFileName) = 0;  // �Ƿ�����֧�ֵİ汾
};

extern CGetV3DBase * _getV3DLoader(const string &sFileName);
