#include <iostream>

#include "../container/sequence container/vector.h"
#include "../container/sequence container/heap.h"
using namespace std;
template<class T>
void heap_print(mini::vector<T>& ivec) {
	for (int i = 0; i < ivec.size(); i++) {
		cout << ivec[i] << " ";
	}
	cout << endl;
}

int main_heap() {
	mini::vector<int> iv{1,2,3,4,5,6,7};
	//�Ƚ���
	_Com<int> com(false);
	//make��sort���������������Ҫһ��
	mini::make_heap(iv.begin(), iv.end(),com);
	//һ��Ҫ��������
	mini::sort_heap(iv.begin(),iv.end(),com);
	heap_print(iv);

	// heap �ײ�ʹ�� array ����
	//int arr[] = { 12, 34, 15, 63, 17 };
	//mini::make_heap(arr, arr + 5);  // array ��Сһ���������� push_back ����
	//mini::sort_heap(arr, arr + 5);
	//size_t arr_size = sizeof(arr) / sizeof(arr[0]);
	//for (size_t i = 0; i < arr_size; i++) {
	//	cout << arr[i] << " ";
	//}
	//cout << endl;
	return 0;
}