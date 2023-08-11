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
	virtual void Release() = 0; // 释放内存

	// 打开V3D文件并加载进场景
	virtual DWORD OpenV3DFile(CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd) = 0;

	// 获取V3D文件的Table目录
	virtual bool GetV3DTable(const std::string &strFileName, V3D_Table &Table) = 0;

	// 得到texture数据在文件中的范围, start: 开始位置, len:数据长度
	virtual void GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len) = 0;

	virtual BOOL isMyVersion(const std::string &sFileName) = 0;  // 是否是我支持的版本
};

extern CGetV3DBase * _getV3DLoader(const string &sFileName);
