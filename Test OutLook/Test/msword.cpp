// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "msword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _Document properties

/////////////////////////////////////////////////////////////////////////////
// _Document operations

CString _Document::GetName()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Document::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetBuiltInDocumentProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetCustomDocumentProperties()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Document::GetPath()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetBookmarks()
{
	LPDISPATCH result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetTables()
{
	LPDISPATCH result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetFootnotes()
{
	LPDISPATCH result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetEndnotes()
{
	LPDISPATCH result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetComments()
{
	LPDISPATCH result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Document::GetType()
{
	long result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetAutoHyphenation()
{
	BOOL result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetAutoHyphenation(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetHyphenateCaps()
{
	BOOL result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetHyphenateCaps(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Document::GetHyphenationZone()
{
	long result;
	InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetHyphenationZone(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Document::GetConsecutiveHyphensLimit()
{
	long result;
	InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetConsecutiveHyphensLimit(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Document::GetSections()
{
	LPDISPATCH result;
	InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetParagraphs()
{
	LPDISPATCH result;
	InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetWords()
{
	LPDISPATCH result;
	InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetSentences()
{
	LPDISPATCH result;
	InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetCharacters()
{
	LPDISPATCH result;
	InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetFields()
{
	LPDISPATCH result;
	InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetFormFields()
{
	LPDISPATCH result;
	InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetStyles()
{
	LPDISPATCH result;
	InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetFrames()
{
	LPDISPATCH result;
	InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetTablesOfFigures()
{
	LPDISPATCH result;
	InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetVariables()
{
	LPDISPATCH result;
	InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetMailMerge()
{
	LPDISPATCH result;
	InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetEnvelope()
{
	LPDISPATCH result;
	InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Document::GetFullName()
{
	CString result;
	InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetRevisions()
{
	LPDISPATCH result;
	InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetTablesOfContents()
{
	LPDISPATCH result;
	InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetTablesOfAuthorities()
{
	LPDISPATCH result;
	InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetPageSetup()
{
	LPDISPATCH result;
	InvokeHelper(0x44d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Document::SetPageSetup(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x44d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Document::GetWindows()
{
	LPDISPATCH result;
	InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetHasRoutingSlip()
{
	BOOL result;
	InvokeHelper(0x23, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetHasRoutingSlip(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x23, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Document::GetRoutingSlip()
{
	LPDISPATCH result;
	InvokeHelper(0x24, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetRouted()
{
	BOOL result;
	InvokeHelper(0x25, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetTablesOfAuthoritiesCategories()
{
	LPDISPATCH result;
	InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetIndexes()
{
	LPDISPATCH result;
	InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetSaved()
{
	BOOL result;
	InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetSaved(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x28, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Document::GetContent()
{
	LPDISPATCH result;
	InvokeHelper(0x29, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetActiveWindow()
{
	LPDISPATCH result;
	InvokeHelper(0x2a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Document::GetKind()
{
	long result;
	InvokeHelper(0x2b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetKind(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x2b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _Document::GetReadOnly()
{
	BOOL result;
	InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetSubdocuments()
{
	LPDISPATCH result;
	InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetIsMasterDocument()
{
	BOOL result;
	InvokeHelper(0x2e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

float _Document::GetDefaultTabStop()
{
	float result;
	InvokeHelper(0x30, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Document::SetDefaultTabStop(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x30, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _Document::GetEmbedTrueTypeFonts()
{
	BOOL result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetEmbedTrueTypeFonts(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetSaveFormsData()
{
	BOOL result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetSaveFormsData(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x33, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetReadOnlyRecommended()
{
	BOOL result;
	InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetReadOnlyRecommended(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x34, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetSaveSubsetFonts()
{
	BOOL result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetSaveSubsetFonts(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x35, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetCompatibility(long Type)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms,
		Type);
	return result;
}

void _Document::SetCompatibility(long Type, BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL;
	InvokeHelper(0x37, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Type, bNewValue);
}

LPDISPATCH _Document::GetStoryRanges()
{
	LPDISPATCH result;
	InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetCommandBars()
{
	LPDISPATCH result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetIsSubdocument()
{
	BOOL result;
	InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long _Document::GetSaveFormat()
{
	long result;
	InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Document::GetProtectionType()
{
	long result;
	InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetHyperlinks()
{
	LPDISPATCH result;
	InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetShapes()
{
	LPDISPATCH result;
	InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetListTemplates()
{
	LPDISPATCH result;
	InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetLists()
{
	LPDISPATCH result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetUpdateStylesOnOpen()
{
	BOOL result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetUpdateStylesOnOpen(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

VARIANT _Document::GetAttachedTemplate()
{
	VARIANT result;
	InvokeHelper(0x43, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _Document::SetAttachedTemplate(VARIANT* newValue)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x43, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Document::GetInlineShapes()
{
	LPDISPATCH result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetBackground()
{
	LPDISPATCH result;
	InvokeHelper(0x45, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Document::SetBackground(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x45, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

BOOL _Document::GetGrammarChecked()
{
	BOOL result;
	InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetGrammarChecked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetSpellingChecked()
{
	BOOL result;
	InvokeHelper(0x47, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetSpellingChecked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x47, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetShowGrammaticalErrors()
{
	BOOL result;
	InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetShowGrammaticalErrors(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetShowSpellingErrors()
{
	BOOL result;
	InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetShowSpellingErrors(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Document::GetVersions()
{
	LPDISPATCH result;
	InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetShowSummary()
{
	BOOL result;
	InvokeHelper(0x4c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetShowSummary(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Document::GetSummaryViewMode()
{
	long result;
	InvokeHelper(0x4d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetSummaryViewMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Document::GetSummaryLength()
{
	long result;
	InvokeHelper(0x4e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetSummaryLength(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _Document::GetPrintFractionalWidths()
{
	BOOL result;
	InvokeHelper(0x4f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetPrintFractionalWidths(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x4f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetPrintPostScriptOverText()
{
	BOOL result;
	InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetPrintPostScriptOverText(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x50, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Document::GetContainer()
{
	LPDISPATCH result;
	InvokeHelper(0x52, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetPrintFormsData()
{
	BOOL result;
	InvokeHelper(0x53, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetPrintFormsData(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x53, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Document::GetListParagraphs()
{
	LPDISPATCH result;
	InvokeHelper(0x54, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Document::SetPassword(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x55, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

void _Document::SetWritePassword(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x56, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Document::GetHasPassword()
{
	BOOL result;
	InvokeHelper(0x57, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetWriteReserved()
{
	BOOL result;
	InvokeHelper(0x58, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _Document::GetActiveWritingStyle(VARIANT* LanguageID)
{
	CString result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x5a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		LanguageID);
	return result;
}

void _Document::SetActiveWritingStyle(VARIANT* LanguageID, LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_BSTR;
	InvokeHelper(0x5a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 LanguageID, lpszNewValue);
}

BOOL _Document::GetUserControl()
{
	BOOL result;
	InvokeHelper(0x5c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetUserControl(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetHasMailer()
{
	BOOL result;
	InvokeHelper(0x5d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetHasMailer(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Document::GetMailer()
{
	LPDISPATCH result;
	InvokeHelper(0x5e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetReadabilityStatistics()
{
	LPDISPATCH result;
	InvokeHelper(0x60, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetGrammaticalErrors()
{
	LPDISPATCH result;
	InvokeHelper(0x61, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetSpellingErrors()
{
	LPDISPATCH result;
	InvokeHelper(0x62, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetVBProject()
{
	LPDISPATCH result;
	InvokeHelper(0x63, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetFormsDesign()
{
	BOOL result;
	InvokeHelper(0x64, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _Document::Get_CodeName()
{
	CString result;
	InvokeHelper(0x80010000, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Document::Set_CodeName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x80010000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Document::GetCodeName()
{
	CString result;
	InvokeHelper(0x106, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetSnapToGrid()
{
	BOOL result;
	InvokeHelper(0x12c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetSnapToGrid(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetSnapToShapes()
{
	BOOL result;
	InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetSnapToShapes(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x12d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

float _Document::GetGridDistanceHorizontal()
{
	float result;
	InvokeHelper(0x12e, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Document::SetGridDistanceHorizontal(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x12e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Document::GetGridDistanceVertical()
{
	float result;
	InvokeHelper(0x12f, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Document::SetGridDistanceVertical(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x12f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Document::GetGridOriginHorizontal()
{
	float result;
	InvokeHelper(0x130, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Document::SetGridOriginHorizontal(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x130, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float _Document::GetGridOriginVertical()
{
	float result;
	InvokeHelper(0x131, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void _Document::SetGridOriginVertical(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x131, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long _Document::GetGridSpaceBetweenHorizontalLines()
{
	long result;
	InvokeHelper(0x132, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetGridSpaceBetweenHorizontalLines(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x132, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Document::GetGridSpaceBetweenVerticalLines()
{
	long result;
	InvokeHelper(0x133, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetGridSpaceBetweenVerticalLines(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x133, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _Document::GetGridOriginFromMargin()
{
	BOOL result;
	InvokeHelper(0x134, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetGridOriginFromMargin(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x134, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetKerningByAlgorithm()
{
	BOOL result;
	InvokeHelper(0x135, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetKerningByAlgorithm(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x135, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Document::GetJustificationMode()
{
	long result;
	InvokeHelper(0x136, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetJustificationMode(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x136, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Document::GetFarEastLineBreakLevel()
{
	long result;
	InvokeHelper(0x137, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetFarEastLineBreakLevel(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x137, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString _Document::GetNoLineBreakBefore()
{
	CString result;
	InvokeHelper(0x138, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Document::SetNoLineBreakBefore(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x138, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString _Document::GetNoLineBreakAfter()
{
	CString result;
	InvokeHelper(0x139, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Document::SetNoLineBreakAfter(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x139, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL _Document::GetTrackRevisions()
{
	BOOL result;
	InvokeHelper(0x13a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetTrackRevisions(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x13a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetPrintRevisions()
{
	BOOL result;
	InvokeHelper(0x13b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetPrintRevisions(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x13b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetShowRevisions()
{
	BOOL result;
	InvokeHelper(0x13c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetShowRevisions(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x13c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

void _Document::Close(VARIANT* SaveChanges, VARIANT* OriginalFormat, VARIANT* RouteDocument)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x451, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveChanges, OriginalFormat, RouteDocument);
}

void _Document::Repaginate()
{
	InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::FitToPages()
{
	InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ManualHyphenation()
{
	InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::Select()
{
	InvokeHelper(0xffff, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::DataForm()
{
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::Route()
{
	InvokeHelper(0x6b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::Save()
{
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::SendMail()
{
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Document::Range(VARIANT* Start, VARIANT* End)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x7d0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Start, End);
	return result;
}

void _Document::RunAutoMacro(long Which)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x70, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Which);
}

void _Document::Activate()
{
	InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::PrintPreview()
{
	InvokeHelper(0x72, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Document::GoTo(VARIANT* What, VARIANT* Which, VARIANT* Count, VARIANT* Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x73, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		What, Which, Count, Name);
	return result;
}

BOOL _Document::Undo(VARIANT* Times)
{
	BOOL result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x74, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Times);
	return result;
}

BOOL _Document::Redo(VARIANT* Times)
{
	BOOL result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x75, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Times);
	return result;
}

long _Document::ComputeStatistics(long Statistic, VARIANT* IncludeFootnotesAndEndnotes)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x76, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Statistic, IncludeFootnotesAndEndnotes);
	return result;
}

void _Document::MakeCompatibilityDefault()
{
	InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::Protect(long Type, VARIANT* NoReset, VARIANT* Password)
{
	static BYTE parms[] =
		VTS_I4 VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x78, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type, NoReset, Password);
}

void _Document::Unprotect(VARIANT* Password)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x79, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Password);
}

void _Document::EditionOptions(long Type, long Option, LPCTSTR Name, VARIANT* Format)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x7a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type, Option, Name, Format);
}

void _Document::RunLetterWizard(VARIANT* LetterContent, VARIANT* WizardMode)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x7b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 LetterContent, WizardMode);
}

LPDISPATCH _Document::GetLetterContent()
{
	LPDISPATCH result;
	InvokeHelper(0x7c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Document::SetLetterContent(VARIANT* LetterContent)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x7d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 LetterContent);
}

void _Document::CopyStylesFromTemplate(LPCTSTR Template)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Template);
}

void _Document::UpdateStyles()
{
	InvokeHelper(0x7f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::CheckGrammar()
{
	InvokeHelper(0x83, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::CheckSpelling(VARIANT* CustomDictionary, VARIANT* IgnoreUppercase, VARIANT* AlwaysSuggest, VARIANT* CustomDictionary2, VARIANT* CustomDictionary3, VARIANT* CustomDictionary4, VARIANT* CustomDictionary5, VARIANT* CustomDictionary6, 
		VARIANT* CustomDictionary7, VARIANT* CustomDictionary8, VARIANT* CustomDictionary9, VARIANT* CustomDictionary10)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x84, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CustomDictionary, IgnoreUppercase, AlwaysSuggest, CustomDictionary2, CustomDictionary3, CustomDictionary4, CustomDictionary5, CustomDictionary6, CustomDictionary7, CustomDictionary8, CustomDictionary9, CustomDictionary10);
}

void _Document::FollowHyperlink(VARIANT* Address, VARIANT* SubAddress, VARIANT* NewWindow, VARIANT* AddHistory, VARIANT* ExtraInfo, VARIANT* Method, VARIANT* HeaderInfo)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x87, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Address, SubAddress, NewWindow, AddHistory, ExtraInfo, Method, HeaderInfo);
}

void _Document::AddToFavorites()
{
	InvokeHelper(0x88, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::Reload()
{
	InvokeHelper(0x89, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Document::AutoSummarize(VARIANT* Length, VARIANT* Mode, VARIANT* UpdateProperties)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x8a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Length, Mode, UpdateProperties);
	return result;
}

void _Document::RemoveNumbers(VARIANT* NumberType)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x8c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NumberType);
}

void _Document::ConvertNumbersToText(VARIANT* NumberType)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x8d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 NumberType);
}

long _Document::CountNumberedItems(VARIANT* NumberType, VARIANT* Level)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x8e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		NumberType, Level);
	return result;
}

void _Document::Post()
{
	InvokeHelper(0x8f, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ToggleFormsDesign()
{
	InvokeHelper(0x90, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::UpdateSummaryProperties()
{
	InvokeHelper(0x92, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

VARIANT _Document::GetCrossReferenceItems(VARIANT* ReferenceType)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x93, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		ReferenceType);
	return result;
}

void _Document::AutoFormat()
{
	InvokeHelper(0x94, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ViewCode()
{
	InvokeHelper(0x95, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ViewPropertyBrowser()
{
	InvokeHelper(0x96, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ForwardMailer()
{
	InvokeHelper(0xfa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::Reply()
{
	InvokeHelper(0xfb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ReplyAll()
{
	InvokeHelper(0xfc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::SendMailer(VARIANT* FileFormat, VARIANT* Priority)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xfd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileFormat, Priority);
}

void _Document::UndoClear()
{
	InvokeHelper(0xfe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::PresentIt()
{
	InvokeHelper(0xff, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::SendFax(LPCTSTR Address, VARIANT* Subject)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x100, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Address, Subject);
}

void _Document::ClosePrintPreview()
{
	InvokeHelper(0x102, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::CheckConsistency()
{
	InvokeHelper(0x103, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Document::CreateLetterContent(LPCTSTR DateFormat, BOOL IncludeHeaderFooter, LPCTSTR PageDesign, long LetterStyle, BOOL Letterhead, long LetterheadLocation, float LetterheadSize, LPCTSTR RecipientName, LPCTSTR RecipientAddress, 
		LPCTSTR Salutation, long SalutationType, LPCTSTR RecipientReference, LPCTSTR MailingInstructions, LPCTSTR AttentionLine, LPCTSTR Subject, LPCTSTR CCList, LPCTSTR ReturnAddress, LPCTSTR SenderName, LPCTSTR Closing, LPCTSTR SenderCompany, 
		LPCTSTR SenderJobTitle, LPCTSTR SenderInitials, long EnclosureNumber, VARIANT* InfoBlock, VARIANT* RecipientCode, VARIANT* RecipientGender, VARIANT* ReturnAddressShortForm, VARIANT* SenderCity, VARIANT* SenderCode, VARIANT* SenderGender, 
		VARIANT* SenderReference)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BSTR VTS_I4 VTS_BOOL VTS_I4 VTS_R4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4 VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT 
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x104, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		DateFormat, IncludeHeaderFooter, PageDesign, LetterStyle, Letterhead, LetterheadLocation, LetterheadSize, RecipientName, RecipientAddress, Salutation, SalutationType, RecipientReference, MailingInstructions, AttentionLine, Subject, 
		CCList, ReturnAddress, SenderName, Closing, SenderCompany, SenderJobTitle, SenderInitials, EnclosureNumber, InfoBlock, RecipientCode, RecipientGender, ReturnAddressShortForm, SenderCity, SenderCode, SenderGender, SenderReference);
	return result;
}

void _Document::AcceptAllRevisions()
{
	InvokeHelper(0x13d, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::RejectAllRevisions()
{
	InvokeHelper(0x13e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::DetectLanguage()
{
	InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ApplyTheme(LPCTSTR Name)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x142, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Name);
}

void _Document::RemoveTheme()
{
	InvokeHelper(0x143, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::WebPagePreview()
{
	InvokeHelper(0x145, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ReloadAs(long Encoding)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x14b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Encoding);
}

CString _Document::GetActiveTheme()
{
	CString result;
	InvokeHelper(0x21c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Document::GetActiveThemeDisplayName()
{
	CString result;
	InvokeHelper(0x21d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetEmail()
{
	LPDISPATCH result;
	InvokeHelper(0x13f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetScripts()
{
	LPDISPATCH result;
	InvokeHelper(0x140, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetLanguageDetected()
{
	BOOL result;
	InvokeHelper(0x141, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetLanguageDetected(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x141, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Document::GetFarEastLineBreakLanguage()
{
	long result;
	InvokeHelper(0x146, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetFarEastLineBreakLanguage(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x146, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH _Document::GetFrameset()
{
	LPDISPATCH result;
	InvokeHelper(0x147, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT _Document::GetClickAndTypeParagraphStyle()
{
	VARIANT result;
	InvokeHelper(0x148, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void _Document::SetClickAndTypeParagraphStyle(VARIANT* newValue)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x148, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH _Document::GetHTMLProject()
{
	LPDISPATCH result;
	InvokeHelper(0x149, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH _Document::GetWebOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x14a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Document::GetOpenEncoding()
{
	long result;
	InvokeHelper(0x14c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long _Document::GetSaveEncoding()
{
	long result;
	InvokeHelper(0x14d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetSaveEncoding(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x14d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _Document::GetOptimizeForWord97()
{
	BOOL result;
	InvokeHelper(0x14e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetOptimizeForWord97(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x14e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetVBASigned()
{
	BOOL result;
	InvokeHelper(0x14f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::ConvertVietDoc(long CodePageOrigin)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1bf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CodePageOrigin);
}

void _Document::PrintOut(VARIANT* Background, VARIANT* Append, VARIANT* Range, VARIANT* OutputFileName, VARIANT* From, VARIANT* To, VARIANT* Item, VARIANT* Copies, VARIANT* Pages, VARIANT* PageType, VARIANT* PrintToFile, VARIANT* Collate, 
		VARIANT* ActivePrinterMacGX, VARIANT* ManualDuplexPrint, VARIANT* PrintZoomColumn, VARIANT* PrintZoomRow, VARIANT* PrintZoomPaperWidth, VARIANT* PrintZoomPaperHeight)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x1be, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Background, Append, Range, OutputFileName, From, To, Item, Copies, Pages, PageType, PrintToFile, Collate, ActivePrinterMacGX, ManualDuplexPrint, PrintZoomColumn, PrintZoomRow, PrintZoomPaperWidth, PrintZoomPaperHeight);
}

LPDISPATCH _Document::GetMailEnvelope()
{
	LPDISPATCH result;
	InvokeHelper(0x150, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetDisableFeatures()
{
	BOOL result;
	InvokeHelper(0x151, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetDisableFeatures(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x151, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetDoNotEmbedSystemFonts()
{
	BOOL result;
	InvokeHelper(0x152, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetDoNotEmbedSystemFonts(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x152, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Document::GetSignatures()
{
	LPDISPATCH result;
	InvokeHelper(0x153, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _Document::GetDefaultTargetFrame()
{
	CString result;
	InvokeHelper(0x154, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Document::SetDefaultTargetFrame(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x154, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH _Document::GetHTMLDivisions()
{
	LPDISPATCH result;
	InvokeHelper(0x156, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long _Document::GetDisableFeaturesIntroducedAfter()
{
	long result;
	InvokeHelper(0x157, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetDisableFeaturesIntroducedAfter(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x157, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL _Document::GetRemovePersonalInformation()
{
	BOOL result;
	InvokeHelper(0x158, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetRemovePersonalInformation(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x158, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH _Document::GetSmartTags()
{
	LPDISPATCH result;
	InvokeHelper(0x15a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void _Document::Compare(LPCTSTR Name, VARIANT* AuthorName, VARIANT* CompareTarget, VARIANT* DetectFormatChanges, VARIANT* IgnoreAllComparisonWarnings, VARIANT* AddToRecentFiles)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x159, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Name, AuthorName, CompareTarget, DetectFormatChanges, IgnoreAllComparisonWarnings, AddToRecentFiles);
}

void _Document::CheckIn(BOOL SaveChanges, VARIANT* Comments, BOOL MakePublic)
{
	static BYTE parms[] =
		VTS_BOOL VTS_PVARIANT VTS_BOOL;
	InvokeHelper(0x15d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 SaveChanges, Comments, MakePublic);
}

BOOL _Document::CanCheckin()
{
	BOOL result;
	InvokeHelper(0x15f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::Merge(LPCTSTR FileName, VARIANT* MergeTarget, VARIANT* DetectFormatChanges, VARIANT* UseFormattingFrom, VARIANT* AddToRecentFiles)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x16a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, MergeTarget, DetectFormatChanges, UseFormattingFrom, AddToRecentFiles);
}

BOOL _Document::GetEmbedSmartTags()
{
	BOOL result;
	InvokeHelper(0x15b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetEmbedSmartTags(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x15b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetSmartTagsAsXMLProps()
{
	BOOL result;
	InvokeHelper(0x15c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetSmartTagsAsXMLProps(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x15c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Document::GetTextEncoding()
{
	long result;
	InvokeHelper(0x165, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetTextEncoding(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x165, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long _Document::GetTextLineEnding()
{
	long result;
	InvokeHelper(0x166, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetTextLineEnding(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x166, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Document::SendForReview(VARIANT* Recipients, VARIANT* Subject, VARIANT* ShowMessage, VARIANT* IncludeAttachment)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x161, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Recipients, Subject, ShowMessage, IncludeAttachment);
}

void _Document::ReplyWithChanges(VARIANT* ShowMessage)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x162, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ShowMessage);
}

void _Document::EndReview()
{
	InvokeHelper(0x164, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH _Document::GetStyleSheets()
{
	LPDISPATCH result;
	InvokeHelper(0x168, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT _Document::GetDefaultTableStyle()
{
	VARIANT result;
	InvokeHelper(0x16d, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

CString _Document::GetPasswordEncryptionProvider()
{
	CString result;
	InvokeHelper(0x16f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _Document::GetPasswordEncryptionAlgorithm()
{
	CString result;
	InvokeHelper(0x170, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

long _Document::GetPasswordEncryptionKeyLength()
{
	long result;
	InvokeHelper(0x171, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _Document::GetPasswordEncryptionFileProperties()
{
	BOOL result;
	InvokeHelper(0x172, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetPasswordEncryptionOptions(LPCTSTR PasswordEncryptionProvider, LPCTSTR PasswordEncryptionAlgorithm, long PasswordEncryptionKeyLength, VARIANT* PasswordEncryptionFileProperties)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x169, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 PasswordEncryptionProvider, PasswordEncryptionAlgorithm, PasswordEncryptionKeyLength, PasswordEncryptionFileProperties);
}

void _Document::RecheckSmartTags()
{
	InvokeHelper(0x16b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::RemoveSmartTags()
{
	InvokeHelper(0x16c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::SetDefaultTableStyle(VARIANT* Style, BOOL SetInTemplate)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_BOOL;
	InvokeHelper(0x16e, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Style, SetInTemplate);
}

void _Document::DeleteAllComments()
{
	InvokeHelper(0x173, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::AcceptAllRevisionsShown()
{
	InvokeHelper(0x174, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::RejectAllRevisionsShown()
{
	InvokeHelper(0x175, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::DeleteAllCommentsShown()
{
	InvokeHelper(0x176, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::ResetFormFields()
{
	InvokeHelper(0x177, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _Document::SaveAs(VARIANT* FileName, VARIANT* FileFormat, VARIANT* LockComments, VARIANT* Password, VARIANT* AddToRecentFiles, VARIANT* WritePassword, VARIANT* ReadOnlyRecommended, VARIANT* EmbedTrueTypeFonts, VARIANT* SaveNativePictureFormat, 
		VARIANT* SaveFormsData, VARIANT* SaveAsAOCELetter, VARIANT* Encoding, VARIANT* InsertLineBreaks, VARIANT* AllowSubstitutions, VARIANT* LineEnding, VARIANT* AddBiDiMarks)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x178, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, FileFormat, LockComments, Password, AddToRecentFiles, WritePassword, ReadOnlyRecommended, EmbedTrueTypeFonts, SaveNativePictureFormat, SaveFormsData, SaveAsAOCELetter, Encoding, InsertLineBreaks, AllowSubstitutions, LineEnding, 
		AddBiDiMarks);
}

BOOL _Document::GetEmbedLinguisticData()
{
	BOOL result;
	InvokeHelper(0x179, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetEmbedLinguisticData(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x179, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetFormattingShowFont()
{
	BOOL result;
	InvokeHelper(0x1c0, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetFormattingShowFont(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1c0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetFormattingShowClear()
{
	BOOL result;
	InvokeHelper(0x1c1, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetFormattingShowClear(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1c1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetFormattingShowParagraph()
{
	BOOL result;
	InvokeHelper(0x1c2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetFormattingShowParagraph(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1c2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL _Document::GetFormattingShowNumbering()
{
	BOOL result;
	InvokeHelper(0x1c3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Document::SetFormattingShowNumbering(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1c3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long _Document::GetFormattingShowFilter()
{
	long result;
	InvokeHelper(0x1c4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void _Document::SetFormattingShowFilter(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x1c4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void _Document::CheckNewSmartTags()
{
	InvokeHelper(0x17a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Range properties

/////////////////////////////////////////////////////////////////////////////
// Range operations

CString Range::GetText()
{
	CString result;
	InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Range::SetText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x0, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH Range::GetFormattedText()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::SetFormattedText(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long Range::GetStart()
{
	long result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetStart(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetEnd()
{
	long result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetEnd(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH Range::GetFont()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::SetFont(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH Range::GetDuplicate()
{
	LPDISPATCH result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetStoryType()
{
	long result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetTables()
{
	LPDISPATCH result;
	InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetWords()
{
	LPDISPATCH result;
	InvokeHelper(0x33, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetSentences()
{
	LPDISPATCH result;
	InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetCharacters()
{
	LPDISPATCH result;
	InvokeHelper(0x35, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetFootnotes()
{
	LPDISPATCH result;
	InvokeHelper(0x36, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetEndnotes()
{
	LPDISPATCH result;
	InvokeHelper(0x37, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetComments()
{
	LPDISPATCH result;
	InvokeHelper(0x38, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetCells()
{
	LPDISPATCH result;
	InvokeHelper(0x39, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetSections()
{
	LPDISPATCH result;
	InvokeHelper(0x3a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetParagraphs()
{
	LPDISPATCH result;
	InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetBorders()
{
	LPDISPATCH result;
	InvokeHelper(0x44c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::SetBorders(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x44c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH Range::GetShading()
{
	LPDISPATCH result;
	InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetTextRetrievalMode()
{
	LPDISPATCH result;
	InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::SetTextRetrievalMode(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x3e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH Range::GetFields()
{
	LPDISPATCH result;
	InvokeHelper(0x40, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetFormFields()
{
	LPDISPATCH result;
	InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetFrames()
{
	LPDISPATCH result;
	InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetParagraphFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x44e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::SetParagraphFormat(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x44e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH Range::GetListFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x44, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetBookmarks()
{
	LPDISPATCH result;
	InvokeHelper(0x4b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetBold()
{
	long result;
	InvokeHelper(0x82, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetBold(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x82, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetItalic()
{
	long result;
	InvokeHelper(0x83, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetItalic(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x83, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetUnderline()
{
	long result;
	InvokeHelper(0x8b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetUnderline(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetEmphasisMark()
{
	long result;
	InvokeHelper(0x8c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetEmphasisMark(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x8c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL Range::GetDisableCharacterSpaceGrid()
{
	BOOL result;
	InvokeHelper(0x8d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Range::SetDisableCharacterSpaceGrid(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH Range::GetRevisions()
{
	LPDISPATCH result;
	InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

VARIANT Range::GetStyle()
{
	VARIANT result;
	InvokeHelper(0x97, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Range::SetStyle(VARIANT* newValue)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x97, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long Range::GetStoryLength()
{
	long result;
	InvokeHelper(0x98, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Range::GetLanguageID()
{
	long result;
	InvokeHelper(0x99, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetLanguageID(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x99, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH Range::GetSynonymInfo()
{
	LPDISPATCH result;
	InvokeHelper(0x9b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetHyperlinks()
{
	LPDISPATCH result;
	InvokeHelper(0x9c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetListParagraphs()
{
	LPDISPATCH result;
	InvokeHelper(0x9d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetSubdocuments()
{
	LPDISPATCH result;
	InvokeHelper(0x9f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL Range::GetGrammarChecked()
{
	BOOL result;
	InvokeHelper(0x104, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Range::SetGrammarChecked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x104, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL Range::GetSpellingChecked()
{
	BOOL result;
	InvokeHelper(0x105, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Range::SetSpellingChecked(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x105, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long Range::GetHighlightColorIndex()
{
	long result;
	InvokeHelper(0x12d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetHighlightColorIndex(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x12d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH Range::GetColumns()
{
	LPDISPATCH result;
	InvokeHelper(0x12e, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetRows()
{
	LPDISPATCH result;
	InvokeHelper(0x12f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL Range::GetIsEndOfRowMark()
{
	BOOL result;
	InvokeHelper(0x133, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

long Range::GetBookmarkID()
{
	long result;
	InvokeHelper(0x134, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

long Range::GetPreviousBookmarkID()
{
	long result;
	InvokeHelper(0x135, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetFind()
{
	LPDISPATCH result;
	InvokeHelper(0x106, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetPageSetup()
{
	LPDISPATCH result;
	InvokeHelper(0x44d, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::SetPageSetup(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x44d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH Range::GetShapeRange()
{
	LPDISPATCH result;
	InvokeHelper(0x137, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetCase()
{
	long result;
	InvokeHelper(0x138, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetCase(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x138, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

VARIANT Range::GetInformation(long Type)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x139, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms,
		Type);
	return result;
}

LPDISPATCH Range::GetReadabilityStatistics()
{
	LPDISPATCH result;
	InvokeHelper(0x13a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetGrammaticalErrors()
{
	LPDISPATCH result;
	InvokeHelper(0x13b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetSpellingErrors()
{
	LPDISPATCH result;
	InvokeHelper(0x13c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetOrientation()
{
	long result;
	InvokeHelper(0x13d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetOrientation(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH Range::GetInlineShapes()
{
	LPDISPATCH result;
	InvokeHelper(0x13f, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetNextStoryRange()
{
	LPDISPATCH result;
	InvokeHelper(0x140, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetLanguageIDFarEast()
{
	long result;
	InvokeHelper(0x141, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetLanguageIDFarEast(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x141, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetLanguageIDOther()
{
	long result;
	InvokeHelper(0x142, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetLanguageIDOther(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x142, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

void Range::Select()
{
	InvokeHelper(0xffff, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::SetRange(long Start, long End)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Start, End);
}

void Range::Collapse(VARIANT* Direction)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Direction);
}

void Range::InsertBefore(LPCTSTR Text)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text);
}

void Range::InsertAfter(LPCTSTR Text)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text);
}

LPDISPATCH Range::Next(VARIANT* Unit, VARIANT* Count)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x69, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Unit, Count);
	return result;
}

LPDISPATCH Range::Previous(VARIANT* Unit, VARIANT* Count)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Unit, Count);
	return result;
}

long Range::StartOf(VARIANT* Unit, VARIANT* Extend)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x6b, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Unit, Extend);
	return result;
}

long Range::EndOf(VARIANT* Unit, VARIANT* Extend)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x6c, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Unit, Extend);
	return result;
}

long Range::Move(VARIANT* Unit, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x6d, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Unit, Count);
	return result;
}

long Range::MoveStart(VARIANT* Unit, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x6e, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Unit, Count);
	return result;
}

long Range::MoveEnd(VARIANT* Unit, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x6f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Unit, Count);
	return result;
}

long Range::MoveWhile(VARIANT* Cset, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x70, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Cset, Count);
	return result;
}

long Range::MoveStartWhile(VARIANT* Cset, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x71, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Cset, Count);
	return result;
}

long Range::MoveEndWhile(VARIANT* Cset, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x72, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Cset, Count);
	return result;
}

long Range::MoveUntil(VARIANT* Cset, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x73, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Cset, Count);
	return result;
}

long Range::MoveStartUntil(VARIANT* Cset, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x74, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Cset, Count);
	return result;
}

long Range::MoveEndUntil(VARIANT* Cset, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x75, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Cset, Count);
	return result;
}

void Range::Cut()
{
	InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::Copy()
{
	InvokeHelper(0x78, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::Paste()
{
	InvokeHelper(0x79, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::InsertBreak(VARIANT* Type)
{
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x7a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type);
}

void Range::InsertFile(LPCTSTR FileName, VARIANT* Range, VARIANT* ConfirmConversions, VARIANT* Link, VARIANT* Attachment)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x7b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 FileName, Range, ConfirmConversions, Link, Attachment);
}

BOOL Range::InStory(LPDISPATCH Range)
{
	BOOL result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x7d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Range);
	return result;
}

BOOL Range::InRange(LPDISPATCH Range)
{
	BOOL result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x7e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Range);
	return result;
}

long Range::Delete(VARIANT* Unit, VARIANT* Count)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x7f, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Unit, Count);
	return result;
}

void Range::WholeStory()
{
	InvokeHelper(0x80, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long Range::Expand(VARIANT* Unit)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x81, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Unit);
	return result;
}

void Range::InsertParagraph()
{
	InvokeHelper(0xa0, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::InsertParagraphAfter()
{
	InvokeHelper(0xa1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::InsertSymbol(long CharacterNumber, VARIANT* Font, VARIANT* Unicode, VARIANT* Bias)
{
	static BYTE parms[] =
		VTS_I4 VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xa4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CharacterNumber, Font, Unicode, Bias);
}

void Range::InsertCrossReference(VARIANT* ReferenceType, long ReferenceKind, VARIANT* ReferenceItem, VARIANT* InsertAsHyperlink, VARIANT* IncludePosition)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_I4 VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xa5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ReferenceType, ReferenceKind, ReferenceItem, InsertAsHyperlink, IncludePosition);
}

void Range::InsertCaption(VARIANT* Label, VARIANT* Title, VARIANT* TitleAutoText, VARIANT* Position)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xa6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Label, Title, TitleAutoText, Position);
}

void Range::CopyAsPicture()
{
	InvokeHelper(0xa7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::SortAscending()
{
	InvokeHelper(0xa9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::SortDescending()
{
	InvokeHelper(0xaa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL Range::IsEqual(LPDISPATCH Range)
{
	BOOL result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xab, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Range);
	return result;
}

float Range::Calculate()
{
	float result;
	InvokeHelper(0xac, DISPATCH_METHOD, VT_R4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GoTo(VARIANT* What, VARIANT* Which, VARIANT* Count, VARIANT* Name)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xad, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		What, Which, Count, Name);
	return result;
}

LPDISPATCH Range::GoToNext(long What)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xae, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		What);
	return result;
}

LPDISPATCH Range::GoToPrevious(long What)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xaf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		What);
	return result;
}

void Range::PasteSpecial(VARIANT* IconIndex, VARIANT* Link, VARIANT* Placement, VARIANT* DisplayAsIcon, VARIANT* DataType, VARIANT* IconFileName, VARIANT* IconLabel)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xb0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 IconIndex, Link, Placement, DisplayAsIcon, DataType, IconFileName, IconLabel);
}

void Range::LookupNameProperties()
{
	InvokeHelper(0xb1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long Range::ComputeStatistics(long Statistic)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb2, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		Statistic);
	return result;
}

void Range::Relocate(long Direction)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xb3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Direction);
}

void Range::CheckSynonyms()
{
	InvokeHelper(0xb4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::SubscribeTo(LPCTSTR Edition, VARIANT* Format)
{
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0xb5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Edition, Format);
}

void Range::CreatePublisher(VARIANT* Edition, VARIANT* ContainsPICT, VARIANT* ContainsRTF, VARIANT* ContainsText)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xb6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Edition, ContainsPICT, ContainsRTF, ContainsText);
}

void Range::InsertAutoText()
{
	InvokeHelper(0xb7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::InsertDatabase(VARIANT* Format, VARIANT* Style, VARIANT* LinkToSource, VARIANT* Connection, VARIANT* SQLStatement, VARIANT* SQLStatement1, VARIANT* PasswordDocument, VARIANT* PasswordTemplate, VARIANT* WritePasswordDocument, 
		VARIANT* WritePasswordTemplate, VARIANT* DataSource, VARIANT* From, VARIANT* To, VARIANT* IncludeFields)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xc2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Format, Style, LinkToSource, Connection, SQLStatement, SQLStatement1, PasswordDocument, PasswordTemplate, WritePasswordDocument, WritePasswordTemplate, DataSource, From, To, IncludeFields);
}

void Range::AutoFormat()
{
	InvokeHelper(0xc3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::CheckGrammar()
{
	InvokeHelper(0xcc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::CheckSpelling(VARIANT* CustomDictionary, VARIANT* IgnoreUppercase, VARIANT* AlwaysSuggest, VARIANT* CustomDictionary2, VARIANT* CustomDictionary3, VARIANT* CustomDictionary4, VARIANT* CustomDictionary5, VARIANT* CustomDictionary6, 
		VARIANT* CustomDictionary7, VARIANT* CustomDictionary8, VARIANT* CustomDictionary9, VARIANT* CustomDictionary10)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xcd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 CustomDictionary, IgnoreUppercase, AlwaysSuggest, CustomDictionary2, CustomDictionary3, CustomDictionary4, CustomDictionary5, CustomDictionary6, CustomDictionary7, CustomDictionary8, CustomDictionary9, CustomDictionary10);
}

LPDISPATCH Range::GetSpellingSuggestions(VARIANT* CustomDictionary, VARIANT* IgnoreUppercase, VARIANT* MainDictionary, VARIANT* SuggestionMode, VARIANT* CustomDictionary2, VARIANT* CustomDictionary3, VARIANT* CustomDictionary4, 
		VARIANT* CustomDictionary5, VARIANT* CustomDictionary6, VARIANT* CustomDictionary7, VARIANT* CustomDictionary8, VARIANT* CustomDictionary9, VARIANT* CustomDictionary10)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xd1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		CustomDictionary, IgnoreUppercase, MainDictionary, SuggestionMode, CustomDictionary2, CustomDictionary3, CustomDictionary4, CustomDictionary5, CustomDictionary6, CustomDictionary7, CustomDictionary8, CustomDictionary9, CustomDictionary10);
	return result;
}

void Range::InsertParagraphBefore()
{
	InvokeHelper(0xd4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::NextSubdocument()
{
	InvokeHelper(0xdb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::PreviousSubdocument()
{
	InvokeHelper(0xdc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::ConvertHangulAndHanja(VARIANT* ConversionsMode, VARIANT* FastConversion, VARIANT* CheckHangulEnding, VARIANT* EnableRecentOrdering, VARIANT* CustomDictionary)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xdd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ConversionsMode, FastConversion, CheckHangulEnding, EnableRecentOrdering, CustomDictionary);
}

void Range::PasteAsNestedTable()
{
	InvokeHelper(0xde, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Range::ModifyEnclosure(VARIANT* Style, VARIANT* Symbol, VARIANT* EnclosedText)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xdf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Style, Symbol, EnclosedText);
}

void Range::PhoneticGuide(LPCTSTR Text, long Alignment, long Raise, long FontSize, LPCTSTR FontName)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(0xe0, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Text, Alignment, Raise, FontSize, FontName);
}

void Range::InsertDateTime(VARIANT* DateTimeFormat, VARIANT* InsertAsField, VARIANT* InsertAsFullWidth, VARIANT* DateLanguage, VARIANT* CalendarType)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x1bc, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 DateTimeFormat, InsertAsField, InsertAsFullWidth, DateLanguage, CalendarType);
}

void Range::Sort(VARIANT* ExcludeHeader, VARIANT* FieldNumber, VARIANT* SortFieldType, VARIANT* SortOrder, VARIANT* FieldNumber2, VARIANT* SortFieldType2, VARIANT* SortOrder2, VARIANT* FieldNumber3, VARIANT* SortFieldType3, VARIANT* SortOrder3, 
		VARIANT* SortColumn, VARIANT* Separator, VARIANT* CaseSensitive, VARIANT* BidiSort, VARIANT* IgnoreThe, VARIANT* IgnoreKashida, VARIANT* IgnoreDiacritics, VARIANT* IgnoreHe, VARIANT* LanguageID)
{
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT 
		VTS_PVARIANT;
	InvokeHelper(0x1e4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ExcludeHeader, FieldNumber, SortFieldType, SortOrder, FieldNumber2, SortFieldType2, SortOrder2, FieldNumber3, SortFieldType3, SortOrder3, SortColumn, Separator, CaseSensitive, BidiSort, IgnoreThe, IgnoreKashida, IgnoreDiacritics, 
		IgnoreHe, LanguageID);
}

void Range::DetectLanguage()
{
	InvokeHelper(0xcb, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Range::ConvertToTable(VARIANT* Separator, VARIANT* NumRows, VARIANT* NumColumns, VARIANT* InitialColumnWidth, VARIANT* Format, VARIANT* ApplyBorders, VARIANT* ApplyShading, VARIANT* ApplyFont, VARIANT* ApplyColor, 
		VARIANT* ApplyHeadingRows, VARIANT* ApplyLastRow, VARIANT* ApplyFirstColumn, VARIANT* ApplyLastColumn, VARIANT* AutoFit, VARIANT* AutoFitBehavior, VARIANT* DefaultTableBehavior)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x1f2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Separator, NumRows, NumColumns, InitialColumnWidth, Format, ApplyBorders, ApplyShading, ApplyFont, ApplyColor, ApplyHeadingRows, ApplyLastRow, ApplyFirstColumn, ApplyLastColumn, AutoFit, AutoFitBehavior, DefaultTableBehavior);
	return result;
}

void Range::TCSCConverter(long WdTCSCConverterDirection, BOOL CommonTerms, BOOL UseVariants)
{
	static BYTE parms[] =
		VTS_I4 VTS_BOOL VTS_BOOL;
	InvokeHelper(0x1f3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 WdTCSCConverterDirection, CommonTerms, UseVariants);
}

BOOL Range::GetLanguageDetected()
{
	BOOL result;
	InvokeHelper(0x107, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Range::SetLanguageDetected(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x107, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

float Range::GetFitTextWidth()
{
	float result;
	InvokeHelper(0x108, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void Range::SetFitTextWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x108, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long Range::GetHorizontalInVertical()
{
	long result;
	InvokeHelper(0x109, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetHorizontalInVertical(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x109, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetTwoLinesInOne()
{
	long result;
	InvokeHelper(0x10a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetTwoLinesInOne(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x10a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

BOOL Range::GetCombineCharacters()
{
	BOOL result;
	InvokeHelper(0x10b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Range::SetCombineCharacters(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x10b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long Range::GetNoProofing()
{
	long result;
	InvokeHelper(0x143, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetNoProofing(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x143, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH Range::GetTopLevelTables()
{
	LPDISPATCH result;
	InvokeHelper(0x144, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetScripts()
{
	LPDISPATCH result;
	InvokeHelper(0x145, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Range::GetCharacterWidth()
{
	long result;
	InvokeHelper(0x146, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetCharacterWidth(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x146, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetKana()
{
	long result;
	InvokeHelper(0x147, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetKana(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x147, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetBoldBi()
{
	long result;
	InvokeHelper(0x190, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetBoldBi(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x190, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long Range::GetItalicBi()
{
	long result;
	InvokeHelper(0x191, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void Range::SetItalicBi(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x191, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

CString Range::GetId()
{
	CString result;
	InvokeHelper(0x195, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void Range::SetId(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x195, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

LPDISPATCH Range::GetHTMLDivisions()
{
	LPDISPATCH result;
	InvokeHelper(0x196, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetSmartTags()
{
	LPDISPATCH result;
	InvokeHelper(0x197, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL Range::GetShowAll()
{
	BOOL result;
	InvokeHelper(0x198, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void Range::SetShowAll(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x198, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

LPDISPATCH Range::GetDocument()
{
	LPDISPATCH result;
	InvokeHelper(0x199, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetFootnoteOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x19a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH Range::GetEndnoteOptions()
{
	LPDISPATCH result;
	InvokeHelper(0x19b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void Range::PasteAndFormat(long Type)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x19c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Type);
}

void Range::PasteExcelTable(BOOL LinkedToExcel, BOOL WordFormatting, BOOL RTF)
{
	static BYTE parms[] =
		VTS_BOOL VTS_BOOL VTS_BOOL;
	InvokeHelper(0x19d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 LinkedToExcel, WordFormatting, RTF);
}

void Range::PasteAppendTable()
{
	InvokeHelper(0x19e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// InlineShape properties

/////////////////////////////////////////////////////////////////////////////
// InlineShape operations

LPDISPATCH InlineShape::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long InlineShape::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetBorders()
{
	LPDISPATCH result;
	InvokeHelper(0x44c, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void InlineShape::SetBorders(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x44c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

LPDISPATCH InlineShape::GetRange()
{
	LPDISPATCH result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetLinkFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetField()
{
	LPDISPATCH result;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetOLEFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long InlineShape::GetType()
{
	long result;
	InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetHyperlink()
{
	LPDISPATCH result;
	InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

float InlineShape::GetHeight()
{
	float result;
	InvokeHelper(0x8, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void InlineShape::SetHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x8, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float InlineShape::GetWidth()
{
	float result;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void InlineShape::SetWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float InlineShape::GetScaleHeight()
{
	float result;
	InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void InlineShape::SetScaleHeight(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

float InlineShape::GetScaleWidth()
{
	float result;
	InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_R4, (void*)&result, NULL);
	return result;
}

void InlineShape::SetScaleWidth(float newValue)
{
	static BYTE parms[] =
		VTS_R4;
	InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

long InlineShape::GetLockAspectRatio()
{
	long result;
	InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void InlineShape::SetLockAspectRatio(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

LPDISPATCH InlineShape::GetLine()
{
	LPDISPATCH result;
	InvokeHelper(0x70, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetFill()
{
	LPDISPATCH result;
	InvokeHelper(0x6b, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetPictureFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void InlineShape::SetPictureFormat(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x76, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void InlineShape::Activate()
{
	InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void InlineShape::Reset()
{
	InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void InlineShape::Delete()
{
	InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void InlineShape::Select()
{
	InvokeHelper(0xffff, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH InlineShape::ConvertToShape()
{
	LPDISPATCH result;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetHorizontalLineFormat()
{
	LPDISPATCH result;
	InvokeHelper(0x77, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetScript()
{
	LPDISPATCH result;
	InvokeHelper(0x7a, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShape::GetTextEffect()
{
	LPDISPATCH result;
	InvokeHelper(0x78, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void InlineShape::SetTextEffect(LPDISPATCH newValue)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x78, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

CString InlineShape::GetAlternativeText()
{
	CString result;
	InvokeHelper(0x83, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void InlineShape::SetAlternativeText(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x83, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL InlineShape::GetIsPictureBullet()
{
	BOOL result;
	InvokeHelper(0x84, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// InlineShapes properties

/////////////////////////////////////////////////////////////////////////////
// InlineShapes operations

LPDISPATCH InlineShapes::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x3e8, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long InlineShapes::GetCreator()
{
	long result;
	InvokeHelper(0x3e9, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShapes::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x3ea, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long InlineShapes::GetCount()
{
	long result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPUNKNOWN InlineShapes::Get_NewEnum()
{
	LPUNKNOWN result;
	InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPDISPATCH InlineShapes::Item(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH InlineShapes::AddPicture(LPCTSTR FileName, VARIANT* LinkToFile, VARIANT* SaveWithDocument, VARIANT* Range)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x64, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, LinkToFile, SaveWithDocument, Range);
	return result;
}

LPDISPATCH InlineShapes::AddOLEObject(VARIANT* ClassType, VARIANT* FileName, VARIANT* LinkToFile, VARIANT* DisplayAsIcon, VARIANT* IconFileName, VARIANT* IconIndex, VARIANT* IconLabel, VARIANT* Range)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ClassType, FileName, LinkToFile, DisplayAsIcon, IconFileName, IconIndex, IconLabel, Range);
	return result;
}

LPDISPATCH InlineShapes::AddOLEControl(VARIANT* ClassType, VARIANT* Range)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ClassType, Range);
	return result;
}

LPDISPATCH InlineShapes::New(LPDISPATCH Range)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0xc8, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Range);
	return result;
}

LPDISPATCH InlineShapes::AddHorizontalLine(LPCTSTR FileName, VARIANT* Range)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x68, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, Range);
	return result;
}

LPDISPATCH InlineShapes::AddHorizontalLineStandard(VARIANT* Range)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x69, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Range);
	return result;
}

LPDISPATCH InlineShapes::AddPictureBullet(LPCTSTR FileName, VARIANT* Range)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT;
	InvokeHelper(0x6a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, Range);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Shapes properties

/////////////////////////////////////////////////////////////////////////////
// Shapes operations

LPDISPATCH Shapes::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x1f40, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shapes::GetCreator()
{
	long result;
	InvokeHelper(0x1f41, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes::GetParent()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

long Shapes::GetCount()
{
	long result;
	InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

LPUNKNOWN Shapes::Get_NewEnum()
{
	LPUNKNOWN result;
	InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

LPDISPATCH Shapes::Item(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH Shapes::AddCallout(long Type, float Left, float Top, float Width, float Height, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_PVARIANT;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Left, Top, Width, Height, Anchor);
	return result;
}

LPDISPATCH Shapes::AddCurve(VARIANT* SafeArrayOfPoints, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		SafeArrayOfPoints, Anchor);
	return result;
}

LPDISPATCH Shapes::AddLabel(long Orientation, float Left, float Top, float Width, float Height, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_PVARIANT;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Orientation, Left, Top, Width, Height, Anchor);
	return result;
}

LPDISPATCH Shapes::AddLine(float BeginX, float BeginY, float EndX, float EndY, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_PVARIANT;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		BeginX, BeginY, EndX, EndY, Anchor);
	return result;
}

LPDISPATCH Shapes::AddPicture(LPCTSTR FileName, VARIANT* LinkToFile, VARIANT* SaveWithDocument, VARIANT* Left, VARIANT* Top, VARIANT* Width, VARIANT* Height, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		FileName, LinkToFile, SaveWithDocument, Left, Top, Width, Height, Anchor);
	return result;
}

LPDISPATCH Shapes::AddPolyline(VARIANT* SafeArrayOfPoints, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		SafeArrayOfPoints, Anchor);
	return result;
}

LPDISPATCH Shapes::AddShape(long Type, float Left, float Top, float Width, float Height, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_PVARIANT;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Left, Top, Width, Height, Anchor);
	return result;
}

LPDISPATCH Shapes::AddTextEffect(long PresetTextEffect, LPCTSTR Text, LPCTSTR FontName, float FontSize, long FontBold, long FontItalic, float Left, float Top, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_BSTR VTS_R4 VTS_I4 VTS_I4 VTS_R4 VTS_R4 VTS_PVARIANT;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		PresetTextEffect, Text, FontName, FontSize, FontBold, FontItalic, Left, Top, Anchor);
	return result;
}

LPDISPATCH Shapes::AddTextbox(long Orientation, float Left, float Top, float Width, float Height, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_PVARIANT;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Orientation, Left, Top, Width, Height, Anchor);
	return result;
}

LPDISPATCH Shapes::BuildFreeform(long EditingType, float X1, float Y1)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		EditingType, X1, Y1);
	return result;
}

LPDISPATCH Shapes::Range(VARIANT* Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

void Shapes::SelectAll()
{
	InvokeHelper(0x16, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH Shapes::AddOLEObject(VARIANT* ClassType, VARIANT* FileName, VARIANT* LinkToFile, VARIANT* DisplayAsIcon, VARIANT* IconFileName, VARIANT* IconIndex, VARIANT* IconLabel, VARIANT* Left, VARIANT* Top, VARIANT* Width, VARIANT* Height, 
		VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ClassType, FileName, LinkToFile, DisplayAsIcon, IconFileName, IconIndex, IconLabel, Left, Top, Width, Height, Anchor);
	return result;
}

LPDISPATCH Shapes::AddOLEControl(VARIANT* ClassType, VARIANT* Left, VARIANT* Top, VARIANT* Width, VARIANT* Height, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT;
	InvokeHelper(0x66, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ClassType, Left, Top, Width, Height, Anchor);
	return result;
}

LPDISPATCH Shapes::AddDiagram(long Type, float Left, float Top, float Width, float Height, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_PVARIANT;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, Left, Top, Width, Height, Anchor);
	return result;
}

LPDISPATCH Shapes::AddCanvas(float Left, float Top, float Width, float Height, VARIANT* Anchor)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_R4 VTS_R4 VTS_R4 VTS_R4 VTS_PVARIANT;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Left, Top, Width, Height, Anchor);
	return result;
}
