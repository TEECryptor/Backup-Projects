/////////////////////////////////////////////////////////////////////////
// �ļ���: CModel.h: interface for the CModel class.
// ��  ��: CModel
// ��  ��: ����Model��
// ��  ��: lixin
// ��  ��: 2006-04-30
/////////////////////////////////////////////////////////////////////////

#ifndef _CMODEL_H
#define _CMODEL_H

#include "VGSHeader.h"
#include ".\fileio\S3D_Def.h"
#include "CManyFVF.h"
#include "CSysFunc.h"

#include "CVertex.h"
#include "CFace.h"
#include "CRenderBoundBox.h"
#include "CMaterial.h"
//#include "CTransform.h"
#include "Node.h"

#include "MovableObject.h" 

#include "VGSDataStruct.h"
#include "Animation/Skeleton.h"

//Add by Baojiahui 2008/7/3---------------------------------------------
//���mesh���ͣ���ԭ�е�ģ�͵Ķ��������԰������SubMesh��Mesh��Ϣ����
typedef struct _subMesh
{
    LPDIRECT3DINDEXBUFFER9   m_IBuffer;		    // ��������
	VOID					 *m_pIndices;	    // ���������������ڴ�����
	D3DFORMAT				IBufferFormat;	    // ��������ĸ�ʽ
	UINT					faceCnt;            // �����
	CFace*					face;               // ���
	CMaterial*				oriShader;			// ԭʼ����,ÿ��submesh��������ӵ�ж����Ĳ���
	CMaterial*				shader;				// ��ǰʹ�õĲ���

	INT                     boneCount;          // Ӱ���submesh�Ĺ�ͷ��Ŀ
	INT                     boneIndex[MAX_BONE_COUNT];  // Ӱ���submesh�Ĺ�ͷ�ڹǼ��е�����

	_subMesh()
	{
		m_IBuffer  = 0;
		m_pIndices = 0;
		faceCnt    = 0;
		faceCnt    = 0;
		oriShader  = 0;
		shader     = 0;
		boneCount  = 0;
	}
}_subMesh;

// model��submodel
class VGS_EXPORT CSubModel
{
public:
	CMaterial*      m_pMaterial;
	CVGSBoundBox    m_boundBox;
	CVGSBoundSphere m_boundSphere;

	// �ڶ��������еĿ�ʼλ�ú���Ŀ
	UINT            m_vertexStart;
	UINT            m_vertexCount;

	// �������������еĿ�ʼλ�ú���Ŀ
	UINT            m_faceStart;
	UINT            m_faceCount;


public:
	CSubModel()
	{
		m_pMaterial   = 0;
		m_vertexStart = 0;
		m_vertexCount = 0;
		m_faceStart   = 0;
		m_faceCount   = 0;
	}
	~CSubModel()
	{
		Release();
	}
	void Release()
	{
		m_pMaterial = NULL;
	}


	/*CMaterial* getMaterial(){return m_pMaterial;}
	void setMaterial(CMaterial *pMat){m_pMaterial = pMat;}
	CVGSBoundBox getBoundBox();
	void setBoundBox(const CVGSBoundBox &bb);
	CVGSBoundSphere getBoundSphere();
	void setBoundSphere(const CVGSBoundSphere &bs);*/
};

//-----------------------------------------------------------------------
//Ŀǰʹ�ð�Χ����ӷ��㣬��Χ����ʱû���õ���lixin, 2006-05-12

//---------------------------------------------------------------------------------
//һ��ģ��Ӧ�ñ���Ⱦ����Щ���棿Ĭ������£�ģ��ֻ��Ҫ��Ⱦ������漴�ɡ�
//Ȼ�����ܶ�ʱ������ʹ��RenderToTexture������Ⱦ���䡢���䡢������Ч�ȡ�
//���ǿ���ʹ�ö�������Ⱦ��������Ⱦÿ����Ⱦ���棬��������ȱ��������Ƶ���л���Ⱦ״̬��
//������ȾЧ�ʵ��¡�����İ취����Ⱦÿ��ģ��ʱ��һ������Ⱦ��������档
//���������һ�����ڸ���ģ����Ⱦ�Ľṹ������ģ�͵���Ⱦ
//---------------------------------------------------------------------------------
enum RENDERTOTEXTURE_TYPE
{
	ADS_RTFMAP,          // ʵʱ������ͼ
	ADS_RTTMAP,			 // ʵʱ������ͼ
	ADS_BUMPWAVEMAP,	 // ��͹ˮ����ͼ
	ADS_CUBEMAP          // ʵʱcubemap
};

typedef struct ADDITIONALRENDERINFO
{
	RENDERTOTEXTURE_TYPE			type;
	DWORD							pRenderToTexture;
}ADDITIONALRENDERINFO;

class VGS_EXPORT CModel : public CMovableObject
{
private:
	//�ҽ�Model�ĳ���SceneNode�ڵ�
	//CSceneNode*				pNode;      
	//Model�����֣�ȡ��SceneNode��ͬ������
	//std::string				name;	

	//Model��D3DXMesh����		
	LPD3DXMESH				m_pMesh;

	//��Mesh�󶨵ĹǼܶ���
	CSkeleton*				m_pSkeleton;

	//���ڹ�����Ƥ��D3DXMESH����
	LPD3DXMESH				m_pSkinMesh;

	//���ڽ�����Ƥ�󶨺ͼ����ISkinInfo����
	LPD3DXSKININFO			m_pSkinInfo;

	//S3D �����Mesh���ݶ���
	CVGSMesh*				m_pVGSMesh;

	//����Mesh��option
	DWORD					m_optionXMesh;

	//Meshʹ�õĲ���ָ���б�ÿ��SubMeshʹ��һ������
	// vector<CMaterial*>		m_vpMtrl;

	vector<CSubModel>       m_vSubModel;  // ��ģ��

private:
	//��ʼ��
	HRESULT					Init();
	//д�붥�㻺������
	HRESULT					FillVertexBuffer();
	//д�붥�㻺������
	HRESULT FillVertexBuffer(void* pVertexSrcData);
	//д��������������
	HRESULT					FillIndexBuffer(vector<CMaterial*> vpShader);
	//���������ģ�͵���������
	HRESULT                 FillIndexBuffer(void* pIndexBuf);
	//д�����Ա�
	HRESULT					FillAttributeTable();
	//д�����Ա�
	HRESULT                 FillAttributeTable(vector<CSubModel> &vSubmodel);
	//���SkinInfo��Ƥ���󲢰󶨹�������
	HRESULT					CreateSkeletonAndSkin();
	//��������νṹ
	CBone*					CreateSkeletonHierarchy(VGS_Bone* pVGSBone, DWORD boneIndex);

public:
	//����CModel����
	HRESULT					Create(CVGSMesh* pVGSMesh, vector<CMaterial*> vpShader);

	// ����ģ��
	HRESULT		Create(DWORD fvf, DWORD vertexCount, DWORD faceCount, void* pVertextBuf, void *pIndexBuf, vector<CSubModel> &vSubModel);

	void                    generateSubmeshIndexData();
	LPD3DXMESH getD3DXmesh() {return m_pMesh;}
public:
	LPDIRECT3DDEVICE9		m_pd3dDevice;	//D3D Device����
	//D3D�������--------------------------------------------------------
	LPDIRECT3DVERTEXBUFFER9  m_VBuffer;		//���㻺��
    VOID*					 m_pVertices;	//���㻺���������ڴ�����
	DWORD					 m_FVF;			//�����ʽFVF
	UINT					 m_FVFSize;     //һ����������ռ�õĿռ��С
	UINT                     m_vertexCount;  // ������Ŀ
	UINT                     m_faceCount;    // ����
	
	HRESULT LockVertexBuffer(void **pVertexBuffer, DWORD op = D3DLOCK_READONLY);    // �������㻺��	
	HRESULT UnlockVertexBuffer();  // �������㻺��

	HRESULT LockIndexBuffer(void **pIndexBuffer, DWORD op = D3DLOCK_READONLY);
	HRESULT UnlockIndexBuffer();
	
	//HRESULT LockIndexBuffer(INT iSubMeshIdx);    // ������ģ�͵���������
	//HRESULT UnlockIndexBuffer(INT iSubMeshIdx);  // ������ģ�͵���������

	CSubModel* getSubModel(int submeshIndex);

public:
	INT					index;				//��Ⱦģ������Ⱦ�����еı��
	// std::string			name;
	
	VGS_BoundBox		oriBBox;			//û�б任���İ�Χ��
	VGS_BoundSphere			oriBSphere;			//û�б任���İ�Χ��

	VGS_BoundBox		boundingBox;		//ģ�͵İ�Χ��
	VGS_BoundSphere		boundingSphere;		//ģ�͵İ�Χ��
//	D3DXVECTOR3			centerInViewSpace;  //��View�ռ������λ��

	//UINT				faceCnt;			//�����
	//CFace*				face;				//���
	UINT				vertexCnt;			//�������
	CVertex*			vertex;				//�����
    
	//vector<_subMesh>	subMesh;    //subMesh��
	
	INT					opacity;			//ģ�͵�͸����
	INT					oriOpacity;			//ԭʼ͸����

	BOOL				mouseInOutable;		//�Ƿ���Ӧ��������������¼�

	BOOL				bMouseShader;		//�Ƿ���Ӧ����¼��仯ģ��ʹ�õĲ���

	//Edit by baojiahui Ŀǰ�İ汾��֧��������������滻���ʵĹ��ܣ�ֻ֧����ɫ�仯
	/*CMaterial*			mouseOverShader;	//�������ȥ��Ĳ���
	CMaterial*			mouseClickedShader; //���ѡ�к�Ĳ���
	CMaterial*			mouseUpShader;		//����ɿ���Ĳ���*/

	BOOL				bMouseColor;		//�Ƿ���Ӧ����¼��ı�ģ�͵Ķ�����ɫ
	D3DCOLOR			oriColor;			//ģ�͵Ķ���ɫ
	D3DCOLOR			mouseOverColor;		//�������ģ�ͺ�Ķ���ɫ
	D3DCOLOR			mouseClickedColor;	//���ѡ��ģ�ͺ�Ķ���ɫ
	D3DCOLOR			mouseUpColor;		//����ɿ���Ķ���ɫ;		//����ɿ���Ķ���ɫ

	BOOL                bModelColor;        //�Ƿ�ʹ��ģ��������ɫ��������ɫ
	D3DCOLOR            modelColor;         //ģ����ɫ
	
	BOOL				visible;			//�Ƿ�ɼ�
	UINT				culled;				//�Ƿ�����׶����
	BOOL				clickable;			//�Ƿ���Ե�ѡ
	BOOL				rayHitable;			//�Ƿ���Ա�������
	UINT				colStatus;			//0��ʾ�������ײ��1��ʾSphere��ײ��2��ʾBox��ײ��3��ʾMesh��ײ
	BOOL				isSkyBox;			//�Ƿ�����պ�
	BOOL				bSelected;			//�Ƿ��Ѿ���ѡ��
	
	UINT				currentTimer;		//��ǰʱ��
	UINT				kfrCnt;				//��ǰģ�͵Ĺؼ�֡����
	UINT				kfrFrameTime;		//�ؼ�֡��������һ֡��ʱ
	UINT				kfrCurFrame;		//��ǰ���ŵ�֡
	INT					startFrame;			//���Źؼ�֡�����Ŀ�ʼ֡
	INT					endFrame;			//���Źؼ�֡��������ֹ֡

	D3DXMATRIX			m_matIdentity;		//��λ����

	//����MoveModelTo, RosModelTo, ScaleModelTo֡�任����
	D3DXVECTOR3*		user_moveKFR;			//�û��Զ���ģ��֡�任����֡�任ʸ��
	INT					user_moveFrameCnt;		//�û��Զ���ģ��֡�任����֡��
	INT					user_moveCurFrame;		//��ǰ֡
	BOOL				bMatrixChanged;			//�Ƿ��Ѿ��任
    //ģ�͵Ĺؼ�֡�������˲���Ҫд��node�
	BOOL				kfrPlay;			//�趨�Ƿ񲥷Źؼ�֡����
	INT					kfrRepeatCnt;		//�ؼ�֡�������Ŵ�����-1��ʾ����ѭ��
	UINT				kfrRepeatIndex;		//��ǰ���ŵĴ���
	/*D3DXMATRIX			*matrix_kfr;		//�ؼ�֡������ǰģ�͵���֡�任����
	D3DXMATRIX			matrix_CurFrame;	//�ؼ�֡������ǰ֡�任����*/
/*
	UINT				vkfrCnt;			//�Ƿ�������㶯��
	BOOL				vkfrPlay;			//�趨�Ƿ񲥷Ŷ��㶯��
	INT					vkfrRepeatCnt;		//���㶯�����Ŵ�����-1��ʾ����ѭ��
	UINT				vkfrRepeatIndex;	//���㶯����ǰ���ŵĴ���	
	CVertex				**vertices_kfr;		//���㶯������֡��������*/
	

	BOOL				isBillboard;		// �Ƿ���Billboardģ�� 
	VGS_PointAtVectors		pointAtOrient;		// BillBoard�������� 
	D3DXVECTOR3         billboardDirectionAxis;      // ģ���������������������
	// fuxb 2008-09-09 ������������ ==>
	D3DXVECTOR3         billboradUpAxis;     // ģ���������ʱ��Up������
	bool                isAutoNormalise;     // ģ������ʱ���Ƿ�Ӧ���Զ����·��ߡ�������ֹ��ռ������ 
	// <==

	BOOL				isCollision;		//�Ƿ�����ײģ��
	FLOAT				colDetectDistance;	//��ײ���ľ��룬���پ������ھ���Ϊ��ײ������

	BOOL				isGround;			//�Ƿ��ǵ���ģ��

	BOOL				isZOne;				//�Ƿ���ZOneģ��

	BOOL                m_isClickable;      // �Ƿ�ɵ��

	BOOL				bDynamicUV;			//�Ƿ��Ƕ�̬UV
//	FLOAT				UStep;				//U�����ƶ�����
//	FLOAT				VStep;				//V�����ƶ�����
protected:
	/**����ϵ
	*/
	Axis*				m_pAxis;
	/**ƽ������ϵ
	*/
	AxisMove*			m_pAxisMove;
	/**��ת����ϵ
	*/
	AxisRotate*			m_pAxisRotate;
	/**��������ϵ
	*/
	AxisScale*			m_pAxisScale;
	/**�Ƿ���ʾ�����
	*/
	bool				m_bShowAxis;
	/** �Ƿ��Ѿ���Ⱦ������ϵ���ڲ����Ʊ���.
	*/
	bool				m_hHasRenderAxis;
	/**��ʾ��������ϵ��0-��ʾm_pAxis��1��m_pAxisMove��2��m_pAxisRotate��3��m_pAxisScale������ֵ����ʾ.
	*/
	int					m_nAxisIndex;
protected:
	/** ��������ϵ.
	*/
	void CreateAxis();
	/** ��������ϵ����.
	*/
	void DestroyAxis();
	/** ��������ϵ.
	*/
	void UpdateAxis();
public:
	Axis* GetAxis() { return m_pAxis; }
	AxisMove* GetAxisMove() { return m_pAxisMove;}
	AxisRotate* GetAxisRotate() { return m_pAxisRotate; }
	AxisScale* GetAxisScale() { return m_pAxisScale; }
	/** ��������ĵ�ǰ��ʾ״̬��ȡ����ϵ����.
	*/
	Axis* GetCurAxis();
	/** �����Ƿ���ʾ����ϵ.
		���ﶯ̬��������������ϵ����.
	*/
	void SetShowAxis(bool bShow);
	/** ����Ҫ��Ⱦ������ϵ״̬.
	*/
	void SetAxisStatus(int nStatus) { m_nAxisIndex = nStatus; }
	/** ��Ⱦ����ϵ.
	*/
	void RenderAxis(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);

public:
	CRenderBoundBox*    pRBBox;             //��Χ��ģ�Ͷ���
	BOOL                showRBBox;          //�Ƿ���ʾ��Χ��ģ��

	// CSceneNode*         pNode;             //��ǰ�ڵ�

    
	DWORD				ID1;				//ģ�͵�ID
	DWORD				ID2;				//ģ�͵���չID;

	//##ModelId=47EBC7480242
	INT					refModelIndex;		//����������
	//##ModelId=47EBC7480246
	CModel*				refModel;			//�������壬����ñ������Ǹ��Ƴ��������壬��refModelΪ��ԭʼ����

	BOOL				bUseFBL;			//�Ƿ�ʹ�÷ֱ��ʹ���

	DWORD				modelGroup;			//�����ĸ�modelGroup
	BOOL				bmouseHintByGroup;	//�Ƿ�ʹ��[ģ����]������������������ѡ������

	//�Ƿ���ʾ�����־
    BOOL                bReflect;
    //������Ҫ�������Ϣ���������û�з��� ��Ϊ-1
	int                 sceneRefInfoListIdx;
    
	vector<UINT>        refedModelIdxList;           //�����巴��������б�����

	CModel*				pCloneModel;				//��¡ģ�͵�ԭģ��

	vector<CCubeMap*>	useMeCubeMap;			//CubeMap�б����CubeMap��Ҫ�����ģ�ͣ���useMeCubeMap������CubeMap
	vector<CBumpMap*>   m_useMeBumpmap;

	
	//------------------------------------------------------------------------
	//��Ⱦ��ģ����Ҫ�ĸ�����Ϣ�����ڱ�ģ��һ����Ⱦ���������
	//------------------------------------------------------------------------
	vector<ADDITIONALRENDERINFO>	m_additionalRenderInfo;

	// std::string GetName();
	// VOID SetName(const std::string &sName);

	UINT getSubModelCount(){return m_vSubModel.size();}
	CMaterial* getSubModelMaterial(INT subMeshIndex);
	VOID setSubModelMaterial(INT subMeshIndex, CMaterial *pMaterial);

	// ����Ƴ�cubemap����������cubemap����ôcubemap�ͻᷴ���ģ��
	VOID addCubemap(CCubeMap *pCubemap);
	VOID removeCubemap(CCubeMap *pCubemap);
	VOID addBumpmap(CBumpMap* pBumpmap);
	VOID removeBumpmap(CBumpMap* pBumpmap);

public:
	//�ƶ���ͼ����UV
	VOID	MoveVertexUV(UINT uvIdx, FLOAT x, FLOAT y);
	//����͸����
	VOID	SetOpacity(UINT opacity);
	//���ò�����
	VOID	SetMtrl(CMaterial* pMtrl, int subMeshIdx);
	CMaterial *GetMaterial(int subMeshIdx);
	//������ɫ
	VOID	SetColor(INT red, INT green, INT blue, INT alpha);
	//Add by baojiahui                              2009/1/15---------------------------------
	//���ö�����ɫ
	VOID    SetVertexColor(INT red, INT green, INT blue, INT alpha);
    //----------------------------------------------------------------------------------------
	BOOL    IsTransparent();               // ģ�ͼ���ʹ�õĲ����Ƿ�͸��	
	BOOL    IsUseTransparentMaterial();    // ģ���Ƿ�ʹ��͸������

	BOOL    IsInsideSphere(D3DXVECTOR3* pPos);   // ���Ƿ���ģ�Ͱ�Χ���ڲ�
	BOOL    IsInsideBox(D3DXVECTOR3* pPos);      // ���Ƿ���ģ�Ͱ�Χ���ڲ�
	
	// �㵽ģ�Ͱ�Χ�����ж�����������ƽ���������ֵ���ģ���ڲ������ⲿ�����
	FLOAT getNearDistanceSq(D3DXVECTOR3* pPos);

//�任���
public:
	//����������ϵ����������ϵƽ��
	void	translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);  //coordEnum = 0 = worldcoord, 1 = selfcoor;
	//ͨ������ģ�͵�����������ģ�͵�λ�ã���������ϵ, Pivotλ��Ϊģ�͵�λ��
	void	setPosition(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//����������ϵ����������ϵ��ת
	void	rotate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//�ƴ����̶����ָ������תһ���ĽǶ�
	void	rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, float fAngle, VGS_TRANSFORMSPACE coordEnum);
	//ֱ��������ת�ĽǶȣ�x,y,z����
	void	setRotationByAngle(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//�������������Ļ������������ķŴ�
	void	scale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//�Թ̶���Ϊ���ķŴ�
	void	scaleByPoint(D3DXVECTOR3* pVec, D3DXVECTOR3* pPos, VGS_TRANSFORMSPACE coordEnum);
	//ֱ����������
	void	setScale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//����
	void    UpdateFromNode();
	//������Ⱦ���ģ�͵ķ���
	void	render();

	//��Ⱦģ�͵�SubMesh
	VOID	RenderSubMesh(INT index);


public:
	//������ǰģ�͵Ķ��㻺��
	HRESULT createVBuffer();
	//������ǰģ�͵���������
	HRESULT createIBuffer();
	//��¡ģ�͵�����
	CModel*	clone(std::string newModelName);
	//�ͷ�
	VOID	Release();

public:
	CModel(LPDIRECT3DDEVICE9 pd3dDevice);
	~CModel();

private:


public:
	
	vector<D3DXMATRIX> m_BoneOffsetMatrix;      // Ӱ��model��ÿ���ͷ��Ӧ��offset matrix
	INT                m_SkeletonIndex;         // ģ��ʹ�õĹǼ�����

};

#endif 
