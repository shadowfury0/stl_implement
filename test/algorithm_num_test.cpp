#include <iostream>
#include "../algorithm/algo.h"
#include "../algorithm/numeric.h"
#include "../container/sequence container/vector.h"
using namespace std;


int main_alg_num() {
	mini::vector<int> iv{ 1,2,3,4,5 };
	cout << mini::accumulate(iv.begin(), iv.end(), 0) << endl;//15
	cout << mini::accumulate(iv.begin(), iv.end(), 0 , minus<int>()) << endl;//-15
	cout << mini::inner_product(iv.begin(), iv.end(), iv.begin(), 10) << endl;//65
	//迭代器绑定到输出
	ostream_iterator<int> oite(cout, " ");
	mini::partial_sum(iv.begin(), iv.end(),oite);// 1,3,6,10,15
	cout << endl;
	mini::adjacent_difference(iv.begin(), iv.end(), oite);//1 ,1 ,1, 1,1
	cout << endl;
	
	//100
	cout << mini::power(10, 3) << endl;//默认是幂运算
	//只能写加或者乘
	cout << mini::power(10, 3, mini::plus<int>()) << endl;//30


	return 0;
}