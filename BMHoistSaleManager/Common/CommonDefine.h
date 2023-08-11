//----------------------------------------------------
//	File Name:CommonDefine.h
//	Introduce:Common data struct definition
//
//----------------------------------------------------
#ifndef	_COMMON_DEFINE_H_
#define	_COMMON_DEFINE_H_
//----------------------------------------------------
#include <list>
#include "BMDataDefine.h"
#include "ADODataBase.h"
#include "ADOTable.h"
#include "..\\Office\\Word\\MSWordApplication.h"
#include "..\\BMHoistSaleManager\\Localization.h"
using namespace std;
//----------------------------------------------------
#define	MAX_COMPONENT_CNT		10
#define	STR_OFFER_PSW			_T("irqi{lhdfcl")
//----------------------------------------------------
#define	WM_USER_SWITCHLANG			WM_USER	+ 1
#define	WM_USER_SHOWCUSTOMER		WM_USER + 2
#define	WM_USER_SHOWOFFERINFO		WM_USER + 3
#define	WM_USER_DATAISUPDATED		WM_USER + 4
#define	WM_USER_PAGECHANGED			WM_USER + 5		//0:Current page is not the first and the last;1:Current page is the first;2:Current page is the last
#define	WM_USER_UPDATETOTALPRICE	WM_USER + 6
//----------------------------------------------------
#define	COLOR_PRICE		RGB(0, 255, 0)
#define	COLOR_PASS		RGB(0, 255, 0)
#define	COLOR_NOTPASS	RGB(255, 0, 0)
//----------------------------------------------------
#define	STR_BMD					_T("D")
#define	STR_BMS					_T("S")
#define	STR_CALCPARAM_LOAD		_T("Load")
//
#define	STR_COMTYPE_HOIST		_T("Hoist")
#define	STR_COMTYPE_PACKAGE		_T("Package")
//
#define	STR_HOIST_STDADD		_T("hoist_add_h")
#define	STR_HOIST_ADDGAU		_T("hoist_add_r")
//----------------------------------------------------
#define	STR_TECHDATA_LIST_HOIST			_T("Hoist")
#define	STR_TECHDATA_LIST_ENDCAR		_T("End carriage")
#define	STR_TECHDATA_LIST_BRIDGEMOTOR	_T("Bridge motor")
#define	STR_TECHDATA_LIST_BRIDGEPANNEL	_T("Bridge panel")
#define	STR_TECHDATA_LIST_TROLLEYPOWER	_T("Trolley power supply")
#define	STR_TECHDATA_LIST_CRANEPOWER	_T("Cranes power supply")
#define	STR_TECHDATA_LIST_OPERATEUNIT	_T("Operater unit")
#define	STR_TECHDATA_LIST_STEELSTRUCT	_T("Steel structure")
//
#define	STR_TECHDATA_HOIST_TYPE			_T("Hoist type code")
#define	STR_TECHDATA_HOIST_FRAMESIZE	_T("Frame size")
#define	STR_TECHDATA_HOIST_REEVINGCODE	_T("Rope reeving code")
#define	STR_TECHDATA_HOIST_DRUMLENCODE	_T("Drum length code")
#define	STR_TECHDATA_HOIST_DUTYGROUP	_T("Hoist duty group")
#define	STR_TECHDATA_HOIST_LOAD			_T("LOAD(SWL)")
#define	STR_TECHDATA_HOIST_VOLTAGE1		_T("Voltage 1(main)")
#define	STR_TECHDATA_HOIST_VOLTAGE2		_T("Voltage 2(control)")
#define	STR_TECHDATA_HOIST_FREQUENCY	_T("Frequency")
#define	STR_TECHDATA_HOIST_HEIGHTOFLIT	_T("Height of lift")
#define	STR_TECHDATA_HOIST_CTRLTYPE		_T("Hoisting control type")
#define	STR_TECHDATA_HOIST_MOTORTYPE	_T("Hoisting motor type and power")
#define	STR_TECHDATA_HOIST_SPEED		_T("Hoisting speed")
#define	STR_TECHDATA_HOIST_SPEEDH		_T("Hoisting speed,high")
#define	STR_TECHDATA_HOIST_SPEEDL		_T("Hoisting speed,low")
#define	STR_TECHDATA_HOIST_TROSPEED		_T("Trolley speed")
#define	STR_TECHDATA_HOIST_TROSPEEDH	_T("Trolley speed,high")
#define	STR_TECHDATA_HOIST_TROSPEEDL	_T("Trolley speed,low")
#define	STR_TECHDATA_HOIST_TROMOTOR		_T("Trolley motor power")
#define	STR_TECHDATA_HOIST_TROCTRLTYPE	_T("Trolley traversing control type")
#define	STR_TECHDATA_HOIST_DATAPLATE	_T("Data plate language / Type code")
#define	STR_TECHDATA_HOIST_TROLLEYTYPE	_T("Trolley type")
#define	STR_TECHDATA_HOIST_RAILORGIRDER	_T("Rail gauge /Width of girder")
#define	STR_TECHDATA_HOIST_RAILTYPE		_T("Rail type")
#define	STR_TECHDATA_HOIST_TRODUTY		_T("Trolley traversing duty group")
#define	STR_TECHDATA_HOIST_TROMACHINERY	_T("Trolley traversing machinery type")
#define	STR_TECHDATA_HOIST_MACHINERYNUM	_T("Number of traversing machinery")
#define	STR_TECHDATA_HOIST_WEIGHT		_T("Hoist and trolley weight")
#define	STR_TECHDATA_HOIST_OVERLOAD		_T("Overload device")
#define	STR_TECHDATA_HOIST_ELECPROV		_T("Electric provision")
#define	STR_TECHDATA_HOIST_ELECIP		_T("Electric cuicles IP class")
#define	STR_TECHDATA_HOIST_MOTORIP		_T("Motor IP class")
/* Main and Aux hoist data */
#define	STR_TECHDATA_HOIST_TYPE_MAIN			_T("Hoist type code (Main)")
#define	STR_TECHDATA_HOIST_FRAMESIZE_MAIN		_T("Frame size (Main)")
#define	STR_TECHDATA_HOIST_REEVINGCODE_MAIN		_T("Rope reeving code (Main)")
#define	STR_TECHDATA_HOIST_DRUMLENCODE_MAIN		_T("Drum length code (Main)")
#define	STR_TECHDATA_HOIST_LOAD_MAIN			_T("LOAD(SWL) (Main)")
#define	STR_TECHDATA_HOIST_HEIGHTOFLIT_MAIN		_T("Height of lift(Max.hoist/Actual crane) (Main)")
#define	STR_TECHDATA_HOIST_SPEED_MAIN			_T("Hoisting speed (Main)")
#define	STR_TECHDATA_HOIST_MOTORTYPE_MAIN		_T("Hoisting motor type and power (Main)")
#define	STR_TECHDATA_HOIST_DUTYGROUP_MAIN		_T("Hoist duty group (Main)")
#define	STR_TECHDATA_HOIST_TYPE_AUX				_T("Hoist type code (Aux)")
#define	STR_TECHDATA_HOIST_FRAMESIZE_AUX		_T("Frame size (Aux)")
#define	STR_TECHDATA_HOIST_REEVINGCODE_AUX		_T("Rope reeving code (Aux)")
#define	STR_TECHDATA_HOIST_DRUMLENCODE_AUX		_T("Drum length code (Aux)")
#define	STR_TECHDATA_HOIST_LOAD_AUX				_T("LOAD(SWL) (Aux)")
#define	STR_TECHDATA_HOIST_HEIGHTOFLIT_AUX		_T("Height of lift(Max.hoist/Actual crane) (Aux)")
#define	STR_TECHDATA_HOIST_SPEED_AUX			_T("Hoisting speed (Aux)")
#define	STR_TECHDATA_HOIST_MOTORTYPE_AUX		_T("Hoisting motor type and power (Aux)")
#define	STR_TECHDATA_HOIST_DUTYGROUP_AUX		_T("Hoist duty group (Aux)")
//
#define	STR_TECHDATA_CRANE_TYPE			_T("Cranes type")
#define	STR_TECHDATA_CRANE_CAPACITY		_T("Capacity cranes")
#define	STR_TECHDATA_CRANE_SPAN			_T("Span")
#define	STR_TECHDATA_CRANE_LIFTHEIGHT	_T("Cranes lifting height")
#define	STR_TECHDATA_CRANE_DUTYGROUP	_T("Cranes duty group")
#define	STR_TECHDATA_CRANE_WEIGHT		_T("Cranes weight")
#define	STR_TECHDATA_CRANE_STEELWEIGHT	_T("Steel structure weight")
#define	STR_TECHDATA_CRANE_TROWEIGHT	_T("Trolley weight")
#define	STR_TECHDATA_CRANE_ENDCARWEIGHT	_T("End carriage weight")
#define	STR_TECHDATA_CRANE_RAILWHEELNUM	_T("One side wheel number")
#define	STR_TECHDATA_CRANE_WHEELLOAD	_T("Max. Static wheel load")
#define	STR_TECHDATA_CRANE_RAILTYPE		_T("Cranes rail type")
#define	STR_TECHDATA_CRANE_MAINVOL		_T("Main voltage")
#define	STR_TECHDATA_CRANE_CTRLVOL		_T("Control voltage")
#define	STR_TECHDATA_CRANE_CURRENT		_T("Starting current/Nominal current")
#define	STR_TECHDATA_CRANE_MAINUSE		_T("Environment/Temperature")
#define	STR_TECHDATA_CRANE_HOISTTYPE	_T("Hoist type")
#define	STR_TECHDATA_CRANE_HOISTCAP		_T("Capacity hoist")
#define	STR_TECHDATA_CRANE_HOISTDUTY	_T("Hoist duty group")
#define	STR_TECHDATA_CRANE_HOISTLIFT	_T("Hoist lifting height")
#define	STR_TECHDATA_CRANE_TROTYPE		_T("Trolley type")
#define	STR_TECHDATA_CRANE_REEVING		_T("Reeving type")
#define	STR_TECHDATA_CRANE_HOISTSPD		_T("Hoisting speed")
#define	STR_TECHDATA_CRANE_TRAVERSSPD	_T("Traversing speed")
#define	STR_TECHDATA_CRANE_TRAVELSPD	_T("Traveling speed")
#define	STR_TECHDATA_CRANE_HOISTMOTOR	_T("Hoisting motor power")
#define	STR_TECHDATA_CRANE_TROMOTOR		_T("Trolley motor power")
#define	STR_TECHDATA_CRANE_CRANEMOTOR	_T("Cranes motor power")
#define	STR_TECHDATA_CRANE_TOTALPOW		_T("Supply power")
#define	STR_TECHDATA_CRANE_ELECIP		_T("Electrical cubicles IP class")
#define	STR_TECHDATA_CRANE_MOTORIP		_T("Motor IP class")
#define	STR_TECHDATA_CRANE_CTRLTYPE		_T("Control Type")
//
//----------------------------------------------------
#define	STR_TECHDATA_HOIST_EN_TYPE			_T("\tHoist type code......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_FRAMESIZE		_T("\tFrame size..............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_REEVINGCODE	_T("\tRope reeving code.................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_DRUMLENCODE	_T("\tDrum length code..................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_DUTYGROUP		_T("\tHoist duty group....................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_LOAD			_T("\tLOAD(SWL)............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_VOLTAGE1		_T("\tVoltage 1(main).....................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_VOLTAGE2		_T("\tVoltage 2(control)..................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_FREQUENCY		_T("\tFrequency..............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_HEIGHTOFLIT	_T("\tHeight of lift(Max. hoist/Actual crane)..................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_CTRLTYPE		_T("\tHoisting control type.............................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_MOTORTYPE		_T("\tHoisting motor type and power.............................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_SPEED 		_T("\tHoisting speed....................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_SPEEDH		_T("\tHoisting speed, high..............................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_SPEEDL		_T("\tHoisting speed, low...............................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TROSPEED		_T("\tTrolley speed........................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TROSPEEDH		_T("\tTrolley speed, high................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TROSPEEDL		_T("\tTrolley speed, low.................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TROMOTOR		_T("\tTrolley motor power.............................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TROCTRLTYPE	_T("\tTrolley traversing control type....................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_DATAPLATE		_T("\tData plate language / Type code..........................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TROLLEYTYPE	_T("\tTrolley type...........................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_RAILORGIRDER	_T("\tRail gauge /Width of girder..................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_RAILTYPE		_T("\tRail type...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TRODUTY		_T("\tTrolley traversing duty group................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TROMACHINERY	_T("\tTrolley traversing machinery type........................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_MACHINERYNUM	_T("\tNumber of traversing machinery.........................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_WEIGHT		_T("\tHoist and trolley weight.......................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_OVERLOAD		_T("\tOverload device....................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_ELECPROV		_T("\tElectric provision..................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_ELECIP		_T("\tElectric cuicles IP class..........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_MOTORIP		_T("\tMotor IP class.......................................................................%s\r\n")
/*	Main and Aux hoist data */
#define	STR_TECHDATA_HOIST_EN_TYPE_MAIN			_T("\tHoist type code (Main)...........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_FRAMESIZE_MAIN	_T("\tFrame size (Main)...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_REEVINGCODE_MAIN	_T("\tRope reeving code (Main)......................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_DRUMLENCODE_MAIN	_T("\tDrum length code (Main).......................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_LOAD_MAIN			_T("\tLOAD(SWL) (Main).................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_HEIGHTOFLIT_MAIN	_T("\tHeight of lift(Max. hoist/Actual crane) (Main).......................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_SPEED_MAIN		_T("\tHoisting speed (Main)............................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_MOTORTYPE_MAIN	_T("\tHoisting motor type and power (Main).................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_DUTYGROUP_MAIN	_T("\tHoist duty group (Main)........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_TYPE_AUX			_T("\tHoist type code (Aux).............................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_FRAMESIZE_AUX		_T("\tFrame size (Aux).....................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_REEVINGCODE_AUX	_T("\tRope reeving code (Aux)........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_DRUMLENCODE_AUX	_T("\tDrum length code (Aux).........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_LOAD_AUX			_T("\tLOAD(SWL) (Aux)...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_HEIGHTOFLIT_AUX	_T("\tHeight of lift(Max. hoist/Actual crane) (Aux).........................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_SPEED_AUX			_T("\tHoisting speed (Aux)..............................................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_MOTORTYPE_AUX		_T("\tHoisting motor type and power (Aux)...................................%s\r\n")
#define	STR_TECHDATA_HOIST_EN_DUTYGROUP_AUX		_T("\tHoist duty group (Aux)..........................................................%s\r\n")
//
#define	STR_TECHDATA_CRANE_EN_TYPE			_T("\tCranes type..............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_CAPACITY		_T("\tCapacity cranes........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_SPAN			_T("\tSpan.........................................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_LIFTHEIGHT	_T("\tCranes lifting height.................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_DUTYGROUP		_T("\tCranes duty group....................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_WEIGHT		_T("\tCranes weight..........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_STEELWEIGHT	_T("\tSteel structure weight.............................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_TROWEIGHT		_T("\tTrolley weight..........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_ENDCARWEIGHT	_T("\tEnd carriage weight.................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_RAILWHEELNUM	_T("\tOne side wheel number...........................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_WHEELLOAD		_T("\tMax. Static wheel load.............................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_RAILTYPE		_T("\tCranes rail type.........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_MAINVOL		_T("\tMain voltage............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_CTRLVOL		_T("\tControl voltage.........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_CURRENT		_T("\tStarting current/Nominal current............................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_MAINUSE		_T("\tEnvironment/Temperature......................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_HOISTTYPE		_T("\tHoist type................................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_HOISTCAP		_T("\tCapacity hoist..........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_HOISTDUTY		_T("\tHoist duty group......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_HOISTLIFT		_T("\tHoist lifting height...................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_TROTYPE		_T("\tTrolley type..............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_REEVING		_T("\tReeving type............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_HOISTSPD		_T("\tHoisting speed.........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_TRAVERSSPD	_T("\tTraversing speed......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_TRAVELSPD		_T("\tTraveling speed........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_HOISTMOTOR	_T("\tHoisting motor power..............................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_TROMOTOR		_T("\tTrolley motor power................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_CRANEMOTOR	_T("\tCranes motor power................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_TOTALPOW		_T("\tSupply power...........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_ELECIP		_T("\tElectrical cubicles IP class........................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_MOTORIP		_T("\tMotor IP class..........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_EN_CTRLTYPE		_T("\tControl Type............................................................................%s\r\n")
//
//----------------------------------------------------
#define	STR_TECHDATA_HOIST_CN_TYPE			_T("\t葫芦代码...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_FRAMESIZE		_T("\t机架代码...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_REEVINGCODE	_T("\t绕绳方式代码.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_DRUMLENCODE	_T("\t卷筒长度代码.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_DUTYGROUP		_T("\t起升工作级别.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_LOAD			_T("\t额定载荷...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_VOLTAGE1		_T("\t主电压...................................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_VOLTAGE2		_T("\t控制电压...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_FREQUENCY		_T("\t频率.......................................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_HEIGHTOFLIT	_T("\t起升高度...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_CTRLTYPE		_T("\t起升控制方式.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_MOTORTYPE		_T("\t起升电机种类和功率...........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_SPEED			_T("\t起升速度...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_SPEEDH		_T("\t起升速度,快速......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_SPEEDL		_T("\t起升速度,慢速......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TROSPEED		_T("\t小车速度..............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TROSPEEDH		_T("\t小车速度,快速......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TROSPEEDL		_T("\t小车速度,慢速......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TROMOTOR		_T("\t小车电机功率.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TROCTRLTYPE	_T("\tTrolley traversing control type....................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_DATAPLATE		_T("\t铭牌语言及类型...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TROLLEYTYPE	_T("\t小车结构形式.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_RAILORGIRDER	_T("\tD轨距/S主梁宽度..............................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_RAILTYPE		_T("\t小车轨道型号.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TRODUTY		_T("\t小车工作级别.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TROMACHINERY	_T("\t小车驱动型号.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_MACHINERYNUM	_T("\t小车电机数量.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_WEIGHT		_T("\t葫芦自重...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_OVERLOAD		_T("\t超载保护装置.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_ELECPROV		_T("\t电气种类...............................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_ELECIP		_T("\t电箱防护等级.......................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_MOTORIP		_T("\t电机防护等级.......................................................................%s\r\n")
/*	Main and Aux hoist data */
#define	STR_TECHDATA_HOIST_CN_TYPE_MAIN			_T("\t葫芦代码 (主钩)..................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_FRAMESIZE_MAIN	_T("\t机架代码 (主钩)..................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_REEVINGCODE_MAIN	_T("\t绕绳方式代码 (主钩)...........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_DRUMLENCODE_MAIN	_T("\t卷筒长度代码 (主钩)...........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_LOAD_MAIN			_T("\t额定载荷 (主钩)...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_HEIGHTOFLIT_MAIN	_T("\t起升高度 (主钩)...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_SPEED_MAIN		_T("\t起升速度 (主钩)...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_MOTORTYPE_MAIN	_T("\t起升电机种类和功率 (主钩)...............................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_DUTYGROUP_MAIN	_T("\t起升工作级别 (主钩)...........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_TYPE_AUX			_T("\t葫芦代码 (副钩)..................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_FRAMESIZE_AUX		_T("\t机架代码 (副钩)..................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_REEVINGCODE_AUX	_T("\t绕绳方式代码 (副钩)...........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_DRUMLENCODE_AUX	_T("\t卷筒长度代码 (副钩)...........................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_LOAD_AUX			_T("\t额定载荷 (副钩)...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_HEIGHTOFLIT_AUX	_T("\t起升高度 (副钩)...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_SPEED_AUX			_T("\t起升速度 (副钩)...................................................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_MOTORTYPE_AUX		_T("\t起升电机种类和功率 (副钩)...............................................%s\r\n")
#define	STR_TECHDATA_HOIST_CN_DUTYGROUP_AUX		_T("\t起升工作级别 (副钩)...........................................................%s\r\n")
//
#define	STR_TECHDATA_CRANE_CN_TYPE			_T("\t行车类型...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_CAPACITY		_T("\t最大起重量...........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_SPAN			_T("\t跨度.......................................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_LIFTHEIGHT	_T("\t起升高度...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_DUTYGROUP		_T("\t起重机工作级别...................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_WEIGHT		_T("\t起重机总重量.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_STEELWEIGHT	_T("\t钢结构重量...........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_TROWEIGHT		_T("\t小车重量...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_ENDCARWEIGHT	_T("\t端梁重量...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_RAILWHEELNUM	_T("\t单侧车轮数量.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_WHEELLOAD		_T("\t最大静态轮压.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_RAILTYPE		_T("\t轨道型号...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_MAINVOL		_T("\t起重机工作电压...................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_CTRLVOL		_T("\t控制电压...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_CURRENT		_T("\t启动 / 额定电流.................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_MAINUSE		_T("\t工况 / 环境温度.................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_HOISTTYPE		_T("\t葫芦类型...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_HOISTCAP		_T("\t最大起重量...........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_HOISTDUTY		_T("\t大车工作级别.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_HOISTLIFT		_T("\t起升高度...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_TROTYPE		_T("\t小车类型...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_REEVING		_T("\t绕绳方式...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_HOISTSPD		_T("\t起升速度...............................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_TRAVERSSPD	_T("\t小车运行速度.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_TRAVELSPD		_T("\t大车运行速度.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_HOISTMOTOR	_T("\t起升电机功率.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_TROMOTOR		_T("\t小车运行电机功率...............................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_CRANEMOTOR	_T("\t大车运行电机功率...............................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_TOTALPOW		_T("\t行车总功率...........................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_ELECIP		_T("\t电箱防护等级.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_MOTORIP		_T("\t电机防护等级.......................................................................%s\r\n")
#define	STR_TECHDATA_CRANE_CN_CTRLTYPE		_T("\t控制类型...............................................................................%s\r\n")
//
//----------------------------------------------------

//----------------------------------------------------
//	Name:eTableID
//	Introduce:Table ID defintion
//----------------------------------------------------
enum eTableID
{
	eTableID_None				= 0,
	eTableID_EmployeeInfo		= 1,
	eTableID_CustomerInfo		= 2,
	eTableID_ContactsInfo		= 3,
	eTableID_OffersInfo			= 4,
	eTableID_ComponentInfo		= 5,
	//
	eTableID_HoiSel_D			= 6,
	eTableID_HoiSel_S			= 7,
	eTableID_HoiSel_F			= 8,
	eTableID_DimHoi_D			= 9,
	eTableID_DimHoi_S			= 10,
	eTableID_DimHoi_F			= 11,
	eTableID_DimHoi_Z			= 12,
	eTableID_CraneData			= 13,
	eTableID_CalcParams			= 14,
	eTableID_Additions			= 15,
	eTableID_HoiMacIndCod		= 16,
	eTableID_TroDriTyp			= 17,
	eTableID_TroDriDat			= 18,
	eTableID_EndCarSteMat		= 19,
	eTableID_JoiPlaTyp			= 20,
	eTableID_EndCarMatPar		= 21,
	eTableID_WheWidDat			= 22,
	eTableID_EndCarWei			= 23,
	eTableID_EndCarPri			= 24,
	eTableID_DatRai				= 25,
	eTableID_RaiGauDat			= 26,
	eTableID_Buffer				= 27,
	eTableID_EndCarStePri		= 28,
	eTableID_CraPowPri			= 29,
	eTableID_CraPanPri1			= 30,
	eTableID_CraPanPri2			= 31,
	eTableID_FasPri				= 32,
	eTableID_PanPri				= 33,
	eTableID_WheLoaCal			= 34,
	eTableID_JoiPlaDat			= 35,
	eTableID_SHoiSel_S			= 36,
	eTableID_SHoiSel_D			= 37,
	eTableID_SHoiSel_F			= 38,
	eTableID_MainAuxH			= 39,
	eTableID_WheMot				= 40,
	eTableID_EndCarBm			= 41,
	eTableID_BufferPri			= 42,
	eTableID_Powlist			= 43,
	eTableID_FasPri1			= 44,
	eTableID_Cnt
};
//----------------------------------------------------
//	Name:eEmployeeGroup
//	Introduce:Employee groups defintion
//----------------------------------------------------
enum eEmployeeGroup
{
	eEmployeeGroup_Admin	= 0,
	eEmployeeGroup_Normal	= 1,
	eEmployeeGroup_Viewer	= 2,
	eEmployeeGroup_Cnt
};
//----------------------------------------------------
//	Name:eTableDataType
//	Introduce:Table data type defintion
//----------------------------------------------------
enum eTableDataType
{
	eTableDataType_UnKnown	= 0,
	eTableDataType_DWORD	= 1,
	eTableDataType_FLOAT	= 2,
	eTableDataType_Date		= 3,
	eTableDataType_String	= 4,
	eTableDataType_Cnt
};
//----------------------------------------------------
//	Name:eOfferStatus
//	Introduce:Offers status defintion
//----------------------------------------------------
enum eOfferStatus
{
	eOfferStatus_New		= 0,
	eOfferStatus_Process	= 1,
	eOfferStatus_Closed		= 2,
	eOfferStatus_Cnt
};
//----------------------------------------------------
//	Name:eCraneType
//	Introduce:Crane main type defintion
//----------------------------------------------------
enum eCraneType
{
	eCraneType_GTD = 0,
	eCraneType_GTS = 1,
	eCraneType_GTZ = 2,
	eCraneType_GTU = 3,
	eCraneType_Cnt
};
//----------------------------------------------------
//	Name:eHoistNumType
//	Introduce:Number of hoist type defintion
//----------------------------------------------------
enum eHoistNumType
{
	eHoistNumType_One			= 0,
	eHoistNumType_MainAux		= 1,
	eHoistNumType_TwoSimilar	= 2,
	eHoistNumType_Cnt
};
//----------------------------------------------------
//	Name:eCraneCtrlType
//	Introduce:Crane control type defintion
//----------------------------------------------------
enum eCraneCtrlType
{
	eCraneCtrlType_Pendent		= 0,
	eCraneCtrlType_Remote		= 1,
	eCraneCtrlType_BlueTooth	= 2,
	eCraneCtrlType_Cabin		= 3,
	eCraneCtrlType_Cnt
};
//----------------------------------------------------
//	Name:eCraneMainUseType
//	Introduce:Crane main use type defintion
//----------------------------------------------------
enum eCraneMainUseType
{
	eCraneMainUseType_Indoors	= 0,
	eCraneMainUseType_Outdoors	= 1,
	eCraneMainUseType_Cnt
};
//----------------------------------------------------
//	Name:eElecCtrlType
//	Introduce:Elec control type defintion
//----------------------------------------------------
enum eElecCtrlType
{
	eElecCtrlType_2Speeds	= 0,
	eElecCtrlType_Inverter	= 1,
	eElecCtrlType_Cnt
};
//----------------------------------------------------
//	Name:eReevingType
//	Introduce:Reeving type defintion
//----------------------------------------------------
enum eReevingType
{
	eReevingType_All	= 0,
	eReevingType_True	= 1,
	eReevingType_Center	= 2,
	eReevingType_Cnt
};
//----------------------------------------------------
//	Name:eHoistDrawType
//	Introduce:Hoist layout drawing type defintion
//----------------------------------------------------
enum eHoistDrawType
{
	eHoistDrawType_S = 0,
	eHoistDrawType_D0,
	eHoistDrawType_D1,
	eHoistDrawType_D2,
	eHoistDrawType_D3,
	eHoistDrawType_D4,
	eHoistDrawType_F0,
	eHoistDrawType_F1,
	eHoistDrawType_F3,
	eHoistDrawType_F4,
	eHoistDrawType_Cnt
};
//----------------------------------------------------
//	Name:eAdditionType
//	Introduce:Addtitions type defintion
//----------------------------------------------------
enum eAdditionType
{
	eAdditionType_Hoist		= 0,
	eAdditionType_EndCar	= 1,
	eAdditionType_Elec		= 2,
	eAdditionType_Cnt
};
//----------------------------------------------------
//	Name:eLayoutViewType
//	Introduce:Layout view type defintion
//----------------------------------------------------
enum eLayoutViewType
{
	eLayoutViewType_Main	= 0,
	eLayoutViewType_Side	= 1,
	eLayoutViewType_Top		= 2,
	eLayoutViewType_Cnt
};
//----------------------------------------------------
//	Name:eCalcResultType
//	Introduce:Calc result type defintion
//----------------------------------------------------
enum eCalcResultType
{
	eCalcResultType_Pass	= 0,
	eCalcResultType_NotPass = 1,
	eCalcResultType_Cnt
};
//----------------------------------------------------
//	Name:eComponentType
//	Introduce:Crane component type defintion
//----------------------------------------------------
enum eComponentType
{
	eComponentType_Hoist	= 0,
	eComponentType_EndCar,
	eComponentType_BridgeDri,
	eComponentType_BridgePannel,
	eComponentType_Fastoon,
	eComponentType_OperateUnit,
	eComponentType_CranePower,
	eComponentType_MainGirder,
	eComponentType_Cnt,
};
//----------------------------------------------------
enum eCurrencyType
{
	eCurrencyType_RMB	= 0,
	eCurrencyType_EUR	= 1,
	eCurrencyType_USD	= 2,
	eCurrencyType_Cnt
};
//----------------------------------------------------


//----------------------------------------------------
//	Name:TableColumn
//	Introduce:Table column struct defintion
//----------------------------------------------------
typedef struct tagTableColumn
{
	tagTableColumn()
	{
		bIsIndex = FALSE;
		bIsVisible = TRUE;
		lColumnWidth = 100;
		strName = L"";
		nTextID = 0;
		eDataType = eTableDataType_String;
	};
	//------------------------------------------------
	tagTableColumn(BOOL bIsAudtoIndex, LONG lWidth, CStringW strColumnName, UINT nID, eTableDataType eType, BOOL bVisible = TRUE)
	{
		bIsIndex = bIsAudtoIndex;
		bIsVisible = bVisible;
		lColumnWidth = lWidth;
		strName = strColumnName;
		nTextID = nID;
		eDataType = eType;
	};
	//------------------------------------------------
	tagTableColumn(BOOL bIsAudtoIndex, LONG lWidth, CStringW strColumnName, CStringW strText, eTableDataType eType, BOOL bVisible = TRUE)
	{
		bIsIndex = bIsAudtoIndex;
		bIsVisible = bVisible;
		lColumnWidth = lWidth;
		strName = strColumnName;
		strDispalyText = strText;
		nTextID = 0;
		eDataType = eType;
	};
	//------------------------------------------------
	tagTableColumn & operator=(tagTableColumn src)
	{
		bIsIndex = src.bIsIndex;
		bIsVisible = src.bIsVisible;
		lColumnWidth = src.lColumnWidth;
		strName = src.strName;
		nTextID = src.nTextID;
		eDataType = src.eDataType;
	}
	//------------------------------------------------
	BOOL				bIsIndex;
	BOOL				bIsVisible;
	CStringW			strName;
	CStringW			strDispalyText;
	UINT				nTextID;
	LONG				lColumnWidth;
	eTableDataType		eDataType;
}TableColumn, *LPTABLECOLUMN;
//----------------------------------------------------
typedef list<TableColumn> TableColumnsList;
//----------------------------------------------------
//	Name:TableData
//	Introduce:A database table struct defintion
//----------------------------------------------------
typedef struct tagTableData
{
	tagTableData()
	{
		eID = eTableID_None;
		strName = L"";
	}
	//------------------------------------------------
	eTableID			eID;
	CStringW			strName;
	TableColumnsList	lstColumns;	
}TableData, *LPTABLEDATA;
//----------------------------------------------------
//	Name:TechData
//	Introduce:Techinology data struct definition
//----------------------------------------------------
struct TechData
{
public:
	TechData()
	{
	}
	TechData(CString name, CString printEN, CString printCN, CString valueEN, CString valueCN = _T("\0"))
	{
		strName = name;
		strValue = valueEN;
		//
		if (valueCN.IsEmpty()) valueCN = valueEN;
		strPrintText_EN.Format(printEN, valueEN);
		strPrintText_CN.Format(printCN, valueCN);
	}
	TechData & operator=(TechData src)
	{
		strName = src.strName;
		strValue =  src.strValue;
		strPrintText_EN = src.strPrintText_EN;
		strPrintText_CN = src.strPrintText_CN;
	}
	CString strName;
	CString strValue;
	CString strPrintText_EN;
	CString strPrintText_CN;
};
//-----------------------------------------------------------------------
typedef std::list<TechData>		TechDataList;
//-----------------------------------------------------------------------
typedef struct tagDetailPriceInfo
{
	tagDetailPriceInfo()
	{
		eType = eComponentType_Hoist;
		strCode = _T("\0");
		iCount = 1;
		iTotalPrice = 0;
	};
	//------------------------------------------------
	tagDetailPriceInfo(eComponentType type, CString code, int count, int totalPrice)
	{
		eType = type;
		strCode = code;
		iCount = count;
		iTotalPrice = totalPrice;
	};
	//------------------------------------------------
	tagDetailPriceInfo & operator=(tagDetailPriceInfo src)
	{
		eType = src.eType;
		strCode = src.strCode;
		iCount = src.iCount;
		iTotalPrice = src.iTotalPrice;
	}
	//------------------------------------------------
	eComponentType	eType;
	CString			strCode;
	int				iCount;
	int				iTotalPrice;
}DetailPriceInfo, *LPDETAILPRICEINFO;
//----------------------------------------------------
typedef list<DetailPriceInfo>	DetailPriceList;
//----------------------------------------------------
typedef struct tagComPriceInfo
{	
	tagComPriceInfo()
	{
		strComName = _T("");
		iComCount = 0;
		iOfferPrice = 0;
		pCalculator = NULL;
	};
	tagComPriceInfo(CString strName, int iCount, int iPrice, LPVOID lpCalculator, DetailPriceList lstDetail)
	{
		strComName = strName;
		iComCount = iCount;
		iOfferPrice = iPrice;
		pCalculator = lpCalculator;
		for (DetailPriceList::iterator it = lstDetail.begin(); it != lstDetail.end(); it++)
		{
			DetailPriceInfo detail = *it;
			lstDetails.push_back(detail);
		}
	};
	//------------------------------------------------
	tagComPriceInfo & operator=(tagComPriceInfo src)
	{
		strComName = src.strComName;
		iComCount = src.iComCount;
		iOfferPrice = src.iOfferPrice;
		pCalculator = src.pCalculator;
		for (DetailPriceList::iterator it = src.lstDetails.begin(); it != src.lstDetails.end(); it++)
		{
			DetailPriceInfo detail = *it;
			lstDetails.push_back(detail);
		}
	}
	//------------------------------------------------
	CString	strComName;
	int		iComCount;
	int		iOfferPrice;
	LPVOID	pCalculator;
	DetailPriceList	lstDetails;
}ComPriceInfo, *LPCOMPRICEINFO;
//----------------------------------------------------
typedef list<ComPriceInfo>	ComponentPriceList;
//----------------------------------------------------


//----------------------------------------------------
extern LoginUserInfo g_LoginUserInfo;
extern CADODataBase* g_pSettingDatabase;
extern CADODataBase* g_pOfferDatabase;
void InitDataListCtrl(CListCtrl* pListCtrl, TableData &td);
void ShowTableRecord(CADOTable* pTable, int iRecordIndex, CListCtrl* pListCtrl, TableData &td);
void ShowTableRecords(CADOTable* pTable, CListCtrl* pListCtrl, TableData &td);
void SelectComboBoxItem(CComboBox* pComboBox, int iItemData);
void SelectComboBoxItem(CComboBox* pComboBox, CString strItemText);
CString GetOfferStatusText(eOfferStatus eStatus);
CString GetStringValueFromTable(CString strSQL, CString strName, CADODataBase* pDatabase = NULL);
LONG GetLONGValueFromTable(CString strSQL, CString strName, CADODataBase* pDatabase = NULL);
float GetFLOATValueFromTable(CString strSQL, CString strName, CADODataBase* pDatabase = NULL);
void GetValueListInTable(CString strTableName, CString strColumnName, eTableDataType eType, list<CString> &lstValueString);
long GetNearBigValue(CString strTableName, CString strColumnName, long lBaseValue);
float GetNearBigValue(CString strTableName, CString strColumnName, float fBaseValue);
CString GetHoistSelTableName(CString strType, CString strBrand);
//----------------------------------------------------
CString	GetOfferOrderTemplateFile(CString strSubFolder, CString strCompany, CString strLang, int iComCnt);
CString	GetMainGirderTemplateFile(CString strSubFolder, CString strCompany, CString strLang, CString strMainAux, CString strECType, int iMoQty, CString strCtrlType);
CString	GetEndCarTemplateFile(CString strSubFolder, CString strCompany, CString strLang, CString strECType, CString strDriverType, int iMoQty);
CString	GetSteelStructureTemplateFile(CString strSubFolder, CString strCompany, CString strLang, CString strMainGirderType, CString strJointType, int iMB81);
BOOL	ExportMainGirderDataToWordTemplate(CMSWordApplication* pWordApp, LPVOID pPointer, int iLangIndex);
BOOL	ExportEndCarriageDataToWordTemplate(CMSWordApplication* pWordApp, LPVOID pPointer);
BOOL	ExportSteelStructureDataToWordTemplate(CMSWordApplication* pWordApp, LPVOID pPointer);
//
CString	ValueToString(CString value);
CString	ValueToString(long value);
CString	ValueToString(float value);
CString	ValueToString(LONGLONG value);
CString	GetMainGirderCalcDataTemplateFile(CString strLang, CString strMainGirderType);
CString	GetEndCarriageCalcDataTemplateFile(CString strLang, CString strECType);
BOOL	ExportMainGirderCalcDataForTest(CMSWordApplication* pWordApp, LPVOID pPointer);
BOOL	ExportEndCarriageCalcDataForTest(CMSWordApplication* pWordApp, LPVOID pPointer);
//----------------------------------------------------


//----------------------------------------------------
//	Name:g_tcTableName
//	Introduce:All table name definition
//----------------------------------------------------
static TCHAR g_tcTableName[eTableID_Cnt][64] =  {	_T("\0"),
													_T("EmployeeInfo\0"),	//eTableID_EmployeeInfo
													_T("CompanyInfo\0"),	//eTableID_CustomerInfo
													_T("ContactInfo\0"),	//eTableID_ContactsInfo
													_T("OffersInfo\0"),		//eTableID_OffersInfo
													_T("ComponentInfo\0"),	//eTableID_ComponentInfo
													_T("HoiSel_D\0"),		//eTableID_HoiSel_D
													_T("HoiSel_S\0"),		//eTableID_HoiSel_S
													_T("HoiSel_F\0"),		//eTableID_HoiSel_F
													_T("DimHoi_D\0"),		//eTableID_DimHoi_D
													_T("DimHoi_S\0"),		//eTableID_DimHoi_S
													_T("DimHoi_F\0"),		//eTableID_DimHoi_F													
													_T("DimHoi_Z\0"),		//eTableID_DimHoi_Z
													_T("CraDat\0"),			//eTableID_CraneData
													_T("CalcParams"),		//eTableID_CalcParams
													_T("AdditionSel"),		//eTableID_Additions
													_T("HoiMacIndCod"),		//eTableID_HoiMacIndCod
													_T("TroDriTyp"),		//eTableID_TroDriTyp
													_T("TroDriDat"),		//eTableID_TroDriDat
													_T("EndCarSteMat"),		//eTableID_EndCarSteMat
													_T("JoiPlaTyp"),		//eTableID_JoiPlaTyp 
													_T("EndCarMatPar"),		//eTableID_EndCarMatPar
													_T("WheWidDat"),		//eTableID_WheWidDat
													_T("EndCarWei"),		//eTableID_EndCarWei
													_T("EndCarPri"),		//eTableID_EndCarPri
													_T("DatRai"),			//eTableID_DatRai
													_T("RaiGauDat"),		//eTableID_RaiGauDat
													_T("Buffer"),			//eTableID_Buffer
													_T("EndCarStePri"),		//eTableID_EndCarStePri
													_T("CraPowPri"),		//eTableID_CraPowPri
													_T("CraPanPri1"),		//eTableID_CraPanPri1	
													_T("CraPanPri2"),		//eTableID_CraPanPri2	
													_T("FasPri"),			//eTableID_FasPri		
													_T("PanPri"),			//eTableID_PanPri
													_T("WheLoaCal"),		//eTableID_WheLoaCal
													_T("JoiPlaDat"),		//eTableID_JoiPlaDat
													_T("SHoiSel_S"),		//eTableID_SHoiSel_S
													_T("SHoiSel_D"),		//eTableID_SHoiSel_D
													_T("SHoiSel_F"),		//eTableID_SHoiSel_F
													_T("MainAuxH"),			//eTableID_MainAuxH
													_T("WheMot"),			//eTableID_WheMot
													_T("EndCarBm"),			//eTableID_EndCarBm
													_T("BufferPri"),		//eTableID_BufferPri
													_T("PowList"),			//eTableID_Powlist
													_T("FasPri1"),			//eTableID_FasPri1;
													};	
//----------------------------------------------------
static TCHAR g_tcTabIndex[MAX_COMPONENT_CNT][8] = {	_T("①"), _T("②"), _T("③"), _T("④"), _T("⑤"),
													_T("⑥"), _T("⑦"), _T("⑧"), _T("⑨"), _T("⑩")};
//----------------------------------------------------
static int g_iRailGauage[] = {0, 1200, 1400, 1700, 2000, 2400, 2700, 3100, 3400, 3800, 4200};
//----------------------------------------------------
#endif	//_COMMON_DEFINE_H_
//----------------------------------------------------
