#include "pch.h"
#include "vector.h"
#include <vector>
#include <map>

TEST(Test, TestVectorSizeInit) {
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