/////////////////////////////////////////////////////////////////////////
// 名  称: CCamera
// 描  述: 摄像机类型定义 
// 作  者: lixin
// 日  期: 2005-12-21
/////////////////////////////////////////////////////////////////////////

#include "CCamera.h"
#include "Viewport.h"
#include "VGSSupport.h"

//##ModelId=47EBC74E03D1
CCamera::CCamera()
{
	InitVars();	
}

//##ModelId=47EBC74E03D8
CCamera::~CCamera()
{
	Release();
}

//-----------------------------------------------------------------------------
// 方法名: Reset()
// 参  数: 
// 用  途: 
//-----------------------------------------------------------------------------
VOID CCamera::Reset()
{
	/*m_from = D3DXVECTOR3(0.0f, 0.0f, -200.0f);
	m_at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_fov = 45.0f;
	m_roll = 0.0f;
	nearClip = 5.0f;
	farClip = 10000.0f;

	kfrInfo = NULL;
	lookAtModel = NULL;

	kfrCnt = 0;	
	kfrCurFrame = 0;
	currentTimer = 0;
	playKfr = TRUE;	
	kfrFPS = 25;	
	kfrFrameTime = 40;
	kfrRepeatCnt = -1;
	kfrRepeatIndex = 0;
	isPlaying = FALSE;

	InitMatrix();*/
}

//  重置相机为original状态
void CCamera::restore()
{
	UpdateFromNode();

	m_realPosition = m_OriginWorldPosition;
	m_realLookat   = m_OriginLookatPoint;

	SetPosition(&m_realPosition, VTS_WORLD);
	Lookat(&m_realLookat, VTS_WORLD);
	SetRoll(m_OriginRoll);
}

//-----------------------------------------------------------------------------
// 方法名: Translate()
// 参  数: D3DXVECTOR3* pVec, UINT coordEnum
// 用  途: 平移摄像机
//-----------------------------------------------------------------------------
//##ModelId=47EBC74E0370
//VOID CCamera::Translate(D3DXVECTOR3* pVec, VGS_TRANSFORMSPACE coordEnum)
//{
//
//}

// 平移相机，from和at同时移动
void CCamera::Move(const D3DXVECTOR3 &deltPos, VGS_TRANSFORMSPACE coordEnum)
{
	UpdateFromNode();

	switch (coordEnum)
	{
	case VTS_WORLD:
		{
			D3DXVECTOR3 vNewPos = m_realPosition + deltPos;
			SetPosition(&vNewPos, VTS_WORLD);
		}
		break;
	case VTS_LOCAL:
		{
			D3DXVECTOR3 vNewPos;
			D3DXVec3TransformCoord(&vNewPos, &deltPos, &m_worldTransform);
			SetPosition(&vNewPos, VTS_WORLD);
		}
		break;
	case VTS_PARENT:
		{
			D3DXVECTOR3 vNewPos = m_localPosition + deltPos;
			SetPosition(&vNewPos, VTS_PARENT);
			break;
		}
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 方法名: Dolly()
// 参  数: FLOAT fDis
// 用  途: 沿direction方向推拉相机的From点，At点不动
//-----------------------------------------------------------------------------
VOID CCamera::Dolly(FLOAT fDis)
{
	// m_realPosition += m_localZ * fDis;
	D3DXVECTOR3 vLocalDir = m_localLookat - m_localPosition;  // 在父节点下的相机方向
	D3DXVec3Normalize(&vLocalDir, &vLocalDir);
	m_localPosition += vLocalDir * fDis;

	// 修改在父坐标系下的变换矩阵
	m_localTransform._41 = m_localPosition.x;
	m_localTransform._42 = m_localPosition.y;
	m_localTransform._43 = m_localPosition.z;

	// 更新世界坐标系下的变化矩阵
	UpdateWorldMatrix();
}

//-----------------------------------------------------------------------------
// 方法名: Rotate()
// 参  数: D3DXVECTOR3* pVec, UINT coordEnum
// 用  途: 在指定的坐标系内，分别绕三个轴旋转摄像机
//         本系统旋转规则，先绕z旋转，再绕x旋转，再绕y旋转
//-----------------------------------------------------------------------------
VOID CCamera::Rotate(D3DXVECTOR3* pVec, VGS_ROTATIONSPACE coordEnum)
{	
	if (!pVec)
	{
		return;
	}

	switch (coordEnum)
	{	
		case VRS_WORLD:  // 以世界坐标系原点为中心，绕世界坐标系旋转摄像机
		{	
			D3DXMATRIX matRosX, matRosY, matRosZ, matRot;

			// 将from点和at点绕世界坐标系旋转，
			// 计算旋转矩阵
			D3DXMatrixRotationX(&matRosX, pVec->z);
			D3DXMatrixRotationY(&matRosY, pVec->x);
			D3DXMatrixRotationZ(&matRosZ, pVec->y);
			D3DXMatrixMultiply(&matRot, &matRosZ, &matRosX);
			D3DXMatrixMultiply(&matRot, &matRot, &matRosY);

			// 计算新的
			D3DXVec3TransformCoord(&m_realPosition, &m_realPosition, &matRot);
			D3DXVec3TransformCoord(&m_realLookat, &m_realLookat, &matRot);
			D3DXVec3TransformNormal(&m_realY, &m_realY, &matRot);
			D3DXVec3Cross(&m_realX, &m_realY, &(m_realLookat - m_realPosition));  
			D3DXMatrixMultiply(&m_worldTransform, &m_worldTransform, &matRot);  // 更新世界变换矩阵

			// 更新父坐标系下的矩阵
			UpdateLocalMatrix();
			break;
		}			
		case VRS_WORLD2:  // 绕穿过相机位置点，与世界坐标系平行的坐标系旋转
		{	
			D3DXMATRIX matRosX, matRosY, matRosZ, matRot, matTrans, mat;

			//将from点和at点绕世界坐标系旋转，本系统旋转规则，先绕x旋转，再绕y旋转，再绕z旋转
			// 计算旋转矩阵
			D3DXMatrixRotationX(&matRosX, pVec->z);
			D3DXMatrixRotationY(&matRosY, pVec->x);
			D3DXMatrixRotationZ(&matRosZ, pVec->y);
			D3DXMatrixMultiply(&matRot, &matRosZ, &matRosX);
			D3DXMatrixMultiply(&matRot, &matRot, &matRosY);

			// 先将相机的from点移动到原点在旋转
			D3DXMatrixTranslation(&matTrans, -m_realPosition.x, -m_realPosition.y, -m_realPosition.z); // 移动到原点
			D3DXMatrixMultiply(&mat, &matTrans, &matRot);
			D3DXMatrixTranslation(&matTrans, m_realPosition.x, m_realPosition.y, m_realPosition.z);    // 移动到原来的位置

			// 计算得到对from点的变换矩阵
			D3DXMatrixMultiply(&mat, &mat, &matTrans);

			// 计算新
			D3DXVec3TransformCoord(&m_realPosition, &m_realPosition, &mat);
			D3DXVec3TransformCoord(&m_realLookat, &m_realLookat, &mat);
			D3DXVec3TransformNormal(&m_realY, &m_realY, &mat);
			D3DXVec3Cross(&m_realX, &m_realY, &(m_realLookat - m_realPosition)); 
			D3DXMatrixMultiply(&m_worldTransform, &m_worldTransform, &mat);  // 更新世界变换矩阵

			// 更新父坐标系下的矩阵
			UpdateLocalMatrix();

			break;
		}
		case VRS_LOCAL:  // 绕自身坐标系旋转像机
			{
				D3DXMATRIX matRosX, matRosY, matRosZ, matRot, matTrans, mat;
				D3DXVECTOR3 _dirction = m_realLookat - m_realPosition;

				// 计算旋转矩阵
				D3DXMatrixRotationAxis(&matRosZ, &_dirction, pVec->z);
				D3DXMatrixRotationAxis(&matRosX, &m_realX, pVec->x);
				D3DXMatrixRotationAxis(&matRosY, &m_realY, pVec->y);
				D3DXMatrixMultiply(&matRot, &matRosZ, &matRosX);
				D3DXMatrixMultiply(&matRot, &matRot, &matRosY);

				// 计算对变换矩阵
				D3DXMatrixTranslation(&matTrans, -m_realPosition.x, -m_realPosition.y, -m_realPosition.z);
				D3DXMatrixMultiply(&mat, &matTrans, &matRot);
				D3DXMatrixTranslation(&matTrans, m_realPosition.x, m_realPosition.y, m_realPosition.z);
				D3DXMatrixMultiply(&mat, &mat, &matTrans);

				// 计算新
				D3DXVec3TransformCoord(&m_realPosition, &m_realPosition, &mat);
				D3DXVec3TransformCoord(&m_realLookat, &m_realLookat, &mat);
				D3DXVec3TransformNormal(&m_realY, &m_realY, &mat);
				D3DXVec3Cross(&m_realX, &m_realY, &(m_realLookat - m_realPosition));  
				D3DXMatrixMultiply(&m_worldTransform, &m_worldTransform, &mat);  // 更新世界变换矩阵

				// 更新父坐标系下的矩阵
				UpdateLocalMatrix();
				break;
			}
		default:
			break;
	} // switch
}

//-----------------------------------------------------------------------------
// 方法名: SetRotation()
// 参  数: D3DXVECTOR3* pRos
// 用  途: 在指定的坐标系内设置摄像机的转角
//-----------------------------------------------------------------------------
//VOID CCamera::SetRotation(D3DXVECTOR3* pRos, VGS_ROTATIONSPACE coordEnum)
//{	
	//if (!pRos || !m_pNode)
	//{
	//	return;
	//}

	//switch (coordEnum)
	//{	
	//case VRS_WORLD:  // 以世界坐标系原点为中心，绕世界坐标系旋转摄像机
	//	{
	//		D3DXMATRIX matRosX, matRosY, matRosZ, matRot, mat;

	//		// 计算旋转矩阵
	//		D3DXMatrixRotationX(&matRosX, pRos->x);
	//		D3DXMatrixRotationY(&matRosY, pRos->y);
	//		D3DXMatrixRotationZ(&matRosZ, pRos->z);
	//		D3DXMatrixMultiply(&matRot, &matRosZ, &matRosX);
	//		D3DXMatrixMultiply(&matRot, &matRot, &matRosY);

	//		D3DXMatrixTranslation(&mat, )
	//		
	//	}
	//case VRS_WORLD2:
	//	{
	//	}
	//case VRS_LOCAL:  // 绕自身坐标系旋转像机
	//	{
	//	}
	//default:
	//	break;
	//}

	//D3DXMATRIX matRosX, matRosY, matRosZ, matTrans, mat;
	////以from为中心，旋转at,绕自身Z轴旋转不影响at的位置
	////本系统旋转摄像机顺序为: 先绕自身Y轴转，再绕自身X轴转，再绕自身Z轴转
	//D3DXMatrixTranslation(&matTrans, -m_from.x, -m_from.y, -m_from.z);
	//D3DXMatrixRotationAxis(&matRosY, &vy, -pRos->y);
	//D3DXMatrixRotationAxis(&matRosZ, &vz, -pRos->z);
	//D3DXMatrixRotationAxis(&matRosX, &vx, -pRos->x);
	//D3DXMatrixMultiply(&transform.rotation_matrix, &matRosY, &matRosZ);
	//D3DXMatrixMultiply(&transform.rotation_matrix, &transform.rotation_matrix, &matRosX);

	//D3DXMatrixMultiply(&mat, &matTrans, &transform.rotation_matrix);
	//D3DXMatrixTranslation(&matTrans, m_from.x, m_from.y, m_from.z);
	//D3DXMatrixMultiply(&mat, &mat, &matTrans);
	////更新at 矢量
	//D3DXVec3TransformCoord(&m_at, &ori_at, &mat);
	////更新up矢量
	//D3DXVec3TransformCoord(&m_up, &m_up, &mat);

	//transform.rotation = *pRos;

//}

//-----------------------------------------------------------------------------
// 方法名: CreateLookAt()
// 参  数: CModel* pModel
// 用  途: 摄像机正对,在模型帧渲染中对正
//-----------------------------------------------------------------------------
//##ModelId=47EBC74E038C
VOID CCamera::SetLookAtModel(CModel* pModel)
{
	m_pLookatModel = pModel;
}

//-----------------------------------------------------------------------------
// 方法名: DeleteLookAt()
// 参  数: 
// 用  途: 删除摄像机正对
//-----------------------------------------------------------------------------
//##ModelId=47EBC74E038E
VOID CCamera::DeleteLookAtModel()
{
	m_pLookatModel = NULL;
}

//-----------------------------------------------------------------------------
// 方法名: Round()
// 参  数: D3DXVECTOR3* pAxisVec, FLOAT fAngle
// 用  途: 以相机at点为中心，以pAxisVec为轴，旋转fAngle弧度
//-----------------------------------------------------------------------------
VOID CCamera::Round(D3DXVECTOR3* pAxisVec, FLOAT fAngle)
{	
	RoundEx(&m_realLookat, pAxisVec, fAngle);
}

// 在世界坐标系内，以目标点为中心，以pCenter为轴心点，pAxisVec为轴，旋转fAngle弧度
VOID CCamera::RoundEx(D3DXVECTOR3* pCenter, D3DXVECTOR3* pAxisVec, FLOAT fAngle)
{
	if (!pCenter || !pAxisVec || fAngle == 0.0f)
	{
		return;
	}

	// 计算绕pAxisVec旋转的变换矩阵
	D3DXMATRIX matTrans, matRot, mat;
	D3DXMatrixTranslation(&matTrans, -pCenter->x, -pCenter->y, -pCenter->z);
	D3DXMatrixRotationAxis(&matRot, pAxisVec, -fAngle);
	D3DXMatrixMultiply(&mat, &matTrans, &matRot);
	D3DXMatrixTranslation(&matTrans, pCenter->x, pCenter->y, pCenter->z);
	D3DXMatrixMultiply(&mat, &mat, &matTrans);

	// 更新相机的变换
	D3DXVec3TransformCoord(&m_realPosition, &m_realPosition, &mat);
	D3DXVec3TransformCoord(&m_realLookat, &m_realLookat, &mat);		
	D3DXVec3TransformNormal(&m_realY, &m_realY, &mat);
	D3DXVec3TransformNormal(&m_realX, &m_realX, &mat);
	D3DXMatrixMultiply(&m_worldTransform, &m_worldTransform, &mat);  // 更新世界变换矩阵

	// 更新父节点下的变换矩阵
	UpdateLocalMatrix();
}


//-----------------------------------------------------------------------------
// 方法名: RoundInCameraSpace()
// 参  数: FLOAT fAngleX, FLOAT fAngleY, FLOAT fAngleZ
// 用  途: 以at点为中心，以相机自身XYZ轴为旋转轴，旋转fAngle弧度
//-----------------------------------------------------------------------------
VOID CCamera::RoundInCameraSpace(FLOAT fAngleX, FLOAT fAngleY, FLOAT fAngleZ)
{
	RoundInCameraSpaceEx(&m_realLookat, fAngleX, fAngleY, fAngleZ);
}

//-----------------------------------------------------------------------------
// 方法名: RoundInCameraSpace()
// 参  数: FLOAT fAngleX, FLOAT fAngleY, FLOAT fAngleZ
// 用  途: 以定点为中心，以相机自身XYZ轴为旋转轴，旋转fAngle弧度
//-----------------------------------------------------------------------------
VOID CCamera::RoundInCameraSpaceEx(D3DXVECTOR3* pCenter, FLOAT fAngleX, FLOAT fAngleY, FLOAT fAngleZ)
{
	if (!pCenter)
	{
		return;
	}

	if (fAngleX != 0)
	{
		RoundEx(pCenter, &m_realX, fAngleX);
	}

	if (fAngleY != 0)
	{
		/*D3DXVECTOR3 Axis;
		if (vy.y > 0)
			Round1(pCenter, &D3DXVECTOR3(0, 1, 0), fAngleY);
		else
			Round1(pCenter, &D3DXVECTOR3(0, -1, 0), fAngleY);*/
		RoundEx(pCenter, &m_realY, fAngleY);
	}

	if (fAngleZ != 0)
	{
		RoundEx(pCenter, &(m_realPosition - m_realLookat), fAngleZ);
	}
}

//-----------------------------------------------------------------------------
// 方法名: SetFrom()
// 参  数: D3DXVECTOR3* pFrom
// 用  途: 设置相机的位置，自身坐标轴保持不变
//-----------------------------------------------------------------------------
VOID CCamera::SetPosition(D3DXVECTOR3* pPos, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pPos)
	{
		return;
	}

	D3DXVECTOR3 moveVec;
	switch (coordEnum)
	{		
		case VTS_WORLD:  //在世界坐标系平移摄像机
			{
				moveVec = *pPos - m_realPosition;
				m_realPosition = *pPos;
				m_realLookat += moveVec;

				m_worldTransform._41 = m_realPosition.x;
				m_worldTransform._42 = m_realPosition.y;
				m_worldTransform._43 = m_realPosition.z;

				UpdateLocalMatrix();
				break;
			}
		case VTS_LOCAL:  //在自身坐标系平移
			{
				D3DXVECTOR3 vNewWorldPos;
				D3DXVec3TransformCoord(&vNewWorldPos, pPos, &m_worldTransform);
				SetPosition(&vNewWorldPos, VTS_WORLD);
				break;
			}
		
		//walk，不能飞到天上去
		case VTS_PARENT:
			{
				moveVec = *pPos - m_localPosition;
				m_localPosition = *pPos;
				m_localLookat   += moveVec;

				m_localTransform._41 = m_localPosition.x;
				m_localTransform._42 = m_localPosition.y;
				m_localTransform._43 = m_localPosition.z;

				UpdateWorldMatrix();
				break;
			}
		default:
			{
				return;
			}
	}
}
// 获取相机的位置
D3DXVECTOR3 CCamera::GetPosition(VGS_TRANSFORMSPACE coordEnum)
{
	switch (coordEnum)
	{
	case VTS_WORLD:
		return m_realPosition;
	case VTS_LOCAL:
		return D3DXVECTOR3_ZERO;
	case VTS_PARENT:
		return m_localPosition;
	default:
		break;
	}
	return D3DXVECTOR3(0, 0, 0);
}

//-----------------------------------------------------------------------------
// 方法名: SetAt()
// 参  数: D3DXVECTOR3* pAt
// 用  途: 通过设置相机的目标点来改变相机的lookat方向以及自身坐标轴
//-----------------------------------------------------------------------------
VOID CCamera::Lookat(const D3DXVECTOR3* pAt, VGS_TRANSFORMSPACE coordEnum)
{
	if (!pAt)
	{
		return;
	}

	switch (coordEnum)
	{
	case VTS_WORLD:
		{
			D3DXVECTOR3 v3At = *pAt;
			D3DXMATRIX nodeMat = GetNodeWordMatrix();
			D3DXMATRIX nodeMatInv;
			D3DXMatrixInverse(&nodeMatInv, NULL, &nodeMat);    // node变换的逆矩阵
			D3DXVec3TransformCoord(&v3At, pAt, &nodeMatInv);

			Lookat(&v3At, VTS_PARENT);
			break;
		}
	case VTS_LOCAL:
		{
			MessageBox(NULL, TEXT("函数没有实现"), TEXT("提示"), 0);
			break;
		}
	case VTS_PARENT:
		{
			D3DXVECTOR3 vOldDirection = m_localLookat - m_localPosition;  // 旧lookat方向
			D3DXVec3Normalize(&vOldDirection, &vOldDirection);
			m_localLookat = *pAt;										  // 新at点
			D3DXVECTOR3 vNewDirection = m_localLookat - m_localPosition;  // 新lookat方向
			D3DXVec3Normalize(&vNewDirection, &vNewDirection);

			// 计算旋转的角度及up和x轴
			D3DXMATRIX matRotation = _getRotationMatrix(vOldDirection, vNewDirection);

			// y轴不要变
			D3DXVec3Normalize(&m_localY, &m_localY);
			D3DXVec3Cross(&m_localX, &m_localY, &vNewDirection);

			// 修改变换矩阵的xy轴
			D3DXMatrixMultiply(&m_localTransform, &m_localTransform, &matRotation);
			m_localTransform._11 = m_localX.x;
			m_localTransform._12 = m_localX.y;
			m_localTransform._13 = m_localX.z;
			m_localTransform._21 = m_localY.x;
			m_localTransform._22 = m_localY.y;
			m_localTransform._23 = m_localY.z;

			UpdateWorldMatrix();

			break;
		}
	default:
		break;
	}
}

D3DXVECTOR3 CCamera::GetLookat(VGS_TRANSFORMSPACE coordEnum)
{
	switch (coordEnum)
	{
	case VTS_WORLD:
		{
			//UpdateFromNode();
			return m_realLookat;
		}
	case VTS_LOCAL:
		{
			// MessageBox(NULL, "函数没有实现", "提示", 0);
			break;
		}
	case VTS_PARENT:
		{
			// UpdateFromNode();
			return m_localLookat;
		}
	default:
		break;
	}

	return D3DXVECTOR3_Z;
}

// 设置转角
VOID CCamera::setRotation(const D3DXQUATERNION &quat, VGS_TRANSFORMSPACE coordEnum)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, &quat);

	D3DXVECTOR3 v3X, v3Y, v3Z;
	D3DXVec3TransformNormal(&v3X, &D3DXVECTOR3_X, &mat);
	D3DXVec3TransformNormal(&v3Y, &D3DXVECTOR3_Y, &mat);
	D3DXVec3TransformNormal(&v3Z, &D3DXVECTOR3_Z, &mat);

	switch (coordEnum)
	{
	case VTS_WORLD:
		{
			D3DXVECTOR3 v3At = m_realPosition + 100 * v3Z;
			Lookat(&v3At, VTS_WORLD);
			SetUp(v3Y, VTS_WORLD);
			break;
		}
	case VTS_LOCAL:
		{
			D3DXVec3TransformNormal(&v3Y, &v3Y, &m_localTransform);
			D3DXVec3TransformNormal(&v3Z, &v3Z, &m_localTransform);
			D3DXVECTOR3 v3At = m_localPosition + 100 * v3Z;
			Lookat(&v3At, VTS_PARENT);
			SetUp(v3Y, VTS_PARENT);
			break;
		}
	case VTS_PARENT:
		{
			D3DXVECTOR3 v3At = m_localPosition + 100 * v3Z;
			Lookat(&v3At, VTS_PARENT);
			SetUp(v3Y, VTS_PARENT);
			break;
		}
	default:
		break;
	}
}

//-----------------------------------------------------------------------------
// 方法名: SetRoll()
// 参  数: D3DXVECTOR3* pAt
// 用  途: 设置Roll
//-----------------------------------------------------------------------------
VOID CCamera::SetRoll(FLOAT fRoll)
{
	// m_fRoll = fRoll;	
	UpdateFromRoll(fRoll);
}

// 设置相机的Up(自身Y轴)方向，也将会影响自身的X轴
VOID CCamera::SetUp(const D3DXVECTOR3 &up, VGS_TRANSFORMSPACE coordEnum)
{
	switch (coordEnum)
	{
	case VTS_WORLD:
		{
			D3DXMATRIX nodeMatInv;
			D3DXMATRIX nodeMat = GetNodeWordMatrix();
			D3DXMatrixInverse(&nodeMatInv, 0, &nodeMat);

			D3DXVECTOR3 v3NewUp;
			D3DXVec3TransformNormal(&v3NewUp, &up, &nodeMatInv);
			//D3DXVec3Normalize(&v3NewUp, &v3NewUp);

			SetUp(v3NewUp, VTS_PARENT);

			//D3DXVECTOR3 vDir = m_realLookat - m_realPosition;
			//float len = D3DXVec3Length(&vDir);  // from到at的距离

			//D3DXVECTOR3 vOldUp = m_realY;  // 旧up点
			//m_realY = up;                  // 新up点

			//// 计算旋转的角度及up和x轴
			//D3DXMATRIX matRotation = _getRotationMatrix(vOldUp, m_realY);
			//D3DXVec3TransformNormal(&vDir, &vDir, &matRotation);
			//D3DXVec3Normalize(&vDir, &vDir);
			//m_realLookat = m_realPosition + vDir * len;  // 新的at点
			//D3DXVec3Cross(&m_realX, &m_realY, &vDir);
			//D3DXMatrixMultiply(&m_worldTransform, &m_worldTransform, &matRotation);

			//UpdateLocalMatrix();

			break;
		}
	case VTS_LOCAL:
		{
			D3DXMATRIX nodeMatInv;
			D3DXMATRIX nodeMat = GetNodeWordMatrix();

			D3DXVECTOR3 v3NewUp;
			D3DXVec3TransformNormal(&v3NewUp, &up, &nodeMat);
			//D3DXVec3Normalize(&v3NewUp, &v3NewUp);

			SetUp(v3NewUp, VTS_PARENT);

			break;
		}
	case VTS_PARENT:
		{
			D3DXVECTOR3 v3NewUp;
			D3DXVec3Normalize(&m_localY, &up);
			D3DXVECTOR3 v3Z = m_localLookat - m_localPosition;
			D3DXVec3Normalize(&v3Z, &v3Z);
			D3DXVec3Cross(&m_realX, &m_localY, &v3Z);
			D3DXVec3Normalize(&m_localX, &m_localX);

			m_localTransform._11 = m_localX.x;
			m_localTransform._12 = m_localX.y;
			m_localTransform._13 = m_localX.z;
			m_localTransform._21 = m_localY.x;
			m_localTransform._22 = m_localY.y;
			m_localTransform._23 = m_localY.z;
			m_localTransform._31 = v3Z.x;
			m_localTransform._32 = v3Z.y;
			m_localTransform._33 = v3Z.z;

			UpdateWorldMatrix();

			break;
		}
	default:
		break;
	}
}

D3DXVECTOR3 CCamera::GetUp(VGS_TRANSFORMSPACE coordEnum)
{
	switch (coordEnum)
	{
	case VTS_WORLD:
		{
			//UpdateFromNode();
			return m_realY;
		}
	case VTS_LOCAL:
		{
			return D3DXVECTOR3_Y;
			// MessageBox(NULL, "函数没有实现", "提示", 0);
			break;
		}
	case VTS_PARENT:
		{
			// UpdateFromNode();
			return m_localY;
		}
	default:
		break;
	}

	return D3DXVECTOR3_Y;
}
// 获取自身x轴方向
D3DXVECTOR3 CCamera::GetRight(VGS_TRANSFORMSPACE coordEnum)
{
	switch (coordEnum)
	{
	case VTS_WORLD:
		{
			return m_realX;
		}
	case VTS_LOCAL:
		{
			return D3DXVECTOR3_X;
			// MessageBox(NULL, "函数没有实现", "提示", 0);
			break;
		}
	case VTS_PARENT:
		{
			// UpdateFromNode();
			return m_localX;
		}
	default:
		break;
	}

	return D3DXVECTOR3_X;
}

// 获取相机的lookat方向（自身Z方向), 相对于世界坐标系
D3DXVECTOR3 CCamera::GetDirection(VGS_TRANSFORMSPACE coordEnum)
{
	D3DXVECTOR3 v3Dir = VGS_ZEROVECTOR3;
	switch (coordEnum)
	{
	case VTS_WORLD:
		{
			// UpdateFromNode();
			v3Dir = m_realLookat - m_realPosition;
			D3DXVec3Normalize(&v3Dir, &v3Dir);
			return v3Dir;
		}
	case VTS_LOCAL:
		{
			return D3DXVECTOR3_Z;
			// MessageBox(NULL, "函数没有实现", "提示", 0);
			break;
		}
	case VTS_PARENT:
		{
			// UpdateFromNode();
			v3Dir = m_localLookat - m_localPosition;
			D3DXVec3Normalize(&v3Dir, &v3Dir);
			return v3Dir;
		}
	default:
		break;
	}

	return D3DXVECTOR3_Z;
}


//-----------------------------------------------------------------------------
// 方法名: SetFov()
// 参  数: FLOAT fov
// 用  途: 设置FOV
//-----------------------------------------------------------------------------
VOID CCamera::SetFov(FLOAT _fov)
{
	m_fFov = _fov;//*ATOS;
	m_tanHalfFieldOfViewV = (FLOAT)tan(m_fFov/2);
}

void CCamera::UpdateMatrixFromParent()
{
	UpdateFromNode();
}

//////////////////////////////// private method ///////////////////////////////////

// 从父节点的变化矩阵更新本相机的变化矩阵
//void CCamera::UpdateMatrix()
//{
//	if (m_bParentTransformChanged && m_pNode)
//	{
//		D3DXMATRIX parentMat = GetNodeWordMatrix();
//		D3DXMatrixMultiply(&m_worldTransform, &parentMat, &m_localTransform);   // 自身的世界左边系下的变换
//		m_realPosition = D3DXVECTOR3(m_worldTransform._41, m_worldTransform._42, m_worldTransform._43);  
//		D3DXVec3TransformCoord(&m_realLookat, &m_localLookat, &parentMat);     // at点的世界坐标
//		m_realX = D3DXVECTOR3(m_worldTransform._11, m_worldTransform._12, m_worldTransform._13);
//		m_realY = D3DXVECTOR3(m_worldTransform._21, m_worldTransform._22, m_worldTransform._23);
//
//		m_bParentTransformChanged = false;
//	}
//}

// 当节点变换矩阵发生了变化是，用于自身的更新
void CCamera::UpdateFromNode()
{
	if (/*m_bParentTransformChanged && */m_pNode)
	{
		D3DXMATRIX parentMat = GetNodeWordMatrix();
		D3DXMatrixMultiply(&m_worldTransform, &m_localTransform, &parentMat);   // 自身的世界左边系下的变换
		m_realPosition = D3DXVECTOR3(m_worldTransform._41, m_worldTransform._42, m_worldTransform._43);
		m_realX = D3DXVECTOR3(m_worldTransform._11, m_worldTransform._12, m_worldTransform._13);
		m_realY = D3DXVECTOR3(m_worldTransform._21, m_worldTransform._22, m_worldTransform._23);  
		D3DXVec3TransformCoord(&m_realLookat, &m_localLookat, &parentMat);     // at点的世界坐标

		// m_bParentTransformChanged = false;
	}

}

VOID CCamera::InitVars()
{
	m_type = VGS_MOVABLETYPE_CAMERA;
	m_camType = VCT_PROJECT;
	m_pNode = NULL;
	m_realPosition = D3DXVECTOR3_ZERO;
	m_realLookat = D3DXVECTOR3_Z;
	m_realY = D3DXVECTOR3_Y;
	m_fFov = 45 * ATOS;
	//m_fRoll = 0.0f;
	m_nearClip = 5.0f;
	m_farClip = 1000000.0f;

	m_sName = "DefaultCamera";

	m_tanHalfFieldOfViewV = (FLOAT)tan(m_fFov/2);

	m_pLookatModel = 0;
	D3DXMatrixIdentity(&m_worldTransform);
	D3DXMatrixIdentity(&m_localTransform);


	m_OriginLookatPoint    = D3DXVECTOR3_Z;
	m_OriginWorldPosition  = D3DXVECTOR3_ZERO;
	m_OriginRoll = 0;

	m_localPosition = D3DXVECTOR3_ZERO;
	m_localLookat = D3DXVECTOR3_Z;
	m_localY = D3DXVECTOR3_Y;
	m_localX = D3DXVECTOR3_X;
	m_realPosition = D3DXVECTOR3_ZERO;
	m_realLookat = D3DXVECTOR3_Z;
	m_realY = D3DXVECTOR3_Y;
	m_realX = D3DXVECTOR3_X;

	//// name = std::string("DefaultCamera");
	//setName("DefaultCamera");

	//kfrInfo = NULL;
	//lookAtModel = NULL;

	//index = -1;
	//kfrCnt = 0;	
	//kfrCurFrame = 0;
	//currentTimer = 0;
	//playKfr = TRUE;	
	//kfrFPS = 25;	
	//kfrFrameTime = 35;
	//isPlaying = TRUE;	
	//kfrRepeatCnt = -1;
	//kfrRepeatIndex = 0;
	//isPlaying = FALSE;
	//tanHalfFieldOfViewV = 1.0f;
	//kfrFSMotionBlur = NULL;	
	//kfrFSMotionBlurValue = NULL;

	//InvokeFuncName = NULL;
	//
	//InitMatrix();

	m_pScene = NULL;
}


//-----------------------------------------------------------------------------
// 方法名: UpdateTranslateMatrix()
// 参  数: 
// 用  途: UpdateTranslateMatrix
//-----------------------------------------------------------------------------
//VOID CCamera::UpdateTranslateMatrix()
//{
//	//摄像机的位置矩阵始终为from点的位置
//	D3DXMatrixTranslation(&transform.translate_matrix, m_from.x, m_from.y, m_from.z);
//	transform.position = m_from;
//}

//-----------------------------------------------------------------------------
// 方法名: InitCameraVars()- OK！！
// 参  数: 
// 用  途: 得到像机初始状态的变换矩阵。通过相机的from, at, roll 计算相机自身坐标系
//		   从at->from的方向为相机的自身z方向，相机视野水平方向为x,垂直为y
//		   摄像机正朝世界坐标系X方向为初始状态0度转角
//-----------------------------------------------------------------------------
VOID CCamera::UpdateAddtionalMatrix()
{		
	UpdateFromNode();

	/*---------------------------------------------------------------
	View矩阵的表示如下，相机的x,y,z轴和视点的位置
	xaxis.x           yaxis.x           zaxis.x           0
	xaxis.y           yaxis.y           zaxis.y           0
	xaxis.z           yaxis.z           zaxis.z           0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  l
	----------------------------------------------------------------*/
	D3DXMatrixLookAtLH(&m_view, &m_realPosition, &m_realLookat, &m_realY);
	
	// perspective matrix
	D3DXMatrixPerspectiveFovLH(&m_proj, m_fFov, m_aspect, m_nearClip, m_farClip);
	
	// and finally the combined viewproj
	m_viewproj = m_view * m_proj;
	
	// and all the inverses
	D3DXMatrixInverse(&m_invproj, NULL, &m_proj);
	D3DXMatrixInverse(&m_invview, NULL, &m_view);
	D3DXMatrixInverse(&m_invviewproj, NULL, &m_viewproj);
	// D3DXVec3TransformNormal(&m_right, &D3DXVECTOR3(1,0,0), &m_invview);
}

//-----------------------------------------------------------------------------
// 方法名: MultiplyMatrix()
// 参  数: 
// 用  途: MultiplyMatrix
//-----------------------------------------------------------------------------
//##ModelId=47EBC74E03CA
//VOID CCamera::MultiplyMatrix()
//{
//	//摄像机的缩放矩阵始终为单位矩阵
//	D3DXMatrixMultiply(&transform.matrix, &transform.rotation_matrix, &transform.translate_matrix);
//}

//-----------------------------------------------------------------------------
// 方法名: GetUpByRoll()
// 参  数: 
// 用  途: 通过Roll值获得up矢量
//-----------------------------------------------------------------------------
//##ModelId=47EBC74E03C2
//VOID CCamera::GetUpByRoll()
//{
//	//vz
//	D3DXVECTOR3 fromToAt = m_at - m_from;
//	D3DXVec3Normalize(&fromToAt, &fromToAt);
//	vz = -fromToAt;
//
//	//vx
//	if (D3DXVec3Dot(&vz, &D3DXVECTOR3(0,1,0)) == 1)
//	{
//		vx = D3DXVECTOR3(1, 0, 0);
//	}
//	else
//	{
//		D3DXVec3Cross(&vx, &vz, &D3DXVECTOR3(0,1,0));
//		D3DXVec3Normalize(&vx, &vx);
//	}
//	D3DXMATRIX matRoll;
//	D3DXMatrixRotationYawPitchRoll(&matRoll, 0, 0, m_roll*ATOS);
//	D3DXVec3TransformNormal(&vx, &vx, &matRoll);
//
//	//vy
//	D3DXVec3Cross(&vy, &vx, &vz);
//	D3DXVec3Normalize(&vy, &vy);
//	m_up = vy;	
//}

//-----------------------------------------------------------------------------
// 方法名: InitMatrix()
// 参  数: 
// 用  途: 得到像机初始状态的变换矩阵。通过相机的from, at, roll 计算相机自身坐标系
//		   从at->from的方向为相机的自身z方向，相机视野水平方向为x,垂直为y
//		   摄像机正朝世界坐标系X方向为初始状态0度转角
//-----------------------------------------------------------------------------
//##ModelId=47EBC74E03C3
//VOID CCamera::InitMatrix()
//{
//	//得到当前坐标系
//	GetAxisVector();
//	//摄像机的旋转矩阵，朝x正方向为0度
//	transform.rotation_matrix._11 = -vz.x;
//	transform.rotation_matrix._12 = -vz.y;
//	transform.rotation_matrix._13 = -vz.z;
//	transform.rotation_matrix._21 = vy.x;
//	transform.rotation_matrix._22 = vy.y;
//	transform.rotation_matrix._23 = vy.z;
//	transform.rotation_matrix._31 = -vx.x;
//	transform.rotation_matrix._32 = -vx.y;
//	transform.rotation_matrix._33 = -vx.z;
//	//得到角度
//	GetRotationVecByRT(&transform.rotation_matrix, &transform.rotation);
//
//	//平移矩阵
//	UpdateTranslateMatrix();
//	ori_from = m_from;
//	ori_at = m_at;
//	ori_roll = m_roll;
//
//	//Multiply matrix
//	MultiplyMatrix();
//
//}

//-----------------------------------------------------------------------------
// 方法名: GetAxisVector()
// 参  数: 
// 用  途: 通过相机的from, at, roll 计算相机自身坐标系
//		   从at->from的方向为相机的自身z方向，相机视野水平方向为x,垂直为y
//		   摄像机正朝世界坐标系X方向为初始状态0度转角
//-----------------------------------------------------------------------------
//##ModelId=47EBC74E03C8
//VOID CCamera::GetAxisVector()
//{
//	m_localZ = m_at - m_from;
//
//	D3DXVec3Normalize(&m_up, &m_up);
//	m_localY = m_up;
//
//	3DXVec3Cross(&m_localX, &m_localY, &m_localZ);
//	D3DXVec3Normalize(&m_localX, &m_localX);
//
//	//vz
//	//D3DXVECTOR3 fromToAt = m_at - m_from;
//	//D3DXVec3Normalize(&fromToAt, &fromToAt);
//	//vz = -fromToAt;
//
//	////vy
//	//vy = m_up;
//
//	////vx
//	//D3DXVec3Cross(&vx, &vz, &vy);
//	//D3DXVec3Normalize(&vx, &vx);
//}

//克隆相机
//CCamera* CCamera::clone(std::string newCamName)
//{
//	CCamera* pNewCam = new CCamera();	
//	
//	//相机的基本信息
//	/*pNewCam->setName(newCamName); 
//	pNewCam->SetFrom(&GetFrom());
//	pNewCam->SetAt(&GetAt());
//	pNewCam->SetFov(GetFov());
//    pNewCam->SetRoll(GetRoll());
//	pNewCam->nearClip = nearClip;
//	pNewCam->farClip = farClip;
//
//	pNewCam->m_localTransform = m_localTransform;*/
//
//	//if (kfrCnt > 0)
//	//{
//	//	//关键帧动画信息
//	//	pNewCam->kfrInfo = new VGS_CAMERA_INFO[kfrCnt];
//	//	for (UINT i = 0; i< kfrCnt; i++)
//	//	{
//	//		pNewCam->kfrInfo[i].from = kfrInfo[i].from;
//	//		pNewCam->kfrInfo[i].at = kfrInfo[i].at;
//	//		pNewCam->kfrInfo[i].roll = kfrInfo[i].roll;
//	//		pNewCam->kfrInfo[i].fov = kfrInfo[i].fov;
//	//	}
//	//	
//	//	//动态模糊信息
//	//	if (kfrFSMotionBlur)
//	//	{
//	//		pNewCam->kfrFSMotionBlur = new BOOL[kfrCnt];
//	//		pNewCam->kfrFSMotionBlurValue = new INT[kfrCnt];
//	//		for (UINT i = 0; i< kfrCnt ;i++)
//	//		{
//	//			pNewCam->kfrFSMotionBlur[i] = kfrFSMotionBlur[i];
//	//			pNewCam->kfrFSMotionBlurValue[i] = kfrFSMotionBlurValue[i];
//	//		}
//	//	}
//
//	//	//关键帧动画过程中的JS Function调用信息
//	//	if (InvokeFuncName)
//	//	{
//	//		pNewCam->InvokeFuncName = new _NAME[kfrCnt];
//	//		for (UINT i =0; i< kfrCnt; i++)
//	//		{
//	//			strcpy_s(pNewCam->InvokeFuncName[i].name, kfrCnt, InvokeFuncName[i].name); 
//	//		}
//	//	}
//	//}
//
//	return pNewCam;
//}

//-----------------------------------------------------------------------------
// 方法名: Release()
// 参  数: 
// 用  途: 释放资源
//-----------------------------------------------------------------------------
//##ModelId=47EBC74E03D0
VOID CCamera::Release()
{
	// 设置使用该相机的视口
// 	set <CViewport*>::iterator it = m_vViewport.begin();
// 	while (it != m_vViewport.end())
// 	{
// 		if ((*it))
// 		{
// 			(*it)->SetCamera(NULL);
// 		}
// 		++it;
// 	}

	//SAFE_DELETE_ARRAY(kfrInfo);	
	//SAFE_DELETE_ARRAY(kfrFSMotionBlur);	
	//SAFE_DELETE_ARRAY(kfrFSMotionBlurValue);
	//if (kfrCnt > 1)
	//{
	//	if (InvokeFuncName)
	//	{
	//		for (UINT i = 0; i< kfrCnt; i++)
	//		{
	//			_NAME NAME = InvokeFuncName[i];
	//			if (NAME.name)
	//				SAFE_DELETE_ARRAY(NAME.name);
	//		}
	//		SAFE_DELETE_ARRAY(InvokeFuncName);
	//	}
	//}
}


// VOID CCamera::SetName(const std::string &sName)
// {
// 	this->name = sName;
// }
// std::string CCamera::GetName()
// {
// 	return this->name;
// }

// 获得相机所属的场景
CScene* CCamera::GetScene() 
{
	return m_pScene;
}

// 设置相机所属的场景
VOID CCamera::SetScene(CScene *pScene)
{
	m_pScene = pScene;
}

// 渲染场景
// VOID CCamera::RenderScene()
// {
// 	m_pScene->Render();
// }

// 添加一个视口
VOID CCamera::addViewport(DWORD pViewport)
{
	bool bAdded = false;
	for (int i = 0; i < (int)m_vViewport.size(); ++i)
	{
		if (m_vViewport[i] == pViewport)
		{
			bAdded = true;
			break;
		}
	}
	if (!bAdded)
	{
		m_vViewport.push_back(pViewport);
	}
	// vector<DWORD>::iterator it = find(m_vViewport.begin(), m_vViewport.end(), pViewport);
	// m_vViewport.insert(pViewport);
}
// 删除一个视口
VOID CCamera::removeViewport(DWORD pViewport)
{
	for (int i = 0; i < (int)m_vViewport.size(); ++i)
	{
		if (m_vViewport[i] == pViewport)
		{
			m_vViewport.erase(m_vViewport.begin() + i);
			break;
		}
	}
}
VOID CCamera::getViewport(vector<DWORD> &vViewport)
{
	vViewport = m_vViewport;
}

VOID CCamera::getViewMatrix(D3DXMATRIX *mat)
{
	/*---------------------------------------------------------------
	View矩阵的表示如下，相机的x,y,z轴和视点的位置
	xaxis.x           yaxis.x           zaxis.x           0
	xaxis.y           yaxis.y           zaxis.y           0
	xaxis.z           yaxis.z           zaxis.z           0
	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  l
	----------------------------------------------------------------*/
	D3DXMatrixLookAtLH(mat, &m_realPosition, &m_realLookat, &m_realY);
}

VOID CCamera::getProjectMatrix(D3DXMATRIX *mat)
{
	float aspect = 1.3f;
	// perspective matrix
	if (m_vViewport.size() > 0)
	{
		aspect = ((CViewport*)m_vViewport[0])->getAspectRatio();
	}
	D3DXMatrixPerspectiveFovLH(mat, m_fFov, aspect, m_nearClip, m_farClip);
}

//VOID CCamera::Update()
//{		
//	// view matrix
//	GetAxisVector();
//	/*---------------------------------------------------------------
//	View矩阵的表示如下，相机的x,y,z轴和视点的位置
//	xaxis.x           yaxis.x           zaxis.x           0
//	xaxis.y           yaxis.y           zaxis.y           0
//	xaxis.z           yaxis.z           zaxis.z           0
//	-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  l
//	----------------------------------------------------------------*/
//	D3DXMatrixLookAtLH(&m_view, &m_from, &(m_at), &m_up);
//
//	//		D3DXVECTOR3 upVec  = D3DXVECTOR3(0,1,0);
//	//D3DXMatrixLookAtLH(&m_view, &m_from, &(m_from + m_forward), &upVec);
//
//	// perspective matrix
//	D3DXMatrixPerspectiveFovLH(&m_proj, m_fov, m_aspect, nearClip, farClip);
//
//	// and finally the combined viewproj
//	m_viewproj = m_view * m_proj;
//
//	// and all the inverses
//	D3DXMatrixInverse(&m_invproj, NULL, &m_proj);
//	D3DXMatrixInverse(&m_invview, NULL, &m_view);
//	D3DXMatrixInverse(&m_invviewproj, NULL, &m_viewproj);
//}

// 用世界坐标系下的矩阵计算父坐标系下的矩阵
VOID CCamera::UpdateLocalMatrix()
{
	// if (m_pNode)
	{
		D3DXMATRIX NodeMat = GetNodeWordMatrix();          // node的变换矩阵
		D3DXMATRIX NodeMatInv;
		D3DXMatrixInverse(&NodeMatInv, NULL, &NodeMat);    // node变换的逆矩阵
		D3DXMatrixMultiply(&m_localTransform, &m_worldTransform, &NodeMatInv);   // 得到节点坐标系下的变换 L = W * N~

		m_realPosition.x = m_worldTransform._41;
		m_realPosition.y = m_worldTransform._42;
		m_realPosition.z = m_worldTransform._43;
		m_realX = D3DXVECTOR3(m_worldTransform._11, m_worldTransform._12, m_worldTransform._13);
		m_realY = D3DXVECTOR3(m_worldTransform._21, m_worldTransform._22, m_worldTransform._23);

		m_localPosition = D3DXVECTOR3(m_localTransform._41, m_localTransform._42, m_localTransform._43);  

		D3DXVECTOR3 v3LocalPosition;
		D3DXVec3TransformCoord(&v3LocalPosition, &m_realPosition, &NodeMatInv);     // at点的世界坐标

		D3DXVec3TransformCoord(&m_localLookat, &m_realLookat, &NodeMatInv);     // at点的世界坐标
		m_localX = D3DXVECTOR3(m_localTransform._11, m_localTransform._12, m_localTransform._13);
		m_localY = D3DXVECTOR3(m_localTransform._21, m_localTransform._22, m_localTransform._23);

		D3DXVECTOR3 v3LocalY, v3LocalX;
		D3DXVec3TransformNormal(&v3LocalY, &m_realY, &NodeMatInv);     
		D3DXVec3TransformNormal(&v3LocalX, &m_realX, &NodeMatInv);   

		int kkk = 0;


		// m_bParentTransformChanged = false;
	}
	
}

// 用父坐标系下的矩阵计算世界坐标系下的矩阵
VOID CCamera::UpdateWorldMatrix()
{
	// if (m_pNode)
	{
		D3DXMATRIX parentMat = GetNodeWordMatrix();

		// 自身的世界左边系下的变换, 注意顺序不能反了
		D3DXMatrixMultiply(&m_worldTransform, &m_localTransform, &parentMat);  
		m_realPosition = D3DXVECTOR3(m_worldTransform._41, m_worldTransform._42, m_worldTransform._43);  
		D3DXVec3TransformCoord(&m_realLookat, &m_localLookat, &parentMat);     // at点的世界坐标
		m_realX = D3DXVECTOR3(m_worldTransform._11, m_worldTransform._12, m_worldTransform._13);
		m_realY = D3DXVECTOR3(m_worldTransform._21, m_worldTransform._22, m_worldTransform._23);
		//D3DXVec3Cross(&m_realX, &m_realY, &(m_realLookat - m_realPosition));
		//D3DXVec3Normalize(&m_realX, &m_realX);
		//D3DXVec3Cross(&m_realY, &(m_realLookat - m_realPosition), &m_realX);
		D3DXVec3Normalize(&m_realY, &m_realY);
		D3DXVec3Normalize(&m_realX, &m_realX);

		int kkk = 0;
		kkk++;
		// m_bParentTransformChanged = false;
	}
}

// 通过roll获取up和right值,looat保持不变
VOID CCamera::UpdateFromRoll(const float& fRoll)
{
	Rotate(&D3DXVECTOR3(0, 0, fRoll), VRS_LOCAL);

	//// vx
	//D3DXVECTOR3 vDir = m_localLookat - m_localPosition;
	//D3DXVECTOR3 vx = D3DXVECTOR3_X;
	//if (D3DXVec3Dot(&vDir, &D3DXVECTOR3_Y) != 1.0f)
	//{
	//	D3DXVec3Cross(&vx, &D3DXVECTOR3_Y, &vDir);
	//	D3DXVec3Normalize(&vx, &vx);
	//}

	//// 计算真正的vx方向
	//D3DXMATRIX matRoll;
	//D3DXMatrixRotationYawPitchRoll(&matRoll, 0, 0, m_fRoll);  // roll转换为矩阵
	//D3DXVec3TransformNormal(&m_localX, &vx, &matRoll);

	//// vy
	//D3DXVec3Cross(&m_localY, &vDir, &m_localX);
	//D3DXVec3Normalize(&m_localY, &m_localY);

	//// 更新矩阵
	//D3DXMatrixMultiply(&m_localTransform, &m_localTransform, &matRoll);

	//UpdateWorldMatrix();
}

// 获取节点的世界坐标变换矩阵
D3DXMATRIX CCamera::GetNodeWordMatrix()
{
	if (m_pNode)
	{
		return m_pNode->getMatrix(VTS_WORLD);
	}
	else
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		return mat;
	}
}