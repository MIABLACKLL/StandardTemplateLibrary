#include <queue>
#include <iostream>


using namespace std;
int main()
{
	int array[] = { 233,0,1,2,3,4,5,6,7,8,9};
	priority_queue<int>pq(array, array + 10);//默认底部容器vector,使用heap隐式表达

	cout << pq.size() << endl;

	cout << pq.top() << endl;

	pq.push(999);

	while (!pq.empty())
	{
		cout << pq.top() << " ";
		pq.pop();//heap算法不会实际将元素弹出，而是放入尾部，由vector::pop_back()负责弹出
	}
}