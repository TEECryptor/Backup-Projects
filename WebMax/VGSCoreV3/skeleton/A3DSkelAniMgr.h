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
    //����Device;
    void                SetDevice(IDirect3DDevice9* pDevice , HINSTANCE hInstance );
    //����ģ��
    CA3DModel*          LoadModel(const char* modelName , const void* buf , int bufLen);
    //����������������modelNameΪģ�����֣�����Ѿ����ڣ����Զ�ȡ����������ڡ�����Զ�����һ��
	CA3DSkelAnimation*  CreatekelAnimation(const char* modelName , const char* aniName ,  const void* buf , int bufLen , D3DXVECTOR3& pos , D3DXVECTOR3& scale , D3DXVECTOR4& rot);
    //ɾ������������
    void                RemoveSkelAnimation(CA3DSkelAnimation* pAni);

    //����һ��ģ��
    CA3DModel*          FindModelByName(const char* _name);

    //ͨ��һ�����������֣����ң����������ڴ�����ʱ��ָ��
    CA3DSkelAnimation*  FindAnimationByName(const char* _name);

    //�������еĶ�������ɾ�����������
    void                ClearAllAnition();

    //׼�����ƣ���Ҫ��׼��һЩ���õľ���һ��Ķ���
    void                BeginDraw();

    //���ƽ���
    void                EndDraw();
    //������Щ�Ѿ�����ɼ��Ķ���������
	void                DrawAllEnabledAnimation(DWORD passedTime);

    //�ͷ����е���Դ    
	void                Uninit();
 
    //����Camera��Ϣ������ȾPerPixel Lighting������Ҫ��Щ��Ϣ
    BOOL                SetCameraInfo(D3DXVECTOR3& up , D3DXVECTOR3& dir , D3DXVECTOR3& pos , float _Near , float _Far , float _Fov , float _fAspect);

    //��ȡD3DDeviceWrapper;
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


    //����
	BOOL     Draw(DWORD passedTime);


    //����һ��Mesh
	BOOL     AddMesh(CCoreMesh* pMesh);

    //ɾ��һ��Mesh
	BOOL     RemoveMesh(CCoreMesh* pMesh);

    //����һ������
    BOOL     AddAction(CBaseAction* pMesh);

    //�Ƴ�һ������;
    BOOL     RemoveAction(CBaseAction* pAction);

	//����һ������
	BOOL     SetAction(int ActionIdx , int iLoopTime = -1 , int TrackIdx = 0);

    //����һ������
    BOOL     SetAction(const char* _actionName , int iLoopTime  = -1 , int TrackIdx = 0 );

    //����һ��������
    //_actionList    : �� / �������������� "Action1/Action2/Action3"
    //bUseTransition : ���������Ƿ��й���
    BOOL     SetActionList(const char* _actionLists , int iLoopTime = -1 , BOOL bUseTransition = true , int TrackIdx = 0);

    //����һ�������ĵ�a��b֡
    //FrameA : ���ŵ���ʼ֡
    //FrameB : ���ŵĽ���֡
    //iLoopTime : ѭ�������� -1 ��ʾ����ѭ��
    BOOL     SetAction(int ActionIdx           , int frameA , int frameB , int iLoopTime = -1 , int TrackIdx = 0);
	
    //����һ�������ĵ�a��b֡
    BOOL     SetAction(const char* _actionName , int frameA , int frameB , int iLoopTime = -1, int TrackIdx = 0 );    


	   //������������֮��Ĺ���ʱ�䣬������Щ��ѭ��������֡��β��ӵģ�����Ӧ�ðѹ��ɶ�������Ϊ0����
    BOOL     SetActionTransitionTime(DWORD _transTime , int TrackIdx = 0);

/*


	// ����һ�����������ݶ������������趨  add by tim
	BOOL     SetAction(const char *ActionName , BOOL bLoop , int TrackIdx = 0);

    //������������֮��Ĺ���ʱ�䣬������Щ��ѭ��������֡��β��ӵģ�����Ӧ�ðѹ��ɶ�������Ϊ0����
    BOOL     SetActionTransitionTime(DWORD _transTime , int TrackIdx = 0);
    //����һ������
	BOOL     SetAction(const char** _ActionNameList , int _nAction , BOOL bLoop );

	void     SetSkaRepeatInfo(int TrackIdx, int from ,int  to , int repeat);
*/

	VOID     SetPosition(const D3DXVECTOR3& pos, bool bMoving = false);	
	void     MovetoPosition(const D3DXVECTOR3& pos ,  int step);



	VOID     SetScale(const D3DXVECTOR3& scale);
	VOID     SetRotation(const D3DXVECTOR4& rot);
   
    BOOL     SetTransform(const D3DXVECTOR3& pos , const D3DXVECTOR3& scale , const D3DXVECTOR4& rot );

    bool     SetEffectShader(const char* file);
//˽����;������Ҫ�ⲿʹ��
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