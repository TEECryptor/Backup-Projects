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
	m_eNumOfHoist = eHoistNumType_One;
	m_iLoadOfCrane = 10000;
	m_iLoadOfHoist = 10000;
	m_fSpan = 22.5f;
	m_fHOL = 9;
	m_strDutyOfCrane = _T("A5");
	m_strDutyOfHoist = _T("2m/M5");
	m_strRailType = _T("P22");
	m_fRailLenght = 50.0f;
	m_iRailTopToFloor = 9000;
	m_iRoofToRailTop = 2000;
	m_strTrolleySpeed = _T("20/5");
	m_strBridgeSpeed = _T("32/8");
	m_eHoistCtrlType = eElecCtrlType_2Speeds;
	m_eTrolleyCtrlType = eElecCtrlType_Inverter;
	m_eBridgeCtrlType = eElecCtrlType_Inverter;
	//
	m_strHoistCode = _T("");
	//
	m_iGirder_c1 = 0;
	m_iGirder_h2 = 0;
	m_fGirder_mt = 0;
	//
	m_iWheBase = 0;
	m_bRailTypeIsOK = FALSE;
	m_iMainGirderPrice = 0;
	m_iEndCarPrice = 0;
	m_iELAdditionsPrice = 0;
	m_iCranePannelPrice = 0;
	m_iFastoonPrice = 0;
	m_iOperterUnitPrice = 0;
	m_iPowerSupplyPrice = 0;
	//
	m_strHoistElecProvision = _T("CRANE");
	m_strHoistElecCubIPClass = _T("IP54");
}
//-----------------------------------------------------------------------
CBMParamCalculator::~CBMParamCalculator(void)
{
}
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
void CBMParamCalculator::UpdateSelectCodes()
{
	if (m_strHoistCode.GetLength() != 14)
		return;

	m_strHoiMacIndCode.Format(_T("%c%c%c%c%c%c%c"), m_strHoistCode.GetAt(0), m_strHoistCode.GetAt(1), m_strHoistCode.GetAt(2), m_strHoistCode.GetAt(8), m_strHoistCode.GetAt(9), m_strHoistCode.GetAt(10), m_strHoistCode.GetAt(11));
	m_strDruAssCode.Format(_T("%c%c%c%c%c%c"), m_strHoistCode.GetAt(0), m_strHoistCode.GetAt(1), m_strHoistCode.GetAt(2), m_strHoistCode.GetAt(3), m_strHoistCode.GetAt(4), m_strHoistCode.GetAt(13));
	m_strHoiDimIndCode.Format(_T("%c%c%c%c%c%c%c"), m_strHoistCode.GetAt(0), m_strHoistCode.GetAt(1), m_strHoistCode.GetAt(2), m_strHoistCode.GetAt(3), m_strHoistCode.GetAt(4), m_strHoistCode.GetAt(12), m_strHoistCode.GetAt(13));
	m_strHokAssIndCode.Format(_T("%c%c%c%c%c"), m_strHoistCode.GetAt(0), m_strHoistCode.GetAt(1), m_strHoistCode.GetAt(2), m_strHoistCode.GetAt(3), m_strHoistCode.GetAt(4));
	m_strHoiPanIndCode.Format(_T("%c%c%c%c%c%c%c%c"), m_strHoistCode.GetAt(0), m_strHoistCode.GetAt(1), m_strHoistCode.GetAt(2), m_strHoistCode.GetAt(3), m_strHoistCode.GetAt(4), m_strHoistCode.GetAt(13), m_strHoistCode.GetAt(8), m_strHoistCode.GetAt(9));
	m_strTroFraCode.Format(_T("%c%c%c"), m_strHoistCode.GetAt(0), m_strHoistCode.GetAt(1), m_strHoistCode.GetAt(2));
	m_strHoiDrawCode.Format(_T("%c%c%c%c%c%c"), m_strHoistCode.GetAt(0), m_strHoistCode.GetAt(1), m_strHoistCode.GetAt(2), m_strHoistCode.GetAt(3), m_strHoistCode.GetAt(4), m_strHoistCode.GetAt(12));
}
//-----------------------------------------------------------------------
int CBMParamCalculator::GetGirder_c1()
{
	if (0 != m_iGirder_c1)
		return m_iGirder_c1;

	if (eCraneType_GTD == m_eCraneType)
	{
		m_iGirder_c1 = m_iHoist_b1 + 300;
	}
	else
	{
		m_iGirder_c1 =m_iHoist_H2 + 400;
	}

	return m_iGirder_c1;
}
//-----------------------------------------------------------------------
int CBMParamCalculator::GetGirder_h2(BOOL bForce)
{
	if (0 != m_iGirder_h2 && !bForce)
		return m_iGirder_h2;

	if (eCraneType_GTD == m_eCraneType)
	{
		m_iGirder_h2 = m_iGirder_h1 * 0.6f;
	}
	else
	{
		m_iGirder_h2 = m_iGirder_h1;
	}	

	return m_iGirder_h2;
}
//-----------------------------------------------------------------------
void CBMParamCalculator::UpdateMainGirderData(CString strMainGirderData)
{
	int iPos1 = strMainGirderData.Find(_T("/"));
	int iPos2 = strMainGirderData.Find(_T('/'), iPos1 + 1);

	CString strTwoValue = strMainGirderData.Left(iPos2);
	CString strOtherValue = strMainGirderData.Right(strMainGirderData.GetLength() - (iPos2+1));
	m_iGirder_w1 = GetLeftValueOfRangeString(strTwoValue);
	m_iGirder_h1 = GetRightValueOfRangeString(strTwoValue);
	//
	iPos1 = strOtherValue.Find(_T("/"));
	iPos2 = strOtherValue.Find(_T('/'), iPos1 + 1);
	strTwoValue = strOtherValue.Left(iPos2);
	strOtherValue = strOtherValue.Right(strOtherValue.GetLength() - (iPos2+1));
	m_iGirder_t1 = GetLeftValueOfRangeString(strTwoValue);
	m_iGirder_t2 = GetRightValueOfRangeString(strTwoValue);
	//
	strTwoValue = strOtherValue;
	m_iGirder_t3 = GetLeftValueOfRangeString(strTwoValue);
	m_iGirder_t4 = GetRightValueOfRangeString(strTwoValue);
	//
	m_iGirder_t5 = m_iGirder_t3;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::CalcMainGirder()
{
	if (FALSE == GetCalcParamFormDB())
		return FALSE;
	
	float fMainGirder_Delta0 = 0.0f;

	m_fGirder_w2 = m_iGirder_w1 - 100;

	// Girder area (mm x2)
	m_fGirder_A = m_iGirder_w1 * (m_iGirder_t2 + m_iGirder_t4) + m_iGirder_h1 * (m_iGirder_t1 + m_iGirder_t3);

	m_fGirder_x = (m_iGirder_w1 * m_iGirder_t4 * (m_iGirder_t2 + m_iGirder_h1 + m_iGirder_t4/2.0f) + m_iGirder_h1 * m_iGirder_t1 *
				  (m_iGirder_t2 + m_iGirder_h1/2.0f) + m_iGirder_h1 * m_iGirder_t3 * (m_iGirder_t2 + m_iGirder_h1/2.0f) + 
				  m_iGirder_w1 * m_iGirder_t2 * m_iGirder_t2/2.0f) / m_fGirder_A;

	m_fGirder_y = ((m_iGirder_t2 + m_iGirder_t4) * m_iGirder_w1 * m_iGirder_w1 / 2 + m_iGirder_h1 * m_iGirder_t1 * (50 + m_iGirder_t1 / 2) +
				  m_iGirder_h1 * m_iGirder_t3 * (m_iGirder_w1 - 50 - m_iGirder_t3 / 2)) / m_fGirder_A;
		
	m_fGirder_Ix1 = m_iGirder_w1 * powf(m_iGirder_t4, 3) / 12;
	m_fGirder_Ix3 = m_iGirder_w1 * powf(m_iGirder_t2, 3) / 12;
	m_fGirder_Ix2 = m_iGirder_t1 * powf(m_iGirder_h1, 3) / 12;
	m_fGirder_Ix4 = m_iGirder_t3 * powf(m_iGirder_h1, 3) / 12;

	m_fGirder_Iy1 = m_iGirder_t4 * powf(m_iGirder_w1, 3) / 12;
	m_fGirder_Iy3 = m_iGirder_t2 * powf(m_iGirder_w1, 3) / 12;
	m_fGirder_Iy2 = m_iGirder_h1 * powf(m_iGirder_t1, 3) / 12;
	m_fGirder_Iy4 = m_iGirder_h1 * powf(m_iGirder_t3, 3) / 12;

	m_fGirder_Ix = m_fGirder_Ix1 + m_fGirder_Ix2 + m_fGirder_Ix3 + m_fGirder_Ix4 +
			       (m_iGirder_w1 * m_iGirder_t4 * powf((m_iGirder_t2 + m_iGirder_h1 + m_iGirder_t4/2.0f - m_fGirder_x), 2)) +
				   (m_iGirder_w1 * m_iGirder_t2 * powf((m_iGirder_t2/2.0f - m_fGirder_x), 2)) +				   
			       (m_iGirder_t1 * m_iGirder_h1 * powf((m_iGirder_t2 + m_iGirder_h1/2.0f - m_fGirder_x), 2)) +
			       (m_iGirder_t3 * m_iGirder_h1 * powf((m_iGirder_t2 + m_iGirder_h1/2.0f - m_fGirder_x), 2));

	m_fGirder_Iy = m_fGirder_Iy1 + m_fGirder_Iy2 + m_fGirder_Iy3 + m_fGirder_Iy4 + 
				   (m_iGirder_t2 + m_iGirder_t4) * m_iGirder_w1 * powf((m_iGirder_w1/2 - m_fGirder_y), 2) +
				   m_iGirder_h1 * m_iGirder_t1 * powf((50 + m_iGirder_t1/2 - m_fGirder_y), 2) +
				   m_iGirder_h1 * m_iGirder_t3 * powf((m_iGirder_w1 - 50 - m_iGirder_t3/2 - m_fGirder_y), 2);

	m_fGirder_Wx = m_fGirder_Ix / m_fGirder_x;
	m_fGirder_Wy = m_fGirder_Iy / m_fGirder_y;

	m_fGirder_mx = (m_fGirder_A * m_fSpan * 7860) / 1000000.0f;

	m_fGirder_js2 = int((m_fSpan / 2.0f + 1) + 0.5f);

	m_fGirder_A1 = (m_fGirder_w2 - m_iGirder_t3 - m_iGirder_t1) * (m_iGirder_h1 - 20);

	m_fGirder_mj2 = (m_fGirder_A1 / 1000000.0f) * 7860 *m_fGirder_js2 * m_lParam_t7 * 0.001f;
	m_fGirder_mj1 = 2 * m_fParam_F * (m_fSpan - 1) * m_fParam_H;
	
	if (eCraneType_GTD == m_eCraneType)
	{
		m_fGirder_mr = m_fParam_R * m_fSpan * 1.0f;
		m_fGirder_mz = (m_fSpan - 1) * 10 + (m_iGirder_w3 + 200) * 0.001f * (m_fSpan - 1) * 0.004f * 7860 + 
				   (m_iGirder_w3 + m_iGirder_w1) * 0.001f * 0.2f * m_lParam_t6 * m_fGirder_js2 * 0.001f * 7860;
	}
	else
	{
		m_fGirder_mr = m_fParam_R * m_fSpan * 0.0f;
		m_fGirder_mz = 0;
	}

	m_fGirder_ms = m_fGirder_mx + m_fGirder_mj2 + m_fGirder_mj1 + m_fGirder_mr + m_fGirder_mz;

	if (eCraneType_GTD == m_eCraneType)
		m_fGirder_mt = (m_fGirder_mx + m_fGirder_mj2 + m_fGirder_mj1 + m_fGirder_mr) * 2 + m_fGirder_mz;
	else
		m_fGirder_mt = (m_fGirder_mx + m_fGirder_mj2 + m_fGirder_mj1 + m_fGirder_mr) * 1 + m_fGirder_mz;

	m_fGirder_Fq = m_fGirder_ms * 9.81f / m_fSpan;

	m_fGirder_Ep = (m_iLoadOfHoist + m_iHoist_G1) * 9.81f;
	if (eCraneType_GTD == m_eCraneType) m_fGirder_Ep /= 2.0f;

	m_fGirder_P1 = m_fGirder_Ep * m_iHoist_b2 / m_iHoist_b;
	if (eCraneType_GTS == m_eCraneType) m_fGirder_P1 /= 2.0f;

	m_fGirder_P2 = m_fGirder_Ep * m_iHoist_b1 / m_iHoist_b;
	if (eCraneType_GTS == m_eCraneType) m_fGirder_P2 /= 2.0f;

	m_fGirder_FQ1 = 1.1f;
	m_fGirder_FQ2 = m_fParam_v2 + m_fParam_v1 * m_fHoistSpeed / 60.0f;
	m_fGirder_FQ4 = 1.1f + 0.058f * m_fParam_v2 * m_iHoist_V3 / 60.0f * sqrt(m_fParam_h);

	if (eCraneCtrlType_Cabin != m_eCraneControl) m_iParam_G2 = 0;
	m_fGirder_Mx = m_fGirder_FQ4 * (m_fGirder_Fq * powf(m_fSpan, 2) / 8 + m_fGirder_Ep * powf(m_fSpan-m_iHoist_b1/1000.0f, 2) / (4 * m_fSpan) + (m_iParam_G2/2)* 9.81f * (m_fSpan - m_iParam_c2/1000.0f));

	m_fGirder_Fp = 0.5f * m_fGirder_FQ4 * m_fGirder_Ep * (1 - m_iHoist_b1/1000.0f/m_fSpan);

	m_fGirder_Fj1 = m_fGirder_FQ4 * (m_fGirder_Fq * m_fSpan / 2 + m_fGirder_Ep * (m_fSpan-m_iGirder_c1/1000.0f) / m_fSpan + m_iParam_G2 * 9.81f * (m_fSpan - m_iParam_c2/1000.0f)/m_fSpan);

	m_fGirder_Pmax = m_fGirder_FQ4 * m_fGirder_P1;

	m_fGirder_Fh = m_fGirder_Fq / 14.0f;

	m_fGirder_Ph = m_fGirder_Ep / 14.0f;

	m_fGirder_Mh = (m_fGirder_Ph * m_fSpan * (1 - 1/(2*m_fParam_r))) / 4.0f + (m_fGirder_Fh * m_fSpan * m_fSpan *(1 - 2/(3*m_fParam_r))) / 8.0f;

	m_fGirder_Fj2 = 0.5f * m_fGirder_Fh * m_fSpan + m_fGirder_Ph * (1 -m_iGirder_c1/1000.0f/m_fSpan);

	int iParam = 2;
	if (eCraneType_GTD == m_eCraneType) iParam = 2;
	else  iParam = 1;
	m_fGirder_Pr1 = 0.5f * (m_fGirder_Fq * m_fSpan + m_fGirder_Ep) * iParam + m_fGirder_mt * m_fParam_md * 9.81f + m_iParam_G2*9.81f*(m_fSpan - m_iParam_c2/1000.0f)/m_fSpan;

	m_fGirder_Ps1 = 0.5f * m_fGirder_Pr1 * (0.32f - m_fParam_ld);

	m_fGirder_Ms = 0.5f * m_fGirder_Ps1 * m_fSpan * m_fParam_ld;

	m_fGirder_My = m_fGirder_Mh + m_fGirder_Ms;

	m_fGirder_Q0x = m_fGirder_Mx * 1000 / m_fGirder_Wx;
	m_fGirder_Q0y = m_fGirder_My * 1000 / m_fGirder_Wy;
	m_fGirder_Q0 = m_fGirder_Q0x + m_fGirder_Q0y;

	/*	GTS crane calculation */
	m_fGirder_Qp = m_fGirder_My * (m_fGirder_y - 8) * 1000 / m_fGirder_Iy;	
	m_fGirder_Qpx = m_fParam_Kpx * m_fGirder_Pmax / (m_iGirder_t2 * m_iGirder_t2);
	m_fGirder_Qpz = m_fParam_Kpz * m_fGirder_Pmax / (m_iGirder_t2 * m_iGirder_t2);
	m_fGirder_Qbz = m_fParam_Kbz * m_fGirder_Pmax / (m_iGirder_t2 * m_iGirder_t2);

	m_fGirder_Q1 = m_fGirder_Q0x + m_fGirder_Q0y + m_fGirder_Qbz;
	m_fGirder_Q2 = sqrt(powf((m_fGirder_Q0x + m_fGirder_Qp + m_fGirder_Qpz) , 2) + m_fGirder_Qpx*m_fGirder_Qpx - (m_fGirder_Q0x + m_fGirder_Qp + m_fGirder_Qpz)*m_fGirder_Qpx);

	/*	GTD crane calculation */
	m_fGirder_sy = m_iGirder_t4 * m_iGirder_w1 * (m_fGirder_x - 0.5f*m_iGirder_t4);

	m_fGirder_T1 = m_fGirder_Fp * m_fGirder_sy / (m_fGirder_Ix * (m_iGirder_t1 + m_iGirder_t3));

	m_fGirder_Qm = m_fGirder_Pmax / ((2*(m_iGirder_t4 + m_iGirder_Rh) + 50) * m_iGirder_t3);

	m_fGirder_Q3 = sqrt(m_fGirder_Q0*m_fGirder_Q0 + m_fGirder_Qm*m_fGirder_Qm - m_fGirder_Q0*m_fGirder_Qm - 3*m_iGirder_t1*m_iGirder_t1);
	m_fGirder_Q4 = m_fGirder_Q0x + m_fGirder_Q0y;

	/*	Common calculation */
	m_fGirder_T0 = 1.5f * m_fGirder_Fj1 / ((m_iGirder_t1 + m_iGirder_t5) * m_iGirder_h2);

	m_fGirder_f = m_fGirder_Ep * powf(m_fSpan * 1000, 3) / (48 * 2.1f * 100000 * m_fGirder_Ix);

	m_fGirder_s = m_fGirder_Ph * powf(m_fSpan * 1000, 3) / (48 * 2.1f * 100000 * m_fGirder_Iy) * ((1-3/(4.0f*m_fParam_r)) + 
			      5*m_fGirder_Fh*m_fSpan / (8*m_fGirder_Ph) * (1 - 4/(5.0f*m_fParam_r)));

	m_fGirder_hw2 = (m_iGirder_h1 + m_iGirder_t2 + m_iGirder_t4) / (m_iGirder_w1 - 100); 

	/*	Strandard Calculationg*/
	m_fGirder_StdQ = m_iGirder_m / m_fParam_n;
	m_fGirder_StdT0 = m_fGirder_StdQ / 1.732f;
	m_fGirder_Stdfmax = m_fSpan * 1000 * m_fParam_fmax;
	m_fGirder_Stdsmax = (m_fSpan  * 1000) / 2000.0f;

	return TRUE;
}
//-----------------------------------------------------------------------
int CBMParamCalculator::GetWheelBase(BOOL bForce)
{
	if (0 != m_iWheBase && !bForce)
		return m_iWheBase;
	
	CString strValue;
	strValue.Format(_T("%c%c"), m_strEndCraCode.GetAt(3), m_strEndCraCode.GetAt(4));
	if (m_strECType == _T("S") || m_strECType == _T("D"))
	{
		m_iWheBase = _ttol(strValue) * 100;
	}
	else if (m_strECType == _T("B"))
	{
		m_iWheBase = _ttol(strValue) * 100 + 100 + _ttol(m_strRaiGauge);
	}

	return m_iWheBase;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::CalcEndCar()
{
	m_iCraneTotalWgt = m_fGirder_mt +  2*m_iEndCar_Wgt + m_iHoist_G1;

	m_iEndCarWheelCnt = 2;
	if (m_strECType == _T("S") || m_strECType == _T("D"))
		m_iEndCarWheelCnt = 2;
	else if (m_strECType == _T("B"))
		m_iEndCarWheelCnt = 4;
	m_fMaxWheLoad = 9.8 * ((m_fGirder_mt + 2*m_iEndCar_Wgt) / m_iEndCarWheelCnt / 2 + (m_iLoadOfCrane + m_iHoist_G1) * (m_fSpan - m_iGirder_c1/1000) / m_fSpan / m_iEndCarWheelCnt) / 1000;

	if (m_strECType == _T("S") || m_strECType == _T("B"))
	{
		CString strValue;
		strValue.Format(_T("%c%c"), m_strEndCraCode.GetAt(3), m_strEndCraCode.GetAt(4));
		m_fMaxStress = m_fMaxWheLoad *  _ttol(strValue) * 100 / 2 / m_iEndCar_EcW;
		if (m_strECType == _T("S") )
		{
			m_fMaxBlend = m_fMaxWheLoad * powf(GetWheelBase(), 3) / (24 * 2.06 * 1000000 * m_iEncCar_EcIx);
		}
		else
		{
			m_fMaxBlend = m_fMaxWheLoad * powf(_ttol(strValue)*100, 3) / (24 * 2.06 * 1000000 * m_iEncCar_EcIx);
		}
	}
	else
	{ 
		int iRaiGauge = _ttol(m_strRaiGauge);
		m_fMaxStress = (m_fMaxWheLoad * (GetWheelBase() - iRaiGauge) - m_fMaxWheLoad * iRaiGauge / 4) / m_iEndCar_EcW;
		m_fMaxBlend = m_fMaxWheLoad * (powf(GetWheelBase(), 3) - powf(iRaiGauge, 2)/2 * (3*GetWheelBase() - iRaiGauge)) / (24 * 2.06 * 1000000 * m_iEncCar_EcIx);
	}

	/*	Check Rail is suitable or not */	
	float fPar1;
	float fPar2;
	CString strSQL;
	CString strWheCode;
	strWheCode.Format(_T("%c%c%d"), m_strEndCraCode.GetAt(1), m_strEndCraCode.GetAt(2), (int)GetLeftValueOfRangeString(m_strBridgeSpeed));
	strSQL.Format(_T("SELECT * FROM %s WHERE WheCode='%s'"), g_tcTableName[eTableID_WheLoaCal], strWheCode);
	fPar1 = GetFLOATValueFromTable(strSQL, _T("Par1"));
	fPar2 = GetFLOATValueFromTable(strSQL, _T("Par2"));
	//
	float fRaiWidEff;
	float fWheLoaCalEff;
	strSQL.Format(_T("SELECT * FROM %s WHERE RaiTyp='%s'"), g_tcTableName[eTableID_DatRai], m_strRailType);
	fRaiWidEff = GetFLOATValueFromTable(strSQL, _T("RaiWidEff"));
	fWheLoaCalEff = GetFLOATValueFromTable(strSQL, _T("WheLoaCalEff"));
	//
	int iTotalWheelCnt = 2 * m_iEndCarWheelCnt;
	float fWheLoaAve = (fPar1 * fRaiWidEff * fWheLoaCalEff) / 1000.0f;
	float fWheLoaMax1 = (fPar2 * fRaiWidEff * fWheLoaCalEff) / 1000.0f;
	float fWheLoaMin1 = (m_fGirder_mt + 2*m_iEndCar_Wgt) * 9.8 / iTotalWheelCnt / 1000.0f;
	float fWheLoaMin2 = (m_fMaxWheLoad*2 + fWheLoaMin1) / 3.0f;
	m_bRailTypeIsOK = (fWheLoaMax1 >= m_fMaxWheLoad) && (fWheLoaAve >= fWheLoaMin2);

	return TRUE;
}
//-----------------------------------------------------------------------
CString	CBMParamCalculator::GetEndCarDataText(int iWheDim)
{
	if (iWheDim == 0)
	{
		CString strWheDim;		
		strWheDim.Format(_T("%c%c"), m_strEndCraCode.GetAt(1), m_strEndCraCode.GetAt(2));
		iWheDim = _ttol(strWheDim);
	}

	CString strTmp;
	CString strECText;
	strECText = m_strEndCraCode;

	// 轮槽宽度
	strTmp.Format(_T("/%d"), m_iRaiWidTop+20);
	strECText += strTmp;

	if (m_iNumOfMotor == 2)
		strECText += _T("-");
	else
		strECText += _T("=");

	// 轨距
	if (eCraneType_GTS == m_eCraneType)
		strECText += _T("00");
	else
	{
		strTmp.Format(_T("%d"), _ttol(m_strRaiGauge)/100);
		strECText += strTmp;
	}
	strECText += GetJoiPlaTypeString(iWheDim);
	strECText += _T("/");
	strECText += m_strGearCode;
	int iBufExt = GetBufferExtend();
	if (iBufExt > 100) iBufExt = int(iBufExt/100);
	else iBufExt = 0;
	strTmp.Format(_T("/%d"), iBufExt);
	strECText += strTmp;
	strECText += GetBufferType();
	
	return strECText;
}
//-----------------------------------------------------------------------
CString CBMParamCalculator::GetJoiPlaTypeString(int iWheDim)
{
	if (iWheDim == 0)
	{
		CString strWheDim;		
		strWheDim.Format(_T("%c%c"), m_strEndCraCode.GetAt(1), m_strEndCraCode.GetAt(2));
		iWheDim = _ttol(strWheDim);
	}

	CString strJoiPlaType;

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE WheDim=%d"), g_tcTableName[eTableID_JoiPlaTyp], iWheDim);
	CString strCode = GetStringValueFromTable(strSQL, _T("JoiPlaCod"));
	//
	switch(m_iGirder_w1)
	{
	case 300:
		strJoiPlaType.Format(_T("%s%d"), strCode, 3);
		break;
	case 410:
		strJoiPlaType.Format(_T("%s%d"), strCode, 4);
		break;
	case 490:
		strJoiPlaType.Format(_T("%s%d"), strCode, 5);
		break;
	case 610:
		strJoiPlaType.Format(_T("%s%d"), strCode, 6);
		break;
	case 740:
		strJoiPlaType.Format(_T("%s%d"), strCode, 7);
		break;
	case 860:
		strJoiPlaType.Format(_T("%s%d"), strCode, 8);
		break;
	}

	return strJoiPlaType;
}
//-----------------------------------------------------------------------
CString	CBMParamCalculator::GetBufferType()
{
	CString strBufTyp;
	CString strCodBufSel;
	strCodBufSel = m_strEndCraCode.Left(3);

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE CodBufSel='%s'"), g_tcTableName[eTableID_Buffer], strCodBufSel);	
	if (GetBufferExtend() > 100)
		strBufTyp = GetStringValueFromTable(strSQL, _T("BufTypExt"));
	else
		strBufTyp = GetStringValueFromTable(strSQL, _T("BufTyp"));

	return strBufTyp;
}
//-----------------------------------------------------------------------
CString CBMParamCalculator::GetGearCodeFromDriver(CString strDriver)
{
	int iPos = strDriver.Find(_T('/'));
	CString strGearCode = strDriver.Right(strDriver.GetLength() - (iPos+1));
	iPos = strGearCode.Find(_T('/'));
	strGearCode = strGearCode.Left(iPos);
	m_strGearCode = strGearCode;
	return m_strGearCode;
};
//-----------------------------------------------------------------------
float CBMParamCalculator::GetCapacityFactor()
{
	float fFactor = 0.0f;

	fFactor = (m_fGirder_mt + m_iLoadOfCrane + m_iHoist_G1 + m_iEndCar_Wgt) / (1000*m_fDriverLoad);

	return fFactor;
}
//-----------------------------------------------------------------------
float CBMParamCalculator::GetBufferExtend()
{
	float fBuffExt = 0.0f;

	if (eCraneType_GTS != m_eCraneType)
	{
		fBuffExt = m_iGirder_w3 - ((GetWheelBase() - _ttol(m_strRaiGauge)) / 2.0f - m_iGirder_w1 + 300) ;
		if (fBuffExt > 0.0f) fBuffExt += 100;
	}

	return fBuffExt;
}
//-----------------------------------------------------------------------
float CBMParamCalculator::GetMainGirder_Q0()
{
	return m_fGirder_Q0;
}
//-----------------------------------------------------------------------
float CBMParamCalculator::GetMainGirder_Q1()
{
	if (eCraneType_GTS == m_eCraneType)
	{
		return m_fGirder_Q2;
	}
	else
	{
		return m_fGirder_Q4;
	}
}
//-----------------------------------------------------------------------
float CBMParamCalculator::GetMainGirder_Q2()
{
	if (eCraneType_GTS == m_eCraneType)
	{
		return m_fGirder_Q1;
	}
	else
	{
		return m_fGirder_Q3;
	}
}
//-----------------------------------------------------------------------
float CBMParamCalculator::GetMainGirder_T()
{
	return m_fGirder_T0;
}
//-----------------------------------------------------------------------
float CBMParamCalculator::GetMainGirder_F()
{
	return m_fGirder_f;
}
//-----------------------------------------------------------------------
float CBMParamCalculator::GetMainGirder_S()
{
	return m_fGirder_s;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetCompomentTechData(TechDataList &dataList)
{	
	dataList.clear();
	dataList.push_back(TechData(STR_TECHDATA_LIST_HOIST, STR_TECHDATA_LIST_HOIST, STR_TECHDATA_LIST_HOIST, m_strHoistCode));
	dataList.push_back(TechData(STR_TECHDATA_LIST_ENDCAR, STR_TECHDATA_LIST_ENDCAR, STR_TECHDATA_LIST_ENDCAR, GetEndCarDataText()));
	dataList.push_back(TechData(STR_TECHDATA_LIST_BRIDGEMOTOR, STR_TECHDATA_LIST_BRIDGEMOTOR, STR_TECHDATA_LIST_BRIDGEMOTOR, m_strDriverCode));
	dataList.push_back(TechData(STR_TECHDATA_LIST_BRIDGEPANNEL, STR_TECHDATA_LIST_BRIDGEPANNEL, STR_TECHDATA_LIST_BRIDGEPANNEL, m_strCraPannelCode));
	dataList.push_back(TechData(STR_TECHDATA_LIST_TROLLEYPOWER, STR_TECHDATA_LIST_TROLLEYPOWER, STR_TECHDATA_LIST_TROLLEYPOWER, m_strFastoonCode));
	dataList.push_back(TechData(STR_TECHDATA_LIST_CRANEPOWER, STR_TECHDATA_LIST_CRANEPOWER, STR_TECHDATA_LIST_CRANEPOWER, m_strCraPowSupplyCode));
	dataList.push_back(TechData(STR_TECHDATA_LIST_OPERATEUNIT, STR_TECHDATA_LIST_OPERATEUNIT, STR_TECHDATA_LIST_OPERATEUNIT, m_strOperterUnitCode));

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::GetHoistTechData(TechDataList &dataList)
{
	CString strValue;	
	dataList.clear();

	dataList.push_back(TechData(STR_TECHDATA_HOIST_TYPE, STR_TECHDATA_HOIST_EN_TYPE, STR_TECHDATA_HOIST_CN_TYPE, m_strHoistCode));
	//
	strValue.Format(_T("%c%c%c"), m_strHoistCode.GetAt(0), m_strHoistCode.GetAt(1), m_strHoistCode.GetAt(2));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_FRAMESIZE, STR_TECHDATA_HOIST_EN_FRAMESIZE, STR_TECHDATA_HOIST_CN_FRAMESIZE, strValue));
	//
	strValue.Format(_T("%c%c"), m_strHoistCode.GetAt(3), m_strHoistCode.GetAt(4));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_REEVINGCODE, STR_TECHDATA_HOIST_EN_REEVINGCODE, STR_TECHDATA_HOIST_CN_REEVINGCODE, strValue));
	//
	strValue.Format(_T("%c"), m_strHoistCode.GetAt(13));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DRUMLENCODE, STR_TECHDATA_HOIST_EN_DRUMLENCODE, STR_TECHDATA_HOIST_CN_DRUMLENCODE, strValue));
	//
	strValue.Format(_T("FEM %c"), m_strHoistCode.GetAt(10));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DUTYGROUP, STR_TECHDATA_HOIST_EN_DUTYGROUP, STR_TECHDATA_HOIST_CN_DUTYGROUP, strValue));
	//
	strValue.Format(_T("%d kg"), m_iLoadOfHoist);
	dataList.push_back(TechData(STR_TECHDATA_HOIST_LOAD, STR_TECHDATA_HOIST_EN_LOAD, STR_TECHDATA_HOIST_CN_LOAD, strValue));
	//
	strValue.Format(_T("%d V"), m_iVoltage1_Main);
	dataList.push_back(TechData(STR_TECHDATA_HOIST_VOLTAGE1, STR_TECHDATA_HOIST_EN_VOLTAGE1, STR_TECHDATA_HOIST_CN_VOLTAGE1, strValue));
	//
	strValue.Format(_T("%d V"), m_iVoltage2_Ctrl);
	dataList.push_back(TechData(STR_TECHDATA_HOIST_VOLTAGE2, STR_TECHDATA_HOIST_EN_VOLTAGE2, STR_TECHDATA_HOIST_CN_VOLTAGE2, strValue));
	//
	strValue.Format(_T("%d Hz"), GetFrequencyByVoltage(m_iVoltage1_Main));  
	dataList.push_back(TechData(STR_TECHDATA_HOIST_FREQUENCY, STR_TECHDATA_HOIST_EN_FREQUENCY, STR_TECHDATA_HOIST_CN_FREQUENCY, strValue));
	//
	strValue.Format(_T("%0.2fm (Max.%0.2fm)"), m_fHOL, m_fHoistHOL);  
	dataList.push_back(TechData(STR_TECHDATA_HOIST_HEIGHTOFLIT, STR_TECHDATA_HOIST_EN_HEIGHTOFLIT, STR_TECHDATA_HOIST_CN_HEIGHTOFLIT, strValue));
	//
	strValue.Format(_T("2sp"));		//TBD
	dataList.push_back(TechData(STR_TECHDATA_HOIST_CTRLTYPE, STR_TECHDATA_HOIST_EN_CTRLTYPE, STR_TECHDATA_HOIST_CN_CTRLTYPE, strValue));
	//
	strValue.Format(_T("%c%c (%s kw)"), m_strHoistCode.GetAt(8), m_strHoistCode.GetAt(9), m_strHoist_Pow);
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORTYPE, STR_TECHDATA_HOIST_EN_MOTORTYPE, STR_TECHDATA_HOIST_CN_MOTORTYPE, strValue));
	//
	strValue.Format(_T("%0.1f m/min"), GetRightValueOfRangeString(m_strHoistingSpeed));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_SPEEDH, STR_TECHDATA_HOIST_EN_SPEEDH, STR_TECHDATA_HOIST_CN_SPEEDH, strValue));
	//
	strValue.Format(_T("%0.1f m/min"), GetLeftValueOfRangeString(m_strHoistingSpeed));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_SPEEDL, STR_TECHDATA_HOIST_EN_SPEEDL, STR_TECHDATA_HOIST_CN_SPEEDL, strValue));
	//
	strValue.Format(_T("%d m/min"), (int)GetRightValueOfRangeString(m_strTrolleySpeed));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROSPEEDH, STR_TECHDATA_HOIST_EN_TROSPEEDH, STR_TECHDATA_HOIST_CN_TROSPEEDH, strValue));
	//
	strValue.Format(_T("%d m/min"), (int)GetLeftValueOfRangeString(m_strTrolleySpeed));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROSPEEDL, STR_TECHDATA_HOIST_EN_TROSPEEDL, STR_TECHDATA_HOIST_CN_TROSPEEDL, strValue));
	//
	strValue.Format(_T("%0.2f kw/pc"), m_fHoist_DriPow);
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROMOTOR, STR_TECHDATA_HOIST_EN_TROMOTOR, STR_TECHDATA_HOIST_CN_TROMOTOR, strValue));
	//
//	strValue.Format(_T("DynADrive"));		//TBD
//	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROCTRLTYPE, STR_TECHDATA_HOIST_EN_TROCTRLTYPE, STR_TECHDATA_HOIST_CN_TROCTRLTYPE, strValue));
	//
	strValue.Format(_T("EN / PO1"));		//TBD
	dataList.push_back(TechData(STR_TECHDATA_HOIST_DATAPLATE, STR_TECHDATA_HOIST_EN_DATAPLATE, STR_TECHDATA_HOIST_CN_DATAPLATE, strValue));
	//
	strValue.Format(_T("%c"), m_strHoistCode.GetAt(12));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROLLEYTYPE, STR_TECHDATA_HOIST_EN_TROLLEYTYPE, STR_TECHDATA_HOIST_CN_TROLLEYTYPE, strValue));
	//
	if (eCraneType_GTS == m_eCraneType)
	{
		if (m_bOnlyHoist)
		{
			strValue.Format(_T("%s mm"), m_strRaiGauge);
		}
		else
		{
			strValue.Format(_T("%d mm"), m_iGirder_w1);
		}
		dataList.push_back(TechData(STR_TECHDATA_HOIST_RAILORGIRDER, STR_TECHDATA_HOIST_EN_RAILORGIRDER, STR_TECHDATA_HOIST_CN_RAILORGIRDER, strValue));
	}
	else
	{
		strValue.Format(_T("%s mm"), m_strRaiGauge);		
		dataList.push_back(TechData(STR_TECHDATA_HOIST_RAILORGIRDER, STR_TECHDATA_HOIST_EN_RAILORGIRDER, STR_TECHDATA_HOIST_CN_RAILORGIRDER, strValue));
		//
		if (m_bOnlyHoist)
		{
			m_strTroRail = _T("50x30");
		}
		strValue.Format(m_strTroRail);
		dataList.push_back(TechData(STR_TECHDATA_HOIST_RAILTYPE, STR_TECHDATA_HOIST_EN_RAILTYPE, STR_TECHDATA_HOIST_CN_RAILTYPE, strValue));
	}
	//
	strValue.Format(_T("FEM M5(2m)"));		//TBD
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TRODUTY, STR_TECHDATA_HOIST_EN_TRODUTY, STR_TECHDATA_HOIST_CN_TRODUTY, strValue));
	//
	strValue.Format(m_strTroMachineryType);
	dataList.push_back(TechData(STR_TECHDATA_HOIST_TROMACHINERY, STR_TECHDATA_HOIST_EN_TROMACHINERY, STR_TECHDATA_HOIST_CN_TROMACHINERY, strValue));
	//
	strValue.Format(_T("%d ps"), m_iTroDriNum);
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MACHINERYNUM, STR_TECHDATA_HOIST_EN_MACHINERYNUM, STR_TECHDATA_HOIST_CN_MACHINERYNUM, strValue));
	//
	strValue.Format(_T("%d kg"), m_iHoist_G1);
	dataList.push_back(TechData(STR_TECHDATA_HOIST_WEIGHT, STR_TECHDATA_HOIST_EN_WEIGHT, STR_TECHDATA_HOIST_CN_WEIGHT, strValue));
	//
	strValue.Format(_T("ELE"));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_OVERLOAD, STR_TECHDATA_HOIST_EN_OVERLOAD, STR_TECHDATA_HOIST_CN_OVERLOAD, strValue));
	//
	strValue.Format(_T("%s"), GetHoistElectricProvision());
	dataList.push_back(TechData(STR_TECHDATA_HOIST_ELECPROV, STR_TECHDATA_HOIST_EN_ELECPROV, STR_TECHDATA_HOIST_CN_ELECPROV, strValue));
	//
	strValue.Format(_T("%s"), GetHoistElectricalCubiclesIPClass());
	dataList.push_back(TechData(STR_TECHDATA_HOIST_ELECIP, STR_TECHDATA_HOIST_EN_ELECIP, STR_TECHDATA_HOIST_CN_ELECIP, strValue));
	//
	strValue.Format(_T("IP55"));
	dataList.push_back(TechData(STR_TECHDATA_HOIST_MOTORIP, STR_TECHDATA_HOIST_EN_MOTORIP, STR_TECHDATA_HOIST_CN_MOTORIP, strValue));

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
	CString strValue;	
	CString strValueCN;	
	dataList.clear();

	switch(m_eCraneType)
	{
	case eCraneType_GTS:
		strValue = _T("BMS");
		break;
	case eCraneType_GTD:
		strValue = _T("BMD");
		break;
	case eCraneType_GTF:
		strValue = _T("BMF");
		break;
	}
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TYPE, STR_TECHDATA_CRANE_EN_TYPE, STR_TECHDATA_CRANE_CN_TYPE, strValue));
	//
	strValue.Format(_T("%d kg"), m_iLoadOfCrane);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CAPACITY, STR_TECHDATA_CRANE_EN_CAPACITY, STR_TECHDATA_CRANE_CN_CAPACITY, strValue));
	//
	strValue.Format(_T("%0.1f m"), m_fSpan);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_SPAN, STR_TECHDATA_CRANE_EN_SPAN, STR_TECHDATA_CRANE_CN_SPAN, strValue));
	//
	strValue.Format(_T("%0.1f m"), m_fHOL);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_LIFTHEIGHT, STR_TECHDATA_CRANE_EN_LIFTHEIGHT, STR_TECHDATA_CRANE_CN_LIFTHEIGHT, strValue));
	//
	strValue.Format(_T("FEM %s"), m_strDutyOfCrane);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_DUTYGROUP, STR_TECHDATA_CRANE_EN_DUTYGROUP, STR_TECHDATA_CRANE_CN_DUTYGROUP, strValue));	
	//
	strValue.Format(_T("%d kg"), m_iCraneTotalWgt);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_WEIGHT, STR_TECHDATA_CRANE_EN_WEIGHT, STR_TECHDATA_CRANE_CN_WEIGHT, strValue));			
	//
	strValue.Format(_T("%d kg"), (int)m_fGirder_mt);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_STEELWEIGHT, STR_TECHDATA_CRANE_EN_STEELWEIGHT, STR_TECHDATA_CRANE_CN_STEELWEIGHT, strValue));			
	//
	strValue.Format(_T("%d kg"), m_iHoist_G1);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TROWEIGHT, STR_TECHDATA_CRANE_EN_TROWEIGHT, STR_TECHDATA_CRANE_CN_TROWEIGHT, strValue));
	//
	strValue.Format(_T("2x%d kg"), m_iEndCar_Wgt);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_ENDCARWEIGHT, STR_TECHDATA_CRANE_EN_ENDCARWEIGHT, STR_TECHDATA_CRANE_CN_ENDCARWEIGHT, strValue));
	//
	strValue.Format(_T("%d"), m_iEndCarWheelCnt);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_RAILWHEELNUM, STR_TECHDATA_CRANE_EN_RAILWHEELNUM, STR_TECHDATA_CRANE_CN_RAILWHEELNUM, strValue));
	//
	strValue.Format(_T("%d KN"), (int)m_fMaxWheLoad);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_WHEELLOAD, STR_TECHDATA_CRANE_EN_WHEELLOAD, STR_TECHDATA_CRANE_CN_WHEELLOAD, strValue));
	//
	strValue.Format(_T("%s"), m_strRailType);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_RAILTYPE, STR_TECHDATA_CRANE_EN_RAILTYPE, STR_TECHDATA_CRANE_CN_RAILTYPE, strValue));
	//
	strValue.Format(_T("%dV/%dHZ"), m_iVoltage1_Main, GetFrequencyByVoltage(m_iVoltage1_Main));
	strValueCN.Format(_T("3相 / %dV / %d HZ"), m_iVoltage1_Main, GetFrequencyByVoltage(m_iVoltage1_Main));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_MAINVOL, STR_TECHDATA_CRANE_EN_MAINVOL, STR_TECHDATA_CRANE_CN_MAINVOL, strValue, strValueCN));
	//
	strValue.Format(_T("%dV"), m_iVoltage2_Ctrl);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CTRLVOL, STR_TECHDATA_CRANE_EN_CTRLVOL, STR_TECHDATA_CRANE_CN_CTRLVOL, strValue));
	//
	/*Remove this item as BM's requesttion
	strValue.Format(_T("%s"), _T("94.2A/26.8A"));		//TBD
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CURRENT, STR_TECHDATA_CRANE_EN_CURRENT, STR_TECHDATA_CRANE_CN_CURRENT, strValue));
	*/
	//
	if (m_eMainUseType == eCraneMainUseType_Indoors)
	{
		strValue.Format(_T("%s"), _T("Indoors/-5~40ºC"));
		strValueCN.Format(_T("%s"), _T("室内/-5~40ºC"));
	}
	else
	{
		strValue.Format(_T("%s"), _T("Outdoors/-5~40ºC"));
		strValueCN.Format(_T("%s"), _T("室外/-5~40ºC"));
	}
	dataList.push_back(TechData(STR_TECHDATA_CRANE_MAINUSE, STR_TECHDATA_CRANE_EN_MAINUSE, STR_TECHDATA_CRANE_CN_MAINUSE, strValue, strValueCN));
	//
	dataList.push_back(TechData(_T(""), _T(""), _T(""), _T("")));
	//
	strValue.Format(_T("%s"), m_strHoistCode);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_HOISTTYPE, STR_TECHDATA_CRANE_EN_HOISTTYPE, STR_TECHDATA_CRANE_CN_HOISTTYPE, strValue));
	//
	strValue.Format(_T("%d kg"), m_iHoistCap);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_HOISTCAP, STR_TECHDATA_CRANE_EN_HOISTCAP, STR_TECHDATA_CRANE_CN_HOISTCAP, strValue));
	//
	strValue.Format(_T("FEM M%c"), m_strHoistCode.GetAt(10));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_HOISTDUTY, STR_TECHDATA_CRANE_EN_HOISTDUTY, STR_TECHDATA_CRANE_CN_HOISTDUTY, strValue));
	//
	strValue.Format(_T("%0.1f m (Max.%0.1f m)"), m_fHOL, m_fHoistHOL);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_HOISTLIFT, STR_TECHDATA_CRANE_EN_HOISTLIFT, STR_TECHDATA_CRANE_CN_HOISTLIFT, strValue));
	//
	strValue.Format(_T("%c"), m_strHoistCode.GetAt(12));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TROTYPE, STR_TECHDATA_CRANE_EN_TROTYPE, STR_TECHDATA_CRANE_CN_TROTYPE, strValue));
	//
	strValue.Format(_T("%c%c"), m_strHoistCode.GetAt(3), m_strHoistCode.GetAt(4));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_REEVING, STR_TECHDATA_CRANE_EN_REEVING, STR_TECHDATA_CRANE_CN_REEVING, strValue));
	//
	strValue.Format(_T("%s m/min (%s)"), m_strHoistingSpeed, (m_eHoistCtrlType==eElecCtrlType_2Speeds) ? _T("2 Speeds"): _T("Inverter"));
	strValueCN.Format(_T("%s m/min (%s)"), m_strHoistingSpeed, (m_eHoistCtrlType==eElecCtrlType_2Speeds) ? _T("双速"): _T("变频"));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_HOISTSPD, STR_TECHDATA_CRANE_EN_HOISTSPD, STR_TECHDATA_CRANE_CN_HOISTSPD, strValue, strValueCN));
	//
	strValue.Format(_T("%s m/min (%s)"), m_strTrolleySpeed, (m_eTrolleyCtrlType==eElecCtrlType_2Speeds) ? _T("2 Speeds"): _T("Inverter"));
	strValueCN.Format(_T("%s m/min (%s)"), m_strTrolleySpeed, (m_eTrolleyCtrlType==eElecCtrlType_2Speeds) ? _T("双速"): _T("变频"));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TRAVERSSPD, STR_TECHDATA_CRANE_EN_TRAVERSSPD, STR_TECHDATA_CRANE_CN_TRAVERSSPD, strValue, strValueCN));
	//
	strValue.Format(_T("%s m/min (%s)"), m_strBridgeSpeed, (m_eBridgeCtrlType==eElecCtrlType_2Speeds) ? _T("2 Speeds"): _T("Inverter"));
	strValueCN.Format(_T("%s m/min (%s)"), m_strBridgeSpeed, (m_eBridgeCtrlType==eElecCtrlType_2Speeds) ? _T("双速"): _T("变频"));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TRAVELSPD, STR_TECHDATA_CRANE_EN_TRAVELSPD, STR_TECHDATA_CRANE_CN_TRAVELSPD, strValue, strValueCN));
	// 
	strValue.Format(_T("%c%c (%s kw)"), m_strHoistCode.GetAt(8), m_strHoistCode.GetAt(9), m_strHoist_Pow);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_HOISTMOTOR, STR_TECHDATA_CRANE_EN_HOISTMOTOR, STR_TECHDATA_CRANE_CN_HOISTMOTOR, strValue));
	// 
	strValue.Format(_T("%dx%0.2f kw"), m_iTroDriNum, m_fHoist_DriPow);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TROMOTOR, STR_TECHDATA_CRANE_EN_TROMOTOR, STR_TECHDATA_CRANE_CN_TROMOTOR, strValue));
	// 
	strValue.Format(_T("%dx%0.2f kw"), m_iNumOfMotor, m_fEndCar_DriPow);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CRANEMOTOR, STR_TECHDATA_CRANE_EN_CRANEMOTOR, STR_TECHDATA_CRANE_CN_CRANEMOTOR, strValue));
	// 
	strValue.Format(_T("%0.2f kw"), GetLeftValueOfRangeString(m_strHoist_Pow) + m_iTroDriNum*m_fHoist_DriPow + m_iNumOfMotor*m_fEndCar_DriPow);
	dataList.push_back(TechData(STR_TECHDATA_CRANE_TOTALPOW, STR_TECHDATA_CRANE_EN_TOTALPOW, STR_TECHDATA_CRANE_CN_TOTALPOW, strValue));
	// 
	strValue.Format(_T("%s"), GetHoistElectricalCubiclesIPClass());
	dataList.push_back(TechData(STR_TECHDATA_CRANE_ELECIP, STR_TECHDATA_CRANE_EN_ELECIP, STR_TECHDATA_CRANE_CN_ELECIP, strValue));
	// 
	strValue.Format(_T("IP55"));
	dataList.push_back(TechData(STR_TECHDATA_CRANE_MOTORIP, STR_TECHDATA_CRANE_EN_MOTORIP, STR_TECHDATA_CRANE_CN_MOTORIP, strValue));
	// 
	switch(m_eCraneControl)
	{
	case eCraneCtrlType_Pendent:
		strValue.Format(_T("%s"), _T("Pendent"));
		strValueCN.Format(_T("%s"), _T("线控"));
		break;
	case eCraneCtrlType_Remote:
		strValue.Format(_T("%s"), _T("Remote"));
		strValueCN.Format(_T("%s"), _T("摇控"));
		break;
	case eCraneCtrlType_BlueTooth:
		strValue.Format(_T("%s"), _T("Bluetooth"));
		strValueCN.Format(_T("%s"), _T("蓝牙"));
		break;
	case eCraneCtrlType_Cabin:
		strValue.Format(_T("%s"), _T("Cabin"));
		strValueCN.Format(_T("%s"), _T("司机室"));
		break;
	}
	dataList.push_back(TechData(STR_TECHDATA_CRANE_CTRLTYPE, STR_TECHDATA_CRANE_EN_CTRLTYPE, STR_TECHDATA_CRANE_CN_CTRLTYPE, strValue, strValueCN));
	
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
BOOL CBMParamCalculator::LoadDataFromDatabase(CString strOfferID, CString strComponentName)
{
	CString strOperations;
	CADOTable tbComponent;
	strOperations.Format(_T("SELECT * FROM %s WHERE OfferNo='%s' AND ComponentName='%s'"), g_tcTableName[eTableID_ComponentInfo], strOfferID, strComponentName);
	if (!tbComponent.OpenTable(g_pOfferDatabase->GetConnectionPtr(), strOperations) || tbComponent.GetRecordCnt() == 0)
		return FALSE;

	// Component type
	TCHAR tcTemp[256] = {_T("0")};
	tbComponent.GetRecordCell(0, L"ComponentType", tcTemp, 256);
	if (_tcsicmp(tcTemp, STR_COMTYPE_HOIST) == 0) m_bOnlyHoist = TRUE;
	else m_bOnlyHoist = FALSE;
	
	/* Read all user set information*/
	tbComponent.GetRecordCell(0, L"CraneType", (DWORD&)m_eCraneType);
	tbComponent.GetRecordCell(0, L"HoistNumberType", (DWORD&)m_eNumOfHoist);
	tbComponent.GetRecordCell(0, L"CraneLoad", (DWORD&)m_iLoadOfCrane);
	tbComponent.GetRecordCell(0, L"HoistLoad", (DWORD&)m_iLoadOfHoist);
	tbComponent.GetRecordCell(0, L"Span", m_fSpan);
	tbComponent.GetRecordCell(0, L"HOL", m_fHOL);
	if (tbComponent.GetRecordCell(0, L"CraneDuty", tcTemp, 256))	m_strDutyOfCrane = tcTemp;
	if (tbComponent.GetRecordCell(0, L"HoistDuty", tcTemp, 256))	m_strDutyOfHoist = tcTemp;
	if (tbComponent.GetRecordCell(0, L"RailType", tcTemp, 256))		m_strRailType = tcTemp;
	tbComponent.GetRecordCell(0, L"RailLength", m_fRailLenght);
	tbComponent.GetRecordCell(0, L"RailTopToFloor", (DWORD&)m_iRailTopToFloor);
	tbComponent.GetRecordCell(0, L"RailRoofToTop", (DWORD&)m_iRoofToRailTop);
	tbComponent.GetRecordCell(0, L"PowerSupply", (DWORD&)m_iVoltage1_Main);
	tbComponent.GetRecordCell(0, L"ControlVol", (DWORD&)m_iVoltage2_Ctrl);
	tbComponent.GetRecordCell(0, L"CraneCtrlType", (DWORD&)m_eCraneControl);
	tbComponent.GetRecordCell(0, L"MainUseType", (DWORD&)m_eMainUseType);
	if (tbComponent.GetRecordCell(0, L"HoistSpeed", tcTemp, 256))	m_strHoistingSpeed = tcTemp;
	if (tbComponent.GetRecordCell(0, L"TrolleySpeed", tcTemp, 256))	m_strTrolleySpeed = tcTemp;
	if (tbComponent.GetRecordCell(0, L"BridgeSpeed", tcTemp, 256))	m_strBridgeSpeed = tcTemp;
	tbComponent.GetRecordCell(0, L"HoistCtrlType", (DWORD&)m_eHoistCtrlType);
	tbComponent.GetRecordCell(0, L"TrolleyCtrlType", (DWORD&)m_eTrolleyCtrlType);
	tbComponent.GetRecordCell(0, L"BridgeCtrlType", (DWORD&)m_eBridgeCtrlType);
	tbComponent.GetRecordCell(0, L"ReevingType", (DWORD&)m_eReevingType);
	//
	if (tbComponent.GetRecordCell(0, L"HoistCode", tcTemp, 256))	m_strHoistCode = tcTemp;
	tbComponent.GetRecordCell(0, L"HoistTotalPrice", (DWORD&)m_iHoistPrice);
	tbComponent.GetRecordCell(0, L"HoistAdditions", tcTemp, 256);	FillHoistAdditionsList(tcTemp);
	//
	if (!m_bOnlyHoist)
	{
		if (tbComponent.GetRecordCell(0, L"CraneCode", tcTemp, 256))		m_strCraneCode = tcTemp;
		if (tbComponent.GetRecordCell(0, L"MainGirderCode", tcTemp, 256))	m_strMainGirderCode = tcTemp;
		tbComponent.GetRecordCell(0, L"MainGirderPrice", (DWORD&)m_iMainGirderPrice);
		tbComponent.GetRecordCell(0, L"MG_W1", (DWORD&)m_iGirder_w1);
		tbComponent.GetRecordCell(0, L"MG_h1", (DWORD&)m_iGirder_h1);
		tbComponent.GetRecordCell(0, L"MG_t1", (DWORD&)m_iGirder_t1);
		tbComponent.GetRecordCell(0, L"MG_t2", (DWORD&)m_iGirder_t2);
		tbComponent.GetRecordCell(0, L"MG_t3", (DWORD&)m_iGirder_t3);
		tbComponent.GetRecordCell(0, L"MG_t4", (DWORD&)m_iGirder_t4);
		if (tbComponent.GetRecordCell(0, L"MG_TroRail", tcTemp, 256))		m_strTroRail = tcTemp;
		tbComponent.GetRecordCell(0, L"MG_Matrial", (DWORD&)m_iGirder_m);
		tbComponent.GetRecordCell(0, L"MG_TrolleyRail_W", (DWORD&)m_iGirder_Rw);
		tbComponent.GetRecordCell(0, L"MG_TrolleyRail_H", (DWORD&)m_iGirder_Rh);
		tbComponent.GetRecordCell(0, L"MG_w3", (DWORD&)m_iGirder_w3);
		tbComponent.GetRecordCell(0, L"MG_c1", (DWORD&)m_iGirder_c1);
		tbComponent.GetRecordCell(0, L"MG_t5", (DWORD&)m_iGirder_t5);
		tbComponent.GetRecordCell(0, L"MG_h2", (DWORD&)m_iGirder_h2);
		//
		if (tbComponent.GetRecordCell(0, L"EC_Type", tcTemp, 256))			m_strECType = tcTemp;
		if (tbComponent.GetRecordCell(0, L"EC_Code", tcTemp, 256))			m_strEndCraCode = tcTemp;
		if (tbComponent.GetRecordCell(0, L"EC_Drive", tcTemp, 256))			m_strDriverCode = tcTemp;
		tbComponent.GetRecordCell(0, L"EC_MotorQty", (DWORD&)m_iNumOfMotor);
		if (tbComponent.GetRecordCell(0, L"EC_JointPlate", tcTemp, 256))	m_strJoiPlaType = tcTemp;
		tbComponent.GetRecordCell(0, L"EC_Matrial", (DWORD&)m_iEndCar_Matrial);
		if (tbComponent.GetRecordCell(0, L"EC_Structure", tcTemp, 256)) m_strECStructure = tcTemp;
		tbComponent.GetRecordCell(0, L"EC_Wheelbase", (DWORD&)m_iWheBase);
		tbComponent.GetRecordCell(0, L"EC_TotalPrice", (DWORD&)m_iEndCarPrice);
		if (tbComponent.GetRecordCell(0, L"EC_Additions", tcTemp, 256))		FillAdditionsList(tcTemp, m_lstEndCarAdditions);
		//
		if (tbComponent.GetRecordCell(0, L"CranePannelCode", tcTemp, 256))	m_strCraPannelCode = tcTemp;
		if (tbComponent.GetRecordCell(0, L"FastoonCode", tcTemp, 256))		m_strFastoonCode = tcTemp;
		if (tbComponent.GetRecordCell(0, L"OperterUnitCode", tcTemp, 256))	m_strOperterUnitCode = tcTemp;
		if (tbComponent.GetRecordCell(0, L"CranePowerSupplyCode", tcTemp, 256))		m_strCraPowSupplyCode = tcTemp;
		tbComponent.GetRecordCell(0, L"CranePannelPrice", (DWORD&)m_iCranePannelPrice);
		tbComponent.GetRecordCell(0, L"FastoonPrice", (DWORD&)m_iFastoonPrice);
		tbComponent.GetRecordCell(0, L"OperterUnitPrice", (DWORD&)m_iOperterUnitPrice);
		tbComponent.GetRecordCell(0, L"CranePowerSupplyPrice", (DWORD&)m_iPowerSupplyPrice);
		if (tbComponent.GetRecordCell(0, L"EL_Addtions", tcTemp, 256))		FillAdditionsList(tcTemp, m_lstElecCtrlAdditions);
	}
	//
	tbComponent.CloseTable();
	UpdateSelectCodes();
	GetGearCodeFromDriver(m_strDriverCode);
	//
	switch(m_eNumOfHoist)
	{
	case eHoistNumType_One:
	case eHoistNumType_MainAux:
		m_iNumOfHoist = 1;
		break;
	case eHoistNumType_TwoSimilar:
		m_iNumOfHoist = 2;
		break;		 
	default:
		m_iNumOfHoist = 1;
		break;
	}

	// This is an exist data
	if (0 != m_iLoadOfCrane && 
		0 != m_iLoadOfHoist && 
		!m_strHoistCode.IsEmpty())
	{
		m_bIsEditData = TRUE;
	}

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

	tbComponent.SetRecordCell(0, L"CraneType", (DWORD)m_eCraneType);
	tbComponent.SetRecordCell(0, L"HoistNumberType", (DWORD)m_eNumOfHoist);
	tbComponent.SetRecordCell(0, L"CraneLoad", (DWORD)m_iLoadOfCrane);
	tbComponent.SetRecordCell(0, L"HoistLoad", (DWORD)m_iLoadOfHoist);
	tbComponent.SetRecordCell(0, L"Span", (float)m_fSpan);
	tbComponent.SetRecordCell(0, L"HOL", (float)m_fHOL);
	tbComponent.SetRecordCell(0, L"CraneDuty", m_strDutyOfCrane.GetBuffer(m_strDutyOfCrane.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistDuty", m_strDutyOfHoist.GetBuffer(m_strDutyOfHoist.GetLength()));
	tbComponent.SetRecordCell(0, L"RailType", m_strRailType.GetBuffer(m_strRailType.GetLength()));
	tbComponent.SetRecordCell(0, L"RailLength", (float)m_fRailLenght);
	tbComponent.SetRecordCell(0, L"RailTopToFloor", (DWORD)m_iRailTopToFloor);
	tbComponent.SetRecordCell(0, L"RailRoofToTop", (DWORD)m_iRoofToRailTop);
	tbComponent.SetRecordCell(0, L"PowerSupply", (DWORD)m_iVoltage1_Main);
	tbComponent.SetRecordCell(0, L"ControlVol", (DWORD)m_iVoltage2_Ctrl);
	tbComponent.SetRecordCell(0, L"CraneCtrlType", (DWORD)m_eCraneControl);
	tbComponent.SetRecordCell(0, L"MainUseType", (DWORD)m_eMainUseType);
	tbComponent.SetRecordCell(0, L"HoistSpeed", m_strHoistingSpeed.GetBuffer(m_strHoistingSpeed.GetLength()));
	tbComponent.SetRecordCell(0, L"TrolleySpeed", m_strTrolleySpeed.GetBuffer(m_strTrolleySpeed.GetLength()));
	tbComponent.SetRecordCell(0, L"BridgeSpeed", m_strBridgeSpeed.GetBuffer(m_strBridgeSpeed.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistCtrlType", (DWORD)m_eHoistCtrlType);
	tbComponent.SetRecordCell(0, L"TrolleyCtrlType", (DWORD)m_eTrolleyCtrlType);
	tbComponent.SetRecordCell(0, L"BridgeCtrlType", (DWORD)m_eBridgeCtrlType);
	tbComponent.SetRecordCell(0, L"ReevingType", (DWORD)m_eReevingType);
	//
	CString strHoistAddNames = GetHoistAdditionsNameString();
	tbComponent.SetRecordCell(0, L"HoistCode", m_strHoistCode.GetBuffer(m_strHoistCode.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistAdditions", strHoistAddNames.GetBuffer(strHoistAddNames.GetLength()));
	tbComponent.SetRecordCell(0, L"HoistTotalPrice", (DWORD)m_iHoistPrice);
	//
	if (!m_bOnlyHoist)
	{
		tbComponent.SetRecordCell(0, L"CraneCode", m_strCraneCode.GetBuffer(m_strCraneCode.GetLength()));
		tbComponent.SetRecordCell(0, L"MainGirderCode", m_strMainGirderCode.GetBuffer(m_strMainGirderCode.GetLength()));
		tbComponent.SetRecordCell(0, L"MainGirderPrice", (DWORD)m_iMainGirderPrice);
		tbComponent.SetRecordCell(0, L"MG_W1", (DWORD)m_iGirder_w1);
		tbComponent.SetRecordCell(0, L"MG_h1", (DWORD)m_iGirder_h1);
		tbComponent.SetRecordCell(0, L"MG_t1", (DWORD)m_iGirder_t1);
		tbComponent.SetRecordCell(0, L"MG_t2", (DWORD)m_iGirder_t2);
		tbComponent.SetRecordCell(0, L"MG_t3", (DWORD)m_iGirder_t3);
		tbComponent.SetRecordCell(0, L"MG_t4", (DWORD)m_iGirder_t4);
		tbComponent.SetRecordCell(0, L"MG_TroRail", m_strTroRail.GetBuffer(m_strTroRail.GetLength()));	
		tbComponent.SetRecordCell(0, L"MG_Matrial", (DWORD)m_iGirder_m);
		tbComponent.SetRecordCell(0, L"MG_TrolleyRail_W", (DWORD)m_iGirder_Rw);
		tbComponent.SetRecordCell(0, L"MG_TrolleyRail_H", (DWORD)m_iGirder_Rh);
		tbComponent.SetRecordCell(0, L"MG_w3", (DWORD)m_iGirder_w3);
		tbComponent.SetRecordCell(0, L"MG_c1", (DWORD)m_iGirder_c1);
		tbComponent.SetRecordCell(0, L"MG_t5", (DWORD)m_iGirder_t5);
		tbComponent.SetRecordCell(0, L"MG_h2", (DWORD)m_iGirder_h2);
		//
		CString strECAddNames = GetAdditionsNameString(m_lstEndCarAdditions);
		tbComponent.SetRecordCell(0, L"EC_Type", m_strECType.GetBuffer(m_strECType.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Code", m_strEndCraCode.GetBuffer(m_strEndCraCode.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Drive", m_strDriverCode.GetBuffer(m_strDriverCode.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_MotorQty", (DWORD)m_iNumOfMotor);
		tbComponent.SetRecordCell(0, L"EC_JointPlate", m_strJoiPlaType.GetBuffer(m_strJoiPlaType.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Matrial", (DWORD)m_iEndCar_Matrial);
		tbComponent.SetRecordCell(0, L"EC_Structure", m_strECStructure.GetBuffer(m_strECStructure.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_Wheelbase", (DWORD)GetWheelBase());
		tbComponent.SetRecordCell(0, L"EC_Additions", strECAddNames.GetBuffer(strECAddNames.GetLength()));
		tbComponent.SetRecordCell(0, L"EC_TotalPrice", (DWORD)m_iEndCarPrice);		
		//
		CString strELAddNames = GetAdditionsNameString(m_lstElecCtrlAdditions);
		tbComponent.SetRecordCell(0, L"CranePannelCode", m_strCraPannelCode.GetBuffer(m_strCraPannelCode.GetLength()));
		tbComponent.SetRecordCell(0, L"FastoonCode", m_strFastoonCode.GetBuffer(m_strFastoonCode.GetLength()));
		tbComponent.SetRecordCell(0, L"OperterUnitCode", m_strOperterUnitCode.GetBuffer(m_strOperterUnitCode.GetLength()));
		tbComponent.SetRecordCell(0, L"CranePowerSupplyCode", m_strCraPowSupplyCode.GetBuffer(m_strCraPowSupplyCode.GetLength()));
		tbComponent.SetRecordCell(0, L"EL_Addtions", strELAddNames.GetBuffer(strELAddNames.GetLength()));
		tbComponent.SetRecordCell(0, L"CranePannelPrice", (DWORD)m_iCranePannelPrice);		
		tbComponent.SetRecordCell(0, L"FastoonPrice", (DWORD)m_iFastoonPrice);		
		tbComponent.SetRecordCell(0, L"OperterUnitPrice", (DWORD)m_iOperterUnitPrice);		
		tbComponent.SetRecordCell(0, L"CranePowerSupplyPrice", (DWORD)m_iPowerSupplyPrice);		
	}
	//
	DWORD dwTotalPrice = m_iNumOfHoist*m_iHoistPrice + m_iMainGirderPrice * m_fGirder_mt/1000.f + m_iEndCarPrice + m_iELAdditionsPrice + m_iCranePannelPrice + m_iFastoonPrice + m_iOperterUnitPrice + m_iPowerSupplyPrice;
	tbComponent.SetRecordCell(0, L"ComponentPrice", dwTotalPrice);
	//
	BOOL bResult = FALSE;
	bResult = tbComponent.UpdateTable();
	tbComponent.CloseTable();

	return TRUE;
}
//-----------------------------------------------------------------------
BOOL CBMParamCalculator::LoadOtherParamsFromDatabase()
{

	/*	Get other hoist parameters contniue */
	CString strHoistTableName;
	CString strHoistDimTableName;
	switch(m_eCraneType)
	{
	case eCraneType_GTS:
		strHoistTableName = g_tcTableName[eTableID_HoiSel_S];
		strHoistDimTableName = g_tcTableName[eTableID_DimHoi_S];
		break;
	case eCraneType_GTD:
		strHoistTableName = g_tcTableName[eTableID_HoiSel_D];
		strHoistDimTableName = g_tcTableName[eTableID_DimHoi_D];
		break;
	case eCraneType_GTF:
		strHoistTableName = g_tcTableName[eTableID_HoiSel_F];
		strHoistDimTableName = g_tcTableName[eTableID_DimHoi_F];
		break;
	}
	CString strSQL;
	CADOTable tbHoist;
	strSQL.Format(_T("SELECT * FROM %s WHERE HoiTyeCod='%s'"), strHoistTableName, m_strHoistCode);
	if (tbHoist.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbHoist.GetRecordCnt() > 0)
	{
		TCHAR tcRaiGauge[32] = {_T("\0")};
		tbHoist.GetRecordCell(0, L"HoiLod", (DWORD&)m_iHoistCap);
		tbHoist.GetRecordCell(0, L"HOL", m_fHoistHOL);
		tbHoist.GetRecordCell(0, L"B0", tcRaiGauge, 32);
		//
		if (eCraneType_GTS == m_eCraneType)
		{
			m_strRaiGauge = tcRaiGauge;
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
			m_strRaiGauge.Format(_T("%d"), iNewRailGauage);
		}
	}
	tbHoist.CloseTable();
	//	
	//	Data from DimHoist
	CADOTable tbDimHoiSel;
	strSQL.Format(L"SELECT * FROM %s WHERE HoiDimIndCod='%s'", strHoistDimTableName, m_strHoiDimIndCode);
	if (tbDimHoiSel.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbDimHoiSel.GetRecordCnt() > 0)
	{
		tbDimHoiSel.GetRecordCell(0, L"WgtHoi", (DWORD&)m_iHoist_G1);
		tbDimHoiSel.GetRecordCell(0, L"b1", (DWORD&)m_iHoist_b1);
		tbDimHoiSel.GetRecordCell(0, L"b2", (DWORD&)m_iHoist_b2);
		tbDimHoiSel.GetRecordCell(0, L"b", (DWORD&)m_iHoist_b);
		tbDimHoiSel.GetRecordCell(0, L"h2", (DWORD&)m_iHoist_H2);
	}
	tbDimHoiSel.CloseTable();
	//
	// Data from table "HoiMacIndCod"
	CADOTable tbHoiMacIndCod;
	strSQL.Format(_T("SELECT * FROM %s WHERE HoiMacIndCod = '%s'"), g_tcTableName[eTableID_HoiMacIndCod], m_strHoiMacIndCode);
	if (tbHoiMacIndCod.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbHoiMacIndCod.GetRecordCnt() > 0)
	{
		TCHAR tcValue[32] = {0};
		tbHoiMacIndCod.GetRecordCell(0, _T("PowHoiMot"), tcValue, 32);	
		tbHoiMacIndCod.GetRecordCell(0, _T("PowHoiMotH"), m_fHoist_PowH);
		m_strHoist_Pow = tcValue;
	}
	tbHoiMacIndCod.CloseTable();
	//
	// Data from table "TroDriTyp"
	CADOTable tbTroDriTyp;
	strSQL.Format(_T("SELECT * FROM %s WHERE HoiDimIndCod = '%s'"), g_tcTableName[eTableID_TroDriTyp], m_strHoiDimIndCode);
	if (tbTroDriTyp.OpenTable(g_pSettingDatabase->GetConnectionPtr(), strSQL) && tbTroDriTyp.GetRecordCnt() > 0)
	{
		DWORD dwTroDriNum = 1;
		TCHAR tcTroDriTyp[32] = {0};
		tbTroDriTyp.GetRecordCell(0, _T("TroDriTyp"), tcTroDriTyp, 32);		
		tbTroDriTyp.GetRecordCell(0, _T("TroDriNum"), dwTroDriNum);	
		//	
		m_strTroMachineryType = tcTroDriTyp;
		m_iTroDriNum = dwTroDriNum;
	}
	tbTroDriTyp.CloseTable();

	strSQL.Format(_T("SELECT * FROM %s WHERE RaiTyp='%s'"), g_tcTableName[eTableID_DatRai], m_strRailType);
	m_iRaiWidTop = (int)GetFLOATValueFromTable(strSQL, _T("RaiWidTop"));

	// Trolley motor power
	strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp = '%s'"), g_tcTableName[eTableID_TroDriDat], m_strTroMachineryType);
	m_fHoist_DriPow = GetFLOATValueFromTable(strSQL, _T("PowTraDri"));
	m_iBridgeDriverPrice = GetFLOATValueFromTable(strSQL, _T("PriTraDriRmb"));

	// EndCar motor power and price
	strSQL.Format(_T("SELECT * FROM %s WHERE TroDriTyp='%s'"), g_tcTableName[eTableID_TroDriDat], m_strDriverCode);
	m_fEndCar_DriPow = GetFLOATValueFromTable(strSQL, _T("PowTraDri"));
	m_iBridgeDriverPrice = GetFLOATValueFromTable(strSQL, _T("PriTraDriRmb"));
	// EndCar weight
	strSQL.Format(_T("SELECT * FROM %s WHERE EndCarTyp='%s'"), g_tcTableName[eTableID_EndCarWei], m_strEndCraCode);	
	m_iEndCar_Wgt = GetLONGValueFromTable(strSQL, _T("ECWei"));

	return TRUE;
}
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
bool CBMParamCalculator::GetMinAndClearanceSize(eCraneType eType, 
												long lCraneLoad, 
												int &iMinSize, 
												int &iClearance)
{
	CString strParamName;
	if (eCraneType_GTD == eType) 
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
	CString strValue;

	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_c2"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param1", strValue.GetBuffer(64), 64);
	m_iParam_c2 = _ttol(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_dh"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param1", strValue.GetBuffer(64), 64);
	m_fParam_dh = _tstof(strValue);
	//
	CString strParam_F;
	strParam_F.Format(_T("Calc_MG_F%d"), m_iGirder_t3);
	iIndex = GetParamIndexInTable(tbCalcParams, strParam_F);
	if (iIndex < 0) return FALSE;	
	tbCalcParams.GetRecordCell(iIndex, L"Param1", m_sParam_FText.GetBuffer(32), 32);
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_F = _tstof(strValue);
	//
	TCHAR tcfmax[32] = {};
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_fmax"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", tcfmax, 32);
	strValue = tcfmax;
	strValue = strValue.Right(strValue.GetLength() - (strValue.Find(_T('/'))+1));
	m_fParam_fmax = 1.0f / _ttol(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_G2"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_iParam_G2 = _ttol(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_h"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_h = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Kbz"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_Kbz = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Kpx"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_Kpx = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_Kpz"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_Kpz = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_ld"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_ld = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_md"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_md = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_n"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_n = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_r"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_r = _tstof(strValue);
	//
	CString strParam_R;
	strParam_R.Format(_T("Calc_MG_R%d"), m_iGirder_Rw);
	iIndex = GetParamIndexInTable(tbCalcParams, strParam_R);
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param1", strValue.GetBuffer(64), 64);
	m_fParam_R = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_t6"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_lParam_t6 = _ttol(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_t7"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_lParam_t7 = _ttol(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_v1"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_v1 = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_v2"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_v2 = _tstof(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_MG_v3"));
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_v3 = _tstof(strValue);
	//
	CString strParam_H;
	strParam_R.Format(_T("Calc_MG_H%d"), GetParamHRightValue(m_iGirder_h1));
	iIndex = GetParamIndexInTable(tbCalcParams, strParam_R);
	if (iIndex < 0) return FALSE;
	tbCalcParams.GetRecordCell(iIndex, GetParamHItemName(m_iGirder_t3), strValue.GetBuffer(64), 64);
	tbCalcParams.GetRecordCell(iIndex, GetParamHTextItemName(m_iGirder_t3), m_strParam_HText.GetBuffer(64), 64);
	m_fParam_H = _ttol(strValue);
	//
	iIndex = GetParamIndexInTable(tbCalcParams, _T("Calc_EC_Stif"));
	tbCalcParams.GetRecordCell(iIndex, L"Param2", strValue.GetBuffer(64), 64);
	m_fParam_EC_Stiffness = _tstof(strValue);

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