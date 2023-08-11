/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // CEditor.h
* 内容摘要： // 物件展示过程类定义
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // 傅新波
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
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

// Editor 控制类
class VGS_EXPORT CEditor : public IController
{

public : // interface

	virtual CONTROLTYPE		GetControlType();  // 得到控制器类型	
	virtual void			Update();    
	virtual string			GetName();
	virtual void			SetName(const string &sName);// 更新control的状态

	virtual void            SetCamera(CCamera *pCamera);  // 设置相机

	// virtual void SetMouseState(const MouseState &ms);                  // 设置鼠标状态
	// virtual void SetKeyboardState(const OIS::Keyboard *pKeyboard);     // 设置键盘状态
	// virtual void SetFrameState(const FrameEvent &FrameEvent)           // 设置帧信息

public:

	CEditor(CScene* pMgr, const EditorInfo &info);
	~CEditor();

	void SetEditorInfo(const EditorInfo &Info);  // 设置参数
	void GetEditorInfo(EditorInfo &Info);        // 获取参数

	CScene * GetSceneManager();

	void LookAt(const VECTOR3 &v3, INT lookatType = 1);  // editor的目标点。lookatType：0：场景中心 1：场景中的一点  2：模型3：相机的目标点

	void LookAtModel(CModel *pModel); // lookat 某个模型
	CModel* GetTargetModel(); // 得到lookat的模型

private:
	EditorInfo        m_EditorInfo;		    // Editor参数信息

	VECTOR3     m_TranslateVector;
	float       m_MoveScale;
	float       m_RotScale;

	CSceneNode*       m_TargetNode;              // Editor始终朝向的目标点，跟相机绑定，可以方便的实现相机的旋转

	// Entity*          m_pTargetModel;            // lookat的目标对象

	CScene  *m_pSceneMgr;            // 场景管理器

	//int m_disX; // 鼠标拖拽的屏幕位移
	//int m_disY; // 鼠标拖拽的屏幕位移
	//static int       m_EditorIndex;             // 创建的第几个editor

	FILE *m_pFile;

};