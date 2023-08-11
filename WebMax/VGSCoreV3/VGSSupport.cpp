
#include "VGSSupport.h"
#include "CCamera.h"
#include "CLight.h"
#include "CModel.h"
#include "TextureManager.h"
#include "VGSDataStruct.h"

// 计算两个向量之间的转角,如果两个轴夹角180度，那么要经过fallbackAxis轴计算转角
D3DXMATRIX _getRotationMatrix(const D3DXVECTOR3& from, const D3DXVECTOR3& dest, 
									const D3DXVECTOR3& fallbackAxis/* = D3DXVECTOR3_ZERO*/)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	// Based on Stan Melax's article in Game Programming Gems
	// Copy, since cannot modify local
	D3DXVECTOR3 v0 = from;
	D3DXVECTOR3 v1 = dest;
	D3DXVec3Normalize(&v0, &v0);
	D3DXVec3Normalize(&v1, &v1);

	float d = D3DXVec3Dot(&v0, &v1);
	// If dot == 1, vectors are the same
	if (d >= 1.0f)
	{
		return mat;
	}
	if (d < (1e-6f - 1.0f))  // 两个轴180度
	{
		if (fallbackAxis != D3DXVECTOR3_ZERO)
		{
			// rotate 180 degrees about the fallback axis
			// q.FromAngleAxis(Radian(Math::PI), fallbackAxis);
			D3DXMatrixRotationAxis(&mat, &fallbackAxis, PI);
		}
		else
		{
			// Generate an axis
			D3DXVECTOR3 axis;
			D3DXVec3Cross(&axis, &D3DXVECTOR3_X, &v0); // Vector3::UNIT_X.crossProduct(*this);
			if (D3DXVec3LengthSq(&axis) < 0.0001) // pick another if colinear
			{
				// axis = Vector3::UNIT_Y.crossProduct(*this);
				D3DXVec3Cross(&axis, &D3DXVECTOR3_Y, &v0);
			}
			D3DXVec3Normalize(&axis, &axis);
			D3DXMatrixRotationAxis(&mat, &axis, PI);
			//axis.normalise();
			//q.FromAngleAxis(Radian(Math::PI), axis);
		}
	}
	else
	{
		float s = sqrt((1 + d) * 2);
		float invs = 1 / s;

		D3DXVECTOR3 c;
		D3DXVec3Cross(&c, &v0, &v1);

		D3DXQUATERNION q;
		q.x = c.x * invs;
		q.y = c.y * invs;
		q.z = c.z * invs;
		q.w = s * 0.5;
		D3DXQuaternionNormalize(&q, &q);
		D3DXMatrixRotationQuaternion(&mat, &q);
	}
	return mat;
}

// 将矩阵分解为位移/旋转和缩放
bool _decomposeTransfromMatrix(const D3DXMATRIX& transMat, D3DXVECTOR3& trans, D3DXVECTOR3& rot,D3DXVECTOR3& scale)
{
	D3DXMATRIX trans_Mat =transMat;
	D3DXVECTOR3 row[3],pdum3; 

	/* Normalize the matrix. */
	if (trans_Mat._44 ==0)
		return 0;
	int i;
	trans_Mat = trans_Mat/trans_Mat._44;
	if ( trans_Mat._14 != 0 || trans_Mat._24 != 0 ||
		trans_Mat._34 != 0 ) 
	{  
		trans_Mat._14 =trans_Mat._24 =trans_Mat._34 = 0;
		trans_Mat._44 = 1;
	}

	//get the translation------------------------------------------------
	for ( i=0; i<3; i++ ) 
	{
		trans[i] =	trans_Mat[3*4+i];
		trans_Mat[3*4+i] = 0;
	}

	for ( i=0; i<3; i++ ) 
	{
		row[i].x = trans_Mat[i*4];
		row[i].y = trans_Mat[i*4+1];
		row[i].z = trans_Mat[i*4+2];
	}
	scale.x =D3DXVec3Length(&row[0]);
	row[0]=row[0]/scale.x;
	scale.y =D3DXVec3Length(&row[1]);
	row[1] = row[1]/scale.y;

	//float SHEARXZ =D3DXVec3Dot(&row[0],&row[2]);
	// (void)V3Combine(&row[2], &row[0], &row[2], 1.0f, -SHEARXZ);

	//float SHEARYZ =D3DXVec3Dot(&row[1], &row[2]);
	//(void)V3Combine(&row[2], &row[1], &row[2], 1.0f, -SHEARYZ);

	scale.z =D3DXVec3Length(&row[2]);
	row[2]=row[2]/scale.z;

	/* At this point, the matrix (in rows[]) is orthonormal.
	* Check for a coordinate system flip.  If the determinant
	* is -1, then negate the matrix and the scaling factors.
	*/
	if ( D3DXVec3Dot( &row[0], D3DXVec3Cross(&pdum3,&row[1], &row[2]) ) < 0 )
		for ( i = 0; i < 3; i++ ) 
		{
			scale[i] *= -1;
			row[i].x *= -1;
			row[i].y *= -1;
			row[i].z *= -1;
		}

		/* Now, get the rotations out, as described in the gem. */
		rot.y = (float)asin(-row[0].z);
		double cosa = cos(rot.y);
		if ( fabs(cosa) > 0.001f )
		{
			rot.x = (float)atan2(row[1].z, row[2].z);
			rot.z = (float)atan2(row[0].y, row[0].x);
			if(fabs((float)rot.z + PI) < 0.001f)
				rot.z = PI;
		} else 
		{
			rot.x = (float)atan2(-row[2].x, row[1].y);
			rot.z = 0.0f;
		}

		//-------------------------------------------------------------------


		return 1;
}


// 是否是透明格式
BOOL _IsTransparentFormat(D3DFORMAT format)
{
	if(format == D3DFMT_A8R8G8B8 || format == D3DFMT_A8 || format == D3DFMT_A1R5G5B5 
		|| format == D3DFMT_A4R4G4B4 || format == D3DFMT_A8R3G3B2 
		|| format == D3DFMT_A2B10G10R10)
	{
		return TRUE;
	}
	return FALSE;
}

// 是否是纯alpha信息的格式
BOOL _IsPureAlphaFormat(D3DFORMAT format)
{
	return format == D3DFMT_A8;
}

// 从文件中加载图像数据
VOID _loadImageFromFile(const std::string &sFileName, CImage *pImage)
{
	if (!pImage)
	{
		return;
	}

	FILE* pTexFile = fopen(sFileName.c_str(), "rb");
	if (pTexFile)
	{
		fseek(pTexFile, 0, SEEK_END);
		pImage->dataSize = ftell(pTexFile);
		fseek(pTexFile, 0, SEEK_SET);
		pImage->data = new BYTE[pImage->dataSize];
		fread(pImage->data, pImage->dataSize, 1, pTexFile);
		fclose(pTexFile);
	}
}

// 判断相机是否是预定义的轴侧相机
bool IsPredefinedOrthCamera(CCamera* pCamera)
{
	if (pCamera)
	{
		std::string sName = pCamera->getName();

		if (sName.compare(CAMERA_NAME_FRONT) == 0 || sName.compare(CAMERA_NAME_BACK) == 0 ||
			sName.compare(CAMERA_NAME_LEFT) == 0 || sName.compare(CAMERA_NAME_RIGHT) == 0 ||
			sName.compare(CAMERA_NAME_TOP) == 0 || sName.compare(CAMERA_NAME_BOTTOM) == 0)
		{
			return true;
		}
	}
	return false;
}

// 判断相机是否是预定义的相机，包括轴侧和透视相机
bool IsPredefinedCamera(CCamera* pCamera)
{
	if (pCamera)
	{
		std::string sName = pCamera->getName();

		if (sName.compare(CAMERA_NAME_FRONT) == 0 || sName.compare(CAMERA_NAME_BACK) == 0 ||
			sName.compare(CAMERA_NAME_LEFT) == 0 || sName.compare(CAMERA_NAME_RIGHT) == 0 ||
			sName.compare(CAMERA_NAME_TOP) == 0 || sName.compare(CAMERA_NAME_BOTTOM) == 0 ||
			sName.compare(CAMERA_NAME_PERSPECTIVE) == 0)
		{
			return true;
		}
	}
	return false;
}