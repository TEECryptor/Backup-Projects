#ifndef __MY_STL_HPP__
#define __MY_STL_HPP__

/** ˵����
	1.���������ṩһЩSTLδʵ�ֵĳ��ù���;
	author:����ȫ
	date:2009-4-10
*/
#include <memory>

namespace MyStd
{
	// һ��copy_if����ȷʵ��
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
