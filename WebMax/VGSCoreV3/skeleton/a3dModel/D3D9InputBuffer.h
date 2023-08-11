#ifndef _XEVOL_DIRECT3D9_INPUT_BUFFER_H_
#define _XEVOL_DIRECT3D9_INPUT_BUFFER_H_
#include "CD3DInclude.h"
enum  eLockPolicy
{
    eLock_Read             = 1,
    eLock_Write            = 2,
    eLock_ReadWrite        = 3,
    eLock_WriteDiscard     = 4,
    eLock_WriteNoOverwrite = 5,
};


class _A3D_API_ CD3D9VertexBuffer 
{

	IDirect3DDevice9*         m_pD3D9RenderAPI;
	IDirect3DVertexBuffer9 *  m_pBuffer;
	void*                     m_pLockData;
    D3DPOOL                   m_Pool; 
    DWORD                     m_Usage; 

	size_t                    m_nElement;
	size_t                    m_nBytePerVertex;  
	size_t                    m_iBufLen;
#ifdef _DEBUG
	void*                     m_SysBuffer;
#endif
	bool _destory() ; 
public:
	IDirect3DVertexBuffer9* handle(){ return m_pBuffer ; }
	CD3D9VertexBuffer(IDirect3DDevice9* pAPI  );
	~CD3D9VertexBuffer();
	bool create(size_t nElement , size_t bytePerVertex , D3DPOOL pool , DWORD usage , void* pInitData  ) ;
	bool lock(eLockPolicy lockPolicy , void** pData);
	bool unlock();
	bool update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
	virtual size_t          nElement(){ return m_nElement ; }
	virtual size_t          bufLen(){ return m_iBufLen ; }
    void onDeviceReset(bool bNewDevice , int _Action);
    
};

class CD3D9IndexBuffer 
{
    IDirect3DDevice9*         m_pD3D9RenderAPI;
    IDirect3DIndexBuffer9 *   m_pBuffer;
    void*                     m_pLockData;
    eLockPolicy               m_lockPolicy;
    D3DPOOL                   m_Pool; 
    DWORD                     m_Usage; 

    size_t                    m_nElement;
    size_t                    m_nBytePerVertex;  
    size_t                    m_iBufLen;
#ifdef _DEBUG
	void*                     m_SysBuffer;
#endif
    bool _destory() ; 
public:
    IDirect3DIndexBuffer9* handle(){ return m_pBuffer ; }
    CD3D9IndexBuffer(IDirect3DDevice9* pAPI  );
    ~CD3D9IndexBuffer();
    bool create(size_t nElement , size_t bytePerVertex , D3DPOOL pool , DWORD usage , void* pInitData  ) ;
    bool lock(eLockPolicy lockPolicy , void** pData);
    bool unlock();
    bool update(eLockPolicy lockPolicy , void* pData , size_t dataLen);
    virtual size_t          nElement(){ return m_nElement ; }
    virtual size_t          bufLen(){ return m_iBufLen ; }
    void onDeviceReset(bool bNewDevice , int _Action);
};


#endif


