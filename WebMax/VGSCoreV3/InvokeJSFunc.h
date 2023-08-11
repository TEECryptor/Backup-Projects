//---------------------------------------------------------
//	名称：InvokeJSFunc.h
//	创建：Lixin
//	日期：06-07-20
//	简介：定义了执行htm文件中JavaScript函数的方法
//---------------------------------------------------------
#ifndef _INVOKEJSFUNC_H_
#define _INVOKEJSFUNC_H_
//---------------------------------------------------------
#include <atlbase.h>
#include <SHLGUID.h>
#include <Mshtml.h> 

#include <comdef.h>

//---------------------------------------------------------
class VGS_EXPORT CInvokeJSFunc
{
	//-----------------------------------------------------
public:
	//-----------------------------------------------------
	CInvokeJSFunc(IOleClientSite* pClientSite)
	{
		m_pClientSite = pClientSite;
		m_spBrowser = NULL;
		m_spDoc = NULL;
		m_DocExist=false;
	}
	~CInvokeJSFunc(){};
	//-----------------------------------------------------
public:
	//-----------------------------------------------------
	//	名称：InvokeJSFunc
	//	简介：执行htm文件中JavaScript函数
	//	参数：strJSFuncName:i:htm文件中的JavaScript函数名称
	//        dispparams: JavaScript函数中的参数
	//	返回：然后执行成功返回TRUE，否则返回FALSE
	//-----------------------------------------------------
	BOOL InvokeJSFunc(BSTR strJSFuncName, DISPPARAMS* dispparams)
	{
		if(!m_DocExist)
		{
			if(m_pClientSite == NULL) 
				return FALSE;
			
			CComQIPtr<IServiceProvider> pISP2(m_pClientSite);
			
			IServiceProvider *pISP =NULL;
			if(pISP2 == NULL) return FALSE;
			
			if(!m_spBrowser) pISP2->QueryService(SID_STopLevelBrowser,IID_IServiceProvider,(void **)&pISP);
			if(!m_spBrowser) pISP->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2,(void **)&m_spBrowser);
			if(m_spBrowser==NULL) return FALSE;
			
			if(!m_spDoc) m_spBrowser->get_Document((IDispatch**)&m_spDoc);
			if(m_spDoc==NULL) return FALSE;
			
			if(getTheScript(m_spDoc,strJSFuncName,dispparams))
			{
				m_DocExist = true;
				return TRUE;
			}
			else{
				if(GetIHTMLDoc(strJSFuncName,dispparams))
					return TRUE;
				else
					return FALSE;
			}
		}else
			{
				if(getTheScript(m_spDoc,strJSFuncName,dispparams))
				{					
					return TRUE;
				}else
				{
				    return FALSE;
				}
				
			}
		}
	
	
	BOOL getTheScript(CComPtr<IHTMLDocument2> spDoc,BSTR JSFuncName, DISPPARAMS* disp)
	{
		
		
		CComPtr<IDispatch> pScript;
		spDoc->get_Script(&pScript);
		if(pScript == NULL)	return FALSE;
		
		CComBSTR bstrMember(JSFuncName);
		DISPID dispid;
		HRESULT hr;
		if(pScript != NULL)
		{
			hr = pScript->GetIDsOfNames(IID_NULL, &bstrMember, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
			if(SUCCEEDED(hr))
			{
				EXCEPINFO excepInfo;
				memset(&excepInfo, 0, sizeof(excepInfo));
				CComVariant vaResult;
				UINT nArgErr = (UINT)-1;  // initialize to invalid arg
				hr = pScript->Invoke(dispid, IID_NULL, 0, DISPATCH_METHOD, disp, &vaResult, &excepInfo, &nArgErr);
			}
		}
		
		return (hr == S_OK ? TRUE : FALSE);
	}		


   //获取IFrame的方法
	// Converts a IHTMLWindow2 object to a IHTMLDocument2. Returns NULL in case of failure.
// It takes into account accessing the DOM across frames loaded from different domains.
CComQIPtr<IHTMLDocument2> HtmlWindowToHtmlDocument(CComQIPtr<IHTMLWindow2> spWindow)
{
     ATLASSERT(spWindow != NULL);

     CComQIPtr<IHTMLDocument2> spDocument;
     HRESULT hRes = spWindow->get_document(&spDocument);
    
     if ((S_OK == hRes) && (spDocument != NULL))
     {
          // The html document was properly retrieved.
          return spDocument;
     }

     // hRes could be E_ACCESSDENIED that means a security restriction that
     // prevents scripting across frames that loads documents from different internet domains.
     CComQIPtr<IWebBrowser2>  spBrws = HtmlWindowToHtmlWebBrowser(spWindow);
     if (spBrws == NULL)
     {
          return CComQIPtr<IHTMLDocument2>();
     }

     // Get the document object from the IWebBrowser2 object.
     CComQIPtr<IDispatch> spDisp;
     hRes = spBrws->get_Document(&spDisp);
     spDocument = spDisp;

     return spDocument;
}


// Converts a IHTMLWindow2 object to a IWebBrowser2. Returns NULL in case of failure.
CComQIPtr<IWebBrowser2> HtmlWindowToHtmlWebBrowser(CComQIPtr<IHTMLWindow2> spWindow)
{
     ATLASSERT(spWindow != NULL);

     CComQIPtr<IServiceProvider>  spServiceProvider = spWindow;
     if (spServiceProvider == NULL)
     {
          return CComQIPtr<IWebBrowser2>();
     }

     CComQIPtr<IWebBrowser2> spWebBrws;
     HRESULT hRes = spServiceProvider->QueryService(IID_IWebBrowserApp, IID_IWebBrowser2, (void**)&spWebBrws);
     if (hRes != S_OK)
     {
          return CComQIPtr<IWebBrowser2>();
     }

     return spWebBrws;
}




    
	//获得IHTML文档对象
	BOOL GetIHTMLDoc(BSTR JSFuncName, DISPPARAMS* disp)
	{		
		CComPtr<IHTMLDocument2>   pDoc = m_spDoc; 
		CComPtr<IHTMLWindow2>  pHTMLWnd = NULL;   
		CComPtr<IHTMLDocument2> pFrameDoc=NULL;   
		CComPtr<IHTMLFramesCollection2> pFramesCollection=NULL;   
		LPDISPATCH	lpDispatch;   
		long	p;   
		VARIANT	varindex,varresult;   
		varresult.vt = VT_DISPATCH;   
		varindex.vt = VT_I4;   
		if(pDoc!=NULL)   
		{   
			HRESULT   hr=pDoc->get_frames(&pFramesCollection);   
			if(SUCCEEDED(hr)&&pFramesCollection!=NULL)   
			{   
				hr=pFramesCollection->get_length(&p);   
				if(SUCCEEDED(hr))   
					for(int i=0; i<p; i++)   
					{   
						varindex.lVal = i;   
						if(pFramesCollection->item(&varindex, &varresult) == S_OK)   
						{   
							lpDispatch=(LPDISPATCH)varresult.ppdispVal;  
							CComQIPtr<IHTMLWindow2> spWindow;
							if   (SUCCEEDED(lpDispatch->QueryInterface(IID_IHTMLWindow2,   (LPVOID   *)&spWindow)))   
							{   
								pFrameDoc =HtmlWindowToHtmlDocument(spWindow);
								if(pFrameDoc!=NULL)
								{
									
									if(getTheScript(pFrameDoc,JSFuncName,disp))
									{
										m_spDoc=pFrameDoc;
										m_DocExist = true;
										return TRUE;
									}
								}
							}   
						}   
					}   
//					pFramesCollection->Release();   
			}   
//			pDoc->Release();   
		}
		return FALSE;
	}
	
	//-----------------------------------------------------
private:
    IOleClientSite*			m_pClientSite;		//
    IWebBrowser2*			m_spBrowser;		//
	CComPtr<IHTMLDocument2> m_spDoc;			//
	bool                    m_DocExist;
};
//---------------------------------------------------------
#endif  //_INVOKEJSFUNC_H_
//---------------------------------------------------------
