/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSGUIManager.h
* ����ժҪ�� // VGS3GUI�Ĺ���������,ͨ������QuickGUIStore�ķ���ʵ��
* ����˵���� // 
* ��ǰ�汾�� // 
* ��    �ߣ� // 
* ������ڣ� // 
* 
* �޸�����        �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#ifndef _CGUIMANAGER_H_
#define _CGUIMANAGER_H_

#include "..\VGSHeader.h"
#include "..\VGSGetRoot.h"
#include "COverlay.h"
#include "CButton.h"
#include "CNavigator.h"
#include "background.h"

#include <algorithm>
#include <vector>
#include <sstream>
#include "MySTL.h"

#include "..\csysfunc.h"

using namespace std;

// GUI������
typedef struct GUIInfo
{
	DWORD           pWidget;		// �����ָ��
	CHAR			sName[MAX_NAMESTR_LENGTH];			// GUI���������
	VGS_GUIOBJ_TYPE guiType;        // gui���������

	FLOAT			Rotation;       // ��ת�Ƕ�
	int             Opacity;
	bool            bVisible;
 
	bool            bRelativeSize;  // �Ƿ�ʹ����Գߴ�
	int           Width;            // ͼ��Ŀ��
	int           Height;           // ͼ��ĸ߶�

	VGSOFFSETORIGIN    Origin;			// ����ʱ�Ĳο�ԭ�㣬OFFSETORIGINֵ 
	bool            bRelativePos;   // �Ƿ�ʹ�����λ��
	int           Left;			    // �������꣬������ӿ�Origin��
	int           Top;			    // ����꣬������ӿ�Origin��
	
	int             ZOrder;         // GUI����Ĳ�Σ�ֵԽ��Խ���ϲ�

}GUIInfo;


class VGS_EXPORT CGUIManager : public CVGSGetRoot
{
public:
	typedef vector<CGUIObject*> VecGuiPtr;
public:
	CGUIManager(LPDIRECT3DDEVICE9 pd3dDevice);
	~CGUIManager();

	void InitGUIStatus(int nVpWidth, int nVpHeight);
	// ��ճ�������
	void Clear();
	//!�Ƿ���GUI����(������ͼ��).
	inline bool HasObjects() { return !m_vecGui.empty(); }
	//!�Ƿ��б���ͼ
	inline bool HasBackDrop() { return (m_pGuiBackgroud != 0); }

	/** ������Ⱦ����GUI���󣨳�����ͼ��.
	*/
	void UpdateLayout();
	/** ������Ⱦ����ͼ.
	*/
	void RenderBackDrop();

	/** ͨ��ָ������ȡGUI����.
	@param hGui in:GUI����ľ��.
	@return ���ؾ����Ӧ��GUI����.
	*/
	CGUIObject* GetObjectByHandle(DWORD hGui);
	/** �õ�����·���GUI������������
	@param x/y  : �ӿ��е�����
	@param type out �� GUI��������ͣ��ο�VGS_GUIOBJ_TYPE����
	@return GUI�����ָ��
	*/
	CGUIObject* GetGUIObjectUnderLoc(int x, int y, VGS_GUIOBJ_TYPE &type);
	//!��ȡ����ͼָ��
	inline BackGround* GetBackDropPtr() { return m_pGuiBackgroud;}
	//!ɾ������ͼ
	inline void RemoveBackDrop() {	SAFE_DELETE(m_pGuiBackgroud); }

		
	/////////////////// ���÷��� ///////////////////////////////////

	// ��ȡ��ά����Ĺ�������
	void GetObjectCommonPropety(CGUIObject* pWidget, GUIInfo &info);

	/** ��ȡ�����Ѿ������Ķ�άԪ��(������ͼ��)�ľ���б�.
	@param vHandle out: GUI����Ľ������.
	*/
	//void GetAllObjectsList(vector <CGUIObject*> &vHandle);
	vector<CGUIObject*>& GetAllObjectsList();
	/** ��ȡ�����Ѿ������Ķ�άԪ��(������ͼ��)������.
	@return:����GUI����(������ͼ��)�ĸ���.
	*/
	inline UINT GetAllObjectsCount() { return m_vecGui.size(); }
	/** ��ȡĳ�����Ͷ�άԪ�صľ���б�.
	@param type in: GUI���������.
	@param vHandle out: GUI����Ľ������.
	*/
	void GetObjectsList(VGS_GUIOBJ_TYPE type, vector <CGUIObject*> &vHandle);
	/** ��ȡĳ�����͵�GUI���������.
	@param type in: GUI���������.
	@return:���ظ�����GUI����ĸ���.
	*/
	UINT GetObjectsCount(VGS_GUIOBJ_TYPE type);


	// ��ȡĳ�����Ͷ�άԪ�صľ��
	CGUIObject* GetObjectHandlebyIndex(VGS_GUIOBJ_TYPE type, UINT Index);
	// �������ֻ�ȡ���
	CGUIObject* GetObjectHandleByName(const std::string &sName);

	// ����GUI�����ZOrder��Խ���ʾGUI������Խ�ϲ�
	int GetObjectZOrder(CGUIObject* pWidget);	

	// GUI��������һ��
	void MoveObjectUp(CGUIObject* pWidget);

	// GUI��������һ��
	void MoveObjectDown(CGUIObject* pWidget);

	// GUI�����ƶ������
	void GotoObjectsTop(CGUIObject* pWidget);

	// GUI�����ƶ�����ײ�
	void GotoObjectsBottom(CGUIObject* pWidget);


	// ָ�����ƵĶ�ά�����Ƿ��Ѿ����ڡ�sVGSName�������VGS���ƣ�������quickgui�ڲ�����
	bool IsObjectExist(const std::string &sVGSName);


	////////////////////// end of ���÷��� //////////////////////////////////
	// ɾ��һ�����. Widget:�����ָ��
	void RemoveObject(CGUIObject* pWidget);
	/** ����һ��ǰ��ͼ
	@param szName in:ǰ��ͼ����.
	@param szBaseImg in:����ͼ���·��.
	@param szOpacImg in:͸��ͼ���·��.
	@return: �����򴴽�ʧ�ܷ���0.
	*/
	CGUIObject* CreateOverLayer(const char* szName, const char* szBaseImg = "", const char* szOpacImg = "");

	// ������ť
	CGUIObject* CreateButton(const char* sName, const RECT &rect, 
						const char* mouseUpImageFile = "",
						const char* mouseOverImageFile = "",
						const char* mouseDownImageFile = "");
	
	// ����������
	CGUIObject* CreateNavigator(const char* szName = "", const char* szBaseImg = "");

	// ����һ��������
	CGUIObject* CreateNaviPointer(const char* szName, const char* szBaseImg = "");

	// ����һ������ͼ, szBaseImg:ͼƬ�ļ�
	BackGround* CreateBackGroudLayer(const char* szBaseImg = "");
	/**����һ��Flash��GUI����.
	@param hParent in: Flash���ڵĸ�����ľ��.
	@param sName in: GUI���������.
	@param sFile in: swf�ļ�����ȫ·��.
	@param rect in:Ҫ������Flash GUI�������С.
	*/
	CATLFlashCtrl* CreateFlashCtrl(HWND hParent, const char* sName, const char* sFile, const RECT &rect);
	
	// ��ȡGUIManagerʹ�õĳ���
	DWORD GetSceneManager();
	// ����GUIManagerʹ�õĳ���
	void SetSceneManager(DWORD sceneManager);

	// �����ӿڴ�С
	void SetViewportSize(int width, int height);
	void GetViewportSize(int &width, int &height);

	/** ����һ��GUI����Ĳ�������.
	@param szGuiName in:GUI���������
	@return :������GUI����Ĳ�����.
	*/
	// static std::string GenerateGuiMtrlName(const char* szGuiName);

private:
	// d3d device
	LPDIRECT3DDEVICE9		m_pd3dDevice;  
	// �ӿڵĿ��
	int           m_vpWidth;   
	// �ӿڵĸ߶�
	int           m_vpHeight;     
	//����CScene�����ָ��
	DWORD m_SceneManager;

	//����ͼ����
	BackGround*	m_pGuiBackgroud;
	//������ͼ�������GUI����.
	VecGuiPtr m_vecGui;
};

// extern CGUIManager *g_pVGSGUIManager;


#endif