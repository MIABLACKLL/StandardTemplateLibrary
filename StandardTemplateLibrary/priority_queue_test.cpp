#include <queue>
#include <iostream>


using namespace std;
int main()
{
	int array[] = { 233,0,1,2,3,4,5,6,7,8,9};
	priority_queue<int>pq(array, array + 10);//Ĭ�ϵײ�����vector,ʹ��heap��ʽ���

	cout << pq.size() << endl;

	cout << pq.top() << endl;

	pq.push(999);

	while (!pq.empty())
	{
		cout << pq.top() << " ";
		pq.pop();//heap�㷨����ʵ�ʽ�Ԫ�ص��������Ƿ���β������vector::pop_back()���𵯳�
	}
}