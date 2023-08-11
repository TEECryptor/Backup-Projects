#pragma  once
#include "..\\VGSHeader.h"
#include "N3D_DefV3.h"

#include "N3DLoaderImpV2.h"

using namespace N3D;
using namespace N3DV3;

typedef struct CCameraN3DTransform
{
	std::string sName;
	D3DXVECTOR3 v3Pos, v3At, v3Up;
	// float fRoll;

	CCameraN3DTransform()
	{
		v3Pos = D3DXVECTOR3(0, 0, 0);
		v3At  = D3DXVECTOR3(0, 0, 1);
		v3Up = D3DXVECTOR3(0, 1, 0);
		// fRoll = 0.0f;
	}
}CCameraN3DTransform;

// ����v3.0�����N3D�ļ�
class VGS_EXPORT CN3DLoaderImpV3 : public CN3DLoaderImpV2
{
public:
	CN3DLoaderImpV3();
	~CN3DLoaderImpV3();

protected:
	// ���ļ�ͷ
	virtual void        ReadHeader();
	//��ȡģ����Ϣ
	virtual VOID        ReadModelData();
	virtual VOID		ReadLightData();
	virtual VOID		ReadCameraData();
	virtual VOID		ReadMaterialData();

	// ��ȡoverlay��Ϣ
	virtual VOID		ReadOverlayData();
	//��ȡNavigate��Ϣ
	virtual VOID		ReadNavigaterData();
	//��ȡButton��Ϣ
	virtual VOID		ReadButtonData();

	virtual VOID        ReadControlData();

	// ��ȡ����ͼ
	virtual VOID		ReadSceneGraph();

protected:
	// ��ȡ������һ��overlay��Ϣ
	COverlay * ReadandCreateAOverlay(VGS_GUIOBJ_TYPE guiType, BYTE **pBuffer);

private:

	//���쳡��ͼ
	VOID ConstructSceneGraph(CSceneNode* parentNode, N3D_Node* pNode, BYTE** offset);

	vector <CCameraN3DTransform> m_vCamN3DTransform;
	CCameraN3DTransform getCameraParamByName(const std::string &sName);

protected:
	N3D_HeaderV3		m_Header;
};