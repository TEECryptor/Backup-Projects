#ifndef __MY_STL_HPP__
#define __MY_STL_HPP__

/** 说明：
	1.本类用于提供一些STL未实现的常用功能;
	author:林秀全
	date:2009-4-10
*/
#include <memory>

namespace MyStd
{
	// 一个copy_if的正确实现
	template<typename InputIterator, typename OutputIterator, typename Predicate>
	OutputIterator copy_if(InputIterator begin,
		InputIterator end,
		OutputIterator destBegin,
		Predicate p) 
	{
		while (begin != end) {
			if (p(*begin))*destBegin++ = *begin;
			++begin;
		}
		return destBegin;
	}

}

#endif
