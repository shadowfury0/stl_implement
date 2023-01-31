#pragma once
#ifndef _MINI_TREE_
#define _MINI_TREE_
#include "dependence.h"
#include "pair.h"

_MINI_BEGIN

typedef bool rb_tree_color_type;
const rb_tree_color_type  tree_red = false;
const rb_tree_color_type  tree_black = true;

struct rb_tree_node_base{
	typedef rb_tree_color_type color_type;
	typedef rb_tree_node_base* base_ptr;

	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	static base_ptr minimum(base_ptr x)
	{
		while ( x->left != 0)
		{
			x = x->left;
		}//一直左走找到最小值
		return x;
	}
	static base_ptr maximum(base_ptr x) {
		while (x->right != 0)
		{
			x = x->right;
		}
		return x;
	}
};

template<class Value>
struct rb_tree_node : public rb_tree_node_base {
	typedef rb_tree_node<Value>* link_type;
	Value value_field;//值大小
};

struct rb_tree_base_iterator
{
	typedef rb_tree_node_base::base_ptr base_ptr;
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	base_ptr node;
	
	void increment() {
		//有没有右节点
		if (node->right != 0) {
			node = node->right;
			while (node->left != 0)
			{
				node = node->left;
			}
		}
		else {
			//没有右节点就往上走找到父节点
			base_ptr y = node->parent;
			while (node == y->right)
			{
				node = y;
				y = y->parent;
			}
			//判断是不是跟节点
			if (node->right != y) {
				node = y;
			}
		}
	}
	void decrement() {
		//判断是不是根节点
		if (node->color == tree_red && node->parent->parent == node)
			node = node->right;
		else if (node->left != 0) {
			//有左子节点
			base_ptr y = node->left;
			while (y->right != 0) {
				y = y->right;
			}
			node = y;
		}
		else {
			//无左子节点 往上找父节点
			base_ptr y = node->parent;
			while (node == y->left) { 
				node = y;
				y = y->parent;
			}
			node = y;
		}
	}
};
//全局rb_tree迭代器比较器
inline bool operator==(const rb_tree_base_iterator& x,const rb_tree_base_iterator& y) {
	return x.node == y.node;
}
inline bool operator!=(const rb_tree_base_iterator& x, const rb_tree_base_iterator& y) {
	return x.node != y.node;
}

template <class Value,class Ref,class Ptr>
struct rb_tree_iterator : public rb_tree_base_iterator {
	typedef Value value_type;
	typedef Ref   reference;
	typedef Ptr   pointer;
	typedef rb_tree_iterator<Value,Value&,Value*>   iterator;
	typedef rb_tree_iterator<Value,const Value&,const Value*>   const_iterator;
	typedef rb_tree_iterator<Value, Ref, Ptr>  self;
	typedef rb_tree_node<Value>*  link_type;

	rb_tree_iterator() { }
	rb_tree_iterator(link_type x) { node = x; }
	rb_tree_iterator(const iterator& it) { node = it.node; }

	reference operator*() const { return link_type(node)->value_field; }
	pointer operator->() const { return &(operator*()); }

	self& operator++() { increment(); return *this; }
	self operator++(int) { self tmp = *this; increment(); return tmp; }
	self& operator--() { decrement(); return *this; }
	self operator--(int) { self tmp = *this; decrement(); return tmp; }
	bool operator==(const iterator& x) { return this->node == x.node; }
	bool operator!=(const iterator& x) { return this->node != x.node; }
};

inline void rb_tree_rotate_left(rb_tree_node_base* x, rb_tree_node_base*& root) {
	rb_tree_node_base* y = x->right;
	x->right = y->left;
	if (y->left != 0)
		y->left->parent = x;
	y->parent = x->parent;

	//让y替代x的位置
	if (x == root)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

inline void rb_tree_rotate_right(rb_tree_node_base* x, rb_tree_node_base*& root) {
	rb_tree_node_base* y = x->left;
	x->left = y->right;
	if (y->right != 0)
		y->right->parent = x;
	y->parent = x->parent;

	if (x == root)
		root = y;
	else if (x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;
}

inline void rb_tree_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root) {
	x->color = tree_red;//新节点必须为红色
	while (x != root && x->parent->color == tree_red )//父节点为红
	{
		//判断左节点
		if (x->parent == x->parent->parent->left) {
			//状况4
			rb_tree_node_base* y = x->parent->parent->right;
			if (y && y->color == tree_red) {
				x->parent->color = tree_black;
				y->color = tree_black;
				x->parent->parent->color = tree_red;
				x = x->parent->parent;
			}
			else {
				//状况2
				if (x == x->parent->right) {
					x = x->parent;
					rb_tree_rotate_left(x, root);
				}
				//状况1
				x->parent->color = tree_black;
				x->parent->parent->color = tree_red;
				rb_tree_rotate_right(x->parent->parent, root);
			}
		}//右节点
		else {
			rb_tree_node_base* y = x->parent->parent->left;
			if (y && y->color == tree_red) {
				x->parent->color = tree_black;
				y->color = tree_black;
				x->parent->parent->color = tree_red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->left) {
					x = x->parent;
					rb_tree_rotate_right(x, root);
				}
				x->parent->color = tree_black;
				x->parent->parent->color = tree_red;
				rb_tree_rotate_left(x->parent->parent, root);
			}
		}
	}
	root->color = tree_black;//根节点永远为黑
}

//平衡整个红黑树，在删除一个节点后
inline rb_tree_node_base* rb_tree_rebalance_for_erase(rb_tree_node_base* z,
	rb_tree_node_base*& root,rb_tree_node_base*& leftmost,rb_tree_node_base*& rightmost) {
	rb_tree_node_base* y = z;
	rb_tree_node_base* x = 0;
	rb_tree_node_base* x_parent = 0;
	//当前节点最多有一个非空的子节点
	if (y->left == 0) {
		x = y->right;//可能为空
	}
	else {
		if (y->right == 0) {//当前节点确定有一个非空子节点
			x = y->left;  // x 不为空
		}
		else
		{//当前节点有两个非空子节点，将 y 赋值给 z ，x可能为空
			y = y->right;
			while (y->left != 0)
				y = y->left;
			x = y->right;
		}
	}
	if (y != z) {
		z->left->parent = y;
		y->left = z->left;
		if (y != z->right) {
			x_parent = y->parent;
			if (x) x->parent = y->parent;
			y->parent->left = x;
			y->right = z->right;
			z->right->parent = y;
		}
		else {
			x_parent = y;
			if (root == z)
				root = y;
			else if (z->parent->left == z)
				z->parent->left = y;
			else
				z->parent->right = y;
			y->parent = z->parent;
			std::swap(y->color, z->color);
			y = z;
			//y现在指向的节点已经被删除了
		}
	}
	else {
		x_parent = y->parent;
		if (x) x->parent = y->parent;
		if (root == z)
			root = x;
		else
			if (z->parent->left == z)
				z->parent->left = x;
			else
				z->parent->right = x;
		if (leftmost == z)
			if (z->right == 0) //当前节点的左节点也必须为空
				leftmost = z->parent;
		//让最左节点判断等不等于header
			else
				leftmost = rb_tree_node_base::minimum(x);
		if (rightmost == z)
			if (z->left == 0)
				rightmost = z->parent;
		//让最右节点判断等不等于header
			else
				rightmost = rb_tree_node_base::maximum(x);
	}
	if (y->color != tree_red) {
		while (x != root && (x == 0 || x->color == tree_black))
			if (x == x_parent->left) {
				rb_tree_node_base* w = x_parent->right;
				if (w->color == tree_red) {
					w->color = tree_black;
					x_parent->color = tree_red;
					rb_tree_rotate_left(x_parent, root);
					w = x_parent->right;
				}
				if ((w->left == 0 ||
					w->left->color == tree_black) &&
					(w->right == 0 ||
						w->right->color == tree_black)) {
					w->color = tree_red;
					x = x->parent;
					x_parent = x_parent->parent;
				}
				else {
					if (w->right == 0 ||
						w->right->color == tree_black) {
						if (w->left) w->left->color = tree_black;
						w->color = tree_red;
						rb_tree_rotate_right(w, root);
						w = x_parent->right;
					}
					w->color = x_parent->color;
					x_parent->color = tree_black;
					if (w->right) w->right->color = tree_black;
					rb_tree_rotate_left(x_parent, root);
					break;
				}
			}
			else {
				rb_tree_node_base* w = x_parent->left;
				if (w->color == tree_red) {
					w->color = tree_black;
					x_parent->color = tree_red;
					rb_tree_rotate_right(x_parent, root);
					w = x_parent->left;
				}
				if ((w->right == 0 ||
					w->right->color == tree_black) &&
					(w->left == 0 ||
						w->left->color == tree_black)) {
					w->color = tree_red;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else {
					if (w->left == 0 ||
						w->left->color == tree_black) {
						if (w->right) w->right->color = tree_black;
						w->color = tree_red;
						rb_tree_rotate_right(w, root);
						w = x_parent->left;
					}
					w->color = x_parent->color;
					x_parent->color = tree_black;
					if (w->left) w->left->color = tree_black;
					rb_tree_rotate_right(x_parent, root);
					break;
				}
			}
		if (x) x->color = tree_black;
	}
	return y;
}
template<class Key,class Value,class KeyOfValue,class Compare,class Alloc = alloc>
class rb_tree {
protected:
	typedef void* void_pointer;
	typedef rb_tree_node_base*  base_ptr;
	typedef rb_tree_node<Value> rb_tree_node;
	typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocater;
	typedef rb_tree_color_type color_type;
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef rb_tree_node* link_type;
	typedef size_t   size_type;
	typedef ptrdiff_t difference_type;

protected:
	link_type get_node() { return rb_tree_node_allocater::allocate(); }
	//释放
	void put_node(link_type p) { rb_tree_node_allocater::deallocate(p); }

	link_type create_node(const value_type& x) {
		link_type tmp = get_node();
		try {
			construct(&tmp->value_field, x);
		}
		catch (...) {
			put_node(tmp);
		}
		return tmp;
	}
	link_type clone_node(link_type x) {
		link_type tmp = create_node(x->value_field);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}
	void destroy_node(link_type p) {
		destroy(&p->value_field);
		put_node(p);
	}

protected:
	size_type node_count;//记录节点数量
	link_type header;//根节点上面的一个节点
	Compare key_compare;

	//以下三个函数用来方便取得header的成员
	link_type& root() const { return (link_type&)header->parent; }
	link_type& leftmost() const { return (link_type&)header->left; }
	link_type& rightmost() const { return (link_type&)header->right;}
	//获取x节点
	static link_type& left(link_type x) { return (link_type&)(x->left); }
	static link_type& right(link_type x) { return (link_type&)(x->right); }
	static link_type& parent(link_type x) { return (link_type&)(x->parent); }
	static reference value(link_type x) { return x->value_field; }
	static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
	static color_type& color(link_type x) { return (color_type&)(x->color); }
	//类型不同
	static link_type& left(base_ptr x) { return (link_type&)(x->left); }
	static link_type& right(base_ptr x) { return (link_type&)(x->right); }
	static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
	static reference value(base_ptr x) { return ((link_type)x)->value_field; }
	static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
	static color_type& color(base_ptr x) { return (color_type&)(link_type(x)->color); }


	//求最大值
	static link_type minimum(link_type x) {
		return (link_type)rb_tree_node_base::minimum(x);
	}
	static link_type maximum(link_type x) {
		return (link_type)rb_tree_node_base::maximum(x);
	}
public:
	typedef rb_tree_iterator<value_type, reference, pointer> iterator;
	typedef rb_tree_iterator<value_type, const reference,const pointer> const_iterator;
	//逆向迭代器暂无定义
private:
	iterator _insert(base_ptr x_, base_ptr y_, const value_type& v);
	void _erase(link_type x);
	link_type _copy(link_type x, link_type p);
	
	//初始化根节点
	void init() {
		header = get_node();
		color(header) = tree_red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}
public:
	rb_tree(const Compare& comp = Compare()) :node_count(0), key_compare(comp) { init(); }
	rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x):node_count(0),key_compare(x.key_compare) {
		if (x.root() == 0) {
			init();
		}
		else {
			color(header) = tree_red;
			root() = _copy(x.root(), header);
			leftmost() = minimum(root());
			rightmost() = maximum(root());
		}
		node_count = x.node_count;
	}
	~rb_tree() { clear(); put_node(header); }

	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

	Compare key_comp() const { return key_compare; }
	iterator begin() { return leftmost(); }
	iterator end()   { return header; }
	bool empty() const { return node_count == 0; }
	size_type  size() const { return node_count; }
	size_type  max_size() const { return size_type(-1); }
	
	//找到不小于k的一个元素包括k
	iterator lower_bound(const Key& k);
	const_iterator lower_bound(const Key& k) const;
	//找到元素大于k的不包括k
	iterator upper_bound(const Key& k);
	const_iterator upper_bound(const Key& k) const;
	//返回一个区间，这个区间的元素都等于这个值
	pair<iterator, iterator> equal_range(const Key& k) {
		return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
	}
	pair<const_iterator,const_iterator> equal_range(const Key& k) const {
		return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
	}

	iterator find(const Key& k);
	void clear() {
		if (node_count != 0) {
			_erase(root());
			leftmost() = header;
			root() = 0;
			rightmost() = header;
			node_count = 0;
		}
	}

	pair<iterator, bool> insert_unique(const value_type& v);
	iterator insert_equal(const value_type& v);
	//移除某个节点
	void erase(iterator position);
	size_type erase(const Key& x);
	void erase(iterator first, iterator last) {
		if (first == begin() && last == end())
			clear();
		else
			while (first != last) erase(first++);
	}
	void erase(const Key* first, const Key* last) {
		while (first != last) erase(*first++);
	}
};
//x当前插入位置,y父节点位置
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(base_ptr x_, base_ptr y_, 
	const value_type& v)
{
	link_type x = (link_type)x_;
	link_type y = (link_type)y_;//父节点
	link_type z;
	if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
		z = create_node(v);
		left(y) = z;
		//如果树没有节点
		if (y == header) {
			root() = z;
			rightmost() = z;
		}//如果插入节点为最左节点
		else if (y == leftmost()) {
			leftmost() = z;
		}
	}//插入右节点
	else {
		z = create_node(v);
		right(y) = z;
		if (y == rightmost())
			rightmost() = z;
	}
	parent(z) = y;
	left(z) = 0;
	right(z) = 0;

	rb_tree_rebalance(z, header->parent);
	++node_count;
	return iterator(z);
}
//清除节点，不进行平衡
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase(link_type x)
{
	while (x != 0) {
		_erase(right(x));//找到最右节点
		link_type y = left(x);//没有右节点，找左节点
		destroy_node(x);//删除当前节点
		x = y;//再往左节点走
	}
}
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
//当前节点x，父节点p
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_copy(link_type x, link_type p)
{
	link_type top = clone_node(x);
	top->parent = p;
	try {
		if (x->right)
			//递归拷贝右边节点
			top->right = _copy(right(x), top);
		//父节点等于拷贝构造的当前节点
		p = top;
		//递归找左边节点
		x = left(x);
		//拷贝左边的节点
		while (x != 0)
		{
			link_type y = clone_node(x);
			p->left = y;
			y->parent = p;
			if (x->right) {
				//再往右拷贝右边节点
				y->right = _copy(right(x), y);
			}
			p = y;
			x = left(x);
		}
	}
	catch (...) {
		erase(top);
	}
	return top;
}
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k)
{
	link_type y = header;
	link_type x = root();
	while (x != 0)
	{
		if (!key_compare(key(x), k))
			y = x, x = left(x);
		else
			x = right(x);
	}

	return iterator(y);
}
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) const
{
	link_type y = header;
	link_type x = root();
	while (x != 0)
	{
		if (!key_compare(key(x), k))
			y = x, x = left(x);
		else
			x = right(x);
	}
	return const_iterator(y);
}
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k)
{
	link_type y = header;
	link_type x = root();

	while ( x != 0 )
	{
		if (key_compare(k, key(x)))
			y = x, x = left(x);
		else
			x = right(x);
	}
	return iterator(y);
}
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) const
{
	link_type y = header;
	link_type x = root();

	while (x != 0)
	{
		if (key_compare(k, key(x)))
			y = x, x = left(x);
		else
			x = right(x);
	}
	return const_iterator(y);
}
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k)
{
	link_type y = header;
	link_type x = root();
	//递归查找
	while ( x != 0)
	{
		if (!key_compare(key(x), k))
			y = x, x = left(x);
		else
			x = right(x);
	}
	iterator j = iterator(y);
	return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool> 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v)
{
	link_type y = header;
	link_type x = root();
	bool comp = true;
	//判断是否为根节点
	while (x != 0)
	{
		y = x;
		comp = key_compare(KeyOfValue()(v), key(x));
		x = comp ? left(x) : right(x);
	}
	iterator j = iterator(y);
	if (comp) {
		if (j == begin()) {
			return pair<iterator, bool>(_insert(x, y, v), true);
		}
		else {
			--j;
		}
	}
	if (key_compare(key(j.node), KeyOfValue()(v)))
		return pair<iterator, bool>(_insert(x, y, v), true);
	return pair<iterator,bool>(j,false);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename  rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& v)
{
	link_type y = header;
	link_type x = root();

	while (x != 0)
	{
		y = x;
		//比较
		x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
	}
	return _insert(x,y,v);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position)
{
	link_type y = (link_type)rb_tree_rebalance_for_erase(position.node, header->parent,
		header->left, header->right);
	destroy_node(y);
	--node_count;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x)
{
	pair<iterator, iterator> p = equal_range(x);
	size_type n = 0;
	distance(p.first, p.second);
	erase(p.first, p.second);
	return n;
}

_MINI_END

#endif // !_MINI_TREE_


