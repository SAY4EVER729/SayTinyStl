#ifndef SAYTINYSTL_UNINITIALIZED_H
#define SAYTINYSTL_UNINITIALIZED_H

// ���ͷ�ļ����ڶ�δ��ʼ���ռ乹��Ԫ��

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace saystl {


	// �����ȫ�ֺ���������δ��ʼ���ռ�  �ֱ��� 
	// construct(), destroy(), uninitialized_copy(), uninitialized_fill(), uninitialized_fill_n()
	// ���µķֱ��Ӧ�ڸ߲�κ��� copy(), fill(), fill_n()
		// uninitialized_copy, true_type ƽ������, Ĭ�Ϲ���
		// uninitialized_copy  ����ʵ������: 1.�����ڴ����飬���԰�������Ԫ��  2.�ڸ������Ϲ���Ԫ��
	template <class InputIter, class ForwardIter>
	ForwardIter
		unchecked_uninit_copy(InputIter first, InputIter last, ForwardIter result, std::true_type) {
		return saystl::copy(first, last, result);
	}

	// ��ƽ���Ķ��󣬵���construct
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

	// ͨ����ȡ�����жϵ��� ƽ������/��ƽ����������
	template <class InputIter, class ForwardIter>
	ForwardIter
		uninitialized_copy(InputIter first, InputIter last, ForwardIter result) {
		return saystl::unchecked_uninit_copy(first, last, result, std::is_trivially_copy_assignable<
			typename iterator_traits<ForwardIter>::
			value_type>{});
	}

	// unintialized_copy_n ��ֵ n �����ݵ� result ��ʼ����λ�ã����ظ��ƽ�����λ��
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
	// �� [first, last) ���������Ԫ��ֵ
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
	// �� first λ�ÿ�ʼ����� n ��Ԫ��ֵ��������������λ��
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

	//��ȡ�����ж��Ƿ�Ϊ POD (Plain Old Data)
	template <class ForwardIter, class Size, class T>
	inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T& value) {
		return saystl::unchecked_uninit_fill_n(first, n, value,
			std::is_trivially_copy_assignable<
			typename iterator_traits<ForwardIter>::
			value_type>{});
	}

	// uninitialized_move
	// ��[first, last)�ϵ������ƶ����� result Ϊ��ʼ���Ŀռ䣬�����ƶ�������λ��
	template <class InputIter, class ForwardIter>
	ForwardIter unchecked_uninit_move(InputIter first, InputIter last, ForwardIter result, std::true_type) {
		return saystl::move(first, last, result); // algobase.h �е� move ���غ���
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

	//��ȡ�����ж��Ƿ�Ϊ POD (Plain Old Data)
	template <class InputIter, class ForwardIter>
	ForwardIter uninitialized_move(InputIter first, InputIter last, ForwardIter result)
	{
		return saystl::unchecked_uninit_move(first, last, result,
			std::is_trivially_move_assignable<
			typename iterator_traits<InputIter>::
			value_type>{});
	}

	// uninitialized_move_n
	// ��[first, first + n)�ϵ������ƶ����� result Ϊ��ʼ���Ŀռ䣬�����ƶ�������λ��
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

	//��ȡ�����ж��Ƿ�Ϊ POD (Plain Old Data)
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


