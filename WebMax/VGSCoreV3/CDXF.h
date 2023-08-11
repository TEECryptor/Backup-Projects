//------------------------------------------------------------------------
//CDXF.h DXF�ļ�����ͷ�ļ�
//����������DXF�ļ���������
//����: lixin
//����: 2006 -05 -07
//------------------------------------------------------------------------

#ifndef _CDXF_H
#define _CDXF_H

#include <stdio.h>
#include "CManyFVF.h"
#include "CScene.h"

class CDXF  
{
public:
	CDXF();
	~CDXF();

//var
private:
	FILE*					DXFFile;
	CScene*					pScene;

	//ģ�Ͷ������ѧ����--------------------------------------------------------
	UINT					m_PolyLineCnt;		//����������
	POLYLINE*				m_pPolyLines;		//����������

	CHAR					Name[32];			//��̬DXFģ���ļ�������
	CHAR					Path[224];			//��̬DXFģ���ļ���·��
	CHAR					PathAndName[256];	//��̬DXFģ���ļ���·��������

	UINT			GetPolyLineCount();
	UINT			GetVertexCount(UINT PolyLineIndex);
	VOID			ConvertPolyLineToMesh();
	HRESULT			LoadDXFFile(char* strFileName);
	//��������
	VOID CreateMtrl();
	//��䶥��
	VOID GetVertexData(CModel* pModel, UINT index);  //index�Ƕ����߱��
	//���������
	VOID GetFaceData(CModel* pModel, UINT index);    //index�Ƕ����߱��

public:
	//ת������ά����
	UINT				faceCnt;
	UINT				vertexCnt;

public:
	VOID	Release();
	HRESULT LoadDXFFileToMesh(char* strDXFFileName, CScene* theScene);
};

#endif 
