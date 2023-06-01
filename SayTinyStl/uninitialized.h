#ifndef SAYTINYSTL_UNINITIALIZED_H
#define SAYTINYSTL_UNINITIALIZED_H

// 这个头文件用于对未初始化空间构造元素

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace saystl {


	// 由五个全局函数作用于未初始化空间  分别是 
	// construct(), destroy(), uninitialized_copy(), uninitialized_fill(), uninitialized_fill_n()
	// 以下的分别对应于高层次函数 copy(), fill(), fill_n()
		// uninitialized_copy, true_type 平凡类型, 默认构造
		// uninitialized_copy  帮助实现容器: 1.配置内存区块，足以包含所有元素  2.在该区块上构造元素
	template <class InputIter, class ForwardIter>
	ForwardIter
		unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type) {
		return saystl::copy(first, last, result);
	}

	// 非平凡的对象，调用construct
	template <class InputIter, class ForwardIter>
	ForwardIter
		unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::false_type) {
		auto cur = result;
		try {
			for (; first != last; ++first, ++cur) {
				saystl::construct(&*cur, *first);
			}
		}
		catch (...) {
			for (; result != cur; --cur) {
				saystl::destroy(&*cur);
			}
		}
		return cur;
	}

	// 通过萃取机来判断调用 平凡类型/非平凡数据类型
	template <class InputIter, class ForwardIter>
	ForwardIter
		uninitialized_copy(InputIter first, InputIter last, ForwardIter result) {
		return saystl::unchecked_uninit_copy(first, last, result, std::is_trivially_copy_assignable<
			typename iterator_traits<ForwardIter>::
			value_type>{});
	}

	// unintialized_copy_n 赋值 n 个内容到 result 起始处的位置，返回复制结束的位置
	template <class InputIter, class Size, class ForwardIter>
	ForwardIter
		unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::true_type) {
		return saystl::copy_n(first, n, result).second;
	}

	template <class InputIter, class Size, class ForwardIter>
	ForwardIter
		unchecked_uninit_copy_n(InputIter first, Size n, ForwardIter result, std::false_type) {
		auto cur = result;
		try {
			for (; n > 0; --n, ++cur, ++first)
			{
				saystl::construct(&*cur, *first);
			}
		}
		catch (...) {
			for (; result != cur; --cur)
				saystl::destroy(&*cur);
		}
		return cur;
	}

	template <class InputIter, class Size, class ForwardIter>
	ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
		return saystl::unchecked_uninit_copy_n(first, n, result,
			std::is_trivially_copy_assignable<
			typename iterator_traits<InputIter>::
			value_type>{});
	}

	// uninitialized_fill
	// 在 [first, last) 区间内填充元素值
	template <class ForwardIter, class T>
	void
		unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::true_type) {
		saystl::fill(first, last, value);
	}

	template <class ForwardIter, class T>
	void
		unchecked_uninit_fill(ForwardIter first, ForwardIter last, const T& value, std::false_type) {
		auto cur = first;
		try {
			for (; cur != last; ++cur) {
				saystl::construct(&*cur, value);
			}
		}
		catch (...) {
			for (; first != cur; ++first)
				saystl::destroy(&*first);
		}
	}

	template <class ForwardIter, class T>
	void  uninitialized_fill(ForwardIter first, ForwardIter last, const T& value) {
		saystl::unchecked_uninit_fill(first, last, value,
			std::is_trivially_copy_assignable<
			typename iterator_traits<ForwardIter>::
			value_type>{});
	}


	// uninitialized_fill_n
	// 从 first 位置开始，填充 n 个元素值，返回填充结束的位置
	template <class ForwardIter, class Size, class T>
	ForwardIter
		unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::true_type) {
		return saystl::fill_n(first, n, value);
	}

	template <class ForwardIter, class Size, class T>
	ForwardIter
		unchecked_uninit_fill_n(ForwardIter first, Size n, const T& value, std::false_type) {
		auto cur = first;
		try
		{
			for (; n > 0; --n, ++cur)
			{
				saystl::construct(&*cur, value);
			}
		}
		catch (...)
		{
			for (; first != cur; ++first)
				saystl::destroy(&*first);
		}
		return cur;
	}

	//萃取机，判断是否为 POD (Plain Old Data)
	template <class ForwardIter, class Size, class T>
	inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
		return saystl::unchecked_uninit_fill_n(first, n, value,
			std::is_trivially_copy_assignable<
			typename iterator_traits<ForwardIter>::
			value_type>{});
	}

	// uninitialized_move
	// 把[first, last)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
	template <class InputIter, class ForwardIter>
	ForwardIter unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type) {
		return saystl::move(first, last, result); // algobase.h 中的 move 重载函数
	}

	template <class InputIter, class ForwardIter>
	ForwardIter unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::false_type) {
		auto cur = result;
		try {
			for (; first != last; ++first, ++cur) {
				saystl::construct(&*cur, saystl::move(*first));
			}
		}
		catch (...) {
			saystl::destroy(result, cur);
		}
		return cur;
	}

	//萃取机，判断是否为 POD (Plain Old Data)
	template <class InputIter, class ForwardIter>
	ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result)
	{
		return saystl::unchecked_uninit_move(first, last, result,
			std::is_trivially_move_assignable<
			typename iterator_traits<InputIter>::
			value_type>{});
	}

	// uninitialized_move_n
	// 把[first, first + n)上的内容移动到以 result 为起始处的空间，返回移动结束的位置
	template <class InputIter, class Size, class ForwardIter>
	ForwardIter
		unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::true_type)
	{
		return saystl::move(first, first + n, result);
	}

	template <class InputIter, class Size, class ForwardIter>
	ForwardIter
		unchecked_uninit_move_n(InputIter first, Size n, ForwardIter result, std::false_type)
	{
		auto cur = result;
		try
		{
			for (; n > 0; --n, ++first, ++cur)
			{
				saystl::construct(&*cur, saystl::move(*first));
			}
		}
		catch (...)
		{
			for (; result != cur; ++result)
				saystl::destroy(&*result);
			throw;
		}
		return cur;
	}

	//萃取机，判断是否为 POD (Plain Old Data)
	template <class InputIter, class Size, class ForwardIter>
	ForwardIter uninitialized_move_n(InputIter first, Size n, ForwardIter result)
	{
		return saystl::unchecked_uninit_move_n(first, n, result,
			std::is_trivially_move_assignable<
			typename iterator_traits<InputIter>::
			value_type>{});
	}

} // namespace saystl

#endif // !SAYTINYSTL_UNINITIALIZED_H


