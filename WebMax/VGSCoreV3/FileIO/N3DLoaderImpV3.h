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

// 加载v3.0引擎的N3D文件
class VGS_EXPORT CN3DLoaderImpV3 : public CN3DLoaderImpV2
{
public:
	CN3DLoaderImpV3();
	~CN3DLoaderImpV3();

protected:
	// 读文件头
	virtual void        ReadHeader();
	//读取模型信息
	virtual VOID        ReadModelData();
	virtual VOID		ReadLightData();
	virtual VOID		ReadCameraData();
	virtual VOID		ReadMaterialData();

	// 读取overlay信息
	virtual VOID		ReadOverlayData();
	//读取Navigate信息
	virtual VOID		ReadNavigaterData();
	//读取Button信息
	virtual VOID		ReadButtonData();

	virtual VOID        ReadControlData();

	// 读取场景图
	virtual VOID		ReadSceneGraph();

protected:
	// 读取并创建一个overlay信息
	COverlay * ReadandCreateAOverlay(VGS_GUIOBJ_TYPE guiType, BYTE **pBuffer);

private:

	//构造场景图
	VOID ConstructSceneGraph(CSceneNode* parentNode, N3D_Node* pNode, BYTE** offset);

	vector <CCameraN3DTransform> m_vCamN3DTransform;
	CCameraN3DTransform getCameraParamByName(const std::string &sName);

protected:
	N3D_HeaderV3		m_Header;
};