#pragma  once

#include "VGSATLHeader.h"

// 所有需要访问 VGS渲染核心库 类的基类
class CVGSGetCore
{
public:
	CVGS2DLL *GetVGSCore(){return m_pVGSCore;}
	virtual VOID   SetVGSCore(CVGS2DLL *pVGSCore){m_pVGSCore = pVGSCore;}

public:
	CVGSGetCore(){m_pVGSCore = 0;}
	virtual ~CVGSGetCore(){m_pVGSCore = 0;}
	virtual VOID Release(){m_pVGSCore = 0;}

protected:
	CVGS2DLL *m_pVGSCore;
};
