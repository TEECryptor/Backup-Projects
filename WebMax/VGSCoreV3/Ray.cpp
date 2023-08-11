#include "Ray.h"

#include "Plane.h"

Ray::Ray()
	:m_vOrigin(0.0f, 0.0f, 0.0f)
	,m_vDirection(1.0f, 1.0f, 1.0f)
{
}
Ray::Ray(const D3DXVECTOR3& vOrigin, const D3DXVECTOR3& vDir)
	:m_vOrigin(vOrigin)
	,m_vDirection(vDir)
{
}

Ray::~Ray()
{
}
void Ray::BuildWithSceenPos(const D3DVIEWPORT9& vp, const D3DXMATRIX& matProj, int nScreenX, int nScreenY)
{
	float px = 0.0f;
	float py = 0.0f;

	px = ((( 2.0f*nScreenX) / vp.Width)  - 1.0f) / matProj(0, 0);
	py = (((-2.0f*nScreenY) / vp.Height) + 1.0f) / matProj(1, 1);

	m_vOrigin    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vDirection = D3DXVECTOR3(px, py, 1.0f);
}
void Ray::Transfrom(const D3DXMATRIX& mat)
{
	D3DXVec3TransformCoord(&m_vOrigin, &m_vOrigin, &mat);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &mat);
	// normalize
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
}

bool Ray::IntersectPlane(const Plane& plane)
{
	float dist;
	return DistanceUptoPlane(plane, dist);
}
bool Ray::GetIntersectPoint(const Plane& plane, D3DXVECTOR3& pt)
{
	float dist;
	if (!DistanceUptoPlane(plane, dist))
		return false;
	
	pt  = m_vOrigin + dist * m_vDirection;

	return true;
}

bool Ray::DistanceUptoPlane(const Plane& plane, float& distance)
{
	float theta = D3DXVec3Dot(&plane.GetNormal(), &m_vDirection);
	if (IsZero(theta))
		return false;

	float t1 = D3DXVec3Dot(&plane.GetNormal(), &m_vOrigin);

	distance = -(plane.d + t1) / theta;

	return (distance >= 0.0f);
}
