
#include "TypeDefs.h"

#include "windows.h"
#undef ExitWindows
#include "comdef.h"

#include "CPSAtlBase.h"

#pragma warning ( disable : 4278 )
#pragma warning ( disable : 4192 )

#include "mso.tlh"
#include "vbe6ext.tlh"
#include "msword.tlh"

void DoWordProcess(IDispatchPtr p, CFileListStrings& embed_file_list, bool NoLineBreak)
{
	Word::_DocumentPtr pDoc=p;

	Word::InlineShapesPtr pShapes=pDoc->GetInlineShapes();
	
	float yPos=0;
	float xPos=0;
	_variant_t LinkToFile(false);
	_variant_t SaveWithDocument(true);

	for(CFileListStrings::iterator it=embed_file_list.begin();it!=embed_file_list.end();it++)
	{
		Word::RangePtr pDocRange=pDoc->Range();
		pDocRange->Collapse(&_variant_t(Word::wdCollapseEnd));

		_variant_t varRange((IDispatch*)IDispatchPtr(pDocRange)); // due to compiler bug

		pShapes->AddPicture(_bstr_t(it->c_str()),&LinkToFile,&SaveWithDocument,&varRange);
		try
		{
			//MM: Exception happened but it works
			Word::RangePtr pDocRange=pDoc->Range();
			pDocRange->Collapse(&_variant_t(Word::wdCollapseEnd));
//			bstr_t name = bstr_t("\n") + it->c_str() + ("\n\n");
			if (!NoLineBreak)
					pDocRange->InsertAfter("\n\n");
		}
		catch(...)
		{
		}

	}
}
