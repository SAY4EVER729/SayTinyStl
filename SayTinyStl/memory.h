#ifndef SAYTINYSTL_MEMORY_H
#define SAYTINYSTL_MEMORY_H

// 这个头文件负责更高级的动态内存管理
// 包含一些基本函数、空间配置器、未初始化的储存空间管理，以及一个模板类 auto_ptr
// 配置器 (allocator) 定义于此文件中

//#include <cstdlib> // yvals_core.h / stdlib.h / math.h
//#include <climits> // 最值头文件

#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif

#if defined _MSC_VER
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>
#include <climits>
#endif // 加上这个就能使用 free 和 malloc
#include <cstddef>//nullptr_t : nullptr的类型; size_t :一种无正负号类型，表示大小(如元素的个数); ptrdiff_t : 一种带正负号类型，表示指针之间的距离


#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace saystl {

	// 获取对象地址
	template <class Tp>
	constexpr Tp* address_of(Tp& value) noexcept {
		return &value;
	}

	// 获取 / 释放 临时缓冲区
	template <class T>
	pair<T*, ptrdiff_t> get_buffer_helper(ptrdiff_t len, T*) {
		if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		while (len > 0) {
			T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
			if (tmp)
				return pair<T*, ptrdiff_t>(tmp, len);
			len /= 2;  // 申请失败时减少 len 的大小, 重复 while 循环
		}
		return pair<T*, ptrdiff_t>(nullptr, 0);
	}

	template <class T>
	pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
		return get_buffer_helper(len, static_cast<T*>(0));
	}

	template <class T>
	pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*) {
		return get_buffer_helper(len, static_cast<T*>(0));
	}

	template <class T>
	void release_temporary_buffer(T* ptr) {
		free(ptr);  //为什么不变颜色？
	}

	//==================================================================================================
	// 类模板 : temporary_buffer
	// 进行临时缓冲区的申请与释放
	template <class ForwardIterator, class T>
	class temporary_buffer {

	private:
		ptrdiff_t original_len;  // 缓冲区申请的大小
		ptrdiff_t len;           // 缓冲区实际的大小
		T* buffer;        // 指向缓冲区的指针

	public:
		// 构造函数
		temporary_buffer(ForwardIterator first, ForwardIterator last);

		// 析构函数
		~temporary_buffer() {
			saystl::destroy(buffer, buffer + len);
			free(buffer);
		}

	public:
		ptrdiff_t size()           const noexcept { return len; }
		ptrdiff_t requested_size() const noexcept { return original_len; }
		T* begin()                noexcept { return buffer; }
		T* end()                  noexcept { return buffer + len; }

	private:
		void allocate_buffer();
		// POD 
		void initialize_buffer(const T&, std::true_type) {}
		// non-POD
		void initialize_buffer(const T& value, std::false_type)
		{
			saystl::uninitialized_fill_n(buffer, len, value);
		}

	}; // !class temporary_buffer

	// 类外实现构造函数
	template <class ForwardIterator, class T>
	temporary_buffer<ForwardIterator, T>::
		temporary_buffer(ForwardIterator first, ForwardIterator last) {
		try {
			len = saystl::distance(first, last);
			allocate_buffer(); // 通过此函数申请区块，通过下方萃取其调用函数填充数据元素
			if (len > 0)
				initialize_buffer(*first, std::is_trivially_default_constructible<T>()); //萃取器，模板推导调用上面类内函数
		}
		catch (...) {
			free(buffer);
			buffer = nullptr;
			len = 0;
		}
	}

	// allocate_buffer 函数
	template <class ForwardIterator, class T>
	void temporary_buffer<ForwardIterator, T>::allocate_buffer()
	{
		original_len = len;
		if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		while (len > 0)
		{
			buffer = static_cast<T*>(malloc(len * sizeof(T)));
			if (buffer)
				break;
			len /= 2;  // 申请失败时减少申请空间大小
		}
	}

	// auto_ptr   smart pointer
	template <class T>
	class auto_ptr {

	public:
		// 构造、赋值、析构函数
		explicit auto_ptr(T* p = 0) : m_ptr(p) {}
		auto_ptr(auto_ptr& rhs) :m_ptr(rhs.release()) {}
		template <class U>
		auto_ptr(auto_ptr<U>& rhs) : m_ptr(rhs.release()) {}

		auto_ptr& operator=(auto_ptr& rhs)
		{
			if (this != &rhs)
			{
				delete m_ptr;
				m_ptr = rhs.release();
			}
			return *this;
		}
		template <class U>
		auto_ptr& operator=(auto_ptr<U>& rhs)
		{
			if (this->get() != rhs.get())
			{
				delete m_ptr;
				m_ptr = rhs.release();
			}
			return *this;
		}

		~auto_ptr() { delete m_ptr; }

	public:
		typedef T    elem_type;

	private:
		T* m_ptr;  // 实际指针

	public:
		// 重载 operator* 和 operator->
		T& operator*()  const { return *m_ptr; }
		T* operator->() const { return m_ptr; }

		// 获得指针
		T* get() const { return m_ptr; }

		// 释放指针
		T* release() {
			T* tmp = m_ptr;
			m_ptr = nullptr;
			return tmp;
		}

		// 重置指针
		void reset(T* p = nullptr) {
			if (m_ptr != p)
			{
				delete m_ptr;
				m_ptr = p;
			}
		}
	}; //!class auto_ptr

} // !namespace saystl

#endif // !SAYTINYSTL_MEMORY_H


