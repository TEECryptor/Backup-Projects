#pragma once

#include "..\VGSHeader.h"
//#include "..\VGSDef.h"
#include "..\Event\VGSFrameListener.h"
//#include "..\CCamera.h"

using namespace std;

//// ���ƶ��������
//enum CONTROLTYPE
//{
//	CT_EDITOR = 0,
//	CT_WALKER,
//	CT_FLYER
//};
// �Ӷ���ͼ������ת����
enum AROUNDWISE
{
	AW_CLOCKWISE = 0,
	AW_ANTICLOCKWISE
};
enum MOUSE_TYPE
{ 
	MOUSE_TYPE_NONE = 0,
	MOUSE_TYPE_LEFT,
	MOUSE_TYPE_MIDDLE,
	MOUSE_TYPE_RIGHT,
	MOUSE_TYPE_DBLEFT
};
enum EXTENTEDKEY_TYPE
{
	EKT_NONE = 0,
	EKT_ALT,
	EKT_CTRL,
	EKT_SHIFT
};
// ���״̬
struct VGSMouseState
{
	INT x;  // �������Ⱦ���ڵ�ƽ������
	INT y;

	EXTENTEDKEY_TYPE extentedKey;  // �������̼�
	MOUSE_TYPE       mouseType;    // ��갴��

	DWORD hRenderWnd;  // ��Ⱦ����
};

// ��windows��Ϣ�����л�ȡ��갴������
extern MOUSE_TYPE getMouseType(UINT wParam);

// ��windows��Ϣ�����л�ȡ���̸���������
extern EXTENTEDKEY_TYPE getExtenedKeyType(UINT wParam);

// ���̰���
struct VGSKeyState
{
	WORD key;
	EXTENTEDKEY_TYPE extentedKey;  // �������̼�

	DWORD hRenderWnd;  // ��Ⱦ����
};

// ���ƶ���walker��editor��flyer�Ļ���
class VGS_EXPORT IController
{

public:

	virtual CONTROLTYPE		GetControlType() = 0;  // �õ�����������	
	virtual void			Update() = 0;          // ����control��״̬
	virtual string			GetName() = 0;
	virtual void			SetName(const string &sName) = 0;
	
	virtual void SetMouseState(const VGSMouseState &ms);
// 	virtual void SetMouseState(const VGSMouseState &ms)                  // �������״̬
// 	{
// 		m_MouseInfo = ms;
// 	}
	virtual void SetKeyboardState(const VGSKeyState &ks);
// 	virtual void SetKeyboardState(const VGSKeyState &ks)     // ���ü���״̬
// 	{
// 		m_Keyboard = ks;
// 	}
	virtual void SetFrameState(const FrameEvent &FrameEvent)         // ����֡��Ϣ
	{
		m_FrameEvent = FrameEvent;
	}
	void SetCamera(CCamera *pCamera)  // �����麯�����������������������Ĵ���
	{
		m_pCamera = pCamera;
	}
	virtual CCamera* GetCamera()
	{
		return m_pCamera;
	}

public:

	IController();

	virtual ~IController()
	{
		Release();
	}

	virtual VOID Release(){m_pCamera = NULL;}
protected:
	
	VGSMouseState       m_MouseInfo;		 // ��ǰ�������Ϣ
	VGSKeyState  	    m_Keyboard;          // ��ǰ�İ���״̬
	FrameEvent          m_FrameEvent;        // ֡��Ϣ
	CCamera              *m_pCamera;         // ���ָ��

	INT m_disX;  // �������Ļ���ƶ�λ�ƣ�X����
	INT m_disY;  // �������Ļ���ƶ�λ�ƣ�Y����

	VGSMouseState       m_lastMouseInfo;   // ǰһ�ε����״̬
	VGSKeyState         m_lastKeyState;    // ǰһ�εİ���״̬

	bool                m_bKeyProcessed;   // �Ƿ��Ѿ������
	bool                m_bMouseProcessed;

};