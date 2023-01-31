#include <iostream>
#include "../container/associative container/hash_map.h"
using namespace std;
//比较字符串
struct eqstr
{
	bool operator() (const char* s1, const char* s2) const
	{
		//等于0说明字符串相同
		return strcmp(s1, s2) == 0;
	}
};
int main_hashmap() {
	mini::hash_map<const char*, int, mini::hash<const char*>, eqstr> s;

	cout << s.size() << endl;

	s["why"] = 3;
	s["always"] = 4;
	s["me"] = 8;
	s["star"] = 9;
	s["steve"] = 18;

	cout << s.size() << endl;

	cout << "why -> " << s["why"] << endl;
	cout << "me -> " << s["me"] << endl;
	cout << "star -> " << s["star"] << endl;
	//mini::hash_map<const char*, int, mini::hash<const char*>, eqstr>::iterator
	auto it = s.begin();
	for (; it != s.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}

	return 0;
}