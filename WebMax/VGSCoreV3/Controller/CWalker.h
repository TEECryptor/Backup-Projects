/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // CWalker.h
* ����ժҪ�� // ����ģʽ�ඨ��
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

#pragma once


#include "..\VGSHeader.h"
#include "Controller.h"
#include "..\CCamera.h"

// Walker ������
class VGS_EXPORT CWalker : public IController
{

public : // interface

	virtual CONTROLTYPE		GetControlType();  // �õ�����������	
	virtual void			Update();    
	virtual string			GetName();
	virtual void			SetName(const string &sName);// ����control��״̬

	virtual void SetCamera(CCamera *pCamera);

	// virtual void SetMouseState(const MouseState &ms);                  // �������״̬
	// virtual void SetKeyboardState(const OIS::Keyboard *pKeyboard);     // ���ü���״̬
	// virtual void SetFrameState(const FrameEvent &FrameEvent)           // ����֡��Ϣ

public:

	CWalker(const WalkerInfo &Info);
	~CWalker();

	void SetWalkerInfo(const WalkerInfo &Info);  // ���ò���
	void GetWalkerInfo(WalkerInfo &Info);        // ��ȡ����

private:
	WalkerInfo        m_WalkerInfo;		    // Editor������Ϣ

	VECTOR3     m_TranslateVector;
	float       m_MoveScale;
	float       m_RotScale;

	float       m_MaxUpperAngle; // ���ȱ�ʾ���������
	float       m_MaxLowerAngle; // ���ȱ�ʾ����󸩽�
};