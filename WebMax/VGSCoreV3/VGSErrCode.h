/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // VGSErrCode.h
* 内容摘要： // VGS用到的错误码及错误描述定义及相关方法声明
* 其它说明： // 
* 当前版本： // V2.0
* 作    者： // 傅新波
* 完成日期： // 
* 
* 修改日期      版本号     修改人	      修改内容
* ---------------------------------------------------------------
* 
*
*********************************************************************/


#pragma once
#include <map>
#include <windows.h>
#include <string>

#include "VGSErrCodeDef.h"


/// 定义了VGS引擎中使用的错误码

using namespace std;

static DWORD g_LastErrorCode;  // 最近一次错误代码

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

