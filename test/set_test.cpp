#include <iostream>
#include "../container/associative container/set.h"
using namespace std;

template <typename T>
void set_print(mini::set<T>& s)
{
	auto t = s.begin();
	/*auto e = s.end();
	for (; t != e; ++t) {
		cout << *t << " ";
	}*/
	int n = s.size();
	while (n--)
	{
		cout << *t << "  ";
		++t;
	}
	cout << endl;
}
int main_set()
{
	// Ä¬ÈÏ³õÊ¼»¯
	mini::set<int> i;
	i.insert(8);
	i.insert(2);
	i.insert(5);
	i.insert(6);
	i.erase(5);
	set_print(i);
	
	mini::set<string> str{ "asd","sad","fff","ddd" };
	set_print(str);
	cout << str.size() << endl;
	return 0;
}