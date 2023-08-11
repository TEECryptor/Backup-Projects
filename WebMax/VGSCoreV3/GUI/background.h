#ifndef __BACK_GROUND_H__
#define __BACK_GROUND_H__
/** ����ͼ�ඨ�� 
@purpose:����ͼ�������ж��壬�������Ժ���չ.
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
	/** �ñ���ͼȫ����ʾ.
	*/
	void FullScreen();
	/** ���ñ���ͼ������������ӿڵı���ֵ.
	*/
	void SetRectF(const FRect& rc);
	void SetRect(float left, float top, float width, float height);

	/** ��ȡ����ͼ���������.
	*/
	FRect GetRectF();
	void GetRect(float &left, float &top, float &width, float &height);

};

#endif
