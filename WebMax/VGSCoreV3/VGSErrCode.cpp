/*********************************************************************
* 版权所有创图网络科技发展有限公司。
* 
* 文件名称： // VGSErrCode.h
* 内容摘要： // 获取错误码及错误描述的相关方法
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

	//////////////// 公共 ///////////////////////////////
	m_vErrInfo[ERR_COMMON_OK]           = "OK";	
	m_vErrInfo[ERR_COMMON_UNKNOWN]      = "无法获取错误信息！";	

    //////////////// 文件相关 ////////////////////////////
	
	m_vErrInfo[ERR_FILE_V3D_NEWVERSION]     = "V3D文件格式太新，无法读取！";
	m_vErrInfo[ERR_FILE_V3D_WRONGFORMAT]    = "V3D文件格式不正确，无法读取！";
	m_vErrInfo[ERR_FILE_V3D_READHEAD]       = "读V3D文件头发生错误，无法读取文件";
	m_vErrInfo[ERR_FILE_V3D_READHEAD]       = "读V3D文件头发生错误，无法读取文件";
	m_vErrInfo[ERR_FILE_V3D_OLDVERSION]     = "V3D文件低于当前引擎支持的版本";
	m_vErrInfo[ERR_FILE_OPENFAIL]			= "读文件发生错误";
	m_vErrInfo[ERR_FILE_READBYTEFAIL]		= "读文件时未读取预订的字节数目";
	m_vErrInfo[ERR_FILE_V3D_NODATA]		    = "读文件时未读到数据";


	m_vErrInfo[ERR_FILE_SAVEV3D_NODATA]    	= "保存V3D时，读取场景数据错误！";
	m_vErrInfo[ERR_FILE_SAVEV3D_OPENFILE] 	= "保存V3D时，打开文件错误！";
	m_vErrInfo[ERR_FILE_SAVEV3D_FAIL] 		= "保存V3D发生错误！";

	m_vErrInfo[ERR_FILE_SAVEV3D_HEAD]    			= "保存V3D时，写入文件头发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_TABLE]  			= "保存V3D时，写入目录表发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_SCENEPROPERTY]  	= "保存V3D时，写入场景信息发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_ENTITY]    			= "保存V3D时，写入模型发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_MESH]  				= "保存V3D时，写入模型数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_MATERIAL]  			= "保存V3D时，写入材质数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_CAMERA]    			= "保存V3D时，写入相机数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_LIGHT]  			= "保存V3D时，写入灯光数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_SCENEGRAPH]  		= "保存V3D时，写入场景图数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_OVERLAY]    		= "保存V3D时，写入前景图数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_BUTTON]  			= "保存V3D时，写入按钮数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_NAVIGATOR]  		= "保存V3D时，写入导航图数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_AUDIO]    			= "保存V3D时，写入声音数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_CONTORLLER]  		= "保存V3D时，写入控制对象数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_ANNIMATION] 		= "保存V3D时，写入动画数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_BACKDROP]    		= "保存V3D时，写入背景图数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_TEXT]  				= "保存V3D时，写入文字数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_TEXTURE]			= "保存V3D时，写入纹理数据发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_CLOSEFILE]			= "保存V3D时，关闭文件时发生错误";
	m_vErrInfo[ERR_FILE_SAVEV3D_FALSH]				= "保存V3D时，写入Flash时发生错误";

	m_vErrInfo[ERR_FILE_N3D_SAVE]		    = "保存N3D发生错误！";
	m_vErrInfo[ERR_FILE_N3D_LOAD]           = "加载N3D法身错误！";
	m_vErrInfo[ERR_FILE_N3D_SCENELOAD]      = "导入N3D场景发生错误！";
	m_vErrInfo[ERR_FILE_N3D_IMAGELOAD]      = "导入N3D贴图发生错误！";
	m_vErrInfo[ERR_FILE_N3D_VERSIONLOW]     = "N3D版本太低！";
	m_vErrInfo[ERR_FILE_N3D_VERSIONHIGH]    = "N3D版本太高！";

	m_vErrInfo[ERR_FILE_E3D_SAVE]		    = "保存E3D发生错误！";
	m_vErrInfo[ERR_FILE_E3D_LOAD]		    = "加载E3D发生错误！";
	m_vErrInfo[ERR_FILE_E3D_LOWVER]         = "E3D文件版本过低！";
	m_vErrInfo[ERR_FILE_E3D_FORMAT]         = "E3D文件格式不正确！";


	m_vErrInfo[ERR_SCENE_NOSCENE]       = "无法获取场景！";
	m_vErrInfo[ERR_SCENE_CREATE]        = "创建对象失败！";

    //////////////// 模型相关 ////////////////////////////

	//////////////// 相机相关 ////////////////////////////


	//////////////// 灯光相关 ////////////////////////////

	//////////////// 材质相关 ////////////////////////////
}