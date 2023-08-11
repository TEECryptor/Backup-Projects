/////////////////////////////////////////////////////////////////////////
// ����: CFont���Ͷ���--������
// ����: CFont���Ͷ����˴�������ķ������Լ��������ֵķ���
// ����: ����	
// ����: 2006-05-19
////////////////////////////////////////////////////////////////////////

#include "CFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74E017C
CFont::CFont(LPDIRECT3DDEVICE9 pDevice)
{
	index = -1;
	m_pd3dDevice = pDevice;
	m_pd3dFont = NULL;
}

//##ModelId=47EBC74E017E
CFont::~CFont()
{
}

//-------------------------------------------------------------------
//����: Init()
//����: 
//��;: ��ʼ���������
//-------------------------------------------------------------------
//##ModelId=47EBC74E015A
VOID CFont::Init(CHAR* fontName)
{
	//����d3dFont�ӿ�ָ�����
	m_LOGFONT.Height = 13;
	m_LOGFONT.Width = 0;
	m_LOGFONT.Weight = 0;
	m_LOGFONT.Italic = 0;
	m_LOGFONT.CharSet = DEFAULT_CHARSET;
	m_LOGFONT.OutputPrecision = OUT_DEFAULT_PRECIS;
	m_LOGFONT.Quality = DEFAULT_QUALITY;
	//m_LOGFONT.FaceName = DEFAULT_PITCH;
	strcpy(m_LOGFONT.FaceName, fontName);

	D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
}

//����
//##ModelId=47EBC74E015C
VOID CFont::Restore()
{
	SAFE_RELEASE_D3D(m_pd3dFont);
	D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
}

//-------------------------------------------------------------------
//����: SetFont()
//����: 
//��;: ��������
//-------------------------------------------------------------------
//##ModelId=47EBC74E015D
VOID CFont::SetFont(CHAR* strFontName)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);
		strcpy(m_LOGFONT.FaceName, strFontName);

		SAFE_RELEASE_D3D(m_pd3dFont);
		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
	}
}

//�������
//##ModelId=47EBC74E015F
VOID CFont::GetFont(CHAR* strFontName)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);
		strcpy(strFontName, m_LOGFONT.FaceName);
	}
}	

//-------------------------------------------------------------------
//����: SetHeight()
//����: 
//��;: �������ָ߶�
//-------------------------------------------------------------------
//##ModelId=47EBC74E0167
VOID CFont::SetHeight(INT height)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);
		m_LOGFONT.Height = height;

		SAFE_RELEASE_D3D(m_pd3dFont);
		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
	}
}

//�õ����ָ߶�
//##ModelId=47EBC74E0169
INT CFont::GetHeight()
{
	if (m_pd3dFont)
		m_pd3dFont->GetDesc(&m_LOGFONT);
	return m_LOGFONT.Height;
}
			
//-------------------------------------------------------------------
//����: SetBold()
//����: 
//��;: �����Ƿ������ʾ
//-------------------------------------------------------------------
//##ModelId=47EBC74E016A
VOID CFont::SetBold(BOOL isBold)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);

		if (isBold)
			m_LOGFONT.Weight = FW_BOLD;
		else
			m_LOGFONT.Weight = FW_NORMAL;

		SAFE_RELEASE_D3D(m_pd3dFont);
		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
	}
}

//�õ��Ƿ������ʾ
//##ModelId=47EBC74E016C
BOOL CFont::GetBold()
{
	if (m_pd3dFont)
		m_pd3dFont->GetDesc(&m_LOGFONT);
	
	if (m_LOGFONT.Weight == FW_BOLD)
		return TRUE;
	else
		return FALSE;
}
					
//-------------------------------------------------------------------
//����: SetItalic()
//����: 
//��;: �����Ƿ�б����ʾ
//-------------------------------------------------------------------
//##ModelId=47EBC74E016D
VOID CFont::SetItalic(BOOL isItalic)
{
	if (m_pd3dFont)
	{
		m_pd3dFont->GetDesc(&m_LOGFONT);
		m_LOGFONT.Italic = TRUE;

		SAFE_RELEASE_D3D(m_pd3dFont);
		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
	}
}

//����Ƿ�б��
//##ModelId=47EBC74E016F
BOOL CFont::GetItalic()
{
	if (m_pd3dFont)
		m_pd3dFont->GetDesc(&m_LOGFONT);
	return m_LOGFONT.Italic;
}

//-------------------------------------------------------------------
//����: SetUnderline()
//����: 
//��;: �����Ƿ���ʾ�»���
//-------------------------------------------------------------------
//##ModelId=47EBC74E0170
VOID CFont::SetUnderline(BOOL isUnderline)
{
// 	if (m_pd3dFont)
// 	{
// 		m_pd3dFont->GetDesc(&m_LOGFONT);
// 		m_LOGFONT.Underline = TRUE;
// 
// 		SAFE_RELEASE_D3D(m_pd3dFont);
// 		D3DXCreateFontIndirect(m_pd3dDevice, &m_LOGFONT, &m_pd3dFont);
// 	}
}
//##ModelId=47EBC74E0172
BOOL CFont::GetUnderline()
{
// 	if (m_pd3dFont)
// 		m_pd3dFont->GetDesc(&m_LOGFONT);
// 	return m_LOGFONT.Underline;
	return FALSE;
}

//-------------------------------------------------------------------
//����: DrawText()
//����: 
//��;: ��������
//-------------------------------------------------------------------
//##ModelId=47EBC74E0173
VOID CFont::DrawText(CHAR* strText, RECT* pRect, UINT uFormat, DWORD color)
{	
	if (m_pd3dFont)
	{
// 		m_pd3dFont->Begin();
		m_pd3dFont->DrawText(NULL,strText, -1, pRect, uFormat, color);
//		m_pd3dFont->End();
	}
} 

//-------------------------------------------------------------------
//����: Release()
//����: 
//��;: �ͷ��ڴ�
//-------------------------------------------------------------------
//##ModelId=47EBC74E017B
VOID CFont::Release()
{
	SAFE_RELEASE_D3D(m_pd3dFont);
}
