//-----------------------------------------------------------------------
//	File Name:BMParamCalculator.cpp
//	Introduce:Class CBMParamCalculator implementation file
//-----------------------------------------------------------------------
#include "StdAfx.h"
#include <math.h>
#include "BMParamCalculator.h"
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
CBMParamCalculator::CBMParamCalculator(void)
{
	m_bOnlyHoist = FALSE;
	m_bIsEditData = FALSE;
	//
	m_eCraneType = eCraneType_GTD;
	m_bRailTypeIsOK = FALSE;
	m_iELAdditionsPrice = 0;
	//
	GetCalcParamFormDB();
}
//-----------------------------------------------------------------------
CBMParamCalculator::~CBMParamCalculator(void)
{
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
void CBMParamCalculator::UpdateBasicDrawingData()
{	
	 m_CalcDatalst.GD1 = 1000 * m_CalcDatalst.GA5;
	 m_CalcDatalst.GD2 = m_CalcDatalst.DF1;
	 m_CalcDatalst.GD3 = m_CalcDatalst.GA9;
	 m_CalcDatalst.GD4 = m_CalcDatalst.GA3 / 1000;
	 if (_T("D") == m_CalcDatalst.GA12)
	 {
		 if (m_CalcDatalst.GA3 <= 5000) m_CalcDatalst.GD5 = 80;
		 else m_CalcDatalst.GD5 = 100;
	 }
	 else
	 {
		m_CalcDatalst.GD5 = 100;
	 }
	 m_CalcDatalst.GD6 = m_CalcDatalst.GB2;
	 m_CalcDatalst.GD7 = m_CalcDatalst.GB3;
	 if (_T("D") == m_CalcDatalst.GA12)
	 {
		 if (m_CalcDatalst.GA3 <= 25000) m_CalcDatalst.GD8 = 300;
		 else m_CalcDatalst.GD8 = 400;
	 }
	 else
	 {
		m_CalcDatalst.GD8 = 200;
	 }
	 m_CalcDatalst.GD9 = m_CalcDatalst.GD7 - m_CalcDatalst.GD8;
}
//-----------------------------------------------------------------------
int CBMParamCalculator::GetGirder_h2(BOOL bForce)
{
	// Get MB7
	if (0 == m_CalcDatalst.MB7 || bForce)
	{
		if (_T("BMD") == m_CalcDatalst.GA11)
			m_CalcDatalst.MB7 = max(200, m_CalcDatalst.MA3 * 0.6);
		else
			m_CalcDatalst.MB7 = max(200, m_CalcDatalst.MA3);
	}

	// Get data based on MB7
	if (_T("BMD") == m_CalcDatalst.GA11)
	{
		m_CalcDatalst.MB71 = m_CalcDatalst.MB32 + m_CalcDatalst.MA3 + m_CalcDatalst.MA5 + m_CalcDatalst.MA7 + 200;
		m_CalcDatalst.MB72 = m_CalcDatalst.HB234 + m_CalcDatalst.HB235;
		m_CalcDatalst.MB73 = m_CalcDatalst.MA2 + 300 + 200 * m_CalcDatalst.HB8;
		m_CalcDatalst.MB74 = m_CalcDatalst.MA2 + m_CalcDatalst.MB4 + 300;
	}
	else
	{
		m_CalcDatalst.MB71 = m_CalcDatalst.MA3 + m_CalcDatalst.MA5 + m_CalcDatalst.MA7 + 30;
		m_CalcDatalst.MB72 = m_CalcDatalst.MB71 - m_CalcDatalst.MA5 + m_CalcDatalst.HB235;
		m_CalcDatalst.MB73 = m_CalcDatalst.MA2*0.5 + m_CalcDatalst.HB239 + 100 + 200*m_CalcDatalst.HB8;
		m_CalcDatalst.MB74 = m_CalcDatalst.MA2*0.5 + m_CalcDatalst.HB236 + 400;
	}	

	if (m_CalcDatalst.MB7 >= m_CalcDatalst.MA3) m_CalcDatalst.MB81 = 1;
	else m_CalcDatalst.MB81 = 2;

	return m_CalcDatalst.MB7;
}
//-----------------------------------------------------------------------
void CBMParamCalculator::UpdateMainGirderData(CString strMainGirderData)
{
	int iPos1 = strMainGirderData.Find(_T("/"));
	int iPos2 = strMainGirderData.Find(_T('/'), iPos1 + 1);

	CString strTwoValue = strMainGirderData.Left(iPos2);
	CString strOtherValue = strMainGirderData.Right(strMainGirderData.GetLength() - (iPos2+1));
	m_CalcDatalst.MA2 = GetLeftValueOfRangeString(strTwoValue);
	m_CalcDatalst.MA3 = GetRightValueOfRangeString(strTwoValue);
	//
	iPos1 = strOtherValue.Find(_T("/"));
	iPos2 = strOtherValue.Find(_T('/'), iPos1 + 1);
	strTwoValue = strOtherValue.Left(iPos2);
	strOtherValue = strOtherValue.Right(strOtherValue.GetLength() - (iPos2+1));
	m_CalcDatalst.MA4 = GetLeftValueOfRangeString(strTwoValue);
	m_CalcDatalst.MA5 = GetRightValueOfRangeString(strTwoValue);
	//
	strTwoValue = strOtherValue;
	m_CalcDatalst.MA6 = GetLeftValueOfRangeString(strTwoValue);
	m_CalcDatalst.MA7 = GetRightValueOfRangeString(strTwoValue);
	//
	if (_T("BMD") == m_CalcDatalst.GA1 || _T("BMZ") == m_CalcDatalst.GA1) 
	{
		m_CalcDatalst.MB5 = m_CalcDatalst.HD09;
		m_CalcDatalst.MB51 = m_CalcDatalst.HD10;
	}
	else if (_T("BMS") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.MB5 = m_CalcDatalst.HB231 + m_CalcDatalst.HB238;
		m_CalcDatalst.MB51 = m_CalcDatalst.HB231 + m_CalcDatalst.HB233 + m_CalcDatalst.HB237;
	}
	else	//"BMU"
	{
		m_CalcDatalst.MB5 = 0;
		m_CalcDatalst.MB51 = 0;
	}
	//
	m_CalcDatalst.MB6 = m_CalcDatalst.MA6;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::CalcMainGirder()
{
	if (FALSE == GetCalcParamFormDB())
		return FALSE;
	
	m_CalcDatalst.M011 = m_CalcDatalst.MA2 * m_CalcDatalst.MA7;
	m_CalcDatalst.M012 = m_CalcDatalst.MA2 * m_CalcDatalst.MA5;
	m_CalcDatalst.M013 = m_CalcDatalst.MA3 * m_CalcDatalst.MA4;
	m_CalcDatalst.M014 = m_CalcDatalst.MA3 * m_CalcDatalst.MA6;
	m_CalcDatalst.M01 = m_CalcDatalst.M011 + m_CalcDatalst.M012 +m_CalcDatalst.M013 + m_CalcDatalst.M014;

	m_CalcDatalst.M021 = m_CalcDatalst.MA5 + m_CalcDatalst.MA3 + m_CalcDatalst.MA7/2.0f;
	m_CalcDatalst.M022 = m_CalcDatalst.MA5 / 2.0f;
	m_CalcDatalst.M023 = m_CalcDatalst.MA5 + m_CalcDatalst.MA3/2.0f;
	m_CalcDatalst.M024 = m_CalcDatalst.MA5 + m_CalcDatalst.MA3/2.0f;
	m_CalcDatalst.M02 = (m_CalcDatalst.M011 * m_CalcDatalst.M021 + m_CalcDatalst.M012 * m_CalcDatalst.M022 + m_CalcDatalst.M013 * m_CalcDatalst.M023 + m_CalcDatalst.M014 * m_CalcDatalst.M024) / m_CalcDatalst.M01;

	m_CalcDatalst.M031 = m_CalcDatalst.MA2 / 2.0f;
	m_CalcDatalst.M032 = m_CalcDatalst.MA2 / 2.0f;
	m_CalcDatalst.M033 = 50/* + m_CalcDatalst.MA4 / 2.0f*/;
	m_CalcDatalst.M034 = m_CalcDatalst.MA2 - 50/* - m_CalcDatalst.MA6 / 2.0f*/;
	m_CalcDatalst.M03 = (m_CalcDatalst.M011 * m_CalcDatalst.M031 + m_CalcDatalst.M012 * m_CalcDatalst.M032 + m_CalcDatalst.M013 * m_CalcDatalst.M033 + m_CalcDatalst.M014 * m_CalcDatalst.M034) / m_CalcDatalst.M01;

	m_CalcDatalst.M041 = m_CalcDatalst.MA2 * powf(m_CalcDatalst.MA7, 3) / 12;
	m_CalcDatalst.M042 = m_CalcDatalst.MA2 * powf(m_CalcDatalst.MA5, 3) / 12;
	m_CalcDatalst.M043 = m_CalcDatalst.MA4 * powf(m_CalcDatalst.MA3, 3) / 12;
	m_CalcDatalst.M044 = m_CalcDatalst.MA6 * powf(m_CalcDatalst.MA3, 3) / 12;
	m_CalcDatalst.M04 = m_CalcDatalst.M041 + m_CalcDatalst.M042 +m_CalcDatalst.M043 + m_CalcDatalst.M044;

	m_CalcDatalst.M051 = m_CalcDatalst.MA7 * powf(m_CalcDatalst.MA2, 3) / 12;
	m_CalcDatalst.M052 = m_CalcDatalst.MA5 * powf(m_CalcDatalst.MA2, 3) / 12;
	m_CalcDatalst.M053 = m_CalcDatalst.MA3 * powf(m_CalcDatalst.MA4, 3) / 12;
	m_CalcDatalst.M054 = m_CalcDatalst.MA3 * powf(m_CalcDatalst.MA6, 3) / 12;
	m_CalcDatalst.M05 = m_CalcDatalst.M051 + m_CalcDatalst.M052 +m_CalcDatalst.M053 + m_CalcDatalst.M054;

	m_CalcDatalst.M06 = m_CalcDatalst.M04 + m_CalcDatalst.M011*powf((m_CalcDatalst.M021-m_CalcDatalst.M02), 2) +
											m_CalcDatalst.M012*powf((m_CalcDatalst.M022-m_CalcDatalst.M02), 2) +
											m_CalcDatalst.M013*powf((m_CalcDatalst.M023-m_CalcDatalst.M02), 2) +
											m_CalcDatalst.M014*powf((m_CalcDatalst.M024-m_CalcDatalst.M02), 2);

	m_CalcDatalst.M07 = m_CalcDatalst.M05 + m_CalcDatalst.M011*powf((m_CalcDatalst.M031-m_CalcDatalst.M03), 2) +
											m_CalcDatalst.M012*powf((m_CalcDatalst.M032-m_CalcDatalst.M03), 2) +
											m_CalcDatalst.M013*powf((m_CalcDatalst.M033-m_CalcDatalst.M03), 2) +
											m_CalcDatalst.M014*powf((m_CalcDatalst.M034-m_CalcDatalst.M03), 2);

	m_CalcDatalst.M08 =  m_CalcDatalst.M06 / m_CalcDatalst.M02;
	m_CalcDatalst.M09 =  m_CalcDatalst.M07 / m_CalcDatalst.M03;

	m_CalcDatalst.M101 =  (m_CalcDatalst.M01 * m_CalcDatalst.GA5 * m_CalcDatalst.P05) / 1000000;
	m_CalcDatalst.M102 =  long(m_CalcDatalst.GA5 / 2.0f + 0.5f);
	if (m_CalcDatalst.M102 % 2 != 0) m_CalcDatalst.M102 += 1;

	m_CalcDatalst.M103 =  (m_CalcDatalst.MA2 - 100 - m_CalcDatalst.MA4 - m_CalcDatalst.MA6) * (m_CalcDatalst.MA3 - 20);
	LONGLONG dwTemp = m_CalcDatalst.M103 * m_CalcDatalst.P07;
	dwTemp *= (LONGLONG)m_CalcDatalst.P05;
	dwTemp *= (LONGLONG)(m_CalcDatalst.M102 + 2);
	m_CalcDatalst.M104 = dwTemp  / 1000000000;
		
	CADOTable tbCalcParams;
	CString strParam_F;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s", g_tcTableName[eTableID_CalcParams]);
	BOOL bResult = tbCalcParams.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	strParam_F.Format(_T("Calc_MG_F%d"), m_CalcDatalst.MA6);
	int iIndex = GetParamIndexInTable(tbCalcParams, strParam_F);
	if (iIndex >= 0)
	{
		TCHAR tcText[64] = {0};
		TCHAR tcValue[64] = {0};
		tbCalcParams.GetRecordCell(iIndex, L"Param1", tcText, 64);
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 64);
		m_CalcDatalst.M105 =  tcText;
		m_CalcDatalst.M108 =  _tstof(tcValue);
	}

	CString strParam_H;
	strParam_H.Format(_T("Calc_MG_H%d"), GetParamHRightValue(m_CalcDatalst.MA3));
	iIndex = GetParamIndexInTable(tbCalcParams, strParam_H);
	if (iIndex >= 0)
	{
		TCHAR tcText[64] = {0};
		TCHAR tcValue[64] = {0};
		tbCalcParams.GetRecordCell(iIndex, GetParamHTextItemName(m_CalcDatalst.MA6), tcText, 64);
		tbCalcParams.GetRecordCell(iIndex, GetParamHItemName(m_CalcDatalst.MA6), tcValue, 64);
		m_CalcDatalst.M106 =  _tstol(tcValue);
		m_CalcDatalst.M107 = tcText;
	}
	tbCalcParams.CloseTable();

	m_CalcDatalst.M109 = m_CalcDatalst.M106 * 2 * (m_CalcDatalst.GA5 - 1) * m_CalcDatalst.M108;

	if (_T("BMD") == m_CalcDatalst.GA11) 
		m_CalcDatalst.M1010 = m_CalcDatalst.GA5 * m_CalcDatalst.MB34;
	else
		m_CalcDatalst.M1010 = 0;

	m_CalcDatalst.M10 = m_CalcDatalst.M101 + m_CalcDatalst.M104 + m_CalcDatalst.M109 + m_CalcDatalst.M1010;
	m_CalcDatalst.M111 = (m_CalcDatalst.GA5 - 1) * 10;
	m_CalcDatalst.M112 = (m_CalcDatalst.MB4+200) * 0.001 * 0.004 * (m_CalcDatalst.GA5 - 1) * m_CalcDatalst.P05;
	m_CalcDatalst.M113 = (m_CalcDatalst.MB4+m_CalcDatalst.MA2) * 0.001 * m_CalcDatalst.P06 * 0.001 * 0.2 * (m_CalcDatalst.M102 + 2) * m_CalcDatalst.P05;
	if (_T("BMD") == m_CalcDatalst.GA11) 
		m_CalcDatalst.M11 = m_CalcDatalst.M111 + m_CalcDatalst.M112 + m_CalcDatalst.M113;
	else
		m_CalcDatalst.M11 = 0;

	if (_T("BMD") == m_CalcDatalst.GA11) 
		m_CalcDatalst.M12 = m_CalcDatalst.M10 * 2 + m_CalcDatalst.M11;
	else
		m_CalcDatalst.M12 = m_CalcDatalst.M10;

	m_CalcDatalst.M13 = (m_CalcDatalst.M10 + m_CalcDatalst.M11) * 9.8 / m_CalcDatalst.GA5;

	if (_T("BMD") == m_CalcDatalst.GA11) 
	{
		m_CalcDatalst.M141 = (m_CalcDatalst.GA4 + m_CalcDatalst.HD01) * 9.81 * 0.5;
		m_CalcDatalst.M142 = m_CalcDatalst.M141 * m_CalcDatalst.HD10 / m_CalcDatalst.HD08;
		m_CalcDatalst.M143 = m_CalcDatalst.M141 * m_CalcDatalst.HD09 / m_CalcDatalst.HD08;
	}
	else
	{
		m_CalcDatalst.M141 = (m_CalcDatalst.GA4 + m_CalcDatalst.HD01) * 9.81;
		m_CalcDatalst.M142 = 0.5 * m_CalcDatalst.M141 * m_CalcDatalst.HD10 / m_CalcDatalst.HD08;
		m_CalcDatalst.M143 = 0.5 * m_CalcDatalst.M141 * m_CalcDatalst.HD09 / m_CalcDatalst.HD08;
	}


	m_CalcDatalst.M151 = 1.1f;
	m_CalcDatalst.M152 = m_CalcDatalst.P13 + m_CalcDatalst.P12 * m_CalcDatalst.HB220 / 60;
	m_CalcDatalst.M153 = 1.1 + 0.058 * (m_CalcDatalst.GC31/60.0f) * sqrt(m_CalcDatalst.P11);


	if (_T("C") == m_CalcDatalst.GB6) 
	{
		m_CalcDatalst.M161 = m_CalcDatalst.P02;
		m_CalcDatalst.M162 = m_CalcDatalst.P03 * 0.001;
	}
	else
	{
		m_CalcDatalst.M161 = 0;
		m_CalcDatalst.M162 = 0;
	}
	if (_T("BMD") == m_CalcDatalst.GA11) 
	{
		m_CalcDatalst.M163 = (m_CalcDatalst.GA3 + m_CalcDatalst.HD01 * m_CalcDatalst.GA2) * 9.81 * 0.5;
	}
	else
	{
		m_CalcDatalst.M163 = (m_CalcDatalst.GA3 + m_CalcDatalst.HD01 * m_CalcDatalst.GA2) * 9.81;
	}
	m_CalcDatalst.M16 = m_CalcDatalst.M153 * (m_CalcDatalst.M13 * m_CalcDatalst.GA5 * m_CalcDatalst.GA5 / 8.0f + m_CalcDatalst.M163 * (m_CalcDatalst.GA5 - m_CalcDatalst.HD09*0.001) * (m_CalcDatalst.GA5 - m_CalcDatalst.HD09*0.001) / (4 * m_CalcDatalst.GA5) + m_CalcDatalst.M161*9.81*(m_CalcDatalst.GA5 - m_CalcDatalst.M162) / 2.0f);
	m_CalcDatalst.M17 = 0.5 * m_CalcDatalst.M153 * m_CalcDatalst.M163 * (1 - m_CalcDatalst.HD09 * 0.001 / m_CalcDatalst.GA5);
	m_CalcDatalst.M18 = m_CalcDatalst.M153 * (m_CalcDatalst.M13*m_CalcDatalst.GA5*0.5 + m_CalcDatalst.M163*(m_CalcDatalst.GA5-m_CalcDatalst.MB5*0.001) / m_CalcDatalst.GA5 + m_CalcDatalst.M161*9.81*(1-m_CalcDatalst.M162/m_CalcDatalst.GA5));
	m_CalcDatalst.M19 = m_CalcDatalst.M142 * m_CalcDatalst.M153;
	m_CalcDatalst.M20 = m_CalcDatalst.M13 / 14.0f;
	m_CalcDatalst.M21 = m_CalcDatalst.M141 / 14.0f;
	m_CalcDatalst.M22 = m_CalcDatalst.M21 * m_CalcDatalst.GA5 * (1 - 1/(2*m_CalcDatalst.P19)) / 4.0f + m_CalcDatalst.M20 * m_CalcDatalst.GA5 * m_CalcDatalst.GA5 * (1-2/(3*m_CalcDatalst.P19)) / 8.0f;
	m_CalcDatalst.M23 = 0.5 * m_CalcDatalst.M20 * m_CalcDatalst.GA5 + m_CalcDatalst.M21 * (1-m_CalcDatalst.MB5*0.001/m_CalcDatalst.GA5);
	
	if (_T("BMD") == m_CalcDatalst.GA11) 
	{
		m_CalcDatalst.M241 = (m_CalcDatalst.M13 * m_CalcDatalst.GA5 + m_CalcDatalst.M163) + m_CalcDatalst.M10 * m_CalcDatalst.P20 * 9.81 + m_CalcDatalst.M161 * (1-m_CalcDatalst.M162) / m_CalcDatalst.GA5 * 9.81;
	}
	else
	{
		m_CalcDatalst.M241 = 0.5 * (m_CalcDatalst.M13*m_CalcDatalst.GA5 + m_CalcDatalst.M141) + m_CalcDatalst.M10 * m_CalcDatalst.P20 * 9.81;
	}
	m_CalcDatalst.M24 = 0.5 * m_CalcDatalst.M241 * m_CalcDatalst.P22; 
	m_CalcDatalst.M25 = 0.5 * m_CalcDatalst.M24 * m_CalcDatalst.GA5 * m_CalcDatalst.P21; 
	m_CalcDatalst.M26 = m_CalcDatalst.M22 + m_CalcDatalst.M25; 
	m_CalcDatalst.M271 = m_CalcDatalst.M16 * 1000 / m_CalcDatalst.M08; 
	m_CalcDatalst.M272 = m_CalcDatalst.M26 * 1000 / m_CalcDatalst.M09; 
	m_CalcDatalst.M273 = m_CalcDatalst.M271 + m_CalcDatalst.M272; 


	if (_T("BMS") == m_CalcDatalst.GA11)
	{
		m_CalcDatalst.M281 = m_CalcDatalst.M26 * 1000 * (m_CalcDatalst.M03 - 8) / m_CalcDatalst.M07;
		m_CalcDatalst.M282 = m_CalcDatalst.P16 * m_CalcDatalst.M19 / (m_CalcDatalst.MA5 * m_CalcDatalst.MA5);
		m_CalcDatalst.M283 = m_CalcDatalst.P17 * m_CalcDatalst.M19 / (m_CalcDatalst.MA5 * m_CalcDatalst.MA5);
		m_CalcDatalst.M284 = m_CalcDatalst.P18 * m_CalcDatalst.M19 / (m_CalcDatalst.MA5 * m_CalcDatalst.MA5);
		m_CalcDatalst.M285 = sqrtf(powf(m_CalcDatalst.M271 + m_CalcDatalst.M281 + m_CalcDatalst.M283, 2) + powf(m_CalcDatalst.M282, 2) - (m_CalcDatalst.M271 + m_CalcDatalst.M281 + m_CalcDatalst.M283) * m_CalcDatalst.M282);
		m_CalcDatalst.M286 = m_CalcDatalst.M271 + m_CalcDatalst.M272 + m_CalcDatalst.M284;		
	}
	else
	{
		m_CalcDatalst.M291 = m_CalcDatalst.MA7 * m_CalcDatalst.MA2 * (m_CalcDatalst.M02 - 0.5 * m_CalcDatalst.MA7);
		m_CalcDatalst.M292 = m_CalcDatalst.M17 * m_CalcDatalst.M291 / (m_CalcDatalst.M06 * (m_CalcDatalst.MA4 + m_CalcDatalst.MA6));
		m_CalcDatalst.M293 = m_CalcDatalst.M19 / ((2*m_CalcDatalst.MA7 + 2*m_CalcDatalst.MB32 + 50) * m_CalcDatalst.MA6);
		m_CalcDatalst.M294 = sqrtf(powf(m_CalcDatalst.M273, 2) + powf(m_CalcDatalst.M293, 2) - m_CalcDatalst.M273 * m_CalcDatalst.M293 - 3 * m_CalcDatalst.M292 * m_CalcDatalst.M292);
		m_CalcDatalst.M295 = m_CalcDatalst.M273;
	}

	m_CalcDatalst.M30 = 1.5f * m_CalcDatalst.M18 / (2 * m_CalcDatalst.MB6 * m_CalcDatalst.MB7);
	m_CalcDatalst.M31 = m_CalcDatalst.M141 * powf(m_CalcDatalst.GA5*1000, 3) / (48*2.1*100000*m_CalcDatalst.M06);
	m_CalcDatalst.M32 = (m_CalcDatalst.M21 * powf(m_CalcDatalst.GA5*1000, 3) / (48*2.1*100000*m_CalcDatalst.M07)) *
						(1-3/(4*m_CalcDatalst.P19) + 5*m_CalcDatalst.M20 * m_CalcDatalst.GA5 * (1-4/(5*m_CalcDatalst.P19)) / (8*m_CalcDatalst.M21));

	m_CalcDatalst.M331 = m_CalcDatalst.MB21 / m_CalcDatalst.P14;
	m_CalcDatalst.M332 = m_CalcDatalst.M331 / sqrtf(3.0f);
	m_CalcDatalst.M333 = (m_CalcDatalst.GA5 * 1000) * m_CalcDatalst.P081;
	m_CalcDatalst.M334 = (m_CalcDatalst.GA5 * 1000) / 2000.0f;

	m_CalcDatalst.MC1 = m_CalcDatalst.M273;
	if (_T("BMD") == m_CalcDatalst.GA11) 
	{
		m_CalcDatalst.MC2 = m_CalcDatalst.M294;
		m_CalcDatalst.MC3 = m_CalcDatalst.M295;
	}
	else
	{
		m_CalcDatalst.MC2 = m_CalcDatalst.M285;
		m_CalcDatalst.MC3 = m_CalcDatalst.M286;
	}
	m_CalcDatalst.MC4 = m_CalcDatalst.M30;
	m_CalcDatalst.MC5 = m_CalcDatalst.M31;
	m_CalcDatalst.MC6 = m_CalcDatalst.M32;

	m_CalcDatalst.MD1 = m_CalcDatalst.MC1 / m_CalcDatalst.M331;
	m_CalcDatalst.MD2 = m_CalcDatalst.MC2 / m_CalcDatalst.M331;
	m_CalcDatalst.MD3 = m_CalcDatalst.MC3 / m_CalcDatalst.M331;
	m_CalcDatalst.MD4 = m_CalcDatalst.MC4 / m_CalcDatalst.M332;
	m_CalcDatalst.MD5 = m_CalcDatalst.MC5 / m_CalcDatalst.M333;
	m_CalcDatalst.MD6 = m_CalcDatalst.MC6 / m_CalcDatalst.M334;


	m_CalcDatalst.ME1.Format(_T("%d/%d/%d/%d/%d/%d"), m_CalcDatalst.MA2, m_CalcDatalst.MA3, m_CalcDatalst.MA4, m_CalcDatalst.MA5, m_CalcDatalst.MA6, m_CalcDatalst.MA7);
	m_CalcDatalst.ME2 = m_CalcDatalst.M10;
	m_CalcDatalst.ME3 = m_CalcDatalst.M11;
	m_CalcDatalst.ME4 = m_CalcDatalst.M1010;
	m_CalcDatalst.ME5 = m_CalcDatalst.M12;
	m_CalcDatalst.ME6 = m_CalcDatalst.M105;
	m_CalcDatalst.ME7 = m_CalcDatalst.M106;
	m_CalcDatalst.ME8 = m_CalcDatalst.M107;
	m_CalcDatalst.ME9 = m_CalcDatalst.P10;
	m_CalcDatalst.MF1 = int(m_CalcDatalst.ME5 * m_CalcDatalst.ME9 / 1000.0f);

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateRailData()
{	
	CString strSQL;
	CADOTable tbWheWidDat;
	strSQL.Format(_T("SELECT * FROM %s WHERE WheBloTyp='%s'"), g_tcTableName[eTableID_WheWidDat], m_CalcDatalst.DB33);
	if (tbWheWidDat.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbWheWidDat.GetRecordCnt() > 0)
	{
		TCHAR tcTemp[256] = {0};
		tbWheWidDat.GetRecordCell(0, _T("EndSidLen"), (DWORD&)m_CalcDatalst.DC51);
		tbWheWidDat.GetRecordCell(0, _T("WheRaiF"), m_CalcDatalst.DC52);
		tbWheWidDat.GetRecordCell(0, _T("WheShDim"), (DWORD&)m_CalcDatalst.DC53);
		tbWheWidDat.GetRecordCell(0, _T("WheFw1"), m_CalcDatalst.DC54);
		tbWheWidDat.GetRecordCell(0, _T("RaiWidMax"), (DWORD&)m_CalcDatalst.DC55);
		tbWheWidDat.GetRecordCell(0, _T("BeaShaDia"), (DWORD&)m_CalcDatalst.DC56);
		tbWheWidDat.GetRecordCell(0, _T("BeaTye"), tcTemp, 256); m_CalcDatalst.DC57 = tcTemp;
		tbWheWidDat.GetRecordCell(0, _T("WheWidMax"), (DWORD&)m_CalcDatalst.DC58);
		tbWheWidDat.GetRecordCell(0, _T("WheFw2"), m_CalcDatalst.DC59);
	}	

	tbWheWidDat.CloseTable();
	
	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateRailDataR04R05()
{
	m_CalcDatalst.RA041.Format(_T("%d%d"), m_CalcDatalst.RA02, m_CalcDatalst.RA031);
	
	CADOTable table;	
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE WheCode = '%s'", g_tcTableName[eTableID_WheLoaCal], m_CalcDatalst.RA041);
	BOOL bResult = table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (TRUE == bResult && table.GetRecordCnt() > 0)
	{
		DWORD dwTemp = 0;
		table.GetRecordCell(0, _T("Par2"), dwTemp);
		table.GetRecordCell(0, _T("Par1"), m_CalcDatalst.RA05);
		m_CalcDatalst.RA04 = dwTemp;
	}
	table.CloseTable();

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateRailDataR08R09()
{
	CADOTable table;	
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s WHERE RaiTyp = '%s'", g_tcTableName[eTableID_DatRai], m_CalcDatalst.RA01);
	BOOL bResult = table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (TRUE == bResult && table.GetRecordCnt() > 0)
	{
		table.GetRecordCell(0, _T("RaiWidEff"), (DWORD&)m_CalcDatalst.RA08);
		table.GetRecordCell(0, _T("WheLoaCalEff"), m_CalcDatalst.RA09);
	}
	table.CloseTable();

	return TRUE;
}
BOOL CBMParamCalculator::UpdateRailDataR10R11R13()
{
	m_CalcDatalst.RA10 = (m_CalcDatalst.RA04*m_CalcDatalst.RA08*m_CalcDatalst.RA09) / 1000;
	m_CalcDatalst.RA11 = (m_CalcDatalst.RA05*m_CalcDatalst.RA08*m_CalcDatalst.RA09) / 1000;
	m_CalcDatalst.RA13 = (m_CalcDatalst.RA06*2+m_CalcDatalst.RA07) / 3;

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateRailTypeData(BOOL bIsEndCarOption)
{
	CString strSQL;
	CADOTable tbSearch;
	if (FALSE == bIsEndCarOption)
	{
		strSQL.Format(_T("SELECT * FROM %s WHERE RaiTyp='%s'"), g_tcTableName[eTableID_DatRai], m_CalcDatalst.MB3);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{		
			tbSearch.GetRecordCell(0, L"RaiWidTop", (DWORD&)m_CalcDatalst.MB31);
			tbSearch.GetRecordCell(0, L"RaiHgt", (DWORD&)m_CalcDatalst.MB32);
			tbSearch.GetRecordCell(0, L"RaiWidEff", (DWORD&)m_CalcDatalst.MB33);
			tbSearch.GetRecordCell(0, L"WgtRai", (DWORD&)m_CalcDatalst.MB34);
			tbSearch.GetRecordCell(0, L"MatSte", (DWORD&)m_CalcDatalst.MB35);
			tbSearch.GetRecordCell(0, L"WheLoaCalEff", m_CalcDatalst.MB36);
		}
	}
	else
	{
		strSQL.Format(_T("SELECT * FROM %s WHERE RaiTyp='%s'"), g_tcTableName[eTableID_DatRai], m_CalcDatalst.DA4);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			tbSearch.GetRecordCell(0, _T("RaiWidTop"), (DWORD&)m_CalcDatalst.DA41);
			tbSearch.GetRecordCell(0, _T("RaiHgt"), (DWORD&)m_CalcDatalst.DA42);
			tbSearch.GetRecordCell(0, _T("RaiWidEff"), (DWORD&)m_CalcDatalst.DA43);
			tbSearch.GetRecordCell(0, _T("WgtRai"), (DWORD&)m_CalcDatalst.DA44);
			tbSearch.GetRecordCell(0, _T("MatSte"), (DWORD&)m_CalcDatalst.DA45);
			tbSearch.GetRecordCell(0, _T("WheLoadCalEff"), (DWORD&)m_CalcDatalst.DA46);
		}
	}
	tbSearch.CloseTable();

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateRailGaugeData()
{
	if (_T("BMD") == m_CalcDatalst.GA11) 
	{
		long lGaiGuage = _ttol(m_CalcDatalst.DA5) / 100;
		m_CalcDatalst.DA51.Format(_T("%d"), lGaiGuage);
	}
	else 
	{
		m_CalcDatalst.DA51 = _T("00");
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateJointPlateData()
{
	CString strSQL;
	CADOTable table;
	strSQL.Format(_T("SELECT * FROM %s WHERE WheelDim='%s'"), g_tcTableName[eTableID_JoiPlaDat], m_CalcDatalst.DB33);
	if (table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && table.GetRecordCnt() > 0)
	{
		TCHAR tcTemp[64] = {0};
		table.GetRecordCell(0, _T("JoiPlaTyp"), tcTemp, 64); m_CalcDatalst.DB51 = tcTemp;
		table.GetRecordCell(0, _T("JoiPlaSide"), (DWORD&)m_CalcDatalst.DB52);
		table.GetRecordCell(0, _T("EcEc27"), (DWORD&)m_CalcDatalst.DB53);
		table.GetRecordCell(0, _T("EcEc30"), (DWORD&)m_CalcDatalst.DB54);
		table.GetRecordCell(0, _T("EcEc17"), (DWORD&)m_CalcDatalst.DB55);
		table.GetRecordCell(0, _T("EcEc21"), (DWORD&)m_CalcDatalst.DB56);
		table.GetRecordCell(0, _T("EcEc26"), (DWORD&)m_CalcDatalst.DB57);
		table.GetRecordCell(0, _T("EcEc32"), (DWORD&)m_CalcDatalst.DB58);
		table.GetRecordCell(0, _T("JoiPlath"), (DWORD&)m_CalcDatalst.DB59);
	}
	table.CloseTable();
	
	switch(m_CalcDatalst.MA2)
	{
	case 300:
		m_CalcDatalst.DB510 = 3;
		break;
	case 410:
		m_CalcDatalst.DB510 = 4;
		break;
	case 490:
		m_CalcDatalst.DB510 = 5;
		break;
	case 610:
		m_CalcDatalst.DB510 = 6;
		break;
	case 740:
		m_CalcDatalst.DB510 = 7;
		break;
	case 860:
		m_CalcDatalst.DB510 = 8;
		break;
	}

	 m_CalcDatalst.DB5.Format(_T("%s%d"), m_CalcDatalst.DB51, m_CalcDatalst.DB510);

	 return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateDriverTypeData()
{	
	CString strSQL;
	CADOTable table;
	strSQL.Format(_T("SELECT * FROM %s WHERE WheCode='%s' AND TraDriTyp='%s'"), g_tcTableName[eTableID_WheMot], m_CalcDatalst.DB21, m_CalcDatalst.DB3);
	if (table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && table.GetRecordCnt() > 0)
	{
		TCHAR tcTemp[64] = {0};
		table.GetRecordCell(0, _T("GeaTyp2"), tcTemp, 64); m_CalcDatalst.DB31 = tcTemp;
		table.GetRecordCell(0, _T("WTCode"), tcTemp, 64); m_CalcDatalst.DB32 = tcTemp;
		m_CalcDatalst.DB33 = m_CalcDatalst.DB31 + m_CalcDatalst.DB21;
		table.GetRecordCell(0, _T("DiaWhe"), (DWORD&)m_CalcDatalst.DB34);
		table.GetRecordCell(0, _T("PowTraDri"), m_CalcDatalst.DB35);
		table.GetRecordCell(0, _T("GeaTyp"), tcTemp, 64); m_CalcDatalst.DB36 = tcTemp;
		table.GetRecordCell(0, _T("DriLoa"), m_CalcDatalst.DB37);
		table.GetRecordCell(0, _T("SpeTraMax"), (DWORD&)m_CalcDatalst.DB38);
		table.GetRecordCell(0, _T("GeaRat"), m_CalcDatalst.DB39);
		m_CalcDatalst.DC3 = m_CalcDatalst.DB38;
		m_CalcDatalst.DC4 = m_CalcDatalst.DB37 * m_CalcDatalst.DB4 / 2;
	}
	table.CloseTable();
	//
	strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_CalcDatalst.DB3);
	if (table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && table.GetRecordCnt() > 0)
	{
		table.GetRecordCell(0, _T("TroRoSpeed"), (DWORD&)m_CalcDatalst.DB310);
		table.GetRecordCell(0, _T("EcEc181"), (DWORD&)m_CalcDatalst.DB311);
		table.GetRecordCell(0, _T("EcEc23"), (DWORD&)m_CalcDatalst.DB312);
		table.GetRecordCell(0, _T("EcEc29"), (DWORD&)m_CalcDatalst.DB313);
		table.GetRecordCell(0, _T("DriWei"), (DWORD&)m_CalcDatalst.DB314);
		table.GetRecordCell(0, _T("PriTraDriRmb"), (DWORD&)m_CalcDatalst.DB315);
	}
	table.CloseTable();

	 return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateWheelBaseData(BOOL bForced)
{	
	m_CalcDatalst.DB81 = m_CalcDatalst.DB22 * 100;

	CString strSQL;
	CADOTable table;
	strSQL.Format(_T("SELECT * FROM %s WHERE BEndCar='%s'"), g_tcTableName[eTableID_EndCarBm], m_CalcDatalst.DB2);
	if (table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && table.GetRecordCnt() > 0)
	{
		table.GetRecordCell(0, _T("Msize"), (DWORD&)m_CalcDatalst.DB82);
		table.GetRecordCell(0, _T("Jmin"), (DWORD&)m_CalcDatalst.DB83);
		table.GetRecordCell(0, _T("Wmin"), (DWORD&)m_CalcDatalst.DB84);
	}
	else
	{
		m_CalcDatalst.DB82 = 0;
		m_CalcDatalst.DB83 = 0;
		m_CalcDatalst.DB84 = 0;
	}
	table.CloseTable();

	if (TRUE == bForced)
	{
		switch(m_CalcDatalst.DB12.GetAt(0))
		{
		case _T('B'):
			m_CalcDatalst.DB8 = m_CalcDatalst.DB81 * 2 + _ttol(m_CalcDatalst.DA5) - m_CalcDatalst.DB82 * 2;
			break;
		case _T('D'):
			m_CalcDatalst.DB8 = m_CalcDatalst.DB81;
			break;
		case _T('S'):
			m_CalcDatalst.DB8 = m_CalcDatalst.DB81;
			break;
		case _T('U'):
			m_CalcDatalst.DB8 = m_CalcDatalst.DB81;
			break;
		}
	}

	if (_T("B") == m_CalcDatalst.DB12)
	{
		m_CalcDatalst.DC6 = _ttol(m_CalcDatalst.DA5) - 2 * (m_CalcDatalst.DB82 + m_CalcDatalst.DB83); 
		m_CalcDatalst.DC61.Format(_T("%04d"), m_CalcDatalst.DC6);
	}
	else
	{
		m_CalcDatalst.DC6 = 0;
		m_CalcDatalst.DC61 = _T("0000");
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateBufferExtendData()
{
	CString strSQL;
	CADOTable table;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodBufSel='%s'"), g_tcTableName[eTableID_Buffer], m_CalcDatalst.DB24);
	if (table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && table.GetRecordCnt() > 0)
	{
		TCHAR tcTemp[256] = {0};
		table.GetRecordCell(0, _T("BufTyp"), tcTemp, 256); m_CalcDatalst.DC71 = tcTemp;
		table.GetRecordCell(0, _T("BufTypExt"), tcTemp, 256); m_CalcDatalst.DC72 = tcTemp;
		table.GetRecordCell(0, _T("BufDia"), (DWORD&)m_CalcDatalst.DC73);
		table.GetRecordCell(0, _T("BufLen"), (DWORD&)m_CalcDatalst.DC74);
		table.GetRecordCell(0, _T("BufIntFac"), tcTemp, 256); m_CalcDatalst.DC75 = tcTemp;
	}
	table.CloseTable();
	m_CalcDatalst.DC76 = m_CalcDatalst.DB8 + 2*m_CalcDatalst.DC51 + 2*m_CalcDatalst.DC74;
	m_CalcDatalst.DC77 = m_CalcDatalst.MA2 + m_CalcDatalst.MB4 + 350 - (m_CalcDatalst.DC76-_ttol(m_CalcDatalst.DA5))/2.0f;

	if (_T("S") == m_CalcDatalst.DB12 ||
		_T("U") == m_CalcDatalst.DB12)
	{
		m_CalcDatalst.DC7 = 0;
	}
	else
	{
		if (m_CalcDatalst.DC77 <= 0) m_CalcDatalst.DC7 = 0;
		else m_CalcDatalst.DC7 = m_CalcDatalst.DC77;
	}
	m_CalcDatalst.DC78.Format(_T("%d"), int(1 + m_CalcDatalst.DC77 / 100.0f));

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateBufferTypeData()
{
	if (m_CalcDatalst.DC7 = 0) m_CalcDatalst.DC8 = m_CalcDatalst.DC71;
	else m_CalcDatalst.DC8 = m_CalcDatalst.DC72;
	
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE BufTyp='%s'"), g_tcTableName[eTableID_BufferPri], m_CalcDatalst.DC8);	
	m_CalcDatalst.DC81 = GetLONGValueFromTable(strSQL, _T("PriBuf"));

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateECStructureData()
{
	CString strSQL;
	CADOTable table;
	strSQL.Format(_T("SELECT * FROM %s WHERE EndCarSec='%s'"), g_tcTableName[eTableID_EndCarMatPar], m_CalcDatalst.DB7);
	if (table.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && table.GetRecordCnt() > 0)
	{
		table.GetRecordCell(0, _T("EcIx"), (DWORD&)m_CalcDatalst.DB72);
		table.GetRecordCell(0, _T("EcW"), (DWORD&)m_CalcDatalst.DB73);
		table.GetRecordCell(0, _T("EcWeiMax"), (DWORD&)m_CalcDatalst.DB74);
		table.GetRecordCell(0, _T("Bufhei"), (DWORD&)m_CalcDatalst.DB75);
		table.GetRecordCell(0, _T("EndCarHei"), (DWORD&)m_CalcDatalst.DB76);
		table.GetRecordCell(0, _T("EcEc24"), (DWORD&)m_CalcDatalst.DB77);
	}
	table.CloseTable();
		
	m_CalcDatalst.DB71 = m_CalcDatalst.DB2 + m_CalcDatalst.DB31;

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateEndCarDataText()
{
	CString strFirst5;	
	if (_T("B") == m_CalcDatalst.DB12)
	{
		strFirst5.Format(_T("%s"), m_CalcDatalst.DB2);
	}
	else
	{
		strFirst5.Format(_T("%s%02d"), m_CalcDatalst.DB24, int(m_CalcDatalst.DB8/100.0 + 0.5f));
	}

	m_CalcDatalst.DC9.Format(_T("%s/%d%s%s%s/%s/%s/%s%s"),
							 strFirst5,
							 m_CalcDatalst.DC5,
							 m_CalcDatalst.DB41,
							 m_CalcDatalst.DA51,
							 m_CalcDatalst.DB5,
							 m_CalcDatalst.DB36,
							 m_CalcDatalst.DC61,
							 m_CalcDatalst.DC78,
							 m_CalcDatalst.DC8);
	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::CalcEndCar()
{
	if (FALSE == GetCalcParamFormDB())
		return FALSE;

	// 最大湾应曲力
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE EndCarTyp='%s'"), g_tcTableName[eTableID_EndCarWei], m_CalcDatalst.DB2);	
	m_CalcDatalst.DC14 = GetFLOATValueFromTable(strSQL, _T("ECWei"));
	m_CalcDatalst.DC12 = (m_CalcDatalst.ME5 + m_CalcDatalst.DC14*2 + m_CalcDatalst.DB314*m_CalcDatalst.DB4) * (1 + m_CalcDatalst.P20);
	switch(m_CalcDatalst.DB12.GetAt(0))
	{
	case _T('B'):
		{
			m_CalcDatalst.DC11 = (m_CalcDatalst.DC12 / 8.0f + (m_CalcDatalst.HD01 * m_CalcDatalst.GA2 + m_CalcDatalst.GA3) / 2.0f * (m_CalcDatalst.GA5 - m_CalcDatalst.MB5 * 0.001) / m_CalcDatalst.GA5 * 0.5f) *0.0098;
			m_CalcDatalst.DC1 = m_CalcDatalst.DC11 * m_CalcDatalst.DB81 / 2.0f;
			m_CalcDatalst.DC13 = m_CalcDatalst.DC12 * 0.0098 / 8;
		}
		break;
	case _T('D'):
		{
			m_CalcDatalst.DC11 = (m_CalcDatalst.DC12 / 4.0f + (m_CalcDatalst.HD01 * m_CalcDatalst.GA2 + m_CalcDatalst.GA3)  * (m_CalcDatalst.GA5 - m_CalcDatalst.MB5 * 0.001) / m_CalcDatalst.GA5 * 0.5f) *0.0098;
			m_CalcDatalst.DC1 = m_CalcDatalst.DC11 * (m_CalcDatalst.DB8 - _ttol(m_CalcDatalst.DA5)) / 2.0f;
			m_CalcDatalst.DC13 = m_CalcDatalst.DC12 * 0.0098 / 4;
		}
		break;
	case _T('S'):
	case _T('U'):
		{
			m_CalcDatalst.DC11 = (m_CalcDatalst.DC12 / 4.0f + (m_CalcDatalst.HD01 * m_CalcDatalst.GA2 + m_CalcDatalst.GA3)  * (m_CalcDatalst.GA5 - m_CalcDatalst.MB5 * 0.001) / m_CalcDatalst.GA5 * 0.5f) *0.0098;
			m_CalcDatalst.DC1 = m_CalcDatalst.DC11 * m_CalcDatalst.DB8 / 2.0f;
			m_CalcDatalst.DC13 = m_CalcDatalst.DC12 * 0.0098 / 4;
		}
		break;
	}
	m_CalcDatalst.DC1 /= m_CalcDatalst.DB73;

	// 最大绕度
	m_CalcDatalst.DC21 = (m_CalcDatalst.DB12.GetAt(0) == _T('B')) ? m_CalcDatalst.DB81 / 1000.0f : m_CalcDatalst.DB8 / 1000.0f;
	m_CalcDatalst.DC22 = 21 * powf(10, 10);
	m_CalcDatalst.DC23 = m_CalcDatalst.DB72;
	m_CalcDatalst.DC2 = (2*m_CalcDatalst.DC11*1000 * powf(m_CalcDatalst.DC21, 3) / (48 * m_CalcDatalst.DC22 * m_CalcDatalst.DC23/powf(10, 8))) * 1000;

	// Results
	m_CalcDatalst.DD1 = m_CalcDatalst.DC1 / (m_CalcDatalst.DB61 / m_CalcDatalst.P14);
	if (m_CalcDatalst.DB12.GetAt(0) == _T('B'))	
	{
		m_CalcDatalst.DD2 = (m_CalcDatalst.DC2 / m_CalcDatalst.DB81) / m_CalcDatalst.P091;
	}
	else 
	{
		m_CalcDatalst.DD2 = (m_CalcDatalst.DC2 / m_CalcDatalst.DB8) / m_CalcDatalst.P091;
	}
	m_CalcDatalst.DD3 = (float)m_CalcDatalst.GC31 / m_CalcDatalst.DC3;
	m_CalcDatalst.DD4 = (m_CalcDatalst.DC12 + m_CalcDatalst.HD01 + m_CalcDatalst.GA4) / (m_CalcDatalst.DC4 * 1000);
	m_CalcDatalst.DD5  = (float)m_CalcDatalst.DC5 / m_CalcDatalst.DC58;
	if (_T("S") != m_CalcDatalst.DB12 &&
		_T("U") != m_CalcDatalst.DB12)
	{
		m_CalcDatalst.DD61 = m_CalcDatalst.DB84 + 2 * (m_CalcDatalst.DB82 + m_CalcDatalst.DB83);
		m_CalcDatalst.DD62 = m_CalcDatalst.DB8 - m_CalcDatalst.DB34 - 100 - m_CalcDatalst.DB52 - 2*m_CalcDatalst.MA2 + 100;
		m_CalcDatalst.DD63 = (float)m_CalcDatalst.DD61 / _ttol(m_CalcDatalst.DA5);
		m_CalcDatalst.DD64 = (float)_ttol(m_CalcDatalst.DA5) / m_CalcDatalst.DD62;
		if (_T("B") == m_CalcDatalst.DB12)
		{
			if (m_CalcDatalst.DD63 <= 1 && m_CalcDatalst.DD64 <= 1)
			{
				m_CalcDatalst.DD6 = m_CalcDatalst.DD63;
			}
			else if (m_CalcDatalst.DD63 > 1 && m_CalcDatalst.DD64 <= 1)
			{
				m_CalcDatalst.DD6 = m_CalcDatalst.DD63;
			}
			else
			{
				m_CalcDatalst.DD6 = m_CalcDatalst.DD64;
			}
		}
		else
		{
			m_CalcDatalst.DD6 = m_CalcDatalst.DD64;
		}
	}
	m_CalcDatalst.DD7 = m_CalcDatalst.GA5 * 1000 * m_CalcDatalst.P21 / m_CalcDatalst.DB8;
	

	/*	Check Rail is suitable or not */	
	float fPar1;
	long lPar2;
	CString strWheCode;
	strWheCode.Format(_T("%s%c%c"), m_CalcDatalst.DB21, m_CalcDatalst.GC3.GetAt(0), m_CalcDatalst.GC3.GetAt(1));
	strSQL.Format(_T("SELECT * FROM %s WHERE WheCode='%s'"), g_tcTableName[eTableID_WheLoaCal], strWheCode);
	fPar1 = GetFLOATValueFromTable(strSQL, _T("Par1"));
	lPar2 = GetLONGValueFromTable(strSQL, _T("Par2"));
	//
	float fRaiWidEff;
	float fWheLoaCalEff;
	strSQL.Format(_T("SELECT * FROM %s WHERE RaiTyp='%s'"), g_tcTableName[eTableID_DatRai], m_CalcDatalst.DA4);
	fRaiWidEff = GetLONGValueFromTable(strSQL, _T("RaiWidEff"));
	fWheLoaCalEff = GetFLOATValueFromTable(strSQL, _T("WheLoaCalEff"));
	//
	float fWheLoaAve = (fPar1 * fRaiWidEff * fWheLoaCalEff) / 1000.0f;
	float fWheLoaMax1 = (lPar2 * fRaiWidEff * fWheLoaCalEff) / 1000.0f;
	m_bRailTypeIsOK = (fWheLoaMax1 >= m_CalcDatalst.DC11) && (fWheLoaAve >= (m_CalcDatalst.DC11*2+m_CalcDatalst.DC13)/3.0f);
	if (_T("BMU") == m_CalcDatalst.GA1)
	{
		m_bRailTypeIsOK = TRUE;
	}

	/*Update rail calculation data*/
	m_CalcDatalst.RA01 = m_CalcDatalst.DA4;
	m_CalcDatalst.RA02 = _ttol(m_CalcDatalst.DB21);
	m_CalcDatalst.RA03 = m_CalcDatalst.GC3;
	m_CalcDatalst.RA031 = (long)GetLeftValueOfRangeString(m_CalcDatalst.GC3);
	m_CalcDatalst.RA06 = m_CalcDatalst.DC11;
	m_CalcDatalst.RA07 = m_CalcDatalst.DC13;
	m_CalcDatalst.RA12 = m_CalcDatalst.RA06;
	UpdateRailDataR04R05();
	UpdateRailDataR08R09();
	UpdateRailDataR10R11R13();
	m_CalcDatalst.DF3 = m_CalcDatalst.RA12 / m_CalcDatalst.RA10;
	m_CalcDatalst.DF4 = m_CalcDatalst.RA13 / m_CalcDatalst.RA11;

	/* XingChe size */
	if (_T("BMD") == m_CalcDatalst.GA1 || _T("BMZ") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.DF2 = m_CalcDatalst.DB76 + m_CalcDatalst.DB59 + m_CalcDatalst.MB7 + m_CalcDatalst.MA7 + m_CalcDatalst.MB32;
	}
	else if(_T("BMS") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.DF2 = m_CalcDatalst.DB76 + m_CalcDatalst.DB59 + m_CalcDatalst.MB7 + m_CalcDatalst.MA7 + 30;
	}
	else //"BMU"
	{
		m_CalcDatalst.DF2 = m_CalcDatalst.DB76 + m_CalcDatalst.MA5 + m_CalcDatalst.MB7 + m_CalcDatalst.MA7;
	}
	//
	if (_T("BMD") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.DF1 = m_CalcDatalst.GB2 + m_CalcDatalst.DB76 + m_CalcDatalst.DB59 + m_CalcDatalst.MB7 + 
							m_CalcDatalst.MA7 + m_CalcDatalst.MB32 - m_CalcDatalst.HB235;
	}
	else if (_T("BMS") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.DF1 = m_CalcDatalst.GB2 + m_CalcDatalst.DB76 + m_CalcDatalst.DB59 + m_CalcDatalst.MA5 + 
							m_CalcDatalst.MB7 - m_CalcDatalst.MA3 - m_CalcDatalst.HB235;
	}
	else if (_T("BMZ") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.DF1 = m_CalcDatalst.GB2 + m_CalcDatalst.DB76 + m_CalcDatalst.DB59 + m_CalcDatalst.MB7 + 
							m_CalcDatalst.MA7 + m_CalcDatalst.MB32 - m_CalcDatalst.HB235;

		m_CalcDatalst.DF11 = m_CalcDatalst.GB2 + m_CalcDatalst.DB76 + m_CalcDatalst.DB59 + m_CalcDatalst.MB7 + 
							 m_CalcDatalst.MA7 + m_CalcDatalst.MB32 - m_CalcDatalst.AU420;
	}
	else if (_T("BMU") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.DF1 = m_CalcDatalst.GB2 - m_CalcDatalst.DB76 - m_CalcDatalst.MA5 - 
							m_CalcDatalst.MB7 - m_CalcDatalst.MA3 - m_CalcDatalst.HB235;
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateElecCodeData()
{	
	// Crane panel
	m_CalcDatalst.EA1.Format(_T("%c"), m_CalcDatalst.EG1.GetAt(13));
	m_CalcDatalst.EA2.Format(_T("%c"), m_CalcDatalst.EG1.GetAt(14));
	m_CalcDatalst.EA3.Format(_T("%c%c"), m_CalcDatalst.EG1.GetAt(15), m_CalcDatalst.EG1.GetAt(16));
	m_CalcDatalst.EA4.Format(_T("%c"), m_CalcDatalst.EG1.GetAt(17));

	// Fastoon
	m_CalcDatalst.EB1.Format(_T("%c"), m_CalcDatalst.EG2.GetAt(7));
	m_CalcDatalst.EB2.Format(_T("%c"), m_CalcDatalst.EG2.GetAt(16));
	m_CalcDatalst.EB3.Format(_T("%c%c"), m_CalcDatalst.EG2.GetAt(17), m_CalcDatalst.EG2.GetAt(18));

	// Operter unit
	int iPos = m_CalcDatalst.EG3.Find(_T("-")) + 1;
	m_CalcDatalst.EC1 = m_CalcDatalst.EG3.Left(iPos - 1);
	if (m_CalcDatalst.EC1 == _T("Radio")) //for cases of "Radio-H" and "Radio-Y".
	{
		m_CalcDatalst.EC1 = m_CalcDatalst.EG3.Left(iPos + 1);
	}
	m_CalcDatalst.EC2.Format(_T("%c%c"), m_CalcDatalst.EG3.GetAt(iPos + 3), m_CalcDatalst.EG3.GetAt(iPos + 4));
	m_CalcDatalst.EC3.Format(_T("%c"), m_CalcDatalst.EG3.GetAt(iPos + 5));
	m_CalcDatalst.EC4.Format(_T("%c%c"), m_CalcDatalst.EG3.GetAt(iPos + 6), m_CalcDatalst.EG3.GetAt(iPos + 7));
	m_CalcDatalst.EC5.Format(_T("%c%c"), m_CalcDatalst.EG3.GetAt(iPos + 11), m_CalcDatalst.EG3.GetAt(iPos + 12));

	// Crane power supply	
	m_CalcDatalst.ED1.Format(_T("%c"), m_CalcDatalst.EG5.GetAt(6));
	m_CalcDatalst.ED2 = _ttol((CString)m_CalcDatalst.EG5.GetAt(9));
	m_CalcDatalst.ED3.Format(_T("%c"), m_CalcDatalst.EG5.GetAt(8));

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::UpdateElecPannelselection()
{
	if (_T("Z") == m_CalcDatalst.GA13)
	{
		if (_T("V3") == m_CalcDatalst.EA3)
			m_CalcDatalst.EA20 = m_CalcDatalst.HB217 + m_CalcDatalst.AU413;
		else
			m_CalcDatalst.EA20 = m_CalcDatalst.HB217;
	}
	else
	{
		if (_T("V3") == m_CalcDatalst.EA3)
			m_CalcDatalst.EA20 = m_CalcDatalst.HB217 * m_CalcDatalst.HB8;
		else
			m_CalcDatalst.EA20 = m_CalcDatalst.HB217;
	}
	return TRUE;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetCompomentTechData(TechDataList &dataList)
{	
	dataList.clear();
	dataList.push_back(TechData(STR_TECHDATA_LIST_HOIST, STR_TECHDATA_LIST_HOIST, STR_TECHDATA_LIST_HOIST, m_CalcDatalst.T25));
	dataList.push_back(TechData(STR_TECHDATA_LIST_ENDCAR, STR_TECHDATA_LIST_ENDCAR, STR_TECHDATA_LIST_ENDCAR, m_CalcDatalst.T26));
	dataList.push_back(TechData(STR_TECHDATA_LIST_BRIDGEMOTOR, STR_TECHDATA_LIST_BRIDGEMOTOR, STR_TECHDATA_LIST_BRIDGEMOTOR, m_CalcDatalst.T27));
	dataList.push_back(TechData(STR_TECHDATA_LIST_BRIDGEPANNEL, STR_TECHDATA_LIST_BRIDGEPANNEL, STR_TECHDATA_LIST_BRIDGEPANNEL, m_CalcDatalst.T28));
	dataList.push_back(TechData(STR_TECHDATA_LIST_TROLLEYPOWER, STR_TECHDATA_LIST_TROLLEYPOWER, STR_TECHDATA_LIST_TROLLEYPOWER, m_CalcDatalst.T29));
	dataList.push_back(TechData(STR_TECHDATA_LIST_OPERATEUNIT, STR_TECHDATA_LIST_OPERATEUNIT, STR_TECHDATA_LIST_OPERATEUNIT, m_CalcDatalst.T30));
	dataList.push_back(TechData(STR_TECHDATA_LIST_CRANEPOWER, STR_TECHDATA_LIST_CRANEPOWER, STR_TECHDATA_LIST_CRANEPOWER, m_CalcDatalst.T32));
	dataList.push_back(TechData(STR_TECHDATA_LIST_STEELSTRUCT, STR_TECHDATA_LIST_STEELSTRUCT, STR_TECHDATA_LIST_STEELSTRUCT, m_CalcDatalst.T33));

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetHoistTechData(TechDataList &dataList)
{	
	dataList.clear();
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TYPE, STR_TECHDATA_HOIST_EN_TYPE, STR_TECHDATA_HOIST_CN_TYPE, m_CalcDatalst.T01));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_FRAMESIZE, STR_TECHDATA_HOIST_EN_FRAMESIZE, STR_TECHDATA_HOIST_CN_FRAMESIZE, m_CalcDatalst.T02));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_REEVINGCODE, STR_TECHDATA_HOIST_EN_REEVINGCODE, STR_TECHDATA_HOIST_CN_REEVINGCODE, m_CalcDatalst.T03));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DRUMLENCODE, STR_TECHDATA_HOIST_EN_DRUMLENCODE, STR_TECHDATA_HOIST_CN_DRUMLENCODE, m_CalcDatalst.T04));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_LOAD, STR_TECHDATA_HOIST_EN_LOAD, STR_TECHDATA_HOIST_CN_LOAD, m_CalcDatalst.T05));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_HEIGHTOFLIT, STR_TECHDATA_HOIST_EN_HEIGHTOFLIT, STR_TECHDATA_HOIST_CN_HEIGHTOFLIT, m_CalcDatalst.T06));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_SPEED, STR_TECHDATA_HOIST_EN_SPEED, STR_TECHDATA_HOIST_CN_SPEED, m_CalcDatalst.T07));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORTYPE, STR_TECHDATA_HOIST_EN_MOTORTYPE, STR_TECHDATA_HOIST_CN_MOTORTYPE, m_CalcDatalst.T08));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DUTYGROUP, STR_TECHDATA_HOIST_EN_DUTYGROUP, STR_TECHDATA_HOIST_CN_DUTYGROUP, m_CalcDatalst.T09));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_VOLTAGE1, STR_TECHDATA_HOIST_EN_VOLTAGE1, STR_TECHDATA_HOIST_CN_VOLTAGE1, m_CalcDatalst.T10));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_VOLTAGE2, STR_TECHDATA_HOIST_EN_VOLTAGE2, STR_TECHDATA_HOIST_CN_VOLTAGE2, m_CalcDatalst.T11));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROSPEED, STR_TECHDATA_HOIST_EN_TROSPEED, STR_TECHDATA_HOIST_CN_TROSPEED,  m_CalcDatalst.T12));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROMOTOR, STR_TECHDATA_HOIST_EN_TROMOTOR, STR_TECHDATA_HOIST_CN_TROMOTOR, m_CalcDatalst.T13));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROMACHINERY, STR_TECHDATA_HOIST_EN_TROMACHINERY, STR_TECHDATA_HOIST_CN_TROMACHINERY, m_CalcDatalst.T14));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROLLEYTYPE, STR_TECHDATA_HOIST_EN_TROLLEYTYPE, STR_TECHDATA_HOIST_CN_TROLLEYTYPE, m_CalcDatalst.T15));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_RAILORGIRDER, STR_TECHDATA_HOIST_EN_RAILORGIRDER, STR_TECHDATA_HOIST_CN_RAILORGIRDER, m_CalcDatalst.T16));
	if (FALSE == m_bOnlyHoist)
	{
		dataList.push_back(TechData(STR_TECHDATA_HOIST_RAILTYPE, STR_TECHDATA_HOIST_EN_RAILTYPE, STR_TECHDATA_HOIST_CN_RAILTYPE, m_CalcDatalst.T17));
	}
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TRODUTY, STR_TECHDATA_HOIST_EN_TRODUTY, STR_TECHDATA_HOIST_CN_TRODUTY, m_CalcDatalst.T18));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_WEIGHT, STR_TECHDATA_HOIST_EN_WEIGHT, STR_TECHDATA_HOIST_CN_WEIGHT, m_CalcDatalst.T19));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_OVERLOAD, STR_TECHDATA_HOIST_EN_OVERLOAD, STR_TECHDATA_HOIST_CN_OVERLOAD, m_CalcDatalst.T20));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_ELECPROV, STR_TECHDATA_HOIST_EN_ELECPROV, STR_TECHDATA_HOIST_CN_ELECPROV, m_CalcDatalst.T21));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_ELECIP, STR_TECHDATA_HOIST_EN_ELECIP, STR_TECHDATA_HOIST_CN_ELECIP, m_CalcDatalst.T22));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORIP, STR_TECHDATA_HOIST_EN_MOTORIP, STR_TECHDATA_HOIST_CN_MOTORIP, m_CalcDatalst.T23));

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetHoistTechDataEx(TechDataList &dataList)
{
	dataList.clear();
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TYPE_MAIN, STR_TECHDATA_HOIST_EN_TYPE_MAIN, STR_TECHDATA_HOIST_CN_TYPE_MAIN, m_CalcDatalst.T01));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TYPE_AUX, STR_TECHDATA_HOIST_EN_TYPE_AUX, STR_TECHDATA_HOIST_CN_TYPE_AUX, m_CalcDatalst.T011));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_FRAMESIZE_MAIN, STR_TECHDATA_HOIST_EN_FRAMESIZE_MAIN, STR_TECHDATA_HOIST_CN_FRAMESIZE_MAIN, m_CalcDatalst.T02));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_FRAMESIZE_AUX, STR_TECHDATA_HOIST_EN_FRAMESIZE_AUX, STR_TECHDATA_HOIST_CN_FRAMESIZE_AUX, m_CalcDatalst.T021));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_REEVINGCODE_MAIN, STR_TECHDATA_HOIST_EN_REEVINGCODE_MAIN, STR_TECHDATA_HOIST_CN_REEVINGCODE_MAIN, m_CalcDatalst.T03));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_REEVINGCODE_AUX, STR_TECHDATA_HOIST_EN_REEVINGCODE_AUX, STR_TECHDATA_HOIST_CN_REEVINGCODE_AUX, m_CalcDatalst.T031));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DRUMLENCODE_MAIN, STR_TECHDATA_HOIST_EN_DRUMLENCODE_MAIN, STR_TECHDATA_HOIST_CN_DRUMLENCODE_MAIN, m_CalcDatalst.T04));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DRUMLENCODE_AUX, STR_TECHDATA_HOIST_EN_DRUMLENCODE_AUX, STR_TECHDATA_HOIST_CN_DRUMLENCODE_AUX, m_CalcDatalst.T041));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_LOAD_MAIN, STR_TECHDATA_HOIST_EN_LOAD_MAIN, STR_TECHDATA_HOIST_CN_LOAD_MAIN, m_CalcDatalst.T05));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_LOAD_AUX, STR_TECHDATA_HOIST_EN_LOAD_AUX, STR_TECHDATA_HOIST_CN_LOAD_AUX, m_CalcDatalst.T051));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_HEIGHTOFLIT_MAIN, STR_TECHDATA_HOIST_EN_HEIGHTOFLIT_MAIN, STR_TECHDATA_HOIST_CN_HEIGHTOFLIT_MAIN, m_CalcDatalst.T06));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_HEIGHTOFLIT_AUX, STR_TECHDATA_HOIST_EN_HEIGHTOFLIT_AUX, STR_TECHDATA_HOIST_CN_HEIGHTOFLIT_AUX, m_CalcDatalst.T061));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_SPEED_MAIN, STR_TECHDATA_HOIST_EN_SPEED_MAIN, STR_TECHDATA_HOIST_CN_SPEED_MAIN, m_CalcDatalst.T07));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_SPEED_AUX, STR_TECHDATA_HOIST_EN_SPEED_AUX, STR_TECHDATA_HOIST_CN_SPEED_AUX, m_CalcDatalst.T071));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORTYPE_MAIN, STR_TECHDATA_HOIST_EN_MOTORTYPE_MAIN, STR_TECHDATA_HOIST_CN_MOTORTYPE_MAIN, m_CalcDatalst.T08));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORTYPE_AUX, STR_TECHDATA_HOIST_EN_MOTORTYPE_AUX, STR_TECHDATA_HOIST_CN_MOTORTYPE_AUX, m_CalcDatalst.T081));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DUTYGROUP_MAIN, STR_TECHDATA_HOIST_EN_DUTYGROUP_MAIN, STR_TECHDATA_HOIST_CN_DUTYGROUP_MAIN, m_CalcDatalst.T09));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DUTYGROUP_AUX, STR_TECHDATA_HOIST_EN_DUTYGROUP_AUX, STR_TECHDATA_HOIST_CN_DUTYGROUP_AUX, m_CalcDatalst.T091));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_VOLTAGE1, STR_TECHDATA_HOIST_EN_VOLTAGE1, STR_TECHDATA_HOIST_CN_VOLTAGE1, m_CalcDatalst.T10));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_VOLTAGE2, STR_TECHDATA_HOIST_EN_VOLTAGE2, STR_TECHDATA_HOIST_CN_VOLTAGE2, m_CalcDatalst.T11));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROSPEED, STR_TECHDATA_HOIST_EN_TROSPEED, STR_TECHDATA_HOIST_CN_TROSPEED,  m_CalcDatalst.T12));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROMOTOR, STR_TECHDATA_HOIST_EN_TROMOTOR, STR_TECHDATA_HOIST_CN_TROMOTOR, m_CalcDatalst.T13));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROMACHINERY, STR_TECHDATA_HOIST_EN_TROMACHINERY, STR_TECHDATA_HOIST_CN_TROMACHINERY, m_CalcDatalst.T14));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROLLEYTYPE, STR_TECHDATA_HOIST_EN_TROLLEYTYPE, STR_TECHDATA_HOIST_CN_TROLLEYTYPE, m_CalcDatalst.T15));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_RAILORGIRDER, STR_TECHDATA_HOIST_EN_RAILORGIRDER, STR_TECHDATA_HOIST_CN_RAILORGIRDER, m_CalcDatalst.T16));
	if (FALSE == m_bOnlyHoist)
	{
		dataList.push_back(TechData(STR_TECHDATA_HOIST_RAILTYPE, STR_TECHDATA_HOIST_EN_RAILTYPE, STR_TECHDATA_HOIST_CN_RAILTYPE, m_CalcDatalst.T17));
	}
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TRODUTY, STR_TECHDATA_HOIST_EN_TRODUTY, STR_TECHDATA_HOIST_CN_TRODUTY, m_CalcDatalst.T18));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_WEIGHT, STR_TECHDATA_HOIST_EN_WEIGHT, STR_TECHDATA_HOIST_CN_WEIGHT, m_CalcDatalst.T19));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_OVERLOAD, STR_TECHDATA_HOIST_EN_OVERLOAD, STR_TECHDATA_HOIST_CN_OVERLOAD, m_CalcDatalst.T20));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_ELECPROV, STR_TECHDATA_HOIST_EN_ELECPROV, STR_TECHDATA_HOIST_CN_ELECPROV, m_CalcDatalst.T21));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_ELECIP, STR_TECHDATA_HOIST_EN_ELECIP, STR_TECHDATA_HOIST_CN_ELECIP, m_CalcDatalst.T22));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORIP, STR_TECHDATA_HOIST_EN_MOTORIP, STR_TECHDATA_HOIST_CN_MOTORIP, m_CalcDatalst.T23));

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetHoistStdAddTechData(TechDataList &dataList)
{
	dataList.clear();

	CADOTable tbAdditions;
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_Additions]);
	if (!tbAdditions.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) || tbAdditions.GetRecordCnt() <= 0)
		return FALSE;

	for (std::list<CString>::iterator it = m_lstStdHoistAdditions.begin();
		it != m_lstStdHoistAdditions.end();
		it++)
	{
		CString strAddition = *it;
	
		DWORD dwCount = tbAdditions.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
		{
			TCHAR tcName[64] = {_T("\0")};
			tbAdditions.GetRecordCell(dwIndex, L"Code", tcName, 64);
			if (_tcsicmp(tcName, strAddition) == 0)
			{
				TCHAR tcENText[256] = {_T("\0")};
				TCHAR tcCNText[256] = {_T("\0")};
				tbAdditions.GetRecordCell(dwIndex, L"Comment", tcENText, 256);
				tbAdditions.GetRecordCell(dwIndex, L"CommentCN", tcCNText, 256);
				dataList.push_back(TechData(tcENText, tcENText, tcCNText, _T("")));
				break;
			}
		}
	}
	tbAdditions.CloseTable();
	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetHoistSelAddTechData(TechDataList &dataList)
{
	dataList.clear();

	CADOTable tbAdditions;
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_Additions]);
	if (!tbAdditions.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) || tbAdditions.GetRecordCnt() <= 0)
		return FALSE;

	for (std::list<CString>::iterator it = m_lstOtherHoistAdditions.begin();
		it != m_lstOtherHoistAdditions.end();
		it++)
	{
		CString strAddition = *it;
	
		DWORD dwCount = tbAdditions.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
		{
			TCHAR tcName[64] = {_T("\0")};
			tbAdditions.GetRecordCell(dwIndex, L"Code", tcName, 64);
			if (_tcsicmp(tcName, strAddition) == 0)
			{
				TCHAR tcENText[256] = {_T("\0")};
				TCHAR tcCNText[256] = {_T("\0")};
				tbAdditions.GetRecordCell(dwIndex, L"Comment", tcENText, 256);
				tbAdditions.GetRecordCell(dwIndex, L"CommentCN", tcCNText, 256);
				dataList.push_back(TechData(tcENText, tcENText, tcCNText, _T("")));
				break;
			}
		}
	}
	tbAdditions.CloseTable();

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetCranesTechData(TechDataList &dataList)
{
	dataList.clear();
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TYPE, STR_TECHDATA_CRANE_EN_TYPE, STR_TECHDATA_CRANE_CN_TYPE, m_CalcDatalst.T34));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CAPACITY, STR_TECHDATA_CRANE_EN_CAPACITY, STR_TECHDATA_CRANE_CN_CAPACITY, m_CalcDatalst.T35));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_SPAN, STR_TECHDATA_CRANE_EN_SPAN, STR_TECHDATA_CRANE_CN_SPAN, m_CalcDatalst.T36));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_LIFTHEIGHT, STR_TECHDATA_CRANE_EN_LIFTHEIGHT, STR_TECHDATA_CRANE_CN_LIFTHEIGHT, m_CalcDatalst.T37));
	//38
	dataList.push_back(TechData(STR_TECHDATA_CRANE_DUTYGROUP, STR_TECHDATA_CRANE_EN_DUTYGROUP, STR_TECHDATA_CRANE_CN_DUTYGROUP, m_CalcDatalst.T39));	
	dataList.push_back(TechData(STR_TECHDATA_CRANE_WEIGHT, STR_TECHDATA_CRANE_EN_WEIGHT, STR_TECHDATA_CRANE_CN_WEIGHT, m_CalcDatalst.T40));	
	dataList.push_back(TechData(STR_TECHDATA_CRANE_STEELWEIGHT, STR_TECHDATA_CRANE_EN_STEELWEIGHT, STR_TECHDATA_CRANE_CN_STEELWEIGHT, m_CalcDatalst.T41));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TROWEIGHT, STR_TECHDATA_CRANE_EN_TROWEIGHT, STR_TECHDATA_CRANE_CN_TROWEIGHT, m_CalcDatalst.T19));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_ENDCARWEIGHT, STR_TECHDATA_CRANE_EN_ENDCARWEIGHT, STR_TECHDATA_CRANE_CN_ENDCARWEIGHT, m_CalcDatalst.T42));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_RAILWHEELNUM, STR_TECHDATA_CRANE_EN_RAILWHEELNUM, STR_TECHDATA_CRANE_CN_RAILWHEELNUM, m_CalcDatalst.T43));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_WHEELLOAD, STR_TECHDATA_CRANE_EN_WHEELLOAD, STR_TECHDATA_CRANE_CN_WHEELLOAD, m_CalcDatalst.T44));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_RAILTYPE, STR_TECHDATA_CRANE_EN_RAILTYPE, STR_TECHDATA_CRANE_CN_RAILTYPE, m_CalcDatalst.T45));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_MAINVOL, STR_TECHDATA_CRANE_EN_MAINVOL, STR_TECHDATA_CRANE_CN_MAINVOL, m_CalcDatalst.T10));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CTRLVOL, STR_TECHDATA_CRANE_EN_CTRLVOL, STR_TECHDATA_CRANE_CN_CTRLVOL, m_CalcDatalst.T11));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_MAINUSE, STR_TECHDATA_CRANE_EN_MAINUSE, STR_TECHDATA_CRANE_CN_MAINUSE, m_CalcDatalst.T46));
	
	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetCranesTechHoistData(TechDataList &dataList)
{
	dataList.clear();
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TYPE, STR_TECHDATA_HOIST_EN_TYPE, STR_TECHDATA_HOIST_CN_TYPE, m_CalcDatalst.T01));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_FRAMESIZE, STR_TECHDATA_HOIST_EN_FRAMESIZE, STR_TECHDATA_HOIST_CN_FRAMESIZE, m_CalcDatalst.T02));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_REEVINGCODE, STR_TECHDATA_HOIST_EN_REEVINGCODE, STR_TECHDATA_HOIST_CN_REEVINGCODE, m_CalcDatalst.T03));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DRUMLENCODE, STR_TECHDATA_HOIST_EN_DRUMLENCODE, STR_TECHDATA_HOIST_CN_DRUMLENCODE, m_CalcDatalst.T04));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_LOAD, STR_TECHDATA_HOIST_EN_LOAD, STR_TECHDATA_HOIST_CN_LOAD, m_CalcDatalst.T05));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_HEIGHTOFLIT, STR_TECHDATA_HOIST_EN_HEIGHTOFLIT, STR_TECHDATA_HOIST_CN_HEIGHTOFLIT, m_CalcDatalst.T06));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_SPEED, STR_TECHDATA_HOIST_EN_SPEED, STR_TECHDATA_HOIST_CN_SPEED, m_CalcDatalst.T07));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORTYPE, STR_TECHDATA_HOIST_EN_MOTORTYPE, STR_TECHDATA_HOIST_CN_MOTORTYPE, m_CalcDatalst.T08));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DUTYGROUP, STR_TECHDATA_HOIST_EN_DUTYGROUP, STR_TECHDATA_HOIST_CN_DUTYGROUP, m_CalcDatalst.T09));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TRODUTY, STR_TECHDATA_HOIST_EN_TRODUTY, STR_TECHDATA_HOIST_CN_TRODUTY, m_CalcDatalst.T18));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_HOISTDUTY, STR_TECHDATA_CRANE_EN_HOISTDUTY, STR_TECHDATA_CRANE_CN_HOISTDUTY, m_CalcDatalst.T18));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TRAVERSSPD, STR_TECHDATA_CRANE_EN_TRAVERSSPD, STR_TECHDATA_CRANE_CN_TRAVERSSPD,  m_CalcDatalst.T12));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TRAVELSPD, STR_TECHDATA_CRANE_EN_TRAVELSPD, STR_TECHDATA_CRANE_CN_TRAVELSPD,  m_CalcDatalst.T47));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TROMOTOR, STR_TECHDATA_CRANE_EN_TROMOTOR, STR_TECHDATA_CRANE_CN_TROMOTOR, m_CalcDatalst.T13));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CRANEMOTOR, STR_TECHDATA_CRANE_EN_CRANEMOTOR, STR_TECHDATA_CRANE_CN_CRANEMOTOR,  m_CalcDatalst.T48));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TOTALPOW, STR_TECHDATA_CRANE_EN_TOTALPOW, STR_TECHDATA_CRANE_CN_TOTALPOW,  m_CalcDatalst.T49));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_ELECIP, STR_TECHDATA_CRANE_EN_ELECIP, STR_TECHDATA_CRANE_CN_ELECIP, m_CalcDatalst.T22));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_MOTORIP, STR_TECHDATA_CRANE_EN_MOTORIP, STR_TECHDATA_CRANE_CN_MOTORIP, m_CalcDatalst.T23));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CTRLTYPE, STR_TECHDATA_CRANE_EN_CTRLTYPE, STR_TECHDATA_CRANE_CN_CTRLTYPE, m_CalcDatalst.T50));
	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetCranesTechHoistDataEx(TechDataList &dataList)
{	
	dataList.clear();
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TYPE_MAIN, STR_TECHDATA_HOIST_EN_TYPE_MAIN, STR_TECHDATA_HOIST_CN_TYPE_MAIN, m_CalcDatalst.T01));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TYPE_AUX, STR_TECHDATA_HOIST_EN_TYPE_AUX, STR_TECHDATA_HOIST_CN_TYPE_AUX, m_CalcDatalst.T011));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_FRAMESIZE_MAIN, STR_TECHDATA_HOIST_EN_FRAMESIZE_MAIN, STR_TECHDATA_HOIST_CN_FRAMESIZE_MAIN, m_CalcDatalst.T02));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_FRAMESIZE_AUX, STR_TECHDATA_HOIST_EN_FRAMESIZE_AUX, STR_TECHDATA_HOIST_CN_FRAMESIZE_AUX, m_CalcDatalst.T021));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_REEVINGCODE_MAIN, STR_TECHDATA_HOIST_EN_REEVINGCODE_MAIN, STR_TECHDATA_HOIST_CN_REEVINGCODE_MAIN, m_CalcDatalst.T03));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_REEVINGCODE_AUX, STR_TECHDATA_HOIST_EN_REEVINGCODE_AUX, STR_TECHDATA_HOIST_CN_REEVINGCODE_AUX, m_CalcDatalst.T031));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DRUMLENCODE_MAIN, STR_TECHDATA_HOIST_EN_DRUMLENCODE_MAIN, STR_TECHDATA_HOIST_CN_DRUMLENCODE_MAIN, m_CalcDatalst.T04));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DRUMLENCODE_AUX, STR_TECHDATA_HOIST_EN_DRUMLENCODE_AUX, STR_TECHDATA_HOIST_CN_DRUMLENCODE_AUX, m_CalcDatalst.T041));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_LOAD_MAIN, STR_TECHDATA_HOIST_EN_LOAD_MAIN, STR_TECHDATA_HOIST_CN_LOAD_MAIN, m_CalcDatalst.T05));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_LOAD_AUX, STR_TECHDATA_HOIST_EN_LOAD_AUX, STR_TECHDATA_HOIST_CN_LOAD_AUX, m_CalcDatalst.T051));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_HEIGHTOFLIT_MAIN, STR_TECHDATA_HOIST_EN_HEIGHTOFLIT_MAIN, STR_TECHDATA_HOIST_CN_HEIGHTOFLIT_MAIN, m_CalcDatalst.T06));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_HEIGHTOFLIT_AUX, STR_TECHDATA_HOIST_EN_HEIGHTOFLIT_AUX, STR_TECHDATA_HOIST_CN_HEIGHTOFLIT_AUX, m_CalcDatalst.T061));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_SPEED_MAIN, STR_TECHDATA_HOIST_EN_SPEED_MAIN, STR_TECHDATA_HOIST_CN_SPEED_MAIN, m_CalcDatalst.T07));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_SPEED_AUX, STR_TECHDATA_HOIST_EN_SPEED_AUX, STR_TECHDATA_HOIST_CN_SPEED_AUX, m_CalcDatalst.T071));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORTYPE_MAIN, STR_TECHDATA_HOIST_EN_MOTORTYPE_MAIN, STR_TECHDATA_HOIST_CN_MOTORTYPE_MAIN, m_CalcDatalst.T08));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORTYPE_AUX, STR_TECHDATA_HOIST_EN_MOTORTYPE_AUX, STR_TECHDATA_HOIST_CN_MOTORTYPE_AUX, m_CalcDatalst.T081));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DUTYGROUP_MAIN, STR_TECHDATA_HOIST_EN_DUTYGROUP_MAIN, STR_TECHDATA_HOIST_CN_DUTYGROUP_MAIN, m_CalcDatalst.T09));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DUTYGROUP_AUX, STR_TECHDATA_HOIST_EN_DUTYGROUP_AUX, STR_TECHDATA_HOIST_CN_DUTYGROUP_AUX, m_CalcDatalst.T091));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TRODUTY, STR_TECHDATA_HOIST_EN_TRODUTY, STR_TECHDATA_HOIST_CN_TRODUTY, m_CalcDatalst.T18));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_HOISTDUTY, STR_TECHDATA_CRANE_EN_HOISTDUTY, STR_TECHDATA_CRANE_CN_HOISTDUTY, m_CalcDatalst.T18));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TRAVERSSPD, STR_TECHDATA_CRANE_EN_TRAVERSSPD, STR_TECHDATA_CRANE_CN_TRAVERSSPD,  m_CalcDatalst.T12));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TRAVELSPD, STR_TECHDATA_CRANE_EN_TRAVELSPD, STR_TECHDATA_CRANE_CN_TRAVELSPD,  m_CalcDatalst.T47));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TROMOTOR, STR_TECHDATA_CRANE_EN_TROMOTOR, STR_TECHDATA_CRANE_CN_TROMOTOR, m_CalcDatalst.T13));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CRANEMOTOR, STR_TECHDATA_CRANE_EN_CRANEMOTOR, STR_TECHDATA_CRANE_CN_CRANEMOTOR,  m_CalcDatalst.T48));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TOTALPOW, STR_TECHDATA_CRANE_EN_TOTALPOW, STR_TECHDATA_CRANE_CN_TOTALPOW,  m_CalcDatalst.T49));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_ELECIP, STR_TECHDATA_CRANE_EN_ELECIP, STR_TECHDATA_CRANE_CN_ELECIP, m_CalcDatalst.T22));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_MOTORIP, STR_TECHDATA_CRANE_EN_MOTORIP, STR_TECHDATA_CRANE_CN_MOTORIP, m_CalcDatalst.T23));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CTRLTYPE, STR_TECHDATA_CRANE_EN_CTRLTYPE, STR_TECHDATA_CRANE_CN_CTRLTYPE, m_CalcDatalst.T50));
	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetCraneSelAddTechData(TechDataList &dataList)
{
	dataList.clear();

	CString strSQL;
	CADOTable tbAdditions;
	strSQL.Format(_T("SELECT * FROM %s"), g_tcTableName[eTableID_Additions]);
	if (!tbAdditions.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) || tbAdditions.GetRecordCnt() <= 0)
		return FALSE;

	for (std::list<CString>::iterator it = m_lstEndCarAdditions.begin();
		it != m_lstEndCarAdditions.end();
		it++)
	{
		CString strAddition = *it;
	
		DWORD dwCount = tbAdditions.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
		{
			TCHAR tcName[64] = {_T("\0")};
			tbAdditions.GetRecordCell(dwIndex, L"Code", tcName, 64);
			if (_tcsicmp(tcName, strAddition) == 0)
			{
				TCHAR tcENText[256] = {_T("\0")};
				TCHAR tcCNText[256] = {_T("\0")};
				tbAdditions.GetRecordCell(dwIndex, L"Comment", tcENText, 256);
				tbAdditions.GetRecordCell(dwIndex, L"CommentCN", tcCNText, 256);
				dataList.push_back(TechData(tcENText, tcENText, tcCNText, _T("")));
				break;
			}
		}
	}
	for (std::list<CString>::iterator it = m_lstElecCtrlAdditions.begin();
		it != m_lstElecCtrlAdditions.end();
		it++)
	{
		CString strAddition = *it;
	
		DWORD dwCount = tbAdditions.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
		{
			TCHAR tcName[64] = {_T("\0")};
			tbAdditions.GetRecordCell(dwIndex, L"Code", tcName, 64);
			if (_tcsicmp(tcName, strAddition) == 0)
			{
				TCHAR tcENText[256] = {_T("\0")};
				TCHAR tcCNText[256] = {_T("\0")};
				tbAdditions.GetRecordCell(dwIndex, L"Comment", tcENText, 256);
				tbAdditions.GetRecordCell(dwIndex, L"CommentCN", tcCNText, 256);
				dataList.push_back(TechData(tcENText, tcENText, tcCNText, _T("")));
				break;
			}
		}
	}
	tbAdditions.CloseTable();
	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::ConstructTechData()
{
	/* Hoist tech data */
	m_CalcDatalst.T01 = m_CalcDatalst.HB2;
	m_CalcDatalst.T011 = m_CalcDatalst.AU4;
	m_CalcDatalst.T02 = m_CalcDatalst.HB240;
	m_CalcDatalst.T021 = m_CalcDatalst.AU422;
	m_CalcDatalst.T03 = m_CalcDatalst.HB204;
	m_CalcDatalst.T031 = m_CalcDatalst.AU404;
	m_CalcDatalst.T04.Format(_T("%s"), m_CalcDatalst.HB210);
	m_CalcDatalst.T041.Format(_T("%s"), m_CalcDatalst.AU410);
	m_CalcDatalst.T05.Format(_T("%dKg"), m_CalcDatalst.HA3);
	m_CalcDatalst.T051.Format(_T("%dKg"), m_CalcDatalst.AU1);
	m_CalcDatalst.T06.Format(_T("%.2fm (max. %.2fm)"), m_bOnlyHoist? m_CalcDatalst.GA6 : m_CalcDatalst.DF1/1000.0f, m_CalcDatalst.HB224);
	m_CalcDatalst.T061.Format(_T("%.2fm (max. %.2fm)"), m_CalcDatalst.DF11/1000.0f, m_CalcDatalst.AU417);
	m_CalcDatalst.T07.Format(_T("%s m/min (%s)"), m_CalcDatalst.HB219, m_CalcDatalst.HB211);
	m_CalcDatalst.T071.Format(_T("%s m/min (%s)"), m_CalcDatalst.AU415, m_CalcDatalst.HB211);
	m_CalcDatalst.T08.Format(_T("%s (%s kW)"), m_CalcDatalst.HB206, m_CalcDatalst.HB218);
	m_CalcDatalst.T081.Format(_T("%s (%s kW)"), m_CalcDatalst.AU406, m_CalcDatalst.AU414);
	m_CalcDatalst.T09.Format(_T("FEM %s"), m_CalcDatalst.HB222);
	m_CalcDatalst.T091.Format(_T("FEM %s"), m_CalcDatalst.AU416);
	m_CalcDatalst.T10.Format(_T("3Ph/%dV/50Hz"), m_CalcDatalst.GB4);
	m_CalcDatalst.T11.Format(_T("%dV"), m_CalcDatalst.GB5);
	m_CalcDatalst.T12.Format(_T("%s m/min (%s)"), m_CalcDatalst.GC2, m_CalcDatalst.GC5);
	m_CalcDatalst.T13.Format(_T("%dx%.2f KW"), m_CalcDatalst.HB215, m_CalcDatalst.HB216);
	m_CalcDatalst.T14.Format(_T("%s"), m_CalcDatalst.HB214);
	m_CalcDatalst.T15.Format(_T("%s"), m_CalcDatalst.GA13);
	m_CalcDatalst.T16.Format(_T("%smm"), m_bOnlyHoist ? m_CalcDatalst.HA8 : m_CalcDatalst.DA5);
	m_CalcDatalst.T17.Format(_T("%s"), m_CalcDatalst.MB3);
	m_CalcDatalst.T18.Format(_T("%s"), _T("FEM 2m/M5"));
	m_CalcDatalst.T19.Format(_T("%d x %d kg"), m_CalcDatalst.HB8, m_CalcDatalst.HD01);
	m_CalcDatalst.T20.Format(_T("%s"), _T("ELE"));
	m_CalcDatalst.T21.Format(_T("%s"), _T("CRANES"));	
	m_CalcDatalst.T22.Format(_T("%s"), _T("IP54"));
	m_CalcDatalst.T23.Format(_T("%s"), _T("IP54"));

	/* Component tech data */
	m_CalcDatalst.T25.Format(_T("%s"), m_CalcDatalst.HD02);
	m_CalcDatalst.T26.Format(_T("%s"), m_CalcDatalst.DC9);
	m_CalcDatalst.T27.Format(_T("%s"), m_CalcDatalst.DB3);
	m_CalcDatalst.T28.Format(_T("%s"), m_CalcDatalst.EG1);
	m_CalcDatalst.T29.Format(_T("%s"), m_CalcDatalst.EG2);
	m_CalcDatalst.T30.Format(_T("%s"), m_CalcDatalst.EG3);
	m_CalcDatalst.T32.Format(_T("%s"), m_CalcDatalst.EG5);
	m_CalcDatalst.T33.Format(_T("%s"), m_CalcDatalst.ME1);

	/* Cranne tech data */
	m_CalcDatalst.T34.Format(_T("%s"), m_CalcDatalst.GA13);
	m_CalcDatalst.T35.Format(_T("%dkg"), m_CalcDatalst.GA3);
	m_CalcDatalst.T36.Format(_T("%.2fm"), m_CalcDatalst.GA5);
	m_CalcDatalst.T37.Format(_T("%.2fm"), m_CalcDatalst.DF1/1000.0f);
	m_CalcDatalst.T38.Format(_T("%dmm"), m_CalcDatalst.GB2);
	m_CalcDatalst.T39.Format(_T("%s"), m_CalcDatalst.GA7);
	m_CalcDatalst.T40.Format(_T("%dkg"), m_CalcDatalst.DC12 + m_CalcDatalst.HD01);
	m_CalcDatalst.T41.Format(_T("%dkg"), m_CalcDatalst.ME5);
	m_CalcDatalst.T42.Format(_T("2 x %dkg"), int(m_CalcDatalst.DC14 + m_CalcDatalst.DB314*m_CalcDatalst.DB4/2));
	m_CalcDatalst.T43.Format(_T("%d"), m_CalcDatalst.DB11);
	m_CalcDatalst.T44.Format(_T("%dKN"), (int)m_CalcDatalst.DC11);
	m_CalcDatalst.T45.Format(_T("%s"), m_CalcDatalst.DA4);
	m_CalcDatalst.T46.Format(_T("%s/-5-40"), m_CalcDatalst.GB7);
	m_CalcDatalst.T47.Format(_T("%sm/min (%s)"), m_CalcDatalst.GC3, m_CalcDatalst.GC6);
	m_CalcDatalst.T48.Format(_T("%d x %.2fKW"), m_CalcDatalst.DB4, m_CalcDatalst.DB35);
	m_CalcDatalst.T49.Format(_T("%.2fKW"), (m_CalcDatalst.EA20 + m_CalcDatalst.HB215*m_CalcDatalst.HB216 + m_CalcDatalst.DB42));
	m_CalcDatalst.T50.Format(_T("%s"), m_CalcDatalst.EC12);

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::ConstructMainGirderDrawingData()
{
	m_CalcDatalst.dim0 = m_CalcDatalst.GD8;
	m_CalcDatalst.dim1 = m_CalcDatalst.GD5 + m_CalcDatalst.DB74;
	m_CalcDatalst.dim2 = m_CalcDatalst.MB51 + m_CalcDatalst.DB74;
	m_CalcDatalst.dim3 = m_CalcDatalst.MB5 + m_CalcDatalst.DB74;
	m_CalcDatalst.dim4 = m_CalcDatalst.GB2;
	m_CalcDatalst.dim5 = m_CalcDatalst.DF2;
	m_CalcDatalst.dim6 = m_CalcDatalst.DF2 + m_CalcDatalst.HB234;
	m_CalcDatalst.dim7 = m_CalcDatalst.dim6 + m_CalcDatalst.dim0;
	m_CalcDatalst.dim8 =  m_CalcDatalst.dim7 + m_CalcDatalst.dim4;
	m_CalcDatalst.dim10 = _ttol(m_CalcDatalst.DA5);
	m_CalcDatalst.dim11 = m_CalcDatalst.DB8;
	m_CalcDatalst.dim12 = m_CalcDatalst.DC76;
	m_CalcDatalst.dim13 = m_CalcDatalst.MB71;
	m_CalcDatalst.dim14 = m_CalcDatalst.dim4 + m_CalcDatalst.dim5 - m_CalcDatalst.dim13;
	m_CalcDatalst.dim15 = m_CalcDatalst.dim4 + m_CalcDatalst.dim5 - 1000;
	m_CalcDatalst.dim16 = m_CalcDatalst.MB72;
	m_CalcDatalst.dim17 = m_CalcDatalst.DF1;
	m_CalcDatalst.dim18 = m_CalcDatalst.DB75;
	m_CalcDatalst.dim19 = m_CalcDatalst.MB74;
	m_CalcDatalst.dim20 = m_CalcDatalst.MB73;
	m_CalcDatalst.dim21 = abs(m_CalcDatalst.HB221*0.5f - m_CalcDatalst.HB231);
	m_CalcDatalst.dim22 = m_CalcDatalst.HB232;
	m_CalcDatalst.dim23 = m_CalcDatalst.HD04;
	//
	m_CalcDatalst.dim30 = m_CalcDatalst.DA42 + m_CalcDatalst.DB76 - 30;
	m_CalcDatalst.dim31 = m_CalcDatalst.dim4 - m_CalcDatalst.dim5;
	m_CalcDatalst.dim32 = m_CalcDatalst.DA42;
	m_CalcDatalst.dim33 = m_CalcDatalst.dim31 + m_CalcDatalst.dim13 - 1000;
	m_CalcDatalst.dim34 = m_CalcDatalst.dim31 + m_CalcDatalst.dim13 - m_CalcDatalst.dim16;

	m_CalcDatalst.cralod.Format(_T("%.2f"), m_CalcDatalst.GD4);
	m_CalcDatalst.hlod1.Format(_T("%s"), m_CalcDatalst.T05);
	m_CalcDatalst.hlod2.Format(_T("%s"), m_CalcDatalst.T051);
	m_CalcDatalst.span1.Format(_T("%d"), m_CalcDatalst.GD1);
	m_CalcDatalst.span2.Format(_T("%d"), m_CalcDatalst.GD1 + 2*m_CalcDatalst.DB74);
	m_CalcDatalst.span.Format(_T("%.2f"), m_CalcDatalst.GA5);
	m_CalcDatalst.hoistcode.Format(_T("%s"), m_CalcDatalst.T25);
	m_CalcDatalst.maxwheelload.Format(_T("%s"), m_CalcDatalst.T44);
	m_CalcDatalst.eccode.Format(_T("%s"), m_CalcDatalst.DC9.Left(5));
	m_CalcDatalst.hol.Format(_T("%s"), m_CalcDatalst.T06);
	m_CalcDatalst.hol1.Format(_T("%d"), int(m_CalcDatalst.HB224 * 1000));
	m_CalcDatalst.hol2.Format(_T("%.2f"), m_CalcDatalst.DF1 * 0.001f);
	m_CalcDatalst.hol3.Format(_T("%s"), m_CalcDatalst.T061);
	m_CalcDatalst.hoispeed.Format(_T("%s"), m_CalcDatalst.T07);
	m_CalcDatalst.hoispeed2.Format(_T("%s"), m_CalcDatalst.T071);	
	m_CalcDatalst.trospeed.Format(_T("%s"), m_CalcDatalst.T12);
	m_CalcDatalst.brispeed.Format(_T("%s"), m_CalcDatalst.T47);
	m_CalcDatalst.weitro.Format(_T("%s"), m_CalcDatalst.T19);
	m_CalcDatalst.weibri.Format(_T("%s"), m_CalcDatalst.T40);
	m_CalcDatalst.V1.Format(_T("%d"), m_CalcDatalst.GB4);
	m_CalcDatalst.V2.Format(_T("%d"), m_CalcDatalst.GB5);
	m_CalcDatalst.cargro.Format(_T("%s"), m_CalcDatalst.GA7);
	m_CalcDatalst.hoigro.Format(_T("%s"), m_CalcDatalst.T09);
	m_CalcDatalst.hoigro2.Format(_T("%s"), m_CalcDatalst.T091);
	m_CalcDatalst.trogro.Format(_T("%s"), m_CalcDatalst.T18);
	m_CalcDatalst.brigro.Format(_T("%s"), m_CalcDatalst.T18);
	m_CalcDatalst.suppow.Format(_T("%s"), m_CalcDatalst.T49);
	m_CalcDatalst.railtype.Format(_T("%s"), m_CalcDatalst.T45);
	m_CalcDatalst.railen.Format(_T("%.2f"), m_CalcDatalst.GB1);
	m_CalcDatalst.contype.Format(_T("%s"), m_CalcDatalst.EC12);

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::ConstructEndcarDrawingData()
{
	m_CalcDatalst.ececcode.Format(_T("%s"), m_CalcDatalst.DC9);
	m_CalcDatalst.ecec1.Format(_T("%s"), m_CalcDatalst.DC9.Left(5));
	m_CalcDatalst.ecec2.Format(_T("%s"), m_CalcDatalst.DB3);
	m_CalcDatalst.ecec3.Format(_T("%s"), m_CalcDatalst.DA4);
	m_CalcDatalst.ecec4.Format(_T("%s"), m_CalcDatalst.DB5);
	m_CalcDatalst.ecec5.Format(_T("%d"), m_CalcDatalst.DC6);
	m_CalcDatalst.ecec6.Format(_T("%s"), m_CalcDatalst.DB7);
	m_CalcDatalst.ecec7.Format(_T("%d"), m_CalcDatalst.DB8);
	m_CalcDatalst.ecec8.Format(_T("%d"), m_CalcDatalst.DC5);
	m_CalcDatalst.ecec9.Format(_T("%d"), m_CalcDatalst.DB59);
	m_CalcDatalst.ecec10.Format(_T("%s"), m_CalcDatalst.DA5);
	m_CalcDatalst.ecec11.Format(_T("%d"), int(0.5f * m_CalcDatalst.DB4));
	m_CalcDatalst.ecec12.Format(_T("%d"), m_CalcDatalst.DC76);
	m_CalcDatalst.ecec13.Format(_T("%d"), m_CalcDatalst.MA2);
	m_CalcDatalst.ecec14.Format(_T("%s"), m_CalcDatalst.DC8);
	m_CalcDatalst.ecec15.Format(_T("%d"), m_CalcDatalst.DB8);
	m_CalcDatalst.ecec16.Format(_T("%d"), m_CalcDatalst.DC73);
	m_CalcDatalst.ecec18.Format(_T("%d"), int(m_CalcDatalst.DB311 + m_CalcDatalst.DB74));
	m_CalcDatalst.ecec19.Format(_T("%d"), m_CalcDatalst.DB57 -15);
	m_CalcDatalst.ecec20.Format(_T("%d"), m_CalcDatalst.DB56 - 15);
	m_CalcDatalst.ecec21.Format(_T("%d"), m_CalcDatalst.DB56);
	m_CalcDatalst.ecec22.Format(_T("%d"), m_CalcDatalst.DB75);
	m_CalcDatalst.ecec23.Format(_T("%d"), m_CalcDatalst.DB312);
	m_CalcDatalst.ecec24.Format(_T("%d"), m_CalcDatalst.DB74);
	m_CalcDatalst.ecec25.Format(_T("%d"), m_CalcDatalst.MA2 + m_CalcDatalst.DB52);
	m_CalcDatalst.ecec26.Format(_T("%d"), m_CalcDatalst.DB57);
	m_CalcDatalst.ecec27.Format(_T("%d"), m_CalcDatalst.DB53);
	m_CalcDatalst.ecec28.Format(_T("%d"), m_CalcDatalst.DB76);
	m_CalcDatalst.ecec29.Format(_T("%d"), m_CalcDatalst.DB313);
	m_CalcDatalst.ecec30.Format(_T("%d"), m_CalcDatalst.DB54);
	m_CalcDatalst.ecec31.Format(_T("%d"), m_CalcDatalst.MA2);
	m_CalcDatalst.ecec32.Format(_T("%d"), m_CalcDatalst.DB58);
	m_CalcDatalst.ecec33.Format(_T("%d"), m_CalcDatalst.DC77);
	m_CalcDatalst.ecec34.Format(_T("%d"), long(m_CalcDatalst.DB11-0.5f*m_CalcDatalst.DB4));
	//
	m_CalcDatalst.ecec40.Format(_T("%d"), m_CalcDatalst.DB76 + m_CalcDatalst.DB54);
	m_CalcDatalst.ecec41.Format(_T("%d"), m_CalcDatalst.DA42 - m_CalcDatalst.DB54);
	m_CalcDatalst.ecec42.Format(_T("%d"), m_CalcDatalst.DA41);
	m_CalcDatalst.ecec43.Format(_T("%d"), m_CalcDatalst.DB77);


	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::ConstructSteelStructDrawingData()
{
	m_CalcDatalst.ctype.Format(_T("%s"), m_CalcDatalst.GA12);
	m_CalcDatalst.st01.Format(_T("%s"), m_CalcDatalst.ME1);	
	m_CalcDatalst.st02.Format(_T("%.2f"), m_CalcDatalst.GA5);
	m_CalcDatalst.st03.Format(_T("%s"), m_CalcDatalst.GA7);
	m_CalcDatalst.st04.Format(_T("%s"), m_CalcDatalst.MB2);
	m_CalcDatalst.st05.Format(_T("%d"), m_CalcDatalst.ME2);
	m_CalcDatalst.st06.Format(_T("%d"), m_CalcDatalst.ME3);
	m_CalcDatalst.st07.Format(_T("%d"), m_CalcDatalst.ME5);
	m_CalcDatalst.st08.Format(_T("%d"), m_CalcDatalst.ME7);
	m_CalcDatalst.st09.Format(_T("%s"), m_CalcDatalst.ME8);
	m_CalcDatalst.st10.Format(_T("%s"), m_CalcDatalst.MB3);
	m_CalcDatalst.st11.Format(_T("%d"), m_CalcDatalst.MA2);
	m_CalcDatalst.st12.Format(_T("%d"), int(47-0.5f*m_CalcDatalst.MA6));
	m_CalcDatalst.st13.Format(_T("%d"), m_CalcDatalst.GD1 + 2*m_CalcDatalst.DB56 - 10);
	m_CalcDatalst.st14.Format(_T("%d"), m_CalcDatalst.DB59 + m_CalcDatalst.MB7 + m_CalcDatalst.MA7);
	m_CalcDatalst.st15.Format(_T("%d"), m_CalcDatalst.MA3);
	m_CalcDatalst.st16.Format(_T("%d"), m_CalcDatalst.MB7);
	m_CalcDatalst.st17.Format(_T("%d"), m_CalcDatalst.MA6);
	m_CalcDatalst.st18.Format(_T("%d"), m_CalcDatalst.DB57);

	m_CalcDatalst.st19.Format(_T("%d"), m_CalcDatalst.MA2 + m_CalcDatalst.DB52);
	m_CalcDatalst.st20.Format(_T("%d"), m_CalcDatalst.DB57 - 15);
	m_CalcDatalst.st21.Format(_T("%d"), m_CalcDatalst.MA2 - m_CalcDatalst.DB52 - 10);
	m_CalcDatalst.st22.Format(_T("%d"), int(m_CalcDatalst.MA2 - 94 - 0.5f*(m_CalcDatalst.MA4+m_CalcDatalst.MA6)));
	m_CalcDatalst.st23.Format(_T("%d"), m_CalcDatalst.GD1-2*(m_CalcDatalst.DB57-m_CalcDatalst.DB56) - 2*(m_CalcDatalst.MA3-m_CalcDatalst.MB7));
	m_CalcDatalst.st24.Format(_T("%d"), m_CalcDatalst.MA3 + m_CalcDatalst.MA5 + m_CalcDatalst.MA7);
	m_CalcDatalst.st25.Format(_T("%d"), m_CalcDatalst.MA3 - 20);
	m_CalcDatalst.st26.Format(_T("%d"), m_CalcDatalst.MA4);
	m_CalcDatalst.st27.Format(_T("%d"), m_CalcDatalst.MA7);
	m_CalcDatalst.st28.Format(_T("%d"), m_CalcDatalst.DB58);
	m_CalcDatalst.st29.Format(_T("%d"), m_CalcDatalst.MA2 + m_CalcDatalst.DB52 - 2*m_CalcDatalst.DB55);
	m_CalcDatalst.st30.Format(_T("%d"), int(m_CalcDatalst.DB52 * 0.5f + (47-0.5f*m_CalcDatalst.MA6)));
	m_CalcDatalst.st31.Format(_T("%d"), 47);
	m_CalcDatalst.st32.Format(_T("%d"), m_CalcDatalst.GD1+2*m_CalcDatalst.DB56 - 30);
	m_CalcDatalst.st33.Format(_T("%d"), m_CalcDatalst.MB7);
	m_CalcDatalst.st34.Format(_T("%d"), m_CalcDatalst.MB7 + 5);
	m_CalcDatalst.st35.Format(_T("%d"), m_CalcDatalst.MA5);
	m_CalcDatalst.st36.Format(_T("%d"), m_CalcDatalst.DB59);
	m_CalcDatalst.st37.Format(_T("%d"), m_CalcDatalst.DB56);
	m_CalcDatalst.st38.Format(_T("%d"), m_CalcDatalst.DB56 - 15);

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::LoadDataFromDatabase(CString strOfferID, CString strComponentName)
{
	CString strOperations;
	CADOTable tbComponent;
	strOperations.Format(_T("SELECT * FROM %s WHERE OfferNo='%s' AND ComponentName='%s'"), g_tcTableName[eTableID_ComponentInfo], strOfferID, strComponentName);
	if (!tbComponent.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations) || tbComponent.GetRecordCnt() == 0)
		return FALSE;

	// To check this component is calculated or not
	DWORD dwCalcFlag = 0;
	tbComponent.GetRecordCell(0, L"IsCalculated", dwCalcFlag);
	if (0 == dwCalcFlag)
	{
		tbComponent.CloseTable();
		return FALSE;
	}
	m_bIsEditData = TRUE;

	// Component type
	TCHAR tcTemp[256] = {_T("0")};
	tbComponent.GetRecordCell(0, L"ComponentType", tcTemp, 256);
	if (_tcsicmp(tcTemp, STR_COMTYPE_HOIST) == 0) m_bOnlyHoist = TRUE;
	else m_bOnlyHoist = FALSE;
	
	/* Read all user set information*/
	eCraneType eType;
	tbComponent.GetRecordCell(0, L"CraneType", (DWORD&)eType);
	switch(eType)
	{
	case eCraneType_GTD:
		m_CalcDatalst.GA1 = _T("BMD");
		break;
	case eCraneType_GTS:
		m_CalcDatalst.GA1 = _T("BMS");
		break;
	case eCraneType_GTZ:
		m_CalcDatalst.GA1 = _T("BMZ");
		break;
	case eCraneType_GTU:
		m_CalcDatalst.GA1 = _T("BMU");
		break;
	default:
		m_CalcDatalst.GA1 = _T("BMD");
	}
	tbComponent.GetRecordCell(0, L"HoistNumberType", (DWORD&)m_CalcDatalst.GA2);
	tbComponent.GetRecordCell(0, L"CraneLoad", (DWORD&)m_CalcDatalst.GA3);
	tbComponent.GetRecordCell(0, L"HoistLoad", (DWORD&)m_CalcDatalst.GA4);
	tbComponent.GetRecordCell(0, L"Span", m_CalcDatalst.GA5);
	tbComponent.GetRecordCell(0, L"HOL", m_CalcDatalst.GA6);
	if (tbComponent.GetRecordCell(0, L"CraneDuty", tcTemp, 256))	m_CalcDatalst.GA7 = tcTemp;
	if (tbComponent.GetRecordCell(0, L"HoistDuty", tcTemp, 256))	m_CalcDatalst.GA81 = tcTemp;
	if (tbComponent.GetRecordCell(0, L"RailType", tcTemp, 256))		m_CalcDatalst.GA9 = tcTemp;
	tbComponent.GetRecordCell(0, L"RailLength", m_CalcDatalst.GB1);
	tbComponent.GetRecordCell(0, L"RailTopToFloor", (DWORD&)m_CalcDatalst.GB2);
	tbComponent.GetRecordCell(0, L"RailRoofToTop", (DWORD&)m_CalcDatalst.GB3);
	tbComponent.GetRecordCell(0, L"PowerSupply", (DWORD&)m_CalcDatalst.GB4);
	tbComponent.GetRecordCell(0, L"ControlVol", (DWORD&)m_CalcDatalst.GB5);
	tbComponent.GetRecordCell(0, L"CraneCtrlType", tcTemp, 256); m_CalcDatalst.GB6 = tcTemp;
	tbComponent.GetRecordCell(0, L"MainUseType", tcTemp, 256); m_CalcDatalst.GB7 = tcTemp;
	if (tbComponent.GetRecordCell(0, L"HoistSpeed", tcTemp, 256))	m_CalcDatalst.GC1 = tcTemp;
	if (tbComponent.GetRecordCell(0, L"TrolleySpeed", tcTemp, 256))	m_CalcDatalst.GC2 = tcTemp;
	if (tbComponent.GetRecordCell(0, L"BridgeSpeed", tcTemp, 256))	m_CalcDatalst.GC3 = tcTemp;
	tbComponent.GetRecordCell(0, L"HoistCtrlType", tcTemp, 256); m_CalcDatalst.GC4 = tcTemp;
	tbComponent.GetRecordCell(0, L"TrolleyCtrlType", tcTemp, 256); m_CalcDatalst.GC5 = tcTemp;
	tbComponent.GetRecordCell(0, L"BridgeCtrlType", tcTemp, 256); m_CalcDatalst.GC6 = tcTemp;
	tbComponent.GetRecordCell(0, L"ReevingType", (DWORD&)m_CalcDatalst.HA7);
	LoadOtherBasicData();
	//
	if (tbComponent.GetRecordCell(0, L"HoistBrand", tcTemp, 256)) m_CalcDatalst.HA10 = tcTemp;
	if (tbComponent.GetRecordCell(0, L"HoistBrand_Aux", tcTemp, 256)) m_CalcDatalst.AU0 = tcTemp;
	if (tbComponent.GetRecordCell(0, L"HoistCode", tcTemp, 256)) m_CalcDatalst.HB201 = tcTemp;
	if (tbComponent.GetRecordCell(0, L"HoistCode_Aux", tcTemp, 256)) m_CalcDatalst.AU401 = tcTemp;
	tbComponent.GetRecordCell(0, L"HoistLoad_Aux", (DWORD&)m_CalcDatalst.AU1);	
	tbComponent.GetRecordCell(0, L"HoistTotalPrice", (DWORD&)m_CalcDatalst.HC1);
	tbComponent.GetRecordCell(0, L"HoistAdditions", tcTemp, 256);	FillHoistAdditionsList(tcTemp);
	LoadOtherHoistData();
	//
	if (!m_bOnlyHoist)
	{
		if (tbComponent.GetRecordCell(0, L"CraneCode", tcTemp, 256))		m_CalcDatalst.MA11 = tcTemp;
		if (tbComponent.GetRecordCell(0, L"MainGirderCode", tcTemp, 256))	m_CalcDatalst.MB1 = tcTemp;
		tbComponent.GetRecordCell(0, L"MainGirderPrice", (DWORD&)m_CalcDatalst.MF1);
		tbComponent.GetRecordCell(0, L"MG_W1", (DWORD&)m_CalcDatalst.MA2);
		tbComponent.GetRecordCell(0, L"MG_h1", (DWORD&)m_CalcDatalst.MA3);
		tbComponent.GetRecordCell(0, L"MG_t1", (DWORD&)m_CalcDatalst.MA4);
		tbComponent.GetRecordCell(0, L"MG_t2", (DWORD&)m_CalcDatalst.MA5);
		tbComponent.GetRecordCell(0, L"MG_t3", (DWORD&)m_CalcDatalst.MA6);
		tbComponent.GetRecordCell(0, L"MG_t4", (DWORD&)m_CalcDatalst.MA7);
		if (tbComponent.GetRecordCell(0, L"MG_TroRail", tcTemp, 256))		m_CalcDatalst.MB3 = tcTemp;
		tbComponent.GetRecordCell(0, L"MG_Matrial", (DWORD&)m_CalcDatalst.MB21);
		tbComponent.GetRecordCell(0, L"MG_TrolleyRail_W", (DWORD&)m_CalcDatalst.MB31);
		tbComponent.GetRecordCell(0, L"MG_TrolleyRail_H", (DWORD&)m_CalcDatalst.MB32);
		tbComponent.GetRecordCell(0, L"MG_w3", (DWORD&)m_CalcDatalst.MB4);
		tbComponent.GetRecordCell(0, L"MG_c1", (DWORD&)m_CalcDatalst.MB5);
		tbComponent.GetRecordCell(0, L"MG_t5", (DWORD&)m_CalcDatalst.MB6);
		tbComponent.GetRecordCell(0, L"MG_h2", (DWORD&)m_CalcDatalst.MB7);
		LoadOtherMainGriderData();
		//
		if (tbComponent.GetRecordCell(0, L"EC_Type", tcTemp, 256))			m_CalcDatalst.DB12 = tcTemp;
		if (tbComponent.GetRecordCell(0, L"EC_Code", tcTemp, 256))			m_CalcDatalst.DB2 = tcTemp;
		if (tbComponent.GetRecordCell(0, L"EC_Drive", tcTemp, 256))			m_CalcDatalst.DB3 = tcTemp;
		tbComponent.GetRecordCell(0, L"EC_MotorQty", (DWORD&)m_CalcDatalst.DB4);
		tbComponent.GetRecordCell(0, L"EC_Matrial", (DWORD&)m_CalcDatalst.DB61);
		if (tbComponent.GetRecordCell(0, L"EC_Structure", tcTemp, 256)) m_CalcDatalst.DB7 = tcTemp;
		tbComponent.GetRecordCell(0, L"EC_Wheelbase", (DWORD&)m_CalcDatalst.DB8);
		if (tbComponent.GetRecordCell(0, L"EC_RailGauge", tcTemp, 256)) m_CalcDatalst.DA5 = tcTemp;
		tbComponent.GetRecordCell(0, L"EC_TotalPrice", (DWORD&)m_CalcDatalst.DE41);
		if (tbComponent.GetRecordCell(0, L"EC_Additions", tcTemp, 256))		FillAdditionsList(tcTemp, m_lstEndCarAdditions);
		LoadOtherEndcarData();
		//
		if (tbComponent.GetRecordCell(0, L"CranePannelCode", tcTemp, 256))	m_CalcDatalst.EG1 = tcTemp;
		if (tbComponent.GetRecordCell(0, L"FastoonCode", tcTemp, 256))		m_CalcDatalst.EG2 = tcTemp;
		if (tbComponent.GetRecordCell(0, L"OperterUnitCode", tcTemp, 256))	m_CalcDatalst.EG3 = tcTemp;
		if (tbComponent.GetRecordCell(0, L"CranePowerSupplyCode", tcTemp, 256))		m_CalcDatalst.EG5 = tcTemp;
		tbComponent.GetRecordCell(0, L"CranePannelPrice", (DWORD&)m_CalcDatalst.EH1);
		tbComponent.GetRecordCell(0, L"FastoonPrice", (DWORD&)m_CalcDatalst.EH2);
		tbComponent.GetRecordCell(0, L"OperterUnitPrice", (DWORD&)m_CalcDatalst.EH3);
		tbComponent.GetRecordCell(0, L"CranePowerSupplyPrice", (DWORD&)m_CalcDatalst.EH5);
		if (tbComponent.GetRecordCell(0, L"EL_Addtions", tcTemp, 256))		FillAdditionsList(tcTemp, m_lstElecCtrlAdditions);
		LoadOtherElecData();
	}
	//
	tbComponent.CloseTable();

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::SaveDataToDatabase(CString strOfferID, CString strComponentName)
{
	CString strOperations;
	CADOTable tbComponent;
	strOperations.Format(_T("SELECT * FROM %s WHERE OfferNo='%s' AND ComponentName='%s'"), g_tcTableName[eTableID_ComponentInfo], strOfferID, strComponentName);
	if (!tbComponent.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations) || tbComponent.GetRecordCnt() == 0)
		return FALSE;

	// Set calculated flag to 1
	tbComponent.SetRecordCell(0, L"IsCalculated", (DWORD)1);

	eCraneType eType;
	if (_T("BMD") == m_CalcDatalst.GA1) eType = eCraneType_GTD;
	else if (_T("BMZ") == m_CalcDatalst.GA1) eType = eCraneType_GTZ;
	else if (_T("BMS") == m_CalcDatalst.GA1) eType = eCraneType_GTS;
	else eType = eCraneType_GTU;

	tbComponent.SetRecordCell(0, L"CraneType", (DWORD)eType);
	tbComponent.SetRecordCell(0, L"HoistNumberType", (DWORD)m_CalcDatalst.GA2);
	tbComponent.SetRecordCell(0, L"CraneLoad", (DWORD)m_CalcDatalst.GA3);
	tbComponent.SetRecordCell(0, L"HoistLoad", (DWORD)m_CalcDatalst.GA4);
	tbComponent.SetRecordCell(0, L"Span", (float)m_CalcDatalst.GA5);
	tbComponent.SetRecordCell(0, L"HOL", (float)m_CalcDatalst.GA6);
	tbComponent.SetRecordCell(0, L"CraneDuty", m_CalcDatalst.GA7.GetBuffer(m_CalcDatalst.GA7.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistDuty", m_CalcDatalst.GA81.GetBuffer(m_CalcDatalst.GA81.GetLength()));
	tbComponent.SetRecordCell(0, L"RailType", m_CalcDatalst.GA9.GetBuffer(m_CalcDatalst.GA9.GetLength()));
	tbComponent.SetRecordCell(0, L"RailLength", (float)m_CalcDatalst.GB1);
	tbComponent.SetRecordCell(0, L"RailTopToFloor", (DWORD)m_CalcDatalst.GB2);
	tbComponent.SetRecordCell(0, L"RailRoofToTop", (DWORD)m_CalcDatalst.GB3);
	tbComponent.SetRecordCell(0, L"PowerSupply", (DWORD)m_CalcDatalst.GB4);
	tbComponent.SetRecordCell(0, L"ControlVol", (DWORD)m_CalcDatalst.GB5);
	tbComponent.SetRecordCell(0, L"CraneCtrlType", m_CalcDatalst.GB6.GetBuffer(m_CalcDatalst.GB6.GetLength()));
	tbComponent.SetRecordCell(0, L"MainUseType", m_CalcDatalst.GB7.GetBuffer(m_CalcDatalst.GB7.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistSpeed", m_CalcDatalst.GC1.GetBuffer(m_CalcDatalst.GC1.GetLength()));
	tbComponent.SetRecordCell(0, L"TrolleySpeed", m_CalcDatalst.GC2.GetBuffer(m_CalcDatalst.GC2.GetLength()));
	tbComponent.SetRecordCell(0, L"BridgeSpeed", m_CalcDatalst.GC3.GetBuffer(m_CalcDatalst.GC3.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistCtrlType", m_CalcDatalst.GC4.GetBuffer(m_CalcDatalst.GC4.GetLength()));
	tbComponent.SetRecordCell(0, L"TrolleyCtrlType", m_CalcDatalst.GC5.GetBuffer(m_CalcDatalst.GC5.GetLength()));
	tbComponent.SetRecordCell(0, L"BridgeCtrlType", m_CalcDatalst.GC6.GetBuffer(m_CalcDatalst.GC6.GetLength()));
	tbComponent.SetRecordCell(0, L"ReevingType", (DWORD)m_CalcDatalst.HA7);
	//
	CString strHoistAddNames = GetHoistAdditionsNameString();
	tbComponent.SetRecordCell(0, L"HoistBrand", m_CalcDatalst.HA10.GetBuffer(m_CalcDatalst.HA10.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistBrand_Aux", m_CalcDatalst.AU0.GetBuffer(m_CalcDatalst.AU0.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistCode", m_CalcDatalst.HB201.GetBuffer(m_CalcDatalst.HB201.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistCode_Aux", m_CalcDatalst.AU401.GetBuffer(m_CalcDatalst.AU401.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistLoad_Aux", (DWORD)m_CalcDatalst.AU1);	
	tbComponent.SetRecordCell(0, L"HoistAdditions", strHoistAddNames.GetBuffer(strHoistAddNames.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistTotalPrice", (DWORD)m_CalcDatalst.HC1);
	//
	if (!m_bOnlyHoist)
	{
		tbComponent.SetRecordCell(0, L"CraneCode", m_CalcDatalst.MA11.GetBuffer(m_CalcDatalst.MA11.GetLength()));
		tbComponent.SetRecordCell(0, L"MainGirderCode", m_CalcDatalst.MB1.GetBuffer(m_CalcDatalst.MB1.GetLength()));
		tbComponent.SetRecordCell(0, L"MainGirderPrice", (DWORD)m_CalcDatalst.MF1);
		tbComponent.SetRecordCell(0, L"MG_W1", (DWORD)m_CalcDatalst.MA2);
		tbComponent.SetRecordCell(0, L"MG_h1", (DWORD)m_CalcDatalst.MA3);
		tbComponent.SetRecordCell(0, L"MG_t1", (DWORD)m_CalcDatalst.MA4);
		tbComponent.SetRecordCell(0, L"MG_t2", (DWORD)m_CalcDatalst.MA5);
		tbComponent.SetRecordCell(0, L"MG_t3", (DWORD)m_CalcDatalst.MA6);
		tbComponent.SetRecordCell(0, L"MG_t4", (DWORD)m_CalcDatalst.MA7);
		tbComponent.SetRecordCell(0, L"MG_TroRail", m_CalcDatalst.MB3.GetBuffer(m_CalcDatalst.MB3.GetLength()));	
		tbComponent.SetRecordCell(0, L"MG_Matrial", (DWORD)m_CalcDatalst.MB21);
		tbComponent.SetRecordCell(0, L"MG_TrolleyRail_W", (DWORD)m_CalcDatalst.MB31);
		tbComponent.SetRecordCell(0, L"MG_TrolleyRail_H", (DWORD)m_CalcDatalst.MB32);
		tbComponent.SetRecordCell(0, L"MG_w3", (DWORD)m_CalcDatalst.MB4);
		tbComponent.SetRecordCell(0, L"MG_c1", (DWORD)m_CalcDatalst.MB5);
		tbComponent.SetRecordCell(0, L"MG_t5", (DWORD)m_CalcDatalst.MB6);
		tbComponent.SetRecordCell(0, L"MG_h2", (DWORD)m_CalcDatalst.MB7);
		//
		CString strECAddNames = GetAdditionsNameString(m_lstEndCarAdditions);
		tbComponent.SetRecordCell(0, L"EC_Type", m_CalcDatalst.DB12.GetBuffer(m_CalcDatalst.DB12.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Code", m_CalcDatalst.DB2.GetBuffer(m_CalcDatalst.DB2.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Drive", m_CalcDatalst.DB3.GetBuffer(m_CalcDatalst.DB3.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_MotorQty", (DWORD)m_CalcDatalst.DB4);
		//tbComponent.SetRecordCell(0, L"EC_JointPlate", m_strJoiPlaType.GetBuffer(m_strJoiPlaType.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Matrial", (DWORD)m_CalcDatalst.DB61);
		tbComponent.SetRecordCell(0, L"EC_Structure", m_CalcDatalst.DB7.GetBuffer(m_CalcDatalst.DB7.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Wheelbase", (DWORD)m_CalcDatalst.DB8);
		tbComponent.SetRecordCell(0, L"EC_RailGauge", m_CalcDatalst.DA5.GetBuffer(m_CalcDatalst.DA5.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Additions", strECAddNames.GetBuffer(strECAddNames.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_TotalPrice", (DWORD)m_CalcDatalst.DE41);		
		//
		CString strELAddNames = GetAdditionsNameString(m_lstElecCtrlAdditions);
		tbComponent.SetRecordCell(0, L"CranePannelCode", m_CalcDatalst.EG1.GetBuffer(m_CalcDatalst.EG1.GetLength()));
		tbComponent.SetRecordCell(0, L"FastoonCode", m_CalcDatalst.EG2.GetBuffer(m_CalcDatalst.EG2.GetLength()));
		tbComponent.SetRecordCell(0, L"OperterUnitCode", m_CalcDatalst.EG3.GetBuffer(m_CalcDatalst.EG3.GetLength()));
		tbComponent.SetRecordCell(0, L"CranePowerSupplyCode", m_CalcDatalst.EG5.GetBuffer(m_CalcDatalst.EG5.GetLength()));
		tbComponent.SetRecordCell(0, L"EL_Addtions", strELAddNames.GetBuffer(strELAddNames.GetLength()));
		tbComponent.SetRecordCell(0, L"CranePannelPrice", (DWORD)m_CalcDatalst.EH1);		
		tbComponent.SetRecordCell(0, L"FastoonPrice", (DWORD)m_CalcDatalst.EH2);		
		tbComponent.SetRecordCell(0, L"OperterUnitPrice", (DWORD)m_CalcDatalst.EH3);		
		tbComponent.SetRecordCell(0, L"CranePowerSupplyPrice", (DWORD)m_CalcDatalst.EH5);		
	}
	//
	DWORD dwTotalPrice = m_CalcDatalst.GA2*m_CalcDatalst.HC1 + m_CalcDatalst.ME9 * m_CalcDatalst.ME5/1000.f + m_CalcDatalst.DE4 + m_iELAdditionsPrice + m_CalcDatalst.EH1 + m_CalcDatalst.EH2 + m_CalcDatalst.EH3 + m_CalcDatalst.EH5;
	tbComponent.SetRecordCell(0, L"ComponentPrice", dwTotalPrice);
	//
	BOOL bResult = FALSE;
	bResult = tbComponent.UpdateTable();
	tbComponent.CloseTable();

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::LoadOtherBasicData()
{
	if (_T("BMD") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.GA11 = _T("BMD");
		m_CalcDatalst.GA12 = _T("D");
		m_CalcDatalst.GA13 = _T("D");
	}
	else if (_T("BMZ") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.GA11 = _T("BMD");
		m_CalcDatalst.GA12 = _T("D");
		m_CalcDatalst.GA13 = _T("Z");
	}
	else
	{
		m_CalcDatalst.GA11 = _T("BMS");
		m_CalcDatalst.GA12 = _T("S");
		m_CalcDatalst.GA13 = _T("S");	
	}
	
	CString strTableName;
	if (_T("D") == m_CalcDatalst.GA12)
		strTableName = g_tcTableName[eTableID_HoiSel_D];
	else
		strTableName = g_tcTableName[eTableID_HoiSel_S];
	m_CalcDatalst.GA31 = GetNearBigValue(strTableName, _T("HoiLod"), m_CalcDatalst.GA3);

	m_CalcDatalst.GA51.Format(_T("%3d"), int(m_CalcDatalst.GA5 * 10));
	m_CalcDatalst.GA52 = GetNearBigValue(g_tcTableName[eTableID_CraneData], _T("CraSpa"), m_CalcDatalst.GA5);
	
	m_CalcDatalst.GA61.Format(_T("%03d"), int(m_CalcDatalst.GA6*10));

	int iPos = m_CalcDatalst.GA81.Find(_T('/'));
	if (iPos != -1)	m_CalcDatalst.GA8 = m_CalcDatalst.GA81.Right(m_CalcDatalst.GA81.GetLength() - (iPos+1));
	m_CalcDatalst.GA8 = _T("M5");

	m_CalcDatalst.GB11.Format(_T("%03d"), int(m_CalcDatalst.GB1+0.5f));

	m_CalcDatalst.GC31 = (int)GetLeftValueOfRangeString(m_CalcDatalst.GC3);

	if (_T("2-SPEED") == m_CalcDatalst.GC5) m_CalcDatalst.GC51 = _T('P');
	 m_CalcDatalst.GC51 = _T('T');

	if (_T("2-SPEED") == m_CalcDatalst.GC6) m_CalcDatalst.GC61 = _T('P');
	 m_CalcDatalst.GC61 = _T('T');

	 UpdateBasicDrawingData();

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::LoadOtherHoistData()
{	
	m_CalcDatalst.HA1.Format(_T("%s%s"), g_LoginUserInfo.C38, m_CalcDatalst.GA13);
	m_CalcDatalst.HA2 = m_CalcDatalst.GA8;
	m_CalcDatalst.HA3 = m_CalcDatalst.GA4;
	m_CalcDatalst.HA4 = m_CalcDatalst.GA6;
	m_CalcDatalst.HA5 = m_CalcDatalst.GC1;
	m_CalcDatalst.HA6 = m_CalcDatalst.GC2;
	m_CalcDatalst.HB8 = m_CalcDatalst.GA2;

	if (_T("BM") == m_CalcDatalst.HA10)	m_CalcDatalst.HA101 = _T("BM");
	else if (_T("GT") == m_CalcDatalst.HA10) m_CalcDatalst.HA101 = _T("BM");
	else if (_T("NS") == m_CalcDatalst.HA10) m_CalcDatalst.HA101 = _T("NS");
	else if (_T("NF") == m_CalcDatalst.HA10) m_CalcDatalst.HA101 = _T("NF");

	m_CalcDatalst.HA8 = LoadHoistRaiGaugeData();
	ParserSelMainHoistData();

	// Has Aux. hoist
	if (_T("Z") == m_CalcDatalst.GA13 && m_CalcDatalst.AU401.GetLength() > 0)
	{
		if (_T("BM") == m_CalcDatalst.AU0 || _T("GT") == m_CalcDatalst.AU0) m_CalcDatalst.AU01 = _T("BM");
		else m_CalcDatalst.AU01 = _T("NF");
		ParserSelAuxHoistData();		
		
		m_CalcDatalst.AU7 = m_CalcDatalst.AU5 + m_CalcDatalst.AU6;
		m_CalcDatalst.HB4 = m_CalcDatalst.AU4;
		m_CalcDatalst.HB5 = m_CalcDatalst.AU7;
	}
	m_CalcDatalst.HB9 = (m_CalcDatalst.HB7 + m_CalcDatalst.HB3 + m_CalcDatalst.HB5);
	
	m_CalcDatalst.HD03 = m_CalcDatalst.HB203 +
											 m_CalcDatalst.HB204 +
											 m_CalcDatalst.AU403 +
											 m_CalcDatalst.AU404;

	CString strSQL;
	CADOTable tbSearch;
	strSQL.Format(_T("SELECT * FROM %s WHERE MainAux='%s'"), g_tcTableName[eTableID_MainAuxH], m_CalcDatalst.HD03);
	if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
	{
		TCHAR tcTemp[256] = {0};
		tbSearch.GetRecordCell(0, L"HookDim", (DWORD&)m_CalcDatalst.HD04);
		tbSearch.GetRecordCell(0, L"mb", (DWORD&)m_CalcDatalst.HD05);
		tbSearch.GetRecordCell(0, L"mb1", (DWORD&)m_CalcDatalst.HD06);
		tbSearch.GetRecordCell(0, L"mb2", (DWORD&)m_CalcDatalst.HD07);
	}
	tbSearch.CloseTable();
	
	if (_T("D") == m_CalcDatalst.GA13 || _T("S") == m_CalcDatalst.GA13)
	{
		m_CalcDatalst.HD01 = m_CalcDatalst.HB227;
		m_CalcDatalst.HD02 = m_CalcDatalst.HB2;
		m_CalcDatalst.HD08 = m_CalcDatalst.HB228;
		m_CalcDatalst.HD09 = m_CalcDatalst.HB229;
		m_CalcDatalst.HD10 = m_CalcDatalst.HB230;
	}
	else
	{
		m_CalcDatalst.HD01 = m_CalcDatalst.HB227 + m_CalcDatalst.AU419;
		m_CalcDatalst.HD02 = m_CalcDatalst.HB2 + _T("+") + m_CalcDatalst.AU4;
		m_CalcDatalst.HD08 = m_CalcDatalst.HD05;
		m_CalcDatalst.HD09 = m_CalcDatalst.HD06;
		m_CalcDatalst.HD10 = m_CalcDatalst.HD07;
	}

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::LoadOtherMainGriderData()
{
	m_CalcDatalst.MB2.Format(_T("Q%d"), m_CalcDatalst.MB21);
	UpdateRailTypeData(FALSE);
	//
	if (_T("BMD") == m_CalcDatalst.GA1 || _T("BMZ") == m_CalcDatalst.GA1) 
	{
		m_CalcDatalst.MB5 = m_CalcDatalst.HD09;
		m_CalcDatalst.MB51 = m_CalcDatalst.HD10;
	}
	else if (_T("BMS") == m_CalcDatalst.GA1)
	{
		m_CalcDatalst.MB5 = m_CalcDatalst.HB231 + m_CalcDatalst.HB238;
		m_CalcDatalst.MB51 = m_CalcDatalst.HB231 + m_CalcDatalst.HB233 + m_CalcDatalst.HB237;
	}
	else	//"BMU"
	{
		m_CalcDatalst.MB5 = 0;
		m_CalcDatalst.MB51 = 0;
	}
	m_CalcDatalst.MB7 = GetGirder_h2(FALSE);

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::LoadOtherEndcarData()
{
	m_CalcDatalst.DA4 = m_CalcDatalst.GA9;
	UpdateRailTypeData(TRUE);

	m_CalcDatalst.DB6.Format(_T("Q%d"), m_CalcDatalst.DB61);
	m_CalcDatalst.DC5 = m_CalcDatalst.DA41 + m_CalcDatalst.P23;	
	if (_T("BMD") == m_CalcDatalst.GA11) 
	{
		long lGaiGuage = _ttol(m_CalcDatalst.DA5) / 100;
		m_CalcDatalst.DA51.Format(_T("%d"), lGaiGuage);
	}
	else 
		m_CalcDatalst.DA51 = _T("00");

	m_CalcDatalst.DA11 = m_CalcDatalst.DB2;
	m_CalcDatalst.DA12 = m_CalcDatalst.DB3;
	m_CalcDatalst.DA13 = m_CalcDatalst.DB3.GetAt(5);
	m_CalcDatalst.DA14 = m_CalcDatalst.DA11 + m_CalcDatalst.DA13;
	m_CalcDatalst.DA15 = m_CalcDatalst.DA11.GetAt(0);
	m_CalcDatalst.DA16 = m_CalcDatalst.DB4;
	m_CalcDatalst.DA2.Format(_T("%s-%s"), m_CalcDatalst.DA11, m_CalcDatalst.DB3);
		
	if (m_CalcDatalst.DB12 == _T("D")) m_CalcDatalst.DB11 = 2;
	else if (m_CalcDatalst.DB12 == _T("B"))	m_CalcDatalst.DB11 = 4;
	else m_CalcDatalst.DB11 = 2;

	CString strDB22;
	m_CalcDatalst.DB21.Format(_T("%c%c"), m_CalcDatalst.DB2.GetAt(1), m_CalcDatalst.DB2.GetAt(2));
	m_CalcDatalst.DB23.Format(_T("%c"), m_CalcDatalst.DB2.GetAt(0));
	m_CalcDatalst.DB24.Format(_T("%c%c%c"), m_CalcDatalst.DB2.GetAt(0), m_CalcDatalst.DB2.GetAt(1), m_CalcDatalst.DB2.GetAt(2));
	strDB22.Format(_T("%c%c"), m_CalcDatalst.DB2.GetAt(3), m_CalcDatalst.DB2.GetAt(4));
	m_CalcDatalst.DB22 = _ttol(strDB22);

	if (4 == m_CalcDatalst.DB4) m_CalcDatalst.DB41 = _T("=");
	else m_CalcDatalst.DB41 = _T("-");

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodEndGer='%s'"), g_tcTableName[eTableID_EndCarSteMat], m_CalcDatalst.DA14);
	m_CalcDatalst.DA3 = GetStringValueFromTable(strSQL, _T("EndStrSec"));
	

	UpdateDriverTypeData();
	UpdateJointPlateData();
	UpdateRailData();
	UpdateWheelBaseData(FALSE);
	UpdateBufferExtendData();
	UpdateBufferTypeData();
	UpdateECStructureData();
	UpdateEndCarDataText();

	// Endcar total power KW
	m_CalcDatalst.DB42 = m_CalcDatalst.DB35 * m_CalcDatalst.DB4;

	strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_CalcDatalst.DB3);	
	m_CalcDatalst.DE5 = GetLONGValueFromTable(strSQL, _T("PriTraDriRmb"));
	m_CalcDatalst.DE5 *= m_CalcDatalst.DB4;

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::LoadOtherElecData()
{
	UpdateElecCodeData();	

	if (_T("Pandent") == m_CalcDatalst.EC1)
	{
		m_CalcDatalst.EC11 = _T("GG");
		m_CalcDatalst.EC12 = _T("Pendant");
		m_CalcDatalst.EC13 = _T("P");
	}
	else if (_T("Radio-H") == m_CalcDatalst.EC1)
	{
		m_CalcDatalst.EC11 = _T("RH");
		m_CalcDatalst.EC12 = _T("Remote + Backup Pandent");
		m_CalcDatalst.EC13 = _T("R");
	}
	else if (_T("Radio-Y") == m_CalcDatalst.EC1)
	{
		m_CalcDatalst.EC11 = _T("RY");
		m_CalcDatalst.EC12 = _T("Remote + Backup Pandent");
		m_CalcDatalst.EC13 = _T("R");
	}
	else
	{
		m_CalcDatalst.EC11 = _T("RB");
		m_CalcDatalst.EC12 = _T("Remote + Backup Pandent");
		m_CalcDatalst.EC13 = _T("R");
	}
	
	UpdateElecPannelselection();

	return TRUE;
}
//-----------------------------------------------------------------------
CString CBMParamCalculator::LoadHoistRaiGaugeData()
{
	CString strRaiGauge = 0;

	CString strSQL;
	CADOTable tbHoist;
	CString strHoistTableName = GetHoistSelTableName(m_CalcDatalst.GA12, m_CalcDatalst.HA10);
	strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), strHoistTableName, m_CalcDatalst.HB201);
	if (tbHoist.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbHoist.GetRecordCnt() > 0)
	{
		TCHAR tcRaiGauge[32] = {_T("\0")};
		tbHoist.GetRecordCell(0, L"HoiLod", (DWORD&)m_CalcDatalst.GA31);
		tbHoist.GetRecordCell(0, L"HOL", m_CalcDatalst.HB224);
		tbHoist.GetRecordCell(0, L"B0", tcRaiGauge, 32);
		//
		if (_T("S") == m_CalcDatalst.GA12)
		{
			strRaiGauge = tcRaiGauge;
		}
		else
		{
			int iAddGauageLevel = 0;
			int iRailGauage = _ttol(tcRaiGauge);
			for (list<CString>::iterator it = m_lstOtherHoistAdditions.begin();
				it != m_lstOtherHoistAdditions.end();
				it++)
			{
				CString strName = *it;
				if (strName.Find(STR_HOIST_ADDGAU) != -1)
				{
					CString strAddGauLevel = strName.Right(2);
					iAddGauageLevel += _ttol(strAddGauLevel);
				}
			}

			// Update Rail Gauage
			int iCurLevel = 0;
			int iSelHoistGauage = _ttol(tcRaiGauge);
			for (int i = 0; i < sizeof(g_iRailGauage) / sizeof(g_iRailGauage[0]); i++)
			{
				if (iSelHoistGauage > g_iRailGauage[i])
					continue;
				iCurLevel = i;
				break;
			}
			int iNewLevel = iCurLevel + iAddGauageLevel;
			iNewLevel = min(iNewLevel, sizeof(g_iRailGauage) / sizeof(g_iRailGauage[0]) - 1);
			int iNewRailGauage = g_iRailGauage[iNewLevel];
			strRaiGauge.Format(_T("%d"), iNewRailGauage);
		}
	}
	tbHoist.CloseTable();

	return strRaiGauge;
}
//-----------------------------------------------------------------------
void CBMParamCalculator::ParserSelMainHoistData()
{
	if (_T("BM") == m_CalcDatalst.HA101)
	{
		m_CalcDatalst.HB202.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(2));
		m_CalcDatalst.HB203.Format(_T("%c%c%c"), m_CalcDatalst.HB201.GetAt(0), m_CalcDatalst.HB201.GetAt(1), m_CalcDatalst.HB201.GetAt(2));
		m_CalcDatalst.HB204.Format(_T("%c%c"), m_CalcDatalst.HB201.GetAt(3), m_CalcDatalst.HB201.GetAt(4));
		m_CalcDatalst.HB204A.Format(_T("%c%c%c"), m_CalcDatalst.HB201.GetAt(5), m_CalcDatalst.HB201.GetAt(6), m_CalcDatalst.HB201.GetAt(7));
		m_CalcDatalst.HB205.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(8));
		m_CalcDatalst.HB206.Format(_T("%c%c"), m_CalcDatalst.HB201.GetAt(8), m_CalcDatalst.HB201.GetAt(9));
		m_CalcDatalst.HB207.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(10));
		m_CalcDatalst.HB208.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(11));
		m_CalcDatalst.HB209.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(12));
		m_CalcDatalst.HB210.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(13));
		//
		m_CalcDatalst.HB2 = m_CalcDatalst.HA10 + 
							m_CalcDatalst.HB202 +
							m_CalcDatalst.HB204 +
							m_CalcDatalst.HB204A +
							m_CalcDatalst.HB206 +
							m_CalcDatalst.HB207 +
							m_CalcDatalst.HB208 +
							m_CalcDatalst.HB209 +
							m_CalcDatalst.HB210;
		//
		if (_T("T") == m_CalcDatalst.HB205) m_CalcDatalst.HB211 = _T("Inverter");
		else m_CalcDatalst.HB211 = _T("2-Speed");
		//
		m_CalcDatalst.HB212 = m_CalcDatalst.HB203 +
							  m_CalcDatalst.HB206 +
							  m_CalcDatalst.HB207 +
							  m_CalcDatalst.HB208;
		m_CalcDatalst.HB213 = m_CalcDatalst.HB203 +
							  m_CalcDatalst.HB204 +
							  m_CalcDatalst.HB209 +
							  m_CalcDatalst.HB210;

		CString strSQL;
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod='%s'"), g_tcTableName[eTableID_TroDriTyp], m_CalcDatalst.HB213);
		m_CalcDatalst.HB214 = GetStringValueFromTable(strSQL, _T("TroDriTyp"));
		m_CalcDatalst.HB215 = GetLONGValueFromTable(strSQL, _T("TroDriNum"));
		
		strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_CalcDatalst.HB214);
		m_CalcDatalst.HB216 = GetFLOATValueFromTable(strSQL, _T("PowTraDri"));

		strSQL.Format(_T("SELECT * FROM %s WHERE HoiMacIndCod='%s'"), g_tcTableName[eTableID_HoiMacIndCod], m_CalcDatalst.HB212);
		m_CalcDatalst.HB217 = GetFLOATValueFromTable(strSQL, _T("PowHoiMotH"));
		m_CalcDatalst.HB218 = GetStringValueFromTable(strSQL, _T("PowHoiMot"));

		CString strTable;
		CADOTable tbSearch;
		if (_T("D") == m_CalcDatalst.GA12) strTable = g_tcTableName[eTableID_HoiSel_D];
		else strTable = g_tcTableName[eTableID_HoiSel_S];
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), strTable, m_CalcDatalst.HB201);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"HoiSpe", tcTemp, 256); m_CalcDatalst.HB219 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiSpe1", (DWORD&)m_CalcDatalst.HB220);
			tbSearch.GetRecordCell(0, L"B0", tcTemp, 256); m_CalcDatalst.HB221 = _ttol(tcTemp); 
			tbSearch.GetRecordCell(0, L"DutCla", tcTemp, 256); m_CalcDatalst.HB222 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiMacMot", tcTemp, 256); m_CalcDatalst.HB223 = tcTemp;
			tbSearch.GetRecordCell(0, L"HOL", m_CalcDatalst.HB224);
			tbSearch.GetRecordCell(0, L"PriLisHoi", (DWORD&)m_CalcDatalst.HB225);
			tbSearch.GetRecordCell(0, L"Stock", tcTemp, 256); m_CalcDatalst.HB226 = tcTemp;
		}
		tbSearch.CloseTable();
		//
		if (_T("D") == m_CalcDatalst.GA12) strTable = g_tcTableName[eTableID_DimHoi_D];
		else strTable = g_tcTableName[eTableID_DimHoi_S];
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod='%s'"), strTable, m_CalcDatalst.HB213);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"WgtHoi", (DWORD&)m_CalcDatalst.HB227);
			tbSearch.GetRecordCell(0, L"b", (DWORD&)m_CalcDatalst.HB228);
			tbSearch.GetRecordCell(0, L"b1", (DWORD&)m_CalcDatalst.HB229); 
			tbSearch.GetRecordCell(0, L"b2", (DWORD&)m_CalcDatalst.HB230); 
			tbSearch.GetRecordCell(0, L"H2", (DWORD&)m_CalcDatalst.HB231); 
			tbSearch.GetRecordCell(0, L"H3", (DWORD&)m_CalcDatalst.HB232); 
			tbSearch.GetRecordCell(0, L"W", (DWORD&)m_CalcDatalst.HB233); 
			tbSearch.GetRecordCell(0, L"H1", (DWORD&)m_CalcDatalst.HB234); 
			tbSearch.GetRecordCell(0, L"C", (DWORD&)m_CalcDatalst.HB235); 
			tbSearch.GetRecordCell(0, L"S4", (DWORD&)m_CalcDatalst.HB236); 
			tbSearch.GetRecordCell(0, L"S1", (DWORD&)m_CalcDatalst.HB237); 
			tbSearch.GetRecordCell(0, L"S2", (DWORD&)m_CalcDatalst.HB238); 
			tbSearch.GetRecordCell(0, L"S3", (DWORD&)m_CalcDatalst.HB239); 
		}
		tbSearch.CloseTable();
		m_CalcDatalst.HB240 = m_CalcDatalst.HA10 + m_CalcDatalst.HB202;
		m_CalcDatalst.HB241 = m_CalcDatalst.HB215 * m_CalcDatalst.HB216;
	
	}
	else if (_T("NS") == m_CalcDatalst.HA101 ||
			 _T("NF") == m_CalcDatalst.HA101)
	{
		m_CalcDatalst.HB2 = m_CalcDatalst.HB201;

		m_CalcDatalst.HB202.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(1));
		m_CalcDatalst.HB203.Format(_T("%c%c"), m_CalcDatalst.HB201.GetAt(0), m_CalcDatalst.HB201.GetAt(1));
		m_CalcDatalst.HB205.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(8));
		m_CalcDatalst.HB206.Format(_T("%c%c"), m_CalcDatalst.HB201.GetAt(8), m_CalcDatalst.HB201.GetAt(9));
		m_CalcDatalst.HB207.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(5));
		m_CalcDatalst.HB208.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(7));
		m_CalcDatalst.HB209.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(4));
		m_CalcDatalst.HB210.Format(_T("%c"), m_CalcDatalst.HB201.GetAt(6));
		//
		if (_T("T") == m_CalcDatalst.HB205) m_CalcDatalst.HB211 = _T("Inverter");
		else m_CalcDatalst.HB211 = _T("2-Speed");
		//

		CString strSQL;
		CString strTable;
		CADOTable tbSearch;
		strTable = GetHoistSelTableName(m_CalcDatalst.GA12, m_CalcDatalst.HA10);
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), strTable, m_CalcDatalst.HB201);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"Reeving", tcTemp, 256); m_CalcDatalst.HB204 = tcTemp;
			tbSearch.GetRecordCell(0, L"TroDriTyp", tcTemp, 256); m_CalcDatalst.HB214 = tcTemp;
			tbSearch.GetRecordCell(0, L"TroDriNum", (DWORD&)m_CalcDatalst.HB215); 
			tbSearch.GetRecordCell(0, L"PowTraDri", m_CalcDatalst.HB216); 
			tbSearch.GetRecordCell(0, L"PowHoiMotH", m_CalcDatalst.HB217); 
			tbSearch.GetRecordCell(0, L"PowHoiMot", tcTemp, 256); m_CalcDatalst.HB218 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiSpe",  tcTemp, 256); m_CalcDatalst.HB219 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiSpe1", (DWORD&)m_CalcDatalst.HB220);
			tbSearch.GetRecordCell(0, L"B0", tcTemp, 256); m_CalcDatalst.HB221 = _ttol(tcTemp);
			tbSearch.GetRecordCell(0, L"DutCla", tcTemp, 256); m_CalcDatalst.HB222 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiMacMot", tcTemp, 256); m_CalcDatalst.HB223 = tcTemp;
			tbSearch.GetRecordCell(0, L"HOL", m_CalcDatalst.HB224);
			tbSearch.GetRecordCell(0, L"PriLisHoi", (DWORD&)m_CalcDatalst.HB225);
			tbSearch.GetRecordCell(0, L"Stock", tcTemp, 256); m_CalcDatalst.HB226 = tcTemp;
			//
			tbSearch.GetRecordCell(0, L"WgtHoi", (DWORD&)m_CalcDatalst.HB227);
			tbSearch.GetRecordCell(0, L"b", (DWORD&)m_CalcDatalst.HB228);
			tbSearch.GetRecordCell(0, L"b1", (DWORD&)m_CalcDatalst.HB229); 
			tbSearch.GetRecordCell(0, L"b2", (DWORD&)m_CalcDatalst.HB230); 
			tbSearch.GetRecordCell(0, L"H2", (DWORD&)m_CalcDatalst.HB231); 
			tbSearch.GetRecordCell(0, L"H3", (DWORD&)m_CalcDatalst.HB232); 
			tbSearch.GetRecordCell(0, L"W", (DWORD&)m_CalcDatalst.HB233); 
			tbSearch.GetRecordCell(0, L"H1", (DWORD&)m_CalcDatalst.HB234); 
			tbSearch.GetRecordCell(0, L"C", (DWORD&)m_CalcDatalst.HB235); 
			tbSearch.GetRecordCell(0, L"S4", (DWORD&)m_CalcDatalst.HB236); 
			tbSearch.GetRecordCell(0, L"S1", (DWORD&)m_CalcDatalst.HB237); 
			tbSearch.GetRecordCell(0, L"S2", (DWORD&)m_CalcDatalst.HB238); 
			tbSearch.GetRecordCell(0, L"S3", (DWORD&)m_CalcDatalst.HB239); 
		}
		tbSearch.CloseTable();
		m_CalcDatalst.HB240 = m_CalcDatalst.HB203;
		m_CalcDatalst.HB241 = m_CalcDatalst.HB215 * m_CalcDatalst.HB216;

		strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_CalcDatalst.HB214);
		m_CalcDatalst.HB216 = GetFLOATValueFromTable(strSQL, _T("PowTraDri"));
	}

	if (_T("BM") == m_CalcDatalst.HA101) m_CalcDatalst.HB3 = m_CalcDatalst.HB225;
	else if (_T("NS") == m_CalcDatalst.HA101) m_CalcDatalst.HB3 = m_CalcDatalst.HB225 * m_CalcDatalst.P25;
	else m_CalcDatalst.HB3 = m_CalcDatalst.HB225 * m_CalcDatalst.P26;
}
//-----------------------------------------------------------------------
void CBMParamCalculator::ParserSelAuxHoistData()
{
	if (_T("BM") == m_CalcDatalst.AU01)
	{
		m_CalcDatalst.AU402.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(2));
		m_CalcDatalst.AU403.Format(_T("%c%c%c"), m_CalcDatalst.AU401.GetAt(0), m_CalcDatalst.AU401.GetAt(1), m_CalcDatalst.AU401.GetAt(2));
		m_CalcDatalst.AU404.Format(_T("%c%c"), m_CalcDatalst.AU401.GetAt(3), m_CalcDatalst.AU401.GetAt(4));
		m_CalcDatalst.AU404A.Format(_T("%c%c%c"), m_CalcDatalst.AU401.GetAt(5), m_CalcDatalst.AU401.GetAt(6), m_CalcDatalst.AU401.GetAt(7));
		m_CalcDatalst.AU405.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(8));
		m_CalcDatalst.AU406.Format(_T("%c%c"), m_CalcDatalst.AU401.GetAt(8), m_CalcDatalst.AU401.GetAt(9));
		m_CalcDatalst.AU407.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(10));
		m_CalcDatalst.AU408.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(11));
		m_CalcDatalst.AU409.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(12));
		m_CalcDatalst.AU410.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(13));
		//
		m_CalcDatalst.AU4 =	m_CalcDatalst.AU0 + 
							m_CalcDatalst.AU402 +
							m_CalcDatalst.AU404 +
							m_CalcDatalst.AU404A +
							m_CalcDatalst.AU406 +
							m_CalcDatalst.AU407 +
							m_CalcDatalst.AU408 +
							m_CalcDatalst.AU409 +
							m_CalcDatalst.AU410;
		m_CalcDatalst.AU411 = m_CalcDatalst.AU403 +
							  m_CalcDatalst.AU406 +
							  m_CalcDatalst.AU407 +
							  m_CalcDatalst.AU408;
		m_CalcDatalst.AU412 = m_CalcDatalst.AU403 +
							  m_CalcDatalst.AU404 +
							  m_CalcDatalst.AU409 +
							  m_CalcDatalst.AU410;

		CString strSQL;
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiMacIndCod='%s'"), g_tcTableName[eTableID_HoiMacIndCod], m_CalcDatalst.AU411);
		m_CalcDatalst.AU413 = GetFLOATValueFromTable(strSQL, _T("PowHoiMotH"));
		m_CalcDatalst.AU414 = GetStringValueFromTable(strSQL, _T("PowHoiMot"));

		CString strTable;
		CADOTable tbSearch;
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), g_tcTableName[eTableID_HoiSel_F], m_CalcDatalst.AU401);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"HoiSpe", tcTemp, 256); m_CalcDatalst.AU415 = tcTemp;
			tbSearch.GetRecordCell(0, L"DutCla", tcTemp, 256); m_CalcDatalst.AU416 = tcTemp;
			tbSearch.GetRecordCell(0, L"HOL", m_CalcDatalst.AU417);
			tbSearch.GetRecordCell(0, L"PriLisHoi", (DWORD&)m_CalcDatalst.AU418);
		}
		tbSearch.CloseTable();
		//
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod='%s'"), g_tcTableName[eTableID_DimHoi_F], m_CalcDatalst.AU412);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"WgtHoi", (DWORD&)m_CalcDatalst.AU419);
			tbSearch.GetRecordCell(0, L"C", (DWORD&)m_CalcDatalst.AU420); 
		}
		tbSearch.CloseTable();		
		if (_T("T") == m_CalcDatalst.AU405) m_CalcDatalst.AU421 = _T("Inverter");
		else m_CalcDatalst.AU421 = _T("2-Speed");
		m_CalcDatalst.AU422 = m_CalcDatalst.AU0 + m_CalcDatalst.AU402;	
	}
	else if (_T("NF") == m_CalcDatalst.AU01)
	{
		m_CalcDatalst.AU4 = m_CalcDatalst.AU401;

		m_CalcDatalst.AU402.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(1));
		m_CalcDatalst.AU403.Format(_T("%c%c"), m_CalcDatalst.AU401.GetAt(0), m_CalcDatalst.AU401.GetAt(1));
		m_CalcDatalst.AU405.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(8));
		m_CalcDatalst.AU406.Format(_T("%c%c"), m_CalcDatalst.AU401.GetAt(8), m_CalcDatalst.AU401.GetAt(9));
		m_CalcDatalst.AU407.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(5));
		m_CalcDatalst.AU408.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(7));
		m_CalcDatalst.AU409.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(4));
		m_CalcDatalst.AU410.Format(_T("%c"), m_CalcDatalst.AU401.GetAt(6));
		//
		CString strSQL;
		CADOTable tbSearch;
		strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), g_tcTableName[eTableID_SHoiSel_F], m_CalcDatalst.AU401);
		if (tbSearch.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbSearch.GetRecordCnt() > 0)
		{
			TCHAR tcTemp[256] = {0};
			tbSearch.GetRecordCell(0, L"Reeving", tcTemp, 256); m_CalcDatalst.AU404 = tcTemp;
			tbSearch.GetRecordCell(0, L"PowHoiMotH", m_CalcDatalst.AU413); 
			tbSearch.GetRecordCell(0, L"PowHoiMot", tcTemp, 256); m_CalcDatalst.AU414 = tcTemp;
			tbSearch.GetRecordCell(0, L"HoiSpe",  tcTemp, 256); m_CalcDatalst.AU415 = tcTemp;
			tbSearch.GetRecordCell(0, L"DutCla", tcTemp, 256); m_CalcDatalst.AU416 = tcTemp;
			tbSearch.GetRecordCell(0, L"HOL", m_CalcDatalst.AU417);
			tbSearch.GetRecordCell(0, L"PriLisHoi", (DWORD&)m_CalcDatalst.AU418);
			//
			tbSearch.GetRecordCell(0, L"WgtHoi", (DWORD&)m_CalcDatalst.AU419);
			tbSearch.GetRecordCell(0, L"C", (DWORD&)m_CalcDatalst.AU420); 
		}
		tbSearch.CloseTable();
		if (_T("T") == m_CalcDatalst.AU405) m_CalcDatalst.AU421 = _T("Inverter");
		else m_CalcDatalst.AU421 = _T("2-Speed");
		m_CalcDatalst.AU422 = m_CalcDatalst.AU403;
	}

	if (_T("BM") == m_CalcDatalst.AU01) m_CalcDatalst.AU5 = m_CalcDatalst.AU418;
	else  m_CalcDatalst.AU5 = m_CalcDatalst.AU418 * m_CalcDatalst.P26;
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
float CBMParamCalculator::GetLeftValueOfRangeString(CString strRange)
{
	float fLeftValue = 0.0f;

	if (!strRange.IsEmpty())
	{
		int iPos = strRange.Find(_T('/'));
		if (iPos != -1)
		{
			CString strLeft = strRange.Left(iPos);
			fLeftValue = _tstof(strLeft);
		}
	}

	return fLeftValue;
}
//-----------------------------------------------------------------------
float CBMParamCalculator::GetRightValueOfRangeString(CString strRange)
{
	float fRightValue = 0.0f;

	if (!strRange.IsEmpty())
	{
		int iPos = strRange.Find(_T('/'));
		if (iPos != -1)
		{
			CString strRight = strRange.Right(strRange.GetLength() - (iPos+1));
			fRightValue = _tstof(strRight);
		}
	}

	return fRightValue;			
}
//-----------------------------------------------------------------------
bool CBMParamCalculator::GetMinAndClearanceSize(CString strTypeCode, 
												long lCraneLoad, 
												int &iMinSize, 
												int &iClearance)
{
	CString strParamName;
	if (_T("BMD") == strTypeCode) 
		strParamName.Format(_T("%s_%s_"), STR_BMD, STR_CALCPARAM_LOAD);
	else
		strParamName.Format(_T("%s_%s_"), STR_BMS, STR_CALCPARAM_LOAD);

	CString strOperations;
	CADOTable tmCalcParams;
	strOperations.Format(_T("SELECT * FROM %s WHERE ParamName LIKE '%s%s'"), g_tcTableName[eTableID_CalcParams], strParamName, _T("%"));
	if (TRUE == tmCalcParams.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOperations))
	{
		DWORD dwCount = tmCalcParams.GetRecordCnt();
		for (DWORD dwIndex = 0; dwIndex < dwCount; dwIndex++)
		{
			DWORD dwLoad = 0;
			CString strLoad;
			TCHAR tcLoad[32] = {_T("\0")};
			tmCalcParams.GetRecordCell(dwIndex, L"ParamName", tcLoad, 32);
			strLoad = tcLoad;
			CString strTmp = strLoad.Right(strLoad.GetLength() - strParamName.GetLength());
			dwLoad = _ttol(strTmp);
			if (lCraneLoad < (LONG)dwLoad)
			{
				TCHAR tcClearance[32] = {_T("\0")};
				TCHAR tcMinSize[32] = {_T("\0")};
				tmCalcParams.GetRecordCell(dwIndex, L"Param1", tcClearance, 32);
				tmCalcParams.GetRecordCell(dwIndex, L"Param2", tcMinSize, 32);
				iMinSize = _ttol(tcMinSize);
				iClearance = _ttol(tcClearance);
				break;
			}
		}
		tmCalcParams.CloseTable();
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetCalcParamFormDB()
{	
	BOOL bResult = TRUE;
	CADOTable tbCalcParams;
	CStringW strOpenOperation;
	strOpenOperation.Format(L"SELECT * FROM %s", g_tcTableName[eTableID_CalcParams]);
	bResult = tbCalcParams.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strOpenOperation);
	if (FALSE == bResult)
		return FALSE;

	int iIndex = -1;
	TCHAR tcValue[256] = {0};
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_c2"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param1", tcValue, 256);
		m_CalcDatalst.P03 = _ttol(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_dh"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param1", tcValue, 256);
		m_CalcDatalst.P04 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_v4"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P05 = _tstol(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_fmax"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P08 = tcValue;
		m_CalcDatalst.P081 = 1.0f / GetRightValueOfRangeString(m_CalcDatalst.P08);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_G2"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P02 = _ttol(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_h"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P11 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Kbz"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P18 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Kpx"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P16 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Kpz"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P17 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_ld"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P21 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_md"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P20 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_n"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P14 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_r"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P19 = _tstof(tcValue);
	}
/*	//
	CString strParam_R;
	strParam_R.Format(_T("Calc_MG_R%d"), m_CalcDatalst.MB31);
	iIndex = GetParamIndexInTable(tbCalcParams, strParam_R);
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param1", tcValue, 256);
		m_fParam_R = _tstof(tcValue);
	}*/
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_t6"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P06 = _ttol(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_t7"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P07 = _ttol(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_v1"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P12 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_v2"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P13 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_v3"));
	if (iIndex >= 0)
	{
		tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
		m_CalcDatalst.P15 = _tstof(tcValue);
	}
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_fmax2"));
	tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
	m_CalcDatalst.P09 = tcValue;
	m_CalcDatalst.P091 = 1.0f / GetRightValueOfRangeString(m_CalcDatalst.P09);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_RMB"));
	tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
	m_CalcDatalst.P10 = _tstol(tcValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_v5"));
	tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
	m_CalcDatalst.P22 = _tstof(tcValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Wd"));
	tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
	m_CalcDatalst.P23 = _tstol(tcValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_PL"));
	tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
	m_CalcDatalst.P24 = _tstol(tcValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Euro1"));
	tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
	m_CalcDatalst.P25 = _tstof(tcValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Euro2"));
	tbCalcParams.GetRecordCell(iIndex, L"Param2", tcValue, 256);
	m_CalcDatalst.P26 = _tstof(tcValue);

	tbCalcParams.CloseTable();
	return TRUE;
}
//-----------------------------------------------------------------------
int CBMParamCalculator::GetParamIndexInTable(CADOTable &tb, CString strCellName)
{
	int iIndex = -1;
	DWORD dwRecordCnt = tb.GetRecordCnt();
	for (DWORD dwCurIndex = 0; dwCurIndex < dwRecordCnt; dwCurIndex++)
	{
		CString strParamCode;
		tb.GetRecordCell(dwCurIndex, L"ParamName", strParamCode.GetBuffer(64), 64);
		if (strCellName == strParamCode)
		{
			iIndex = (int)dwCurIndex;
			break;
		}
	}

	return iIndex;
}
//-----------------------------------------------------------------------
int CBMParamCalculator::GetParamHRightValue(int iInputH)
{
	const int iStdHValues[] = {400, 500, 600, 700, 800, 900, 1000, 1100,1300, 1500, 1700, 1900, 2200, 2500};
	
	int iRightH = iInputH;
	int iCnt = sizeof(iStdHValues) / sizeof(iStdHValues[0]);
	for (int i = 1; i < iCnt; i++)
	{
		if (iInputH < iStdHValues[i])
		{
			if (iInputH <= (iStdHValues[i-1] + (iStdHValues[i] - iStdHValues[i-1])/2))
				iRightH = iStdHValues[i-1];
			else
				iRightH = iStdHValues[i];
			break;
		}
		else if (iInputH == iStdHValues[i])
		{
			iRightH = iInputH;
			break;
		}
	}
	return iRightH;
}
//-----------------------------------------------------------------------
CString CBMParamCalculator::GetParamHItemName(int iInputT3)
{
	CString strHItemName;
	if (6 == iInputT3)
		strHItemName = _T("Param1");
	else if (8 == iInputT3)
		strHItemName = _T("Param2");
	else if (10 == iInputT3)
		strHItemName = _T("Param3");
	else if (12 == iInputT3)
		strHItemName = _T("Param4");

	return strHItemName;
}
//-----------------------------------------------------------------------
CString	CBMParamCalculator::GetParamHTextItemName(int iInputT3)
{
	CString strHTextItemName;
	if (6 == iInputT3)
		strHTextItemName = _T("Param5");
	else if (8 == iInputT3)
		strHTextItemName = _T("Param6");
	else if (10 == iInputT3)
		strHTextItemName = _T("Param7");
	else if (12 == iInputT3)
		strHTextItemName = _T("Param8");

	return strHTextItemName;
}
//-----------------------------------------------------------------------
int	CBMParamCalculator::GetFrequencyByVoltage(int iVoltage)
{
	int iFrequency = 0;
	switch(iVoltage)
	{
	case 380:
		iFrequency = 50;
		break;
	case 400:
		iFrequency = 50;
		break;
	case 415:
		iFrequency = 50;
		break;
	case 460:
		iFrequency = 60;
		break;
	case 500:
		iFrequency = 60;
		break;
	default:
		iFrequency = 50;
		break;
	}

	return iFrequency;
}
//-----------------------------------------------------------------------
CString	CBMParamCalculator::GetHoistElectricProvision()
{
	CString strResult = _T("CRANE");

	for (list<CString>::iterator it = m_lstOtherHoistAdditions.begin();
		it != m_lstOtherHoistAdditions.end();
		it++)
	{
		CString strAddName = *it;
		if (strAddName == _T("hoist_add_c01"))
		{
			strResult = _T("CRANE + T");
		}
		else if (strAddName == _T("hoist_add_c02"))
		{
			strResult = _T("CRANE + T + P");
		}
	}

	return strResult;
}
//-----------------------------------------------------------------------
CString	CBMParamCalculator::GetHoistElectricalCubiclesIPClass()
{
	CString strResult = _T("IP54");
	for (list<CString>::iterator it = m_lstOtherHoistAdditions.begin();
		it != m_lstOtherHoistAdditions.end();
		it++)
	{
		CString strAddName = *it;
		if (strAddName == _T("hoist_add_c03"))
		{
			strResult = _T("IP65");
			break;
		}
	}

	return strResult;
}
//-----------------------------------------------------------------------
CString	CBMParamCalculator::GetAdditionsNameString(list<CString> lstAdditions)
{
	TCHAR tcNameString[256] = {0};

	int iIndex = 0;
	for (list<CString>::iterator it = lstAdditions.begin();
		it != lstAdditions.end();
		it++, iIndex++)
	{
		CString strName = *it;

		if (iIndex > 0) _tcscat(tcNameString, _T("/"));
		_tcscat(tcNameString, strName);
	}

	return CString(tcNameString);
}
//-----------------------------------------------------------------------
CString	CBMParamCalculator::GetHoistAdditionsNameString()
{
	TCHAR tcNameString[256] = {0};

	CString strStdAdditions = GetAdditionsNameString(m_lstStdHoistAdditions);
	CString strSelAdditions = GetAdditionsNameString(m_lstOtherHoistAdditions);

	if (!strStdAdditions.IsEmpty()) 
		_tcscat(tcNameString, strStdAdditions);

	if (!strStdAdditions.IsEmpty() && !strSelAdditions.IsEmpty())
		_tcscat(tcNameString, _T("/"));

	if (!strSelAdditions.IsEmpty()) 
		_tcscat(tcNameString, strSelAdditions);

	return CString(tcNameString);
}
//-----------------------------------------------------------------------
void CBMParamCalculator::FillAdditionsList(CString strNameStrings, list<CString> &lstAdditions)
{
	lstAdditions.clear();

	CString strCurNames = strNameStrings;

	int iPos = 0;	
	do
	{
		CString strName;
		iPos = strCurNames.Find(_T("/"));
		if (iPos != -1)
		{
			strName = strCurNames.Left(iPos);
			strCurNames = strCurNames.Right(strCurNames.GetLength() - (iPos+1));
		}
		else
		{
			strName = strCurNames;
		}
		lstAdditions.push_back(strName);
	}while(iPos != -1);
}
//-----------------------------------------------------------------------
void CBMParamCalculator::FillHoistAdditionsList(CString strNameStrings)
{
	CString strCurNames = strNameStrings;

	int iPos = 0;	
	do
	{
		CString strName;
		iPos = strCurNames.Find(_T("/"));
		if (iPos != -1)
		{
			strName = strCurNames.Left(iPos);
			strCurNames = strCurNames.Right(strCurNames.GetLength() - (iPos+1));
		}
		else
		{
			strName = strCurNames;
		}

		if (strName.Find(STR_HOIST_STDADD) != -1)
			m_lstStdHoistAdditions.push_back(strName);
		else
			m_lstOtherHoistAdditions.push_back(strName);
	}while(iPos != -1);
}
//-----------------------------------------------------------------------