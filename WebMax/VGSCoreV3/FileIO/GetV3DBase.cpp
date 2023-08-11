#include "GetV3DBase.h"
#include "GetV3DImpV2.h"
#include "GetV3DImpV3.h"

CGetV3DBase::CGetV3DBase(void)
{
}

CGetV3DBase::~CGetV3DBase(void)
{
}

CGetV3DBase * _getV3DLoader(const string &sFileName)
{
	CGetV3DBase *pV3DLoader = 0;

	FILE *pFile = NULL;
	fopen_s(&pFile, sFileName.c_str(), "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_SET); // ��λ���ļ���ͷ

		V3D_Header V3DHeader;
		fread(&V3DHeader, sizeof(V3DHeader), 1, pFile);

		// �й�˾��ʶ�������ļ��汾��С�ڵ��ڵ�ǰ����֧�ֵİ汾�ţ�����Ϊ��֧�ֵ�
		if (0 == strcmp(string(V3DHeader.corp).c_str(), SUNTOWARD_NAME))
		{
			 if (V3DHeader.v3dVersion < 60000)
			 {
				pV3DLoader = new CGetV3DImpV2();
			 }
			 else if (V3DHeader.v3dVersion >= 60000 && V3DHeader.v3dVersion <= CURRENT_V3DVERSION)
			 {
				pV3DLoader = new CGetV3DImpV3();
			 }
		}
		fclose(pFile);
	}
	return pV3DLoader;
}
