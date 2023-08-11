// JpgManager.cpp: implementation of the CJpgManager class.
//
//////////////////////////////////////////////////////////////////////

#include "JpgManager.h"
#include "ijl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJpgManager::CJpgManager()
{
	m_iImageSize = 0;
	m_iImageWidth = 0;
	m_iImageHeight = 0;
	m_iImageChannels = 0;
	m_pImageData = NULL;
}

CJpgManager::~CJpgManager()
{
	if(m_pImageData)
	{
		delete m_pImageData;
		m_pImageData = NULL;
	}
}

//--------------------------------------------------
//	���ƣ�jpgOpenFile
//	��飺��jpg�ļ�����ȡͼƬ���Ժ����ݵ���Ա����
//	������pszFileName:i:Ҫ���ص�jpg�ļ���
//	���أ����سɹ�����TRUE,����ΪFALSE
//--------------------------------------------------
BOOL CJpgManager::jpgOpenFile(const char* pszFileName)
{
	//���ͼƬ���Ժ����ݵĽṹ��
	JPEG_CORE_PROPERTIES image;
	ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );
	BYTE* imageData;

	//��ȡ�ļ�����
	try
	{
		//��ʼ��Ijl��
		if( ijlInit( &image ) != IJL_OK )
		{
			//TRACE( "Cannot initialize Intel JPEG library\n" );
			//AfxThrowUserException();
		}

		//�ļ���
		image.JPGFile = const_cast<char*>(pszFileName);

		//��ȡͼƬ��������
		if( ijlRead( &image, IJL_JFILE_READPARAMS ) != IJL_OK )
		{/*
			TRACE( "Cannot read JPEG file header from %s file\n",
			  image.JPGFile );
			AfxThrowUserException();*/
		}

		// !dudnik: to fix bug case 584680, [OT:287A305B]
		// Set the JPG color space ... this will always be
		// somewhat of an educated guess at best because JPEG
		// is "color blind" (i.e., nothing in the bit stream
		// tells you what color space the data was encoded from).
		// However, in this example we assume that we are
		// reading JFIF files which means that 3 channel images
		// are in the YCbCr color space and 1 channel images are
		// in the Y color space.
		switch(image.JPGChannels)
		{
		case 1:
		  image.JPGColor    = IJL_G;
		  image.DIBChannels = 3;
		  image.DIBColor    = IJL_BGR;
		  break;

		case 3:
		  image.JPGColor    = IJL_YCBCR;
		  image.DIBChannels = 3;
		  image.DIBColor    = IJL_BGR;
		  break;

		case 4:
		  image.JPGColor    = IJL_YCBCRA_FPX;
		  image.DIBChannels = 4;
		  image.DIBColor    = IJL_RGBA_FPX;
		  break;

		default:
		  // This catches everything else, but no
		  // color twist will be performed by the IJL.
		  image.DIBColor = (IJL_COLOR)IJL_OTHER;
		  image.JPGColor = (IJL_COLOR)IJL_OTHER;
		  image.DIBChannels = image.JPGChannels;
		  break;
		}
		//ͼƬ����
		image.DIBWidth    = image.JPGWidth;
		image.DIBHeight   = image.JPGHeight;
		image.DIBPadBytes = IJL_DIB_PAD_BYTES(image.DIBWidth,image.DIBChannels);

		m_iImageSize = (image.DIBWidth * image.DIBChannels + image.DIBPadBytes) *
		  image.DIBHeight;

		imageData = new BYTE[ m_iImageSize ];
		if( imageData == NULL )
		{
//			TRACE( "Cannot allocate memory for image\n" );
//			AfxThrowUserException();
		}

		image.DIBBytes = imageData;

		//��ȡͼƬ����
		if( ijlRead( &image, IJL_JFILE_READWHOLEIMAGE ) != IJL_OK )
		{
//			TRACE( "Cannot read image data from %s file\n", image.JPGFile );
			delete[] imageData;
//			AfxThrowUserException();
		}

		//�ͷ�Ijl��
		if( ijlFree( &image ) != IJL_OK )
		{
//			TRACE( "Cannot free Intel(R) JPEG library" );
		}

		//��ʽת��
		if(image.DIBColor == IJL_RGBA_FPX)
		{
			jpgRGBA_FPX_to_BGRA(imageData,image.DIBWidth,image.DIBHeight);
		}
	}
	catch(... )
	{
		ijlFree( &image );

		//AfxMessageBox( "Error opening JPEG file" );
		return FALSE;
	}

	//END_CATCH_ALL

	// initializing incapsulated image with correct values
	m_pImageData     = imageData;
	m_iImageWidth	 = image.DIBWidth;
	m_iImageHeight	 = image.DIBHeight;
	m_iImageChannels = image.DIBChannels;

	// now we have
	//   m_imageData  containing image data, and
	//   m_imageDims  with image dimensions, and
	//   m_imageChannels with image number of channels

   BITMAPINFOHEADER& bih = m_stuBMPInfo.bmiHeader;

	::ZeroMemory( &bih, sizeof( BITMAPINFOHEADER ) );

	bih.biSize        = sizeof( BITMAPINFOHEADER );
	bih.biWidth       = m_iImageWidth;//sizes.cx;
	bih.biHeight      = -m_iImageHeight;//-sizes.cy;
	bih.biCompression = BI_RGB;
	bih.biPlanes      = 1;

	switch(m_iImageChannels)
	{
	case 3:
	  bih.biBitCount = 24;
	  break;

	case 4:
	  bih.biBitCount = 32;
	  break;

	default:
//	  TRACE("Unsupported number of channels!\n");
	  break;
	}
	return TRUE;
}

//--------------------------------------------------
//	jpgRGBA_FPX_to_BGRA
//	��飺��ʽת��
//	������data:i:RGBA��ʽ��ͼƬ����
//			  :o:ת�����BGRA��ʽͼƬ����
//		  width:i:ͼƬ�����ؿ�
//		  height:i:ͼƬ�����ظ�
//	���أ���
//--------------------------------------------------
void CJpgManager::jpgRGBA_FPX_to_BGRA(BYTE* data,int width,int height)
{
	int   i;
	int   j;
	int   pad;
	int   line_width;
	BYTE  r, g, b, a;
	BYTE* ptr;

	ptr = data;
	pad = IJL_DIB_PAD_BYTES(width,4);
	line_width = width * 4 + pad;

	for(i = 0; i < height; i++)
	{
		ptr = data + line_width*i;
		for(j = 0; j < width; j++)
		{
			r = ptr[0];
			g = ptr[1];
			b = ptr[2];
			a = ptr[3];
			ptr[2] = (r*a+1) >> 8;
			ptr[1] = (g*a+1) >> 8;
			ptr[0] = (b*a+1) >> 8;
			ptr += 4;
		}
	}
}
//--------------------------------------------------
//	���ƣ�jpgGetImageData
//	��飺����jpgͼƬ����
//	��������
//	���أ�jpgͼƬ����
//--------------------------------------------------
BYTE* CJpgManager::jpgGetImageData()
{
	return m_pImageData;
}

//--------------------------------------------------
//	���ƣ�jpgGetIamgeParams
//	��飺����ͼƬ����
//	������eParams:i:��������,0-ͼƬ���ݴ�С1-ͼƬ���2-ͼƬ�߶�3-ͼƬ��ͨ������
//	���أ�jpgͼƬ����
//--------------------------------------------------
int CJpgManager::jpgGetImageParams(JPGPARAMS eParams)
{
	switch(eParams)
	{
	case _JPGIMAGESIZE:
		return m_iImageSize;
	case _JPGIMAGEWIDTH:
		return m_iImageWidth;
	case _JPGIMAGEHEIGHT:
		return m_iImageHeight;
	case _JPGIMAGECHANNELS:
		return m_iImageChannels;
	default:
		return 0;
	}
}

//--------------------------------------------------
//	���ƣ�jpgGetIamgeBMPInfo
//	��飺����ͼƬBMP����Ϣ
//	������stuBMPInfo:i:BITMAPINFO�ṹ��
//					:o:BITMAPINFO�ṹ��
//	���أ�jpgͼƬBITMAPINFO�ṹ��Ϣ
//--------------------------------------------------
void CJpgManager::jpgGetImageBMPInfo(BITMAPINFO &stuBMPInfo)
{
	memcpy(&stuBMPInfo,&m_stuBMPInfo,sizeof(BITMAPINFO));
}


//--------------------------------------------------
//	���ƣ�jpgSetImageParams
//	��飺����ͼƬ����
//	������eParams:i:��������,0-ͼƬ���ݴ�С1-ͼƬ���2-ͼƬ�߶�3-ͼƬ��ͨ������
//		  iParam:i:����ֵ
//	���أ���
//--------------------------------------------------
void CJpgManager::jpgSetImageParams(JPGPARAMS eParams, int iParam)
{
	switch(eParams)
	{
	case _JPGIMAGESIZE:
		m_iImageSize = iParam;
		break;
	case _JPGIMAGEWIDTH:
		m_iImageWidth = iParam;
		break;
	case _JPGIMAGEHEIGHT:
		m_iImageHeight = iParam;
		break;
	case _JPGIMAGECHANNELS:
		m_iImageChannels = iParam;
		break;
	default:
		return;
	}
}

//--------------------------------------------------
//	���ƣ�jpgSetImageParams
//	��飺�����ݱ���ΪJPG��ʽ���ļ�
//	������eParams:i:��������,0-ͼƬ���ݴ�С1-ͼƬ���2-ͼƬ�߶�3-ͼƬ��ͨ������
//	���أ���
//--------------------------------------------------
BOOL CJpgManager::jpgSaveFile(BYTE* pImageData, const char* pszFileName)
{
	//���buff
	if(pImageData == NULL)
	{
//		AfxMessageBox("Ҫ�����ͼƬ���ݴ���");
		return FALSE;
			
	}
	//
    BOOL bres = TRUE;
    JPEG_CORE_PROPERTIES image;
    ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );
	//
    try
	{
        if( ijlInit( &image ) != IJL_OK )
        {
//            TRACE( "Can't initialize Intel(R) JPEG library\n" );
//            AfxThrowUserException();
        }

        // Setup DIB
        image.DIBWidth         = m_iImageWidth;//m_imageDims.cx;
        image.DIBHeight        = m_iImageHeight;//m_imageDims.cy;
        image.DIBBytes         = pImageData;//m_imageData;

        // Setup JPEG
        image.JPGFile          = const_cast<char*>(pszFileName);
        image.JPGWidth         = m_iImageWidth;//m_imageDims.cx;
        image.JPGHeight        = m_iImageHeight;//m_imageDims.cy;

        switch(m_iImageChannels)
        {
        case 3:
          image.DIBColor       = IJL_BGR;
          image.DIBChannels    = 3;
          image.DIBPadBytes    = IJL_DIB_PAD_BYTES(image.DIBWidth,3);
          image.JPGColor       = IJL_YCBCR;
          image.JPGChannels    = 3;
          image.JPGSubsampling = IJL_411;
          break;

        case 4:
          image.DIBColor       = IJL_RGBA_FPX;
          image.DIBChannels    = 4;
          image.DIBPadBytes    = IJL_DIB_PAD_BYTES(image.DIBWidth,4);
          image.JPGColor       = IJL_YCBCRA_FPX;
          image.JPGChannels    = 4;
          image.JPGSubsampling = IJL_4114;
          break;

        default:
          break;
        }

        if(image.DIBColor == IJL_RGBA_FPX)
        {
			jpgBGRA_to_RGBA(image.DIBBytes, image.DIBWidth, image.DIBHeight);
        }

        if( ijlWrite( &image, IJL_JFILE_WRITEWHOLEIMAGE ) != IJL_OK )
        {
//            TRACE( "Can't write image\n" );
//            AfxThrowUserException();
        }

        if( ijlFree( &image ) != IJL_OK )
        {
//            TRACE( "Can't free Intel(R) JPEG library\n" );
        }
	}
    catch( ... )
	{
        bres = FALSE;
        ijlFree( &image );

        //AfxMessageBox( "Error storing JPEG file" );
	}
   // END_CATCH_ALL

    return bres;
}
//--------------------------------------------------
//	��λͼ���ݱ���ΪJPG��ʽ��buff
//--------------------------------------------------
BOOL CJpgManager::jpgSaveBuff(BYTE* pImageData, int &iJpgSize)
{	
	//���buff
	if(pImageData == NULL)
	{
//		AfxMessageBox("Ҫ�����ͼƬ���ݴ���");
		return FALSE;			
	}
	//
    BOOL bres = TRUE;
    JPEG_CORE_PROPERTIES image;
    ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );
	//
    try
	{
        if( ijlInit( &image ) != IJL_OK )
        {
//            TRACE( "Can't initialize Intel(R) JPEG library\n" );
//            AfxThrowUserException();
        }

        // Setup DIB
        image.DIBWidth         = m_iImageWidth;
        image.DIBHeight        = m_iImageHeight;
        image.DIBBytes         = pImageData;

        // Setup JPEG
        image.JPGFile          = const_cast<char*>("");
        image.JPGWidth         = m_iImageWidth;
        image.JPGHeight        = m_iImageHeight;

        switch(m_iImageChannels)
        {
        case 3:
          image.DIBColor       = IJL_BGR;
          image.DIBChannels    = 3;
          image.DIBPadBytes    = IJL_DIB_PAD_BYTES(image.DIBWidth,3);
          image.JPGColor       = IJL_YCBCR;
          image.JPGChannels    = 3;
          image.JPGSubsampling = IJL_411;
          break;

        case 4:
          image.DIBColor       = IJL_RGBA_FPX;
          image.DIBChannels    = 4;
          image.DIBPadBytes    = IJL_DIB_PAD_BYTES(image.DIBWidth,4);
          image.JPGColor       = IJL_YCBCRA_FPX;
          image.JPGChannels    = 4;
          image.JPGSubsampling = IJL_4114;
          break;

        default:
          break;
        }

        if(image.DIBColor == IJL_RGBA_FPX)
        {
			jpgBGRA_to_RGBA(image.DIBBytes, image.DIBWidth, image.DIBHeight);
        }

        if( ijlWrite( &image, IJL_JBUFF_WRITEWHOLEIMAGE ) != IJL_OK )
        {
			iJpgSize = image.JPGSizeBytes;
//            TRACE( "Can't write image\n" );
//            AfxThrowUserException();
        }

        if( ijlFree( &image ) != IJL_OK )
        {
//            TRACE( "Can't free Intel(R) JPEG library\n" );
        }
	}
    catch( ... )
	{
        bres = FALSE;
        ijlFree( &image );

        //AfxMessageBox( "Error storing JPEG file" );
	}
   // END_CATCH_ALL

    return bres;
}
//--------------------------------------------------


//--------------------------------------------------
//	���ƣ�jpgBGRA_to_RGBA
//	��飺��ʽת��
//	������data:i:Ҫת����ͼƬ����
//			  :o:ת�����ͼƬ����
//		  width:i:ͼƬ���
//		  height:i:ͼƬ�߶�
//	���أ���
//--------------------------------------------------
void CJpgManager::jpgBGRA_to_RGBA(BYTE* data,int width,int height)
{
	int   i;
	int   j;
	int   pad;
	int   line_width;
	BYTE  r, g, b, a;
	BYTE* ptr;

	ptr = data;
	pad = IJL_DIB_PAD_BYTES(width,4);
	line_width = width * 4 + pad;

	for(i = 0; i < height; i++)
	{
		ptr = data + line_width*i;
		for(j = 0; j < width; j++)
		{
			b = ptr[0];
			g = ptr[1];
			r = ptr[2];
			a = ptr[3];
			ptr[0] = r;
			ptr[1] = g;
			ptr[2] = b;
			ptr += 4;
		}
	}
}

//--------------------------------------------------
//	���ƣ�jpgReadBuff
//	��飺���ڴ����ȡ���ݣ�Ȼ��ת��Ϊλͼ����
//	������pJpgData:i:Ҫת����jpgͼƬ����
//	���أ���
//--------------------------------------------------
BOOL CJpgManager::jpgReadBuff(BYTE* pJpgData, int iJpgDataSize)
{
	//���ͼƬ���Ժ����ݵĽṹ��
	JPEG_CORE_PROPERTIES image;
	ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );
	BYTE* imageData;

	//��ȡbuff����
	try
	{
		//��ʼ��Ijl��
		if( ijlInit( &image ) != IJL_OK )
		{
			//TRACE( "Cannot initialize Intel JPEG library\n" );
			//AfxThrowUserException();
		}

		//jpg�ļ�������buff�ʹ�С
		image.JPGFile = "no name";
		image.JPGBytes = pJpgData;
		image.JPGSizeBytes = iJpgDataSize;

		//��buff���ȡͼƬ��������
		IJLERR ijlErr;
		if( (ijlErr = ijlRead( &image, IJL_JBUFF_READPARAMS )) != IJL_OK )
		{
/*			TRACE( "Cannot read JPEG file header from %s file\n",
			  image.JPGFile );
			AfxThrowUserException();*/
		}

		// !dudnik: to fix bug case 584680, [OT:287A305B]
		// Set the JPG color space ... this will always be
		// somewhat of an educated guess at best because JPEG
		// is "color blind" (i.e., nothing in the bit stream
		// tells you what color space the data was encoded from).
		// However, in this example we assume that we are
		// reading JFIF files which means that 3 channel images
		// are in the YCbCr color space and 1 channel images are
		// in the Y color space.
		switch(image.JPGChannels)
		{
		case 1:
		  image.JPGColor    = IJL_G;
		  image.DIBChannels = 3;
		  image.DIBColor    = IJL_BGR;
		  break;

		case 3:
		  image.JPGColor    = IJL_YCBCR;
		  image.DIBChannels = 3;
		  image.DIBColor    = IJL_BGR;
		  break;

		case 4:
		  image.JPGColor    = IJL_YCBCRA_FPX;
		  image.DIBChannels = 4;
		  image.DIBColor    = IJL_RGBA_FPX;
		  break;

		default:
		  // This catches everything else, but no
		  // color twist will be performed by the IJL.
		  image.DIBColor = (IJL_COLOR)IJL_OTHER;
		  image.JPGColor = (IJL_COLOR)IJL_OTHER;
		  image.DIBChannels = image.JPGChannels;
		  break;
		}
		//ͼƬ����
		image.DIBWidth    = image.JPGWidth;
		image.DIBHeight   = image.JPGHeight;
		image.DIBPadBytes = IJL_DIB_PAD_BYTES(image.DIBWidth,image.DIBChannels);

		m_iImageSize = (image.DIBWidth * image.DIBChannels + image.DIBPadBytes) *
		  image.DIBHeight;

		imageData = new BYTE[ m_iImageSize ];
		if( imageData == NULL )
		{
			//TRACE( "Cannot allocate memory for image\n" );
			//AfxThrowUserException();
		}

		image.DIBBytes = imageData;

		//��ȡͼƬ����
		if( ijlRead( &image, IJL_JBUFF_READWHOLEIMAGE ) != IJL_OK )
		{
			//TRACE( "Cannot read image data from %s file\n", image.JPGFile );
			delete[] imageData;
			//AfxThrowUserException();
		}

		//�ͷ�Ijl��
		if( ijlFree( &image ) != IJL_OK )
		{
			//TRACE( "Cannot free Intel(R) JPEG library" );
		}

		//��ʽת��
		if(image.DIBColor == IJL_RGBA_FPX)
		{
			jpgRGBA_FPX_to_BGRA(imageData,image.DIBWidth,image.DIBHeight);
		}
	}
	catch( ... )
	{
		ijlFree( &image );

		//AfxMessageBox( "Error opening JPEG file" );
		return FALSE;
	}

	//END_CATCH_ALL

	// initializing incapsulated image with correct values
	m_pImageData     = imageData;
	m_iImageWidth	 = image.DIBWidth;
	m_iImageHeight	 = image.DIBHeight;
	m_iImageChannels = image.DIBChannels;

	// now we have
	//   m_imageData  containing image data, and
	//   m_imageDims  with image dimensions, and
	//   m_imageChannels with image number of channels

   BITMAPINFOHEADER& bih = m_stuBMPInfo.bmiHeader;

	::ZeroMemory( &bih, sizeof( BITMAPINFOHEADER ) );

	bih.biSize        = sizeof( BITMAPINFOHEADER );
	bih.biWidth       = m_iImageWidth;//sizes.cx;
	bih.biHeight      = -m_iImageHeight;//-sizes.cy;
	bih.biCompression = BI_RGB;
	bih.biPlanes      = 1;

	switch(m_iImageChannels)
	{
	case 3:
	  bih.biBitCount = 24;
	  break;

	case 4:
	  bih.biBitCount = 32;
	  break;

	default:
	 // TRACE("Unsupported number of channels!\n");
	  break;
	}
	return TRUE;
}
