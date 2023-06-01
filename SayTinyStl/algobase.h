#ifndef SAYSTL_ALGOBASE_H
#define SAYSTL_ALGOBASE_H

// ���ͷ�ļ������� saystl �Ļ����㷨

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
	//  ʹ�ø��㷨ʱ�� �������ж���Ԫ�ظ����Ƿ���ͬ
	//	�Ƚϵ�һ������[first, last)�����ϵ�Ԫ��ֵ�Ƿ�͵ڶ��������
	template <class InputIter1, class InputIter2>
	inline bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2) {
		//������� 1 ��Ԫ�ظ������� ���� 2 ��Ԫ�ظ����� �����
		for (; first1 != last1; ++first1, ++first2)
			if (*first1 != *first2)
				return false;
		return true;
	}
	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
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
	// ȡ���ߵĽϴ�ֵ�����ʱ���ص�ʱ��һ������
	// �汾һ  ʹ�ö����ͱ� T ���ṩ�� greater-than ���������жϴ�С
	template <class T>
	inline const T& max(const T& lhs, const T& rhs) {
		return lhs < rhs ? rhs : lhs;
	}

	// �汾��  ʹ�÷º��� cmp ���жϴ�С
	template <class T, class Compare>
	inline const T& max(const T& lhs, const T& rhs, Compare comp) {
		return comp(lhs, rhs) ? rhs : lhs;
	}

	// min 
	// ȡ���ߵĽ�Сֵ�����ʱ���ص�ʱ��һ������
	// �汾һ  ʹ�ö����ͱ� T ���ṩ�� less-than ���������жϴ�С
	template <class T>
	inline const T& min(const T& lhs, const T& rhs) {
		return rhs < lhs ? rhs : lhs;
	}

	// �汾��  ʹ�÷º��� cmp ���жϴ�С
	template <class T, class Compare>
	inline const T& min(const T& lhs, const T& rhs, Compare comp) {
		return comp(rhs, lhs) ? rhs : lhs;
	}


	// iterator_swap
	// ��������������ָ�Ķ���Ե�
	template <class FirstIter1, class FirstIter2>
	void iter_swap(FirstIter1 lhs, FirstIter2 rhs) {
		saystl::swap(*lhs, *rhs);
	}

	//======================================================================================================
	/*                                   copy

			   copy( �������汾 )   --->  unchecked_copy_cat (3���汾)  --->  n �������ٶ�(����)
								   --->  unchecked_copy  (ָ���ػ��汾) --->  memmove

	 �� [first, last) �����Ԫ�ؿ����� [result, result + (last - first)) �� ���� resul����λ��    */

	 // input_iterator_tag �汾
	template <class InputIter, class OutputIter>
	inline OutputIter
		unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
			saystl::input_iterator_tag) {
		for (; first != last; ++first, ++result) {
			*result = *first;
		}
		return result;
	}

	// ramdom_access_iterator_tag �汾
	template <class InputIter, class OutputIter>
	inline OutputIter
		unchecked_copy_cat(InputIter first, InputIter last, OutputIter result,
			saystl::random_access_iterator_tag) {
		for (auto n = last - first; n > 0; --n, ++first, ++result) { // �� n ����ѭ����ִ�д����� �ٶȿ�
			*result = *first;
		}
		return result;
	}

	// ���ݸ��������ذ汾�� �ñ���������ʹ���ĸ�ģ�庯��
	template <class InputIter, class OutputIter>
	inline OutputIter
		unchecked_copy(InputIter first, InputIter last, OutputIter result) {
		return unchecked_copy_cat(first, last, result, iterator_category(first));
	}


	// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
	template <class Tp, class Up>
	typename std::enable_if<
		std::is_same<typename std::remove_const<Tp>::type, Up>::value&&
		std::is_trivially_copy_assignable<Up>::value, Up*>::type
		unchecked_copy(Tp* first, Tp* last, Up* result) {
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(Up)); // ���� memmove �Ƚ������������帴��������û�и��ǵ�Σ��
		return result + n;
	}

	// ���ݸ�����unchecked_copy���ذ汾�� �ñ���������ʹ�� trivially ���� Iterator ģ�庯��
	template <class InputIter, class OutputIter>
	OutputIter copy(InputIter first, InputIter last, OutputIter result)
	{
		return unchecked_copy(first, last, result);
	}
	//======================================================================================================

	//======================================================================================================
	/*                             copy_backward
	 unchecked_copy_backward(�������汾) --->  unchecked_copy_backward_cat (3���汾)  --->  n �������ٶ�(����)
										--->  unchecked_copy_backward  (ָ���ػ��汾) --->  memmove

	  �� [first, last)�����ڵ�Ԫ�ؿ����� [result - (last - first), result)��
	  �����еķ����Ƶ��� result-1 Ϊ��㣬������Ϊ���е�������
	*/

	// unchecked_copy_backward_cat �������汾
	// bidirectional_iterator_tag �汾
	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result, saystl::bidirectional_iterator_tag) {
		while (first != last)
			*--result = *--last;
		return result;
	}

	// random_access_iterator_tag �汾
	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_copy_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result, saystl::random_access_iterator_tag) {
		for (auto n = last - first; n > 0; --n) {
			*--result = *--last;
		}
		return result;
	}

	// unchecked_copy_backward �������汾
	// 1 ����ᴫ�ݵ� unchecked_copy_backward_cat ����ģ�庯���Ƶ�
	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result) {
		return unchecked_copy_backward_cat(first, last, result, iterator_category(first));
	}

	// 2 unchecked_copy_backward ��ƽ���������� trivially_copy_assignable �ػ��汾
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

	// 3 �������Լ��������� 1 ���� 2
	template <class BidirectionalIter1, class BidirectionalIter2>
	BidirectionalIter2
		copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
	{
		return unchecked_copy_backward(first, last, result);
	}
	//======================================================================================================

	//======================================================================================================
	/*                                           copy_if
			   ��[first, last)������һԪ���� unary_pred ��Ԫ�ؿ������� result Ϊ��ʼ��λ����
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
		�� [first, first + n)�����ϵ�Ԫ�ؿ����� [result, result + n)��, ����һ�� pair �ֱ�ָ�򿽱�������β��
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
					 �� [first, last)�����ڵ�Ԫ���ƶ��� [result, result + (last - first))��
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

	// ramdom_access_iterator_tag �汾
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

	// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
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
					 �� [first, last)�����ڵ�Ԫ���ƶ��� [result - (last - first), result)��
	*/
	// bidirectional_iterator_tag �汾
	template <class BidirectionalIter1, class BidirectionalIter2>
	inline BidirectionalIter2
		unchecked_move_backward_cat(BidirectionalIter1 first, BidirectionalIter1 last,
			BidirectionalIter2 result, saystl::bidirectional_iterator_tag)
	{
		while (first != last)
			*--result = saystl::move(*--last);
		return result;
	}

	// random_access_iterator_tag �汾
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

	// Ϊ trivially_copy_assignable �����ṩ�ػ��汾
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
	// �� [first, last) ��ǰ n ��Ԫ�ظ�����ֵ
	// ���صĵ�����ָ����������һ��Ԫ�ص���һ��λ��
	template <class OutputIter, class Size, class T>
	inline OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value) {
		for (; n > 0; --n, ++first) {
			*first = value;
		}
		return first;
	}

	// Ϊ one-byte �����ṩ�ػ��汾
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
	// �� [first, last) ������Ԫ�ظ�����ֵ
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
			 ���ֵ������ж��������� [first1, last1) �� [first2, last2) ��Ӧλ��Ԫ�ؽ��бȽϣ�ֱ�����м��������
		  1) �����һ���е�Ԫ�ؽ�С������ true �����򷵻� false
		  2) ������� last1 ����δ���� last2 ���� true
		  3) ������� last2 ����δ���� last1 ���� false
		  4) ���ͬʱ���� last1 �� last2 ���� false
			 Ҳ����˵����һ�������ֵ����з�ʽ (lexicographical) ��С�ڵڶ�����
	*/
	template <class InputIter1, class InputIter2>
	bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
		InputIter2 first2, InputIter1 last2) {
		for (; first1 != last1 && first2 != last2; ++first1, ++first2) {

			if (*first1 < *first2) // ��һ���е�Ԫ��С�ڵڶ�����Ԫ��
				return true;
			if (*first2 < *first1)
				return false;
			// ���������������������ʾ��ֵ��ȣ��Ǿͽ���һ������ӦԪ�ضԱ�
		}
		// ������е���������һ���е���β�˶��ڶ����������࣬��ô��һ����С�ڵڶ�����
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

	// ���ԭ��ָ�� const unsigned char* ���ػ��汾
	bool lexicographical_compare(const unsigned char* first1,
		const unsigned char* last1,
		const unsigned char* first2,
		const unsigned char* last2)
	{
		const auto len1 = last1 - first1;
		const auto len2 = last2 - first2;
		// �ȱȽ���ͬ���ȵĲ���
		const auto result = std::memcmp(first1, first2, saystl::min(len1, len2));
		// ����ȣ����Ƚϳ��ıȽϴ�
		return result != 0 ? result < 0 : len1 < len2;
	}

	// mismatch ʧ��
	// ƽ�бȽ��������У��ҵ���һ��ʧ���Ԫ�أ�����һ�Ե��������ֱ�ָ������������ʧ��Ĳ�ƥ���
	// �����ƥ�䣬 �򷵻������и��Ե� last ������
	template <class InputIter1, class InputIter2>
	saystl::pair<InputIter1, InputIter2> mismatch(InputIter1 first1, InputIter1 last1,
		InputIter2 first2) {
		while (first1 != last1 && *first1 == *first2) {
			++first1;
			++first2;
		}
		return saystl::pair<InputIter1, InputIter2>(first1, first2);
	}

	// ���ذ汾ʹ�ú������� comp ����Ƚϲ���
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
