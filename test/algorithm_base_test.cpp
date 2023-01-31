#include <iostream>
#include "../algorithm/algobase.h"
#include "../container/sequence container/list.h"
using namespace std;

int main_algorithm_base() {

	//��debug��ʽ���ж�����Դִ��·��
	//����copy����
	const char ccs[5] = { 'a','b','c','d','e' };
	char ccd[5];
	mini::copy(ccs, ccs + 5, ccd);
	for (int i = 0; i < 5; ++i)
		cout << ccd[i] << ' ';
	cout << endl;

	//������Ҫ����һ�±������Ż�,��Ȼ���ܻ���char·��
	const wchar_t cwcs[5] = { 'a','b','c','d','e' };
	wchar_t cwcd[5];
	mini::copy(cwcs, cwcs + 5, cwcd);
	for (int i = 0; i < 5; ++i)
		cout << cwcd[i] << ' ';
	cout << endl;

	int ia[5] = { 0,1,2,3,4 };
	mini::copy(ia,ia+5,ia);//������ô����
	for (int i = 0; i < 5; ++i)
		cout << ia[i] << ' ';
	cout << endl;

	mini::list<int> il1{1,2,3,4,5};
	mini::list<int> il2(5,0);
	mini::copy(il1.begin(), il1.end(), il2.begin());
	for (auto i = il2.begin(); i != il2.end();++i) {
		cout << *i << ' ';
	}
	cout << endl;
	//���򿽱�����ԭ����ͬ��ʱ����
	return 0;
}