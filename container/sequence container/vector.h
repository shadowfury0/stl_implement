#pragma once
#ifndef _MINI_VEC_
#define _MINI_VEC_


#include "dependence.h"

_MINI_BEGIN
template<class T, class Alloc = mini::alloc>
class vector {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef const iterator const_iterator;
	typedef value_type& reference;
	typedef const reference const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

protected:
	//�ռ�������
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;//��ʾĿǰʹ�ÿռ��ͷ
	iterator finish;//��ʾĿǰʹ�ÿռ��β
	iterator end_of_storage;//��ʾĿǰ���ÿռ��β
	void deallocate() {
		if (start) {
			data_allocator::deallocate(start, end_of_storage - start );
		}
	}
	//�����и�ֵ����ռ�
	void fill_initialize(size_type n) {
		start = allocate_and_fill(n);
		finish = start;
		end_of_storage = finish+n;
	}
	//��ֵ����ռ�
	void fill_initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
public:
	iterator begin()  { return start; }
	const_iterator begin() const { return start; }
	iterator end()   { return finish; }
	const_iterator end()  const { return finish; }
	size_type size() const { return size_type(end() - begin()); }
	size_type capacity() const {
		return size_type(end_of_storage - begin());
	}
	bool empty() const { return begin() == end(); }

	reference operator[](size_type n) { return *(begin() + n); }
	const_reference operator[](size_type n) const { return *(begin() + n); }
	reference operator=(const vector& x){
		fill_initialize(x.capacity());
		iterator iter = x.begin();
		for (; iter != x.end(); iter++) {
			construct(&*finish, *iter);
			++finish;
		}
		return *this;
	}

	vector() : start(0),finish(0),end_of_storage(0){}
	vector(size_type n, const T& value) { fill_initialize(n, value); }
	vector(int n, const T& value) { fill_initialize(n, value); }
	//�б��ʼ��
	vector(std::initializer_list<T> list) {
		size_type n = list.size();
		finish = start = allocate_and_fill(n);
		auto iter = list.begin();
		for (;iter != list.end(); ++iter) {
			construct(&*finish,*iter);
			++finish;
		}
		end_of_storage = start + n;
	}
	explicit vector(size_type n) { fill_initialize(n, T()); }
	//�������캯��
	vector(const vector& x) {
		fill_initialize(x.capacity());
		iterator iter = x.begin();
		for (; iter != x.end(); iter++) {
			construct(&*finish, *iter);
			++finish;
		}
	}
	//ָ��λ�õĿ������캯��
	template <class InputIterator>
	vector(InputIterator first, InputIterator last) {
		assign(first, last);
	}
	~vector() {
		mini::destroy(start,finish);
		deallocate();
	}
	//����ʼ���캯��һ��
	template<class InputIterator>
	void assign(InputIterator first, InputIterator last) {
		typedef typename Is_Integer<InputIterator>::value type;
		assign_dispatch(first,last,type());
	};
	void assign(size_type n, const T& val) {
		if (!empty()) {
			clear();
		}
		fill_initialize(n, val);
	};

	//��������
	void swap(vector<T, Alloc>& x) {
		std::swap(start, x.start);
		std::swap(finish, x.finish);
		std::swap(end_of_storage, x.end_of_storage);
	}
	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back()  { return *(end() - 1); }
	const_reference back() const { return *(end() - 1); }
	reference at(size_type n) {
		if (n > size() - 1) {
			std::cout << " out of range " << std::endl;
		}
		return *(begin() + n);
	};
	const_reference at(size_type n) const { 
		if (n>size()-1) {
			std::cout << " out of range " << std::endl;
		}
		return *(begin() + n);
	}
	void push_back(const T& x) {
		if (finish != end_of_storage) {
			construct(finish, x);
			++finish;
		}
		else {
			//�ռ䲻��
			insert_aux(end(), x);
		}
	}
	//���뷽��
	void insert(iterator position, size_type n, const T& x);
	void insert(iterator position, const T& x) {
		//������
		if (position > finish || position < start ) {
			std::cout << "out of range " << std::endl;
			return;
		}
		insert(position, 1, x);
	}
	//ȡ�����һ��Ԫ��
	void pop_back() {
		if (empty()) {
			std::cout << "vector null" << std::endl;
			return;
		}
		--finish;
		destroy(finish);
	}
	//�÷���ֻ���ƶ�������û������ɾ������
	//���[first��last)�е�����Ԫ��
	iterator erase(iterator first, iterator last) {
		//�����ж�
		if (first > last || first >= end() || last < begin()) {
			std::cout << " out of range : " << std::endl;
			return nullptr;
		}
		iterator i = std::copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}
	//���ĳһ��λ���ϵ�Ԫ��
	iterator erase(iterator position) {
		//�����ж�
		if (position >= end() || position < begin() ) {
			std::cout << " out of range : " << std::endl;
			return nullptr;
		}
		if (position + 1 != end()) {
			std::copy(position + 1, finish, position);
		}
		--finish;
		destroy(finish);
		return position;
	}
	void resize(size_type new_size, const T& x) {
		if (new_size < size()) {
			erase(begin() + new_size, end());
		}
		else {
			insert(end(), new_size - size(), x);
		}
	}
	void resize(size_type new_size) { return resize(new_size, T()); }
	void clear() { erase(begin(), end()); }
protected:
	void insert_aux(iterator position, const T& x);
	//����ռ�
	iterator allocate_and_fill(size_type n) {
		return data_allocator::allocate(n);
	}
	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		iterator cur = result;
		for ( n > 0; n--; ++cur) {
			construct(&*cur,x);
		}
		return result;
	}
	//assign_dispatch
	template <class Integer>
	void assign_dispatch(Integer n, Integer value, true_type) {
		assign((size_type)n,(T)value);
	}
	template <class InputIterator>
	void assign_dispatch(InputIterator first, InputIterator last, false_type) {
		if (!empty()) {
			clear();
		}
		fill_initialize(last - first);
		for (InputIterator iter = first; iter < last; iter++) {
			*finish = *iter;
			++finish;
		}
	}
};
//����ʵ��
template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x)
{
	const size_type old_size = capacity();
	//����2������
	const size_type len = old_size != 0 ? 2 * old_size : 2;
	iterator new_start = data_allocator::allocate(len);
	iterator new_finish = new_start;
	//�����ǳ��Թ���һ���µ�vector�����˿�������
	//��֪��ΪʲôҪ����д(����)�е��鷳
	try {
		iterator iter = start;
		for (; iter < position; iter++) {
			construct(&*new_finish, *iter);
			++new_finish;
		}
		construct(new_finish, x);
		++new_finish;
	}
	catch (...) {
		//�׳��쳣
		destroy(new_start, new_finish);
		data_allocator::deallocate(new_start, len);
		throw;
	}
	destroy(begin(), end());
	deallocate();
	start = new_start;
	finish = new_finish;
	end_of_storage = new_start + len;
	//��ָ���ÿն���
	new_start = nullptr;
	new_finish = nullptr;
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
{
	//�����ж�
	if (position > finish || position < start) {
		std::cout << "out of range " << std::endl;
		return;
	}

	if (n != 0) {
		//����ռ�����
		if (size_type(end_of_storage - finish) >= n) {
			T x_copy = x;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			//�жϲ����λ��
			if (elems_after > n) {
				//��������п�������
				iterator cur = finish - n;
				for (; cur != old_finish; ++cur) {
					construct(&*finish, *cur);
					++finish;
				}
				//�Ƚ�Ԫ�غ��Ƹ�position�ó��ռ�
				std::copy_backward(position, old_finish - n, old_finish);
				//�����ֵ
				std::fill(position, position + n, x_copy);
			}
			else {
				finish += n;
				//��һ��
				iterator cur = finish - elems_after;
				iterator pi = position;
				for (; cur != finish; ++cur) {
					construct(&*cur, *pi);
					++pi;
				}
				//�ڶ���
				std::fill(position, position + n, x_copy);
				pi = nullptr;
				cur = nullptr;
			}
		}
		else {
			const size_type old_size = size();
			const size_type len = old_size + std::max(old_size, n);
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				//��һ��
				iterator cur = start;
				for (; cur != position; ++cur) {
					construct(&*new_finish, *cur);
					++new_finish;
				}
				//�ڶ���
				std::fill(new_finish, new_finish + n, x);
				new_finish += n;
				//������
				cur = position;
				for (; cur != finish; ++cur) {
					construct(&*new_finish, *cur);
					++new_finish;
				}
				cur = nullptr;
			}
			catch (...) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			destroy(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}
}
_MINI_END

#endif // !_MINI_VEC_
