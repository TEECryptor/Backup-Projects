

#ifndef WIN32_H
#define WIN32_H

#define CK_PTR *

#define CK_DEFINE_FUNCTION(returnType, name) \
	returnType __declspec(dllexport) name

#ifdef Csp_Buildin_RelSpy
#define CK_DECLARE_FUNCTION(returnType, name) \
	returnType name
#else
#define CK_DECLARE_FUNCTION(returnType, name) \
	returnType __declspec(dllexport) name
#endif

#define CK_DECLARE_FUNCTION_POINTER(returnType, name) \
	returnType __declspec(dllimport) (* name)

#define CK_CALLBACK_FUNCTION(returnType, name) \
	returnType (* name)

#ifndef NULL_PTR
#define NULL_PTR 0
#endif

#define DllImport	__declspec( dllimport )
#define DllExport	__declspec( dllexport )

#endif
