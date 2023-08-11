
#include "ModelKeyFrame.h"

CModelKeyFrame::CModelKeyFrame(CSceneNode *pNode) : CNodeKeyFrame(pNode)
{
	m_pModel = NULL;
	if (pNode && pNode->getMovableType() == VGS_MOVABLETYPE_MODEL)
	{
		m_pModel = (CModel*)(pNode->getMovableObject());
	}
}

CModelKeyFrame::~CModelKeyFrame()
{
	Release();
}

VOID CModelKeyFrame::Release()
{
	m_pModel = NULL;
}

// ÖØÔØ·½·¨
VOID CModelKeyFrame::update()
{
	CNodeKeyFrame::update();

	if (m_pModel)
	{
		m_pModel->UpdateFromNode();
	}
}

VGS_ANIMATIONTYPE CModelKeyFrame::getType()
{
	return VAT_ANIMATIONTYPE_MODEL;
}