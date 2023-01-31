#pragma once
#ifndef _MINI_HASH_SET_
#define _MINI_HASH_SET_

#include "hashtable.h"

_MINI_BEGIN

template<class Value,class HashFcn = hash<Value>,
	class EqualKey = equal_to<Value>,class Alloc = alloc>
class hash_set {
private:
	typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;

	ht rp;
public:
	typedef typename ht::key_type key_type;
	typedef typename ht::value_type value_type;
	typedef typename ht::hasher hasher;
	typedef typename ht::key_equal key_equal;
					 
	typedef typename ht::size_type size_type;
	typedef typename ht::difference_type difference_type;
	typedef typename ht::const_pointer pointer;
	typedef typename ht::const_pointer const_pointer;
	typedef typename ht::const_reference reference;
	typedef typename ht::const_reference const_reference;
					 
	typedef typename ht::const_iterator iterator;
	typedef typename ht::const_iterator const_iterator;

public:
	hasher hash_funct() const { return rp.hash_funct(); }
	key_equal key_eq() const { return rp.key_eq(); }
	//这里默认给50
	hash_set() :rp(50,hasher(),key_equal()){}
	explicit hash_set(size_type n):rp(n,hasher(),key_equal()) {}
	hash_set(size_type n, const hasher& hf):rp(n,hf,key_equal()){}
	hash_set(size_type n,const hasher& hf,const key_equal& eql):rp(n,hf,eql){}

	template<class InputIterator>
	hash_set(InputIterator f,InputIterator l) : rp(50,hasher(),key_equal())
	{
		rp.insert_unique(f, l);
	}
	template<class InputIterator>
	hash_set(InputIterator f, InputIterator l,size_type n) : rp(n, hasher(), key_equal())
	{
		rp.insert_unique(f, l);
	}
	template<class InputIterator>
	hash_set(InputIterator f, InputIterator l, size_type n,const hasher& hf) : rp(n, hf, key_equal())
	{
		rp.insert_unique(f, l);
	}
	template<class InputIterator>
	hash_set(InputIterator f, InputIterator l, size_type n, const hasher& hf,const key_equal& eql) : rp(n, hf, eql)
	{
		rp.insert_unique(f, l);
	}


public:
	size_type size() const { return rp.size(); }
	size_type max_size() const { return rp.max_size(); }
	bool empty() const { return rp.empty(); }
	void swap(hash_set& hs) { rp.swap(hs.rp); }

	iterator begin() const { return rp.begin(); }
	iterator end() const { return rp.end(); }
public:
	//类型转换出了问题就不接收返回值了
	/*pair<iterator, bool> insert(const value_type& obj) {
		pair<typename ht::iterator, bool> p = rp.insert_unique(obj);
		return pair<iterator,bool>(p.first,p.second);
	}*/
	void insert(const value_type& obj) {
		rp.insert_unique(obj);
	}
	template<class InputIterator>
	void insert(InputIterator f, InputIterator l) { rp.insert_unique(f, l); }
	pair<iterator, bool> insert_noresize(const value_type& obj) {
		pair<typename ht::iterator, bool> p = rp.insert_unique_noresize(obj);
		return pair<iterator, bool>(p.first, p.second);
	}
	iterator find(const key_type& key) const { return rp.find(key); }
	//其实可以直接返回1
	size_type count(const key_type& key) const { return rp.count(key); }

	/*pair<iterator, bool> equal_range(const key_type & key) const {
		return rp.equal_range(key);
	}*/
	size_type erase(const key_type& key) { return rp.erase(key); }
	void erase(iterator it) { rp.erase(it); }
	void clear() { rp.clear(); }

	void resize(size_type hint) { rp.resize(hint); }
	size_type bucket_count() const { return rp.bucket_count(); }
	size_type max_bucket_count() const { return rp.max_bucket_count(); }
	size_type elems_in_bucket(size_type n) const { return rp.elems_in_bucket(n); }

	bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1);
	bool operator!=(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1);
};					 


template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool hash_set<Value, HashFcn, EqualKey, Alloc>::operator==(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1)
{
	return rp == hs1.rp;
}
template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool hash_set<Value, HashFcn, EqualKey, Alloc>::operator!=(const hash_set<Value, HashFcn, EqualKey, Alloc>& hs1)
{
	return !(rp == hs1.rp);
}
_MINI_END

#endif // !_MINI_HASH_SET_


