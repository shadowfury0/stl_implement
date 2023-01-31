#pragma once
#ifndef _MINI_HEAP_
#define _MINI_HEAP_



#include "dependence.h"
#include "../../functor/function.h"

//trueÉýÐò false ½µÐò£¬Ä¬ÈÏÉýÐò
template<class T>
struct _Com {
	_Com() {
		flag = true;
	}
	_Com(bool f) {
		flag = f;
	}
	~_Com(){}
	bool operator()(const T& x, const T& y) {
		return flag ? x < y : x > y;
	}
private:
	bool flag;
};

_MINI_BEGIN
template <class RandomAccessIterator,class Distance,class T,class Com>
void __push_heap(RandomAccessIterator first, Distance holeIndex,
	Distance topIndex, T value,Com com) {
	Distance parent = (holeIndex - 1) / 2;
	while ( holeIndex > topIndex && com(*(first + parent), value))
	{
		*(first + holeIndex) = *(first + parent);
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;
}
template <class RandomAccessIterator, class Distance, class T, class Com>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last,
	Distance*, T*,Com com) {
	__push_heap(first, Distance((last - first) - 1), Distance(0),T(*(last-1)),com);
}
template <class RandomAccessIterator,class Com>
inline void push_heap(RandomAccessIterator first,
	RandomAccessIterator last,Com com) {//
	__push_heap_aux(first, last, distance_type(first), value_type(first),com);
}
template <class RandomAccessIterator,class Distance,class T, class Com>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex,
	Distance len, T value, Com com) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;
	while (secondChild < len) {
		if ( com(*(first + secondChild) , *(first + (secondChild - 1))) ) {
			secondChild--;
		}
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		secondChild = 2 * (secondChild + 1);
	}
	if (secondChild == len) {
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}
	__push_heap(first, holeIndex, topIndex, value,com);
}
template <class RandomAccessIterator, class T, class Distance,class Com>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last,
	RandomAccessIterator result, T value, Distance*, Com com) {
	*result = *first;
	__adjust_heap(first, Distance(0), Distance(last - first), value , com);
}
template <class RandomAccessIterator, class T, class Com>
inline void __pop_heap_aux(RandomAccessIterator first, 
	RandomAccessIterator last, T*, Com com) {
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first),com);
}
template <class RandomAccessIterator,class Com>
inline void pop_heap(RandomAccessIterator first,
	RandomAccessIterator last, Com com ) {//
	__pop_heap_aux(first, last, value_type(first),com);
}
template <class RandomAccessIterator, class Com>
inline void sort_heap(RandomAccessIterator first,
	RandomAccessIterator last,Com com ){//
	while (last - first > 1) {
		pop_heap(first, last--,com);
	}
}
template<class RandomAccessIterator,class T,class Distance, class Com>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*, Com com) {
	if (last - first < 2) return;
	Distance len = last - first;
	Distance parent = (len - 2) / 2;
	while (true)
	{
		__adjust_heap(first, parent, len, T(*(first + parent)),com);
		if (0 == parent) return;
		parent--;
	}
}
template <class RandomAccessIterator, class Com>
inline void make_heap(RandomAccessIterator first,
	RandomAccessIterator last, Com com ) {//
	__make_heap(first, last, value_type(first), distance_type(first),com);
}
_MINI_END
#endif // !_MINI_HEAP_
