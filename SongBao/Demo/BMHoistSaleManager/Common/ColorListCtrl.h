//-----------------------------------------------
//	File Name:ColorListCtrl.h
//	Introduce:Header file
//	Date:
//------------------------------------------------
#pragma once
//------------------------------------------------
#include <map>
using namespace std;
//------------------------------------------------
typedef map<int, COLORREF> mapItemColor;
//------------------------------------------------
//	Name:CColorListCtrl
//	Introduce:Color supported list contorl
//------------------------------------------------
class CColorListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CColorListCtrl)
public:
	CColorListCtrl();
	virtual ~CColorListCtrl();
	//--------------------------------------------
protected:
	DECLARE_MESSAGE_MAP()
	//--------------------------------------------
public:
	void	SetFullItemBKColor(int iItem, COLORREF clrBK);
	void	SetFullItemBKColor(COLORREF clrBK1, COLORREF clrBK2);
	void	SetItemTextColor(COLORREF clrText);
	void	SetItemIsTitle(int iItem);
	//--------------------------------------------
private:
	mapItemColor	m_mapItemColor;
	COLORREF		m_clrText;
	COLORREF		m_clrItem1;
	COLORREF		m_clrItem2;
	//--------------------------------------------
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//--------------------------------------------
};
//------------------------------------------------


