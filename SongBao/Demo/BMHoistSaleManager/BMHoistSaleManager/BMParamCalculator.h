//-----------------------------------------------------------------------
//	File Name:BMParamCalculator.h
//	Introduce:header file
//-----------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------
#include "CommonDefine.h"
#include <list>
using namespace std;
//-----------------------------------------------------------------------



//-----------------------------------------------------------------------
class CBMParamCalculator
{
public:
	CBMParamCalculator(void);
	virtual ~CBMParamCalculator(void);
	//-------------------------------------------------------------------
public:
	void	UpdateSelectCodes();
	int		GetGirder_c1();
	int		GetGirder_h2(BOOL bForce = FALSE);
	void	UpdateMainGirderData(CString strMainGirderData);
	BOOL	CalcMainGirder();
	float	GetMainGirder_Q0();
	float	GetMainGirder_Q1();
	float	GetMainGirder_Q2();
	float	GetMainGirder_T();
	float	GetMainGirder_F();
	float	GetMainGirder_S();
	//
	int		GetWheelBase(BOOL bForce = FALSE);
	BOOL	CalcEndCar();
	CString	GetEndCarDataText(int iWheDim = 0);
	CString GetJoiPlaTypeString(int iWheDim = 0);
	CString	GetBufferType();
	CString	GetGearCodeFromDriver(CString strDriver);
	float	GetCapacityFactor();
	float	GetBufferExtend();
	//
	BOOL	GetCompomentTechData(TechDataList &dataList);
	BOOL	GetHoistTechData(TechDataList &dataList);
	BOOL	GetHoistStdAddTechData(TechDataList &dataList);
	BOOL	GetHoistSelAddTechData(TechDataList &dataList);
	BOOL	GetCranesTechData(TechDataList &dataList);
	BOOL	GetCraneSelAddTechData(TechDataList &dataList);
	//
	BOOL	LoadDataFromDatabase(CString strOfferID, CString strComponentName);
	BOOL	SaveDataToDatabase(CString strOfferID, CString strComponentName);
	BOOL	LoadOtherParamsFromDatabase();
	//-------------------------------------------------------------------
public:
	float static GetLeftValueOfRangeString(CString strRange);
	float static GetRightValueOfRangeString(CString strRange);
	bool  static GetMinAndClearanceSize(eCraneType eType, long lCraneLoad, int &iMinSize, int &iClearance);
	//-------------------------------------------------------------------
private:
	BOOL	GetCalcParamFormDB();
	int		GetParamIndexInTable(CADOTable &tb, CString strCellName);
	int		GetParamHRightValue(int iInputH);
	CString	GetParamHItemName(int iInputT3);
	CString	GetParamHTextItemName(int iInputT3);
	int		GetFrequencyByVoltage(int iVoltage);
	CString	GetHoistElectricProvision();
	CString	GetHoistElectricalCubiclesIPClass();
	CString GetAdditionsNameString(list<CString> lstAdditions);
	CString	GetHoistAdditionsNameString();
	void	FillAdditionsList(CString strNameStrings, list<CString> &lstAdditions);
	void	FillHoistAdditionsList(CString strNameStrings);
	//-------------------------------------------------------------------
public:
	BOOL	m_bOnlyHoist;
	BOOL	m_bIsEditData;
	/*	Basic parameters*/
	int		m_iLoadOfCrane;		//kg
	int		m_iLoadOfHoist;		//kg
	int		m_iNumOfHoist;
	int		m_iRailTopToFloor;	//mm
	int		m_iRoofToRailTop;	//mm
	int		m_iVoltage1_Main;	//V
	int		m_iVoltage2_Ctrl;	//V
	int		m_iDutyOfHoist;
	int		m_iRaiWidTop;
	float	m_fHoistSpeed;
	double	m_fSpan;			//m
	double	m_fHOL;				//m
	double	m_fRailLenght;		//m
	//
	eCraneType			m_eCraneType;
	eHoistNumType		m_eNumOfHoist;
	eCraneCtrlType		m_eCraneControl;
	eCraneMainUseType	m_eMainUseType;
	eElecCtrlType		m_eHoistCtrlType;
	eElecCtrlType		m_eTrolleyCtrlType;
	eElecCtrlType		m_eBridgeCtrlType;
	eReevingType		m_eReevingType;
	//
	CString		m_strDutyOfCrane;
	CString		m_strDutyOfHoist;
	CString		m_strRailType;
	CString		m_strHoistingSpeed;
	CString		m_strTrolleySpeed;
	CString		m_strBridgeSpeed;
	
	/*	Hoist code and size parameter*/
	CString		m_strHoistCode;
	CString		m_strHoiMacIndCode;		//起升查询码		0,1,2,8,9,10,11
	CString		m_strDruAssCode;		//卷筒组代码		0,1,2,3,4,13
	CString		m_strHoiDimIndCode;		//葫芦尺寸查询码	0,1,2,3,4,12,13
	CString		m_strHokAssIndCode;		//吊钩查询码		0,1,2,3,4
	CString		m_strHoiPanIndCode;		//电箱查询码		0,1,2,3,4,12,8,9
	CString		m_strTroFraCode;		//机架号代码		0,1,2
	CString		m_strHoiDrawCode;		//葫芦图纸代码		0,1,2,3,4,12
	CString		m_strRaiGauge;			//For 'Only Hoist', is a true string like "300-500"
	CString		m_strHoist_Pow;
	CString		m_strHoistElecProvision;	
	CString		m_strHoistElecCubIPClass;
	CString		m_strTroMachineryType;
	list<CString>	m_lstStdHoistAdditions;
	list<CString>	m_lstOtherHoistAdditions;
	list<CString>	m_lstEndCarAdditions;
	list<CString>	m_lstElecCtrlAdditions;
	int			m_iHoist_H2;
	int			m_iHoist_b;
	int			m_iHoist_b1;
	int			m_iHoist_b2;
	int			m_iHoist_G1;
	int			m_iHoist_V3;
	int			m_iTroDriNum;
	int			m_iHoistPrice;
	int			m_iHoistCap;
	float		m_fHoistHOL;
	float		m_fHoist_PowH;
	float		m_fHoist_DriPow;

	/*	Girder and EndCar parameters*/
	CString		m_strCraneCode;
	CString		m_strMainGirderCode;
	CString		m_strEndCraCode;
	CString		m_strDriverCode;
	CString		m_strGearCode;
	CString		m_strMotorCode;
	CString		m_strJoiPlaType;
	CString		m_strECStructure;
	CString		m_strTroRail;
	CString		m_strECType;
	int			m_iNumOfMotor;
	int			m_iGirder_w1;
	int			m_iGirder_w3;
	int			m_iGirder_c1;
	int			m_iGirder_h1;
	int			m_iGirder_h2;
	int			m_iGirder_t1;
	int			m_iGirder_t2;
	int			m_iGirder_t3;
	int			m_iGirder_t4;
	int			m_iGirder_t5;
	int			m_iGirder_Rw;		//Rail width
	int			m_iGirder_Rh;		//Rail height
	int			m_iGirder_m;		//matrial
	//
	BOOL		m_bRailTypeIsOK;
	int			m_iEncCar_EcIx;
	int			m_iEndCar_EcW;
	int			m_iEndCar_Matrial;
	int			m_iWheBase;
	int			m_iMaxSpeed;
	float		m_fDriverLoad;
	float		m_fEndCar_DriPow;
	int			m_iMainGirderPrice;
	int			m_iEndCarPrice;
	int			m_iBridgeDriverPrice;
	
	/*	Elec data parameters*/
	CString		m_strCraPannelCode;
	CString		m_strFastoonCode;
	CString		m_strOperterUnitCode;
	CString		m_strCraPowSupplyCode;

	/*	Calculating parameters*/
	int			m_iParam_c2;
	double		m_fParam_dh;
	float		m_fParam_F;
	CString		m_sParam_FText;
	double		m_fParam_fmax;
	int			m_iParam_G2;
	double		m_fParam_h;
	double		m_fParam_Kpx;
	double		m_fParam_Kpz;
	double		m_fParam_Kbz;
	double		m_fParam_ld;
	double		m_fParam_md;
	double		m_fParam_n;
	double		m_fParam_r;
	double		m_fParam_R;
	long		m_lParam_t6;
	long		m_lParam_t7;
	double		m_fParam_v1;
	double		m_fParam_v2;
	double		m_fParam_v3;
	long		m_fParam_H;
	CString		m_strParam_HText;
	float		m_fParam_EC_Stiffness;
	//
	double		m_fGirder_mr;
	double		m_fGirder_mz;
	double		m_fGirder_ms;
	double		m_fGirder_mt;
	double		m_fGirder_StdQ;
	double		m_fGirder_StdT0;
	double		m_fGirder_Stdfmax;
	double		m_fGirder_Stdsmax;
	//
	int			m_iEndCar_Wgt;
	int			m_iEndCarWheelCnt;
	int			m_iCraneTotalWgt;
	float		m_fMaxWheLoad;
	float		m_fMaxStress;		//σ - EC
	float		m_fMaxBlend;		//F - EC
	//
	int			m_iELAdditionsPrice;
	int			m_iCranePannelPrice;
	int			m_iFastoonPrice;
	int			m_iOperterUnitPrice;
	int			m_iPowerSupplyPrice;
	//-------------------------------------------------------------------
private:
	double		m_fGirder_A;	//Girder area
	double		m_fGirder_w2;
	double		m_fGirder_x;	//Girder 
	double		m_fGirder_y;
	double		m_fGirder_Ix;
	double		m_fGirder_Ix1;
	double		m_fGirder_Ix2;
	double		m_fGirder_Ix3;
	double		m_fGirder_Ix4;
	double		m_fGirder_Iy;
	double		m_fGirder_Iy1;
	double		m_fGirder_Iy2;
	double		m_fGirder_Iy3;
	double		m_fGirder_Iy4;
	double		m_fGirder_Wx;
	double		m_fGirder_Wy;
	double		m_fGirder_mx;
	double		m_fGirder_js2;
	double		m_fGirder_A1;
	double		m_fGirder_mj1;
	double		m_fGirder_mj2;
	double		m_fGirder_Fq;
	double		m_fGirder_Ep;
	double		m_fGirder_P1;
	double		m_fGirder_P2;
	double		m_fGirder_FQ1;
	double		m_fGirder_FQ2;
	double		m_fGirder_FQ4;
	double		m_fGirder_Mx;
	double		m_fGirder_Fp;
	double		m_fGirder_Fj1;
	double		m_fGirder_Fj2;
	double		m_fGirder_Pmax;
	double		m_fGirder_Fh;
	double		m_fGirder_Ph;
	double		m_fGirder_Mh;
	double		m_fGirder_Pr1;
	double		m_fGirder_Ps1;
	double		m_fGirder_Ms;
	double		m_fGirder_My;
	double		m_fGirder_Q0;
	double		m_fGirder_Q0x;
	double		m_fGirder_Q0y;
	double		m_fGirder_Qp;
	double		m_fGirder_Qpx;
	double		m_fGirder_Qpz;
	double		m_fGirder_Qbz;
	double		m_fGirder_Q1;
	double		m_fGirder_Q2;
	double		m_fGirder_Q3;
	double		m_fGirder_Q4;
	double		m_fGirder_sy;
	double		m_fGirder_T0;
	double		m_fGirder_T1;
	double		m_fGirder_Qm;
	double		m_fGirder_f;
	double		m_fGirder_s;
	double		m_fGirder_hw2;
	double		m_fGirder_fmax;
	double		m_fGirder_smax;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
