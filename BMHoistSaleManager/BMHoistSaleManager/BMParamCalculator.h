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
	void	UpdateBasicDrawingData();
	int		GetGirder_h2(BOOL bForce = FALSE);
	void	UpdateMainGirderData(CString strMainGirderData);
	BOOL	CalcMainGirder();
	//
	BOOL	UpdateRailData();
	BOOL	UpdateRailDataR04R05();
	BOOL	UpdateRailDataR08R09();
	BOOL	UpdateRailDataR10R11R13();
	BOOL	UpdateRailTypeData(BOOL bIsEndCarOption);
	BOOL	UpdateRailGaugeData();
	BOOL	UpdateJointPlateData();
	BOOL	UpdateDriverTypeData();
	BOOL	UpdateWheelBaseData(BOOL bForced = TRUE);
	BOOL	UpdateBufferExtendData();
	BOOL	UpdateBufferTypeData();
	BOOL	UpdateECStructureData();
	BOOL	UpdateEndCarDataText();
	BOOL	CalcEndCar();
	//
	BOOL	UpdateElecCodeData();
	BOOL	UpdateElecPannelselection();
	//
	BOOL	GetCompomentTechData(TechDataList &dataList);
	BOOL	GetHoistTechData(TechDataList &dataList);
	BOOL	GetHoistTechDataEx(TechDataList &dataList);
	BOOL	GetHoistStdAddTechData(TechDataList &dataList);
	BOOL	GetHoistSelAddTechData(TechDataList &dataList);
	BOOL	GetCranesTechData(TechDataList &dataList);
	BOOL	GetCranesTechHoistData(TechDataList &dataList);
	BOOL	GetCranesTechHoistDataEx(TechDataList &dataList);
	BOOL	GetCraneSelAddTechData(TechDataList &dataList);
	BOOL	ConstructTechData();
	BOOL	ConstructMainGirderDrawingData();
	BOOL	ConstructEndcarDrawingData();
	BOOL	ConstructSteelStructDrawingData();
	//
	BOOL	LoadDataFromDatabase(CString strOfferID, CString strComponentName);
	BOOL	SaveDataToDatabase(CString strOfferID, CString strComponentName);
	BOOL	LoadOtherBasicData();
	BOOL	LoadOtherHoistData();
	BOOL	LoadOtherMainGriderData();
	BOOL	LoadOtherEndcarData();
	BOOL	LoadOtherElecData();
	CString	LoadHoistRaiGaugeData();
	void	ParserSelMainHoistData();
	void	ParserSelAuxHoistData();
	//-------------------------------------------------------------------
public:
	float static GetLeftValueOfRangeString(CString strRange);
	float static GetRightValueOfRangeString(CString strRange);
	bool  static GetMinAndClearanceSize(CString strTypeCode, long lCraneLoad, int &iMinSize, int &iClearance);
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
	BOOL			m_bOnlyHoist;
	BOOL			m_bIsEditData;
	BOOL			m_bRailTypeIsOK;
	CalcDataList	m_CalcDatalst;
	list<CString>	m_lstStdHoistAdditions;
	list<CString>	m_lstOtherHoistAdditions;
	list<CString>	m_lstEndCarAdditions;
	list<CString>	m_lstElecCtrlAdditions;

	/* Waiting for deleting */
	eCraneType		m_eCraneType;	
	CString			m_strHoiDrawCode;		//葫芦图纸代码		0,1,2,3,4,12
	int				m_iELAdditionsPrice;
	//-------------------------------------------------------------------
};
//-----------------------------------------------------------------------
