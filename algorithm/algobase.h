#pragma once
#ifndef _MINI_ALGO_BASE_
#define _MINI_ALOG_BASE_

#include "dependence.h"
#include "../container/associative container/pair.h"
/*
�����ǻ����ڲ��㷨��ʵ��
*/


_MINI_BEGIN

//�Ե���������ָ��Ԫ��
template<class F1,class F2,class T>
inline void __iter_swap(F1 a, F2 b, T*) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}
template<class ForwardIterator1,class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	__iter_swap(a,b,value_type(a));
}

//����������������
template<class T>
inline void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template<class T>
inline const T& min(const T& a, const T& b) {
	return b < a ? b : a;
}
template<class T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;
}
//�º���
template<class T,class Com>
inline const T& min(const T& a, const T& b,Com c) {
	return c(a,b) ? b : a;
}
template<class T, class Com>
inline const T& max(const T& a, const T& b, Com c) {
	return c(a, b) ? b : a;
}

template<class RandomAccessIterator,class OutputIterator,class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first,RandomAccessIterator last,
		OutputIterator result,Distance*) {
	for (Distance n = last - first; n > 0; --n, ++result, ++first) {
		*result = *first;
	}
	return result;
}
//ͨ�ð汾
template<class InputIterator,class OutputIterator>
inline OutputIterator __copy(InputIterator first,InputIterator last,
		OutputIterator result,input_iterator_tag) {
	for (; first != last; ++result, ++first)
		*result = *first;
	return result;
}
//�����������
template<class RandomAccessIterator,class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first,RandomAccessIterator last,
		OutputIterator result,random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));
}
//��ȡ����������
//�����������ʵ��û��д�й��Ƿ��й���������������ȡ�жϡ�(Ĭ���鷳һ����һ����)
template <class InputIterator,class OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}
};
template <class T>
struct __copy_dispatch<T*,T*>
{
	T* operator()(T* first, T* last, T* result) {
		//���ﲻ����ȡ����ͳһ����
		return __copy(first, last, result, iterator_category(first));
	}
};
template <class T>
struct __copy_dispatch<const T*, T*>
{
	T* operator()(const T* first,const T* last, T* result) {
		return __copy(first, last, result, iterator_category(first));
	}
};
//����汾
inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}
//����汾
inline char* copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

template<class InputIterator,class OuputIterator>
inline OuputIterator copy(InputIterator first,InputIterator last,OuputIterator result) {
	return __copy_dispatch<InputIterator,OuputIterator>()(first, last, result);
}
//���򿽱�����ԭ����ͬ��ʱ����

//�ж����������Ƿ����
template<class Input1,class Input2>
inline bool equal(Input1 first1, Input1 last1, Input2 first2, Input2 last2) {
	for (; first1 != last1 && first2!=last2 ; ++first1, first2) {
		if (*first1 != *first2)
			return false;
	}
	return true;
}
//����ָ���º������бȽ�
template<class Input1, class Input2,class BinaryPredicate>
inline bool equal(Input1 first1, Input1 last1, Input2 first2, Input2 last2,BinaryPredicate b) {
	for (; first1 != last1 && first2 != last2; ++first1, first2) {
		if (!b(*first1,first2))
			return false;
	}
	return true;
}

//���ֵ����Ĭ�� [first,last)
template<class ForwardIterator,class T>
void fill(ForwardIterator first,ForwardIterator last,const T& value) {
	for (; first != last; ++first)
		*first = value;
}
template<class OutputIterator,class Size,class T>
OutputIterator fill_n(OutputIterator first,Size n,const T& value) {
	for (; n > 0; --n, ++first) {
		*first = value;
	}
	return first;//���ص�ǰδ���ĵ�ֵ
}

//ƽ�бȽ���������,�ҳ��������в���ͬ��λ��,�����ز���ͬ�ĵ�����
template<class Iterator1,class Iterator2>
pair<Iterator1,Iterator2> mismatch(Iterator1 f1,Iterator1 l1,
	Iterator2 f2) {
	while ( f1 != l1 && * f1 == * f2)
	{
		++f1;
		++f2;
	}
	return pair<Iterator1, Iterator2>(f1, f2);
}
template<class Iterator1, class Iterator2,class BinaryPredicate>
pair<Iterator1, Iterator2> mismatch(Iterator1 f1, Iterator1 l1,
	Iterator2 f2,BinaryPredicate b) {
	while (f1 != l1 && b(*f1,*f2))
	{
		++f1;
		++f2;
	}
	return pair<Iterator1, Iterator2>(f1, f2);
}



// ��"�ֵ����з�ʽ"���������� [first, last) ���бȽ�
template <class _InputIter1, class _InputIter2>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
	_InputIter2 __first2, _InputIter2 __last2) {
	for (; __first1 != __last1 && __first2 != __last2
		; ++__first1, ++__first2) {
		if (*__first1 < *__first2)
			return true;
		if (*__first2 < *__first1)
			return false;
	}
	return __first1 == __last1 && __first2 != __last2;
}

template <class _InputIter1, class _InputIter2, class _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
	_InputIter2 __first2, _InputIter2 __last2,
	_Compare __comp) {
	for (; __first1 != __last1 && __first2 != __last2
		; ++__first1, ++__first2) {
		if (__comp(*__first1, *__first2))
			return true;
		if (__comp(*__first2, *__first1))
			return false;
	}
	return __first1 == __last1 && __first2 != __last2;
}

// �ػ��� lexicographical_compare��ʹ�� memcmp
inline bool
lexicographical_compare(const unsigned char* __first1,
	const unsigned char* __last1,
	const unsigned char* __first2,
	const unsigned char* __last2)
{
	const size_t __len1 = __last1 - __first1;
	const size_t __len2 = __last2 - __first2;
	const int __result = memcmp(__first1, __first2, min(__len1, __len2));
	return __result != 0 ? __result < 0 : __len1 < __len2;
}


_MINI_END


#endif // !_MINI_ALGO_BASE_
