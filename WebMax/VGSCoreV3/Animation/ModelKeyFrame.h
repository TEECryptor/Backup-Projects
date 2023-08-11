
#pragma once

#include "NodeKeyFrame.h"
#include "..\VGSHeader.h"
#include "..\Node.h"
#include "..\CModel.h"

class VGS_EXPORT CModelKeyFrame : public CNodeKeyFrame
{
public:
	CModelKeyFrame(CSceneNode *pNode);
	~CModelKeyFrame();
	
	VOID Release(); 
	VOID update();  // ÖØÔØ·½·¨
	VGS_ANIMATIONTYPE getType();

private:
	CModel*    m_pModel;
};