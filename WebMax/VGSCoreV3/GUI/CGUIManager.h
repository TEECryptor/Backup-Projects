/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // VGSGUIManager.h
* 内容摘要： // VGS3GUI的管理类声明,通过调用QuickGUIStore的方法实现
* 其它说明： // 
* 当前版本： // 
* 作    者： // 
* 完成日期： // 
* 
* 修改日期        版本号     修改人	      修改内容
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#ifndef _CGUIMANAGER_H_
#define _CGUIMANAGER_H_

#include "..\VGSHeader.h"
#include "..\VGSGetRoot.h"
#include "COverlay.h"
#include "CButton.h"
#include "CNavigator.h"
#include "background.h"

#include <algorithm>
#include <vector>
#include <sstream>
#include "MySTL.h"

#include "..\csysfunc.h"

using namespace std;

// GUI的属性
typedef struct GUIInfo
{
	DWORD           pWidget;		// 对象的指针
	CHAR			sName[MAX_NAMESTR_LENGTH];			// GUI对象的名称
	VGS_GUIOBJ_TYPE guiType;        // gui对象的类型

	FLOAT			Rotation;       // 旋转角度
	int             Opacity;
	bool            bVisible;
 
	bool            bRelativeSize;  // 是否使用相对尺寸
	int           Width;            // 图像的宽度
	int           Height;           // 图像的高度

	VGSOFFSETORIGIN    Origin;			// 对齐时的参考原点，OFFSETORIGIN值 
	bool            bRelativePos;   // 是否使用相对位置
	int           Left;			    // 横向坐标，相对于视口Origin点
	int           Top;			    // 纵向标，相对于视口Origin点
	
	int             ZOrder;         // GUI对象的层次，值越大越在上层

}GUIInfo;


class VGS_EXPORT CGUIManager : public CVGSGetRoot
{
public:
	typedef vector<CGUIObject*> VecGuiPtr;
public:
	CGUIManager(LPDIRECT3DDEVICE9 pd3dDevice);
	~CGUIManager();

	void InitGUIStatus(int nVpWidth, int nVpHeight);
	// 清空场景内容
	void Clear();
	//!是否有GUI对象(除背景图外).
	inline bool HasObjects() { return !m_vecGui.empty(); }
	//!是否有背景图
	inline bool HasBackDrop() { return (m_pGuiBackgroud != 0); }

	/** 更新渲染所有GUI对象（除背景图）.
	*/
	void UpdateLayout();
	/** 更新渲染背景图.
	*/
	void RenderBackDrop();

	/** 通过指针句柄获取GUI对象.
	@param hGui in:GUI对象的句柄.
	@return 返回句柄对应的GUI对象.
	*/
	CGUIObject* GetObjectByHandle(DWORD hGui);
	/** 得到光标下方的GUI对象句柄及类型
	@param x/y  : 视口中的坐标
	@param type out ： GUI对象的类型，参考VGS_GUIOBJ_TYPE定义
	@return GUI对象的指针
	*/
	CGUIObject* GetGUIObjectUnderLoc(int x, int y, VGS_GUIOBJ_TYPE &type);
	//!获取背景图指针
	inline BackGround* GetBackDropPtr() { return m_pGuiBackgroud;}
	//!删除背景图
	inline void RemoveBackDrop() {	SAFE_DELETE(m_pGuiBackgroud); }

		
	/////////////////// 公用方法 ///////////////////////////////////

	// 获取二维对象的公用属性
	void GetObjectCommonPropety(CGUIObject* pWidget, GUIInfo &info);

	/** 获取所有已经创建的二维元素(除背景图外)的句柄列表.
	@param vHandle out: GUI对象的结果向量.
	*/
	//void GetAllObjectsList(vector <CGUIObject*> &vHandle);
	vector<CGUIObject*>& GetAllObjectsList();
	/** 获取所有已经创建的二维元素(除背景图外)的数量.
	@return:返回GUI对象(除背景图外)的个数.
	*/
	inline UINT GetAllObjectsCount() { return m_vecGui.size(); }
	/** 获取某种类型二维元素的句柄列表.
	@param type in: GUI对象的类型.
	@param vHandle out: GUI对象的结果向量.
	*/
	void GetObjectsList(VGS_GUIOBJ_TYPE type, vector <CGUIObject*> &vHandle);
	/** 获取某种类型的GUI对象的数量.
	@param type in: GUI对象的类型.
	@return:返回该类型GUI对象的个数.
	*/
	UINT GetObjectsCount(VGS_GUIOBJ_TYPE type);


	// 获取某种类型二维元素的句柄
	CGUIObject* GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE type, UINT Index);
	// 根据名字获取句柄
	CGUIObject* GetObjectHandleByName(const std::string &sName);

	// 返回GUI对象的ZOrder，越大表示GUI对象处于越上层
	int GetObjectZOrder(CGUIObject* pWidget);	

	// GUI对象上移一层
	void MoveObjectUp(CGUIObject* pWidget);

	// GUI对象下移一层
	void MoveObjectDown(CGUIObject* pWidget);

	// GUI对象移动到最顶层
	void GotoObjectsTop(CGUIObject* pWidget);

	// GUI对象移动到最底层
	void GotoObjectsBottom(CGUIObject* pWidget);


	// 指定名称的二维对象是否已经存在。sVGSName：对象的VGS名称，并不是quickgui内部名称
	bool IsObjectExist(const std::string &sVGSName);


	////////////////////// end of 公用方法 //////////////////////////////////
	// 删除一个组件. Widget:组件的指针
	void RemoveObject(CGUIObject* pWidget);
	/** 创建一个前景图
	@param szName in:前景图名称.
	@param szBaseImg in:基本图像的路径.
	@param szOpacImg in:透明图像的路径.
	@return: 重名或创建失败返回0.
	*/
	CGUIObject* CreateOverLayer(const char* szName, const char* szBaseImg = "", const char* szOpacImg = "");

	// 创建按钮
	CGUIObject* CreateButton(const char* sName, const RECT &rect, 
						const char* mouseUpImageFile = "",
						const char* mouseOverImageFile = "",
						const char* mouseDownImageFile = "");
	
	// 创建导航器
	CGUIObject* CreateNavigator(const char* szName = "", const char* szBaseImg = "");

	// 创建一个导航点
	CGUIObject* CreateNaviPointer(const char* szName, const char* szBaseImg = "");

	// 创建一个背景图, szBaseImg:图片文件
	BackGround* CreateBackGroudLayer(const char* szBaseImg = "");
	/**创建一个Flash的GUI对象.
	@param hParent in: Flash窗口的父窗体的句柄.
	@param sName in: GUI对象的名称.
	@param sFile in: swf文件的完全路径.
	@param rect in:要创建的Flash GUI的区域大小.
	*/
	CATLFlashCtrl* CreateFlashCtrl(HWND hParent, const char* sName, const char* sFile, const RECT &rect);
	
	// 获取GUIManager使用的场景
	DWORD GetSceneManager();
	// 设置GUIManager使用的场景
	void SetSceneManager(DWORD sceneManager);

	// 设置视口大小
	void SetViewportSize(int width, int height);
	void GetViewportSize(int &width, int &height);

	/** 生成一个GUI对象的材质名称.
	@param szGuiName in:GUI对象的名称
	@return :产生的GUI对象的材质名.
	*/
	// static std::string GenerateGuiMtrlName(const char* szGuiName);

private:
	// d3d device
	LPDIRECT3DDEVICE9		m_pd3dDevice;  
	// 视口的宽度
	int           m_vpWidth;   
	// 视口的高度
	int           m_vpHeight;     
	//场景CScene对象的指针
	DWORD m_SceneManager;

	//背景图对象
	BackGround*	m_pGuiBackgroud;
	//除背景图外的所有GUI对象.
	VecGuiPtr m_vecGui;
};

// extern CGUIManager *g_pVGSGUIManager;


#endif