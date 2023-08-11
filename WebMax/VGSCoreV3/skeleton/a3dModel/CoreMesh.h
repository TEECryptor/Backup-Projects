#ifndef __XBASE_MESH_H__
#define __XBASE_MESH_H__
#include "CD3DInclude.h"
#include "CoreMeshDef.h"
#include "ModelFWDecl.h"
#include "TextureManager.h"
#include "D3D9InputBuffer.h"

class _A3D_API_ CCoreMesh : public CRefCountObject
{
public:
	CCoreMesh(CBaseTextureMgr* pTexMgr, CBaseModel* pModel/* = NULL*/);
	bool                           load(const wchar_t* fileName , unsigned long  arg);
	bool                           load(const wchar_t* fileName , const BYTE* buf , size_t bufLen, unsigned long arg);
	bool                           isLoaded();
	bool                           unload();
	unsigned long                  memUsage();
    const wchar_t*                 name(){ return m_name.c_str() ; }
	void                           ToCpuSkin();
	void                           ToGpuSkin();
    void                           SetShader(const wchar_t* shaderName);
public:
	bool                           load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
    const CBoundBox&               aabb(){return m_aabb ; }

    bool                           load_dir(const wchar_t* _dir , unsigned int arg);
    virtual bool                   draw(CCoreSkeleton* pSkeleton , CCoreActionFrame* pActionFrame, bool bCpuSkin);
	virtual bool                   setCurrentFrame(int iFrame);
    virtual void                   setTexture(int iSubMesh , int iTex , HBaseTexture hTexture) ;
    
 protected:                        
	bool                           loadMaterial(xcomdoc& doc ,const wchar_t* _dir);
	bool                           readSingleFrame(xcomdoc& doc , const wchar_t* _dir , unsigned int arg , bool bInvertVCoord);
    bool                           readAnimationFrame(xcomdoc& doc , const wchar_t* _dir , unsigned int arg , bool bInvertVCoord);
    
private:
    void drawSubMesh( xMeshMaterial& mat, int i ) ;                             
protected:
    bool                           doSkin(CCoreSkeleton* pSkeleton , CCoreActionFrame* pActionFrame);
    void                           PrepareNonGpuSkinData() ;
	void                           PrepareGpuSkinData();
    IDirect3DVertexDeclaration9*   CreateInputAssembler(int nUVChanel , bool bSkin , int &nBytePerVertex);
protected:
	struct CSubMesh
	{
		int                        m_nFace;
		int                        m_iMatIdx;
		CD3D9IndexBuffer*          m_pIdxBuffer;
	};
    typedef std::vector<CSubMesh           > vSubMeshs; 
	typedef std::vector<CD3D9VertexBuffer* > vInputBuffers;  

	int                           m_nFace;
	int                           m_nVertex;
	wstring                       m_name;
	int                           m_nSubMesh;
	int                           m_nUVMaps;
	CSkeletonID                   m_SkeltonID;
	bool                          m_bSkin;
    int                           m_nBytePerSkinVertex;
	int                           m_nBytePerStaticVertex;

	vMeshMaterials                m_Materials;
	vSubMeshs                     m_SubMeshs;
	vInputBuffers                 m_VertexBuffers;
    CD3D9VertexBuffer*            m_pVertexBuffer;
 
	IDirect3DVertexDeclaration9*  m_pSkinVD;
	IDirect3DVertexDeclaration9*  m_pStaticVD;
	int                           m_nFrame;
	CBoundBox                     m_aabb;
    bool                          m_bGpuSkin;

    //只有GpuSkin为false的时候，这两个Buffer才有内容
    BYTE*                         m_pSkinVertexData;
    BYTE*                         m_pStaticVertexData;
    CCoreSkeleton*                m_pSkeleton;
	CBaseTextureMgr*              m_pTexMgr;
    CA3DDeviceWrapper*            m_DeviceWrapper;
};


#endif
