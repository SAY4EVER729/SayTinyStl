#ifndef SAYTINYSTL_ALGO_H
#define SAYTINYSTL_ALGO_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

// ���ͷ�ļ������� saystl ��һϵ���㷨

#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "memory.h"
#include "heap_algo.h"
#include "functional.h"

namespace saystl {

	// all_of
	// ���[first, last)���Ƿ�ȫ��Ԫ�ض�����һԪ���� unary_pred Ϊ true ������������򷵻� true
	template <class InputIter, class UnaryPredicate>
	bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (!unary_pred(*first))
				return false;
		}
		return true;
	}

	// any_of
	// ���[first, last)���Ƿ����ĳ��Ԫ������һԪ���� unary_pred Ϊ true ������������򷵻� true
	template <class InputIter, class UnaryPredicate>
	bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (unary_pred(*first))
				return true;
		}
		return false;
	}

	// none_of
	// ���[first, last)���Ƿ�ȫ��Ԫ�ض�������һԪ���� unary_pred Ϊ true ������������򷵻� true
	template <class InputIter, class UnaryPredicate>
	bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (unary_pred(*first))
				return false;
		}
		return true;
	}

	// count
	// ��[first, last)�����ڵ�Ԫ�������ֵ���бȽϣ�ȱʡʹ�� operator==������Ԫ����ȵĸ���
	template <class InputIter, class T>
	size_t count(InputIter first, InputIter last, const T& value) {
		size_t n = 0;
		for (; first != last; ++first) {
			if (*first == value)
				++n;
		}
		return n;
	}

	// count_if
	// ��[first, last)�����ڵ�ÿ��Ԫ�ض�����һԪ unary_pred ���������ؽ��Ϊ true �ĸ���
	template <class InputIter, class UnaryPredicate>
	size_t count_if(InputIter first, InputIter last, UnaryPredicate unary_pred) {
		size_t n = 0;
		for (; first != last; ++first) {
			if (unary_pred(*first))
				++n;
		}
		return n;
	}

	// find
	// ��[first, last)�������ҵ����� value ��Ԫ�أ�����ָ���Ԫ�صĵ�����, �Ҳ����򷵻� end()
	template <class InputIter, class T>
	InputIter
		find(InputIter first, InputIter last, const T& value) {
		while (first != last && *first != value)
			++first;
		return first;
	}

	// find_if
	// ��[first, last)�������ҵ���һ����һԪ���� unary_pred Ϊ true ��Ԫ�ز�����ָ���Ԫ�صĵ�����
	template <class InputIter, class UnaryPredicate>
	InputIter
		find_if(InputIter first, InputIter last, UnaryPredicate unary_pred) {
		while (first != last && !unary_pred(*first))
			++first;
		return first;
	}

	// find_if_not
	// ��[first, last)�������ҵ���һ����һԪ���� unary_pred Ϊ false ��Ԫ�ز�����ָ���Ԫ�صĵ�����
	template <class InputIter, class UnaryPredicate>
	InputIter
		find_if_not(InputIter first, InputIter last, UnaryPredicate unary_pred) {
		while (first != last && unary_pred(*first))
			++first;
		return first;
	}

	// search
	// ��[first1, last1)�в���[first2, last2)���״γ��ֵ�
	// ���� ����2 �� ����1 ��������
	template <class ForwardIter1, class ForwardIter2>
	ForwardIter1
		search(ForwardIter1 first1, ForwardIter1 last1,
			ForwardIter2 first2, ForwardIter2 last2) {
		auto d1 = saystl::distance(first1, last1);
		auto d2 = saystl::distance(first2, last2);
		if (d1 < d2)                       // ��� ����2 ���� ����1, ��ôֱ�ӷ��أ� ����2 ������ ���� ����1
			return last1;
		auto current1 = first1;
		auto current2 = first2;
		while (current2 != last2) {
			if (*current1 == *current2) {
				++current1;
				++current2;
			}
			else {                         // ������ȣ�����򷵻ص�ǰ last1
				if (d1 == d2) {            // �������������һ����
					return last1;          // ��ʾ�����ܳɹ���     
				}
				else {                     // ����� first2 �����ʼ���������²���
					current1 = ++first1;
					current2 = first2;
					--d1;                  // �Ѿ��ų��� ����1 ��һ��Ԫ�أ����� ����1 �ĳ���Ҫ��һ
				}
			}
		}
		return first1;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class ForwardIter1, class ForwardIter2, class Compared>
	ForwardIter1
		search(ForwardIter1 first1, ForwardIter1 last1,
			ForwardIter2 first2, ForwardIter2 last2, Compared comp)
	{
		auto d1 = saystl::distance(first1, last1);
		auto d2 = saystl::distance(first2, last2);
		if (d1 < d2)
			return last1;
		auto current1 = first1;
		auto current2 = first2;
		while (current2 != last2) {
			if (comp(*current1, *current2)) {
				++current1;
				++current2;
			}
			else {
				if (d1 == d2) {
					return last1;
				}
				else {
					current1 = ++first1;
					current2 = first2;
					--d1;
				}
			}
		}
		return first1;
	}

	// search_n
	// ��[first, last)�в������� n �� value ���γɵ������У�����һ��������ָ��������е���ʼ��
	// ����汾ָ����������� "equality"
	template <class ForwardIter, class Size, class T>
	ForwardIter
		search_n(ForwardIter first, ForwardIter last, Size n, const T& value) {
		if (n <= 0)
			return first;
		else {
			first = saystl::find(first, last, value);         // �����ҳ� value ��һ�γ��ֵ�
			while (first != last) {
				auto m = n - 1;                               // value ��Ӧ�ó��� m ��
				auto i = first;
				++i;
				while (i != last && m != 0 && *i == value) {
					++i;
					--m;
				}
				if (m == 0)                                   // m==0 �����ҵ��ˣ�����ָ����������е���ʼ��
					return first;
				else first = saystl::find(i, last, value);    // û���ҵ�������һ�� value �ĳ��ֵ㣬��׼���ص���ѭ��
			}
			return last;
		}
	}

	// search_n  �ڶ����汾
	// ��[first, last)�в������� n �� value ���γɵ������У�����һ��������ָ��������е���ʼ��
	// ����汾ָ����������� "comp"
	template <class ForwardIter, class Size, class T, class Compared>
	ForwardIter
		search_n(ForwardIter first, ForwardIter last, Size n, const T& value, Compared comp) {
		if (n <= 0)
			return first;
		else {
			while (first != last) {
				if (comp(*first, value))
					break;
				++first;
			}
			while (first != last) {
				auto m = n - 1;
				auto i = first;
				++i;
				while (i != last && m != 0 && comp(*i, value)) {
					++i;
					--m;
				}
				if (m == 0)
					return first;
				else {
					while (i != last) {
						if (comp(*i, value))
							break;
						++i;
					}
					first = i;
				}
			}
			return last;
		}
	}

	// find_end
	// ��[first1, last1)�����в���[first2, last2)���һ�γ��ֵĵط����������ڷ��� last1
	// find_end_dispatch �� forward_iterator_tag �汾
	template <class ForwardIter1, class ForwardIter2>
	ForwardIter1
		find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
			ForwardIter2 first2, ForwardIter2 last2,
			forward_iterator_tag, forward_iterator_tag) {
		if (first2 == last2) {       // �������Ŀ���ǿյ�
			return last1;
		}
		else {
			auto result = last1;
			while (true) {
				// ���� search ����ĳ�������е��״γ��ֵ㣬�Ҳ����򷵻� last1
				auto new_result = saystl::search(first1, last1, first2, last2);
				if (new_result == last1) {         // ������ҵ�Ŀ���ǿյ�
					return result;                 // ���� last1����ʾ�� ���������С� �������ֵ�
				}
				else {
					result = new_result;
					first1 = new_result;
					++first1;                      // ������� ׼����һ�β����ж�
				}
			}
		}
	}

	// find_end_dispatch �� bidirectional_iterator_tag �汾 ( ����������� )
	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter1
		find_end_dispatch(BidirectionalIter1 first1, BidirectionalIter1 last1,
			BidirectionalIter2 first2, BidirectionalIter2 last2,
			bidirectional_iterator_tag, bidirectional_iterator_tag)
	{
		// ���ڲ��ҵ��� "�����ֵ͵�" ��˷�����ұȽϿ�  ���� reverse_iterator
		typedef reverse_iterator<BidirectionalIter1> reviter1;
		typedef reverse_iterator<BidirectionalIter2> reviter2;
		reviter1 rlast1(first1);
		reviter2 rlast2(first2);
		reviter1 rresult = saystl::search(reviter1(last1), rlast1, reviter2(last2), rlast2);
		if (rresult == rlast1)
		{
			return last1;
		}
		else
		{
			auto result = rresult.base();
			saystl::advance(result, -saystl::distance(first2, last2));
			return result;
		}
	}

	template <class ForwardIter1, class ForwardIter2>
	ForwardIter1
		find_end(ForwardIter1 first1, ForwardIter1 last1,
			ForwardIter2 first2, ForwardIter2 last2)
	{
		typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
		typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
		return saystl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2());
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	// find_end_dispatch �� forward_iterator_tag �汾
	template <class ForwardIter1, class ForwardIter2, class Compared>
	ForwardIter1
		find_end_dispatch(ForwardIter1 first1, ForwardIter1 last1,
			ForwardIter2 first2, ForwardIter2 last2,
			forward_iterator_tag, forward_iterator_tag, Compared comp) {
		if (first2 == last2)
			return last1;
		else {
			auto result = last1;
			while (true) {
				// ���� search ����ĳ�������е��״γ��ֵ㣬�Ҳ����򷵻� last1
				auto new_result = saystl::search(first1, last1, first2, last2, comp);
				if (new_result == last1)
					return result;
				else {
					result = new_result;
					first1 = new_result;
					++first1;
				}
			}
		}
	}

	// find_end_dispatch �� bidirectional_iterator_tag �汾
	template <class BidirectionalIter1, class BidirectionalIter2, class Compared>
	BidirectionalIter1
		find_end_dispatch(BidirectionalIter1 first1, BidirectionalIter1 last1,
			BidirectionalIter2 first2, BidirectionalIter2 last2,
			bidirectional_iterator_tag, bidirectional_iterator_tag, Compared comp) {
		typedef reverse_iterator<BidirectionalIter1> reviter1;
		typedef reverse_iterator<BidirectionalIter2> reviter2;
		reviter1 rlast1(first1);
		reviter2 rlast2(first2);
		reviter1 rresult = saystl::search(reviter1(last1), rlast1, reviter2(last2), rlast2, comp);
		if (rresult == rlast1)
			return last1;
		else {
			auto result = rresult.base();
			saystl::advance(result, -saystl::distance(first2, last2));
			return result;
		}
	}

	template <class ForwardIter1, class ForwardIter2, class Compared>
	ForwardIter1
		find_end(ForwardIter1 first1, ForwardIter1 last1,
			ForwardIter2 first2, ForwardIter2 last2, Compared comp) {
		typedef typename iterator_traits<ForwardIter1>::iterator_category Category1;
		typedef typename iterator_traits<ForwardIter2>::iterator_category Category2;
		return saystl::find_end_dispatch(first1, last1, first2, last2, Category1(), Category2(), comp);
	}

	// find_first_of
	// ��[first1, last1)�в���[first2, last2)�е�ĳЩԪ�أ�����ָ���һ�γ��ֵ�Ԫ�صĵ�����
	// �� ����2 �κ�����һ��Ԫ�ص�һ�γ����ڵ�һ���еĵ͵㣬��δ�ҵ����� last1
	template <class InputIter, class ForwardIter>
	InputIter
		find_first_of(InputIter first1, InputIter last1,
			ForwardIter first2, ForwardIter last2) {
		for (; first1 != last1; ++first1) {
			for (auto iter = first2; iter != last2; ++iter) {
				if (*first1 == *iter)
					return first1;
			}
		}
		return last1;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class InputIter, class ForwardIter, class Compared>
	InputIter
		find_first_of(InputIter first1, InputIter last1,
			ForwardIter first2, ForwardIter last2, Compared comp) {
		for (; first1 != last1; ++first1) {
			for (auto iter = first2; iter != last2; ++iter) {
				if (comp(*first1, *iter))
					return first1;
			}
		}
		return last1;
	}

	// for_each
	// ʹ��һ���������� f ��[first, last)�����ڵ�ÿ��Ԫ��ִ��һ�� operator() �����������ܸı�Ԫ������
	// f() �ɷ���һ��ֵ������ֵ�ᱻ����
	template <class InputIter, class Function>
	Function for_each(InputIter first, InputIter last, Function f) {
		for (; first != last; ++first) {
			f(*first);
		}
		return f;
	}

	// adjacent_find
	// �ҳ���һ��ƥ�������Ԫ�أ�ȱʡʹ�� operator== �Ƚϣ�����ҵ�����һ����������ָ�����Ԫ�صĵ�һ��Ԫ��
	template <class ForwardIter>
	ForwardIter adjacent_find(ForwardIter first, ForwardIter last) {
		if (first == last)  return last;
		auto next = first;
		while (++next != last) {
			if (*first == *next)  return first;
			first = next;
		}
		return last;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class ForwardIter, class Compared>
	ForwardIter adjacent_find(ForwardIter first, ForwardIter last, Compared comp) {
		if (first == last)  return last;
		auto next = first;
		while (++next != last) {
			if (comp(*first, *next))  return first;
			first = next;
		}
		return last;
	}

	// lower_bound
	// ��[first, last)�в��ҵ�һ����С�� value ��Ԫ�أ�������ָ�����ĵ���������û���򷵻� last
	// lbound_dispatch �� forward_iterator_tag �汾 ( Ӧ��������ռ� )
	template <class ForwardIter, class T>
	ForwardIter
		lbound_dispatch(ForwardIter first, ForwardIter last,
			const T& value, forward_iterator_tag) {
		auto len = saystl::distance(first, last);
		auto half = len;
		ForwardIter middle;
		while (len > 0) {
			half = len >> 1;
			middle = first;
			saystl::advance(middle, half);
			if (*middle < value) {
				first = middle;
				++first;
				len = len - half - 1;
			}
			else {
				len = half;
			}
		}
		return first;
	}

	// lbound_dispatch �� random_access_iterator_tag �汾
	template <class RandomIter, class T>
	RandomIter
		lbound_dispatch(RandomIter first, RandomIter last,
			const T& value, random_access_iterator_tag) {
		auto len = last - first;
		auto half = len;
		RandomIter middle;
		while (len > 0) {
			half = len >> 1;
			middle = first + half;
			if (*middle < value) {
				first = middle + 1;
				len = len - half - 1;
			}
			else {
				len = half;
			}
		}
		return first;
	}

	template <class ForwardIter, class T>
	ForwardIter
		lower_bound(ForwardIter first, ForwardIter last, const T& value) {
		return saystl::lbound_dispatch(first, last, value, iterator_category(first));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	// lbound_dispatch �� forward_iterator_tag �汾
	template <class ForwardIter, class T, class Compared>
	ForwardIter
		lbound_dispatch(ForwardIter first, ForwardIter last,
			const T& value, forward_iterator_tag, Compared comp) {
		auto len = saystl::distance(first, last);
		auto half = len;
		ForwardIter middle;
		while (len > 0) {
			half = len >> 1;
			middle = first;
			saystl::advance(middle, half);
			if (comp(*middle, value)) {
				first = middle;
				++first;
				len = len - half - 1;
			}
			else {
				len = half;
			}
		}
		return first;
	}

	// lbound_dispatch �� random_access_iterator_tag �汾
	template <class RandomIter, class T, class Compared>
	RandomIter
		lbound_dispatch(RandomIter first, RandomIter last,
			const T& value, random_access_iterator_tag, Compared comp) {
		auto len = last - first;
		auto half = len;
		RandomIter middle;
		while (len > 0) {
			half = len >> 1;
			middle = first + half;
			if (comp(*middle, value)) {
				first = middle + 1;
				len = len - half - 1;
			}
			else {
				len = half;
			}
		}
		return first;
	}

	template <class ForwardIter, class T, class Compared>
	ForwardIter
		lower_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp) {
		return saystl::lbound_dispatch(first, last, value, iterator_category(first), comp);
	}

	// upper_bound
	// ��[first, last)�в��ҵ�һ������value ��Ԫ�أ�������ָ�����ĵ���������û���򷵻� last
	// ubound_dispatch �� forward_iterator_tag �汾
	template <class ForwardIter, class T>
	ForwardIter
		ubound_dispatch(ForwardIter first, ForwardIter last,
			const T& value, forward_iterator_tag) {
		auto len = saystl::distance(first, last);
		auto half = len;
		ForwardIter middle;
		while (len > 0) {
			half = len >> 1;
			middle = first;
			saystl::advance(middle, half);
			if (value < *middle) {
				len = half;
			}
			else {
				first = middle;
				++first;
				len = len - half - 1;
			}
		}
		return first;
	}

	// ubound_dispatch �� random_access_iterator_tag �汾
	template <class RandomIter, class T>
	RandomIter
		ubound_dispatch(RandomIter first, RandomIter last,
			const T& value, random_access_iterator_tag) {
		auto len = last - first;
		auto half = len;
		RandomIter middle;
		while (len > 0) {
			half = len >> 1;
			middle = first + half;
			if (value < *middle) {
				len = half;
			}
			else {
				first = middle + 1;
				len = len - half - 1;
			}
		}
		return first;
	}

	template <class ForwardIter, class T>
	ForwardIter
		upper_bound(ForwardIter first, ForwardIter last, const T& value)
	{
		return saystl::ubound_dispatch(first, last, value, iterator_category(first));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	// ubound_dispatch �� forward_iterator_tag �汾
	template <class ForwardIter, class T, class Compared>
	ForwardIter
		ubound_dispatch(ForwardIter first, ForwardIter last,
			const T& value, forward_iterator_tag, Compared comp) {
		auto len = saystl::distance(first, last);
		auto half = len;
		ForwardIter middle;
		while (len > 0) {
			half = len >> 1;
			middle = first;
			saystl::advance(middle, half);
			if (comp(value, *middle)) {
				len = half;
			}
			else {
				first = middle;
				++first;
				len = len - half - 1;
			}
		}
		return first;
	}

	// ubound_dispatch �� random_access_iterator_tag �汾
	template <class RandomIter, class T, class Compared>
	RandomIter
		ubound_dispatch(RandomIter first, RandomIter last,
			const T& value, random_access_iterator_tag, Compared comp) {
		auto len = last - first;
		auto half = len;
		RandomIter middle;
		while (len > 0) {
			half = len >> 1;
			middle = first + half;
			if (comp(value, *middle)) {
				len = half;
			}
			else {
				first = middle + 1;
				len = len - half - 1;
			}
		}
		return first;
	}

	template <class ForwardIter, class T, class Compared>
	ForwardIter
		upper_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp)
	{
		return saystl::ubound_dispatch(first, last, value, iterator_category(first), comp);
	}

	// binary_search
	// ���ֲ��ң������[first, last)���е�ͬ�� value ��Ԫ�أ����� true�����򷵻� false
	template <class ForwardIter, class T>
	bool binary_search(ForwardIter first, ForwardIter last, const T& value) {
		auto i = saystl::lower_bound(first, last, value);
		return i != last && !(value < *i);
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class ForwardIter, class T, class Compared>
	bool binary_search(ForwardIter first, ForwardIter last, const T& value, Compared comp) {
		auto i = saystl::lower_bound(first, last, value);
		return i != last && !comp(value, *i);
	}

	// equal_range
	// ����[first,last)�������� value ��ȵ�Ԫ�����γɵ����䣬����һ�Ե�����ָ��������β
	// ��һ��������ָ���һ����С�� value ��Ԫ�أ��ڶ���������ָ���һ������ value ��Ԫ��
	// erange_dispatch �� forward_iterator_tag �汾
	template <class ForwardIter, class T>
	saystl::pair<ForwardIter, ForwardIter>
		erange_dispatch(ForwardIter first, ForwardIter last,
			const T& value, forward_iterator_tag)
	{
		auto len = saystl::distance(first, last);
		auto half = len;
		ForwardIter middle, left, right;
		while (len > 0) {
			half = len >> 1;
			middle = first;
			saystl::advance(middle, half);
			if (*middle < value) {
				first = middle;
				++first;
				len = len - half - 1;
			}
			else if (value < *middle) {
				len = half;
			}
			else {
				left = saystl::lower_bound(first, last, value);
				saystl::advance(first, len);
				right = saystl::upper_bound(++middle, first, value);
				return saystl::pair<ForwardIter, ForwardIter>(left, right);
			}
		}
		return saystl::pair<ForwardIter, ForwardIter>(last, last);
	}

	// erange_dispatch �� random_access_iterator_tag �汾
	template <class RandomIter, class T>
	saystl::pair<RandomIter, RandomIter>
		erange_dispatch(RandomIter first, RandomIter last,
			const T& value, random_access_iterator_tag) {
		auto len = last - first;
		auto half = len;
		RandomIter middle, left, right;
		while (len > 0) {
			half = len >> 1;
			middle = first + half;
			if (*middle < value) {
				first = middle + 1;
				len = len - half - 1;
			}
			else if (value < *middle) {
				len = half;
			}
			else {
				left = saystl::lower_bound(first, middle, value);
				right = saystl::upper_bound(++middle, first + len, value);
				return saystl::pair<RandomIter, RandomIter>(left, right);
			}
		}
		return saystl::pair<RandomIter, RandomIter>(last, last);
	}

	template <class ForwardIter, class T>
	saystl::pair<ForwardIter, ForwardIter>
		equal_range(ForwardIter first, ForwardIter last, const T& value) {
		return saystl::erange_dispatch(first, last, value, iterator_category(first));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	// erange_dispatch �� forward iterator �汾
	template <class ForwardIter, class T, class Compared>
	saystl::pair<ForwardIter, ForwardIter>
		erange_dispatch(ForwardIter first, ForwardIter last,
			const T& value, forward_iterator_tag, Compared comp)
	{
		auto len = saystl::distance(first, last);
		auto half = len;
		ForwardIter middle, left, right;
		while (len > 0) {
			half = len >> 1;
			middle = first;
			saystl::advance(middle, half);
			if (comp(*middle, value)) {
				first = middle;
				++first;
				len = len - half - 1;
			}
			else if (comp(value, *middle)) {
				len = half;
			}
			else {
				left = saystl::lower_bound(first, last, value, comp);
				saystl::advance(first, len);
				right = saystl::upper_bound(++middle, first, value, comp);
				return saystl::pair<ForwardIter, ForwardIter>(left, right);
			}
		}
		return saystl::pair<ForwardIter, ForwardIter>(last, last);
	}

	// erange_dispatch �� random access iterator �汾
	template <class RandomIter, class T, class Compared>
	saystl::pair<RandomIter, RandomIter>
		erange_dispatch(RandomIter first, RandomIter last,
			const T& value, random_access_iterator_tag, Compared comp) {
		auto len = last - first;
		auto half = len;
		RandomIter middle, left, right;
		while (len > 0) {
			half = len >> 1;
			middle = first + half;
			if (comp(*middle, value)) {
				first = middle + 1;
				len = len - half - 1;
			}
			else if (comp(value, *middle)) {
				len = half;
			}
			else {
				left = saystl::lower_bound(first, middle, value, comp);
				right = saystl::upper_bound(++middle, first + len, value, comp);
				return saystl::pair<RandomIter, RandomIter>(left, right);
			}
		}
		return saystl::pair<RandomIter, RandomIter>(last, last);
	}

	template <class ForwardIter, class T, class Compared>
	saystl::pair<ForwardIter, ForwardIter>
		equal_range(ForwardIter first, ForwardIter last, const T& value, Compared comp) {
		return saystl::erange_dispatch(first, last, value, iterator_category(first), comp);
	}

	// generate
	// ���������� gen ����������[first, last)�ڵ�ÿ��Ԫ�ظ�ֵ
	template <class ForwardIter, class Generator>
	void generate(ForwardIter first, ForwardIter last, Generator gen) {
		for (; first != last; ++first) {
			*first = gen();
		}
	}

	// generate_n
	// �ú������� gen ������ n ��Ԫ�ظ�ֵ
	template <class ForwardIter, class Size, class Generator>
	void generate_n(ForwardIter first, Size n, Generator gen) {
		for (; n > 0; --n, ++first) {
			*first = gen();
		}
	}

	// includes
	// �ж�����һS1 �Ƿ�������ж�S2
	template <class InputIter1, class InputIter2>
	bool includes(InputIter1 first1, InputIter1 last1,
		InputIter2 first2, InputIter2 last2) {
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1)
				return false;
			else if (*first1 < *first2)
				++first1;
			else ++first1, ++first2;
		}
		return first2 == last2;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class InputIter1, class InputIter2, class Compared>
	bool includes(InputIter1 first1, InputIter1 last1,
		InputIter2 first2, InputIter2 last2, Compared comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first2, *first1))
				return false;
			else if (comp(*first1, *first2))
				++first1;
			else ++first1, ++first2;
		}
		return first2 == last2;
	}

	// is_heap
	// ���[first, last)�ڵ�Ԫ���Ƿ�Ϊһ���ѣ�����ǣ��򷵻� true
	template <class RandomIter>
	bool is_heap(RandomIter first, RandomIter last) {
		auto n = saystl::distance(first, last);
		auto parent = 0;
		for (auto child = 1; child < n; ++child) {
			if (first[parent] < first[child])
				return false;
			// &�Ƕ�����λ�������㣬ĩβ��ʼ������������ĩβ��1��ż����0��Ҳ����˵����i&1���Ϊ1��ż����Ϊ0
			if ((child & 1) == 0) // & ������(Ҳ����˵����i&1���Ϊ1��ż����Ϊ0)  child & 1 ������� 0 ���� child == ż��
				++parent;
		}
		return true;
	}
	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class RandomIter, class Compared>
	bool is_heap(RandomIter first, RandomIter last, Compared comp) {
		auto n = saystl::distance(first, last);
		auto parent = 0;
		for (auto child = 1; child < n; ++child) {
			if (comp(first[parent], first[child]))
				return false;
			if ((child & 1) == 0)
				++parent;
		}
		return true;
	}

	// is_sorted
	// ���[first, last)�ڵ�Ԫ���Ƿ���������������򷵻� true
	template <class ForwardIter>
	bool is_sorted(ForwardIter first, ForwardIter last) {
		if (first == last)
			return true;
		auto next = first;
		++next;
		for (; next != last; first = next, ++next) {
			if (*next < *first)
				return false;
		}
		return true;
	}
	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class ForwardIter, class Compared>
	bool is_sorted(ForwardIter first, ForwardIter last, Compared comp) {
		if (first == last)
			return true;
		auto next = first;
		++next;
		for (; next != last; first = next, ++next) {
			if (comp(*next, *first))
				return false;
		}
		return true;
	}

	// median
	// �ҳ�����ֵ���м�ֵ
	template <class T>
	const T& median(const T& left, const T& mid, const T& right) {
		if (left < mid)
			if (mid < right)        // left < mid < right
				return mid;
			else if (left < right)  // left < right <= mid
				return right;
			else                    // right <= left < mid
				return left;
		else if (left < right)      // mid <= left < right
			return left;
		else if (mid < right)       // mid < right <= left
			return right;
		else                        // right <= mid <= left
			return mid;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class T, class Compared>
	const T& median(const T& left, const T& mid, const T& right, Compared comp) {
		if (comp(left, mid))
			if (comp(mid, right))
				return mid;
			else if (comp(left, right))
				return right;
			else
				return left;
		else if (comp(left, right))
			return left;
		else if (comp(mid, right))
			return right;
		else
			return mid;
	}

	// max_element
	// ����һ����������ָ������������Ԫ��
	template <class ForwardIter>
	ForwardIter max_element(ForwardIter first, ForwardIter last) {
		if (first == last)
			return first;
		auto result = first;
		while (++first != last) {
			if (*result < *first)
				result = first;
		}
		return result;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class ForwardIter, class Compared>
	ForwardIter max_element(ForwardIter first, ForwardIter last, Compared comp) {
		if (first == last)
			return first;
		auto result = first;
		while (++first != last) {
			if (comp(*result, *first))
				result = first;
		}
		return result;
	}

	// min_element
	// ����һ����������ָ����������С��Ԫ��
	template <class ForwardIter>
	ForwardIter min_elememt(ForwardIter first, ForwardIter last)
	{
		if (first == last)
			return first;
		auto result = first;
		while (++first != last) {
			if (*first < *result)
				result = first;
		}
		return result;
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class ForwardIter, class Compared>
	ForwardIter min_elememt(ForwardIter first, ForwardIter last, Compared comp) {
		if (first == last)
			return first;
		auto result = first;
		while (++first != last) {
			if (comp(*first, *result))
				result = first;
		}
		return result;
	}

	// swap_ranges
	// ��[first1, last1)�� first2 ��ʼ, ������ͬ����Ԫ��
	// ���������䳤�ȱ�����ͬ, �������в��ܻ����ص�, ����һ��������ָ�����ж����һ��������Ԫ�ص���һλ��
	template <class ForwardIter1, class ForwardIter2>
	ForwardIter2
		swap_ranges(ForwardIter1 first1, ForwardIter1 last1,
			ForwardIter2 first2) {
		for (; first1 != last1; ++first1, ++first2) {
			saystl::iter_swap(first1, first2);
		}
		return first2;
	}

	// transform
	// ��һ���汾�Ժ������� unary_op ������[first, last)�е�ÿ��Ԫ�ز������������ result ��
	// �ڶ����汾�Ժ������� binary_op ��������������[first1, last1)��[first2, last2)����ͬλ��
	template <class InputIter, class OutputIter, class UnaryOperation>
	OutputIter
		transform(InputIter first, InputIter last,
			OutputIter result, UnaryOperation unary_op) {
		for (; first != last; ++first, ++result) {
			*result = unary_op(*first);
		}
		return result;
	}

	template <class InputIter1, class InputIter2, class OutputIter, class BinaryOperation>
	OutputIter
		transform(InputIter1 first1, InputIter1 last1,
			InputIter2 first2, OutputIter result, BinaryOperation binary_op) {
		for (; first1 != last1; ++first1, ++first2, ++result) {
			*result = binary_op(*first1, *first2);
		}
		return result;
	}

	// remove_copy
	// �Ƴ���������ָ�� value ��ȵ�Ԫ�أ�����������Ƶ��� result ��ʾ��ʼλ�õ�������
	template <class InputIter, class OutputIter, class T>
	OutputIter
		remove_copy(InputIter first, InputIter last, OutputIter result, const T& value) {
		for (; first != last; ++first) {
			if (*first != value)
				*result++ = *first;
		}
		return result;
	}

	// remove
	// �Ƴ�������ָ�� value ��ȵ�Ԫ��
	// ������������ɾ����ЩԪ�أ����� remove �� remove_if �������� array
	template <class ForwardIter, class T>
	ForwardIter remove(ForwardIter first, ForwardIter last, const T& value) {
		first = saystl::find(first, last, value);  // ���� find �ҳ���һ��ƥ��ĵط�
		auto next = first;
		return first == last ? first : saystl::remove_copy(++next, last, first, value);
	}

	// remove_copy_if
	// �Ƴ�������������һԪ���� unary_pred Ϊ true ��Ԫ�أ�����������Ƶ��� result Ϊ��ʼλ�õ�������
	template <class InputIter, class OutputIter, class UnaryPredicate>
	OutputIter
		remove_copy_if(InputIter first, InputIter last,
			OutputIter result, UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (!unary_pred(*first)) {
				*result = *first;
				++result;
			}
		}
		return result;
	}

	// remove_if
	// �Ƴ�������������һԪ���� unary_pred Ϊ true ��Ԫ��
	template <class ForwardIter, class UnaryPredicate>
	ForwardIter
		remove_if(ForwardIter first, ForwardIter last, UnaryPredicate unary_pred) {
		first = saystl::find_if(first, last, unary_pred);  // ���� find_if �ҳ���һ��ƥ��ĵط�
		auto next = first;
		return first == last ? first : saystl::remove_copy_if(++next, last, first, unary_pred);
	}

	// replace
	// �����������е� old_value ���� new_value ���
	template <class ForwardIter, class T>
	void replace(ForwardIter first, ForwardIter last,
		const T& old_value, const T& new_value) {
		for (; first != last; ++first) {
			if (*first == old_value)
				*first = new_value;
		}
	}

	// replace_copy
	// ��Ϊ�� replace ���ƣ���ͬ���ǽ�������Ƶ� result ��ָ�������У�ԭ����û�иı�
	template <class InputIter, class OutputIter, class T>
	OutputIter
		replace_copy(InputIter first, InputIter last,
			OutputIter result, const T& old_value, const T& new_value) {
		for (; first != last; ++first, ++result) {
			*result = *first == old_value ? new_value : *first;
		}
		return result;
	}

	// replace_copy_if
	// ��Ϊ�� replace_if ���ƣ���ͬ���ǽ�������Ƶ� result ��ָ�������У�ԭ����û�иı�
	template <class InputIter, class OutputIter, class UnaryPredicate, class T>
	OutputIter
		replace_copy_if(InputIter first, InputIter last,
			OutputIter result, UnaryPredicate unary_pred, const T& new_value) {
		for (; first != last; ++first, ++result) {
			*result = unary_pred(*first) ? new_value : *first;
		}
		return result;
	}

	// replace_if
	// ��������������һԪ���� unary_pred Ϊ true ��Ԫ�ض��� new_value ���
	template <class ForwardIter, class UnaryPredicate, class T>
	void replace_if(ForwardIter first, ForwardIter last,
		UnaryPredicate unary_pred, const T& new_value) {
		for (; first != last; ++first) {
			if (unary_pred(*first))
				*first = new_value;
		}
	}

	// reverse
	// ��[first, last)�����ڵ�Ԫ�ط�ת
	// reverse_dispatch �� bidirectional_iterator_tag �汾
	template <class BidirectionalIter>
	void reverse_dispatch(BidirectionalIter first, BidirectionalIter last,
		bidirectional_iterator_tag) {
		while (true) {
			if (first == last || first == --last)
				return;
			saystl::iter_swap(first++, last);
		}
	}

	// reverse_dispatch �� random_access_iterator_tag �汾
	template <class RandomIter>
	void reverse_dispatch(RandomIter first, RandomIter last,
		random_access_iterator_tag) {
		while (first < last)
			saystl::iter_swap(first++, --last);
	}

	template <class BidirectionalIter>
	void reverse(BidirectionalIter first, BidirectionalIter last) {
		saystl::reverse_dispatch(first, last, iterator_category(first));
	}

	// reverse_copy
	// ��Ϊ�� reverse ���ƣ���ͬ���ǽ�������Ƶ� result ��ָ������
	template <class BidirectionalIter, class OutputIter>
	OutputIter
		reverse_copy(BidirectionalIter first, BidirectionalIter last,
			OutputIter result) {
		while (first != last) {
			--last;
			*result = *last;
			++result;
		}
		return result;
	}

	// random_shuffle
	// ��[first, last)�ڵ�Ԫ�ش����������
	// ���ذ汾ʹ��һ������������ĺ������� rand
	template <class RandomIter>
	void random_shuffle(RandomIter first, RandomIter last) {
		if (first == last)
			return;
		srand((unsigned)time(0));
		for (auto i = first + 1; i != last; ++i) {
			saystl::iter_swap(i, first + (rand() % (i - first + 1)));
		}
	}

	// ���ذ汾ʹ��һ������������ĺ������� rand
	template <class RandomIter, class RandomNumberGenerator>
	void random_shuffle(RandomIter first, RandomIter last,
		RandomNumberGenerator& rand) {
		if (first == last)
			return;
		auto len = saystl::distance(first, last);
		for (auto i = first + 1; i != last; ++i) {
			saystl::iter_swap(i, first + (rand(i - first + 1) % len));
		}
	}

	// rotate
	// ��[first, middle)�ڵ�Ԫ�غ� [middle, last)�ڵ�Ԫ�ػ��������Խ����������Ȳ�ͬ������
	// ���ؽ����� middle ��λ��
	// rotate_dispatch �� forward_iterator_tag �汾
	template <class ForwardIter>
	ForwardIter
		rotate_dispatch(ForwardIter first, ForwardIter middle,
			ForwardIter last, forward_iterator_tag) {
		auto first2 = middle;
		do {
			saystl::swap(*first++, *first2++);
			if (first == middle)
				middle = first2;
		} while (first2 != last);  // �����Ƶ�ǰ��

		auto new_middle = first;   // ���������ص�λ��
		first2 = middle;
		while (first2 != last) {   // ����ʣ��Ԫ��
			saystl::swap(*first++, *first2++);
			if (first == middle)
				middle = first2;
			else if (first2 == last)
				first2 = middle;
		}
		return new_middle;
	}

	// rotate_dispatch �� bidirectional_iterator_tag �汾
	template <class BidirectionalIter>
	BidirectionalIter
		rotate_dispatch(BidirectionalIter first, BidirectionalIter middle,
			BidirectionalIter last, bidirectional_iterator_tag) {
		saystl::reverse_dispatch(first, middle, bidirectional_iterator_tag());
		saystl::reverse_dispatch(middle, last, bidirectional_iterator_tag());
		while (first != middle && middle != last)
			saystl::swap(*first++, *--last);
		if (first == middle) {
			saystl::reverse_dispatch(middle, last, bidirectional_iterator_tag());
			return last;
		}
		else {
			saystl::reverse_dispatch(first, middle, bidirectional_iterator_tag());
			return first;
		}
	}

	// ���������
	template <class EuclideanRingElement>
	EuclideanRingElement rgcd(EuclideanRingElement m, EuclideanRingElement n) {
		while (n != 0) {
			auto t = m % n;
			m = n;
			n = t;
		}
		return m;
	}

	// rotate_dispatch �� random_access_iterator_tag �汾
	template <class RandomIter>
	RandomIter
		rotate_dispatch(RandomIter first, RandomIter middle,
			RandomIter last, random_access_iterator_tag)
	{
		// ��Ϊ�� random access iterator�����ǿ���ȷ��ÿ��Ԫ�ص�λ��
		auto n = last - first;
		auto l = middle - first;
		auto r = n - l;
		auto result = first + (last - middle);
		if (l == r) {
			saystl::swap_ranges(first, middle, middle);
			return result;
		}
		auto cycle_times = rgcd(n, l);
		for (auto i = 0; i < cycle_times; ++i) {
			auto tmp = *first;
			auto p = first;
			if (l < r) {
				for (auto j = 0; j < r / cycle_times; ++j) {
					if (p > first + r) {
						*p = *(p - r);
						p -= r;
					}
					*p = *(p + l);
					p += l;
				}
			}
			else {
				for (auto j = 0; j < l / cycle_times - 1; ++j) {
					if (p < last - l) {
						*p = *(p + l);
						p += l;
					}
					*p = *(p - r);
					p -= r;
				}
			}
			*p = tmp;
			++first;
		}
		return result;
	}

	template <class ForwardIter>
	ForwardIter
		rotate(ForwardIter first, ForwardIter middle, ForwardIter last) {
		if (first == middle)
			return last;
		if (middle == last)
			return first;
		return saystl::rotate_dispatch(first, middle, last, iterator_category(first));
	}

	// rotate_copy
	// ��Ϊ�� rotate ���ƣ���ͬ���ǽ�������Ƶ� result ��ָ��������
	template <class ForwardIter, class OutputIter>
	ForwardIter
		rotate_copy(ForwardIter first, ForwardIter middle,
			ForwardIter last, OutputIter result) {
		return saystl::copy(first, middle, saystl::copy(middle, last, result));
	}

	// is_permutation
	// �ж�[first1,last1)�Ƿ�Ϊ[first2, last2)���������
	template <class ForwardIter1, class ForwardIter2, class BinaryPred>
	bool is_permutation_aux(ForwardIter1 first1, ForwardIter1 last1,
		ForwardIter2 first2, ForwardIter2 last2,
		BinaryPred pred) {
		constexpr bool is_ra_it = saystl::is_random_access_iterator<ForwardIter1>::value
			&& saystl::is_random_access_iterator<ForwardIter2>::value;
		if (is_ra_it) {
			auto len1 = last1 - first1;
			auto len2 = last2 - first2;
			if (len1 != len2)
				return false;
		}

		// ���ҳ���ͬ��ǰ׺��
		for (; first1 != last1 && first2 != last2; ++first1, (void) ++first2) {
			if (!pred(*first1, *first2))
				break;
		}
		if (is_ra_it) {
			if (first1 == last1)
				return true;
		}
		else {
			auto len1 = saystl::distance(first1, last1);
			auto len2 = saystl::distance(first2, last2);
			if (len1 == 0 && len2 == 0)
				return true;
			if (len1 != len2)
				return false;
		}

		// �ж�ʣ�ಿ��
		for (auto i = first1; i != last1; ++i) {
			bool is_repeated = false;
			for (auto j = first1; j != i; ++j) {
				if (pred(*j, *i)) {
					is_repeated = true;
					break;
				}
			}

			if (!is_repeated) {
				// ���� *i �� [first2, last2) ����Ŀ
				auto c2 = 0;
				for (auto j = first2; j != last2; ++j) {
					if (pred(*i, *j))
						++c2;
				}
				if (c2 == 0)
					return false;

				// ���� *i �� [first1, last1) ����Ŀ
				auto c1 = 1;
				auto j = i;
				for (++j; j != last1; ++j) {
					if (pred(*i, *j))
						++c1;
				}
				if (c1 != c2)
					return false;
			}
		}
		return true;
	}

	template <class ForwardIter1, class ForwardIter2, class BinaryPred>
	bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
		ForwardIter2 first2, ForwardIter2 last2,
		BinaryPred pred) {
		return is_permutation_aux(first1, last1, first2, last2, pred);
	}

	template <class ForwardIter1, class ForwardIter2>
	bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
		ForwardIter2 first2, ForwardIter2 last2) {
		typedef typename iterator_traits<ForwardIter1>::value_type v1;
		typedef typename iterator_traits<ForwardIter2>::value_type v2;
		static_assert(std::is_same<v1, v2>::value,
			"the type should be same in saystl::is_permutation");
		return is_permutation_aux(first1, last1, first2, last2,
			saystl::equal_to<v1>());
	}

	// next_permutation
	// ȡ��[first, last)����ʾ���е���һ��������ϣ����û����һ��������ϣ����� false�����򷵻� true
	template <class BidirectionalIter>
	bool next_permutation(BidirectionalIter first, BidirectionalIter last) {
		auto i = last;
		if (first == last || first == --i)
			return false;
		for (;;) {
			auto ii = i;
			if (*--i < *ii) {                 // �ҵ���һ��С�ڹ�ϵ��Ԫ��
				auto j = last;
				while (!(*i < *--j)) {}
				saystl::iter_swap(i, j);       // ���� i��j ��ָԪ��
				saystl::reverse(ii, last);     // �� ii ֮�������Ԫ�ط�ת
				return true;
			}
			if (i == first) {
				saystl::reverse(first, last);
				return false;
			}
		}
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class BidirectionalIter, class Compared>
	bool next_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp) {
		auto i = last;
		if (first == last || first == --i)
			return false;
		for (;;) {
			auto ii = i;
			if (comp(*--i, *ii)) {
				auto j = last;
				while (!comp(*i, *--j)) {}
				saystl::iter_swap(i, j);       // ���� i��j ��ָԪ��
				saystl::reverse(ii, last);     // �� ii ֮�������Ԫ�ط�ת
				return true;
			}
			if (i == first) {
				saystl::reverse(first, last);
				return false;
			}
		}
	}

	// prev_permutation
	// ȡ��[first, last)����ʾ���е���һ��������ϣ����û����һ��������ϣ����� false�����򷵻� true
	template <class BidirectionalIter>
	bool prev_permutation(BidirectionalIter first, BidirectionalIter last) {
		auto i = last;
		if (first == last || first == --i)
			return false;
		for (;;) {
			auto ii = i;
			if (*ii < *--i)
			{                 // �ҵ���һ�Դ��ڹ�ϵ��Ԫ��
				auto j = last;
				while (!(*--j < *i)) {}
				saystl::iter_swap(i, j);       // ����i��j
				saystl::reverse(ii, last);     // �� ii ֮�������Ԫ�ط�ת
				return true;
			}
			if (i == first) {
				saystl::reverse(first, last);
				return false;
			}
		}
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class BidirectionalIter, class Compared>
	bool prev_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp) {
		auto i = last;
		if (first == last || first == --i)
			return false;
		for (;;) {
			auto ii = i;
			if (comp(*ii, *--i)) {
				auto j = last;
				while (!comp(*--j, *i)) {}
				saystl::iter_swap(i, j);       // ����i��j
				saystl::reverse(ii, last);     // �� ii ֮�������Ԫ�ط�ת
				return true;
			}
			if (i == first) {
				saystl::reverse(first, last);
				return false;
			}
		}
	}

	// merge
	// ��������������ļ��� S1 �� S2 �ϲ�����������һ�οռ䣬����һ��������ָ�����һ��Ԫ�ص���һλ��
	template <class InputIter1, class InputIter2, class OutputIter>
	OutputIter
		merge(InputIter1 first1, InputIter1 last1,
			InputIter2 first2, InputIter2 last2,
			OutputIter result) {
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				*result = *first2;
				++first2;
			}
			else {
				*result = *first1;
				++first1;
			}
			++result;
		}
		return saystl::copy(first2, last2, saystl::copy(first1, last1, result));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class InputIter1, class InputIter2, class OutputIter, class Compared>
	OutputIter
		merge(InputIter1 first1, InputIter1 last1,
			InputIter2 first2, InputIter2 last2,
			OutputIter result, Compared comp) {
		while (first1 != last1 && first2 != last2) {
			if (comp(*first2, *first1)) {
				*result = *first2;
				++first2;
			}
			else {
				*result = *first1;
				++first1;
			}
			++result;
		}
		return saystl::copy(first2, last2, saystl::copy(first1, last1, result));
	}

	// inplace_merge
	// ��������һ��������������н�ϳɵ�һ���в���������
	// û�л�����������ºϲ�
	template <class BidirectionalIter, class Distance>
	void merge_without_buffer(BidirectionalIter first, BidirectionalIter middle,
		BidirectionalIter last, Distance len1, Distance len2)
	{
		if (len1 == 0 || len2 == 0)
			return;
		if (len1 + len2 == 2)
		{
			if (*middle < *first)
				saystl::iter_swap(first, middle);
			return;
		}
		auto first_cut = first;
		auto second_cut = middle;
		Distance len11 = 0;
		Distance len22 = 0;
		if (len1 > len2)
		{  // ����һ�ϳ����ҵ�����һ���е�
			len11 = len1 >> 1;
			saystl::advance(first_cut, len11);
			second_cut = saystl::lower_bound(middle, last, *first_cut);
			len22 = saystl::distance(middle, second_cut);
		}
		else
		{              // ���ж��ϳ����ҵ����ж����е�
			len22 = len2 >> 1;
			saystl::advance(second_cut, len22);
			first_cut = saystl::upper_bound(first, middle, *second_cut);
			len11 = saystl::distance(first, first_cut);
		}
		auto new_middle = saystl::rotate(first_cut, middle, second_cut);
		saystl::merge_without_buffer(first, first_cut, new_middle, len11, len22);
		saystl::merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22);
	}

	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter1
		merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
			BidirectionalIter2 first2, BidirectionalIter2 last2,
			BidirectionalIter1 result)
	{
		if (first1 == last1)
			return saystl::copy_backward(first2, last2, result);
		if (first2 == last2)
			return saystl::copy_backward(first1, last1, result);
		--last1;
		--last2;
		while (true)
		{
			if (*last2 < *last1)
			{
				*--result = *last1;
				if (first1 == last1)
					return saystl::copy_backward(first2, ++last2, result);
				--last1;
			}
			else
			{
				*--result = *last2;
				if (first2 == last2)
					return saystl::copy_backward(first1, ++last1, result);
				--last2;
			}
		}
	}

	template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
	BidirectionalIter1
		rotate_adaptive(BidirectionalIter1 first, BidirectionalIter1 middle,
			BidirectionalIter1 last, Distance len1, Distance len2,
			BidirectionalIter2 buffer, Distance buffer_size)
	{
		BidirectionalIter2 buffer_end;
		if (len1 > len2 && len2 <= buffer_size)
		{
			buffer_end = saystl::copy(middle, last, buffer);
			saystl::copy_backward(first, middle, last);
			return saystl::copy(buffer, buffer_end, first);
		}
		else if (len1 <= buffer_size)
		{
			buffer_end = saystl::copy(first, middle, buffer);
			saystl::copy(middle, last, first);
			return saystl::copy_backward(buffer, buffer_end, last);
		}
		else
		{
			return saystl::rotate(first, middle, last);
		}
	}

	// �л�����������ºϲ�
	template <class BidirectionalIter, class Distance, class Pointer>
	void merge_adaptive(BidirectionalIter first, BidirectionalIter middle,
		BidirectionalIter last, Distance len1, Distance len2,
		Pointer buffer, Distance buffer_size)
	{
		// ���䳤���㹻�Ž�������
		if (len1 <= len2 && len1 <= buffer_size)
		{
			Pointer buffer_end = saystl::copy(first, middle, buffer);
			saystl::merge(buffer, buffer_end, middle, last, first);
		}
		else if (len2 <= buffer_size)
		{
			Pointer buffer_end = saystl::copy(middle, last, buffer);
			saystl::merge_backward(first, middle, buffer, buffer_end, last);
		}
		else
		{  // ���䳤��̫�����ָ�ݹ鴦��
			auto first_cut = first;
			auto second_cut = middle;
			Distance len11 = 0;
			Distance len22 = 0;
			if (len1 > len2)
			{
				len11 = len1 >> 1;
				saystl::advance(first_cut, len11);
				second_cut = saystl::lower_bound(middle, last, *first_cut);
				len22 = saystl::distance(middle, second_cut);
			}
			else
			{
				len22 = len2 >> 1;
				saystl::advance(second_cut, len22);
				first_cut = saystl::upper_bound(first, middle, *second_cut);
				len11 = saystl::distance(first, first_cut);
			}
			auto new_middle = saystl::rotate_adaptive(first_cut, middle, second_cut,
				len1 - len11, len22, buffer, buffer_size);
			saystl::merge_adaptive(first, first_cut, new_middle, len11, len22, buffer, buffer_size);
			saystl::merge_adaptive(new_middle, second_cut, last, len1 - len11,
				len2 - len22, buffer, buffer_size);
		}
	}

	template <class BidirectionalIter, class T>
	void
		inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle,
			BidirectionalIter last, T*)
	{
		auto len1 = saystl::distance(first, middle);
		auto len2 = saystl::distance(middle, last);
		temporary_buffer<BidirectionalIter, T> buf(first, last);
		if (!buf.begin())
		{
			saystl::merge_without_buffer(first, middle, last, len1, len2);
		}
		else
		{
			saystl::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size());
		}
	}

	template <class BidirectionalIter>
	void
		inplace_merge(BidirectionalIter first, BidirectionalIter middle,
			BidirectionalIter last)
	{
		if (first == middle || middle == last)
			return;
		saystl::inplace_merge_aux(first, middle, last, value_type(first));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	// û�л�����������ºϲ�
	template <class BidirectionalIter, class Distance, class Compared>
	void merge_without_buffer(BidirectionalIter first, BidirectionalIter middle,
		BidirectionalIter last, Distance len1, Distance len2,
		Compared comp)
	{
		if (len1 == 0 || len2 == 0)
			return;
		if (len1 + len2 == 2)
		{
			if (comp(*middle, *first))
				saystl::iter_swap(first, middle);
			return;
		}
		auto first_cut = first;
		auto second_cut = middle;
		Distance len11 = 0;
		Distance len22 = 0;
		if (len1 > len2)
		{
			len11 = len1 >> 1;
			saystl::advance(first_cut, len11);
			second_cut = saystl::lower_bound(middle, last, *first_cut, comp);
			len22 = saystl::distance(middle, second_cut);
		}
		else
		{
			len22 = len2 >> 1;
			saystl::advance(second_cut, len22);
			first_cut = saystl::upper_bound(first, middle, *second_cut, comp);
			len11 = saystl::distance(first, first_cut);
		}
		auto new_middle = saystl::rotate(first_cut, middle, second_cut);
		saystl::merge_without_buffer(first, first_cut, new_middle, len11, len22, comp);
		saystl::merge_without_buffer(new_middle, second_cut, last, len1 - len11, len2 - len22, comp);
	}

	template <class BidirectionalIter1, class BidirectionalIter2, class Compared>
	BidirectionalIter1
		merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1,
			BidirectionalIter2 first2, BidirectionalIter2 last2,
			BidirectionalIter1 result, Compared comp)
	{
		if (first1 == last1)
			return saystl::copy_backward(first2, last2, result);
		if (first2 == last2)
			return saystl::copy_backward(first1, last1, result);
		--last1;
		--last2;
		while (true)
		{
			if (comp(*last2, *last1))
			{
				*--result = *last1;
				if (first1 == last1)
					return saystl::copy_backward(first2, ++last2, result);
				--last1;
			}
			else
			{
				*--result = *last2;
				if (first2 == last2)
					return saystl::copy_backward(first1, ++last1, result);
				--last2;
			}
		}
	}

	// �л�����������ºϲ�
	template <class BidirectionalIter, class Distance, class Pointer, class Compared>
	void merge_adaptive(BidirectionalIter first, BidirectionalIter middle,
		BidirectionalIter last, Distance len1, Distance len2,
		Pointer buffer, Distance buffer_size, Compared comp)
	{
		// ���䳤���㹻�Ž�������
		if (len1 <= len2 && len1 <= buffer_size)
		{
			Pointer buffer_end = saystl::copy(first, middle, buffer);
			saystl::merge(buffer, buffer_end, middle, last, first, comp);
		}
		else if (len2 <= buffer_size)
		{
			Pointer buffer_end = saystl::copy(middle, last, buffer);
			saystl::merge_backward(first, middle, buffer, buffer_end, last, comp);
		}
		else
		{  // ���䳤��̫�����ָ�ݹ鴦��
			auto first_cut = first;
			auto second_cut = middle;
			Distance len11 = 0;
			Distance len22 = 0;
			if (len1 > len2)
			{
				len11 = len1 >> 1;
				saystl::advance(first_cut, len11);
				second_cut = saystl::lower_bound(middle, last, *first_cut, comp);
				len22 = saystl::distance(middle, second_cut);
			}
			else
			{
				len22 = len2 >> 1;
				saystl::advance(second_cut, len22);
				first_cut = saystl::upper_bound(first, middle, *second_cut, comp);
				len11 = saystl::distance(first, first_cut);
			}
			auto new_middle = saystl::rotate_adaptive(first_cut, middle, second_cut, len1 - len11,
				len22, buffer, buffer_size);
			saystl::merge_adaptive(first, first_cut, new_middle, len11,
				len22, buffer, buffer_size, comp);
			saystl::merge_adaptive(new_middle, second_cut, last, len1 - len11,
				len2 - len22, buffer, buffer_size, comp);
		}
	}

	template <class BidirectionalIter, class T, class Compared>
	void
		inplace_merge_aux(BidirectionalIter first, BidirectionalIter middle,
			BidirectionalIter last, T*, Compared comp)
	{
		auto len1 = saystl::distance(first, middle);
		auto len2 = saystl::distance(middle, last);
		temporary_buffer<BidirectionalIter, T> buf(first, last);
		if (!buf.begin())
		{
			saystl::merge_without_buffer(first, middle, last, len1, len2, comp);
		}
		else
		{
			saystl::merge_adaptive(first, middle, last, len1, len2, buf.begin(), buf.size(), comp);
		}
	}

	template <class BidirectionalIter, class Compared>
	void
		inplace_merge(BidirectionalIter first, BidirectionalIter middle,
			BidirectionalIter last, Compared comp)
	{
		if (first == middle || middle == last)
			return;
		saystl::inplace_merge_aux(first, middle, last, value_type(first), comp);
	}

	// partial_sort
	// �������������������򣬱�֤��С�� N ��Ԫ���Ե���˳������[first, first + N)��
	template <class RandomIter>
	void partial_sort(RandomIter first, RandomIter middle,
		RandomIter last)
	{
		saystl::make_heap(first, middle);
		for (auto i = middle; i < last; ++i)
		{
			if (*i < *first)
			{
				saystl::pop_heap_aux(first, middle, i, *i, distance_type(first));
			}
		}
		saystl::sort_heap(first, middle);
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class RandomIter, class Compared>
	void partial_sort(RandomIter first, RandomIter middle,
		RandomIter last, Compared comp)
	{
		saystl::make_heap(first, middle, comp);
		for (auto i = middle; i < last; ++i)
		{
			if (comp(*i, *first))
			{
				saystl::pop_heap_aux(first, middle, i, *i, distance_type(first), comp);
			}
		}
		saystl::sort_heap(first, middle, comp);
	}

	// partial_sort_copy
	// ��Ϊ�� partial_sort ���ƣ���ͬ���ǰ����������Ƶ� result ������
	template <class InputIter, class RandomIter, class Distance>
	RandomIter
		psort_copy_aux(InputIter first, InputIter last,
			RandomIter result_first, RandomIter result_last,
			Distance*)
	{
		if (result_first == result_last)
			return result_last;
		auto result_iter = result_first;
		while (first != last && result_iter != result_last)
		{
			*result_iter = *first;
			++result_iter;
			++first;
		}
		saystl::make_heap(result_first, result_iter);
		while (first != last)
		{
			if (*first < *result_first)
			{
				saystl::adjust_heap(result_first, static_cast<Distance>(0),
					result_iter - result_first, *first);
			}
			++first;
		}
		saystl::sort_heap(result_first, result_iter);
		return result_iter;
	}

	template <class InputIter, class RandomIter>
	RandomIter
		partial_sort_copy(InputIter first, InputIter last,
			RandomIter result_first, RandomIter result_last)
	{
		return saystl::psort_copy_aux(first, last, result_first, result_last,
			distance_type(result_first));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class InputIter, class RandomIter, class Distance, class Compared>
	RandomIter
		psort_copy_aux(InputIter first, InputIter last,
			RandomIter result_first, RandomIter result_last,
			Distance*, Compared comp)
	{
		if (result_first == result_last)
			return result_last;
		auto result_iter = result_first;
		while (first != last && result_iter != result_last)
		{
			*result_iter = *first;
			++result_iter;
			++first;
		}
		saystl::make_heap(result_first, result_iter, comp);
		while (first != last)
		{
			if (comp(*first, *result_first))
			{
				saystl::adjust_heap(result_first, static_cast<Distance>(0),
					result_iter - result_first, *first, comp);
			}
			++first;
		}
		saystl::sort_heap(result_first, result_iter, comp);
		return result_iter;
	}

	template <class InputIter, class RandomIter, class Compared>
	RandomIter
		partial_sort_copy(InputIter first, InputIter last,
			RandomIter result_first, RandomIter result_last,
			Compared comp)
	{
		return saystl::psort_copy_aux(first, last, result_first, result_last,
			distance_type(result_first), comp);
	}

	// partition
	// �������ڵ�Ԫ�����ţ���һԪ���������ж�Ϊ true ��Ԫ�ػ�ŵ������ǰ��
	// �ú�������֤Ԫ�ص�ԭʼ���λ��
	template <class BidirectionalIter, class UnaryPredicate>
	BidirectionalIter
		partition(BidirectionalIter first, BidirectionalIter last,
			UnaryPredicate unary_pred)
	{
		while (true)
		{
			while (first != last && unary_pred(*first))
			{
				++first;
			}
			if (first == last)
				break;
			--last;
			while (first != last && !unary_pred(*last))
			{
				--last;
			}
			if (first == last)
				break;
			saystl::iter_swap(first, last);
			++first;
		}
		return first;
	}

	// partition_copy
	// ��Ϊ�� partition ���ƣ���ͬ���ǣ�����һԪ�������ж�Ϊ true �ķŵ� result_true ���������
	// ����ŵ� result_false ��������䣬������һ�� saystl::pair ָ�������������β��
	template <class InputIter, class OutputIter1, class OutputIter2, class UnaryPredicate>
	saystl::pair<OutputIter1, OutputIter2>
		partition_copy(InputIter first, InputIter last,
			OutputIter1 result_true, OutputIter2 result_false,
			UnaryPredicate unary_pred) {
		for (; first != last; ++first) {
			if (unary_pred(*first))
				*result_true++ = *first;
			else
				*result_false++ = *first;
		}
		return saystl::pair<OutputIter1, OutputIter2>(result_true, result_false);
	}

	// sort
	// ��[first, last)�ڵ�Ԫ���Ե����ķ�ʽ����
	constexpr static size_t kSmallSectionSize = 128;  // С������Ĵ�С���������С�ڲ��ò�������

	// ���ڿ��Ʒָ�񻯵����
	template <class Size>
	Size slg2(Size n)
	{ // �ҳ� lgk <= n �� k �����ֵ
		Size k = 0;
		for (; n > 1; n >>= 1)
			++k;
		return k;
	}

	// �ָ�� unchecked_partition
	template <class RandomIter, class T>
	RandomIter
		unchecked_partition(RandomIter first, RandomIter last, const T& pivot)
	{
		while (true)
		{
			while (*first < pivot)
				++first;
			--last;
			while (pivot < *last)
				--last;
			if (!(first < last))
				return first;
			saystl::iter_swap(first, last);
			++first;
		}
	}

	// ��ʡʽ�����Ƚ��� quick sort�����ָ���Ϊ�ж�����ʱ������ heap sort
	template <class RandomIter, class Size>
	void intro_sort(RandomIter first, RandomIter last, Size depth_limit)
	{
		while (static_cast<size_t>(last - first) > kSmallSectionSize)
		{
			if (depth_limit == 0)
			{                      // �������ָ��������
				saystl::partial_sort(first, last, last);  // ���� heap_sort
				return;
			}
			--depth_limit;
			auto mid = saystl::median(*(first), *(first + (last - first) / 2), *(last - 1));
			auto cut = saystl::unchecked_partition(first, last, mid);
			saystl::intro_sort(cut, last, depth_limit);
			last = cut;
		}
	}

	// �������������� unchecked_linear_insert
	template <class RandomIter, class T>
	void unchecked_linear_insert(RandomIter last, const T& value)
	{
		auto next = last;
		--next;
		while (value < *next)
		{
			*last = *next;
			last = next;
			--next;
		}
		*last = value;
	}

	// ���������� unchecked_insertion_sort
	template <class RandomIter>
	void unchecked_insertion_sort(RandomIter first, RandomIter last) {
		for (auto i = first; i != last; ++i) {
			saystl::unchecked_linear_insert(i, *i);
		}
	}

	// ���������� insertion_sort
	template <class RandomIter>
	void insertion_sort(RandomIter first, RandomIter last) {
		if (first == last)
			return;
		for (auto i = first + 1; i != last; ++i) {
			auto value = *i;
			if (value < *first) {
				saystl::copy_backward(first, i, i + 1);
				*first = value;
			}
			else
				saystl::unchecked_linear_insert(i, value);
		}
	}

	// ���ղ��������� final_insertion_sort
	template <class RandomIter>
	void final_insertion_sort(RandomIter first, RandomIter last)
	{
		if (static_cast<size_t>(last - first) > kSmallSectionSize)
		{
			saystl::insertion_sort(first, first + kSmallSectionSize);
			saystl::unchecked_insertion_sort(first + kSmallSectionSize, last);
		}
		else
			saystl::insertion_sort(first, last);
	}

	template <class RandomIter>
	void sort(RandomIter first, RandomIter last) {
		if (first != last) {
			// ��ʡʽ���򣬽������Ϊһ����С���䣬Ȼ���������в�������
			saystl::intro_sort(first, last, slg2(last - first) * 2);
			saystl::final_insertion_sort(first, last);
		}
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	// �ָ�� unchecked_partition
	template <class RandomIter, class T, class Compared>
	RandomIter
		unchecked_partition(RandomIter first, RandomIter last,
			const T& pivot, Compared comp)
	{
		while (true)
		{
			while (comp(*first, pivot))
				++first;
			--last;
			while (comp(pivot, *last))
				--last;
			if (!(first < last))
				return first;
			saystl::iter_swap(first, last);
			++first;
		}
	}

	// ��ʡʽ�����Ƚ��� quick sort�����ָ���Ϊ�ж�����ʱ������ heap sort
	template <class RandomIter, class Size, class Compared>
	void intro_sort(RandomIter first, RandomIter last,
		Size depth_limit, Compared comp)
	{
		while (static_cast<size_t>(last - first) > kSmallSectionSize)
		{
			if (depth_limit == 0)
			{                            // �������ָ��������
				saystl::partial_sort(first, last, last, comp);  // ���� heap_sort
				return;
			}
			--depth_limit;
			auto mid = saystl::median(*(first), *(first + (last - first) / 2), *(last - 1));
			auto cut = saystl::unchecked_partition(first, last, mid, comp);
			saystl::intro_sort(cut, last, depth_limit, comp);
			last = cut;
		}
	}

	// �������������� unchecked_linear_insert
	template <class RandomIter, class T, class Compared>
	void unchecked_linear_insert(RandomIter last, const T& value, Compared comp)
	{
		auto next = last;
		--next;
		while (comp(value, *next))
		{  // ��β����ʼѰ�ҵ�һ���ɲ���λ��
			*last = *next;
			last = next;
			--next;
		}
		*last = value;
	}

	// ���������� unchecked_insertion_sort
	template <class RandomIter, class Compared>
	void unchecked_insertion_sort(RandomIter first, RandomIter last,
		Compared comp)
	{
		for (auto i = first; i != last; ++i)
		{
			saystl::unchecked_linear_insert(i, *i, comp);
		}
	}

	// ���������� insertion_sort
	template <class RandomIter, class Compared>
	void insertion_sort(RandomIter first, RandomIter last, Compared comp)
	{
		if (first == last)
			return;
		for (auto i = first + 1; i != last; ++i)
		{
			auto value = *i;
			if (comp(value, *first))
			{
				saystl::copy_backward(first, i, i + 1);
				*first = value;
			}
			else
			{
				saystl::unchecked_linear_insert(i, value, comp);
			}
		}
	}

	// ���ղ��������� final_insertion_sort
	template <class RandomIter, class Compared>
	void final_insertion_sort(RandomIter first, RandomIter last, Compared comp)
	{
		if (static_cast<size_t>(last - first) > kSmallSectionSize)
		{
			saystl::insertion_sort(first, first + kSmallSectionSize, comp);
			saystl::unchecked_insertion_sort(first + kSmallSectionSize, last, comp);
		}
		else
		{
			saystl::insertion_sort(first, last, comp);
		}
	}

	template <class RandomIter, class Compared>
	void sort(RandomIter first, RandomIter last, Compared comp)
	{
		if (first != last)
		{
			// ��ʡʽ���򣬽������Ϊһ����С���䣬Ȼ���������в�������
			saystl::intro_sort(first, last, slg2(last - first) * 2, comp);
			saystl::final_insertion_sort(first, last, comp);
		}
	}

	// nth_element
	// ���������ţ�ʹ������С�ڵ� n ��Ԫ�ص�Ԫ�س���������ǰ�棬�������ĳ��������ĺ���
	template <class RandomIter>
	void nth_element(RandomIter first, RandomIter nth,
		RandomIter last)
	{
		if (nth == last)
			return;
		while (last - first > 3)
		{
			auto cut = saystl::unchecked_partition(first, last, saystl::median(*first,
				*(first + (last - first) / 2),
				*(last - 1)));
			if (cut <= nth)  // ��� nth λ���Ҷ�
				first = cut;   // ���Ҷν��зָ�
			else
				last = cut;    // ����ν��зָ�
		}
		saystl::insertion_sort(first, last);
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class RandomIter, class Compared>
	void nth_element(RandomIter first, RandomIter nth,
		RandomIter last, Compared comp)
	{
		if (nth == last)
			return;
		while (last - first > 3)
		{
			auto cut = saystl::unchecked_partition(first, last, saystl::median(*first,
				*(first + (last - first) / 2),
				*(last - 1)), comp);
			if (cut <= nth)  // ��� nth λ���Ҷ�
				first = cut;   // ���Ҷν��зָ�
			else
				last = cut;    // ����ν��зָ�
		}
		saystl::insertion_sort(first, last, comp);
	}

	// unique_copy
	// ��[first, last)�н�Ԫ�ظ��Ƶ� result �ϣ����б�������������ظ���Ԫ�أ�ֻ�Ḵ��һ��
	// unique_copy_dispatch �� forward_iterator_tag �汾
	template <class InputIter, class ForwardIter>
	ForwardIter
		unique_copy_dispatch(InputIter first, InputIter last,
			ForwardIter result, forward_iterator_tag)
	{
		*result = *first;
		while (++first != last)
		{
			if (*result != *first)
				*++result = *first;
		}
		return ++result;
	}

	// unique_copy_dispatch �� output_iterator_tag �汾
	// ���� output iterator ֻ�ܽ���ֻ�����������Բ����� *result != *first �������ж�
	template <class InputIter, class OutputIter>
	OutputIter
		unique_copy_dispatch(InputIter first, InputIter last,
			OutputIter result, output_iterator_tag)
	{
		auto value = *first;
		*result = value;
		while (++first != last)
		{
			if (value != *first)
			{
				value = *first;
				*++result = value;
			}
		}
		return ++result;
	}

	template <class InputIter, class OutputIter>
	OutputIter
		unique_copy(InputIter first, InputIter last, OutputIter result)
	{
		if (first == last)
			return result;
		return saystl::unique_copy_dispatch(first, last, result, iterator_category(result));
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	// unique_copy_dispatch �� forward_iterator_tag �汾
	template <class InputIter, class ForwardIter, class Compared>
	ForwardIter
		unique_copy_dispatch(InputIter first, InputIter last,
			ForwardIter result, forward_iterator_tag, Compared comp)
	{
		*result = *first;
		while (++first != last)
		{
			if (!comp(*result, *first))
				*++result = *first;
		}
		return ++result;
	}

	// unique_copy_dispatch �� output_iterator_tag �汾
	// ���� output iterator ֻ�ܽ���ֻ�����������Բ����� *result != *first �������ж�
	template <class InputIter, class OutputIter, class Compared>
	OutputIter
		unique_copy_dispatch(InputIter first, InputIter last,
			OutputIter result, output_iterator_tag, Compared comp)
	{
		auto value = *first;
		*result = value;
		while (++first != last)
		{
			if (!comp(value, *first))
			{
				value = *first;
				*++result = value;
			}
		}
		return ++result;
	}

	template <class InputIter, class OutputIter, class Compared>
	OutputIter
		unique_copy(InputIter first, InputIter last, OutputIter result, Compared comp)
	{
		if (first == last)
			return result;
		return saystl::unique_copy_dispatch(first, last, result, iterator_category(result), comp);
	}

	// unique
	// �Ƴ�[first, last)���ظ���Ԫ�أ����б������򣬺� remove ���ƣ���Ҳ����������ɾ���ظ�Ԫ��
	template <class ForwardIter>
	ForwardIter unique(ForwardIter first, ForwardIter last)
	{
		first = saystl::adjacent_find(first, last);
		return saystl::unique_copy(first, last, first);
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
	template <class ForwardIter, class Compared>
	ForwardIter unique(ForwardIter first, ForwardIter last, Compared comp)
	{
		first = saystl::adjacent_find(first, last, comp);
		return saystl::unique_copy(first, last, first, comp);
	}

} // !namespace saystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // !SAYTINYSTL_ALGO_H

