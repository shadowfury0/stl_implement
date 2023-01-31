#pragma once
#ifndef _MINI_SET_
#define _MINI_SET_
#include "tree.h"
//set集合底层的实现为rb_tree

_MINI_BEGIN
template<class Key,class Compare = less<Key>,class Alloc = alloc>
class set {
public:
	typedef Key key_type;
	typedef Key value_type;
	//因为set key和value用同一个
	typedef Compare key_compare;
	typedef Compare value_compare;
private:
	//红黑树
	typedef rb_tree<key_type, value_type, _Identity<value_type>, key_compare, Alloc> rep_type;
	rep_type t;
public:
	//因为set的类型不能改变其值所以都是const
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator  iterator;
	typedef typename rep_type::const_iterator  const_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	explicit set(const Compare& comp = Compare()):t(comp) {}
	set(std::initializer_list<Key> list):t() {
		auto iter = list.begin();
		for (; iter != list.end(); ++iter) {
			t.insert_unique(*iter);
		}
	}
	key_compare key_comp() const { return t.key_comp(); }
	value_compare value_comp() const { return t.key_comp(); }
	iterator begin() { return t.begin(); }
	const_iterator begin() const { return t.begin(); }
	iterator end() { return t.end(); }
	const_iterator end() const { return t.end(); }
	//反向迭代器暂无
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }
	size_type max_size() const { return t.max_size(); }
	
	pair<iterator, bool> insert(const value_type& x) {
		pair<typename rep_type::iterator, bool > p = t.insert_unique(x);
		return pair<iterator, bool>(p.first, p.second);
	}
	void erase(iterator p) {
		return t.erase(p);
	}
	size_type erase(const key_type& x) {
		return t.erase(x);
	}
	void clear() { t.clear(); }
	iterator find(const key_type& x) const { return t.find(x); }
};

_MINI_END

#endif // !_MINI_SET_
