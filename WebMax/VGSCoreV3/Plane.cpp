#include "Plane.h"

#include <math.h>
bool IsZero(float f)
{
	if(f < EPSINON  && f > - EPSINON)
		return true;
	return false;
}

Plane::Plane()
	:D3DXPLANE(0.0f, 0.0f, 0.0f, 0.0f)
{
	m_vPlaneNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
Plane::Plane(float a, float b, float c, float d)
{
	D3DXVec3Normalize(&m_vPlaneNormal, &D3DXVECTOR3(a,b,c));

	a = m_vPlaneNormal.x;
	b = m_vPlaneNormal.y;
	c = m_vPlaneNormal.z;
}
Plane::Plane(const D3DXVECTOR3 &vPlaneNormal, const D3DXVECTOR3 &vPoint)
{
	D3DXVec3Normalize(&m_vPlaneNormal, &vPlaneNormal);
	d = - D3DXVec3Dot(&m_vPlaneNormal, &vPoint);

	a = m_vPlaneNormal.x;
	b = m_vPlaneNormal.y;
	c = m_vPlaneNormal.z;
}

Plane::~Plane()
{

}
