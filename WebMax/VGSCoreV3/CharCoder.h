#ifndef CHARACTER_CODE_CONVERTER_H
#define CHARACTER_CODE_CONVERTER_H

#include <string>
#include <Windows.h>

/**	字符集转换.
	description:
		1.实现常用字符集（GB2312、Utf-8等）之间的转换；
		2.说明：
			UTF-8:   3字节一个字符
			UNICODE: 2字节一个字符
			GB2312:  1字节一个字符
	author:
		林秀全
	email:
		linxq@live.com
	date:
		2010-7-15
*/
class CharCoder
{
public:
	// 把UTF-8转换成Unicode
	static void UTF_8ToUnicode(wchar_t* pOut,const char *pText)
	{
		char* uchar = (char *)pOut;

		uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
		uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	}
	//Unicode 转换成UTF-8
	static void UnicodeToUTF_8(char* pOut,const wchar_t* pText)
	{
		// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
		char* pchar = (char *)pText;

		pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
		pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
		pOut[2] = (0x80 | (pchar[0] & 0x3F));
	}
	// 把Unicode 转换成 GB2312
	static void UnicodeToGB2312(char* pOut,wchar_t uData)
	{
		WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
	}
	// GB2312 转换成　Unicode
	static void Gb2312ToUnicode(wchar_t* pOut,const char *gbBuffer)
	{
		::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	}
	//GB2312 转为 UTF-8
	static void GB2312ToUTF_8(std::string& pOut,const char *pText, int pLen)
	{
		char buf[4];
		int nLength = pLen* 3;
		char* rst = new char[nLength];

		memset(buf,0,4);
		memset(rst,0,nLength);

		int i = 0;
		int j = 0;     
		while(i < pLen)
		{
			//如果是英文直接复制就能
			if( *(pText + i) >= 0)
			{
				rst[j++] = pText[i++];
			}
			else
			{
				wchar_t pbuffer;
				Gb2312ToUnicode(&pbuffer,pText+i);

				UnicodeToUTF_8(buf,&pbuffer);

				unsigned short int tmp = 0;
				tmp = rst[j] = buf[0];
				tmp = rst[j+1] = buf[1];
				tmp = rst[j+2] = buf[2];    

				j += 3;
				i += 2;
			}
		}
		rst[j] = '\0';

		//返回结果
		pOut = rst;             
		delete []rst;
	}
	//UTF-8 转为 GB2312
	static void UTF_8ToGB2312(std::string &pOut, const char *pText, int pLen)
	{
		char * newBuf = new char[pLen];
		char Ctemp[4];
		memset(Ctemp,0,4);

		int i =0;
		int j = 0;

		while(i < pLen)
		{
			if(pText > 0)
			{
				newBuf[j++] = pText[i++];                      
			}
			else                
			{
				WCHAR Wtemp;
				UTF_8ToUnicode(&Wtemp,pText + i);

				UnicodeToGB2312(Ctemp,Wtemp);

				newBuf[j] = Ctemp[0];
				newBuf[j + 1] = Ctemp[1];

				i += 3;    
				j += 2;  
			}
		}
		newBuf[j] = '\0';

		pOut = newBuf;
		delete []newBuf;
	}
};

#endif
