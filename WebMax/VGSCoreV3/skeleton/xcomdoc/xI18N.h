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

#ifndef __XEVOL_I18N_H__
#define __XEVOL_I18N_H__
#include <wchar.h>
bool XEvol_Utf8ToUnicode(const char* _in, wchar_t* _out,size_t outlen);
bool XEvol_UnicodeToUtf8(const wchar_t* _in, char* _out,size_t outlen);
bool XEvol_LocaleToUtf8(const char* _in, char* _out,size_t outlen);
bool XEvol_Utf8ToLocale(const char* _in, char* _out,size_t outlen);
bool XEvol_UnicodeToLocale(const wchar_t* _in, char* _out,size_t outlen);
bool XEvol_LocaleToUnicode(const char* _in, wchar_t* _out,size_t outlen);
#endif

