#ifndef EFFECT_REFLECT_H
#define EFFECT_REFLECT_H

#include "VGSHeader.h"
#include "ResourceManager.h"

/**
@purpose:
	��������HLSL�ķ��������.<br/>
	��ʹ��ǰӦ���Ӳ����shader��֧��.<br/>
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
	/**D3D Device����
	*/
	LPDIRECT3DDEVICE9	m_pD3dDevice;
	/**D3DXEffect�������.
	*/
	LPD3DXEFFECT		m_pEffect;

	LPDIRECT3DTEXTURE9	m_pTargetTexture;
	LPDIRECT3DSURFACE9	m_pOldRenderTarget;

	/**��ȾĿ��Ŀ�ߣ���λ����.
	*/
	UINT	m_nTargetWidth, m_nTargetHeight;

	/**����ƽ��.
	*/
	D3DXPLANE	m_reflectPlane;
	/**ȷ������ƽ���ģ��.
	*/
	CModel*		m_pRefPlaneModel;
	/**����任����
	*/
	D3DXMATRIX m_matReflective;

	/**�������ģ�͵��б�.
	*/
	std::vector<CModel*>	m_vecReflectedModels;
};
#endif
