#ifndef SAYTINYSTL_CONSTRUCT_H
#define SAYTINYSTL_CONSTRUCT_H

// ���ͷ�ļ������������� construct��destroy
// construct : �������Ĺ���
// destroy   : ������������

#include <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER						// ΢���Ԥ�������
#pragma warning(push)				// ���浱ǰ�ı���������״̬
#pragma warning(disable : 4100)		// �� ��4100 ������ΪʧЧ��

#endif // _MSC_VER

namespace saystl {

	// construct �������
		// ��һ���汾������һ��ָ��
	template <class Ty>
	inline void construct(Ty* ptr) {
		::new ((void*)ptr) Ty();
	}

	template <class Ty1, class Ty2>
	inline void construct(Ty1* ptr, const Ty2& value) {
		::new ((void*)ptr) Ty1(value);
	}

	template <class Ty, class...Args>
	inline void construct(Ty* ptr, Args&&... args) {
		::new ((void*)ptr) Ty(saystl::forward<Args>(args)...);
	}

	// destroy ��������
		// ��һ���汾 ����һ��ָ��
		// ���Ԫ�ص���ֵ�ͱ� (value_type) �� trivial type �����
	template <class Ty>
	inline void destroy_one(Ty*, std::true_type) {}

	// ���Ԫ�ص���ֵ�ͱ� (value_type) non-trivial destructor ����øö������������
	template <class Ty>
	inline void destroy_one(Ty* pointer, std::false_type) {
		if (pointer != nullptr) {
			pointer->~Ty(); // ���� dtor ~Ty()
		}
	}


	// ������ destroy() �ڶ��汾���������������������к����跨�ҳ�Ԫ�ص���ֵ�ͱ�
	template <class ForwardIter>
	void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

	template <class ForwardIter>
	void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
		for (; first != last; ++first)
			destroy(&*first); //����� non-trivial-type , ���������destroy(һ��ָ��İ汾)
	}

	// destroy(һ��ָ��İ汾)
	template <class Ty>
	void destroy(Ty* pointer)
	{
		destroy_one(pointer, std::is_trivially_destructible<Ty>{}); //���ø���������� dtor ~Ty()
	}

	//destroy(һ���������İ汾)
	//����յ�һ�������������� destroy_cat �ж��Ƿ�Ϊ non-trivial-destructible-type
	template <class ForwardIter>
	void destroy(ForwardIter first, ForwardIter last) {
		destroy_cat(first, last, std::is_trivially_destructible<
			typename iterator_traits<ForwardIter>::value_type>{});
	}

} // namespace saystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // !SAYTINYSTL_CONSTRUCT_H


