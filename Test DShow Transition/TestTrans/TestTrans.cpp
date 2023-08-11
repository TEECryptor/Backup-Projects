// TestTrans.cpp : Defines the entry point for the application.
//
//---------------------------------------------------
#include "stdafx.h"
#include "resource.h"
#include <TCHAR.h>
#include <dshow.h>
#include <commdlg.h>
#include "CPoster.h"
#include "Timeline.h"
#include "Graph.h"
//---------------------------------------------------
typedef enum _SOURCE_TYPE 
{
	ST_LEFT		= 0,
	ST_RIGHT	= 1,
	ST_COUNT	
}Source_Type;
//---------------------------------------------------
HWND                g_hMainWnd = 0;         // Main dialog window
HINSTANCE           g_hInstance = 0;        // App instance
//
CGraphHelper        g_Graph;          
CPosterImage*		g_pSourceImage[ST_COUNT];
CTimeline*			g_pTimeLine = 0;
//---------------------------------------------------
//	The main dialog process funciton
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
//	The Graph Event Call Back function
void GraphEventCallback(long evCode, long Param1, long Param2);
//	The WM_INITDIALOG message handler
void OnInitDialog(HWND hDlg);
//	Dispose of everything. Called before the application exits.
void CleanUp();
//	To Enable or Disable the preview button
void EnablePreviewBtn(BOOL bEnable);
//	The WM_PAINT message handler
HRESULT OnPaint();
//	To open the sources file
HRESULT OnOpenSource(UINT uID);
//	To set the selected transition
HRESULT SetTransition();
//	To play the translate rendering
HRESULT Play();
//	To stop the translate rendering
void Stop();
//---------------------------------------------------


//---------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    g_hInstance = hInstance; 

    // Initialize COM   
    if(FAILED(CoInitialize(NULL))) 
    {
        MessageBox(NULL, _T("Could not initialize COM library."), _T("TestTrans"), MB_OK);
        return FALSE;
    }

    // Display the main dialog box.
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDLG), NULL, MainDlgProc);

    // Release COM
    CoUninitialize();

	return 0;
}
//---------------------------------------------------




//---------------------------------------------------
// Name: MainDlgProc
// Desc: The main dialog process funciton
//---------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch (msg) 
    {   
        case WM_INITDIALOG:
            OnInitDialog(hDlg);
            return FALSE;
        case WM_CLOSE:
            PostQuitMessage(0);
            break;    
        case WM_DESTROY:
			CleanUp();
            EndDialog(hDlg, 0);
            break;
        case WM_PAINT:
			{
				OnPaint();
				return FALSE;
			}
			break;
        case WM_COMMAND:        
			{
				switch (LOWORD(wParam))
				{
                case IDC_LISTTRANS:
                    switch (HIWORD(wParam))
                    {
                        case LBN_SELCHANGE:
                            Stop();          // Stop the current transition
                            EnablePreviewBtn(TRUE);
                            SetTransition();
                            break;

                        default: 
                            return FALSE;    // Did not handle this message
                    }
                    break;
				case IDC_BTN_OPEN1:
					OnOpenSource(ST_LEFT);
					break;
				case IDC_BTN_OPEN2:
					OnOpenSource(ST_RIGHT);
					break;
				case IDC_BTN_PREVIEW:
					Play();
					break;
				case IDOK:
					PostQuitMessage(0);
					break;
				default: 
                     return FALSE;     
				}
			}
			break;
        case WM_GRAPHNOTIFY:
            // Handle graph events on a callback
            g_Graph.ProcessEvents(GraphEventCallback);  
            break;
        default:
            return FALSE;
	}

	return TRUE;
}
//---------------------------------------------------
// Name: MainDlgProc
// Desc: The Graph Event Call Back function
//---------------------------------------------------
void GraphEventCallback(long evCode, long Param1, long Param2)
{
    // We only care about stopping events
    switch (evCode) 
    {
        case EC_COMPLETE:   // fall through
        case EC_ERRORABORT: // fall through
        case EC_USERABORT:
            Stop();
            EnablePreviewBtn(TRUE);
            break;
		default:
			break;
    }
}
//---------------------------------------------------
// Name: MainDlgProc
// Desc: The WM_INITDIALOG message handler
//---------------------------------------------------
void OnInitDialog(HWND hDlg)
{
	g_hMainWnd = hDlg;

	RECT rc;
	GetClientRect(hDlg, &rc);
	int iDlgWidth = rc.right - rc.left;
	int iDlgHeight = rc.bottom - rc.top;
	iDlgHeight += 36;

	int iScreenW = GetSystemMetrics(SM_CXSCREEN);
	int iScreenH = GetSystemMetrics(SM_CYSCREEN);

	rc.left = (iScreenW - iDlgWidth) / 2;
	rc.top = (iScreenH - iDlgHeight) / 2;
	rc.right = rc.left + iDlgWidth;
	rc.bottom = rc.top + iDlgHeight;

	SetWindowPos(hDlg, NULL, rc.left, rc.top, iDlgWidth, iDlgHeight, SWP_SHOWWINDOW);

	g_pSourceImage[ST_LEFT] = new CPosterImage(GetDlgItem(hDlg, IDC_SOURCE1));
	g_pSourceImage[ST_RIGHT] = new CPosterImage(GetDlgItem(hDlg, IDC_SOURCE2));

	g_pTimeLine = new CTimeline();

    RECT rect;
    GetClientRect(GetDlgItem(hDlg, IDC_VIEW), &rect);
    g_pTimeLine->InitTimeline(rect);

    // Populate the list of transitions
    g_pTimeLine->InitTransitionList(GetDlgItem(hDlg, IDC_LISTTRANS));
}
//---------------------------------------------------
// Name: OnPaint
// Desc: Dispose of everything. Called before the application exits.
//---------------------------------------------------
void CleanUp()
{
	if(NULL != g_pTimeLine)
	{
		delete g_pTimeLine;
		g_pTimeLine = NULL;
	}
	//
	for(int iCnt = 0; iCnt < ST_COUNT; iCnt++)
	{
		if(NULL != g_pSourceImage[iCnt]) 
		{
			delete g_pSourceImage[iCnt];
			g_pSourceImage[iCnt] = NULL;
		}
	}
}
//---------------------------------------------------
// Name: OnPaint
// Desc: To Enable or Disable the preview button
//---------------------------------------------------
void EnablePreviewBtn(BOOL bEnable)
{
    EnableWindow(GetDlgItem(g_hMainWnd, IDC_BTN_PREVIEW), bEnable);
}
//---------------------------------------------------
// Name: OnPaint
// Desc: The WM_PAINT message handler
//---------------------------------------------------
HRESULT OnPaint()
{
	for(int iCnt = 0; iCnt < ST_COUNT; iCnt++)
	{
		if(NULL != g_pSourceImage[iCnt]) 
		{
			g_pSourceImage[iCnt]->Draw();
		}
	}

	return S_OK;
}
//---------------------------------------------------
// Name: OnOpenSource
// Desc: To open the source files
//---------------------------------------------------
HRESULT OnOpenSource(UINT uID)
{	
    OPENFILENAME ofn;
    TCHAR tszFileName[MAX_PATH] = _T("\0");
    TCHAR tszFileTitle[MAX_PATH] = _T("\0");

    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize  = sizeof(OPENFILENAME);
    ofn.hwndOwner    = g_hMainWnd;
    ofn.lpstrFilter  = _T("Image Files (*.jpg, *.bmp, *.gif, *.tga)\0*.jpg; *.bmp; *.gif; *.tga\0\0");
    ofn.lpstrFile    = tszFileName;
    ofn.nMaxFile     = MAX_PATH;
    ofn.lpstrFileTitle = tszFileTitle;
    ofn.nMaxFileTitle  = MAX_PATH;
    ofn.lpstrTitle   = _T("Select a Source file...");
    ofn.Flags        = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY;

    if (!GetOpenFileName(&ofn))
        return S_FALSE;

	g_pTimeLine->SetFileName(uID==ST_LEFT ? 1 : 2, ofn.lpstrFile);

	if(uID < ST_COUNT)
	{
		g_pSourceImage[uID]->SetBitmap(ofn.lpstrFile);
	}

	return S_OK;
}
//---------------------------------------------------
// Name: SetTransition
// Desc: To set the selected transition
//---------------------------------------------------
HRESULT SetTransition()
{
    HWND hListBox = GetDlgItem(g_hMainWnd, IDC_LISTTRANS);
    int nTransition = (int) SendMessage(hListBox, LB_GETCURSEL, 0, 0);

    HRESULT hr = g_pTimeLine->SetTransition(nTransition);

    return hr;
}
//---------------------------------------------------
// Name: Play
// Desc: To play the transition rendering
//---------------------------------------------------
HRESULT Play()
{
    HRESULT hr;
    static bool bFirstRender = true;

    // Disable the "Preview" button while playing this transition
    EnablePreviewBtn(FALSE);

    g_Graph.Stop();

    g_pTimeLine->RenderTimeline();

    // Get the filter graph and set up the video window.
    if (bFirstRender) 
    {
        CComPtr<IGraphBuilder> pGraph;

        hr = g_pTimeLine->GetFilterGraph(&pGraph);

        g_Graph.SetFilterGraph(pGraph);
        g_Graph.SetVideoWindow(GetDlgItem(g_hMainWnd, IDC_VIEW));
        g_Graph.SetEventWindow(g_hMainWnd);

        bFirstRender = false;
    }

    // Run the graph.  When the playback completes, an EC_COMPLETE event 
    // will be generated and processed.
    return g_Graph.Run();
}
//---------------------------------------------------
// Name: Stop
// Desc: To stop the transition rendering
//---------------------------------------------------
void Stop()
{
}
//---------------------------------------------------
