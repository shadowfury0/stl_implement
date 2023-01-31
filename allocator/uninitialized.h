#pragma once
#include <memory>
//����ļ��ò���

/*
��SGIԴ�������º��������þ��� ��ͨ�� �������������ж�
����˵�������жϣ����������жϣ��Ƿ�ΪPOD���������жϵȡ�
 ���ļ�(type_traits.h)�� ͨ�����������жϵõ� 
__true_type�ṹ��
__false_type�ṹ��
�õ��Ĳ���Ȼ����Ե����������ͽ����ڴ�� �����ͷ� ����ؿռ��������Ĳ�����

��Ȼ�û�Ҳ�����Զ�������type_traits������

template struct __type_traits<�Զ���>{
   typedef __true_type    has_trivial_default_constructor;
   typedef __false_type    has_trivial_copy_constructor;
   typedef __false_type    has_trivial_assignment_operator;
   typedef __false_type    has_trivial_destructor;
   typedef __false_type    is_POD_type;
}
��������ͨ�������Զ������������ ͨ����ȡ�������������ݣ��������ڴ�ռ�
*/
struct __true_type{};
struct __false_type{};
//��ʱ����������
template <class ForwardIterator,class Size,class T>
inline ForwardIterator unintialized_fill_n(ForwardIterator first, Size n, const T& x) {
	//value_type����
	return __uninitialized_fill_n(first, n, x, value_type(first));
}
template <class ForwardIterator,class Size,class T,class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
	//ҲҪ��ʱ����
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}
template <class ForwardIterator, class Size, class T>
//����
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n,const T& x,__true_type) {
	return fill_n(first, n, x);
}
template <class ForwardIterator, class Size, class T>
//����
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
	for (; n > 0; --n; ++cur)
		//2.3��
		construct();
	return cur;
}
template <class InputIterator, class ForwardIterator>
//����
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,ForwardIterator result) {
	return __uninitialized_copy(first,last,result,value_type(result));
}
template <class InputIterator, class ForwardIterator,class T>
//����
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result,T*) {
	typedef typename __type_trait<T>::is_POD_type is_POD;
	return __unintialized_copy_aux(first, last, result, is_POD());
}
template <class InputIterator, class ForwardIterator>
//����
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result,__true_type) {
	return copy(first, last, result);
}
template <class InputIterator, class ForwardIterator>
//����
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
	ForwardIterator cur = first;
	for (; first != last; ++first, ++cur) {
		construct(&*cur, *first);
	}
	return cur;
}
//��Գ����ַ���������
inline char* uninitialized_copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}
template <class ForwardIterator,class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T&) {
	//����
	__uninitialized_fill(first, last, x, value_type(first));
}
template <class ForwardIterator, class T,class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x,T1*) {
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	//����
	__uninitialized_fill(first, last, x, is_POD());
}
template <class ForwardIterator, class T>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x,__true_type ) {
	//ϵͳ����
	fill(first, last, x);
	//����
}
template <class ForwardIterator, class T>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
	//ϵͳ����
	ForwardIterator cur = first;
	for (; cur != last; ++cur)
		construct(&*cur, x);//����һ�����Ĺ����޷���������
	//����
}