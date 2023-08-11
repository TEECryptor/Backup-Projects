#pragma once
#pragma warning(disable : 4786)

#define _A3D_API_


#ifdef   _A3D_BUILD_DLL_ 
#undef _A3D_API_
#define  _A3D_API_   __declspec( dllexport )
#endif

#ifdef  _A3D_USE_DLL_
#undef _A3D_API_
#define  _A3D_API_   __declspec( dllimport )
#endif     



#include <Unknwn.h>
//#if _MSC_VER >= 1310 
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <assert.h>
//#include <hash_map>
//using namespace stdext;
#include <queue>
#include <algorithm>
/*
#else
#include <map.h>
#include <vector.h>
#include <string.h>
#include <queue.h>
#include <iostream.h>
#include <hash_map.h>
using namespace stdext;
#include <algorithm.h>
#endif
*/

class _A3D_API_ xRefCount
{
private:
	int _count;
public:
	xRefCount(int _init = 1):_count(_init) {  }
	operator int(){ return _count ; }
	int& operator = (int _v) { _count = _v ; return _count ; }
	int  operator ++(int) {++_count  ; return _count-1 ; }// a++
	int& operator ++()    {++_count  ; return _count   ; }// ++a
	int  operator --(int) {--_count  ; return _count+1 ; }// a--
	int& operator --()    {--_count  ; return _count   ; }// --a
};

class _A3D_API_ CRefCountObject : public IUnknown
{
protected:                                
	xRefCount        m_RefCount;  
public:
	virtual HRESULT STDMETHODCALLTYPE   QueryInterface(  REFIID riid,  void __RPC_FAR *__RPC_FAR *ppvObject) ;
	virtual ULONG   STDMETHODCALLTYPE   AddRef( void) ;
	virtual ULONG   STDMETHODCALLTYPE   Release( void) ;
};

class _A3D_API_  CFxShader;
class _A3D_API_  CVertexShaderD3D;
class _A3D_API_  CPixelShader;
class _A3D_API_  CBaseTextureLoader;
class _A3D_API_  CTextureName;
class _A3D_API_  xMeshTexture;
class _A3D_API_  xMeshMaterial;
class _A3D_API_  xA3DTexture;
class _A3D_API_  CA3DDeviceWrapper;

