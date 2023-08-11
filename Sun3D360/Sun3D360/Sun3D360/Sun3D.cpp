//--------------------------------------------------------------
//	����:Sun3D.cpp
//	���:��Sun3Dʵ���ļ�
//	����:Ҧ�з�
//	����:05-4-29
//	�汾:0.1V
//--------------------------------------------------------------
#include "Sun3D.h"
//--------------------------------------------------------------
//	���ƣ�CSun3D
//	��飺���캯��
//--------------------------------------------------------------
CSun3D::CSun3D()
{
	m_pOpenGLWnd = NULL;
}
//--------------------------------------------------------------
//	���ƣ�~CSun3D
//	��飺��������
//--------------------------------------------------------------
CSun3D::~CSun3D()
{
}
//--------------------------------------------------------------
//	���ƣ�S3D360_Create
//	��飺����OpenGL���壬����ʼ��OpenGL
//	������hInstance:i:Ӧ�ó���ʵ��
//	      hParent:i:�����ھ��
//		  rc:i:OpenGL������ڸ������ϵ�λ��
//	���أ������ɹ�����true�����򷵻�false
//--------------------------------------------------------------
bool CSun3D::S3D360_Create(HINSTANCE hInstance, HWND hParent, RECT rc)
{	
	if(m_pOpenGLWnd) S3D360_Destroy();

	m_pOpenGLWnd = new COpenGLWnd();
	if(!m_pOpenGLWnd->Create(hInstance, hParent, rc)) return false;

	ShowWindow(m_pOpenGLWnd->m_hWnd, SW_SHOW);
	UpdateWindow(m_pOpenGLWnd->m_hWnd);
	return true;
}
//--------------------------------------------------------------
//	���ƣ�S3D360_Resize
//	��飺����OpenGL�����λ��
//	������rc:i:OpenGL�����ڸ������ϵ���λ��
//	���أ���
//--------------------------------------------------------------
void CSun3D::S3D360_Resize(RECT rc)
{
	if(m_pOpenGLWnd)
	{
		::SetWindowPos(	m_pOpenGLWnd->m_hWnd, NULL, rc.left, rc.top, 
					rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
	}
}
//--------------------------------------------------------------
//	���ƣ�S3D360_Destroy
//	��飺����OpenGL����
//	��������
//	���أ����ٳɹ�����true�����򷵻�false
//--------------------------------------------------------------
bool CSun3D::S3D360_Destroy()
{
	if(m_pOpenGLWnd)
	{
		::DestroyWindow(m_pOpenGLWnd->m_hWnd);
		delete m_pOpenGLWnd;
		m_pOpenGLWnd = NULL;
	}

	return true;
}
//--------------------------------------------------------------


//--------------------------------------------------------------
/*  �����ǽӿں������� */
//--------------------------------------------------------------
//	���ƣ�S3D360_OpenFile
//	��飺��ָ����*.S3D�ļ����򿪳ɹ��󲥷ŵ�0֡
//	������file:i:ָ����*.S3D�ļ���
//	���أ��򿪳ɹ�����true�����򷵻�false
//--------------------------------------------------------------
bool CSun3D::S3D360_OpenFile(const char* file)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->OpenFile(file);
	return true;
}
//--------------------------------------------------------------
//	���ƣ�S3D360_SetPlayType
//	��飺���ò��ŵķ�ʽ
//	������iType:i:���ŷ�ʽ��0-�Զ����ţ�1-�ֶ�����
//	���أ���
//--------------------------------------------------------------
void CSun3D::S3D360_SetPlayType(int iType)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->SetPlayType(iType);
}
//--------------------------------------------------------------
//	���ƣ�S3D360_SetPause
//	��飺������ͣ
//	������iType:i:������ͣ��ture-��ͣ,false-��������
//	���أ���
//--------------------------------------------------------------
void CSun3D::S3D360_SetPause(bool bPause)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->SetPause(bPause);
}
//--------------------------------------------------------------
//	���ƣ�S3D360_SetCameraInfo
//	��飺�����������
//	������ang:i:�н�
//		  near:i:���ü���
//		  far:i:Զ�ü���
//	���أ���
//--------------------------------------------------------------
void CSun3D::S3D360_SetCameraInfo(float ang, float nearpos, float farpos)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->SetCameraInfo(ang, nearpos, farpos);
}
//--------------------------------------------------------------
//	���ƣ�S3D360_SetTimerInter
//	��飺������Ⱦʱ�Ӽ��
//	������inter:i:ʱ�Ӽ������λ������
//	���أ���
//--------------------------------------------------------------
void CSun3D::S3D360_SetTimerInter(int inter)
{
	if(m_pOpenGLWnd) m_pOpenGLWnd->SetTimerInter(inter);
}
//--------------------------------------------------------------
