#pragma once
#ifndef _MINI_ALGO_
#define _MINI_ALGO_

#include "dependence.h"
#include "algobase.h"
#include "../container/sequence container/heap.h"//包含堆算法

_MINI_BEGIN
/*这个算法类本来是要进行大量的类型检查的，为了方便起见，以及类型检查不属于书籍内容
* 就简化这一部分的内容
*/


//for_each 对迭代器的元素 传入函数进行操作
template <class InputIter, class Function>
Function for_each(InputIter __first, InputIter __last, Function __f) {
	for (; __first != __last; ++__first)
		__f(*__first);
	return __f;
}

//查找相邻元素,未找到就返回最后一个元素
template<class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first,ForwardIterator last) {
	if (first == last) return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next) return first;
		//未找到相邻传入上一个元素
		first = next;
	}
	return last;
}

template<class ForwardIterator,class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,BinaryPredicate b) {
	if (first == last) return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (b(*first , *next)) return first;
		//未找到相邻传入上一个元素
		first = next;
	}
	return last;
}

//记数
template<class InputIterator,class T>
typename iterator_trait<InputIterator>::difference_type
count(InputIterator first,InputIterator last,const T& value) {
	typename iterator_trait<InputIterator>::difference_type n = 0;
	for (; first != last; ++first)
		if (*first == value)
			++n;
	return n;
}
template <class _InputIter, class Predicate>
typename iterator_trait<_InputIter>::difference_type
count_if(_InputIter __first, _InputIter __last, Predicate __pred) {
	typename iterator_trait<_InputIter>::difference_type n = 0;
	for (; __first != __last; ++__first)
		if (__pred(*__first))
			++n;
	return n;
}

//查找元素,找出第一个匹配的
template<class InputIterator,class T>
InputIterator find( InputIterator first,InputIterator last,const T& value) {
	while (first != last && *first != value) ++first;
	return first;
}
//查找条件
template<class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
	while (first != last && !pred(*first)) ++first;
	return first;
}

template<class ForwardIterator1, class ForwardIterator2,class Distance1,class Distance2>
ForwardIterator1 __search(ForwardIterator1 first1,ForwardIterator1 last1,
	ForwardIterator2 first2,ForwardIterator2 last2,Distance1*,Distance2*) {
	Distance1 d1 = mini::distance(first1, last1);
	Distance2 d2 = mini::distance(first2, last2);
	if (d1 < d2)return last1;//如果小于直接跳出
	ForwardIterator1 cur1 = first1;
	ForwardIterator1 cur2 = first2;

	while (cur2 != last2) {
		if (*cur1 == *cur2) {
			++cur1;
			++cur2;
		}
		else {
			if (d1 == d2)
				return last1;
			else {
				cur1 = ++first1;
				cur2 = first2;
				--d1;
			}
		}
	}
	return first1;
}

//查找 序列1[first1,last1)覆盖的区间中,查找[first2,last2)序列2在1中出现的位置
template<class ForwardIterator1,class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,
	ForwardIterator2 first2,ForwardIterator2 last2) {
	return __search(first1, last1, first2, last2, distance_type(first1), distance_type(first2));
}
//第二个版本直接抄源码了
template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1,
	_ForwardIter2 __first2, _ForwardIter2 __last2,
	_BinaryPred  __predicate)
{
	// Test for empty ranges
	if (__first1 == __last1 || __first2 == __last2)
		return __first1;

	// Test for a pattern of length 1.
	_ForwardIter2 __tmp(__first2);
	++__tmp;
	if (__tmp == __last2) {
		while (__first1 != __last1 && !__predicate(*__first1, *__first2))
			++__first1;
		return __first1;
	}
	// General case.
	_ForwardIter2 __p1, __p;
	__p1 = __first2; ++__p1;

	_ForwardIter1 __current = __first1;

	while (__first1 != __last1) {
		while (__first1 != __last1) {
			if (__predicate(*__first1, *__first2))
				break;
			++__first1;
		}
		while (__first1 != __last1 && !__predicate(*__first1, *__first2))
			++__first1;
		if (__first1 == __last1)
			return __last1;

		__p = __p1;
		__current = __first1;
		if (++__current == __last1) return __last1;

		while (__predicate(*__current, *__p)) {
			if (++__p == __last2)
				return __first1;
			if (++__current == __last1)
				return __last1;
		}

		++__first1;
	}
	return __first1;
}

template<class ForwardIterator,class Integer,class T>
ForwardIterator search_n(ForwardIterator first,ForwardIterator last
	,Integer count,const T& value) {
	if (count <= 0) return first;
	else {
		first = mini::find(first, last, value);
		while (first != last)
		{
			Integer n = count - 1;
			ForwardIterator i = first;
			++i;
			while (i != last && n != 0 && *i == value)
			{
				++i; 
				--n;
			}
			if (n == 0) return first;
			else first = mini::find(i, last, value);
		}
		return last;
	}
}

template <class _ForwardIter, class _Integer, class _Tp, class _BinaryPred>
_ForwardIter search_n(_ForwardIter __first, _ForwardIter __last,
	_Integer __count, const _Tp& __val,_BinaryPred __binary_pred) {
	if (__count <= 0)
		return __first;
	else {
		while (__first != __last) {
			if (__binary_pred(*__first, __val))
				break;
			++__first;
		}
		while (__first != __last) {
			_Integer __n = __count - 1;
			_ForwardIter __i = __first;
			++__i;
			while (__i != __last && __n != 0 && __binary_pred(*__i, __val)) {
				++__i;
				--__n;
			}
			if (__n == 0)
				return __first;
			else {
				while (__i != __last) {
					if (__binary_pred(*__i, __val))
						break;
					++__i;
				}
				__first = __i;
			}
		}
		return __last;
	}
}

//交换两个容器
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1,_ForwardIter2 __first2) {
	for (; __first1 != __last1; ++__first1, ++__first2)
		mini::iter_swap(__first1, __first2);
	return __first2;
}

// transform
template <class _InputIter, class _OutputIter, class _UnaryOperation>
_OutputIter transform(_InputIter __first, _InputIter __last,_OutputIter __result, _UnaryOperation __opr) {
	for (; __first != __last; ++__first, ++__result)
		*__result = __opr(*__first);
	return __result;
}

template <class _InputIter1, class _InputIter2, class _OutputIter,class _BinaryOperation>
_OutputIter transform(_InputIter1 __first1, _InputIter1 __last1,_InputIter2 __first2, _OutputIter __result,_BinaryOperation __binary_op) {
	for (; __first1 != __last1; ++__first1, ++__first2, ++__result)
		*__result = __binary_op(*__first1, *__first2);
	return __result;
}

//替换值
template<class ForwardIterator,class T>
void replace(ForwardIterator first, ForwardIterator last,
		const T& old_value,const T& new_value) {
	for (; first != last; ++first) {
		if (*first == old_value) *first = new_value;
	}
}
template<class InputIterator,class OutputIterator,class T>
OutputIterator replace_copy(InputIterator first,InputIterator last,
			OutputIterator result,const T& old_value,const T& new_value) {
	for (; first != last; ++first, ++result) {
		*result = *first == old_value ? new_value : *first;
	}
	return result;
}
template<class ForwardIterator,class Predicate,class T>
void replace_if(ForwardIterator first,ForwardIterator last
	,Predicate pred,const T&new_value) {
	for (; first != last; ++first)
		if (pred(*first))*first = new_value;
}

template<class InputIterator, class OutputIterator, class T,class Predicate>
OutputIterator replace_copy_if(InputIterator first, InputIterator last,
	OutputIterator result, Predicate pred, const T& new_value) {
	for (; first != last; ++first, ++result) {
		*result = pred(*first) ? new_value : *first;
	}
	return result;
}

//将仿函数gen的运算结果填写[first,last)区间内的所有元素身上。
template<class ForwardIterator,class Generator>
void generate(ForwardIterator first,ForwardIterator last,Generator gen) {
	for (; first != last; ++first) {
		*first = gen();
	}
}
//将仿函数gen的运算结构从[first开始往后n个元素身上。
template<class OuputIterator,class Size,class Generator>
OuputIterator generate_n(OuputIterator first,Size n,Generator gen) {
	for (; n > 0; --n, ++first) {//限制n个元素
		*first = gen();
	}
	return first;
}

template<class InputIterator,class OutputIterator,class Predicate>
OutputIterator remove_copy_if(InputIterator first,InputIterator last,
			OutputIterator result,Predicate pred) {
	for (; first != last; ++first) {
		if (!pred(*first)) {
			*result = *first;
			++result;
		}
	}
	return result;
}

template<class ForwardIterator,class Predicate>
ForwardIterator remove_if(ForwardIterator first,ForwardIterator last,
		Predicate pred) {
	first = mini::find_if(first, last, pred);
	ForwardIterator next = first;
	return first == last ? first : remove_copy_if(++next, last, first, pred);
}
//移除不删除
template<class InputIterator,class OutputIterator,class T>
OutputIterator remove_copy(InputIterator first,InputIterator last,
		OutputIterator result,const T& value) {
	for (; first != last; ++first) {
		if (*first != value) {
			*result = *first;
			++result;
		}
	}
	return result;
}
//移除不删除
template<class ForwardIterator,class T>
ForwardIterator remove(ForwardIterator first,ForwardIterator last,
	const T& value) {
	first = mini::find(first, last, value);
	ForwardIterator next = first;
	return first == last ? first : mini::remove_copy(++next,last,first,value);
}


template<class InputIterator,class ForwardIterator>
ForwardIterator __unique_copy(InputIterator first,InputIterator last,
		ForwardIterator result,forward_iterator_tag) {
	*result = *first;
	while (++first != last)
		if (*result != *first) *result = *first;
	return ++result;
}

template<class InputIterator, class OutputIterator,class T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
	OutputIterator result, T*) {
	T value = *first;
	*result = value;
	while (++first != last)
	{
		if (value != *first) {
			value = *first;
			*result = value;
		}
	}
	return ++result;
}

template<class InputIterator, class OutputIterator>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
	OutputIterator result, output_iterator_tag) {
	return __unique_copy(first, last, result, value_type(first));
}
template<class InputIterator,class OutputIterator>
inline OutputIterator unique_copy(InputIterator first,InputIterator last,
		OutputIterator result) {
	if (first == last) return result;
	return __unique_copy(first, last, result, iterator_category(result));
}

//让迭代器元素唯一
template<class ForwardIterator>
ForwardIterator unique(ForwardIterator first,ForwardIterator last) {
	first = mini::adjacent_find(first, last);
	return unique_copy(first, last, first);
}

template<class BidirectionalIterator>
void __reverse(BidirectionalIterator first,BidirectionalIterator last,bidirectional_iterator_tag) {
	while (true)
	{
		if (first == last || first == --last)return;
		else mini::iter_swap(first++, last);
	}
}
template<class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last,random_access_iterator_tag) {
	while (first < last) mini::iter_swap(first++, --last);
}
template<class BidirectionalIterator>
inline void reverse(BidirectionalIterator first,BidirectionalIterator last) {
	__reverse(first, last, iterator_category(first));
}

//原序列未改变
template<class BidirectionalIterator,class OutpurIterator>
OutpurIterator reverse_copy(BidirectionalIterator first,BidirectionalIterator last,
	OutpurIterator result) {
	while ( first != last)
	{
		--last;
		*result = *last;
		++result;
	}
	return result;
}

//二分查找，前提是排好序
template<class ForwardIterator, class T,class Distance>
inline ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
	const T& val, Distance*) {
	Distance len = distance(first, last);//距离计算
	Distance half;
	ForwardIterator middle;
	while (len > 0)
	{
		//除2
		half = len >> 1;
		//中间位置
		middle = first;
		advance(middle, half);
		if (*middle < val) {
			//如果小于该值继续查找
			first = middle;
			++first;
			len = len - half - 1;
		}
		else {
			len = half;
		}
	}
	return first;
}
//找到不小于val的值的位置
template<class ForwardIterator,class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
	const T& val) {
	return __lower_bound(first, last, val, distance_type(first));
}

//该函数判断两个序列(递增或者递减，但必须同时递增或递减) 判断序列2是否包含在序列1里面
//是否包含
template<class InputIterator1,class InputIterator2>
bool includes(InputIterator1 first1,InputIterator1 last1,
			InputIterator2 first2,InputIterator2 last2) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1)
			return false;
		else if (*first1 < *first2)
			++first1;
		else
			++first1, ++first2;
	}
	return first2 == last2;
}
//需要注意comp类型既不是binarypredicate也不是binaryoperation
template<class InputIterator1, class InputIterator2,class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2,Compare com) {
	while (first1 != last1 && first2 != last2) {
		if (com(*first2 , *first1))
			return false;
		else if (comp(*first1 , *first2))
			++first1;
		else
			++first1, ++first2;
	}
	return first2 == last2;
}

template <class ForwardIterator, class Predicate>
ForwardIterator __partition(ForwardIterator __first,
	ForwardIterator __last,Predicate   __pred,forward_iterator_tag) {
	if (__first == __last) return __first;
	while (__pred(*__first))
		if (++__first == __last) return __first;
	ForwardIterator __next = __first;
	while (++__next != __last)
		if (__pred(*__next)) {
			mini::swap(*__first, *__next);
			++__first;
		}
	return __first;
}

template<class BidirectionalIterator,class Predicate>
BidirectionalIterator __partition(BidirectionalIterator first,
	BidirectionalIterator last,Predicate pred,bidirectional_iterator_tag) {
	while (true) {
		while (true)
		{
			if (first == last) return first;//退出
			else if (pred(*first)) ++first;
			else break;
		}
		--last;
		while (true)
		{
			if (first == last) return first;
			else if (!pred(*last)) --last;
			else break;
		}
		mini::iter_swap(first, last);//交换指针
		++first;
	}
}
//进行分区(不稳定分区)
template<class ForwardIter,class Predicate>
inline ForwardIter partition(ForwardIter first,ForwardIter last,Predicate pred) {
	return __partition(first, last, pred, iterator_category(first));
}

//将两个有序的序列合并
template<class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator merge(InputIterator1 first1,InputIterator1 last1,
		InputIterator2 first2,InputIterator2 last2,OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return mini::copy(first2, last2, mini::copy(first1,last1,result));
}

template<class InputIterator1, class InputIterator2, class OutputIterator,class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result,Compare com) {
	while (first1 != last1 && first2 != last2) {
		if (com(*first2 , *first1)) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return mini::copy(first2, last2, mini::copy(first1, last1, result));
}

//set数据结构算法

//求set并集
template<class InputIterator1,class InputIterator2,class OuputIterator>
OuputIterator set_union(InputIterator1 first1,InputIterator1 last1,
	InputIterator2 first2,InputIterator2 last2,OuputIterator result) {

	while ( first1 != last1 && first2 != last2 )
	{
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	
	return mini::copy(first2, last2, mini::copy(first1,last1,result) );
}

template<class InputIterator1, class InputIterator2, class OuputIterator,class Comp>
OuputIterator set_union(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OuputIterator result,Comp com) {

	while (first1 != last1 && first2 != last2)
	{
		if ( com(*first1 , *first2) ) {
			*result = *first1;
			++first1;
		}
		else if ( com(*first2 , *first1) ) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}

	return mini::copy(first2, last2, mini::copy(first1, last1, result));
}
//交集
template<class InputIterator1,class InputIterator2,class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1,InputIterator1 last1,
	InputIterator2 first2,InputIterator2 last2,OutputIterator result) {
	while ( first1 != last1 && first2 != last2)
	{
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator,class Com>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result,Com com) {
	while (first1 != last1 && first2 != last2)
	{
		if (com(*first1 , *first2))
			++first1;
		else if (com(*first2 , *first1))
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}

//差集
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while ( first1 != last1 && first2 != last2 ) 
	{
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			++first2;
		}
		else {
			++first1;
			++first2;
		}
	}
	return mini::copy(first1, last1, result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator,class Com>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result,Com com) {
	while (first1 != last1 && first2 != last2)
	{
		if (com(*first1 , *first2)) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (com(*first2 , *first1)) {
			++first2;
		}
		else {
			++first1;
			++first2;
		}
	}
	return mini::copy(first1, last1, result);
}

//对称差集 (S1-S2)∪(S2-S1)
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmertric_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2 )
	{
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
			++result;
		}
		else {
			++first1;
			++first2;
		}
	}
	return mini::copy(first2, last2, mini::copy(first1,last1,result));
}

template<class InputIterator1, class InputIterator2, class OutputIterator,class Com>
OutputIterator set_symmertric_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result,Com com) {
	while (first1 != last1 && first2 != last2)
	{
		if (com(*first1 , *first2)) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (com(*first2 , *first1)) {
			*result = *first2;
			++first2;
			++result;
		}
		else {
			++first1;
			++first2;
		}
	}
	return mini::copy(first2, last2, mini::copy(first1, last1, result));
}

//最大元素返回
template<class ForwardIterator>
ForwardIterator max_element(ForwardIterator first,ForwardIterator last) {
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last) {
		if (*result < *first) result = first;
	}
	return result;
}
template<class ForwardIterator,class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last,Compare com) {
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last) {
		if (com(*result , *first)) result = first;
	}
	return result;
}
//最小元素返回
template<class ForwardIterator>
ForwardIterator min_element(ForwardIterator first,ForwardIterator last) {
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*first < *result) result = first;
	return result;
}
template<class ForwardIterator,class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last,Compare comp) {
	if (first == last) return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*first , *result)) result = first;
	return result;
}


//判断是否为堆
template <class _RandomAccessIter, class _Distance>
bool __is_heap(_RandomAccessIter __first, _Distance __n)
{
	_Distance __parent = 0;
	for (_Distance __child = 1; __child < __n; ++__child) {
		if (__first[__parent] < __first[__child])
			return false;
		if ((__child & 1) == 0)
			++__parent;
	}
	return true;
}

template <class _RandomAccessIter, class _Distance, class _StrictWeakOrdering>
bool __is_heap(_RandomAccessIter __first, _StrictWeakOrdering __comp,
	_Distance __n)
{
	_Distance __parent = 0;
	for (_Distance __child = 1; __child < __n; ++__child) {
		if (__comp(__first[__parent], __first[__child]))
			return false;
		if ((__child & 1) == 0)
			++__parent;
	}
	return true;
}

template <class _RandomAccessIter>
inline bool is_heap(_RandomAccessIter __first, _RandomAccessIter __last)
{
	return __is_heap(__first, __last - __first);
}

template <class _RandomAccessIter, class _StrictWeakOrdering>
inline bool is_heap(_RandomAccessIter __first, _RandomAccessIter __last,
	_StrictWeakOrdering __comp)
{
	return __is_heap(__first, __comp, __last - __first);
}

//在迭代器内 找迭代器内的值
template<class InputIterator,class ForwardsIterator>
InputIterator find_first_of(InputIterator first1,InputIterator last1,
	ForwardsIterator first2,ForwardsIterator last2) {
	for (; first1 != last1; ++first1)
		for (ForwardsIterator iter = first2; iter != last2; ++iter)
			if(*first1 == *iter)
				return first1;//找到了
	return last1;
}

template<class InputIterator, class ForwardsIterator,class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
	ForwardsIterator first2, ForwardsIterator last2,BinaryPredicate comp) {
	for (; first1 != last1; ++first1)
		for (ForwardsIterator iter = first2; iter != last2; ++iter)
			if( comp( *first1 , *iter ) )
				return first1;//找到了
	return last1;
}



//查找最后一次出现的位置
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter1 __find_end(_ForwardIter1 __first1, _ForwardIter1 __last1,_ForwardIter2 __first2, _ForwardIter2 __last2,
	forward_iterator_tag, forward_iterator_tag)
{
	if (__first2 == __last2)
		return __last1;
	else {
		_ForwardIter1 __result = __last1;
		while (1) {
			_ForwardIter1 __new_result = mini::search(__first1, __last1, __first2, __last2);
			if (__new_result == __last1)//没找到
				return __result;
			else {
				__result = __new_result;
				__first1 = __new_result;
				++__first1;
			}
		}
	}
}

//反向迭代器
template <class _ForwardIter1, class _ForwardIter2, class _BinaryPredicate>
_ForwardIter1 __find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2,
	forward_iterator_tag, forward_iterator_tag, _BinaryPredicate __comp)
{
	if (__first2 == __last2)
		return __last1;
	else {
		_ForwardIter1 __result = __last1;
		while (1) {
			_ForwardIter1 __new_result = mini::search(__first1, __last1, __first2, __last2, __comp);
			if (__new_result == __last1)
				return __result;
			else {
				__result = __new_result;
				__first1 = __new_result;
				++__first1;
			}
		}
	}
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter1 __find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1, _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
	bidirectional_iterator_tag, bidirectional_iterator_tag)
{
	typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
	typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

	_RevIter1 __rlast1(__first1);
	_RevIter2 __rlast2(__first2);
	_RevIter1 __rresult = mini::search(_RevIter1(__last1), __rlast1,_RevIter2(__last2), __rlast2);

	if (__rresult == __rlast1)
		return __last1;
	else {
		_BidirectionalIter1 __result = __rresult.base();
		mini::advance(__result, - mini::distance(__first2, __last2));
		return __result;
	}
}

template <class _BidirectionalIter1, class _BidirectionalIter2, class _BinaryPredicate>
_BidirectionalIter1 __find_end(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,_BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
	bidirectional_iterator_tag, bidirectional_iterator_tag,
	_BinaryPredicate __comp)
{
	typedef reverse_iterator<_BidirectionalIter1> _RevIter1;
	typedef reverse_iterator<_BidirectionalIter2> _RevIter2;

	_RevIter1 __rlast1(__first1);
	_RevIter2 __rlast2(__first2);
	_RevIter1 __rresult = mini::search(_RevIter1(__last1), __rlast1,
		_RevIter2(__last2), __rlast2,
		__comp);

	if (__rresult == __rlast1)
		return __last1;
	else {
		_BidirectionalIter1 __result = __rresult.base();
		mini::advance(__result, - mini::distance(__first2, __last2));
		return __result;
	}
}

template <class _ForwardIter1, class _ForwardIter2>
inline _ForwardIter1
find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2)
{
	typedef typename iterator_trait<_ForwardIter1>::iterator_category category1;
	typedef typename iterator_trait<_ForwardIter2>::iterator_category category2;
	return __find_end(__first1, __last1, __first2, __last2,category1(),category2());
}

template <class _ForwardIter1, class _ForwardIter2,
	class _BinaryPredicate>
inline _ForwardIter1 find_end(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2,
	_BinaryPredicate __comp)
{
	typedef typename iterator_trait<_ForwardIter1>::iterator_category category1;
	typedef typename iterator_trait<_ForwardIter2>::iterator_category category2;
	return __find_end(__first1, __last1, __first2, __last2,
		category1(), category2(), __comp);
}

_MINI_END


#endif // !_MINI_ALGO_
