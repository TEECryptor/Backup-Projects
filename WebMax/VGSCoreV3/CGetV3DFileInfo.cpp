/////////////////////////////////////////////////////////////////////////
// ����: CGetV3DFileInfo--V3D�ļ���ȡ��
// ����: ��ȡV3D�ļ�����������������
// ����: lixin	
// ����: 2006-05-06
////////////////////////////////////////////////////////////////////////

#include "CGetV3DFileInfo.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=47EBC74B03D8
CGetV3DFileInfo::CGetV3DFileInfo(CRITICAL_SECTION*	pRenderCS)
{
}

//##ModelId=47EBC74C000F
CGetV3DFileInfo::~CGetV3DFileInfo()
{

}


//-------------------------------------------------------------------
//����: ReadHeader()
//����: 
//��;: ��ȡ�ļ�ͷ
//-------------------------------------------------------------------
//##ModelId=47EBC74B02E2
VOID CGetV3DFileInfo::ReadHeader()
{
}

//-------------------------------------------------------------------
//����: ReadVertexPosData()
//����: 
//��;: ��ȡ����λ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B032C
VOID CGetV3DFileInfo::ReadVertexPosData()
{
}

//-------------------------------------------------------------------
//����: ReadVertexNormalData()
//����: 
//��;: ��ȡ����ķ�����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B034B
VOID CGetV3DFileInfo::ReadVertexNormalData()
{
}

//-------------------------------------------------------------------
//����: ReadVertexUVData()
//����: 
//��;: ��ȡ�����UV��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B036B
VOID CGetV3DFileInfo::ReadVertexUVData()
{
}

//-------------------------------------------------------------------
//����: ReadVetexData()
//����: 
//��;: ��ȡ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B037A
VOID CGetV3DFileInfo::ReadVertexData()
{
}

//-------------------------------------------------------------------
//����: ReadModelData()
//����: 
//��;: ��ȡģ����Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B031C
VOID CGetV3DFileInfo::ReadModelData()
{
}

//-------------------------------------------------------------------
//����: ReadCameraData()
//����: 
//��;: ��ȡ�������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B02FD
VOID CGetV3DFileInfo::ReadCameraData()
{
}

//-------------------------------------------------------------------
//����: ReadTexData()
//����: 
//��;: ��ȡ��ͼ��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B03AB
VOID CGetV3DFileInfo::ReadTexData()
{
}

//-------------------------------------------------------------------
//����: ReadMaterialData()
//����: 
//��;: ��ȡ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B030D
VOID CGetV3DFileInfo::ReadMaterialData()
{
}

//-------------------------------------------------------------------
//����: ReadLightData()
//����: 
//��;: ����ƹ���Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B030E
VOID CGetV3DFileInfo::ReadLightData()
{
}

//-------------------------------------------------------------------
//����: ReadTextData()
//����: 
//��;: ��ȡ������Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B0399
VOID CGetV3DFileInfo::ReadTextData()
{
	/*
	fseek(pFile, Header.FileTable.TextDataPtr, SEEK_SET);

	//������������
	fread(&(TextData.TextCnt), sizeof(TextData.TextCnt), 1, pFile);

	//�����������Ϊ0������Ա������������б��ֶ�
	if (TextData.TextCnt == 0) return;

	//text data
	TextData.Texts = new V3D_Text[TextData.TextCnt];
	for (UINT i= 0; i< TextData.TextCnt; i++)
	{
		CText* pText = new CText();

		pText->index = i;
		
		//������������
		fread(&(TextData.Texts[i].Length), sizeof(TextData.Texts[i].Length), 1, pFile);

		//����ַ���Ϊ�գ�����Ա����ַ�����StrText�ֶ�
		if (TextData.Texts[i].Length > 0) 
		{
			TextData.Texts[i].StrText = new CHAR[TextData.Texts[i].Length];
			fread(TextData.Texts[i].StrText, TextData.Texts[i].Length, 1, pFile);
			pText->strText = new CHAR[TextData.Texts[i].Length];
			strcpy(pText->strText, TextData.Texts[i].StrText);
			pText->strText[TextData.Texts[i].Length] = '\0';
			//release memory
			SAFE_DELETE_ARRAY(TextData.Texts[i].StrText);
		}

		fread(&(TextData.Texts[i].Color), sizeof(TextData.Texts[i].Color), 1, pFile);
		pText->color = TextData.Texts[i].Color;

		fread(&(TextData.Texts[i].X), sizeof(TextData.Texts[i].X), 1, pFile);
		pText->rect.left = TextData.Texts[i].X ;

		fread(&(TextData.Texts[i].Y), sizeof(TextData.Texts[i].Y), 1, pFile);
		pText->rect.top = TextData.Texts[i].Y;

		pScene->text.push_back(pText);

	}

	//release memory
	SAFE_DELETE_ARRAY(TextData.Texts);
	*/
}

//-------------------------------------------------------------------
//����: ReadOverlayData()
//����: 
//��;: ��ȡOverlay��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B039A
VOID CGetV3DFileInfo::ReadOverlayData()
{
}

//-------------------------------------------------------------------
//����: ReadNavigaterData()
//����: 
//��;: ��ȡNavigate��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B03A9
VOID CGetV3DFileInfo::ReadNavigaterData()
{
}

//-------------------------------------------------------------------
//����: ReadButtonData()
//����: 
//��;: ��ȡButton��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B03AA
VOID CGetV3DFileInfo::ReadButtonData()
{
}

//-------------------------------------------------------------------
//����: ReadImageData()
//����: 
//��;: ��ȡImage��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B03B9
VOID CGetV3DFileInfo::ReadImageData()
{}

//-------------------------------------------------------------------
//����: ReadCubeMapData()
//����: 
//��;: ��ȡCubeMap��Ϣ
//-------------------------------------------------------------------
//##ModelId=47EBC74B03BA
VOID CGetV3DFileInfo::ReadCubeMapData()
{
}

//��ȡMovieTex��Ϣ��2.5�汾Up
//##ModelId=47EBC74B03BB
VOID CGetV3DFileInfo::ReadMovieTexData()
{
}

//��ȡBumpWater��Ϣ��2.5�汾Up
//##ModelId=47EBC74B03C8
VOID CGetV3DFileInfo::ReadBumpWaterData()
{
}

//-------------------------------------------------------------------
//����: FindReferenceModel()
//����: 
//��;: ����ÿ��ģ�͵�����ģ�Ͳ��õ�����ģ��ָ��
//-------------------------------------------------------------------
//##ModelId=47EBC74B03C9
VOID CGetV3DFileInfo::FindReferenceModel()
{
	
}

//-------------------------------------------------------------------
//����: LoadE3DFile()
//����: CScene* pScene, CHAR*  strFileName
//��;: ��ȡ��Ӧ��Scene����ΪE3D�ļ�
//-------------------------------------------------------------------
//##ModelId=47EBC74B03CA
HRESULT CGetV3DFileInfo::LoadV3DFile(CScene* theScene, CHAR*  strFileName)
{
	return S_OK;
}		

//##ModelId=47EBC74B03CD
VOID CGetV3DFileInfo::Release()
{
}
