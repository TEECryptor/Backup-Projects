//----------------------------------------------------------------------
// N3D�ļ���ؽṹ
//----------------------------------------------------------------------

#ifndef _N3DV3_DEF_H
#define _N3DV3_DEF_H

#include "..\VGSHeader.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include "S3D_Def.h"
#include "N3D_Def.h"

using namespace std;
using namespace N3D;

#pragma pack(push, 1) // ���ݽṹ�Ե��ֽڶ���

namespace N3DV3
{
	//ģ��
	typedef struct N3D_ModelV3
	{
		//Node������
		CHAR                    name[MAX_NAMESTR_LENGTH];
		//��������
		UINT					vertexCnt;	
		//��������
		N3D_Vertex*				vertex;
		//submesh 
		std::vector<N3D_SubMesh> subMesh;
		//��Χ��
		N3D_BoundBox			boundBox;

		float                   boundSphereRadius;

		bool                    isBillboard;
		bool                    isCollision;
		bool                    isGround;
		bool                    isSkyBox; 
		bool                    isVisible;		
		bool                    isClickable;
		bool                    isAutoNormalise;            // ģ������ʱ���Ƿ��Զ����·��� fuxb 2008-09-09

		// �¼���Ӧ
		BOOL bMouseOverFunc;
		CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseDBClickFunc;
		CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseLDownFunc;
		CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseLUpFunc;
		CHAR sMouseLUpFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseRDownFunc;
		CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseRUpFunc;
		CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

	}N3D_ModelV3;

	//�ṹ��: ģ����
	typedef struct N3D_ModelGroup
	{
		//Node������
		CHAR name[MAX_NAMESTR_LENGTH];

		INT						ModelCnt;			//���е�ģ������
		INT*					ModelIndex;			//���е�ģ�ͱ������,��Ӧmeshlist�е�������

		std::vector<N3D_ModelGroup*> ModelGroup;
	}N3D_ModelGroup;

	//�ṹ��: ģ�����б� --ģ�����б���Ϊһ��scene�µ�ȫ��vectorʹ�ã����ٵ�������ṹ�塢

	//ģ���б�
	typedef struct N3D_ModelListV3
	{
		//�����е�model����
		UINT					modelCnt;
		// model�б�
		N3D_ModelV3*				model;
	}N3D_ModelListV3;

	typedef struct N3D_ImageInfoV3
	{
		CHAR			Name[MAX_NAMESTR_LENGTH];		//����
		DWORD			Size;			//ѹ�����Image���ݴ�С
	}N3D_ImageInfoV3;

	//����http�ϵ�N3D�ļ��е�Image��Ȼ�󱣴浽�ڴ��е�Image��Ԫ���ݽṹ
	typedef struct N3D_ZipImageDataV3
	{

		N3D_ImageInfoV3   imageInfo;      //image����
		BYTE*			    Data;			//ѹ�����Image����
	}N3D_ZipImageDataV3;

	//�ṹ��: Flash���ݽṹ
	typedef struct N3D_FlashV3
	{
		CHAR			Name[MAX_NAMESTR_LENGTH];			//Flash��������
		INT             FileNameLen;        //Flash�ļ�·������
		CHAR*			pFileName;		    //Flash�ļ�ȫ·��

		INT				OffsetOrigin;		//ƫ��λ�õ�����ԭ���ʾ
		BOOL			UseRelativePos;		//�Ƿ�ʹ�����λ��

		RECT			Rect;				//Flash �ߴ�
		FLOAT			Angle;				//ת��
		INT				Opacity;			//͸����
		BOOL			Transparency;		//�Ƿ�ٱ���

		BOOL			UseFlashSize;		//ʹ��Flash�ĳߴ�
	}N3D_FlashV3;

	//�ṹ��: Flash�б�
	typedef struct N3D_FlashListV3
	{		
		INT				FlashCnt;			//Flash����
		N3D_FlashV3*	    Flash;				//Flash�б�
	}N3D_FlashListV3;

	typedef struct N3D_OverlayBasalInfoV3
	{
		CHAR		Name[N3D_MAX_NAME_LENGTH];			// Overlay������

		INT			OffsetOrigin;		// ƫ��λ�õ�����ԭ��
		BOOL		UseRelativePos;		// ʹ�����λ��
		BOOL		UseRelativeSize;	// �Ƿ�ʹ����Եĳߴ�

		RECT		Rect;				// Overlay�ߴ缰λ��
		FLOAT		Angle;				// ת��
		INT			Opacity;			// ͸����
		BOOL		IsBackdrop;			// �Ƿ��Ǳ���ͼ

		INT			MaterialIndex;		// Overlay�Ĳ���Index

		BOOL        IsVisible;          // �Ƿ�ɼ�

		INT         zOrder;    // overlay��z˳��, ֵԽ��Խ���ϲ�

		// �¼���Ӧ
		CHAR	sMouseEventFunc[VGS_MOUSEEVENT_CNT][MAX_NAMESTR_LENGTH];
/*
		BOOL bMouseOverFunc;
		CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseDBClickFunc;
		CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseLDownFunc;
		CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseLUpFunc;
		CHAR sMouseLUpFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseRDownFunc;
		CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
		BOOL bMouseRUpFunc;
		CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];*/

	}N3D_OverlayBasalInfoV3;

	//�ṹ��: Overlay�����ṹ
	typedef struct N3D_OverlayV3
	{
		N3D_OverlayBasalInfoV3	BasalInfo;  // ������Ϣ
	}N3D_OverlayV3;

	//�ṹ��: Overlay�б�
	typedef struct N3D_OverlayListV3
	{
		INT				OverlayCnt;	//Overlay������
		N3D_OverlayV3*	Overlays;	//Overlay�б�
	}N3D_OverlayListV3;

	//����ͼָ����Ϣ
	typedef struct N3D_NavigaterPointerV3
	{
		N3D_OverlayBasalInfoV3 baseInfo;
		UINT		naviType;				//Pointer�����ͣ�Camera-0, Model-1
		INT			ObjIndex;			//�����ģ�͵ı��
	}N3D_NavigaterPointerV3;

	//�ṹ��: Navigater�����ṹ
	typedef struct N3D_NavigaterV3
	{
		N3D_OverlayBasalInfoV3 baseInfo;  // С��ͼ����Ϣ

		FLOAT						sceneLeft;					// ����TopView��Rect
		FLOAT						sceneTop;
		FLOAT						sceneRight;
		FLOAT						sceneBottom;	

		UINT						PointerCnt;					// �����������
		N3D_NavigaterPointerV3*		NavPointer;					// ���������Ϣ

	}N3D_NavigaterV3;

	//�ṹ��: Navigater�б�
	typedef struct N3D_NavigaterListV3
	{
		INT						    NavigaterCnt;	//Navigater����
		N3D_NavigaterV3*			    Navigaters;		//Navigater�б�
	}N3D_NavigaterListV3;

	// ��ť��������
	typedef struct N3D_ButtonBasalInfoV3
	{
		N3D_OverlayBasalInfoV3 baseInfo; 
		INT		MouseOverMtrlIndex;	//overImage�Ĳ���ID��
		INT		MouseDownMtrlIndex;	//downImage�Ĳ���ID��

	}N3D_ButtonBasalInfoV3;

	//�ṹ��: Button�б�
	typedef struct N3D_ButtonListV3
	{
		INT					ButtonCnt;			//��ť����
		N3D_ButtonBasalInfoV3*		Buttons;			//��ť�б�
	}N3D_ButtonListV3;

	//�ṹ��: N3D_1_0��Ȩ�ṹ��
	typedef struct N3D_PrivacyV3
	{
		INT				VersionType;			//�汾��ʾ(���˰汾0����׼�汾1����ҵ�汾2)
		DWORD			ClientIndex;			//��Ȩ�ͻ����
		CHAR			SerialNum[128];			//����Ȩ��������кţ��Ѿ���128λ����
		CHAR			Authorize[128];			//�Ƿ��Ѿ�����Ȩȥ�������к�
		DWORD			ExtendInfo1;			//��չ������Ϣ1
		DWORD			ExtendInfo2;			//��չ������Ϣ2
	}N3D_PrivacyV3;

	//�ṹ��: Walker����Ľṹ��
	typedef struct N3D_WalkerV3
	{
		CHAR			Name[MAX_NAMESTR_LENGTH];		//����
		INT				CameraIndex;	//ʹ�õ�������

		FLOAT			walkSpeed;		//Walker������������ٶ�
		FLOAT			walkAcc;		//Walker����������߼��ٶ�

		FLOAT			rosSpeed;		//Walker���������ת�ٶ�
		FLOAT			rosAcc;			//Walker���������ת���ٶ�

		FLOAT			height;			//Walker�ĸ߶�

		BOOL			bCollision;		//�Ƿ�����ײ
		FLOAT			colDis;			//Walker����ײ����

		BOOL			bWeight;		//�Ƿ�������
		FLOAT			liftSpeed;		//�����½����ٶȣ�ʧȥ����ʱ��Ч
		FLOAT			liftAcc;		//�����½��ļ��ٶȣ�ʧȥ����ʱ��Ч

		FLOAT			upperAngle;		//̧ͷ���������
		FLOAT			lowerAngle;		//��ͷ������ӽ�
	}N3D_WalkerV3;

	//�ṹ��: walker�����б�
	typedef struct N3D_WalkerListV3
	{
		INT				WalkerCnt;			//Walker������
		N3D_WalkerV3* Walkers;			//Walkder�����б�
	}N3D_WalkerListV3;

	//�ṹ��: Editor����
	typedef struct N3D_EditorV3
	{	
		CHAR		Name[MAX_NAMESTR_LENGTH];		//����

		INT			CameraIndex;					//ʹ�õ�������

		BOOL		bIsAutoRoundRoll;				//Editģʽ���Ƿ��Զ�����

		FLOAT		autoSpeedX;						// Editģʽ���Զ��� X ����ת���ٶ�
		FLOAT		autoSpeedY;						// Editģʽ���Զ��� Y ����ת���ٶ�

		FLOAT		manualSpeedX;					//Editģʽ���ֶ���ת������ٶ��� X ��
		FLOAT		manualSpeedY;					//Editģʽ���ֶ�����������ٶ��� Y ��

		FLOAT		manualPanXSpeed;				//ƽ�Ƶ�X�����ٶ�
		FLOAT		manualPanYSpeed;				//ƽ�Ƶ�Y�����ٶ�

		FLOAT		dollySpeed;						//��������ʱ�ƶ�������ٶ�
		FLOAT		dollyAcc;						//����������ٶ�

		FLOAT		dollyNearDis;					//�����Dolly
		FLOAT		dollyFarDis;					//��Զ��Dolly

		INT			rosAxis;						// ��ת���־λ, 0: xy, 1 : x, 2 : y
		vector3     LookAtPos;                      // Editor��Ŀ��� 

	}N3D_EditorV3;

	//�ṹ�壺Editor�����б�
	typedef struct N3D_EditorListV3
	{
		INT					EditorCnt;				//Editor������
		N3D_EditorV3*		Editors;				//Editor�б�
	}N3D_EditorListV3;

	//�ṹ��: Flyer����
	typedef struct N3D_FlyerV3
	{
		CHAR				Name[MAX_NAMESTR_LENGTH];		//����

		INT					CameraIndex;					//ʹ�õ�������

		FLOAT				panSpeed;						//ƽ���ٶ�
		FLOAT				panAcc;							//ƽ�Ƽ��ٶ�

		FLOAT				dollySpeed;						//��������ٶ�
		FLOAT				dollyAcc;						//����������ٶ�

		FLOAT				dollyNearDis;					//�����Dolly
		FLOAT				dollyFarDis;					//��Զ��Dolly

		FLOAT				rotateSpeed;					//�����ת���ٶ�
		FLOAT				rotateAcc;						//�����ת�ļ��ٶ�

		FLOAT				liftSpeed;						//�����½����ٶȣ�ʧȥ����ʱ��Ч
		FLOAT				liftAcc;						//�����½��ļ��ٶȣ�ʧȥ����ʱ��Ч

		FLOAT				upperAngle;						//̧ͷ���������
		FLOAT				lowerAngle;						//��ͷ������ӽ�

		BOOL				bCollision;						//�Ƿ�����ײ
		FLOAT				colDis;							//��ײ����
	}N3D_FlyerV3;

	//�ṹ��: Flyer�����б�
	typedef struct N3D_FlyerListV3
	{
		INT					FlyerCnt;						//�����������
		N3D_FlyerV3*		Flyers;							//��������б�
	}N3D_FlyerListV3;

	//�ṹ�壺���ƶ���
	typedef struct N3D_ControlDataV3
	{
		INT					ControlType;					//��������	CT_WALKER = 0, CT_EDITOR = 1, CT_FLYER = 2,	CT_NONE �ǽ���ģʽ 
		INT					ControlerIndex;					//���ƶ�����

		N3D_WalkerListV3	WalkerList;						//���ζ����б�
		N3D_EditorListV3	EditorList;						//�༭�����б�
		N3D_FlyerListV3	FlyerList;						//���ж����б�
	}N3D_ControlDataV3;


	// ��Ȩ�ṹ�� 32 ���ֽ�
	typedef struct N3D_RegInfoV3
	{
		DWORD			CorpID;				// ����Ȩ�Ĺ�˾��ʶ
		DWORD			Date1;				// ��Ȩ�����ڲ���
		DWORD			Date2;				// ��Ȩ�����ڲ���
		DWORD           Reserved[5];        // Ԥ��λ��
	}N3D_RegInfoV3;

	// �ṹ��: N3D�ļ�ͷ�ṹ 96���ֽ� + ����Ŀɱ䲿��
	typedef struct N3D_HeaderV3
	{	
		N3D_Header		baseHeader;				// ����ͷ��Ϣ

		// v3����������
		DWORD           imageCnt;               // �ļ��б����ͼƬ�ļ���Ŀ
		DWORD*          pImageIndexData;        // ÿ��ͼƬ������n3d�ļ��еĿ�ʼλ��

	}N3D_HeaderV3;

} // namespace N3D



#pragma pack(pop)

#endif