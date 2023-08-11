//------------------------------------------------
//	FileName:	BmpImage.cpp
//	Introduce:	The CBmpImage class .cpp file
//	Creator:	Youfang Yao
//	Date:		05/24/06
//	Version:	1.0v
//------------------------------------------------
#include "stdafx.h"
#include "BmpImage.h"
//------------------------------------------------
//	Construction
//------------------------------------------------
CBmpImage::CBmpImage()
{
}
//------------------------------------------------
//	Distruction
//------------------------------------------------
CBmpImage::~CBmpImage()
{
}
//------------------------------------------------
//Load the BMP image data from a bitmap file
//------------------------------------------------
bool CBmpImage::LoadDataFromFile(const char* pszFile)
{
	if(pszFile == NULL)
		return false;

	//Open file
	ifstream inf(pszFile, ios::binary);
	if(0 == inf.is_open()) 
		return false;

	//Load the file header
	BITMAPFILEHEADER header;
	memset(&header, 0, sizeof(header));
	inf.read((char*)&header, sizeof(header));
	if(header.bfType != 0x4D42)
		return false;

	//Load the image information header
	BITMAPINFOHEADER infoheader;
	memset(&infoheader, 0, sizeof(infoheader));
	inf.read((char*)&infoheader, sizeof(infoheader));
	m_iImageWidth = infoheader.biWidth;
	m_iImageHeight = infoheader.biHeight;
	m_iBitsPerPixel = infoheader.biBitCount;

	//Calculate the image data size
	int iLineByteCnt = (((m_iImageWidth*m_iBitsPerPixel) + 31) >> 5) << 2;
	m_iImageDataSize = iLineByteCnt * m_iImageHeight;
	
	//Load the image data to buffer
	if(m_pImageData) delete []m_pImageData;
	m_pImageData = new unsigned char[m_iImageDataSize];
	inf.read((char*)m_pImageData, m_iImageDataSize);

	inf.close();

	//Create a bitmap handle
	m_hBitmap = CreateBitmap(m_iImageWidth, m_iImageHeight, 1, m_iBitsPerPixel, m_pImageData);

	return true;
}
//------------------------------------------------
//Load the BMP image data from a bitmap buffer
//------------------------------------------------
bool CBmpImage::LoadDataFromBuff(unsigned char* pImageBuff, int iLen)
{
	if(pImageBuff == NULL) 
		return false;

	//Read the file header
	BITMAPFILEHEADER header;
	memset(&header, 0, sizeof(header));
	memcpy(&header, pImageBuff, sizeof(header));
	if(header.bfType != 0x4D42)
		return false;

	//Read the image information header
	BITMAPINFOHEADER infoheader;
	memset(&infoheader, 0, sizeof(infoheader));
	memcpy(&infoheader, pImageBuff+sizeof(header), sizeof(infoheader));
	m_iImageWidth = infoheader.biWidth;
	m_iImageHeight = infoheader.biHeight;
	m_iBitsPerPixel = infoheader.biBitCount;
	
	//Calculate the image data size
	int iLineByteCnt = (((m_iImageWidth*m_iBitsPerPixel) + 31) >> 5) << 2;
	m_iImageDataSize = iLineByteCnt * m_iImageHeight;

	//Load the image data to buffer
	if(m_pImageData) delete []m_pImageData;
	m_pImageData = new unsigned char[m_iImageDataSize];
	memcpy(m_pImageData, pImageBuff+sizeof(header)+sizeof(infoheader), m_iImageDataSize);

	//Create a bitmap handle
	m_hBitmap = CreateBitmap(m_iImageWidth, m_iImageHeight, 1, m_iBitsPerPixel, m_pImageData);

	return true;
}
//------------------------------------------------
//Save the image data in this class to a bmp file
//Before call this function,MUST set the image attributes
//and image data buffer pointer to this class
//------------------------------------------------
bool CBmpImage::SaveToFile(const char* pszFile)
{	
	if(pszFile == NULL)
		return false;

	if(m_pImageData == NULL)
		return false;

	//Open file
	ofstream outf(pszFile, ios::binary);
	if(0 == outf.is_open()) 
		return false;

	//Write the file header
	BITMAPFILEHEADER header;
	memset(&header, 0, sizeof(header));
	header.bfType = 0x4D42;
	header.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_iImageDataSize;
	header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	outf.write((const char*)&header, sizeof(header));

	//Write the image information header
	BITMAPINFOHEADER infoheader;
	memset(&infoheader, 0, sizeof(infoheader));
	infoheader.biSize = sizeof(infoheader);
	infoheader.biWidth = m_iImageWidth;
	infoheader.biHeight = m_iImageHeight;
	infoheader.biPlanes = 1;
	infoheader.biBitCount = m_iBitsPerPixel;
	infoheader.biCompression = BI_RGB;

	//Calculate the image line byte size
	int iLineByteCnt = (((m_iImageWidth*m_iBitsPerPixel) + 31) >> 5) << 2;
	infoheader.biSizeImage = iLineByteCnt * m_iImageHeight;
	outf.write((const char*)&infoheader, sizeof(infoheader));

	//Do image data row arrange
	int iBytesPerPixel = m_iBitsPerPixel/8;
	if(infoheader.biSizeImage != m_iImageDataSize)
	{		
		DealImageRowPad();
	}
	
	//Write image data
	outf.write((const char*)m_pImageData, m_iImageDataSize);	
	
	outf.close();

	return true;
}
//------------------------------------------------