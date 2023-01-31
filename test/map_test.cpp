#include <iostream>
#include "../container/associative container/map.h"

using namespace std;
template <typename T>
void map_print(T& m)
{
	cout << "{ ";
	for (auto& n : m) {
		cout << n.first << " : " << n.second << " ";
	}
	cout << "}\n";
}
int main_map() {

	mini::map<string, int> s;
	s[string("aa")] = 1;
	s[string("bb")] = 2;
	s[string("aa")] = 3;//更改数值
	mini::pair<const string, int> value("aa", 4);//插入失败
	s.insert(value);
	map_print(s);
	return 0;
}