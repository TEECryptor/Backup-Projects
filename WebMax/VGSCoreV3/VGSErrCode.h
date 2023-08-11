/*********************************************************************
* ��Ȩ���д�ͼ����Ƽ���չ���޹�˾��
* 
* �ļ����ƣ� // VGSErrCode.h
* ����ժҪ�� // VGS�õ��Ĵ����뼰�����������弰��ط�������
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


#pragma once
#include <map>
#include <windows.h>
#include <string>

#include "VGSErrCodeDef.h"


/// ������VGS������ʹ�õĴ�����

using namespace std;

static DWORD g_LastErrorCode;  // ���һ�δ������

class CVGSErrCode
{
public:
	static UINT GetLastErrorCode();
	static string GetLastErrorStr();
	static string GetErrorStr(UINT ErrCode);
	CVGSErrCode();
private:
	static map<UINT, std::string> m_vErrInfo;

};

