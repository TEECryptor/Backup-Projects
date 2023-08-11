// Node.h: interface for the SceneNode class.
// ��������Ĺ������ͣ����ڸ�������Ĺ����任
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__CD931C06_760E_43C0_81D1_3E68EB898063__INCLUDED_)
#define AFX_NODE_H__CD931C06_760E_43C0_81D1_3E68EB898063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "CSysFunc.h"
#include "CTransform.h"
// #include ".\fileio\N3D_Def.h"
#include "MovableObject.h"
#include "vgsDef.h"

#include "VGSGetRoot.h"

#include <vector>
using namespace std;
// using namespace N3D;


class VGS_EXPORT CSceneNode  : public CVGSGetRoot
{
public:
	CSceneNode();
	virtual ~CSceneNode();

	//�ͷ�
	void	Release();

public:
	std::string					name;			//�ڵ�����
	
	//�ڵ㸸�ӹ�ϵ
	CSceneNode*						m_parent;
	vector<CSceneNode*>				m_child;

	//�任����-------------------------------------------------------------------------
private:
	//��¼�ؼ�֡�任��Ϣ
	//CTransform                  curTransform;
	//UINT                        currentFrame;       
	//UINT                        keyFrameCnt;  

	D3DXVECTOR3					m_WorldPosition;		//�ڵ�����������ϵ�е�λ��
	D3DXQUATERNION				m_WorldRotation;		//�ڵ�����������ϵ�е�ת��
	D3DXVECTOR3					m_WorldScale;			//�ڵ�����������ϵ�е�����
	D3DXMATRIX                  m_WorldMatrix;
	D3DXMATRIX                  m_WorldMatrixInverse;   // ����任����������

	D3DXVECTOR3					m_Position;		//�ڵ��ڸ�������ϵ�е�λ��
	D3DXQUATERNION				m_Rotation;		//�ڵ��ڸ�������ϵ�е�ת��
	D3DXVECTOR3					m_Scale;		//�ڵ��ڸ�������ϵ�е�����
	D3DXMATRIX                  m_Matrix;

	// bool                        m_bNeedUpdate;   // �Ƿ���Ҫ����
    
	//����任�ķ���-------------------------------------------------------------------
public:
	//����������ϵ����������ϵƽ��
	void	translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);  //coordEnum = 0 = worldcoord, 1 = selfcoor; 2: parent
	//ͨ������ģ�͵�����������ģ�͵�λ�ã���������ϵ, Pivotλ��Ϊģ�͵�λ�� coordEnum : 0 = worldcoord, 1 = selfcoor; 2 = parent;
	void	setPosition(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);

	// ��ȡ��ǰ������
	D3DXVECTOR3 getPosition(VGS_TRANSFORMSPACE coordEnum);

	//�����Ǹ�������������������Ԫ����ת
	void	rotate(D3DXQUATERNION* pQuat, VGS_TRANSFORMSPACE coordEnum);
	//����������ϵ����������ϵ��ת
	void	rotate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//�ƴ����̶����ָ������תһ���ĽǶ�
	void	rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, float fAngle, VGS_TRANSFORMSPACE coordEnum);
	//ֱ��������ת�ĽǶȣ�x,y,z����
	void	setRotationByAngle(const D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//ֱ��������ת�ĽǶȣ���Ԫ��
	void	setRotationByQuat(const D3DXQUATERNION* pQuat, VGS_TRANSFORMSPACE coordEnum);

	// ��õ�ǰ����ת��Ԫ��
	D3DXQUATERNION getRotition(VGS_TRANSFORMSPACE coordEnum);

	//�������������Ļ������������ķŴ�
	void	scale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	// �Թ̶���Ϊ���ķŴ�
	void	scaleByPoint(D3DXVECTOR3* pVec, D3DXVECTOR3* pPos, VGS_TRANSFORMSPACE coordEnum);
	//ֱ���������ţ�0 : ��������ϵ�� 1����������ϵ��2:������ϵ
	void	setScale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);

	// ��õ�ǰ������
	D3DXVECTOR3 getScale(VGS_TRANSFORMSPACE coordEnum);

	// ��ñ任����
	D3DXMATRIX getMatrix(VGS_TRANSFORMSPACE coordEnum);

	// ���ñ任����
	void setMatrix(const D3DXMATRIX& mat, VGS_TRANSFORMSPACE coord);

	const D3DXMATRIX& getWorldInverseMatrix() const { return m_WorldMatrixInverse; }
public:
	// �ڵ����
	//VGS_MOVABLETYPE getNodeType(){return m_Type;}
	//void setNodeType(VGS_MOVABLETYPE type){m_Type = type;}
	VGS_MOVABLETYPE getMovableType();
	CSceneNode* CreateChild();
	CSceneNode* GetChildByIndex(INT index);
	UINT        GetChildCount();

	// �ڵ��¹���һ������
	VOID attachObject(/*VGS_MOVABLETYPE type, */CMovableObject* objectHandel);
	CMovableObject* getMovableObject(){return m_object;}

	// �ж�һ���ڵ��Ƿ������ţ��������ڵ������
	BOOL getDerivedScaled();

		//update the Inherit transform from the Parent
	// void    updateFromParent();
	//update the transform Matrix
	void UpdateChildTransform();
	void UpdateWorldMatrix();  // ������������ϵ�µı任����
	void UpdateLocalMatrix();  // ��������任����

	// ���ýڵ��z��ָ��v3Lookat
	// v3Lookat : z��ָ��ĵ㣻coordEnum : �ο�����ϵ��yawFixed : �Ƿ�����ʼyaw��ת���᣻yawFixedAxis ���������yaw�ᣬ��ָ����
	void    lookAt(const D3DXVECTOR3& v3Lookat, const VGS_TRANSFORMSPACE& coordEnum, const bool& bYawFixed = true, const D3DXVECTOR3& v3YawFixedAxis = D3DXVECTOR3(0, 1, 0));

protected:
	bool m_bLocalMatrixChanged;          // ������ϵ�µ�matrix�����˱任
	// void notifyTransformChanged();  // ֪ͨ�ӽڵ�͹ҵĶ��󣬽ڵ�ı仯�������˱仯

	//----------------------------------------------------------------------------------
private:

	//�ҽӵĶ�������
	// VGS_MOVABLETYPE 			m_Type;
	//����
	/*DWORD*/CMovableObject*	   m_object;

	//���಻ʵ�ֻ�ʵ���������鷳���ɼ̳��������Ҫ���Ĺ���, Ʃ�����ģ�͵İ�Χ��
	// virtual void update();

	//update translate matrix
	void	updateTranslateMatrix();
	//update rotate matrix
	void	updateRotateMatrix();
	//update scale matrix
	void	updateScaleMatrix();
	//update matrix
	void	multiplyMatrix();

	void    GetQuatRotation(D3DXQUATERNION* rotQuat, const D3DXVECTOR3& curDir, const D3DXVECTOR3& desDir, const D3DXVECTOR3& fallbackAxis =D3DXVECTOR3(0, 0, 0));
	//ͨ��3�������������Ԫ��
	void    GetQuatBy3Axis(D3DXQUATERNION* rotQuat, const D3DXVECTOR3& xVec, const D3DXVECTOR3& yVec, const D3DXVECTOR3& zVec);


};

#endif // !defined(AFX_NODE_H__CD931C06_760E_43C0_81D1_3E68EB898063__INCLUDED_)
