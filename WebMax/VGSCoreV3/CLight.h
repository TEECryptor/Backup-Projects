/////////////////////////////////////////////////////////////////////////
// 文件名: CLights.h: interface for the CLights class.
// 类  名: CLights灯光列表类
// 描  述: 场景中的灯光链表
// 作  者: lixin
// 日  期: 2006-06-03
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
	//D3D设备
	LPDIRECT3DDEVICE9    m_pd3dDevice;

	INT					index;
	// std::string			name;
	//##ModelId=47EBC7490061
	D3DLIGHT9			param;				//d3d灯光
	//##ModelId=47EBC7490065
	FLOAT				multiply;			//灯光的强度倍数
	//##ModelId=47EBC7490067
	D3DCOLORVALUE		ambient;			//全局光颜色
	//##ModelId=47EBC749006E
	D3DCOLORVALUE		diffuse;			//灯光颜色
	//Edit by baojiahui 2008/7/22-------------------------------------------------------------
	// D3DCOLORVALUE		specular;			//原始的高光颜色
	//----------------------------------------------------------------------------------------
	BOOL				enable;				//是否激活

	//##ModelId=47EBC7490078
	UINT				currentTimer;		//当前时钟
	//##ModelId=47EBC7490079
	UINT				kfrCnt;				//当前的关键帧数量
	//##ModelId=47EBC749007A
	UINT				kfrFrameTime;		//关键帧动画播放一帧耗时
	//##ModelId=47EBC749007B
	UINT				kfrCurFrame;		//当前播放的帧

	//##ModelId=47EBC749007D
	//BOOL				kfrPlay;			//设定是否播放关键帧动画
	//##ModelId=47EBC749007E
	INT					kfrRepeatCnt;		//关键帧动画播放次数，-1表示无限循环
	//##ModelId=47EBC749007F
	INT					kfrRepeatIndex;		//当前播放的次数
	//_DLightInfo*		kfrInfo;			//关键帧动画当前模型的逐帧参数表
	//_DLightInfo			CurFrameInfo;		//关键帧动画当前帧参数
    N3D_OMNI *          pAnimOmniLight;           // 如果type!=NLT_OMINI, pOmniLight = NULL
	N3D_DIRECTIONAL*    pAnimDirectionalLight;    // 如果type!=NLT_DIRECTIONLIGHT, pDirectionalLight = NULL
	N3D_SPOTLIGHT*      pAnimSpotLight;
	
	BOOL				isPlaying;			//是否正在播放关键帧动画
	//##ModelId=47EBC749008C
	BOOL				playKfr;			//是否播放关键帧动画

	//场景图节点
    // CSceneNode*              pNode;             //当前节点
	//##ModelId=47EBC749008E
	D3DXVECTOR3			position;		
	//##ModelId=47EBC7490093
	D3DXVECTOR3			direction;
	//移动灯光
	//##ModelId=47EBC7490097
	VOID				move(D3DXVECTOR3* pVec);
	//旋转灯光
	//##ModelId=47EBC7490099
	VOID				rotate(D3DXVECTOR3* pVec);
	//设置灯光的位置
	//##ModelId=47EBC749009D
	VOID				setPosition(D3DXVECTOR3* pVec);
	//设置灯光的方向
	//##ModelId=47EBC749009F
	VOID				setDirection(D3DXVECTOR3* pDir);
	//得到灯光的位置
	//##ModelId=47EBC74900A1
	D3DXVECTOR3*		getPosition();
	//得到转角
	//##ModelId=47EBC74900A2
	D3DXVECTOR3*		getDirection();

	void UpdateFromNode();

public:
	//克隆灯光
	CLight*				clone(std::string newLightName);

	//设置灯光的激活状态
	void				SetLightEnable(bool status);

	// VOID                SetName(const std::string &sName);
	// std::string         GetName();

	VGS_LIGHT_TYPE      getType();       // 获取灯光类型	
	VOID setType(VGS_LIGHT_TYPE type);   // 设置灯光类型

	//##ModelId=47EBC74900A3
	VOID				Release();

	//##ModelId=47EBC74900A4
	CLight(LPDIRECT3DDEVICE9 pd3dDevice);
	//##ModelId=47EBC74900A5
	~CLight();

};

#endif
