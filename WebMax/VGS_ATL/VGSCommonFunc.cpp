
#include "stdafx.h"
#include "VGSCommonFunc.h"

CONTROLTYPE _getControlStatus(INT status)
{
	//0--WALKģʽ, 1--Editģʽ, 2--FLYģʽ, 3ΪNoneģʽ
	switch (status)
	{
	case 0: 
		return CT_WALKER;
	case 1:
		return CT_EDITOR;
	case 2:
		return CT_FLYER;
	case 3:
	default:
		return CT_NONE;
	}
}