#ifndef SAYSTL_ALGOBASE_H
#define SAYSTL_ALGOBASE_H

// 这个头文件包含了 saystl 的基本算法

#include <cstring>

#include "iterator.h"
#include "util.h"

namespace saystl {

#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif // min

	//  equal
	//  使用该算法时， 必须先判断其元素个数是否相同
	//	比较第一序列在[first, last)区间上的元素值是否和第二序列相等
	template <class InputIter1, class InputIter2>
	inline bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2) {
		//如果序列 1 的元素个数多于 序列 2 的元素个数， 会出错
		for (; first1 != last1; ++first1, ++first2)
			if (*first1 != *first2)
				return false;
		return true;
	}
	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIter1, class InputIter2, class Compared>
	inline bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compared comp)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!comp(*first1, *first2))
				return false;
		}
		return true;
	}

	// max
	// 取二者的较大值，相等时返回的时第一个参数
	// 版本一  使用对象型别 T 所提供的 greater-than 操作符来判断大小
	template <class T>
	inline const T& max(const T& lhs, const T& rhs) {
		return lhs < rhs ? rhs : lhs;
	}

	// 版本二  使用仿函数 cmp 来判断大小
	template <class T, class Compare>
	inline const T& max(const T& lhs, const T& rhs, Compare comp) {
		return comp(lhs, rhs) ? rhs : lhs;
	}

	// min 
	// 取二者的较小值，相等时返回的时第一个参数
	// 版本一  使用对象型别 T 所提供的 less-than 操作符来判断大小
	template <class T>
	inline const T& min(const T& lhs, const T& rhs) {
		return rhs < lhs ? rhs : lhs;
	}

	// 版本二  使用仿函数 cmp 来判断大小
	template <class T, class Compare>
	inline const T& min(const T& lhs, const T& rhs, Compare comp) {
		return comp(rhs, lhs) ? rhs : lhs;
	}


	// iterator_swap
	// 将两个迭代器所指的对象对调
	template <class FirstIter1, class FirstIter2>
	void iter_swap(FirstIter1 lhs, FirstIter2 rhs) {
		saystl::swap(*lhs, *rhs);
	}

	//======================================================================================================
	/*                                   copy

			   copy( 迭代器版本 )   --->  unchecked_copy_cat (3个版本)  --->  n 来决定速度(更快)
								   --->  unchecked_copy  (指针特化版本) --->  memmove

	 把 [first, last) 区间的元素拷贝到 [result, result + (last - first)) 内 返回 resul最后的位置    */

	 // input_iterator_tag 版本
	template <class InputIter, class OutputIter>
	inline OutputIter
		unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
			saystl::input_iterator_tag) {
		for (; first != last; ++first, ++result) {
			*result = *first;
		}
		return result;
	}

	// ramdom_access_iterator_tag 版本
	template <class InputIter, class OutputIter>
	inline OutputIter
		unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
			saystl::random_access_iterator_tag) {
		for (auto n = last - first; n > 0; --n, ++first, ++result) { // 以 n 决定循环的执行次数。 速度快
			*result = *first;
		}
		return result;
	}

	// 传递给上述重载版本， 让编译器决定使用哪个模板函数
	template <class InputIter, class OutputIter>
	inline OutputIter
		unchecked_copy(InputIter first, InputIter last, OutputIter result) {
		return unchecked_copy_cat(first, last, result, iterator_category(first));
	}


	// 为 trivially_copy_assignable 类型提供特化版本
	template <class Tp, class Up>
	typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_copy_assignable<Up>::value, Up*>::type
		unchecked_copy(Tp* first, Tp* last, Up* result) {
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(Up)); // 调用 memmove 先将输入区间整体复制下来，没有覆盖的危险
		return result + n;
	}

	// 传递给上述unchecked_copy重载版本， 让编译器决定使用 trivially 或是 Iterator 模板函数
	template <class InputIter, class OutputIter>
	OutputIter copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy(first, last, result);
	}
	//======================================================================================================

	//======================================================================================================
	/*                             copy_backward
	 unchecked_copy_backward(迭代器版本) --->  unchecked_copy_backward_cat (3个版本)  --->  n 来决定速度(更快)
										--->  unchecked_copy_backward  (指针特化版本) --->  memmove

	  将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
	  以逆行的方向复制到以 result-1 为起点，方向亦为逆行的区间上
	*/

	// unchecked_copy_backward_cat 的两个版本
	// bidirectional_iterator_tag 版本
	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result, saystl::bidirectional_iterator_tag) {
		while (first != last)
			*--result = *--last;
		return result;
	}

	// random_access_iterator_tag 版本
	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result, saystl::random_access_iterator_tag) {
		for (auto n = last - first; n > 0; --n) {
			*--result = *--last;
		}
		return result;
	}

	// unchecked_copy_backward 的三个版本
	// 1 这个会传递到 unchecked_copy_backward_cat 进行模板函数推导
	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result) {
		return unchecked_copy_backward_cat(first, last, result, iterator_category(first));
	}

	// 2 unchecked_copy_backward 的平凡数据类型 trivially_copy_assignable 特化版本
	template <class Tp, class Up>
	inline typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_copy_assignable<Up>::value, Up*>::type
		unchecked_copy_backward(Tp* first, Tp* last, Up* result) {
		const auto n = static_cast<size_t>(last - first);
		if (n != 0) {
			result -= n;
			std::memmove(result, first, n * sizeof(Up));
		}
		return result;
	}

	// 3 编译器自己决定传给 1 或者 2
	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
		copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		return unchecked_copy_backward(first, last, result);
	}
	//======================================================================================================

	//======================================================================================================
	/*                                           copy_if
			   把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上
	*/
	template <class InputIter, class OutputIter, class UnaryPredicate>
	inline OutputIter
		copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred)
	{
		for (; first != last; ++first)
		{
			if (unary_pred(*first))
				*result++ = *first;
		}
		return result;
	}
	//======================================================================================================

	//======================================================================================================
	/*                                           copy_n
		把 [first, first + n)区间上的元素拷贝到 [result, result + n)上, 返回一个 pair 分别指向拷贝结束的尾部
	*/
	template <class InputIter, class Size, class OutputIter>
	inline saystl::pair<InputIter, OutputIter>
		unchecked_copy_n(InputIter first, Size n, OutputIter result, saystl::input_iterator_tag)
	{
		for (; n > 0; --n, ++first, ++result)
		{
			*result = *first;
		}
		return saystl::pair<InputIter, OutputIter>(first, result);
	}

	template <class RandomIter, class Size, class OutputIter>
	inline saystl::pair<RandomIter, OutputIter>
		unchecked_copy_n(RandomIter first, Size n, OutputIter result,
			saystl::random_access_iterator_tag)
	{
		auto last = first + n;
		return saystl::pair<RandomIter, OutputIter>(last, saystl::copy(first, last, result));
	}

	template <class InputIter, class Size, class OutputIter>
	inline saystl::pair<InputIter, OutputIter>
		copy_n(InputIter first, Size n, OutputIter result)
	{
		return unchecked_copy_n(first, n, result, iterator_category(first));
	}
	//======================================================================================================

	//======================================================================================================
	/*                                           move
					 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
	*/
	template <class InputIter, class OutputIter>
	inline OutputIter
		unchecked_move_cat(InputIter first, InputIter last, OutputIter result,
			saystl::input_iterator_tag)
	{
		for (; first != last; ++first, ++result)
		{
			*result = saystl::move(*first);
		}
		return result;
	}

	// ramdom_access_iterator_tag 版本
	template <class RandomIter, class OutputIter>
	inline OutputIter
		unchecked_move_cat(RandomIter first, RandomIter last, OutputIter result,
			saystl::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n, ++first, ++result)
		{
			*result = saystl::move(*first);
		}
		return result;
	}

	template <class InputIter, class OutputIter>
	inline OutputIter
		unchecked_move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move_cat(first, last, result, iterator_category(first));
	}

	// 为 trivially_copy_assignable 类型提供特化版本
	template <class Tp, class Up>
	inline typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_move_assignable<Up>::value,
		Up*>::type
		unchecked_move(Tp* first, Tp* last, Up* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(Up));
		return result + n;
	}

	template <class InputIter, class OutputIter>
	inline OutputIter move(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_move(first, last, result);
	}
	//======================================================================================================

	//======================================================================================================
	/*                                        move_backward
					 将 [first, last)区间内的元素移动到 [result - (last - first), result)内
	*/
	// bidirectional_iterator_tag 版本
	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result, saystl::bidirectional_iterator_tag)
	{
		while (first != last)
			*--result = saystl::move(*--last);
		return result;
	}

	// random_access_iterator_tag 版本
	template <class RandomIter1, class RandomIter2>
	inline RandomIter2
		unchecked_move_backward_cat(RandomIter1 first, RandomIter1 last,
			RandomIter2 result, saystl::random_access_iterator_tag)
	{
		for (auto n = last - first; n > 0; --n)
			*--result = saystl::move(*--last);
		return result;
	}

	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_move_backward(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result)
	{
		return unchecked_move_backward_cat(first, last, result,
			iterator_category(first));
	}

	// 为 trivially_copy_assignable 类型提供特化版本
	template <class Tp, class Up>
	inline typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_move_assignable<Up>::value,
		Up*>::type
		unchecked_move_backward(Tp* first, Tp* last, Up* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n != 0)
		{
			result -= n;
			std::memmove(result, first, n * sizeof(Up));
		}
		return result;
	}

	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		move_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		return unchecked_move_backward(first, last, result);
	}
	//======================================================================================================


	// fill_n
	// 将 [first, last) 的前 n 个元素改填新值
	// 返回的迭代器指向被填入的最后一个元素的下一个位置
	template <class OutputIter, class Size, class T>
	inline OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value) {
		for (; n > 0; --n, ++first) {
			*first = value;
		}
		return first;
	}

	// 为 one-byte 类型提供特化版本
	template <class Tp, class Size, class Up>
	inline typename std::enable_if<
		std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
		!std::is_same<Tp, bool>::value&&
		std::is_integral<Up>::value && sizeof(Up) == 1,
		Tp*>::type
		unchecked_fill_n(Tp* first, Size n, Up value)
	{
		if (n > 0)
		{
			std::memset(first, (unsigned char)value, (size_t)(n));
		}
		return first + n;
	}

	template <class OutputIter, class Size, class T>
	inline OutputIter fill_n(OutputIter first, Size n, const T& value)
	{
		return unchecked_fill_n(first, n, value);
	}

	// fill
	// 将 [first, last) 的所有元素改填新值
	template <class ForwardIter, class T>
	void fill_cat(ForwardIter first, ForwardIter last, const T& value,
		saystl::forward_iterator_tag)
	{
		for (; first != last; ++first)
		{
			*first = value;
		}
	}

	template <class RandomIter, class T>
	void fill_cat(RandomIter first, RandomIter last, const T& value,
		saystl::random_access_iterator_tag)
	{
		fill_n(first, last - first, value);
	}

	template <class ForwardIter, class T>
	void fill(ForwardIter first, ForwardIter last, const T& value)
	{
		fill_cat(first, last, value, iterator_category(first));
	}

	//======================================================================================================
	/*                                  lexicographical_compare
			 以字典序排列对两个序列 [first1, last1) 和 [first2, last2) 对应位置元素进行比较，直到下列几种情况：
		  1) 如果第一序列的元素较小，返回 true ，否则返回 false
		  2) 如果到达 last1 而尚未到达 last2 返回 true
		  3) 如果到达 last2 而尚未到达 last1 返回 false
		  4) 如果同时到达 last1 和 last2 返回 false
			 也就是说，第一序列以字典排列方式 (lexicographical) 不小于第二序列
	*/
	template <class InputIter1, class InputIter2>
	bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
		InputIter2 first2, InputIter1 last2) {
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {

			if (*first1 < *first2) // 第一序列的元素小于第二序列元素
				return true;
			if (*first2 < *first1)
				return false;
			// 如果不符合以上条件，表示两值相等，那就进行一下组相应元素对比
		}
		// 如果进行到这里，如果第一序列到达尾端而第二序列尚有余，那么第一序列小于第二序列
		return first1 == last1 && first2 != last2;
	}

	template <class InputIter1, class InputIter2, class Compare>
	bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
		InputIter2 first2, InputIter1 last2, Compare comp) {
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {

			if (comp(*first1, *first2))
				return true;
			if (comp(*first2, *first1))
				return false;
		}
		return first1 == last1 && first2 != last2;
	}

	// 针对原生指针 const unsigned char* 的特化版本
	bool lexicographical_compare(const unsigned char* first1,
		const unsigned char* last1,
		const unsigned char* first2,
		const unsigned char* last2)
	{
		const auto len1 = last1 - first1;
		const auto len2 = last2 - first2;
		// 先比较相同长度的部分
		const auto result = std::memcmp(first1, first2, saystl::min(len1, len2));
		// 若相等，长度较长的比较大
		return result != 0 ? result < 0 : len1 < len2;
	}

	// mismatch 失配
	// 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的不匹配点
	// 如果都匹配， 则返回两序列各自的 last 迭代器
	template <class InputIter1, class InputIter2>
	saystl::pair<InputIter1, InputIter2> mismatch(InputIter1 first1, InputIter1 last1,
		InputIter2 first2) {
		while (first1 != last1 && *first1 == *first2) {
			++first1;
			++first2;
		}
		return saystl::pair<InputIter1, InputIter2>(first1, first2);
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class InputIter1, class InputIter2, class Compred>
	saystl::pair<InputIter1, InputIter2>
		mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, Compred comp) {
		while (first1 != last1 && comp(*first1, *first2)) {
			++first1;
			++first2;
		}
		return saystl::pair<InputIter1, InputIter2>(first1, first2);
	}

} // namespace saystl

#endif // !SAYSTL_ALGOBASE_H

