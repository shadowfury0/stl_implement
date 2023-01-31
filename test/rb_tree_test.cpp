#include <iostream>
#include "../container/associative container/tree.h"

using namespace std;
template<class T>
void print(mini::rb_tree<T,T,mini::identity<T>,mini::less<T> >& itr) {
	if (itr.empty()) {
		cout << "   empty " << endl;
		return;
	}
	typename mini::rb_tree<T, T, mini::identity<T>, mini::less<T> >::iterator
	ite1 = itr.begin();
	typename  mini::rb_tree<T, T, mini::identity<T>, mini::less<T> >::iterator
	ite2 = itr.end();
	mini::rb_tree_base_iterator rbtite;
	//��ӡ���
	for (; ite1 != ite2; ++ite1) {
		rbtite = mini::rb_tree_base_iterator(ite1);
		cout << *ite1 << " ( " << rbtite.node->color << " ) ";
	}
	cout << endl;
}
//������Ļ���������ʵ���Ѿ��������
int main_rb_tree() {
	mini::rb_tree<int, int, mini::identity<int>, mini::less<int> > itree;
	
	cout << itree.size() << endl;

	itree.insert_unique(10);
	itree.insert_unique(7);
	itree.insert_unique(8);

	//�Ժ�ɾ���Ľڵ�
	auto d1	=itree.insert_unique(15);
	itree.insert_unique(5);
	auto d2 =itree.insert_unique(6);
	itree.insert_unique(11);
	auto d3 =itree.insert_unique(13);
	itree.insert_unique(12);

	cout << itree.size() << endl;

	print(itree);
	itree.erase(d1.first);
	itree.erase(d2.first);
	itree.erase(d3.first);
	//itree.clear();
	print(itree);
	return 0;
}
