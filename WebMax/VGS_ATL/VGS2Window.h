// VGS2Window.h : CVGS2Window ������
//-------------------------------------------------------------------
#pragma once
//-------------------------------------------------------------------
#include "resource.h"       // ������
#include <atlhost.h>
#include <Mmsystem.h>
#include <exdisp.h> 
#include <comdef.h>
#include <vector>
#include <map>
// 
#include "VGSGetCore.h"

// //N3D�ļ�������ͷ�ļ�
#include ".\\N3D\\CDownLoadN3DFile.h"
// //�����ļ�������ͷ�ļ�
#include ".\\Download\\VGSFileDownloadManager.h"
#include ".\\Download\\FileLoadManager.h"

#include "InvokeJSFunc.h"
// 
// //N3D�ļ����ݽ�����ͷ�ļ�
// #include "GetN3DFileInfo_5.h"
// //Flash�ؼ�����ͷ�ļ�
// #include "ATLFlashCtrlUnit.h"
// //����VGS2����ͷ�ļ�
 #include "VGS2DLL.h"
//-------------------------------------------------------------------
class CVGS2Window : 
	public CAxDialogImpl<CVGS2Window>, public CVGSGetCore
{
public:
	//������������
	CVGS2Window(IOleClientSite* pClientSite, CRITICAL_SECTION* pRenderCS, CRITICAL_SECTION* pCtrlCS);
	~CVGS2Window();
	//-------------------------------------------------------
	enum { IDD = IDD_VGS2WINDOW };
	VOID SetVGS2Interface(CVGS2 *pVGS2);

	//-------------------------------------------------------
	//VGS������Ϣӳ��
	//-------------------------------------------------------
BEGIN_MSG_MAP(CVGS2Window)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUP)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDBClick)
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
	MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
	MESSAGE_HANDLER(MM_JOY1MOVE, OnMmJoyMove)
//	MESSAGE_HANDLER(WM_DOWNLOADN3D_PROC, OnDownLoadN3DProc)
//	MESSAGE_HANDLER(WM_DOWNLOADFILE_PROC, OnDownLoadFileProc)
//	MESSAGE_HANDLER(WM_JSFUNC_PROC, OnJSFuncProc);
//	MESSAGE_HANDLER(WM_ZTRIGFUNC_PROC,OnZTrigFuncProc);
//  MESSAGE_HANDLER(WM_CANCELMODE, OnCancelMode)
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
    MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp)

/*
	MESSAGE_HANDLER(VGS_DOWNLOAD_MSG_STATUSCHANGE, OnHttpDownLoad_ChangeStatus)
	MESSAGE_HANDLER(VGS_DOWNLOAD_MSG_DOWNLOADPROGRESS, OnHttpDownLoad_Progress)
	MESSAGE_HANDLER(VGS_DOWNLOAD_EVENT_FILE_DOWNLOADED, OnHttpDownLoad_FileDownloaded)
	MESSAGE_HANDLER(VGS_DOWNLOAD_EVENT_N3DONEIMG_DOWNLOADED, OnHttpDownLoad_N3DOneImageDownloaded)
	MESSAGE_HANDLER(VGS_DOWNLOAD_EVENT_N3DSCENE_DOWNLOADED, OnHttpDownLoad_N3DSceneDownloaded)
*/
	MESSAGE_HANDLER(WM_DOWNLOAD_MSG_FILE_DOWNLOADED, OnDownLoadFileCompletedMT);
	MESSAGE_HANDLER(WM_DOWNLOAD_MSG_DOWNLOADPROGRESS, OnDownLoadFilePercentMT);
	MESSAGE_HANDLER(WM_JSFUNC_MSG_SCENEISDOWN, OnSceneIsDownProc);
	MESSAGE_HANDLER(WM_JSFUNC_MSG_ALLISDOWN, OnAllIsDownProc);
END_MSG_MAP()
	//-------------------------------------------------------
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUP(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMmJoyMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDownLoadN3DProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDownLoadFileProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnJSFuncProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT OnZTrigFuncProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// http������Ӧ��Ϣ
	/*
	LRESULT OnHttpDownLoad_ChangeStatus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHttpDownLoad_Progress(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHttpDownLoad_FileDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHttpDownLoad_N3DOneImageDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHttpDownLoad_N3DSceneDownloaded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	*/	
	// ���߳�������Ϣ����
	LRESULT OnDownLoadFileCompletedMT(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDownLoadFilePercentMT(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSceneIsDownProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnAllIsDownProc(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//-------------------------------------------------------	


	//-------------------------------------------------------
	//VGS�������
	//-------------------------------------------------------
public:
    IOleClientSite*				m_pClientSite;					//�ͻ�������
// 	int							m_iWndWidth;					//���ڿ�
// 	int							m_iWndHeight;					//���ڸ�
// 	RECT						m_ClientRect;					//���ڿͻ�����Rect
// 
// 	HCURSOR						m_hHandCursor;					//���������
// 	HCURSOR						m_hDefaultCursor;				//�������ȱʡ
// 	POINT						m_CursorPos;					//��굱ǰ��λ��
// 	bool						m_bTerminated;					//��ǰ���߳��Ƿ���ֹ
// 	BOOL						m_bUnitedLogoStartDown;			//���˵�Logo�Ƿ��Ѿ���ʼ����

	//-------------------------------------------------------
	//ϵͳ����
	//-------------------------------------------------------
public:
 	HWND						m_IEhWnd;						//IE����ľ��
 	HWND						m_VGSWnd;						//VGS����ľ��
// 	DWORD						m_timer1;						//��ǰʱ��
// 	DWORD						m_timer2;						//��ǰʱ��
// 	DWORD						m_timeStep;						//��ǰ���õ���Ⱦʱ����
// 	DWORD						m_UserEventTimeStep;			//�û��Զ����¼�ʱ����
// 
 	BOOL						m_bActive;						//��ǰ�����Ƿ��Ѿ�����
// 	BOOL						m_bStepFinish;					//һ����Ⱦѭ������
// 
	//-------------------------------------------------------
	//����N3D�ļ���-CDownLoadN3DFile�����
	//-------------------------------------------------------
public:	
	//Load��N3D���ض����б�Load��N3D�ļ�ֱ�Ӽ��ص�����
 	vector<CDownLoadN3DFile*>   m_LoadN3DObj;	
	BOOL						m_bN3DFileDownloaded;
// 	
// 	//�����ļ������ض����б�, Ʃ���ⲿͼƬ������(��ʱ����)
//   	CDownLoadFile*				UnitedLogo_pDownLoadFile;
//     //����
// 	vector<CDownLoadFile*>      m_LoadFiles;
// 	//��־��ǰ̨���̨����N3D����
// 	INT							m_bDownType;
// 	CDownLoadN3DFile*			m_pCurN3D;						//��ǰ���ص�N3D����
// 
// 	DWORD						m_curDownSize;					//�Ѿ����ص�N3D���ݴ�С
 	DWORD						m_downText;						//down text
// 	INT							m_percent;						//�Ѿ����صİٷֱ�
// 	CHAR						m_strPercent[32];				//m_Percentת��Ϊ�ַ���	
// 	CHAR						m_strDownLoading[256];			//���ع�������ʾ���ַ���			
// 	BOOL						m_bGetSceneDataOK;				//���������Ƿ��Ѿ�������
// 
// 	BOOL						m_bCanConstructScene;			//�����Ƿ���Թ��쳡��	
// 	BOOL						m_AllIsOK;						//�����Ѿ�������ɲ��������	
// 	BOOL						m_DownedTexIndex;				//��ǰ���ص���ͼ���	
// 	BOOL						m_DownedImageIndex;				//��ǰ���ص���ͼ���

	//-------------------------------------------------------
	//VGS������ر���
	//-------------------------------------------------------
private:
 	// CVGS2DLL					m_VGS2;							//��ǰ��VGS2����
public:
	BOOL						m_ShowDefaultLogo;				//��ʾĬ�ϵ�VGS LOGO
// 	BOOL						m_IsCreasing;					//���ؿ�ʼʱ��VGS LOGOЧ����־λ
// 
// 	CONTROLSTATUS				m_controlStatus;				//��ǰ�Ŀ�������
// 
// 	CButton*					m_pButton;						//������һ���Ƴ��İ�ť
// 	COverlay*					m_pOverlay;						//������һ�ΰ��µ�Overlay
// 	CModel*						m_pCurCursorModel;				//��ǰ����·���ģ��
// 	CModel*						m_pLastCursorModel;				//������һ���Ƴ���model
// 	CModel*						m_pLastSelectedModel;			//���һ��ѡ���ģ��
// 	CModel*						m_pCurSelectedModel;			//��ǰѡ�е�ģ��

	//-------------------------------------------------------
	//Flash Ctrl��ر���
	//-------------------------------------------------------
public:
	//vector<CATLFlashCtrl*>		m_Flash;						//Flash�б�

	//-------------------------------------------------------
	//JavaScript������ر���
	//-------------------------------------------------------
public:
 	CInvokeJSFunc*				invokeJS;						//����Javascript�Ķ���	
// 	
// 	//VGS�������JS�ķ���
// 	BSTR						BSTR_EngineStart;				//�����ʼ������
// 	BSTR						BSTR_SceneIsDown;				//SceneIsDown�������ڳ����������ʱ�Զ�����	
// 	BSTR						BSTR_AllIsDown;					//AllIsDown�������ڳ�������ͼȫ���������ʱ����
// 	
// 	DISPPARAMS					dispparams;						//ִ��IE��JavaScript�����Ĳ���
// 
 	BSTR						BSTR_VGSMouseDown;				//VGSMouseUp�ַ���	
 	BSTR						BSTR_VGSMouseUp;				//VGSMouseUp�ַ���
 	BSTR						BSTR_VGSMouseMove;
 	BSTR						BSTR_VGSMouseDBClick;
// 
// 
 	BOOL						bEnable_MouseDown_Callback;
 	BOOL						bEnable_MouseUp_Callback;
 	BOOL						bEnable_MouseMove_Callback;
 	BOOL						bEnable_MouseDBClick_Callback;
// 
// 	//���ڱ�ʾһ���¼��еĴ����Ƿ��Ѿ�ִ���꣬
// 	//��ֹ��û��ִ����ʱ���еڶ��ε���
// 	BOOL						m_bMouseDownEvent_Finished;
// 	BOOL						m_bMouseUpEvent_Finished;
// 	BOOL						m_bMouseMoveEvent_Finished;
// 	BOOL						m_bMouseDBClickEvent_Finished;
// 	
// 
// 	//��ǰ����Ƿ�ѡ����Nav��Pointer
// 	BOOL						m_bNavPointerSelected;
// 	CNavigater*					m_pSelectedNavigater;
// 	NavPointer*					m_pSelectedNavPointer;
// 
// 	//��������ƶ��¼����Ƿ񼤻�ModelUnderLoc���
// 	BOOL						m_bModelEventOnMouseMove;
// 	//�Ҽ��˵���ʾ���
// 	BOOL                        m_bRightMenu;
// 
// 	HMENU                       hMainMenu;
// 	HMENU                       hPopMenu;
// 
// 
// 	//-------------------------------------------------------
// 	//����������ر���
// 	//-------------------------------------------------------
 	//��Ⱦ�߳�ͬ������ָ�룬��CVGS2�ഫ��	
 	CRITICAL_SECTION*			m_pRender_cs;		
 	//�����߳�ͬ������ָ�룬��CVGS2�ഫ��			
 	CRITICAL_SECTION*			m_pCtrl_cs;						
	
	//-------------------------------------------------------
	//��������ر���
	//-------------------------------------------------------
public:
// 	DWORD						modelCntStr;
// 	DWORD						texCntStr;
// 	DWORD						shaderCntStr;
// 	DWORD						triCntStr;
// 	DWORD						curCamFromStr;
// 	DWORD						curCamAtStr;
// 	
// 	char						str_shaderCnt[256];
// 	char						str_modelCnt[256];
// 	char						str_texCnt[256];
// 	char						str_triCnt[256];
// 	char						str_curCamFrom[256];
// 	char						str_curCamAt[256];


public:
	HRESULT						SetCurrentPath(const std::string &sCurrentPath);
	HRESULT                     AddFileToProgressbar(const std::string &sFileURL);
	HRESULT                     DownloadAndLoadN3DFile(const std::string &sURL, const std::string sN3DName);
	HRESULT                     DownloadAndLoadA3DFile(const std::string &sURL, const std::string sA3DName);
	HRESULT						ReadLocalN3DFile(CFileDwonloadInfo *pInfo);
	HRESULT						ReadLocalA3DFile(CFileDwonloadInfo *pInfo);
 	//��ʼ�����ֱ���
 	HRESULT						InitVars();
 	//��ʼ����ά����
 	HRESULT						InitScene();
// 	//��ʼ������̿��ư�ť
// 	VOID						InitControlButton();
 	//��ʼ����������
	VOID						InitOther();
// 	//��������Logo 
// 	VOID						CreateUnitedLogo(CDownLoadFile* pLogoDownLoadFile, RECT* pRect);
// 
// 	//���쵱ǰ����
// 	HRESULT						ConstructCurScene();
// 
// 	//�ж�������ĸ���ť�Ϸ�
// 	CButton*					GetMouseOverButton(INT x, INT y);
// 
// 	//�������ذٷֱȵ�����
// 	VOID						SetDownPercentText();
// 
// 	//ֹͣ���ͷ�N3D���ض���
// 	VOID						DestroyN3D();
// 	//ֹͣ���ͷ�filedownload���ض���
// 	VOID                        DestroyDownloadFile();
// 	//�ͷ�Flash��Դ
// 	VOID						DestroyFlash();
// 	//�õ���ʱĿ¼
// 	VOID						GetTmpFolder(char*  );
// 	//�Զ��������ƶ���Ϣ
// 	VOID						UserMouseMoveEvent();
// 	//Flash���ŵ����һ֡���¼�
// 	VOID						FlashPlayOverEvent();
// 	//Flash������ɵ��¼�
// 	VOID						FlashLoadOverEvent();
// 	//����FlashRect
// 	VOID						UpdateFlashRect();
// 	//Add by Baojiahui 2009/1/9-------------------------
// 	//����flash�е��ź�����Ϣ
// 	VOID                        UpdateFlashInfo();
// 	//��ʾ������ϸ��Ϣ
// 	VOID						ShowSceneDetailInfo();
// 	//Ϊ��������                
// 	VOID                        SortTheModelList();
// 	
// 	//ɾ���Ѿ�����������ض���
// 	void						EraseN3DDownloader(CDownLoadN3DFile* pN3D);
//     //ɾ���Ѿ�������������ļ�����
// 	void                        EraseFileDownloader(CDownLoadFile* pDFile);
// 	
// 	//------------------------------------------------------------------------------
// 	//Invoke JS �ĺ���
// 	//------------------------------------------------------------------------------
 	HRESULT				SceneIsDownInvoke(const std::string &sN3DName);
 	HRESULT				AllIsDownInvoke(const std::string &sN3DName);
// 	HRESULT				MouseDownInvoke();
// 	HRESULT				MouseUpInvoke();
 	HRESULT				MouseDBClickInvoke(int x, int y);
// 	HRESULT				MouseMoveInvoke();
// 	HRESULT				ButtonClickInvoke();
// 	HRESULT				DownloadInvoke(CHAR* strPercent);
// 
// 	LRESULT OnCancelMode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// 	{
// 		// TODO : Add Code for message handler. Call DefWindowProc if necessary.
		//return 0;
// 	}
	//---------------------------------------------------------------
public:
	// CVGS2DLL* getVGSObject(){return &m_VGS2;}
	// �ص�js�ű���ں�������ʼִ�нű�
	VOID EnableStartEngineCall();
	// �������濪ʼ�Ľű�
	LRESULT InvokeStartEngine();

	CVGSFileDownloadManager& GetDownloadN3DFileManager(){return m_DownloadFileManagerMT;}

	virtual VOID   SetVGSCore(CVGS2DLL *pVGSCore);
	//---------------------------------------------------------------
private:
	// �жϴ����Ƿ���flash����
	bool IsFlashWnd(HWND wnd);
	// ����һ��ͼƬ����
	void CreateN3DImageTexture(CVGSN3DFileDownloadStreamTask *pN3DTask, int imgIdx);
	// ��ǰ�Ѿ���ɵĽ���	
	DWORD	GetProgressbarFinishedBytes();				
	// ɾ��������
	void	DeleteProgressbar();
	// ���������ʾ
	void	ProcessProgress(float percent);
	//---------------------------------------------------------------
private:
	DISPPARAMS					dispparams;					//ִ��IE��JavaScript�����Ĳ���
	string						m_sCurrentURLPath;			// ��ǰURLĿ¼
	CVGSFileDownloadManager		m_DownloadFileManagerMT;	// �ļ��������������
	CFileLoadManager			m_FileLoadManagerMT;		// ������Դ�Ĺ�����
	vector<CFileDwonloadInfo*>	m_vFileDownloadInfoMT;		// ���߳����ص��ļ�������Ϣ
	set<string>					m_vFilesUseProgressbar;		// ʹ�ý��������ļ��б�
	DWORD						m_dwTotalProgressbar;       // ���������ܽ���
	//---------------------------------------------------------------
/*
 * For Stream downloading, don't support still
*/
/*
	// n3d ������Ϣ
	typedef struct CTaskCreateInfo
	{
		DWORD imageCount;
		set<int> setImageTobeCreated;
		set<int> setImageCreated;
		bool  bSceneCreated;	
		CTaskCreateInfo()
		{
			imageCount = 0;
			bSceneCreated = false;
		}
	};

	map<DWORD, CTaskCreateInfo> m_mapN3DTaskCreateInfo; // N3D������Ϣ
*/
};
//-------------------------------------------------------------------



