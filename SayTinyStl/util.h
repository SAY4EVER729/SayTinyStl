#ifndef SAYTINYSTL_UTIL_H
#define SAYTINYSTL_UTIL_H

// 这个文件包含一些通用的工具，包括 move, forward, swap 等函数， 以及 pair

#include <cstddef>
#include "type_traits.h"

namespace saystl {

	// move
	// 右值引用 , 既可以传递给 move 一个左值, 也可以传递给它一个右值 (c primer p611)
	// noexcept 不可以用于指定某个函数不抛出异常。预先知道函数不会抛出异常有助于简化调用该函数的代码，
	//          而且编译器确认函数不会抛出异常，就能执行某些特殊的优化操作。
	template <class T>
	typename std::remove_reference<T>::type&& move(T&& arg) noexcept {
		return static_cast<typename std::remove_reference<T>::type&&>(arg);// 用static_cast显示地将一个左值转换为一个右值引用
	}

	// forward 
	// 转发, 会保持实参类型的所有细节, 可以接受左值、右值
	template <class T>
	T&& forward(typename std::remove_reference<T>::type& arg) noexcept {
		return static_cast<T&&>(arg);
	}

	// 静态断言 static_assert
	// 如果第一个参数常量表达式的值为真(true或者非零值)，那么static_assert不做任何事情，就像它不存在一样，
	// 否则会产生一条编译错误，错误位置就是该static_assert语句所在行，错误提示就是第二个参数提示字符串。
	template <class T>
	T&& forward(typename std::remove_reference<T>::type&& arg) noexcept {
		// is_lvalue_reference 检查 T 是否为左值, 是则提供 value = true, 否则 false
		static_assert(!std::is_lvalue_reference<T>::value, "bad forward");//!true 即 T 为左值 则报错 “bad forwad”
		return static_cast<T&&>(arg);
	}

	// swap

	template <class Tp>
	void swap(Tp& lhs, Tp& rhs) {
		auto tmp(saystl::move(lhs));
		lhs = saystl::move(rhs);
		rhs = saystl::move(tmp);
	}

	// 迭代器swap
	template <class ForwardIter1, class ForwardIter2>
	ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2) {
		for (; first1 != last1; ++first1, (void)++first2)
			saystl::swap(*first1, *first2);
		return first2;
	}

	template <class Tp, size_t N>
	void swap(Tp(&a)[N], Tp(&b)[N]) {
		saystl::swap_range(a, a + N, b);
	}


	//===============================================================================================
	//pair

	// 结构体模板 : pair
	// 两个模板参数分别表示两个数据的类型
	// 用 first 和 second 来分别取出第一个数据和第二个数据
	template <class Ty1, class Ty2>
	struct pair {
		typedef Ty1   first_type;
		typedef Ty2   second_type;

		first_type    first;    //保存第一个数据
		second_type   second;   //保存第二个数据

		// 默认构造函数
		// template< bool B, class T = void > 
		// If B is true, std::enable_if has a public member typedef type, equal to T; otherwise, there is no member typedef
		// Other1, Other2都是默认构造吗？如果都是的话，那么 type =  
		template <class Other1 = Ty1, class Other2 = Ty2,
			typename = typename std::enable_if<
			std::is_default_constructible<Other1>::value&&
			std::is_default_constructible<Other2>::value, void>::type>
		constexpr pair()
			: first(), second()
		{
		}

		//拷贝构造
		template <class U1 = Ty1, class U2 = Ty2,
			typename std::enable_if<
			std::is_copy_constructible<U1>::value&&
			std::is_copy_constructible<U2>::value&&
			std::is_convertible<const U1&, Ty1>::value&&
			std::is_convertible<const U2&, Ty2>::value, int>::type = 0>
		constexpr pair(const Ty1& a, const Ty2& b)
			: first(a), second(b)
		{
		}

		//显式拷贝构造函数
		template <class U1 = Ty1, class U2 = Ty2,
			typename std::enable_if<
			std::is_copy_constructible<U1>::value&&
			std::is_copy_constructible<U2>::value &&
			(!std::is_convertible<const U1&, Ty1>::value ||
				!std::is_convertible<const U2&, Ty2>::value), int>::type = 0>
		explicit constexpr pair(const Ty1& a, const Ty2& b)
			: first(a), second(b)
		{
		}


		pair(const pair& rhs) = default;
		pair(pair&& rhs) = default;

		// 对于其他类型的隐式构造函数
		template < class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, Other1>::value&&
			std::is_constructible<Ty1, Other2>::value&&
			std::is_convertible<Other1&&, Ty1>::value&&
			std::is_convertible<Other2&&, Ty2>::value, int>::type = 0>
		constexpr pair(Other1&& a, Other2&& b)
			: first(saystl::forward<Other1>(a)),
			second(saystl::forward<Other2>(b))
		{

		}

		// 对于其他类型的显式构造函数
		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, Other1>::value&&
			std::is_constructible<Ty1, Other2>::value &&
			(!std::is_convertible<Other1, Ty1>::value ||
				!std::is_convertible<Other2, Ty2>::value), int>::type = 0>
		explicit constexpr pair(Other1&& a, Other2&& b)
			: first(saystl::forward<Other1>(a)),
			second(saystl::forward<Other2>(b))
		{

		}

		// 对于其他 pair 的隐式拷贝构造函数
		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, const Other1>::value&&
			std::is_constructible<Ty1, const Other2>::value&&
			std::is_convertible<const Other1&&, Ty1>::value&&
			std::is_convertible<const Other2&&, Ty2>::value, int>::type = 0>
		constexpr pair(const pair<Other1, Other2>& other)
			: first(other.first),
			second(other.second)
		{

		}

		// 对于其他 pair 的显式拷贝构造函数
		  // explicit constructiable for other pair
		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, const Other1&>::value&&
			std::is_constructible<Ty2, const Other2&>::value &&
			(!std::is_convertible<const Other1&, Ty1>::value ||
				!std::is_convertible<const Other2&, Ty2>::value), int>::type = 0>
		explicit constexpr pair(const pair<Other1, Other2>& other)
			: first(other.first),
			second(other.second)
		{

		}

		// 对于其他 pair 的隐式构造函数
		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, Other1>::value&&
			std::is_constructible<Ty2, Other2>::value&&
			std::is_convertible<Other1, Ty1>::value&&
			std::is_convertible<Other2, Ty2>::value, int>::type = 0>
		constexpr pair(pair<Other1, Other2>&& other)
			: first(saystl::forward<Other1>(other.first)),
			second(saystl::forward<Other2>(other.second))
		{

		}

		// 对于其他 pair 的显式构造函数
		template <class Other1, class Other2,
			typename std::enable_if<
			std::is_constructible<Ty1, Other1>::value&&
			std::is_constructible<Ty2, Other2>::value &&
			(!std::is_convertible<Other1, Ty1>::value ||
				!std::is_convertible<Other2, Ty2>::value), int>::type = 0>
		explicit constexpr pair(pair<Other1, Other2>&& other)
			: first(saystl::forward<Other1>(other.first)),
			second(saystl::forward<Other2>(other.second))
		{

		}

		//拷贝赋值(对于这个pair)
		pair& operator=(pair&& rhs) {
			if (this != rhs) {
				first = saystl::move(rhs.first);
				second = saystl::move(rhs.second);
			}
			return *this;
		}

		//拷贝赋值(对于其他pair)
		template <class Other1, class Other2>
		pair& operator=(const pair<Other1, Other2>& other) {
			first = other.first;
			second = other.second;
			return *this;
		}

		// 搬移赋值(对于其他pair)
		template <class Other1, class Other2>
		pair& operator=(pair<Other1, Other2>&& other)
		{
			first = saystl::forward<Other1>(other.first);
			second = saystl::forward<Other2>(other.second);
			return *this;
		}

		~pair() = default;

		void swap(pair& other) {
			if (this != &other) {
				saystl::swap(first, other.first);
				saystl::swap(second, other.second);
			}
		}

	};

	// 重载比较操作符 
	template <class Ty1, class Ty2>
	bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
		return lhs.first == rhs.first && lhs.second == rhs.second;
	}

	template <class Ty1, class Ty2>
	bool operator<(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
		return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
	}

	template <class Ty1, class Ty2>
	bool operator>(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
		return rhs < lhs;
	}

	template <class Ty1, class Ty2>
	bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
		return !(lhs == rhs);
	}

	template <class Ty1, class Ty2>
	bool operator<=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
		return !(rhs < lhs);
	}

	template <class Ty1, class Ty2>
	bool operator>=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) {
		return !(lhs < rhs);
	}

	// 重载 saystl 的 swap
	template <class Ty1, class Ty2>
	void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs) {
		lhs.swap(rhs);
	}

	// 全局函数，让两个数据类型成为一个pair
	template <class Ty1, class Ty2>
	pair<Ty1, Ty2> make_pair(Ty1&& first, Ty2&& second) {
		return pair<Ty1, Ty2>(saystl::forward<Ty1>(first), saystl::forward<Ty2>(second));
	}

} //saystl

#endif // !SAYTINYSTL_UTIL_H

