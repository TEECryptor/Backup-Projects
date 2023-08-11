///////////////////////////////////////////////////////////////////////////
// SaveV3D.h: interface for the CSaveE3D class.
// ��  ��: CSaveV3D
// ��  ��: �������ݱ����࣬���泡���е�����Ϊ�ϰ汾*.V3D�ļ�, ��ͼ�������ⲿ·������
// ��  ��: Lixin
// ��  ��: 2007-04-09
// ��  Ȩ: �Ϻ���ͼ����������Ϣ�������޹�˾
///////////////////////////////////////////////////////////////////////////

#ifndef _SAVEV3D_H
#define _SAVEV3D_H

//------------------------------------------------------------------------
#include "CScene.h"
#include "V3D_def.h"

//------------------------------------------------------------------------
//##ModelId=47EBC74402CE
class CSaveV3D  
{
public:
	//##ModelId=47EBC74402DE
	CSaveV3D();
	//##ModelId=47EBC74402EE
	~CSaveV3D();

private:
	//##ModelId=47EBC744030E
	CScene*				pScene;					//ָ�򳡾�����Scene��ָ��
	//##ModelId=47EBC7440312
	FILE*				pFile;					//ָ��V3D�ļ���ָ��
	//##ModelId=47EBC7440313
	DWORD				nFileSize;				//V3D�ļ��Ĵ�С
	//##ModelId=47EBC7440314
	CHAR				FileName[256];			//V3D�ļ���

	//##ModelId=47EBC744031D
	V3D_V3DHeader			Header;					//V3D�ļ�ͷ
	//##ModelId=47EBC7440322
	V3D_CameraList			CameraData;				
	//##ModelId=47EBC7440327
	V3D_MaterialList		MaterialData;			
	//##ModelId=47EBC744032C
	V3D_LightList			LightData;				//�ƹ�����
	//##ModelId=47EBC7440331
	V3D_ModelList			ModelData;
	//##ModelId=47EBC7440336
	V3D_VertexList			VertexData;				//��������
	//##ModelId=47EBC744033B
	V3D_VertexPosList		VertexPosData;			//ֻ���������λ��
	//##ModelId=47EBC7440340
	V3D_VertexNormalList	VertexNormalData;		//ֻ��������ķ���
	//##ModelId=47EBC7440345
	V3D_UVList				UVData;					//��ͼ��UV���ݣ���λ�á����ߡ�UV�ֿ���������Ϊ�кܶඥ���λ�á����ߡ�UV����ͬ�ģ�����ֻ����һ��
	//##ModelId=47EBC744034A
	V3D_TextList			TextData;				//��������
	//##ModelId=47EBC744034F
	V3D_OverlayList			OverlayData;			//Overlay����
	//##ModelId=47EBC7440354
	V3D_NavigaterList		NavigaterData;			//Navigater����
	//##ModelId=47EBC7440359
	V3D_ButtonList			ButtonData;				//��ť�б�
	//##ModelId=47EBC744035E
	V3D_CubeMapList			CubeMapData;			//CubeMap�б�2007��2��25��������WebMax2.5�汾
	//##ModelId=47EBC7440363
	V3D_MovieTexList		MovieTexData;			//��Ӱ��ͼ�б�2007��3��17��������WebMax2.5�汾
	//##ModelId=47EBC7440368
	V3D_BumpWaterList		BumpWaterData;			//BumpWater��ͼ�б�2007��3��17������

	//##ModelId=47EBC744036D
	V3D_TextureList			TexData;				//��ͼ����
	//##ModelId=47EBC7440372
	V3D_ImageList			ImageData;				//Image����


public:

	//д���ļ�ͷ
	//##ModelId=47EBC7440376
	VOID		WriteHeader();
	//д���������Ϣ
	//##ModelId=47EBC744038A
	VOID		WriteCameraData();
	//д�������Ϣ
	//##ModelId=47EBC7440399
	VOID		WriteMaterialData();
	//д��ƹ���Ϣ
	//##ModelId=47EBC74403B9
	VOID		WriteLightData();
	//д��ģ����Ϣ
	//##ModelId=47EBC74403C8
	VOID		WriteModelData();
	//д�붥��λ����Ϣ
	//##ModelId=47EBC74403D8
	VOID		WriteVertexPosData(BOOL zipStatus);
	//д�붥�㷨����Ϣ
	//##ModelId=47EBC7450000
	VOID		WriteVertexNormalData(BOOL zipStatus);
	//д����ͼ������Ϣ
	//##ModelId=47EBC745000F
	VOID		WriteUVData(BOOL zipStatus);
	//д�붥����Ϣ
	//##ModelId=47EBC745001F
	VOID		WriteVertexData(BOOL zipStatus);
	//д��������Ϣ
	//##ModelId=47EBC745002E
	VOID		WriteTextData();
	//д��Overlay��Ϣ
	//##ModelId=47EBC745003E
	VOID		WriteOverlayData();
	//д��Navigater��Ϣ
	//##ModelId=47EBC745003F
	VOID		WriteNavigaterData();
	//д��Button��Ϣ
	//##ModelId=47EBC745004E
	VOID		WriteButtonData();
	
	//д��CubeMap��Ϣ
	//##ModelId=47EBC745004F
	VOID		WriteCubeMapData();
	//д��MovieTex��Ϣ
	//##ModelId=47EBC7450050
	VOID		WriteMovieTexData();
	//д��BumpWater��Ϣ
	//##ModelId=47EBC745005D
	VOID		WriteBumpWaterData();

	//д����ͼ��Ϣ
	//##ModelId=47EBC745005E
	VOID		WriteTexData();
	//д���ⲿͼ����Ϣ
	//##ModelId=47EBC745006D
	VOID		WriteImageData();

	//ѹ������λ�����ݣ�ȥ���ظ�����
	//##ModelId=47EBC745006E
	VOID		ZipVertexPosData(BOOL zipStatus);
	//�ҵ�pModel�ĵ�index������VertexPosData�����е�λ��
	//##ModelId=47EBC7450070
	UINT		FindOffsetInVertexPosData(UINT modelIndex, UINT vertexIndex);

	//ѹ�����㷨�����ݣ�ȥ���ظ�����
	//##ModelId=47EBC745007F
	VOID		ZipVertexNormalData(BOOL zipStatus);
	//�ҵ�pModel�ĵ�index������VertexNormalData�����е�λ��
	//##ModelId=47EBC745008C
	UINT		FindOffsetInVertexNormalData(UINT modelIndex, UINT vertexIndex);

	//ѹ������UV���ݣ�ȥ���ظ�����
	//##ModelId=47EBC745008F
	VOID		ZipUVData(BOOL zipStatus);
	//�ҵ�pModel�ĵ�index�����UV��UVData�����е�λ��
	//##ModelId=47EBC745009C
	UINT		FindOffsetInUVData(UINT modelIndex, UINT vertexIndex);

	//ѹ��ģ�����ݣ���ͼ���ݲ�ѹ��
	//##ModelId=47EBC745009F
	HRESULT		ZipModelData();

	//�ͷ���Դ
	//##ModelId=47EBC74500A0
	VOID		Release();

	//�����Ӧ��Scene����ΪE3D�ļ�
	//##ModelId=47EBC74500A1
	HRESULT		SaveSceneAsV3D(CScene* pScene, CHAR* strFileName, BOOL zipStatus);	

	//�ڱ���֮ǰ��Ϊ�����������ñ�ţ���ֹ��Ϊɾ������󣬵��±�������Ŵ���
	//##ModelId=47EBC74500AE
	VOID		ResetModelIndex();
	//##ModelId=47EBC74500AF
	VOID		ResetTextureIndex();
	//##ModelId=47EBC74500BB
	VOID		ResetBumpWaterIndex();
	//##ModelId=47EBC74500BC
	VOID		ResetCubeMapIndex();
	//##ModelId=47EBC74500BD
	VOID		ResetMovieTexIndex();
	//##ModelId=47EBC74500BE
	VOID		ResetMaterialIndex();
	//##ModelId=47EBC74500BF
	VOID		ResetOverlayIndex();

	//##ModelId=47EBC74500C0
	VOID		ResetCameraIndex();
	//##ModelId=47EBC74500C1
	VOID		ResetLightIndex();
	//##ModelId=47EBC74500C2
	VOID		ResetNavigaterIndex();
	//##ModelId=47EBC74500C3
	VOID		ResetTextIndex();

	//##ModelId=47EBC74500C4
	VOID		ResetButtonIndex();
	//##ModelId=47EBC74500C5
	VOID		ResetImageIndex();


};


//------------------------------------------------------------------------
#endif
