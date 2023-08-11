/////////////////////////////////////////////////////////////////////////
// ����: CGetV3DFileInfo--V3D�ļ��ϰ汾��ȡ��
// ����: ��ȡV3D�ļ�����������������
// ����: lixin	
// ����: 2007-04-09
////////////////////////////////////////////////////////////////////////

#ifndef _CGETV3DFILEINFO_H
#define _CGETV3DFILEINFO_H

#include "V3D.h"
#include "CScene.h"

//-----------------------------------------------------------------------------
//��  ��: CGetE3DFileInfo
//��  ;: ���ڴ�E3D�ļ�������ȡ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74B0280
class CGetV3DFileInfo  
{

public:
	//##ModelId=47EBC74B0282
	V3D_V3DHeader			Header;				//E3D�ļ�ͷ
	//##ModelId=47EBC74B0287
	V3D_CameraList			CameraData;			//������б�
	//##ModelId=47EBC74B028C
	V3D_MaterialList		MaterialData;		//�����б�
	//##ModelId=47EBC74B0291
	V3D_LightList			LightData;			//�ƹ��б�
	//##ModelId=47EBC74B0296
	V3D_ModelList			ModelData;			//Model�б�ģ���б�

	//##ModelId=47EBC74B029B
	V3D_VertexList			VertexData;			//�����б�
	//##ModelId=47EBC74B02A0
	V3D_VertexPosList		VertexPosData;		//ѹ����Ķ���λ���б�
	//##ModelId=47EBC74B02A5
	V3D_VertexNormalList	VertexNormalData;   //ѹ����Ķ��㷨���б�
	//##ModelId=47EBC74B02AA
	V3D_UVList				UVData;				//����UV����


	//##ModelId=47EBC74B02AF
	V3D_TextList			TextData;			//�����б�
	//##ModelId=47EBC74B02B4
	V3D_OverlayList			OverlayData;		//Overlay�б�
	//##ModelId=47EBC74B02B9
	V3D_NavigaterList		NavigaterData;		//Navigater�б�
	//##ModelId=47EBC74B02BE
	V3D_ButtonList			ButtonData;			//��ť�б�

	//##ModelId=47EBC74B02C3
	V3D_CubeMapList			CubeMapData;		//CubeMap�б�2.5Version Up
	//##ModelId=47EBC74B02C8
	V3D_MovieTexList		MovieTexData;		//��Ӱ��ͼ�б�
	//##ModelId=47EBC74B02CD
	V3D_BumpWaterList		BumpWaterData;		//ˮ����ͼ�б�

	//##ModelId=47EBC74B02D2
	V3D_TextureList			TexData;			//��ͼ�б�
	//##ModelId=47EBC74B02D7
	V3D_ImageList			ImageData;			//ͼ���б�

	//##ModelId=47EBC74B02DB
	CHAR					m_strFileName[256];	//�ļ���
	//##ModelId=47EBC74B02DD
	CScene*					pScene;				//����
	
	//##ModelId=47EBC74B02E1
	CRITICAL_SECTION*		m_pRenderCS;		//��Ⱦ�̹߳������

	//��ȡ�ļ�ͷ
	//##ModelId=47EBC74B02E2
	VOID		ReadHeader();
	//��ȡ�������Ϣ
	//##ModelId=47EBC74B02FD
	VOID		ReadCameraData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74B030D
	VOID		ReadMaterialData();
	//��ȡ�ƹ���Ϣ
	//##ModelId=47EBC74B030E
	VOID		ReadLightData();
	//��ȡģ����Ϣ
	//##ModelId=47EBC74B031C
	VOID		ReadModelData();
	//��ȡ����λ����Ϣ
	//##ModelId=47EBC74B032C
	VOID		ReadVertexPosData();
	//��ȡ����ķ�����Ϣ
	//##ModelId=47EBC74B034B
	VOID		ReadVertexNormalData();
	//��ȡ�����UV��Ϣ
	//##ModelId=47EBC74B036B
	VOID		ReadVertexUVData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74B037A
	VOID		ReadVertexData();
	//��ȡ������Ϣ
	//##ModelId=47EBC74B0399
	VOID		ReadTextData();
	//��ȡOverlay��Ϣ
	//##ModelId=47EBC74B039A
	VOID		ReadOverlayData();
	//��ȡNavigate��Ϣ
	//##ModelId=47EBC74B03A9
	VOID		ReadNavigaterData();
	//��ȡButton��Ϣ
	//##ModelId=47EBC74B03AA
	VOID		ReadButtonData();
	//��ȡ��ͼ��Ϣ
	//##ModelId=47EBC74B03AB
	VOID		ReadTexData();
	//��ȡImage��Ϣ
	//##ModelId=47EBC74B03B9
	VOID		ReadImageData();
	//��ȡCubeMap��Ϣ��2.5�汾Up
	//##ModelId=47EBC74B03BA
	VOID		ReadCubeMapData();
	//��ȡMovieTex��Ϣ��2.5�汾Up
	//##ModelId=47EBC74B03BB
	VOID		ReadMovieTexData();
	//��ȡBumpWater��Ϣ��2.5�汾Up
	//##ModelId=47EBC74B03C8
	VOID		ReadBumpWaterData();

	//����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
	//##ModelId=47EBC74B03C9
	VOID		FindReferenceModel();
	//�򿪶�Ӧ��Scene����ΪE3D�ļ�
	//##ModelId=47EBC74B03CA
	HRESULT		LoadV3DFile(CScene* theScene, CHAR*  strFileName);	
	
	//##ModelId=47EBC74B03CD
	VOID		Release();

//------------------------------------------------------------------------------	
//�������ݴ��ݵı���
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74B03CE
	FILE		*pFile;						//E3D�ļ�������

public:
	//##ModelId=47EBC74B03D8
	CGetV3DFileInfo(CRITICAL_SECTION* pRenderCS);				//��Ⱦ�̹߳������
	//##ModelId=47EBC74C000F
	~CGetV3DFileInfo();

};

#endif
