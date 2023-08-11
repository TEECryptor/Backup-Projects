
#pragma once

#include "VGSHeader.h"
#include "ResourceManager.h"
//#include "VGSGetRoot.h"
#include <map>

class VGS_EXPORT CTextureManager : public CResourceManager //, public CVGSGetRoot, public Singleton<CTextureManager>
{
public:
	CTextureManager();
	~CTextureManager();
	VOID Release();
	void setVGSRender(CVGSRender*);

	// 由一个图像文件，创建一个texture对象
	CTexture* createTexture(const std::string &sImageFile, BOOL bAlphaOnly = FALSE, INT mipMap = DEFAULT_MIPMAP, /*D3DFORMAT format = D3DFMT_UNKNOWN, */const std::string &sTextureName = "");
	
	CTexture* createEmptyTexture(const std::string &sName, D3DFORMAT format, INT width, INT height, INT mipmap);

	// 由一个图像文件buffer，创建一个texture对象
	// CResource* createTextureFromBuffer(const std::string &sTextureName, const VOID* pBuffer, BOOL bAlphaOnly/* = FALSE*/, INT mipMap/* = DEFAULT_MIPMAP*/)

	BOOL createTextureFromBuffer(CTexture *pTexture, char* buffer, DWORD len, BOOL bAlphaOnly = FALSE);

	bool IsTextureExist(const std::string &sName);

	/** 增加一层动态贴图
	@param strName in:贴图名称
	@param vecFileName in:所有图像名称列表
	@param nDuration in:播放周期
	@param nUVidx in:UV索引
	@return:CTextureLayer对象指针.
	*/
	CDynamicTex* CreateDynamicTexture(const std::string& strName, const std::vector<std::string>& vecFileName, float fDuration = 1.0f);

	CCubeMap* CreateStaticCubeTexture(const std::string& strName, const vector<std::string> &vcFileName, int nSize);

	CCubeMap* CreateDynamicCubeTexture(const std::string& strName, int nSize);

	CBumpMap* CreateBumpTexture(const std::string& strName);

	CRTFMap* CreateRTreflectTexture(const std::string& strName,int nWidth, int nHeight);


	/** 创建一个FlashTexture.
	@param pTxtrName in:要创建的贴图名称，不可空.
	@param strFlashPath in: Flash文件的路径名称.
	@param flashWidth in: Flash的宽度.
	@param flashHeight in: Flash高度.
	@param nWidth in:要创建的贴图的宽度.
	@param nHeight in:要创建的贴图的高度.
	@return:返回贴图.
	*/
	SwfTexture* CreateFlashTexture(const std::string& strName, const std::string& strFlashPath,
		int flashWidth, int flashHeight, int nWidth, int nHeight);
protected:
	CVGSRender* mVGSRender;
	// std::map<std::string, CTexture*> m_vTextureMap;
};