// Node.cpp: implementation of the CSceneNode class.
//
//////////////////////////////////////////////////////////////////////

// #include "stdafx.h"
#include "Node.h"
#include "VGSSupport.h"
#include "Root.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSceneNode::CSceneNode()
{
	//节点父子关系
	m_parent = NULL;

	//在各自类型的列表中的index
	m_object = 0;
	
	//变换参数-------------------------------------------------------------------------
	// updateFromParent();
	m_WorldPosition = D3DXVECTOR3(0, 0, 0);		//节点在世界坐标系中的位置
	// worldRotation = D3DXVECTOR3(0, 0, 0);		//节点在世界坐标系中的转角
	m_WorldRotation = VGS_ZEROQUATERNION; //D3DXQUATERNION(0, 0, 0, 1);
	m_WorldScale = D3DXVECTOR3(1, 1, 1);			//节点在世界坐标系中的缩放
	D3DXMatrixIdentity(&m_WorldMatrix);

	m_Position = D3DXVECTOR3(0, 0, 0);
	m_Rotation = VGS_ZEROQUATERNION;
	m_Scale    = D3DXVECTOR3(1, 1, 1);
	D3DXMatrixIdentity(&m_Matrix);

	// m_bNeedUpdate = false;
	m_bLocalMatrixChanged = false;

}

CSceneNode::~CSceneNode()
{	
	Release();
}

VGS_MOVABLETYPE CSceneNode::getMovableType()
{
	if (m_object)
	{
		return m_object->getType(); 
	}
	return VGS_MOVABLETYPE_NONE;
}

//绕自身坐标系或世界坐标系平移//coordEnum = 0 = worldcoord, 1 = selfcoor; 2: parent
void CSceneNode::translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pVec)
	{
		return;
	}

	switch (coordEnum)
	{
		//沿世界坐标系平移
	case VTS_WORLD:
		{
			// 自身的世界变换
			//getMatrix(VTS_WORLD); 
			m_WorldMatrix._41 += pVec->x;  // 平移
			m_WorldMatrix._42 += pVec->y;
			m_WorldMatrix._43 += pVec->z;

			// 更新自身变换矩阵
			UpdateLocalMatrix();
		}
		break;
		
		//沿自身坐标系平移
	case VTS_LOCAL:
		{
			D3DXVECTOR3 v3DeltPos;
			D3DXVec3TransformNormal(&v3DeltPos, pVec, &m_Matrix);
			m_Matrix._41 += v3DeltPos.x;  // 平移
			m_Matrix._42 += v3DeltPos.y;
			m_Matrix._43 += v3DeltPos.z;

			// 更新世界坐标系矩阵
			UpdateWorldMatrix();
		}	
		break;
	case VTS_PARENT:  //沿父级坐标系变换
		{
			m_Matrix._41 += pVec->x;  // 平移
			m_Matrix._42 += pVec->y;
			m_Matrix._43 += pVec->z;

			// 更新世界坐标系矩阵
			UpdateWorldMatrix();
		}
		break;
	}		
}

//通过设置模型的坐标来设置模型的位置，世界坐标系, Pivot位置为模型的位置
void CSceneNode::setPosition(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum/* = 0*/)
{
	D3DXVECTOR3 delTranslate = VGS_ZEROVECTOR3;

	if (coordEnum == VTS_WORLD) // world position
	{
		// 计算
		delTranslate = *pVec - m_WorldPosition;
		translate(&delTranslate, VTS_WORLD);
	}
	else //if (coordEnum == 1) // 父亲坐标系
	{
		// 计算
		delTranslate = *pVec - m_Position;
		translate(&delTranslate, VTS_PARENT);
	}
	
}

// 获取当前的坐标
D3DXVECTOR3 CSceneNode::getPosition(VGS_TRANSFORMSPACE coordEnum)
{
	D3DXVECTOR3 vsPos = VGS_ZEROVECTOR3;
	switch (coordEnum)
	{
	case VTS_WORLD:
		return m_WorldPosition;
		break;
	case VTS_LOCAL:
		break;
	case VTS_PARENT:
		return m_Position;
		break;
	}
	return vsPos;
}
// 绕经过节点中心旋转一个角度。
void CSceneNode::rotate(D3DXQUATERNION* ptrQuat,VGS_TRANSFORMSPACE coordEnum)
{
	if (!ptrQuat)
	{
		return;
	}

	D3DXQUATERNION quat;
	D3DXQuaternionNormalize(&quat, ptrQuat); 

	switch (coordEnum)
	{
		//绕穿过模型中心并与世界坐标系平行的坐标系旋转
	case VTS_WORLD:
		D3DXQuaternionMultiply(&m_WorldRotation, &m_WorldRotation, &quat);
		D3DXQuaternionNormalize(&m_WorldRotation, &m_WorldRotation); 
		D3DXMatrixTransformation(&m_WorldMatrix, NULL, NULL, &m_WorldScale, NULL, &m_WorldRotation, &m_WorldPosition);
		UpdateLocalMatrix();
		break;
		//绕自身坐标系旋转,本系统定义先绕自身X轴旋转,再绕自身Y轴旋转,再绕自身Z轴旋转
	case VTS_LOCAL:
		{
			D3DXMATRIX mat;
			D3DXMatrixRotationQuaternion(&mat, &quat);
			D3DXMatrixMultiply(&m_Matrix, &mat, &m_Matrix);
			UpdateWorldMatrix();
		}
		break;
		
		//绕父级节点旋转
	case VTS_PARENT:
		{
			D3DXMATRIX mat;
			D3DXMatrixRotationQuaternion(&mat, &quat);
			D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &mat);
			UpdateWorldMatrix();
			break;
		}
	}

}
//绕自身坐标系或世界坐标系旋转
void CSceneNode::rotate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pVec)
	{
		return;
	}
    D3DXQUATERNION quat(0, 0, 0, 1);
    D3DXQuaternionRotationYawPitchRoll(&quat, pVec->y, pVec->x, pVec->z);
	D3DXQuaternionNormalize(&quat, &quat);
	rotate(&quat, coordEnum);
}

//绕穿过固定点的指定轴旋转一定的角度
void CSceneNode::rotate(D3DXVECTOR3* pPos, D3DXVECTOR3* pVec, float fAngle,VGS_TRANSFORMSPACE coordEnum)
{
	MessageBox(0, TEXT("rotate 没有实现"), TEXT(""), 0);
	return;
		
 //   //获得指定轴的worldMatrix
	//D3DXMATRIX  axisWorldMat,axisMat,axisWorldMat_Inv; 
 //   D3DXQUATERNION quat, rot_quat;
	//D3DXQuaternionRotationYawPitchRoll(&quat, 0, 0, 0);

 //   //获得轴相对于世界坐标系的rotate_Matrix
	////rot_quat = quat;
	//D3DXVECTOR3 ZeroVector = D3DXVECTOR3(0,0,0);
	//D3DXVECTOR3 NomVector = D3DXVECTOR3(1,1,1);
	//D3DXQUATERNION ZeroQuat = D3DXQUATERNION(0, 0, 0, 0);

 //   //获得变换矩阵
 //   D3DXMatrixTransformation(&axisWorldMat, &ZeroVector,&ZeroQuat,&NomVector,&ZeroVector, &quat, pPos); 

 //   //获得节点相对于轴的相对变换矩阵
	//D3DXMatrixInverse(&axisWorldMat_Inv,NULL,&axisWorldMat);
 //   D3DXMatrixMultiply(&axisMat,&curTransform.worldMatrix, &axisWorldMat_Inv);

 //   //获得轴相对于世界坐标系的trans_Matrix
 //   //D3DXMatrixTranslation(&axisTrans, pPos->x, pPos->y, pPos->z);
 //   //获得轴相对于世界坐标系的scale_Matrix
 //   //D3DXMatrixScaling(&axisScale, 1.0f,1.0f,1.0f);

 //   //获得轴的旋转四元数
	//D3DXQuaternionRotationAxis(&rot_quat, pVec, fAngle);

	////轴绕自身旋转
	//D3DXQuaternionMultiply(&quat,  &quat, &rot_quat);

 //  	//指定轴绕自身旋转fAngle，得到新的worldMatrix
 //   //D3DXMatrixRotationQuaternion(&axisRot, &rot_quat);

	//D3DXMatrixTransformation(&axisWorldMat, &ZeroVector,&ZeroQuat,&NomVector,&ZeroVector, &quat, pPos); 
	//
	////将轴的worldMatrix和相对变化矩阵相乘得到节点新的worldMatrix
 //   D3DXMatrixMultiply(&curTransform.worldMatrix ,&axisMat,&axisWorldMat); 

	////重新计算节点的相对父节点的相对变化矩阵
	//if(m_parent)
	//{
 //      D3DXMatrixInverse(&axisWorldMat_Inv,NULL,&m_parent->curTransform.worldMatrix);
 //      D3DXMatrixMultiply(&curTransform.matrix,&curTransform.worldMatrix,&axisWorldMat_Inv);
	//}else
	//{
	//  curTransform.matrix =curTransform.worldMatrix;
	//}
 //   
	//updateTransform();
	////curTransform.ori_matrix = curTransform.matrix;
}

//直接设置在世界坐标系中旋转的角度，x,y,z三轴
void CSceneNode::setRotationByAngle(const D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pVec)
	{
		return;
	}

	if (coordEnum == VTS_WORLD)
	{
		D3DXQUATERNION quat;
		D3DXQuaternionRotationYawPitchRoll(&quat, pVec->y, pVec->x, pVec->z);
		D3DXMatrixTransformation(&m_WorldMatrix, NULL, NULL, &m_WorldScale, NULL, &quat, &m_WorldPosition);
		UpdateLocalMatrix();
	}
	else if (coordEnum == VTS_PARENT)
	{
		D3DXQUATERNION quat;
		D3DXQuaternionRotationYawPitchRoll(&quat, pVec->y, pVec->x, pVec->z);
		D3DXMatrixTransformation(&m_Matrix, NULL, NULL, &m_Scale, NULL, &quat, &m_Position);
		UpdateWorldMatrix();
	}	
}

//直接设置旋转的角度，四元数
void CSceneNode::setRotationByQuat(const D3DXQUATERNION* pQuat, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pQuat)
	{
		return;
	}

	if (coordEnum == VTS_WORLD)
	{
		D3DXQuaternionNormalize(&m_WorldRotation, pQuat);
		D3DXMatrixTransformation(&m_WorldMatrix, NULL, NULL, &m_WorldScale, NULL, &m_WorldRotation, &m_WorldPosition);
		UpdateLocalMatrix();
	}
	else if (coordEnum == VTS_PARENT)
	{
		D3DXQuaternionNormalize(&m_Rotation, pQuat);
		D3DXMatrixTransformation(&m_Matrix, NULL, NULL, &m_Scale, NULL, &m_Rotation, &m_Position);
		UpdateWorldMatrix();
	}	

}

void CSceneNode::GetQuatBy3Axis(D3DXQUATERNION* rotQuat, const D3DXVECTOR3& xVec, const D3DXVECTOR3& yVec, const D3DXVECTOR3& zVec)
{
       D3DXMATRIX rotMat;
	   D3DXMatrixIdentity( &rotMat);
	   rotMat._11 = xVec.x;
	   rotMat._21 = xVec.y;
	   rotMat._31 = xVec.z;
	   rotMat._12 = yVec.x;
	   rotMat._22 = yVec.y;
	   rotMat._32 = yVec.z;
	   rotMat._13 = zVec.x;
	   rotMat._23 = zVec.y;
	   rotMat._33 = zVec.z;
	   D3DXQuaternionRotationMatrix( rotQuat , &rotMat);
}

// 设置节点的z轴指向v3Lookat
// v3Lookat : z轴指向的点；coordEnum : 参考坐标系；yawFixed : 是否锁定始yaw旋转的轴；yawFixedAxis ：如果锁定yaw轴，则指定轴
void CSceneNode::lookAt(const D3DXVECTOR3& v3Lookat, const VGS_TRANSFORMSPACE& coordEnum, const bool& bYawFixed/* = true*/, const D3DXVECTOR3& v3YawFixedAxis/* = D3DXVECTOR3(0, 1, 0)*/)
{
	D3DXVECTOR3 v3WorldLookat;
	D3DXVECTOR3 v3WorldYawAxis;
	
	switch(coordEnum)
	{
	case VTS_WORLD:
		v3WorldLookat = v3Lookat;
		if (bYawFixed)
		{
			v3WorldYawAxis = v3YawFixedAxis;
		}
		else
		{
			v3WorldYawAxis = D3DXVECTOR3_Y; // Y轴
		}
		break;
	case VTS_LOCAL:
		D3DXVec3TransformCoord(&v3WorldLookat, &v3Lookat, &m_WorldMatrixInverse);
		if (bYawFixed)
		{
			v3WorldYawAxis = v3YawFixedAxis;
		}
		else
		{
			v3WorldYawAxis = D3DXVECTOR3_Y; // Y轴
		}
		D3DXVec3TransformNormal(&v3WorldYawAxis, &v3WorldYawAxis, &m_WorldMatrixInverse);

		lookAt(v3WorldLookat, VTS_WORLD, bYawFixed, v3WorldYawAxis);
		break;
	case VTS_PARENT:
		{
			D3DXMATRIX mat;
			if (m_parent)
			{
				mat = m_parent->getMatrix(VTS_WORLD);
			}
			else
			{
				mat = m_WorldMatrix;
			}

			D3DXVec3TransformCoord(&v3WorldLookat, &v3Lookat, &mat);
			if (bYawFixed)
			{
				v3WorldYawAxis = v3YawFixedAxis;
			}
			else
			{
				v3WorldYawAxis = D3DXVECTOR3_Y; // Y轴
			}
			D3DXVec3TransformNormal(&v3WorldYawAxis, &v3WorldYawAxis, &mat);

			lookAt(v3WorldLookat, VTS_WORLD, bYawFixed, v3WorldYawAxis);
		}
		break;
	default:
		break;
	}

	// 计算z轴的转角
	D3DXVECTOR3 v3Pos = getPosition(VTS_WORLD);
	D3DXVECTOR3 v3LookatDir = v3WorldLookat - v3Pos;
	//v3LookatDir.y = 0;
	D3DXVec3Normalize(&v3LookatDir, &v3LookatDir);

	// 计算旋转的角度及up和x轴
	D3DXVECTOR3 v3ZAxis = D3DXVECTOR3(m_WorldMatrix._31, m_WorldMatrix._32, m_WorldMatrix._33);  // z轴
	//v3ZAxis.y = 0;
	D3DXVec3Normalize(&v3ZAxis, &v3ZAxis);

	D3DXMATRIX matRotation = _getRotationMatrix(v3ZAxis, v3LookatDir);  // 计算新旧z轴的转角

	// y轴不要变
	D3DXVECTOR3 v3XAxis;
	D3DXVECTOR3 v3YAxis = D3DXVECTOR3(v3WorldYawAxis.x, v3WorldYawAxis.y, v3WorldYawAxis.z);  // y轴
	D3DXVec3Normalize(&v3YAxis, &v3YAxis);
	D3DXVec3Cross(&v3XAxis, &v3YAxis, &v3LookatDir);
	D3DXVec3Cross(&v3ZAxis, &v3XAxis, &v3YAxis);

	// 修改变换矩阵的xy轴
	m_WorldMatrix._41 = 0;
	m_WorldMatrix._42 = 0;
	m_WorldMatrix._43 = 0;
	D3DXMatrixMultiply(&m_WorldMatrix, &m_WorldMatrix, &matRotation);
	m_WorldMatrix._11 = v3XAxis.x;
	m_WorldMatrix._12 = v3XAxis.y;
	m_WorldMatrix._13 = v3XAxis.z;
	m_WorldMatrix._21 = v3YAxis.x;
	m_WorldMatrix._22 = v3YAxis.y;
	m_WorldMatrix._23 = v3YAxis.z;
	m_WorldMatrix._31 = v3ZAxis.x;
	m_WorldMatrix._32 = v3ZAxis.y;
	m_WorldMatrix._33 = v3ZAxis.z;
	m_WorldMatrix._41 = v3Pos.x;
	m_WorldMatrix._42 = v3Pos.y;
	m_WorldMatrix._43 = v3Pos.z;

	UpdateWorldMatrix();

	return;

    // 不处理零向量
	//if (vec == D3DXVECTOR3(0, 0, 0)) 
	//{
	//	return;   
	//}

	//D3DXVec3Normalize(&vec, &vec);

	//D3DXMATRIX rotMat;    
	//switch(coordEnum)
	//{
	//  case VTS_WORLD:
 //         //world
 //         
	//	  break;
	//  case VTS_LOCAL:
 //         //local       		  
 //         D3DXMatrixRotationYawPitchRoll(&rotMat, m_WorldRotation.y, m_WorldRotation.x, m_WorldRotation.z);
	//	  D3DXVec3TransformNormal(&vec, &vec, &rotMat);
	//	  break;
	//  case VTS_PARENT:
 //         // parent
 //         if (m_parent)
	//	  {
	//		  D3DXMatrixRotationYawPitchRoll(&rotMat, m_parent->m_WorldRotation.y, m_parent->m_WorldRotation.x, m_parent->m_WorldRotation.z);
	//		  D3DXVec3TransformNormal( &vec, &vec, &rotMat);
	//	  }
	//	  else
	//	  {
	//		  D3DXMatrixRotationYawPitchRoll(&rotMat, m_WorldRotation.y , m_WorldRotation.x, m_WorldRotation.z);
	//		  D3DXVec3TransformNormal( &vec, &vec, &rotMat);
	//	  }
	//	  break;
	//}

 //   D3DXVECTOR3 curDir;
	//D3DXQUATERNION targetQuat,curQuat;

	//if (yawFixed)
	//{
	//	D3DXVECTOR3 xVec, yVec;
 //       D3DXVec3Cross(&xVec, &yawFixedAxis, &vec);
 //       D3DXVec3Normalize(&xVec, &xVec);
 //       D3DXVec3Cross(&yVec,&vec,&xVec);
 //       D3DXVec3Normalize(&yVec, &yVec);
 //       D3DXQUATERNION unitZToTarget;
	//	GetQuatBy3Axis(&unitZToTarget, xVec, yVec, vec);
	//	if(localDir == D3DXVECTOR3( 0, 0, -1))
	//	{
	//	  targetQuat =D3DXQUATERNION(-unitZToTarget.z, unitZToTarget.w, unitZToTarget.x, -unitZToTarget.y);
	//	}
	//	else
	//	{
	//		D3DXVECTOR3 zVector = D3DXVECTOR3 (0, 0, 1);
	//		D3DXQUATERNION localToUnitZ;
	//		GetQuatRotation(&localToUnitZ, localDir, zVector);
	//		D3DXQuaternionMultiply(&targetQuat, &localToUnitZ, &unitZToTarget);
	//	}
	//}
	//else
	//{
	//	D3DXQuaternionRotationYawPitchRoll(&curQuat, m_WorldRotation.y , m_WorldRotation.x , m_WorldRotation.z);
	//	D3DXMatrixRotationYawPitchRoll(&rotMat, m_WorldRotation.y , m_WorldRotation.x , m_WorldRotation.z);
	//	D3DXVec3TransformNormal( &curDir, &localDir, &rotMat);
	//	D3DXVECTOR3 dir;
	//	D3DXVec3Add(&dir, &curDir, &vec);
	//	if((D3DXVec3LengthSq(&dir))<0.00005f)
	//	{
	//		targetQuat = D3DXQUATERNION(-curQuat.z, curQuat.w, curQuat.x, -curQuat.y);
	//	}else
	//	{
	//		D3DXQUATERNION rotQuat;
	//		GetQuatRotation(&rotQuat,curDir,vec);
	//		D3DXQuaternionMultiply(&targetQuat, &curQuat, &rotQuat);
	//		
	//	}
	//}

	////获得变换矩阵
	//D3DXVECTOR3 ZeroVector  = D3DXVECTOR3(0, 0, 0);	
	//D3DXQUATERNION ZeroQuat = D3DXQUATERNION(0, 0, 0, 1);
	//D3DXMatrixTransformation(&m_WorldMatrix, &ZeroVector, &ZeroQuat, &m_WorldScale, &ZeroVector, &targetQuat, &m_WorldPosition);
	//
	//UpdateLocalMatrix();
}

//获取两个向量
void  CSceneNode::GetQuatRotation(D3DXQUATERNION* rotQuat,const D3DXVECTOR3& curDir,const D3DXVECTOR3& desDir,const D3DXVECTOR3& fallbackAxis)//=D3DXVECTOR3(0, 0, 0)
{
    D3DXQUATERNION q;

	D3DXVECTOR3 v0 = curDir;
	D3DXVECTOR3 v1 = desDir;
	D3DXVec3Normalize(&v0, &v0);
    D3DXVec3Normalize(&v1, &v1);

	FLOAT d=D3DXVec3Dot(&v0, &v1);
	if(d>=1.0f)
	{
	   D3DXQuaternionIdentity(rotQuat);
	   return;
	}
	if(d<(1e-6f -1.0f))
	{
	   if(fallbackAxis !=D3DXVECTOR3(0,0,0))
	   {
	       D3DXQuaternionRotationAxis(&q, &fallbackAxis,PI);
	   }else
	   {
	       D3DXVECTOR3 axis;
		   D3DXVECTOR3 unit_x =D3DXVECTOR3(1, 0, 0);
		   D3DXVECTOR3 unit_y =D3DXVECTOR3(0, 1, 0);
		   D3DXVec3Cross(&axis, &unit_x, &curDir);
		   if((D3DXVec3Length(&axis))<1e-06)
		   {
		      D3DXVec3Cross(&axis, &unit_y, &curDir);
		   }
		   D3DXVec3Normalize(&axis,&axis);
           D3DXQuaternionRotationAxis(&q, &axis, PI);
	   }
	}else
	{
	   DOUBLE s =sqrt((1+d)*2);
	   DOUBLE invs =1.0f/s;

	   D3DXVECTOR3 c;
       D3DXVec3Cross(&c, &v0, &v1);
	   
	   q.x = c.x * (float)invs;
	   q.y = c.y * (float)invs;
	   q.z = c.z * (float)invs;
	   q.w = (float)s * 0.5f;

	   D3DXQuaternionNormalize(&q,&q);
	}	
	*rotQuat= q;
}

//以自身坐标中心或世界坐标中心放大
void CSceneNode::scale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pVec)
	{
		return;
	}

	switch (coordEnum)
	{
	case VTS_WORLD:
		m_WorldScale.x *= pVec->x;
		m_WorldScale.y *= pVec->y;
		m_WorldScale.z *= pVec->z;
		D3DXMatrixTransformation(&m_WorldMatrix, NULL, NULL, &m_WorldScale, NULL, &m_WorldRotation, &m_WorldPosition);
		UpdateLocalMatrix();
		break;
	case VTS_LOCAL:
	case VTS_PARENT:
		m_Scale.x *= pVec->x;
		m_Scale.y *= pVec->y;
		m_Scale.z *= pVec->z;
		D3DXMatrixTransformation(&m_Matrix, NULL, NULL, &m_Scale, NULL, &m_Rotation, &m_Position);
		UpdateWorldMatrix();
		break;
	}
}

//以固定点为中心放大
void CSceneNode::scaleByPoint(D3DXVECTOR3* pVec, D3DXVECTOR3* pPos, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pVec || !pPos)
	{
		return;
	}

	switch (coordEnum)
	{
	case VTS_WORLD:
		//缩放模型
		m_WorldScale.x *= pVec->x;
		m_WorldScale.y *= pVec->y;
		m_WorldScale.z *= pVec->z;

		//因缩放而引起的位置变化,手动编写
		m_WorldPosition   -= *pPos;
		m_WorldPosition.x *= pVec->x;
		m_WorldPosition.y *= pVec->y;
		m_WorldPosition.z *= pVec->z;
		m_WorldPosition   += *pPos;
		D3DXMatrixTransformation(&m_WorldMatrix, NULL, NULL, &m_WorldScale, NULL, &m_WorldRotation, &m_WorldPosition);
		UpdateLocalMatrix();
		break;
	case VTS_LOCAL:
		break;
	case VTS_PARENT:
		//缩放模型
		m_Scale.x *= pVec->x;
		m_Scale.y *= pVec->y;
		m_Scale.z *= pVec->z;

		//因缩放而引起的位置变化,手动编写
		m_Position   -= *pPos;
		m_Position.x *= pVec->x;
		m_Position.y *= pVec->y;
		m_Position.z *= pVec->z;
		m_Position   += *pPos;
		D3DXMatrixTransformation(&m_Matrix, NULL, NULL, &m_Scale, NULL, &m_Rotation, &m_Position);
		UpdateWorldMatrix();
		break;
	}
}

//直接设置缩放，0 : 世界坐标系， 1：自身坐标系,2:父坐标系
void CSceneNode::setScale(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pVec)
	{
		return;
	}

	D3DXVECTOR3 v3ScaleDelt = VGS_ONEVECTOR3;
	D3DXVECTOR3 v3Scale = getScale(coordEnum);

   if (v3Scale.x != 0)
   {
	   v3ScaleDelt.x = pVec->x / v3Scale.x;
   }

   if (v3Scale.y != 0)
   {
	   v3ScaleDelt.y = pVec->y / v3Scale.y;
   }

   if (v3Scale.z != 0)
   {
	   v3ScaleDelt.z = pVec->z / v3Scale.z;
   }
   
   scale(&v3ScaleDelt, coordEnum);
}

// 获得当前的旋转四元数
D3DXQUATERNION CSceneNode::getRotition(VGS_TRANSFORMSPACE coordEnum)
{
	D3DXQUATERNION qutRot = VGS_ZEROQUATERNION;
	switch (coordEnum)
	{
	case VTS_WORLD:
		return m_WorldRotation;
		break;
	case VTS_LOCAL:
		break;
	case VTS_PARENT:
		return m_Rotation;
		break;
	default:
		break;
	}

	return qutRot;
}

// 获得当前的缩放
D3DXVECTOR3 CSceneNode::getScale(VGS_TRANSFORMSPACE coordEnum)
{
	D3DXVECTOR3 v3Scale = VGS_ONEVECTOR3;
	switch (coordEnum)
	{
	case VTS_WORLD:
		return m_WorldScale;
		break;
	case VTS_LOCAL:
		break;
	case VTS_PARENT:
		return m_Scale;
		break;
	default:
		break;
	}

	return v3Scale;

}

// 获得变换矩阵
D3DXMATRIX CSceneNode::getMatrix(VGS_TRANSFORMSPACE coordEnum)
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	switch (coordEnum)
	{
	case VTS_WORLD:
		return m_WorldMatrix;
		break;
	case VTS_LOCAL:
		break;
	case VTS_PARENT:
		return m_Matrix;
		break;
	default:
		break;
	}

	return mat;
}
// 设置变换矩阵
void CSceneNode::setMatrix(const D3DXMATRIX& mat, VGS_TRANSFORMSPACE coord)
{
	switch (coord)
	{
	case VTS_WORLD:
		m_WorldMatrix = mat;
		UpdateLocalMatrix();
		break;
	case VTS_LOCAL:
		break;
	case VTS_PARENT:
		m_Matrix = mat;
		UpdateWorldMatrix();
		break;
	default:
		break;
	}
}

//
////update translate matrix
//void CSceneNode::updateTranslateMatrix()
//{
//	//cal translate matrix
//	D3DXMatrixIdentity(&curTransform.translate_matrix);
//	D3DXMatrixTranslation(&curTransform.translate_matrix, curTransform.position.x, curTransform.position.y, curTransform.position.z);
//	//worldPosition = curTransform.position;
//}
//
////update rotate matrix
//void	CSceneNode::updateRotateMatrix()
//{
//	//cal rotate matrix
//	D3DXMatrixIdentity(&curTransform.rotation_matrix);
//	D3DXQUATERNION quat;
//	D3DXQuaternionRotationYawPitchRoll(&quat, curTransform.rotation.y,curTransform.rotation.x, curTransform.rotation.z);
//    D3DXMatrixRotationQuaternion(&curTransform.rotation_matrix, &quat); 
//}
//
////update scale matrix
//void	CSceneNode::updateScaleMatrix()
//{
//	//cal scale matrix
//	D3DXMatrixIdentity(&curTransform.scale_matrix);
//    D3DXMatrixScaling(&curTransform.scale_matrix, curTransform.scale.x,curTransform.scale.y ,curTransform.scale.z);
//}
//
////update matrix
//void	CSceneNode::multiplyMatrix()
//{
//	//cal all matrix
//	//改变世界变换矩阵
//	D3DXMatrixMultiply(&(curTransform.matrix), 
//					   &(curTransform.scale_matrix), 
//					   &(curTransform.rotation_matrix));
//	D3DXMatrixMultiply(&(curTransform.matrix), 
//					   &(curTransform.matrix), 
//					   &(curTransform.translate_matrix ));
//	//获得自身相对父节点的变换矩阵
//    /*if(m_parent)
//	{
//	   	D3DXMATRIX trans_Mat;
//		D3DXMatrixInverse(&trans_Mat, NULL, m_parent->curTransform.worldMatrix);
//		D3DXMatrixMuliply(&(curTransform.matrix),
//			              &(curTransform.worldMatrix),
//						  &(trans_Mat));
//	}else
//	{
//	   curTransform.matrix= curTransform.worldMatrix;
//	}*/
//
//}


//本类不实现或实现起来很麻烦，由继承类具体需要做的工作, 譬如更新模型的包围盒
//void CSceneNode::update()
//{
//	
//}

// 根据自身坐标系下的变换矩阵，更新世界坐标系下的变换矩阵
// void CSceneNode::updateFromParent()
void CSceneNode::UpdateWorldMatrix()
{	
	// 更新世界坐标系矩阵
	if (m_parent)
	{
		D3DXMATRIX matParent = m_parent->getMatrix(VTS_WORLD);
		D3DXMatrixMultiply(&m_WorldMatrix, &m_Matrix, &matParent);
	}
	else
	{
		m_WorldMatrix = m_Matrix;
	}

	// 分解矩阵
	D3DXMatrixDecompose(&m_WorldScale, &m_WorldRotation, &m_WorldPosition, &m_WorldMatrix);
	D3DXMatrixDecompose(&m_Scale, &m_Rotation, &m_Position, &m_Matrix);

	D3DXMatrixInverse(&m_WorldMatrixInverse, 0, &m_WorldMatrix);
#if _DEBUG
	/*D3DXVECTOR3 v3Pos, v3Scale, v3Rotation;
	_decomposeTransfromMatrix(m_WorldMatrix, v3Pos, v3Rotation, v3Scale);
	D3DXVECTOR3 v3Pos2, v3Scale2, v3Rotation2;
	_decomposeTransfromMatrix(m_Matrix, v3Pos2, v3Rotation2, v3Scale2);*/
#endif

	// object对象更新
	if (m_object)
	{
		m_object->UpdateFromNode();
	}

	// 更新子节点的变换矩阵
	UpdateChildTransform();
}

// 根据世界坐标系下的变换矩阵，更新自身坐标系下的变换矩阵
void CSceneNode::UpdateLocalMatrix()
{	
	// 更新世界坐标系矩阵
	if (m_parent)
	{
		D3DXMATRIX matParent_Inv;
		D3DXMATRIX matParent = m_parent->getMatrix(VTS_WORLD);
		D3DXMatrixInverse(&matParent_Inv, NULL, &matParent);
		D3DXMatrixMultiply(&m_Matrix, &m_WorldMatrix, &matParent_Inv);
	}
	else
	{
		m_Matrix = m_WorldMatrix;
	}

	// 分解矩阵
	D3DXMatrixDecompose(&m_WorldScale, &m_WorldRotation, &m_WorldPosition, &m_WorldMatrix);
	D3DXMatrixDecompose(&m_Scale, &m_Rotation, &m_Position, &m_Matrix);

	D3DXMatrixInverse(&m_WorldMatrixInverse, 0, &m_WorldMatrix);

	// object对象更新
	if (m_object)
	{
		m_object->UpdateFromNode();
	}

	// 更新子节点的变换矩阵
	UpdateChildTransform();
}

// 更新子节点的变换矩阵
void CSceneNode::UpdateChildTransform()
{
   int childCnt = m_child.size();
   std::vector<CSceneNode*>::iterator ppNode = m_child.begin();
   for (int i = 0; i< childCnt; i++)
   {
	   CSceneNode* pNode = *(ppNode + i);
	   if (pNode)
	   {
		   pNode->UpdateWorldMatrix();
	   }
   }
}

//释放
void CSceneNode::Release()
{
	if (0 != m_pRoot)
	{
		m_pRoot->GetAnimManager().DestroyNodeKeyFrmAnimation(this);  // 删除关键帧动画
	}

	m_parent = NULL;
	m_object = NULL;
	// m_Type   = VGS_MOVABLETYPE_NONE;
	for (int i = 0 ; i <(int) m_child.size(); ++i)
	{
		// m_child[i]->Release();
		SAFE_RELEASE(m_child[i]);
		SAFE_DELETE(m_child[i]);
	}
	m_child.clear();
	//SAFE_DELETE_ARRAY(kfrtransform);
}

// 节点相关
CSceneNode* CSceneNode::CreateChild()
{
	CSceneNode *pNode = new CSceneNode();
	if (pNode)
	{
		pNode->m_parent = this;
		m_child.push_back(pNode);
		pNode->UpdateWorldMatrix();
	}
	return pNode;
}
CSceneNode* CSceneNode::GetChildByIndex(INT index)
{
	if (index >= 0 && index < (int)m_child.size())
	{
		return m_child[index];
	}
	return 0;
}
UINT CSceneNode::GetChildCount()
{
	return m_child.size();
}

// 节点下关联一个物体
VOID CSceneNode::attachObject(/*VGS_MOVABLETYPE type, */CMovableObject* objectHandel)
{
	// m_Type   = type;
	m_object = objectHandel;
	
	if (objectHandel)
	{
		objectHandel->m_pNode = this;
		objectHandel->UpdateFromNode();
	}
}

// 判断一个节点是否有缩放，包括父节点的缩放
BOOL CSceneNode::getDerivedScaled()
{
	D3DXVECTOR3 v3Scale = getScale(VTS_WORLD);
	if (D3DXVec3Length(&v3Scale) > 0)
	{
		return TRUE;
	}
	/*if (D3DXVec3Length(&curTransform.scale) > 0)
	{
		return TRUE;
	}
	while (m_parent->getDerivedScaled())
	{
		return TRUE;
	}*/
	return FALSE;
}

// 通知子节点和挂的对象，节点的变化矩阵发生了变化
//void CSceneNode::notifyTransformChanged()  
//{
//	if (m_object)
//	{
//		m_object->UpdateFromNode();
//	}
//
//	UINT iChildCount = m_child.size();
//	for (UINT i = 0; i < iChildCount; ++i)
//	{
//		CSceneNode *pNode = m_child[i];
//		if (pNode)
//		{
//			pNode->notifyTransformChanged();
//		}
//	}
//}
