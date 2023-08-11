/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // V3D_Def.h
* ����ժҪ�� // V3D version3.0���ݽṹ����
* ����˵���� // 
* ��ǰ�汾�� // V3.0
* ��    �ߣ� // ���²�
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#pragma once

#include "..\VGSHeader.h"
#include "V3D_Def.h"

#include <vector>
using namespace std;

#pragma pack(push, 1) // ���ݽṹ�Ե��ֽڶ���


//////////////////////////////////// V3D //////////////////////////////////////////

//V3D�ļ�ͷ
typedef struct V3D_HeaderV3
{
	//��ͼ��˾������
	char					corp[MAX_CORPNAME_LENGTH];
	//v3d�ļ��İ汾
	float					v3dVersion;
	CHAR                    Reserverd[60]; // ������չ
}V3D_HeaderV3;

// V3D����Ŀ¼��
typedef struct V3D_TableV3  // 96 bytes
{
	// ��������
	DWORD                   SceneproperyPtr;
	//�������ݵ�ƫ����
	DWORD					mtrlDataPtr;
	// texture ptr
	DWORD                   texturePtr;
	//������ݵ�ƫ����
	DWORD					cameraDataPtr;
	//�ƹ����ݵ�ƫ����
	DWORD					lightDataPtr;
	//�������ݵ�ƫ����
	DWORD					skeletonDataPtr;

	// ģ����Դ���ݵ�ƫ����
	DWORD					meshDataPtr;

	// ģ����Ϣ����ƫ����
	DWORD                   entityDataPtr;  // fuxb fuxb 2008-07-23

	//ͼ�����ݵ�ƫ����
	DWORD					shapeDataPtr;
	//����ͼ���ݵ�ƫ����
	DWORD					sceneGraphDataPtr;

	// �������ݵ�ƫ����
	DWORD                   TextDataPtr;
	// ��������
	DWORD                   AnimationPtr;
	// overlay����
	DWORD                   OverlayPtr;
	// button
	DWORD                   ButtonPtr;
	// ����ͼ
	DWORD                   NavigatotPtr;

	DWORD                   BackdropPtr;  // ����ͼ  fuxb 2008--7-17

	// ����
	DWORD                   AudioPtr;
	// ������Ϣ
	DWORD                   ControlPtr;
	// flash
	DWORD                   FlashPtr;

	// ��Ȩ��Ϣ
	DWORD                   PrivacyDataPtr;


	CHAR                    Reserved[28];  // ������չ
}V3D_TableV3;

//�ṹ�壺����������
typedef struct V3D_ScenePropV3
{
	INT				Width;				//������
	INT				Height;				//����߶�
	DWORD			BackColor;			//������ɫ
	INT				FPS;				//��Ⱦ֡��
	INT				AntiAliasLevel;		//���ܳݼ���
	//INT				FSBlurLevel;		//ȫ����̬ģ���ļ���
	//INT				FSGlowLevel;		//ȫ�����εļ���

	//INT				KeyframeCnt;		//�ؼ�֡������
	//INT				RepeatCnt;			//ѭ�����ŵĴ����� -1Ϊ���޴�ѭ��
}V3D_ScenePropV3;

//�ṹ��: Flash���ݽṹ
typedef struct V3D_FlashV3
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
}V3D_FlashV3;

//�ṹ��: Flash�б�
typedef struct V3D_FlashListV3
{		
	INT				FlashCnt;			//Flash����
	V3D_FlashV3*	    Flash;				//Flash�б�
}V3D_FlashListV3;


// v3d mesh����
typedef struct V3D_MeshDetailV3
{
	std::string sName;
	UINT vertextCount;
	char* pVertexBuf;
	UINT subMeshCout;
	vector3 boxNearConner;
	vector3 boxFarConner;
	float   sphereRadius;
	// float   sphereCenter;
	vector<V3D_SubMeshDetail> vSubMesh;

	// �¼���Ӧ
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

	V3D_MeshDetail()
	{
		sName = "";
		vertextCount = 0;
		pVertexBuf   = 0;
		subMeshCout  = 0;
		boxFarConner = boxNearConner = vector3(0, 0, 0);
		sphereRadius = 0;
		bMouseOverFunc = bMouseDBClickFunc = bMouseLDownFunc = bMouseLUpFunc = bMouseRDownFunc = bMouseRUpFunc = FALSE;
	}

}V3D_MeshDetailV3;

//�ṹ��: V3D_1_0��Ȩ�ṹ��
typedef struct V3D_PrivacyV3
{
	INT				VersionType;			//�汾��ʾ(���˰汾0����׼�汾1����ҵ�汾2)
	DWORD			ClientIndex;			//��Ȩ�ͻ����
	CHAR			SerialNum[128];			//����Ȩ��������кţ��Ѿ���128λ����
	CHAR			Authorize[128];			//�Ƿ��Ѿ�����Ȩȥ�������к�
	DWORD			ExtendInfo1;			//��չ������Ϣ1
	DWORD			ExtendInfo2;			//��չ������Ϣ2
}V3D_PrivacyV3;

//�ṹ��: Walker����Ľṹ��
typedef struct V3D_WalkerV3
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
}V3D_WalkerV3;

//�ṹ��: walker�����б�
typedef struct V3D_WalkerListV3
{
	INT				WalkerCnt;			//Walker������
	V3D_WalkerV3* Walkers;			//Walkder�����б�
}V3D_WalkerListV3;

//�ṹ��: Editor����
typedef struct V3D_EditorV3
{	
	CHAR				Name[MAX_NAMESTR_LENGTH];		//����

	INT					CameraIndex;	//ʹ�õ�������

	BOOL				bIsAutoRoundRoll;					//Editģʽ���Ƿ��Զ�����

	FLOAT				autoPitchSpeed;						//Editģʽ���Զ���X����ת���ٶ�
	FLOAT				autoRoundSpeed;						//Editģʽ���Զ���ת������ٶ�
	FLOAT				autoRollSpeed;						//Editģʽ���Զ�������̵��ٶ�

	FLOAT				manualPitchSpeed;					//Editģʽ���ֶ���ת������ٶ�
	FLOAT				manualRoundSpeed;					//Editģʽ���ֶ�����������ٶ�

	FLOAT				manualPanXSpeed;					//Editģʽ���ֶ�ƽ��������ٶ�
	FLOAT				manualPanYSpeed;					//Editģʽ���ֶ�ƽ��������ٶ�

	FLOAT				dollySpeed;							//��������ʱ�ƶ�������ٶ�
	FLOAT				dollyAcc;							//����������ٶ�

	FLOAT				dollyNearDis;						//�����Dolly
	FLOAT				dollyFarDis;						//��Զ��Dolly
}V3D_EditorV3;

//�ṹ�壺Editor�����б�
typedef struct V3D_EditorListV3
{
	INT					EditorCnt;							//Editor������
	V3D_EditorV3*		Editors;							//Editor�б�
}V3D_EditorListV3;

//�ṹ��: Flyer����
typedef struct V3D_FlyerV3
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
}V3D_FlyerV3;

//�ṹ��: Flyer�����б�
typedef struct V3D_FlyerListV3
{
	INT				FlyerCnt;						//�����������
	V3D_FlyerV3*		Flyers;							//��������б�
}V3D_FlyerListV3;

//�ṹ�壺���ƶ���
typedef struct V3D_ControlDataV3
{
	INT					ControlType;					//��������
	INT					ControlerIndex;					//���ƶ�����

	V3D_WalkerListV3	WalkerList;						//���ζ����б�
	V3D_EditorListV3	EditorList;						//�༭�����б�
	V3D_FlyerListV3	FlyerList;						//���ж����б�
}V3D_ControlDataV3;


//////////////////// �������ݽṹ��Ϊ�����д��V3Dʱ��Ч�ʶ������ ///////////////////////////////////

// v3�汾����Ϣ
typedef struct V3D_OverlayInfoV3   
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // ����
	INT				materialIndex;  // ʹ�õĲ�������

	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	INT             Origin;  // ����ʱ�Ĳο�ԭ�㣬VGSOFFSETORIGINֵ 
	FLOAT			Width;   // Ŀǰ�Ŀ��, ���ʹ����Դ�С�����ﱣ��ٷֱ� 
	FLOAT			Height;  // Ŀǰ�ĸ߶�
	FLOAT           Left;    // �����꣬������ӿ����Ͻ�, ���ʹ�����λ�ã����ﱣ��ٷֱ�
	FLOAT           Top;     // �����꣬������ӿ����Ͻ� 

	int             ZOrder;  // ���µĲ��˳��Խ��Խ����

	// �¼���Ӧ
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];
}V3D_OverlayInfoV3;


// V3D�б����button����
typedef struct V3D_OverlayListV3
{
	UINT					count;
	V3D_OverlayInfoV3*		OverlayList;	
}V3D_OverlayListV3;


// V3D�б���ĵ���������
typedef struct V3D_NaviPointerInfoV3  
{
	CHAR        sName[MAX_NAMESTR_LENGTH];  // �����������
	int			NavType;		// 0��ʾ�����������1��ʾ��ģ�͵���
	int		    PointedObjIndex;		// �����ģ�Ͷ��������

	int        materialIndex;   // ������ʹ�õĲ�������
	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	int             ZOrder;  // ���е�����֮��Ĳ�ι�ϵ����ʼ���ڵ���ͼ�ϣ�Խ��Խ����

	// �¼���Ӧ
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

}V3D_NaviPointerInfoV3;

typedef struct V3D_NavigatorBaseInfoV3
{
	CHAR			Name[MAX_NAMESTR_LENGTH];    // ����
	int             MapMaterialIndex;   // ��ͼ�õ��Ĳ�������

	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	FLOAT           Opacity;       // ͸����

	INT             Origin;  // ����ʱ�Ĳο�ԭ�㣬VGSOFFSETORIGINֵ 
	float			Width;   // Ŀǰ�Ŀ��, ���ʹ����Դ�С�����ﱣ��ٷֱ�
	float			Height;  // Ŀǰ�ĸ߶�
	float			Left;    // �����꣬������ӿ����Ͻ� , ���ʹ�����λ�ã����ﱣ��ٷֱ�
	float           Top;     // �����꣬������ӿ����Ͻ�

	int             ZOrder;  // ���µĲ��˳��Խ��Խ����, ���ڵ���ͼ������ZOrder��Ч

}V3D_NavigatorBaseInfoV3;


// V3D�б���ĵ���ͼ����
typedef struct V3D_NavigatorInfoV3   
{
	V3D_NavigatorBaseInfoV3		NaviBaseInfo;     // ����ͼ�Ļ�������

	float						mapRectMinX;	  // ����ͼӳ���3D�ռ䷶Χ�������Ϸ��ĸ������
	float                       mapRectMaxX;
	float                       mapRectMinZ;
	float                       mapRectMaxZ;

	INT							PointerCount;     // ���������Ŀ
	V3D_NaviPointerInfoV3*		PointerList;      // �������б�

	// �¼���Ӧ
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

}V3D_NavigatorInfoV3;

// V3D�б���ĵ���ͼ����
typedef struct V3D_NavigatorListV3   
{
	UINT				count;
	V3D_NavigatorInfoV3*  NaviList;	
}V3D_NavigatorListV3;

typedef struct V3D_ButtonInfoV3
{
	CHAR			Name[MAX_NAMESTR_LENGTH];    // ����

	int             UpMaterialIndex;     // ��굯��ʱ��������״̬ʱ�õ��Ĳ�������
	int             DownMaterialIndex;   // ��갴��ʱʹ�õĲ�������
	int             OverMaterialIndex;   // ����ڰ�ť��ʱʹ�õĲ�������
	CHAR            CallbackFunName[MAX_NAMESTR_LENGTH];  // ��ť�Ļص���������

	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	FLOAT           Opacity;       // ͸����

	INT             Origin;  // ����ʱ�Ĳο�ԭ�㣬VGSOFFSETORIGINֵ 
	FLOAT			Width;   // Ŀǰ�Ŀ��, ���ʹ����Դ�С�����ﱣ��ٷֱ�
	FLOAT			Height;  // Ŀǰ�ĸ߶�, ���ʹ����Դ�С�����ﱣ��ٷֱ�
	FLOAT           Left;    // �����꣬������ӿ����Ͻ� , ���ʹ�����λ�ã����ﱣ��ٷֱ�
	FLOAT           Top;     // �����꣬������ӿ����Ͻ� , ���ʹ�����λ�ã����ﱣ��ٷֱ� 

	int             ZOrder;  // ���µĲ��˳��Խ��Խ����

	// �¼���Ӧ
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

}V3D_ButtonInfoV3;

// V3D�б����button����
typedef struct V3D_ButtonListV3
{
	UINT				count;
	V3D_ButtonInfoV3*		ButtonList;	
}V3D_ButtonListV3;

// V3D�б����ǰ��ͼ����
typedef struct V3D_BackdropInfoV3   
{
	CHAR			Name[MAX_NAMESTR_LENGTH];  // ����

	int             materialIndex;     // ��������

	BOOL            bRelativeSize; // �Ƿ�ʹ����Գߴ�
	BOOL            bRelativePos;  // �Ƿ�ʹ�����λ��
	BOOL            bVisible;      // �Ƿ�ɼ�

	INT             Origin;  // ����ʱ�Ĳο�ԭ�㣬VGSOFFSETORIGINֵ 
	FLOAT			Width;   // Ŀǰ�Ŀ��, ���ʹ����Դ�С�����ﱣ��ٷֱ�
	FLOAT			Height;  // Ŀǰ�ĸ߶�, ���ʹ����Դ�С�����ﱣ��ٷֱ�
	FLOAT           Left;    // �����꣬������ӿ����Ͻ� , ���ʹ�����λ�ã����ﱣ��ٷֱ�
	FLOAT           Top;     // �����꣬������ӿ����Ͻ� , ���ʹ�����λ�ã����ﱣ��ٷֱ� 

	int             ZOrder;  // ���µĲ��˳��Խ��Խ����

	// �¼���Ӧ
	BOOL bMouseOverFunc;
	CHAR sMouseOverFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseDBClickFunc;
	CHAR sMouseDBClickFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLDownFunc;
	CHAR sMouseLDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseLUpFunc;
	CHAR bMouseLUpFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRDownFunc;
	CHAR sMouseRDownFunc[MAX_NAMESTR_LENGTH];
	BOOL bMouseRUpFunc;
	CHAR sMouseRUpFunc[MAX_NAMESTR_LENGTH];

}V3D_BackdropInfoV3;

// V3D�б���ı���ͼ����
typedef struct V3D_BackdropListV3
{
	UINT					count;
	V3D_BackdropInfoV3*		BackdropList;	
}V3D_BackdropListV3;

//////////////////////////////////// End of V3D //////////////////////////////////////////

#pragma pack(pop)
