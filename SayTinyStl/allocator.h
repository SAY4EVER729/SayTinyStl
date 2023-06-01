#ifndef SAYTINYSTL_ALLOCATOR_H
#define SAYTINYSTL_ALLOCATOR_H

// 这个头文件包含一个模板类 allocator，用于管理内存的分配、释放，对象的构造、析构

#include "construct.h"
#include "util.h"

namespace saystl {

	// 模板类 allocator
	// 模板函数代表数据类型

	template <class T>
	class allocator {
	public:
		typedef T            value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t       size_type;
		typedef ptrdiff_t    difference_type;

	public:
		// 内存配置操作由 ::allocate() 负责，内存释放操作由 ::deallocate() 负责
		static T* allocate();
		static T* allocate(size_type n);

		static void deallocate(T* ptr);
		static void deallocate(T* ptr, size_type n);


		// 对象构造由 ::construct() 负责， 对象析构由 ::destroy() 负责
		static void construct(T* ptr);
		static void construct(T* ptr, const T& value);
		static void construct(T* ptr, T&& value);

		template <class...Args>
		static void construct(T* ptr, Args&& ...args);

		static void destroy(T* ptr);
		static void destroy(T* first, T* last);
	};

	template <class T>
	inline T* allocator<T>::allocate() {
		return static_cast<T*>(::operator new(sizeof(T)));
	}

	template <class T>
	inline T* allocator<T>::allocate(size_type n) {
		if (n == 0)
			return nullptr;
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}

	template <class T>
	inline void allocator<T>::deallocate(T* ptr) {
		if (ptr == nullptr)
			return;
		::operator delete(ptr);
	}
	// 因为是指针，所以和上一步一样
	template <class T>
	inline void allocator<T>::deallocate(T* ptr, size_type) {
		if (ptr == nullptr)
			return;
		::operator delete(ptr);
	}

	// 对象配置的实现  用construct
	template <class T>
	inline void allocator<T>::construct(T* ptr) {
		saystl::construct(ptr);
	}

	template <class T>
	inline void allocator<T>::construct(T* ptr, const T& value) {
		saystl::construct(ptr, value);
	}

	template <class T>
	inline void allocator<T>::construct(T* ptr, T&& value) {
		saystl::construct(ptr, saystl::move(value));
	}

	template< class T>
	template <class...Args>
	inline void allocator<T>::construct(T* ptr, Args&& ...args) {
		saystl::construct(ptr, saystl::forward<Args>(args)...);
	}

	template <class T>
	inline void allocator<T>::destroy(T* ptr) {
		saystl::destroy(ptr);
	}

	template <class T>
	inline void allocator<T>::destroy(T* first, T* last) {
		saystl::destroy(first, last);
	}

}

#endif // !SAYTINYSTL_ALLOCATOR_H


