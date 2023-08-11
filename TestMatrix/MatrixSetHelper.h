//----------------------------------------
//	Name:MatrixSetHelper.h
//	Introduce:To find a corrent matrix point by specified condition
//	Creator:Youfang Yao
//	Date:12-22-06
//	Version:1.0V
//----------------------------------------
#ifndef _MATRIXSETHELPER_H_
#define	_MATRIXSETHELPER_H_
//----------------------------------------
#include <vector>
//----------------------------------------
using namespace std;
//----------------------------------------
//	Name:LINE_DIR
//	Introduce:The line direction definition
//----------------------------------------
typedef enum _LINE_DIR
{
	LD_LEFT_RIGHT			= 0,
	LD_LEFTUP_RIGHTBOTTOM	= 1,
	LD_UP_BOTTOM			= 2,
	LD_RIGHTUP_LEFTBOTTOM	= 3,
	LD_COUNT
}LINE_DIR;
//----------------------------------------
//	Name:IDList
//	Introduce:The ID list result struct definition
//----------------------------------------
typedef struct tagIDList
{
	int		iCount;
	int*	pIDs;
}IDList, *LPIDLIST;
//----------------------------------------
//	Name:CMatrixSetHelper
//	Introduce:This class help application to 
//		      find a corrent matric point by
//			  specified condition
//----------------------------------------
class CMatrixSetHelper
{
	//------------------------------------
public:
	CMatrixSetHelper();
	virtual ~CMatrixSetHelper();
	//------------------------------------
public:
	//To sreach the corrent IDs by specified params
	int		FindCorrentIDs(int iRowCnt, int iColCnt, int iIDCnt, int iMaxLineCnt, int iMaxAroundCnt, int *&pOut);
	//------------------------------------
private:
	//To find the legal IDs
	BOOL	FindLegalID(int* pCurLDLst, int iIndex, int iBeginID);
	//To check a ID is legal or not
	BOOL	IDIsLegal(int iCurID);
	//To calucation ID selected count on a line direction
	int		IDCntOnLine(int iCenterID, LINE_DIR eDir);
	//To check a ID is selected or not
	BOOL	IDHasSelected(int iCurID);	
	//To clear the result list
	void	ClearResultList();
	//------------------------------------
private:
	int		m_iRowCnt;
	int		m_iColCnt;
	int		m_iMaxLineCnt;
	int		m_iMaxAroundCnt;
	int		m_iIDCnt;
	//
	int		m_iCurIDCnt;
	int*	m_pCurIDLst;
	int					m_iResultCnt;
	vector<LPIDLIST>	m_pResult;
	//------------------------------------
};
//----------------------------------------
#endif	_MATRIXSETHELPER_H_
//----------------------------------------