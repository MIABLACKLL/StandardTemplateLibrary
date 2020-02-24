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
	a(a& x) :test(std::move(x.test)) { std::cout << "&move" << std::endl; }
	a(a&& x) :test(std::move(x.test)) { std::cout << "&&move" << std::endl; }
	a& operator=(const a& other) = default;
};

template<class ...a>
void testa(a&&... v)
{
	std::cout << "true" << std::endl;
}

TEST(TestCaseName, TestName) {
	std::allocator<a> al;
	a ap(233);
	const a& ra = ap;
	a newa(ap);
	testa(ra);
	auto pa = al.allocate(1);
	al.construct(pa, std::move(ap));
	std::cout << pa->test;

	std::vector<int> aaa;

}