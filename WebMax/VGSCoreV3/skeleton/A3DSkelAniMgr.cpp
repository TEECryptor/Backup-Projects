#include "./a3dModel/CD3DInclude.h"
#include "./a3dModel/A3DModel.h"
#include "A3DSkelAniMgr.h"
#include "./xcomdoc/xI18N.h"
#include "./a3dModel/CoreMesh.h"

#if _DEBUG
//#include "../logManager/logManager.h"
#endif

BOOL CA3DModelLoader::SetCameraInfo(D3DXVECTOR3& up , D3DXVECTOR3& dir , D3DXVECTOR3& pos , float _Near , float _Far , float _Fov , float _fAspect)
{
    if(m_DeviceWrapper == NULL)
        return FALSE;

    m_DeviceWrapper->m_TransformBuffer.m_CameraInfo.m_fFar    = _Far;
    m_DeviceWrapper->m_TransformBuffer.m_CameraInfo.m_fAspect = _fAspect;
    m_DeviceWrapper->m_TransformBuffer.m_CameraInfo.m_fNear   = _Near;
    m_DeviceWrapper->m_TransformBuffer.m_CameraInfo.m_fFov    = _Fov;

    m_DeviceWrapper->m_TransformBuffer.m_CameraInfo.m_EyePos = D3DXVECTOR4(pos  , 1.0f);
    m_DeviceWrapper->m_TransformBuffer.m_CameraInfo.m_Up     = D3DXVECTOR4(up   , 1.0f);
    m_DeviceWrapper->m_TransformBuffer.m_CameraInfo.m_EyeDir = D3DXVECTOR4(dir  , 1.0f);

    m_DeviceWrapper->m_LightState.nLight[0] = m_DeviceWrapper->m_LightState.nLight[1] = m_DeviceWrapper->m_LightState.nLight[2] = m_DeviceWrapper->m_LightState.nLight[3] = 1;
    for(int i = 0 ; i < MAX_LIGHT ; i ++)
    {
        m_DeviceWrapper->m_LightState.m_Ambient [i]  = D3DXCOLOR(0.1f , 0.1f , 0.1f , 1.0f);
        m_DeviceWrapper->m_LightState.m_Diffuse [i]  = D3DXCOLOR(1.0f , 1.0f , 1.0f , 1.0f);
        m_DeviceWrapper->m_LightState.m_Speculer[i]  = D3DXCOLOR(1.0f , 1.0f ,1.0f  , 1.0f);
        m_DeviceWrapper->m_LightState.m_dir     [i]  = D3DXVECTOR4(1.0f , 0.0f , 0.0f , 0.0f);
        m_DeviceWrapper->m_LightState.m_Position[i]  = D3DXVECTOR4(0.0f , 0.0f , 0.0f , 1.0f);

    }
    return TRUE;
}

CA3DModelLoader::CA3DModelLoader( )
{
    m_OutStateBlock = NULL;
    m_DeviceWrapper = NULL;
	m_OldFVF = 0;
	m_pOldVDecel = NULL;
}

CA3DModelLoader::~CA3DModelLoader()
{

	 Uninit();



}
void CA3DModelLoader::OnDeviceLost()
{
	if( m_DeviceWrapper )
		m_DeviceWrapper->onDeviceLost();

    XSAFE_RELEASE(m_OutStateBlock);
}

void CA3DModelLoader::AfterDeviceReset(IDirect3DDevice9* pDevice)
{ 
	if( m_DeviceWrapper == NULL )
		return;

	if(pDevice == NULL)
		pDevice = m_DeviceWrapper->m_pDevice;

	//没有新创建Device;
	if(pDevice == m_DeviceWrapper->m_pDevice)
	{
		pDevice->CreateStateBlock(D3DSBT_ALL , &m_OutStateBlock);
		OutputDebugStringA("Device Lost没处理完毕，等待完善\n");
	}
	else
	{
		SetDevice(pDevice , NULL);
		//....
		//重新创建新的资源
		OutputDebugStringA("Device Lost没处理正确，重新创建了Device\n");
	}
	
	m_DeviceWrapper->afterDeviceLost();
}


void CA3DModelLoader::Uninit()
{
    //释放所有的模型
    CA3DModelMap::iterator pos = m_MapModels.begin();
    for(;pos != m_MapModels.end() ; pos ++)
    {
        CA3DModel* pModel = pos->second;
        XSAFE_RELEASE(pModel);
    }
    m_MapModels.clear();
	
    //释放所有的动画管理器
    CA3DSkelAnimationArray::iterator it = m_vSkelAnimations.begin();
    for(;it != m_vSkelAnimations.end() ; it ++)
    {
        CA3DSkelAnimation* pAni = it->second;
        XSAFE_RELEASE(pAni);
    }
    m_vSkelAnimations.clear();
    XSAFE_RELEASE(m_OutStateBlock);
	
    if(m_DeviceWrapper ) m_DeviceWrapper->Uninit();
    XSAFE_DELETE(m_DeviceWrapper);
	
}



void CA3DModelLoader::SetDevice(IDirect3DDevice9* pDevice , HINSTANCE hInstance )
{
    if(m_DeviceWrapper == NULL)
    {
        m_DeviceWrapper = new CA3DDeviceWrapper();		

    }

	if( hInstance == NULL )
	{
		XSAFE_RELEASE(m_OutStateBlock);
		return;
	}


    m_DeviceWrapper->Init(pDevice , hInstance);
    if(pDevice)
    {
        XSAFE_RELEASE(m_OutStateBlock);
        m_DeviceWrapper->m_pDevice->CreateStateBlock(D3DSBT_ALL , &m_OutStateBlock);
    }


}

CA3DModel* CA3DModelLoader::LoadModel(const char* modelFile, const void* buf , int bufLen)
{
    std::string modelFileName = modelFile;
    wchar_t wcsFileName[1024] = {0};
    XEvol_LocaleToUnicode(modelFile , wcsFileName , 1024);
    CA3DModelMap::iterator pos = m_MapModels.find(modelFileName);
    if(pos != m_MapModels.end() )
    {
        CA3DModel* pModel = pos->second;
        pModel->addPackage(wcsFileName , (const BYTE*)buf , bufLen);
        return pModel;
    }

    CA3DModel* pModel = new CA3DModel(m_DeviceWrapper , false);
    if(true == pModel->load(wcsFileName , (const BYTE*)buf , bufLen , true) )
    {
        m_MapModels.insert( CA3DModelMap::value_type(modelFileName , pModel) );
        return pModel;
    }
    delete pModel;
    return NULL;

}

CA3DModel*  CA3DModelLoader::FindModelByName(const char* _name)
{
    CA3DModelMap::iterator pos = m_MapModels.find(_name);
    if(pos != m_MapModels.end() )
    {
        CA3DModel* pModel = pos->second;
        return pModel;
    }
    return NULL;
}

CA3DSkelAnimation*  CA3DModelLoader::FindAnimationByName(const char* _name)
{
    CA3DSkelAnimationArray::iterator pos = m_vSkelAnimations.find(_name);
    if(pos != m_vSkelAnimations.end() )
    {
        CA3DSkelAnimation* pAni = pos->second;
        return pAni;
    }
    return NULL;
}


CA3DSkelAnimation* CA3DModelLoader::getMouseHitSkeleton( D3DXVECTOR3 *orig,  D3DXVECTOR3* dir  )
{

    CA3DSkelAnimationArray::iterator it = m_vSkelAnimations.begin();
    for(;it != m_vSkelAnimations.end() ; it ++)
    {
        CA3DSkelAnimation* pAni = it->second;

		D3DXMATRIX matrix = pAni->m_PosMatrix ;	
		D3DXVECTOR3 v, vmin, vmax;
		v.x = matrix._41;
		v.y = matrix._42;
		v.z = matrix._43;


		vmin =   pAni->m_BoundBox.m_Min + v + D3DXVECTOR3(90,0,0) ;
		vmax =   pAni->m_BoundBox.m_Max + v +  D3DXVECTOR3(90,0,0 ) ;
		if(  D3DXBoxBoundProbe( &( vmin) , &(vmax) , orig, dir) )
			return pAni;        
    }

	return	NULL;
}


CA3DSkelAnimation* CA3DModelLoader::CreatekelAnimation(const char* modelFile , const char* _name , const void* buf , int bufLen , D3DXVECTOR3& pos , D3DXVECTOR3& scale , D3DXVECTOR4& rot)
{
    CA3DSkelAnimation* pAni = FindAnimationByName(_name);
    if(pAni )
        return pAni;

    CA3DModel* pModel = LoadModel(modelFile , buf , bufLen);
    if(pModel == NULL)
        return NULL;
    pModel->AddRef();
    
    pAni = new CA3DSkelAnimation(_name , this , pModel , TRUE);
    m_vSkelAnimations[_name ] = ( pAni );
	
    pAni->SetTransform(pos , scale , rot);
    return pAni;
}

bool CA3DModelLoader::IsMaterialSupport(const char* _file)
{
    wchar_t wcsFileName[1024] = {0};
    XEvol_LocaleToUnicode(_file , wcsFileName , 1024);
    if( m_DeviceWrapper->FindShader(wcsFileName) )
        return true;
    return false;
}

void CA3DModelLoader::RemoveSkelAnimation(CA3DSkelAnimation* pAni)
{
    const char* _name = pAni->name();
    CA3DSkelAnimationArray::iterator pos = m_vSkelAnimations.find(_name);
    if(m_vSkelAnimations.end() != pos)
    {
        CA3DSkelAnimation* pAni = pos->second;
        pAni->Release();
        m_vSkelAnimations.erase( pos );
    }
    return ;
}

void CA3DModelLoader::ClearAllAnition()
{
    CA3DSkelAnimationArray::iterator pos = m_vSkelAnimations.begin();
    for( ; pos != m_vSkelAnimations.end() ; pos ++)
    {
        CA3DSkelAnimation* pAni = pos->second;
        pAni->Release();
    }
    m_vSkelAnimations.clear();
    return ;
}


bool CA3DModelLoader::IsInFrustrum(D3DXVECTOR3* pos, D3DXMATRIX* pMatView, float tanHalfFieldOfViewV,  float nearClip, float farClip )
{

	//如果相机在当前模型包围球的外部，得到包围球中心在View空间中的位置
	D3DXVECTOR3 SCInView;
	D3DXVec3TransformCoord(&SCInView, pos, pMatView);
	

	//看物体是否在视锥体内
	//ZPlane
	//前后剪裁面
	if (SCInView.z > farClip || SCInView.z < nearClip)
		return false;
	
	//YPlane-上下剪裁面
	if (SCInView.y/SCInView.z < -tanHalfFieldOfViewV)
		return false;
	if (SCInView.y/SCInView.z > tanHalfFieldOfViewV)
		return false;
	
	//XPlane
	float tanHalfFieldOfViewH = tanHalfFieldOfViewV *  1399/662.0f;
	if (SCInView.x/SCInView.z > tanHalfFieldOfViewH)
		return false;
	if (SCInView.x/SCInView.z < -tanHalfFieldOfViewH)
		return false;
	
	return true;


}

void    CA3DModelLoader::FrustrumTestA3D(D3DXVECTOR3* pCamPos, D3DXMATRIX* pMatView,
									float tanHalfFieldOfViewV, float nearClip, float farClip)
{



	CA3DSkelAnimationArray::iterator pos = m_vSkelAnimations.begin();

	for( ; pos != m_vSkelAnimations.end() ; pos ++)
    {
        CA3DSkelAnimation* pAni = pos->second;

		
		D3DXMATRIX matrix = pAni->m_PosMatrix ;
		
		D3DXVECTOR3 v, vmin , vmax;
		v.x = matrix._41;
		v.y = matrix._42;
		v.z = matrix._43;

		bool bIn = IsInFrustrum(&v, pMatView, tanHalfFieldOfViewV, nearClip, farClip );
		if( bIn)
		{
			pAni->SetCulling(false);   
			continue;

		}

		vmin =   pAni->m_BoundBox.m_Min + v ;//+ D3DXVECTOR3(90,0,0) ;
		vmax =   pAni->m_BoundBox.m_Max + v ;//+  D3DXVECTOR3(90,0,0 ) ;

		bIn = IsInFrustrum(&vmin, pMatView, tanHalfFieldOfViewV, nearClip, farClip );
		if( bIn)
		{
			pAni->SetCulling(false);   
			continue;
		}

		
		bIn = IsInFrustrum(&vmax, pMatView, tanHalfFieldOfViewV, nearClip, farClip );
		if( bIn)
		{
			pAni->SetCulling(false);   
			continue;			
		}
		
		pAni->SetCulling(true);   
		

	}
}





void CA3DModelLoader::DrawAllEnabledAnimation(DWORD passedTime)
{
	if (0 == m_DeviceWrapper || m_vSkelAnimations.empty())
		return;
	
	IDirect3DVertexDeclaration9* pDecl = 0;
	m_DeviceWrapper->m_pDevice->GetVertexDeclaration(&pDecl);

    BeginDraw();
   
    CA3DSkelAnimationArray::iterator pos = m_vSkelAnimations.begin();
    for( ; pos != m_vSkelAnimations.end() ; pos ++)
    {
        CA3DSkelAnimation* pAni = pos->second;
        if(/* pAni->IsEnable()  && */  pAni->IsVisible()  && !pAni->IsCulling() )
        {
            pAni->Draw(passedTime );
        }
    }

    EndDraw();

	m_DeviceWrapper->m_pDevice->SetVertexDeclaration(pDecl);
	if( pDecl) 
	{
		pDecl->Release();
		pDecl = NULL;
	}

}

void CA3DModelLoader::BeginDraw()
{

      //保存当前的状态
    if(m_OutStateBlock == NULL)
	{
		m_DeviceWrapper->m_pDevice->CreateStateBlock(D3DSBT_ALL , &m_OutStateBlock);		
	}
	m_OutStateBlock->Capture();
	m_DeviceWrapper->m_pDevice->GetFVF(&m_OldFVF);
	m_DeviceWrapper->m_pDevice->GetVertexDeclaration(&m_pOldVDecel);

    m_DeviceWrapper->m_pDevice->GetTransform(D3DTS_VIEW       , & m_DeviceWrapper->m_TransformBuffer.m_matView    );
    m_DeviceWrapper->m_pDevice->GetTransform(D3DTS_PROJECTION , & m_DeviceWrapper->m_TransformBuffer.m_matProject );

    m_DeviceWrapper->m_pDevice->SetRenderState(D3DRS_BLENDOP      , D3DBLENDOP_ADD);
    m_DeviceWrapper->m_pDevice->SetRenderState(D3DRS_BLENDOPALPHA , D3DBLENDOP_ADD);
    m_DeviceWrapper->m_pDevice->SetRenderState(D3DRS_SRCBLEND     , D3DBLEND_SRCALPHA);
    m_DeviceWrapper->m_pDevice->SetRenderState(D3DRS_DESTBLEND    , D3DBLEND_INVSRCALPHA);
    m_DeviceWrapper->m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE    , TRUE);
    m_DeviceWrapper->m_pDevice->SetRenderState(D3DRS_ALPHAFUNC          , D3DCMP_GREATEREQUAL);
    m_DeviceWrapper->m_pDevice->SetRenderState(D3DRS_ALPHAREF           , 64);

    IDirect3DDevice9* pDevice = m_DeviceWrapper->m_pDevice;
    DWORD bLight = FALSE;
    pDevice->GetRenderState(D3DRS_LIGHTING , &bLight);

	m_DeviceWrapper->m_LightState.nLight[0] = 0;


    if(bLight != FALSE)
    {
         int lightIdx = 0;
         for(int i = 0 ; i < 8 ; i ++)
         {
             BOOL bEnble  = FALSE;
             pDevice->GetLightEnable(i , &bEnble);
             if(bEnble)
             {
                 D3DLIGHT9 LightInfo;
                 pDevice->GetLight(i , &LightInfo);
                 m_DeviceWrapper->m_LightState.m_Ambient [lightIdx]= D3DXCOLOR(LightInfo.Ambient);
                 m_DeviceWrapper->m_LightState.m_Diffuse [lightIdx]= D3DXCOLOR(LightInfo.Diffuse);                 
                 m_DeviceWrapper->m_LightState.m_Speculer[lightIdx]= D3DXCOLOR(LightInfo.Specular);

                 m_DeviceWrapper->m_LightState.m_dir     [lightIdx]= D3DXVECTOR4(LightInfo.Direction , 0.0f );
                 m_DeviceWrapper->m_LightState.m_Position[lightIdx]= D3DXVECTOR4(LightInfo.Position  , 0.0f );

                 lightIdx ++;
                 if(lightIdx == 4)
                     break;
             }
         }
         m_DeviceWrapper->m_LightState.nLight[0] = lightIdx;
    }
    else
    {
		m_DeviceWrapper->m_LightState.nLight[0] = 1;
		m_DeviceWrapper->m_LightState.m_Ambient [0]= D3DXCOLOR(1.0f , 1.0f , 1.0f , 1.0f);
		m_DeviceWrapper->m_LightState.m_Diffuse [0]= D3DXCOLOR(0.0f , 0.0f , 0.0f , 1.0f);                 
		m_DeviceWrapper->m_LightState.m_Speculer[0]= D3DXCOLOR(0.0f , 0.0f , 0.0f , 1.0f);
		
		m_DeviceWrapper->m_LightState.m_dir     [0]= D3DXVECTOR4(1.0,1.0f,1.0f,1.0f );
		m_DeviceWrapper->m_LightState.m_Position[0]= D3DXVECTOR4(1.0,1.0f,1.0f,1.0f );


  
    }
}

//绘制结束
void CA3DModelLoader::EndDraw()
{
    if(m_OutStateBlock) m_OutStateBlock->Apply();
	m_DeviceWrapper->m_pDevice->SetPixelShader(NULL);
	m_DeviceWrapper->m_pDevice->SetVertexShader(NULL);
	m_DeviceWrapper->m_pDevice->SetFVF(m_OldFVF);
	if(m_pOldVDecel)
	{
		m_DeviceWrapper->m_pDevice->SetVertexDeclaration(m_pOldVDecel);
		XSAFE_RELEASE(m_pOldVDecel);
	}	


}

//////////////////////////////////////////////////////////////////////////
class xActionStateListenner : public IActionTrackListenner
{
public:
	virtual void onActionTrackEvent(eActionTrackEvent _event, int idx ,  CBaseAction* pAction)
	{
		m_pAniMgr->onActionTrackEvent(_event , idx , pAction);
	}
public:
	CA3DSkelAnimation* m_pAniMgr;
};


CA3DSkelAnimation::CA3DSkelAnimation(const char* _name , CA3DModelLoader* pModelMgr , CA3DModel* pModel , BOOL bAddMeshs)
{
//	m_pFxShader = NULL;

	m_strShaderName = L"";
    m_bGpuSkin      = false;


    m_name = _name;
    m_DeviceWrapper = pModelMgr->GetDevice();
    D3DXMatrixIdentity(&m_PosMatrix);
    m_ActionFrame = NULL;
    m_pSkeleton   = NULL;
    m_BoundBox.Init();
    m_bEnable = TRUE;
    m_bVisible = TRUE;

	m_bCulling = false;

	m_nMoveStep =0;

    m_pMixedAction = NULL;
    m_pModel = pModel;
    m_pSkeleton = pModel->skeleton();
    if(m_pSkeleton)
    {
        m_ActionFrame = m_pSkeleton->createActionFrame();
    }
    if(bAddMeshs)
    {
        int _nMesh = (int)pModel->nMesh();
        for(int i = 0 ; i < _nMesh ; i ++)
        {
            CCoreMesh* pMesh = pModel->getMesh(i);
            AddMesh(pMesh);
        }
        _BuildBoundBox();

        int _nAction = (int)pModel->nAction();
        for(int j = 0 ; j < _nAction ; j ++)
        {
            CBaseAction* pAction = pModel->coreAction(j);
            AddAction(pAction);
        }

        
    }

	xActionStateListenner* pListener = new xActionStateListenner();
	pListener->m_pAniMgr = this;
    m_TrackListenner = pListener;

    CActionTrack* pTrack = new CActionTrack(m_pModel , L"Main"  , 0 );
    pTrack->addListenner(m_TrackListenner);
    m_vActionTrack.push_back(pTrack);

    if(m_vActions.size() > 0 )
    {
        SetAction(0 , -1 );
    }

    //设置默认的,从高级到低级一个个来
    if(SetEffectShader("PhongGpuSkin") == true) return ;  
}

void CA3DSkelAnimation::onActionTrackEvent(eActionTrackEvent _event, int idx ,  CBaseAction* pAction)
{

}



CA3DSkelAnimation::~CA3DSkelAnimation()
{

    for(int i = 0 ; i < (int)m_vMeshs.size() ; i ++)
    {
        XSAFE_RELEASE(m_vMeshs[i]);
    }
    m_vMeshs.clear();

    for(int j = 0 ; j < (int)m_vActions.size() ; j ++)
    {
        XSAFE_RELEASE(m_vActions[j]);
    }
    m_vActions.clear();

    m_pModelMgr->RemoveSkelAnimation(this);

	XSAFE_DELETE(m_TrackListenner);




 
}


BOOL CA3DSkelAnimation::IsEnable()
{
    return m_bEnable;

}
void CA3DSkelAnimation::Enable(BOOL bEnable)
{
    m_bEnable = bEnable;
}

BOOL CA3DSkelAnimation::IsVisible()
{
    return m_bVisible;
}
void CA3DSkelAnimation::SetVisible(BOOL bVisible)
{
    m_bVisible = bVisible;
}
void CA3DSkelAnimation::updateTransform( DWORD passedTime )
{

	if(m_pMixedAction == NULL)
    {
		CActionTrack* pTrack = m_vActionTrack[0];
		CBaseAction* pAction = pTrack->getCurrentAction();
		pTrack->update(passedTime  , m_pSkeleton);
		if(pAction)
		{
			pAction->blend(pTrack->actionTime() , m_ActionFrame , m_pSkeleton );
		}
    }
    else
    {
        //上下身动作融合的时候，需要用的
		
    }
	
    m_pModel->setupAnimation(m_ActionFrame);
    
    
    //拷贝Bone到Device里。
    m_DeviceWrapper->setBoneFrames( (D3DXMATRIX*)m_ActionFrame->data() , m_ActionFrame->nBone() );
    //准备好Transform。
    m_DeviceWrapper->m_TransformBuffer.m_matWorld = m_PosMatrix;
	
	m_DeviceWrapper->m_pDevice->SetTransform(D3DTS_WORLD, &m_PosMatrix);


	float  dis = D3DXVec3Length(&( m_Pos - m_destPos ) );


	if( m_nMoveStep > 0  )
	{
		D3DXVECTOR3 vOffset = m_destPos- m_Pos ;

		D3DXVECTOR3  vNewPos = m_Pos + vOffset/m_nMoveStep; 
		m_nMoveStep --;

		Sleep(0);
		this->SetPosition( vNewPos, true ) ;

		if( m_nMoveStep == 0 )
		{
	//		SetAction(5 , -1);
		}
	}
	return ;

}



BOOL CA3DSkelAnimation::Draw(DWORD passedTime)
{
	// fuxb 2010-05-12 ==>
	updateTransform(passedTime);	

	_drawAllMesh(!m_bGpuSkin);
    return TRUE;



    //设置Shader.
    CFxShader* pShader = m_pFxShader;
    if(pShader)
    {
        OutputDebugStringA("Fx Shader Drawing............\n");
        pShader->bind();
        pShader->setTechnique("Main");
        size_t nPass = 0;
        pShader->begin(&nPass , 0);
        for(size_t iPass = 0 ; iPass < nPass ; iPass ++)
        {
            pShader->beginPass(iPass);
            _drawAllMesh(false);
            pShader->endPass();

        }
        pShader->unbind();
    }
    else
    {
        _drawAllMesh(true);
    }


    return true;
}



bool CA3DSkelAnimation::SetEffectShader(const char* file)
{
    CFxShader* pShader = NULL;
    if(file == NULL)
    {
        m_strShaderName  = L"";
        m_bGpuSkin       = false;
    }
	else
	{
		wchar_t wcsFileName[64] = {0};
		XEvol_LocaleToUnicode(file , wcsFileName , 64);
		pShader = m_DeviceWrapper->FindShader(wcsFileName);

        //如果不是特定的Shader。则可以支持加载Shader
        if(pShader == NULL)
            pShader = m_DeviceWrapper->addShader(wcsFileName , wcsFileName);

        if(pShader && pShader->HasGpuSkin() != m_pModel->HasGpuSkin() )
        {
            pShader = NULL;

        }
		else if(pShader != NULL &&  pShader->HasGpuSkin() == m_pModel->HasGpuSkin() )
		{
			m_strShaderName = wcsFileName;
            m_bGpuSkin      = m_pModel->HasGpuSkin();

		}
	}
	if(pShader == NULL)
	{
		_ToCpuSkin(true);

	}
	else
	{
		_ToCpuSkin(false);
	}

    size_t _nMeshs = m_vMeshs.size();
    for(size_t i = 0 ; i <  _nMeshs; i ++)
    {
        m_vMeshs[i]->SetShader( m_strShaderName.c_str() );
    }
    return true;
} 


void   CA3DSkelAnimation::_drawAllMesh(bool bCpuSkin) 
{
    size_t _nMeshs = m_vMeshs.size();
    for(size_t i = 0 ; i <  _nMeshs; i ++)
    {
        m_vMeshs[i]->draw(m_pSkeleton , m_ActionFrame , bCpuSkin);
    }
}

void   CA3DSkelAnimation::_ToCpuSkin(bool bCpuSkin) 
{
	size_t _nMeshs = m_vMeshs.size();
	for(size_t i = 0 ; i <  _nMeshs; i ++)
	{
		if(bCpuSkin) m_vMeshs[i]->ToCpuSkin();
	}
}

VOID  CA3DSkelAnimation::SetPosition(const D3DXVECTOR3& pos, bool bMoving)
{
	SetTransform(pos, m_Scale, m_Rotate);

	if(!bMoving )
	{
		m_destPos = pos;
	}
}

VOID  CA3DSkelAnimation::SetScale(const D3DXVECTOR3& scale)
{
	SetTransform(m_Pos, scale, m_Rotate);
}

VOID  CA3DSkelAnimation::SetRotation(const D3DXVECTOR4& rot)
{	
	SetTransform(m_Pos, m_Scale, rot);
}

BOOL  CA3DSkelAnimation::SetTransform(const D3DXVECTOR3& pos , const D3DXVECTOR3& scale , const D3DXVECTOR4& rot )
{
	m_Pos    = pos;
	m_Scale  = scale;
	m_Rotate = rot;

	D3DXMATRIX mat ;
	D3DXMATRIX rotMat;
	D3DXMATRIX scaleMat;
	D3DXMATRIX transMat;

	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&scaleMat);
	D3DXMatrixIdentity(&transMat);

	D3DXMatrixTranslation(&transMat , pos.x , pos.y , pos.z );      
	D3DXMatrixRotationAxis(&rotMat , (D3DXVECTOR3*)&m_Rotate , m_Rotate.w );
	D3DXMatrixScaling(&scaleMat  , scale.x , scale.y , scale.z );

	D3DXMatrixMultiply(&mat , &scaleMat , &rotMat);
	D3DXMatrixMultiply(&m_PosMatrix , &mat , &transMat); // v * scale * rot * trans //

	return TRUE;

}




//未实现
BOOL CA3DSkelAnimation::AddMesh(CCoreMesh* pMesh)
{
    m_vMeshs.push_back( pMesh );
    pMesh->AddRef();
    return NULL;
}

BOOL CA3DSkelAnimation::AddAction(CBaseAction* pAction)
{
    m_vActions.push_back( pAction );
    pAction->AddRef();
    return NULL;
}


BOOL CA3DSkelAnimation::RemoveMesh(CCoreMesh* pMesh)
{
    std::vector<CCoreMesh*>::iterator pos = m_vMeshs.begin();
    for(; pos != m_vMeshs.end() ; pos ++ )
    {
        if(*pos == pMesh)
        {
            XSAFE_RELEASE(pMesh);
            m_vMeshs.erase(pos);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL CA3DSkelAnimation::RemoveAction(CBaseAction* pAction)
{
    std::vector<CBaseAction*>::iterator pos = m_vActions.begin();
    for(; pos != m_vActions.end() ; pos ++ )
    {
        if(*pos == pAction)
        {
            XSAFE_RELEASE(pAction);
            m_vActions.erase(pos);
            return TRUE;
        }
    }
    return FALSE;
}



CBaseAction* CA3DSkelAnimation::_findAction(const char* _name)
{
    if(_name == NULL) return NULL;

    wchar_t _wcsName[64] = {0};
    XEvol_LocaleToUnicode(_name , _wcsName , 64);
    std::wstring wcsName = _wcsName;
    for(size_t i  = 0 ; i < m_vActions.size() ; i ++)
    {
        if(m_vActions[i]->name() == wcsName )
        {
            return m_vActions[i];
        }
    }

    //或许，我们在这里要去资源管理器里去加载这个动作，不过目前还没有决定好资源的布局。
    //直接返回失败。
    return NULL;
}
//等待实现
BOOL CA3DSkelAnimation::SetAction(int ActionIdx , int iLoopTime , int TrackIdx)
{
    if(ActionIdx >= m_vActions.size() )
        return FALSE;

	CBaseAction* pAction = m_vActions[ActionIdx];
    if(pAction == NULL ) return FALSE;


    vActionList vList;
    vList.push_back(m_vActions[ActionIdx]);

    CActionTrack* pTrack = m_vActionTrack[TrackIdx];
//    pTrack->TimeCtller()->setLoopCtrl(0);

	float Fps = pAction->info()->m_lTime / (float)( pAction->info()->m_nFrame );
	int _timeA = pAction->info()->m_iFirstFrame *Fps;
	int _timeB = pAction->info()->m_iLastFrame *Fps;

	pTrack->TimeCtller()->setLoopCtrl(_timeA , _timeB , iLoopTime);


	pTrack->setActionList(vList , iLoopTime );
    return TRUE;
}

BOOL CA3DSkelAnimation::SetActionTransitionTime(DWORD _transTime , int TrackIdx)
{
    CActionTrack* pTrack = m_vActionTrack[TrackIdx];
    pTrack->setTransitionTime(_transTime);
    return TRUE;
}

//播放一个动作
BOOL CA3DSkelAnimation::SetAction(const char* _actionName , int iLoopTime , int TrackIdx )
{
      CBaseAction* pAction = _findAction(_actionName);
      if(pAction == NULL ) return FALSE;

      vActionList vList;
      vList.push_back( pAction );

      CActionTrack* pTrack = m_vActionTrack[TrackIdx];
   //   pTrack->TimeCtller()->setLoopCtrl(0);

	  float Fps = pAction->info()->m_lTime / (float)( pAction->info()->m_nFrame );
	  int _timeA = pAction->info()->m_iFirstFrame *Fps;
	  int _timeB = pAction->info()->m_iLastFrame *Fps;

	  pTrack->TimeCtller()->setLoopCtrl(_timeA , _timeB , iLoopTime);


	  pTrack->setActionList(vList , iLoopTime );      
      return TRUE;

}

//播放一连串动作
//_actionList    : 用 / 隔开动作，比如 "Action1/Action2/Action3"
//bUseTransition : 两个动作是否有过渡

bool ToWordList(const char* strWordList , std::vector<std::string>& vWordList)
{
    std::string actName;
    const char* text = strWordList;
    while(*text)
    {
        if( *text == '/' || *text == '\\')
        {
            if(actName.length()>0)
            {
                vWordList.push_back(actName);
                actName  = "";
            }
        }
        else
        {
            actName = (*text); //tim
        }
        text ++;
    }

    if(actName.length() >0 )
    {
        vWordList.push_back(actName);
        actName  = "";
    }

    if(vWordList.size() > 0)
    {
        return true;
    }
    return false;
}

BOOL CA3DSkelAnimation::SetActionList(const char* _actionLists , int iLoopTime , BOOL bUseTransition , int TrackIdx )
{
        if(_actionLists == NULL)
            return FALSE;

        vActionList vList;

        std::vector<std::string> vActNameList;
        ToWordList(_actionLists , vActNameList);

        for(size_t  i = 0 ; i < vActNameList.size() ; i ++)
        {
            std::string& _actName = vActNameList[i];
            CBaseAction* pAction = _findAction( _actName.c_str() );
            if(pAction != NULL )
            {
                vList.push_back( pAction );
            }
            else
            {
                OutputDebugStringA("Action not found :");
                OutputDebugStringA(_actName.c_str() );
                OutputDebugStringA("\n");
            }
        }
        if(vList.size() == 0)
        {
            return FALSE;
        }

        CActionTrack* pTrack = m_vActionTrack[TrackIdx];
        pTrack->TimeCtller()->setLoopCtrl(0);
		pTrack->setActionList(vList , iLoopTime , bUseTransition );        
        return TRUE;
}

//播放一个动作的第a到b帧
//FrameA : 播放的起始帧
//FrameB : 播放的结束帧
//iLoopTime : 循环次数， -1 表示无限循环
BOOL CA3DSkelAnimation::SetAction(int ActionIdx           , int frameA , int frameB , int iLoopTime , int TrackIdx)
{
    if(ActionIdx >= m_vActions.size() )
        return FALSE;

    CBaseAction* pAction = m_vActions[ActionIdx];
    if(pAction == NULL ) return FALSE;

    vActionList vList;
    vList.push_back( pAction );

    CActionTrack* pTrack = m_vActionTrack[TrackIdx];

    float Fps = pAction->info()->m_lTime / (float)( pAction->info()->m_nFrame );
    int _timeA = frameA * Fps;
    int _timeB = frameB * Fps;
    pTrack->TimeCtller()->setLoopCtrl(_timeA , _timeB , iLoopTime);
    pTrack->setActionList(vList , FALSE );
    return TRUE;
}

//播放一个动作的第a到b帧
BOOL CA3DSkelAnimation::SetAction(const char* _actionName , int frameA , int frameB , int iLoopTime, int TrackIdx)
{
    CBaseAction* pAction = _findAction(_actionName);
    if(pAction == NULL ) return FALSE;

    vActionList vList;
    vList.push_back( pAction );

    CActionTrack* pTrack = m_vActionTrack[TrackIdx];

    float Fps = pAction->info()->m_lTime / (float)( pAction->info()->m_nFrame );
    int _timeA = frameA * Fps;
    int _timeB = frameB * Fps;
    pTrack->TimeCtller()->setLoopCtrl(_timeA , _timeB , iLoopTime);
    pTrack->setActionList(vList , FALSE );
    return TRUE;
}


//类似于goto制定
VOID  CA3DSkelAnimation::MovetoPosition(const D3DXVECTOR3& pos, int step)
{
	m_destPos = pos;	
	m_nMoveStep = step;
}

////////////////////////////////////



void CA3DSkelAnimation::_BuildBoundBox()
{
    m_BoundBox.Init();
    int _nMesh = (int)m_vMeshs.size() ;
    for(int i = 0 ; i < _nMesh ; i ++)
    {
        CCoreMesh* pMesh =m_vMeshs[i];
        const CBoundBox& _aabb = pMesh->aabb();
        m_BoundBox.AddPoint(_aabb.m_Max);
        m_BoundBox.AddPoint(_aabb.m_Min);
    }
}