#pragma once
#ifndef _MINI_ITER_
typedef __int64          ptrdiff_t;
#define _MINI_BEGIN namespace mini{
#define _MINI_END }
////////////////////////////////////////////////////////////////////////////////  
	// STL迭代器定义  
	////////////////////////////////////////////////////////////////////////////////  
	// Input Iterator               read only  
	// Output Iterator              write only  
	// Forward Iterator             允许"写入型"算法在其指向区间进行操作  
	// Bidirectional Iterator       提供双向访问能力  
	// Random Access Iterator       支持原生指针具有的全部能力  
	////////////////////////////////////////////////////////////////////////////////  
_MINI_BEGIN
struct false_type {};
struct true_type {};

struct input_iterator_tag {};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

//为了避免写迭代器的时候出现问题，开发迭代器的时候最好继承std::iterator
template<class Category,class T,class Distance = ptrdiff_t,class Pointer = T*,class Reference = T&>
//类型
struct iterator {
	typedef Category iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;
};
//萃取器
//防止跟std冲突 iterator_traits 就不加s了
template<class Iterator>
struct iterator_trait
{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
};
template<class T>
struct iterator_trait<T*>
{
	typedef random_access_iterator_tag  iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;
};

template<class T>
struct iterator_trait<const T*>
{
	typedef random_access_iterator_tag  iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;
};

template<class Iterator>
inline typename iterator_trait<Iterator>::iterator_category
	iterator_category(const Iterator&) {
	typedef typename iterator_trait<Iterator>::iterator_category category;
	return category();
}
//这种方式可以很快确定某个迭代器的distance type
template<class Iterator>
inline typename iterator_trait<Iterator>::difference_type*
	distance_type(const Iterator&) {
	return static_cast<typename iterator_trait<Iterator>::difference_type * >(0);
}
//这种方式能很快的确定某个迭代器的value type
template<class Iterator>
inline typename iterator_trait<Iterator>::value_type*
	value_type(const Iterator&) {
	return static_cast<typename iterator_trait<Iterator>::value_type* >(0);
}

template <class InputIterator>
inline typename iterator_trait<InputIterator>::template difference_type
	__distance(InputIterator first, InputIterator last, input_iterator_tag) {
	iterator_trait<InputIterator>::template difference_type n = 0;
	while (first != last)
	{
		++first; ++n;
	}
	return n;
}
template<class RandomAccessIterator>
inline typename iterator_trait<RandomAccessIterator>::template difference_type
	__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	return last - first;
}
template<class InputIterator>
inline typename iterator_trait<InputIterator>::template difference_type
	distance(InputIterator first,InputIterator last) {
	typedef typename iterator_trait<InputIterator>::iterator_category category;
	return __distance(first, last, category());
}
//advance函数
template<class InputIterator,class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag)
{
	//向前移动n格
	while (n--) ++i;
}
template<class BidectionalIterator,class Distance>
inline void __advance(BidectionalIterator& i, Distance n, bidirectional_iterator_tag)
{
	if (n >= 0) {
		while (n--) ++i;
	}
	else
	{
		while (n++) --i;
	}
}
template<class RandomAccessIterator,class Distance>
inline void __advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag)
{
	i += n;
}
template <class InputIterator,class Distance>
inline void advance(InputIterator& i, Distance n)
{
	__advance(i, n, iterator_category(i));
}


//反转迭代器的定义
template<class Iterator>
struct reverse_iterator {
protected:
	Iterator cur;
public:
	typedef typename iterator_trait<Iterator>::iterator_category iterator_category;
	typedef typename iterator_trait<Iterator>::value_type value_type;
	typedef typename iterator_trait<Iterator>::difference_type difference_type;
	typedef typename iterator_trait<Iterator>::pointer pointer;
	typedef typename iterator_trait<Iterator>::reference reference;

	typedef Iterator iterator_type;
	typedef reverse_iterator<Iterator> self;

	iterator_type base() const { return cur; }
	reverse_iterator(){}
	explicit reverse_iterator(iterator_type x) : cur(x) {}
	reverse_iterator(const self& x) : cur(x.cur){};

	reference operator*() const { Iterator tmp = cur; return *--tmp; }
	pointer operator->() const { &(operator*()); }
	bool operator==(const self& x) const { return base() == x.base(); }
	bool operator!=(const self& x) const { return base() != x.base(); }
	self& operator++() {
		--cur;
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		--cur;
		return tmp;
	}
	self& operator--() {
		++cur;
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		++cur;
		return tmp;
	}
	self operator+(difference_type n) const {
		return self(cur - n);
	}
	self& operator+=(difference_type n) {
		cur -= n;
		return *this;
	}
	self operator-(difference_type n) const {
		return self(cur + n);
	}
	self& operator-=(difference_type n) {
		cur += n;
		return *this;
	}
	reference operator[](difference_type n) const { return *(*this + n); }
};

template <class _Iterator>
inline bool operator==(const reverse_iterator<_Iterator>& __x,
	const reverse_iterator<_Iterator>& __y) {
	return __x.base() == __y.base();
}
template <class _Iterator>
inline bool operator<(const reverse_iterator<_Iterator>& __x,
	const reverse_iterator<_Iterator>& __y) {
	return __y.base() < __x.base();
}
template <class _Iterator>
inline bool operator!=(const reverse_iterator<_Iterator>& __x,
	const reverse_iterator<_Iterator>& __y) {
	return !(__x == __y);
}
template <class _Iterator>
inline bool operator>(const reverse_iterator<_Iterator>& __x,
	const reverse_iterator<_Iterator>& __y) {
	return __y < __x;
}
template <class _Iterator>
inline bool operator<=(const reverse_iterator<_Iterator>& __x,
	const reverse_iterator<_Iterator>& __y) {
	return !(__y < __x);
}
template <class _Iterator>
inline bool operator>=(const reverse_iterator<_Iterator>& __x,
	const reverse_iterator<_Iterator>& __y) {
	return !(__x < __y);
}
template <class _Iterator>
inline typename reverse_iterator<_Iterator>::difference_type
operator-(const reverse_iterator<_Iterator>& __x,
	const reverse_iterator<_Iterator>& __y) {
	return __y.base() - __x.base();
}
template <class _Iterator>
inline reverse_iterator<_Iterator>
operator+(typename reverse_iterator<_Iterator>::difference_type __n,
	const reverse_iterator<_Iterator>& __x) {
	return reverse_iterator<_Iterator>(__x.base() - __n);
}

template <class _T> 
struct Is_Integer {
	typedef false_type value;
};
template <> 
struct Is_Integer<bool> {
	typedef true_type value;
};
template <>
struct Is_Integer<char> {
	typedef true_type value;
};
template <>
struct Is_Integer<signed char> {
	typedef true_type value;
};
template <>
struct Is_Integer<unsigned char> {
	typedef true_type value;
};
template <>
struct Is_Integer<wchar_t> {
	typedef true_type value;
}; 
template <>
struct Is_Integer<short> {
	typedef true_type value;
};
template <>
struct Is_Integer<unsigned short> {
	typedef true_type value;
};
template <>
struct Is_Integer<int> {
	typedef true_type value;
};
template <>
struct Is_Integer<unsigned int> {
	typedef true_type value;
};
template <>
struct Is_Integer<long> {
	typedef true_type value;
};
template <>
struct Is_Integer<unsigned long> {
	typedef true_type value;
};
template <>
struct Is_Integer<long long> {
	typedef true_type value;
};
template <>
struct Is_Integer<unsigned long long> {
	typedef true_type value;
};
_MINI_END
#endif // !_MINI_ITER_
