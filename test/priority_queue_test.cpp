#include <iostream>
#include "../container/sequence container/queue.h"
using namespace std;
template <typename T>
void priority_print(mini::priority_queue<T>& ipq)
{
	while (!ipq.empty()) {
		cout << ipq.top() << " ";
		ipq.pop();
	}
	cout << endl;
}

int main_priority()
{
	mini::vector<int> iv{ 4, 8, 9, 7, 32, 12 };

	// priority_queue ���캯��������Ϊ heap
	mini::priority_queue<int> ipq(iv.begin(), iv.end());

	cout << "ipq size = " << ipq.size() << endl;

	priority_print(ipq); // �����������Ϊ��

	cout << "ipq size = " << ipq.size() << endl;

	// ������β�˲���Ԫ�أ������µ���
	ipq.push(1);
	ipq.push(100);
	ipq.push(12);
	ipq.push(44);

	cout << "ipq insert size = " << ipq.size() << endl;

	priority_print(ipq);
	return 0;
}