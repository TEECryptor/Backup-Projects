/////////////////////////////////////////////////////////////////////////
// ����: CFont���Ͷ���--������
// ����: CFont���Ͷ����˴�������ķ������Լ��������ֵķ���
// ����: ����	
// ����: 2006-05-19
////////////////////////////////////////////////////////////////////////

#ifndef _CFONT_H
#define _CFONT_H

#include "VGSHeader.h"
#include "CSysFunc.h"

//##ModelId=47EBC74E0148
class VGS_EXPORT CFont  
{
private:
	//##ModelId=47EBC74E014A
	LPDIRECT3DDEVICE9	m_pd3dDevice;					//d3d�豸�ӿ�
	//##ModelId=47EBC74E014F
	LPD3DXFONT			m_pd3dFont;						//d3d����ӿ�
	//##ModelId=47EBC74E0154
	D3DXFONT_DESC		m_LOGFONT;						//Windows����
	
public:
	//##ModelId=47EBC74E0158
	CHAR				name[32];						//����
	//##ModelId=47EBC74E0159
	INT					index;							//���

	//CHAR				m_FontName[32];					//��������
public:
	
	//##ModelId=47EBC74E015A
	VOID				Init(CHAR* fontName);			//��ʼ���������

	//����
	//##ModelId=47EBC74E015C
	VOID				Restore();					

	//##ModelId=47EBC74E015D
	VOID				SetFont(CHAR* strFontName);		
	//##ModelId=47EBC74E015F
	VOID				GetFont(CHAR* strFontName);

	//##ModelId=47EBC74E0167
	VOID				SetHeight(INT height);			//�������ָ߶�
	//##ModelId=47EBC74E0169
	INT					GetHeight();					//�õ����ָ߶�
		
	//##ModelId=47EBC74E016A
	VOID				SetBold(BOOL isBold);			//�����Ƿ������ʾ
	//##ModelId=47EBC74E016C
	BOOL				GetBold();						//�õ��Ƿ������ʾ

	//##ModelId=47EBC74E016D
	VOID				SetItalic(BOOL isItalic);		//�����Ƿ�б����ʾ
	//##ModelId=47EBC74E016F
	BOOL				GetItalic();					//�õ��Ƿ�б����ʾ

	//##ModelId=47EBC74E0170
	VOID				SetUnderline(BOOL isUnderline); //�����Ƿ���ʾ�»���
	//##ModelId=47EBC74E0172
	BOOL				GetUnderline();					//�õ��Ƿ���ʾ�»���

	//##ModelId=47EBC74E0173
	VOID				DrawText(CHAR* strText, RECT* pRect, UINT uFormat, DWORD color); //��������
	
	//##ModelId=47EBC74E017B
	VOID				Release();

public:
	//##ModelId=47EBC74E017C
	CFont(LPDIRECT3DDEVICE9 pDevice);
	//##ModelId=47EBC74E017E
	~CFont();

};

#endif
