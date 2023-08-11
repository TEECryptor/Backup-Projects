
#pragma once

#include "VGSHeader.h"
#include "VGSDataStruct.h"
#include "Node.h"
#include "CScene.h"
#include "CImage.h"
#include "CCamera.h"

// ������������֮���ת��,���������н�180�ȣ���ôҪ����fallbackAxis�����ת��
extern D3DXMATRIX _getRotationMatrix(const D3DXVECTOR3& from, const D3DXVECTOR3& dest, 
									const D3DXVECTOR3& fallbackAxis = D3DXVECTOR3_ZERO);

// ������ֽ�Ϊλ��/��ת������
extern bool _decomposeTransfromMatrix(const D3DXMATRIX& transMat, D3DXVECTOR3& trans, D3DXVECTOR3& rot,D3DXVECTOR3& scale);

// ��ͼ���ļ�����һ��texturelayer
// extern CTextureLayer* _CreateTextureLayerFromFile(CScene *pScene, const std::string &sLayerName, const std::string &sTextureFileName, BOOL bAlphaOnly = FALSE, INT UVIndex = 0, INT mipMap = DEFAULT_MIPMAP);

// �Ƿ���͸����ʽ
extern BOOL _IsTransparentFormat(D3DFORMAT format);

// �Ƿ��Ǵ�alpha��Ϣ�ĸ�ʽ
extern BOOL _IsPureAlphaFormat(D3DFORMAT format);

// ���ļ��м���ͼ������
extern VOID _loadImageFromFile(const std::string &sFileName, CImage *pImage);

/////////////////////  ������ //////////////////////////////////////

// �ж�����Ƿ���Ԥ�����������
bool IsPredefinedOrthCamera(CCamera* pCamera);

// �ж�����Ƿ���Ԥ������������������͸�����
bool IsPredefinedCamera(CCamera* pCamera);