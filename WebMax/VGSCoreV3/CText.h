/////////////////////////////////////////////////////////////////////////
// ����: CText ���Ͷ���--������
// ����: CText ���Ͷ����˳����е����ֵ�Ԫ
// ����: ����	
// ����: 2006-05-19
////////////////////////////////////////////////////////////////////////

#ifndef _CTEXT_H
#define _CTEXT_H

#include "CSysFunc.h"
#include "CFont.h"

//�ؼ�֡��Ϣ
//##ModelId=47EBC72F0000
typedef struct _TextKfrInfo
{
	INT				x;
	INT				y;
	DWORD			color;
}_TextKfrInfo;

//##ModelId=47EBC72F0002
class VGS_EXPORT CText
{
public:
	//##ModelId=47EBC72F0003
	INT					index;
	//##ModelId=47EBC72F0004
	CHAR				name[32];

	BOOL				visible;

	//ʹ�õ��������
	//##ModelId=47EBC72F0006
	CFont*				m_pFont;	

	//##ModelId=47EBC72F000F
	CHAR*				strText;		//�ַ���
	//##ModelId=47EBC72F0011
	RECT				rect;			//���ֵ�λ��
	//##ModelId=47EBC72F0015
	UINT				uFormat;		//������ʾ�ĸ�ʽ
	//##ModelId=47EBC72F0017
	D3DCOLOR			color;			//���ֵ���ɫ

public:
	//�ؼ�֡����
	//##ModelId=47EBC72F001B
	INT							m_kfrCnt;			//�ؼ�֡����
	//##ModelId=47EBC72F001D
	_TextKfrInfo*				m_kfrInfo;			//�ؼ�֡����

public:
	//�ͷ���Դ
	//##ModelId=47EBC72F0021
	VOID				Release();

public:
	//##ModelId=47EBC72F0022
	CText(CFont* pFont);
	//##ModelId=47EBC72F0024
	~CText();

};

#endif
