// GetN3DFileInfo_5.h: interface for the CGetN3DFileInfo_5 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETN3DFILEINFO_5_H__6D920467_70D4_479B_B084_0237A5D88D9A__INCLUDED_)
#define AFX_GETN3DFILEINFO_5_H__6D920467_70D4_479B_B084_0237A5D88D9A__INCLUDED_

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
#include "..\\VGSATLHeader.h"
#include "N3D_Def.h"
// #include "..\\vgszip\\vgszip.h"
// #include "SceneResource.h"

#include "..\\VGSGetCore.h"

using namespace N3D;

class CGetN3DFileInfo_5  : public CVGSGetCore
{
public:
	N3D_SceneProp               sceneProp;                  //��������

    N3D_5_1_Table               TableData;                  //�ļ�ָ��

	DWORD                       ImageDataPtr;
	//E3D_3_1_WindowProp			WindowProp;				//�������
    N3D_AudioList               AudioData;
	//E3D_3_1_AudioList			AudioData;			//�������ݵ���ʼλ��
	N3D_FlashList               FlashData;
	//E3D_3_1_FlashList			FlashData;			//Flash���ݵ���ʼλ��
	N3D_CameraList			    CameraData;
	//E3D_3_1_CameraList			CameraData;			//������б�
    N3D_MtrlList                MaterialData;
	//E3D_3_1_MaterialList		MaterialData;		//�����б�
	N3D_LightList               LightData;
	//E3D_3_1_LightList			LightData;			//�ƹ��б�
    N3D_ModelList               ModelData;         //ģ���б�
/*
	E3D_3_1_VertexList			VertexData;			//�����б�
	E3D_3_1_VertexPosList		VertexPosData;		//ѹ����Ķ���λ���б�
	E3D_3_1_VertexNormalList	VertexNormalData;   //ѹ����Ķ��㷨���б�
	E3D_3_1_UVList				UVData;				//����UV����
	E3D_3_1_ModelList			ModelData;			//Model�б�ģ���б�*/
    N3D_ModelGroup              ModelGroupData;
	//E3D_3_1_ModelGroup			ModelGroupData;		//ģ�����б�

    N3D_TextList			    TextData;			//�����б�
	N3D_OverlayList			    OverlayData;		//Overlay�б�
	N3D_NavigaterList		    NavigaterData;		//Navigater�б�
	N3D_ButtonList			    ButtonData;			//��ť�б�

	
	/*
	E3D_3_1_TextList			TextData;			//�����б�
	E3D_3_1_OverlayList			OverlayData;		//Overlay�б�
	E3D_3_1_NavigaterList		NavigaterData;		//Navigater�б�
	E3D_3_1_ButtonList			ButtonData;			//��ť�б�

	E3D_3_1_CubeMapList			CubeMapData;		//CubeMap�б�2.5Version Up
	E3D_3_1_MovieTexList		MovieTexData;		//��Ӱ��ͼ�б�
	E3D_3_1_BumpWaterList		BumpWaterData;		//ˮ����ͼ�б�*/

	N3D_ControlData			    ControlData;		//�û����ƶ�������

	N3D_TextureList             texData;
	//E3D_3_1_TextureList			TexData;			//��ͼ�б�
	N3D_ImageList               ImageData;			    //ͼ���б�



//	N3D_NormalTexList           NormalTexData;
	
	//N3D_OpaTexList              OpaTexData;
    
	//N3D_DynamicTexList          DynamicTexData;
//------------------------------------------------------------------------------	
//�������ݴ��ݵı���
//------------------------------------------------------------------------------
public:
	DWORD				N3DBufSize;							//N3D���ݳߴ�
	BYTE*				N3DBuffer;							//N3D���ݻ���

	DWORD				V3DBufSize;							//E3D�ļ��ĳߴ�

	BYTE*				m_pSceneData;						//��ѹ��ĳ�������

	INT					m_preModelCnt;						//֮ǰ��Model����
	INT					m_preModelGroupCnt;

	INT					m_preMtrlCnt;						//�ڼ��ر�N3D�ļ�֮ǰ�����еĲ�������
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
	FILE				*pDebugFile;						//���ڵ��Ե�������


public:
	//DWORD						m_pScene;					//�ܳ���

	DWORD/*CSceneResource**/				m_pSceneResource;			//��ǰ�����������
	//CRITICAL_SECTION*			m_pRenderCS;		//��Ⱦ�̹߳������
	//CRITICAL_SECTION*			m_pCtrlCS;			//�����̹߳������

	CHAR						m_strFileName[256]; //N3D���ļ���
	//E3D_3_1_DynamicTexList		DynamicTexData;		//��̬��ͼ����
//----------------------------------------------------------------
//��ȡ����
//----------------------------------------------------------------
public:
	//��ѹ�������ݣ�д���ѹ������ݵ�destBuf
	BOOL		UnzipSceneData(BYTE* srcBuf, DWORD srcBufSize, DWORD destBufSize);    
	//���쳡��
	BOOL        ConstructScene(const std::string &sResourceGoupName/*CHAR* sceneName, DWORD pSceneResource*/);
	//��ȡ��������ָ��
 	// VOID        ReadFileTable();

	// �����ļ�ͷ��Ϣ
	VOID SetHeaderInfo(N3D_Header N3DHeader);

// 	//VOID		ReadHeader();
// 	//��ȡ������Ϣ
// 	VOID		ReadSceneProp();
// 
// 	//��ȡ��������
// 	VOID		ReadAudioData();
// 	//��ȡFlash����
// 	VOID		ReadFlashData();
// 
// 	//��ȡ�������Ϣ
// 	VOID		ReadCameraData();
// 	//��ȡ������Ϣ
// 	VOID		ReadMaterialData();
// 	//��ȡ�ƹ���Ϣ
// 	VOID		ReadLightData();
    //��ȡͼ����Ϣ
	VOID        ReadImageData(BYTE *pImageDataBuffer);
// 	//��ȡģ����Ϣ
// 	VOID        ReadModelData();
// 
// 	//��ȡģ�����б���Ϣ
// 	VOID		ReadModelGroupData();
// 	//��ȡģ�����б���Ϣ
// 	// VOID		ReadModelGroupData1(CModelGroup* pModelGroup, BYTE* offset);
// 
// 	//��ȡ������Ϣ
// 	VOID		ReadTextData();
// 	//��ȡOverlay��Ϣ
// 	VOID		ReadOverlayData();
// 	//��ȡNavigate��Ϣ
// 	VOID		ReadNavigaterData();
// 	//��ȡButton��Ϣ
// 	VOID		ReadButtonData();
// 
// 	//��ȡ��̬��ͼ��Ϣ
// 	VOID		ReadDynamicTexData();
// 	//��ȡ������Ϣ
// 	VOID		ReadControlData();
    //��ȡ�ڵ㳡��ͼ��Ϣ
	//VOID        ReadSceneGraph();

	//���ģ����Ҫ������Ⱦ���棬���ģ�͵�������Ⱦ���桪����͹�ϳɱ���
	//VOID		BandModelToAddtionalSurface_Bump(CModel* pModel);

	//VOID        ConstructSceneGraph(CSceneNode** parentNode, N3D_Node* pNode, BYTE** offset);

	//����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
// 	VOID		FindReferenceModel();
// 
 	//�ͷŹ�������
 	VOID		Release();
// 
// 	//�����ͼ�����Ƿ��Ѿ�����
// 	int			IsTextureExist(std::string texName, INT width, INT height, D3DFORMAT format);
// 
// 	//�����õ���ͼ������л�����ͼ����
// 	VOID		CreateBasicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);
// 
// 	//�����õ���ͼ������л�����ͼ����
// 	VOID		CreateDynamicTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);
// 
// 	//������N3D���õ�����ͼ��CubeMap��ͼ
// 	VOID		CreateCubeTextureUsedThisImage(N3D_5_1_ZipImageData* pImageData);
// 
// 	//�ж�ʹ�ø���ͼ�Ĳ����Ƿ���AlphaBlend--------------------------------------------------------
// 	VOID		UpdateAllMtrlAlphaBlendStatus(DWORD pTex);
// 
 	//��ͼ�񹹽���ͼ
 	BOOL        ConstructTextureFromImage(N3D_5_1_ZipImageData* pImageData, INT imgIndex);
// 	
// 	D3DTEXTUREOP GetTexOP(const N3D_BlenderOP& n3D_BlenderOP); 

public:
	CGetN3DFileInfo_5(/*DWORD pScene*/);
	virtual ~CGetN3DFileInfo_5();

private:
	N3D_Header m_N3DHeader;
};

#endif // !defined(AFX_GETN3DFILEINFO_5_H__6D920467_70D4_479B_B084_0237A5D88D9A__INCLUDED_)
