/////////////////////////////////////////////////////////////////////////
// 名称: CTransform 类型定义
// 描述: CVertex 类型定义了空间变换
// 作者: Lixin	
// 日期: 2006-04-30
////////////////////////////////////////////////////////////////////////

#include "CTransform.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransform::CTransform()
{
	D3DXMatrixIdentity(&matrix);
	D3DXMatrixIdentity(&ori_matrix);
	D3DXMatrixIdentity(&worldMatrix);
	D3DXMatrixIdentity(&translate_matrix);
	D3DXMatrixIdentity(&rotation_matrix);
	D3DXMatrixIdentity(&scale_matrix);

//	D3DXMatrixIdentity(&ctrl_matrix);

	position = D3DXVECTOR3(0,0,0);
	rotation = D3DXVECTOR3(0,0,0);
	scale = D3DXVECTOR3(1,1,1);
}

CTransform::~CTransform()
{

}
bool CTransform::decomposeTransfromMatrix(const D3DXMATRIX& transMat, D3DXVECTOR3& trans, D3DXVECTOR3& rot,D3DXVECTOR3& scale)
{
   D3DXMATRIX trans_Mat = transMat;
   D3DXVECTOR3 row[3],pdum3; 
   /* Normalize the matrix. */
   if(trans_Mat._44 ==0)
	   return 0;
   int i;
   trans_Mat =trans_Mat/trans_Mat._44;
   if ( trans_Mat._14 != 0 || trans_Mat._24 != 0 || trans_Mat._34 != 0 ) 
   {  
	   trans_Mat._14 = trans_Mat._24 = trans_Mat._34 = 0;
 	   trans_Mat._44 = 1;
   }

   //get the translation------------------------------------------------
   for (i = 0; i < 3; i++) 
   {
 	   trans[i] = trans_Mat[3 * 4 + i];
	   trans_Mat[3 * 4 + i] = 0;
 	}
   
   for (i = 0; i < 3; i++) 
   {
 		row[i].x = trans_Mat[i * 4];
 		row[i].y = trans_Mat[i * 4 + 1];
 		row[i].z = trans_Mat[i * 4 + 2];
 	}

   scale.x = D3DXVec3Length(&row[0]);
   row[0]  = row[0] / scale.x;
   scale.y = D3DXVec3Length(&row[1]);
   row[1]  = row[1] / scale.y;

   //float SHEARXZ =D3DXVec3Dot(&row[0],&row[2]);
  // (void)V3Combine(&row[2], &row[0], &row[2], 1.0f, -SHEARXZ);

   //float SHEARYZ =D3DXVec3Dot(&row[1], &row[2]);
   //(void)V3Combine(&row[2], &row[1], &row[2], 1.0f, -SHEARYZ);

   scale.z = D3DXVec3Length(&row[2]);
   row[2]  = row[2] / scale.z;

    	/* At this point, the matrix (in rows[]) is orthonormal.
 	 * Check for a coordinate system flip.  If the determinant
 	 * is -1, then negate the matrix and the scaling factors.
 	 */
 	if ( D3DXVec3Dot(&row[0], D3DXVec3Cross(&pdum3,&row[1], &row[2]) ) < 0 )
	{
		for (i = 0; i < 3; i++) 
		{
 			scale[i] *= -1;
 			row[i].x *= -1;
 			row[i].y *= -1;
 			row[i].z *= -1;
 		}
	}

 	/* Now, get the rotations out, as described in the gem. */
 	rot.y = (float)asin(-row[0].z);
	double cosa = cos(rot.y);
 	if ( fabs(cosa) > 0.001f ) 
	{
 		rot.x = (float)atan2(row[1].z, row[2].z);
 		rot.z = (float)atan2(row[0].y, row[0].x);
		if (fabs((float)rot.z + PI) < 0.001f)
		{
			rot.z =PI;
		}
 	} 
	else 
	{
 		rot.x = (float)atan2(-row[2].x, row[1].y);
 		rot.z = 0.0f;
 	}

   //-------------------------------------------------------------------
   

   return 1;
}

D3DXVECTOR3* CTransform::V3Combine (D3DXVECTOR3* a,D3DXVECTOR3* b,D3DXVECTOR3* result,float ascl,float bscl) 
{
	result->x = (ascl * a->x) + (bscl * b->x);
	result->y = (ascl * a->y) + (bscl * b->y);
	result->z = (ascl * a->z) + (bscl * b->z);
	return(result);
}
