#ifndef MY_PLANE_H
#define MY_PLANE_H

#include <d3dx9.h>

const float EPSINON = 0.000001f;
/** �жϸ������Ƿ�Ϊ0.
*/
bool IsZero(float f);

/**	�Զ����ƽ����.
	����Ҫ�����ǣ�������ķ��ߺ�����һ��������ƽ��.
	author:����ȫ
	email:lxqmail@gmail.com
	date:2010-2-1
*/
class Plane : public D3DXPLANE
{
public:
	Plane(void);
	Plane(float a, float b, float c, float d);
	Plane(const D3DXVECTOR3& vPlaneNormal, const D3DXVECTOR3& vPoint);

	virtual ~Plane();

	const D3DXVECTOR3& GetNormal() const { return m_vPlaneNormal; }

protected:
	/** ƽ��ķ���
	*/
	D3DXVECTOR3	m_vPlaneNormal;
};

#endif
