/*----------------------------------------------------------------

	简介：Sprite类接口
	文件：Sprite.h
	版权所有：2003-	王延樑

	当前版本：0.1
	作者：王延樑
	时间：2004-07-15 21:17

----------------------------------------------------------------*/
#include "stdafx.h"
#include "OGLExtension.h"
//----------------------------------------------------------------
//定义OpenGL扩展函数指针
//WGL_EXT_swap_control
PFNWGLSWAPINTERVALEXTPROC    wglSwapIntervalEXT    = NULL;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = NULL;
//GL_ARB_multitexture
PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB       = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
PFNGLMULTITEXCOORD1DARBPROC     glMultiTexCoord1dARB     = NULL;
PFNGLMULTITEXCOORD1DVARBPROC    glMultiTexCoord1dvARB    = NULL;
PFNGLMULTITEXCOORD1FARBPROC     glMultiTexCoord1fARB     = NULL;
PFNGLMULTITEXCOORD1FVARBPROC    glMultiTexCoord1fvARB    = NULL;
PFNGLMULTITEXCOORD2DARBPROC     glMultiTexCoord2dARB     = NULL;
PFNGLMULTITEXCOORD2DVARBPROC    glMultiTexCoord2dvARB    = NULL;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB     = NULL;
PFNGLMULTITEXCOORD2FVARBPROC    glMultiTexCoord2fvARB    = NULL;
PFNGLMULTITEXCOORD3DARBPROC     glMultiTexCoord3dARB     = NULL;
PFNGLMULTITEXCOORD3DVARBPROC    glMultiTexCoord3dvARB    = NULL;
PFNGLMULTITEXCOORD3FARBPROC     glMultiTexCoord3fARB     = NULL;
PFNGLMULTITEXCOORD3FVARBPROC    glMultiTexCoord3fvARB    = NULL;
PFNGLMULTITEXCOORD4DARBPROC     glMultiTexCoord4dARB     = NULL;
PFNGLMULTITEXCOORD4DVARBPROC    glMultiTexCoord4dvARB    = NULL;
PFNGLMULTITEXCOORD4FARBPROC     glMultiTexCoord4fARB     = NULL;
PFNGLMULTITEXCOORD4FVARBPROC    glMultiTexCoord4fvARB    = NULL;
//GL_EXT_texture3D
PFNGLTEXIMAGE3DEXTPROC    glTexImage3DEXT    = NULL;
PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3DEXT = NULL;
//GL_ARB_texture_compression
PFNGLCOMPRESSEDTEXIMAGE3DARBPROC    glCompressedTexImage3DARB    = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC    glCompressedTexImage2DARB    = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DARBPROC    glCompressedTexImage1DARB    = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC glCompressedTexSubImage3DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC glCompressedTexSubImage2DARB = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC glCompressedTexSubImage1DARB = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEARBPROC   glGetCompressedTexImageARB   = NULL;
// WGL_ARB_extensions_string
PFNWGLGETEXTENSIONSSTRINGARBPROC    wglGetExtensionsStringARB    = NULL;
// WGL_ARB_pixel_format
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB      = NULL;
// WGL_ARB_multisample
PFNGLSAMPLECOVERAGEARBPROC glSampleCoverageARB = NULL;
// WGL_ARB_pbuffer
PFNWGLCREATEPBUFFERARBPROC    wglCreatePbufferARB    = NULL;
PFNWGLGETPBUFFERDCARBPROC     wglGetPbufferDCARB     = NULL;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB = NULL;
PFNWGLDESTROYPBUFFERARBPROC   wglDestroyPbufferARB   = NULL;
PFNWGLQUERYPBUFFERARBPROC     wglQueryPbufferARB     = NULL;
// WGL_ARB_RENDER_TEXTURE
PFNWGLBINDTEXIMAGEARBPROC     wglBindTexImageARB     = NULL;
PFNWGLRELEASETEXIMAGEARBPROC  wglReleaseTexImageARB  = NULL;
PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB = NULL;
//GL_ARB_vertex_buffer_object
PFNGLBINDBUFFERARBPROC           glBindBufferARB           = NULL;
PFNGLDELETEBUFFERSARBPROC        glDeleteBuffersARB        = NULL;
PFNGLGENBUFFERSARBPROC           glGenBuffersARB           = NULL;
PFNGLISBUFFERARBPROC             glIsBufferARB             = NULL;
PFNGLBUFFERDATAARBPROC           glBufferDataARB           = NULL;
PFNGLBUFFERSUBDATAARBPROC        glBufferSubDataARB        = NULL;
PFNGLGETBUFFERSUBDATAARBPROC     glGetBufferSubDataARB     = NULL;
PFNGLMAPBUFFERARBPROC            glMapBufferARB            = NULL;
PFNGLUNMAPBUFFERARBPROC          glUnmapBufferARB          = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = NULL;
PFNGLGETBUFFERPOINTERVARBPROC    glGetBufferPointervARB    = NULL;
//GL_ARB_point_parameters
PFNGLPOINTPARAMETERFEXTPROC      glPointParameterfARB  = NULL;
PFNGLPOINTPARAMETERFVEXTPROC     glPointParameterfvARB = NULL;
//----------------------------------------------------------------
//	名称：InitOGLExtension
//	简介：初始化OGL扩展函数指针
//	参数：无
//	返回：无
//----------------------------------------------------------------
void GetProc(PROC* func, const char* name)
{
	*func = wglGetProcAddress(name);
}
//----------------------------------------------------------------
//	名称：InitOGLExtension
//	简介：初始化OGL扩展函数指针
//	参数：无
//	返回：无
//----------------------------------------------------------------
void InitOGLExtension(void)
{
	static bool s_Initlized = false;
	if(!s_Initlized)
	{
#define GET_PROC(name) GetProc((PROC*)&name, #name)
		//WGL_EXT_swap_control
		GET_PROC(wglSwapIntervalEXT);
		GET_PROC(wglGetSwapIntervalEXT);
		//GL_ARB_multitexture
		GET_PROC(glActiveTextureARB);
		GET_PROC(glClientActiveTextureARB);
		GET_PROC(glMultiTexCoord1dARB);
		GET_PROC(glMultiTexCoord1dvARB);
		GET_PROC(glMultiTexCoord1fARB);
		GET_PROC(glMultiTexCoord1fvARB);
		GET_PROC(glMultiTexCoord2dARB);
		GET_PROC(glMultiTexCoord2dvARB);
		GET_PROC(glMultiTexCoord2fARB);
		GET_PROC(glMultiTexCoord2fvARB);
		GET_PROC(glMultiTexCoord3dARB);
		GET_PROC(glMultiTexCoord3dvARB);
		GET_PROC(glMultiTexCoord3fARB);
		GET_PROC(glMultiTexCoord3fvARB);
		GET_PROC(glMultiTexCoord4dARB);
		GET_PROC(glMultiTexCoord4dvARB);
		GET_PROC(glMultiTexCoord4fARB);
		GET_PROC(glMultiTexCoord4fvARB);
		//GL_EXT_texture3D
		GET_PROC(glTexImage3DEXT);
		GET_PROC(glTexSubImage3DEXT);
		//GL_ARB_texture_compression
		GET_PROC(glCompressedTexImage3DARB);
		GET_PROC(glCompressedTexImage2DARB);
		GET_PROC(glCompressedTexImage1DARB);
		GET_PROC(glCompressedTexSubImage3DARB);
		GET_PROC(glCompressedTexSubImage2DARB);
		GET_PROC(glCompressedTexSubImage1DARB);
		GET_PROC(glGetCompressedTexImageARB);
		// WGL_ARB_extensions_string
		GET_PROC(wglGetExtensionsStringARB);
		// WGL_ARB_pixel_format
		GET_PROC(wglGetPixelFormatAttribivARB);
		GET_PROC(wglGetPixelFormatAttribfvARB);
		GET_PROC(wglChoosePixelFormatARB);
		// WGL_ARB_multisample
		GET_PROC(glSampleCoverageARB);
		// WGL_ARB_pbuffer
		GET_PROC(wglCreatePbufferARB);
		GET_PROC(wglGetPbufferDCARB);
		GET_PROC(wglReleasePbufferDCARB);
		GET_PROC(wglDestroyPbufferARB);
		GET_PROC(wglQueryPbufferARB);
		// WGL_ARB_RENDER_TEXTURE
		GET_PROC(wglBindTexImageARB);
		GET_PROC(wglReleaseTexImageARB);
		GET_PROC(wglSetPbufferAttribARB);
		//GL_ARB_vertex_buffer_object
		GET_PROC(glBindBufferARB);
		GET_PROC(glDeleteBuffersARB);
		GET_PROC(glGenBuffersARB);
		GET_PROC(glIsBufferARB);
		GET_PROC(glBufferDataARB);
		GET_PROC(glBufferSubDataARB);
		GET_PROC(glGetBufferSubDataARB);
		GET_PROC(glMapBufferARB);
		GET_PROC(glUnmapBufferARB);
		GET_PROC(glGetBufferParameterivARB);
		GET_PROC(glGetBufferPointervARB);
		//GL_ARB_point_parameters
		GET_PROC(glPointParameterfARB);
		GET_PROC(glPointParameterfvARB);
#undef GET_PROC
		s_Initlized = true;
	}
}
//----------------------------------------------------------------
