/////////////////////////////////////////////////////////////////////////
// ����: CText ���Ͷ���--������
// ����: CText ���Ͷ����˳����е����ֵ�Ԫ
// ����: ����	
// ����: 2006-05-19
////////////////////////////////////////////////////////////////////////

#include "CText.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC72F0022
CText::CText(CFont* pFont)
{
	m_pFont = pFont;
	strText = NULL;
	rect.left = 0;
	rect.top = 0;
	uFormat = DT_LEFT;
	color = D3DCOLOR_RGBA(0,0,0,255);

	m_kfrCnt = 0;
	m_kfrInfo = NULL;
	visible = TRUE;
}	

//##ModelId=47EBC72F0024
CText::~CText()
{

}


//##ModelId=47EBC72F0021
VOID CText::Release()
{
	//SAFE_DELETE_ARRAY(strText);
	SAFE_DELETE_ARRAY(m_kfrInfo);
}
