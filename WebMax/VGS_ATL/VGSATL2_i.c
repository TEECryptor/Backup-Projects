

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Sat Sep 04 19:24:30 2010
 */
/* Compiler settings for .\VGSATL2.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IVGS2,0xF66246AF,0xD0D7,0x46D3,0x89,0x1F,0x9E,0xFB,0xC9,0xFA,0xC2,0x57);


MIDL_DEFINE_GUID(IID, LIBID_VGSATL2Lib,0xA357793E,0x7647,0x47B3,0x8E,0xB8,0x37,0x24,0x32,0xAB,0xF6,0xB2);


MIDL_DEFINE_GUID(IID, DIID__IVGS2Events,0x994E6C81,0x8A03,0x4313,0x97,0x36,0xF6,0xFA,0xA1,0xDE,0xF9,0x5C);


MIDL_DEFINE_GUID(CLSID, CLSID_VGS2,0x046216A1,0x952E,0x486C,0x85,0x98,0x4F,0x2D,0x0D,0x06,0x76,0xF2);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



