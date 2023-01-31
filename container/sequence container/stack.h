#pragma once
#ifndef _MINI_STACK_
#define _MINI_STACK_

#include "deque.h"

_MINI_BEGIN
template<class T,class Seq =deque<T>>
class stack {
	friend bool operator== (const stack&,const stack&);
	friend bool operator!= (const stack&, const stack&);
	friend bool operator< (const stack&, const stack&);
	friend bool operator> (const stack&, const stack&);
	friend bool operator<= (const stack&, const stack&);
	friend bool operator>= (const stack&, const stack&);
public:
	typedef typename Seq::value_type value_type;
	typedef typename Seq::size_type  size_type;
	typedef typename Seq::reference  reference;
	typedef typename Seq::const_reference  const_reference;
protected:
	Seq c;
public:
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }

	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_back(); }
};
template<class T,class Seq>
bool operator== (const stack<T,Seq>& x,const stack<T, Seq>& y) {
	return x.c == x.c;
}
template<class T, class Seq>
bool operator!=(const stack<T, Seq>& x, const stack<T, Seq>& y)
{
	return x.c != y.c;
}
template<class T, class Seq>
bool operator<(const stack<T, Seq>& x,const stack<T, Seq>& y)
{
	return x.c < y.c;
}
template<class T, class Seq>
bool operator>(const stack<T, Seq>& x, const stack<T, Seq>& y)
{
	return x.c > y.c;
}
template<class T, class Seq>
bool operator<=(const stack<T, Seq>& x, const stack<T, Seq>& y)
{
	return x.c <= y.c;
}
template<class T, class Seq>
bool operator>=(const stack<T, Seq>& x, const stack<T, Seq>& y)
{
	return x.c >= y.c;
}
_MINI_END
#endif // !_MINI_STACK_
