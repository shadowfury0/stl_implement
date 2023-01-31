#pragma once
#ifndef _MINI_QUEUE_
#define _MINI_QUEUE_

#include "heap.h"
#include "vector.h"
#include "deque.h"

_MINI_BEGIN
template <class T,class Seq = deque<T>>
class queue {
	friend bool operator== (const queue&, const queue&);
	friend bool operator!= (const queue&, const queue&);
	friend bool operator< (const queue&, const queue&);
	friend bool operator> (const queue&, const queue&);
	friend bool operator<= (const queue&, const queue&);
	friend bool operator>= (const queue&, const queue&);
public:
	typedef typename Seq::value_type value_type;
	typedef typename Seq::size_type  size_type;
	typedef typename Seq::reference  reference;
	typedef typename Seq::const_reference  const_reference;
protected:
	Seq c;
public:
	queue() :c() {}
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_front(); }
};
template<class T, class Seq>
bool operator== (const queue<T, Seq>& x, const queue<T, Seq>& y) {
	return x.c == x.c;
}
template<class T, class Seq>
bool operator!=(const queue<T, Seq>& x, const queue<T, Seq>& y)
{
	return x.c != y.c;
}
template<class T, class Seq>
bool operator<(const queue<T, Seq>& x, const queue<T, Seq>& y)
{
	return x.c < y.c;
}
template<class T, class Seq>
bool operator>(const queue<T, Seq>& x, const queue<T, Seq>& y)
{
	return x.c > y.c;
}
template<class T, class Seq>
bool operator<=(const queue<T, Seq>& x, const queue<T, Seq>& y)
{
	return x.c <= y.c;
}
template<class T, class Seq>
bool operator>=(const queue<T, Seq>& x, const queue<T, Seq>& y)
{
	return x.c >= y.c;
}

//Ä¬ÈÏ½µÐò
template<class T,class Seq = vector<T>,class Com = less<typename Seq::value_type> >
class priority_queue {
public:
	typedef typename Seq::value_type value_type;
	typedef typename Seq::size_type  size_type;
	typedef typename Seq::reference  reference;
	typedef typename Seq::const_reference  const_reference;
protected:
	Seq c;
	Com com;
public:
	priority_queue() :c() {}
	explicit priority_queue(const Com& x) :c(), com(x) {}
	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last) : c(first, last) { 
		make_heap(c.begin(),c.end(),com); 
	}
	template<class InputIterator>
	priority_queue(InputIterator first, InputIterator last,const Com& x) : c(first, last),com(x) {
		make_heap(c.begin(), c.end(),x);
	}
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	const_reference top() const { return c.front(); }
	void push(const value_type& x) {
		try {
			c.push_back(x);
			push_heap(c.begin(), c.end(), com);
		}
		catch (...) {
			c.clear();
		}
	}
	void pop() {
		try {
			pop_heap(c.begin(), c.end(), com);
			c.pop_back();
		}
		catch (...) {
			c.clear();
		}
	}
};
_MINI_END
#endif // !_MINI_QUEUE_
