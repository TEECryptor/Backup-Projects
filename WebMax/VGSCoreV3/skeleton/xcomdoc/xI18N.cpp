/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "xI18N.h"
#include <windows.h>

bool XEvol_Utf8ToUnicode(const char* _in, wchar_t* _out,size_t outlen)
{
    return 0 != MultiByteToWideChar(CP_UTF8 , 0 , _in , (int)strlen(_in) , _out , (int)outlen);
}

bool XEvol_UnicodeToUtf8(const wchar_t* _in, char* _out,size_t outlen)
{
   return 0 != WideCharToMultiByte(CP_UTF8 , 0 , _in , (int)wcslen(_in) , _out , (int)outlen , NULL , NULL);
}


bool XEvol_LocaleToUtf8(const char* _in, char* _out,size_t outlen)
{
    wchar_t* _unicode =  new wchar_t [ strlen(_in) + 1] ;
    XEvol_LocaleToUnicode(_in , _unicode , (int)strlen(_in) + 1 );
    bool ret = XEvol_UnicodeToUtf8(_unicode , _out , outlen);
    delete [] _unicode;
    return ret;
}


bool XEvol_Utf8ToLocale(const char* _in, char* _out,size_t outlen)
{
    wchar_t* _unicode =  new wchar_t [ strlen(_in) + 1] ;
    XEvol_Utf8ToUnicode(_in , _unicode , (int)strlen(_in) + 1 );
    bool ret = XEvol_UnicodeToLocale(_unicode , _out , outlen);
    delete [] _unicode;
    return ret;
}

bool XEvol_UnicodeToLocale(const wchar_t* _in, char* _out,size_t outlen)
{
   return 0 != WideCharToMultiByte(CP_ACP , 0 , _in , (int)wcslen(_in) , _out , (int)outlen , NULL , NULL);
}

bool XEvol_LocaleToUnicode(const char* _in, wchar_t* _out,size_t outlen)
{
    return 0 != MultiByteToWideChar(CP_ACP , 0 , _in , (int)strlen(_in) , _out , (int)outlen);
}

