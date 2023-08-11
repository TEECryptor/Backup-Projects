/////////////////////////////////////////////////////////////////////////
// ����: CMaterial���Ͷ���--�����еĲ��ʶ���
// ����: CMaterial���Ͷ����˲��ʵĸ�������
// ����: Lixin	
// ����: 2006-04-30
////////////////////////////////////////////////////////////////////////

#ifndef _CMATERIAL_H
#define _CMATERIAL_H

#include "VGSHeader.h"
#include "CTexture.h"
#include "CSysFunc.h"
#include "BumpMap.h"
#include "CCubeMap.h"
#include "CDynamicTex.h"
#include "RTFMap.h"
#include ".\fileio\N3D_def.h"
//#include "ResourceManager.h"
//#include "TextureManager.h"

#define TWO_SIDED           0x01
#define REF_TEXTURE_MAPPING 0x02
#define NO_TEXTURE          0x04
#define FLAT_SHADING		0x0000
#define GOURAUD_SHADING     0x08
#define PHONG_SHADING       0x20


#include <vector>
#include <set>
using std::vector;
using namespace N3D;

//Add by Baojiahui ----------------------------------------------------------------------------------
//change the stucture in order to group the mtrl by submesh 
class VGS_EXPORT VGS_RenderSubmesh
{
public:
	VGS_RenderSubmesh(){modelPointer = subMeshID = 0;}
	DWORD			modelPointer;					//subMesh���ڵ�modelָ��
	int			subMeshID;						//subMesh��������
	inline friend bool operator < (const VGS_RenderSubmesh& lv, const VGS_RenderSubmesh& rv)
	{
		return lv.modelPointer < rv.modelPointer && lv.subMeshID < rv.subMeshID;
	}
	inline friend bool operator == (const VGS_RenderSubmesh& lv, const VGS_RenderSubmesh& rv)
	{
		return lv.modelPointer == rv.modelPointer && lv.subMeshID == rv.subMeshID;
	}
};

typedef struct _BlenderInfo
{
	D3DTEXTUREOP			ColorBlendOption;       // ��ɫ��ɫ��ʽ
	D3DCOLOR 		        ColorBlendParam;        // ��ɫ��ɫ����
	
	D3DTEXTUREOP			AlphaBlendOption;       // alpha��ɫ��ʽ
	D3DCOLOR 		        AlphaBlendParam;        // alpha��ɫ����
	

	_BlenderInfo()
	{
		ColorBlendOption = D3DTOP_MODULATE;
		ColorBlendParam = 0;
		AlphaBlendOption = D3DTOP_MODULATE;
		AlphaBlendParam = 0;
	}
}_BlenderInfo;

typedef struct TextureLayer_old
{
	CHAR					Name[N3D_MAX_NAME_LENGTH];     // ��ͼ����
	
	VGS_TextureType         type;                          // ��ͼ����
	int						UVIndex;                       // ʹ�õ�UV����
	_BlenderInfo            Blenderinfo;                   // ��ɫ����
	DWORD					pTexture;
}TextureLayer_old;

class VGS_EXPORT CTextureLayer : public CVGSGetRoot
{
public:
	std::string             m_sName;
	VGS_TextureType         type;                          // ��ͼ����
	N3D_TextureParam        param;                         // texture ����
	int						UVIndex;                       // ʹ�õ�UV����
	_BlenderInfo            Blenderinfo;                   // ��ɫ����
	float                   USpeed;
	float                   VSpeed;
	CTexture*               pTexture;
	INT                     mipMap;

	VGS_REFLECT_TYPE        m_refletionType;  //  ��type == TLT_REFLECTʱ����ʾ���䷽ʽ

	UINT getFrameCount();
	std::string getFrameFileName(INT Index);
	//add by Nick, 2008��12��13�գ�����������ͼ��ı任
	D3DTEXTURETRANSFORMFLAGS	transformFlag;
	CTextureLayer()
	{
	  USpeed = 0;
	  VSpeed = 0;
	  UVIndex = 0;
	  pTexture = NULL; //��ֹҰָ��
	}
	~CTextureLayer()
	{
		// CTextureManager::getSingleton().removeResource(pTexture);
// 		SAFE_DELETE(pTexture);
	}

private:
	
};


class VGS_EXPORT CMaterial : public CResource
{
public:
	INT					index;						//��������Ⱦ�����еı��
	D3DMATERIAL9		d3dmtrl;					//��������
 
	D3DCOLORVALUE		specular;					//�߹�ɫ
	FLOAT				specularLevel;				//�߹�ǿ��	
	
	//UINT				opacity;					//͸����
	//INT					oriOpacity;					//ԭʼ͸����
	INT					type;						//��������
	BOOL				twoSide;					//�Ƿ�Ϊ˫�����
	_D3DFILLMODE		fillMode;					//���ģʽ

	BOOL				bShowTexture;				//�Ƿ���ʾ��ͼ

	BOOL                bReadyToRender;                  //��ͼ�Ƿ��Ѿ��������

	// BOOL                bUseDiffuse;                //�Ƿ�ʹ��ģ�͵�diffuse��ɫ

	UINT				selfIllumination;			//�Է���


	UINT				m_FVF;	
	UINT				m_FVFSize;	

	vector<VGS_RenderSubmesh>  subMeshList;				//ʹ��������ʵ�SubMesh�б�

	//�Ƿ�ʹ��AlphaTest��ʽ(��ʹ�û�ɫ��ʹ��ALPHA����)����Ⱦʹ�øò��ʵ�ģ��
	BOOL				bAlphaTest;	
	BOOL				bAlphaBlend;				//�Ƿ�ʹ��Alpha��ɫ

	INT                 AlpahaTestValue;            // AlphaTest������ֵ��С�ڸ����޵ĵ㽫�ᱻ�޳�

	BOOL				bIsOverlayMtrl;				//�Ƿ���Overlay����

	BOOL				bCulled;					//�ò��ʵ�����ģ�Ͷ����Ƿ񶼱�����

	vector<CTextureLayer*>    m_vpTexLayers;     //���ʲ��б�

	INT					bumpWaterIndex;           //bumpwater���� �����кţ�����bumpwater��Ϊ -1

	BOOL                m_bUseDiffuse;            // �Ƿ�ʹ��diffuse��ɫ
	D3DXMATRIX			m_transformMat[8];			//��ͼ�ı任����

	/** ���ʶ�Ӧ�ĵ�ǰģ��.��ǰ��Ҫ���ڷ�����ͼ��Ⱦʱ�ƹⷽ��ļ���.
	*/
	CModel*				m_pCurModel;
public:
		
	VOID				Release();

	CMaterial();
	~CMaterial();

	BOOL                IsTransparent();        // �Ƿ�͸��

	BOOL                IsReadyToRender();      // ��ͼ�Ƿ��Ѿ��������

	std::string getName();
	VOID setName(const std::string &sName);

	UINT getTexureLayerCount();

	// texture����غ���
	CTextureLayer* getTextureLayerByIndex(INT index);
	VOID clearTextureLayerByIndex(size_t index);
	VOID clearTextureLayerByHandle(CTextureLayer *pLayer);

	// ����һ����ͨ��ͼ
	CTextureLayer* createTextureLayer(const std::string &sImageFileName, std::string &sTextureName, INT uvIndex, size_t nIndex = 100000);
	
	// ����һ��͸����ͼ
	CTextureLayer* createOpacityTextureLayer(const std::string &sImageFileName, std::string &sTextureName, INT uvIndex, size_t nIndex = 100000);
	
	/** ����һ�㶯̬��ͼ
	@param szTextureName in:��ͼ����
	@param vecFileName in:����ͼ�������б�
	@param nDuration in:��������
	@param nUVidx in:UV����
	@return:CTextureLayer����ָ��.
	*/
	CTextureLayer* CreateDynamicTextureLayer(const char* szTextureName, const std::vector<std::string>& vecFileName, float fDuration, int nUVidx, size_t nIndex = 100000);

	// ������̬cubemap
	CTextureLayer* createStaticCubemapTextureLayer(const CHAR* pTextureName, const vector<std::string> &vsFileName, int nSize, size_t nIndex = 100000);

	// ������̬cubemap
	CTextureLayer* createDynamicCubemapTextureLayer(const CHAR* pTextureName, int nSize, size_t nIndex = 100000);

	// ����bump��ͼ
	CTextureLayer* createBumpTextureLayer(const CHAR* pTextureName/*, const CHAR* pTextureFileName*/, size_t nIndex = 100000);
	/** ����ʵʱ������ͼ��.
	@param strTextureName in:��ͼ����.
	@param nWidth in:��ͼ���.
	@param nHeight in:��ͼ�߶�.
	*/
	CTextureLayer* CreateRTreflectTextureLayer(const std::string& strTextureName, int nWidth, int nHeight, size_t nIndex = 100000);

	CTextureLayer* CreateFlashTextureLayer(const std::string& strTextureName, const std::string& strFlashPath,
		int flashWidth, int flashHeight, int nWidth, int nHeight, size_t nIndex = 100000);

	//����ǰ�������õ��豸
	VOID				SetToDevice(LPDIRECT3DDEVICE9 pd3dDevice, BOOL bRenderBump, CModel* pModel = NULL);

	//���ÿ����ͼͨ�������ݺ�״̬
	VOID				ClearTextureMem(LPDIRECT3DDEVICE9 pd3dDevice);
	//������ͼ��
	VOID				SetTextureLayer(LPDIRECT3DDEVICE9 pd3dDevice, CTextureLayer* pTexLayer, int CurTexStage);
	//���ð�͹��ͼ��
	//˵��: ������ʰ�����͹ˮ����ͼ�����Ƚ�������ͼ���ɫ��������ɫ��Ⱦ�Ľ���밼͹��ɫ
	VOID				SetBumpWaveTextureLayerAndOP(LPDIRECT3DDEVICE9 pd3dDevice, CTextureLayer* pTextureLayer);
	//�ƶ���ͼ���UV
	VOID				MoveTextureUV(LPDIRECT3DDEVICE9 pd3dDevice, int index, float uSpeed, float vSpeed, D3DTEXTURETRANSFORMFLAGS flag);
	//�ж���ͼ�����Ƿ������͹ˮ����ͼ
	CTextureLayer*		IsContainBumpWaveMap();

	// ���ò��ʵ�͸����
	VOID setOpacity(INT Opacity);
	INT getOpacity(){return m_Opacity;};

	// �����Ƿ����͸����ͼ
	BOOL hasTransparentTexture();

	// ��ʹ�ñ����ʵ�submesh��ӵ��б�
	VOID AddSubmeshToList(const VGS_RenderSubmesh& rsm);

private:
	UINT				m_Opacity;					//͸����
	INT					m_OriOpacity;					//ԭʼ͸����
};

#endif
