#include <iostream>

#include "../container/sequence container/deque.h"
using namespace std;
template <class T>
void deque_print(const mini::deque<T>& de) {
	for (auto it = de.begin(); it != de.end(); ++it)
		std::cout << ' ' << *it;
	cout <<endl;
}
int main_deque() {
	// constructors used in the same order as described above:
	mini::deque<int> first;                                // empty deque of ints
	mini::deque<int> second(4, 100);                       // four ints with value 100
	mini::deque<int> third(second.begin(), second.end());  // iterating through second
	mini::deque<int> fourth(third);                       // a copy of third
	// the iterator constructor can be used to copy arrays:
	//!!!
	int myints[] = { 16,2,77,29 };
	mini::deque<int> fifth(myints, myints+4);
	//列表初始化
	mini::deque<int> sixth{ 1,2,3,4 };
	mini::deque<int> seventh = sixth;
	deque_print(first);
	deque_print(second);
	deque_print(third);
	deque_print(fourth);
	deque_print(fifth);
	deque_print(sixth);
	deque_print(seventh);

	cout << "-------------------------------------------------------------------------" << endl;
	cout << "mydeque contains:";
	for (unsigned i = 0; i < fifth.size() - 1; i++)
		cout << ' ' << fifth.at(i);
	cout << endl;

	cout << fifth[0] << endl;
	cout << *fifth.begin() << endl;
	cout << *fifth.end() << endl;
	cout << fifth.front() << endl;
	cout << fifth.back() << endl;
	cout << *fifth.rbegin() << endl;
	cout << *fifth.rend() << endl;
	cout << fifth.size() << endl;

	cout << "-------------------------------------------------------------------------" << endl;
	//push pop insert clear在源码验证过
	//这里就展示insert
	mini::deque<int> my;
	// set some initial values:
	for (int i = 1; i < 6; i++) my.push_back(i); // 1 2 3 4 5
	mini::deque<int>::iterator it = my.begin();
	++it;
	it = my.insert(it, 10);                  // 1 10 2 3 4 5
	// "it" now points to the newly inserted 10
	my.insert(it, 2, 20);

	deque_print(my);
	cout << "-------------------------------------------------------------------------" << endl;
	mini::deque<int> er;

	// set some values (from 1 to 10)
	for (int i = 1; i <= 10; i++) er.push_back(i);
	er.erase(er.begin() + 5,er.end());
	deque_print(er);
	cout << "-------------------------------------------------------------------------" << endl;

	mini::deque<int> mydeque;
	for (int i = 1; i < 10; ++i) mydeque.push_back(i);
	mydeque.resize(5);
	mydeque.resize(8,10);
	mydeque.resize(12);
	cout << "mydeque contains:";
	deque_print(mydeque);
	return 0;
}