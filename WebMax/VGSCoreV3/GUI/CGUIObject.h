/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // VGSGUIObject.h
* 内容摘要： // 二维对象公共基类定义及实现
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // 傅新波	林秀全
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
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


// 自定义的float类型Rect
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
	//选择框的8个小矩形的宽高
	/*const static */float SELBOX_WIDTH;
	/*const static */float SELBOX_HEIGHT;
public:
	CGUIObject(LPDIRECT3DDEVICE9 pd3dDevice, const char* szName);
	virtual ~CGUIObject();

	virtual void Destroy(){};

	virtual void Render() {};
	void RenderSelBox();

	//!判定一个坐标点是否位于gui范围内
	virtual bool IsInRect(int x, int y);
	/** 更新GUI的区域，用于位置、大小、参考坐标有变化时。
	*/
	virtual HRESULT UpdateRect(const RECT& rc) = 0;

	/** 获取材质某层纹理的图像文件的名称.只适用于每层纹理只有一张图像文件.
	@param str: 返回纹理层的图像文件的名称.
	@param pMtrl in:要操作的材质对象指针.
	@param nLayerIdx in:纹理层的索引.
	@return: .
	*/
	static bool GetTextureImageOfMtrl(std::string& str, CMaterial* pMtrl, int nLayerIdx = 0);
	/** 设置（或修改）材质某层纹理的图像文件.只适用于每层纹理只有一张图像文件.
	@param pMtrl in:要操作的材质对象指针.
	@param nLayerIdx in:纹理层的索引.
	@param szFile: 图像文件的文件名.
	@return : 返回是否设置成功.
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
	// 获取指定组件的相对位置或绝对位置——x,y是相对于m_OffsetOrigin的
	void GetPosition(INT &x, INT &y);
	//!x,y是相对于m_OffsetOrigin的位置
	void SetPosition(INT x, INT y) ;
	void GetSize(INT &width, INT &height);
	void SetSize(INT width, INT height) ;
	//!相对于m_OffsetOrigin的区域
	void GetRect(RECT &rc) ;
	void SetRect(const RECT &rc);
	//!获取绝对区域
	inline void GetAbsoluteRect(RECT& rc) { rc = m_rc;}
	//!设置绝对区域
	void SetAbsoluteRect(const RECT &rc) ;

	void SetOffsetOrigin(VGSOFFSETORIGIN offsetOrigin) ;

	// 设置选择框是否可见
	inline void  SetShowSelBox(bool bShow) { m_bShowSelBox = bShow; }	
	inline bool GetShowSelBox() 	{	return m_bShowSelBox;}
	// 获取对象类型
	inline VGS_GUIOBJ_TYPE GetType() { return m_ObjType; }
	inline void SetType(VGS_GUIOBJ_TYPE typ) { m_ObjType = typ; }

	/** 设置视口的宽高——单位像素
	@param nWith in:
	@param nHeight in:
	*/
	void SetViewportSize(int nWidth, int nHeight);

	/** 设置GUI的角度
	@param fAngle in:以弧度为单位的角度
	@return : S_OK返回正确.
	*/
	virtual HRESULT SetRotation(float fAngle) = 0;
	float getRotation(){return m_fAngle;}

	int getZOrder(){return m_zOrder;}
	void setZOrder(int zorder){m_zOrder = zorder;}

public://设置与获取对象的Lua回调函数
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

	// 设置事件函数名称
	void SetEventFuncName(VGS_MOUSEEVENT_TYPE type, const std::string &sFuncName);
	// 获取事件函数名称
	const std::string& GetEventFuncName(VGS_MOUSEEVENT_TYPE type);

protected://motheds

	/** 根据相对坐标计算GUI对象的真实位置，适用于视口大小变化时。
	*/
	void CalcAbsolutePosition();
	/** 根据相对坐标计算GUI对象的真实大小，适用于视口大小变化时。
	*/
	void CalcAbsoluteSize();
	//获取相应坐标原点的绝对位置
	void GetPosByOffsetOrigin(int& nLeft, int& nTop);

	/** 计算GUI对象的相对位置的比例值，用于GUI位置变化时.
	@param dwGUI in:GUI对象的指针句柄.
	*/
	void CalcRelativePos();
	/** 计算GUI对象的相对大小的比例值，用于GUI大小变化时.
	@param dwGUI in:GUI对象的指针句柄.
	*/
	void CalcRelativeSize();

	//!初始化选择框
	virtual void InitSelBoxBuf();
	//!据一个2D点生成一个选择框的数据
	void GenerateSelBoxByPoint(int x, int y);
	void UpdateSelBox();

protected://Data Members
	//!D3d设置
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	
	//!顶点格式FVF 
	DWORD			m_FVF;			
	//!一个顶点数据占用的空间大小
	UINT			m_FVFSize;      
	//!Overlay所使用的基本材质	
	CMaterial*		m_pMtrl;
	
	//!对象的名称
	std::string     m_strName; 
	
	// 对象的类型
	VGS_GUIOBJ_TYPE         m_ObjType;        

	//--postion and size---
	//!对齐视口的参考原点
	VGSOFFSETORIGIN m_OffsetOrigin;
	//!GUI的绝对区域
	RECT		m_rc;
	//!是否使用相对位置
	bool          m_bRelativePos;
	//!是否使用相对大小
	bool		  m_bRelativeSize;
	float         m_fWidth;
	float         m_fHeight;
	//!相对位置X
	float         m_fTop;
	//!相对位置Y
	float         m_fLeft;          	 

	//!透明度 [0, 100]
	int				m_nOpacity;
	//!角度
	float			m_fAngle;
	//!是否可见
	bool			m_bVisible;
	//!是否有效
	bool			m_bEnable;
	//!是否用于滤镜
	bool			m_bIsFilter;
	//!是否显示选择框
	bool			m_bShowSelBox;

	int             m_zOrder;   // z order

	//!选择框4个顶点，共有8个这样的小矩形
	VERTEXDECL_POSRHWCOLORCOORD1	m_SelBox[4];
	//!选择框顶点缓冲区
	LPDIRECT3DVERTEXBUFFER9 m_pSelBoxVbuf;
	//!选择框索引缓冲区
	LPDIRECT3DINDEXBUFFER9	m_pSelBoxIbuf;

	//!视口宽度
	int	m_nVpWidth;
	//!视口高度
	int m_nVpHeight;

protected://保存GUI对象的Lua事件的回调函数名称
	std::string   m_sMouseOverFunc;
	std::string   m_sMouseDBClickFunc;
	std::string   m_sMouseLDownFunc;
	std::string   m_sMouseLUpFunc;
	std::string   m_sMouseRDownFunc;
	std::string   m_sMouseRUpFunc;
	std::string   m_sMouseMoveFunc;
	std::string   m_sMouseLeaveFunc;

};

//!GUI类型的函数对象
class VGS_EXPORT GuiTypeFunctor
{
public:
	GuiTypeFunctor(VGS_GUIOBJ_TYPE type):_type(type){}
	bool operator () (CGUIObject* ptr) { return (ptr->GetType() == _type);}
private:
	VGS_GUIOBJ_TYPE _type;
};
//!GUI名称的函数对象
class VGS_EXPORT GuiNameFunctor
{
public:
	GuiNameFunctor(const std::string & name):_name(name){}
	bool operator () (CGUIObject* ptr) { return (_name == ptr->GetName());}
private:
	std::string _name;
};


#endif
