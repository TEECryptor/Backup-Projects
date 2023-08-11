/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // GetV3D.cpp
* ����ժҪ�� // ��ȡV3D�ļ�ʵ��
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


#include "GetV3DImpV2.h"
#include "..\CSysFunc.h"
#include "..\VGSErrCode.h"
#include "..\VGSSupport.h"
#include "..\VGSDef.h"
#include "..\Root.h"

#pragma pack(push, 4) // ���ݽṹ��4�ֽڶ���
///����Editor�����ݽṹ
typedef struct EditorInfo_Old
{
	CHAR		Name[MAX_NAMESTR_LENGTH];							//Editor������
	// INT			cameraIndex;					//ʹ�õ�������
	CHAR        Camera[MAX_NAMESTR_LENGTH];						    //ʹ�õ��������

	BOOL		bIsAutoRoundRoll;					//Editģʽ���Ƿ��Զ�����

	FLOAT		autoSpeedX;						    // Editģʽ���Զ��� X ����ת���ٶȣ���λ����/����
	FLOAT		autoSpeedY;						    // Editģʽ���Զ��� Y ����ת���ٶȣ���λ����/����

	FLOAT		manualSpeedX;					    //Editģʽ���ֶ�����������ٶ��� X ��
	FLOAT		manualSpeedY;					    //Editģʽ���ֶ���ת������ٶ��� Y ��

	FLOAT		manualPanXSpeed;					//ƽ�Ƶ�X�����ٶ� 
	FLOAT		manualPanYSpeed;					//ƽ�Ƶ�Y�����ٶ� 

	FLOAT		dollySpeed;							//��������ʱ�ƶ�������ٶ� [1, 100]
	FLOAT		dollyAcc;							//����������ٶ�

	FLOAT		dollyNearDis;						//�����Dolly
	FLOAT		dollyFarDis;						//��Զ��Dolly

	INT			rosAxis;							// ��ת���־λ, 0: xy, 1 : x, 2 : y
	VECTOR3     LookAtPos;                          // Editor��Ŀ��� 

	DWORD        LookAtModelIndex;                   // fuxb 2008-12-15 �������ĳ��ģ�ͣ�����ģ�͵����������û��ģ�ͣ���ֵΪ0xffffffff
}EditorInfo_Old;
#pragma pack(pop)

// texture������
enum V3D_TextureType_Old
{
	VTT_INVALID = 0,  // ��Ч��ͼ
	VTT_BASAL,		  // ������ͼ
	VTT_CUBIC,
	VTT_DYNAMIC,
	VTT_MOVIE,
	VTT_FLASH,
	VTT_BUMP
};

CGetV3DImpV2::CGetV3DImpV2(/*LPDIRECT3DDEVICE9 pd3dDevice*/)
{
	//m_pD3dDevice = pd3dDevice;

	m_pVGSSceneMgr = NULL;
	m_pFile = NULL;

	m_sV3DPath = "";
	m_sDefaultTexturePath = "";

	m_V3DFileVersion = CURRENT_V3DVERSION;

	m_pMaterailBuf = NULL;
	m_pMeshBuf = NULL;
	m_pEntityBuf = NULL;
	m_pCameraBuf = NULL;
	m_pLightBuf = NULL;
	m_pOverlayBuf = NULL;
	m_pButtonBuf = NULL;
	m_pNaviBuf = NULL;
	m_pTextBuf = NULL;
	m_pPrivacyBuf = NULL;
	m_pFlashBuf = NULL;
	m_pAudioBuf = NULL;
	m_pControlBuf = NULL;
	m_pAnimationBuf = NULL;
	m_pSceneGraphBuf = NULL;
	m_pTextureBuf = NULL;
	m_pBackdropBuf = NULL;

}

CGetV3DImpV2::~CGetV3DImpV2()
{
	m_pVGSSceneMgr = NULL;
	m_pFile = NULL;
	m_sV3DPath = "";

	DELETE_ARRAYPTR(m_pMeshBuf);
	DELETE_ARRAYPTR(m_pEntityBuf);
	DELETE_ARRAYPTR(m_pCameraBuf);
	DELETE_ARRAYPTR(m_pLightBuf);
	DELETE_ARRAYPTR(m_pOverlayBuf);
	DELETE_ARRAYPTR(m_pButtonBuf);
	DELETE_ARRAYPTR(m_pNaviBuf);
	DELETE_ARRAYPTR(m_pTextBuf);
	DELETE_ARRAYPTR(m_pPrivacyBuf);
	DELETE_ARRAYPTR(m_pFlashBuf);
	DELETE_ARRAYPTR(m_pAudioBuf);
	DELETE_ARRAYPTR(m_pControlBuf);
	DELETE_ARRAYPTR(m_pAnimationBuf);
	DELETE_ARRAYPTR(m_pSceneGraphBuf);
	DELETE_ARRAYPTR(m_pTextureBuf);
	DELETE_ARRAYPTR(m_pBackdropBuf);
}

void CGetV3DImpV2::Release()
{
	DELETE_ARRAYPTR(m_pMeshBuf);
	DELETE_ARRAYPTR(m_pEntityBuf);
	DELETE_ARRAYPTR(m_pCameraBuf);
	DELETE_ARRAYPTR(m_pLightBuf);
	DELETE_ARRAYPTR(m_pOverlayBuf);
	DELETE_ARRAYPTR(m_pButtonBuf);
	DELETE_ARRAYPTR(m_pNaviBuf);
	DELETE_ARRAYPTR(m_pTextBuf);
	DELETE_ARRAYPTR(m_pPrivacyBuf);
	DELETE_ARRAYPTR(m_pFlashBuf);
	DELETE_ARRAYPTR(m_pAudioBuf);
	DELETE_ARRAYPTR(m_pControlBuf);
	DELETE_ARRAYPTR(m_pBackdropBuf);
}

// ����v3d�ļ������õ���ǰ��V3DĿ¼
void CGetV3DImpV2::SetCurrentDirctory(const std::string &sFileName)
{
	int maxlen = sFileName.length();
	m_sV3DPath = _GetPureFilePathStr(sFileName);
	m_sDefaultTexturePath = m_sV3DPath;
	m_sDefaultTexturePath.append(std::string("resource\\texture\\"));
}

// ��ȡV3D�ļ���TableĿ¼
bool CGetV3DImpV2::GetV3DTable(const std::string &strFileName, V3D_Table &Table)
{
	// �����ļ�
	fopen_s(&m_pFile, strFileName.c_str(), "rb");
	if (NULL == m_pFile)
	{
		return false;
	}
	
	// ���ļ�ͷ
	if (ERR_COMMON_OK != ReadHeader())
	{
		fclose(m_pFile);
		return false;
	}

	// ��table
	if (ERR_COMMON_OK != ReadFileTable())
	{
		fclose(m_pFile);
		return false;
	}

	fclose(m_pFile);

	Table = m_V3DTable;

	return true; 
}

// �Ƿ�����֧�ֵİ汾
BOOL CGetV3DImpV2::isMyVersion(const std::string &sFileName) 
{
	BOOL re = FALSE;
	FILE *pFile = NULL;
	fopen_s(&pFile, sFileName.c_str(), "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_SET); // ��λ���ļ���ͷ

		V3D_Header V3DHeader;
		fread(&V3DHeader, sizeof(V3DHeader), 1, pFile);
		
		// �й�˾��ʶ�������ļ��汾��С�ڵ��ڵ�ǰ����֧�ֵİ汾�ţ�����Ϊ��֧�ֵ�
		if (0 == strcmp(string(V3DHeader.corp).c_str(), SUNTOWARD_NAME) && V3DHeader.v3dVersion <= CURRENT_V3DVERSION)
		{
			re = TRUE;
		}
		fclose(pFile);
	}
	return re;
}

DWORD CGetV3DImpV2::OpenV3DFile(CScene* pVGSSceneMgr, const std::string &strFileName, HWND hWnd)
{
	try
	{
		m_hWnd = hWnd;

		if (pVGSSceneMgr == NULL)
		{
			// ::MessageBox(NULL, LPCSTR("�Ҳ�������"), LPCSTR("����"), MB_OK | MB_ICONERROR);
			return ERR_SCENE_NOSCENE; // �޷���ȡ������
		}

		m_pVGSSceneMgr = (CScene*)pVGSSceneMgr;

		SetRoot(m_pVGSSceneMgr->GetRoot());

		m_pVGSSceneMgr->Reset();  // ��ճ�������

		// �����ļ�
		fopen_s(&m_pFile, strFileName.c_str(), "rb");
		if (NULL == m_pFile)
		{
			// ::MessageBox(NULL, LPCSTR("�����ļ�ʧ��"), LPCSTR("����"), MB_OK | MB_ICONERROR);
			return ERR_FILE_OPENFAIL;
		}
		
		SetCurrentDirctory(strFileName);  // �õ���ǰ��V3DĿ¼�����ڼ�����ͼ����Դ

		// m_vCameraDetail.clear();
		m_vpCamera.clear();
		m_vpLight.clear();

		// ������
		m_ProcessTotalSize = 33;
		m_ProcessCurSize   = 0;

		// Read the header data
		DWORD result = ReadHeader();
		RETURN(result);

		result = ReadFileTable();
		RETURN(result);

		//Read the window prop
		result = ReadWindowProp();
		RETURN(result);


		// ��ȡ��������
		ReadAllData();

		// �ر��ļ�
		fclose(m_pFile);


		/////////////// �������� /////////////////////

		//Read the  camera data
		result = ReadCameraData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read the light data
		result = ReadLightData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read the texture data
		result = ReadTextureData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read the Material data
		result = ReadMaterialData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		// Read the Model data
		result = ReadMeshData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		// Read the Model data
		result = ReadModelData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read the Text data
		result = ReadTextData();
		RETURN(result);

		//Read the overlay data
		result = ReadOverlayData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read the navigater data
		result = ReadNavigaterData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read the button data
		result = ReadButtonData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read the backdrop
		result = ReadBackdropData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read AudioData
		result = ReadAudioData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read FlashData
		result = ReadFlashData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		// ����
		result = ReadAnimationData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		//Read privacy data
		result = ReadPrivacyData();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		// ����
		result = ReadControlData(); 
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		result = ReadSceneGraph();
		RETURN(result);
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		PostOpen();  // �������ɺ��һЩ����
		PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

		if (m_ProcessCurSize <= m_ProcessTotalSize)
		{
			PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessTotalSize, m_ProcessTotalSize);  // ��ʼ����
		}

		Release(); // �ͷ�buf�ڴ�

		return ERR_COMMON_OK;
	}
	catch (...)
	{
		//fclose(m_pFile);
		return ERR_FILE_OPENFAIL;
	}
}

DWORD CGetV3DImpV2::ReadHeader()
{
	if (!m_pFile)
	{
		// throw Exception::exception("���ļ�ͷ��������(01)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}

	fseek(m_pFile, 0, SEEK_SET); // ��λ���ļ���ͷ
	V3D_Header V3DHeader;
	fread(&V3DHeader, sizeof(V3DHeader), 1, m_pFile);
	if (0 != strcmp(string(V3DHeader.corp).c_str(), SUNTOWARD_NAME))
	{
		// throw Exception::exception("�ļ���ʽ����ȷ���޷���ȡ��");
		return ERR_FILE_V3D_WRONGFORMAT;
	}
	
	m_V3DFileVersion = V3DHeader.v3dVersion;
	if (m_V3DFileVersion > CURRENT_V3DVERSION) // �ļ��汾�Ŵ��ڵ�ǰ����֧�ֵİ汾��
	{
		// throw Exception::exception("V3D�ļ���ʽ̫�£��޷���ȡ��");
		return ERR_FILE_V3D_NEWVERSION;
	}
	/*else if (m_V3DFileVersion < CURRENT_V3DVERSION)
	{
		return ERR_FILE_V3D_OLDVERSION;
	}*/

	return ERR_COMMON_OK;  // ������ȡ
	
}


DWORD CGetV3DImpV2::ReadFileTable()
{
	if (!m_pFile)
	{
		// throw Exception::exception("���ļ���������(02)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}

	fseek(m_pFile, sizeof(V3D_Header), SEEK_SET);
	if (1 != fread(&m_V3DTable, sizeof(m_V3DTable), 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL;
	}
	return ERR_COMMON_OK;
}

DWORD CGetV3DImpV2::ReadWindowProp()
{
	if (!m_pFile)
	{
		// throw Exception::exception("���ļ���������(03)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}	

	// ��ȡ��������	
	fseek(m_pFile, m_V3DTable.SceneproperyPtr, SEEK_SET);
	if (1 != fread(&m_SceneProperty, sizeof(m_SceneProperty), 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

	return ERR_COMMON_OK;
}

// ��ȡ��������
DWORD CGetV3DImpV2::ReadAllData()
{	
	
	fseek(m_pFile, m_V3DTable.PrivacyDataPtr, SEEK_SET); // �ƶ�����ʼλ��

	// ��Ȩ��Ϣ
	UINT PrivacyDataSize = m_V3DTable.cameraDataPtr - m_V3DTable.PrivacyDataPtr;
	m_pPrivacyBuf = new char[PrivacyDataSize];
	// fseek(m_pFile, m_V3DTable.PrivacyDataPtr, SEEK_SET);	
	if (1 != fread(m_pPrivacyBuf, PrivacyDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// �������
	UINT CameDataSize = m_V3DTable.lightDataPtr - m_V3DTable.cameraDataPtr;
	m_pCameraBuf = new char[CameDataSize];
	// fseek(m_pFile, m_V3DTable.cameraDataPtr, SEEK_SET);	
	if (1 != fread(m_pCameraBuf, CameDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// �ƹ�
	UINT LightDataSize = m_V3DTable.texturePtr - m_V3DTable.lightDataPtr;
	m_pLightBuf = new char[LightDataSize];
	// fseek(m_pFile, m_V3DTable.lightDataPtr, SEEK_SET);
	if (1 != fread(m_pLightBuf, LightDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ��ͼ��������
	UINT TextureDataSize = m_V3DTable.mtrlDataPtr - m_V3DTable.texturePtr;
	m_pTextureBuf = new char[TextureDataSize];
	// fseek(m_pFile, m_V3DTable.texturePtr, SEEK_SET);
	if (1 != fread(m_pTextureBuf, TextureDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ��������
	UINT MtrDataSize = m_V3DTable.entityDataPtr - m_V3DTable.mtrlDataPtr;	
	m_pMaterailBuf = new char[MtrDataSize];
	// fseek(m_pFile, m_V3DTable.mtrlDataPtr, SEEK_SET);
	if (1 != fread(m_pMaterailBuf, MtrDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// entity
	UINT EntityDataSize = m_V3DTable.meshDataPtr - m_V3DTable.entityDataPtr;
	m_pEntityBuf = new char[EntityDataSize];
	// fseek(m_pFile, m_V3DTable.meshDataPtr, SEEK_SET);
	if (1 != fread(m_pEntityBuf, EntityDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// mesh
 	UINT MeshDataSize = m_V3DTable.TextDataPtr - m_V3DTable.meshDataPtr;
 	m_pMeshBuf = new char[MeshDataSize];
 	// fseek(m_pFile, m_V3DTable.meshDataPtr, SEEK_SET);
 	if (1 != fread(m_pMeshBuf, MeshDataSize, 1, m_pFile))
 	{
 		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
 	}

	// ����
	UINT TextDataSize = m_V3DTable.OverlayPtr - m_V3DTable.TextDataPtr;
	m_pTextBuf = new char[TextDataSize];
	// fseek(m_pFile, m_V3DTable.TextDataPtr, SEEK_SET);
	if (1 != fread(m_pTextBuf, TextDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// overlay
	UINT OverlayDataSize = m_V3DTable.NavigatotPtr - m_V3DTable.OverlayPtr;
	m_pOverlayBuf = new char[OverlayDataSize];
	// fseek(m_pFile, m_V3DTable.OverlayPtr, SEEK_SET);
	if (1 != fread(m_pOverlayBuf, OverlayDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����ͼ
	UINT NaviSize = m_V3DTable.ButtonPtr - m_V3DTable.NavigatotPtr;
	m_pNaviBuf = new char[NaviSize];
	int ReadSize = fread(m_pNaviBuf, NaviSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ��ť
	UINT ButtonSize = m_V3DTable.BackdropPtr - m_V3DTable.ButtonPtr;
	m_pButtonBuf = new char[ButtonSize];
	ReadSize = fread(m_pButtonBuf, ButtonSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����ͼ
	UINT BackdropSize = m_V3DTable.AudioPtr - m_V3DTable.BackdropPtr;
	m_pBackdropBuf = new char[BackdropSize];
	ReadSize = fread(m_pBackdropBuf, BackdropSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����
	UINT AudioSize = m_V3DTable.FlashPtr - m_V3DTable.AudioPtr;
	m_pAudioBuf = new char[AudioSize];
	ReadSize = fread(m_pAudioBuf, AudioSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// flash
	UINT FlashDataSize = m_V3DTable.AnimationPtr - m_V3DTable.FlashPtr;
	m_pFlashBuf = new char[FlashDataSize];
	ReadSize = fread(m_pFlashBuf, FlashDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����
	UINT AnimationDataSize = m_V3DTable.ControlPtr - m_V3DTable.AnimationPtr;
	m_pAnimationBuf = new char[AnimationDataSize];
	ReadSize = fread(m_pAnimationBuf, AnimationDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ������Ϣ
	UINT ControlDataSize = m_V3DTable.sceneGraphDataPtr - m_V3DTable.ControlPtr;
	m_pControlBuf = new char[ControlDataSize];
	fseek(m_pFile, m_V3DTable.ControlPtr, SEEK_SET);
	ReadSize = fread(m_pControlBuf, ControlDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	// ����ͼ
	fseek(m_pFile, 0, SEEK_END); // �ļ���β
	UINT FileSize = ftell(m_pFile);
	UINT DataSize = FileSize - m_V3DTable.sceneGraphDataPtr;
	m_pSceneGraphBuf = new char[DataSize];
	fseek(m_pFile, m_V3DTable.sceneGraphDataPtr, SEEK_SET);
	ReadSize = fread(m_pSceneGraphBuf, DataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	PostMessage(m_hWnd, WM_WEBMAX_PROGRESS, m_ProcessCurSize++, m_ProcessTotalSize);  // ��ʼ����

	return ERR_COMMON_OK;
}


DWORD CGetV3DImpV2::ReadTextureData()
{
	DELETE_ARRAYPTR(m_pTextureBuf);  // ɾ���ֽ���
	return ERR_COMMON_OK;
}

DWORD CGetV3DImpV2::ReadMaterialData()
{
	if (!m_pFile)
	{
		// throw Exception::exception("���ļ���������(03)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pMaterailBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	m_vMaterial.clear();

	// ��ȡ������Ϣ
	char* Offset = m_pMaterailBuf;          // �ֽ��������ݿ�ʼ��λ��
	DWORD MtrCount = 0;                     // ������Ŀ
	CPY_FROM_STREAM(MtrCount, Offset)       // ��ȡ������Ŀ
	for (DWORD MtrIdx = 0; MtrIdx < MtrCount; ++MtrIdx)
	{
		DWORD len = 0;
		CPY_FROM_STREAM(len, Offset)        // Mtrl Name length
		std::string sMtlName = "";
		if (len > 0)
		{
			char *pName = new char[len + 1];
			ZeroMemory(pName, len + 1);
			memcpy(pName, Offset, len);         // Mtrl Name
			Offset += len;
			// pMtrl->setName(std::string(pName));	
			sMtlName = std::string(pName);
			delete [] pName;
			pName = NULL;
		}
		else
		{
			// ����������Դ��ָ����Դ��, Ogre���Զ�����һ��technique��һ��pass
			// pMtrl = MaterialManager::getSingleton().create(pName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			MessageBox(NULL, TEXT("������û��Ϊ����ָ�����ƣ����ܻᵼ�����⣡"), TEXT("����"), 0);
			continue;
		}

		//����������Դ��ָ����Դ��
		CMaterial *pMtrl = m_pVGSSceneMgr->CreateMaterial(sMtlName);
		if (!pMtrl)
		{
			continue;
		}

		bool bRecShadow = false;
		float USpeed, VSpeed;
		CPY_FROM_STREAM(bRecShadow, Offset);
		CPY_FROM_STREAM(USpeed, Offset);
		CPY_FROM_STREAM(VSpeed, Offset);

		// all technology
		UINT techCount;
		CPY_FROM_STREAM(techCount, Offset);
		for (UINT iTech = 0; iTech < techCount; ++iTech)
		{
			UINT nameLen;
			CPY_FROM_STREAM(nameLen, Offset);
			char *pName = new char[nameLen];
			CPY_PTR_FROM_STREAM(pName, nameLen, Offset);
			delete [] pName;
			pName = 0;

			// all pass
			UINT passCount = 0;
			CPY_FROM_STREAM(passCount, Offset);
			for (UINT iPass = 0; iPass < passCount; ++iPass)
			{
				UINT nameLen;
				CPY_FROM_STREAM(nameLen, Offset);
				Offset += nameLen;  // ����name

				// ���ʵĻ�������
				V3D_PassDetail PassDetail;
				CPY_FROM_STREAM(PassDetail, Offset)

				// ע�⣺�ɰ汾V3D�б������ɫ��rgba��ʽ
				pMtrl->m_bUseDiffuse     = PassDetail.bUseDiffuse;
				if (pMtrl->m_bUseDiffuse)
				{
					pMtrl->d3dmtrl.Diffuse = _Get_D3DCOLORVALUE_RGBA(PassDetail.Dif_RGBA); // D3DCOLOR_RGBA(PassDetail.basedetail.Dif_RGBA.r, PassDetail.basedetail.Dif_RGBA.g, PassDetail.basedetail.Dif_RGBA.b, PassDetail.basedetail.Dif_RGBA.a);
				}
				else
				{
					D3DCOLORVALUE color;
					color.r = color.g = color.b = 1.0f;
					color.a = (PassDetail.Dif_RGBA & 0x000000ff) / 255.0f; // ��ʾ��rgba��ʽ
					pMtrl->d3dmtrl.Diffuse = color;
				}
				pMtrl->d3dmtrl.Ambient = _Get_D3DCOLORVALUE_RGBA(PassDetail.Amb_RGBA); // D3DCOLOR_RGBA(PassDetail.basedetail.Amb_RGBA.r, PassDetail.basedetail.Amb_RGBA.g, PassDetail.basedetail.Amb_RGBA.b, PassDetail.basedetail.Amb_RGBA.a);		

				pMtrl->specular = _Get_D3DCOLORVALUE_RGBA(PassDetail.Spe_RGBA); // D3DCOLOR_RGBA(PassDetail.basedetail.Spe_RGBA.r, PassDetail.basedetail.Spe_RGBA.g, PassDetail.basedetail.Spe_RGBA.b, PassDetail.basedetail.Spe_RGBA.a);
				pMtrl->specularLevel = PassDetail.Spe_Level;
				float factor = pMtrl->specularLevel / 100.0f;
				pMtrl->d3dmtrl.Specular = _Multiply_D3DCOLORVALUE(pMtrl->specular, factor);
				pMtrl->d3dmtrl.Power    = PassDetail.shininess;
				
				pMtrl->selfIllumination = (UINT)PassDetail.selfIllumination;
				factor = pMtrl->selfIllumination / 100.0f;
				pMtrl->d3dmtrl.Emissive = _Multiply_D3DCOLORVALUE(pMtrl->d3dmtrl.Diffuse, factor);
				
				if (PassDetail.RejFunc == 1/*CMPF_ALWAYS_PASS*/)
				{
					pMtrl->bAlphaTest = false;
				}
				else
				{
					pMtrl->bAlphaTest = true;
				}
				pMtrl->AlpahaTestValue = PassDetail.AlphaValue;

				if (PassDetail.CullMode == 1 /*CULL_NONE*/)
				{
					pMtrl->twoSide = true;
				}
				else 
				{
					pMtrl->twoSide = false;
				}

				pMtrl->fillMode = (_D3DFILLMODE)PassDetail.PolygonMode;
				pMtrl->setOpacity((float)(pMtrl->d3dmtrl.Diffuse.a * 100/*/ 2.55f*/ + 0.5));  // ��[0, 255]�����[0, 100]

				// ���ʵ���ͼ��
				UINT texLayerCount;
				CPY_FROM_STREAM(texLayerCount, Offset)
				for (UINT iLayer = 0; iLayer < texLayerCount; ++iLayer)
				{
					CTextureLayer *pLayer = new CTextureLayer;
					UINT NameLen;
					CPY_FROM_STREAM(NameLen, Offset)
					char *pTmp = new char[NameLen + 1];
					ZeroMemory(pTmp, NameLen + 1);
					memcpy(pTmp, Offset, NameLen);
					pLayer->m_sName = std::string(pTmp);
					delete [] pTmp;
					Offset += NameLen;

					UINT uvIndex;
					BYTE mipMap, minFilter, magFilter, mipFilter;
					FLOAT USpeed, VSpeed;
					int texType;
					CPY_FROM_STREAM(uvIndex, Offset)
					CPY_FROM_STREAM(minFilter, Offset)
					CPY_FROM_STREAM(magFilter, Offset)
					CPY_FROM_STREAM(mipFilter, Offset)
					CPY_FROM_STREAM(mipMap, Offset)
					CPY_FROM_STREAM(USpeed, Offset)
					CPY_FROM_STREAM(VSpeed, Offset)
					CPY_FROM_STREAM(texType, Offset)
					pLayer->UVIndex = uvIndex;
					pLayer->USpeed = USpeed;
					pLayer->VSpeed = VSpeed;
					pLayer->type = (VGS_TextureType)texType;  // ������

					int dynamicDuration;
					char reflectType;
					VGS_BumpType bumpType;
					VGS_BumpwaterTextureParam  bumpWaterParam;
					VGS_BumpEnvTextureParam    bumpEnvParam;
					VGS_BumpNormalTextureParam bumpNormalParam;
					VGS_RTF_PARAM              reflectionParam;  // ʵʱ����
					VGS_RTF_PARAM              refrationParam;   // ʵʱ����
					if (pLayer->type == TLT_DYNAMIC)
					{						
						CPY_FROM_STREAM(dynamicDuration, Offset);
					}
					else if (pLayer->type == TLT_REFLECT)
					{						
						CPY_FROM_STREAM(reflectType, Offset);
					}
					else if (pLayer->type == TLT_BUMPMAP)
					{						
						CPY_FROM_STREAM(bumpType, Offset);
						if (bumpType == VBT_BUMPWATER)
						{
							CPY_FROM_STREAM(bumpWaterParam.XScale, Offset);
							CPY_FROM_STREAM(bumpWaterParam.YScale, Offset);
							CPY_FROM_STREAM(bumpWaterParam.Radius, Offset);
							CPY_FROM_STREAM(bumpWaterParam.Speed, Offset);
							CPY_FROM_STREAM(bumpWaterParam.CenterX, Offset);
							CPY_FROM_STREAM(bumpWaterParam.CenterY, Offset);
						}
						else if (bumpType == VBT_BUMPENVIRONMENT)
						{
							CPY_FROM_STREAM(bumpEnvParam.noise1, Offset);
							CPY_FROM_STREAM(bumpEnvParam.noise2, Offset);
						}
						else if (bumpType == VBT_BUMPNORMARLMAP)
						{
							int lightIndex;
							CPY_FROM_STREAM(lightIndex, Offset);
							if (lightIndex >= 0 && lightIndex < m_vpLight.size())
							{
								bumpNormalParam.LightHandle = (DWORD)m_vpLight[lightIndex];
							}
							else
							{
								bumpNormalParam.LightHandle = 0;
							}
						}
					}
					else if (pLayer->type == TLT_RTFMAP) // ʵʱ����
					{
						ZeroMemory(&reflectionParam, sizeof(reflectionParam));
						int  modelIndex;
						UINT refCount;
						CPY_FROM_STREAM(reflectionParam.width, Offset);
						CPY_FROM_STREAM(reflectionParam.height, Offset);
						CPY_FROM_STREAM(modelIndex, Offset);
						if (modelIndex >= 0 && modelIndex <= m_vModel.size())
						{
							reflectionParam.refectionModel = (DWORD)m_vModel[modelIndex];
						}
						CPY_FROM_STREAM(refCount, Offset);
						for (UINT icount = 0; icount < refCount; ++icount)
						{
							CPY_FROM_STREAM(modelIndex, Offset);
							if (modelIndex >= 0 && modelIndex <= m_vModel.size())
							{
								reflectionParam.vrefectedModels.push_back((DWORD)m_vModel[modelIndex]);
							}
						}
					}
					else if (pLayer->type == TLT_RTTMAP) // ʵʱ����
					{
						ZeroMemory(&refrationParam, sizeof(refrationParam));
						int  modelIndex;
						UINT refCount;
						CPY_FROM_STREAM(refrationParam.width, Offset);
						CPY_FROM_STREAM(refrationParam.height, Offset);
						CPY_FROM_STREAM(modelIndex, Offset);
						if (modelIndex >= 0 && modelIndex <= m_vModel.size())
						{
							refrationParam.refectionModel = (DWORD)m_vModel[modelIndex];
						}
						CPY_FROM_STREAM(refCount, Offset);
						for (UINT icount = 0; icount < refCount; ++icount)
						{
							CPY_FROM_STREAM(modelIndex, Offset);
							if (modelIndex >= 0 && modelIndex <= m_vModel.size())
							{
								refrationParam.vrefectedModels.push_back((DWORD)m_vModel[modelIndex]);
							}
						}
					}

					// д����ɫ�Ļ�ɫģʽ
					VGS_BlenderInfo colorBlendInfo;
					CPY_FROM_STREAM(colorBlendInfo, Offset);

					// д��alpahͨ���Ļ�ɫģʽ
					VGS_BlenderInfo alphaBlendInfo;
					CPY_FROM_STREAM(alphaBlendInfo, Offset);

					// ֡��Ŀ
					unsigned short iFrameCount;
					CPY_FROM_STREAM(iFrameCount, Offset);

					// ��ȡÿһ��ͼƬ������
					vector <std::string> vImage;
					for (UINT iFrame = 0; iFrame < iFrameCount; ++iFrame)
					{
						UINT len;
						CPY_FROM_STREAM(len, Offset);
						char *pName = new char[len + 1];
						ZeroMemory(pName, len + 1);
						memcpy(pName, Offset, len);
						std::string strFileName(pName);
						strFileName = AddPathtoTextureFile(strFileName);
						Offset += len;
						vImage.push_back(strFileName);
						SAFE_DELETE_ARRAY(pName);
					}

					// ������ͼ��					
					pLayer->pTexture = 0;
					switch (pLayer->type)
					{
					case TLT_OPACITY:
						if (vImage.size() > 0)
						{
							pLayer->pTexture = m_pRoot->GetTextureManager().createTexture(vImage[0], TRUE, mipMap);
						}
						break;
					case TLT_BUMPMAP:
						{
							std::string strName = pLayer->m_sName;
							if (vImage.size() > 0)
							{
								strName = vImage[0];
							}
							pLayer->pTexture = m_pRoot->GetTextureManager().CreateBumpTexture(strName);
							CBumpMap *pBump = ((CBumpMap*)pLayer->pTexture);
							if (pBump && bumpType == VBT_BUMPWATER)
							{
								pBump->SetBumpAsWavemap(bumpWaterParam.width, bumpWaterParam.height, bumpWaterParam.XScale, bumpWaterParam.YScale,
									                    bumpWaterParam.Radius, bumpWaterParam.Speed, bumpWaterParam.CenterX, bumpWaterParam.CenterY);
							}
							else if (pBump && bumpType == VBT_BUMPENVIRONMENT)
							{
								pBump->SetBumpAsEnviromentmap(strName, bumpEnvParam.noise1, bumpEnvParam.noise2);
							}
							else if (pBump && bumpType == VBT_BUMPNORMARLMAP)
							{
								pBump->SetBumpAsNormalmap(strName, (CLight*)bumpNormalParam.LightHandle);
							}
							break;
						}
					case TLT_RTFMAP: // ʵʱ����
						{
							std::string strName = pLayer->m_sName;
							if (vImage.size() > 0)
							{
								strName = vImage[0];
							}
							pLayer->pTexture = m_pRoot->GetTextureManager().CreateRTreflectTexture(strName, refrationParam.width, reflectionParam.height);
							CRTFMap* pRTFMap = (CRTFMap*)pLayer->pTexture;
							if (pRTFMap)
							{
								pRTFMap->SetRefPlaneByModel(reflectionParam.refectionModel);
								for (UINT iModel = 0; iModel < reflectionParam.vrefectedModels.size(); ++iModel)
								{
									pRTFMap->AddRefModel(reflectionParam.vrefectedModels[iModel]);
								}
							}
						}
						break;
					case TLT_RTTMAP:  // ʵʱ����
						{
							/*std::string strName = pLayer->m_sName;
							if (vImage.size() > 0)
							{
								strName = vImage[0];
							}
							pLayer->pTexture = CTextureManager::getSingleton().CreateRTreflectTexture(strName, refrationParam.width, reflectionParam.height);
							CRTFMap* pRTFMap = (CRTFMap*)pLayer->pTexture;
							if (pRTFMap)
							{
								pRTFMap->SetRefPlaneByModel(reflectionParam.refectionModel);
								for (UINT iModel = 0; iModel < reflectionParam.vrefectedModels.size(); ++iModel)
								{
									pRTFMap->AddRefModel(reflectionParam.vrefectedModels[iModel]);
								}
							}*/
						}
						break;
					case TLT_BASAL:
					case TLT_REFLECT:
					default:
						if (vImage.size() > 0)
						{
							pLayer->pTexture = m_pRoot->GetTextureManager().createTexture(vImage[0], FALSE, mipMap);
						}
						break;
					}
					

					// д����ɫ�Ļ�ɫģʽ
					pLayer->Blenderinfo.ColorBlendOption = _getD3DBlendOperation(colorBlendInfo.option);
					pLayer->Blenderinfo.ColorBlendParam  = D3DCOLOR_COLORVALUE(0, 0, 0, colorBlendInfo.param.BlenderOP_alpha);
					
					// д��alpahͨ���Ļ�ɫģʽ
					pLayer->Blenderinfo.AlphaBlendOption = _getD3DBlendOperation(alphaBlendInfo.option);
					pLayer->Blenderinfo.AlphaBlendParam  = D3DCOLOR_COLORVALUE(0, 0, 0, alphaBlendInfo.param.BlenderOP_alpha);


					// ���浽���ʵĲ��б�
					pMtrl->m_vpTexLayers.push_back(pLayer);

				} // for iLayer
			} // for ipass			
		} // for itech
		
		// m_pVGSSceneMgr->m_vShader.push_back(pMtrl);
		m_vMaterial.push_back(pMtrl);  //  save to temp list

	} // Material 

	DELETE_ARRAYPTR(m_pMaterailBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}	

DWORD CGetV3DImpV2::ReadMeshData()
{
	m_vMesh.clear();

	if (!m_pFile)
	{
		// throw Exception::exception("���ļ���������(03)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pMeshBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// ��ȡentity��Ϣ
	char* Offset = m_pMeshBuf;              // �ֽ��������ݿ�ʼ��λ��
	DWORD meshCount;                        // entity��Ŀ
	CPY_FROM_STREAM(meshCount, Offset)      // ��ȡentity��Ŀ

	m_vMesh.resize(meshCount);

	// ��ȡÿһ��mesh
	for (DWORD iMesh = 0; iMesh < meshCount; ++iMesh)
	{
		CVGSMesh &mesh = m_vMesh[iMesh];
		mesh.faceCnt = 0;

		UINT len;
		CPY_FROM_STREAM(len, Offset);
		char *pName = new char[len + 1];
		ZeroMemory(pName, len + 1);
		CPY_PTR_FROM_STREAM(pName, len, Offset);
		mesh.sName = std::string(pName);
		SAFE_DELETE_ARRAY(pName);

		UINT vertexElementCount;
		CPY_FROM_STREAM(vertexElementCount, Offset);  // ���������ĸ���
		UINT verBufSize = sizeof(unsigned short) + sizeof(UINT) + 3 * sizeof(unsigned short);
		Offset += verBufSize * vertexElementCount;  // �����������

		UINT startIndex;
		CPY_FROM_STREAM(mesh.vertexCnt, Offset);
		CPY_FROM_STREAM(startIndex, Offset); // ʼ����0

		mesh.fvf = FVF_POSNORMALCOLORCOORD4;  // �̶���ʽ
		mesh.pVertex = (void*)(new CVertex_POSNORMALCOLORCOORD4[mesh.vertexCnt]);
		memcpy(mesh.pVertex, Offset, mesh.vertexCnt * sizeof(CVertex_POSNORMALCOLORCOORD4));
		Offset += mesh.vertexCnt * sizeof(CVertex_POSNORMALCOLORCOORD4);

		//  modify vertex position and normal, because of the coord (right or left hand)
		for (DWORD iVer = 0; iVer < mesh.vertexCnt; ++iVer)
		{
			((CVertex_POSNORMALCOLORCOORD4*)mesh.pVertex)[iVer].position.z *= -1;
			((CVertex_POSNORMALCOLORCOORD4*)mesh.pVertex)[iVer].normal.z *= -1;
		}

		// ��ÿһ��submesh
		unsigned short subMeshCount;
		CPY_FROM_STREAM(subMeshCount, Offset);
		mesh.subMeshCount = subMeshCount;
		mesh.pSubMesh = (void*)new CVGSSubMesh[mesh.subMeshCount];
		for (UINT iSubMesh = 0; iSubMesh < mesh.subMeshCount; ++iSubMesh)
		{
			CVGSSubMesh &subMesh = ((CVGSSubMesh*)mesh.pSubMesh)[iSubMesh];
			UINT len;
			CPY_FROM_STREAM(len, Offset);
			char *pMtrName = new char[len + 1];
			ZeroMemory(pMtrName, len + 1);
			CPY_PTR_FROM_STREAM(pMtrName, len, Offset);
			std::string sMtrName = std::string(pMtrName);  // submeshʹ�õĲ�������
			//CMaterial* pMaterial = m_pVGSSceneMgr->GetMaterialByName(sMtrName);
			//subMesh.mtrIndex = m_pVGSSceneMgr->getMaterialIndex(pMaterial);
			subMesh.mtrIndex = _GetIndexByName(m_vMaterial, sMtrName);
			SAFE_DELETE_ARRAY(pName);

			// save material to scene list
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_3D, _GetHandleByName(m_vMaterial, sMtrName));

			UINT indexCount, startIndex;
			CPY_FROM_STREAM(indexCount, Offset);
			CPY_FROM_STREAM(startIndex, Offset); // ʼ����0
			subMesh.faceCount = indexCount / 3;
			mesh.faceCnt += subMesh.faceCount; // ����mesh��������
			subMesh.pFace = new CVGSFace[subMesh.faceCount];

			int indexsize = 0;
			if (mesh.vertexCnt > 65535)
			{
				indexsize = sizeof(DWORD);
				UINT iVertexIndex = 0;
				//memcpy((void*)subMesh.pFace, Offset, indexCount * indexsize);
				WORD *pIndexData = (WORD*)Offset;
				for (UINT nFace = 0; nFace < subMesh.faceCount; nFace++)
				{
					subMesh.pFace[nFace].a = pIndexData[iVertexIndex++];
					subMesh.pFace[nFace].b = pIndexData[iVertexIndex++];
					subMesh.pFace[nFace].c = pIndexData[iVertexIndex++];
				}
			}
			else
			{
				indexsize = sizeof(WORD);
				UINT iVertexIndex = 0;
				WORD *pIndexData = (WORD*)Offset;
				for (UINT nFace = 0; nFace < subMesh.faceCount; nFace++)
				{
					subMesh.pFace[nFace].a = pIndexData[iVertexIndex++];
					subMesh.pFace[nFace].b = pIndexData[iVertexIndex++];
					subMesh.pFace[nFace].c = pIndexData[iVertexIndex++];
				}
			}
			Offset += indexCount * indexsize;
		}

		// ��Χ�кͰ�Χ��
		vector3 boxNearConner, boxFarConner;
		float sphereRadius;
		CPY_FROM_STREAM(boxNearConner, Offset);
		CPY_FROM_STREAM(boxFarConner, Offset);
		CPY_FROM_STREAM(sphereRadius, Offset);
	}

	return ERR_COMMON_OK;
}

// ��ȡģ������
DWORD CGetV3DImpV2::ReadModelData()
{
	if (!m_pFile)
	{
		// throw Exception::exception("���ļ���������(03)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pEntityBuf)
	{
		return ERR_FILE_V3D_NODATA; 
	}

	// ��ȡentity��Ϣ
	char* Offset = m_pEntityBuf;              // �ֽ��������ݿ�ʼ��λ��
	DWORD EntityCount;                        // entity��Ŀ
	CPY_FROM_STREAM(EntityCount, Offset)      // ��ȡentity��Ŀ
	
	// ��ȡÿ��ģ��
	for (UINT idx = 0; idx < EntityCount; ++idx)
	{
		int namelen = 0;
		char sName[MAX_FILENAME_LENGTH];
		ZeroMemory(sName, MAX_FILENAME_LENGTH);
		CPY_FROM_STREAM(namelen, Offset)      // entity����
		
		memcpy(sName, Offset, namelen);       // ��ȡ����
		Offset += namelen;

		CModel *pModel = m_pVGSSceneMgr->CreateModel(std::string(sName));

		CVGSMesh* pMesh = 0;
		int meshIndex;
		CPY_FROM_STREAM(meshIndex, Offset);
		if (meshIndex >= 0 && meshIndex < m_vMesh.size())
		{
			pMesh = &m_vMesh[meshIndex];
		}
		
		bool bAutoNormal, bBillborad, bClickable, bCollision, bIsGround, bReflection, bVisible;
		VGS_AXIS_TYPE billboardUp, billboardDir;
		CPY_FROM_STREAM(bAutoNormal, Offset);
		CPY_FROM_STREAM(bBillborad, Offset);
		CPY_FROM_STREAM(bClickable, Offset);
		CPY_FROM_STREAM(bCollision, Offset);
		CPY_FROM_STREAM(bIsGround, Offset);
		CPY_FROM_STREAM(billboardUp, Offset);
		CPY_FROM_STREAM(billboardDir, Offset);
		CPY_FROM_STREAM(bVisible, Offset);
		CPY_FROM_STREAM(bReflection, Offset)      // �Ƿ�������ģ��
			
		pModel->isAutoNormalise = bAutoNormal;
		pModel->isBillboard   = bBillborad;
		pModel->m_isClickable = bClickable;
		pModel->isCollision = bCollision;
		pModel->isGround = bIsGround;
		pModel->visible = bVisible;

		pModel->bReflect = bReflection;
		if (pModel->bReflect)
		{
			UINT reflectionCount = 0;
			CPY_FROM_STREAM(reflectionCount, Offset)      // �������ģ����Ŀ
			for (UINT iModel = 0; iModel < reflectionCount; ++iModel)
			{
				int ModelIndex;
				CPY_FROM_STREAM(ModelIndex, Offset)      // �������ģ����Ŀ
				pModel->refedModelIdxList.push_back(ModelIndex);
			}
		} // if isReflection

		bool bIsRefration;
		CPY_FROM_STREAM(bIsRefration, Offset);      // �Ƿ���������ģ��
		//pModel->bReflect
		//if (pModel->bReflect)
		//{
		//	UINT reflectionCount = 0;
		//	CPY_FROM_STREAM(reflectionCount, Offset)      // �������ģ����Ŀ
		//		for (UINT iModel = 0; iModel < reflectionCount; ++iModel)
		//		{
		//			int ModelIndex;
		//			CPY_FROM_STREAM(ModelIndex, Offset)      // �������ģ����Ŀ
		//				pModel->refedModelIdxList.push_back(ModelIndex);
		//		}
		//} // if isReflection

		DWORD dwMajorPriority = 0;
		DWORD dwMinorPriority = 0;
		if (CURRENT_V3DVERSION >= 50009)
		{
			CPY_FROM_STREAM(dwMajorPriority, Offset);
			CPY_FROM_STREAM(dwMinorPriority, Offset);
		}

		pModel->index = m_pVGSSceneMgr->GetModelCount();

		//vector<CMaterial*> vMaterial;
		//m_pVGSSceneMgr->getMaterialList(vMaterial);
		pModel->Create(pMesh, m_vMaterial);
		SAFE_RELEASE(pMesh);

		m_vModel.push_back(pModel);
	}

	m_vMesh.clear();
	DELETE_ARRAYPTR(m_pEntityBuf);  // ɾ���ֽ���
	DELETE_ARRAYPTR(m_pMeshBuf);    // ɾ���ֽ���

	return ERR_COMMON_OK;
}



//��������Ĳ�������
void CGetV3DImpV2::CreateCameraParamAnimation(CCamera* pCamera, VGS_V3D_Camera* pVGSCamera, int frameCnt)
{
	//�ؼ�֡����
// 	CameraParamAnimation* pCameraParamAni = new CameraParamAnimation(pCamera);
// 
// 	pCameraParamAni->mCamera = pCamera; // fuxb 2008-4-18
// 	pCameraParamAni->frameCnt = frameCnt;
// 	pCameraParamAni->cameraInfoList = new VGS_V3D_CAMINFO[frameCnt];
// 	for (INT frameIndex = 0; frameIndex < frameCnt; frameIndex++)
// 	{
// 		pCameraParamAni->cameraInfoList[frameIndex] = pVGSCamera->v3dAnimationInfo[frameIndex];
// 	}
// 
// 	pCameraParamAni->setLength(Real(frameCnt)/gkfrPlaySpeed);
// 	
// 	//�������������������
// 	// cameraParamAnimationList.push_back(pCameraParamAni);
// 	m_pVGSSceneMgr->GetAnimationManager()->AddCameraAnimation(pCameraParamAni);
}
BOOL CGetV3DImpV2::CreateCameraNodeAnimation(CSceneNode* pSceneNode, CCamera* pCamera, const std::vector<CNodeTransform> &vTransform)
{
	if (NULL == pSceneNode || NULL == pCamera)
		return FALSE;

	//������̹ؼ�֡��Ϣ
	std::vector<CVGS_CAMERA_ANIMINFO> vCamAnimInfo;
	std::string strCameraName = pCamera->getName();
	for (std::vector<VGS_V3D_Camera>::iterator it = m_arCameraFrameInfo.begin();
		 it != m_arCameraFrameInfo.end();
		 it++)
	{
		std::string strCurName = (*it).name;
		if (stricmp(strCurName.c_str(), strCameraName.c_str()) == 0)
		{
			for (std::vector<VGS_V3D_CAMINFO>::iterator it2 = (*it).v3dAnimationInfo.begin();
				 it2 != (*it).v3dAnimationInfo.end();
				 it2++)
			{
				CVGS_CAMERA_ANIMINFO camAnimInfo;
				VGS_V3D_CAMINFO camFrameInfo = (*it2);
				camAnimInfo.farClip = camFrameInfo.farClipDis;
				camAnimInfo.nearClip = camFrameInfo.nearClipDis;
				camAnimInfo.fov = camFrameInfo.fov;
				vCamAnimInfo.push_back(camAnimInfo);
			}
		}
	}
	
	CNodeAnimation* pNodeAnimation = m_pRoot->GetAnimManager().CreateCameraAnimation(pSceneNode, vTransform, vCamAnimInfo);

	return pNodeAnimation ? TRUE : FALSE;
}


//��ȡ������ݲ��������������
DWORD CGetV3DImpV2::ReadCameraData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}
	if (!m_pCameraBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// �ֽ��������ݿ�ʼ��λ��
	char* Offset = m_pCameraBuf;   
	
	m_vpCamera.clear();  // �������б�	
	m_arCameraFrameInfo.clear();

	//�����������
	UINT cameraCnt = 0;                         
	CPY_FROM_STREAM(cameraCnt, Offset)       

	//��������������
	for (UINT i = 0; i < cameraCnt; i++)
	{
		//�������
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset); 
		char *pName = new char[len + 1];
		ZeroMemory(pName, len + 1);
		//strncpy_s(pName, sizeof(len + 1), Offset, len); // '\0' excluded
		//Offset += len;
		CPY_PTR_FROM_STREAM(pName, len, Offset);
		std::string sName = std::string(pName);
		delete pName;
		pName = NULL;

		// �������
		CCamera *pCamera = m_pVGSSceneMgr->CreateCamera(sName);
		if (!pCamera)
		{
			return ERR_SCENE_CREATE;
		}

		// ����Ļ�������
		VGS_V3D_CAMINFO camInfo;
		// CPY_FROM_STREAM(camInfo, Offset)
		CPY_FROM_STREAM(camInfo.isOrtho, Offset);
		CPY_FROM_STREAM(camInfo.fov, Offset);
		CPY_FROM_STREAM(camInfo.nearClipDis, Offset);
		CPY_FROM_STREAM(camInfo.farClipDis, Offset);
		CPY_FROM_STREAM(camInfo.position, Offset);
		CPY_FROM_STREAM(camInfo.quat, Offset);

		// ͶӰ����
		if (camInfo.isOrtho)
		{
			pCamera->m_camType = VCT_ORTHO;
		}
		else
		{
			pCamera->m_camType = VCT_PROJECT;
		}
		
		// Fov
		pCamera->SetFov(camInfo.fov);

		//near far clip
		pCamera->setNearClip(camInfo.nearClipDis);
		pCamera->setFarClip(camInfo.farClipDis);

		pCamera->SetPosition(&D3DXVECTOR3(camInfo.position.x, camInfo.position.y, -camInfo.position.z), VTS_PARENT);
		pCamera->setRotation(D3DXQUATERNION(camInfo.quat.x, camInfo.quat.y, camInfo.quat.z, camInfo.quat.w)/*camInfo.quat*/, VTS_PARENT);
		// pCamera->Lookat(&D3DXVECTOR3(camInfo.at.x, camInfo.at.y, camInfo.at.z), VTS_PARENT);
		// pCamera->SetRoll(camInfo.roll);

		m_vpCamera.push_back(pCamera);   // ����������������ܻ�ʹ��
		
		VGS_V3D_Camera cameraFrameInfo;
		ZeroMemory(&cameraFrameInfo, sizeof(cameraFrameInfo));
		cameraFrameInfo.name = pCamera->getName();
		cameraFrameInfo.nameLen = cameraFrameInfo.name.size();

		//�����������֡��
		int frameCnt;
		CPY_FROM_STREAM(frameCnt, Offset);
		for (int iFrame = 0; iFrame < frameCnt; iFrame++)
		{
			VGS_V3D_CAMINFO camFrameInfo;
			CPY_FROM_STREAM(camFrameInfo, Offset);
			camFrameInfo.position.z *= -1;
			camFrameInfo.quat.x *= -1;
			camFrameInfo.quat.y *= -1;
			camFrameInfo.at.z *= -1;
			camFrameInfo.farClipDis = 100000;
			camFrameInfo.nearClipDis = 1;
			camFrameInfo.fov = 0.6;
			cameraFrameInfo.v3dAnimationInfo.push_back(camFrameInfo);
		}
		m_arCameraFrameInfo.push_back(cameraFrameInfo);
	}

	DELETE_ARRAYPTR(m_pCameraBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}


DWORD CGetV3DImpV2::ReadLightData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pLightBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}
	if (!m_pVGSSceneMgr)
	{
		return ERR_SCENE_NOSCENE;
	}

	//��ȡ�ƹ������
	char* Offset = m_pLightBuf; 
	int LightCount = 0; 
	CPY_FROM_STREAM(LightCount, Offset);

	m_vpLight.clear();	
	m_arLightFrameInfo.clear();

	//����ƹ��ȡ����
	for (int i = 0; i < LightCount; i++)
	{
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset);
		char pName[256];
		ZeroMemory(pName, 256);
		memcpy(pName, Offset, len);
		Offset += len;		
		
		CLight *pLight = m_pVGSSceneMgr->CreateLight(std::string(pName));
		if (!pLight)
		{
			return ERR_SCENE_CREATE;
		}

		//�ƹ�����
		VGS_LIGHT_TYPE type;
		CPY_FROM_STREAM(type, Offset)

		//�ƹ�ؼ�֡������Ϣ
		VGS_V3D_Light lightFrameInfo;
		ZeroMemory(&lightFrameInfo, sizeof(lightFrameInfo));
		lightFrameInfo.name = pLight->getName();
		lightFrameInfo.strLen = lightFrameInfo.name.size();
		lightFrameInfo.type = type;

		switch (type)
		{
		case VLT_OMNI:
			{
				//������Դ��Ϣ
				VGS_V3D_OMNI omni;
				CPY_FROM_STREAM(omni, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((omni.color>>16) & 0xff)/255.0f;
				diffuse.g = ((omni.color>>8) & 0xff)/255.0f;
				diffuse.b = ((omni.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = omni.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * omni.multiply;
				pLight->param.Diffuse.g = diffuse.g * omni.multiply;
				pLight->param.Diffuse.b = diffuse.b * omni.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//�����⣬����������ģ��ȫ�ֹ�
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//�߹�
				pLight->param.Specular   = omni.multiply * pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
				pLight->param.Specular.a = 1.0f;
				
				//���ߵ������Զ����
				pLight->param.Range        = omni.attenuationRange;
				pLight->param.Attenuation0 = omni.attenuation0;
				pLight->param.Attenuation1 = omni.attenuation1;
				pLight->param.Attenuation2 = omni.attenuation2;
				
				pLight->param.Type = D3DLIGHT_POINT;
				D3DXVECTOR3 vPos(omni.position.x, omni.position.y, -omni.position.z);  
				pLight->setPosition(&vPos);	

				//���붯��֡��
				int frameCnt = 0;
				CPY_FROM_STREAM(frameCnt, Offset);
				for (int iFrame = 0; iFrame < frameCnt; iFrame++)
				{
					VGS_V3D_OMNI omniLight;
					CPY_FROM_STREAM(omniLight, Offset);
					omniLight.position.z *= -1;
					lightFrameInfo.omniInfo.push_back(omniLight);
				}				
			}
			break;

		case VLT_DIR:
			{				
				//������Դ��Ϣ
				VGS_V3D_DIRECTIONAL directionLight;
				CPY_FROM_STREAM(directionLight, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((directionLight.color>>16) & 0xff)/255.0f;
				diffuse.g = ((directionLight.color>>8) & 0xff)/255.0f;
				diffuse.b = ((directionLight.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = directionLight.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * directionLight.multiply;
				pLight->param.Diffuse.g = diffuse.g * directionLight.multiply;
				pLight->param.Diffuse.b = diffuse.b * directionLight.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//�����⣬����������ģ��ȫ�ֹ�
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//�߹�
				pLight->param.Specular   = directionLight.multiply * pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
				pLight->param.Specular.a = 1.0f;
				
				//���ߵ������Զ����
				pLight->param.Range        = directionLight.attenuationRange;
				pLight->param.Attenuation0 = directionLight.attenuation0;
				pLight->param.Attenuation1 = directionLight.attenuation1;
				pLight->param.Attenuation2 = directionLight.attenuation2;
				
				pLight->param.Type = D3DLIGHT_DIRECTIONAL;

				//����
				D3DXVECTOR3 dir(directionLight.direction.x, directionLight.direction.y, -directionLight.direction.z);
				pLight->setDirection(&dir);

				//���붯��֡��
				int frameCnt = 0;
				CPY_FROM_STREAM(frameCnt, Offset);
				for (int iFrame = 0; iFrame < frameCnt; iFrame++)
				{
					VGS_V3D_DIRECTIONAL dirLight;
					CPY_FROM_STREAM(dirLight, Offset);
					dirLight.direction.z *= -1;
					lightFrameInfo.directionLightInfo.push_back(dirLight);
				}
			}
			break;

		case VLT_SPOT:
			{
				//������Դ��Ϣ
				VGS_V3D_SPOTLIGHT spotLight;
				CPY_FROM_STREAM(spotLight, Offset);
				
				D3DCOLORVALUE diffuse;
				diffuse.r = ((spotLight.color>>16) & 0xff)/255.0f;
				diffuse.g = ((spotLight.color>>8) & 0xff)/255.0f;
				diffuse.b = ((spotLight.color) & 0xff)/255.0f;
				diffuse.a = 1.0f;
				pLight->diffuse  = diffuse;
				pLight->multiply = spotLight.multiply;
				
				pLight->param.Diffuse.r = diffuse.r * spotLight.multiply;
				pLight->param.Diffuse.g = diffuse.g * spotLight.multiply;
				pLight->param.Diffuse.b = diffuse.b * spotLight.multiply;
				pLight->param.Diffuse.a = 1.0f;
				
				//�����⣬����������ģ��ȫ�ֹ�
				pLight->ambient   = pLight->diffuse;
				pLight->ambient.a = 1.0f;
				pLight->param.Ambient   = 0.0f * pLight->param.Diffuse; //pLight->param.Diffuse; // fuxb �Ѳ���0,05��Ϊ��0.0f
				pLight->param.Ambient.a = 1.0f;
				
				//�߹�
				pLight->param.Specular   = spotLight.multiply * pLight->diffuse;  // fuxb 2007-10-16 ��3ds max������ͬ�Ĵ���ʽ
				pLight->param.Specular.a = 1.0f;
				
				//���ߵ������Զ����
				pLight->param.Range        = spotLight.attenuationRange;
				pLight->param.Attenuation0 = spotLight.attenuation0;
				pLight->param.Attenuation1 = spotLight.attenuation1;
				pLight->param.Attenuation2 = spotLight.attenuation2;
				
				pLight->param.Type = D3DLIGHT_SPOT;
				
				//����
				D3DXVECTOR3 dir(spotLight.direction.x, spotLight.direction.y, -spotLight.direction.z);
				pLight->setDirection(&dir);

				//�Ž�
				pLight->param.Theta = spotLight.innerAngle;
				pLight->param.Phi = spotLight.outerAngle;
				pLight->param.Falloff = spotLight.fallOff;

				// λ��
				D3DXVECTOR3 vPos(spotLight.position.x, spotLight.position.y, -spotLight.position.z);  
				pLight->setPosition(&vPos);
				
				//���붯��֡��
				int frameCnt = 0;
				CPY_FROM_STREAM(frameCnt, Offset);
				for (int iFrame = 0; iFrame < frameCnt; iFrame++)
				{
					VGS_V3D_SPOTLIGHT spotLight;
					CPY_FROM_STREAM(spotLight, Offset);
					spotLight.direction.z *= -1;
					spotLight.position.z *= -1;
					lightFrameInfo.spotLightInfo.push_back(spotLight);
				}
			}
			break;
		}
		m_vpLight.push_back(pLight);  // �����Լ����صĵƹ�
		m_arLightFrameInfo.push_back(lightFrameInfo);
	}

	DELETE_ARRAYPTR(m_pLightBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}

//�����ƹ��������
void CGetV3DImpV2::CreateLightParamAnimation(CLight* pLight, VGS_Light* pVGSLight, int frameCnt, char** Offset)
{
	//����������������
// 	LightParamAnimation* pLightParamAni = new LightParamAnimation(pLight);
// 	pLightParamAni->mLight = pLight;
// 	pLightParamAni->frameCnt = frameCnt;
// 	//д��ÿ֡������
// 	switch (pVGSLight->type)
// 	{
// 	case LIGHTTYPE::OMINI:
// 		{
// 			pLightParamAni->omniInfoList = new VGS_V3D_OMNI[frameCnt];
// 			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
// 				CPY_FROM_STREAM(pLightParamAni->omniInfoList[frameIndex], *Offset);
// 		}
// 		break;
// 
// 	case LIGHTTYPE::DIRECTIONLIGHT:
// 		{	
// 			pLightParamAni->directionLightInfoList = new VGS_V3D_DIRECTIONAL[frameCnt];
// 			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
// 				CPY_FROM_STREAM(pLightParamAni->directionLightInfoList[frameIndex], *Offset);
// 		}
// 		break;
// 
// 	case LIGHTTYPE::SPOTLIGHT:
// 		{
// 			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
// 				CPY_FROM_STREAM(pLightParamAni->spotLightInfoList[frameIndex], *Offset);
// 		}
// 		break;
// 	}
// 	
// 	pLightParamAni->setLength(Real(frameCnt)/gkfrPlaySpeed);
// 	
// 	//����ƹ������������
// 	// lightParamAnimationList.push_back(pLightParamAni);
// 	m_pVGSSceneMgr->GetAnimationManager()->AddLightAnimation(pLightParamAni);
}
BOOL CGetV3DImpV2::CreateLightNodeAnimation(CSceneNode* pSceneNode, CLight* pLight, const std::vector<CNodeTransform> &vTransform)
{	
	if (NULL == pSceneNode || NULL == pLight)
		return FALSE;

	//���ҵƹ�ؼ�֡��Ϣ
	std::vector<CVGS_LIGHT_INFO> vLightAnimInfo;
	std::string strLightName = pLight->getName();
	for (std::vector<VGS_V3D_Light>::iterator it = m_arLightFrameInfo.begin();
		 it != m_arLightFrameInfo.end();
		 it++)
	{
		std::string strCurName = (*it).name;
		if (stricmp(strCurName.c_str(), strLightName.c_str()) == 0)
		{
			switch(pLight->getType())
			{
			case VLT_OMNI:
				{
					for (std::vector<VGS_V3D_OMNI>::iterator it2 = (*it).omniInfo.begin();
						 it2 != (*it).omniInfo.end();
						 it2++)
					{
						CVGS_LIGHT_INFO lightAnimInfo;
						VGS_V3D_OMNI omniLightFrameInfo = (*it2);
						lightAnimInfo.color = omniLightFrameInfo.color;
						lightAnimInfo.range2 = omniLightFrameInfo.attenuationRange;
						lightAnimInfo.multiply = omniLightFrameInfo.multiply;
						vLightAnimInfo.push_back(lightAnimInfo);
					}
				}
				break;
			case VLT_DIR:
				{
					for (std::vector<VGS_V3D_DIRECTIONAL>::iterator it2 = (*it).directionLightInfo.begin();
						 it2 != (*it).directionLightInfo.end();
						 it2++)
					{
						CVGS_LIGHT_INFO lightAnimInfo;
						VGS_V3D_DIRECTIONAL dirLightFrameInfo = (*it2);
						lightAnimInfo.color = dirLightFrameInfo.color;
						lightAnimInfo.range2 = dirLightFrameInfo.attenuationRange;
						lightAnimInfo.multiply = dirLightFrameInfo.multiply;
						lightAnimInfo.dir = dirLightFrameInfo.direction;
						vLightAnimInfo.push_back(lightAnimInfo);
					}
				}
				break;
			case VLT_SPOT:
				{
					for (std::vector<VGS_V3D_SPOTLIGHT>::iterator it2 = (*it).spotLightInfo.begin();
						 it2 != (*it).spotLightInfo.end();
						 it2++)
					{
						CVGS_LIGHT_INFO lightAnimInfo;
						VGS_V3D_SPOTLIGHT spotLightFrameInfo = (*it2);
						lightAnimInfo.color = spotLightFrameInfo.color;
						lightAnimInfo.range2 = spotLightFrameInfo.attenuationRange;
						lightAnimInfo.multiply = spotLightFrameInfo.multiply;
						lightAnimInfo.dir = spotLightFrameInfo.direction;
						lightAnimInfo.theta = spotLightFrameInfo.outerAngle;
						lightAnimInfo.phi = spotLightFrameInfo.innerAngle;
						vLightAnimInfo.push_back(lightAnimInfo);
					}
				}
				break;
			}
		}
	}
	
	CNodeAnimation* pNodeAnimation = m_pRoot->GetAnimManager().CreateLightAnimation(pSceneNode, vTransform, pLight->getType(), vLightAnimInfo);

	return pNodeAnimation ? TRUE : FALSE;
}

//Read the Text data
DWORD CGetV3DImpV2::ReadTextData()
{
	return ERR_COMMON_OK;
}
//Read the overlay data
DWORD CGetV3DImpV2::ReadOverlayData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pOverlayBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}
	
 	char* Offset = m_pOverlayBuf;
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();
 
 	// ��ȡOverlay������
 	int iOverlayCnt = 0; 
 	CPY_FROM_STREAM(iOverlayCnt, Offset);

	// ��ȡOverlay���Բ�����Overlay����
	for (int iIndex = 0; iIndex < iOverlayCnt; iIndex++)
	{
 		V3D_OverlayInfo overlayInfo;
		CPY_FROM_STREAM(overlayInfo, Offset);

		std::string strBaseImage;
		std::string strOpacImage;
		if (strlen(overlayInfo.BaseImg) > 0) strBaseImage = AddPathtoTextureFile(overlayInfo.BaseImg);
		if (strlen(overlayInfo.OpacImg) > 0) strOpacImage = AddPathtoTextureFile(overlayInfo.OpacImg);

		CGUIObject* pOverlay = GUIMngr.CreateOverLayer(overlayInfo.Name, strBaseImage.c_str(), strOpacImage.c_str());
		if (NULL != pOverlay)
		{
 			// �ӿڴ�С
 			int iViewWidth = 0;
			int iViewHight = 0;
			GUIMngr.GetViewportSize(iViewWidth, iViewHight);
	 
			// overlay��ʵ�����꼰��С
 			float fLeft = 0.0f;
			float fTop = 0.0f;
			float fWidth = 0.0f;
			float fHeight = 0.0f;
			// ת���ɾ�������
 			if (overlayInfo.bRelativePos)
 			{
 				fLeft = iViewWidth * overlayInfo.Left;
 				fTop  = iViewHight * overlayInfo.Top;
 			}
 			else
 			{
 				fLeft = overlayInfo.Left;
 				fTop  = overlayInfo.Top;
 			}
			// ת���ɾ��Գߴ�
 			if (overlayInfo.bRelativeSize)  
 			{
 				fWidth  = iViewWidth * overlayInfo.Width;
 				fHeight = iViewHight * overlayInfo.Height;
 			}
 			else
 			{
 				fWidth  = overlayInfo.Width;
 				fHeight = overlayInfo.Height;
 			}
			pOverlay->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);	 //������Ϊ���ϽǱ�������λ��
			pOverlay->SetUsedRelativePos(overlayInfo.bRelativePos);
			pOverlay->SetUsedRelativeSize(overlayInfo.bRelativeSize);
			pOverlay->SetSize(fWidth, fHeight);
			pOverlay->SetOffsetOrigin(VGSOFFSETORIGIN(overlayInfo.Origin));	//����offset origin
			pOverlay->SetPosition(fLeft, fTop);
			pOverlay->SetOpacity(overlayInfo.Opacity);
			pOverlay->SetVisibility(overlayInfo.bVisible);

			// save to scene lits
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pOverlay->GetMaterial());
		}
	}
 
	return ERR_COMMON_OK;
}
//Read the navigater data
DWORD CGetV3DImpV2::ReadNavigaterData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pNaviBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}
 
 	char* Offset = m_pNaviBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// ��ȡ����ͼ������
 	int iNavCount = 0; 
 	CPY_FROM_STREAM(iNavCount, Offset);
 
	// ��ȡ����ͼ���Բ���������ͼ����
 	for (int iIndex = 0; iIndex < iNavCount; iIndex++)
 	{
		FRect mapRect;
		V3D_NavigatorInfo naviInfo; 
		ZeroMemory(&mapRect, sizeof(FRect));
		ZeroMemory(&naviInfo, sizeof(V3D_NavigatorInfo));
 
		// ��������
 		CPY_FROM_STREAM(naviInfo.NaviBaseInfo, Offset); 
 
		// map��������	
 		CPY_FROM_STREAM(mapRect, Offset); 
 
 		// �ӿڴ�С
		int iViewWidth = 0;
		int iViewHight = 0;
		GUIMngr.GetViewportSize(iViewWidth, iViewHight);
 
		// ת���ɾ�������
 		FRect rect;
 		if (naviInfo.NaviBaseInfo.bRelativePos)  
 		{
 			rect.Left = iViewWidth * naviInfo.NaviBaseInfo.Left;
 			rect.Top  = iViewHight * naviInfo.NaviBaseInfo.Top;
 		}
 		else
 		{
 			rect.Left = naviInfo.NaviBaseInfo.Left;
 			rect.Top  = naviInfo.NaviBaseInfo.Top;
 		} 
		// ת���ɾ�������
 		if (naviInfo.NaviBaseInfo.bRelativeSize)  
 		{
 			rect.Right  = rect.Left + iViewWidth * naviInfo.NaviBaseInfo.Width;
 			rect.Bottom = rect.Top + iViewHight * naviInfo.NaviBaseInfo.Height;
 		}
 		else
 		{
 			rect.Right  = rect.Left + naviInfo.NaviBaseInfo.Width;
 			rect.Bottom = rect.Top +naviInfo.NaviBaseInfo.Height;
 		}		
 
		// ���������
 		CPY_FROM_STREAM(naviInfo.PointerCount, Offset); 
		if (naviInfo.PointerCount > 0)
		{
			// ��������Ϣ
			naviInfo.PointerList = new V3D_NaviPointerInfo[naviInfo.PointerCount];
			ZeroMemory(naviInfo.PointerList, naviInfo.PointerCount*sizeof(V3D_NaviPointerInfo));
 			for (int iPointer = 0; iPointer < naviInfo.PointerCount; iPointer++)
 			{
 				CPY_FROM_STREAM(naviInfo.PointerList[iPointer], Offset);
 			}
		}
 
 		// ��������ͼ
		std::string strMapImage = AddPathtoTextureFile(naviInfo.NaviBaseInfo.MapImg);
		CNavigator* pNavigator = (CNavigator*)GUIMngr.CreateNavigator(naviInfo.NaviBaseInfo.Name, strMapImage.c_str());
		if (NULL != pNavigator)
		{
			pNavigator->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);					// ������Ϊ���ϽǱ�������λ��
			pNavigator->SetUsedRelativePos(naviInfo.NaviBaseInfo.bRelativePos);
			pNavigator->SetUsedRelativeSize(naviInfo.NaviBaseInfo.bRelativeSize);
			pNavigator->SetSize(rect.Right - rect.Left, rect.Bottom - rect.Top);
			pNavigator->SetOffsetOrigin(VGSOFFSETORIGIN(naviInfo.NaviBaseInfo.Origin));  // ����offset origin
			pNavigator->SetPosition(rect.Left, rect.Top);
			pNavigator->SetOpacity(naviInfo.NaviBaseInfo.Opacity);
			pNavigator->SetVisibility(naviInfo.NaviBaseInfo.bVisible);
			//
			pNavigator->SetNavRange(mapRect);

			// save to scene lits
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pNavigator->GetMaterial());

			//
			for (int iPointer = 0; iPointer < naviInfo.PointerCount; iPointer++)
			{
				std::string strPointerImg = AddPathtoTextureFile(naviInfo.PointerList[iPointer].PointerImg);
				CNavPointer* pPointer = (CNavPointer*)GUIMngr.CreateNaviPointer(naviInfo.PointerList[iPointer].sName, strPointerImg.c_str());
				if (NULL != pPointer)
				{					
 					DWORD dwPointedHandle = 0;
					if (0 == naviInfo.PointerList[iPointer].NavType &&
						naviInfo.PointerList[iPointer].PointedIndex >= 0 &&
						naviInfo.PointerList[iPointer].PointedIndex < m_vpCamera.size())	 // camera
					{
						dwPointedHandle = (DWORD)m_vpCamera[naviInfo.PointerList[iPointer].PointedIndex];
					}
					else if (1 == naviInfo.PointerList[iPointer].NavType &&
						naviInfo.PointerList[iPointer].PointedIndex >= 0 &&
						naviInfo.PointerList[iPointer].PointedIndex < m_vModel.size())		// ģ��
					{
						dwPointedHandle = (DWORD)m_vModel[naviInfo.PointerList[iPointer].PointedIndex];
					}
					pPointer->SetPointerHandle(dwPointedHandle);
					pPointer->SetNavType(naviInfo.PointerList[iPointer].NavType);
					pPointer->SetVisibility(naviInfo.PointerList[iPointer].bVisible);
					pPointer->SetOpacity(naviInfo.PointerList[iPointer].Opacity);
					pPointer->SetSize(naviInfo.PointerList[iPointer].width, naviInfo.PointerList[iPointer].height);
					pPointer->SetPosition(rect.Left + naviInfo.PointerList[iPointer].left, rect.Top + naviInfo.PointerList[iPointer].top);

					pNavigator->AddNavPointer(pPointer);

					// save to scene lits
					m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pPointer->GetMaterial());
				}
			}
		}
			
		// �ͷ��ڴ�
		if (NULL != naviInfo.PointerList)
		{
			delete []naviInfo.PointerList;
			naviInfo.PointerList = NULL;
		}
 	}

	return ERR_COMMON_OK;
}

//Read the button data
DWORD CGetV3DImpV2::ReadButtonData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pButtonBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}
 
 	char* Offset = m_pButtonBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// ��ȡbutton������
 	int iBtnCount = 0; 
 	CPY_FROM_STREAM(iBtnCount, Offset);
 
	// ��ȡbutton���Բ�����button����
 	for (int iIndex = 0; iIndex < iBtnCount; iIndex++)
 	{
 		V3D_ButtonInfo BtnInfo;
 		CPY_FROM_STREAM(BtnInfo, Offset); 

 		// ����button
		RECT rcBtn;
		std::string strUpImage;
		std::string strOverImage;
		std::string strDownImage;
		memset(&rcBtn, 0, sizeof(RECT));
		if (strlen(BtnInfo.UpImageName) > 0)
			strUpImage = AddPathtoTextureFile(BtnInfo.UpImageName);
		if (strlen(BtnInfo.OverImageName) > 0)
			strOverImage = AddPathtoTextureFile(BtnInfo.OverImageName);
		if (strlen(BtnInfo.DownImageName) > 0)
			strDownImage = AddPathtoTextureFile(BtnInfo.DownImageName);
		CButton* pButton = (CButton*)GUIMngr.CreateButton(BtnInfo.Name, rcBtn, strUpImage.c_str(), strOverImage.c_str(), strDownImage.c_str());
		if (NULL != pButton)
		{	
			// �ӿڴ�С
			int iViewWidth = 0;
			int iViewHight = 0;
			GUIMngr.GetViewportSize(iViewWidth, iViewHight);

 			float fLeft = 0.0f;
			float fTop = 0.0f;
			float fWidth = 0.0f;
			float fHeight = 0.0f; 

			// ת���ɾ�������
 			if (BtnInfo.bRelativePos)  
 			{
 				fLeft = iViewWidth * BtnInfo.Left;
 				fTop  = iViewHight * BtnInfo.Top;
 			}
 			else
 			{
 				fLeft = BtnInfo.Left;
 				fTop  = BtnInfo.Top;
 			}
	 
			// ת���ɾ�������
 			if (BtnInfo.bRelativeSize)  			
			{
 				fWidth  = iViewWidth * BtnInfo.Width;
 				fHeight = iViewHight * BtnInfo.Height;
 			}
 			else
 			{
 				fWidth  = BtnInfo.Width;
 				fHeight = BtnInfo.Height;
 			}

			pButton->SetOffsetOrigin(VGSOFFSETORIGIN::VOO_TOP_LEFT);   // ������Ϊ���ϽǱ�������λ��
			pButton->SetUsedRelativePos(BtnInfo.bRelativePos);
			pButton->SetUsedRelativeSize(BtnInfo.bRelativeSize);
			pButton->SetSize(fWidth, fHeight);
			pButton->SetOffsetOrigin(VGSOFFSETORIGIN(BtnInfo.Origin));  // ����offset origin
			pButton->SetPosition(fLeft, fTop);
			pButton->SetOpacity(BtnInfo.Opacity);
			pButton->SetVisibility(BtnInfo.bVisible);
			pButton->SetJsCallbackFunc(BtnInfo.CallbackFunName);

			// save to scene lits
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pButton->GetMaterial(0));
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pButton->GetMaterial(1));
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pButton->GetMaterial(2));
		} 		
 	}
	
	return ERR_COMMON_OK;
}

// ����ͼ
DWORD CGetV3DImpV2::ReadBackdropData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pBackdropBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}

 	char* Offset = m_pBackdropBuf; 
	CGUIManager& GUIMngr = m_pVGSSceneMgr->GetGuiManagerRef();

 	// ��ȡ����ͼƬ����
 	int iBkgndCount = 0; 
 	CPY_FROM_STREAM(iBkgndCount, Offset);

	// ��ȡ����ͼƬ���Բ���������ͼƬ����
 	for (int iIndex = 0; iIndex < iBkgndCount; iIndex++)
 	{
 		V3D_BackdropInfo bkgndInfo;
 		CPY_FROM_STREAM(bkgndInfo, Offset); 
		//
		std::string strImage;
		if (strlen(bkgndInfo.ImgFile) > 0)
			strImage = AddPathtoTextureFile(bkgndInfo.ImgFile);
		//
		BackGround* pBkgrnd = GUIMngr.CreateBackGroudLayer(strImage.c_str());
		if (NULL != pBkgrnd)
		{
			pBkgrnd->SetVisibility(bkgndInfo.bVisible);
			pBkgrnd->SetRect(bkgndInfo.Left, bkgndInfo.Top, bkgndInfo.Width, bkgndInfo.Height);

			// save to scene lits
			m_pVGSSceneMgr->AddMaterial(VGS_MATERIAL_TYPE_2D, pBkgrnd->GetMaterial());
		}
	}
 
	return ERR_COMMON_OK;
}

//Read AudioData
DWORD CGetV3DImpV2::ReadAudioData()
{
	return ERR_COMMON_OK;
}

//Read FlashData
DWORD CGetV3DImpV2::ReadFlashData()
{
	return ERR_COMMON_OK;
}

// ����
DWORD CGetV3DImpV2::ReadAnimationData()
{
	return ERR_COMMON_OK;
}

//Read privacy data
DWORD CGetV3DImpV2::ReadPrivacyData()
{
	return ERR_COMMON_OK;
}

// ����
DWORD CGetV3DImpV2::ReadControlData()
{
 	if (!m_pFile)
 	{
 		return ERR_FILE_OPENFAIL;
 	}	
 	if (!m_pVGSSceneMgr || !m_pControlBuf)
 	{
 		return ERR_FILE_V3D_NODATA;
 	}	
 
 	char* Offset = m_pControlBuf; 
		
 	// ��ȡ�ܵĿ�������Ŀ
 	DWORD TotalCount = 0;
 	CPY_FROM_STREAM(TotalCount, Offset)	
 	if (TotalCount == 0)
 	{
 		return ERR_COMMON_OK;
 	}
 
	// walker��Ŀ
 	DWORD WalkerCount = 0;                        
 	CPY_FROM_STREAM(WalkerCount, Offset) 	
	// ��ȡwalker��Ŀ
	for (UINT i = 0; i < WalkerCount; ++i)
 	{
 		WalkerInfo Info;
 		CPY_FROM_STREAM(Info, Offset)
 		m_pVGSSceneMgr->CreateWalker(Info);
 	}
 
	// flyer��Ŀ
 	DWORD FlyerCount = 0;
	// ��ȡflyer��Ŀ
 	CPY_FROM_STREAM(FlyerCount, Offset)
 	for (UINT i = 0; i < FlyerCount; ++i)
 	{
 		FlyerInfo Info;
 		CPY_FROM_STREAM(Info, Offset)
 		m_pVGSSceneMgr->CreateFlyer(Info);
 	}
 
	// editor��Ŀ
 	DWORD EditorCount = 0;
	// ��ȡeditor��Ŀ
 	CPY_FROM_STREAM(EditorCount, Offset)
 	for (UINT i = 0; i < EditorCount; ++i)
 	{
 		if (m_V3DFileVersion < 50004)
 		{
 			EditorInfo Info;
 			EditorInfo_Old Info_Old;
 			CPY_FROM_STREAM(Info_Old, Offset)
 			memcpy(&Info, &Info_Old, sizeof(Info_Old));
 			Info.LookAtType = 0;
 			Info.LookAtModelIndex = 0xffffffff;
 			m_pVGSSceneMgr->CreateEditor(Info);
 		}
 		else
 		{
 			EditorInfo Info;
 			CPY_FROM_STREAM(Info, Offset)
			if (Info.LookAtModelIndex >= 0 && Info.LookAtModelIndex < m_vModel.size())
 			{
 				//Info.LookAtModelIndex = m_vModel[Info.LookAtModelIndex];  // ����Ϊ������ģ�;��
 			}
 			m_pVGSSceneMgr->CreateEditor(Info);
 		}
 	}
 
 	BYTE type;
 	UINT Index;
 	CPY_FROM_STREAM(type, Offset) 
 	CPY_FROM_STREAM(Index, Offset) 
 
 	m_pVGSSceneMgr->SetCurrentController(NULL);
 
 	if ((CONTROLTYPE)type == CT_EDITOR)
 	{
 		if (Index >= 0 && Index < m_pVGSSceneMgr->GetEditorCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetEditor(Index));
 			((CEditor*)(m_pVGSSceneMgr->GetEditor(Index)))->SetCamera(m_pVGSSceneMgr->GetEditor(Index)->GetCamera()); // ������������������Ŀ���
 		}
 	}
 	else if ((CONTROLTYPE)type == CT_WALKER)
 	{
 		if (Index >= 0 && Index < m_pVGSSceneMgr->GetWalkerCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetWalker(Index));
 		}
 	}
 	else if ((CONTROLTYPE)type == CT_FLYER)
 	{
 		if (Index >= 0 && Index < m_pVGSSceneMgr->GetFlyerCount())
 		{
 			m_pVGSSceneMgr->SetCurrentController(m_pVGSSceneMgr->GetFlyer(Index));
 		}
 	}
	
	return ERR_COMMON_OK;
}

//�ݹ鴴������
void CGetV3DImpV2::ConstructSceneGraph(CSceneNode* pNode, char* offset)
{
	CSceneNode* pChildNode = NULL;
	bool	bHasSkeleton = false;
	VGS_Node node;
	string RealName;

	//����node������
	CPY_FROM_STREAM(node.type, offset);
	
	//����node�ı��
	CPY_FROM_STREAM(node.index, offset);

	//����node������
	CPY_FROM_STREAM(node.strLen, offset);
	node.name.reserve(node.strLen);
	memcpy((void*)(node.name.c_str()), offset, node.strLen);
	offset += node.strLen;
	// m_Offset += node.strLen;

	//���벢��������\ת��\λ��
	VECTOR3 pos;
	CPY_FROM_STREAM(pos.x, offset);
	CPY_FROM_STREAM(pos.y, offset);
	CPY_FROM_STREAM(pos.z, offset);
	pos.z *= -1;

	D3DXQUATERNION quat;
	CPY_FROM_STREAM(quat.x, offset);
	CPY_FROM_STREAM(quat.y, offset);
	CPY_FROM_STREAM(quat.z, offset);
	CPY_FROM_STREAM(quat.w, offset);	//
	quat.x = -quat.x;
	quat.y = -quat.y;

	VECTOR3 scale;
	CPY_FROM_STREAM(scale.x, offset);
	CPY_FROM_STREAM(scale.y, offset);
	CPY_FROM_STREAM(scale.z, offset);

	std::vector<CNodeTransform> vNodeTransform;

	float animationLen = 0;
	CPY_FROM_STREAM(animationLen, offset);
	if (animationLen > 0)
	{
		int keyFrameCnt;
		CPY_FROM_STREAM(keyFrameCnt, offset);
		vNodeTransform.reserve(keyFrameCnt);
		for (INT kfrIndex = 0; kfrIndex < keyFrameCnt; kfrIndex++)
		{
			NodeTransform transform;
			CPY_FROM_STREAM(transform, offset);
			//
			CNodeTransform nodeTrans;
			nodeTrans.position = transform.position;
			nodeTrans.position.z *= -1;
			nodeTrans.quat = transform.quat;
			nodeTrans.quat.x *= -1;
			nodeTrans.quat.y *= -1;
			nodeTrans.scale = transform.scale;
			if (nodeTrans.scale.x <= 0.001)
			{
				nodeTrans.scale.x = 1;
			}
			if (nodeTrans.scale.y <= 0.001)
			{
				nodeTrans.scale.y = 1;
			}
			if (nodeTrans.scale.z <= 0.001)
			{
				nodeTrans.scale.z = 1;
			}
			nodeTrans.time = transform.timeElapsed;
			vNodeTransform.push_back(nodeTrans);
		}
	}

	//Node�ı任, ����ı任�������ﴦ��---------------------------------------------------------
	//λ��//ת������ϵ
	if (node.type == SCENEROOT)
	{		
		pChildNode = pNode;
		pChildNode->setScale(&D3DXVECTOR3(scale.x, scale.y, scale.z), VTS_WORLD);
		pChildNode->setRotationByQuat(&quat, VTS_WORLD);
		pChildNode->setPosition(&D3DXVECTOR3(pos.x, pos.y, pos.z), VTS_WORLD);
	}
	else
	{
		pChildNode = pNode->CreateChild();
		if (pChildNode)
		{
			//����//ת������ϵ
			pChildNode->setScale(&D3DXVECTOR3(scale.x, scale.y, scale.z), VTS_PARENT);
			//pChildNode->updateTransform();
			//��ת//ת������ϵ
			pChildNode->setRotationByQuat(&quat, VTS_PARENT);
			//pChildNode->updateTransform();
			//λ��
			pChildNode->setPosition(&D3DXVECTOR3(pos.x, pos.y, pos.z), VTS_PARENT);
			//pChildNode->updateTransform();
			
			if (node.type == CAMERANODE)
			{
				if (node.index >= 0 && node.index < (int)m_vpCamera.size())
				{				
					CCamera *pCamera = m_vpCamera[node.index];
					if (pCamera)
					{
						D3DXVECTOR3 v3Pos = pCamera->GetPosition(VTS_PARENT);
						D3DXVECTOR3 v3At  = pCamera->GetLookat(VTS_PARENT);
						pChildNode->attachObject(pCamera);
						pCamera->m_pNode = pChildNode;
						pCamera->SetPosition(&v3Pos, VTS_PARENT);
						//
						if (animationLen > 0)
						{
							CreateCameraNodeAnimation(pChildNode, pCamera, vNodeTransform);
						}
					}
				}
			}
			else if (node.type == LIGHTNODE)
			{			
				if (node.index >= 0 && node.index < (int)m_vpLight.size())
				{
					CLight *pLight = m_vpLight[node.index];
					if (pLight)
					{
						pChildNode->attachObject(m_vpLight[node.index]);
						m_vpLight[node.index]->m_pNode = pChildNode;
						//
						if (animationLen > 0)
						{
							CreateLightNodeAnimation(pChildNode, pLight, vNodeTransform);
						}
					}
				}
			}
			else if (node.type == MESHNODE)
			{			
				if (node.index >= 0 && node.index < (int)m_vModel.size())
				{
					CModel *pModel = m_vModel[node.index];
					if (pModel)
					{
						pChildNode->attachObject(pModel);
						pModel->m_pNode = pChildNode;
						pModel->UpdateFromNode();
						//			
						if (animationLen > 0)
						{			
							m_pRoot->GetAnimManager().CreateModelAnimation(pChildNode, vNodeTransform);
						}
					}				
				}
			} // else if
		} // if pChildNode
			
	}  // node type

	m_Offset = offset - m_pSceneGraphBuf; // �ض�λoffset

	//�����ӽڵ�����
	int childNodeCnt = 0;
	CPY_FROM_STREAM(childNodeCnt, offset);
	for (int i = 0; i< childNodeCnt; i++)
	{
		ConstructSceneGraph(pChildNode, m_pSceneGraphBuf + m_Offset);
	}
}

//�����ڵ�Ķ���
void CGetV3DImpV2::ReadAndCreateNodeAnimation(CSceneNode* pNode, float animationLength, char** offset)
{
	//���붯������-------------------------------------------------
	//�����ؼ�֡����
	// Ogre::String strAnimName = "Anim_" + pNode->getName();
// 	Ogre::String strAnimName = _GetNodeAnimationName(pNode);  // ��ȡ�ڵ㶯������
// 	Animation* pAnim = m_pVGSSceneMgr->mSceneMgr->createAnimation(strAnimName, animationLength);
// 
// 	//����ؼ�֡�����Ĺؼ�֡����
// 	int keyframeCnt;
// 	CPY_FROM_STREAM(keyframeCnt, *offset);
// 
// 	//���ù��ɷ�ʽ
// 	pAnim->setInterpolationMode(Ogre::Animation::InterpolationMode::IM_LINEAR);
// 	
// 	//����һ�ζ����켣
// 	NodeAnimationTrack* pAnimTrack = pAnim->createNodeTrack(0, pNode);
// 	//��֡��������
// 	for (int kfrIndex = 0; kfrIndex< keyframeCnt; kfrIndex++)
// 	{
// 		NodeTransform nodeTransform; 
// 		CPY_FROM_STREAM(nodeTransform, *offset);
// 		
// 		//��ǰ֡��Ӧ��Transform����
// 		TransformKeyFrame* pKfr = pAnimTrack->createNodeKeyFrame(nodeTransform.time);
// 
// 		//λ��//����ϵת��
// 		Vector3 pos;
// 		pos.x = nodeTransform.position.x;
// 		pos.y = nodeTransform.position.y;
// 		pos.z = nodeTransform.position.z;
// 		pKfr->setTranslate(pos);
// 
// 		//ת��//����ϵת��
// 		Ogre::Quaternion quat;
// 		quat.x = nodeTransform.quat.x;
// 		quat.y = nodeTransform.quat.y;
// 		quat.z = nodeTransform.quat.z;
// 		quat.w = nodeTransform.quat.w;
// 		pKfr->setRotation(quat);
// 
// 		//����
// 		Vector3 scale;
// 		scale.x = nodeTransform.scale.x;
// 		scale.y = nodeTransform.scale.y;
// 		scale.z = nodeTransform.scale.z;
// 		pKfr->setScale(scale);
// 	}
// 	
// 	//�Ż�
// 	pAnimTrack->optimise();
// 
// 	//��������״̬����
// 	AnimationState* pAnimState = m_pVGSSceneMgr->mSceneMgr->createAnimationState(strAnimName);
// 	pAnimState->setEnabled(true);
// 	pAnimState->setLoop(true);
// 
// 	// ����һ���ڵ㶯��
// 	m_pVGSSceneMgr->GetAnimationManager()->AddNodeKeyfrmAnimation(pAnimState);
}


// ����ͼ
DWORD CGetV3DImpV2::ReadSceneGraph()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pSceneGraphBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}
		
	//�ݹ���볡��ͼ�ṹ������������
	m_Offset = 0;
	CSceneNode* pRootNode = m_pVGSSceneMgr->GetRootSceneNode();
	ConstructSceneGraph(pRootNode, m_pSceneGraphBuf);	// ��ȡSceneGraph��Ϣ// �ֽ��������ݿ�ʼ��λ��

	// ɾ���ֽ���
	DELETE_ARRAYPTR(m_pSceneGraphBuf);  

	return ERR_COMMON_OK;
}

// �õ�texture�������ļ��еķ�Χ
void CGetV3DImpV2::GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len)
{
	
	// ����������λ����Ϣ��С����������texturePtr�����λ�ü�ȥtexturePtr����texure�ĳ���
// 	std::vector<DWORD> v_ptrSortVector;
// 	
//     v_ptrSortVector.push_back(table.SceneproperyPtr);
// 	v_ptrSortVector.push_back(table.mtrlDataPtr);
// 	v_ptrSortVector.push_back(table.texturePtr);
// 	v_ptrSortVector.push_back(table.cameraDataPtr);
// 	v_ptrSortVector.push_back(table.lightDataPtr);
// 	v_ptrSortVector.push_back(table.skeletonDataPtr);
// 	v_ptrSortVector.push_back(table.meshDataPtr);
// 	v_ptrSortVector.push_back(table.shapeDataPtr);
// 	v_ptrSortVector.push_back(table.sceneGraphDataPtr);
// 	v_ptrSortVector.push_back(table.TextDataPtr);
// 	v_ptrSortVector.push_back(table.AnimationPtr);
// 	v_ptrSortVector.push_back(table.OverlayPtr);
// 	v_ptrSortVector.push_back(table.ButtonPtr);
// 	v_ptrSortVector.push_back(table.NavigatotPtr);
// 	v_ptrSortVector.push_back(table.AudioPtr);
// 	v_ptrSortVector.push_back(table.ControlPtr);
// 	v_ptrSortVector.push_back(table.FlashPtr);
// 	v_ptrSortVector.push_back(table.PrivacyDataPtr);
// 
// 	std::sort(v_ptrSortVector.begin(), v_ptrSortVector.end());
//     
// 	std::vector<DWORD>::iterator it = v_ptrSortVector.begin();
// 	for (; it != v_ptrSortVector.end(); ++it)
// 	{
// 		if ((*it) == table.texturePtr)
// 		{
// 			len = (*(it + 1)) - table.texturePtr;
// 		   break;
// 		}
// 	}
// 
// 	start = table.texturePtr;        // texture��ʼλ��
}

// ��ָ���Ĵ��ļ������·��
std::string CGetV3DImpV2::AddPathtoTextureFile(const std::string &sPureFileName)
{
	// ����ļ��Ƿ���ָ��������·���»�ǰ��Դ·����
// 	Ogre::String str = "";
// 	if (_IsFileNameInResourcePath(Ogre::String(sPureFileName), str))
// 	{
// 		return str;
// 	}
	std::string str = m_sDefaultTexturePath;
	str.append(sPureFileName);
	return str;
}

// ���·�����ͼ�еĵƹ���
VOID CGetV3DImpV2::UpdateBumpNormalmapLight()
{
// 	UINT count = m_pVGSSceneMgr->GetBumpmapCount();
// 	for (UINT i = 0; i < count; ++i)
// 	{
// 		VGSBumpmapInfo info;
// 		m_pVGSSceneMgr->GetBumpmapParam(i, info);
// 		if (info.m_Type == VBT_NORMARLMAP)
// 		{
// 			INT index = (INT)info.m_BumpNormalParam.LightHandle;
// 			if (index >= 0 && index < m_vpLight.size())
// 			{
// 				info.m_BumpNormalParam.LightHandle = (DWORD)m_vpLight[index];
// 			}
// 			else
// 			{
// 				info.m_BumpNormalParam.LightHandle = 0;
// 			}
// 		}
// 	}
}

// �������ɺ��һЩ����
VOID CGetV3DImpV2::PostOpen()
{
	// ���·�����ͼ�еĵƹ���
	// UpdateBumpNormalmapLight();
}