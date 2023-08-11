#include "A3DHelperFunction.h"
#include "./xcomdoc/xI18N.h"
#define PATH_SPLITTER '\\'
#define PATH_SPLITTER_STRING L"\\"
#define PATH_DOTSLASH L".\\"

static std::wstring gs_ResourcePath = L".\\";

void            A3D_SetResourcePath(const wchar_t* _ResPath)
{
    gs_ResourcePath = _ResPath;
}

void            A3D_SetResourcePath(const char*    _ResPath)
{
    wchar_t _buf[1024] = {0 };
    XEvol_LocaleToUnicode(_ResPath , _buf , 1024);
    gs_ResourcePath  = _buf;
}

const wchar_t*  A3D_ABSPATH(const wchar_t* _relativePath)
{
    return CFileSystem::singleton()->getAbsPath(_relativePath , gs_ResourcePath.c_str() );
}


static std::wstring _GetFilePath(const wchar_t* appName , bool bWithLastSlash = false)
{
    std::wstring _filePath = L"";
    wchar_t appPath[512]={0};
    wcsncpy(appPath , appName , 512);
    int i  = 0;
    bool bLastSlash = true;
    for(i = (int)wcslen(appPath) - 1 ; i >= 0 ; i --  )
    {
        if(appPath[i] == '/' || appPath[i] == '\\') 
        {
            if(bLastSlash && bWithLastSlash) //如果是最后一个Slash，并且允许存在最后的Slash，就继续
            {
                continue;
            }
            //否则终止循环
            appPath[i] = 0;
            break;		
        }
        bLastSlash = false;
    }
    if( i == 0)
    {
        _filePath = PATH_DOTSLASH;
    }
    else
    {
        _filePath = std::wstring(appPath) + PATH_SPLITTER_STRING;
    }
    return _filePath;
}



bool bIsUpLevelDir( const wchar_t* _relPath )
{
    return (_relPath[0] == '.' &&  _relPath[1] == '.' &&  (_relPath[2] == '/' || _relPath[2] == '\\') );
}


void CFileSystem::convertToSystem(std::wstring& str)
{
    for(size_t i = 0 ; i < str.length() ; i ++)
    {
        if(str[i] == '\\' || str[i] == '/')
        {
            str[i] = PATH_SPLITTER;
        }
    }
}


void   CFileSystem::convertSystemFileName(std::wstring& _fileName)
{
    for(size_t i = 0 ; i < _fileName.length() ; i ++)
    {
        if(_fileName[i] == '/')
        {
            _fileName[i] = '\\';
        }
        else if(_fileName[i] >= 'A' && _fileName[i] <= 'Z')
        {
            _fileName[i] -= ('A'-'a');
        }
    }
}

std::wstring  CFileSystem::GetUpLevelAbsPath(const wchar_t* _relPath , const wchar_t* pRootPath)
{
    _relPath += 3;
    std::wstring rootPath = _GetFilePath(pRootPath , true).c_str();
    size_t nLen = wcslen(_relPath);
    if(nLen >= 2 && bIsUpLevelDir(_relPath) )
    {
        return GetUpLevelAbsPath(_relPath , rootPath.c_str() );
    }

    std::wstring ret  = rootPath + _relPath;
    return ret;
}

std::wstring CFileSystem::getRelatePath(const wchar_t* _filename , const wchar_t* _basePath)
{
    std::wstring fileName = _filename;
    std::wstring basePath = _basePath;
    convertSystemFileName(fileName);
    convertSystemFileName(basePath);
    if(basePath[basePath.length() - 1] != PATH_SPLITTER )
    {
        basePath += PATH_SPLITTER_STRING;
    }

    std::wstring::size_type pos = fileName.find(basePath);
    if(pos != 0)
        return L"";
    std::wstring ret = fileName.c_str() + basePath.length();;
    return ret;
}



std::wstring    CFileSystem::getPath(const wchar_t* _filename)
{
    static std::wstring _strAppPath;
    wchar_t appPath [256];

    wcsncpy(appPath,_filename,256);
    size_t i  = 0;
    for(i = wcslen(appPath) - 1 ; i >= 0 ; i --  )
    {
        if(appPath[i] == '/' || appPath[i] == '\\') 
        {
            appPath[i] = 0;
            break;
        }
    }
    if( i == 0)
        _strAppPath =  PATH_DOTSLASH;
    else
        _strAppPath =  std::wstring(appPath) + PATH_SPLITTER_STRING;
    return _strAppPath.c_str();
}





const wchar_t*  CFileSystem::getAbsPath(const wchar_t* _relPath , const wchar_t* pRootPath)
{
    static std::wstring _RetPath = L"";
    size_t nLen = wcslen(_relPath);
    if(nLen >= 2)
    {
        if(_relPath[1] == '\\' && _relPath[0] == '\\')
        {
            _RetPath = _relPath;
            convertToSystem(_RetPath);
            return _RetPath.c_str();
        }
        if(_relPath[1] == ':' || _relPath[0] == '/'  || _relPath[0] == '\\') //[C:\....]  [/mnt/....]  [\program files\]
        {
            _RetPath = _relPath;
            convertToSystem(_RetPath);
            return _RetPath.c_str();
        }
        else if(_relPath[0] == '.' &&( _relPath[1] == '/' ||  _relPath[1] == '\\' ) ) // ["./xxxxx"]
        {
            _RetPath = std::wstring( pRootPath ) + &_relPath[2];
            convertToSystem(_RetPath);
            return _RetPath.c_str();
        }
        else if( bIsUpLevelDir(_relPath)  ) // ["../xxxxx"]
        {
            _RetPath = GetUpLevelAbsPath(_relPath , pRootPath);
            convertToSystem(_RetPath);
            return _RetPath.c_str();
        }
        else
        {
            _RetPath = std::wstring( pRootPath ) + _relPath;
            convertToSystem(_RetPath);
            return _RetPath.c_str();
        }
    }
    if(nLen == 0 || _relPath[0] == '.')
    {
        _RetPath = pRootPath;
        convertToSystem(_RetPath);
        return _RetPath.c_str();
    }
    _RetPath = std::wstring( pRootPath ) + _relPath;
    convertToSystem(_RetPath);
    return _RetPath.c_str();
}


CFileSystem* CFileSystem::singleton()
{
    static CFileSystem sys;
    return &sys;
}



std::wstring CFileSystem::getFileName(const wchar_t* fullName)
{
    if(fullName == NULL)
        return L"";
    int idx = (int)wcslen(fullName);
    for(; idx >= 0 ; idx --)
    {
        if(fullName[idx] == '\\' || fullName[idx]=='/')
        {
            break;
        }
    }
    std::wstring _ret = fullName + (idx + 1);   
    return _ret;
}

std::wstring CFileSystem::getPathName(const wchar_t* fullName)
{
    if(fullName == NULL)
        return L"";
    std::wstring _ret = fullName;   
    std::wstring::size_type pos = _ret.rfind('\\');
    if(pos == std::wstring::npos) 
    {
        pos = _ret.rfind('/');
    }
    if(pos == std::wstring::npos )
    {
        return L"";
    }

    _ret.replace(pos ,wcslen(fullName), L"");
    return _ret + L"/";
}

bool CFileSystem::fileExist(const wchar_t* wcsFileName)
{
    FILE* fp = _wfopen(wcsFileName , L"rb");
    bool bRet = fp != NULL;
    if(fp) fclose(fp);
    return bRet;
}



const wchar_t* CFileSystem::getFileExtName(const wchar_t* filename )
{
    static wchar_t name_ext[16]={0};
    name_ext[0] = 0;
    size_t  len = wcslen(filename);

    for( size_t i = len-1 ; i > 0 ; i -- )
    {
        if(filename[i] == '.')
        {
            wcsncpy(name_ext,filename + i + 1,16);
            {
                for(size_t j=0;j< (len - i) ; j++)
                {
                    if(name_ext[j] < 127)
                    {
                        name_ext[j] = tolower(name_ext[j]);
                    }	
                }
            }
            break ;
        }
    }
    return name_ext;
}
