/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // VGSGUIManager.h
* 内容摘要： // VGS3GUI的管理类的实现,通过调用QuickGUIStore的方法实现
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
#include "CGUIManager.h"
#include "..\CMaterial.h"
#include "..\Root.h"
#include "ATLFlashCtrlUnit.h"

#include <algorithm>

using namespace std;

CGUIManager::CGUIManager(LPDIRECT3DDEVICE9 pd3dDevice):m_pd3dDevice(pd3dDevice), m_pGuiBackgroud(0)
{
	 m_vecGui.clear();
}

CGUIManager::~CGUIManager()
{	
	Clear();
}
void CGUIManager::InitGUIStatus(int nVpWidth, int nVpHeight)
{
	Clear();
	m_vpWidth = nVpWidth;
	m_vpHeight = nVpHeight;
}

// 清空场景内容
void CGUIManager::Clear()
{
	SAFE_DELETE(m_pGuiBackgroud);
	VecGuiPtr::iterator it = m_vecGui.begin(), end = m_vecGui.end();
	for (; it != end; ++it)
		SAFE_DELETE(*it);
	m_vecGui.clear();
}

// 设置GUIManager使用的场景
void CGUIManager::SetSceneManager(DWORD sceneManager)
{
	m_SceneManager = sceneManager;
}

// 获取GUIManager使用的场景
DWORD CGUIManager::GetSceneManager()
{
	return m_SceneManager;
}

// 设置视口大小
void CGUIManager::SetViewportSize(INT width, INT height)
{
	if (0 == m_vpWidth || 0 == m_vpHeight)
		return;
	m_vpWidth  = width;
	m_vpHeight = height;
	//更新所有GUI...
	VecGuiPtr::iterator it = m_vecGui.begin(), end = m_vecGui.end();
	for ( ; it != end; ++it)
	{
		if (*it)
		{
			(*it)->SetViewportSize(m_vpWidth, m_vpHeight);
		}
	}
	//更新背景图...
	if (m_pGuiBackgroud)
	{
		m_pGuiBackgroud->SetSize(m_vpWidth, m_vpHeight);
	}
}

void CGUIManager::GetViewportSize(INT &width, INT &height)
{
	width  = m_vpWidth;
	height = m_vpHeight;
}
// 获取二维对象的公用属性
void CGUIManager::GetObjectCommonPropety(CGUIObject* pWidget, GUIInfo &info)
{
	ZeroMemory(&info, sizeof(info));

	CGUIObject *pObj = (CGUIObject *)pWidget;
	if (pObj)
	{
		std::string str = pObj->GetName();
		COPY_NAME(info.sName, str.c_str());

		info.bRelativePos  = pObj->GetUsedRelativePos();
		info.bRelativeSize = pObj->GetUsedRelativeSize();
		info.bVisible        = pObj->GetVisibility();
		info.guiType         = pObj->GetType();
		pObj->GetSize(info.Width, info.Height);
		pObj->GetPosition(info.Left, info.Top);
		info.Opacity         = pObj->GetOpacity();
		info.Origin          = pObj->GetOffsetOrigin();
		info.pWidget         = (DWORD)pObj;
		// info.ZOrder          = GetObjectZOrder(pWidget);
	}
}
/** 得到光标下方的GUI对象句柄及类型,如果有多个对象，返回最上层（ZOrder最大）的一个GUI对象
@param x/y  : 视口中的坐标
@param type out ： GUI对象的类型，参考VGS_GUIOBJ_TYPE定义
@return GUI对象的指针
*/
CGUIObject* CGUIManager::GetGUIObjectUnderLoc(INT x, INT y, VGS_GUIOBJ_TYPE &type)
{
	//先检测GUI
	VecGuiPtr::reverse_iterator r_it = m_vecGui.rbegin();
	for ( ; r_it != m_vecGui.rend(); ++r_it)
	{
		CGUIObject *pObj = (CGUIObject*)(*r_it);
		if (pObj && pObj->IsInRect(x, y))
		{
			type = pObj->GetType();
			return pObj;
		}
	}
	//再检测背景图
	if (m_pGuiBackgroud && m_pGuiBackgroud->IsInRect(x, y))
	{
		type = m_pGuiBackgroud->GetType();
		return m_pGuiBackgroud;
	}
	return NULL;
}

// 获取某种类型二维元素的句柄列表
void CGUIManager::GetObjectsList(VGS_GUIOBJ_TYPE type, vector <CGUIObject*> &vHandle)
{
	vHandle.clear();
	MyStd::copy_if(m_vecGui.begin(), m_vecGui.end(),vHandle.begin(), GuiTypeFunctor(type));
}

UINT CGUIManager::GetObjectsCount(VGS_GUIOBJ_TYPE type)
{
	return (UINT)(std::count_if(m_vecGui.begin(), m_vecGui.end(), GuiTypeFunctor(type)));
}

// 获取所有已经创建的二维元素的句柄列表
vector<CGUIObject*>& CGUIManager::GetAllObjectsList()
{
	return m_vecGui;
}

// 获取某种类型二维元素的句柄
CGUIObject* CGUIManager::GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE type, UINT Index)
{
// 	VecGuiPtr vecObj;
// 	MyStd::copy_if(m_vecGui.begin(), m_vecGui.end(),vecObj.begin(), GuiTypeFunctor(type));
// 	if (Index < vecObj.size() )
// 		return vecObj[Index];
// 	else
// 		return 0;
	UINT icount = 0;
	vector<CGUIObject*>::iterator it = m_vecGui.begin(), end = m_vecGui.end();
	for ( ; it != end; ++it)
	{
		if ((*it)->GetType() == type)
		{
			if (icount++ == Index)
			{
				return (*it);
			}
		}
	}
	return 0;
}

// 根据名字获取句柄
CGUIObject* CGUIManager::GetObjectHandleByName(const std::string &sName)
{
	if (sName.empty())
		return 0;
	vector<CGUIObject*>::iterator it = m_vecGui.begin(), end = m_vecGui.end();
	for ( ; it != end; ++it)
	{
		if (*it && sName == (*it)->GetName())
		{
			return (*it);
		}
	}
	return 0;
}
CGUIObject* CGUIManager::GetObjectByHandle(DWORD hGui)
{
	CGUIObject* ptr = (CGUIObject*)hGui;
	VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), ptr);
	if (it != m_vecGui.end())
		return *it;
	else
		return 0;
}
//std::string CGUIManager::GenerateGuiMtrlName(const char* szGuiName)
//{
//	std::string strMtrl = szGuiName;
//	strMtrl += "_mtrl";
//	int idx = 0;
//	std::ostringstream os;
//	os<<idx;
//	while (CMaterialManager::getSingleton().IsResourceExist(strMtrl + os.str()))
//	{
//		os.clear();
//		idx++;
//		os<<idx;
//	}
//	return strMtrl;
//}

// 指定名称的二维对象是否已经存在。sVGSName：对象的VGS名称
bool CGUIManager::IsObjectExist(const std::string &sVGSName)
{
	VecGuiPtr::iterator it = std::find_if(m_vecGui.begin(), m_vecGui.end(), GuiNameFunctor(sVGSName));
	return (it != m_vecGui.end());
}

// 删除一个组件
void CGUIManager::RemoveObject(CGUIObject* pWidget)
{
	if (!pWidget)
		return;
	VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
	if (it != m_vecGui.end())
	{
		//需要从Mtrl Manager中删除材质....
		(*it)->GetMaterial()->Release();
		m_vecGui.erase(it);
		SAFE_DELETE(pWidget);
	}
}

CGUIObject* CGUIManager::CreateButton(const char* szName, 
						 const RECT &rect, 
						 const char* mouseUpImageFile,
						 const char* mouseOverImageFile,
						 const char* mouseDownImageFile)
{
	// 如果同名对象已经存在，返回false
	if (IsObjectExist(szName))
	{
		return 0;
	}

	// 创建按钮
	CButton *pButton = new CButton(m_pd3dDevice, szName);
	if (pButton)
	{		
		pButton->SetRoot(m_pRoot);
		std::string strImg[3] = {"", "", ""};
		int ncount = 0;
		if (mouseUpImageFile)
		{
			strImg[0]= mouseUpImageFile;
			ncount++;
		}
		if (mouseDownImageFile)
		{
			strImg[1] = mouseDownImageFile; 
			ncount++;
		}
		if (mouseOverImageFile)
		{
			strImg[2] = mouseOverImageFile;
			ncount++;
		}
		for (int i = 0; i < ncount; i++)
		{
			if (strImg[i].length() > 0)//material..
			{
				std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(szName);
				CMaterial* pMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
				if (pMtrl)
				{
					pMtrl->createTextureLayer(strImg[i], strMtrl + "_txt0", 0);
					pButton->SetMaterial(i, pMtrl);
				}
			}
		}
		pButton->SetType(VGS_GUIOBJ_TYPE_BUTTON);
		pButton->SetViewportSize(m_vpWidth, m_vpHeight);
		m_vecGui.push_back(pButton);
		return pButton;
	}
	return 0;
}


// 创建导航器,指定背景图/导航点的图片文件名，指定导航图在世界中导航的范围，ret是顶视图看到的平面范围
// 鼠标点击导航器上的点,向hWnd发送位置消息
CGUIObject* CGUIManager::CreateNavigator(const char* szName, const char* szBaseImg)
{
	// 如果同名对象已经存在，返回false
	if (IsObjectExist(szName))
	{
		return 0;
	}

	// 创建按钮
	CNavigator *pNavigator = new CNavigator(m_pd3dDevice, szName);	
	if (pNavigator)
	{
		pNavigator->SetRoot(m_pRoot);
		std::string strImg = szBaseImg;
		if (strImg.length() > 0)//material..
		{
			std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(szName);
			CMaterial* pMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
			pMtrl->createTextureLayer(strImg, strMtrl + "_txt0", 0);
			pNavigator->SetMaterial(pMtrl);
		}
		pNavigator->SetViewportSize(m_vpWidth, m_vpHeight);
		//导航图总是在最前面，且后创建的在最前面
		m_vecGui.push_back(pNavigator);
		GotoObjectsTop(pNavigator);
		return pNavigator;
	}

	return 0;
}

// 创建一个导航点
CGUIObject* CGUIManager::CreateNaviPointer(const char* szName, const char* szBaseImg)
{
	// 如果同名对象已经存在，返回false
	if (IsObjectExist(szName))
	{
		return 0;
	}

	// 创建按钮
	CNavPointer *pNaviPointer = new CNavPointer(m_pd3dDevice, szName);	
	if (pNaviPointer)
	{
		pNaviPointer->SetRoot(m_pRoot);
		std::string strImg = szBaseImg;
		if (strImg.length() > 0)//material..
		{
			std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(szName);
			CMaterial* pMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
			pMtrl->createTextureLayer(strImg, strMtrl + "_txt0", 0);
			pNaviPointer->SetMaterial(pMtrl);
		}
		pNaviPointer->SetViewportSize(m_vpWidth, m_vpHeight);
		m_vecGui.push_back(pNaviPointer);
	}
	return pNaviPointer;
}

// 创建一个前景图, ImgName:图片文件
CGUIObject* CGUIManager::CreateOverLayer(const char* szName, const char* szBaseImg, const char* szOpacImg)
{
	// 如果同名对象已经存在，返回false
	if (IsObjectExist(szName) || !m_pd3dDevice)
	{
		return 0;
	}
	COverlay *pOverlay = new COverlay(m_pd3dDevice, szName);
	if (pOverlay)
	{		
		pOverlay->SetRoot(m_pRoot);
		std::string strImg = szBaseImg;
		if (strImg.length() > 0)//material..
		{
			std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(szName);
			CMaterial* pMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
			pMtrl->createTextureLayer(strImg, strMtrl + "_txt0", 0);
			std::string strOpacity = szOpacImg;
			if (strOpacity.length() > 0)
			{
				pMtrl->createOpacityTextureLayer(strOpacity, strMtrl + "txt1", 0);
			}
			pOverlay->SetMaterial(pMtrl);
		}
		pOverlay->SetType(VGS_GUIOBJ_TYPE_OVERLAY);
		pOverlay->SetViewportSize(m_vpWidth, m_vpHeight);
		m_vecGui.push_back(pOverlay);
		return pOverlay;
	}
	return 0;
}

// 创建一个背景图, ImgName:图片文件
BackGround* CGUIManager::CreateBackGroudLayer(const char* szBaseImg)
{
	if (m_pGuiBackgroud)//如果已创建则直接返回
		return m_pGuiBackgroud;
	m_pGuiBackgroud = new BackGround(m_pd3dDevice);
	if (m_pGuiBackgroud)
	{		
		m_pGuiBackgroud->SetRoot(m_pRoot);
		std::string strImg = szBaseImg;
		if (strImg.length() > 0)//material..
		{
			std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(m_pGuiBackgroud->GetName());
			CMaterial* pMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
			pMtrl->createTextureLayer(strImg, strMtrl + "_txt0", 0);
			m_pGuiBackgroud->SetMaterial(pMtrl);
		}
		m_pGuiBackgroud->SetViewportSize(m_vpWidth, m_vpHeight);
		m_pGuiBackgroud->SetSize(m_vpWidth, m_vpHeight);
		return m_pGuiBackgroud;
	}
	return 0;
}
CATLFlashCtrl* CGUIManager::CreateFlashCtrl(HWND hParent, const char* sName, const char* sFile, const RECT &rect)
{
	// 如果同名对象已经存在，返回false
	if (IsObjectExist(sName))
	{
		return 0;
	}
	CATLFlashCtrl *pFlashGui = new CATLFlashCtrl(sName, hParent, NULL);
	if (pFlashGui)
	{		
		//flash file 
		_bstr_t bst(sFile);

		pFlashGui->Create(rect, bst.copy());
		pFlashGui->Play(1);


// 		pFlashGui->SetViewportSize(m_vpWidth, m_vpHeight);
		m_vecGui.push_back(pFlashGui);
		return pFlashGui;
	}
	return 0;
}


//应改进GUI渲染——以材质设置为原则，并分透明与不透明
void CGUIManager::UpdateLayout()
{
	//render gui
	VecGuiPtr::iterator it = m_vecGui.begin(), end = m_vecGui.end();
	for ( ; it != end; ++it)
	{
		(*it)->Render();
	}
	//clear texture
	for (int i = 0; i < 8; i++)
	{
		m_pd3dDevice->SetTexture(i, NULL);
		m_pd3dDevice->SetTextureStageState(i, D3DTSS_COLOROP, D3DTOP_DISABLE);
		m_pd3dDevice->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, 0);
	}
	//render select box
	it = m_vecGui.begin(), end = m_vecGui.end();
	for ( ; it != end; ++it)
	{
		if ((*it)->GetShowSelBox())
			(*it)->RenderSelBox();
	}
}
void CGUIManager::RenderBackDrop()
{
	if (m_pGuiBackgroud)
	{
		m_pGuiBackgroud->Render();
	}
}
// 返回GUI对象的ZOrder，越大表示GUI对象处于越上层
// 实际上返回的是Widget在m_vZOrder中的索引
INT CGUIManager::GetObjectZOrder(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			return it - m_vecGui.begin();
		}
	}
	return 0;
}

// GUI对象上移一层
// 实现过程：把m_vZOrder中Widget前的对象依次移动到最顶层，然后把Widget后面的一个对象移动到最顶层，
// 然后再把Widget移动到最顶层，最后把Widget后第二个对象知道队列末尾的所有对象依次移动到最顶层 
void CGUIManager::MoveObjectUp(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			if (it == (m_vecGui.end() - 1))//已经在最顶层
				return;
			else
				std::iter_swap(it, it +1);
		}
	}
}

// GUI对象下移一层
// 实现过程：把m_vZOrder中Widget前第二个对象及其之前的对象依次移动到最顶层，然后把Widget移动到最顶层，
// 然后再把Widget之前的一个对象移动到最顶层，最后把Widget之后的所有对象依次移动到最顶层 
void CGUIManager::MoveObjectDown(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			if (it == m_vecGui.begin())//已经在最底层
				return;
			else
				std::iter_swap(it, it - 1);
		}
	}
}

// GUI对象移动到最顶层
void CGUIManager::GotoObjectsTop(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			if (it == (m_vecGui.end() - 1))//已经在最顶层
				return;
			else
				std::iter_swap(it, m_vecGui.end() - 1);
		}
	}
}

// GUI对象移动到最底层
void CGUIManager::GotoObjectsBottom(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			if (it == m_vecGui.begin())//已经在最底层
				return;
			else
				std::iter_swap(it, m_vecGui.begin());
		}
	}
}
