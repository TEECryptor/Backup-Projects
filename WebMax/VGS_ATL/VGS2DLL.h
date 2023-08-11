//------------------------------------------------------------------------
//VGS2DLL.h CVGS2DLL��ͷ�ļ�
//������VGS2�汾����API������
//����: lixin
//����: 2006 -3 -31
//------------------------------------------------------------------------

#ifndef _VGS2DLL_H
#define _VGS2DLL_H

#include <stdio.h>
#include <windows.h>

#include "VGSATLHeader.h"
#include "..\\VGSCoreV3\\vgscore.h"
//#include "VGSCore\\vgscore.h"

// #include "VGSSingelton.h"
#include "CInput.h"

using namespace std;
// 
// //-------------------------------------------------------------------------------------------
// //***����ö�����Ͷ���
// //-------------------------------------------------------------------------------------------
// enum CONTROLSTATUS
// {
// 	WALK,			//Ԥ��״̬�����������г��õķ���
// 	EDIT,			//�༭״̬, ���չʾ�г��õķ���
// 	FLY,			//����״̬��GISģʽ
// 	NONE			//�ǽ���״̬
// };
// 
// //D3D FORMAT
// enum TEXFORMAT
// {
// 	RGBA5650,
// 	RGBA5551,
// 	RGBA4444,
// 	RGBA8880,
// 	RGBA8888
// };
// 
// //����ģʽ
// enum ALIGNMODE
// {
// 	ALIGN_NONE,
// 	ALIGN_LEFT,
// 	ALIGN_RIGHT,
// 	ALIGN_BOTTOM,
// 	ALIGN_TOP,
// 	ALIGN_LEFT_TOP,
// 	ALIGN_MIDDLE_TOP,
// 	ALIGN_RIGHT_TOP,
// 	ALIGN_LEFT_BOTTOM,
// 	ALIGN_MIDDLE_BOTTOM,
// 	ALIGN_RIGHT_BOTTOM,
// 	ALIGN_CENTER
// };
// 
// //-------------------------------------------------------------------------------------------
// //***ʸ������
// //-------------------------------------------------------------------------------------------
// typedef struct VECTOR2
// {
// 	FLOAT x;
// 	FLOAT y;
// }VECTOR2;
// 
// typedef struct VECTOR3
// {
// 	FLOAT x;
// 	FLOAT y;
// 	FLOAT z;
// }VECTOR3;
// 
// 
// //-------------------------------------------------------------------------------------------
// //***��ɫ����
// //-------------------------------------------------------------------------------------------
// typedef struct RGB
// {
// 	BYTE red;
// 	BYTE green;
// 	BYTE blue;
// }RGB;
// 
// typedef struct RGBA
// {
// 	BYTE red;
// 	BYTE green;
// 	BYTE blue;
// 	BYTE alpha;
// }RGBA;
// 
// //�����Χ����Ϣ�ṹ
// typedef struct BoundSphere
// {
// 	VECTOR3 vCenter; //���ĵ�
// 	FLOAT   fRadius; //�뾶
// }BoundSphere;
// 
// //�����Χ�����ݽṹ
// typedef struct BoundBox
// {
// 	VECTOR3 vMin; //���½�
// 	VECTOR3 vMax; //���Ͻ�
// }BoundBox;
// 
// //������������Ե����ݽṹ
// typedef struct CameraInfo
// {
// 	VECTOR3 from;			//����������
// 	VECTOR3 at;				//��������յ�	
// 	FLOAT	roll;			//�������roll�ĽǶ�
// 	FLOAT	nearClip;		//�������nearClip
// 	FLOAT   farClip;		//�������farClip
// 	FLOAT	fov;			//�������fov
// }CameraInfo;
// 
// 
// //����ƹ����Ե����ݽṹ
// typedef struct LightInfo
// {
//     UINT			Type;
//     RGB				Diffuse;
//     RGB				Ambient;
//     FLOAT			Multiply;
//     VECTOR3			Position;
//     VECTOR3			Direction;
//     FLOAT           Range;
//     FLOAT           Falloff;
//     FLOAT           Attenuation0;
//     FLOAT           Attenuation1;
//     FLOAT           Attenuation2;
//     FLOAT           Theta;
//     FLOAT           Phi;
// }LightInfo;
// 
// //����������Ե����ݽṹ
// typedef struct MtrlInfo
// {
// 	std::string			Name;		//������
// 	RGB				Ambient;		//����ɫ
//     RGB				Diffuse;		//��ɫ
// 	RGB				Specular;		//�߹���ɫ
// 	FLOAT			SpecularLevel;	//�߹�ǿ��
// 	FLOAT			SpecularPower;	//�������
// 	RGB				Emissive;		//�Է�����ɫ
// 	INT				Opacity;		//͸����
// 	BOOL			bTwoSide;		//�Ƿ�˫��
// 	BOOL			bAlphaTest;		//�Ƿ����Alpha����
// }MtrlInfo;

class CVGS2DLL //: public Singleton <CVGS2DLL>
{

public:

	CVGSCore         m_VGSCore;  // vgs��Ⱦ��
	//-------------------------------------------------------------------------------------------
	//***ϵͳ����
	//-------------------------------------------------------------------------------------------
	DWORD				m_vgsScene;					// ����
	DWORD               m_RenderTarget;             // ��ǰ��ȾĿ��
	DWORD               m_Viewport;                 // ��ǰ���ӿ�
	vector<DWORD>	sceneResource;				//���еĳ���(1��N3D�ļ���Ӧһ������)��Դ����
	FLOAT				fAxisScale;					//ȱʡ������ֵ
	CONTROLTYPE		controlStatus;				//��ǰ�û�����״ֵ̬

	BOOL                m_bRender;  // �Ƿ���Ⱦ����

	//-------------------------------------------------------------------------------------------
	//ʱ����ر���
	//-------------------------------------------------------------------------------------------
	UINT				lastFrameTimer;						//��һ֡��Ⱦ������ʱ��
	BOOL				bShowDebugInfo;						//�Ƿ���ʾ������Ϣ

	//-------------------------------------------------------------------------------------------
	//��ײ������ر���
	//-------------------------------------------------------------------------------------------
	BOOL				bCollision;							//��ײ�Ƿ���Ч
	BOOL				bIsCollision;						//��ǰ�Ƿ�����ײ����
	BOOL				bWeight;							//�����Ƿ���Ч
	FLOAT				colDistance;						//��ײ����

// 	CModel*				curColModel;						//��ǰ������ײ��ģ��
// 	CModel*				curGroundModel;						//��ǰ����ģ��
// 	CModel*             preGroundModel;                     //֮ǰ�ĵ���ģ��

	//-------------------------------------------------------------------------------------------
	//WALKģʽ����
	//-------------------------------------------------------------------------------------------
// 	CWalker				walker;								//walker���������
// 	D3DXVECTOR3			walker_realMoveVec;					//walker��ʵ���ƶ��ٶ�
// 	D3DXVECTOR3			walker_realRotateVec;				//walker��ʵ����ת�ٶ�
// 	FLOAT				moveStep;
// 	FLOAT				rosDis;


	//-------------------------------------------------------------------------------------------
	//EDITģʽ����
	//-------------------------------------------------------------------------------------------
// 	CEditor				editor;								//editor���������
// 	INT					mouseX;
// 	INT					mouseY;
// 
// 	//-------------------------------------------------------------------------------------------
// 	//FLYģʽ����
// 	//-------------------------------------------------------------------------------------------
// 	CFlyer				flyer;								//���ж���
// 
// 	//-------------------------------------------------------------------------------------------
// 	//�ⲿ�������֡�ƶ���Ŀ���ķ�����ر���
// 	//-------------------------------------------------------------------------------------------
// 	BOOL				bFrameMoveCamera;					//��Ⱦ֡���Ƿ��ƶ����
// 	D3DXVECTOR3			FrameMoveCamera_From;				//֡�ƶ������Ŀ��From
// 	D3DXVECTOR3			FrameMoveCamera_At;					//֡�ƶ������Ŀ��At
// 	INT					FrameMoveCamera_FrameCnt;			//�ƶ��������֡��
// 	INT					FrameMoveCamera_FrameIndex;			//�ƶ�����ĵ�ǰ֡
// 	INT					FrameMoveCamera_AccMode;			//����ģʽ
// 	D3DXVECTOR3			FrameMoveCamera_FromAcc;			//From����ٶ�
// 	D3DXVECTOR3			FrameMoveCamera_FromVel;			//From���ٶ�
// 	D3DXVECTOR3			FrameMoveCamera_AtAcc;				//At����ٶ�
// 	D3DXVECTOR3			FrameMoveCamera_AtVel;				//At���ٶ�
// 
// 	//-------------------------------------------------------------------------
// 	//������Ƶ�״̬����
// 	//-------------------------------------------------------------------------
// 	BOOL				bCanZoom;							//Home��End������
// 	BOOL				bCanMove;							//ASWD�ͷ��������
// 	BOOL				bCanLift;							//PageUp��PageDown����
// 	BOOL				bCanRos;							//QERF�������ת����
// 
// 	FLOAT				fZoomSpeed;							//����PlayerZoomIn��PlayerZoomOut�����������ٶ�
public:
	//-------------------------------------------------------------------------------------------
	//***�����ʼ����صķ���
	//-------------------------------------------------------------------------------------------
	// ��ʼ������
	BOOL SunInitVGS(HWND hWnd);

	VOID SunRelease();

	// ��ʼ��ȱʡ����Ⱦ����
	BOOL InitDefaultRenderForm(HWND hWnd);

	// ���ȱʡʹ�õ��ӿ�
	DWORD SunGetDefaultViewport();

	// ��ʼ��GUI״̬
	VOID SunInitGUIStatus();

	//����VGS����
	DWORD SunCreateScene(/*HWND hWnd, RECT* rect*/);
	//���VGS����
	VOID SunDeleteScene();
	//Reset VGS����
	VOID SunResetScene();
	//���ó����ı���ɫ
	VOID SunSetBackground(VGSRGB* color);
	//����Logo
	BOOL CreateLogo();
	//ɾ��Logo
	VOID DeleteLogo();
	//Invalidate Scene
	VOID InvalidateScene();
	//Restore Scene
	VOID RestoreScene();
	//���ÿ��ܳݵļ���
	VOID SetAntiAliasLevel(INT level);

	//--------------------------------------------------------------------------------------------
	//������Դ������صķ�����������Դ������LoadN3D�ļ�ʱ�Զ�������һ��N3D�ļ���Ӧһ��������Դ����
	//--------------------------------------------------------------------------------------------
	//����������Դ����
// 	CSceneResource* CreateSceneResource(std::string sceneResourceName, D3DXMATRIX* pTransform);
// 	//ɾ��ָ��������Դ����Ҳɾ������Դ����
// 	void	DeleteSceneResource(CSceneResource* pSceneResource);
// 	//�ҵ�ָ���ĳ�����Դ����
// 	CSceneResource* GetSceneResource(std::string sceneResourceName);
// 
// 	//��¡ָ���ĳ���
// 	CSceneResource* CloneScene(std::string destSceneName, 
// 								std::string srcSceneName, 
// 								std::string parentSceneName, 
// 								D3DXMATRIX* pTransformMat);
// 

	//-------------------------------------------------------------------------------------------
	//***ģ����صķ���
	//-------------------------------------------------------------------------------------------
	//�õ�������ģ�͵�����
	UINT SunGetModelCount();
	//����ģ�͵ľ���õ�ģ�͵�����
	VOID SunGetModelNameByHandle(DWORD modelHandle, CHAR* strModelName);
	//����ģ�͵����Ƶõ�ģ�͵ľ��
	DWORD SunGetModelHandleByName(const CHAR* strModelName);
	//����ģ�͵ı�ŵõ�ģ�͵ľ��
	DWORD SunGetModelHandleByIndex(UINT index);
	//����ģ�͵Ŀɼ���
	VOID SunSetModelVisibility(DWORD modelHandle, BOOL visible);
	//�õ�ģ�͵Ŀɼ���
	BOOL SunGetModelVisibility(DWORD modelHandle);
	//ɾ�������е�ģ��
	VOID SunDeleteModelByHandle(DWORD modelHandle);
	
	/** �ƶ�ģ�͵�λ��
	@param in modelHandle   : ������
	@param in pVec          ���ƶ����������
	@param in flag          ���ƶ��Ĳο�����ϵ
							   0 - ����������ϵƽ��
							   1 - ����������ϵƽ��
	*/
	VOID SunMoveModel(DWORD modelHandle, VECTOR3* pVec, UINT flag);

	//�ƶ�ָ����ģ�͵�ĳ��λ��
	//acctype 0-�����˶���1-�ȼ����˶�, 2-�ȼ����˶�
	VOID MoveModelTo(DWORD modelHandle, VECTOR3* pVec, INT frameCnt, INT accType);

	//�õ�ģ�͵�λ��
	VOID SunGetModelPosition(DWORD modelHandle, VECTOR3* pVec);
	//����ģ�͵�λ��
	VOID SunSetModelPosition(DWORD modelHandle, VECTOR3* pVec);
	//��תģ��, flag = 0-> �ƴ�������Privot������������ϵƽ�е�����ϵ��ת, flag =1-> ��������ϵ, flag =2->������ϵ
	VOID SunRotateModel(DWORD modelHandle, VECTOR3* pVec, UINT flag);
	//ģ����ָ��������ת
	VOID SunRotateModelByAxis(DWORD modelHandle, VECTOR3* pPos, VECTOR3* pVec, FLOAT fAngle);
	//�õ�ģ�͵�ת��
	VOID SunGetModelRotation(DWORD modelHandle, VECTOR3* pRot);
	//����ģ�͵�ת��
	VOID SunSetModelRotation(DWORD modelHandle, VECTOR3* pRot);
	//����ģ��, flag = 0-> GCS , flag =1->LCS
	VOID SunScaleModel(DWORD modelHandle, VECTOR3* pScale, UINT flag);
	//�Թ̶���Ϊ��������ģ��
	VOID SunScaleModelByPoint(DWORD modelHandle, VECTOR3* pPos, VECTOR3* pScale);
	//�õ�ģ�͵�����
	VOID SunGetModelScale(DWORD modelHandle, VECTOR3* pScale);
	//����ģ������
	VOID SunSetModelScale(DWORD modelHandle, VECTOR3* pScale);
	//�õ�����·���ģ�;��
	DWORD SunGetModelUnderLoc(UINT x, UINT y);
	//�õ�����·����������Ӧ��ģ�;��
	DWORD SunGetMouseHintModelUnderLoc(UINT x, UINT y);
	//����ģ�͵�����ģ��
	VOID SunSetModelReference(DWORD curModelHandle, DWORD refModelHandle);
	//���¼���ģ�͵ķ��ߣ���ģ�Ϳ������ǳ��⻬Smooth,�������ϴ�
	VOID SunSmoothModelNormal(DWORD modelHandle); 
	//����ģ������Billboard״̬
	VOID SunSetBillBoardStatus(DWORD modelHandle, BOOL status);
	//����ģ������Billboard��Up����
	VOID SunSetBillboardUp(DWORD modelHandle, VECTOR3* pUp);
	//���ó�����ײ����Ч״̬
	VOID SunEnableCollision(BOOL status);
	//������ײ��������Ч����, Ĭ��Ϊ1.0
	VOID SunSetColDistance(FLOAT distance);
	//����ģ�͵���ײ
	VOID SunSetCollisionModel(DWORD modelHandle, BOOL status);
	//����������Ч״̬
	VOID SunEnableWeight(BOOL status);
	//����ģ��Ϊ����ģ��
	VOID SunSetGroundModel(DWORD modelHandle, BOOL status);
	//ת��ģ��Diffuse��ͼ��UV
	VOID SunMoveModelUV(DWORD modelHandle, FLOAT u, FLOAT v);
	//����ģ���Ƿ�ʹ���Զ�ת��UV
	VOID SunSetDynamicUVStatus(DWORD modelHandle, BOOL status);
	//����ģ�͵��Զ�ת��UV�Ĳ���
	VOID SunSetDynamicUVProp(DWORD modelHandle, FLOAT UStep, FLOAT VStep, FLOAT speed);
	//����ģ���Ƿ���Ե��
	VOID SunSetModelClickable(DWORD modelHandle, BOOL status);

	//����ģ�͵�opacity
	VOID SunSetModelOpacity(DWORD modelHandle, UINT opacity);
	//����ģ�͵Ķ�����ɫ
	VOID SunSetModelColor(DWORD modelHandle, BYTE red, BYTE green, BYTE blue, BYTE alpha);

	//-------------------------------------------------------------------------------------------
	//***������صķ���
	//-------------------------------------------------------------------------------------------
	//�õ������в��ʵ�����
	UINT SunGetMaterialCount();
	//���ݲ��ʵľ���õ����ʵ�����
	VOID SunGetMaterialNameByHandle(DWORD mtrlHandle, CHAR* strMtrlName);
	//���ݲ��ʵ����Ƶõ����ʵľ��
	DWORD SunGetMaterialHandleByName(const CHAR* strMtrlName);
	//���ݲ��ʵı�ŵõ����ʵľ��
	DWORD SunGetMaterialHandleByIndex(UINT index);
	//�õ����ʵ�Diffuse��ͼ
// 	DWORD SunGetDifTexture(DWORD mtrlHandle);
// 	//���ò��ʵ�Diffuse��ͼ
// 	VOID SunSetDifTexture(DWORD mtrlHandle, DWORD texHandle);
// 
// 	//�õ����ʵ�Opacity��ͼ
// 	DWORD SunGetOpacityTexture(DWORD mtrlHandle);
// 	//���ò��ʵ�Opacity��ͼ
// 	VOID SunSetOpacityTexture(DWORD mtrlHandle, DWORD texHandle);
// 	//�õ����ʵ�Lightmap��ͼ
// 	DWORD SunGetLightTexture(DWORD mtrlHandle);
// 	//���ò��ʵ�Lightmap��ͼ
// 	VOID SunSetLightTexture(DWORD mtrlHandle, DWORD texHandle);
// 	//�õ����ʵ�Reflection������ͼ
// 	DWORD SunGetRefTexture(DWORD mtrlHandle);
// 	//���ò��ʵ�Reflection������ͼ
// 	VOID	SunSetRefTexture(DWORD mtrlHandle, DWORD texHandle);
// 	//���ò��ʵķ�����ͼ�ĳ��̶ֳ�
// 	VOID SunSetRefLevel(DWORD mtrlHandle, UINT level);
	//��������
	DWORD SunCreateEmptyMtrl(CHAR* strMtrlName);
	//ɾ������
	VOID SunDeleteMtrl(DWORD mtrlHandle);
	//ͨ��ģ�͵õ�����
	DWORD SunGetMtrlByModel(DWORD modelHandle);
	//����ĳ��ģ�͵Ĳ���
	VOID SunSetModelMtrl(DWORD modelHandle, INT subMeshID, DWORD mtrlHandle);
	//���ò��ʵ�opacity
	VOID SetMtrlOpacity(DWORD mtrlHandle, UINT opacity);
	//���ò��ʵ���ɫ
	VOID SunSetMtrlColor(DWORD mtrlHandle, INT red, INT green, INT blue, INT alpha);

	//����ģ��ΪSkyBoxģ��
	VOID SunSetSkyBox(DWORD modelHandle, BOOL status);
	//�õ����ʵ�CubeMap
	DWORD SunGetCubeMap(DWORD mtrlHandle);
	//���ò��ʵ�CubeMap
	VOID SunSetMtrlCubeMap(DWORD mtrllHandle, DWORD cubeMapHandle, FLOAT power);
	//���ò��ʵĸ߹�CubeMap
	VOID SetMtrlSpecularCubeMap(DWORD mtrlHandle, DWORD cubeMapHandle);
	//ͨ��CubeMap�����Ƶõ�CubeMap
	DWORD GetCubeMapHandleByName(CHAR* cubeMapName);
	//ͨ��CubeMap��index�õ�CubeMap
	DWORD GetCubeMapHandleByIndex(INT index);
	
	//��ò�����Ϣ
	VOID SunGetMtrlInfo(DWORD mtrlHandle, MtrlInfo* pMtrlInfo);
	//���ò�����Ϣ
	VOID SunSetMtrlInfo(DWORD mtrlhandle, MtrlInfo* pMtrlInfo);


	//����Bump��ͼ
	DWORD CreateBumpTexture(CHAR* name, VGS_BumpType type, INT width, INT height, INT xSize, INT ySize, 
							FLOAT radius, FLOAT speed, FLOAT centerX, FLOAT centerY);
	//ɾ��BumpWater
	VOID DeleteBumpTexture(DWORD bumpWaterHandle);
	//ͨ��Bumpwater�����ֻ��ָ��
	DWORD	GetBumpTextureHandleByName(CHAR* strName);

	//���ò��ʵ�ĳ����ͼ
	VOID SetMtrlTexture(CHAR* mtrlName, INT layerIndex, VGS_TextureType type, CHAR* textureName);

	//-------------------------------------------------------------------------------------------
	//***��ͼ��صķ���
	//-------------------------------------------------------------------------------------------
	//�õ���������ͼ������
	UINT SunGetTextureCount();
	//������ͼ�ľ���õ���ͼ������
	VOID SunGetTextureNameByHandle(DWORD texHandle, CHAR* strTexName);
	//������ͼ�����Ƶõ���ͼ�ľ��
	DWORD SunGetTextureHandleByName(CHAR* strTexName);
	//������ͼ�ı�ŵõ���ͼ�ľ��
	DWORD SunGetTextureHandleByIndex(UINT index);

	//Add by Baojiahui 2008/12/8-----------------------------------------------------------------
	//-------------------------------------------------------------------------------------------

	//�ӱ����ļ�������ͼ
	DWORD SunCreateTexture(CHAR* texName, CHAR* strImageFile, TEXFORMAT format);
	//��Image���󴴽���ͼ
	DWORD SunCreateTextureFromImage(CHAR* texName, CHAR* strImageName, TEXFORMAT format, INT mipLevel);
	// ��Image buffer����ʹ�ø�Image������texture
	VOID SunFillTexturesFromBuffer(UINT len, CHAR* pBuffer, INT imgIndex);
	//ɾ����ͼ
	VOID SunDeleteTexture(DWORD texHandle);
	//������ͼ��FORMAT
	VOID SunSetTextureFormat(DWORD texHandle, TEXFORMAT format);
	//�ж���ͼ�Ƿ񱻳������õ�
	BOOL IsTextureInScene(DWORD pTex);
	//�ж�CubeMap�����Ƿ񱻳����õ�
	BOOL IsCubeMapInScene(DWORD pCubeMap);
	//�ж϶�̬��ͼ�����Ƿ񱻳����õ�
	BOOL IsDyTexInScene(DWORD pDyTex);
	//�жϵ�Ӱ��ͼ�Ƿ񱻳����õ�
	BOOL IsMovieTexInScene(DWORD pMovieTex);
	//ɾ����̬��ͼ
	VOID DeleteDynamicTexture(DWORD dynamicTexHandle);
	//-------------------------------------------------------------------------------------------
	//***�������صķ���
	//-------------------------------------------------------------------------------------------
	//�õ������������������
	UINT SunGetCameraCount();
	//��������ľ���õ����������
	VOID SunGetCameraNameByHandle(DWORD camHandle, CHAR* strCameraName);
	//������������Ƶõ�����ľ��
	DWORD SunGetCameraHandleByName(CHAR* strCameraName);
	//����������ı�ŵõ�����ľ��
	DWORD SunGetCameraHandleByIndex(UINT index);
	//���õ�ǰ�����
	VOID SunSetCurrentCamera(DWORD camHandle);
	//�õ���ǰ��������
	DWORD SunGetCurrentCamera();

	/** ���Ŀ��㲻�������������ǰ�����������
	@param in sceneHandle : ����ָ��
	@param in camHandle : ���ָ��
	@param dollyValue  in   : ��������ľ��룬> 0 ��ǰ�� < 1���
	*/
	VOID SunDollyCamera(DWORD handle, FLOAT value);

	//ƽ�������
	VOID SunMoveCamera(DWORD camHandle, VECTOR3* pVec, UINT coordEnum);
	//��ת�����
	VOID SunRotateCamera(DWORD camHandle, VECTOR3* pVec, UINT coordEnum);
	//��Ŀ���Ϊ���ģ���pAxisVecΪ�ᣬ��תFrom��fAngle����
	VOID SunRoundCamera(DWORD camHandle, VECTOR3* pAxisVec, FLOAT fAngle); 
	//���������
	VOID SunSetCameraLookAt(DWORD camHandle, DWORD modelHandle);
	
	//�������
	DWORD SunCreateCamera(const CHAR* name, D3DXVECTOR3* pFrom, D3DXVECTOR3* pAt);
	// ����һ��ȱʡ���
	DWORD SunCreatePredefinedCamera(DEFAULTCAMERA_TYPE type);

	//�������������ʼ��
	VOID SunSetCameraFrom(DWORD camHandle, VECTOR3* pPos);
	//�����������Ŀ���
	VOID SunSetCameraAt(DWORD camHandle, VECTOR3* pPos);
	VOID SunGetCameraAt(DWORD camHandle, VECTOR3* pPos);
	//�����������FOV
	VOID SunSetCameraFov(DWORD camHandle, FLOAT fov);
	//�����������nearClip
	VOID SunSetCameraNearClip(DWORD camHandle, FLOAT nearClip);
	//�����������farClip
	VOID SunSetCameraFarClip(DWORD camHandle, FLOAT farClip);
	//�����������Roll
	VOID SunSetCameraRoll(DWORD camHandle, FLOAT rollAngle);
	//һ�������������������
	VOID SunSetCameraInfo(DWORD camHandle, CameraInfo* pCamInfo);
	//����������ؼ�֡����
	VOID SunPlayCameraKeyFrame(DWORD camHandle);
	//��ͣ������ؼ�֡����
	VOID SunPauseCameraKeyFrame(DWORD camHandle);
	//����������ؼ�֡����
	VOID SunStopCameraKeyFrame(DWORD camHandle);
	//���������������ĳһ֡
	VOID SunGotoCameraFrame(DWORD camHandle, INT frame);

	//ѭ�����÷���, ��Ҫ��֡ѭ���е��á�
	//frameCntΪ����Ŀ������֡����accTypeΪ�˶������еļ��ٷ���
	VOID MoveCurrentCameraTo(D3DXVECTOR3* vFrom, D3DXVECTOR3* vAt, INT frameCnt, BOOL accType);
	//��Ⱦ֡�Զ����õ��ƶ�����ķ�����ÿ֡����һ��(�ڲ�����)
	VOID FrameMoveCurrentCamera();

	//-------------------------------------------------------------------------------------------
	//***�ؼ�֡������صķ���
	//-------------------------------------------------------------------------------------------
	//���Źؼ�֡����
	VOID PlayKeyframe();
	//��ͣ�ؼ�֡����
	VOID PauseKeyframe();
	//ֹͣ�ؼ�֡����
	VOID StopKeyframe();

	//�õ��ؼ�֡������֡��
	INT GetKeyframeCount();
	//����ĳһ֡
	VOID GotoFrame(INT index);
	//�õ���ǰ���ŵ�֡
	INT GetCurrentFrame();

	//���ùؼ�֡�������ŵ�֡��
	VOID SetKfrPlaySpeed(INT index);
	//��ùؼ�֡�������ŵ�֡��
	INT GetKfrPlaySpeed(); 
	//���ùؼ�֡�����Ĳ��Ŵ���
	VOID SetKfrRepeatCount(INT index);

	//-------------------------------------------------------------------------------------------
	//***�ƹ���صķ���
	//-------------------------------------------------------------------------------------------
	//�õ������еƹ������
	UINT SunGetLightCount();

	//���ݵƹ�����Ƶõ��ƹ�ľ��
	DWORD SunGetLightHandleByName(CHAR* strLightName);
	//���ݵƹ�ı�ŵõ��ƹ�ľ��
	DWORD SunGetLightHandleByIndex(UINT index);

	VOID	ResetLightIndex();
	//���ݵƹ�ľ���õ��ƹ������
	VOID SunGetLightNameByHandle(DWORD lightHandle, CHAR* strLightName);
	//�õ��ƹ�ı��
	INT SunGetLightIndexByHandle(DWORD lightHandle);

	//���õƹ���Ƿ���Ч
	VOID SunEnableLight(DWORD lightHandle, BOOL isEnalbe);
	//�ƶ��ƹ�
	VOID SunMoveLight(DWORD lightHandle, VECTOR3* pVec);
	//��ת�ƹ�
	VOID SunRotateLight(DWORD lightHandle, VECTOR3* pRos);
	//���õƹ��λ��
	VOID SunSetLightPosition(DWORD lightHandle, VECTOR3* pPos);
	//���õƹ�ķ���
	VOID SunSetLightDirection(DWORD lightHandle, VECTOR3* pDir);
	//�õ��ƹ������
	VOID SunGetLightInfo(DWORD lightHandle, LightInfo* pLightInfo);
	//���õƹ������
	VOID SunSetLightInfo(DWORD lightHandle, LightInfo* pLightInfo);
	/** ���õƹ��Ƿ���Ч
	*/
	VOID SunSetLightEnableStatus(DWORD lightHandle, BOOL bEnable);

	//�����Ƿ񲥷ŵƹ�Ĺؼ�֡����
	VOID SetLightKfrPlayStatus(DWORD lightHandle, BOOL bFlag);
	VOID SetAllLightKfrPlayStatus(BOOL bFlag);

	//���õƹ�ؼ�֡�����Ĳ��Ŵ���
	VOID SetLightKfrRepeatCnt(DWORD lightHandle, INT repeatCnt);
	VOID SetAllLightKfrRepeatCnt(INT repeatCnt);

	//-------------------------------------------------------------------------------------------
	//***������صķ���
	//-------------------------------------------------------------------------------------------
	//�½����ֶ���
	DWORD SunNewText(CHAR* strText);
	//ɾ�����ֶ���
	VOID SunDelText(DWORD textHandle);
	//�õ����������ֶ��������
	UINT SunGetTextCount();
	//ͨ�����ֶ���ı�ŵõ����ֶ���ľ��
	DWORD SunGetTextHandleByIndex(UINT index);
	//�������ֵ���ɫ
	VOID SunSetTextColor(DWORD textHandle, DWORD color);
	//�������ֶ����λ��
	VOID SunSetTextPosition(DWORD textHandle, UINT x, UINT y);
	//�������ֵ�����
	VOID SunSetTextString(DWORD textHandle, CHAR* strText);


	//-------------------------------------------------------------------------------------------
	//***Overlay��صķ���
	//-------------------------------------------------------------------------------------------
	DWORD CreateOverlayFromMemory(RECT* pRect, const CHAR* overlayName, const CHAR* overlayTexName, const BYTE* pData ,UINT dataSize);
	//�½� Overlay, nameΪOverlay�����ƣ� strOriImageFileΪԭͼ��strAlphaImageFileΪAlpha��ͼ, alignModeΪ����ģʽ�� pRectΪOverlay��Rect
	DWORD SunNewOverlay(CHAR* name, CHAR* strOriImageFile, CHAR* strAlphaImageFile, ALIGNMODE alignMode, RECT* pRect);
	//��Buffer���� Overlay
	DWORD SunNewOverlayFromBuf(CHAR* name, BYTE* pOriBuf, DWORD oriDataSize, BYTE* pAlphaBuf, DWORD alphaDataSize, ALIGNMODE alignMode, RECT* pRect);
	//��Image�����½�Overlay
	DWORD SunNewOverlayFromImage(CHAR* overlayName, CHAR* pOriImageName, CHAR* pAlphaImageName, ALIGNMODE alignMode, RECT* pRect);
	//ɾ�� Overlay
	VOID SunDelOverlay(DWORD overlayHandle);

	//ͨ�����Ƶõ�Ovelay�ľ��
	DWORD SunGetOverlayHandleByName(CHAR* strName);
	//ͨ����ŵõ�Overlay�ľ��
	DWORD SunGetOverlayHandleByIndex(INT index);
	//�õ�Overlay��index
	INT SunGetOverlayIndex(DWORD overlayHandle);
	//�õ�Overlay������
	VOID SunGetOverlayName(DWORD overlayHandle, CHAR* strName);
	//�õ� Overlay ���ļ���
	VOID SunGetOverlayImage(DWORD overlayHandle, CHAR* strImageFile);
	//���� Overlay ���ļ���
	VOID SunSetOverlayImage(DWORD overlayHandle, CHAR* strImageFile);
	//�õ� Overlay ��Rect
	VOID SunGetOverlayRect(DWORD overlayHandle, RECT* rect);
	//���� Overlay ��Rect
	VOID SunSetOverlayRect(DWORD overlayHandle, RECT* rect);
	//�õ� Overlay ��͸����
	UINT SunGetOverlayOpacity(DWORD overlayHandle); 
	//���� Overlay ��͸����
	VOID SunSetOverlayOpacity(DWORD overlayHandle, UINT opacity);
	//�õ� Overlay ����Ⱦģʽ
	VOID SunGetOverlayFormat(DWORD overlayHandle, UINT enumStatus);
	//���� Overlay ����Ⱦģʽ
	UINT SunSetOverlayFormat(DWORD overlayHandle);
	//�ƶ� Overlay
	VOID SunMoveOverlay(DWORD overlayHandle, INT x, INT y);
	//��ת Overlay
	VOID SunRotateOverlay(DWORD overlayHandle, FLOAT angle);
	//�õ� Overlay��ת�ĽǶ�
	FLOAT SunGetOverlayRotation(DWORD overlayHandle);
	//���� Overlay��ת�ĽǶ�
	VOID SunSetOverlayRotation(DWORD overlayHandle, FLOAT angle);
	//�õ� �������Overlay
	DWORD SunGetOverlayUnderLoc(INT x, INT y);
	//����Overlay�Ķ���ģʽ
	VOID SetOverlayAlignMode(DWORD overlayHandle, INT alignMode);
	
	//��Overlay�Ƶ���һ��[Overlay��ǰ���ϵ������Ļᱻǰ��ĵ�ס]
	VOID PushOverlayBack(DWORD overlayHandle);
	//��Overlay����һ��
	VOID PushOverlayFront(DWORD overlayHandle);

	//-------------------------------------------------------------------------------------------
	//***Button��صķ���, �½���Button������Ϊ��Դ������E3D�ļ��У����Զ�̬����
	//-------------------------------------------------------------------------------------------
	//�ӱ����ļ�����Button
	//name-Button������
	//mouseUpImageFile-����ɿ���Դͼ���ļ�
	//mouseUpAlphaImageFile-����ɿ���Alphaͼ���ļ�
	//mouseOverImageFile-������ϵ�Դͼ���ļ�
	//mouseOverAlphaImageFile-������ϵ�Alphaͼ���ļ�
	//mouseDownImageFile-��갴�µ�Դͼ���ļ�
	//mouseDownAlphaImageFile-��갴�µ�Alphaͼ���ļ�
	//alignMode-����ģʽ����ΪNONEʱpButtonRect->top��pButtonRect->top�Զ�ʧЧ
	//pButtonRect-��ť��Rect
	//callBackFunc-���°�ť�Ļص�����
	DWORD SunNewButton(CHAR* name, 
								 CHAR* mouseUpImageFile, CHAR* mouseUpAlphaImageFile,
								 CHAR* mouseOverImageFile, CHAR* mouseOverAlphaImageFile,
								 CHAR* mouseDownImageFile, CHAR* mouseDownAlphaImageFile,
								 ALIGNMODE alignMode,
								 RECT* pButtonRect,
								 CHAR* callBackFunc);

	//��Image���󴴽�Button
	//name-Button������
	//mouseUpImage-����ɿ���Դͼ�����
	//mouseUpImage_alpha-����ɿ���Alphaͼ�����
	//mouseOverImage-������ϵ�Դͼ�����
	//mouseOverImage_alpha-������ϵ�Alphaͼ�����
	//mouseDownImage-��갴�µ�Դͼ�����
	//mouseDownImage_alpha-��갴�µ�Alphaͼ�����
	//ALIGNMODE alignMode-����ģʽ
	//pButtonRect-Button��Rect��������alignģʽ����Ϊ��Ӧģʽʱ����Ӧ������С
	DWORD SunNewButtonFromImage(CHAR* name, 
										  CHAR* mouseUpImage, CHAR* mouseUpImage_alpha,
										  CHAR* mouseOverImage, CHAR* mouseOverImage_alpha,
										  CHAR* mouseDownImage, CHAR* mouseDownImage_alpha,
										  ALIGNMODE alignMode,
										  RECT* pButtonRect,
										  CHAR* callBackFunc);

	//ͨ�����Ƶõ�Button
	DWORD SunGetButtonByName(CHAR* name);
	//ͨ����ŵõ�Button
	DWORD SunGetButtonByIndex(UINT index);
	//ɾ����ť
	VOID SunDeleteButton(DWORD buttonHandle);

	//-------------------------------------------------------------------------------------------
	//***Image��صķ���, �½���Image������Ϊ��Դ������E3D�ļ��У����Զ�̬����
	//-------------------------------------------------------------------------------------------
	//�½�Image��Դ
	DWORD SunNewImage(CHAR* name, CHAR* fileName);
	//ͨ����ŵõ�Image����
	DWORD SunGetImageHandleByIndex(INT index);
	//ͨ��ͼ�����Ƶõ�Image����
	DWORD SunGetImageHandleByName(CHAR* name);
	//ͨ��ͼ����ɾ��Image����
	VOID SunDeleteImageByIndex(INT index);
	//ͨ��ͼ������ɾ��Image��Դ
	VOID SunDeleteImageByName(CHAR* name);


	//-------------------------------------------------------------------------------------------
	//***����ͼ��صķ���
	//-------------------------------------------------------------------------------------------
	//ɾ��Navigater
	VOID SunDeleteNavigater(DWORD navigterHandle);
	//ͨ��Navigater�����Ƶõ����
	DWORD SunGetNavigaterByName(CHAR* name);
	
	//���ӵ���ָ�����
	//navigaterHandle - ����ͼ����ľ��
	//navPointerName - ����ͼָ����������
	//pointerType - ����ͼָ������ͣ�0Ϊ���������1Ϊģ�͵���
	//pointerHandle - ����ͼָ��ľ�����������ͼָ������Ϊ�������Ϊ������������Ϊģ�;��
	//oriPointerFile - ����ͼָ��ͼƬ
	//alphaPointerFile - ����ͼָ��alphaͼƬ
	//pointerWidth, pointerHeight - ����ͼָ��ĳߴ�
	//���ص���ͼָ�����ľ��
	DWORD AddNavigaterPointer(DWORD navigaterHandle, 
										CHAR* navPointerName, 
										INT	  pointerType,	//camera or model 
										DWORD pointerHandle, //camera or model handle
										CHAR* oriPointerImageName, 
										CHAR* alphaPointerImageName, 
										INT pointerWidth, 
										INT pointerHeight);

	//-------------------------------------------------------------------------------------------
	//***����ͼ��صķ���
	//-------------------------------------------------------------------------------------------
	//��������ͼ����
	//sectionMapName-����ͼ��������
	//sliceModelName-��Ƭģ��
	//leftTopX, leftTopY, leftTopZ �������Ͻ�����
	//rightTopX, rightTopY, rightTopZ �������Ͻ�����
	//leftBottomX, leftBottomY, leftBottom �������½�����
	//rightBottomX, rightBottomY, rightBottomZ �������½�����
	//image��������
	DWORD	CreateSectionMap(CHAR* sectioMapName, 
							INT width, INT height,
							DWORD sliceModelHandle, 
							 FLOAT leftTopX, FLOAT leftTopY, FLOAT leftTopZ, 
							 FLOAT rightTopX, FLOAT rightTopY, FLOAT rightTopZ, 
							 FLOAT leftBottomX, FLOAT leftBottomY, FLOAT leftBottomZ, 
							 FLOAT rightBottomX, FLOAT rightBottomY, FLOAT rightBottomZ, 
							 DWORD imageHandle);

	//ִ����Ƭ
	VOID	ExecuteSlice(DWORD sectionMapHandle);

	//ͨ��SectionMap�����Ƶõ����
	DWORD	GetSectionMapHandleByName(CHAR* sectionMapName);

	//-------------------------------------------------------------------------------------------
	//***������Ⱦ��صķ���
	//-------------------------------------------------------------------------------------------
	//��Ⱦ��������
	VOID SunRenderScene();
	//���ñ���ɫ
	VOID SunSetBackColor(BYTE red, BYTE green, BYTE blue);
	//������Ⱦ֡��
	VOID SunSetRenderFPS(UINT fps);
	//�����ӿڴ�С
	VOID SunSetViewPort(RECT* rect);
	//�õ��ӿڴ�С
	VOID SunGetViewPort(RECT* rect);

	// ��ȡ/�����ӿڵ����
	DWORD SunGetViewportCamera(DWORD vp);
	VOID SunSetViewportCamera(DWORD vp, DWORD cam);

	//���º󻺳���
	BOOL SunUpdateBackBuffer();
	//���õ�ǰ�����Ƿ���Ⱦ
	VOID SunSetRenderStatus(BOOL renderIt);

	//-------------------------------------------------------------------------------------------
	//***��Ч��صķ���
	//-------------------------------------------------------------------------------------------
	//������̬ģ����Ч����
	DWORD CreateFSMotionBlur();
	//�򿪶�̬ģ����Ч
	VOID	OpenFSMotionBlur();
	//�رն�̬ģ����Ч
	VOID    CloseFSMotionBlur();
	//���ö�̬ģ����ǿ��, 0-100
	VOID	SetFSMotionBlurPower(INT value);


	//����ȫ�����ζ���
	DWORD CreateFSGlow();
	//��ȫ�����ζ���
	VOID OpenFSGlow();
	//�ر�ȫ�����ζ���
	VOID CloseFSGlow();
	//����ȫ�����ε�ǿ��
	VOID SetFSGlowValue(FLOAT BloomValue, FLOAT BlurValue);


	//����LensFlare��ͷ��߶���
	DWORD	CreateLensFlare(VECTOR3* sunDir);

	//fogMode, 0-LINEAR, 1-EXP, 2-EXP2
	VOID SunCreateFog(INT fogMode, BYTE red, BYTE green, BYTE blue, FLOAT density, FLOAT start, FLOAT end);
	VOID SunSetFogMode(int mode);
	VOID SunSetFogDensity(FLOAT density);
	VOID SunSetFogFiled(FLOAT start, FLOAT end);
	VOID SunSetFogColor(BYTE red, BYTE green, INT blue);
	VOID SunSetFogEnableStatus(BOOL enable);

	//������̬CubeMap, 
	//6��Image����
	//sizeΪcubeMap�ĳߴ磬
	//modeΪ����CubeMap��ģʽ
	//����CubeMap����ľ��
	DWORD CreateStaticCubeMap(CHAR* name, 
								DWORD frontImage, DWORD backImage, 
								DWORD leftImage, DWORD rightImage, 
								DWORD topImage, DWORD bottomImage,
								INT size);

	//ɾ��CubeMap
	VOID DeleteStaticCubeMap(DWORD cubeMapHandle);
	//������̬CubeMap
	//sizeΪcubeMap�ĳߴ磬
	//modeΪ����CubeMap��ģʽ
	//����CubeMap����ľ��
	DWORD CreateDynamicCubeMap(CHAR* cubeMapName, CHAR* centerModelName, UINT size, FLOAT power);
	//�������ɶ�̬CubeMap��Ҫ��Ⱦ��ģ�ͣ�ֻ�е�CreateCubeMap��mode��������Ϊ1ʱ��Ч
	VOID AddDyCubeMapReflectionModel(DWORD cubeMapHandle, DWORD modelHandle);
	//ɾ�����ɶ�̬CubeMap��Ҫ��Ⱦ��ģ�ͣ�ֻ�е�CreateCubeMap��mode��������Ϊ1ʱ��Ч
	VOID DeleteDyCubeMapReflectionModel(DWORD cubeMapHandle, DWORD modelHandle);
	
	//������Ӱ��ͼ
	DWORD CreateMovieTexture(CHAR* name, 
							CHAR* movieFilePath, 
							INT mediaType, 
							BOOL withAudio, 
							INT	repeatCnt, 
							CHAR* mtrlName,
							CRITICAL_SECTION* pRenderCS,
							CRITICAL_SECTION* pCtrlCS);
	//ͨ�����Ƶõ���Ӱ��ͼ����
	DWORD	GetMovieTextureHandleByName(CHAR* name);
	//ͨ����ŵõ���Ӱ��ͼ����
	DWORD	GetMovieTextureHandleByIndex(INT index);
	//���ò���Ϊ��Ӱ��ͼ����
	VOID	SetMovieMtrl(DWORD mtrlHandle, DWORD movieTexHandle);
	//ɾ����Ӱ��ͼ
	VOID	DeleteMovieTexture(DWORD movieTexHandle);
	//ɾ����Ӱ��ͼ
	VOID	DeleteMovieTextureByName(CHAR* name);
	//ɾ����Ӱ��ͼ
	VOID	DeleteMovieTextureByIndex(INT index);
	//ɾ�����еĵ�Ӱ��ͼ
	VOID	DeleteAllMovieTexture();

//-------------------------------------------------------------------------------------------
//***����������صķ���
//-------------------------------------------------------------------------------------------
	DWORD CreateAudio(CHAR* name, CHAR* AudioFilePath, INT repeatCnt);

	VOID DeleteAudio(DWORD audioHandle);
	VOID DeleteAudioByName(CHAR* name);
	VOID DeleteAudioByIndex(INT index);
	VOID DeleteAllAudio();

	INT GetAudioCount();

	DWORD GetAudioHandleByName(CHAR* name);
	DWORD GetAudioHandleByIndex(INT index);

	VOID GetAudioNameByHandle(DWORD audioHandle, CHAR* name);
	INT GetAudioIndexByHandle(DWORD audioHandle);

	VOID SetAudioFilePath(DWORD audioHandle, CHAR* AudioFilePath);
	VOID GetAudioFilePath(DWORD audioHandle, CHAR* AudioFilePath);

	VOID SetAudioRepeatCount(DWORD audioHandle, INT repeatCount);
	INT	GetAudioRepeatCount(DWORD audioHandle);

	VOID PlayAudio(DWORD audioHandle);
	VOID PauseAudio(DWORD audioHandle);
	VOID StopAudio(DWORD audioHandle);
	VOID RewindAudio(DWORD audioHandle);


//-------------------------------------------------------------------------------------------
//***ģ�ͳ�����صķ���
//-------------------------------------------------------------------------------------------
	DWORD	CreateModelGroup(CHAR* groupName);

	VOID	DeleteModelGroup(DWORD ModelGroupHandle);
	VOID	DeleteModelGroupByName(CHAR* groupName);
	VOID	DeleteModelGroupByIndex(INT index);
	VOID	DeleteAllModelGroup();

	INT		GetModelGroupCount();
	VOID	ResetModelGroupIndex();

	DWORD	GetModelGroupHandleByName(CHAR* groupName);
	DWORD	GetModelGroupHandleByIndex(INT index);

	VOID	GetModelGroupNameByHandle(DWORD ModelGroupHandle, CHAR* groupName);
	INT		GetModelGroupIndexByHandle(DWORD ModelGroupHandle);
	
	VOID	AddModelToModelGroup(DWORD modelGroupHandle, DWORD modelHandle);
	VOID	DeleteModelFromModelGroup(DWORD modelGroupHandle, DWORD modelHandle);
	VOID	ClearModelGroup(DWORD modelGroupHandle);

	//-------------------------------------------------------------------------------------------
	//***�û�������صķ���
	//-------------------------------------------------------------------------------------------

	// ��õ�ǰʹ�õĿ��ƶ���
	DWORD SunGetCurrentController();

	// ���õ�ǰʹ�õĿ��ƶ���
	VOID SunSetCurrentController(DWORD controllerHandle);

	// ���ÿ����������
	VOID SunSetControllerCamera(DWORD ControllerHandle, DWORD CameraHandle);

	// ��ȡ��ǰ���ƶ���ʹ�õ����
	DWORD SunGetControllerCamera(DWORD ControllerHandle);

	// ��ȡ��ǰ���ƶ���ʹ�õ����
	DWORD SunGetCurrentControllerCamera();

	//���õ�ǰ�û�������״ֵ̬
	VOID SunSetControlStatus(CONTROLTYPE controlEnum);
	CONTROLTYPE SunGetControllerType(DWORD controller);

	//����״̬����------------------------------------------------------
	VOID WalkControl();
	//�������ߵ��ٶ�
	VOID SetWalkCtrl_MoveParam(FLOAT speed, FLOAT acc);
	//������ת���ٶ�
	VOID SetWalkCtrl_RotateParam(FLOAT speed, FLOAT acc);
	//------------------------------------------------------------------

	//�༭״̬����------------------------------------------------------
	VOID EditControl();
	//����EDITģʽ�µ�ǰ������Զ���ת���ٶȣ�ֻ����EDITģʽ����Ч
	VOID SetAutoPitchRoundRollSpeed(FLOAT fPitchValue, FLOAT fRoundValue, FLOAT fRollValue);
	//����EDITģʽ�µ�ǰ������ֶ���ת���ٶȣ�ֻ����EDITģʽ����Ч
	VOID SetManualPitchRoundSpeed(FLOAT fPitchValue, FLOAT fRoundValue);
	//------------------------------------------------------------------

	//����״̬����------------------------------------------------------
	VOID FlyControl();
	//�������ƽ�Ƶ�ƽ���ٶ�
	VOID SetFlyCtrl_PanParam(FLOAT speed, FLOAT acc);
	//������������ٶ�
	VOID SetFlyCtrl_DollyParam(FLOAT speed, FLOAT acc);
	//------------------------------------------------------------------
    //��ü���״̬
    BOOL GetShiftStatus();


	//---------------------------------------------------------------------------
	//���µ�ˮ��Ч
	//---------------------------------------------------------------------------
	//����ˮ�����x,y,zΪPosition, gridx/gridyΪˮ��ģ�͵�������
	VOID CreateRealWater(CHAR* name, FLOAT x, FLOAT y, FLOAT z, INT gridX, INT gridY);
	//ͨ��ˮ�����ƻ�ö���
	DWORD GetRealWaterHandleByName(CHAR* name);
	//����ˮ�����������
	VOID AddRealWaterRTTModel(DWORD realWaterHandle, DWORD modelHandle);
	//����ˮ��ķ�������
	VOID AddRealWaterRTFModel(DWORD realWaterHandle, DWORD modelHandle);
	//����ˮ���õ���CubeMap����ͼ
	VOID SetRealWaterCubeMap(DWORD realWaterHandle, DWORD cubeMapHandle);

	//---------------------------------------------------------------------------
	//ʵʱ���������
	//---------------------------------------------------------------------------
	//����RTF��ͼ����
	DWORD CreateRTFTexture(CHAR* RTFTextureName, CHAR* refPlaneModel, INT width, INT height, FLOAT power);
	//ͨ��RTF��ͼ�����ֻ�ö���
	DWORD GetRTFTextureHandleByName(CHAR* RTFTextureName);
	//ΪRTF��ͼ����������Ҫ�����ģ��
	VOID AddModelToRTFTexture(CHAR* RTFTextureName, CHAR* modelName);
	
	

	//---------------------------------------------------------------------------
	//���µĻ��������Ч
	//---------------------------------------------------------------------------
	//������ɫ�༭��
	VOID CreateColorEffect();
	//���û�������ȣ��Աȶȣ���ɫ����
	VOID SetColorEffect(FLOAT brightness, FLOAT contrast, FLOAT red, FLOAT green, FLOAT blue);



    //---------------------------------------------------------------------------
	//����ϵͳ
	//---------------------------------------------------------------------------
	//��������ϵͳ
	DWORD CreateParticles(CHAR* name);
	//��������ϵͳ�Ĳ���

	//////////////// ������� ///////////////////////
	BOOL SunLoadSceneFromBuffer(DWORD version, const BYTE* pBuffer);       // ��buffer�ж�ȡ������Ϣ	
	BOOL SunLoadImageDataFromBuffer(DWORD version, const BYTE* pBuffer);   // ��buffer�ж�ȡͼ����Ϣ	
	BOOL SunCreateImageTextureFromBuffer(const BYTE* pBuffer, DWORD len, int imgIdx);  // ��һ��ͼƬ��buffer��������

public:
	CVGS2DLL();
	~CVGS2DLL();
	
public:	
	VOID SetAddtionalKeysStatus(BOOL bEnable);                         // ����VGS��Ӧ�ĵڶ��װ��������˷�������Ƿ���Ч
	VOID SetAddtionalKeys(BYTE left, BYTE right, BYTE up, BYTE down);  // ����VGS��Ӧ�ĵڶ��װ���ֵ����Ҫ���밴����ascii��

	DWORD GetDefaultRenderWnd(){return m_defaultRenderWnd;}
	DWORD GetDefaultViewport(){return m_defaultViewport;}

private:
	BOOL     m_bAddtionalKeysEnable;        // �ڶ��׿��Ƽ��Ƿ����
	BYTE     m_AddKeyLeft;                  // �ڶ��׿��Ƽ�
	BYTE     m_AddKeyRight;
	BYTE     m_AddKeyUp;
	BYTE     m_AddKeyDown;

	HWND     m_hwnd;  // ������

	// ȱʡ��������Ⱦ���ں��ӿ�
	DWORD    m_defaultRenderWnd;
	DWORD    m_defaultViewport;

	CInput   m_inputDevice;   // �����豸

	// ��ǰ�Ŀ�����Ϣ
	WalkerInfo  m_curWalkerInfo;
	EditorInfo  m_curEditorInfo;
	FlyerInfo   m_curFlyerInfo;

	VECTOR3  walker_realMoveVec;
	VECTOR3  walker_realRotateVec;

	int m_mouseX, m_mouseY;  // ������Ļλ��

	// ĳ���ַ����Ƿ񱻴���
	BOOL IsAlphabetKeyBuffered(BYTE key, BOOL caseSensitive = FALSE);

	// ĳ���ַ����Ƿ񱻴���
	BOOL IsAlphaKeyBuffered(BYTE key, BOOL caseSensitive = FALSE);

	// ��ȡ��ǰ���ƶ�����Ϣ
	VOID GetCurrentControllerInfo();
};

#endif
