#include "xBaseLexer.h"
#include <assert.h>


 bool  XEvol_IsReadalbeChar(int ch)
{
	if(XEvol_IsAlphaNumChar(ch))
		return true;
	return false;
}



