#pragma  once

#include "VGSATLHeader.h"

// ������Ҫ���� VGS��Ⱦ���Ŀ� ��Ļ���
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
