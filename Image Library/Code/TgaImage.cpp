//------------------------------------------------
//	FileName:	TgaImage.cpp
//	Introduce:	The CTgaImage class .cpp file
//	Creator:	Youfang Yao
//	Date:		05/25/06
//	Version:	1.0v
//------------------------------------------------
#include "stdafx.h"
#include "TgaImage.h"
#include <sstream>
#include <vector>
//------------------------------------------------
//	Construction
//------------------------------------------------
CTgaImage::CTgaImage()
{
}
//------------------------------------------------
//	Distruction
//------------------------------------------------
CTgaImage::~CTgaImage()
{
}
//------------------------------------------------	
//Load the Tga image data from a tga file
//------------------------------------------------	
bool CTgaImage::LoadDataFromFile(const char* pszFile)
{
	if(pszFile == NULL)
		return false;	

	//Open file
	ifstream inf(pszFile, ios::binary);
	if(0 == inf.is_open()) 
		return false;

	bool bResult = LoadDataFromStream(inf);
	inf.close();

	return bResult;
}
//------------------------------------------------	
//Load the Tga image data from a tga image file data buffer
//------------------------------------------------	
bool CTgaImage::LoadDataFromBuff(unsigned char* pImageBuff, int iLen)
{
	if(pImageBuff == NULL || iLen <= 0)
		return false;

	vector<char> buf(iLen);
	memcpy(&buf[0], pImageBuff, iLen);
    string str(buf.begin(), buf.end());
    stringstream ins(str);
	str.erase(0, str.length());
	
	bool bResult = LoadDataFromStream(ins);
	ins.clear();

	return bResult;
}
//------------------------------------------------	
//Save the image data in this class to a tga file
//------------------------------------------------	
bool CTgaImage::SaveToFile(const char* pszFile)
{
	if(pszFile == NULL)
		return false;	

	//Have not set the image information
	if( m_pImageData == NULL || 
		m_iImageDataSize <= 0 ||
		m_iImageWidth <= 0 ||
		m_iImageHeight <= 0 ||
		(m_iBitsPerPixel != 24 && m_iBitsPerPixel != 32))
		return false;

	//Open file
	ofstream outf(pszFile, ios::binary);
	if(0 == outf.is_open()) 
		return false;

	//Image information string
	const char szImageInfo[] = "Saved by Image Library!";

	//Construct the file header
	TgaHeader header;
	memset(&header, 0, sizeof(header));
	header.ubImageInfoByteCnt = sizeof(szImageInfo);
	header.ubImageType = 2;
	header.ubImageInfo[4] = m_iImageWidth % 256;
	header.ubImageInfo[5] = m_iImageWidth / 256;
	header.ubImageInfo[6] = m_iImageHeight % 256;
	header.ubImageInfo[7] = m_iImageHeight / 256;
	header.ubImageInfo[8] = m_iBitsPerPixel;
	outf.write((const char*)&header, sizeof(header));

	outf.write((const char*)szImageInfo, sizeof(szImageInfo));

	outf.write((const char*)m_pImageData, m_iImageDataSize);

	outf.close();

	return true;
}
//------------------------------------------------	



//------------------------------------------------
//Load the Tga image data from a stream
//------------------------------------------------
bool CTgaImage::LoadDataFromStream(istream &is)
{
	//Load the file header
	TgaHeader header;
	memset(&header, 0, sizeof(header));
	is.read((char*)&header, sizeof(header));
	if(header.ubImageType != 2 && header.ubImageType != 10)
		return false;

	//Get image size
	m_iImageWidth = (header.ubImageInfo[5]<<8) + header.ubImageInfo[4];
	m_iImageHeight = (header.ubImageInfo[7]<<8) + header.ubImageInfo[6];
	m_iBitsPerPixel = header.ubImageInfo[8];
	if(	m_iImageWidth <=0 || m_iImageHeight <= 0 ||
		(m_iBitsPerPixel != 24 && m_iBitsPerPixel != 32))
		return false;
	m_iImageDataSize = m_iImageWidth * m_iImageHeight * m_iBitsPerPixel/8;

	//Load the image information
	char info[255] = {'\0'};
	is.read(info, header.ubImageInfoByteCnt);

	//Load the image data
	bool bNeedTurnOver = ((header.ubImageInfo[9] >> 5) & 0x1) == 1 ? true : false;
	if(header.ubImageType == 2)
		return LoadUnCompareData(is, bNeedTurnOver);
	else if(header.ubImageType == 10)
		return LoadCompareData(is, bNeedTurnOver);

	return false;
}
//------------------------------------------------	
//Load the uncompare tga image data from file
//------------------------------------------------	
bool CTgaImage::LoadUnCompareData(istream &is, bool bNeedTurnOver)
{
	//Load image data 
	if(m_pImageData) delete []m_pImageData;
	m_pImageData = new unsigned char[m_iImageDataSize];
	is.read((char*)m_pImageData, m_iImageDataSize);

	//Do image data row arrange
	DealImageRowPad();

	//Exchange the top and bottom of the image data
	if(bNeedTurnOver)
		TurnOverImage();

	return true;
}
//------------------------------------------------	
//Load the compare tga image data from file
//------------------------------------------------	
bool CTgaImage::LoadCompareData(istream &is, bool bNeedTurnOver)
{
	if(m_pImageData) delete []m_pImageData;
	m_pImageData = new unsigned char[m_iImageDataSize];

	int iBytePerPixel = m_iBitsPerPixel/8;
	int iPixelCnt = m_iImageWidth * m_iImageHeight;
	int iCurPixel = 0;
	unsigned char ucColorBuffer[4] = {'\0'};

	do{
		BYTE chunkheader = 0;
		is.read((char*)&chunkheader, sizeof(BYTE));

		//run length data
		if(chunkheader & 0x80)
		{
			int iCnt = (chunkheader & 0x7f) + 1;
			is.read((char*)ucColorBuffer, iBytePerPixel);
		
			for(int i = 0; i < iCnt; i++)
			{
				m_pImageData[iCurPixel*iBytePerPixel + 0] = ucColorBuffer[0];
				m_pImageData[iCurPixel*iBytePerPixel + 1] = ucColorBuffer[1];
				m_pImageData[iCurPixel*iBytePerPixel + 2] = ucColorBuffer[2];
				if(m_iBitsPerPixel == 32)
				{
					m_pImageData[iCurPixel*iBytePerPixel + 3] = ucColorBuffer[3];
				}

				iCurPixel++;

				if(iCurPixel > iPixelCnt) 
					return false;
			}

		}
		//no processed data
		else
		{
			int iCnt = chunkheader + 1;
			for(int i = 0; i < iCnt; i++)
			{				
				is.read((char*)ucColorBuffer, iBytePerPixel);

				m_pImageData[iCurPixel*iBytePerPixel + 0] = ucColorBuffer[0];
				m_pImageData[iCurPixel*iBytePerPixel + 1] = ucColorBuffer[1];
				m_pImageData[iCurPixel*iBytePerPixel + 2] = ucColorBuffer[2];
				if(m_iBitsPerPixel == 32)
				{
					m_pImageData[iCurPixel*iBytePerPixel + 3] = ucColorBuffer[3];
				}

				iCurPixel++;
				
				if(iCurPixel > iPixelCnt) 
					return false;
			}
		}

	}while(iCurPixel < iPixelCnt);

	//Do image data row arrange
	DealImageRowPad();

	//Exchange the top and bottom of the image data
	if(bNeedTurnOver)
		TurnOverImage();

	return true;
}
//------------------------------------------------	