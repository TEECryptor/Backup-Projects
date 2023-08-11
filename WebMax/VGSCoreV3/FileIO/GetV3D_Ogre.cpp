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


#include "GetV3D.h"
#include "VGSErrCode.h"

#include "vgsSupport.h"

#include "VGSCommonDef.h"
#include "AnimationManager.h"

#include"CEditor.h"

using namespace Ogre;

#define RETURN(RE)                       {if (RE != ERR_COMMON_OK)return RE;} 

// ɾ������
#define DELETE_ARRAYPTR(Arrayptr)        {if (Arrayptr) {delete [] Arrayptr; Arrayptr = NULL;}}                                

// ��Offsetλ�ÿ��������ֽ�
#define CPY_FROM_STREAM(value, Offset)   {memcpy(&value, Offset, sizeof(value)); Offset += sizeof(value);  m_Offset += sizeof(value);} 

// ��offsetλ�ÿ���һ��vector3���ݽṹ
#define CPY_VECTOR3_FROM_STRRAM(V3, Offset)\
{\
	CPY_FROM_STREAM((V3.x), Offset)\
	CPY_FROM_STREAM((V3.y), Offset)\
	CPY_FROM_STREAM((V3.z), Offset)\
}

// ���ļ��ж�һ��Vector3
#define READ_VECTOR3(V3, pFile) {fread(&(V3.x), sizeof(V3.x), 1, pFile); fread(&(V3.y), sizeof(V3.y), 1, pFile); fread(&(V3.z), sizeof(V3.z), 1, pFile);}

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

CGetV3D::CGetV3D()
{
	m_pVGSSceneMgr = NULL;
	m_pFile = NULL;

	m_sV3DPath = "";
	sDefaultTexturePath = "";

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

CGetV3D::~CGetV3D()
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

void CGetV3D::Release()
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
void CGetV3D::SetCurrentDirctory(const char *sFileName)
{
	int maxlen = strlen(sFileName);
	char *pPath = new char[maxlen];
	ZeroMemory(pPath, maxlen);
	GetPureFilePath(sFileName, pPath);
	m_sV3DPath = String(pPath);
	sDefaultTexturePath = m_sV3DPath + "resource\\texture\\";
}

// ��ȡV3D�ļ���TableĿ¼
bool CGetV3D::GetV3DTable(const CHAR* strFileName, V3D_Table &Table)
{
	// �����ļ�
	if (NULL == (m_pFile = fopen(strFileName, "rb")) )
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

DWORD CGetV3D::OpenV3DFile(CVGS_SceneManager* pVGSSceneMgr, const CHAR* strFileName)
{
	try
	{
		if (pVGSSceneMgr == NULL)
		{
			// ::MessageBox(NULL, LPCSTR("�Ҳ�������"), LPCSTR("����"), MB_OK | MB_ICONERROR);
			return ERR_SCENE_NOSCENE; // �޷���ȡ������
		}
		m_pVGSSceneMgr = (CVGS_SceneManager*)pVGSSceneMgr;

		// �����ļ�
		if (NULL == (m_pFile = fopen(strFileName, "rb")) )
		{
			// ::MessageBox(NULL, LPCSTR("�����ļ�ʧ��"), LPCSTR("����"), MB_OK | MB_ICONERROR);
			return ERR_FILE_OPENFAIL;
		}
		
		SetCurrentDirctory(strFileName);  // �õ���ǰ��V3DĿ¼�����ڼ�����ͼ����Դ

		// m_vCameraDetail.clear();
		m_vpCamera.clear();
		m_vpLight.clear();

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

		if (m_pVGSSceneMgr->ogreResGroupName.empty())
		{
			m_pVGSSceneMgr->CreateResGroupName();

			//����ϵͳ�ṩ����Դ�鼰��Դ·����������Ч����Ⱦ
			ResourceGroupManager::getSingleton().createResourceGroup(m_pVGSSceneMgr->ogreResGroupName);
			ResourceGroupManager::getSingleton().addResourceLocation(m_sV3DPath + "Resource/materials", "FileSystem");
			ResourceGroupManager::getSingleton().addResourceLocation(m_sV3DPath + "Resource/texture/", "FileSystem");
			ResourceGroupManager::getSingleton().addResourceLocation(m_sV3DPath + "Resource/scripts", "FileSystem");
			ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		}

		/////////////// �������� /////////////////////

		//Read the  camera data
		result = ReadCameraData();
		RETURN(result);

		//Read the texture data
		result = ReadTextureData();
		RETURN(result);

		//Read the Material data
		result = ReadMaterialData();
		RETURN(result);

		//Read the light data
		result = ReadLightData();
		RETURN(result);

		// ��ȡģ����Դ���ݣ�ע��Ҫ�ȶ�mesh���ٶ�entity
		result = ReadMeshData();
		RETURN(result);

		// Read the Model data
		result = ReadEnityData();
		RETURN(result);

		//Read the Text data
		result = ReadTextData();
		RETURN(result);

		//Read the overlay data
		result = ReadOverlayData();
		RETURN(result);

		//Read the navigater data
		result = ReadNavigaterData();
		RETURN(result);

		//Read the button data
		result = ReadButtonData();
		RETURN(result);

		//Read the backdrop
		result = ReadBackdropData();
		RETURN(result);

		//Read AudioData
		result = ReadAudioData();
		RETURN(result);

		//Read FlashData
		result = ReadFlashData();
		RETURN(result);

		// ����
		result = ReadAnimationData();
		RETURN(result);

		//Read privacy data
		result = ReadPrivacyData();
		RETURN(result);

		// ����
		result = ReadControlData();
		RETURN(result);

		result = ReadSceneGraph();
		RETURN(result);

		PostOpen();  // �������ɺ��һЩ����
 
		Release(); // �ͷ�buf�ڴ�

		return ERR_COMMON_OK;
	}
	catch (Ogre::Exception &e)
	{
		//fclose(m_pFile);
		return ERR_FILE_OPENFAIL;
	}
}

DWORD CGetV3D::ReadHeader()
{
	if (!m_pFile)
	{
		// throw Exception::exception("���ļ�ͷ��������(01)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}

	fseek(m_pFile, 0, SEEK_SET); // ��λ���ļ���ͷ
	V3D_Header V3DHeader;
	fread(&V3DHeader, sizeof(V3DHeader), 1, m_pFile);
	if (0 != strcmp(String(V3DHeader.corp).c_str(), SUNTOWARD_NAME))
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


DWORD CGetV3D::ReadFileTable()
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

DWORD CGetV3D::ReadWindowProp()
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
DWORD CGetV3D::ReadAllData()
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

	// �������
	UINT CameDataSize = m_V3DTable.lightDataPtr - m_V3DTable.cameraDataPtr;
	m_pCameraBuf = new char[CameDataSize];
	// fseek(m_pFile, m_V3DTable.cameraDataPtr, SEEK_SET);	
	if (1 != fread(m_pCameraBuf, CameDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

	// �ƹ�
	UINT LightDataSize = m_V3DTable.texturePtr - m_V3DTable.lightDataPtr;
	m_pLightBuf = new char[LightDataSize];
	// fseek(m_pFile, m_V3DTable.lightDataPtr, SEEK_SET);
	if (1 != fread(m_pLightBuf, LightDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

	// ��ͼ��������
	UINT TextureDataSize = m_V3DTable.mtrlDataPtr - m_V3DTable.texturePtr;
	m_pTextureBuf = new char[TextureDataSize];
	// fseek(m_pFile, m_V3DTable.texturePtr, SEEK_SET);
	if (1 != fread(m_pTextureBuf, TextureDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

	// ��������
	UINT MtrDataSize = m_V3DTable.entityDataPtr - m_V3DTable.mtrlDataPtr;
	m_pMaterailBuf = new char[MtrDataSize];
	// fseek(m_pFile, m_V3DTable.mtrlDataPtr, SEEK_SET);
	if (1 != fread(m_pMaterailBuf, MtrDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

	// entity
	UINT EntityDataSize = m_V3DTable.meshDataPtr - m_V3DTable.entityDataPtr;
	m_pEntityBuf = new char[EntityDataSize];
	// fseek(m_pFile, m_V3DTable.meshDataPtr, SEEK_SET);
	if (1 != fread(m_pEntityBuf, EntityDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

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
	// overlay
	UINT OverlayDataSize = m_V3DTable.NavigatotPtr - m_V3DTable.OverlayPtr;
	m_pOverlayBuf = new char[OverlayDataSize];
	// fseek(m_pFile, m_V3DTable.OverlayPtr, SEEK_SET);
	if (1 != fread(m_pOverlayBuf, OverlayDataSize, 1, m_pFile))
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	// ����ͼ
	UINT NaviSize = m_V3DTable.ButtonPtr - m_V3DTable.NavigatotPtr;
	m_pNaviBuf = new char[NaviSize];
	int ReadSize = fread(m_pNaviBuf, NaviSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	// ��ť
	UINT ButtonSize = m_V3DTable.BackdropPtr - m_V3DTable.ButtonPtr;
	m_pButtonBuf = new char[ButtonSize];
	ReadSize = fread(m_pButtonBuf, ButtonSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

	// ����ͼ
	UINT BackdropSize = m_V3DTable.AudioPtr - m_V3DTable.BackdropPtr;
	m_pBackdropBuf = new char[BackdropSize];
	ReadSize = fread(m_pBackdropBuf, BackdropSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

	// ����
	UINT AudioSize = m_V3DTable.FlashPtr - m_V3DTable.AudioPtr;
	m_pAudioBuf = new char[AudioSize];
	ReadSize = fread(m_pAudioBuf, AudioSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	// flash
	UINT FlashDataSize = m_V3DTable.AnimationPtr - m_V3DTable.FlashPtr;
	m_pFlashBuf = new char[FlashDataSize];
	ReadSize = fread(m_pFlashBuf, FlashDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	// ����
	UINT AnimationDataSize = m_V3DTable.ControlPtr - m_V3DTable.AnimationPtr;
	m_pAnimationBuf = new char[AnimationDataSize];
	ReadSize = fread(m_pAnimationBuf, AnimationDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}
	// ������Ϣ
	UINT ControlDataSize = m_V3DTable.sceneGraphDataPtr - m_V3DTable.ControlPtr;
	m_pControlBuf = new char[ControlDataSize];
	fseek(m_pFile, m_V3DTable.ControlPtr, SEEK_SET);
	ReadSize = fread(m_pControlBuf, ControlDataSize, 1, m_pFile); 
	if (ReadSize != 1)
	{
		return ERR_FILE_READBYTEFAIL; // ��ȡԤ�����ֽ�������
	}

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

	return ERR_COMMON_OK;
}


DWORD CGetV3D::ReadTextureData()
{
	DELETE_ARRAYPTR(m_pTextureBuf);  // ɾ���ֽ���
	return ERR_COMMON_OK;
}

DWORD CGetV3D::ReadMaterialData()
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

	// ��ȡ������Ϣ
	char* Offset = m_pMaterailBuf;          // �ֽ��������ݿ�ʼ��λ��
	DWORD MtrCount = 0;                     // ������Ŀ
	CPY_FROM_STREAM(MtrCount, Offset)       // ��ȡ������Ŀ
	for (DWORD MtrIdx = 0; MtrIdx < MtrCount; ++MtrIdx)
	{
		DWORD len = 0;
		CPY_FROM_STREAM(len, Offset)        // Mtrl Name length
		MaterialPtr pMtrl;
		if (len > 0)
		{
			char *pName = new char[len + 1];
			ZeroMemory(pName, len + 1);
			memcpy(pName, Offset, len);         // Mtrl Name
			Offset += len;
			// ����������Դ��ָ����Դ��, Ogre���Զ�����һ��technique��һ��pass
			pMtrl = MaterialManager::getSingleton().create(pName, m_pVGSSceneMgr->ogreResGroupName);
			delete [] pName;
			pName = NULL;
		}
		else
		{
			// ����������Դ��ָ����Դ��, Ogre���Զ�����һ��technique��һ��pass
			// pMtrl = MaterialManager::getSingleton().create(pName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			MessageBox(NULL, LPCSTR("������û��Ϊ����ָ�����ƣ����ܻᵼ�����⣡"), LPCSTR("����"), 0);
			continue;
		}

		bool  bRecShadow;
		DWORD TechCount;
		float USpeed, VSpeed;  // ��̬UV�ٶ�
		CPY_FROM_STREAM(bRecShadow, Offset)   // Mtrl Receive Shadow
		CPY_FROM_STREAM(USpeed, Offset)   // Mtrl Receive Shadow
		CPY_FROM_STREAM(VSpeed, Offset)    // Mtrl Technique count

		pMtrl->setReceiveShadows(bRecShadow); // set Mtrl receive shadow
		
		CPY_FROM_STREAM(TechCount, Offset)    // Mtrl Technique count

		// ���ڴ�������ʱ�����Զ�����һ��technique��pass�����ԣ��������һ��techniqueʱ���Ͳ��ô�����
		if (TechCount == 1)
		{
			Technique * pTech = pMtrl->getTechnique(0);		

			UINT namelen;
			CPY_FROM_STREAM(namelen, Offset)               // length of tech name
			if (namelen > 0)
			{
				char *pName = new char[namelen + 1];
				ZeroMemory(pName, namelen + 1);
				memcpy(pName, Offset, namelen);                 // tech name
				Offset += namelen;
				pTech->setName(pName);                           // ���ü���������				
				_AddTextureLayerName(pName);
				delete [] pName;
				pName = NULL;
			}

			UINT passCount;
			CPY_FROM_STREAM(passCount, Offset)              // pass count of this technique
			
			if (passCount == 1)  // ���ֻ��һ��pass�� ֱ�Ӹ�ֵ���ɣ�����Ҫ�´�����
			{
				//���ò��ʵĲ���
				Pass* pass = pTech->getPass(0);

				UINT namelen;
				CPY_FROM_STREAM(namelen, Offset)                // length of pass name
				if (namelen > 0)
				{
					char *pName = new char[namelen + 1];
					ZeroMemory(pName, namelen + 1);
					memcpy(pName, Offset, namelen);                 // pass name
					Offset += namelen;
					pass->setName(pName);      // set pass name
					_AddTextureLayerName(pName);
					delete [] pName;
					pName = NULL;
				}

				ReadPass(pass, &Offset);         // ��ȡpass����
			}
			
			// ����ж��pass���ȴ���pass��ֵ
			for (UINT iPass = 1; iPass < passCount; ++iPass)
			{
				Pass *pass = pTech->createPass();

				UINT namelen;
				CPY_FROM_STREAM(namelen, Offset)   // pass ���ֳ���
				if (namelen > 0)
				{
					char *pName = new char[namelen] + 1;
					ZeroMemory(pName, namelen + 1);
					memcpy(pName, Offset, namelen);               // pass ���� 
					Offset += namelen;
					pass->setName(pName);
					_AddTextureLayerName(pName);
					delete [] pName;
					pName = NULL;
				}

				ReadPass(pass, &Offset);         // ��ȡpass����

			} // ���pass	
		} // end of TechCount == 1

		// ���ز���
		pMtrl->load();

		// _SetMaterialUVSpeed(pMtrl.get(), USpeed, VSpeed);  // ���ö�̬UV�ٶ�

	} // Material 

	DELETE_ARRAYPTR(m_pMaterailBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}	
// ���ֽ����ж�ȡPass����
void CGetV3D::ReadPass(Pass *pPass, char** OffsetPtr)
{
	char *Offset = *OffsetPtr;	

	//bool bTransparent;
	//UINT Amb_RGBA, Dif_RGBA, Spe_RGBA, sel_RGBA;
	//float shininess, bDepthCheck, bLighting, bDepthWrite;
	//BYTE ShadingMode, sBlendFactor, dBlendFactor, CullMode, PolygonMode, RejFunc, AlphaValue;
	Ogre::ColourValue thisColourValue;	

	V3D_PassDetail_Ex PassDetail;
	CPY_FROM_STREAM(PassDetail.basedetail, Offset)  // һ���Զ�ȡpass�Ĳ���
	if ((int)m_V3DFileVersion >= 50001)
	{
		CPY_FROM_STREAM(PassDetail.bUseDiffuse, Offset)  // ��ȡ�Ƿ�ʹ�ò���diffuse��ɫ
	}

	// ��ֵ��pass
	pPass->setShadingMode((Ogre::ShadeOptions)(PassDetail.basedetail.ShadingMode));
	thisColourValue.setAsRGBA(PassDetail.basedetail.Amb_RGBA);
	pPass->setAmbient(thisColourValue);
	thisColourValue.setAsRGBA(PassDetail.basedetail.Dif_RGBA);

	if (PassDetail.bUseDiffuse)  // ���ʹ�ò���diffuse
	{
		pPass->setDiffuse(thisColourValue);
	}
	else
	{
		pPass->setDiffuse(1.0f, 1.0f, 1.0f, thisColourValue.a); // ʹ�ð�ɫ
	}

	// fuxb 2008-10-30 ==>
	// thisColourValue.setAsRGBA(PassDetail.basedetail.Spe_RGBA);
	// pPass->setSpecular(ColourValue);
	thisColourValue.setAsRGBA(PassDetail.basedetail.Spe_RGBA);
	thisColourValue.r *= PassDetail.basedetail.Spe_Level / 100.0;
	thisColourValue.g *= PassDetail.basedetail.Spe_Level / 100.0;
	thisColourValue.b *= PassDetail.basedetail.Spe_Level / 100.0;
	thisColourValue.a *= PassDetail.basedetail.Spe_Level / 100.0;
	pPass->setSpecular(thisColourValue);
	// <==

	pPass->setShininess(Real(PassDetail.basedetail.shininess));
	thisColourValue.setAsRGBA(PassDetail.basedetail.sel_RGBA);
	pPass->setSelfIllumination(thisColourValue);
	pPass->setSceneBlending((const Ogre::SceneBlendFactor)(PassDetail.basedetail.sBlendFactor), (const Ogre::SceneBlendFactor)(PassDetail.basedetail.dBlendFactor));
	pPass->setCullingMode((Ogre::CullingMode)(PassDetail.basedetail.CullMode));
	pPass->setDepthCheckEnabled((PassDetail.basedetail.bDepthCheck));	
	pPass->setDepthWriteEnabled(PassDetail.basedetail.bDepthWrite);
	pPass->setLightingEnabled(PassDetail.basedetail.bLighting);
	pPass->setPolygonMode((Ogre::PolygonMode)(PassDetail.basedetail.PolygonMode));
	pPass->setAlphaRejectSettings((Ogre::CompareFunction)(PassDetail.basedetail.RejFunc), (PassDetail.basedetail.AlphaValue));

	// fuxb ==> 2008-08-31 ���뵽VGS����������
	CVGSMaterialInfo VGSMatInfo;
	VGSMatInfo.sVGSName = pPass->getParent()->getParent()->getName();
	VGSMatInfo.SpecularLevel = PassDetail.basedetail.Spe_Level; // [0, 100]
	VGSMatInfo.Emissive = PassDetail.basedetail.selfIllumination; // [0, 100]
	// VGSMatInfo.USpeed = PassDetail.basedetail.USpeed;
	// VGSMatInfo.VSpeed = PassDetail.basedetail.VSpeed;
	VGSMatInfo.bUseDiffuse = PassDetail.bUseDiffuse;  // 2008-10-29, �Ƿ�ʹ�ò��ʵ�diffuse��ɫ
	VGSMatInfo.Diffuse.setAsRGBA(PassDetail.basedetail.Dif_RGBA);
	VGSMatInfo.Specular.setAsRGBA(PassDetail.basedetail.Spe_RGBA);
	m_pVGSSceneMgr->AddVGSMaterialInfo(VGSMatInfo);   // ����VGSMaterial��Ϣ
	// <==

	*OffsetPtr = Offset;  // updata offsetptr to current positon

	// ��ȡ��ͼ��
	if (m_V3DFileVersion < 50005)
	{
		ReadPassTexture_old(pPass, OffsetPtr);
	}
	else
	{
		ReadPassTexture(pPass, OffsetPtr);
	}

	// ���û�ɫ
	if (pPass->getDiffuse().a < 1.0f || _HasOpactityTexture(pPass))
	{
		pPass->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);
	}
	else
	{
		pPass->setSceneBlending(Ogre::SceneBlendType::SBT_REPLACE);
	}	
}
// ���ֽ����ж�ȡtexture����
void CGetV3D::ReadPassTexture(Pass *pPass, char** OffsetPtr)
{	
	char *Offset = *OffsetPtr;	 
	UINT UnitStateCount;
	memcpy(&UnitStateCount, Offset, sizeof(UnitStateCount));  // get texture count
	Offset += sizeof(UnitStateCount);
	for (UINT iUnit = 0; iUnit < UnitStateCount; ++iUnit) // pass�õ���������Ϣ
	{
		TextureUnitState *pUnit = pPass->createTextureUnitState();
		UINT namelen;
		CPY_FROM_STREAM(namelen, Offset)                // length of unit name
		if (namelen > 0)
		{
			char *pName = new char[namelen + 1];
			ZeroMemory(pName, namelen + 1);
			memcpy(pName, Offset, namelen);                 // unit name
			Offset += namelen;
			pUnit->setName(pName);                          // set unit name
			_AddTextureLayerName(pName);
			delete [] pName;	
			pName = NULL;
		}

		UINT CoordSet;
		BYTE minFilter, magFilter, mipFilter, mipMap ;
		CPY_FROM_STREAM(CoordSet, Offset)                // CoordSet 
		CPY_FROM_STREAM(minFilter, Offset)               // texture filter 
		CPY_FROM_STREAM(magFilter, Offset)               // texture filter  
		CPY_FROM_STREAM(mipFilter, Offset)               // texture filter  
		CPY_FROM_STREAM(mipMap, Offset)					 // texture mipMap  

		// fuxb ��ȡÿ��texture��UV Speed
		FLOAT USpeed, VSpeed;
		CPY_FROM_STREAM(USpeed, Offset) 
		CPY_FROM_STREAM(VSpeed, Offset) 

		int texType;
		CPY_FROM_STREAM(texType, Offset)               // texture ����
		
		int duration = 0;				// ����Ƕ�̬��ͼ����¼��������

		VGS_REFLECT_TYPE reflectType;
		VGSBumpmapInfo bumpInfo;   // bump����

		if (texType == VGS_TextureType::TLT_DYNAMIC)
		{
			CPY_FROM_STREAM(duration, Offset);       
		}
		else if (texType == VGS_TextureType::TLT_REFLECT)
		{			
			char type;
			CPY_FROM_STREAM(type, Offset)		// ���䷽ʽ��ƽ�滹������
			reflectType = (VGS_REFLECT_TYPE)type;
		}
		else if (texType == VGS_TextureType::TLT_BUMPMAP)
		{
			CPY_FROM_STREAM(bumpInfo.m_Type, Offset);   // bump������
			bumpInfo.m_pTUS = pUnit;
			if (bumpInfo.m_Type == VBT_WATER)
			{
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.XScale, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.YScale, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.Radius, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.Speed, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.CenterX, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpWaterParam.CenterY, Offset);
			}
			else if (bumpInfo.m_Type == VBT_ENVIRONMENT)
			{
				CPY_FROM_STREAM(bumpInfo.m_BumpEnvParam.noise1, Offset);
				CPY_FROM_STREAM(bumpInfo.m_BumpEnvParam.noise2, Offset);
			}
			else if (bumpInfo.m_Type == VBT_NORMARLMAP)
			{
				INT index;
				CPY_FROM_STREAM(index, Offset);
				bumpInfo.m_BumpNormalParam.LightHandle = (DWORD)index;
			} 
		}
		else if (texType == VGS_TextureType::TLT_CUBEMAP)
		{
			// ...
		}

		// �����Ļ�ɫ��ʽ
		VGS_BlenderInfo ColorBlendInfo, AlphaBlendInfo;
		CPY_FROM_STREAM(ColorBlendInfo, Offset)
		CPY_FROM_STREAM(AlphaBlendInfo, Offset) 

		// textureʹ�õ�ͼƬ��Ŀ
		unsigned short FrmCount;
		CPY_FROM_STREAM(FrmCount, Offset)

		vector <Ogre::String> vsFrameTexture;  // texture�õ�������֡ͼ��
		for (unsigned short iFrm = 0; iFrm < FrmCount; ++iFrm)
		{
			CPY_FROM_STREAM(namelen, Offset)                // length of texture name 

			if (namelen > 0)  
			{
				char* pName = new char[namelen + 1];
				ZeroMemory(pName, namelen + 1);
				memcpy(pName, Offset, namelen);                 // texture name, ������·��
				Offset += namelen;
				vsFrameTexture.push_back(String(pName));		// ��ʱ����

				delete [] pName;
				pName = NULL;
			}
		} // for iFrm

		// ��������
		Ogre::String sMatName = pPass->getParent()->getParent()->getName();		

		// �����cubicmap����Ҫ��pUnit�������н�����ͼƬ����
		if (texType == VGS_TextureType::TLT_CUBEMAP) // cubic map
		{
			if (FrmCount == 6)
			{
				for (int idx = 0; idx < FrmCount; ++idx)
				{
					// ������ͼ�Ͳ��ʵ�ӳ��
					m_pVGSSceneMgr->NotifyTextureUpdate();  // ֪ͨ�����е�texture�б����˱仯
					vsFrameTexture[idx] = AddPathtoTextureFile(vsFrameTexture[idx].c_str());  // ����ļ�·��
				}
				_SetCubicTextureImages((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, vsFrameTexture);  // ����cubicmap
			}
		}
		else  // ��cubic texture, ֱ�Ӷ�ȡ����ͼƬ
		{
			// ����ͼ���ļ�
			int FrmCount = vsFrameTexture.size();
			for (int i = 0; i < FrmCount; ++i)
			{
				// ������ͼ�Ͳ��ʵ�ӳ��
				m_pVGSSceneMgr->NotifyTextureUpdate();  // ֪ͨ�����е�texture�б����˱仯
				Ogre::String sTextureFile = AddPathtoTextureFile(vsFrameTexture[i].c_str());
				pUnit->addFrameTextureName(sTextureFile); // ��������textureĿ¼��
			}

			// ����texture
			if (texType == VGS_TextureType::TLT_DYNAMIC)
			{
				SetDynamicTextureDuration((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, duration);  
			}
			else if (texType == VGS_TextureType::TLT_REFLECT)
			{
				if ((VGS_REFLECT_TYPE)reflectType != VGS_REFLECT_TYPE::VRT_INVALID)
				{
					_SetTextureAsReflect(pUnit, (VGS_REFLECT_TYPE)reflectType);
				}
			}
			else if (texType == VGS_TextureType::TLT_OPACITY)
			{
				SetTextureAsOpacity((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, TRUE); 
			}
			else if (texType == VGS_TextureType::TLT_BUMPMAP)
			{
				//if (bumpInfo.m_Type == VGS_BumpType::VBT_WATER)
				//{
				//	pUnit->setTextureNameAlias(g_BumpWaterNamePrefix + pUnit->getName());  // ���ñ����Լ�¼��bump
				//}
				//else if (bumpInfo.m_Type == VGS_BumpType::VBT_ENVIRONMENT)
				//{
				//	pUnit->setTextureNameAlias(g_BumpEnvNamePrefix + pUnit->getName());  // ���ñ����Լ�¼��bump
				//}
				//else if (bumpInfo.m_Type == VGS_BumpType::VBT_NORMARLMAP)
				//{
				//	pUnit->setTextureNameAlias(g_BumpNormalNamePrefix + pUnit->getName());  // ���ñ����Լ�¼��bump
				//}
				
				m_pVGSSceneMgr->UpdateBumpmapParam(pUnit, bumpInfo);
			}
		}

		if (CoordSet < 0 || CoordSet > 3)
		{
			CoordSet = 0;
		}

		pUnit->setTextureCoordSet(CoordSet);  // ����textureʹ�õ�UV������
		pUnit->setNumMipmaps(mipMap);         // mipMap

		pUnit->setTextureFiltering((Ogre::FilterOptions)minFilter, (Ogre::FilterOptions)magFilter, (Ogre::FilterOptions)mipFilter); // ����texture filter
			
		// ��ɫ��ɫ��ʽ
		pUnit->setColourOperationEx(_GetOgreBlendOperation(ColorBlendInfo.option), Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
									ColourValue::White, ColourValue::White,
									ColorBlendInfo.param.BlenderOP_alpha);
		pUnit->setColourOpMultipassFallback(SBF_DEST_COLOUR, SBF_ZERO);

		// alphaͨ���Ļ�ɫ��ʽ
		pUnit->setAlphaOperation(_GetOgreBlendOperation(AlphaBlendInfo.option), Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
									1.0f, 1.0f,
									AlphaBlendInfo.param.BlenderOP_alpha);
		 
		_SetTextureUVSpeed(pUnit, USpeed, VSpeed);  // ����UV Speed

	} // for iUnit

	*OffsetPtr = Offset;  // updata offsetptr to current positon
}
// ���ֽ����ж�ȡtexture����, 50004����֮ǰ�汾
void CGetV3D::ReadPassTexture_old(Pass *pPass, char** OffsetPtr)
{
	char *Offset = *OffsetPtr;	 
	UINT UnitStateCount;
	memcpy(&UnitStateCount, Offset, sizeof(UnitStateCount));  // get texture count
	Offset += sizeof(UnitStateCount);
	for (UINT iUnit = 0; iUnit < UnitStateCount; ++iUnit) // pass�õ���������Ϣ
	{
		TextureUnitState *pUnit = pPass->createTextureUnitState();
		UINT namelen;
		CPY_FROM_STREAM(namelen, Offset)                // length of unit name
		if (namelen > 0)
		{
			char *pName = new char[namelen + 1];
			ZeroMemory(pName, namelen + 1);
			memcpy(pName, Offset, namelen);                 // unit name
			Offset += namelen;
			pUnit->setName(pName);                          // set unit name
			_AddTextureLayerName(pName);
			delete [] pName;	
			pName = NULL;
		}

		UINT CoordSet;
		BYTE minFilter, magFilter, mipFilter, mipMap ;
		CPY_FROM_STREAM(CoordSet, Offset)                // CoordSet 
		CPY_FROM_STREAM(minFilter, Offset)               // texture filter 
		CPY_FROM_STREAM(magFilter, Offset)               // texture filter  
		CPY_FROM_STREAM(mipFilter, Offset)               // texture filter  
		CPY_FROM_STREAM(mipMap, Offset)					 // texture mipMap  

		int texType;
		CPY_FROM_STREAM(texType, Offset)               // texture ����
		
		int duration = 0;				// ����Ƕ�̬��ͼ����¼��������			
		bool bOpacity = false;                  // ��¼�Ƿ���͸����ͼ
		char reflectType;               // ��������
		if (texType == V3D_TextureType_Old::VTT_DYNAMIC)
		{
			CPY_FROM_STREAM(duration, Offset);       
		}
		else if (texType == V3D_TextureType_Old::VTT_BASAL)
		{
			CPY_FROM_STREAM(reflectType, Offset)		// �Ƿ�����������ͼ
			CPY_FROM_STREAM(bOpacity, Offset)			// �Ƿ�����͸����ͼ	

			// 50001����ǰ�İ汾������boolֵ�����Ƿ��Ƿ�����ͼ���°汾��char��¼�������ͣ�-1��ʾ���Ƿ�����ͼ
			if (m_V3DFileVersion < 50002 && reflectType == 0)
			{
				reflectType = -1;  // ���Ƿ�����ͼ
			}
		}
		else if (texType == V3D_TextureType_Old::VTT_BUMP)
		{
			// nothing
		}

		// �����Ļ�ɫ��ʽ
		VGS_BlenderInfo ColorBlendInfo, AlphaBlendInfo;
		CPY_FROM_STREAM(ColorBlendInfo, Offset)
		CPY_FROM_STREAM(AlphaBlendInfo, Offset) 

		// textureʹ�õ�ͼƬ��Ŀ
		unsigned short FrmCount;
		CPY_FROM_STREAM(FrmCount, Offset)

		vector <Ogre::String> vsFrameTexture;  // texture�õ�������֡ͼ��
		for (unsigned short iFrm = 0; iFrm < FrmCount; ++iFrm)
		{
			CPY_FROM_STREAM(namelen, Offset)                // length of texture name 

			if (namelen > 0)  
			{
				char* pName = new char[namelen + 1];
				ZeroMemory(pName, namelen + 1);
				memcpy(pName, Offset, namelen);                 // texture name, ������·��
				Offset += namelen;
				vsFrameTexture.push_back(String(pName));  // ��ʱ����

				delete [] pName;
				pName = NULL;
			}
		} // for iFrm

		// ��������
		Ogre::String sMatName = pPass->getParent()->getParent()->getName();		

		// �����cubicmap����Ҫ��pUnit�������н�����ͼƬ����
		if (texType == V3D_TextureType_Old::VTT_CUBIC) // cubic map
		{
			// ����50002�汾�Ĵ�texturename�н����ļ�����
			if (m_V3DFileVersion < 50002)
			{
				// �����ַ�������ȡ�ļ��б�
				Ogre::String sFilelist = pUnit->getName();
				_GetFileListFormString(sFilelist, vsFrameTexture);
				FrmCount = vsFrameTexture.size();
			} 
			
			if (FrmCount == 6)
			{
				for (int idx = 0; idx < FrmCount; ++idx)
				{
					// ������ͼ�Ͳ��ʵ�ӳ��
					m_pVGSSceneMgr->NotifyTextureUpdate();  // ֪ͨ�����е�texture�б����˱仯
					vsFrameTexture[idx] = AddPathtoTextureFile(vsFrameTexture[idx].c_str());  // ����ļ�·��
				}
				_SetCubicTextureImages((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, vsFrameTexture);  // ����cubicmap
			}
		}
		else  // ��cubic texture, ֱ�Ӷ�ȡ����ͼƬ
		{
			// ����ͼ���ļ�
			int FrmCount = vsFrameTexture.size();
			for (int i = 0; i < FrmCount; ++i)
			{
				// ������ͼ�Ͳ��ʵ�ӳ��
				m_pVGSSceneMgr->NotifyTextureUpdate();  // ֪ͨ�����е�texture�б����˱仯
				Ogre::String sTextureFile = AddPathtoTextureFile(vsFrameTexture[i].c_str());
				pUnit->addFrameTextureName(sTextureFile); // ��������textureĿ¼��

			}

			// ����texture
			if (texType == V3D_TextureType_Old::VTT_DYNAMIC)
			{
				SetDynamicTextureDuration((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, duration);  
			}
			else if (texType == V3D_TextureType_Old::VTT_BASAL)
			{
				if ((VGS_REFLECT_TYPE)reflectType != VGS_REFLECT_TYPE::VRT_INVALID)
				{
					_SetTextureAsReflect(pUnit, (VGS_REFLECT_TYPE)reflectType);
				}
				if (bOpacity)
				{
					SetTextureAsOpacity((DWORD)m_pVGSSceneMgr, (DWORD)pUnit, TRUE); 
				}
			}
			else if (texType == V3D_TextureType_Old::VTT_BUMP)
			{
				pUnit->setTextureNameAlias(g_BumpEnvNamePrefix + pUnit->getName());  // ���ñ����Լ�¼��bump
			}
		}

		if (CoordSet < 0 || CoordSet > 3)
		{
			CoordSet = 0;
		}

		pUnit->setTextureCoordSet(CoordSet);  // ����textureʹ�õ�UV������
		pUnit->setNumMipmaps(mipMap);         // mipMap

		pUnit->setTextureFiltering((Ogre::FilterOptions)minFilter, (Ogre::FilterOptions)magFilter, (Ogre::FilterOptions)mipFilter); // ����texture filter
			
		// ��ɫ��ɫ��ʽ
		pUnit->setColourOperationEx(_GetOgreBlendOperation(ColorBlendInfo.option), Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
									ColourValue::White, ColourValue::White,
									ColorBlendInfo.param.BlenderOP_alpha);
		pUnit->setColourOpMultipassFallback(SBF_DEST_COLOUR, SBF_ZERO);

		// alphaͨ���Ļ�ɫ��ʽ
		pUnit->setAlphaOperation(_GetOgreBlendOperation(AlphaBlendInfo.option), Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT,
									1.0f, 1.0f,
									AlphaBlendInfo.param.BlenderOP_alpha);
		 
		
	} // for iUnit

	*OffsetPtr = Offset;  // updata offsetptr to current positon
}

// ��ȡģ������
DWORD CGetV3D::ReadEnityData()
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
	
	// ��ȡÿ��mesh
	for (UINT idx = 0; idx < EntityCount; ++idx)
	{
		int namelen = 0;
		char sName[256];
		ZeroMemory(sName, 256);
		CPY_FROM_STREAM(namelen, Offset)      // entity����
		
		memcpy(sName, Offset, namelen);       // ��ȡ����
		Offset += namelen;

		int meshIndex = -1;
		CPY_FROM_STREAM(meshIndex, Offset)      // mesh����

		VGS_EntityInfo modelInfo;
		if (m_V3DFileVersion < 50003)  // 50001�汾�����V3D�����⣬������Ҫ��ȡ52���ֽ�
		{
			char tmp[52];
			memcpy(tmp, Offset, sizeof(tmp));
			// m_Offset += sizeof(tmp);
			Offset += sizeof(tmp);
			
			// ����Model����
			modelInfo.isAutoNormalise = true;			
		}
		else
		{
			CPY_FROM_STREAM(modelInfo.isAutoNormalise, Offset);
			CPY_FROM_STREAM(modelInfo.isBillboard, Offset);
			CPY_FROM_STREAM(modelInfo.isClickable, Offset);
			CPY_FROM_STREAM(modelInfo.isCollision, Offset);
			CPY_FROM_STREAM(modelInfo.isGround, Offset);
			CPY_FROM_STREAM(modelInfo.upAxis, Offset);
			CPY_FROM_STREAM(modelInfo.dirctionAxis, Offset);
			CPY_FROM_STREAM(modelInfo.isReflection, Offset)      // �Ƿ�������ģ��

			if (modelInfo.isReflection)
			{
				UINT reflectionCount = 0;
				CPY_FROM_STREAM(reflectionCount, Offset)      // �������ģ����Ŀ
				for (int iModel = 0; iModel < reflectionCount; ++iModel)
				{
					int ModelIndex;
					CPY_FROM_STREAM(ModelIndex, Offset)      // �������ģ����Ŀ
					modelInfo.vReflectionList.push_back(ModelIndex);
				}
			} // if isReflection
			CPY_FROM_STREAM(modelInfo.isRefraction, Offset)      // �Ƿ���������ģ��
			if (modelInfo.isReflection)
			{
				UINT refractionCount = 0;
				CPY_FROM_STREAM(refractionCount, Offset)      // �������ģ����Ŀ
				for (int iModel = 0; iModel < refractionCount; ++iModel)
				{
					int ModelIndex;
					CPY_FROM_STREAM(ModelIndex, Offset)      // �������ģ����Ŀ
					modelInfo.vRefractionList.push_back(ModelIndex);
				}
			} // if isReflection
		
		} // m_V3DFileVersion

		if (meshIndex >= 0 && meshIndex < m_vMesh.size())
		{
			//����ʵ��
			Entity* pEntity = m_pVGSSceneMgr->mSceneMgr->createEntity(Ogre::String(sName), m_vMesh[meshIndex]->getName());
			// pEntity->setNormaliseNormals(true);
			pEntity->_initialise();

			//����VGS����������
			m_pVGSSceneMgr->AddEntityNameMap(Ogre::String(sName), Ogre::String(sName));
			m_pVGSSceneMgr->SetEntityInfo((DWORD)pEntity, modelInfo);                      // ����

			m_vEntityList.push_back((DWORD)pEntity);   // ����Entity�б�������ܻ�ʹ��
		}
	} // for

	DELETE_ARRAYPTR(m_pEntityBuf);  // ɾ���ֽ���

	// ��ģ�ͷ�������ģ�͵���������Ϊģ�͵ľ��
	// �Ժ���


	return ERR_COMMON_OK;
}

// ��ȡģ����Դ����
DWORD CGetV3D::ReadMeshData()
{
	if (!m_pFile)
	{
		// throw Exception::exception("���ļ���������(03)���޷���ȡ�ļ�");
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pMeshBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	m_vMesh.clear();   // ���Mesh�б�

	// ��ȡmesh��Ϣ
	char* Offset = m_pMeshBuf;              // �ֽ��������ݿ�ʼ��λ��
	DWORD MeshCount;                        // mesh��Ŀ
	CPY_FROM_STREAM(MeshCount, Offset)      // ��ȡmesh��Ŀ
	
	// ��ȡÿ��mesh
	for (UINT iMesh = 0; iMesh < MeshCount; ++iMesh)
	{	
		UINT NameLen;
		CPY_FROM_STREAM(NameLen, Offset)

		MeshPtr pMesh;
		if (NameLen > 0)
		{
			char *sName = new char[NameLen + 1];
			ZeroMemory(sName, NameLen + 1);
			memcpy(sName, Offset, NameLen);
			Offset += NameLen;					

			//����Mesh����
			try
			{
				int idx = 0;
				Ogre::String sNewName = Ogre::String(sName);
				while (MeshManager::getSingleton().getByName(sNewName).get() != NULL)
				{					
					char tmp[10];
					sNewName = Ogre::String(sName) + Ogre::String(itoa(idx++, tmp, 10));
				}
				pMesh = MeshManager::getSingleton().createManual(sNewName, /*ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME*/m_pVGSSceneMgr->ogreResGroupName);
			}
			catch (Ogre::Exception &e)
			{
				// MessageBox(NULL, LPCSTR("�������Ķ��󣬿��ܻᵼ�����⣡"), LPCSTR("����"), 0);
				MessageBox(NULL, LPCSTR(e.getDescription().c_str()), LPCSTR("����"), 0);
				return ERR_SCENE_CREATE;
			}
			DELETE_ARRAYPTR(sName)

			m_vMesh.push_back(pMesh.get());  // ���浽Mesh�б�����EntityʱҪ��
		}
		else
		{
			//����Mesh����
			// pMesh = MeshManager::getSingleton().createManual(sName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			MessageBox(NULL, LPCSTR("������û��Ϊmeshָ�����ƣ����ܻᵼ�����⣡"), LPCSTR("����"), 0);
			continue;
		}

		//��Mesh����䶥���������
		//ʹ�õ�һ�Ĺ������������㻺���Ӧ���ڴ�ָ��
		pMesh->sharedVertexData = new VertexData();
		VertexData* pVertexData = pMesh->sharedVertexData;
		
		//���ö�������
		VertexDeclaration* pVertexDecl = pVertexData->vertexDeclaration;

		UINT ElementCount;
		CPY_FROM_STREAM(ElementCount, Offset)  // elment��Ŀ

		// ��������Element����, Element�д洢���Ƕ���ĸ�ʽ˵��
		for (UINT iElement = 0; iElement < ElementCount; ++iElement) 
		{
			unsigned short source;
			UINT offsetIdx;
			unsigned short type, Semantic, index;
			CPY_FROM_STREAM(source, Offset)
			CPY_FROM_STREAM(offsetIdx, Offset)
			CPY_FROM_STREAM(type, Offset)
			CPY_FROM_STREAM(Semantic, Offset)
			CPY_FROM_STREAM(index, Offset)
			pVertexDecl->addElement(source, offsetIdx, (Ogre::VertexElementType)type, (Ogre::VertexElementSemantic)Semantic, index);
		}

		UINT vertexCount;
		CPY_FROM_STREAM(vertexCount, Offset)         // ������Ŀ
		UINT StartIdx;
		CPY_FROM_STREAM(StartIdx, Offset)            // �������ݿ�ʼλ�ã����ÿ��mesh���Լ����涥�㻺�����ݵĻ�����ô���ֵ�͵���0
		pVertexData->vertexCount = vertexCount;
		pVertexData->vertexStart = StartIdx;

		//���䶥�㻺��
		INT vertexSize = pVertexDecl->getVertexSize(0);  // �õ��洢һ���������ݵ��ֽ���Ŀ
		HardwareVertexBufferSharedPtr vBuf;
		//if (pVGSMesh->bindingSkeletonIndex == -1) // ��ʱ��û�ж�ȡ��������
		{
			vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexSize,
				   pVertexData->vertexCount, Ogre::HardwareBuffer::Usage::HBU_STATIC_WRITE_ONLY, false);
		}
		/*else
		{
			vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexSize,
				   pVertexData->vertexCount, HardwareBuffer::Usage::HBU_DYNAMIC_WRITE_ONLY, false);
		}*/
		VertexBufferBinding* pBinding = pVertexData->vertexBufferBinding;
		pBinding->setBinding(0, vBuf);

		//��䶥������
		UINT VertexBytes = vertexSize * pVertexData->vertexCount;
		char *pVertexBuf = static_cast<char*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		memcpy(pVertexBuf, Offset, VertexBytes);  // һ���Զ�ȡ���ж�������
		Offset += VertexBytes;

		vBuf->unlock();

		unsigned short SubMeshCount;
		CPY_FROM_STREAM(SubMeshCount, Offset)    // sub mesh count
		for (UINT iSubMesh = 0; iSubMesh < SubMeshCount; ++iSubMesh)
		{
			//����SubMesh����
			SubMesh* pSubMesh = pMesh->createSubMesh();
			//�����������壬�������
			pSubMesh->useSharedVertices = true;			
			
			UINT NameLen;
			CPY_FROM_STREAM(NameLen, Offset)    // submeshʹ�õĲ������Ƴ���
			if (NameLen > 0)  // submesh ����û������
			{
				char *sName = new char[NameLen + 1];
				ZeroMemory(sName, NameLen + 1);
				memcpy(sName, Offset, NameLen);     // submeshʹ�õĲ�������	
				pSubMesh->setMaterialName(String(sName));   // ���ò���
				Offset += NameLen;
				DELETE_ARRAYPTR(sName)
			}

			UINT IndexCount, IndexStart;
			CPY_FROM_STREAM(IndexCount, Offset)  // submesh��������Ŀ
			CPY_FROM_STREAM(IndexStart, Offset)  // �����������еĿ�ʼλ�ã����ÿ��submesh�Լ�������������Ļ������ֵ�͵���0
			pSubMesh->indexData->indexCount = IndexCount;
			pSubMesh->indexData->indexStart = IndexStart;

			if (IndexCount > 65536)  // ���ĸ��ֽڵĻ���
			{
				pSubMesh->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IndexType::IT_32BIT,
				                                   pSubMesh->indexData->indexCount, Ogre::HardwareBuffer::Usage::HBU_STATIC_WRITE_ONLY, false);
		
				HardwareIndexBufferSharedPtr iBuf = pSubMesh->indexData->indexBuffer;

				//�������������������
				char *pIndexData = static_cast<char*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));
				
				// һ���Զ�ȡ��������
				memcpy(pIndexData, Offset, 4 * IndexCount);
				Offset += 4 * IndexCount;

				iBuf->unlock();
			}
			else  // �������ֽڵĻ���
			{
				pSubMesh->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IndexType::IT_16BIT,
				                                   pSubMesh->indexData->indexCount, Ogre::HardwareBuffer::Usage::HBU_STATIC_WRITE_ONLY, false);
		
				HardwareIndexBufferSharedPtr iBuf = pSubMesh->indexData->indexBuffer;

				//�������������������
				char *pIndexData = static_cast<char*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));
				
				// һ���Զ�ȡ��������
				memcpy(pIndexData, Offset, 2 * IndexCount);
				Offset += 2 * IndexCount;

				iBuf->unlock();
			}			
		}

		// ��Χ��
		Vector3 NearConner, FarConner;
		CPY_VECTOR3_FROM_STRRAM(NearConner, Offset)
		CPY_VECTOR3_FROM_STRRAM(FarConner, Offset)
		Ogre::AxisAlignedBox AABB;
		AABB.setExtents(NearConner, FarConner);
		pMesh->_setBounds(AABB, false);

		// ��Χ��
		float radius;
		CPY_FROM_STREAM(radius, Offset);
		pMesh->_setBoundingSphereRadius((Ogre::Real)radius);

		pMesh->load();  // ����mesh

		// ��������
		// ...

		////����ʵ��
		//Entity* pEntity = m_pVGSSceneMgr->mSceneMgr->createEntity(pMesh->getName(), pMesh->getName());
		//pEntity->setNormaliseNormals(true);
		//pEntity->_initialise();

		// ������������
		
		
		//����VGS����������
		m_pVGSSceneMgr->AddMeshNameMap(pMesh->getName(), pMesh->getName());

		//����VGS����������
		//m_pVGSSceneMgr->AddEntityNameMap(pEntity->getName(), pEntity->getName());

	} // mesh

	DELETE_ARRAYPTR(m_pMeshBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}

//��������Ĳ�������
void CGetV3D::CreateCameraParamAnimation(Ogre::Camera* pCamera, VGS_V3D_Camera* pVGSCamera, int frameCnt)
{
	//�ؼ�֡����
	CameraParamAnimation* pCameraParamAni = new CameraParamAnimation(pCamera);

	pCameraParamAni->mCamera = pCamera; // fuxb 2008-4-18
	pCameraParamAni->frameCnt = frameCnt;
	pCameraParamAni->cameraInfoList = new VGS_V3D_CAMINFO[frameCnt];
	for (INT frameIndex = 0; frameIndex < frameCnt; frameIndex++)
	{
		pCameraParamAni->cameraInfoList[frameIndex] = pVGSCamera->v3dAnimationInfo[frameIndex];
	}

	pCameraParamAni->setLength(Real(frameCnt)/gkfrPlaySpeed);
	
	//�������������������
	// cameraParamAnimationList.push_back(pCameraParamAni);
	m_pVGSSceneMgr->GetAnimationManager()->AddCameraAnimation(pCameraParamAni);
}

//��ȡ������ݲ��������������
DWORD CGetV3D::ReadCameraData()
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

	//�����������
	int cameraCnt = 0;                         
	CPY_FROM_STREAM(cameraCnt, Offset)       

	//��������������
	for (int i = 0; i< cameraCnt; i++)
	{
		VGS_V3D_Camera VGSCam;
		ZeroMemory((void*)&VGSCam, sizeof(VGSCam));

		//�������
		UINT len = 0;
		CPY_FROM_STREAM(len, Offset);
		char *pName = new char[len + 1];
		ZeroMemory(pName, len + 1);
		strncpy(pName, Offset, len); // '\0' excluded
		Offset += len;
		VGSCam.name = std::string(pName);
		VGSCam.nameLen = VGSCam.name.length();  // include '\0'
		delete pName;
		pName = NULL;

		//��Ⱦͬ������
		//EnterCriticalSection(m_render_cs);
		//�������
		Ogre::Camera* pCamera = m_pVGSSceneMgr->CreateCamera(Ogre::String(VGSCam.name.c_str()), false);
		//��Ⱦͬ������
		//LeaveCriticalSection(m_render_cs);

		//����Ļ�������
		CPY_FROM_STREAM(VGSCam.v3dBaseInfo, Offset);

		//�����ͶӰ����
		// CPY_FROM_STREAM(VGSCam.isOrtho, Offset);
		//�Ƿ������
		if (VGSCam.v3dBaseInfo.isOrtho)
			pCamera->setProjectionType(PT_ORTHOGRAPHIC);
		else
			pCamera->setProjectionType(PT_PERSPECTIVE);

		//pCamera->setFixedYawAxis(false);  // fuxb 2008-08-29


		//Fov
		pCamera->setFOVy(Radian(VGSCam.v3dBaseInfo.fov));

		//near far clip
		pCamera->setNearClipDistance(VGSCam.v3dBaseInfo.nearClipDis);
		pCamera->setFarClipDistance(VGSCam.v3dBaseInfo.farClipDis);

		// ���λ�ú���Ԫ��
		pCamera->setPosition(Vector3(VGSCam.v3dBaseInfo.position.x, VGSCam.v3dBaseInfo.position.y, VGSCam.v3dBaseInfo.position.z));
		pCamera->setOrientation(Quaternion(VGSCam.v3dBaseInfo.quat.w, VGSCam.v3dBaseInfo.quat.x, VGSCam.v3dBaseInfo.quat.y, VGSCam.v3dBaseInfo.quat.z));

		m_vpCamera.push_back(pCamera);   // ����������������ܻ�ʹ��

		//�����������֡��
		int frameCnt;
		CPY_FROM_STREAM(frameCnt, Offset);
		if (frameCnt == 0) continue;

		//д�������������
		for (int frameIndex = 0; frameIndex < frameCnt; frameIndex++)
		{
			VGS_V3D_CAMINFO camInfo;
			//��������Ķ�����Ϣ
			CPY_FROM_STREAM(camInfo, Offset);
			//�������
			VGSCam.v3dAnimationInfo.push_back(camInfo);
		}

		//��������Ĳ�������
		CreateCameraParamAnimation(pCamera, &VGSCam, frameCnt);		
	}

	DELETE_ARRAYPTR(m_pCameraBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}


DWORD CGetV3D::ReadLightData()
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
	
	//����ƹ��ȡ����
	for (int i = 0; i < LightCount; i++)
	{
		//����ƹ������
		VGS_Light vgsLight;
		ZeroMemory((void*)&vgsLight, sizeof(vgsLight)); 

		UINT len = 0;
		CPY_FROM_STREAM(len, Offset);
		vgsLight.name.reserve(len + 1);
		strncpy((char*)vgsLight.name.c_str(), Offset, len); // ������'\0'
		//vgsLight.name[len] = '\0';
		vgsLight.strLen = len + 1; // ����'\0'
		Offset += len;

		//�����ƹ�
		Light* pLight = m_pVGSSceneMgr->CreateLight(Ogre::String(vgsLight.name.c_str()), false);

		//����ƹ�Ļ�������
		CPY_FROM_STREAM(vgsLight.type, Offset);
		//�ƹ�����
		switch (vgsLight.type)
		{
		case LIGHTTYPE::OMINI:
			{
				pLight->setType(Ogre::Light::LT_POINT); // ���Դ

				VGS_V3D_OMNI omni;
				//������Դ��Ϣ
				CPY_FROM_STREAM(omni, Offset);
				
				//��ɫ
				Ogre::ColourValue color;
				color.r = ((omni.color >> 24) & 0xff)/255.0f;
				color.g = ((omni.color >> 16) & 0xff)/255.0f;
				color.b = ((omni.color >> 8) & 0xff)/255.0f;
				color.a = ((omni.color) & 0xff)/255.0f;
				
				//ǿ��
				color *= omni.multiply;
				pLight->setDiffuseColour(color);

				// ����ƹ����������������
				// pLight->setPowerScale(omni.multiply);
				CVGSLightInfo VGSLightInfo;
				VGSLightInfo.m_sVGSName = Ogre::String(vgsLight.name.c_str());
				VGSLightInfo.m_Multiply = omni.multiply;
				m_pVGSSceneMgr->AddVGSLightInfo(VGSLightInfo);

				//�߹�
				pLight->setSpecularColour(color);
				
				//˥��
				pLight->setAttenuation(omni.attenuationRange, omni.attenuation0, omni.attenuation1, omni.attenuation2);

				pLight->setPosition(Vector3(omni.position.x, omni.position.y, omni.position.z));
			}
			break;

		case LIGHTTYPE::DIRECTIONLIGHT:
			{
				pLight->setType(Ogre::Light::LT_DIRECTIONAL); // �����

				VGS_V3D_DIRECTIONAL directionLight;
				//���뷽���Դ��Ϣ
				CPY_FROM_STREAM(directionLight, Offset);

				//��ɫ
				Ogre::ColourValue color;
				color.r = ((directionLight.color>>24) & 0xff)/255.0f;
				color.g = ((directionLight.color>>16) & 0xff)/255.0f;
				color.b = ((directionLight.color>>8) & 0xff)/255.0f;
				color.a = (directionLight.color & 0xff) / 255.0f;
				color *= directionLight.multiply;

				//�ƹ���ɫ
				pLight->setDiffuseColour(color);

				// ����ƹ����������������
				// pLight->setPowerScale(omni.multiply);
				CVGSLightInfo VGSLightInfo;
				VGSLightInfo.m_sVGSName = Ogre::String(vgsLight.name.c_str());
				VGSLightInfo.m_Multiply = directionLight.multiply;
				m_pVGSSceneMgr->AddVGSLightInfo(VGSLightInfo);

				//�߹�
				pLight->setSpecularColour(color);
				
				//����
				Ogre::Vector3 direction;
				direction.x = directionLight.direction.x;
				direction.y = directionLight.direction.y;
				direction.z = directionLight.direction.z;
				pLight->setDirection(direction);

				//˥��
				pLight->setAttenuation(directionLight.attenuationRange, directionLight.attenuation0, directionLight.attenuation1, directionLight.attenuation2);

				// λ��
				//pLight->setPosition(Vector3(directionLight.position.x, directionLight.position.y, directionLight.position.z));
			}
			break;

		case LIGHTTYPE::SPOTLIGHT:
			{
				pLight->setType(Ogre::Light::LT_SPOTLIGHT); // �۹��

				VGS_V3D_SPOTLIGHT spotLight;
				//����۹����Ϣ
				CPY_FROM_STREAM(spotLight, Offset);
				
				//��ɫ
				Ogre::ColourValue color;
				color.r = ((spotLight.color>>24) & 0xff)/255.0f;
				color.g = ((spotLight.color>>16) & 0xff)/255.0f;
				color.b = ((spotLight.color>>8) & 0xff)/255.0f;
				color.a = (spotLight.color & 0xff) / 255.0f;
				color *= spotLight.multiply;

				//�ƹ���ɫ
				pLight->setDiffuseColour(color);

				// ����ƹ����������������
				CVGSLightInfo VGSLightInfo;
				VGSLightInfo.m_sVGSName = Ogre::String(vgsLight.name.c_str());
				VGSLightInfo.m_Multiply = spotLight.multiply;
				m_pVGSSceneMgr->AddVGSLightInfo(VGSLightInfo);

				//�߹�
				pLight->setSpecularColour(color);
				
				//����
				Ogre::Vector3 direction;
				direction.x = spotLight.direction.x;
				direction.y = spotLight.direction.y;
				direction.z = spotLight.direction.z;
				pLight->setDirection(direction);

				//˥��
				pLight->setAttenuation(spotLight.attenuationRange, spotLight.attenuation0, spotLight.attenuation1, spotLight.attenuation2);

				//�Ž�
				pLight->setSpotlightInnerAngle(Radian(spotLight.innerAngle));
				pLight->setSpotlightOuterAngle(Radian(spotLight.outerAngle));
				pLight->setSpotlightFalloff(spotLight.fallOff);

				// λ��
				pLight->setPosition(Vector3(spotLight.position.x, spotLight.position.y, spotLight.position.z));

			}
			break;
		}

		m_vpLight.push_back(pLight);  // �����Լ����صĵƹ�

		//�ƹ⶯��
		//���붯��֡��
		int frameCnt = 0;
		CPY_FROM_STREAM(frameCnt, Offset);
		if (frameCnt == 0) continue;

		//�����ƹ��������-----------------------------------------------------------------
		CreateLightParamAnimation(pLight, &vgsLight, frameCnt, &Offset);
	}

	DELETE_ARRAYPTR(m_pLightBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}

//�����ƹ��������
void CGetV3D::CreateLightParamAnimation(Light* pLight, VGS_Light* pVGSLight, int frameCnt, char** Offset)
{
	//����������������
	LightParamAnimation* pLightParamAni = new LightParamAnimation(pLight);
	pLightParamAni->mLight = pLight;
	pLightParamAni->frameCnt = frameCnt;
	//д��ÿ֡������
	switch (pVGSLight->type)
	{
	case LIGHTTYPE::OMINI:
		{
			pLightParamAni->omniInfoList = new VGS_V3D_OMNI[frameCnt];
			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
				CPY_FROM_STREAM(pLightParamAni->omniInfoList[frameIndex], *Offset);
		}
		break;

	case LIGHTTYPE::DIRECTIONLIGHT:
		{	
			pLightParamAni->directionLightInfoList = new VGS_V3D_DIRECTIONAL[frameCnt];
			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
				CPY_FROM_STREAM(pLightParamAni->directionLightInfoList[frameIndex], *Offset);
		}
		break;

	case LIGHTTYPE::SPOTLIGHT:
		{
			for (int frameIndex = 0; frameIndex< frameCnt; frameIndex++)
				CPY_FROM_STREAM(pLightParamAni->spotLightInfoList[frameIndex], *Offset);
		}
		break;
	}
	
	pLightParamAni->setLength(Real(frameCnt)/gkfrPlaySpeed);
	
	//����ƹ������������
	// lightParamAnimationList.push_back(pLightParamAni);
	m_pVGSSceneMgr->GetAnimationManager()->AddLightAnimation(pLightParamAni);
}
/*
//��ȡ������ݲ��������������
DWORD CGetV3D::ReadCameraData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pCameraBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// ��ȡmesh��Ϣ
	char* Offset = m_pCameraBuf;            // �ֽ��������ݿ�ʼ��λ��
	DWORD CamCount;                         // Camera��Ŀ
	CPY_FROM_STREAM(CamCount, Offset)       // ��ȡCamera��Ŀ
	
	// ��ȡÿ��Camera
	for (UINT iCam = 0; iCam < CamCount; ++iCam)
	{
		UINT NameLen;
		CPY_FROM_STREAM(NameLen, Offset)
		Camera* pCamera = NULL;
		if (NameLen > 0)
		{
			char *sName = new char[NameLen + 1];
			ZeroMemory(sName, NameLen + 1);
			memcpy(sName, Offset, NameLen);
			Offset += NameLen;

			//�������
			pCamera = m_pVGSSceneMgr->CreateCamera(String(sName), false);
			DELETE_ARRAYPTR(sName)
		}
		else
		{
			MessageBox(NULL, LPCSTR("������û��Ϊ���ָ�����ƣ����ܻᵼ�����⣡"), LPCSTR("����"), 0);
			continue;
		}

		V3D_CameraDetail CamDetail;
		CPY_FROM_STREAM(CamDetail, Offset)  // һ���Զ�ȡ�������

		m_vCameraDetail.push_back(CamDetail);  // ��ʱ���棬�ȴ�������ͼ��ʱ���õ�


		//��������Ĳ�������
		// CreateCameraParamAnimation(pCamera, pVGSCamera);

		//����VGS����������
		m_pVGSSceneMgr->AddCamNameMap(pCamera->getName(), pCamera->getName());

	}

	DELETE_ARRAYPTR(m_pCameraBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}
DWORD CGetV3D::ReadLightData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pLightBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// ��ȡmesh��Ϣ
	char* Offset = m_pLightBuf;               // �ֽ��������ݿ�ʼ��λ��
	DWORD LightCount;                         // Light��Ŀ
	CPY_FROM_STREAM(LightCount, Offset)       // ��ȡLight��Ŀ
	
	// ��ȡÿ��Light
	for (UINT iLight = 0; iLight < LightCount; ++iLight)
	{
		UINT NameLen;
		CPY_FROM_STREAM(NameLen, Offset)
		Light* pLight = NULL;
		if (NameLen > 0)
		{
			char *sName = new char[NameLen + 1];
			ZeroMemory(sName, NameLen + 1);
			memcpy(sName, Offset, NameLen);
			Offset += NameLen;
			//�����ƹ�
			pLight = m_pVGSSceneMgr->CreateLight(sName, false);
			DELETE_ARRAYPTR(sName)
		}
		else
		{
			MessageBox(NULL, LPCSTR("������û��Ϊ�ƹ�ָ�����ƣ����ܻᵼ�����⣡"), LPCSTR("����"), 0);
			//�����ƹ�
			// pLight = m_pVGSSceneMgr->mSceneMgr->createLight(sName);
			continue;
		}

		V3D_LightDetail LightDetail;
		CPY_FROM_STREAM(LightDetail, Offset)  // һ���Զ�ȡlight����

		Ogre::ColourValue color;
		pLight->setType(Ogre::Light::LightTypes(LightDetail.type));
		color.setAsRGBA(Ogre::RGBA(LightDetail.Dif_RGBA));
		pLight->setDiffuseColour(color);
		color.setAsRGBA(Ogre::RGBA(LightDetail.Spe_RGBA));
		pLight->setSpecularColour(color);
		pLight->setAttenuation(LightDetail.fAttenuationRange, LightDetail.fAttenuationConst, LightDetail.fAttenuationLinear, LightDetail.fAttenuationQuadric);
		pLight->setPosition(LightDetail.PositionX,  LightDetail.PositionY, LightDetail.PositionZ);
		pLight->setDirection(LightDetail.DirectionX, LightDetail.DirectionY, LightDetail.DirectionZ);

		// ����Ǿ۹�ƣ���Ҫ��ȡ�۹�Ƶ��ڡ���׶�εĽǶ�,falloff
		if (pLight->getType() == Ogre::Light::LightTypes::LT_SPOTLIGHT)
		{
			float innerAngle, outerAngle, falloff;
			CPY_FROM_STREAM(innerAngle, Offset)
			CPY_FROM_STREAM(outerAngle, Offset)
			CPY_FROM_STREAM(falloff, Offset)
	
			pLight->setSpotlightFalloff(Ogre::Real(falloff));
			pLight->setSpotlightInnerAngle(Ogre::Radian(Ogre::Real(innerAngle)));
			pLight->setSpotlightOuterAngle(Ogre::Radian(Ogre::Real(outerAngle)));
			
		}
	} // for light

	DELETE_ARRAYPTR(m_pLightBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}*/

//Read the Text data
DWORD CGetV3D::ReadTextData()
{
	return ERR_COMMON_OK;
}
//Read the overlay data
DWORD CGetV3D::ReadOverlayData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pOverlayBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// ��ȡ����ͼ������
	char* Offset = m_pOverlayBuf; 
	int OverlayCount = 0; 
	CPY_FROM_STREAM(OverlayCount, Offset);

	for (int i = 0; i < OverlayCount; ++i)
	{
		V3D_OverlayInfo Info;
		CPY_FROM_STREAM(Info, Offset);  // Overlay��������

		// ����Overlay
		RECT rect;
		rect.left = rect.right = rect.top = rect.bottom = 10;

		Ogre::String sBaseIMg = "";
		Ogre::String sOpacImg = "";
		if (!Ogre::String(Info.BaseImg).empty())
		{
			sBaseIMg = AddPathtoTextureFile(Info.BaseImg);
		}
		if (!Ogre::String(Info.OpacImg).empty())
		{
			sOpacImg = AddPathtoTextureFile(Info.OpacImg);
		}

		// ����Overlay
		DWORD overlyaHandle = CreateOverlayFromFile((DWORD)m_pVGSSceneMgr, Info.Name, sBaseIMg.c_str(), sOpacImg.c_str(), rect);

		// �ӿڴ�С
		float vpwidth, vpheight;
		GUI_GetViewportSize(vpwidth, vpheight);

		float left, top, width, height;  // overlay��ʵ�����꼰��С

		if (Info.bRelativePos)  // ת���ɾ�������
		{
			left = vpwidth * Info.Left;
			top  = vpheight * Info.Top;
		}
		else
		{
			left = Info.Left;
			top  = Info.Top;
		}

		if (Info.bRelativeSize)  // ת���ɾ�������
		{
			width  = vpwidth * Info.Width;
			height = vpheight * Info.Height;
		}
		else
		{
			width  = Info.Width;
			height = Info.Height;
		}

		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, overlyaHandle, VGSOFFSETORIGIN::VOO_TOP_LEFT);   // ������Ϊ���ϽǱ�������λ��
		SetGUIObjUseRelativePos((DWORD)m_pVGSSceneMgr, overlyaHandle, Info.bRelativePos);
		SetGUIObjUseRelativeSize((DWORD)m_pVGSSceneMgr, overlyaHandle, Info.bRelativeSize);
		SetGUIObjectSize((DWORD)m_pVGSSceneMgr, overlyaHandle, width, height, Info.bRelativeSize);
		SetGUIObjectPosition((DWORD)m_pVGSSceneMgr, overlyaHandle, left, top, Info.bRelativePos);
		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, overlyaHandle, VGSOFFSETORIGIN(Info.Origin));  // ����offset origin
		SetGUIObjectOpacity((DWORD)m_pVGSSceneMgr, overlyaHandle, Info.Opacity);
		SetGUIObjectVisibility((DWORD)m_pVGSSceneMgr, overlyaHandle, Info.bVisible);

	}

	return ERR_COMMON_OK;
}
//Read the navigater data
DWORD CGetV3D::ReadNavigaterData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pNaviBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// ��ȡ����ͼ������
	char* Offset = m_pNaviBuf; 
	int NavCount = 0; 
	CPY_FROM_STREAM(NavCount, Offset);

	for (int i = 0; i < NavCount; ++i)
	{
		V3D_NavigatorInfo NaviInfo;  // һ������ͼ������

		CPY_FROM_STREAM(NaviInfo.NaviBaseInfo, Offset);  // ��������

		FRect maprect;
		CPY_FROM_STREAM(maprect, Offset);                 // map��������	

		// �ӿڴ�С
		float vpwidth, vpheight;
		GUI_GetViewportSize(vpwidth, vpheight);

		FRect rect;
		if (NaviInfo.NaviBaseInfo.bRelativePos)  // ת���ɾ�������
		{
			rect.Left = vpwidth * NaviInfo.NaviBaseInfo.Left;
			rect.Top  = vpheight * NaviInfo.NaviBaseInfo.Top;
		}
		else
		{
			rect.Left = NaviInfo.NaviBaseInfo.Left;
			rect.Top  = NaviInfo.NaviBaseInfo.Top;
		}

		if (NaviInfo.NaviBaseInfo.bRelativeSize)  // ת���ɾ�������
		{
			rect.Right  = rect.Left + vpwidth * NaviInfo.NaviBaseInfo.Width;
			rect.Bottom = rect.Top + vpheight * NaviInfo.NaviBaseInfo.Height;
		}
		else
		{
			rect.Right  = rect.Left + NaviInfo.NaviBaseInfo.Width;
			rect.Bottom = rect.Top +NaviInfo.NaviBaseInfo.Height;
		}

		// ��������ͼ
		DWORD Navhandle = CreateNavigater((DWORD)m_pVGSSceneMgr, NaviInfo.NaviBaseInfo.Name, 
			AddPathtoTextureFile(NaviInfo.NaviBaseInfo.MapImg).c_str(), rect);   

		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, Navhandle, VGSOFFSETORIGIN::VOO_TOP_LEFT);   // ������Ϊ���ϽǱ�������λ��
		SetGUIObjUseRelativePos((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.bRelativePos);
		SetGUIObjUseRelativeSize((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.bRelativeSize);
		SetGUIObjectSize((DWORD)m_pVGSSceneMgr, Navhandle, rect.Right - rect.Left, rect.Bottom - rect.Top, NaviInfo.NaviBaseInfo.bRelativeSize);
		SetGUIObjectPosition((DWORD)m_pVGSSceneMgr, Navhandle, rect.Left, rect.Top, NaviInfo.NaviBaseInfo.bRelativePos);
		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, Navhandle, VGSOFFSETORIGIN(NaviInfo.NaviBaseInfo.Origin));  // ����offset origin
		SetGUIObjectOpacity((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.Opacity);
		SetGUIObjectVisibility((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.bVisible);

		// SetNavigaterMap((DWORD)m_pVGSSceneMgr, Navhandle, NaviInfo.NaviBaseInfo.MapImg);   // ���õ�����ͼ
		SetNavigaterSceneRect((DWORD)m_pVGSSceneMgr, Navhandle, maprect);  // ���õ�������

		CPY_FROM_STREAM(NaviInfo.PointerCount, Offset);   // ���������

		for (int i = 0; i < NaviInfo.PointerCount; ++i)
		{
			V3D_NaviPointerInfo PointerInfo;
			CPY_FROM_STREAM(PointerInfo, Offset);   // ����������

			// ��ȡ�����㵼���Ķ�����
			DWORD pointedHandle = 0;
			if (PointerInfo.NavType == 0 && PointerInfo.PointedIndex >=0 && PointerInfo.PointedIndex < m_vpCamera.size())  // camera
			{
				pointedHandle = (DWORD)m_vpCamera[PointerInfo.PointedIndex];
			}
			else if (PointerInfo.NavType == 1 && PointerInfo.PointedIndex >=0 && PointerInfo.PointedIndex < m_vEntityList.size())  // ģ��
			{
				pointedHandle = m_vEntityList[PointerInfo.PointedIndex];
			}

			// ����������
			bool re = AddNavigaterPointer((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, AddPathtoTextureFile(PointerInfo.PointerImg).c_str(), 
								PointerInfo.NavType, pointedHandle);
			// ���õ��������
			if (re)
			{
				SetNavPointerVisiblilty((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, PointerInfo.bVisible);
				SetNavPointerOpacity((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, PointerInfo.Opacity);
				SetNavPointerSize((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, PointerInfo.width, PointerInfo.height);
				SetPointerPosition((DWORD)m_pVGSSceneMgr, Navhandle, PointerInfo.sName, PointerInfo.left, PointerInfo.top);
			} // if

		} // for ÿ��������

	} // for ÿ������ͼ

	return ERR_COMMON_OK;
}

//Read the button data
DWORD CGetV3D::ReadButtonData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pButtonBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// ��ȡ����ͼ������
	char* Offset = m_pButtonBuf; 
	int BtnCount = 0; 
	CPY_FROM_STREAM(BtnCount, Offset);

	for (int i = 0; i < BtnCount; ++i)
	{
		V3D_ButtonInfo BtnInfo;  // һ����ť������
		CPY_FROM_STREAM(BtnInfo, Offset);  // ��������

		// ������ť
		RECT rect;
		rect.bottom = rect.left = rect.right = rect.top = 0;

		Ogre::String sUpImg = "";
		Ogre::String sDownImg = "";
		Ogre::String sOverImg = "";
		if (!Ogre::String(BtnInfo.UpImageName).empty())
		{
			sUpImg = AddPathtoTextureFile(BtnInfo.UpImageName);
		}
		if (!Ogre::String(BtnInfo.DownImageName).empty())
		{
			sDownImg = AddPathtoTextureFile(BtnInfo.DownImageName);
		}
		if (!Ogre::String(BtnInfo.OverImageName).empty())
		{
			sOverImg = AddPathtoTextureFile(BtnInfo.OverImageName);
		}
		DWORD BtnHandle = CreateButtonFromFile((DWORD)m_pVGSSceneMgr, BtnInfo.Name, rect, 
			                                    sUpImg.c_str(), sOverImg.c_str(), sDownImg.c_str());
		
		// ���㰴ť��ʵ��λ�ú�ʵ�ʴ�С

		// �ӿڴ�С
		float vpwidth, vpheight;
		GUI_GetViewportSize(vpwidth, vpheight);

		float left, top, width, height;  // overlay��ʵ�����꼰��С

		if (BtnInfo.bRelativePos)  // ת���ɾ�������
		{
			left = vpwidth * BtnInfo.Left;
			top  = vpheight * BtnInfo.Top;
		}
		else
		{
			left = BtnInfo.Left;
			top  = BtnInfo.Top;
		}

		if (BtnInfo.bRelativeSize)  // ת���ɾ�������
		{
			width  = vpwidth * BtnInfo.Width;
			height = vpheight * BtnInfo.Height;
		}
		else
		{
			width  = BtnInfo.Width;
			height = BtnInfo.Height;
		}

		// ���ð�ť������
		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, BtnHandle, VGSOFFSETORIGIN::VOO_TOP_LEFT);   // ������Ϊ���ϽǱ�������λ��
		SetGUIObjUseRelativePos((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.bRelativePos);
		SetGUIObjUseRelativeSize((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.bRelativeSize);
		SetGUIObjectSize((DWORD)m_pVGSSceneMgr, BtnHandle, width, height, BtnInfo.bRelativeSize);
		SetGUIObjectPosition((DWORD)m_pVGSSceneMgr, BtnHandle, left, top, BtnInfo.bRelativePos);
		SetGUIObjOffsetOrigin((DWORD)m_pVGSSceneMgr, BtnHandle, VGSOFFSETORIGIN(BtnInfo.Origin));  // ����offset origin
		SetGUIObjectOpacity((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.Opacity);
		SetGUIObjectVisibility((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.bVisible);
		SetButtonCallBack((DWORD)m_pVGSSceneMgr, BtnHandle, BtnInfo.CallbackFunName);   // ��ť�Ļص�����
		
	}
	
	return ERR_COMMON_OK;
}

// ����ͼ
DWORD CGetV3D::ReadBackdropData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pBackdropBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	// ��ȡ����ͼ������
	char* Offset = m_pBackdropBuf; 
	int Count = 0; 
	CPY_FROM_STREAM(Count, Offset);

	if (Count > 0)
	{
		V3D_BackdropInfo Info;
		CPY_FROM_STREAM(Info, Offset);  // ��������

		Ogre::String str = AddPathtoTextureFile(Info.ImgFile);
		SetBackground((DWORD)m_pVGSSceneMgr, str.c_str());
		SetBackgroundVisibility((DWORD)m_pVGSSceneMgr, Info.bVisible);
		SetBackgroundRect((DWORD)m_pVGSSceneMgr, Info.Left, Info.Top, Info.Width, Info.Height);
	}

	return ERR_COMMON_OK;
}

//Read AudioData
DWORD CGetV3D::ReadAudioData()
{
	return ERR_COMMON_OK;
}

//Read FlashData
DWORD CGetV3D::ReadFlashData()
{
	return ERR_COMMON_OK;
}

// ����
DWORD CGetV3D::ReadAnimationData()
{
	return ERR_COMMON_OK;
}

//Read privacy data
DWORD CGetV3D::ReadPrivacyData()
{
	return ERR_COMMON_OK;
}

// ����
DWORD CGetV3D::ReadControlData()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pVGSSceneMgr || !m_pControlBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}	

	// ��ȡ�ܵĿ�������Ŀ
	char* Offset = m_pControlBuf;                 // �ֽ�����controller���ݿ�ʼ��λ��
	DWORD TotalCount = 0;                         // controller����Ŀ
	CPY_FROM_STREAM(TotalCount, Offset)           // ��ȡcontroller����Ŀ
	
	if (TotalCount == 0)
	{
		return ERR_COMMON_OK;
	}

	DWORD WalkerCount = 0;                         // walker��Ŀ
	CPY_FROM_STREAM(WalkerCount, Offset)           // ��ȡwalker��Ŀ
	for (UINT i = 0; i < WalkerCount; ++i)
	{
		WalkerInfo Info;
		CPY_FROM_STREAM(Info, Offset)
		m_pVGSSceneMgr->CreateWalker(Info);
	}

	DWORD FlyerCount = 0;                          // flyer��Ŀ
	CPY_FROM_STREAM(FlyerCount, Offset)           // ��ȡflyer��Ŀ
	for (UINT i = 0; i < FlyerCount; ++i)
	{
		FlyerInfo Info;
		CPY_FROM_STREAM(Info, Offset)
		m_pVGSSceneMgr->CreateFlyer(Info);
	}

	DWORD EditorCount = 0;                          // editor��Ŀ
	CPY_FROM_STREAM(EditorCount, Offset)            // ��ȡeditor��Ŀ
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
			if (Info.LookAtModelIndex >= 0 && Info.LookAtModelIndex < m_vEntityList.size())
			{
				Info.LookAtModelIndex = m_vEntityList[Info.LookAtModelIndex];  // ����Ϊ������ģ�;��
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
void CGetV3D::ConstructSceneGraph(SceneNode* pNode, char* offset)
{
	SceneNode* pChildNode = NULL;
	bool	bHasSkeleton = false;
	VGS_Node node;
	String RealName;

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

	//���벢����λ��\ת��\����
	Ogre::Vector3 pos;
	CPY_FROM_STREAM(pos.x, offset);
	CPY_FROM_STREAM(pos.y, offset);
	CPY_FROM_STREAM(pos.z, offset);

	Ogre::Quaternion quat;
	CPY_FROM_STREAM(quat.x, offset);
	CPY_FROM_STREAM(quat.y, offset);
	CPY_FROM_STREAM(quat.z, offset);
	CPY_FROM_STREAM(quat.w, offset);

	Vector3 scale;
	CPY_FROM_STREAM(scale.x, offset);
	CPY_FROM_STREAM(scale.y, offset);
	CPY_FROM_STREAM(scale.z, offset);

	//�����Node�ؼ�֡�����ĳ���
	float animationLength;
	CPY_FROM_STREAM(animationLength, offset);

	//�ҵ���Ӧ�ĳ�������
	switch (node.type)
	{
	case SCENEROOT:
		pChildNode = pNode;
		break;

	case CAMERANODE:
		{
			if (node.index >= 0 && node.index < m_vpCamera.size())
			{
				Ogre::Camera* pCam = m_vpCamera[node.index];/*m_pVGSSceneMgr->GetCamera(node.index)*/;
				Vector3 vPos = pCam->getPosition();
				Quaternion q = pCam->getOrientation();

				RealName = pCam->getName();
				pChildNode = pNode->createChildSceneNode(RealName);
				pChildNode->attachObject(pCam);
				pCam->setPosition(vPos);
				pCam->setOrientation(q);
			}
			else
			{
				pChildNode = pNode->createChildSceneNode();
			}
		}
		break;

	case LIGHTNODE:
		{
			//if (node.index >= 0 && node.index < m_vpLight.size())
			{
				Ogre::Light* pLight = /*m_vpLight[node.index];*/m_pVGSSceneMgr->GetLight(node.index);

				Vector3 vPos = pLight->getPosition();
				Vector3 vDir = pLight->getDirection();

				RealName = pLight->getName();
				pChildNode = pNode->createChildSceneNode(RealName);
				pChildNode->attachObject(pLight);

				pLight->setPosition(vPos);

				if (pLight->getType() != Ogre::Light::LT_POINT)
				{
					pLight->setDirection(vDir);
				}
			}
			/*else
			{
				pChildNode = pNode->createChildSceneNode();
			}*/
		}
		break;

	case MESHNODE:
		{
			Ogre::Entity* pEntity = m_pVGSSceneMgr->GetEntity(node.index);
			RealName = pEntity->getName();
			pChildNode = pNode->createChildSceneNode(RealName);
			pChildNode->attachObject(pEntity);
			bHasSkeleton = pEntity->hasSkeleton();
		}
		break;

	case BACKDROP:
		{
			pChildNode = m_pVGSSceneMgr->GetBackgroundNode();
			break;
		}
	}

	//Node�ı任, ����ı任�������ﴦ��---------------------------------------------------------
	//λ��//ת������ϵ
	if (node.type == SCENEROOT)
	{
		pChildNode->setPosition(Vector3(0,0,0));
	}
	else
	{
		//λ��
		pChildNode->setPosition(pos);
		//��ת//ת������ϵ
		pChildNode->setOrientation(quat);
		//����//ת������ϵ
		pChildNode->setScale(scale);
		
		//����VGS����������
		m_pVGSSceneMgr->AddNodeNameMap(pChildNode->getName(), pChildNode->getName());
	}

	//��Ⱦͬ������
	//EnterCriticalSection(m_render_cs);
	//Node�Ĺؼ�֡����----------------------------------------------------
	if (animationLength > 0 && !bHasSkeleton) //�й�����������Թؼ�֡����
		ReadAndCreateNodeAnimation(pChildNode, animationLength, &offset);
	//��Ⱦͬ������
	//LeaveCriticalSection(m_render_cs);

	m_Offset = offset - m_pSceneGraphBuf; // �ض�λoffset

	//�����ӽڵ�����
	int childNodeCnt = 0;
	CPY_FROM_STREAM(childNodeCnt, offset);
	for (int i = 0; i< childNodeCnt; i++)
	{
		ConstructSceneGraph(pChildNode, m_pSceneGraphBuf + m_Offset);
		// ConstructSceneGraph(pChildNode, m_Offset);
	}

}

//�����ڵ�Ķ���
void CGetV3D::ReadAndCreateNodeAnimation(SceneNode* pNode, float animationLength, char** offset)
{
	//���붯������-------------------------------------------------
	//�����ؼ�֡����
	// Ogre::String strAnimName = "Anim_" + pNode->getName();
	Ogre::String strAnimName = _GetNodeAnimationName(pNode);  // ��ȡ�ڵ㶯������
	Animation* pAnim = m_pVGSSceneMgr->mSceneMgr->createAnimation(strAnimName, animationLength);

	//����ؼ�֡�����Ĺؼ�֡����
	int keyframeCnt;
	CPY_FROM_STREAM(keyframeCnt, *offset);

	//���ù��ɷ�ʽ
	pAnim->setInterpolationMode(Ogre::Animation::InterpolationMode::IM_LINEAR);
	
	//����һ�ζ����켣
	NodeAnimationTrack* pAnimTrack = pAnim->createNodeTrack(0, pNode);
	//��֡��������
	for (int kfrIndex = 0; kfrIndex< keyframeCnt; kfrIndex++)
	{
		NodeTransform nodeTransform; 
		CPY_FROM_STREAM(nodeTransform, *offset);
		
		//��ǰ֡��Ӧ��Transform����
		TransformKeyFrame* pKfr = pAnimTrack->createNodeKeyFrame(nodeTransform.time);

		//λ��//����ϵת��
		Vector3 pos;
		pos.x = nodeTransform.position.x;
		pos.y = nodeTransform.position.y;
		pos.z = nodeTransform.position.z;
		pKfr->setTranslate(pos);

		//ת��//����ϵת��
		Ogre::Quaternion quat;
		quat.x = nodeTransform.quat.x;
		quat.y = nodeTransform.quat.y;
		quat.z = nodeTransform.quat.z;
		quat.w = nodeTransform.quat.w;
		pKfr->setRotation(quat);

		//����
		Vector3 scale;
		scale.x = nodeTransform.scale.x;
		scale.y = nodeTransform.scale.y;
		scale.z = nodeTransform.scale.z;
		pKfr->setScale(scale);
	}
	
	//�Ż�
	pAnimTrack->optimise();

	//��������״̬����
	AnimationState* pAnimState = m_pVGSSceneMgr->mSceneMgr->createAnimationState(strAnimName);
	pAnimState->setEnabled(true);
	pAnimState->setLoop(true);

	// ����һ���ڵ㶯��
	m_pVGSSceneMgr->GetAnimationManager()->AddNodeKeyfrmAnimation(pAnimState);
}


// ����ͼ
DWORD CGetV3D::ReadSceneGraph()
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
	SceneNode* pRootNode = m_pVGSSceneMgr->mSceneMgr->getRootSceneNode();
	ConstructSceneGraph(pRootNode, m_pSceneGraphBuf);	// ��ȡSceneGraph��Ϣ// �ֽ��������ݿ�ʼ��λ��

	// ɾ���ֽ���
	DELETE_ARRAYPTR(m_pSceneGraphBuf);  

	return ERR_COMMON_OK;
}


/*
// ����ͼ
DWORD CGetV3D::ReadSceneGraph()
{
	if (!m_pFile)
	{
		return ERR_FILE_OPENFAIL;
	}	
	if (!m_pSceneGraphBuf)
	{
		return ERR_FILE_V3D_NODATA;
	}

	std::vector <SceneGraphNode> vSceneGraph;  // ����ͼ���ݽṹ

	// ��ȡSceneGraph��Ϣ
	char* Offset = m_pSceneGraphBuf;               // �ֽ��������ݿ�ʼ��λ��
	DWORD NodeCount;                               // �ڵ���Ŀ
	CPY_FROM_STREAM(NodeCount, Offset)             // ��ȡ�ڵ���Ŀ
	
	if (NodeCount == 0)
	{
		return ERR_COMMON_OK;
	}

	vSceneGraph.reserve(NodeCount); // Ԥ���ռ�

	// ��ȡ���ڵ��µĽڵ�
	for (UINT iNode = 0; iNode < NodeCount; ++iNode)
	{
		SceneGraphNode SGNode;
		UINT ObjCount = 0;

		memcpy(&SGNode, Offset, sizeof(NodeSaveParam));  // ֱ�Ӷ�ȡsizeof(NodeSaveParam)��С�����ݿ鵽SGNode
		Offset += sizeof(NodeSaveParam);

		CPY_FROM_STREAM(ObjCount, Offset)               // ��ȡ�ڵ��¸�������ĸ���
		for (UINT iObj = 0; iObj < ObjCount; ++iObj)
		{
			NodeObject obj;
			CPY_FROM_STREAM(obj.type, Offset)   // �ڵ���������
			CPY_FROM_STREAM(obj.Index, Offset)  // �ڵ�����ڸ��Դ洢�����е�����
			SGNode.vObject.push_back(obj);
		}		
		vSceneGraph.push_back(SGNode);
		// vSceneGraph[iNode] = SGNode;
	} // for iNode

	SceneManager *pSceneMgr = m_pVGSSceneMgr->mSceneMgr;   // Ogre����������

	std::vector <Ogre::SceneNode*> vSceneNodeList;       // �����Ѿ�������ogre�����ڵ�
	vSceneNodeList.reserve(NodeCount);                    // Ԥ����ռ�
	// vSceneNodeList.resize(NodeCount);
	// vSceneNodeList[0]  = pSceneMgr->getRootSceneNode();   // ��һ��λ�ô洢���ڵ�  
	vSceneNodeList.push_back(pSceneMgr->getRootSceneNode());
	pSceneMgr->getRootSceneNode()->setPosition(0, 0, 0);

	// ����ÿһ�������ڵ�
	for (DWORD iNode = 0; iNode < NodeCount; ++iNode)
	{
		Ogre::SceneNode *pNode = NULL; //pSceneMgr->createSceneNode();    //  create secene node
		SceneGraphNode &SGNode = vSceneGraph[iNode];              // scene graph node
		if (SGNode.ParentIdx != -1 && (DWORD)SGNode.ParentIdx <= iNode)  // ���ڵ�һ�����Ѿ��������Ľڵ�
		{
			// vSceneNodeList[SGNode.ParentIdx]->addChild(pNode);    // ���ø��׽ڵ�
			pNode = vSceneNodeList[SGNode.ParentIdx]->createChildSceneNode();
		}
		if (!pNode)
		{
			continue;
		}
		vSceneNodeList.push_back(pNode);   // 

		// set node transform
		pNode->setPosition(SGNode.PosX, SGNode.PosY, SGNode.PosZ);
		pNode->setScale(SGNode.ScaleX, SGNode.ScaleY, SGNode.ScaleZ);
		pNode->setOrientation(SGNode.OrientW, SGNode.OrientX, SGNode.OrientY, SGNode.OrientZ);

		// �ѽڵ�����ж���attach���ڵ���
		UINT ObjCount = SGNode.vObject.size();
		for (UINT iObj = 0; iObj < ObjCount; ++iObj)
		{
			NodeObject &Obj = SGNode.vObject[iObj];
			if (Obj.type == NBT_CAMERA) // camera node
			{
				Camera *pCamera = m_pVGSSceneMgr->GetCamera(Obj.Index);
				if (pCamera)
				{
					pNode->attachObject(pCamera);
					V3D_CameraDetail &CamDetail = m_vCameraDetail[Obj.Index];
					pCamera->setProjectionType(Ogre::ProjectionType(CamDetail.PrjType));
					pCamera->setPosition(CamDetail.FromX, CamDetail.FromY, CamDetail.FromZ);
					Ogre::Quaternion orient(CamDetail.OrientW, CamDetail.OrientX, CamDetail.OrientY, CamDetail.OrientZ);
					pCamera->setOrientation(orient);
					pCamera->setPolygonMode(Ogre::PolygonMode(CamDetail.PolygonMode));
					pCamera->setFOVy(Ogre::Radian(Real(CamDetail.fFOVY)));
					pCamera->setNearClipDistance(Ogre::Real(CamDetail.nearClip));
					pCamera->setFarClipDistance(Ogre::Real(CamDetail.farClip));
					pCamera->setDirection(CamDetail.DirectionX, CamDetail.DirectionY, CamDetail.DirectionZ);

				}
			}
			else if (Obj.type == NBT_LIGHT) // camera node
			{
				Light* pLight = m_pVGSSceneMgr->GetLight(Obj.Index);
				if (pLight)
				{
					pNode->attachObject(pLight);
					pLight->setPosition(0, 0, 0);
				}
			}
			else if (Obj.type == NBT_ENTITY) // entity
			{
				Entity *pEntity = m_pVGSSceneMgr->GetEntity(Obj.Index);
				if (pEntity)
				{
					pNode->attachObject(pEntity);
				}		
			}
		} // for obj
	} // for iNode

	DELETE_ARRAYPTR(m_pSceneGraphBuf);  // ɾ���ֽ���

	return ERR_COMMON_OK;
}*/


// �õ�texture�������ļ��еķ�Χ
void CGetV3D::GetTextureDataRange(const V3D_Table &table, DWORD &start, DWORD &len)
{
	
	// ����������λ����Ϣ��С����������texturePtr�����λ�ü�ȥtexturePtr����texure�ĳ���
	std::vector<DWORD> v_ptrSortVector;
	
    v_ptrSortVector.push_back(table.SceneproperyPtr);
	v_ptrSortVector.push_back(table.mtrlDataPtr);
	v_ptrSortVector.push_back(table.texturePtr);
	v_ptrSortVector.push_back(table.cameraDataPtr);
	v_ptrSortVector.push_back(table.lightDataPtr);
	v_ptrSortVector.push_back(table.skeletonDataPtr);
	v_ptrSortVector.push_back(table.meshDataPtr);
	v_ptrSortVector.push_back(table.shapeDataPtr);
	v_ptrSortVector.push_back(table.sceneGraphDataPtr);
	v_ptrSortVector.push_back(table.TextDataPtr);
	v_ptrSortVector.push_back(table.AnimationPtr);
	v_ptrSortVector.push_back(table.OverlayPtr);
	v_ptrSortVector.push_back(table.ButtonPtr);
	v_ptrSortVector.push_back(table.NavigatotPtr);
	v_ptrSortVector.push_back(table.AudioPtr);
	v_ptrSortVector.push_back(table.ControlPtr);
	v_ptrSortVector.push_back(table.FlashPtr);
	v_ptrSortVector.push_back(table.PrivacyDataPtr);

	std::sort(v_ptrSortVector.begin(), v_ptrSortVector.end());
    
	std::vector<DWORD>::iterator it = v_ptrSortVector.begin();
	for (; it != v_ptrSortVector.end(); ++it)
	{
		if ((*it) == table.texturePtr)
		{
			len = (*(it + 1)) - table.texturePtr;
		   break;
		}
	}

	start = table.texturePtr;        // texture��ʼλ��
}

// ��ָ���Ĵ��ļ������·��
Ogre::String CGetV3D::AddPathtoTextureFile(const char * sPureFileName)
{
	// ����ļ��Ƿ���ָ��������·���»�ǰ��Դ·����
	Ogre::String str = "";
	if (_IsFileNameInResourcePath(Ogre::String(sPureFileName), str))
	{
		return str;
	}
	return (sDefaultTexturePath + Ogre::String(sPureFileName));
}

// ���·�����ͼ�еĵƹ���
VOID CGetV3D::UpdateBumpNormalmapLight()
{
	UINT count = m_pVGSSceneMgr->GetBumpmapCount();
	for (UINT i = 0; i < count; ++i)
	{
		VGSBumpmapInfo info;
		m_pVGSSceneMgr->GetBumpmapParam(i, info);
		if (info.m_Type == VBT_NORMARLMAP)
		{
			INT index = (INT)info.m_BumpNormalParam.LightHandle;
			if (index >= 0 && index < m_vpLight.size())
			{
				info.m_BumpNormalParam.LightHandle = (DWORD)m_vpLight[index];
			}
			else
			{
				info.m_BumpNormalParam.LightHandle = 0;
			}
		}
	}
}

// �������ɺ��һЩ����
VOID CGetV3D::PostOpen()
{
	// ���·�����ͼ�еĵƹ���
	UpdateBumpNormalmapLight();
}