#include "pch.h"
#include "vector.h"
#include <vector>
#include <map>

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