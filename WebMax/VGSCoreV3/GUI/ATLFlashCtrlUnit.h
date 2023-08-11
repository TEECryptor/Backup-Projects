//--------------------------------------------------------------------------
//  ���ƣ�ATLFlashCtrlUnit.h
//  ��飺����Atl.dll����Flash�ļ�
//  ������yaoyoufang
//  ���ڣ�04-11-5
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
//���ϵͳ��Ϣʵ��ͨ��C++ֱ�ӵ���flash�ĺ����ķ���
/* for our custom flex message */
#define WM_FLEXCALLBACK (WM_USER + 100)
#define WM_FLEXCALLBACK_RETURN (WM_USER + 102)
//---------------------------------------------------------------------------------------
//Add by baojiahui ---------------------------------------------------------
//���flash�е��ź���������ֵΪtrue�ǵ��ýű�
typedef struct Semaphore
{
  string SemaphoreName;
  string SemaphoreFunc;
}Semaphore;
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//  ���ƣ�TFlashEventHandle
//  ��飺Flash�¼���Ӧ�ӿ�                                                                              
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
        val1 = pParam->rgvarg[0];   //����2
        val2 = pParam->rgvarg[1];   //����2

		DISPPARAMS dispparams;
		memset(&dispparams, 0, sizeof dispparams);
	  	dispparams.cArgs = 0;
		dispparams.cNamedArgs = 0;
        //ִ��ָ����JS����
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
//	���ƣ�CATLFlashCtrl
//	��飺ATL Flash�ؼ�     
//----------------------------------------------------------
class VGS_EXPORT CATLFlashCtrl : public IDispEventImpl<0, CATLFlashCtrl, &DIID__IShockwaveFlashEvents, &LIBID_ShockwaveFlashObjects, 0, 0>, public CGUIObject
{        
//----------------------------------------------------------
//  ����������
//----------------------------------------------------------
public:
    CATLFlashCtrl(const string& strName, HWND hParentWnd, IOleClientSite* pClientSite);
    ~CATLFlashCtrl();
//----------------------------------------------------------
//	Flash�ؼ���Ϣ�¼�ӳ��
//----------------------------------------------------------
public:
	BEGIN_SINK_MAP(CATLFlashCtrl)
		//SINK_ENTRY_EX(0,DIID__IShockwaveFlashEvents, 0xFFFFFD9F, OnReadyStateChange)
		//SINK_ENTRY_EX(0,DIID__IShockwaveFlashEvents, 0x000007A6, OnProgress)
		//SINK_ENTRY_EX(0, DIID__IShockwaveFlashEvents, 0x00000096, OnFSCommand)
	END_SINK_MAP();	

//----------------------------------------------------------
//	Flash�ؼ���Ϣ�¼�����
//----------------------------------------------------------
public:
	//STDMETHOD (OnReadyStateChange)(long newState)	{	return	0;	}
	//STDMETHOD (OnProgress)(long percentDone)		{	return	0;	}	
	//STDMETHOD (OnFSCommand)(BSTR cmd, BSTR args);

//----------------------------------------------------------
//  ������Ա����
//----------------------------------------------------------
public:
    //����
    void        Create(const RECT& rc, BSTR pszMovieFile);
   	//������Ϣcallback 
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//����
    void        Play(BOOL bLoop = false);
	//��ͣ
	void		Pause();
    //ֹͣ
    void        Stop();
	//����ָ����֡
	void		SetCurFrm(long frm);
    //���ø�����Ĵ�С
    void        SetParentRect(RECT parentRect);
	void		GetParentRect(RECT& rc) { rc = m_rcParentWnd;}
    //����Flash����Rect
    void        SetRect(int _left, int _top, int _width, int _height);

	//�õ���ǰFlash����֡��
	int			GetFrameCnt();
	//�õ�Flash�ĵ�ǰ֡
	int			GetCurrentFrame();
	//�õ�Load�İٷֱ�
	int			GetLoadPercent();

    //Add Flash ��VC֮��ı���ͨ�ŷ���
	//���ýű�ִ��
	void        SetJSInvoker(CInvokeJSFunc*  JSInvoker);
	//����flash�еı���
	string      GetFlashVariable(string variableName);
	//����flash�ı���
	void        SetFlashVariable(string variableName, string variable);

public://CGUIObject methods
	void SetRect(const RECT &rc);
	void SetVisibility(bool bVisible);
	HRESULT UpdateRect(const RECT& rc) { return S_OK;}
	void SetOpacity(int nOpactity) {}
	HRESULT SetRotation(float fAngle) { return S_OK;}
	//�ͷ��ڴ�
	void Destroy();

//----------------------------------------------------------
//  ˽�г�Ա����
//----------------------------------------------------------
private:
    //���ݸ�����Ĵ�С����Flash����Ĵ�С��λ��
    void        CalcCurFlashRect();
//----------------------------------------------------------
//  ������Ա����
//----------------------------------------------------------
public:
	//Movie����
    BSTR	m_strFlashMoviePath;
	//����״̬			
	INT     m_nPlayStatus;  
	//�ź����б�
	vector<Semaphore> SemaphoreList;    

	bool	m_bFullWidth;		//�Ƿ���ռ��������Ļ
	bool	m_bFullHeight;		//�Ƿ�߶�մ��������Ļ

	bool	m_bCreated;
	CInvokeJSFunc*  m_pJSInvoker;

//----------------------------------------------------------
//  ˽�г�Ա����
//----------------------------------------------------------
private:
    HWND		m_hParent;			//��������
    HWND		m_hFlashWnd;            //Flash������
    IShockwaveFlash*    m_atlFlash;     //Atl Flash����

    RECT		m_rcParentWnd;        //����������    
    //
    IOleClientSite*     m_pClientSite;  //
};
//--------------------------------------------------------------------------
#endif //_ATLFLASHCTRL_UNIT_H_
//--------------------------------------------------------------------------







