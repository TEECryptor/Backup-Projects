//------------------------------------------------	
//	Name:XMLHelper.h
//	Introduce:XML file loader help class
//	Creator:Youfang Yao
//	Date:2007-04-03
//	Version:1.0V
//------------------------------------------------	
#ifndef	_XMLHELPER_H_
#define	_XMLHELPER_H_
//------------------------------------------------	
#import "msxml4.dll" 
using namespace MSXML2;
//------------------------------------------------	
//	Name:CXMLHelper
//	Introduce:The class for XML file loader help
//------------------------------------------------
class CXMLHelper
{
public:
	CXMLHelper();
	virtual ~CXMLHelper();
	//--------------------------------------------
public:
	//--------------------------------------------
	//	Load a XML file
	static BOOL	LoadXMLFile(IXMLDOMDocumentPtr &pDocument, LPCTSTR lpszFile);
	//	Get a string attribute of node
	static BOOL	GetNodeAttribute(IXMLDOMElementPtr pElement, LPCTSTR lpszName, LPTSTR lpValue);
	//	Get a int attribute of node
	static BOOL	GetNodeAttribute(IXMLDOMElementPtr pElement, LPCTSTR lpszName, int* lpValue);
	//	Get a long attribute of node
	static BOOL	GetNodeAttribute(IXMLDOMElementPtr pElement, LPCTSTR lpszName, long* lpValue);
	//--------------------------------------------
};
//------------------------------------------------	
#endif	//_XMLHELPER_H_
//------------------------------------------------	