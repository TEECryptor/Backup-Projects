
#pragma once

#include "VGSHeader.h"
#include "VGSDataStruct.h"
#include "Node.h"
#include "CScene.h"
#include "CImage.h"
#include "CCamera.h"

// 计算两个向量之间的转角,如果两个轴夹角180度，那么要经过fallbackAxis轴计算转角
extern D3DXMATRIX _getRotationMatrix(const D3DXVECTOR3& from, const D3DXVECTOR3& dest, 
									const D3DXVECTOR3& fallbackAxis = D3DXVECTOR3_ZERO);

// 将矩阵分解为位移/旋转和缩放
extern bool _decomposeTransfromMatrix(const D3DXMATRIX& transMat, D3DXVECTOR3& trans, D3DXVECTOR3& rot,D3DXVECTOR3& scale);

// 从图像文件创建一个texturelayer
// extern CTextureLayer* _CreateTextureLayerFromFile(CScene *pScene, const std::string &sLayerName, const std::string &sTextureFileName, BOOL bAlphaOnly = FALSE, INT UVIndex = 0, INT mipMap = DEFAULT_MIPMAP);

// 是否是透明格式
extern BOOL _IsTransparentFormat(D3DFORMAT format);

// 是否是纯alpha信息的格式
extern BOOL _IsPureAlphaFormat(D3DFORMAT format);

// 从文件中加载图像数据
extern VOID _loadImageFromFile(const std::string &sFileName, CImage *pImage);

/////////////////////  相机相关 //////////////////////////////////////

// 判断相机是否是预定义的轴侧相机
bool IsPredefinedOrthCamera(CCamera* pCamera);

// 判断相机是否是预定义的相机，包括轴侧和透视相机
bool IsPredefinedCamera(CCamera* pCamera);