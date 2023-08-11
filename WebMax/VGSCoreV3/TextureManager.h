
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

	// ��һ��ͼ���ļ�������һ��texture����
	CTexture* createTexture(const std::string &sImageFile, BOOL bAlphaOnly = FALSE, INT mipMap = DEFAULT_MIPMAP, /*D3DFORMAT format = D3DFMT_UNKNOWN, */const std::string &sTextureName = "");
	
	CTexture* createEmptyTexture(const std::string &sName, D3DFORMAT format, INT width, INT height, INT mipmap);

	// ��һ��ͼ���ļ�buffer������һ��texture����
	// CResource* createTextureFromBuffer(const std::string &sTextureName, const VOID* pBuffer, BOOL bAlphaOnly/* = FALSE*/, INT mipMap/* = DEFAULT_MIPMAP*/)

	BOOL createTextureFromBuffer(CTexture *pTexture, char* buffer, DWORD len, BOOL bAlphaOnly = FALSE);

	bool IsTextureExist(const std::string &sName);

	/** ����һ�㶯̬��ͼ
	@param strName in:��ͼ����
	@param vecFileName in:����ͼ�������б�
	@param nDuration in:��������
	@param nUVidx in:UV����
	@return:CTextureLayer����ָ��.
	*/
	CDynamicTex* CreateDynamicTexture(const std::string& strName, const std::vector<std::string>& vecFileName, float fDuration = 1.0f);

	CCubeMap* CreateStaticCubeTexture(const std::string& strName, const vector<std::string> &vcFileName, int nSize);

	CCubeMap* CreateDynamicCubeTexture(const std::string& strName, int nSize);

	CBumpMap* CreateBumpTexture(const std::string& strName);

	CRTFMap* CreateRTreflectTexture(const std::string& strName,int nWidth, int nHeight);


	/** ����һ��FlashTexture.
	@param pTxtrName in:Ҫ��������ͼ���ƣ����ɿ�.
	@param strFlashPath in: Flash�ļ���·������.
	@param flashWidth in: Flash�Ŀ��.
	@param flashHeight in: Flash�߶�.
	@param nWidth in:Ҫ��������ͼ�Ŀ��.
	@param nHeight in:Ҫ��������ͼ�ĸ߶�.
	@return:������ͼ.
	*/
	SwfTexture* CreateFlashTexture(const std::string& strName, const std::string& strFlashPath,
		int flashWidth, int flashHeight, int nWidth, int nHeight);
protected:
	CVGSRender* mVGSRender;
	// std::map<std::string, CTexture*> m_vTextureMap;
};