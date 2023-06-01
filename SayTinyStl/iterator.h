#ifndef SAYTINYSTL_ITERATOR_H
#define SAYTINYSTL_ITERATOR_H

//这个头文件用于迭代器的设计，包含了一些模板结构体和全局函数

#include <cstddef>

#include "type_traits.h"

namespace saystl {

	//五种迭代器类型
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//设计的迭代器都继承于STL的 iterator class(避免代码挂一漏万)
	template <class Category, class T, class Distance = ptrdiff_t,
		class Pointer = T*, class Reference = T&>
	struct iterator {
		typedef Category      iterator_category;
		typedef T             value_type;
		typedef Distance      difference_type;
		typedef Pointer       pointer;
		typedef Reference     reference;
	};
	//traits
	//template <class Iterator>
	//struct iterator_traits {
	//	//对于用于模板定义的依赖模板参数的名称，只有在实例化的参数中存在这个类型名，
	//	//或者这个名称前面使用了typename关键字修饰，编译器才会将这个名字当做是类型。
	//	//除了以上两种情况，编译器不会将它视为类型。
	//	typedef typename Iterator::iterator_category ietrator_category;//五类迭代器
	//	typedef typename Iterator::value_type        value_type;       //迭代器所指对象的型别
	//	typedef typename Iterator::difference_type   difference_type;  //两个迭代器间的距离
	//	typedef typename Iterator::pointer           pointer;          //返回一个pointer，指向迭代器所指之物
	//	typedef typename Iterator::reference         reference;        //constant iterators/mutable iterators
	//};
	//iterator
	template <class T>
	struct has_iterator_cat
	{
	private:
		struct two { char a; char b; };
		template <class U>
		static two test(...);
		template <class U>
		static char test(typename U::iterator_category* = 0);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(char);//这是什么意思？
	};

	template <class Iterator, bool>
	struct iterator_traits_impl {};

	template <class Iterator>
	struct iterator_traits_impl<Iterator, true>
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
		typedef typename Iterator::difference_type   difference_type;
	};

	template <class Iterator, bool>
	struct iterator_traits_helper {};

	//is_convertible模板用于检查是否可以将任何数据类型A隐式转换为任何数据类型B。它返回布尔值true或false
	//helper继承于impl,如果可以转换成input或output,那么就是<Iterator, true>,则会拥有五个属性，否则没有
	template <class Iterator>
	struct iterator_traits_helper <Iterator, true> : public iterator_traits_impl <Iterator,
		std::is_convertible <typename Iterator::iterator_category, input_iterator_tag>::value ||
		std::is_convertible <typename Iterator::iterator_category, output_iterator_tag>::value> {};

	// 萃取迭代器的特性
	template <class Iterator>
	struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

	//针对原生指针(native pointer)而设计的traits偏特化版本
	template <class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag     iterator_category;
		typedef T                             value_type;
		typedef ptrdiff_t                     difference_type;
		typedef T* pointer;
		typedef T& reference;
	};
	//针对原生指针之pointer-to-constant而设计的traits偏特化版本
	template <class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag     iterator_category;
		typedef T                             value_type;
		typedef ptrdiff_t                     difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};

	//用于对比迭代器的category是否可以转换为T2(in/output)
	template <class T1, class T2, bool = has_iterator_cat<iterator_traits<T1>>::value>
	struct has_iterator_cat_of : public say_bool_constant<std::is_convertible<
		typename iterator_traits<T1>::iterator_category, T2>::value> {};

	// 萃取某种迭代器 cat_of 重载版本，不能转换，false
	template <class T1, class T2>
	struct has_iterator_cat_of <T1, T2, false> : public say_false_type {};

	template <class Iter>
	struct is_input_iterator : public has_iterator_cat_of <Iter, input_iterator_tag> {};

	template <class Iter>
	struct is_output_iterator : public has_iterator_cat_of <Iter, output_iterator_tag> {};

	template <class Iter>
	struct is_forward_iterator : public has_iterator_cat_of <Iter, forward_iterator_tag> {};

	template <class Iter>
	struct is_bidirectional_iterator : public has_iterator_cat_of <Iter, bidirectional_iterator_tag> {};

	template <class Iter>
	struct is_random_access_iterator : public has_iterator_cat_of <Iter, random_access_iterator_tag> {};

	//判断是否是一个继承于output 和 input 的迭代器
	template <class Iterator>
	struct is_iterator : public say_bool_constant <is_input_iterator<Iterator>::value ||
		is_output_iterator<Iterator>::value> {};

	//萃取某个迭代器的 category
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category Category;
		return Category();
	}
	//萃取某个迭代器的 distance_type
	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//萃取某个迭代器的 value_type
	template <class Iterator>
	typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//以下是整组distance函数
	template <class InputIterator>
	inline typename iterator_traits <InputIterator>::difference_type
		distance_dispatch(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits <InputIterator>::difference_type n = 0;
		while (first != last) {
			++first;
			++n;
		}
		return n;
	}

	// distance 的 random_access_iterator_tag 的重载版本
	template <class RandomAccessIterator>
	inline typename iterator_traits <RandomAccessIterator>::difference_type
		distance_dispatch(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}

	//通过distance 调用 重载版本的distance_dispatch(哪个快调哪个）
	template <class InputIterator>
	inline typename iterator_traits <InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {
		//typedef typename iterator_traits<InputIterator>::iterator_category category;
		return distance_dispatch(first, last, iterator_category(first));
	}

	//以下是整组 advance 函数  让迭代器前进 n 个距离
	// advance 的 input_iterator_tag 的版本
	template <class InputIterator, class Distance>
	inline void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
		while (n--) ++i;
	}

	// advance 的 bidirectional_iterator_tag 的版本
	template <class BidirectionalIterator, class Distance>
	inline void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (n--)  ++i;
		else
			while (n++)  --i;
	}

	// advance 的 random_access_iterator_tag 的版本
	template <class RandomAccessIterator, class Distance>
	inline void advance_dispatch(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
		i += n;
	}

	//通过 advance 调用 重载版本的advance_dispatch(哪个快调哪个）
	template <class InputIterator, class Distance>
	void advance(InputIterator& i, Distance n) {
		advance_dispatch(i, n, iterator_category(i));
	}

	/*=====================================================================================================*/
	// 模板类 ： reverse_iterator
	// 代表反向迭代器，使前进为后退，后退为前进
	template <class Iterator>
	class reverse_iterator {
	private:
		Iterator current; //对应正向迭代器
	public:
		typedef typename iterator_traits<Iterator>::iterator_category  iterator_category;
		typedef typename iterator_traits<Iterator>::value_type         value_type;
		typedef typename iterator_traits<Iterator>::difference_type    difference_type;
		typedef typename iterator_traits<Iterator>::pointer            pointer;
		typedef typename iterator_traits<Iterator>::reference          reference;

		typedef Iterator                                               iterator_type;
		typedef reverse_iterator<Iterator>                             self;

	public:
		//构造函数
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type i) : current(i) {} // explicit关键字的作用就是防止类构造函数的隐式自动转换
		reverse_iterator(const self& rhs) :current(rhs.current) {} // 拷贝构造

	public:
		//取出对应的正向迭代器
		inline iterator_type base() const { return current; }

		//重载操作符
		inline reference operator*() const {
			// 实际对应正向迭代器的前一个位置，所以--
			auto tmp = current;
			return *--tmp;
		}

		inline pointer operator->() const {
			return &(operator*());
		}

		//前++
		self& operator++() {
			--current;
			return *this;
		}
		//后++ 括号里加int区别
		self operator++(int) {
			self tmp = *this;
			--current;
			return tmp;
		}
		//后退(--)变为前进(++)
		self& operator--() {
			++current;
			return *this;
		}
		self operator--(int) {
			self tmp = current;
			++current;
			return tmp;
		}
		self& operator+=(difference_type n) {
			current -= n;
			return *this;
		}
		self operator+(difference_type n) const {
			return (current - n);
		}
		self& operator-=(difference_type n) {
			current += n;
			return *this;
		}
		self operator-(difference_type n) const {
			return self(current + n);
		}
		reference operator[](difference_type n) const {
			return *(*this + n);
		}
	};
	// 重载 operator-
	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type
		operator-(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs) {
		return rhs.base() - lhs.base();
	}

	// 重载 operator==
	template <class Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return rhs.base() == lhs.base();
	}

	// 重载 operator <
	template <class Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return rhs.base() < lhs.base();
	}

	// 重载 operator >
	template <class Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return rhs < lhs;
	}

	// 重载 operator !=
	template <class Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(rhs == lhs);
	}

	// 重载 operator <=
	template <class Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(rhs < lhs);
	}

	// 重载 operator >=
	template <class Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(lhs < rhs);
	}

}//saystl

#endif //SAYTINYSTL_ITERATOR_H

