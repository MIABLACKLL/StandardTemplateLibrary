#include <deque>
#include <iostream>
#include <algorithm>

using namespace std;

//deque˫������������ͨ��map(����STL map)�п���ά�������������Ŀǰ�汾�����ӿ��ƺ��޷����õ�����������С���ϰ汾SGI_STL֧�֣�
//ǰ������Ч�ʶ��ǳ���
//���ڵ������漰������֮����л�������ʱӦ���Ƶ�vector�������ٸ��ƻ�deque

int main()
{
	deque<int> deq(20, 9);//���޷������������õ�����������С

	cout << deq.size() << endl;

	for (int i = 0; i < deq.size(); i++)
		deq[i] = i;//֧���±����

	for (int i = 0; i < 3; i++)
		deq.push_back(i);

	deq.push_front(23333);

	for (int i = 0; i < deq.size(); i++)
		cout << deq[i] << " ";
	cout << endl;

	cout << deq.size() << endl;

	deq.push_front(666);
	deq.push_front(667);

	for (int i = 0; i < deq.size(); i++)
		cout << deq[i] << " ";
	cout << endl;

	deque<int>::iterator it;
	it = find(deq.begin(), deq.end(), 23333);
	cout << *it << endl;

	cout << deq.max_size() << endl;
}