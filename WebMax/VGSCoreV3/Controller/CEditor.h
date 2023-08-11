/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // CEditor.h
* ����ժҪ�� // ���չʾ�����ඨ��
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
#include "..\CScene.h"
#include "..\CModel.h"
#include "..\Node.h"
#include "..\CCamera.h"

#include "Controller.h"

// Editor ������
class VGS_EXPORT CEditor : public IController
{

public : // interface

	virtual CONTROLTYPE		GetControlType();  // �õ�����������	
	virtual void			Update();    
	virtual string			GetName();
	virtual void			SetName(const string &sName);// ����control��״̬

	virtual void            SetCamera(CCamera *pCamera);  // �������

	// virtual void SetMouseState(const MouseState &ms);                  // �������״̬
	// virtual void SetKeyboardState(const OIS::Keyboard *pKeyboard);     // ���ü���״̬
	// virtual void SetFrameState(const FrameEvent &FrameEvent)           // ����֡��Ϣ

public:

	CEditor(CScene* pMgr, const EditorInfo &info);
	~CEditor();

	void SetEditorInfo(const EditorInfo &Info);  // ���ò���
	void GetEditorInfo(EditorInfo &Info);        // ��ȡ����

	CScene * GetSceneManager();

	void LookAt(const VECTOR3 &v3, INT lookatType = 1);  // editor��Ŀ��㡣lookatType��0���������� 1�������е�һ��  2��ģ��3�������Ŀ���

	void LookAtModel(CModel *pModel); // lookat ĳ��ģ��
	CModel* GetTargetModel(); // �õ�lookat��ģ��

private:
	EditorInfo        m_EditorInfo;		    // Editor������Ϣ

	VECTOR3     m_TranslateVector;
	float       m_MoveScale;
	float       m_RotScale;

	CSceneNode*       m_TargetNode;              // Editorʼ�ճ����Ŀ��㣬������󶨣����Է����ʵ���������ת

	// Entity*          m_pTargetModel;            // lookat��Ŀ�����

	CScene  *m_pSceneMgr;            // ����������

	//int m_disX; // �����ק����Ļλ��
	//int m_disY; // �����ק����Ļλ��
	//static int       m_EditorIndex;             // �����ĵڼ���editor

	FILE *m_pFile;

};