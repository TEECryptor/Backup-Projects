///////////////////////////////////////////////////////////////////////////////
// AS_StringHelper.h
// Copyright (c) 2001 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_StringHelper_h__
#define __AS_StringHelper_h__

#include "AS_StorageTypes.h"

// Client side string helper classes
template<class X, AS_String::Type Y>
struct AS_StringX : AS_String
{
   AS_StringX(size_t size) : s(size, 0) { init(); }
   AS_StringX(const X * data) : s(data) { init(); }
   AS_StringX(std::basic_string<X> string) : s(string) { init(); }
   AS_StringX(const AS_StringX & string) : s(string.s) { init(); }
   AS_StringX & operator =(const AS_StringX & string) { s = string.s; init(); return *this; }
   operator const X *() const { return s.c_str(); }
   operator const std::basic_string<X> () const { return s.c_str(); }

private:

   void init() { type = Y; size = s.size() + 1; data = const_cast<X *>(s.c_str()); }
   std::basic_string<X> s;
};

typedef AS_StringX<char, AS_String::Type_ASCII> AS_StringA; // ASCII string (local encoding)
typedef AS_StringX<unsigned char, AS_String::Type_UTF8> AS_String8; // UTF8 string
typedef AS_StringX<wchar_t, AS_String::Type_Wide> AS_StringW; // wide string



#endif //__AS_StringHelper_h__
