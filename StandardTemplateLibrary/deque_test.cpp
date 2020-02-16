#include <deque>
#include <iostream>
#include <algorithm>

using namespace std;

//deque双口序列容器，通过map(不是STL map)中控器维护多个缓冲区（目前版本公共接口似乎无法设置单个缓冲区大小，老版本SGI_STL支持）
//前后插入的效率都是常数
//由于迭代器涉及缓冲区之间的切换，排序时应复制到vector中排序再复制回deque

int main()
{
	deque<int> deq(20, 9);//已无法像样例中设置单个缓冲区大小

	cout << deq.size() << endl;

	for (int i = 0; i < deq.size(); i++)
		deq[i] = i;//支持下标遍历

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