/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSGUIManager.h
* ����ժҪ�� // VGS3GUI�Ĺ������ʵ��,ͨ������QuickGUIStore�ķ���ʵ��
* ����˵���� // 
* ��ǰ�汾�� // 
* ��    �ߣ� // 
* ������ڣ� // 
* 
* �޸�����        �汾��     �޸���	      �޸�����
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

// ��ճ�������
void CGUIManager::Clear()
{
	SAFE_DELETE(m_pGuiBackgroud);
	VecGuiPtr::iterator it = m_vecGui.begin(), end = m_vecGui.end();
	for (; it != end; ++it)
		SAFE_DELETE(*it);
	m_vecGui.clear();
}

// ����GUIManagerʹ�õĳ���
void CGUIManager::SetSceneManager(DWORD sceneManager)
{
	m_SceneManager = sceneManager;
}

// ��ȡGUIManagerʹ�õĳ���
DWORD CGUIManager::GetSceneManager()
{
	return m_SceneManager;
}

// �����ӿڴ�С
void CGUIManager::SetViewportSize(INT width, INT height)
{
	if (0 == m_vpWidth || 0 == m_vpHeight)
		return;
	m_vpWidth  = width;
	m_vpHeight = height;
	//��������GUI...
	VecGuiPtr::iterator it = m_vecGui.begin(), end = m_vecGui.end();
	for ( ; it != end; ++it)
	{
		if (*it)
		{
			(*it)->SetViewportSize(m_vpWidth, m_vpHeight);
		}
	}
	//���±���ͼ...
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
// ��ȡ��ά����Ĺ�������
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
/** �õ�����·���GUI������������,����ж�����󣬷������ϲ㣨ZOrder��󣩵�һ��GUI����
@param x/y  : �ӿ��е�����
@param type out �� GUI��������ͣ��ο�VGS_GUIOBJ_TYPE����
@return GUI�����ָ��
*/
CGUIObject* CGUIManager::GetGUIObjectUnderLoc(INT x, INT y, VGS_GUIOBJ_TYPE &type)
{
	//�ȼ��GUI
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
	//�ټ�ⱳ��ͼ
	if (m_pGuiBackgroud && m_pGuiBackgroud->IsInRect(x, y))
	{
		type = m_pGuiBackgroud->GetType();
		return m_pGuiBackgroud;
	}
	return NULL;
}

// ��ȡĳ�����Ͷ�άԪ�صľ���б�
void CGUIManager::GetObjectsList(VGS_GUIOBJ_TYPE type, vector <CGUIObject*> &vHandle)
{
	vHandle.clear();
	MyStd::copy_if(m_vecGui.begin(), m_vecGui.end(),vHandle.begin(), GuiTypeFunctor(type));
}

UINT CGUIManager::GetObjectsCount(VGS_GUIOBJ_TYPE type)
{
	return (UINT)(std::count_if(m_vecGui.begin(), m_vecGui.end(), GuiTypeFunctor(type)));
}

// ��ȡ�����Ѿ������Ķ�άԪ�صľ���б�
vector<CGUIObject*>& CGUIManager::GetAllObjectsList()
{
	return m_vecGui;
}

// ��ȡĳ�����Ͷ�άԪ�صľ��
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

// �������ֻ�ȡ���
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

// ָ�����ƵĶ�ά�����Ƿ��Ѿ����ڡ�sVGSName�������VGS����
bool CGUIManager::IsObjectExist(const std::string &sVGSName)
{
	VecGuiPtr::iterator it = std::find_if(m_vecGui.begin(), m_vecGui.end(), GuiNameFunctor(sVGSName));
	return (it != m_vecGui.end());
}

// ɾ��һ�����
void CGUIManager::RemoveObject(CGUIObject* pWidget)
{
	if (!pWidget)
		return;
	VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
	if (it != m_vecGui.end())
	{
		//��Ҫ��Mtrl Manager��ɾ������....
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
	// ���ͬ�������Ѿ����ڣ�����false
	if (IsObjectExist(szName))
	{
		return 0;
	}

	// ������ť
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


// ����������,ָ������ͼ/�������ͼƬ�ļ�����ָ������ͼ�������е����ķ�Χ��ret�Ƕ���ͼ������ƽ�淶Χ
// ������������ϵĵ�,��hWnd����λ����Ϣ
CGUIObject* CGUIManager::CreateNavigator(const char* szName, const char* szBaseImg)
{
	// ���ͬ�������Ѿ����ڣ�����false
	if (IsObjectExist(szName))
	{
		return 0;
	}

	// ������ť
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
		//����ͼ��������ǰ�棬�Һ󴴽�������ǰ��
		m_vecGui.push_back(pNavigator);
		GotoObjectsTop(pNavigator);
		return pNavigator;
	}

	return 0;
}

// ����һ��������
CGUIObject* CGUIManager::CreateNaviPointer(const char* szName, const char* szBaseImg)
{
	// ���ͬ�������Ѿ����ڣ�����false
	if (IsObjectExist(szName))
	{
		return 0;
	}

	// ������ť
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

// ����һ��ǰ��ͼ, ImgName:ͼƬ�ļ�
CGUIObject* CGUIManager::CreateOverLayer(const char* szName, const char* szBaseImg, const char* szOpacImg)
{
	// ���ͬ�������Ѿ����ڣ�����false
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

// ����һ������ͼ, ImgName:ͼƬ�ļ�
BackGround* CGUIManager::CreateBackGroudLayer(const char* szBaseImg)
{
	if (m_pGuiBackgroud)//����Ѵ�����ֱ�ӷ���
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
	// ���ͬ�������Ѿ����ڣ�����false
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


//Ӧ�Ľ�GUI��Ⱦ�����Բ�������Ϊԭ�򣬲���͸���벻͸��
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
// ����GUI�����ZOrder��Խ���ʾGUI������Խ�ϲ�
// ʵ���Ϸ��ص���Widget��m_vZOrder�е�����
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

// GUI��������һ��
// ʵ�ֹ��̣���m_vZOrder��Widgetǰ�Ķ��������ƶ�����㣬Ȼ���Widget�����һ�������ƶ�����㣬
// Ȼ���ٰ�Widget�ƶ�����㣬����Widget��ڶ�������֪������ĩβ�����ж��������ƶ������ 
void CGUIManager::MoveObjectUp(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			if (it == (m_vecGui.end() - 1))//�Ѿ������
				return;
			else
				std::iter_swap(it, it +1);
		}
	}
}

// GUI��������һ��
// ʵ�ֹ��̣���m_vZOrder��Widgetǰ�ڶ���������֮ǰ�Ķ��������ƶ�����㣬Ȼ���Widget�ƶ�����㣬
// Ȼ���ٰ�Widget֮ǰ��һ�������ƶ�����㣬����Widget֮������ж��������ƶ������ 
void CGUIManager::MoveObjectDown(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			if (it == m_vecGui.begin())//�Ѿ�����ײ�
				return;
			else
				std::iter_swap(it, it - 1);
		}
	}
}

// GUI�����ƶ������
void CGUIManager::GotoObjectsTop(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			if (it == (m_vecGui.end() - 1))//�Ѿ������
				return;
			else
				std::iter_swap(it, m_vecGui.end() - 1);
		}
	}
}

// GUI�����ƶ�����ײ�
void CGUIManager::GotoObjectsBottom(CGUIObject* pWidget)
{
	if (pWidget)
	{
		VecGuiPtr::iterator it = std::find(m_vecGui.begin(), m_vecGui.end(), pWidget);
		if (it != m_vecGui.end())
		{
			if (it == m_vecGui.begin())//�Ѿ�����ײ�
				return;
			else
				std::iter_swap(it, m_vecGui.begin());
		}
	}
}
