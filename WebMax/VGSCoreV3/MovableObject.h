
#pragma once

#include "VGSHeader.h"
#include "VGSDef.h"
#include "Node.h"

class VGS_EXPORT CMovableObject
{
public:
	CMovableObject();
	virtual ~CMovableObject();
	virtual VOID Release();

public:

	// ����¼����������Ҽ�
	const std::string&  getMouseOverFuncName(){return m_sMouseOverFunc;}
	const std::string&  getMouseDBClickFuncName(){return m_sMouseDBClickFunc;}
	const std::string&  getMouseMoveFuncName(){return m_sMouseMoveFunc;}
	const std::string&  getMouseLeaveFuncName(){return m_sMouseLeaveFunc;}
	VOID  setMouseOverFuncName(const std::string &sFuncName){m_sMouseOverFunc = sFuncName;}
	VOID  setMouseDBClickFuncName(const std::string &sFuncName){m_sMouseDBClickFunc = sFuncName;}
	VOID  setMouseMoveFuncName(const std::string &sFuncName){m_sMouseMoveFunc = sFuncName;}
	VOID  setMouseLeaveFuncName(const std::string &sFuncName){m_sMouseLeaveFunc = sFuncName;}

	// �������¼�
	//std::string  getMouseLClickFuncName(){return m_sMouseLClickFunc;}
	const std::string&  getMouseLDwonFuncName(){return m_sMouseLDownFunc;}
	const std::string&  getMouseLUpFuncName(){return m_sMouseLUpFunc;}
	//VOID  setMouseLClickFuncName(const std::string &sFuncName){m_sMouseLClickFunc = sFuncName;}
	VOID  setMouseLDwonFuncName(const std::string &sFuncName){m_sMouseLDownFunc = sFuncName;}
	VOID  setMouseLUpFuncName(const std::string &sFuncName){m_sMouseLUpFunc = sFuncName;}

	// ����Ҽ��¼�
	const std::string&  getMouseRDownFuncName(){return m_sMouseRDownFunc;}
	const std::string&  getMouseRUpFuncName(){return m_sMouseRUpFunc;}
	VOID  setMouseRDownFuncName(const std::string &sFuncName){m_sMouseRDownFunc = sFuncName;}
	VOID  setMouseRUpFuncName(const std::string &sFuncName){m_sMouseRUpFunc = sFuncName;}

	VGS_MOVABLETYPE getType(){return m_type;}
	VOID setType(VGS_MOVABLETYPE type){m_type = type;}

	std::string  getName(){return m_sName;}
	VOID setName(const std::string sName){m_sName = sName;}

	// �����¼���������
	VOID setEventFuncName(VGS_MOUSEEVENT_TYPE type, const std::string &sFuncName);

	// ��ȡ�¼���������
	std::string getEventFuncName(VGS_MOUSEEVENT_TYPE type);

	// void notifyNodeTransform(){m_bParentTransformChanged = true;}   // ֪ͨ��object���ڵ�ı仯�������˱仯

public:
	// ���ڵ�任�������˱仯�ǣ���������ĸ���
	virtual void UpdateFromNode(){}

	CSceneNode*			m_pNode;   // ���ڵ�

protected:
	std::string   m_sMouseOverFunc;
	std::string   m_sMouseDBClickFunc;
	//std::string   m_sMouseLClickFunc;
	std::string   m_sMouseLDownFunc;
	std::string   m_sMouseLUpFunc;
	std::string   m_sMouseRDownFunc;
	std::string   m_sMouseRUpFunc;
	std::string   m_sMouseMoveFunc;
	std::string   m_sMouseLeaveFunc;

	VGS_MOVABLETYPE   m_type;
	std::string		  m_sName;

	// bool          m_bParentTransformChanged;   // ���ڵ�ı任�����Ƿ����˱仯

};