/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // AnimationManager.cpp
* ����ժҪ�� // ���ж���ͳһ����ӿ�  
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

#include "AnimationManager.h" 
#include "NodeKeyFrame.h"
#include "CameraKeyFrame.h"
#include "LightKeyFrame.h"
#include "ModelKeyFrame.h"


#pragma   warning(disable:4786)  // ��ʱ���ε�warning

//template<> CAnimationManager* Singleton<CAnimationManager>::ms_Singleton = 0;

CAnimationManager::CAnimationManager()
{
}
CAnimationManager::~CAnimationManager()
{	
	Release();
}

//CAnimationManager* CAnimationManager::getSingletonPtr()
//{
//	return ms_Singleton;
//}
//CAnimationManager& CAnimationManager::getSingleton()
//{
//	assert(ms_Singleton);  
//	return (*ms_Singleton);
//}

// ɾ���������ͷ��ڴ�
// ֻ��Ҫ����ƹ������Ķ������ɣ�������ogre�Լ�����
void CAnimationManager::Release()
{
	UINT count = m_vKeyfrmList.size();
	for (UINT iCam = 0; iCam < count; ++iCam)
	{
		m_vKeyfrmList[iCam]->Release();
	}

	m_mAnimStatus.clear();
}

// ��ȡ���ж�������Ŀ
UINT CAnimationManager::getAllAnimationCount()
{
	UINT count = 0;
	for (INT i = (INT)VAT_ANIMATIONTYPE_MODEL; i <= (INT)VAT_ANIMATIONTYPE_SKELETON; ++i)
	{
		VGS_ANIMATIONTYPE type = (VGS_ANIMATIONTYPE)i;
		count += GetAnimationCount(type);
	}
	return count;
}

/** ��ȡָ�����Ͷ�������Ŀ
@param in type : VAT_MODEL��model�ؼ�֡������VAT_CAMERA�����������VAT_LIGHT���ƹ⶯��, VAT_SKELETON:��������
*/
DWORD CAnimationManager::GetAnimationCount(VGS_ANIMATIONTYPE type)
{
	switch (type)
	{
	case VAT_ANIMATIONTYPE_MODEL:
		{
			//�����еĹؼ�֡����
			int iCount = 0;
			int count = m_vKeyfrmList.size();
			for (int i = 0; i < count; ++i)
			{
				if (m_vKeyfrmList[i]->getType() == VAT_ANIMATIONTYPE_MODEL)
				{
					++iCount;
				}
			}
			return iCount;
		}
 	case VAT_ANIMATIONTYPE_CAMERA:
		{
			//�����еĹؼ�֡����
			int iCount = 0;
			int count = m_vKeyfrmList.size();
			for (int i = 0; i < count; ++i)
			{
				if (m_vKeyfrmList[i]->getType() == VAT_ANIMATIONTYPE_CAMERA)
				{
					++iCount;
				}
			}
			return iCount;
		}
 	case VAT_ANIMATIONTYPE_LIGHT:
		{
			//�����еĹؼ�֡����
			int iCount = 0;
			int count = m_vKeyfrmList.size();
			for (int i = 0; i < count; ++i)
			{
				if (m_vKeyfrmList[i]->getType() == VAT_ANIMATIONTYPE_LIGHT)
				{
					++iCount;
				}
			}
			return iCount;
		}
// 	case VAT_SKELETON:
// 		return DWORD(m_vSkeletonEntityList.size());
	default:
		return 0;
	}
}

/** ���������õ�����������
@param in type  : VAT_KEYFRM���ؼ�֡������VAT_CAMERA�����������VAT_LIGHT���ƹ⶯��, VAT_SKELETON:��������
@param in index : ��������
@param out sName : ����
@return �õ����ƣ�����true������false
*/
bool CAnimationManager::GetAnimationNameByIndex(VGS_ANIMATIONTYPE type, int index, char* sName)
{
	switch (type)
	{
	case VAT_ANIMATIONTYPE_MODEL:
		{
			//�����еĹؼ�֡����
			int iCount = 0;
			int count = m_vKeyfrmList.size();
			for (int i = 0; i < count; ++i)
			{
				if (m_vKeyfrmList[i]->getType() == VAT_ANIMATIONTYPE_MODEL)
				{
					if (iCount++ == index)
					{
						COPY_NAME(sName, m_vKeyfrmList[i]->getName().c_str());
						return true;
					}
				}
			}
			return false;
		}
	case VAT_ANIMATIONTYPE_CAMERA:
		{
			//�����еĹؼ�֡����
			int iCount = 0;
			int count = m_vKeyfrmList.size();
			for (int i = 0; i < count; ++i)
			{
				if (m_vKeyfrmList[i]->getType() == VAT_ANIMATIONTYPE_CAMERA)
				{
					if (iCount++ == index)
					{
						COPY_NAME(sName, m_vKeyfrmList[i]->getName().c_str());
						return true;
					}
				}
			}
			return false;
		}
	case VAT_ANIMATIONTYPE_LIGHT:
		{
			//�����еĹؼ�֡����
			int iCount = 0;
			int count = m_vKeyfrmList.size();
			for (int i = 0; i < count; ++i)
			{
				if (m_vKeyfrmList[i]->getType() == VAT_ANIMATIONTYPE_LIGHT)
				{
					if (iCount++ == index)
					{
						COPY_NAME(sName, m_vKeyfrmList[i]->getName().c_str());
						return true;
					}
				}
			}
			return false;
		}
	case VAT_ANIMATIONTYPE_SKELETON:
		{
// 			if (index >= 0 && index < (int)m_vSkeletonEntityList.size())
// 			{
// 				Entity *pEntity = m_vSkeletonEntityList[index];
// 				std::string str = pSceneMgr->GetVGSEntityNamebyHandle(DWORD(pEntity));
// 				COPY_NAME(sName, str.c_str());
// 				return true;
// 			}
		}
	default:
		return false;
	}

	return false;
}

// ����һ��Node�ؼ�֡����
void CAnimationManager::AddNodeKeyfrmAnimation(CNodeAnimation* pAnim)
{
	m_vKeyfrmList.push_back(pAnim);
	m_mAnimStatus[DWORD(pAnim)] = AnimStatus(VAS_PLAYING, 1);
}

// ɾ���ؼ�֡����
void CAnimationManager::DestroyNodeKeyFrmAnimation(CSceneNode* pNode)
{
	std::vector<CNodeAnimation*>::iterator it = m_vKeyfrmList.begin();
	for (; it != m_vKeyfrmList.end(); ++it)
	{
		if ((*it)->getNode() == pNode)
		{
			SAFE_DELETE(*it);
			m_vKeyfrmList.erase(it);
			break;
		}
	}
}

// �������йؼ�֡����
VOID CAnimationManager::destroyAllNodeKeyframeAnimation()
{
	std::vector<CNodeAnimation*>::iterator it = m_vKeyfrmList.begin();
	for (; it != m_vKeyfrmList.end(); ++it)
	{
		SAFE_DELETE(*it);
	}
	m_vKeyfrmList.clear();
}
// �������ж���
VOID CAnimationManager::destroyAllAnimation()
{
	// ���ٹؼ�֡����
	destroyAllNodeKeyframeAnimation();
}

// �������ж����Ĳ���״̬
void CAnimationManager::SetAllAnimStatus(AnimStatus status)
{
	map<DWORD, AnimStatus>::iterator it = m_mAnimStatus.begin();
	for (; it != m_mAnimStatus.end(); ++it)
	{
		if (status.playstatus != VAS_NONE)
		{
			it->second.playstatus = status.playstatus;
		}
		if (status.velocity != VGS_ANIMSPEED_NONE)  // ���velocity == VGS_ANIMSPEED_NONE, �����á�һ����ͣ���Ŷ�����ʱ����õ�
		{
			it->second.velocity = status.velocity;
		}
	}
}

void CAnimationManager::SetAnimStatus(const std::string &sAnimName, AnimStatus status)
{
	VGS_ANIMATIONTYPE type;
	DWORD hAnim = GetAnimationHandleByName(sAnimName, type);
	map<DWORD, AnimStatus>::iterator it = m_mAnimStatus.find(hAnim);
	if (it != m_mAnimStatus.end())
	{
		if (status.playstatus != VAS_NONE)
		{
			it->second.playstatus = status.playstatus;
		}
		if (status.velocity != VGS_ANIMSPEED_NONE)  // ���velocity == VGS_ANIMSPEED_NONE, �����á�һ����ͣ���Ŷ�����ʱ����õ�
		{
			it->second.velocity = status.velocity;
		}
	}
}

void CAnimationManager::GetAnimStatus(const std::string &sAnimName, AnimStatus &status)
{
	VGS_ANIMATIONTYPE type;
	DWORD hAnim = GetAnimationHandleByName(sAnimName, type);
	map<DWORD, AnimStatus>::iterator it = m_mAnimStatus.find(hAnim);
	if (it != m_mAnimStatus.end())
	{
		status.playstatus = it->second.playstatus;
		status.velocity = it->second.velocity;
	}
}

// ���ݶ������ƻ�ȡ�������������
// type out : ����������
DWORD CAnimationManager::GetAnimationHandleByName(const std::string sAnimName, VGS_ANIMATIONTYPE &type)
{
	CNodeAnimation* pAnim = getKeyFrameByName(sAnimName);
	if (pAnim)
	{
		type = pAnim->getType();
	}
	return (DWORD)pAnim;
}

void CAnimationManager::PlayOneFrame(double AddTime)
{
	//���Ź�������
	//PlaySkeletonAnimation(AddTime);

	//���Žڵ�ؼ�֡����
	PlayNodeAnimation(AddTime);
}

//���Žڵ�ؼ�֡����
void CAnimationManager::PlayNodeAnimation(double AddTime) 
{	
	std::vector<CNodeAnimation*>::iterator it = m_vKeyfrmList.begin();
	for (; it != m_vKeyfrmList.end(); ++it)
	{
		// ��鶯���Ĳ���״̬
		std::map<DWORD, AnimStatus>::iterator statusIter = m_mAnimStatus.find((DWORD)(*it));
		if (statusIter != m_mAnimStatus.end())
		{
			if ((*statusIter).second.playstatus == VAS_PLAYING) // ֻ���¡����ڲ���״̬���Ķ���
			{
				(*it)->addTime(AddTime);
				(*it)->update();
			}
		}
	}

	/*
	if( CSkeleton::m_skeletonsList.size() > 0 )
	{
		int cc = CSkeleton::m_skeletonsList[0]->m_vSkeletonAnim.size();
		if( cc > 0 )
		{
			int c_keyframe = CSkeleton::m_skeletonsList[0]->m_vSkeletonAnim[0]->m_vKeyfrmList.size();
			std::vector<CNodeAnimation*> & vNodeAnimations = CSkeleton::m_skeletonsList[0]->m_vSkeletonAnim[0]->m_vKeyfrmList;
			
			for( int i = 0;i<c_keyframe;i++ )
			{
				vNodeAnimations[i]->addTime(AddTime);	
				vNodeAnimations[i]->update();			
			}	
		}
	}
	*/

	UpdateAnimScene();
}

void CAnimationManager::UpdateAnimScene()
{
}

INT CAnimationManager::GetAnimFramCount(const std::string &sAnimName)
{
	CNodeAnimation *pAnim = getKeyFrameByName(sAnimName);
	if (pAnim)
	{
		return pAnim->getFrameCount();
	}
	return 0;
}
INT CAnimationManager::GetCurrentFrame(const std::string &sAnimName)
{
	CNodeAnimation* pAnim = getKeyFrameByName(sAnimName);
	if (pAnim)
	{
		return pAnim->getCurFrame();
	}
	return 0;
}
void CAnimationManager::SetCurrentFrame(const std::string &sAnimName, int FrameIdx)
{
	CNodeAnimation* pAnim = getKeyFrameByName(sAnimName);
	if (pAnim)
	{
		pAnim->setTimePosition(FrameIdx / (double)gkfrPlaySpeed);
	}
}

// ��ȡ�����Ĳ��Ŵ���
BOOL CAnimationManager::GetAnimationLoop(const std::string &sAnimName)
{
	CNodeAnimation* pAnim = getKeyFrameByName(sAnimName);
	if (pAnim)
	{
		return pAnim->getLoop();
	}
	return false;
}

//���ö����Ĳ��Ŵ���
void CAnimationManager::SetAnimationLoop(const std::string &sAnimName, bool bLoop)
{
	CNodeAnimation* pAnim = getKeyFrameByName(sAnimName);
	if (pAnim)
	{
		pAnim->setLoop(bLoop);
	}
}

// �������Ƶõ�������ָ��
CNodeAnimation* CAnimationManager::getKeyFrameByName(const std::string &sName)
{	
	std::vector<CNodeAnimation*>::iterator it = m_vKeyfrmList.begin();
	for (; it != m_vKeyfrmList.end(); ++it)
	{
		if ( strcmp((*it)->getName().c_str(), sName.c_str()) == 0)
		{
			return (*it);
		}
	}
	return 0;
}


// ��ȡ�ڵ㶯��������
std::string CAnimationManager::GetNodeAnimationName(CSceneNode *pNode)
{
	std::string strAnimName = std::string("Anim_");
	strAnimName.append(pNode->name);

	if (pNode->getMovableObject() != 0)
	{
		if (pNode->getMovableType() == VGS_MOVABLETYPE_CAMERA)
		{
			strAnimName = ((CCamera*)pNode->getMovableObject())->getName();
		}
		else if (pNode->getMovableType() == VGS_MOVABLETYPE_LIGHT)
		{
			strAnimName = ((CLight*)pNode->getMovableObject())->getName();
		}
		else if (pNode->getMovableType() == VGS_MOVABLETYPE_MODEL)
		{
			strAnimName = ((CModel*)pNode->getMovableObject())->getName();
		}
	}
	return strAnimName;
}

//  ��������ؼ�֡����
CNodeAnimation* CAnimationManager::CreateCameraAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform, 
									   const std::vector<CVGS_CAMERA_ANIMINFO> &vCamInfo)
{
	if (!pNode)
	{
		return 0;
	}

	UINT iTransCount = vTransform.size();
	UINT iCamInfoCount = vCamInfo.size();
	if (iTransCount != iCamInfoCount || (iTransCount == 0 && iCamInfoCount == 0))
	{
		return 0;
	}

	// �����ڵ�Ĺؼ�֡����
	std::string strAnimName = GetNodeAnimationName(pNode);
	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  
	if (!pAnim)
	{
		return 0;
	}

	bool bHasParamAnimation = (vCamInfo.size() > 0);  // �Ƿ��в�������

	// ����ÿһ֡
	for (UINT iFrame = 0; iFrame < iTransCount; ++iFrame)
	{
		CCameraKeyFrame *pCurFrame = new CCameraKeyFrame(pNode);

		// �ڵ�任
		const CNodeTransform &transform = vTransform[iFrame];
		pCurFrame->setTranslate(transform.position);
		pCurFrame->setRotation(transform.quat);
		pCurFrame->setScale(transform.scale);

		// �������
		pCurFrame->m_bHasParamAnimation = bHasParamAnimation;
		if (bHasParamAnimation)
		{
			const CVGS_CAMERA_ANIMINFO &camInfo = vCamInfo[iFrame]; 
			pCurFrame->setFov(camInfo.fov);
			pCurFrame->setNearClip(camInfo.nearClip);
			pCurFrame->setFarClip(camInfo.farClip);
		}

		pAnim->addKeyframe(pCurFrame); // ���һ֡����
	}
	pAnim->setStartPlayTime(0);
	pAnim->setLength(iTransCount / (double)gkfrPlaySpeed);

	// ���뵽����������
	AddNodeKeyfrmAnimation(pAnim);
	return pAnim;
}
//  �����ƹ��֡����
CNodeAnimation* CAnimationManager::CreateLightAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform, 
									  VGS_LIGHT_TYPE lightType, const std::vector<CVGS_LIGHT_INFO> &vLightInfo)
{
	if (!pNode)
	{
		return 0;
	}

	UINT iTransCount = vTransform.size();
	UINT iLightInfoCount = vLightInfo.size();
	if (iTransCount != iLightInfoCount || (iTransCount == 0 && iLightInfoCount == 0))
	{
		return 0;
	}

	// �����ڵ�Ĺؼ�֡����
	std::string strAnimName = GetNodeAnimationName(pNode);
	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  
	if (!pAnim)
	{
		return 0;
	}

	// ����ÿһ֡
	bool bHasParamAnimation = (vLightInfo.size() > 0);  // �Ƿ��в�������
	for (UINT iFrame = 0; iFrame < iTransCount; ++iFrame)
	{
		CLightKeyFrame *pCurFrame = new CLightKeyFrame(pNode);

		// �ڵ�任
		const CNodeTransform &transform = vTransform[iFrame];
		pCurFrame->setScale(transform.scale);
		pCurFrame->setRotation(transform.quat);
		pCurFrame->setTranslate(transform.position);

		// �ƹ����
		pCurFrame->m_bHasParamAnimation = bHasParamAnimation;
		if (bHasParamAnimation)
		{
			const CVGS_LIGHT_INFO &lightInfo = vLightInfo[iFrame]; //��ɫ

			float r = ((lightInfo.color >> 16) & 0xff) / 255.0f * lightInfo.multiply;
			float g = ((lightInfo.color >> 8) & 0xff) / 255.0f * lightInfo.multiply;
			float b = ((lightInfo.color) & 0xff) / 255.0f * lightInfo.multiply;
			pCurFrame->setColor(r, g, b, 1.0f);
			pCurFrame->setRange(lightInfo.range2);		
			pCurFrame->setAttenuation();

			if (lightType == VLT_DIR)
			{
				pCurFrame->setDirection(VECTOR3(lightInfo.dir.x, lightInfo.dir.y, lightInfo.dir.z));
				pCurFrame->setAngle(lightInfo.theta, lightInfo.phi);
			}
			else if (lightType == VLT_SPOT)
			{
				pCurFrame->setDirection(VECTOR3(lightInfo.dir.x, lightInfo.dir.y, lightInfo.dir.z));
			}
		}
		pAnim->addKeyframe(pCurFrame); // ���һ֡����

	}
	pAnim->setStartPlayTime(0);
	pAnim->setLength(iTransCount / (double)gkfrPlaySpeed);

	// ���뵽����������
	AddNodeKeyfrmAnimation(pAnim);
	return pAnim;
}
//  ����ģ�͹ؼ�֡����
CNodeAnimation* CAnimationManager::CreateModelAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform)
{
	if (!pNode || vTransform.empty())
	{
		return 0;
	}

	// �����ڵ�Ĺؼ�֡����
	std::string strAnimName = GetNodeAnimationName(pNode);
	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  
	if (!pAnim)
	{
		return 0;
	}

	if (pAnim)
	{
		// ����ÿһ֡
		UINT iFrameCount = vTransform.size();
		for (UINT iFrame = 0; iFrame < iFrameCount; ++iFrame)
		{		
			CModelKeyFrame* pCurFrame = new CModelKeyFrame(pNode);
			const CNodeTransform &transform = vTransform[iFrame];
			pCurFrame->setScale(transform.scale);
			pCurFrame->setTranslate(transform.position);
			pCurFrame->setRotation(transform.quat);

			pAnim->addKeyframe(pCurFrame); // ���һ֡����
		}
		pAnim->setStartPlayTime(0);
		pAnim->setLength(iFrameCount / (double)gkfrPlaySpeed);


		// ���뵽����������
		AddNodeKeyfrmAnimation(pAnim);
	}
	return pAnim;
}