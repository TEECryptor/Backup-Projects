/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSGUIObject.h
* ����ժҪ�� // ��ά���󹫹����ඨ�弰ʵ��
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²�	����ȫ
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 2009-04-03
* 2009-05-06
*
*********************************************************************/
#ifndef __VGS_GUI_OBJECT_H__
#define __VGS_GUI_OBJECT_H__

#include "..\VGSHeader.h"
#include "..\vgsgetroot.h"

#include <string>


// �Զ����float����Rect
// struct RectF
// {
// 	float left,top,right,bottom;
// 	RectF():left(0.0f),top(0.0f),right(0.0f),bottom(0.0f){}
// 	RectF(float _l, float _t, float _r, float _b):left(_l),top(_t),right(_r),bottom(_b){}
// 	float Width() { return right - left; }
// 	float Height() { return bottom - top; }
// };

class VGS_EXPORT CGUIObject : public CVGSGetRoot
{
public:
	//ѡ����8��С���εĿ��
	/*const static */float SELBOX_WIDTH;
	/*const static */float SELBOX_HEIGHT;
public:
	CGUIObject(LPDIRECT3DDEVICE9 pd3dDevice, const char* szName);
	virtual ~CGUIObject();

	virtual void Destroy(){};

	virtual void Render() {};
	void RenderSelBox();

	//!�ж�һ��������Ƿ�λ��gui��Χ��
	virtual bool IsInRect(int x, int y);
	/** ����GUI����������λ�á���С���ο������б仯ʱ��
	*/
	virtual HRESULT UpdateRect(const RECT& rc) = 0;

	/** ��ȡ����ĳ�������ͼ���ļ�������.ֻ������ÿ������ֻ��һ��ͼ���ļ�.
	@param str: ����������ͼ���ļ�������.
	@param pMtrl in:Ҫ�����Ĳ��ʶ���ָ��.
	@param nLayerIdx in:����������.
	@return: .
	*/
	static bool GetTextureImageOfMtrl(std::string& str, CMaterial* pMtrl, int nLayerIdx = 0);
	/** ���ã����޸ģ�����ĳ�������ͼ���ļ�.ֻ������ÿ������ֻ��һ��ͼ���ļ�.
	@param pMtrl in:Ҫ�����Ĳ��ʶ���ָ��.
	@param nLayerIdx in:����������.
	@param szFile: ͼ���ļ����ļ���.
	@return : �����Ƿ����óɹ�.
	*/
	static bool SetMtrlImageFileName(CMaterial* pMtrl, int nLayerIdx = 0, const char* szFile = "");

public://getter--setter 
	inline const char * GetName() { return m_strName.c_str(); }
	inline void SetName(const char* szName) { m_strName = szName; }
	CMaterial* GetMaterial() { return m_pMtrl; }
	void SetMaterial(CMaterial* pMtrl) { m_pMtrl = pMtrl; }
	inline bool GetVisibility() {	return m_bVisible;	}
	virtual void SetVisibility(bool bVisible) {	m_bVisible = bVisible;	}
	inline int	GetOpacity()	{	return m_nOpacity/* * 100 /255*/;	}
	virtual void SetOpacity(int nOpactity) = 0;
	inline bool GetEnable() {	return m_bEnable;	}	
	inline void SetEnable(bool bEnable)	{	m_bEnable = bEnable;	}
	inline bool GetFilterEnable()	{	return m_bIsFilter;	}
	inline void SetFilterEnable(bool bFilter)	{	m_bIsFilter = bFilter;	}	

	inline bool GetUsedRelativePos() { return m_bRelativePos; }
	inline bool GetUsedRelativeSize() { return m_bRelativeSize; }
	inline VGSOFFSETORIGIN GetOffsetOrigin() {	return m_OffsetOrigin; }

	void SetUsedRelativePos(bool bUseRelative) ;
	void SetUsedRelativeSize(bool bUseRelative); 
	// ��ȡָ����������λ�û����λ�á���x,y�������m_OffsetOrigin��
	void GetPosition(INT &x, INT &y);
	//!x,y�������m_OffsetOrigin��λ��
	void SetPosition(INT x, INT y) ;
	void GetSize(INT &width, INT &height);
	void SetSize(INT width, INT height) ;
	//!�����m_OffsetOrigin������
	void GetRect(RECT &rc) ;
	void SetRect(const RECT &rc);
	//!��ȡ��������
	inline void GetAbsoluteRect(RECT& rc) { rc = m_rc;}
	//!���þ�������
	void SetAbsoluteRect(const RECT &rc) ;

	void SetOffsetOrigin(VGSOFFSETORIGIN offsetOrigin) ;

	// ����ѡ����Ƿ�ɼ�
	inline void  SetShowSelBox(bool bShow) { m_bShowSelBox = bShow; }	
	inline bool GetShowSelBox() 	{	return m_bShowSelBox;}
	// ��ȡ��������
	inline VGS_GUIOBJ_TYPE GetType() { return m_ObjType; }
	inline void SetType(VGS_GUIOBJ_TYPE typ) { m_ObjType = typ; }

	/** �����ӿڵĿ�ߡ�����λ����
	@param nWith in:
	@param nHeight in:
	*/
	void SetViewportSize(int nWidth, int nHeight);

	/** ����GUI�ĽǶ�
	@param fAngle in:�Ի���Ϊ��λ�ĽǶ�
	@return : S_OK������ȷ.
	*/
	virtual HRESULT SetRotation(float fAngle) = 0;
	float getRotation(){return m_fAngle;}

	int getZOrder(){return m_zOrder;}
	void setZOrder(int zorder){m_zOrder = zorder;}

public://�������ȡ�����Lua�ص�����
	const std::string&  GetMouseOverFuncName(){return m_sMouseOverFunc;}
	void  SetMouseOverFuncName(const std::string &sFuncName){m_sMouseOverFunc = sFuncName;}
	const std::string&  GetMouseDBClickFuncName(){return m_sMouseDBClickFunc;}
	void  SetMouseDBClickFuncName(const std::string &sFuncName){m_sMouseDBClickFunc = sFuncName;}
	const std::string&  GetMouseMoveFuncName(){return m_sMouseMoveFunc;}
	void  SetMouseMoveFuncName(const std::string &sFuncName){m_sMouseMoveFunc = sFuncName;}
	const std::string&  GetMouseLeaveFuncName(){return m_sMouseLeaveFunc;}
	void  SetMouseLeaveFuncName(const std::string &sFuncName){m_sMouseLeaveFunc = sFuncName;}
	const std::string&  GetMouseLDwonFuncName(){return m_sMouseLDownFunc;}
	void  SetMouseLDwonFuncName(const std::string &sFuncName){m_sMouseLDownFunc = sFuncName;}
	const std::string&  GetMouseLUpFuncName(){return m_sMouseLUpFunc;}
	void  SetMouseLUpFuncName(const std::string &sFuncName){m_sMouseLUpFunc = sFuncName;}
	const std::string&  GetMouseRDownFuncName(){return m_sMouseRDownFunc;}
	void  SetMouseRDownFuncName(const std::string &sFuncName){m_sMouseRDownFunc = sFuncName;}
	const std::string&  GetMouseRUpFuncName(){return m_sMouseRUpFunc;}
	void  SetMouseRUpFuncName(const std::string &sFuncName){m_sMouseRUpFunc = sFuncName;}

	// �����¼���������
	void SetEventFuncName(VGS_MOUSEEVENT_TYPE type, const std::string &sFuncName);
	// ��ȡ�¼���������
	const std::string& GetEventFuncName(VGS_MOUSEEVENT_TYPE type);

protected://motheds

	/** ��������������GUI�������ʵλ�ã��������ӿڴ�С�仯ʱ��
	*/
	void CalcAbsolutePosition();
	/** ��������������GUI�������ʵ��С���������ӿڴ�С�仯ʱ��
	*/
	void CalcAbsoluteSize();
	//��ȡ��Ӧ����ԭ��ľ���λ��
	void GetPosByOffsetOrigin(int& nLeft, int& nTop);

	/** ����GUI��������λ�õı���ֵ������GUIλ�ñ仯ʱ.
	@param dwGUI in:GUI�����ָ����.
	*/
	void CalcRelativePos();
	/** ����GUI�������Դ�С�ı���ֵ������GUI��С�仯ʱ.
	@param dwGUI in:GUI�����ָ����.
	*/
	void CalcRelativeSize();

	//!��ʼ��ѡ���
	virtual void InitSelBoxBuf();
	//!��һ��2D������һ��ѡ��������
	void GenerateSelBoxByPoint(int x, int y);
	void UpdateSelBox();

protected://Data Members
	//!D3d����
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	
	//!�����ʽFVF 
	DWORD			m_FVF;			
	//!һ����������ռ�õĿռ��С
	UINT			m_FVFSize;      
	//!Overlay��ʹ�õĻ�������	
	CMaterial*		m_pMtrl;
	
	//!���������
	std::string     m_strName; 
	
	// ���������
	VGS_GUIOBJ_TYPE         m_ObjType;        

	//--postion and size---
	//!�����ӿڵĲο�ԭ��
	VGSOFFSETORIGIN m_OffsetOrigin;
	//!GUI�ľ�������
	RECT		m_rc;
	//!�Ƿ�ʹ�����λ��
	bool          m_bRelativePos;
	//!�Ƿ�ʹ����Դ�С
	bool		  m_bRelativeSize;
	float         m_fWidth;
	float         m_fHeight;
	//!���λ��X
	float         m_fTop;
	//!���λ��Y
	float         m_fLeft;          	 

	//!͸���� [0, 100]
	int				m_nOpacity;
	//!�Ƕ�
	float			m_fAngle;
	//!�Ƿ�ɼ�
	bool			m_bVisible;
	//!�Ƿ���Ч
	bool			m_bEnable;
	//!�Ƿ������˾�
	bool			m_bIsFilter;
	//!�Ƿ���ʾѡ���
	bool			m_bShowSelBox;

	int             m_zOrder;   // z order

	//!ѡ���4�����㣬����8��������С����
	VERTEXDECL_POSRHWCOLORCOORD1	m_SelBox[4];
	//!ѡ��򶥵㻺����
	LPDIRECT3DVERTEXBUFFER9 m_pSelBoxVbuf;
	//!ѡ�������������
	LPDIRECT3DINDEXBUFFER9	m_pSelBoxIbuf;

	//!�ӿڿ��
	int	m_nVpWidth;
	//!�ӿڸ߶�
	int m_nVpHeight;

protected://����GUI�����Lua�¼��Ļص���������
	std::string   m_sMouseOverFunc;
	std::string   m_sMouseDBClickFunc;
	std::string   m_sMouseLDownFunc;
	std::string   m_sMouseLUpFunc;
	std::string   m_sMouseRDownFunc;
	std::string   m_sMouseRUpFunc;
	std::string   m_sMouseMoveFunc;
	std::string   m_sMouseLeaveFunc;

};

//!GUI���͵ĺ�������
class VGS_EXPORT GuiTypeFunctor
{
public:
	GuiTypeFunctor(VGS_GUIOBJ_TYPE type):_type(type){}
	bool operator () (CGUIObject* ptr) { return (ptr->GetType() == _type);}
private:
	VGS_GUIOBJ_TYPE _type;
};
//!GUI���Ƶĺ�������
class VGS_EXPORT GuiNameFunctor
{
public:
	GuiNameFunctor(const std::string & name):_name(name){}
	bool operator () (CGUIObject* ptr) { return (_name == ptr->GetName());}
private:
	std::string _name;
};


#endif
