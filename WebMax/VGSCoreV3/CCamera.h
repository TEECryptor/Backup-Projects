/////////////////////////////////////////////////////////////////////////
// 名  称: CCamera
// 描  述: 摄像机类型定义，该类把Target Camera和Direct Camera的优势结合起来了
//		   既有From和at属性，又有旋转的方法，而且互相更新，使用更加方便。
//		   所有camera变换的方法都是通过改变form, at, roll的值来实现的
// 作  者: lixin
// 日  期: 2005-12-21
/////////////////////////////////////////////////////////////////////////
#ifndef _CCAMERA_H
#define _CCAMERA_H

#include "VGSHeader.h"
#include ".\fileio\S3D_Def.H"
#include "CSysFunc.h"
#include "CTransform.h"
#include "CModel.h"
#include "CScene.h"

#include "MovableObject.h"

#include <vector>

class VGS_EXPORT CCamera : public CMovableObject
{

// 继承自父类的接口
public:
	// 当节点变换矩阵发生了变化是，用于自身的更新
	void UpdateFromNode();

public:	
	INT					index;

	VGS_CAMERA_TYPE     m_camType;         // 轴侧相机还是透视相机


	float				m_aspect;					//相机投影成像的长宽比
	D3DXMATRIX			m_view, m_proj, m_viewproj;  //相机的view矩阵，view矩阵，投影矩阵，view矩阵和投影矩阵的乘积
	D3DXMATRIX			m_invview, m_invproj, m_invviewproj; //上述三个矩阵的逆矩阵

	float      m_tanHalfFieldOfViewV;

//摄像机变换方法
public:
	// 平移摄像机, 相机的from和at同时移动
	// VOID	Translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	void Move(const D3DXVECTOR3 &deltPos, VGS_TRANSFORMSPACE coordEnum);

	// 沿direction方向推拉相机的From点，At点不动
	VOID    Dolly(FLOAT fDis);

	// 在指定的坐标系内，分别绕三个轴旋转摄像机
	VOID Rotate(D3DXVECTOR3* pVec, VGS_ROTATIONSPACE coordEnum);

	// 在指定的坐标系内设置摄像机的转角
	// VOID SetRotation(D3DXVECTOR3* pRos, VGS_ROTATIONSPACE coordEnum);

	// 建立摄像机正对
	VOID	SetLookAtModel(CModel* pModel);
	CModel*	GetLookAtModel(){return m_pLookatModel;}

	// 删除摄像机正对
	VOID	DeleteLookAtModel();

	// 以相机at点为中心，以pAxisVec为轴，旋转fAngle弧度
	VOID	Round(D3DXVECTOR3* pAxisVec, FLOAT fAngle); 

	// 以目标点为中心，以pCenter为轴心点，pAxisVec为轴向，旋转fAngle弧度
	VOID	RoundEx(D3DXVECTOR3* pCenter, D3DXVECTOR3* pAxisVec, FLOAT fAngle);

	// 以目标点为中心，以相机自身XYZ轴为旋转轴，旋转fAngle弧度
	VOID	RoundInCameraSpace(FLOAT fAngleX, FLOAT fAngleY, FLOAT fAngleZ);

	// 定点为中心，以相机自身XYZ轴为旋转轴，旋转fAngle弧度
	VOID	RoundInCameraSpaceEx(D3DXVECTOR3* pCenter, FLOAT fAngleX, FLOAT fAngleY, FLOAT fAngleZ);

	// 设置相机的位置，自身坐标轴保持不变
	VOID	      SetPosition(D3DXVECTOR3* pFrom, VGS_TRANSFORMSPACE coordEnum);
	D3DXVECTOR3	  GetPosition(VGS_TRANSFORMSPACE coordEnum);

	// 通过设置相机的目标点来改变相机的lookat方向以及自身坐标轴
	VOID	      Lookat(const D3DXVECTOR3* pAt, VGS_TRANSFORMSPACE coordEnum);
	D3DXVECTOR3	  GetLookat(VGS_TRANSFORMSPACE coordEnum);

	// 设置转角
	VOID		  setRotation(const D3DXQUATERNION &quat, VGS_TRANSFORMSPACE coordEnum);
	
	// 设置相机的Up(自身Y轴)方向，也将会影响自身的X轴
	VOID          SetUp(const D3DXVECTOR3 &up, VGS_TRANSFORMSPACE coordEnum);
	D3DXVECTOR3	  GetUp(VGS_TRANSFORMSPACE coordEnum);

	// 设置Roll
	VOID	SetRoll(FLOAT roll);
	// FLOAT   GetRoll() {return m_fRoll;}

	D3DXVECTOR3   GetRight(VGS_TRANSFORMSPACE coordEnum);       // 获取自身x轴方向
	D3DXVECTOR3   GetDirection(VGS_TRANSFORMSPACE coordEnum);   // 获取相机的lookat方向（自身Z方向), 相对于世界坐标系	

	// 设置FOV, 用弧度表示
	VOID	SetFov(FLOAT fov);
	FLOAT	GetFov() {return m_fFov;};
	VOID    setNearClip(float nearClip){m_nearClip = nearClip;}
	float   getNearClip(){return m_nearClip;}
	VOID    setFarClip(float farClip){m_farClip = farClip;}
	float   getFarClip(){return m_farClip;}

	// 添加/删除一个视口
	VOID addViewport(DWORD pViewport);
	VOID removeViewport(DWORD pViewport);
	VOID getViewport(vector<DWORD> &vViewport);


	VOID getViewMatrix(D3DXMATRIX *mat);
	VOID getProjectMatrix(D3DXMATRIX *mat);
	VOID UpdateAddtionalMatrix();

	// 获取节点的世界坐标变换矩阵
	D3DXMATRIX GetNodeWordMatrix();

	// 设置original参数，restore时用到
	void setOriginPosition(const D3DXVECTOR3 &v3Pos){m_OriginWorldPosition = v3Pos;}
	void setOriginLookatPos(const D3DXVECTOR3 &v3posAt){m_OriginLookatPoint = v3posAt;}
	void setOriginRoll(const float roll){m_OriginRoll = roll;}

	//  重置相机为original状态
	void restore();


private:
	// 通过Roll值获得up矢量
	//VOID	GetUpByRoll();	
	// 通过roll获取up和right值,looat保持不变
	VOID UpdateFromRoll(const float& fRoll);

	//通过相机的from, at, roll 计算相机自身坐标系，初始化时使用
	//VOID	InitMatrix();

	//初始化变量
	VOID	InitVars();

	//初始化像机参数
	VOID	Reset();
	
	// UpdateMatrix变换时使用
	//VOID	GetAxisVector();
	//VOID	UpdateTranslateMatrix();
	//VOID	MultiplyMatrix();

	// VOID Update();

	// 更新本地矩阵
	VOID UpdateLocalMatrix();

	// 更新世界坐标下的变化矩阵
	VOID UpdateWorldMatrix();

public:
	CCamera();
	~CCamera();

	//释放资源
	VOID	Release();

	//CCamera*	clone(std::string newCamName);	

	CScene* GetScene();                // 获得相机所属的场景	
	VOID    SetScene(CScene *pScene);  // 设置相机所属的场景	


	// 从父节点的变化矩阵更新本相机的变化矩阵
	void UpdateMatrixFromParent();

private:
	
	CScene*  m_pScene;           // 相机所属的场景
	// CSceneNode*			m_pNode;

	vector <DWORD> m_vViewport;  // 使用该相机的视口
	

	// D3DXVECTOR3 m_from, m_at, m_up;
	D3DXVECTOR3 m_localPosition, m_localLookat, m_localY, m_localX;   // 父节点下的参数
	D3DXVECTOR3 m_realPosition, m_realLookat, m_realY, m_realX;       // 世界坐标系下的参数

	float   m_fRoll;  // 沿Direction（自身z轴）轴的旋转角度，单位是弧度
	float   m_fFov;
	float   m_nearClip;
	float   m_farClip;

	D3DXVECTOR3             m_OriginLookatPoint;
	D3DXVECTOR3             m_OriginWorldPosition;
	float                   m_OriginRoll;

	D3DXMATRIX  m_localTransform;  // 相对于父节点的变换矩阵 
	D3DXMATRIX  m_worldTransform;  // 在世界坐标系中的变换矩阵
	D3DXVECTOR3 m_originWorldTransform;  // 最初在世界坐标系中的变换矩阵，restore时用到

	CModel*		m_pLookatModel;				//lookAt模型的指针

	// 从父节点的变化矩阵更新本相机的变化矩阵
	// 更新矩阵
	// void UpdateMatrix();

};

#endif
