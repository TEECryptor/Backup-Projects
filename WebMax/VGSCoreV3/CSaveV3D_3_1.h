///////////////////////////////////////////////////////////////////////////
// SaveV3D.h: interface for the CSaveE3D class.
// ��  ��: CSaveV3D_3_1
// ��  ��: �������ݱ����࣬���泡���е�����Ϊ1.0�汾��*.V3D�ļ�, ��ͼ�������ⲿ·������
// ��  ��: Lixin
// ��  ��: 2007-05-06
// ��  Ȩ: �Ϻ���ͼ����������Ϣ�������޹�˾
///////////////////////////////////////////////////////////////////////////

#ifndef _SAVEV3D_3_1_H
#define _SAVEV3D_3_1_H

//------------------------------------------------------------------------
#include "CScene.h"
#include "V3D_3_1.h"

//------------------------------------------------------------------------
//##ModelId=47EBC74302BF
class CSaveV3D_3_1  
{
public:
	//##ModelId=47EBC74302C1
	V3D_3_1_Header				Header;					//V3D�ļ�ͷ
	//##ModelId=47EBC74302CF
	V3D_3_1_WindowProp			WindowProp;				//�������
	
	//##ModelId=47EBC74302D4
	V3D_3_1_AudioList			AudioData;				//�������ݵ���ʼλ��
	//##ModelId=47EBC74302D9
	V3D_3_1_FlashList			FlashData;				//Flash���ݵ���ʼλ��

	//##ModelId=47EBC74302DE
	V3D_3_1_CameraList			CameraData;				
	//##ModelId=47EBC74302E3
	V3D_3_1_MaterialList		MaterialData;			
	//##ModelId=47EBC74302E8
	V3D_3_1_LightList			LightData;				//�ƹ�����

	//##ModelId=47EBC74302ED
	V3D_3_1_VertexList			VertexData;				//��������
	//##ModelId=47EBC74302F2
	V3D_3_1_VertexPosList		VertexPosData;			//ֻ���������λ��
	//##ModelId=47EBC74302F7
	V3D_3_1_VertexNormalList	VertexNormalData;		//ֻ��������ķ���
	//##ModelId=47EBC74302FC
	V3D_3_1_UVList				UVData;					//��ͼ��UV���ݣ���λ�á����ߡ�UV�ֿ���������Ϊ�кܶඥ���λ�á����ߡ�UV����ͬ�ģ�����ֻ����һ��
	//##ModelId=47EBC7430301
	V3D_3_1_ModelList			ModelData;
	//##ModelId=47EBC7430306
	V3D_3_1_ModelGroup			ModelGroupData;			//ģ�����б�

	//##ModelId=47EBC743030B
	V3D_3_1_TextList			TextData;				//��������
	//##ModelId=47EBC7430310
	V3D_3_1_OverlayList			OverlayData;			//Overlay����
	//##ModelId=47EBC7430315
	V3D_3_1_NavigaterList		NavigaterData;			//Navigater����
	//##ModelId=47EBC743031A
	V3D_3_1_ButtonList			ButtonData;				//��ť�б�
	//##ModelId=47EBC743031F
	V3D_3_1_CubeMapList			CubeMapData;			//CubeMap�б�2007��2��25��������WebMax2.5�汾
	//##ModelId=47EBC7430324
	V3D_3_1_MovieTexList		MovieTexData;			//��Ӱ��ͼ�б�2007��3��17��������WebMax2.5�汾
	//##ModelId=47EBC7430329
	V3D_3_1_BumpWaterList		BumpWaterData;			//BumpWater��ͼ�б�2007��3��17������

	//##ModelId=47EBC743032E
	V3D_3_1_DynamicTexList		DynamicTexData;			//��̬��ͼ������ʼλ��

	//##ModelId=47EBC7430333
	V3D_3_1_Privacy				PrivacyData;			//��Ȩ����

	//##ModelId=47EBC7430338
	V3D_3_1_ControlData			ControlData;			//�û����ƶ�������

	//##ModelId=47EBC743033D
	V3D_3_1_TextureList			TexData;				//��ͼ����
	//##ModelId=47EBC7430342
	V3D_3_1_ImageList			ImageData;				//Image����

private:
	//##ModelId=47EBC7430347
	CScene*				pScene;					//ָ�򳡾�����Scene��ָ��
	//##ModelId=47EBC743034B
	FILE*				pFile;					//ָ��V3D�ļ���ָ��
	//##ModelId=47EBC743034C
	DWORD				nFileSize;				//V3D�ļ��Ĵ�С
	//##ModelId=47EBC743034D
	CHAR				FileName[256];			//V3D�ļ���

public:

	//д���ļ�ͷ
	//##ModelId=47EBC743035B
	VOID		WriteHeader();
	//д�봰������
	//##ModelId=47EBC743038A
	VOID		WriteWindowProp();
	
	//д����������
	//##ModelId=47EBC74303B9
	VOID		WriteAudioData();
	//д��Flash����
	//##ModelId=47EBC74303BA
	VOID		WriteFlashData();

	//д���������Ϣ
	//##ModelId=47EBC74303BB
	VOID		WriteCameraData();
	//д�������Ϣ
	//##ModelId=47EBC7440000
	VOID		WriteMaterialData();
	//д��ƹ���Ϣ
	//##ModelId=47EBC744001F
	VOID		WriteLightData();

	//д�붥��λ����Ϣ
	//##ModelId=47EBC744003E
	VOID		WriteVertexPosData(BOOL zipStatus);
	//д�붥�㷨����Ϣ
	//##ModelId=47EBC744005D
	VOID		WriteVertexNormalData(BOOL zipStatus);
	//д����ͼ������Ϣ
	//##ModelId=47EBC744007D
	VOID		WriteUVData(BOOL zipStatus);
	//д�붥����Ϣ
	//##ModelId=47EBC744009C
	VOID		WriteVertexData(BOOL zipStatus);
	//д��ģ����Ϣ
	//##ModelId=47EBC74400BB
	VOID		WriteModelData();
	//д��ģ������Ϣ
	//##ModelId=47EBC74400DA
	VOID		WriteModelGroupData();
	//##ModelId=47EBC74400DB
	VOID		WriteModelGroupData1(CModelGroup* pModelGroup);

	//д��������Ϣ
	//##ModelId=47EBC74400EB
	VOID		WriteTextData();
	//д��Overlay��Ϣ
	//##ModelId=47EBC74400FA
	VOID		WriteOverlayData();
	//д��Navigater��Ϣ
	//##ModelId=47EBC7440119
	VOID		WriteNavigaterData();
	//д��Button��Ϣ
	//##ModelId=47EBC7440128
	VOID		WriteButtonData();
	
	//д��CubeMap��Ϣ
	//##ModelId=47EBC7440138
	VOID		WriteCubeMapData();
	//д��MovieTex��Ϣ
	//##ModelId=47EBC7440139
	VOID		WriteMovieTexData();
	//д��BumpWater��Ϣ
	//##ModelId=47EBC7440148
	VOID		WriteBumpWaterData();

	//д�붯̬��ͼ��Ϣ
	//##ModelId=47EBC7440157
	VOID		WriteDynamicTexData();
	//д���ļ��汾���ͺ���Ȩ��ʾ
	//##ModelId=47EBC7440158
	VOID		WritePrivacyData();

	//д����ƶ�������
	//##ModelId=47EBC7440159
	VOID		WriteControlData();

	//д����ͼ��Ϣ
	//##ModelId=47EBC744015A
	VOID		WriteTexData();
	//д���ⲿͼ����Ϣ
	//##ModelId=47EBC7440177
	VOID		WriteImageData();

	//ѹ������λ�����ݣ�ȥ���ظ�����
	//##ModelId=47EBC7440178
	VOID		ZipVertexPosData(BOOL zipStatus);
	//�ҵ�pModel�ĵ�index������VertexPosData�����е�λ��
	//##ModelId=47EBC7440187
	UINT		FindOffsetInVertexPosData(UINT modelIndex, UINT vertexIndex);

	//ѹ�����㷨�����ݣ�ȥ���ظ�����
	//##ModelId=47EBC74401A5
	VOID		ZipVertexNormalData(BOOL zipStatus);
	//�ҵ�pModel�ĵ�index������VertexNormalData�����е�λ��
	//##ModelId=47EBC74401A7
	UINT		FindOffsetInVertexNormalData(UINT modelIndex, UINT vertexIndex);

	//ѹ������UV���ݣ�ȥ���ظ�����
	//##ModelId=47EBC74401B6
	VOID		ZipUVData(BOOL zipStatus);
	//�ҵ�pModel�ĵ�index�����UV��UVData�����е�λ��
	//##ModelId=47EBC74401C5
	UINT		FindOffsetInUVData(UINT modelIndex, UINT vertexIndex);

	//ѹ��ģ�����ݣ���ͼ���ݲ�ѹ��
	//##ModelId=47EBC74401D5
	HRESULT		ZipModelData();

	//�ͷ���Դ
	//##ModelId=47EBC74401D6
	VOID		Release();

	//�����Ӧ��Scene����ΪE3D�ļ�
	//##ModelId=47EBC74401E4
	HRESULT		SaveSceneAsV3D(CScene* pScene, CHAR* strFileName, BOOL zipStatus);	

	//�ڱ���֮ǰ��Ϊ�����������ñ�ţ���ֹ��Ϊɾ������󣬵��±�������Ŵ���
	//##ModelId=47EBC74401F5
	VOID		ResetModelIndex();
	//##ModelId=47EBC74401F6
	VOID		ResetTextureIndex();
	//##ModelId=47EBC74401F7
	VOID		ResetBumpWaterIndex();
	//##ModelId=47EBC74401F8
	VOID		ResetCubeMapIndex();
	//##ModelId=47EBC74401F9
	VOID		ResetMovieTexIndex();
	//##ModelId=47EBC7440203
	VOID		ResetMaterialIndex();
	//##ModelId=47EBC7440204
	VOID		ResetOverlayIndex();
	//##ModelId=47EBC7440205
	VOID		ResetDyTexIndex();

	//##ModelId=47EBC7440206
	VOID		ResetCameraIndex();
	//##ModelId=47EBC7440207
	VOID		ResetLightIndex();
	//##ModelId=47EBC7440208
	VOID		ResetNavigaterIndex();
	//##ModelId=47EBC7440209
	VOID		ResetTextIndex();

	//�����ƶ������±��
	//##ModelId=47EBC744020A
	VOID		ResetWalkerIndex();
	//##ModelId=47EBC744020B
	VOID		ResetEditorIndex();
	//##ModelId=47EBC744020C
	VOID		ResetFlyerIndex();

	//##ModelId=47EBC744020D
	VOID		ResetButtonIndex();
	//##ModelId=47EBC744020E
	VOID		ResetImageIndex();

public:
	//##ModelId=47EBC7440213
	CSaveV3D_3_1();
	//##ModelId=47EBC7440242
	~CSaveV3D_3_1();

};


//------------------------------------------------------------------------
#endif
