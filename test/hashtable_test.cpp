#include <iostream>

#include "../container/associative container/hashtable.h"

using namespace std;
int main_hasht() {
	//hashtable 处理不了string类型
	mini::hashtable<int, int, mini::hash<int>, mini::identity<int>,
		mini::equal_to<int>, mini::alloc >
		iht(50,mini::hash<int>(),mini::equal_to<int>());
	iht.insert_unique(63);
	iht.insert_unique(10);
	iht.insert_unique(20);
	iht.insert_unique(30);
	iht.insert_unique(40);
	iht.insert_unique(50);
	iht.insert_unique(59);
	iht.insert_unique(2);
	iht.insert_equal(2);
	iht.insert_unique(55);
	cout << iht.size() << endl;
	cout << iht.bucket_count() << endl;

	auto ite = iht.begin();
	
	for (int i = 0; i < iht.size(); ++i, ++ite)
		cout << *ite << ' ';
	cout << endl;
	cout << " ------------------------------------------  " << endl;


	for (int i = 0; i < iht.bucket_count(); ++i) {
		int n = iht.elems_in_bucket(i);
		if (n != 0)
			cout << "bucket[" << i << "] has " << n << " elems. " << endl;
	}

	cout << *(iht.find(2)) << endl;
	cout << iht.count(2) << endl;//这里插入了两个2的键值

	//字符测试
	mini::hashtable<char*, char*, mini::hash<char*>, mini::identity<char*>,
		mini::equal_to<char*>, mini::alloc >
		str(50, mini::hash<char*>(), mini::equal_to<char*>());
	char a[5] = "aaaa";
	char b[5] = "bbbb";
	char c[5] = "cccc";
	str.insert_unique(a);
	str.insert_unique(b);
	str.insert_unique(c);
	auto ibe = str.begin();
	for (int i = 0; i < str.size(); ++i, ++ibe)
		cout << *ibe << ' ';
	cout << endl;
	//std::string 类型不能做相关的处理因为没有写相关的函数
	cout << " ------------------------------------------  " << endl;
	//一种特殊的用法
	mini::hashtable<int, int, mini::hash<int>, mini::identity<int>,
		mini::equal_to<int>, mini::alloc >
		tes(50, mini::hash<int>(), mini::equal_to<int>());
	int tmp[4] = { 10,20,30,40 };
	tes.insert_unique(tmp, tmp + 3);
	auto te = tes.begin();
	for (int i = 0; i < tes.size(); ++i, ++te) {
		cout << *te << ' ';
	}
	cout << endl;
	return 0;
}