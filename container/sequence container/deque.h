#pragma once
#ifndef _MINI_DEQUE_
#define _MINI_DEQUE_


#include "dependence.h"

_MINI_BEGIN
inline size_t __deque_buf_size(size_t sz) {
	//如果大于512每个缓冲区就只有一个元素
	//512可以酌情修改
	return sz < 512 ? size_t(512 / sz) : size_t(1);
}
template<class T,class Ref,class Ptr>
struct __deque_iterator {
	typedef __deque_iterator<T,T&, T*> iterator;
	typedef __deque_iterator<T,const T&,const T*> const_iterator;
	static size_t buffer_size() { return __deque_buf_size(sizeof(T)); }

	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;

	typedef __deque_iterator self;
	//容器结构
	T* cur;
	T* first;
	T* last;
	map_pointer node;

	__deque_iterator():cur(0),first(0),last(0),node(0) {}
	__deque_iterator(T* x, map_pointer y) :cur(x), first(*y),
		last(*y+buffer_size()),node(y) {};
	__deque_iterator(const iterator& x):cur(x.cur),first(x.first),last(x.last),node(x.node) {}

	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}
	reference operator*() const { return *cur; }
	reference operator->() const { return &(*operator*()); }
	difference_type operator-(const self& x) const {
		return difference_type(buffer_size()) * (node - x.node - 1) +
			(cur - first) + (x.last - x.cur);
	}
	self& operator++() {
		++cur;
		if (cur == last) {//如果达到尾端
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		++*this;
		return tmp;
	}
	self& operator--() {
		if (cur == first) {//如果碰到最前端
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		--* this;
		return tmp;
	}
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size())) {
			//在同一缓冲区内
			cur += n;
		}
		else {
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
				: -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}
	self operator+(difference_type n) const {
		self tmp = *this;
		return tmp += n;
	}
	self& operator-=(difference_type n) { return *this += -n; }
	self operator-(difference_type n) const {
		self tmp = *this;
		return tmp -= n;
	}
	reference operator[](difference_type n) const { return *(*this + n); }
	bool operator==(const self& x) const { return cur == x.cur; }
	bool operator!=(const self& x) const { return cur != x.cur; }
	bool operator<(const self& x) const {
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}
	bool operator>(const self& x) const { return x < *this; };
	bool operator<=(const self& x) const { return !(x < *this); }
	bool operator>=(const self& x) const { return !(*this < x); }
};

template<class T,class Alloc = mini::alloc>
class deque {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const pointer const_pointer;
	typedef value_type& reference;
	typedef const reference const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef __deque_iterator<T, T&, T*> iterator;
	typedef __deque_iterator<T,const T&,const T*> const_iterator;

	typedef mini::reverse_iterator<iterator> reverse_iterator;
	typedef mini::reverse_iterator<const_iterator> const_reverse_iterator;
protected:
	enum { initial_map_size = 8 };
	typedef pointer* map_pointer;

	//map节点
	iterator start;//第一个节点
	iterator finish;//最后一个节点

	map_pointer map;
	size_type map_size;//有多少个指标

	typedef simple_alloc<value_type, Alloc> data_allocator;
	typedef simple_alloc<pointer, Alloc> map_allocator;
public:
	static size_t buffer_size() { return __deque_buf_size(sizeof(T)); }

	deque() { fill_initialize(0,value_type()); };
	explicit deque(size_type n, const value_type& value = value_type()) :start(), finish(), map(0), map_size(0)
	{
		fill_initialize(n, value);
	}
	template <class InputIterator>
	explicit deque(InputIterator first,InputIterator last) :start(), finish(), map(0), map_size(0) {
		fill_initialize(first, last);
	}
	deque(std::initializer_list<T> list) :start(), finish(), map(0), map_size(0) {
		fill_initialize(list.begin(), list.end());
	}
	//拷贝构造
	deque(const deque& x) :start(), finish(), map(0), map_size(0) {
		fill_initialize(x.begin(), x.end());
	}

	~deque() { destroy(start,finish); }

	template <class InputIterator>
	void assign(InputIterator first, InputIterator last) {
		if (!empty()) {
			clear();
		}
		fill_initialize(first, last);
	}
	void assign(size_type n, const value_type& value) {
		if (!empty()) {
			clear();
		}
		fill_initialize(n, value);
	}
	void resize(size_type n,const value_type& value = value_type()) {
		const size_type len = size();
		if (n < len) {
			erase(start + n, finish);
		}
		else {
			insert(finish,n - len,value);
		}
	}

	iterator begin() { return start; }
	const_iterator begin() const { return start; }
	iterator end() { return finish; }
	const_iterator end() const { return finish; }

	reverse_iterator rbegin() { return reverse_iterator(++finish); }
	reverse_iterator rend() { return reverse_iterator(++start); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(++finish); }
	const_reverse_iterator rend() const { return const_reverse_iterator(++start); }

	reference operator[](size_type n) {
		return start[difference_type(n)];
	}
	const_reference operator[](size_type n) const {
		return start[difference_type(n)];
	}
	reference at(size_type n) {
		return (*this)[n];
	}
	const_reference at(size_type n) const {
		return (*this)[n];
	}

	reference front() { return *start; }
	const_reference front() const { return *start; }
	reference back() { 
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}
	const_reference back() const{
		const_iterator tmp = finish;
		--tmp;
		return *tmp;
	}
	size_type size() const { return finish - start ; }
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return finish == start; }

	void push_back(const value_type& t) {
		if (finish.cur != finish.last - 1) {
			construct(finish.cur, t);
			++finish.cur;
		}
		else {//没空间了
			push_back_aux(t);
		}
	}
	void push_back_aux(const value_type& t);
	void push_front(const value_type& t) {
		if (start.cur != start.first ) {
			construct(start.cur - 1, t);
			--start.cur;
		}
		else {
			push_front_aux(t);
		}
	}
	void push_front_aux(const value_type& t);
	void pop_back() {
		if (finish.cur != finish.first) {
			--finish.cur;
			destroy(finish.cur);
		}
		else {
			pop_back_aux();
		}
	}
	void pop_back_aux();
	void pop_front() {
		if (start.cur != start.last) {
			destroy(start.cur);
			++start.cur;
		}
		else {
			pop_front_aux();
		}
	}
	void pop_front_aux();

	void clear();
	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if (index < (size() >> 1)) {//如果清除之前的元素比较少
			std::copy_backward(start, pos, next);
			pop_front();
		}
		else {
			std::copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}
	iterator erase(iterator first, iterator last);
	iterator insert(iterator position, const value_type& x) {
		if (position.cur == start.cur) {
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur) {
			push_back(x);
			iterator tmp = finish;
			return (--tmp);
		}
		else {
			return insert_aux(position, x);
		}
	}
	iterator insert(iterator position, size_type n, const value_type& val) {
		iterator tmp = position;
		while (n--)
		{
			insert(position, val);
		}
		return position;
	}
	iterator insert_aux(iterator pos, const value_type& x);
protected:
	pointer allocate_node() { return data_allocator::allocate(__deque_buf_size(sizeof(T))); }
	void deallocate_node(pointer p) { data_allocator::deallocate(p); }

	//这里如果写模板需要判断 int类型以及普通的指针类型和迭代器类型
	//所以这里重复写同样的代码类型就好了
	//这里代码到时候再看看能不能优化
	void fill_initialize(size_type n,const value_type& value);
	void fill_initialize(iterator first,iterator last);
	void fill_initialize(pointer first, pointer last);
	void fill_initialize(const_iterator first, const_iterator last);
	//只能这么写
	void fill_initialize(const value_type* first, const value_type* last);

	void create_map_and_nodes(size_type num_elements);
	//重新扩容map空间
	void reserve_map_at_back(size_type node_add = 1) {
		if (node_add + 1 > map_size - (finish.node - map)) {
			reallocate_map(node_add,false);
		}
	}
	void reserve_map_at_front(size_type node_add = 1) {
		if (node_add > size_type(start.node - map)) {
			reallocate_map(node_add, true);
		}
	}
	void reallocate_map(size_type n,bool add_front);

};
template<class T, class Alloc>
void deque<T, Alloc>::push_back_aux(const value_type& t)
{
	value_type t_copy = t;
	//符合条件更换map
	reserve_map_at_back();
	*(finish.node + 1) = allocate_node();
	try {
		construct(finish.cur,t_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}
	catch (...) {
		destroy(*(finish.node+1));
		deallocate_node(*(finish.node + 1));
	}
}
template<class T, class Alloc>
void deque<T, Alloc>::push_front_aux(const value_type& t)
{
	value_type t_copy = t;
	reserve_map_at_front();
	*(start.node - 1) = allocate_node();
	try {
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, t_copy);
	}
	catch(...) {
		start.set_node(start.node + 1);
		start.cur = start.first;
		deallocate_node(*(start.node - 1));
		throw;
	}
}
template<class T, class Alloc>
void deque<T, Alloc>::pop_back_aux()
{
	deallocate_node(finish.first);//释放最后一个缓存区
	finish.set_node(finish.node - 1);
	finish.cur = finish.last - 1;
	destroy(finish.cur);//将元素释放
}
template<class T, class Alloc>
void deque<T, Alloc>::pop_front_aux()
{
	destroy(start.cur);
	deallocate_node(start.first);
	start.set_node(start.node + 1);
	start.cur = start.first;
}
template<class T, class Alloc>
void deque<T, Alloc>::clear()
{
	for (map_pointer node = start.node + 1; node < finish.node; ++node) {
		destroy(*node, *node + buffer_size());
		data_allocator::deallocate(*node, buffer_size());
	}
	if (start.node != finish.node) {
		destroy(start.cur, start.last);
		destroy(finish.first, finish.cur);
		data_allocator::deallocate(finish.first, buffer_size());
	}
	else {
		destroy(start.cur, finish.cur);
		finish = start;//重置状态
	}
}
template<class T, class Alloc>
deque<T,Alloc>::template iterator deque<T, Alloc>::erase(iterator first, iterator last)
{
	//如果清除整个区间的话
	if (first == start && last == finish) {
		clear();
		return finish;
	}
	else {
		difference_type n = last - first;
		difference_type elems_before = first - start;
		if (elems_before < (size() - n) / 2) {
			std::copy_backward(start, first, last);
			iterator new_start = start + n;
			destroy(start, new_start);
			for (map_pointer cur = start.node; cur < new_start.node; ++cur)
				data_allocator::deallocate(*cur, buffer_size());
			start = new_start;
		}
		else {
			std::copy(last, finish, first);
			iterator new_finish = finish - n;
			destroy(new_finish, finish);
			for (map_pointer cur = new_finish.node + 1; cur <= finish.node; cur++) {
				data_allocator::deallocate(*cur, buffer_size());
			}
			finish = new_finish;//设置新的尾节点
		}
		return start + elems_before;
	}
}
template<class T, class Alloc>
deque<T, Alloc>::template iterator
deque<T, Alloc>::insert_aux(iterator pos, const value_type& x)
{
	difference_type index = pos - start;
	value_type x_copy = x;
	//经过测试这里必须要减1
	if (index < size()-1  / 2) {
		push_front(front());
		iterator front1 = start;
		++front1;
		iterator front2 = front1;
		++front2;
		pos = start + index;
		iterator pos1 = pos;
		++pos1;
		std::copy(front2, pos1, front1);
	}
	else {
		push_back(back());
		iterator back1 = finish;
		--back1;
		iterator back2 = back1;
		--back2;
		pos = start + index;
		std::copy_backward(pos, back2, back1);
	}
	*pos = x_copy;
	return pos;
}
template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(size_type n, const value_type& value)
{
	create_map_and_nodes(n);
	map_pointer cur;
	try {
		for (cur = start.node; cur < finish.node; ++cur) {
			//初始化构建
			pointer tmp = *cur;
			for (; tmp != *cur+buffer_size(); ++tmp)
				construct(&*tmp, value);
		}
		pointer tmp = finish.first;
		for (; tmp != finish.cur; ++tmp)
			construct(&*tmp, value);
	}catch (...) {
		destroy(*cur,*cur+buffer_size());
		data_allocator::deallocate(*cur,buffer_size());
		destroy(finish.first,finish.cur);
		data_allocator::deallocate(finish.first, finish.cur - finish.first);
		throw;
	}
}
template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(iterator first, iterator last)
{
	ptrdiff_t n = distance(first, last);
	create_map_and_nodes(n);
	map_pointer cur;
	iterator st = first;
	//这里没有什么特别好的办法就稍微改一改
	try {
		for (cur = start.node; cur < finish.node; ++cur) {
			//初始化构建
			pointer tmp = *cur;
			for (; tmp != *cur + buffer_size(); ++tmp, ++st)
				construct(&*tmp, *st.cur);
		}
		pointer tmp = finish.first;
		for (; tmp != finish.cur; ++tmp, ++st)
			construct(&*tmp, *st.cur);
	}
	catch (...) {
		destroy(*cur, *cur + buffer_size());
		data_allocator::deallocate(*cur, buffer_size());
		destroy(finish.first, finish.cur);
		data_allocator::deallocate(finish.first, finish.cur - finish.first);
		throw;
	}
}
template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(pointer first, pointer last)
{
	ptrdiff_t n = last-first;
	create_map_and_nodes(n+1);
	map_pointer cur;
	pointer st = first;
	//这里没有什么特别好的办法就稍微改一改
	try {
		for (cur = start.node; cur < finish.node; ++cur) {
			//初始化构建
			pointer tmp = *cur;
			for (; tmp != *cur + buffer_size(); ++tmp, ++st)
				construct(&*tmp, *st);
		}
		pointer tmp = finish.first;
		for (; tmp != finish.cur; ++tmp,++st)
			construct(&*tmp, *st);
	}
	catch (...) {
		destroy(*cur, *cur + buffer_size());
		data_allocator::deallocate(*cur, buffer_size());
		destroy(finish.first, finish.cur);
		data_allocator::deallocate(finish.first, finish.cur - finish.first);
		throw;
	}
}
template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(const_iterator first, const_iterator last)
{
	ptrdiff_t n = distance(first, last);
	create_map_and_nodes(n);
	map_pointer cur;
	const_iterator st = first;
	//这里没有什么特别好的办法就稍微改一改
	try {
		for (cur = start.node; cur < finish.node; ++cur) {
			//初始化构建
			pointer tmp = *cur;
			for (; tmp != *cur + buffer_size(); ++tmp, ++st)
				construct(&*tmp, *st.cur);
		}
		pointer tmp = finish.first;
		for (; tmp != finish.cur; ++tmp, ++st)
			construct(&*tmp, *st.cur);
	}
	catch (...) {
		destroy(*cur, *cur + buffer_size());
		data_allocator::deallocate(*cur, buffer_size());
		destroy(finish.first, finish.cur);
		data_allocator::deallocate(finish.first, finish.cur - finish.first);
		throw;
	}
}
template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(const value_type* first,const value_type* last)
{
	ptrdiff_t n = last - first ;
	create_map_and_nodes(n);
	map_pointer cur;
	const value_type* st = first;
	//这里没有什么特别好的办法就稍微改一改
	try {
		for (cur = start.node; cur < finish.node; ++cur) {
			//初始化构建
			pointer tmp = *cur;
			for (; tmp != *cur + buffer_size(); ++tmp, ++st)
				construct(&*tmp, *st);
		}
		pointer tmp = finish.first;
		for (; tmp != finish.cur; ++tmp, ++st)
			construct(&*tmp, *st);
	}
	catch (...) {
		destroy(*cur, *cur + buffer_size());
		data_allocator::deallocate(*cur, buffer_size());
		destroy(finish.first, finish.cur);
		data_allocator::deallocate(finish.first, finish.cur - finish.first);
		throw;
	}
}

template<class T, class Alloc>
void deque<T, Alloc>::create_map_and_nodes(size_type num_elements)
{
	//需要节点数
	size_type num_nodes = num_elements / buffer_size() + 1;
	//最大节点数 最少管理8个节点
	map_size = initial_map_size < num_nodes + 2 ? num_nodes + 2 : initial_map_size;
	map = map_allocator::allocate(map_size);
	//尽量放在中央位置
	map_pointer nstart = map + (map_size - num_nodes) / 2;
	map_pointer nfinish = nstart + num_nodes - 1;
	map_pointer cur;
	try {
		for (cur = nstart; cur <= nfinish; ++cur) {
			*cur = allocate_node();
		}
	}
	catch (...) {
		destroy(nstart,nfinish);
		map_allocator::deallocate(nstart, nfinish - nstart);
		destroy(*nstart,*nfinish);
		data_allocator::deallocate(*nstart,*nfinish-*nstart);
		throw;
	}
	start.set_node(nstart);
	finish.set_node(nstart);
	start.cur = start.first;
	finish.cur = finish.first + num_elements % buffer_size();
}
template<class T, class Alloc>
void deque<T, Alloc>::reallocate_map(size_type n, bool add_front)
{
	size_type old_num_nodes = finish.node - start.node + 1;
	size_type new_num_nodes = old_num_nodes + n;
	map_pointer new_start;
	//判断是否超出两倍的空间如果没有就移动内存位置
	if (map_size > 2 * new_num_nodes) {
		new_start = map + (map_size - new_num_nodes) / 2 + (add_front ? n : 0);
		if (new_start < start.node) {
			//新节点在前面就往前移
			std::copy(start.node,finish.node+1,new_start);
		}   //新节点在后面就往后移
		else {
			std::copy_backward(start.node, finish.node + 1, new_start + old_num_nodes);
		}
	}
	else {
		size_type new_map_size = map_size + std::max(map_size, n) + 2;
		
		map_pointer new_map = map_allocator::allocate(new_map_size);
		//确定起点
		new_start = new_map + (new_map_size - new_num_nodes) / 2 + (add_front ? n : 0);
		std::copy(start.node, finish.node + 1, new_start);
		//释放内存
		map_allocator::deallocate(map,map_size);

		map = new_map;
		map_size = new_map_size;
	}
	start.set_node(new_start);
	finish.set_node(new_start + old_num_nodes - 1);
}
_MINI_END

#endif // _MINI_DEQUE_


