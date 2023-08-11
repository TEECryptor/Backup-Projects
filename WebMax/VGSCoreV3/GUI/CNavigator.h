/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // VGSGUINavigator.h
* 内容摘要： // 导航图类定义及实现
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

#pragma once

#include "COverlay.h"

#include <vector>

using namespace std;

//导航指针类定义
class VGS_EXPORT CNavPointer : public COverlay
{
private:
	//!0表示以相机导航，1表示用模型导航
	INT			NavType;
	//!相机或模型的指针句柄
	DWORD		PointedHandle;

	//@!航指针相对于导航图的位置
	int			m_nRelativeLeft;
	int			m_nRelativeTop;
	
public:
	CNavPointer(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName) : COverlay(pd3dDevice, sName),m_nRelativeLeft(0),m_nRelativeTop(0)
	{
		m_ObjType = VGS_GUIOBJ_TYPE_NAVIPOINTOR;
		::SetRect(&m_rc, 0, 0, 20, 20);		
	}

	inline int GetNavType() { return NavType; }
	inline void SetNavType(int navType) { NavType = navType; }
	inline DWORD GetPointerHandle() { return PointedHandle; }
	inline void SetPointerHandle(DWORD dwHandle) { PointedHandle = dwHandle; }
	inline void GetRelativePosition(int& x, int& y) { x = m_nRelativeLeft; y = m_nRelativeTop; }
	inline void SetRelativePosition(int x, int y) { m_nRelativeLeft = x; m_nRelativeTop = y; }
};

class VGS_EXPORT CNavigator : public COverlay
{
public:
	typedef vector<CNavPointer*> VecNavPointerPtr;
public:
	CNavigator(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName);
	~CNavigator();
	
	//!重载导航图的渲染
	virtual void Render();
	//!重载导航图的矩形更新——主要用于实现导航图更新时也更新导航点
	virtual HRESULT UpdateRect(const RECT& rc);

	bool IsExist(const std::string &sVGSName);
	// 添加/删除导航点
	// 添加/删除导航点
	void AddNavPointer(CNavPointer* ptr);
	CNavPointer* AddNavPointer(const std::string &sName, const std::string &sFileName = "", INT pointerType = 0, DWORD pointedHandle = 0);
	CNavPointer* CloneNavPointer(const std::string &sName, const std::string &sReferencePointer);
	void deleteNavPointer(const std::string &sName);

	//!获得导航点数量
	inline int  GetPointerCount() { return m_vNavPointer.size(); }
	//!通过索引获取导航点对象的指针
	CNavPointer* GetPointerByIndex(int idx);
	//!通过索引获取导航点对象的名称
	std::string GetPointerName(INT index);
	//!通过句柄获取导航点对象的指针
	CNavPointer* GetPointerByHandle(DWORD dwHandle);
	//!通过名称获取导航点对象的指针
	CNavPointer* GetPointerByName(const std::string& name);

	// 更新导航范围
	VOID SetNavRange(const FRect& rc);
	inline VOID GetNavRange(FRect& rc) { rc = m_rcNavScene; }

	// 更新导航地图
	VOID SetNavMap(const std::string &sNavImageFile);
	// 得到导航背景图使用的图像名称
	std::string GetNavMap();

	// 更新导航点位置
	VOID UpdatePointers();
	
	void UpdateMap(RECT& rc);
protected:
	//!导航点指针的向量
	VecNavPointerPtr m_vNavPointer;
	//!场景的导航范围
	FRect m_rcNavScene;

};