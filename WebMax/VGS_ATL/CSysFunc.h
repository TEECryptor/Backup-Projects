/////////////////////////////////////////////////////////////////////////
// ����: CSysFunc.h
// ����: ���Ͷ���,��������ϵͳ�ڴ�������
// ����: Lixin
// ����: 2006-05-01(2)
/////////////////////////////////////////////////////////////////////////

#ifndef _CSYSFUNC_H
#define _CSYSFUNC_H

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <algorithm>


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p); p=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p); p=NULL; } }
#endif

//�Զ�����Ķ����Release����ֻ���ͷ��˶����ڲ��������ڴ棬������ͷŻ���Ҫdelete
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { p->Release();} }
#endif

//d3d����ֻ��ʹ��Release()����,����delete
#ifndef SAFE_RELEASE_D3D
#define SAFE_RELEASE_D3D(p)		{ if(p) { p->Release(); p=NULL; } }
#endif

//define const Macro
const float PI2 = D3DX_PI * 2;
const float PI = D3DX_PI;
const float PI_2 = D3DX_PI/2;
const float PI_4 = D3DX_PI/4;
const float STOA = 180.0f/PI;
const float ATOS = PI/180.0f;

typedef struct _NAME
{
	CHAR*	name;
}_NAME;
//Edit by Baojiahui 2007/9/27
typedef struct TriggerCallBack
{
  DWORD      triggerHandle;         //�������ľ��
  CHAR*      callBackFunc;          //�ص���������
}TriggerCallBack;

//�ؼ�֡������֡�ص������ṹ
typedef struct KFRCallBack
{
	INT			frameIndex;				//֡��ֵ
	CHAR*		callBackFunc;			//�ص���������
}KFRCallBack;

//�������������
enum 
{
	NODETYPE_NONE,
	NODETYPE_MODEL,
	NODETYPE_CAMERA,
	NODETYPE_LIGHT
};
//�õ��ַ����ĳ���
//inline DWORD GetStrLength(CHAR* strString)
//{
//	UINT index = 0;
//	while (strString[index] != '\0')
//		index ++;
//	return index;
//}

//���ַ���ȫ��ת���ɴ�д��ĸ
//inline VOID StrToCaption(CHAR* strString)
//{
//	//�ȵõ��ַ����ĳ���
//	UINT index = 0;
//	while (strString[index] != '\0')
//	{
//		if (strString[index] >= 97 && strString[index] < 129)
//		strString[index] = strString[index] - 32;
//		index ++;
//	}
//}

//-------------------------------------------------------------------------------------------
//������: GetChar()
//��  ��: CHAR *DesString, CHAR* OriString, UUUINT startIndex, UUUINT endIndex
//��  ;: �õ��ַ���ָ���ֶ�
//-------------------------------------------------------------------------------------------
//inline VOID GetChar(CHAR *DesString, CHAR* OriString, UINT startIndex, UINT endIndex)
//{
//	//�õ��ַ��ĸ���
//	for (UINT i= startIndex; i<= endIndex; i++)
//	{
//		DesString[i- startIndex] = OriString[i];
//	}
//	
//}

//-------------------------------------------------------------------------------------------
//������: ChangeFileEx()
//��  ����CHAR* FilePath,ԭ�����ļ�����CHAR* strExName �µ���չ��
//��  ;���޸�����·���ļ�����չ��
//-------------------------------------------------------------------------------------------
//inline VOID ChangeFileEx(CHAR* strFileName, CHAR* strExName)
//{
//	//get char count
//	UINT CharCnt = 0;
//	while (strFileName[CharCnt] != '\0')
//	{
//		CharCnt++;
//	}
//	
//	//get name char index
//	UINT CharIndex = 0;
//	while (strFileName[CharIndex] != '.')
//	{
//		CharIndex ++;
//	}
//	
//	strFileName[CharIndex+1] = strExName[0];
//	strFileName[CharIndex+2] = strExName[1];
//	strFileName[CharIndex+3] = strExName[2];
//	strFileName[CharIndex+4] = '\0';
//}

//-------------------------------------------------------------------------------------------
//������: GetFilePath()
//��  ��: CHAR* filePath, CHAR* fileName
//��  ;: �õ��ļ�·��
//-------------------------------------------------------------------------------------------
inline VOID GetFilePath(CHAR* filePath, CHAR* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != '\0')
	{
		CharCnt++;
	}
	//�ҵ����һ��б�ܵ�λ��
	UINT CharIndex = CharCnt;
	while (fileName[CharIndex] != '\\' && fileName[CharIndex] != '/')
	{
		CharIndex--;
	}
	//get path
	for (UINT m =0; m<= CharIndex; m++)
	{
		filePath[m] = fileName[m];
	}
	filePath[m] = '\0';
}

//-------------------------------------------------------------------------------------------
//������: GetPureFileName()
//��  ��: CHAR* pureFileName, CHAR* fileName
//��  ;: �õ��ļ�����
//-------------------------------------------------------------------------------------------
inline VOID GetPureFileName(CHAR* pureFileName, CHAR* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != '\0')
	{
		CharCnt++;
	}
	//�ҵ����һ��б�ܵ�λ��
	UINT CharIndex = CharCnt;
	while (fileName[CharIndex] != '\\' && fileName[CharIndex] != '/')
	{
		CharIndex--;
	}
	//get filename
	UINT pIndex =0;
	for (UINT m =CharIndex+1; m<= CharCnt; m++)
	{
		pureFileName[pIndex] = fileName[m];
		pIndex++;
	}
}

inline std::string _GetPureFileNameStr(const std::string &str)
{
	CHAR pureFileName[1024];
	ZeroMemory(pureFileName, 1024);
	GetPureFileName(pureFileName, (char*)str.c_str());
	return std::string(pureFileName);
}

inline VOID GetPureFileNameT(TCHAR* pureFileName, const TCHAR* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != TCHAR('\0'))
	{
		CharCnt++;
	}
	//�ҵ����һ��б�ܵ�λ��
	UINT CharIndex = CharCnt;
	while (fileName[CharIndex] != TCHAR('\\') && fileName[CharIndex] != TCHAR('/'))
	{
		CharIndex--;
	}
	//get filename
	UINT pIndex =0;
	for (UINT m = CharIndex+1; m<= CharCnt; m++)
	{
		pureFileName[pIndex] = fileName[m];
		pIndex++;
	}
}

//-------------------------------------------------------------------------------------------
//������: GetAppName()
//��  ��: CHAR* AppName, CHAR* fileName
//��  ;: �õ�Ӧ�ó�����
//-------------------------------------------------------------------------------------------
inline void GetAppName(char* AppName, const char* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != char('\0'))
	{
		CharCnt++;
	}
	//�ҵ����һ��б�ܵ�λ��
	UINT CharIndex = CharCnt;
	while (fileName[CharIndex] != char('\\'))
	{
		CharIndex--;
	}
	//�ҵ�.��λ��
	UINT DotIndex = CharCnt;
	while (fileName[DotIndex] != char('.'))
	{
		DotIndex--;
	}
	//get AppName
	UINT pIndex =0;
	for (UINT m =CharIndex+1; m< DotIndex; m++)
	{
		AppName[pIndex] = fileName[m];
		pIndex++;
	}
	AppName[pIndex] = char('\0');
}

inline VOID FindMediaFile(char* realPathName, char* mediaName)
{
	//char strFileName[256];
	//char strFilePath[256];
	//::GetModuleFileName(NULL, strFileName, 256);
	//GetFilePath(strFilePath, strFileName);
	//strcpy(realPathName, strFilePath);

	strcpy(realPathName, "D:\\EngineV4\\Resource\\");
	strcat(realPathName, mediaName);
}

//-------------------------------------------------------------------------------------------
//������: GetRotationVecByRT()
//��  ��: D3DXMATRIX *RosMat, D3DXVECTOR3 *RosVec
//��  ;: ������ת����õ��Ƹ������ת��
//-------------------------------------------------------------------------------------------
inline VOID GetRotationVecByRT(D3DXMATRIX *RosMat, D3DXVECTOR3 *RosVec)
{
	//���Ǹ��Ƚϸ��ӵ����⣬ʵ����������ϵ��ת�����⣬����˼·���£�
	//��һ��: ͨ����ת���X��ʸ�����õ�����ϵ��Y���ת��
	FLOAT fAngleY;
	if (RosMat->_11 == 0)
	{
		if (RosMat->_13 > 0)
			fAngleY = PI_2;
		else 
			fAngleY = PI + PI_2;
	}
	else
	{
		fAngleY = (FLOAT)atan(RosMat->_13/RosMat->_11);
		if (RosMat->_11 < 0.0f )
		{
			if (RosMat->_13 >= 0.0f) fAngleY = fAngleY + PI;
			else fAngleY = fAngleY - PI;
		}
	}
	//�ڶ���: ��Ȼͨ����ת���X��ʸ������������ϵ��Y����ת����������Z���ת��
	FLOAT fAngleZ = (FLOAT)asin(RosMat->_12);
	//�ж�����ǻ��Ƕ۽�
	if (D3DXVec3Dot(&D3DXVECTOR3(RosMat->_11, RosMat->_12, RosMat->_13), 
					 &D3DXVECTOR3(RosMat->_11, 0, RosMat->_13) ) < 0) 
		fAngleZ = PI - fAngleZ;
	//˳ʱ��ת��ʱ��
	fAngleZ = -fAngleZ;

	//������: ��������������Y����ת���Z'��������Z"��ļн�
	D3DXVECTOR3 Z_1 = D3DXVECTOR3((FLOAT)cos(PI_2 + fAngleY), 0, (FLOAT)sin(PI_2 + fAngleY));
	D3DXVECTOR3 Z_2 = D3DXVECTOR3(RosMat->_31, RosMat->_32, RosMat->_33);
	D3DXVec3Normalize(&Z_1, &Z_1);
	D3DXVec3Normalize(&Z_2, &Z_2);
	FLOAT fAngleX = (FLOAT)acos(D3DXVec3Dot(&Z_2, &Z_1));
	
	RosVec->x = fAngleX;
	RosVec->y = fAngleY;
	RosVec->z = fAngleZ;
}

//-------------------------------------------------------------------------------------------
//������: GetRTByRotation()
//��  ��: D3DXVECTOR3 *Ros, VecD3DXMATRIX *RosMat
//��  ;: ������ת����õ��Ƹ������ת��
//-------------------------------------------------------------------------------------------
inline VOID GetRTByRotationVec(D3DXVECTOR3 *Ros, D3DXMATRIX *RosMat)
{
	//�� GetRotationVecByRT()�ķ�����������
	//����Y����ת
	D3DXMatrixRotationY(RosMat, -Ros->y);
	//�����µ�Z'����ת
	D3DXMATRIX matRosZ;
	D3DXVECTOR3 Z_1 = D3DXVECTOR3((FLOAT)cos(PI_2 + Ros->y), 0, (FLOAT)sin(PI_2 + Ros->y));
	D3DXMatrixRotationAxis(&matRosZ, &Z_1, -Ros->z);
	//�����µ�X'����ת
	//------�ȼ����µ�X'������, D3D�����еĽǶȶ���˳ʱ�뷽��
	D3DXMATRIX matRosX;
	D3DXVECTOR3 X_1 = D3DXVECTOR3((FLOAT)cos(Ros->y), (FLOAT)sin(-Ros->z), (FLOAT)sin(Ros->y));
	D3DXMatrixRotationAxis(&matRosX, &X_1, -Ros->x);
	//�������
	D3DXMatrixMultiply(RosMat, RosMat, &matRosZ);
	D3DXMatrixMultiply(RosMat, RosMat, &matRosX);
}

// ��ȡ�����᷽��
inline D3DXVECTOR3 _GetAxisDirection(BYTE axis)
{
	switch (axis)
	{
	case 0:  // VGS_LAXIS_X:
		return D3DXVECTOR3(1, 0, 0);
	case 1: // VGS_LAXIS_Y:
		return D3DXVECTOR3(0, 1, 0);
	case 2: //VGS_LAXIS_Z:
		return D3DXVECTOR3(0, 0, 1);
	case 3: // VGS_LAXIS_NEGATIVEX:
		return D3DXVECTOR3(-1, 0, 0);
	case 4: //VGS_LAXIS_NEGATIVEY:  // -y
		return D3DXVECTOR3(0, -1, 0);
	case 5: //VGS_LAXIS_NEGATIVEZ: // -z
		return D3DXVECTOR3(0, 0, -1);
	default:
		return D3DXVECTOR3(0, 0, 0);
	}
}

//  �ļ���ַ����
enum VGS_FILELOCATION_TYPE
{
	VGS_FILELOCATION_TYPE_NONE = 0,
	VGS_FILELOCATION_TYPE_LOCAL,  // �����ļ�
	VGS_FILELOCATION_TYPE_WEB     // �����ļ�
};

//inline VGS_FILELOCATION_TYPE _getFileLocationType(const std::string sPath)
//{
//	std::string str = sPath;
//	std::transform(str.begin(),str.end(), str.begin(), tolower); // ת����Сд
//	if (str.find("file") == 0)
//	{
//		return VGS_FILELOCATION_TYPE_LOCAL;
//	}
//	else if (str.find("http") == 0)
//	{
//		return VGS_FILELOCATION_TYPE_WEB;
//	}
//	else
//	{
//		return VGS_FILELOCATION_TYPE_NONE;
//	}
//	
//}

// �õ���һ��Ŀ¼
// str: ��ǰĿ¼�� cFlag ��Ŀ¼�ָ����ţ�URLΪ'/'������Ϊ"\"
inline std::string _getUpperFolder(const std::string &str, const char &cFlag = '/')
{
	int length = str.length();
	if (length == 0)
	{
		return "";
	}

	// ȥ�����һ��'\'����
	std::string sNewStr = str;
	if (sNewStr[length - 1] == cFlag)
	{
		sNewStr[length - 1] = '\0';
		length--;
	}

	// �������һ��'\'����
	int idx = (int)sNewStr.rfind(cFlag);
	if (idx >= 0 && idx < length)
	{
		return sNewStr.substr(0, idx + 1);
	}

	return str;  // ����ԭ�ַ���
}

// �Ƿ���web·��
inline bool _isWebPath(const std::string &sPath)
{
	if (   (sPath[0] == 'h' || sPath[0] == 'H') && (sPath[1] == 't' || sPath[1] == 'T')
		&& (sPath[2] == 't' || sPath[2] == 'T') && (sPath[3] == 'p' || sPath[3] == 'P') )
	{
		return true;
	}
	return false;
}

// �Ƿ��Ǳ���·��
inline  bool _isLocalPath(const std::string &sPath)
{
	if (sPath.length() < 2)
	{
		return false;
	}
	if (sPath[1] == ':')
	{
		return true;
	}
	return false;
}

// ��ȡ����·��
// 1. ���path�����������URL, ֱ��ʹ��path·����
// 2. ���pathû��ʹ��$(UPPER)��ʶ�����򷵻�sServer+path�������򷵻�sServer����һ��·����path·��
inline std::string _getFullPath(const std::string &sServer, const std::string &path)
{
	// 	if (   (path[0] == 'h' || path[0] == 'H') && (path[1] == 't' || path[1] == 'T')
	// 		&& (path[2] == 't' || path[2] == 'T') && (path[3] == 'p' || path[3] == 'P') )
	if (_isWebPath(path))  // ���������URL·��
	{
		return path;
	}

	int pathLen = path.length();
	if (pathLen > 2 && path[1] == ':')  // ����Ǳ��ؾ���·��
	{
		return path;
	}

	// bool bURL = _isWebPath(sServer);  // �Ƿ�URL·��
	// char cFlag = bURL ? '/' : '\\';   // url�ͱ��ص�Ŀ¼�ָ�����ͬ
	char cFlag = '/';

	// ��ǰĿ¼
	if (pathLen > 2 && path[0] == '.' && path[1] == '/') 
	{
		std::string strRemainPath = path.substr(2);
		return _getFullPath(sServer, strRemainPath);
	}

	// ��һ��Ŀ¼
	if (pathLen > 3 && path[0] == '.' && path[1] == '.' && path[2] == '/')
	{
		std::string strUpperServer = _getUpperFolder(sServer, cFlag);  // ��һ��Ŀ¼
		std::string strRemainPath = path.substr(3);
		return _getFullPath(strUpperServer, strRemainPath);
	}

	// ��������·��
	std::string sFullPaht = sServer;
	return sFullPaht.append(path);
}

// ����ļ�����չ����Сд,����".jpg"
inline  std::string _getFileExtension(const std::string &sFileName)
{
	std::string strExtension = "";
	if (!sFileName.empty())
	{	
		std::basic_string<char>::size_type idx = sFileName.find_last_of(".", sFileName.length());
		if (idx != std::string::npos && idx < sFileName.length() - 1)
		{
			strExtension = sFileName.substr(idx, sFileName.length() - idx);
			std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), towlower);
		}	
	}

	return strExtension;
}

#endif
