#include <iostream>

#include "../container/sequence container/vector.h"
using namespace std;
template<class T>
void vec_print1(const mini::vector<T>& vec) {
	for (auto iter = vec.begin(); iter != vec.end();iter++) {
		std::cout << *iter << " ";
	}
	std::cout << std::endl;
}
int main_vector() {
	//初始化vector
	mini::vector<char> ch;
	mini::vector<std::string> str;
	struct Tmp{};
	mini::vector<Tmp> t;
	//列表初始化
	mini::vector<int> hc{ 4,3,6,7,9 };
	vec_print1(hc);

	//初始化vector
	mini::vector<int> first ;                               // empty vector of ints
	mini::vector<int> second(4, 100);                       // four ints with value 100
	mini::vector<int> third(second.begin(), second.end()-1);// iterating through second
	mini::vector<int> fourth(third);						// a copy of third
	mini::vector<int> fifith = third;						// = copy third
	int tmp_arry[5] = { 10,20,30,40,50 };
	//这个单独拿出来作比较
	mini::vector<int> sixth;
	sixth.assign(tmp_arry, tmp_arry + 5);// = arry copy
	//打印输出
	vec_print1(first);//nNULL
	vec_print1(second);
	vec_print1(third);
	vec_print1(fourth);
	vec_print1(fifith);
	vec_print1(sixth);
	cout << "-------------------------------------------------------------------------" << endl;
	//pop_back,push_back测试
	first.pop_back();//error
	first.push_back(2);
	first.push_back(3);
	first.push_back(5);
	vec_print1(first);
	first[1] = 2;//通过数组方式进行更改
	vec_print1(first);
	cout << "-------------------------------------------------------------------------" << endl;
	//基本函数测试
	cout << "begin : " << first.begin() << endl;
	cout <<  "end : " << first.end() << endl;
	cout <<  "cbegin : " << first.begin() << endl;
	cout <<  "cend : " << first.end() << endl;
	cout <<  "capacity :  "  << first.capacity() << endl;
	cout <<  "size : " <<first.size() << endl;
	cout <<  "front : " <<first.front() << endl;
	cout <<  "back : " <<first.back() << endl;
	cout << "-------------------------------------------------------------------------" << endl;
	mini::vector<int> ins;
	//insert函数场景测试
	ins.push_back(1);ins.push_back(2);
	ins.push_back(3);ins.push_back(4);
	ins.push_back(5);ins.push_back(6);
	cout << ins.capacity() << endl;//容量为8
	cout << ins.size() << endl;	   //大小为8
	cout << "statement  1  : ------------------------------------" << endl;
	//情况一、
	//空间足够情况
	//在第4个位置插入2个0。
	ins.insert(ins.begin()+3,2,0);
	vec_print1(ins);
	//删除从[4,6)位置上的元素
	ins.erase(ins.begin()+3,ins.begin()+5);
	vec_print1(ins);
	cout << "statement  2  : ------------------------------------" << endl;
	//情况二、
	ins.erase(ins.end()-1);
	vec_print1(ins);
	ins.insert(ins.begin() + 3, 3, 0);
	vec_print1(ins);
	//情况三、其实一开始内存不够的时候就已经测试过了。
	ins.clear();
	
	return 0;
}

