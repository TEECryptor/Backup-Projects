#pragma once
#include <Windows.h>
#include <string>
#include "./a3dModel/xStdInclude.h"
class _A3D_API_ CFileSystem
{
    CFileSystem(){};
public:
    std::wstring                   getPath(const wchar_t* _filename);
    void                           convertToSystem(std::wstring& str);
    void                           convertSystemFileName(std::wstring& _fileName);
    std::wstring                   GetUpLevelAbsPath(const wchar_t* _relPath , const wchar_t* pRootPath);
    const wchar_t*                 getAbsPath(const wchar_t* _relPath , const wchar_t* pRootPath);
    std::wstring                   getRelatePath(const wchar_t* _filename , const wchar_t* _basePath);
    std::wstring                   getFileName(const wchar_t* fullName);
    std::wstring                   getPathName(const wchar_t* fullName);
    const wchar_t*                 getFileExtName(const wchar_t* filename );
    bool                           fileExist(const wchar_t* filename);
    static CFileSystem*            singleton();
    template <typename T>void      fileNameWithoutExt(const wchar_t* name , T& out)
    {
        for(size_t i = 0 ; i < wcslen(name) ; i ++)
        {
            if(name[i] == '.') break;
            out.push_back( name[i] );
        }
    }
};

_A3D_API_ void            A3D_SetResourcePath(const wchar_t* _ResPath);
_A3D_API_ void            A3D_SetResourcePath(const char*    _ResPath);
_A3D_API_ const wchar_t*  A3D_ABSPATH(const wchar_t* _relativePath);