/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSGUINavigator.h
* ����ժҪ�� // ����ͼ�ඨ�弰ʵ��
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²�
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#pragma once

#include "COverlay.h"

#include <vector>

using namespace std;

//����ָ���ඨ��
class VGS_EXPORT CNavPointer : public COverlay
{
private:
	//!0��ʾ�����������1��ʾ��ģ�͵���
	INT			NavType;
	//!�����ģ�͵�ָ����
	DWORD		PointedHandle;

	//@!��ָ������ڵ���ͼ��λ��
	int			m_nRelativeLeft;
	int			m_nRelativeTop;
	
public:
	CNavPointer(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName) : COverlay(pd3dDevice, sName),m_nRelativeLeft(0),m_nRelativeTop(0)
	{
		m_ObjType = VGS_GUIOBJ_TYPE_NAVIPOINTOR;
		::SetRect(&m_rc, 0, 0, 20, 20);		
	}

	inline int GetNavType() { return NavType; }
	inline void SetNavType(int navType) { NavType = navType; }
	inline DWORD GetPointerHandle() { return PointedHandle; }
	inline void SetPointerHandle(DWORD dwHandle) { PointedHandle = dwHandle; }
	inline void GetRelativePosition(int& x, int& y) { x = m_nRelativeLeft; y = m_nRelativeTop; }
	inline void SetRelativePosition(int x, int y) { m_nRelativeLeft = x; m_nRelativeTop = y; }
};

class VGS_EXPORT CNavigator : public COverlay
{
public:
	typedef vector<CNavPointer*> VecNavPointerPtr;
public:
	CNavigator(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName);
	~CNavigator();
	
	//!���ص���ͼ����Ⱦ
	virtual void Render();
	//!���ص���ͼ�ľ��θ��¡�����Ҫ����ʵ�ֵ���ͼ����ʱҲ���µ�����
	virtual HRESULT UpdateRect(const RECT& rc);

	bool IsExist(const std::string &sVGSName);
	// ���/ɾ��������
	// ���/ɾ��������
	void AddNavPointer(CNavPointer* ptr);
	CNavPointer* AddNavPointer(const std::string &sName, const std::string &sFileName = "", INT pointerType = 0, DWORD pointedHandle = 0);
	CNavPointer* CloneNavPointer(const std::string &sName, const std::string &sReferencePointer);
	void deleteNavPointer(const std::string &sName);

	//!��õ���������
	inline int  GetPointerCount() { return m_vNavPointer.size(); }
	//!ͨ��������ȡ����������ָ��
	CNavPointer* GetPointerByIndex(int idx);
	//!ͨ��������ȡ��������������
	std::string GetPointerName(INT index);
	//!ͨ�������ȡ����������ָ��
	CNavPointer* GetPointerByHandle(DWORD dwHandle);
	//!ͨ�����ƻ�ȡ����������ָ��
	CNavPointer* GetPointerByName(const std::string& name);

	// ���µ�����Χ
	VOID SetNavRange(const FRect& rc);
	inline VOID GetNavRange(FRect& rc) { rc = m_rcNavScene; }

	// ���µ�����ͼ
	VOID SetNavMap(const std::string &sNavImageFile);
	// �õ���������ͼʹ�õ�ͼ������
	std::string GetNavMap();

	// ���µ�����λ��
	VOID UpdatePointers();
	
	void UpdateMap(RECT& rc);
protected:
	//!������ָ�������
	VecNavPointerPtr m_vNavPointer;
	//!�����ĵ�����Χ
	FRect m_rcNavScene;

};