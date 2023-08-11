#pragma once

#include "A3DSkelAniMgr.h"

class CA3DManager
{
public:
	CA3DModelLoader* getA3DModelLoader()
	{
		return &m_A3DModelLoader;
	}
	
	void setDevice(IDirect3DDevice9* pDevice, HINSTANCE hInst  )
	{
		m_A3DModelLoader.SetDevice(pDevice,  hInst );
	}
	
	void setSkeletonResourcePath(const std::string &sPath);

	void loadA3DFile(const std::string &sFileName,   const std::string &sMeshName  );


	void loadA3DFile(const std::string &strFileName, const std::string &sSkeleName, const char* pBuffer, UINT bufLen);

	void render(long timeInterval);  

	void setSkeletonPosition(const std::string &sSkeleName, const D3DXVECTOR3& v3Pos);

	void setSkeletonAnimVisibility(const std::string &sSkeleName, BOOL bVisible);
	BOOL getSkeletonAnimVisibility(const std::string &sSkeleName);
	void setSkeletonAnimEnable(const std::string &sSkeleName, BOOL bEnable);
	BOOL getSkeletonAnimEnable(const std::string &sSkeleName);
	CA3DSkelAnimation * getSkeletonAnim(const std::string &sSkeleName);

	

	//根据鼠标位置，得到ray. 检测是否选中了角色对象
	CA3DSkelAnimation* getMouseHitSkeleton(D3DXVECTOR3* orig, D3DXVECTOR3* dir);

	//检测模型是否在视锥体内
	void FrustrumTestA3D(D3DXVECTOR3* pCamPos, D3DXMATRIX* pMatView, 
		float tanHalfFieldOfViewV, float nearClip, float farClip);



	//播放动作的入口
	void setSkaActionInfo(const std::string &sSkeleName,const std::string &sActionName,
											int from , int to , int repeat,int TrackIdx =0);


	void setSkaActionInfo(const std::string &sSkeleName,int actionIdx, 
											int from , int to , int repeat,int TrackIdx =0);


	//for demo
	void MoveSkeletonToPos( const std::string &sSkeleName, float offsetx, float offsety ,float offsetz, int step);
	
	
private:
	CA3DModelLoader m_A3DModelLoader;
};