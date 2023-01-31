#include <iostream>
#include "../algorithm/algo.h"
#include "../container/associative container/set.h"
#include "../container/sequence container/vector.h"
using namespace std;

template<class T>
struct display {//չʾ
	void operator()(const T& x) const {
		cout << x << ' ';
	}
};
//ģ2
struct even {
	bool operator()(int x)const {
		return x % 2 ? false : true;
;	}
};
//��2
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
	//�ҳ�iv����Ԫ����ȵĵ�һ��Ԫ��
	cout << *mini::adjacent_find(iv.begin(), iv.end(), mini::equal_to<int>()) << endl;//6
	//�ҳ�6�ĸ���
	cout << mini::count(iv.begin(),iv.end(),6) << endl;//3

	//�ҳ�iv��С��7��Ԫ�ظ���
	cout << mini::count_if(iv.begin(), iv.end(), mini::bind2nd(mini::less<int>(), 7)) << endl;
	//9
	//�ҵ���һ��4Ԫ�ص�λ��
	cout << *mini::find(iv.begin(), iv.end(), 4) << endl;//4
	//�ҵ�����2�ĵ�һ��Ԫ�ص�λ��
	cout << *mini::find_if(iv.begin(), iv.end(), mini::bind2nd(mini::greater<int>(), 2)) << endl;
	//3

	//�ҳ�������iv2�����ֵ����һ��λ��
	mini::vector<int> iv2(ia + 6, ia + 8);
	cout << *(mini::find_end(iv.begin(), iv.end(), iv2.begin(), iv2.end())+3) << endl;//8

	//�ҵ������г��ֵĵ�һ��λ��
	cout << *mini::find_first_of(iv.begin(), iv.end(), iv2.begin(), iv2.end())<<endl;//6

	//����
	mini::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl;//0 1 2 3 4 5 6 6 6 7 8

	//��������iv2���䣬��ÿһ��Ԫ�ؽ���even by two ����
	mini::generate(iv2.begin(), iv2.end(), even_by_two());//2 4
	mini::for_each(iv2.begin(), iv2.end(), display<int>());
	cout << endl;

	mini::generate_n(iv.begin(),3, even_by_two());//2 4
	mini::for_each(iv.begin(), iv.end(), display<int>()); //6 8 10 3 4 5 6 6 6 7 8
	cout << endl;
	//ɾ�����ǲ�ɾ��Ԫ��6 �������ݱ�����ĩβ
	mini::remove(iv.begin(), iv.end(), 6);
	mini::for_each(iv.begin(), iv.end(), display<int>());//8 10 3 4 5 7 8  �������� 6 6 7 8 
	cout << endl;
	mini::vector<int> iv3(12);
	mini::remove_copy(iv.begin(),iv.end(),iv3.begin(),6);
	mini::for_each(iv3.begin(), iv3.end(),display<int>());// 8 10 3 4 5 7 8 7 8 �������� 0 0 0
	cout << endl;

	mini::remove_if(iv.begin(), iv.end(),mini::bind2nd(mini::less<int>(), 6));
	mini::for_each(iv.begin(), iv.end(),display<int>());//8 10 7 8 6 6 7 8 �������� 6 7 8
	cout << endl;

	mini::remove_copy_if(iv.begin(), iv.end(),iv3.begin(), mini::bind2nd(mini::less<int>(), 7));
	mini::for_each(iv3.begin(), iv3.end(), display<int>());//8 10 7 8 7 8 7 8 8 �������� 0 0 0
	cout << endl;

	//��6 �滻��3
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
	// �������������� 30,30 �����ص�һ��30
	cout << *mini::search_n(iv4.begin(), iv4.end(), 2, 30) << endl;//30
	// ��������������С�ڵ���20���� �����ص�һ����
	cout << *mini::search_n(iv4.begin(), iv4.end(), 2, 20,mini::less_equal<int>()) 
		<< endl;//10
	cout << "---------------------------------------------------" << endl;
	mini::vector<int> f1(5, 10);        // foo: 10 10 10 10 10
	mini::vector<int> f2(5, 33);        // bar: 33 33 33 33 33

	mini::swap_ranges(f1.begin(), f1.end(),f2.begin());//10 10 10 10 10
	//��������f2����
	mini::for_each(f2.begin(), f2.end(), display<int>());
	cout << endl;
	cout << "---------------------------------------------------" << endl;

	mini::vector<int> iv5 = { 1,2,3,4,5,6 };
	//Ԫ��ȫ����2
	mini::transform(iv5.begin(), iv5.end(), iv5.begin(), mini::bind2nd(mini::minus<int>(), 2));
	mini::for_each(iv5.begin(), iv5.end(), display<int>());
	cout << endl;//-1 0 1 2 3 4
	cout << "---------------------------------------------------" << endl;
	mini::vector<int> iv61{0,1,2,3,4,5,6,6,6,7,8};
	mini::vector<int> iv62{0,4,5,6,6,7};
	cout << *mini::max_element(iv61.begin(), iv61.end()) << endl;//8
	cout << *mini::min_element(iv61.begin(), iv61.end()) << endl;//0
	//�ж�Ԫ���Ƿ񶼰���
	cout << mini::includes(iv61.begin(), iv61.end(), iv62.begin(), iv62.end()) << endl;//1
	//����(����ż)
	mini::partition(iv61.begin(), iv61.end(), even());
	mini::for_each(iv61.begin(), iv61.end(), display<int>());
	cout << endl;//0 8 2 6 4 6 6 5 3 7 1
	//ȥ��
	mini::unique(iv61.begin(), iv61.end());
	mini::for_each(iv61.begin(), iv61.end(), display<int>());
	cout << endl;//0 8 2 6 4 1 6 5 3 7 1



	return 0;
}
int main_algo_set() {
	//��vector����Ҳ�ǿ��Ե�,����Ҫ�ź���
	mini::set<int> s1{ 1,3,5,7,9 };
	mini::set<int> s2{ 1,1,2,3,5,8,13 };
	mini::set<int> s3{1,2,3,4,5,6,7,8};//��ʼ��һ���Ŀռ�,�е㱿
	//����
	//��һ�ִ�ӡ��ʽ
	mini::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),
		ostream_iterator<int>(cout," "));
	//1 2 3 5 7 8 9 13
	cout << endl;
	//�ڶ��ִ�ӡ��ʽ
	mini::set<int>::iterator it;
	it = mini::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(),s3.begin());
	
	for (auto i : s3)
		cout << i << " ";
	cout << endl;
	//����
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