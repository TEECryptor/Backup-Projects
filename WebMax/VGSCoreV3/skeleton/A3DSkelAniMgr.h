#pragma once
#include "./a3dModel/CD3DInclude.h"
#include "A3DHelperFunction.h"

class _A3D_API_ CA3DSkelAnimation;
class _A3D_API_ CMixedAction;
class _A3D_API_ CA3DModel;
class _A3D_API_ CCoreMesh;
class _A3D_API_ CBaseAction;
class _A3D_API_ CActionTrack;
class _A3D_API_ IActionTrackListenner;
class _A3D_API_ CCoreSkeleton;
class _A3D_API_ CCoreActionFrame;
class _A3D_API_ CA3DDeviceWrapper;
enum eActionTrackEvent;

class _A3D_API_ CA3DModelLoader
{
public:  
	typedef std::map<std::string , CA3DModel*    >      CA3DModelMap;
	typedef std::map<std::string , CA3DSkelAnimation*>  CA3DSkelAnimationArray;
public:
	CA3DModelLoader();
    ~CA3DModelLoader();
	void                OnDeviceLost();
	void                AfterDeviceReset(IDirect3DDevice9* pDevice  );
    //设置Device;
    void                SetDevice(IDirect3DDevice9* pDevice , HINSTANCE hInstance );
    //加载模型
    CA3DModel*          LoadModel(const char* modelName , const void* buf , int bufLen);
    //创建动画管理器，modelName为模型名字，如果已经存在，则自动取出。如果不在。则会自动创建一份
	CA3DSkelAnimation*  CreatekelAnimation(const char* modelName , const char* aniName ,  const void* buf , int bufLen , D3DXVECTOR3& pos , D3DXVECTOR3& scale , D3DXVECTOR4& rot);
    //删除动画管理器
    void                RemoveSkelAnimation(CA3DSkelAnimation* pAni);

    //查找一个模型
    CA3DModel*          FindModelByName(const char* _name);

    //通过一个动画的名字，查找，动画名字在创建的时候指定
    CA3DSkelAnimation*  FindAnimationByName(const char* _name);

    //清理所有的动画。会删除这个动画的
    void                ClearAllAnition();

    //准备绘制，主要是准备一些公用的矩阵一类的东西
    void                BeginDraw();

    //绘制结束
    void                EndDraw();
    //绘制那些已经激活并可见的动画管理器
	void                DrawAllEnabledAnimation(DWORD passedTime);

    //释放所有的资源    
	void                Uninit();
 
    //设置Camera信息。在渲染PerPixel Lighting必须需要这些信息
    BOOL                SetCameraInfo(D3DXVECTOR3& up , D3DXVECTOR3& dir , D3DXVECTOR3& pos , float _Near , float _Far , float _Fov , float _fAspect);

    //获取D3DDeviceWrapper;
    CA3DDeviceWrapper*  GetDevice(){ return m_DeviceWrapper ; }

    bool                IsMaterialSupport(const char* _file);

	void                FrustrumTestA3D(D3DXVECTOR3* pCamPos, D3DXMATRIX* pMatView,
							float tanHalfFieldOfViewV, float nearClip, float farClip);

	bool				IsInFrustrum(D3DXVECTOR3* pos, D3DXMATRIX* pMatView, float tanHalfFieldOfViewV, float nearClip, float farClip ) ;


	CA3DSkelAnimation  *getMouseHitSkeleton( D3DXVECTOR3 *orig,  D3DXVECTOR3* dir  );
protected:
	CA3DModelMap           m_MapModels;
    CA3DSkelAnimationArray m_vSkelAnimations;
	CA3DDeviceWrapper*     m_DeviceWrapper;
    IDirect3DStateBlock9*  m_OutStateBlock;
	IDirect3DVertexDeclaration9* m_pOldVDecel;
	DWORD                        m_OldFVF;
};


//friend class xActionStateListenner;


class CA3DSkelAnimation : public CRefCountObject
{
public:
    CA3DSkelAnimation(const char* _name , CA3DModelLoader* pModelMgr , CA3DModel* pModel , BOOL bAddMeshs);
    ~CA3DSkelAnimation();
    const char* name(){ return m_name.c_str() ; }
    BOOL     IsEnable();
	void     Enable(BOOL bEnable);

	BOOL     IsVisible();
	void     SetVisible(BOOL bVisible);

	bool     IsCulling() {return  m_bCulling;}
	void     SetCulling(bool bCulling) { m_bCulling = bCulling; }


    //绘制
	BOOL     Draw(DWORD passedTime);


    //增加一个Mesh
	BOOL     AddMesh(CCoreMesh* pMesh);

    //删除一个Mesh
	BOOL     RemoveMesh(CCoreMesh* pMesh);

    //增加一个动作
    BOOL     AddAction(CBaseAction* pMesh);

    //移除一个动作;
    BOOL     RemoveAction(CBaseAction* pAction);

	//播放一个动作
	BOOL     SetAction(int ActionIdx , int iLoopTime = -1 , int TrackIdx = 0);

    //播放一个动作
    BOOL     SetAction(const char* _actionName , int iLoopTime  = -1 , int TrackIdx = 0 );

    //播放一连串动作
    //_actionList    : 用 / 隔开动作，比如 "Action1/Action2/Action3"
    //bUseTransition : 两个动作是否有过渡
    BOOL     SetActionList(const char* _actionLists , int iLoopTime = -1 , BOOL bUseTransition = true , int TrackIdx = 0);

    //播放一个动作的第a到b帧
    //FrameA : 播放的起始帧
    //FrameB : 播放的结束帧
    //iLoopTime : 循环次数， -1 表示无限循环
    BOOL     SetAction(int ActionIdx           , int frameA , int frameB , int iLoopTime = -1 , int TrackIdx = 0);
	
    //播放一个动作的第a到b帧
    BOOL     SetAction(const char* _actionName , int frameA , int frameB , int iLoopTime = -1, int TrackIdx = 0 );    


	   //设置两个动作之间的过渡时间，对于那些是循环动作（帧首尾相接的），则应该把过渡动作设置为0）。
    BOOL     SetActionTransitionTime(DWORD _transTime , int TrackIdx = 0);

/*


	// 播放一个动作，根据动作的名称来设定  add by tim
	BOOL     SetAction(const char *ActionName , BOOL bLoop , int TrackIdx = 0);

    //设置两个动作之间的过渡时间，对于那些是循环动作（帧首尾相接的），则应该把过渡动作设置为0）。
    BOOL     SetActionTransitionTime(DWORD _transTime , int TrackIdx = 0);
    //播放一串动作
	BOOL     SetAction(const char** _ActionNameList , int _nAction , BOOL bLoop );

	void     SetSkaRepeatInfo(int TrackIdx, int from ,int  to , int repeat);
*/

	VOID     SetPosition(const D3DXVECTOR3& pos, bool bMoving = false);	
	void     MovetoPosition(const D3DXVECTOR3& pos ,  int step);



	VOID     SetScale(const D3DXVECTOR3& scale);
	VOID     SetRotation(const D3DXVECTOR4& rot);
   
    BOOL     SetTransform(const D3DXVECTOR3& pos , const D3DXVECTOR3& scale , const D3DXVECTOR4& rot );

    bool     SetEffectShader(const char* file);
//私有用途。不需要外部使用
	void     onActionTrackEvent(eActionTrackEvent _event, int idx ,  CBaseAction* pAction);

    D3DXMATRIX                m_PosMatrix;
	CBoundBox                 m_BoundBox; 

	D3DXVECTOR3  GetPos() {return m_Pos; }

protected:

	void          updateTransform( DWORD passedTime );
	
	void          _BuildBoundBox();
	void          _drawAllMesh(bool bCpuSkin) ;
	void          _ToCpuSkin(bool bCpuSkin) ;
	CBaseAction*  _findAction(const char* _name);



protected:
    std::string               m_name;
    typedef std::vector<CActionTrack*> vActionTracks;
	CCoreSkeleton*            m_pSkeleton;
    CA3DModel*                m_pModel;
	CCoreActionFrame*         m_ActionFrame;
	std::vector<CCoreMesh*>   m_vMeshs;
    std::vector<CBaseAction*> m_vActions;

    vActionTracks             m_vActionTrack;
    IActionTrackListenner*    m_TrackListenner;

    CMixedAction*             m_pMixedAction;


    CA3DModelLoader*          m_pModelMgr;
    BOOL                      m_bVisible;
    BOOL                      m_bEnable;

	bool                      m_bCulling;
    CA3DDeviceWrapper*        m_DeviceWrapper;
    CFxShader*                m_pFxShader;

	std::wstring              m_strShaderName;
    bool                      m_bGpuSkin;


    D3DXVECTOR3               m_Pos;
    D3DXVECTOR4               m_Rotate;
    D3DXVECTOR3               m_Scale;

	D3DXVECTOR3               m_destPos;
	int                     m_nMoveStep;




};