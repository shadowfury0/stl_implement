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
//���simple_alloc Ϊ�˷��Ϲ淶������һ���װ���Է����������ռ����õ��㷨һ����
template<class _Tp, class _Alloc>
class simple_alloc {
public:
	// ���� n ��Ԫ��
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
//�ڴ�����Ĵ���ʽ
//������������NULL ������������оͿ��Դ�ӡ�쳣,��ʱ�����쳣�Ĵ���ʽ
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
//ע�⣬����ֱ�ӽ�����instָ��Ϊ0.
typedef __malloc_alloc_template<0> malloc_alloc;
//�ڶ����ڴ���䷽ʽ
enum { __ALIGN = 8 }; // �����С
enum { __MAX_BYTES = 128 };//��������
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };//�ڴ�����
template<bool threads, int inst>
class __default_alloc_template {
private:
	//����ȡ��(8����)
	static size_t ROUND_UP(size_t bytes) 
	{
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}
	union obj {
		union obj* list_link;/*�б�ṹ*/
		char data[1];/*ʵ����������*/
	};
	static obj* volatile free_list[__NFREELISTS];
	//ʹ�õ�n��
	static size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}
	//����һ����СΪn�Ķ����������ܵļ����СΪn���������鵽list
	//û�п��õ�free list �ռ���ô˷���
	static void* refill(size_t n);
	static char* chunk_alloc(size_t size, int& nobjs);

	static char* start_free;
	static char* end_free;
	static size_t heap_size;
public:
	//nһ��Ҫ����0
	static void* allocate(size_t n) {
		obj* volatile* my_free_list;
		obj* result;
		//����128b�ͽе�һ���ռ�������
		if (n > (size_t)__MAX_BYTES) {
			return (malloc_alloc::allocate(n));
		}
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (0 == result) {
			//û�ҵ����õ�free list
			void* r = refill(ROUND_UP(n));
			return r;
		}
		*my_free_list = result->list_link;
		return (result);
	}
	//pָ�벻��Ϊ0
	static void deallocate(void* p, size_t n) {
		obj* q = (obj*)p;
		obj* volatile* my_free_list;
		//����128b
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
//�������ֵ���Ե�
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

/*
Ĭ������£�C++ ��ٶ� __default_alloc_template<threads, inst>::obj ����ģ�壻��ˣ�
����� < ����ΪС�ںš� ������ʽ��֪������ obj  ��ģ�壬
�Ա�����ȷ���������š� ��Ҫ�����˴��������������͵�������ʹ�� template �ؼ��֣�������ʾ��
__default_alloc_template<threads, inst>::template obj
������Ҫע�������д�Ĳ�һ���������Ǳ�����ʶ�����⣡��������������
*/
template<bool threads, int inst>
__default_alloc_template<threads, inst>::template obj * volatile
__default_alloc_template<threads, inst>::template free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };

template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n)
{
	//����Ĭ��������20��Ĵ�С
	int nobjs = 20;
	char* chunk = chunk_alloc(n, nobjs);
	obj* volatile* my_free_list;
	obj* result;
	obj* current_obj, * next_obj;
	int i;
	if (1 == nobjs) return(chunk);
	//�������free list ,�����µĽڵ㡣
	my_free_list = free_list + FREELIST_INDEX(n);
	result = (obj*)chunk;//��һ�鴫���������ά����
	*my_free_list = next_obj = (obj*)(chunk + n);
	//��free list�ĸ����ڵ㴮������
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
* �ڴ�Ļ���أ�����������ʱ���ͷŵ����ڴ档
* ���Ч�ʶ���
*/
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs)
{
	char* result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;
	if (bytes_left >= total_bytes) {
		//�ڴ���ȫ����ʣ��ռ�������
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else if (bytes_left >= size) {
		//�ڴ治����ȫ���������������㹻��Ӧ(��)һ�����ϵ�����
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else {
		//�ڴ��ʣ��ռ���һ���С���޷��ṩ�� heapsize��0����
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		if (bytes_left > 0) {
			//�����ڴ��ʣ��Ĵ�С���ܲ������á�
			//��Ѱ��free list��
			obj* volatile* my_free_list = free_list + FREELIST_INDEX(bytes_left);
			//����free list ����ʣ��ռ�
			((obj*)start_free)->list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		//���öѿռ�
		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free) {
			//�ѿռ䲻�㣬����ʧ��
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
			end_free = 0;//�����û�ڴ���
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return (chunk_alloc(size, nobjs));
	}
	return nullptr;
}
	//sgiԴ����Ҳ��Ĭ���õڶ����ռ�����������ȻҲ�������������һ���ռ�������
#ifdef __USE_MINI_MALLOC_
	typedef malloc_alloc alloc;//��allocΪ��һ���ռ�������
#else
	typedef __default_alloc_template<false, 0> alloc;
#endif // __USE_MINI_MALLOC_
_MINI_END


#endif // !_MINI_ALLOC_