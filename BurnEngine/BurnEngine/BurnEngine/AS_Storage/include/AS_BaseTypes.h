///////////////////////////////////////////////////////////////////////////////
// AS_BaseTypes.h
// Copyright (c) 2003 - 2007, Sonic Solutions.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef __AS_BaseTypes_h__
#define __AS_BaseTypes_h__

#include <cassert>
#include <string>

#if defined(__unix__)
	#define AS_UNIX
	#define AS_CALLBACK
	#ifdef OSXPORT
		#define AS_API __attribute__((visibility("default")))
	#else
		#ifndef AS_API
			#define AS_API
		#endif
	#endif
#endif

#if defined(_WIN32)
#include <wchar.h>
#define AS_WIN32
#define AS_CALLBACK __stdcall
#ifndef AS_API
#define AS_API __stdcall
#endif
#endif

struct AS_String; // string

#ifdef _MSC_VER

typedef signed __int8 SInt8;		//!< signed 8-bit integer
typedef unsigned __int8 UInt8;		//!< unsigned 8-bit integer
typedef signed __int16 SInt16;		//!< signed 16-bit integer
typedef unsigned __int16 UInt16;	//!< unsigned 16-bit integer
typedef signed __int32 SInt32;		//!< signed 32-bit integer
typedef unsigned __int32 UInt32;	//!< unsigned 32-bit integer
typedef signed __int64 SInt64;		//!< signed 64-bit integer
typedef unsigned __int64 UInt64;	//!< unsigned 64-bit integer

#else

typedef signed char SInt8;			//!< signed 8-bit integer
typedef unsigned char UInt8;		//!< unsigned 8-bit integer
typedef signed short SInt16;		//!< signed 16-bit integer
typedef unsigned short UInt16;		//!< unsigned 16-bit integer
typedef signed long SInt32;			//!< signed 32-bit integer
typedef unsigned long UInt32;		//!< unsigned 32-bit integer
typedef signed long long SInt64;	//!< signed 64-bit integer
typedef unsigned long long UInt64;	//!< unsigned 64-bit integer

#endif

typedef SInt32 AS_Error;	//!< error number

#ifndef AS_Message_DEFINED
#define AS_Message_DEFINED

struct AS_Message	//! message localization
{
	enum Language	//! message languages
	{
		Language_ChineseSimplified = 'CHS',
		Language_ChineseTraditional = 'CHT',
		Language_Danish = 'DAN',
		Language_Dutch = 'NLD',
		Language_English = 'ENU',
		Language_Finnish = 'FIN',
		Language_French = 'FRA',
		Language_German = 'DEU',
		Language_Italian = 'ITA',
		Language_Japanese = 'JPN',
		Language_Korean = 'KOR',
		Language_Norweigian = 'NOR',
		Language_Portuguese = 'PTG',
		Language_PortugueseBrazil = 'PTB',
		Language_Spanish = 'ESN',
		Language_Swedish = 'SVE',
		Language_Default = 0,	//!< embedded English (default)
		Language_Count = 17		//!< count of languages
	};

	//! Message callback proc.
	//! @param[in]	context - callback context
	//! @param[out]	text - message text
	//! @param[out]	number - message number
	typedef void AS_CALLBACK Proc(const void * context, AS_String & text, SInt32 & number);
};

#endif //AS_Message_DEFINED

#ifndef AS_Alert_DEFINED
#define AS_Alert_DEFINED

struct AS_Alert //! alert
{
	enum Bttn //! alert buttons
	{
		Bttn_Default = 0x0000,	//!< use default handler
		Bttn_Cancel = 0x0001,	//!< "Cancel" button
		Bttn_No = 0x0002,		//!< "No" button
		Bttn_Ok = 0x0004,		//!< "Ok" button
		Bttn_Retry = 0x0008,	//!< "Retry" button
		Bttn_Yes = 0x0010		//!< "Yes" button
	};

	enum Level //! alert warning levels
	{
		Level_Note = 1,		//!< informational
		Level_Caution = 2,	//!< cautionary
		Level_Stop = 3		//!< fatal error
	};

	enum Type //! alert types
	{
		Type_Ok = Bttn_Ok,								//!< "Ok" button
		Type_OkCancel = Bttn_Ok | Bttn_Cancel,			//!< "Ok" and "Cancel" buttons
		Type_RetryCancel = Bttn_Retry | Bttn_Cancel,	//!< "Retry" and "Cancel" buttons
		Type_YesNo = Bttn_Yes | Bttn_No					//!< "Yes" and "No" buttons
	};

	struct Info //! alert information
	{
		Level level;				//!< alert level
		Type type;					//!< alert type
		void * context;				//!< message proc context
		AS_Message::Proc * message;	//!< suggested alert message
	};

	//! Alert callback proc.
	//! @param[in]	info - alert information
	//! @return		alert button pressed
	//! @note		An alert handler need not present UI and may instead simulate the pressing of a button.
	typedef Bttn AS_CALLBACK Proc(const Info & info, const void * userdata);
};

#endif //AS_Alert_DEFINED

#ifndef AS_Bitmap_DEFINED
#define AS_Bitmap_DEFINED

struct AS_Bitmap //! bitmap
{
	enum Format //! pixel formats
	{
		Format_BGR24 = 1,	//!< 24 bit BGR
		Format_RGB24 = 2	//!< 24 bit RGB
	};

	struct BGR24 //! 24 bit BGR pixel
	{
		UInt8 blue;		//!< blue component
		UInt8 green;	//!< green component
		UInt8 red;		//!< red component
	};

	struct RGB24 //! 24 bit RGB pixel
	{
		UInt8 red;		//!< red component
		UInt8 green;	//!< green component
		UInt8 blue;		//!< blue component
	};

	UInt16 width;	//!< width in pixels
	UInt16 height;	//!< height in pixels
	Format format;	//!< pixel format
	UInt32 stride;	//!< width of row in bytes
	void * pixels;	//!< pointer to topleft pixel
};

#endif //AS_Bitmap_DEFINED

#ifndef AS_Color_DEFINED
#define AS_Color_DEFINED

struct AS_Color //! color specification
{
	UInt8 r;	//!< red component
	UInt8 g;	//!< green component
	UInt8 b;	//!< blue component
};

#endif //AS_Color_DEFINED

#ifndef AS_Point_DEFINED
#define AS_Point_DEFINED

struct AS_Point //! point
{
	SInt32 x;	//!< x coordinate
	SInt32 y;	//!< y coordinate

	AS_Point() {}
	AS_Point(SInt32 _x, SInt32 _y) : x(_x), y(_y) {}
};

#endif //AS_Point_DEFINED

#ifndef AS_ProgressProc_DEFINED
#define AS_ProgressProc_DEFINED

//! Progress callback proc.
//! @param[in]	progress - fractional progress (0.0 .. 1.0)
//! @param[in]	userdata - progress callback userdata
//! @param[in]	context - callback context
//! @param[in]	message - pointer to message proc (0 == no message available)
//! @param[in]	step - global step number (1 .. count)
//! @param[in]	count - global step count
//! @return		cancel current operation?
typedef bool AS_CALLBACK AS_ProgressProc(double progress, const void * userdata, const void * context, AS_Message::Proc * message, UInt32 step, UInt32 count); 

#endif //AS_ProgressProc_DEFINED

#ifndef AS_Rect_DEFINED
#define AS_Rect_DEFINED

struct AS_Rect //! rectangle
{
	SInt32 top;		//!< top coordinate
	SInt32 left;	//!< left coordinate
	SInt32 bottom;	//!< bottom coordinate
	SInt32 right;	//!< right coordinate
};

#endif //AS_Rect_DEFINED

#ifndef AS_String_DEFINED
#define AS_String_DEFINED

// AS_String: Generic string type to pass to AS_ functions.
// Supports 3 encoding types:
// ASCII – Use this type only if you are certain the string cannot contain
//         any characters that are not in the 7-bit ASCII character set.
// UTF-8 – This can contain any Unicode character and is a consistent string
//         cross-platform.  You must ensure you are using the correct
//         locale/code page to convert or display this string type.  This
//         is useful for strings that are saved to a file intended to be
//         compatible cross-platform. A single Unicode code point is a
//         variable length of 1 to 4 bytes.
// Wide  - This can contain any Unicode character in a wchar_t type.  The
//         Unicode type depends on the platform your program is compiled
//         for (could be UTF-16LE, UTF-16BE, UTF-32LE, or UTF-32BE), but
//         it is useful because C has calls to natively use wchar_t type
//         regardless of the current local/code page.  You should be
//         aware of the character set used for this type if you intend
//         to share this string cross-platform.  For UTF-32, a single
//         Unicode code point is always 4 bytes.  For UTF-16, a single
//         Unicode code point is either 2 or 4 bytes.
// See www.unicode.org for more details on Unicode character encoding.

struct AS_String //! string
{
	enum Type //! encoding type
	{
		Type_ASCII = 1,	//!< local encoding (8-bit)
		Type_UTF8 = 2,	//!< Unicode (8-bit)
		Type_Wide = 3,	//!< Unicode (wchar_t)
	};

	Type type;		//!< encoding type
	size_t size;	//!< size of data buffer in encoded characters
	void * data;	//!< pointer to data buffer
};

#endif //AS_String_DEFINED

#ifndef AS_Time_DEFINED
#define AS_Time_DEFINED

struct AS_Time //! time specification
{
	enum Unit //! time unit
	{
		Unit_Frames,	//!< video frames
		Unit_Seconds	//!< seconds
	};

	Unit unit; //!< time unit
	union
	{
		SInt64 frames;	//!< time in video frames
		double seconds;	//!< time in seconds
	};
};

#endif //AS_Time_DEFINED

#endif //__AS_BaseTypes_h__
