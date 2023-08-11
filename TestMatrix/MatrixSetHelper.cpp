//----------------------------------------
//	Name:MatrixSetHelper.cpp
//	Introduce:The cpp file for class CMatrixSetHelper
//	Creator:Youfang Yao
//	Date:12-22-06
//	Version:1.0V
//----------------------------------------
#include "stdafx.h"
#include "MatrixSetHelper.h"
//----------------------------------------
//	Name:CMatrixSetHelper
//	Introduce:Constructor
//----------------------------------------
CMatrixSetHelper::CMatrixSetHelper()
 : m_iRowCnt(0),
   m_iColCnt(0),
   m_iMaxLineCnt(99999),
   m_iMaxAroundCnt(99999),
   m_iIDCnt(0),
   m_iCurIDCnt(0),
   m_pCurIDLst(NULL)
{
}
//----------------------------------------
//	Name:~CMatrixSetHelper
//	Introduce:Distructor
//----------------------------------------
CMatrixSetHelper::~CMatrixSetHelper()
{
	if(NULL != m_pCurIDLst)
	{
		delete []m_pCurIDLst;
		m_pCurIDLst = NULL;
	}

	ClearResultList();
}
//----------------------------------------
//	Name:FindCorrentIDs
//	Introduce:To sreach the corrent IDs by specified params
//----------------------------------------
int CMatrixSetHelper::FindCorrentIDs(int iRowCnt, int iColCnt, 
		int iIDCnt, int iMaxLineCnt, int iMaxAroundCnt, int *&pOut)
{
	if(iRowCnt <= 0 || iColCnt <= 0 || 
		iMaxLineCnt <= 0 || iMaxAroundCnt <= 0 ||
		iIDCnt <= 0)
		return 0;

	m_iRowCnt = iRowCnt;
	m_iColCnt = iColCnt;
	m_iIDCnt = iIDCnt;
	m_iMaxLineCnt = iMaxLineCnt;
	m_iMaxAroundCnt = iMaxAroundCnt;

	if(NULL != m_pCurIDLst)
		delete []m_pCurIDLst;
	m_pCurIDLst = new int[iIDCnt];
	m_iCurIDCnt = 0;

	ClearResultList();
	FindLegalID(m_pCurIDLst, 0, 0);

	int iResutlCnt = m_pResult.size();

	if(NULL != pOut)
		delete []pOut;	
	pOut = new int[iResutlCnt*m_iIDCnt*sizeof(int)];

	for(int i = 0; i < iResutlCnt; i++)
	{
		memcpy(pOut + i*m_iIDCnt, m_pResult[i]->pIDs, m_iIDCnt*sizeof(int));
	}

	return m_pResult.size();
}
//----------------------------------------



//----------------------------------------
//	Name:FindLegalID
//	Introduce:To find the legal IDs
//----------------------------------------
BOOL CMatrixSetHelper::FindLegalID(int* pCurLDLst, int iIndex, int iBeginID)
{
	int iTotalIDCnt = m_iRowCnt * m_iColCnt;

	//All result group have been finded
	if(iIndex == 0 && iBeginID >= (iTotalIDCnt-1))
		return TRUE;

	int iNewID = -1;
	BOOL bHasFind = FALSE;
	for(int iCurID = iBeginID; iCurID < iTotalIDCnt; iCurID++)
	{
		if(TRUE == IDIsLegal(iCurID))
		{
			iNewID = iCurID;
			bHasFind = TRUE;
			break;
		}
	}

	//Get a corrent ID
	if(TRUE == bHasFind)
	{
		//We get a group corrent IDs, and MUST save them to result list.	
		if(iIndex == m_iIDCnt-1)
		{
			pCurLDLst[iIndex] = iCurID;

			LPIDLIST lpIDList = new IDList;
			lpIDList->iCount = m_iIDCnt;
			lpIDList->pIDs = new int[m_iIDCnt];
			memcpy(lpIDList->pIDs, pCurLDLst, m_iIDCnt*sizeof(int));
			m_pResult.push_back(lpIDList);	

			m_iResultCnt = m_pResult.size();
			
			pCurLDLst[iIndex] = 0;
			return FindLegalID(pCurLDLst, iIndex, iNewID+1);
		}
		//We get a corrent ID, and MUST add it to current group selected list
		else
		{
			pCurLDLst[iIndex] = iCurID;
			iIndex++;

			m_iCurIDCnt++;
			return FindLegalID(pCurLDLst, iIndex, iNewID+1);
		}
	}
	//There are not any ID can be selectd, chang the previus ID and find continue 
	else
	{
		iIndex--;
		m_iCurIDCnt--;
		return FindLegalID(pCurLDLst, iIndex, pCurLDLst[iIndex]+1);
	}
}
//----------------------------------------
//	Name:IDIsLegal
//	Introduce:To check a ID is legal or not
//----------------------------------------
BOOL CMatrixSetHelper::IDIsLegal(int iCurID)
{
	if(m_iCurIDCnt <= 0 || NULL == m_pCurIDLst)
		return TRUE;

	if(TRUE == IDHasSelected(iCurID))
		return FALSE;

	//Try to add this ID to selected
	m_pCurIDLst[m_iCurIDCnt] = iCurID;
	m_iCurIDCnt++;

	//To check all IDs selected are legal or not
	BOOL bAllIsLegal = TRUE;
	for(int iCnt = 0; iCnt < m_iCurIDCnt; iCnt++)
	{
		int iAroundCnt = 0;
		for(UINT uDir = LD_LEFT_RIGHT; uDir < LD_COUNT; uDir++)
		{
			int iLineCnt = IDCntOnLine(m_pCurIDLst[iCnt], (LINE_DIR)uDir);
			if(iLineCnt+1 > m_iMaxLineCnt)
			{
				bAllIsLegal = FALSE;
				break;
			}
			else
			{
				iAroundCnt += iLineCnt;
			}
		}

		if((FALSE == bAllIsLegal) || (iAroundCnt+1 > m_iMaxAroundCnt))
		{
			bAllIsLegal = FALSE;
			break;
		}
	}

	//Reset the selected IDs list
	m_iCurIDCnt--;
	m_pCurIDLst[m_iCurIDCnt] = 0;

	if(FALSE == bAllIsLegal)
		return FALSE;

	return TRUE;
}
//----------------------------------------
//	Name:IDCntOnLine
//	Introduce:To calucation ID selected count on a line direction
//----------------------------------------
int	CMatrixSetHelper::IDCntOnLine(int iCenterID, LINE_DIR eDir)
{
	int iCenterRow = iCenterID / m_iRowCnt;
	int iCenterCol = iCenterID % m_iRowCnt;

	int iIDCount = 0;
	switch(eDir)
	{
	case LD_LEFT_RIGHT:
		{
			int iCurRow = iCenterRow;
			int iCurCol = iCenterCol - 1;
			for(; iCurCol >= 0; iCurCol--)
			{
				int iCurID = iCurRow*m_iColCnt + iCurCol;
				if(IDHasSelected(iCurID))
					iIDCount++;
				else
					break;
			}
			//
			iCurCol = iCenterCol + 1;
			for(; iCurCol < m_iColCnt; iCurCol++)
			{
				int iCurID = iCurRow*m_iColCnt + iCurCol;
				if(IDHasSelected(iCurID))
					iIDCount++;
				else
					break;
			}
		}
		break;
	case LD_LEFTUP_RIGHTBOTTOM:
		{			
			int iCurRow = iCenterRow - 1;
			int iCurCol = iCenterCol - 1;
			for(; iCurRow >= 0 && iCurCol >= 0; iCurRow--, iCurCol--)
			{
				int iCurID = iCurRow*m_iColCnt + iCurCol;
				if(IDHasSelected(iCurID))
					iIDCount++;
				else
					break;
			}
			//
			iCurRow = iCenterRow + 1;
			iCurCol = iCenterCol + 1;
			for(; iCurRow < m_iRowCnt && iCurCol < m_iColCnt; iCurRow++, iCurCol++)
			{
				int iCurID = iCurRow*m_iColCnt + iCurCol;
				if(IDHasSelected(iCurID))
					iIDCount++;
				else
					break;
			}
		}
		break;
	case LD_UP_BOTTOM:
		{
			int iCurRow = iCenterRow - 1;
			int iCurCol = iCenterCol;
			for(; iCurRow >= 0; iCurRow--)
			{
				int iCurID = iCurRow*m_iColCnt + iCurCol;
				if(IDHasSelected(iCurID))
					iIDCount++;
				else
					break;
			}
			//
			iCurRow = iCenterRow + 1;
			for(; iCurRow < m_iRowCnt; iCurRow++)
			{
				int iCurID = iCurRow*m_iColCnt + iCurCol;
				if(IDHasSelected(iCurID))
					iIDCount++;
				else
					break;
			}
		}
		break;	
	case LD_RIGHTUP_LEFTBOTTOM:
		{			
			int iCurRow = iCenterRow - 1;
			int iCurCol = iCenterCol + 1;
			for(; iCurRow >= 0 && iCurCol < m_iColCnt; iCurRow--, iCurCol++)
			{
				int iCurID = iCurRow*m_iColCnt + iCurCol;
				if(IDHasSelected(iCurID))
					iIDCount++;
				else
					break;
			}
			//
			iCurRow = iCenterRow + 1;
			iCurCol = iCenterCol - 1;
			for(; iCurRow < m_iRowCnt && iCurCol >= 0; iCurRow++, iCurCol--)
			{
				int iCurID = iCurRow*m_iColCnt + iCurCol;
				if(IDHasSelected(iCurID))
					iIDCount++;
				else
					break;
			}
		}
		break;
	default:
		return 0;
	}

	return iIDCount;
}
//----------------------------------------
//	Name:IDHasSelected
//	Introduce:To check a ID is selected or not
//----------------------------------------
BOOL CMatrixSetHelper::IDHasSelected(int iCurID)
{
	if(m_iCurIDCnt <= 0 || NULL == m_pCurIDLst)
		return FALSE;

	BOOL bHasSelected = FALSE;
	for(int iCnt = 0; iCnt < m_iCurIDCnt; iCnt++)
	{
		if(iCurID == m_pCurIDLst[iCnt])
		{
			bHasSelected = TRUE;
			break;
		}
	}

	return bHasSelected;
}
//----------------------------------------	
//	Name:ClearResultList
//	Introduce:To clear the result list
//----------------------------------------	
void CMatrixSetHelper::ClearResultList()
{
	LPIDLIST lpIDList = NULL;
	for(int iCnt = 0; iCnt < m_pResult.size(); iCnt++)
	{
		lpIDList = m_pResult[iCnt];
		if(NULL != lpIDList)
		{
			delete []lpIDList->pIDs;
			lpIDList->pIDs = NULL;
			//
			delete lpIDList;
			lpIDList = NULL;
		}
	}
	m_pResult.clear();
}
//----------------------------------------	