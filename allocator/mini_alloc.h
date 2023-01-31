#pragma once
#ifndef _MINI_ALLOC_
#define _MINI_ALLOC_

#define _MINI_BEGIN namespace mini{
#define _MINI_END }

#if 0
#include <new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#include <iostream>
#define __THROW_BAD_ALLOC std::cerr << "out of memory  " << std::endl; exit(1);
#endif

_MINI_BEGIN
//这个simple_alloc 为了符合规范进行了一层包装，以方便调用下面空间配置的算法一，二
template<class _Tp, class _Alloc>
class simple_alloc {
public:
	// 配置 n 个元素
	static _Tp* allocate(size_t __n)
	{
		return 0 == __n ? 0 : (_Tp*)_Alloc::allocate(__n * sizeof(_Tp));
	}
	static _Tp* allocate(void)
	{
		return (_Tp*)_Alloc::allocate(sizeof(_Tp));
	}
	static void deallocate(_Tp* __p, size_t __n)
	{
		if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(_Tp));
	}
	static void deallocate(_Tp* __p)
	{
		_Alloc::deallocate(__p, sizeof(_Tp));
	}
};

template<int inst>
class __malloc_alloc_template {
private:
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*, size_t);
	static void(*__malloc_alloc_oom_handler)();
public:
	static void* allocate(size_t n) {
		void* result = malloc(n);
		if (0 == result) result = oom_malloc(n);
		return result;
	}
	static void deallocate(void* p, size_t) {
		free(p);
	}
	static void* reallocate(void* p, size_t, size_t new_sz) {
		void* result = realloc(p, new_sz);
		if (0 == result) result = oom_realloc(p, new_sz);
		return result;
	}
	static void (*set_malloc_handler(void (*f)()))(){
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};
//内存溢出的处理方式
//这里让他返回NULL 后续处理操作中就可以打印异常,暂时不做异常的处理方式
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;
template<int inst>
inline void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
	void (*my_malloc_handler)();
	void* result;
	for (;;) {
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();
		result = malloc(n);
		if (result) return (result);
	}
}

template<int inst>
inline void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n)
{
	void (*my_malloc_handler)();
	void* result;
	for (;;) {
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();
		result = realloc(p, n);
		if (result) return (result);
	}
	return nullptr;
}
//注意，以下直接将参数inst指定为0.
typedef __malloc_alloc_template<0> malloc_alloc;
//第二种内存分配方式
enum { __ALIGN = 8 }; // 区块大小
enum { __MAX_BYTES = 128 };//区块上限
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };//内存块个数
template<bool threads, int inst>
class __default_alloc_template {
private:
	//向上取整(8进制)
	static size_t ROUND_UP(size_t bytes) 
	{
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}
	union obj {
		union obj* list_link;/*列表结构*/
		char data[1];/*实际数据区域*/
	};
	static obj* volatile free_list[__NFREELISTS];
	//使用第n块
	static size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}
	//传回一个大小为n的东西，并可能的加入大小为n的其他区块到list
	//没有可用的free list 空间调用此方法
	static void* refill(size_t n);
	static char* chunk_alloc(size_t size, int& nobjs);

	static char* start_free;
	static char* end_free;
	static size_t heap_size;
public:
	//n一定要大于0
	static void* allocate(size_t n) {
		obj* volatile* my_free_list;
		obj* result;
		//大于128b就叫第一级空间配置器
		if (n > (size_t)__MAX_BYTES) {
			return (malloc_alloc::allocate(n));
		}
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (0 == result) {
			//没找到可用的free list
			void* r = refill(ROUND_UP(n));
			return r;
		}
		*my_free_list = result->list_link;
		return (result);
	}
	//p指针不能为0
	static void deallocate(void* p, size_t n) {
		obj* q = (obj*)p;
		obj* volatile* my_free_list;
		//大于128b
		if (n > (size_t)__MAX_BYTES) {
			malloc_alloc::deallocate(p, n);
			return;
		}
		my_free_list = free_list + FREELIST_INDEX(n);
		q->list_link = *my_free_list;
		*my_free_list = q;
	}
		
	static void* reallocate(void* p, size_t old_sz, size_t new_sz);
};
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;
//这里的数值可以调
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

/*
默认情况下，C++ 会假定 __default_alloc_template<threads, inst>::obj 不是模板；因此，
后面的 < 解释为小于号。 必须显式告知编译器 obj  是模板，
以便其正确分析尖括号。 若要更正此错误，请在依赖类型的名称上使用 template 关键字，如下所示：
__default_alloc_template<threads, inst>::template obj
这里需要注意跟书上写的不一样，属于是编译器识别问题！！！！！！！！
*/
template<bool threads, int inst>
__default_alloc_template<threads, inst>::template obj * volatile
__default_alloc_template<threads, inst>::template free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };

template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n)
{
	//这里默认设置了20块的大小
	int nobjs = 20;
	char* chunk = chunk_alloc(n, nobjs);
	obj* volatile* my_free_list;
	obj* result;
	obj* current_obj, * next_obj;
	int i;
	if (1 == nobjs) return(chunk);
	//否则调整free list ,纳入新的节点。
	my_free_list = free_list + FREELIST_INDEX(n);
	result = (obj*)chunk;//这一块传给数组进行维护。
	*my_free_list = next_obj = (obj*)(chunk + n);
	//将free list的各个节点串接起来
	for (i = 1; ; i++) {
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);
		if (nobjs - 1 == i) {
			current_obj->list_link = 0;
			break;
		}
		else {
			current_obj->list_link = next_obj;
		}
	}
	return (result);
}
/*
* 内存的缓存池，用来缓存暂时被释放掉的内存。
* 提高效率而已
*/
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs)
{
	char* result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;
	if (bytes_left >= total_bytes) {
		//内存完全满足剩余空间需求量
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else if (bytes_left >= size) {
		//内存不能完全满足需求量，但足够供应(含)一个以上的区块
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else {
		//内存池剩余空间连一块大小都无法提供。 heapsize清0操作
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		if (bytes_left > 0) {
			//尝试内存池剩余的大小还能不能利用。
			//先寻找free list。
			obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
			//调整free list 利用剩余空间
			((obj*)start_free)->list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		//配置堆空间
		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free) {
			//堆空间不足，分配失败
			int i;
			obj* volatile* my_free_list, * p;

			for (i = size; i <= __MAX_BYTES; i += __ALIGN) {
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p) {
					*my_free_list = p->list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					return (chunk_alloc(size, nobjs));
				}
			}
			end_free = 0;//如果都没内存了
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return (chunk_alloc(size, nobjs));
	}
	return nullptr;
}
	//sgi源码中也是默认用第二级空间配置器，当然也可以声明定义第一级空间配置器
#ifdef __USE_MINI_MALLOC_
	typedef malloc_alloc alloc;//令alloc为第一级空间配置器
#else
	typedef __default_alloc_template<false, 0> alloc;
#endif // __USE_MINI_MALLOC_
_MINI_END


#endif // !_MINI_ALLOC_