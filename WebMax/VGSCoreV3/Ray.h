#ifndef MY_RAY_H
#define MY_RAY_H

#include <d3dx9.h>

class Plane;

/**	自定义的射线类.
	主要功能，判定与面相交并获取交点.
	author:林秀全
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

	/** 构建特定视口中，屏幕坐标处的射线.射线此时在视图空间中.
	@param vp in: 视口对象.
	@param matProj in:投影矩阵.
	@param nScreenX in:屏幕坐标
	@param nScreenY in:
	*/
	void BuildWithSceenPos(const D3DVIEWPORT9& vp, const D3DXMATRIX& matProj, int nScreenX, int nScreenY);
	/** 把射线变换到特定坐标系中.
	@param mat in:变换矩阵.
	*/
	void Transfrom(const D3DXMATRIX& mat);

	/** 判断射线与平面相交.
	@param plane in:平面对象.
	@return:true,则相交.
	*/
	bool IntersectPlane(const Plane& plane);
	/** 获取平面与射线的交点（如果相交的话）.
	@param plane in:平面对象.
	@param pt out: 交点.
	@return :true,则相交.
	*/
	bool GetIntersectPoint(const Plane& plane, D3DXVECTOR3& pt);
	/** 射线原点到平面交点的距离.
	@return :true,则相交.
	*/
	bool DistanceUptoPlane(const Plane& plane, float& distance);
protected:
	/** 射线原点.
	*/
	D3DXVECTOR3	m_vOrigin;
	/** 射线方向.
	*/
	D3DXVECTOR3 m_vDirection;
};

#endif
