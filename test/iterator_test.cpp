/*
������ȡ�� ��ϰ���� ����Ŀ�޹�
*/
#include<iostream>
struct A {};
struct B:public A{};// �̳еĺô����ǣ���������Ҫ����ΪA,���㴫��Ĳ���ΪB��ʱ�򣬿�������һֱ�ҵ��ʺϵĶ���

//������һ��δ֪����
template<class U>
struct un_class {
	typedef U value_type;
};
//�Ѹ�������͸���ȡ����
template<class un_class>
struct un_class_traits {
	typedef typename un_class::value_type value_type;
};
//��ȡԭ��ָ������
template<class U>
struct un_class_traits<U*>
{
	typedef U value_type;
};
//��ȡԭ���ĳ�������
template<class U>
struct un_class_traits<const U*>
{
	typedef const U value_type;
};
//��������һ�����͵ĺ���
template<class un_class>
inline typename un_class_traits<un_class>::value_type	get_type(un_class) {
	typedef typename  un_class_traits<un_class>::value_type RT;
	return RT();
}

template<class un_class>
inline typename un_class_traits<un_class>::value_type _fun(un_class, A) {
	std::cout << "�ҵ�����Ϊ A " << std::endl;
	return A();
}

template<class un_class>
inline typename un_class_traits<un_class>::value_type _fun(un_class, B) {
	std::cout << "�ҵ�����Ϊ B " << std::endl;
	return B();
}
template<class un_class, class T>
inline T _fun(un_class, T) {
	std::cout << "�ҵ�����Ϊ ����ָ�� " << std::endl;
	return T();
}


//�ú��������������
template<class un_class>
inline typename un_class_traits<un_class>::value_type fun(un_class u) {
	typedef typename un_class_traits<un_class>::value_type RT;
	//����ȥ��
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