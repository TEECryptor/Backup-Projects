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
//	名称：jpgOpenFile
//	简介：打开jpg文件，读取图片属性和数据到成员变量
//	参数：pszFileName:i:要加载的jpg文件名
//	返回：加载成功返回TRUE,否则为FALSE
//--------------------------------------------------
BOOL CJpgManager::jpgOpenFile(const char* pszFileName)
{
	//存放图片属性和数据的结构体
	JPEG_CORE_PROPERTIES image;
	ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );
	BYTE* imageData;

	//读取文件内容
	try
	{
		//初始化Ijl库
		if( ijlInit( &image ) != IJL_OK )
		{
			//TRACE( "Cannot initialize Intel JPEG library\n" );
			//AfxThrowUserException();
		}

		//文件名
		image.JPGFile = const_cast<char*>(pszFileName);

		//读取图片属性数据
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
		//图片属性
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

		//读取图片数据
		if( ijlRead( &image, IJL_JFILE_READWHOLEIMAGE ) != IJL_OK )
		{
//			TRACE( "Cannot read image data from %s file\n", image.JPGFile );
			delete[] imageData;
//			AfxThrowUserException();
		}

		//释放Ijl库
		if( ijlFree( &image ) != IJL_OK )
		{
//			TRACE( "Cannot free Intel(R) JPEG library" );
		}

		//格式转换
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
//	简介：格式转换
//	参数：data:i:RGBA格式的图片数据
//			  :o:转换后的BGRA格式图片数据
//		  width:i:图片的像素宽
//		  height:i:图片的像素高
//	返回：无
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
//	名称：jpgGetImageData
//	简介：返回jpg图片数据
//	参数：无
//	返回：jpg图片数据
//--------------------------------------------------
BYTE* CJpgManager::jpgGetImageData()
{
	return m_pImageData;
}

//--------------------------------------------------
//	名称：jpgGetIamgeParams
//	简介：返回图片参数
//	参数：eParams:i:参数类型,0-图片数据大小1-图片宽度2-图片高度3-图片的通道数据
//	返回：jpg图片数据
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
//	名称：jpgGetIamgeBMPInfo
//	简介：返回图片BMP的信息
//	参数：stuBMPInfo:i:BITMAPINFO结构体
//					:o:BITMAPINFO结构体
//	返回：jpg图片BITMAPINFO结构信息
//--------------------------------------------------
void CJpgManager::jpgGetImageBMPInfo(BITMAPINFO &stuBMPInfo)
{
	memcpy(&stuBMPInfo,&m_stuBMPInfo,sizeof(BITMAPINFO));
}


//--------------------------------------------------
//	名称：jpgSetImageParams
//	简介：设置图片参数
//	参数：eParams:i:参数类型,0-图片数据大小1-图片宽度2-图片高度3-图片的通道数据
//		  iParam:i:参数值
//	返回：无
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
//	名称：jpgSetImageParams
//	简介：将数据保存为JPG格式的文件
//	参数：eParams:i:参数类型,0-图片数据大小1-图片宽度2-图片高度3-图片的通道数据
//	返回：无
//--------------------------------------------------
BOOL CJpgManager::jpgSaveFile(BYTE* pImageData, const char* pszFileName)
{
	//检查buff
	if(pImageData == NULL)
	{
//		AfxMessageBox("要保存的图片数据错误！");
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
//	将位图数据保存为JPG格式的buff
//--------------------------------------------------
BOOL CJpgManager::jpgSaveBuff(BYTE* pImageData, int &iJpgSize)
{	
	//检查buff
	if(pImageData == NULL)
	{
//		AfxMessageBox("要保存的图片数据错误！");
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
//	名称：jpgBGRA_to_RGBA
//	简介：格式转换
//	参数：data:i:要转换的图片数据
//			  :o:转换后的图片数据
//		  width:i:图片宽度
//		  height:i:图片高度
//	返回：无
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
//	名称：jpgReadBuff
//	简介：从内存里读取数据，然后转化为位图数据
//	参数：pJpgData:i:要转换的jpg图片数据
//	返回：无
//--------------------------------------------------
BOOL CJpgManager::jpgReadBuff(BYTE* pJpgData, int iJpgDataSize)
{
	//存放图片属性和数据的结构体
	JPEG_CORE_PROPERTIES image;
	ZeroMemory( &image, sizeof( JPEG_CORE_PROPERTIES ) );
	BYTE* imageData;

	//读取buff内容
	try
	{
		//初始化Ijl库
		if( ijlInit( &image ) != IJL_OK )
		{
			//TRACE( "Cannot initialize Intel JPEG library\n" );
			//AfxThrowUserException();
		}

		//jpg文件的数据buff和大小
		image.JPGFile = "no name";
		image.JPGBytes = pJpgData;
		image.JPGSizeBytes = iJpgDataSize;

		//从buff里读取图片属性数据
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
		//图片属性
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

		//读取图片数据
		if( ijlRead( &image, IJL_JBUFF_READWHOLEIMAGE ) != IJL_OK )
		{
			//TRACE( "Cannot read image data from %s file\n", image.JPGFile );
			delete[] imageData;
			//AfxThrowUserException();
		}

		//释放Ijl库
		if( ijlFree( &image ) != IJL_OK )
		{
			//TRACE( "Cannot free Intel(R) JPEG library" );
		}

		//格式转换
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
