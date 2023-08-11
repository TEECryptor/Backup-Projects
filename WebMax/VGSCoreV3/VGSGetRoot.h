#pragma  once

#include "VGSHeader.h"

// 所有需要访问 资源管理器 类的基类
class VGS_EXPORT CVGSGetRoot
{
public:
	CRoot *GetRoot(){return m_pRoot;}
	virtual VOID   SetRoot(CRoot *pRoot){m_pRoot = pRoot;}

public:
	CVGSGetRoot(){m_pRoot = 0;}
	virtual ~CVGSGetRoot(){m_pRoot = 0;}
	virtual VOID Release(){m_pRoot = 0;}

protected:
	CRoot *m_pRoot;
};
