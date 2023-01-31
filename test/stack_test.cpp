#include <iostream>

#include "../container/sequence container/stack.h"
#include "../container/sequence container/list.h"
#include "../container/sequence container/vector.h"
using namespace std;
template <typename T>
void stack_deque_print(mini::stack<T>& s)
{
	int stack_size = s.size();

	for (int i = 0; i < stack_size; i++) {
		cout  <<  s.top() << " ";
		s.pop();
	}

	cout << endl;
}
template <typename T>
void stack_list_print(mini::stack<T, mini::list<T> >& s)
{
	int stack_size = s.size();
	for (int i = 0; i < stack_size; i++) {
		cout << s.top() << " ";
		s.pop();
	}
	cout << endl;
}
template <typename T>
void stack_vector_print(mini::stack<T, mini::vector<T> >& s)
{
	int stack_size = s.size();
	for (int i = 0; i < stack_size; i++) {
		cout << s.top() << " ";
		s.pop();
	}
	cout << endl;
}
int main_stack() {

	mini::stack<int> s;
	s.push(1);
	s.push(2);
	s.push(3);

	cout << "size = " << s.size() << endl;
	stack_deque_print(s);

	mini::stack<int, mini::list<int>> s1;
	s1.push(1);
	s1.push(2);
	s1.push(3);
	cout << "size = " << s1.size() << endl;
	stack_list_print(s1);

	mini::stack<int, mini::vector<int>> s2;
	s2.push(1);
	s2.push(2);
	s2.push(3);
	cout << "size = " << s2.size() << endl;
	stack_vector_print(s2);
	return 0;
}