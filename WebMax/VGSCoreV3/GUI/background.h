#ifndef __BACK_GROUND_H__
#define __BACK_GROUND_H__
/** 背景图类定义 
@purpose:背景图单独进行定义，以利于以后扩展.
@date:2009-4-22
@author:linxq
*/
#include "..\\VGSHeader.h"
#include "COverlay.h"

class VGS_EXPORT BackGround : public COverlay
{
public:
	BackGround(LPDIRECT3DDEVICE9 pd3dDevice);
	~BackGround();

	void GetBaseImage(std::string& strImg);
	void SetBaseImage(const std::string& strImg);
	/** 让背景图全屏显示.
	*/
	void FullScreen();
	/** 设置背景图的区域，相对于视口的比例值.
	*/
	void SetRectF(const FRect& rc);
	void SetRect(float left, float top, float width, float height);

	/** 获取背景图的相对区域.
	*/
	FRect GetRectF();
	void GetRect(float &left, float &top, float &width, float &height);

};

#endif
