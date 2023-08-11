// SwapChain.h: interface for the CSwapChain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SWAPCHAIN_H__D0FDFC32_B3A8_4820_AA31_488DF9FE9697__INCLUDED_)
#define AFX_SWAPCHAIN_H__D0FDFC32_B3A8_4820_AA31_488DF9FE9697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"

/*#include "VGSHeader.h"
#include "CSysFunc.h"*/

#include "CCamera.h"
// #include "CInput.h"

// #include ".\Controller\CWalker.h"
// #include ".\Controller\CEditor.h"
// #include ".\Controller\CFlyer.h"

#include "Viewport.h"
#include <WINDEF.h>

#include "RenderWindow.h"

//鼠标的信息结构体
//##ModelId=47EBC72C030E
typedef struct _MOUSEINFO
{
	SHORT		X;
	SHORT		Y;				//坐标
	BOOL		LButton;		//按下为1，松开为0
	BOOL		MButton;		//中键按下为1松开为0
	BOOL		RButton;
	FLOAT		RollDis;		//滚轮
}_MOUSEINFO;

//##ModelId=47EBC72C031D
class VGS_EXPORT CSwapChain : public CRenderWindow
{
public:
	//##ModelId=47EBC72C032D
	IDirect3DSwapChain9*    m_pSwapChain;					//交换链
	//##ModelId=47EBC72C0331
	// HWND					m_hWnd;							//窗体
	//##ModelId=47EBC72C0333
	// RECT					m_rect;							//窗体的Rect

	//##ModelId=47EBC72C0338
	LPDIRECT3D9				m_pd3d;							//Direct3D对象
	//##ModelId=47EBC72C033D
	LPDIRECT3DDEVICE9		m_pd3dDevice;					//D3D Device对象
	//##ModelId=47EBC72C0342
	D3DPRESENT_PARAMETERS	m_d3dpp;						//当前的显卡参数
	//##ModelId=47EBC72C0347
	D3DDISPLAYMODE			m_d3dmm;						//当前的显存参数

	//变换矩阵有关的成员变量-----------------------------------------------------------------------
	//##ModelId=47EBC72C034C
	D3DXMATRIX				m_MatView;						//当前View变换矩阵
	//##ModelId=47EBC72C0351
	D3DXMATRIX				m_MatViewProj;					
	//##ModelId=47EBC72C0356
	D3DXMATRIX				m_MatViewInverse;				//当前View变换矩阵的逆矩阵
	//##ModelId=47EBC72C035B
	D3DXMATRIX				m_MatProj;						//current Projection Transform Matrix

	//##ModelId=47EBC72C0360
	//CInput*					m_pInput;						//输入对象、

	//##ModelId=47EBC72C0365
	_MOUSEINFO				mouseInfo;						//鼠标信息

	//##ModelId=47EBC72C036A
	_MOUSEINFO				pre_mouseInfo;					//之前鼠标信息
	//相机
	//##ModelId=47EBC72C036F
	CCamera*				m_pCamera;						//当前SwapChain的相机
	//##ModelId=47EBC72C0373
	INT						CameraType;						//相机类型，与CAMERATYPE对应，用户自定义为0
	
	//控制类对象
	//##ModelId=47EBC72C0374
	INT						controlType;					//当前的控制类型0walk,1Editor, 2Flyers, eles None

	////当前Walker对象, Walker对象对应的参数
	//##ModelId=47EBC72C0376
	CWalker*				currentWalker;					
	//##ModelId=47EBC72C037B
	CEditor*				currentEditor;					//当前Editor对象
	//##ModelId=47EBC72C0380
	CFlyer*					currentFlyer;					//当前Flyer对象


	//##ModelId=47EBC72C0384
	INT						m_projMode;						//投影模式，透视模式为0，轴侧模式为1
	//##ModelId=47EBC72C0386
	_D3DFILLMODE			FillMode;						//填充模式

	//##ModelId=47EBC72C038A
	FLOAT					m_ratio;						//窗体长宽比

	//##ModelId=47EBC72C038C
	D3DMULTISAMPLE_TYPE		m_MultiSampleType;				//抗拒齿级别

	//##ModelId=47EBC72C0390
	BOOL					m_bRender;						//是否激活渲染

	//##ModelId=47EBC72C0391
	BOOL					m_bActiveControl;				//是否激活Walk, Edit, Flyer控制
	//##ModelId=47EBC72C0399
	FLOAT                   selectAxisScale;                //记录编辑坐标轴的scale值
public:
	//创建SwapChain
	//##ModelId=47EBC72C039A
	VOID					Create();
	//释放SwapChain
	//##ModelId=47EBC72C039B
	VOID					Invalidate();
	//重建SwapChain
	//##ModelId=47EBC72C039C
	VOID					Restore();
	//更新SwapChian的后缓冲, 当窗体改变大小时
	//##ModelId=47EBC72C039D
	BOOL					UpdateBackbuffer();
	//释放SwapChain
	//##ModelId=47EBC72C039E
	VOID					Release();

	//	添加一个视口
	// CViewport*              AddViewport(CCamera* pCamera, FLOAT left, FLOAT top, FLOAT width, FLOAT height, INT ZOrder);
	//BOOL                    RemoveViewport(CViewport *pViewport);
	//CViewport*              GetViewport(INT index);

	// 设置视口的大小和位置 left, FLOAT top, FLOAT width, FLOAT height 均为百分比，取值[0, 1]
	//VOID SetViewportRect(CViewport *pVP, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

	// 得到视口的大小和位置
	//BOOL GetViewportRect(CViewport *pVP, FLOAT &left, FLOAT &top, FLOAT &width, FLOAT &height);

	LPDIRECT3DSURFACE9 getDepthStencilSurface(){return m_DepthStencilBuffer;}

public:
	//##ModelId=47EBC72C03A9
	CSwapChain(LPDIRECT3D9 pd3d, LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd);
	//##ModelId=47EBC72C03AD
	~CSwapChain();

private:
	// vector <CViewport*> m_vViewpot;

	LPDIRECT3DSURFACE9 m_DepthStencilBuffer;

};

#endif // !defined(AFX_SWAPCHAIN_H__D0FDFC32_B3A8_4820_AA31_488DF9FE9697__INCLUDED_)
