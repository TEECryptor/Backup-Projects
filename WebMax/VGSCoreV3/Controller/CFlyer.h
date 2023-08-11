#pragma once

#include "Controller.h"

#include "..\CCamera.h"

// Flyer ������
class VGS_EXPORT CFlyer : public IController
{

public : // interface

	virtual CONTROLTYPE		GetControlType();        // �õ�����������	
	virtual void			Update();    
	virtual string			GetName();
	virtual void			SetName(const string &sName);  // ����control��״̬
	virtual void            SetCamera(CCamera *pCamera);  // �������

public:

	CFlyer(const FlyerInfo &info);
	~CFlyer();

	void SetFlyerInfo(const FlyerInfo &Info);  // ���ò���
	void GetFlyerInfo(FlyerInfo &Info);        // ��ȡ����

private:
	FlyerInfo        m_FlyerInfo;		    // FlyerInfo������Ϣ

	VECTOR3     m_TranslateVector;
	float       m_MoveScale;
	float       m_RotScale;
};