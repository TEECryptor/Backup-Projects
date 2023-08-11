// 剖面图对象
// 该类用于创建剖面图，用于隧道工程土层信息剖面图
// 用一个面片模型去切一个土层模型，假设土层模型是Box型，
// 土层信息的在水平方向是变化的，而在纵深方向是不变的，
// 则剖面一定是规则矩形。
// 通过一幅侧面的图像来描述土层结构，然后通过切面的在Box上切位置来
// 计算对应的图像像素点。
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSECTIONMAP_H__28181F80_076B_49DF_8241_32079DFD488C__INCLUDED_)
#define AFX_CSECTIONMAP_H__28181F80_076B_49DF_8241_32079DFD488C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CModel.h"
#include "CImage.h"

class VGS_EXPORT CSectionMap  
{
private:	
	DWORD					m_pScene;

public:	
	CHAR					name[32];	
	INT						index;		

	//切片模型
	CModel*					m_pSectionFace;	
	
	//侧面的四个顶点，这四个顶点用于和侧面图像计算对应贴图坐标
	D3DXVECTOR3				m_LeftTopPoint;
	D3DXVECTOR3				m_RightTopPoint;
	D3DXVECTOR3				m_LeftBottomPoint;
	D3DXVECTOR3				m_RightBottomPoint;

	//侧面图像的Image对象
	CImage*					m_pImage;

	//侧面图像创建的贴图对象
	CTexture*				m_pImageTexture;

	//交点
	D3DXVECTOR3				m_topIntersectPoint;
	D3DXVECTOR3				m_bottomIntersectPoint;

	//交点在边线方向上的比例
	FLOAT					m_topIntersectPercent;
	FLOAT					m_bottomIntersectPercent;

	//上交点对应侧面图的像素位置
	INT						m_IS_Top_PixelX;
	INT						m_IS_Top_PixelY;

	//下交点对应侧面图的像素位置
	INT						m_IS_Bottom_PixelX;
	INT						m_IS_Bottom_PixelY;

	//切片的原贴图，用于暂存切片的原贴图
	CTexture*				m_pSectionOriTexture;

	//执行剖切，剖面图创建的贴图对象
	CTexture*				m_pSectionTexture;

public:
	//创建剖面图对象
	HRESULT					Create(CModel* pModel,						//切片模型
								   D3DXVECTOR3* leftTopCorner,			//被切模型侧面的左上角
								   D3DXVECTOR3* rightTopCorner,			//被切模型侧面的右上角
								   D3DXVECTOR3* leftBottomCorner,		//被切模型侧面的左下角
								   D3DXVECTOR3* rightBottomCorner,
								   INT width, INT height);		

	//创建用于保存剖面图的贴图对象
	HRESULT					CreateSectionTexture(INT width, INT height);

	//执行切割,生成剖面图到m_pSectionTexture
	BOOL					Slice();
	
	//计算上边线和切面交点、下边线和切面的交点
	BOOL					CalSectionPoint();

	//计算交点对应的侧面图像素位置
	VOID					CalImagePixelPos();

	//找到剖面贴图的像素对应的侧面图的像素编号
	DWORD					GetImagePixelIndex(INT line);

	//生成剖面图到切面
	HRESULT					WriteSectionTexture();

	//显示剖面图
	VOID					ShowSectionTexture();

	//重置切面贴图到原贴图
	VOID					ResetSectionTexture();

	//释放资源
	VOID					Release();

public:
	CSectionMap(DWORD pScene);
	virtual ~CSectionMap();

};

#endif // !defined(AFX_CSECTIONMAP_H__28181F80_076B_49DF_8241_32079DFD488C__INCLUDED_)
