#include "CoreMesh.h"
#include "BaseModel.h"
#include "BaseAction.h"
#include "CoreSkeleton.h"
#include "Color.h"
template<typename T> void T_SWAP(T& v1 , T& v2)
{
    T _t = v1;
    v1 = v2;
    v2 = _t;
}


CCoreMesh::CCoreMesh(CBaseTextureMgr* pTexMgr , CBaseModel* pModel )
{
    m_nFace   = 0;
    m_nVertex = 0;
    m_name    = L"";
    m_nSubMesh= 0;
    m_nUVMaps  = 0;
    m_SkeltonID.m_HiWord = 0;
    m_SkeltonID.m_LoWord = 0;
    m_bSkin = false;
    m_nBytePerSkinVertex   = 0;
    m_nBytePerStaticVertex = 0;
    m_pVertexBuffer = NULL;
    m_pSkinVD    = NULL;
    m_pStaticVD  = NULL;
    m_nFrame = 1;
    m_bGpuSkin = false;
    m_pSkinVertexData   = NULL;
    m_pStaticVertexData = NULL;
    m_pSkeleton = NULL;
    m_pTexMgr = NULL;
    m_DeviceWrapper = NULL;


    m_pTexMgr          = pTexMgr;
    m_DeviceWrapper    = pTexMgr->GetDevice();
    m_aabb.Init();


    //目前从DirectX 10开始支持GPU Skin
    if(pModel)
    {
        m_pSkeleton = pModel->skeleton();
        m_bGpuSkin  = pModel->HasGpuSkin( );
    }
}

bool CCoreMesh::load(const wchar_t* fileName , unsigned long  arg)
{
    return load(fileName , NULL , 0 , arg);
}

bool CCoreMesh::load(const wchar_t* fileName , const BYTE* buf , size_t bufLen, unsigned long arg)
{
    xcomdoc doc;
    bool bRet = true;
    if(buf == NULL || bufLen == 0)
    {
        bRet = doc.open(fileName,  xcdm_read);
    }
    else
    {
        bRet = doc.open( (const _xcd_int8*)buf, (size_t)bufLen,  xcdm_read);
    }

    if( bRet == false )
        return false;

    xcomdocstream* pDesc = doc.open_stream(L"desc.xml");
    if(pDesc != NULL)
    {
        bRet = load(doc , L"" , arg);
        doc.close_stream(pDesc);
        doc.close();
    }
    return bRet;
}

bool CCoreMesh::isLoaded()
{
    return m_nVertex > 0;
}

bool CCoreMesh::unload()
{
    m_nFace = 0 ;
    m_nVertex = 0 ;
    m_name = L"";
    m_nSubMesh = 0;
    m_nUVMaps = 0;
    m_SkeltonID.m_HiWord = 0;
    m_SkeltonID.m_LoWord = 0;
    m_bSkin = false;
	size_t i = 0;
    for(i = 0 ; i < m_VertexBuffers.size() ; i ++)
    {
        delete m_VertexBuffers[i];
    }
    m_VertexBuffers.clear();
    m_pVertexBuffer = NULL;
    m_Materials.clear();
    size_t _nSubMesh = m_SubMeshs.size() ;
    for(i = 0 ; i <  _nSubMesh; i ++)
    {
        XSAFE_DELETE(m_SubMeshs[i].m_pIdxBuffer);
    }
    m_SubMeshs.clear();
    XSAFE_RELEASE(m_pSkinVD);
    XSAFE_RELEASE(m_pStaticVD);
    XSAFE_DELETE_ARRAY(m_pSkinVertexData);
    XSAFE_DELETE_ARRAY(m_pStaticVertexData);
    return true;
}

unsigned long CCoreMesh::memUsage()
{
    return 1;
}

void AddVertexDeclElement(std::vector<D3DVERTEXELEMENT9>& InputDesc  , BYTE _usage , BYTE _type , int Offset ,  int SemanticIdx , int bufferIdx )
{
    D3DVERTEXELEMENT9 _el;
    _el.Offset     = (UINT)Offset;
    _el.Stream     = (WORD)bufferIdx;
    _el.UsageIndex = (BYTE)SemanticIdx;
    _el.Usage      = _usage;
    _el.Type       = _type;
    _el.Method     = D3DDECLMETHOD_DEFAULT;//如果要使用Tesslator ,则可以用这个参数
    InputDesc.push_back( _el );
}

IDirect3DVertexDeclaration9* CCoreMesh::CreateInputAssembler(int nUVChanel , bool bSkin , int& nBytePerVertex)
{
    wchar_t _name[32] = {0};
    std::vector<D3DVERTEXELEMENT9> InputDesc;
    int BuffIdx = 0;
    int Offset = 0;
    if(bSkin)
    {
        swprintf(_name,L"SkinMeshIA_%0dUV" , nUVChanel);
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_POSITION     , D3DDECLTYPE_FLOAT3   ,      Offset     ,  0 , BuffIdx ); Offset += 3 * sizeof(float);
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_NORMAL       , D3DDECLTYPE_FLOAT3   ,      Offset     ,  0 , BuffIdx ); Offset += 3 * sizeof(float);
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_COLOR        , D3DDECLTYPE_D3DCOLOR ,      Offset     ,  0 , BuffIdx ); Offset += 4 * sizeof(char );
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_TANGENT      , D3DDECLTYPE_FLOAT3   ,      Offset     ,  0 , BuffIdx ); Offset += 3 * sizeof(float);
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_BLENDWEIGHT  , D3DDECLTYPE_FLOAT4   ,      Offset     ,  0 , BuffIdx ); Offset += 4 * sizeof(float);
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_BLENDINDICES , D3DDECLTYPE_SHORT4   ,      Offset     ,  0 , BuffIdx ); Offset += 4 * sizeof(short);

        for(int j = 0 ; j < nUVChanel ; j ++)
        {
            AddVertexDeclElement(InputDesc , D3DDECLUSAGE_TEXCOORD , D3DDECLTYPE_FLOAT2  ,      Offset     ,  j , BuffIdx ); Offset += 2 * sizeof(float);
        }
    }
    else
    {
        swprintf(_name,L"StaticMeshIA_%0dUV" , nUVChanel);
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_POSITION     , D3DDECLTYPE_FLOAT3    ,      Offset     ,  0 , BuffIdx ); Offset += 3 * sizeof(float);
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_NORMAL       , D3DDECLTYPE_FLOAT3    ,      Offset     ,  0 , BuffIdx ); Offset += 3 * sizeof(float);
        AddVertexDeclElement(InputDesc , D3DDECLUSAGE_COLOR        , D3DDECLTYPE_D3DCOLOR  ,      Offset     ,  0 , BuffIdx ); Offset += 4 * sizeof(char);
        //AddVertexDeclElement(InputDesc , D3DDECLUSAGE_TANGENT      , D3DDECLTYPE_FLOAT4   ,      Offset     ,  0 , BuffIdx ); Offset += 4 * sizeof(float);

        for(int j = 0 ; j < nUVChanel ; j ++)
        {
            AddVertexDeclElement(InputDesc , D3DDECLUSAGE_TEXCOORD , D3DDECLTYPE_FLOAT2 ,      Offset     ,  j , BuffIdx ); Offset += 2 * sizeof(float);
        }
    }

    nBytePerVertex = Offset;

    D3DVERTEXELEMENT9* D3DElementDesc = new D3DVERTEXELEMENT9[ InputDesc.size() + 1];
    D3DVERTEXELEMENT9 _end = { 0xFF,0,D3DDECLTYPE_UNUSED,0,0,0} ;
    D3DElementDesc[ InputDesc.size() ] = _end;
    int numElements = (int)InputDesc.size();
    for(int i = 0 ; i < numElements ; i ++)
    {
        D3DElementDesc[i] =  InputDesc[i];
    }
    IDirect3DVertexDeclaration9* pVD = NULL;
    HRESULT hr = m_DeviceWrapper->m_pDevice->CreateVertexDeclaration(D3DElementDesc , &pVD);
    delete [] D3DElementDesc;
    if(SUCCEEDED(hr))
        return pVD;
    return NULL;

}

void ReadXml( xcomdocstream* pdescstream, xXmlDocument &xml ) 
{
    int xmlBufferLen = (int)pdescstream->data_len();
    BYTE* xmlBuffer = new BYTE[xmlBufferLen + 1];
    //读到内存里来
    int offs = pdescstream->stream_tellr();
    pdescstream->stream_seekr(xcdsd_beg,0);
    pdescstream->read(xmlBuffer, xmlBufferLen);
    pdescstream->stream_seekr(xcdsd_beg,offs);
    xmlBuffer[xmlBufferLen] = 0;


    xml.load(xmlBuffer , xmlBufferLen , true);

}

bool CCoreMesh::loadMaterial(xcomdoc& doc , const wchar_t* _dir)
{
    wstring matXMLName = wstring(_dir) + L"material.xml";

    xcomdocstream* pMatStream = doc.open_stream(matXMLName.c_str() );
    xXmlDocument xml;
    ReadXml(pMatStream , xml );
    doc.close_stream(pMatStream);

    xXmlNode* pRootNode = xml.root();
    if(pRootNode == NULL)
        return false;

    xXmlNode::XmlNodes nodes;
    pRootNode->findNode(L"material" , nodes);
    size_t nMatNodes = nodes.size() ;
    for(size_t i = 0 ; i < nMatNodes ; i ++)
    {
        xXmlNode* pMatNode = nodes[i];
        xMeshMaterial mat;
        xXmlNode::XmlNodes texNodes;
        pMatNode->findNode(L"texture" , texNodes);
        if(pMatNode->value(L"matscript") )
        {
            mat.m_Script = pMatNode->value(L"matscript") ;
        }

        xXmlNode* pAmbNode = pMatNode->findNode(L"Ambient");
        if(pAmbNode)
        {
            mat.m_Ambient.r=pAmbNode->float_value(L"r"); 
            mat.m_Ambient.g=pAmbNode->float_value(L"g"); 
            mat.m_Ambient.b=pAmbNode->float_value(L"b"); 
            mat.m_Ambient.a=pAmbNode->float_value(L"a"); 
        }
        xXmlNode* pDiffNode = pMatNode->findNode(L"Diffuse");
        if(pDiffNode)
        {
            mat.m_Diffuse.r=pDiffNode->float_value(L"r"); 
            mat.m_Diffuse.g=pDiffNode->float_value(L"g"); 
            mat.m_Diffuse.b=pDiffNode->float_value(L"b"); 
            mat.m_Diffuse.a=pDiffNode->float_value(L"a"); 
        }


        xXmlNode* pSpecNode = pMatNode->findNode(L"Speculer");
        if(pSpecNode)
        {
            mat.m_Speculer.r=pSpecNode->float_value(L"r"); 
            mat.m_Speculer.g=pSpecNode->float_value(L"g"); 
            mat.m_Speculer.b=pSpecNode->float_value(L"b"); 
            mat.m_Speculer.a=pSpecNode->float_value(L"a"); 
        }

        xXmlNode* pEmissNode = pMatNode->findNode(L"Emissive");
        if(pEmissNode)
        {
            mat.m_Emissive.r=pEmissNode->float_value(L"r"); 
            mat.m_Emissive.g=pEmissNode->float_value(L"g"); 
            mat.m_Emissive.b=pEmissNode->float_value(L"b"); 
            mat.m_Emissive.a=pEmissNode->float_value(L"a");
        }

        mat.m_fShiness = pMatNode->float_value(L"Shiness" );
        mat.m_fOpactiy = pMatNode->float_value(L"Opactiy" );
        size_t nTexNodes = texNodes.size() ;
        for(size_t iTex = 0; iTex < nTexNodes ; iTex ++)
        {
            xMeshTexture tex;
            tex.m_TexOp.m_iChannel = texNodes[iTex]->int_value(L"chanel");
            tex.m_TexOp.m_iUsage   = (eTexLayerUsage)texNodes[iTex]->int_value(L"iUsage");
            tex.m_TexName  = texNodes[iTex]->value(L"image");
            CTextureName _name;
            _name.m_Name = tex.m_TexName;
            _name.m_Format = D3DFMT_UNKNOWN;
            if(tex.m_TexOp.m_iUsage == tex_usage_opacity)
            {
                _name.m_Format = D3DFMT_A8;
            }

            tex.m_hTexture = m_pTexMgr->add(_name , 0 , true);

            if(tex.m_hTexture.getResource() )
            {
                xA3DTexture* pTexture = tex.m_hTexture.getResource();
                //透明贴图
                if(tex.m_TexOp.m_iUsage == tex_usage_opacity)
                {
                    //当前颜色
                    tex.m_TexOp.m_COLOROP     = D3DTOP_SELECTARG1;
                    tex.m_TexOp.m_COLORARG1   = D3DTA_CURRENT;
                    tex.m_TexOp.m_ALPHAOP     = D3DTOP_MODULATE;
                    tex.m_TexOp.m_ALPHAARG1   = D3DTA_CURRENT;
                    tex.m_TexOp.m_ALPHAARG2   = D3DTA_TEXTURE;
                }
            }
            tex.m_TexOp.Build();
            mat.m_vTextures.push_back(tex);
        }

        //如果读取出来的纹理个数为0.
        if(nTexNodes == 0)
        {
            xMeshTexture tex;
            tex.m_TexOp.m_iChannel = 0;
            tex.m_TexOp.m_iUsage   = tex_usage_diffuse;
            tex.m_TexName  = L"white.tga";
            tex.m_hTexture = m_pTexMgr->add(tex.m_TexName.c_str() , 0 , true);
            mat.m_vTextures.push_back(tex);
        }
        std::string strShader = mat.BuildTextureShader();
        mat.Build();
        m_Materials.push_back(mat);
    }

    return true;
}

struct xModelWeightedVertex
{
    D3DXVECTOR3 m_InitPos;
    int         m_nEffBone; //影响到的骨头数目
    struct 
    {
        int    m_BoneIndex;
        int    m_BoneID;
        float  m_fWeight;
    }m_Weight[4];
};



bool CCoreMesh::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
    //读入文件头
    wstring descName = wstring(_dir) + L"desc.xml";
    xcomdocstream* pdescstream = doc.open_stream(descName.c_str() );
    if(pdescstream == NULL)
        return false;

    xXmlDocument xml;
    ReadXml(pdescstream, xml);
    doc.close_stream(pdescstream);


    m_aabb.Init(10000000000.0f);
    xXmlNode* pRootNode = xml.root();
    m_name     =pRootNode->value(L"name");
    m_nFace    =pRootNode->int_value(L"nFace");
    m_nVertex  =pRootNode->int_value(L"nVertex");
    m_nSubMesh =pRootNode->int_value(L"nSubMesh");
    m_nUVMaps  =pRootNode->int_value(L"nUVMaps");
    m_nFrame   = 1;
    if(pRootNode->value(L"nFrame") ) m_nFrame = pRootNode->int_value(L"nFrame");

    //纹理v坐标的属性。
    const wchar_t* pTexCoordStyle = pRootNode->value(L"TexCoordStyle");
    std::wstring  strTexCoordStyle = L"OpenGL";
    if(pTexCoordStyle != NULL)
    {
        strTexCoordStyle = pTexCoordStyle;
    }
    wstring rApiTexCoordStyle = L"Direct3D";
    bool bInvertVCoord = rApiTexCoordStyle != strTexCoordStyle;


    //骨架ID
    xXmlNode* pSkelNode = pRootNode->findNode(L"skeleton");
    if(pSkelNode)
    {
        m_SkeltonID.m_HiWord = pSkelNode->int_value(L"HiID");
        m_SkeltonID.m_LoWord = pSkelNode->int_value(L"LoID");
    }
    else
    {
        m_SkeltonID.m_HiWord = 0;
        m_SkeltonID.m_LoWord = 0;
    }

    //读入SubMesh信息
    m_SubMeshs.resize(m_nSubMesh);
    xXmlNode::XmlNodes subMeshNodes;
    pRootNode->findNode(L"submesh", subMeshNodes);
    assert(subMeshNodes.size() == m_nSubMesh);
    for(int i = 0 ; i < m_nSubMesh ; i ++)
    {
        CSubMesh& subMesh      = m_SubMeshs[i];
        xXmlNode* pSubMeshNode = subMeshNodes[i];
        subMesh.m_iMatIdx = pSubMeshNode->int_value(L"nMatIndex");
        subMesh.m_nFace   = pSubMeshNode->int_value(L"nFace");
    }
    //加载材质

    loadMaterial(doc , _dir);

    wstring faceName     = wstring(_dir) + L"face";
    xcomdocstream* pFaceStream     = doc.open_stream(faceName.c_str() );

    //读入所有的面
    {
        for(int i = 0 ; i < m_nSubMesh ; i ++)
        {
            int32 nFace ;
            int32 iMatIdx;
            pFaceStream->read(nFace);
            pFaceStream->read(iMatIdx);
            xMeshFace* pData = new xMeshFace [nFace];
            pFaceStream->read(pData , nFace);
            for(int j = 0 ; j < nFace ; j ++)
            {
                //T_SWAP(pData[j].m_Idx[0] , pData[j].m_Idx[1]);
            }

            CD3D9IndexBuffer* pIndexBuffer = new CD3D9IndexBuffer( m_DeviceWrapper->m_pDevice );
            pIndexBuffer->create( nFace *3  , 4 , D3DPOOL_MANAGED , 0 ,  pData);
            m_SubMeshs[i].m_pIdxBuffer = pIndexBuffer;
            delete pData;		
        }
        doc.close_stream( pFaceStream);
    }

    if(m_nFrame == 1)
        return readSingleFrame(doc , _dir , arg , bInvertVCoord);
    else
    {
        return readAnimationFrame(doc , _dir , arg , bInvertVCoord);
    }
    return true;
}

bool CCoreMesh::readSingleFrame(xcomdoc& doc , const wchar_t* _dir , unsigned int arg , bool bInvertVCoord)
{
    wstring weightName   = wstring(_dir) + L"weight";
    wstring positionName = wstring(_dir) + L"position";
    wstring normalName   = wstring(_dir) + L"normal";
    wstring tangentName  = wstring(_dir) + L"tangent";
    wstring diffusenName = wstring(_dir) + L"diffuse";
    wstring faceName     = wstring(_dir) + L"face";


    xcomdocstream* pWeightStream   = doc.open_stream(weightName.c_str() );
    xcomdocstream* pPositionStream = doc.open_stream(positionName.c_str() );
    xcomdocstream* pNormalStream   = doc.open_stream(normalName.c_str() );
    xcomdocstream* pTangentStream  = doc.open_stream(tangentName.c_str() );
    xcomdocstream* pDiffuseStream  = doc.open_stream(diffusenName.c_str() );
    xcomdocstream* pFaceStream     = doc.open_stream(faceName.c_str() );

    xcomdocstream** pUVStream = new xcomdocstream* [m_nUVMaps];
    for(int i = 0 ; i < m_nUVMaps ; i ++)
    {
        wchar_t uvName[9] = {0};
        swprintf(uvName , L"uv_%d" , i);
        wstring faceName     = wstring(_dir) + uvName;
        pUVStream[i] = doc.open_stream( faceName.c_str() );
        int32 nVertex = 0;
        pUVStream[i]->read(nVertex);
    }
    m_nFrame = 1;
    m_bSkin = pWeightStream != NULL;
    m_VertexBuffers.resize(m_nFrame);

    //如果不支持Gpu的Skin。那么就创建一个静态的SkinMesh
    m_pSkinVD   =  CreateInputAssembler(m_nUVMaps ,  true  , m_nBytePerSkinVertex  );//m_bGpuSkin && m_bSkin);
    m_pStaticVD =  CreateInputAssembler(m_nUVMaps ,  false , m_nBytePerStaticVertex);
    void* pInitData = NULL; 
    int32 nVertex = 0;
    pNormalStream->read(nVertex);
    pTangentStream->read(nVertex);
    pDiffuseStream->read(nVertex);

    xMeshMaterial* pMat = NULL;
    bool useMatColor = false;
    size_t nMat = m_Materials.size();
    //只要有一个材质的纹理层数为0，就用这个材质的颜色填充。
    for(size_t iMat = 0 ; iMat < nMat ; iMat ++)
    {
        if(m_Materials[iMat].m_vTextures.size() == 0) 
        {
            pMat = &m_Materials[iMat];
            useMatColor = true;
        }
    }

    //如果数据能一次性读入，将优化读取速度，此处因我偷懒没优化。

    //骨骼动画数据读取
    if(pWeightStream)
    {
        pWeightStream->read(nVertex);
        int  nSkinVertSize = sizeof(xSkinMeshVertex) + (m_nUVMaps - 1) * sizeof(D3DXVECTOR2);
        m_pSkinVertexData = new BYTE[  nSkinVertSize * nVertex ];
        for(int i = 0 ; i < nVertex ; i ++)
        {
            xSkinMeshVertex* pVertex =  (xSkinMeshVertex*)(m_pSkinVertexData + i * nSkinVertSize);
            pVertex->m_weight[0] =  pVertex->m_weight[1] = pVertex->m_weight[2] = pVertex->m_weight[3] = 0.0f;
            pVertex->m_windex[0] =  pVertex->m_windex[1] = pVertex->m_windex[2] = pVertex->m_windex[3] = 0;

            xModelWeightedVertex wv ;
            D3DXVECTOR3       nrm;
            xColor_4ub  cl;
            D3DXVECTOR3       tan;
            pWeightStream->read(wv);
            pNormalStream->read(nrm);
            pTangentStream->read(tan);
            pDiffuseStream->read(cl);
            pVertex->m_Position = D3DXVECTOR3(wv.m_InitPos.x , wv.m_InitPos.y , wv.m_InitPos.z);// , 1.0f);
            m_aabb.AddPoint(wv.m_InitPos);
            for(int32 iBone = 0 ; iBone < wv.m_nEffBone ; iBone ++)
            {
                pVertex->m_weight[iBone] = wv.m_Weight[iBone].m_fWeight;
                pVertex->m_windex[iBone] = wv.m_Weight[iBone].m_BoneIndex ;
            }

            pVertex->m_Normal   = nrm;//D3DXVECTOR3(nrm.x  , nrm.y , nrm.z);//  , 1.0f);
            pVertex->m_Tangent  = tan;//D3DXVECTOR3(tan.x  , tan.y , tan.z);//  , 1.0f);
            pVertex->m_Diffuse  = cl.color ;//D3DXVECTOR3(cl.r/255.0f , cl.g /255.0f , cl.b/255.0f , cl.a /255.0f);

            //如果某个材质的
            if(useMatColor && pMat)
            {
                pVertex->m_Diffuse = pMat->m_Diffuse;//
                //pVertex->m_Diffuse.x *= pMat->m_Diffuse.r ; 
                //pVertex->m_Diffuse.y *= pMat->m_Diffuse.g ;
                //pVertex->m_Diffuse.z *= pMat->m_Diffuse.b ;
                //pVertex->m_Diffuse.w *= pMat->m_Diffuse.a ;

            }
            for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
            {
                D3DXVECTOR2 uv;
                pUVStream[iUV]->read(uv);
                if(bInvertVCoord)   { uv.y = 1.0f - uv.y; }//把纹理坐标逆过来
                pVertex->m_uv[iUV] = uv;
            }
        }


        if(m_bGpuSkin)
        {
            PrepareGpuSkinData();
            //XSAFE_DELETE_ARRAY(m_pSkinVertexData);
        }
        else
        {
            //如果不是GPU Skin则不填充数据
            PrepareNonGpuSkinData();
        }
    }
    else
    {
        pPositionStream->read(nVertex);
        int  vertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(D3DXVECTOR2);
        char*  pData = new char[  (vertSize) * (nVertex )];
        for(int i = 0 ; i < nVertex ; i ++)
        {
            xStaticMeshVertex* pVertex =  (xStaticMeshVertex*)(pData + i * vertSize);
            D3DXVECTOR3       pos ;
            D3DXVECTOR3       nrm;
            xColor_4ub        cl;
            D3DXVECTOR3       tan;
            pPositionStream->read(pos);
            pNormalStream->read(nrm);
            pTangentStream->read(tan);
            pDiffuseStream->read(cl);
            m_aabb.AddPoint(pos);
            pVertex->m_Position = pos;//D3DXVECTOR4(pos.x  , pos.y , pos.z  , 1.0f);
            pVertex->m_Normal   = nrm;//D3DXVECTOR4(nrm.x  , nrm.y , nrm.z  , 1.0f);
            //pVertex->m_Tangent  = tan;//D3DXVECTOR4(tan.x  , tan.y , tan.z  , 1.0f);
            pVertex->m_Diffuse  = cl.color ;//D3DXVECTOR4(cl.r/255.0f , cl.g /255.0f , cl.b/255.0f , cl.a /255.0f);
            for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
            {
                D3DXVECTOR2 uv;
                pUVStream[iUV]->read(uv);
                if(bInvertVCoord) { uv.y = 1.0f - uv.y; } //把纹理坐标逆过来
                pVertex->m_uv[iUV] = uv;
            }
        }
        CD3D9VertexBuffer* pVB = new CD3D9VertexBuffer( m_DeviceWrapper->m_pDevice );
        pVB->create( nVertex ,  m_nBytePerStaticVertex , D3DPOOL_MANAGED , 0 , pData );
        m_VertexBuffers[0] = pVB;
        m_pVertexBuffer = pVB;
        delete [] pData;
    }

    doc.close_stream( pWeightStream   );
    doc.close_stream( pPositionStream );
    doc.close_stream( pNormalStream   );
    doc.close_stream( pTangentStream  );
    doc.close_stream( pDiffuseStream  );

    //关闭纹理流
    {
        for(int j = 0 ; j < m_nUVMaps ; j ++)
        {
            doc.close_stream(pUVStream[j]);
        }
    }

    delete [] pUVStream;
    return true;
}

void CCoreMesh::ToCpuSkin()
{
    //不是骨骼动画是不需要这样转化的。
    if(m_bSkin == false)
        return ;

    if(m_pSkinVertexData != NULL && m_pStaticVertexData != NULL)
        return ;

    PrepareNonGpuSkinData();
}

void CCoreMesh::SetShader(const wchar_t* shaderName)
{
    for(int i = 0 ; i < this->m_Materials.size() ; i ++ )
    {
        m_Materials[i].SetShader(m_DeviceWrapper , shaderName);
    }
}

void CCoreMesh::ToGpuSkin()
{
    //不是骨骼动画是不需要这样转化的。
    if(m_bSkin == false)
        return ;

    //如果不支持GpuSkin。也没办法啊。
    if(m_bGpuSkin == false)
        return ;

    if(m_pSkinVertexData == NULL)
        return ;
    PrepareGpuSkinData();
}

void CCoreMesh::PrepareGpuSkinData() 
{
    if(m_pVertexBuffer) delete m_pVertexBuffer;
    CD3D9VertexBuffer* pVB = new CD3D9VertexBuffer( m_DeviceWrapper->m_pDevice );
    pVB->create( m_nVertex ,  m_nBytePerSkinVertex , D3DPOOL_MANAGED , 0 , m_pSkinVertexData );
    m_VertexBuffers[0] = pVB;
    m_pVertexBuffer = pVB;
}

void CCoreMesh::PrepareNonGpuSkinData() 
{
    if(m_pVertexBuffer) delete m_pVertexBuffer;
    CD3D9VertexBuffer* pVB = new CD3D9VertexBuffer( m_DeviceWrapper->m_pDevice );
    pVB->create( m_nVertex ,  m_nBytePerStaticVertex , D3DPOOL_MANAGED , 0 , NULL );
    m_VertexBuffers[0] = pVB;
    m_pVertexBuffer = pVB;

    int  nSkinVertSize   = sizeof(xSkinMeshVertex)   + (m_nUVMaps - 1) * sizeof(D3DXVECTOR2);
    int  nStaticVertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(D3DXVECTOR2);
    m_pStaticVertexData = new BYTE[  (nStaticVertSize) * (m_nVertex )];
    for(int i = 0 ; i < m_nVertex ; ++i )
    {
        xStaticMeshVertex* pStaticVertex = (xStaticMeshVertex*)(m_pStaticVertexData + nStaticVertSize * i );
        xSkinMeshVertex*   pSkinedVertex = (xSkinMeshVertex*)  (m_pSkinVertexData   + nSkinVertSize   * i );
        for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
        {
            pStaticVertex->m_uv[iUV] = pSkinedVertex->m_uv[iUV];
        }
        pStaticVertex->m_Diffuse = pSkinedVertex->m_Diffuse;
    }
}


bool CCoreMesh::readAnimationFrame(xcomdoc& doc , const wchar_t* _dir , unsigned int arg , bool bInvertVCoord)
{
    wstring positionName = wstring(_dir) + L"position";
    wstring normalName   = wstring(_dir) + L"normal";
    wstring tangentName  = wstring(_dir) + L"tangent";
    wstring diffusenName = wstring(_dir) + L"diffuse";
    wstring faceName     = wstring(_dir) + L"face";

    xcomdocstream* pPositionStream = doc.open_stream(positionName.c_str() );
    xcomdocstream* pNormalStream   = doc.open_stream(normalName.c_str() );
    xcomdocstream* pTangentStream  = doc.open_stream(tangentName.c_str() );
    xcomdocstream* pDiffuseStream  = doc.open_stream(diffusenName.c_str() );
    xcomdocstream* pFaceStream     = doc.open_stream(faceName.c_str() );

    xcomdocstream** pUVStream = new xcomdocstream* [m_nUVMaps];
    for(int i = 0 ; i < m_nUVMaps ; i ++)
    {
        wchar_t uvName[9] = {0};
        swprintf(uvName , L"uv_%d" , i);
        wstring faceName     = wstring(_dir) + uvName;
        pUVStream[i] = doc.open_stream( faceName.c_str() );
        int nVertex = 0;
        pUVStream[i]->read(nVertex);
    }
    m_bSkin   = false;
    m_VertexBuffers.resize(m_nFrame);

    m_pSkinVD   = CreateInputAssembler(m_nUVMaps , true  , m_nBytePerSkinVertex  );//m_bSkin);
    m_pStaticVD = CreateInputAssembler(m_nUVMaps , false , m_nBytePerStaticVertex);//m_bSkin);


    void* pInitData = NULL; 
    int32 nVertex = 0;
    pNormalStream->read(nVertex);
    pTangentStream->read(nVertex);
    pDiffuseStream->read(nVertex);
    pPositionStream->read(nVertex);

    m_nVertex = nVertex;
    int  vertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(D3DXVECTOR2);
    char*  pData = new char[  (vertSize) * (nVertex )];
    for(int iFrame = 0 ;iFrame < m_nFrame  ; iFrame ++)
    {
        for(int iVertex = 0 ; iVertex < nVertex ; iVertex ++)
        {
            xStaticMeshVertex* pVertex =  (xStaticMeshVertex*)(pData + iVertex * vertSize);
            D3DXVECTOR3       pos ;
            D3DXVECTOR3       nrm;
            xColor_4ub  cl;
            D3DXVECTOR3       tan;
            pPositionStream->read(pos);
            pNormalStream->read(nrm);
            pTangentStream->read(tan);
            pDiffuseStream->read(cl);
            pVertex->m_Position = pos;//D3DXVECTOR4(pos.x  , pos.y , pos.z  , 1.0f);
            pVertex->m_Normal   = nrm;//D3DXVECTOR4(nrm.x  , nrm.y , nrm.z  , 1.0f);
            //pVertex->m_Tangent  = tan;//D3DXVECTOR4(tan.x  , tan.y , tan.z  , 1.0f);
            pVertex->m_Diffuse  = cl.color ;//D3DXVECTOR4(cl.r/255.0f , cl.g /255.0f , cl.b/255.0f , cl.a /255.0f);
            m_aabb.AddPoint(pos);
            for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
            {
                D3DXVECTOR2 uv;
                pUVStream[iUV]->read(uv);
                if(bInvertVCoord) { uv.y = 1.0f - uv.y; } //把纹理坐标逆过来
                pVertex->m_uv[iUV] = uv;
            }
        }
        CD3D9VertexBuffer* pVB = new CD3D9VertexBuffer( m_DeviceWrapper->m_pDevice );
        pVB->create( nVertex ,  m_nBytePerStaticVertex , D3DPOOL_MANAGED , 0 , pData );
        m_VertexBuffers[0] = pVB;
        m_VertexBuffers[iFrame] = pVB;
        m_pVertexBuffer = pVB;


    }
    delete [] pData;

    doc.close_stream( pPositionStream );
    doc.close_stream( pNormalStream   );
    doc.close_stream( pTangentStream  );
    doc.close_stream( pDiffuseStream  );
    for(int iUv = 0 ; iUv < m_nUVMaps ; iUv ++)
    {
        doc.close_stream(pUVStream[iUv]);
    }
    delete [] pUVStream;
    return true;
}

void CCoreMesh::setTexture(int iSubMesh , int iTex , HBaseTexture hTexture)
{
    int matidx = m_SubMeshs[iSubMesh].m_iMatIdx;
    if( matidx >= (int)m_Materials.size() ) 
        matidx = (int)m_Materials.size() - 1;

    xMeshMaterial& mat = m_Materials[ matidx ];
    size_t nTex = mat.m_vTextures.size() ;
    if(iTex >= (int)nTex)
        return ;

    mat.m_vTextures[iTex].m_hTexture = hTexture;
}

bool CCoreMesh::draw(CCoreSkeleton* pSkeleton , CCoreActionFrame* pActionFrame , bool bCpuSkin)
{
    if(pSkeleton == NULL) pSkeleton = m_pSkeleton;

    IDirect3DDevice9* pDevice = m_DeviceWrapper->m_pDevice;

    bool bNeedGpuShader = false;
    //根据是不是Skin。来选择是否要Cpu来计算Skin。并选择正确的VertexDecl;
    int nBytePerVertex = 0;
    if(m_bGpuSkin == false || bCpuSkin) 
    {
        doSkin(pSkeleton , pActionFrame);
        pDevice->SetVertexDeclaration(m_pStaticVD);
        nBytePerVertex = m_nBytePerStaticVertex;
    }
    else if(m_bSkin)
    {
        pDevice->SetVertexDeclaration(m_pSkinVD);
        nBytePerVertex = m_nBytePerSkinVertex;
        bNeedGpuShader = true;
    }
    else
    {
        pDevice->SetVertexDeclaration(m_pStaticVD);
        nBytePerVertex = m_nBytePerStaticVertex;
    }


    pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CW );

    pDevice->SetStreamSource(0 , m_pVertexBuffer->handle() , 0 , nBytePerVertex );
    //m_pRenderApi->setVertexStream( m_pVertexStream );
    for(int i = 0 ; i < m_nSubMesh ; i ++)
    {
        CSubMesh& subMesh = m_SubMeshs[i];
        int matidx = m_SubMeshs[i].m_iMatIdx;
        if( matidx >= (int)m_Materials.size() ) matidx = (int)m_Materials.size() - 1;

        xMeshMaterial& mat = m_Materials[ matidx ];
        CFxShader* pShader = mat.m_FxShader;
        if(pShader  != NULL ) 
        {
            mat.Begin(pDevice);
            pShader->setTechnique("Main");
            size_t nPass = 0;
            pShader->begin(&nPass , 0);
            for(size_t iPass = 0 ; iPass < nPass ; iPass ++)
            {
                pShader->beginPass(iPass);
                drawSubMesh(mat, i);
                pShader->endPass();

            }
            mat.End(pDevice);
        }
        else
        {
            mat.Begin(pDevice);
            drawSubMesh(mat, i);
            mat.End(pDevice);
        }

        continue;
        //m_pRenderApi->draw(m_SubMeshs[i].m_pIdxBuffer , m_SubMeshs[i].m_nFace * 3 );
    }
    return true;	
}

void CCoreMesh::drawSubMesh( xMeshMaterial& mat, int i ) 
{
    m_DeviceWrapper->m_pDevice->SetIndices( m_SubMeshs[i].m_pIdxBuffer->handle() );
    m_DeviceWrapper->m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0 , 0 , m_nVertex , (UINT)0 , m_SubMeshs[i].m_nFace );
}
void D3DXVec3NormalizeInplace(D3DXVECTOR3& v)
{
    float len = (float) sqrt( float(v.x * v.x + v.y * v.y + v.z * v.z) );

    if(len == 0)
        return ;
    v.x = float(v.x/len);
    v.y = float(v.y/len);
    v.z = float(v.z/len);
    //v.w = 1.0f;
}


bool CCoreMesh::doSkin(CCoreSkeleton* pSkeleton , CCoreActionFrame* pActionFrame)
{
    if(pSkeleton == NULL || pActionFrame == NULL)
        return false;

    this->ToCpuSkin();
    CCoreActionFrame& actionFrame = *pActionFrame;
    int  nSkinVertSize   = sizeof(xSkinMeshVertex)   + (m_nUVMaps - 1) * sizeof(D3DXVECTOR2);
    int  nStaticVertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(D3DXVECTOR2);

    //unsigned __int64 startTime  = __rdtsc();
    //#pragma omp parallel
    //#pragma omp for 

    for(int i = 0 ; i < m_nVertex ; ++i )
    {
        xStaticMeshVertex* pStaticVertex = (xStaticMeshVertex*)(m_pStaticVertexData + nStaticVertSize * i );
        xSkinMeshVertex*   pSkinedVertex = (xSkinMeshVertex*)  (m_pSkinVertexData   + nSkinVertSize   * i );


        D3DXVECTOR3&  v   = pStaticVertex->m_Position;
        D3DXVECTOR3&  nrm = pStaticVertex->m_Normal;
        //D3DXVECTOR3&  tg  = pStaticVertex->m_Tangent;

        D3DXVECTOR3&  nrm_init  = pSkinedVertex->m_Normal;
        //D3DXVECTOR3&  tg_init   = pSkinedVertex->m_Tangent;

        v   = D3DXVECTOR3(0.0f,0.0f,0.0f);//,1.0f);
        nrm = D3DXVECTOR3(0.0f,0.0f,0.0f);//,1.0f);
        //tg  = D3DXVECTOR3(0.0f,0.0f,0.0f,1.0f);
        D3DXVECTOR3& initPos = pSkinedVertex->m_Position;
        for(int iw = 0 ; iw < 4 ; ++iw)
        {
            D3DXVECTOR3   temp;
            int boneIndex = pSkinedVertex->m_windex[iw];
            float fWeight = pSkinedVertex->m_weight[iw];
            if(fWeight <= 0.00000000001f)
                break;

            D3DXMATRIX& m = actionFrame.operator[](boneIndex);

            //计算法向量
            //m.transform_l_3x3(nrm_init,temp);
            D3DXVec3TransformNormal((D3DXVECTOR3*)&temp , (D3DXVECTOR3*)&nrm_init , &m); //temp.w = 1.0f;                
            temp *= fWeight;

            //XM_Add(nrm,temp,nrm);
            nrm = nrm + temp;

            //计算tangent
            //m.transform_l_3x3(tg_init,temp);
            //D3DXVec3TransformNormal((D3DXVECTOR3*)&temp , (D3DXVECTOR3*)&tg_init , &m); temp.w = 1.0f;  
            //temp *= fWeight;
            //XM_Add(tg,temp,tg);
            //tg = tg + temp;

            //计算顶点位置
            //D3DX
            D3DXVECTOR4 _out;
            D3DXVec3Transform( &_out , &initPos , &m);// temp.w = 1.0f; 
            temp = D3DXVECTOR3(_out.x / _out.w , _out.y / _out.w ,_out.z / _out.w );
            //XM_Mul(initPos,m,temp);
            temp *= fWeight;                
            //XM_Add(v,temp,v);
            v = v + temp;

        }
        //v = initPos;
        //nrm = nrm_init;
        D3DXVec3NormalizeInplace(nrm);
        //D3DXVec4NormalizeInplace(tg);

        T_SWAP(v.y   , v.z);
        T_SWAP(nrm.y , nrm.z);
        //T_SWAP(tg.y , tg.z);            

        //nrm.normalize();
        //tg.normalize();
        //v.w = nrm.w = tg.w = 1.0f;
    }

    //unsigned __int64  endTime  = __rdtsc();
    //XEVOL_LOG(eXR_LOG_LEVEL::eXL_DEBUG_HIGH , "Skin time=%d" , endTime - startTime);
    m_pVertexBuffer->update(eLock_WriteDiscard , m_pStaticVertexData , m_nVertex * nStaticVertSize);
    return true;
}


bool CCoreMesh::setCurrentFrame(int iFrame)
{
    //只对帧动画有用
    if(iFrame >= (int)m_VertexBuffers.size() )
    {
        iFrame = (int)m_VertexBuffers.size() - 1;
    }
    m_pVertexBuffer = m_VertexBuffers[iFrame];
    //m_pVertexStream->setInputBuffer(0,m_VertexBuffers[iFrame]);
    return true;
}


