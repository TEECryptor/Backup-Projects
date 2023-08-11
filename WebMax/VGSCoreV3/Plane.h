#ifndef MY_PLANE_H
#define MY_PLANE_H

#include <d3dx9.h>

const float EPSINON = 0.000001f;
/** 判断浮点数是否为0.
*/
bool IsZero(float f);

/**	自定义的平面类.
	其主要功能是，根据面的法线和面上一点来描述平面.
	author:林秀全
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
	/** 平面的法线
	*/
	D3DXVECTOR3	m_vPlaneNormal;
};

#endif
