/////////////////////////////////////////////////////////////////////////
// ����: CGetV3DFileInfo_3_1 -- V3D 1.0�汾�ļ���ȡ��
// ����: ��ȡV3D�ļ�����������������
// ����: lixin	
// ����: 2007-05-09
////////////////////////////////////////////////////////////////////////

#include "CGetV3DFileInfo_3_1.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74B01B7
CGetV3DFileInfo_3_1::CGetV3DFileInfo_3_1(CRITICAL_SECTION*	pRenderCS)
{
}

//##ModelId=47EBC74B01F4
CGetV3DFileInfo_3_1::~CGetV3DFileInfo_3_1()
{

}


//-------------------------------------------------------------------
//����: ReadHeader()
//����: 
//��;: ��ȡ�ļ�ͷ
//-------------------------------------------------------------------
//##ModelId=47EBC74A02C1
VOID CGetV3DFileInfo_3_1::ReadHeader()
{
}
	
//��ȡ������Ϣ
//##ModelId=47EBC74A02FD
VOID CGetV3DFileInfo_3_1::ReadWindowProp()
{
}

//������Ȩ��Ϣ
//##ModelId=47EBC74B0138
VOID CGetV3DFileInfo_3_1::ReadPrivacyData()
{
}

//��ȡ������Ϣ
//##ModelId=47EBC74B0177
VOID  CGetV3DFileInfo_3_1::ReadContorlData()
{
}

//��ȡ��������
//##ModelId=47EBC74A032C
VOID  CGetV3DFileInfo_3_1::ReadAudioData()
{
}

//��ȡFlash����
//##ModelId=47EBC74A035B
VOID  CGetV3DFileInfo_3_1::ReadFlashData()
{
}

//��ȡģ�����б���Ϣ
//##ModelId=47EBC74B00AB
VOID  CGetV3DFileInfo_3_1::ReadModelGroupData1(CModelGroup* pModelGroup)
{

}

//��ȡģ�����б���Ϣ
//##ModelId=47EBC74B007D
VOID  CGetV3DFileInfo_3_1::ReadModelGroupData()
{
}

//��ȡ��̬��ͼ��Ϣ
//##ModelId=47EBC74B0119
VOID  CGetV3DFileInfo_3_1::ReadDynamicTexData()
{
}

//-------------------------------------------------------------------
//����: ReadVertexPosData()
//����: 
//��;: ��ȡ����λ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74A03B9
VOID CGetV3DFileInfo_3_1::ReadVertexPosData()
{}

//-------------------------------------------------------------------
//����: ReadVertexNormalData()
//����: 
//��;: ��ȡ����ķ�����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74A03D8
VOID CGetV3DFileInfo_3_1::ReadVertexNormalData()
{}

//-------------------------------------------------------------------
//����: ReadVertexUVData()
//����: 
//��;: ��ȡ�����UV��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B001F
VOID CGetV3DFileInfo_3_1::ReadVertexUVData()
{
}

//-------------------------------------------------------------------
//����: ReadVetexData()
//����: 
//��;: ��ȡ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B003E
VOID CGetV3DFileInfo_3_1::ReadVertexData()
{
}

//-------------------------------------------------------------------
//����: ReadModelData()
//����: 
//��;: ��ȡģ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B005D
VOID CGetV3DFileInfo_3_1::ReadModelData()
{
	
}

//-------------------------------------------------------------------
//����: ReadCameraData()
//����: 
//��;: ��ȡ�������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74A038A
VOID CGetV3DFileInfo_3_1::ReadCameraData()
{
	
}

//-------------------------------------------------------------------
//����: ReadTexData()
//����: 
//��;: ��ȡ��ͼ��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B00FA
VOID CGetV3DFileInfo_3_1::ReadTexData()
{
}

//-------------------------------------------------------------------
//����: ReadMaterialData()
//����: 
//��;: ��ȡ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74A0399
VOID CGetV3DFileInfo_3_1::ReadMaterialData()
{
}

//-------------------------------------------------------------------
//����: ReadLightData()
//����: 
//��;: ����ƹ���Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74A03A9
VOID CGetV3DFileInfo_3_1::ReadLightData()
{
}

//-------------------------------------------------------------------
//����: ReadTextData()
//����: 
//��;: ��ȡ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B00DA
VOID CGetV3DFileInfo_3_1::ReadTextData()
{
}

//-------------------------------------------------------------------
//����: ReadOverlayData()
//����: 
//��;: ��ȡOverlay��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B00EA
VOID CGetV3DFileInfo_3_1::ReadOverlayData()
{
}

//-------------------------------------------------------------------
//����: ReadNavigaterData()
//����: 
//��;: ��ȡNavigate��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B00EB
VOID CGetV3DFileInfo_3_1::ReadNavigaterData()
{
}

//-------------------------------------------------------------------
//����: ReadButtonData()
//����: 
//��;: ��ȡButton��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B00EC
VOID CGetV3DFileInfo_3_1::ReadButtonData()
{

}

//-------------------------------------------------------------------
//����: ReadImageData()
//����: 
//��;: ��ȡImage��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B0109
VOID CGetV3DFileInfo_3_1::ReadImageData()
{
}

//-------------------------------------------------------------------
//����: ReadCubeMapData()
//����: 
//��;: ��ȡCubeMap��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B010A
VOID CGetV3DFileInfo_3_1::ReadCubeMapData()
{

}

//��ȡMovieTex��Ϣ��2.5�汾Up
//##ModelId=47EBC74B010B
VOID CGetV3DFileInfo_3_1::ReadMovieTexData()
{
}

//��ȡBumpWater��Ϣ��2.5�汾Up
//##ModelId=47EBC74B010C
VOID CGetV3DFileInfo_3_1::ReadBumpWaterData()
{
}

//-------------------------------------------------------------------
//����: FindReferenceModel()
//����: 
//��;: ����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
//-------------------------------------------------------------------
//##ModelId=47EBC74B01A5
VOID CGetV3DFileInfo_3_1::FindReferenceModel()
{
}

//-------------------------------------------------------------------
//����: LoadE3DFile()
//����: CScene* pScene, CHAR*  strFileName
//��;: ��ȡ��Ӧ��Scene����ΪE3D�ļ�
//-------------------------------------------------------------------
//##ModelId=47EBC74B01A6
HRESULT CGetV3DFileInfo_3_1::LoadV3DFile(CScene* theScene, CHAR*  strFileName)
{
	return S_OK;
}		

//##ModelId=47EBC74B01B5
VOID CGetV3DFileInfo_3_1::Release()
{
}
