#include "../xcomdoc/xcomdoc.h"
#include "../xcomdoc/xI18N.h"
#include "CoreSkeleton.h"
#include "CoreMesh.h"

template <typename T, typename _TSTREAM > void Ts_Write(_TSTREAM& _out ,const T& data)
{
    _out.write( (const char*) &data,sizeof(T));
}


template <typename T, typename _TSTREAM , typename _SIZE> void  Ts_Write(_TSTREAM& _out , const T* data, _SIZE nObject)
{
    _out.write( (const char*)data,sizeof(T)*nObject);
}

template <typename T, typename _TSTREAM > void  Ts_Read(_TSTREAM& _in , T& data)
{
    _in.read( (char*) &data,sizeof(T));
}

template <typename T , typename _TSTREAM , typename _SIZE> void  Ts_Read(_TSTREAM& _in ,  T* data, _SIZE nObject)
{
    _in.read( (char*)data , ( (_SIZE)sizeof(T) ) * nObject);
}

template <typename T, typename _TSTREAM > void  Ts_ReadPtr(_TSTREAM* _in , T& data)
{
    _in->read( (char*) &data,sizeof(T));
}

template <typename T , typename _TSTREAM , typename _SIZE> void  Ts_ReadPtr(_TSTREAM* _in ,  T* data, _SIZE nObject)
{
    _in->read( (char*)data , ( (_SIZE)sizeof(T) ) * nObject);
}

//骨架层次类
xBoneHiberarchy::xBoneHiberarchy()
{
    m_nChildren = 0;
}
xBoneHiberarchy::~xBoneHiberarchy()
{
    m_nChildren = 0 ;
    m_Childrens.clear();
}
void xBoneHiberarchy::clear()
{
    m_nChildren = 0 ;
    m_Childrens.clear();
}
bool xBoneHiberarchy::add_child(int index )
{
	size_t _nChild = m_Childrens.size() ;
    for(size_t i = 0 ; i < _nChild ;  ++i)
    {
        if(m_Childrens[i] == index)
            return false;
    }
    m_Childrens.push_back(index);
    m_nChildren ++;
    return true;
}
void xBoneHiberarchy::push_back(int index )
{
    m_Childrens.push_back(index);
    m_nChildren ++;   
}

//骨架类
CCoreSkeleton::CCoreSkeleton()
{
}

int  CCoreSkeleton::findBoneIndex(const wchar_t* boneName)
{
	if(boneName == NULL)
		return 0;
	std::wstring _boneName = boneName;
	for(int i = 0 ; i < m_nBone ; i ++)
	{
		if(m_Bones[i].m_BoneName == _boneName)
		{
			return i;
		}
	}
	return 0;
}

CCoreSkeleton::~CCoreSkeleton()
{
    unload();
}

bool CCoreSkeleton::unload()
{
    m_RootHibers.clear();
    m_BoneHibers.clear();

    //骨架信息
    m_nBone = 0;
    m_Bones.clear();
    return true;
}

bool CCoreSkeleton::load(xcomdoc& doc,const wchar_t* skeleton_dir)
{
    //Load skeleton
    {
        std::wstring _name = std::wstring(skeleton_dir ) + L"skeleton/skeleton";
        xcomdocstream* boneinfo_stream = doc.open_stream(_name.c_str(),true);
        if(boneinfo_stream == NULL)
            return false;
        bool bRet = loadBoneList(boneinfo_stream);
        doc.close_stream(boneinfo_stream);	
        if(bRet == false)
            return false;
    }

    //加载骨架信息
    {
        std::wstring _name = std::wstring(skeleton_dir ) + L"skeleton/hiberarchy";
        xcomdocstream* actdata_hiberarchy = doc.open_stream(_name.c_str(),true);

        if(actdata_hiberarchy == NULL)
            return false;

        bool bRet = loadHiberarchy(actdata_hiberarchy);
        doc.close_stream(actdata_hiberarchy);		
        if(bRet == false)
            return false;

        //_name = std::wstring(skeleton_dir ) + L"skeleton/hiberarchy.xml";
        //xcomdocstream* actdata_hiberarchy_xml = doc.open_stream(_name.c_str(),true);
        //if(actdata_hiberarchy_xml == NULL)
        //{
        //    actdata_hiberarchy_xml = doc.open_stream(_name.c_str(),false);
        //    if(actdata_hiberarchy_xml)
        //    {
        //        saveHiberarchy(actdata_hiberarchy_xml);
        //        doc.close_stream(actdata_hiberarchy_xml);	
        //    }
        //}
    }

    return true;
}

bool CCoreSkeleton::loadHiberarchy(xcomdocstream* _in)
{
    m_BoneHibers.clear();
    int32 nRootNode = -1;
    CSkeletonID id ;
    Ts_ReadPtr(_in , id);
    Ts_ReadPtr(_in , nRootNode );
    for(int i = 0 ; i < nRootNode ; ++i)
    {
        int child = - 1;
        _in->read( (char*)&child , sizeof(child) );

        if(child == -1)
            return false;

        m_RootHibers.add_child(child);
    }
    int nBone = -1;
    Ts_ReadPtr(_in,nBone);
    m_BoneHibers.resize(nBone);
    for(int iBone = 0 ; iBone < nBone ; ++iBone)
    {

        xBoneHiberarchy& boneH = m_BoneHibers[iBone];
        int nChild = 0;

        Ts_ReadPtr(_in, nChild );
        for(int j = 0 ; j < nChild ; ++j)
        {
            int child = - 1;
            Ts_ReadPtr(_in,child);

            if(child == -1)
                return false;

            boneH.add_child(child);
        }
    }
    return true;
}

xBoneHiberarchy&  CCoreSkeleton::boneHiberarchy(int iBone)
{
	return m_BoneHibers[iBone];
}


bool CCoreSkeleton::loadBoneList(xcomdocstream* _in)
{
    int nBone = -1;
    Ts_ReadPtr(_in , nBone);
    if(nBone <= 0)
        return false;

    //首次读入Skeleton的ID.以后每次加载数据的时候要和这个ID进行验证
    //需要验证的数据是和Skeleton有绑定关系的数据。
    //主要数据如下: 一): 动作数据 二) 
    Ts_ReadPtr(_in , m_SkeletonID);
    bool bUseBeforeSkeletonPose;
    Ts_ReadPtr(_in , bUseBeforeSkeletonPose);


    for(int i = 0 ; i < nBone ; i ++)
    {
        xSkinBone bone;
        //判断wchar_t的长度
        if(sizeof(wchar_t) == 2)
        {
            Ts_ReadPtr(_in , bone);
        }
        m_Bones.push_back(bone);
    }
    m_nBone = (int)m_Bones.size();
    float fScale = 0.0f;
    Ts_ReadPtr(_in , fScale);
    return true;
}

void     Quaternial2Matrix(D3DXQUATERNION& qt , D3DXMATRIX& mOut)
{
#define M(row,col)  mOut.m [row ] [ col ]

    float x2 = qt.x * qt.x;
    float y2 = qt.y * qt.y;
    float z2 = qt.z * qt.z;
    float w2 = qt.w * qt.w;
    float xy = 2.0f * qt.x * qt.y;
    float wz = 2.0f * qt.w * qt.z;
    float wy = 2.0f * qt.w * qt.y;
    float xz = 2.0f * qt.x * qt.z;
    float yz = 2.0f * qt.y * qt.z;
    float wx = 2.0f * qt.x * qt.w;

    M(0,0) = 1.0f - 2.0f * (y2+z2);
    M(1,0) = (xy + wz);
    M(2,0) = (xz - wy);
    M(3,0) = 0;

    M(0,1) = (xy - wz);
    M(1,1) = 1.0f - 2.0f * (x2 + z2);
    M(2,1) = (yz + wx);
    M(3,1) = 0;

    M(0,2) = (xz + wy);
    M(1,2) = (yz - wx);
    M(2,2) = 1.0f - 2.0f * (x2 + y2);
    M(3,2) = 0;

    M(0,3) = 0;
    M(1,3) = 0;
    M(2,3) = 0;
    M(3,3) = 1;
#undef M
}

void CBoneTrans::toMatrix(D3DXMATRIX& mat)
{
    //暂时忽略了缩放
    D3DXMATRIX scaleMat;
    D3DXMATRIX rotMat;
    Quaternial2Matrix(m_Rotate , rotMat);
    //m_Rotate.toMatrix(rotMat);
    D3DXMatrixIdentity(&scaleMat);

    //scaleMat.identity();
    scaleMat.m[0][0] = m_Scale.x;
    scaleMat.m[1][1] = m_Scale.y;
    scaleMat.m[2][2] = m_Scale.z;

    D3DXMatrixMultiply(&mat , &scaleMat , &rotMat );
    //XM_Mul(scaleMat , rotMat  , mat);
    
    //m_Rotate.toMatrix(mat);
    //mat.scale(this->m_Scale.x , m_Scale.y , m_Scale.y );

    mat.m[3][0] = m_Trans.x;
    mat.m[3][1] = m_Trans.y;
    mat.m[3][2] = m_Trans.z;

    //mat.setTransform(m_Trans.x , m_Trans.y , m_Trans.z);
    return ;
}




//bool makeTransData(xBoneData& boneData)
//{
//     xmat4 wsMat = boneData.m_LocaleTM ; //pBone->m_InitMT * boneData.m_Matrix;
//     xeuler _euler;
//     _euler.fromMatrix(wsMat);
//     xquat quat ;
//     quat.fromEuler(_euler);
//     boneData.m_BoneTrans.m_Rotate.fromEuler(_euler);
//     boneData.m_BoneTrans.m_Scale = boneData.m_BoneTrans.m_Scale;
//     boneData.m_BoneTrans.m_Trans.x = wsMat.m30;
//     boneData.m_BoneTrans.m_Trans.y = wsMat.m31;
//     boneData.m_BoneTrans.m_Trans.z = wsMat.m32;
//     return true;
//}

bool CCoreSkeleton::blendSlerp(CBaseAction* pAction, CBaseActionState* pActionState , CCoreActionAttr* pAttr ,  CCoreActionFrame& skeletonFrame,int parent)
{
    D3DXMATRIX parentMT;
    D3DXMATRIX parentMTInv;
    xBoneHiberarchy* pBoneH = NULL;
    if(parent != -1)
    {
        parentMT = skeletonFrame.m_FrameMat[parent];
        pBoneH = &m_BoneHibers[parent];
    }
    else
    {
        D3DXMatrixIdentity( &parentMT );
        //parentMT.identity();
        pBoneH = &m_RootHibers;

    }
    D3DXMATRIX localeMT;
    xBoneHiberarchy& boneH = *pBoneH;
    for(int i = 0 ; i < boneH.m_nChildren; ++i)
    {
        int     boneIndex = boneH.m_Childrens[i];
        xSkinBone*  pBone = &m_Bones[boneIndex];

		D3DXMATRIX& transMat       = skeletonFrame.m_FrameMat[boneIndex];
		D3DXMATRIX& wsMat          = skeletonFrame.m_WSMats[boneIndex];
		CBoneTrans& boneTrans = skeletonFrame.boneTrans(boneIndex);

		bool bHasBone = pAction->getBoneTrans(boneIndex , pActionState , boneTrans);
		boneTrans.toMatrix(localeMT);

		//骨骼不属于这个动作
        if(bHasBone == false || pAttr->boneUsed(boneIndex) == false )
		{
			blendSlerp(pAction,  pActionState , pAttr , skeletonFrame ,boneIndex);
			continue;
		}
 
        //mat = boneData1.m_Matrix;
        //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , mat);
        if(parent == -1)
        {
            //使用记录的变量插值
            wsMat      = localeMT;
            //XM_Mul(pBone->m_InitMTInv , localeMT , transMat);
            D3DXMatrixMultiply( &transMat , &pBone->m_InitMTInv , &localeMT );
            
            //直接使用夫节点
            //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , transMat);
            //XM_Mul(pBone->m_InitMT , transMat , wsMat);
        }
        else
        {
            /*
            xmat4 mat1 ,mat2;
            XM_Mul(pBone->m_InitMTInv , localeMT, mat1);
            XM_Mul(mat1 , m_Bones[parent].m_InitMT , mat2);
            XM_Mul(mat2 , parentMT , transMat);
            //因为顶点已经带有了 pBone->m_InitMT。 所以要先乘上 pBone->m_InitMTInv 。
            //另外，夫节点上带了 pBone->m_InitMTInv。 所以在parentMT前要乘上m_Bones[parent]->m_InitMT
            //mat = pBone->m_InitMTInv * localeMT * m_Bones[parent]->m_InitMT * parentMT;
            */
            //优化
            ////XM_Mul(localeMT , skeletonFrame.m_WSMats[parent] , wsMat);
            ////XM_Mul(pBone->m_InitMTInv , wsMat , transMat);

            D3DXMatrixMultiply( &wsMat    , &localeMT           , &skeletonFrame.m_WSMats[parent] );
            D3DXMatrixMultiply( &transMat , &pBone->m_InitMTInv , &wsMat  );
            //mat = pBone->m_InitMTInv * wsMat --> wsMat = localeMT * parentWsMT;

        }
        blendSlerp(pAction,  pActionState , pAttr , skeletonFrame ,boneIndex);
    }
    return true;
}
CCoreActionFrame*  CCoreSkeleton::createActionFrame()
{
	CCoreActionFrame* pActFrame = new CCoreActionFrame;
	pActFrame->load( nBone() );
	return pActFrame;
}

void  CCoreSkeleton::destoryActionFrame(CCoreActionFrame* pAcFrame)
{
	delete pAcFrame;
}
bool CCoreSkeleton::blendSlerp(CBaseAction* pAction, CBaseActionState* pActionState ,CCoreActionFrame& skeletonFrame , int parent)
{
    D3DXMATRIX parentMT;
    D3DXMATRIX parentMTInv;
    xBoneHiberarchy* pBoneH = NULL;
    if(parent != -1)
    {
        parentMT = skeletonFrame.m_FrameMat[parent];
        pBoneH = &m_BoneHibers[parent];
    }
    else
    {
        D3DXMatrixIdentity( &parentMT );
        //parentMT.identity();
        pBoneH = &m_RootHibers;

    }
    D3DXMATRIX localeMT;
    float tTime = 0.0f;
    xBoneHiberarchy& boneH = *pBoneH;
    for(int i = 0 ; i < boneH.m_nChildren; ++i)
    {
        int     boneIndex = boneH.m_Childrens[i];
        xSkinBone*  pBone = &m_Bones[boneIndex];


        D3DXMATRIX& transMat       = skeletonFrame.m_FrameMat[boneIndex];
        D3DXMATRIX& wsMat          = skeletonFrame.m_WSMats[boneIndex];
        CBoneTrans& boneTrans = skeletonFrame.boneTrans(boneIndex);
        assert(true == pAction->getBoneTrans(boneIndex , pActionState , boneTrans ) );
		boneTrans.toMatrix(localeMT);
        //      mat = boneData1.m_Matrix;
        //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , mat);

        if(parent == -1)
        {
            //使用记录的变量插值
            wsMat      = localeMT;
            D3DXMatrixMultiply( &transMat , &pBone->m_InitMTInv , &localeMT );
        }
        else
        {
            //优化
            D3DXMatrixMultiply( &wsMat    , &localeMT           , &skeletonFrame.m_WSMats[parent] );
            D3DXMatrixMultiply( &transMat , &pBone->m_InitMTInv , &wsMat  );
        }
        blendSlerp(pAction, pActionState , skeletonFrame ,boneIndex);
    }
    return true;
}


