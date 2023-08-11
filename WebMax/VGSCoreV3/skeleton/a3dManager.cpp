#include "a3dManager.h"
#include "..\CSysFunc.h"
#include "A3DHelperFunction.h"


void CA3DManager::setSkeletonResourcePath(const std::string &sPath)
{
	A3D_SetResourcePath(sPath.c_str());
}

void CA3DManager::render(long timeInterval)
{
	m_A3DModelLoader.DrawAllEnabledAnimation(timeInterval);
}

void CA3DManager::loadA3DFile(const std::string &sFileName ,  const std::string &sMeshName  )
{
	FILE* fp =  fopen(sFileName.c_str() , "rb");
    if(fp != NULL)
    {
        fseek(fp , 0 , SEEK_END);
        int len = ftell(fp);
        fseek(fp , 0 , SEEK_SET);
        unsigned char* fBuf = new unsigned char[len];
        fread(fBuf , 1 , len , fp);
        fclose(fp);
		
		// 设置当前路径
		std::string strPath = _GetPureFilePathStr(sFileName);
		setSkeletonResourcePath(strPath);

        D3DXVECTOR3 vPos(0.0f , 0.0f , 0.0f);
        D3DXVECTOR3 vScale(1.0f , 1.0f , 1.0f);
		D3DXVECTOR4 vRot(0.0f , 0.0f , 0.0f , 0/*3.14f/2.0*/);
		std::string sSkeleAnimName = _GetPureFileNameStrWithoutExtention(sFileName);
        CA3DSkelAnimation *pAniation = m_A3DModelLoader.CreatekelAnimation(sFileName.c_str(), sMeshName.c_str(), fBuf , len , vPos , vScale ,  vRot );
        pAniation->SetVisible(TRUE);
        pAniation->Enable(TRUE);
        pAniation->SetActionTransitionTime(30);
		
        delete [] fBuf;		
    }
}

void CA3DManager::loadA3DFile(const std::string &strFileName, const std::string &sSkeleName, const char* pBuffer, UINT bufLen)
{
	if (pBuffer && bufLen > 0)
	{
        D3DXVECTOR3 vPos(0.0f , 0.0f , 0.0f);
        D3DXVECTOR3 vScale(1.0f , 1.0f , 1.0f);
		D3DXVECTOR4 vRot(0.0f , 0.0f , 0.0f , 0);
	//	std::string sSkeleAnimName = _GetPureFileNameStrWithoutExtention(sSkeleName);
		CA3DSkelAnimation *pAniation = m_A3DModelLoader.CreatekelAnimation(strFileName.c_str() , sSkeleName.c_str() , pBuffer , bufLen , vPos , vScale ,  vRot );
		pAniation->SetVisible(TRUE);
		pAniation->Enable(TRUE);
		pAniation->SetActionTransitionTime(30);
	}

}

void CA3DManager::setSkeletonPosition(const std::string &sSkeleName, const D3DXVECTOR3& v3Pos)
{
	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());
	if (pAnim)
	{
		pAnim->SetPosition(v3Pos);	
	}
}

void CA3DManager::FrustrumTestA3D(D3DXVECTOR3* pCamPos, D3DXMATRIX* pMatView, float tanHalfFieldOfViewV, float nearClip, float farClip)
{

	m_A3DModelLoader.FrustrumTestA3D( pCamPos, pMatView, tanHalfFieldOfViewV,
		nearClip, farClip );



}

CA3DSkelAnimation* CA3DManager::getMouseHitSkeleton(D3DXVECTOR3* orig, D3DXVECTOR3* dir)
{
	return m_A3DModelLoader.getMouseHitSkeleton(orig, dir );

}

void CA3DManager::setSkeletonAnimVisibility(const std::string &sSkeleName, BOOL bVisible)
{
	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());
	if (pAnim)
	{
		pAnim->SetVisible(bVisible);
	}
}

BOOL CA3DManager::getSkeletonAnimVisibility(const std::string &sSkeleName)
{
	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());
	if (pAnim)
	{
		return pAnim->IsVisible();
	}
	return FALSE;
}

void CA3DManager::setSkeletonAnimEnable(const std::string &sSkeleName, BOOL bEnable)
{
	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());
	if (pAnim)
	{
		pAnim->Enable(bEnable);
	}
}

BOOL CA3DManager::getSkeletonAnimEnable(const std::string &sSkeleName)
{
	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());
	if (pAnim)
	{
		return pAnim->IsEnable();
	}
	return FALSE;
}


CA3DSkelAnimation *CA3DManager::getSkeletonAnim(const std::string &sSkeleName)
{
	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());
	if (pAnim)
	{
		return pAnim;
	}
	return NULL;
}

void CA3DManager::MoveSkeletonToPos( const std::string &sSkeleName , float offsetx, float offsety ,float offsetz, int step)
{
	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());  
	if (pAnim)
	{
			D3DXVECTOR3 pos = 	pAnim->GetPos();
			pAnim->MovetoPosition( ( pos+ D3DXVECTOR3(offsetx,offsety, offsetz)) , step ); 


	}

}




void CA3DManager::setSkaActionInfo(const std::string &sSkeleName,const std::string &sActionName,
											int repeat , int from , int to , int TrackIdx )
{

	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());
	if (pAnim)
	{
		pAnim->Enable(true);
		if( from == -1 && to == -1  )
		{
			pAnim->SetAction( sActionName.c_str() , repeat ); 
		}
		else
		{
			pAnim->SetAction( sActionName.c_str() , from , to , repeat, TrackIdx );

		}
	}


}



void CA3DManager::setSkaActionInfo(const std::string &sSkeleName,int actionIdx, int repeat, int from , int to ,  int TrackIdx )
{

	CA3DSkelAnimation *pAnim = m_A3DModelLoader.FindAnimationByName(sSkeleName.c_str());
	if (pAnim)
	{
		pAnim->Enable(true);
		if( from == -1 && to == -1  )
		{
			pAnim->SetAction( actionIdx, repeat ); 
		}
		else
		{
			pAnim->SetAction( actionIdx, from , to , repeat, TrackIdx );

		}
	}

}

