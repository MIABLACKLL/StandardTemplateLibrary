#include "pch.h"
#include "vector.h"
#include <vector>
#include <map>
#include <queue>

TEST(Test, TestVectorSizeInit) {
	MIA::Vector<int> MyVector(5);
	for (auto i : MyVector)
		EXPECT_EQ(i, 0);
}

TEST(Test, TestVectorSizeValueInit) {
	MIA::Vector<int> MyVector(5,10);
	for (auto i : MyVector)
		EXPECT_EQ(i, 10);
}

TEST(Test, TestVectorListInit) {
	MIA::Vector<int> MyVector = { 1,2,6,5,8 };
	EXPECT_EQ(MyVector[0], 1);
	EXPECT_EQ(MyVector[1], 2);
	EXPECT_EQ(MyVector[2], 6);
	EXPECT_EQ(MyVector[3], 5);
	EXPECT_EQ(MyVector[4], 8);
}

TEST(Test, TestVectorSelfVectorInit) {
	MIA::Vector<int> SelfVector = { 1,2,6,5,8 };
	MIA::Vector<int> MyVector = SelfVector;
	EXPECT_EQ(MyVector[0], 1);
	EXPECT_EQ(MyVector[1], 2);
	EXPECT_EQ(MyVector[2], 6);
	EXPECT_EQ(MyVector[3], 5);
	EXPECT_EQ(MyVector[4], 8);
}

TEST(Test, TestVectorIteratorInit) {
	std::vector<int> SelfVector = { 1,2,6,5,8 };
	MIA::Vector<int> MyVector(SelfVector.begin(),SelfVector.end());
	EXPECT_EQ(MyVector[0], 1);
	EXPECT_EQ(MyVector[1], 2);
	EXPECT_EQ(MyVector[2], 6);
	EXPECT_EQ(MyVector[3], 5);
	EXPECT_EQ(MyVector[4], 8);
}

TEST(Test, TestPushBack) {
	MIA::Vector<int> MyVector;
	MyVector.push_back(1);
	EXPECT_EQ(MyVector[0], 1);
	MyVector.push_back(2);
	EXPECT_EQ(MyVector[1], 2);
	MyVector.push_back(6);
	EXPECT_EQ(MyVector[2], 6);
	MyVector.push_back(5);
	EXPECT_EQ(MyVector[3], 5);
	MyVector.push_back(8);
	EXPECT_EQ(MyVector[4], 8);
}

TEST(Test, TestPopBack) {
	MIA::Vector<int> MyVector = { 1,2,6,5,8 };
	EXPECT_EQ(MyVector.back(), 8);
	MyVector.pop_back();
	EXPECT_EQ(MyVector.back(), 5);
	MyVector.pop_back();
	EXPECT_EQ(MyVector.back(), 6);
	MyVector.pop_back();
	EXPECT_EQ(MyVector.back(), 2);
	MyVector.pop_back();
	EXPECT_EQ(MyVector.back(), 1);
}

TEST(Test, TestEraseElement) {
	MIA::Vector<int> MyVector = { 1,2,6,5,8 };
	auto it = std::find(MyVector.begin(), MyVector.end(), 6);
	MyVector.erase(it);
	EXPECT_EQ(MyVector[0], 1);
	EXPECT_EQ(MyVector[1], 2);
	EXPECT_EQ(MyVector[2], 5);
	EXPECT_EQ(MyVector[3], 8);
}

TEST(Test, TestClear) {
	MIA::Vector<int> MyVector = { 1,2,6,5,8 };
	MyVector.clear();
	EXPECT_EQ(MyVector.begin(), MyVector.end());
}

TEST(Test, TestInsertElement) {
	MIA::Vector<int> MyVector = { 1,2,6,5,8 };
	auto it = std::find(MyVector.begin(), MyVector.end(), 6);
	MyVector.insert(it,999);
	EXPECT_EQ(MyVector[0], 1);
	EXPECT_EQ(MyVector[1], 2);
	EXPECT_EQ(MyVector[2], 999);
	EXPECT_EQ(MyVector[3], 6);
	EXPECT_EQ(MyVector[4], 5);
	EXPECT_EQ(MyVector[5], 8);
}

TEST(Test, TestInsertMultElements) {
	MIA::Vector<int> MyVector = { 1,2,6,5,8 };
	auto it = std::find(MyVector.begin(), MyVector.end(), 6);
	MyVector.insert(it, 3, 999);
	EXPECT_EQ(MyVector[0], 1);
	EXPECT_EQ(MyVector[1], 2);
	EXPECT_EQ(MyVector[2], 999);
	EXPECT_EQ(MyVector[3], 999);
	EXPECT_EQ(MyVector[4], 999);
	EXPECT_EQ(MyVector[5], 6);
	EXPECT_EQ(MyVector[6], 5);
	EXPECT_EQ(MyVector[7], 8);
}

TEST(Test, TestInsertList) {
	MIA::Vector<int> MyVector = { 1,2,6,5,8 };
	auto it = std::find(MyVector.begin(), MyVector.end(), 6);
	MyVector.insert(it, { 997,998,999 });
	EXPECT_EQ(MyVector[0], 1);
	EXPECT_EQ(MyVector[1], 2);
	EXPECT_EQ(MyVector[2], 997);
	EXPECT_EQ(MyVector[3], 998);
	EXPECT_EQ(MyVector[4], 999);
	EXPECT_EQ(MyVector[5], 6);
	EXPECT_EQ(MyVector[6], 5);
	EXPECT_EQ(MyVector[7], 8);
}

TEST(Test, TestInsertVector) {
	std::vector<int> SelfVector = { 997,998,999 };
	MIA::Vector<int> MyVector = { 1,2,6,5,8 };
	auto it = std::find(MyVector.begin(), MyVector.end(), 6);
	MyVector.insert(it, SelfVector.begin(), SelfVector.end());
	EXPECT_EQ(MyVector[0], 1);
	EXPECT_EQ(MyVector[1], 2);
	EXPECT_EQ(MyVector[2], 997);
	EXPECT_EQ(MyVector[3], 998);
	EXPECT_EQ(MyVector[4], 999);
	EXPECT_EQ(MyVector[5], 6);
	EXPECT_EQ(MyVector[6], 5);
	EXPECT_EQ(MyVector[7], 8);
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	return 0;
}
