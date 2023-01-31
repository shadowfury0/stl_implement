#pragma once
#ifndef _MINI_A_CONSTRUCT_

#include <new>
#include "../iterator/iterator.h"
/*
该文件是构造器的内存分配
*/
namespace mini {
    template<class T1,class T2>
    inline void construct(T1* p, const T2& value) {
        new (p)T1(value);
    }
    template<class T1>
    inline void construct(T1* p) {
        new (p)T1();
    }
    template<class T>
    inline void destroy(T* pointer) {
        pointer->~T();
    }
    //这里直接不管有没有析构函数都直接调用了，要判断还需要一大堆东西太过麻烦就省略掉了
    template<class ForwardIterator>
    inline void _destroy_aux(ForwardIterator first, ForwardIterator last) {
        for (; first < last; first++)
            destroy(&*first);
    }
    template<class ForwardIterator, class T>
    inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
        _destroy_aux(first, last);
    }
    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last) {
        _destroy(first, last, mini::value_type(first));
    }
    
    //以下是其余版本
    inline void destroy(char* ,char*){}
    inline void destroy(wchar_t* ,wchar_t*){}
}

#endif // !_MINI_A_CONSTRUCT_
