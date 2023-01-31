#include <iostream>
#include "../container/sequence container/queue.h"
#include "../container/sequence container/list.h"
using namespace std;
template <typename T>
void queue_deque_print(mini::queue<T>& s)
{
	int queue_size = s.size();

	for (int i = 0; i < queue_size; i++) {
		cout << s.front() << " ";
		s.pop();
	}
	cout << endl;
}
template <typename T>
void queue_list_print(mini::queue<T, mini::list<T> >& s)
{
	int queue_size = s.size();
	for (int i = 0; i < queue_size; i++) {
		cout << s.front() << " ";
		s.pop();
	}
	cout << endl;
}


int main_queue() {
	// Ä¬ÈÏÒÔ queue Îªµ×²ãÈÝÆ÷
	mini::queue<int> q;

	q.push(1);
	q.push(2);
	q.push(3);

	cout << "size = " << q.size() << endl;

	queue_deque_print(q);

	// ÒÔ list Îªµ×²ãÈÝÆ÷
	mini::queue<string, mini::list<string> > iq;

	iq.push("why");
	iq.push("always");
	iq.push("me");

	cout << "iq size = " << iq.size() << endl;
	queue_list_print(iq);

	return 0;
}