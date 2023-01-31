/*
特性提取机 练习测试 跟项目无关
*/
#include<iostream>
struct A {};
struct B:public A{};// 继承的好处就是，当函数需要参数为A,而你传入的参数为B的时候，可以往上一直找到适合的对象

//假设有一个未知的类
template<class U>
struct un_class {
	typedef U value_type;
};
//把该类的类型给萃取出来
template<class un_class>
struct un_class_traits {
	typedef typename un_class::value_type value_type;
};
//获取原生指针类型
template<class U>
struct un_class_traits<U*>
{
	typedef U value_type;
};
//获取原来的常数类型
template<class U>
struct un_class_traits<const U*>
{
	typedef const U value_type;
};
//返回是哪一个类型的函数
template<class un_class>
inline typename un_class_traits<un_class>::value_type	get_type(un_class) {
	typedef typename  un_class_traits<un_class>::value_type RT;
	return RT();
}

template<class un_class>
inline typename un_class_traits<un_class>::value_type _fun(un_class, A) {
	std::cout << "找到类型为 A " << std::endl;
	return A();
}

template<class un_class>
inline typename un_class_traits<un_class>::value_type _fun(un_class, B) {
	std::cout << "找到类型为 B " << std::endl;
	return B();
}
template<class un_class, class T>
inline T _fun(un_class, T) {
	std::cout << "找到类型为 常数指针 " << std::endl;
	return T();
}


//用函数进行区别测试
template<class un_class>
inline typename un_class_traits<un_class>::value_type fun(un_class u) {
	typedef typename un_class_traits<un_class>::value_type RT;
	//往上去找
	return _fun(u,RT());
}


int main_i_test() {
	un_class<B> b;
	un_class<A> a;
	//unknown_class<int> i;
	
	int value = 1;
    int *p = &value;
    A v1 = fun(a);
    B v2 = fun(b);
    int v3 = fun(p);
	return 0;
}