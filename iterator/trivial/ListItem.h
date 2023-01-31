#pragma once
#ifndef __MINI_LIST_ITEM__
#define __MINI_LIST_ITEM__
template<typename T>
class ListItem
{
public:
	T value() const { return _value; }
	ListItem* next() const { return _next; }
private:
	T _value;
	ListItem* _next;
};
template<typename T>
class List{
public:
	void insert_front(T value){};
	void insert_end(T value){};
	
private:
	ListItem<T>* _end;
	ListItem<T>* _front;
	long _size;
};


template<class Item>
struct ListIter {
	Item* ptr;
	ListIter(Item* p = 0):ptr(p){}
	Item& operator*() const { return *ptr; }
	Item* operator->() const { return ptr; }
	ListIter& operator++() { ptr = ptr->next(); return *this; }
	ListIter operator++(int) { ListIter tmp = *this; ++* this; return tmp; }
	bool operator==(const ListIter& i) const { return ptr == i.ptr; }
	bool operator!=(const ListIter& i)const { return ptr != i.ptr; }
};	
#endif // !__MINI_LIST_ITEM__
