//------------------------------------------------
//	BoxFace.cpp
//------------------------------------------------
#include "stdafx.h"
#include "BoxFace.h"
#include <math.h>
//------------------------------------------------
const float			k_TexCoord_S = 0.002f;
const float			k_TexCoord_E = 0.998f;
//------------------------------------------------
CBoxFace::CBoxFace()
:m_iID(0),m_iSmallCnt(1),m_pQuads(NULL),m_uTex(NULL),
m_fMinX(0),m_fMaxX(0),m_fMinY(0),m_fMaxY(0),m_fMinZ(0),m_fMaxZ(0)
{
	m_stuVisible.iTotalVisible = 1;
	m_stuVisible.iChildVisible = NULL;
}
//------------------------------------------------
CBoxFace::~CBoxFace()
{
	if(m_pQuads)
	{
		delete []m_pQuads;
		m_pQuads = NULL;
	}
	if(m_stuVisible.iChildVisible)
	{
		delete []m_stuVisible.iChildVisible;
		m_stuVisible.iChildVisible = NULL;
	}
	glDeleteTextures(m_iSmallCnt, m_uTex);	
}
//------------------------------------------------
bool CBoxFace::Create(int iFace, int iDivType, int iWidth)
{
	if(iFace < 0 || iFace >= 6) return false;
	if(iWidth <= 0) return false;
	if(iDivType < DIS1_1 || iDivType > DIS8_8) return false;

	switch(iDivType)
	{
	case DIS1_1:
		m_iSmallCnt = 1;
		break;
	case DIS2_2:
		m_iSmallCnt = 2*2;
		break;
	case DIS4_4:
		m_iSmallCnt = 4*4;
		break;
	case DIS8_8:
		m_iSmallCnt = 8*8;
		break;
	default:
		m_iSmallCnt = 1;
		break;
	}

	//To create the quads
	m_iID = iFace;
	switch(m_iID)
	{
	case 0:
		CalucFaceCorrds_F(iWidth);
		break;
	case 1:
		CalucFaceCorrds_B(iWidth);
		break;
	case 2:
		CalucFaceCorrds_L(iWidth);
		break;
	case 3:
		CalucFaceCorrds_R(iWidth);
		break;
	case 4:
		CalucFaceCorrds_T(iWidth);
		break;
	case 5:
		CalucFaceCorrds_U(iWidth);
		break;
	default:
		break;
	}

	//Create the texture
	m_uTex = new GLuint[m_iSmallCnt];

	//The visible
	if(m_stuVisible.iChildVisible) delete []m_stuVisible.iChildVisible;
	m_stuVisible.iChildVisible = new int[m_iSmallCnt];
	memset(m_stuVisible.iChildVisible, 1, sizeof(int)*m_iSmallCnt);

	return true;
}
//------------------------------------------------
void CBoxFace::CalucFaceCorrds_F(int iWinWidth)
{
	int iCnt = (int)sqrt(m_iSmallCnt);
	float fTexCorrdW = k_TexCoord_E - k_TexCoord_S;
	if(m_pQuads) delete []m_pQuads;
	m_pQuads = new Quad3D[m_iSmallCnt];
	for(int i = 0;i < m_iSmallCnt; i++)
	{
		m_pQuads[i].fV[0][0] = iWinWidth * (i%iCnt)/(float)iCnt - iWinWidth/2.0f;
		m_pQuads[i].fV[0][1] = iWinWidth * ((m_iSmallCnt-i-1)/iCnt)/(float)iCnt - iWinWidth/2.0f;
		m_pQuads[i].fV[0][2] = - iWinWidth/2.0f;
		//
		m_pQuads[i].fV[1][0] = m_pQuads[i].fV[0][0] + iWinWidth/(float)iCnt;
		m_pQuads[i].fV[1][1] = m_pQuads[i].fV[0][1];
		m_pQuads[i].fV[1][2] = - iWinWidth/2.0f;
		//
		m_pQuads[i].fV[2][0] = m_pQuads[i].fV[1][0];
		m_pQuads[i].fV[2][1] = m_pQuads[i].fV[0][1] + iWinWidth/(float)iCnt;
		m_pQuads[i].fV[2][2] = - iWinWidth/2.0f;
		//
		m_pQuads[i].fV[3][0] = m_pQuads[i].fV[0][0];
		m_pQuads[i].fV[3][1] = m_pQuads[i].fV[2][1];
		m_pQuads[i].fV[3][2] = - iWinWidth/2.0f;


		m_pQuads[i].fT[0][0] = k_TexCoord_S;
		m_pQuads[i].fT[0][1] = k_TexCoord_S;
		m_pQuads[i].fT[1][0] = k_TexCoord_E;
		m_pQuads[i].fT[1][1] = k_TexCoord_S;
		m_pQuads[i].fT[2][0] = k_TexCoord_E;
		m_pQuads[i].fT[2][1] = k_TexCoord_E;
		m_pQuads[i].fT[3][0] = k_TexCoord_S;
		m_pQuads[i].fT[3][1] = k_TexCoord_E;
	}

	m_fMinX = -iWinWidth/2.0f;
	m_fMaxX = iWinWidth/2.0f;
	m_fMinY = -iWinWidth/2.0f;
	m_fMaxY = iWinWidth/2.0f;
	m_fMinZ = -iWinWidth/2.0f;
	m_fMaxZ = -iWinWidth/2.0f;
}
//------------------------------------------------
void CBoxFace::CalucFaceCorrds_B(int iWinWidth)
{	
	int iCnt = (int)sqrt(m_iSmallCnt);
	float fTexCorrdW = k_TexCoord_E - k_TexCoord_S;
	if(m_pQuads) delete []m_pQuads;
	m_pQuads = new Quad3D[m_iSmallCnt];
	for(int i = 0;i < m_iSmallCnt; i++)
	{
		m_pQuads[i].fV[0][0] = iWinWidth/2.0f - iWinWidth * (i%iCnt)/(float)iCnt;
		m_pQuads[i].fV[0][1] = iWinWidth * ((m_iSmallCnt-i-1)/iCnt)/(float)iCnt - iWinWidth/2.0f;
		m_pQuads[i].fV[0][2] = iWinWidth/2.0f;
		//
		m_pQuads[i].fV[1][0] = m_pQuads[i].fV[0][0] - iWinWidth/(float)iCnt;
		m_pQuads[i].fV[1][1] = m_pQuads[i].fV[0][1];
		m_pQuads[i].fV[1][2] = iWinWidth/2.0f;
		//
		m_pQuads[i].fV[2][0] = m_pQuads[i].fV[1][0];
		m_pQuads[i].fV[2][1] = m_pQuads[i].fV[0][1] + iWinWidth/(float)iCnt;
		m_pQuads[i].fV[2][2] = iWinWidth/2.0f;
		//
		m_pQuads[i].fV[3][0] = m_pQuads[i].fV[0][0];
		m_pQuads[i].fV[3][1] = m_pQuads[i].fV[2][1];
		m_pQuads[i].fV[3][2] = iWinWidth/2.0f;

		
		m_pQuads[i].fT[0][0] = k_TexCoord_S;
		m_pQuads[i].fT[0][1] = k_TexCoord_S;
		m_pQuads[i].fT[1][0] = k_TexCoord_E;
		m_pQuads[i].fT[1][1] = k_TexCoord_S;
		m_pQuads[i].fT[2][0] = k_TexCoord_E;
		m_pQuads[i].fT[2][1] = k_TexCoord_E;
		m_pQuads[i].fT[3][0] = k_TexCoord_S;
		m_pQuads[i].fT[3][1] = k_TexCoord_E;
	}
	
	m_fMinX = -iWinWidth/2.0f;
	m_fMaxX = iWinWidth/2.0f;
	m_fMinY = -iWinWidth/2.0f;
	m_fMaxY = iWinWidth/2.0f;
	m_fMinZ = iWinWidth/2.0f;
	m_fMaxZ = iWinWidth/2.0f;
}
//------------------------------------------------
void CBoxFace::CalucFaceCorrds_L(int iWinWidth)
{
	int iCnt = (int)sqrt(m_iSmallCnt);
	float fTexCorrdW = k_TexCoord_E - k_TexCoord_S;
	if(m_pQuads) delete []m_pQuads;
	m_pQuads = new Quad3D[m_iSmallCnt];
	for(int i = 0;i < m_iSmallCnt; i++)
	{
		m_pQuads[i].fV[0][0] = -iWinWidth/2.0f;
		m_pQuads[i].fV[0][1] = iWinWidth * ((m_iSmallCnt-i-1)/iCnt)/(float)iCnt - iWinWidth/2.0f;
		m_pQuads[i].fV[0][2] = iWinWidth/2.0f - iWinWidth * (i%iCnt)/(float)iCnt;
		//
		m_pQuads[i].fV[1][0] = -iWinWidth/2.0f;
		m_pQuads[i].fV[1][1] = m_pQuads[i].fV[0][1];
		m_pQuads[i].fV[1][2] = m_pQuads[i].fV[0][2] - iWinWidth/(float)iCnt;
		//
		m_pQuads[i].fV[2][0] = -iWinWidth/2.0f;
		m_pQuads[i].fV[2][1] = m_pQuads[i].fV[0][1] + iWinWidth/(float)iCnt;
		m_pQuads[i].fV[2][2] = m_pQuads[i].fV[1][2];
		//
		m_pQuads[i].fV[3][0] = -iWinWidth/2.0f;
		m_pQuads[i].fV[3][1] = m_pQuads[i].fV[2][1];
		m_pQuads[i].fV[3][2] = m_pQuads[i].fV[0][2];
		

		m_pQuads[i].fT[0][0] = k_TexCoord_S;
		m_pQuads[i].fT[0][1] = k_TexCoord_S;
		m_pQuads[i].fT[1][0] = k_TexCoord_E;
		m_pQuads[i].fT[1][1] = k_TexCoord_S;
		m_pQuads[i].fT[2][0] = k_TexCoord_E;
		m_pQuads[i].fT[2][1] = k_TexCoord_E;
		m_pQuads[i].fT[3][0] = k_TexCoord_S;
		m_pQuads[i].fT[3][1] = k_TexCoord_E;
	}

	m_fMinX = -iWinWidth/2.0f;
	m_fMaxX = -iWinWidth/2.0f;
	m_fMinY = -iWinWidth/2.0f;
	m_fMaxY = iWinWidth/2.0f;
	m_fMinZ = -iWinWidth/2.0f;
	m_fMaxZ = iWinWidth/2.0f;
}
//------------------------------------------------
void CBoxFace::CalucFaceCorrds_R(int iWinWidth)
{
	int iCnt = (int)sqrt(m_iSmallCnt);
	float fTexCorrdW = k_TexCoord_E - k_TexCoord_S;
	if(m_pQuads) delete []m_pQuads;
	m_pQuads = new Quad3D[m_iSmallCnt];
	for(int i = 0;i < m_iSmallCnt; i++)
	{
		m_pQuads[i].fV[0][0] = iWinWidth/2.0f;
		m_pQuads[i].fV[0][1] = iWinWidth * ((m_iSmallCnt-i-1)/iCnt)/(float)iCnt - iWinWidth/2.0f;
		m_pQuads[i].fV[0][2] = iWinWidth * (i%iCnt)/(float)iCnt - iWinWidth/2.0f;
		//
		m_pQuads[i].fV[1][0] = iWinWidth/2.0f;
		m_pQuads[i].fV[1][1] = m_pQuads[i].fV[0][1];
		m_pQuads[i].fV[1][2] = m_pQuads[i].fV[0][2] + iWinWidth/(float)iCnt;
		//
		m_pQuads[i].fV[2][0] = iWinWidth/2.0f;
		m_pQuads[i].fV[2][1] = m_pQuads[i].fV[0][1] + iWinWidth/(float)iCnt;
		m_pQuads[i].fV[2][2] = m_pQuads[i].fV[1][2];
		//
		m_pQuads[i].fV[3][0] = iWinWidth/2.0f;
		m_pQuads[i].fV[3][1] = m_pQuads[i].fV[2][1];
		m_pQuads[i].fV[3][2] = m_pQuads[i].fV[0][2];
		

		m_pQuads[i].fT[0][0] = k_TexCoord_S;
		m_pQuads[i].fT[0][1] = k_TexCoord_S;
		m_pQuads[i].fT[1][0] = k_TexCoord_E;
		m_pQuads[i].fT[1][1] = k_TexCoord_S;
		m_pQuads[i].fT[2][0] = k_TexCoord_E;
		m_pQuads[i].fT[2][1] = k_TexCoord_E;
		m_pQuads[i].fT[3][0] = k_TexCoord_S;
		m_pQuads[i].fT[3][1] = k_TexCoord_E;
	}
		
	m_fMinX = iWinWidth/2.0f;
	m_fMaxX = iWinWidth/2.0f;
	m_fMinY = -iWinWidth/2.0f;
	m_fMaxY = iWinWidth/2.0f;
	m_fMinZ = -iWinWidth/2.0f;
	m_fMaxZ = iWinWidth/2.0f;
}
//------------------------------------------------
void CBoxFace::CalucFaceCorrds_T(int iWinWidth)
{
	int iCnt = (int)sqrt(m_iSmallCnt);
	float fTexCorrdW = k_TexCoord_E - k_TexCoord_S;
	if(m_pQuads) delete []m_pQuads;
	m_pQuads = new Quad3D[m_iSmallCnt];
	for(int i = 0;i < m_iSmallCnt; i++)
	{
		m_pQuads[i].fV[0][0] = iWinWidth * (i%iCnt)/(float)iCnt - iWinWidth/2.0f;
		m_pQuads[i].fV[0][1] = iWinWidth/2.0f;
		m_pQuads[i].fV[0][2] = iWinWidth * ((m_iSmallCnt-i-1)/iCnt)/(float)iCnt - iWinWidth/2.0f;
		//
		m_pQuads[i].fV[1][0] = m_pQuads[i].fV[0][0] + iWinWidth/(float)iCnt;
		m_pQuads[i].fV[1][1] = iWinWidth/2.0f;
		m_pQuads[i].fV[1][2] = m_pQuads[i].fV[0][2];
		//
		m_pQuads[i].fV[2][0] = m_pQuads[i].fV[1][0];
		m_pQuads[i].fV[2][1] = iWinWidth/2.0f;
		m_pQuads[i].fV[2][2] = m_pQuads[i].fV[1][2] + iWinWidth/(float)iCnt;
		//
		m_pQuads[i].fV[3][0] = m_pQuads[i].fV[0][0];
		m_pQuads[i].fV[3][1] = iWinWidth/2.0f;
		m_pQuads[i].fV[3][2] = m_pQuads[i].fV[2][2];


		m_pQuads[i].fT[0][0] = k_TexCoord_S;
		m_pQuads[i].fT[0][1] = k_TexCoord_S;
		m_pQuads[i].fT[1][0] = k_TexCoord_E;
		m_pQuads[i].fT[1][1] = k_TexCoord_S;
		m_pQuads[i].fT[2][0] = k_TexCoord_E;
		m_pQuads[i].fT[2][1] = k_TexCoord_E;
		m_pQuads[i].fT[3][0] = k_TexCoord_S;
		m_pQuads[i].fT[3][1] = k_TexCoord_E;
	}

	m_fMinX = -iWinWidth/2.0f;
	m_fMaxX = iWinWidth/2.0f;
	m_fMinY = iWinWidth/2.0f;
	m_fMaxY = iWinWidth/2.0f;
	m_fMinZ = -iWinWidth/2.0f;
	m_fMaxZ = iWinWidth/2.0f;
}
//------------------------------------------------
void CBoxFace::CalucFaceCorrds_U(int iWinWidth)
{
	int iCnt = (int)sqrt(m_iSmallCnt);
	float fTexCorrdW = k_TexCoord_E - k_TexCoord_S;
	if(m_pQuads) delete []m_pQuads;
	m_pQuads = new Quad3D[m_iSmallCnt];
	for(int i = 0;i < m_iSmallCnt; i++)
	{
		m_pQuads[i].fV[0][0] = iWinWidth * (i%iCnt)/(float)iCnt - iWinWidth/2.0f;
		m_pQuads[i].fV[0][1] = -iWinWidth/2.0f;
		m_pQuads[i].fV[0][2] = iWinWidth/2.0f - iWinWidth * ((m_iSmallCnt-i-1)/iCnt)/(float)iCnt;
		//
		m_pQuads[i].fV[1][0] = m_pQuads[i].fV[0][0] + iWinWidth/(float)iCnt;
		m_pQuads[i].fV[1][1] = -iWinWidth/2.0f;
		m_pQuads[i].fV[1][2] = m_pQuads[i].fV[0][2];
		//
		m_pQuads[i].fV[2][0] = m_pQuads[i].fV[1][0];
		m_pQuads[i].fV[2][1] = -iWinWidth/2.0f;
		m_pQuads[i].fV[2][2] = m_pQuads[i].fV[1][2] - iWinWidth/(float)iCnt;
		//
		m_pQuads[i].fV[3][0] = m_pQuads[i].fV[0][0];
		m_pQuads[i].fV[3][1] = -iWinWidth/2.0f;
		m_pQuads[i].fV[3][2] = m_pQuads[i].fV[2][2];


		m_pQuads[i].fT[0][0] = k_TexCoord_S;
		m_pQuads[i].fT[0][1] = k_TexCoord_S;
		m_pQuads[i].fT[1][0] = k_TexCoord_E;
		m_pQuads[i].fT[1][1] = k_TexCoord_S;
		m_pQuads[i].fT[2][0] = k_TexCoord_E;
		m_pQuads[i].fT[2][1] = k_TexCoord_E;
		m_pQuads[i].fT[3][0] = k_TexCoord_S;
		m_pQuads[i].fT[3][1] = k_TexCoord_E;
	}

	m_fMinX = -iWinWidth/2.0f;
	m_fMaxX = iWinWidth/2.0f;
	m_fMinY = -iWinWidth/2.0f;
	m_fMaxY = -iWinWidth/2.0f;
	m_fMinZ = -iWinWidth/2.0f;
	m_fMaxZ = iWinWidth/2.0f;
}
//------------------------------------------------
void CBoxFace::GetChildMinMax(int iID, float &fMinX, float &fMaxX, 
	float &fMinY, float &fMaxY, float &fMinZ, float &fMaxZ)
{	
	switch(m_iID)
	{
	case 0:
		fMinX = m_pQuads[iID].fV[0][0];
		fMaxX = m_pQuads[iID].fV[2][0];
		fMinY = m_pQuads[iID].fV[0][1];
		fMaxY = m_pQuads[iID].fV[2][1];
		fMinZ = m_pQuads[iID].fV[0][2];
		fMaxZ = m_pQuads[iID].fV[0][2];
		break;
	case 1:
		fMinX = m_pQuads[iID].fV[2][0];
		fMaxX = m_pQuads[iID].fV[0][0];
		fMinY = m_pQuads[iID].fV[0][1];
		fMaxY = m_pQuads[iID].fV[2][1];
		fMinZ = m_pQuads[iID].fV[0][2];
		fMaxZ = m_pQuads[iID].fV[0][2];
		break;
	case 2:
		fMinX = m_pQuads[iID].fV[0][0];
		fMaxX = m_pQuads[iID].fV[0][0];
		fMinY = m_pQuads[iID].fV[0][1];
		fMaxY = m_pQuads[iID].fV[2][1];
		fMinZ = m_pQuads[iID].fV[2][2];
		fMaxZ = m_pQuads[iID].fV[0][2];
		break;
	case 3:
		fMinX = m_pQuads[iID].fV[0][0];
		fMaxX = m_pQuads[iID].fV[0][0];
		fMinY = m_pQuads[iID].fV[0][1];
		fMaxY = m_pQuads[iID].fV[2][1];
		fMinZ = m_pQuads[iID].fV[0][2];
		fMaxZ = m_pQuads[iID].fV[2][2];
		break;
	case 4:
		fMinX = m_pQuads[iID].fV[0][0];
		fMaxX = m_pQuads[iID].fV[2][0];
		fMinY = m_pQuads[iID].fV[0][1];
		fMaxY = m_pQuads[iID].fV[0][1];
		fMinZ = m_pQuads[iID].fV[0][2];
		fMaxZ = m_pQuads[iID].fV[2][2];
		break;
	case 5:
		fMinX = m_pQuads[iID].fV[0][0];
		fMaxX = m_pQuads[iID].fV[2][0];
		fMinY = m_pQuads[iID].fV[0][1];
		fMaxY = m_pQuads[iID].fV[0][1];
		fMinZ = m_pQuads[iID].fV[2][2];
		fMaxZ = m_pQuads[iID].fV[0][2];
		break;
	}
}
//------------------------------------------------
void CBoxFace::Draw()
{
	for(int iCnt = 0;iCnt < m_iSmallCnt;iCnt++)
	{
		glBindTexture(GL_TEXTURE_2D, m_uTex[iCnt]);  
		glBegin(GL_QUADS);
			for(int iPnt = 0;iPnt < 4;iPnt++)
			{
				glTexCoord2fv(m_pQuads[iCnt].fT[iPnt]);	glVertex3fv(m_pQuads[iCnt].fV[iPnt]);
			}
		glEnd();
	}
}
//------------------------------------------------

