#ifndef SAYTINYSTL_ITERATOR_H
#define SAYTINYSTL_ITERATOR_H

//���ͷ�ļ����ڵ���������ƣ�������һЩģ��ṹ���ȫ�ֺ���

#include <cstddef>

#include "type_traits.h"

namespace saystl {

	//���ֵ���������
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//��Ƶĵ��������̳���STL�� iterator class(��������һ©��)
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
	//	//��������ģ�嶨�������ģ����������ƣ�ֻ����ʵ�����Ĳ����д��������������
	//	//�����������ǰ��ʹ����typename�ؼ������Σ��������ŻὫ������ֵ��������͡�
	//	//��������������������������Ὣ����Ϊ���͡�
	//	typedef typename Iterator::iterator_category ietrator_category;//���������
	//	typedef typename Iterator::value_type        value_type;       //��������ָ������ͱ�
	//	typedef typename Iterator::difference_type   difference_type;  //������������ľ���
	//	typedef typename Iterator::pointer           pointer;          //����һ��pointer��ָ���������ָ֮��
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
		static const bool value = sizeof(test<T>(0)) == sizeof(char);//����ʲô��˼��
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

	//is_convertibleģ�����ڼ���Ƿ���Խ��κ���������A��ʽת��Ϊ�κ���������B�������ز���ֵtrue��false
	//helper�̳���impl,�������ת����input��output,��ô����<Iterator, true>,���ӵ��������ԣ�����û��
	template <class Iterator>
	struct iterator_traits_helper <Iterator, true> : public iterator_traits_impl <Iterator,
		std::is_convertible <typename Iterator::iterator_category, input_iterator_tag>::value ||
		std::is_convertible <typename Iterator::iterator_category, output_iterator_tag>::value> {};

	// ��ȡ������������
	template <class Iterator>
	struct iterator_traits : public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value> {};

	//���ԭ��ָ��(native pointer)����Ƶ�traitsƫ�ػ��汾
	template <class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag     iterator_category;
		typedef T                             value_type;
		typedef ptrdiff_t                     difference_type;
		typedef T* pointer;
		typedef T& reference;
	};
	//���ԭ��ָ��֮pointer-to-constant����Ƶ�traitsƫ�ػ��汾
	template <class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag     iterator_category;
		typedef T                             value_type;
		typedef ptrdiff_t                     difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};

	//���ڶԱȵ�������category�Ƿ����ת��ΪT2(in/output)
	template <class T1, class T2, bool = has_iterator_cat<iterator_traits<T1>>::value>
	struct has_iterator_cat_of : public say_bool_constant<std::is_convertible<
		typename iterator_traits<T1>::iterator_category, T2>::value> {};

	// ��ȡĳ�ֵ����� cat_of ���ذ汾������ת����false
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

	//�ж��Ƿ���һ���̳���output �� input �ĵ�����
	template <class Iterator>
	struct is_iterator : public say_bool_constant <is_input_iterator<Iterator>::value ||
		is_output_iterator<Iterator>::value> {};

	//��ȡĳ���������� category
	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category Category;
		return Category();
	}
	//��ȡĳ���������� distance_type
	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//��ȡĳ���������� value_type
	template <class Iterator>
	typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//����������distance����
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

	// distance �� random_access_iterator_tag �����ذ汾
	template <class RandomAccessIterator>
	inline typename iterator_traits <RandomAccessIterator>::difference_type
		distance_dispatch(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}

	//ͨ��distance ���� ���ذ汾��distance_dispatch(�ĸ�����ĸ���
	template <class InputIterator>
	inline typename iterator_traits <InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {
		//typedef typename iterator_traits<InputIterator>::iterator_category category;
		return distance_dispatch(first, last, iterator_category(first));
	}

	//���������� advance ����  �õ�����ǰ�� n ������
	// advance �� input_iterator_tag �İ汾
	template <class InputIterator, class Distance>
	inline void advance_dispatch(InputIterator& i, Distance n, input_iterator_tag) {
		while (n--) ++i;
	}

	// advance �� bidirectional_iterator_tag �İ汾
	template <class BidirectionalIterator, class Distance>
	inline void advance_dispatch(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
		if (n >= 0)
			while (n--)  ++i;
		else
			while (n++)  --i;
	}

	// advance �� random_access_iterator_tag �İ汾
	template <class RandomAccessIterator, class Distance>
	inline void advance_dispatch(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
		i += n;
	}

	//ͨ�� advance ���� ���ذ汾��advance_dispatch(�ĸ�����ĸ���
	template <class InputIterator, class Distance>
	void advance(InputIterator& i, Distance n) {
		advance_dispatch(i, n, iterator_category(i));
	}

	/*=====================================================================================================*/
	// ģ���� �� reverse_iterator
	// �������������ʹǰ��Ϊ���ˣ�����Ϊǰ��
	template <class Iterator>
	class reverse_iterator {
	private:
		Iterator current; //��Ӧ���������
	public:
		typedef typename iterator_traits<Iterator>::iterator_category  iterator_category;
		typedef typename iterator_traits<Iterator>::value_type         value_type;
		typedef typename iterator_traits<Iterator>::difference_type    difference_type;
		typedef typename iterator_traits<Iterator>::pointer            pointer;
		typedef typename iterator_traits<Iterator>::reference          reference;

		typedef Iterator                                               iterator_type;
		typedef reverse_iterator<Iterator>                             self;

	public:
		//���캯��
		reverse_iterator() {}
		explicit reverse_iterator(iterator_type i) : current(i) {} // explicit�ؼ��ֵ����þ��Ƿ�ֹ�๹�캯������ʽ�Զ�ת��
		reverse_iterator(const self& rhs) :current(rhs.current) {} // ��������

	public:
		//ȡ����Ӧ�����������
		inline iterator_type base() const { return current; }

		//���ز�����
		inline reference operator*() const {
			// ʵ�ʶ�Ӧ�����������ǰһ��λ�ã�����--
			auto tmp = current;
			return *--tmp;
		}

		inline pointer operator->() const {
			return &(operator*());
		}

		//ǰ++
		self& operator++() {
			--current;
			return *this;
		}
		//��++ �������int����
		self operator++(int) {
			self tmp = *this;
			--current;
			return tmp;
		}
		//����(--)��Ϊǰ��(++)
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
	// ���� operator-
	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type
		operator-(const reverse_iterator<Iterator>& lhs,
			const reverse_iterator<Iterator>& rhs) {
		return rhs.base() - lhs.base();
	}

	// ���� operator==
	template <class Iterator>
	bool operator==(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return rhs.base() == lhs.base();
	}

	// ���� operator <
	template <class Iterator>
	bool operator<(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return rhs.base() < lhs.base();
	}

	// ���� operator >
	template <class Iterator>
	bool operator>(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return rhs < lhs;
	}

	// ���� operator !=
	template <class Iterator>
	bool operator!=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(rhs == lhs);
	}

	// ���� operator <=
	template <class Iterator>
	bool operator<=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(rhs < lhs);
	}

	// ���� operator >=
	template <class Iterator>
	bool operator>=(const reverse_iterator<Iterator>& lhs,
		const reverse_iterator<Iterator>& rhs) {
		return !(lhs < rhs);
	}

}//saystl

#endif //SAYTINYSTL_ITERATOR_H

