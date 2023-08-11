/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // AnimationManager.h
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
	VGS_ANIMATIONSTATUS playstatus;  // ����״̬
	int velocity;                    // �����ٶȣ��������0��������velocity
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

	void Release();  // ɾ���������ͷ��ڴ档ֻ����ƹ������Ķ������ɣ�������ogre�Լ�����

	// ��ȡ���ж�������Ŀ
	UINT getAllAnimationCount();

	/** ��ȡָ�����Ͷ�������Ŀ
	@param in type : VAT_KEYFRM���ؼ�֡������VAT_CAMERA�����������VAT_LIGHT���ƹ⶯��, VAT_SKELETON:��������
	*/
	DWORD GetAnimationCount(VGS_ANIMATIONTYPE type);

	/** ���������õ�����������
	@param in type  : VAT_KEYFRM���ؼ�֡������VAT_CAMERA�����������VAT_LIGHT���ƹ⶯��, VAT_SKELETON:��������
	@param in index : ��������
	@param out sName : ����
	@return �õ����ƣ�����true������false
	*/
	bool GetAnimationNameByIndex(VGS_ANIMATIONTYPE type, int index, char* sName);

	void AddNodeKeyfrmAnimation(CNodeAnimation* pAnim);

	// ɾ���ؼ�֡����
	void DestroyNodeKeyFrmAnimation(CSceneNode* pNode);

	// �������йؼ�֡����
	VOID destroyAllNodeKeyframeAnimation();

	// �������ж���
	VOID destroyAllAnimation();

	void SetAllAnimStatus(AnimStatus status);                                // �������ж����Ĳ���״̬
	void SetAnimStatus(const std::string &sAnimName, AnimStatus status);    // ����ָ�������Ĳ���״̬
	void GetAnimStatus(const std::string &sAnimName, AnimStatus &status);
	void PlayOneFrame(double AddTime);                                       // ����һ֡������AddTime:��ǰʱ�������ӵ�ʱ��

	INT GetAnimFramCount(const std::string &sAnimName);
	INT GetCurrentFrame(const std::string &sAnimName);
	void SetCurrentFrame(const std::string &sAnimName, int FrameIdx);

	// ��ȡ/���ö����Ĳ��Ŵ���
	BOOL  GetAnimationLoop(const std::string &sAnimName);
	void  SetAnimationLoop(const std::string &sAnimName, bool bLoop);

	// �������Ƶõ�������ָ��
	CNodeAnimation* getKeyFrameByName(const std::string &sName);


	// ��ȡ�ڵ㶯��������
	std::string GetNodeAnimationName(CSceneNode *pNode);

	//  ��������ؼ�֡����
	CNodeAnimation* CreateCameraAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform, 
		const std::vector<CVGS_CAMERA_ANIMINFO> &vCamInfo);
	//  �����ƹ��֡����
	CNodeAnimation* CreateLightAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform, 
		VGS_LIGHT_TYPE lightType, const std::vector<CVGS_LIGHT_INFO> &vLightInfo);

	//  ����ģ�͹ؼ�֡����
	CNodeAnimation* CreateModelAnimation(CSceneNode *pNode, const std::vector<CNodeTransform> &vTransform);


private:

	//����node�ؼ�֡�����б�
	vector <CNodeAnimation*> m_vKeyfrmList;

	// ���ƺ�״̬ӳ���б������������ string : ����������, AnimStatus : ����״̬
	std::map<DWORD, AnimStatus> m_mAnimStatus;       // DWORD : �����ľ��, AnimStatus : ����״̬

	//���Ź�������
	void PlaySkeletonAnimation(double AddTime);

	//���Žڵ�ؼ�֡����
	void PlayNodeAnimation(double AddTime);

	void UpdateAnimScene();

	// ���ݶ������ƻ�ȡ������������ͣ� type �����ض���������
	DWORD GetAnimationHandleByName(const std::string sAnimName, VGS_ANIMATIONTYPE &type);


};