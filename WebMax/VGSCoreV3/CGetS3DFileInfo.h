/////////////////////////////////////////////////////////////////////////
// ����: GetS3DFileInfo--S3D�ļ���ȡ��
// ����: ��ȡS3D�ļ�����������������
// ����: lixin	
// ����: 2005-12-23
////////////////////////////////////////////////////////////////////////

#ifndef _CGETS3DFILEINFO_H
#define _CGETS3DFILEINFO_H

#include "S3D.h"
#include "CScene.h"

//-----------------------------------------------------------------------------
//������GetS3DFileInfo
//��;�����ڴ�S3D�ļ�������ȡ��������
//-----------------------------------------------------------------------------
//##ModelId=47EBC74C007D
class CGetS3DFileInfo
{
//------------------------------------------------------------------------------
//���ڱ��泡�����ݵı���
//------------------------------------------------------------------------------
private:
	//##ModelId=47EBC74C008D
	S3DHEADER Header; //�ļ�ͷ
	//##ModelId=47EBC74C0092
	SCENETABLE SceneTable; //������Ϣ�б�

	//##ModelId=47EBC74C0097
	GCS SceneGCS; //����ȫ������ϵ

	//##ModelId=47EBC74C009C
	NAME* TexNameList; //��ͼ�ļ������б� (from 1 to Texture Count), use 32 byte per TexName
	//##ModelId=47EBC74C00A0
	UINT TextureCnt; //��ͼ������
	//##ModelId=47EBC74C00A1
	UINT TexNameListOffset; //��ͼ�ļ����б����ʼλ�ã�����ļ���ͷ��ƫ����

	//##ModelId=47EBC74C00A3
	MATERIAL *MaterialInfoList;//���������б�
	//##ModelId=47EBC74C00A7
	UINT MaterialCnt; //��������
	//##ModelId=47EBC74C00A8
	UINT MaterialInfoListOffset; //���������б����ʼλ�ã�����ļ��Ŀ�ͷ��ƫ����

	//##ModelId=47EBC74C00AC
	ORIFACE *FaceInfoList; //��������б�
	//##ModelId=47EBC74C00B1
	FACE *FaceInfoList1; //FaceInfoList�⿪�����ϸ����
	//##ModelId=47EBC74C00B5
	UINT FaceCnt; //������
	//##ModelId=47EBC74C00B6
	UINT FaceInfoListOffset; //�������б����ʼλ�ã�����ļ��Ŀ�ͷ��ƫ����

	//##ModelId=47EBC74C00BC
	VERTEX * VertexInfoList; //����������б�
	//##ModelId=47EBC74C00C0
	UINT VertexCnt; //��������
	//##ModelId=47EBC74C00C1
	UINT VertexInfoListOffset; //���������б����ʼλ�ã�����ļ��Ŀ�ͷ��ƫ����

	//##ModelId=47EBC74C00C3
	DCAMINFO * CameraInfoList;//���������ϸ�����б�,�����ǰ�����û�ж�������û������
	//##ModelId=47EBC74C00CC
	CAMERA *CameraList;//��Ӱ���ĸ�Ҫ�����б�����ָ���Ӧ��ϸ���ݵ�ָ��
	//##ModelId=47EBC74C00D0
	UINT CameraCnt;//���������
	//##ModelId=47EBC74C00D1
	UINT CameraListOffset;//�������Ҫ�����б����ʼλ�ã�����ļ���ͷ��ƫ����

	//##ModelId=47EBC74C00D3
	UNIT * UnitInfoList; //ģ�������б�
	//##ModelId=47EBC74C00DA
	UINT UnitCnt; //ģ��������û�йؼ�֡����ʱ
	//##ModelId=47EBC74C00DB
	UINT UnitInfoListOffset; //ģ����Ϣƫ������û�йؼ�֡����ʱ

	//##ModelId=47EBC74C00DD
	DUNITINFO *DUnitInfoList;//ģ�ͱ任�����йؼ�֡����ʱ
	//##ModelId=47EBC74C00E2
	DVTXUNITINFO *VTXDUnitInfoList;//��̬���������б���ģ��Ϊ��λ�����ж��㶯��ʱ
	//##ModelId=47EBC74C00EB
	DUNIT * DUnitList; //�ؼ�֡����ģ����Ϣ���йؼ�֡����ʱ

	//##ModelId=47EBC74C00EF
	UINT KeyFrameCnt;//��������Ĺؼ�֡�����������ǹؼ�֡���ݶ�ȡ�����
	//##ModelId=47EBC74C00F0
	UINT DUnitCnt; //�����������ģ������,���йؼ�֡����ʱ
	//##ModelId=47EBC74C00FA
	UINT DUnitListOffset;//�ؼ�֡����ģ�����ݵ���ʼλ�ã�����ļ���ͷ��ƫ��������DMODEL * DModelInfo��;

	//##ModelId=47EBC74C00FC
	LIGHTFRAME* LightInfoList;//���������еƹ���ϸ��Ϣ�����б�
	//##ModelId=47EBC74C0101
	LIGHT*	LightList;		//�����еĵƹ��Ҫ��Ϣ�����б�
	//##ModelId=47EBC74C0109
	UINT LightCnt; //�ƹ�����
	//##ModelId=47EBC74C010A
	UINT LightListOffset; //�ƹ��Ҫ��Ϣ�б����ʼλ�ã�����ļ���ͷ��ƫ����

//------------------------------------------------------------------------------	
//�������ݴ��ݵı���
//------------------------------------------------------------------------------
private:
	//�ļ��йصĳ�Ա����---------------------------------------------------------------------------
	//##ModelId=47EBC74C010B
	CHAR					m_StrS3DFilePath[256];			//��ǰS3D�ļ���·��
	//##ModelId=47EBC74C0119
	CHAR					m_FileName[256];				//�ļ���
	//##ModelId=47EBC74C011A
	FILE*					S3DFile;						//S3D�ļ�������
	//##ModelId=47EBC74C011B
	FILE*					DebugFile;						//���ڵ��Ե�������
	//##ModelId=47EBC74C0129
	CScene*					pScene;							//����
	
	//���ļ�ͷ
	//##ModelId=47EBC74C012D
	VOID ReadFileHeader();
	//��������Ϣ��
	//##ModelId=47EBC74C0148
	VOID ReadSceneTable();
	//����������ϵ
	//##ModelId=47EBC74C0167
	VOID ReadSceneGCS();

	//����ͼ��Ϣ
	//##ModelId=47EBC74C0186
	VOID ReadTextureInfo();
	//��������Ϣ
	//##ModelId=47EBC74C0196
	VOID ReadMaterialInfo();

	//��ģ����Ϣ
	//##ModelId=47EBC74C01B5
	VOID ReadUnitInfo();
	//������Ϣ
	//##ModelId=47EBC74C01C5
	VOID ReadFaceInfo();
	//��������Ϣ
	//##ModelId=47EBC74C01D4
	VOID ReadVerticesInfo();
	//��ģ�͹ؼ�֡������Ϣ
	//##ModelId=47EBC74C01F4
	VOID ReadUnitKeyFrameInfo();

	//��ȡ�������Ϣ
	//##ModelId=47EBC74C0203
	VOID ReadCameraInfo();
	//��ȡ�ƹ���Ϣ
	//##ModelId=47EBC74C0213
	VOID ReadLightInfo();

	//�õ���Ͷ���ķ��ߣ�����ķ���Ϊ���øö������ķ���ƽ��ֵ
	//##ModelId=47EBC74C0222
	VOID CalculateNormal(CModel* pModel, CFace* pFace);

	//��2��DWORD������������ȡ����INT��������,֮ǰ���������ԭ����Ϊ�˽�ʡ�ռ�
	//##ModelId=47EBC74C0232
	inline VOID DetachData_1(DWORD ab, DWORD bc, DWORD *a, DWORD *b, DWORD *c)
	{
		//�����㷨�ο���˾λ�����ĵ�
		*a = (ab >> 10) & 0x003fffff;
		*b = ((ab << 11) & 0x001ff800) + ((bc >> 21) & 0x000007ff); //��+�ţ�����&
		*c = bc & 0x001fffff;
	}

	//--------------------------------------------------------------------------
	//��S3DFile�����ȡ���ݣ��������ݹ�����Scene��
	//--------------------------------------------------------------------------
	//������ͼ����
	//##ModelId=47EBC74C0243
	VOID ReadTextureData();
	//�����������
	//##ModelId=47EBC74C0244
	VOID ReadMtrlData();
	//�������������
	//##ModelId=47EBC74C0251
	VOID ReadCameraData();
	//����ģ������
	//##ModelId=47EBC74C0252
	VOID ReadModelData();
	//�õ��ƹ�����
	//##ModelId=47EBC74C0253
	VOID ReadLightData();
	//�õ�ģ���б��е�ģ�ͱ�Ŷ�Ӧ�Ķ�̬ģ���б��еı��
	//##ModelId=47EBC74C0261
	INT  GetDUnitIndex(UINT unitIndex);


//------------------------------------------------------------------------------
//���ڵ���˽�����ݵķ���
//------------------------------------------------------------------------------
public:
	//��S3D�ļ�������
	//##ModelId=47EBC74C0263
	HRESULT LoadS3DFile(CScene* pScene, CHAR* strFileName, BOOL bImportModel, BOOL bImportLight, BOOL bImportCamera);
	//��S3D�ļ�
	//##ModelId=47EBC74C0280
	HRESULT	LoadS3DFile(CHAR* strFileName);
	//������ͼ����Ŀ���
	//##ModelId=47EBC74C0290
	UINT	GetTextureCount();
	//##ModelId=47EBC74C0291
	CHAR*	GetTextureName(UINT index);
	//�ͷ�S3D���󣬻����ڴ�
	//##ModelId=47EBC74C0293
	VOID Release();


public:
	//##ModelId=47EBC74C0294
	CGetS3DFileInfo();
	//##ModelId=47EBC74C02AF
	~CGetS3DFileInfo();

};
#endif
