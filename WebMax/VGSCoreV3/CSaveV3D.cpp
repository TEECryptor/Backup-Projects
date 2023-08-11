///////////////////////////////////////////////////////////////////////////
// SaveV3D.cpp: interface for the CSaveE3D class.
// ��  ��: CSaveV3D
// ��  ��: �������ݱ����࣬���泡���е�����Ϊ*.V3D�ļ�, ��ͼ�������ⲿ·������
// ��  ��: Lixin
// ��  ��: 2007-04-09
// ��  Ȩ: �Ϻ���ͼ����������Ϣ�������޹�˾
///////////////////////////////////////////////////////////////////////////

#include "CSaveV3D.h"
#include "CMovieTexture.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74402DE
CSaveV3D::CSaveV3D()
{
	pScene = NULL;				//ָ�򳡾�����Scene��ָ��
	pFile = NULL;				//ָ��E3D�ļ���ָ��
	nFileSize = 0;				//E3D�ļ��Ĵ�С
}

//##ModelId=47EBC74402EE
CSaveV3D::~CSaveV3D()
{

}

//-------------------------------------------------------------------
//����: WriteHeader()
//����: 
//��;: д���ļ�ͷ
//-------------------------------------------------------------------
//##ModelId=47EBC7440376
VOID CSaveV3D::WriteHeader()
{
}

//-------------------------------------------------------------------
//����: WriteCameraData()
//����: 
//��;: д���������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC744038A
VOID CSaveV3D::WriteCameraData()
{
	
}

//-------------------------------------------------------------------
//����: WriteCameraData()
//����: 
//��;: д�������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC7440399
VOID CSaveV3D::WriteMaterialData()
{

}

//-------------------------------------------------------------------
//����: WriteLightData()
//����: 
//��;: д��ƹ���Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74403B9
VOID CSaveV3D::WriteLightData()
{}

//-------------------------------------------------------------------
//����: WriteModelData()
//����: 
//��;: д��ģ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74403C8
VOID CSaveV3D::WriteModelData()
{
}

//-------------------------------------------------------------------
//����: WriteVertexPosData()
//����: 
//��;: д�붥��λ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74403D8
VOID CSaveV3D::WriteVertexPosData(BOOL zipStatus)
{}


//-------------------------------------------------------------------
//����: WriteVertexNormalData()
//����: 
//��;: д�붥�㷨����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC7450000
VOID CSaveV3D::WriteVertexNormalData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//����: WriteVetexData()
//����: 
//��;: д�붥����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC745001F
VOID CSaveV3D::WriteVertexData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//����: WriteUVData()
//����: 
//��;: д����ͼ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC745000F
VOID CSaveV3D::WriteUVData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//����: WriteTexData()
//����: 
//��;: д����ͼ��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC745005E
VOID CSaveV3D::WriteTexData()
{

}

//-------------------------------------------------------------------
//����: WriteTextData()
//����: 
//��;: д��������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC745002E
VOID CSaveV3D::WriteTextData()
{
	
}

//-------------------------------------------------------------------
//����: WriteOverlayData()
//����: 
//��;: д��Overlay��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC745003E
VOID CSaveV3D::WriteOverlayData()
{
}

//-------------------------------------------------------------------
//����: WriteNavigaterData
//����: 
//��;: д��Navigater��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC745003F
VOID CSaveV3D::WriteNavigaterData()
{
}

//-------------------------------------------------------------------
//����: WriteButtonData()
//����: 
//��;: write the button data
//-------------------------------------------------------------------
//##ModelId=47EBC745004E
VOID CSaveV3D::WriteButtonData()
{
}

//-------------------------------------------------------------------
//����: WriteImageData
//����: 
//��;: д���ⲿͼ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC745006D
VOID CSaveV3D::WriteImageData()
{
}


//-------------------------------------------------------------------
//����: WriteCubeMapData()
//����: 
//��;: д��CubeMap ����
//-------------------------------------------------------------------
//##ModelId=47EBC745004F
VOID CSaveV3D::WriteCubeMapData()
{
}

//д��MovieTex��Ϣ
//##ModelId=47EBC7450050
VOID CSaveV3D::WriteMovieTexData()
{	
}

//д��BumpWater��Ϣ
//##ModelId=47EBC745005D
VOID CSaveV3D::WriteBumpWaterData()
{
}

//-------------------------------------------------------------------
//����: ZipVertexPosData()
//����: 
//��;: ��������λ����Ϣ��ȥ���ظ�����
//-------------------------------------------------------------------
//##ModelId=47EBC745006E
VOID CSaveV3D::ZipVertexPosData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//����: FindOffsetInVertexPosData()
//����: UINT modelIndex, UINT vertexIndex
//��;: �ҵ�pModel�ĵ�index������VertexPosData�����е�λ��
//-------------------------------------------------------------------
//##ModelId=47EBC7450070
UINT CSaveV3D::FindOffsetInVertexPosData(UINT modelIndex, UINT vertexIndex)
{
	return 0;

}

//-------------------------------------------------------------------
//����: ZipVertexNormalData()
//����: 
//��;: ѹ�����㷨�����ݣ�ȥ���ظ�����
//-------------------------------------------------------------------
//##ModelId=47EBC745007F
VOID CSaveV3D::ZipVertexNormalData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//����: FindOffsetInVertexNormalData()
//����: 
//��;: �ҵ�pModel�ĵ�index������VertexNormalData�����е�λ��
//-------------------------------------------------------------------
//##ModelId=47EBC745008C
UINT CSaveV3D::FindOffsetInVertexNormalData(UINT modelIndex, UINT vertexIndex)
{
	return 0;
}

//-------------------------------------------------------------------
//����: ZipUVData()
//����: 
//��;: ѹ��UV���ݣ�ȥ���ظ�����
//-------------------------------------------------------------------
//##ModelId=47EBC745008F
VOID CSaveV3D::ZipUVData(BOOL zipStatus)
{
}

//-------------------------------------------------------------------
//����: FindOffsetInVertexPosData()
//����: UINT modelIndex, UINT vertexIndex
//��;: �ҵ�pModel�ĵ�index�����UV��UVData�����е�λ��
//-------------------------------------------------------------------
//##ModelId=47EBC745009C
UINT CSaveV3D::FindOffsetInUVData(UINT modelIndex, UINT vertexIndex)
{
	return 0;
}

//-------------------------------------------------------------------
//����: ZipModelData()
//����: 
//��;: ѹ��ģ�����ݣ���ͼ���ݲ�ѹ��
//-------------------------------------------------------------------
//##ModelId=47EBC745009F
HRESULT CSaveV3D::ZipModelData()
{
	return S_OK;
}

//-------------------------------------------------------------------
//����: Release()
//����: 
//��;: �ͷ���Դ
//-------------------------------------------------------------------
//##ModelId=47EBC74500A0
VOID CSaveV3D::Release()
{
	
}

//-------------------------------------------------------------------
//����: SaveSceneAsV3D()
//����: CScene* pScene, CHAR*  strFileName
//��;: �����Ӧ��Scene����ΪV3D�ļ�
//-------------------------------------------------------------------
//##ModelId=47EBC74500A1
HRESULT CSaveV3D::SaveSceneAsV3D(CScene* theScene, CHAR*  strFileName, BOOL zipStatus)
{

	return S_OK;

}

//��ģ�Ͷ������±��
//##ModelId=47EBC74500AE
VOID CSaveV3D::ResetModelIndex()
{
}

//���¸���ͼ���б��
//##ModelId=47EBC74500AF
VOID CSaveV3D::ResetTextureIndex()
{
}


//��BumpWater���±��
//##ModelId=47EBC74500BB
VOID CSaveV3D::ResetBumpWaterIndex()
{	
}


//��CubeMap�����е�Ԫ�����±��
//##ModelId=47EBC74500BC
VOID CSaveV3D::ResetCubeMapIndex()
{}

//��MovieTex���±��
//##ModelId=47EBC74500BD
VOID CSaveV3D::ResetMovieTexIndex()
{
}

//�����ʶ������±��
//##ModelId=47EBC74500BE
VOID CSaveV3D::ResetMaterialIndex()
{
	CMaterial** ppMtrl = (CMaterial**)pScene->m_vShader.begin();
	INT mtrlCnt = pScene->m_vShader.size();
	for (INT i = 0; i< mtrlCnt; i++)
	{
		CMaterial* pMtrl = *(ppMtrl + i);
		pMtrl->index = i;
	}
}

//ΪOverlay���±��
//##ModelId=47EBC74500BF
VOID CSaveV3D::ResetOverlayIndex()
{
}

//Ϊ������б��
//##ModelId=47EBC74500C0
VOID CSaveV3D::ResetCameraIndex()
{
}

//Ϊ�ƹ���б��
//##ModelId=47EBC74500C1
VOID CSaveV3D::ResetLightIndex()
{
}

//Ϊ����ͼ�������±��
//##ModelId=47EBC74500C2
VOID CSaveV3D::ResetNavigaterIndex()
{

}

//Ϊ���ֶ������±��
//##ModelId=47EBC74500C3
VOID CSaveV3D::ResetTextIndex()
{
}

//��Button�������±��
//##ModelId=47EBC74500C4
VOID CSaveV3D::ResetButtonIndex()
{
}

//��Image�������±��
//##ModelId=47EBC74500C5
VOID CSaveV3D::ResetImageIndex()
{
}
