/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // AnimationManager.h
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

#pragma once

#include "..\VGSHeader.h"
#include "NodeAnimation.h"

#include "..\CCamera.h"
#include "..\CLight.h"
//#include "..\VGSSingelton.h"
//#include "..\VGSCore.h"

#include <vector>
#include <map>

using namespace std;

typedef struct  VGS_EXPORT AnimStatus
{
	VGS_ANIMATIONSTATUS playstatus;  // 播放状态
	int velocity;                    // 播放速度，如果等于0，不设置velocity
	AnimStatus()
	{
		playstatus = VAS_PLAYING;
		velocity   = 1;
	}
	AnimStatus(VGS_ANIMATIONSTATUS status, int vel)
	{
		playstatus = status;
		velocity   = vel;
	}
}AnimStatus;


class VGS_EXPORT CAnimationManager //: public Singleton<CAnimationManager>
{
public:
	CAnimationManager();
	~CAnimationManager();

	//static CAnimationManager* getSingletonPtr();
	//static CAnimationManager& getSingleton();

	void Release();  // 删除动画，释放内存。只处理灯光和相机的动画即可，其他的ogre自己管理

	// 获取所有动画的数目
	UINT getAllAnimationCount();

	/** 获取指定类型动画的数目
	@param in type : VAT_KEYFRM：关键帧动画，VAT_CAMERA：相机动画，VAT_LIGHT：灯光动画, VAT_SKELETON:骨骼动画
	*/
	DWORD GetAnimationCount(VGS_ANIMATIONTYPE type);

	/** 根据索引得到动画的名称
	@param in type  : VAT_KEYFRM：关键帧动画，VAT_CAMERA：相机动画，VAT_LIGHT：灯光动画, VAT_SKELETON:骨骼动画
	@param in index : 动画索引
	@param out sName : 名称
	@return 得到名称，返回true，否则false
	*/
	bool GetAnimationNameByIndex(VGS_ANIMATIONTYPE type, int index, char* sName);

	void AddNodeKeyfrmAnimation(CNodeAnimation* pAnim);

	// 删除关键帧动画
	void DestroyNodeKeyFrmAnimation(CSceneNode* pNode);

	// 销毁所有关键帧动画
	VOID destroyAllNodeKeyframeAnimation();

	// 销毁所有动画
	VOID destroyAllAnimation();

	void SetAllAnimStatus(AnimStatus status);                                // 设置所有动画的播放状态
	void SetAnimStatus(const std::string &sAnimName, AnimStatus status);    // 设置指定动画的播放状态
	void GetAnimStatus(const std::string &sAnimName, AnimStatus &status);
	void PlayOneFrame(double AddTime);                                       // 播放一帧动画，AddTime:当前时间上增加的时间

	INT GetAnimFramCount(const std::string &sAnimName);
	INT GetCurrentFrame(const std::string &sAnimName);
	void SetCurrentFrame(const std::string &sAnimName, int FrameIdx);

	// 获取/设置动画的播放次数
	BOOL  GetAnimationLoop(const std::string &sAnimName);
	void  SetAnimationLoop(const std::string &sAnimName, bool bLoop);

	// 根据名称得到动画的指针
	CNodeAnimation* getKeyFrameByName(const std::string &sName);


	// 获取节点动画的名称
	std::string GetNodeAnimationName(CSceneNode *pNode);

	//  创建相机关键帧动画
	CNodeAnimation* CreateCameraAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform, 
		const std::vector<CVGS_CAMERA_ANIMINFO> &vCamInfo);
	//  创建灯光键帧动画
	CNodeAnimation* CreateLightAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform, 
		VGS_LIGHT_TYPE lightType, const std::vector<CVGS_LIGHT_INFO> &vLightInfo);

	//  创建模型关键帧动画
	CNodeAnimation* CreateModelAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform);


private:

	//包含node关键帧动画列表
	vector <CNodeAnimation*> m_vKeyfrmList;

	// 名称和状态映射列表，保存了相机和 string : 动画的名称, AnimStatus : 播放状态
	std::map<DWORD, AnimStatus> m_mAnimStatus;       // DWORD : 动画的句柄, AnimStatus : 播放状态

	//播放骨骼动画
	void PlaySkeletonAnimation(double AddTime);

	//播放节点关键帧动画
	void PlayNodeAnimation(double AddTime);

	void UpdateAnimScene();

	// 根据动画名称获取动画句柄及类型， type ：返回动画的类型
	DWORD GetAnimationHandleByName(const std::string sAnimName, VGS_ANIMATIONTYPE &type);


};