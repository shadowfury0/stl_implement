#include <iostream>
#include "../algorithm/algo.h"
#include "../container/associative container/set.h"
#include "../container/sequence container/vector.h"
using namespace std;

template<class T>
struct display {//展示
	void operator()(const T& x) const {
		cout << x << ' ';
	}
};
//模2
struct even {
	bool operator()(int x)const {
		return x % 2 ? false : true;
;	}
};
//加2
class even_by_two {
public:
	int operator()()const {
		return x += 2;
	}
private:
	static int x ;
};
int even_by_two::x = 0;

int main_algo_base() {

	int ia[] = { 0,1,2,3,4,5,6,6,6,7,8 };
	mini::vector<int> iv(ia, ia + sizeof(ia) / sizeof(int));
	//找出iv相邻元素相等的第一个元素
	cout << *mini::adjacent_find(iv.begin(), iv.end(), mini::equal_to<int>()) << endl;//6
	//找出6的个数
	cout << mini::count(iv.begin(),iv.end(),6) << endl;//3

	//找出iv中小于7的元素个数
	cout << mini::count_if(iv.begin(), iv.end(), mini::bind2nd(mini::less<int>(), 7)) << endl;
	//9
	//找到第一个4元素的位置
	cout << *mini::find(iv.begin(), iv.end(), 4) << endl;//4
	//找到大于2的第一个元素的位置
	cout << *mini::find_if(iv.begin(), iv.end(), mini::bind2nd(mini::greater<int>(), 2)) << endl;
	//3

	//找出子序列iv2所出现的最后一个位置
	mini::vector<int> iv2(ia + 6, ia + 8);
	cout << *(mini::find_end(iv.begin(), iv.end(), iv2.begin(), iv2.end())+3) << endl;//8

	//找到子序列出现的第一个位置
	cout << *mini::find_first_of(iv.begin(), iv.end(), iv2.begin(), iv2.end())<<endl;//6

	//遍历
	mini::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl;//0 1 2 3 4 5 6 6 6 7 8

	//编译整个iv2区间，对每一个元素进行even by two 操作
	mini::generate(iv2.begin(), iv2.end(), even_by_two());//2 4
	mini::for_each(iv2.begin(), iv2.end(), display<int>());
	cout << endl;

	mini::generate_n(iv.begin(),3, even_by_two());//2 4
	mini::for_each(iv.begin(), iv.end(), display<int>()); //6 8 10 3 4 5 6 6 6 7 8
	cout << endl;
	//删除但是不删除元素6 其余数据保留到末尾
	mini::remove(iv.begin(), iv.end(), 6);
	mini::for_each(iv.begin(), iv.end(), display<int>());//8 10 3 4 5 7 8  残余数据 6 6 7 8 
	cout << endl;
	mini::vector<int> iv3(12);
	mini::remove_copy(iv.begin(),iv.end(),iv3.begin(),6);
	mini::for_each(iv3.begin(), iv3.end(),display<int>());// 8 10 3 4 5 7 8 7 8 残余数据 0 0 0
	cout << endl;

	mini::remove_if(iv.begin(), iv.end(),mini::bind2nd(mini::less<int>(), 6));
	mini::for_each(iv.begin(), iv.end(),display<int>());//8 10 7 8 6 6 7 8 残余数据 6 7 8
	cout << endl;

	mini::remove_copy_if(iv.begin(), iv.end(),iv3.begin(), mini::bind2nd(mini::less<int>(), 7));
	mini::for_each(iv3.begin(), iv3.end(), display<int>());//8 10 7 8 7 8 7 8 8 残余数据 0 0 0
	cout << endl;

	//将6 替换成3
	mini::replace(iv.begin(), iv.end(), 6, 3);
	mini::for_each(iv.begin(), iv.end(), display<int>());//8 10 7 8 3 3 7 8 3 7 8
	cout << endl;

	mini::replace_copy(iv.begin(), iv.end(), iv3.begin(), 3, 5);
	mini::for_each(iv3.begin(), iv3.end(), display<int>());//8 10 7 8 5 5 7 8 5 7 8 0
	cout << endl;

	mini::replace_if(iv.begin(), iv.end(), mini::bind2nd(mini::less<int>(), 5), 2);
	mini::for_each(iv.begin(), iv.end(), display<int>());// 8 10 7 8 2 2 7 8 2 7 8
	cout << endl;

	mini::replace_copy_if(iv.begin(), iv.end(), iv3.begin(),mini::bind2nd(mini::equal_to<int>(),8),9);
	mini::for_each(iv3.begin(), iv3.end(), display<int>());//9 10 7 9 2 2 7 9 2 7 9 0
	cout << endl;

	mini::reverse(iv.begin(), iv.end());
	mini::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl;//8 7 2 8 7 2 2 8 7 10 8

	mini::reverse_copy(iv.begin(), iv.end(),iv3.begin());
	mini::for_each(iv3.begin(), iv3.end(), display<int>());
	cout << endl;//8 10 7 8 2 2 7 8 2 7 8 0

	cout << "---------------------------------------------------" << endl;
	int ia2[] = { 10,20,30,30,20,10,10,20 };
	mini::vector<int> iv4(ia2, ia2 + 8);

	cout << *mini::search(iv4.begin(),iv4.end(),ia2+2,ia2+4) << endl;//30
	// 查找两个连续的 30,30 并返回第一个30
	cout << *mini::search_n(iv4.begin(), iv4.end(), 2, 30) << endl;//30
	// 查找两个连续的小于等于20的数 并返回第一个数
	cout << *mini::search_n(iv4.begin(), iv4.end(), 2, 20,mini::less_equal<int>()) 
		<< endl;//10
	cout << "---------------------------------------------------" << endl;
	mini::vector<int> f1(5, 10);        // foo: 10 10 10 10 10
	mini::vector<int> f2(5, 33);        // bar: 33 33 33 33 33

	mini::swap_ranges(f1.begin(), f1.end(),f2.begin());//10 10 10 10 10
	//交换所有f2数据
	mini::for_each(f2.begin(), f2.end(), display<int>());
	cout << endl;
	cout << "---------------------------------------------------" << endl;

	mini::vector<int> iv5 = { 1,2,3,4,5,6 };
	//元素全部减2
	mini::transform(iv5.begin(), iv5.end(), iv5.begin(), mini::bind2nd(mini::minus<int>(), 2));
	mini::for_each(iv5.begin(), iv5.end(), display<int>());
	cout << endl;//-1 0 1 2 3 4
	cout << "---------------------------------------------------" << endl;
	mini::vector<int> iv61{0,1,2,3,4,5,6,6,6,7,8};
	mini::vector<int> iv62{0,4,5,6,6,7};
	cout << *mini::max_element(iv61.begin(), iv61.end()) << endl;//8
	cout << *mini::min_element(iv61.begin(), iv61.end()) << endl;//0
	//判断元素是否都包含
	cout << mini::includes(iv61.begin(), iv61.end(), iv62.begin(), iv62.end()) << endl;//1
	//分区(按奇偶)
	mini::partition(iv61.begin(), iv61.end(), even());
	mini::for_each(iv61.begin(), iv61.end(), display<int>());
	cout << endl;//0 8 2 6 4 6 6 5 3 7 1
	//去重
	mini::unique(iv61.begin(), iv61.end());
	mini::for_each(iv61.begin(), iv61.end(), display<int>());
	cout << endl;//0 8 2 6 4 1 6 5 3 7 1



	return 0;
}
int main_algo_set() {
	//用vector数组也是可以的,但是要排好序
	mini::set<int> s1{ 1,3,5,7,9 };
	mini::set<int> s2{ 1,1,2,3,5,8,13 };
	mini::set<int> s3{1,2,3,4,5,6,7,8};//初始化一定的空间,有点笨
	//并集
	//第一种打印方式
	mini::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),
		ostream_iterator<int>(cout," "));
	//1 2 3 5 7 8 9 13
	cout << endl;
	//第二种打印方式
	mini::set<int>::iterator it;
	it = mini::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),s3.begin());
	
	for (auto i : s3)
		cout << i << " ";
	cout << endl;
	//交集
	mini::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
		ostream_iterator<int>(cout, " "));
	//1 3 5
	cout << endl;
	mini::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
		ostream_iterator<int>(cout, " "));
	//7 9
	cout << endl;

	mini::set_symmertric_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
		ostream_iterator<int>(cout, " "));
	cout << endl;
	//2 7 8 9 13
	return 0;
}