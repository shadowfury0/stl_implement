#include <iostream>

#include "../container/sequence container/list.h"
using namespace std;
template<class T>
void list_print1(const mini::list<T>& t) {
	auto val = t.begin();
	int si = t.size();
	int i = 1;
	while (si--)
	{
		cout << " Number "<<  i << " : Value " << *val++<< "	";
		i++;
	}
	cout << endl;
}
// a predicate implemented as a function:
bool single_digit(const int& value) { return (value < 10); }

// a predicate implemented as a class:
struct is_odd {
	bool operator() (const int& value) { return (value % 2) == 1; }
};
int main_list() {
	// constructors used in the same order as described above:
	mini::list<int> first;                                // empty list of ints
	mini::list<int> second(4, 100);                       // four ints with value 100
	mini::list<int> third(second.begin(), second.end());  // iterating through second
	mini::list<int> fourth(third);                       // a copy of third
	// the iterator constructor can also be used to construct from arrays:
	int myints[] = { 16,2,77,29 };
	mini::list<int> fifth(myints, myints + sizeof(myints) / sizeof(int));
	//列表初始化
	mini::list<int> sixth = {1,2,4,5,6};
	mini::list<int> seventh = sixth;
	// the iterator constructor can also be used to construct from arrays:

	list_print1(first);
	list_print1(second);
	list_print1(third);
	list_print1(fourth);
	list_print1(fifth);
	list_print1(sixth);
	list_print1(seventh);
	cout << "-------------------------------------------------------------------------" << endl;
	//函数测试
	mini::list<int> test{1,2,3,4,5};
	list_print1(test);
	cout << *test.begin() << endl;
	cout << test.rend().base().node->data << endl;
	cout << *test.end() << endl;
	cout << test.rbegin().base().node->data << endl;
	cout << test.empty() << endl;
	cout << test.size() << endl;
	test.insert(test.begin(),1);
	list_print1(test);
	test.erase(test.begin());
	list_print1(test);
	std::cout << "front() is now " << test.front() << '\n';
	std::cout << "back() is now "  << test.back() << '\n';
	cout << "-------------------------------------------------------------------------" << endl;
	mini::list<double> s1, s2;

	s1.push_back(3.1);
	s1.push_back(2.2);
	s1.push_back(2.9);

	s2.push_back(3.7);
	s2.push_back(7.1);
	s2.push_back(1.4);

	s1.sort();
	s2.sort();
	list_print1(s1);
	list_print1(s2);
	s1.merge(s2);
	list_print1(s1);
	list_print1(s2);//null
	//这里transfer 和 swap 都不用测试了因为merge里面包含了这一部分的测试
	s1.remove(1.4);
	list_print1(s1);
	cout << "-------------------------------------------------------------------------" << endl;

	int array[] = { 15,36,7,17,20,39,4,1 };
	mini::list<int> mylist(array, array + 8);   // 15 36 7 17 20 39 4 1

	mylist.remove_if(single_digit);           // 15 36 17 20 39
	list_print1(mylist);
	mylist.remove_if(is_odd());				  // 36 20
	list_print1(mylist);
	cout << "-------------------------------------------------------------------------" << endl;
	//剩下的就没有什么特别重要的函数了可以自行测试
	return 0;
}