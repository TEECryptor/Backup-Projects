//--------------------------------------------------------------
//	����:Sun3D.h
//	���:��Sun3D�����ļ�
//	����:Ҧ�з�
//	����:05-4-29
//	�汾:0.1V
//--------------------------------------------------------------
#ifndef _SUN3D_H_
#define _SUN3D_H_
//--------------------------------------------------------------
#include "../Common/Sun3D360.h"
#include "OpenGLWnd.h"
//--------------------------------------------------------------
class CSun3D : public CSun3D360
{
public:
	CSun3D();
	~CSun3D();
	//----------------------------------------------------------
public:
	//����CSun3D360�ӿڣ�Ҳ���Ǵ���OpenGL���壬�ýӿڱ����ȵ��ã�Ȼ����ܵ��������ӿ�
	bool __stdcall S3D360_Create(HINSTANCE hInstance, HWND hParent, RECT rc);
	//��������OpenGL����Ĵ�С�����丸����Ĵ�С�ı�ʱ�����øýӿ�
	void __stdcall S3D360_Resize(RECT rc);
	//����OpenGL���壬��Ӧ�ó����˳�ʱ�����øýӿ�
	bool __stdcall S3D360_Destroy();
	//----------------------------------------------------------
	//��ָ����*.S3D�ļ�
	bool __stdcall S3D360_OpenFile(const char* file);
	//���ò��ŵķ�ʽ��0-�Զ����ţ�1-�ֶ�����
	void __stdcall S3D360_SetPlayType(int iType);
	//������ͣ,ture-��ͣ,false-��������
	void __stdcall S3D360_SetPause(bool bPause);
	//�����������
	void __stdcall S3D360_SetCameraInfo(float ang, float nearpos, float farpos);	
	//������Ⱦʱ�Ӽ��
	void __stdcall S3D360_SetTimerInter(int inter);
	//----------------------------------------------------------
private:
	COpenGLWnd*			m_pOpenGLWnd;		//OpenGL��Ⱦ����
};
//--------------------------------------------------------------
#endif	//_SUN3D_H_
//--------------------------------------------------------------
