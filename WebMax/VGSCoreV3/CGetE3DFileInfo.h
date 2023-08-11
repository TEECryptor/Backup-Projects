/////////////////////////////////////////////////////////////////////////
// ����: CGetE3DFileInfo--E3D�ļ���ȡ��
// ����: ��ȡE3D�ļ�����������������
// ����: lixin	
// ����: 2006-05-06
////////////////////////////////////////////////////////////////////////

#ifndef _CGETE3DFILEINFO_H
#define _CGETE3DFILEINFO_H

#include "E3D.h"
#include "CScene.h"

//-----------------------------------------------------------------------------
//��  ��: CGetE3DFileInfo
//��  ;: ���ڴ�E3D�ļ�������ȡ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74D02AF
class CGetE3DFileInfo  
{

public:
	//##ModelId=47EBC74D02B1
	_E3DHeader			Header;				//E3D�ļ�ͷ
	//##ModelId=47EBC74D02B6
	_CameraList			CameraData;			//������б�
	//##ModelId=47EBC74D02C0
	_MaterialList		MaterialData;		//�����б�
	//##ModelId=47EBC74D02C5
	_LightList			LightData;			//�ƹ��б�
	//##ModelId=47EBC74D02CA
	_ModelList			ModelData;			//Model�б�ģ���б�

	//##ModelId=47EBC74D02CF
	_VertexList			VertexData;			//�����б�
	//##ModelId=47EBC74D02D4
	_VertexPosList		VertexPosData;		//ѹ����Ķ���λ���б�
	//##ModelId=47EBC74D02D9
	_VertexNormalList	VertexNormalData;   //ѹ����Ķ��㷨���б�
	//##ModelId=47EBC74D02DE
	_UVList				UVData;				//����UV����


	//##ModelId=47EBC74D02E3
	_TextList			TextData;			//�����б�
	//##ModelId=47EBC74D02E8
	_OverlayList		OverlayData;		//Overlay�б�
	//##ModelId=47EBC74D02ED
	_NavigaterList		NavigaterData;		//Navigater�б�
	//##ModelId=47EBC74D02F2
	_ButtonList			ButtonData;			//��ť�б�

	//##ModelId=47EBC74D02F7
	_CubeMapList		CubeMapData;		//CubeMap�б�2.5Version Up
	//##ModelId=47EBC74D02FC
	_MovieTexList		MovieTexData;		//��Ӱ��ͼ�б�
	//##ModelId=47EBC74D0301
	_BumpWaterList		BumpWaterData;		//ˮ����ͼ�б�

	//##ModelId=47EBC74D0306
	_TextureList		TexData;			//��ͼ�б�
	//##ModelId=47EBC74D030B
	_ImageList			ImageData;			//ͼ���б�

	//##ModelId=47EBC74D030F
	CHAR				m_strFileName[256];	//�ļ���
	//##ModelId=47EBC74D0311
	CScene*				pScene;				//����
	
	//##ModelId=47EBC74D0315
	CRITICAL_SECTION*	m_pRenderCS;		//��Ⱦ�̹߳������

	//��ȡ�ļ�ͷ
	//##ModelId=47EBC74D0316
	VOID		ReadHeader();
	//��ȡ�������Ϣ
	//##ModelId=47EBC74D032C
	VOID		ReadCameraData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74D033C
	VOID		ReadMaterialData();
	//��ȡ�ƹ���Ϣ
	//##ModelId=47EBC74D033D
	VOID		ReadLightData();
	//��ȡģ����Ϣ
	//##ModelId=47EBC74D034B
	VOID		ReadModelData();
	//��ȡ����λ����Ϣ
	//##ModelId=47EBC74D035B
	VOID		ReadVertexPosData();
	//��ȡ����ķ�����Ϣ
	//##ModelId=47EBC74D036B
	VOID		ReadVertexNormalData();
	//��ȡ�����UV��Ϣ
	//##ModelId=47EBC74D038A
	VOID		ReadVertexUVData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74D0399
	VOID		ReadVertexData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74D03A9
	VOID		ReadTextData();
	//��ȡOverlay��Ϣ
	//##ModelId=47EBC74D03B9
	VOID		ReadOverlayData();
	//��ȡNavigate��Ϣ
	//##ModelId=47EBC74D03BA
	VOID		ReadNavigaterData();
	//��ȡButton��Ϣ
	//##ModelId=47EBC74D03BB
	VOID		ReadButtonData();
	//��ȡ��ͼ��Ϣ
	//##ModelId=47EBC74D03C8
	VOID		ReadTexData();
	//��ȡImage��Ϣ
	//##ModelId=47EBC74D03C9
	VOID		ReadImageData();
	//��ȡCubeMap��Ϣ��2.5�汾Up
	//##ModelId=47EBC74D03CA
	VOID		ReadCubeMapData();
	//��ȡMovieTex��Ϣ��2.5�汾Up
	//##ModelId=47EBC74D03D8
	VOID		ReadMovieTexData();
	//��ȡBumpWater��Ϣ��2.5�汾Up
	//##ModelId=47EBC74D03D9
	VOID		ReadBumpWaterData();

	//����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
	//##ModelId=47EBC74D03DA
	VOID		FindReferenceModel();
	//�򿪶�Ӧ��Scene����ΪE3D�ļ�
	//##ModelId=47EBC74D03DB
	HRESULT		LoadE3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74D03DE
	VOID		Release();

//------------------------------------------------------------------------------	
//�������ݴ��ݵı���
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74E0000
	FILE *pFile;						//E3D�ļ�������

public:
	//##ModelId=47EBC74E0001
	CGetE3DFileInfo(CRITICAL_SECTION* pRenderCS);				//��Ⱦ�̹߳������
	//##ModelId=47EBC74E001F
	~CGetE3DFileInfo();

};

#endif
