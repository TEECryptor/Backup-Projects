#ifndef MY_RAY_H
#define MY_RAY_H

#include <d3dx9.h>

class Plane;

/**	�Զ����������.
	��Ҫ���ܣ��ж������ཻ����ȡ����.
	author:����ȫ
	email:lxqmail@gmail.com
	date:2010-2-1
	update: 2010-2-2
*/
class Ray
{
public:
	Ray(void);
	Ray(const D3DXVECTOR3& vOrigin, const D3DXVECTOR3& vDir);

	virtual ~Ray();

	const D3DXVECTOR3& GetOrigin() const { return m_vOrigin; }
	const D3DXVECTOR3& GetDirection() const { return m_vDirection; }

	/** �����ض��ӿ��У���Ļ���괦������.���ߴ�ʱ����ͼ�ռ���.
	@param vp in: �ӿڶ���.
	@param matProj in:ͶӰ����.
	@param nScreenX in:��Ļ����
	@param nScreenY in:
	*/
	void BuildWithSceenPos(const D3DVIEWPORT9& vp, const D3DXMATRIX& matProj, int nScreenX, int nScreenY);
	/** �����߱任���ض�����ϵ��.
	@param mat in:�任����.
	*/
	void Transfrom(const D3DXMATRIX& mat);

	/** �ж�������ƽ���ཻ.
	@param plane in:ƽ�����.
	@return:true,���ཻ.
	*/
	bool IntersectPlane(const Plane& plane);
	/** ��ȡƽ�������ߵĽ��㣨����ཻ�Ļ���.
	@param plane in:ƽ�����.
	@param pt out: ����.
	@return :true,���ཻ.
	*/
	bool GetIntersectPoint(const Plane& plane, D3DXVECTOR3& pt);
	/** ����ԭ�㵽ƽ�潻��ľ���.
	@return :true,���ཻ.
	*/
	bool DistanceUptoPlane(const Plane& plane, float& distance);
protected:
	/** ����ԭ��.
	*/
	D3DXVECTOR3	m_vOrigin;
	/** ���߷���.
	*/
	D3DXVECTOR3 m_vDirection;
};

#endif
