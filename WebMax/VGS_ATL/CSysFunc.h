/////////////////////////////////////////////////////////////////////////
// 名称: CSysFunc.h
// 描述: 类型定义,包含各种系统内存整理函数
// 作者: Lixin
// 日期: 2006-05-01(2)
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

//自定义类的对象的Release方法只是释放了对象内部创建的内存，自身的释放还需要delete
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { p->Release();} }
#endif

//d3d对象只能使用Release()方法,不能delete
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
  DWORD      triggerHandle;         //触发器的句柄
  CHAR*      callBackFunc;          //回调函数名称
}TriggerCallBack;

//关键帧动画的帧回调函数结构
typedef struct KFRCallBack
{
	INT			frameIndex;				//帧数值
	CHAR*		callBackFunc;			//回调函数名称
}KFRCallBack;

//场景对象的类型
enum 
{
	NODETYPE_NONE,
	NODETYPE_MODEL,
	NODETYPE_CAMERA,
	NODETYPE_LIGHT
};
//得到字符串的长度
//inline DWORD GetStrLength(CHAR* strString)
//{
//	UINT index = 0;
//	while (strString[index] != '\0')
//		index ++;
//	return index;
//}

//把字符串全部转化成大写字母
//inline VOID StrToCaption(CHAR* strString)
//{
//	//先得到字符串的长度
//	UINT index = 0;
//	while (strString[index] != '\0')
//	{
//		if (strString[index] >= 97 && strString[index] < 129)
//		strString[index] = strString[index] - 32;
//		index ++;
//	}
//}

//-------------------------------------------------------------------------------------------
//方法名: GetChar()
//参  数: CHAR *DesString, CHAR* OriString, UUUINT startIndex, UUUINT endIndex
//用  途: 得到字符的指定字段
//-------------------------------------------------------------------------------------------
//inline VOID GetChar(CHAR *DesString, CHAR* OriString, UINT startIndex, UINT endIndex)
//{
//	//得到字符的个数
//	for (UINT i= startIndex; i<= endIndex; i++)
//	{
//		DesString[i- startIndex] = OriString[i];
//	}
//	
//}

//-------------------------------------------------------------------------------------------
//方法名: ChangeFileEx()
//参  数：CHAR* FilePath,原来的文件名，CHAR* strExName 新的扩展名
//用  途：修改完整路径文件的扩展名
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
//方法名: GetFilePath()
//参  数: CHAR* filePath, CHAR* fileName
//用  途: 得到文件路径
//-------------------------------------------------------------------------------------------
inline VOID GetFilePath(CHAR* filePath, CHAR* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != '\0')
	{
		CharCnt++;
	}
	//找到最后一个斜杠的位置
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
//方法名: GetPureFileName()
//参  数: CHAR* pureFileName, CHAR* fileName
//用  途: 得到文件名称
//-------------------------------------------------------------------------------------------
inline VOID GetPureFileName(CHAR* pureFileName, CHAR* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != '\0')
	{
		CharCnt++;
	}
	//找到最后一个斜杠的位置
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
	//找到最后一个斜杠的位置
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
//方法名: GetAppName()
//参  数: CHAR* AppName, CHAR* fileName
//用  途: 得到应用程序名
//-------------------------------------------------------------------------------------------
inline void GetAppName(char* AppName, const char* fileName)
{
	//get char count
	UINT CharCnt = 0;
	while (fileName[CharCnt] != char('\0'))
	{
		CharCnt++;
	}
	//找到最后一个斜杠的位置
	UINT CharIndex = CharCnt;
	while (fileName[CharIndex] != char('\\'))
	{
		CharIndex--;
	}
	//找到.的位置
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
//方法名: GetRotationVecByRT()
//参  数: D3DXMATRIX *RosMat, D3DXVECTOR3 *RosVec
//用  途: 根据旋转矩阵得到绕各个轴的转角
//-------------------------------------------------------------------------------------------
inline VOID GetRotationVecByRT(D3DXMATRIX *RosMat, D3DXVECTOR3 *RosVec)
{
	//这是个比较复杂的问题，实际上是坐标系旋转的问题，总体思路如下：
	//第一步: 通过旋转后的X轴矢量，得到坐标系绕Y轴的转角
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
	//第二步: 仍然通过旋转后的X轴矢量，计算坐标系绕Y轴旋转后再绕自身Z轴的转角
	FLOAT fAngleZ = (FLOAT)asin(RosMat->_12);
	//判断是锐角还是钝角
	if (D3DXVec3Dot(&D3DXVECTOR3(RosMat->_11, RosMat->_12, RosMat->_13), 
					 &D3DXVECTOR3(RosMat->_11, 0, RosMat->_13) ) < 0) 
		fAngleZ = PI - fAngleZ;
	//顺时针转逆时针
	fAngleZ = -fAngleZ;

	//第三步: 计算绕上两步中Y轴旋转后的Z'轴与最终Z"轴的夹角
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
//方法名: GetRTByRotation()
//参  数: D3DXVECTOR3 *Ros, VecD3DXMATRIX *RosMat
//用  途: 根据旋转矩阵得到绕各个轴的转角
//-------------------------------------------------------------------------------------------
inline VOID GetRTByRotationVec(D3DXVECTOR3 *Ros, D3DXMATRIX *RosMat)
{
	//把 GetRotationVecByRT()的方法反过来求
	//先绕Y轴旋转
	D3DXMatrixRotationY(RosMat, -Ros->y);
	//再绕新的Z'轴旋转
	D3DXMATRIX matRosZ;
	D3DXVECTOR3 Z_1 = D3DXVECTOR3((FLOAT)cos(PI_2 + Ros->y), 0, (FLOAT)sin(PI_2 + Ros->y));
	D3DXMatrixRotationAxis(&matRosZ, &Z_1, -Ros->z);
	//再绕新的X'轴旋转
	//------先计算新的X'轴向量, D3D方法中的角度都是顺时针方向
	D3DXMATRIX matRosX;
	D3DXVECTOR3 X_1 = D3DXVECTOR3((FLOAT)cos(Ros->y), (FLOAT)sin(-Ros->z), (FLOAT)sin(Ros->y));
	D3DXMatrixRotationAxis(&matRosX, &X_1, -Ros->x);
	//矩阵相乘
	D3DXMatrixMultiply(RosMat, RosMat, &matRosZ);
	D3DXMatrixMultiply(RosMat, RosMat, &matRosX);
}

// 获取坐标轴方向
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

//  文件地址类型
enum VGS_FILELOCATION_TYPE
{
	VGS_FILELOCATION_TYPE_NONE = 0,
	VGS_FILELOCATION_TYPE_LOCAL,  // 本地文件
	VGS_FILELOCATION_TYPE_WEB     // 网络文件
};

//inline VGS_FILELOCATION_TYPE _getFileLocationType(const std::string sPath)
//{
//	std::string str = sPath;
//	std::transform(str.begin(),str.end(), str.begin(), tolower); // 转换成小写
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

// 得到上一级目录
// str: 当前目录， cFlag ：目录分隔符号，URL为'/'，本地为"\"
inline std::string _getUpperFolder(const std::string &str, const char &cFlag = '/')
{
	int length = str.length();
	if (length == 0)
	{
		return "";
	}

	// 去掉最后一个'\'符号
	std::string sNewStr = str;
	if (sNewStr[length - 1] == cFlag)
	{
		sNewStr[length - 1] = '\0';
		length--;
	}

	// 查找最后一个'\'符号
	int idx = (int)sNewStr.rfind(cFlag);
	if (idx >= 0 && idx < length)
	{
		return sNewStr.substr(0, idx + 1);
	}

	return str;  // 返回原字符串
}

// 是否是web路径
inline bool _isWebPath(const std::string &sPath)
{
	if (   (sPath[0] == 'h' || sPath[0] == 'H') && (sPath[1] == 't' || sPath[1] == 'T')
		&& (sPath[2] == 't' || sPath[2] == 'T') && (sPath[3] == 'p' || sPath[3] == 'P') )
	{
		return true;
	}
	return false;
}

// 是否是本地路径
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

// 获取完整路径
// 1. 如果path本身就是完整URL, 直接使用path路径；
// 2. 如果path没有使用$(UPPER)标识符，则返回sServer+path，否则，则返回sServer的上一级路径加path路径
inline std::string _getFullPath(const std::string &sServer, const std::string &path)
{
	// 	if (   (path[0] == 'h' || path[0] == 'H') && (path[1] == 't' || path[1] == 'T')
	// 		&& (path[2] == 't' || path[2] == 'T') && (path[3] == 'p' || path[3] == 'P') )
	if (_isWebPath(path))  // 如果是完整URL路径
	{
		return path;
	}

	int pathLen = path.length();
	if (pathLen > 2 && path[1] == ':')  // 如果是本地绝对路径
	{
		return path;
	}

	// bool bURL = _isWebPath(sServer);  // 是否URL路径
	// char cFlag = bURL ? '/' : '\\';   // url和本地的目录分隔符不同
	char cFlag = '/';

	// 当前目录
	if (pathLen > 2 && path[0] == '.' && path[1] == '/') 
	{
		std::string strRemainPath = path.substr(2);
		return _getFullPath(sServer, strRemainPath);
	}

	// 上一级目录
	if (pathLen > 3 && path[0] == '.' && path[1] == '.' && path[2] == '/')
	{
		std::string strUpperServer = _getUpperFolder(sServer, cFlag);  // 上一级目录
		std::string strRemainPath = path.substr(3);
		return _getFullPath(strUpperServer, strRemainPath);
	}

	// 返回完整路径
	std::string sFullPaht = sServer;
	return sFullPaht.append(path);
}

// 获得文件的扩展名，小写,例如".jpg"
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
