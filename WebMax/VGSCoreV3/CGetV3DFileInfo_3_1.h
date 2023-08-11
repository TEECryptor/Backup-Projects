/////////////////////////////////////////////////////////////////////////
// ����: CGetV3DFileInfo_3_1 -- V3D 1.0�汾�ļ���ȡ��
// ����: ��ȡV3D�ļ�����������������
// ����: lixin	
// ����: 2007-05-09
////////////////////////////////////////////////////////////////////////

#ifndef _CGetV3DFileInfo_3_1_H
#define _CGetV3DFileInfo_3_1_H

#include "CScene.h"
#include "V3D_3_1.h"

//-----------------------------------------------------------------------------
//��  ��: CGetV3DFileInfo
//��  ;: ���ڴ�V3D�ļ�������ȡ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74A0232
class CGetV3DFileInfo_3_1  
{
	//V3D�ļ����������ݽ���Ϊ��Ӧ�����ݽṹ
public:
	//##ModelId=47EBC74A0234
	V3D_3_1_Header				Header;				//E3D�ļ�ͷ
	//##ModelId=47EBC74A0243
	V3D_3_1_WindowProp			WindowProp;			//�������
	
	//##ModelId=47EBC74A0248
	V3D_3_1_AudioList			AudioData;			//�������ݵ���ʼλ��
	//##ModelId=47EBC74A024D
	V3D_3_1_FlashList			FlashData;			//Flash���ݵ���ʼλ��
	
	//##ModelId=47EBC74A0252
	V3D_3_1_CameraList			CameraData;			//������б�
	//##ModelId=47EBC74A0257
	V3D_3_1_MaterialList		MaterialData;		//�����б�
	//##ModelId=47EBC74A025C
	V3D_3_1_LightList			LightData;			//�ƹ��б�

	//##ModelId=47EBC74A0261
	V3D_3_1_VertexList			VertexData;			//�����б�
	//##ModelId=47EBC74A0266
	V3D_3_1_VertexPosList		VertexPosData;		//ѹ����Ķ���λ���б�
	//##ModelId=47EBC74A026B
	V3D_3_1_VertexNormalList	VertexNormalData;   //ѹ����Ķ��㷨���б�
	//##ModelId=47EBC74A0270
	V3D_3_1_UVList				UVData;				//����UV����
	//##ModelId=47EBC74A0275
	V3D_3_1_ModelList			ModelData;			//Model�б�ģ���б�
	//##ModelId=47EBC74A027A
	V3D_3_1_ModelGroup			ModelGroupData;		//ģ�����б�

	//##ModelId=47EBC74A027F
	V3D_3_1_TextList			TextData;			//�����б�
	//##ModelId=47EBC74A0284
	V3D_3_1_OverlayList			OverlayData;		//Overlay�б�
	//##ModelId=47EBC74A0289
	V3D_3_1_NavigaterList		NavigaterData;		//Navigater�б�
	//##ModelId=47EBC74A028E
	V3D_3_1_ButtonList			ButtonData;			//��ť�б�

	//##ModelId=47EBC74A0293
	V3D_3_1_CubeMapList			CubeMapData;		//CubeMap�б�2.5Version Up
	//##ModelId=47EBC74A0298
	V3D_3_1_MovieTexList		MovieTexData;		//��Ӱ��ͼ�б�
	//##ModelId=47EBC74A029D
	V3D_3_1_BumpWaterList		BumpWaterData;		//ˮ����ͼ�б�

	//##ModelId=47EBC74A02A2
	V3D_3_1_DynamicTexList		DynamicTexData;		//��̬��ͼ������ʼλ��

	//##ModelId=47EBC74A02A7
	V3D_3_1_Privacy				PrivacyData;		//��Ȩ����

	//##ModelId=47EBC74A02AC
	V3D_3_1_ControlData			ControlData;		//�û����ƶ�������

	//##ModelId=47EBC74A02B1
	V3D_3_1_TextureList			TexData;			//��ͼ�б�
	//##ModelId=47EBC74A02B6
	V3D_3_1_ImageList			ImageData;			//ͼ���б�

public:
	//##ModelId=47EBC74A02BA
	CHAR						m_strFileName[256];	//�ļ���
	//##ModelId=47EBC74A02BC
	CScene*						pScene;				//����
	
	//##ModelId=47EBC74A02C0
	CRITICAL_SECTION*			m_pRenderCS;		//��Ⱦ�̹߳������

public:
	//��ȡ�ļ�ͷ
	//##ModelId=47EBC74A02C1
	VOID		ReadHeader();
	//��ȡ������Ϣ
	//##ModelId=47EBC74A02FD
	VOID		ReadWindowProp();

	//��ȡ��������
	//##ModelId=47EBC74A032C
	VOID		ReadAudioData();
	//��ȡFlash����
	//##ModelId=47EBC74A035B
	VOID		ReadFlashData();

	//��ȡ�������Ϣ
	//##ModelId=47EBC74A038A
	VOID		ReadCameraData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74A0399
	VOID		ReadMaterialData();
	//��ȡ�ƹ���Ϣ
	//##ModelId=47EBC74A03A9
	VOID		ReadLightData();

	//��ȡ����λ����Ϣ
	//##ModelId=47EBC74A03B9
	VOID		ReadVertexPosData();
	//��ȡ����ķ�����Ϣ
	//##ModelId=47EBC74A03D8
	VOID		ReadVertexNormalData();
	//��ȡ�����UV��Ϣ
	//##ModelId=47EBC74B001F
	VOID		ReadVertexUVData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74B003E
	VOID		ReadVertexData();
	//��ȡģ����Ϣ
	//##ModelId=47EBC74B005D
	VOID		ReadModelData();
	//��ȡģ�����б���Ϣ
	//##ModelId=47EBC74B007D
	VOID		ReadModelGroupData();
	//��ȡģ�����б���Ϣ
	//##ModelId=47EBC74B00AB
	VOID		ReadModelGroupData1(CModelGroup* pModelGroup);

	//��ȡ������Ϣ
	//##ModelId=47EBC74B00DA
	VOID		ReadTextData();
	//��ȡOverlay��Ϣ
	//##ModelId=47EBC74B00EA
	VOID		ReadOverlayData();
	//��ȡNavigate��Ϣ
	//##ModelId=47EBC74B00EB
	VOID		ReadNavigaterData();
	//��ȡButton��Ϣ
	//##ModelId=47EBC74B00EC
	VOID		ReadButtonData();
	//��ȡ��ͼ��Ϣ
	//##ModelId=47EBC74B00FA
	VOID		ReadTexData();
	//��ȡImage��Ϣ
	//##ModelId=47EBC74B0109
	VOID		ReadImageData();
	//��ȡCubeMap��Ϣ��2.5�汾Up
	//##ModelId=47EBC74B010A
	VOID		ReadCubeMapData();
	//��ȡMovieTex��Ϣ��2.5�汾Up
	//##ModelId=47EBC74B010B
	VOID		ReadMovieTexData();
	//��ȡBumpWater��Ϣ��2.5�汾Up
	//##ModelId=47EBC74B010C
	VOID		ReadBumpWaterData();

	//��ȡ��̬��ͼ��Ϣ
	//##ModelId=47EBC74B0119
	VOID		ReadDynamicTexData();
	//��ȡ��Ȩ��Ϣ
	//##ModelId=47EBC74B0138
	VOID		ReadPrivacyData();

	//��ȡ������Ϣ
	//##ModelId=47EBC74B0177
	VOID		ReadContorlData();

	//����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
	//##ModelId=47EBC74B01A5
	VOID		FindReferenceModel();
	//�򿪶�Ӧ��Scene����ΪE3D�ļ�
	//##ModelId=47EBC74B01A6
	HRESULT		LoadV3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74B01B5
	VOID		Release();

//------------------------------------------------------------------------------	
//�������ݴ��ݵı���
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74B01B6
	FILE		*pFile;						//E3D�ļ�������

public:
	//##ModelId=47EBC74B01B7
	CGetV3DFileInfo_3_1(CRITICAL_SECTION* pRenderCS);				//��Ⱦ�̹߳������
	//##ModelId=47EBC74B01F4
	~CGetV3DFileInfo_3_1();

};

#endif
