/////////////////////////////////////////////////////////////////////////
// ��  ��: CCamera
// ��  ��: ��������Ͷ��壬�����Target Camera��Direct Camera�����ƽ��������
//		   ����From��at���ԣ�������ת�ķ��������һ�����£�ʹ�ø��ӷ��㡣
//		   ����camera�任�ķ�������ͨ���ı�form, at, roll��ֵ��ʵ�ֵ�
// ��  ��: lixin
// ��  ��: 2005-12-21
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

// �̳��Ը���Ľӿ�
public:
	// ���ڵ�任�������˱仯�ǣ���������ĸ���
	void UpdateFromNode();

public:	
	INT					index;

	VGS_CAMERA_TYPE     m_camType;         // ����������͸�����


	float				m_aspect;					//���ͶӰ����ĳ����
	D3DXMATRIX			m_view, m_proj, m_viewproj;  //�����view����view����ͶӰ����view�����ͶӰ����ĳ˻�
	D3DXMATRIX			m_invview, m_invproj, m_invviewproj; //������������������

	float      m_tanHalfFieldOfViewV;

//������任����
public:
	// ƽ�������, �����from��atͬʱ�ƶ�
	// VOID	Translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	void Move(const D3DXVECTOR3 &deltPos, VGS_TRANSFORMSPACE coordEnum);

	// ��direction�������������From�㣬At�㲻��
	VOID    Dolly(FLOAT fDis);

	// ��ָ��������ϵ�ڣ��ֱ�����������ת�����
	VOID Rotate(D3DXVECTOR3* pVec, VGS_ROTATIONSPACE coordEnum);

	// ��ָ��������ϵ�������������ת��
	// VOID SetRotation(D3DXVECTOR3* pRos, VGS_ROTATIONSPACE coordEnum);

	// �������������
	VOID	SetLookAtModel(CModel* pModel);
	CModel*	GetLookAtModel(){return m_pLookatModel;}

	// ɾ�����������
	VOID	DeleteLookAtModel();

	// �����at��Ϊ���ģ���pAxisVecΪ�ᣬ��תfAngle����
	VOID	Round(D3DXVECTOR3* pAxisVec, FLOAT fAngle); 

	// ��Ŀ���Ϊ���ģ���pCenterΪ���ĵ㣬pAxisVecΪ������תfAngle����
	VOID	RoundEx(D3DXVECTOR3* pCenter, D3DXVECTOR3* pAxisVec, FLOAT fAngle);

	// ��Ŀ���Ϊ���ģ����������XYZ��Ϊ��ת�ᣬ��תfAngle����
	VOID	RoundInCameraSpace(FLOAT fAngleX, FLOAT fAngleY, FLOAT fAngleZ);

	// ����Ϊ���ģ����������XYZ��Ϊ��ת�ᣬ��תfAngle����
	VOID	RoundInCameraSpaceEx(D3DXVECTOR3* pCenter, FLOAT fAngleX, FLOAT fAngleY, FLOAT fAngleZ);

	// ���������λ�ã����������ᱣ�ֲ���
	VOID	      SetPosition(D3DXVECTOR3* pFrom, VGS_TRANSFORMSPACE coordEnum);
	D3DXVECTOR3	  GetPosition(VGS_TRANSFORMSPACE coordEnum);

	// ͨ�����������Ŀ������ı������lookat�����Լ�����������
	VOID	      Lookat(const D3DXVECTOR3* pAt, VGS_TRANSFORMSPACE coordEnum);
	D3DXVECTOR3	  GetLookat(VGS_TRANSFORMSPACE coordEnum);

	// ����ת��
	VOID		  setRotation(const D3DXQUATERNION &quat, VGS_TRANSFORMSPACE coordEnum);
	
	// ���������Up(����Y��)����Ҳ����Ӱ�������X��
	VOID          SetUp(const D3DXVECTOR3 &up, VGS_TRANSFORMSPACE coordEnum);
	D3DXVECTOR3	  GetUp(VGS_TRANSFORMSPACE coordEnum);

	// ����Roll
	VOID	SetRoll(FLOAT roll);
	// FLOAT   GetRoll() {return m_fRoll;}

	D3DXVECTOR3   GetRight(VGS_TRANSFORMSPACE coordEnum);       // ��ȡ����x�᷽��
	D3DXVECTOR3   GetDirection(VGS_TRANSFORMSPACE coordEnum);   // ��ȡ�����lookat��������Z����), �������������ϵ	

	// ����FOV, �û��ȱ�ʾ
	VOID	SetFov(FLOAT fov);
	FLOAT	GetFov() {return m_fFov;};
	VOID    setNearClip(float nearClip){m_nearClip = nearClip;}
	float   getNearClip(){return m_nearClip;}
	VOID    setFarClip(float farClip){m_farClip = farClip;}
	float   getFarClip(){return m_farClip;}

	// ���/ɾ��һ���ӿ�
	VOID addViewport(DWORD pViewport);
	VOID removeViewport(DWORD pViewport);
	VOID getViewport(vector<DWORD> &vViewport);


	VOID getViewMatrix(D3DXMATRIX *mat);
	VOID getProjectMatrix(D3DXMATRIX *mat);
	VOID UpdateAddtionalMatrix();

	// ��ȡ�ڵ����������任����
	D3DXMATRIX GetNodeWordMatrix();

	// ����original������restoreʱ�õ�
	void setOriginPosition(const D3DXVECTOR3 &v3Pos){m_OriginWorldPosition = v3Pos;}
	void setOriginLookatPos(const D3DXVECTOR3 &v3posAt){m_OriginLookatPoint = v3posAt;}
	void setOriginRoll(const float roll){m_OriginRoll = roll;}

	//  �������Ϊoriginal״̬
	void restore();


private:
	// ͨ��Rollֵ���upʸ��
	//VOID	GetUpByRoll();	
	// ͨ��roll��ȡup��rightֵ,looat���ֲ���
	VOID UpdateFromRoll(const float& fRoll);

	//ͨ�������from, at, roll ���������������ϵ����ʼ��ʱʹ��
	//VOID	InitMatrix();

	//��ʼ������
	VOID	InitVars();

	//��ʼ���������
	VOID	Reset();
	
	// UpdateMatrix�任ʱʹ��
	//VOID	GetAxisVector();
	//VOID	UpdateTranslateMatrix();
	//VOID	MultiplyMatrix();

	// VOID Update();

	// ���±��ؾ���
	VOID UpdateLocalMatrix();

	// �������������µı仯����
	VOID UpdateWorldMatrix();

public:
	CCamera();
	~CCamera();

	//�ͷ���Դ
	VOID	Release();

	//CCamera*	clone(std::string newCamName);	

	CScene* GetScene();                // �����������ĳ���	
	VOID    SetScene(CScene *pScene);  // ������������ĳ���	


	// �Ӹ��ڵ�ı仯������±�����ı仯����
	void UpdateMatrixFromParent();

private:
	
	CScene*  m_pScene;           // ��������ĳ���
	// CSceneNode*			m_pNode;

	vector <DWORD> m_vViewport;  // ʹ�ø�������ӿ�
	

	// D3DXVECTOR3 m_from, m_at, m_up;
	D3DXVECTOR3 m_localPosition, m_localLookat, m_localY, m_localX;   // ���ڵ��µĲ���
	D3DXVECTOR3 m_realPosition, m_realLookat, m_realY, m_realX;       // ��������ϵ�µĲ���

	float   m_fRoll;  // ��Direction������z�ᣩ�����ת�Ƕȣ���λ�ǻ���
	float   m_fFov;
	float   m_nearClip;
	float   m_farClip;

	D3DXVECTOR3             m_OriginLookatPoint;
	D3DXVECTOR3             m_OriginWorldPosition;
	float                   m_OriginRoll;

	D3DXMATRIX  m_localTransform;  // ����ڸ��ڵ�ı任���� 
	D3DXMATRIX  m_worldTransform;  // ����������ϵ�еı任����
	D3DXVECTOR3 m_originWorldTransform;  // �������������ϵ�еı任����restoreʱ�õ�

	CModel*		m_pLookatModel;				//lookAtģ�͵�ָ��

	// �Ӹ��ڵ�ı仯������±�����ı仯����
	// ���¾���
	// void UpdateMatrix();

};

#endif
