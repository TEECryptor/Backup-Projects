/////////////////////////////////////////////////////////////////////////
// ����: CSysFunc.h
// ����: ���Ͷ���,��������ϵͳ�ڴ�������
// ����: Lixin
// ����: 2006-05-01(2)
/////////////////////////////////////////////////////////////////////////

#ifndef _CSYSFUNC_H
#define _CSYSFUNC_H

#include "VGSHeader.h"
//#include "VGSCore.h"
#include <windows.h>

#include <WINDEF.h>

#include <vector>

using namespace std;

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

// ���ļ���д��һ������
#define WRITE_VALUE(V, pFile) fwrite(&V, sizeof(V), 1, pFile);
#define WRITE_PTR(ptr, len, pFile) fwrite(ptr, len, 1, pFile);

// ���ļ���д��һ���ַ���
// #define WRITE_STR(STR, pFile) {UINT STR_len = STR.length(); fwrite(&STR_len, sizeof(STR_len), 1, pFile); if (STR_len > 0) fwrite(STR.c_str(), STR_len, 1, pFile);}
#define WRITE_STR(STR, pFile) {UINT STR_len = strlen(STR.c_str()); fwrite(&STR_len, sizeof(STR_len), 1, pFile); if (STR_len > 0) fwrite(STR.c_str(), STR_len, 1, pFile);}

// ���ļ���д��һ��vector3
#define WRITE_VECTOR3(V3, pFile) {fwrite(&(V3.x), sizeof(V3.x), 1, pFile); fwrite(&(V3.y), sizeof(V3.y), 1, pFile); fwrite(&(V3.z), sizeof(V3.z), 1, pFile); }


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

// vgs �����0ֵ
const float VGS_ZERO_VALUE_5 = 0.00001f;
const float VGS_ZERO_VALUE_8 = 0.00000001f;


///////////////////////// һЩ���õĺ� /////////////////////////////////
// �������֣��MAX_NAMESTR_LENGTH���ֽڣ�����'\0'
#define COPY_NAME(dest, source)\
{\
	int srcLen = min((int)strlen(source), MAX_NAMESTR_LENGTH - 1);\
	strncpy_s((char*)(dest), MAX_NAMESTR_LENGTH, (char*)(source), srcLen);\
	dest[srcLen] = '\0';\
}


// ���ݵ�DWORDת��ΪD3DCOLORVALUE
inline D3DCOLORVALUE _Get_D3DCOLORVALUE_ARGB(UINT _ARGB)
{
	D3DCOLORVALUE color;
	color.a = ((BYTE)(_ARGB >> 24) & 0x000000ff) / 255.0f;
	color.r = ((BYTE)(_ARGB >> 16) & 0x000000ff) / 255.0f;
	color.g = ((BYTE)(_ARGB >> 8) & 0x000000ff) / 255.0f;
	color.b = ((BYTE)(_ARGB & 0x000000ff)) / 255.0f;
	return color;
}

// ���ݵ�DWORDת��ΪD3DCOLORVALUE
inline D3DCOLORVALUE _Get_D3DCOLORVALUE_RGBA(UINT _RGBA)
{
	D3DCOLORVALUE color;
	color.r = ((BYTE)((_RGBA >> 24) & 0x000000ff)) / 255.0f;
	color.g = ((BYTE)((_RGBA >> 16) & 0x000000ff)) / 255.0f;
	color.b = ((BYTE)((_RGBA >> 8) & 0x000000ff)) / 255.0f;
	color.a = ((BYTE)(_RGBA & 0x000000ff)) / 255.0f;
	return color;
}

// d3d��ɫ���Ա���
inline D3DCOLORVALUE _Multiply_D3DCOLORVALUE(const D3DCOLORVALUE &colorValue, float factor)
{
	D3DCOLORVALUE color;
	color.a = colorValue.a * factor;
	color.r = colorValue.r * factor;
	color.g = colorValue.g * factor;
	color.b = colorValue.b * factor;
	return color;
}


//�õ��ַ����ĳ���
inline DWORD GetStrLength(CHAR* strString)
{
	UINT index = 0;
	while (strString[index] != '\0')
		index ++;
	return index;
}

//���ַ���ȫ��ת���ɴ�д��ĸ
inline VOID StrToCaption(CHAR* strString)
{
	//�ȵõ��ַ����ĳ���
	UINT index = 0;
	while (strString[index] != '\0')
	{
		if (strString[index] >= 97 && strString[index] < 129)
		strString[index] = strString[index] - 32;
		index ++;
	}
}

//-------------------------------------------------------------------------------------------
//������: GetChar()
//��  ��: CHAR *DesString, CHAR* OriString, UUUINT startIndex, UUUINT endIndex
//��  ;: �õ��ַ���ָ���ֶ�
//-------------------------------------------------------------------------------------------
inline VOID GetChar(CHAR *DesString, CHAR* OriString, UINT startIndex, UINT endIndex)
{
	//�õ��ַ��ĸ���
	for (UINT i= startIndex; i<= endIndex; i++)
	{
		DesString[i- startIndex] = OriString[i];
	}
	
}

//-------------------------------------------------------------------------------------------
//������: ChangeFileEx()
//��  ����CHAR* FilePath,ԭ�����ļ�����CHAR* strExName �µ���չ��
//��  ;���޸�����·���ļ�����չ��
//-------------------------------------------------------------------------------------------
inline VOID ChangeFileEx(CHAR* strFileName, CHAR* strExName)
{
	//get char count
	UINT CharCnt = 0;
	while (strFileName[CharCnt] != '\0')
	{
		CharCnt++;
	}
	
	//get name char index
	UINT CharIndex = 0;
	while (strFileName[CharIndex] != '.')
	{
		CharIndex ++;
	}
	
	strFileName[CharIndex+1] = strExName[0];
	strFileName[CharIndex+2] = strExName[1];
	strFileName[CharIndex+3] = strExName[2];
	strFileName[CharIndex+4] = '\0';
}

/** �õ�һ��������Ŀ¼�Ĵ��ļ���,��������C:\tmp\1.jpg���������1.jpg
@param fileName:  ������ļ���,��NULL��β,���ҳ��Ȳ�����256
@param pureFileName: �õ��Ĳ�����·�����ļ���
*/
inline VOID _GetPureFileName(const CHAR* fileName, CHAR* pureFileName)
{
	if (!fileName || std::string(fileName).empty())
	{
		pureFileName[0] = '\0';
		return;
	}
	
	int CharCnt = strlen(fileName);  // �ַ�������󳤶�
	
	// �ҵ����һ��б��
	INT CharIndex = CharCnt;
	while (fileName[CharIndex] != '\\'  && fileName[CharIndex] != '/' && CharIndex >= 0)
	{
		CharIndex--;
	}
	//get filename, ����б�ߺ��������
	INT pIndex =0;
	if (CharIndex < CharCnt)
	{
		for (INT m = CharIndex + 1; m <= CharCnt; ++m)
		{
			pureFileName[pIndex] = fileName[m];
			pIndex++;
		}
	}
	
	pureFileName[pIndex] = '\0';
}
inline std::string _GetPureFileNameStr(const std::string &str)
{
	CHAR pureFileName[MAX_FILENAME_LENGTH];
	ZeroMemory(pureFileName, MAX_FILENAME_LENGTH);
	_GetPureFileName(str.c_str(), pureFileName);
	return std::string(pureFileName);
}
/** �õ�һ���������ļ����Ĵ�Ŀ¼����������C:\tmp\1.jpg���������C:\tmp\
@param fileName:  ������ļ���,��NULL��β,���ҳ��Ȳ�����256
@param pureFileName: �õ��Ĳ������ļ����Ĵ�Ŀ¼
*/
inline VOID _GetPureFilePath(const CHAR* fileName, CHAR* pureFilePath)
{
	if (!fileName || std::string(fileName).empty())
	{
		pureFilePath[0] = '\0';
		return;
	}
	
	int CharCnt = strlen(fileName);  // �ַ�������󳤶�
	
	// �ҵ����һ��б��
	INT CharIndex = CharCnt;
	while (fileName[CharIndex] != '\\'  && fileName[CharIndex] != '/' && CharIndex >= 0)
	{
		CharIndex--;
	}
	// �õ�б�߼�ǰ�������
	memcpy(pureFilePath, fileName, CharIndex + 1);	
	
	pureFilePath[CharIndex + 1] = '\0';
}



// �õ��ļ����ƣ���������չ��
inline std::string _GetPureFileNameStrWithoutExtention(const std::string &str)
{
	std::string sFileName = _GetPureFileNameStr(str);
	if (!sFileName.empty())
	{	
		basic_string <char>::size_type idx = sFileName.find_last_of(".", sFileName.length());
		if (idx != string::npos && idx < sFileName.length() - 1)
		{
			sFileName = sFileName.substr(0, idx);
		}	
	}
	return sFileName;
}



inline std::string _GetPureFilePathStr(const std::string &str)
{
	CHAR pureFilePath[MAX_FILENAME_LENGTH];
	ZeroMemory(pureFilePath, MAX_FILENAME_LENGTH);
	_GetPureFilePath(str.c_str(), pureFilePath);
	return std::string(pureFilePath);
}



//-------------------------------------------------------------------------------------------
//������: GetAppName()
//��  ��: CHAR* AppName, CHAR* fileName
//��  ;: �õ�Ӧ�ó�����
//-------------------------------------------------------------------------------------------
inline void GetAppName(CHAR* AppName, CHAR* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != '\0')
	{
		CharCnt++;
	}
	//�ҵ����һ��б�ܵ�λ��
	UINT CharIndex = CharCnt;
	while (fileName[CharIndex] != '\\')
	{
		CharIndex--;
	}
	//�ҵ�.��λ��
	UINT DotIndex = CharCnt;
	while (fileName[DotIndex] != '.')
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
	AppName[pIndex] = '\0';
}

//-------------------------------------------------------------------------------------------
//������: GetRotationVecByRT()
//��  ��: D3DXMATRIX *RosMat, D3DXVECTOR3 *RosVec
//��  ;: ������ת����õ��Ƹ������ת��
//-------------------------------------------------------------------------------------------
// �ɺ����ѱ��º������
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
	// fuxb 2007-9-20 ��asin�Ĳ������Ϸ���� ==>
	// FLOAT fAngleZ = (FLOAT)asin(RosMat->_12);
	FLOAT fTemp = (FLOAT)max(min(RosMat->_12, 1.0), -1.0); // asin�Ķ�������[-1.0, 1.0]
	FLOAT fAngleZ = (FLOAT)asin(fTemp);
	// <==

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

	// fuxb 2007-9-20 ��accos�Ĳ������Ϸ���� ==>
	// FLOAT fAngleX = (FLOAT)acos(D3DXVec3Dot(&Z_2, &Z_1));
    FLOAT ParaValue = D3DXVec3Dot(&Z_2, &Z_1);
	ParaValue = (FLOAT)max(min(ParaValue, 1.0), -1.0); // acos�Ķ�������[-1.0, 1.0]
	FLOAT fAngleX = (FLOAT)acos(ParaValue);
	// <==
	
	RosVec->x = fAngleX;
	RosVec->y = fAngleY;
	RosVec->z = fAngleZ;
}

/*//���º���
inline VOID GetRotationVecByRT(D3DXMATRIX *RosMat, D3DXVECTOR3 *RosVec)
{
	//Edit by Baojiahui 07/9/22
	//ͨ����ת��ת����Ĺ�ʽ��������Ƕ�
	FLOAT fAngleX = 0.0f;  //pitch rotation
    FLOAT fAngleY = 0.0f;  //heading rotation
	FLOAT fAngleZ = 0.0f;  //bank rotation

	//��ת�����ŷ���Ƕ�֮��Ĺ�ϵ
	
    //cos(y)*cos(z)+sin(y)*sin(x)*sin(z)    -cos(y)*sin(z)+sin(y)*sin(x)*cos(z)    sin(y)*cos(x)
	//          sin(z)*cos(x)                         cos(z)*cos(x)                   -sin(x)
    //-sin(y)*cos(z)+cos(y)*sin(x)*sin(z)   sin(z)*sin(y)+cos(y)*sin(x)*cos(z)     cos(y)*cos(x)
    

	FLOAT sp = RosMat->_23;
	//Caculate the x rotation
	if (sp <= -1.0f)
	{
	   fAngleX = -PI_2;
	}
	else if (sp >= 1.0f)
	{
	  fAngleX = PI_2;
	}
	else
	{
		fAngleX = (FLOAT)asin(RosMat->_23);
	}

   //��xΪ����90��ʱ���������������ת��z��ó�����Ϊ������Զ�Ǵ�ֱ��z�ᣬ��������z���ת��Ϊ0��
   if (sp > 0.9999f) 
   {
	    fAngleZ = 0.0f;
        fAngleY = (FLOAT)atan2(-RosMat->_31, RosMat->_11);
   }
   else
   {
       fAngleY = (FLOAT)atan2(RosMat->_13, RosMat->_33);

       fAngleZ = (FLOAT)atan2(RosMat->_21, RosMat->_22);
   }

	RosVec->x = fAngleX;
	RosVec->y = fAngleY;
	RosVec->z = fAngleZ;
}*/

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

//С����������ȡ����
inline INT ToInt1(FLOAT value)
{
    return (INT)((value + 0.5)*10/10.0f);
}

inline bool IsFileExists(CHAR *pFileName)
{
	bool bResult = true;

	HANDLE	hFile = NULL;	

	// ����CreateFile����ļ��Ƿ����
	hFile= CreateFile((pFileName),	
		              GENERIC_READ,
					  FILE_SHARE_READ,
					  NULL,
					  OPEN_EXISTING, //�ж��ļ��Ƿ����		
					  FILE_ATTRIBUTE_NORMAL,
					  NULL);
	
	DWORD dwErr = GetLastError();
	if (dwErr == ERROR_FILE_NOT_FOUND || dwErr == ERROR_PATH_NOT_FOUND) // �����ǲ�����Ϊ�ļ������ڵ��µĴ���
	{
		bResult = false;
	}

	if (NULL != hFile)
	{
		CloseHandle(hFile); // �ر��ļ�
	}

	return bResult;

}

// ��ȡ�����᷽��
inline VECTOR3 _GetAxisDirection(VGS_AXIS_TYPE axis)
{
	switch (axis)
	{
	case VGS_LAXIS_X:
	case VGS_WAXIS_X:
	case VGS_PAXIS_X:
		return VECTOR3(1, 0, 0);
	case VGS_LAXIS_Y:
	case VGS_WAXIS_Y:
	case VGS_PAXIS_Y:
		return VECTOR3(0, 1, 0);
	case VGS_LAXIS_Z:
	case VGS_WAXIS_Z:
	case VGS_PAXIS_Z:
		return VECTOR3(0, 0, 1);
	case VGS_LAXIS_NEGATIVEX:
	case VGS_WAXIS_NEGATIVEX:
	case VGS_PAXIS_NEGATIVEX:
		return VECTOR3(-1, 0, 0);
	case VGS_LAXIS_NEGATIVEY:  // -y
	case VGS_WAXIS_NEGATIVEY:
	case VGS_PAXIS_NEGATIVEY:
		return VECTOR3(0, -1, 0);
	case VGS_LAXIS_NEGATIVEZ: // -z
	case VGS_WAXIS_NEGATIVEZ:
	case VGS_PAXIS_NEGATIVEZ:
		return VECTOR3(0, 0, -1);
	case VGS_AXIS_TYPE_NONE:
		return VECTOR3(0, 0, 0);
	default:
		return VECTOR3(0, 0, 0);
	}
}

// �õ�����
template <class T>
T _GetHandleByName (std::vector<T> &vValue, const std::string &sName)
{
	std::vector<T>::iterator it = vValue.begin();
	for ( ; it != vValue.end(); ++it)
	{
		std::string name = (*it)->getName();
		if (strcmp(sName.c_str(), name.c_str()) == 0)
		{
			return (T)(*it);
		}
	}
	return 0;
}

template <class T>
T _GetHandleByIndex (std::vector<T> &vValue, INT index)
{
	if (index >= 0 && index < (INT)vValue.size())
	{
		return vValue[index];
	}
	return 0;
}

template <class T>
std::string _GetNameByIndex(std::vector<T> &vValue, INT index)
{
	if (index >= 0 && index < (INT)vValue.size())
	{
		T v = vValue[index];
		return v->getName();
	}
	return "";
}

template <class T>
VOID _DeleteByHandle(std::vector<T> &vValue, T v)
{
	std::vector<T>::iterator it = vValue.begin();
	for ( ; it != vValue.end(); ++it)
	{
		if ((*it) == v)
		{
			vValue.erase(it);			
			SAFE_RELEASE(v);
			SAFE_DELETE(v);
			break;
		}
	}
}
template <class T>
VOID _DeleteByIndex(std::vector<T> &vValue, INT index)
{
	if (index >= 0 && index < vValue.size())
	{
		// vValue.erase(vValue.begin() + index);
		_DeleteByHandle(vValue[index]);
	}
}

template <class T>
INT _GetIndexByHandle(std::vector<T> &vValue, T v)
{
	int i = 0;
	std::vector<T>::iterator it = vValue.begin();
	for ( ; it != vValue.end(); ++it)
	{
		if ((*it) == v)
		{
			return i;
		}
		++i;
	}
	return -1;
}
template <class T>
INT _GetIndexByName(std::vector<T> &vValue, const std::string sName)
{
	int i = 0;
	std::vector<T>::iterator it = vValue.begin();
	for (; it != vValue.end(); ++it, ++i)
	{
		std::string name = (*it)->getName();
		if (strcmp(sName.c_str(), name.c_str()) == 0)
		{
			return i;
		}
	}
	return -1;
}

template <class T>
BOOL _IsExist(std::vector<T> &vValue, T v)
{
	return (std::find(vValue.begin(), vValue.end(), v)  != vValue.end());

	/*std::vector<T>::iterator it = vValue.begin();
	for ( ; it != vValue.end(); ++it)
	{
		if ((*it) == v)
		{
			return TRUE;
		}
	}
	return FALSE;*/
}


//////////////// ��ɫ
inline VGSRGB _GetRGBColor(const D3DCOLORVALUE &color)
{
	VGSRGB re;
	re.red   = (BYTE)(color.r * 255 + 0.5);
	re.green = (BYTE)(color.g * 255 + 0.5);
	re.blue  = (BYTE)(color.b * 255 + 0.5);
	return re;	
}

inline D3DCOLORVALUE _GetD3DColor(const VGSRGB &color)
{
	D3DCOLORVALUE re;
	ZeroMemory(&re, sizeof(D3DCOLORVALUE));
	re.r = color.red / 255.0f;
	re.g = color.green / 255.0f;
	re.b = color.blue / 255.0f;
	re.a = 1.0f;
	return re;
}

inline VECTOR3 _GetVECTOR3(D3DXVECTOR3* d3dVector)
{
	VECTOR3 v3;
	v3.x = d3dVector->x;
	v3.y = d3dVector->y;
	v3.z = d3dVector->z;
	return v3;
}

inline D3DXVECTOR3 _GetD3DVECTOR3(const VECTOR3 &v3)
{
	D3DXVECTOR3 d3dv3;
	d3dv3.x = v3.x;
	d3dv3.y = v3.y;
	d3dv3.z = v3.z;
	return d3dv3;
}

//-------------------------------------------------------------------------------------------
//������: GetLocalFilePath()
//��  ��: CHAR* filePath, CHAR* fileName
//��  ;: ���ݱ����ļ����õ��ļ�·��
//-------------------------------------------------------------------------------------------
inline void GetLocalFilePath(char* filePath, const char* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != '\0')
	{
		CharCnt++;
	}
	//�ҵ����һ��б�ܵ�λ��
	UINT CharIndex = CharCnt;
	while (fileName[CharIndex] != '\\' && fileName[CharIndex] != '/' && CharIndex >= 0)
	{
		CharIndex--;
	}

	//get path
	for (UINT m = 0; m < CharIndex; m++)
	{
		filePath[m] = fileName[m];
		if (filePath[m] == '/')
			filePath[m] = '\\';
	}
	filePath[m] = '\0';
}


//////////// d3d //////////////

// ��VGS�Ļ�ɫģʽת��ΪD3D�Ļ�ɫģʽ
inline D3DTEXTUREOP _getD3DBlendOperation(const VGS_BlenderOP& OP)
{	
	switch (OP)
	{
	case VBO_ADD:
		return D3DTOP_ADD;

	case VBO_ADD_SIGNED:
		return D3DTOP_ADDSIGNED;

	case VBO_ADD_SMOOTH:
		return D3DTOP_ADDSMOOTH;

	case VBO_BLEND_CURRENT_ALPHA:
		return D3DTOP_BLENDCURRENTALPHA;

	case VBO_BLEND_DIFFUSE_ALPHA:
		return D3DTOP_BLENDDIFFUSEALPHA;

	//case VBO_BLEND_DIFFUSE_COLOUR:
	//	return D3DTOP_MODULATE/*D3DTOP_SELECTARG1*/;

	case VBO_BLEND_MANUAL:
		return D3DTOP_BLENDFACTORALPHA;

	case VBO_BLEND_TEXTURE_ALPHA:
		return D3DTOP_BLENDTEXTUREALPHA;

	case VBO_DISABLE:
		return D3DTOP_DISABLE;

	case VBO_DOTPRODUCT:
		return D3DTOP_DOTPRODUCT3;

	case VBO_MODULATE:
		return D3DTOP_MODULATE;

	case VBO_MODULATE2:
		return D3DTOP_MODULATE2X;

	case VBO_MODULATE4:
		return D3DTOP_MODULATE4X;

	case VBO_REPLACE:
		return D3DTOP_SELECTARG1;

	case VBO_SUBTRACT:
		return D3DTOP_SUBTRACT;
	default:
		return	D3DTOP_MODULATE;
	}

}

inline VGS_BlenderOP _getVGSBlenderOperation(const D3DTEXTUREOP &op)
{	
	switch (op)
	{
	case D3DTOP_ADD:
		return VBO_ADD;		
	case D3DTOP_ADDSIGNED:
		return VBO_ADD_SIGNED;		
	case D3DTOP_ADDSMOOTH:
		return VBO_ADD_SMOOTH;		
	case D3DTOP_BLENDCURRENTALPHA:
		return VBO_BLEND_CURRENT_ALPHA;		
	case D3DTOP_BLENDDIFFUSEALPHA:
		return VBO_BLEND_DIFFUSE_ALPHA;		
	case D3DTOP_SELECTARG1:
		return VBO_REPLACE;		
	case D3DTOP_BLENDFACTORALPHA:
		return VBO_BLEND_MANUAL;		
	case D3DTOP_BLENDTEXTUREALPHA:
		return VBO_BLEND_TEXTURE_ALPHA;		
	case D3DTOP_DISABLE:		
		return VBO_DISABLE;		
	case D3DTOP_DOTPRODUCT3:
		return VBO_DOTPRODUCT;		
	case D3DTOP_MODULATE:
		return VBO_MODULATE;		
	case D3DTOP_MODULATE2X:
		return VBO_MODULATE2;		
	case D3DTOP_MODULATE4X:
		return VBO_MODULATE4;		
	case D3DTOP_SELECTARG2:
		return VBO_DISABLE;		
	case D3DTOP_SUBTRACT:
		return VBO_SUBTRACT;
	default:
		return VBO_MODULATE;
	}
}

// ����ת�����ַ���
inline std::string _InttoString(INT i)
{
	CHAR tmp[10];
	if (0 == _itoa_s(i, tmp, sizeof(tmp), 10))
	{
		return std::string(tmp);
	}
	return "";
}

// �ж�һ���ļ��Ƿ����
inline BOOL _FileExist(const std::string &sFileName)
{
	FILE *pFile;
	fopen_s(&pFile, sFileName.c_str(), "rb");
	if (pFile)
	{
		fclose(pFile);
		return TRUE;
	}
	return FALSE;
}


// �ҵ�vector��ĳ��Ԫ�ص�����
template <typename T>
int _GetIndexInVector(const std::vector<T> &vVector, const T &value)
{
	int count = vVector.size();
	
	for (int i = 0; i < count; ++i)
	{
		if (vVector[i] == value)
		{
			return i;
		}
	}
	
	return -1;
}

// ���б���ɾ��һ��Ԫ��
template <typename T>
void _DeleteFromVector(std::vector<T> &vVector, const T &value)
{
	std::vector<T>::iterator it = std::find(vVector.begin(), vVector.end(), value);
	if (it != vVector.end())
	{
		vVector.erase(it);
	}
}


//////////////////////////// d3d ////////////////////////////////////

// ��Texture�е�ͼ�񱣴浽�ļ�
inline void _SaveImageFromTexture(LPDIRECT3DTEXTURE9 pTex, std::string sFileName)
{
	if (pTex)
	{
		LPDIRECT3DSURFACE9 pDestTexSurface;
		HRESULT hr = pTex->GetSurfaceLevel(0, &pDestTexSurface);
		D3DXSaveSurfaceToFile((sFileName.c_str()), D3DXIFF_JPG, pDestTexSurface, NULL, NULL);
		SAFE_RELEASE_D3D(pDestTexSurface);
	}
}

#endif
