#include "pch.h"
#include "vector.h"
#include <vector>
#include <memory>
#include <iterator>

class a
{
public:
	int test;
	a(int x) :test(x) {}
};

TEST(TestCaseName, TestName) {
	std::allocator<a> al;
	a a1(233);
	auto pa = al.allocate(1);
	al.construct(pa, al);
	std::cout << pa->test;

	std::vector<int> v;
	std::iterator_traits<std::vector<int>::iterator>::iterator_category;
}