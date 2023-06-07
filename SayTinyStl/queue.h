#ifndef SAYTINYSTL_QUEUE_H
#define SAYTINYSTL_QUEUE_H

// ���ͷ�ļ�����������ģ���� queue �� priority_queue
// queue          : ����
// priority_queue : ���ȶ���

// ���� �� ���ȶ��� ����deque�Ϲ����ģ�Ҳ����������

#include "deque.h"
#include "vector.h"
#include "functional.h"
#include "heap_algo.h"

namespace saystl {

// ģ���� queue
// ����һ�����������ͣ�����������ײ��������ͣ�ȱʡʹ�� saystl::deque ��Ϊ�ײ�����

template <class T, class Container = saystl::deque<T>>
class queue {
public:
	typedef Container                             container_type;

	// ʹ�õײ��������ͱ�
	typedef typename Container::value_type        value_type;
	typedef typename Container::size_type         size_type;
	typedef typename Container::reference         reference;
	typedef typename Container::const_reference   const_reference;

	static_assert(std::is_same<T, value_type>::value,
		"the value_type of Container should be same with T");

private:
	container_type c_;  //�õײ��������� queue

public:
	queue() = default;

	explicit queue(size_type n) : c_(n) {}
	
	queue(size_type n, const value_type& value) :c_(n, value) {}

	template <class IIter>
	queue(IIter first, IIter last) : c_(first, last) {}

	queue(std::initializer_list<T> ilist) :c_(ilist.begin(), ilist.end()) {}

	queue(const Container& c) : c_(c) {}

	queue(Container&& c) noexcept(std::is_nothrow_constructible<Container>::value)
		: c_(saystl::move(c)) {}

	queue(const queue& rhs) : c_(rhs.c_) {}

	queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
		:c_(saystl::move(rhs.c_)) {}

	queue& operator=(const queue& rhs) {
		c_ = rhs.c_;
		return *this;
	}

	queue& operator=(queue&& rhs) noexcept(std::is_nothrow_constructible<Container>::value) {
		c_ = move(rhs.c_);
		return *this;
	}

	queue& operator=(std::initializer_list<T> ilist) {
		c_ = ilist;
		return *this;
	}

	~queue() = default;

	// ����Ԫ�ص���ز���
	reference       front()       { return c_.front(); }
	const_reference front() const { return c_.front(); }
	reference       back()        { return c_.back(); }
	const_reference back()  const { return c_.back(); }

	// ��������
	bool  empty() const noexcept { return c_.empty(); }
	size_type size()  const noexcept { return c_.size(); }

	// �޸�������ز���
	template <class ...Args>
	void emplace(Args&& ...args) {
		c_.emplace_back(saystl::forward<Args>(args)...);
	}

	void push(const value_type& value) {
		c_.push_back(value);
	}
	void push(value_type&& value) {
		c_.emplace_back(saystl::move(value));
	}

	void pop() {
		c_.pop_front();
	}

	void clear() {
		while (!empty())
			pop();
	}

	void swap(queue& rhs) noexcept(noexcept(saystl::swap(c_, rhs.c_))) {
		saystl::swap(c_, rhs.c_);
	}

public:
	friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.c_ == rhs.c_; }
	friend bool operator< (const queue& lhs, const queue& rhs) { return lhs.c_ < rhs.c_; }
};

// ���رȽϲ�����
template <class T, class Container>
bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
	return lhs == rhs;
}

template <class T, class Container>
bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
	return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
	return lhs < rhs;
}

template <class T, class Container>
bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
	return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
	return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs) {
	return !(lhs < rhs);
}

// ���� saystl �� swap
template <class T, class Container>
void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
	lhs.swap(rhs);
}

/*****************************************************************************************/

// ģ���� priority_queue
// ����һ�����������ͣ������������������ͣ�ȱʡʹ�� saystl::vector ��Ϊ�ײ�����
// ����������Ƚ�Ȩֵ�ķ�ʽ��ȱʡʹ�� saystl::less ��Ϊ�ȽϷ�ʽ
template <class T, class Container = saystl::vector<T>,
	class Compare = saystl::less<typename Container::value_type>>
	class priority_queue
{
public:
	typedef Container                           container_type;
	typedef Compare                             value_compare;
	// ʹ�õײ��������ͱ�
	typedef typename Container::value_type      value_type;
	typedef typename Container::size_type       size_type;
	typedef typename Container::reference       reference;
	typedef typename Container::const_reference const_reference;

	static_assert(std::is_same<T, value_type>::value,
		"the value_type of Container should be same with T");

private:
	container_type c_;     // �õײ����������� priority_queue
	value_compare  comp_;  // Ȩֵ�Ƚϵı�׼

public:
	// ���졢���ơ��ƶ�����
	priority_queue() = default;

	priority_queue(const Compare& c)
		:c_(), comp_(c){}

	explicit priority_queue(size_type n)
		:c_(n){
		saystl::make_heap(c_.begin(), c_.end(), comp_);
	}
	priority_queue(size_type n, const value_type& value)
		:c_(n, value){
		saystl::make_heap(c_.begin(), c_.end(), comp_);
	}

	template <class IIter>
	priority_queue(IIter first, IIter last)
		: c_(first, last){
		saystl::make_heap(c_.begin(), c_.end(), comp_);
	}

	priority_queue(std::initializer_list<T> ilist)
		:c_(ilist){
		saystl::make_heap(c_.begin(), c_.end(), comp_);
	}

	priority_queue(const Container& s)
		:c_(s){
		saystl::make_heap(c_.begin(), c_.end(), comp_);
	}
	priority_queue(Container&& s)
		:c_(saystl::move(s)){
		saystl::make_heap(c_.begin(), c_.end(), comp_);
	}

	priority_queue(const priority_queue& rhs)
		:c_(rhs.c_), comp_(rhs.comp_){
		saystl::make_heap(c_.begin(), c_.end(), comp_);
	}
	priority_queue(priority_queue&& rhs)
		:c_(saystl::move(rhs.c_)), comp_(rhs.comp_){
		saystl::make_heap(c_.begin(), c_.end(), comp_);
	}

	priority_queue& operator=(const priority_queue& rhs){
		c_ = rhs.c_;
		comp_ = rhs.comp_;
		saystl::make_heap(c_.begin(), c_.end(), comp_);
		return *this;
	}
	priority_queue& operator=(priority_queue&& rhs){
		c_ = saystl::move(rhs.c_);
		comp_ = rhs.comp_;
		saystl::make_heap(c_.begin(), c_.end(), comp_);
		return *this;
	}
	priority_queue& operator=(std::initializer_list<T> ilist){
		c_ = ilist;
		comp_ = value_compare();
		saystl::make_heap(c_.begin(), c_.end(), comp_);
		return *this;
	}

	~priority_queue() = default;

public:

	// ����Ԫ����ز���
	const_reference top() const { return c_.front(); }

	// ������ز���
	bool      empty() const noexcept { return c_.empty(); }
	size_type size()  const noexcept { return c_.size(); }

	// �޸�������ز���
	template <class... Args>
	void emplace(Args&& ...args)
	{
		c_.emplace_back(saystl::forward<Args>(args)...);
		saystl::push_heap(c_.begin(), c_.end(), comp_);
	}

	void push(const value_type& value)
	{
		c_.push_back(value);
		saystl::push_heap(c_.begin(), c_.end(), comp_);
	}
	void push(value_type&& value)
	{
		c_.push_back(saystl::move(value));
		saystl::push_heap(c_.begin(), c_.end(), comp_);
	}

	void pop()
	{
		saystl::pop_heap(c_.begin(), c_.end(), comp_);
		c_.pop_back();
	}

	void clear()
	{
		while (!empty())
			pop();
	}

	void swap(priority_queue& rhs) noexcept(noexcept(saystl::swap(c_, rhs.c_)) &&
		noexcept(saystl::swap(comp_, rhs.comp_)))
	{
		saystl::swap(c_, rhs.c_);
		saystl::swap(comp_, rhs.comp_);
	}

public:
	friend bool operator==(const priority_queue& lhs, const priority_queue& rhs)
	{
		return lhs.c_ == rhs.c_;
	}
	friend bool operator!=(const priority_queue& lhs, const priority_queue& rhs)
	{
		return lhs.c_ != rhs.c_;
	}
};

// ���رȽϲ�����
template <class T, class Container, class Compare>
bool operator==(priority_queue<T, Container, Compare>& lhs,
	priority_queue<T, Container, Compare>& rhs)
{
	return lhs == rhs;
}

template <class T, class Container, class Compare>
bool operator!=(priority_queue<T, Container, Compare>& lhs,
	priority_queue<T, Container, Compare>& rhs)
{
	return lhs != rhs;
}

// ���� saystl �� swap
template <class T, class Container, class Compare>
void swap(priority_queue<T, Container, Compare>& lhs,
	priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}



}// !namespace saystl

#endif // !SAYTINYSTL_QUEUE_H

