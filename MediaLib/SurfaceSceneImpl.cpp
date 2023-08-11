//-----------------------------------------
//	FileName:SurfaceSceneImpl.cpp
//	Introduce:
//	Creator:Youfang Yao
//	Date:11-06-06
//-----------------------------------------
#include "stdafx.h"
#include <TCHAR.h>
#include "SurfaceSceneImpl.h"
//-----------------------------------------
//Constructor
//-----------------------------------------
CSurfaceSceneImpl::CSurfaceSceneImpl()
 : m_fViewLeft(-1),
   m_fViewRight(1),
   m_fViewTop(1),
   m_fViewBottom(-1),
   m_pD3D(NULL),
   m_pd3dDevice(NULL),
   m_pBackground(NULL),
   m_pBKTexture(NULL)
{
	lstrcpy(m_szTexFilePath, _T("\0"));
};
//-----------------------------------------
//Distructor
//-----------------------------------------
CSurfaceSceneImpl::~CSurfaceSceneImpl()
{
	Clear(SCT_BK | SCT_OBJECT | SCT_TEXTURE);

	if(NULL != m_pd3dDevice)
	{
		m_pd3dDevice->Release();
		m_pd3dDevice = NULL;
	}
	if(NULL != m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
};
//-----------------------------------------	
//To find the rect object in objects list
//-----------------------------------------	
CRectObject* CSurfaceSceneImpl::GetRectObject(LPCTSTR lpszName)
{
	//Convert the object name string to lowercase
	TCHAR szName[MAX_NAME_LEN] = {0};
	lstrcpy(szName, lpszName);
	_tcslwr(szName);

	//To find the object
	CRectObject* pObject = NULL;
	for(int iCnt = 0; iCnt < m_lstRectObjects.size(); iCnt++)
	{
		pObject = m_lstRectObjects[iCnt];
		if(lstrcmp(pObject->GetName(), szName) == 0)
		{
			return pObject;
		}
	}

	//Not find the object in the list
	return NULL;
}
//-----------------------------------------	
//To find a texture object in the textures list
//-----------------------------------------	
CTexture* CSurfaceSceneImpl::GetTextureObj(LPCTSTR lpszTextureFile)
{
	//Convert the texture file string to lowercase
	TCHAR szTexFile[MAX_PATH] = {0};
	lstrcpy(szTexFile, lpszTextureFile);
	_tcslwr(szTexFile);

	//To find the texture object is exist or not
	for(int iCnt = 0; iCnt < m_lstTexture.size(); iCnt++)
	{
		if(lstrcmp(m_lstTexture[iCnt]->GetTexFileName(), szTexFile) == 0)
		{
			return m_lstTexture[iCnt];
		}
	}

	//Not find the texture object in the list
	return NULL;
}
//-----------------------------------------	
//To gen a texture object,if the texture object in the list, DO NOT load it from file again
//-----------------------------------------	
HRESULT	CSurfaceSceneImpl::GenTexture(LPCTSTR lpszTexFile, CTexture** lpTexture, BOOL bAddToList)
{
	if(m_pd3dDevice == NULL || lpszTexFile == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;

	//At first we find the texture is in list or not
	TCHAR szTexFileName[MAX_PATH] = {_T("\0")};
	lstrcpy(szTexFileName, m_szTexFilePath);
	lstrcat(szTexFileName, lpszTexFile);
	*lpTexture = GetTextureObj(szTexFileName);

	//If find not, then load it from the file
	if(*lpTexture == NULL)
	{	
		//Get the file extern name string
		TCHAR szExtern[8] = {_T("\0")};
		LPCTSTR lpFind = lpszTexFile;
		while(*lpFind != '\0') lpFind++;
		while(*lpFind != '.') lpFind--;
		lstrcpy(szExtern, lpFind);
		_tcslwr(szExtern);

		//1.Is static texture, create a static texture object pointer;
		if(lstrcmp(szExtern, _T(".dtx")) != 0)
		{
			CStaticTexture* lpStaticTexture = new CStaticTexture();
			hr = lpStaticTexture->LoadTextureFromFile(m_pd3dDevice, szTexFileName);
			if(FAILED(hr))
			{
				delete lpStaticTexture;
				lpStaticTexture = NULL;
				return hr;
			}

			*lpTexture = lpStaticTexture;
		}
		//2.Is dynamic texture, create a dynamic texture object pointer
		else
		{
			CDynamicTexture* lpDynamicTexture = new CDynamicTexture();

			lpDynamicTexture->SetTextreFilePath(m_szTexFilePath);
			hr = lpDynamicTexture->LoadTextureFromFile(m_pd3dDevice, szTexFileName);
			if(FAILED(hr))
			{
				delete lpDynamicTexture;
				lpDynamicTexture = NULL;
				return hr;
			}

			*lpTexture = lpDynamicTexture;
		}


		//Add it to the texture list
		if(bAddToList)
		{
			m_lstTexture.push_back(*lpTexture);
		}
	}

	return S_OK;
}
//-----------------------------------------	



//-----------------------------------------	
//To clear or rect objects and texture objects
//-----------------------------------------	
void CSurfaceSceneImpl::Clear(DWORD dwClearType)
{
	//To clear the object list
	if(dwClearType & SCT_OBJECT)
	{
		CRectObject* pRectObj = NULL;
		for(int iCnt = 0; iCnt < m_lstRectObjects.size(); iCnt++)
		{
			pRectObj = m_lstRectObjects[iCnt];
			if(pRectObj)
			{
				delete pRectObj;
				pRectObj = NULL;
			}
		}
		m_lstRectObjects.clear();
	}

	//To clear the texture lis
	if(dwClearType & SCT_TEXTURE)
	{
		CTexture* lpTexture = NULL;
		for(int iCnt = 0; iCnt < m_lstTexture.size(); iCnt++)
		{
			lpTexture = m_lstTexture[iCnt];
			if(lpTexture)
			{
				delete lpTexture;
				lpTexture = NULL;
			}
		}
		m_lstTexture.clear();
	}

	//To clear the background object
	if(dwClearType & SCT_BK)
	{
		if(m_pBackground)
		{
			delete m_pBackground;
			m_pBackground = NULL;
		}
	}
}
//-----------------------------------------	
//To initliaze the scene
//-----------------------------------------	
HRESULT	CSurfaceSceneImpl::InitScene(HWND hWnd, UINT uScWidth, UINT uScHeight, RECT rcViewPoint)
{ 
	HRESULT hr = S_OK;   

	//The render window handle
	if(hWnd == NULL)
		return E_POINTER;
	m_hRenderWnd = hWnd;
	
	//To create the D3D interface
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
        return E_FAIL;

	//To create the D3D device interface
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
	//d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferWidth = uScWidth;
	d3dpp.BackBufferHeight = uScHeight;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//
	hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                               D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                               &d3dpp, &m_pd3dDevice);
    if(FAILED(hr))
        return hr;

	hr = SetViewSize(-(uScWidth/2.0f), uScWidth/2.0f, uScHeight/2.0f, -(uScHeight/2.0f));
    if(FAILED(hr))
        return hr;

	//To set the scence states
    m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}
//-----------------------------------------	
//To set the view size for current scene
//-----------------------------------------	
HRESULT	CSurfaceSceneImpl::SetViewSize(float fLeft, float fRight, float fTop, float fBottom)
{
	if(NULL == m_pd3dDevice)
        return E_POINTER;

	m_fViewLeft = fLeft;
	m_fViewRight = fRight;
	m_fViewTop = fTop;
	m_fViewBottom = fBottom;

	//To set the project matrix
    D3DXMATRIXA16 matProj;
	D3DXMatrixOrthoOffCenterLH(&matProj, fLeft, fRight, fBottom, fTop, 0, 1.0f);
    m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	
	//To set the world matrix
    D3DXMATRIXA16 matIdentity;;
    D3DXMatrixIdentity(&matIdentity);
    m_pd3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	//To set the view matrix
    m_pd3dDevice->SetTransform(D3DTS_VIEW, &matIdentity);

	return S_OK;
}
//-----------------------------------------	
//To set the texture files path
//-----------------------------------------	
HRESULT	CSurfaceSceneImpl::SetTextureFilePath(LPCTSTR lpszPath)
{
	lstrcpy(m_szTexFilePath, lpszPath);

	return S_OK;
}
//-----------------------------------------	
//To set the background rect object
//-----------------------------------------	
HRESULT	CSurfaceSceneImpl::AddBackground(LPCTSTR lpszName, 
		float fLeft, float fTop, float fRight, float fBottom, 
		LPCTSTR lpszTexture, BOOL bTexLoop, BOOL bVisible)
{
	HRESULT hr = S_OK;

	//Create the background object
	if(m_pBackground)
		delete m_pBackground;
	m_pBackground = new CRectObject();
	hr = m_pBackground->Create(m_pd3dDevice, lpszName, fLeft, fTop, fRight, fBottom);
	if(FAILED(hr))
		return hr;

	//Create the texture object for background
	CTexture* lpTexture = NULL;
	if(NULL != lpszTexture && lstrlen(lpszTexture) > 0)
	{
		hr = GenTexture(lpszTexture, &lpTexture);
		if(FAILED(hr))
			return hr;
	}

	//Set the texture object for background object
	hr = m_pBackground->SetTextureObject(lpTexture, NULL, NULL, NULL, bTexLoop);
	if(FAILED(hr))
		return hr;

	return S_OK;
}
//-----------------------------------------	
//To add a rect object into the scene
//-----------------------------------------	
HRESULT	CSurfaceSceneImpl::AddRectObject(LPCTSTR lpszName, 
		float fLeft, float fTop, float fRight, float fBottom, 
		LPCTSTR lpszNorTexture, LPCTSTR lpszOnTexture, 
		LPCTSTR lpszDownTexture, LPCTSTR lpszDisableTexture, 
		BOOL bTexLoop, BOOL bVisible)
{
	HRESULT hr = S_OK;

	//To get the normal state texture object
	CTexture* lpNorTexture = NULL;
	if(NULL != lpszNorTexture && lstrlen(lpszNorTexture) > 0)
	{
		hr = GenTexture(lpszNorTexture, &lpNorTexture);
		if(FAILED(hr))
			return hr;
	}

	//To get the mouse on state texture object
	CTexture* lpOnTexture = NULL;
	if(NULL != lpszOnTexture && lstrlen(lpszOnTexture) > 0)
	{
		hr = GenTexture(lpszOnTexture, &lpOnTexture);
		if(FAILED(hr))
			return hr;
	}

	//To get the mouse down state texture object
	CTexture* lpDownTexture = NULL;
	if(NULL != lpszDownTexture && lstrlen(lpszDownTexture) > 0)
	{
		hr = GenTexture(lpszDownTexture, &lpDownTexture);
		if(FAILED(hr))
			return hr;
	}

	//To get the disable state texture object
	CTexture* lpDisableTexture = NULL;
	if(NULL != lpszDisableTexture && lstrlen(lpszDisableTexture) > 0)
	{
		hr = GenTexture(lpszDisableTexture, &lpDisableTexture);
		if(FAILED(hr))
			return hr;
	}

	//Create the new rect object
	TCHAR szName[MAX_NAME_LEN] = {0};
	lstrcpy(szName, lpszName);
	_tcslwr(szName);
	//
	CRectObject* pNewRect = new CRectObject();
	pNewRect->Create(m_pd3dDevice, szName, fLeft, fTop, fRight, fBottom, bVisible);
	pNewRect->SetTextureObject(lpNorTexture, lpOnTexture, lpDownTexture, lpDisableTexture, bTexLoop);

	//Add it to the list
	m_lstRectObjects.push_back(pNewRect);

	return S_OK;
}
//-----------------------------------------	
//To render the scene
//-----------------------------------------	
HRESULT	CSurfaceSceneImpl::Render()
{
    if(NULL == m_pd3dDevice)
        return E_POINTER;

    //Clear the backbuffer to a blue color
    m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    
    //Begin the scene
    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
    {    
		//Render background at first
		if(m_pBackground)
		{
			m_pBackground->Render();
		}

		//Render the rect objects list
		for(int iCnt = 0; iCnt < m_lstRectObjects.size(); iCnt++)
		{
			m_lstRectObjects[iCnt]->Render();
		}

        //End the scene
        m_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return S_OK;
}
//-----------------------------------------	
//To get a object name by mouse point
//-----------------------------------------	
LPCTSTR	CSurfaceSceneImpl::GetObjNameByPnt(int x, int y)
{
	RECT stuCurWndRect;
	GetClientRect(m_hRenderWnd, &stuCurWndRect);
	float fScaleX = (m_fViewRight-m_fViewLeft) / (stuCurWndRect.right-stuCurWndRect.left);
	float fScaleY = (m_fViewTop-m_fViewBottom) / (stuCurWndRect.bottom-stuCurWndRect.top);

	float fPntInSceneX = m_fViewLeft + x*fScaleX;
	float fPntInSceneY = m_fViewTop - y*fScaleY;

	int iObjCnt = m_lstRectObjects.size();
	for(int iCnt = iObjCnt-1; iCnt >= 0; iCnt--)
	{
		if(	m_lstRectObjects[iCnt]->PntInObject(fPntInSceneX, fPntInSceneY) &&
			m_lstRectObjects[iCnt]->GetVisible())
		{
			return m_lstRectObjects[iCnt]->GetName();
		}
	}

	return NULL;
}
//-----------------------------------------	
//To set a object visible or not by name string
//-----------------------------------------	
void CSurfaceSceneImpl::SetObjVisible(LPCTSTR lpszObjName, BOOL bVisible)
{
	//Convert the object name string to lowercase
	TCHAR szName[MAX_NAME_LEN] = {0};
	lstrcpy(szName, lpszObjName);
	_tcslwr(szName);

	//To find the object
	CRectObject *pObj = NULL;
	for(int iCnt = 0; iCnt < m_lstRectObjects.size(); iCnt++)
	{
		pObj = m_lstRectObjects[iCnt];

		//Set all the objects visible attribute
		if(lstrcmp(szName, _T("")) == 0)
		{
			pObj->SetVisible(bVisible);
		}
		else if(lstrcmp(szName, pObj->GetName()) == 0)
		{
			pObj->SetVisible(bVisible);
			break;
		}
	}
}
//-----------------------------------------	
//To create a texutre object from file and add it to the scene texture objects list
//-----------------------------------------	
HRESULT CSurfaceSceneImpl::LoadTextureFile(LPCTSTR lpszTextureFile)
{
	HRESULT hr = S_OK;

	if(lpszTextureFile == NULL)
		return E_POINTER;

	CTexture* lpTexture = NULL;
	
	hr = GenTexture(lpszTextureFile, &lpTexture);
	if(FAILED(hr))
		return hr;

	return S_OK;
}
//-----------------------------------------	
//To chang the background object texture
//-----------------------------------------	
HRESULT CSurfaceSceneImpl::ChangeBkTextrue(LPCTSTR lpszNewTextureFile, BOOL bTexLoop)
{	
	HRESULT hr = S_OK;

	if(lpszNewTextureFile == NULL)
		return E_POINTER;

	//To gen the new texture object
	CTexture* lpTexture = NULL;
	hr = GenTexture(lpszNewTextureFile, &lpTexture);
	if(FAILED(hr))
		return hr;

	m_pBackground->SetTextureObject(lpTexture, NULL, NULL, NULL, bTexLoop);

	return S_OK;
}
//-----------------------------------------	
//To chang a rect object texture
//-----------------------------------------	
HRESULT CSurfaceSceneImpl::ChangeObjTexture(LPCTSTR lpszObjName, UINT uState, LPCTSTR lpszNewTextureFile, BOOL bTexLoop)
{
	HRESULT hr = S_OK;

	if(lpszObjName == NULL || lpszNewTextureFile == NULL)
		return E_POINTER;

	//Convert the object name string to lowercase
	TCHAR szName[MAX_NAME_LEN] = {0};
	lstrcpy(szName, lpszObjName);
	_tcslwr(szName);

	//To find the object
	CRectObject* pObject = NULL;
	pObject = GetRectObject(lpszObjName);

	//Find not the object
	if(pObject == NULL)
		return S_OK;

	//To gen the new texture object
	CTexture* lpTexture = NULL;
	hr = GenTexture(lpszNewTextureFile, &lpTexture);
	if(FAILED(hr))
		return hr;

	//Set the new texture object to the object
	pObject->ChangeTextureObject(uState, lpTexture, bTexLoop);

	return S_OK;
}
//-----------------------------------------	
//Move the rect object by steps
//-----------------------------------------	
HRESULT CSurfaceSceneImpl::MoveObject(LPCTSTR lpszObjName, float fStepX, float fStepY)
{
	HRESULT hr = S_OK;

	CRectObject* pObject = GetRectObject(lpszObjName);
	if(pObject == NULL)
		return E_POINTER;

	pObject->MoveByStep(fStepX, fStepY);

	return S_OK;
}	
//-----------------------------------------	
//Move a rect object to a new position
//-----------------------------------------	
HRESULT CSurfaceSceneImpl::MoveObjectTo(LPCTSTR lpszObjName, float fPosX, float fPosY)
{
	HRESULT hr = S_OK;

	CRectObject* pObject = GetRectObject(lpszObjName);
	if(pObject == NULL)
		return E_POINTER;

	pObject->MoveTo(fPosX, fPosY);

	return S_OK;
}
//-----------------------------------------	
//Rotate the rect object by angle	
//-----------------------------------------	
HRESULT CSurfaceSceneImpl::RotateObject(LPCTSTR lpszObjName, float fAngle)
{
	HRESULT hr = S_OK;

	CRectObject* pObject = GetRectObject(lpszObjName);
	if(pObject == NULL)
		return E_POINTER;

	pObject->RotateByStep(fAngle);

	return S_OK;
}
//-----------------------------------------	
//Set a object state based on object name
//-----------------------------------------	
HRESULT CSurfaceSceneImpl::SetObjectState(LPCTSTR lpszObjName, UINT uState)
{
	if(NULL == lpszObjName)
		return S_FALSE;

	CRectObject* pObject = NULL;
	pObject = GetRectObject(lpszObjName);
	if(NULL == pObject)
		return S_FALSE;

	pObject->SetState(uState);

	return S_OK;
}
//-----------------------------------------	
//Get a object state based on object name
//-----------------------------------------	
UINT CSurfaceSceneImpl::GetObjectState(LPCTSTR lpszObjName)
{
	if(NULL == lpszObjName)
		return S_FALSE;

	CRectObject* pObject = NULL;
	pObject = GetRectObject(lpszObjName);
	if(NULL == pObject)
		return S_FALSE;

	UINT uState = pObject->GetState();

	return uState;
}
//-----------------------------------------	