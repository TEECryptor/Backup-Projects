//-------------------------------------------------
//	Name:RussiaClientDef.h
//	Introduce:The common definition for Russlia Client
//	Creator:Youfang Yao
//	Date:12-13-06
//-------------------------------------------------
#ifndef _RUSSIACLIENTDEF_
#define _RUSSIACLIENTDEF_
//-------------------------------------------------
#include "CommonDef.h"
//-------------------------------------------------
/*	Result flag objcet count definition*/
//-------------------------------------------------
#define RESULTFLG_CNT		70
//-------------------------------------------------
/*	Ball images count definition*/
//-------------------------------------------------
#define BALLIMAGE_CNT		10
//-------------------------------------------------
/*	Limit number object definition*/
//-------------------------------------------------
#define	LIMITNUM_CNT		10
//#define	LIMITNUM_WIDTH		10
//#define	LIMITNUM_HEIGHT		16
//-------------------------------------------------
/*	Total total bet value number object definition*/
//-------------------------------------------------
#define TOTALBETNUM_CNT		10
//#define	TOTALBETNUM_WIDTH	10
//#define	TOTALBETNUM_HEIGHT	16
//-------------------------------------------------
/*	Timer number object definition*/
//-------------------------------------------------
#define TIMENUM_CNT			2
#define	TIMENUM_WIDTH		20
#define	TIMENUM_HEIGHT		30
//-------------------------------------------------
/*	Total current bet value number object definition*/
//-------------------------------------------------
#define BETVALNUM_CNT		6
#define	BETVALNUM_WIDTH		16
#define	BETVALNUM_HEIGHT	24
//-------------------------------------------------
#define CHIPOBJ_CNT			2
#define	CHIPOBJ_WIDTH		80
#define	CHIPOBJ_HEIGHT		80
//-------------------------------------------------
/*	Total current user value number object definition*/
//-------------------------------------------------
#define USERVALNUM_CNT		6
#define	USERVALNUM_WIDTH	16
#define	USERVALNUM_HEIGHT	24
//-------------------------------------------------
/*	Client computer ID number object definition*/
//-------------------------------------------------
#define CLIENTIDNUM_CNT		3
#define	CLIENTIDNUM_WIDTH	16
#define	CLIENTIDNUM_HEIGHT	24
//-------------------------------------------------


//-------------------------------------------------
//	Name:UI_Stype
//	Introduce:The client UI stype definition
//-------------------------------------------------
typedef enum _UI_STYPE
{
	UIS_H		 = 0,
	UIS_V		 = 1,
	UIS_COUNT
}UI_Stype;
//-------------------------------------------------
//	Name:Client_State
//	Introduce:The client states definition
//-------------------------------------------------
typedef enum _CLIENT_STATE
{
	CS_NONE			= 0,
	CS_LOADED		= 1,
	CS_CONNECTED	= 2,
	CS_ACTIVED		= 3,
	CS_BETTING		= 4,
	CS_SUBMITED		= 5,
	CS_BETTED		= 6,
	CS_TURNING		= 7,
	CS_SHOWRESULT	= 8,
	CS_GAMEEND		= 9,
	CS_GAMEOVER		= 10,
	CS_COUNT
}Client_State;
//-------------------------------------------------
//	Name:Color_Type
//	Introduce:The color type definition
//-------------------------------------------------
typedef enum _COLOR_TYPE
{
	CT_RED		= 0,
	CT_BLACK	= 1,
	CT_GREEN	= 2,
	CT_COUNT
}Color_Type;
//-------------------------------------------------
//	Name:Bet_Type
//	Introduce:Bet value type definition
//-------------------------------------------------
typedef enum _BET_TYPE
{
	BT_RED_MIN	= 0,
	BT_RED_MAX	= 1,
	BT_BLACK_MIN= 2,
	BT_BLACK_MAX= 3,
	BT_GREEN_MIN= 4,
	BT_GREEN_MAX= 5,
	BT_COUNT
}Bet_Type;
//-------------------------------------------------
//	Name:CurValue_Type
//	Introduce:User current value type definition
//-------------------------------------------------
typedef enum _CURVALUE_TYPE
{
	CVT_REMAIN	= 0,
	CVT_WINVAL	= 1,
	CVT_COUNT
}CurValue_Type;
//-------------------------------------------------
//	Name:Timer_Type
//	Introduce:Timer type definition
//-------------------------------------------------
typedef enum _TIMER_TYPE
{
	TT_BETTIMER		= 0,
	TT_RENDERBET	= 1,
	TT_RENDERRES	= 2,
	TT_COUNT
}Timer_Type;
//-------------------------------------------------
//	Name:ChipVal_Type
//	Introduce:Chip value types definition
//-------------------------------------------------
typedef enum _CHIPVAL_TYPE
{
	CT_VAL5		= 0,
	CT_VAL10	= 1,
	CT_VAL20	= 2,
	CT_VAL50	= 3,
	CT_VAL100	= 4,
	CT_VAL200	= 5,
	CT_VAL500	= 6,
	CT_VAL1000	= 7,
	CT_VAL2000	= 8,
	CT_VALCOUNT
}ChipVal_Type;
//-------------------------------------------------
//	Name:ChipObj_Type
//	Introduce:Chip object types definition
//-------------------------------------------------
typedef enum _CHIPOBJ_TYPE
{
	CT_OBJMIN	= 0,
	CT_OBJMAX	= 1,
	CT_OBJCOUNT
}ChipObj_Type;
//-------------------------------------------------


//-------------------------------------------------
//	Name:BetUndo
//	Introduce:Bet undo operation definition
//-------------------------------------------------
typedef struct tagBetUndo
{
	UINT	uBetColor;
	float	fBetValue;
}BetUndo, *LPBETUNDO;
//-------------------------------------------------

//-------------------------------------------------
#endif	// _RUSSIACLIENTDEF_
//-------------------------------------------------

