#include "../xcomdoc/xcomdoc.h"
#include "../xcomdoc/xI18N.h"
#include "CoreAction.h"
#include "CoreSkeleton.h"
#include "CoreMesh.h"

//////////////////////////////////////////////////////////////////////////
xSkeletonAction::xSkeletonAction(CCoreSkeleton* pSkeleton , int param) : CBaseAction(pSkeleton , param)
{

}

bool xSkeletonAction::setTime(float _time)
{
    m_Info.m_lTime = 1000 * _time;
    return true;
}

float xSkeletonAction::getDurTime()
{
    return m_Info.m_lTime/1000.0f;
}
int xSkeletonAction::memUsage()
{
    return m_nBone * m_Info.m_nFrame * sizeof(xBoneData);
}
void xSkeletonAction::setBoneNumber(int _nBone)
{
    m_nBone = _nBone;
}


//CoreAction
CCoreAction::CCoreAction(CCoreSkeleton* pSkeleton , int param) : xSkeletonAction(pSkeleton , param)
{
	m_bLoopAction = true;
}
CCoreAction::~CCoreAction()
{

}

bool CCoreAction::getBoneTrans(int boneIndex, CBaseActionState* pActionState , CBoneTrans& boneTrans)
{
	float t = pActionState->m_fTime;
	int frame1 = pActionState->m_Frame1;
	int frame2 = pActionState->m_Frame2;
	CBoneTrans* _boneData1 = this->getBoneData(boneIndex, frame1);
	CBoneTrans* _boneData2 = this->getBoneData(boneIndex, frame2);
	if(_boneData1 == NULL || _boneData2 == NULL)
		return false;

	CBoneTrans& boneData1 = *_boneData1;
	CBoneTrans& boneData2 = *_boneData2;

	//插值旋转
	D3DXQUATERNION q1 = boneData1.m_Rotate;
	D3DXQUATERNION q2 = boneData2.m_Rotate;
    D3DXQuaternionSlerp(&boneTrans.m_Rotate , &q1 , &q2 , t);


	//插值平移
	D3DXVECTOR3& t1 = boneData1.m_Trans;
	D3DXVECTOR3& t2 = boneData2.m_Trans;
	boneTrans.m_Trans = t1*(1.0f - t) + t2*t;

	//插值平移
	D3DXVECTOR3& s1 = boneData1.m_Scale;
	D3DXVECTOR3& s2 = boneData2.m_Scale;
	boneTrans.m_Scale = s1*(1.0f - t) + s2*t;
	return true;
}


bool CCoreAction::blend(long _time_in_ms, CCoreActionFrame* _skeletonFrame , CCoreSkeleton* pSkeleton)
{
	CCoreActionFrame& skeletonFrame = *_skeletonFrame;
	m_ActionState.m_lTime = _time_in_ms;
	updateState(_time_in_ms , m_ActionState , m_bLoopAction );
    pSkeleton->blendSlerp(this, &m_ActionState , attribute() ,  skeletonFrame );
    return true;
}



CBoneTrans* CCoreAction::getBoneData(int boneIndex, int frame)
{
	if(frame == -1 || frame >= m_Info.m_nFrame) 
		frame = m_Info.m_nFrame - 1;
	return &m_BoneFrame[boneIndex][frame];
}



bool  CCoreAction::load(xXmlNode* pCfgNode )
{
	m_Info.m_lTime       = pCfgNode->int_value(L"DurTime")    ;//"5000"
	m_Info.m_iLastFrame  = pCfgNode->int_value(L"LastFrame")  ;//"77"
	m_Info.m_iFirstFrame = pCfgNode->int_value(L"FirstFrame") ;//"50"
	m_Info.m_nFrame      = pCfgNode->int_value(L"nFrame") ;//"nFrame"
	m_Info.m_eActType    = (eActionType)pCfgNode->int_value(L"ActionType") ;//"1"
	m_Name	     = pCfgNode->value(L"Name");
	m_SkeID.m_HiWord = 0;
	m_SkeID.m_LoWord = 0;
	m_nBone = 0;
	if(m_Info.m_nFrame == 0)
	{
		m_Info.m_nFrame      = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;  
	}
	return true;
}

bool  CCoreAction::load(const wchar_t * _name , istream& stream)
{
	m_Name	     = _name;
	stream.read((char*)&m_SkeID , sizeof(m_SkeID) );
	stream.read((char*)&m_nBone , sizeof(m_nBone) );
	stream.read((char*)&m_Info  , sizeof(m_Info) );
    istream::pos_type cur_pos = stream.tellg();
    stream.seekg(0 , ios::end );
    int len = (int)(stream.tellg() - cur_pos );
    stream.seekg(cur_pos , ios::beg);

	int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;
	assert(m_Info.m_nFrame == nFrame );
	
	int dim[2];
	dim[0] = m_nBone;
	dim[1] = m_Info.m_nFrame;

	m_BoneFrame.create(dim);
	m_ActionAttr.init(m_nBone);

    if(len > sizeof(CBoneTrans) * nFrame * m_nBone )
    {
        xBoneData  boneData ;
        for(int iFrame = 0 ; iFrame < nFrame ; iFrame ++ )
        {
            for(int iBone = 0 ; iBone < m_nBone ; iBone++)
            {
                stream.read((char*)&boneData , sizeof(xBoneData) );
                m_BoneFrame[iBone][iFrame] = boneData.m_BoneTrans;
            }
        }
    }
    else
    {
        for(int iFrame = 0 ; iFrame < nFrame ; iFrame ++ )
        {
            for(int iBone = 0 ; iBone < m_nBone ; iBone++)
            {
                CBoneTrans& boneTrans = m_BoneFrame[iBone][iFrame];
                stream.read((char*)&boneTrans , sizeof(CBoneTrans) );
            }
        }
    }
	return true;
}

bool CCoreAction::load(const wchar_t * _name , xcomdocstream* stream)
{
    m_Name	     = _name;
    stream->read(m_SkeID);
    stream->read(m_nBone);
    stream->read(m_Info);

    int cur_pos = stream->stream_tellr();
    stream->stream_seekr(xcdsd_end , 0 );
    int len = (int)(stream->stream_tellr() - cur_pos );
    stream->stream_seekr(xcdsd_beg , cur_pos );

    int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;
    assert(m_Info.m_nFrame == nFrame );
	
    int dim[2];
    dim[0] = m_nBone;
    dim[1] = nFrame;

    m_BoneFrame.create(dim);
	m_ActionAttr.init(m_nBone);

    if(len > sizeof(CBoneTrans) * nFrame * m_nBone )
    {
        xBoneData  boneData ;
        for(int iFrame = 0 ; iFrame < nFrame ; iFrame ++ )
        {
            for(int iBone = 0 ; iBone < m_nBone ; iBone++)
            {
                if(stream->read(boneData) == sizeof(boneData) )
                {
                    m_BoneFrame[iBone][iFrame] = boneData.m_BoneTrans;
                }
            }
        }
    }
    else //否则，只有BoneTrans数据
    {
        for(int iFrame = 0 ; iFrame < nFrame ; iFrame ++ )
        {
            for(int iBone = 0 ; iBone < m_nBone ; iBone++)
            {
                stream->read(m_BoneFrame[iBone][iFrame]);
            }
        }
    }
 
	return true;
	/*
    //write;
    fullName = fullName + L".new";
    xcomdocstream* newstream =doc.open_stream(fullName.c_str() ,false);
    if(newstream == NULL)
    return false;

    newstream->write(m_SkeID);
    newstream->write(m_nBone);
    newstream->write(m_Info);
    for(int iBone = 0 ; iBone < m_nBone ; iBone++)
    {
    for(int iFrame = 0 ; iFrame <= m_Info.m_iLastFrame ; iFrame ++ )
    {
    xBoneData& boneData = m_BoneFrame[iBone][iFrame];
    newstream->write(boneData);
    }
    }
    doc.close_stream(newstream);
    */
}
bool CCoreAction::load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir)
{
    std::wstring fullName = std::wstring(actionDir) + pCfgNode->name() + L".xra";
    m_Info.m_lTime       = pCfgNode->int_value(L"DurTime")    ;//"5000"
    m_Info.m_iLastFrame  = pCfgNode->int_value(L"LastFrame")  ;//"77"
    m_Info.m_iFirstFrame = pCfgNode->int_value(L"FirstFrame") ;//"50"
    m_Info.m_nFrame      = pCfgNode->int_value(L"nFrame") ;//"nFrame"
    m_Info.m_eActType    = (eActionType)pCfgNode->int_value(L"ActionType") ;//"1"

	xcomdocstream* stream =doc.open_stream(fullName.c_str() , true);
	if(stream == NULL)
	{
		//找没扩展名的
		fullName = std::wstring(actionDir) + pCfgNode->name();
		stream = doc.open_stream(fullName.c_str() , true);
	}

	if(stream == NULL)
		return false;

    load(pCfgNode->value(L"Name") , stream );
	doc.close_stream(stream);
    return true;
}



void CCoreAction::unload()
{
    m_BoneFrame.free();
}



