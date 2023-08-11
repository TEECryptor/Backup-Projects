/////////////////////////////////////////////////////////////////////////
// ����: CGetE3DFileInfo_3_1--E3D (3.1�汾)�ļ���ȡ��
// ����: ��ȡE3D�ļ�����������������
// ����: lixin	
// ����: 2007-05-10
////////////////////////////////////////////////////////////////////////

#ifndef _CGETE3DFILEINFO_3_1_H
#define _CGETE3DFILEINFO_3_1_H

#include "CScene.h"
#include "E3D_3_1.h"

//-----------------------------------------------------------------------------
//��  ��: CGetE3DFileInfo_3_1
//��  ;: ���ڴ�E3D�ļ�������ȡ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74C033C
class CGetE3DFileInfo_3_1  
{
public:
	//##ModelId=47EBC74C033E
	E3D_3_1_Header				Header;				//E3D�ļ�ͷ
	//##ModelId=47EBC74C034E
	E3D_3_1_WindowProp			WindowProp;				//�������

	//##ModelId=47EBC74C0353
	E3D_3_1_AudioList			AudioData;			//�������ݵ���ʼλ��
	//##ModelId=47EBC74C0358
	E3D_3_1_FlashList			FlashData;			//Flash���ݵ���ʼλ��
	
	//##ModelId=47EBC74C035D
	E3D_3_1_CameraList			CameraData;			//������б�
	//##ModelId=47EBC74C0362
	E3D_3_1_MaterialList		MaterialData;		//�����б�
	//##ModelId=47EBC74C0367
	E3D_3_1_LightList			LightData;			//�ƹ��б�

	//##ModelId=47EBC74C036C
	E3D_3_1_VertexList			VertexData;			//�����б�
	//##ModelId=47EBC74C0371
	E3D_3_1_VertexPosList		VertexPosData;		//ѹ����Ķ���λ���б�
	//##ModelId=47EBC74C0376
	E3D_3_1_VertexNormalList	VertexNormalData;   //ѹ����Ķ��㷨���б�
	//##ModelId=47EBC74C037B
	E3D_3_1_UVList				UVData;				//����UV����
	//##ModelId=47EBC74C0380
	E3D_3_1_ModelList			ModelData;			//Model�б�ģ���б�
	//##ModelId=47EBC74C0385
	E3D_3_1_ModelGroup			ModelGroupData;		//ģ�����б�

	//##ModelId=47EBC74C038A
	E3D_3_1_TextList			TextData;			//�����б�
	//##ModelId=47EBC74C038F
	E3D_3_1_OverlayList			OverlayData;		//Overlay�б�
	//##ModelId=47EBC74C0394
	E3D_3_1_NavigaterList		NavigaterData;		//Navigater�б�
	//##ModelId=47EBC74C0399
	E3D_3_1_ButtonList			ButtonData;			//��ť�б�

	//##ModelId=47EBC74C039E
	E3D_3_1_CubeMapList			CubeMapData;		//CubeMap�б�2.5Version Up
	//##ModelId=47EBC74C03A3
	E3D_3_1_MovieTexList		MovieTexData;		//��Ӱ��ͼ�б�
	//##ModelId=47EBC74C03A8
	E3D_3_1_BumpWaterList		BumpWaterData;		//ˮ����ͼ�б�

	//##ModelId=47EBC74C03AD
	E3D_3_1_Privacy				PrivacyData;		//��Ȩ����

	//##ModelId=47EBC74C03B2
	E3D_3_1_ControlData			ControlData;		//�û����ƶ�������

	//##ModelId=47EBC74C03B7
	E3D_3_1_TextureList			TexData;			//��ͼ�б�
	//##ModelId=47EBC74C03BC
	E3D_3_1_ImageList			ImageData;			//ͼ���б�
	//##ModelId=47EBC74C03C1
	E3D_3_1_DynamicTexList		DynamicTexData;		//��̬��ͼ����

public:
	//##ModelId=47EBC74C03C5
	CHAR						m_strFileName[256];	//�ļ���
	//##ModelId=47EBC74C03C9
	CScene*						pScene;				//����
	
	//##ModelId=47EBC74C03CD
	CRITICAL_SECTION*			m_pRenderCS;		//��Ⱦ�̹߳������

public:
	//��ȡ�ļ�ͷ
	//##ModelId=47EBC74C03CE
	VOID		ReadHeader();
	//��ȡ������Ϣ
	//##ModelId=47EBC74D000F
	VOID		ReadWindowProp();

	//��ȡ��������
	//##ModelId=47EBC74D003E
	VOID		ReadAudioData();
	//��ȡFlash����
	//##ModelId=47EBC74D005D
	VOID		ReadFlashData();

	//��ȡ�������Ϣ
	//##ModelId=47EBC74D008C
	VOID		ReadCameraData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74D009C
	VOID		ReadMaterialData();
	//��ȡ�ƹ���Ϣ
	//##ModelId=47EBC74D00BB
	VOID		ReadLightData();

	//��ȡ����λ����Ϣ
	//##ModelId=47EBC74D00BC
	VOID		ReadVertexPosData();
	//��ȡ����ķ�����Ϣ
	//##ModelId=47EBC74D00EA
	VOID		ReadVertexNormalData();
	//��ȡ�����UV��Ϣ
	//##ModelId=47EBC74D0109
	VOID		ReadVertexUVData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74D0138
	VOID		ReadVertexData();
	//��ȡģ����Ϣ
	//##ModelId=47EBC74D0157
	VOID		ReadModelData();
	//��ȡģ�����б���Ϣ
	//##ModelId=47EBC74D0177
	VOID		ReadModelGroupData();
	//��ȡģ�����б���Ϣ
	//##ModelId=47EBC74D0196
	VOID		ReadModelGroupData1(CModelGroup* pModelGroup);

	//��ȡ������Ϣ
	//##ModelId=47EBC74D01B5
	VOID		ReadTextData();
	//��ȡOverlay��Ϣ
	//##ModelId=47EBC74D01B6
	VOID		ReadOverlayData();
	//��ȡNavigate��Ϣ
	//##ModelId=47EBC74D01C5
	VOID		ReadNavigaterData();
	//��ȡButton��Ϣ
	//##ModelId=47EBC74D01C6
	VOID		ReadButtonData();
	//��ȡ��ͼ��Ϣ
	//##ModelId=47EBC74D01C7
	VOID		ReadTexData();
	//��ȡImage��Ϣ
	//##ModelId=47EBC74D01E4
	VOID		ReadImageData();
	//��ȡCubeMap��Ϣ��2.5�汾Up
	//##ModelId=47EBC74D01E5
	VOID		ReadCubeMapData();
	//��ȡMovieTex��Ϣ��2.5�汾Up
	//##ModelId=47EBC74D01E6
	VOID		ReadMovieTexData();
	//��ȡBumpWater��Ϣ��2.5�汾Up
	//##ModelId=47EBC74D01F4
	VOID		ReadBumpWaterData();

	//��ȡ��̬��ͼ��Ϣ
	//##ModelId=47EBC74D01F5
	VOID		ReadDynamicTexData();
	//��ȡ��Ȩ��Ϣ
	//##ModelId=47EBC74D01F6
	VOID		ReadPrivacyData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74D01F7
	VOID		ReadContorlData();

	//����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
	//##ModelId=47EBC74D01F8
	VOID		FindReferenceModel();
	//�򿪶�Ӧ��Scene����ΪE3D�ļ�
	//##ModelId=47EBC74D01F9
	HRESULT		LoadE3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74D0204
	VOID		Release();

//------------------------------------------------------------------------------	
//�������ݴ��ݵı���
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74D0205
	FILE *pFile;						//E3D�ļ�������

public:
	//##ModelId=47EBC74D0213
	CGetE3DFileInfo_3_1(CRITICAL_SECTION* pRenderCS);				//��Ⱦ�̹߳������
	//##ModelId=47EBC74D0242
	~CGetE3DFileInfo_3_1();

};

#endif
