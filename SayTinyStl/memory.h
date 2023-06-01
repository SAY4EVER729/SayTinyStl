#ifndef SAYTINYSTL_MEMORY_H
#define SAYTINYSTL_MEMORY_H

// ���ͷ�ļ�������߼��Ķ�̬�ڴ����
// ����һЩ�����������ռ���������δ��ʼ���Ĵ���ռ�����Լ�һ��ģ���� auto_ptr
// ������ (allocator) �����ڴ��ļ���

//#include <cstdlib> // yvals_core.h / stdlib.h / math.h
//#include <climits> // ��ֵͷ�ļ�

#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#endif

#if defined _MSC_VER
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>
#include <climits>
#endif // �����������ʹ�� free �� malloc
#include <cstddef>//nullptr_t : nullptr������; size_t :һ�������������ͣ���ʾ��С(��Ԫ�صĸ���); ptrdiff_t : һ�ִ����������ͣ���ʾָ��֮��ľ���


#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"

namespace saystl {

	// ��ȡ�����ַ
	template <class Tp>
	constexpr Tp* address_of(Tp& value) noexcept {
		return &value;
	}

	// ��ȡ / �ͷ� ��ʱ������
	template <class T>
	pair<T*, ptrdiff_t> get_buffer_helper(ptrdiff_t len, T*) {
		if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T)))
			len = INT_MAX / sizeof(T);
		while (len > 0) {
			T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
			if (tmp)
				return pair<T*, ptrdiff_t>(tmp, len);
			len /= 2;  // ����ʧ��ʱ���� len �Ĵ�С, �ظ� while ѭ��
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
		free(ptr);  //Ϊʲô������ɫ��
	}

	//==================================================================================================
	// ��ģ�� : temporary_buffer
	// ������ʱ���������������ͷ�
	template <class ForwardIterator, class T>
	class temporary_buffer {

	private:
		ptrdiff_t original_len;  // ����������Ĵ�С
		ptrdiff_t len;           // ������ʵ�ʵĴ�С
		T* buffer;        // ָ�򻺳�����ָ��

	public:
		// ���캯��
		temporary_buffer(ForwardIterator first, ForwardIterator last);

		// ��������
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

	// ����ʵ�ֹ��캯��
	template <class ForwardIterator, class T>
	temporary_buffer<ForwardIterator, T>::
		temporary_buffer(ForwardIterator first, ForwardIterator last) {
		try {
			len = saystl::distance(first, last);
			allocate_buffer(); // ͨ���˺����������飬ͨ���·���ȡ����ú����������Ԫ��
			if (len > 0)
				initialize_buffer(*first, std::is_trivially_default_constructible<T>()); //��ȡ����ģ���Ƶ������������ں���
		}
		catch (...) {
			free(buffer);
			buffer = nullptr;
			len = 0;
		}
	}

	// allocate_buffer ����
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
			len /= 2;  // ����ʧ��ʱ��������ռ��С
		}
	}

	// auto_ptr   smart pointer
	template <class T>
	class auto_ptr {

	public:
		// ���졢��ֵ����������
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
		T* m_ptr;  // ʵ��ָ��

	public:
		// ���� operator* �� operator->
		T& operator*()  const { return *m_ptr; }
		T* operator->() const { return m_ptr; }

		// ���ָ��
		T* get() const { return m_ptr; }

		// �ͷ�ָ��
		T* release() {
			T* tmp = m_ptr;
			m_ptr = nullptr;
			return tmp;
		}

		// ����ָ��
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


