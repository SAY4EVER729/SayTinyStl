#ifndef SAYTINYSTL_HEAP_ALGO_H
#define SAYTINYSTL_HEAP_ALGO_H

// 这个头文件包含 heap 的四个算法 : push_heap, pop_heap, sort_heap, make_heap

#include "iterator.h"

namespace saystl {

	// push_heap
	// 该函数接受两个迭代器，表示一个 heap 容器的首尾，并且新元素已经插入到底部容器的最尾端，调整 heap

		// 以下这组 push_back() 不允许指定 “大小比较准则” 提供的是 max_heap, 被 push_heap_d 调用
	template <class RandomIter, class Distance, class T>
	void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value) {
		auto parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && *(first + parent) < value) {
			// 如果尚未达到顶端， 且父节点小于新值 (于是不符合 heap 的次序特性)
			// 由于 < 在 “iterator.h” 已被重载,  返回的是反向迭代器的 rhs.base() < lhs.base() 所以是 max-heap
			// value 是 *(last - 1) ; *(first + parent) 是 holeIndex 的 父节点值
			*(first + holeIndex) = *(first + parent); // 当前的 hole 的值等于 parent 的值
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;             // 重新定义新的父节点
		}
		*(first + holeIndex) = value;                 // 定义最终 hole 的值，也就是插入的节点位的值
	}

	// 调用 push_heap_aux   被 push_heap 调用
	template <class RandomIter, class Distance>
	void push_heap_d(RandomIter first, RandomIter last, Distance*) {
		saystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0), *(last - 1));
		// 新值必置于底部容器最尾端，此即第一个 hole 号：(last - first) - 1
	}

	template <class RandomIter>
	void push_heap(RandomIter first, RandomIter last)
	{ // 新元素应该已置于底部容器的最尾端
		saystl::push_heap_d(first, last, distance_type(first));
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class RandomIter, class Distance, class T, class Compared>
	void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value,
		Compared comp)
	{
		auto parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && comp(*(first + parent), value))
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template <class RandomIter, class Compared, class Distance>
	void push_heap_d(RandomIter first, RandomIter last, Distance*, Compared comp)
	{
		saystl::push_heap_aux(first, (last - first) - 1, static_cast<Distance>(0),
			*(last - 1), comp);
	}

	template <class RandomIter, class Compared>
	void push_heap(RandomIter first, RandomIter last, Compared comp)
	{
		saystl::push_heap_d(first, last, distance_type(first), comp);
	}

	// pop_heap
	// 该函数接受两个迭代器，表示 heap 容器的首尾，将 heap 的根节点取出放到容器尾部，调整 heap
	// 使用 pop_heap 后，最大元素只是被置于放于底部容器的最尾端，如果要移除，需要使用 vector 提供的pop_back()

		// 默认是 max-heap, 所以默认取走最大值
	template <class RandomIter, class T, class Distance>
	void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value)
	{
		// 先进行下溯(percolate down)过程
		auto topIndex = holeIndex;
		auto rchild = 2 * holeIndex + 2;
		while (rchild < len) {
			if (*(first + rchild) < *(first + rchild - 1))
				--rchild;
			*(first + holeIndex) = *(first + rchild);
			holeIndex = rchild;
			rchild = 2 * (rchild + 1);
		}
		if (rchild == len) {  // 如果没有右子节点
			*(first + holeIndex) = *(first + (rchild - 1));
			holeIndex = rchild - 1;
		}
		// 再执行一次上溯(percolate up)过程
		saystl::push_heap_aux(first, holeIndex, topIndex, value);
	}

	template <class RandomIter, class T, class Distance>
	void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value,
		Distance*) {
		// 先将首值调至尾节点，然后调整[first, last - 1)使之重新成为一个 max-heap
		*result = *first;
		saystl::adjust_heap(first, static_cast<Distance>(0), last - first, value);
	}

	template <class RandomIter>
	void pop_heap(RandomIter first, RandomIter last) {
		saystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
	}


	// 重载版本使用函数对象 comp 代替比较操作
	template <class RandomIter, class T, class Distance, class Compared>
	void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value,
		Compared comp)
	{
		// 先进行下溯(percolate down)过程
		auto topIndex = holeIndex;
		auto rchild = 2 * holeIndex + 2;
		while (rchild < len) {
			if (comp(*(first + rchild), *(first + rchild - 1)))  --rchild;
			*(first + holeIndex) = *(first + rchild);
			holeIndex = rchild;
			rchild = 2 * (rchild + 1);
		}
		if (rchild == len) {
			*(first + holeIndex) = *(first + (rchild - 1));
			holeIndex = rchild - 1;
		}
		// 再执行一次上溯(percolate up)过程
		saystl::push_heap_aux(first, holeIndex, topIndex, value, comp);
	}

	template <class RandomIter, class T, class Distance, class Compared>
	void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result,
		T value, Distance*, Compared comp) {
		*result = *first;  // 先将尾指设置成首值，即尾指为欲求结果
		saystl::adjust_heap(first, static_cast<Distance>(0), last - first, value, comp);
	}

	template <class RandomIter, class Compared>
	void pop_heap(RandomIter first, RandomIter last, Compared comp) {
		saystl::pop_heap_aux(first, last - 1, last - 1, *(last - 1),
			distance_type(first), comp);
	}

	// sort_heap
	// 该函数接受两个迭代器，表示 heap 容器的首尾，不断执行 pop_heap 操作，直到首尾最多相差1
		// 注意：排序过后 原来的 heap 就不是一个合法的 heap 了
	template <class RandomIter>
	void sort_heap(RandomIter first, RandomIter last) {
		// 每执行一次 pop_heap，最大的元素都被放到尾部，直到容器最多只有一个元素，完成排序
		while (last - first > 1) {
			saystl::pop_heap(first, last--);
		}
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class RandomIter, class Compared>
	void sort_heap(RandomIter first, RandomIter last, Compared comp) {
		while (last - first > 1) {
			saystl::pop_heap(first, last--, comp);
		}
	}

	// make_heap
	// 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap
	template <class RandomIter, class Distance>
	void make_heap_aux(RandomIter first, RandomIter last, Distance*) {
		if (last - first < 2)
			return;
		auto len = last - first;
		auto holeIndex = (len - 2) / 2;
		while (true) {
			// 重排以 holeIndex 为首的子树
			saystl::adjust_heap(first, holeIndex, len, *(first + holeIndex));
			if (holeIndex == 0)
				return;
			holeIndex--;
		}
	}

	template <class RandomIter>
	void make_heap(RandomIter first, RandomIter last) {
		saystl::make_heap_aux(first, last, distance_type(first));;
	}

	// 重载版本使用函数对象 comp 代替比较操作
	template <class RandomIter, class Distance, class Compared>
	void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared comp) {
		if (last - first < 2)
			return;
		auto len = last - first;
		auto holeIndex = (len - 2) / 2;
		while (true) {
			// 重排以 holeIndex 为首的子树
			saystl::adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
			if (holeIndex == 0)
				return;
			holeIndex--;
		}
	}

	template <class RandomIter, class Compared>
	void make_heap(RandomIter first, RandomIter last, Compared comp) {
		saystl::make_heap_aux(first, last, distance_type(first), comp);
	}

} // !namespace saystl
#endif // !SAYTINYSTL_HEAP_ALGO_H

