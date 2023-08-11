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


//��������ͼ����
HRESULT CSectionMap::Create(CModel* pModel,						//��Ƭģ��
						   D3DXVECTOR3* leftTopCorner,			//����ģ�Ͳ�������Ͻ�
						   D3DXVECTOR3* rightTopCorner,			//����ģ�Ͳ�������Ͻ�
						   D3DXVECTOR3* leftBottomCorner,		//����ģ�Ͳ�������½�
						   D3DXVECTOR3* rightBottomCorner,
						   INT width, INT height)						
{
	//��Ƭģ��
/*	m_pSectionFace = pModel;
	//������ĸ�����
	m_LeftTopPoint = *leftTopCorner;
	m_RightTopPoint = *rightTopCorner;
	m_LeftBottomPoint = *leftBottomCorner;
	m_RightBottomPoint = *rightBottomCorner;

	//�������ڱ������ɵ�����ͼ����ͼ����
	return CreateSectionTexture(width, height);*/
	return S_OK;
}

//�������ڱ������ɵ�����ͼ����ͼ����
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

//�����ϱ��ߺ����潻�㡢�±��ߺ�����Ľ�������Ӧ��UV����
BOOL CSectionMap::CalSectionPoint()
{
/*
	//�����ϱ��ߺ�����Ľ���
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

	//�ϱ��߽���
	m_topIntersectPoint = orig + distance * dir;
	//�������ϱ����ϵ����λ��(0-1)
	FLOAT leftDis = D3DXVec3Length(&(m_topIntersectPoint - m_LeftTopPoint));
	FLOAT rightDis = D3DXVec3Length(&(m_RightTopPoint - m_topIntersectPoint));
	m_topIntersectPercent = leftDis/(leftDis + rightDis);
	
	//�߳�
	FLOAT topAllDis = D3DXVec3Length(&(m_RightTopPoint - m_LeftTopPoint));
	if (leftDis > topAllDis || rightDis > topAllDis)
		return FALSE;

	//�����±��ߺ�����Ľ���
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

	//�±��߽���
	m_bottomIntersectPoint = orig + distance * dir;
	//�������±����ϵ����λ��(0-1)
	leftDis = D3DXVec3Length(&(m_bottomIntersectPoint - m_LeftBottomPoint));
	rightDis = D3DXVec3Length(&(m_RightBottomPoint - m_bottomIntersectPoint));
	m_bottomIntersectPercent = leftDis/(leftDis + rightDis);

	//�߳�
	FLOAT bottomAllDis = D3DXVec3Length(&(m_RightBottomPoint - m_LeftBottomPoint));
	if (leftDis > bottomAllDis || rightDis > bottomAllDis)
		return FALSE;
	*/
	return TRUE;
}

//���㽻���Ӧ�Ĳ���ͼ����λ��
VOID CSectionMap::CalImagePixelPos()
{
/*
	m_IS_Top_PixelX = m_topIntersectPercent * m_pImageTexture->width;
	m_IS_Top_PixelY = 0;

	m_IS_Bottom_PixelX = m_bottomIntersectPercent * m_pImageTexture->width;
	m_IS_Bottom_PixelY = m_pImageTexture->height;
	*/
}

//�ҵ�������ͼ�����ض�Ӧ�Ĳ���ͼ�����ر��
DWORD CSectionMap::GetImagePixelIndex(INT line)
{
/*
	FLOAT yPercent = 1.0f * line / m_pSectionTexture->height;

	INT pixelY = yPercent * m_pImageTexture->height;

	FLOAT step = (m_IS_Bottom_PixelX - m_IS_Top_PixelX) * 1.0f / 
							(m_IS_Bottom_PixelY - m_IS_Top_PixelY);

	//�ҵ�Image��Ӧ���к�
	INT	imageLineIndex = yPercent * m_pImageTexture->height;

	INT pixelX = m_IS_Top_PixelX + imageLineIndex * step;
	
	return (pixelY * m_pImageTexture->width + pixelX);
	*/
	return 0;
}

//��������ͼ������
HRESULT CSectionMap::WriteSectionTexture()
{
/*
	//�ҵ�����ͼ��Ķ�Ӧλ�����ص���ɫ����䵽����ͼ
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
		//�ҵ���Ӧ��ImageTexture���ر��
		INT imageIndex = GetImagePixelIndex(i);

		//�ҵ�ImageTextureָ����ŵ�������ɫ
		INT red = pImageTexBuf[4*imageIndex + 0];
		INT green = pImageTexBuf[4*imageIndex + 1];
		INT blue = pImageTexBuf[4*imageIndex + 2];
		INT alpha = 255;

		for (INT j = 0; j<m_pSectionTexture->width; j++)
		{
			//SectionTexture�����ر��
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

//ִ���и�,��������ͼ��m_pSectionTexture
BOOL CSectionMap::Slice()
{

	/*
	//�����ϱ��ߺ����潻�㡢�±��ߺ�����Ľ���
	if (!CalSectionPoint())
	{
		ResetSectionTexture();
		return FALSE;
	}

	//�ҵ�����ͼ���Ӧ���������ص�����
	CalImagePixelPos();
	if (m_IS_Top_PixelX < 0 || m_IS_Top_PixelX > m_pImageTexture->width || 
		m_IS_Bottom_PixelX < 0 || m_IS_Bottom_PixelX > m_pImageTexture->width)
	{
		ResetSectionTexture();
		return FALSE;
	}
		

	//��д����ͼ����ͼ���������
	WriteSectionTexture();

	
	ShowSectionTexture();
*/	
	return TRUE;
}

//��ʾ����ͼ
VOID CSectionMap::ShowSectionTexture()
{
	//m_pSectionFace->shader->diffuseTex = m_pSectionTexture;
}

//����������ͼ��ԭ��ͼ
VOID CSectionMap::ResetSectionTexture()
{
//	m_pSectionFace->shader->diffuseTex = m_pSectionOriTexture;
}

//�ͷ���Դ
VOID CSectionMap::Release()
{
}
