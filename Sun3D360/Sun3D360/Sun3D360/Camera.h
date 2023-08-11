//---------------------------------------------
//	Name:Camera.h
//	Introduce:The define of CCamera class 
//	Author:Yaoyoufang
//	Date:05-4-28
//	Version:0.1v
//---------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_
//---------------------------------------------
#include <math.h>
#include <gl/gl.h>
//---------------------------------------------
#define SQR(x) (x * x)
#define sqrtf(x) sqrt(x)
#define M_ITEM(m, x, y)  m[(x * 4) + y]
//---------------------------------------------
class CCamera
{
	//-----------------------------------------
	enum FrustumFace
	{
		ffRight = 0,	//右剪切面
		ffLeft,			//左剪切面
		ffBottom,		//下剪切面
		ffTop,			//上剪切面
		ffNear,			//近剪切面
		ffFar			//远剪切面
	};
	//-----------------------------------------
public:
	CCamera()
	{
	}
	~CCamera()
	{
	}
	//-----------------------------------------
public:
	//-----------------------------------------
	//	名称：CalculateViewFrustum
	//	简介：计算视锥
	//	参数：无
	//	返回：无
	//----------------------------------------
	void CalculateViewFrustum()
	{
		GLfloat projMtrx[16];	//projection矩阵
		GLfloat mvMtrx[16];		//modelview矩阵
		GLfloat clip[16];
		GLfloat Norm_to_1;

		glGetFloatv(GL_PROJECTION_MATRIX, projMtrx);
		glGetFloatv(GL_MODELVIEW_MATRIX, mvMtrx);

		Mul(clip, mvMtrx, projMtrx);

		/* Extract the numbers for the RIGHT plane */
		f_viewFrustum[ffRight][0] = clip[ 3] - clip[ 0];
		f_viewFrustum[ffRight][1] = clip[ 7] - clip[ 4];
		f_viewFrustum[ffRight][2] = clip[11] - clip[ 8];
		f_viewFrustum[ffRight][3] = clip[15] - clip[12];

		//normalize the plane
		Norm_to_1 = 1.0f / sqrtf(SQR(f_viewFrustum[ffRight][0])
								 + SQR(f_viewFrustum[ffRight][1])
								 + SQR( f_viewFrustum[ffRight][2]));
		f_viewFrustum[ffRight][0] *= Norm_to_1;
		f_viewFrustum[ffRight][1] *= Norm_to_1;
		f_viewFrustum[ffRight][2] *= Norm_to_1;
		f_viewFrustum[ffRight][3] *= Norm_to_1;

		/* Extract the numbers for the LEFT plane */
		f_viewFrustum[ffLeft][0] = clip[ 3] + clip[ 0];
		f_viewFrustum[ffLeft][1] = clip[ 7] + clip[ 4];
		f_viewFrustum[ffLeft][2] = clip[11] + clip[ 8];
		f_viewFrustum[ffLeft][3] = clip[15] + clip[12];

		//normalize the plane
		Norm_to_1 = 1.0f / sqrtf(SQR(f_viewFrustum[ffLeft][0])
								 + SQR(f_viewFrustum[ffLeft][1])
								 + SQR(f_viewFrustum[ffLeft][2]));
		f_viewFrustum[ffLeft][0] *= Norm_to_1;
		f_viewFrustum[ffLeft][1] *= Norm_to_1;
		f_viewFrustum[ffLeft][2] *= Norm_to_1;
		f_viewFrustum[ffLeft][3] *= Norm_to_1;

		/* Extract the BOTTOM plane */
		f_viewFrustum[ffBottom][0] = clip[ 3] + clip[ 1];
		f_viewFrustum[ffBottom][1] = clip[ 7] + clip[ 5];
		f_viewFrustum[ffBottom][2] = clip[11] + clip[ 9];
		f_viewFrustum[ffBottom][3] = clip[15] + clip[13];

		//normalize the plane
		Norm_to_1 = 1.0f / sqrtf(SQR(f_viewFrustum[ffBottom][0])
								 + SQR(f_viewFrustum[ffBottom][1])
								 + SQR(f_viewFrustum[ffBottom][2]));
		f_viewFrustum[ffBottom][0] *= Norm_to_1;
		f_viewFrustum[ffBottom][1] *= Norm_to_1;
		f_viewFrustum[ffBottom][2] *= Norm_to_1;
		f_viewFrustum[ffBottom][3] *= Norm_to_1;

		/* Extract the TOP plane */
		f_viewFrustum[ffTop][0] = clip[ 3] - clip[ 1];
		f_viewFrustum[ffTop][1] = clip[ 7] - clip[ 5];
		f_viewFrustum[ffTop][2] = clip[11] - clip[ 9];
		f_viewFrustum[ffTop][3] = clip[15] - clip[13];

		//normalize the plane
		Norm_to_1 = 1.0f / sqrtf(SQR(f_viewFrustum[ffTop][0])
								 + SQR(f_viewFrustum[ffTop][1])
								 + SQR(f_viewFrustum[ffTop][2]));
		f_viewFrustum[ffTop][0] *= Norm_to_1;
		f_viewFrustum[ffTop][1] *= Norm_to_1;
		f_viewFrustum[ffTop][2] *= Norm_to_1;
		f_viewFrustum[ffTop][3] *= Norm_to_1;

		/* Extract the FAR plane */
		f_viewFrustum[ffFar][0] = clip[ 3] - clip[ 2];
		f_viewFrustum[ffFar][1] = clip[ 7] - clip[ 6];
		f_viewFrustum[ffFar][2] = clip[11] - clip[10];
		f_viewFrustum[ffFar][3] = clip[15] - clip[14];

		//normalize the plane
		Norm_to_1 = 1.0f / sqrtf(SQR(f_viewFrustum[ffFar][0])
								 + SQR(f_viewFrustum[ffFar][1])
								 + SQR(f_viewFrustum[ffFar][2]));
		f_viewFrustum[ffFar][0] *= Norm_to_1;
		f_viewFrustum[ffFar][1] *= Norm_to_1;
		f_viewFrustum[ffFar][2] *= Norm_to_1;
		f_viewFrustum[ffFar][3] *= Norm_to_1;

		/* Extract the NEAR plane */
		f_viewFrustum[ffNear][0] = clip[ 3] + clip[ 2];
		f_viewFrustum[ffNear][1] = clip[ 7] + clip[ 6];
		f_viewFrustum[ffNear][2] = clip[11] + clip[10];
		f_viewFrustum[ffNear][3] = clip[15] + clip[14];

		//normalize the plane
		Norm_to_1 = 1.0f / sqrtf(SQR(f_viewFrustum[ffNear][0])
								 + SQR(f_viewFrustum[ffNear][1])
								 + SQR(f_viewFrustum[ffNear][2]));
		f_viewFrustum[ffNear][0] *= Norm_to_1;
		f_viewFrustum[ffNear][1] *= Norm_to_1;
		f_viewFrustum[ffNear][2] *= Norm_to_1;
		f_viewFrustum[ffNear][3] *= Norm_to_1;
	}
	//-----------------------------------------
	//	名称：CuboidFrustumTest
	//	简介：测试长方体是否在视锥中
	//	参数：minX, maxX, minY, maxY, minZ, maxZ:i:长方体的范围
	//	返回：0:长方体不在视锥内
	//	      1:长方体完全在视锥内
	//	      2:长方体部分在视锥内
	//----------------------------------------
	GLint CuboidFrustumTest(GLfloat minX, GLfloat maxX,
							GLfloat minY, GLfloat maxY,
							GLfloat minZ, GLfloat maxZ)
	{
		static GLint s_ret = 1;
		static bool s_Infront[8];
		for(GLint i = ffRight;i < ffFar;++i)
		{
			s_Infront[0] = (f_viewFrustum[i][0] * minX
							+ f_viewFrustum[i][1] * maxY
							+ f_viewFrustum[i][2] * minZ
							+ f_viewFrustum[i][3] > 0);
			s_Infront[1] = (f_viewFrustum[i][0] * maxX
							+ f_viewFrustum[i][1] * maxY
							+ f_viewFrustum[i][2] * minZ
							+ f_viewFrustum[i][3] > 0);
			s_Infront[2] = (f_viewFrustum[i][0] * minX
							+ f_viewFrustum[i][1] * minY
							+ f_viewFrustum[i][2] * minZ
							+ f_viewFrustum[i][3] > 0);
			s_Infront[3] = (f_viewFrustum[i][0] * maxX
							+ f_viewFrustum[i][1] * minY
							+ f_viewFrustum[i][2] * minZ
							+ f_viewFrustum[i][3] > 0);
			s_Infront[4] = (f_viewFrustum[i][0] * minX
							+ f_viewFrustum[i][1] * maxY
							+ f_viewFrustum[i][2] * maxZ
							+ f_viewFrustum[i][3] > 0);
			s_Infront[5] = (f_viewFrustum[i][0] * maxX
							+ f_viewFrustum[i][1] * maxY
							+ f_viewFrustum[i][2] * maxZ
							+ f_viewFrustum[i][3] > 0);
			s_Infront[6] = (f_viewFrustum[i][0] * minX
							+ f_viewFrustum[i][1] * minY
							+ f_viewFrustum[i][2] * maxZ
							+ f_viewFrustum[i][3] > 0);
			s_Infront[7] = (f_viewFrustum[i][0] * maxX
							+ f_viewFrustum[i][1] * minY
							+ f_viewFrustum[i][2] * maxZ
							+ f_viewFrustum[i][3] > 0);

			if(!(s_Infront[0]
				 || s_Infront[1]
				 || s_Infront[2]
				 || s_Infront[3]
				 || s_Infront[4]
				 || s_Infront[5]
				 || s_Infront[6]
				 || s_Infront[7]))
			{
				return 0;
			}
			if(!(s_Infront[0]
				 && s_Infront[1]
				 && s_Infront[2]
				 && s_Infront[3]
				 && s_Infront[4]
				 && s_Infront[5]
				 && s_Infront[6]
				 && s_Infront[7]))
			{
				s_ret = 2;
			}
		}
		
		return s_ret;
	}
	//-----------------------------------------
private:
	//-----------------------------------------
	//	名称：Mul
	//	简介：矩阵乘矩阵
	//	参数：ret:i:结果矩阵
	//	      lv:i:左矩阵
	//	      rv:i:右矩阵
	//	返回：无
	//	注意：ret不能和lv或者rv重叠
	//-----------------------------------------
	void Mul(GLfloat* ret, GLfloat* lv, GLfloat* rv)
	{
		for(GLint i = 0;i < 4;++i)
		{
			for(GLint j = 0;j < 4;++j)
			{
				M_ITEM(ret, i, j) = M_ITEM(lv, i, 0) * M_ITEM(rv, 0, j)
									+ M_ITEM(lv, i, 1) * M_ITEM(rv, 1, j)
									+ M_ITEM(lv, i, 2) * M_ITEM(rv, 2, j)
									+ M_ITEM(lv, i, 3) * M_ITEM(rv, 3, j);

			}
		}
	}
	//-----------------------------------------
private:
	//-----------------------------------------
	//	名称：f_viewFrustum
	//	简介：6个视锥平面
	//----------------------------------------
	GLfloat f_viewFrustum[6][4];
	//-----------------------------------------
};
//---------------------------------------------
#endif	//_CAMERA_H_
//---------------------------------------------