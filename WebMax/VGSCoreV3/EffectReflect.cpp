#include "EffectReflect.h"
#include "CModel.h"

EffectReflect::EffectReflect(LPDIRECT3DDEVICE9 pDevice, const std::string &strName, UINT nTargetWidth, UINT nTargetHeight)
	:m_pD3dDevice(pDevice)
	,m_pEffect(NULL)
	,m_nTargetWidth(nTargetWidth)
	,m_nTargetHeight(nTargetHeight)
{
	m_sName = strName;

}

EffectReflect::~EffectReflect()
{
	Release();
}

bool EffectReflect::Init(CModel* pReflectiveModelPlane)
{
	if (NULL == pReflectiveModelPlane)
		return false;
	if (FAILED(D3DXCreateTexture(m_pD3dDevice, m_nTargetWidth, m_nTargetHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &m_pTargetTexture)))
		return false;

	//D3DXEffect
	ID3DXBuffer* pErrBuf = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(m_pD3dDevice, "reflection.fx", NULL, NULL, NULL, NULL, &m_pEffect, &pErrBuf);
	if (pErrBuf)
	{
		std::string str = "Effect::CreateFromFile():";
		str += (char*)(pErrBuf->GetBufferPointer());
		SAFE_RELEASE_D3D(pErrBuf);
	}
	if (FAILED(hr))
	{
		std::string str = "Effect::CreateFromFile():";
		str += "create failed--";
		return false;
	}
	//Get Defualt valid technique
	D3DXHANDLE hTechnique;
	m_pEffect->FindNextValidTechnique(NULL, &hTechnique);
	m_pEffect->SetTechnique(hTechnique);

	//����ƽ��
	m_pRefPlaneModel = pReflectiveModelPlane;
	D3DXPLANE refPlane;
	//�������Y�����������ϵ����
	D3DXVECTOR3 normal;
	D3DXVec3TransformNormal(&normal, &D3DXVECTOR3(0, 1, 0), &m_pRefPlaneModel->pNode->curTransform.worldMatrix);
	//���÷���ƽ��
	D3DXPlaneFromPointNormal(&refPlane, &m_pRefPlaneModel->boundingSphere.Center, &normal);
	//���㷴�����
	D3DXMatrixReflect(&m_matReflective, &refPlane);

	return true;
}
void EffectReflect::Release()
{
	SAFE_RELEASE_D3D(m_pTargetTexture);
	SAFE_RELEASE_D3D(m_pEffect);
}
void EffectReflect::AddReflectedModel(CModel* pModel)
{
	m_vecReflectedModels.push_back(pModel);
}

void EffectReflect::Update()
{
	//safe old target
	//�����ӽǱ任
	D3DXMATRIX matView, matViewSaved;
	m_pD3dDevice->GetTransform( D3DTS_VIEW, &matViewSaved );
	D3DXMatrixMultiply(&matView, &m_matReflective, &matViewSaved);
	//m_pD3dDevice->SetTransform( D3DTS_VIEW, &matView );

	//render objects
	std::vector<CModel*>::iterator it = m_vecReflectedModels.begin(), end = m_vecReflectedModels.end();
	for (; it != end; ++it)
	{

	}

	//retstore target
}