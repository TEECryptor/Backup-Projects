#ifndef EFFECT_REFLECT_H
#define EFFECT_REFLECT_H

#include "VGSHeader.h"
#include "ResourceManager.h"

/**
@purpose:
	创建基于HLSL的反射纹理层.<br/>
	在使用前应检测硬件对shader的支持.<br/>
@email:lxqmail@gmail.com
*/
class EffectReflect : public CResource
{
public:
	EffectReflect(LPDIRECT3DDEVICE9 pDevice, const std::string& strName, UINT nTargetWidth, UINT nTargetHeight);
	virtual ~EffectReflect();

	bool Init(CModel* pReflectiveModelPlane);
	void Release();

	void AddReflectedModel(CModel* pModel);

	/** Render Objects to Target.
	*/
	void Update();

	LPDIRECT3DTEXTURE9 GetTexture() const { return m_pTargetTexture; }

protected:
	/**D3D Device对象
	*/
	LPDIRECT3DDEVICE9	m_pD3dDevice;
	/**D3DXEffect对象打针.
	*/
	LPD3DXEFFECT		m_pEffect;

	LPDIRECT3DTEXTURE9	m_pTargetTexture;
	LPDIRECT3DSURFACE9	m_pOldRenderTarget;

	/**渲染目标的宽高，单位像素.
	*/
	UINT	m_nTargetWidth, m_nTargetHeight;

	/**反射平面.
	*/
	D3DXPLANE	m_reflectPlane;
	/**确定反射平面的模型.
	*/
	CModel*		m_pRefPlaneModel;
	/**反射变换矩阵
	*/
	D3DXMATRIX m_matReflective;

	/**被反射的模型的列表.
	*/
	std::vector<CModel*>	m_vecReflectedModels;
};
#endif
