// VGSRender.h: interface for the CVGSRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VGSRENDER_H__545DE559_E9EC_4D27_9E4D_350A4CC38046__INCLUDED_)
#define AFX_VGSRENDER_H__545DE559_E9EC_4D27_9E4D_350A4CC38046__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VGSHeader.h"
#include "CD3DDevice.h"
#include "CFont.h"
#include "Viewport.h"
//#include "lua/VgsLua.h"
#include "vgsdef.h"
class VGS_EXPORT CVGSRender  
{
public:
	//D3D�������----------------------------------------------------------------------------------
	//##ModelId=47EBC7290178
 	LPDIRECT3D9				m_pd3d;							//Direct3D����
	//##ModelId=47EBC729017D
	LPDIRECT3DDEVICE9		m_pd3dDevice;					//D3D Device����
	
	//##ModelId=47EBC7290181
	HWND					m_hWnd;							//������

public:
	//D3D��ʾ�豸��صı���------------------------------------------------------------------------
	//##ModelId=47EBC7290183
	CD3DDevice*				m_DeviceInfo;					//��ǰϵͳ�������豸��Ϣ

	//##ModelId=47EBC729018A
	D3DPRESENT_PARAMETERS	m_d3dpp;						//��ǰ���Կ�����
	//##ModelId=47EBC729018F
	D3DDISPLAYMODE			m_d3dmm;						//��ǰ���Դ����
	//##ModelId=47EBC7290194
	D3D_PARAM				d3dParam;

	//##ModelId=47EBC7290198
	INT						m_antiLevel;					//��ǰ�Ŀ��ܳݼ���

	//##ModelId=47EBC729019A
    LPDIRECT3DSURFACE9		m_pBackBuffer;					//�Կ��󻺳����
	//##ModelId=47EBC729019F
	LPDIRECT3DSURFACE9		m_pZBuffer;						//�Կ�ZBuffer����

	//##ModelId=47EBC72901A4
	CFont*					m_pFont;							//�������

public:
	//��ʼ��VGS����
	//##ModelId=47EBC72901A8
	HRESULT					Init(HWND hWnd, WEBBROWSERTYPE type = WBT_IE);
	//��ʼ��d3d�豸
	//##ModelId=47EBC72901AA
	HRESULT					InitD3DDevice();
	//����
	//##ModelId=47EBC72901AB
	HRESULT					Update(CViewport *pViewport);
	//�ͷ�d3d�豸����Դ
	//##ModelId=47EBC72901AC
	VOID					Release();

	// �Ƿ�֧��cubemap
	BOOL IsSupportCUBEMAP();

	/** �������������
	@param type INT : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
	*/
	VOID SetWebBrowserType(WEBBROWSERTYPE type){m_BrowserType = type;}

	/** ��ȡ���������
	@return : 0 IE, 1 ff, 2 google, 3 opera, 4 safari
	*/
	WEBBROWSERTYPE  GetWebBrowserType(){return m_BrowserType;}

	/** ����IE�������������
	@param type INT : 0 IE, 1 maxthon, 2 tencent
	*/
	VOID SetWebBowserSubtype(WEBBROWSER_SUBTYPE subType){m_BrowserSubtype = subType;}

	/** ��ȡIE�������������
	@return : 0 IE, 1 maxthon, 2 tencent
	*/
	WEBBROWSER_SUBTYPE  GetWebBowserSubtype(){return m_BrowserSubtype;}

public:
	//##ModelId=47EBC72901AD
	CVGSRender();
	//##ModelId=47EBC72901AE
	~CVGSRender();

private:
	LPDIRECT3DSURFACE9 m_DepthStencilBuffer;

	WEBBROWSERTYPE          m_BrowserType;                  // ���������
	WEBBROWSER_SUBTYPE      m_BrowserSubtype;               // �����IE���������ʾ����IE�����

};

// extern CVGSRender g_VGSRender;  // ȫ�ֵ���Ⱦ������

#endif // !defined(AFX_VGSRENDER_H__545DE559_E9EC_4D27_9E4D_350A4CC38046__INCLUDED_)
