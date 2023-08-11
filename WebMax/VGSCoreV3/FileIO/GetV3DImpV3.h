//----------------------------------------------------------
#pragma once
//----------------------------------------------------------
#include "GetV3DImpV2.h"
#include "V3D_DefV3.h"
//----------------------------------------------------------
class CGetV3DImpV3 : public CGetV3DImpV2
{
public:
	CGetV3DImpV3(void);
	~CGetV3DImpV3(void);
	//------------------------------------------------------
protected:
	// 读取所有数据
	virtual DWORD ReadAllData();

	virtual DWORD ReadMaterialData();
	virtual DWORD ReadCameraData();
	virtual DWORD ReadLightData();
	virtual DWORD ReadMeshData();
	virtual DWORD ReadModelData();
	//
	virtual DWORD ReadOverlayData();
	virtual DWORD ReadNavigaterData();
	virtual DWORD ReadButtonData();
	virtual DWORD ReadBackdropData();
	virtual DWORD ReadControlData();

	//递归创建场景
	virtual void ConstructSceneGraph(CSceneNode* pNode, char* offset);
	
	//读入该节点的动画数据
	virtual void ReadANodeAnimation(CSceneNode* pSceneNode, VGS_Node* pNode, char* offset);
	//------------------------------------------------------
};

//----------------------------------------------------------