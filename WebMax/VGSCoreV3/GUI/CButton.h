/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // VGSGUIButton.h
* 内容摘要： // 按钮类定义及实现
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // 傅新波
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
* ---------------------------------------------------------------
* 
*
*********************************************************************/


#pragma once

#include "COverlay.h"

#include <vector>

using namespace std;

class VGS_EXPORT CButton : public COverlay
{
public:

	CButton(LPDIRECT3DDEVICE9 pd3dDevice, const char* sName);
	~CButton();

	/**获得按钮的旧接口的JS回调函数名称
	@return callBackFunc out : 按钮事件的回调函数名称
	*/
	const char * GetJsCallbackFunc();
	/**设置按钮的旧接口的JS回调函数名称
	@param sFunc int : 按钮事件的回调函数名称
	*/
	void SetJsCallbackFunc(const char * sFunc);

	/**获得按钮使用的图像文件
	@param type       in  : 0-按钮正常状态(up)使用的图片，1-按下(down)时使用的图片，2-鼠标悬停(over)时使用的图片
	@param sImageFile out : 图片文件名，包含完整路径
	*/
	//void GetImageFile(int type, char* sImageFile);

	/**设置按钮使用的图像文件
	@param type       in : 0-按钮正常状态(up)使用的图片，1-按下(down)时使用的图片，2-鼠标悬停(over)时使用的图片
	@param sImageFile in : 图片文件名，包含完整路径
	*/
	// void SetImageFile(int type, const char* sImageFile);


	// 设置材质  Type : 0-按钮正常状态(up)使用的材质，1-按下(down)时使用的材质，2-鼠标悬停(over)时使用的材质
	VOID SetMaterial(INT Type, CMaterial* pMaterial);
	// 获取材质
	CMaterial* GetMaterial(INT Type);  // 0-按钮正常状态(up)使用的材质，1-按下(down)时使用的材质，2-鼠标悬停(over)时使用的材质

	bool GetMouseUpImageFileName(std::string& str);
	bool SetMouseUpImageFileName(const char* szFile);
	bool GetMouseDownImageFileName(std::string& str);
	bool SetMouseDownImageFileName(const char* szFile);
	bool GetMouseOverImageFileName(std::string& str);
	bool SetMouseOverImageFileName(const char* szFile);

private:
	/* 基本材质使用基类的m_pMtrl */
	//!鼠标进入时的材质
	CMaterial*   m_pMouseOverMtrl;
	//!鼠标按下时的材质
	CMaterial*   m_pMouseDownMtrl;

	std::string	m_strJsCallbackFunc;
	std::vector<std::string> m_vCallBackFunc;  // 回调函数名称列表
};