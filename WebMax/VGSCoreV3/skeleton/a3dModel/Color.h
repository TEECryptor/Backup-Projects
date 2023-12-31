/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef __XEVOL3D_COLOR_H__
#define __XEVOL3D_COLOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma pack(push,1)

class  xColor_4ub;
class  xColor_3ub;

template <typename T> T _CLAM_TO_(T v , T a , T b)
{
	if( v < a ) return a ;
	if( v > b ) return b;
	return v;
}

template <typename T> T TMin(const T& v1 , const T& v2)
{
	return v1 < v2?v1:v2;
}

template <typename T> T TMax(const T& v1 , const T& v2)
{
	return v1 > v2?v1:v2;
}

class  xColor_4f
{
public:
	float r,g,b,a;

	xColor_4f(){}
	xColor_4f(float _r, float _g,float _b,float _a) {
		r = _r; g = _g ; b = _b; a = _a;
	}
	xColor_4f operator = (const xColor_4ub& rhv);
	xColor_4f(const xColor_4ub& rhv);
	xColor_4f operator = (const xColor_3ub& rhv);
	xColor_4f(const xColor_3ub& rhv);
	static xColor_4f fromByte(BYTE r, BYTE g, BYTE b, BYTE a = 255){
		return xColor_4f(r/255.f,g/255.f,b/255.f,a/255.f);
	}

	xColor_4f  operator +(const xColor_4f& rhv)
	{
		xColor_4f outcl;
		outcl.r = TMin(1.0f,r + rhv.r);
		outcl.g = TMin(1.0f,g + rhv.g);
		outcl.b = TMin(1.0f,b + rhv.b);
		outcl.a = TMin(1.0f,a + rhv.a);
		return outcl;
	}

	xColor_4f  operator -(const xColor_4f& rhv)
	{
		xColor_4f outcl;
		outcl.r = TMax(0.0f,r - rhv.r);
		outcl.g = TMax(0.0f,g - rhv.g);
		outcl.b = TMax(0.0f,b - rhv.b);
		outcl.a = TMax(0.0f,a - rhv.a);
		return outcl;
	}

	xColor_4f& operator +=(const xColor_4f& rhv)
	{
		r = TMin(1.0f,r + rhv.r);
		g = TMin(1.0f,g + rhv.g);
		b = TMin(1.0f,b + rhv.b);
		a = TMin(1.0f,a + rhv.a);
		return *this;
	}

	xColor_4f& operator -=(const xColor_4f& rhv)
	{
		r = TMax(0.0f,r - rhv.r);
		g = TMax(0.0f,g - rhv.g);
		b = TMax(0.0f,b - rhv.b);
		a = TMax(0.0f,a - rhv.a);
		return *this;
	}
	xColor_4f& operator *=(float scale)
	{
		r = TMax(0.0f,scale * r);
		g = TMax(0.0f,scale * g);
		b = TMax(0.0f,scale * b);
		a = TMax(0.0f,scale * a);
		return *this;
	}
	xColor_4f operator *(float scale)
	{
		xColor_4f _ret;
		_ret.r = TMax(0.0f,scale * r);
		_ret.g = TMax(0.0f,scale * g);
		_ret.b = TMax(0.0f,scale * b);
		_ret.a = TMax(0.0f,scale * a);
		return _ret;
	}
	inline friend bool operator ==(const xColor_4f& c1, const xColor_4f& c2){
		return c1.a == c2.a && c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
	}
	inline friend bool operator !=(const xColor_4f& c1, const xColor_4f& c2){
		return c1.a != c2.a || c1.r != c2.r || c1.g != c2.g || c1.b != c2.b;
	}

	float abs()
	{
		return  (0.3f * r + 0.59f * g + 0.11f * b);
	}

};


#if !defined(_MSC_VER) || defined(_WIN32_WCE)



#endif

class  xColor_4ub
{
public:
	union
	{
		union
		{
			struct 
			{
#ifdef _BIG_ENDIAN_
				BYTE a,b,g,r;
#else
				BYTE r,g,b,a;
#endif
			};
			struct 
			{
#ifdef _BIG_ENDIAN_
                char sa,sb,sg,sr;
#else
                char sr,sg,sb,sa;
#endif
			};
		};

		unsigned long color;
	};

	xColor_4ub operator = (const xColor_4f& rhv)
	{
		r = (BYTE)(rhv.r * 255);
		g = (BYTE)(rhv.g * 255);
		b = (BYTE)(rhv.b * 255);
		a = (BYTE)(rhv.a * 255);
		return * this;

	}
	xColor_4ub(const xColor_4f& rhv)
	{
		r = (BYTE)(rhv.r * 255);
		g = (BYTE)(rhv.g * 255);
		b = (BYTE)(rhv.b * 255);
		a = (BYTE)(rhv.a * 255);         
	}

	xColor_4ub operator = (const xColor_3ub& rhv);
	xColor_4ub(const xColor_3ub& rhv);



public:
	xColor_4ub(){}
	xColor_4ub(BYTE _r,BYTE _g , BYTE _b,BYTE _a = 255):r(_r),g(_g),b(_b),a(_a) {};
    template<typename T> void swap(T&v1 , T& v2)
    {
        T temp = v1;
        v1 = v2;
        v2 = temp;
    };

	void        swapEndian()
	{
		swap(r , a);
        swap(b , g);
	}

    void        swapRB()
    {
        swap(r , b);
    }

	xColor_4ub(float _r,float _g , float _b , float _a  )
	{
		r = (BYTE) ( _CLAM_TO_(  _r , 0.0f, 1.0f) * 255);
		g = (BYTE) ( _CLAM_TO_(  _g , 0.0f, 1.0f) * 255);
		b = (BYTE) ( _CLAM_TO_(  _b , 0.0f, 1.0f) * 255);
		a = (BYTE) ( _CLAM_TO_(  _a , 0.0f, 1.0f) * 255);
	}

	xColor_4ub  operator +(const xColor_4ub& rhv)
	{
		xColor_4ub outcl;
#if defined(_MSC_VER) && !defined(_WIN32_WCE) && !defined(_WIN64)
		__asm
		{
			MOVD mm0 , this;
			PADDUSB mm0,rhv;
			MOVD outcl,mm0;
		}
#else
		outcl.r = TMin(255,r + int(rhv.r));
		outcl.g = TMin(255,g + int(rhv.g));
		outcl.b = TMin(255,b + int(rhv.b));
		outcl.a = TMin(255,a + int(rhv.a));
#endif
		return outcl;
	}

	xColor_4ub  operator -(const xColor_4ub& rhv)
	{
		xColor_4ub outcl;
#if defined(_MSC_VER) && !defined(_WIN32_WCE) && !defined(_WIN64)
		__asm
		{
			MOVD mm0 , this;
			PSUBUSB mm0,rhv;
			MOVD outcl,mm0;
		}
#else
		outcl.r = TMax(0,r - int(rhv.r));
		outcl.g = TMax(0,g - int(rhv.g));
		outcl.b = TMax(0,b - int(rhv.b));
		outcl.a = TMax(0,a - int(rhv.a));
#endif
		return outcl;
	}

	xColor_4ub& operator +=(const xColor_4ub& rhv)
	{
#if defined(_MSC_VER) && !defined(_WIN32_WCE) && !defined(_WIN64)
		__asm
		{
			MOVD mm0 , this;
			PADDUSB mm0,rhv;
			MOVD this,mm0;
		}
#else
		r = TMin(255,r + int(rhv.r));
		g = TMin(255,g + int(rhv.g));
		b = TMin(255,b + int(rhv.b));
		a = TMin(255,a + int(rhv.a));
#endif
		return *this;
	}

	xColor_4ub& operator -=(const xColor_4ub& rhv)
	{
#if defined(_MSC_VER) && !defined(_WIN32_WCE) && !defined(_WIN64)
		__asm
		{
			MOVD mm0 , this;
			PSUBUSB mm0,rhv;
			MOVD this,mm0;
		}
#else
		r = TMax(0,r - int(rhv.r));
		g = TMax(0,g - int(rhv.g));
		b = TMax(0,b - int(rhv.b));
		a = TMax(0,a - int(rhv.a));
#endif
		return *this;
	}

	BYTE abs()
	{
		int n = int(0.3f * r + 0.59f * g + 0.11f * b);
		return (BYTE)n;
	}
};

class  xColor_3ub
{
public:
	union
	{
		struct  
		{
			BYTE r,g,b;
		};

	};

	xColor_3ub operator = (const xColor_4f& rhv)
	{
		r = (BYTE)(rhv.r * 255);
		g = (BYTE)(rhv.g * 255);
		b = (BYTE)(rhv.b * 255);
		return * this;
	}

	xColor_3ub(const xColor_4f& rhv)
	{
		r = (BYTE)(rhv.r * 255);
		g = (BYTE)(rhv.g * 255);
		b = (BYTE)(rhv.b * 255);
	}

	xColor_3ub operator = (const xColor_4ub& rhv)
	{
		r = (rhv.r);
		g = (rhv.g);
		b = (rhv.b);
		return * this;
	}

	xColor_3ub(const xColor_4ub& rhv)
	{
		r = (rhv.r);
		g = (rhv.g);
		b = (rhv.b);
	}


public:
	xColor_3ub(){}
	xColor_3ub(BYTE _r,BYTE _g , BYTE _b):r(_r),g(_g),b(_b){};
	void        swapEndian()
	{
		BYTE t = r;
		r = b;
		b = t;
	}

	xColor_3ub(float _r, float _g , float _b , float _a)
	{
		r = (BYTE) ( _CLAM_TO_(  _r , 0.0f, 1.0f) * 255);
		g = (BYTE) ( _CLAM_TO_(  _g , 0.0f, 1.0f) * 255);
		b = (BYTE) ( _CLAM_TO_(  _b , 0.0f, 1.0f) * 255);
	}

	xColor_3ub  operator +(const xColor_3ub& rhv)
	{
		xColor_3ub outcl;
		outcl.r = TMin(255,r + int(rhv.r));
		outcl.g = TMin(255,g + int(rhv.g));
		outcl.b = TMin(255,b + int(rhv.b));
		return outcl;
	}

	xColor_3ub  operator -(const xColor_3ub& rhv)
	{
		xColor_3ub outcl;
		outcl.r = TMax(0,r - int(rhv.r));
		outcl.g = TMax(0,g - int(rhv.g));
		outcl.b = TMax(0,b - int(rhv.b));
		return outcl;
	}

	xColor_3ub& operator +=(const xColor_3ub& rhv)
	{
		r = TMin(255,r + int(rhv.r));
		g = TMin(255,g + int(rhv.g));
		b = TMin(255,b + int(rhv.b));
		return *this;
	}

	xColor_3ub& operator -=(const xColor_3ub& rhv)
	{
		r = TMax(0,r - int(rhv.r));
		g = TMax(0,g - int(rhv.g));
		b = TMax(0,b - int(rhv.b));
		return *this;
	}

	BYTE abs()
	{
		int n = int(0.3f * r + 0.59f * g + 0.11f * b);
		return (BYTE)n;
	}
};

inline xColor_4f xColor_4f::operator = (const xColor_4ub& rhv)
{
	r = rhv.r /255.0f;
	g = rhv.g /255.0f;
	b = rhv.b /255.0f;
	a = rhv.a /255.0f;
	return *this;
}

inline xColor_4f xColor_4f::operator = (const xColor_3ub& rhv)
{
	r = rhv.r /255.0f;
	g = rhv.g /255.0f;
	b = rhv.b /255.0f;
	a = 1.0f;
	return *this;
}

inline xColor_4f::xColor_4f(const xColor_4ub& rhv)
{
	r = rhv.r /255.0f;
	g = rhv.g /255.0f;
	b = rhv.b /255.0f;
	a = rhv.a /255.0f;     
}

inline xColor_4f::xColor_4f(const xColor_3ub& rhv)
{
	r = rhv.r /255.0f;
	g = rhv.g /255.0f;
	b = rhv.b /255.0f;
	a = 1.0f;     
}


inline xColor_4ub xColor_4ub::operator = (const xColor_3ub& rhv)
{
	r = rhv.r;
	g = rhv.g;
	b = rhv.b;
	a = 255;
	return *this;
}


inline xColor_4ub::xColor_4ub(const xColor_3ub& rhv)
{
	r = rhv.r;
	g = rhv.g;
	b = rhv.b;
	a = 255;     
}


#pragma pack(pop)
#endif
