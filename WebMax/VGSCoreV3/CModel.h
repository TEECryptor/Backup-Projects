/////////////////////////////////////////////////////////////////////////
// 文件名: CModel.h: interface for the CModel class.
// 类  名: CModel
// 描  述: 基本Model类
// 作  者: lixin
// 日  期: 2006-04-30
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
//添加mesh类型，将原有的模型的定点数据以包含多个SubMesh的Mesh信息保存
typedef struct _subMesh
{
    LPDIRECT3DINDEXBUFFER9   m_IBuffer;		    // 索引缓冲
	VOID					 *m_pIndices;	    // 索引缓冲锁定的内存区域
	D3DFORMAT				IBufferFormat;	    // 索引缓冲的格式
	UINT					faceCnt;            // 面个数
	CFace*					face;               // 面表
	CMaterial*				oriShader;			// 原始材质,每个submesh都将可以拥有独立的材质
	CMaterial*				shader;				// 当前使用的材质

	INT                     boneCount;          // 影响该submesh的骨头数目
	INT                     boneIndex[MAX_BONE_COUNT];  // 影响该submesh的骨头在骨架中的索引

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

// model的submodel
class VGS_EXPORT CSubModel
{
public:
	CMaterial*      m_pMaterial;
	CVGSBoundBox    m_boundBox;
	CVGSBoundSphere m_boundSphere;

	// 在顶点序列中的开始位置和数目
	UINT            m_vertexStart;
	UINT            m_vertexCount;

	// 在面索引序列中的开始位置和数目
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
//目前使用包围球更加方便，包围盒暂时没有用到，lixin, 2006-05-12

//---------------------------------------------------------------------------------
//一个模型应该被渲染到哪些表面？默认情况下，模型只需要渲染到后表面即可。
//然而，很多时候，我们使用RenderToTexture技术渲染反射、折射、各种特效等。
//我们可以使用独立的渲染过程来渲染每个渲染表面，这样做的缺点是由于频繁切换渲染状态，
//导致渲染效率低下。理想的办法是渲染每个模型时，一次性渲染到多个表面。
//这里设计了一个用于辅助模型渲染的结构，用于模型的渲染
//---------------------------------------------------------------------------------
enum RENDERTOTEXTURE_TYPE
{
	ADS_RTFMAP,          // 实时反射贴图
	ADS_RTTMAP,			 // 实时折射贴图
	ADS_BUMPWAVEMAP,	 // 凹凸水面贴图
	ADS_CUBEMAP          // 实时cubemap
};

typedef struct ADDITIONALRENDERINFO
{
	RENDERTOTEXTURE_TYPE			type;
	DWORD							pRenderToTexture;
}ADDITIONALRENDERINFO;

class VGS_EXPORT CModel : public CMovableObject
{
private:
	//挂接Model的场景SceneNode节点
	//CSceneNode*				pNode;      
	//Model的名字，取和SceneNode相同的名字
	//std::string				name;	

	//Model的D3DXMesh对象		
	LPD3DXMESH				m_pMesh;

	//和Mesh绑定的骨架对象
	CSkeleton*				m_pSkeleton;

	//用于骨骼蒙皮的D3DXMESH对象
	LPD3DXMESH				m_pSkinMesh;

	//用于进行蒙皮绑定和计算的ISkinInfo对象
	LPD3DXSKININFO			m_pSkinInfo;

	//S3D 保存的Mesh数据对象
	CVGSMesh*				m_pVGSMesh;

	//创建Mesh的option
	DWORD					m_optionXMesh;

	//Mesh使用的材质指针列表，每个SubMesh使用一个材质
	// vector<CMaterial*>		m_vpMtrl;

	vector<CSubModel>       m_vSubModel;  // 子模型

private:
	//初始化
	HRESULT					Init();
	//写入顶点缓冲数据
	HRESULT					FillVertexBuffer();
	//写入顶点缓冲数据
	HRESULT FillVertexBuffer(void* pVertexSrcData);
	//写入索引缓冲数据
	HRESULT					FillIndexBuffer(vector<CMaterial*> vpShader);
	//锁定并填充模型的索引缓冲
	HRESULT                 FillIndexBuffer(void* pIndexBuf);
	//写入属性表
	HRESULT					FillAttributeTable();
	//写入属性表
	HRESULT                 FillAttributeTable(vector<CSubModel> &vSubmodel);
	//填充SkinInfo蒙皮对象并绑定骨骼数据
	HRESULT					CreateSkeletonAndSkin();
	//填充骨骼层次结构
	CBone*					CreateSkeletonHierarchy(VGS_Bone* pVGSBone, DWORD boneIndex);

public:
	//创建CModel对象
	HRESULT					Create(CVGSMesh* pVGSMesh, vector<CMaterial*> vpShader);

	// 创建模型
	HRESULT		Create(DWORD fvf, DWORD vertexCount, DWORD faceCount, void* pVertextBuf, void *pIndexBuf, vector<CSubModel> &vSubModel);

	void                    generateSubmeshIndexData();
	LPD3DXMESH getD3DXmesh() {return m_pMesh;}
public:
	LPDIRECT3DDEVICE9		m_pd3dDevice;	//D3D Device对象
	//D3D对象变量--------------------------------------------------------
	LPDIRECT3DVERTEXBUFFER9  m_VBuffer;		//顶点缓冲
    VOID*					 m_pVertices;	//顶点缓冲锁定的内存区域
	DWORD					 m_FVF;			//顶点格式FVF
	UINT					 m_FVFSize;     //一个顶点数据占用的空间大小
	UINT                     m_vertexCount;  // 定点数目
	UINT                     m_faceCount;    // 面数
	
	HRESULT LockVertexBuffer(void **pVertexBuffer, DWORD op = D3DLOCK_READONLY);    // 加锁顶点缓冲	
	HRESULT UnlockVertexBuffer();  // 解锁顶点缓冲

	HRESULT LockIndexBuffer(void **pIndexBuffer, DWORD op = D3DLOCK_READONLY);
	HRESULT UnlockIndexBuffer();
	
	//HRESULT LockIndexBuffer(INT iSubMeshIdx);    // 加锁子模型的索引缓冲
	//HRESULT UnlockIndexBuffer(INT iSubMeshIdx);  // 解锁子模型的索引缓冲

	CSubModel* getSubModel(int submeshIndex);

public:
	INT					index;				//渲染模型在渲染队列中的编号
	// std::string			name;
	
	VGS_BoundBox		oriBBox;			//没有变换过的包围盒
	VGS_BoundSphere			oriBSphere;			//没有变换过的包围球

	VGS_BoundBox		boundingBox;		//模型的包围盒
	VGS_BoundSphere		boundingSphere;		//模型的包围球
//	D3DXVECTOR3			centerInViewSpace;  //在View空间的球心位置

	//UINT				faceCnt;			//面个数
	//CFace*				face;				//面表
	UINT				vertexCnt;			//顶点个数
	CVertex*			vertex;				//顶点表
    
	//vector<_subMesh>	subMesh;    //subMesh表
	
	INT					opacity;			//模型的透明度
	INT					oriOpacity;			//原始透明度

	BOOL				mouseInOutable;		//是否响应鼠标在物体内外事件

	BOOL				bMouseShader;		//是否响应鼠标事件变化模型使用的材质

	//Edit by baojiahui 目前的版本不支持鼠标在物体上替换材质的功能，只支持颜色变化
	/*CMaterial*			mouseOverShader;	//鼠标移上去后的材质
	CMaterial*			mouseClickedShader; //鼠标选中后的材质
	CMaterial*			mouseUpShader;		//鼠标松开后的材质*/

	BOOL				bMouseColor;		//是否响应鼠标事件改变模型的顶点颜色
	D3DCOLOR			oriColor;			//模型的顶点色
	D3DCOLOR			mouseOverColor;		//鼠标移上模型后的顶点色
	D3DCOLOR			mouseClickedColor;	//鼠标选中模型后的顶点色
	D3DCOLOR			mouseUpColor;		//鼠标松开后的顶点色;		//鼠标松开后的顶点色

	BOOL                bModelColor;        //是否使用模型自身颜色而材质颜色
	D3DCOLOR            modelColor;         //模型颜色
	
	BOOL				visible;			//是否可见
	UINT				culled;				//是否在视锥体外
	BOOL				clickable;			//是否可以点选
	BOOL				rayHitable;			//是否可以被射线求交
	UINT				colStatus;			//0表示不侦测碰撞，1表示Sphere碰撞，2表示Box碰撞，3表示Mesh碰撞
	BOOL				isSkyBox;			//是否是天空盒
	BOOL				bSelected;			//是否已经被选中
	
	UINT				currentTimer;		//当前时钟
	UINT				kfrCnt;				//当前模型的关键帧数量
	UINT				kfrFrameTime;		//关键帧动画播放一帧耗时
	UINT				kfrCurFrame;		//当前播放的帧
	INT					startFrame;			//播放关键帧动画的开始帧
	INT					endFrame;			//播放关键帧动画的终止帧

	D3DXMATRIX			m_matIdentity;		//单位矩阵

	//用于MoveModelTo, RosModelTo, ScaleModelTo帧变换方法
	D3DXVECTOR3*		user_moveKFR;			//用户自定义模型帧变换的逐帧变换矢量
	INT					user_moveFrameCnt;		//用户自定义模型帧变换矩阵帧数
	INT					user_moveCurFrame;		//当前帧
	BOOL				bMatrixChanged;			//是否已经变换
    //模型的关键帧动画（此部分要写到node里）
	BOOL				kfrPlay;			//设定是否播放关键帧动画
	INT					kfrRepeatCnt;		//关键帧动画播放次数，-1表示无限循环
	UINT				kfrRepeatIndex;		//当前播放的次数
	/*D3DXMATRIX			*matrix_kfr;		//关键帧动画当前模型的逐帧变换矩阵
	D3DXMATRIX			matrix_CurFrame;	//关键帧动画当前帧变换矩阵*/
/*
	UINT				vkfrCnt;			//是否包含顶点动画
	BOOL				vkfrPlay;			//设定是否播放顶点动画
	INT					vkfrRepeatCnt;		//顶点动画播放次数，-1表示无限循环
	UINT				vkfrRepeatIndex;	//顶点动画当前播放的次数	
	CVertex				**vertices_kfr;		//顶点动画，逐帧各个顶点*/
	

	BOOL				isBillboard;		// 是否是Billboard模型 
	VGS_PointAtVectors		pointAtOrient;		// BillBoard对正向量 
	D3DXVECTOR3         billboardDirectionAxis;      // 模型正对相机的自身坐标轴
	// fuxb 2008-09-09 增加两个参数 ==>
	D3DXVECTOR3         billboradUpAxis;     // 模型正对相机时的Up方向轴
	bool                isAutoNormalise;     // 模型缩放时，是否应该自动更新法线。避免出现光照计算错误 
	// <==

	BOOL				isCollision;		//是否是碰撞模型
	FLOAT				colDetectDistance;	//碰撞检测的距离，多少距离以内就认为碰撞发生了

	BOOL				isGround;			//是否是地面模型

	BOOL				isZOne;				//是否是ZOne模型

	BOOL                m_isClickable;      // 是否可点击

	BOOL				bDynamicUV;			//是否是动态UV
//	FLOAT				UStep;				//U方向移动步长
//	FLOAT				VStep;				//V方向移动步长
protected:
	/**坐标系
	*/
	Axis*				m_pAxis;
	/**平移坐标系
	*/
	AxisMove*			m_pAxisMove;
	/**旋转坐标系
	*/
	AxisRotate*			m_pAxisRotate;
	/**缩放坐标系
	*/
	AxisScale*			m_pAxisScale;
	/**是否显示轴对象
	*/
	bool				m_bShowAxis;
	/** 是否已经渲染过坐标系，内部控制变量.
	*/
	bool				m_hHasRenderAxis;
	/**显示哪种坐标系：0-显示m_pAxis，1－m_pAxisMove，2－m_pAxisRotate，3－m_pAxisScale，其他值不显示.
	*/
	int					m_nAxisIndex;
protected:
	/** 创建坐标系.
	*/
	void CreateAxis();
	/** 销毁坐标系对象.
	*/
	void DestroyAxis();
	/** 更新坐标系.
	*/
	void UpdateAxis();
public:
	Axis* GetAxis() { return m_pAxis; }
	AxisMove* GetAxisMove() { return m_pAxisMove;}
	AxisRotate* GetAxisRotate() { return m_pAxisRotate; }
	AxisScale* GetAxisScale() { return m_pAxisScale; }
	/** 根据坐标的当前显示状态获取坐标系对象.
	*/
	Axis* GetCurAxis();
	/** 设置是否显示坐标系.
		这里动态创建或销毁坐标系对象.
	*/
	void SetShowAxis(bool bShow);
	/** 设置要渲染的坐标系状态.
	*/
	void SetAxisStatus(int nStatus) { m_nAxisIndex = nStatus; }
	/** 渲染坐标系.
	*/
	void RenderAxis(D3DVIEWPORT9* pViewport, D3DXMATRIX* pView, D3DXMATRIX* pProjection);

public:
	CRenderBoundBox*    pRBBox;             //包围盒模型对象
	BOOL                showRBBox;          //是否显示包围盒模型

	// CSceneNode*         pNode;             //当前节点

    
	DWORD				ID1;				//模型的ID
	DWORD				ID2;				//模型的扩展ID;

	//##ModelId=47EBC7480242
	INT					refModelIndex;		//引用物体编号
	//##ModelId=47EBC7480246
	CModel*				refModel;			//引用物体，如果该本物体是复制出来的物体，则refModel为其原始物体

	BOOL				bUseFBL;			//是否使用分辨率功能

	DWORD				modelGroup;			//属于哪个modelGroup
	BOOL				bmouseHintByGroup;	//是否使用[模型组]来处理鼠标移上物体和选中物体

	//是否显示反射标志
    BOOL                bReflect;
    //该物体要反射的信息索引，如果没有反射 则为-1
	int                 sceneRefInfoListIdx;
    
	vector<UINT>        refedModelIdxList;           //该物体反射的物体列表索引

	CModel*				pCloneModel;				//克隆模型的原模型

	vector<CCubeMap*>	useMeCubeMap;			//CubeMap列表，如果CubeMap需要反射该模型，则useMeCubeMap包含该CubeMap
	vector<CBumpMap*>   m_useMeBumpmap;

	
	//------------------------------------------------------------------------
	//渲染本模型需要的附加信息，用于本模型一次渲染到多个表面
	//------------------------------------------------------------------------
	vector<ADDITIONALRENDERINFO>	m_additionalRenderInfo;

	// std::string GetName();
	// VOID SetName(const std::string &sName);

	UINT getSubModelCount(){return m_vSubModel.size();}
	CMaterial* getSubModelMaterial(INT subMeshIndex);
	VOID setSubModelMaterial(INT subMeshIndex, CMaterial *pMaterial);

	// 添加移除cubemap，如果添加了cubemap，那么cubemap就会反射该模型
	VOID addCubemap(CCubeMap *pCubemap);
	VOID removeCubemap(CCubeMap *pCubemap);
	VOID addBumpmap(CBumpMap* pBumpmap);
	VOID removeBumpmap(CBumpMap* pBumpmap);

public:
	//移动贴图坐标UV
	VOID	MoveVertexUV(UINT uvIdx, FLOAT x, FLOAT y);
	//设置透明度
	VOID	SetOpacity(UINT opacity);
	//设置材质球
	VOID	SetMtrl(CMaterial* pMtrl, int subMeshIdx);
	CMaterial *GetMaterial(int subMeshIdx);
	//设置颜色
	VOID	SetColor(INT red, INT green, INT blue, INT alpha);
	//Add by baojiahui                              2009/1/15---------------------------------
	//设置顶点颜色
	VOID    SetVertexColor(INT red, INT green, INT blue, INT alpha);
    //----------------------------------------------------------------------------------------
	BOOL    IsTransparent();               // 模型及其使用的材质是否透明	
	BOOL    IsUseTransparentMaterial();    // 模型是否使用透明材质

	BOOL    IsInsideSphere(D3DXVECTOR3* pPos);   // 点是否在模型包围球内部
	BOOL    IsInsideBox(D3DXVECTOR3* pPos);      // 点是否在模型包围盒内部
	
	// 点到模型包围盒所有顶点的最近距离平方，不区分点在模型内部还是外部的情况
	FLOAT getNearDistanceSq(D3DXVECTOR3* pPos);

//变换相关
public:
	//绕自身坐标系或世界坐标系平移
	void	translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);  //coordEnum = 0 = worldcoord, 1 = selfcoor;
	//通过设置模型的坐标来设置模型的位置，世界坐标系, Pivot位置为模型的位置
	void	setPosition(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//绕自身坐标系或世界坐标系旋转
	void	rotate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//绕穿过固定点的指定轴旋转一定的角度
	void	rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, float fAngle, VGS_TRANSFORMSPACE coordEnum);
	//直接设置旋转的角度，x,y,z三轴
	void	setRotationByAngle(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//以自身坐标中心或世界坐标中心放大
	void	scale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//以固定点为中心放大
	void	scaleByPoint(D3DXVECTOR3* pVec, D3DXVECTOR3* pPos, VGS_TRANSFORMSPACE coordEnum);
	//直接设置缩放
	void	setScale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum);
	//更新
	void    UpdateFromNode();
	//单独渲染这个模型的方法
	void	render();

	//渲染模型的SubMesh
	VOID	RenderSubMesh(INT index);


public:
	//创建当前模型的顶点缓冲
	HRESULT createVBuffer();
	//创建当前模型的索引缓冲
	HRESULT createIBuffer();
	//克隆模型到场景
	CModel*	clone(std::string newModelName);
	//释放
	VOID	Release();

public:
	CModel(LPDIRECT3DDEVICE9 pd3dDevice);
	~CModel();

private:


public:
	
	vector<D3DXMATRIX> m_BoneOffsetMatrix;      // 影响model的每块骨头对应的offset matrix
	INT                m_SkeletonIndex;         // 模型使用的骨架索引

};

#endif 
