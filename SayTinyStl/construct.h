#ifndef SAYTINYSTL_CONSTRUCT_H
#define SAYTINYSTL_CONSTRUCT_H

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>

#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER						// 微软的预编译控制
#pragma warning(push)				// 保存当前的编译器警告状态
#pragma warning(disable : 4100)		// 将 第4100 警报置为失效。

#endif // _MSC_VER

namespace saystl {

	// construct 构造对象
		// 第一个版本，接受一个指针
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

	// destroy 析构对象
		// 第一个版本 接受一个指针
		// 如果元素的数值型别 (value_type) 有 trivial type 则忽略
	template <class Ty>
	inline void destroy_one(Ty*, std::true_type) {}

	// 如果元素的数值型别 (value_type) non-trivial destructor 则调用该对象的析构函数
	template <class Ty>
	inline void destroy_one(Ty* pointer, std::false_type) {
		if (pointer != nullptr) {
			pointer->~Ty(); // 调用 dtor ~Ty()
		}
	}


	// 以下是 destroy() 第二版本，接受两个迭代器，下列函数设法找出元素的数值型别
	template <class ForwardIter>
	void destroy_cat(ForwardIter, ForwardIter, std::true_type) {}

	template <class ForwardIter>
	void destroy_cat(ForwardIter first, ForwardIter last, std::false_type) {
		for (; first != last; ++first)
			destroy(&*first); //如果是 non-trivial-type , 调用下面的destroy(一个指针的版本)
	}

	// destroy(一个指针的版本)
	template <class Ty>
	void destroy(Ty* pointer)
	{
		destroy_one(pointer, std::is_trivially_destructible<Ty>{}); //调用该类型自身的 dtor ~Ty()
	}

	//destroy(一个迭代器的版本)
	//如果收到一个迭代器，则用 destroy_cat 判断是否为 non-trivial-destructible-type
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


