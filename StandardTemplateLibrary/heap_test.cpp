#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;
int main()
{
	int array[] = { 0,1,2,3,4,5,6,7,8,9 };
	vector<int> vt(array, array + 9);

	make_heap(vt.begin(), vt.end());

	for (int i : vt)
		cout << i << " ";
	cout << endl;

	vt.push_back(999);

	push_heap(vt.begin(), vt.end());

	for (int i : vt)
		cout << i << " ";
	cout << endl;

	pop_heap(vt.begin(), vt.end());
	for (int i : vt)
		cout << i << " ";
	cout << endl;
	vt.pop_back();

	sort_heap(vt.begin(), vt.end());
	for (int i : vt)
		cout << i << " ";
	cout << endl;
}