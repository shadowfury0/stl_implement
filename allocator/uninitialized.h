#pragma once
#include <memory>
//这个文件用不着

/*
在SGI源码中以下函数的作用就是 ，通过 迭代器的类型判断
比如说构造器判断，析构函数判断，是否为POD类型数据判断等。
 在文件(type_traits.h)中 通过这种类型判断得到 
__true_type结构体
__false_type结构体
得到的参数然后针对迭代器的类型进行内存的 申请释放 等相关空间适配器的操作。

当然用户也可以自定义这种type_traits的类型

template struct __type_traits<自定义>{
   typedef __true_type    has_trivial_default_constructor;
   typedef __false_type    has_trivial_copy_constructor;
   typedef __false_type    has_trivial_assignment_operator;
   typedef __false_type    has_trivial_destructor;
   typedef __false_type    is_POD_type;
}
编译器会通过上面自定义的数据类型 通过萃取器（迭代器内容）来分配内存空间
*/
struct __true_type{};
struct __false_type{};
//暂时搁置在这里
template <class ForwardIterator,class Size,class T>
inline ForwardIterator unintialized_fill_n(ForwardIterator first, Size n, const T& x) {
	//value_type暂无
	return __uninitialized_fill_n(first, n, x, value_type(first));
}
template <class ForwardIterator,class Size,class T,class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
	//也要暂时搁置
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}
template <class ForwardIterator, class Size, class T>
//搁置
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n,const T& x,__true_type) {
	return fill_n(first, n, x);
}
template <class ForwardIterator, class Size, class T>
//搁置
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
	for (; n > 0; --n; ++cur)
		//2.3节
		construct();
	return cur;
}
template <class InputIterator, class ForwardIterator>
//搁置
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,ForwardIterator result) {
	return __uninitialized_copy(first,last,result,value_type(result));
}
template <class InputIterator, class ForwardIterator,class T>
//搁置
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result,T*) {
	typedef typename __type_trait<T>::is_POD_type is_POD;
	return __unintialized_copy_aux(first, last, result, is_POD());
}
template <class InputIterator, class ForwardIterator>
//搁置
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result,__true_type) {
	return copy(first, last, result);
}
template <class InputIterator, class ForwardIterator>
//搁置
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
	ForwardIterator cur = first;
	for (; first != last; ++first, ++cur) {
		construct(&*cur, *first);
	}
	return cur;
}
//针对常量字符串的做法
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
	//搁置
	__uninitialized_fill(first, last, x, value_type(first));
}
template <class ForwardIterator, class T,class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x,T1*) {
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	//搁置
	__uninitialized_fill(first, last, x, is_POD());
}
template <class ForwardIterator, class T>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x,__true_type ) {
	//系统调用
	fill(first, last, x);
	//搁置
}
template <class ForwardIterator, class T>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, __false_type) {
	//系统调用
	ForwardIterator cur = first;
	for (; cur != last; ++cur)
		construct(&*cur, x);//必须一个个的构建无法批量进行
	//搁置
}