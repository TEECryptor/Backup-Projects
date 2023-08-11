//------------------------------------------------
//	BoxFace.h
//------------------------------------------------
#ifndef _BOXFACE_H_
#define _BOXFACE_H_
//------------------------------------------------
#include <gl/gl.h>
#include <gl/glu.h>
//------------------------------------------------
//	How division the big image to small images
//------------------------------------------------
enum
{
	DIS1_1 = 0,				//Do not to divisin the big image
	DIS2_2,					//Division to 2x2 small images
	DIS4_4,					//Division to 4x4 small images
	DIS8_8,					//Division to 8x8 small images
	DIS_CNT					//The count of the division type
};
//------------------------------------------------
typedef struct tagQuad3D
{
	float			fV[4][3];			//The 4 vertex of the quad
	float			fT[4][2];			//The texture coord 
}Quad3D, *LPQUAD3D;
//------------------------------------------------
typedef struct tagFaceVisible
{
	int				iTotalVisible;
	int*			iChildVisible;
}FaceVisible, *LPFACEVISIBLE;
//------------------------------------------------
class CBoxFace
{
public:
	//--------------------------------------------
	CBoxFace();
	~CBoxFace();
	//--------------------------------------------
public:
	//--------------------------------------------
	bool		Create(int iFace, int iDivType, int iWidth);
	void		GetChildMinMax(int iID, float &fMinX, float &fMaxX, float &fMinY, float &fMaxY, float &fMinZ, float &fMaxZ);
	void		Draw();
	//--------------------------------------------
private:
	//--------------------------------------------
	void		CalucFaceCorrds_F(int iWinWidth);
	void		CalucFaceCorrds_B(int iWinWidth);
	void		CalucFaceCorrds_L(int iWinWidth);
	void		CalucFaceCorrds_R(int iWinWidth);
	void		CalucFaceCorrds_T(int iWinWidth);
	void		CalucFaceCorrds_U(int iWinWidth);
	//--------------------------------------------
public:
	//--------------------------------------------
	int			m_iID;
	int			m_iSmallCnt;
	LPQUAD3D	m_pQuads;
	GLuint*		m_uTex;
	//--------------------------------------------
	float		m_fMinX;
	float		m_fMaxX;
	float		m_fMinY;
	float		m_fMaxY;
	float		m_fMinZ;
	float		m_fMaxZ;
	FaceVisible	m_stuVisible;
	//--------------------------------------------
};
//------------------------------------------------
#endif	//_BOXFACE_H_
//------------------------------------------------