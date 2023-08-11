/////////////////////////////////////////////////////////////////////////
// ����: CGetV3DFileInfo_3_2-- V3D 3.2�汾�ļ���ȡ��
// ����: ��ȡV3D�ļ�����������������
// ����: lixin	
// ����: 2007-05-09
////////////////////////////////////////////////////////////////////////

#ifndef _CGetV3DFileInfo_3_2_H
#define _CGetV3DFileInfo_3_2_H

#include "CScene.h"
#include "V3D_3_2.h"

//-----------------------------------------------------------------------------
//��  ��: CGetV3DFileInfo
//��  ;: ���ڴ�V3D�ļ�������ȡ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74901E4
class CGetV3DFileInfo_3_2 
{
	//V3D�ļ����������ݽ���Ϊ��Ӧ�����ݽṹ
public:
	//##ModelId=47EBC74901E6
	V3D_3_2_Header				Header;				//E3D�ļ�ͷ
	//##ModelId=47EBC74901EB
	V3D_3_2_WindowProp			WindowProp;			//�������
	
	//##ModelId=47EBC74901F0
	V3D_3_2_AudioList			AudioData;			//�������ݵ���ʼλ��
	//##ModelId=47EBC74901F5
	V3D_3_2_FlashList			FlashData;			//Flash���ݵ���ʼλ��
	
	//##ModelId=47EBC74901FA
	V3D_3_2_CameraList			CameraData;			//������б�
	//##ModelId=47EBC74901FF
	V3D_3_2_MaterialList		MaterialData;		//�����б�
	//##ModelId=47EBC7490204
	V3D_3_2_LightList			LightData;			//�ƹ��б�

	//##ModelId=47EBC7490209
	V3D_3_2_VertexList			VertexData;			//�����б�
	//##ModelId=47EBC749020E
	V3D_3_2_VertexPosList		VertexPosData;		//ѹ����Ķ���λ���б�
	//##ModelId=47EBC7490213
	V3D_3_2_VertexNormalList	VertexNormalData;   //ѹ����Ķ��㷨���б�
	//##ModelId=47EBC7490218
	V3D_3_2_UVList				UVData;				//����UV����
	//##ModelId=47EBC749021D
	V3D_3_2_ModelList			ModelData;			//Model�б�ģ���б�
	//##ModelId=47EBC7490222
	V3D_3_2_ModelGroup			ModelGroupData;		//ģ�����б�

	//##ModelId=47EBC7490227
	V3D_3_2_TextList			TextData;			//�����б�
	//##ModelId=47EBC749022C
	V3D_3_2_OverlayList			OverlayData;		//Overlay�б�
	//##ModelId=47EBC7490231
	V3D_3_2_NavigaterList		NavigaterData;		//Navigater�б�
	//##ModelId=47EBC7490236
	V3D_3_2_ButtonList			ButtonData;			//��ť�б�

	//##ModelId=47EBC749023B
	V3D_3_2_CubeMapList			CubeMapData;		//CubeMap�б�2.5Version Up
	//##ModelId=47EBC7490240
	V3D_3_2_MovieTexList		MovieTexData;		//��Ӱ��ͼ�б�
	//##ModelId=47EBC7490245
	V3D_3_2_BumpWaterList		BumpWaterData;		//ˮ����ͼ�б�

	//##ModelId=47EBC749024A
	V3D_3_2_DynamicTexList		DynamicTexData;		//��̬��ͼ������ʼλ��

	//##ModelId=47EBC749024F
	V3D_3_2_Privacy				PrivacyData;		//��Ȩ����

	//##ModelId=47EBC7490254
	V3D_3_2_ControlData			ControlData;		//�û����ƶ�������

	//##ModelId=47EBC7490259
	V3D_3_2_TextureList			TexData;			//��ͼ�б�
	//##ModelId=47EBC749025E
	V3D_3_2_ImageList			ImageData;			//ͼ���б�

public:
	//##ModelId=47EBC7490262
	CHAR						m_strFileName[256];	//�ļ���
	//##ModelId=47EBC7490264
	CScene*						pScene;				//����
	
	//##ModelId=47EBC7490268
	CRITICAL_SECTION*			m_pRenderCS;		//��Ⱦ�̹߳������

public:
	//��ȡ�ļ�ͷ
	//##ModelId=47EBC7490271
	VOID		ReadHeader();
	//��ȡ������Ϣ
	//##ModelId=47EBC749029F
	VOID		ReadWindowProp();

	//��ȡ��������
	//##ModelId=47EBC74902CE
	VOID		ReadAudioData();
	//��ȡFlash����
	//##ModelId=47EBC74902FD
	VOID		ReadFlashData();

	//��ȡ�������Ϣ
	//##ModelId=47EBC749032C
	VOID		ReadCameraData();
	//��ȡ������Ϣ
	//##ModelId=47EBC749033C
	VOID		ReadMaterialData();
	//��ȡ�ƹ���Ϣ
	//##ModelId=47EBC749034B
	VOID		ReadLightData();

	//��ȡ����λ����Ϣ
	//##ModelId=47EBC749035B
	VOID		ReadVertexPosData();
	//��ȡ����ķ�����Ϣ
	//##ModelId=47EBC749037A
	VOID		ReadVertexNormalData();
	//��ȡ�����UV��Ϣ
	//##ModelId=47EBC7490399
	VOID		ReadVertexUVData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74903C8
	VOID		ReadVertexData();
	//��ȡģ����Ϣ
	//##ModelId=47EBC74A0000
	VOID		ReadModelData();
	//��ȡģ�����б���Ϣ
	//##ModelId=47EBC74A001F
	VOID		ReadModelGroupData();
	//��ȡģ�����б���Ϣ
	//##ModelId=47EBC74A003E
	VOID		ReadModelGroupData1(CModelGroup* pModelGroup);

	//��ȡ������Ϣ
	//##ModelId=47EBC74A006D
	VOID		ReadTextData();
	//��ȡOverlay��Ϣ
	//##ModelId=47EBC74A007D
	VOID		ReadOverlayData();
	//��ȡNavigate��Ϣ
	//##ModelId=47EBC74A007E
	VOID		ReadNavigaterData();
	//��ȡButton��Ϣ
	//##ModelId=47EBC74A007F
	VOID		ReadButtonData();
	//��ȡ��ͼ��Ϣ
	//##ModelId=47EBC74A008C
	VOID		ReadTexData();
	//��ȡImage��Ϣ
	//##ModelId=47EBC74A009C
	VOID		ReadImageData();
	//��ȡCubeMap��Ϣ��2.5�汾Up
	//##ModelId=47EBC74A009D
	VOID		ReadCubeMapData();
	//��ȡMovieTex��Ϣ��2.5�汾Up
	//##ModelId=47EBC74A009E
	VOID		ReadMovieTexData();
	//��ȡBumpWater��Ϣ��2.5�汾Up
	//##ModelId=47EBC74A00AB
	VOID		ReadBumpWaterData();

	//��ȡ��̬��ͼ��Ϣ
	//##ModelId=47EBC74A00AC
	VOID		ReadDynamicTexData();
	//��ȡ��Ȩ��Ϣ
	//##ModelId=47EBC74A00CB
	VOID		ReadPrivacyData();

	//��ȡ������Ϣ
	//##ModelId=47EBC74A00FA
	VOID		ReadContorlData();

	//����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
	//##ModelId=47EBC74A0138
	VOID		FindReferenceModel();
	//�򿪶�Ӧ��Scene����ΪE3D�ļ�
	//##ModelId=47EBC74A0139
	HRESULT		LoadV3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74A0148
	VOID		Release();

//------------------------------------------------------------------------------	
//�������ݴ��ݵı���
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74A0149
	FILE		*pFile;						//E3D�ļ�������

public:
	//##ModelId=47EBC74A014A
	CGetV3DFileInfo_3_2(CRITICAL_SECTION* pRenderCS);				//��Ⱦ�̹߳������
	//##ModelId=47EBC74A0186
	~CGetV3DFileInfo_3_2();

};

#endif
