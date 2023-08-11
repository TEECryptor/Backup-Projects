#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <sstream>
#include "../TextureManager.h"
#include "D3DShader.h"
#include "../../xcomdoc/xcomdoc.h"
#include "../../xcomdoc/xStringHash.h"
//////////////////////////////////////////////////////////////////////////
//基础代码
using namespace std;
std::wstring ToWcs(const char* _name)
{
    wchar_t v[1024] = {0};
    MultiByteToWideChar(CP_ACP , 0 , _name , (int)strlen(_name) , v , 1024);
    return v;
}

std::string ToAnsi(const wchar_t* _name)
{
    char v[1024] = {0};
    WideCharToMultiByte(CP_ACP , 0 , _name , (int)wcslen(_name) , v , 1024 , NULL , NULL);
    return v;
}

std::wstring  strFromMatrix4x4(D3DXMATRIX& out)
{
    wchar_t buf[1024];
    float* matdata = (float*)out.m;
    swprintf(buf, L"[%g,%g,%g,%g][%g,%g,%g,%g][%g,%g,%g,%g][%g,%g,%g,%g]", 
        matdata[0] , matdata[1]  , matdata[2]  , matdata[3]  , 
        matdata[4] , matdata[5]  , matdata[6]  , matdata[7]  , 
        matdata[8] , matdata[9]  , matdata[10] , matdata[11] ,
        matdata[12], matdata[13] , matdata[14] , matdata[15] );
    return std::wstring(buf);
}

std::wstring  strFromVector4(D3DXVECTOR4& point)
{
    wchar_t buf[256];
    swprintf(buf,L"%f,%f,%f,%f",point.x , point.y , point.z, point.w);
    return std::wstring(buf);
}

std::wstring  strFromFloatArray(float* out, int nValue)
{
    std::wstringstream strsteam;
    strsteam<<out[0];
    for(int i = 1 ; i < nValue ; i ++)
    {
        strsteam<<L","<<out[i];
    }
    return strsteam.str().c_str();
}

std::wstring strFromIntArray(int* out, int nValue)
{
    std::wstringstream strsteam;
    strsteam<<out[0];
    for(int i = 1 ; i < nValue ; i ++)
    {
        strsteam<<L","<<out[i];
    }
    return strsteam.str().c_str();
}

std::wstring  strFromBOOLArray(BOOL* out, int nValue)
{
    std::wstringstream strsteam;
    strsteam<<out[0];
    for(int i = 1 ; i < nValue ; i ++)
    {
        strsteam<<L","<<out[i];
    }
    return strsteam.str().c_str();
}


void  strToMatrix4x4(const wchar_t* str , D3DXMATRIX& out)
{
    float* matdata = (float*)out.m;
    swscanf(str,L"[%f,%f,%f,%f][%f,%f,%f,%f][%f,%f,%f,%f][%f,%f,%f,%f]", 
        &matdata[0] , &matdata[1]  , &matdata[2]  , &matdata[3]  , 
        &matdata[4] , &matdata[5]  , &matdata[6]  , &matdata[7]  , 
        &matdata[8] , &matdata[9]  , &matdata[10] , &matdata[11] ,
        &matdata[12], &matdata[13] , &matdata[14] , &matdata[15] );
}

void  strToVector4(const wchar_t* str, D3DXVECTOR4& point)
{
    swscanf(str,L"%f,%f,%f,%f", &point.x , &point.y , &point.z, &point.w);
}

const wchar_t* findSplitter(const wchar_t * str, wchar_t cspl)
{
    for(size_t i = 0 ; i < wcslen(str)  ; i ++)
    {
        if(str[i] == cspl) return str + i + 1;
    }
    return NULL;
}

void  strToFloatArray(const wchar_t* str, float* out, int nValue)
{
    if(nValue <= 0 ) return ;
    swscanf(str,L"%f,", &out[0]);
    const wchar_t* s1 = findSplitter(str,',');
    for(int i  = 1 ; (i < nValue)  && s1; i ++)
    {        
        swscanf(s1,L"%f,", &out[i]);
        s1 = findSplitter(s1,',');
    }
}

void  strToIntArray(const wchar_t* str, int* out, int nValue)
{
    if(nValue <= 0 ) return ;
    swscanf(str,L"%d,", &out[0]);
    const wchar_t* s1 = findSplitter(str,',');
    for(int i  = 1 ; (i < nValue)  && s1; i ++)
    {        
        swscanf(s1,L"%d,", &out[i]);
        s1 = findSplitter(s1,',');
    }
}

void  strToBOOLArray(const wchar_t* str, BOOL* out, int nValue)
{
    if(nValue <= 0 ) return ;
    swscanf(str, L"%d,", &out[0]);
    const wchar_t* s1 = findSplitter(str,',');
    for(int i  = 1 ; (i < nValue)  && s1; i ++)
    {        
        swscanf(s1, L"%d,", &out[i]);
        s1 = findSplitter(s1,',');
    }
}

std::wstring HShaderParamater::_getValueString()
{
    std::wstring value;
    switch(_class())
    {
    case eSPClass_MATRIX4x3:
        {
        }
        break;
    case eSPClass_MATRIX4x4:
        {
            value = strFromMatrix4x4( *(D3DXMATRIX*)_getParamData());
        }
        break;
    case eSPClass_FLOAT:
        {
            value = strFromFloatArray( (float*)_getParamData(),m_nElement );
        }
        break;
    case eSPClass_INT:
        {
            value = strFromIntArray( (int*)_getParamData(),m_nElement );
        }
        break;
    case eSPClass_BOOL:
        {

        }
        break;
    case eSPClass_TEXTURE2D:
    case eSPClass_TEXTURE3D:
    case eSPClass_TEXTURE1D:
    case eSPClass_TEXTURE:
        {
            HShaderTextureParam* pTexPa = (HShaderTextureParam*)_getParamData();
            value = ToWcs(pTexPa->m_TextureName);
        }
        break;
    }
    return value;

}
void HShaderParamater::_setValueString(const std::wstring& strValue)
{
    switch(_class())
    {
    case eSPClass_MATRIX4x3:
        {
        }
        break;
    case eSPClass_MATRIX4x4:
        {
            strToMatrix4x4(strValue.c_str(), *(D3DXMATRIX*)_getParamData());
        }
        break;
    case eSPClass_FLOAT:
        {
            strToFloatArray(strValue.c_str(), (float*)_getParamData(),m_nElement );
        }
        break;
    case eSPClass_INT:
        {
            strToIntArray(strValue.c_str(), (int*)_getParamData(),m_nElement );
        }
        break;
    case eSPClass_BOOL:
        {

        }
        break;
    case eSPClass_TEXTURE2D:
    case eSPClass_TEXTURE3D:
    case eSPClass_TEXTURE1D:
    case eSPClass_TEXTURE:
        {
            HShaderTextureParam* pTexPa = (HShaderTextureParam*)_getParamData();
            std::string _ansiStr = ToAnsi( strValue.c_str() );
            strncpy(pTexPa->m_TextureName, _ansiStr.c_str() , 64 );
        }
        break;
    }
}

HShaderParamater::~HShaderParamater()
{
    _releasePramaData();
}

HShaderParamater::HShaderParamater()
{
    m_PramaData = NULL;
    m_strName[0] = 0;
}

void* HShaderParamater::_createParamData()
{
    _releasePramaData();

    switch(_class())
    {
    case eSPClass_TEXTURE2D:
    case eSPClass_TEXTURE3D:
    case eSPClass_TEXTURE1D:
    case eSPClass_TEXTURE:
        {
            m_DataBytes = sizeof(HShaderTextureParam);
        }
    }

    if(m_DataBytes == 0)
    {
        m_PramaData = NULL;
        return NULL;
    }
    m_PramaData = new char[m_DataBytes];
    memset(m_PramaData,0,m_DataBytes);
    return m_PramaData;
}

void  HShaderParamater::_releasePramaData()
{
    if(m_PramaData) 
        delete [] m_PramaData;
    m_PramaData =  NULL;
}

void*  HShaderParamater::_getParamData()
{
    return m_PramaData;
}


//////////////////////////////////////////////////////////////////////////
//ParamTable

#pragma warning (disable : 4312)
//自动设置那些有确定Semantic含义的变量
int CShaderParamTable::find(const char* name)
{
	for(int i = 0 ; i < (int)m_Paramaters.size() ; ++i)
	{
		if(std::string(name) == m_Paramaters[i].m_strName ) return i;
	}
	return -1;
}

bool CShaderParamTable::release()
{
    m_Paramaters.clear();
	return true;
}

bool CShaderParamTable::load(IStreamIO* pReader)
{
    
	 int nParam = 0 ;
	 //pReader->seek(0 , xcdsd_cur);
	 //pReader->beginRead();
	 pReader->read( (char*)&nParam, sizeof(int)) ;
	 m_Paramaters.clear();
	 m_Paramaters.resize(nParam);
	 for(int i = 0 ; i < nParam ; i ++)
	 {
		  CShaderParam& param = m_Paramaters[i];
		 pReader->read( (char*)&param, sizeof(CShaderParam));
		 param.m_hHandle = NULL;
		 param._createParamData();
		 pReader->read( (char*)param.m_PramaData , param.m_DataBytes);
	 }
	 //pReader->endRead();
	 return true;
}

bool CShaderParamTable::save(IStreamIO* pWriter)
{
	int nParam = (int)m_Paramaters.size() ;
	//pWriter->seek(0);
	//pWriter->beginWrite();
	pWriter->write( (char*)&nParam, sizeof(int)) ;
	for(int i = 0 ; i < nParam ; i ++)
	{
		CShaderParam& param = m_Paramaters[i];
		pWriter->write( (char*)&param, sizeof(CShaderParam));
		pWriter->write( (char*)param.m_PramaData , param.m_DataBytes);
	}
	//pWriter->endWrite();
	return true;
}

bool CShaderD3DBase::commitParamTable()
{
     int nParam = (int)m_ParamTable.m_Paramaters.size();
	 for(int i = 0 ; i < nParam ; ++i)
	 {
          CShaderParam & param = m_ParamTable.m_Paramaters[i];
		  switch(param.m_ParamClass)
		  {
		  case eSPClass_MATRIX4x4:
			  this->setParamValue(param, (const D3DXMATRIX*)param.m_PramaData,1);
			  break;
		  case eSPClass_MATRIX3x3:
			  //this->setParamValue(param, (const zMatrix3x3*)param.m_PramaData,1);
			  break;
		  case eSPClass_FLOAT:
			  this->setParamValue(param, *(float*)param.m_PramaData);
			  break;
		  case eSPClass_FLOAT4://vector;
			  this->setParamValue(param, *(const D3DXVECTOR4*)param.m_PramaData);
			  break;
		  case eSPClass_INT:
			  this->setParamValue(param, *(int*)param.m_PramaData);
			  break;
		  case eSPClass_INT4:
			  this->setParamValue(param, (int*)param.m_PramaData,4);
			  break;
		  case eSPClass_BOOL:
			  this->setParamValue(param, *(bool*)param.m_PramaData);
			  break;
		  case eSPClass_BOOL4:
			  this->setParamValue(param, (bool*)param.m_PramaData,4);
			  break;
		  //纹理是不需要自己提交的。 

		  }
	 }
	return true;
}

eShaderParamSemantic CShaderD3DBase::parseEditorSemantic(const char* semantic)
{
    if(semantic == NULL) 
        return SPS_NOT_USED;
    if( std::string(semantic) == "EDITOR_COLOR")
    {
        return SPS_EDITOR_COLOR;
    }

    return SPS_NOT_USED;
}
eShaderParamSemantic CShaderD3DBase::parseSemantic(const char* semantic)
{
    static char * semantics []=
    {
        /*   SPS_POSITION   = 0               ,*/  "position",
        /*	 SPS_DIRECTION                    ,*/  "direction",
        /*	 SPS_DIFFUSE                      ,*/  "diffuse",
        /*	 SPS_SPECULAR                     ,*/  "specular",
        /*	 SPS_AMBIENT                      ,*/  "ambient",
        /*	 SPS_SPECULARPOWER                ,*/  "specularpower",
        /*	 SPS_EMISSIVE                     ,*/  "emissive",
        /*	 SPS_NORMAL                       ,*/  "normal",
        /*	 SPS_HEIGHT                       ,*/  "height",
        /*	 SPS_REFRACTION                   ,*/  "refraction",
        /*	 SPS_OPACITY                      ,*/  "opacity",
        /*	 SPS_ENVIRONMENT                  ,*/  "environment",
        /*	 SPS_ENVIROMENTNORMAL             ,*/  "environmentnormal",
        /*	 SPS_FRESNEL                      ,*/  "fresnel",
        /*	 SPS_WORLDMAT                     ,*/  "world",
        /*	 SPS_WORLDTRANS_MAT               ,*/  "worldtranspose",
        /*	 SPS_WORLDINV_MAT                 ,*/  "worldinverse",
        /*	 SPS_WORLDINVTRANS_MAT            ,*/  "worldinversetranspose",
        /*	 SPS_WORLDVIEW_MAT                ,*/  "worldview",
        /*	 SPS_WORLDVIEWINV_MAT             ,*/  "worldviewinverse",
        /*	 SPS_WORLDVIEWINVTRANS_MAT        ,*/  "worldviewinversetranspose",
        /*	 SPS_WVP_MAT                      ,*/  "worldviewprojection",
        /*	 SPS_WVPINV_MAT                   ,*/  "worldviewprojectioninverse",
        /*	 SPS_WVPINVTRANS_MAT              ,*/  "worldviewprojectioninversetranspose",
        /*	 SPS_VIEW_MAT                     ,*/  "view",
        /*	 SPS_VIEWTRANS_MAT                ,*/  "viewtranspose",
        /*	 SPS_VIEWPROJECTION_MAT           ,*/  "viewprojection",
        /*	 SPS_VIEWINV_MAT                  ,*/  "viewinverse",
        /*	 SPS_VIEWINVTRANS_MAT             ,*/  "viewinversetranspose",
        /*	 SPS_VIEWPROJECTIONTRANS_MAT      ,*/  "viewprojectiontranspose",           
        /*	 SPS_WORLDVIEWTRANS_MAT           ,*/  "worldviewtranspose",
        /*	 SPS_VIEWPROJECTIONINV_MAT        ,*/  "viewprojectioninverse",
        /*	 SPS_VIEWPROJECTIONINVTRANS_MAT   ,*/  "viewprojectioninversetranspose",
        /*	 SPS_PROJECTION_MAT               ,*/  "projection",
        /*	 SPS_PROJECTIONINV_MAT            ,*/  "projectioninverse",
        /*	 SPS_PROJECTIONINVTRANS_MAT       ,*/  "projectioninversetranspose",
        /*	 SPS_TEXTURE_MAT                  ,*/  "texturematrix",

        /*	 SPS_VIEWPORTPIXEL_SIZE           ,*/  "viewportpixelsize",
        /*	 SPS_VIEWPORTCLIPPING             ,*/  "viewportclipping",

        /*	 SPS_TIME                         ,*/  "time",
        /*	 SPS_LAST_TIME                    ,*/  "lasttime",
        /*	 SPS_ELAPSED_TIME                 ,*/  "elapsedtime",

        /*	 SPS_ATTENUATION                  ,*/  "attenuation",
        /*	 SPS_RENDERTARGET_COLOR           ,*/  "rendercolortarget",
        /*	 SPS_RENDERTARGET_DEPTH           ,*/  "renderdepthstenciltarget",
        /*	 SPS_RANDOM                       ,*/  "random",
        /*	 SPS_UNITSSCALE                   ,*/  "unitsscale",

        /*	 SPS_CAMERAPOSITION               ,*/  "cameraposition",

        /*	 SPS_LIGHT_UMBRA                  ,*/  "lightumbra", 
        /*	 SPS_LIGHT_PENMUBAR               ,*/  "lightpenumbra",
        /*	 SPS_LIGHT_RANGE                  ,*/  "lightrange",
        /*	 SPS_LIGHT_FALLOFF                ,*/  "lightfalloff",
        /*	 SPS_LIGHT_PROJECTION             ,*/  "lightprojection",
        /*	 SPS_LIGHT_POSITION               ,*/  "lightposition",
        /*	 SPS_LIGHT_DIFFUSE                ,*/  "lightdiffuse",
        /*	 SPS_LIGHT_SPECALAR               ,*/  "lightspecular",
        /*	 SPS_LIGHT_AMBIENT                ,*/  "lightambient",
        /*	 SPS_LIGHT_DIRECTION              ,*/  "lightdirection",
    };

    if(semantic == NULL)
        return SPS_NOT_USED;

    for(int i = 0 ; i < SPS_COUNTER ; i ++)
    {
        if(std::string(semantics[i]) == semantic )
        {
            return eShaderParamSemantic(i);
        }
    }
    return SPS_NOT_USED;
}				


bool CShaderD3DBase::commitParamTable(CShaderParamTable& table)
{
    CShaderParamTable& ParamTable = *(CShaderParamTable*)&table;
	int nParam = (int)ParamTable.m_Paramaters.size();
	for(int i = 0 ; i < nParam ; ++i)
	{
		CShaderParam & param = ParamTable.m_Paramaters[i];
		if(param.m_hHandle == NULL)
		{
			param.m_hHandle = (void*)__getD3DXHandle(param.m_strName);
		}
		if(param.m_hHandle)
			continue;

		switch(param.m_ParamClass)
		{
		case eSPClass_MATRIX4x4:
			this->setParamValue(param, (const D3DXMATRIX*)param.m_PramaData,1);
			break;
		case eSPClass_MATRIX3x3:
			//this->setParamValue(param, (const zMatrix3x3*)param.m_PramaData,1);
			break;
		case eSPClass_FLOAT:
			this->setParamValue(param, *(float*)param.m_PramaData);
			break;
		case eSPClass_FLOAT4://vector;
			this->setParamValue(param, *(const D3DXVECTOR4*)param.m_PramaData);
			break;
		case eSPClass_INT:
			this->setParamValue(param, *(int*)param.m_PramaData);
			break;
		case eSPClass_INT4:
			this->setParamValue(param, (int*)param.m_PramaData,4);
			break;
		case eSPClass_BOOL:
			this->setParamValue(param, *(bool*)param.m_PramaData);
			break;
		case eSPClass_BOOL4:
			this->setParamValue(param, (bool*)param.m_PramaData,4);
			break;
		}
	}
	return true;
}

bool CShaderD3DBase::_autoSetParamter()
{
    IDirect3DDevice9* pDevice = m_DeviceWrapper->m_pDevice;
	int nParam = (int)m_AutoSetParamTable.m_Paramaters.size();
	for(int i = 0 ; i  < nParam ; i ++)
	{
		CShaderParam & param = m_AutoSetParamTable.m_Paramaters[i];
		switch(param.m_ParamSemantic)
		{
		case SPS_POSITION:
			break;
		case SPS_DIRECTION:
			break; 
		case SPS_DIFFUSE:
			break; 
		case SPS_SPECULAR:
			break; 
		case SPS_AMBIENT:
			break; 
		case SPS_SPECULARPOWER :
			break; 
		case SPS_EMISSIVE :
			{

			}
			break;  

		case SPS_NORMAL :
			{

			}
			break;  

		case SPS_HEIGHT :
			{

			}
			break;  

		case SPS_REFRACTION :
			{

			}
			break;  

		case SPS_OPACITY :
			{

			}
			break;  

		case SPS_ENVIRONMENT :
			{

			}
			break;  

		case SPS_ENVIROMENTNORMAL :
			{

			}
			break;  

		case SPS_FRESNEL :
			{

			}
			break;  

		case SPS_WORLDMAT :
			{
                D3DXMATRIX matrix;
                pDevice->GetTransform(D3DTS_WORLD , &matrix);
                setParamValue(param,matrix );
			}
			break;  

		case SPS_WORLDTRANS_MAT :
			{
               D3DXMATRIX mat;
               pDevice->GetTransform(D3DTS_WORLD , &mat);
               D3DXMATRIX _tMat;
			   D3DXMatrixTranspose(&_tMat , &mat);
			   setParamValue(param,_tMat);
			}
			break;  

		case SPS_WORLDINV_MAT :
			{
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_WORLD , &mat);
                D3DXMATRIX _tMat;
                D3DXMatrixInverse(&_tMat , NULL, &mat);
				setParamValue(param,_tMat);
			}
			break;  

		case SPS_WORLDINVTRANS_MAT :
			{
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_WORLD , &mat);
                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat , &mat);
                D3DXMatrixInverse(&mat , NULL, &_tMat);
				setParamValue(param,mat);
			}
			break;  

		case SPS_WORLDVIEW_MAT :
			{
				D3DXMATRIX world_mat ;
				D3DXMATRIX view_mat  ;
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_WORLD , &world_mat);
                pDevice->GetTransform(D3DTS_VIEW  , &view_mat);
                mat = world_mat * view_mat;
				setParamValue(param,mat);
			}
			break;  

		case SPS_WORLDVIEWINV_MAT :
			{
                D3DXMATRIX world_mat ;
                D3DXMATRIX view_mat  ;
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_WORLD , &world_mat);
                pDevice->GetTransform(D3DTS_VIEW  , &view_mat);
                mat = world_mat * view_mat;

                D3DXMATRIX _tMat;
                D3DXMatrixInverse(&_tMat , NULL, &mat);
                setParamValue(param,_tMat);

			}
			break;  

		case SPS_WORLDVIEWINVTRANS_MAT :
			{
                D3DXMATRIX world_mat ;
                D3DXMATRIX view_mat  ;
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_WORLD , &world_mat);
                pDevice->GetTransform(D3DTS_VIEW  , &view_mat);
                mat = world_mat * view_mat;
                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat , &mat);
                D3DXMatrixInverse(&mat , NULL, &_tMat);
				setParamValue(param,mat);
			}
			break;  

		case SPS_WVP_MAT :
			{
				D3DXMATRIX world_mat ;
				D3DXMATRIX view_mat  ;
				D3DXMATRIX proj_mat  ;
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_WORLD       , &world_mat);
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                pDevice->GetTransform(D3DTS_PROJECTION  , &proj_mat);
				mat = world_mat * view_mat * proj_mat;
				setParamValue(param,mat);
			}
			break;  

		case SPS_WVPINV_MAT :
			{
                D3DXMATRIX world_mat ;
                D3DXMATRIX view_mat  ;
                D3DXMATRIX proj_mat  ;
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_WORLD       , &world_mat);
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                pDevice->GetTransform(D3DTS_PROJECTION  , &proj_mat);
                mat = world_mat * view_mat * proj_mat;

                D3DXMATRIX _tMat;
                D3DXMatrixInverse(&_tMat , NULL, &mat);
                setParamValue(param,_tMat);
			}
			break;  

		case SPS_WVPINVTRANS_MAT :
			{
                D3DXMATRIX world_mat ;
                D3DXMATRIX view_mat  ;
                D3DXMATRIX proj_mat  ;
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_WORLD       , &world_mat);
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                pDevice->GetTransform(D3DTS_PROJECTION  , &proj_mat);
                mat = world_mat * view_mat * proj_mat;

                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat ,  &mat);
                D3DXMatrixInverse(&mat , NULL, &_tMat);
                setParamValue(param,mat);
			}
			break;  

		case SPS_VIEW_MAT :
			{
				D3DXMATRIX view_mat  ;
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
				setParamValue(param,view_mat);
			}
			break;  

		case SPS_VIEWTRANS_MAT :
			{
				D3DXMATRIX view_mat ;
				pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat , &view_mat);
                setParamValue(param,_tMat);
			}
			break;  

		case SPS_VIEWINV_MAT :
			{
                D3DXMATRIX view_mat ;
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                D3DXMATRIX _tMat;
                D3DXMatrixInverse(&_tMat , NULL, &view_mat);
                setParamValue(param,_tMat);
			}
			break;  

		case SPS_VIEWINVTRANS_MAT :
			{
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_VIEW , &mat);
                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat , &mat);
                D3DXMatrixInverse(&mat , NULL , &_tMat);
                setParamValue(param,mat);
			}
			break;  

        case SPS_VIEWPROJECTION_MAT :
            {
                D3DXMATRIX view_mat  ;
                D3DXMATRIX proj_mat  ;
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                pDevice->GetTransform(D3DTS_PROJECTION  , &proj_mat);
                D3DXMATRIX mat = view_mat * proj_mat;

                setParamValue(param,mat);
            }
            break;  

		case SPS_VIEWPROJECTIONTRANS_MAT :
			{
                D3DXMATRIX view_mat  ;
                D3DXMATRIX proj_mat  ;
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                pDevice->GetTransform(D3DTS_PROJECTION  , &proj_mat);
                D3DXMATRIX mat = view_mat * proj_mat;

                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat , &mat);

				setParamValue(param,_tMat);
			}
			break;  

        case SPS_VIEWPROJECTIONINV_MAT :
            {
                D3DXMATRIX view_mat  ;
                D3DXMATRIX proj_mat  ;
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                pDevice->GetTransform(D3DTS_PROJECTION  , &proj_mat);
                D3DXMATRIX mat = view_mat * proj_mat;

                D3DXMATRIX _tMat;
                D3DXMatrixInverse(&_tMat , NULL , &mat);

                setParamValue(param,_tMat);
            }
            break;  

        case SPS_VIEWPROJECTIONINVTRANS_MAT :
            {
                D3DXMATRIX view_mat  ;
                D3DXMATRIX proj_mat  ;
                pDevice->GetTransform(D3DTS_VIEW        , &view_mat);
                pDevice->GetTransform(D3DTS_PROJECTION  , &proj_mat);
                D3DXMATRIX mat = view_mat * proj_mat;

                D3DXMATRIX _tMat;
                D3DXMatrixInverse(&_tMat , NULL , &mat);
                D3DXMatrixTranspose(&mat , &_tMat);
                setParamValue(param,mat);
            }
            break;  

		case SPS_WORLDVIEWTRANS_MAT :
			{
				D3DXMATRIX world_mat  ;
				D3DXMATRIX view_mat   ;
	
                pDevice->GetTransform(D3DTS_WORLD        , &world_mat);
                pDevice->GetTransform(D3DTS_VIEW  , &view_mat);

				D3DXMATRIX mat = world_mat * view_mat;
                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat , &mat);
                setParamValue(param,_tMat);
			}
			break;  



		case SPS_PROJECTION_MAT :
			{
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_PROJECTION , &mat);
                setParamValue(param,mat);
			}
			break;  

		case SPS_PROJECTIONINV_MAT :
			{
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_PROJECTION , &mat);
                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat , &mat);
                setParamValue(param,_tMat);
			}
			break;  

		case SPS_PROJECTIONINVTRANS_MAT :
			{
                D3DXMATRIX mat;
                pDevice->GetTransform(D3DTS_PROJECTION , &mat);
                D3DXMATRIX _tMat;
                D3DXMatrixTranspose(&_tMat , &mat);
                D3DXMatrixInverse(&mat , NULL, &_tMat);
                setParamValue(param,mat);
			}
			break;  

		case SPS_TEXTURE_MAT :
			{

			}
			break;  

		case SPS_VIEWPORTPIXEL_SIZE :
			{

			}
			break;  

		case SPS_VIEWPORTCLIPPING :
			{

			}
			break;  

		case SPS_TIME  :
			{
                float ftime = (float)ElaspedTime();
				setParamValue(param,ftime);
			}
			break;  

		case SPS_LAST_TIME :
			{

			}
			break;  

		case SPS_ELAPSED_TIME :
			{
				float ftime = (float)ElaspedTime();
				setParamValue(param,ftime);
			}
			break;  

		case SPS_ATTENUATION  :
			{

			}
			break;  

		case SPS_RENDERTARGET_COLOR :
			{

			}
			break;  

		case SPS_RENDERTARGET_DEPTH :
			{

			}
			break;  

		case SPS_RANDOM   :
			{

			}
			break;  

		case SPS_UNITSSCALE :
			{

			}
			break;  

		case SPS_CAMERAPOSITION  :
			{
                //const zPoint & camera_pos = zeusRS->getCurrentCamera()->getPosition();
				//setParamValue(param,)
			}
			break;  

		case SPS_LIGHT_UMBRA  :
			{

			}
			break;  

		case SPS_LIGHT_PENMUBAR :
			{

			}
			break;  

		case SPS_LIGHT_RANGE  :
			{

			}
			break;  

		case SPS_LIGHT_FALLOFF  :
			{

			}
			break;  

		case SPS_LIGHT_PROJECTION  :
			{

			}
			break;  

		case SPS_LIGHT_POSITION  :
			{

			}
			break;  

		case SPS_LIGHT_DIFFUSE  :
			{

			}
			break;  

		case SPS_LIGHT_SPECALAR  :
			{

			}
			break;  

		case SPS_LIGHT_AMBIENT :
			{

			}
			break;  

		case SPS_LIGHT_DIRECTION  :
			{

			}
			break;  

		default:
			//devLog("错误: 不支持的Semantic:<%d>,却被写到了列表里,请通知程序员(Stanlylee)",param.m_ParamSemantic);
			break;

		}
	}

	return false;
}

float CShaderD3DBase::ElaspedTime()
{
    return GetTickCount()/100.0f;
}

float CShaderD3DBase::frameTime()
{
    return 1000.0f/60.0f;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
eShaderParamClass CD3DShader::__parseParamClass(D3DXCONSTANT_DESC& param_desc)
{
	switch(param_desc.Class)
	{
		//提取矩阵
	case D3DXPC_MATRIX_COLUMNS:
	case D3DXPC_MATRIX_ROWS:
		{
			if(param_desc.Type != D3DXPT_FLOAT)
				return eSPClass_NotSupported;
			if(param_desc.Columns == 4 && param_desc.Rows == 4)
			{
				return eSPClass_MATRIX4x4;
			}
			if(param_desc.Columns == 4 && param_desc.Rows == 3)
			{
				return eSPClass_MATRIX4x3;
			}
			if(param_desc.Columns == 3 && param_desc.Rows == 3)
			{
				return eSPClass_MATRIX3x3;
			}
			return eSPClass_NotSupported;
		}
		break;

		//提取向量
	case D3DXPC_SCALAR:
		switch(param_desc.Type)
		{
		case D3DXPT_BOOL:
			return eSPClass_BOOL ;
			break;
		case D3DXPT_INT:
			return eSPClass_INT ;
			break;
		case D3DXPT_FLOAT:
			return eSPClass_FLOAT ;
			break;
		default:
			return eSPClass_NotSupported;
		}

		//提取向量
	case D3DXPC_VECTOR:
		switch(param_desc.Type)
		{
		case D3DXPT_BOOL:
			return (eShaderParamClass) ((int)eSPClass_BOOL + (param_desc.Columns - 1)  );
			break;
		case D3DXPT_INT:
			return (eShaderParamClass) ((int)eSPClass_INT + (param_desc.Columns - 1)   );
			break;
		case D3DXPT_FLOAT:
			return (eShaderParamClass) ((int)eSPClass_FLOAT + (param_desc.Columns - 1) );
			break;
		default:
			return eSPClass_NotSupported;
		}
	case D3DXPC_OBJECT:
		{
			switch(param_desc.Type)
			{
			case D3DXPT_TEXTURE:
				return eSPClass_TEXTURE;
				break;
			case D3DXPT_TEXTURE1D:
				return eSPClass_TEXTURE1D;
				break;
			case D3DXPT_TEXTURE2D:
				return eSPClass_TEXTURE2D;
				break;
			case D3DXPT_TEXTURE3D:
				return eSPClass_TEXTURE3D;
				break;
			case D3DXPT_TEXTURECUBE:
				return eSPClass_TEXTURECUBE;
				break;

			case D3DXPT_SAMPLER:
				return eSPClass_SAMPLER;
				break;
			case D3DXPT_SAMPLER1D:
				return eSPClass_SAMPLER1D;
				break;
			case D3DXPT_SAMPLER2D:
				return eSPClass_SAMPLER2D;
				break;
			case D3DXPT_SAMPLER3D:
				return eSPClass_SAMPLER3D;
				break;
			case D3DXPT_SAMPLERCUBE:
				return eSPClass_SAMPLERCUBE;
				break;
			default:
				return eSPClass_NotSupported;
			}
		}
		break;
	}
	return eSPClass_NotSupported;
}

int CD3DShader::__build_const_table()
{
	if(m_pConstTable == NULL)
		return 0;

	D3DXCONSTANTTABLE_DESC dxct_desc;

	m_pConstTable->GetDesc(&dxct_desc);

	int nParam = dxct_desc.Constants;

	//提取所有的Paramater
	for(int i = 0 ; i < nParam ; i ++)
	{
		D3DXHANDLE  hParam = m_pConstTable->GetConstant(NULL,i);
		D3DXCONSTANT_DESC  p_desc;
		UINT nConstant = 1;
		m_pConstTable->GetConstantDesc(hParam,&p_desc,&nConstant);
		CShaderParam d3dShaderParam;
		d3dShaderParam.m_ParamSemantic = parseSemantic(p_desc.Name);
		d3dShaderParam.m_Name = xStringHash(p_desc.Name);
		strncpy(d3dShaderParam.m_strName,p_desc.Name,32);
		d3dShaderParam.m_ParamClass = __parseParamClass(p_desc);
		d3dShaderParam.m_hHandle = (void*)hParam;
		d3dShaderParam.m_nElement = p_desc.Elements==0?1:p_desc.Elements;
		d3dShaderParam.m_DataBytes = p_desc.Bytes;
		d3dShaderParam._createParamData();

		if( d3dShaderParam.m_ParamClass <= eSPClass_SAMPLERCUBE && 
			d3dShaderParam.m_ParamClass >= eSPClass_TEXTURE)
		{
			d3dShaderParam.m_DataBytes = sizeof(HShaderTextureParam);
		}

		if(d3dShaderParam._semantic() != SPS_NOT_USED)
		{
			m_AutoSetParamTable.m_Paramaters.push_back(d3dShaderParam);
		}
		else
		{
            d3dShaderParam.m_ParamSemantic = parseEditorSemantic(p_desc.Name);
			m_ParamTable.m_Paramaters.push_back(d3dShaderParam);

			
		}
	}
	m_AutoSetParamTable.m_nParamater = (int)m_AutoSetParamTable.m_Paramaters.size();
	m_ParamTable.m_nParamater        = (int)m_ParamTable.m_Paramaters.size();
	return m_ParamTable.numOfParamater() + m_AutoSetParamTable.numOfParamater();
}

int  CD3DShader::addRef()
{
	return 1;
}
void CD3DShader::release()
{
	unload();
	delete this;
}

CShaderParamTable& CD3DShader::getAutoSetParamTable()
{
	return m_AutoSetParamTable;
}
CShaderParamTable& CD3DShader::getParamTable()
{
	return m_ParamTable;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam , float    v  )
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetFloat(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v) ) )
			return true;
	}
	return false;

}

bool CD3DShader::setParamValue(const HShaderParamater& hParam ,const  float*   v , int Count , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetFloatArray(m_DeviceWrapper->m_pDevice, hD3DXHanle,v ,Count) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam , int      v  )
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetInt(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam ,const  int*     v , int Count , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetIntArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v,Count) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam , bool      v  )
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetBool(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam ,const bool*     v , int Count , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		BOOL* array = new BOOL[Count];
		for(int i = 0 ; i < Count ; ++i) array[i] =  v[i]?TRUE:FALSE;
		if(!FAILED(m_pConstTable->SetBoolArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle,array,Count) ) )
		{
			delete array;
			return true;
		}
		delete array;
	}
	return false;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam , const D3DXVECTOR4& vec)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetVector(m_DeviceWrapper->m_pDevice ,hD3DXHanle,(D3DXVECTOR4*)&vec) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam , const D3DXVECTOR4* vecs, int nVector , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetVectorArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle,(D3DXVECTOR4*)vecs,nVector ) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam , const D3DXMATRIX& mat )
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetMatrix(m_DeviceWrapper->m_pDevice ,hD3DXHanle, (D3DXMATRIX*)&mat) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const HShaderParamater& hParam , const  D3DXMATRIX* mats, int nMat , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  (D3DXHANDLE)hParam._get_location();
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetMatrixArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle, (D3DXMATRIX*)mats , nMat )  ) )
			return true;
	}
	return false;
}


bool CD3DShader::setParamValue(const char* Name , float    v  )
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetFloat(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v) ) )
			return true;
	}
	return false;

}

bool CD3DShader::setParamValue(const char* Name ,const  float*   v , int Count , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetFloatArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v ,Count) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const char* Name , int      v  )
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetInt(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const char* Name ,const  int*     v , int Count , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetIntArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v,Count) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const char* Name , bool      v  )
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetBool(m_DeviceWrapper->m_pDevice ,hD3DXHanle,v) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const char* Name ,const bool*     v , int Count , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		BOOL* array = new BOOL[Count];
		for(int i = 0 ; i < Count ; ++i) array[i] =  v[i]?TRUE:FALSE;
		if(!FAILED(m_pConstTable->SetBoolArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle,array,Count) ) )
		{
			delete array;
			return true;
		}
		delete array;
	}
	return false;
}

bool CD3DShader::setParamValue(const char* Name , const D3DXVECTOR4& vec)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetVector(m_DeviceWrapper->m_pDevice ,hD3DXHanle,(D3DXVECTOR4*)&vec) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const char* Name , const D3DXVECTOR4* vecs, int nVector , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetVectorArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle,(D3DXVECTOR4*)vecs,nVector ) ) )
			return true;
	}
	return false;
}

bool CD3DShader::setParamValue(const char* Name , const D3DXMATRIX& mat )
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetMatrix(m_DeviceWrapper->m_pDevice ,hD3DXHanle, (D3DXMATRIX*)&mat) ) )
			return true;
	}
	return false;
}
D3DXHANDLE CD3DShader::__getD3DXHandle(const char* pConstName)
{
	return m_pConstTable->GetConstantByName(NULL,pConstName);
}

bool CD3DShader::setParamValue(const char* Name , const  D3DXMATRIX* mats, int nMat , int baseIndex)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	hD3DXHanle = m_pConstTable->GetConstantElement(hD3DXHanle, baseIndex);
	if(hD3DXHanle)
	{
		if(!FAILED(m_pConstTable->SetMatrixArray(m_DeviceWrapper->m_pDevice ,hD3DXHanle, (D3DXMATRIX*)mats , nMat )  ) )
			return true;
	}
	return false;
}

bool CD3DShader::setTexture(const char* Name                , IDirect3DTexture9* pTexture)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =  m_pConstTable->GetConstantByName(NULL,Name);
	if(hD3DXHanle)
	{
		UINT texStage = m_pConstTable->GetSamplerIndex(hD3DXHanle);
        m_DeviceWrapper->m_pDevice->SetTexture(texStage,pTexture);
        return true;
	}
	return false;
}

bool CD3DShader::setTexture(const HShaderParamater&  hParam , IDirect3DTexture9* pTexture)
{
	if(m_pConstTable == NULL)
		return false;
	D3DXHANDLE hD3DXHanle =   (D3DXHANDLE)hParam._get_location();
	if(hD3DXHanle)
	{
		UINT texStage = m_pConstTable->GetSamplerIndex(hD3DXHanle);
		m_DeviceWrapper->m_pDevice->SetTexture(texStage,pTexture);
		return true;
	}
	return false;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Pixel Shader for Direct3D
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void CPixelShader::bind()
{
	if(m_pPixelShader)
	{
		m_DeviceWrapper->m_pDevice->SetPixelShader(m_pPixelShader);
	}
}

void CPixelShader::unbind()
{
	if(m_pPixelShader)
	{
		m_DeviceWrapper->m_pDevice->SetPixelShader(NULL);
	}

}

bool CPixelShader::bValidPS(DWORD PSversion)
{
	if(PSversion >= D3DPS_VERSION(2,0))
		return true;

	return false;

}
CPixelShader::CPixelShader()
{
	m_DeviceWrapper = NULL;
	m_pPixelShader = NULL;
	m_pConstTable = NULL;
}
bool CPixelShader::load(const wchar_t* pixelShader)
{
	assert(m_DeviceWrapper);
	LPD3DXBUFFER pCode;                  // Buffer with the assembled shader code
	LPD3DXBUFFER pErrorMsgs; // Buffer with error messages
	D3DCAPS9 Caps;
	m_DeviceWrapper->m_pDevice->GetDeviceCaps(&Caps);
	if(bValidPS(Caps.PixelShaderVersion ))
	{
		HRESULT hr = D3DXCompileShaderFromFileW(pixelShader,NULL,NULL,"main","ps_2_0",0,&pCode,&pErrorMsgs,&m_pConstTable);
		if(pCode != 0)
		{
			DWORD* pCodeBuffer = NULL;
			try
			{
				pCodeBuffer = (DWORD*)pCode->GetBufferPointer();
				if(pCodeBuffer)
				{
					m_DeviceWrapper->m_pDevice->CreatePixelShader(pCodeBuffer,&m_pPixelShader);
				}
				pCode->Release();
			}
			catch(...){}
		}
		else
		{
			if(pErrorMsgs)
			{
				char* erromesg = (char*)pErrorMsgs->GetBufferPointer();
				//devLog("Loading Shader Fialed: %s\n",erromesg);
			}
		}
	}
	else
	{
		m_pPixelShader = NULL;
		m_pConstTable = NULL;
	}
	__build_const_table();
	return 1;
}

bool CPixelShader::unload()
{
	if(m_pPixelShader)
		m_pPixelShader->Release();
	if(m_pConstTable)
		m_pConstTable->Release();
	return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//

bool CVertexShaderD3D::bValidVS(DWORD VSversion)
{
	if(VSversion >= D3DVS_VERSION(2,0))
		return true;

	return false;
}

CVertexShaderD3D::CVertexShaderD3D()
{
	m_pVertexShader = NULL;
	m_pConstTable = NULL;
}

bool CVertexShaderD3D::load(const wchar_t* vertexShader)
{
	assert(m_DeviceWrapper);
	LPD3DXBUFFER pCode;                  // Buffer with the assembled shader code
	LPD3DXBUFFER pErrorMsgs;             // Buffer with error messages
	HRESULT hr = D3DXCompileShaderFromFileW(vertexShader,NULL,NULL,"main","vs_2_0",0,&pCode,&pErrorMsgs,&m_pConstTable);
	D3DCAPS9 Caps;
	m_DeviceWrapper->m_pDevice->GetDeviceCaps(&Caps);
	if(bValidVS(Caps.VertexShaderVersion ))
	{
		if(pCode != 0)
		{
			DWORD* pCodeBuffer = NULL;
			pCodeBuffer = (DWORD*)pCode->GetBufferPointer();
			if(pCodeBuffer)
			{
				m_DeviceWrapper->m_pDevice->CreateVertexShader(pCodeBuffer,&m_pVertexShader);
			}
			pCode->Release();
		}
		else
		{
			if(pErrorMsgs)
			{
				char* erromesg = (char*)pErrorMsgs->GetBufferPointer();
				//devLog("Loading Shader Fialed: %s\n",erromesg);
			}

		}
	}
	else
	{
		m_pConstTable = NULL;
		m_pVertexShader = NULL;
	}

	__build_const_table();
	return 1;
}

void CVertexShaderD3D::bind()
{
	if(m_pVertexShader)
	{
		m_DeviceWrapper->m_pDevice->SetVertexShader(m_pVertexShader);
	}
}

void CVertexShaderD3D::unbind()
{
	if(m_pVertexShader)
	{
		m_DeviceWrapper->m_pDevice->SetVertexShader(NULL);
	}
}

bool CVertexShaderD3D::unload()
{
	if(m_pVertexShader)
		m_pVertexShader->Release();
	if(m_pConstTable)
		m_pConstTable->Release();

	return true;
}





