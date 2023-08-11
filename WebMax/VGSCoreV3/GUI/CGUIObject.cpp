#include "CGuiObject.h"
//#include "..\CFace.h"
//#include "..\CVertex.h"
#include "..\CMaterial.h"

#include <sstream>

//const float CGUIObject::SELBOX_WIDTH = 2.0f;
//const float CGUIObject::SELBOX_HEIGHT = 2.0f;

CGUIObject::CGUIObject(LPDIRECT3DDEVICE9 pd3dDevice, const char* szName)
	:m_pd3dDevice(pd3dDevice)
	,m_strName(szName)
{
	m_FVFSize = sizeof(VERTEXDECL_POSRHWCOLORCOORD1);
	m_FVF = FVF_POSRHWCOLORCOORD1;
	m_pMtrl = NULL;

	::SetRect(&m_rc, 0, 0, 100, 80);		

	m_ObjType = VGS_GUIOBJ_TYPE_NONE;

	m_bEnable = true;
	m_bIsFilter = false;
	m_bVisible = true;
	m_bShowSelBox = false;
	m_nOpacity  = 100;
	m_fAngle    = 0.0f;	


	SELBOX_WIDTH = 2.0f;
	SELBOX_HEIGHT = 2.0f;
	
	m_OffsetOrigin = VOO_TOP_LEFT;         // 对齐视口的参考原点
	
	m_bRelativeSize = true;
	m_bRelativePos = true;
	m_fWidth  = 1.0f;
	m_fHeight = 1.0f;
	m_fTop  = 0.0f;
	m_fLeft = 0.0f;

	m_zOrder = 0;
		
	m_nVpWidth = 0;
	m_nVpHeight = 0;

	m_pSelBoxVbuf = 0;
	m_pSelBoxIbuf = 0;

	InitSelBoxBuf();

	m_sMouseOverFunc = "";
	m_sMouseDBClickFunc = "";
	m_sMouseLDownFunc = "";
	m_sMouseLUpFunc = "";
	m_sMouseRDownFunc = "";
	m_sMouseRUpFunc = "";
	m_sMouseMoveFunc = "";
	m_sMouseLeaveFunc = "";

}

CGUIObject::~CGUIObject()
{
	Destroy();
}


void CGUIObject::InitSelBoxBuf()
{
	memset(m_SelBox, 0, sizeof(VERTEXDECL_POSRHWCOLORCOORD1)*4);
	GenerateSelBoxByPoint(m_rc.left, m_rc.top);
//	m_SelBox[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_SelBox[0].rhw = 1.0f;
//	m_SelBox[1].diffuse = m_SelBox[0].diffuse;
	m_SelBox[1].rhw = 1.0f;
//	m_SelBox[2].diffuse = m_SelBox[0].diffuse;
	m_SelBox[2].rhw = 1.0f;
//	m_SelBox[3].diffuse = m_SelBox[0].diffuse;
	m_SelBox[3].rhw = 1.0f;

	if (NULL == m_pd3dDevice)
		return;
	//vertex buffer
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(sizeof(VERTEXDECL_POSRHWCOLORCOORD1)*4, 0,m_FVF, D3DPOOL_MANAGED, &m_pSelBoxVbuf, NULL)))
		return;
	void* buf = 0; 
	if (FAILED(m_pSelBoxVbuf->Lock(0, sizeof(VERTEXDECL_POSRHWCOLORCOORD1)*4, &buf, D3DLOCK_DISCARD)))
		return;
	//逐个顶点填充数据
	for (UINT i = 0; i< 4; i++)
	{
		//填充坐标数据到顶点缓冲
		((VERTEXDECL_POSRHWCOLORCOORD1*)buf)[i] = m_SelBox[i];
	}
	//unlock
	m_pSelBoxVbuf->Unlock();

	//index buffer
	if (FAILED(m_pd3dDevice->CreateIndexBuffer(2*3*sizeof(UINT), 0, D3DFMT_INDEX16,D3DPOOL_MANAGED, &m_pSelBoxIbuf, NULL)))
		return;
	unsigned short * idx = 0;
	if (FAILED(m_pSelBoxIbuf->Lock(0, 2*3*sizeof(UINT),(VOID**)&idx, 0)))
		return;
	idx[0] = 0;
	idx[1] = 2;
	idx[2] = 1;
	idx[3] = 0;
	idx[4] = 3;
	idx[5] = 2;
	m_pSelBoxIbuf->Unlock();
}
void CGUIObject::GenerateSelBoxByPoint(int x, int y)
{
	m_SelBox[0].position = D3DXVECTOR3(x - SELBOX_WIDTH, y - SELBOX_HEIGHT, 0.0f);

	m_SelBox[1].position = D3DXVECTOR3(x + SELBOX_WIDTH, y - SELBOX_HEIGHT, 0.0f);

	m_SelBox[2].position = D3DXVECTOR3(x + SELBOX_WIDTH, y + SELBOX_HEIGHT, 0.0f);

	m_SelBox[3].position = D3DXVECTOR3(x - SELBOX_WIDTH, y + SELBOX_HEIGHT, 0.0f);
}


void CGUIObject::RenderSelBox()
{
	int i = 0;
	//select box 8
	int x[8],y[8];
	x[0] = m_rc.left; y[0] = m_rc.top;
	x[1] = (m_rc.left + m_rc.right)/2; y[1] = m_rc.top;
	x[2] = m_rc.right; y[2] = m_rc.top;
	x[3] = m_rc.left; y[3] = (m_rc.top + m_rc.bottom)/2;
	x[4] = m_rc.right; y[4] = (m_rc.top + m_rc.bottom)/2;
	x[5] = m_rc.left; y[5] = m_rc.bottom;
	x[6] = (m_rc.left + m_rc.right)/2; y[6] = m_rc.bottom;
	x[7] = m_rc.right; y[7] = m_rc.bottom;
	//render
	for (i = 0; i < 8; i++)
	{
		GenerateSelBoxByPoint(x[i], y[i]);
		UpdateSelBox();
		m_pd3dDevice->SetStreamSource(0, m_pSelBoxVbuf, 0, m_FVFSize );
		m_pd3dDevice->SetIndices(m_pSelBoxIbuf);
		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 	0, 	2);
	}
}
bool CGUIObject::IsInRect(int x, int y)
{
	return (x >= m_rc.left && x <= m_rc.right && y >= m_rc.top && y <= m_rc.bottom);
}

void CGUIObject::SetUsedRelativePos(bool bUseRelative) 	
{	
	m_bRelativePos = bUseRelative;	
	CalcRelativePos();
	UpdateRect(m_rc); 
}
void CGUIObject::SetUsedRelativeSize(bool bUseRelative) 
{
	m_bRelativeSize = bUseRelative;
	CalcRelativeSize();
	UpdateRect(m_rc);
}
void CGUIObject::GetPosition(INT &x, INT &y) 
{
	int l = 0, t = 0;
	GetPosByOffsetOrigin(l,t);
	x = m_rc.left - l; 
	y = m_rc.top - t; 
}
//!x,y——相对于m_OffsetOrigin的坐标
void CGUIObject::SetPosition(INT x, INT y)
{ 
	int w = m_rc.right - m_rc.left, h = m_rc.bottom - m_rc.top;
	int l = 0, t = 0;
	GetPosByOffsetOrigin(l,t);
	m_rc.left = l + x;
	m_rc.top = t + y; 
	CalcRelativePos();
	SetSize(w, h);
	UpdateRect(m_rc);
}
void CGUIObject::GetSize(INT &width, INT &height) 
{
	width = m_rc.right - m_rc.left; 
	height = m_rc.bottom - m_rc.top; 
}

void CGUIObject::SetSize(INT width, INT height) 
{
	m_rc.right = m_rc.left + width;
	m_rc.bottom = m_rc.top + height; 
	CalcRelativeSize();
	UpdateRect(m_rc);
}
void CGUIObject::GetRect(RECT &rc) 
{
	int l = 0, t = 0;
	GetPosByOffsetOrigin(l,t);
	rc.left = m_rc.left - l; 
	rc.top = m_rc.top - t; 
	rc.right = m_rc.right - l; 
	rc.bottom = m_rc.bottom - t; 
}

void CGUIObject::SetRect(const RECT &rc)
{
	int l = 0, t = 0;
	GetPosByOffsetOrigin(l,t);
	m_rc.left = l + rc.left;
	m_rc.right = l + rc.right;
	m_rc.top = t + rc.top;
	m_rc.bottom = t + rc.bottom;

	CalcRelativePos();
	CalcRelativeSize();
	//
	UpdateRect(m_rc); 
}
void CGUIObject::SetAbsoluteRect(const RECT &rc) 
{
	m_rc = rc;
	CalcRelativePos();
	CalcRelativeSize();
	//
	UpdateRect(m_rc); 
}

void CGUIObject::SetOffsetOrigin(VGSOFFSETORIGIN offsetOrigin) 
{
	m_OffsetOrigin = offsetOrigin; 
	SetPosition(0, 0);
}
void CGUIObject::SetViewportSize(int nWidth, int nHeight)
{ 
	m_nVpWidth = nWidth;
	m_nVpHeight = nHeight; 
	CalcAbsolutePosition();
	CalcAbsoluteSize();
	UpdateRect(m_rc);
}
void CGUIObject::GetPosByOffsetOrigin(int& nLeft, int& nTop)
{
	nLeft = 0;
	nTop = 0;
	switch (m_OffsetOrigin)
	{
	case VOO_BOTTOM_LEFT:
		nTop  = m_nVpHeight;
		nLeft = 0;
		break;
	case VOO_BOTTOM_MIDDLE:
		nTop  = m_nVpHeight;
		nLeft = m_nVpWidth / 2;
		break;
	case VOO_BOTTOM_RIGHT:
		nTop  = m_nVpHeight;
		nLeft = m_nVpWidth;
		break;
	case VOO_CENTER:
		nTop  = m_nVpHeight / 2;
		nLeft = m_nVpWidth / 2;
		break;
	case VOO_MIDDLE_LEFT:
		nTop  = m_nVpHeight / 2;
		nLeft = 0;
		break;
	case VOO_MIDDLE_RIGHT:
		nTop  = m_nVpHeight / 2;
		nLeft = m_nVpWidth;
		break;
	case VOO_TOP_LEFT:
		break;
	case VOO_TOP_MIDDLE:
		nTop  = 0;
		nLeft = m_nVpWidth / 2;
		break;
	case VOO_TOP_RIGHT:
		nTop  = 0;
		nLeft = m_nVpWidth;
		break;
	default:
		break;
	}
}


// 根据相对坐标计算GUI对象的真实位置
void CGUIObject::CalcAbsolutePosition()
{
	if (!m_bRelativePos)
		return;
	//相对于视口的位置
	float relativeLeft = m_fLeft * m_nVpWidth;
	float relativeTop = m_fTop * m_nVpHeight;
	// 坐标原点
	int  OriginLeft = 0;
	int OriginTop  = 0;
	//新的相对比例的绝对坐标值
	GetPosByOffsetOrigin(OriginLeft, OriginTop);
	// 真实坐标 = 坐标原点 + 相对坐标
	m_rc.left = OriginLeft + (int)relativeLeft;
	m_rc.top  = OriginTop + (int)relativeTop;
}
void CGUIObject::CalcAbsoluteSize()
{
	if (!m_bRelativeSize)
		return;
	m_rc.right = m_rc.left + (int)(m_fWidth * m_nVpWidth);
	m_rc.bottom = m_rc.top + (int)(m_fHeight * m_nVpHeight);
}
void CGUIObject::CalcRelativePos()
{
	if (m_bRelativePos)
	{
		m_fLeft = (float)m_rc.left/(float)m_nVpWidth;
		m_fTop = (float)m_rc.top/(float)m_nVpHeight;
	}
}
void CGUIObject::CalcRelativeSize()
{
	if (m_bRelativeSize)
	{
		m_fWidth = (float)(m_rc.right - m_rc.left)/(float)m_nVpWidth;
		m_fHeight = (float)(m_rc.bottom - m_rc.top)/(float)m_nVpHeight;
	}
}


void CGUIObject::UpdateSelBox()
{
	void* buf = 0; 
	if (FAILED(m_pSelBoxVbuf->Lock(0, sizeof(VERTEXDECL_POSRHWCOLORCOORD1)*4, &buf, D3DLOCK_DISCARD)))
		return;
	//逐个顶点填充数据
	for (UINT i = 0; i< 4; i++)
	{
		//填充坐标数据到顶点缓冲
		((VERTEXDECL_POSRHWCOLORCOORD1*)buf)[i] = m_SelBox[i];
	}
	//unlock
	m_pSelBoxVbuf->Unlock();
}

bool CGUIObject::GetTextureImageOfMtrl(std::string& str, CMaterial* pMtrl, int nLayerIdx)
{
 	if (pMtrl && (int)pMtrl->getTexureLayerCount() > nLayerIdx)
	{
		CTexture* pTxt = pMtrl->getTextureLayerByIndex(nLayerIdx)->pTexture;
		if (pTxt)
		{	
			str = pTxt->getTextrueFileName(); 
			return true;
		}
	}
	return false;
}
bool CGUIObject::SetMtrlImageFileName(CMaterial* pMtrl, int nLayerIdx, const char* szFile)
{
	if (!szFile || strlen(szFile) <= 0)
		return false;
	if (pMtrl)
	{
		std::ostringstream os;
		os<<nLayerIdx;
		std::string str = pMtrl->getName();
		str += os.str();
		if ((int)pMtrl->getTexureLayerCount() > nLayerIdx)
		{
			pMtrl->clearTextureLayerByIndex(nLayerIdx);
		}
		pMtrl->createTextureLayer(std::string(szFile), str,0); 
	}
	return false;
}

void CGUIObject::SetEventFuncName(VGS_MOUSEEVENT_TYPE type, const std::string &sFuncName)
{
	switch (type)
	{
	case VGS_MOUSEEVENT_TYPE_OVER:
		SetMouseOverFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_DBCLICK:
		SetMouseDBClickFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_LEFTDOWN:
		SetMouseLDwonFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_LEFTUP:
		SetMouseLUpFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_RIGHTDOWN:
		SetMouseRDownFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_RIGHTUP:
		SetMouseRUpFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_MOVE:
		SetMouseMoveFuncName(sFuncName);
		break;
	case VGS_MOUSEEVENT_TYPE_LEAVE:
		SetMouseLeaveFuncName(sFuncName);
		break;
	default:
		break;
	}
}
const std::string& CGUIObject::GetEventFuncName(VGS_MOUSEEVENT_TYPE type)
{
	switch (type)
	{
	case VGS_MOUSEEVENT_TYPE_OVER:
		return GetMouseOverFuncName();
	case VGS_MOUSEEVENT_TYPE_DBCLICK:
		return GetMouseDBClickFuncName();
	case VGS_MOUSEEVENT_TYPE_LEFTDOWN:
		return GetMouseLDwonFuncName();
	case VGS_MOUSEEVENT_TYPE_LEFTUP:
		return GetMouseLUpFuncName();
	case VGS_MOUSEEVENT_TYPE_RIGHTDOWN:
		return GetMouseRDownFuncName();
	case VGS_MOUSEEVENT_TYPE_RIGHTUP:
		return GetMouseRUpFuncName();
	case VGS_MOUSEEVENT_TYPE_MOVE:        // 移动
		return GetMouseMoveFuncName();
	case VGS_MOUSEEVENT_TYPE_LEAVE:        // 离开按钮 
		return GetMouseLeaveFuncName();
	default:
		return std::string("");
	}
}
