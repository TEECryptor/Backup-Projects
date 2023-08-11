/////////////////////////////////////////////////////////////////////////
// ����: CScene����ģ�Ͷ���--��������
// ����: �����ĳ�ʼ������������Ⱦ�����ƶ����������ʵ��
//       ��������Ⱦ�Ѿ�̬ģ�͡��ؼ�֡��̬ģ�͡�͸��ģ�ͷֿ���Ⱦ�������ȾЧ��
// ����: lixin
// ����: 2006-04-30(2)
/////////////////////////////////////////////////////////////////////////

#ifndef _CSCENE_H
#define _CSCENE_H

#include "VGSHeader.h"

#include "CSysFunc.h"
#include <stdio.h>
#include "CManyFVF.h"
#include "CD3DDevice.h"
//#include "CInput.h"
#include "CFont.h"

//����Ԫ��
#include "CCamera.h"
#include "CTexture.h"
#include "CDynamicTex.h"

#include "CMaterial.h"
#include "CModel.h"
#include "CModelGroup.h"
#include "CLight.h"
#include "CText.h"
#include "CImage.h"
//GUI
#include "GUI\CGUIManager.h"
#include "GUI\background.h"
//������
#include "Axis.h"

//����ͼ��
#include "CSectionMap.h"

//��Ч
#include "CFullScreenMotionBlur.h"
#include "CFSGlow.h"
#include "CLensFlare.h"

//������
#include ".\Controller\CWalker.h"
#include ".\Controller\CEditor.h"
#include ".\Controller\CFlyer.h"

#include "RealWater.h"

#include "VGSGetRoot.h"

#include <vector>
#include <set>
using namespace std;


#include "skeleton/a3dManager.h"

// #include "GfxManager.h"

/*
//�Զ�����Ϣ
//����������ŵ�ĳһ֡ʱ���͸������ڵ���Ϣ������JS Func�ص�
#define WM_JSFUNC_PROC		(WM_USER + 119)	
#define WM_ZTRIGFUNC_PROC   (WM_USER + 1999)
*/
// material type
enum VGS_MATERIAL_TYPE
{
	VGS_MATERIAL_TYPE_3D = 0,
	VGS_MATERIAL_TYPE_2D
};


typedef struct RenderToTexture
{
	UINT                        texStage;
	LPDIRECT3DTEXTURE9			curRenderTexture;             //��ǰ�󻺳��Ӧ����Ⱦ������(Render-to-Texture)
	LPDIRECT3DSURFACE9			curRenderSurface; 
	LPDIRECT3DSURFACE9          curRenderZSurface;
}RenderToTexture;

typedef struct ReflectInfo
{
	D3DXPLANE        refPlane;
	CModel*          refModel;
	RenderToTexture  renderToTexInfo;  
}ReflectInfo;

class VGS_EXPORT CScene : public CVGSGetRoot
{
	//D3D��ʾ�豸��صı���------------------------------------------
public:	
 	LPDIRECT3D9				m_pd3d;							//Direct3D����
	LPDIRECT3DDEVICE9		m_pd3dDevice;					//D3D Device����
	// CD3DDevice*				m_DeviceInfo;					//��ǰϵͳ�������豸��Ϣ
	D3DPRESENT_PARAMETERS	m_d3dpp;						//��ǰ���Կ�����
	D3D_PARAM				d3dParam;
	UINT					m_AvailableVideoMemSize;		//ʣ����Դ��С
	
	D3DDISPLAYMODE			m_d3dmm;						//��ǰ���Դ����

    LPDIRECT3DSURFACE9		m_pBackBuffer;					//�Կ��󻺳����
	LPDIRECT3DSURFACE9		m_pZBuffer;						//�Կ�ZBuffer����

	INT                     m_index;                        // �ڳ��������е�����
	std::string             m_sName;

	//CInput					InputDevice;					//�����豸����
	
	//��ʾ����
public:
	HWND					m_hWnd;							//����D3D�豸�Ĵ���
    CHAR					windowTitle[256]; 				//���ڱ���
    BOOL					m_bWindowed;					//�Ƿ��Դ���ģʽ��ʾ

	INT						m_oriWidth;						//���õĳ�ʼ���
	INT						m_oriHeight;					//���õĳ�ʼ�߶�
	D3DCOLOR				m_backgroundColor;						//������ɫ

	//RECT					m_SceneRect;							//����RECT
	//FLOAT					m_SceneRatio;							//�������ڵĿ�߱�width / height
	D3DVIEWPORT9			m_viewport;						//�ӿ�
	
    
    BOOL					bActive;						//��ǰD3D�����Ƿ񼤻�

	//�任�����йصĳ�Ա����---------------------------------------------------
public:
	INT						m_projMode;						//ͶӰģʽ��͸��ģʽΪ0�����ģʽΪ1
	D3DXMATRIX				m_MatWorld;						//world transform Matrix
	D3DXMATRIX				m_MatView;						//��ǰView�任����	
	D3DXMATRIX				m_MatProj;						//Projection Transform Matrix
	D3DXMATRIX				m_MatViewProj;					//�ӽǺ�ͶӰ����ĳ˻�

	D3DXMATRIX				m_MatViewOri;					//��ʼView�任����	
	D3DXMATRIX				m_MatViewInverse;				//��ǰView�任����������

	D3DXMATRIX				m_MatIdentity;					//��λ����
	D3DXMATRIX				m_RollMatrix;					//�������ת����
	D3DXMATRIX				m_SkyBoxMatView;				//SKYBox��view�任����
	D3DXMATRIX				m_SkyBoxMatProj;				//SkyBox��proj�任����

	
	//�û������йصĳ�Ա����-------------------------------------------------
public:
	BOOL					userInput;						//�û�����״̬

	POINT					lastClickPos;					//���һ�ΰ������ʱ����λ��
	D3DXVECTOR3				m_pickRayDir;					//��ǰ����ת��Ϊ��ά�ռ������߷���
	D3DXVECTOR3				m_rayDir;						//��ǰ֡���Ӿ�����ʸ��
    D3DXVECTOR3				m_rayOrig;						//��ǰ�Ӿ����ߵ���ʼ��λ�ã����ӵ�
	
	CModel*					m_CursorModel;					//����·���ģ��
	CButton*				pMouseOverButton;				//��ǰ����·��İ�ť

	COverlay*				pCtrlOl;						//������Ƽ�ͷOverlay
	COverlay*				pRosOl;							//��ת����Overlay
	BOOL					softKey[12];					//�����-�����
	
	INT						controlType;					//��ǰ�Ŀ�������0walk,1Editor, 2Flyers, eles None
	CWalker*				currentWalker;					//��ǰWalker����
	CEditor*				currentEditor;					//��ǰEditor����
	CFlyer*					currentFlyer;					//��ǰFlyer����
   
	INT                     sceneEditMode;                  //��ǰ�����ı༭״̬
	BOOL                    m_ModelEdit;                    //ģ���Ƿ񱻱༭��״̬
	BOOL                    pLButton;                       //��һ����������״̬
	D3DXVECTOR3             mouseWorldPos;                 //������������������е�λ��

	//��Ⱦ��������-----------------------------------------------------
public:
	UINT					renderStatus;					//��Ⱦ״̬
	BOOL					renderScene;					//�Ƿ���Ⱦ
	INT						m_FrameCnt;						//��ĿǰΪֹ��Ⱦ����֡��
	UINT					fps;							//��Ⱦ֡��
	INT						antiLevel;						//���ܳݽ��

	CModel*					m_CurModel;						//�Կ���ǰ��Ⱦ��ģ��
	CMaterial*				m_CurMtrl;						//��ʾ����ǰʹ�õĲ���	
	CTexture*				m_CurDifTex;					//��ʾ����ǰʹ�õ�DifTexture
	CTexture*				m_CurLightTex;					//��ʾ����ǰʹ�õ�LightTexture
	CMaterial*				m_CurCubeMtrl;					//��Ⱦ��ǰCubeMap�Ĳ��ʣ���Ⱦ��CubeMap�����û���
	CCubeMap*				m_CurCubeMap;					//��ǰ��Ⱦ��CubeMap
	CTexture				m_DefaultTex;					//Ĭ�ϵ���ͼ
	INT						m_CurTexStage;					//��ǰ����ͼStage

	CCamera*                m_pCurCamera;                   // ��ǰʹ�õ����
	CViewport*              m_CurViewport;                  // ��ǰҪ��Ⱦ���ӿ�
	CSwapChain*             m_pCurSwapChain;                // ��ǰ��Ⱦ��swapchain

	FLOAT					fbl;							//��ʾģ�͵İ�Χ��뾶��ģ�͵���ǰ����ľ���ı�ֵ�趨

	BOOL					m_bRenderScene;					//��Ⱦ����
	BOOL					m_bRenderGUI;					//��Ⱦ����

	CHAR					m_err[256];						//������Ϣ�ַ���
	
	BOOL					m_bAuthorized;					//����N3D�Ƿ�ȡ�úϷ���Ȩ

	//�����е���Դ---------------------------------------------------
public:
	// FLOAT					m_fileVersion;					//��ǰ��Ⱦ�ó�����Դ�ļ��İ汾��

	//ģ���йصĳ�Ա����----------------------------------------------
	vector<CModel*>			model;							//����ģ����Ⱦ����
	vector<CModel*>			m_vTransModel;						//͸��ģ����Ⱦ����
	// std::set<CModel*>            m_TransModel; // ͸��ģ�Ͷ���

    vector<CModel*>			m_SelectedModel;				//ѡ�е�ģ���б�
	//������
	Axis*				    m_pAxis;

    //Edit by Baojiahui     2007/9/27                       
	vector<TriggerCallBack*> triggerModel;                    //������ײ�����б�

	//Add by Baojiahui      2008/10/21
	vector<ReflectInfo>     reflectInfoList;
    BOOL                    beReflect;

	//ģ�����йصı���------------------------------------------------
	vector<CModelGroup*>	modelGroup;						//ģ����

	//����ͼ----------------------------------------------------------
	vector<CSectionMap*>    sectionMap;						//����ͼ

	//�����йصĳ�Ա����----------------------------------------------
	vector<CMaterial*>		m_vShader;							// 3D models ������Դ����
	vector<CMaterial*>      m_v2DMaterial;                      // GUI ������Դ����

	//��ͼ�йصĳ�Ա����----------------------------------------------
	vector<CTexture*>		m_vTexture;						//��ͼ��Դ����
	// vector<CBumpWater*>		bumpWater;						//BumpWater����
	vector<CCubeMap*>		cubeMap;						//CubeMap����
	//vector<DWORD>			movieTexture;					//movieTex����
	vector<CDynamicTex*>	dynamicTexture;					//��̬��ͼ

	vector<CBumpMap*>       m_vBumpTexture;
	vector<CRTFMap*>		m_vRtfTexture;					//ʵʱ������ͼ�б�

	//������йصĳ�Ա����--------------------------------------------
	vector<CCamera*>		m_vCamera;							//������б�

	//�ƹ��йصĳ�Ա����----------------------------------------------
	vector<CLight*>			m_vLight;							//�ƹ��б�

	//Overlay�йصĳ�Ա����-------------------------------------------
	vector<COverlay*>		m_vOverlay;						   //Overlay�б�

	BackGround*             m_pBackGround;                      // ����ͼ

	//Image�йصĳ�Ա����----------------------------------------------
	vector<CImage*>			m_vImage;							//image�б�

	//Navigater�йصĳ�Ա����------------------------------------------
	vector<CNavigator*>		m_vNavigater;						//navigaters�б�

	//Button�йصĳ�Ա����---------------------------------------------
	vector<CButton*>		m_vButton;							//button�б�

	//������صĳ�Ա����-----------------------------------------------
	vector<CFont*>			m_vFont;							//�������
	vector<CText*>			m_vText;							//�����б�

	//��Ƶ��صĳ�Ա����-----------------------------------------------
	// vector<DWORD>			m_vAudio;							//��Ƶ����

	//���������-----------------------------------------------------
	//vector<CWalker*>		m_vWalker;							//�������ζ���
	//vector<CEditor*>		m_vEditor;							//���չʾ���ƶ���
	//vector<CFlyer*>			m_vFlyer;							//�������ζ���

	//alphaTest ���
	DWORD                  alphaTestLevel;

	vector <CSwapChain*>   m_vSwapChain;                   // �����е�swapchain

	//##ModelId=47EBC730013D
	INT						m_sceneType;					//��׼����Ϊ0�������򳡾�Ϊ1

	// ������
	UINT getCameraCount();
	CCamera*  CreateCamera(const std::string &sName);
	CCamera*  GetCameraByName(const std::string &sName);
	CCamera*  GetCameraByIndex(INT index);
	std::string  GetCameraNameByIndex(INT index);
	INT GetCameraIndex(CCamera* pCamera);
	VOID DeleteCamera(CCamera* pCamera);
	VOID DeleteCamera(INT index);
	BOOL IsCameraExist(const std::string &sName);
	// ��������������������
	std::string renameCameraIfExist(std::string &sName);

	// �ƹ����
	UINT getLightCount();
	CLight*  CreateLight(const std::string &sName);
	CLight*  GetLightByName(const std::string &sName);
	CLight*  GetLightByIndex(INT index);
	std::string  GetLightNameByIndex(INT index);
	INT GetLightIndex(CLight* pLight);
	VOID DeleteLight(CLight* pLight);
	VOID DeleteLight(INT index);
	BOOL IsLightExist(const std::string &sName);

	CSceneNode*             m_pRootNode;                    // �����ĸ��ڵ�


	// ģ�����
	// ����ģ��
	CModel *CreateModel(const std::string &sName);
	UINT GetModelCount();
	CModel* GetModelByName(const std::string &sName);
	CModel* GetModelByIndex(INT index);
	std::string GetModelNameByIndex(INT index);
	INT GetModelIndex(CModel* pModel);
	VOID DeleteModel(CModel* pModel);
	VOID DeleteModel(INT index);
	BOOL IsModelExist(const std::string &sName);
	// ��ģ����ӵ�����
	void AddModel(CModel *pModel);

	// ��ģ�����/ɾ��͸��ģ�Ͷ���
	void AddModelToTransparentList(CModel *pModel);
	void RemoveModelFormTransparentList(CModel *pModel);

	//��Ч�����-----------------------------------------------------
public:
	//��̬ģ������
	CFullScreenMotionBlur*		m_pFSMotionBlur;			//��̬ģ������
	BOOL						m_bFSMotionBlur;			//�Ƿ�򿪶�̬ģ��
	INT							FSBlurLevel;				//ȫ����̬ģ���ļ���

	BOOL						m_bLastFSMotionBlurStatus;	//�ڲ�������ؼ�֡����ʱ�����涯̬ģ����״ֵ̬
	BOOL						m_bRenderCurrentFrame;		//�Ƿ���Ⱦ��ǰ֡
	BOOL						m_bLastFrameBlur;			//��һ֡�Ƿ�̬ģ��

	//ȫ�����ζ���
	CFSGlow*					m_pFSGlow;					//ȫ�����ζ���
	BOOL						m_bFSGlow;					//�Ƿ��ȫ������
	INT							FSGlowLevel;				//ȫ�����ε�ǿ��

	//��ͷ��߶���
	CLensFlare*					m_pLensFlare;
	BOOL						m_bLensFlare;				//�Ƿ�򿪾�ͷ�����Ч
	BOOL						m_bLensFlareCoverd;			//�Ƿ�����ڵ�
	BOOL						m_bLensFlare_CalCover;		//�Ƿ�����ڵ�
	BOOL						m_bLensFlare_Inverse;		//�Ƿ����ӽǱ���

	//�����
	BOOL						m_bFog;						//�Ƿ�ʹ����
	D3DCOLOR					m_FogColor;					//�����ɫ
	FLOAT						m_FogStart;					//�����ֹ��
	FLOAT						m_FogEnd;
	FLOAT						m_FogDensity;				//���Ũ��
	D3DFOGMODE					m_FogMode;					//���˥��ģʽ


	//�ؼ�֡��������------------------------------------------------------
public:
	INT							m_KfrCnt;					//�ؼ�֡����
	INT							m_KfrRepeatCnt;				//�ؼ�֡�������ŵĴ���
	INT							m_KfrRepeatIndex;			//�ؼ�֡�������ظ�����
	INT							m_CurrentFrame;				//��ǰ֡
	BOOL						m_bKfrPlaying;				//�ؼ�֡�����Ƿ��ڲ���
	INT							m_kfrPlayspeed;				//�ؼ�֡�������ŵ��ٶ�

	INT							m_StartFrame;				//�ؼ�֡�������ŵ���ʼ֡
	INT							m_EndFrame;					//�ؼ�֡�������ŵ���ֹ֡
	
	INT							m_TotalFrameCnt;			//�ܵ�֡��
	//INT						m_bRenderComplete;			//��ǰ֡�Ƿ���Ⱦ���
	DWORD						m_lastTimer;				//ϵͳʱ��


	//����Ĭ�ϵĶ���-----------------------------------------------
public:
	//Ĭ�ϵļ������
	/*CCamera						frontCamera;
	CCamera						backCamera;
	CCamera						leftCamera;
	CCamera						rightCamera;
	CCamera						topCamera;
	CCamera						bottomCamera;
	CCamera						userCamera;
	CCamera						perspectiveCamera;
	
	CCamera						defaultCam;						//Ĭ�ϵ������
	BOOL						useDefCam;						//�Ƿ�ʹ��Ĭ�ϵ����ģʽ
	CCamera*					currentCam;						//��ǰʹ�õ������
*/
	CFont*						pDefaultFont;
	
	COverlay*					m_pStartLogo;					//��˾Logo Overlay
	COverlay*					m_pUnitedLogo;					//���˹�˾��Logo Overlay
	COverlay*                   m_pLoadingBg;                   //loading background Image
	COverlay*                   m_pLoadingBar;                  //loading progress bar Image


//-----------------------------------------------------------------------------
// Name: �ڲ����ú���
//-----------------------------------------------------------------------------
private:

	//����BackBuffer�ĳ����
	// VOID UpdateRatio();
	//�ӽǱ任
	VOID ViewTransform(); 
	//ͶӰ�任
	VOID ProjTransform(); 
	//����Ĭ�ϵ������
	// VOID UseDefaultCam();
	//��Ⱦ�����Ӷ���-��͸��ģ��
	VOID RenderOpacModels();
	// VOID DrawOpacModel();
	//��Ⱦ�����Ӷ���-͸��ģ��
	VOID RenderTransModels();

	//��Ⱦ��ɫ����
	VOID RenderA3D();

	//��Ⱦˮ�����
	VOID RenderBumpWater(CMaterial* pMtrl);
	//��ȾLensFlare
	VOID RenderLensFlare();
	//��ȾCubeMap
	VOID RenderCubeMap(CMaterial* pMtrl, INT stageIndex);
	//��ȾLogo
	VOID RenderLogo();
	//��Ⱦ��͸����������
	VOID RenderOpacReflect(const VGS_RenderSubmesh& subMesh);
	//��Ⱦ͸����������
    //VOID RenderTransReflect(const UINT& subMeshIdx);

	//��Ⱦ�����Ӷ���-����
	VOID RenderText();
	//��Ⱦ�����Ӷ���-Overlay
	VOID RenderOverlay();
    //��Ⱦ�ض���overlay
	VOID RenderOverlay(COverlay* pOverlay);
	//��Ⱦbackdrop
	// VOID RenderBackdrop();
	//��Ⱦ�����
	VOID RenderAttach();
	//�����ͼ�ڴ�
	VOID ClearTextureMem();

	//�ҵ���ǰ���ģ�Ͷ�Ӧ�Ķ�̬ģ�ͱ��
	//INT GetDUnitIndex(GetS3DFileInfo* pS3DFile, UINT unitIndex);
	//����ģ�Ͷ���
	VOID PlayModelKeyFrame();
	VOID UpdateParentFrame(CModel** curModel);


	//�����༭״̬�µ��ú���
	//ʹ������ƶ��ض�������
	VOID MoveModel();
	//ʹ�������ת�ض�����
	VOID RotateModel();
    //������ת�ĽǶ�
    //-----------------------------------------------------------------------------
    FLOAT GetRotateAngle(D3DXVECTOR3* oriVector, D3DXVECTOR3* desVector, D3DXVECTOR3* axisVector);
	//��������༭״̬
	VOID setModelCtrl();
    
public:

    //����Ļ����ת����Ŀ��ƽ�����������
	VOID GetCoordScreenToWorldFromPlane(CViewport *pViewport/*DWORD pSwapChain*/, UINT x, UINT y, 
		                                 const VECTOR3 &planePoint, const VECTOR3 &planeNormal,
										 VECTOR3 &OutVec);
    // VOID GetCoordScreenToWorldFromPlane(CSwapChain* pSwapChain, UINT x,UINT y,D3DXVECTOR3* planePoint,D3DXVECTOR3* planeNormal,D3DXVECTOR3* pOutVec);
	//����Ļ����ת����Ŀ��ƽ�����������
    VOID GetCoordScreenToWorldFromPlaneAndRay(CViewport *pViewport/*CSwapChain* pSwapChain*/, UINT x,UINT y,D3DXVECTOR3* planePoint,D3DXVECTOR3* planeNormal,D3DXVECTOR3* dipartRaydir,D3DXVECTOR3* pOutVec);

	//������
	BOOL IntersectModel(D3DXVECTOR3* orig, 
						D3DXVECTOR3* dir, 
						CModel* pModel, 
						D3DXVECTOR3* pNormal, 
						INT*   faceIndex,
						FLOAT* pSU,
						FLOAT* pSV,
						FLOAT* distance);
	BOOL IntersectModelBound(D3DXVECTOR3* orig, D3DXVECTOR3* dir, CModel* pModel);
	BOOL IntersectModelMesh(D3DXVECTOR3* orig, D3DXVECTOR3* dir, CModel* pModel, 
									 INT* FaceIndex, FLOAT* pU, FLOAT* pV, 
									 FLOAT* pDist, D3DXVECTOR3* vColNormal);
	BOOL IntersectTriangle(D3DXVECTOR3* orig, D3DXVECTOR3* dir, 
							   D3DXVECTOR3* p0, D3DXVECTOR3* p1, 
							   D3DXVECTOR3* p2, //��������
							   float *pU, float *pV, //UV����
							   float *pDist); //����
	//���ģ���Ƿ�����׶����
	BOOL IsInFrustrum(CModel* pModel);
	//��ⶥ���Ƿ�����׶����
	BOOL IsInFrustrum(D3DXVECTOR3* pos);
	//���ü���
	VOID FrustrumTest();

	//����LensFlare����Ļ������㷽��
	VOID CalcScreenCoords(CViewport* pViewport, D3DXVECTOR3* pos, INT* ScreenX, INT* ScreenY);
	//��̬�ƹ�
	VOID	LightFrame();

	//���µ�����Pointer
	VOID	UpdateNavigaterPointer();
	//���µ�����Map
	VOID	UpdateNavigaterMap();

	//����moveModelTo�������ڲ�����
	VOID	PlayUserMoveModelKFR();

//-----------------------------------------------------------------------------
// Name: ���ⲿ���õĺ����ӿ�
//-----------------------------------------------------------------------------
public:
	//��ʼ������
	VOID	InitVar();
	//��յ�ǰ���������ͷų��������ж�����ڴ�
	VOID    Reset();
	//ɾ��Scene�����ͷ��ڴ�
	VOID	Release();
    //ɾ��image����
	VOID    ClearImageList();
	//�������е���ͼ��Դ,û���õ���ɾ��
	VOID	CleanTextureSource();

	// ��ȡ������ʹ�õ�ͼƬ�����б�
	void getImageNameList(bool includePath, vector<std::string> &vNameList);
//------------------------------------------------------------------------------
// ������صķ���
//------------------------------------------------------------------------------
	//���ñ���ɫ
	VOID    SetBackground(BYTE red, BYTE green, BYTE blue);
	//����Overlay��Rect
	VOID	UpdateOverlayRect(COverlay* pOverlay);

//------------------------------------------------------------------------------
// ��Ⱦ��صķ���
//------------------------------------------------------------------------------
	//��Ⱦ��������
	VOID Render(CCamera *pCamera, CViewport *pVP);

	//VOID RenderGrapha();

	//�����Ƶ�Ĳ���״̬
	VOID CheckAudioStatus();

	//���õ�ǰ�Ĳ���״̬
	VOID SetMaterial(CMaterial* pMtrl);

	//������ͼ��
	VOID SetTextureLayer(CTextureLayer* pTexLayer);
	//����bumpwater�Ĳ���;
	//VOID SetRenderWaterMaterial(CMaterial* pMtrl);
    //����Overlay��ͼ
	VOID SetOverlayTexture();
/*	//���÷�����ͼ
	VOID SetReflectionTexture();
	//����CubeMap������ͼ
	VOID SetCubeTexture();
	//���ø߹ⷴ����ͼ
	VOID SetSpecularCubeTexture();
	//���ø�����ͼ
	VOID SetSpecularTexture();*/

	//���º󻺳�
	//BOOL UpdateBackBuf(RECT* pRect);

//-----------------------------------------------------------------------------
// ģ������趨
//-----------------------------------------------------------------------------	
	//ͨ������λ�õĻ��ָ��������
// 	VOID GetRayUnderPoint(CSwapChain* pSwapChain, UINT x, UINT y, 
// 						  D3DXVECTOR3* pOrig, D3DXVECTOR3* pDir);
	VOID GetRayUnderPoint(CViewport* pViewport, UINT x, UINT y, 
						  D3DXVECTOR3* pOrig, D3DXVECTOR3* pDir);
	//�õ�RayHitģ��
	CModel* GetModelUnderRay(CSwapChain* pSwapChain, D3DXVECTOR3* orig, D3DXVECTOR3* dir, 
							 FLOAT* pU, FLOAT* pV,
							 FLOAT* pDist, D3DXVECTOR3* vColNormal);
	//�õ�����·���ģ��
	// CModel* GetModelUnderCursor(CSwapChain* pSwapChain, UINT x, UINT y);
	CModel* GetModelUnderCursor(CViewport* pViewport, UINT x, UINT y);
	//�õ�����·���MouseHintģ��
	CModel* GetMouseHintModelUnderCursor(CSwapChain* pSwapChain, UINT x, UINT y);
	//�⻬ģ�ͱ���Ķ��㷨��
	HRESULT SmoothModelNormal(CModel* pModel);
	//����ģ��Billboard
	VOID SetModelBillboard(CModel* pModel);
	//�����ײģ�ͺ���ײ����
	BOOL DetectCollision(D3DXVECTOR3* pOri, D3DXVECTOR3* pRay, 
						 CModel* pModel, D3DXVECTOR3* pNormal, 
						 FLOAT* distance);
	//���Walker�·��������ײģ�ͺͷ���
	BOOL DetectGround(D3DXVECTOR3* pOri, D3DXVECTOR3* pRay, 
					  DWORD* pModelHandle, D3DXVECTOR3* pNormal, 
					  FLOAT* distance);
	
	//��������
	HRESULT CreateReflectEffect(CModel* pRefModel,UINT width, UINT height, float power);
    //���÷�������
	VOID SetModelReflected(CModel* pRefModel, CModel* pRefedModel,BOOL checkFlag);

	// ��ȡ����������
	CSceneNode* GetRootSceneNode();

//-----------------------------------------------------------------------------
// ��������趨
//-----------------------------------------------------------------------------	
	//HRESULT CreateTransparentTexture(CTexture* pTex);
    //��ò���ͶӰ����
	D3DXMATRIXA16 GetMatriceProjectionTexture();
	//������ʵ�SubMesh�б�
	void ConstructShaderSubMeshList();
	//��ʹ��pMtrl���ʵ�SubMesh������pMtrl->subMeshList��
	void PushSubMeshToShader(CMaterial* pMtrl);

	BOOL IsMaterialExist(const std::string &sMatName);
	CMaterial* CreateMaterial(const std::string &sMatName);
	VOID DeleteMaterialIfNoUse(CMaterial *pMaterial);

	UINT getMaterialCount();
	VOID getMaterialList(vector <CMaterial*> &vMaterial);
	CMaterial* GetMaterialByName(const std::string &sName);
	CMaterial* GetMaterialByIndex(INT index);
	INT getMaterialIndex(CMaterial* pMaterial);
	// add a meterial to list
	CMaterial* AddMaterial(VGS_MATERIAL_TYPE type, CMaterial *pMaterial);

	// ��Դ������
    std::string RenameMaterialIfExist(const std::string &sName);
	std::string RenameCameraIfExist(const std::string &sName);
	std::string RenameLightIfExist(const std::string &sName);
	std::string RenameModelIfExist(const std::string &sName);
	

//-----------------------------------------------------------------------------
// ��ͼ����趨
//-----------------------------------------------------------------------------	
	//��Data���崴����ͼ,����Ĭ�ϵĳߴ�
	//HRESULT CreateTexFromBuffer(CTexture* pTex, INT mipLevel);
	//��Data���崴����ͼ��ָ���ߴ�
	//HRESULT	CreateTexFromBuffer(CTexture* pTex, UINT width, UINT height, INT mipLevel);
	//��������ͼ
	//HRESULT	CreateEmptyTex(CTexture* pTex, INT mipLevel);
	CTexture*	CreateEmptyTex(const std::string &sName, D3DFORMAT format, INT width, INT height, INT mipLevel);
	//��RGB���崴����ͼ
	// HRESULT	CreateTexFromRGBBuffer(CTexture* pTex, INT mipLevel);
	//���ļ�������ͼ
	// HRESULT	CreateTexFromFile(CHAR* strFileName, CTexture* pTex, INT mipLevel);
	// ���ļ�����һ��Texture����
	CTexture* CreateTexFromFile(const CHAR* strFileName, BOOL bOpacityOnly, const CHAR *strTextureName, INT mipLevel);

	// HRESULT	CreateOpacityTexFromFile(CHAR* strFileName, CTexture* pTex, INT mipLevel);
	
	//���ڴ洴����̬��ͼ
	//HRESULT CreateDyTexFromFileInMemory(CTexture* pTex);

	//VOID CreateDyTexFrameImageBuffer();
    //����cubemap
	//VOID CreateCubeMapBuffer();

	//������ͼ������
	HRESULT SetTexBrightness(CTexture* pTex, UINT brightness);
	//�õ�CubeMap����ͼMaxtrix
	D3DXMATRIX GetCubeMapViewMatrix( DWORD dwFace );

	// texture���
	UINT getTextureCount();
	CTexture* getTextureByIndex(INT index);
	CTexture* getTextureByName(const std::string &sName);

	// ����������ȡͼ��
	UINT getImageCount();
	CImage* getImageByIndex(INT index);



	//����SwapChain
	CSwapChain*  CreateSwapChain(HWND hWnd, D3DMULTISAMPLE_TYPE multiSampleType);

	// ׼����Ⱦ 
	VOID InitRenderSatusFromSwapchain(CSwapChain* pSwapChain);

	// ���������ǵ���
	HRESULT InvalidateScene();
	HRESULT RestoreScene();

public://GUI������
	CGUIManager          m_GUIManager;   // GUI���������
	//��ȡGUI����������--added by linxq 09-4-9
	inline CGUIManager& GetGuiManagerRef() { return m_GUIManager;}

public:
	// ���ƶ������
	IController *CreateWalker(const WalkerInfo &walkinfo);
	IController *CreateFlyer(const FlyerInfo &flyerinfo);
	IController *CreateEditor(const EditorInfo &editorinfo);
	UINT GetControlCount();
	UINT GetWalkerCount();
	UINT GetFlyerCount();
	UINT GetEditorCount();
	VOID AddWalker(IController *pWalker);
	VOID AddEditor(IController *pEditor);
	VOID AddFlyer(IController *pFlyer);
    IController* GetControl(UINT idx);
    IController* GetWalker(UINT idx);
    IController* GetFlyer(UINT idx);
    IController* GetEditor(UINT idx);
    IController* GetWalker(const string sName);
    IController* GetFlyer(const string sName);
    IController* GetEditor(const string sName);
	INT GetWalkerIndex(IController *pWalker);
	INT GetEditorIndex(IController *pEditor);
	INT GetFlyerIndex(IController *pFlyer);
	void DeleteWalker(const string &sName);
	void DeleteEditor(const string &sName);
	void DeleteFlyer(const string &sName);
	void DeleteWalker(DWORD handle);
	void DeleteEditor(DWORD handle);
	void DeleteFlyer(DWORD handle);	
	
	// ���á���ȡ��ǰ������
	void SetCurrentController(IController* pController);
	IController *GetCurrentController();

	// ���һ����̬cubemap�����б�
	VOID addDynamicCubemap(CCubeMap* pCubemap);
	// ���һ����̬bump map�����б�
	VOID addBumpmap(CBumpMap* pBumpmap);

public:
	// �ļ���� 
	// ����s3d����
#if(!defined(_VGS_LIB)&&!defined(_VGS_ATL))
	DWORD ImportS3DFile(const std::string &strS3DFileName, HWND hWnd);
	DWORD SaveV3DFile(const std::string &sFile, HWND hWnd);
	DWORD OpenV3DFile(const std::string &sFileName, HWND hWnd);
#endif

public:
	CScene(LPDIRECT3D9 pD3D, LPDIRECT3DDEVICE9 pD3DDevice);
	~CScene();

private:

	// ���ƶ����б�
	IController *m_pCurController;   // ��ǰ������
	std::vector<IController*> m_vWalker;
	std::vector<IController*> m_vEditor;
	std::vector<IController*> m_vFlyer;
	void DeleteAllController();

	// ˮ�����
	vector<CRealWater*>		m_vRealWater;

	vector<CCubeMap*>       m_vpDynamicCubemap;  // ��̬cubemap

	//��Ⱦǰ������
	VOID StartRender();

	// ��Ⱦ�������
	VOID RenderadditionalTexture();
	
	//��Ⱦbackdrop
	VOID RenderBackDrop();
	
	//��Ⱦģ��
	VOID RenderModels();
	
	//������֡����Ⱦ
	VOID EndRender();
	
	//��Ⱦ�����Ķ���
	VOID RenderOthers();

	VOID ClearAllAdditionalSurface();  // ��ճ��������еĸ�����Ⱦ����

	VOID UpdateAllAdditionalSurface();	//�������и�����Ⱦ����

	//��ȾSubMesh��ʵʱ��ͼ����
	VOID RenderSubMeshToRTFTexture(CRTFMap* pRTFMap, VGS_RenderSubmesh* pSubMesh/*, CMaterial* pMtrl*/);
	//��Ⱦһ��ģ�͵�ʵʱ��ͼ����----linxq-��ǰ���ڲ���
	void RenderMeshToRTFTexture(CRTFMap* pRTFMap);
	//��ȾSubMesh����͹ˮ������ͼ����
	VOID RenderSubMeshToBumpWaveCompositeTexture(CBumpMap* pBumpMap, VGS_RenderSubmesh* pSubMesh, CMaterial* pMtrl);
	VOID RenderModelToDynamicCubeMapSurface(CModel* pModel, VGS_RenderSubmesh* pSubMesh, CCubeMap* pCubeMap);

	//��Ⱦ�����е���ʵˮ��
	VOID RenderRealWater();

	//��Ⱦʹ�õ�ǰ���ʵ�ģ��
	VOID RenderModelsByMtrl(/*CMaterial* pMtrl*/);

	//��Ⱦһ���ƶ�����
	VOID RenderOneModel(CModel* pModel);

	//������ж�̬CubeMap�ı���
	VOID ClearDynamicCubeMapSurface();
	//�л�CubeMap�ı���
	VOID SwtichDynamicCubeMapSurface();

private:

	// ��submesh��Ⱦ��bump texture
	VOID RenderSubmeshToBumptexture(CBumpMap* pBumpmap, const vector<VGS_RenderSubmesh> &vSubmeshList);

public:
// 	DWORD LoadSceneFromBuffer(DWORD version, const BYTE* pBuffer, HWND hWnd);
	// 	DWORD LoadImageDataFromBuffer(DWORD version, const BYTE* pBuffer, HWND hWnd);

	D3DXMATRIX GetTextureProjMatrix();  //����RenderToTexture�Ķ�����Ⱦ��ͼ��ͶӰ����

	VOID   SetRoot(CRoot *pRoot);


public:
	CA3DManager m_A3DManager;  // ���������ļ�������


};

#endif