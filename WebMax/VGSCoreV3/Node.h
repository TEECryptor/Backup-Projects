// Node.h: interface for the SceneNode class.
// 场景对象的广义类型，用于父子物体的关联变换
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

	//释放
	void	Release();

public:
	std::string					name;			//节点名称
	
	//节点父子关系
	CSceneNode*						m_parent;
	vector<CSceneNode*>				m_child;

	//变换参数-------------------------------------------------------------------------
private:
	//记录关键帧变换信息
	//CTransform                  curTransform;
	//UINT                        currentFrame;       
	//UINT                        keyFrameCnt;  

	D3DXVECTOR3					m_WorldPosition;		//节点在世界坐标系中的位置
	D3DXQUATERNION				m_WorldRotation;		//节点在世界坐标系中的转角
	D3DXVECTOR3					m_WorldScale;			//节点在世界坐标系中的缩放
	D3DXMATRIX                  m_WorldMatrix;
	D3DXMATRIX                  m_WorldMatrixInverse;   // 世界变换矩阵的逆矩阵

	D3DXVECTOR3					m_Position;		//节点在父极坐标系中的位置
	D3DXQUATERNION				m_Rotation;		//节点在父极坐标系中的转角
	D3DXVECTOR3					m_Scale;		//节点在父极坐标系中的缩放
	D3DXMATRIX                  m_Matrix;

	// bool                        m_bNeedUpdate;   // 是否需要更新
    
	//坐标变换的方法-------------------------------------------------------------------
public:
	//绕自身坐标系或世界坐标系平移
	void	translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);  //coordEnum = 0 = worldcoord, 1 = selfcoor; 2: parent
	//通过设置模型的坐标来设置模型的位置，世界坐标系, Pivot位置为模型的位置 coordEnum : 0 = worldcoord, 1 = selfcoor; 2 = parent;
	void	setPosition(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);

	// 获取当前的坐标
	D3DXVECTOR3 getPosition(VGS_TRANSFORMSPACE coordEnum);

	//绕有那个湖定义的坐标轴进行四元数旋转
	void	rotate(D3DXQUATERNION* pQuat, VGS_TRANSFORMSPACE coordEnum);
	//绕自身坐标系或世界坐标系旋转
	void	rotate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//绕穿过固定点的指定轴旋转一定的角度
	void	rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, float fAngle, VGS_TRANSFORMSPACE coordEnum);
	//直接设置旋转的角度，x,y,z三轴
	void	setRotationByAngle(const D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//直接设置旋转的角度，四元数
	void	setRotationByQuat(const D3DXQUATERNION* pQuat, VGS_TRANSFORMSPACE coordEnum);

	// 获得当前的旋转四元数
	D3DXQUATERNION getRotition(VGS_TRANSFORMSPACE coordEnum);

	//以自身坐标中心或世界坐标中心放大
	void	scale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	// 以固定点为中心放大
	void	scaleByPoint(D3DXVECTOR3* pVec, D3DXVECTOR3* pPos, VGS_TRANSFORMSPACE coordEnum);
	//直接设置缩放，0 : 世界坐标系， 1：自身坐标系，2:父坐标系
	void	setScale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);

	// 获得当前的缩放
	D3DXVECTOR3 getScale(VGS_TRANSFORMSPACE coordEnum);

	// 获得变换矩阵
	D3DXMATRIX getMatrix(VGS_TRANSFORMSPACE coordEnum);

	// 设置变换矩阵
	void setMatrix(const D3DXMATRIX& mat, VGS_TRANSFORMSPACE coord);

	const D3DXMATRIX& getWorldInverseMatrix() const { return m_WorldMatrixInverse; }
public:
	// 节点相关
	//VGS_MOVABLETYPE getNodeType(){return m_Type;}
	//void setNodeType(VGS_MOVABLETYPE type){m_Type = type;}
	VGS_MOVABLETYPE getMovableType();
	CSceneNode* CreateChild();
	CSceneNode* GetChildByIndex(INT index);
	UINT        GetChildCount();

	// 节点下关联一个物体
	VOID attachObject(/*VGS_MOVABLETYPE type, */CMovableObject* objectHandel);
	CMovableObject* getMovableObject(){return m_object;}

	// 判断一个节点是否有缩放，包括父节点的缩放
	BOOL getDerivedScaled();

		//update the Inherit transform from the Parent
	// void    updateFromParent();
	//update the transform Matrix
	void UpdateChildTransform();
	void UpdateWorldMatrix();  // 更新世界坐标系下的变换矩阵
	void UpdateLocalMatrix();  // 更新自身变换矩阵

	// 设置节点的z轴指向v3Lookat
	// v3Lookat : z轴指向的点；coordEnum : 参考坐标系；yawFixed : 是否锁定始yaw旋转的轴；yawFixedAxis ：如果锁定yaw轴，则指定轴
	void    lookAt(const D3DXVECTOR3& v3Lookat, const VGS_TRANSFORMSPACE& coordEnum, const bool& bYawFixed = true, const D3DXVECTOR3& v3YawFixedAxis = D3DXVECTOR3(0, 1, 0));

protected:
	bool m_bLocalMatrixChanged;          // 父坐标系下的matrix发生了变换
	// void notifyTransformChanged();  // 通知子节点和挂的对象，节点的变化矩阵发生了变化

	//----------------------------------------------------------------------------------
private:

	//挂接的对象类型
	// VGS_MOVABLETYPE 			m_Type;
	//对象
	/*DWORD*/CMovableObject*	   m_object;

	//本类不实现或实现起来很麻烦，由继承类具体需要做的工作, 譬如更新模型的包围盒
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
	//通过3个轴向量获得四元数
	void    GetQuatBy3Axis(D3DXQUATERNION* rotQuat, const D3DXVECTOR3& xVec, const D3DXVECTOR3& yVec, const D3DXVECTOR3& zVec);


};

#endif // !defined(AFX_NODE_H__CD931C06_760E_43C0_81D1_3E68EB898063__INCLUDED_)
