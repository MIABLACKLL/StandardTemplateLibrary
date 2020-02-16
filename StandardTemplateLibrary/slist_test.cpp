#include <slist>
#include <iostream>

using namespace std;
//�汾���ϣ����벻��
//slistΪ��״�ṹ��ֻ֧�� Forward Iterator
//listΪ��״�ṹ��֧�� Bidirectional Iterator
int main()
{
	slist<int> sl;
	cout << sl.size() << endl;

	sl.push_front(99);
	sl.push_front(233);
	sl.push_front(2);
	sl.push_front(1111);
	sl.push_front(0);

	cout << sl.size() << endl;

	slist<int>::iterator itb = sl.begin();
	slist<int>::iterator ite = sl.end();

	for (; itb != ite; itb++)
		cout << *itb << " ";

	itb = find(sl.begin(), sl.end(), 233);
	if (itb != sl.end())
		sl.insert(itb, 666);

	for (; itb != ite; itb++)
		cout << *itb << " ";

	sl.pop_front();
	sl.pop_front();
	sl.erase(itb);

	for (; itb != ite; itb++)
		cout << *itb << " ";
}