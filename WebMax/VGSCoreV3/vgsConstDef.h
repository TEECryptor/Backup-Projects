
#pragma once

#include "d3dx9math.h"

/////////// ���峣�� ////////////////////////////////////
/// �����256���ֽڣ�����'\0'
const INT _MAX_NAMESTR_LENGTH  = 64;    // ����������������64
const INT _MAX_ERRORSTR_LENGTH = 128;
const INT _MAX_FILENAME_LENGTH = 256;   // �ļ�����·�������ƣ�������256�ֽ�

// Ԥ�������������
const char CAMERA_NAME_FRONT[]       = "VGS_FrontCamera";
const char CAMERA_NAME_BACK[]        = "VGS_BackCamera";
const char CAMERA_NAME_LEFT[]        = "VGS_leftCamera";
const char CAMERA_NAME_RIGHT[]       = "VGS_RightCamera";
const char CAMERA_NAME_TOP[]         = "VGS_TopCamera";
const char CAMERA_NAME_BOTTOM[]      = "VGS_BottomCamera";
const char CAMERA_NAME_PERSPECTIVE[] = "VGS_PerspectiveCamera";

// ȱʡ�����Զ��������
const INT g_CamDefaultNearClip = 5;
const INT g_CamDefaultfarClip  = 10000 * g_CamDefaultNearClip;

//�ؼ�֡�������ŵ�֡��
const int	gkfrPlaySpeed = 25;

// ��ά�ؼ�����ڴ��ڵ�λ�ù�ϵ
enum OFFSETORIGIN
{
	TOP_LEFT = 0,  // ���Ͻ�
	TOP_MIDDLE,    // ������е�
	TOP_RIGHT,     // ���Ͻ�
	MIDDLE_LEFT,   // ����ߵ��е�
	CENTER,        // ����
	MIDDLE_RIGHT,  // ���ұߵ��е�
	BOTTOM_LEFT,   // ���½�
	BOTTOM_MIDDLE, // ��������е�
	BOTTOM_RIGHT   // ���½�
};

#pragma pack(push, 1) // ���ݽṹ�Ե��ֽڶ���

// GUI ����Ļ�������
struct GUI_CommonProperty
{
	char            sName[_MAX_NAMESTR_LENGTH];
	float			left;   // ���ʹ�����λ�ã�������ǰٷֱȣ����ʹ�õ��Ǿ���λ�ã�������Ǿ�������
	float			top;  
	float			width;	// ���ʹ����Դ�С��������ǰٷֱȣ����ʹ�õ��Ǿ��Դ�С��������Ǿ��Դ�С
	float			height;
	bool			bIsRelativePos;
	bool			bIsRelativeSize;
	OFFSETORIGIN	OffsetOrigin;
	int             Opacity;   // [0, 100]
	bool            bVisible;
	int             ZOrder;  // ���µĲ��˳��Խ��Խ����
};

#pragma pack(pop)


#define WM_GUI_SENDTEXT	            (WM_USER + 0x130) // ChatInputBox��������
#define WM_GUI_COMBOBOXCHANGED	    (WM_USER + 0x131) // combboboxѡ�����ݱ仯
#define WM_GUI_BUTTONCLICK   	    (WM_USER + 0x132) // ��ťclick
#define WM_GUI_NAVIGATORCLICK       (WM_USER + 0x133) // ����ͼclick
#define WM_GUI_SCROLLBARPOS    	    (WM_USER + 0x134) // ����������
#define WM_GUI_PROGRESSBARPOS  	    (WM_USER + 0x135) // ���������ȸ���
#define WM_GUI_TEXTBOX       	    (WM_USER + 0x136) // textbox���»س�


const INT  DEFAULT_MIPMAP         = 3;    // ȱʡ��mipmap
const INT  DEFAULT_ALPHATESTVALUE = 64;   // ȱʡ��alphaTestֵ

// ��������
#define  VGS_ZEROVECTOR3			D3DXVECTOR3(0, 0, 0)
#define  VGS_XVECTOR3				D3DXVECTOR3(1, 0, 0)
#define  VGS_YVECTOR3				D3DXVECTOR3(0, 1, 0)
#define  VGS_ZVECTOR3				D3DXVECTOR3(0, 0, 1)
#define  VGS_ONEVECTOR3				D3DXVECTOR3(1, 1, 1)
#define  VGS_ZEROQUATERNION			D3DXQUATERNION(0, 0, 0, 1)
