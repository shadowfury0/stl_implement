#pragma once
#ifndef _MINI_HASH_TABLE_
#define _MINI_HASH_TABLE_

#include "dependence.h"
#include "../sequence container/vector.h"
#include "../../algorithm/algo.h"
#include "pair.h"

_MINI_BEGIN
template<class Value>
struct __hashtable_node
{
	__hashtable_node* next;
	Value val;
};
//类声明
template <class _Val, class _Key, class _HashFcn,
	class _ExtractKey, class _EqualKey, class _Alloc = alloc>
class hashtable;
template <class _Val, class _Key, class _HashFcn,
	class _ExtractKey, class _EqualKey, class _Alloc = alloc>
class __hashtable_iterator;
template <class _Val, class _Key, class _HashFcn,
	class _ExtractKey, class _EqualKey, class _Alloc = alloc>
class __hashtable_const_iterator;

//hashtable迭代器没有--操作，以及逆向迭代器
template<class Value,class Key,class HashFcn,
	class ExtractKey,class EqualKey,class Alloc>
struct __hashtable_iterator {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		iterator;
	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		const_iterator;
	typedef __hashtable_node<Value> node;

	typedef forward_iterator_tag  iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t  size_type;
	typedef Value& reference;
	typedef Value* pointer;

	node* cur;//迭代器目前所指的节点
	hashtable* ht;//保持容器连结关系

	__hashtable_iterator(node* n,hashtable* tab) :cur(n),ht(tab){}
	__hashtable_iterator() {}
	reference operator*() const { return cur->val; }
	pointer  operator->() const { return &(operator*()); }
	iterator& operator++();
	iterator  operator++(int);
	bool  operator==(const iterator& it) const { return cur == it.cur; }
	bool  operator!=(const iterator& it) const { return cur != it.cur; }
};
//常量迭代器
template<class Value, class Key, class HashFcn,
	class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_const_iterator {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		iterator;
	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		const_iterator;
	typedef __hashtable_node<Value> node;

	typedef forward_iterator_tag  iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t  size_type;
	typedef const Value& reference;
	typedef const Value* pointer;
	//注意这里都需要加const 不然会出现转换问题
	const node* cur;//迭代器目前所指的节点
	const hashtable* ht;//保持容器连结关系

	__hashtable_const_iterator(const node* n,const hashtable* tab) :cur(n), ht(tab) {}
	__hashtable_const_iterator() {}
	reference operator*() const { return cur->val; }
	pointer  operator->() const { return &(operator*()); }
	const_iterator& operator++();
	const_iterator  operator++(int);
	bool  operator==(const const_iterator& it) const { return cur == it.cur; }
	bool  operator!=(const const_iterator& it) const { return cur != it.cur; }
};


enum { __mini_num_primes = 28 };
// SGI STL 提供 28 个质数，用质数大小来设计 buckets
static const unsigned long __mini_prime_list[__mini_num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};

//找出最接近的那个质数，并大于它
inline unsigned long __mini_next_prine(unsigned long n) {
	const unsigned long* first = __mini_prime_list;
	const unsigned long* last = __mini_prime_list + __mini_num_primes;
	const unsigned long* pos = lower_bound(first,last,n);

	return pos == last ? *(last - 1) : *pos;
}


template<class Value,class Key,class HashFcn,
	class ExtractKey,class EqualKey,class Alloc>
class hashtable {
public:
	typedef Key   key_type;
	typedef Value value_type;
	typedef HashFcn hasher;
	typedef EqualKey key_equal;

	typedef size_t  size_type;
	typedef ptrdiff_t  difference_type;
	typedef value_type* pointer;
	typedef  const value_type* const_pointer;
	typedef  value_type& reference;
	typedef  const value_type& const_reference;
	hasher hash_funct() const { return hash; }
	key_equal key_eq() const { return equals; }
private:
	hasher hash;
	key_equal equals;
	ExtractKey get_key;

	typedef __hashtable_node<Value> node;
	typedef simple_alloc<node, Alloc>  node_allocator;

	vector<node*, Alloc>  buckets;
	size_type  num_elements;
public:
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		iterator;
	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey,
		Alloc>
		const_iterator;
	//友元类
	friend struct
		__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	friend struct
		__hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
public:
	hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :hash(hf), equals(eql),
		get_key(ExtractKey()), num_elements(0)
	{
		initialize_buckets(n);
	}
	~hashtable() { clear(); }
public:
	iterator begin() {
		for (size_type n = 0; n < buckets.size(); ++n) {
			if (buckets[n])
				return iterator(buckets[n], this);
		}
		return end();
	}
	const_iterator begin() const {
		for (size_type n = 0; n < buckets.size(); ++n) {
			if (buckets[n])
				return const_iterator(buckets[n], this);
		}
		return end();
	}
	iterator end()  { return iterator(0, this); }
	const_iterator end() const { return const_iterator(0, this); }

	size_type bucket_count() const { return buckets.size(); }
	//总共可以有多少个buckets
	size_type max_bucket_count() const { return __mini_prime_list[(int)__mini_num_primes - 1]; }

	pair<iterator, bool> insert_unique(const value_type& obj) {
		resize(num_elements + 1);
		return insert_unique_noresize(obj);//返回不需要扩大的重建表格
	}

	template<class ForwardIterator>
	void insert_unique(ForwardIterator f, ForwardIterator l) {
		size_type n = distance(f, l);
		resize(num_elements + n);
		for (; n > 0; --n, ++f) {
			insert_unique_noresize(*f);
		}
	}
	iterator insert_equal(const value_type& obj) {
		resize(num_elements + 1);
		return insert_equal_noresize(obj);
	}
	template<class ForwardIterator>
	void insert_equal(ForwardIterator f, ForwardIterator l) {
		size_type n = distance(f, l);
		resize(num_elements + n);
		for (; n > 0; --n, ++f) {
			insert_equal_noresize(*f);
		}
	}
	pair<iterator, bool> insert_unique_noresize(const value_type& obj);
	iterator insert_equal_noresize(const value_type& obj);

	size_type erase(const key_type& key);
	void erase(const iterator& it);
	void erase(const const_iterator& it);


	size_type size() const { return num_elements; }
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return num_elements == 0; }
	void clear();
	void swap(hashtable& __ht)
	{
		std::swap(hash, __ht.hash);
		std::swap(equals, __ht.equals);
		std::swap(get_key, __ht.get_key);
		buckets.swap(__ht.buckets);
		std::swap(num_elements, __ht.num_elements);
	}
	void resize(size_type num_elements_hint);
	void copy_from(const hashtable& ht);

	size_type elems_in_bucket(size_type bucket) const{
		size_type result = 0;
		for (node* cur = buckets[bucket]; cur; cur = cur->next) {
			++result;
		}
		return result;
	}

	iterator find(const key_type& key) {
		size_type n = bkt_num_key(key);
		node* first;
		for (first = buckets[n]; first && !equals(get_key(first->val), key);first = first->next) 
		{}
		return iterator(first, this);
	}
	const_iterator find(const key_type& key) const {
		size_type n = bkt_num_key(key);
		const node* first;
		for (first = buckets[n]; first && !equals(get_key(first->val), key); first = first->next)
		{
		}
		return const_iterator(first, this);
	}
	//方便map的一个方法
	reference find_or_insert(const value_type& obj);

	//返回满足该键值相同的个数
	size_type count(const key_type& key) const {
		const size_type n = bkt_num_key(key);
		size_type count = 0;
		for (const node* cur = buckets[n]; cur; cur = cur->next) {
			if (equals(get_key(cur->val), key))
				++count;
		}
		return count;
	}

private:
	size_type next_size(size_type n) const{return __mini_next_prine(n);}
	//键值
	size_type bkt_num_key(const key_type& key) const { return bkt_num_key(key, buckets.size()); }
	size_type bkt_num_key(const key_type& key, size_t n) const { return hash(key) % n; }
	//实际值
	size_type bkt_num(const value_type& obj) const { return bkt_num_key(get_key(obj)); }
	size_type bkt_num(const value_type& obj,size_t n) const { return bkt_num_key(get_key(obj),n); }

	//节点配置与释放
	node* new_node(const value_type& obj) {
		node* n = node_allocator::allocate();
		n->next = 0;
		try {
			construct(&n->val, obj);
			return n;
		}
		catch (...) {
			node_allocator::deallocate(n);
		}
	}
	void delete_node(node* n) {
		destroy(&n->val);
		node_allocator::deallocate(n);
	}

	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);

		buckets.insert(buckets.end(), n_buckets, (node*)0);
		num_elements = 0;
	}
};


template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename __hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator&
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
{
	const node* old = cur;
	cur = cur->next;
	//如果为空
	if (!cur) {
		size_type bucket = ht->bkt_num(old->val);
		//一直找到不为空的bucket
		while (!cur && ++bucket < ht->buckets.size())
			cur = ht->buckets[bucket];
	}
	return *this;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator 
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
{
	iterator tmp = *this;
	++* this;
	return tmp;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator& __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++()
{
	const node* old = cur;
	cur = cur->next;
	//如果为空
	if (!cur) {
		size_type bucket = ht->bkt_num(old->val);
		//一直找到不为空的bucket
		while (!cur && ++bucket < ht->buckets.size())
			cur = ht->buckets[bucket];
	}
	return *this;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline typename __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int)
{
	const_iterator tmp = *this;
	++* this;
	return tmp;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& obj)
{
	const size_type n = bkt_num(obj);
	node* first = buckets[n];

	for (node* cur = first; cur; cur = cur->next) {
		//如果发现值相同就立即返回，不插入
		if (equals(get_key(cur->val), get_key(obj)))
			return pair<iterator, bool>(iterator(cur, this), false);
	}
	node* tmp = new_node(obj);
	tmp->next = first;
	buckets[n] = tmp;
	++num_elements;
	return pair<iterator, bool>(iterator(tmp,this),true);
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj)
{
	const size_type n = bkt_num(obj);
	node* first = buckets[n];

	for (node* cur = first; cur; cur = cur->next) {
		//如果发现值相同就立即返回，不插入
		if (equals(get_key(cur->val), get_key(obj))){
			node* tmp = new_node(obj);
			tmp->next = cur->next;
			cur->next = tmp;
			++num_elements;
			return iterator(tmp,this);
		}
	}
	node* tmp = new_node(obj);
	tmp->next = first;
	buckets[n] = tmp;
	++num_elements;
	return iterator(tmp, this);
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::size_type
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const key_type& key)
{
	const size_type n = bkt_num_key(key);
	node* first = buckets[n];
	size_type e = 0;
	if (first) {
		node* cur = first;
		node* next = cur->next;
		while (next)
		{
			if (equals(get_key(next->val), key)) {
				cur->next = next->next;
				delete_node(next);
				next = cur->next;
				++e;
				--num_elements;
			}
			else {
				cur = next;
				next = cur->next;
			}
		}
		if (equals(get_key(first->val), key)) {
			buckets[n] = first->next;
			delete_node(first);
			++e;
			--num_elements;
		}
	}
	return e;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const iterator& it)
{
	node* p = it.cur;
	if (p) {
		const size_type n = bkt_num(p->val);
		node* cur = buckets[n];
		if (cur == p) {
			buckets[n] = cur->next;
			delete_node(cur);
			--num_elements;
		}
		else {
			node* next = cur->next;
			while (next)
			{
				if (next == p) {
					cur->next = next->next;
					delete_node(next);
					--num_elements;
					break;
				}
				else {
					cur = next;
					next = cur->next;
				}
			}
		}
	}
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::erase(const const_iterator& it)
{
	erase(iterator(const_cast<node*>(it.cur), const_cast<hashtable*>(it.ht)));
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
	for (size_type i = 0; i < buckets.size(); ++i) {
		node* cur = buckets[i];
		while (cur!=0)
		{
			node* next = cur->next;
			delete_node(cur);
			cur = next;
		}
		buckets[i] = 0;
	}
	num_elements = 0;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint)
{
	const size_type old_n = buckets.size();
	if (num_elements_hint > old_n) {//需要重新配置
		const size_type n = next_size(num_elements_hint);//找出下一个质数
		if (n > old_n) {
			vector<node*, Alloc> tmp(n, (node*)0);//新的buckets
			try {
				//处理之前的每一个旧的buckets
				for (size_type bucket = 0; bucket < old_n; ++bucket) {
					node* first = buckets[bucket];
					while (first)
					{
						//找出下一个节点在哪个位置
						size_type new_bucket = bkt_num(first->val, n);
						//让旧的bucket指向对应串行的下一个节点
						buckets[bucket] = first->next;
						//将当前节点插入到新的bucket内，成为其对应串行的第一个节点
						first->next = tmp[new_bucket];
						tmp[new_bucket] = first;
						//回到旧的bucket处理之前的串行节点，准备下一个节点
						first = buckets[bucket];
					}
				}
				buckets.swap(tmp);//两个bucket对调
				//离开时释放local tmp 内存
			}
			catch (...) {
				for (size_type bucket = 0; bucket < tmp.size(); ++bucket) {
					while (tmp[bucket])
					{
						node* next = tmp[bucket]->next;
						delete_node(tmp[bucket]);
						tmp[bucket] = next;
					}
				}
				throw;
			}
		}
	}
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& ht)
{
	//清除buckets ,重新分配空间
	buckets.clear();
	//为bucket保留空间 跟拷贝的相同，如果大于对方的空间就不动，小于就会扩大
	buckets.insert(buckets.end(),ht.buckets.size(), (node*)0);

	try {
		for (size_type i = 0; i < ht.buckets.size(); ++i) {
			if (const node* cur = ht.buckets[i]) {
				node* copy = new_node(cur->val);
				buckets[i] = copy;
				for (node* next = cur->next; cur = next; next = cur->next) {
					copy->next = new_node(next->val);
					copy = copy->next;
				}
			}
		}
		num_elements = ht.num_elements;
	}
	catch (...) {
		clear();
	}
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::reference 
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::find_or_insert(const value_type& obj)
{
	resize(num_elements + 1);
	size_type n = bkt_num(obj);
	node* first = buckets[n];
	for (node* cur = first; cur; cur = cur->next) {
		if (equals(get_key(cur->val), get_key(obj))) {
			return cur->val;
		}
	}
	node* tmp = new_node(obj);
	tmp->next = first;
	buckets[n] = tmp;
	++num_elements;
	return tmp->val;
}



//hash fun 仿函数
template <class _Key> struct hash { };
// 对字符字符串进行转换
inline size_t __stl_hash_string(const char* __s)
{
	unsigned long __h = 0;
	for (; *__s; ++__s)
		__h = 5 * __h + *__s;

	return size_t(__h);
}
template<>
struct hash<char*>
{
	size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
};
template<>
struct hash<const char*>
{
	size_t operator()(const char* __s) const { return __stl_hash_string(__s); }
};
// 其它类型，直接返回原值
template<>
struct hash<char> {
	size_t operator()(char __x) const { return __x; }
};
template<>
struct hash<unsigned char> {
	size_t operator()(unsigned char __x) const { return __x; }
};
template<>
struct hash<signed char> {
	size_t operator()(unsigned char __x) const { return __x; }
};
template<>
struct hash<short> {
	size_t operator()(short __x) const { return __x; }
};
template<>
struct hash<unsigned short> {
	size_t operator()(unsigned short __x) const { return __x; }
};
template<>
struct hash<int> {
	size_t operator()(int __x) const { return __x; }
};
template<>
struct hash<unsigned int> {
	size_t operator()(unsigned int __x) const { return __x; }
};
template<>
struct hash<long> {
	size_t operator()(long __x) const { return __x; }
};
template<>
struct hash<unsigned long> {
	size_t operator()(unsigned long __x) const { return __x; }
};


_MINI_END

#endif // !_MINI_HASH_TABLE_


