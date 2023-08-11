// CModelGroup.h: interface for the CModelGroup class.
// ÃèÊö: Ä£ÐÍGroup
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMODELGROUP_H__AD7DD3AC_741C_481B_A431_4E610FF9353E__INCLUDED_)
#define AFX_CMODELGROUP_H__AD7DD3AC_741C_481B_A431_4E610FF9353E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CModel.h"

#include <vector>
using std::vector;

class VGS_EXPORT CModelGroup  
{
public:
	CHAR				name[32];
	INT					index;

	vector<CModel*>				model;
	vector<CModelGroup*>		modelGroup;	

public:
	void				Release();

public:
	CModelGroup();
	~CModelGroup();

};

#endif // !defined(AFX_CMODELGROUP_H__AD7DD3AC_741C_481B_A431_4E610FF9353E__INCLUDED_)
