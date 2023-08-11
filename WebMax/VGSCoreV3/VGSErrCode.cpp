/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSErrCode.h
* ����ժҪ�� // ��ȡ�����뼰������������ط���
* ����˵���� // 
* ��ǰ�汾�� // V2.0
* ��    �ߣ� // ���²�
* ������ڣ� // 
* 
* �޸�����      �汾��     �޸���	      �޸�����
* ---------------------------------------------------------------
* 
*
*********************************************************************/

#include "VGSErrCode.h"

// UINT g_LastErrorCode;

std::map <UINT, std::string> CVGSErrCode::m_vErrInfo;

UINT CVGSErrCode::GetLastErrorCode()
{
	return g_LastErrorCode;
}
string CVGSErrCode::GetLastErrorStr()
{
	if (m_vErrInfo.find(g_LastErrorCode) != m_vErrInfo.end())
	{
		return m_vErrInfo[g_LastErrorCode];
	}
	return m_vErrInfo[ERR_COMMON_UNKNOWN];
}
string CVGSErrCode::GetErrorStr(UINT ErrCode)
{
	if (m_vErrInfo.find(ErrCode) != m_vErrInfo.end())
	{
		return m_vErrInfo[ErrCode];
	}
	return m_vErrInfo[ERR_COMMON_UNKNOWN];
}

CVGSErrCode::CVGSErrCode()
{
	m_vErrInfo.clear();

	//////////////// ���� ///////////////////////////////
	m_vErrInfo[ERR_COMMON_OK]           = "OK";	
	m_vErrInfo[ERR_COMMON_UNKNOWN]      = "�޷���ȡ������Ϣ��";	

    //////////////// �ļ���� ////////////////////////////
	
	m_vErrInfo[ERR_FILE_V3D_NEWVERSION]     = "V3D�ļ���ʽ̫�£��޷���ȡ��";
	m_vErrInfo[ERR_FILE_V3D_WRONGFORMAT]    = "V3D�ļ���ʽ����ȷ���޷���ȡ��";
	m_vErrInfo[ERR_FILE_V3D_READHEAD]       = "��V3D�ļ�ͷ���������޷���ȡ�ļ�";
	m_vErrInfo[ERR_FILE_V3D_READHEAD]       = "��V3D�ļ�ͷ���������޷���ȡ�ļ�";
	m_vErrInfo[ERR_FILE_V3D_OLDVERSION]     = "V3D�ļ����ڵ�ǰ����֧�ֵİ汾";
	m_vErrInfo[ERR_FILE_OPENFAIL]			= "���ļ���������";
	m_vErrInfo[ERR_FILE_READBYTEFAIL]		= "���ļ�ʱδ��ȡԤ�����ֽ���Ŀ";
	m_vErrInfo[ERR_FILE_V3D_NODATA]		    = "���ļ�ʱδ��������";


	m_vErrInfo[ERR_FILE_SAVEV3D_NODATA]    	= "����V3Dʱ����ȡ�������ݴ���";
	m_vErrInfo[ERR_FILE_SAVEV3D_OPENFILE] 	= "����V3Dʱ�����ļ�����";
	m_vErrInfo[ERR_FILE_SAVEV3D_FAIL] 		= "����V3D��������";

	m_vErrInfo[ERR_FILE_SAVEV3D_HEAD]    			= "����V3Dʱ��д���ļ�ͷ��������";
	m_vErrInfo[ERR_FILE_SAVEV3D_TABLE]  			= "����V3Dʱ��д��Ŀ¼��������";
	m_vErrInfo[ERR_FILE_SAVEV3D_SCENEPROPERTY]  	= "����V3Dʱ��д�볡����Ϣ��������";
	m_vErrInfo[ERR_FILE_SAVEV3D_ENTITY]    			= "����V3Dʱ��д��ģ�ͷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_MESH]  				= "����V3Dʱ��д��ģ�����ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_MATERIAL]  			= "����V3Dʱ��д��������ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_CAMERA]    			= "����V3Dʱ��д��������ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_LIGHT]  			= "����V3Dʱ��д��ƹ����ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_SCENEGRAPH]  		= "����V3Dʱ��д�볡��ͼ���ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_OVERLAY]    		= "����V3Dʱ��д��ǰ��ͼ���ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_BUTTON]  			= "����V3Dʱ��д�밴ť���ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_NAVIGATOR]  		= "����V3Dʱ��д�뵼��ͼ���ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_AUDIO]    			= "����V3Dʱ��д���������ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_CONTORLLER]  		= "����V3Dʱ��д����ƶ������ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_ANNIMATION] 		= "����V3Dʱ��д�붯�����ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_BACKDROP]    		= "����V3Dʱ��д�뱳��ͼ���ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_TEXT]  				= "����V3Dʱ��д���������ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_TEXTURE]			= "����V3Dʱ��д���������ݷ�������";
	m_vErrInfo[ERR_FILE_SAVEV3D_CLOSEFILE]			= "����V3Dʱ���ر��ļ�ʱ��������";
	m_vErrInfo[ERR_FILE_SAVEV3D_FALSH]				= "����V3Dʱ��д��Flashʱ��������";

	m_vErrInfo[ERR_FILE_N3D_SAVE]		    = "����N3D��������";
	m_vErrInfo[ERR_FILE_N3D_LOAD]           = "����N3D�������";
	m_vErrInfo[ERR_FILE_N3D_SCENELOAD]      = "����N3D������������";
	m_vErrInfo[ERR_FILE_N3D_IMAGELOAD]      = "����N3D��ͼ��������";
	m_vErrInfo[ERR_FILE_N3D_VERSIONLOW]     = "N3D�汾̫�ͣ�";
	m_vErrInfo[ERR_FILE_N3D_VERSIONHIGH]    = "N3D�汾̫�ߣ�";

	m_vErrInfo[ERR_FILE_E3D_SAVE]		    = "����E3D��������";
	m_vErrInfo[ERR_FILE_E3D_LOAD]		    = "����E3D��������";
	m_vErrInfo[ERR_FILE_E3D_LOWVER]         = "E3D�ļ��汾���ͣ�";
	m_vErrInfo[ERR_FILE_E3D_FORMAT]         = "E3D�ļ���ʽ����ȷ��";


	m_vErrInfo[ERR_SCENE_NOSCENE]       = "�޷���ȡ������";
	m_vErrInfo[ERR_SCENE_CREATE]        = "��������ʧ�ܣ�";

    //////////////// ģ����� ////////////////////////////

	//////////////// ������ ////////////////////////////


	//////////////// �ƹ���� ////////////////////////////

	//////////////// ������� ////////////////////////////
}