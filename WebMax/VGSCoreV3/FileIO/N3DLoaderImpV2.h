// GetN3DFileInfo_5.h: interface for the CGetN3DFileInfo_5 class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////
// ����: CGetN3DFileInfo_5_1-- �����ļ���ȡ��
// ����: ��ȡN3D�ļ���N3D���壬�������������ݵ�Ԥ��Ⱦ������
// ����: Baojiahui	
// ����: 2007-10-19
////////////////////////////////////////////////////////////////////////

//����N3D��E3D��ѹ���ļ��������ļ���ѹ��Ľṹ��V3D��һ���ģ���V3D�ļ��ṹ����
#include "..\\VGSHeader.h"
#include "N3D_Def.h"
#include "..\\CScene.h"
#include "..\\SceneResource.h"
#include "..\\gui\CGUIManager.h"
#include "N3DLoaderImpBase.h"
#include "..\\SceneResource.h"
#include "..\\CLight.h"
#include <vector>

#include "..\\VGSGetRoot.h"

using namespace std;
using namespace N3D;

typedef struct N3DOverlayData
{
	BOOL        bCreated;
	N3D_Overlay overlayInfo;
	N3DOverlayData(BOOL b, N3D_Overlay info)
	{
		bCreated = b;
		overlayInfo = info;
	}
}N3DOverlayData;

// �ƹ������������
typedef struct N3DLightParamData
{
	CLight *pLight;
	vector<N3D_OMNI> vOmni;
	vector<N3D_DIRECTIONAL>vDir;
	vector<N3D_SPOTLIGHT> vSpot;
	N3DLightParamData()
	{
		pLight = 0;
	}
}N3DLightParamData;

class VGS_EXPORT CN3DLoaderImpV2 : public CN3DLoaderImpBase , public CVGSGetRoot
{
// �̳��Ի���Ľӿ�
public:
	// ����һ��N3D�ļ�
	virtual DWORD LoadN3DFile(CScene *pScene, const std::string &sFileName, HWND hWnd);
	virtual DWORD LoadSceneFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd);
	virtual DWORD LoadImageDataFromBuffer(CScene *pScene, DWORD version, const BYTE* pBuffer, HWND hWnd);
protected:  
	// ���ļ�ͷ
	virtual void        ReadHeader();
	//��ȡ��������ָ��
	virtual VOID        ReadFileTable();
	//��ȡ������Ϣ
	virtual VOID		ReadSceneProp();
	// ��ȡ˽����Ϣ
	virtual VOID        ReadPrivacyData();
	//��ȡ�������Ϣ
	virtual VOID		ReadCameraData();
	//��ȡ��������
	virtual VOID		ReadAudioData();
	//��ȡFlash����
	virtual VOID		ReadFlashData();
	//��ȡ�ƹ���Ϣ
	virtual VOID		ReadLightData();
	//��ȡ������Ϣ
	virtual VOID		ReadMaterialData();
	//��ȡͼ����Ϣ
	virtual VOID        ReadImageData();
	//��ȡģ����Ϣ
	virtual VOID        ReadModelData();
	//��ȡģ�����б���Ϣ
	virtual VOID		ReadModelGroupData();
	//��ȡģ�����б���Ϣ
	// virtual VOID		ReadModelGroupData1(CModelGroup* pModelGroup, BYTE* offset);
	//��ȡ������Ϣ
	virtual VOID		ReadTextData();
	//��ȡOverlay��Ϣ
	virtual VOID		ReadOverlayData();
	//��ȡNavigate��Ϣ
	virtual VOID		ReadNavigaterData();
	//��ȡButton��Ϣ
	virtual VOID		ReadButtonData();
	//��ȡ������Ϣ
	virtual VOID		ReadControlData();
	//��ȡ�ڵ㳡��ͼ��Ϣ
	virtual VOID        ReadSceneGraph();


protected:
	N3D_SceneProp               sceneProp;                  //��������

	N3D_5_1_Table               TableData;                  //�ļ�ָ��

	DWORD                       ImageDataPtr;
    N3D_AudioList               AudioData;
	N3D_FlashList               FlashData;
	N3D_CameraList			    CameraData;
    N3D_MtrlList                MaterialData;
	N3D_LightList               LightData;

    N3D_TextList			    TextData;			//�����б�
	N3D_OverlayList			    OverlayData;		//Overlay�б�
	N3D_NavigaterList		    NavigaterData;		//Navigater�б�
	N3D_ButtonList			    ButtonData;			//��ť�б�

	N3D_ControlData			    ControlData;		//�û����ƶ�������

	N3D_TextureList             texData;
	N3D_ImageList               ImageData;			    //ͼ���б�

	vector <CMaterial*>			m_vMaterial;

//------------------------------------------------------------------------------	
//�������ݴ��ݵı���
//------------------------------------------------------------------------------
protected:
	DWORD				N3DBufSize;							//N3D���ݳߴ�
	BYTE*				N3DBuffer;							//N3D���ݻ���

	DWORD				V3DBufSize;							//E3D�ļ��ĳߴ�

	BYTE*				m_pSceneData;						//��ѹ��ĳ�������
	BYTE*               m_pImageData;                       // image����

	INT					m_preModelCnt;						//֮ǰ��Model����
	INT					m_preModelGroupCnt;

	//INT					m_preMtrlCnt;						//�ڼ��ر�N3D�ļ�֮ǰ�����еĲ�������
	INT					m_preTexCnt;		                //�ڵ��뱾N3D�ļ�֮ǰ���������Ѿ��е���ͼ����
	INT					m_preCubeMapCnt;					//�ڼ��ر�N3D�ļ�֮ǰ�����е�CubeMap����
	INT					m_preBumpWaterCnt;
	INT					m_preDyTexCnt;
	INT					m_preMovieTexCnt;

	INT					m_preOverlayCnt;					//�ڼ��ر�N3D�ļ�֮ǰ�����е�Overlay����
	INT					m_preButtonCnt;						//�ڼ��ر�N3D�ļ�֮ǰ�����е�Button����
	INT					m_preNavigaterCnt;

	INT					m_preCameraCnt;						//�ڼ��ر�N3D�ļ�֮ǰ�����е�Camera����
	INT                 m_preLightCnt;

	INT					m_ImageIndex;						//ͼ��ı��
	//FILE				*pDebugFile;						//���ڵ��Ե�������


protected:
	CScene*						m_pScene;					//�ܳ���

	CSceneResource*				m_pSceneResource;			//��ǰ�����������
	CRITICAL_SECTION*			m_pRenderCS;		//��Ⱦ�̹߳������
	CRITICAL_SECTION*			m_pCtrlCS;			//�����̹߳������

	CHAR						m_strFileName[256]; //N3D���ļ���

	FLOAT        m_fN3DFileVersion;  // ���ڲ�����N3D�ļ��汾��

protected:
	// ���쳡��
	DWORD        ConstructScene(CScene *pScene, CSceneResource* pSceneResource);
	//��ȡ��̬��ͼ��Ϣ
	VOID		ReadDynamicTexData();
	// ������û�д�����Overlay
	VOID CreateOverlay();

	//���ģ����Ҫ������Ⱦ���棬���ģ�͵�������Ⱦ���桪����͹�ϳɱ���
	VOID		BandModelToAddtionalSurface_Bump(CModel* pModel);

	VOID        ConstructSceneGraph(CSceneNode* parentNode, N3D_Node* pNode, BYTE** offset);

	//����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
	VOID		FindReferenceModel();

	//�ͷŹ�������
	VOID		Release();

	//�����ͼ�����Ƿ��Ѿ�����
	int			IsTextureExist(std::string texName, INT width, INT height, D3DFORMAT format);

	//�����õ���ͼ������л�����ͼ����
	VOID		CreateBasicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);

	//�����õ���ͼ������л�����ͼ����
	VOID		CreateDynamicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);

	//������N3D���õ�����ͼ��CubeMap��ͼ
	VOID		CreateCubeTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);

	//�ж�ʹ�ø���ͼ�Ĳ����Ƿ���AlphaBlend--------------------------------------------------------
	VOID		UpdateAllMtrlAlphaBlendStatus(DWORD pTex);

	//��ͼ�񹹽���ͼ
	BOOL        ConstructTextureFromImage(N3D_5_1_ZipImageData* pImageData);
	
	D3DTEXTUREOP GetD3DTexOP(const N3D_BlenderOP& n3D_BlenderOP); 


public:
	CN3DLoaderImpV2();
	virtual ~CN3DLoaderImpV2();

protected:
	INT m_SceneWidth;
	INT m_SceneHeight;

	vector <N3DOverlayData> m_vN3DOverlayData;  // n3d�ж�����overlay����

	// ��N3D_Overlay�л�ȡOverlay����
	VOID CopyOverlayInfo(COverlay *pOverlay, const N3D_Overlay &info);

	vector <N3DLightParamData> m_vLightParam;
	N3DLightParamData getLightParamFrameData(CLight *pLight)
	{
		UINT count = m_vLightParam.size();
		for (UINT i = 0; i < count; ++i)
		{
			if (m_vLightParam[i].pLight == pLight)
			{
				return m_vLightParam[i];
			}
		}

		N3DLightParamData param;
		return param;
	}
};
