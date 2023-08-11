/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSGUIButton.h
* ����ժҪ�� // ��ť�ඨ�弰ʵ��
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²� 
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
*  
*
*********************************************************************/

#include "CButton.h"
#include "CGUIManager.h"
#include "..\Root.h"

CButton::CButton(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName)
		:COverlay(pd3dDevice, sName)
		,m_pMouseDownMtrl(NULL)
		,m_pMouseOverMtrl(NULL)
{
	// ����Overlay����
	m_ObjType = VGS_GUIOBJ_TYPE_BUTTON;
}

CButton::~CButton()
{
}

const char * CButton::GetJsCallbackFunc()
{
	return m_strJsCallbackFunc.c_str();
}

void CButton::SetJsCallbackFunc(const char * sFunc)
{
	m_strJsCallbackFunc = sFunc;
}

// ���ò���  Type : 0-��ť����״̬(up)ʹ�õĲ��ʣ�1-����(down)ʱʹ�õĲ��ʣ�2-�����ͣ(over)ʱʹ�õĲ���
VOID CButton::SetMaterial(INT Type, CMaterial* pMaterial)
{
	switch (Type)
	{
	case 0: // base
		m_pMtrl = pMaterial;
		break;
	case 1: // down
		m_pMouseDownMtrl = pMaterial;
		break;
	case 2: // over
		m_pMouseOverMtrl = pMaterial;
		break;
	default:
		break;
	}

}

// ��ȡ����  0-��ť����״̬(up)ʹ�õĲ��ʣ�1-����(down)ʱʹ�õĲ��ʣ�2-�����ͣ(over)ʱʹ�õĲ���
CMaterial* CButton::GetMaterial(INT Type)
{
	switch (Type)
	{
	case 0: // base
		return m_pMtrl;
	case 1: // down
		return m_pMouseDownMtrl;
	case 2: // over
		return m_pMouseOverMtrl;
	default:
		return 0;
	}
}

bool CButton::GetMouseUpImageFileName(std::string& str)
{
	if (!m_pMtrl)
		return false;
	else
		return GetTextureImageOfMtrl(str, m_pMtrl, 0);
}
bool CButton::SetMouseUpImageFileName(const char* szFile)
{
	if (!m_pMtrl)
	{
		//create mtrl
		std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(GetName());
		m_pMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
	}
	return SetMtrlImageFileName(m_pMtrl, 0, szFile);
}
bool CButton::GetMouseDownImageFileName(std::string& str)
{
	if (!m_pMouseDownMtrl)
		return false;
	else
		return GetTextureImageOfMtrl(str, m_pMouseDownMtrl, 0);
}

bool CButton::SetMouseDownImageFileName(const char* szFile)
{
	if (!m_pMouseDownMtrl)
	{
		std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(GetName());
		strMtrl += "_MouseDown";
		m_pMouseDownMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
	}
	return SetMtrlImageFileName(m_pMouseDownMtrl, 0, szFile);
}
bool CButton::GetMouseOverImageFileName(std::string& str)
{
	if (!m_pMouseOverMtrl)
		return false;
	else
		return GetTextureImageOfMtrl(str, m_pMouseOverMtrl, 0);
}
bool CButton::SetMouseOverImageFileName(const char* szFile)
{
	if (!m_pMouseOverMtrl)
	{
		std::string strMtrl = m_pRoot->GetMaterialManager().GenerateGuiMtrlName(GetName());
		strMtrl += "_MouseOver";
		m_pMouseOverMtrl = (CMaterial*)m_pRoot->GetMaterialManager().createMaterial(strMtrl);
	}
	return SetMtrlImageFileName(m_pMouseOverMtrl, 0, szFile);
}
