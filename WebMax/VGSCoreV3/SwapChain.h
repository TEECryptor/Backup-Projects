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

//������Ϣ�ṹ��
//##ModelId=47EBC72C030E
typedef struct _MOUSEINFO
{
	SHORT		X;
	SHORT		Y;				//����
	BOOL		LButton;		//����Ϊ1���ɿ�Ϊ0
	BOOL		MButton;		//�м�����Ϊ1�ɿ�Ϊ0
	BOOL		RButton;
	FLOAT		RollDis;		//����
}_MOUSEINFO;

//##ModelId=47EBC72C031D
class VGS_EXPORT CSwapChain : public CRenderWindow
{
public:
	//##ModelId=47EBC72C032D
	IDirect3DSwapChain9*    m_pSwapChain;					//������
	//##ModelId=47EBC72C0331
	// HWND					m_hWnd;							//����
	//##ModelId=47EBC72C0333
	// RECT					m_rect;							//�����Rect

	//##ModelId=47EBC72C0338
	LPDIRECT3D9				m_pd3d;							//Direct3D����
	//##ModelId=47EBC72C033D
	LPDIRECT3DDEVICE9		m_pd3dDevice;					//D3D Device����
	//##ModelId=47EBC72C0342
	D3DPRESENT_PARAMETERS	m_d3dpp;						//��ǰ���Կ�����
	//##ModelId=47EBC72C0347
	D3DDISPLAYMODE			m_d3dmm;						//��ǰ���Դ����

	//�任�����йصĳ�Ա����-----------------------------------------------------------------------
	//##ModelId=47EBC72C034C
	D3DXMATRIX				m_MatView;						//��ǰView�任����
	//##ModelId=47EBC72C0351
	D3DXMATRIX				m_MatViewProj;					
	//##ModelId=47EBC72C0356
	D3DXMATRIX				m_MatViewInverse;				//��ǰView�任����������
	//##ModelId=47EBC72C035B
	D3DXMATRIX				m_MatProj;						//current Projection Transform Matrix

	//##ModelId=47EBC72C0360
	//CInput*					m_pInput;						//�������

	//##ModelId=47EBC72C0365
	_MOUSEINFO				mouseInfo;						//�����Ϣ

	//##ModelId=47EBC72C036A
	_MOUSEINFO				pre_mouseInfo;					//֮ǰ�����Ϣ
	//���
	//##ModelId=47EBC72C036F
	CCamera*				m_pCamera;						//��ǰSwapChain�����
	//##ModelId=47EBC72C0373
	INT						CameraType;						//������ͣ���CAMERATYPE��Ӧ���û��Զ���Ϊ0
	
	//���������
	//##ModelId=47EBC72C0374
	INT						controlType;					//��ǰ�Ŀ�������0walk,1Editor, 2Flyers, eles None

	////��ǰWalker����, Walker�����Ӧ�Ĳ���
	//##ModelId=47EBC72C0376
	CWalker*				currentWalker;					
	//##ModelId=47EBC72C037B
	CEditor*				currentEditor;					//��ǰEditor����
	//##ModelId=47EBC72C0380
	CFlyer*					currentFlyer;					//��ǰFlyer����


	//##ModelId=47EBC72C0384
	INT						m_projMode;						//ͶӰģʽ��͸��ģʽΪ0�����ģʽΪ1
	//##ModelId=47EBC72C0386
	_D3DFILLMODE			FillMode;						//���ģʽ

	//##ModelId=47EBC72C038A
	FLOAT					m_ratio;						//���峤���

	//##ModelId=47EBC72C038C
	D3DMULTISAMPLE_TYPE		m_MultiSampleType;				//���ܳݼ���

	//##ModelId=47EBC72C0390
	BOOL					m_bRender;						//�Ƿ񼤻���Ⱦ

	//##ModelId=47EBC72C0391
	BOOL					m_bActiveControl;				//�Ƿ񼤻�Walk, Edit, Flyer����
	//##ModelId=47EBC72C0399
	FLOAT                   selectAxisScale;                //��¼�༭�������scaleֵ
public:
	//����SwapChain
	//##ModelId=47EBC72C039A
	VOID					Create();
	//�ͷ�SwapChain
	//##ModelId=47EBC72C039B
	VOID					Invalidate();
	//�ؽ�SwapChain
	//##ModelId=47EBC72C039C
	VOID					Restore();
	//����SwapChian�ĺ󻺳�, ������ı��Сʱ
	//##ModelId=47EBC72C039D
	BOOL					UpdateBackbuffer();
	//�ͷ�SwapChain
	//##ModelId=47EBC72C039E
	VOID					Release();

	//	���һ���ӿ�
	// CViewport*              AddViewport(CCamera* pCamera, FLOAT left, FLOAT top, FLOAT width, FLOAT height, INT ZOrder);
	//BOOL                    RemoveViewport(CViewport *pViewport);
	//CViewport*              GetViewport(INT index);

	// �����ӿڵĴ�С��λ�� left, FLOAT top, FLOAT width, FLOAT height ��Ϊ�ٷֱȣ�ȡֵ[0, 1]
	//VOID SetViewportRect(CViewport *pVP, FLOAT left, FLOAT top, FLOAT width, FLOAT height);

	// �õ��ӿڵĴ�С��λ��
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
