/////////////////////////////////////////////////////////////////////////
// ����: CInput.h--CInput class
// ����: ��DX�����豸�Ĵ����࣬���������̵�����Ķ�ȡ
// ����: lixin	
// ����: 2006-07-22
////////////////////////////////////////////////////////////////////////

#ifndef _CINPUT_H
#define _CINPUT_H

//-----------------------------------------------------------------------
#include "..\\VGSCoreV3\\VGSDef.h"
#include "CSysFunc.h"

//������Ϣ�ṹ��
//typedef struct MOUSEINFO
//{
//	SHORT		X;
//	SHORT		Y;				//����
//	BOOL		LButton;		//����Ϊ1���ɿ�Ϊ0
//	BOOL		RButton;
//	FLOAT		RollDis;		//����
//}MOUSEINFO;

//-----------------------------------------------------------------------
class CInput  
{
public:
	//����������ݵĻ���
	BYTE					keyBoardBuf[256];
	//����������ݵĻ���
	MOUSEINFO				mouseInfo;
	//�ͷ�������Դ
	VOID					Release();

public:
	CInput();
	~CInput();

};

#endif