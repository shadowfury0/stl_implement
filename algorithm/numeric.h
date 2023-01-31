#pragma once
#ifndef _MINI_NUMERIC_
#define _MINI_NUMERIC_

#include "dependence.h"

_MINI_BEGIN
//以下是正常 数字类型的普通算法

//累加
template<class InputIterator,class T>
T accumulate(InputIterator first, InputIterator last, T init) {
	for (; first != last; ++first)
		init = init + *first;
	return init;
}
template<class InputIterator, class T,class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init,BinaryOperation binary_op) {
	for (; first != last; ++first)
		init = binary_op(init, *first);
	return init;
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator __adjust_difference(InputIterator first, InputIterator last, OutputIterator result, T*)
{
	T value = *first;
	while (++first != last)
	{
		T tmp = *first;
		*++result = tmp - value;
		value = tmp;
	}
	return ++result;
}
// 计算区间内相邻元素之间的差 
template<class InputIterator,class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
	OutputIterator result) {
	if (first == last) return result;
	*result = *first;
	return __adjust_difference(first, last, result, value_type(first));
}

template<class InputIterator, class OutputIterator, class T,class BinaryOperation>
OutputIterator __adjust_difference(InputIterator first, InputIterator last, 
	OutputIterator result, T*,BinaryOperation binary_op)
{
	T value = *first;
	while (++first != last)
	{
		T tmp = *first;
		*++result = binary_op(tmp,value);
		value = tmp;
	}
	return ++result;
}
template<class InputIterator, class OutputIterator,class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
	OutputIterator result, BinaryOperation binary_op) {
	if (first == last) return result;
	*result = *first;
	return __adjust_difference(first, last, result, value_type(first), binary_op);
}

// 计算两个区间元素的内积 
template <class _InputIterator1, class _InputIterator2, class _Tp>
_Tp inner_product(_InputIterator1 __first1, _InputIterator1 __last1,
	_InputIterator2 __first2, _Tp __init)
{
	for (; __first1 != __last1; ++__first1, ++__first2)
		__init = __init + (*__first1 * *__first2);
	return __init;
}

// 用两个二元函数计算
template <class _InputIterator1, class _InputIterator2, class _Tp,
	class _BinaryOperation1, class _BinaryOperation2>
_Tp inner_product(_InputIterator1 __first1, _InputIterator1 __last1,
	_InputIterator2 __first2, _Tp __init,
	_BinaryOperation1 __binary_op1,
	_BinaryOperation2 __binary_op2)
{
	for (; __first1 != __last1; ++__first1, ++__first2)
		__init = __binary_op1(__init, __binary_op2(*__first1, *__first2));
	return __init;
}


// 计算范围 [first, last) 的子范围中元素的部分和，并写入到始于 result 的范围
template <class _InputIterator, class _OutputIterator, class _Tp>
_OutputIterator
__partial_sum(_InputIterator __first, _InputIterator __last,
	_OutputIterator __result, _Tp*)
{
	_Tp __value = *__first;
	while (++__first != __last) {
		__value = __value + *__first;
		*++__result = __value;
	}
	return ++__result;
}

template <class _InputIterator, class _OutputIterator>
_OutputIterator
partial_sum(_InputIterator __first, _InputIterator __last,
	_OutputIterator __result)
{
	if (__first == __last) return __result;
	*__result = *__first;
	return __partial_sum(__first, __last, __result, value_type(__first));
}

template <class _InputIterator, class _OutputIterator, class _Tp,
	class _BinaryOperation>
_OutputIterator
__partial_sum(_InputIterator __first, _InputIterator __last,
	_OutputIterator __result, _Tp*, _BinaryOperation __binary_op)
{
	_Tp __value = *__first;
	while (++__first != __last) {
		__value = __binary_op(__value, *__first);
		*++__result = __value;
	}
	return ++__result;
}

template <class _InputIterator, class _OutputIterator, class _BinaryOperation>
_OutputIterator
partial_sum(_InputIterator __first, _InputIterator __last,
	_OutputIterator __result, _BinaryOperation __binary_op)
{
	if (__first == __last) return __result;
	*__result = *__first;
	return __partial_sum(__first, __last, __result, value_type(__first),
		__binary_op);
}

// 求幂，power 不属于 STL 标准，属于 SGI 专属
template <class _Tp, class _Integer, class _MonoidOperation>
_Tp __power(_Tp __x, _Integer __n, _MonoidOperation __opr)
{
	if (__n == 0)
		return identity_element(__opr);
	else {
		while ((__n & 1) == 0) {
			__n >>= 1;
			__x = __opr(__x, __x);
		}

		_Tp __result = __x;
		__n >>= 1;
		while (__n != 0) {
			__x = __opr(__x, __x);
			if ((__n & 1) != 0)
				__result = __opr(__result, __x);
			__n >>= 1;
		}
		return __result;
	}
}

template <class _Tp, class _Integer>
inline _Tp __power(_Tp __x, _Integer __n)
{
	return __power(__x, __n, multiplies<_Tp>());
}
template <class _Tp, class _Integer, class _MonoidOperation>
inline _Tp power(_Tp __x, _Integer __n, _MonoidOperation __opr)
{
	return __power(__x, __n, __opr);
}

template <class _Tp, class _Integer>
inline _Tp power(_Tp __x, _Integer __n)
{
	return __power(__x, __n);
}

// iota is not part of the C++ standard.  It is an extension.
// 以始于 value 并重复地求值 ++value 的顺序递增值填充范围 [first, last) 
template <class ForwardIterator, class _Tp>
void
iota(ForwardIterator __first, ForwardIterator __last, _Tp __value)
{
	while (__first != __last)
		*__first++ = __value++;
}


_MINI_END
#endif // !_MINI_NUMERIC_
