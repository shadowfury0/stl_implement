#pragma once
#ifndef  _MINI_LIST_


#include "dependence.h"

_MINI_BEGIN
template<class T>
struct _list_node {
	typedef void* void_pointer;
	void_pointer prev;
	void_pointer next;
	T data;
};
//列表的迭代器
template<class T,class Ref,class Ptr>
struct list_iterator {
	typedef list_iterator<T, T&, T*> iterator;
	typedef list_iterator<T,const T&,const T*> const_iterator;
	typedef list_iterator<T, Ref, Ptr> self;

	typedef bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef _list_node<T>* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	
	link_type node;

	list_iterator(link_type x) :node(x) {};
	list_iterator() {};
	list_iterator(const iterator& x) : node(x.node) {};

	bool operator==(const self& x) const { return node == x.node; }
	bool operator!=(const self& x) const { return node != x.node; }
	//取值
	reference operator*() const { return (*node).data; }
	pointer	  operator->() const { return &(operator*()); }
	self& operator++() {
		node = (link_type)((*node).next);
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		++* this;
		return tmp;
	}
	self& operator--() {
		node = (link_type)((*node).prev);
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		--* this;
		return tmp;
	}

};
template <class T,class Alloc = mini::alloc>//预设配置
class list {
protected:
	typedef _list_node<T> list_node;
public:
	typedef list_node* link_type;

	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const reference const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef list_iterator<T, T&, T*> iterator;
	typedef list_iterator<T, const T&, const T*> const_iterator;
	//加命名空间可以区分名称
	typedef mini::reverse_iterator<iterator>  reverse_iterator;
	typedef mini::reverse_iterator<const_iterator>  const_reverse_iterator;
	//空间配置器
	typedef simple_alloc<list_node, Alloc> list_node_allocator;
protected:
	link_type node;//表示环形状的双向序列，辅助节点

public:

	list() { empty_initialize(); }
	explicit list(size_type n,const value_type& val) { 
		empty_initialize();
		assign(n, val);
	}
	template <class InputIterator>
	explicit list(InputIterator first, InputIterator last) {
		empty_initialize();
		assign(first,last);
	}
	list(const list& x) {
		empty_initialize();
		assign(x.begin(),x.end());
	}//列表初始化
	list(std::initializer_list<T> list) {
		empty_initialize();
		auto iter = list.begin();
		for (; iter != list.end(); ++iter) {
			push_back(*iter);
		}
	}
	~list() {};
	list& operator=(const list& x) {
		if (empty()) {
			empty_initialize();
		}
		assign(x.begin(), x.end());
	};
	
	void assign(size_type n, const value_type& val) {
		if (!empty()) {
			clear();
		}
		if (n == 0) {
			std::cout << " n variable is zero ! " << std::endl;
			return;
		}
		while (n--)
		{
			push_back(val);
		}
	}
	template<class InputIterator>
	void assign(InputIterator first, InputIterator last) {
		typedef typename Is_Integer<InputIterator>::value type;
		assign_dispatch(first, last, type());
	}

	iterator begin() { return (link_type)((*node).next); }
	const_iterator begin() const { return (link_type)((*node).next); }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return reverse_iterator(end()); }

	iterator end()	 { return node; }
	const_iterator end()  const	 { return node; }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return reverse_iterator(begin()); }

	bool empty() const { return node->next == node; }
	size_type size() const {
		size_type result = mini::distance(begin(), end());
		return result;
	}
	reference front() { return *begin(); }
	const_reference front() const { return *begin(); }
	reference back()  { return *(--end()); }
	const_reference back() const { return *(--end()); }
	void push_front(const T& x) { insert(begin(), x); }
	void push_back(const T& x)  { insert(end(), x); }
	void pop_front()  { erase(begin()); }
	void pop_back()   { erase(--end()); }
	iterator  insert(iterator position, const T& x) {
		link_type tmp = create_node(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		(link_type(position.node->prev))->next = tmp;
		position.node->prev = tmp;
		return tmp;
	}
	//返回插入位置的节点
	iterator  insert(iterator position, size_type n, const value_type& val) {
		while (n--)
		{
			position = insert(position,val);
		}
		return position;
	}
	template <class InputIterator>   
	void insert(iterator position, InputIterator first, InputIterator last) {
		while (first != last)
		{
			insert(position,*first++);
		}
	};
	iterator  erase(iterator position) {
		link_type next_node = link_type(position.node->next);
		link_type prev_node = link_type(position.node->prev);
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destroy_node(position.node);
		//返回下一个节点
		return iterator(next_node);
	}
	iterator erase(iterator first, iterator last) {
		while ( first != last )
		{
			erase(first++);
		}
		return first;
	}
	// 交换内容
	void swap(list& x)  
	{
		//书上写的有问题，可能是编译器问题
		std::swap(this->node, x.node);
	}
	//移除所有元素
	void remove(const T& value);
	//传入的是一个判断条件的函数，移除符合判断条件的元素
	template<class Predicate >
	void remove_if(Predicate pred);
	//移除数值(连续)相同的元素
	void unique();
	//清除所有节点
	void clear();
	//将另一个list的[first,last)内所有的元素搬到position之前。之前的列表有损失
	void transfer(iterator position, iterator first, iterator last);
	//跟上一个方法一样，传入的是一个list
	void splice(iterator position, list& x) {
		if (!x.empty()) {
			transfer(position, x.begin(), x.end());
		}
	}
	//将i所指元素插入到position所指的位置之前。position和i可以指向同一个list
	void splice(iterator position, list&, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j) return;
		transfer(position, i, j);
	}
	//将[first,last)内所有元素插入到position所指的位置之前。
	//可以指向同一个list
	//但是poition不能在[first,last)之内。
	void splice(iterator position, list&, iterator first, iterator last) {
		if (first!= last) {
			transfer(position, first, last);
		}
	}
	//将两个列表从小到大合并
	//注意前提是两个lists都已经经过递增排序。
	void merge(list& x);
	//将列表内容的内容倒过来
	void reverse();
	//因为STL算法sort只接受 RamdonAccessIterator ，所有这个排序必须自己重写
	//采用快速排序
	void sort();
protected:
	void empty_initialize() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}

	//配置一个节点并返回
	link_type get_node() { return list_node_allocator::allocate(); }
	//释放一个节点
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	//产生一个节点
	link_type create_node(const T& x) {
		link_type p = get_node();
		construct(&p->data, x);
		return p;
	}
	//摧毁一个节点
	void destroy_node(link_type p) {
		destroy(&p->data);
		put_node(p);
	}

	//assign分配
	template <class Integer>
	void assign_dispatch(Integer n, Integer value, true_type) {
		assign((size_type)n,(T)value);
	}
	template <class InputIterator>
	void assign_dispatch(InputIterator first,InputIterator last,false_type) {
		if (!empty()) {
			clear();
		}
		if (first == 0 || last == 0) {
			std::cout << "input error ! " << std::endl;
			return;
		}
		//只有一个元素时
		if (first == last) {
			push_back(*first);
			return;
		}

		for (; first != last; ++first) {
			push_back(*first);
		}
	}
};
template <class T,class Alloc>
void list<T, Alloc>::clear() {
	link_type cur = (link_type)node->next;
	while (cur != node) {
		link_type tmp = cur;
		cur = (link_type)cur->next;
		destroy_node(tmp);
	}
	//恢复node的原始状态
	node->next = node;
	node->prev = node;
}
template <class T,class Alloc>
void list<T, Alloc>::remove(const T& value) {
	iterator first = begin();
	iterator last = end();
	//访问每一个节点
	while (first!=last)
	{
		iterator next = first;
		++next;
		if (*first == value) erase(first);
		first = next;
	}
}
template<class T, class Alloc>
template<class Predicate>
void list<T, Alloc>::remove_if(Predicate pred)
{
	iterator first = begin();
	iterator last = end();
	while ( first != last )
	{
		iterator next = first;
		++next;
		//传入到函数进行判断
		if (pred(*first)) {
			erase(first);
		}
		first = next;
	}
}
template <class T, class Alloc>
void list<T, Alloc>::unique() {
	iterator first = begin();
	iterator last = end();
	if (first == last) return;//什么元素都没有返回
	iterator next = first;
	while (++next != last)
	{
		if (*first == *next) {
			erase(next);
		}
		else {
			first = next;
		}
		next = first;//修正
	}
}
template <class T, class Alloc>
void list<T, Alloc>::transfer(iterator position, iterator first, iterator last) {
	if (position != last) {
		(*(link_type((*last.node).prev))).next = position.node;
		(*(link_type((*first.node).prev))).next = last.node;
		(*(link_type((*position.node).prev))).next = first.node;
		link_type tmp = link_type((*position.node).prev);
		(*position.node).prev = (*last.node).prev;
		(*last.node).prev = (*first.node).prev;
		(*first.node).prev = tmp;
	}
}
template <class T, class Alloc>
void list<T, Alloc>::merge(list<T, Alloc>& x) {
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = x.begin();
	iterator last2 = x.end();
	//注意前提是两个lists都已经经过递增排序.
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			iterator next = first2;
			transfer(first1, first2, ++next);
			first2 = next;
		}
		else {
			++first1;
		}
		if (first2!= last2) transfer(last1, first2, last2);
	}
}
template <class T, class Alloc>
void list<T, Alloc>::reverse() {
	//如果列表为空或者只有一个就不用排序
	if (node->next == node || link_type(node->next)->next == node) {
		std::cout << "  No reversing required  " << std::endl;
		return;
	}
	iterator first = begin();
	++first;
	while (first!=end){
		iterator old = first;
		++first;
		transfer(begin(), old, first);
	}
}
template <class T, class Alloc>
void list<T, Alloc>::sort() {
	//如果列表为空或者只有一个就不用排序
	if (node->next == node || link_type(node->next)->next == node) {
		std::cout << "  No sorting required  " << std::endl;
		return;
	}
	//作为存放区
	list<T, Alloc> carry;
	list<T, Alloc> counter[64];
	int fill = 0;
	while (!empty())
	{
		carry.splice(carry.begin(), *this, begin());
		int i = 0;
		while (i < fill && !counter[i].empty()) {
			counter[i].merge(carry);
			carry.swap(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill) {
			++fill;
		} 
	}
	for (int i = 1; i < fill; ++i) {
		counter[i].merge(counter[i - 1]);
	}
	swap(counter[fill - 1]);
}

_MINI_END

#endif // ! _MINI_LIST_


