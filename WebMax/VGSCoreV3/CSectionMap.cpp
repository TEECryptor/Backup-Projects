// CSectionMap.cpp: implementation of the CSectionMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSectionMap.h"

#include "CScene.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSectionMap::CSectionMap(DWORD pScene)
{
	m_pSectionFace = NULL;
	m_pImage = NULL;
	m_pImageTexture = NULL;

	m_pSectionOriTexture = NULL;
	m_pSectionTexture = NULL;

	m_pScene = pScene;

	index = -1;
}

CSectionMap::~CSectionMap()
{

}


//创建剖面图对象
HRESULT CSectionMap::Create(CModel* pModel,						//切片模型
						   D3DXVECTOR3* leftTopCorner,			//被切模型侧面的左上角
						   D3DXVECTOR3* rightTopCorner,			//被切模型侧面的右上角
						   D3DXVECTOR3* leftBottomCorner,		//被切模型侧面的左下角
						   D3DXVECTOR3* rightBottomCorner,
						   INT width, INT height)						
{
	//切片模型
/*	m_pSectionFace = pModel;
	//侧面的四个顶点
	m_LeftTopPoint = *leftTopCorner;
	m_RightTopPoint = *rightTopCorner;
	m_LeftBottomPoint = *leftBottomCorner;
	m_RightBottomPoint = *rightBottomCorner;

	//创建用于保存生成的剖面图的贴图对象
	return CreateSectionTexture(width, height);*/
	return S_OK;
}

//创建用于保存生成的剖面图的贴图对象
HRESULT CSectionMap::CreateSectionTexture(INT width, INT height)
{
	/*m_pSectionTexture = new CTexture();
	//name
	strcpy(m_pSectionTexture->name, "Section_Texture"); 
	CHAR strIndex[8];
	itoa(index, strIndex, 10);
	strcat(m_pSectionTexture->name, strIndex);
	//width,height
	m_pSectionTexture->width = width;
	m_pSectionTexture->height = height;
	//mipLevel
	m_pSectionTexture->mipCount = 1;
	//Format
	m_pSectionTexture->BP = D3DFMT_R8G8B8;

	return (D3DXCreateTexture(((CScene*)m_pScene)->m_pd3dDevice,
								m_pSectionTexture->width, 
								m_pSectionTexture->height, 
								m_pSectionTexture->mipCount, 
								0, 
								m_pSectionTexture->BP,
								D3DPOOL_MANAGED,
								&m_pSectionTexture->textureBuf));

*/
	return S_OK;
}

//计算上边线和切面交点、下边线和切面的交点所对应的UV坐标
BOOL CSectionMap::CalSectionPoint()
{
/*
	//计算上边线和切面的交点
	CScene* pScene = (CScene*)m_pScene;
	D3DXVECTOR3 orig = m_RightTopPoint;
	D3DXVECTOR3 dir = m_LeftTopPoint - m_RightTopPoint;
	D3DXVec3Normalize(&dir, &dir);
	D3DXVECTOR3 normal;
	INT	faceIndex;
	FLOAT distance;
	FLOAT su, sv;

	BOOL result1 = pScene->IntersectModel(&orig, &dir, m_pSectionFace, &normal, &faceIndex,
							&su,
							&sv,
							&distance);
	if (!result1)
		return FALSE;

	//上边线交点
	m_topIntersectPoint = orig + distance * dir;
	//交点在上边线上的相对位置(0-1)
	FLOAT leftDis = D3DXVec3Length(&(m_topIntersectPoint - m_LeftTopPoint));
	FLOAT rightDis = D3DXVec3Length(&(m_RightTopPoint - m_topIntersectPoint));
	m_topIntersectPercent = leftDis/(leftDis + rightDis);
	
	//边长
	FLOAT topAllDis = D3DXVec3Length(&(m_RightTopPoint - m_LeftTopPoint));
	if (leftDis > topAllDis || rightDis > topAllDis)
		return FALSE;

	//计算下边线和切面的交点
	orig = m_RightBottomPoint;
	dir = m_LeftBottomPoint - m_RightBottomPoint;
	D3DXVec3Normalize(&dir, &dir);
	BOOL result2 = pScene->IntersectModel(&orig, &dir, m_pSectionFace, 
											&normal, &faceIndex,
											&su,
											&sv,
											&distance);

	if (!result2)
		return FALSE;

	//下边线交点
	m_bottomIntersectPoint = orig + distance * dir;
	//交点在下边线上的相对位置(0-1)
	leftDis = D3DXVec3Length(&(m_bottomIntersectPoint - m_LeftBottomPoint));
	rightDis = D3DXVec3Length(&(m_RightBottomPoint - m_bottomIntersectPoint));
	m_bottomIntersectPercent = leftDis/(leftDis + rightDis);

	//边长
	FLOAT bottomAllDis = D3DXVec3Length(&(m_RightBottomPoint - m_LeftBottomPoint));
	if (leftDis > bottomAllDis || rightDis > bottomAllDis)
		return FALSE;
	*/
	return TRUE;
}

//计算交点对应的侧面图像素位置
VOID CSectionMap::CalImagePixelPos()
{
/*
	m_IS_Top_PixelX = m_topIntersectPercent * m_pImageTexture->width;
	m_IS_Top_PixelY = 0;

	m_IS_Bottom_PixelX = m_bottomIntersectPercent * m_pImageTexture->width;
	m_IS_Bottom_PixelY = m_pImageTexture->height;
	*/
}

//找到剖面贴图的像素对应的侧面图的像素编号
DWORD CSectionMap::GetImagePixelIndex(INT line)
{
/*
	FLOAT yPercent = 1.0f * line / m_pSectionTexture->height;

	INT pixelY = yPercent * m_pImageTexture->height;

	FLOAT step = (m_IS_Bottom_PixelX - m_IS_Top_PixelX) * 1.0f / 
							(m_IS_Bottom_PixelY - m_IS_Top_PixelY);

	//找到Image对应的行号
	INT	imageLineIndex = yPercent * m_pImageTexture->height;

	INT pixelX = m_IS_Top_PixelX + imageLineIndex * step;
	
	return (pixelY * m_pImageTexture->width + pixelX);
	*/
	return 0;
}

//生成剖面图到切面
HRESULT CSectionMap::WriteSectionTexture()
{
/*
	//找到侧面图像的对应位置像素的颜色，填充到剖面图
	D3DLOCKED_RECT imageRect;
	if (FAILED(m_pImageTexture->textureBuf->LockRect(0, &imageRect, NULL, D3DLOCK_DISCARD)))
		return E_FAIL;
	BYTE* pImageTexBuf = (BYTE*)imageRect.pBits;

	D3DLOCKED_RECT TexRect;
	if (FAILED(m_pSectionTexture->textureBuf->LockRect(0, &TexRect, NULL, D3DLOCK_DISCARD)))
		return E_FAIL;
	BYTE* pSectionTexBuf = (BYTE*)TexRect.pBits;
	
	for (INT i = 0; i<m_pSectionTexture->height; i++)
	{	
		//找到对应的ImageTexture像素编号
		INT imageIndex = GetImagePixelIndex(i);

		//找到ImageTexture指定编号的像素颜色
		INT red = pImageTexBuf[4*imageIndex + 0];
		INT green = pImageTexBuf[4*imageIndex + 1];
		INT blue = pImageTexBuf[4*imageIndex + 2];
		INT alpha = 255;

		for (INT j = 0; j<m_pSectionTexture->width; j++)
		{
			//SectionTexture的像素编号
			INT index = 4 * (m_pSectionTexture->width * i + j);
			
			pSectionTexBuf[index + 0] = red;
			pSectionTexBuf[index + 1] = green;
			pSectionTexBuf[index + 2] = blue;
			pSectionTexBuf[index + 3] = alpha;
		}
	}

	//unlock texture rect
	m_pSectionTexture->textureBuf->UnlockRect(0);
	//unlock texture rect
	m_pImageTexture->textureBuf->UnlockRect(0);
*/
	return S_OK;
}

//执行切割,生成剖面图到m_pSectionTexture
BOOL CSectionMap::Slice()
{

	/*
	//计算上边线和切面交点、下边线和切面的交点
	if (!CalSectionPoint())
	{
		ResetSectionTexture();
		return FALSE;
	}

	//找到侧面图像对应的两个像素点坐标
	CalImagePixelPos();
	if (m_IS_Top_PixelX < 0 || m_IS_Top_PixelX > m_pImageTexture->width || 
		m_IS_Bottom_PixelX < 0 || m_IS_Bottom_PixelX > m_pImageTexture->width)
	{
		ResetSectionTexture();
		return FALSE;
	}
		

	//改写剖面图的贴图对象的内容
	WriteSectionTexture();

	
	ShowSectionTexture();
*/	
	return TRUE;
}

//显示剖面图
VOID CSectionMap::ShowSectionTexture()
{
	//m_pSectionFace->shader->diffuseTex = m_pSectionTexture;
}

//重置切面贴图到原贴图
VOID CSectionMap::ResetSectionTexture()
{
//	m_pSectionFace->shader->diffuseTex = m_pSectionOriTexture;
}

//释放资源
VOID CSectionMap::Release()
{
}
