/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // AnimationManager.cpp
* 内容摘要： // 所有动画统一管理接口  
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

#include "AnimationManager.h" 
#include "NodeKeyFrame.h"
#include "CameraKeyFrame.h"
#include "LightKeyFrame.h"
#include "ModelKeyFrame.h"


#pragma   warning(disable:4786)  // 暂时屏蔽掉warning

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

// 删除动画，释放内存
// 只需要处理灯光和相机的动画即可，其他的ogre自己管理
void CAnimationManager::Release()
{
	UINT count = m_vKeyfrmList.size();
	for (UINT iCam = 0; iCam < count; ++iCam)
	{
		m_vKeyfrmList[iCam]->Release();
	}

	m_mAnimStatus.clear();
}

// 获取所有动画的数目
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

/** 获取指定类型动画的数目
@param in type : VAT_MODEL：model关键帧动画，VAT_CAMERA：相机动画，VAT_LIGHT：灯光动画, VAT_SKELETON:骨骼动画
*/
DWORD CAnimationManager::GetAnimationCount(VGS_ANIMATIONTYPE type)
{
	switch (type)
	{
	case VAT_ANIMATIONTYPE_MODEL:
		{
			//场景中的关键帧动画
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
			//场景中的关键帧动画
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
			//场景中的关键帧动画
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

/** 根据索引得到动画的名称
@param in type  : VAT_KEYFRM：关键帧动画，VAT_CAMERA：相机动画，VAT_LIGHT：灯光动画, VAT_SKELETON:骨骼动画
@param in index : 动画索引
@param out sName : 名称
@return 得到名称，返回true，否则false
*/
bool CAnimationManager::GetAnimationNameByIndex(VGS_ANIMATIONTYPE type, int index, char* sName)
{
	switch (type)
	{
	case VAT_ANIMATIONTYPE_MODEL:
		{
			//场景中的关键帧动画
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
			//场景中的关键帧动画
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
			//场景中的关键帧动画
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

// 增加一个Node关键帧动画
void CAnimationManager::AddNodeKeyfrmAnimation(CNodeAnimation* pAnim)
{
	m_vKeyfrmList.push_back(pAnim);
	m_mAnimStatus[DWORD(pAnim)] = AnimStatus(VAS_PLAYING, 1);
}

// 删除关键帧动画
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

// 销毁所有关键帧动画
VOID CAnimationManager::destroyAllNodeKeyframeAnimation()
{
	std::vector<CNodeAnimation*>::iterator it = m_vKeyfrmList.begin();
	for (; it != m_vKeyfrmList.end(); ++it)
	{
		SAFE_DELETE(*it);
	}
	m_vKeyfrmList.clear();
}
// 销毁所有动画
VOID CAnimationManager::destroyAllAnimation()
{
	// 销毁关键帧动画
	destroyAllNodeKeyframeAnimation();
}

// 设置所有动画的播放状态
void CAnimationManager::SetAllAnimStatus(AnimStatus status)
{
	map<DWORD, AnimStatus>::iterator it = m_mAnimStatus.begin();
	for (; it != m_mAnimStatus.end(); ++it)
	{
		if (status.playstatus != VAS_NONE)
		{
			it->second.playstatus = status.playstatus;
		}
		if (status.velocity != VGS_ANIMSPEED_NONE)  // 如果velocity == VGS_ANIMSPEED_NONE, 不设置。一般暂停播放动画的时候会用到
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
		if (status.velocity != VGS_ANIMSPEED_NONE)  // 如果velocity == VGS_ANIMSPEED_NONE, 不设置。一般暂停播放动画的时候会用到
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

// 根据动画名称获取动画句柄及类型
// type out : 动画的类型
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
	//播放骨骼动画
	//PlaySkeletonAnimation(AddTime);

	//播放节点关键帧动画
	PlayNodeAnimation(AddTime);
}

//播放节点关键帧动画
void CAnimationManager::PlayNodeAnimation(double AddTime) 
{	
	std::vector<CNodeAnimation*>::iterator it = m_vKeyfrmList.begin();
	for (; it != m_vKeyfrmList.end(); ++it)
	{
		// 检查动画的播放状态
		std::map<DWORD, AnimStatus>::iterator statusIter = m_mAnimStatus.find((DWORD)(*it));
		if (statusIter != m_mAnimStatus.end())
		{
			if ((*statusIter).second.playstatus == VAS_PLAYING) // 只更新“正在播放状态”的动画
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

// 获取动画的播放次数
BOOL CAnimationManager::GetAnimationLoop(const std::string &sAnimName)
{
	CNodeAnimation* pAnim = getKeyFrameByName(sAnimName);
	if (pAnim)
	{
		return pAnim->getLoop();
	}
	return false;
}

//设置动画的播放次数
void CAnimationManager::SetAnimationLoop(const std::string &sAnimName, bool bLoop)
{
	CNodeAnimation* pAnim = getKeyFrameByName(sAnimName);
	if (pAnim)
	{
		pAnim->setLoop(bLoop);
	}
}

// 根据名称得到动画的指针
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


// 获取节点动画的名称
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

//  创建相机关键帧动画
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

	// 创建节点的关键帧动画
	std::string strAnimName = GetNodeAnimationName(pNode);
	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  
	if (!pAnim)
	{
		return 0;
	}

	bool bHasParamAnimation = (vCamInfo.size() > 0);  // 是否有参数动画

	// 计算每一帧
	for (UINT iFrame = 0; iFrame < iTransCount; ++iFrame)
	{
		CCameraKeyFrame *pCurFrame = new CCameraKeyFrame(pNode);

		// 节点变换
		const CNodeTransform &transform = vTransform[iFrame];
		pCurFrame->setTranslate(transform.position);
		pCurFrame->setRotation(transform.quat);
		pCurFrame->setScale(transform.scale);

		// 相机参数
		pCurFrame->m_bHasParamAnimation = bHasParamAnimation;
		if (bHasParamAnimation)
		{
			const CVGS_CAMERA_ANIMINFO &camInfo = vCamInfo[iFrame]; 
			pCurFrame->setFov(camInfo.fov);
			pCurFrame->setNearClip(camInfo.nearClip);
			pCurFrame->setFarClip(camInfo.farClip);
		}

		pAnim->addKeyframe(pCurFrame); // 添加一帧动画
	}
	pAnim->setStartPlayTime(0);
	pAnim->setLength(iTransCount / (double)gkfrPlaySpeed);

	// 加入到动画管理器
	AddNodeKeyfrmAnimation(pAnim);
	return pAnim;
}
//  创建灯光键帧动画
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

	// 创建节点的关键帧动画
	std::string strAnimName = GetNodeAnimationName(pNode);
	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  
	if (!pAnim)
	{
		return 0;
	}

	// 计算每一帧
	bool bHasParamAnimation = (vLightInfo.size() > 0);  // 是否有参数动画
	for (UINT iFrame = 0; iFrame < iTransCount; ++iFrame)
	{
		CLightKeyFrame *pCurFrame = new CLightKeyFrame(pNode);

		// 节点变换
		const CNodeTransform &transform = vTransform[iFrame];
		pCurFrame->setScale(transform.scale);
		pCurFrame->setRotation(transform.quat);
		pCurFrame->setTranslate(transform.position);

		// 灯光参数
		pCurFrame->m_bHasParamAnimation = bHasParamAnimation;
		if (bHasParamAnimation)
		{
			const CVGS_LIGHT_INFO &lightInfo = vLightInfo[iFrame]; //颜色

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
		pAnim->addKeyframe(pCurFrame); // 添加一帧动画

	}
	pAnim->setStartPlayTime(0);
	pAnim->setLength(iTransCount / (double)gkfrPlaySpeed);

	// 加入到动画管理器
	AddNodeKeyfrmAnimation(pAnim);
	return pAnim;
}
//  创建模型关键帧动画
CNodeAnimation* CAnimationManager::CreateModelAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform)
{
	if (!pNode || vTransform.empty())
	{
		return 0;
	}

	// 创建节点的关键帧动画
	std::string strAnimName = GetNodeAnimationName(pNode);
	CNodeAnimation *pAnim = new CNodeAnimation(pNode, strAnimName);  
	if (!pAnim)
	{
		return 0;
	}

	if (pAnim)
	{
		// 计算每一帧
		UINT iFrameCount = vTransform.size();
		for (UINT iFrame = 0; iFrame < iFrameCount; ++iFrame)
		{		
			CModelKeyFrame* pCurFrame = new CModelKeyFrame(pNode);
			const CNodeTransform &transform = vTransform[iFrame];
			pCurFrame->setScale(transform.scale);
			pCurFrame->setTranslate(transform.position);
			pCurFrame->setRotation(transform.quat);

			pAnim->addKeyframe(pCurFrame); // 添加一帧动画
		}
		pAnim->setStartPlayTime(0);
		pAnim->setLength(iFrameCount / (double)gkfrPlaySpeed);


		// 加入到动画管理器
		AddNodeKeyfrmAnimation(pAnim);
	}
	return pAnim;
}