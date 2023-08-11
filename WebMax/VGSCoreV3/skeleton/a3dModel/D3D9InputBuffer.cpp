#include "CD3DInclude.h"
#include "TextureManager.h"
#include "D3D9InputBuffer.h"

CD3D9VertexBuffer::CD3D9VertexBuffer( IDirect3DDevice9* pDevice  ) 
{
	m_iBufLen = 0;
	m_nBytePerVertex = 0;
	m_nElement = 0;
    m_pBuffer = NULL;
    m_pD3D9RenderAPI = pDevice;
	m_pLockData = NULL;
#ifdef _DEBUG
	m_SysBuffer = NULL;
#endif
}

CD3D9VertexBuffer::~CD3D9VertexBuffer()
{
	_destory();
}

void CD3D9VertexBuffer::onDeviceReset(bool bNewDevice , int _Action)
{
    if( !bNewDevice && m_Pool != D3DPOOL_DEFAULT)
        return ;

    if(_Action == 0)
        XSAFE_RELEASE(m_pBuffer);

    if(_Action == 1)
    {
        //新创建的纹理是不会有内容的
        create(m_nElement , m_nBytePerVertex , m_Pool , m_Usage , NULL);
    }
}

bool CD3D9VertexBuffer::create(size_t nElement , size_t bytePerVertex , D3DPOOL pool , DWORD usage , void* pData  )
{
	_destory();
	m_nElement = nElement;
	m_iBufLen = m_nElement * bytePerVertex;
	m_nBytePerVertex = bytePerVertex;

	IDirect3DDevice9* pDevice = m_pD3D9RenderAPI;
    
    m_Usage = usage;//xD3D9ConstLexer::singleton()->GetD3DUsage(pDesc->m_accessFlage , pDesc->m_bindtype , pDesc->m_usage);
    m_Pool  = pool;
    if(m_Pool == D3DPOOL_DEFAULT ) m_Usage |= D3DUSAGE_WRITEONLY;

    pDevice->CreateVertexBuffer( (UINT)m_iBufLen , m_Usage , 0 , m_Pool , &m_pBuffer , NULL );
    if(m_pBuffer == NULL)
        return false;

#ifdef _DEBUG
	m_SysBuffer = new char[m_iBufLen];
#endif

    if(pData) 
    {
        update(eLock_WriteDiscard , pData , m_iBufLen);
    }
	return true;

}


DWORD GetLockFlag(eLockPolicy lockPolicy , D3DPOOL _Pool)
{

      if(_Pool == D3DPOOL_DEFAULT) return 0;
      return 0;

      if(lockPolicy == eLock_WriteDiscard)    return D3DLOCK_DISCARD ;
      if(lockPolicy == eLock_Read)             return D3DLOCK_READONLY ;
      if(lockPolicy == eLock_WriteNoOverwrite) return D3DLOCK_NOOVERWRITE;
      return 0; 
}

bool CD3D9VertexBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
	 if(m_pBuffer == NULL)
		 return false;

     IDirect3DDevice9* pDevice = m_pD3D9RenderAPI;
     HRESULT hr = m_pBuffer->Lock(0 , (UINT)m_iBufLen , pData , GetLockFlag(lockPolicy , m_Pool) );
	 m_pLockData = *pData;
	 return SUCCEEDED(hr);
}

bool CD3D9VertexBuffer::unlock()
{
	if(m_pBuffer == NULL)
		return false;

#ifdef _DEBUG
	memcpy(m_SysBuffer , m_pLockData , m_iBufLen);
#endif
	m_pLockData = NULL;
	m_pBuffer->Unlock();
	return true ;
}

bool CD3D9VertexBuffer::update(eLockPolicy lockPolicy , void* pSrcData , size_t dataLen)
{
    if(m_pBuffer == NULL)
        return false;

    void * pData = NULL;
    IDirect3DDevice9* pDevice = m_pD3D9RenderAPI;
    HRESULT hr = m_pBuffer->Lock(0 , (UINT)dataLen , &pData , GetLockFlag(lockPolicy , m_Pool) );
    if(FAILED(hr) )
        return false;
    memcpy(pData , pSrcData , dataLen);
#ifdef _DEBUG
	memcpy(m_SysBuffer , pSrcData , dataLen);
#endif
    m_pBuffer->Unlock();
    return SUCCEEDED(hr);
}

bool CD3D9VertexBuffer::_destory()
{
#ifdef _DEBUG
	delete [] m_SysBuffer;
	m_SysBuffer = NULL;
#endif

	XSAFE_RELEASE(m_pBuffer);	
	return true;
}


//=========
CD3D9IndexBuffer::CD3D9IndexBuffer(IDirect3DDevice9* pAPI  )
{
    m_iBufLen = 0;
    m_nBytePerVertex = 0;
    m_nElement = 0;
    m_pBuffer = NULL;
    m_pD3D9RenderAPI = pAPI;
	m_pLockData = NULL;
#ifdef _DEBUG
	m_SysBuffer = NULL;
#endif
}

CD3D9IndexBuffer::~CD3D9IndexBuffer()
{
    _destory();
}

void CD3D9IndexBuffer::onDeviceReset(bool bNewDevice , int _Action)
{
    if( !bNewDevice && m_Pool != D3DPOOL_DEFAULT)
        return ;

    if(_Action == 0)
        XSAFE_RELEASE(m_pBuffer);

    if(_Action == 1)
    {
        //新创建的纹理是不会有内容的
        create(m_nElement , m_nBytePerVertex , m_Pool , m_Usage , NULL);
    }
}
bool CD3D9IndexBuffer::create(size_t nElement , size_t bytePerVertex , D3DPOOL pool , DWORD usage , void* pData  )
{
    if(bytePerVertex != 4 && bytePerVertex != 2)
        return false;

    _destory();
    m_nElement = nElement;
    m_iBufLen = m_nElement * bytePerVertex;
    m_nBytePerVertex = bytePerVertex;

    IDirect3DDevice9* pDevice = m_pD3D9RenderAPI;
    m_Usage = usage;//xD3D9ConstLexer::singleton()->GetD3DUsage(pDesc->m_accessFlage , pDesc->m_bindtype , pDesc->m_usage);
    m_Pool  = pool;
    if(m_Pool == D3DPOOL_DEFAULT ) m_Usage |= D3DUSAGE_WRITEONLY;

    D3DFORMAT fmt = bytePerVertex == 4 ? D3DFMT_INDEX32 : D3DFMT_INDEX16;
    pDevice->CreateIndexBuffer((UINT)m_iBufLen , m_Usage , fmt , m_Pool , &m_pBuffer , NULL );
    if(m_pBuffer == NULL)
        return false;

#ifdef _DEBUG
	m_SysBuffer = new char[m_iBufLen];
#endif

    if(pData) 
    {
        update(eLock_WriteDiscard , pData , m_iBufLen);
    }
    return true;

}


bool CD3D9IndexBuffer::lock(eLockPolicy lockPolicy , void** pData)
{
    if(m_pBuffer == NULL)
        return false;

    IDirect3DDevice9* pDevice = m_pD3D9RenderAPI;
    HRESULT hr = m_pBuffer->Lock(0 , (UINT)m_iBufLen , pData , GetLockFlag(lockPolicy , m_Pool) );
	m_pLockData = *pData;
    return SUCCEEDED(hr);
}

bool CD3D9IndexBuffer::unlock()
{
    if(m_pBuffer == NULL)
        return false;
	
#ifdef _DEBUG
	memcpy(m_SysBuffer , m_pLockData , m_iBufLen);
#endif
	m_pLockData = NULL;
    m_pBuffer->Unlock();
    return true ;
}

bool CD3D9IndexBuffer::update(eLockPolicy lockPolicy , void* pSrcData , size_t dataLen)
{
    if(m_pBuffer == NULL)
        return false;

    void * pData = NULL;
    IDirect3DDevice9* pDevice = m_pD3D9RenderAPI;
    HRESULT hr = m_pBuffer->Lock(0 , (UINT)dataLen , &pData , GetLockFlag(lockPolicy , m_Pool) );
    if(FAILED(hr) )
        return false;
    memcpy(pData , pSrcData , dataLen);
#ifdef _DEBUG
	memcpy(m_SysBuffer , pSrcData , dataLen);
#endif
    m_pBuffer->Unlock();
    return SUCCEEDED(hr);
}

bool CD3D9IndexBuffer::_destory()
{
    XSAFE_RELEASE(m_pBuffer);
#ifdef _DEBUG
	delete [] m_SysBuffer;
	m_SysBuffer = NULL;
#endif
    return true;
}


