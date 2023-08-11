/////////////////////////////////////////////////////////////////////////
// �ļ���: CLights.h: interface for the CLights class.
// ��  ��: CLights�ƹ��б���
// ��  ��: �����еĵƹ�����
// ��  ��: lixin
// ��  ��: 2006-06-03
/////////////////////////////////////////////////////////////////////////

#ifndef _CLIGHT_H
#define _CLIGHT_H

#include "VGSHeader.h"
//#include "S3D.h"
//#include "CTransform.h"

#include "CSysFunc.h"
#include "Node.h"
#include ".\fileio\N3D_Def.h"

#include "MovableObject.h"

using namespace N3D;



class VGS_EXPORT CLight : public CMovableObject
{
public:
	//D3D�豸
	LPDIRECT3DDEVICE9    m_pd3dDevice;

	INT					index;
	// std::string			name;
	//##ModelId=47EBC7490061
	D3DLIGHT9			param;				//d3d�ƹ�
	//##ModelId=47EBC7490065
	FLOAT				multiply;			//�ƹ��ǿ�ȱ���
	//##ModelId=47EBC7490067
	D3DCOLORVALUE		ambient;			//ȫ�ֹ���ɫ
	//##ModelId=47EBC749006E
	D3DCOLORVALUE		diffuse;			//�ƹ���ɫ
	//Edit by baojiahui 2008/7/22-------------------------------------------------------------
	// D3DCOLORVALUE		specular;			//ԭʼ�ĸ߹���ɫ
	//----------------------------------------------------------------------------------------
	BOOL				enable;				//�Ƿ񼤻�

	//##ModelId=47EBC7490078
	UINT				currentTimer;		//��ǰʱ��
	//##ModelId=47EBC7490079
	UINT				kfrCnt;				//��ǰ�Ĺؼ�֡����
	//##ModelId=47EBC749007A
	UINT				kfrFrameTime;		//�ؼ�֡��������һ֡��ʱ
	//##ModelId=47EBC749007B
	UINT				kfrCurFrame;		//��ǰ���ŵ�֡

	//##ModelId=47EBC749007D
	//BOOL				kfrPlay;			//�趨�Ƿ񲥷Źؼ�֡����
	//##ModelId=47EBC749007E
	INT					kfrRepeatCnt;		//�ؼ�֡�������Ŵ�����-1��ʾ����ѭ��
	//##ModelId=47EBC749007F
	INT					kfrRepeatIndex;		//��ǰ���ŵĴ���
	//_DLightInfo*		kfrInfo;			//�ؼ�֡������ǰģ�͵���֡������
	//_DLightInfo			CurFrameInfo;		//�ؼ�֡������ǰ֡����
    N3D_OMNI *          pAnimOmniLight;           // ���type!=NLT_OMINI, pOmniLight = NULL
	N3D_DIRECTIONAL*    pAnimDirectionalLight;    // ���type!=NLT_DIRECTIONLIGHT, pDirectionalLight = NULL
	N3D_SPOTLIGHT*      pAnimSpotLight;
	
	BOOL				isPlaying;			//�Ƿ����ڲ��Źؼ�֡����
	//##ModelId=47EBC749008C
	BOOL				playKfr;			//�Ƿ񲥷Źؼ�֡����

	//����ͼ�ڵ�
    // CSceneNode*              pNode;             //��ǰ�ڵ�
	//##ModelId=47EBC749008E
	D3DXVECTOR3			position;		
	//##ModelId=47EBC7490093
	D3DXVECTOR3			direction;
	//�ƶ��ƹ�
	//##ModelId=47EBC7490097
	VOID				move(D3DXVECTOR3* pVec);
	//��ת�ƹ�
	//##ModelId=47EBC7490099
	VOID				rotate(D3DXVECTOR3* pVec);
	//���õƹ��λ��
	//##ModelId=47EBC749009D
	VOID				setPosition(D3DXVECTOR3* pVec);
	//���õƹ�ķ���
	//##ModelId=47EBC749009F
	VOID				setDirection(D3DXVECTOR3* pDir);
	//�õ��ƹ��λ��
	//##ModelId=47EBC74900A1
	D3DXVECTOR3*		getPosition();
	//�õ�ת��
	//##ModelId=47EBC74900A2
	D3DXVECTOR3*		getDirection();

	void UpdateFromNode();

public:
	//��¡�ƹ�
	CLight*				clone(std::string newLightName);

	//���õƹ�ļ���״̬
	void				SetLightEnable(bool status);

	// VOID                SetName(const std::string &sName);
	// std::string         GetName();

	VGS_LIGHT_TYPE      getType();       // ��ȡ�ƹ�����	
	VOID setType(VGS_LIGHT_TYPE type);   // ���õƹ�����

	//##ModelId=47EBC74900A3
	VOID				Release();

	//##ModelId=47EBC74900A4
	CLight(LPDIRECT3DDEVICE9 pd3dDevice);
	//##ModelId=47EBC74900A5
	~CLight();

};

#endif
