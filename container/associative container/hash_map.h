#pragma once
#ifndef _MINI_HASH_MAP_
#define _MINI_HASH_MAP_

#include "hashtable.h"

_MINI_BEGIN

template<class Key,class T,class HashFcn = hash<Key>,
	class EqualKey = equal_to<Key>,class Alloc = alloc>
class hash_map {
private:
	typedef hashtable<pair<const Key, T>, Key, HashFcn, select1st<pair<const Key, T>>, EqualKey, Alloc> ht;
	ht rp;
public:
	typedef typename ht::key_type key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef typename ht::value_type value_type;
	typedef typename ht::hasher hasher;
	typedef typename ht::key_equal key_equal;

	typedef typename ht::size_type size_type;
	typedef typename ht::difference_type difference_type;
	typedef typename ht::pointer pointer;
	typedef typename ht::const_pointer const_pointer;
	typedef typename ht::reference reference;
	typedef typename ht::const_reference const_reference;

	typedef typename ht::iterator iterator;
	typedef typename ht::const_iterator const_iterator;

	hasher hash_funct() const { return rp.hash_funct(); }
	key_equal key_eq() const { return rp.key_eq(); }
public:
	hash_map() : rp(100,hasher(),key_equal()) {}
	explicit hash_map(size_type n) : rp(n,hasher(),key_equal()) {}
	hash_map(size_type n,const hasher& hf) : rp(n,hf,key_equal()) {}
	hash_map(size_type n,const hasher& hf,const key_equal& eql) : rp(n,hf,eql) {}

	template<class InputIterator>
	hash_map(InputIterator f, InputIterator l) : rp(100, hasher(), key_equal()) {
		rp.insert_unique(f, l);
	}
	template<class InputIterator>
	hash_map(InputIterator f, InputIterator l,size_type n) : rp(n, hasher(), key_equal()) {
		rp.insert_unique(f, l);
	}
	template<class InputIterator>
	hash_map(InputIterator f, InputIterator l, size_type n,const hasher& hf) : rp(n, hf, key_equal()) {
		rp.insert_unique(f, l);
	}
	template<class InputIterator>
	hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf,const key_equal& eql) : rp(n, hf, eql) {
		rp.insert_unique(f, l);
	}
public:
	size_type size() const { return rp.size(); }
	size_type max_size() const { return rp.max_size(); }
	bool empty() const { return rp.empty(); }
	void swap(hash_map& hs) { rp.swap(hs.rp); }
	friend bool operator==(const hash_map&, const hash_map&);
	
	iterator begin() { return rp.begin(); }
	const_iterator begin() const { return rp.begin(); }
	iterator end() { return rp.end(); }
	const_iterator end() const { return rp.end(); }

public:
	pair<iterator, bool> insert(const value_type& obj) { return rp.insert_unique(obj); }
	template<class InputIterator>
	void insert(InputIterator f, InputIterator l) { rp.insert_unique(f, l); }
	pair<iterator, bool> insert_noresize(const value_type& obj) { return rp.insert_unique_noresize(obj); }
	iterator find(const key_type& key) { return rp.find(key); }

	T& operator[](const key_type& key) {
		return rp.find_or_insert(value_type(key, T())).second;
	}
	size_type erase(const key_type& key) { return rp.erase(key); }
	void erase(iterator it) { return rp.erase(it); }

	void clear() { rp.clear(); }
public:
	void resize(size_type hint) { rp.resize(hint); }
	size_type bucket_count() const { return rp.bucket_count(); }
	size_type max_bucket_count() const { return rp.max_bucket_count(); }
	size_type elems_in_bucket(size_type n) const { return rp.elems_in_bucket(n); }
};

template <class Key, class T, class HashFcn, class EqlKey, class Alloc>
inline bool operator==(const hash_map<Key, T, HashFcn, EqlKey, Alloc>& hm1,
	const hash_map<Key, T, HashFcn, EqlKey, Alloc>& hm2)
{
	return hm1.rp == hm2.rp;
}

_MINI_END

#endif // !_MINI_HASH_MAP_
