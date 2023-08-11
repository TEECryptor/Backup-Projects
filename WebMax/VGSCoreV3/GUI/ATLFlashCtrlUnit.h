//--------------------------------------------------------------------------
//  名称：ATLFlashCtrlUnit.h
//  简介：利用Atl.dll播放Flash文件
//  创建：yaoyoufang
//  日期：04-11-5
//--------------------------------------------------------------------------
#ifndef _ATLFLASHCTRL_UNIT_H_
#define _ATLFLASHCTRL_UNIT_H_
//--------------------------------------------------------------------------
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include "CGuiObject.h"

#include <string>
#include <vector>
using namespace std;
#include "../Flash.h"
//--------------------------------------------------------------------------
#include "../InvokeJSFunc.h"
#include "../CSysFunc.h"
//Add by Baojiahui ----------------------------------------------------------------------
//添加系统消息实现通过C++直接调用flash的函数的方法
/* for our custom flex message */
#define WM_FLEXCALLBACK (WM_USER + 100)
#define WM_FLEXCALLBACK_RETURN (WM_USER + 102)
//---------------------------------------------------------------------------------------
//Add by baojiahui ---------------------------------------------------------
//检测flash中的信号量，当该值为true是调用脚本
typedef struct Semaphore
{
  string SemaphoreName;
  string SemaphoreFunc;
}Semaphore;
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//  名称：TFlashEventHandle
//  简介：Flash事件响应接口                                                                              
//--------------------------------------------------------------------------
class VGS_EXPORT TFlashEventHandle : public IDispatch
{
public:
  TFlashEventHandle(IOleClientSite* pClientSite)
  : m_JSInvoker(pClientSite)
  {
    refcount = 1;
  }
  //----------------------------------------------------------
  virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(/*O*/ UINT *pctinfo)
  {
      return S_OK;
  }
  //----------------------------------------------------------
  virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
      /*I*/ UINT iTInfo,/*I*/ LCID lcid,/*O*/ ITypeInfo **ppTInfo)
  {
      return S_OK;
  }
  //----------------------------------------------------------
  virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
  	/*I*/ REFIID riid, /*I*/ LPOLESTR *rgszNames,
  	/*I*/ UINT cNames,/*I*/ LCID lcid,
  	/*O*/ DISPID *rgDispId)
  {
           return E_NOTIMPL;
  }
  //----------------------------------------------------------
  virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
  	/*I*/ DISPID dispIdMember,/*I*/ REFIID riid,
    /*I*/ LCID lcid, /*I*/ WORD wFlags,
    /*IO*/DISPPARAMS *pParam,
    /*O*/ VARIANT *pVarRet,
    /*O*/ EXCEPINFO *pExcepInfo,
    /*O*/ UINT *puArgErr)
  {
    VARIANTARG val1,val2;
    switch(dispIdMember)
    {
    case 0x00000096:   //OnfsCommand
        val1 = pParam->rgvarg[0];   //参数2
        val2 = pParam->rgvarg[1];   //参数2

		DISPPARAMS dispparams;
		memset(&dispparams, 0, sizeof dispparams);
	  	dispparams.cArgs = 0;
		dispparams.cNamedArgs = 0;
        //执行指定的JS函数
        m_JSInvoker.InvokeJSFunc(val1.bstrVal, &dispparams);
        break;
    default :
        return E_NOTIMPL;
    }
    return S_OK;
  }
  //----------------------------------------------------------
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID classid, void** intf)
  {
      if (classid == ::DIID__IShockwaveFlashEvents)
          *intf = (IDispatch*)this;
      else if (classid == IID_IUnknown)
          *intf = (IUnknown*)this;
      else
          return E_NOINTERFACE;

      AddRef();
      return S_OK;
  }
  //----------------------------------------------------------
  virtual ULONG STDMETHODCALLTYPE AddRef()
  {
      InterlockedIncrement(&refcount);
      return refcount;
  }
  //----------------------------------------------------------
  virtual ULONG STDMETHODCALLTYPE Release()
  {
      InterlockedDecrement(&refcount);
      if (refcount == 0) delete this;
      return refcount;
  }  
//----------------------------------------------------------
private:
  long refcount;     
  CInvokeJSFunc  m_JSInvoker;
};                                                                       
//----------------------------------------------------------   
//	名称：CATLFlashCtrl
//	简介：ATL Flash控件     
//----------------------------------------------------------
class VGS_EXPORT CATLFlashCtrl : public IDispEventImpl<0, CATLFlashCtrl, &DIID__IShockwaveFlashEvents, &LIBID_ShockwaveFlashObjects, 0, 0>, public CGUIObject
{        
//----------------------------------------------------------
//  构造与析构
//----------------------------------------------------------
public:
    CATLFlashCtrl(const string& strName, HWND hParentWnd, IOleClientSite* pClientSite);
    ~CATLFlashCtrl();
//----------------------------------------------------------
//	Flash控件消息事件映射
//----------------------------------------------------------
public:
	BEGIN_SINK_MAP(CATLFlashCtrl)
		//SINK_ENTRY_EX(0,DIID__IShockwaveFlashEvents, 0xFFFFFD9F, OnReadyStateChange)
		//SINK_ENTRY_EX(0,DIID__IShockwaveFlashEvents, 0x000007A6, OnProgress)
		//SINK_ENTRY_EX(0, DIID__IShockwaveFlashEvents, 0x00000096, OnFSCommand)
	END_SINK_MAP();	

//----------------------------------------------------------
//	Flash控件消息事件函数
//----------------------------------------------------------
public:
	//STDMETHOD (OnReadyStateChange)(long newState)	{	return	0;	}
	//STDMETHOD (OnProgress)(long percentDone)		{	return	0;	}	
	//STDMETHOD (OnFSCommand)(BSTR cmd, BSTR args);

//----------------------------------------------------------
//  公共成员函数
//----------------------------------------------------------
public:
    //创建
    void        Create(const RECT& rc, BSTR pszMovieFile);
   	//窗口消息callback 
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//播放
    void        Play(BOOL bLoop = false);
	//暂停
	void		Pause();
    //停止
    void        Stop();
	//跳至指定的帧
	void		SetCurFrm(long frm);
    //设置父窗体的大小
    void        SetParentRect(RECT parentRect);
	void		GetParentRect(RECT& rc) { rc = m_rcParentWnd;}
    //设置Flash窗体Rect
    void        SetRect(int _left, int _top, int _width, int _height);

	//得到当前Flash的总帧数
	int			GetFrameCnt();
	//得到Flash的当前帧
	int			GetCurrentFrame();
	//得到Load的百分比
	int			GetLoadPercent();

    //Add Flash 和VC之间的变量通信方法
	//设置脚本执行
	void        SetJSInvoker(CInvokeJSFunc*  JSInvoker);
	//返回flash中的变量
	string      GetFlashVariable(string variableName);
	//设置flash的变量
	void        SetFlashVariable(string variableName, string variable);

public://CGUIObject methods
	void SetRect(const RECT &rc);
	void SetVisibility(bool bVisible);
	HRESULT UpdateRect(const RECT& rc) { return S_OK;}
	void SetOpacity(int nOpactity) {}
	HRESULT SetRotation(float fAngle) { return S_OK;}
	//释放内存
	void Destroy();

//----------------------------------------------------------
//  私有成员函数
//----------------------------------------------------------
private:
    //根据父窗体的大小调整Flash窗体的大小和位置
    void        CalcCurFlashRect();
//----------------------------------------------------------
//  公共成员变量
//----------------------------------------------------------
public:
	//Movie名称
    BSTR	m_strFlashMoviePath;
	//播放状态			
	INT     m_nPlayStatus;  
	//信号量列表
	vector<Semaphore> SemaphoreList;    

	bool	m_bFullWidth;		//是否宽度占满整个屏幕
	bool	m_bFullHeight;		//是否高度沾满整个屏幕

	bool	m_bCreated;
	CInvokeJSFunc*  m_pJSInvoker;

//----------------------------------------------------------
//  私有成员变量
//----------------------------------------------------------
private:
    HWND		m_hParent;			//父窗体句柄
    HWND		m_hFlashWnd;            //Flash窗体句柄
    IShockwaveFlash*    m_atlFlash;     //Atl Flash对象

    RECT		m_rcParentWnd;        //父窗体区域    
    //
    IOleClientSite*     m_pClientSite;  //
};
//--------------------------------------------------------------------------
#endif //_ATLFLASHCTRL_UNIT_H_
//--------------------------------------------------------------------------







