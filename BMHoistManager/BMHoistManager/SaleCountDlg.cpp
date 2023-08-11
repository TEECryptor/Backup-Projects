// SaleCountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMHoistManager.h"
#include "SaleCountDlg.h"
#include <math.h>


// CSaleCountDlg dialog

IMPLEMENT_DYNAMIC(CSaleCountDlg, CDialog)

CSaleCountDlg::CSaleCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaleCountDlg::IDD, pParent)
{

}

CSaleCountDlg::~CSaleCountDlg()
{
}

void CSaleCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COUNTDLG_EQUIPMENT, m_EquipmentTypeCombox);
	DDX_Control(pDX, IDC_COMBO_COUNTDLG_SALER, m_SalerCombox);
	DDX_Control(pDX, IDC_COMBO_COUNTDLG_SHOWTYPE, m_ShowTypeCombox);
	DDX_Control(pDX, IDC_DATETIMEPICKER_COUNTDLG_START, m_StartDateCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER_COUNTDLG_END, m_EndDateCtrl);
}


BEGIN_MESSAGE_MAP(CSaleCountDlg, CDialog)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_COUNTDLG_EQUIPMENT, &CSaleCountDlg::OnCbnSelchangeComboCountdlgEquipment)
	ON_CBN_SELCHANGE(IDC_COMBO_COUNTDLG_SALER, &CSaleCountDlg::OnCbnSelchangeComboCountdlgSaler)
	ON_CBN_SELCHANGE(IDC_COMBO_COUNTDLG_SHOWTYPE, &CSaleCountDlg::OnCbnSelchangeComboCountdlgShowtype)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER_COUNTDLG_START, &CSaleCountDlg::OnDtnCloseupDatetimepickerCountdlgStart)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER_COUNTDLG_END, &CSaleCountDlg::OnDtnCloseupDatetimepickerCountdlgEnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSaleCountDlg message handlers

BOOL CSaleCountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//	设置标题
	if (eTableID_Offer == m_eCountTableID) SetWindowText(_T("报价数据统计"));
	else if (eTableID_Contract == m_eCountTableID) SetWindowText(_T("合同数据统计"));

	//	设备类型选择框
	FillEquipmentsComboBox(&m_EquipmentTypeCombox);
	m_EquipmentTypeCombox.InsertString(0, _T("所有类型"));
	m_EquipmentTypeCombox.SetCurSel(0);

	//	销售员选择框
	CString strSQL;
	strSQL.Format(L"SELECT UserName FROM %s", g_Tables[eTableID_User].strName);
	FillComboBoxFromTable(strSQL, _T("UserName"), _T(""), &m_SalerCombox);
	m_SalerCombox.InsertString(0, _T("所有销售"));
	m_SalerCombox.SetCurSel(0);

	//	显示类型选择框
	DWORD dwItemIndex;
	dwItemIndex = m_ShowTypeCombox.AddString(_T("曲线图"));
	m_ShowTypeCombox.SetItemData(dwItemIndex, 0);
	dwItemIndex = m_ShowTypeCombox.AddString(_T("柱状图"));
	m_ShowTypeCombox.SetItemData(dwItemIndex, 1);
	dwItemIndex = m_ShowTypeCombox.AddString(_T("饼状图"));
	m_ShowTypeCombox.SetItemData(dwItemIndex, 2);
	m_ShowTypeCombox.SetCurSel(0);

	//	缺省时间为当年1月至当前月
	CTime tmEnd = CTime::GetCurrentTime();
	CTime tmStart(tmEnd.GetYear(), 1, 1, 0, 0, 0);
	m_EndDateCtrl.SetTime(&tmEnd);
	m_StartDateCtrl.SetTime(&tmStart);

	//	默认统计数据
	OnCbnSelchangeComboCountdlgShowtype();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSaleCountDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}


void CSaleCountDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rcChart;
	GetDlgItem(IDC_STATIC_CHART)->GetWindowRect(&rcChart);
	ScreenToClient(&rcChart);
	rcChart.top += 20;
	rcChart.left += 5;
	rcChart.right -= 5;
	rcChart.bottom -= 5;

	HBRUSH hBrush = CreateSolidBrush(RGB(192, 192, 192));
	FillRect(dc.m_hDC, &rcChart, hBrush);

	switch(m_dwShowType)
	{
	case 0:	//曲线图
		DrawMonthDataGraphCurve(dc.m_hDC, rcChart, m_lstMonthAmountInfo);
		break;
	case 1:	//柱状图
		DrawMonthDataGraphPillar(dc.m_hDC, rcChart, m_lstMonthAmountInfo);
		break;
	case 2:	//饼状图
		DrawMonthDataGraphPieChart(dc.m_hDC, rcChart, m_lstMonthAmountInfo);
		break;
	default:
		break;
	}

	DeleteObject(hBrush);
}

void CSaleCountDlg::OnCbnSelchangeComboCountdlgEquipment()
{
	CountDataFromTable(m_eCountTableID);
	Invalidate();
	UpdateWindow();
}

void CSaleCountDlg::OnCbnSelchangeComboCountdlgSaler()
{
	CountDataFromTable(m_eCountTableID);
	Invalidate();
	UpdateWindow();
}

void CSaleCountDlg::OnCbnSelchangeComboCountdlgShowtype()
{
	int iSelIndex = m_ShowTypeCombox.GetCurSel();
	m_dwShowType = m_ShowTypeCombox.GetItemData(iSelIndex);
	CountDataFromTable(m_eCountTableID);
	Invalidate();
	UpdateWindow();
}

void CSaleCountDlg::OnDtnCloseupDatetimepickerCountdlgStart(NMHDR *pNMHDR, LRESULT *pResult)
{
	CountDataFromTable(m_eCountTableID);
	Invalidate();
	UpdateWindow();
	*pResult = 0;
}

void CSaleCountDlg::OnDtnCloseupDatetimepickerCountdlgEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	CountDataFromTable(m_eCountTableID);
	Invalidate();
	UpdateWindow();
	*pResult = 0;
}

void CSaleCountDlg::CountDataFromTable(eTableID tableID)
{
	CTime tmStart, tmEnd;
	CString strEquipment = _T("");
	CString strSaler = _T("");
	CString strStartDate = _T("");
	CString strEndDate = _T("");
	CString strSQL;
	CString strEquipmentQuery;
	CString strSalerQuery;
	CString strDateQuery;

	
	//	获取当前设置
	if (m_EquipmentTypeCombox.GetCurSel() > 0)
	{
		m_EquipmentTypeCombox.GetWindowText(strEquipment);
	}
	if (m_SalerCombox.GetCurSel() > 0)
	{
		m_SalerCombox.GetWindowText(strSaler);
	}
	m_StartDateCtrl.GetTime(tmStart);
	m_EndDateCtrl.GetTime(tmEnd);
	strStartDate.Format(_T("%d-%d-%d"), tmStart.GetYear(), tmStart.GetMonth(), tmStart.GetDay());
	strEndDate.Format(_T("%d-%d-%d"), tmEnd.GetYear(), tmEnd.GetMonth(), tmEnd.GetDay());

	//	清空老数据
	m_lstMonthAmountInfo.clear();

	//	判断参数
	if (!strEquipment.IsEmpty())
	{
		if (eTableID_Offer == tableID)	strEquipmentQuery.Format(_T("OfferType ='%s'"), strEquipment);
		else if (eTableID_Contract == tableID)	strEquipmentQuery.Format(_T("Type ='%s'"), strEquipment);
	}
	if (!strSaler.IsEmpty())
	{
		if (eTableID_Offer == tableID)	strSalerQuery.Format(_T("OfferPeople ='%s'"), strSaler);
		else if (eTableID_Contract == tableID)	strSalerQuery.Format(_T("Saler ='%s'"), strSaler);
	}

	if (eTableID_Offer == tableID) strDateQuery.Format(_T("OfferDate >= #%s# AND OfferDate <= #%s#"), strStartDate, strEndDate);
	else if (eTableID_Contract == tableID) strDateQuery.Format(_T("ContractDate >= #%s# AND ContractDate <= #%s#"), strStartDate, strEndDate);
	
	//	合成查询条件
	strSQL.Format(_T("SELECT * FROM %s"), g_Tables[tableID].strName);
	if (!strEquipmentQuery.IsEmpty())
	{
		strSQL += _T(" WHERE ");
		strSQL += strEquipmentQuery;
	}
	if (!strSalerQuery.IsEmpty())
	{
		if (!strEquipmentQuery.IsEmpty())
		{
			strSQL += _T(" AND ");
			strSQL += strSalerQuery;
		}
		else
		{
			strSQL += _T(" WHERE ");
			strSQL += strSalerQuery;
		}
	}
	if (!strDateQuery.IsEmpty())
	{
		if (!strEquipmentQuery.IsEmpty() || !strSalerQuery.IsEmpty())
		{
			strSQL += _T(" AND ");
			strSQL += strDateQuery;
		}
		else
		{
			strSQL += _T(" WHERE ");
			strSQL += strDateQuery;
		}
	}

	//	查询表	
	CADOTable table;
	BOOL bResult = FALSE;
	bResult = table.OpenTable(g_pDatabase->GetConnectionPtr(), strSQL);
	if (FALSE == bResult || table.GetRecordCnt() == 0)
	{
		return;
	}

	//	统计数据
	CTime tm, nullTm;
	float fTotalCost = 0;
	for (DWORD dwIndex = 0; dwIndex < table.GetRecordCnt(); dwIndex++)
	{
		if (eTableID_Offer == tableID) table.GetRecordCell(dwIndex, L"OfferDate", tm);
		else if (eTableID_Contract == tableID) table.GetRecordCell(dwIndex, L"ContractDate", tm);
		else continue;
		if (tm != nullTm)
		{
			CString strDate;
			float fCost = 0.0f;
			strDate.Format(_T("%d-%02d"), tm.GetYear(), tm.GetMonth());
			table.GetRecordCell(dwIndex, L"Cost", fCost);
			AddMonthAmountInfo(strDate, fCost);
			fTotalCost += fCost;
		}
	}
	
	//	显示总金额
	CString strValue;
	strValue.Format(_T("%.02f"), fTotalCost);
	SetDlgItemText(IDC_EDIT_COUNTDLG_COST, strValue);
}

void CSaleCountDlg::AddMonthAmountInfo(CString strMonth, float fMomey)
{
	BOOL bHasAdded = FALSE;
	for (lstMonthAmountInfo::iterator it = m_lstMonthAmountInfo.begin();
		it != m_lstMonthAmountInfo.end();
		it ++)
	{
		if ((*it).strMonth == strMonth)
		{
			(*it).fAmountMomey += fMomey;
			bHasAdded = TRUE;
			break;
		}
	}

	if (!bHasAdded)
	{
		MonthAmountInfo info = {0};
		info.strMonth = strMonth;
		info.fAmountMomey = fMomey;
		m_lstMonthAmountInfo.push_back(info);
	}
}

void CSaleCountDlg::DrawMonthDataGraphCurve(HDC hDC, CRect rcRect, lstMonthAmountInfo lstData)
{
	CPoint ptOld;
	CPoint ptOrg, ptY, ptX;

	//	无数据
	if (lstData.size() == 0)
	{
		MessageBox(_T("无数据"), _T("错误"), MB_OK);
		return;
	}

	int nOldMode;
	nOldMode = SetBkMode(hDC, TRANSPARENT);

	//	查找最大值和最小值
	float fMaxValue = 0;
	float fMinValue = 999999999.0f;
	for (int i = 0; i < (int)lstData.size(); i++)
	{
		if (lstData[i].fAmountMomey > fMaxValue)
		{
			fMaxValue = lstData[i].fAmountMomey;
		}
		if (lstData[i].fAmountMomey < fMinValue)
		{
			fMinValue = lstData[i].fAmountMomey;
		}
	}
	//
	int nTimes = 0;
	int nTopValue = (int)fMaxValue;
	int nBottomValue = (int)fMinValue;
	while(nTopValue > 10)
	{
		nTimes++;
		nTopValue /= 10;
	}
	nTopValue = (nTopValue+1) * (int)pow(10.0f, nTimes);
	nTimes = 0;
	while(nBottomValue > 10)
	{
		nTimes++;
		nBottomValue /= 10;
	}
	nBottomValue = (nBottomValue-1) * (int)pow(10.0f, nTimes);
	
	//	画坐标轴
	ptOrg.x = rcRect.left + 50;
	ptOrg.y = rcRect.bottom - 30;
	//
	ptY.x = ptOrg.x;
	ptY.y = rcRect.top + 30;
	//
	ptX.x = rcRect.right - 30;
	ptX.y = ptOrg.y;
	//
	MoveToEx(hDC, ptOrg.x, ptOrg.y, &ptOld);
	LineTo(hDC, ptY.x, ptY.y);
	//
	MoveToEx(hDC, ptOrg.x, ptOrg.y, &ptOld);
	LineTo(hDC, ptX.x, ptX.y);
	//
	MoveToEx(hDC, ptX.x, ptX.y, &ptOld);
	LineTo(hDC, ptX.x, ptY.y);
	
	//	画横线(金额线)
	float fStepY = (ptOrg.y - ptY.y) / 10.0f;
	float fStepValue = (nTopValue - nBottomValue) / 10.0f;
	for (int i = 0; i < 10; i++)
	{
		CPoint ptStart(ptOrg.x, int(ptOrg.y-(i+1)*fStepY));
		CPoint ptEnd(ptX.x, ptStart.y);
		MoveToEx(hDC, ptStart.x, ptStart.y, &ptOld);
		LineTo(hDC, ptEnd.x, ptEnd.y);
		//
		CRect rcText;
		CSize csText;
		TCHAR tcValue[32] = {0};
		_stprintf_s(tcValue, 32, _T("%d"), int(nBottomValue+(i+1)*fStepValue));
		GetTextExtentPoint(hDC, tcValue, _tcslen(tcValue), &csText);
		rcText.left = ptStart.x - (csText.cx + 5);
		rcText.top = ptStart.y - csText.cy/2;
		rcText.right = rcText.left + csText.cx;
		rcText.bottom = rcText.top + csText.cy;
		DrawText(hDC, tcValue, _tcslen(tcValue), &rcText, DT_CENTER|DT_VCENTER);
	}

	//	画竖线(月份线)
	float fStepX = (ptX.x - ptOrg.x) / (lstData.size() + 1.0f);
	for (int i = 0; i < (int)lstData.size(); i++)
	{
		CPoint ptStart(int(ptOrg.x + (i+1)*fStepX), ptX.y);
		CPoint ptEnd(ptStart.x, ptY.y);
		MoveToEx(hDC, ptStart.x, ptStart.y, &ptOld);
		LineTo(hDC, ptEnd.x, ptEnd.y);
		//
		CRect rcText;
		CSize csText;
		GetTextExtentPoint(hDC, lstData[i].strMonth, lstData[i].strMonth.GetLength(), &csText);
		rcText.left = ptStart.x - csText.cx / 2;
		rcText.top = ptStart.y + 5;
		rcText.right = rcText.left + csText.cx;
		rcText.bottom = rcText.top + csText.cy;
		DrawText(hDC, lstData[i].strMonth, lstData[i].strMonth.GetLength(), &rcText, DT_CENTER|DT_VCENTER);
	}

	//	画曲线
	HPEN hNewPen = CreatePen(PS_SOLID, 2, RGB(240, 95, 40));
    HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);
	HBRUSH hNewBrush = CreateSolidBrush(RGB(240, 95, 40));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hNewBrush);
	MoveToEx(hDC, ptOrg.x, ptOrg.y, &ptOld);
	for (int i = 0; i < (int)lstData.size(); i++)
	{
		int nY;
		CPoint pt;
		nY = (int)((ptOrg.y-ptY.y)*(lstData[i].fAmountMomey-nBottomValue)/(nTopValue-nBottomValue) + 0.5);
		pt = CPoint(int(ptOrg.x + (i+1)*fStepX), ptOrg.y - nY);
		LineTo(hDC, pt.x, pt.y);
		Ellipse(hDC, pt.x-5, pt.y-5, pt.x+5, pt.y+5);
	}
	SelectObject(hDC, hOldBrush);
	SelectObject(hDC, hOldPen);
	SetBkMode(hDC, nOldMode);
	DeleteObject(hNewBrush);
	DeleteObject(hNewPen);
}

void CSaleCountDlg::DrawMonthDataGraphPillar(HDC hDC, CRect rcRect, lstMonthAmountInfo lstData)
{
	CPoint ptOld;
	CPoint ptOrg, ptY, ptX;

	//	无数据
	if (lstData.size() == 0)
	{
		MessageBox(_T("无数据"), _T("错误"), MB_OK);
		return;
	}

	int nOldMode;
	nOldMode = SetBkMode(hDC, TRANSPARENT);

	//	查找到最大值和最小值
	float fMaxValue = 0;
	float fMinValue = 999999999.0f;
	for (int i = 0; i < (int)lstData.size(); i++)
	{
		if (lstData[i].fAmountMomey > fMaxValue)
		{
			fMaxValue = lstData[i].fAmountMomey;
		}
		if (lstData[i].fAmountMomey < fMinValue)
		{
			fMinValue = lstData[i].fAmountMomey;
		}
	}
	//
	int nTimes = 0;
	int nTopValue = (int)fMaxValue;
	int nBottomValue = (int)fMinValue;
	while(nTopValue > 10)
	{
		nTimes++;
		nTopValue /= 10;
	}
	nTopValue = (nTopValue+1) * (int)pow(10.0f, nTimes);
	nTimes = 0;
	while(nBottomValue > 10)
	{
		nTimes++;
		nBottomValue /= 10;
	}
	nBottomValue = (nBottomValue-1) * (int)pow(10.0f, nTimes);

	//	画坐标轴
	ptOrg.x = rcRect.left + 50;
	ptOrg.y = rcRect.bottom - 30;
	//
	ptY.x = ptOrg.x;
	ptY.y = rcRect.top + 30;
	//
	ptX.x = rcRect.right - 30;
	ptX.y = ptOrg.y;
	//
	MoveToEx(hDC, ptOrg.x, ptOrg.y, &ptOld);
	LineTo(hDC, ptY.x, ptY.y);
	//
	MoveToEx(hDC, ptOrg.x, ptOrg.y, &ptOld);
	LineTo(hDC, ptX.x, ptX.y);
	//
	MoveToEx(hDC, ptX.x, ptX.y, &ptOld);
	LineTo(hDC, ptX.x, ptY.y);
	
	//	画横线(金额线)
	float fStepY = (ptOrg.y - ptY.y) / 10.0f;
	float fStepValue = (nTopValue - nBottomValue) / 10.0f;
	for (int i = 0; i < 10; i++)
	{
		CPoint ptStart(ptOrg.x, int(ptOrg.y-(i+1)*fStepY));
		CPoint ptEnd(ptX.x, ptStart.y);
		MoveToEx(hDC, ptStart.x, ptStart.y, &ptOld);
		LineTo(hDC, ptEnd.x, ptEnd.y);
		//
		CRect rcText;
		CSize csText;
		TCHAR tcValue[32] = {0};
		_stprintf_s(tcValue, 32, _T("%d"), int(nBottomValue+(i+1)*fStepValue));
		GetTextExtentPoint(hDC, tcValue, _tcslen(tcValue), &csText);
		rcText.left = ptStart.x - (csText.cx + 5);
		rcText.top = ptStart.y - csText.cy/2;
		rcText.right = rcText.left + csText.cx;
		rcText.bottom = rcText.top + csText.cy;
		DrawText(hDC, tcValue, _tcslen(tcValue), &rcText, DT_CENTER|DT_VCENTER);
	}
	
	//	画月份柱状
	HBRUSH hBrush = CreateSolidBrush(RGB(155, 155, 255));
	HBRUSH hBorderBrush = CreateSolidBrush(RGB(0, 0, 0));
	float fStepX = (ptX.x - ptOrg.x) / (lstData.size() + 1.0f);
	for (int i = 0; i < (int)lstData.size(); i++)
	{		
		int nX, nY;
		nX = int(ptOrg.x + (i+1)*fStepX);
		nY = (int)((ptOrg.y-ptY.y)*(lstData[i].fAmountMomey-nBottomValue)/(nTopValue-nBottomValue) + 0.5);

		CRect rcPillarBorder;
		rcPillarBorder.left = nX - 10;
		rcPillarBorder.bottom = ptOrg.y;
		rcPillarBorder.right = rcPillarBorder.left + 20;
		rcPillarBorder.top = rcPillarBorder.bottom - nY;
        FillRect(hDC, &rcPillarBorder, hBorderBrush);
		//
		CRect rcPillar;
		rcPillar.left = rcPillarBorder.left + 1;
		rcPillar.bottom = rcPillarBorder.bottom;
		rcPillar.right = rcPillarBorder.right - 1;
		rcPillar.top = rcPillarBorder.top + 1;
        FillRect(hDC, &rcPillar, hBrush);
		//
		CRect rcValueText;
		CSize csValueText;
		TCHAR tcValueText[64] = {0};
		_stprintf_s(tcValueText, 64, _T("%d"), (int)lstData[i].fAmountMomey); 
		GetTextExtentPoint(hDC, tcValueText, _tcslen(tcValueText), &csValueText);
		rcValueText.left = nX - csValueText.cx / 2;
		rcValueText.bottom = rcPillar.top - 5;
		rcValueText.right = rcValueText.left + csValueText.cx;
		rcValueText.top = rcValueText.bottom - csValueText.cy;
		DrawText(hDC, tcValueText, _tcslen(tcValueText), &rcValueText, DT_CENTER|DT_VCENTER);
		//
		CRect rcText;
		CSize csText;
		GetTextExtentPoint(hDC, lstData[i].strMonth, lstData[i].strMonth.GetLength(), &csText);
		rcText.left = nX - csText.cx / 2;
		rcText.top = ptOrg.y + 5;
		rcText.right = rcText.left + csText.cx;
		rcText.bottom = rcText.top + csText.cy;
		DrawText(hDC, lstData[i].strMonth, lstData[i].strMonth.GetLength(), &rcText, DT_CENTER|DT_VCENTER);
	}	
	::DeleteObject(hBorderBrush);
	::DeleteObject(hBrush);
}

void CSaleCountDlg::DrawMonthDataGraphPieChart(HDC hDC, CRect rcRect, lstMonthAmountInfo lstData)
{
}