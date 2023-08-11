#include "background.h"
#include "..\Root.h"
#include "..\CMaterial.h"

const char* BACKGROUND_NAME = "scene_back_ground_layer";

BackGround::BackGround(LPDIRECT3DDEVICE9 pd3dDevice):COverlay(pd3dDevice, BACKGROUND_NAME)
{
	m_ObjType = VGS_GUIOBJ_TYPE_BACKDROP;
}
BackGround::~BackGround()
{

}

void BackGround::GetBaseImage(std::string &strImg)
{
	if (m_pMtrl && m_pMtrl->getTexureLayerCount() > 0)
	{
		CTextureLayer* pTxt = m_pMtrl->getTextureLayerByIndex(0);
		strImg = pTxt->pTexture->getTextrueFileName();
	}			
}
void BackGround::SetBaseImage(const std::string& strImg)
{
	if (m_pMtrl)
	{
		if (m_pMtrl->getTexureLayerCount()  > 0)
		{
			CTextureLayer* pTxt = 0;
			pTxt = m_pMtrl->getTextureLayerByIndex(0);
			if (pTxt)
				m_pMtrl->clearTextureLayerByHandle(pTxt);
		}
		if (strImg.length() > 0)
			m_pMtrl->createTextureLayer(strImg, m_pMtrl->getName() + "_txt0", 0);
	}			
}

void BackGround::FullScreen()
{
	m_fLeft = 0.0f;
	m_fTop = 0.0f;
	m_fWidth = 1.0f;
	m_fHeight = 1.0f;
	
	::SetRect(&m_rc, 0, 0, m_nVpWidth, m_nVpHeight);
	UpdateRect(m_rc);
}

void BackGround::SetRectF(const FRect &rc)
{

}
void BackGround::SetRect(float left, float top, float width, float height)
{
	m_fLeft = left;
	m_fTop = top;
	m_fWidth = width;
	m_fHeight = height;
	//update...
	m_rc.left = (int)(m_nVpWidth*m_fLeft);
	m_rc.top = (int)(m_nVpHeight*m_fTop);
	m_rc.right = m_rc.left + (int)(m_fWidth*m_nVpWidth);
	m_rc.bottom = m_rc.top + (int)(m_fHeight*m_nVpHeight);
	UpdateRect(m_rc);
}
FRect BackGround::GetRectF()
{
	FRect rc;
// 	rc.Left = m_fLeft;
// 	rc.Top = m_fTop;
// 	rc.Right = rc.Left + 
	return rc;
}
void BackGround::GetRect(float &left, float &top, float &width, float &height)
{
	left = m_fLeft;
	top = m_fTop;
	width = m_fWidth;
	height = m_fHeight;
}
