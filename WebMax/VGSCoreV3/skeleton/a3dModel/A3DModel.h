#ifndef __XRM_MODEL_H__
#define __XRM_MODEL_H__

#include "BaseModel.h"


class _A3D_API_ CA3DModel : public CBaseModel
{
public:
	CA3DModel(CA3DDeviceWrapper* pDevice , bool bSysMemCopy);
	~CA3DModel();
public:
	virtual bool      load(const wchar_t* fileName , const BYTE* buf , size_t bufLen, unsigned long arg);
	virtual bool      load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
public:
	bool              loadSkeleton(xcomdoc& doc , const wchar_t* _dir);
	bool              loadAction(xcomdoc& doc , const wchar_t* _dir);
	bool              loadEmbMeshs(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode);
	bool              loadEmbSkinGroup(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode);

    bool              addPackage(const wchar_t* fileName , const BYTE* buf , size_t bufLen);
protected:
	bool              _loadModelMeshMateril(CCoreMesh* mesh , xXmlNode* pMeshNode);
	void              _setupMesh( CCoreMesh* pMesh, xXmlNode* pMeshNode );
	int               _findEmbMesh(const wchar_t* _name);
    void              _addFile(xcomdoc& doc );
public:
    std::vector<xcomdoc> m_FileLists;
};


#endif
