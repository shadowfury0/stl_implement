#include <iostream>
#include "../container/associative container/hash_set.h"
using namespace std;

struct eqstr
{
	bool operator() (const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) == 0;
	}
};
void setFind(const mini::hash_set<const char*, mini::hash<const char*>, eqstr>& s, const char* word)
{
	mini::hash_set<const char*, mini::hash<const char*>, eqstr>::const_iterator it
		= s.find(word);
	cout << word << ": " << (it != s.end() ? "present" : "not present") << endl;
}
int main_hashset() {
	mini::hash_set<const char*, mini::hash<const char*>, eqstr> s;

	cout << s.size() << endl;

	s.insert("why");
	s.insert("always");
	s.insert("me");
	s.insert("star");
	s.insert("steve");
	cout << s.size() << endl;
	setFind(s, "always");
	setFind(s, "apple");
	mini::hash_set<const char*, mini::hash<const char*>, eqstr>::iterator it = s.begin();
	for (; it != s.end(); it++) {
		cout << *it << endl;
	}
	return 0;
}