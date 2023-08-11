// ����ͼ����
// �������ڴ�������ͼ�������������������Ϣ����ͼ
// ��һ����Ƭģ��ȥ��һ������ģ�ͣ���������ģ����Box�ͣ�
// ������Ϣ����ˮƽ�����Ǳ仯�ģ�����������ǲ���ģ�
// ������һ���ǹ�����Ρ�
// ͨ��һ�������ͼ������������ṹ��Ȼ��ͨ���������Box����λ����
// �����Ӧ��ͼ�����ص㡣
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

	//��Ƭģ��
	CModel*					m_pSectionFace;	
	
	//������ĸ����㣬���ĸ��������ںͲ���ͼ������Ӧ��ͼ����
	D3DXVECTOR3				m_LeftTopPoint;
	D3DXVECTOR3				m_RightTopPoint;
	D3DXVECTOR3				m_LeftBottomPoint;
	D3DXVECTOR3				m_RightBottomPoint;

	//����ͼ���Image����
	CImage*					m_pImage;

	//����ͼ�񴴽�����ͼ����
	CTexture*				m_pImageTexture;

	//����
	D3DXVECTOR3				m_topIntersectPoint;
	D3DXVECTOR3				m_bottomIntersectPoint;

	//�����ڱ��߷����ϵı���
	FLOAT					m_topIntersectPercent;
	FLOAT					m_bottomIntersectPercent;

	//�Ͻ����Ӧ����ͼ������λ��
	INT						m_IS_Top_PixelX;
	INT						m_IS_Top_PixelY;

	//�½����Ӧ����ͼ������λ��
	INT						m_IS_Bottom_PixelX;
	INT						m_IS_Bottom_PixelY;

	//��Ƭ��ԭ��ͼ�������ݴ���Ƭ��ԭ��ͼ
	CTexture*				m_pSectionOriTexture;

	//ִ�����У�����ͼ��������ͼ����
	CTexture*				m_pSectionTexture;

public:
	//��������ͼ����
	HRESULT					Create(CModel* pModel,						//��Ƭģ��
								   D3DXVECTOR3* leftTopCorner,			//����ģ�Ͳ�������Ͻ�
								   D3DXVECTOR3* rightTopCorner,			//����ģ�Ͳ�������Ͻ�
								   D3DXVECTOR3* leftBottomCorner,		//����ģ�Ͳ�������½�
								   D3DXVECTOR3* rightBottomCorner,
								   INT width, INT height);		

	//�������ڱ�������ͼ����ͼ����
	HRESULT					CreateSectionTexture(INT width, INT height);

	//ִ���и�,��������ͼ��m_pSectionTexture
	BOOL					Slice();
	
	//�����ϱ��ߺ����潻�㡢�±��ߺ�����Ľ���
	BOOL					CalSectionPoint();

	//���㽻���Ӧ�Ĳ���ͼ����λ��
	VOID					CalImagePixelPos();

	//�ҵ�������ͼ�����ض�Ӧ�Ĳ���ͼ�����ر��
	DWORD					GetImagePixelIndex(INT line);

	//��������ͼ������
	HRESULT					WriteSectionTexture();

	//��ʾ����ͼ
	VOID					ShowSectionTexture();

	//����������ͼ��ԭ��ͼ
	VOID					ResetSectionTexture();

	//�ͷ���Դ
	VOID					Release();

public:
	CSectionMap(DWORD pScene);
	virtual ~CSectionMap();

};

#endif // !defined(AFX_CSECTIONMAP_H__28181F80_076B_49DF_8241_32079DFD488C__INCLUDED_)
